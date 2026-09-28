#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

extern void log_event(const char *category, const char *event, const char *json);

extern void *g_snapshot_keys_fn;
extern int g_snapshot_keys_ready;

typedef int (*snapshot_keys_fn_t)(const char *const *keys, int32_t *triples,
                                  int count, int64_t *epoch, int32_t *status);

extern uint64_t g_ctx_bolt_marker;
extern uint64_t g_ctx_a;
extern uint64_t g_ctx_c;
extern uint64_t g_ctx_frame_no;
extern uint64_t g_ctx_self;
extern uint64_t g_team_world;
extern uint64_t g_team_ctrl;
extern uint64_t g_los_world;
extern uint64_t g_los_ctrl;
extern uint32_t g_ctx_hp_cur;
extern uint32_t g_ctx_hp_max;
extern uintptr_t g_engine_base;
extern uint64_t g_colt_ctx;
extern uint64_t g_farm_world;

extern int evasion_key_adapter_family(const char *name);
extern int brawler_mod_key_family(const char *name);
extern int bolt_mod_armed(void);
extern int evasion_runtime_gate(void);
extern uintptr_t game_read(uintptr_t handle, uintptr_t addr, void *out, uint32_t len);
extern int remote_guarded_apply(void *record, uintptr_t target,
                                const void *in, void *out);
extern void colt_write_apply(void *payload);
extern int dyna_state_read(uintptr_t ctx, void *read_fn, int flags, void *out);
extern int dyna_jump_armed(void);
extern int autofarm_plan_seed(uint64_t epoch, void *out);
extern int game_object_resolve(uintptr_t engine, uintptr_t obj, void *read_fn,
                               int flags, void *out);
extern int entity_team_block(uintptr_t world, uintptr_t ctrl,
                             uint32_t team_a, uint32_t team_b);
extern float entity_threat_score(uintptr_t entity);
extern int self_resolve(void *self_ctx, void *out);
extern int page_perm_check(uintptr_t addr, int64_t *out);
extern int movement_ctx_valid(uint64_t ctx, uint64_t obj);
extern int dyna_engine_gate(void);
extern int autofarm_decide(float hp_own, float hp_target, float threat_own,
                           float threat_target, float dist, void *state);
extern uintptr_t farm_wall_probe(float x, float y, int mode);

extern int nexus_plus_key(const char *key);
extern int32_t port_id_lookup(const char *key);
extern int32_t port_bit_index(const char *key);
extern uint64_t port_bitmap(void);
extern uint32_t port_gate_version(void);
extern int prediction_gate(uint64_t bitmap, int32_t value);
extern int port_gate_check(int32_t port_id, uint32_t gate, int flags);
extern int evasion_key_force_flag(const char *key);
extern const void *prop_entry_lookup(const char *key);
extern int triple_gate_resolve(uint64_t bitmap, uint32_t mask, void *out,
                               int32_t *out_status);
extern int port_bitmap_gate(uint64_t bitmap);
extern int holdfire_gate_a(void);
extern int holdfire_gate_b(int flags);
extern void spin_status_fill(int32_t *out);
extern void port_status_fill(const char *key, int32_t *out);
extern void handler_state_refresh(void);
extern void family_once_init(void);

extern char g_handler_lock;
extern uint64_t g_state_epoch;
extern int32_t g_state_status;
extern uint64_t g_state_aux;
extern uint64_t g_state_epoch2;
extern pthread_once_t g_family_once;
extern void *g_family_once_fn;

extern uint64_t g_pin_family_epoch;
extern uint64_t g_pin_family_aux;
extern void *g_pin_family_check;
extern uint64_t g_nexusplus_family_epoch;
extern uint64_t g_nexusplus_family_aux;
extern void *g_nexusplus_family_check;
extern uint64_t g_brawler_family_epoch;
extern uint64_t g_brawler_family_aux;
extern void *g_brawler_family_check;
extern void *g_adapter_ctx;
extern void *g_adapter_mask_fn;
extern uint64_t g_pin_aux_epoch;
extern uint64_t g_holdfire_epoch;
extern uint64_t g_holdfire_aux;
extern uint64_t g_holdfire_aux2;
extern uint32_t g_triple_gate_a;
extern uint32_t g_triple_gate_b;
extern uint32_t g_triple_gate_c;

extern uint64_t g_dyna_guard_a;
extern uint64_t g_dyna_guard_b;
extern uint64_t g_dyna_guard_c;
extern uint64_t g_dyna_world_last;
extern uint64_t g_dyna_guard_d;
extern uint64_t g_dyna_own_obj;
extern uint64_t g_autofarm_state[13];
extern uint64_t g_farm_last_frame;
extern uint64_t g_farm_last_fire_ms;

extern int32_t g_ui_query_ready;
extern void *g_ui_query_fn;
extern pthread_once_t g_pin_once;
extern void *g_pin_once_fn;
extern long (*g_ui_key_lookup)(const char *key);

#define ROD_STATUS_TAG  (*(const uint64_t *)(uintptr_t)0x1047a0)
#define ROD_PIN_KEYS    ((const char *const *)(uintptr_t)0x1c4320)

#define STATUS_OUT_VERSION 1
#define STATUS_OUT_SIZE    0x48

#define ENGINE_PLAY_ALLOC_OFF   0x11a2840u
#define ENGINE_PLAY_INIT_OFF    0x0f41ba0u
#define ENGINE_PLAY_SEND_OFF    0x0ac319cu
#define ENGINE_PLAY_STATIC_OFF  0x13053c0u
#define ENGINE_PLAY_VTABLE_OFF  0x11ebcd0u
#define ENGINE_AIM_CHECK_OFF    0xecfb74u
#define ENGINE_ATTACK_OFF       0xb2e994u

typedef struct {
    const char *name;
    int32_t state_index;
    int32_t kind;
    int32_t vmin;
    int32_t vmax;
    int32_t f24;
    int32_t f28;
    int32_t f32;
    int32_t f36;
} evasion_prop_entry_t;

extern evasion_prop_entry_t g_prop_table[142];
extern int32_t g_prop_state[144];

static void handler_lock_acquire(void)
{
    while (__atomic_test_and_set(&g_handler_lock, __ATOMIC_ACQUIRE))
        ;
}

static void handler_lock_release(void)
{
    g_handler_lock = 0;
}

static const evasion_prop_entry_t *prop_find(const char *name)
{
    if (name == NULL)
        return NULL;
    for (int i = 0; i < 142; i++) {
        if (strcmp(name, g_prop_table[i].name) == 0)
            return &g_prop_table[i];
    }
    return NULL;
}

static uint32_t family_gate(uint64_t family_epoch, uint64_t family_aux,
                            void *family_check)
{
    int (*check)(void);
    int (*once_fn)(int);

    if (family_epoch == 0 || family_check == NULL)
        return 0;

    check = (int (*)(void))family_check;
    if (check() != 1 || g_state_epoch2 != family_epoch || g_state_aux != family_aux)
        return 0;

    pthread_once(&g_family_once, family_once_init);
    if (g_family_once_fn == NULL)
        return 0;

    once_fn = (int (*)(int))g_family_once_fn;
    return (uint32_t)(once_fn(1) == 1);
}

static uint32_t brawler_family_gate(const char *key, uint64_t family_epoch,
                                    uint64_t family_aux, void *family_check)
{
    int (*check)(void);
    int (*once_fn)(int);

    if (family_epoch == 0 || family_check == NULL)
        return 0;

    check = (int (*)(void))family_check;
    if (check() != 1 || g_state_epoch2 != family_epoch || g_state_aux != family_aux)
        return 0;

    if (brawler_mod_key_family(key) == 0) {
        pthread_once(&g_family_once, family_once_init);
        if (g_family_once_fn == NULL)
            return 0;
        once_fn = (int (*)(int))g_family_once_fn;
        return (uint32_t)(once_fn(1) == 1);
    }
    return 1;
}

static uint32_t adapter_mask(void)
{
    uint32_t (*mask_fn)(void *);

    if (g_adapter_mask_fn == NULL)
        return 0;

    mask_fn = (uint32_t (*)(void *))g_adapter_mask_fn;
    return mask_fn(g_adapter_ctx);
}

void nexus_evasion_handler_status_v1(const char *name, int32_t *out)
{
    const evasion_prop_entry_t *entry;
    int32_t requested;
    uint32_t gate;
    uint32_t mask;
    uint32_t effective;
    uint32_t masked;
    uint32_t full;
    int code;
    int registered;

    if (evasion_key_adapter_family(name) == 0) {
        if (name == NULL)
            return;

        if (strcmp(name, "pinEnabled") == 0 || strcmp(name, "sprayEnabled") == 0) {
            if (out == NULL || out[0] != STATUS_OUT_VERSION
                || out[1] != STATUS_OUT_SIZE)
                return;

            handler_lock_acquire();
            handler_state_refresh();

            entry = prop_find(name);
            if (entry == NULL) {
                handler_lock_release();
                return;
            }
            requested = g_prop_state[entry->state_index];

            gate = family_gate(g_pin_family_epoch, g_pin_family_aux,
                               g_pin_family_check);
            mask = adapter_mask();
            effective = 0;
            code = 5;
            if (requested == 0)
                code = 1;
            if (gate == 0) {
                code = 3;
            } else {
                effective = (uint32_t)(requested != 0);
            }
            masked = 0;
            if (g_state_status == 0)
                masked = effective;
            full = 0;
            if ((mask ^ 0xffffffffu) & 0x7fu)
                full = 0;
            else
                full = masked;
            registered = g_pin_family_epoch != 0;

            *(uint64_t *)out = ROD_STATUS_TAG;
            out[2] = requested;
            out[3] = registered;
            out[4] = (int32_t)gate;
            out[5] = (int32_t)effective;
            out[6] = (int32_t)full;
            out[7] = registered ? code : 2;
            out[8] = (int32_t)mask;
            out[9] = 0;
            *(uint64_t *)(out + 10) = g_state_epoch;
            *(uint64_t *)(out + 12) = g_pin_aux_epoch;
            *(uint64_t *)(out + 14) = g_pin_family_epoch;
            *(uint64_t *)(out + 16) = 0;

            handler_lock_release();
            return;
        }

        if (nexus_plus_key(name) != 0) {
            if (out == NULL || out[0] != STATUS_OUT_VERSION
                || out[1] != STATUS_OUT_SIZE)
                return;

            handler_lock_acquire();
            handler_state_refresh();

            entry = prop_find(name);
            if (entry == NULL) {
                handler_lock_release();
                return;
            }
            requested = g_prop_state[entry->state_index];

            gate = family_gate(g_nexusplus_family_epoch, g_nexusplus_family_aux,
                               g_nexusplus_family_check);
            mask = adapter_mask();
            effective = 0;
            code = 5;
            if (requested == 0)
                code = 1;
            if (gate == 0) {
                code = 3;
            } else {
                effective = (uint32_t)(requested != 0);
            }
            masked = 0;
            if (g_state_status == 0)
                masked = effective;
            full = 0;
            if ((mask ^ 0xffffffffu) & 0x7fu)
                full = 0;
            else
                full = masked;
            registered = g_nexusplus_family_epoch != 0;

            *(uint64_t *)out = ROD_STATUS_TAG;
            out[2] = requested;
            out[3] = registered;
            out[4] = (int32_t)gate;
            out[5] = (int32_t)effective;
            out[6] = (int32_t)full;
            out[7] = registered ? code : 2;
            out[8] = (int32_t)mask;
            out[9] = 0;
            *(uint64_t *)(out + 10) = g_state_epoch;
            *(uint64_t *)(out + 12) = g_pin_aux_epoch;
            *(uint64_t *)(out + 14) = g_nexusplus_family_epoch;
            *(uint64_t *)(out + 16) = 0;

            handler_lock_release();
            return;
        }

        if (strcmp(name, "holdToShootEnabled") == 0
            || strcmp(name, "holdToShootAim") == 0
            || strcmp(name, "holdToShootRangeCheck") == 0) {
            int gate_a;
            int gate_b;

            if (out == NULL)
                return;
            if (out[0] != STATUS_OUT_VERSION || out[1] != STATUS_OUT_SIZE)
                return;

            handler_lock_acquire();
            handler_state_refresh();

            entry = prop_find(name);
            if (entry == NULL) {
                handler_lock_release();
                return;
            }
            requested = g_prop_state[entry->state_index];

            gate_a = holdfire_gate_a();
            gate_b = holdfire_gate_b(0);
            registered = g_holdfire_epoch != 0;

            if (!registered)
                code = 2;
            else if (gate_a == 0)
                code = 3;
            else {
                code = 4;
                if (gate_b != 0)
                    code = 5;
                if (requested == 0)
                    code = 1;
            }

            *(uint64_t *)out = ROD_STATUS_TAG;
            out[2] = requested;
            out[3] = registered;
            out[4] = gate_a;
            out[5] = (int32_t)(requested != 0 && gate_b != 0);
            out[6] = 0;
            out[7] = code;
            out[8] = 0;
            out[9] = 0;
            *(uint64_t *)(out + 10) = g_state_epoch;
            *(uint64_t *)(out + 12) = g_holdfire_aux2;
            *(uint64_t *)(out + 14) = g_holdfire_epoch;
            *(uint64_t *)(out + 16) = g_holdfire_aux;

            handler_lock_release();
            return;
        }

        if (strcmp(name, "isSpinEnabled") == 0) {
            spin_status_fill(out);
            return;
        }

        if (port_id_lookup(name) >= 0) {
            port_status_fill(name, out);
            return;
        }

        if (out == NULL || out[0] != STATUS_OUT_VERSION
            || out[1] != STATUS_OUT_SIZE)
            return;

        {
            const void *prop = prop_entry_lookup(name);
            const evasion_prop_entry_t *prop_entry;
            uint64_t bitmap;
            uint32_t gate_flags;
            uint32_t port_bit;
            uint32_t bit_set;
            uint32_t port_ok;
            int32_t port_idx;
            int bitmap_gate;
            int triple_ok;
            int32_t triple_status = 0;
            uint8_t triple_out[48];
            uint32_t port_valid;

            if (prop == NULL)
                return;
            prop_entry = (const evasion_prop_entry_t *)prop;
            if (prop_entry->kind != 0)
                return;

            handler_lock_acquire();
            handler_state_refresh();

            port_idx = port_bit_index(name);
            bitmap = port_bitmap();
            gate_flags = (uint32_t)(g_triple_gate_a != 0)
                       | (uint32_t)(g_triple_gate_b != 0) << 1
                       | (uint32_t)(g_triple_gate_c != 0) << 2;
            mask = adapter_mask();
            memset(triple_out, 0, sizeof triple_out);
            triple_ok = triple_gate_resolve(bitmap, mask, triple_out, &triple_status);

            port_bit = 1u << (port_idx & 0x1f);
            requested = g_prop_state[prop_entry->state_index];
            bit_set = (uint32_t)(bitmap & port_bit);
            port_ok = 0;
            if (bit_set != 0)
                port_ok = (uint32_t)(port_idx >= 0);
            bitmap_gate = port_bitmap_gate(bitmap);

            port_valid = (uint32_t)(port_idx >= 0 && (gate_flags & port_bit) != 0);
            code = 2;
            if (port_valid != 0)
                code = 3;
            if (port_valid == 1 && bit_set != 0) {
                if (requested == 0) {
                    code = 1;
                } else {
                    code = 4;
                    if (bitmap_gate != 0)
                        code = triple_status;
                }
            }

            effective = 0;
            if (requested != 0)
                effective = port_ok;
            masked = 0;
            if (bitmap_gate != 0)
                masked = effective;
            full = 0;
            if (triple_ok != 0)
                full = effective;

            *(uint64_t *)out = ROD_STATUS_TAG;
            out[2] = requested;
            out[3] = (int32_t)port_valid;
            out[4] = (int32_t)port_ok;
            out[5] = (int32_t)masked;
            out[6] = (int32_t)full;
            out[7] = code;
            out[8] = (int32_t)mask;
            out[9] = (int32_t)gate_flags;
            *(uint64_t *)(out + 10) = g_state_epoch;
            *(uint64_t *)(out + 12) = g_pin_aux_epoch;
            *(uint64_t *)(out + 14) = g_state_epoch2;
            *(uint64_t *)(out + 16) = 0;

            handler_lock_release();
            return;
        }
    }

    if (out == NULL || out[0] != STATUS_OUT_VERSION || out[1] != STATUS_OUT_SIZE)
        return;

    handler_lock_acquire();
    handler_state_refresh();

    entry = prop_find(name);
    if (entry == NULL) {
        handler_lock_release();
        return;
    }
    requested = g_prop_state[entry->state_index];

    gate = brawler_family_gate(name, g_brawler_family_epoch,
                               g_brawler_family_aux, g_brawler_family_check);
    mask = adapter_mask();
    effective = 0;
    code = 5;
    if (requested == 0)
        code = 1;
    if (gate == 0) {
        code = 3;
    } else {
        effective = (uint32_t)(requested != 0);
    }
    masked = 0;
    if (g_state_status == 0)
        masked = effective;
    full = 0;
    if ((mask ^ 0xffffffffu) & 0x7fu)
        full = 0;
    else
        full = masked;
    registered = g_brawler_family_epoch != 0;

    *(uint64_t *)out = ROD_STATUS_TAG;
    out[2] = requested;
    out[3] = registered;
    out[4] = (int32_t)gate;
    out[5] = (int32_t)effective;
    out[6] = (int32_t)full;
    out[7] = registered ? code : 2;
    out[8] = (int32_t)mask;
    out[9] = 0;
    *(uint64_t *)(out + 10) = g_state_epoch;
    *(uint64_t *)(out + 12) = g_pin_aux_epoch;
    *(uint64_t *)(out + 14) = g_brawler_family_epoch;
    *(uint64_t *)(out + 16) = 0;

    handler_lock_release();
}

void nexus_evasion_snapshot_keys_v1(const char *const *keys, int32_t *triples,
                                    long count, uint64_t *epoch_out,
                                    int32_t *status_out)
{
    const evasion_prop_entry_t *entries[142];
    uint64_t bitmap;
    uint32_t gate_version;
    int predict_block;
    const evasion_prop_entry_t *predict_entry;
    int i;

    if (count - 1U >= 0x8e || keys == NULL || triples == NULL
        || epoch_out == NULL || status_out == NULL)
        return;

    for (i = 0; i < count; i++) {
        const char *key = keys[i];
        size_t len;

        if (key == NULL)
            return;
        for (len = 0; key[len] != '\0'; len++) {
            if (len == 0x60)
                return;
        }
        entries[i] = prop_find(key);
        if (entries[i] == NULL)
            return;
    }

    handler_lock_acquire();
    handler_state_refresh();

    bitmap = port_bitmap();
    gate_version = port_gate_version();
    predict_entry = prop_find("aopPredictEnabled");
    predict_block = prediction_gate(bitmap,
                                     g_prop_state[predict_entry->state_index]);

    for (i = 0; i < count; i++) {
        const char *key = keys[i];
        const evasion_prop_entry_t *entry = entries[i];
        uint32_t port_bit;
        uint32_t port_ok;
        uint32_t gate;
        uint32_t effective;
        uint32_t flags;
        int32_t port_idx;
        int32_t pid;

        port_idx = port_bit_index(key);
        port_ok = 0;
        if (port_idx >= 0 && predict_block != 0)
            port_ok = (uint32_t)(bitmap >> (port_idx & 0x1f) & 1);

        port_bit = (uint32_t)g_prop_state[entry->state_index];
        pid = port_id_lookup(key);

        if (pid < 0) {
            gate = (uint32_t)(port_idx < 0);
        } else {
            port_ok = (uint32_t)port_gate_check(pid, gate_version, 0);
            gate = 1;
        }

        if (nexus_plus_key(key) != 0) {
            port_ok = 0;
            gate = (uint32_t)family_gate(g_nexusplus_family_epoch,
                                         g_nexusplus_family_aux,
                                         g_nexusplus_family_check);
        }

        if (strcmp(key, "pinEnabled") == 0 || strcmp(key, "sprayEnabled") == 0) {
            port_ok = 0;
            gate = (uint32_t)family_gate(g_pin_family_epoch, g_pin_family_aux,
                                         g_pin_family_check);
        }

        if (evasion_key_adapter_family(key) != 0) {
            gate = 1;
            port_ok = brawler_family_gate(key, g_brawler_family_epoch,
                                          g_brawler_family_aux,
                                          g_brawler_family_check);
        }

        effective = port_bit;
        if (port_ok == 0 || gate == 0)
            effective = 0;
        if (entry->kind != 0)
            effective = port_bit;

        if (gate == 0) {
            if (entry->f32 == 0)
                flags = (uint32_t)(entry->kind != 0);
            else
                flags = 3;
        } else {
            flags = (uint32_t)(port_ok != 0) << 1;
        }

        triples[i * 3 + 0] = (int32_t)port_bit;
        triples[i * 3 + 1] = (int32_t)effective;
        triples[i * 3 + 2] = (int32_t)flags;
    }

    for (i = 0; i < count; i++) {
        if (evasion_key_force_flag(keys[i]) != 0) {
            triples[i * 3 + 1] = triples[i * 3 + 0];
            triples[i * 3 + 2] = 1;
        }
    }

    *epoch_out = g_state_epoch;
    *status_out = g_state_status;

    handler_lock_release();
}

int autofarm_armed(void)
{
    const char *keys[1] = { "autofarmEnabled" };
    int32_t triples[3];
    int64_t epoch = 0;
    int32_t status = -1;
    snapshot_keys_fn_t query = (snapshot_keys_fn_t)g_snapshot_keys_fn;

    if (g_snapshot_keys_ready != 1 || query == NULL)
        return 0;
    if (query(keys, triples, 1, &epoch, &status) != 1)
        return 0;
    if (epoch == 0 || status != 0 || triples[2] != 2 || triples[1] != 1)
        return 0;

    keys[0] = "autofarmAttackEnemies";
    if (query(keys, triples, 1, &epoch, &status) != 1)
        return 0;
    if (epoch == 0 || status != 0 || triples[2] != 2 || triples[1] != 1)
        return 0;

    return evasion_runtime_gate() != 0;
}

int autofarm_write_verify(void *payload)
{
    uint64_t *rec = payload;
    const char *keys[1] = { "autofarmEnabled" };
    int32_t triples[3];
    int64_t epoch = 0;
    int32_t status = -1;
    snapshot_keys_fn_t query = (snapshot_keys_fn_t)g_snapshot_keys_fn;

    if (autofarm_armed() == 0)
        return 0;
    if (g_snapshot_keys_ready != 1 || query == NULL)
        return 0;
    if (query(keys, triples, 1, &epoch, &status) != 1)
        return 0;
    if (epoch == 0 || status != 0 || triples[2] != 2 || triples[1] != 1)
        return 0;
    if ((uint64_t)epoch != rec[2])
        return 0;

    return movement_ctx_valid(rec[0], rec[1]) != 0;
}

void pin_status_query(void *out)
{
    const char *const *keys = ROD_PIN_KEYS;
    int32_t t[12];
    int64_t epoch = 0;
    int32_t status = -1;
    snapshot_keys_fn_t query;

    if (g_ui_query_ready != 1)
        return;

    pthread_once(&g_pin_once, family_once_init);
    if (g_pin_once_fn == NULL)
        return;

    query = (snapshot_keys_fn_t)g_ui_query_fn;
    if (query == NULL)
        return;
    if (query(keys, t, 4, &epoch, &status) != 1)
        return;
    if (epoch == 0 || status != 0 || t[2] != 2)
        return;
    if (t[1] >= 2)
        return;
    if ((uint32_t)(t[7] - 100u) > 0x1324u || t[5] != 2)
        return;
    if (t[4] >= 2)
        return;
    if ((uint32_t)(t[10] - 5001u) <= 0xffffecdau)
        return;

    *(uint64_t *)((char *)out + 0x10) = (uint64_t)epoch;
    *(uint32_t *)((char *)out + 0x4c) = (uint32_t)t[1];
    *(uint32_t *)((char *)out + 0x50) = (uint32_t)t[4];
    *(int32_t *)((char *)out + 0x54) = t[7];
    *(int32_t *)((char *)out + 0x58) = t[10];
}

void autofarm_attack_tick(void *frame)
{
    const char *keys[1] = { "autofarmEnabled" };
    const char *fkeys[2] = { "autofarmFollowTarget", "combatFireInterval" };
    int32_t triples[3];
    int32_t ft[6];
    int64_t epoch = 0;
    int64_t f_epoch = 0;
    int32_t status = -1;
    int32_t f_status = -1;
    snapshot_keys_fn_t query = (snapshot_keys_fn_t)g_snapshot_keys_fn;
    uint8_t farm_state[2];
    uint64_t seed[6];
    int32_t resolve[32];
    struct timespec ts;
    uint64_t now_ms;
    uint32_t interval;
    int decide;

    if (frame == NULL)
        return;
    if (g_snapshot_keys_ready != 1 || query == NULL)
        return;
    if (query(keys, triples, 1, &epoch, &status) != 1)
        return;
    if (epoch == 0 || status != 0 || triples[2] != 2 || triples[1] != 1)
        return;
    if (bolt_mod_armed() != 0)
        return;
    if (autofarm_armed() == 0)
        return;

    if (query(fkeys, ft, 2, &f_epoch, &f_status) != 1)
        return;
    if ((uint64_t)f_epoch != (uint64_t)epoch || f_status != 0)
        return;
    if (ft[0] < 0 || ft[0] >= 2)
        return;

    interval = (uint32_t)ft[3];
    if ((int32_t)interval < 0x79)
        interval = 0x78;
    if (interval > 0x1a3)
        interval = 0x1a4;

    if (g_farm_last_frame != g_ctx_frame_no) {
        g_farm_last_fire_ms = 0;
        g_farm_last_frame = g_ctx_frame_no;
    }

    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
        return;
    now_ms = (uint64_t)ts.tv_sec * 1000 + (uint64_t)ts.tv_nsec / 1000000;

    if (!(now_ms <= g_farm_last_fire_ms - 1
          || (uint64_t)interval <= now_ms - g_farm_last_fire_ms))
        return;

    if (g_dyna_guard_a != 0 || g_dyna_guard_b != 0 || g_dyna_guard_c != 0
        || g_dyna_world_last == g_ctx_a)
        return;
    if (dyna_engine_gate() == 0)
        return;

    memset(farm_state, 0, sizeof farm_state);
    if (dyna_state_read(g_farm_world, (void *)game_read, 0, farm_state) == 0)
        return;
    if (farm_state[0] != 0 || farm_state[1] != 0)
        return;
    if (dyna_jump_armed() != 0)
        return;

    memset(seed, 0, sizeof seed);
    if (autofarm_plan_seed((uint64_t)epoch, seed) == 0)
        return;

    memset(resolve, 0, sizeof resolve);
    if (game_object_resolve(g_engine_base, seed[0], (void *)game_read, 0,
                            resolve) == 0)
        return;
    if (resolve[5] == 0)
        return;
    if ((uint32_t)resolve[9] != (uint32_t)seed[3])
        return;
    if (entity_team_block(g_team_world, g_team_ctrl,
                          (uint32_t)resolve[4], (uint32_t)resolve[6]) != 0)
        return;

    {
        float hp_own = 1.0f;
        float hp_target = 1.0f;
        float threat_own;
        float threat_target;
        float dist;
        uint64_t self_out[2] = { 0, 0 };
        int64_t ctrl_list = 0;
        int64_t ctrl_entry = 0;
        uint32_t ctrl_count = 0;
        uint32_t ctrl_index = 0;
        uintptr_t h;
        float target_x = *(float *)((char *)resolve + 0x34);
        float target_y = *(float *)((char *)resolve + 0x38);
        int aim_ok;
        uintptr_t wall_h;
        uint8_t pair[8];
        uint64_t new_pos;
        struct {
            uint64_t *payload;
            void *read;
            void (*apply)(void *);
            int (*verify)(void *);
            uint64_t slots[8];
        } record;
        uint64_t payload[3];
        int result;

        if (g_ctx_hp_max != 0 && g_ctx_hp_cur <= g_ctx_hp_max)
            hp_own = (float)g_ctx_hp_cur / (float)g_ctx_hp_max;
        if (resolve[7] != 0 && (uint32_t)resolve[8] <= (uint32_t)resolve[7])
            hp_target = (float)(uint32_t)resolve[8] / (float)(uint32_t)resolve[7];

        threat_own = entity_threat_score((uintptr_t)g_ctx_self);
        threat_target = entity_threat_score((uintptr_t)resolve);
        dist = hypotf(target_x - (float)(int32_t)(uint32_t)g_los_world,
                      target_y - (float)(int32_t)(uint32_t)g_los_ctrl);

        decide = autofarm_decide(hp_own, hp_target, threat_own, threat_target,
                                 dist, g_autofarm_state);
        if (decide == 0)
            return;
        if (g_dyna_own_obj != g_ctx_self)
            return;
        if (self_resolve((void *)(uintptr_t)g_ctx_self, self_out) == 0)
            return;

        h = (uintptr_t)page_perm_check(g_colt_ctx, &ctrl_list);
        if (h == 0)
            return;
        h = game_read(h, g_colt_ctx + 0xc, &ctrl_count, 4);
        if (h == 0)
            return;
        h = game_read(h, g_colt_ctx + 0xe0, &ctrl_index, 4);
        if (h == 0)
            return;
        if (ctrl_count == 0 || ctrl_count >= 0x41)
            return;
        if ((int32_t)ctrl_index < 0 || ctrl_index >= ctrl_count)
            return;
        if (page_perm_check((uintptr_t)ctrl_list + (uintptr_t)ctrl_index * 8,
                            &ctrl_entry) == 0)
            return;

        aim_ok = ((int (*)(uint64_t, int64_t, uint64_t))
            (g_engine_base + ENGINE_AIM_CHECK_OFF))(
                *(uint64_t *)self_out, ctrl_entry, g_ctx_self);
        if (aim_ok == 0)
            return;

        {
            union { int32_t i; float f; } cx, cy;
            cx.f = target_y * 300.0f;
            cy.f = target_x * 300.0f;
            new_pos = ((uint64_t)(uint32_t)cx.i << 32) | (uint32_t)cy.i;
        }

        wall_h = farm_wall_probe(target_x, target_y, 1);
        if (wall_h == 0)
            return;
        wall_h = farm_wall_probe(*(float *)((char *)resolve + 0x38),
                                 *(float *)((char *)resolve + 0x34), 1);
        if (wall_h == 0)
            return;
        if (game_read(wall_h, g_farm_world + 0xfac, pair, 8) == 0)
            return;

        payload[0] = g_farm_world;
        payload[1] = g_ctx_self;
        payload[2] = (uint64_t)epoch;

        record.payload = payload;
        record.read = (void *)game_read;
        record.apply = colt_write_apply;
        record.verify = autofarm_write_verify;
        memset(record.slots, 0, sizeof record.slots);

        result = remote_guarded_apply(&record, g_farm_world, pair, &new_pos);
        if (result == 1) {
            if (autofarm_write_verify(payload) != 0) {
                uint64_t guard_saved = g_dyna_guard_d;
                uint64_t attack_result;

                g_dyna_guard_a = 4;
                g_dyna_guard_d = 1;
                g_dyna_guard_b = 1;
                g_farm_last_fire_ms = now_ms;

                attack_result = ((uint64_t (*)(uint64_t, uint64_t))
                    (g_engine_base + ENGINE_ATTACK_OFF))(payload[0], payload[1]);

                g_dyna_guard_a = 0;
                g_dyna_guard_b = 0;
                g_dyna_world_last = g_ctx_a;
                g_dyna_guard_d = guard_saved;

                char json[0xf0];
                snprintf(json, sizeof json,
                         ",\"target_gid\":%u,\"result\":%d,\"raw_x\":%d,\"raw_y\":%d",
                         (uint32_t)resolve[9], (uint32_t)attack_result,
                         (uint32_t)new_pos, (uint32_t)(new_pos >> 32));
                log_event("autofarm_fire", "original_wrapper", json);
            }
        } else if (result == -1) {
            log_event("fatal", "farm_xy_restore_unverified", NULL);
            abort();
        }
    }
}

uint32_t autofarm_play_again(uint32_t state)
{
    if (state - 1u > 2u || g_ui_key_lookup == NULL)
        return 0;

    if (g_ui_key_lookup("autofarmEnabled") != 1)
        return 0;

    {
        void (*alloc_fn)(void);
        void (*init_fn)(void);
        void (*send_fn)(void);
        uint64_t slot = 0;
        uint64_t vtable = 0;
        uint64_t fn = 0;
        int64_t obj = 0;
        const char *event;
        const char *sent;
        uint32_t ok;
        char json[0x60];

        alloc_fn = (void (*)(void))(g_engine_base + ENGINE_PLAY_ALLOC_OFF);
        init_fn = (void (*)(void))(g_engine_base + ENGINE_PLAY_INIT_OFF);
        send_fn = (void (*)(void))(g_engine_base + ENGINE_PLAY_SEND_OFF);

        if (alloc_fn == NULL || init_fn == NULL || send_fn == NULL)
            goto fail;
        if (game_read(1, g_engine_base + ENGINE_PLAY_STATIC_OFF, &slot, 8) == 0)
            goto fail;
        if (slot < 0x1000 || (slot & 7) != 0)
            goto fail;
        if (game_read(1, slot, &vtable, 8) == 0)
            goto fail;
        if (vtable != g_engine_base + ENGINE_PLAY_VTABLE_OFF)
            goto fail;
        if (game_read(1, vtable + 0x18, &fn, 8) == 0)
            goto fail;
        if (fn != g_engine_base + ENGINE_PLAY_SEND_OFF)
            goto fail;

        obj = ((int64_t (*)(uint64_t))alloc_fn)(0x98);
        if (obj == 0)
            goto fail;

        ((void (*)(int64_t, int, int))init_fn)(obj, 1, 0);
        ((void (*)(uint64_t, int64_t))send_fn)(slot, obj);

        ok = 1;
        event = "play_again_status";
        sent = "true";
        goto done;

fail:
        ok = 0;
        event = "native_send_failed";
        sent = "false";

done:
        snprintf(json, sizeof json, ",\"status\":%d,\"sent\":%s",
                 state, sent);
        log_event("autofarm_play_again", event, json);
        return ok;
    }
}

bool ui_persist_key(const char *key)
{
    if (strncmp(key, "nexus_quick_menu_", 0x11) == 0
        || strcmp(key, "nexus_autofarm_post_delay_ms") == 0
        || strcmp(key, "nexus_autofarm_click_gap_ms") == 0
        || strcmp(key, "nexus_auto_play_again_enabled") == 0
        || strcmp(key, "nexus_autofarm_show_stats") == 0)
        return true;

    return strcmp(key, "nexus_sx_outline_color_preset") == 0;
}
