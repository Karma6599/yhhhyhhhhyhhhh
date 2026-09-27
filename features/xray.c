#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <pthread.h>

extern int evasion_set(const char *name, uint32_t value, int is_parameter);

extern pthread_once_t g_snapshot_plus_once;
extern long (*snapshot_plus_active_fn)(void);
extern void snapshot_plus_once_init(void);

extern void *g_snapshot_keys_fn;
extern int   g_snapshot_keys_ready;

typedef int (*snapshot_keys_fn_t)(const char *const *keys, int32_t *triples,
                                  int count, int64_t *epoch, int32_t *status);

#define SNAPSHOT_TRIPLE_REQUESTED 0
#define SNAPSHOT_TRIPLE_EFFECTIVE 1
#define SNAPSHOT_TRIPLE_FLAGS     2

#define SNAPSHOT_FLAG_ACTIVE 2

typedef struct {
    uint64_t zero;
    uint64_t epoch;
    int32_t  xray_enabled;
    int32_t  aim_enabled;
    int32_t  target_mode;
    int32_t  show_names;
    uint64_t reserved_20;
    uint64_t reserved_28;
    int32_t  box_w;
    int32_t  box_h;
    uint64_t reserved_38[8];
} shadowx_snapshot_t;

_Static_assert(sizeof(shadowx_snapshot_t) == 0x78, "shadowx_snapshot_t size");

bool nexus_shadowx_set_feature(const char *name, uint32_t value)
{
    return evasion_set(name, value, 0) == 1;
}

bool nexus_shadowx_set_param(const char *name, uint32_t value)
{
    return evasion_set(name, value, 1) == 1;
}

int shadowx_snapshot_fetch(shadowx_snapshot_t *out)
{
    static const char *const keys[4] = {
        "isXrayEnabled",
        "xrayShowTargetName",
        "xrayTargetMode",
        "aopAimEnabled",
    };

    if (__atomic_load_n(&g_snapshot_keys_ready, __ATOMIC_ACQUIRE) != 1
        || g_snapshot_keys_fn == NULL)
        return 0;

    pthread_once(&g_snapshot_plus_once, snapshot_plus_once_init);
    if (snapshot_plus_active_fn == NULL || snapshot_plus_active_fn() != 1)
        return 0;

    int32_t triples[12];
    int64_t epoch = 0;
    int32_t status = -1;

    snapshot_keys_fn_t query = (snapshot_keys_fn_t)g_snapshot_keys_fn;
    if (query(keys, triples, 4, &epoch, &status) != 1)
        return 0;
    if (epoch == 0 || status != 0)
        return 0;

    int32_t xray_flags     = triples[0 * 3 + SNAPSHOT_TRIPLE_FLAGS];
    int32_t showname_flags = triples[1 * 3 + SNAPSHOT_TRIPLE_FLAGS];
    if (xray_flags != SNAPSHOT_FLAG_ACTIVE || showname_flags != SNAPSHOT_FLAG_ACTIVE)
        return 0;

    int32_t xray_eff      = triples[0 * 3 + SNAPSHOT_TRIPLE_EFFECTIVE];
    int32_t showname_eff  = triples[1 * 3 + SNAPSHOT_TRIPLE_EFFECTIVE];
    int32_t target_eff    = triples[2 * 3 + SNAPSHOT_TRIPLE_EFFECTIVE];
    int32_t aim_eff       = triples[3 * 3 + SNAPSHOT_TRIPLE_EFFECTIVE];

    if (xray_eff < 0 || xray_eff > 1)
        return 0;
    if (showname_eff < 0 || showname_eff > 1)
        return 0;
    if (target_eff < 1 || target_eff > 2)
        return 0;
    if (aim_eff < 0 || aim_eff > 1)
        return 0;

    memset(out, 0, sizeof *out);
    out->epoch = epoch;
    out->xray_enabled = xray_eff;
    out->aim_enabled = aim_eff;
    out->target_mode = target_eff;
    out->show_names = showname_eff;
    out->box_w = 100;
    out->box_h = 200;
    return 1;
}
