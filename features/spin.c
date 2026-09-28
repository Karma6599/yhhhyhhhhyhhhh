#define _GNU_SOURCE

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <dlfcn.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <pthread.h>

#define SNAPSHOT_VERSION 1
#define STATUS_OUT_SIZE  0x48
#define SPIN_SNAPSHOT_SIZE 0x58
#define QUERY_SLOT_COUNT 0x76

#define SPIN_ACTION 0x19

#define SPIN_FRAME_ENTRY_RVA 0xb30690u
#define SPIN_FRAME_BL_RVA    0xb30694u
#define SPIN_MOVE_ENTRY_RVA  0xe7b768u
#define SPIN_MOVE_BL_RVA     0xe7b76cu
#define SPIN_MOVE_FN_RVA     0xe7af70u

#define ISLAND_SIZE 0x158

#define ROD_STATUS_TAG   (*(const uint64_t *)(uintptr_t)0x1047a0)
#define ROD_SPIN_HEADER  (*(const uint64_t *)(uintptr_t)0x1047b0)
#define ROD_LEASE_TAG_IN (*(const uint64_t *)(uintptr_t)0x1047a8)
#define ROD_LEASE_TAG_OUT (*(const uint64_t *)(uintptr_t)0x1047c0)
#define ROD_NG_BIND_TAG  (*(const uint64_t *)(uintptr_t)0x10e7f8)
#define ROD_FRAME_HEADER (*(const uint64_t *)(uintptr_t)0x10e7a8)
#define ROD_SPIN_STAMP   (*(const uint64_t *)(uintptr_t)0x10e540)
#define ROD_SPIN_BL_TMPL (*(const uint64_t *)(uintptr_t)0x10e5b8)
#define ROD_MOVE_BL_TMPL (*(const uint64_t *)(uintptr_t)0x10e6b8)
#define ROD_ONLINE_TMPL  ((const void *)(uintptr_t)0x1c3270)
#define ROD_SPIN_CB_TMPL (*(const uint64_t *)(uintptr_t)0x1cb6f0)
#define ROD_ONLINE_STAMP (*(const uint64_t *)(uintptr_t)0x10e670)
#define ROD_ONLINE_RESET (*(const uint64_t *)(uintptr_t)0x10e6c0)
#define ROD_IDENTITY_A   (*(const uint64_t *)(uintptr_t)0x112a40)
#define ROD_IDENTITY_B   (*(const uint64_t *)(uintptr_t)0x112a90)
#define ROD_IDENTITY_C   (*(const uint64_t *)(uintptr_t)0x112c30)

#define ROD_LOADER_FIELD_D   ((const char *)(uintptr_t)0x117e9e)
#define ROD_LOADER_SIG_D     "Lnexus/loader/d;"
#define ROD_LOADER_FIELD_E   ((const char *)(uintptr_t)0x11fd4c)
#define ROD_LOADER_SIG_E     "Lnexus/loader/e;"
#define ROD_SIG_STRING_A     ((const char *)(uintptr_t)0x11d6aa)
#define ROD_SIG_STRING_B     ((const char *)(uintptr_t)0x1147f8)
#define ROD_CB_FIELD_NAME    ((const char *)(uintptr_t)0x119e58)
#define ROD_CB_FIELD_SIG     ((const char *)(uintptr_t)0x11896b)
#define ROD_CB_BOOL_NAME     ((const char *)(uintptr_t)0x120926)
#define ROD_MODULES_NAME     ((const char *)(uintptr_t)0x120939)
#define ROD_MAP_NAME_A       ((const char *)(uintptr_t)0x11df95)
#define ROD_SIG_MAP          "Ljava/util/Map;"
#define ROD_SIG_LIST         "Ljava/util/List;"
#define ROD_MODULE_NAME_FIELD ((const char *)(uintptr_t)0x1141aa)

static const char GAME_SHA[] =
    "a10aeb6b4085fb2a15d969a130cd9608231998b5269a40aed41129d99624ede3";
static const char PROFILE_SHA[] =
    "114ba105835bd4157cfd80aad9498734d51108ee193601410abeb5e0bab3a71c";

extern char g_handler_lock;
extern uint64_t g_state_epoch;
extern int32_t g_state_status;
extern uint64_t g_state_epoch2;
extern uint64_t g_state_aux;
extern int32_t g_prop_state[144];

extern uint64_t g_spin_family_epoch;
extern uint64_t g_spin_family_aux;
extern void *g_spin_lease_ctx;
extern int (*g_spin_lease_fn)(void *, int, void *);
extern uint64_t g_spin_aux;
extern uint64_t g_spin_aux2;
extern uint64_t g_aim_holdfire_gate;
extern uint32_t g_aim_pair_bitmap;
extern uint64_t g_holdfire_epoch;
extern uint64_t g_brawler_family_epoch;
extern uint64_t g_brawler_family_aux;
extern void *g_brawler_family_check;
extern uint64_t g_port_gate[10];
extern pthread_once_t g_port_once;
extern void *g_port_once_fn;
extern uint64_t g_port_aux_epoch;
extern uint64_t g_port_aux;
extern uint64_t g_port_aux2;

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

extern void handler_state_refresh(void);
extern void family_once_init(void);
extern int spin_active(void);
extern int evasion_key_force_flag(const char *key);
extern int feature_port_state_check(const char *key);
extern int spin_mode_gate(int32_t mode);
extern int port_gate_check(int port_id, uint32_t version, int flags);
void spin_post_callback(uint64_t frame);
void loader_identity_verify(void);
extern int brawler_key_active(const char *key);
extern int32_t port_id_lookup(const char *key);
extern uint32_t port_gate_version(void);
extern int holdfire_gate_a(void);
extern int nexus_evasion_restore_v1(uint64_t *fns, int32_t *values);

extern void runtime_journal_write(const char *stage, const char *reason,
                                  const char *payload);
extern uintptr_t g_engine_base;
extern uint64_t g_engine_epoch;
extern uint64_t g_page_size;
extern int32_t g_files_dir_fd;
extern int32_t g_journal_fd;
extern int32_t g_init_thread_id;
extern uint32_t g_game_callback_acked;
extern uint64_t g_frame_parse_enabled;
extern uint64_t g_frame_record_table[];
extern void *g_evasion_lib_handle;
extern void *g_expected_lib_base;
extern char g_expected_lib_path[];
extern uintptr_t game_read(uintptr_t handle, uintptr_t addr, void *out,
                          uint32_t len);
extern int game_image_sha_verify(void *out, size_t len, const void *template_);
extern int game_thread_present(void);
extern int guest_identity_check(void *state, uintptr_t engine, void *ptrs,
                               void *read_fn, int flags);
extern uint64_t provider_capacity(void);
extern int spin_provider_install(void *table, uint64_t capacity,
                                 const void *record);
extern int online_move_install(void);
extern int online_move_verify(void *state, void *out);
extern void *game_island_alloc(void);
extern int frame_island_code_build(void *out);
extern int movement_island_code_build(void *out);
extern void code_cache_flush(void *begin, void *end);
extern int spin_post_publish(uint64_t entry, uint32_t bl_word, uint64_t bl_full);
extern int spin_frame_post_verify(int flags);
extern int spin_move_post_verify(int flags);
extern int spin_post_exec(void *record, void *header);
extern void spin_post_field_apply(uint64_t token);
extern void spin_provider_publish(void *table, void *record, void *out);
extern uint64_t mono_now_us(void);
extern uint64_t mono_now_ms_fn(int clock);
extern int frame_entity_resolve(uintptr_t engine, uint64_t obj, void *read_fn,
                                int flags, void *out);
extern void spin_online_preapply(void);
extern int spin_online_entity_valid(void *state);
extern int jni_get_object_field(void *env, void *obj, const char *name,
                                const char *sig);
extern int jni_string_equals(void *env, void *str, const char *expected);
extern int jni_get_boolean_field(void *env, void *obj, const char *name);
extern int jni_string_read(void *env, void *str, char *buf, size_t len);
extern int jni_get_int_field(void *env, void *obj, const char *name,
                             int64_t *out);
extern void *jni_map_value(void *env, void *map);
extern int sha256_prefix_equal(void *a, void *b);
extern int file_accessible(const char *path);
extern int holdfire_post_handler(void);
extern void spin_move_post_cb(uint64_t frame);
extern void spin_provider_cb_a(void);
extern void spin_provider_cb_b(void);
extern void spin_provider_cb_c(void);
extern void spin_provider_cb_d(void);
extern void spin_register_cb_a(void);
extern void spin_register_cb_b(void);

extern void *g_spin_register_fn;
extern char g_spin_move_active;
extern int frame_record_parse(void *table, const void *frame, void *record_out);
extern uint32_t g_spin_pub_reason;
extern uint32_t g_spin_pub_writes;
extern uint32_t g_spin_pub_field_apply;
extern int32_t g_spin_pub_angle;
extern uint32_t g_spin_pub_readback;
extern void (*g_spin_snapshot_fn_rt)(void);
extern void (*g_spin_lease_fn_rt)(void);
extern void (*g_spin_recheck_fn_rt)(void);
extern int (*g_spin_publish_post_fn)(const void *);
extern int (*g_spin_validate_post_fn)(const void *);
extern int (*g_spin_publish_movement_fn)(const void *);
extern int (*g_spin_validate_movement_fn)(const void *);
extern char g_spin_armed;
extern uint64_t g_spin_identity_state[4];
extern uint64_t g_spin_provider_table[0x1000 / 8];
extern uint64_t g_spin_provider_active;
extern char g_online_move_bound;
extern uint64_t g_online_move_engine;
extern uint64_t g_online_move_tmpl[6];
extern uint64_t g_online_move_state[8];
extern char g_spin_post_ready;
extern uint64_t g_spin_frame_entry;
extern void *g_spin_frame_island;
extern void (*g_spin_frame_handler)(void);
extern void (*g_spin_frame_cb)(uint64_t);
extern uint64_t g_spin_frame_bl_slot;
extern uint64_t g_spin_frame_epoch;
extern uint64_t g_spin_frame_engine;
extern uint64_t g_spin_frame_stamp;
extern uint64_t g_spin_frame_bl;
extern uint64_t g_spin_frame_bl_hi;
extern uint64_t g_spin_move_entry;
extern void *g_spin_move_island;
extern void (*g_spin_move_handler)(void);
extern void (*g_spin_move_cb)(uint64_t);
extern uint64_t g_spin_move_bl_slot;
extern uint64_t g_spin_move_epoch;
extern uint64_t g_spin_move_engine;
extern uint64_t g_spin_move_stamp;
extern uint64_t g_spin_move_bl;
extern uint64_t g_spin_move_bl_hi;
extern char g_loader_verify_gate_a;
extern char g_loader_verify_gate_b;
extern char g_spin_loader_verified;
extern uint64_t g_spin_loader_generation;
extern uint64_t g_spin_profile_sha[8];
extern uint64_t g_spin_telemetry[53];
extern char g_spin_state_flag;
extern char g_spin_stand_latch;
extern uint64_t g_spin_move_yields;
extern uint64_t g_spin_move_count;
extern uint32_t g_spin_move_saved;
extern uint64_t g_spin_move_entity;
extern uint32_t g_spin_move_apply_state;
extern uint32_t g_spin_move_apply_active;
extern uint64_t g_spin_move_own_static;
extern uint64_t g_spin_move_last_tick;
extern uint64_t g_spin_move_last_apply_tick;
extern float g_spin_move_angle;
extern uint64_t g_spin_online_state[40];
extern uint64_t g_spin_pub_count;
extern uint64_t g_spin_pub_tag;
extern uint64_t g_spin_pub_aux;
extern uint64_t g_spin_move_cfg_radius;
extern uint32_t g_spin_move_cfg_rate;
extern uint32_t g_spin_move_cfg_mode;
extern uint32_t g_spin_move_cfg_online;
extern int32_t g_spin_move_cfg_radius2;
extern int32_t g_spin_move_cfg_stand;
extern uint64_t g_spin_move_min_mono;
extern uint64_t g_spin_frame_identity_a;
extern uint64_t g_spin_frame_identity_b;
extern uint64_t g_spin_frame_identity_c;
extern uint64_t g_spin_move_gate;
extern uint64_t g_spin_move_gate_hi;
extern uint64_t g_spin_journal_tag;
extern uint64_t g_spin_move_last_journal;
extern int32_t (*g_get_requested_thunk)(const char *);
extern int32_t (*g_get_effective_thunk)(const char *);
extern uint32_t (*g_get_port_state_thunk)(const char *);

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

static int32_t prop_value(const char *name)
{
    const evasion_prop_entry_t *entry = prop_find(name);

    if (entry == NULL)
        return 0;
    return g_prop_state[entry->state_index];
}

const void *prop_entry_lookup(const char *key)
{
    size_t len;

    if (key == NULL)
        return NULL;
    for (len = 0; key[len] != '\0'; len++) {
        if (len == 0x60)
            return NULL;
    }
    return prop_find(key);
}

uint32_t nexus_evasion_get_port_state(const char *name)
{
    size_t len;
    int i;

    if (name == NULL)
        return 0xffffffffu;

    for (len = 0; name[len] != '\0'; len++) {
        if (len == 0x60)
            return 0xffffffffu;
    }

    for (i = 0; i < 142; i++) {
        if (strcmp(name, g_prop_table[i].name) != 0)
            continue;
        if (evasion_key_force_flag(name) != 0)
            return 1;
        if (g_prop_table[i].f32 == 0) {
            int ok;

            if (i >= 69)
                return 1;
            handler_lock_acquire();
            handler_state_refresh();
            ok = feature_port_state_check(name);
            handler_lock_release();
            return (uint32_t)(ok != 0) << 1;
        }
        return 3;
    }
    return 0xffffffffu;
}

void spin_status_fill(int32_t *out)
{
    int active;
    int32_t requested;
    int32_t mode;
    uint32_t effective;
    int code;

    if (out == NULL || out[0] != SNAPSHOT_VERSION || out[1] != STATUS_OUT_SIZE)
        return;

    handler_lock_acquire();
    handler_state_refresh();

    active = spin_active();
    requested = g_prop_state[0];
    mode = spin_mode_gate(prop_value("spinMovementMode"));

    effective = 0;
    if (active != 0)
        effective = (uint32_t)(mode != 0);
    code = 3;
    if (active != 0)
        code = 1;
    if (g_spin_family_epoch == 0)
        code = 2;
    if (requested != 0 && active != 0 && g_spin_family_epoch != 0)
        code = mode != 0 ? 5 : 4;

    *(uint64_t *)(void *)out = ROD_STATUS_TAG;
    out[2] = requested;
    out[3] = (int32_t)(g_spin_family_epoch != 0);
    out[4] = active;
    out[5] = (int32_t)(mode != 0 ? effective : 0);
    out[6] = 0;
    out[7] = code;
    out[8] = 0;
    out[9] = 0;
    *(uint64_t *)(void *)(out + 10) = g_state_epoch;
    *(uint64_t *)(void *)(out + 12) = g_spin_aux2;
    *(uint64_t *)(void *)(out + 14) = g_spin_family_epoch;
    *(uint64_t *)(void *)(out + 16) = g_spin_aux;

    handler_lock_release();
}

int port_status_fill(const char *key, int32_t *out)
{
    uint32_t port_id;
    uint32_t gate_flags;
    uint32_t version;
    int gate;
    uint32_t port_bit;
    uint32_t version_bit;
    uint32_t flags_bit;
    int32_t requested;
    uint32_t effective;
    int code;

    if (out == NULL || out[0] != SNAPSHOT_VERSION || out[1] != STATUS_OUT_SIZE)
        return 0;

    port_id = (uint32_t)port_id_lookup(key);
    if ((int32_t)port_id < 0)
        return 0;

    handler_lock_acquire();
    handler_state_refresh();

    gate_flags = 0;
    for (int i = 0; i < 10; i++)
        gate_flags |= (uint32_t)(g_port_gate[i] != 0) << i;
    version = port_gate_version();
    gate = port_gate_check((int)port_id, version, 0);
    requested = prop_value(key);

    port_bit = 1u << (port_id & 0x1f);
    version_bit = (uint32_t)(version & port_bit) != 0;
    flags_bit = (uint32_t)(gate_flags & port_bit) != 0;

    effective = 0;
    if (gate != 0)
        effective = (uint32_t)(requested != 0);
    code = 4;
    if (gate != 0)
        code = 5;
    if (requested == 0)
        code = 1;
    if (!version_bit)
        code = 3;
    if (!flags_bit)
        code = 2;

    *(uint64_t *)(void *)out = ROD_STATUS_TAG;
    out[2] = requested;
    out[3] = (int32_t)flags_bit;
    out[4] = (int32_t)version_bit;
    out[5] = (int32_t)effective;
    out[6] = 0;
    out[7] = code;
    out[8] = 0;
    out[9] = (int32_t)gate_flags;
    *(uint64_t *)(void *)(out + 10) = g_state_epoch;
    *(uint64_t *)(void *)(out + 12) = g_port_aux_epoch;
    *(uint64_t *)(void *)(out + 14) = g_port_aux;
    *(uint64_t *)(void *)(out + 16) = g_port_aux2;

    handler_lock_release();
    return 1;
}

int nexus_evasion_spin_snapshot_v1(int32_t *out)
{
    int active;
    int32_t status;

    if (out == NULL)
        return 0;
    if (out[0] != SNAPSHOT_VERSION || out[1] != SPIN_SNAPSHOT_SIZE)
        return 0;

    handler_lock_acquire();
    handler_state_refresh();

    active = spin_active();
    status = g_state_status;

    *(uint64_t *)(void *)(out + 2) = g_state_epoch;
    *(uint64_t *)(void *)(out + 4) = g_spin_family_epoch;
    *(uint64_t *)(void *)(out + 6) = g_spin_family_aux;
    *(uint64_t *)(void *)(out + 8) = g_spin_aux;
    *(uint64_t *)(void *)(out + 10) = g_spin_aux2;
    out[12] = (int32_t)(g_spin_family_epoch != 0);
    out[13] = active;
    out[14] = g_prop_state[0];
    out[15] = prop_value("spinSpeed");
    out[16] = 0x168;
    out[17] = status;
    out[18] = prop_value("spinMovementMode");
    out[19] = prop_value("spinRadius");
    out[20] = prop_value("spinOnlineOnlyIdle");
    *(uint64_t *)(void *)out = ROD_SPIN_HEADER;

    handler_lock_release();
    return 1;
}

typedef struct {
    uint64_t header;
    uint32_t flags;
    uint32_t token_hi;
    uint64_t lease_ctx;
    uint64_t lease_aux;
    uint32_t token;
    uint32_t zero;
} spin_lease_record_t;

_Static_assert(sizeof(spin_lease_record_t) == 0x28, "lease record size");

void spin_action_lease_submit(int action, uint32_t mode, uint64_t *out)
{
    int32_t mode_value;
    int mode_ok;

    handler_state_refresh();

    if (mode == 1 && mode_value == 0) {
        mode_ok = 1;
    } else if (mode_value == 1) {
        mode_ok = (mode & 0xfffffffeu) == 2;
    } else {
        return;
    }

    if (action != SPIN_ACTION || !mode_ok)
        return;

    if (spin_mode_gate(mode_value) == 0)
        return;
    if (!(g_state_status == 0 && g_prop_state[0] != 0))
        return;
    if (spin_active() == 0)
        return;
    if (g_spin_lease_fn == NULL)
        return;

    {
        spin_lease_record_t rec;
        uint32_t need_mask;
        int ok;

        memset(&rec, 0, sizeof rec);
        rec.header = ROD_LEASE_TAG_IN;
        ok = g_spin_lease_fn(g_spin_lease_ctx, SPIN_ACTION, &rec);
        if (ok != 1)
            return;
        if (rec.header != ROD_LEASE_TAG_IN)
            return;
        if ((uint32_t)rec.header != 1 || (uint32_t)(rec.header >> 32) != 0x28)
            return;

        need_mask = mode_value != 1 ? 0x3fu : 0xffu;
        if ((need_mask & ~rec.flags) != 0)
            return;
        if (rec.zero != 0)
            return;
        if (rec.lease_ctx == 0 || rec.lease_aux == 0 || rec.token_hi == 0)
            return;
        if (!(rec.token > 999999u && rec.token < 2000000u))
            return;

        out[0] = ROD_LEASE_TAG_OUT;
        *(uint32_t *)(void *)(out + 1) = SPIN_ACTION;
        *(uint32_t *)(void *)((char *)(out + 1) + 4) = mode;
        out[2] = g_state_epoch;
        out[3] = g_spin_family_epoch;
        out[4] = g_spin_family_aux;
        out[5] = g_spin_aux;
        out[6] = g_spin_aux2;
        out[7] = rec.header;
        out[8] = rec.flags | (uint64_t)rec.token_hi << 32;
        out[9] = rec.lease_ctx;
        out[10] = rec.lease_aux;
        out[11] = (uint64_t)rec.token | (uint64_t)rec.zero << 32;
    }
}

static int32_t key_value_get(const char *name)
{
    return prop_value(name);
}

int port_gate_check(int port_id, uint32_t version, int flags)
{
    uint32_t port_bit;
    int32_t v;

    if (port_id > 9)
        return 0;
    port_bit = 1u << (port_id & 0x1f);
    if ((version & port_bit) == 0)
        return 0;

    if (port_id < 2) {
        if ((version & 1) == 0)
            return 0;
        if (prop_value("holdToShootEnabled") != 0) {
            if (g_aim_holdfire_gate == 0)
                return 0;
            if (holdfire_gate_a() == 0)
                return 0;
            v = prop_value("holdToShootAim") | prop_value("holdToShootRangeCheck");
            if (v > 1)
                return 0;
            if ((g_aim_pair_bitmap >> (v & 0x1f) & 1) == 0)
                return 0;
        }
        if ((flags != 0 && port_id == 1) || prop_value("aopPredictEnabled") != 0) {
            if (prop_value("killauraEnabled") == 0) {
                if ((version >> 1 & 1) == 0)
                    return 0;
            } else {
                if ((version >> 1 & 1) == 0)
                    return 0;
                v = key_value_get("aopPredictVersion");
                if ((uint32_t)(v - 3) > 1)
                    return 0;
            }
            v = key_value_get("aopPredictVersion");
            if ((uint32_t)(v - 3) > 1)
                return 0;
        }
        if (prop_value("aopAimTargetMode") == 1)
            return 1;
        return key_value_get("aopAimTargetMode") == 2;
    }

    switch (port_id) {
    case 4:
    case 9:
        break;
    case 5:
    case 6:
    case 7:
    case 8: {
        const char *key;

        if (g_brawler_family_epoch == 0)
            return 0;
        if (g_brawler_family_check == NULL)
            return 0;
        if (((int (*)(void))g_brawler_family_check)() != 1)
            return 0;
        if (g_state_epoch2 != g_brawler_family_epoch)
            return 0;
        if (g_state_aux != g_brawler_family_aux)
            return 0;
        key = port_id == 5 ? "coltModEnabled"
             : port_id == 6 ? "autofarmEnabled"
             : port_id == 7 ? "boltModEnabled"
             : "kitNaniModEnabled";
        if (brawler_key_active(key) != 0)
            return 1;
        break;
    }
    default:
        if ((version >> 2 & 1) == 0)
            return 0;
        v = key_value_get("dodgeVersion");
        if ((uint32_t)(v - 1) > 4)
            return 0;
        if (key_value_get("isSpinEnabled") == 0)
            return 1;
        if (key_value_get("spinMovementMode") == 0)
            return 1;
        return key_value_get("spinOnlineOnlyIdle") != 0;
    }

    pthread_once(&g_port_once, family_once_init);
    if (g_port_once_fn == NULL)
        return 0;
    return ((int (*)(int))g_port_once_fn)(0) == 1;
}


int evasion_query_restore(void *self, const int32_t *record, int32_t size)
{
    uint64_t fns[QUERY_SLOT_COUNT];
    int32_t values[QUERY_SLOT_COUNT];
    const int32_t *slot;
    const int32_t *table;
    int count;
    int result;
    int i;

    (void)self;

    if (record == NULL || size != QUERY_SLOT_COUNT)
        return 4;

    count = 0;
    slot = record;
    table = (const int32_t *)(uintptr_t)0x124248;
    for (i = 0; i < QUERY_SLOT_COUNT; i++) {
        const int32_t *entry = table + i * 6;
        int64_t fn = *(const int64_t *)(const void *)(entry - 4);

        if (slot[0] != i || slot[1] < entry[-1] || slot[1] > entry[0])
            return 4;
        if (fn != 0) {
            fns[count] = (uint64_t)fn;
            values[count] = slot[1];
            count++;
        }
        slot += 2;
    }

    result = (int)nexus_evasion_restore_v1(fns, values);
    if (result == 1)
        return 1;
    if (result == 2)
        return 2;
    return 4;
}

int32_t nexus_evasion_get_requested_thunk(const char *name)
{
    return g_get_requested_thunk(name);
}

int32_t nexus_evasion_get_effective_thunk(const char *name)
{
    return g_get_effective_thunk(name);
}

uint32_t nexus_evasion_get_port_state_thunk(const char *name)
{
    return g_get_port_state_thunk(name);
}

static int sym_verified(void *sym)
{
    Dl_info info;

    if (sym == NULL)
        return 0;
    if (dladdr(sym, &info) == 0)
        return 0;
    if (info.dli_fbase != g_expected_lib_base)
        return 0;
    if (info.dli_fname == NULL)
        return 0;
    return strcmp(info.dli_fname, g_expected_lib_path) == 0;
}

typedef struct {
    uint64_t header;
    uintptr_t engine;
    void *reserved;
    void *read_fn;
    void *cb_a;
    void *cb_b;
    void *cb_c;
    void *cb_d;
} spin_provider_record_t;

void spin_exports_bind(void)
{
    const char *stage;
    const char *event;
    const char *payload;
    uint64_t island;
    uint64_t island2;
    int result;

    g_spin_register_fn = dlsym(g_evasion_lib_handle,
                               "nexus_evasion_spin_register_v1");
    g_spin_snapshot_fn_rt = (void (*)(void))dlsym(
        g_evasion_lib_handle, "nexus_evasion_spin_snapshot_v1");
    g_spin_lease_fn_rt = (void (*)(void))dlsym(
        g_evasion_lib_handle, "nexus_evasion_spin_lease_v1");
    g_spin_recheck_fn_rt = (void (*)(void))dlsym(
        g_evasion_lib_handle, "nexus_evasion_spin_recheck_v1");
    g_spin_publish_post_fn = (int (*)(const void *))dlsym(
        g_evasion_lib_handle, "nexus_evasion_publish_spin_post_v1");
    g_spin_validate_post_fn = (int (*)(const void *))dlsym(
        g_evasion_lib_handle, "nexus_evasion_spin_validate_post_v1");
    g_spin_publish_movement_fn = (int (*)(const void *))dlsym(
        g_evasion_lib_handle, "nexus_evasion_publish_spin_movement_post_v1");
    g_spin_validate_movement_fn = (int (*)(const void *))dlsym(
        g_evasion_lib_handle, "nexus_evasion_spin_validate_movement_post_v1");

    if (!sym_verified(g_spin_register_fn)
        || !sym_verified((void *)(uintptr_t)g_spin_snapshot_fn_rt)
        || !sym_verified((void *)(uintptr_t)g_spin_lease_fn_rt)
        || !sym_verified((void *)(uintptr_t)g_spin_recheck_fn_rt)
        || !sym_verified((void *)(uintptr_t)g_spin_publish_post_fn)
        || !sym_verified((void *)(uintptr_t)g_spin_validate_post_fn)
        || !sym_verified((void *)(uintptr_t)g_spin_publish_movement_fn)
        || !sym_verified((void *)(uintptr_t)g_spin_validate_movement_fn)) {
        stage = "spin_unavailable";
        event = "exports";
        payload = ",\"action\":25,\"other_features_unchanged\":true";
        goto out;
    }

    g_spin_armed = 1;

    if (guest_identity_check(g_spin_identity_state, g_engine_base,
                             (void *)(uintptr_t)0x1c3220,
                             (void *)(uintptr_t)game_read, 0) == 0) {
        stage = "spin_unavailable";
        event = "guest_game_identity";
        payload = ",\"action\":25,\"other_features_unchanged\":true";
        goto out;
    }

    if (provider_capacity() - 0x1001 >= 0xfffffffffffff000ull) {
        stage = "spin_unavailable";
        event = "provider_size";
        payload = ",\"action\":25,\"other_features_unchanged\":true";
        goto out;
    }

    {
        spin_provider_record_t rec;

        rec.header = ROD_NG_BIND_TAG;
        rec.engine = g_engine_base;
        rec.reserved = NULL;
        rec.read_fn = (void *)(uintptr_t)game_read;
        rec.cb_a = (void *)(uintptr_t)spin_provider_cb_a;
        rec.cb_b = (void *)(uintptr_t)spin_provider_cb_b;
        rec.cb_c = (void *)(uintptr_t)spin_provider_cb_c;
        rec.cb_d = (void *)(uintptr_t)spin_provider_cb_d;

        g_spin_provider_active = 1;
        result = spin_provider_install(g_spin_provider_table, 0x1000, &rec);
        g_spin_provider_active = 0;
        if (result != 1) {
            stage = "spin_unavailable";
            event = "provider_native_guards";
            payload = ",\"action\":25,\"other_features_unchanged\":true";
            goto out;
        }
    }

    g_online_move_bound = 1;
    g_online_move_engine = g_engine_base;
    memcpy(g_online_move_tmpl, (const void *)(uintptr_t)0x1c3270,
           sizeof g_online_move_tmpl);

    if (online_move_install() == 0
        || online_move_verify(&g_online_move_engine, &g_online_move_state) != 1) {
        stage = "spin_unavailable";
        event = "online_movement_native_guards";
        payload = ",\"action\":25,\"other_features_unchanged\":true";
        goto out;
    }

    g_spin_post_ready = 1;
    g_spin_frame_entry = g_engine_base + SPIN_FRAME_ENTRY_RVA;
    g_spin_frame_island = NULL;
    g_spin_frame_handler = (void (*)(void))(uintptr_t)holdfire_post_handler;
    g_spin_frame_cb = spin_post_callback;
    g_spin_frame_bl_slot = g_engine_base + SPIN_FRAME_BL_RVA;
    g_spin_frame_epoch = g_engine_epoch;
    g_spin_frame_engine = g_engine_base;
    g_spin_frame_stamp = ROD_SPIN_STAMP;
    g_spin_frame_bl = ROD_SPIN_BL_TMPL;

    island = (uint64_t)(uintptr_t)game_island_alloc();
    g_spin_frame_island = (void *)(uintptr_t)island;
    if (island == 0 || ((uint32_t)g_spin_frame_entry | (uint32_t)island) & 3) {
        stage = "spin_unavailable";
        event = "post_island";
        payload = ",\"action\":25,\"other_features_unchanged\":true";
        goto out;
    }
    if (island - g_spin_frame_entry - 0x7fffffdull
        <= 0xfffffffff0000002ull) {
        stage = "spin_unavailable";
        event = "post_island";
        payload = ",\"action\":25,\"other_features_unchanged\":true";
        goto out;
    }

    {
        uint8_t island_code[ISLAND_SIZE];
        uint8_t readback[ISLAND_SIZE];
        uint32_t delta;
        uint32_t q;

        if (frame_island_code_build(island_code) == 0) {
            stage = "spin_unavailable";
            event = "post_island";
            payload = ",\"action\":25,\"other_features_unchanged\":true";
            goto out;
        }

        delta = (uint32_t)(island - g_spin_frame_entry);
        q = (int32_t)delta < 0 ? delta + 3 : delta;
        g_spin_frame_bl = (((((uint64_t)q >> 2) << 32)
                           | (g_spin_frame_bl & 0xffffffffull))
                          & 0x3ffffffffffffffull) | 0x1400000000000000ull;

        memcpy((void *)(uintptr_t)island, island_code, ISLAND_SIZE);
        code_cache_flush((void *)(uintptr_t)island,
                         (void *)(uintptr_t)(island + ISLAND_SIZE));
        if (mprotect((void *)(uintptr_t)island, g_page_size,
                     PROT_READ | PROT_EXEC) != 0) {
            stage = "spin_unavailable";
            event = "post_prepublication_readback";
            payload = ",\"action\":25,\"other_features_unchanged\":true";
            goto out;
        }
        if (game_read(0, island, readback, ISLAND_SIZE) == 0
            || memcmp(island_code, readback, ISLAND_SIZE) != 0
            || spin_frame_post_verify(0) == 0) {
            stage = "spin_unavailable";
            event = "post_prepublication_readback";
            payload = ",\"action\":25,\"other_features_unchanged\":true";
            goto out;
        }
    }

    g_spin_move_entry = g_engine_base + SPIN_MOVE_ENTRY_RVA;
    g_spin_move_bl_slot = g_engine_base + SPIN_MOVE_BL_RVA;
    g_spin_move_island = NULL;
    g_spin_move_epoch = g_engine_epoch;
    g_spin_move_engine = g_engine_base;
    g_spin_move_stamp = ROD_SPIN_STAMP;
    g_spin_move_handler = (void (*)(void))(uintptr_t)holdfire_post_handler;
    g_spin_move_cb = spin_move_post_cb;
    g_spin_move_bl = ROD_MOVE_BL_TMPL;

    island2 = (uint64_t)(uintptr_t)game_island_alloc();
    g_spin_move_island = (void *)(uintptr_t)island2;
    if (island2 == 0 || ((uint32_t)g_spin_move_entry | (uint32_t)island2) & 3) {
        stage = "spin_unavailable";
        event = "movement_post_island";
        payload = ",\"action\":25,\"other_features_unchanged\":true";
        goto out;
    }
    if (island2 - g_spin_move_entry - 0x7fffffdull
        <= 0xfffffffff0000002ull) {
        stage = "spin_unavailable";
        event = "movement_post_island";
        payload = ",\"action\":25,\"other_features_unchanged\":true";
        goto out;
    }

    {
        uint8_t island_code[ISLAND_SIZE];
        uint8_t readback[ISLAND_SIZE];
        uint32_t delta;
        uint32_t q;

        if (movement_island_code_build(island_code) == 0) {
            stage = "spin_unavailable";
            event = "movement_post_island";
            payload = ",\"action\":25,\"other_features_unchanged\":true";
            goto out;
        }

        delta = (uint32_t)(island2 - g_spin_move_entry);
        q = (int32_t)delta < 0 ? delta + 3 : delta;
        g_spin_move_bl = (((((uint64_t)q >> 2) << 32)
                          | (g_spin_move_bl & 0xffffffffull))
                         & 0x3ffffffffffffffull) | 0x1400000000000000ull;

        memcpy((void *)(uintptr_t)island2, island_code, ISLAND_SIZE);
        code_cache_flush((void *)(uintptr_t)island2,
                         (void *)(uintptr_t)(island2 + ISLAND_SIZE));
        if (mprotect((void *)(uintptr_t)island2, g_page_size,
                     PROT_READ | PROT_EXEC) != 0) {
            stage = "spin_unavailable";
            event = "movement_post_prepublication_readback";
            payload = ",\"action\":25,\"other_features_unchanged\":true";
            goto out;
        }
        if (game_read(0, island2, readback, ISLAND_SIZE) == 0
            || memcmp(island_code, readback, ISLAND_SIZE) != 0
            || spin_move_post_verify(0) == 0) {
            stage = "spin_unavailable";
            event = "movement_post_prepublication_readback";
            payload = ",\"action\":25,\"other_features_unchanged\":true";
            goto out;
        }
    }

    result = spin_post_publish(g_spin_frame_entry,
                               (uint32_t)g_spin_frame_bl,
                               g_spin_frame_bl_hi);
    if (result < 0) {
        runtime_journal_write("fatal",
                              "spin_post_publication_restore_unverified",
                              NULL);
        abort();
    }
    if (result == 1)
        g_loader_verify_gate_a = 1;

    if (result == 1) {
        result = spin_post_publish(g_spin_move_entry,
                                   (uint32_t)g_spin_move_bl,
                                   g_spin_move_bl_hi);
        if (result < 0) {
            runtime_journal_write(
                "fatal", "spin_movement_post_publication_restore_unverified",
                NULL);
            abort();
        }
        if (result == 1) {
            g_loader_verify_gate_b = 1;
            if (spin_frame_post_verify(0) != 0
                && spin_move_post_verify(1) != 0) {
                runtime_journal_write(
                    "spin_post_installed",
                    "owned_final_and_movement_epilogues_no_capability_yet",
                    ",\"action\":25,\"frame_entry_rva\":\"0xb30690\","
                    "\"movement_entry_rva\":\"0xe7b768\","
                    "\"movement_function_rva\":\"0xe7af70\","
                    "\"original_call_added\":false,\"online_movement_bound\":true");
                return;
            }
            loader_identity_verify();
            stage = "spin_unavailable";
            event = "movement_post_publication";
            payload = ",\"action\":25,\"other_features_unchanged\":true";
            goto out;
        }
        stage = "spin_unavailable";
        event = "post_publication";
        payload = ",\"action\":25,\"other_features_unchanged\":true";
        goto out;
    }

    stage = "spin_unavailable";
    event = "post_publication";
    payload = ",\"action\":25,\"other_features_unchanged\":true";

out:
    g_spin_provider_active = 0;
    runtime_journal_write(stage, event, payload);
}

typedef struct {
    uint64_t stamp;
    uint64_t epoch;
    uint64_t generation;
    void *handler;
    void (*callback)(uint64_t);
    uint64_t reserved;
    void *cb_tmpl;
    void *reg_cb_a;
    void *reg_cb_b;
} spin_register_record_t;

void spin_handler_install(int acked)
{
    if (!(g_loader_verify_gate_a == 1 && g_loader_verify_gate_b != 0
          && g_online_move_bound != 0))
        return;

    if (acked == 0 || g_spin_loader_verified == 0 || g_spin_loader_generation == 0) {
        loader_identity_verify();
        runtime_journal_write("spin_unavailable",
                              "verified_lab_loader_identity_missing", NULL);
        return;
    }

    if (g_spin_validate_post_fn(&g_spin_frame_stamp) != 1) {
        loader_identity_verify();
        runtime_journal_write("spin_unavailable", "post_proof_rejected", NULL);
        return;
    }

    if (g_spin_validate_movement_fn(&g_spin_move_stamp) != 1) {
        loader_identity_verify();
        runtime_journal_write("spin_unavailable",
                              "movement_post_proof_rejected", NULL);
        return;
    }

    g_spin_move_apply_state = 1;
    g_online_move_state[0] = 0x100000001ull;

    {
        spin_register_record_t rec;
        int result;

        rec.stamp = ROD_SPIN_STAMP;
        rec.epoch = g_engine_epoch;
        rec.generation = g_spin_loader_generation;
        rec.handler = (void *)(uintptr_t)holdfire_post_handler;
        rec.callback = spin_post_callback;
        rec.reserved = 0;
        rec.cb_tmpl = (void *)(uintptr_t)ROD_SPIN_CB_TMPL;
        rec.reg_cb_a = (void *)(uintptr_t)spin_register_cb_a;
        rec.reg_cb_b = (void *)(uintptr_t)spin_register_cb_b;

        result = ((int (*)(const void *))g_spin_register_fn)(&rec);
        if (result == 1) {
            g_spin_state_flag = 1;
            runtime_journal_write(
                "spin_registered", "verified_action25_lab_trial",
                ",\"action\":25,\"scope\":\"LAB_TRIAL\","
                "\"modes\":[\"offline_facing_priority\",\"online_movement\"],"
                "\"rotation\":360,\"movement_reassert_rva\":\"0xe7b768\","
                "\"paid_state_changed\":false,\"free_grant\":false");
        } else {
            g_online_move_state[0] = 0x200000000ull;
            runtime_journal_write("spin_unavailable",
                                  "lab_trial_registration_rejected", NULL);
        }
    }
}

void spin_post_callback(uint64_t frame)
{
    int saved_errno = errno;
    uint64_t start_us;
    uint64_t now_ms;
    int32_t tid = g_init_thread_id;

    if (!(g_spin_state_flag == 1 && (int32_t)g_game_callback_acked != 0
          && g_init_thread_id != 0))
        goto out;
    if (!((int32_t)syscall(SYS_gettid) == tid && frame != 0))
        goto out;
    if (!((g_spin_move_gate & 1) == 0 && g_spin_telemetry[0] != 0
          && g_spin_telemetry[1] == 0))
        goto out;

    g_spin_telemetry[1] = 1;
    g_spin_pub_count++;

    if (g_spin_move_cfg_online == 1)
        goto release;

    if (!(((g_spin_frame_identity_a ^ *(const uint64_t *)(const void *)(frame + 0x98))
           & 0xffffffffffffffull) == 0
          && ((g_spin_frame_identity_b ^ *(const uint64_t *)(const void *)(frame + 0xc0))
              & 0xffffffffffffffull) == 0
          && g_spin_frame_identity_c == g_spin_move_gate))
        goto release;

    start_us = mono_now_us();
    now_ms = mono_now_ms_fn(1);
    if (now_ms == 0 || now_ms < g_spin_move_min_mono)
        goto release;

    {
        uint64_t exec_record[34];
        uint64_t provider_record[13];
        char buf[1400];
        int result;
        uint32_t ok_a;
        uint32_t ok_b;

        memset(exec_record, 0, sizeof exec_record);
        memset(provider_record, 0, sizeof provider_record);

        memcpy(&exec_record[4], &g_spin_telemetry[1], 13 * 8);
        memcpy(&exec_record[17], &g_spin_telemetry[14], 26 * 8);

        provider_record[0] = ROD_FRAME_HEADER;
        provider_record[4] = ROD_SPIN_STAMP;
        provider_record[5] = ROD_NG_BIND_TAG;
        provider_record[6] = (uint64_t)(uintptr_t)&exec_record[4];
        provider_record[7] = g_spin_frame_identity_a;
        provider_record[8] = (uint64_t)(uintptr_t)&exec_record[17];
        provider_record[9] = ROD_IDENTITY_A;
        provider_record[10] = ROD_IDENTITY_B;
        provider_record[11] = ROD_IDENTITY_C;
        provider_record[12] = now_ms;

        g_spin_move_gate = 1;
        result = spin_post_exec(provider_record, exec_record);
        ok_a = 0;
        if ((uint32_t)exec_record[1] != 0)
            ok_a = (uint32_t)(result != 0);
        ok_b = 0;
        if ((int32_t)exec_record[0] != 0)
            ok_b = (uint32_t)(result != 0);
        provider_record[5] = (uint64_t)result | (uint64_t)ok_a << 32;
        provider_record[4] = (provider_record[4] & 0xffffffffull)
                             | (uint64_t)ok_b << 32;

        spin_post_field_apply(g_spin_frame_identity_b);
        spin_provider_publish(g_spin_provider_table, provider_record,
                              &g_spin_pub_tag);

        g_spin_move_gate = 0;
        g_spin_move_gate_hi = 0;

        if (g_spin_pub_count < 9 || g_spin_pub_tag != g_spin_journal_tag
            || g_spin_move_last_journal == 0
            || 999 < now_ms - g_spin_move_last_journal) {
            snprintf(buf, 0x578,
                     ",\"action\":25,\"scope\":\"LAB_TRIAL\","
                     "\"mode\":\"offline\",\"publication\":%llu,"
                     "\"sequence\":%llu,\"epoch\":%llu,\"own_gid\":%d,"
                     "\"requested\":%u,\"generation\":%llu,"
                     "\"loader_generation\":%llu,"
                     "\"post_entry_rva\":\"0xb30690\",\"post_calls\":%llu,"
                     "\"active_known\":%u,\"active\":%u,\"ended\":%u,"
                     "\"provider_reason\":%u,\"angle\":%d,"
                     "\"writes_completed\":%u,\"readback_verified\":%u,"
                     "\"field_apply_calls\":%llu,\"duration_us\":%llu,"
                     "\"native_calls\":0,\"client_inputs\":0,"
                     "\"paid_state_changed\":false",
                     (unsigned long long)g_spin_telemetry[4],
                     (unsigned long long)g_spin_telemetry[2],
                     (unsigned long long)g_spin_telemetry[11],
                     (uint32_t)g_spin_telemetry[24],
                     (uint32_t)g_spin_telemetry[38],
                     (unsigned long long)g_spin_telemetry[35],
                     (unsigned long long)g_spin_loader_generation,
                     (unsigned long long)g_spin_pub_count,
                     (uint32_t)result, ok_a, ok_b,
                     g_spin_pub_reason, (int)g_spin_pub_angle,
                     g_spin_pub_writes, g_spin_pub_readback,
                     (unsigned long long)g_spin_pub_field_apply,
                     (unsigned long long)(mono_now_us() - start_us));
            runtime_journal_write("spin_post", "post_natural_local_facing", buf);
            g_spin_journal_tag = g_spin_pub_tag;
            g_spin_move_last_journal = now_ms;
        }
    }

release:
    g_spin_telemetry[1] = 0;
out:
    errno = saved_errno;
}

void loader_identity_verify(void)
{
    int result;

    if (g_loader_verify_gate_b == 1) {
        result = spin_move_post_verify(1);
        if (result != 0) {
            result = spin_post_publish(g_spin_move_entry,
                                       g_spin_move_bl_hi,
                                       g_spin_move_bl);
            if (result < 0) {
                runtime_journal_write(
                    "fatal", "spin_movement_post_restore_unverified", NULL);
                abort();
            }
            if (result == 1)
                g_loader_verify_gate_b = 0;
        }
    }

    if (g_loader_verify_gate_a == 1) {
        result = spin_frame_post_verify(1);
        if (result != 0) {
            result = spin_post_publish(g_spin_frame_entry,
                                       g_spin_frame_bl_hi,
                                       g_spin_frame_bl);
            if (result < 0) {
                runtime_journal_write("fatal", "spin_post_restore_unverified",
                                      NULL);
                abort();
            }
            if (result == 1)
                g_loader_verify_gate_a = 0;
        }
    }
}

void spin_move_callback(uint64_t frame)
{
    const uint64_t *f = (const uint64_t *)(uintptr_t)frame;
    uint64_t world;
    uint64_t now;
    uint64_t ptr;
    float angle;
    float sin_v;
    float cos_v;
    int target_x;
    int target_y;
    int apply;
    int result;
    uint8_t entity[44];
    char buf[800];

    if (frame == 0 || g_spin_post_ready == 0 || g_spin_telemetry[0] == 0)
        goto inactive;
    if (f[1] == 0)
        goto inactive;
    world = f[2];
    if (world == 0)
        goto inactive;
    if (*(const int32_t *)(const void *)(f + 0x24 / 8) == 0)
        goto inactive;
    if (g_spin_move_cfg_radius == 0 || g_spin_move_cfg_online != 1)
        goto inactive;
    if (g_spin_move_cfg_radius2 <= 0)
        goto inactive;
    if (g_init_thread_id == 0
        || (int32_t)syscall(SYS_gettid) != g_init_thread_id)
        goto inactive;
    if ((int32_t)g_spin_move_gate_hi == 0)
        goto inactive;

    ptr = 0;
    if (game_read((uint64_t)(uintptr_t)syscall(SYS_gettid),
                  *(const uint64_t *)(const void *)
                      ((const char *)f + 0x30) + 0x58,
                  &ptr, 8) == 0)
        goto inactive;
    if (ptr + 0x2000 < 0x12000 || (ptr & 7) != 0)
        goto inactive;
    if (frame_entity_resolve(g_engine_base,
                             *(const uint64_t *)(const void *)
                                 ((const char *)f + 0x40),
                             (void *)(uintptr_t)game_read, 0, entity) == 0)
        goto inactive;
    if (*(int32_t *)(const void *)((char *)entity + 40) == 0)
        goto inactive;

    {
        uint64_t rec_in[8];
        uint64_t rec_out[5];
        uint64_t *e = (uint64_t *)(uintptr_t)world;

        rec_in[0] = *(const uint64_t *)(const void *)((const char *)f + 0x20);
        rec_in[1] = *(const uint64_t *)(const void *)((const char *)f + 0x28);
        rec_in[2] = *(const uint64_t *)(const void *)((const char *)f + 0x38);
        rec_in[3] = *(const uint64_t *)(const void *)((const char *)f + 0x30);
        rec_in[4] = *(const uint64_t *)(const void *)
                    (*(const uint64_t *)(const void *)(f + 1) + 8);
        rec_in[5] = *(const uint64_t *)(const void *)((const char *)f + 0x40);
        rec_in[6] = *(const uint64_t *)(const void *)((const char *)e + 0x70);
        rec_in[7] = 0;
        memset(rec_out, 0, sizeof rec_out);
        rec_out[0] = ROD_FRAME_HEADER;

        if (!(g_frame_parse_enabled == 1
              && frame_record_parse(&g_frame_record_table, rec_in, rec_out) == 1
              && (int32_t)rec_out[1] != 0
              && (uint32_t)(rec_out[1] >> 32) != 0
              && (int32_t)rec_out[2] == 0))
            goto inactive;

        g_spin_online_state[2] = *(const uint64_t *)(const void *)
                                 ((const char *)e + 0x20);
        g_spin_online_state[4] = *(const uint64_t *)(const void *)
                                 ((const char *)e + 0x28);
        g_spin_online_state[5] = *(const uint64_t *)(const void *)
                                 ((const char *)e + 0x38);
        g_spin_online_state[3] = *(const uint64_t *)(const void *)
                                 ((const char *)e + 0x30);
        g_spin_online_state[6] = *(const uint64_t *)(const void *)
                                 ((const char *)e + 0x40);
        g_spin_online_state[7] = *(const uint64_t *)(const void *)
                                 ((const char *)e + 0x70);
        g_spin_online_state[0] = ROD_ONLINE_STAMP;
        g_spin_online_state[1] = 1;
        g_spin_online_state[8] = (uint64_t)(uint32_t)g_spin_move_gate_hi;
        g_spin_online_state[9] = *(const uint64_t *)(const void *)
                                 ((const char *)e + 0x78);
        g_spin_online_state[10] = 0;
        g_spin_online_state[11] = 0;
        g_spin_online_state[12] = 0;
        g_spin_online_state[13] = g_spin_frame_identity_c;
        g_spin_online_state[14] = ptr;
        g_spin_online_state[15] = ROD_IDENTITY_A;
        g_spin_online_state[16] = ROD_IDENTITY_C;
        g_spin_online_state[17] = ROD_IDENTITY_B;
        g_spin_online_state[19] = 0;
    }

    if (g_spin_move_cfg_stand != 0
        && (g_spin_stand_latch == 0
            || spin_online_entity_valid(g_spin_online_state) == 0)) {
        g_spin_move_yields++;
        if (g_spin_move_active == 1)
            g_spin_move_count++;
        g_spin_move_active = 0;
        g_spin_move_entity = 0;
        g_spin_move_last_tick = 0;
        g_spin_move_last_apply_tick = 0;
        g_spin_move_own_static = 0;
        g_spin_move_angle = 0.0f;
        g_spin_move_saved = (uint32_t)g_spin_move_count;
        g_online_move_state[1] = ROD_ONLINE_RESET;
        g_spin_move_apply_state = (uint32_t)g_spin_online_state[13];
        g_spin_move_apply_active = (uint32_t)(g_spin_online_state[13] >> 32);
        g_spin_move_last_journal = g_spin_move_last_journal;
        return;
    }

    g_spin_move_apply_active = 0;
    if (g_spin_move_entity != 0)
        g_spin_move_count = g_spin_move_count;
    g_spin_move_saved = (uint32_t)g_spin_move_count;
    g_online_move_state[1] = ROD_ONLINE_RESET;
    g_spin_move_apply_state = (uint32_t)g_spin_move_gate_hi;

    if (g_spin_move_entity == g_spin_online_state[6]
        && g_spin_move_own_static == g_spin_online_state[2]
        && g_spin_move_last_tick != 0) {
        uint64_t delta;

        now = f[5];
        delta = now - g_spin_move_last_tick;
        if (now < g_spin_move_last_tick || delta > 0xfa)
            goto resync;
        if (delta > 99)
            delta = 100;
        if (delta > 0xf) {
            g_spin_move_angle = fmaf((float)delta * (float)(int)g_spin_move_cfg_rate,
                                     1.0e-7f, g_spin_move_angle);
            g_spin_move_last_tick = now;
        }
        angle = g_spin_move_angle;
        while (!(angle < 6.2831855f))
            angle -= 6.2831855f;
        g_spin_move_angle = angle;
    } else {
resync:
        if (g_spin_move_active == 1)
            g_spin_move_count++;
        g_spin_move_active = 0;
        g_spin_move_own_static = *(const uint64_t *)(const void *)
                                 ((const uint64_t *)(uintptr_t)world + 4);
        g_spin_move_last_tick = f[5];
        g_spin_move_last_apply_tick = 0;
        g_spin_move_angle = 0.0f;
        g_spin_move_saved = (uint32_t)g_spin_move_count;
        g_spin_move_entity = g_spin_online_state[6];
    }

    g_spin_move_apply_state = 0;
    g_spin_telemetry[47] = 1;
    g_spin_move_yields++;
    g_online_move_state[1] = g_spin_online_state[13];

    spin_online_preapply();

    {
        const uint64_t *e = (const uint64_t *)(uintptr_t)world;

        sincosf(g_spin_move_angle, &sin_v, &cos_v);
        target_y = (int)*(const int32_t *)(const void *)((const char *)e + 0x74)
                   + (int)cos_v;
        target_x = (int)*(const int32_t *)(const void *)((const char *)e + 0x78)
                   + (int)sin_v;
        if ((int)cos_v == 0 && (int)sin_v == 0)
            target_y++;
        g_spin_move_count = (uint64_t)(uint32_t)target_x
                            | (uint64_t)(uint32_t)target_y << 32;
    }

    apply = g_spin_move_apply_active == 0 || f[5] < g_spin_move_last_apply_tick
            || f[5] - g_spin_move_last_apply_tick > 0x17;
    if (apply) {
        result = ((int (*)(uint64_t, void *, int, int, int))
                  (uintptr_t)g_online_move_state[4])(
            g_online_move_state[3], (void *)(uintptr_t)g_spin_online_state, 1,
            target_y, target_x);
        if (result != 1)
            goto apply_rejected;

        ((void (*)(uint64_t, uint64_t, int, int, int))
         (uintptr_t)g_online_move_state[10])(
            g_online_move_state[3],
            *(const uint64_t *)(const void *)((const uint64_t *)(uintptr_t)world + 7),
            target_y, target_x, 1);

        g_spin_move_apply_active = 1;
        g_online_move_state[4] = g_spin_online_state[5];
        g_online_move_state[3] = g_spin_online_state[3];
        g_online_move_state[6] = g_spin_online_state[5];
        g_online_move_state[5] = g_spin_online_state[4];
        g_spin_move_apply_state = 1;
        g_spin_telemetry[48]++;
        g_spin_move_active = 1;
        g_spin_move_last_apply_tick = f[5];
    } else if (g_spin_move_apply_active == 0) {
        g_spin_move_apply_state = 7;
        goto journal;
    } else {
apply_rejected:
        if (g_spin_move_apply_state == 0)
            g_spin_move_apply_state = 5;
    }

journal:
    {
        uint64_t now_tick = f[5];

        if (g_spin_move_yields < 9 || g_spin_move_last_journal == 0
            || 999 < now_tick - g_spin_move_last_journal) {
            snprintf(buf, sizeof buf,
                     ",\"action\":25,\"mode\":\"online\","
                     "\"publication\":%llu,\"source_tick\":%u,"
                     "\"epoch\":%llu,\"own_gid\":%d,"
                     "\"configured_radius\":%d,"
                     "\"effective_radius_raw\":1,\"only_standing\":%d,"
                     "\"priority_yields\":%llu,\"phase\":%.6f,"
                     "\"apply_reason\":%u,\"pending\":%u,"
                     "\"local_calls\":%llu,\"queue_calls\":%llu,"
                     "\"target_x\":%d,\"target_y\":%d,"
                     "\"native_acceptance_proven\":false",
                     (unsigned long long)g_spin_online_state[13],
                     (unsigned)g_spin_move_gate_hi,
                     (unsigned long long)g_spin_online_state[2],
                     (int)g_spin_online_state[1],
                     (int)g_spin_move_cfg_radius2,
                     (int)g_spin_move_cfg_stand,
                     (unsigned long long)g_spin_move_yields,
                     (double)g_spin_move_angle,
                     g_spin_move_apply_state,
                     (unsigned)g_spin_move_active,
                     (unsigned long long)g_spin_telemetry[48],
                     (unsigned long long)g_spin_telemetry[46],
                     (int)(uint32_t)g_spin_move_count,
                     (int)(uint32_t)(g_spin_move_count >> 32));
            runtime_journal_write("spin_move", "tale_style_real_client_input",
                                  buf);
            g_spin_move_last_journal = now_tick;
        }
    }
    return;

inactive:
    if (g_spin_move_active == 1)
        g_spin_move_count++;
    g_spin_move_active = 0;
    g_spin_move_apply_state = 0;
    g_spin_move_entity = 0;
    g_spin_move_last_apply_tick = 0;
    g_spin_move_last_tick = 0;
    g_spin_move_own_static = 0;
    g_spin_move_angle = 0.0f;
    g_spin_move_saved = (uint32_t)g_spin_move_count;
}

#ifdef NEXUS_WITH_JNI
#include <jni.h>

extern uint32_t g_evasion_game_acked;

void handlers_install_main(JNIEnv *env, jclass loader_cls, int acked)
{
    jfieldID field_d;
    jfieldID field_e;
    jfieldID field_str;
    jfieldID field_cb;
    jobject loader_d;
    jobject loader_e;
    jclass d_cls;
    jstring phase;
    jstring profile;
    jstring str;
    jlong revision = 0;
    const char *utf;
    char phase_buf[0xa0];
    char profile_buf[0x41];
    char self_path[0x1000];
    char self_sha[0x41];
    char payload[0x140];
    const char *stage = "core";
    const char *event;
    const char *extra;
    int refused = 1;
    jsize len;
    int i;

    (void)acked;

    if (env == NULL || g_evasion_game_acked == 0)
        return;
    if (!(g_files_dir_fd >= 0 && g_engine_epoch != 0))
        return;

    if ((*env)->PushLocalFrame(env, 0x80) < 0) {
        (*env)->ExceptionClear(env);
        return;
    }

    field_d = (*env)->GetStaticFieldID(env, loader_cls, ROD_LOADER_FIELD_D,
                                       ROD_LOADER_SIG_D);
    loader_d = field_d == NULL ? NULL
        : (*env)->GetStaticObjectField(env, loader_cls, field_d);
    if (loader_d == NULL || (*env)->ExceptionCheck(env)
        || (d_cls = (*env)->GetObjectClass(env, loader_d)) == NULL)
        goto out;

    field_e = (*env)->GetFieldID(env, d_cls, ROD_LOADER_FIELD_E,
                                 ROD_LOADER_SIG_E);
    loader_e = field_e == NULL ? NULL
        : (*env)->GetObjectField(env, loader_d, field_e);
    (*env)->DeleteLocalRef(env, d_cls);
    if (loader_e == NULL || (*env)->ExceptionCheck(env)) {
        stage = "core";
        goto out;
    }

    if (!(*env)->ExceptionCheck(env)
        && (d_cls = (*env)->GetObjectClass(env, loader_e)) != NULL) {
        field_str = (*env)->GetFieldID(env, d_cls, ROD_SIG_STRING_A,
                                       "Ljava/lang/String;");
        phase = field_str == NULL ? NULL
            : (*env)->GetObjectField(env, loader_e, field_str);
        (*env)->DeleteLocalRef(env, d_cls);
    } else {
        phase = NULL;
    }

    if (phase == NULL || (*env)->ExceptionCheck(env)) {
        stage = "core";
        goto out;
    }

    len = (*env)->GetStringUTFLength(env, phase);
    if (len < 0 || len >= 0xa0 || (*env)->ExceptionCheck(env))
        goto out;
    utf = (*env)->GetStringUTFChars(env, phase, NULL);
    if (utf == NULL)
        goto out;
    memcpy(phase_buf, utf, (size_t)len);
    phase_buf[len] = '\0';
    (*env)->ReleaseStringUTFChars(env, phase, utf);

    if (memcmp(phase_buf, "linking", 8) != 0
        || (*env)->ExceptionCheck(env))
        goto out;

    if (!(*env)->ExceptionCheck(env)
        && (d_cls = (*env)->GetObjectClass(env, loader_e)) != NULL) {
        field_str = (*env)->GetFieldID(env, d_cls, ROD_SIG_STRING_B,
                                       "Ljava/lang/String;");
        str = field_str == NULL ? NULL
            : (*env)->GetObjectField(env, loader_e, field_str);
        (*env)->DeleteLocalRef(env, d_cls);
    } else {
        str = NULL;
    }

    if (str == NULL || (*env)->ExceptionCheck(env))
        goto out;

    len = (*env)->GetStringUTFLength(env, str);
    if (len < 0 || len >= 0xa0 || (*env)->ExceptionCheck(env))
        goto out;
    utf = (*env)->GetStringUTFChars(env, str, NULL);
    if (utf == NULL)
        goto out;
    memcpy(phase_buf, utf, (size_t)len);
    phase_buf[len] = '\0';
    (*env)->ReleaseStringUTFChars(env, str, utf);

    if (!(memcmp(phase_buf, "EvasionGame", 12) == 0
          && !(*env)->ExceptionCheck(env)))
        goto out;

    if (!(*env)->ExceptionCheck(env)
        && (d_cls = (*env)->GetObjectClass(env, loader_e)) != NULL) {
        field_cb = (*env)->GetFieldID(env, d_cls, ROD_CB_FIELD_NAME,
                                      ROD_CB_FIELD_SIG);
        if (field_cb != NULL) {
            if ((*env)->GetBooleanField(env, loader_e, field_cb) == JNI_TRUE
                && !(*env)->ExceptionCheck(env)
                && jni_get_boolean_field(env, loader_e, ROD_CB_BOOL_NAME) != 0)
                goto verified;
        }
        (*env)->DeleteLocalRef(env, d_cls);
    }
    goto out;

verified:
    profile = (jstring)(uintptr_t)jni_get_object_field(
        env, loader_e, "profileSha", "Ljava/lang/String;");
    if (profile == NULL
        || jni_string_read(env, profile, profile_buf, 0x41) == 0
        || memcmp(profile_buf, PROFILE_SHA, 0x41) != 0) {
        stage = "selected_profile";
        goto out;
    }

    if (jni_get_int_field(env, loader_e, "revision", &revision) == 0
        || revision <= 0
        || jni_get_boolean_field(env, loader_e, "localAllowed") == 0) {
        stage = "selected_profile";
        goto out;
    }

    if (!jni_string_equals(env, (jstring)(uintptr_t)jni_get_object_field(
                               env, loader_e, "transport", "Ljava/lang/String;"),
                           "https_release"))
        goto out;
    if (!jni_string_equals(env, (jstring)(uintptr_t)jni_get_object_field(
                               env, loader_e, "gameSha", "Ljava/lang/String;"),
                           GAME_SHA))
        goto out;

    {
        jobject record = (jobject)(uintptr_t)jni_get_object_field(
            env, loader_d, ROD_MODULES_NAME, ROD_SIG_MAP);
        jobject acked_obj = (jobject)(uintptr_t)jni_map_value(env, record);
        jclass bool_cls;
        jmethodID bool_mid;
        jobject modules;
        jclass list_cls;
        jmethodID size_mid;
        jmethodID get_mid;
        jsize count;
        int found = 0;
        int64_t bytes = 0;

        if (acked_obj == NULL || (*env)->ExceptionCheck(env))
            goto record_fail;
        bool_cls = (*env)->GetObjectClass(env, acked_obj);
        if (bool_cls == NULL)
            goto record_fail;
        bool_mid = (*env)->GetMethodID(env, bool_cls, "booleanValue",
                                       "()Ljava/lang/Boolean;");
        if (bool_mid == NULL
            || (*env)->CallBooleanMethod(env, acked_obj, bool_mid) != JNI_TRUE
            || (*env)->ExceptionCheck(env))
            goto record_fail;

        modules = (jobject)(uintptr_t)jni_get_object_field(
            env, loader_e, "modules", ROD_SIG_LIST);
        if (modules == NULL
            || (*env)->GetObjectClass(env, modules) == NULL)
            goto record_fail;

        list_cls = (*env)->GetObjectClass(env, modules);
        size_mid = (*env)->GetMethodID(env, list_cls, "size", "()I");
        get_mid = (*env)->GetMethodID(env, list_cls, "get",
                                      "(I)Ljava/lang/Object;");
        if (size_mid == NULL || get_mid == NULL
            || (*env)->ExceptionCheck(env))
            goto record_fail;

        count = (*env)->CallIntMethod(env, modules, size_mid);
        if ((uint32_t)(count - 0x11) < 0xfffffff0
            || (*env)->ExceptionCheck(env)) {
            stage = "selected_module";
            goto out;
        }

        for (i = 0; i < count; i++) {
            jobject module = (*env)->CallObjectMethod(env, modules, get_mid, i);
            jstring name;
            jobject cb_str;

            if (module == NULL || (*env)->ExceptionCheck(env))
                goto out;

            name = (jstring)(uintptr_t)jni_get_object_field(
                env, module, ROD_MODULE_NAME_FIELD, "Ljava/lang/String;");
            if (jni_string_equals(env, name, "EvasionGame")) {
                if (jni_get_boolean_field(env, module, "callback") == 0)
                    goto out;
                cb_str = (jstring)(uintptr_t)jni_get_object_field(
                    env, module, "name", "Ljava/lang/String;");
                if (!jni_string_equals(env, cb_str, "jni-onload-v1"))
                    goto out;
                cb_str = (jstring)(uintptr_t)jni_get_object_field(
                    env, module, "sha", "Ljava/lang/String;");
                if (cb_str == NULL
                    || jni_string_read(env, cb_str, self_sha, 0x41) == 0
                    || jni_get_int_field(env, module, "bytes", &bytes) == 0)
                    goto out;
                found++;
            }
            if (name != NULL)
                (*env)->DeleteLocalRef(env, name);
            (*env)->DeleteLocalRef(env, module);
        }

        if (found != 1 || bytes < 0x40 || bytes > 0x100000
            || (*env)->ExceptionCheck(env)) {
            stage = "selected_module";
            goto out;
        }

        {
            jobject path_map = (jobject)(uintptr_t)jni_get_object_field(
                env, loader_d, ROD_MAP_NAME_A, ROD_SIG_MAP);
            jobject path_obj = (jobject)(uintptr_t)jni_map_value(env, path_map);
            Dl_info info;

            if (path_obj == NULL
                || jni_string_read(env, (jstring)path_obj, self_path,
                                   0x1000) == 0
                || !sha256_prefix_equal(self_sha, self_path + 0x1000)) {
                stage = "selected_self_path_sha";
                goto out;
            }

            if (dladdr((void *)(uintptr_t)spin_post_callback, &info) == 0
                || info.dli_fbase == NULL || info.dli_fname == NULL
                || !file_accessible(self_path)
                || game_image_sha_verify(self_path, (size_t)bytes,
                                         (void *)(uintptr_t)0x10ef58) == 0) {
                stage = "selected_self_path_sha";
                goto out;
            }
        }

        {
            jfieldID fresh_e;
            jobject fresh_obj;
            jlong fresh_rev = 0;

            fresh_e = (*env)->GetFieldID(env, (*env)->GetObjectClass(env, loader_d),
                                         ROD_LOADER_FIELD_E, ROD_LOADER_SIG_E);
            fresh_obj = fresh_e == NULL ? NULL
                : (*env)->GetObjectField(env, loader_d, fresh_e);
            if (fresh_obj == NULL
                || !(*env)->IsSameObject(env, loader_e, fresh_obj)
                || !(*env)->IsSameObject(env, loader_e, fresh_obj)) {
                stage = "selected_plan_changed";
                goto out;
            }

            if (jni_get_int_field(env, fresh_obj, "revision", &fresh_rev) == 0
                || fresh_rev != revision) {
                stage = "selected_plan_changed";
                goto out;
            }

            if (!jni_string_equals(env,
                                   (jstring)(uintptr_t)jni_get_object_field(
                                       env, loader_d, ROD_SIG_STRING_A,
                                       "Ljava/lang/String;"),
                                   "linking"))
                goto out;
            if (!jni_string_equals(env,
                                   (jstring)(uintptr_t)jni_get_object_field(
                                       env, loader_d, ROD_SIG_STRING_B,
                                       "Ljava/lang/String;"),
                                   "EvasionGame"))
                goto out;
            if ((*env)->ExceptionCheck(env))
                goto out;

            g_spin_loader_generation = (uint64_t)revision;
            g_spin_loader_verified = 1;
            refused = 0;
            memcpy(g_spin_profile_sha, profile_buf, 8 * 8);
        }
        goto out;

record_fail:
        stage = "callback_record";
    }

out:
    if ((*env)->ExceptionCheck(env))
        (*env)->ExceptionClear(env);
    (*env)->PopLocalFrame(env, NULL);

    if (refused) {
        event = "spin_loader_refused";
        stage = stage != NULL ? stage : "core";
        extra = ",\"action\":25,\"capability_issued\":false";
    } else {
        snprintf(payload, sizeof payload,
                 ",\"action\":25,\"loader_generation\":%llu,"
                 "\"profile_sha256\":\"%s\",\"callback_verified\":true,"
                 "\"self_file_verified\":true,\"paid_state_changed\":false",
                 (unsigned long long)g_spin_loader_generation, PROFILE_SHA);
        event = "spin_loader_verified";
        stage = "actual_signed_release_selection";
        extra = payload;
    }
    runtime_journal_write(event, stage, extra);
}
#endif


