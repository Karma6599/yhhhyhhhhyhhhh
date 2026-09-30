#define _GNU_SOURCE

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h>
#include <dlfcn.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>

#define PROP_COUNT        142
#define PROFILE_SLOT_COUNT 6
#define QUERY_SLOT_COUNT  0x76
#define DODGE_KEY_COUNT   0xa8
#define LEASE_QWORDS      11

#define ROD_SNAPSHOT_HDR   (*(const uint64_t *)(uintptr_t)0x1047d0)
#define ROD_LEASE_HDR      (*(const uint64_t *)(uintptr_t)0x1047a8)
#define ROD_LEASE_HDR2     (*(const uint64_t *)(uintptr_t)0x1047b0)
#define ROD_JOURNAL_EMPTY  ((const char *)(uintptr_t)0x117c20)
#define ROD_JOURNAL_COMMA  ((const char *)(uintptr_t)0x120242)
#define ROD_NG_FEED_TAG    (*(const uint64_t *)(uintptr_t)0x10e668)
#define ROD_NG_STATUS_TAG  (*(const uint64_t *)(uintptr_t)0x10e808)
#define ROD_FRAME_CB_TAG   (*(const uint64_t *)(uintptr_t)0x10e580)
#define ROD_SPIN_STAMP     (*(const uint64_t *)(uintptr_t)0x10e670)
#define ROD_AURA_STAMP     (*(const uint64_t *)(uintptr_t)0x10e7a8)
#define ROD_REGISTRY_TAG   (*(const uint64_t *)(uintptr_t)0x10e7f8)
#define ROD_FRAME_TAG      (*(const uint64_t *)(uintptr_t)0x10e740)
#define ROD_FRAME_REC_TAG  (*(const uint64_t *)(uintptr_t)0x10e800)
#define ROD_STATUS_TAG     (*(const uint64_t *)(uintptr_t)0x10e6a0)
#define ROD_CAPTURE_TAG    (*(const uint64_t *)(uintptr_t)0x10e628)
#define ROD_MOVE_TAG       (*(const uint64_t *)(uintptr_t)0x10e700)
#define ROD_INPUT_TAG      (*(const uint64_t *)(uintptr_t)0x10e780)
#define ROD_CONTEXT_TAG    (*(const uint64_t *)(uintptr_t)0x10e7d0)
#define ROD_MOVE2_TAG      (*(const uint64_t *)(uintptr_t)0x10e590)
#define ROD_SPIN_CB_TAG    (*(const uint64_t *)(uintptr_t)0x10e540)
#define ROD_UI_IGNORE_HDR  (*(const uint64_t *)(uintptr_t)0x10f858)

#define JOURNAL_MAX_LINE   0x80000
#define CAPTURE_MAX_REJECTS 8
#define PERF_WINDOW        0x78
#define PROJECTILE_SLOTS   8
#define THREAT_SLOTS       64
#define IGNORE_MAGIC       0x3149524eu
#define IGNORE_SEED        0x3b7d4c22u
#define IGNORE_SIZE        0x20u
#define IGNORE_BITS        0x6cu

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

typedef struct {
    const char *name;
    uint64_t aux;
    int32_t effective_flag;
} dodge_query_entry_t;

typedef struct {
    const char *name;
    uint64_t aux;
    int32_t effective_flag;
    uint32_t bit_select;
} dodge_key_entry_t;

extern evasion_prop_entry_t g_prop_table[PROP_COUNT];
extern dodge_query_entry_t g_dodge_query_table[QUERY_SLOT_COUNT];
extern dodge_key_entry_t g_dodge_key_table[DODGE_KEY_COUNT];
extern const char *g_dodge_profile_suffixes[PROFILE_SLOT_COUNT];
extern const char *g_projectile_names[0x13e];
extern const uint16_t g_projectile_name_bits[0x13e];

extern int32_t g_prop_state[144];
extern uint64_t g_state_revision;
extern int32_t g_replay_block;
extern volatile int g_state_lock;

extern uint64_t g_pin_epoch_pub;
extern uint64_t g_pin_epoch_aux;
extern int (*g_pin_verify_fn)(int);
extern uint64_t g_move_epoch_pub;
extern uint64_t g_move_epoch_aux;
extern int (*g_move_verify_fn)(int);
extern uint64_t g_adapter_epoch_pub;
extern uint64_t g_adapter_epoch_aux;
extern int (*g_adapter_verify_fn)(int);
extern uint64_t g_registry_committed_epoch;
extern uint64_t g_registry_committed_aux;

extern int evasion_is_free_scope(const char *name);
extern int evasion_entitlement_check(void);
extern int evasion_key_is_visual_only(const char *name);
extern int evasion_key_forces_pass(const char *name);
extern int evasion_key_uses_move_registry(const char *name);
extern int holdfire_key_active(const char *name);
extern int spin_status_query(void);
extern int evasion_brawler_bitmap_index(const char *name);
extern int evasion_brawler_family_active(const char *name, int arg);
extern int32_t evasion_port_gate_deep(const char *name);
extern uint32_t evasion_port_bitmap(void);
extern int evasion_port_bitmap_ok(uint32_t bitmap);
extern void handler_state_refresh(void);
extern int32_t evasion_status_query(void);
extern int32_t evasion_port_status(int port, int status, int flags);
extern int32_t evasion_get_requested(const char *name);
extern int32_t evasion_get_effective(const char *name);
extern int32_t evasion_get_port_state(const char *name);
extern int nexus_evasion_set_parameter(const char *name, uint32_t value);
extern const evasion_prop_entry_t *evasion_prop_entry_lookup(const char *name,
                                                             int gate);

extern uintptr_t game_read(uintptr_t handle, uintptr_t addr, void *out,
                          uint32_t len);
extern uint64_t monotonic_us(void);

static void spin_lock_acquire(volatile int *lock)
{
    while (__atomic_test_and_set(lock, __ATOMIC_ACQUIRE))
        ;
}

static void spin_lock_release(volatile int *lock)
{
    __atomic_clear(lock, __ATOMIC_RELEASE);
}

extern int __android_log_print(int prio, const char *tag, const char *fmt, ...);
extern int __android_log_write(int prio, const char *tag, const char *text);

extern int32_t g_files_dir_fd;
extern int32_t g_journal_fd;
extern uint64_t g_capture_request_id;
extern uint64_t g_journal_bytes;
extern uint32_t g_capture_samples;
extern uint32_t g_capture_interval;
extern uint32_t g_capture_accepted;
extern uint32_t g_capture_rejects;
extern uint64_t g_capture_open_ms;
extern volatile int g_capture_open_lock;
extern volatile int g_frame_lock;
extern uint64_t g_frame_sequence;
extern uint64_t g_frame_feed_us;
extern uint64_t g_frame_read_calls;
extern int32_t g_main_thread_id;
extern int ng_status_v1(void *out);
extern int ng_projectiles_v1(void *slots, int max, uint64_t *seq_out);
extern uint64_t ng_diagnostics(char *out, size_t cap);
int ng_feed_v1(int *record);

extern void dodge_tick(void *ctx);

extern void hud_pump(void *ctx);
extern void ng_frame_post(void *ctx);
extern void aim_pump(void *ctx);
extern void trophies_battle_tick(void *ctx);
extern void branding_pump(void *ctx);
extern void periodic_pump(void *ctx);
extern void chat_frame_record(void *ctx);
extern void visual_pump(void *ctx);
extern void visual_pump_b(void *ctx);
extern void stage_pump_a(void *ctx);
extern void stage_pump_b(void *ctx);
extern void stage_pump_c(void *ctx);
extern void stage_pump_d(void *ctx);
extern void stage_pump_e(void *ctx);
extern void stage_pump_f(void *ctx);
extern void stage_pump_g(void *ctx);
extern int stage_capture_tick(void *ctx);
extern int stage_capture_flush(void);
extern int capture_request_parse(const void *buf, size_t len,
                                 uint64_t *watermark, uint32_t *samples,
                                 uint32_t *interval);
extern int journal_reopen(int *fd);
extern int game_frame_lock_try(uint64_t tick);
extern void game_frame_lock_release(uint64_t tick, int seq);

extern void *g_game_consumer_ctx;
extern int (*g_game_consumer_fn)(void *, void *);
extern char g_game_consumer_armed;

extern void *g_evasion_lib_handle;
extern void *g_expected_lib_base;
extern char g_expected_lib_path[];
extern int32_t (*g_be_get_requested_fn)(const char *);
extern int32_t (*g_be_dodge_profile_fn)(uint32_t, void *);

extern uintptr_t g_game_base;

extern void *g_dodge_state_a;
extern void *g_dodge_state_b;
extern char g_dodge_bound;
extern uint64_t g_dodge_publication;
extern uint64_t g_dodge_stats_word_a;
extern uint64_t g_dodge_stats_word_b;
extern uint64_t g_projectile_gid_list[0x100];
extern uint64_t g_ignore_mask[2];
extern uint64_t g_ignore_read_ms;
extern uint32_t g_capture_seq_lo;
extern uint32_t g_capture_seq_hi;

extern uint64_t g_snapshot_aux_a;
extern uint64_t g_snapshot_aux_b;
extern uint64_t g_snapshot_aux_c;
extern void *g_lease_provider_ctx;
extern int (*g_lease_provider_fn)(void *, int, void *);
extern uint64_t g_adapter_ready_bits[10];

static int registry_pass(uint64_t epoch_pub, uint64_t epoch_aux,
                         int (*verify_fn)(int))
{
    if (epoch_pub == 0)
        return 0;
    if (verify_fn == NULL)
        return 0;
    if (verify_fn(2) != 1)
        return 0;
    if (g_registry_committed_epoch != epoch_pub)
        return 0;
    if (g_registry_committed_aux != epoch_aux)
        return 0;
    return 1;
}

static int prop_index_of(const char *name)
{
    for (int i = 0; i < PROP_COUNT; i++)
        if (strcmp(name, g_prop_table[i].name) == 0)
            return i;
    return -1;
}

static const evasion_prop_entry_t *prop_find(const char *name)
{
    int i = prop_index_of(name);
    return (i < 0) ? NULL : &g_prop_table[i];
}

static int prop_state_index_of(const char *name)
{
    const evasion_prop_entry_t *e = prop_find(name);
    return e ? e->state_index : g_prop_table[0].state_index;
}

static uint32_t rol5(uint32_t v)
{
    return (v >> 0x1b) | (v << 5);
}

#define IDX_DODGE_VERSION 135
#define IDX_SPEED_LEVEL   136
#define IDX_TARGET_MODE_A 137
#define IDX_TARGET_MODE_B 139
#define IDX_FOLLOW_FAMILY_A 41
#define IDX_FOLLOW_FAMILY_B 45

int evasion_set_value(const char *name, uint32_t value, uint32_t is_parameter)
{
    if (name == NULL)
        return -1;

    size_t len = strlen(name);
    if (len == 0 || len >= 0x60)
        return -1;

    int idx = prop_index_of(name);
    if (idx < 0)
        return -1;
    const evasion_prop_entry_t *prop = &g_prop_table[idx];
    if ((uint32_t)prop->kind != is_parameter)
        return -1;

    uint32_t v = value;
    if (strcmp(name, "combatDodgeRequireHold") == 0)
        v = 0;

    if (v != 0 || is_parameter != 0) {
        if (is_parameter == 0) {
            if (!evasion_is_free_scope(name) && !evasion_entitlement_check())
                return 2;
        } else {
            static const char *exempt[] = {
                "outlineOpacity", "outlineColorR", "outlineColorG",
                "outlineColorB", "aop", "killaura", "combatAura",
                "combatFire", "combatDodge", "dodge", "v2Dodge",
                "v3Dodge", "v4Dodge", "v5Dodge", "holdToShoot",
            };
            int exempt_ok = 0;
            for (size_t i = 0; i < sizeof exempt / sizeof *exempt; i++) {
                if (strncmp(name, exempt[i], strlen(exempt[i])) == 0) {
                    exempt_ok = 1;
                    break;
                }
            }
            if (!exempt_ok && !evasion_entitlement_check())
                return 2;
        }
    }

    spin_lock_acquire(&g_state_lock);
    handler_state_refresh();

    if (is_parameter == 0 && v != 0 && !evasion_key_is_visual_only(name)) {
        int pass;
        if (evasion_key_forces_pass(name)) {
            pass = registry_pass(g_adapter_epoch_pub, g_adapter_epoch_aux,
                                 g_adapter_verify_fn)
                   && (evasion_is_free_scope(name) ||
                       evasion_entitlement_check());
        } else if (strcmp(name, "pinEnabled") == 0 ||
                   strcmp(name, "sprayEnabled") == 0) {
            pass = registry_pass(g_pin_epoch_pub, g_pin_epoch_aux,
                                 g_pin_verify_fn)
                   && evasion_entitlement_check();
        } else if (evasion_key_uses_move_registry(name)) {
            pass = registry_pass(g_move_epoch_pub, g_move_epoch_aux,
                                 g_move_verify_fn)
                   && evasion_entitlement_check();
        } else if (strcmp(name, "speedExploitEnabled") == 0 ||
                   strcmp(name, "speedLocalMoveEnabled") == 0) {
            pass = 1;
        } else if (strcmp(name, "holdToShootEnabled") == 0 ||
                   strcmp(name, "holdToShootAim") == 0 ||
                   strcmp(name, "holdToShootRangeCheck") == 0) {
            pass = holdfire_key_active(name) != 0;
        } else if (strcmp(name, "isSpinEnabled") == 0) {
            pass = spin_status_query() != 0;
        } else {
            int bmi = evasion_brawler_bitmap_index(name);
            if (bmi < 0) {
                int32_t slot = evasion_port_gate_deep(name);
                const evasion_prop_entry_t *predict =
                    evasion_prop_entry_lookup("aopPredictEnabled", 1);
                if (predict == NULL) {
                    spin_lock_release(&g_state_lock);
                    return 2;
                }
                if ((int32_t)slot < 0) {
                    int gate = (g_prop_state[predict->state_index] != 0) ||
                               (slot == 2);
                    const evasion_prop_entry_t *self =
                        evasion_prop_entry_lookup(name, gate);
                    if (self == NULL) {
                        spin_lock_release(&g_state_lock);
                        return 2;
                    }
                    if (self->kind != 0) {
                        spin_lock_release(&g_state_lock);
                        return 2;
                    }
                    pass = (self->f24 == 0);
                } else {
                    uint32_t bitmap = evasion_port_bitmap();
                    if (((bitmap >> (slot & 0x1f)) & 1) == 0) {
                        spin_lock_release(&g_state_lock);
                        return 2;
                    }
                    pass = evasion_port_bitmap_ok(bitmap) != 0;
                }
            } else {
                pass = evasion_brawler_family_active(name, 1) != 0;
            }
        }
        if (!pass) {
            spin_lock_release(&g_state_lock);
            return 2;
        }
    }

    uint32_t coerced;
    if (is_parameter != 0) {
        uint32_t hi = ((int32_t)value < prop->vmax)
                          ? value : (uint32_t)prop->vmax;
        coerced = ((int32_t)value >= prop->vmin) ? hi : (uint32_t)prop->vmin;
    } else {
        coerced = (v != 0) ? 1u : 0u;
    }
    if (idx == IDX_DODGE_VERSION) {
        coerced = (value >= 1 && value <= 5) ? value : 3;
    } else if (idx >= IDX_TARGET_MODE_A && idx <= IDX_TARGET_MODE_B) {
        coerced = (value == 2) ? 2u : 1u;
    } else if (idx == IDX_SPEED_LEVEL) {
        coerced = 4;
    }

    if (idx >= IDX_FOLLOW_FAMILY_A && idx <= IDX_FOLLOW_FAMILY_B) {
        if (strcmp(name, "followModeEnemy") == 0 ||
            strcmp(name, "followModeTeam") == 0) {
            coerced = 1;
        } else {
            int fidx = prop_state_index_of("followModeTeam");
            if ((uint32_t)g_prop_state[fidx] != (coerced != 0)) {
                g_prop_state[fidx] = (coerced != 0);
                g_state_revision++;
            }
        }
    }

    static const char *mex_pairs[2][2] = {
        { "speedExploitEnabled", "speedLocalMoveEnabled" },
        { "speedLocalMoveEnabled", "speedExploitEnabled" },
    };
    for (int i = 0; i < 2; i++) {
        if (strcmp(name, mex_pairs[i][0]) == 0 && coerced != 0) {
            int midx = prop_state_index_of(mex_pairs[i][1]);
            if (g_prop_state[midx] != 0) {
                g_prop_state[midx] = 0;
                g_state_revision++;
            }
        }
    }

    if ((uint32_t)g_prop_state[prop->state_index] != coerced) {
        g_prop_state[prop->state_index] = coerced;
        g_state_revision++;
    }

    uint32_t result = 1;
    if (is_parameter != 0 && coerced != 0 && prop->f24 != 0)
        result = 2;
    spin_lock_release(&g_state_lock);
    return (int)result;
}

void nexus_evasion_functions_snapshot_v1(int *out)
{
    if (out == NULL)
        return;
    if (out[0] != 1 || out[1] != 0xb8)
        return;

    spin_lock_acquire(&g_state_lock);
    handler_state_refresh();

    int32_t status = evasion_status_query();
    int32_t ports[10];
    for (int i = 0; i < 10; i++)
        ports[i] = evasion_port_status(i, status, 0);

    uint32_t adapter_bits = 0;
    for (int i = 0; i < 10; i++)
        adapter_bits |= (g_adapter_ready_bits[i] != 0) ? (1u << i) : 0;

    uint32_t port_bits = 0;
    for (int i = 0; i < 10; i++)
        port_bits |= (ports[i] != 0) ? (1u << i) : 0;

    static const char *keys[] = {
        "aopAimEnabled", "aopPredictEnabled", "aopPredictVersion",
        "aopAimTargetMode", "aopTargetMode", "aopMaxRange",
        "aopProjectileSpeed", "aopLeadScale", "aopReactionMs",
        "aopAimUltimate", "aopAimGadget", "killauraEnabled",
        "killauraMainAttack", "holdToShootEnabled", "autofarmEnabled",
        "xrayEnabled", "coltModEnabled", "ballAssistEnabled",
        "boltModEnabled", "autododgeEnabled", "dodgeVersion",
        "dodgeBlacklistMask", "followEnabled", "followDistance",
        "kitNaniModEnabled", "speedExploitEnabled",
    };

    int32_t vals[26];
    for (int i = 0; i < 26; i++)
        vals[i] = g_prop_state[prop_state_index_of(keys[i])];

    uint32_t autododge_family = (vals[19] != 0)
        ? 1u
        : (uint32_t)(g_prop_state[prop_state_index_of("kitNaniModEnabled")] != 0);

    *(uint64_t *)(void *)out = ROD_SNAPSHOT_HDR;
    *(uint64_t *)(void *)(out + 2) = g_state_revision;
    *(uint64_t *)(void *)(out + 4) = g_snapshot_aux_a;
    *(uint64_t *)(void *)(out + 6) = g_snapshot_aux_b;
    out[8] = (int)adapter_bits;
    out[9] = status;
    out[10] = (int)port_bits;
    out[11] = g_replay_block;
    for (int i = 0; i < 21; i++)
        out[12 + i] = vals[i];
    out[31] = (int)autododge_family;
    out[32] = vals[20];
    for (int i = 0; i < 6; i++)
        out[33 + i] = 0;
    out[40] = vals[21];
    out[41] = vals[22];
    out[42] = vals[23];
    out[43] = vals[24];
    out[44] = vals[25];

    for (int i = 0; i < PROFILE_SLOT_COUNT; i++) {
        char key[0x40];
        if (vals[20] == 1)
            snprintf(key, sizeof key, "dodge%s", g_dodge_profile_suffixes[i]);
        else
            snprintf(key, sizeof key, "v%dDodge%s", vals[20],
                     g_dodge_profile_suffixes[i]);
        out[34 + i] = g_prop_state[prop_state_index_of(key)];
    }

    spin_lock_release(&g_state_lock);
}

static const char *lease_peer_key(int port)
{
    switch (port) {
    case 4: return "followEnabled";
    case 5: return "coltModEnabled";
    case 6: return "autofarmEnabled";
    default: return "boltModEnabled";
    }
}

void evasion_action_lease_submit(int action_id, int mode, uint64_t *out)
{
    if (out == NULL)
        return;

    int port = 2;
    int aim_mode = 0;
    if (action_id == 0x17) {
        port = 0;
        aim_mode = 1;
        if (mode != 1 && mode != 4)
            return;
    } else if (action_id == 0x1a) {
        port = 4;
    } else if (action_id == 0x2e) {
        port = 9;
    } else if (action_id == 0x44) {
        port = 6;
    } else if (action_id == 0x79) {
        port = 8;
    } else if (action_id == 0x7a) {
        port = 5;
    } else if (action_id == 0x84) {
        port = 7;
    } else if (action_id != 0x16) {
        return;
    }

    int family_needed = (port == 2) || ((port & 0xc) == 4) ||
                        ((port & 0xe) == 8);
    if ((uint32_t)(mode - 4u) <= 0xfffffffdu && family_needed)
        return;

    handler_state_refresh();
    int32_t status = evasion_status_query();
    if (g_replay_block != 0)
        return;
    if (evasion_port_status(port, status, 0) == 0)
        return;

    int32_t family_value;
    if (port == 2) {
        family_value = g_prop_state[prop_state_index_of("autododgeEnabled")];
        if (family_value == 0)
            family_value =
                g_prop_state[prop_state_index_of("kitNaniModEnabled")];
    } else {
        const char *key;
        if (aim_mode)
            key = "aopAimEnabled";
        else
            key = lease_peer_key(port);
        if (port == 8 || port == 9)
            key = (port == 8) ? "kitNaniModEnabled"
                              : "speedExploitEnabled";
        family_value = g_prop_state[prop_state_index_of(key)];
    }
    if (family_value == 0)
        return;

    if (g_lease_provider_fn == NULL)
        return;

    uint64_t rec[5] = {0};
    rec[0] = ROD_LEASE_HDR;
    if (g_lease_provider_fn(g_lease_provider_ctx, action_id, rec) != 1)
        return;
    if ((uint32_t)rec[0] != 1 || (uint32_t)(rec[0] >> 32) != 0x28)
        return;

    uint32_t mask = aim_mode ? 0x3fu : 0xffu;
    if ((mask & (uint32_t)~(uint32_t)rec[1]) != 0)
        return;
    if ((uint32_t)(rec[4] >> 32) != 0)
        return;
    if (rec[2] == 0 || rec[3] == 0)
        return;
    if ((uint32_t)(rec[1] >> 32) == 0)
        return;
    uint64_t token = rec[4];
    if (token <= 999999u || token >= 2000000u)
        return;

    ((uint64_t *)out)[0] = ROD_LEASE_HDR2;
    ((int *)out)[2] = action_id;
    ((int *)out)[3] = mode;
    ((uint64_t *)out)[2] = g_state_revision;
    ((uint64_t *)out)[3] = g_snapshot_aux_a;
    ((uint64_t *)out)[4] = g_snapshot_aux_b;
    ((uint64_t *)out)[5] = g_snapshot_aux_c;
    ((uint64_t *)out)[6] = rec[0];
    ((uint64_t *)out)[7] = rec[1];
    ((uint64_t *)out)[8] = rec[2];
    ((uint64_t *)out)[9] = rec[3];
    ((uint64_t *)out)[10] = rec[4];
}

void nexus_evasion_dodge_profile(uint32_t version, int64_t out)
{
    if (out == 0 || version - 6 < 0xfffffffbu)
        return;

    spin_lock_acquire(&g_state_lock);
    handler_state_refresh();

    for (int i = 0; i < PROFILE_SLOT_COUNT; i++) {
        char key[0x30];
        if (version == 1)
            snprintf(key, sizeof key, "dodge%s", g_dodge_profile_suffixes[i]);
        else
            snprintf(key, sizeof key, "v%dDodge%s", version,
                     g_dodge_profile_suffixes[i]);

        const evasion_prop_entry_t *e = prop_find(key);
        if (e == NULL)
            break;
        int32_t v = g_prop_state[e->state_index];
        int32_t clamped = (e->vmax <= v) ? e->vmax : v;
        if (e->vmin <= v)
            clamped = v;
        *(int32_t *)(uintptr_t)(out + (int64_t)i * 4) =
            (e->vmin <= v) ? clamped : e->vmin;
    }
    spin_lock_release(&g_state_lock);
}

bool nexus_autododge_set_enabled(uint32_t value)
{
    return evasion_set_value("autododgeEnabled", value, 0) == 1;
}

bool nexus_autododge_set_replay_block(int value)
{
    uint32_t before = (uint32_t)g_replay_block;
    spin_lock_acquire(&g_state_lock);
    g_replay_block = (value != 0);
    spin_lock_release(&g_state_lock);
    return before != (uint32_t)(value != 0);
}

void *nexus_evasion_resolve_slot(int slot)
{
    static void *const slots[4] = {
        (void *)(uintptr_t)0x117d08,
        (void *)(uintptr_t)0x11a5b4,
        (void *)(uintptr_t)0x11a76c,
        (void *)(uintptr_t)0x11a798,
    };
    if ((uint32_t)(slot - 5) < 4)
        return slots[slot - 5];
    return NULL;
}

static const char *dodge_key_resolve(const dodge_key_entry_t *entry,
                                     uint32_t version, char *buf)
{
    const char *name = entry->name;
    if (name == NULL || entry->effective_flag == 0)
        return name;
    if (strncmp(name, "dodge", 5) == 0 &&
        strcmp(name, "dodgeVersion") != 0 &&
        strcmp(name, "dodgeBlacklistMask") != 0 &&
        strcmp(name, "dodgePrecision") != 0 &&
        version - 2 < 4) {
        snprintf(buf, 0x60, "v%uDodge%s", version, name + 5);
        return buf;
    }
    return name;
}

static void dodge_triple_fill(int *out, const char *name,
                              int32_t effective_flag, uint32_t requested,
                              int port_state)
{
    out[2] = 0;
    out[3] = 0;
    out[4] = 0;
    out[5] = 0;
    out[3] = (int)requested;
    *(uint8_t *)((char *)out + 0x15) = 1;
    out[0] = 0x18;
    uint32_t effective = requested;
    if (effective_flag == 0)
        effective = (uint32_t)evasion_get_effective(name);
    out[2] = (int)effective;
    *(bool *)((void *)out + 20) = (port_state != 3);
    uint32_t status;
    if (port_state == 3) {
        status = 3;
    } else {
        status = (effective_flag == 0 && port_state != 2 && port_state != 1)
                     ? 1u : 0u;
    }
    out[4] = (int)status;
}

void evasion_menu_backend_dodge(void *ctx, uint32_t key, int *out)
{
    (void)ctx;
    if (out == NULL || out[0] != 0x18) {
        if (out != NULL)
            out[0] = 4;
        return;
    }

    if (key - 0x10000u < QUERY_SLOT_COUNT) {
        uint32_t q = key - 0x10000u;
        const char *name = g_dodge_query_table[q].name;
        if (name == NULL)
            return;
        uint32_t requested = (uint32_t)evasion_get_requested(name);
        int port_state = evasion_get_port_state(name);
        if (requested == 0x80000000u)
            return;
        out[1] = (int)requested;
        dodge_triple_fill(out, name, g_dodge_query_table[q].effective_flag,
                          requested, port_state);
        out[1] = (int)requested;
    } else if (key <= 0xa7) {
        uint32_t idx = key;
        char buf[0x60];
        uint32_t version = (uint32_t)evasion_get_requested("dodgeVersion");
        const char *name = dodge_key_resolve(&g_dodge_key_table[idx],
                                             version, buf);
        if (name == NULL)
            return;
        uint32_t requested = (uint32_t)evasion_get_requested(name);
        int port_state = evasion_get_port_state(name);
        if (requested == 0x80000000u)
            return;
        uint32_t value = requested;
        if (idx - 0x5fu < 10)
            value = (requested >> (g_dodge_key_table[idx].bit_select & 0x1f)) & 1;
        dodge_triple_fill(out, name, g_dodge_key_table[idx].effective_flag,
                          requested, port_state);
        out[1] = (int)value;
    }
}

void dodge_version_set_param(void *ctx, const char *name, uint32_t value)
{
    (void)ctx;
    (void)name;
    nexus_evasion_set_parameter("dodgeVersion", value);
}

extern void journal_log_only(const char *reason, int code);
extern void *dlopen_resident(const char *path);
extern void game_call_flush(uint64_t epoch, uint64_t mono);
extern void capture_replay_enter(volatile int *gate, void *ctx);
extern volatile int g_capture_replay_gate;
extern char g_capture_journal_busy;
extern char g_capture_replay_latch;
extern uint64_t g_capture_replay_state;

extern char g_visual_ready;
extern uint64_t g_journal_sequence;
extern char g_journal_name[0x80];
extern uint64_t g_journal_lines;
extern uint64_t g_capture_last_ms;
extern int capture_gate_try(uint64_t *gate, uint64_t mono);
extern void capture_gate_release(uint64_t *gate, int seq);
extern uint64_t g_capture_gate;
extern uint64_t g_frame_gate;
extern char g_frame_visual_byte;
extern uint64_t g_hud_us;
extern uint64_t g_hud_max_us;
extern uint64_t g_aim_us;
extern uint64_t g_aim_max_us;
extern uint64_t g_trophy_us;
extern uint64_t g_trophy_max_us;
extern uint64_t g_branding_us;
extern uint64_t g_branding_max_us;
extern uint64_t g_visual_us;
extern uint64_t g_visual_max_us;
extern uint64_t g_dodge_us;
extern uint64_t g_dodge_max_us;
extern uint64_t g_periodic_us;
extern uint64_t g_periodic_max_us;
extern uint64_t g_consumer_us;
extern uint64_t g_consumer_max_us;
extern uint64_t g_perf_frames;
extern uint64_t g_perf_total_us;
extern uint64_t g_prev_callback_us;
extern uint64_t g_max_callback_us;
extern uint64_t g_frame_epoch;
extern uint64_t g_frame_read_bytes;
extern uint64_t g_frame_read_failures;
extern int journal_chunk_append(char *buf, uint64_t pos, uint64_t *pos_out,
                                const char *fmt, ...);

extern char g_functions_route_armed;
extern char g_functions_route_done;
extern uint64_t g_route_tag;
extern int (*g_route_register_a)(const void *);
extern int (*g_route_register_b)(const void *);
extern uint64_t g_route_field_a;
extern uint64_t g_route_field_b;
extern uint64_t g_route_field_c;
extern uint64_t g_route_field_d;
extern uint64_t g_route_field_e;
extern uint64_t g_route_field_f;
extern uint64_t g_route_field_g;
extern uint64_t g_route_field_h;
extern uint64_t g_route_field_i;
extern uint64_t g_route_field_j;
extern uint64_t g_route_field_k;
extern uint64_t g_route_field_l;
extern uint64_t g_route_field_m;
extern uint64_t g_route_field_n;
extern uint64_t g_route_aux_a;
extern uint64_t g_route_aux_b;
extern uint64_t g_route_aux_c;
extern void *g_route_template_a;
extern void *g_route_template_b;
extern void *g_route_template_c;
extern void route_stage_a(void);
extern void route_stage_b(void *ctx);
extern void route_stage_c(void *ctx);

static const char GAME_SHA[] =
    "a10aeb6b4085fb2a15d969a130cd9608231998b5269a40aed41129d99624ede3";
static const char PROFILE_SHA[] =
    "114ba105835bd4157cfd80aad9498734d51108ee193601410abeb5e0bab3a71c";

void runtime_journal_write(const char *stage, const char *reason,
                           const char *payload)
{
    uint32_t limit = 0x120;
    if (strcmp(stage, "aura_phase") == 0 || strcmp(stage, "dodge_phase") == 0)
        limit = 0x140;

    if (g_journal_lines >= limit)
        return;
    g_journal_lines++;

    uint32_t pid = (uint32_t)getpid();
    uint32_t uid = (uint32_t)getuid();
    uint32_t tid = (uint32_t)gettid();
    uint64_t seq = g_journal_sequence;

    struct timespec ts;
    uint64_t time_ms = 0;
    if (clock_gettime(CLOCK_REALTIME, &ts) == 0)
        time_ms = (uint64_t)ts.tv_sec * 1000 + (uint64_t)ts.tv_nsec / 1000000;
    uint64_t mono_ms = 0;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0)
        mono_ms = (uint64_t)ts.tv_sec * 1000 + (uint64_t)ts.tv_nsec / 1000000;

    const char *suffix = (payload != NULL) ? payload : ROD_JOURNAL_EMPTY;

    char buf[4000];
    int n = snprintf(buf, sizeof buf,
        "{\"schema\":1,\"component\":\"nexus_evasion_runtime\","
        "\"module\":\"EvasionGame\",\"stage\":\"evasion_game_%s\","
        "\"reason\":\"%s\",\"pid\":%d,\"uid\":%d,\"tid\":%d,"
        "\"start_ticks\":%llu,\"time_ms\":%llu,\"mono_ms\":%llu,"
        "\"observer_rva\":\"0xb337c4\",\"capture_request_id\":%llu,"
        "\"gameplay_effective\":false%s}",
        stage, reason, pid, uid, tid, (unsigned long long)seq,
        (unsigned long long)time_ms, (unsigned long long)mono_ms,
        (unsigned long long)g_capture_request_id, suffix);
    if (n < 0 || (size_t)n >= sizeof buf)
        return;

    __android_log_write(4, "NexusLab69252", buf);

    if (g_journal_fd < 0)
        return;
    if ((uint64_t)n + 1 + g_journal_bytes >= JOURNAL_MAX_LINE + 1)
        return;
    ssize_t w1 = write(g_journal_fd, buf, (size_t)n);
    if (w1 != (ssize_t)n)
        goto broken;
    ssize_t w2 = write(g_journal_fd, "\n", 1);
    if (w2 != 1)
        goto broken;
    g_journal_bytes += (uint64_t)n + 1;
    return;

broken:
    close(g_journal_fd);
    g_journal_fd = -1;
}

bool evasion_backend_bind(void)
{
    g_evasion_lib_handle = dlopen_resident(g_expected_lib_path);
    if (g_evasion_lib_handle == NULL) {
        const char *err = dlerror();
        if (err == NULL)
            err = dlerror();
        if (err == NULL)
            err = "unknown";
        __android_log_print(6, "NexusMem",
                            "evasion backend resident handle missing path=%s error=%s",
                            g_expected_lib_path, err);
        return false;
    }

    g_be_get_requested_fn = (int32_t (*)(const char *))dlsym(
        g_evasion_lib_handle, "nexus_evasion_get_requested");
    g_be_dodge_profile_fn = (int32_t (*)(uint32_t, void *))dlsym(
        g_evasion_lib_handle, "nexus_evasion_dodge_profile");

    Dl_info info;
    int bad = 0;
    if (g_be_get_requested_fn == NULL ||
        dladdr((void *)(uintptr_t)g_be_get_requested_fn, &info) == 0 ||
        info.dli_fbase != g_expected_lib_base ||
        info.dli_fname == NULL ||
        strcmp(info.dli_fname, g_expected_lib_path) != 0)
        bad = 1;
    if (g_be_dodge_profile_fn == NULL ||
        dladdr((void *)(uintptr_t)g_be_dodge_profile_fn, &info) == 0 ||
        info.dli_fbase != g_expected_lib_base ||
        info.dli_fname == NULL ||
        strcmp(info.dli_fname, g_expected_lib_path) != 0)
        bad = 1;

    if (bad) {
        __android_log_print(6, "NexusMem",
                            "evasion backend export owner mismatch req=%p profile=%p base=%p path=%s",
                            (void *)(uintptr_t)g_be_get_requested_fn,
                            (void *)(uintptr_t)g_be_dodge_profile_fn,
                            g_expected_lib_base, g_expected_lib_path);
        return false;
    }

    int32_t version = g_be_get_requested_fn("dodgeVersion");
    if (version - 1 < 5) {
        int32_t profile[PROFILE_SLOT_COUNT];
        if (g_be_dodge_profile_fn((uint32_t)version, profile) == 1)
            return true;
    }
    return false;
}

static void capture_request_poll(void)
{
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
        return;
    uint64_t mono = (uint64_t)ts.tv_sec * 1000 + (uint64_t)ts.tv_nsec / 1000000;
    if (mono == 0 || mono <= g_capture_last_ms)
        return;
    if (g_capture_last_ms != 0 && mono - g_capture_last_ms <= 999)
        return;
    if (g_files_dir_fd < 0)
        return;

    uint64_t expected = g_capture_last_ms;
    if (__atomic_compare_exchange_n(&g_capture_last_ms, &expected, mono,
                                    false, __ATOMIC_ACQ_REL,
                                    __ATOMIC_ACQUIRE) == 0)
        return;

    spin_lock_acquire(&g_capture_open_lock);
    {
        int fd = openat(g_files_dir_fd, "nexus-evasion-game.capture", 0x88800);
        if (fd < 0) {
            if (errno != ENOENT)
                journal_log_only("control_open_refused", 0);
        } else {
            struct stat sb;
            if (fstat(fd, &sb) != 0 || !S_ISREG(sb.st_mode) ||
                sb.st_uid != getuid() || sb.st_nlink != 1 ||
                (sb.st_mode & 0x1ff) != 0x180 ||
                sb.st_size < 1 || sb.st_size > 0x60) {
                spin_lock_acquire(&g_frame_lock);
                if (g_capture_rejects < CAPTURE_MAX_REJECTS)
                    runtime_journal_write("capture_request_rejected",
                                          "control_owner_mode_type_link_or_size",
                                          NULL);
                g_capture_rejects++;
                spin_lock_release(&g_frame_lock);
                close(fd);
                goto out;
            }

            char buf[0x60];
            size_t len = (size_t)sb.st_size;
            ssize_t r = read(fd, buf, len);
            struct stat sb2;
            if (r != (ssize_t)len || fstat(fd, &sb2) != 0 ||
                sb.st_dev != sb2.st_dev || sb.st_ino != sb2.st_ino ||
                sb.st_size != sb2.st_size || sb.st_uid != sb2.st_uid ||
                sb.st_mode != sb2.st_mode || sb.st_nlink != sb2.st_nlink ||
                sb.st_mtim.tv_sec != sb2.st_mtim.tv_sec) {
                close(fd);
                journal_log_only("control_changed_during_read", 0);
                goto out;
            }
            close(fd);
            if (sb.st_mtim.tv_nsec != sb2.st_mtim.tv_nsec) {
                journal_log_only("control_changed_during_read", 0);
                goto out;
            }

            uint64_t watermark = 0;
            uint32_t samples = 0;
            uint32_t interval = 0;
            if (capture_request_parse(buf, len, &watermark, &samples,
                                      &interval) == 0) {
                journal_log_only("control_format", 0);
                goto out;
            }

            if (g_capture_request_id >= watermark)
                goto out;

            spin_lock_acquire(&g_frame_lock);
            if (g_journal_fd >= 0) {
                struct stat jb;
                if (fstat(g_journal_fd, &jb) != 0 || !S_ISREG(jb.st_mode) ||
                    journal_reopen((int *)&g_journal_fd) == 0)
                    goto reject_reset;
                g_journal_bytes = 0;
                g_journal_lines = 0;
                g_capture_request_id = watermark;
                g_capture_samples = samples;
                g_capture_interval = interval;
                g_capture_accepted = 0;
                g_capture_seq_lo = 0;
                g_capture_seq_hi = 0;
                g_max_callback_us = 0;
                g_prev_callback_us = 0;
                g_frame_read_calls = 0;
                g_frame_feed_us = 0;
                char extra[0x100];
                snprintf(extra, sizeof extra,
                         ",\"samples\":%u,\"interval_ms\":%u,"
                         "\"journal\":\"%s\",\"diagnostic_reset_only\":true",
                         samples, interval, g_journal_name);
                runtime_journal_write("capture_request_accepted",
                                      "validated_private_diagnostic_reset",
                                      extra);
            } else {
reject_reset:
                if (g_capture_rejects < CAPTURE_MAX_REJECTS)
                    runtime_journal_write("capture_request_rejected",
                                          "capture_journal_reset_failed", NULL);
                g_capture_rejects++;
            }
            spin_lock_release(&g_frame_lock);
        }
    }
out:
    spin_lock_release(&g_capture_open_lock);
}

typedef struct {
    uint64_t tag;
    uint64_t frame_a;
    uint64_t frame_b;
    uint64_t frame_c;
    uint64_t sequence;
} ng_feed_record_t;

typedef struct {
    uint64_t tag;
    uint64_t screen;
    uint64_t own_arg;
    uint64_t optional_arg;
    uint64_t client;
    uint64_t battle;
    int32_t own_gid;
    int32_t own_x;
    int32_t own_y;
    int32_t own_z;
    int32_t own_radius;
    int32_t own_team;
    int32_t object_count;
    uint32_t friendly_projectiles;
    uint32_t unknown_objects;
    uint32_t feed_complete;
    int32_t requested_autododge;
    int32_t dodge_version;
    int32_t profile[PROFILE_SLOT_COUNT];
    uint32_t frame_reason;
    uint32_t stable_frames;
    uint32_t passive_ready;
    uint64_t epoch;
    uint32_t proj_gid[PROJECTILE_SLOTS];
    int32_t proj_x[PROJECTILE_SLOTS];
    int32_t proj_y[PROJECTILE_SLOTS];
    int32_t proj_z[PROJECTILE_SLOTS];
    int32_t proj_team[PROJECTILE_SLOTS];
    int32_t proj_speed[PROJECTILE_SLOTS];
    int32_t proj_radius[PROJECTILE_SLOTS];
    uint32_t proj_bouncing[PROJECTILE_SLOTS];
} ng_status_record_t;

typedef struct {
    ng_feed_record_t *feed;
    ng_status_record_t *status;
    void *projectiles;
    uint32_t projectile_count;
    uint64_t mono;
} frame_work_t;

void evasion_game_frame(uint64_t *frame_ctx)
{
    int saved_errno = errno;

    if (g_visual_ready == 0)
        goto done;

    capture_request_poll();

    spin_lock_acquire(&g_frame_lock);

    struct timespec ts;
    uint64_t mono = 0;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0)
        mono = (uint64_t)ts.tv_sec * 1000 + (uint64_t)ts.tv_nsec / 1000000;

    int gated = capture_gate_try(&g_capture_gate, mono);
    if (gated != 0 || g_game_consumer_fn != NULL || g_frame_visual_byte != 0) {
        uint64_t start_us = 0;
        if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0)
            start_us = (uint64_t)ts.tv_sec * 1000000 +
                       (uint64_t)ts.tv_nsec / 1000;
        int tid = gettid();

        char name[16] = {0};
        pthread_t self = pthread_self();
        int name_ok = pthread_getname_np(self, name, sizeof name) == 0 &&
                      strcmp(name, "Mainloop") == 0;
        if (name_ok && (g_main_thread_id == 0 || g_main_thread_id == tid)) {
            g_frame_read_calls = 0;
            g_frame_read_bytes = 0;
            g_frame_read_failures = 0;

            uint64_t t0 = monotonic_us();
            ng_feed_record_t feed;
            feed.tag = ROD_NG_FEED_TAG;
            feed.frame_a = frame_ctx[1];
            feed.frame_b = frame_ctx[0];
            feed.frame_c = frame_ctx[2];
            feed.sequence = g_frame_sequence;
            int feed_ret = ng_feed_v1((int *)&feed);

            ng_status_record_t status;
            memset(&status, 0, sizeof status);
            status.tag = ROD_NG_STATUS_TAG;
            ng_status_v1(&status);

            uint64_t t1 = monotonic_us();
            g_frame_feed_us = t1 - t0;
            if (g_hud_max_us < g_frame_feed_us)
                g_hud_max_us = g_frame_feed_us;

            if (gated != 0)
                capture_gate_release(&g_capture_gate, feed_ret);

            uint64_t proj_seq = 0;
            uint32_t proj_count = 0;
            uint64_t proj_slots[0x50];
            if (feed_ret == 0 && g_main_thread_id == 0) {
                proj_count = 0;
            } else {
                g_main_thread_id = tid;
                int n = ng_projectiles_v1(proj_slots, 0x40, &proj_seq);
                if (n >= 0 && proj_seq == g_frame_sequence)
                    proj_count = (uint32_t)n;
            }

            if (g_game_consumer_fn != NULL || g_frame_visual_byte != 0) {
                frame_work_t work;
                work.feed = &feed;
                work.status = &status;
                work.projectiles = proj_slots;
                work.projectile_count = proj_count;
                work.mono = mono;

                uint64_t ta = monotonic_us();
                hud_pump(&work);
                uint64_t tb = monotonic_us();
                g_hud_us += tb - ta;
                if (g_hud_max_us < tb - ta)
                    g_hud_max_us = tb - ta;

                game_call_flush(g_frame_epoch, mono);
                ng_frame_post(&work);

                ta = monotonic_us();
                aim_pump(&work);
                tb = monotonic_us();
                g_aim_us += tb - ta;
                if (g_aim_max_us < tb - ta)
                    g_aim_max_us = tb - ta;

                ta = monotonic_us();
                trophies_battle_tick(&work);
                tb = monotonic_us();
                g_trophy_us += tb - ta;
                if (g_trophy_max_us < tb - ta)
                    g_trophy_max_us = tb - ta;

                ta = monotonic_us();
                branding_pump(&work);
                tb = monotonic_us();
                g_branding_us += tb - ta;
                if (g_branding_max_us < tb - ta)
                    g_branding_max_us = tb - ta;

                stage_pump_a(&work);
                stage_pump_b(&work);

                ta = monotonic_us();
                periodic_pump(&work);
                tb = monotonic_us();
                g_periodic_us += tb - ta;
                if (g_periodic_max_us < tb - ta)
                    g_periodic_max_us = tb - ta;

                chat_frame_record(&work);

                ta = monotonic_us();
                visual_pump(&work);
                tb = monotonic_us();
                g_visual_us += tb - ta;
                if (g_visual_max_us < tb - ta)
                    g_visual_max_us = tb - ta;

                visual_pump_b(&work);

                ta = monotonic_us();
                dodge_tick(&work);
                tb = monotonic_us();
                g_dodge_us += tb - ta;
                if (g_dodge_max_us < tb - ta)
                    g_dodge_max_us = tb - ta;

                stage_pump_c(&work);
                stage_pump_d(&work);
                stage_pump_e(&work);
                stage_pump_f(&work);
                stage_pump_g(&work);
                stage_capture_tick(&work);

                if (g_frame_visual_byte == 1 && stage_capture_flush() != 0) {
                    g_capture_journal_busy = 1;
                    capture_replay_enter(&g_capture_replay_gate, &work);
                    g_capture_journal_busy = 0;
                    if (g_capture_replay_latch == 1) {
                        g_capture_replay_latch = 0;
                        g_capture_replay_state = 0;
                    }
                }

                if (g_game_consumer_fn != NULL) {
                    ta = monotonic_us();
                    g_game_consumer_fn(g_game_consumer_ctx, &work);
                    tb = monotonic_us();
                    g_consumer_us += tb - ta;
                    if (g_consumer_max_us < tb - ta)
                        g_consumer_max_us = tb - ta;
                }
            }

            if (gated != 0 && (feed_ret != 0 || g_capture_accepted < 0x21)) {
                char journal_buf[3200];
                uint64_t jpos = 0;
                int jok = journal_chunk_append(
                    journal_buf, 0, &jpos,
                    ",\"accepted\":%s,\"callbacks\":%llu,\"sequence\":%llu,"
                    "\"frame_reason\":%u,\"stable_frames\":%u,"
                    "\"passive_ready\":%u,\"epoch\":%llu,"
                    "\"screen\":\"0x%llx\",\"own_arg\":\"0x%llx\","
                    "\"optional_arg\":\"0x%llx\",\"client\":\"0x%llx\","
                    "\"battle\":\"0x%llx\",\"own_gid\":%d,\"own_x\":%d,"
                    "\"own_y\":%d,\"own_z\":%d,\"own_radius\":%d,"
                    "\"own_team\":%d,\"object_count\":%d,"
                    "\"projectile_count\":%u,\"friendly_projectiles\":%u,"
                    "\"unknown_objects\":%u,\"feed_complete\":%u,"
                    "\"requested_autododge\":%d,\"dodge_version\":%d,"
                    "\"profile\":[%d,%d,%d,%d,%d,%d],\"duration_us\":%llu,"
                    "\"read_calls\":%llu,\"read_bytes\":%llu,"
                    "\"read_failures\":%llu,\"projectiles\":[",
                    feed_ret != 0 ? "true" : "false",
                    (unsigned long long)g_journal_sequence,
                    (unsigned long long)g_frame_sequence,
                    status.frame_reason, status.stable_frames,
                    status.passive_ready, (unsigned long long)status.epoch,
                    (unsigned long long)status.screen,
                    (unsigned long long)status.own_arg,
                    (unsigned long long)status.optional_arg,
                    (unsigned long long)status.client,
                    (unsigned long long)status.battle,
                    status.own_gid, status.own_x, status.own_y, status.own_z,
                    status.own_radius, status.own_team, status.object_count,
                    proj_count, status.friendly_projectiles,
                    status.unknown_objects, status.feed_complete,
                    status.requested_autododge, status.dodge_version,
                    status.profile[0], status.profile[1], status.profile[2],
                    status.profile[3], status.profile[4], status.profile[5],
                    (unsigned long long)g_frame_feed_us,
                    (unsigned long long)g_frame_read_calls,
                    (unsigned long long)g_frame_read_bytes,
                    (unsigned long long)g_frame_read_failures);

                uint32_t logged = (proj_count < PROJECTILE_SLOTS)
                                      ? proj_count : PROJECTILE_SLOTS;
                for (uint32_t i = 0; jok && i < logged; i++) {
                    jok = journal_chunk_append(
                        journal_buf, jpos, &jpos,
                        "%s{\"gid\":%d,\"x\":%d,\"y\":%d,\"z\":%d,"
                        "\"team\":%d,\"speed\":%d,\"radius\":%d,"
                        "\"bouncing\":%u}",
                        (i == 0) ? ROD_JOURNAL_EMPTY : ROD_JOURNAL_COMMA,
                        status.proj_gid[i], status.proj_x[i],
                        status.proj_y[i], status.proj_z[i],
                        status.proj_team[i], status.proj_speed[i],
                        status.proj_radius[i], status.proj_bouncing[i]);
                }
                if (jok) {
                    jok = journal_chunk_append(
                        journal_buf, jpos, &jpos,
                        "],\"projectiles_logged\":%d,"
                        "\"projectiles_omitted\":%d,"
                        "\"previous_callback_us\":%llu,"
                        "\"max_callback_us\":%llu",
                        logged, proj_count - logged,
                        (unsigned long long)g_prev_callback_us,
                        (unsigned long long)g_max_callback_us);
                }
                if (jok)
                    runtime_journal_write(
                        "frame",
                        (feed_ret != 0) ? "readonly_frame_observed"
                                        : "frame_contract_rejected",
                        journal_buf);

                if (g_capture_samples == 0)
                    runtime_journal_write("capture_limit",
                                          "256_accepted_samples_complete",
                                          NULL);
            }
        } else {
            uint32_t seq = g_capture_seq_lo;
            g_capture_seq_hi = (uint32_t)g_capture_seq_lo + 1;
            if (seq < 8)
                runtime_journal_write("frame_thread_rejected",
                                      "expected_Mainloop", NULL);
        }

        if (start_us != 0) {
            uint64_t now_us = 0;
            if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0)
                now_us = (uint64_t)ts.tv_sec * 1000000 +
                         (uint64_t)ts.tv_nsec / 1000;
            g_prev_callback_us = now_us - start_us;
            if (g_max_callback_us < g_prev_callback_us)
                g_max_callback_us = g_prev_callback_us;
            g_perf_total_us += g_prev_callback_us;
            g_perf_frames++;
            if (g_perf_frames > PERF_WINDOW) {
                uint64_t n = g_perf_frames;
                __android_log_print(
                    4, "NexusPerf",
                    "frames=%u total_us=%llu feed_us=%llu reads=%llu "
                    "hud=%llu aim=%llu trophy=%llu branding=%llu "
                    "visual=%llu dodge=%llu periodic=%llu consumer=%llu "
                    "aim_max=%llu branding_max=%llu",
                    (uint32_t)n, g_perf_total_us / n,
                    g_frame_feed_us / n, g_frame_read_calls / n,
                    g_hud_us / n, g_aim_us / n, g_trophy_us / n,
                    g_branding_us / n, g_visual_us / n, g_dodge_us / n,
                    g_periodic_us / n, g_consumer_us / n, g_aim_max_us,
                    g_branding_max_us);
                g_perf_frames = 0;
                g_perf_total_us = 0;
                g_frame_read_calls = 0;
                g_hud_us = 0;
                g_aim_us = 0;
                g_trophy_us = 0;
                g_branding_us = 0;
                g_visual_us = 0;
                g_dodge_us = 0;
                g_periodic_us = 0;
                g_consumer_us = 0;
                g_aim_max_us = 0;
                g_hud_max_us = 0;
                g_branding_max_us = 0;
                g_trophy_max_us = 0;
                g_visual_max_us = 0;
                g_dodge_max_us = 0;
                g_consumer_max_us = 0;
                g_periodic_max_us = 0;
            }
        }
    }

    spin_lock_release(&g_frame_lock);
done:
    errno = saved_errno;
}

uint32_t functions_route_register(int param)
{
    if (param == 0 || g_functions_route_armed == 0)
        return 0;

    uint64_t rec[30];
    memset(rec, 0, sizeof rec);
    rec[0] = ROD_INPUT_TAG;
    rec[1] = g_journal_sequence;
    rec[2] = g_route_field_a;
    rec[3] = g_route_field_b;
    rec[4] = g_route_field_c;
    rec[5] = 0;
    rec[6] = g_route_field_d;
    rec[7] = g_route_field_e;
    rec[8] = (uint64_t)(uintptr_t)&evasion_game_frame;
    rec[9] = (uint64_t)(uintptr_t)g_route_template_a;
    rec[10] = (uint64_t)(uintptr_t)g_route_template_b;
    rec[11] = g_route_field_f;
    rec[12] = g_route_field_g;
    rec[13] = g_route_field_h;
    rec[14] = g_route_field_i;
    rec[15] = g_route_field_j;
    rec[16] = g_route_field_k;
    rec[17] = g_route_field_l;
    rec[18] = g_route_field_m;
    rec[19] = g_route_field_n;
    rec[20] = g_route_aux_a;
    rec[21] = g_route_aux_b;
    rec[22] = g_route_aux_c;
    if ((*g_route_register_a)(rec) == 0)
        return 0;

    g_route_tag = 0x1000003f7;
    void *template_c = (void *)(uintptr_t)0x1cb6e0;
    if (g_dodge_bound == 0) {
        g_route_tag = 0x100000003;
        template_c = NULL;
    }

    uint64_t rec2[36];
    memset(rec2, 0, sizeof rec2);
    rec2[0] = ROD_CAPTURE_TAG;
    rec2[1] = g_journal_sequence;
    rec2[2] = (uint64_t)(uintptr_t)&evasion_game_frame;
    rec2[3] = (uint64_t)(uintptr_t)route_stage_a;
    rec2[4] = ROD_STATUS_TAG;
    rec2[5] = (uint64_t)(uintptr_t)route_stage_b;
    rec2[6] = 0;
    rec2[7] = (uint64_t)(uintptr_t)template_c;
    rec2[8] = (uint64_t)(uintptr_t)template_c;
    rec2[9] = (uint64_t)(uintptr_t)template_c;
    rec2[10] = (uint64_t)(uintptr_t)template_c;
    rec2[11] = (uint64_t)(uintptr_t)template_c;
    rec2[12] = (uint64_t)(uintptr_t)route_stage_c;
    if ((*g_route_register_b)(rec2) == 0) {
        g_route_tag = 0x200000000;
        return 0;
    }

    g_functions_route_done = 1;
    runtime_journal_write(
        "functions_registered",
        (g_dodge_bound != 0) ? "smartaim_dodge_v1_v5_follow"
                             : "natural_smartaim_only",
        ",\"aim\":true,\"prediction_for_aim\":true,"
        "\"dodge_handler_separate_diagnostic\":true,"
        "\"require_move_hold\":false,\"free_scope\":2,"
        "\"paid_state_changed\":false");
    return 1;
}

extern char g_game_image_ok;
extern char g_alloc_owner;
extern char g_alloc_owner_ok;
extern int guest_game_elf_check(void *a, uintptr_t base, const void *table,
                                uintptr_t read_fn, int flags);
extern int allocator_got_owner(void *ctx, const char *tag);
extern void *actor_scratch_new(void *out);
extern void *actor_scratch_read(void *scratch, void *buf, uint32_t len);
extern void actor_scratch_checksum(void *scratch, uint64_t sums[4]);
extern uint64_t dodge_state_size_a(void);
extern uint64_t dodge_state_size_b(void);
extern int dodge_state_init_a(void *state, uint64_t len, const void *rec);
extern int dodge_state_init_b(void *state, uint64_t len);
extern void dodge_state_publish_a(void *state, const void *rec, void *out);
extern void dodge_state_publish_b(void *state, const void *rec, void *out);
extern void bind_stage_a(uint64_t a, uint64_t b);
extern void bind_stage_b(uint64_t a, uint64_t b);
extern void bind_stage_c(uint64_t a, uint64_t b, uint64_t c, uint64_t d);
extern void bind_stage_d(void *a, uint64_t b);
#include <link.h>

extern int phdr_owner_cb(struct dl_phdr_info *info, size_t size, void *data);
extern int actor_region_read(uintptr_t handle, uintptr_t base, void *out,
                             uint32_t len);

extern uint64_t g_dodge_poison_table;
extern uint64_t g_dodge_actor_tick_us;
extern uint64_t g_dodge_max_actor_tick_us;
extern uint64_t g_dodge_published;
extern uint64_t g_dodge_source_tick;
extern uint64_t g_dodge_active_known;
extern uint64_t g_dodge_active;
extern uint64_t g_dodge_ended;
extern uint32_t g_dodge_collect_stage;
extern uint32_t g_dodge_collect_index;
extern uint32_t g_dodge_object_count;
extern uint32_t g_dodge_actor_count;
extern uint32_t g_dodge_coverage_known;
extern uint32_t g_dodge_reason;
extern uint64_t g_dodge_local_calls;
extern uint64_t g_dodge_queue_calls;
extern int32_t g_dodge_target_x;
extern int32_t g_dodge_target_y;
extern uint64_t g_dodge_local_perm_calls;
extern uint64_t g_dodge_queue_perm_calls;
extern uint64_t g_dodge_local_perm_refusals;
extern uint64_t g_dodge_queue_perm_refusals;
extern uint64_t g_dodge_epoch;
extern uint64_t g_dodge_settings_gen;
extern uint32_t g_dodge_proposal_kind;
extern uint32_t g_dodge_proposal_reason;
extern uint32_t g_dodge_live_threats;
extern uint32_t g_dodge_begin_reason;
extern uint32_t g_dodge_apply_reason;
extern uint32_t g_dodge_pending;
extern uint64_t g_dodge_frame_publications;
extern uint64_t g_dodge_last_status_ms;
extern uint32_t g_dodge_last_status_bits;
extern uint64_t g_dodge_last_status_change_ms;
extern uint32_t g_dodge_requested;
extern uint32_t g_dodge_version;
extern int32_t g_dodge_own_gid;
extern int32_t g_dodge_own_x;
extern int32_t g_dodge_own_y;
extern uint32_t g_dodge_wall_known;
extern uint32_t g_dodge_hazard_known;
extern uint32_t g_dodge_classifier_ready;
extern uint32_t g_dodge_classifier_reason;
extern uint32_t g_dodge_consumer_reason;

extern int (*g_dodge_lock_fn)(void *);
extern void *g_dodge_lock_ctx;
extern char g_dodge_unlock_flag;
extern char g_dodge_extra_gate;
extern uint64_t g_dodge_extra_ctx;
extern uint64_t g_dodge_own_hp;
extern uint64_t g_dodge_team_table;
extern uint64_t g_dodge_proj_count;
extern uint64_t g_dodge_state_block[0xdb8 / 8];
extern uint64_t g_dodge_reset_tag;
extern uint64_t g_dodge_collect_tag;
extern uint64_t g_dodge_frame_tag;
extern uint64_t g_dodge_own_speed;
extern uint64_t g_dodge_own_radius;

typedef struct {
    uint64_t f[11];
} projectile_record_t;

extern projectile_record_t g_projectile_records[8];
extern int projectile_record_read(uintptr_t base, uint64_t gid,
                                  projectile_record_t *out);
extern int poison_record_read(uintptr_t base, uint64_t gid, void *out);
extern uint64_t threat_score(float x, float y, float ox, float oy,
                             uint64_t a, uint64_t b, void *fn, int c,
                             int cap);
extern uint32_t projectile_classify(uint64_t addr);

static const uint64_t actor_region_sizes[6] = {
    0, 0, 0, 0, 0, 0,
};
static const uint64_t actor_region_bases[6] = {
    0, 0, 0, 0, 0, 0,
};
static const uint64_t actor_region_sums[6][4] = {
    {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
    {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
};

void dodge_bind(void)
{
    const char *reason;
    const char *stage = "dodge_disabled";
    const char *suffix = ",\"action\":22,\"handler_registered\":false";

    if (guest_game_elf_check((void *)&g_game_image_ok, g_game_base,
                             (const void *)(uintptr_t)0x1c3220,
                             (uintptr_t)(uintptr_t)game_read, 0) == 0) {
        reason = "guest_game_elf";
        goto report;
    }

    g_game_image_ok = 0;
    dl_iterate_phdr(phdr_owner_cb, NULL);
    reason = "packaged_cpp_owner";
    if (g_game_image_ok != 1 || g_alloc_owner == 0)
        goto report;

    g_alloc_owner_ok = 1;
    if (allocator_got_owner(NULL, "packaged_cpp_owner") == 0) {
        reason = "allocator_got_owner";
        goto report;
    }

    for (int i = 0; i < 6; i++) {
        char scratch[0x70];
        uint64_t sums[4];
        uint64_t total = actor_region_sizes[i];
        if (total != 0) {
            uint64_t off = 0;
            while (off < total) {
                uint32_t chunk = (uint32_t)(total - off);
                if (chunk > 0x100)
                    chunk = 0x100;
                char buf[0x100];
                if (actor_region_read(0, g_game_base + off +
                                              actor_region_bases[i],
                                      buf, chunk) == 0)
                    goto contract_failed;
                if (actor_scratch_read(scratch, buf, chunk) == 0)
                    goto contract_failed;
                off += chunk;
            }
        }
        actor_scratch_checksum(scratch, sums);
        for (int k = 0; k < 4; k++) {
            if (sums[k] != actor_region_sums[i][k])
                goto contract_failed;
        }
    }

    uint64_t size_a = dodge_state_size_a();
    uint64_t size_b = dodge_state_size_b();
    if (size_a - 0x100001 <= 0xffffffffffffefffu || size_b == 0 ||
        size_b >= 0x10001) {
        reason = "state_size";
        goto report;
    }

    g_dodge_state_a = mmap(NULL, size_a, 3, 0x22, -1, 0);
    g_dodge_state_b = mmap(NULL, size_b, 3, 0x22, -1, 0);
    reason = "resident_state_allocation";
    if (g_dodge_state_a == MAP_FAILED || g_dodge_state_b == MAP_FAILED)
        goto report;

    {
        uint64_t rec[10];
        rec[0] = g_game_base;
        rec[1] = (uint64_t)(uintptr_t)GAME_SHA;
        rec[2] = (uint64_t)(uintptr_t)PROFILE_SHA;
        rec[3] = 0;
        rec[4] = ROD_MOVE_TAG;
        rec[5] = (uint64_t)(uintptr_t)game_read;
        rec[6] = (uint64_t)(uintptr_t)bind_stage_a;
        rec[7] = (uint64_t)(uintptr_t)bind_stage_b;
        rec[8] = (uint64_t)(uintptr_t)bind_stage_c;
        rec[9] = (uint64_t)(uintptr_t)bind_stage_d;

        if (dodge_state_init_a(g_dodge_state_a, size_a, rec) != 0 &&
            dodge_state_init_b(g_dodge_state_b, size_b) != 0) {
            g_dodge_bound = 1;
            runtime_journal_write(
                "dodge_bound", "single_source_consumer_and_guest_ops",
                ",\"action\":22,\"version\":3,\"require_move_hold\":false,"
                "\"native_calls_during_bind\":0,\"new_hooks\":0");
            return;
        }
        reason = "consumer_native_binding";
    }
    goto report;

contract_failed:
    reason = "actor_native_contract";
report:
    g_dodge_bound = 0;
    runtime_journal_write(stage, reason, suffix);
}

void dodge_tick(void *ctx)
{
    uint64_t *frame = ctx;
    struct timespec ts;
    uint64_t t0 = 0;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0)
        t0 = (uint64_t)ts.tv_sec * 1000000 + (uint64_t)ts.tv_nsec / 1000;

    if (g_dodge_bound != 1)
        goto epilogue;

    g_dodge_stats_word_a = 0;
    g_dodge_stats_word_b = 0;
    g_dodge_publication++;
    memset((void *)g_dodge_state_block, 0, 0xdb8);
    g_dodge_active_known = 0;
    g_dodge_active = 0;
    g_dodge_ended = 0;
    g_dodge_pending = 0;

    uintptr_t battle = 0;
    int valid = 0;
    if (frame != NULL && frame[1] != 0) {
        battle = (uintptr_t)frame[2];
        if (battle != 0 && *(int *)(void *)(battle + 0x24) != 0)
            valid = 1;
    }

    if (!valid || g_main_thread_id != gettid() ||
        g_dodge_lock_fn(g_dodge_lock_ctx) != 1) {
        dodge_state_publish_a(g_dodge_state_a, NULL, &g_dodge_reset_tag);
        dodge_state_publish_b(g_dodge_state_b, NULL, &g_dodge_frame_tag);
        goto epilogue;
    }

    uint32_t own_hp = 0;
    game_read(0, battle + 0x24c, &own_hp, 4);
    if (g_dodge_own_hp == 5) {
        if (battle + 0x24c < 0xfffffffffffffdb4ull ||
            own_hp <= 99 || own_hp >= 0x7d1) {
            dodge_state_publish_a(g_dodge_state_a, NULL, &g_dodge_reset_tag);
            dodge_state_publish_b(g_dodge_state_b, NULL, &g_dodge_frame_tag);
            goto epilogue;
        }
    }

    uint64_t team_table = 0, proj_array = 0, proj_list = 0;
    if (game_read(0, *(uintptr_t *)(void *)(battle + 0x30) + 0x58,
                  &team_table, 8) == 0 ||
        game_read(0, *(uintptr_t *)(void *)(battle + 0x38) + 0x28,
                  &proj_list, 8) == 0 ||
        game_read(0, proj_list, &proj_array, 8) == 0 ||
        game_read(0, proj_list + 0xc, &g_dodge_proj_count, 4) == 0 ||
        (int32_t)g_dodge_proj_count < 1 ||
        (int32_t)g_dodge_proj_count > 0x100 ||
        game_read(0, proj_array, g_projectile_gid_list,
                  (size_t)(uint32_t)(int32_t)g_dodge_proj_count * 8) == 0) {
        dodge_state_publish_a(g_dodge_state_a, NULL, &g_dodge_reset_tag);
        dodge_state_publish_b(g_dodge_state_b, NULL, &g_dodge_frame_tag);
        goto epilogue;
    }

    uint32_t collected = 0;
    uint32_t poison_count = 0;
    int collect_ok = 1;
    for (uint32_t i = 0; i < (uint32_t)(int32_t)g_dodge_proj_count; i++) {
        g_dodge_collect_index = i;
        uint64_t gid = g_projectile_gid_list[i];
        if (gid + 0x2000 < 0x12000 || (gid & 7) != 0 ||
            game_read(0, gid, &g_dodge_team_table, 8) == 0) {
            collect_ok = 0;
            break;
        }
        if (gid == *(uintptr_t *)(void *)(battle + 0x40))
            continue;
        uintptr_t obj_type = (uintptr_t)g_dodge_team_table;
        if (obj_type == g_game_base + 0x12206a8) {
            if (collected >= 8) {
                collect_ok = 0;
                break;
            }
            projectile_record_t rec;
            if (projectile_record_read(g_game_base, gid, &rec) == 0) {
                collect_ok = 0;
                break;
            }
            rec.f[0] = i;
            if (g_dodge_extra_gate != 0 &&
                g_dodge_extra_ctx == *(uintptr_t *)(void *)(battle + 0x50)) {
                rec.f[10] = threat_score(
                    (float)(int32_t)rec.f[4] / 300.0f,
                    (float)(int32_t)rec.f[5] / 300.0f,
                    (float)*(int32_t *)(void *)(battle + 0x74) / 300.0f,
                    (float)*(int32_t *)(void *)(battle + 0x78) / 300.0f,
                    g_dodge_own_speed, g_dodge_own_radius,
                    (void *)(uintptr_t)0x150fa0, 0, 0x40);
            } else {
                rec.f[10] = 0xffffffffu;
            }
            g_projectile_records[collected] = rec;
            collected++;
        } else if (obj_type == g_game_base + 0x1220040 ||
                   obj_type == g_game_base + 0x12200a0) {
            if (poison_count >= 0x30) {
                collect_ok = 0;
                break;
            }
            if (poison_record_read(g_game_base, gid, &g_dodge_poison_table) != 0)
                poison_count++;
        } else if (obj_type != g_game_base + 0x121fff8 &&
                   obj_type != g_game_base + 0x12205d0) {
            collect_ok = 0;
            break;
        }
    }

    if (collected != 0) {
        uint32_t kept = 0;
        for (uint32_t i = 0; i < collected; i++) {
            if (projectile_classify(g_projectile_records[i].f[1]) == 0) {
                if (kept != i)
                    g_projectile_records[kept] = g_projectile_records[i];
                kept++;
            }
        }
        collected = kept;
    }

    g_dodge_collect_stage = collect_ok ? 0 : 4;
    if (collect_ok) {
        dodge_state_publish_b(g_dodge_state_b, &g_dodge_collect_tag,
                              &g_dodge_frame_tag);
        g_dodge_published = 1;
    }

epilogue:
    g_dodge_unlock_flag = 0;
    {
        uint64_t now = 0;
        if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0)
            now = (uint64_t)ts.tv_sec * 1000000 +
                  (uint64_t)ts.tv_nsec / 1000;
        g_dodge_actor_tick_us = now - t0;
        if (g_dodge_max_actor_tick_us < g_dodge_actor_tick_us)
            g_dodge_max_actor_tick_us = g_dodge_actor_tick_us;
    }

    uint64_t mono = 0;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0)
        mono = (uint64_t)ts.tv_sec * 1000 + (uint64_t)ts.tv_nsec / 1000000;

    int status_changed = (g_dodge_last_status_bits != g_dodge_stats_word_a) ||
                         (g_dodge_active != g_dodge_last_status_change_ms);
    if (g_dodge_frame_publications < 9 || status_changed ||
        mono - g_dodge_last_status_ms >= 1000) {
        char buf[0xc80];
        int n = snprintf(buf, sizeof buf,
            ",\"action\":22,\"publication\":%llu,\"source_tick\":%u,"
            "\"epoch\":%llu,\"settings_generation\":%llu,\"requested\":%u,"
            "\"version\":%u,\"active_known\":%u,\"active\":%u,"
            "\"ended\":%u,\"own_gid\":%d,\"own_x\":%d,\"own_y\":%d,"
            "\"collect_stage\":%u,\"collect_index\":%u,"
            "\"object_count\":%u,\"projectile_count\":%u,"
            "\"actor_count\":%u,\"actors_complete\":%u,"
            "\"coverage_known\":%u,\"wall_known\":%u,"
            "\"poison_known\":%u,\"hazard_known\":%u,"
            "\"classifier_ready\":%u,\"classifier_reason\":%u,"
            "\"consumer_reason\":%u,\"proposal_kind\":%u,"
            "\"proposal_reason\":%u,\"live_threats\":%u,"
            "\"begin_reason\":%u,\"apply_reason\":%u,\"pending\":%u,"
            "\"local_calls\":%llu,\"queue_calls\":%llu,"
            "\"target_x\":%d,\"target_y\":%d,"
            "\"local_permission_calls\":%llu,"
            "\"queue_permission_calls\":%llu,"
            "\"local_permission_refusals\":%llu,"
            "\"queue_permission_refusals\":%llu,\"duration_us\":%llu,"
            "\"native_acceptance_proven\":false",
            (unsigned long long)g_dodge_publication,
            (uint32_t)g_dodge_source_tick,
            (unsigned long long)g_dodge_epoch,
            (unsigned long long)g_dodge_settings_gen,
            g_dodge_requested, g_dodge_version,
            (uint32_t)g_dodge_active_known, (uint32_t)g_dodge_active,
            (uint32_t)g_dodge_ended, g_dodge_own_gid, g_dodge_own_x,
            g_dodge_own_y, g_dodge_collect_stage, g_dodge_collect_index,
            g_dodge_object_count, (uint32_t)g_dodge_proj_count,
            g_dodge_actor_count, collected, g_dodge_coverage_known,
            g_dodge_wall_known, poison_count, g_dodge_hazard_known,
            g_dodge_classifier_ready, g_dodge_classifier_reason,
            g_dodge_consumer_reason, g_dodge_proposal_kind,
            g_dodge_proposal_reason, g_dodge_live_threats,
            g_dodge_begin_reason, g_dodge_apply_reason, g_dodge_pending,
            (unsigned long long)g_dodge_local_calls,
            (unsigned long long)g_dodge_queue_calls, g_dodge_target_x,
            g_dodge_target_y, (unsigned long long)g_dodge_local_perm_calls,
            (unsigned long long)g_dodge_queue_perm_calls,
            (unsigned long long)g_dodge_local_perm_refusals,
            (unsigned long long)g_dodge_queue_perm_refusals,
            (unsigned long long)g_dodge_actor_tick_us);
        if (n > 0 && (size_t)n < sizeof buf) {
            size_t len = (size_t)n;
            uint32_t logged =
                ((uint32_t)g_dodge_proj_count < 4) ? (uint32_t)g_dodge_proj_count : 4;
            int m = snprintf(buf + len, sizeof buf - len,
                             ",\"projectiles_omitted\":%u,\"raw_projectiles\":[",
                             (uint32_t)g_dodge_proj_count - logged);
            if (m > 0 && (size_t)m < sizeof buf - len) {
                len += (size_t)m;
                for (uint32_t i = 0; i < logged; i++) {
                    m = snprintf(buf + len, sizeof buf - len,
                                 "%s{\"gid\":%u,\"known\":%u,\"z\":%u,"
                                 "\"team_a\":%u,\"team_b\":%u,"
                                 "\"bouncing\":%u,\"wall_los\":%d}",
                                 (i == 0) ? ROD_JOURNAL_EMPTY
                                          : ROD_JOURNAL_COMMA,
                                 (unsigned)g_projectile_records[i].f[0],
                                 (unsigned)g_projectile_records[i].f[1],
                                 (unsigned)g_projectile_records[i].f[2],
                                 (unsigned)g_projectile_records[i].f[3],
                                 (unsigned)g_projectile_records[i].f[4],
                                 (unsigned)g_projectile_records[i].f[5],
                                 (int)g_projectile_records[i].f[6]);
                    if (m <= 0 || (size_t)m >= sizeof buf - len)
                        goto phase;
                    len += (size_t)m;
                }
                if (sizeof buf - len >= 2) {
                    buf[len] = ']';
                    buf[len + 1] = '\0';
                    runtime_journal_write("dodge_frame",
                                          "single_consumer_frame", buf);
                    g_dodge_last_status_ms = mono;
                }
            }
        }
    }

phase:
    if (g_dodge_active != 0 &&
        (g_dodge_last_status_change_ms == 0 || status_changed ||
         (mono - g_dodge_last_status_change_ms) >> 3 > 0x270)) {
        char buf[0x14a];
        snprintf(buf, sizeof buf,
                 ",\"action\":22,\"publication\":%llu,\"epoch\":%llu,"
                 "\"active\":%u,\"ended\":%u,\"local_calls\":%llu,"
                 "\"queue_calls\":%llu",
                 (unsigned long long)g_dodge_publication,
                 (unsigned long long)g_dodge_epoch,
                 (uint32_t)g_dodge_active, (uint32_t)g_dodge_ended,
                 (unsigned long long)g_dodge_local_calls,
                 (unsigned long long)g_dodge_queue_calls);
        runtime_journal_write("dodge_phase",
                              "phase_and_cumulative_operations", buf);
        g_dodge_last_status_ms = mono;
    }
    g_dodge_last_status_bits = (uint32_t)g_dodge_stats_word_a;
    g_dodge_last_status_change_ms = g_dodge_active;
}

static int ignore_words_valid(const uint32_t words[8])
{
    if (words[0] != IGNORE_MAGIC || words[1] != 1 || words[2] != IGNORE_BITS)
        return 0;
    uint32_t t = words[4] ^ rol5(words[3] ^ IGNORE_SEED);
    t = words[5] ^ rol5(t);
    return words[7] == (words[6] ^ rol5(t));
}

uint32_t projectile_classify(uint64_t addr)
{
    struct timespec ts;
    uint64_t now = 0;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0)
        now = (uint64_t)ts.tv_sec * 1000 + (uint64_t)ts.tv_nsec / 1000000;

    if (now != 0 && (g_ignore_read_ms == 0 || now - g_ignore_read_ms > 0xf9)) {
        uint64_t expected = g_ignore_read_ms;
        if (__atomic_compare_exchange_n(&g_ignore_read_ms, &expected, now,
                                        false, __ATOMIC_ACQ_REL,
                                        __ATOMIC_ACQUIRE) != 0 &&
            g_files_dir_fd >= 0) {
            int dir = openat(g_files_dir_fd, "nexus-menu", 0x8c000);
            if (dir >= 0) {
                int fd = openat(dir, "autododge-ignore.bin", 0x88000);
                close(dir);
                if (fd >= 0) {
                    struct stat sb;
                    uint32_t words[8];
                    if (fstat(fd, &sb) == 0 && S_ISREG(sb.st_mode) &&
                        sb.st_uid == getuid() && sb.st_nlink == 1 &&
                        sb.st_size == 0x20) {
                        ssize_t r;
                        do {
                            r = pread(fd, words, 0x20, 0);
                        } while (r < 0 && errno == EINTR);
                        if (r == 0x20 && ignore_words_valid(words) &&
                            words[6] < 0x1000) {
                            g_ignore_mask[0] = (uint64_t)words[3] |
                                               ((uint64_t)words[4] << 32);
                            g_ignore_mask[1] = (uint64_t)words[5] |
                                               ((uint64_t)words[6] << 32);
                        }
                    }
                    close(fd);
                }
            }
        }
    }

    if (g_ignore_mask[0] == 0 && g_ignore_mask[1] == 0)
        return 0;

    uint64_t obj = 0;
    uint32_t count = 0;
    uint64_t arr = 0, lst = 0, ent = 0;
    if (game_read(0, addr + 8, &obj, 8) == 0 ||
        obj + 0x800 < 0x12000 || (obj & 7) != 0)
        return 0;
    if (game_read(0, obj + 2, &count, 4) == 0 ||
        (int32_t)count < 0 || (int32_t)count > 0x2710)
        return 0;
    if (game_read(0, obj, &arr, 8) == 0 ||
        arr + 0x2000 < 0x12000 || (arr & 7) != 0)
        return 0;
    if (game_read(0, arr + 0x38, &lst, 8) == 0 ||
        lst + 0x2000 < 0x12000 || (lst & 7) != 0)
        return 0;
    if (game_read(0, lst, &ent, 8) == 0 ||
        ent + 0x2000 < 0x12000 || (ent & 7) != 0)
        return 0;

    uint32_t desc[4] = {0, 0, 0, 0};
    if (game_read(0, ent + (uint64_t)(int32_t)count * 0x10, desc, 0x10) == 0)
        return 0;
    if (desc[1] - 0x40 > 0xffffffc0u)
        return 0;

    char name[0x48];
    if (desc[1] < 8) {
        memcpy(name, &desc[2], desc[1] + 1);
    } else {
        if (game_read(0, (uintptr_t)desc[2], name, desc[1] + 1) == 0)
            return 0;
    }

    size_t len = strlen(name);
    int printable = 1;
    for (size_t i = 0; i < len; i++) {
        uint8_t c = (uint8_t)name[i];
        if ((uint32_t)(c & 0xffffffdfu) - 0x41u > 0x19u && c != 0x5f &&
            (uint32_t)c - 0x30u > 9u) {
            printable = 0;
            break;
        }
    }
    if (!printable)
        return 0;

    size_t lo = 0, hi = 0x13e;
    while (lo < hi) {
        size_t mid = lo + ((hi - lo) >> 1);
        int r = strcmp(name, g_projectile_names[mid]);
        if (r == 0) {
            uint16_t bit = g_projectile_name_bits[mid];
            return (uint32_t)((g_ignore_mask[bit >> 6] >> (bit & 0x3f)) & 1);
        }
        if (r > 0)
            lo = mid + 1;
        else
            hi = mid;
    }
    return 0;
}

extern void *g_game_image;
extern int (*g_ng_read_fn)(void *, uintptr_t, void *, uint32_t);
extern void *g_ng_read_ctx;
extern int32_t (*g_ng_get_requested_fn)(const char *);
extern int32_t (*g_ng_dodge_profile_fn)(uint32_t, void *);
extern uintptr_t game_deref(uintptr_t handle, uintptr_t addr);
extern int entity_position_read(uintptr_t handle, uintptr_t entity,
                                int32_t *x, int32_t *y, int32_t *z);
extern int entity_team_read(uintptr_t handle, uintptr_t entity,
                            uintptr_t world, int32_t *team);

extern volatile int g_ng_lock;
extern uint32_t g_ng_bound;
extern uint32_t g_ng_reason;
extern uint32_t g_ng_ready;
extern uint32_t g_ng_stable_frames;
extern uint32_t g_ng_projectiles;
extern uint32_t g_ng_feed_complete;
extern uint32_t g_ng_requested;
extern uint32_t g_ng_version;
extern uint64_t g_ng_accepted;
extern uint64_t g_ng_rejects;
extern uint64_t g_ng_attempts;
extern uint64_t g_ng_sequence;
extern uint64_t g_ng_frame_a;
extern uint64_t g_ng_frame_b;
extern uint64_t g_ng_frame_c;
extern uint64_t g_ng_frame_d;
extern uint64_t g_ng_frame_e;
extern uint64_t g_ng_frame_f;
extern uint64_t g_ng_threats[THREAT_SLOTS][5];
extern uint32_t g_ng_threat_count;
extern uint32_t g_ng_unknown;

int ng_feed_v1(int *record)
{
    spin_lock_acquire(&g_ng_lock);
    int ret = 0;
    uint32_t reason = 0;

    do {
        if (g_ng_read_fn == NULL) {
            reason = 1;
            break;
        }
        if (record == NULL ||
            *(const uint64_t *)(const void *)record != ROD_NG_FEED_TAG) {
            reason = 2;
            break;
        }
        uint64_t frame_static = *(const uint64_t *)(const void *)(record + 2);
        uint64_t own_entity = *(const uint64_t *)(const void *)(record + 3);
        uint64_t sequence = *(const uint64_t *)(const void *)(record + 4);
        if (frame_static + 0x2000 < 0x12000 || (frame_static & 7) != 0) {
            reason = 2;
            break;
        }
        if (own_entity + 0x2000 < 0x12000 || (own_entity & 7) != 0) {
            reason = 2;
            break;
        }
        if (sequence == 0 || sequence <= g_ng_sequence) {
            reason = 8;
            break;
        }

        uintptr_t frame = game_deref(0, (uintptr_t)g_game_image + 0x1307e20);
        if (frame == 0 || frame + 0x54 < 0x10004) {
            reason = 3;
            break;
        }
        uint32_t kind = 0;
        if (g_ng_read_fn(g_ng_read_ctx, frame + 0x50, &kind, 4) != 1 ||
            kind != 5) {
            reason = 3;
            break;
        }
        uintptr_t session = game_deref(0, frame + 0x48);
        if (session == 0) {
            reason = 3;
            break;
        }
        uintptr_t session_b = game_deref(0, frame_static + 0x918);
        if (session_b != session) {
            reason = 4;
            break;
        }
        uintptr_t world = game_deref(0, game_deref(0, session_b + 0x28));
        if (world == 0 || world + 0x104 < 0x10004) {
            reason = 4;
            break;
        }
        uint32_t obj_count = 0;
        if (g_ng_read_fn(g_ng_read_ctx, world + 0x100, &obj_count, 4) != 1 ||
            (int32_t)obj_count < 1) {
            reason = 4;
            break;
        }
        uintptr_t proj_list = game_deref(0, world + 0x28);
        if (proj_list == 0 || proj_list + 0x10 < 0x10004) {
            reason = 4;
            break;
        }
        uint32_t proj_count = 0;
        if (g_ng_read_fn(g_ng_read_ctx, proj_list + 0xc, &proj_count, 4) != 1 ||
            (int32_t)proj_count < 1 || (int32_t)proj_count > 0x200) {
            reason = 4;
            break;
        }
        uintptr_t proj_arr = game_deref(0, proj_list);
        if (proj_arr < 0x10000 ||
            proj_arr + ((uint64_t)(int32_t)proj_count << 3) < 0x10000) {
            reason = 4;
            break;
        }
        uintptr_t world_b = game_deref(0, world + 0xf8);
        if (world_b == 0) {
            reason = 4;
            break;
        }
        uintptr_t clock_obj = game_deref(0, session + 0x58);
        if (clock_obj == 0) {
            reason = 4;
            break;
        }
        uint64_t gids[0x200];
        if (g_ng_read_fn(g_ng_read_ctx, proj_arr, gids,
                         (uint32_t)proj_count * 8) != 1) {
            reason = 4;
            break;
        }

        uint32_t own_hits = 0;
        for (uint32_t i = 0; i < proj_count; i++)
            if (gids[i] == own_entity)
                own_hits++;
        if (own_hits != 1 || own_entity + 0xc < 0x10004) {
            reason = 5;
            break;
        }
        uint32_t own_stamp = 0;
        if (g_ng_read_fn(g_ng_read_ctx, own_entity + 8, &own_stamp, 4) != 1 ||
            own_stamp != obj_count) {
            reason = 5;
            break;
        }
        uintptr_t own_vtable = game_deref(0, own_entity);
        if (own_vtable != (uintptr_t)g_game_image + 0x1220040 &&
            own_vtable != (uintptr_t)g_game_image + 0x12200a0) {
            reason = 6;
            break;
        }
        uintptr_t own_extra = game_deref(0, own_entity + 0x10);
        if (game_deref(0, own_extra) != (uintptr_t)g_game_image + 0x121c438 ||
            own_extra + 0x2b0 < 0x10004) {
            reason = 6;
            break;
        }
        uint32_t ping = 0;
        if (g_ng_read_fn(g_ng_read_ctx, own_extra + 0x2ac, &ping, 4) != 1 ||
            (int32_t)ping < 1 || (int32_t)ping > 0x5dc) {
            reason = 6;
            break;
        }
        int32_t ox = 0, oy = 0, oz = 0;
        if (entity_position_read(0, own_entity, &ox, &oy, &oz) == 0 ||
            entity_team_read(0, own_entity, world, (int32_t *)&own_stamp) == 0) {
            reason = 6;
            break;
        }

        int32_t requested = g_ng_get_requested_fn("autododgeEnabled");
        int32_t version = g_ng_get_requested_fn("dodgeVersion");
        if (requested > 1 || version < 1 || version > 5) {
            reason = 9;
            break;
        }
        int32_t profile[PROFILE_SLOT_COUNT];
        if (g_ng_dodge_profile_fn((uint32_t)version, profile) != 1) {
            reason = 9;
            break;
        }
        if (profile[0] != version ||
            g_ng_get_requested_fn("dodgeVersion") != version ||
            g_ng_get_requested_fn("autododgeEnabled") != requested) {
            reason = 9;
            break;
        }
        if ((int32_t)ping < 0) {
            reason = 9;
            break;
        }

        uint32_t threats = 0;
        uint32_t unknown = 0;
        for (uint32_t i = 0; i < proj_count; i++) {
            uint64_t gid = gids[i];
            if (gid == own_entity)
                continue;
            uintptr_t obj_type;
            if (gid + 0x2000 < 0x12000 || (gid & 7) != 0 ||
                (obj_type = game_deref(0, gid)) == 0) {
                reason = 0;
                unknown++;
                continue;
            }
            if (obj_type == (uintptr_t)g_game_image + 0x12206a8) {
                uintptr_t rec = game_deref(0, gid + 0x10);
                if (game_deref(0, rec) != (uintptr_t)g_game_image + 0x121ea60) {
                    reason = 0;
                    continue;
                }
                uint32_t hp = 0;
                int32_t tx = 0, ty = 0, tz = 0;
                int32_t team = 0;
                uint32_t speed = 0, radius = 0;
                uint8_t cls = 0;
                if (g_ng_read_fn(g_ng_read_ctx, gid + 8, &hp, 4) != 1 ||
                    (int32_t)hp <= 0 ||
                    entity_position_read(0, gid, &tx, &ty, &tz) == 0 ||
                    entity_team_read(0, gid, world, &team) == 0 ||
                    rec + 0x128 < 0x10004 ||
                    g_ng_read_fn(g_ng_read_ctx, rec + 0x124, &speed, 4) != 1 ||
                    rec + 0x134 < 0x10004 ||
                    g_ng_read_fn(g_ng_read_ctx, rec + 0x130, &radius, 4) != 1 ||
                    rec + 0x1db < 0x10001 ||
                    g_ng_read_fn(g_ng_read_ctx, rec + 0x1da, &cls, 1) != 1) {
                    reason = 0;
                    continue;
                }
                if (tx >= ox && tx - ox <= 600) {
                    if (team == (int32_t)own_stamp) {
                        threats++;
                    } else if (team >= 0 && (int32_t)ping >= 0 && cls < 2) {
                        if (threats == THREAT_SLOTS) {
                            reason = 0;
                        } else {
                            g_ng_threats[threats][0] = gid;
                            g_ng_threats[threats][1] = (uint32_t)tx;
                            g_ng_threats[threats][2] = (uint32_t)ty;
                            g_ng_threats[threats][3] = (uint32_t)tz;
                            g_ng_threats[threats][4] =
                                (uint64_t)radius | ((uint64_t)speed << 32) |
                                ((uint64_t)cls << 8);
                            threats++;
                        }
                    }
                }
            } else if (obj_type != (uintptr_t)g_game_image + 0x1220040 &&
                       obj_type != (uintptr_t)g_game_image + 0x12200a0) {
                unknown++;
            }
        }

        if (game_deref(0, frame + 0x48) != session ||
            game_deref(0, frame_static + 0x918) != session ||
            game_deref(0, game_deref(0, game_deref(0, frame_static + 0x918) +
                                            0x28)) != world ||
            game_deref(0, game_deref(0, session_b + 0x28)) != world) {
            reason = 7;
            break;
        }

        uint32_t stable;
        if (g_ng_frame_a == frame_static && g_ng_frame_b == session &&
            g_ng_frame_c == world && g_ng_frame_d == own_entity &&
            g_ng_frame_e == obj_count && g_ng_frame_f == clock_obj) {
            stable = (g_ng_stable_frames < 4) ? g_ng_stable_frames + 1 : 4;
        } else {
            stable = 1;
            g_ng_attempts++;
        }

        g_ng_bound = 1;
        g_ng_reason = reason;
        g_ng_ready = (reason == 0 && stable >= 3) ? 1u : 0u;
        g_ng_stable_frames = stable;
        g_ng_projectiles = proj_count;
        g_ng_threat_count = threats;
        g_ng_unknown = unknown;
        g_ng_feed_complete = (reason == 0) ? 10u : 0u;
        g_ng_requested = (uint32_t)requested;
        g_ng_version = (uint32_t)version;
        g_ng_frame_a = frame_static;
        g_ng_frame_b = session;
        g_ng_frame_c = world;
        g_ng_frame_d = own_entity;
        g_ng_frame_e = obj_count;
        g_ng_frame_f = clock_obj;
        g_ng_sequence = sequence;
        g_ng_accepted++;
        ret = 1;
    } while (0);

    if (ret == 0) {
        g_ng_bound = 0;
        g_ng_reason = reason;
        g_ng_rejects++;
        g_ng_attempts++;
    }
    spin_lock_release(&g_ng_lock);
    return ret;
}

uint64_t ng_diagnostics(char *out, size_t cap)
{
    if (out == NULL || cap == 0)
        return 0;

    spin_lock_acquire(&g_ng_lock);
    int n = snprintf(out, cap,
        "{\"schema\":\"nexus-evasion-game-passive/v1\",\"bound\":%u,"
        "\"accepted\":%llu,\"rejected\":%llu,\"sequence\":%llu,"
        "\"reason\":%u,\"stable_frames\":%u,\"passive_ready\":%u,"
        "\"projectiles\":%u,\"feed_complete\":%u,"
        "\"requested_autododge\":%d,\"requested_version\":%d,"
        "\"movement_enabled\":false,\"gameplay_effective\":false}",
        g_ng_bound, (unsigned long long)g_ng_accepted,
        (unsigned long long)g_ng_rejects, (unsigned long long)g_ng_sequence,
        g_ng_reason, g_ng_stable_frames, g_ng_ready, g_ng_projectiles,
        g_ng_feed_complete, g_ng_requested, g_ng_version);
    spin_lock_release(&g_ng_lock);

    if (n < 0)
        return 0;
    return (uint64_t)n + 1;
}

extern int32_t g_ui_files_dir_fd;
extern const char *g_ui_key_table[0x76 * 3];
extern int32_t g_ui_key_values[0x76];

bool ui_ignore_mask_read(uint32_t words[8])
{
    if (words == NULL || g_ui_files_dir_fd < 0)
        return false;

    for (int i = 0; i < 8; i++)
        words[i] = 0;

    int fd = openat(g_ui_files_dir_fd, "autododge-ignore.bin", 0x88000);
    if (fd < 0) {
        if (errno == ENOENT) {
            *(uint64_t *)(void *)words = ROD_UI_IGNORE_HDR;
            words[2] = IGNORE_BITS;
            uint32_t t = words[4] ^ rol5(words[3] ^ IGNORE_SEED);
            t = words[5] ^ rol5(t);
            words[7] = words[6] ^ rol5(t);
            return true;
        }
        return false;
    }

    struct stat sb;
    bool ok = false;
    if (fstat(fd, &sb) == 0 && S_ISREG(sb.st_mode) &&
        sb.st_uid == getuid() && sb.st_nlink == 1 && sb.st_size == 0x20) {
        ssize_t r;
        do {
            r = pread(fd, words, 0x20, 0);
        } while (r < 0 && errno == EINTR);
        if (r == 0x20)
            ok = ignore_words_valid(words);
    }
    close(fd);
    return ok;
}

bool ui_ignore_mask_write(void *ctx, uint64_t arg, uint32_t bit, uint8_t on)
{
    (void)ctx;
    (void)arg;
    if (bit >= IGNORE_BITS || on >= 2)
        return false;

    uint32_t words[8];
    if (!ui_ignore_mask_read(words))
        return false;

    uint32_t mask = 1u << (bit & 0x1f);
    if (on == 1)
        words[3 + (bit >> 5)] |= mask;
    else
        words[3 + (bit >> 5)] &= ~mask;

    words[1] = words[0] ^ rol5(words[0] ^ 0xba0d24b4u);
    words[2] ^= rol5(words[1]);
    words[3] ^= rol5(words[2]);
    words[4] ^= rol5(words[3]);
    words[5] ^= rol5(words[4]);
    words[7] = words[6] ^ rol5(words[5]);

    if (g_ui_files_dir_fd < 0)
        return false;

    char path[0x50];
    struct timespec ts;
    uint64_t mono = 0;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0)
        mono = (uint64_t)ts.tv_sec * 1000 + (uint64_t)ts.tv_nsec / 1000000;
    snprintf(path, sizeof path, "autododge-ignore-%d-%llu.tmp",
             (int)getpid(), (unsigned long long)mono);

    int fd = openat(g_ui_files_dir_fd, path, 0x88241, 0x180);
    if (fd < 0)
        return false;

    size_t done = 0;
    while (done < IGNORE_SIZE) {
        ssize_t w = write(fd, (char *)words + done, IGNORE_SIZE - done);
        if (w < 0) {
            if (errno == EINTR)
                continue;
            break;
        }
        if (w == 0)
            break;
        done += (size_t)w;
    }
    if (done != IGNORE_SIZE) {
        close(fd);
        unlinkat(g_ui_files_dir_fd, path, 0);
        return false;
    }
    int fs = fsync(fd);
    close(fd);
    if (fs != 0)
        goto remove;
    if (renameat(g_ui_files_dir_fd, path, g_ui_files_dir_fd,
                 "autododge-ignore.bin") != 0)
        goto remove;
    if (fsync(g_ui_files_dir_fd) != 0)
        goto remove;
    return true;

remove:
    unlinkat(g_ui_files_dir_fd, path, 0);
    return false;
}

uint32_t ui_dodge_key_normalize(void *menu_entry)
{
    const char *name = *(const char **)((char *)menu_entry + 0x20);
    if (name == NULL)
        return 0xffffffffu;

    if (strncmp(name, "nexus_dodge_", 0xc) == 0 &&
        strstr(name, "blacklist") == NULL) {
        int version = 3;
        int found = 0;
        for (int i = 0; i < 0x76; i++) {
            if (strcmp(g_ui_key_table[i * 3], "nexus_autododge_version") == 0) {
                version = g_ui_key_values[i];
                found = 1;
                break;
            }
        }
        (void)found;
        if (version > 1) {
            char buf[0x60];
            snprintf(buf, sizeof buf, "nexus_v%d_dodge_%s", version, name + 0xc);
            for (int i = 0; i < 0x76; i++) {
                if (strcmp(g_ui_key_table[i * 3], buf) == 0)
                    return (uint32_t)i;
            }
        }
    }
    return *(uint32_t *)((char *)menu_entry + 0x38);
}

uint32_t ui_dodge_version_current(void)
{
    for (int i = 0; i < 0x76; i++) {
        if (strcmp(g_ui_key_table[i * 3], "nexus_autododge_version") == 0)
            return (uint32_t)g_ui_key_values[i];
    }
    return 3;
}
