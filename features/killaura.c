#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#define EV_MAX_NAME 0x60

#define PROP_FEATURE 0
#define PROP_PARAM   1

extern int32_t evasion_get_requested(const char *name);
extern int32_t evasion_prop_state_index(const char *name);
extern int32_t evasion_prop_order(const char *name);
extern uint64_t evasion_state_revision(void);
extern int evasion_prediction_version_ok(uint32_t dep_flags, int predict_enabled);

extern uint32_t g_adapter_ready_a;
extern uint32_t g_adapter_ready_b;
extern uint32_t g_evasion_status;
extern uint64_t g_route_bind_epoch;
extern uint64_t g_registry_epoch;
extern uint64_t g_route_token;

extern void *g_snapshot_keys_fn;

typedef int (*snapshot_keys_fn_t)(const char *const *keys, int32_t *triples,
                                  int count, int64_t *epoch, int32_t *status);

#define SNAPSHOT_TRIPLE_REQUESTED 0
#define SNAPSHOT_TRIPLE_EFFECTIVE 1
#define SNAPSHOT_TRIPLE_FLAGS     2
#define SNAPSHOT_FLAG_ACTIVE      2

#define AURA_ROUTE_TAG 0x0000002800000001ull

extern uintptr_t runtime_read_handle(void);
extern uint32_t g_ctx_hp_cur;
extern uint32_t g_ctx_hp_max;

int evasion_key_forces_pass(const char *name)
{
    if (name == NULL)
        return 0;

    if (strcmp(name, "killauraNoWall") == 0 ||
        strcmp(name, "killauraNoBall") == 0 ||
        strcmp(name, "espShowNames") == 0 ||
        strcmp(name, "espShowDistance") == 0 ||
        strcmp(name, "espShowHitbox") == 0 ||
        strcmp(name, "espShowCircle") == 0)
        return 1;

    return strcmp(name, "espShowAimLine") == 0;
}

int evasion_brawler_bitmap_index(const char *name)
{
    if (name == NULL)
        return -1;

    size_t len = strlen(name);
    if (len == 0 || len >= EV_MAX_NAME)
        return -1;

    if (evasion_prop_order(name) > 0x44)
        return -1;

    int32_t idx = evasion_prop_state_index(name);
    if (idx < 0)
        return -1;

    if (idx == evasion_prop_state_index("killauraEnabled"))
        return 0;
    if (idx == evasion_prop_state_index("killauraMainAttack"))
        return 1;
    if (idx == evasion_prop_state_index("aopPredictEnabled"))
        return 2;
    return -1;
}

bool evasion_aura_route_compose(uint32_t dep_flags, uint32_t adapter_mask,
                                uint64_t *route_out, uint32_t *status_out)
{
    bool ok = false;
    uint32_t status;

    if (evasion_get_requested("killauraEnabled") != 0
        && evasion_get_requested("killauraMainAttack") != 0) {

        uint32_t ready = (g_adapter_ready_a != 0 ? 1u : 0u)
                       | (g_adapter_ready_b != 0 ? 2u : 0u);

        if (ready == 3) {
            if ((dep_flags ^ 0xffffffffu) & 3u) {
                status = 3;
                goto done;
            }
            if (evasion_prediction_version_ok(
                    dep_flags,
                    (int)evasion_get_requested("aopPredictEnabled")) != 0) {
                bool mask_ok = ((adapter_mask ^ 0xffffffffu) & 0x7fu) == 0;
                ok = mask_ok && g_evasion_status == 0;
                status = ok ? 0 : 5;
                goto done;
            }
            status = 4;
        } else {
            status = 2;
        }
    } else {
        status = 1;
    }

done:
    if (status_out != NULL)
        *status_out = status;

    if (ok) {
        route_out[0] = AURA_ROUTE_TAG;
        route_out[1] = evasion_state_revision();
        route_out[2] = g_route_bind_epoch;
        route_out[3] = g_registry_epoch;
        route_out[4] = g_route_token;
    }
    return ok;
}

int killaura_channels_snapshot(uint64_t *out)
{
    static const char *const keys[6] = {
        "autofarmEnabled",
        "killauraEnabled",
        "killauraSuper",
        "killauraGadget",
        "combatFireInterval",
        "killauraDisableBelowHealthPercent",
    };

    if (g_snapshot_keys_fn == NULL)
        return 0;

    int32_t triples[18];
    int64_t epoch = 0;
    int32_t status = 0;

    snapshot_keys_fn_t query = (snapshot_keys_fn_t)g_snapshot_keys_fn;
    if (query(keys, triples, 6, &epoch, &status) != 1)
        return 0;
    if (epoch == 0 || status != 0)
        return 0;

    int32_t autofarm_eff    = triples[0 * 3 + SNAPSHOT_TRIPLE_EFFECTIVE];
    int32_t autofarm_flags  = triples[0 * 3 + SNAPSHOT_TRIPLE_FLAGS];
    int32_t killaura_eff    = triples[1 * 3 + SNAPSHOT_TRIPLE_EFFECTIVE];
    int32_t killaura_flags  = triples[1 * 3 + SNAPSHOT_TRIPLE_FLAGS];
    int32_t super_eff       = triples[2 * 3 + SNAPSHOT_TRIPLE_EFFECTIVE];
    int32_t super_flags     = triples[2 * 3 + SNAPSHOT_TRIPLE_FLAGS];
    int32_t gadget_eff      = triples[3 * 3 + SNAPSHOT_TRIPLE_EFFECTIVE];
    int32_t gadget_flags    = triples[3 * 3 + SNAPSHOT_TRIPLE_FLAGS];
    int32_t interval_req    = triples[4 * 3 + SNAPSHOT_TRIPLE_REQUESTED];
    int32_t health_threshold = triples[5 * 3 + SNAPSHOT_TRIPLE_REQUESTED];

    if (!(autofarm_eff < 2 && killaura_eff < 2 && super_eff < 2 && gadget_eff < 2))
        return 0;

    uint32_t autofarm_active =
        (autofarm_eff == 1 && autofarm_flags == SNAPSHOT_FLAG_ACTIVE) ? 1u : 0u;
    uint32_t killaura_ok =
        (killaura_eff == 1 && killaura_flags == SNAPSHOT_FLAG_ACTIVE) ? 1u : 0u;

    if (autofarm_active == 0) {
        float ratio;
        if (g_ctx_hp_max == 0 || g_ctx_hp_max < g_ctx_hp_cur)
            ratio = 100.0f;
        else
            ratio = ((float)g_ctx_hp_cur / (float)g_ctx_hp_max) * 100.0f;
        if (ratio < (float)health_threshold)
            killaura_ok = 0;
    }

    uint32_t super_fire = (super_eff == 1) ? killaura_ok : 0;
    uint32_t super_final = (super_flags == SNAPSHOT_FLAG_ACTIVE) ? super_fire : 0;
    uint32_t gadget_fire = (gadget_eff == 1) ? killaura_ok : 0;
    uint32_t gadget_final = (gadget_flags == SNAPSHOT_FLAG_ACTIVE) ? gadget_fire : 0;

    int32_t interval = interval_req;
    if (interval < 0x79)
        interval = 0x78;

    *(uint64_t *)((char *)out + 0x10) = (uint64_t)epoch;
    *(uint32_t *)((char *)out + 0x38) = autofarm_active;
    *(uint32_t *)((char *)out + 0x3c) = super_final;
    *(uint32_t *)((char *)out + 0x40) = gadget_final;
    *(int32_t  *)((char *)out + 0x44) = interval;

    if (autofarm_active || super_final || gadget_final)
        return runtime_read_handle() != 0;
    return 0;
}
