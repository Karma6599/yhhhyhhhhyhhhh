/* renderer p1 chunk 1: covers raw lines 1-280 */
/*
 * renderer — UI subsystem
 * Decompiled with Ghidra 11.3.2 (arm64 pseudocode) from: libNexusUI69252.so
 * Related menu entries (from embedded nexus-overlay-wire/v1):
 *   - menu.killaura "Kill aura" [free]
 *   - menu.autododge "Auto dodge" [free]
 *   - menu.follow "Follow" [Nexus+ PAID]
 *   - menu.aim "Smart aim" [free]
 *   - menu.xray "X-Ray" [Nexus+ PAID]
 *   - menu.hold "Hold fire" [free]
 *   - menu.spin "Spin" [Nexus+ PAID]
 *   - killauraEnabled "Kill aura" [free]
 *   - aopPredictEnabled "Prediction" [free]
 *   - killauraMainAttack "Main attack" [free]
 *   - killauraNoWall "Wall check" [free]
 *   - killauraNoBall "Ignore ball" [free]
 *   - autododgeEnabled "Auto dodge" [free]
 *   - aopAimEnabled "Smart aim" [free]
 *   - isSpinEnabled "Spin" [Nexus+ PAID]
 *   - followEnabled "Follow" [Nexus+ PAID]
 *   - followClosestAllyEnabled "Closest ally" [Nexus+ PAID]
 *   - ballAssistEnabled "Ball assist" [Nexus+ PAID]
 *   - holdToShootEnabled "Hold fire" [free]
 *   - isXrayEnabled "X-Ray" [Nexus+ PAID]
 *   - espEnabled "ESP" [Nexus+ PAID]
 *   - characterOutlineEnabled "Character outline" [free]
 *   - attackRangeIndicator "Attack range" [Nexus+ PAID]
 *   - hitboxRenderer "Hitboxes" [Nexus+ PAID]
 *   - enemyTracer "Enemy tracer" [Nexus+ PAID]
 *   - trophiesAboveHead "Trophies" [Nexus+ PAID]
 *   - pinEnabled "Auto pin" [Nexus+ PAID]
 *   - sprayEnabled "Auto spray" [Nexus+ PAID]
 *   - ... +205 more (see docs/feature_list.json)
 * Notes: nexus_rich_* immediate-mode renderer + color/diagnostics.
 */

/*
 * Reconstructed (renderer p1, chunk 1 — raw lines 1-280): launcher_stage_wait
 * @ 001608e0 — hold the launcher stage and attach the rich view. The remaining
 * p1 definitions (raw lines 37-210, 330+) follow in later chunks.
 */

#define _GNU_SOURCE

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>
#include <time.h>

/* ===== cross-file externs (names per ui_name_map.md / ui_conventions.md) ===== */

/* misc.c — remote memory + SC object helpers */
extern int      proc_mem_read(long handle, uint64_t addr, void *out, uint64_t len);
                                      /* misc.c @ 0014e7c4 — raw arg 1 is a dead x0 slot, body ignores it */
extern uint64_t remote_read_ptr(uint64_t addr);
                                      /* misc.c @ 00154cf0 — read remote qword, sanity-gated as pointer */
extern int      remote_child_link_valid(uint64_t node, uint64_t parent);
                                      /* misc.c @ 00154d5c — node->parent==parent && slot[index]==node */
extern int      sc_object_release(uint64_t obj);
                                      /* misc.c @ 0014e5ac — validate free SC node, clear +0x218 state */
extern int      sc_object_free_check_local(uint64_t obj);
                                      /* misc.c @ 00164090 — SC-object free check (global-base variant) */
extern bool     on_main_pool_thread(void);
                                      /* misc.c @ 0014e720 — true when thread name is "MainPool" */
extern int      style_bundle_lookup(uint64_t ctx, const char *name);
                                      /* misc.c @ 00163eb8 — "sc/ui.sc" bundle style table lookup */

/* misc.c — theme/menu status */
extern bool     theme_menu_id_claim(int menu_id);
                                      /* misc.c @ 00183c78 — latch test-and-set on g_theme_latch */
extern uint64_t theme_menu_status_set(uint32_t code, int menu_id, const char *text);
                                      /* misc.c @ 00183cdc — status code + formatted status line */

/* menu_engine.c */
extern void     menu_stage_log(uint64_t stage, const char *event, const char *action,
                               uint32_t value);
                                      /* menu_engine.c @ 00156760 — logcat 'NexusLab69252' JSON */
extern int      menu_launcher_style_apply(int screen, const void *style_pair);
                                      /* menu_engine.c @ 00160c3c — launcher button per screen */

/* renderer.c part 2 (defined later in this file's p2 chunks) */
extern void          *nexus_rich_renderer_ops_v1(void);
                                      /* renderer.c @ 00176ac8 — returns ops v1 table, 0x28 bytes */
extern unsigned long nexus_rich_validate_native(void);
                                      /* renderer.c @ 00176904 — native-side guard check */

/* shared globals owned by other files (address comment = join key) */
extern uint64_t g_proc_mem_bias;          /* 0x22d8e0 — misc.c: game image base / remote-read bias */
extern uint32_t g_native_guards_armed;    /* 0x22f4c0 — widgets.c: game-state latch gating native guards */
extern const uint64_t LAUNCHER_GUARD_MAGIC; /* 0x10f730 — rodata: magic qword leading the guard bundle */

/* ===== launcher stage state (owned by this part; address comment = join key) ===== */

/* nexus_rich renderer ops v1 table, 0x28 bytes (built by nexus_rich_renderer_ops_v1) */
typedef struct {
    int32_t abi_version;                /* +0x00 — must be 1 */
    int32_t table_bytes;                /* +0x04 — must be 0x28 */
    int   (*validate)(const void *guard_bundle); /* +0x08 — native-guard bundle check */
    void  *slot_10;                     /* +0x10 — menu_engine: metrics call */
    void  *slot_18;                     /* +0x18 — menu_engine: timestamp call */
    void  *slot_20;                     /* +0x20 */
} launcher_ops_v1_t;

/* native-guard bundle handed to launcher_ops_v1_t.validate (9 qwords) */
typedef struct {
    uint64_t    magic;                  /* LAUNCHER_GUARD_MAGIC (0x10f730) */
    uint64_t    zero;                   /* raw always stores 0 */
    uint64_t    remote_bias;            /* g_proc_mem_bias (0x22d8e0) */
    const char *build_hash_a;           /* expected native build hash */
    const char *build_hash_b;           /* expected native build hash */
    int      (*read_mem)(long handle, uint64_t addr, void *out, uint64_t len);
    bool     (*on_main_thread)(void);
    int      (*style_lookup)(uint64_t ctx, const char *name);
    void     (*stage_log)(uint64_t stage, const char *event, const char *action,
                          uint32_t value);
} launcher_guard_bundle_t;

uint64_t g_launcher_stage_root;            /* 0x22f178 — stashed stage root held across waits */
uint64_t g_launcher_stage_view;            /* 0x22db60 — stashed stage view (root child) */
uint32_t g_launcher_menu_id;               /* 0x22f038 — stashed menu id / owning tid */
int32_t  g_launcher_attached;              /* 0x22f03c — 1 attached, -1 failed, 0 pending */
uint64_t g_launcher_view;                  /* 0x22f180 — 0x260-byte rich launcher view object */
launcher_ops_v1_t *g_launcher_renderer_ops; /* 0x25ca38 — cached renderer ops v1 table */
uint32_t g_launcher_named;                 /* 0x25ca48 — bit 0: "Nexus" name registered */
uint64_t g_launcher_name_obj;              /* 0x25ca50 — SC string object set to "Nexus" */
const char *g_launcher_status = "launcher initializing";
                                           /* 0x1a7c58 — status reason (initial rodata: s_launcher_initializing) */

/* ===== launcher stage wait (shell attach) ===== */

/*
 * launcher_stage_wait — hold the launcher stage and attach the rich view.
 * Gates: stable stage identity across waits, theme-menu id claim, renderer
 * ops v1 ABI (version 1, 0x28 bytes) + native guards on first use, then a
 * live root (engine +0x12eb9f0) and view-lineage match. Allocates the
 * 0x260-byte view via the game allocator and registers "Nexus" once.
 * Returns 1 attached, 0 not ready (retry), -1 failed (g_launcher_status
 * holds the reason). Side effects: stashes stage root/view/menu id.
 * @ 001608e0
 */
int launcher_stage_wait(uint64_t stage_root, uint64_t stage_view, uint32_t menu_id)
{
    const char *reason = NULL;   /* failure reason stored into g_launcher_status */
    uint64_t style_pair[2];      /* 16 bytes read from stage_root + 0x3c */
    int result;

    if (g_launcher_view != 0 &&
        (g_launcher_stage_root != stage_root || g_launcher_stage_view != stage_view)) {
        reason = "launcher_stage_changed_during_wait";
        goto fail;
    }

    g_launcher_stage_view = stage_view;
    g_launcher_menu_id = menu_id;
    g_launcher_stage_root = stage_root;

    if (!theme_menu_id_claim(menu_id))
        return 0;                          /* id not claimable yet — retry next wait */

    if (g_launcher_renderer_ops == NULL) {
        g_launcher_renderer_ops = (launcher_ops_v1_t *)nexus_rich_renderer_ops_v1();
        if (g_launcher_renderer_ops == NULL ||
            g_launcher_renderer_ops->abi_version != 1 ||
            g_launcher_renderer_ops->table_bytes != 0x28) {
            reason = "launcher_renderer_abi";
            goto fail;
        }
        launcher_guard_bundle_t guard = {
            .magic          = LAUNCHER_GUARD_MAGIC,
            .zero           = 0,
            .remote_bias    = g_proc_mem_bias,
            .build_hash_a   = "a10aeb6b4085fb2a15d969a130cd9608231998b5269a40aed41129d99624ede3",
            .build_hash_b   = "2e7e6e69e839a8b790f2d96c9d540fc313d8481d232828283034c71d705ec8d4",
            .read_mem       = proc_mem_read,
            .on_main_thread = on_main_pool_thread,
            .style_lookup   = style_bundle_lookup,
            .stage_log      = menu_stage_log,
        };
        if (g_native_guards_armed == 0 ||
            g_launcher_renderer_ops->validate(&guard) != 1 ||
            nexus_rich_validate_native() != 1) {
            reason = "launcher_native_guards";
            goto fail;
        }
    }

    if (!proc_mem_read(1 /* dead x0 slot at the raw call sites */,
                       stage_root + 0x3c, style_pair, 0x10))
        return 0;                          /* stage memory not readable yet */

    result = menu_launcher_style_apply(0, style_pair);
    if (result != 1)
        return result;

    if (g_launcher_view == 0) {
        /* game allocator at image base + 0x11a2840: fresh 0x260-byte view */
        g_launcher_view = (uint64_t)((void *(*)(uint64_t))
                                     (g_proc_mem_bias + 0x11a2840))(0x260);
        if (g_launcher_view == 0) {
            reason = "launcher_allocation";
            goto fail;
        }
        /* engine +0x887c14 (misc.c engine_call_887c14): post-alloc bookkeeping */
        ((void (*)(void))(g_proc_mem_bias + 0x887c14))();
        if (sc_object_release(g_launcher_view) == 0 ||
            sc_object_free_check_local(g_launcher_view) == 0) {
            reason = "launcher_constructor";
            goto fail;
        }
        if ((g_launcher_named & 1) == 0) {
            /* engine +0x66ae58 (misc.c engine_call_66ae58): name it "Nexus" */
            ((void (*)(void *, const char *))(g_proc_mem_bias + 0x66ae58))(
                &g_launcher_name_obj, "Nexus");
            g_launcher_named = 1;
        }
    }

    result = menu_launcher_style_apply(0, style_pair);
    if (result != 1)
        return result;
    if (sc_object_free_check_local(g_launcher_view) == 0) {
        reason = "launcher_fresh_contract";
        goto fail;
    }

    /* live stage must still match the stashed root and view */
    if (remote_read_ptr(g_proc_mem_bias + 0x12eb9f0) != stage_root ||
        remote_read_ptr(stage_root + 0x90) != stage_view) {
        reason = "launcher_stage_changed";
        goto fail;
    }

    /* engine +0x5988ec (misc.c engine_call_5988ec): attach view to the stage */
    ((void (*)(uint64_t, uint64_t))(g_proc_mem_bias + 0x5988ec))(stage_root,
                                                                 g_launcher_view);
    if (remote_child_link_valid(g_launcher_view, stage_view) == 0) {
        reason = "launcher_attachment";
        goto fail;
    }

    g_launcher_attached = 1;
    menu_stage_log(theme_menu_status_set(2, menu_id, "building_rich_menu"),
                   "nexus_shell_launcher_attached", "current_root_verified", 0);
    return 1;

fail:
    g_launcher_status = reason;
    return -1;
}
/* renderer p1 chunk 2: covers raw lines 36-211 */
/* ===== reload-request lifecycle (script-port entry points) ===== */

/*
 * Reload-request lifecycle globals (renderer-owned; menu_engine.c externs these
 * per its part sheets — the 0xADDR comments are the join key).
 */
uint32_t g_ui_cycle_blocked;       /* 0x22d888 — bit0: all UI cycles blocked (set on unverified rollback) */
uint32_t g_reload_requested;       /* 0x22d88c — bit0: UI reload requested, awaiting menu_engine */
uint32_t g_graphics_policy_active; /* 0x22d890 — bit0: performance/graphics policy engaged */
uint32_t g_reload_request_arg;     /* 0x22d894 — zeroed when a reload is requested (menu_engine advances) */
uint32_t g_reload_phase;           /* 0x22d898 — zeroed on request and on UI release (menu_engine walks it) */
uint64_t g_reload_cookie;          /* 0x22d8a0 — zeroed when a reload is requested */
uint64_t g_reload_deadline_ms;     /* 0x22d8a8 — CLOCK_MONOTONIC ms when the reload was requested */
uint32_t g_ui_release_done;        /* 0x22db70 — set by the release path once the frame is freed */

/*
 * Graphics-cycle gate flags (renderer-owned; menu_engine.c externs 0x22dfb8/0x22dfbc).
 */
uint32_t g_graphics_cycle_ready;     /* 0x22dfb0 — 0 = cycle disabled (cleared on publish failure) */
uint32_t g_graphics_reload_pending;  /* 0x22dfb4 — debug-quality state published, reload in flight */
uint32_t g_graphics_policy_failed;   /* 0x22dfb8 — latched policy/release failure */
uint32_t g_graphics_inhibit;         /* 0x22dfbc — must be 0 for the cycle to run */
uint32_t g_graphics_block_a;         /* 0x22dff4 — must be 0 for the rollback path */
uint32_t g_graphics_block_b;         /* 0x22dff8 — set when an allocator rollback stays unverified */

/*
 * Published debug-quality state (written by nexus_script_port_ui_graphics_cycle,
 * reconciled by renderer_graphics_release).
 */
uint32_t g_debug_quality_base;   /* 0x22e000 — 1 once a quality block was published */
uint64_t g_debug_quality_ticks;  /* 0x22e004 — {tick0, tick1} pair echoed at allocator +0x88 */

/* extern (menu_engine.c-owned) */
extern uint32_t g_ui_release_armed;  /* 0x22db78 — nonzero once frame-v1 release is armed */

/* extern (misc.c-owned, registry) */
extern uint64_t g_battle_state;  /* 0x22d880 — battle/graphics state qword; bit 32 = restart required */

/* ===== cross-file externs used by this chunk ===== */

extern bool nexus_menu_server_thread(int tid);
                                      /* menu_engine.c @ 00185e94 — true when tid == g_theme_menu_id */
extern int  nexus_menu_ui_state(void *state_out, int tid);
                                      /* menu_engine.c @ 00185650 — 8-byte state out; 1 on match */
extern int  menu_sc_preflight_poll(void);
                                      /* menu_engine.c @ 00150378 — stage/thread/ui-state + 'sc/ui.sc' check */
extern int  integrity_sweep_verify(void);
                                      /* misc.c @ 001505d8 — region 0x59567c + table + 6 code regions */
extern int  allocator_state_read(void *probe, void *state_out);
                                      /* misc.c @ 00150710 — probe = allocator_probe_t*, 0x20-byte state */
extern int  debug_quality_file_write(void *block);
                                      /* misc.c @ 00150920 — 0x18-byte block -> bsd-debug-quality.bin */
extern int  allocator_state_write(long ctx, uint64_t dest, const void *src, uint64_t len);
                                      /* misc.c @ 001556ec — guarded write (allocator +0x88 / mode byte) */
extern bool maps_range_is_rw(uint64_t ctx, uint64_t addr, uint64_t len);
                                      /* misc.c @ 00155848 — /proc/self maps [addr,addr+len) is rw-p */

/*
 * allocator probe bundle: remote bias + zero ctx + the misc.c accessor pair.
 * Handed to allocator_state_read / allocator_state_toggle / allocator_state_commit.
 */
typedef struct {
    uint64_t remote_bias;   /* +0x00 g_proc_mem_bias */
    uint64_t zero_ctx;      /* +0x08 raw stores 0; passed as leading ctx to the fn ptrs */
    int    (*read_mem)(long handle, uint64_t addr, void *out, uint64_t len);        /* +0x10 proc_mem_read */
    int    (*write_mem)(long ctx, uint64_t dest, const void *src, uint64_t len);    /* +0x18 allocator_state_write */
    bool   (*maps_rw)(uint64_t ctx, uint64_t addr, uint64_t len);                   /* +0x20 maps_range_is_rw */
} allocator_probe_t;

/*
 * 0x20-byte allocator state block (out-param of allocator_state_read, misc.c).
 */
typedef struct {
    uint64_t alloc_base;  /* +0x00 resolved game allocator base */
    uint64_t field_08;    /* +0x08 tick pair echoed at allocator +0x88 */
    uint32_t field_10;    /* +0x10 */
    uint32_t _pad14;      /* +0x14 */
    uint8_t  field_18;    /* +0x18 flag byte — must be 0 to proceed */
} allocator_state_t;

/*
 * 0x18-byte debug-quality block (debug_quality_file_write payload).
 */
typedef struct {
    uint32_t base;     /* +0x00 written as 1 */
    uint32_t tick[2];  /* +0x04 snapshot fields / wrapped mod-4 counters (indexed by tick_index) */
    uint32_t tail[3];  /* +0x0c raw leaves the tail uninitialized (stack reuse) */
} debug_quality_block_t;

/*
 * nexus_script_port_ui_reload_request — request a full UI reload from the menu
 * server thread. Gates: caller is the menu server thread (tid == g_theme_menu_id),
 * nexus_menu_ui_state() == 1 with state[5] clear, and the whole reload/battle/
 * policy flag block quiet. On pass: zeroes the reload lifecycle fields, stamps
 * g_reload_deadline_ms (CLOCK_MONOTONIC ms) and sets g_reload_requested.
 * Returns 1 requested, 0 refused. Side effects: writes 0x22d894..0x22d8a8.
 * @ 0014d054
 */
int nexus_script_port_ui_reload_request(void)
{
    uint8_t state[8] = {0};   /* nexus_menu_ui_state out: +0 dword, +5/+6/+7 bytes */
    int tid = gettid();

    if (!nexus_menu_server_thread(tid))
        return 0;
    if (nexus_menu_ui_state(state, tid) != 1)
        return 0;

    if (state[5] != 0
        || ((uint32_t)(g_battle_state >> 32) & 1u) != 0   /* restart-required bit @ 0x22d884 */
        || (g_ui_cycle_blocked & 1u) != 0                 /* 0x22d888 */
        || (g_reload_requested & 1u) != 0                 /* 0x22d88c */
        || (g_graphics_policy_active & 1u) != 0)          /* 0x22d890 */
        return 0;

    g_reload_request_arg = 0;    /* 0x22d894 */
    g_reload_phase = 0;          /* 0x22d898 */
    g_reload_cookie = 0;         /* 0x22d8a0 */
    struct timespec ts;
    g_reload_deadline_ms = 0;    /* 0x22d8a8 */
    if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0)
        g_reload_deadline_ms = (uint64_t)ts.tv_sec * 1000 + (uint64_t)ts.tv_nsec / 1000000;
    g_reload_requested = 1;      /* 0x22d88c */
    return 1;
}

/*
 * nexus_release_ui_frame_v1 — frame identity check for the v1 release handshake:
 * true only for the frame at g_proc_mem_bias + 0x7316b8 once the release is armed
 * (g_ui_release_armed) and completed (g_ui_release_done). No side effects.
 * @ 0014fc7c
 */
bool nexus_release_ui_frame_v1(uint64_t frame)
{
    if (frame == g_proc_mem_bias + 0x7316b8 && g_ui_release_armed != 0)
        return g_ui_release_done != 0;
    return false;
}

/*
 * nexus_script_port_ui_graphics_cycle — per-cycle graphics policy probe.
 * tick_index (0/1) selects which mod-4 counter advances; apply == 0 stops after
 * the read/writeability probes (returns 1). Gates: both args <= 1, the whole
 * reload/battle/policy flag block quiet, menu_sc_preflight_poll() and
 * integrity_sweep_verify() pass, allocator state readable and stable across two
 * reads, allocator +0x88 and image +0x12f03e4 writable. On apply: bumps the tick
 * counter, writes the 0x18-byte debug-quality block, publishes {1, tick0, tick1}
 * to 0x22e000/0x22e004, sets g_graphics_reload_pending and requests a UI reload;
 * rolls the file and published state back if the reload is refused.
 * Returns 1 success, 0 failure (side effects: 0x22dfb0/0x22dfb4, 0x22e000-0x22e008,
 * bsd-debug-quality.bin). Preserves errno.
 * @ 001500c0
 */
int nexus_script_port_ui_graphics_cycle(uint32_t tick_index, uint32_t apply)
{
    int saved_errno = errno;   /* raw saves/restores errno around the probe */
    int result = 0;

    if ((tick_index | apply) > 1u
        || g_graphics_cycle_ready == 0
        || g_graphics_reload_pending != 0
        || (uint32_t)g_battle_state != 0
        || (g_reload_requested & 1u) != 0
        || (g_graphics_policy_active & 1u) != 0
        || (g_graphics_policy_failed & 1u) != 0
        || ((uint32_t)(g_battle_state >> 32) & 1u) != 0   /* restart-required bit @ 0x22d884 */
        || (g_ui_cycle_blocked & 1u) != 0
        || g_graphics_inhibit != 0
        || g_graphics_block_a != 0
        || g_graphics_block_b != 0)
        goto out;

    if (menu_sc_preflight_poll() == 0)
        goto out;
    if (integrity_sweep_verify() == 0)
        goto out;

    allocator_probe_t probe = {
        g_proc_mem_bias, 0, proc_mem_read, allocator_state_write, maps_range_is_rw,
    };
    allocator_state_t state_a, state_b;

    if (allocator_state_read(&probe, &state_a) == 0 || state_a.field_18 != 0)
        goto out;
    if (!maps_range_is_rw(probe.zero_ctx, state_a.alloc_base + 0x88, 8))
        goto out;
    if (!maps_range_is_rw(probe.zero_ctx, g_proc_mem_bias + 0x12f03e4, 1))
        goto out;

    if (apply == 0) {
        result = 1;   /* probe-only cycle succeeded */
        goto out;
    }

    if (allocator_state_read(&probe, &state_b) == 0)
        goto out;
    if (state_a.alloc_base != state_b.alloc_base
        || state_a.field_08 != state_b.field_08
        || state_a.field_10 != state_b.field_10
        || state_a.field_18 != state_b.field_18)
        goto out;   /* allocator state not stable across reads */

    debug_quality_block_t dq = {0};   /* raw leaves the 0x0c..0x14 tail uninitialized */
    dq.base = 1;
    dq.tick[0] = (uint32_t)state_a.field_08;
    dq.tick[1] = state_a.field_10;
    /* advance the selected counter, wrapping mod 4 (raw keeps the signed shape) */
    int32_t next = (int32_t)dq.tick[tick_index] + 1;
    uint32_t wrapped;
    if (next < 1)
        wrapped = (uint32_t)-(-next & 3);
    else
        wrapped = (uint32_t)next & 3u;
    dq.tick[tick_index] = wrapped;

    /* snapshot the published state for rollback (raw aliases this with dq's tail) */
    debug_quality_block_t backup = {0};
    backup.base = g_debug_quality_base;
    backup.tick[0] = (uint32_t)g_debug_quality_ticks;
    backup.tick[1] = (uint32_t)(g_debug_quality_ticks >> 32);

    result = 1;
    if (debug_quality_file_write(&dq) != 0) {
        g_debug_quality_base = dq.base;                       /* publish 0x22e000 */
        g_debug_quality_ticks = (uint64_t)dq.tick[0]
                              | ((uint64_t)dq.tick[1] << 32); /* publish 0x22e004 */
        g_graphics_reload_pending = 1;
        if (nexus_script_port_ui_reload_request() != 0)
            goto out;   /* keep result 1 */
        /* reload refused: roll the published state and the file back */
        g_graphics_reload_pending = 0;
        g_debug_quality_base = backup.base;
        g_debug_quality_ticks = (uint64_t)backup.tick[0]
                              | ((uint64_t)backup.tick[1] << 32);
        if (debug_quality_file_write(&backup) != 0)
            goto out_zero_result;   /* file restored — leave g_graphics_cycle_ready armed */
    } else {
        if (debug_quality_file_write(&backup) != 0)
            goto out_zero_result;   /* file restored — leave g_graphics_cycle_ready armed */
    }

out_zero_result:
    result = 0;
    g_graphics_cycle_ready = 0;   /* disable the cycle until re-armed */
out:
    errno = saved_errno;
    return result;
}
/* renderer p1 chunk 3: covers raw lines 329-799 */

/* ===== cross-file externs used by this chunk ===== */

extern uint32_t nexus_rich_main_row(void);
                                      /* menu_engine.c @ 00178b1c — current rich main-view row */
extern void nexus_menu_main_view(uint64_t view, uint32_t row, int32_t screen);
                                      /* themes.c @ 00189884 — 10-tag rich view dispatcher */
extern void nexus_menu_view(uint64_t view, int32_t screen);
                                      /* menu_engine.c @ 001856f8 — plain menu view draw */
extern int font_chooser_snapshot_read(void *out);
                                      /* misc.c @ 00191df8 — 0x30-byte font chooser snapshot */
extern int script_port_font_apply(uint32_t font_id);
                                      /* misc.c @ 00191e38 — forwards nexus_script_port_font_apply */
extern int engine_global_pair_read(int32_t *src_pair, int32_t *out_pair);
                                      /* misc.c @ 00167524 — double-read u32 + validated pair */
extern int allocator_state_toggle(void *probe, uint64_t alloc_base, uint32_t battle_lo);
                                      /* misc.c @ 00168028 — swap allocator block saved states */
extern int allocator_state_commit(void *probe, void *state_out);
                                      /* misc.c @ 00168410 — write + verify allocator state */
extern int nexus_ui_graphics_resources(uint64_t root, uint64_t view,
                                       uint64_t launcher_view, uint32_t phase);
                                      /* renderer.c p2 @ 00177afc — phase 0 verify / 1 release */

/* bionic logcat (android/log.h) — declared locally: host gcc has no NDK headers */
extern int __android_log_print(int prio, const char *tag, const char *fmt, ...);

/* rodata (misc.c / image) */
extern const uint64_t FONT_SNAPSHOT_MAGIC;  /* 0x10f748 — fonts.c name; registry calls it g_module_base_b */
extern const int32_t RICH_ASSET_OFFSETS[7]; /* 0x13cd7c — 7 offsets relative to the table itself */
extern const char RICH_ASSET_BLOCK[];       /* 0x13cd7c — asset strings base (same address as the table) */
extern const uint32_t RICH_COLOR_TABLE_DIM[];    /* 0x13cd98 */
extern const uint32_t RICH_COLOR_TABLE_BRIGHT[]; /* 0x13ce18 */

/* ===== menu view dispatch ===== */

/*
 * menu_view_draw — per-frame menu draw dispatch. Reads the menu UI state
 * (menu server thread only) and routes to the rich 10-tag main view when the
 * screen id (state[6], range 0x43..0x74) hits the bitmask 0x22006000cd001 and
 * state[5] is clear; otherwise the plain menu view. No return value.
 * @ 0016205c
 */
void menu_view_draw(uint64_t view, uint32_t screen)
{
    uint8_t state[8] = {0};

    /* raw shows a 1-arg call (w1 untracked by the decompiler); recovered as
     * gettid() — nexus_menu_ui_state only succeeds when tid == g_theme_menu_id */
    if (nexus_menu_ui_state(state, gettid()) != 1)
        return;

    uint8_t screen_id = state[6];
    if (screen_id - 0x43 < 0x32
        && ((1ULL << ((screen_id - 0x43) & 0x3f)) & 0x22006000cd001ULL) != 0
        && state[5] == 0) {
        uint32_t row = nexus_rich_main_row();
        nexus_menu_main_view(view, row, screen);
    } else {
        nexus_menu_view(view, screen);
    }
}

/* ===== font chooser commit ===== */

/*
 * 0x30-byte font-chooser snapshot (reader: font_chooser_snapshot_read, misc.c
 * @ 00191df8). Caller pre-fills the magic qword; the reader echoes status/size.
 * Note: ui/fonts.c's font_chooser_snapshot_t field guess differs from this layout.
 */
typedef struct {
    uint32_t magic;           /* +0x00 must be 1 after the read */
    uint32_t size;            /* +0x04 must be 0x30 */
    uint64_t engage_pair;     /* +0x08 low dword: engage gate; high dword gates the commit */
    uint32_t selected;        /* +0x10 current font, 0..5 */
    int32_t  reload_pending;  /* +0x14 nonzero when the game must reload for this pick */
    uint32_t bitmap;          /* +0x18 available-font bitmap, bit per font id */
    uint32_t _pad1c;          /* +0x1c */
    uint64_t field_20;        /* +0x20 */
    uint64_t field_28;        /* +0x28 */
} rich_font_snapshot_t;

rich_font_snapshot_t g_font_snapshot_cache;  /* 0x25caf0 — cached snapshot (0x30 bytes) */

/*
 * font_chooser_commit_apply — apply a committed font-chooser pick.
 * Gates: snapshot {status 1, size 0x30, engage low+high nonzero, selected < 6},
 * the font's bit set in the availability bitmap. Applies via
 * script_port_font_apply(), re-reads the snapshot (caching it when still valid
 * with selected <= 5), and requests a UI reload unless the apply already
 * cleared reload_pending for this exact selection. Returns 1 applied without
 * reload, reload_request() result otherwise, 0 on any gate failure.
 * Side effects: refreshes the 0x25caf0 snapshot cache.
 * @ 00164450
 */
int font_chooser_commit_apply(uint32_t font_id)
{
    rich_font_snapshot_t snap;
    uint64_t magic = FONT_SNAPSHOT_MAGIC;   /* 0x10f748 — caller pre-fills the qword */
    memcpy(&snap, &magic, sizeof magic);

    if (font_chooser_snapshot_read(&snap) == 0)
        return 0;
    if (snap.magic != 1 || snap.size != 0x30)
        return 0;
    if ((uint32_t)snap.engage_pair == 0 || snap.selected >= 6)
        return 0;

    g_font_snapshot_cache = snap;   /* cache 0x25caf0..0x25cb1f */

    if ((uint32_t)(snap.engage_pair >> 32) == 0)
        return 0;   /* high-dword engage gate (cached at 0x25cafc) */
    if (((snap.bitmap >> (font_id & 0x1f)) & 1u) == 0)
        return 0;   /* font not in the availability bitmap (cached at 0x25cb08) */

    uint32_t pending1 = (uint32_t)snap.reload_pending;   /* cached at 0x25cb04 */
    bool selected_is_id = snap.selected == font_id;
    bool was_pending_clear = pending1 == 0;

    if (script_port_font_apply(font_id) == 0)
        return 0;

    /* re-read: the pick may already be reflected in the snapshot */
    rich_font_snapshot_t snap2;
    uint64_t magic2 = FONT_SNAPSHOT_MAGIC;
    memcpy(&snap2, &magic2, sizeof magic2);
    bool valid2 = font_chooser_snapshot_read(&snap2) != 0
               && snap2.magic == 1
               && snap2.size == 0x30
               && (uint32_t)snap2.engage_pair != 0;
    if (valid2 && snap2.selected <= 5)
        g_font_snapshot_cache = snap2;

    if ((g_font_snapshot_cache.reload_pending == 0 || !valid2 || snap2.selected > 5)
        && selected_is_id && was_pending_clear)
        return 1;   /* applied, no reload needed */

    return nexus_script_port_ui_reload_request() != 0;
}

/* ===== graphics release ===== */

/*
 * Graphics-policy release state (renderer-owned).
 */
uint64_t g_graphics_policy_ms;        /* 0x281a20 — CLOCK_MONOTONIC ms when the policy was applied */
int32_t  g_graphics_policy_pair[2];   /* 0x281a28 — source pair for the policy probe; [1] = 0x281a2c */
int32_t  g_launcher_state_a;          /* 0x1a7c60 — set to -1 on UI release (next to g_launcher_status) */
int32_t  g_launcher_state_b;          /* 0x1a7c64 — set to -1 on UI release */
uint32_t g_launcher_reset_a;          /* 0x25ca20 — zeroed on UI release */
uint32_t g_launcher_reset_b;          /* 0x25ca28 — zeroed on UI release */
uint32_t g_launcher_reset_c;          /* 0x25ca60 — zeroed on UI release */
uint32_t g_launcher_reset_d;          /* 0x25ca68 — zeroed on UI release */
uint32_t g_stage_log_count;           /* 0x22f4bc — capped 128: shared stage-log counter (menu_stage_log) */

/* externs (menu_engine.c-owned) */
extern uint8_t  g_launcher_retry_flag;  /* 0x25ca6c — retry latch, cleared on UI release */
extern uint64_t g_launcher_retry_last;  /* 0x25ca78 — last retry identity (menu_engine compares) */

/*
 * renderer_graphics_release — verify the graphics policy then release the UI.
 * Gates: engine_global_pair_read on 0x281a28 and nexus_ui_graphics_resources(...,0)
 * both succeed. Toggles the allocator policy (after reconciling the published
 * tick pair at allocator +0x88 when a reload is pending), then releases via
 * nexus_ui_graphics_resources(...,1) and zeroes the whole launcher/reload state
 * block. Failures latch g_graphics_policy_failed and log a capped (128)
 * 'performance_graphics' JSON line with reason 'policy_unavailable' /
 * 'policy_rollback_unverified' / 'ui_release_unavailable' /
 * 'ui_release_unverified_restart_required'. Raw arg 1 is never referenced.
 * Returns 1 released, 0 otherwise. @ 0016793c
 */
int renderer_graphics_release(uint64_t unused_arg, uint64_t alloc_base)
{
    (void)unused_arg;   /* raw arg 1 unused */

    int32_t policy_pair[2];
    if (engine_global_pair_read(g_graphics_policy_pair, policy_pair) == 0)
        return 0;

    if (nexus_ui_graphics_resources(g_launcher_stage_root, g_launcher_stage_view,
                                    g_launcher_view, 0) != 1)
        return 0;

    uint32_t battle_lo = (uint32_t)g_battle_state;
    allocator_probe_t probe = {
        g_proc_mem_bias, 0, proc_mem_read, allocator_state_write, maps_range_is_rw,
    };
    allocator_state_t state, state2;
    int toggle_result;
    uint32_t *fail_flag = &g_graphics_policy_failed;
    const char *reason = "policy_unavailable";

    if (g_graphics_reload_pending == 0) {
        toggle_result = allocator_state_toggle(&probe, alloc_base, battle_lo);
    } else {
        if (g_graphics_cycle_ready != 0 && g_debug_quality_base != 0
            && g_graphics_block_a == 0 && g_graphics_block_b == 0) {
            int ok = allocator_state_read(&probe, &state);
            int result = 0;   /* raw zeroes the result on any gate failure here */
            if (ok != 0 && state.alloc_base == alloc_base && state.field_18 == 0) {
                if (maps_range_is_rw(probe.zero_ctx, alloc_base + 0x88, 8)) {
                    if (maps_range_is_rw(probe.zero_ctx, probe.remote_bias + 0x12f03e4, 1)) {
                        /* reconcile the published tick pair at allocator +0x88 */
                        uint64_t saved_alloc = state.alloc_base;
                        uint8_t  saved_flag = state.field_18;
                        uint64_t published = g_debug_quality_ticks;   /* qword at 0x22e004 */
                        bool ready;
                        if (state.field_08 == published) {
                            ready = true;
                        } else {
                            ready = allocator_state_write(probe.zero_ctx, alloc_base + 0x88,
                                                          &published, 8) != 0
                                 && allocator_state_read(&probe, &state2) != 0
                                 && state2.alloc_base == saved_alloc
                                 && state2.field_08 == published
                                 && state2.field_18 == saved_flag;
                        }
                        if (ready) {
                            toggle_result = allocator_state_toggle(&probe, alloc_base,
                                                                   battle_lo);
                            if (toggle_result == 1) {
                                g_graphics_reload_pending = 0;
                            } else if (toggle_result >= 0) {
                                toggle_result = allocator_state_commit(&probe, &state);
                                if (toggle_result == 0) {
                                    toggle_result = -1;
                                    g_graphics_block_b = 1;
                                } else {
                                    toggle_result = 0;
                                }
                            } else {
                                g_graphics_block_b = 1;
                                toggle_result = -1;
                            }
                        } else {
                            toggle_result = allocator_state_commit(&probe, &state);
                            if (toggle_result == 0) {
                                toggle_result = -1;
                                g_graphics_block_b = 1;
                            } else {
                                toggle_result = 0;
                            }
                        }
                        goto policy_block;
                    }
                }
            }
            toggle_result = result;   /* 0 — gates failed inside the reconciliation */
            goto policy_block;
        }
        *fail_flag = 1;   /* raw: *puVar8 = 1 on the outer gate failure */
        goto log_and_fail;
    }

policy_block:
    if (toggle_result != 1) {
        g_graphics_policy_failed = 1;
        if (toggle_result < 0) {
            reason = "policy_rollback_unverified";
            g_ui_cycle_blocked = 1;   /* 0x22d888 — block every UI cycle */
            goto log_and_fail;
        }
        reason = "policy_unavailable";
        goto log_and_fail;
    }

    g_graphics_policy_active = 1;
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0)
        g_graphics_policy_ms = (uint64_t)ts.tv_sec * 1000
                             + (uint64_t)ts.tv_nsec / 1000000;
    else
        g_graphics_policy_ms = 0;
    g_ui_release_done = 0;

    int release = (int)nexus_ui_graphics_resources(g_launcher_stage_root,
                                                   g_launcher_stage_view,
                                                   g_launcher_view, 1);
    if (release == 1) {
        g_launcher_view = 0;            /* 0x22f180 */
        g_launcher_attached = 0;        /* 0x22f03c */
        g_launcher_reset_b = 0;         /* 0x25ca28 */
        g_launcher_reset_a = 0;         /* 0x25ca20 */
        g_launcher_state_b = -1;        /* 0x1a7c64 */
        g_launcher_state_a = -1;        /* 0x1a7c60 */
        g_launcher_reset_c = 0;         /* 0x25ca60 */
        g_launcher_retry_flag = 0;      /* 0x25ca6c — menu_engine-owned */
        g_launcher_retry_last = 0;      /* 0x25ca78 — menu_engine-owned */
        g_launcher_reset_d = 0;         /* 0x25ca68 */
        g_reload_phase = 0;             /* 0x22d898 */
        g_reload_cookie = 0;            /* 0x22d8a0 */
        g_graphics_policy_pair[1] = (int32_t)battle_lo;   /* raw: 0x281a2c */
        return release;
    }

    g_graphics_policy_failed = 1;
    if (release < 0) {
        reason = "ui_release_unverified_restart_required";
        /* byte at 0x22d884 (battle-state high dword) := 1, 0x22d885-87 preserved */
        g_battle_state = (g_battle_state & ~0xff00000000ULL) | (1ULL << 32);
        g_launcher_attached = -1;       /* 0x22f03c = 0xffffffff */
    } else {
        reason = "ui_release_unavailable";
        g_graphics_policy_active = 0;
    }

log_and_fail:
    {
        /* capped (128) stage log — same counter as menu_stage_log (menu_engine.c) */
        uint32_t count = g_stage_log_count + 1;
        bool under_cap = g_stage_log_count < 0x80;
        g_stage_log_count = count;
        if (under_cap) {
            pid_t pid = getpid();
            uint32_t tid = (uint32_t)gettid();   /* raw passes the pid to gettid() */
            struct timespec ts2;
            uint64_t ms = 0;
            if (clock_gettime(CLOCK_REALTIME, &ts2) == 0)
                ms = (uint64_t)ts2.tv_sec * 1000 + (uint64_t)ts2.tv_nsec / 1000000;
            __android_log_print(4, "NexusLab69252",
                "{\"stage\":\"%s\",\"reason\":\"%s\",\"action\":%u,\"pid\":%d,"
                "\"tid\":%d,\"time_ms\":%llu}",
                "performance_graphics", reason, 0x20002, pid, tid, ms);
        }
    }
    return 0;
}

/* ===== rich asset / color tables ===== */

/*
 * nexus_rich_asset — map an asset index (0..6) to its rodata string via the
 * 7-entry offset table at 0x13cd7c (offsets are relative to the table itself).
 * Returns NULL for index >= 7. @ 0016a1dc
 */
const char *nexus_rich_asset(uint32_t index)
{
    if (index >= 7)
        return NULL;
    return RICH_ASSET_BLOCK + RICH_ASSET_OFFSETS[index];
}

/*
 * nexus_rich_color — immediate-mode color for a rich-view cell.
 * bright picks the {span 0xa0, coeffs -6/13, row 0x14, col 0x10, mods 8/8/8/9,
 * boost 0xff/0xff/0xff} variant and the 0x13ce18 table; dim picks
 * {span 0x60, coeffs -7/5, row 0x10, col 0xe, mods 6/7/7/7, boost
 * 0xfa/0xcd} and the 0x13cd98 table. position is divided by 0x28 (40) for the
 * sub-cell offset; level defaults to 1. Cells within 9 units of the level edge
 * are lerped toward the boost targets. Returns 0xff000000 | r<<16 | g<<8 | b.
 * @ 0016a200
 */
uint32_t nexus_rich_color(int32_t bright, int32_t cell_x, int32_t cell_y,
                          int32_t level, uint64_t position)
{
    bool hi = bright != 0;
    uint32_t span = hi ? 0xa0 : 0x60;
    int32_t coeff_a = hi ? -6 : -7;
    int32_t coeff_b = hi ? 0xd : 5;
    const uint32_t *table = hi ? RICH_COLOR_TABLE_BRIGHT : RICH_COLOR_TABLE_DIM;

    int32_t pos = (int32_t)(position / 0x28);
    int32_t cell = 0;
    if (span != 0)
        cell = pos / (int32_t)span;
    int32_t in_span = pos - cell * (int32_t)span;

    int32_t idx = coeff_a * cell_x + coeff_b * cell_y + in_span;
    int32_t q = 0;
    if (span != 0)
        q = idx / (int32_t)span;
    idx = idx - q * (int32_t)span;
    /* Ghidra idiom: wrap a negative remainder into [0, span) */
    int16_t rem = (int16_t)(((uint16_t)span & (uint16_t)(idx >> 0x1f)) + (int16_t)idx);

    int32_t row_h = hi ? 0x14 : 0x10;
    int32_t col_w = hi ? 0x10 : 0xe;
    int16_t row = 0;
    if (row_h != 0)
        row = (int16_t)(rem / row_h);
    int32_t mod_a = hi ? 8 : 6;
    int32_t mod_b = hi ? 8 : 7;

    in_span = in_span + (hi ? 9 : 7) * cell_y;
    int32_t next_row = (int8_t)(row + 1);
    rem = (int16_t)(rem - row * (int16_t)row_h);
    int32_t q2 = 0;
    if (mod_a != 0)
        q2 = next_row / mod_a;

    if (level == 0)
        level = 1;

    uint32_t c0 = table[row];
    int32_t bytes_per = (level + mod_b) * 4;
    uint32_t c1 = table[next_row - q2 * mod_a];
    int32_t q3 = 0;
    if (bytes_per != 0)
        q3 = in_span / bytes_per;

    uint32_t r0 = (c0 >> 0x10) & 0xff;
    uint32_t g0 = (c0 >> 8) & 0xff;
    int16_t dr = 0, dg = 0, db = 0;
    if (row_h != 0) {
        dr = (int16_t)((int32_t)(int16_t)(rem * (int16_t)(((c1 >> 0x10) & 0xff) - r0)) / row_h);
        dg = (int16_t)((int32_t)(int16_t)(rem * (int16_t)(((c1 >> 8) & 0xff) - g0)) / row_h);
        db = (int16_t)((int32_t)(int16_t)(rem * (int16_t)((c1 & 0xff) - (c0 & 0xff))) / row_h);
    }

    uint32_t level_off = (uint32_t)(col_w + cell_x * 4 + (q3 * bytes_per - in_span));
    int32_t r = (int32_t)r0 + dr;
    int32_t g = (int32_t)g0 + dg;
    uint32_t edge = -level_off;
    if ((int32_t)level_off > -1)
        edge = level_off;   /* abs(level_off) */
    int32_t b = (int32_t)(c0 & 0xff) + db;

    if (edge < 9) {
        /* near the level edge: lerp toward the boost targets */
        int32_t boost_g = hi ? 0xff : 0xfa;
        int32_t boost_b = hi ? 0xff : 0xcd;
        int32_t step = (9 - (int32_t)edge) * 0x12;
        r = step * (0xff - r) / 0xff + r;
        g = step * (boost_g - g) / 0xff + g;
        b = step * (boost_b - b) / 0xff + b;
    }

    return (uint32_t)((r << 0x10) | (g << 8) | b | 0xff000000);
}
/* renderer p1 chunk 4: covers raw lines 801-1311 */
/* ===== rich text layout (immediate-mode rows) ===== */

/* cross-file externs used by this chunk */
extern float font_scale_for_char(float base, int cp);
                                      /* misc.c @ 0016cca0 — width of one char scaled by charset class */

/* rodata row templates (raw reads them as qwords) */
extern const uint64_t RICH_ROW_TEMPLATE;   /* 0x10f7e0 — kind/flags qword for text rows */
extern const uint64_t RICH_ROW_PAD_XY;    /* 0x10f9b0 — x/y qword for padded empty rows */
extern const uint64_t RICH_ROW_PAD_FONT;  /* 0x10f9b8 — font-pair qword for padded empty rows */

/*
 * rich-view row record, 0xb0 stride. The view object starts with a 0x10-byte
 * header (row count at +0x00); row i lives at view + 0x10 + i*0xb0, so this
 * struct is indexed from (view + 0x10). Kept byte-faithful to the raw stores.
 */
typedef struct {
    uint32_t kind;         /* +0x00 — low dword of the template qword; 3 = free slot */
    uint32_t flags;        /* +0x04 — high dword of the template qword */
    uint32_t _r08;         /* +0x08 */
    uint32_t color;        /* +0x0c */
    uint32_t field_10;     /* +0x10 — raw stores 0 */
    float    x;            /* +0x14 */
    float    y;            /* +0x18 */
    float    font_size;    /* +0x1c — raw CONCAT44 pair: same value twice */
    float    font_size_2;  /* +0x20 */
    uint8_t  _r24[8];      /* +0x24 */
    uint8_t  active;       /* +0x2c — 1 for live rows, 0 for padded rows */
    uint8_t  font_class;   /* +0x2d — bright flag + 1 (text rows) / 0 or 2 (form rows) */
    char     text[0x82];   /* +0x2e — NUL-terminated glyph text */
} rich_row_t;              /* sizeof == 0xb0 */

#define RICH_ROW_COUNT(view)   (*(uint32_t *)(void *)(view))
#define RICH_VIEW_ROWS(view)   ((rich_row_t *)((char *)(view) + 0x10))
#define RICH_ROW_MAX           0x280u   /* 640 rows hard cap (raw: 0x27f/0x281 compares) */

/*
 * rich_utf8_next — decode one UTF-8 codepoint (the raw inlines this decode
 * twice: the measure and emit passes of rich_form_row_build). Returns the
 * sequence length (1-4); 0 on invalid input (bad lead, NUL/truncated or
 * non-continuation follower, overlong, surrogate, above U+10FFFF).
 * *cp_out receives the codepoint. @ raw 0016c470..0016c664 (inlined)
 */
static int rich_utf8_next(const uint8_t *p, uint32_t *cp_out)
{
    uint32_t cp = p[0];
    uint32_t lead = p[0];
    int len;
    uint32_t mask;
    bool is_3byte, is_4byte;

    if (lead < 0x80) {
        len = 1;
        mask = 0;
        is_3byte = is_4byte = false;
    } else {
        /* raw: 0x1d < (lead + 0x3e) & 0xff separates 0xc2-0xdf leads from the rest */
        bool wide = 0x1d < ((lead + 0x3e) & 0xff);
        bool is_2byte = !wide;
        if (wide) {
            if ((lead & 0xf0) == 0xe0) {
                is_3byte = true;
                is_4byte = false;
                mask = 0xf;
                len = 3;
            } else {
                if (((lead + 0xb) & 0xff) < 0xfb)
                    return 0;      /* lead 0x80-0xc1 or 0xf5-0xff */
                is_3byte = false;
                is_4byte = true;
                mask = 7;
                len = 4;
            }
        } else {
            is_3byte = is_4byte = false;
            mask = 0x1f;
            len = 2;
        }

        uint8_t b = p[1];
        if (b == 0 || (b & 0xc0) != 0x80)
            return 0;
        cp = (b & 0x3f) | (lead & mask) << 6;
        if (wide) {
            b = p[2];
            if (b == 0 || (b & 0xc0) != 0x80)
                return 0;
            cp = (b & 0x3f) | cp << 6;
            if (!is_3byte) {
                b = p[3];
                if (b == 0 || (b & 0xc0) != 0x80)
                    return 0;
                cp = (b & 0x3f) | cp << 6;
            }
        }
        (void)is_2byte;
    }

    /* overlong / range / surrogate rejection (raw flag dance) */
    if (cp < 0x80 && len == 2)
        return 0;
    if (cp < 0x800 && is_3byte)
        return 0;
    if (cp < 0x10000 && is_4byte)
        return 0;
    if (cp > 0x10ffff)
        return 0;
    if ((cp & 0x7ff800) == 0xd800)
        return 0;

    *cp_out = cp;
    return len;
}

/*
 * rich_glyph_width — advance width of one non-space glyph (the raw inlines
 * this in both passes of rich_form_row_build). ASCII takes
 * font_scale_for_char(); non-ASCII is 8.2*sy, or for font_kind 0 the
 * 0x414-relative table: 10.8/11.8/8.8 by bitmask. @ raw (inlined)
 */
static float rich_glyph_width(float sy, uint32_t cp, int font_kind)
{
    float w;

    if (cp < 0x80) {
        w = font_scale_for_char(sy, (int)cp);
    } else {
        w = sy * 8.2f;
        if (font_kind == 0) {
            w = 8.8f;
            uint32_t k = cp - 0x414;
            if (k < 0x3b) {
                if ((1ULL << (k & 0x3f) & 0x491010504910105ULL) == 0) {
                    if ((1ULL << (k & 0x3f) & 0x20000000200000ULL) != 0)
                        w = 11.8f;
                } else {
                    w = 10.8f;
                }
            }
            w = w * sy;
        }
    }
    return w;
}

/*
 * rich_text_layout — lay a single line of ASCII text into rich-view rows
 * (stride 0xb0). Spaces advance x by 5*font_size; each glyph becomes one row
 * {x centered on its width, y, color = nexus_rich_color(bright, char_index,
 * cell_y, strlen(text), position), font pair, class = bright+1, 1-char text}.
 * Stops at '\n', a negative (>= 0x80) byte, max_rows glyphs or the 640-row
 * cap; then pads empty rows (template 0x10f7e0/0x10f9b0/0x10f9b8) up to
 * max_rows or row 640. No return value (raw tail feeds only the canary).
 * @ 0016b76c
 */
void rich_text_layout(float x, uint32_t y, float font_size, void *view,
                      const char *text, int bright, uint32_t cell_y,
                      uint32_t max_rows, uint64_t position)
{
    float start_x = bright ? x : x - 36.0f;   /* raw: param_1 + -36.0 */
    float x_run = 0.0f;                       /* space delta since the last glyph */
    float cur_x = start_x;                    /* x of the last emitted glyph */
    float prev_w = 0.0f;
    bool emitted = false;
    uint32_t count_emitted = 0;
    uint32_t i = 0;

    for (;;) {
        while (text[i] == ' ') {              /* raw inner space-skip loop */
            i++;
            x_run = fmaf(font_size, 5.0f, x_run);
        }
        char c = text[i];
        if (c == '\0') {
            /* end of text: pad empty rows up to max_rows / the 640-row cap */
            if (max_rows <= count_emitted)
                return;
            uint32_t count = RICH_ROW_COUNT(view);
            uint32_t fill = max_rows - count_emitted;
            uint32_t cap = count < 0x281 ? 0x280 : count;   /* raw: 0x281/0x280 */
            uint64_t off = (uint64_t)count * 0xb0;
            while (cap != count) {
                count++;
                fill--;
                RICH_ROW_COUNT(view) = count;
                rich_row_t *row = (rich_row_t *)((char *)view + off);
                *(uint64_t *)(void *)&row->kind = RICH_ROW_TEMPLATE;       /* +0x10 abs */
                *(uint64_t *)(void *)&row->font_size = RICH_ROW_PAD_FONT;  /* +0x2c abs */
                *(uint64_t *)(void *)&row->x = RICH_ROW_PAD_XY;            /* +0x24 abs */
                row->text[0] = '\0';                                       /* +0x3e abs */
                *(uint64_t *)(void *)&row->color = 0xffffffff;             /* +0x1c abs */
                row->active = 0;                                           /* +0x3c abs */
                off += 0xb0;
                if (fill == 0)
                    break;
            }
            return;
        }

        int chi = (int)c;
        if (chi < 0 || chi == 10 || count_emitted == max_rows)
            return;                          /* '\n', non-ASCII byte, or full */

        float w = font_scale_for_char(font_size, chi);
        if (emitted) {
            float gap = fmaf(prev_w, 0.5f, font_size * 3.0f);  /* prev half-width + 3*fs */
            x_run = fmaf(w, 0.5f, gap + x_run);
            x_run = cur_x + x_run;           /* raw: fVar19 + fVar18 */
        } else {
            x_run = fmaf(w, 0.5f, x - 36.0f);
            if (bright)
                x_run = start_x;
        }
        cur_x = x_run;

        char glyph[2] = { c, '\0' };
        uint32_t color = nexus_rich_color(bright, i, cell_y,
                                          (uint32_t)strlen(text), position);
        if (RICH_ROW_COUNT(view) > 0x27f)     /* raw: 0x27f < count — 640-row cap */
            return;
        uint32_t row_idx = RICH_ROW_COUNT(view);
        rich_row_t *row = &RICH_VIEW_ROWS(view)[row_idx];
        RICH_ROW_COUNT(view) = row_idx + 1;
        *(uint64_t *)(void *)&row->kind = RICH_ROW_TEMPLATE;
        row->x = cur_x;
        row->y = y;
        row->font_size = font_size;           /* raw CONCAT44(param_3,param_3) */
        row->font_size_2 = font_size;
        row->active = 1;
        row->color = 0xffffffff;
        row->field_10 = 0;
        memcpy(row->text, glyph, 2);          /* 1 char + NUL at +0x3e abs */
        row->color = color;

        x_run = 0.0f;
        emitted = true;
        count_emitted++;
        row->font_class = (uint8_t)(bright + 1);   /* +0x3d abs */
        i++;
        prev_w = w;
    }
}

/*
 * rich_form_row_build — build one rich form row: measure the UTF-8 text
 * (full validation — returns 0 on any invalid sequence), fit-scale sy so the
 * measured width fits w when w > 0, then emit one row per glyph into free
 * slots (kind == 3) scanned from *cap. Each glyph row gets the centered x,
 * base y = fs - sy*y, font pair, color (param color, or
 * nexus_rich_color(1, glyph_index, 2, glyph_count, ctx) when font != 0),
 * class 2/0, and the UTF-8 bytes as text. Letter spacing 4.2*sy (3.0*sy when
 * font != 0), spaces 5*sy. Returns 1 on success / cap reached, 0 on invalid
 * UTF-8 or row exhaustion. Side effects: advances *cap past used rows.
 * @ 0016c388
 */
int rich_form_row_build(float x, float y, float sy, float fs, float w,
                        void *view, uint32_t *cap, const uint8_t *text,
                        uint32_t color, uint32_t glyph_cap, int font,
                        uint64_t ctx)
{
    /* ---- pass 1: measure (width + glyph count) ---- */
    float spacing = font ? 3.0f /* 0x40400000 */ : 4.2f /* 0x40866666 */;
    float width = 0.0f;
    uint32_t glyphs = 0;

    if (text[0] != 0) {
        const uint8_t *p = text;
        uint8_t b = p[0];
        do {
            uint32_t cp;
            int len = rich_utf8_next(p, &cp);
            if (len == 0)
                return 0;
            if (cp == 0x20) {
                width = fmaf(sy, 5.0f, width);
            } else if (glyphs == glyph_cap) {
                break;                       /* glyph cap hit: stop measuring */
            } else {
                float sp = fmaf(spacing, sy, width);
                if (glyphs != 0)
                    width = sp;              /* spacing applies after glyph 0 */
                width += rich_glyph_width(sy, cp, font);
                glyphs++;
            }
            p += len;
            b = p[0];
        } while (b != 0);
    }
    glyph_cap = glyphs;                      /* raw reuses param_10 as the count */

    if (w > 0.0f && width > w) {             /* raw ordered-compare idiom */
        sy = (w / width) * sy;
        width = w;
    }

    if (text[0] == 0 || glyph_cap == 0)
        return 1;

    /* ---- pass 2: emit ---- */
    float base_x = fmaf(sy, -20.0f /* 0xc1a00000 */, x);
    float base_y = fs - sy * y;              /* raw: fmsub(param_4, param_3, param_2) */
    float x_run = fmaf(width, -0.5f, base_x);   /* center the row */
    uint32_t glyph_idx = 0;
    const uint8_t *p = text;

    for (;;) {
        uint32_t cp;
        int len = rich_utf8_next(p, &cp);
        if (len == 0)
            return 0;
        if (cp == 0x20) {
            x_run = fmaf(sy, 5.0f, x_run);
        } else {
            /* find the next free row (kind == 3) from *cap */
            uint32_t row_idx = *cap;
            if (RICH_ROW_COUNT(view) <= row_idx)
                return 0;
            while (RICH_VIEW_ROWS(view)[row_idx].kind != 3) {
                row_idx++;
                *cap = row_idx;
                if (RICH_ROW_COUNT(view) <= row_idx)
                    return 0;
            }
            *cap = row_idx + 1;

            float gw = rich_glyph_width(sy, cp, font);
            float sp = fmaf(spacing, sy, x_run);
            if (glyph_idx != 0)
                x_run = sp;
            rich_row_t *row = &RICH_VIEW_ROWS(view)[row_idx];
            /* raw zeroes 15 qwords covering the text tail; the last store at
             * +0xb6 abs spills 8 bytes into the NEXT record's +0x06 */
            memset((uint8_t *)(void *)row + 0x2e, 0, 0x90);
            memcpy(row->text, p, (size_t)len);
            float x_center = fmaf(gw, 0.5f, x_run);
            row->y = base_y;
            row->font_size = sy;             /* raw CONCAT44(param_3,param_3) */
            row->font_size_2 = sy;
            row->x = x_center;
            uint32_t glyph_color;
            uint8_t class_byte;
            if (font == 0) {
                class_byte = 0;
                glyph_color = color;
            } else {
                class_byte = 2;
                glyph_color = nexus_rich_color(1, glyph_idx, 2, glyph_cap, ctx);
            }
            row->color = glyph_color;
            row->font_class = class_byte;
            row->active = 1;
            x_run += gw;
            glyph_idx++;
        }
        p += len;
        if (p[0] == 0)
            break;
        if (glyph_cap <= glyph_idx)
            return 1;
    }
    return 1;
}
/* renderer p1 chunk 5: covers raw lines 1313-1632 */
/* ===== rich stage geometry ===== */

/*
 * nexus_rich_launcher_geometry — map screen dimensions to launcher plan
 * coordinates. Gates: w/h finite, 960 <= w <= 8192, 540 <= h <= 8192,
 * 0 < cell <= 64 (finite), |x_off|/|y_off| <= 16384 (finite), out != NULL.
 * On pass writes out[0] = (x_off - w/2)/cell, out[1] = ((h - s*576)/2 +
 * s*30 - y_off)/cell, out[2] = s/cell where s = min(w/1024, h/576).
 * Returns 1 when out[2] is finite and in [0.0001, 128], else 4.
 * @ 0016c9d4
 */
int nexus_rich_launcher_geometry(float w, float h, float cell,
                                 float x_off, float y_off, float *out)
{
    if (!isfinite(w) || out == NULL)
        return 4;
    if (!(h <= 8192.0f) || !(w <= 8192.0f))   /* raw ordered-compare idiom */
        return 4;
    if (!(h >= 540.0f) || !(w >= 960.0f) || !isfinite(h))
        return 4;

    float ay = fabsf(y_off);
    float ax = fabsf(x_off);
    if (!(ay <= 16384.0f) || !(ax <= 16384.0f) || !isfinite(ay) || !isfinite(ax))
        return 4;
    if (!(cell <= 64.0f) || !(0.0f < cell) || !isfinite(cell))
        return 4;

    float left = x_off - w * 0.5f;             /* raw fnmsub(param_1, 0.5, param_4) */
    float s = fminf(w * 0.0009765625f /* 1/1024 */, h / 576.0f);
    float band = h - s * 576.0f;               /* raw fmadd(s, -576.0, h) */
    float scale = s / cell;
    float mid = fmaf(band, 0.5f, s * 30.0f);

    out[0] = left / cell;
    out[2] = scale;
    int code = scale <= 128.0f ? 1 : 4;
    out[1] = (mid - y_off) / cell;

    /* raw flag dance: 1 only for a finite scale >= 0.0001 */
    if (isfinite(scale) && scale >= 0.0001f)
        return code;
    return 4;
}

/* widgets.c — full def is (float, float, long, uint64_t, uint32_t *); the raw
 * call site below only recovers arg 4 (x3) — coordinator must reconcile. */
extern int nexus_rich_plan(uint64_t arg4);   /* widgets.c @ 0016a3ec */

/*
 * nexus_rich_plan_stage — validate stage bounds, run nexus_rich_plan, then
 * rescale the stage rect at stage (+4/+8/+0xc) into cell units:
 * y' = y/cell, x' = (x - x_off)/cell, z' = (z - y_off)/cell (written back in
 * place). Gates: |x_off|, |y_off| <= 16384 finite, 0 < cell <= 64 finite,
 * nexus_rich_plan(...) == 1, then y' finite in [0.0001, 128] and |x'|, |z'|
 * finite. Returns 1 ok, 4 invalid. Raw args 1/2 are 16-byte NEON values the
 * decompiler never tracks (unused here). @ 0016cb58
 */
int nexus_rich_plan_stage(const void *unused_v0, const void *unused_v1,
                          float cell, float x_off, float y_off,
                          uint64_t plan_arg4, uint64_t plan_arg5,
                          float *stage)
{
    (void)unused_v0;
    (void)unused_v1;
    (void)plan_arg5;                        /* raw param_7: unreferenced (x5) */

    float ay = fabsf(y_off);
    float ax = fabsf(x_off);
    if (!(ay <= 16384.0f) || !isfinite(ay) || !isfinite(ax))
        return 4;
    if (!(ax <= 16384.0f))
        return 4;
    if (!(cell <= 64.0f) || !(0.0f < cell) || !isfinite(cell))
        return 4;
    if (nexus_rich_plan(plan_arg4) != 1)
        return 4;

    float y_p = stage[1] / cell;                    /* stage + 4 */
    float x_p = (stage[2] - x_off) / cell;          /* stage + 8 */
    float z_p = (stage[3] - y_off) / cell;          /* stage + 0xc */
    stage[1] = y_p;
    stage[2] = x_p;
    stage[3] = z_p;

    if (isfinite(y_p) && 0.0001f <= y_p && y_p <= 128.0f) {
        float az = fabsf(z_p);
        float axp = fabsf(x_p);
        /* raw flag dance: 1 unless |x'| or |z'| is Inf/NaN */
        if (isfinite(az) && isfinite(axp))
            return 1;
    }
    return 4;
}

/* ===== rich toast (anchored label) ===== */

/* toast widget state (renderer-owned; misc.c reads g_toast_widget) */
uint64_t g_toast_widget;          /* 0x281a50 — anchored label widget (0 = not built) */
uint64_t g_toast_context;         /* 0x281a58 — context the widget was built for */
uint64_t g_toast_text_obj;        /* 0x281a60 — label text object */
uint32_t g_toast_failed;          /* 0x281a68 — bit0: build/lineage check failed */
uint32_t g_toast_visible;         /* 0x281a6c — 1 once positioned on screen */
uint8_t  g_toast_scaled;          /* 0x281a70 — 1 once the scale was applied */
char     g_toast_text[0x80];      /* 0x281a74 — cached label text (__memcpy_chk cap 0x80) */
float    g_toast_scale;           /* 0x281af4 — applied scale */
float    g_toast_density;         /* 0x281af8 — density the scale was computed for */
float    g_toast_center_x;        /* 0x281afc — center x it was positioned for */
float    g_toast_center_y;        /* 0x281b00 — center y it was positioned for */

/* misc.c-owned host table + engine helpers */
extern void *g_host_ctx;                    /* 0x1a7cc8 — host context handle */
extern int  (*g_host_ready)(void *ctx);     /* 0x1a7cf0 */
extern int  (*g_host_read)(void *ctx, uint64_t addr, void *out, uint32_t len);
                                             /* 0x1a7ce8 */
extern int  (*g_host_style)(void *ctx, const char *name);
                                             /* 0x1a7cf8 — SC style lookup */
extern uint64_t g_ui_scene_root;            /* 0x1a7d28 — SC scene root node */
extern uint64_t g_ui_context;               /* 0x1a7d30 — UI widget context root */
extern int  widget_child_link_valid(uint64_t node, uint64_t parent);
                                              /* misc.c @ 0016d5cc — parent+0x50 slot[index] == node */
extern void widget_set_position(uint64_t widget, float x, float y);
                                              /* misc.c @ 0016cf5c — engine 0x595314 */
extern uint64_t engine_sc_bundle_load(const char *path);
                                              /* misc.c @ 0016ce48 — loads "sc/ui.sc" */
extern int      g_host_init_done;      /* 0x1a7d0c — host table installed (nexus_rich_initialize) */
extern float    g_ui_density;          /* 0x1e53fc — design-density divisor (fonts.c name) */
extern float    g_ui_center_x;         /* 0x1e5400 */
extern float    g_ui_center_y;         /* 0x1e5404 */
extern const char RICH_TOAST_STYLE[];  /* 0x134a97 — style name for the toast label */

extern int  ui_scene_fonts_ready(void);        /* misc.c @ 0016d910 */
extern int  ui_fonts_preflight_gate(void);     /* misc.c @ 0016db8c */
extern int  movie_frame_count_check(uint64_t movie, int idx, int min);
                                              /* misc.c @ 0016dc20 — frame count (+0xbe) vs table */
extern int  widget_detached_check(uint64_t node);
                                              /* misc.c @ 0016ddc8 — parent NULL and index -1 */
extern void engine_call_5d7c30(uint64_t obj, int a);
                                              /* misc.c @ 0016ce84 */
extern uint64_t engine_call_5d8ad4(uint64_t bundle, const char *style);
                                              /* misc.c @ 0016ce98 — styled object create */
extern int  remote_vtable_matches(uint64_t obj, uint64_t rva);
                                              /* misc.c @ 0016debc — vptr vs game base + rva */
extern int  widget_scene_owner_check(uint64_t widget);
                                              /* misc.c @ 0016df78 — +0x30 root is scene root */
extern int  widget_chain_links_unique_check(uint64_t node, uint64_t child);
                                              /* misc.c @ 0016e020 */
extern void engine_call_594e80(uint64_t obj, int a);
                                              /* misc.c @ 0016ce20 */
extern void engine_call_5988ec(uint64_t parent, uint64_t child);
                                              /* misc.c @ 0016cfb8 — attach child to parent */
extern int  widget_flag48_equals(uint64_t widget, int expected);
                                              /* misc.c @ 0016e464 — +0x48 flag byte */
extern void engine_call_66ae58(void *out, const char *text);
                                              /* misc.c @ 0016ced8 — string object create */
extern void engine_call_5921a0(uint64_t obj, void *text);
                                              /* misc.c @ 0016ceac */
extern void game_call_66ad48(void *obj);
                                              /* misc.c @ 0014ec30 — forward via base + 0x66ad48 */
extern void engine_call_592250(uint64_t obj, uint32_t arg);
                                              /* misc.c @ 0016ceec */
extern void widget_set_scale(uint64_t widget, float sx, float sy);
                                              /* misc.c @ 0016cf00 */
extern void widget_set_anchor(uint64_t widget, void *out2f);
                                              /* misc.c @ 0016cf84 — engine 0x595378 */

/*
 * rich_toast_set — show/hide the anchored rich toast label.
 * show == 0 hides (park at (-10000, -10000), clear visible/scaled); show != 0
 * gates on host table + scene + fonts preflight, builds the label once
 * (bundle from nexus_rich_asset(5), styled object RICH_TOAST_STYLE, vtable
 * 0x11abad8, attached to the scene root), updates text when it changed
 * (cached in g_toast_text, cap 0x80), reads the char count at text_obj +0xb0
 * (1..0x100), scales to 21px = 21/(density*count) (clamped 0.01..8.0) and
 * anchors at ((20-center_x)/density, (55-center_y)/density) minus the anchor
 * offset, caching density/center/scale to skip redundant work. Returns
 * nothing (raw tail feeds only the canary). @ 0016d020
 */
void rich_toast_set(const char *text, int show)
{
    if (g_host_init_done == 0)
        return;
    if (!g_host_ready(g_host_ctx) || g_ui_scene_root == 0 || g_ui_context == 0)
        return;

    if (g_toast_widget != 0) {
        /* verify the label's parent chain still reaches the toast widget */
        if (g_toast_context != g_ui_context
            || !widget_child_link_valid(g_toast_widget, g_ui_context))
            goto lineage_failed;
        uint64_t node = g_toast_text_obj;
        if (node != 0) {
            uint32_t depth = 0;
            for (;;) {
                if (node == g_toast_widget || 0xf < depth)
                    break;
                uint64_t parent = 0;
                bool read_ok = false;
                if ((node + 0x40) >> 3 < 0x201) {
                    read_ok = false;          /* raw inline pointer-sanity gate */
                } else {
                    read_ok = g_host_read(g_host_ctx, node + 0x38, &parent, 8) == 1;
                }
                bool plausible = 0xfff < parent && read_ok && (parent & 7) == 0;
                uint64_t next = plausible ? parent : 0;
                if (!widget_child_link_valid(node, next))
                    goto lineage_failed;
                depth++;
                node = parent;
                if (!plausible) {
                    node = 0;
                    break;
                }
            }
        }
        if (node != g_toast_widget)
            goto lineage_failed;
    }

    if (show == 0) {
        /* hide: park the label off-screen */
        if (g_toast_widget != 0 && g_toast_visible != 0)
            widget_set_position(g_toast_widget, -10000.0f /* 0xc61c3c00 */,
                                -10000.0f);
        g_toast_visible = 0;
        g_toast_scaled = 0;
        return;
    }

    if (text == NULL || (g_toast_failed & 1) != 0)
        return;
    if (0x7f < strlen(text))
        return;
    if (!ui_scene_fonts_ready() || !ui_fonts_preflight_gate())
        return;

    if (g_toast_widget == 0) {
        /* build the toast label once */
        if (!g_host_style(g_host_ctx, nexus_rich_asset(5)))
            return;
        uint64_t bundle = engine_sc_bundle_load(nexus_rich_asset(5));
        if (!movie_frame_count_check(bundle, 5, 0))
            return;
        if (!widget_detached_check(bundle))
            return;
        engine_call_5d7c30(bundle, 0);
        uint64_t text_obj = engine_call_5d8ad4(bundle, RICH_TOAST_STYLE);
        if (!remote_vtable_matches(text_obj, 0x11abad8))
            return;
        if (!widget_scene_owner_check(text_obj))
            return;
        if (!widget_chain_links_unique_check(bundle, text_obj))
            return;
        engine_call_594e80(bundle, 0);
        widget_set_position(bundle, -10000.0f, -10000.0f);
        engine_call_5988ec(g_ui_scene_root, bundle);
        if (!widget_child_link_valid(bundle, g_ui_context))
            goto build_failed;
        if (!widget_flag48_equals(bundle, 0))
            goto build_failed;
        g_toast_context = g_ui_context;
        g_toast_widget = bundle;
        g_toast_text_obj = text_obj;
    }

    /* update the text when it changed */
    if (strcmp(text, g_toast_text) != 0) {
        g_toast_scaled = 0;
        uint64_t str_obj = 0;
        engine_call_66ae58(&str_obj, text);
        engine_call_5921a0(g_toast_text_obj, &str_obj);
        game_call_66ad48(&str_obj);
        __memcpy_chk(g_toast_text, text, strlen(text) + 1, 0x80);
        engine_call_592250(g_toast_text_obj, 0xffffdf75);
    }

    /* char count at text_obj + 0xb0 (2-byte read, 1..0x100) */
    uint16_t count = 0;
    bool read_failed = true;
    if (0xfff < g_toast_text_obj + 0xb0
        && (g_toast_text_obj & 0xfffffffffffffffeULL) != 0xffffffffffffff4eULL) {
        /* raw pointer-sanity gate (the ~1 compare is Ghidra's rendering) */
        read_failed = g_host_read(g_host_ctx, g_toast_text_obj + 0xb0,
                                  &count, 2) != 1;
    }
    if (read_failed || count == 0 || count >= 0x101 || !(0.0f < g_ui_density))
        return;

    float scale = 21.0f / (g_ui_density * (float)count);
    if (!isfinite(scale) || !(0.01f <= scale) || !(scale <= 8.0f))
        return;

    if (g_toast_scaled == 1 && g_toast_visible != 0
        && g_toast_scale == scale
        && g_toast_density == g_ui_density
        && g_toast_center_x == g_ui_center_x
        && g_toast_center_y == g_ui_center_y)
        return;                               /* already positioned */

    g_toast_scaled = 0;
    widget_set_scale(g_toast_text_obj, scale, scale);
    float anchor[2] = {0};
    widget_set_anchor(g_toast_text_obj, anchor);
    if (!isfinite(anchor[0]) || !isfinite(anchor[1]))
        return;
    widget_set_position(g_toast_text_obj,
                        (20.0f - g_ui_center_x) / g_ui_density - anchor[0],
                        (55.0f - g_ui_center_y) / g_ui_density - anchor[1]);
    if ((g_toast_visible & 1) == 0)
        widget_set_position(g_toast_widget, 0, 0);
    g_toast_density = g_ui_density;
    g_toast_center_x = g_ui_center_x;
    g_toast_center_y = g_ui_center_y;
    g_toast_visible = 1;
    g_toast_scaled = 1;
    g_toast_scale = scale;
    return;

lineage_failed:
    g_toast_failed = 1;
    return;
build_failed:
    g_toast_failed = 1;
    return;
}
/* renderer p1 chunk 6: covers raw lines 1634-2275 */
/* ===== script-port popups ===== */

/* fps popup state block (renderer-owned, 0x9a8 bytes at 0x282150) */
uint64_t g_fps_popup_owner;         /* 0x282150 — context the popup was built for */

uint32_t g_fps_popup_open;         /* 0x282af8 — 1 while the fps popup is up */
uint32_t g_fps_popup_limit;        /* 0x282afc — selected fps limit (min 0x91 = 145) */
uint32_t g_fps_popup_applied;      /* 0x282b00 — applied limit mirror of g_fps_popup_limit */
uint32_t g_fps_popup_field_b04;    /* 0x282b04 — zeroed on open */
uint32_t g_fps_popup_opens;        /* 0x282b0c — capped 24: popup_open event counter */

/* cross-file externs used by this chunk */
extern void (*g_host_log)(void *ctx, const char *cat, const char *event,
                           uint32_t value);
                                      /* 0x1a7d00 — misc.c-owned host log entry */
extern uint64_t g_game_base;            /* 0x1a7cd0 — misc.c-owned game image base */

/* cross-file externs used by this chunk */
extern int  script_port_client_performance_query(void *out);
                                      /* misc.c @ 00191c90 — forwarded query, 0x98-byte out */
extern int  ui_scene_fonts_ready(void);
                                      /* misc.c @ 0016d910 — scene root identity + fonts gate */
extern int  ui_edit_controls_ctx_valid(void);
                                      /* misc.c @ 0016e7c0 — 0x282168 ctx: class 0x11c0c58, bound */
extern uint64_t g_fps_popup_edit_ctx;  /* 0x282168 — misc.c/widgets.c edit-controls widget */
extern uint64_t g_fps_popup_field_ae0; /* 0x282ae0 — widgets.c fps panel field */

/* rodata: performance query magic (leads the 0x98-byte query result) */
extern const uint64_t PERF_QUERY_MAGIC;  /* 0x10f850 */

/*
 * nexus_script_port_fps_open — open the FPS-limit popup from the script port.
 * Gates: host table ready, scene fonts ready, performance query succeeds with
 * bit 0 of the second qword set (state[8]). If the popup block's owner is a
 * foreign context (not 0 and not g_ui_context) it is reset (memset 0x9a8).
 * Sets the popup limit to max(0x91, queried limit) (0x91 = 145), marks it
 * open, mirrors the limit into the applied field, pushes the limit into the
 * edit-controls widget (+0xe8) and clears its +0x140 flag when the widget is
 * valid. Logs 'script_port_fps'/'popup_open' (capped 24 opens). Returns
 * nothing; raw tail feeds only the canary.
 * @ 0016e618
 */
void nexus_script_port_fps_open(void)
{
    uint8_t query[0x98] = {0};
    uint64_t magic = PERF_QUERY_MAGIC;
    memcpy(query, &magic, sizeof magic);   /* caller pre-fills the magic qword */

    if (g_host_ready == NULL)
        return;
    if (!g_host_ready(g_host_ctx)
        || !ui_scene_fonts_ready()
        || script_port_client_performance_query(query) == 0)
        return;
    if ((((const uint64_t *)query)[1] & 1) == 0)   /* state[8]: engage bit */
        return;

    int32_t queried = *(const int32_t *)(query + 0x10);   /* state[16] */

    if (g_fps_popup_owner == 0 || g_fps_popup_owner == g_ui_context) {
        g_fps_popup_limit = 0x91;
        if ((uint32_t)(queried - 0x91) > 0xffffff6fU)   /* raw: queried > 0x91 */
            g_fps_popup_limit = queried;
        g_fps_popup_open = 1;
        g_fps_popup_field_b04 = 0;
        g_fps_popup_applied = g_fps_popup_limit;
        if (g_fps_popup_field_ae0 != 0 && ui_edit_controls_ctx_valid()) {
            *(int32_t *)(g_fps_popup_edit_ctx + 0xe8) = (int32_t)g_fps_popup_limit;
            *(uint8_t *)(g_fps_popup_edit_ctx + 0x140) = 0;
        }
    } else {
        /* stale popup built for a foreign context: reset the 0x9a8 block */
        memset(&g_fps_popup_owner, 0, 0x9a8);
        g_fps_popup_field_b04 = 0;
        g_fps_popup_open = 1;
        g_fps_popup_limit = 0x91;
        g_fps_popup_applied = g_fps_popup_limit;
        if ((uint32_t)(queried - 0x91) > 0xffffff6fU) {
            g_fps_popup_limit = queried;
            g_fps_popup_applied = queried;
        }
    }

    uint32_t opens = g_fps_popup_opens + 1;
    bool under_cap = g_fps_popup_opens < 0x18;
    g_fps_popup_opens = opens;
    if (under_cap && g_host_log != NULL)
        g_host_log(g_host_ctx, "script_port_fps", "popup_open", g_fps_popup_limit);
}

/* ===== host table install ===== */

/* host service table (renderer-owned — installed by nexus_rich_initialize) */
uint64_t g_host_table_v1;          /* 0x1a7cc0 — qword 0 of the host table (abi/version) */
void    *g_host_module;            /* 0x1a7cd8 — host module handle (name ptr in raw) */
uint64_t g_host_read_alt;          /* 0x1a7ce0 — alternate read entry (unused elsewhere) */
uint32_t g_host_init_latch;        /* 0x1a7d08 — ui_latch_test_and_set gate for init/render */
int32_t  g_rich_stage_state;       /* 0x1a7d10 — 0 idle / 1 building / 2 attached / -1 stopped */
uint32_t g_rich_hidden_latch;      /* 0x1a7d14 — 1 while the rich scene is parked hidden */
uint32_t g_rich_stop_log_count;    /* 0x1a7d20 — capped 64: 'nexus_rich_stopped' events */
const char *g_rich_status;         /* 0x1a7d50 — current status rodata ("not_initialized") */
uint32_t g_rich_menu_entry;        /* 0x1a7d18 — current plus-menu entry being built */
uint64_t g_rich_container;         /* 0x1a7d38 — 0x80-byte rich scene container widget */
uint32_t g_rich_clip_row;          /* 0x1a7d58 — next clip row to build (g_rich_row_total) */
float    g_rich_container_scale;   /* 0x1a7d5c — container scale (scale,scale pair) */
float    g_rich_container_x;       /* 0x1a7d60 — container x / y pair (low=x, high=y) */
uint64_t g_rich_perf_deadline;     /* 0x1a7d40 — frame stamp until which a cached paint holds */
uint64_t g_rich_perf_key;          /* 0x1a7d48 — {frame_stamp, performance_mode} cache key */
float    g_rich_clip_row_stage[4]; /* 0x1a7d58..0x1a7d68 — stage rect written by nexus_rich_plan_stage
                                        (0x1a7d58 doubles as the clip-row total uint32) */
#define G_RICH_STAGE_RECT ((float *)g_rich_clip_row_stage)

/* plus-layout table (renderer-owned; seeded by nexus_rich_initialize).
 * Declared BEFORE nexus_rich_initialize so the installer can seed it. */
uint64_t g_rich_plus_layout_lo;    /* 0x1e5168 — low qword of the template pair */
uint64_t g_rich_plus_layout_mid;   /* 0x1e5170 — mid qword */
uint64_t g_rich_plus_layout_hi;    /* 0x1e5178 — high qword (fonts.c uses 0x1e5168._4_4_) */

/* rodata templates for the plus-layout table at 0x1e5168 */
extern const uint64_t RICH_PLUS_LAYOUT_TEMPLATE_LO; /* 0x10f8a0 */
extern const uint64_t RICH_PLUS_LAYOUT_TEMPLATE_MID; /* 0x10fa80 */
extern const uint64_t RICH_PLUS_LAYOUT_TEMPLATE_HI; /* 0x10fa88 */

extern void engine_base_set(uint64_t base);
                                      /* misc.c @ 0016cde4 — stores the engine base global */
extern int  ui_latch_test_and_set(int value, volatile int *latch);
                                      /* menu_engine.c @ 00193f80 */

/*
 * nexus_rich_initialize — install the host service table from the caller's
 * 0x48-byte v1 table (int array: [0]=abi must be 1, [1]=size must be 0x48).
 * Gates: table != NULL, non-NULL read/style/ready/log entries, image span
 * >= 0x1000, and both build hashes matching. Installs the pointers at
 * 0x1a7cc0..0x1a7d0c, zeroes the 0x1e5168..0x1e51e0 plus-layout block,
 * seeds it with the 0x10f8a0/0x10fa80/0x10fa88 templates, sets the status
 * to "waiting parent mainloop" and stores the engine base. Returns 1
 * installed, 3 already rendering (latch held), 4 bad table / double init.
 * @ 0016e968
 */
int nexus_rich_initialize(int *table)
{
    if (table == NULL || table[0] != 1 || table[1] != 0x48
        || *(uint64_t *)(table + 10) == 0      /* read  @ +0x28 */
        || *(uint64_t *)(table + 0xc) == 0     /* ready @ +0x30 */
        || *(uint64_t *)(table + 0xe) == 0     /* style @ +0x38 */
        || *(uint64_t *)(table + 4) < 0x1000   /* image span @ +0x10 */
        || *(char **)(table + 6) == NULL       /* build hash a @ +0x18 */
        || strcmp(*(char **)(table + 6),
                  "a10aeb6b4085fb2a15d969a130cd9608231998b5269a40aed41129d99624ede3") != 0
        || *(char **)(table + 8) == NULL       /* build hash b @ +0x20 */
        || strcmp(*(char **)(table + 8),
                  "2e7e6e69e839a8b790f2d96c9d540fc313d8481d232828283034c71d705ec8d4") != 0)
        return 4;

    if (ui_latch_test_and_set(1, (volatile int *)&g_host_init_latch) & 1)
        return 3;                              /* render in flight */

    int result;
    if (g_host_init_done == 0) {
        *(void **)&g_host_read  = *(void **)(table + 10);   /* 0x1a7ce8 */
        g_host_read_alt         = *(uint64_t *)(table + 8); /* 0x1a7ce0 */
        *(void **)&g_host_style = *(void **)(table + 0xe);  /* 0x1a7cf8 */
        *(void **)&g_host_ready = *(void **)(table + 0xc);  /* 0x1a7cf0 */
        result = 1;
        *(void **)&g_host_log   = *(void **)(table + 0x10); /* 0x1a7d00 */
        g_host_module           = *(void **)(table + 6);    /* 0x1a7cd8 */
        g_game_base             = *(uint64_t *)(table + 4); /* 0x1a7cd0 */
        g_host_ctx              = *(void **)(table + 2);    /* 0x1a7cc8 */
        g_host_table_v1         = *(uint64_t *)(table + 0); /* 0x1a7cc0 */
        g_host_init_done        = 1;                        /* 0x1a7d0c */
        memset((void *)0x1e5180, 0, 0x60);      /* 0x1e5180..0x1e51e0 zeroed */
        g_rich_plus_layout_lo = RICH_PLUS_LAYOUT_TEMPLATE_LO;   /* 0x1e5168 */
        g_rich_plus_layout_hi = RICH_PLUS_LAYOUT_TEMPLATE_HI;   /* 0x1e5178 */
        g_rich_plus_layout_mid = RICH_PLUS_LAYOUT_TEMPLATE_MID; /* 0x1e5170 */
        g_rich_status = "waiting parent mainloop";
        engine_base_set(*(uint64_t *)(table + 4));
    } else {
        result = 4;                             /* already initialized */
    }
    g_host_init_latch = 0;
    return result;
}

/* externs: cross-file and external-to-dump */
extern int  nexus_rich_plus_layout(uint64_t *stage, uint64_t *table, uint64_t frame);
                                      /* external @ 0016bae0 — not in dump (Ghidra-recovered name) */
extern int  nexus_rich_header_entitlement(uint64_t *stage, uint32_t table_hi);
                                      /* external — not in dump (Ghidra-recovered name) */
extern int  rich_viewport_sync(float a, float b);
                                      /* renderer.c p2 @ 0016f840 — stage scale/offset cache */
extern void ui_diag_overlay_render(void);
                                      /* misc.c @ 0016fb10 — 0x200 diag text overlay */
extern void hud_camera_settings_render(uint64_t a, uint64_t b, void *state,
                                       uint64_t frame);
                                      /* widgets.c @ 0016fc60 — CAMERA SETTINGS popup */
extern void rich_view_render(void);
                                      /* renderer.c p2 @ 00170640 — rich overlay draw */
extern void hud_battle_button_render(void *state);
                                      /* widgets.c @ 00170db4 — native BATTLE button */
extern void fonts_chooser_frame(uint64_t a, uint64_t b, uint64_t frame);
                                      /* ui/fonts.c @ 00171808 (already reconstructed) */
extern void hud_fps_limit_panel_render(uint64_t a, uint64_t b, uint64_t frame);
                                      /* widgets.c @ 00171d68 — FPS LIMIT panel */
extern void theme_preview_pump(void *entry);
                                      /* ui/themes.c @ 00172aac — per-entry preview pump */
extern int  engine_code_snapshot_verify(void);
                                      /* misc.c @ 00172da8 — 57 engine regions memcmp */
extern uint64_t engine_call_11a2840(uint64_t size);
                                      /* misc.c @ 0016cdf0 — allocator via engine base */
extern void engine_call_593f14(uint64_t arg);
                                      /* misc.c @ 0016ce08 — render/draw pass hook */
extern void engine_call_594e80(uint64_t obj, int a);
                                      /* misc.c @ 0016ce20 */
extern int  widget_clip_create_attach(uint32_t row);
                                      /* widgets.c @ 00172fd4 — clip widget per row */
extern int  rich_button_style_apply(uint32_t idx);
                                      /* widgets.c @ 0017474c — style rich buttons */
extern int  ui_frame_state_sync(void *state);
                                      /* misc.c @ 001753b0 — per-frame cache sync */
extern uint64_t nexus_ui_performance_mode(void);
                                      /* misc.c @ 0014d044 — performance mode flag */

/*
 * nexus_rich_render — per-frame rich overlay pump (immediate-mode renderer).
 * Gates: state != NULL, host table installed, stage state >= 0, host ready,
 * init latch free (skips the frame if held). Stashes scene root/view; verifies
 * the live root (engine +0x12eb9f0) and view (root +0x90) match, runs the
 * fonts preflight + scene identity checks, then per stage: (0) require all
 * 7 rich assets styled, the engine code snapshot verified, allocate the
 * 0x80-byte container (vtable 0x11abcf0, +0x4e empty check), enable input,
 * park it hidden and attach to the root; (1) build clip rows 8 at a time
 * (capped by the g_rich_clip_row total); (2) run the frame pipeline
 * (viewport sync, diag overlay, camera settings, rich view, battle button,
 * font chooser, fps panel, theme apply) and paint unless the perf cache key
 * {frame, performance_mode} is unchanged. Stage regression logs
 * 'nexus_rich_stopped' with the reason (capped 64), status in g_rich_status.
 * Raw returns 0 waiting / 1 painted / 2 building / 3 latch held / 4 invalid /
 * -1 stopped via the canary feed; reconstructed as void (callers ignore it).
 * Side effects: sets g_rich_stage_state, clears the init latch at exit.
 * @ 0016eac4
 */
uint64_t nexus_rich_render(uint64_t param_a, uint64_t param_b, int *state,
                       uint64_t scene_root, uint64_t scene_view, uint64_t frame)
{
    uint64_t result = 0;

    if (state == NULL || g_host_init_done == 0 || g_rich_stage_state < 0)
        return 0;
    if (!g_host_ready(g_host_ctx))
        return 0;
    if (ui_latch_test_and_set(1, (volatile int *)&g_host_init_latch) & 1)
        return 3;                     /* render already in flight */

    if (g_rich_stage_state != 0
        && (g_ui_scene_root != scene_root || g_ui_context != scene_view)) {
        /* stage generation changed under us */
        result = 0xffffffff;
        g_rich_stage_state = -1;
        g_rich_status = "stage_generation";
        uint32_t count = g_rich_stop_log_count + 1;
        bool over_cap = 0x3f < g_rich_stop_log_count;
        g_rich_stop_log_count = count;
        if (over_cap || g_host_log == NULL)
            goto out;
        g_host_log(g_host_ctx, "nexus_rich_stopped", "stage_generation", 0);
        result = 0xffffffff;
        goto out;
    }
    g_ui_scene_root = scene_root;
    g_ui_context = scene_view;

    if (scene_root == 0 || scene_view == 0) {
        if (g_rich_stage_state == 0) {
            result = 0;
        } else {
            result = 0xffffffff;
            g_rich_stage_state = -1;
            g_rich_status = "stage_root_identity";
            uint32_t count = g_rich_stop_log_count + 1;
            bool under_cap = g_rich_stop_log_count < 0x40;
            g_rich_stop_log_count = count;
            if (under_cap && g_host_log != NULL) {
                g_rich_stage_state = -1;
                g_host_log(g_host_ctx, "nexus_rich_stopped", "stage_root_identity", 0);
                result = 0xffffffff;
            }
        }
        goto out;
    }

    /* live root identity: engine +0x12eb9f0 must be the scene root */
    uint64_t live_root = 0;
    bool read_ok = false;
    if ((g_game_base + 0x12eb9f8U) >> 3 >= 0x201) {
        read_ok = g_host_read(g_host_ctx, g_game_base + 0x12eb9f0, &live_root, 8) == 1;
    }
    bool plausible = 0xfff < live_root && read_ok && (live_root & 7) == 0;
    if ((plausible ? live_root : 0) != scene_root)
        goto stage_root_identity;
    /* live view identity: root +0x90 must be the scene view */
    uint64_t live_view = 0;
    read_ok = false;
    if ((scene_root + 0x98) >> 3 >= 0x201) {
        read_ok = g_host_read(g_host_ctx, scene_root + 0x90, &live_view, 8) == 1;
    }
    plausible = 0xfff < live_view && read_ok && (live_view & 7) == 0;
    if ((plausible ? live_view : 0) != scene_view)
        goto stage_root_identity;

    if (!ui_fonts_preflight_gate()) {
        g_rich_status = "waiting stage bounds idle";
        goto out;
    }
    if (!ui_scene_fonts_ready()) {
        g_rich_status = "waiting identity root transform";
        if (g_rich_stage_state != 0) {
            result = 0xffffffff;
            g_rich_stage_state = -1;
            g_rich_status = "root_transform_changed";
            uint32_t count = g_rich_stop_log_count + 1;
            bool under_cap = g_rich_stop_log_count < 0x40;
            g_rich_stop_log_count = count;
            if (under_cap && g_host_log != NULL) {
                g_host_log(g_host_ctx, "nexus_rich_stopped",
                           "root_transform_changed", 0);
                result = 0xffffffff;
            }
        }
        goto out;
    }

    /* stage bounds rect at root +0x3c (16 bytes) must equal (param_a, param_b) */
    uint8_t bounds[16] = {0};
    bool bounds_ok = false;
    if ((scene_root + 0x4c) >> 4 < 0x101) {
        bounds_ok = false;
    } else {
        bounds_ok = g_host_read(g_host_ctx, scene_root + 0x3c, bounds, 0x10) == 1;
    }
    result = 4;
    {
        /* raw NEON fcmeq/umaxv: all 4 lanes of {param_a_lo, param_b_lo, 0, 0}
         * must differ from the loaded bounds */
        int32_t want[4] = { (int32_t)param_a, (int32_t)param_b, 0, 0 };
        bool all_differ = true;
        for (int i = 0; i < 4; i++) {
            if (*(int32_t *)(bounds + i * 4) == want[i]) {
                all_differ = false;
                break;
            }
        }
        if (!all_differ && bounds_ok) {
            if (rich_viewport_sync(param_a, param_b) == 0) {
                g_rich_status = "waiting stage ui projection";
                goto out;
            }
            ui_diag_overlay_render();
            hud_camera_settings_render(param_a, param_b, state, frame);
            rich_view_render();
            hud_battle_button_render(state);
            fonts_chooser_frame(param_a, param_b, frame);
            hud_fps_limit_panel_render(param_a, param_b, frame);
            theme_preview_pump(state);

            if (g_rich_stage_state != 0) {
                if (!widget_child_link_valid(g_rich_container, scene_view))
                    goto container_root_membership;
                if (!widget_flag48_equals(g_rich_container, 1)) {
                    result = 0xffffffff;
                    g_rich_stage_state = -1;
                    g_rich_status = "container_input_disabled";
                    uint32_t count = g_rich_stop_log_count + 1;
                    bool under_cap = g_rich_stop_log_count < 0x40;
                    g_rich_stop_log_count = count;
                    if (under_cap && g_host_log != NULL) {
                        g_host_log(g_host_ctx, "nexus_rich_stopped",
                                   "container_input_disabled", 0);
                        result = 0xffffffff;
                    }
                    goto out;
                }
                goto stage_checks;
            }

            /* stage 0: build the container */
            if (*(char *)((uintptr_t)state + 9) == '\0') {
                result = 0;
                goto out;
            }
            if (!g_host_style(g_host_ctx, nexus_rich_asset(0))
                || !g_host_style(g_host_ctx, nexus_rich_asset(1))
                || !g_host_style(g_host_ctx, nexus_rich_asset(2))
                || !g_host_style(g_host_ctx, nexus_rich_asset(3))
                || !g_host_style(g_host_ctx, nexus_rich_asset(4))
                || !g_host_style(g_host_ctx, nexus_rich_asset(5))
                || !g_host_style(g_host_ctx, nexus_rich_asset(6))) {
                result = 0;
                goto out;
            }
            if (engine_code_snapshot_verify() == 0) {
                result = 0xffffffff;
                g_rich_stage_state = -1;
                g_rich_status = "a10_code_or_vtable_guard";
                uint32_t count = g_rich_stop_log_count + 1;
                bool under_cap = g_rich_stop_log_count < 0x40;
                g_rich_stop_log_count = count;
                if (under_cap && g_host_log != NULL) {
                    g_host_log(g_host_ctx, "nexus_rich_stopped",
                               "a10_code_or_vtable_guard", 0);
                }
                goto out;
            }
            g_rich_container = engine_call_11a2840(0x80);
            if (g_rich_container == 0) {
                result = 0xffffffff;
                g_rich_stage_state = -1;
                g_rich_status = "container_allocation";
                uint32_t count = g_rich_stop_log_count + 1;
                bool under_cap = g_rich_stop_log_count < 0x40;
                g_rich_stop_log_count = count;
                if (under_cap && g_host_log != NULL) {
                    g_host_log(g_host_ctx, "nexus_rich_stopped",
                               "container_allocation", 0);
                }
                goto out;
            }
            engine_call_593f14(g_rich_container);
            if (!remote_vtable_matches(g_rich_container, 0x11abcf0)
                || !widget_detached_check(g_rich_container)) {
                result = 0xffffffff;
                g_rich_stage_state = -1;
                g_rich_status = "sprite_constructor";
                uint32_t count = g_rich_stop_log_count + 1;
                bool under_cap = g_rich_stop_log_count < 0x40;
                g_rich_stop_log_count = count;
                if (under_cap && g_host_log != NULL) {
                    g_host_log(g_host_ctx, "nexus_rich_stopped",
                               "sprite_constructor", 0);
                }
                goto out;
            }
            /* container +0x4e must be an empty (0) 2-byte count */
            {
                uint64_t tmp = 1;   /* raw CONCAT62(...,1) — low half 1 */
                uint16_t count16 = 0;
                bool read_bad = g_rich_container + 0x4e < 0x1000
                             || (g_rich_container & 0xfffffffffffffffeULL)
                                  == 0xffffffffffffffb0ULL
                             || g_host_read(g_host_ctx, g_rich_container + 0x4e,
                                            &count16, 2) != 1
                             || count16 != 0;
                (void)tmp;
                if (read_bad) {
                    g_rich_stage_state = -1;
                    g_rich_status = "container_not_empty_for_input";
                    uint32_t count = g_rich_stop_log_count + 1;
                    bool under_cap = g_rich_stop_log_count < 0x40;
                    g_rich_stop_log_count = count;
                    if (under_cap && g_host_log != NULL) {
                        g_rich_stage_state = -1;
                        g_host_log(g_host_ctx, "nexus_rich_stopped",
                                   "container_not_empty_for_input", 0);
                    }
                    result = 0xffffffff;
                    goto out;
                }
            }
            engine_call_594e80(g_rich_container, 1);
            if (!widget_flag48_equals(g_rich_container, 1)) {
                g_rich_stage_state = -1;
                g_rich_status = "container_input_enable_failed";
                uint32_t count = g_rich_stop_log_count + 1;
                bool under_cap = g_rich_stop_log_count < 0x40;
                g_rich_stop_log_count = count;
                if (under_cap && g_host_log != NULL) {
                    g_rich_stage_state = -1;
                    g_host_log(g_host_ctx, "nexus_rich_stopped",
                               "container_input_enable_failed", 0);
                }
                result = 0xffffffff;
                goto out;
            }
            {
                uint32_t count = g_rich_stop_log_count + 1;
                bool under_cap = g_rich_stop_log_count < 0x40;
                g_rich_stop_log_count = count;
                if (under_cap && g_host_log != NULL)
                    g_host_log(g_host_ctx, "nexus_rich_input_enabled",
                               "empty_container_byte48_verified", 0);
            }
            widget_set_position(g_rich_container, -10000.0f, -10000.0f);
            engine_call_5988ec(scene_root, g_rich_container);
            if (widget_child_link_valid(g_rich_container, scene_view)) {
                uint32_t count = g_rich_stop_log_count + 1;
                g_rich_stage_state = 1;
                g_rich_status = "building_hidden_rich_scene";
                bool under_cap = g_rich_stop_log_count < 0x40;
                g_rich_stop_log_count = count;
                if (under_cap && g_host_log != NULL)
                    g_host_log(g_host_ctx, "nexus_rich_building",
                               "building_hidden_rich_scene", g_rich_clip_row);
                goto stage_checks;
            }
            g_rich_stage_state = -1;
            {
                uint32_t count = g_rich_stop_log_count + 1;
                g_rich_status = "container_stage_attachment";
                bool under_cap = g_rich_stop_log_count < 0x40;
                g_rich_stop_log_count = count;
                if (under_cap && g_host_log != NULL) {
                    g_rich_stage_state = -1;
                    g_host_log(g_host_ctx, "nexus_rich_stopped",
                               "container_stage_attachment", 0);
                }
            }
            result = 0xffffffff;
            goto out;
        }
    }
    goto out;

stage_root_identity:
    if (g_rich_stage_state == 0) {
        result = 0;
    } else {
        result = 0xffffffff;
        g_rich_stage_state = -1;
        g_rich_status = "stage_root_identity";
        uint32_t count = g_rich_stop_log_count + 1;
        bool under_cap = g_rich_stop_log_count < 0x40;
        g_rich_stop_log_count = count;
        if (under_cap && g_host_log != NULL) {
            g_rich_stage_state = -1;
            g_host_log(g_host_ctx, "nexus_rich_stopped", "stage_root_identity", 0);
            result = 0xffffffff;
        }
    }
    goto out;

container_root_membership:
    result = 0xffffffff;
    g_rich_stage_state = -1;
    g_rich_status = "container_root_membership";
    {
        uint32_t count = g_rich_stop_log_count + 1;
        bool under_cap = g_rich_stop_log_count < 0x40;
        g_rich_stop_log_count = count;
        if (under_cap && g_host_log != NULL) {
            g_host_log(g_host_ctx, "nexus_rich_stopped",
                       "container_root_membership", 0);
        }
    }
    goto out;

stage_checks:
    /* stage 1: clip rows, 8 per frame */
    if (!widget_child_link_valid(g_rich_container, scene_view))
        goto container_root_membership;
    if (g_rich_stage_state == 1) {
        uint32_t limit = g_rich_menu_entry + 8;
        if (g_rich_clip_row <= g_rich_menu_entry + 8)
            limit = g_rich_clip_row;
        for (; g_rich_menu_entry < limit; g_rich_menu_entry++) {
            if (widget_clip_create_attach(g_rich_menu_entry) != 1)
                goto out;
        }
        result = 2;
        if (g_rich_menu_entry < g_rich_clip_row)
            goto out;
        g_rich_stage_state = 2;
        g_rich_status = "rich_scene_attached";
        {
            uint32_t count = g_rich_stop_log_count + 1;
            bool under_cap = g_rich_stop_log_count < 0x40;
            g_rich_stop_log_count = count;
            if (under_cap && g_host_log != NULL)
                g_host_log(g_host_ctx, "nexus_rich_constructed",
                           "rich_scene_attached", 0);
        }
    }
    /* stage 2: per-frame paint */
    if (g_rich_clip_row != 0) {
        for (uint32_t i = 0; i < g_rich_clip_row; i++) {
            if (rich_button_style_apply(i) != 1)
                goto out;
        }
    }
    if (!ui_scene_fonts_ready()) {
        result = 0xffffffff;
        g_rich_stage_state = -1;
        g_rich_status = "stage_changed_during_paint";
        {
            uint32_t count = g_rich_stop_log_count + 1;
            bool over_cap = 0x3f < g_rich_stop_log_count;
            g_rich_stop_log_count = count;
            if (over_cap || g_host_log == NULL)
                goto out;
            g_host_log(g_host_ctx, "nexus_rich_stopped",
                       "stage_changed_during_paint", 0);
        }
        goto out;
    }
    if (!ui_frame_state_sync(state)) {
        result = 0xffffffff;
        g_rich_stage_state = -1;
        g_rich_status = "main_page_content_paint";
        {
            uint32_t count = g_rich_stop_log_count + 1;
            bool under_cap = g_rich_stop_log_count < 0x40;
            g_rich_stop_log_count = count;
            if (under_cap && g_host_log != NULL) {
                g_host_log(g_host_ctx, "nexus_rich_stopped",
                           "main_page_content_paint", 0);
            }
        }
        goto out;
    }

    /* perf cache: skip the paint when {frame, perf_mode} is unchanged */
    int perf = (int)nexus_ui_performance_mode();
    int frame_id = *state;
    if (!(frame < g_rich_perf_deadline) || g_rich_stage_state != 2
        || g_rich_hidden_latch != 0) {
        /* cache miss: recompute the plan + paint below */
        g_rich_perf_key = (uint64_t)(uint32_t)frame_id
                        | ((uint64_t)(uint32_t)perf << 32);
        uint64_t deadline = perf ? 100 : 0x28;
        g_rich_perf_deadline = deadline + frame;
        if (perf)
            frame = 0;
    } else {
        if ((uint32_t)g_rich_perf_key == (uint32_t)frame_id
            && (uint32_t)(g_rich_perf_key >> 32) == (uint32_t)perf) {
            result = 1;
            goto out;
        }
        g_rich_perf_key = (uint64_t)(uint32_t)frame_id
                        | ((uint64_t)(uint32_t)perf << 32);
        uint64_t deadline = perf ? 100 : 0x28;
        g_rich_perf_deadline = deadline + frame;
        if (perf)
            frame = 0;
    }

    if (nexus_rich_plan_stage((const void *)(uintptr_t)param_a,
                              (const void *)(uintptr_t)param_b,
                              g_ui_density, g_ui_center_x, g_ui_center_y,
                              (uint64_t)(uintptr_t)state, frame,
                              G_RICH_STAGE_RECT) != 1)
        goto plan_failed;
    if ((char)state[2] == 'Y' && *(char *)((uintptr_t)state + 10) == '\0') {
        if (nexus_rich_plus_layout((uint64_t *)G_RICH_STAGE_RECT,
                                   &g_rich_plus_layout_lo, frame) != 1)
            goto plan_failed;
    }
    if (nexus_rich_header_entitlement((uint64_t *)G_RICH_STAGE_RECT,
                                      (uint32_t)(g_rich_plus_layout_lo >> 32)) != 1)
        goto plan_failed;

    if (g_rich_stage_state == 0) {
        if (*(char *)((uintptr_t)state + 9) == '\0') {
            result = 0;
            goto out;
        }
        result = 0;   /* raw falls through the style ladder with uVar10 = asset(6) result */
        goto out;
    }

    widget_set_scale(g_rich_container, g_rich_container_scale,
                     g_rich_container_scale);
    float x = -10000.0f;   /* 0xc61c3c00 */
    float y = -10000.0f;
    if (*(char *)((uintptr_t)state + 9) != '\0') {
        y = g_rich_container_x;                 /* raw: high dword of 0x1a7d60 */
        x = *(float *)(void *)&g_rich_container_scale;  /* raw: low dword of 0x1a7d60 */
    }
    widget_set_position(g_rich_container, x, y);
    g_rich_hidden_latch = 0;
    result = 1;
    g_rich_status = "rich_scene_hidden";
    if (*(char *)((uintptr_t)state + 9) != '\0')
        g_rich_status = "rich_scene_attached";
    goto out;

plan_failed:
    result = 4;
    goto out;

    /* nexus_rich_render returns the stage result through the raw canary feed;
     * every exit path clears the init latch. */
out:
    g_host_init_latch = 0;
    return result;
}
/* renderer p1 chunk 7: covers raw lines 2277-2404 */
/* ===== viewport cache (rich pipeline step 1) ===== */

/* cross-file externs used by this chunk */
extern void ui_text_style_format(double px, double py, double stage_scale,
                                 double off_x, double off_y, char *buf,
                                  size_t cap_a, size_t cap_b, const char *fmt);
                                      /* misc.c @ 00176a24 — checked text formatter
                                         (raw arg order: a, b, scale, off_x, off_y,
                                         out, cap, cap, fmt) */
extern int chat_snapshot_read(void *out, size_t size);
                                      /* misc.c @ 001921d4 — chat snapshot read */

/* viewport status line (renderer-owned; formatted by rich_viewport_sync) */
char g_rich_viewport_line[0xc0];   /* 0x1e5408 — "pixels=... stage_scale=... offset=..." */

/*
 * rich_viewport_sync — read, validate and cache the stage viewport from the
 * scene root. Reads cell size (root +0x178), center x (root +0x4c), center y
 * (root +0x54) and the 8-byte pixel size (root +0x284) through host_read,
 * each behind the raw pointer-sanity gate. Gates: cell finite in (0, 64],
 * |center_x| and |center_y| finite <= 16384, and trunc(px/cell) ==
 * low(pixel_size) && trunc(py/cell) == high(pixel_size). On change formats
 * "pixels=%.9gx%.9g stage_scale=%.9g offset=%.9g,%.9g ui=%dx%d" into
 * g_rich_viewport_line (caps 0xc0/0xc0), logs 'nexus_rich_viewport' (capped
 * 64 events) and publishes cell->g_ui_density, center_x->g_ui_center_x,
 * center_y->g_ui_center_y. Returns 1 cached, 0 any gate failed.
 * Raw args are the stage bounds pair used for the px/py compare.
 * @ 0016f840
 */
int rich_viewport_sync(float bound_a, float bound_b)
{
    uint8_t cell_scale[8] = {0};   /* {center_x (f32), cell (f32)} */
    float center_x, center_y;
    uint64_t pixel_size = 0;

    if (g_ui_scene_root + 0x178 < 0x1000
        || (g_ui_scene_root & 0xfffffffffffffffcULL) == 0xfffffffffffffe84ULL)
        return 0;                          /* raw pointer-sanity gate */
    if (g_host_read(g_host_ctx, g_ui_scene_root + 0x178, cell_scale + 4, 4) != 1)
        return 0;
    if (g_ui_scene_root + 0x4c < 0x1000
        || (g_ui_scene_root & 0xfffffffffffffffcULL) == 0xffffffffffffffb0ULL)
        return 0;
    if (g_host_read(g_host_ctx, g_ui_scene_root + 0x4c, cell_scale, 4) != 1)
        return 0;
    if (g_ui_scene_root + 0x54 < 0x1000
        || (g_ui_scene_root & 0xfffffffffffffffcULL) == 0xffffffffffffffa8ULL)
        return 0;
    if (g_host_read(g_host_ctx, g_ui_scene_root + 0x54, &center_y, 4) != 1)
        return 0;

    if (!((g_ui_scene_root + 0x28c) >> 3 > 0x200))
        return 0;
    if (g_host_read(g_host_ctx, g_ui_scene_root + 0x284, &pixel_size, 8) != 1)
        return 0;

    float cell;
    memcpy(&cell, cell_scale + 4, sizeof cell);
    center_x = *(const float *)(void *)cell_scale;

    if (!isfinite(fabsf(cell)) || !(0.0f < cell) || !(cell <= 64.0f))
        return 0;
    float acx = fabsf((float)center_x);
    if (acx == INFINITY || isnan(acx))
        return 0;
    float acy = fabsf(center_y);
    if (acy == INFINITY || isnan(acy)
        || !(acx <= 16384.0f) || !(acy <= 16384.0f))
        return 0;

    if ((float)(int)(bound_a / cell) == (float)(int)(uint32_t)pixel_size
        && (float)(int)(bound_b / cell) == (float)(int)(uint32_t)(pixel_size >> 32)) {
        if (cell != g_ui_density || center_x != g_ui_center_x
            || center_y != g_ui_center_y) {
            ui_text_style_format((double)bound_a, (double)bound_b,
                                 (double)cell, (double)center_x,
                                 (double)center_y, g_rich_viewport_line,
                                 0xc0, 0xc0,
                                 "pixels=%.9gx%.9g stage_scale=%.9g "
                                 "offset=%.9g,%.9g ui=%dx%d");
            uint32_t count = g_rich_stop_log_count + 1;
            bool under_cap = g_rich_stop_log_count < 0x40;
            g_rich_stop_log_count = count;
            if (under_cap && g_host_log != NULL)
                g_host_log(g_host_ctx, "nexus_rich_viewport",
                           g_rich_viewport_line, 0);
        }
        g_ui_center_x = center_x;
        g_ui_density = cell;
        g_ui_center_y = center_y;
        return 1;
    }
    return 0;
}
/* renderer p1 chunk 8 (renderer.c part 2, chunk 1): covers raw lines 2361-2745 */
/* ===== part 2 externs: p1-owned globals and functions (see renderer_p1*.c) ===== */

extern void    *g_host_ctx;             /* 0x1a7cc8 — p1: host context handle */
extern int    (*g_host_read)(void *ctx, uint64_t addr, void *out, uint32_t len);
                                          /* 0x1a7ce8 — p1 */
extern int    (*g_host_ready)(void *ctx); /* 0x1a7cf0 — p1 */
extern int    (*g_host_style)(void *ctx, const char *name);
                                          /* 0x1a7cf8 — p1 */
extern void  (*g_host_log)(void *ctx, const char *cat, const char *event,
                           uint32_t value);
                                          /* 0x1a7d00 — p1 */
extern uint64_t g_game_base;              /* 0x1a7cd0 — p1 */
extern uint64_t g_ui_scene_root;          /* 0x1a7d28 — p1 */
extern uint64_t g_ui_context;             /* 0x1a7d30 — p1 */
extern int      g_host_init_done;         /* 0x1a7d0c — p1 */
extern uint32_t g_host_init_latch;        /* 0x1a7d08 — p1 */
extern int32_t  g_rich_stage_state;       /* 0x1a7d10 — p1 */
extern uint32_t g_rich_menu_entry;        /* 0x1a7d18 — p1 */
extern uint32_t g_rich_stop_log_count;    /* 0x1a7d20 — p1 */
extern uint64_t g_rich_container;         /* 0x1a7d38 — p1 */
extern const char *g_rich_status;         /* 0x1a7d50 — p1 */
extern uint32_t g_rich_clip_row;          /* 0x1a7d58 — p1 */
extern float    g_ui_density;             /* 0x1e53fc — p1 (design-density divisor) */
extern float    g_ui_center_x;            /* 0x1e5400 — p1 */
extern float    g_ui_center_y;            /* 0x1e5404 — p1 */
extern char     g_rich_viewport_line[0xc0]; /* 0x1e5408 — p1 (viewport status line) */
extern uint64_t g_rich_plus_layout_lo;    /* 0x1e5168 — p1 */
extern uint64_t g_rich_plus_layout_mid;   /* 0x1e5170 — p1 */
extern uint64_t g_rich_plus_layout_hi;    /* 0x1e5178 — p1 */
extern uint64_t g_toast_widget;           /* 0x281a50 — p1 */
extern uint64_t g_toast_context;          /* 0x281a58 — p1 */
extern uint64_t g_toast_text_obj;         /* 0x281a60 — p1 */
extern uint32_t g_toast_failed;           /* 0x281a68 — p1 */
extern uint64_t g_fps_popup_owner;        /* 0x282150 — p1 (0x9a8 block base) */

/* p1-owned functions */
extern int rich_viewport_sync(float a, float b);  /* p1 @ 0016f840 */

/* ===== part 2 externs: other files ===== */

extern int  ui_latch_test_and_set(int value, volatile int *latch);
                                      /* menu_engine.c @ 00193f80 */

/* misc.c engine helpers (subset used by this chunk) */
extern int      chat_snapshot_read(void *out, size_t size);
                                      /* misc.c @ 001921d4 — chat snapshot read */
extern void     engine_call_887c14(void);
                                      /* misc.c @ 0016ce34 — post-alloc bookkeeping */
extern uint64_t engine_call_11a2840(uint64_t size);
                                      /* misc.c @ 0016cdf0 — allocator via engine base */
extern int      engine_code_snapshot_verify(void);
                                      /* misc.c @ 00172da8 — 57 engine regions memcmp */
extern uint64_t engine_sc_bundle_load(const char *path);
                                      /* misc.c @ 0016ce48 — loads "sc/ui.sc" */
extern int      movie_frame_count_check(uint64_t movie, int idx, int min);
                                      /* misc.c @ 0016dc20 — frame count (+0xbe) vs table */
extern void     engine_call_5d7c30(uint64_t obj, int a);
                                      /* misc.c @ 0016ce84 */
extern int      sc_object_release(uint64_t obj);
                                      /* misc.c @ 0014e5ac — validate free SC node */
extern void     widget_set_text(uint64_t widget, uint64_t text_obj);
                                      /* misc.c @ 0016cec0 — engine 0x88836c */
extern void     widget_set_position(uint64_t widget, float x, float y);
                                      /* misc.c @ 0016cf5c — engine 0x595314 */
extern void     widget_set_scale(uint64_t widget, float sx, float sy);
                                      /* misc.c @ 0016cf00 — engine 0x595344 */
extern void     widget_set_anchor(uint64_t widget, void *out2f);
                                      /* misc.c @ 0016cf84 — engine 0x595378 */
extern void     engine_call_5988ec(uint64_t parent, uint64_t child);
                                      /* misc.c @ 0016cfb8 — attach child to parent */
extern int      widget_child_link_valid(uint64_t node, uint64_t parent);
                                      /* misc.c @ 0016d5cc — parent+0x50 slot[index] == node */
extern int      widget_scene_owner_check(uint64_t widget);
                                      /* misc.c @ 0016df78 — +0x30 root is scene root */
extern int      widget_flag48_equals(uint64_t widget, int expected);
                                      /* misc.c @ 0016e464 — +0x48 flag byte */
extern uint64_t ui_string_object(const char *text);
                                      /* misc.c @ 0017b858 — intern string object */

/* renderer-owned row metrics (raw 0x283330 / 0x22d818) */
float g_rich_row_base;      /* 0x283330 — label y base for the diag text */
float g_rich_row_height;    /* 0x22d818 — row height scale (battle-state adjacent) */

/* ===== rich view render (chat overlay rows) ===== */

/* renderer-owned chat row state (raw block at 0x283a00, 0x820 bytes) */
char g_rich_chat_text[0x820];        /* 0x283a00 — 0x800 diag text + state tail */
float g_rich_chat_anchor_x;          /* 0x283a00-8 — anchor x reference (block top) */
float g_rich_chat_anchor_y;          /* 0x283a00-4 — anchor y reference */

/* rich-view chat label widget slots: 2 records at 0x284220, stride 0x1b qwords */
typedef struct {
    uint64_t widget;          /* +0x00 — 0x260-byte row widget */
    uint64_t field_08;        /* +0x08 */
    uint64_t source;          /* +0x10 */
    uint64_t field_18;        /* +0x18 */
    uint64_t field_20;        /* +0x20 */
    uint64_t field_28;        /* +0x28 */
    uint64_t field_30;        /* +0x30 */
    uint64_t field_38;        /* +0x38 */
    uint64_t field_40;        /* +0x40 */
    uint64_t field_48;        /* +0x48 */
    uint64_t field_50;        /* +0x50 */
    uint64_t field_58;        /* +0x58 */
    uint64_t field_60;        /* +0x60 */
    uint64_t field_68;        /* +0x68 */
    uint64_t field_70;        /* +0x70 */
    uint64_t field_78;        /* +0x78 */
    uint64_t field_80;        /* +0x80 */
    uint64_t field_88;        /* +0x88 */
} rich_chat_row_t;             /* 0xd8 bytes = 0x1b qwords */

rich_chat_row_t g_rich_chat_rows[2];   /* 0x284220 — CHAT/CLEAR label rows */
uint64_t g_rich_chat_owner;            /* 0x2843d0 — context the rows were built for */
uint8_t  g_rich_chat_built;            /* 0x2843d8 — 1 once both rows exist */
uint32_t g_rich_chat_ready;            /* 0x2839fc — 1 when rows are positioned */

/* cross-file widgets.c renderers used below */
extern void rich_label_state_render(float x, float y, float font_size,
                                    uint64_t *state, const char *text);
                                      /* renderer.c p2 @ 00179f68 — label from 0x820 state */
extern int  rich_binding_bind(uint64_t *row_slot, uint64_t display,
                              uint32_t row_index);
                                      /* renderer.c p2 @ 0017c42c — bind widget to display */

/* misc.c-owned row sources */
extern uint64_t g_menu_page_list_widget;   /* 0x2842f8 — registry page list (widgets.c) */
extern uint64_t g_menu_edit_controls;      /* 0x2843d0 lower half usage in misc (verify) */

/*
 * rich_view_render — render the rich overlay: pull the 0x800 diag text, draw
 * the CHAT label, and build/position the two 0x260-byte chat row widgets
 * (table at 0x284220). Gates per row: bundle loaded and styled
 * (nexus_rich_asset(5)), engine code snapshot verified, parent/detached
 * checks, widget vptr == game_base + 0x11c0a48, +0x38/+0x40 identity reads,
 * sc_object_release, rich_binding_bind(row, bundle, index|900), text set to
 * "CHAT"/"CLEAR", parked hidden, attached to the scene root, lineage +
 * +0x48 flag verified. Positioning: scale 76*y*density/(width*density)
 * (clamped 0 < s <= 8), x = (80*i - center_x)/density - anchor_x,
 * y = (center_y - 450*density)/density - anchor_y. Resets the whole
 * 0x284220..0x2843d8 block when the owner context changed. No return value.
 * @ 00170640
 */
void rich_view_render(void)
{
    char diag[0x800];
    int have_text = chat_snapshot_read(diag, 0x800);
    const char *text = have_text ? diag : NULL;

    g_rich_chat_ready = 0;
    float base = 0.0f;
    if (g_rich_row_height == 0.0f || !(g_rich_row_height > 0.0f))
        base = 0.0f;
    else
        base = fmaf(g_rich_row_height, 8.0f /* 0x41000000 */,
                    g_rich_row_base /* 0x283330 */);

    float top = g_rich_row_height * 260.0f;
    float cap = top <= base ? base : top;
    rich_label_state_render(g_rich_row_height * 20.0f, cap,
                            g_rich_row_height * 14.0f,
                            (void *)g_rich_chat_text, text);

    if (have_text == 0) {
        /* no chat snapshot: park any live rows off-screen */
        if (g_rich_chat_rows[0].widget != 0
            && widget_child_link_valid(g_rich_chat_rows[0].widget, g_ui_context))
            widget_set_position(g_rich_chat_rows[0].widget,
                                -10000.0f, -10000.0f);
        if (g_rich_chat_rows[1].widget != 0
            && widget_child_link_valid(g_rich_chat_rows[1].widget, g_ui_context))
            widget_set_position(g_rich_chat_rows[1].widget,
                                -10000.0f, -10000.0f);
        return;
    }

    if (g_rich_chat_owner == 0 || g_rich_chat_owner == g_ui_context) {
        if (g_rich_chat_built != 1)
            goto build_rows;
    } else {
        /* owner context changed: reset the whole row block */
        memset(&g_rich_chat_rows, 0, sizeof g_rich_chat_rows);
        g_rich_chat_owner = 0;
        g_rich_chat_built = 0;
    }

build_rows:
    if (g_rich_chat_built != 1) {
        if (!g_host_style(g_host_ctx, nexus_rich_asset(5)))
            return;
        if (engine_code_snapshot_verify() == 0)
            return;
        for (int i = 0; i < 2; i++) {
            rich_chat_row_t *row = &g_rich_chat_rows[i];
            if (row->widget == 0) {
                uint64_t bundle = engine_sc_bundle_load(nexus_rich_asset(5));
                if (!movie_frame_count_check(bundle, 5, 0))
                    return;
                /* bundle +0x38 must be 0 (parent) and +0x40 must be -1 (index) */
                uint64_t parent = 1;
                float index_f = 0.0f;
                if (bundle + 0x38 < 0x1000
                    || (bundle & 0xfffffffffffffff8ULL) == 0xffffffffffffffc0ULL
                    || g_host_read(g_host_ctx, bundle + 0x38, &parent, 8) != 1
                    || parent != 0
                    || bundle + 0x40 < 0x1000
                    || (bundle & 0xfffffffffffffffcULL) == 0xffffffffffffffbcULL
                    || g_host_read(g_host_ctx, bundle + 0x40, &index_f, 4) != 1
                    || index_f != -1.0f)
                    return;
                uint64_t widget = engine_call_11a2840(0x260);
                row->widget = widget;
                if (widget == 0)
                    return;
                engine_call_887c14();
                /* widget vptr must be game_base + 0x11c0a48 */
                uint64_t vptr = 0;
                bool read_ok = false;
                if ((widget + 8) >> 3 >= 0x201)
                    read_ok = g_host_read(g_host_ctx, widget, &vptr, 8) == 1;
                bool plausible = 0xfff < vptr && read_ok && (vptr & 7) == 0;
                if ((plausible ? vptr : 0) != g_game_base + 0x11c0a48U)
                    return;
                /* widget +0x38 parent must be 0 and +0x40 index -1 */
                parent = 1;
                index_f = 0.0f;
                if (widget + 0x38 < 0x1000
                    || (widget & 0xfffffffffffffff8ULL) == 0xffffffffffffffc0ULL
                    || g_host_read(g_host_ctx, widget + 0x38, &parent, 8) != 1
                    || parent != 0
                    || widget + 0x40 < 0x1000
                    || (widget & 0xfffffffffffffffcULL) == 0xffffffffffffffbcULL
                    || g_host_read(g_host_ctx, widget + 0x40, &index_f, 4) != 1
                    || index_f != -1.0f)
                    return;
                if (sc_object_release(widget) == 0)
                    return;
                if (rich_binding_bind(&row->widget, bundle,
                                      (uint32_t)i | 900) == 0)
                    return;
                engine_call_5d7c30(bundle, 0);
                uint64_t text_obj = ui_string_object(i == 0 ? "CHAT" : "CLEAR");
                widget_set_text(row->widget, text_obj);
                widget_set_position(row->widget, -10000.0f, -10000.0f);
                engine_call_5988ec(g_ui_scene_root, row->widget);
                if (!widget_child_link_valid(row->widget, g_ui_context))
                    return;
                /* +0x48 flag must be 1 */
                uint8_t flag = 0xff;
                if (row->widget + 0x49 < 0x1001
                    || g_host_read(g_host_ctx, row->widget + 0x48, &flag, 1) != 1
                    || flag != 1)
                    return;
            }
        }
        g_rich_chat_owner = g_ui_context;
        g_rich_chat_built = 1;
    }

    /* position both rows */
    for (int i = 0; i < 2; i++) {
        rich_chat_row_t *row = &g_rich_chat_rows[i];
        widget_set_position(row->widget, 0, 0);
        widget_set_scale(row->widget, 1.0f /* 0x3f800000 */,
                         1.0f /* 0x3f800000 */);
        float anchor[2] = {0};
        widget_set_anchor(row->widget, anchor);
        if (!isfinite(anchor[0]) || !isfinite(anchor[1]))
            return;
        /* raw: anchor deltas must be strictly positive (a-b > 0 twice) */
        if (!(anchor[0] - g_rich_chat_anchor_x > 0.0f)
            || !(anchor[1] - g_rich_chat_anchor_y > 0.0f))
            return;
        float width = 0.0f;
        uint8_t size16[16] = {0};
        if (row->widget + 0x10 < 0x1000
            || (row->widget & 0xfffffffffffffff0ULL) == 0xffffffffffffffe0ULL
            || g_host_read(g_host_ctx, row->widget + 0x10, size16, 0x10) != 1)
            return;
        float w_lo, w_hi, s_lo, s_hi;
        memcpy(&w_lo, size16, 4);
        memcpy(&w_hi, size16 + 4, 4);
        memcpy(&s_lo, size16 + 8, 4);
        memcpy(&s_hi, size16 + 12, 4);
        (void)s_hi;
        if (!isfinite(w_lo) || !isfinite(w_hi) || !isfinite(s_lo)
            || !isfinite(s_hi))
            return;
        if (w_hi != 0.0f || s_lo == 0.0f || !(w_lo > 0.0f) || !(s_hi > 0.0f)
            || !(w_lo <= 128.0f) || !(s_hi <= 128.0f))
            return;
        (void)w_lo; (void)s_lo;
        /* scale from the row width: 76 * row_h * density / (width * density) */
        float scale = (w_lo * 76.0f * g_rich_row_height) / (width * g_ui_density);
        if (!isfinite(scale) || !(scale > 0.0f) || !(scale <= 8.0f))
            return;
        widget_set_scale(row->widget, scale, scale);
        widget_set_anchor(row->widget, anchor);
        float y_center = fmaf((float)i, 80.0f /* 0x42a80000 */,
                              20.0f /* 0x41a00000 */);
        float x = g_ui_center_x - y_center * g_rich_row_height;
        float y = g_ui_center_y - 450.0f /* 0x431e0000 */ * g_rich_row_height;
        widget_set_position(row->widget,
                            x / g_ui_density - anchor[0],
                            y / g_ui_density - anchor[1]);
    }
    g_rich_chat_ready = 1;
}

/* renderer-owned row metrics (raw 0x283330/0x22d818) */
float g_rich_row_base;    /* 0x283330 — label y base for the diag text */
float g_rich_row_height;  /* 0x22d818 — row height scale (battle-state adjacent) */
float anchor_x_ref;       /* 0x283a00-8 — placeholder, replaced below */
float anchor_y_ref;       /* 0x283a00-4 — placeholder, replaced below */

/* ===== rich binding probe ===== */

/* binding diagnostics snapshot (renderer-owned, raw block 0x1e51e8-0x1e5238) */
uint64_t g_rich_binding_diag[10];   /* 0x1e51e8..0x1e5238 — probe out fields */
char     g_rich_binding_line[0x1c0]; /* 0x1e5238 — formatted binding line */

/* misc.c-owned registry globals used below */
extern uint64_t g_menu_widget_b;    /* 0x22d4f0 — misc.c: registry menu widget */
extern uint64_t g_menu_widget_a;    /* 0x22d4e0 — misc.c: registry menu widget */
extern int      widget_pair_snapshot_read(uint64_t *node_out, int mode, void *out);
                                      /* misc.c @ 0017c704 — {node,parent} pair snapshot */
extern int      widget_ancestor_of(uint64_t node, uint64_t target);
                                      /* misc.c @ 0016d800 — <=16-level parent walk */
extern void     ui_text_style_format_d(void *buf, size_t cap_a, size_t cap_b,
                                       const char *fmt, ...);
                                      /* misc.c @ 00176a24 — checked formatter */

/*
 * rich_binding_probe — validate a live movie-clip binding: node's parent
 * chain reaches the container (or the registry menu widget when the row index
 * hash > 0x2a), +0x30 scene owner is root/NULL, +0x48 flag matches
 * (mode == 2), then by mode: 2 -> widget_pair_snapshot_read; 3 ->
 * vptr == game_base + 0x11ad208 and param_1[2] vptr == +0x11abad8 with
 * scene-owner + ancestor checks. On a mode-2 snapshot failure copies the
 * probe fields to the 0x1e51e8 diag block, formats the 15-field
 * 'ok=%d mode=%u ...' line and logs 'nexus_rich_binding' (capped 64).
 * Returns 1 valid, 0 invalid. Raw arg 2 is the mode int.
 * @ 0017609c
 */
int rich_binding_probe(uint64_t *node_slot, int *mode_ptr)
{
    if ((int)node_slot[0x1a] == 0)
        return 0;

    /* row-index hash: (slot - 0x1d3568)/8 * 0x684bda13 — picks the anchor */
    uint32_t row_hash = (uint32_t)(((uintptr_t)node_slot - 0x1d3568UL) >> 3)
                      * 0x684bda13U;
    const uint64_t *anchor_slot = row_hash > 0x2a
                                ? &g_menu_widget_b   /* 0x22d4f0 */
                                : (const uint64_t *)&g_rich_container;
    if (!widget_child_link_valid(*node_slot, *anchor_slot))
        return 0;

    uint64_t scene_owner = 0;
    uint64_t node = *node_slot;
    if (!(0xfff < node + 0x30)
        || (node & 0xfffffffffffffff8ULL) == 0xffffffffffffffc8ULL
        || g_host_read(g_host_ctx, node + 0x30, &scene_owner, 8) != 1
        || (scene_owner != 0 && scene_owner != g_ui_scene_root))
        return 0;

    int mode = *mode_ptr;
    uint8_t flag = 0xff;
    if (node + 0x49 <= 0x1000
        || g_host_read(g_host_ctx, node + 0x48, &flag, 1) != 1
        || (flag != 0) != (mode == 2))
        return 0;

    if (mode == 2) {
        /* mode 2: full pair snapshot; on failure dump the probe diagnostics */
        uint64_t probe_out[13];
        memset(probe_out, 0, sizeof probe_out);
        if (widget_pair_snapshot_read(node_slot, 1, probe_out) != 0)
            return 0;
        /* snapshot failed: copy the raw probe fields into the diag block */
        g_rich_binding_diag[0] = probe_out[1];   /* 0x1e51e8 */
        g_rich_binding_diag[1] = probe_out[2];   /* 0x1e51f0 */
        g_rich_binding_diag[2] = probe_out[3];   /* 0x1e51f8 */
        g_rich_binding_diag[3] = probe_out[4];   /* 0x1e5200 */
        g_rich_binding_diag[4] = probe_out[5];   /* 0x1e5208 */
        g_rich_binding_diag[5] = probe_out[6];   /* 0x1e5210 */
        g_rich_binding_diag[6] = probe_out[7];   /* 0x1e5218 */
        g_rich_binding_diag[7] = probe_out[8];   /* 0x1e5220 */
        g_rich_binding_diag[8] = probe_out[9];   /* 0x1e5228 (shorts/flags) */
        g_rich_binding_diag[9] = probe_out[10];  /* 0x1e5230 */
        ui_text_style_format_d(g_rich_binding_line, 0x1c0, 0x1c0,
            "ok=%d mode=%u reads=%x button=%llx source=%llx clip80=%llx "
            "display88=%llx sourceb0=%llx mcparent=%llx mcindex=%d "
            "displayparent=%llx displayindex=%d named=%d matches=%u active=%u",
            0, probe_out[11], probe_out[12]);
        uint32_t count = g_rich_stop_log_count + 1;
        bool under_cap = g_rich_stop_log_count < 0x40;
        g_rich_stop_log_count = count;
        if (under_cap && g_host_log != NULL)
            g_host_log(g_host_ctx, "nexus_rich_binding",
                       g_rich_binding_line, row_hash);
        return 0;
    }

    /* mode 3: movie-clip vtable + child display checks */
    uint64_t vptr = 0;
    bool read_ok = false;
    if ((node + 8) >> 3 >= 0x201)
        read_ok = g_host_read(g_host_ctx, node, &vptr, 8) == 1;
    bool plausible = 0xfff < vptr && read_ok && (vptr & 7) == 0;
    if ((plausible ? vptr : 0) != g_game_base + 0x11ad208U)
        return 0;
    if (*mode_ptr != 3)
        return 1;

    uint64_t child = node_slot[2];
    vptr = 0;
    read_ok = false;
    if ((child + 8) >> 3 >= 0x201)
        read_ok = g_host_read(g_host_ctx, child, &vptr, 8) == 1;
    plausible = 0xfff < vptr && read_ok && (vptr & 7) == 0;
    if ((plausible ? vptr : 0) == g_game_base + 0x11abad8U
        && widget_scene_owner_check(child))
        return widget_ancestor_of(child, node) != 0;
    return 0;
}
/* renderer p1 chunk 9 (renderer.c part 2, chunk 2): covers raw lines 2747-2814 */
/* ===== native guards, diagnostics and ops table ===== */

/*
 * nexus_rich_validate_native — native-side guard check for the rich
 * renderer: host table installed, host ready, init latch free, then the
 * engine code snapshot (57 regions memcmp). Returns 1 verified, 0 host not
 * ready / table missing, 3 render in flight (latch held), -1 snapshot
 * mismatch. Clears the init latch on the latch-held-free paths.
 * @ 00176904
 */
uint64_t nexus_rich_validate_native(void)
{
    if (g_host_init_done == 0)
        return 0;
    if (!g_host_ready(g_host_ctx))
        return 0;
    if (ui_latch_test_and_set(1, (volatile int *)&g_host_init_latch) & 1)
        return 3;
    uint64_t result = engine_code_snapshot_verify() != 0 ? 1 : 0xffffffffULL;
    g_host_init_latch = 0;
    return result;
}

/*
 * nexus_rich_diagnostics — format a one-line JSON diagnostics summary into
 * buf (cap = size): {"schema":1,"phase":%d,"built":%u,"planned":%u,
 * "strings":%u,"reason":"%s","projection":"%s","binding":"%s"} pulling the
 * phase/built/planned/strings fields from the rich stage globals, the
 * reason from the status line, the projection from the viewport line and
 * the binding from the binding diag line. Gates: buf and status != NULL,
 * init latch free. Returns the formatted length (min 0), 0 on gates.
 * @ 0017697c
 */
extern uint32_t g_rich_strings_built;  /* 0x1a7d1c — strings built count (p1-adjacent) */

int nexus_rich_diagnostics(char *buf, size_t size)
{
    int len = 0;
    if (buf == NULL || size == 0)
        return 0;
    if (ui_latch_test_and_set(1, (volatile int *)&g_host_init_latch) & 1)
        return 0;
    len = snprintf(buf, size,
        "{\"schema\":1,\"phase\":%d,\"built\":%u,\"planned\":%u,"
        "\"strings\":%u,\"reason\":\"%s\",\"projection\":\"%s\","
        "\"binding\":\"%s\"}",
        g_rich_stage_state, g_rich_menu_entry, g_rich_clip_row,
        g_rich_strings_built /* 0x1a7d1c */, g_rich_status,
        g_rich_viewport_line, g_rich_binding_line);
    g_host_init_latch = 0;
    if (len < 1)
        len = 0;
    return len;
}

/*
 * nexus_rich_renderer_ops_v1 — return the v1 renderer ops table (static
 * rodata at 0x19c048: {abi 1, size 0x28, validate fn, ...}). No gates.
 * @ 00176ac8
 */
extern const uint64_t RICH_RENDERER_OPS_V1[];  /* 0x19c048 — 0x28-byte ops table (rodata) */

void *nexus_rich_renderer_ops_v1(void)
{
    return (void *)&RICH_RENDERER_OPS_V1;   /* 0x19c048 */
}
/* renderer p1 chunk 10 (renderer.c part 2, chunk 3): covers raw lines 2816-3323 */
/* ===== UI graphics release (the big reset) ===== */

/* p1/p2-owned externs for this chunk */
extern void    *g_host_ctx;             /* 0x1a7cc8 — p1 */
extern int    (*g_host_read)(void *ctx, uint64_t addr, void *out, uint32_t len);
                                          /* 0x1a7ce8 — p1 */
extern int    (*g_host_ready)(void *ctx); /* 0x1a7cf0 — p1 */
extern void  (*g_host_log)(void *ctx, const char *cat, const char *event,
                           uint32_t value);
                                          /* 0x1a7d00 — p1 */
extern uint64_t g_game_base;              /* 0x1a7cd0 — p1 */
extern uint64_t g_ui_scene_root;          /* 0x1a7d28 — p1 */
extern uint64_t g_ui_context;             /* 0x1a7d30 — p1 */
extern int      g_host_init_done;         /* 0x1a7d0c — p1 */
extern uint32_t g_host_init_latch;        /* 0x1a7d08 — p1 */
extern int32_t  g_rich_stage_state;       /* 0x1a7d10 — p1 */
extern uint32_t g_rich_hidden_latch;      /* 0x1a7d14 — p1 */
extern uint32_t g_rich_menu_entry;        /* 0x1a7d18 — p1 */
extern uint32_t g_rich_stop_log_count;    /* 0x1a7d20 — p1 */
extern uint64_t g_rich_container;         /* 0x1a7d38 — p1 */
extern uint64_t g_rich_perf_deadline;     /* 0x1a7d40 — p1 */
extern uint64_t g_rich_perf_key;          /* 0x1a7d48 — p1 */
extern const char *g_rich_status;         /* 0x1a7d50 — p1 */
extern uint32_t g_rich_clip_row;          /* 0x1a7d58 — p1 */
extern float    g_ui_density;             /* 0x1e53fc — p1 */
extern float    g_ui_center_x;            /* 0x1e5400 — p1 */
extern float    g_ui_center_y;            /* 0x1e5404 — p1 */
extern uint64_t g_rich_plus_layout_lo;    /* 0x1e5168 — p1 */
extern uint64_t g_rich_plus_layout_mid;   /* 0x1e5170 — p1 */
extern uint64_t g_rich_plus_layout_hi;    /* 0x1e5178 — p1 */
extern char     g_rich_binding_line[0x1c0]; /* 0x1e5238 — c07 */
extern uint64_t g_rich_binding_diag[10];   /* 0x1e51e8..0x1e5238 — c07 */
extern uint64_t g_toast_widget;           /* 0x281a50 — p1 */
extern uint64_t g_toast_context;          /* 0x281a58 — p1 */
extern uint64_t g_toast_text_obj;         /* 0x281a60 — p1 */
extern uint32_t g_toast_failed;           /* 0x281a68 — p1 */
extern uint32_t g_toast_visible;         /* 0x281a6c — p1 */
extern uint8_t  g_toast_scaled;          /* 0x281a70 — p1 */
extern char     g_toast_text[0x80];       /* 0x281a74 — p1 */
extern float    g_toast_scale;            /* 0x281af4 — p1 */
extern float    g_toast_density;          /* 0x281af8 — p1 */
extern float    g_toast_center_x;         /* 0x281afc — p1 */
extern float    g_toast_center_y;         /* 0x281b00 — p1 */
extern rich_chat_row_t g_rich_chat_rows[2];  /* 0x284220 — c07 */
extern uint64_t g_rich_chat_owner;        /* 0x2843d0 — c07 */
extern uint8_t  g_rich_chat_built;        /* 0x2843d8 — c07 */
extern uint32_t g_rich_chat_ready;        /* 0x2839fc — c07 */
extern char     g_rich_chat_text[0x820];  /* 0x283a00 — c07 */
extern float    g_rich_row_base;          /* 0x283330 — c07 */
extern float    g_rich_row_height;        /* 0x22d818 — c07 */
extern int      rich_binding_probe(uint64_t *node_slot, int *mode_ptr);
                                          /* c07 @ 0017609c */
extern int      ui_latch_test_and_set(int value, volatile int *latch);
                                          /* menu_engine.c @ 00193f80 */

/* misc.c helpers */
extern int      ui_scene_fonts_ready(void);   /* misc.c @ 0016d910 */
extern int      remote_vtable_matches(uint64_t obj, uint64_t rva);
                                          /* misc.c @ 0016debc */
extern int      widget_child_link_valid(uint64_t node, uint64_t parent);
                                          /* misc.c @ 0016d5cc */
extern int      widget_scene_owner_check(uint64_t widget);
                                          /* misc.c @ 0016df78 */
extern int      widget_flag48_equals(uint64_t widget, int expected);
                                          /* misc.c @ 0016e464 */
extern int      movie_frame_count_check(uint64_t movie, int idx, int min);
                                          /* misc.c @ 0016dc20 */
extern int      rich_panel_page_state_build(void *page, uint64_t display);
                                          /* misc.c @ 00176d98 — 0xc10 panel page */
extern int      widget_kind1_validate(uint64_t widget, uint64_t ctx, void *snap);
                                          /* misc.c @ 00177620 — kind-1 deep validate */
extern int      ui_clips_ready_check(void);   /* misc.c @ 00179448 */
extern int      widget_pair_snapshot_read(uint64_t *node_out, int mode, void *out);
                                          /* misc.c @ 0017c704 */
extern int      captured_pair_pump(int phase);
                                          /* misc.c @ 0014f830 — overlay pair verify/publish */
extern int      widget_detach_release(uint64_t widget, int mode, int *touched);
                                          /* misc.c @ 00177834 — detach + release by mode */
extern int      ui_clip_cache_release(void);  /* misc.c @ 00178714 */
extern int      widget_detach_and_verify(uint64_t widget);
                                          /* misc.c @ 001789a8 */
extern void     ui_state_cache_reset(void);   /* misc.c @ 00178ab0 */
extern int      theme_preview_orphaned(uint64_t preview, uint64_t ctx);
                                          /* misc.c @ 00182b58 — nonzero = stale */

/* misc.c-owned registry/theme globals */
extern uint64_t g_menu_widget_a;        /* 0x22d4e0 — registry menu widget */
extern uint64_t g_menu_widget_b;        /* 0x22d4f0 — registry menu widget */
extern uint64_t g_theme_preview_obj;    /* 0x284650 */
extern uint64_t g_theme_preview_ctx;    /* 0x284658 */
extern uint32_t g_theme_preview_done;   /* 0x284668 */
extern uint32_t g_theme_preview_retries; /* 0x284670 low */

/* rich row table: 0x280 records of 0x1b qwords at 0x1c3568 (misc.c data) */
extern uint64_t RICH_ROW_TABLE[][0x1b];  /* 0x1c3568 — 640 records (0x22d4e0-adjacent) */
/* per-row mode array at 0x1a7d68, one int per row (0x2c int stride in raw) */
extern uint32_t RICH_ROW_MODES[];        /* 0x1a7d68 — 2 = pair-snapshot row */

/* clip snapshot pair captured before release (raw locals, kept as globals
 * only for the doc; they are stack values in the raw) */

/*
 * nexus_ui_graphics_resources — verify the graphics state (phase 0) or tear
 * the whole rich UI down (phase 1). Gates: phase <= 1, host table installed,
 * host ready, init latch free, scene root/view match the stashed pair,
 * ui_scene_fonts_ready, scene-root +0x19c byte clear, stage in {0,1,2} with
 * hidden latch clear, menu entries <= 0x280, display not the view/root, then
 * widget_kind1_validate(display). Phase 0 returns 1 when every check holds.
 * Phase 1: validates the container (vtable 0x11abcf0, +0x4e count 0x2c,
 * clips ready), walks all 0x280 row-table records (identity vs container /
 * registry widgets / view / display, duplicates, rich_binding_probe), checks
 * the toast widget chain, builds the 0xc10 panel page twice, releases the
 * theme preview when orphaned, detaches/releases every live row by mode
 * (engine 0x59567c / 0x5940d0 / 0x5d48e8 / 0x88991c), then zeroes the whole
 * UI state block (0x283338, 0x282b10, 0x283a00, the 0x284220 rows, 0x282150
 * fps popup, 0x281b08 chooser, the 0x2843e0-0x284648 tail) and — when
 * captured_pair_pump(1) succeeds — releases the remaining rows, the toast
 * and the display, resets the rich stage globals, clears the 0x1a7d58
 * 0x3d410 plan block and sets the status to "waiting resources after
 * reload". On an unverified release: stage := -1 (hidden latch preserved),
 * status "graphics_owned_release_unverified", capped-64
 * 'nexus_rich_stopped' log. Returns 1 (phase 0 verified / phase 1 released),
 * 0 on gates — via the raw canary feed (typed int for p1's extern).
 * @ 00177afc
 */
int nexus_ui_graphics_resources(uint64_t scene_root, uint64_t scene_view,
                                 uint64_t display, uint32_t phase)
{
    if (phase > 1u || g_host_init_done == 0)
        return 0;
    if (!g_host_ready(g_host_ctx))
        return 0;
    if (ui_latch_test_and_set(1, (volatile int *)&g_host_init_latch) & 1)
        return 0;

    int touched = 0;                 /* raw local_c90: any release performed */
    uint64_t clip_pair[2] = {0};     /* raw locals local_c78/local_c68 */

    if (scene_view == 0 || scene_root == 0
        || g_ui_scene_root != scene_root || g_ui_context != scene_view
        || !ui_scene_fonts_ready())
        goto out;

    {
        uint8_t scene_byte = 1;
        if (g_ui_scene_root + 0x19dU >= 0x1001
            && g_host_read(g_host_ctx, g_ui_scene_root + 0x19c,
                           &scene_byte, 1) == 1
            && scene_byte == 0
            && ((uint32_t)g_rich_stage_state & ~2u) == 0
            && g_rich_menu_entry <= 0x280
            && (g_toast_failed & 1) == 0
            && !(display == scene_view || display == 0 || display == scene_root)
            && display != g_rich_container
            && display != g_toast_widget) {
            uint8_t kind1_snap[16] = {0};
            if (widget_kind1_validate(display, scene_view, kind1_snap) == 0)
                goto out;

            if (g_rich_stage_state != 0) {
                /* live stage: the container must be a healthy clip host */
                clip_pair[0] &= 0xffffffffffff0000ULL;
                if (g_rich_container != scene_view && g_rich_container != 0
                    && g_rich_container != scene_root
                    && g_rich_container != g_toast_widget
                    && remote_vtable_matches(g_rich_container, 0x11abcf0)
                    && widget_child_link_valid(g_rich_container, scene_view)
                    && widget_scene_owner_check(g_rich_container)
                    && widget_flag48_equals(g_rich_container, 1)
                    && 0xfff < g_rich_container + 0x4e
                    && (g_rich_container & 0xfffffffffffffffeULL)
                           != 0xffffffffffffffb0ULL
                    && g_host_read(g_host_ctx, g_rich_container + 0x4e,
                                   clip_pair, 2) == 1
                    && (int16_t)clip_pair[0] == 0x2c
                    && g_rich_clip_row == g_rich_menu_entry
                    && ui_clips_ready_check())
                    goto rows_walk;
                goto out;
            }
            if (g_rich_container != 0 || g_rich_menu_entry == 0
                || ui_clips_ready_check() == 0)
                goto out;

        rows_walk:
            /* walk all 0x280 row-table records */
            for (uint32_t i = 0; i < 0x280; i++) {
                uint64_t *rec = RICH_ROW_TABLE[i];
                uint64_t node = rec[0];
                if (i < g_rich_menu_entry) {
                    if (node == 0 || node == g_ui_scene_root)
                        goto out;
                    /* raw NEON cmeq: node must not match the container, the
                     * registry widget pair, the view or the display */
                    if (node == g_rich_container || node == g_menu_widget_a
                        || node == g_menu_widget_b || node == g_ui_context
                        || node == display)
                        goto out;
                    if (i != 0) {
                        if (RICH_ROW_TABLE[0][0] == node)
                            goto out;
                        /* no earlier record may hold the same node */
                        uint32_t j = 0;
                        uint64_t prev;
                        do {
                            if (i - 1 == j)
                                goto probe;
                            prev = RICH_ROW_TABLE[j + 1][0];
                            j++;
                        } while (prev != node);
                        if (j < i)
                            goto out;
                    }
                probe:
                    if (rich_binding_probe(rec, (int *)RICH_ROW_MODES + i) == 0)
                        goto out;
                    if (rec[2] == clip_pair[0] || rec[2] == clip_pair[1]
                        || (rec[3] != 0
                            && (rec[3] == clip_pair[0]
                                || rec[3] == clip_pair[1])))
                        goto out;
                    if (i != 0) {
                        /* no earlier record may reference this node either */
                        for (uint32_t j = 0; j < i; j++) {
                            if (node == RICH_ROW_TABLE[j][3]
                                || node == RICH_ROW_TABLE[j + 1][3]
                                || (rec[3] != 0
                                    && (rec[3] == RICH_ROW_TABLE[j][3]
                                        || rec[3] == RICH_ROW_TABLE[j + 1][3])))
                                goto out;
                        }
                    }
                    if (RICH_ROW_MODES[i] == 2) {
                        if (widget_pair_snapshot_read(rec, 1, clip_pair) == 0)
                            goto out;
                    } else if (rec[3] != 0 || rec[0] != node) {
                        goto out;
                    }
                } else if (rec[0] != 0 || rec[2] != 0 || rec[3] != 0
                           || rec[4] != 0 || RICH_ROW_TABLE[i][0x10] != 0) {
                    goto out;   /* unused record must be fully zero */
                }
            }

            /* toast widget chain must still be healthy */
            if (g_toast_widget != 0) {
                if (g_toast_widget != scene_root && g_toast_widget != scene_view
                    && g_toast_text_obj != 0
                    && g_toast_context == scene_view
                    && movie_frame_count_check(g_toast_widget, 5, 0)
                    && widget_child_link_valid(g_toast_widget, scene_view)
                    && widget_flag48_equals(g_toast_widget, 0)
                    && remote_vtable_matches(g_toast_text_obj, 0x11abad8)
                    && widget_scene_owner_check(g_toast_text_obj)
                    && widget_ancestor_of(g_toast_text_obj, g_toast_widget))
                    goto release_phase;
                goto out;
            }
            if (g_toast_text_obj != 0 || g_toast_context != 0)
                goto out;

        release_phase:
            if (rich_panel_page_state_build(clip_pair, display) == 0)
                goto out;
            if (captured_pair_pump(0) == 0)
                goto out;

            if (phase == 0) {
                return 1;   /* verified, nothing to release */
            }
            if (rich_panel_page_state_build(clip_pair, display) == 0) {
                if (touched == 0)
                    goto out;
                goto out;
            }

            /* release the theme preview when it went stale */
            if (g_theme_preview_obj != 0) {
                touched = 1;
                if (theme_preview_orphaned(g_theme_preview_obj,
                                           g_theme_preview_ctx) == 0)
                    goto out_with_touched;
                g_theme_preview_retries = 0;
                g_theme_preview_ctx = 0;
                g_theme_preview_obj = 0;
                g_theme_preview_done = 0;
            }

            /* detach/release every live row (mode from the row table) */
            {
                /* raw: local_c70 table of {node, mode} pairs, count local_80 */
                uint32_t live_rows = 0;
                for (uint32_t i = 0; i < g_rich_menu_entry; i++) {
                    uint64_t node = RICH_ROW_TABLE[i][0];
                    uint32_t mode = RICH_ROW_MODES[i];
                    if (mode < 2) {
                        if (widget_detach_release(node, (int)mode, &touched) == 0)
                            goto out_with_touched;
                    } else {
                        if (mode == 3) {
                            /* +0x4e must be empty (0) before release */
                            uint64_t tmp = 1;
                            if (node + 0x4e < 0x1000
                                || (node & 0xfffffffffffffffeULL)
                                       == 0xffffffffffffffb0ULL
                                || g_host_read(g_host_ctx, node + 0x4e,
                                               &tmp, 2) != 1
                                || (int16_t)tmp != 0)
                                goto out_with_touched;
                            mode = RICH_ROW_MODES[i];
                        }
                        touched = 1;
                        uint32_t release_rva = 0x5d48e8;
                        if (mode != 4) {
                            /* engine 0x59567c: detach */
                            ((void (*)(uint64_t))
                             (g_game_base + 0x59567c))(node);
                            /* verify node +0x38 == 0 and +0x40 == -1 */
                            uint64_t parent = 1;
                            int32_t index = 0;
                            if (node + 0x38 < 0x1000
                                || (node & 0xfffffffffffffff8ULL)
                                       == 0xffffffffffffffc0ULL
                                || g_host_read(g_host_ctx, node + 0x38,
                                               &parent, 8) != 1
                                || parent != 0
                                || node + 0x40 < 0x1000
                                || (node & 0xfffffffffffffffcULL)
                                       == 0xffffffffffffffbcULL
                                || g_host_read(g_host_ctx, node + 0x40,
                                               &index, 4) != 1
                                || index != -1)
                                goto out_with_touched;
                            release_rva = mode == 3 ? 0x5940d0 : 0x5d48e8;
                        }
                        ((void (*)(uint64_t))
                         (g_game_base + release_rva))(node);
                        live_rows++;
                    }
                    RICH_ROW_TABLE[i][0] = 0;
                }
            }

            /* full UI state block reset */
            memset((void *)0x283338, 0, 0x6b8);
            memset((void *)0x282b10, 0, 0x820);
            memset((void *)0x283a00, 0, 0x820);
            memset(&g_rich_chat_rows, 0, sizeof g_rich_chat_rows);
            g_rich_chat_ready = 0;
            g_rich_chat_built = 0;
            g_rich_chat_owner = 0;
            g_rich_row_base = 0;
            memset((void *)0x282150, 0, 0x9b8);   /* fps popup block */
            memset((void *)0x281b08, 0, 0x648);   /* font chooser block */

            int pump = captured_pair_pump(1);
            if (pump == 1) {
                touched = 1;
                /* release every remaining live row through the channel */
                for (uint32_t i = 0; i < g_rich_menu_entry; i++) {
                    if (widget_detach_release(RICH_ROW_TABLE[i][0],
                                              RICH_ROW_MODES[i] == 2,
                                              &touched) == 0)
                        goto out_with_touched;
                    memset(RICH_ROW_TABLE[i], 0,
                           sizeof(RICH_ROW_TABLE[i]));
                }
                if (ui_clip_cache_release() != 0) {
                    if (g_rich_container == 0) {
                        /* release the toast widget, then the display */
                        if (g_toast_widget != 0) {
                            if (widget_detach_release(g_toast_widget, 0,
                                                      &touched) == 0)
                                goto out_with_touched;
                            g_toast_context = 0;
                            g_toast_text_obj = 0;
                            g_toast_widget = 0;
                        }
                        if (widget_detach_release(display, 1, &touched) != 0) {
                            g_rich_stage_state = 0;
                            g_rich_menu_entry = 0;
                            g_rich_perf_key = 0;
                            g_ui_context = 0;
                            g_ui_scene_root = 0;
                            g_rich_perf_deadline = 0;
                            g_rich_container = 0;
                            ui_state_cache_reset();
                            memset((void *)0x1a7d58, 0, 0x3d410);
                            g_toast_text_obj = 0;
                            g_rich_binding_line[0] = '\0';
                            memset(g_rich_binding_diag, 0,
                                   sizeof g_rich_binding_diag);
                            g_toast_widget = 0;
                            g_ui_density = 0;
                            g_rich_status = "waiting resources after reload";
                            g_ui_center_x = 0;    /* raw: 0x1e5400 bytes 1-3 */
                            g_ui_center_y = 0;
                            g_toast_visible = 0;
                            g_toast_context = 0;
                            g_toast_text[0] = '\0';
                            g_toast_failed = 0;
                            g_toast_scaled = 0;
                            g_toast_center_y = 0;
                            g_toast_density = 0;
                            g_toast_center_x = 0;
                            g_toast_scale = 0;
                            goto out;   /* released cleanly */
                        }
                    } else {
                        /* release the container first, then fall through */
                        uint16_t count16 = 1;
                        if (0xfff < g_rich_container + 0x4e
                            && (g_rich_container & 0xfffffffffffffffeULL)
                                   != 0xffffffffffffffb0ULL
                            && g_host_read(g_host_ctx, g_rich_container + 0x4e,
                                           &count16, 2) == 1
                            && count16 == 0) {
                            touched = 1;
                            if (widget_detach_and_verify(g_rich_container) != 0) {
                                ((void (*)(uint64_t))
                                 (g_game_base + 0x5940d0))(g_rich_container);
                                g_rich_container = 0;
                                /* fall through to the toast/display release */
                                if (g_toast_widget != 0) {
                                    if (widget_detach_release(g_toast_widget, 0,
                                                              &touched) == 0)
                                        goto out_with_touched;
                                    g_toast_context = 0;
                                    g_toast_text_obj = 0;
                                    g_toast_widget = 0;
                                }
                                if (widget_detach_release(display, 1,
                                                          &touched) != 0) {
                                    g_rich_stage_state = 0;
                                    g_rich_menu_entry = 0;
                                    g_rich_perf_key = 0;
                                    g_ui_context = 0;
                                    g_ui_scene_root = 0;
                                    g_rich_perf_deadline = 0;
                                    g_rich_container = 0;
                                    ui_state_cache_reset();
                                    memset((void *)0x1a7d58, 0, 0x3d410);
                                    g_toast_text_obj = 0;
                                    g_rich_binding_line[0] = '\0';
                                    memset(g_rich_binding_diag, 0,
                                           sizeof g_rich_binding_diag);
                                    g_toast_widget = 0;
                                    g_ui_density = 0;
                                    g_rich_status =
                                        "waiting resources after reload";
                                    g_ui_center_y = 0;
                                    g_toast_visible = 0;
                                    g_toast_density = 0;
                                    g_toast_scale = 0;
                                    goto out;
                                }
                            }
                        }
                        goto out_with_touched;
                    }
                }
                goto out_with_touched;
            }
            if (pump >= 0)
                goto out_with_touched;
            touched = 1;
        }
    }

    /* unverified release: stop the renderer, keep the hidden latch */
    g_ui_scene_root = 0;
    g_ui_context = 0;
    g_rich_stage_state = -1; /* raw CONCAT44(d14, -1): 32-bit state store; the
                              * hidden latch in the high dword (0x1a7d14) is
                              * left untouched by this write */
    g_rich_status = "graphics_owned_release_unverified";
    {
        uint32_t count = g_rich_stop_log_count + 1;
        bool under_cap = g_rich_stop_log_count < 0x40;
        g_rich_stop_log_count = count;
        if (under_cap && g_host_log != NULL)
            g_host_log(g_host_ctx, "nexus_rich_stopped",
                       "graphics_owned_release_unverified", 0);
    }

out_with_touched:
    (void)touched;
out:
    g_host_init_latch = 0;
    return 0;
}
/* renderer p1 chunk 11 (renderer.c part 2, chunk 4): covers raw lines 3325-3730 */
/* ===== main frame, label state, panel buttons ===== */

/* cross-file externs used by this chunk */
extern int  nexus_menu_main_count(void);
                                      /* menu_engine.c @ 0018975c — entries in the main menu */
extern int  ui_viewport_cache_check(void);
                                      /* misc.c @ 001795ac — re-read viewport, compare cache */
extern int  ui_pointer_coords_transform(int engage, void *coords_out, void *state);
                                      /* misc.c @ 0017975c — mutex-protected pointer update */
extern int  ui_pointer_events_handle(uint64_t frame, int mode, uint64_t scroll,
                                     void *batch, uint32_t batch_size,
                                     uint64_t page_span);
                                      /* misc.c @ 00179000 — pointer batch dispatch */
extern int  ui_clip_y_snap(uint64_t scroll_state);
                                      /* misc.c @ 00179c50 — snap clip y, cache 0x22d7e8 */
extern void inertia_state_step(int a, int b, void *state, uint64_t frame,
                               int c, int d, int32_t e, uint64_t page_span);
                                      /* misc.c @ 00183230 — inertial scroll advance */
extern uint32_t nexus_rich_main_row(void);
                                      /* menu_engine.c @ 00178b1c */
extern int  widget_ancestor_of(uint64_t node, uint64_t target);
                                      /* misc.c @ 0016d800 — <=16-level parent walk */
extern int  widget_detached_check(uint64_t node);
                                      /* misc.c @ 0016ddc8 — parent NULL and index -1 */
extern int  widget_chain_links_unique_check(uint64_t node, uint64_t child);
                                      /* misc.c @ 0016e020 */
extern void engine_call_594e80(uint64_t obj, int a);
                                      /* misc.c @ 0016ce20 */
extern void engine_call_5988ec(uint64_t parent, uint64_t child);
                                      /* misc.c @ 0016cfb8 — attach child to parent */
extern int  engine_call_594130(uint64_t a, uint64_t b);
                                      /* misc.c @ 0016cfa4 — engine base + 0x594130 */
extern int  widget_rich_panel_bound_check(uint64_t widget);
                                      /* misc.c @ 0017c598 — bound under 0x283340 (<=16) */
extern void engine_call_66ae58(void *out, const char *text);
                                      /* misc.c @ 0016ced8 — string object create */
extern void engine_call_5921a0(uint64_t obj, void *text);
                                      /* misc.c @ 0016ceac */
extern void game_call_66ad48(void *obj);
                                      /* misc.c @ 0014ec30 — forward via base + 0x66ad48 */
extern void engine_call_592250(uint64_t obj, uint32_t arg);
                                      /* misc.c @ 0016ceec */
extern int  rich_binding_bind(uint64_t *row_slot, uint64_t display,
                              uint32_t row_index);
                                      /* renderer.c p2 @ 0017c42c */
extern int  ui_clips_ready_check(void);   /* misc.c @ 00179448 */

/* misc.c-owned pointer/scroll state block (0x22d518..0x22d5f8, p2/p3 misc) */
extern uint64_t g_scroll_state_block;  /* 0x22d518 — inertia state (misc p2 owns) */
extern uint32_t g_pointer_active;      /* 0x22d564 */
extern float    g_scroll_target;       /* 0x22d560 */
extern uint64_t g_pointer_seq_pair;    /* 0x22d580 — {seq, page} packed */
extern uint64_t g_pointer_source;      /* 0x22d588 */
extern uint64_t g_pointer_last_frame;  /* 0x22d590 */
extern uint64_t g_pointer_touch_frame; /* 0x22d5a0 */
extern uint32_t g_pointer_repeat_count; /* 0x22d5a8 */
extern uint64_t g_pointer_param_6;     /* 0x22d5c8 */
extern uint64_t g_pointer_cached_arg;  /* 0x22d5f8 */
extern uint32_t g_scroll_position;     /* 0x22d518 low (see misc p2) */
extern uint64_t g_pointer_event_arg;   /* 0x22d588 (raw reuses the slot) */
extern uint64_t g_pointer_widget;      /* 0x22d568 */
extern uint64_t g_pointer_guard_lo;    /* 0x22d56c */
extern uint64_t g_pointer_guard_hi;    /* 0x22d570 */
extern uint32_t g_scroll_revision;     /* 0x22d584 (g_menu_scroll_revision family) */

/* main-frame scroll anchors (menu_engine.c-owned) */
extern uint64_t g_main_scroll_anchor;  /* 0x22d7a8 — menu_engine: scroll anchor */
extern uint64_t g_main_frame_ptr;      /* 0x22d7b8 — menu_engine: current frame arg */
extern uint32_t g_main_page_index;     /* 0x22d794 — menu_engine: page index */
extern uint64_t g_main_row_last;       /* 0x22d79c — menu_engine: last main row */
extern uint64_t g_main_field_798;      /* 0x22d798 — menu_engine: engage flag */
extern uint64_t g_main_field_7c0;      /* 0x22d7c0 — frame counter */

/* rodata pointer-snapshot templates */
extern const uint64_t POINTER_SNAP_TEMPLATE_LO; /* 0x10fa90 */
extern const uint64_t POINTER_SNAP_TEMPLATE_HI; /* 0x10fa98 */

/*
 * nexus_rich_main_frame — per-frame main-menu pump. Gates: host table
 * installed, host ready, param_2 <= 1, init latch free. Computes the page
 * span (nexus_menu_main_count()+4)/5; when the stage is not attached (or
 * param_3 == 0 / engage flag 0): (re)seeds the pointer/scroll state block
 * from the 0x10fa90/0x10fa98 templates, syncs {seq, page} to the anchor,
 * runs inertia_state_step and stores the frame arg. When attached: verifies
 * the scene-root +0x19c byte and the clips, transforms pointer coords
 * (param_2 selects engage vs query mode), dispatches pointer events, bumps
 * the frame counter, snaps the clip y when the main row changed and stops
 * with 'main_page_motion' when the snap fails; success clears the perf
 * deadline. Raw returns 0/1 through the canary feed. Side effects: clears
 * the init latch on every exit.
 * @ 00178c78
 */
void nexus_rich_main_frame(uint64_t frame, uint32_t mode, int attached)
{
    if (g_host_init_done == 0)
        return;
    if (1u < mode || !g_host_ready(g_host_ctx))
        return;
    if (ui_latch_test_and_set(1, (volatile int *)&g_host_init_latch) & 1)
        return;

    int count = nexus_menu_main_count();
    uint64_t page_span = (uint64_t)(count + 4) / 5;

    if (attached == 0 || g_main_field_798 == 0
        || g_rich_stage_state != 2 || g_rich_hidden_latch != 0) {
        /* idle: (re)seed the pointer/scroll state block */
        if (g_scroll_target == 0) {
            g_pointer_cached_arg = 0;
            g_scroll_target = 1;
            g_pointer_param_6 = 0;
            memset((void *)0x22d5e0, 0, 0x18);   /* 0x22d5e0/0x22d5e8/0x22d5f0 */
            memset((void *)0x22d5b0, 0, 0x28);   /* 0x22d5b0..0x22d5d8 */
            memset((void *)0x22d598, 0, 0x10);   /* 0x22d598/0x22d5a0 */
            memset((void *)0x22d568, 0, 0x18);   /* 0x22d568..0x22d580 */
            g_pointer_seq_pair = 0xffffffff00000000ULL;
            g_pointer_source = g_main_scroll_anchor;
            g_pointer_last_frame = frame;
        } else if (g_pointer_source != g_main_scroll_anchor) {
            g_pointer_seq_pair = 0xffffffff00000000ULL;
            g_pointer_source = g_main_scroll_anchor;
        }
        /* {seq, page} high dword := page span (raw CONCAT44 keep-seq) */
        g_pointer_seq_pair = (g_pointer_seq_pair & 0xffffffffULL)
                           | ((uint64_t)(uint32_t)page_span << 32);
        g_pointer_guard_hi = POINTER_SNAP_TEMPLATE_HI;   /* 0x22d578 */
        g_pointer_guard_lo = POINTER_SNAP_TEMPLATE_LO;   /* 0x22d570 */
        g_pointer_active = 0;
        inertia_state_step(0, 0, &g_scroll_state_block, frame, 0, 4,
                           0xffffffff, page_span);
        g_pointer_last_frame = frame;
        g_main_page_index = 0;
        goto out;
    }

    /* attached: run the pointer pipeline */
    if (!ui_scene_fonts_ready())
        goto reset_idle;
    {
        uint8_t scene_byte = 1;
        if (g_ui_scene_root + 0x19dU >= 0x1001
            && g_host_read(g_host_ctx, g_ui_scene_root + 0x19c,
                           &scene_byte, 1) == 1
            && scene_byte == 0
            && ui_clips_ready_check()
            && ui_viewport_cache_check()) {
            if (mode != 0) {
                g_main_frame_ptr = frame;
                int r = ui_pointer_coords_transform(1, &g_pointer_widget /* 0x22d600 */,
                                                    &g_main_field_798 /* 0x22d790 */);
                g_main_page_index = r == 1;
                if (r < 0 || r == 1)
                    goto out;
                goto reset_idle;
            }
            uint8_t batch[400] = {0};
            uint32_t batch_size = 0;
            int r = ui_pointer_coords_transform(0, batch, &batch_size);
            if (r < 0)
                goto out_zero;      /* raw: uVar5 = 0 on the snap-fail path */
            if (r == 0)
                goto reset_idle;
            ui_pointer_events_handle(frame, 1, g_main_scroll_anchor, batch,
                                     batch_size, page_span);
            g_main_field_7c0++;
            if (nexus_rich_main_row() == (uint32_t)g_main_row_last) {
                if (ui_clip_y_snap(g_scroll_state_block /* 0x22d518 */) == 0) {
                    g_rich_stage_state = -1;
                    g_rich_status = "main_page_motion";
                    uint32_t cnt = g_rich_stop_log_count + 1;
                    bool over_cap = 0x3f < g_rich_stop_log_count;
                    g_rich_stop_log_count = cnt;
                    if (over_cap || g_host_log == NULL)
                        goto out;
                    g_host_log(g_host_ctx, "nexus_rich_stopped",
                               "main_page_motion", 0);
                }
            } else {
                g_rich_perf_deadline = 0;   /* raw: uVar5 = 1 */
            }
            goto out;
        }
    }

reset_idle:
    g_pointer_active = 0;
    g_pointer_guard_hi = POINTER_SNAP_TEMPLATE_HI;
    g_pointer_guard_lo = POINTER_SNAP_TEMPLATE_LO;
    inertia_state_step(0, 0, &g_scroll_state_block, frame, 0, 4,
                       0xffffffff, page_span);
    g_main_page_index = 0;

out_zero:
    ;
out:
    g_host_init_latch = 0;
}

/* ===== rich label state (anchored text with 0x820 state) ===== */

/*
 * rich_label_state_render — render an anchored text label from a 0x820-byte
 * state block {widget, text_obj, owner_ctx, cached_text[0x808], failed flag
 * @ +0x818, anchor_y @ +0x81c}. Gates: owner is NULL or the UI context and
 * the widget's parent chain (<=16 levels, each link validated through
 * host_read) reaches the widget; text != NULL && text[0] != 0 and the
 * failed flag clear and ui_scene_fonts_ready. Builds the label once
 * (bundle from nexus_rich_asset(5), styled object 0x134a97, vtable
 * 0x11abad8, attached to the scene root, +0x48 flag 0), updates the text
 * when it changed (cap 0x7ff, cached at state+0x18, engine 0x66ad48 +
 * 0xffffffff arg), reads the char count at text_obj + 0xb0 (1..0x100),
 * scales font_size to density*count (clamped 0.01..8.0), parks the parent
 * at (0,0), scales the text object, anchors it and positions at
 * ((x-center_x)/density-anchor_x, (y-center_y)/density-anchor_y), storing
 * the anchor delta at +0x81c. Empty text parks the widget at (-10000,
 * -10000) and zeroes +0x81c. @ 00179f68
 */
void rich_label_state_render(float x, float y, float font_size,
                             uint64_t *state, const char *text)
{
    /* verify ownership + the parent chain of the widget */
    if (state[2] == 0 || state[2] == g_ui_context) {
        if (state[0] != 0) {
            if (widget_child_link_valid(state[0], state[2])) {
                uint64_t widget = state[0];
                uint64_t node = state[1];
                if (node != 0) {
                    uint32_t depth = 0;
                    for (;;) {
                        if (node == widget || 0xf < depth)
                            break;
                        uint64_t parent = 0;
                        bool read_ok = false;
                        if ((node + 0x40) >> 3 >= 0x201)
                            read_ok = g_host_read(g_host_ctx, node + 0x38,
                                                  &parent, 8) == 1;
                        bool plausible = 0xfff < parent && read_ok
                                      && (parent & 7) == 0;
                        uint64_t next = plausible ? parent : 0;
                        if (!widget_child_link_valid(node, next))
                            goto reset;
                        depth++;
                        node = parent;
                        if (!plausible) {
                            node = 0;
                            break;
                        }
                    }
                }
                if (node == widget)
                    goto verified;
            }
            goto reset;
        }
    } else {
reset:
        memset(state, 0, 0x820);
    }

verified:
    if (text == NULL || text[0] == '\0') {
        if (state[0] != 0)
            widget_set_position(state[0], -10000.0f, -10000.0f);
        *(float *)((char *)state + 0x81c) = 0.0f;
        return;
    }

    if ((int)state[0x103] != 0 || !ui_scene_fonts_ready())
        return;
    {
        uint8_t scene_byte = 1;
        if (g_ui_scene_root + 0x19dU < 0x1001
            || g_host_read(g_host_ctx, g_ui_scene_root + 0x19c,
                           &scene_byte, 1) != 1
            || scene_byte != 0)
            return;

        if (state[0] == 0) {
            /* build the label once */
            if (!g_host_style(g_host_ctx, nexus_rich_asset(5)))
                return;
            if (engine_code_snapshot_verify() == 0)
                return;
            uint64_t bundle = engine_sc_bundle_load(nexus_rich_asset(5));
            if (!movie_frame_count_check(bundle, 5, 0))
                return;
            if (!widget_detached_check(bundle))
                return;
            engine_call_5d7c30(bundle, 0);
            uint64_t text_obj = engine_call_5d8ad4(bundle, RICH_TOAST_STYLE);
            if (!remote_vtable_matches(text_obj, 0x11abad8))
                return;
            if (!widget_scene_owner_check(text_obj))
                return;
            if (!widget_chain_links_unique_check(bundle, text_obj))
                return;
            engine_call_594e80(bundle, 0);
            widget_set_position(bundle, -10000.0f, -10000.0f);
            engine_call_5988ec(g_ui_scene_root, bundle);
            if (!widget_child_link_valid(bundle, g_ui_context))
                goto build_failed;
            if (!widget_flag48_equals(bundle, 0))
                goto build_failed;
            state[0] = bundle;
            state[1] = text_obj;
            state[2] = g_ui_context;
            goto set_text;
        build_failed:
            *(uint32_t *)(state + 0x103) = 1;   /* +0x818: failed flag */
            return;
        }

    set_text:
        if (strcmp(text, (const char *)(state + 3)) != 0) {
            size_t len = strlen(text);
            if (0x7ff < len)
                return;
            uint64_t str_obj = 0;
            engine_call_66ae58(&str_obj, text);
            engine_call_5921a0(state[1], &str_obj);
            game_call_66ad48(&str_obj);
            memcpy(state + 3, text, len + 1);   /* cached at +0x18 */
            engine_call_592250(state[1], 0xffffffff);
        }

        /* char count at text_obj + 0xb0 (2 bytes, 1..0x100) */
        uint16_t count = 0;
        bool read_bad = true;
        uint64_t addr = state[1] + 0xb0;
        if (0xfff < addr && (state[1] & 0xfffffffffffffffeULL)
                             != 0xffffffffffffff4eULL) {
            read_bad = g_host_read(g_host_ctx, addr, &count, 2) != 1;
        }
        if (read_bad || count == 0 || count >= 0x101
            || !(0.0f < g_ui_density))
            return;

        float scale = font_size / (g_ui_density * (float)count);
        if (!isfinite(scale) || !(0.01f <= scale) || !(scale <= 8.0f))
            return;

        widget_set_position(state[0], 0, 0);
        widget_set_position(state[1], 0, 0);
        widget_set_scale(state[1], scale, scale);
        float anchor[2] = {0};
        widget_set_anchor(state[1], anchor);
        if (!isfinite(anchor[0]))
            return;
        if (!isfinite(anchor[1]))
            return;
        {
            float anchor_y = *(const float *)((const char *)state + 0x81c);
            if (!isfinite(anchor_y) || !isfinite(anchor[1]))
                return;
            float delta = (anchor_y - anchor[1]) * g_ui_density;
            if (delta <= 0.0f)
                delta = 0.0f;
            *(float *)((char *)state + 0x81c) = delta;
            widget_set_position(state[1],
                                (x - g_ui_center_x) / g_ui_density - anchor[0],
                                (y - g_ui_center_y) / g_ui_density - anchor[1]);
            widget_set_position(state[0], 0, 0);
        }
    }
}

/* ===== rich panel buttons ===== */

extern uint64_t g_rich_panel_ctx;   /* 0x283340 — misc.c: rich panel context */

/*
 * rich_panel_button_build — build one button widget on the rich panel.
 * Gates: bundle for nexus_rich_asset(kind) loads and passes
 * movie_frame_count_check, bundle +0x38 == 0 (parent) and +0x40 == -1
 * (index), allocation of the 0x260-byte widget succeeds, widget vptr ==
 * game_base + 0x11c0a48, widget +0x38/+0x40 identity, sc_object_release,
 * rich_binding_bind(slot, bundle, 0), label set from the text object,
 * parked at (-10000, -10000), attached via engine 0x594130 (rich panel ctx
 * 0x283340) and widget_rich_panel_bound_check. Fills the 0x1b-qword slot:
 * [0] = widget, [1] = bundle. Returns 1 (final +0x48 flag read == 1), 0 on
 * any gate failure. @ 0017bf48
 */
int rich_panel_button_build(uint64_t *slot, uint32_t kind, const char *label)
{
    uint64_t bundle = engine_sc_bundle_load(nexus_rich_asset(kind));
    if (movie_frame_count_check(bundle, (int)kind, 0) == 0)
        return 0;

    int32_t index = 0;
    uint64_t parent = 1;
    if (!(0xfff < bundle + 0x38)
        || (bundle & 0xfffffffffffffff8ULL) == 0xffffffffffffffc0ULL
        || g_host_read(g_host_ctx, bundle + 0x38, &parent, 8) != 1
        || parent != 0
        || !(0xfff < bundle + 0x40)
        || (bundle & 0xfffffffffffffffcULL) == 0xffffffffffffffbcULL
        || g_host_read(g_host_ctx, bundle + 0x40, &index, 4) != 1
        || index != -1)
        return 0;

    engine_call_5d7c30(bundle, 0);
    uint64_t widget = engine_call_11a2840(0x260);
    slot[0] = widget;
    if (widget == 0)
        return 0;
    engine_call_887c14();

    uint64_t vptr = 0;
    bool read_ok = false;
    if ((widget + 8) >> 3 >= 0x201)
        read_ok = g_host_read(g_host_ctx, widget, &vptr, 8) == 1;
    bool plausible = 0xfff < vptr && read_ok && (vptr & 7) == 0;
    if ((plausible ? vptr : 0) != g_game_base + 0x11c0a48U)
        return 0;

    parent = 1;
    index = 0;
    if (!(0xfff < widget + 0x38)
        || (widget & 0xfffffffffffffff8ULL) == 0xffffffffffffffc0ULL
        || g_host_read(g_host_ctx, widget + 0x38, &parent, 8) != 1
        || parent != 0
        || !(0xfff < widget + 0x40)
        || (widget & 0xfffffffffffffffcULL) == 0xffffffffffffffbcULL
        || g_host_read(g_host_ctx, widget + 0x40, &index, 4) != 1
        || index != -1)
        return 0;

    if (sc_object_release(widget) == 0)
        return 0;

    /* zero the binding slot, then bind [0]=widget to the bundle */
    uint64_t bind_slot[0x1b] = {0};
    bind_slot[0] = slot[0];
    if (rich_binding_bind(bind_slot, bundle, 0) == 0)
        return 0;
    slot[1] = bundle;

    uint64_t text_obj = ui_string_object(label);
    widget_set_text(widget, text_obj);
    widget_set_position(widget, -10000.0f, -10000.0f);
    engine_call_594130(g_rich_panel_ctx, widget);
    if (widget_rich_panel_bound_check(widget) == 0)
        return 0;

    uint8_t flag = 0xff;
    if (widget + 0x49 <= 0x1000)
        return 0;
    if (g_host_read(g_host_ctx, widget + 0x48, &flag, 1) != 1)
        return 0;
    return flag == 1;
}
/* renderer p1 chunk 12 (renderer.c part 2, chunk 5): covers raw lines 3732-4178 */
/* ===== rich binding + widget builders ===== */

/* cross-file externs used by this chunk */
extern void engine_call_772a30(uint64_t a, uint64_t b, int c);
                                      /* misc.c @ 0016ce6c — engine base + 0x772a30 */
extern int  widget_pair_snapshot_read(uint64_t *node_out, int mode, void *out);
                                      /* misc.c @ 0017c704 — {node,parent} pair snapshot, 0x50 out */
extern void ui_text_style_format_d(void *buf, size_t cap_a, size_t cap_b,
                                   const char *fmt, ...);
                                      /* misc.c @ 00176a24 — checked formatter */
extern int  widget_popover_bound_check(uint64_t widget);
                                      /* misc.c @ 0017df44 — bound under 0x281b10 (<=16) */
extern int  widget_edit_controls_bound_check(uint64_t widget);
                                      /* misc.c @ 00179dfc — bound under 0x282158 (<=16) */
extern int  ui_char_classify(uint32_t code);
                                      /* widgets.c @ 0018d0fc — table 0x140830 classify */
extern int  settings_blob_push(void);
                                      /* misc.c @ 0018d334 — build + submit settings blob */
extern long spin_group_record_find(uint32_t feature);
                                      /* misc.c @ 0018d3b4 — spin channel group record */
extern int  settings_values_commit(void *values);
                                      /* misc.c @ 001846b8 — 29x16B records commit */
extern void menu_row_state_init(void *out, const void *entry);
                                      /* widgets.c @ 00184e28 — 0x18 row state */
extern int  plus_active_refresh_alt(void);
                                      /* misc.c @ 0018d500 — resolve plus_active, cache fn */
extern int  plus_active_query(uint32_t feature);
                                      /* external @ 0018d15c — not in dump (plus-active value) */
extern int  settings_preset_field_set(void *values, const char *key, int32_t value);
                                      /* external @ 0018d2a0 — not in dump (key/value setter) */
extern uint32_t settings_apply_now(void);
                                      /* external @ 0018d48c — not in dump (immediate apply) */
extern void theme_machine_pump(void);
                                      /* external @ 00185d10 — not in dump (theme machine tick) */

/* menu/settings state (misc.c-owned) */
extern uint32_t g_theme_latch;        /* 0x284688 — test-and-set gate */
extern int32_t  g_theme_menu_id;      /* 0x284694 */
extern uint32_t g_theme_report_pending; /* 0x28469f */
extern uint32_t g_theme_revision;     /* 0x2846a0 */
extern uint32_t g_theme_last_status;  /* 0x2846a8 — last set_value result */
extern uint32_t g_theme_last_feature; /* 0x2846a4 — last feature id */
extern uint32_t g_theme_row_kind;     /* 0x2846ac — last row kind */
extern uint32_t g_theme_values[0x76]; /* 0x2846b0 — 29x16B value cache (0x1d8) */
extern uint64_t g_popover_context;    /* 0x281b10 — popover ctx (misc p2) */
extern uint64_t g_edit_controls_ctx2; /* 0x282158 — edit-controls ctx (widgets) */
extern void    *g_plus_active_fn;     /* 0x28dd00 — cached plus_active resolver */
extern pthread_once_t g_plus_once;    /* 0x28dcfc — plus_active once gate */

/* menu entry table (0xa8 records, 0x48 stride) + spin-channel groups */
extern const uint8_t MENU_ENTRY_TABLE[];  /* 0x1a36f0 — 0xa8 records x 0x48 */
extern const void *SPIN_GROUP_TABLE[];    /* 0x1a36f8 — "nexus_sx_spin" groups, 0x18 stride */

/* outline color preset rgb table (0xc stride, r/g/b at +0/+4/+8) */
extern const uint32_t OUTLINE_PRESET_RGB[][3];  /* 0x14073c */

/* p1/p2-owned binding diag globals */
extern uint64_t g_rich_binding_diag[10];   /* 0x1e51e8..0x1e5238 — c07 */
extern char     g_rich_binding_line[0x1c0]; /* 0x1e5238 — c07 */
extern uint32_t g_rich_stop_log_count;      /* 0x1a7d20 — p1 */
extern uint32_t g_rich_bind_log_count;      /* 0x1e53f8 — cap 5 binding logs */

/*
 * rich_binding_bind — bind a row widget to its display source. Calls
 * engine_call_772a30 on the widget, stores the display at slot[1], takes a
 * {node,parent} pair snapshot (mode 0), copies the probe fields into the
 * 0x1e51e8 diag block, formats the 15-field 'ok=%d mode=%u reads=%x ...'
 * line and — when the snapshot failed or the binding-log cap (5) is not yet
 * reached — logs 'nexus_rich_binding' (also capped 64 by the stop counter).
 * On success also stores the clip80 field at slot[3]. Returns the snapshot
 * result (1 ok, 0 failed). @ 0017c42c
 */
int rich_binding_bind(uint64_t *slot, uint64_t display, uint32_t row_index)
{
    engine_call_772a30(slot[0], display, 0);
    slot[1] = display;

    uint64_t probe[13];
    int snap = widget_pair_snapshot_read(slot, 0, probe);
    g_rich_binding_diag[0] = probe[0];   /* 0x1e51e8 */
    g_rich_binding_diag[1] = probe[1];   /* 0x1e51f0 */
    g_rich_binding_diag[2] = probe[2];   /* 0x1e51f8 */
    g_rich_binding_diag[3] = probe[3];   /* 0x1e5200 */
    g_rich_binding_diag[4] = probe[4];   /* 0x1e5208 */
    g_rich_binding_diag[5] = probe[5];   /* 0x1e5210 */
    g_rich_binding_diag[6] = probe[6];   /* 0x1e5218 */
    g_rich_binding_diag[7] = probe[7];   /* 0x1e5220 */
    g_rich_binding_diag[8] = probe[8];   /* 0x1e5228 */
    g_rich_binding_diag[9] = probe[9];   /* 0x1e5230 */
    ui_text_style_format_d(g_rich_binding_line, 0x1c0, 0x1c0,
        "ok=%d mode=%u reads=%x button=%llx source=%llx clip80=%llx "
        "display88=%llx sourceb0=%llx mcparent=%llx mcindex=%d "
        "displayparent=%llx displayindex=%d named=%d matches=%u active=%u",
        snap, probe[10], probe[11], probe[0], probe[1], probe[2], probe[3],
        probe[4], probe[5], probe[6], probe[7], probe[8], probe[9],
        probe[12]);

    uint32_t count = g_rich_bind_log_count + 1;
    bool under_cap = g_rich_bind_log_count < 4;
    g_rich_bind_log_count = count;
    if (snap == 0 || under_cap) {
        uint32_t stops = g_rich_stop_log_count + 1;
        bool stop_cap = g_rich_stop_log_count < 0x40;
        g_rich_stop_log_count = stops;
        if (stop_cap && g_host_log != NULL)
            g_host_log(g_host_ctx, "nexus_rich_binding",
                       g_rich_binding_line, row_index);
    }
    if (snap != 0)
        slot[3] = probe[3];   /* clip80 field */
    return snap;
}

/*
 * chooser_button_build — build one font-chooser button on the popover
 * context (0x281b10). Same gate ladder as rich_panel_button_build (bundle
 * from nexus_rich_asset(kind), movie_frame_count_check, +0x38 == 0 /
 * +0x40 == -1, 0x260-byte alloc, vptr game_base + 0x11c0a48, identity
 * re-read, sc_object_release, rich_binding_bind(slot, bundle, 0)), then
 * label via ui_string_object, park at (-10000, -10000), attach through
 * engine 0x594130 on the popover ctx and require
 * widget_popover_bound_check + the +0x48 flag == 1. Fills slot[0]=widget,
 * slot[1]=bundle. Returns 1 ok, 0 any gate failed.
 * @ 0017d8c4
 */
int chooser_button_build(uint64_t *slot, uint32_t kind, const char *label)
{
    uint64_t bundle = engine_sc_bundle_load(nexus_rich_asset(kind));
    if (movie_frame_count_check(bundle, (int)kind, 0) == 0)
        return 0;

    int32_t index = 0;
    uint64_t parent = 1;
    if (!(0xfff < bundle + 0x38)
        || (bundle & 0xfffffffffffffff8ULL) == 0xffffffffffffffc0ULL
        || g_host_read(g_host_ctx, bundle + 0x38, &parent, 8) != 1
        || parent != 0
        || !(0xfff < bundle + 0x40)
        || (bundle & 0xfffffffffffffffcULL) == 0xffffffffffffffbcULL
        || g_host_read(g_host_ctx, bundle + 0x40, &index, 4) != 1
        || index != -1)
        return 0;

    engine_call_5d7c30(bundle, 0);
    uint64_t widget = engine_call_11a2840(0x260);
    slot[0] = widget;
    if (widget == 0)
        return 0;
    engine_call_887c14();

    uint64_t vptr = 0;
    bool read_ok = false;
    if ((widget + 8) >> 3 >= 0x201)
        read_ok = g_host_read(g_host_ctx, widget, &vptr, 8) == 1;
    bool plausible = 0xfff < vptr && read_ok && (vptr & 7) == 0;
    if ((plausible ? vptr : 0) != g_game_base + 0x11c0a48U)
        return 0;

    parent = 1;
    index = 0;
    if (!(0xfff < widget + 0x38)
        || (widget & 0xfffffffffffffff8ULL) == 0xffffffffffffffc0ULL
        || g_host_read(g_host_ctx, widget + 0x38, &parent, 8) != 1
        || parent != 0
        || !(0xfff < widget + 0x40)
        || (widget & 0xfffffffffffffffcULL) == 0xffffffffffffffbcULL
        || g_host_read(g_host_ctx, widget + 0x40, &index, 4) != 1
        || index != -1)
        return 0;

    if (sc_object_release(widget) == 0)
        return 0;

    uint64_t bind_slot[0x1b] = {0};
    bind_slot[0] = slot[0];
    if (rich_binding_bind(bind_slot, bundle, 0) == 0)
        return 0;
    slot[1] = bundle;

    uint64_t text_obj = ui_string_object(label);
    widget_set_text(widget, text_obj);
    widget_set_position(widget, -10000.0f, -10000.0f);
    engine_call_594130(g_popover_context, widget);
    if (widget_popover_bound_check(widget) == 0)
        return 0;

    uint8_t flag = 0xff;
    if (widget + 0x49 <= 0x1000)
        return 0;
    if (g_host_read(g_host_ctx, widget + 0x48, &flag, 1) != 1)
        return 0;
    return flag == 1;
}

/*
 * ui_edit_controls_button_build — build one button on the edit-controls
 * context (0x282158). Identical gate ladder to chooser_button_build but
 * attaches through engine 0x594130 on the edit-controls ctx and requires
 * widget_edit_controls_bound_check + the +0x48 flag == 1.
 * @ 0017f2f8
 */
int ui_edit_controls_button_build(uint64_t *slot, uint32_t kind,
                                  const char *label)
{
    uint64_t bundle = engine_sc_bundle_load(nexus_rich_asset(kind));
    if (movie_frame_count_check(bundle, (int)kind, 0) == 0)
        return 0;

    int32_t index = 0;
    uint64_t parent = 1;
    if (!(0xfff < bundle + 0x38)
        || (bundle & 0xfffffffffffffff8ULL) == 0xffffffffffffffc0ULL
        || g_host_read(g_host_ctx, bundle + 0x38, &parent, 8) != 1
        || parent != 0
        || !(0xfff < bundle + 0x40)
        || (bundle & 0xfffffffffffffffcULL) == 0xffffffffffffffbcULL
        || g_host_read(g_host_ctx, bundle + 0x40, &index, 4) != 1
        || index != -1)
        return 0;

    engine_call_5d7c30(bundle, 0);
    uint64_t widget = engine_call_11a2840(0x260);
    slot[0] = widget;
    if (widget == 0)
        return 0;
    engine_call_887c14();

    uint64_t vptr = 0;
    bool read_ok = false;
    if ((widget + 8) >> 3 >= 0x201)
        read_ok = g_host_read(g_host_ctx, widget, &vptr, 8) == 1;
    bool plausible = 0xfff < vptr && read_ok && (vptr & 7) == 0;
    if ((plausible ? vptr : 0) != g_game_base + 0x11c0a48U)
        return 0;

    parent = 1;
    index = 0;
    if (!(0xfff < widget + 0x38)
        || (widget & 0xfffffffffffffff8ULL) == 0xffffffffffffffc0ULL
        || g_host_read(g_host_ctx, widget + 0x38, &parent, 8) != 1
        || parent != 0
        || !(0xfff < widget + 0x40)
        || (widget & 0xfffffffffffffffcULL) == 0xffffffffffffffbcULL
        || g_host_read(g_host_ctx, widget + 0x40, &index, 4) != 1
        || index != -1)
        return 0;

    if (sc_object_release(widget) == 0)
        return 0;

    uint64_t bind_slot[0x1b] = {0};
    bind_slot[0] = slot[0];
    if (rich_binding_bind(bind_slot, bundle, 0) == 0)
        return 0;
    slot[1] = bundle;

    uint64_t text_obj = ui_string_object(label);
    widget_set_text(widget, text_obj);
    widget_set_position(widget, -10000.0f, -10000.0f);
    engine_call_594130(g_edit_controls_ctx2, widget);
    if (widget_edit_controls_bound_check(widget) == 0)
        return 0;

    uint8_t flag = 0xff;
    if (widget + 0x49 <= 0x1000)
        return 0;
    if (g_host_read(g_host_ctx, widget + 0x48, &flag, 1) != 1)
        return 0;
    return flag == 1;
}

/* ===== menu value setter ===== */

#include <pthread.h>

/*
 * nexus_menu_set_value — set one menu feature value from the script port.
 * Gates: feature <= 0xa7, theme latch free, caller tid == g_theme_menu_id.
 * Stores the feature id, classifies it (ui_char_classify) and requires the
 * plus-active gate (pthread_once + 0x28dd00 resolver) to pass, then pumps
 * the theme machine and looks up the 0x48-stride menu entry record. The
 * spin group must accept param_2 (min at +0x08, max at +0x0c of the
 * 0x18-stride group). Feature 0x70 < 7: copies the 0x1d8 value cache,
 * writes "nexus_sx_outline_color_preset" + r/g/b from the 0x14073c preset
 * table and commits. Other features: by kind byte at +0x44 (mask 0xd8,
 * kinds < 5 with the 0x2000000003c00001 feature mask) either the spin
 * channel vtable call at +0x30 with a 0x40-byte request (then stores
 * param_2 into the value cache at the group index) or feature 0x79 < 3
 * kind 2; every path ends with settings_blob_push. Stores the result code
 * in g_theme_last_status and bumps g_theme_revision. Returns the result
 * (0 ok, 3 latch held, 4 invalid feature/value, 1/2 channel codes).
 * @ 001849cc
 */
int nexus_menu_set_value(uint32_t feature, uint32_t value, int tid)
{
    if (0xa7 < feature)
        return 4;
    if (ui_latch_test_and_set(1, (volatile int *)&g_theme_latch) & 1)
        return 3;

    uint64_t result = 4;
    if (0 < tid && g_theme_menu_id == tid) {
        g_theme_last_feature = feature;
        if (ui_char_classify(feature) == 0
            && pthread_once(&g_plus_once, (void (*)(void))plus_active_refresh_alt) == 0
            && (g_plus_active_fn == NULL
                || ((int (*)(void))g_plus_active_fn)() != 1))
            goto ok_zero;

        theme_machine_pump();
        const uint8_t *entry = MENU_ENTRY_TABLE + (size_t)feature * 0x48;
        if (entry[0x47] != 0)
            goto ok_zero;      /* entry disabled */

        uint32_t group = plus_active_query(feature);   /* external @ 0018d15c */
        const void *spin = SPIN_GROUP_TABLE[group];
        int32_t min_v = *(const int32_t *)((const char *)spin + 8);
        int32_t max_v = *(const int32_t *)((const char *)spin + 0xc);
        if ((int32_t)group < 0 || (int32_t)value < min_v || max_v < (int32_t)value)
            goto invalid;

        if (feature == 0x70) {
            /* outline color preset: write preset + r/g/b then commit */
            if (value < 7) {
                uint32_t values[0x76];
                memcpy(values, g_theme_values, 0x1d8);
                if (settings_preset_field_set(values,
                                              "nexus_sx_outline_color_preset",
                                              (int32_t)value) != 0
                    && settings_preset_field_set(
                           values, "nexus_sx_outline_r",
                           (int32_t)OUTLINE_PRESET_RGB[value][0]) != 0
                    && settings_preset_field_set(
                           values, "nexus_sx_outline_g",
                           (int32_t)OUTLINE_PRESET_RGB[value][1]) != 0
                    && settings_preset_field_set(
                           values, "nexus_sx_outline_b",
                           (int32_t)OUTLINE_PRESET_RGB[value][2]) != 0) {
                    result = settings_values_commit(values);
                    if ((uint32_t)result - 3 > 0xfffffffdU) {
                        uint32_t pushed = settings_blob_push();
                        uint32_t code = (uint32_t)result;
                        if (pushed != 1)
                            code = pushed;
                        result = code;
                    }
                    goto store;
                }
            }
            goto invalid;
        }

        uint8_t kind = entry[0x44];
        uint64_t channel_result;
        if (kind < 8 && (1u << (kind & 0x1f) & 0xd8U) != 0) {
            if (kind < 5
                && (0x3d < feature - 0x2f
                    || (1ULL << ((feature - 0x2f) & 0x3f)
                        & 0x2000000003c00001ULL) == 0)) {
                /* run the spin-channel request through the group record */
                long group_rec = spin_group_record_find(feature);
                uint8_t row_state[0x18];
                menu_row_state_init(row_state, entry);
                g_theme_row_kind = *(uint32_t *)(void *)row_state;
                if (group_rec == 0 || row_state[4] == 0)
                    goto ok_zero;
                uint32_t req[0x10] = {0};
                ((uint32_t *)req)[0] = 0x40;          /* size */
                req[0x0c] = feature;
                req[0x0e] = kind;
                req[0x0f] = value;
                req[0x0d] = settings_apply_now();     /* external @ 0018d48c */
                uint64_t vt = *(uint64_t *)(void *)((char *)group_rec + 0x30);
                uint64_t ctx = *(uint64_t *)(void *)((char *)group_rec + 8);
                channel_result = ((uint64_t (*)(uint64_t, void *))vt)(ctx, req);
                if (channel_result - 1 < 2) {
                    g_theme_values[group] = value;
                    result = settings_blob_push();
                    if ((int)result != 1)
                        goto store;
                }
            } else {
                g_theme_values[group] = value;
                result = settings_blob_push();
                if ((int)result != 1)
                    goto store;
                channel_result = 1;
            }
            result = channel_result;
        } else {
            result = 4;
            if (feature == 0x79 && value < 3 && kind == 2) {
                /* HUD scale row: same spin-channel request path */
                long group_rec = spin_group_record_find(feature);
                uint8_t row_state[0x18];
                menu_row_state_init(row_state, entry);
                g_theme_row_kind = *(uint32_t *)(void *)row_state;
                if (group_rec == 0 || row_state[4] == 0)
                    goto ok_zero;
                uint32_t req[0x10] = {0};
                ((uint32_t *)req)[0] = 0x40;
                req[0x0c] = feature;
                req[0x0e] = kind;
                req[0x0f] = value;
                req[0x0d] = settings_apply_now();
                uint64_t vt = *(uint64_t *)(void *)((char *)group_rec + 0x30);
                uint64_t ctx = *(uint64_t *)(void *)((char *)group_rec + 8);
                channel_result = ((uint64_t (*)(uint64_t, void *))vt)(ctx, req);
                if (channel_result - 1 < 2) {
                    g_theme_values[group] = value;
                    result = settings_blob_push();
                    if ((int)result != 1)
                        goto store;
                }
                result = channel_result;
            }
        }
    store:
        g_theme_last_status = (uint32_t)result;
        g_theme_revision++;
    }
    g_theme_latch = 0;
    return (int)result;

ok_zero:
    result = 0;
    goto store;
invalid:
    result = 4;
    goto store;
}
/* renderer p1 chunk 13 (renderer.c part 2, chunk 6 — file tail): covers raw lines 4180-4393 */
/* ===== deferred theme apply ===== */

/* misc.c-owned theme machine state */
extern uint32_t g_theme_machine_state;  /* 0x287ba4 — 0 idle, 2 apply pending */
extern uint32_t g_theme_apply_state;    /* 0x287b98 */
extern uint32_t g_theme_apply_kind;     /* 0x287ba8 */
extern uint32_t g_theme_apply_a;        /* 0x287bb8 */
extern uint32_t g_theme_apply_b;        /* 0x287bbc */
extern uint32_t g_theme_apply_gen;      /* 0x287bac */
extern uint32_t g_theme_apply_latch;    /* 0x287bb4 */
extern uint32_t g_theme_apply_src_a;    /* 0x287c44 */
extern uint32_t g_theme_apply_src_b;    /* 0x287c48 — slot pair source */
extern uint32_t g_theme_apply_dst;      /* 0x287c50 — slot pair destination */
extern uint32_t g_theme_slot_a;         /* 0x285414 — slot state (kind != 1) */
extern uint32_t g_theme_slot_b;         /* 0x287c40 — slot state (kind 1) */
extern uint32_t g_theme_apply_slot_sel; /* 0x285410 — slot selected flag */
extern char     g_theme_status_line[0x60];  /* 0x287c6c */
extern int      ui_status_line_format(char *buf, size_t cap_a, size_t cap_b,
                                      const char *fmt, const char *text);
                                      /* misc.c @ 00183ac8 */

/*
 * theme_apply_defer — defer a theme apply by one frame. Gates: machine
 * idle (0x287ba4 == 0), apply state live (0x287b98 != 0) and a slot
 * selected (0x285410 != 0). Sets the machine state to 2, snapshots the
 * selected slot word (0x287c40 for kind 1, 0x285414 otherwise) into
 * 0x287c44 and copies the 0x287c48 slot word to 0x287c50, stores the kind/args, formats the status line
 * "APPLYING ON NEXT FRAME" (fmt 0x1343dc), clears the report-pending flag
 * and bumps the apply generation + theme revision. No return value.
 * @ 00186c90
 */
void theme_apply_defer(int kind, uint32_t arg_a, uint32_t arg_b)
{
    if (g_theme_machine_state != 0 || g_theme_apply_state == 0
        || g_theme_apply_slot_sel == 0)
        return;

    g_theme_machine_state = 2;
    uint32_t slot = kind != 1 ? g_theme_slot_a /* 0x285414 */
                              : g_theme_slot_b /* 0x287c40 */;
    g_theme_apply_src_a = slot;                 /* 0x287c44 */
    g_theme_apply_dst = g_theme_apply_src_b;     /* 0x287c48 -> 0x287c50 */
    g_theme_apply_kind = kind;                  /* 0x287ba8 */
    g_theme_apply_a = arg_a;                    /* 0x287bb8 */
    g_theme_apply_b = arg_b;                    /* 0x287bbc */
    ui_status_line_format(g_theme_status_line, 0x60, 0x60,
                          (const char *)0x1343dc, "APPLYING ON NEXT FRAME");
    g_theme_apply_latch = 0;                    /* 0x287bb4 */
    g_theme_report_pending = 0;                 /* 0x28469f */
    g_theme_apply_gen++;                        /* 0x287bac */
    g_theme_revision++;                         /* 0x2846a0 */
}

/* ===== movie bind verify ===== */

/* ops table for movie_bind_verify (raw param_2 — function-pointer bundle) */
typedef struct {
    void    *ctx;               /* +0x00 */
    void    *read_mem;          /* +0x08 — (ctx, addr, out, len) -> int */
    void    *field_10;          /* +0x10 */
    void    *field_18;          /* +0x18 */
    void    *field_20;          /* +0x20 */
    void    *alloc;             /* +0x28 — (ctx, key) -> handle */
    void    *attach;            /* +0x30 — (ctx, parent, child) */
    void    *detach;            /* +0x38 — (ctx, obj, 0) */
    void    *pair;              /* +0x40 — (ctx, a, b) */
    void    *link;              /* +0x48 — (ctx, parent, child) */
    void    *field_50;          /* +0x50 */
    void    *field_58;          /* +0x58 */
    void    *log_event;         /* +0x60 — (ctx, event, key, state) */
} movie_ops_t;

/*
 * movie_bind_verify — verify and bind a movie clip. Resolves the clip via
 * ops->alloc(ops->ctx, key), stores it at state[0x10] with parent 0 and
 * frame count -1. Gates: clip >= 0x1000 and 8-aligned; vptr (read at +0)
 * must equal state[0] + 0x11ad208; the BE frame count at clip +0xbe (2
 * bytes) must read and be >= 1. On pass logs 'movie_observed', attaches
 * the clip to the parent (ops->attach), verifies the bound clip at parent
 * +0x80 is a plausible pointer, detaches (ops->detach), pairs and links
 * (ops->pair/ops->link), sets state[0x10..0x12] and returns 1. On any
 * failure sets state[+8] = 3, logs 'stopped' with the reason
 * ('movie_result_null_or_unaligned', 'movie_vptr_mismatch',
 * 'movie_be_read_failed', 'movie_frame_guard', 'bound_clip_missing') and
 * returns 0. @ 00190ad0
 */
int movie_bind_verify(long *state, movie_ops_t *ops, uint64_t parent,
                      uint64_t key, uint64_t arg_a, uint64_t arg_b)
{
    uint64_t clip = ((uint64_t (*)(void *, uint64_t))ops->alloc)(ops->ctx, key);
    uint16_t frame_count = 0;

    state[0x10] = clip;
    state[0x11] = 0;
    *(int32_t *)(state + 0x12) = -1;

    const char *reason;
    if (clip < 0x1000 || (clip & 7) != 0) {
        reason = "movie_result_null_or_unaligned";
    } else {
        uint64_t vptr = 0;
        bool read_ok = false;
        if ((clip + 8) >> 3 >= 0x201)
            read_ok = ((int (*)(void *, uint64_t, void *, uint32_t))
                       ops->read_mem)(ops->ctx, clip, &vptr, 8) != 0;
        bool plausible = 0xfff < vptr && read_ok && (vptr & 7) == 0;
        state[0x11] = plausible ? vptr : 0;
        if ((uint64_t)state[0x11] == (uint64_t)state[0] + 0x11ad208ULL) {
            if (clip + 0xbe < 0x1000
                || (clip & 0xfffffffffffffffeULL) == 0xffffffffffffff40ULL
                || ((int (*)(void *, uint64_t, void *, uint32_t))
                    ops->read_mem)(ops->ctx, clip + 0xbe, &frame_count, 2) == 0) {
                reason = "movie_be_read_failed";
            } else {
                *(int32_t *)(state + 0x12) = (int32_t)frame_count;
                ((void (*)(void *, const char *, uint64_t, long *))
                 ops->log_event)(ops->ctx, "movie_observed", key, state);
                if (frame_count < 1) {
                    reason = "movie_frame_guard";
                } else {
                    ((void (*)(void *, uint64_t, uint64_t))ops->attach)(
                        ops->ctx, parent, clip);
                    uint64_t bound = 0;
                    if (((parent + 0x88U) >> 3 > 0x200
                         && ((int (*)(void *, uint64_t, void *, uint32_t))
                             ops->read_mem)(ops->ctx, parent + 0x80,
                                            &bound, 8) != 0)
                        && (0xfff < bound && (bound & 7) == 0)) {
                        ((void (*)(void *, uint64_t, int))ops->detach)(
                            ops->ctx, clip, 0);
                        ((void (*)(void *, uint64_t, uint64_t))ops->pair)(
                            ops->ctx, arg_a, arg_b);
                        ((void (*)(void *, uint64_t, uint64_t))ops->link)(
                            ops->ctx, parent, arg_a);
                        return 1;
                    }
                    reason = "bound_clip_missing";
                }
            }
        } else {
            reason = "movie_vptr_mismatch";
        }
    }
    *(int32_t *)(state + 8) = 3;
    ((void (*)(void *, const char *, const char *, long *))
     ops->log_event)(ops->ctx, "stopped", reason, state);
    return 0;
}

/* ===== dlsym export thunks (file tail) ===== */

/*
 * The raw tail is a block of PLT-style thunks: each named def tail-calls
 * through a GOT slot. Reconstructed as forwarding wrappers <fn>__export
 * that call the real (same-file) implementations.
 */

void nexus_script_port_ui_reload_request__export(void);
                                       /* — dlsym export thunk @ 00194150 */
void nexus_script_port_ui_reload_request__export(void)
{
    nexus_script_port_ui_reload_request();
}

void nexus_rich_main_frame__export(uint64_t a, uint32_t b, int c);
                                       /* — dlsym export thunk @ 00194640 */
void nexus_rich_main_frame__export(uint64_t a, uint32_t b, int c)
{
    nexus_rich_main_frame(a, b, c);
}

void nexus_script_port_fps_open__export(void);
                                       /* — dlsym export thunk @ 00194690 */
void nexus_script_port_fps_open__export(void)
{
    nexus_script_port_fps_open();
}

void nexus_ui_graphics_resources__export(uint64_t a, uint64_t b,
                                         uint64_t c, uint32_t d);
                                       /* — dlsym export thunk @ 00194780 */
void nexus_ui_graphics_resources__export(uint64_t a, uint64_t b,
                                         uint64_t c, uint32_t d)
{
    nexus_ui_graphics_resources(a, b, c, d);
}

void *nexus_rich_renderer_ops_v1__export(void);
                                       /* — dlsym export thunk @ 00194790 */
void *nexus_rich_renderer_ops_v1__export(void)
{
    return nexus_rich_renderer_ops_v1();
}

unsigned long nexus_rich_validate_native__export(void);
                                       /* — dlsym export thunk @ 001947a0 */
unsigned long nexus_rich_validate_native__export(void)
{
    return nexus_rich_validate_native();
}

int nexus_rich_launcher_geometry__export(float a, float b, float c,
                                         float d, float e, float *out);
                                       /* — dlsym export thunk @ 001947b0 */
int nexus_rich_launcher_geometry__export(float a, float b, float c,
                                         float d, float e, float *out)
{
    return nexus_rich_launcher_geometry(a, b, c, d, e, out);
}

uint32_t nexus_rich_main_row__export(void);
                                       /* — dlsym export thunk @ 001947f0 */
uint32_t nexus_rich_main_row__export(void)
{
    return nexus_rich_main_row();
}

const char *nexus_rich_asset__export(uint32_t index);
                                       /* — dlsym export thunk @ 00194870 */
const char *nexus_rich_asset__export(uint32_t index)
{
    return nexus_rich_asset(index);
}

uint32_t nexus_rich_color__export(int32_t a, int32_t b, int32_t c,
                                  int32_t d, uint64_t e);
                                       /* — dlsym export thunk @ 00194880 */
uint32_t nexus_rich_color__export(int32_t a, int32_t b, int32_t c,
                                  int32_t d, uint64_t e)
{
    return nexus_rich_color(a, b, c, d, e);
}

int nexus_rich_plan_stage__export(const void *a, const void *b, float c,
                                  float d, float e, uint64_t f, uint64_t g,
                                  float *stage);
                                       /* — dlsym export thunk @ 001948c0 */
int nexus_rich_plan_stage__export(const void *a, const void *b, float c,
                                  float d, float e, uint64_t f, uint64_t g,
                                  float *stage)
{
    return nexus_rich_plan_stage(a, b, c, d, e, f, g, stage);
}
