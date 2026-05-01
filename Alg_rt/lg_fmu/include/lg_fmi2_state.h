/*
 * lg_fmi2_state.h — Per-instance state for the Linux LegoCliSINC FMU.
 *
 * Counterpart of the Windows lego_fmu_types.h::LegoModelInstance, but
 * built around the Linux IPC stack:
 *
 *   - libsim/var_sh + lg_var_mapping  (variable name/addr discovery)
 *   - libRt RtDbPunti                 (read/write SHM cells by addr)
 *   - libdispatcher SD_run/SD_freeze/SD_goup  (drive the simulation)
 *
 * No Win32 HANDLEs, no flat xy/uu/dati layout, no per-instance shared
 * memory: net_sked is already running on the task and we plug into it.
 *
 * ValueReference convention — see project_lg_fmu_linux memory and
 * lg_var_mapping.h:  vr == SHM addr (lg_var_info.addr), unique per cell,
 * stable across runs of the same model. Look up addr → RtDbPGet/Put.
 */

#ifndef LG_FMI2_STATE_H
#define LG_FMI2_STATE_H

#include <stddef.h>
#include "fmi/headers/fmi2TypesPlatform.h"
#include "fmi/headers/fmi2FunctionTypes.h"
#include "lg_var_mapping.h"

/* ----- limits ----- */
#define LG_FMI2_NAME_MAX     256
#define LG_FMI2_PATH_MAX     1024
#define LG_FMI2_GUID_MAX     128

/* ----- FMU lifecycle state ----- */
typedef enum {
    LG_FMI2_S_START,        /* after fmi2Instantiate                 */
    LG_FMI2_S_SETUP,        /* after fmi2SetupExperiment             */
    LG_FMI2_S_INIT,         /* in initialization mode                */
    LG_FMI2_S_READY,        /* after fmi2ExitInitializationMode      */
    LG_FMI2_S_TERMINATED,   /* after fmi2Terminate                   */
    LG_FMI2_S_ERROR
} lg_fmi2_state;

/* opaque RtErrore / RtDbPunti pointers — typed as void* here to keep
 * the header self-contained (real types come from <Rt/RtErrore.h> and
 * <Rt/RtDbPunti.h> in the .c). */
typedef void *lg_rt_errore_t;
typedef void *lg_rt_dbpunti_t;

typedef struct {
    /* --- FMI identity (copied at fmi2Instantiate) --- */
    char                    instanceName[LG_FMI2_NAME_MAX];
    fmi2Type                fmuType;
    char                    fmuGUID[LG_FMI2_GUID_MAX];
    char                    fmuResourceLocation[LG_FMI2_PATH_MAX];
    fmi2CallbackFunctions   callbacks;       /* full copy, not pointer */
    fmi2Boolean             visible;
    fmi2Boolean             loggingOn;

    /* --- lifecycle --- */
    lg_fmi2_state           state;

    /* --- linux IPC handles --- */
    int                     shr_usr_key;     /* from getenv */
    lg_rt_errore_t          errore;          /* RtErroreOggetto */
    lg_rt_dbpunti_t         dbpunti;         /* RtDbPuntiOggetto */
    lg_var_mapping         *vars;            /* opaque, owns its own ID_SHM_VAR ref */

    /* --- simulation cadence (read once, after dbpunti is open) --- */
    int                     model_idx;       /* 1-based, default 1     */
    float                   dt_sked;         /* RtDbPGetDt for model_idx-1 */
    float                   timescaling;     /* RtDbPGetTimeScaling    */

    /* --- variable index (built in Instantiate) ---
     * One entry per *unique* SHM addr exposed to the FMU.
     * vr == addr; we keep both the addr array and a parallel "is_input"
     * mask so Set/GetReal can validate and route. */
    int                    *unique_addrs;    /* size = n_unique         */
    unsigned char          *is_input;        /* 1 = INGRESSO_NC, 0 = STATO */
    int                     n_unique;
    int                     n_inputs;        /* count of is_input==1    */
    int                     n_outputs;       /* count of is_input==0    */

    /* --- runtime --- */
    fmi2Real                start_time;
    fmi2Real                current_time;
    long                    step_count;

    /* --- autonomous mode (Punto 4) ---
     * Letti da resources/task_info.env in fmi2Instantiate; la FMU usa
     * questi path per ricostruire l'env (LEGOROOT) e fare chdir al task,
     * cosi' fmpy/Simulink non devono pre-lanciare net_startup. */
    char                    task_path[LG_FMI2_PATH_MAX];
    char                    legoroot_path[LG_FMI2_PATH_MAX];
    int                     we_started_sim;   /* 1 = la FMU ha lanciato
                                                 net_sked, deve fare
                                                 killsim su Terminate */
    int                     bundle_mode;      /* 1 = self-contained bundle
                                                 (P7): LEGOROOT derivato
                                                 dalla resource URI, lib/
                                                 e launch_sim.sh nel
                                                 bundle estratto. */
} lg_fmi2_instance;

#endif /* LG_FMI2_STATE_H */
