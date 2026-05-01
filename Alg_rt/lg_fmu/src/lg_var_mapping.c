/*
 * lg_var_mapping.c — implementation, see lg_var_mapping.h.
 *
 * Internally we keep the raw VARIABILI* table that costruisci_var hands
 * back, plus three private arrays:
 *   - by_name_idx[]:  indices sorted by space-padded name, for bsearch
 *   - input_idx[]:    indices of INGRESSO_NC variables only
 *   - output_idx[]:   indices of STATO variables only
 *
 * Names in VARIABILI[].nome are 8 characters space-padded (Fortran-style),
 * not NUL-terminated. We never modify the SHM contents; the trimmed
 * NUL-terminated names exposed to callers live in our scratch buffer.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sqlite3.h>
#include <sim_param.h>
#include <sim_types.h>

#include "lg_var_mapping.h"

/* libsim/var_sh.c symbols — SHM attach + accessors */
extern void  costruisci_var(char **p_ind, VARIABILI **p_var, int *id);
extern int   numero_modelli (char *);
extern int   numero_variabili (char *);
extern char *nome_modello (char *, int);

/* libsim/var_sh.c references this; we never use the sqlite branches. */
sqlite3 *db = NULL;

#define NAME_PAD_LEN 8

struct lg_var_mapping {
    char       *ind_sh_top;      /* SHM head, owned by libsim                */
    VARIABILI  *vars;            /* points into SHM                          */
    int         n_vars;
    int         n_models;
    int         shm_id;          /* unused on the close path (shared)        */

    /* Trimmed names (NUL-terminated, 9 bytes per entry). Indexed by
     * vars[i] order; cheaper than re-trimming on every call. */
    char       *names_trim;      /* n_vars * 9 bytes                         */

    /* Sorted-by-padded-name index for bsearch */
    int        *by_name_idx;     /* n_vars entries, sorted by vars[i].nome   */

    int        *input_idx;
    int         n_input;
    int        *output_idx;
    int         n_output;

    /* Distinct-addr view. unique_idx[k] is the canonical (= first seen)
     * VARIABILI[] index for the k-th distinct addr. */
    int        *unique_idx;
    int         n_unique;
};

static void trim_name(const char *padded /*[NAME_PAD_LEN]*/, char *dst /*[9]*/)
{
    int i, end = 0;
    for (i = 0; i < NAME_PAD_LEN; i++) {
        dst[i] = padded[i];
        if (padded[i] != ' ' && padded[i] != '\0')
            end = i + 1;
    }
    dst[end] = '\0';
}

/* Comparator for qsort — compares trimmed (NUL-terminated) names.
 * SHM `nome[]` is supposed to be 8-char space-padded but in practice can
 * contain NUL or residual bytes after the real name; comparing trimmed
 * keys avoids that whole class of bugs. */
static const char *g_sort_names_trim;   /* used only inside qsort        */
static int cmp_idx_by_trim(const void *pa, const void *pb)
{
    int a = *(const int*)pa, b = *(const int*)pb;
    return strcmp(&g_sort_names_trim[a * 9], &g_sort_names_trim[b * 9]);
}

/* Trim trailing spaces from a NUL-terminated name into a 9-byte buffer.
 * Used to canonicalize the user-provided lookup key. */
static void canon_trim(const char *src, char *dst /*[9]*/)
{
    size_t n = strlen(src);
    if (n > NAME_PAD_LEN) n = NAME_PAD_LEN;
    while (n > 0 && src[n - 1] == ' ') n--;
    memcpy(dst, src, n);
    dst[n] = '\0';
}

lg_var_mapping *lg_var_open(void)
{
    lg_var_mapping *m;
    int i;

    m = (lg_var_mapping*)calloc(1, sizeof(*m));
    if (!m) return NULL;

    costruisci_var(&m->ind_sh_top, &m->vars, &m->shm_id);
    if (!m->ind_sh_top || !m->vars) {
        free(m);
        return NULL;
    }
    m->n_models = numero_modelli(m->ind_sh_top);
    m->n_vars   = numero_variabili(m->ind_sh_top);
    if (m->n_vars <= 0) {
        free(m);
        return NULL;
    }

    /* trimmed names cache */
    m->names_trim = (char*)malloc((size_t)m->n_vars * 9u);
    if (!m->names_trim) goto fail;
    for (i = 0; i < m->n_vars; i++)
        trim_name(m->vars[i].nome, &m->names_trim[i * 9]);

    /* sorted index by trimmed name */
    m->by_name_idx = (int*)malloc((size_t)m->n_vars * sizeof(int));
    if (!m->by_name_idx) goto fail;
    for (i = 0; i < m->n_vars; i++) m->by_name_idx[i] = i;
    g_sort_names_trim = m->names_trim;
    qsort(m->by_name_idx, (size_t)m->n_vars, sizeof(int), cmp_idx_by_trim);
    g_sort_names_trim = NULL;

    /* count + collect inputs / outputs */
    for (i = 0; i < m->n_vars; i++) {
        if (m->vars[i].tipo == STATO)       m->n_output++;
        else if (m->vars[i].tipo == INGRESSO_NC) m->n_input++;
    }
    if (m->n_input  > 0) m->input_idx  = (int*)malloc((size_t)m->n_input  * sizeof(int));
    if (m->n_output > 0) m->output_idx = (int*)malloc((size_t)m->n_output * sizeof(int));
    if ((m->n_input > 0 && !m->input_idx) ||
        (m->n_output > 0 && !m->output_idx))
        goto fail;
    {
        int ki = 0, ko = 0;
        for (i = 0; i < m->n_vars; i++) {
            if (m->vars[i].tipo == STATO)            m->output_idx[ko++] = i;
            else if (m->vars[i].tipo == INGRESSO_NC) m->input_idx [ki++] = i;
        }
    }

    /* Build distinct-addr view: walk VARIABILI[] in order and pick the
     * first entry for each addr. O(N^2) is fine here (N ~ a few k). */
    m->unique_idx = (int*)malloc((size_t)m->n_vars * sizeof(int));
    if (!m->unique_idx) goto fail;
    {
        int j;
        for (i = 0; i < m->n_vars; i++) {
            int dup = 0;
            for (j = 0; j < m->n_unique; j++) {
                if (m->vars[m->unique_idx[j]].addr == m->vars[i].addr) {
                    dup = 1;
                    break;
                }
            }
            if (!dup)
                m->unique_idx[m->n_unique++] = i;
        }
    }

    return m;

fail:
    lg_var_close(m);
    return NULL;
}

void lg_var_close(lg_var_mapping *m)
{
    if (!m) return;
    free(m->names_trim);
    free(m->by_name_idx);
    free(m->input_idx);
    free(m->output_idx);
    free(m->unique_idx);
    /* SHM is shared; do NOT detach here. */
    free(m);
}

int lg_var_count(const lg_var_mapping *m)        { return m ? m->n_vars   : 0; }
int lg_var_model_count(const lg_var_mapping *m)  { return m ? m->n_models : 0; }

const char *lg_var_model_name(const lg_var_mapping *m, int model_1based)
{
    if (!m || model_1based < 1 || model_1based > m->n_models) return NULL;
    return nome_modello(m->ind_sh_top, model_1based);
}

int lg_var_index_by_name(const lg_var_mapping *m, const char *name)
{
    char key[NAME_PAD_LEN + 1];
    int  lo, hi, found = -1;

    if (!m || !name || !*name) return -1;
    canon_trim(name, key);   /* "TM      " -> "TM", "TM" -> "TM" */

    /* binary search over by_name_idx (sorted by trimmed name). When the
     * key is duplicated, return the FIRST occurrence in sort order so
     * the result is deterministic. */
    lo = 0; hi = m->n_vars - 1;
    while (lo <= hi) {
        int mid = (lo + hi) >> 1;
        int idx = m->by_name_idx[mid];
        int c = strcmp(&m->names_trim[idx * 9], key);
        if (c == 0) { found = mid; hi = mid - 1; }
        else if (c < 0) lo = mid + 1;
        else            hi = mid - 1;
    }
    return (found >= 0) ? m->by_name_idx[found] : -1;
}

int lg_var_index_by_addr(const lg_var_mapping *m, int addr)
{
    int i;
    if (!m) return -1;
    for (i = 0; i < m->n_vars; i++)
        if (m->vars[i].addr == addr)
            return i;
    return -1;
}

int lg_var_get_info(const lg_var_mapping *m, int idx, lg_var_info *out)
{
    const VARIABILI *v;
    if (!m || !out || idx < 0 || idx >= m->n_vars) return -1;
    v = &m->vars[idx];
    memcpy(out->name, &m->names_trim[idx * 9], 9);
    out->descr = v->descr;
    out->model = (int)v->mod;
    out->block = (int)v->blocco;
    out->addr  = v->addr;
    switch (v->tipo) {
        case STATO:       out->kind = LG_VAR_OUTPUT;          break;
        case INGRESSO_NC: out->kind = LG_VAR_INPUT_FREE;      break;
        case INGRESSO_C:  out->kind = LG_VAR_INPUT_CONNECTED; break;
        default:          out->kind = LG_VAR_INPUT_CONNECTED; break;
    }
    return 0;
}

int lg_var_input_count (const lg_var_mapping *m) { return m ? m->n_input  : 0; }
int lg_var_output_count(const lg_var_mapping *m) { return m ? m->n_output : 0; }

int lg_var_input_at(const lg_var_mapping *m, int n)
{
    if (!m || n < 0 || n >= m->n_input) return -1;
    return m->input_idx[n];
}

int lg_var_output_at(const lg_var_mapping *m, int n)
{
    if (!m || n < 0 || n >= m->n_output) return -1;
    return m->output_idx[n];
}

int lg_var_unique_count(const lg_var_mapping *m)
{
    return m ? m->n_unique : 0;
}

int lg_var_unique_at(const lg_var_mapping *m, int n)
{
    if (!m || n < 0 || n >= m->n_unique) return -1;
    return m->unique_idx[n];
}
