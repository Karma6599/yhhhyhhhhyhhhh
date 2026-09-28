#define _GNU_SOURCE 1

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <dlfcn.h>

extern void log_event(const char *category, const char *event, const char *json);

extern int evasion_key_adapter_family(const char *name);
extern int brawler_mod_key_family(const char *name);
extern int nexus_plus_key(const char *key);
extern int32_t port_id_lookup(const char *key);
extern int32_t port_bit_index(const char *key);
extern uint64_t port_bitmap(void);
extern int port_bitmap_gate(uint64_t bitmap);
extern uint32_t port_gate_version(void);
extern int port_gate_check(int32_t port_id, uint32_t gate, int flags);
extern const void *prop_entry_lookup(const char *key);
extern int spin_active(void);
extern int port_active(const char *key, int flags);
extern int holdfire_gate_a(void);
extern int holdfire_gate_b(const char *key);
extern int hold_lease_impl(uint32_t action, uint32_t slot, uint64_t *out);
extern int dyna_jump_armed(void);
extern int dyna_state_read(uintptr_t ctx, void *read_fn, int flags, void *out);
extern void handler_state_refresh(void);
extern void family_once_init(void);
extern int hold_register_check_a(void *ctx_a, void *ctx_b, void *template_);
extern int hold_register_check_b(void *ctx_a, void *ctx_b);
extern int hold_register_verify(const void *record, void *out);
extern int event_routes_install(void *routes, void *vtable);
extern int provider_guard_init(int slot, void *out);
extern int guest_identity_check(void *state, uintptr_t engine, void *ptrs,
                                void *read_fn, int flags);
extern int holdfire_policy_resolve(void *policy);
extern void holdfire_fire_super(void);
extern void holdfire_fire_main(void *state, void *snapshot, void *params,
                               void *result);
extern void holdfire_sig_helper(void);
extern int holdfire_verify_fn(uint64_t ctx, void *out);
extern int holdfire_describe_fn(uint64_t ctx, uint32_t action, uint32_t slot,
                                void *out);
extern uintptr_t game_read(uintptr_t handle, uintptr_t addr, void *out,
                           uint32_t len);

extern char g_handler_lock;
extern uint64_t g_state_epoch;
extern int32_t g_state_status;
extern uint64_t g_state_aux;
extern uint64_t g_state_epoch2;
extern pthread_once_t g_family_once;
extern void *g_family_once_fn;
extern int32_t g_prop_state[144];

extern uint64_t g_pin_family_epoch;
extern uint64_t g_pin_family_aux;
extern void *g_pin_family_check;
extern uint64_t g_nexusplus_family_epoch;
extern uint64_t g_nexusplus_family_aux;
extern void *g_nexusplus_family_check;
extern uint64_t g_brawler_family_epoch;
extern uint64_t g_brawler_family_aux;
extern void *g_brawler_family_check;

extern uint64_t g_hold_gen_floor;
extern uint64_t g_hold_record[11];
extern uint64_t g_hold_counters[18];
extern uint64_t g_holdfire_gen2;
extern uint32_t g_hold_reset_flag;

extern void *g_evasion_lib_handle;
extern void *g_expected_lib_base;
extern char g_expected_lib_path[];
extern void *g_hold_register_fn;
extern void *g_hold_snapshot_fn;
extern void *g_hold_lease_fn;
extern void *g_hold_recheck_fn;
extern void *g_event_routes_fn;
extern char g_exports_ok;
extern char g_signed_loader_ok;
extern uint64_t g_hold_generation_rt;
extern uint64_t g_guest_identity_state[4];
extern uint64_t g_hold_routes_flag;
extern uint64_t g_hold_routes_state[21];
extern char g_hold_registered;
extern uint64_t g_hold_state_word;

extern long g_main_tid;
extern uint64_t g_hold_reentrancy;
extern uint64_t g_hold_fire_state[21];
extern uint64_t g_hold_fire_pub;
extern uint64_t g_hold_fire_epoch;
extern uint64_t g_hold_fire_ctx;
extern uint64_t g_hold_fire_ctx2;
extern uint32_t g_hold_fire_main_held;
extern uint32_t g_hold_fire_req2;
extern uint32_t g_hold_fire_pair_a;
extern uint32_t g_hold_fire_pair_b;
extern uint32_t g_hold_fire_own_gid;
extern uint64_t g_hold_fire_generation;
extern uint32_t g_hold_fire_requested;
extern uint32_t g_hold_fire_gate_a2;
extern uint32_t g_hold_fire_configured;
extern uint64_t g_hold_fire_gen3;
extern uint64_t g_hold_fire_dispatch_state;
extern uint64_t g_hold_fire_result;
extern uint32_t g_hold_fire_result2;
extern uint64_t g_hold_fire_last_log_ms;
extern int32_t g_hold_dedup_a;
extern uint32_t g_hold_dedup_b;

extern uintptr_t g_engine_base;

#define ROD_HOLD_MAGIC    (*(const uint64_t *)(uintptr_t)0x104810)
#define ROD_LEASE_TAG_IN  (*(const uint64_t *)(uintptr_t)0x1047a8)
#define ROD_LEASE_TAG_OUT (*(const uint64_t *)(uintptr_t)0x1047c0)
#define ROD_VERIFY_TAG    (*(const uint64_t *)(uintptr_t)0x1047b8)
#define ROD_CTX_A         ((const uint64_t *)(uintptr_t)0x129350)
#define ROD_CTX_B         ((const uint64_t *)(uintptr_t)0x129918)
#define ROD_TEMPLATE      ((const void *)(uintptr_t)0x1296c0)
#define ROD_VERIFY_VT     (*(const uint64_t *)(uintptr_t)0x10e780)
#define ROD_ROUTES_VT     (*(const uint64_t *)(uintptr_t)0x10e700)
#define ROD_POLICY_VT     (*(const uint64_t *)(uintptr_t)0x10e7a8)
#define ROD_FIRE_VT       (*(const uint64_t *)(uintptr_t)0x10e7d8)
#define ROD_FIRE_VT2      (*(const uint64_t *)(uintptr_t)0x10e548)
#define ROD_EVENT_BASE    (*(const uint64_t *)(uintptr_t)0x209d00)
#define ROD_FIRE_PARAMS   ((const void *)(uintptr_t)0x1cfb60)
#define ROD_IDENTITY_PTRS ((const void *)(uintptr_t)0x1c3220)
#define ROD_FN_TABLE      ((const void *const *)(uintptr_t)0x1c5880)

#define HOLD_ACTION 0x1du

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

static uint32_t brawler_family_gate(const char *key)
{
    int (*check)(void);
    int (*once_fn)(int);

    if (g_brawler_family_epoch == 0 || g_brawler_family_check == NULL)
        return 0;

    check = (int (*)(void))g_brawler_family_check;
    if (check() != 1 || g_state_epoch2 != g_brawler_family_epoch
        || g_state_aux != g_brawler_family_aux)
        return 0;

    if (brawler_mod_key_family(key) != 0)
        return 1;

    pthread_once(&g_family_once, family_once_init);
    if (g_family_once_fn == NULL)
        return 0;

    once_fn = (int (*)(int))g_family_once_fn;
    return (uint32_t)(once_fn(1) == 1);
}

uint64_t evasion_key_active(const char *key)
{
    const evasion_prop_entry_t *entry;
    int32_t port_idx;
    uint64_t bitmap;

    if (evasion_key_adapter_family(key) == 0) {
        if (key == NULL) {
            if (nexus_plus_key(NULL) != 0) {
                if (family_gate(g_nexusplus_family_epoch,
                                g_nexusplus_family_aux,
                                g_nexusplus_family_check) == 0)
                    return 0;
                goto once_gate;
            }
            goto spin_or_port;
        }

        if (strcmp(key, "pinEnabled") == 0 || strcmp(key, "sprayEnabled") == 0) {
            if (family_gate(g_pin_family_epoch, g_pin_family_aux,
                            g_pin_family_check) == 0)
                return 0;
            goto once_gate;
        }

        if (nexus_plus_key(key) != 0) {
            if (family_gate(g_nexusplus_family_epoch, g_nexusplus_family_aux,
                            g_nexusplus_family_check) == 0)
                return 0;
            goto once_gate;
        }

        if (strcmp(key, "speedExploitEnabled") == 0)
            return 1;
        if (strcmp(key, "speedLocalMoveEnabled") == 0)
            return 1;

        if (strcmp(key, "holdToShootEnabled") == 0
            || strcmp(key, "holdToShootAim") == 0
            || strcmp(key, "holdToShootRangeCheck") == 0)
            return (uint64_t)(holdfire_gate_b(NULL) != 0);

spin_or_port:
        if (strcmp(key, "isSpinEnabled") == 0)
            return (uint64_t)spin_active();

        if (port_id_lookup(key) >= 0)
            return (uint64_t)port_active(key, 0);

        port_idx = port_bit_index(key);
        bitmap = port_bitmap();
        if (port_idx >= 0) {
            if ((bitmap >> (port_idx & 0x1f) & 1) == 0)
                return 0;
            return (uint64_t)(port_bitmap_gate(bitmap) != 0);
        }

        entry = (const evasion_prop_entry_t *)prop_entry_lookup(key);
        if (entry == NULL)
            return 0;
        if (entry->kind != 0)
            return 0;
        return (uint64_t)(entry->f32 == 0);
    }

    if (brawler_family_gate(key) == 0)
        return 0;
    return 1;

once_gate:
    pthread_once(&g_family_once, family_once_init);
    if (g_family_once_fn == NULL)
        return 0;
    return (uint64_t)(((int (*)(int))g_family_once_fn)(1) == 1);
}

void nexus_evasion_hold_register_v1(const uint64_t *param_1)
{
    uint64_t rec[11];
    Dl_info info;
    Dl_info fi;
    uint64_t ctx_a[13];
    uint64_t ctx_b[21];
    uint64_t snap_epoch2;
    uint64_t snap_ctx2;
    uint8_t template_[0x228];
    uint64_t out[18];
    int (*verify_fn)(uint64_t, void *);

    if (param_1 == NULL) {
        handler_lock_acquire();
        if (g_hold_gen_floor < g_hold_record[2])
            g_hold_gen_floor = g_hold_record[2];
        g_holdfire_gen2 = g_holdfire_gen2 + 1;
        memset(g_hold_record, 0, sizeof g_hold_record);
        memset(g_hold_counters, 0, sizeof g_hold_counters);
        g_hold_reset_flag = 0;
        handler_lock_release();
        return;
    }

    memcpy(rec, param_1, sizeof rec);

    {
        uint64_t swapped[2];
        swapped[0] = ((rec[0] & 0xffffffffu) << 32) | (rec[0] >> 32);
        swapped[1] = ((rec[10] & 0xffffffffu) << 32) | (rec[10] >> 32);
        if (memcmp(swapped, (const void *)(uintptr_t)0x104810, 16) != 0)
            return;
    }

    if (rec[8] == 0 || rec[9] == 0 || rec[5] == 0 || rec[1] == 0
        || rec[2] == 0 || rec[3] == 0)
        return;

    if (dladdr((void *)(uintptr_t)rec[3], &info) == 0 || info.dli_fbase == NULL)
        return;

    if (dladdr((void *)(uintptr_t)rec[8], &fi) == 0 || fi.dli_fbase != info.dli_fbase)
        return;
    if (dladdr((void *)(uintptr_t)rec[9], &fi) == 0 || fi.dli_fbase != info.dli_fbase)
        return;
    if (dladdr((void *)(uintptr_t)rec[5], &fi) == 0 || fi.dli_fbase != info.dli_fbase)
        return;
    if (rec[6] != 0
        && (dladdr((void *)(uintptr_t)rec[6], &fi) == 0
            || fi.dli_fbase != info.dli_fbase))
        return;
    if (rec[7] != 0
        && (dladdr((void *)(uintptr_t)rec[7], &fi) == 0
            || fi.dli_fbase != info.dli_fbase))
        return;

    handler_lock_acquire();
    snap_ctx2 = ROD_CTX_A[13];
    snap_epoch2 = g_state_epoch2;
    memcpy(ctx_a, ROD_CTX_A, sizeof ctx_a);
    memcpy(ctx_b, ROD_CTX_B, sizeof ctx_b);
    handler_lock_release();

    if (rec[1] != g_state_epoch2 || rec[3] != g_state_aux
        || ctx_b[1] != g_state_epoch2 || ctx_b[3] != rec[3])
        return;

    handler_lock_acquire();
    memcpy(template_, ROD_TEMPLATE, sizeof template_);
    handler_lock_release();

    if (hold_register_check_a(ctx_a, ctx_b, template_) == 0)
        return;
    if (hold_register_check_b(ctx_a, ctx_b) == 0)
        return;

    handler_lock_acquire();

    if (snap_ctx2 != ROD_CTX_A[13] || snap_epoch2 != g_state_epoch2
        || !(g_hold_gen_floor < rec[2])
        || ctx_a[6] != ROD_CTX_A[6] || ctx_a[7] != ROD_CTX_A[7]
        || ctx_a[8] != ROD_CTX_A[8] || ctx_a[9] != ROD_CTX_A[9]
        || ctx_a[10] != ROD_CTX_A[10] || ctx_a[11] != ROD_CTX_A[11]
        || ctx_a[12] != ROD_CTX_A[12]) {
        handler_lock_release();
        return;
    }

    memset(out, 0, sizeof out);
    out[0] = ROD_VERIFY_TAG;

    if (memcmp(ctx_b, ROD_CTX_B, 0xa8) != 0)
        goto unlock_fail;

    verify_fn = (int (*)(uint64_t, void *))(uintptr_t)rec[8];
    if (verify_fn(rec[4], out) != 1)
        goto unlock_fail;

    if (hold_register_verify(rec, out) == 0)
        goto unlock_fail;

    if ((uint32_t)(out[5] >> 32) == 0 && g_hold_record[1] != 0) {
        if (memcmp(g_hold_record, rec, 0x58) == 0) {
            handler_lock_release();
            return;
        }
        goto unlock_fail;
    }

    if ((uint32_t)(out[5] >> 32) != 0)
        goto unlock_fail;

    if (g_hold_gen_floor < g_hold_record[2])
        g_hold_gen_floor = g_hold_record[2];

    memcpy(g_hold_record, rec, sizeof g_hold_record);
    g_holdfire_gen2 = g_holdfire_gen2 + 1;
    memcpy(g_hold_counters, out, sizeof g_hold_counters);
    g_hold_reset_flag = 0;

    handler_lock_release();
    return;

unlock_fail:
    handler_lock_release();
}

int holdfire_gate_b(const char *key)
{
    const evasion_prop_entry_t *aim_entry;
    const evasion_prop_entry_t *range_entry;
    const evasion_prop_entry_t *entry;
    uint32_t aim;
    uint32_t range;

    aim_entry = prop_find("holdToShootAim");
    range_entry = prop_find("holdToShootRangeCheck");

    aim = (uint32_t)g_prop_state[aim_entry->state_index];
    range = (uint32_t)g_prop_state[range_entry->state_index];

    if (key != NULL) {
        if (strcmp(key, "holdToShootAim") == 0)
            aim = 1;
        if (strcmp(key, "holdToShootRangeCheck") == 0)
            range = 1;
    }

    if (holdfire_gate_a() == 0)
        return 0;

    if ((range | aim) > 1)
        return 0;

    if ((g_hold_record[10] >> ((aim | range << 1) & 0x1f) & 1) == 0)
        return 0;

    if (aim != 0) {
        if (g_hold_record[7] == 0)
            return 0;

        entry = prop_find("aopPredictVersion");
        if (g_prop_state[entry->state_index] < 1)
            return 0;
        if (g_prop_state[entry->state_index] > 2)
            return 0;
    }

    entry = prop_find("aopAimEnabled");
    if (g_prop_state[entry->state_index] != 0) {
        if (g_hold_record[7] == 0)
            return 0;
        if (port_gate_check(0, port_gate_version(), 0) == 0)
            return 0;
    }

    return 1;
}

uint64_t nexus_evasion_hold_lease_v1(uint64_t param_1, uint64_t param_2,
                                     int32_t *param_3)
{
    if (param_3 == NULL || param_3[0] != 1 || param_3[1] != 0x60)
        return 0;

    handler_lock_acquire();
    hold_lease_impl((uint32_t)param_1, (uint32_t)param_2, (uint64_t *)(void *)param_3);
    handler_lock_release();
    return 0;
}

int hold_lease_impl(uint32_t action, uint32_t slot, uint64_t *out)
{
    uint32_t lease[10];
    const evasion_prop_entry_t *entry;

    handler_state_refresh();

    if (action != HOLD_ACTION || !(slot - 4u > 0xfffffffcu))
        return 0;
    if (g_state_status != 0)
        return 0;

    entry = prop_find("holdToShootEnabled");
    if (g_prop_state[entry->state_index] == 0)
        return 0;

    if (holdfire_gate_b(NULL) == 0)
        return 0;

    if (g_hold_record[5 + slot - 1] == 0)
        return 0;
    if (g_hold_record[9] == 0)
        return 0;

    memset(lease, 0, sizeof lease);
    *(uint64_t *)lease = ROD_LEASE_TAG_IN;

    if (holdfire_describe_fn(g_hold_record[4], HOLD_ACTION, slot, lease) != 1)
        return 0;

    if (lease[0] != 1 || lease[1] != 0x28)
        return 0;
    if ((lease[2] ^ 0xffffffffu) & 0x3fu)
        return 0;
    if (lease[9] != 0)
        return 0;
    if (lease[4] == 0 || lease[6] == 0 || lease[3] == 0)
        return 0;
    if (lease[8] <= 999999u || lease[8] >= 2000000u)
        return 0;

    out[0] = ROD_LEASE_TAG_OUT;
    *(uint32_t *)(out + 1) = HOLD_ACTION;
    *(uint32_t *)((char *)out + 0x0c) = slot;
    out[2] = g_state_epoch;
    out[3] = g_hold_record[1];
    out[4] = g_hold_record[2];
    out[5] = g_hold_counters[4];
    out[6] = g_holdfire_gen2;
    *(uint64_t *)(out + 7) = *(uint64_t *)lease;
    *(uint64_t *)(out + 8) = *(uint64_t *)(lease + 2);
    *(uint64_t *)(out + 9) = *(uint64_t *)(lease + 4);
    *(uint64_t *)(out + 10) = *(uint64_t *)(lease + 6);
    *(uint64_t *)(out + 11) = *(uint64_t *)(lease + 8);
    return 1;
}

void nexus_evasion_hold_recheck_v1(int32_t *param_1)
{
    uint64_t fresh[12];
    int ok;

    if (param_1 == NULL)
        return;
    if (param_1[0] != 1 || param_1[1] != 0x60)
        return;

    handler_lock_acquire();
    memset(fresh, 0, sizeof fresh);
    ok = hold_lease_impl((uint32_t)param_1[2], (uint32_t)param_1[3], fresh);
    if (ok != 0)
        ok = memcmp(param_1, fresh, 0x60) == 0;
    handler_lock_release();
}

void holdfire_bootstrap(int mode)
{
    Dl_info info;
    const char *stage;

    g_hold_register_fn = dlsym(g_evasion_lib_handle, "nexus_evasion_hold_register_v1");
    g_hold_snapshot_fn = dlsym(g_evasion_lib_handle, "nexus_evasion_hold_snapshot_v1");
    g_hold_lease_fn = dlsym(g_evasion_lib_handle, "nexus_evasion_hold_lease_v1");
    g_hold_recheck_fn = dlsym(g_evasion_lib_handle, "nexus_evasion_hold_recheck_v1");
    g_event_routes_fn = dlsym(g_evasion_lib_handle, "nexus_evasion_event_routes_v2");

    {
        void *fns[5] = { g_hold_register_fn, g_hold_snapshot_fn, g_hold_lease_fn,
                         g_hold_recheck_fn, g_event_routes_fn };

        for (int i = 0; i < 5; i++) {
            if (fns[i] == NULL)
                goto exports_fail;
            if (dladdr(fns[i], &info) == 0)
                goto exports_fail;
            if (info.dli_fbase != g_expected_lib_base)
                goto exports_fail;
            if (info.dli_fname == NULL)
                goto exports_fail;
            if (strcmp(info.dli_fname, g_expected_lib_path) != 0)
                goto exports_fail;
        }
    }

    g_exports_ok = 1;
    stage = "actual_signed_loader_identity";

    if (mode == 0 || g_signed_loader_ok == 0 || g_hold_generation_rt == 0)
        goto hold_unavailable;

    if (guest_identity_check(g_guest_identity_state, g_engine_base,
                             (void *)ROD_IDENTITY_PTRS, (void *)game_read, 0) == 0) {
        stage = "guest_game_identity";
        goto hold_unavailable;
    }

    memset(g_hold_routes_state, 0, sizeof g_hold_routes_state);
    g_hold_routes_state[0] = ROD_VERIFY_VT;

    {
        int routes_result = ((int (*)(void))g_event_routes_fn)();

        if (routes_result != 1)
            goto owned_event_proof;
        if (event_routes_install((void *)(uintptr_t)routes_result,
                                 g_hold_routes_state) == 0)
            goto owned_event_proof;
    }

    {
        uint64_t guard1 = 0;
        uint64_t guard2 = 0;

        g_hold_routes_flag = 1;
        if (provider_guard_init(1, &guard1) == 0
            || provider_guard_init(2, &guard2) == 0) {
            stage = "provider_all1420_native_guards";
            goto hold_unavailable;
        }
    }

    g_hold_state_word = 0x100000001ull;

    {
        uint64_t reg[11];

        reg[0] = ROD_ROUTES_VT;
        reg[1] = ROD_EVENT_BASE;
        reg[2] = g_hold_generation_rt;
        reg[3] = (uint64_t)(uintptr_t)holdfire_sig_helper;
        reg[4] = 0;
        reg[5] = (uint64_t)(uintptr_t)ROD_FN_TABLE[0];
        reg[6] = (uint64_t)(uintptr_t)ROD_FN_TABLE[1];
        reg[7] = (uint64_t)(uintptr_t)ROD_FN_TABLE[2];
        reg[8] = (uint64_t)(uintptr_t)holdfire_verify_fn;
        reg[9] = (uint64_t)(uintptr_t)holdfire_describe_fn;
        reg[10] = ROD_FIRE_VT2;

        if (((int (*)(const uint64_t *))g_hold_register_fn)(reg) == 1) {
            g_hold_registered = 1;
            log_event("hold_registered", "actual_held_default_free_handler",
                      ",\"action\":29,\"scope\":\"SIGNED_LOCAL_FREE\","
                      "\"supported_domains\":15,\"main\":true,\"super_typed\":true,"
                      "\"hold_aim\":true,\"new_hooks\":0,\"paid_state_changed\":false");
            return;
        }

        stage = "registered_default_handler";
        goto hold_unavailable;
    }

exports_fail:
    stage = "exports";
    goto hold_unavailable;

owned_event_proof:
    stage = "owned_event_proof";

hold_unavailable:
    g_hold_state_word = 0x200000000ull;
    log_event("hold_unavailable", stage,
              ",\"action\":29,\"other_features_unchanged\":true");
}

void holdfire_fire_tick(int mode)
{
    uint64_t policy[7];
    struct timespec ts;
    uint64_t now_ms;
    uint64_t now2_ms;
    int policy_ok;
    uint64_t snapshot[21];
    uint64_t state_out[2];

    if (g_hold_registered != 1 || (g_hold_reentrancy & 1) != 0 || g_main_tid == 0)
        return;
    if (gettid() != g_main_tid)
        return;

    memset(g_hold_fire_state, 0, sizeof g_hold_fire_state);
    g_hold_fire_state[0] = ROD_FIRE_VT;

    if (((int (*)(void))g_hold_snapshot_fn)() != 1)
        return;

    memset(policy, 0, sizeof policy);
    policy[0] = ROD_POLICY_VT;
    policy_ok = holdfire_policy_resolve(policy);

    if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0)
        now_ms = (uint64_t)ts.tv_sec * 1000 + (uint64_t)ts.tv_nsec / 1000000;
    else
        now_ms = 0;

    memset(snapshot, 0, sizeof snapshot);
    snapshot[0] = g_hold_fire_pub;
    snapshot[1] = g_hold_fire_epoch;
    snapshot[2] = g_hold_fire_ctx;
    snapshot[3] = g_hold_fire_ctx2;
    *(uint32_t *)(snapshot + 4) = (uint32_t)g_main_tid;
    *(uint32_t *)((char *)snapshot + 0x24) = (uint32_t)g_main_tid;
    snapshot[5] = now_ms;
    snapshot[6] = (uint64_t)(uint32_t)(policy_ok != 0
                                       && (uint32_t)(policy[1] >> 32) != 0
                                       && (int32_t)policy[6] == 0);
    snapshot[7] = (uint64_t)(uint32_t)((g_hold_fire_requested != 0
                                        && g_hold_fire_gate_a2 != 0)
                                       && g_hold_fire_configured != 0);
    *(uint32_t *)(snapshot + 8) = g_hold_fire_main_held;
    snapshot[9] = ((uint64_t)g_hold_fire_pair_b << 32) | g_hold_fire_pair_a;
    snapshot[10] = 0;
    snapshot[11] = g_hold_fire_req2;
    snapshot[12] = (uint64_t)(uint32_t)dyna_jump_armed();
    snapshot[13] = (uint64_t)(uint32_t)dyna_jump_armed();
    snapshot[14] = (uint64_t)(uint32_t)(mode == 2);

    g_hold_fire_gen3 = g_hold_fire_gen3 + 1;
    g_hold_reentrancy = 1;

    if (mode == 2) {
        holdfire_fire_super();
    } else {
        holdfire_fire_main(&g_hold_fire_dispatch_state, snapshot,
                           (void *)ROD_FIRE_PARAMS, &g_hold_fire_result);
    }
    g_hold_reentrancy = 0;

    if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0)
        now2_ms = (uint64_t)ts.tv_sec * 1000 + (uint64_t)ts.tv_nsec / 1000000;
    else
        now2_ms = 0;

    if (g_hold_fire_gen3 < 0x11 || g_hold_fire_result2 != 0
        || (policy_ok != 0 && (int32_t)policy[6] != g_hold_dedup_a)
        || g_hold_fire_result != g_hold_dedup_b
        || g_hold_fire_last_log_ms == 0
        || now2_ms - g_hold_fire_last_log_ms > 999) {

        if (g_hold_fire_main_held == 1)
            dyna_state_read(g_hold_fire_ctx, (void *)game_read, 0, state_out);

        {
            char json[0x640];
            snprintf(json, sizeof json,
                     ",\"action\":29,\"scope\":\"SIGNED_LOCAL_FREE\","
                     "\"publication\":%llu,\"epoch\":%llu,\"own_gid\":%d,"
                     "\"generation\":%llu,\"requested\":%d,\"configured\":%u,"
                     "\"active_known\":%u,\"active\":%u,\"ended\":%u,"
                     "\"held_known\":%d,\"main_held\":%u,\"super_held\":%u,"
                     "\"delay_ms\":%d,\"status\":%d,\"route\":%d,\"called\":%d,"
                     "\"return_known\":%d,\"native_rc\":%d,\"raw_x\":%d,\"raw_y\":%d,"
                     "\"main_calls\":%llu,\"super_calls\":%llu,"
                     "\"manual_fallback_calls\":%llu,\"calls_while_ended\":%llu,"
                     "\"paid_state_changed\":false",
                     (unsigned long long)snapshot[1],
                     (unsigned long long)snapshot[0],
                     (int)g_hold_fire_own_gid,
                     (unsigned long long)g_hold_fire_generation,
                     (int)g_hold_fire_requested,
                     (unsigned)g_hold_fire_configured,
                     (unsigned)snapshot[7],
                     (unsigned)snapshot[6],
                     (unsigned)snapshot[14],
                     (int)snapshot[12],
                     (unsigned)snapshot[8],
                     (unsigned)snapshot[13],
                     (int)snapshot[11],
                     (int)snapshot[10],
                     (int)snapshot[9],
                     (int)g_hold_fire_gen3,
                     (int)g_hold_fire_state[6],
                     (int)g_hold_fire_result2,
                     (int)snapshot[5],
                     (int)snapshot[4],
                     (unsigned long long)g_hold_fire_state[9],
                     (unsigned long long)g_hold_fire_state[10],
                     (unsigned long long)g_hold_fire_state[11],
                     (unsigned long long)g_hold_fire_state[12]);
            log_event("hold_fire",
                      g_hold_fire_result2 != 0 ? "original_once_called"
                                               : "bounded_held_policy",
                      json);
        }

        g_hold_dedup_b = g_hold_fire_result;
        g_hold_fire_last_log_ms = now2_ms;
        if (policy_ok != 0)
            g_hold_dedup_a = (int32_t)policy[6];
    }
}
