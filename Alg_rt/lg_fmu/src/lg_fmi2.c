/*
 * lg_fmi2.c — FMI 2.0 Co-Simulation entry points (Linux LegoCliSINC FMU)
 *
 * Linux counterpart of the Windows lego_fmu_core.c. Differences:
 *
 *   - No Win32 IPC: input/output go through libRt RtDbPGet/PutValue on
 *     SHM addrs, simulation cadence is driven by libdispatcher SD_goup.
 *   - No private SHM mapping: net_sked is already running on the task,
 *     net_startup must have been launched before fmi2Instantiate (the
 *     FMU just attaches via SHR_USR_KEY).
 *   - ValueReference == SHM addr (lg_var_info.addr). Stable across runs
 *     of the same model; one ScalarVariable per unique addr.
 *
 * State machine, callback semantics, and error handling follow the FMI
 * 2.0 spec (sections 2.1.6, 4.2). This skeleton implements the minimum
 * Co-Simulation lifecycle: Instantiate → SetupExperiment → EnterInit →
 * ExitInit → DoStep ... → Terminate → FreeInstance.
 *
 * Not yet implemented (return Discard or Error as appropriate):
 *   - Get/Set FMUstate, Serialize/DeSerializeFMUstate (canGetAndSetFMUstate=false)
 *   - DirectionalDerivative (providesDirectionalDerivative=false)
 *   - Real input derivatives, output derivatives
 *   - CancelStep
 *
 * The auto-launch of net_startup is intentionally out of scope for now;
 * see project_lg_fmu_linux memory, "Punto 3".
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <sqlite3.h>
#include <sim_param.h>
#include <sim_types.h>
#include <sim_ipc.h>
#include <sked.h>
#include <dispatcher.h>
#include <libdispatcher.h>
#include <Rt/RtErrore.h>
#include <Rt/RtDbPunti.h>

#include "fmi/headers/fmi2Functions.h"
#include "lg_fmi2_state.h"
#include "lg_var_mapping.h"

/* sender id used when posting commands to the dispatcher (same as
 * net_operator and probe_step). */
#define LG_DISPATCHER_SENDER  BI

/* poll cadence for waiting on time advance after SD_goup */
#define LG_GOUP_POLL_MS       20
/* Default 10 s; override con LG_FMU_STEP_TIMEOUT_S=<sec> per task grandi */
#define LG_GOUP_MAX_POLLS_DEFAULT  500

/* poll cadence for waiting on sim startup (Punto 4) */
#define LG_BOOT_POLL_MS       200
#define LG_BOOT_MAX_POLLS     150   /* 30 s totali */

/* poll cadence per attendere completamento init (sked_start + lg5sk spawn) */
#define LG_INIT_POLL_MS       200
#define LG_INIT_MAX_POLLS     150   /* 30 s totali */


/* ====================================================================
 * Helpers
 * ==================================================================== */

static void lg_log(lg_fmi2_instance *inst, fmi2Status status,
                   const char *category, const char *fmt, ...)
{
    if (!inst || !inst->loggingOn || !inst->callbacks.logger) return;

    char buf[1024];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);

    inst->callbacks.logger(inst->callbacks.componentEnvironment,
                           inst->instanceName, status, category, "%s", buf);
}

static void msleep(long ms)
{
    struct timespec ts;
    ts.tv_sec  = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000L;
    nanosleep(&ts, NULL);
}

/* Build the unique-addr index from lg_var_mapping. Returns 0 on success. */
static int build_var_index(lg_fmi2_instance *inst)
{
    int n = lg_var_unique_count(inst->vars);
    if (n <= 0) return -1;

    inst->unique_addrs = (int *)         calloc(n, sizeof(int));
    inst->is_input     = (unsigned char *)calloc(n, sizeof(unsigned char));
    if (!inst->unique_addrs || !inst->is_input) return -1;

    inst->n_unique = n;
    inst->n_inputs = 0;
    inst->n_outputs = 0;

    for (int k = 0; k < n; ++k) {
        int idx = lg_var_unique_at(inst->vars, k);
        lg_var_info info;
        if (lg_var_get_info(inst->vars, idx, &info) != 0) return -1;
        inst->unique_addrs[k] = info.addr;
        inst->is_input[k]     = (info.kind == LG_VAR_INPUT_FREE) ? 1 : 0;
        if (inst->is_input[k]) inst->n_inputs++;
        else                   inst->n_outputs++;
    }
    return 0;
}

/* Linear scan addr → index. Skeleton; could be a sorted/hashed lookup. */
static int find_unique_idx_by_addr(const lg_fmi2_instance *inst, int addr)
{
    for (int k = 0; k < inst->n_unique; ++k)
        if (inst->unique_addrs[k] == addr) return k;
    return -1;
}


/* ====================================================================
 * Autonomous mode (Punto 4): attach-or-launch della simulazione.
 *
 * fmi2Instantiate non puo' assumere che net_startup sia gia' stato
 * lanciato (Simulink/fmpy non lo sanno fare). Strategia:
 *
 *   1. Risolvi resourceLocation (file:///path/to/resources) → dir
 *   2. Leggi resources/task_info.env → TASK_PATH + LEGOROOT
 *   3. setenv LEGOROOT, OS, SHR_USR_KEY (= getuid()*10000 come Alg_env.sh)
 *      e PATH += $LEGOROOT/Alg_rt/bin
 *   4. chdir(TASK_PATH)
 *   5. Try RtCreateDbPunti → se OK, sim viva, attach (we_started_sim=0)
 *   6. Se fallisce, exec scripts/net_startup_headless.sh, poll fino al
 *      successo o timeout (we_started_sim=1)
 * ==================================================================== */

/* file:///abs/path  ->  /abs/path   (in-place, in buf di out_path) */
static int parse_resource_uri(const char *uri, char *out_path, size_t cap)
{
    if (!uri) return -1;
    const char *p = uri;
    if (strncmp(p, "file:///", 8) == 0) p += 7;        /* lascia uno '/' */
    else if (strncmp(p, "file://", 7) == 0) p += 7;
    else if (strncmp(p, "file:/", 6) == 0) p += 5;
    if (cap == 0) return -1;
    strncpy(out_path, p, cap - 1);
    out_path[cap - 1] = '\0';
    return 0;
}

/* Legge una riga "KEY=VAL" tipo .env. Skippa righe vuote e commenti #.
 * Cerca nel file `path` la chiave `key`, copia il valore in out (max cap-1).
 * Returns 0 ok, -1 not found / IO error. Stripping di whitespace finale. */
static int read_env_value(const char *path, const char *key,
                          char *out, size_t cap)
{
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    size_t klen = strlen(key);
    char line[1024];
    int found = 0;
    while (fgets(line, sizeof(line), f)) {
        char *p = line;
        while (*p == ' ' || *p == '\t') ++p;
        if (*p == '#' || *p == '\n' || *p == '\0') continue;
        if (strncmp(p, key, klen) != 0) continue;
        if (p[klen] != '=') continue;
        char *v = p + klen + 1;
        /* trim trailing whitespace/newline */
        size_t vlen = strlen(v);
        while (vlen > 0 && (v[vlen-1] == '\n' || v[vlen-1] == '\r' ||
                            v[vlen-1] == ' '  || v[vlen-1] == '\t'))
            v[--vlen] = '\0';
        strncpy(out, v, cap - 1);
        out[cap - 1] = '\0';
        found = 1;
        break;
    }
    fclose(f);
    return found ? 0 : -1;
}

/* Imposta env var SOLO se non gia' presenti (rispettare sovrascritture
 * deliberate del caller). Ricostruisce PATH per includere $LEGOROOT/Alg_rt/bin
 * (e in bundle mode anche $LEGOROOT/lego_big/bin per initav).
 * In bundle mode, setta SHR_TAV_KEY=999 e LD_LIBRARY_PATH=$LEGOROOT/lib
 * (sennò le shared libs bundlate in lib/ non vengono trovate dal loader). */
static void setup_legopst_env(const char *legoroot, int bundle_mode)
{
    if (!getenv("LEGOROOT")) setenv("LEGOROOT", legoroot, 0);
    if (!getenv("OS"))       setenv("OS", "Linux", 0);

    /* SHR_USR_KEY: in attach mode (sim viva) e' gia' in env del caller
     * (settata da .profile_legoroot/Alg_env.sh = uid*10000) -> non si tocca,
     * altrimenti la FMU non si attacca alla sim utente.
     *
     * In launch mode (no env, es. fmpy lancia la FMU da una shell pulita)
     * scegliamo una key che NON collida con:
     *   - la sessione LegoPST normale dell'utente (slot 0 = uid*10000,
     *     riservato ad Alg_env.sh);
     *   - altre FMU concorrenti dello stesso uid.
     * Probe degli slot 1..8 (= uid*10000 + slot*1100): per ogni slot
     * candidato, shmget(ID_SHM_VAR) torna ENOENT se libero. Primo slot libero
     * vince. Spazio uid*10000 = 10000 key, ogni istanza occupa ~1030 key
     * consecutive, slot 0 riservato -> max 8 FMU concorrenti per uid.
     * Nota: si usa ID_SHM_VAR (=5, topology DB) invece di ID_SHM_SIM (=0)
     * perché in modalità headless net_sked NON crea la SHM ID_SHM_SIM; l'unica
     * SHM garantita è quella creata da RtCreateDbPunti (ID_SHM_VAR=5).
     *
     * Caso uid=0 (container root): key=0 == IPC_PRIVATE in SysV, non
     * condivisibile -> fallback PID-based di P7-bis (single-instance ok). */
    if (!getenv("SHR_USR_KEY")) {
        int key = -1;
        uid_t uid = getuid();
        if (uid == 0) {
            key = (int)getpid() * 10 + 10000;
        } else {
            for (int slot = 1; slot < 9; slot++) {
                int candidate = (int)(uid * 10000) + slot * 1100;
                errno = 0;
                int id = shmget(candidate + ID_SHM_VAR, 0, 0);
                if (id == -1 && errno == ENOENT) { key = candidate; break; }
            }
            if (key == -1) {
                fprintf(stderr,
                    "[lg_fmu] tutti gli 8 slot SHR_USR_KEY per uid=%u sono "
                    "occupati: rilascia istanze precedenti (killsim) o attendi.\n",
                    (unsigned)uid);
                return;
            }
        }
        char buf[32];
        snprintf(buf, sizeof(buf), "%d", key);
        setenv("SHR_USR_KEY", buf, 0);
    }
    /* SHR_USR_KEYS = SHR_USR_KEY + 1000 (vedi Alg_env.sh:236). Senza questa
     * killsim fa exit(0) immediato e fmi2FreeInstance non pulisce nulla. */
    if (!getenv("SHR_USR_KEYS")) {
        char buf[32];
        snprintf(buf, sizeof(buf), "%d", atoi(getenv("SHR_USR_KEY")) + 1000);
        setenv("SHR_USR_KEYS", buf, 0);
    }

    /* PATH += $LEGOROOT/Alg_rt/bin (e lego_big/bin in bundle) */
    char rt_bin[LG_FMI2_PATH_MAX];
    snprintf(rt_bin, sizeof(rt_bin), "%s/Alg_rt/bin", legoroot);
    const char *cur = getenv("PATH");
    if (!cur || !strstr(cur, rt_bin)) {
        size_t need = strlen(rt_bin) + 2 + (cur ? strlen(cur) : 0);
        if (bundle_mode) need += strlen(legoroot) + 16; /* + ":/lego_big/bin" */
        char *new_path = (char *)malloc(need);
        if (new_path) {
            if (bundle_mode && cur && *cur)
                snprintf(new_path, need, "%s:%s/lego_big/bin:%s",
                         rt_bin, legoroot, cur);
            else if (bundle_mode)
                snprintf(new_path, need, "%s:%s/lego_big/bin",
                         rt_bin, legoroot);
            else if (cur && *cur)
                snprintf(new_path, need, "%s:%s", rt_bin, cur);
            else
                snprintf(new_path, need, "%s", rt_bin);
            setenv("PATH", new_path, 1);
            free(new_path);
        }
    }

    if (bundle_mode) {
        /* SHR_TAV_KEY: hardcoded 999 in Alg_env.sh:232 */
        if (!getenv("SHR_TAV_KEY")) setenv("SHR_TAV_KEY", "999", 0);

        /* LD_LIBRARY_PATH += $LEGOROOT/lib */
        char lib_dir[LG_FMI2_PATH_MAX];
        snprintf(lib_dir, sizeof(lib_dir), "%s/lib", legoroot);
        const char *cur_lp = getenv("LD_LIBRARY_PATH");
        if (!cur_lp || !strstr(cur_lp, lib_dir)) {
            size_t need = strlen(lib_dir) + 2 + (cur_lp ? strlen(cur_lp) : 0);
            char *np = (char *)malloc(need);
            if (np) {
                if (cur_lp && *cur_lp)
                    snprintf(np, need, "%s:%s", lib_dir, cur_lp);
                else
                    snprintf(np, need, "%s", lib_dir);
                setenv("LD_LIBRARY_PATH", np, 1);
                free(np);
            }
        }
    }
}

/* Se la sim e' in STATO_STOP (net_startup_headless lascia la sim ferma in
 * attesa di SKDIS_INITIALIZE, normalmente inviato dal banco operatore),
 * manda SD_inizializza(BI) e poll fino a transizione FREEZE/RUN o timeout.
 * Senza questo, il primo SD_goup di fmi2DoStep non avanza il tempo (sked in
 * stato STOP ignora GO_UP). Idempotente: se la sim e' gia' in FREEZE/RUN
 * (caso "sim gia' viva, attach-only" su sim avviata da banco), no-op. */
static int init_sim_if_stopped(lg_fmi2_instance *inst)
{
    int dbg = getenv("LG_FMU_DEBUG") != NULL;
    int stato = -1;

    RtDbPGetStato((RtDbPuntiOggetto)inst->dbpunti, &stato);
    if (dbg) fprintf(stderr, "[lg_fmu DBG] stato pre-init=%d\n", stato);
    if (stato != STATO_STOP) return 0;

    int rc = SD_inizializza(BI);
    if (dbg) fprintf(stderr, "[lg_fmu DBG] SD_inizializza(BI) -> %d\n", rc);
    if (rc <= 0) {
        lg_log(inst, fmi2Error, "logStatusError",
               "SD_inizializza(BI) fallita rc=%d", rc);
        return -1;
    }

    /* Poll: la transizione STOP -> FREEZE puo' richiedere qualche secondo
     * (es. modello collet inizializza ~246s di tempo simulato prima di
     * stabilizzarsi). 30s di budget con poll a 200ms. */
    for (int k = 0; k < 150; ++k) {
        msleep(200);
        RtDbPGetStato((RtDbPuntiOggetto)inst->dbpunti, &stato);
        if (stato != STATO_STOP) {
            if (dbg) fprintf(stderr,
                "[lg_fmu DBG] init OK dopo %d poll, stato=%d\n", k+1, stato);
            return 0;
        }
    }
    lg_log(inst, fmi2Error, "logStatusError",
           "timeout post-SD_inizializza, stato resta STOP");
    return -1;
}

/* try attach: on success, store dbpunti in inst->dbpunti and return 1.
 * Passiamo file_top="TEST" (non NULL): in RtDbPunti.c::InitializeDbPunti
 * questo seleziona condizione=2, che in caso di magic mismatch (sim non viva
 * o residui invalidati da killsim) si limita a sganciare la SHM in silenzio
 * invece di emettere "[error shared-memory not attached]" via RtShowErrore.
 * Per noi try_attach_db e' speculativo (polling pre-launch) e il fail e'
 * atteso: niente rumore. Se la sim e' viva, condizione=2 stampa "TEST:
 * SHARED AGGANCIATA" che e' informativo e non segnala errore. */
static int try_attach_db(lg_fmi2_instance *inst)
{
    if (!inst->errore)
        inst->errore = (lg_rt_errore_t)RtCreateErrore(
            RT_ERRORE_TERMINALE, (char *)inst->instanceName);
    inst->dbpunti = (lg_rt_dbpunti_t)RtCreateDbPunti(
        (RtErroreOggetto)inst->errore, "TEST", DB_PUNTI_INT, NULL);
    return inst->dbpunti ? 1 : 0;
}

static void msleep_lg(long ms)
{
    struct timespec ts;
    ts.tv_sec  = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000L;
    nanosleep(&ts, NULL);
}

/* exec headless launcher and poll fino a sim ready o timeout. */
static int launch_sim_and_wait(lg_fmi2_instance *inst)
{
    char script[LG_FMI2_PATH_MAX];
    char cmd[LG_FMI2_PATH_MAX * 2 + 64];
    if (inst->bundle_mode) {
        /* fmpy estrae il bundle via Python zipfile, che NON preserva il
         * bit +x. restore_perms.sh (generato da bundle/build.sh) riapplica
         * chmod +x sui file noti del bundle; lo invochiamo via `bash` (che
         * non richiede l'exec bit sul file stesso) prima del launch.
         * Idempotente: se i bit erano gia' a posto (estrazione via unzip
         * sull'host), il chmod e' un no-op. */
        char rcmd[LG_FMI2_PATH_MAX + 64];
        snprintf(rcmd, sizeof(rcmd),
                 "bash \"%s/restore_perms.sh\" >/dev/null 2>&1",
                 inst->legoroot_path);
        if (getenv("LG_FMU_DEBUG"))
            fprintf(stderr, "[lg_fmu DBG] restore_perms: %s\n", rcmd);
        system(rcmd);

        /* launch_sim.sh setta LEGOROOT/PATH/LD_LIBRARY_PATH e poi invoca
         * net_startup_headless.sh dentro il bundle. */
        snprintf(script, sizeof(script),
                 "%s/launch_sim.sh", inst->legoroot_path);
    } else {
        snprintf(script, sizeof(script),
                 "%s/Alg_rt/lg_fmu/scripts/net_startup_headless.sh",
                 inst->legoroot_path);
    }

    struct stat st;
    if (stat(script, &st) != 0) {
        lg_log(inst, fmi2Error, "logStatusError",
               "headless launcher non trovato: %s", script);
        return 0;
    }

    int dbg = getenv("LG_FMU_DEBUG") != NULL;

    /* Quote sicuro per spazi nel TASK_PATH; in debug NON redirigere a null
     * cosi' vediamo cosa stampa il launcher. */
    if (dbg) {
        snprintf(cmd, sizeof(cmd),
                 "\"%s\" \"%s\" 2>&1 | sed 's,^,[lg_fmu LAUNCH] ,' >&2",
                 script, inst->task_path);
    } else {
        snprintf(cmd, sizeof(cmd), "\"%s\" \"%s\" >/dev/null 2>&1",
                 script, inst->task_path);
    }

    lg_log(inst, fmi2OK, "logAll", "auto-launch: %s", cmd);
    if (dbg) fprintf(stderr, "[lg_fmu DBG] system: %s\n", cmd);

    int rc = system(cmd);
    if (dbg) fprintf(stderr, "[lg_fmu DBG] system rc=%d (WEXITSTATUS=%d)\n",
                     rc, WEXITSTATUS(rc));
    if (rc != 0) {
        lg_log(inst, fmi2Error, "logStatusError",
               "headless launcher uscito con rc=%d", rc);
        /* prosegui col polling: a volte i background sono partiti comunque */
    }

    /* Poll finche' RtCreateDbPunti riesce, o timeout. */
    for (int i = 0; i < LG_BOOT_MAX_POLLS; ++i) {
        if (try_attach_db(inst)) {
            inst->we_started_sim = 1;
            if (dbg) fprintf(stderr,
                "[lg_fmu DBG] try_attach_db OK dopo %d ms\n",
                (i + 1) * LG_BOOT_POLL_MS);
            lg_log(inst, fmi2OK, "logAll",
                   "sim attiva dopo %d ms", (i + 1) * LG_BOOT_POLL_MS);

            int rc_init = SD_inizializza(LG_DISPATCHER_SENDER);
            if (dbg) fprintf(stderr,
                "[lg_fmu DBG] SD_inizializza(BI) -> %d\n", rc_init);
            if (rc_init <= 0) {
                lg_log(inst, fmi2Error, "logStatusError",
                       "SD_inizializza(BI) fallita rc=%d", rc_init);
                return 0;
            }
            lg_log(inst, fmi2OK, "logAll",
                   "SD_inizializza(BI) inviata, attendo sked_start...");

            int last_s = -1;
            for (int j = 0; j < LG_INIT_MAX_POLLS; ++j) {
                int s = -1;
                RtDbPGetStato((RtDbPuntiOggetto)inst->dbpunti, &s);
                if (dbg && s != last_s) {
                    fprintf(stderr,
                        "[lg_fmu DBG] poll j=%d stato=%d\n", j, s);
                    last_s = s;
                }
                if (s == STATO_FREEZE) {
                    lg_log(inst, fmi2OK, "logAll",
                           "init completata dopo %d ms (stato=FREEZE)",
                           (j + 1) * LG_INIT_POLL_MS);
                    return 1;
                }
                msleep_lg(LG_INIT_POLL_MS);
            }
            if (dbg) fprintf(stderr,
                "[lg_fmu DBG] TIMEOUT init: ultimo stato=%d (atteso STATO_FREEZE=%d)\n",
                last_s, STATO_FREEZE);
            lg_log(inst, fmi2Error, "logStatusError",
                   "timeout (%d s) attendendo STATO_FREEZE post-init",
                   LG_INIT_MAX_POLLS * LG_INIT_POLL_MS / 1000);
            return 0;
        }
        msleep_lg(LG_BOOT_POLL_MS);
    }
    if (dbg) fprintf(stderr,
        "[lg_fmu DBG] TIMEOUT boot: try_attach_db non riuscito in %d s\n",
        LG_BOOT_MAX_POLLS * LG_BOOT_POLL_MS / 1000);
    lg_log(inst, fmi2Error, "logStatusError",
           "timeout (%d s) attendendo l'avvio della sim",
           LG_BOOT_MAX_POLLS * LG_BOOT_POLL_MS / 1000);
    return 0;
}

/* attach-or-launch: la chiave del Punto 4. Returns 0 ok, -1 fail. */
static int attach_or_launch(lg_fmi2_instance *inst)
{
    int dbg = getenv("LG_FMU_DEBUG") != NULL;

    /* 1. Resource dir */
    char res_dir[LG_FMI2_PATH_MAX];
    if (parse_resource_uri(inst->fmuResourceLocation, res_dir, sizeof(res_dir)) != 0
        || res_dir[0] == '\0') {
        lg_log(inst, fmi2Error, "logStatusError",
               "fmuResourceLocation invalido: '%s'", inst->fmuResourceLocation);
        return -1;
    }
    if (dbg) fprintf(stderr,
        "[lg_fmu DBG] resource_uri='%s' -> res_dir='%s'\n",
        inst->fmuResourceLocation, res_dir);

    /* 2. Leggi task_info.env */
    char info[LG_FMI2_PATH_MAX];
    snprintf(info, sizeof(info), "%s/task_info.env", res_dir);
    if (read_env_value(info, "TASK_PATH",
                       inst->task_path, sizeof(inst->task_path)) != 0) {
        lg_log(inst, fmi2Error, "logStatusError",
               "TASK_PATH non leggibile da %s", info);
        return -1;
    }
    /* LEGOROOT puo' essere vuoto in bundle mode (lo deriviamo da res_dir). */
    inst->legoroot_path[0] = '\0';
    (void)read_env_value(info, "LEGOROOT",
                         inst->legoroot_path, sizeof(inst->legoroot_path));

    /* BUNDLE_MODE opzionale (legacy task_info.env senza la chiave -> 0). */
    char bm[16] = {0};
    inst->bundle_mode = 0;
    if (read_env_value(info, "BUNDLE_MODE", bm, sizeof(bm)) == 0
        && atoi(bm) == 1) {
        inst->bundle_mode = 1;
    }

    /* Parametri di dimensionamento LEGO da task_info.env: senza questi
     * GetParLego() (libsim/par_lego.c) cade sui DEF_* e il dimensionamento
     * delle strutture interne non combacia con la SHM creata dalla sim
     * (-> fmi2DoStep status 3). Setenv solo se la chiave esiste in
     * task_info.env e non e' gia' definita nell'env del caller. */
    static const char *const lego_keys[] = {
        "N000","N001","N002","N003","N004","N005","N007",
        "M001","M002","M003","M004","M005", NULL
    };
    for (const char *const *k = lego_keys; *k; ++k) {
        char val[64] = {0};
        if (read_env_value(info, *k, val, sizeof(val)) == 0 && val[0])
            setenv(*k, val, 0);
    }

    if (inst->bundle_mode) {
        /* In bundle mode i path sono relativi al bundle estratto da fmpy:
         * res_dir = .../resources, LEGOROOT = .../resources/bundle,
         * TASK_PATH (relativo, e.g. "task/collet") -> assoluto. */
        char abs_legoroot[LG_FMI2_PATH_MAX];
        snprintf(abs_legoroot, sizeof(abs_legoroot), "%s/bundle", res_dir);
        strncpy(inst->legoroot_path, abs_legoroot,
                sizeof(inst->legoroot_path) - 1);
        inst->legoroot_path[sizeof(inst->legoroot_path) - 1] = '\0';

        if (inst->task_path[0] != '/') {
            char abs_task[LG_FMI2_PATH_MAX];
            snprintf(abs_task, sizeof(abs_task), "%s/%s",
                     inst->legoroot_path, inst->task_path);
            strncpy(inst->task_path, abs_task,
                    sizeof(inst->task_path) - 1);
            inst->task_path[sizeof(inst->task_path) - 1] = '\0';
        }
        lg_log(inst, fmi2OK, "logAll",
               "BUNDLE_MODE=1 -> LEGOROOT=%s task=%s",
               inst->legoroot_path, inst->task_path);
    } else if (inst->legoroot_path[0] == '\0') {
        lg_log(inst, fmi2Error, "logStatusError",
               "LEGOROOT non leggibile da %s e BUNDLE_MODE!=1", info);
        return -1;
    }

    if (dbg) fprintf(stderr,
        "[lg_fmu DBG] bundle_mode=%d legoroot='%s' task='%s'\n",
        inst->bundle_mode, inst->legoroot_path, inst->task_path);

    /* 3. setenv + 4. chdir */
    setup_legopst_env(inst->legoroot_path, inst->bundle_mode);
    if (dbg) fprintf(stderr,
        "[lg_fmu DBG] post setup_legopst_env: SHR_USR_KEY=%s SHR_TAV_KEY=%s "
        "PATH=%s LD_LIBRARY_PATH=%s\n",
        getenv("SHR_USR_KEY")?:"(unset)",
        getenv("SHR_TAV_KEY")?:"(unset)",
        getenv("PATH")?:"(unset)",
        getenv("LD_LIBRARY_PATH")?:"(unset)");
    if (chdir(inst->task_path) != 0) {
        if (dbg) fprintf(stderr,
            "[lg_fmu DBG] chdir('%s') errno=%d\n", inst->task_path, errno);
        lg_log(inst, fmi2Error, "logStatusError",
               "chdir(%s) fallito", inst->task_path);
        return -1;
    }

    /* 5. Try attach */
    if (try_attach_db(inst)) {
        inst->we_started_sim = 0;
        lg_log(inst, fmi2OK, "logAll",
               "sim gia' viva, attach-only su %s", inst->task_path);
        return init_sim_if_stopped(inst);
    }

    /* 6. Launch + wait */
    if (launch_sim_and_wait(inst)) return init_sim_if_stopped(inst);
    return -1;
}


/* ====================================================================
 * Inquire version / debug logging
 * ==================================================================== */

FMI2_Export const char *fmi2GetTypesPlatform(void)
{
    return fmi2TypesPlatform;
}

FMI2_Export const char *fmi2GetVersion(void)
{
    return "2.0";
}

FMI2_Export fmi2Status fmi2SetDebugLogging(fmi2Component c,
                                           fmi2Boolean loggingOn,
                                           size_t nCategories,
                                           const fmi2String categories[])
{
    (void)nCategories; (void)categories;
    if (!c) return fmi2Error;
    ((lg_fmi2_instance *)c)->loggingOn = loggingOn;
    return fmi2OK;
}


/* ====================================================================
 * Instantiate / FreeInstance
 * ==================================================================== */

FMI2_Export fmi2Component fmi2Instantiate(fmi2String instanceName,
                                          fmi2Type fmuType,
                                          fmi2String fmuGUID,
                                          fmi2String fmuResourceLocation,
                                          const fmi2CallbackFunctions *functions,
                                          fmi2Boolean visible,
                                          fmi2Boolean loggingOn)
{
    if (!instanceName || !functions || !functions->logger) return NULL;
    if (fmuType != fmi2CoSimulation) return NULL;

    lg_fmi2_instance *inst = (lg_fmi2_instance *)calloc(1, sizeof(*inst));
    if (!inst) return NULL;

    strncpy(inst->instanceName, instanceName, LG_FMI2_NAME_MAX - 1);
    inst->fmuType    = fmuType;
    if (fmuGUID)
        strncpy(inst->fmuGUID, fmuGUID, LG_FMI2_GUID_MAX - 1);
    if (fmuResourceLocation)
        strncpy(inst->fmuResourceLocation, fmuResourceLocation, LG_FMI2_PATH_MAX - 1);
    inst->callbacks  = *functions;
    inst->visible    = visible;
    inst->loggingOn  = loggingOn;
    inst->state      = LG_FMI2_S_START;
    inst->model_idx  = 1;

    /* Punto 4: attach-or-launch.
     * Legge resources/task_info.env per ricavare TASK_PATH/LEGOROOT,
     * imposta env e cwd, prova ad attaccarsi alla sim; se non c'e',
     * lancia net_startup_headless.sh e attende che sia pronta. */
    int dbg_inst = getenv("LG_FMU_DEBUG") != NULL;
    if (attach_or_launch(inst) != 0) {
        if (dbg_inst) fprintf(stderr, "[lg_fmu DBG] attach_or_launch FAIL\n");
        if (inst->dbpunti) RtDestroyDbPunti((RtDbPuntiOggetto)inst->dbpunti);
        free(inst);
        return NULL;
    }
    if (dbg_inst) fprintf(stderr,
        "[lg_fmu DBG] attach_or_launch OK (we_started_sim=%d)\n",
        inst->we_started_sim);
    /* SHR_USR_KEY ora e' garantita (settata da setup_legopst_env se mancava). */
    {
        const char *env = getenv("SHR_USR_KEY");
        inst->shr_usr_key = env ? atoi(env) : 0;
    }

    /* Variable map (libsim/var_sh wrapper). */
    inst->vars = lg_var_open();
    if (dbg_inst) fprintf(stderr, "[lg_fmu DBG] lg_var_open -> %p\n",
                          (void*)inst->vars);
    if (!inst->vars) {
        lg_log(inst, fmi2Error, "logStatusError",
               "lg_var_open ha fallito: variabili.rtf/S04/S05 non raggiungibili?");
        RtDestroyDbPunti((RtDbPuntiOggetto)inst->dbpunti);
        free(inst);
        return NULL;
    }
    int bvr = build_var_index(inst);
    if (dbg_inst) fprintf(stderr, "[lg_fmu DBG] build_var_index -> %d\n", bvr);
    if (bvr != 0) {
        lg_log(inst, fmi2Error, "logStatusError",
               "build_var_index ha fallito");
        lg_var_close(inst->vars);
        RtDestroyDbPunti((RtDbPuntiOggetto)inst->dbpunti);
        free(inst);
        return NULL;
    }

    /* Cadence (read once, used by DoStep). */
    float dt = -1.0f, ts = -1.0f;
    RtDbPGetDt((RtDbPuntiOggetto)inst->dbpunti, inst->model_idx - 1, &dt);
    RtDbPGetTimeScaling((RtDbPuntiOggetto)inst->dbpunti, &ts);
    inst->dt_sked     = (dt > 0.0f) ? dt : 1.0f;
    inst->timescaling = (ts > 0.0f) ? ts : 1.0f;

    lg_log(inst, fmi2OK, "logAll",
           "Instantiate ok: %d variabili (%d in, %d out), dt_sked=%.3f",
           inst->n_unique, inst->n_inputs, inst->n_outputs, inst->dt_sked);
    return (fmi2Component)inst;
}

FMI2_Export void fmi2FreeInstance(fmi2Component c)
{
    if (!c) return;
    lg_fmi2_instance *inst = (lg_fmi2_instance *)c;

    free(inst->unique_addrs);
    free(inst->is_input);
    if (inst->vars) lg_var_close(inst->vars);
    /* RtErrore: no public Destroy in this header set; leak the handle. */

    /* Cleanup ordine condizionale:
     * - launch mode (we_started_sim==1): killsim PRIMA cancella SHM/code/proc
     *   della sim. NON chiamare RtDestroyDbPunti dopo: le SHM sono gia'
     *   sparite e CloseDbPunti->elimina_shrmem tenterebbe un re-attach interno
     *   che logga "[error shared-memory not attached]" come effetto cosmetico
     *   (la sim e' comunque pulita, no zombi).
     * - attach mode (we_started_sim==0): la sim utente continua a girare per
     *   altro, niente killsim. Sganciamo solo il nostro handle al db. */
    if (inst->we_started_sim) {
        lg_log(inst, fmi2OK, "logAll",
               "auto-cleanup: killsim (la FMU aveva lanciato la sim)");
        int rc = system("killsim >/dev/null 2>&1");
        if (rc != 0)
            lg_log(inst, fmi2Warning, "logAll",
                   "killsim ha ritornato rc=%d (non fatale)", rc);
    } else if (inst->dbpunti) {
        RtDestroyDbPunti((RtDbPuntiOggetto)inst->dbpunti);
    }

    free(inst);
}


/* ====================================================================
 * SetupExperiment / EnterInit / ExitInit / Terminate / Reset
 * ==================================================================== */

FMI2_Export fmi2Status fmi2SetupExperiment(fmi2Component c,
                                           fmi2Boolean toleranceDefined,
                                           fmi2Real tolerance,
                                           fmi2Real startTime,
                                           fmi2Boolean stopTimeDefined,
                                           fmi2Real stopTime)
{
    (void)toleranceDefined; (void)tolerance;
    (void)stopTimeDefined;  (void)stopTime;
    if (!c) return fmi2Error;
    lg_fmi2_instance *inst = (lg_fmi2_instance *)c;
    if (inst->state != LG_FMI2_S_START) return fmi2Error;

    inst->start_time   = startTime;
    inst->current_time = startTime;
    inst->state        = LG_FMI2_S_SETUP;
    return fmi2OK;
}

FMI2_Export fmi2Status fmi2EnterInitializationMode(fmi2Component c)
{
    if (!c) return fmi2Error;
    lg_fmi2_instance *inst = (lg_fmi2_instance *)c;
    if (inst->state != LG_FMI2_S_SETUP) return fmi2Error;
    inst->state = LG_FMI2_S_INIT;
    return fmi2OK;
}

FMI2_Export fmi2Status fmi2ExitInitializationMode(fmi2Component c)
{
    if (!c) return fmi2Error;
    lg_fmi2_instance *inst = (lg_fmi2_instance *)c;
    if (inst->state != LG_FMI2_S_INIT) return fmi2Error;

    /* Make sure the simulation is paused before the first DoStep so
     * that SD_goup advances exactly one dt_sked at a time. */
    SD_freeze(LG_DISPATCHER_SENDER);
    inst->state = LG_FMI2_S_READY;
    return fmi2OK;
}

FMI2_Export fmi2Status fmi2Terminate(fmi2Component c)
{
    if (!c) return fmi2Error;
    lg_fmi2_instance *inst = (lg_fmi2_instance *)c;
    if (inst->state != LG_FMI2_S_READY) return fmi2Error;
    inst->state = LG_FMI2_S_TERMINATED;
    return fmi2OK;
}

FMI2_Export fmi2Status fmi2Reset(fmi2Component c)
{
    if (!c) return fmi2Error;
    lg_fmi2_instance *inst = (lg_fmi2_instance *)c;
    inst->state        = LG_FMI2_S_START;
    inst->current_time = 0.0;
    inst->step_count   = 0;
    return fmi2OK;
}


/* ====================================================================
 * Get / Set value
 * ==================================================================== */

FMI2_Export fmi2Status fmi2GetReal(fmi2Component c,
                                   const fmi2ValueReference vr[], size_t nvr,
                                   fmi2Real value[])
{
    if (!c || (nvr > 0 && (!vr || !value))) return fmi2Error;
    lg_fmi2_instance *inst = (lg_fmi2_instance *)c;

    for (size_t i = 0; i < nvr; ++i) {
        int addr = (int)vr[i];
        float v = 0.0f;
        if (!RtDbPGetValue((RtDbPuntiOggetto)inst->dbpunti, addr, &v))
            return fmi2Error;
        value[i] = (fmi2Real)v;
    }
    return fmi2OK;
}

FMI2_Export fmi2Status fmi2SetReal(fmi2Component c,
                                   const fmi2ValueReference vr[], size_t nvr,
                                   const fmi2Real value[])
{
    if (!c || (nvr > 0 && (!vr || !value))) return fmi2Error;
    lg_fmi2_instance *inst = (lg_fmi2_instance *)c;

    for (size_t i = 0; i < nvr; ++i) {
        int addr = (int)vr[i];
        if (!RtDbPPutValue((RtDbPuntiOggetto)inst->dbpunti, addr, (float)value[i]))
            return fmi2Error;
    }
    return fmi2OK;
}

/* No Integer/Boolean/String variables exposed yet → discard cleanly. */
FMI2_Export fmi2Status fmi2GetInteger(fmi2Component c, const fmi2ValueReference vr[],
                                      size_t nvr, fmi2Integer value[])
{ (void)c; (void)vr; (void)nvr; (void)value; return fmi2Discard; }

FMI2_Export fmi2Status fmi2GetBoolean(fmi2Component c, const fmi2ValueReference vr[],
                                      size_t nvr, fmi2Boolean value[])
{ (void)c; (void)vr; (void)nvr; (void)value; return fmi2Discard; }

FMI2_Export fmi2Status fmi2GetString(fmi2Component c, const fmi2ValueReference vr[],
                                     size_t nvr, fmi2String value[])
{ (void)c; (void)vr; (void)nvr; (void)value; return fmi2Discard; }

FMI2_Export fmi2Status fmi2SetInteger(fmi2Component c, const fmi2ValueReference vr[],
                                      size_t nvr, const fmi2Integer value[])
{ (void)c; (void)vr; (void)nvr; (void)value; return fmi2Discard; }

FMI2_Export fmi2Status fmi2SetBoolean(fmi2Component c, const fmi2ValueReference vr[],
                                      size_t nvr, const fmi2Boolean value[])
{ (void)c; (void)vr; (void)nvr; (void)value; return fmi2Discard; }

FMI2_Export fmi2Status fmi2SetString(fmi2Component c, const fmi2ValueReference vr[],
                                     size_t nvr, const fmi2String value[])
{ (void)c; (void)vr; (void)nvr; (void)value; return fmi2Discard; }


/* ====================================================================
 * DoStep / CancelStep / Status
 * ==================================================================== */

FMI2_Export fmi2Status fmi2DoStep(fmi2Component c,
                                  fmi2Real currentCommunicationPoint,
                                  fmi2Real communicationStepSize,
                                  fmi2Boolean noSetFMUStatePriorToCurrentPoint)
{
    (void)noSetFMUStatePriorToCurrentPoint;
    if (!c) return fmi2Error;
    lg_fmi2_instance *inst = (lg_fmi2_instance *)c;
    if (inst->state != LG_FMI2_S_READY) return fmi2Error;
    if (communicationStepSize <= 0.0)   return fmi2Error;

    /* La FMU dichiara canHandleVariableCommunicationStepSize="false" e
     * DefaultExperiment.stepSize=dt_sked. Se il master chiede stepSize
     * piu' piccolo di dt_sked, la sim avanza comunque di un dt_sked
     * intero (granularita' minima): il tempo FMI risultante e' sganciato
     * dal tempo fisico interno. fmi2Discard sarebbe semanticamente
     * corretto, ma molti master (fmpy) entrano in loop infinito perche'
     * non degradano stepSize ne' chiamano Terminate. Log un warning e
     * proseguiamo: il master vedra' il tempo "gonfiato" ma niente crash. */
    if (communicationStepSize < (double)inst->dt_sked - 1e-9) {
        lg_log(inst, fmi2Warning, "logStatusWarning",
               "stepSize=%.6g < dt_sked=%.3f: la sim avanzera' un dt_sked intero",
               communicationStepSize, inst->dt_sked);
    }

    /* How many sked iterations to advance? Round up so we never
     * undershoot the requested communication step. */
    double steps_d = communicationStepSize / (double)inst->dt_sked;
    int    n_goup  = (int)ceil(steps_d);
    if (n_goup < 1) n_goup = 1;

    /* Timeout per step configurabile: LG_FMU_STEP_TIMEOUT_S=<sec> (default 10). */
    int timeout_s = LG_GOUP_MAX_POLLS_DEFAULT * LG_GOUP_POLL_MS / 1000;
    const char *to_env = getenv("LG_FMU_STEP_TIMEOUT_S");
    if (to_env && atoi(to_env) > 0) timeout_s = atoi(to_env);
    int max_polls = timeout_s * 1000 / LG_GOUP_POLL_MS;

    int dbg_step = getenv("LG_FMU_DEBUG") != NULL;
    if (dbg_step)
        fprintf(stderr,
            "[lg_fmu DBG] DoStep t=%.3f h=%.3f dt_sked=%.3f n_goup=%d timeout=%ds\n",
            (double)currentCommunicationPoint, (double)communicationStepSize,
            (double)inst->dt_sked, n_goup, timeout_s);

    float t_pre = -1.0f, t_post = -1.0f;
    RtDbPGetTime((RtDbPuntiOggetto)inst->dbpunti, &t_pre);

    for (int i = 0; i < n_goup; ++i) {
        float t_before = -1.0f;
        RtDbPGetTime((RtDbPuntiOggetto)inst->dbpunti, &t_before);

        int rc = SD_goup(LG_DISPATCHER_SENDER);
        if (rc <= 0) {
            lg_log(inst, fmi2Error, "logStatusError",
                   "SD_goup #%d ritorna %d", i + 1, rc);
            return fmi2Error;
        }

        /* Wait for sked to publish the new time (single iteration). */
        int k;
        for (k = 0; k < max_polls; ++k) {
            msleep(LG_GOUP_POLL_MS);
            RtDbPGetTime((RtDbPuntiOggetto)inst->dbpunti, &t_post);
            if (t_post > t_before + 1e-4f) break;
        }
        if (k >= max_polls) {
            lg_log(inst, fmi2Error, "logStatusError",
                   "timeout %ds aspettando avanzamento tempo (step %d/%d)",
                   timeout_s, i + 1, n_goup);
            return fmi2Error;
        }
    }

    inst->current_time = currentCommunicationPoint + communicationStepSize;
    inst->step_count++;
    return fmi2OK;
}

FMI2_Export fmi2Status fmi2CancelStep(fmi2Component c)
{
    (void)c;
    return fmi2Error;  /* canRunAsynchronuously=false */
}

FMI2_Export fmi2Status fmi2GetStatus(fmi2Component c, const fmi2StatusKind s,
                                     fmi2Status *value)
{ (void)c; (void)s; (void)value; return fmi2Discard; }

FMI2_Export fmi2Status fmi2GetRealStatus(fmi2Component c, const fmi2StatusKind s,
                                         fmi2Real *value)
{
    if (!c || !value) return fmi2Error;
    if (s == fmi2LastSuccessfulTime) {
        *value = ((lg_fmi2_instance *)c)->current_time;
        return fmi2OK;
    }
    return fmi2Discard;
}

FMI2_Export fmi2Status fmi2GetIntegerStatus(fmi2Component c, const fmi2StatusKind s,
                                            fmi2Integer *value)
{ (void)c; (void)s; (void)value; return fmi2Discard; }

FMI2_Export fmi2Status fmi2GetBooleanStatus(fmi2Component c, const fmi2StatusKind s,
                                            fmi2Boolean *value)
{
    if (!c || !value) return fmi2Error;
    if (s == fmi2Terminated) {
        *value = (((lg_fmi2_instance *)c)->state == LG_FMI2_S_TERMINATED);
        return fmi2OK;
    }
    return fmi2Discard;
}

FMI2_Export fmi2Status fmi2GetStringStatus(fmi2Component c, const fmi2StatusKind s,
                                           fmi2String *value)
{ (void)c; (void)s; (void)value; return fmi2Discard; }


/* ====================================================================
 * Unsupported (canGetAndSetFMUstate=false, providesDirectionalDerivative=false)
 * ==================================================================== */

FMI2_Export fmi2Status fmi2GetFMUstate(fmi2Component c, fmi2FMUstate *st)
{ (void)c; (void)st; return fmi2Error; }

FMI2_Export fmi2Status fmi2SetFMUstate(fmi2Component c, fmi2FMUstate st)
{ (void)c; (void)st; return fmi2Error; }

FMI2_Export fmi2Status fmi2FreeFMUstate(fmi2Component c, fmi2FMUstate *st)
{ (void)c; (void)st; return fmi2Error; }

FMI2_Export fmi2Status fmi2SerializedFMUstateSize(fmi2Component c, fmi2FMUstate st,
                                                  size_t *size)
{ (void)c; (void)st; (void)size; return fmi2Error; }

FMI2_Export fmi2Status fmi2SerializeFMUstate(fmi2Component c, fmi2FMUstate st,
                                             fmi2Byte serializedState[], size_t size)
{ (void)c; (void)st; (void)serializedState; (void)size; return fmi2Error; }

FMI2_Export fmi2Status fmi2DeSerializeFMUstate(fmi2Component c,
                                               const fmi2Byte serializedState[],
                                               size_t size, fmi2FMUstate *st)
{ (void)c; (void)serializedState; (void)size; (void)st; return fmi2Error; }

FMI2_Export fmi2Status fmi2GetDirectionalDerivative(fmi2Component c,
                                                    const fmi2ValueReference vUnknown[],
                                                    size_t nUnknown,
                                                    const fmi2ValueReference vKnown[],
                                                    size_t nKnown,
                                                    const fmi2Real dvKnown[],
                                                    fmi2Real dvUnknown[])
{
    (void)c; (void)vUnknown; (void)nUnknown; (void)vKnown; (void)nKnown;
    (void)dvKnown; (void)dvUnknown;
    return fmi2Error;
}

FMI2_Export fmi2Status fmi2SetRealInputDerivatives(fmi2Component c,
                                                   const fmi2ValueReference vr[],
                                                   size_t nvr,
                                                   const fmi2Integer order[],
                                                   const fmi2Real value[])
{
    (void)c; (void)vr; (void)nvr; (void)order; (void)value;
    return fmi2Error;  /* canInterpolateInputs=false */
}

FMI2_Export fmi2Status fmi2GetRealOutputDerivatives(fmi2Component c,
                                                    const fmi2ValueReference vr[],
                                                    size_t nvr,
                                                    const fmi2Integer order[],
                                                    fmi2Real value[])
{
    (void)c; (void)vr; (void)nvr; (void)order; (void)value;
    return fmi2Error;  /* maxOutputDerivativeOrder=0 */
}
