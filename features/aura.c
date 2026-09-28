#define _GNU_SOURCE

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <dlfcn.h>
#include <link.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/syscall.h>

#define PROP_FEATURE 0
#define PROP_PARAM   1

#define SNAPSHOT_VERSION 1

#define PRED_SNAPSHOT_SIZE 0x88
#define HOLD_SNAPSHOT_SIZE 0xc0
#define AURA_SNAPSHOT_SIZE 0x48
#define AURA_ROUTE_SIZE    0x10

#define AURA_FRAME_REC_SIZE  0x38
#define AURA_WEAPON_REC_SIZE 96

#define ENGINE_OBSERVER_RVA 0xb337c4u

#define ROD_PRED_HEADER    (*(const uint64_t *)(uintptr_t)0x1047f0)
#define ROD_PRED_PROVIDER  (*(const uint64_t *)(uintptr_t)0x1047f8)
#define ROD_HOLD_HEADER    (*(const uint64_t *)(uintptr_t)0x1047d8)
#define ROD_AURA_HEADER    (*(const uint64_t *)(uintptr_t)0x1047a0)
#define ROD_FRAME_HEADER   (*(const uint64_t *)(uintptr_t)0x10e7a8)
#define ROD_IMAGE_TAG      (*(const uint64_t *)(uintptr_t)0x10e580)
#define ROD_FAMILY_TAG     (*(const uint64_t *)(uintptr_t)0x10e800)
#define ROD_NG_BIND_TAG    (*(const uint64_t *)(uintptr_t)0x10e7f8)
#define ROD_PHDR_EXPECTED  (*(const uint64_t *)(uintptr_t)0x10e600)
#define ROD_SHA_TEMPLATE   ((const void *)(uintptr_t)0x10ef58)
#define ROD_NG_ENTRY_TMPL  ((const void *)(uintptr_t)0x1c2d60)
#define ROD_FAMILY_TMPL    ((const void *)(uintptr_t)0x1c31c0)

static const char GAME_SHA[] =
    "a10aeb6b4085fb2a15d969a130cd9608231998b5269a40aed41129d99624ede3";
static const char BACKEND_SHA[] =
    "fdf834103d333f9f8a1947b3a405b32da6ebb651";
static const char BACKEND_BOUND_PAYLOAD[] =
    ",\"game_sha256\":\"a10aeb6b4085fb2a15d969a130cd9608231998b5269a40aed41129d99624ede3\","
    "\"backend_sha256\":\"70bbfc332f9e8682046bf278782228a7478ae83cbe97d3e3ed6acab4bc2387c4\"";

extern char g_handler_lock;
extern uint64_t g_state_epoch;
extern int32_t g_state_status;
extern uint64_t g_state_epoch2;
extern uint64_t g_triple_gate_a;
extern uint64_t g_triple_gate_b;
extern uint64_t g_triple_gate_c;
extern void *g_adapter_ctx;
extern uint32_t (*g_adapter_mask_fn)(void *);
extern uint64_t g_holdfire_epoch;
extern uint64_t g_holdfire_aux;
extern uint64_t g_holdfire_aux2;
extern uint64_t g_holdfire_aux3;
extern uint64_t g_port_gate_epoch;
extern uint64_t g_port_gate_serial;
extern int32_t g_prop_state[144];

extern void handler_state_refresh(void);
extern uint64_t port_bitmap(void);
extern int holdfire_gate_a(void);
extern int holdfire_gate_b(int flags);
extern uintptr_t game_read(uintptr_t handle, uintptr_t addr, void *out,
                          uint32_t len);

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

extern int (*g_aura_consumer_fn)(uint64_t, void *);

extern uint64_t g_frame_family_table[];
extern uint64_t g_frame_record_table[];

extern void *g_evasion_lib_handle;
extern void *g_expected_lib_base;
extern char g_expected_lib_path[];
extern void (*g_register_handlers_fn)(void *);
extern int (*g_attempt_stamp_fn)(void *);
extern int (*g_attempt_recheck_fn)(void *);
extern void *g_aura_snapshot_fn;
extern int (*g_register_frame_family_fn)(const void *);
extern int (*g_bind_image_fn)(const void *);
extern void *g_publish_observer_fn;
extern int (*g_get_requested_fn)(void *);
extern int (*g_dodge_profile_fn)(void *);

extern uintptr_t g_engine_base;
extern void *g_java_vm;
extern uint64_t g_engine_epoch;
extern char g_journal_name[0x80];
extern int32_t g_journal_fd;
extern int32_t g_paired_backend_base;
extern int32_t g_files_dir_fd;
extern int32_t g_pair_channel_fd;
extern const char *g_runtime_init_reason;
extern void *g_status_cell_a;
extern void *g_status_cell_b;
extern void *g_status_cell_c;
extern uint64_t g_page_size;
extern uint32_t g_mapped_game_count;
extern uint32_t g_mapped_paired_count;
extern char g_game_sha_buf[0x1000];
extern int32_t g_entry_write_result;
extern uint32_t g_evasion_game_acked;
extern int32_t g_init_thread_id;
extern uint32_t g_game_callback_acked;

extern uint64_t g_periodic_stop_once;
extern uint64_t g_periodic_worker_run;
extern uint64_t g_periodic_worker_aux;
extern uint64_t g_periodic_worker_flag;
extern uint64_t g_periodic_generation;
extern uint64_t g_frame_stats[28];
extern uint32_t g_frame_stats_gen;
extern uint64_t g_gameplay_stats[26];
extern uint64_t g_cache_state_a;
extern uint64_t g_cache_state_b;
extern uint64_t g_cache_lock;
extern uint64_t g_cache_flag;
extern char g_telemetry_cache[0x784];

extern uint32_t g_visual_counter_a;
extern uint32_t g_visual_counter_b;
extern uint32_t g_visual_flag;
extern uint32_t g_visual_flag2;
extern void (*g_teardown_fn_a)(void *);
extern void (*g_teardown_fn_b)(void *);
extern void (*g_teardown_fn_c)(void *);
extern void (*g_teardown_fn_d)(void *);
extern void (*g_teardown_fn_e)(void *);
extern uint64_t g_gl_state_a;
extern uint64_t g_gl_state_b;
extern uint64_t g_gl_state_c;
extern uint64_t g_teardown_gen_a;
extern char g_teardown_armed_d;
extern uint64_t g_teardown_gen_b;
extern uint64_t g_teardown_gen_c;
extern uint64_t g_teardown_gen_d;
extern char g_teardown_armed_e;
extern uint64_t g_visual_state_e;
extern uint64_t g_gl_zero_a;
extern uint64_t g_gl_zero_b;
extern uint32_t g_mod_flag;
extern uint64_t g_mod_state_block[7];
extern uint64_t g_spin_state_flag;
extern uint64_t g_spin_telemetry[53];
extern char g_spin_armed;
extern void (*g_spin_register_fn)(void *);
extern uint32_t g_loader_verify_gate_a;
extern char g_loader_verify_gate_b;
extern uint32_t g_periodic_counter;
extern char g_periodic_armed;
extern uint32_t g_periodic_saved;
extern uint32_t g_periodic_field_a;
extern uint32_t g_periodic_field_b;
extern uint32_t g_periodic_field_c;
extern uint32_t g_periodic_field_d;
extern uint32_t g_periodic_field_e;
extern uint32_t g_periodic_field_f;
extern char g_event_routes_armed;
extern uint64_t g_event_routes_state[1];

extern char g_aura_adapter_ready;
extern uint64_t g_aura_route_lock;
extern uint64_t g_frame_parse_enabled;
extern uint64_t g_entry_write_ctx;
extern uint64_t g_ng_entry_state;
extern uint64_t g_aura_dedup_epoch;
extern uint64_t g_aura_dedup_known;
extern uint32_t g_aura_dedup_eligible;
extern uint32_t g_aura_dedup_ended;
extern uint64_t g_aura_dedup_ms;
extern uint64_t g_aura_wrapper_calls;
extern uint64_t g_aura_calls_ended;
extern uint64_t g_aura_last_call_ms;
extern uint32_t g_aura_state_reason;
extern uint64_t g_aura_state_ms;
extern uint64_t g_aura_guard;

extern void runtime_journal_write(const char *stage, const char *reason,
                                  const char *payload);
extern int family_callback_register(void *table,
                                    int (*callback)(void *, void *));
extern void family_active_set(void *table, int active);
extern void family_record_read(void *table, void *record_out);
extern int frame_record_parse(void *table, const void *frame, void *record_out);
extern int frame_record_install(void *table, uint64_t capacity,
                                const void *record);
extern uint64_t frame_record_capacity(void);
extern uint64_t frame_family_capacity(void);
extern int frame_family_install(void *table, uint64_t capacity,
                                const void *record);
extern int frame_stats_block_current(void *block, uint32_t *out_value);
extern int game_thread_present(void);
extern int files_dir_open(const char *path);
extern int phdr_find_game_lib(struct dl_phdr_info *info, size_t size, void *data);
extern int phdr_count_game_libs(struct dl_phdr_info *info, size_t size, void *data);
extern int phdr_check_build_id(struct dl_phdr_info *info, size_t size, void *data);
extern int game_image_sha_verify(void *out, size_t len, const void *template_);
extern int evasion_backend_bind(void);
extern int ng_bind_v1(void *record);
extern int function_routes_bind(void);
extern void *status_cell(void *key);
extern void spin_exports_bind(void);
extern void function_routes_publish(void);
extern void *entry_write_prepare(void *entry);
extern const char *entry_prep_fail_report(void);
extern int ng_entry_write(void *entry, void *state, void *template_);
extern int owned_observer_proof(void);
extern int persistent_handlers_register(int acked);
extern int owned_handlers_proof(int acked);
extern void visual_tweaks_restore(void);
extern void periodic_worker_join(void);
extern int loader_identity_verify(void);
extern int event_route_restore(void *routes);
extern void observer_state_reset(uint64_t a, uint64_t b, uint64_t c);
extern void gameplay_state_reset(uint64_t a, uint64_t b);
extern void nexus_visual_gl_disable_v1(void);
extern void frame_tick_stamp(void);
extern int attempt_stamp_bridge(void *self, void *arg);
extern int attempt_recheck_bridge(void *self, void *arg);
extern void frame_family_cb_a(void);
extern void frame_family_cb_g(void);
extern void handler_install_b(int acked);
extern void handler_install_d(int acked);
extern void handler_install_f(int acked);
extern void aura_phase_publisher_install(void);
extern void trophies_handler_install(int acked);
extern void spin_handler_install(int acked);

static int aura_frame_callback(void *self, void *frame);

typedef struct {
    uint64_t f0;
    uint64_t sequence;
    uint64_t epoch;
    uint64_t screen;
    uint64_t f20;
    uint64_t client;
    uint32_t own_gid;
} aura_frame_t;

typedef struct {
    uint64_t header;
    uint32_t active_known;
    uint32_t active_eligible;
    uint32_t f8;
    uint32_t fc;
    uint32_t f10;
    int32_t active_route;
    uint32_t f18;
    uint32_t f1c;
    uint32_t f20;
    uint32_t f24;
    uint32_t active_ended;
    uint32_t f2c;
} aura_frame_record_t;

_Static_assert(sizeof(aura_frame_record_t) == AURA_FRAME_REC_SIZE,
               "aura_frame_record_t size");

typedef struct {
    uint32_t aura_reason;
    uint32_t wrapper_called;
    uint32_t evidence_ready;
    int32_t wrapper_raw_result;
    int32_t raw_x;
    int32_t raw_y;
    uint32_t f18;
    uint32_t f1c;
    uint64_t worker_target;
    uint64_t final_target;
    uint64_t settings_generation;
} aura_state_record_t;

typedef struct {
    uint32_t weapon_known;
    uint32_t weapon_radius_raw;
    uint32_t weapon_bypass;
    uint32_t weapon_variants;
    uint32_t reserved;
    char weapon_name[76];
} aura_weapon_record_t;

_Static_assert(sizeof(aura_weapon_record_t) == AURA_WEAPON_REC_SIZE,
               "aura_weapon_record_t size");

typedef struct {
    uint64_t header;
    uintptr_t engine;
    const char *game_sha;
    const char *backend_sha;
    void *read_fn;
    void *reserved;
} image_bind_record_t;

typedef struct {
    uint64_t header;
    uintptr_t engine;
    const char *game_sha;
    const char *backend_sha;
    void *read_ctx;
    void *read_fn;
} frame_record_bind_t;

typedef struct {
    uint64_t header;
    uintptr_t engine;
    const char *game_sha;
    const char *backend_sha;
    void *read_ctx;
    void *read_fn;
    void *cb[8];
} frame_family_record_t;

typedef struct {
    uint64_t header;
    uintptr_t engine;
    const char *game_sha;
    const char *backend_sha;
    void *read_fn;
    void *reserved;
    void *get_requested_fn;
    void *dodge_profile_fn;
} ng_bind_record_t;

typedef struct {
    uint64_t word;
    uint64_t count;
} phdr_game_match_t;

static aura_frame_record_t g_aura_frame;

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

static int32_t snapshot_prop(const char *name)
{
    const evasion_prop_entry_t *entry = prop_find(name);

    if (entry == NULL)
        return 0;
    return g_prop_state[entry->state_index];
}

static uint32_t adapter_mask(void)
{
    uint32_t (*mask_fn)(void *);

    if (g_adapter_mask_fn == NULL)
        return 0;

    mask_fn = g_adapter_mask_fn;
    return mask_fn(g_adapter_ctx);
}

static uint64_t mono_now_ms(void)
{
    struct timespec ts;

    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
        return 0;
    return (uint64_t)ts.tv_sec * 1000 + (uint64_t)ts.tv_nsec / 1000000;
}

int nexus_evasion_prediction_snapshot_v1(int32_t *out)
{
    uint32_t mask;
    uint32_t gate_flags;
    int32_t port_bits;
    int32_t status;

    if (out == NULL)
        return 0;
    if (out[0] != SNAPSHOT_VERSION || out[1] != PRED_SNAPSHOT_SIZE)
        return 0;

    handler_lock_acquire();
    handler_state_refresh();

    port_bits = (int32_t)port_bitmap();
    mask = adapter_mask();
    status = g_state_status;
    gate_flags = (uint32_t)(g_triple_gate_a != 0)
               | (uint32_t)(g_triple_gate_b != 0) << 1
               | (uint32_t)(g_triple_gate_c != 0) << 2;

    *(uint64_t *)(void *)out = ROD_PRED_HEADER;
    *(uint64_t *)(void *)(out + 2) = ROD_PRED_PROVIDER;
    *(uint64_t *)(void *)(out + 4) = g_state_epoch;
    out[6] = (int32_t)mask;
    out[7] = status;
    out[8] = snapshot_prop("killauraEnabled");
    out[9] = snapshot_prop("killauraMainAttack");
    out[10] = snapshot_prop("aopPredictEnabled");
    out[11] = snapshot_prop("aopTargetMode");
    out[12] = snapshot_prop("combatAuraRange");
    out[13] = snapshot_prop("combatFireInterval");
    out[14] = snapshot_prop("killauraDisableBelowHealthPercent");
    out[15] = snapshot_prop("holdToShootEnabled");
    out[16] = snapshot_prop("autofarmEnabled");
    out[17] = snapshot_prop("xrayEnabled");
    out[18] = snapshot_prop("coltModEnabled");
    out[20] = snapshot_prop("aopPredictVersion");
    out[21] = snapshot_prop("aopProjectileSpeed");
    out[22] = snapshot_prop("aopLeadScale");
    out[23] = snapshot_prop("aopReactionMs");
    out[24] = snapshot_prop("aopAimEnabled");
    out[25] = snapshot_prop("autododgeEnabled");
    out[26] = (int32_t)gate_flags;
    out[27] = port_bits;
    *(uint64_t *)(void *)(out + 28) = g_port_gate_epoch;
    *(uint64_t *)(void *)(out + 30) = g_state_epoch2;
    *(uint64_t *)(void *)(out + 32) = g_port_gate_serial;

    handler_lock_release();
    return 1;
}

int nexus_evasion_hold_snapshot_v1(int32_t *out)
{
    int gate_a;
    int gate_b;
    int32_t status;

    if (out == NULL)
        return 0;
    if (out[0] != SNAPSHOT_VERSION || out[1] != HOLD_SNAPSHOT_SIZE)
        return 0;

    handler_lock_acquire();
    handler_state_refresh();

    gate_a = holdfire_gate_a();
    gate_b = holdfire_gate_b(0);
    status = g_state_status;

    *(uint64_t *)(void *)(out + 2) = g_state_epoch;
    out[12] = (int32_t)(g_holdfire_epoch != 0);
    out[13] = gate_a;
    out[14] = status;
    out[15] = gate_b;
    out[16] = snapshot_prop("holdToShootEnabled");
    out[17] = snapshot_prop("holdToShootAim");
    out[18] = snapshot_prop("holdToShootRangeCheck");
    out[19] = snapshot_prop("holdToShootDelayMs");
    out[20] = status;
    out[21] = snapshot_prop("aopAimEnabled");
    out[22] = snapshot_prop("aopPredictEnabled");
    out[23] = snapshot_prop("aopPredictVersion");
    out[24] = snapshot_prop("aopAimTargetMode");
    out[25] = snapshot_prop("aopTargetMode");
    out[26] = snapshot_prop("aopMaxRange");
    out[27] = snapshot_prop("aopProjectileSpeed");
    out[28] = snapshot_prop("aopLeadScale");
    out[29] = snapshot_prop("aopReactionMs");
    out[30] = snapshot_prop("killauraEnabled");
    out[31] = snapshot_prop("killauraMainAttack");
    out[32] = snapshot_prop("killauraSuper");
    out[33] = snapshot_prop("killauraGadget");
    out[34] = snapshot_prop("killauraNoWall");
    out[35] = snapshot_prop("killauraNoBall");
    out[36] = snapshot_prop("combatAuraRange");
    out[37] = snapshot_prop("combatFireInterval");
    out[38] = snapshot_prop("killauraDisableBelowHealthPercent");
    out[39] = snapshot_prop("autofarmEnabled");
    out[40] = snapshot_prop("xrayEnabled");
    out[41] = snapshot_prop("coltModEnabled");
    out[42] = snapshot_prop("dynaJumpEnabled");
    out[43] = snapshot_prop("boltModEnabled");
    out[44] = snapshot_prop("ballAssistEnabled");
    out[45] = snapshot_prop("followEnabled");
    out[46] = g_prop_state[0];
    out[47] = 0;
    *(uint64_t *)(void *)(out + 4) = g_holdfire_epoch;
    *(uint64_t *)(void *)(out + 6) = g_holdfire_aux;
    *(uint64_t *)(void *)(out + 8) = g_holdfire_aux2;
    *(uint64_t *)(void *)(out + 10) = g_holdfire_aux3;
    *(uint64_t *)(void *)out = ROD_HOLD_HEADER;

    handler_lock_release();
    return 1;
}

int nexus_evasion_aura_snapshot_v1(int32_t *out)
{
    uint32_t mask;
    int32_t status;

    if (out == NULL)
        return 0;
    if (out[0] != SNAPSHOT_VERSION || out[1] != AURA_SNAPSHOT_SIZE)
        return 0;

    handler_lock_acquire();
    handler_state_refresh();

    mask = adapter_mask();
    status = g_state_status;

    *(uint64_t *)(void *)(out + 2) = g_state_epoch;
    out[4] = (int32_t)mask;
    out[5] = status;
    out[6] = snapshot_prop("killauraEnabled");
    out[7] = snapshot_prop("killauraMainAttack");
    out[8] = snapshot_prop("aopPredictEnabled");
    out[9] = snapshot_prop("aopTargetMode");
    out[10] = snapshot_prop("combatAuraRange");
    out[11] = snapshot_prop("combatFireInterval");
    out[12] = snapshot_prop("killauraDisableBelowHealthPercent");
    out[13] = snapshot_prop("holdToShootEnabled");
    out[14] = snapshot_prop("autofarmEnabled");
    out[15] = snapshot_prop("xrayEnabled");
    out[16] = snapshot_prop("coltModEnabled");
    *(uint64_t *)(void *)out = ROD_AURA_HEADER;

    handler_lock_release();
    return 1;
}

int nexus_evasion_runtime_aura_route_v1(const int32_t *route)
{
    uint64_t prev;
    int result;

    if (!(route == NULL
          || (route[0] == SNAPSHOT_VERSION && route[1] == AURA_ROUTE_SIZE
              && *(const uint64_t *)(const void *)(route + 2) != 0)))
        return 0;
    if (g_aura_adapter_ready != 1)
        return 0;

    prev = __atomic_exchange_n(&g_aura_route_lock, 1ull, __ATOMIC_ACQ_REL);
    if (prev == 0) {
        if (route == NULL) {
            g_aura_consumer_fn = NULL;
        } else {
            g_aura_consumer_fn = (int (*)(uint64_t, void *))
                *(const uint64_t *)(const void *)(route + 2);
        }
        result = family_callback_register(&g_frame_family_table,
                                          aura_frame_callback);
        g_aura_route_lock = 0;
        return result;
    }
    return 0;
}

static int aura_frame_callback(void *self, void *frame)
{
    aura_frame_t *f = frame;
    aura_frame_record_t rec;
    char buf[704];
    uint64_t now_ms;
    int active_known = 0;
    int phase_eligible;

    (void)self;

    memset(&rec, 0, sizeof rec);
    rec.header = ROD_FRAME_HEADER;
    memset(&g_aura_frame, 0, sizeof g_aura_frame);

    if (g_frame_parse_enabled == 1) {
        if (frame_record_parse(&g_frame_record_table, frame, &rec) != 0
            && rec.active_known != 0) {
            active_known = 1;
            g_aura_frame = rec;
        }
    }

    now_ms = mono_now_ms();
    phase_eligible = (int)g_aura_frame.active_eligible;

    if (g_aura_dedup_epoch == f->epoch
        && g_aura_dedup_known == (uint64_t)active_known
        && g_aura_dedup_eligible == g_aura_frame.active_eligible
        && g_aura_dedup_ended == g_aura_frame.active_ended) {
        phase_eligible = 0;
        if (rec.active_ended == 1)
            phase_eligible = active_known;
        if (phase_eligible != 1 || now_ms < g_aura_dedup_ms
            || now_ms - g_aura_dedup_ms < 2000)
            goto dispatch;
    }

    snprintf(buf, 700,
             ",\"epoch\":%llu,\"sequence\":%llu,\"screen\":\"0x%llx\","
             "\"client\":\"0x%llx\",\"own_gid\":%d,\"active_known\":%d,"
             "\"active_eligible\":%u,\"active_ended\":%u,"
             "\"native_wrapper_calls\":%llu,\"calls_while_ended\":%llu,"
             "\"last_call_mono_ms\":%llu",
             (unsigned long long)f->epoch,
             (unsigned long long)f->sequence,
             (unsigned long long)f->screen,
             (unsigned long long)f->client,
             (int)f->own_gid,
             active_known,
             (unsigned)phase_eligible,
             g_aura_frame.active_ended,
             (unsigned long long)g_aura_wrapper_calls,
             (unsigned long long)g_aura_calls_ended,
             (unsigned long long)g_aura_last_call_ms);
    runtime_journal_write("aura_phase", "typed_current_phase_and_call_counters",
                          buf);

    g_aura_dedup_epoch = f->epoch;
    g_aura_dedup_eligible = g_aura_frame.active_eligible;
    g_aura_dedup_ended = g_aura_frame.active_ended;
    g_aura_dedup_known = (uint64_t)active_known;
    g_aura_dedup_ms = now_ms;

dispatch:
    if (active_known == 0)
        return -1;
    if (rec.active_eligible == 0)
        return 0;
    if (g_aura_consumer_fn == NULL)
        return 1;
    return g_aura_consumer_fn(0, frame);
}

static void aura_state_logger(void *self, const aura_state_record_t *state,
                              int candidates, uint32_t ready_mask)
{
    aura_weapon_record_t weapon;
    char buf[1200];
    uint64_t now_ms;
    const char *policy;

    (void)self;

    now_ms = mono_now_ms();

    if (state->wrapper_called != 0) {
        g_aura_wrapper_calls++;
        g_aura_last_call_ms = now_ms;
        if (g_aura_frame.active_known != 0 && g_aura_frame.active_ended != 0)
            g_aura_calls_ended++;
    }

    if (state->aura_reason != g_aura_state_reason || g_aura_state_ms == 0
        || now_ms < g_aura_state_ms || now_ms - g_aura_state_ms > 999) {
        memset(&weapon, 0, sizeof weapon);
        g_aura_state_reason = state->aura_reason;
        g_aura_state_ms = now_ms;
        family_record_read(&g_frame_family_table, &weapon);

        policy = "bounded_primary_policy";
        if (state->wrapper_called != 0)
            policy = "native_wrapper_invoked";

        snprintf(buf, sizeof buf,
                 ",\"aura_reason\":%u,\"ready_mask\":%u,"
                 "\"settings_generation\":%llu,"
                 "\"frame_execution_evidence_ready\":%u,\"wrapper_called\":%u,"
                 "\"wrapper_raw_result\":%d,\"candidates\":%u,"
                 "\"worker_target\":\"0x%llx\",\"final_target\":\"0x%llx\","
                 "\"raw_x\":%d,\"raw_y\":%d,\"damage_proven\":false,"
                 "\"weapon_known\":%u,\"weapon_name\":\"%s\","
                 "\"weapon_radius_raw\":%u,\"weapon_bypass\":%u,"
                 "\"weapon_variants\":%u,\"weapon_scope\":\"raw_context\","
                 "\"active_known\":%u,\"active_eligible\":%u,"
                 "\"active_route\":%d,\"active_ended\":%u",
                 (unsigned)state->aura_reason,
                 (unsigned)ready_mask,
                 (unsigned long long)state->settings_generation,
                 (unsigned)state->evidence_ready,
                 (unsigned)state->wrapper_called,
                 (int)state->wrapper_raw_result,
                 (unsigned)candidates,
                 (unsigned long long)state->worker_target,
                 (unsigned long long)state->final_target,
                 (int)state->raw_x,
                 (int)state->raw_y,
                 (unsigned)weapon.weapon_known,
                 weapon.weapon_name,
                 (unsigned)weapon.weapon_radius_raw,
                 (unsigned)weapon.weapon_bypass,
                 (unsigned)weapon.weapon_variants,
                 (unsigned)g_aura_frame.active_known,
                 (unsigned)g_aura_frame.active_eligible,
                 (int)g_aura_frame.active_route,
                 (unsigned)g_aura_frame.active_ended);
        runtime_journal_write("aura_state", policy, buf);
    }
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

static int aura_adapter_bootstrap(void)
{
    image_bind_record_t image_rec;
    frame_record_bind_t frame_rec;
    frame_family_record_t family_rec;
    uint64_t family_tmpl[3];
    int ok = 0;

    g_register_handlers_fn = (void (*)(void *))dlsym(
        g_evasion_lib_handle, "nexus_evasion_register_handlers_v1");
    g_attempt_stamp_fn = (int (*)(void *))dlsym(
        g_evasion_lib_handle, "nexus_evasion_attempt_stamp_v1");
    g_attempt_recheck_fn = (int (*)(void *))dlsym(
        g_evasion_lib_handle, "nexus_evasion_attempt_recheck_v1");

    if (!sym_verified((void *)(uintptr_t)g_register_handlers_fn)
        || !sym_verified((void *)(uintptr_t)g_attempt_stamp_fn)
        || !sym_verified((void *)(uintptr_t)g_attempt_recheck_fn))
        return 0;

    g_aura_snapshot_fn = dlsym(g_evasion_lib_handle,
                               "nexus_evasion_aura_snapshot_v1");
    g_register_frame_family_fn = (int (*)(const void *))dlsym(
        g_evasion_lib_handle, "nexus_evasion_register_frame_family_v1");
    g_bind_image_fn = (int (*)(const void *))dlsym(
        g_evasion_lib_handle, "nexus_evasion_bind_image_v1");
    g_publish_observer_fn = dlsym(g_evasion_lib_handle,
                                  "nexus_evasion_publish_observer_v1");

    if (!sym_verified(g_aura_snapshot_fn)
        || !sym_verified((void *)(uintptr_t)g_register_frame_family_fn)
        || !sym_verified((void *)(uintptr_t)g_bind_image_fn)
        || !sym_verified(g_publish_observer_fn))
        return 0;

    if (frame_family_capacity() < 0x10001) {
        image_rec.header = ROD_IMAGE_TAG;
        image_rec.engine = (uintptr_t)g_engine_base;
        image_rec.game_sha = GAME_SHA;
        image_rec.backend_sha = BACKEND_SHA;
        image_rec.read_fn = (void *)(uintptr_t)game_read;
        image_rec.reserved = NULL;

        if (g_bind_image_fn(&image_rec) != 0) {
            if (frame_record_capacity() < 0x101) {
                frame_rec.header = ROD_IMAGE_TAG;
                frame_rec.engine = (uintptr_t)g_engine_base;
                frame_rec.game_sha = GAME_SHA;
                frame_rec.backend_sha = BACKEND_SHA;
                frame_rec.read_ctx = NULL;
                frame_rec.read_fn = (void *)(uintptr_t)game_read;

                if (frame_record_install(&g_frame_record_table, 0x100,
                                         &frame_rec) != 0) {
                    g_frame_parse_enabled = 1;

                    family_rec.header = ROD_FAMILY_TAG;
                    family_rec.engine = (uintptr_t)g_engine_base;
                    family_rec.game_sha = GAME_SHA;
                    family_rec.backend_sha = BACKEND_SHA;
                    family_rec.read_ctx = NULL;
                    family_rec.read_fn = (void *)(uintptr_t)game_read;
                    family_rec.cb[0] = (void *)(uintptr_t)frame_family_cb_a;
                    family_rec.cb[1] = g_aura_snapshot_fn;
                    family_rec.cb[2] = (void *)(uintptr_t)aura_frame_callback;
                    family_rec.cb[3] = (void *)(uintptr_t)aura_state_logger;
                    family_rec.cb[4] = (void *)(uintptr_t)frame_tick_stamp;
                    family_rec.cb[5] = (void *)(uintptr_t)attempt_stamp_bridge;
                    family_rec.cb[6] = (void *)(uintptr_t)attempt_recheck_bridge;
                    family_rec.cb[7] = (void *)(uintptr_t)frame_family_cb_g;

                    if (frame_family_install(&g_frame_family_table, 0x10000,
                                             &family_rec) != 0) {
                        memcpy(family_tmpl, ROD_FAMILY_TMPL,
                               sizeof family_tmpl);
                        if (g_register_frame_family_fn(family_tmpl) != 0) {
                            ok = 1;
                            g_aura_adapter_ready = 1;
                        }
                    }
                }
            }
        }
    }
    return ok;
}

#ifdef NEXUS_WITH_JNI
#include <jni.h>

extern void runtime_fatal_report(JavaVM *vm, const char *reason,
                                 const char *detail);
extern void handlers_install_main(JNIEnv *env, jclass loader_cls, int acked);

static uint64_t proc_start_time_ticks(void)
{
    char buf[2048];
    char *p;
    char *tok;
    char *save;
    ssize_t n;
    int fd;
    int i;

    fd = open("/proc/self/stat", O_RDONLY | O_CLOEXEC);
    if (fd < 0)
        return 0;

    memset(buf, 0, sizeof buf);
    n = read(fd, buf, sizeof buf - 1);
    close(fd);
    if (n < 1)
        return 0;

    p = strrchr(buf, ')');
    if (p == NULL || p[1] != ' ')
        return 0;

    save = NULL;
    tok = strtok_r(p + 2, " ", &save);
    for (i = 0; i < 19; i++) {
        if (tok == NULL)
            return 0;
        tok = strtok_r(NULL, " ", &save);
    }
    if (tok == NULL)
        return 0;
    return strtoull(tok, NULL, 10);
}

static int32_t cur_tid(void)
{
    return (int32_t)syscall(SYS_gettid);
}

static void runtime_teardown(JNIEnv *env, const char *reason)
{
    uint32_t probe;

    if (g_init_thread_id != 0 && cur_tid() == g_init_thread_id) {
        probe = 0xffffffffu;
        if (frame_stats_block_current(g_frame_stats, &probe) != 0
            && (int32_t)g_frame_stats[27] <= (int32_t)probe)
            goto periodic;
        g_frame_stats_gen = 0;
        memset(g_frame_stats, 0, sizeof g_frame_stats);
    }

periodic:
    if ((__atomic_exchange_n(&g_periodic_stop_once, 1ull, __ATOMIC_ACQ_REL)
         & 1) == 0) {
        visual_tweaks_restore();
        if (g_periodic_worker_run != 0) {
            g_periodic_worker_flag = 0;
            g_periodic_generation++;
            periodic_worker_join();
        }
        g_periodic_worker_run = 0;
        g_periodic_worker_aux = 0;
        g_periodic_stop_once = 0;
    }

    g_cache_state_a = 0;
    g_cache_state_b = 0;
    if ((__atomic_exchange_n(&g_cache_lock, 1ull, __ATOMIC_ACQ_REL) & 1) == 0) {
        memset(g_telemetry_cache, 0, sizeof g_telemetry_cache);
        g_cache_flag = 0;
        g_cache_lock = 0;
    }

    memset(g_gameplay_stats, 0, sizeof g_gameplay_stats);
    g_aura_guard = 0;
    g_pair_channel_fd = -1;
    observer_state_reset(0, 0, 0xffffffffu);
    gameplay_state_reset(0, 0);
    g_visual_counter_a = 0;
    g_visual_counter_b = 0;
    g_visual_flag = 0;

    if (g_teardown_fn_a != NULL)
        g_teardown_fn_a(NULL);
    g_gl_state_a = 0;

    if (g_teardown_fn_b != NULL)
        g_teardown_fn_b(NULL);
    g_gl_state_b = 0;
    g_gl_state_c = 0;

    nexus_visual_gl_disable_v1();

    if (g_teardown_fn_c != NULL)
        g_teardown_fn_c(NULL);
    g_visual_flag2 = 0;

    g_teardown_gen_a = (g_teardown_gen_a & 0xffffffff00000000ull)
                     + 0x100000000ull;
    if (g_teardown_armed_d == 1)
        g_teardown_fn_d(NULL);
    g_spin_state_flag = 0;
    memset(g_mod_state_block, 0, sizeof g_mod_state_block);

    g_teardown_gen_b = (g_teardown_gen_b & 0xffffffff00000000ull)
                     + 0x100000000ull;
    if (g_spin_armed == 1)
        g_spin_register_fn(NULL);

    if (g_periodic_armed == 1)
        g_periodic_counter++;
    g_periodic_armed = 0;
    g_periodic_field_a = 0;
    g_periodic_field_b = 0;
    g_periodic_field_c = 0;
    g_periodic_field_d = 0;
    g_periodic_field_e = 0;
    g_periodic_field_f = 0;
    memset(g_spin_telemetry, 0, sizeof g_spin_telemetry);
    g_periodic_saved = g_periodic_counter;

    if (((g_loader_verify_gate_a & 1) != 0 || g_loader_verify_gate_b != 0)
        && game_thread_present() != 0)
        (void)loader_identity_verify();

    g_visual_state_e = 0;
    g_teardown_gen_c = (g_teardown_gen_c & 0xffffffff00000000ull)
                     + 0x100000000ull;
    if (g_teardown_armed_e == 1)
        g_teardown_fn_e(NULL);

    if (g_event_routes_armed == 1 && game_thread_present() != 0) {
        int rr = event_route_restore(g_event_routes_state);

        if (rr == 0) {
            g_event_routes_armed = 0;
        } else {
            if (rr == -1) {
                runtime_journal_write("fatal", "event_route_restore_unverified",
                                      NULL);
                abort();
            }
            runtime_journal_write("event_route_restore_pending",
                                  "resident_disabled_owner", NULL);
        }
    }

    g_teardown_gen_d = (g_teardown_gen_d & 0xffffffff00000000ull)
                     + 0x100000000ull;
    if (g_register_handlers_fn != NULL)
        g_register_handlers_fn(NULL);

    if (env != NULL && (*env)->ExceptionCheck(env))
        (*env)->ExceptionClear(env);

    runtime_journal_write("disabled", reason, NULL);
}

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    JNIEnv *env = NULL;
    jclass loader_cls = NULL;
    jclass ctx_cls = NULL;
    jclass fd_cls = NULL;
    jobject context = NULL;
    jobject files_dir = NULL;
    jstring pkg = NULL;
    jstring cpath = NULL;
    jmethodID app_ctx_mid = NULL;
    jmethodID pkg_mid = NULL;
    jmethodID files_mid = NULL;
    jmethodID cpath_mid = NULL;
    const char *pkg_utf = NULL;
    const char *cpath_utf = NULL;
    struct stat sb;
    char ctx_buf[200];
    const char *journal_open;
    const char *reason;
    ng_bind_record_t ng_rec;
    phdr_game_match_t match;
    uint64_t ng_tmpl[4];
    int files_fd;
    int pkg_ok;
    long page;

    (void)reserved;

    g_java_vm = vm;
    g_engine_epoch = proc_start_time_ticks();
    runtime_journal_write("initialize_enter", "readonly_observer", NULL);

    reason = "jni_environment";

    if (vm == NULL)
        goto fail;

    g_runtime_init_reason = "verified_loader_context";
    reason = g_runtime_init_reason;

    if ((*vm)->GetEnv(vm, (void **)&env, JNI_VERSION_1_6) != JNI_OK
        || env == NULL)
        goto fail;

    loader_cls = (*env)->FindClass(env, "nexus/loader/NexusLoader");
    if (loader_cls == NULL)
        goto fail;
    if ((*env)->ExceptionCheck(env))
        goto fail;

    app_ctx_mid = (*env)->GetStaticMethodID(env, loader_cls,
                                            "applicationContext",
                                            "()Landroid/content/Context;");
    if (app_ctx_mid == NULL)
        goto fail;
    if ((*env)->ExceptionCheck(env))
        goto fail;

    context = (*env)->CallStaticObjectMethod(env, loader_cls, app_ctx_mid);
    if (context == NULL)
        goto fail;
    if ((*env)->ExceptionCheck(env))
        goto fail;

    ctx_cls = (*env)->GetObjectClass(env, context);
    if (ctx_cls == NULL)
        goto fail;
    if ((*env)->ExceptionCheck(env))
        goto fail;

    pkg_mid = (*env)->GetMethodID(env, ctx_cls, "getPackageName",
                                  "()Ljava/lang/String;");
    if (pkg_mid == NULL)
        goto fail;
    if ((*env)->ExceptionCheck(env))
        goto fail;

    pkg = (*env)->CallObjectMethod(env, context, pkg_mid);
    if (pkg == NULL)
        goto fail;
    if ((*env)->ExceptionCheck(env))
        goto fail;

    pkg_utf = (*env)->GetStringUTFChars(env, pkg, NULL);
    if (pkg_utf == NULL)
        goto fail;
    pkg_ok = strcmp(pkg_utf, "bsd.suitcase.nexusv2") == 0;
    (*env)->ReleaseStringUTFChars(env, pkg, pkg_utf);
    if (!pkg_ok)
        goto fail;

    files_mid = (*env)->GetMethodID(env, ctx_cls, "getFilesDir",
                                    "()Ljava/io/File;");
    if (files_mid == NULL)
        goto fail;
    if ((*env)->ExceptionCheck(env))
        goto fail;

    files_dir = (*env)->CallObjectMethod(env, context, files_mid);
    if (files_dir == NULL)
        goto fail;
    if ((*env)->ExceptionCheck(env))
        goto fail;

    fd_cls = (*env)->GetObjectClass(env, files_dir);
    if (fd_cls == NULL)
        goto fail;
    if ((*env)->ExceptionCheck(env))
        goto fail;

    cpath_mid = (*env)->GetMethodID(fd_cls, "getCanonicalPath",
                                    "()Ljava/lang/String;");
    if (cpath_mid == NULL)
        goto fail;
    if ((*env)->ExceptionCheck(env))
        goto fail;

    cpath = (*env)->CallObjectMethod(env, files_dir, cpath_mid);
    if (cpath == NULL)
        goto fail;
    if ((*env)->ExceptionCheck(env))
        goto fail;

    cpath_utf = (*env)->GetStringUTFChars(env, cpath, NULL);
    if (cpath_utf == NULL)
        goto fail;

    g_runtime_init_reason = "storage_private_files";
    reason = g_runtime_init_reason;

    files_fd = files_dir_open(cpath_utf);
    (*env)->ReleaseStringUTFChars(env, cpath, cpath_utf);

    if (files_fd < 0) {
        close(files_fd);
        goto fail;
    }

    if (fstat(files_fd, &sb) != 0 || !S_ISDIR(sb.st_mode)
        || sb.st_uid != getuid())
        goto fail;

    snprintf(g_journal_name, sizeof g_journal_name,
             "nexus-evasion-game-%d-%llu.jsonl",
             (int)getpid(), (unsigned long long)g_engine_epoch);
    g_journal_fd = openat(files_fd, g_journal_name,
                          O_WRONLY | O_CREAT | O_EXCL | O_LARGEFILE
                          | O_CLOEXEC, 0600);
    journal_open = "false";
    if (g_journal_fd >= 0) {
        if (fstat(g_journal_fd, &sb) != 0 || !S_ISREG(sb.st_mode)
            || sb.st_uid != getuid() || sb.st_nlink != 1) {
            close(g_journal_fd);
            g_journal_fd = -1;
        } else {
            journal_open = "true";
        }
    }

    g_files_dir_fd = files_fd;
    snprintf(ctx_buf, sizeof ctx_buf,
             ",\"journal\":\"%s\",\"journal_open\":%s",
             g_journal_name, journal_open);
    runtime_journal_write("context_ready", "actual_lab_context", ctx_buf);

    if (game_thread_present() == 0)
        goto fail;

    match.word = ROD_PHDR_EXPECTED;
    match.count = 0;
    dl_iterate_phdr(phdr_find_game_lib, &match);
    g_paired_backend_base = (int32_t)(match.word >> 32);
    if ((int32_t)match.count != 1)
        g_paired_backend_base = -1;

    if (g_paired_backend_base < 0) {
        reason = "process_memory";
        goto fail;
    }

    page = sysconf(_SC_PAGESIZE);
    if (!(page == 0x4000 || page == 0x1000)) {
        reason = "page_size";
        goto fail;
    }

    g_page_size = (uint64_t)page;
    dl_iterate_phdr(phdr_count_game_libs, NULL);
    if (!(g_mapped_game_count == 1 && g_engine_base != 0)) {
        reason = "mapped_game_count";
        goto fail;
    }

    if (g_mapped_paired_count != 1) {
        reason = "mapped_paired_backend_count";
        goto fail;
    }

    {
        int build_ok = 0;

        dl_iterate_phdr(phdr_check_build_id, &build_ok);
        if (build_ok == 0) {
            reason = "mapped_game_BuildID";
            goto fail;
        }
    }

    if (game_image_sha_verify(g_game_sha_buf, 0x1340c90,
                              ROD_SHA_TEMPLATE) == 0) {
        reason = "mapped_game_file_SHA";
        goto fail;
    }

    if (evasion_backend_bind() == 0) {
        reason = "paired_backend_NOLOAD_exports";
        goto fail;
    }

    memset(&ng_rec, 0, sizeof ng_rec);
    ng_rec.header = ROD_NG_BIND_TAG;
    ng_rec.engine = (uintptr_t)g_engine_base;
    ng_rec.game_sha = GAME_SHA;
    ng_rec.backend_sha = BACKEND_SHA;
    ng_rec.read_fn = (void *)(uintptr_t)game_read;
    ng_rec.reserved = NULL;
    ng_rec.get_requested_fn = (void *)(uintptr_t)g_get_requested_fn;
    ng_rec.dodge_profile_fn = (void *)(uintptr_t)g_dodge_profile_fn;

    if (ng_bind_v1(&ng_rec) == 0) {
        reason = "a10_passive_code_or_vtable_guards";
        goto fail;
    }

    if (aura_adapter_bootstrap() == 0) {
        reason = "aura_adapter_guards_or_activation_exports";
        goto fail;
    }

    {
        int routes = function_routes_bind();
        int cell;

        *(uint32_t *)status_cell(&g_status_cell_a) = 0;
        if (routes == 0) {
            cell = *(int *)status_cell(&g_status_cell_b);
            reason = "function_routes_preparation";
            if (cell != 0)
                reason = (const char *)status_cell(&g_status_cell_c);
            goto fail;
        }
    }

    spin_exports_bind();
    runtime_journal_write("backend_bound",
                          "a10_and_paired_ack_backend_verified",
                          BACKEND_BOUND_PAYLOAD);
    function_routes_publish();

    g_entry_write_ctx = (uint64_t)(uintptr_t)entry_write_prepare(
        (void *)(uintptr_t)(g_engine_base + ENGINE_OBSERVER_RVA));
    if (g_entry_write_ctx == 0) {
        reason = entry_prep_fail_report();
        goto fail;
    }

    reason = g_runtime_init_reason;
    if (game_thread_present() == 0)
        goto fail;

    memcpy(ng_tmpl, ROD_NG_ENTRY_TMPL, sizeof ng_tmpl);
    g_entry_write_result = ng_entry_write(
        (void *)(uintptr_t)(g_engine_base + ENGINE_OBSERVER_RVA),
        (void *)(uintptr_t)g_ng_entry_state, ng_tmpl);

    if (g_entry_write_result < 0) {
        runtime_journal_write("fatal", "entry_write_and_restore_unverified",
                              NULL);
        runtime_fatal_report(vm, "entry_restore_unverified",
                             "native_link_failed");
        abort();
    }

    if (g_entry_write_result != 1) {
        reason = "entry_publication_failed_original_restored";
        goto fail;
    }

    reason = "owned_observer_postpublication_proof";
    if (owned_observer_proof() == 0)
        goto fail;

    runtime_journal_write("observer_installed", "one_preserving_preobserver",
                          NULL);

    {
        jmethodID native_loaded_mid;
        jstring arg;
        jboolean ret;
        const char *cb_stage;
        uint32_t ack_arg;

        native_loaded_mid = (*env)->GetStaticMethodID(env, loader_cls,
                                                       "NativeLoaded",
                                                       "(Ljava/lang/String;)Z");
        arg = NULL;
        cb_stage = "callback_lookup_or_argument";
        if (native_loaded_mid != NULL)
            arg = (*env)->NewStringUTF(env, "EvasionGame");
        if (arg != NULL && !(*env)->ExceptionCheck(env)) {
            ret = (*env)->CallStaticBooleanMethod(env, loader_cls,
                                                  native_loaded_mid, arg);
            cb_stage = "callback_invocation_or_rejected";
            g_evasion_game_acked = (uint32_t)(ret == JNI_TRUE);
        }

        if ((*env)->ExceptionCheck(env)) {
            (*env)->ExceptionClear(env);
            g_evasion_game_acked = 0;
            runtime_fatal_report(vm, cb_stage,
                                 "required_synchronous_callback_absent");
        } else if (g_evasion_game_acked == 0) {
            runtime_fatal_report(vm, cb_stage,
                                 "required_synchronous_callback_absent");
        }

        family_active_set(&g_frame_family_table, g_evasion_game_acked != 0);

        ack_arg = 0;
        if (g_evasion_game_acked != 0) {
            runtime_journal_write(
                "aura_policy", "signed_local_free_feature_test",
                ",\"plus_entitlement_used\":false,\"paid_state_changed\":false");
            ack_arg = g_evasion_game_acked;
        }

        if (persistent_handlers_register((int)ack_arg) == 0) {
            reason = "persistent_handler_registration";
            goto fail;
        }

        runtime_journal_write(
            "handler_registered",
            "implemented_main_free_scope_after_actual_loader_callback",
            ",\"implemented_handlers\":7,\"prediction_handler_registered\":true,\"lobby_ack_allowed\":true");

        if (owned_handlers_proof((int)g_evasion_game_acked) == 0) {
            reason = "functions_owned_proof_and_handlers";
            goto fail;
        }

        handlers_install_main(env, loader_cls, (int)g_evasion_game_acked);
        spin_handler_install((int)g_evasion_game_acked);
        handler_install_b((int)g_evasion_game_acked);
        handler_install_d((int)g_evasion_game_acked);
        aura_phase_publisher_install();
        handler_install_f((int)g_evasion_game_acked);
        trophies_handler_install((int)g_evasion_game_acked);

        g_game_callback_acked = g_evasion_game_acked;
        runtime_journal_write(
            "callback_return",
            g_evasion_game_acked != 0 ? "EvasionGame_acknowledged"
                                      : "EvasionGame_rejected",
            NULL);
    }

    return JNI_VERSION_1_6;

fail:
    runtime_teardown(env, reason);
    runtime_fatal_report(
        vm, reason,
        g_evasion_game_acked != 0 ? "startup_failure"
                                  : "required_synchronous_callback_absent");
    return JNI_VERSION_1_6;
}
#endif

