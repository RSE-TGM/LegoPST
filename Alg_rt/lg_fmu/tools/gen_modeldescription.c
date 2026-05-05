/*
 * gen_modeldescription.c — generate modelDescription.xml for the Linux
 * LegoCliSINC FMU bundle.
 *
 * Si attacca a una simulazione attiva (richiede SHR_USR_KEY in env e una
 * task LegoPST con net_startup gia' eseguito), apre lg_var_mapping,
 * itera sulle celle SHM uniche e produce un modelDescription.xml
 * completo a partire dal template `bundle/modelDescription.xml.in`.
 *
 * Sostituzioni:
 *   @MODEL_NAME@         valore --model-name (default LegoCliSINC)
 *   @GUID@               valore --guid (default: uuidgen-style random)
 *   @DESCRIPTION@        valore --description
 *   @VERSION@            valore --version (default 1.0.0)
 *   @GENERATION_TOOL@    "lg_fmu/gen_modeldescription"
 *   @GENERATION_DATE@    timestamp ISO-8601 al momento della build
 *   @DEFAULT_START@      0.0
 *   @DEFAULT_STEP@       dt_sked del modello attivo (RtDbPGetDt)
 *   @MODEL_VARIABLES@    lista di <ScalarVariable> indentata
 *   @MODEL_OUTPUTS@      lista di <Unknown index="N"/> per gli output
 *
 * ValueReference == addr SHM (lg_var_info.addr). Stable cross-run del
 * medesimo modello.
 *
 * Naming "structured": "<modello>.<blocco>.<nome>" — disambiguato per
 * costruzione (un nome puo' essere ripetuto in blocchi diversi).
 *
 * Build:    make -f Makefile.mk gen_modeldescription
 * Run:
 *   cd /path/to/legocad/MDC_GV
 *   /path/to/lg_fmu/tools/gen_modeldescription \
 *       --template /path/to/lg_fmu/bundle/modelDescription.xml.in \
 *       --out      /tmp/modelDescription.xml \
 *       --model-name LegoCliSINC \
 *       --description "LEGO thermo-hydraulic CS FMU"
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>

#include <sqlite3.h>
#include <sim_param.h>
#include <sim_types.h>
#include <Rt/RtErrore.h>
#include <Rt/RtDbPunti.h>

#include "lg_var_mapping.h"


/* -------------------------------------------------------------------- */
/* Dynamic string buffer                                                */
/* -------------------------------------------------------------------- */
typedef struct {
    char  *buf;
    size_t len;
    size_t cap;
} dstr;

static void dstr_init(dstr *s) { s->buf = NULL; s->len = 0; s->cap = 0; }

static void dstr_free(dstr *s)
{
    free(s->buf); s->buf = NULL; s->len = 0; s->cap = 0;
}

static void dstr_reserve(dstr *s, size_t need)
{
    if (s->cap >= need + 1) return;
    size_t nc = s->cap ? s->cap : 256;
    while (nc < need + 1) nc *= 2;
    s->buf = realloc(s->buf, nc);
    if (!s->buf) { fprintf(stderr, "OOM\n"); exit(99); }
    s->cap = nc;
}

static void dstr_appendn(dstr *s, const char *p, size_t n)
{
    dstr_reserve(s, s->len + n);
    memcpy(s->buf + s->len, p, n);
    s->len += n;
    s->buf[s->len] = '\0';
}

static void dstr_append(dstr *s, const char *p) { dstr_appendn(s, p, strlen(p)); }

static void dstr_appendf(dstr *s, const char *fmt, ...)
{
    char tmp[1024];
    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(tmp, sizeof(tmp), fmt, ap);
    va_end(ap);
    if (n > 0) dstr_appendn(s, tmp, (size_t)n);
}


/* -------------------------------------------------------------------- */
/* XML escape (minimal: & < > " ').                                      */
/* Le description LegoPST sono Latin-1/CP1252 (es. m³ = byte 0xB3 da     */
/* solo, "²" = 0xB2). Il modelDescription.xml dichiara encoding="UTF-8",  */
/* quindi convertiamo on-the-fly: ogni byte >= 0x80 -> sequenza UTF-8 a   */
/* 2 byte. Senza questa conversione lxml/fmpy.read_model_description     */
/* fallisce con "Invalid bytes in character encoding".                   */
/* -------------------------------------------------------------------- */
static void xml_escape(dstr *out, const char *s)
{
    for (const unsigned char *u = (const unsigned char *)s; *u; ++u) {
        switch (*u) {
            case '&':  dstr_append(out, "&amp;");  break;
            case '<':  dstr_append(out, "&lt;");   break;
            case '>':  dstr_append(out, "&gt;");   break;
            case '"':  dstr_append(out, "&quot;"); break;
            case '\'': dstr_append(out, "&apos;"); break;
            default:
                if (*u < 0x80) {
                    dstr_appendn(out, (const char *)u, 1);
                } else {
                    /* Latin-1 -> UTF-8: 0x80..0xFF -> 0xC2/0xC3 + low6 */
                    char b[2];
                    b[0] = (char)(0xC0 | (*u >> 6));
                    b[1] = (char)(0x80 | (*u & 0x3F));
                    dstr_appendn(out, b, 2);
                }
                break;
        }
    }
}


/* -------------------------------------------------------------------- */
/* Template loading + @KEY@ substitution                                */
/* -------------------------------------------------------------------- */
static int load_file(const char *path, dstr *out)
{
    FILE *f = fopen(path, "rb");
    if (!f) {
        fprintf(stderr, "ERRORE: impossibile aprire '%s'\n", path);
        return -1;
    }
    char buf[4096];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), f)) > 0)
        dstr_appendn(out, buf, n);
    fclose(f);
    return 0;
}

/* Sostituisce in-place tutte le occorrenze di @KEY@ con `value`.
 * Builds a new buffer and swaps it into *s. */
static void replace_token(dstr *s, const char *key, const char *value)
{
    char tok[64];
    snprintf(tok, sizeof(tok), "@%s@", key);
    size_t toklen = strlen(tok);
    size_t vallen = strlen(value);

    dstr out; dstr_init(&out);
    const char *p = s->buf;
    const char *end = s->buf + s->len;
    while (p < end) {
        const char *hit = strstr(p, tok);
        if (!hit) { dstr_appendn(&out, p, end - p); break; }
        dstr_appendn(&out, p, hit - p);
        dstr_appendn(&out, value, vallen);
        p = hit + toklen;
    }
    dstr_free(s);
    *s = out;
}


/* -------------------------------------------------------------------- */
/* GUID / timestamp                                                     */
/* -------------------------------------------------------------------- */
static void gen_guid(char out[40])
{
    /* Try /proc/sys/kernel/random/uuid first (no libuuid dep). */
    FILE *f = fopen("/proc/sys/kernel/random/uuid", "r");
    if (f) {
        if (fgets(out, 40, f) && strlen(out) >= 36) {
            out[36] = '\0';
            fclose(f);
            return;
        }
        fclose(f);
    }
    /* Fallback: deterministic-ish from time+pid (not cryptographic). */
    unsigned int seed = (unsigned int)time(NULL) ^ (unsigned int)getpid();
    srand(seed);
    snprintf(out, 40, "%08x-%04x-%04x-%04x-%012x",
             rand(), rand() & 0xFFFF, rand() & 0xFFFF,
             rand() & 0xFFFF, rand());
}

static void iso8601_now(char out[32])
{
    time_t t = time(NULL);
    struct tm tm;
    gmtime_r(&t, &tm);
    strftime(out, 32, "%Y-%m-%dT%H:%M:%SZ", &tm);
}


/* -------------------------------------------------------------------- */
/* Build the <ScalarVariable> and <Unknown index=> blocks.              */
/*                                                                      */
/* indents:                                                             */
/*   "        " (8 spazi) per <ScalarVariable>                          */
/*   "            " (12 spazi) per <Unknown index="N"/> dentro Outputs  */
/* -------------------------------------------------------------------- */
static void build_variables(const lg_var_mapping *m,
                            RtDbPuntiOggetto      dbpunti,
                            dstr *vars_out,
                            dstr *outs_out)
{
    int n_unique = lg_var_unique_count(m);

    for (int k = 0; k < n_unique; ++k) {
        int idx = lg_var_unique_at(m, k);
        lg_var_info info;
        if (lg_var_get_info(m, idx, &info) != 0) continue;

        const char *model_name = lg_var_model_name(m, info.model);
        const char *causality;

        switch (info.kind) {
            case LG_VAR_INPUT_FREE:      causality = "input";  break;
            case LG_VAR_OUTPUT:          causality = "output"; break;
            case LG_VAR_INPUT_CONNECTED: /* skip: written by sim itself */
            default:                     continue;
        }

        /* qualified name: "<model>.<block>.<name>" */
        dstr name; dstr_init(&name);
        if (model_name && *model_name) {
            xml_escape(&name, model_name);
            dstr_append(&name, ".");
        }
        dstr_appendf(&name, "B%d.", info.block);
        xml_escape(&name, info.name);

        /* index FMI 2.0 nelle ScalarVariable e' 1-based, monotono: e' k+1. */
        int fmi_index = (int)vars_out->len;  /* used as marker only */
        (void)fmi_index;

        /* read current value: serve come `start` per gli input, come
         * commento descrittivo per gli output (FMI non lo richiede). */
        float v = 0.0f;
        int   has_v = RtDbPGetValue(dbpunti, info.addr, &v) ? 1 : 0;

        dstr_append(vars_out, "        <ScalarVariable name=\"");
        dstr_appendn(vars_out, name.buf, name.len);
        dstr_appendf(vars_out, "\" valueReference=\"%d\"", info.addr);
        dstr_appendf(vars_out, " causality=\"%s\" variability=\"continuous\"",
                     causality);
        if (info.descr && *info.descr) {
            dstr_append(vars_out, " description=\"");
            xml_escape(vars_out, info.descr);
            dstr_append(vars_out, "\"");
        }
        dstr_append(vars_out, ">\n");

        /* Real type: include start solo per gli input (FMI 2.0 lo
         * richiede), per gli output e' opzionale e di norma omesso. */
        if (info.kind == LG_VAR_INPUT_FREE) {
            dstr_appendf(vars_out, "            <Real start=\"%.7g\"/>\n",
                         has_v ? v : 0.0);
        } else {
            dstr_append(vars_out, "            <Real/>\n");
        }
        dstr_append(vars_out, "        </ScalarVariable>\n");

        dstr_free(&name);

        /* For ModelStructure.Outputs we need 1-based indices into the
         * ModelVariables list. We compute those after the loop. */
    }

    /* Second pass: <Unknown index="N"/> for output variables. The FMI
     * spec uses 1-based indices into the order of the just-emitted
     * ScalarVariable list, so we re-iterate the unique view in the same
     * order and count. */
    int fmi_idx = 0;
    for (int k = 0; k < n_unique; ++k) {
        int idx = lg_var_unique_at(m, k);
        lg_var_info info;
        if (lg_var_get_info(m, idx, &info) != 0) continue;
        if (info.kind != LG_VAR_OUTPUT && info.kind != LG_VAR_INPUT_FREE)
            continue;
        ++fmi_idx;   /* allinea con l'enumerazione di vars_out */
        if (info.kind == LG_VAR_OUTPUT)
            dstr_appendf(outs_out, "            <Unknown index=\"%d\"/>\n",
                         fmi_idx);
    }
}


/* -------------------------------------------------------------------- */
/* Main                                                                 */
/* -------------------------------------------------------------------- */
static void usage(const char *p)
{
    fprintf(stderr,
        "Usage: %s --template FILE --out FILE [opzioni]\n"
        "\n"
        "Opzioni:\n"
        "  --template FILE       path al modelDescription.xml.in\n"
        "  --out FILE            path output del modelDescription.xml\n"
        "  --model-name NAME     default: LegoCliSINC\n"
        "  --guid UUID           default: generato (random uuid)\n"
        "  --description TEXT    default: 'LEGO thermo-hydraulic CS FMU'\n"
        "  --version V           default: 1.0.0\n"
        "  --tool NAME           default: lg_fmu/gen_modeldescription\n"
        "\n"
        "Pre-requisito: shell con env LegoPST sourceato, SHR_USR_KEY definita,\n"
        "cwd dentro la task target (variabili.rtf + S04/S05 raggiungibili).\n",
        p);
}

int main(int argc, char **argv)
{
    const char *template_path = NULL;
    const char *out_path      = NULL;
    const char *model_name    = "LegoCliSINC";
    const char *description   = "LEGO thermo-hydraulic Co-Simulation FMU (Linux)";
    const char *version_str   = "1.0.0";
    const char *tool_name     = "lg_fmu/gen_modeldescription";
    char        guid[40]      = {0};
    int         have_guid     = 0;

    for (int i = 1; i < argc; ++i) {
        if      (!strcmp(argv[i], "--template") && i+1 < argc) template_path = argv[++i];
        else if (!strcmp(argv[i], "--out")      && i+1 < argc) out_path      = argv[++i];
        else if (!strcmp(argv[i], "--model-name") && i+1 < argc) model_name  = argv[++i];
        else if (!strcmp(argv[i], "--description") && i+1 < argc) description= argv[++i];
        else if (!strcmp(argv[i], "--version")  && i+1 < argc) version_str   = argv[++i];
        else if (!strcmp(argv[i], "--tool")     && i+1 < argc) tool_name     = argv[++i];
        else if (!strcmp(argv[i], "--guid")     && i+1 < argc) {
            strncpy(guid, argv[++i], sizeof(guid) - 1);
            have_guid = 1;
        }
        else { usage(argv[0]); return 1; }
    }

    if (!template_path || !out_path) { usage(argv[0]); return 1; }
    if (!have_guid) gen_guid(guid);

    /* Attach to live simulation. */
    const char *shr_env = getenv("SHR_USR_KEY");
    if (!shr_env || !*shr_env) {
        fprintf(stderr, "ERRORE: SHR_USR_KEY non definita.\n");
        return 2;
    }

    RtErroreOggetto  errore  = RtCreateErrore(RT_ERRORE_TERMINALE,
                                              "gen_modeldescription");
    RtDbPuntiOggetto dbpunti = RtCreateDbPunti(errore, NULL, DB_PUNTI_INT, NULL);
    if (!dbpunti) {
        fprintf(stderr, "ERRORE: simulazione non attiva (RtCreateDbPunti=NULL).\n");
        return 3;
    }

    lg_var_mapping *vmap = lg_var_open();
    if (!vmap) {
        fprintf(stderr, "ERRORE: lg_var_open fallito.\n");
        RtDestroyDbPunti(dbpunti);
        return 4;
    }

    /* Cadence per DefaultExperiment. */
    float dt_sked = 1.0f;
    RtDbPGetDt(dbpunti, 0, &dt_sked);
    if (dt_sked <= 0.0f) dt_sked = 1.0f;

    /* Build the variable + outputs blocks. */
    dstr vars_block, outs_block;
    dstr_init(&vars_block);
    dstr_init(&outs_block);
    build_variables(vmap, dbpunti, &vars_block, &outs_block);

    /* Template substitution. */
    dstr xml; dstr_init(&xml);
    if (load_file(template_path, &xml) != 0) {
        lg_var_close(vmap);
        RtDestroyDbPunti(dbpunti);
        return 5;
    }

    char gen_date[32];
    iso8601_now(gen_date);

    char dt_str[32];
    snprintf(dt_str, sizeof(dt_str), "%.7g", (double)dt_sked);

    replace_token(&xml, "MODEL_NAME",       model_name);
    replace_token(&xml, "GUID",             guid);
    replace_token(&xml, "DESCRIPTION",      description);
    replace_token(&xml, "VERSION",          version_str);
    replace_token(&xml, "GENERATION_TOOL",  tool_name);
    replace_token(&xml, "GENERATION_DATE",  gen_date);
    replace_token(&xml, "DEFAULT_START",    "0.0");
    replace_token(&xml, "DEFAULT_STEP",     dt_str);
    replace_token(&xml, "MODEL_VARIABLES",  vars_block.buf ? vars_block.buf : "");
    replace_token(&xml, "MODEL_OUTPUTS",    outs_block.buf ? outs_block.buf : "");

    /* Write output. */
    FILE *fo = fopen(out_path, "wb");
    if (!fo) {
        fprintf(stderr, "ERRORE: impossibile scrivere '%s'\n", out_path);
        dstr_free(&vars_block); dstr_free(&outs_block); dstr_free(&xml);
        lg_var_close(vmap); RtDestroyDbPunti(dbpunti);
        return 6;
    }
    fwrite(xml.buf, 1, xml.len, fo);
    fclose(fo);

    /* Summary on stderr (stdout reserved per eventuali pipe future). */
    fprintf(stderr,
        "[gen_modeldescription] OK\n"
        "  template : %s\n"
        "  output   : %s\n"
        "  model    : %s\n"
        "  guid     : %s\n"
        "  dt_sked  : %s\n"
        "  variabili: %d uniche (%d input, %d output)\n",
        template_path, out_path, model_name, guid, dt_str,
        lg_var_unique_count(vmap),
        lg_var_input_count(vmap),
        lg_var_output_count(vmap));

    dstr_free(&vars_block);
    dstr_free(&outs_block);
    dstr_free(&xml);
    lg_var_close(vmap);
    RtDestroyDbPunti(dbpunti);
    return 0;
}
