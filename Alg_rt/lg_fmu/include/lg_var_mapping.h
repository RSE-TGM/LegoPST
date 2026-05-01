/*
 * lg_var_mapping.h — Variable name → SHM address mapping for the Linux
 * port of the LegoCliSINC FMU.
 *
 * Wraps libsim/var_sh (costruisci_var + numero_modelli/numero_variabili
 * + nome_modello + …) behind an opaque handle, so the FMU adapter does
 * not have to deal with the on-disk layout of variabili.rtf or with the
 * Fortran-style 8-char space-padded names.
 *
 * Kinds reflect the simulator-side classification:
 *   LG_VAR_OUTPUT          (STATO=0)        state/output
 *   LG_VAR_INPUT_FREE      (INGRESSO_NC=1)  input not connected (driver)
 *   LG_VAR_INPUT_CONNECTED (INGRESSO_C=2)   input connected internally
 *
 * Typical FMU exposure: outputs ↔ STATE/OUTPUT, fmu inputs ↔ INPUT_FREE.
 * INPUT_CONNECTED variables are usually NOT to be exposed because they
 * are written by the simulator itself (an internal connection).
 *
 * Lifecycle:
 *   lg_var_open()  attaches to ID_SHM_VAR (must be called from the task
 *                  cwd, after net_startup). The SHM itself is shared and
 *                  is NOT released by lg_var_close — only our private
 *                  index arrays are freed.
 *
 * Read/write of values: open with RtCreateDbPunti, then use RtDbPGetValue
 * / RtDbPPutValue with the addr returned in lg_var_info.addr.
 */

#ifndef LG_VAR_MAPPING_H
#define LG_VAR_MAPPING_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    LG_VAR_OUTPUT          = 0,
    LG_VAR_INPUT_FREE      = 1,
    LG_VAR_INPUT_CONNECTED = 2
} lg_var_kind;

typedef struct {
    char        name[9];   /* 8-char Fortran-padded -> trimmed + NUL */
    const char *descr;     /* points into mapping, do not free       */
    int         model;     /* 1-based                                */
    int         block;     /* 1-based                                */
    int         addr;      /* SHM addr, for RtDbPGet/PutValue        */
    lg_var_kind kind;
} lg_var_info;

typedef struct lg_var_mapping lg_var_mapping;

/* Attach to ID_SHM_VAR. Must be called from the task cwd (variabili.rtf
 * + S04/S05 must be reachable). Returns NULL on failure. */
lg_var_mapping *lg_var_open(void);

/* Free private index arrays. Does NOT detach ID_SHM_VAR (others use it).*/
void lg_var_close(lg_var_mapping *m);

int          lg_var_count       (const lg_var_mapping *m);
int          lg_var_model_count (const lg_var_mapping *m);

/* model_1based ∈ [1, lg_var_model_count]. Returns NUL-terminated trimmed
 * name pointing into the SHM (do not free). NULL if out of range. */
const char  *lg_var_model_name  (const lg_var_mapping *m, int model_1based);

/* Find by name. Accepts both "TM" and "TM      " (8-padded). Returns
 * idx ∈ [0, count) or -1.
 *
 * NB: a name alone is NOT necessarily unique — the same variable name
 * can appear in different blocks of a multi-block model (e.g. the same
 * regulator replicated). When a duplicate exists, this returns the
 * FIRST match in (sorted) name order, which is deterministic but may
 * not be the entry the caller wanted. For FMU runtime use, prefer the
 * SHM addr as the value reference and lg_var_index_by_addr for inverse
 * lookup. */
int          lg_var_index_by_name(const lg_var_mapping *m, const char *name);

/* Inverse lookup: addr -> idx. addr IS the unique SHM cell key (it is
 * the offset the simulator uses to Get/Put the value), but a single
 * cell can be referenced by MULTIPLE entries in VARIABILI[] when the
 * task exposes the same variable through different blocks (typical for
 * a regulator vs plant view). Returns the FIRST entry with that addr,
 * which is fine for FMU use because the value is shared. Returns -1
 * if no variable has that addr. O(N), used at instantiation time. */
int          lg_var_index_by_addr(const lg_var_mapping *m, int addr);

/* Distinct-addr view (FMU-friendly). A real-world MDC_GV task has 210
 * VARIABILI entries that map onto ~163 distinct SHM cells; the FMU
 * should expose one ScalarVariable per *unique* addr, not one per
 * VARIABILI entry. These functions iterate the unique addrs in stable
 * order (the order in which addrs are first seen scanning VARIABILI[]).
 *
 *   lg_var_unique_count(m)           total distinct addrs
 *   lg_var_unique_at   (m, n)        idx of the canonical entry for the
 *                                    n-th distinct addr (use it with
 *                                    lg_var_get_info)
 */
int          lg_var_unique_count(const lg_var_mapping *m);
int          lg_var_unique_at   (const lg_var_mapping *m, int n);

/* Fill *out with the variable info at idx. Returns 0 on success, -1 if
 * idx is out of range. The descr pointer is owned by the mapping. */
int          lg_var_get_info    (const lg_var_mapping *m, int idx,
                                 lg_var_info *out);

/* FMU-friendly views: enumerate inputs (INGRESSO_NC) and outputs (STATO)
 * separately. INGRESSO_C is intentionally not exposed by these views.
 * `_at(n)` returns the idx ∈ [0, count) suitable for lg_var_get_info. */
int          lg_var_input_count (const lg_var_mapping *m);
int          lg_var_output_count(const lg_var_mapping *m);
int          lg_var_input_at    (const lg_var_mapping *m, int n);
int          lg_var_output_at   (const lg_var_mapping *m, int n);

#ifdef __cplusplus
}
#endif

#endif /* LG_VAR_MAPPING_H */
