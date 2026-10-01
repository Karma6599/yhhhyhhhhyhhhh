/* widgets chain 3 chunk 1: covers raw lines 1-325 (defs only; JNI_OnLoad @326 deferred to chunk 2) */

/*
 * widgets — UI subsystem
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
 *   - espEnabled "ESP" [free]
 *   - characterOutlineEnabled "Character outline" [free]
 *   - attackRangeIndicator "Attack range" [Nexus+ PAID]
 *   - hitboxRenderer "Hitboxes" [Nexus+ PAID]
 *   - enemyTracer "Enemy tracer" [Nexus+ PAID]
 *   - trophiesAboveHead "Trophies" [Nexus+ PAID]
 *   - pinEnabled "Auto pin" [Nexus+ PAID]
 *   - sprayEnabled "Auto spray" [Nexus+ PAID]
 *   - ... +205 more (see docs/feature_list.json)
 *
 * Reconstructed from the Ghidra 11.3.2 arm64 pseudocode into readable C
 * (semantic reconstruction; every magic number, offset and string literal
 * kept). Chunk 1 contents — the battle-end overlay gate family:
 *   - battle_end_widget_visibility()   — fill the 0x18-byte battle-end
 *     request record with the ShowFastPlayAgainButton /
 *     BattleEndInstantExit visibility verdict @ 0014ec44
 *   - battle_end_feature_arm()         — arm the battle-end action
 *     (fast-replay pair or captured end-screen node) @ 0014f5b8
 *   - nexus_native_trophy_text_guard() — integrity guard for the
 *     trophies-above-head native text hook @ 0014fcec
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
#include <pthread.h>

/* ===== shared menu context (mirror of menu_engine.c's menu_context_t) ===== */

/* 0x38-byte context snapshot refreshed by menu_context_init (~1 Hz while
 * the menu runs on the Mainloop thread). */
typedef struct {
    uint64_t refresh_ms;        /* +0x00 — timestamp of this refresh (ms) */
    uint64_t generation;        /* +0x08 — stage generation counter snapshot */
    uint64_t stage_view;        /* +0x10 — stashed stage view */
    uint64_t widget_obj;        /* +0x18 — cached remote widget object */
    uint32_t autofarm_enabled;  /* +0x20 — evasion 'autofarmEnabled' verdict */
    uint32_t evasion_armed;     /* +0x24 — protected-plus verified + record kind ok */
    uint32_t autofarm_gated;    /* +0x28 — autofarm_verified && setting 0x25 == 1 */
    uint32_t stage_byte;        /* +0x2c — byte at stage root + 0x19c */
    uint32_t limit_0x62;        /* +0x30 — setting 0x62 (default 3500 / 0xdac) */
    uint32_t limit_99;          /* +0x34 — setting 99 (default 2000) */
} menu_context_t;

/* ===== battle-end request / fast-replay records (mirror misc.c typedefs) ===== */

/* 0x18-byte NRI1-style request record, seeded from the 0x10f960 literal. */
typedef struct {
    uint32_t magic;        /* +0x00 "NRI1" (from the 0x10f960 literal) */
    uint32_t verdict;      /* +0x04 resolved plus verdict */
    uint32_t plus_ok;      /* +0x08 */
    uint32_t verdict2;     /* +0x0c verdict copy */
    uint32_t installed;    /* +0x10 */
    uint32_t installed_hi; /* +0x14 */
    uint8_t  is_plus_req;  /* +0x15 1 = plus request */
    uint8_t  _pad16;       /* +0x16 */
    uint16_t _pad17;       /* +0x17 */
} plus_request_record_t;

/* 0x20-byte fast-replay service record (filled by
 * script_port_fast_replay_snapshot). */
typedef struct {
    uint64_t header;     /* +0x00 literal header (0x10f720) */
    uint32_t ok_a;       /* +0x08 must be nonzero to accept */
    uint32_t ok_b;       /* +0x0c */
    uint64_t field_10;   /* +0x10 must be nonzero to accept */
    uint64_t field_18;   /* +0x18 must be nonzero to accept */
} service_record_t;

/* ===== cross-file functions ===== */

extern void menu_context_init(menu_context_t *ctx, uint64_t now_ms);
                                      /* menu_engine.c @ 0014ef90 (stale extern
                                       * alias in misc.c: menu_engine_tick) */
extern uint64_t script_port_fast_replay_snapshot(uint64_t out_addr);
                                      /* misc.c @ 00192138 — fast-replay service
                                       * record snapshot, 0 when unbound */
extern int proc_mem_read(void *ctx, uint64_t addr, void *out, uint32_t len);
                                      /* misc.c @ 0014e7c4 — remote read; arg 1
                                       * is a dead x0 slot, body ignores it */
extern uint64_t button_lineage_check(uint64_t node, uint64_t screen);
                                      /* misc.c @ 00154eb4 — returns the
                                       * validated screen node, 0 on failure */
extern int code_region_hash_verify_report(uint64_t offset, size_t len,
                                          const char *expect_hex);
                                      /* misc.c @ 0014fe14 — 1 when the region
                                       * digest matches, 0 otherwise */

/* ===== rodata ===== */

extern const uint64_t SERVICE_RECORD_HEADER;   /* 0x10f720 — literal fast-replay record header */
extern const uint64_t PLUS_REQ_LITERAL;        /* 0x10f960 — {magic lo, fields hi} */
extern const uint64_t PLUS_REQ_LITERAL_2;      /* 0x10f968 — second qword of the request literal */

/* ===== globals owned elsewhere (extern) ===== */

extern uint8_t  g_postbattle_installed;        /* 0x22d8d8 — misc.c: 1 = postbattle islands installed */
extern uint64_t g_proc_mem_bias;               /* 0x22d8e0 — misc.c: libg.so load base (remote-read bias) */
extern uint64_t g_pair_release_epoch;          /* 0x22dad8 — misc.c: cleared when the pair is fully released */
extern uint32_t g_hook_inflight;               /* 0x22db38 — misc.c: capture in-flight word */
extern uint64_t g_ui_server_trampoline_addr;   /* 0x22db80 — misc.c: island address */
extern int32_t  g_ui_server_trampoline_id;     /* 0x22db88 — misc.c: island id word */
extern uint64_t g_ui_server_trampoline_len;    /* 0x22db90 — misc.c: template length */
extern char     g_ui_server_trampoline_copy[0x208]; /* 0x22db98 — misc.c: expected image */
extern int32_t  g_ui_server_trampoline_done;   /* 0x22dd98 — misc.c: 1 = recorded */
extern uint32_t g_launcher_menu_id;            /* 0x22f038 — renderer.c: owning menu id / Mainloop tid */

/* ===== capture / battle-end state (widgets.c-owned; read here, written here
 * and by the postbattle pump later in this file) ===== */

uint8_t  g_track_enabled;      /* 0x22d8e8 — 1 = capture tracking on (misc.c reads) */
uint64_t g_plus_obs_90;        /* 0x22d8f8 — observed record word: context generation (misc.c reads) */
uint64_t g_plus_obs_a0;        /* 0x22d900 — observed stage view (identity partner of g_plus_obs_90) */
uint64_t g_plus_obs_armed;     /* 0x22da74 — observation arm pair; hi dword (0x22da78) arms the gate */
uint64_t g_plus_obs_word68;    /* 0x22da7c — gate word: low dword == 1 arms the plus-observation gate */
uint64_t g_battle_end_node;    /* 0x22dae0 — armed end-screen node (fast-replay path: 0) */
uint64_t g_battle_end_stage;   /* 0x22dae8 — stage view the arm was latched for */
uint64_t g_battle_end_word;    /* 0x22daf0 — captured +0x248 word (fast-replay path: 0) */
uint64_t g_battle_end_stamp;   /* 0x22daf8 — context generation at arm time */
uint64_t g_battle_end_ms;      /* 0x22db00 — arm time in ms */
uint64_t g_battle_end_pair_a;  /* 0x22db08 — fast-replay pair slot a (record +0x10) */
uint64_t g_battle_end_pair_b;  /* 0x22db10 — fast-replay pair slot b (record +0x18) */
uint64_t g_hook_module_handle; /* 0x22db18 — captured overlay node (misc.c reads) */
uint64_t g_hook_last_a;        /* 0x22db20 — last captured node id (stage view) */
uint64_t g_hook_last_b;        /* 0x22db28 — last captured +0x248 value */
uint64_t g_hook_last_c;        /* 0x22db30 — last captured identity (generation) */
uint64_t g_guard_last_a;       /* 0x22f1b0 — island identity a (stage view) */
uint64_t g_guard_last_b;       /* 0x22f1b8 — island identity b (generation) */
uint64_t g_guard_last_c;       /* 0x22f1c0 — island identity c (in flight) */

/*
 * is_plausible_ptr_e — remote-pointer sanity gate for the battle-end node
 * checks: reject anything below 0x1000 or not 8-byte aligned (the dump
 * spells this "(v & 7) != 0 || v < 0x1000").
 */
static bool is_plausible_ptr_e(uint64_t v)
{
    return v >= 0x1000 && (v & 7) == 0;
}

/* ===== battle-end overlay gate family ===== */

/*
 * battle_end_widget_visibility — visibility verdict for the two battle-end
 * overlay features "ShowFastPlayAgainButton" and "BattleEndInstantExit";
 * any other name returns 0 with the record untouched. On a match capture
 * tracking is switched on, the shared menu context refreshed and the
 * 0x18-byte request record seeded from the 0x10f960 literal with the
 * verdict: installed (+0x10) = !visible, installed_hi (+0x14) = visible.
 * The buttons are visible only when the end-screen island identity is NOT
 * current for this context and either the fast-replay service record
 * accepts, or — Mainloop thread, stage byte clear, pair released — the
 * captured hook node's lineage and its +0x248 word still match the capture.
 * Returns 1 when the record was filled. @ 0014ec44
 */
int battle_end_widget_visibility(const char *feature, plus_request_record_t *out)
{
    menu_context_t ctx;
    struct timespec now;
    service_record_t replay;
    char thread_name[0x10];
    uint64_t now_ms;
    uint64_t word248;
    bool replay_ok;
    bool plus_obs_current;
    bool guard_current;
    uint32_t visible;
    int fast_play;
    int read_ok;

    fast_play = strcmp(feature, "ShowFastPlayAgainButton") == 0;
    if (!fast_play && strcmp(feature, "BattleEndInstantExit") != 0)
        return 0;

    g_track_enabled = 1;

    if (clock_gettime(CLOCK_MONOTONIC, &now) == 0)
        now_ms = (uint64_t)now.tv_sec * 1000 + (uint64_t)now.tv_nsec / 1000000;
    else
        now_ms = 0;
    menu_context_init(&ctx, now_ms);

    /* fast-play only: is the fast-replay service offering a rematch? */
    replay_ok = false;
    if (fast_play && ctx.autofarm_enabled != 0 && ctx.widget_obj != 0
        && !(g_guard_last_c != 0 && g_guard_last_a == ctx.stage_view
             && g_guard_last_b == ctx.generation)) {
        memset(&replay, 0, sizeof replay);
        replay.header = SERVICE_RECORD_HEADER;
        if (script_port_fast_replay_snapshot((uint64_t)(uintptr_t)&replay) != 0
            && replay.field_10 != 0 && replay.field_18 != 0)
            replay_ok = replay.ok_a != 0;
    }

    /* is the cached plus-service observation current for this context? */
    plus_obs_current = false;
    if ((g_plus_obs_armed >> 32) != 0 && (int32_t)g_plus_obs_word68 == 1) {
        if (g_plus_obs_a0 == ctx.stage_view)
            plus_obs_current = g_plus_obs_90 == ctx.generation;
    }

    visible = 0;
    if (g_postbattle_installed == 1
        && g_launcher_menu_id >= 1
        && gettid() == (int)g_launcher_menu_id
        && pthread_getname_np(pthread_self(), thread_name,
                              sizeof thread_name) == 0) {
        if (memcmp(thread_name, "Mainloop", 8) != 0 /* 0x706f6f6c6e69614d */
            || thread_name[8] != '\0')
            plus_obs_current = true;   /* off the Mainloop thread: skip the deep verify */

        if (!plus_obs_current && ctx.stage_byte == 0
            && g_pair_release_epoch == 0) {
            /* island identity current for this context => already handled */
            guard_current = g_guard_last_c != 0
                            && g_guard_last_a == ctx.stage_view
                            && g_guard_last_b == ctx.generation;
            if (guard_current || replay_ok) {
                visible = !guard_current;
            } else if (g_hook_inflight == 0
                       && ctx.autofarm_enabled != 0
                       && g_hook_module_handle != 0
                       && ctx.stage_view == g_hook_last_a
                       && ctx.generation == g_hook_last_c
                       && button_lineage_check(g_hook_module_handle,
                                               ctx.stage_view) != 0) {
                /* re-read the +0x248 word from the captured end-screen node */
                word248 = 0;
                read_ok = proc_mem_read(NULL, g_hook_module_handle + 0x248,
                                        &word248, 8);
                if (!is_plausible_ptr_e(word248) || read_ok == 0)
                    word248 = 0;
                visible = word248 == g_hook_last_b;
            }
        }
    }

    /* seed the record from the 0x10f960 request literal and store the verdict */
    memcpy(out, (const uint64_t[2]){ PLUS_REQ_LITERAL,   /* 0x10f960 */
                                      PLUS_REQ_LITERAL_2 /* 0x10f968 */ },
           2 * sizeof(uint64_t));
    out->installed = visible ^ 1;
    out->installed_hi = visible;
    out->is_plus_req = 0;
    out->_pad16 = 0;
    out->_pad17 = 0;
    return 1;
}

/*
 * battle_end_feature_arm — arm the battle-end action for
 * "ShowFastPlayAgainButton" (mode 1) or "BattleEndInstantExit" (mode 2);
 * other names are ignored. The feature must be visible per
 * battle_end_widget_visibility (record +0x14) and the refreshed context on
 * the verified path (autofarm verdict set, stage byte clear, pair
 * released). Mode 1 arms through a fresh fast-replay service record (pair
 * slots a/b, node cleared); when that is not offered — and always for mode
 * 2 — the captured hook node path is used (lineage + the +0x248 word must
 * still match; pair slots cleared, node stored). The arm is latched into
 * the 0x22dae0 battle-end block and g_pair_release_epoch takes the mode.
 * Returns 2 when armed, 0 otherwise. @ 0014f5b8
 */
int battle_end_feature_arm(const char *feature)
{
    menu_context_t ctx;
    struct timespec now;
    plus_request_record_t rec;
    service_record_t replay;
    uint64_t now_ms;
    uint64_t word248;
    int mode;
    int read_ok;

    if (strcmp(feature, "ShowFastPlayAgainButton") == 0)
        mode = 1;
    else if (strcmp(feature, "BattleEndInstantExit") == 0)
        mode = 2;
    else
        return 0;

    if (battle_end_widget_visibility(feature, &rec) == 0
        || rec.installed_hi == 0)
        return 0;

    if (clock_gettime(CLOCK_MONOTONIC, &now) == 0)
        now_ms = (uint64_t)now.tv_sec * 1000 + (uint64_t)now.tv_nsec / 1000000;
    else
        now_ms = 0;
    menu_context_init(&ctx, now_ms);

    if (ctx.autofarm_enabled == 0 || ctx.stage_byte != 0
        || g_pair_release_epoch != 0)
        return 0;

    word248 = 0;
    if (mode == 1) {
        /* fast-play: accept a fresh fast-replay service record */
        memset(&replay, 0, sizeof replay);
        replay.header = SERVICE_RECORD_HEADER;
        if (ctx.widget_obj != 0
            && !(g_guard_last_c != 0 && g_guard_last_a == ctx.stage_view
                 && g_guard_last_b == ctx.generation)
            && script_port_fast_replay_snapshot((uint64_t)(uintptr_t)&replay) != 0
            && replay.field_10 != 0 && replay.field_18 != 0
            && replay.ok_a != 0) {
            g_battle_end_node = 0;
            g_battle_end_pair_a = replay.field_10;
            g_battle_end_pair_b = replay.field_18;
            goto commit;
        }
    }

    /* fallback (and BattleEndInstantExit): leave through the captured node */
    if (g_hook_module_handle == 0)
        return 0;
    if (ctx.stage_view != g_hook_last_a || ctx.generation != g_hook_last_c)
        return 0;
    if (button_lineage_check(g_hook_module_handle, ctx.stage_view) == 0)
        return 0;
    word248 = 0;
    read_ok = proc_mem_read(NULL, g_hook_module_handle + 0x248, &word248, 8);
    if (!is_plausible_ptr_e(word248) || read_ok == 0)
        word248 = 0;
    if (word248 != g_hook_last_b)
        return 0;
    g_battle_end_pair_a = 0;
    g_battle_end_pair_b = 0;
    g_battle_end_node = g_hook_module_handle;

commit:
    g_pair_release_epoch = mode;
    g_battle_end_stage = ctx.stage_view;
    g_battle_end_word = word248;
    g_battle_end_stamp = ctx.generation;
    g_battle_end_ms = now_ms;
    return 2;
}

/*
 * nexus_native_trophy_text_guard — integrity guard behind the
 * trophies-above-head text hook. The hook must pass the exact libg load
 * base; the postbattle islands must be installed and the ui-server
 * trampoline recorded (0x22db80 block, length capped at 0x200). The island
 * id dword at libg+0x5921a0 must match the recorded id and the live island
 * image must equal the saved copy; only then is the guard's own code region
 * (libg+0x5921a4, 0xac bytes) verified against the pinned SHA-256. errno
 * is preserved across the reads. Returns the verification verdict (the
 * raw leaves it in x0). @ 0014fcec
 */
int nexus_native_trophy_text_guard(uint64_t base)
{
    char image[0x200];
    int32_t island_id = 0;
    int saved_errno = errno;
    int verdict = 0;

    if (base != g_proc_mem_bias
        || g_postbattle_installed == 0
        || g_ui_server_trampoline_done == 0
        || g_ui_server_trampoline_addr == 0
        || g_ui_server_trampoline_len == 0
        || g_ui_server_trampoline_len > 0x200
        || proc_mem_read(NULL, g_proc_mem_bias + 0x5921a0,
                         &island_id, 4) == 0)
        goto out;

    if (island_id == g_ui_server_trampoline_id) {
        if (proc_mem_read(NULL, g_ui_server_trampoline_addr, image,
                          (uint32_t)g_ui_server_trampoline_len) == 0)
            goto out;
        if (memcmp(image, g_ui_server_trampoline_copy,
                   (size_t)g_ui_server_trampoline_len) == 0) {
            verdict = code_region_hash_verify_report(0x5921a4, 0xac,
                "53397ef450e30da2b8d4a3670db34ea376776454fa87062980a88ff69c879891");
            goto out;
        }
    }
    verdict = 0;
out:
    errno = saved_errno;
    return verdict;
}
/* widgets chain 3 chunk 2: covers raw lines 326-1066 (JNI_OnLoad @ 00150c18, whole — it straddled the planned 326-700 cut and is finished here) */

#include <link.h>
#include <sys/stat.h>

/* ===== JNI vtable veneers (bionic arm64 layout; slot = byte offset into
 * the functions table, index = slot / 8 — see misc.c chunk 1) ===== */

static long jni_call0_e(void *obj, uint32_t slot)
{
    long *table = *(long **)obj;
    long (*fn)(void *) = (long (*)(void *))table[slot / 8];
    return fn(obj);
}

static long jni_call1_e(void *obj, uint32_t slot, long a)
{
    long *table = *(long **)obj;
    long (*fn)(void *, long) = (long (*)(void *, long))table[slot / 8];
    return fn(obj, a);
}

static long jni_call2_e(void *obj, uint32_t slot, long a, long b)
{
    long *table = *(long **)obj;
    long (*fn)(void *, long, long) = (long (*)(void *, long, long))table[slot / 8];
    return fn(obj, a, b);
}

static long jni_call3_e(void *obj, uint32_t slot, long a, long b, long c)
{
    long *table = *(long **)obj;
    long (*fn)(void *, long, long, long) =
        (long (*)(void *, long, long, long))table[slot / 8];
    return fn(obj, a, b, c);
}

/* ===== JNI_OnLoad state (captured service/class/method slots) ===== */

void *g_loader_service;             /* 0x22e010 — JavaVM* handed to JNI_OnLoad
                                     * (menu_engine.c: loader service locator) */
uint64_t g_trampoline_len;          /* 0x22f020 — page size; mprotect length
                                     * for the islands (misc.c reads) */
void *g_probe_jvm;                  /* 0x22d860 — JavaVM* for the online probe (misc.c reads) */
void *g_probe_class;                /* 0x22d868 — nexus/loader/h global ref (misc.c reads) */
void *g_probe_method;               /* 0x22d870 — snapshot jmethodID (misc.c reads) */
uint8_t g_notify_armed;             /* 0x22d8b0 — 1 = overlay bridge registered
                                     * (misc.c: g_channel_installed; menu_engine.c:
                                     * g_notify_armed) */
void *g_battle_menu_host_class;     /* 0x22d8b8 — NexusBattleMenuHost global ref
                                     * (misc.c: g_channel_class; menu_engine.c:
                                     * g_battle_notify_method) */
void *g_battle_menu_toggle_mid;     /* 0x22d8c0 — togglePanelFromNative jmethodID
                                     * (misc.c: g_channel_method) */
int32_t g_battle_notify_ready;      /* 0x22d8c8 — 1 = battle notify pair usable */
void *g_notify_service;             /* 0x22d8d0 — bridge JavaVM* (misc.c:
                                     * g_channel_jvm; menu_engine.c: g_notify_service) */
void *g_overlay_host_class;         /* 0x22f030 — NexusOverlayHost global ref
                                     * (misc.c: g_channel_arg_ref; menu_engine.c:
                                     * g_notify_method) */
void *g_overlay_open_from_native_mid;  /* 0x22f4f0 — openFromNative jmethodID
                                        * (menu_engine.c: g_notify_token) */
void *g_battle_state_notify_mid;       /* 0x22f4f8 — onNativeBattleState jmethodID
                                        * (menu_engine.c: g_battle_notify_token) */
uint32_t g_native_guards_armed;     /* 0x22f4c0 — UiAssetProof.verify verdict
                                     * (renderer.c: game-state latch) */
void *g_activation_jvm;             /* 0x22f4c8 — ActivationController JavaVM*
                                     * (misc.c/activation_plus.c: g_clip_service;
                                     * menu_engine.c: g_jvm_service) */
void *g_activation_snapshot_mid;    /* 0x22f4d0 — nativeUiSnapshot jmethodID
                                     * (activation_plus.c: g_clip_arg) */
void *g_activation_dispatch_mid;    /* 0x22f4d8 — dispatchNativeUi jmethodID */
void *g_activation_dismiss_mid;     /* 0x22f4e0 — dismissNativeUi jmethodID */
void *g_activation_class;           /* 0x22f4e8 — ActivationController global ref */
long g_gameapp_getinstance_mid;     /* 0x25c8d0 — GameApp.getInstance jmethodID
                                     * (misc.c: g_social_req_d) */
void *g_gameapp_class;              /* 0x25c8d8 — GameApp global ref */
long g_dialog_mgr_field;            /* 0x25c8e0 — NativeDialogManager static field id
                                     * (menu_engine.c: g_plus_jstr) */
long g_dialog_getdialog_mid;        /* 0x25c8e8 — DialogFragment.getDialog */
long g_dialog_findviewbyid_mid;     /* 0x25c8f0 — Dialog.findViewById */
long g_dialog_gettext_mid;          /* 0x25c8f8 — TextView.getText */
long g_dialog_tostring_mid;         /* 0x25c900 — Object.toString */
long g_dialog_performclick_mid;     /* 0x25c908 — View.performClick */
long g_dialog_isshown_mid;          /* 0x25c910 — View.isShown */
long g_dialog_isenabled_mid;        /* 0x25c918 — View.isEnabled */
void *g_dialog_mgr_class;           /* 0x25c920 — NativeDialogManager global ref */

/* ===== cross-file functions ===== */

extern void ui_stage_log(const char *stage, const char *reason);   /* below @ 001524e8 */
extern int  phdr_libg_capture(const void *info, size_t size, void *data);
                                      /* misc.c @ 001529bc — libg.so phdr scan */
extern int  delivery_process_memory_find(const void *info, size_t size,
                                         void *data);
                                      /* misc.c @ 00155cf0 — libNexusDelivery channel */
extern int  files_dir_open(char *path);   /* misc.c @ 00155e94 */
extern void menu_performance_load(int dirfd);   /* menu_engine.c @ 001560d8 */
extern uint64_t nexus_menu_init(void);           /* menu_engine.c @ 00183a08 */
extern uint64_t nexus_menu_register_storage(int *block);  /* menu_engine.c @ 0018416c */
extern uint64_t nexus_menu_register_backend(uint32_t kind, int *callbacks);
                                      /* menu_engine.c @ 00183e8c */
extern int64_t bind_state_init(uint64_t *st, const int64_t *req,
                               const int64_t *cred);   /* misc.c @ 00169420 */
extern int  postbattle_install(void);            /* misc.c @ 00153060 */
extern int  social_islands_install(void);        /* misc.c @ 00153670 */
extern void jni_native_loaded_report(void *jvm); /* misc.c @ 00153a10 */
extern void jni_diagnostics_failure(void *jvm, const char *tag,
                                     const char *message);  /* misc.c @ 001525f4 */
extern int  menu_mainloop_alive_check(void);     /* menu_engine.c @ 00152b7c */
extern int  remote_modules_hash_verify(uint64_t base,
                                       int (*read)(void *, uint64_t, void *, uint32_t),
                                       void *unused, const uint8_t *table,
                                       uint64_t count);  /* misc.c @ 00191058 */
extern int  engine_vtables_verify(uint64_t base,
                                  int (*read)(void *, uint64_t, void *, uint32_t),
                                  void *unused);         /* misc.c @ 00191170 */
extern void sha256_init(void *state);                    /* misc.c @ 00191314 (112-byte ctx) */
extern void sha256_update(void *state, const void *data, size_t len);
                                                          /* misc.c @ 00191338 */
extern void sha256_final(void *state, void *out32);      /* misc.c @ 00191678 */
extern int  __open_2(const char *path, int flags);       /* fortified open */
extern int  __openat_2(int fd, const char *path, int flags);
extern int  __android_log_print(int prio, const char *tag, const char *fmt, ...);
extern int *__errno(void);                                /* bionic errno slot */
pid_t gettid(void);

/* dl_iterate_phdr adapters (the misc.c callbacks are typed against their own
 * dl_phdr_info mirrors) */
static int phdr_libg_scan(struct dl_phdr_info *info, size_t size, void *data)
{
    return phdr_libg_capture(info, size, data);
}

static int delivery_scan(struct dl_phdr_info *info, size_t size, void *data)
{
    return delivery_process_memory_find(info, size, data);
}

/* ===== globals owned elsewhere ===== */

extern uint32_t g_host_channel_fd;      /* 0x1a7c38 — misc.c: remote-memory channel fd */
extern const char *g_ui_initialize_reason;  /* 0x1a7c40 — misc.c: bootstrap status line */
extern int g_host_session;              /* 0x1a7c50 — misc.c: files-dir fd */
extern uint32_t g_libg_capture_count;   /* 0x22e018 — misc.c: libg.so phdr callbacks seen */
extern char g_libg_path[0x1000];        /* 0x22e01c — misc.c: captured libg.so path */
extern uint64_t g_proc_mem_bias;        /* 0x22d8e0 — misc.c: libg.so load base */
extern uint64_t g_bind_state;           /* 0x22f040 — misc.c: bind state block */
extern uint64_t g_menu_store_domain;    /* 0x22d838 — menu_engine.c: store domain token
                                         * (the ClientInputController class ref) */
extern uint64_t g_menu_store_register;  /* 0x22d840 — menu_engine.c: register op token */
extern uint64_t g_menu_store_read;      /* 0x22d848 — menu_engine.c: read op token */
extern uint64_t g_menu_store_remove;    /* 0x22d850 — menu_engine.c: remove op token */
extern void *g_menu_service;            /* 0x22f028 — menu_engine.c: service locator
                                         * (the input-controller JavaVM*) */
extern uint32_t g_ui_release_armed;     /* 0x22db78 — menu_engine.c: frame-v1 release arm
                                         * (misc.c: g_plus_service_installed) */

/* ===== rodata: bootstrap stage labels ===== */

extern const char g_stage_game_mapping[];            /* 0x13b998 */
extern const char g_stage_process_memory[];          /* 0x13938d */
extern const char g_stage_page_size[];               /* 0x13939c */
extern const char g_stage_game_file_identity[];      /* 0x1382f9 */
extern const char g_stage_original_code_guards[];    /* 0x1360e9 */
extern const char g_stage_menu_environment[];        /* 0x13361b */
extern const char g_stage_menu_initialize[];         /* 0x131e93 */
extern const char g_stage_storage_context[];         /* 0x139da4 */
extern const char g_stage_storage_private_files[];   /* 0x132f3e */
extern const char g_stage_storage_menu_directory[];  /* 0x131ea3 */
extern const char g_stage_storage_registration[];    /* 0x13771a */
extern const char g_stage_backend_binding_context[]; /* 0x13aef9 */
extern const char g_stage_ui_assets[];               /* 0x1366bf */
extern const char g_stage_social_registration[];     /* 0x139877 */
extern const char g_stage_menu_facade_class[];       /* 0x133b58 */
extern const char g_stage_menu_method_signature[];   /* 0x1341a5 */
extern const char g_stage_menu_native_registration[];/* 0x137c08 */

/* ===== rodata: JNI names / signatures ===== */

extern const char g_sig_snapshot_result[];   /* 0x135b2c — "snapshot"/"getNativeMenuLoadState" sig */
extern const char g_sig_dispatch_native[];   /* 0x133632 — "dispatchNativeUi" sig */
extern const char g_sig_noarg_z[];           /* 0x13ba46 — "dismissNativeUi"/"startNativeUiHooks" sig */
extern const char g_name_input_second[];     /* 0x1348a4 — input 2nd method name ("(I)[B") */
extern const char g_sig_input_dismiss[];     /* 0x13725d — "dismiss" sig */
extern const char g_sig_verify_z[];          /* 0x1361b3 — openFromNative/performClick/isShown/isEnabled sig */
extern const char g_sig_toggle_panel[];      /* 0x133bbb — "togglePanelFromNative" sig */
extern const char g_name_dialog_field[];     /* 0x137c33 — NativeDialogManager field name */
extern const uint8_t g_code_guard_table[];   /* 0x10fbf8 — 19-entry guard table */
extern const int64_t g_delivery_seed;        /* 0x10f790 — {cookie lo, fd hi} channel seed */

/* ===== rodata: storage / bind / backend contract records ===== */

extern const int64_t g_storage_rec_header;   /* 0x198bf0 — {version 1, size 0x20} */
extern const int64_t g_storage_rec_word1;    /* 0x198bf8 */
extern const int64_t g_storage_rec_fn2;      /* 0x198c00 */
extern const int64_t g_storage_rec_fn3;      /* 0x198c08 */

extern const int64_t g_bind_req_header;      /* 0x198c40 — {version 1, size 0x38} */
extern const int64_t g_bind_req_word1;       /* 0x198c48 */
extern const void *const g_bind_req_slot2;   /* 0x198c50 */
extern const void *const g_bind_req_slot3;   /* 0x198c58 */
extern const void *const g_bind_req_slot4;   /* 0x198c60 */
extern const void *const g_bind_req_slot5;   /* 0x198c68 */
extern const void *const g_bind_req_slot6;   /* 0x198c70 */

extern const int64_t g_bind_cred_header;     /* 0x198c10 — {version 1, size 0x30} */
extern const int64_t g_bind_cred_word1;      /* 0x198c18 */
extern const char *const g_bind_cred_sha_hex;  /* 0x198c20 — sealed SHA hex */
extern const int64_t g_bind_cred_word3;      /* 0x10fff8 */
extern const uint32_t g_bind_cred_word4_lo;  /* 0x110000 */
extern const uint32_t g_bind_cred_word4_hi;  /* 0x110004 */

extern const int64_t g_menu_backend_header;  /* 0x19adc0 — {version 1, size 0x40} */
extern const int64_t g_menu_backend_word1;   /* 0x19adc8 */
extern const int64_t g_menu_backend_word2;   /* 0x19add0 */
extern const int64_t g_menu_backend_word3;   /* 0x10fa10 */
extern const int64_t g_menu_backend_word4;   /* 0x10fa18 */
extern const int64_t g_menu_backend_word5;   /* 0x19ade8 */
extern const void *const g_menu_backend_fn6; /* 0x19adf0 */
extern const void *const g_menu_backend_fn7; /* 0x19adf8 */

/* ===== rodata: AdvancedEvasionController native method table ===== */

typedef struct {
    const char *name;   /* +0x00 */
    const char *sig;    /* +0x08 */
    void       *fn;     /* +0x10 */
} jni_native_method_e_t;

extern const char *const g_evasion_hook_name;   /* 0x198ba8 — "startNativeUiHooks" */
extern const char *const g_evasion_hook_sig;    /* 0x198bb0 */
extern const void *const g_evasion_hook_fn;     /* 0x198bb8 */
extern const char *const g_evasion_state_name;  /* 0x198bc0 — "getNativeMenuLoadState" */
extern const char *const g_evasion_state_sig;   /* 0x198bc8 */
extern const void *const g_evasion_state_fn;    /* 0x198bd0 */
extern const char *const g_evasion_diag_name;   /* 0x198bd8 — "getNativeMenuDiagnostics" */
extern const char *const g_evasion_diag_sig;    /* 0x198be0 — "()Ljava/lang/String;" */
extern const void *const g_evasion_diag_fn;     /* 0x198be8 */

/* nexus/overlay/NexusOverlayBridge native table (11 entries, "nativeSnapshot" first) */
extern const jni_native_method_e_t g_overlay_bridge_methods[11];  /* 0x19acb8 */

/*
 * JNI_OnLoad — the library bootstrap. Only the lab build may initialize:
 * /proc/self/cmdline must start with "bsd.suitcase.nexusv2". Then the libg.so
 * module is captured (phdr scan), the delivery channel resolved, the game
 * image verified (regular file, size 0x1340c90, unchanged across the read,
 * pinned SHA-256, 19-entry code guards, vtables, Mainloop alive) before the
 * menu environment is created: GetEnv, nexus_menu_init, the files-dir storage
 * backend ("nexus-menu" dir, performance.bin load, storage + bind-state +
 * backend registration), then the ui-assets binding — NexusOnlineProbe,
 * ClientInputController, UiAssetProof.verify, ActivationController,
 * NexusOverlayBridge natives + overlay hosts, the dialog-manager method ids
 * and the AdvancedEvasionController hooks — and finally the postbattle and
 * social island installs. Returns JNI_VERSION_1_6 (0x10006) on success and
 * on every "disabled" gate; -1 only when the native link failed after the
 * menu environment was up. @ 00150c18
 */
int32_t JNI_OnLoad(void *vm)
{
    char cmdline[0x60];
    uint8_t chunk_buf[0x4000];
    struct stat st_open, st_recheck, st_dir;
    uint8_t sha_ctx[112];        /* misc.c sha256_state_t */
    uint64_t digest[4];
    struct {
        uint32_t cookie;         /* +0x00 callback cookie */
        int32_t  fd;             /* +0x04 channel fd */
        uint32_t passes;         /* +0x08 phdr passes seen */
    } delivery;
    jni_native_method_e_t bridge_methods[11];
    jni_native_method_e_t evasion_hooks[3];
    int64_t storage_block[4];
    int64_t bind_req[7];
    int64_t bind_cred[6];
    int64_t backend_block[12];   /* 0x60 bytes */
    void *env = NULL;
    void *vm_out;
    long loader, ctx_mid, ctx_obj, ctx_class, files_mid, files_obj;
    long files_class, path_mid, path_str;
    const char *path_utf;
    long probe_cls, snapshot_mid, probe_ref;
    long input_cls, request_mid, map_mid, dismiss_mid, input_ref;
    long loader2, ctx2_mid, ctx2, proof_cls, verify_mid;
    long activation_cls;
    long bridge_cls, host_cls, battle_cls, open_mid, state_mid, toggle_mid;
    long dialog_mgr, dialog_frag, dialog_cls, text_view, object_cls, view_cls;
    long gameapp;
    long evasion_cls, hook_mid, load_mid, diag_mid;
    long page_size;
    ssize_t n, last_read;
    uint64_t total;
    uint64_t w0, w8, w13;
    int files_fd, fd, rc, unchanged;
    char exc, verify_ok;
    int bound, method_ok, armed, stored;
    const char *stage, *reason;

    g_loader_service = vm;
    ui_stage_log("initialize_enter", "a10_lab_only");

    /* --- package identity: /proc/self/cmdline must be the lab build --- */
    memset(cmdline, 0, sizeof cmdline);
    fd = __open_2("/proc/self/cmdline", 0x80000 /* O_RDONLY|O_CLOEXEC */);
    n = -1;
    if (fd >= 0) {
        n = read(fd, cmdline, 0x5f);
        close(fd);
    }
    memcpy(&w0, cmdline, 8);
    memcpy(&w8, cmdline + 8, 8);
    memcpy(&w13, cmdline + 13, 8);
    if (n < 0x15
        || w0 != 0x746975732e647362ULL   /* "bsd.suit" */
        || w8 != 0x78656e2e65736163ULL   /* "case.nex" */
        || w13 != 0x3276737578656eULL) { /* "nexusv2\0" */
        ui_stage_log("disabled", "package_identity");
        jni_diagnostics_failure(vm, "package_identity",
                                "required_synchronous_callback_absent");
        return 0x10006;
    }

    /* --- game module capture + delivery channel --- */
    dl_iterate_phdr(phdr_libg_scan, NULL);

    /* the {cookie, fd} pair is seeded from the 0x10f790 rodata literal */
    memcpy(&delivery, &g_delivery_seed, 8);
    delivery.passes = 0;
    dl_iterate_phdr(delivery_scan, &delivery);
    g_host_channel_fd = (uint32_t)delivery.fd;
    if (delivery.passes != 1)
        g_host_channel_fd = 0xffffffffu;

    /* --- environment gates --- */
    page_size = sysconf(0x27 /* _SC_PAGESIZE */);
    g_ui_initialize_reason = g_stage_game_mapping;
    if (!(g_libg_capture_count == 1 && g_proc_mem_bias != 0))
        goto disabled_tail;
    g_ui_initialize_reason = g_stage_process_memory;
    if ((int32_t)g_host_channel_fd < 0)
        goto disabled_tail;
    g_ui_initialize_reason = g_stage_page_size;
    if (!(page_size == 0x4000 || page_size == 0x1000))
        goto disabled_tail;

    /* --- game file identity: hash the captured libg.so image --- */
    g_ui_initialize_reason = g_stage_game_file_identity;
    fd = __open_2(g_libg_path, 0x88000 /* O_RDONLY|O_CLOEXEC */);
    if (fd < 0) {
        close(fd);
        goto disabled_tail;
    }
    if (fstat(fd, &st_open) != 0
        || (st_open.st_mode & 0xf000) != 0x8000
        || st_open.st_size != 0x1340c90) {
        close(fd);
        goto disabled_tail;
    }
    sha256_init(sha_ctx);
    total = 0;
    last_read = read(fd, chunk_buf, 0x4000);
    while (last_read > 0) {
        total += (uint64_t)last_read;
        sha256_update(sha_ctx, chunk_buf, (size_t)last_read);
        last_read = read(fd, chunk_buf, 0x4000);
    }
    unchanged = 0;
    if (fstat(fd, &st_recheck) == 0
        && st_open.st_dev == st_recheck.st_dev
        && st_open.st_ino == st_recheck.st_ino
        && st_recheck.st_size == 0x1340c90
        && st_open.st_mtim.tv_sec == st_recheck.st_mtim.tv_sec
        && st_open.st_mtim.tv_nsec == st_recheck.st_mtim.tv_nsec)
        unchanged = 1;
    close(fd);
    sha256_final(sha_ctx, digest);
    if (!(last_read == 0 && unchanged == 1 && total == 0x1340c90
          && digest[0] == 0x2afb85406beb0aa1ULL
          && digest[1] == 0x0896cd30a169d915ULL
          && digest[2] == (uint64_t)-0x51bf65d94a67e6dd  /* 0xae429a26b5981923 */
          && digest[3] == (uint64_t)-0x1c12db6926d6ee2c)) /* 0xe3ed2496d92911d4 */
        goto disabled_tail;

    /* --- original code guards --- */
    g_ui_initialize_reason = g_stage_original_code_guards;
    if (remote_modules_hash_verify(g_proc_mem_bias, proc_mem_read, NULL,
                                   g_code_guard_table, 0x13) == 0
        || engine_vtables_verify(g_proc_mem_bias, proc_mem_read, NULL) == 0
        || menu_mainloop_alive_check() == 0)
        goto link_failed;

    /* --- menu environment --- */
    g_ui_initialize_reason = g_stage_menu_environment;
    g_trampoline_len = (uint64_t)page_size;
    env = NULL;
    if (jni_call2_e(vm, 0x30 /* GetEnv */, (long)(void *)&env,
                    0x10006 /* JNI_VERSION_1_6 */) != 0
        || env == NULL)
        goto link_failed;

    g_ui_initialize_reason = g_stage_menu_initialize;
    if (nexus_menu_init() != 1)
        goto link_failed;

    /* --- storage context: NexusLoader.applicationContext().getFilesDir() --- */
    g_ui_initialize_reason = g_stage_storage_context;
    loader = jni_call1_e(env, 0x30 /* FindClass */,
                         (long)"nexus/loader/NexusLoader");
    ctx_mid = 0;
    ctx_obj = 0;
    if (loader != 0)
        ctx_mid = jni_call3_e(env, 0x388 /* GetStaticMethodID */, loader,
                              (long)"applicationContext",
                              (long)"()Landroid/content/Context;");
    if (ctx_mid != 0)
        ctx_obj = jni_call2_e(env, 0x390 /* CallStaticObjectMethod */, loader,
                              ctx_mid);
    if (ctx_obj == 0 || jni_call0_e(env, 0x720 /* ExceptionCheck */) != 0)
        goto link_failed;

    ctx_class = jni_call1_e(env, 0xf8 /* GetObjectClass */, ctx_obj);
    files_mid = jni_call3_e(env, 0x108 /* GetMethodID */, ctx_class,
                            (long)"getFilesDir", (long)"()Ljava/io/File;");
    files_obj = 0;
    if (files_mid != 0)
        files_obj = jni_call2_e(env, 0x110 /* CallObjectMethod */, ctx_obj,
                                files_mid);
    if (files_obj == 0 || jni_call0_e(env, 0x720) != 0)
        goto link_failed;

    files_class = jni_call1_e(env, 0xf8 /* GetObjectClass */, files_obj);
    path_mid = jni_call3_e(env, 0x108 /* GetMethodID */, files_class,
                           (long)"getCanonicalPath",
                           (long)"()Ljava/lang/String;");
    path_str = 0;
    if (path_mid != 0)
        path_str = jni_call2_e(env, 0x110 /* CallObjectMethod */, files_obj,
                               path_mid);
    if (path_str == 0 || jni_call0_e(env, 0x720) != 0)
        goto link_failed;
    path_utf = (const char *)(intptr_t)jni_call2_e(env,
                        0x548 /* GetStringUTFChars */, path_str, 0);
    if (path_utf == NULL)
        goto link_failed;

    /* --- storage: private files dir, sealed "nexus-menu" directory --- */
    g_ui_initialize_reason = g_stage_storage_private_files;
    files_fd = files_dir_open((char *)path_utf);
    jni_call2_e(env, 0x550 /* ReleaseStringUTFChars */, path_str,
                (long)(intptr_t)path_utf);
    jni_call1_e(env, 0xb8 /* DeleteLocalRef */, path_str);
    jni_call1_e(env, 0xb8, files_class);
    jni_call1_e(env, 0xb8, files_obj);
    jni_call1_e(env, 0xb8, ctx_class);
    jni_call1_e(env, 0xb8, ctx_obj);
    jni_call1_e(env, 0xb8, loader);
    if (files_fd < 0)
        goto link_failed;

    g_ui_initialize_reason = g_stage_storage_menu_directory;
    rc = mkdirat(files_fd, "nexus-menu", 0x1c0 /* 0700 */);
    if (rc != 0 && *(__errno()) != 0x11 /* EEXIST */) {
        close(files_fd);
        goto link_failed;
    }
    g_host_session = __openat_2(files_fd, "nexus-menu", 0x8c000);
    close(files_fd);
    if (g_host_session < 0 || fstat(g_host_session, &st_dir) != 0)
        goto link_failed;
    if (st_dir.st_uid != getuid() || (st_dir.st_mode & 0x3f) != 0)
        goto link_failed;

    /* --- storage registration + bind state + ui backend --- */
    g_ui_initialize_reason = g_stage_storage_registration;
    menu_performance_load(g_host_session);
    storage_block[0] = g_storage_rec_header;
    storage_block[1] = g_storage_rec_word1;
    storage_block[2] = (int64_t)(intptr_t)g_storage_rec_fn2;
    storage_block[3] = (int64_t)(intptr_t)g_storage_rec_fn3;
    if (nexus_menu_register_storage((int *)storage_block) != 1)
        goto link_failed;

    g_ui_initialize_reason = g_stage_backend_binding_context;
    bind_req[0] = g_bind_req_header;
    bind_req[1] = g_bind_req_word1;
    bind_req[2] = (int64_t)(intptr_t)g_bind_req_slot2;
    bind_req[3] = (int64_t)(intptr_t)g_bind_req_slot3;
    bind_req[4] = (int64_t)(intptr_t)g_bind_req_slot4;
    bind_req[5] = (int64_t)(intptr_t)g_bind_req_slot5;
    bind_req[6] = (int64_t)(intptr_t)g_bind_req_slot6;
    bind_cred[0] = g_bind_cred_header;
    bind_cred[1] = g_bind_cred_word1;
    bind_cred[2] = (int64_t)(intptr_t)g_bind_cred_sha_hex;
    bind_cred[3] = g_bind_cred_word3;
    bind_cred[4] = (int64_t)(((uint64_t)g_bind_cred_word4_hi << 32)
                             | g_bind_cred_word4_lo);
    bind_cred[5] = (int64_t)0xfc1f05fff0ULL;
    if (bind_state_init(&g_bind_state, bind_req, bind_cred) != 1)
        goto link_failed;

    /* --- ui assets: online probe (nexus/loader/h snapshot) --- */
    g_ui_initialize_reason = g_stage_ui_assets;
    if (g_probe_class == NULL) {
        vm_out = NULL;
        if (jni_call2_e(env, 0x6d8 /* GetJavaVM */,
                        (long)(void *)&vm_out, 0) == 0 && vm_out != NULL) {
            probe_cls = jni_call1_e(env, 0x30, (long)"nexus/loader/h");
            bound = 0;
            method_ok = 0;
            probe_ref = 0;
            snapshot_mid = 0;
            if (probe_cls == 0 || jni_call0_e(env, 0x720) != 0) {
                jni_call0_e(env, 0x88 /* ExceptionClear */);
                __android_log_print(4, "NexusOnlineProbe", "class_missing");
            } else {
                snapshot_mid = jni_call3_e(env, 0x388, probe_cls,
                                           (long)"snapshot",
                                           (long)(intptr_t)g_sig_snapshot_result);
                if (jni_call0_e(env, 0x720) == 0) {
                    if (snapshot_mid != 0) {
                        probe_ref = jni_call1_e(env, 0xa8 /* NewGlobalRef */,
                                                probe_cls);
                        method_ok = 1;
                    }
                } else {
                    jni_call0_e(env, 0x88);
                    snapshot_mid = 0;
                }
                jni_call1_e(env, 0xb8 /* DeleteLocalRef */, probe_cls);
                if (jni_call0_e(env, 0x720) == 0) {
                    if (probe_ref != 0) {
                        bound = 1;
                        g_probe_jvm = vm_out;
                        g_probe_class = (void *)(intptr_t)probe_ref;
                        g_probe_method = (void *)(intptr_t)snapshot_mid;
                    }
                } else {
                    jni_call0_e(env, 0x88);
                }
                __android_log_print(4, "NexusOnlineProbe",
                                    "bound=%d method=%d", bound, method_ok);
            }
        }
    }

    /* --- ui assets: ClientInputController (menu store tokens) --- */
    if (g_menu_store_domain == 0) {
        vm_out = NULL;
        if (jni_call2_e(env, 0x6d8 /* GetJavaVM */,
                        (long)(void *)&vm_out, 0) == 0 && vm_out != NULL) {
            input_cls = jni_call1_e(env, 0x30,
                                    (long)"nexus/bsd/ClientInputController");
            if (input_cls != 0 && jni_call0_e(env, 0x720) == 0) {
                request_mid = jni_call3_e(env, 0x388, input_cls,
                                          (long)"requestUtf8",
                                          (long)"(I[B[BII)Z");
                if (jni_call0_e(env, 0x720) != 0) {
                    jni_call0_e(env, 0x88);
                    request_mid = 0;
                }
                map_mid = jni_call3_e(env, 0x388, input_cls,
                                      (long)(intptr_t)g_name_input_second,
                                      (long)"(I)[B");
                if (jni_call0_e(env, 0x720) != 0) {
                    jni_call0_e(env, 0x88);
                    map_mid = 0;
                }
                dismiss_mid = jni_call3_e(env, 0x388, input_cls,
                                          (long)"dismiss",
                                          (long)(intptr_t)g_sig_input_dismiss);
                input_ref = 0;
                if (jni_call0_e(env, 0x720) == 0) {
                    if (request_mid != 0 && map_mid != 0 && dismiss_mid != 0)
                        input_ref = jni_call1_e(env, 0xa8, input_cls);
                } else {
                    jni_call0_e(env, 0x88);
                    dismiss_mid = 0;
                }
                jni_call1_e(env, 0xb8, input_cls);
                if (jni_call0_e(env, 0x720) == 0) {
                    if (input_ref != 0) {
                        g_menu_service = vm_out;
                        g_menu_store_register = (uint64_t)request_mid;
                        g_menu_store_read = (uint64_t)map_mid;
                        g_menu_store_domain = (uint64_t)input_ref;
                        g_menu_store_remove = (uint64_t)dismiss_mid;
                    }
                    goto asset_proof;
                }
            }
            jni_call0_e(env, 0x88 /* ExceptionClear */);
        }
    }

    /* --- ui assets: UiAssetProof.verify + activation + overlay bridge --- */
asset_proof:
    loader2 = jni_call1_e(env, 0x30, (long)"nexus/loader/NexusLoader");
    ctx2_mid = 0;
    ctx2 = 0;
    proof_cls = 0;
    if (loader2 != 0)
        ctx2_mid = jni_call3_e(env, 0x388, loader2,
                               (long)"applicationContext",
                               (long)"()Landroid/content/Context;");
    if (ctx2_mid != 0)
        ctx2 = jni_call2_e(env, 0x390, loader2, ctx2_mid);
    if (ctx2 != 0 && jni_call0_e(env, 0x720) == 0)
        proof_cls = jni_call1_e(env, 0x30, (long)"nexus/ui/UiAssetProof");
    if (proof_cls != 0) {
        verify_mid = jni_call3_e(env, 0x388, proof_cls, (long)"verify",
                                 (long)"(Landroid/content/Context;)Z");
        if (verify_mid == 0)
            verify_ok = 0;
        else
            verify_ok = (char)jni_call3_e(env, 0x3a8 /* CallStaticBooleanMethod */,
                                          proof_cls, verify_mid, ctx2);
        exc = (char)jni_call0_e(env, 0x720);
        jni_call1_e(env, 0xb8, proof_cls);
        jni_call1_e(env, 0xb8, ctx2);
        jni_call1_e(env, 0xb8, loader2);
        g_native_guards_armed = (uint32_t)(exc == 0 && verify_ok == 1);
        if (exc == 0 && verify_ok == 1) {
            /* activation controller service slots */
            if (jni_call2_e(env, 0x6d8 /* GetJavaVM */,
                            (long)(void *)&g_activation_jvm, 0) == 0) {
                activation_cls = jni_call1_e(env, 0x30,
                        (long)"nexus/brawl/activation/ActivationController");
                if (activation_cls == 0) {
                    jni_call0_e(env, 0x88);
                } else {
                    g_activation_snapshot_mid =
                        (void *)(intptr_t)jni_call3_e(env, 0x388, activation_cls,
                                (long)"nativeUiSnapshot",
                                (long)"()[Ljava/lang/String;");
                    if (jni_call0_e(env, 0x720) != 0) {
                        jni_call0_e(env, 0x88);
                        g_activation_snapshot_mid = NULL;
                    }
                    g_activation_dispatch_mid =
                        (void *)(intptr_t)jni_call3_e(env, 0x388, activation_cls,
                                (long)"dispatchNativeUi",
                                (long)(intptr_t)g_sig_dispatch_native);
                    if (jni_call0_e(env, 0x720) != 0) {
                        jni_call0_e(env, 0x88);
                        g_activation_dispatch_mid = NULL;
                    }
                    g_activation_dismiss_mid =
                        (void *)(intptr_t)jni_call3_e(env, 0x388, activation_cls,
                                (long)"dismissNativeUi",
                                (long)(intptr_t)g_sig_noarg_z);
                    if (jni_call0_e(env, 0x720) != 0) {
                        jni_call0_e(env, 0x88);
                        g_activation_dismiss_mid = NULL;
                    }
                    if (g_activation_snapshot_mid != NULL
                        && g_activation_dispatch_mid != NULL)
                        g_activation_class = (void *)(intptr_t)
                            jni_call1_e(env, 0xa8, activation_cls);
                    jni_call1_e(env, 0xb8, activation_cls);
                }
            }

            /* overlay bridge natives + hosts */
            if ((g_notify_armed & 1) == 0) {
                armed = 0;
                stored = 0;
                bridge_cls = 0;
                host_cls = 0;
                battle_cls = 0;
                if (jni_call2_e(env, 0x6d8 /* GetJavaVM */,
                                (long)(void *)&g_notify_service, 0) == 0) {
                    bridge_cls = jni_call1_e(env, 0x30,
                            (long)"nexus/overlay/NexusOverlayBridge");
                    if (bridge_cls != 0
                        && jni_call0_e(env, 0x720) == 0) {
                        memcpy(bridge_methods, g_overlay_bridge_methods,
                               sizeof bridge_methods);
                        if (jni_call3_e(env, 0x6b8 /* RegisterNatives */,
                                        bridge_cls, (long)(void *)bridge_methods,
                                        0xb) == 0
                            && jni_call0_e(env, 0x720) == 0) {
                            host_cls = jni_call1_e(env, 0x30,
                                    (long)"nexus/overlay/NexusOverlayHost");
                            open_mid = 0;
                            if (host_cls != 0
                                && jni_call0_e(env, 0x720) == 0) {
                                open_mid = jni_call3_e(env, 0x388, host_cls,
                                        (long)"openFromNative",
                                        (long)(intptr_t)g_sig_verify_z);
                            }
                            if (open_mid != 0
                                && jni_call0_e(env, 0x720) == 0) {
                                battle_cls = jni_call1_e(env, 0x30,
                                        (long)"nexus/overlay/NexusBattleMenuHost");
                                state_mid = 0;
                                if (battle_cls != 0
                                    && jni_call0_e(env, 0x720) == 0) {
                                    state_mid = jni_call3_e(env, 0x388,
                                            battle_cls,
                                            (long)"onNativeBattleState",
                                            (long)"(ZJ)Z");
                                }
                                if (state_mid != 0
                                    && jni_call0_e(env, 0x720) == 0) {
                                    toggle_mid = jni_call3_e(env, 0x388,
                                            battle_cls,
                                            (long)"togglePanelFromNative",
                                            (long)(intptr_t)g_sig_toggle_panel);
                                    if (jni_call0_e(env, 0x720) != 0) {
                                        jni_call0_e(env, 0x88);
                                        toggle_mid = 0;
                                    }
                                    g_overlay_host_class = (void *)(intptr_t)
                                        jni_call1_e(env, 0xa8, host_cls);
                                    g_overlay_open_from_native_mid =
                                        (void *)(intptr_t)open_mid;
                                    g_battle_menu_host_class = (void *)(intptr_t)
                                        jni_call1_e(env, 0xa8, battle_cls);
                                    g_battle_menu_toggle_mid =
                                        (void *)(intptr_t)toggle_mid;
                                    g_battle_state_notify_mid =
                                        (void *)(intptr_t)state_mid;
                                    stored = 1;
                                }
                            }
                        }
                    }
                    /* shared release + arm check */
                    if (jni_call0_e(env, 0x720) != 0)
                        jni_call0_e(env, 0x88);
                    if (battle_cls != 0)
                        jni_call1_e(env, 0xb8, battle_cls);
                    if (host_cls != 0)
                        jni_call1_e(env, 0xb8, host_cls);
                    if (bridge_cls != 0)
                        jni_call1_e(env, 0xb8, bridge_cls);
                    if (stored && g_overlay_host_class != NULL
                        && g_battle_menu_host_class != NULL
                        && jni_call0_e(env, 0x720) == 0) {
                        g_battle_notify_ready = 1;
                        g_notify_armed = 1;
                        armed = 1;
                    }
                    if (armed)
                        goto social_registration;
                }
                if (jni_call0_e(env, 0x720) != 0)
                    jni_call0_e(env, 0x88);
            }
            g_native_guards_armed = 0;  /* verify ok but the bridge did not bind */
            goto link_failed;
        }
    }
    /* verify failed (or the proof class is missing): still run the
     * registration path with the guards unarmed */

social_registration:
    /* --- social registration: dialog manager + GameApp method ids --- */
    g_ui_initialize_reason = g_stage_social_registration;
    g_native_guards_armed = 1;
    if (jni_call1_e(env, 0x98 /* PushLocalFrame */, 0xc) < 0) {
        jni_call0_e(env, 0x88 /* ExceptionClear */);
    } else {
        dialog_mgr = jni_call1_e(env, 0x30,
                                 (long)"com/supercell/titan/NativeDialogManager");
        dialog_frag = jni_call1_e(env, 0x30, (long)"android/app/DialogFragment");
        dialog_cls = jni_call1_e(env, 0x30, (long)"android/app/Dialog");
        text_view = jni_call1_e(env, 0x30, (long)"android/widget/TextView");
        object_cls = jni_call1_e(env, 0x30, (long)"java/lang/Object");
        view_cls = jni_call1_e(env, 0x30, (long)"android/view/View");
        if (jni_call0_e(env, 0x720) == 0 && dialog_mgr != 0 && dialog_frag != 0
            && dialog_cls != 0 && text_view != 0 && object_cls != 0
            && view_cls != 0) {
            g_dialog_mgr_field = jni_call3_e(env, 0x480 /* GetStaticFieldID */,
                    dialog_mgr, (long)(intptr_t)g_name_dialog_field,
                    (long)"Lcom/supercell/titan/NativeDialogManager;");
            g_dialog_getdialog_mid = jni_call3_e(env, 0x108, dialog_frag,
                    (long)"getDialog", (long)"()Landroid/app/Dialog;");
            g_dialog_findviewbyid_mid = jni_call3_e(env, 0x108, dialog_cls,
                    (long)"findViewById", (long)"(I)Landroid/view/View;");
            g_dialog_gettext_mid = jni_call3_e(env, 0x108, text_view,
                    (long)"getText", (long)"()Ljava/lang/CharSequence;");
            g_dialog_tostring_mid = jni_call3_e(env, 0x108, object_cls,
                    (long)"toString", (long)"()Ljava/lang/String;");
            g_dialog_performclick_mid = jni_call3_e(env, 0x108, view_cls,
                    (long)"performClick", (long)(intptr_t)g_sig_verify_z);
            g_dialog_isshown_mid = jni_call3_e(env, 0x108, view_cls,
                    (long)"isShown", (long)(intptr_t)g_sig_verify_z);
            g_dialog_isenabled_mid = jni_call3_e(env, 0x108, view_cls,
                    (long)"isEnabled", (long)(intptr_t)g_sig_verify_z);
            if (jni_call0_e(env, 0x720) == 0 && g_dialog_mgr_field != 0
                && g_dialog_getdialog_mid != 0 && g_dialog_findviewbyid_mid != 0
                && g_dialog_gettext_mid != 0 && g_dialog_tostring_mid != 0
                && g_dialog_performclick_mid != 0 && g_dialog_isshown_mid != 0
                && g_dialog_isenabled_mid != 0)
                g_dialog_mgr_class = (void *)(intptr_t)
                    jni_call1_e(env, 0xa8, dialog_mgr);
        }
        if (jni_call0_e(env, 0x720) != 0)
            jni_call0_e(env, 0x88);
        jni_call1_e(env, 0xa0 /* PopLocalFrame */, 0);
    }
    gameapp = jni_call1_e(env, 0x30, (long)"com/supercell/titan/GameApp");
    if (gameapp != 0) {
        g_gameapp_getinstance_mid = jni_call3_e(env, 0x388, gameapp,
                (long)"getInstance", (long)"()Lcom/supercell/titan/GameApp;");
        if (g_gameapp_getinstance_mid != 0
            && jni_call0_e(env, 0x720) == 0)
            g_gameapp_class = (void *)(intptr_t)
                jni_call1_e(env, 0xa8, gameapp);
        jni_call1_e(env, 0xb8, gameapp);
    }
    if (jni_call0_e(env, 0x720) != 0)
        jni_call0_e(env, 0x88);

    /* --- ui backend (kind 1) --- */
    memset(backend_block, 0, sizeof backend_block);
    backend_block[0] = g_menu_backend_header;
    backend_block[1] = g_menu_backend_word1;
    backend_block[2] = g_menu_backend_word2;
    backend_block[3] = g_menu_backend_word3;
    backend_block[4] = g_menu_backend_word4;
    backend_block[5] = g_menu_backend_word5;
    backend_block[6] = (int64_t)(intptr_t)g_menu_backend_fn6;
    backend_block[7] = (int64_t)(intptr_t)g_menu_backend_fn7;
    if (nexus_menu_register_backend(1, (int *)backend_block) != 1)
        goto link_failed;

    /* --- AdvancedEvasionController native hooks --- */
    g_ui_initialize_reason = g_stage_menu_facade_class;
    evasion_cls = jni_call1_e(env, 0x30,
            (long)"nexus/brawl/evasion/AdvancedEvasionController");
    if (evasion_cls != 0) {
        evasion_hooks[0].name = g_evasion_hook_name;
        evasion_hooks[0].sig = g_evasion_hook_sig;
        evasion_hooks[0].fn = (void *)(intptr_t)g_evasion_hook_fn;
        evasion_hooks[1].name = g_evasion_state_name;
        evasion_hooks[1].sig = g_evasion_state_sig;
        evasion_hooks[1].fn = (void *)(intptr_t)g_evasion_state_fn;
        evasion_hooks[2].name = g_evasion_diag_name;
        evasion_hooks[2].sig = g_evasion_diag_sig;
        evasion_hooks[2].fn = (void *)(intptr_t)g_evasion_diag_fn;
        g_ui_initialize_reason = g_stage_menu_method_signature;
        hook_mid = jni_call3_e(env, 0x388, evasion_cls,
                               (long)"startNativeUiHooks",
                               (long)(intptr_t)g_sig_noarg_z);
        load_mid = 0;
        diag_mid = 0;
        if (hook_mid != 0)
            load_mid = jni_call3_e(env, 0x388, evasion_cls,
                                   (long)"getNativeMenuLoadState",
                                   (long)(intptr_t)g_sig_snapshot_result);
        if (load_mid != 0)
            diag_mid = jni_call3_e(env, 0x388, evasion_cls,
                                   (long)"getNativeMenuDiagnostics",
                                   (long)"()Ljava/lang/String;");
        if (hook_mid == 0 || load_mid == 0 || diag_mid == 0) {
            jni_call1_e(env, 0xb8, evasion_cls);
            goto link_failed;
        }
        g_ui_initialize_reason = g_stage_menu_native_registration;
        rc = (int)jni_call3_e(env, 0x6b8 /* RegisterNatives */, evasion_cls,
                              (long)(void *)evasion_hooks, 3);
        jni_call1_e(env, 0xb8, evasion_cls);
        if (rc != 0)
            goto link_failed;

        /* --- postbattle + social island install, native-loaded report --- */
        rc = postbattle_install();
        if (rc == 1) {
            if (social_islands_install() == 0)
                ui_stage_log("social_unavailable", "native_guard_or_install");
            stage = "hooks_installed";
            reason = "four_preserving_observers";
        } else {
            if (rc < 0) {
                ui_stage_log("fatal", "target_write_and_restore_unverified");
                jni_diagnostics_failure(vm, "target_restore_unverified",
                                        "native_link_failed");
                abort();
            }
            stage = "disabled";
            reason = "publication_failed_passthrough";
        }
        g_ui_release_armed = (uint32_t)(rc == 1);
        ui_stage_log(stage, reason);
        jni_native_loaded_report(vm);
        return 0x10006;
    }

link_failed:
    ui_stage_log("disabled", "menu_host_or_JNI_registration");
    jni_diagnostics_failure(vm, g_ui_initialize_reason, "native_link_failed");
    return 0xffffffff;

disabled_tail:
    ui_stage_log("disabled", g_ui_initialize_reason);
    jni_diagnostics_failure(vm, g_ui_initialize_reason,
                            "required_synchronous_callback_absent");
    return 0x10006;
}
/* widgets chain 3 chunk 3: covers raw lines 1067-1104 */

/* ===== JNI startup stage log (NexusLab69252) ===== */

uint32_t g_ui_stage_log_count;   /* 0x22f428 — stage lines logged, capped at 0x40 */

/*
 * ui_stage_log — emit one capped "NexusLab69252" stage line (prio 4) as the
 * JSON blob below; the stage is prefixed "runtime_ui_" and the game digest
 * is pinned. The trailing pointers/counters are logged as null/0 slots at
 * this stage (they belong to the later menu frame). @ 001524e8
 */
void ui_stage_log(const char *stage, const char *reason)
{
    struct timespec now;
    uint64_t ms;
    pid_t pid;
    pid_t tid;

    if (g_ui_stage_log_count >= 0x40)
        return;
    g_ui_stage_log_count = g_ui_stage_log_count + 1;

    pid = getpid();
    tid = gettid();
    if (clock_gettime(0 /* CLOCK_REALTIME */, &now) == 0)
        ms = (uint64_t)now.tv_sec * 1000 + (uint64_t)now.tv_nsec / 1000000;
    else
        ms = 0;

    __android_log_print(4, "NexusLab69252",
        "{\"stage\":\"runtime_ui_%s\",\"reason\":\"%s\",\"pid\":%d,\"tid\":%d,"
        "\"time_ms\":%llu,\"game_sha256\":\"%s\",\"stage_ptr\":\"%p\","
        "\"root\":\"%p\",\"launcher\":\"%p\",\"panel\":\"%p\",\"open\":%d,"
        "\"scheduling_rva\":\"0x5921a0\",\"observed_text\":\"%p\","
        "\"observed_movie\":\"%p\",\"observed_movie_vptr\":\"%p\","
        "\"movie_be_s16\":%d}",
        stage, reason, pid, tid, ms,
        "a10aeb6b4085fb2a15d969a130cd9608231998b5269a40aed41129d99624ede3",
        NULL, NULL, NULL, NULL, 0, NULL, NULL, NULL, 0);
}
/* widgets chain 3 chunk 4: covers raw lines 1106-1591 (ui_android_dialog_bind straddled the 1550 cut and is finished here) */

/* ===== JNI vtable veneers, 4/5-arg forms (see chunk 2) ===== */

static long jni_call4_e(void *obj, uint32_t slot, long a, long b, long c, long d)
{
    long *table = *(long **)obj;
    long (*fn)(void *, long, long, long, long) =
        (long (*)(void *, long, long, long, long))table[slot / 8];
    return fn(obj, a, b, c, d);
}

static long jni_call5_e(void *obj, uint32_t slot, long a, long b, long c,
                        long d, long e)
{
    long *table = *(long **)obj;
    long (*fn)(void *, long, long, long, long, long) =
        (long (*)(void *, long, long, long, long, long))table[slot / 8];
    return fn(obj, a, b, c, d, e);
}

/* ===== cross-file functions ===== */

extern int string_obj_from_cstr(const char *s,
                                uint64_t obj[3]); /* misc.c @ 00190fd0 — obj[0]
                                                   * = handle, obj[1] = length */
extern uint32_t plus_request_submit(int32_t mode, const void *name, size_t len,
                                    uint64_t parent, uint64_t env);
                                      /* misc.c @ 00159bbc — store a plus
                                       * request; 2 stored / 3 pending / 4 bad */
extern void *dialog_close_wait_thread(void *arg);
                                      /* misc.c @ 0015b33c — watches the
                                       * dialog until isShowing turns false */
void ui_toast_show(const char *text);  /* below @ 0015b620 (chunk 5) */

/* ===== globals owned elsewhere ===== */

extern int32_t g_dialog_bind_latch;   /* 0x25c9d8 — misc.c: dialog bind in-flight latch */

/* ===== rodata: dialog texts / JNI names / signatures ===== */

extern const char g_dialog_hint_brawltv[];    /* 0x1317d0 — EditText hint (mode 0x7f) */
extern const char g_dialog_hint_other[];      /* 0x139425 — EditText hint (other modes) */
extern const char g_dialog_button_positive[]; /* 0x13af51 — positive-button label */
extern const char g_dialog_toast_cancelled[]; /* 0x13a33f — toast when the dialog path aborts */
extern const char g_dialog_toast_too_long[];  /* 0x1311c6 — toast for input longer than 0x60 */
extern const char g_dialog_toast_ok_brawltv[];/* 0x13729c — accepted toast (mode 0x7f) */
extern const char g_dialog_toast_ok_other[];  /* 0x13b4da — accepted toast (other modes) */
extern const char g_name_looper_loop[];       /* 0x13af3f — Looper "loop" */
extern const char g_name_dialog_show[];       /* 0x137c35 — Dialog/Toast "show" */
extern const char g_sig_bool_v[];             /* 0x13b4c4 — "(Z)V" */

/*
 * ui_sc_bundle_ready — verify the "sc/ui.sc" bundle is live and carries the
 * two popover styles this UI needs. The bundle-override gates at libg+
 * 0x12eb0e8 and +0x12eb170 must both be clear, the bundle must open
 * (libg+0x51eacc, flag 0) with its +0x80 ready bit set, and the style-table
 * chain (+0x88 -> +0x8 -> +0xf8) plus the table dims (+0x100/+0x110 in
 * (0, 0x10000)) must resolve. Both "popover_button_blue" and
 * "popover_button_spectate" must convert to string objects the bundle's
 * style lookup (libg+0x5dfbf8) resolves. Returns 1 when both resolve.
 * @ 001554b0
 */
int ui_sc_bundle_ready(void *ctx)
{
    uint8_t gate_a = 1, gate_b = 1, ready = 0;
    uint64_t bundle, table, node, dim_rows, dim_cols;
    uint64_t str_obj[3];
    uint64_t hit;

    (void)ctx;  /* the raw arg 1 is the dead x0 slot of proc_mem_read */

    if (proc_mem_read(NULL, (uint64_t)g_proc_mem_bias + 0x12eb0e8,
                      &gate_a, 1) == 0 || gate_a != 0)
        return 0;
    if (proc_mem_read(NULL, (uint64_t)g_proc_mem_bias + 0x12eb170,
                      &gate_b, 1) == 0 || gate_b != 0)
        return 0;
    bundle = ((uint64_t (*)(const char *, int))(
        (uint64_t)g_proc_mem_bias + 0x51eacc))("sc/ui.sc", 0);
    if (bundle == 0)
        return 0;
    if (proc_mem_read(NULL, bundle + 0x80, &ready, 1) == 0)
        return 0;
    if ((ready & 1) == 0)
        return 0;

    table = 0;
    if (proc_mem_read(NULL, bundle + 0x88, &table, 8) == 0
        || !(0xfff < table && (table & 7) == 0))
        return 0;
    node = 0;
    if (proc_mem_read(NULL, table + 8, &node, 8) == 0
        || !(0xfff < node && (node & 7) == 0))
        return 0;
    /* the +0xf8 deref is a validity probe; the lookup target is node+0xf8 */
    {
        uint64_t probe = 0;

        if (proc_mem_read(NULL, node + 0xf8, &probe, 8) == 0
            || probe < 0x1000 || (probe & 7) != 0)
            return 0;
    }
    dim_rows = 0;
    if (proc_mem_read(NULL, node + 0x100, &dim_rows, 8) == 0
        || dim_rows == 0 || 0x10000 < dim_rows)
        return 0;
    dim_cols = 0;
    if (proc_mem_read(NULL, node + 0x110, &dim_cols, 8) == 0
        || dim_cols == 0 || 0x10000 < dim_cols)
        return 0;

    if (string_obj_from_cstr("popover_button_blue", str_obj) == 0)
        return 0;
    hit = ((uint64_t (*)(uint64_t, void *))(
        (uint64_t)g_proc_mem_bias + 0x5dfbf8))(node + 0xf8, str_obj);
    if (hit == 0)
        return 0;
    if (string_obj_from_cstr("popover_button_spectate", str_obj) == 0)
        return 0;
    hit = ((uint64_t (*)(uint64_t, void *))(
        (uint64_t)g_proc_mem_bias + 0x5dfbf8))(node + 0xf8, str_obj);
    return hit != 0;
}

/*
 * widget_overlay_schema_publish — hand the overlay wire schema to Java as
 * one NewStringUTF string. The literal below is the head of the embedded
 * "nexus-overlay-wire/v1" document (six categories, then the entries list:
 * menu.killaura, menu.autododge, menu.follow, menu.aim, ...); Ghidra
 * truncated the literal mid-way — the full document carries all 233 entries
 * (see docs/feature_list.json) and stays byte-for-byte in the original
 * image. The returned jstring is left in x0 (the raw drops it).
 * @ 00158f5c
 */
void widget_overlay_schema_publish(void *env)
{
    (void)jni_call1_e(env, 0x538 /* NewStringUTF */,
        (long)(intptr_t)
        "{\"schema\":\"nexus-overlay-wire/v1\",\"categories\":["
        "{\"id\":\"main\",\"label\":\"Main\",\"icon\":\"sliders\"},"
        "{\"id\":\"combat\",\"label\":\"Combat\",\"icon\":\"crosshair\"},"
        "{\"id\":\"visual\",\"label\":\"Visual\",\"icon\":\"eye\"},"
        "{\"id\":\"utility\",\"label\":\"Utility\",\"icon\":\"wrench\"},"
        "{\"id\":\"settings\",\"label\":\"Settings\",\"icon\":\"gear\"},"
        "{\"id\":\"plus\",\"label\":\"Nexus+\",\"icon\":\"star\"}],"
        "\"entries\":["
        "{\"key\":\"menu.killaura\",\"label\":\"Kill aura\",\"description\":"
        "\"\\u041d\\u0430\\u0441\\u0442\\u0440\\u043e\\u0439\\u043a\\u0430 "
        "\\u0430\\u0432\\u0442\\u043e\\u043c\\u0430\\u0442\\u0438\\u0447"
        "\\u0435\\u0441\\u043a\\u043e\\u0433\\u043e \\u0432\\u044b\\u0431"
        "\\u043e\\u0440\\u0430 \\u0446\\u0435\\u043b\\u0438 \\u0438 "
        "\\u0430\\u0442\\u0430\\u043a\\u0438.\",\"icon\":\"crosshair\","
        "\"category\":\"combat\",\"parentKey\":\"\",\"control\":4,"
        "\"actionId\":7,\"queryActionId\":17,\"min\":0,\"max\":0,\"step\":0,"
        "\"incrementActionId\":-1,\"decrementActionId\":-1,\"screen\":75,"
        "\"paid\":false,\"topLevel\":true,\"options\":[],\"aliases\":[7]},"
        "{\"key\":\"menu.autododge\",\"label\":\"Auto dodge\",\"description\":"
        "\"\\u041d\\u0430\\u0441\\u0442\\u0440\\u043e\\u0439\\u043a\\u0430 "
        "\\u0443\\u043a\\u043b\\u043e\\u043d\\u0435\\u043d\\u0438\\u044f "
        "\\u043e\\u0442 \\u0441\\u043d\\u0430\\u0440\\u044f\\u0434\\u043e"
        "\\u0432.\",\"icon\":\"shield\",\"category\":\"combat\","
        "\"parentKey\":\"\",\"control\":4,\"actionId\":8,"
        "\"queryActionId\":22,\"min\":0,\"max\":0,\"step\":0,"
        "\"incrementActionId\":-1,\"decrementActionId\":-1,\"screen\":68,"
        "\"paid\":false,\"topLevel\":true,\"options\":[],\"aliases\":[8]},"
        "{\"key\":\"menu.follow\",\"label\":\"Follow\",\"description\":"
        "\"\\u041d\\u0430\\u0441\\u0442\\u0440\\u043e\\u0439\\u043a\\u0430 "
        "\\u0441\\u043b\\u0435\\u0434\\u043e\\u0432\\u0430\\u043d\\u0438"
        "\\u044f \\u0437\\u0430 \\u0441\\u043e\\u044e\\u0437\\u043d\\u0438"
        "\\u043a\\u043e\\u043c.\",\"icon\":\"user\",\"category\":\"utility\","
        "\"parentKey\":\"\",\"control\":4,\"actionId\":10,"
        "\"queryActionId\":26,\"min\":0,\"max\":0,\"step\":0,"
        "\"incrementActionId\":-1,\"decrementActionId\":-1,\"screen\":70,"
        "\"paid\":true,\"topLevel\":true,\"options\":[],\"aliases\":[10]},"
        "{\"key\":\"menu.aim\",\"label\":\"Smart aim\",\"description\":"
        "\"\\u041d\\u0430\\u0441\\u0442\\u0440\\u043e\\u0439\\u043a\\u0430 "
        "\\u043f\\u0440\\u0438\\u0446\\u0435\\u043b"
        /* ... Ghidra truncated the literal here; the full 233-entry wire
         * document continues in the original image ... */);
}

/* ===== tag-input dialog worker (spawned by plus_request_dispatch) ===== */

/* 0x38-byte job block (calloc'd by plus_request_dispatch; the tail flags
 * are shared with misc.c's dialog_close_wait_thread). */
typedef struct {
    int32_t  mode;        /* +0x00 request mode (0x7d/0x7f/0x80) */
    uint32_t _pad04;      /* +0x04 */
    uint64_t parent;      /* +0x08 parent widget */
    uint64_t env;         /* +0x10 plus request env handle */
    void    *dialog_ref;  /* +0x18 AlertDialog global ref */
    void    *looper_ref;  /* +0x20 worker Looper global ref */
    uint32_t started;     /* +0x28 close-wait thread started */
    uint32_t pump_done;   /* +0x2c close-wait thread finished */
    uint32_t closed;      /* +0x30 dialog closed (isShowing false) */
    uint32_t done;        /* +0x34 bind worker done */
} dialog_job_t;

/*
 * ui_android_dialog_bind — pthread body for the tag-input dialog. Attaches
 * to the activation JVM, prepares a worker Looper, resolves the whole
 * AlertDialog/EditText method set and builds the mode-specific dialog
 * (0x7d "Spectate With Tag", 0x80 "Invite By Tag", else "Visual BrawlTV";
 * single-line input, input type 0x1091 vs 2, soft-input mode 0x15). The
 * dialog and its Looper are global-ref'd into the job,
 * dialog_close_wait_thread is spawned to watch it, and this thread then
 * runs Looper.loop() until the dialog closes. On a clean close the GameApp
 * context must still be the same live object (IsSameObject, not finishing/
 * destroyed); the trimmed input (1..0x60 UTF-8 bytes) is submitted through
 * plus_request_submit — verdict 4 shows the accepted toast. Any lookup or
 * call failure dismisses the dialog, toasts the cancelled message and
 * joins the watcher when it had started. Always frees the job, clears the
 * 0x25c9d8 latch, and detaches when it had to attach. Returns 0.
 * @ 0015a108
 */
void *ui_android_dialog_bind(void *arg)
{
    dialog_job_t *job = arg;
    void *env = NULL;
    pthread_t watcher;
    long looper_cls = 0, builder_cls = 0, alert_cls = 0, edit_cls = 0;
    long activity_cls = 0, object_cls = 0, window_cls = 0;
    long mid_prepare = 0, mid_mylooper = 0, mid_loop = 0;
    long builder_init = 0, edit_init = 0;
    long mid_settitle = 0, mid_setview = 0, mid_setpositive = 0;
    long mid_setcancelable = 0, mid_create = 0, mid_show = 0;
    long mid_dismiss = 0, mid_setcanceled = 0, mid_setsingle = 0;
    long mid_sethint = 0, mid_setinput = 0, mid_gettext = 0;
    long mid_tostring = 0, mid_requestfocus = 0, mid_getwindow = 0;
    long mid_setsoft = 0, mid_isfinishing = 0, mid_isdestroyed = 0;
    long looper, context, builder, edit, dialog, window;
    long jtitle, jhint, jbutton, editable, jstr, utf;
    long fresh_context, len;
    char finishing, destroyed, same;
    uint32_t input_type, verdict;
    const char *title, *hint;
    int need_detach = 0;
    int frame_pushed = 0;
    int watcher_spawned = 0;
    int join_watcher = 0;
    int rc, i;

    if (g_activation_jvm == NULL || g_activation_class == NULL)
        goto out;

    rc = (int)jni_call2_e(g_activation_jvm, 0x30 /* GetEnv */,
                          (long)(void *)&env, 0x10006 /* JNI_VERSION_1_6 */);
    if (rc != 0) {
        if (rc != -2 /* JNI_EDETACHED */)
            goto out;
        if (jni_call2_e(g_activation_jvm, 0x20 /* AttachCurrentThread */,
                        (long)(void *)&env, 0) != 0)
            goto out;
        need_detach = 1;
    }

    if (env == NULL)
        goto out;

    if (jni_call1_e(env, 0x98 /* PushLocalFrame */, 0x30) != 0)
        goto cleanup;
    frame_pushed = 1;

    looper_cls = jni_call1_e(env, 0x30 /* FindClass */, (long)"android/os/Looper");
    if (looper_cls == 0 || jni_call0_e(env, 0x720 /* ExceptionCheck */) != 0)
        goto cleanup;
    builder_cls = jni_call1_e(env, 0x30, (long)"android/app/AlertDialog$Builder");
    if (builder_cls == 0 || jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    alert_cls = jni_call1_e(env, 0x30, (long)"android/app/AlertDialog");
    if (alert_cls == 0 || jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    edit_cls = jni_call1_e(env, 0x30, (long)"android/widget/EditText");
    if (edit_cls == 0 || jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    activity_cls = jni_call1_e(env, 0x30, (long)"android/app/Activity");
    if (activity_cls == 0 || jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    object_cls = jni_call1_e(env, 0x30, (long)"java/lang/Object");
    if (object_cls == 0 || jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    window_cls = jni_call1_e(env, 0x30, (long)"android/view/Window");
    if (window_cls == 0 || jni_call0_e(env, 0x720) != 0)
        goto cleanup;

    mid_prepare = jni_call3_e(env, 0x388 /* GetStaticMethodID */, looper_cls,
                              (long)"prepare", (long)(intptr_t)g_sig_noarg_z);
    if (mid_prepare == 0 || jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    mid_mylooper = jni_call3_e(env, 0x388, looper_cls, (long)"myLooper",
                               (long)"()Landroid/os/Looper;");
    if (mid_mylooper == 0 || jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    mid_loop = jni_call3_e(env, 0x388, looper_cls,
                           (long)(intptr_t)g_name_looper_loop,
                           (long)(intptr_t)g_sig_noarg_z);
    if (mid_loop == 0 || jni_call0_e(env, 0x720) != 0)
        goto cleanup;

    builder_init = jni_call3_e(env, 0x108 /* GetMethodID */, builder_cls,
                               (long)"<init>", (long)"(Landroid/content/Context;)V");
    if (builder_init == 0 || jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    edit_init = jni_call3_e(env, 0x108, edit_cls, (long)"<init>",
                            (long)"(Landroid/content/Context;)V");
    if (edit_init == 0 || jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    mid_settitle = jni_call3_e(env, 0x108, builder_cls, (long)"setTitle",
        (long)"(Ljava/lang/CharSequence;)Landroid/app/AlertDialog$Builder;");
    if (mid_settitle == 0 || jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    mid_setview = jni_call3_e(env, 0x108, builder_cls, (long)"setView",
        (long)"(Landroid/view/View;)Landroid/app/AlertDialog$Builder;");
    if (mid_setview == 0 || jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    mid_setpositive = jni_call3_e(env, 0x108, builder_cls,
        (long)"setPositiveButton",
        (long)"(Ljava/lang/CharSequence;Landroid/content/DialogInterface$OnClickListener;)Landroid/app/AlertDialog$Builder;");
    if (mid_setpositive == 0 || jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    mid_setcancelable = jni_call3_e(env, 0x108, builder_cls,
        (long)"setCancelable",
        (long)"(Z)Landroid/app/AlertDialog$Builder;");
    if (mid_setcancelable == 0 || jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    mid_create = jni_call3_e(env, 0x108, builder_cls, (long)"create",
                             (long)"()Landroid/app/AlertDialog;");
    if (mid_create == 0 || jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    mid_show = jni_call3_e(env, 0x108, alert_cls,
                           (long)(intptr_t)g_name_dialog_show,
                           (long)(intptr_t)g_sig_noarg_z);
    if (mid_show == 0 || jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    mid_dismiss = jni_call3_e(env, 0x108, alert_cls, (long)"dismiss",
                              (long)(intptr_t)g_sig_noarg_z);
    if (mid_dismiss == 0 || jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    mid_setcanceled = jni_call3_e(env, 0x108, alert_cls,
        (long)"setCanceledOnTouchOutside", (long)(intptr_t)g_sig_bool_v);
    if (mid_setcanceled == 0 || jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    mid_setsingle = jni_call3_e(env, 0x108, edit_cls, (long)"setSingleLine",
                                (long)(intptr_t)g_sig_bool_v);
    if (mid_setsingle == 0 || jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    mid_sethint = jni_call3_e(env, 0x108, edit_cls, (long)"setHint",
                              (long)"(Ljava/lang/CharSequence;)V");
    if (mid_sethint == 0 || jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    mid_setinput = jni_call3_e(env, 0x108, edit_cls, (long)"setInputType",
                               (long)(intptr_t)g_sig_input_dismiss);
    if (mid_setinput == 0 || jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    mid_gettext = jni_call3_e(env, 0x108, edit_cls, (long)"getText",
                              (long)"()Landroid/text/Editable;");
    if (mid_gettext == 0 || jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    mid_tostring = jni_call3_e(env, 0x108, object_cls, (long)"toString",
                               (long)"()Ljava/lang/String;");
    if (mid_tostring == 0 || jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    mid_requestfocus = jni_call3_e(env, 0x108, edit_cls, (long)"requestFocus",
                                   (long)(intptr_t)g_sig_verify_z);
    if (mid_requestfocus == 0 || jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    mid_getwindow = jni_call3_e(env, 0x108, alert_cls, (long)"getWindow",
                                (long)"()Landroid/view/Window;");
    if (mid_getwindow == 0 || jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    mid_setsoft = jni_call3_e(env, 0x108, window_cls, (long)"setSoftInputMode",
                              (long)(intptr_t)g_sig_input_dismiss);
    if (mid_setsoft == 0 || jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    mid_isfinishing = jni_call3_e(env, 0x108, activity_cls, (long)"isFinishing",
                                  (long)(intptr_t)g_sig_verify_z);
    if (mid_isfinishing == 0 || jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    mid_isdestroyed = jni_call3_e(env, 0x108, activity_cls, (long)"isDestroyed",
                                  (long)(intptr_t)g_sig_verify_z);
    if (mid_isdestroyed == 0 || jni_call0_e(env, 0x720) != 0)
        goto cleanup;

    /* --- build and run the dialog --- */
    jni_call2_e(env, 0x468 /* CallStaticVoidMethod */, looper_cls, mid_prepare);
    if (jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    looper = jni_call2_e(env, 0x390 /* CallStaticObjectMethod */, looper_cls,
                         mid_mylooper);
    if (jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    context = jni_call2_e(env, 0x390, (long)(intptr_t)g_gameapp_class,
                          g_gameapp_getinstance_mid);
    if (jni_call0_e(env, 0x720) != 0)
        goto cleanup;

    dialog = 0;
    if (looper == 0 || context == 0)
        goto cleanup;

    builder = jni_call3_e(env, 0xe0 /* NewObject */, builder_cls,
                          builder_init, context);
    if (jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    edit = jni_call3_e(env, 0xe0, edit_cls, edit_init, context);
    if (jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    dialog = 0;
    if (builder == 0 || edit == 0)
        goto cleanup;

    title = "Visual BrawlTV";
    if (job->mode == 0x80)
        title = "Invite By Tag";
    if (job->mode == 0x7d)
        title = "Spectate With Tag";
    hint = (job->mode == 0x7f) ? g_dialog_hint_brawltv : g_dialog_hint_other;
    input_type = (job->mode == 0x7f) ? 2 : 0x1091;

    jtitle = jni_call1_e(env, 0x538 /* NewStringUTF */, (long)(intptr_t)title);
    if (jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    jhint = jni_call1_e(env, 0x538, (long)(intptr_t)hint);
    if (jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    jbutton = jni_call1_e(env, 0x538,
                          (long)(intptr_t)g_dialog_button_positive);
    if (jni_call0_e(env, 0x720) != 0)
        goto cleanup;

    jni_call3_e(env, 0x1e8 /* CallVoidMethod */, edit, mid_setsingle, 1);
    if (jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    jni_call3_e(env, 0x1e8, edit, mid_sethint, jhint);
    if (jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    jni_call3_e(env, 0x1e8, edit, mid_setinput, (long)input_type);
    if (jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    jni_call3_e(env, 0x110 /* CallObjectMethod */, builder, mid_settitle,
                jtitle);
    if (jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    jni_call3_e(env, 0x110, builder, mid_setview, edit);
    if (jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    jni_call4_e(env, 0x110, builder, mid_setpositive, jbutton, 0);
    if (jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    jni_call3_e(env, 0x110, builder, mid_setcancelable, 0);
    if (jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    dialog = jni_call2_e(env, 0x110, builder, mid_create);
    if (jni_call0_e(env, 0x720) != 0 || dialog == 0)
        goto cleanup;

    jni_call3_e(env, 0x1e8, dialog, mid_setcanceled, 0);
    if (jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    jni_call2_e(env, 0x1e8, dialog, mid_show);
    if (jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    jni_call2_e(env, 0x128 /* CallBooleanMethod */, edit, mid_requestfocus);
    if (jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    window = jni_call2_e(env, 0x110, dialog, mid_getwindow);
    if (jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    if (window != 0) {
        jni_call3_e(env, 0x1e8, window, mid_setsoft, 0x15);
        if (jni_call0_e(env, 0x720) != 0)
            goto cleanup;
    }

    job->dialog_ref = (void *)(intptr_t)jni_call1_e(env, 0xa8 /* NewGlobalRef */,
                                                    dialog);
    if (jni_call0_e(env, 0x720) != 0)
        goto cleanup;
    job->looper_ref = (void *)(intptr_t)jni_call1_e(env, 0xa8, looper);
    if (jni_call0_e(env, 0x720) != 0 || job->dialog_ref == NULL
        || job->looper_ref == NULL)
        goto cleanup;

    if (pthread_create(&watcher, NULL, dialog_close_wait_thread, job) != 0)
        goto cleanup;
    watcher_spawned = 1;

    /* wait up to 1 s (100 x 10 ms) for the watcher to start */
    for (i = 100; i != 0; i--) {
        if (job->started != 0 || job->pump_done != 0)
            break;
        usleep(10000);
    }
    if (job->started == 0) {
        join_watcher = 1;
        goto cleanup;
    }

    /* run the Looper until the dialog closes */
    jni_call2_e(env, 0x468, looper_cls, mid_loop);
    if (jni_call0_e(env, 0x720) != 0) {
        join_watcher = 1;
        goto cleanup;
    }
    job->done = 1;
    pthread_join(watcher, NULL);
    watcher_spawned = 0;

    if (job->closed == 0)
        goto finished;

    /* the context must still be the same, live GameApp */
    fresh_context = jni_call2_e(env, 0x390, (long)(intptr_t)g_gameapp_class,
                                g_gameapp_getinstance_mid);
    if (jni_call0_e(env, 0x720) != 0)
        goto finished;
    finishing = (char)jni_call2_e(env, 0x128, context, mid_isfinishing);
    if (jni_call0_e(env, 0x720) != 0)
        goto finished;
    destroyed = (char)jni_call2_e(env, 0x128, context, mid_isdestroyed);
    if (jni_call0_e(env, 0x720) != 0)
        goto finished;
    same = (char)jni_call2_e(env, 0xc0 /* IsSameObject */, context,
                             fresh_context);
    if (fresh_context == 0 || same == 0 || finishing != 0 || destroyed != 0)
        goto finished;

    editable = jni_call2_e(env, 0x110, edit, mid_gettext);
    if (jni_call0_e(env, 0x720) != 0)
        goto finished;
    if (editable == 0)
        goto finished;
    jstr = jni_call2_e(env, 0x110, editable, mid_tostring);
    if (jni_call0_e(env, 0x720) != 0)
        goto finished;
    if (jstr == 0)
        goto finished;

    len = jni_call1_e(env, 0x540 /* GetStringUTFLength */, jstr);
    if (jni_call0_e(env, 0x720) != 0)
        goto finished;
    if (1 <= len && len <= 0x60) {
        utf = jni_call2_e(env, 0x548 /* GetStringUTFChars */, jstr, 0);
        if (jni_call0_e(env, 0x720) != 0)
            goto finished;
        if (utf != 0) {
            verdict = plus_request_submit(job->mode,
                                          (const void *)(intptr_t)utf,
                                          (size_t)len, job->parent, job->env);
            jni_call2_e(env, 0x550 /* ReleaseStringUTFChars */, jstr, utf);
            if (verdict == 4) {
                ui_toast_show(job->mode == 0x7f ? g_dialog_toast_ok_brawltv
                                                : g_dialog_toast_ok_other);
                goto finished;
            }
        }
        goto finished;
    }
    if (0x61 <= len)
        ui_toast_show(g_dialog_toast_too_long);

finished:
    job->done = 1;
    if (jni_call0_e(env, 0x720) != 0)
        jni_call0_e(env, 0x88 /* ExceptionClear */);
    if (job->dialog_ref != NULL)
        jni_call1_e(env, 0xb0 /* DeleteGlobalRef */,
                    (long)(intptr_t)job->dialog_ref);
    if (job->looper_ref != NULL)
        jni_call1_e(env, 0xb0, (long)(intptr_t)job->looper_ref);
    if (frame_pushed)
        jni_call1_e(env, 0xa0 /* PopLocalFrame */, 0);
    goto out;

cleanup:
    if (jni_call0_e(env, 0x720) != 0)
        jni_call0_e(env, 0x88);
    if (dialog != 0 && mid_dismiss != 0) {
        jni_call2_e(env, 0x1e8, dialog, mid_dismiss);
        if (jni_call0_e(env, 0x720) != 0)
            jni_call0_e(env, 0x88);
    }
    ui_toast_show(g_dialog_toast_cancelled);
    job->done = 1;
    if (watcher_spawned && join_watcher) {
        pthread_join(watcher, NULL);
        watcher_spawned = 0;
    }
    if (jni_call0_e(env, 0x720) != 0)
        jni_call0_e(env, 0x88);
    if (job->dialog_ref != NULL)
        jni_call1_e(env, 0xb0, (long)(intptr_t)job->dialog_ref);
    if (job->looper_ref != NULL)
        jni_call1_e(env, 0xb0, (long)(intptr_t)job->looper_ref);
    if (frame_pushed)
        jni_call1_e(env, 0xa0, 0);

out:
    job->done = 1;
    free(job);
    g_dialog_bind_latch = 0;
    if (need_detach != 0)
        jni_call0_e(g_activation_jvm, 0x28 /* DetachCurrentThread */);
    return NULL;
}
/* widgets chain 3 chunk 5: covers raw lines 1593-2216 (ui_trampoline_install straddled the 2000 cut and is finished here) */

#include <sys/mman.h>

/* ===== cross-file functions ===== */

extern int arm64_branch_encode(uint64_t from, uint64_t to,
                               uint32_t *out);  /* misc.c @ 00190d68 — 1 when
                                                  * the branch encodes */
extern void code_cache_flush_range(uint64_t start, uint64_t end);
                                      /* misc.c @ 00193944 */
extern void *ui_server_hook_target(void);  /* misc.c @ 0015d224 — island entry */
extern void gap_span_update(uint64_t *window, uint64_t site,
                            uint64_t prev_end, uint64_t start);
                                      /* @ 00168b00 — gap window updater
                                       * (not in the pushed files) */
extern void *__emutls_get_address(void *control);  /* bionic emutls */
extern size_t __strlen_chk(const char *s, size_t s_len);  /* fortified strlen */

/* ===== globals owned elsewhere ===== */

extern char g_postbattle_tls_ctrl[];   /* 0x1a7c88 — misc.c: emutls control
                                        * (2-word block: trampoline stage
                                        * label + value) */

/* ===== rodata: island template GOT slots ===== */

extern const void *const g_island_tmpl_begin;  /* 0x1a36c0 — GOT: template image start (misc.c) */
extern const void *const g_island_tmpl_end;    /* 0x1a36c8 — GOT: template image end (misc.c) */
extern const void *const g_tmpl_slot_d0;       /* 0x1a36d0 — GOT: resume-branch slot */
extern const void *const g_tmpl_slot_d8;       /* 0x1a36d8 — GOT: movz-literal slot */
extern const void *const g_tmpl_slot_e0;       /* 0x1a36e0 — GOT: alloc-id slot */
extern const void *const g_tmpl_slot_e8;       /* 0x1a36e8 — GOT: hook-fn slot */

/* ===== rodata: trampoline stage labels ===== */

extern const char g_stage_tramp_begin[];  /* 0x132a0b — initial stage label */
extern const char g_stage_tramp_scan[];   /* 0x13673d — page-scan stage label */
extern const char g_stage_tramp_mmap[];   /* 0x13a39a — mmap-failure stage label */
extern const char g_stage_tramp_seal[];   /* 0x13715b — mprotect-failure stage label */

/*
 * ui_toast_show — show a short Toast (duration 0) with the given text on
 * the activation JVM: Toast.makeText(GameApp.getInstance(), text, 0).show()
 * through the captured GameApp method ids. Attaches the thread when
 * needed (and detaches again), pushes an 8-slot local frame and clears
 * any pending exception on the way out. @ 0015b620
 */
void ui_toast_show(const char *text)
{
    void *env = NULL;
    long context = 0, toast_cls, make_text, show_mid, jtext, toast;
    char exc;
    int need_detach = 0;
    int rc;

    if (g_activation_jvm == NULL || g_activation_class == NULL)
        return;

    rc = (int)jni_call2_e(g_activation_jvm, 0x30 /* GetEnv */,
                          (long)(void *)&env, 0x10006 /* JNI_VERSION_1_6 */);
    if (rc == 0) {
        need_detach = 0;
    } else if (rc == -2 /* JNI_EDETACHED */) {
        if (jni_call2_e(g_activation_jvm, 0x20 /* AttachCurrentThread */,
                        (long)(void *)&env, 0) != 0)
            return;
        need_detach = 1;
    } else {
        return;
    }

    if (env == NULL)
        return;

    if (jni_call1_e(env, 0x98 /* PushLocalFrame */, 8) == 0) {
        if (g_gameapp_class != 0)
            context = jni_call2_e(env, 0x390 /* CallStaticObjectMethod */,
                                  (long)(intptr_t)g_gameapp_class,
                                  g_gameapp_getinstance_mid);
        exc = (char)jni_call0_e(env, 0x720 /* ExceptionCheck */);
        if (exc == 0) {
            toast_cls = jni_call1_e(env, 0x30 /* FindClass */,
                                    (long)"android/widget/Toast");
            if (toast_cls != 0) {
                make_text = jni_call3_e(env, 0x388 /* GetStaticMethodID */,
                        toast_cls, (long)"makeText",
                        (long)"(Landroid/content/Context;Ljava/lang/CharSequence;I)Landroid/widget/Toast;");
                show_mid = 0;
                jtext = 0;
                if (make_text != 0) {
                    show_mid = jni_call3_e(env, 0x108 /* GetMethodID */,
                            toast_cls, (long)(intptr_t)g_name_dialog_show,
                            (long)(intptr_t)g_sig_noarg_z);
                    if (show_mid != 0) {
                        jtext = jni_call1_e(env, 0x538 /* NewStringUTF */,
                                            (long)(intptr_t)text);
                    }
                }
                if (jtext != 0 && context != 0
                    && jni_call0_e(env, 0x720) == 0) {
                    toast = jni_call5_e(env, 0x390 /* CallStaticObjectMethod */,
                                        toast_cls, make_text, context, jtext, 0);
                    if (toast != 0 && jni_call0_e(env, 0x720) == 0)
                        jni_call2_e(env, 0x1e8 /* CallVoidMethod */, toast,
                                    show_mid);
                }
            }
        }
        if (jni_call0_e(env, 0x720) != 0)
            jni_call0_e(env, 0x88 /* ExceptionClear */);
        jni_call1_e(env, 0xa0 /* PopLocalFrame */, 0);
    } else {
        if (jni_call0_e(env, 0x720) != 0)
            jni_call0_e(env, 0x88);
    }

    if (need_detach)
        jni_call0_e(g_activation_jvm, 0x28 /* DetachCurrentThread */);
}

/* ===== code-island allocator ===== */

/* gap window handed to gap_span_update: the ±128 MB span around the site
 * plus the candidate list it fills in. */
typedef struct {
    uint64_t lo;             /* +0x000 window low end (page-aligned) */
    uint64_t hi;             /* +0x008 window high end (page-aligned) */
    uint64_t page;           /* +0x010 page size */
    uint64_t candidates[64]; /* +0x018 gap candidate addresses */
    uint32_t count;          /* +0x218 candidates collected */
    uint32_t _pad21c;        /* +0x21c */
} gap_window_t;

/*
 * ui_trampoline_install — allocate and wire one code island for a hook
 * site. The island image is copied from the GOT-delimited template
 * (0x1a36c0..0x1a36c8) with four slots patched: the MOVZ literal
 * (table_id << 5 | 0x52800002), the alloc id, the resume branch back into
 * site+4, and the hook fn (ui_server_hook_target); the page is then
 * cache-flushed and mprotect'ed RX. Pass 1 mmaps pages walking outward
 * from the site in 0x100000 steps (up to 0x6f00000 away, above first,
 * then below), accepting the first page where both ARM64 branches
 * site->page and page+resume_slot->site+4 encode. Pass 2 — when the site
 * is 4-aligned inside the libg image (within bias..bias+0x1400000) and
 * the template layout checks out — scans /proc/self/maps for page-aligned
 * gaps in the ±128 MB window (EINTR retried, perms parsed by hand,
 * candidates collected through gap_span_update, never inside the libg
 * image) and installs into the first gap whose MAP_FIXED mmap succeeds.
 * The emutls pair at 0x1a7c88 carries the stage label ("enter", "resume",
 * "ready", "gap_*", the rodata stage labels) and errno. Returns the island
 * address, 0 on failure. @ 0015b8f4
 */
uint64_t ui_trampoline_install(uint64_t site, uint64_t table_id,
                               uint32_t alloc_id)
{
    uint64_t *tls;
    const uint8_t *tmpl_begin;
    uint64_t tmpl_len;
    uint64_t base, step;
    uint32_t movz_word;
    uint64_t off_d0, off_d8, off_e0, off_e8;
    uint32_t branch_enter, branch_resume;
    uint32_t branch_check;
    gap_window_t window;
    char line[0x2000];
    FILE *f;
    uint64_t lo, span, base_off, hi;
    uint64_t total_len = 0, prev_end = 0, start, end;
    uint64_t lines = 0;
    int pending = 0, pending_prev = 0;
    const char *label;
    int err;

    tls = __emutls_get_address(g_postbattle_tls_ctrl);
    tmpl_begin = (const uint8_t *)g_island_tmpl_begin;
    tls[0] = (uint64_t)(uintptr_t)g_stage_tramp_begin;
    *(uint32_t *)(tls + 1) = 0;
    tmpl_len = (uint64_t)((const uint8_t *)g_island_tmpl_end - tmpl_begin);
    if (g_trampoline_len < tmpl_len)
        return 0;
    base = -g_trampoline_len & site;
    movz_word = (uint32_t)(table_id << 5 | 0x52800002);  /* movz w2, #table_id */
    tls[0] = (uint64_t)(uintptr_t)g_stage_tramp_scan;
    off_d0 = (uint64_t)((const uint8_t *)g_tmpl_slot_d0 - tmpl_begin);
    off_d8 = (uint64_t)((const uint8_t *)g_tmpl_slot_d8 - tmpl_begin);
    off_e0 = (uint64_t)((const uint8_t *)g_tmpl_slot_e0 - tmpl_begin);
    off_e8 = (uint64_t)((const uint8_t *)g_tmpl_slot_e8 - tmpl_begin);

    /* --- pass 1: mmap hints walking outward from the site --- */
    for (step = 0x100000; step < 0x6f00001; step += 0x100000) {
        void *below = step <= base ? (void *)(uintptr_t)(base - step) : NULL;
        void *page;

        if (base + step != 0) {
            page = mmap((void *)(uintptr_t)(base + step),
                        (size_t)g_trampoline_len, 3 /* RW */,
                        0x22 /* MAP_PRIVATE|MAP_ANONYMOUS */, -1, 0);
            if (page == (void *)-1) {
                tls[0] = (uint64_t)(uintptr_t)g_stage_tramp_mmap;
                *(uint32_t *)(tls + 1) = (uint32_t)*__errno();
            } else {
                if (arm64_branch_encode(site, (uint64_t)(uintptr_t)page,
                                        &branch_enter) == 0) {
                    tls[0] = (uint64_t)(uintptr_t)"enter";
                    *(uint32_t *)(tls + 1) = 0;
                } else if (arm64_branch_encode(
                               (uint64_t)(uintptr_t)page + off_d0, site + 4,
                               &branch_resume) == 0) {
                    tls[0] = (uint64_t)(uintptr_t)"resume";
                    *(uint32_t *)(tls + 1) = 0;
                } else {
                    memcpy(page, tmpl_begin, (size_t)tmpl_len);
                    *(uint32_t *)((char *)page + off_d8) = movz_word;
                    *(uint32_t *)((char *)page + off_e0) = alloc_id;
                    *(uint32_t *)((char *)page + off_d0) = branch_resume;
                    *(void **)((char *)page + off_e8) = (void *)&ui_server_hook_target;
                    code_cache_flush_range((uint64_t)(uintptr_t)page,
                                           (uint64_t)(uintptr_t)page + tmpl_len);
                    if (mprotect(page, (size_t)g_trampoline_len,
                                 5 /* RX */) == 0) {
                        tls[0] = (uint64_t)(uintptr_t)"ready";
                        *(uint32_t *)(tls + 1) = 0;
                        return (uint64_t)(uintptr_t)page;
                    }
                    tls[0] = (uint64_t)(uintptr_t)g_stage_tramp_seal;
                    *(uint32_t *)(tls + 1) = (uint32_t)*__errno();
                }
                munmap(page, (size_t)g_trampoline_len);
            }
        }
        if (step < base) {
            page = mmap(below, (size_t)g_trampoline_len, 3 /* RW */,
                        0x22 /* MAP_PRIVATE|MAP_ANONYMOUS */, -1, 0);
            if (page == (void *)-1) {
                tls[0] = (uint64_t)(uintptr_t)g_stage_tramp_mmap;
                *(uint32_t *)(tls + 1) = (uint32_t)*__errno();
            } else {
                if (arm64_branch_encode(site, (uint64_t)(uintptr_t)page,
                                        &branch_enter) == 0) {
                    tls[0] = (uint64_t)(uintptr_t)"enter";
                    *(uint32_t *)(tls + 1) = 0;
                } else if (arm64_branch_encode(
                               (uint64_t)(uintptr_t)page + off_d0, site + 4,
                               &branch_resume) == 0) {
                    tls[0] = (uint64_t)(uintptr_t)"resume";
                    *(uint32_t *)(tls + 1) = 0;
                } else {
                    memcpy(page, tmpl_begin, (size_t)tmpl_len);
                    *(uint32_t *)((char *)page + off_d8) = movz_word;
                    *(uint32_t *)((char *)page + off_e0) = alloc_id;
                    *(uint32_t *)((char *)page + off_d0) = branch_resume;
                    *(void **)((char *)page + off_e8) = (void *)&ui_server_hook_target;
                    code_cache_flush_range((uint64_t)(uintptr_t)page,
                                           (uint64_t)(uintptr_t)page + tmpl_len);
                    if (mprotect(page, (size_t)g_trampoline_len,
                                 5 /* RX */) == 0) {
                        tls[0] = (uint64_t)(uintptr_t)"ready";
                        *(uint32_t *)(tls + 1) = 0;
                        return (uint64_t)(uintptr_t)page;
                    }
                    tls[0] = (uint64_t)(uintptr_t)g_stage_tramp_seal;
                    *(uint32_t *)(tls + 1) = (uint32_t)*__errno();
                }
                munmap(page, (size_t)g_trampoline_len);
            }
        }
    }

    /* --- pass 2: /proc/self/maps gap scan --- */
    memset(window.candidates, 0, 0x208);
    if (!((g_trampoline_len == 0x4000 || g_trampoline_len == 0x1000)
          && tmpl_len <= g_trampoline_len
          && off_e0 <= tmpl_len - 4 && off_d8 <= tmpl_len - 4
          && (off_d0 & 3) == 0 && off_d0 <= tmpl_len - 4 && 3 < off_d0
          && 0xb < tmpl_len))
        goto gap_window_fail;
    if (!((site & 3) == 0
          && off_e8 <= tmpl_len - 8
          && g_proc_mem_bias < 0xfffffffffec00000ULL
          && g_proc_mem_bias <= site
          && (site - g_proc_mem_bias) >> 0x16 < 5))
        goto gap_window_fail;

    lo = site - 0x8000000;
    if (lo < 0x10001)
        lo = 0x10000;
    base_off = (uint64_t)(uintptr_t)g_island_tmpl_begin + 0x8000004
             - (uint64_t)(uintptr_t)g_tmpl_slot_d0;
    hi = base_off + site;
    if (~(uint64_t)base_off < site)
        hi = 0xffffffffffffffffULL;   /* the raw overflow guard */
    if ((uint64_t)~g_trampoline_len <= hi)
        hi = ~g_trampoline_len;
    span = lo - 1;
    if (site >> 0x1b == 0)
        span = 0xffff;
    window.hi = hi & -g_trampoline_len;
    window.lo = (g_trampoline_len + span) & -g_trampoline_len;
    window.page = g_trampoline_len;
    if (window.lo > window.hi)
        goto gap_window_fail;

    f = fopen("/proc/self/maps", "r");
    if (f == NULL) {
        /* EINTR retry — the raw unrolls this 8 levels deep */
        int tries;

        for (tries = 0; tries < 8; tries++) {
            if (*__errno() != 4)
                break;
            f = fopen("/proc/self/maps", "r");
            if (f != NULL)
                goto scan;
        }
        err = *__errno();
        label = "gap_open";
        goto out_stage;
    }

scan:
    for (;;) {
        int ferr = 0, gerr = 0;
        int tries;
        size_t len, i, j;

        pending_prev = pending;
        *__errno() = 0;
        if (fgets(line, 0x2000, f) != NULL) {
            len = __strlen_chk(line, 0x2000);
            if (len != 0 && line[len - 1] == '\n')
                goto full_line;
            /* partial line: fall through to the read-error/EOF handling */
        } else {
            ferr = ferror(f);
            gerr = *__errno();
            if (ferr && gerr == 4) {
                for (tries = 0; tries < 8; tries++) {
                    clearerr(f);
                    *__errno() = 0;
                    if (fgets(line, 0x2000, f) != NULL) {
                        len = __strlen_chk(line, 0x2000);
                        if (len != 0 && line[len - 1] == '\n')
                            goto full_line;
                        break;
                    }
                    ferr = ferror(f);
                    gerr = *__errno();
                    if (!(ferr && gerr == 4))
                        break;
                }
            }
        }
        if (!(ferr && gerr == 4)) {
            if (!ferr) {
                if (feof(f))
                    goto eof_path;
                gerr = 0;
            }
        }
        err = gerr;
        label = "gap_read";
        goto maps_close;

    full_line:
        err = 0;
        lines = lines + 1;
        label = "gap_bounds";
        if (0x8000 < lines)
            goto maps_close;
        if (0x800000 - total_len < len)
            goto maps_close;
        total_len = total_len + len;

        /* first hex number: the range start */
        i = 0;
        start = 0;
        for (;;) {
            unsigned char c = (unsigned char)line[i];
            int sub;

            if (c - 0x30 < 10)
                sub = -0x30;
            else if (c - 0x61 <= 5)
                sub = -0x57;
            else if (c - 0x41 <= 5)
                sub = -0x37;
            else
                break;
            if (start >> 0x3c != 0) {
                err = 0;
                label = "gap_parse";
                goto maps_close;
            }
            start = start * 0x10 + c + sub;
            i = i + 1;
        }
        if (i == 0)
            goto maps_close;             /* label stays "gap_bounds" */
        label = "gap_parse";
        if (line[i] != '-')
            goto maps_close;

        /* second hex number: the range end */
        j = 0;
        end = 0;
        for (;;) {
            unsigned char c = (unsigned char)line[i + j + 1];
            int sub;

            if (c - 0x30 < 10)
                sub = -0x30;
            else if (c - 0x61 <= 5)
                sub = -0x57;
            else if (c - 0x41 <= 5)
                sub = -0x37;
            else
                break;
            if (end >> 0x3c != 0) {
                err = 0;
                label = "gap_parse";
                goto maps_close;
            }
            end = end * 0x10 + c + sub;
            j = j + 1;
        }
        if (j == 0 || line[i + j + 1] != ' ') {
            err = 0;
            label = "gap_parse";
            goto maps_close;
        }

        /* perms field: "rwxsp " style (each of r/w/x may be '-') */
        {
            const char *perms = line + i + j + 2;

            if (strlen(perms) < 5
                || (perms[0] != 'r' && perms[0] != '-')
                || !((perms[1] == 'w' || perms[1] == '-')
                     && (perms[2] == 'x' || perms[2] == '-'))
                || !((perms[3] == 's' || perms[3] == 'p')
                     && perms[4] == ' ')) {
                err = 0;
                label = "gap_parse";
                goto maps_close;
            }
        }

        /* well-formed readable range [start, end) */
        if (end <= start || start < prev_end) {
            err = 0;
            label = "gap_parse";
            goto maps_close;
        }
        if ((g_trampoline_len - 1) & (start | end)) {
            err = 0;
            label = "gap_parse";
            goto maps_close;
        }
        gap_span_update((uint64_t *)&window, site, prev_end, start);
        pending = pending_prev;
        if (site + 4 <= end)
            pending = 1;
        prev_end = end;
        if (site < start || 0xfffffffffffffffbULL < site)
            pending = pending_prev;
    }

eof_path:
    err = 0;
    label = "gap_maps";
    if (lines == 0 || !pending_prev)
        goto maps_close;
    gap_span_update((uint64_t *)&window, site, prev_end,
                    0xffffffffffffffffULL);
    if (fclose(f) != 0) {
        err = *__errno();
        label = "gap_close";
        goto out_stage;
    }
    tls[0] = (uint64_t)(uintptr_t)"gap_empty";
    *(uint32_t *)(tls + 1) = 0;
    if (window.count == 0)
        return 0;

    /* candidate loop: install into the first usable gap */
    {
        uint64_t remaining = window.count;
        uint64_t *cand_p = window.candidates;
        uint64_t cand;
        void *mapped;
        int ferr2;
        const char *fail_label;

        for (;;) {
            cand = *cand_p;

            /* validate the candidate (never inside the libg image) */
            if (cand < 0x10000
                || (g_trampoline_len - 1) & cand
                || 0xfffffffffffffffbULL < site
                || (uint64_t)~g_trampoline_len < cand
                || (cand < g_proc_mem_bias + 0x1400000
                    && g_proc_mem_bias < cand + g_trampoline_len)
                || arm64_branch_encode(site, cand, &branch_check) == 0) {
                tls[0] = (uint64_t)(uintptr_t)"gap_range";
                *(uint32_t *)(tls + 1) = 0;
                return 0;
            }
            /* resume-branch feasibility before touching the address space */
            if (arm64_branch_encode(cand + off_d0, site + 4,
                                    &branch_check) == 0) {
                tls[0] = (uint64_t)(uintptr_t)"gap_range";
                *(uint32_t *)(tls + 1) = 0;
                return 0;
            }
            mapped = mmap((void *)(uintptr_t)cand, (size_t)g_trampoline_len,
                          3 /* RW */, 0x100022 /* MAP_FIXED|PRIVATE|ANON */,
                          -1, 0);
            if (mapped == (void *)-1) {
                tls[0] = (uint64_t)(uintptr_t)"gap_mmap";
                *(uint32_t *)(tls + 1) = (uint32_t)*__errno();
            } else if (mapped != (void *)(uintptr_t)cand) {
                tls[0] = (uint64_t)(uintptr_t)"gap_return";
                *(uint32_t *)(tls + 1) = 0;
                if (munmap(mapped, (size_t)g_trampoline_len) != 0) {
                    tls[0] = (uint64_t)(uintptr_t)"gap_unmap";
                    *(uint32_t *)(tls + 1) = (uint32_t)*__errno();
                    return 0;
                }
            } else {
                if (arm64_branch_encode(site, (uint64_t)(uintptr_t)mapped,
                                        &branch_enter) == 0) {
                    ferr2 = 0;
                    fail_label = "enter";
                } else if (arm64_branch_encode(
                               (uint64_t)(uintptr_t)mapped + off_d0,
                               site + 4, &branch_resume) == 0) {
                    ferr2 = 0;
                    fail_label = "resume";
                } else {
                    memcpy(mapped, tmpl_begin, (size_t)tmpl_len);
                    *(uint32_t *)((char *)mapped + off_d8) = movz_word;
                    *(uint32_t *)((char *)mapped + off_e0) = alloc_id;
                    *(uint32_t *)((char *)mapped + off_d0) = branch_resume;
                    *(void **)((char *)mapped + off_e8) =
                        (void *)&ui_server_hook_target;
                    code_cache_flush_range((uint64_t)(uintptr_t)mapped,
                                           (uint64_t)(uintptr_t)mapped + tmpl_len);
                    if (mprotect(mapped, (size_t)g_trampoline_len,
                                 5 /* RX */) == 0) {
                        tls[0] = (uint64_t)(uintptr_t)"ready";
                        *(uint32_t *)(tls + 1) = 0;
                        return cand;
                    }
                    ferr2 = *__errno();
                    fail_label = "seal";
                }
                tls[0] = (uint64_t)(uintptr_t)fail_label;
                *(uint32_t *)(tls + 1) = (uint32_t)ferr2;
                if (munmap(mapped, (size_t)g_trampoline_len) != 0) {
                    tls[0] = (uint64_t)(uintptr_t)"gap_unmap";
                    *(uint32_t *)(tls + 1) = (uint32_t)*__errno();
                    return 0;
                }
            }
            remaining = remaining - 1;
            cand_p = cand_p + 1;
            if (remaining == 0)
                break;
        }
    }
    return 0;

maps_close:
    fclose(f);
    /* fall through */

out_stage:
    tls[0] = (uint64_t)(uintptr_t)label;
    *(uint32_t *)(tls + 1) = (uint32_t)err;
    return 0;

gap_window_fail:
    tls[0] = (uint64_t)(uintptr_t)"gap_window";
    *(uint32_t *)(tls + 1) = 0;
    return 0;
}
/* widgets chain 3 chunk 6: covers raw lines 2217-2334 (widget_probe_line_format; battle_end_state_tick @2335 is deferred whole to chunk 7) */

/* ===== results-screen tracker types (mirror of misc.c) =====
 * The tracker state block lives at 0x22d8f0 (0x1e8 bytes). The screen
 * record is the same 0x38-byte snapshot as the shared menu context — the
 * battle-end tick fills it with menu_context_init and the misc.c tracker
 * feeds read it as a screen_record_t. */

typedef struct {
    uint64_t stamp;                /* +0x00 screen stamp (refresh ms) */
    uint64_t identity;             /* +0x08 screen identity (context generation) */
    uint64_t node;                 /* +0x10 screen node (stage view) */
    uint64_t flag;                 /* +0x18 pending-event flag (cached widget obj) */
    int32_t  count_a;              /* +0x20 primary count (autofarm verdict) */
    int32_t  count_a_hi;           /* +0x24 evasion-armed flag */
    int32_t  count_b;              /* +0x28 secondary count (autofarm gated) */
    int32_t  count_b_hi;           /* +0x2c stage byte */
    uint32_t close_window_again;   /* +0x30 min age of the play-again capture,
                                    * clamped to [1000, 10000] (setting 0x62) */
    uint32_t close_window_refresh; /* +0x34 min age of the refresh capture,
                                    * clamped to [500, 5000] (setting 99) */
} screen_record_t;                 /* 0x38 bytes */

typedef struct {
    uint64_t subject;              /* +0x00 widget the record was captured from */
    uint64_t word1;                /* +0x08 matched against the screen node */
    uint64_t word2;                /* +0x10 */
    uint64_t word3;                /* +0x18 */
    uint64_t word4;                /* +0x20 */
    uint64_t word5;                /* +0x28 */
    uint64_t identity;             /* +0x30 screen identity (stamped at capture) */
    uint64_t stamp;                /* +0x38 screen stamp (stamped at capture) */
    int32_t  mode;                 /* +0x40 capture class: 1 play-again, 2 refresh */
    uint32_t mode_hi;              /* +0x44 */
    uint64_t flag;                 /* +0x48 */
} tracker_record_t;                /* 0x50 bytes */

typedef struct {
    screen_record_t rec;           /* +0x000 adopted screen identity */
    tracker_record_t again;        /* +0x038 "PLAY AGAIN" capture */
    tracker_record_t refresh;      /* +0x088 refresh-mode capture */
    uint64_t last_event_node;      /* +0x0d8 node the last text event came from */
    uint64_t close_stamp;          /* +0x0e0 screen-close base time */
    uint64_t first_seen;           /* +0x0e8 first event stamp on this screen */
    uint64_t outcome_stamp;        /* +0x0f0 stamp of the last outcome change */
    uint64_t delta_first;          /* +0x0f8 first delta-note stamp */
    uint64_t refresh_base;         /* +0x100 staleness base for the refresh capture */
    uint64_t again_anchor;         /* +0x108 play-again anchor stamp */
    uint64_t refresh_anchor;       /* +0x110 refresh anchor stamp */
    uint64_t last_seen;            /* +0x118 last event stamp */
    uint64_t outcome_last;         /* +0x120 last outcome-class stamp */
    uint64_t delta_last;           /* +0x128 last delta-note stamp */
    uint64_t screen_changes;       /* +0x130 identity-change counter ("BATTLES") */
    uint64_t tally_count_hi;       /* +0x138 net win tally ("W") */
    uint64_t tally_count_lo;       /* +0x140 net loss tally ("L") */
    uint64_t tally_count_two;      /* +0x148 net other tally ("D") */
    uint64_t tally_identity;       /* +0x150 identity the tallies belong to */
    uint64_t delta_sum;            /* +0x158 applied delta sum ("TROPHIES") */
    int32_t  outcome;              /* +0x160 outcome class (1 win, -1 loss, 2 other) */
    int32_t  delta_value;          /* +0x164 pending delta-note value */
    int32_t  rank_value;           /* +0x168 last parsed rank/placement value */
    int32_t  outcome_pending;      /* +0x16c outcome being transitioned to */
    int32_t  delta_applied_value;  /* +0x170 delta value already summed in */
    int32_t  delta_latch;          /* +0x174 a delta value has been seen */
    int32_t  delta_applied;        /* +0x178 delta_applied_value is live */
    int32_t  note_latch;           /* +0x17c external delta-note was taken */
    int32_t  pending_valid;        /* +0x180 outcome_pending is set */
    int32_t  tally_active;         /* +0x184 tallies started for this screen */
    int32_t  refresh_active;       /* +0x188 refresh capture is live */
    int32_t  refresh_mode;         /* +0x18c refresh mode (1 event, 2 captured) */
    int32_t  archived;             /* +0x190 screen was archived */
    int32_t  adopted;              /* +0x194 tracker adopted this screen */
    tracker_record_t archive;      /* +0x198 archived capture */
} results_tracker_t;               /* 0x1e8 bytes */

/* ===== results tracker block storage (0x22d8f0..0x22dad8) =====
 * The named globals below are the misc.c join keys into the single
 * results_tracker_t the raw lays over the same addresses (Ghidra's
 * overlapping-symbol warning); the overlay macro gives the new code
 * field-level access. Chunk 1 already owns 0x22d8f8/0x22d900/0x22da74/
 * 0x22da7c (g_plus_obs_90 / g_plus_obs_a0 / g_plus_obs_armed /
 * g_plus_obs_word68 = rec.identity / rec.node / {tally_active,
 * refresh_active} / {refresh_mode, archived}). */

uint64_t g_plus_obs_98;         /* 0x22d8f0 — misc.c: base alias — the tracker head (rec.stamp) */
uint64_t g_plus_obs_widget;     /* 0x22d908 — cached widget object (rec.flag) */
uint64_t g_endcap_node;         /* 0x22d928 — misc.c: captured screen node (again.subject) */
uint64_t g_endcap_identity;     /* 0x22d930 — misc.c: captured screen identity (again.word1) */
int64_t  g_endcap_rec[10];      /* 0x22d938 — misc.c: captured end-screen record (again.word2..) */
uint64_t g_endcap_word50;       /* 0x22d970 — misc.c: captured +0x50 word (again.flag) */
uint64_t g_endcap_word58;       /* 0x22d978 — misc.c: captured +0x58 word (refresh.subject) */
uint64_t g_plus_obs_identity;   /* 0x22d9c8 — misc.c: observed screen identity (last_event_node) */
uint64_t g_plus_obs_word10;     /* 0x22d9d8 — misc.c: first observed node (first_seen) */
uint64_t g_plus_obs_last;       /* 0x22d9e8 — misc.c: last distinct node (delta_first) */
uint64_t g_plus_obs_stamp;      /* 0x22da08 — misc.c: observed screen stamp (last_seen) */
uint64_t g_plus_obs_stamp2;     /* 0x22da18 — misc.c: stamp echo (delta_last) */
uint64_t g_plus_obs_field10;    /* 0x22da50 — misc.c: observed +0xb0 field (outcome) */
uint64_t g_plus_obs_valid_a;    /* 0x22da64 — misc.c: observation valid a (delta_latch) */
uint64_t g_plus_obs_valid_b;    /* 0x22da6c — misc.c: observation valid b (note_latch) */
uint64_t g_plus_obs_gate;       /* 0x22da84 — misc.c: observation gate (adopted) */
uint64_t g_guard_stamp;         /* 0x22f1c8 — guard-triple publish stamp (refresh ms) */

#define g_results_tracker \
    (*(results_tracker_t *)(void *)&g_plus_obs_98)

/* battle-stats line latch (0x2815c0 triple, just before the tracked block);
 * the raw spells the armed flag as a byte store into 0x2815d0 — the same
 * qword misc.c names g_view_node_cache */
uint64_t g_stats_stamp;          /* 0x2815c0 — battle-stats line stamp */
uint32_t g_stats_interval;       /* 0x2815c8 — battle-stats line interval (50/250) */
uint64_t g_view_node_cache;      /* 0x2815d0 — misc.c: last seen node; the low
                                  * byte doubles as the stats-line latch */

/* kind-3 lineage sweep cache (8 slots, just past the tracked block) */
uint64_t g_track_poll_next;      /* 0x281910 — next lineage-sweep poll (ms) */
uint64_t g_track_lineage_slots[8][4];  /* 0x281918 — per-slot {node, expected
                                        * +0x248 word, stop node, spare} */

/* postbattle probe status line (the 0x25c9dc tail of the 0x25c9d8 blob;
 * misc.c declares it const — the writes go through the formatter below) */
char g_status_postbattle_failed[0x38];  /* 0x25c9dc */

/* ===== plus-service accessor (raw layout) =====
 * Note: the raw seeds the leading pair as {0, libg base} and puts the
 * engine fn at libg+0xac319c in the LAST slot; the pushed misc.c types
 * the same block as {base, ctx, read, alloc, init, register_fn} with the
 * default accessor swapping the 0xac319c/0x10fa30/38 roles — flagged for
 * the coordinator. */
typedef struct {
    uint64_t ctx;                 /* +0x00 (0) */
    uint64_t base;                /* +0x08 libg load base */
    int (*read)(void *ctx, uint64_t addr, void *out, uint32_t len);  /* +0x10 */
    void *(*alloc)(uint64_t size);                                    /* +0x18 */
    void (*init)(void *obj, int a, int b);                            /* +0x20 */
    void (*register_fn)(uint64_t service, void *obj);                 /* +0x28 */
} plus_accessor_raw_t;

/* ===== cross-file functions (misc.c / menu_engine.c / renderer.c) ===== */

extern int str_format(char *buf, size_t cap, size_t slen,
                      const char *fmt, ...);               /* misc.c @ 0015540c */
extern const char *strategy_name_normalize(const char *name);  /* misc.c @ 00168d18 */
extern void failure_reason_format(char *out, uint64_t code);   /* misc.c @ 00168dfc */
extern int code_region_hash_verify(uint64_t offset, size_t len,
                                   const char *expect_hex);   /* misc.c @ 0014d168 */
extern uint64_t script_port_fast_replay_claim(uint64_t a, uint64_t b,
                                              uint32_t c);   /* misc.c @ 00192178 */
extern int button_widget_resolve(uint64_t node, uint64_t root,
                                 uint64_t *out);         /* misc.c @ 00165944 */
extern int plus_service_object_create(void *accessor);       /* misc.c @ 00165768 */
extern int64_t plus_service_object_create_default(void);
                                      /* misc.c @ 001658c4 — typed void there;
                                       * the raw consumes the create verdict */
extern uint64_t remote_read_ptr(uint64_t addr);
                                      /* misc.c @ 00154cf0 — one-arg form (the
                                       * raw reads the value straight back) */
extern int remote_child_link_valid(uint64_t node, uint64_t parent);
                                      /* misc.c @ 00154d5c */
extern int remote_widget_ancestor_of(uint64_t node, uint64_t target);
                                      /* misc.c @ 00155084 — the raw passes the
                                       * implicit node pair (dump reads at addr) */
extern int results_tracker_screen_archive(results_tracker_t *trk,
                                         const screen_record_t *rec,
                                         tracker_record_t *out);  /* misc.c @ 0014e030 */
extern int service_object_acquire(const screen_record_t *rec,
                                  service_record_t *out);    /* misc.c @ 0014f434 */
extern int captured_widget_check(const screen_record_t *rec); /* misc.c @ 0014f4dc */
extern int results_record_still_valid(const int64_t *rec);    /* misc.c @ 0016563c */
extern void screen_text_feed(const screen_record_t *rec, uint64_t node,
                             const char *text);               /* misc.c @ 00165224 */
extern void result_text_feed(const screen_record_t *rec, uint64_t node,
                             const char *text);               /* misc.c @ 00165360 */
extern int remote_string_read(uint64_t obj, void *out);       /* misc.c @ 0016515c */
extern void screen_scan_pump(screen_record_t *rec);           /* misc.c @ 00164978 */
extern int nexus_menu_ui_state(void *state_out, int tid);     /* menu_engine.c @ 00183d70 */
extern uint64_t nexus_menu_setting_value(uint32_t id,
                                         int32_t *value_out); /* menu_engine.c @ 001841f0 */
extern int ui_named_state_query(const char *name, void *out); /* this file @ 00192340
                                                  * (d-chain chunk: 0x18-byte record) */
extern void rich_toast_set(const char *text, int show);       /* renderer.c @ 0016d020 */
extern uint64_t g_battle_state;      /* 0x22d880 — misc.c: battle/screen state word */
extern int32_t g_dialog_pending_tid; /* 0x22f03c — misc.c: dialog bind worker tid */
extern uint32_t g_stage_log_count;   /* 0x22f4bc — menu_engine.c: capped-128 stage log */
extern uint64_t g_capture_subject;   /* 0x22db40 — misc.c: widget the pair was captured from */
extern uint64_t g_capture_pair_a;    /* 0x22db48 — misc.c: captured pair slot a */
extern uint64_t g_capture_pair_b;    /* 0x22db50 — misc.c: captured pair slot b */
extern uint64_t g_capture_flags;     /* 0x22db58 — misc.c: hi dword 0x22db5c = slot
                                      * live bits {a = bit0, b = bit1} */
extern uint64_t g_overlay_parent;    /* 0x22db60 — misc.c: parent widget */
extern uint64_t g_tracked_node;      /* 0x2815d8 — misc.c: tracked screen node (block head) */

/* ===== rodata ===== */

extern const uint64_t PLUS_SVC_ALLOC_RVA;  /* 0x10fa30 — plus-service alloc fn rva */
extern const uint64_t PLUS_SVC_INIT_RVA;   /* 0x10fa38 — plus-service init fn rva */

/* ===== postbattle island site offsets (misc.c postbattle_install) ===== */

#define ISLAND_SITE_WIDGET_TEXT  0x772870u
#define ISLAND_SITE_TROPHY_TEXT  0x5921a0u
#define ISLAND_SITE_DIALOG_HOOK  0x88836cu
#define ISLAND_SITE_UI_SERVER    0x678fe4u

/*
 * site_offset_resolve — map an absolute hook-site address back to its
 * module-relative island offset (0x772870 / 0x5921a0 / 0x88836c /
 * 0x678fe4), each compare guarded against base+offset wrapping (the raw's
 * 0xffffffffff… sentinels). Returns 0 when the site is none of the four.
 */
static uint64_t site_offset_resolve(uint64_t base, uint64_t site)
{
    if (base <= 0xffffffffff88d78fULL && base + ISLAND_SITE_WIDGET_TEXT == site)
        return ISLAND_SITE_WIDGET_TEXT;
    if (base <= 0xffffffffffa6de5fULL && base + ISLAND_SITE_TROPHY_TEXT == site)
        return ISLAND_SITE_TROPHY_TEXT;
    if (base <= 0xffffffffff777c93ULL && base + ISLAND_SITE_DIALOG_HOOK == site)
        return ISLAND_SITE_DIALOG_HOOK;
    if (base <= 0xffffffffff98701bULL && base + ISLAND_SITE_UI_SERVER == site)
        return ISLAND_SITE_UI_SERVER;
    return 0;
}

/* ===== tracker reset helpers (the raw's zero sweeps) ===== */

/*
 * tracker_reset_full — the dead-context sweep: both captures, the event
 * stamps, the outcome/delta state, the latches and the archive record.
 * The adopted screen record, close_stamp, the tallies (BATTLES/W/L/D),
 * the trophy sum and delta_applied_value survive.
 */
static void tracker_reset_full(void)
{
    memset(&g_results_tracker.again, 0, sizeof g_results_tracker.again);
    memset(&g_results_tracker.refresh, 0, sizeof g_results_tracker.refresh);
    g_results_tracker.last_event_node = 0;
    /* 0x22d9d0 (close_stamp) is deliberately preserved */
    memset(&g_results_tracker.first_seen, 0, 9 * sizeof(uint64_t));
    g_results_tracker.outcome = 0;
    g_results_tracker.delta_value = 0;
    g_results_tracker.rank_value = 0;
    g_results_tracker.outcome_pending = 0;
    g_results_tracker.delta_latch = 0;
    g_results_tracker.delta_applied = 0;
    g_results_tracker.note_latch = 0;
    g_results_tracker.pending_valid = 0;
    g_results_tracker.tally_active = 0;
    g_results_tracker.refresh_active = 0;
    g_results_tracker.refresh_mode = 0;
    g_results_tracker.archived = 0;
    g_results_tracker.adopted = 0;
    memset(&g_results_tracker.archive, 0, sizeof g_results_tracker.archive);
}

/*
 * tracker_reset_captures — the stale-identity sweep: like the full sweep
 * but the tracker stays adopted (0x22da84 survives).
 */
static void tracker_reset_captures(void)
{
    memset(&g_results_tracker.again, 0, sizeof g_results_tracker.again);
    memset(&g_results_tracker.refresh, 0, sizeof g_results_tracker.refresh);
    g_results_tracker.last_event_node = 0;
    memset(&g_results_tracker.first_seen, 0, 9 * sizeof(uint64_t));
    g_results_tracker.outcome = 0;
    g_results_tracker.delta_value = 0;
    g_results_tracker.rank_value = 0;
    g_results_tracker.outcome_pending = 0;
    g_results_tracker.delta_latch = 0;
    g_results_tracker.delta_applied = 0;
    g_results_tracker.note_latch = 0;
    g_results_tracker.pending_valid = 0;
    g_results_tracker.tally_active = 0;
    g_results_tracker.refresh_active = 0;
    g_results_tracker.refresh_mode = 0;
    g_results_tracker.archived = 0;
    memset(&g_results_tracker.archive, 0, sizeof g_results_tracker.archive);
}

/*
 * tracker_archive_drop — the tail sweep: clear the archived flag and the
 * archive record (the raw's rearm tail dword runs).
 */
static void tracker_archive_drop(void)
{
    g_results_tracker.archived = 0;
    memset(&g_results_tracker.archive, 0, sizeof g_results_tracker.archive);
}

/* ===== postbattle probe line formatter ===== */

/* 0x58-byte probe record (the misc.c postbattle failure feeds) */
typedef struct {
    uint32_t stage;        /* +0x00 island stage index (<= 3) */
    uint32_t _pad04;       /* +0x04 */
    uint64_t site;         /* +0x08 absolute hook-site address */
    const char *kind;      /* +0x10 "write" / "read" / "mismatch" */
    uint32_t guard_word;   /* +0x18 guard word (the mismatch line's %%08x) */
    uint32_t _pad1c;       /* +0x1c */
    const char *mem_kind;  /* +0x20 "proc" / "vm" */
    const char *expect;    /* +0x28 expected direction ("write"/"read") */
    uint64_t strategy;     /* +0x30 allocator strategy */
    uint64_t direction;    /* +0x38 direction code (must be 4) */
    uint64_t fail_code;    /* +0x40 failure code */
    int64_t  size;         /* +0x48 transfer size */
    uint32_t val_a;        /* +0x50 */
    uint32_t val_b;        /* +0x54 */
} probe_line_rec_t;

/*
 * widget_probe_line_format — render one postbattle probe record into the
 * status blob at 0x25c9dc. The site is mapped back to its island offset;
 * unknown sites, stage > 3, unknown kinds, a missing/mismatched expected
 * direction or a direction code != 4 render the context-unknown line.
 * Mismatch records render the short guard line; everything else renders
 * the full probe line with the failure reason, the normalized strategy
 * and the size reason (alloc/remap strategies with a non-negative size
 * render "other"). The two trailing counters are clamped to [1, 0xfff]
 * (anything else becomes 0). @ 0015cd20
 */
void widget_probe_line_format(uint64_t base, const probe_line_rec_t *rec)
{
    uint64_t off;
    char kind_ch, mem_ch;
    const char *kind_str;
    const char *strategy;
    char fail_text[8];
    char size_text[8];
    uint32_t val_a, val_b;

    off = site_offset_resolve(base, rec->site);

    kind_ch = 'u';   /* NULL or an unknown kind string */
    if (rec->kind != NULL) {
        if (strcmp(rec->kind, "write") == 0)
            kind_ch = 'w';
        else if (strcmp(rec->kind, "read") == 0)
            kind_ch = 'r';
        else if (strcmp(rec->kind, "mismatch") == 0)
            kind_ch = 'm';
    }

    mem_ch = 'u';   /* NULL or an unknown memory kind */
    if (rec->mem_kind != NULL) {
        if (strcmp(rec->mem_kind, "proc") == 0)
            mem_ch = 'p';
        else if (strcmp(rec->mem_kind, "vm") == 0)
            mem_ch = 'v';
    }

    kind_str = kind_ch == 'w' ? "write" : "read";

    if (off == 0 || 3 < rec->stage
        || kind_ch == 'u'
        || rec->expect == NULL || strcmp(rec->expect, kind_str) != 0
        || rec->direction != 4) {
        str_format(g_status_postbattle_failed, 0x37, 0x37,
                   "pb_%lx_%c_context_unknown", off, kind_ch);
        return;
    }

    if (kind_ch == 'm') {
        str_format(g_status_postbattle_failed, 0x37, 0x37,
                   "pb_%lx_m_%c_g%08x", off, mem_ch, rec->guard_word);
        return;
    }

    strategy = strategy_name_normalize((const char *)(intptr_t)rec->strategy);
    failure_reason_format(fail_text, rec->fail_code);
    if (strcmp(strategy, "alloc") == 0 || strcmp(strategy, "remap") == 0) {
        if (rec->size < 0)
            failure_reason_format(size_text, (uint64_t)rec->size);
        else
            str_format(size_text, 8, 8, "other");
    } else {
        failure_reason_format(size_text, (uint64_t)rec->size);
    }

    val_a = rec->val_a;
    val_b = rec->val_b;
    if (0xffe < val_a - 1)
        val_a = 0;
    if (0xffe < val_b - 1)
        val_b = 0;

    str_format(g_status_postbattle_failed, 0x37, 0x37,
               "pb_%lx_%c_%c_%s_e%u_%s_%s_e%u",
               off, kind_ch, mem_ch, fail_text, val_a, strategy, size_text,
               val_b);
}
/* widgets chain 3 chunk 7: covers raw lines 2335-3231 (battle_end_state_tick, whole) */

/* forward declaration within this chain */
void overlay_pair_state_pump(uint64_t flags, menu_context_t *ctx,
                             int menu_open);              /* below @ 00165ab8 */

/*
 * battle_end_state_tick — the battle-end overlay pump. kind 1 is a click
 * on `node`, kind 2 a text event, kind 0 a plain pump and kind 3 the
 * per-frame tick (50 ms lineage sweep / observation poll). Only the
 * Mainloop thread runs (owning tid + thread name), with the postbattle
 * islands installed and the dialog worker live.
 *
 * Per tick: refresh the shared menu context (== the tracker's screen
 * record), adopt or reset the results tracker when the observation is not
 * archived, sweep the eight lineage slots and the captured hook node
 * (vtable + +0x248 word + ancestor chain must hold), refresh the
 * guard-triple identity, claim/publish fast-replay pairs into the guard
 * triple, create the plus-service object on the observation interval
 * (setting 99 clamped to [500, 5000] ms), route the two battle-end
 * feature clicks through battle_end_feature_arm (capped-128
 * "script_port_result_click" stage log), dispatch the armed action (mode
 * 1: plus-service create; mode 2: leave-battle through libg+0x91aae0
 * after the 0x91aae0/0x1dc digest check), publish the BATTLES/W/L/D/
 * TROPHIES toast line (50/250 ms latch) and feed screen texts into the
 * tracker. kind 3 also archives a finished screen and dispatches its
 * record (word5(word4, subject)) with the "postbattle_dispatch" log.
 * @ 0015f334
 */
void battle_end_state_tick(uint32_t kind, uint64_t node, uint64_t text_obj,
                           uint64_t now_ms)
{
    menu_context_t ctx;
    menu_context_t fresh;
    service_record_t replay;
    service_record_t replay2;
    tracker_record_t track;
    plus_accessor_raw_t acc;
    uint64_t resolved[3];       /* button_widget_resolve record */
    uint8_t ui_state[8];
    uint32_t q_rec[6];          /* ui_named_state_query 0x18-byte record */
    char text[0x100];
    char thread_name[0x10];
    struct timespec now;
    uint64_t word, word248, end_node, pair_a, pair_b;
    uint64_t interval, ms, setting;
    uint32_t mask, action, verdict, mode;
    const char *feature, *reason;
    bool fast_visible, any_visible, menu_open, state_ok;
    bool guard_stale, interval_ok, subject_ok, claimed;
    bool vtable_ok;
    int owning_tid = (int)g_launcher_menu_id;
    int r1, r2, created, hops, i;

    if (g_postbattle_installed != 1)
        return;
    if (g_launcher_menu_id < 1 || gettid() != owning_tid)
        return;
    memset(thread_name, 0, sizeof thread_name);
    if (pthread_getname_np(pthread_self(), thread_name,
                           sizeof thread_name) != 0)
        return;
    if (memcmp(thread_name, "Mainloop", 8) != 0 /* 0x706f6f6c6e69614d */
        || thread_name[8] != '\0'
        || g_dialog_pending_tid < 1)
        return;

    menu_context_init(&ctx, now_ms);

    /* --- tracker adoption / reset (only while not archived) --- */
    if (g_results_tracker.archived == 0) {
        if (ctx.autofarm_enabled == 0
            || (ctx.evasion_armed == 0 && ctx.autofarm_gated == 0)) {
            tracker_reset_full();
        } else {
            if (g_results_tracker.adopted == 0
                || ctx.refresh_ms < (uint64_t)g_results_tracker.rec.stamp
                || ctx.stage_view != g_results_tracker.rec.node
                || ctx.generation
                       != (uint64_t)g_results_tracker.rec.identity) {
                tracker_reset_captures();
            }
            g_results_tracker.adopted = 1;
            if (ctx.widget_obj != 0 && ctx.generation != 0) {
                g_results_tracker.close_stamp = ctx.refresh_ms;
                g_results_tracker.last_event_node = ctx.generation;
            }
        }
        /* publish the fresh context as the tracker's screen record */
        g_results_tracker.rec.count_b = ctx.autofarm_gated;
        g_results_tracker.rec.count_b_hi = ctx.stage_byte;
        g_results_tracker.rec.count_a = ctx.evasion_armed;
        g_results_tracker.rec.count_a_hi = ctx.autofarm_enabled;
        g_results_tracker.rec.close_window_again = ctx.limit_0x62;
        g_results_tracker.rec.close_window_refresh = ctx.limit_99;
        g_results_tracker.rec.identity = ctx.generation;
        g_results_tracker.rec.stamp = ctx.refresh_ms;
        g_results_tracker.rec.flag = ctx.widget_obj;
        g_results_tracker.rec.node = ctx.stage_view;
    }

    /* --- kind 3: sweep the eight lineage slots --- */
    if (kind == 3) {
        for (i = 0; i < 8; i++) {
            uint64_t slot_node = g_track_lineage_slots[i][0];

            if (slot_node == 0)
                continue;
            word = 0;
            r1 = proc_mem_read(NULL, slot_node, &word, 8);
            vtable_ok = word == (uint64_t)g_proc_mem_bias
                              + 0x11c7b88;   /* widget-class vtable */
            r2 = proc_mem_read(NULL, slot_node + 0x248, &word, 8);
            if ((r2 == 0 || word == g_track_lineage_slots[i][1])
                && (r1 == 0 || vtable_ok)) {
                uint64_t cur = slot_node;

                hops = 0x20;
                do {
                    if (cur == ctx.stage_view
                        || cur == g_track_lineage_slots[i][2])
                        break;
                    if (proc_mem_read(NULL, cur + 0x38, &word, 8) == 0)
                        break;
                    if (word == 0)
                        goto invalidate_slot;
                    hops = hops - 1;
                    cur = word;
                } while (hops != 0);
            } else {
invalidate_slot:
                g_track_lineage_slots[i][3] = 0;
                g_track_lineage_slots[i][2] = 0;
                g_track_lineage_slots[i][1] = 0;
                g_track_lineage_slots[i][0] = 0;
            }
        }
    }

    /* --- sweep the captured hook node --- */
    word = g_hook_module_handle;
    if (g_hook_module_handle != 0) {
        uint64_t cur = word;
        uint64_t v = 0;

        r1 = proc_mem_read(NULL, g_hook_module_handle, &v, 8);
        vtable_ok = v == (uint64_t)g_proc_mem_bias + 0x11c7b88;
        r2 = proc_mem_read(NULL, g_hook_module_handle + 0x248, &v, 8);
        if ((v == g_hook_last_b || r2 == 0) && (r1 == 0 || vtable_ok)) {
            hops = 0x20;
            do {
                if (cur == ctx.stage_view || cur == g_hook_last_a)
                    break;
                if (proc_mem_read(NULL, cur + 0x38, &word, 8) == 0)
                    break;
                if (word == 0)
                    goto invalidate_hook;
                hops = hops - 1;
                cur = word;
            } while (hops != 0);
        } else {
invalidate_hook:
            g_hook_inflight = 0;
            g_hook_last_a = 0;
            g_hook_module_handle = 0;
            g_hook_last_c = 0;
            g_hook_last_b = 0;
        }
    }

    /* --- guard-triple staleness --- */
    if (g_guard_last_c == 0) {
        guard_stale = true;
    } else if (g_guard_last_a == ctx.stage_view
               && g_guard_last_b == ctx.generation) {
        guard_stale = false;
    } else {
        guard_stale = true;
        g_guard_last_b = 0;
        g_guard_last_a = 0;
        g_guard_stamp = 0;
        g_guard_last_c = 0;
    }

    /* --- evasion-armed: publish a fresh fast-replay pair as the guard --- */
    if (ctx.evasion_armed != 0) {
        memset(&replay, 0, sizeof replay);
        replay.header = SERVICE_RECORD_HEADER;
        if (ctx.autofarm_enabled != 0 && ctx.widget_obj != 0
            && (guard_stale || (g_guard_last_a != ctx.stage_view
                                || g_guard_last_b != ctx.generation))
            && script_port_fast_replay_snapshot(
                   (uint64_t)(uintptr_t)&replay) != 0
            && replay.field_10 != 0 && replay.field_18 != 0
            && replay.ok_a == 0 && replay.ok_b != 0) {
            tracker_archive_drop();
            g_guard_last_b = ctx.generation;
            g_guard_last_a = ctx.stage_view;
            g_guard_last_c = replay.field_18;
            g_guard_stamp = ctx.refresh_ms;
            g_pair_release_epoch = 0;
            g_results_tracker.archived = 0;
            g_results_tracker.refresh_active = 1;
            g_results_tracker.refresh_mode = 1;
            g_results_tracker.refresh_base = ctx.refresh_ms;
        }
    }

    /* --- observation-gated claim (refresh capture live for this screen) --- */
    if (g_results_tracker.refresh_active != 0
        && g_results_tracker.refresh_mode == 1
        && g_results_tracker.rec.node == ctx.stage_view
        && g_results_tracker.rec.identity == ctx.generation) {
        memset(&replay, 0, sizeof replay);
        replay.header = SERVICE_RECORD_HEADER;
        if (ctx.autofarm_enabled != 0 && ctx.widget_obj != 0
            && (g_guard_last_c == 0
                || (g_guard_last_a != ctx.stage_view
                    || g_guard_last_b != ctx.generation))
            && script_port_fast_replay_snapshot(
                   (uint64_t)(uintptr_t)&replay) != 0
            && replay.field_10 != 0 && replay.field_18 != 0
            && replay.ok_a != 0
            && script_port_fast_replay_claim(replay.field_10,
                                             replay.field_18, 1) != 0) {
            g_guard_last_c = replay.field_18;
            g_guard_stamp = ctx.refresh_ms;
            g_guard_last_b = ctx.generation;
            g_guard_last_a = ctx.stage_view;
        }
    }

    /* --- observation interval (setting 99, clamped [500, 5000] ms) --- */
    if (4999 < ctx.limit_99)
        ctx.limit_99 = 5000;
    if (ctx.limit_99 < 0x1f5)
        ctx.limit_99 = 500;
    if (g_results_tracker.refresh_base == 0
        || ctx.refresh_ms < g_results_tracker.refresh_base)
        interval_ok = true;
    else
        interval_ok = (uint64_t)ctx.limit_99
                      <= ctx.refresh_ms - g_results_tracker.refresh_base;

    if (ctx.stage_byte == 0) {
        if (ctx.evasion_armed == 0) {
            if (ctx.autofarm_gated != 0 && interval_ok)
                goto observe;
        } else if (interval_ok) {
observe:
            memset(&replay, 0, sizeof replay);
            replay.header = SERVICE_RECORD_HEADER;
            if (ctx.autofarm_enabled != 0 && ctx.widget_obj != 0
                && (g_guard_last_c == 0
                    || (g_guard_last_a != ctx.stage_view
                        || g_guard_last_b != ctx.generation))
                && script_port_fast_replay_snapshot(
                       (uint64_t)(uintptr_t)&replay) != 0
                && replay.field_10 != 0 && replay.field_18 != 0
                && replay.ok_a != 0 && replay.ok_b != 0
                && script_port_fast_replay_claim(replay.field_10,
                                                 replay.field_18, 1) != 0) {
                g_results_tracker.refresh_base = ctx.refresh_ms;

                /* the default plus-service accessor (raw layout) */
                acc.ctx = 0;
                acc.base = (uint64_t)g_proc_mem_bias;
                acc.read = proc_mem_read;
                acc.alloc = (void *(*)(uint64_t))(
                    (uint64_t)g_proc_mem_bias + PLUS_SVC_ALLOC_RVA);
                acc.init = (void (*)(void *, int, int))(
                    (uint64_t)g_proc_mem_bias + PLUS_SVC_INIT_RVA);
                acc.register_fn = (void (*)(uint64_t, void *))(
                    (uint64_t)g_proc_mem_bias + 0xac319c);
                created = plus_service_object_create(&acc);
                if (created != 0) {
                    g_guard_last_c = replay.field_18;
                    g_guard_stamp = ctx.refresh_ms;
                    g_guard_last_b = ctx.generation;
                    g_guard_last_a = ctx.stage_view;
                    tracker_archive_drop();
                    g_pair_release_epoch = 0;
                    g_results_tracker.refresh_active = 1;
                    g_results_tracker.refresh_mode = 1;
                    return;
                }
                script_port_fast_replay_claim(replay.field_10,
                                              replay.field_18, 0);
            }
        }
    }

    /* --- kind 1: click on the captured hook node arms the feature --- */
    if (kind == 1 && ctx.autofarm_enabled != 0 && g_hook_module_handle != 0
        && ctx.stage_view == g_hook_last_a
        && ctx.generation == g_hook_last_c
        && button_lineage_check(g_hook_module_handle, ctx.stage_view) != 0) {
        word248 = 0;
        r1 = proc_mem_read(NULL, g_hook_module_handle + 0x248, &word248, 8);
        if (!is_plausible_ptr_e(word248) || r1 == 0)
            word248 = 0;
        if (word248 == g_hook_last_b
            && button_widget_resolve(g_hook_module_handle, ctx.stage_view,
                                     resolved) != 0
            && resolved[0] == node) {
            memset(&replay, 0, sizeof replay);
            replay.header = SERVICE_RECORD_HEADER;
            g_pair_release_epoch = 0;
            g_hook_inflight = (g_hook_inflight & ~0xffffffffu) | 1;
            if (ctx.autofarm_enabled != 0 && ctx.widget_obj != 0
                && (g_guard_last_c == 0
                    || (g_guard_last_a != ctx.stage_view
                        || g_guard_last_b != ctx.generation))
                && script_port_fast_replay_snapshot(
                       (uint64_t)(uintptr_t)&replay) != 0
                && replay.field_10 != 0 && replay.field_18 != 0
                && replay.ok_a != 0
                && script_port_fast_replay_claim(replay.field_10,
                                                 replay.field_18, 1) != 0) {
                g_guard_last_c = replay.field_18;
                g_guard_stamp = ctx.refresh_ms;
                g_guard_last_b = ctx.generation;
                g_guard_last_a = ctx.stage_view;
            }
            tracker_archive_drop();
            g_results_tracker.refresh_active = 1;
            g_results_tracker.refresh_mode = 1;
            g_results_tracker.refresh_base = ctx.refresh_ms;
        }
    }

    /* --- visibility verdicts for the two battle-end features --- */
    memset(ui_state, 0, sizeof ui_state);
    r1 = nexus_menu_ui_state(ui_state, owning_tid);
    menu_open = ui_state[4] != 0;
    state_ok = r1 == 1 && menu_open;

    memset(q_rec, 0, sizeof q_rec);
    r1 = ui_named_state_query("ShowFastPlayAgainButton", q_rec);
    fast_visible = r1 == 1 && ((char *)q_rec)[0x14] != 0 && q_rec[3] != 0;
    r1 = ui_named_state_query("BattleEndInstantExit", q_rec);
    mask = (uint32_t)fast_visible | 2;
    any_visible = (r1 == 1 && ((char *)q_rec)[0x14] != 0 && q_rec[3] != 0)
                  || fast_visible;
    if (r1 != 1 || ((char *)q_rec)[0x14] == 0 || q_rec[3] == 0)
        mask = (uint32_t)fast_visible;

    if (any_visible
        || (r1 != 1 || (!menu_open && g_pair_release_epoch == 0)))
        g_track_enabled = (uint8_t)any_visible;
    {
        /* re-read the exit record the same way the raw does (the verdict
         * above consumed it) */
        (void)mask;
    }

    if (ctx.autofarm_enabled == 0) {
        g_pair_release_epoch = 0;
        overlay_pair_state_pump(0, &ctx, state_ok);
    } else {
        if (g_guard_last_c != 0 && g_guard_last_a == ctx.stage_view
            && g_guard_last_b == ctx.generation) {
            if (ctx.widget_obj == 0) {
                if (g_hook_module_handle != 0
                    && g_guard_last_a == g_hook_last_a
                    && g_guard_last_b == g_hook_last_c
                    && button_lineage_check(g_hook_module_handle,
                                            ctx.stage_view) != 0) {
                    word248 = 0;
                    r1 = proc_mem_read(NULL, g_hook_module_handle + 0x248,
                                       &word248, 8);
                    if (!is_plausible_ptr_e(word248) || r1 == 0)
                        word248 = 0;
                    if (word248 == g_hook_last_b)
                        goto release_rearm;
                }
                goto hook_inflight_check;
            }
release_rearm:
            g_pair_release_epoch = 0;
rearm:
            g_results_tracker.archived = 0;
            memset(&g_results_tracker.archive, 0,
                   sizeof g_results_tracker.archive);
            g_results_tracker.refresh_active = 1;
            g_results_tracker.refresh_mode = 1;
            overlay_pair_state_pump(0, &ctx, state_ok);
            return;
        }

hook_inflight_check:
        if (g_hook_inflight != 0 && ctx.autofarm_enabled != 0
            && g_hook_module_handle != 0
            && ctx.stage_view == g_hook_last_a
            && ctx.generation == g_hook_last_c
            && button_lineage_check(g_hook_module_handle,
                                    ctx.stage_view) != 0) {
            word248 = 0;
            r1 = proc_mem_read(NULL, g_hook_module_handle + 0x248,
                               &word248, 8);
            if (!is_plausible_ptr_e(word248) || r1 == 0)
                word248 = 0;
            if (word248 == g_hook_last_b)
                goto rearm;
        }
        if (g_track_enabled != 1) {
            overlay_pair_state_pump(0, &ctx, state_ok);
            goto stats_line;
        }
        screen_scan_pump((screen_record_t *)(void *)&ctx);
        if (g_hook_inflight != 0 && ctx.autofarm_enabled != 0
            && g_hook_module_handle != 0
            && ctx.stage_view == g_hook_last_a
            && ctx.generation == g_hook_last_c
            && button_lineage_check(g_hook_module_handle,
                                    ctx.stage_view) != 0) {
            word248 = 0;
            r1 = proc_mem_read(NULL, g_hook_module_handle + 0x248,
                               &word248, 8);
            if (!is_plausible_ptr_e(word248) || r1 == 0)
                word248 = 0;
            if (word248 == g_hook_last_b)
                goto rearm;
        }
        overlay_pair_state_pump(mask, &ctx, state_ok);

        /* --- the armed-action dispatch --- */
        pair_b = g_battle_end_pair_b;
        pair_a = g_battle_end_pair_a;
        end_node = g_battle_end_node;
        mode = (uint32_t)g_pair_release_epoch;
        if (kind == 1) {
            /* click routing through the captured overlay pair */
            if ((mask & (uint32_t)(g_capture_flags >> 32) & 1) != 0
                && g_capture_subject == g_overlay_parent
                && g_capture_pair_a != 0) {
                word = 0;
                r1 = proc_mem_read(NULL, g_capture_pair_a, &word, 8);
                if (!is_plausible_ptr_e(word) || r1 == 0)
                    word = 0;
                if (word == (uint64_t)g_proc_mem_bias + 0x11c0a48
                    && remote_child_link_valid(g_capture_pair_a,
                                               g_overlay_parent) != 0
                    && g_capture_pair_a == node) {
                    action = 1;
                    feature = "ShowFastPlayAgainButton";
                    goto feature_log;
                }
            }
            if (((mask & (uint32_t)(g_capture_flags >> 32)) >> 1) != 0
                && g_capture_subject == g_overlay_parent
                && g_capture_pair_b != 0) {
                word = 0;
                r1 = proc_mem_read(NULL, g_capture_pair_b, &word, 8);
                if (!is_plausible_ptr_e(word) || r1 == 0)
                    word = 0;
                if (word == (uint64_t)g_proc_mem_bias + 0x11c0a48
                    && remote_child_link_valid(g_capture_pair_b,
                                               g_overlay_parent) != 0
                    && g_capture_pair_b == node) {
                    action = 2;
                    feature = "BattleEndInstantExit";
                    goto feature_log;
                }
            }
            mask = (uint32_t)(g_pair_release_epoch != 0);
        } else {
            if (g_pair_release_epoch == 0)
                goto stats_line;
            if (kind != 3 || ctx.refresh_ms <= g_battle_end_ms)
                return;

            subject_ok = false;
            claimed = false;
            if (g_pair_release_epoch == 1 && g_battle_end_pair_b != 0) {
                memset(&replay2, 0, sizeof replay2);
                r1 = service_object_acquire(
                    (const screen_record_t *)(const void *)&ctx, &replay2);
                if (r1 != 0 && replay2.field_10 == pair_a) {
                    subject_ok = replay2.field_18 == pair_b;
                    claimed = subject_ok;
                }
            } else if (g_battle_end_pair_b == 0) {
                r1 = captured_widget_check(
                    (const screen_record_t *)(const void *)&ctx);
                if (r1 != 0 && end_node == g_hook_module_handle) {
                    word = remote_read_ptr(end_node + 0x248);
                    subject_ok = word == g_battle_end_word;
                    claimed = false;
                }
            } else {
                subject_ok = false;
                claimed = false;
            }

            if (g_battle_end_ms < ctx.refresh_ms
                && ctx.refresh_ms - g_battle_end_ms < 0x7d1
                && ctx.stage_byte == 0
                && ctx.stage_view == g_battle_end_stage) {
                mask = 0;
                g_pair_release_epoch = 0;
                {
                    bool go = false;

                    if (ctx.generation == g_battle_end_stamp)
                        go = subject_ok;
                    if (go) {
                        if (!claimed
                            || script_port_fast_replay_claim(pair_a, pair_b,
                                                             1) != 0) {
                            if (mode == 1) {
                                mask = (uint32_t)plus_service_object_create_default();
                                if (mask != 0)
                                    goto action_finish;
                            } else {
                                if (code_region_hash_verify(0x91aae0, 0x1dc,
                                        "ffc8b3034ec3cc494046376cfe663e164e4e4e6210f7a4ba0eb1d68c3346715f") == 0)
                                    goto action_failed;
                                if (button_lineage_check(end_node,
                                                         ctx.stage_view) != 0) {
                                    word = remote_read_ptr(end_node + 0x248);
                                    if (word == g_battle_end_word) {
                                        mask = 1;
                                        /* engine leave-battle call */
                                        ((void (*)(uint64_t, uint64_t))(
                                            (uint64_t)g_proc_mem_bias
                                            + 0x91aae0))(end_node, 1);
                                        if (button_lineage_check(
                                                end_node,
                                                ctx.stage_view) != 0
                                            && remote_read_ptr(
                                                   end_node + 0x248)
                                                   == g_battle_end_word) {
                                            ((void (*)(uint64_t, uint64_t))(
                                                (uint64_t)g_proc_mem_bias
                                                + 0x91aae0))(end_node, 1);
                                        }
                                        goto action_finish;
                                    }
                                }
                            }
                        action_failed:
                            if (claimed)
                                script_port_fast_replay_claim(pair_a, pair_b,
                                                              0);
                        }
                        mask = 0;
                    }
                }
            } else {
                mask = 0;
                g_pair_release_epoch = 0;
            }
        }

        /* both dispatch paths meet here: a completed action returns, a
         * failed/deferred one falls through to the stats line */
        if (mask != 0)
            return;
    }

feature_log:
        verdict = (uint32_t)battle_end_feature_arm(feature);
        reason = verdict == 2 ? "queued" : "unavailable";
        if (g_stage_log_count < 0x80) {
            g_stage_log_count = g_stage_log_count + 1;
            if (clock_gettime(0 /* CLOCK_REALTIME */, &now) == 0)
                ms = (uint64_t)now.tv_sec * 1000
                     + (uint64_t)now.tv_nsec / 1000000;
            else
                ms = 0;
            __android_log_print(4, "NexusLab69252",
                "{\"stage\":\"%s\",\"reason\":\"%s\",\"action\":%u,"
                "\"pid\":%d,\"tid\":%d,\"time_ms\":%llu}",
                "script_port_result_click", reason, action, getpid(),
                owning_tid, ms);
        }
        return;

action_finish:
        /* the successful-action tail: latch the hook/guard identity and
         * re-arm the refresh capture */
        if (claimed) {
            if (captured_widget_check(
                    (const screen_record_t *)(const void *)&ctx) != 0)
                g_hook_inflight = (g_hook_inflight & ~0xffffffffu) | 1;
            g_guard_last_c = pair_b;
            g_guard_stamp = ctx.refresh_ms;
            g_guard_last_b = ctx.generation;
            g_guard_last_a = ctx.stage_view;
        } else {
            g_hook_inflight = (g_hook_inflight & ~0xffffffffu) | 1;
        }
        tracker_archive_drop();
        g_results_tracker.refresh_active = 1;
        g_results_tracker.refresh_mode = 1;
        g_results_tracker.refresh_base = ctx.refresh_ms;
        return;

stats_line:
    /* --- the BATTLES/W/L/D/TROPHIES toast line (50/250 ms latch) --- */
    interval = 0x32;
    if ((int)g_battle_state != 0)
        interval = 0xfa;
    if (*(const uint8_t *)&g_view_node_cache != 1
        || g_stats_interval != interval
        || now_ms < g_stats_stamp
        || interval <= now_ms - g_stats_stamp) {
        bool setting_on = false;

        *(uint8_t *)&g_view_node_cache = 1;
        setting = setting & 0xffffffff00000000ULL;  /* the raw preserves the
                                                     * slot's high dword */
        g_stats_stamp = now_ms;
        g_stats_interval = (uint32_t)interval;
        if (ctx.autofarm_enabled != 0 && ctx.evasion_armed != 0) {
            if (nexus_menu_setting_value(0x27, (int32_t *)&setting) == 1)
                setting_on = (int32_t)setting == 1;
        }
        str_format(text, 0x80, 0x80,
                   "BATTLES %llu  W %llu  L %llu  D %llu\nTROPHIES %+lld",
                   g_results_tracker.screen_changes,
                   g_results_tracker.tally_count_hi,
                   g_results_tracker.tally_count_lo,
                   g_results_tracker.tally_count_two,
                   (int64_t)g_results_tracker.delta_sum);
        rich_toast_set(text, setting_on);
    }

    /* --- tail feeds --- */
    if (ctx.autofarm_enabled == 0
        || (ctx.evasion_armed == 0 && ctx.autofarm_gated == 0)) {
        if ((g_track_enabled & 1) == 0)
            memset((void *)&g_tracked_node, 0, 0x338);
        return;
    }
    screen_scan_pump((screen_record_t *)(void *)&ctx);

    if (kind == 1) {
        if (g_endcap_node == node
            || g_results_tracker.refresh.subject == node)
            g_results_tracker.refresh_active = 1;
        return;
    }
    if ((kind | 2u) == 2u) {
        if (remote_string_read(text_obj, text) != 0) {
            if (kind == 2)
                screen_text_feed((const screen_record_t *)(const void *)&ctx,
                                 node, text);
            else
                result_text_feed((const screen_record_t *)(const void *)&ctx,
                                 node, text);
        }
        return;
    }

    /* --- kind 3: the observation poll (every 0x32 ms) --- */
    if (kind != 3 || now_ms < g_track_poll_next)
        return;
    g_track_poll_next = now_ms + 0x32;

    memset(&track, 0, sizeof track);
    if (results_tracker_screen_archive(&g_results_tracker,
            (const screen_record_t *)(const void *)&ctx, &track) == 0)
        return;

    menu_context_init(&fresh, now_ms);
    {
        bool invalid;

        invalid = !(track.subject == node
                    || remote_widget_ancestor_of(track.subject,
                                                 ctx.stage_view) != 0)
            || results_record_still_valid((const int64_t *)&track) == 0
            || (fresh.autofarm_enabled == 0 || fresh.stage_byte != 0)
            || fresh.generation != ctx.generation
            || (fresh.stage_view != ctx.stage_view
                || (fresh.evasion_armed == 0 && fresh.autofarm_gated == 0));

        if (invalid) {
            if (g_results_tracker.archived != 0)
                tracker_archive_drop();
            return;
        }
    }

    if (track.mode == 1) {
        /* create the plus-service object through the default accessor */
        acc.ctx = 0;
        acc.base = (uint64_t)g_proc_mem_bias;
        acc.read = proc_mem_read;
        acc.alloc = (void *(*)(uint64_t))(
            (uint64_t)g_proc_mem_bias + PLUS_SVC_ALLOC_RVA);
        acc.init = (void (*)(void *, int, int))(
            (uint64_t)g_proc_mem_bias + PLUS_SVC_INIT_RVA);
        acc.register_fn = (void (*)(uint64_t, void *))(
            (uint64_t)g_proc_mem_bias + 0xac319c);
        created = plus_service_object_create(&acc);
        if (g_results_tracker.archived != 0) {
            if (created != 0) {
                g_results_tracker.refresh_active = 1;
                g_results_tracker.refresh_mode =
                    g_results_tracker.archive.mode;
            }
            tracker_archive_drop();
            if (created == 0)
                return;
        }
    } else {
        if (track.mode == 2
            && (fresh.evasion_armed == 0 || fresh.widget_obj != 0)) {
            if (g_results_tracker.archived != 0)
                tracker_archive_drop();
            return;
        }
        /* the archived dispatch: word5(word4, subject) */
        ((void (*)(uint64_t, uint64_t))(uintptr_t)track.word5)(track.word4,
                                                               track.subject);
        if (g_results_tracker.archived != 0) {
            g_results_tracker.refresh_active = 1;
            g_results_tracker.refresh_mode = g_results_tracker.archive.mode;
            tracker_archive_drop();
        }
    }

    __android_log_print(4, "NexusRelease69252",
        "{\"stage\":\"postbattle_dispatch\",\"kind\":%u,\"epoch\":%llu,"
        "\"matches\":%llu,\"trophies\":%lld,\"server_acceptance_proven\":false}",
        (uint32_t)track.mode, ctx.generation, g_results_tracker.screen_changes,
        (int64_t)g_results_tracker.delta_sum);
}
/* widgets chain 3 chunk 8: covers raw lines 3233-3605 (menu_perf_status_row_append + battle_end_button_build; nexus_rich_plan @3606 is deferred whole to chunk 9) */

/* ===== rich-view row record (mirror of renderer.c) =====
 * 0xb0 stride; the view object starts with a 0x10-byte header (row count at
 * +0x00); row i lives at view + 0x10 + i*0xb0. Kept byte-faithful to the
 * raw stores. */

typedef struct {
    uint32_t kind;         /* +0x00 — low dword of the template qword; 2/3 = interactive */
    uint32_t flags;        /* +0x04 — high dword of the template qword */
    uint32_t _r08;         /* +0x08 — entry id (known entries) / 0 */
    uint32_t color;        /* +0x0c */
    uint32_t field_10;     /* +0x10 — raw stores (flags == 0) */
    float    x;            /* +0x14 */
    float    y;            /* +0x18 */
    float    font_size;    /* +0x1c — raw CONCAT44 pair: same value twice */
    float    font_size_2;  /* +0x20 */
    uint8_t  _r24[8];      /* +0x24 */
    uint8_t  active;       /* +0x2c — 1 for live rows, 0 for padded rows */
    uint8_t  font_class;   /* +0x2d */
    char     text[0x82];   /* +0x2e — NUL-terminated glyph text */
} rich_row_e_t;            /* sizeof == 0xb0 */

#define RICH_E_COUNT(view)     (*(uint32_t *)(void *)(view))
#define RICH_E_ROWS(view)      ((rich_row_e_t *)((char *)(view) + 0x10))
#define RICH_E_ROW(view, i)    ((rich_row_e_t *)((char *)(view) + 0x10 + (size_t)(i) * 0xb0))

/* ===== cross-file functions ===== */

extern int sc_object_release(uint64_t obj);   /* misc.c @ 0014e5ac — validate +
                                               * clear the +0x218 state word */
extern int sc_object_free_check_local(uint64_t obj);  /* misc.c @ 00164090 */
extern void widget_set_scale(uint64_t widget, float sx, float sy);
                                      /* misc.c @ 0016cf00 — engine 0x595344
                                       * (raw shows (sx, sy, widget) — the
                                       * Ghidra float-merge display order) */
extern int style_bundle_lookup(void *ctx, const char *name);  /* misc.c @ 00163eb8 */
extern int rich_text_layout(float x, float y, float font_size, float scale,
                            uint32_t *view, const char *text, int bright,
                            uint32_t cell_y, uint32_t max_rows,
                            uint64_t position);
                                      /* renderer.c @ 0016b76c — the raw call
                                       * sites carry 10 args and consume the
                                       * verdict; renderer.c's own def folds
                                       * arg2/4 and drops the return —
                                       * coordinator reconciles */

/* ===== globals owned elsewhere ===== */

extern uint32_t g_ui_cycle_blocked;       /* 0x22d888 — renderer.c */
extern uint32_t g_reload_requested;       /* 0x22d88c — renderer.c */
extern uint32_t g_graphics_policy_active; /* 0x22d890 — renderer.c */
extern uint32_t g_battle_word4;           /* 0x22d894 — misc.c: reload-failed */
extern uint32_t g_perf_latch;             /* 0x22dfb8 — misc.c: perf save in flight */
extern uint32_t g_perf_confirmed;         /* 0x22dfbc — misc.c: dir fsync failed */
extern uint32_t g_perf_seq;               /* 0x22f4b8 — misc.c: perf save sequence */
extern uint32_t g_stage_log_count;        /* 0x22f4bc — menu_engine.c: capped-128 stage log */
extern uint64_t g_battle_state;           /* 0x22d880 — misc.c: battle/screen state word */
extern uint64_t g_launcher_stage_root;    /* 0x22f178 — renderer.c: stashed stage root */
extern uint64_t g_launcher_stage_view;    /* 0x22db60 — renderer.c: stashed stage view */
extern uint32_t g_lowres_atlas_pair;      /* 0x25cb38 — menu_engine.c: lowres atlas
                                           * source pair value */
extern uint64_t g_capture_subject;        /* 0x22db40 — misc.c: pair capture subject */
extern uint64_t g_capture_pair_a;         /* 0x22db48 — misc.c: captured pair slot a */
extern uint64_t g_capture_pair_b;         /* 0x22db50 — misc.c: captured pair slot b */
extern uint64_t g_capture_flags;          /* 0x22db58 — misc.c: hi dword = slot live bits */
extern uint64_t g_overlay_parent;         /* 0x22db60 — misc.c: capture parent (= the
                                           * stage view) */
extern uint64_t g_hook_module_handle;     /* 0x22db18 — this file chunk 1 */
extern uint64_t g_hook_last_a;            /* 0x22db20 */
extern uint64_t g_hook_last_b;            /* 0x22db28 */
extern uint64_t g_hook_last_c;            /* 0x22db30 */
extern uint32_t g_hook_inflight;          /* 0x22db38 */
extern uint64_t g_guard_last_a;           /* 0x22f1b0 */
extern uint64_t g_guard_last_b;           /* 0x22f1b8 */
extern uint64_t g_guard_last_c;           /* 0x22f1c0 */
extern uint64_t g_pair_release_epoch;     /* 0x22dad8 — misc.c */
extern uint64_t g_proc_mem_bias;          /* 0x22d8e0 — misc.c */

/* ===== rodata: rich-row template qwords ===== */

extern const uint64_t RICH_HDR_KIND;      /* 0x10f860 — header-row kind/flags */
extern const uint64_t RICH_HDR_FIELD1C;   /* 0x10f910 — header +0x1c pair */
extern const uint64_t RICH_TAB_KIND;      /* 0x10f7b0 — category-tab kind/flags */
extern const uint64_t RICH_TAB_TEMPLATE;  /* 0x10f840 — tab +0x28 template */
extern const uint64_t RICH_TAB_SEL_KIND;  /* 0x10f920 — selected-tab kind/flags */
extern const uint64_t RICH_TAB_SEL_XY;    /* 0x10f848 — selected-tab xy delta pair */
extern const uint64_t RICH_TAB_XY;        /* 0x10f8e0 — tab +0x24 xy pair */
extern const uint64_t RICH_HDR2_KIND;     /* 0x10f860 — separator kind (shared) */
extern const uint64_t RICH_HDR2_FIELD1C;  /* 0x10f878 — separator +0x34 pair */
extern const uint64_t RICH_CLASSBAR_KIND; /* 0x10f728 — class-scale bar kind/flags */
extern const uint64_t RICH_CLASSBAR_A;    /* 0x10fb70 — class bar +0x28 pair */
extern const uint64_t RICH_CLASSBAR_B;    /* 0x10fb78 — class bar +0x30 pair */
extern const uint64_t RICH_ROW_TEMPLATE;  /* 0x10f7e0 — text-row kind/flags */
extern const uint64_t RICH_ROW_PAD_FONT;  /* 0x10f9b8 — pad-row font pair */
extern const uint64_t RICH_ROW_PAD_XY;    /* 0x10f9b0 — pad-row xy pair */
extern const uint64_t RICH_HDR_FIELD2C;   /* 0x10f958 — header +0x2c pair */
extern const uint64_t RICH_HDR_FIELD24;   /* 0x10f950 — header +0x24 pair */
extern const uint64_t RICH_ROW_FIELD1C;   /* 0x10f7c0 — form-row +0x24 pair */
extern const uint64_t RICH_ROW_FIELD24;   /* 0x10f7e8 — form-row +0x1c pair */
extern const uint64_t RICH_SEP_FIELD1C;   /* 0x10faa0 — post-classbar separator +0x24 */
extern const uint64_t RICH_SEP_FIELD24;   /* 0x10faa8 — post-classbar separator +0x1c */
extern const uint64_t RICH_FORM_FONT_A;   /* 0x10fab0 — form row font pair a */
extern const uint64_t RICH_FORM_FONT_B;   /* 0x10fab8 — form row font pair b */
extern const uint64_t RICH_FORM_FIELD1C;  /* 0x10faf0 — form row +0x2c pair */
extern const uint64_t RICH_FORM_FIELD24;  /* 0x10faf8 — form row +0x24 pair */
extern const uint64_t RICH_PLACE_FONT_A;  /* 0x10f980 — placeholder +0x24 pair */
extern const uint64_t RICH_PLACE_FONT_B;  /* 0x10f988 — placeholder +0x1c pair */
extern const uint64_t RICH_CENTER_A;      /* 0x10f7a0 — center literal a {x, y} */
extern const uint64_t RICH_CENTER_B;      /* 0x10f7a8 — center literal b */
extern const uint64_t RICH_TAB_ICON_A;    /* 0x10f990 — tab icon +0x18 pair */
extern const uint64_t RICH_TAB_ICON_B;    /* 0x10f998 — tab icon +0x20 pair */
extern const uint64_t RICH_TAB_ICON_C;    /* 0x10fb20 — tab icon +0x34 pair */
extern const uint64_t RICH_TAB_ICON_D;    /* 0x10fb28 — tab icon +0x3c pair */
extern const uint64_t RICH_MAIN_TITLE_A;  /* 0x10f9b0 — (shared pad xy) */
extern const uint64_t RICH_CLASSBAR_C;    /* 0x10fb90 — form fill +0x24 */
extern const uint64_t RICH_CLASSBAR_D;    /* 0x10fb98 — form fill +0x1c */
extern const uint64_t RICH_PLUS_STATE_LO; /* 0x10f8a0 — plus-tab state seed lo */
extern const uint64_t PERF_ROW_FIELD28;   /* 0x10f770 — perf status row +0x28 pair */
extern const uint64_t RICH_SCALE_CENTER;  /* 0x10f880 — scale center literal */
extern const uint64_t RICH_SCALE_CENTER2; /* 0x10f860 — (shared header kind) */

/* ===== rodata: rich view content tables ===== */

extern const uint64_t g_rich_tab_ids[6];        /* 0x13cdb0 — tab id qwords */
extern const char g_class_set_a[];              /* 0x138fa4 — 3-char class set */
extern const char *const g_main_page_titles[6]; /* 0x19b450 — MAIN PAGE strings */
extern const char g_rich_subtitle[];            /* 0x1384c3 — the subtitle text */
extern const char g_rich_map_editor_button[];   /* "map_editor_big_exit_button" */

/* rich page descriptor table: {class char, entry id, ..., +0xc flags,
 * +0x10 owner} — 0x20 stride, 83 entries */
typedef struct {
    uint8_t  class_char;   /* +0x00 — the view class char */
    uint8_t  _pad01[3];
    uint32_t entry_id;     /* +0x04 — the entry id (row kind) */
    uint32_t _pad08;
    uint32_t flags;        /* +0x0c — row flags when +0x10 is 0 */
    uint32_t owner;        /* +0x10 — 0 = use the flags word */
    uint32_t _pad14[3];
    uint32_t _pad18[2];
} rich_page_desc_t;

extern const rich_page_desc_t g_rich_page_table[83];  /* 0x19b5e8 */

/* description-label offsets: {x, y} qword pairs, 0x18-stride */
extern const uint64_t g_rich_desc_offsets[];    /* 0x19b490 */

/*
 * menu_perf_status_row_append — append the performance / texture-reload
 * status row to a menu page (0x40-stride rows, count at +4, cap 0x1a).
 * Only while the page is open, enabled and not filtered; the status line
 * picks the first failure in the fixed ladder: cycles blocked -> "TEXTURES
 * FAILED / RESTART GAME", dir-fsync failed -> "SAVE FAILED / TAP TO
 * RETRY", save in flight -> "TEXTURES FAILED / TAP TO RETRY", reload
 * failed -> "RELOAD FAILED / TAP TO RETRY", else the reload/low-res state
 * ("RELOADING TEXTURES" / "LOW-RES TEXTURES / GAME RELOAD"). The row gets
 * the "MAX OPTIMIZATION" title, category 0x20002, action 0x10100000000,
 * the 0x10f770 payload, enabled = mode && atlas-pair && !latch, and the
 * 5-tap focus. @ 00162118
 */
void menu_perf_status_row_append(void *page)
{
    uint8_t *rows = (uint8_t *)page;
    uint32_t *count = (uint32_t *)((char *)page + 4);
    uint8_t *row;
    const char *status;
    uint32_t mode = (uint32_t)g_battle_state;
    uint8_t blocked = (uint8_t)g_ui_cycle_blocked;
    uint8_t latch = (uint8_t)g_perf_latch;

    if (((char *)page)[9] == 0 || ((char *)page)[10] != 0
        || ((char *)page)[8] != 'O' || *count >= 0x1a)
        return;

    row = rows + (size_t)*count * 0x40;
    *count = *count + 1;

    if ((blocked & 1) != 0) {
        status = "TEXTURES FAILED / RESTART GAME";
    } else if (g_perf_confirmed == 0) {
        if ((latch & 1) == 0) {
            if (g_battle_word4 == 0) {
                status = "LOW-RES TEXTURES / GAME RELOAD";
                if (g_perf_seq != 0)
                    status = "RELOADING TEXTURES";
                if ((g_reload_requested | g_graphics_policy_active) & 1)
                    status = "RELOADING TEXTURES";
            } else {
                status = "RELOAD FAILED / TAP TO RETRY";
            }
        } else {
            status = "TEXTURES FAILED / TAP TO RETRY";
        }
    } else {
        status = "SAVE FAILED / TAP TO RETRY";
    }

    *(const char **)(row + 0x18) = "MAX OPTIMIZATION";
    *(const char **)(row + 0x20) = status;
    *(uint32_t *)(row + 0x10) = 0x20002;                 /* category */
    *(uint64_t *)(row + 0x40) = 0x10100000000ULL;        /* action id */
    *(uint32_t *)(row + 0x30) = 0x18;                    /* row kind */
    *(int *)(row + 0x34) = (int)mode;
    *(uint64_t *)(row + 0x28) = PERF_ROW_FIELD28;        /* 0x10f770 */
    *(uint32_t *)(row + 0x38) =
        (uint32_t)(mode != 0 && g_lowres_atlas_pair != 0) & (uint32_t)(latch ^ 1);
    *(int *)(row + 0x3c) = (int)mode;
    *(uint16_t *)(row + 0x48) = 5;                       /* focus/tap count */
}

/* ===== battle-end game-button pair ===== */

/*
 * overlay_pair_state_pump — validate and rebuild the two battle-end game
 * buttons ("PLAY AGAIN" and "EXIT") when the screen state demands it.
 * mask bit0/bit1 request the two buttons; menu_open gates the build (a
 * closed menu only releases stale buttons).
 *
 * First the captured pair slots are revalidated (vtable libg+0x11c0a48,
 * child link to the capture parent) and dropped when stale. Then the
 * captured hook node's +0x248 word is rechecked, a fresh fast-replay
 * service record claimed (when the guard triple is not current), and the
 * two buttons built from the "map_editor_big_exit_button" template: the
 * SC bundle style must resolve, a 0x260 engine object allocated and
 * released as free, the styled object created (vtable libg+0x11ad208),
 * attached (+0x80/+0xb0 self-links) and its child (+0x88, lineage-checked)
 * labeled through the engine text calls, scaled 1.25 and attached to the
 * stage root. Button failures log the capped-128
 * "script_port_result_button_unavailable"/"owned_gamebutton_contract"
 * line. Positions: PLAY AGAIN at (grid_x/scale - 95 ... /scale, (88 -
 * field_y)/scale), EXIT at (95, same y). The pair live-bits in the
 * 0x22db5c flags word track what is installed. @ 00165ab8
 */
void overlay_pair_state_pump(uint64_t mask, menu_context_t *ctx, int menu_open)
{
    service_record_t replay;
    struct timespec now;
    uint64_t word, word2, button, styled;
    uint64_t *slot_p;
    uint32_t live_bits, hook_triple;
    float density, field_x, field_y, base_x, x;
    float grid[4];
    int r, i;
    bool hook_ok, replay_ok, first = true;

    /* --- revalidate the captured pair slots --- */
    if (g_capture_subject == 0 || g_capture_subject == g_overlay_parent) {
        if (g_capture_pair_a != 0) {
            if (g_capture_subject == g_overlay_parent) {
                word = 0;
                r = proc_mem_read(NULL, g_capture_pair_a, &word, 8);
                if (!is_plausible_ptr_e(word) || r == 0)
                    word = 0;
                if (word == (uint64_t)g_proc_mem_bias + 0x11c0a48
                    && remote_child_link_valid(g_capture_pair_a,
                                               g_overlay_parent) != 0)
                    goto slot_b;
            }
            g_capture_pair_a = 0;
            g_capture_flags = g_capture_flags & 0xfffffffeffffffffULL;
        }
    } else {
        g_capture_pair_a = 0;
        g_capture_subject = 0;
        g_capture_flags = 0;
        g_capture_pair_b = 0;
    }
slot_b:
    if (g_capture_pair_b != 0) {
        if (g_capture_subject == g_overlay_parent) {
            word = 0;
            r = proc_mem_read(NULL, g_capture_pair_b, &word, 8);
            if (!is_plausible_ptr_e(word) || r == 0)
                word = 0;
            if (word == (uint64_t)g_proc_mem_bias + 0x11c0a48
                && remote_child_link_valid(g_capture_pair_b,
                                           g_overlay_parent) != 0)
                goto slots_done;
        }
        g_capture_pair_b = 0;
        g_capture_flags = g_capture_flags & 0xfffffffdffffffffULL;
    }
slots_done:

    /* --- the captured hook node still matches this screen? --- */
    hook_ok = false;
    if (ctx->autofarm_enabled != 0 && g_hook_module_handle != 0) {
        if (ctx->stage_view == g_hook_last_a
            && ctx->generation == g_hook_last_c
            && button_lineage_check(g_hook_module_handle,
                                    ctx->stage_view) != 0) {
            word = 0;
            r = proc_mem_read(NULL, g_hook_module_handle + 0x248, &word, 8);
            if (!is_plausible_ptr_e(word) || r == 0)
                word = 0;
            hook_ok = word == g_hook_last_b;
        }
    }

    /* --- a fresh fast-replay pair may be claimed for this screen --- */
    memset(&replay, 0, sizeof replay);
    replay.header = SERVICE_RECORD_HEADER;
    replay_ok = false;
    if (ctx->autofarm_enabled != 0 && ctx->widget_obj != 0) {
        if (g_guard_last_c == 0
            || (g_guard_last_a != ctx->stage_view
                || g_guard_last_b != ctx->generation)) {
            if (script_port_fast_replay_snapshot(
                    (uint64_t)(uintptr_t)&replay) != 0
                && replay.field_10 != 0 && replay.field_18 != 0)
                replay_ok = replay.ok_a != 0;
        }
    }

    hook_triple = (hook_ok && g_hook_inflight == 0) ? 3 : 0;
    live_bits = 0;
    if ((mask & 1) != 0 && menu_open == 0) {
        /* both buttons requested while the menu is closed: only when the
         * context is live, no battle-end epoch is armed and the guard
         * triple is not current for this screen */
        if (!(ctx->autofarm_enabled == 0 || ctx->stage_byte != 0
              || g_pair_release_epoch != 0
              || (g_guard_last_c != 0 && g_guard_last_a == ctx->stage_view
                  && g_guard_last_b == ctx->generation)))
            live_bits = ((uint32_t)replay_ok | hook_triple) & (uint32_t)mask;
    }

    /* --- stage geometry (density, field center, grid base) --- */
    {
        uint32_t density_u = 0, field_x_u = 0, field_y_u = 0;

        if (proc_mem_read(NULL, g_launcher_stage_root + 0x178,
                          &density_u, 4) == 0
            || !isfinite((float)density_u) || !(0.0f < (float)density_u)
            || !((float)density_u <= 16.0f)
            || proc_mem_read(NULL, g_launcher_stage_root + 0x4c,
                             &field_x_u, 4) == 0
            || proc_mem_read(NULL, g_launcher_stage_root + 0x54,
                             &field_y_u, 4) == 0
            || !isfinite((float)field_x_u) || !isfinite((float)field_y_u)
            || proc_mem_read(NULL, g_launcher_stage_root + 0x3c, grid,
                             0x10) == 0
            || !isfinite(grid[3]) || !(300.0f <= grid[3]))
            live_bits = 0;

        density = (float)density_u;
        field_x = (float)field_x_u;
        field_y = (float)field_y_u;
        base_x = grid[3];   /* the +0x44 float anchors PLAY AGAIN */
    }

    /* --- per-button loop: bit0 = PLAY AGAIN, bit1 = EXIT --- */
    for (i = 0; ; i = 1, first = false) {
        uint32_t bit = 1u << i;

        if ((bit & live_bits) == 0) {
            /* not requested: release the slot when it holds a live button */
            if (g_capture_subject == g_overlay_parent) {
                uint64_t *slot_p = (i == 0) ? &g_capture_pair_a
                                            : &g_capture_pair_b;

                if (*slot_p != 0) {
                    word = 0;
                    r = proc_mem_read(NULL, *slot_p, &word, 8);
                    if (!is_plausible_ptr_e(word) || r == 0)
                        word = 0;
                    if (word == (uint64_t)g_proc_mem_bias + 0x11c0a48
                        && remote_child_link_valid(*slot_p,
                                                   g_overlay_parent) != 0) {
                        /* engine detach + release */
                        ((void (*)(uint64_t, uint64_t, uint64_t))(
                            (uint64_t)g_proc_mem_bias + 0x595314))(
                                0xc61c3c00, 0xc61c3c00, *slot_p);
                        /* clear the slot's live bit in the flags word */
                        g_capture_flags &= ~((uint64_t)bit << 32);
                        goto next;
                    }
                }
            }
            goto next;
        }

        /* requested: reuse a live button or build one */
        if (g_capture_subject == g_overlay_parent) {
            uint64_t *slot_p = (i == 0) ? &g_capture_pair_a
                                        : &g_capture_pair_b;

            if (*slot_p == 0)
                goto build;
            word = 0;
            r = proc_mem_read(NULL, *slot_p, &word, 8);
            if (!is_plausible_ptr_e(word) || r == 0)
                word = 0;
            if (word != (uint64_t)g_proc_mem_bias + 0x11c0a48
                || remote_child_link_valid(*slot_p, g_overlay_parent) == 0)
                goto build;
            button = *slot_p;
            goto position;
        }

build:
        /* the template style must resolve in the loaded bundle */
        if (!(g_capture_flags == 0
              && style_bundle_lookup(NULL,
                                     "map_editor_big_exit_button") != 0))
            goto button_failed;

        button = ((uint64_t (*)(uint64_t))(
            (uint64_t)g_proc_mem_bias + 0x11a2840))(0x260);
        if (button == 0)
            goto button_failed;
        ((void (*)(void))((uint64_t)g_proc_mem_bias + 0x887c14))();
        if (sc_object_release(button) == 0
            || sc_object_free_check_local(button) == 0)
            goto button_failed;
        styled = ((uint64_t (*)(const char *, const char *))(
            (uint64_t)g_proc_mem_bias + 0x51ecec))("sc/ui.sc",
                                                   "map_editor_big_exit_button");
        if (styled == 0)
            goto button_failed;
        word = 0;
        r = proc_mem_read(NULL, styled, &word, 8);
        if (!is_plausible_ptr_e(word) || r == 0)
            word = 0;
        if (word != (uint64_t)g_proc_mem_bias + 0x11ad208)
            goto button_failed;

        ((void (*)(uint64_t, uint64_t, int))(
            (uint64_t)g_proc_mem_bias + 0x772a30))(button, styled, 1);
        word2 = ((uint64_t (*)(uint64_t, int))(
            (uint64_t)g_proc_mem_bias + 0x5d7c30))(styled, 0);
        word = 0;
        r = proc_mem_read(NULL, word2 + button + 0x80, &word, 8);
        if (!is_plausible_ptr_e(word) || r == 0)
            word = 0;
        if (word == styled) {
            word = 0;
            r = proc_mem_read(NULL, button + 0xb0, &word, 8);
            if (!is_plausible_ptr_e(word) || r == 0)
                word = 0;
            if (word == styled) {
                word = 0;
                r = proc_mem_read(NULL, button + 0x88, &word, 8);
                if (!is_plausible_ptr_e(word) || r == 0)
                    word = 0;
                if (remote_child_link_valid(word, button) != 0) {
                    char label[16];

                    strcpy(label, first ? "PLAY AGAIN" : "EXIT");
                    ((void (*)(char *, const char *))(
                        (uint64_t)g_proc_mem_bias + 0x66ae58))(label,
                                                               label);
                    ((void (*)(uint64_t, char *, int))(
                        (uint64_t)g_proc_mem_bias + 0x88836c))(button, label,
                                                               1);
                    ((void (*)(char *))(
                        (uint64_t)g_proc_mem_bias + 0x66ad48))(label);
                    widget_set_scale(button, 1.25f /* 0x3fa00000 */,
                                     1.25f /* 0x3fa00000 */);
                    ((void (*)(uint64_t, uint64_t, uint64_t))(
                        (uint64_t)g_proc_mem_bias + 0x595314))(
                            0xc61c3c00, 0xc61c3c00, button);
                    ((void (*)(uint64_t, uint64_t))(
                        (uint64_t)g_proc_mem_bias + 0x5988ec))(
                            g_launcher_stage_root, button);
                    if (remote_child_link_valid(button, g_overlay_parent) != 0) {
                        g_capture_subject = g_overlay_parent;
                        *slot_p = button;
                        goto position;
                    }
                }
            }
        }

button_failed:
        g_capture_flags = ((uint64_t)(uint32_t)g_capture_flags << 32) | 1;
        if (g_stage_log_count < 0x80) {
            g_stage_log_count = g_stage_log_count + 1;
            if (clock_gettime(0 /* CLOCK_REALTIME */, &now) == 0)
                word = (uint64_t)now.tv_sec * 1000
                       + (uint64_t)now.tv_nsec / 1000000;
            else
                word = 0;
            __android_log_print(4, "NexusLab69252",
                "{\"stage\":\"%s\",\"reason\":\"%s\",\"action\":%u,"
                "\"pid\":%d,\"tid\":%d,\"time_ms\":%llu}",
                "script_port_result_button_unavailable",
                "owned_gamebutton_contract", (uint32_t)i, getpid(),
                (int)gettid(), word);
        }
        goto next;

position:
        /* PLAY AGAIN parks left of the field grid, EXIT right of center */
        x = base_x - 95.0f;   /* raw: the +0x44 anchor minus 95.0 */
        if (!first)
            x = 95.0f;
        ((void (*)(float, float, uint64_t))(
            (uint64_t)g_proc_mem_bias + 0x595314))(
                (x - field_x) / density,
                (88.0f - field_y) / density, button);
        g_capture_flags = g_capture_flags | ((uint64_t)bit << 32);

next:
        if (!first)
            return;
    }
}
/* widgets chain 3 chunk 9: covers raw lines 3606-4384 (nexus_rich_plan, whole — the 0x1b810 view filler) */

/* ===== cross-file functions ===== */

extern int rich_text_layout(float x, float y, float font_size, float scale,
                            uint32_t *view, const char *text, int bright,
                            uint32_t cell_y, uint32_t max_rows,
                            uint64_t position);
                                      /* renderer.c @ 0016b76c — the raw call
                                       * sites carry 10 args and consume the
                                       * verdict; renderer.c's own def folds
                                       * arg2/4 and drops the return —
                                       * coordinator reconciles */
extern void plus_field_text_fill(char *buf, size_t cap, ...);
                                      /* misc.c @ 0016ba38 — __vsnprintf_chk
                                       * trampoline; the format travels in the
                                       * variadic registers Ghidra dropped */
extern long rich_label_metric_index(const char *label);
                                      /* external @ 0016bae0 — not in dump:
                                       * maps a label to its metric-table
                                       * index (>= 0), -1 when unknown */
extern float font_scale_for_char(float base_size, char c);
                                      /* misc.c @ 0016cca0 — the raw call
                                       * sites only recover base 1.4; the
                                       * char rides in w1 */
extern int nexus_rich_plus_layout(uint32_t *view, int *state, void *ctx);
                                      /* activation_plus.c @ 0016bc94 */
extern char *__strchr_chk(const char *s, int c, size_t s_len);

/* ===== rich-view stage descriptor (param_3) ===== */

/* 0x28-byte stage header + up to 0x1a entries of 0x40 bytes at +0x28. */
typedef struct {
    uint32_t magic;        /* +0x00 */
    uint32_t entry_count;  /* +0x04 — <= 0x1a */
    char     class_char;   /* +0x08 — the view class letter */
    uint8_t  no_footer;    /* +0x09 — 1 = deactivate every row at the end */
    uint8_t  filtered;     /* +0x0a — 1 = skip the page-descriptor lookup */
    uint8_t  _pad0b[0x1d];
    struct {
        uint32_t word0;    /* +0x00 — 0 marks a placeholder; else the entry id */
        uint32_t _pad04;
        uint64_t _pad08;
        float    grid_x;   /* +0x10 — 315.0-family x */
        float    grid_y;   /* +0x14 — 100.0/516.0-family y */
        uint64_t _pad18;
        uint8_t  skip;     /* +0x20 — 1 + grid_y 516.0 = skip the entry */
        uint8_t  _pad21[3];
        uint32_t field_24; /* +0x24 */
        uint32_t id2;      /* +0x28 */
        char    *title;    /* +0x2c? — the raw reads *(char **)(entry + 0x28) */
        uint32_t word_2c;  /* +0x30 — value cell flag (id 0x45..0x48 = ms) */
        uint32_t word_30;  /* +0x34 — value present flag */
        uint32_t word_34;  /* +0x38 — 0x30/0x31/0x32 special kinds */
        uint8_t  kind;     /* +0x3c — text class */
    } entries[0x1a];
} rich_stage_info_e_t;

/* ===== the 0x1b810-byte rich view ===== */

#define RICH_VIEW_SIZE     0x1b810u
#define RICH_VIEW_ROW_MAX  0x280u   /* 640 rows hard cap (raw 0x27f/0x281) */

/*
 * nexus_rich_plan — fill the 0x1b810-byte rich view for one frame.
 * Gates: stage info and view non-NULL, width in [960, 8192], height in
 * [540, 8192] (both finite), entry count <= 0x1a; anything else returns 4
 * with the view untouched.
 *
 * The view is zeroed, then the header pair of rows (title bar: the
 * 0x10f860/0x10f910/0x10f950/0x10f958/0x10f990/0x10f998/0x10fb20/0x10fb28
 * templates) is laid around the scaled screen center — scale =
 * min(width/1024, height/576), center = scale*0x10f7a0/0x10f7a8 + (dim +
 * scale*0x10f880)/2. Six category-tab rows follow (tab ids 0x13cdb0,
 * y = 146*i - 456, the tab matching the view class highlighted through
 * 0x10f920/0x10f848), a separator row, the class-scale bar (1..5 by class
 * char, y = 146*scale - 448 + 0.02057143, width 76.923) and the six
 * "MAIN PAGE" title rows (font 1.05..1.18 by label length, x = 493.0,
 * y = 146*i - 25.0). Three rich_text_layout calls paint "NEXUS MENU"
 * (-549.0, 64.0, 2.25, 9 rows), the 0x1384c3 subtitle (-306.0, 64.0,
 * 2.25, 1 row) and "t.me/NexusBrawl" (-528.0, 111.0, 0.85, 15 rows).
 *
 * Then the stage entries: up to 20 are staged (skip flag + 516.0 y, and
 * the 315.0/100.0 placeholder pair). Each becomes one interactive row
 * (kind 2, flags from the page-descriptor table at 0x19b5e8 — matched by
 * class char + entry id — or from the entry's own words; placeholders
 * take the 5-column grid x = 175*(col) - 440, y = 110*row + 178, y' =
 * 0.8*(idx/5) + y - 2.4). Known entries fill their value cell at +0x2e
 * through plus_field_text_fill — ids 0x45..0x48 format the +0x2b word as
 * seconds/1000 — and 0x1b (27) pad rows close each entry. Unknown entries
 * (no descriptor) lay their title (auto-scaled to 207.2/width, min-scale
 * 1.4 when the width <= 148) and description (font 1.15..1.7 by length,
 * the 0x19b490 offset pair, color 0xffa9adb6); placeholders deactivate
 * the rows they allocated. The 'Y' class appends the Nexus+ tab through
 * nexus_rich_plus_layout (state seeded {0x10f8a0, -1}); the footer flag
 * finally deactivates every row when the stage says no footer. Returns 1
 * on success, 4 on any gate/overflow. @ 0016a3ec
 */
int nexus_rich_plan(float width, float height, const void *stage_info,
                    uint64_t frame, uint32_t *view)
{
    const rich_stage_info_e_t *stage = stage_info;
    const void *entry;
    const void *entries[0x14];
    const rich_page_desc_t *desc;
    char title_buf[0x200];
    char desc_buf[0x200];
    uint64_t metric_off;
    long metric;
    float scale, cx, cy;
    float entry_x, entry_y, y;
    uint32_t row_idx, id, flags, tab_match;
    uint32_t i, n;
    size_t len;
    int r;

    if (stage_info == NULL || view == NULL
        || !isfinite(width) || isnan(width)
        || 0x1a < stage->entry_count
        || !(height <= 8192.0f)
        || !(width <= 8192.0f)
        || height < 540.0f
        || width < 960.0f
        || !isfinite(height))
        return 4;

    memset(view, 0, RICH_VIEW_SIZE);

    /* --- header pair: title bar around the scaled screen center --- */
    scale = fminf(width * 0.0009765625f /* 1/1024 */, height / 576.0f);
    row_idx = RICH_E_COUNT(view);
    RICH_E_COUNT(view) = row_idx + 2;
    {
        rich_row_e_t *r0 = RICH_E_ROW(view, row_idx);
        rich_row_e_t *r1 = RICH_E_ROW(view, row_idx + 1);

        *(uint64_t *)(void *)&r0->kind = RICH_HDR_KIND;            /* 0x10f860 */
        r0->active = 1;
        *(uint64_t *)(void *)&r0->font_size = RICH_HDR_FIELD2C;     /* 0x10f958 */
        *(uint64_t *)(void *)&r0->_r24[0] = RICH_HDR_FIELD24;      /* 0x10f950 */
        *(uint64_t *)(void *)&r1->_r24[0] = RICH_TAB_ICON_C;       /* 0x10fb20 */
        *(uint64_t *)(void *)&r1->font_size = RICH_TAB_ICON_D;     /* 0x10fb28 */
        *(uint64_t *)(void *)&r1->kind = RICH_TAB_ICON_A;          /* 0x10f990 */
        *(uint64_t *)(void *)&r1->x = RICH_TAB_ICON_B;             /* 0x10f998 */
        r1->active = 1;
        *(uint64_t *)(void *)&r1->field_10 = RICH_HDR_FIELD1C;      /* 0x10f910 */
    }
    view[1] = (uint32_t)scale;
    {
        float la = (float)(uint32_t)(uint64_t)RICH_CENTER_A;   /* 0x10f7a0 lo */
        float lb = (float)(uint32_t)(RICH_CENTER_A >> 32);
        float ca = (float)(uint32_t)(uint64_t)RICH_SCALE_CENTER;  /* 0x10f880 lo */
        float cb = (float)(uint32_t)(RICH_SCALE_CENTER >> 32);

        cx = la * scale + (width + ca * scale) * 0.5f;
        cy = lb * scale + (height + cb * scale) * 0.5f;
        *(uint64_t *)(void *)((char *)view + 8) =
            ((uint64_t)(uint32_t)cy << 32) | (uint32_t)cx;
    }
    RICH_E_ROW(view, row_idx)->field_10 = 0;   /* the raw clears +0x34 rel */

    /* --- six category tabs --- */
    for (i = 0; i < 6; i++) {
        row_idx = RICH_E_COUNT(view);
        if (row_idx < RICH_VIEW_ROW_MAX) {
            rich_row_e_t *r = RICH_E_ROW(view, row_idx);

            RICH_E_COUNT(view) = row_idx + 1;
            *(uint64_t *)(void *)&r->kind = RICH_TAB_KIND;         /* 0x10f7b0 */
            *(uint64_t *)(void *)&r->_r24[0] = RICH_TAB_TEMPLATE;  /* 0x10f840 */
            r->font_size_2 = 1.12f /* 0x3f8f5c29 */;
            r->active = 1;
            r->y = fmaf((float)i, 146.0f /* 0x43120000 */,
                        -456.0f /* 0xc3e40000 */);
            *(uint64_t *)(void *)&r->font_size = RICH_HDR_FIELD1C;  /* 0x10f910 */
            *(uint64_t *)(void *)&r->_r24[0] = RICH_TAB_XY;        /* 0x10f8e0 */
            r->_r08 = (uint32_t)g_rich_tab_ids[i];   /* 0x13cdb0 */
        } else {
            return 4;
        }

        /* the tab matching the view class is highlighted */
        {
            char cls = stage->class_char;

            if (__strchr_chk("CASFBTX", cls, 8) != NULL)
                tab_match = 1;
            else if (__strchr_chk(g_class_set_a, cls, 4) != NULL)
                tab_match = 2;
            else if (cls == 'U')
                tab_match = 3;
            else if (__strchr_chk("OQKDZGNPJL", cls, 0xb) != NULL)
                tab_match = 4;
            else if (cls == 'Y')
                tab_match = 5;
            else
                tab_match = 0;
        }
        if (i == tab_match) {
            rich_row_e_t *r = RICH_E_ROW(view, row_idx);
            float fx = (float)(uint32_t)(uint64_t)RICH_TAB_SEL_XY;      /* 0x10f848 lo */
            float fy = (float)(uint32_t)(RICH_TAB_SEL_XY >> 32);

            *(uint64_t *)(void *)&r->kind = RICH_TAB_SEL_KIND;     /* 0x10f920 */
            r->x = r->x + fx;
            r->y = r->y + fy;
        }
    }

    /* --- separator row --- */
    row_idx = RICH_E_COUNT(view);
    RICH_E_COUNT(view) = row_idx + 1;
    {
        rich_row_e_t *r = RICH_E_ROW(view, row_idx);

        *(uint64_t *)(void *)&r->_r24[0] = RICH_SEP_FIELD24;   /* 0x10faa8 */
        *(uint64_t *)(void *)&r->font_size = RICH_SEP_FIELD1C; /* 0x10faa0 */
        *(uint64_t *)(void *)&r->kind = RICH_HDR_KIND;         /* 0x10f860 */
        r->active = 1;
        *(uint64_t *)(void *)&r->font_size_2 = RICH_HDR2_FIELD1C;  /* 0x10f878 */
    }

    /* --- the class-scale bar (1..5 by class char) --- */
    {
        char cls = stage->class_char;
        float class_scale;

        if (__strchr_chk("CASFBTX", cls, 8) != NULL)
            class_scale = 1.0f /* 0x3f800000 */;
        else if (__strchr_chk(g_class_set_a, cls, 4) != NULL)
            class_scale = 2.0f /* 0x40000000 */;
        else if (cls == 'U')
            class_scale = 3.0f /* 0x40400000 */;
        else if (__strchr_chk("OQKDZGNPJL", cls, 0xb) != NULL)
            class_scale = 4.0f /* 0x40800000 */;
        else if (cls == 'Y')
            class_scale = 5.0f /* 0x40a00000 */;
        else
            class_scale = 0.0f;

        row_idx = RICH_E_COUNT(view);
        y = fmaf(class_scale, 146.0f, -448.0f);
        RICH_E_COUNT(view) = row_idx + 1;
        {
            rich_row_e_t *r = RICH_E_ROW(view, row_idx);

            *(uint64_t *)(void *)&r->kind = RICH_CLASSBAR_KIND;    /* 0x10f728 */
            *(uint64_t *)(void *)&r->_r24[0] = RICH_CLASSBAR_B;    /* 0x10fb78 */
            *(uint64_t *)(void *)&r->font_size = RICH_CLASSBAR_A;  /* 0x10fb70 */
            r->active = 1;
            r->x = y + 0.02057143f;
            r->y = 76.92308044f /* 0x4299d89e */;
            r->color = 0xffffffff;
            r->field_10 = 0;
        }
    }

    /* --- the six MAIN PAGE title rows --- */
    for (i = 0; i < 6; i++) {
        const char *s = g_main_page_titles[i];   /* 0x19b450 */
        float font;

        len = strlen(s);
        if (len < 7)
            font = 1.05f /* 0x3f866666 */;
        else if (len == 7)
            font = 1.08f /* 0x3f8a3d71 */;
        else {
            font = 1.12f /* 0x3f8f5c29 */;
            if (len != 8)
                font = 1.18f /* 0x3f970a3d */;
        }
        len = strlen(s);
        if (0x7f < len)
            return 4;
        row_idx = RICH_E_COUNT(view);
        if (0x27f < row_idx)
            return 4;
        {
            rich_row_e_t *r = RICH_E_ROW(view, row_idx);

            *(uint64_t *)(void *)&r->kind = RICH_ROW_TEMPLATE;  /* 0x10f7e0 */
            RICH_E_COUNT(view) = row_idx + 1;
            r->x = 493.0f /* 0x43f68000 */;
            r->active = 1;
            r->color = 0xffffffff;
            r->field_10 = 0;
            r->font_size = font;
            r->font_size_2 = font;
            r->y = fmaf((float)i, 146.0f, -25.0f);
            strcpy(r->text, s);
            r->_r08 = 0;
        }
    }

    /* --- the three headline text blocks --- */
    if (rich_text_layout(-549.0f /* 0xc4094000 */, 64.0f, 2.25f, 1.0f, view,
                         "NEXUS MENU", 1, 0, 9, frame) == 0)
        return 4;
    if (rich_text_layout(-306.0f /* 0xc3990000 */, 64.0f, 2.25f, 1.0f, view,
                         g_rich_subtitle, 1, 0, 1, frame) == 0)
        return 4;
    if (rich_text_layout(-528.0f /* 0xc4040000 */, 111.0f, 0.85f, 1.0f, view,
                         "t.me/NexusBrawl", 1, 1, 0xf, frame) == 0)
        return 4;

    /* --- the interactive rows: two anchor rows then the stage entries --- */
    row_idx = RICH_E_COUNT(view);
    {
        rich_row_e_t *r = RICH_E_ROW(view, row_idx);

        RICH_E_COUNT(view) = row_idx + 2;
        *(uint64_t *)(void *)&r->font_size = RICH_FORM_FIELD24;   /* 0x10faf0 */
        *(uint64_t *)(void *)&r->_r24[0] = RICH_FORM_FIELD1C;     /* 0x10faf8 */
        *(uint8_t *)((char *)view + 0x45b) = 0;   /* raw: +0x45b clear */
        r->field_10 = 0;
        *(uint64_t *)(void *)&r->kind = RICH_FORM_FONT_B;         /* 0x10fab8 */
        *(uint64_t *)(void *)&r->_r08 = 0;
        r->active = 1;
        *(uint64_t *)(void *)&r->color = RICH_FORM_FONT_A;        /* 0x10fab0 */
    }
    if (row_idx < 0x27f) {
        rich_row_e_t *r = RICH_E_ROW(view, row_idx + 1);

        *(uint64_t *)(void *)&r->font_size = RICH_PLACE_FONT_B;   /* 0x10f988 */
        *(uint64_t *)(void *)&r->_r24[0] = RICH_PLACE_FONT_A;     /* 0x10f980 */
        *(uint64_t *)(void *)&r->kind = RICH_ROW_TEMPLATE;        /* 0x10f7e0 */
        r->active = 1;
        *(uint16_t *)(void *)r->text = 0x58;    /* 'X' + NUL at +0x3e */
        r->color = 0xffffffff;
        r->field_10 = 0;
    }

    /* stage the entries: the skip flag (+0x20 byte with grid_y 516.0) and
     * the raw sentinel triple (word0 == 0x1600-float, grid 315/100) drop
     * an entry; a staged entry with a zero leading word is a placeholder */
    memset(entries, 0, sizeof entries);
    n = stage->entry_count;
    if (n != 0) {
        uint32_t staged = 0;
        const char *p = (const char *)stage + 0x28;

        do {
            const float *grid = (const float *)(p + 0x10);
            float word0 = *(const float *)(const void *)p;

            if (*(const char *)(p + 0x20) == 1 && grid[1] == 516.0f) {
                /* skipped entry */
            } else if (word0 == 1.82169e-43f /* bits 0x1600 */
                       && grid[0] == 315.0f && grid[1] == 100.0f) {
                /* the raw's sentinel triple: not staged */
            } else {
                if (staged == 0x14)
                    return 4;
                entries[staged++] = word0 != 0.0f ? (const void *)p : NULL;
            }
            p += 0x40;
            n--;
        } while (n != 0);
    }

    /* --- the entry loop --- */
    for (i = 0; i < 0x14; i++) {
        entry = entries[i];
        desc = NULL;
        if (entry == NULL) {
            /* placeholder: 5-column grid position */
            uint32_t col = (i & 0xff) / 5;
            uint32_t in_row = (i + col * -5) & 0xff;

            id = 0;
            flags = 0;
            entry_x = fmaf((float)in_row, 175.0f /* 0x432f0000 */,
                           -440.0f /* 0xc3dc0000 */);
            entry_y = fmaf((float)col, 110.0f /* 0x42dc0000 */,
                           178.0f /* 0x43320000 */);
        } else {
            /* page-descriptor lookup by class char + entry id */
            if (stage->filtered == 0) {
                size_t k;

                for (k = 0; k < 0xa60; k += 0x20) {
                    if (*(const uint32_t *)((const char *)g_rich_page_table
                                            + k) == (uint8_t)stage->class_char
                        && *(const uint32_t *)((const char *)g_rich_page_table
                                               + k + 4)
                               == *(const uint32_t *)entry) {
                        desc = (const rich_page_desc_t *)(
                            (const char *)g_rich_page_table + k);
                        break;
                    }
                }
            }

            {
                const uint32_t *e = (const uint32_t *)entry;
                float *ef = (float *)entry;

                if (e[10] == 0 || (char)e[0xd] == 0)
                    flags = 0;
                else
                    flags = (uint32_t)(e[0xc] == 0) << 1;
                if (desc != NULL && desc->owner == 0)
                    flags = desc->flags;
                entry_x = ef[6];
                entry_y = ef[7];
                if ((char)e[0xe] == 0) {
                    id = e[0];
                    if (id == 0x30)
                        flags = 2;
                    else if (id == 0x32)
                        flags = 3;
                    else if (id == 0x31)
                        flags = 1;
                } else {
                    id = e[0];
                }
            }
        }

        /* the interactive row itself */
        row_idx = RICH_E_COUNT(view);
        if (row_idx < RICH_VIEW_ROW_MAX) {
            rich_row_e_t *r = RICH_E_ROW(view, row_idx);

            RICH_E_COUNT(view) = row_idx + 1;
            r->kind = 2;
            r->flags = flags;
            r->x = entry_x;
            r->field_10 = (uint32_t)(flags == 0);
            r->active = 1;
            r->y = fmaf((float)(i / 5), 0.8f /* 0x3f4ccccd */,
                        entry_y - 2.4f);
            r->color = 0xffffffff;
            r->_r08 = entry != NULL ? id : 0;
            *(uint64_t *)(void *)&r->_r24[0] = RICH_ROW_FIELD1C;   /* 0x10f7c0 */
        } else {
            return 4;
        }

        if (desc != NULL) {
            /* known entry: position + value cell */
            rich_row_e_t *r = RICH_E_ROW(view, row_idx);

            r->x = entry_x;
            r->y = entry_y;
            *(uint64_t *)(void *)&r->_r24[0] = RICH_CLASSBAR_D;   /* 0x10fb98 */
            *(uint64_t *)(void *)&r->font_size = RICH_CLASSBAR_C; /* 0x10fb90 */

            if (*(const int *)((const char *)desc + 8) == 0
                || *(const char *)((const char *)entry + 0x35) == 0) {
                /* plain text fill (the raw's common tail) */
                plus_field_text_fill(r->text, (size_t)-1);
            } else {
                char *cell = r->text;

                switch (id) {
                case 0x37: case 0x42: case 0x6a: case 0x92:
                case 0x70: case 0x74:
                    /* plain in-place text */
                    plus_field_text_fill(cell, (size_t)-1);
                    break;
                case 0x45: case 0x46: case 0x47: case 0x48:
                    /* milliseconds -> seconds value (the double rides the
                     * variadic registers the raw could not recover) */
                    plus_field_text_fill(
                        cell, (size_t)-1,
                        (double)(int)((const uint32_t *)entry)[0xb] / 1000.0);
                    break;
                case 0x3e: case 0x3f: case 0x98: case 0x99:
                case 0x75: case 0x76: case 0x77: case 0x78:
                case 0x89: case 0x8a: case 0x8f: case 0x90:
                case 0x8b: case 0x8c: case 0x8d: case 0x8e:
                    /* filled value cells */
                    plus_field_text_fill(cell, (size_t)-1);
                    break;
                default:
                    /* 0x38..0x3d, 0x4b..0x5b, 0x6e, 0x6f, 0x73, 0x9b:
                     * the common fill after the switch */
                    plus_field_text_fill(cell, (size_t)-1);
                    break;
                }
            }

            /* 0x1b (27) pad rows close the entry */
            {
                uint32_t count = RICH_E_COUNT(view);
                int pad = count < 0x281 ? 0x280 - (int)count : 0;
                uint64_t off = (uint64_t)count * 0xb0;
                int k;

                for (k = 0; k < 0x1b; k++) {
                    if (pad == k)
                        return 4;
                    RICH_E_COUNT(view) = count + 1 + (uint32_t)k;
                    *(uint64_t *)((char *)view + off + 0x10) =
                        *(const uint64_t *)(const void *)&RICH_ROW_TEMPLATE;
                    *(uint64_t *)((char *)view + off + 0x2c) =
                        RICH_ROW_PAD_FONT;                     /* 0x10f9b8 */
                    *(uint64_t *)((char *)view + off + 0x24) =
                        RICH_ROW_PAD_XY;                       /* 0x10f9b0 */
                    *(uint8_t *)((char *)view + off + 0x3e) = 0;
                    *(uint64_t *)((char *)view + off + 0x1c) = 0xffffffff;
                    *(uint8_t *)((char *)view + off + 0x3c) = 0;
                    off += 0xb0;
                }
            }
        } else {
            /* unknown entry: title + description */
            const char *label;
            float text_scale, gap, glyph_w, total_w;
            float font;
            bool metric_ok = false;

            metric_off = 0;
            if (entry == NULL) {
                metric_ok = false;
                metric = -1;
            } else {
                label = *(const char *const *)((const char *)entry + 0x28);
                if (label == NULL || 0x7f < strlen(label))
                    return 4;
                plus_field_text_fill(title_buf, 0x80);
                {
                    char *nl = __strchr_chk(title_buf, 10, 0x80);

                    if (nl != NULL)
                        *nl = 0;
                }
                plus_field_text_fill(desc_buf, 0x80);
                if (id - 0x21037 < 0xffffffc9    /* the 0x21037..0x21048 band */
                    && (uint8_t)((const uint32_t *)entry)[0xe] < 8
                    && (1u << (((const uint32_t *)entry)[0xe] & 0x1f) & 0xd8u) != 0)
                    plus_field_text_fill(desc_buf, 0x80);
                if (stage->class_char == 'R') {
                    if (stage->filtered == 0) {
                        metric = rich_label_metric_index(label);
                        if (metric < 0) {
                            metric_ok = false;
                        } else {
                            plus_field_text_fill(desc_buf, 0x80);
                            metric_ok = true;
                        }
                    } else {
                        metric_ok = false;
                        metric = -1;
                    }
                } else {
                    metric_ok = false;
                    metric = -1;
                }
            }

            /* measure the title: spaces 7.0, glyphs 1.4-scaled widths */
            total_w = 0.0f;
            {
                bool emitted = false;
                float space_run = 0.0f;
                uint32_t k = 0;

                for (;;) {
                    for (; title_buf[k] == ' '; k++)
                        space_run = space_run + 7.0f;
                    if (title_buf[k] == '\0')
                        break;
                    gap = 0.0f;
                    if (emitted)
                        gap = space_run + 4.2f;
                    glyph_w = font_scale_for_char(1.4f /* 0x3fb33333 */,
                                                   title_buf[k]);
                    space_run = 0.0f;
                    total_w = total_w + gap + glyph_w;
                    emitted = true;
                    k = k + 1;
                }
            }
            text_scale = 207.2f / total_w;
            if (total_w <= 148.0f)
                text_scale = 1.4f;

            r = rich_text_layout(0.78f /* 0x3f47ae14 */,
                                 entry_x - 55.0f,
                                 entry_y - 27.0f, text_scale, view,
                                 title_buf, 0, i, 0x1a, frame);
            if (r == 0)
                return 4;

            /* the description row */
            metric_off = 0;
            if (metric_ok)
                metric_off = *(const uint64_t *)(
                    (const char *)g_rich_desc_offsets
                    + (size_t)metric * 0x18);
            len = __strlen_chk(desc_buf, 0x80);
            if (len < 0xc)
                font = 1.15f /* 0x3f933333 */;
            else if (len < 0xf)
                font = 1.3f /* 0x3fa66666 */;
            else {
                font = 1.5f /* 0x3fc00000 */;
                if (0x12 < len)
                    font = 1.7f /* 0x3fd9999a */;
            }
            len = __strlen_chk(desc_buf, 0x80);
            if (len < 0x80) {
                row_idx = RICH_E_COUNT(view);
                if (row_idx < 0x280) {
                    rich_row_e_t *r2 = RICH_E_ROW(view, row_idx);
                    float ox = (float)(uint32_t)(uint64_t)metric_off;
                    float oy = (float)(uint32_t)(metric_off >> 32);

                    RICH_E_COUNT(view) = row_idx + 1;
                    *(uint64_t *)(void *)&r2->kind = RICH_ROW_TEMPLATE;
                    r2->active = 1;
                    r2->color = 0xffffffff;
                    r2->field_10 = 0;
                    r2->font_size = font;
                    r2->font_size_2 = font;
                    r2->x = entry_x - 55.0f + ox;
                    r2->y = entry_y - 10.0f + oy;
                    strcpy(r2->text, desc_buf);
                    r2->_r08 = 0;
                    r2->color = 0xffa9adb6;

                    /* placeholders deactivate the rows they allocated */
                    if (entry == NULL) {
                        uint32_t now_count = RICH_E_COUNT(view);

                        if (row_idx < now_count) {
                            uint32_t k;

                            for (k = row_idx; k < now_count; k++)
                                RICH_E_ROW(view, k)->active = 0;
                        }
                    }
                } else {
                    return 4;
                }
            } else {
                return 4;
            }
        }
    }

    /* --- the 'Y' class appends the Nexus+ tab --- */
    if (stage->class_char == 'Y' && stage->filtered == 0) {
        uint64_t state[16];

        memset(state, 0, sizeof state);
        state[0] = RICH_PLUS_STATE_LO;    /* 0x10f8a0 */
        state[1] = 0xffffffffffffffffULL;
        if (nexus_rich_plus_layout(view, (int *)state, (void *)frame) != 1)
            return 4;
    }

    /* --- the footer flag deactivates every row --- */
    if (stage->no_footer == 0) {
        uint32_t count = RICH_E_COUNT(view);

        if (count != 0) {
            uint32_t k;

            for (k = 0; k < count; k++)
                RICH_E_ROW(view, k)->active = 0;
            return 1;
        }
    }
    return 1;
}
/* widgets chain 3 chunk 10: covers raw lines 4386-4679 (hud_camera_settings_render; the next def @4680 belongs to chain 2) */

/* ===== cross-file functions ===== */

extern uint64_t script_port_camera_snapshot(void *out);
                                      /* misc.c @ 00191bf4 — 0x38-byte camera
                                       * snapshot (seed 0x10f8e8), 0 unbound */
extern uint64_t script_port_camera_control(uint32_t op, uint32_t a,
                                           uint32_t b);  /* misc.c @ 00191c34 */
extern int widget_child_link_valid(uint64_t node, uint64_t parent);
                                      /* misc.c @ 0016d5cc */
extern int ui_scene_fonts_ready(void); /* misc.c @ 0016d910 */
extern void widget_set_position(uint64_t widget, float x, float y);
                                      /* misc.c @ 0016cf5c — engine 0x595314
                                       * (raw shows the Ghidra float-merge
                                       * order (x, y, widget)) */
extern void widget_set_scale(uint64_t widget, float sx, float sy);
                                      /* misc.c @ 0016cf00 — engine 0x595344 */
extern void widget_set_font_size(uint64_t widget, float size);
                                      /* misc.c @ 0016cf70 */
extern void widget_set_text(uint64_t widget, uint64_t text_obj);
                                      /* misc.c @ 0016cec0 — engine 0x88836c */
extern uint64_t ui_string_object(const char *text);  /* misc.c @ 0017b858 */
extern void ui_text_format(char *buf, size_t cap_a, size_t cap_b,
                           const char *fmt, ...);
                                      /* misc.c @ 00176a24 — __vsnprintf_chk
                                       * trampoline (fonts.c externs the
                                       * 6-arg form; the raw call sites are
                                       * variadic) */
extern void ui_cached_widgets_hide(void);            /* misc.c @ 0017b3f0 */
extern int ui_uv_rect_compute(float w, float h, float mul_a, float mul_b,
                              float out[4]);         /* misc.c @ 0017b530 */
extern int widget_set_frame(uint64_t widget, float x, float y,
                            float w, float h);       /* misc.c @ 0017b5e8 */
extern void rich_panel_label_render(float x, float y, float font,
                                    void *record, const char *text);
                                      /* misc.c @ 0017b920 */
extern int rich_panel_widget_slot_check(uint32_t index);
                                      /* misc.c @ 0017bb9c — slider slot
                                       * validity (the d-chain chunk 5 externs
                                       * this as camera_slider_ownership_check;
                                       * coordinator reconciles) */
extern int hud_camera_settings_build(void);
                                      /* this file (chain 2 chunk 5) @ 0017a478 */

/* ===== host service table (misc.c) ===== */

extern void *g_host_ctx;             /* 0x1a7cc8 */
extern uint64_t g_game_base;         /* 0x1a7cd0 */
extern int (*g_host_read)(void *ctx, uint64_t addr, void *out, uint32_t len);
                                     /* 0x1a7ce8 */
extern int (*g_host_style)(void *ctx, const char *name);  /* 0x1a7cf8 */
extern void (*g_host_log)(void *ctx, const char *cat, const char *event,
                          uint32_t value);                 /* 0x1a7d00 */
extern uint64_t g_ui_scene_root;     /* 0x1a7d28 */
extern uint64_t g_ui_context;        /* 0x1a7d30 */

/* ===== rich panel block slots (renderer.c-owned) ===== */

extern uint64_t g_rich_panel_root;       /* 0x283338 — owner UI context */
extern uint64_t g_rich_panel_ctx;        /* 0x283340 — panel ctx (kind 3) */
extern uint64_t g_rich_panel_ctx_slot;   /* 0x283348 — kind-0 slot (canvas) */
extern uint64_t g_rich_panel_k2_slots[5];/* 0x283350 — slider widget slots */
extern uint64_t g_rich_panel_page_ctx;   /* 0x283980 — built flag (kind 4) */

/* rich panel kind-1 rows: {widget, extra} at 0x78 stride from 0x2833a0
 * (row 0 = CAMERA title, row 1 = panel canvas, rows 2-5 = option rows) */
typedef struct {
    uint64_t widget;   /* +0x00 */
    uint64_t extra;    /* +0x08 */
    uint64_t _pad[13]; /* +0x10..0x77 */
} panel_row_t;

extern panel_row_t g_rich_panel_rows_a[6];  /* 0x2833a0 */

/* ===== camera panel state (widgets.c-owned) ===== */

uint32_t g_camera_scope_mask;        /* 0x22d81c — cached scope-API gate mask */
uint64_t g_camera_screen_id;         /* 0x283940 — snapshot owning screen id */
uint64_t g_camera_slider_tap_ms[5];  /* 0x283950 — per-slider tap stamps */
uint64_t g_camera_frame_stamp;       /* 0x283948 — last rendered frame */
uint32_t g_camera_build_failed;      /* 0x283984 — 1 = native build failed once */
uint64_t g_camera_panel_state;       /* 0x283988 — hi dword: rendered once */
uint32_t g_camera_slider_flags[5];   /* 0x283990 — per-slider drag flags */
uint32_t g_camera_slider_changed_count; /* 0x2839a4 — slider_changed logs (8) */
uint32_t g_camera_style_mask;       /* 0x2839f0 — last waiting-assets style mask */
extern uint32_t g_camera_build_stage;   /* 0x2839a8 — chain 2 chunk 5 */
extern uint32_t g_camera_log_count;     /* 0x2839f4 — chain 2 chunk 4 (misc.c
                                         * names this address g_camera_guards_
                                         * count — coordinator reconciles) */
float g_camera_view_w;               /* 0x2839ac — last render width */
float g_camera_view_h;               /* 0x2839b0 — last render height */
float g_camera_view_scale;           /* 0x2839b4 — fit scale */
uint64_t g_camera_snap_cache[7];     /* 0x2839b8 — 0x38-byte snapshot cache */

/* camera label records: {pad, ctx, widget, 0x60 text cache}, 0x78 stride.
 * The title record sits at block+0x338 and the five slider labels at
 * block+0x3b0 + i*0x78 (misc.c's g_rich_panel_rows_b addresses are 8 higher
 * — the raw's label records are the authoritative layout). */
typedef struct {
    uint64_t _pad0;     /* +0x00 */
    uint64_t ctx;       /* +0x08 — panel ctx (rich_panel_label_render) */
    uint64_t widget;    /* +0x10 — label widget */
    char     text[0x60];/* +0x18 — cached text */
} camera_label_rec_t;   /* 0x78 */

camera_label_rec_t g_camera_title_label;    /* 0x283670 */
camera_label_rec_t g_camera_slider_labels[5]; /* 0x2836e8, 0x78 stride */

/* ===== rodata ===== */

extern const uint64_t CAM_SNAPSHOT_SEED;   /* 0x10f8e8 — camera snapshot seed */
extern const char g_camera_zoom_name[];    /* 0x139a93 — zoom slider name */
extern const int32_t g_camera_mode_off_tbl[];   /* 0x13ce4c — mode-name offsets */
extern const int32_t g_camera_slider_off_tbl[];/* 0x13ce5c — slider-name offsets */
extern float g_ui_density;                 /* 0x1e53fc — design-density divisor */
extern float g_ui_center_x;                /* 0x1e5400 */
extern float g_ui_center_y;                /* 0x1e5404 */

/* camera snapshot (script_port_camera_snapshot out-record) */
typedef struct {
    uint64_t magic;       /* +0x00 seed (0x10f8e8) */
    uint32_t status;      /* +0x08 must be nonzero */
    uint32_t flags;       /* +0x0c must be nonzero */
    int32_t  mode;        /* +0x10 camera mode (name-table index) */
    int32_t  values[5];   /* +0x14 slider values ([0] = zoom x100) */
    uint64_t screen_id;   /* +0x28 owning screen id */
    uint64_t field_30;    /* +0x30 */
} camera_snapshot_e_t;    /* 0x38 */

/*
 * hud_camera_settings_render — per-frame camera settings panel pump.
 * A stale panel block (owner context != current) is fully reset (the
 * 0x6b8-byte clear covers 0x283338..0x2839f0). The scope gate folds five
 * bits — snapshot ok, snapshot status, snapshot flags, the stage "+9"
 * closed flag and the native-build-failure latch — into 0x22d81c, logging
 * "scope_api_enabled_live_menu_failed" (cap 0x30) when it changes. The
 * panel needs the whole gate plus fonts-ready and the scene's swapped
 * byte clear; anything else parks the panel (camera_control(5,0,0), state
 * clear, panel ctx parked at (-10000,-10000)).
 *
 * The first frame after the page-list flag is set builds the panel
 * (hud_camera_settings_build; failures latch 0x283984 and log
 * "native_ui_contract_failed" with the build stage; missing styles log
 * "waiting_assets_slider_popup" with the style mask). Rendering: the
 * snapshot is cached at 0x2839b8, the panel scaled to
 * min(w/620, h/440, 1) (density-normalized) and centered; the first
 * render hides the cached widgets and applies the computed UV rect to the
 * CAMERA row, later frames lay out the canvas (popup_generic 600x400),
 * the four option rows, the mode name and the "CAMERA SETTINGS" title,
 * then fade-in (min 0.1) the five sliders: each slot is validated, the
 * +0x140 drag byte and +0xe8 value read, non-dragging sliders forced to
 * the snapshot value, a tap within 200 ms resetting to the default
 * (zoom 100, else 0) through camera_control(1, i, v), the engine slider
 * render called with the fade, the value read back (a drift re-applies
 * camera_control(1, i) and flags "slider_changed", cap 8) and the
 * "%s: %.2fx" / "%s: %d" label rendered at (130, 45i-90) /
 * (-255, 45i-105). Any tap-commit ends with camera_control(5,0,0) and
 * the snapshot is re-cached. @ 0016fc60
 */
void hud_camera_settings_render(float width, float height,
                                const rich_stage_info_e_t *stage,
                                uint64_t frame)
{
    camera_snapshot_e_t snap;
    char label[0x60];
    float uv_rect[4];     /* the raw shares the label scratch; exclusive paths */
    uint8_t swapped = 1;
    uint64_t slider;
    uint32_t mask, style_mask, new_flag;
    int32_t value, readback, target;
    float sw, sh, scale, fade;
    bool snap_ok, value_changed, tap_committed;
    int i;

    /* stale panel block: full reset (0x283338..0x2839f0) */
    if (g_rich_panel_root != 0 && g_rich_panel_root != g_ui_context)
        memset((void *)&g_rich_panel_root, 0, 0x6b8);

    memset(&snap, 0, sizeof snap);
    snap.magic = CAM_SNAPSHOT_SEED;
    snap_ok = script_port_camera_snapshot(&snap) != 0;

    /* scope gate mask: {snapshot, status, flags, stage closed, build failed} */
    mask = (uint32_t)snap_ok
         | ((uint32_t)(snap.status != 0) << 1)
         | ((uint32_t)(snap.flags != 0) << 2)
         | ((uint32_t)(stage->no_footer != 0) << 3)
         | ((uint32_t)(g_camera_build_failed != 0) << 4);
    if (mask != g_camera_scope_mask) {
        uint32_t under_cap = g_camera_log_count < 0x30;

        g_camera_scope_mask = mask;
        g_camera_log_count = g_camera_log_count + 1;
        if (under_cap && g_host_log != NULL)
            g_host_log(g_host_ctx, "script_port_camera",
                       "scope_api_enabled_live_menu_failed", mask);
    }

    if (!snap_ok || snap.status == 0 || snap.flags == 0
        || stage->no_footer != 0) {
        /* disabled: release the panel and park it */
        if (g_camera_panel_state != 0)
            script_port_camera_control(5, 0, 0);
        g_camera_panel_state = 0;
        if (g_rich_panel_page_ctx != 0
            && widget_child_link_valid(g_rich_panel_ctx, g_ui_context) != 0)
            widget_set_position(g_rich_panel_ctx,
                                -10000.0f /* 0xc61c3c00 */,
                                -10000.0f /* 0xc61c3c00 */);
        return;
    }

    if (g_camera_build_failed != 0 || ui_scene_fonts_ready() == 0)
        return;

    if (g_ui_scene_root + 0x19d < 0x1001
        || g_host_read(g_host_ctx, g_ui_scene_root + 0x19c, &swapped, 1) != 1
        || swapped != 0)
        return;

    if (g_rich_panel_page_ctx == 0) {
        /* first frame: the two popup styles must resolve, then build */
        int have_edit = g_host_style(g_host_ctx, "edit_controls_ui") != 0;
        int have_popup = g_host_style(g_host_ctx, "popup_generic") != 0;

        style_mask = (uint32_t)have_edit | ((uint32_t)have_popup << 1);
        if (style_mask == 3) {
            if (hud_camera_settings_build() != 0)
                goto render;
            g_camera_build_failed = 1;
            {
                uint32_t under_cap = g_camera_log_count < 0x30;

                g_camera_log_count = g_camera_log_count + 1;
                if (under_cap && g_host_log != NULL)
                    g_host_log(g_host_ctx, "script_port_camera",
                               "native_ui_contract_failed",
                               g_camera_build_stage);
            }
            return;
        }
        {
            uint32_t expect = style_mask + 1;

            if (g_camera_style_mask != expect) {
                uint32_t under_cap = g_camera_log_count < 0x30;

                g_camera_style_mask = expect;
                g_camera_log_count = g_camera_log_count + 1;
                if (under_cap && g_host_log != NULL)
                    g_host_log(g_host_ctx, "script_port_camera",
                               "waiting_assets_slider_popup", style_mask);
            }
        }
        return;
    }

render:
    if (widget_child_link_valid(g_rich_panel_ctx, g_ui_context) == 0)
        return;

    /* screen change: drop the drag/tap state */
    if (g_camera_screen_id != snap.screen_id) {
        g_camera_panel_state = 0;
        g_camera_screen_id = snap.screen_id;
        memset(g_camera_slider_tap_ms, 0, sizeof g_camera_slider_tap_ms);
        memset(g_camera_slider_flags, 0, sizeof g_camera_slider_flags);
    }

    /* cache the snapshot (0x2839b8) and the view size */
    memcpy(g_camera_snap_cache, &snap, sizeof snap);
    sw = width / g_ui_density;
    sh = height / g_ui_density;
    scale = fminf(fminf(sw / 620.0f, sh / 440.0f), 1.0f);
    g_camera_panel_state |= (uint64_t)1 << 32;   /* rendered once */
    g_camera_view_w = width;
    g_camera_view_h = height;

    if (isfinite(scale) && scale > 0.0f) {
        g_camera_view_scale = scale;
        widget_set_scale(g_rich_panel_ctx, scale, scale);
        widget_set_position(g_rich_panel_ctx,
                            (g_ui_center_x - width * 0.5f) / g_ui_density,
                            (g_ui_center_y - height * 0.5f) / g_ui_density);

        if (g_camera_panel_state == 0) {
            /* first render: hide the cached widgets and frame the title */
            ui_cached_widgets_hide();
            if (ui_uv_rect_compute(width, height, g_ui_density, scale,
                                   uv_rect) != 0)
                widget_set_frame(g_rich_panel_rows_a[0].widget, uv_rect[0],
                                 uv_rect[1], uv_rect[2], uv_rect[3]);
        } else {
            /* steady state: lay out the panel and pump the sliders */
            widget_set_position(g_rich_panel_rows_a[0].widget,
                                -10000.0f /* 0xc61c3c00 */,
                                -10000.0f /* 0xc61c3c00 */);
            widget_set_frame(g_rich_panel_rows_a[1].widget, 0.0f, 0.0f,
                             sw / scale, sh / scale);
            widget_set_font_size(g_rich_panel_rows_a[1].widget,
                                 0.65f /* 0x3f266666 */);
            widget_set_frame(g_rich_panel_ctx_slot, 0.0f, 0.0f,
                             600.0f /* 0x44160000 */,
                             400.0f /* 0x43c80000 */);
            widget_set_frame(g_rich_panel_rows_a[2].widget,
                             264.0f /* 0x43840000 */,
                             -168.0f /* 0xc3280000 */,
                             48.0f /* 0x42400000 */,
                             42.0f /* 0x42280000 */);
            widget_set_frame(g_rich_panel_rows_a[3].widget,
                             -175.0f /* 0xc32f0000 */,
                             161.0f /* 0x43210000 */,
                             150.0f /* 0x43160000 */,
                             42.0f /* 0x42280000 */);
            widget_set_frame(g_rich_panel_rows_a[4].widget, 0.0f,
                             161.0f /* 0x43210000 */,
                             150.0f /* 0x43160000 */,
                             42.0f /* 0x42280000 */);
            widget_set_frame(g_rich_panel_rows_a[5].widget,
                             175.0f /* 0x432f0000 */,
                             161.0f /* 0x43210000 */,
                             150.0f /* 0x43160000 */,
                             42.0f /* 0x42280000 */);
            widget_set_text(g_rich_panel_rows_a[3].widget,
                ui_string_object((const char *)(const void *)(
                    (const uint8_t *)g_camera_mode_off_tbl
                    + g_camera_mode_off_tbl[snap.mode])));
            rich_panel_label_render(-125.0f /* 0xc2fa0000 */,
                                    -169.0f /* 0xc3290000 */,
                                    24.0f /* 0x41c00000 */,
                                    &g_camera_title_label,
                                    "CAMERA SETTINGS");

            fade = 0.0f;
            if (g_camera_frame_stamp - 1 < frame)
                fade = (float)(frame - g_camera_frame_stamp) / 1000.0f;
            fade = fminf(fade, 0.1f /* 0x3dcccccd */);
            g_camera_frame_stamp = frame;

            value_changed = false;
            tap_committed = false;
            for (i = 0; i < 5; i++) {
                if (rich_panel_widget_slot_check((uint32_t)i) == 0)
                    return;

                slider = g_rich_panel_k2_slots[i];
                new_flag = 0;
                if (slider + 0x141 < 0x1001)
                    return;
                value = 0;
                if (g_host_read(g_host_ctx, slider + 0x140, &new_flag, 1) != 1
                    || 1 < new_flag || slider < 0xf18)
                    return;
                if (g_host_read(g_host_ctx, slider + 0xe8, &value, 4) != 1)
                    return;

                target = snap.values[i];
                if (new_flag == 0) {
                    /* idle: force the snapshot value */
                    if (value != target) {
                        *(int32_t *)(uintptr_t)(slider + 0xe8) = target;
                        value = target;
                    }
                    if (g_camera_slider_flags[i] != 0)
                        tap_committed = true;
                } else {
                    /* dragging: a tap inside the 200 ms window resets */
                    if (g_camera_slider_flags[i] == 0) {
                        if (g_camera_slider_tap_ms[i] - 1 < frame
                            && frame - g_camera_slider_tap_ms[i] < 200) {
                            value = (i == 0) ? 100 : 0;
                            *(int32_t *)(uintptr_t)(slider + 0xe8) = value;
                            new_flag = 0;
                            *(uint8_t *)(uintptr_t)(slider + 0x140) = 0;
                            tap_committed = true;
                            script_port_camera_control(1, (uint32_t)i,
                                                       (uint32_t)value);
                            g_camera_slider_tap_ms[i] = 0;
                            snap.values[i] = value;
                            if (new_flag == 0
                                && g_camera_slider_flags[i] != 0)
                                tap_committed = true;
                        } else {
                            g_camera_slider_tap_ms[i] = frame;
                        }
                    }
                }

                g_camera_slider_flags[i] = new_flag;
                /* engine slider render (fade, widget) */
                ((void (*)(float, uint64_t))(
                    (uintptr_t)(g_game_base + 0x889940)))(fade, slider);

                readback = 0;
                if (g_host_read(g_host_ctx, slider + 0xe8, &readback, 4) != 1)
                    return;
                if (readback != snap.values[i]
                    && script_port_camera_control(1, (uint32_t)i,
                                                  0) != 0) {
                    value_changed = true;
                    snap.values[i] = readback;   /* the raw drops arg 3 */
                }

                widget_set_position(slider, 130.0f /* 0x43020000 */,
                                    fmaf((float)i, 45.0f /* 0x42340000 */,
                                         -90.0f /* 0xc2b40000 */));

                if (i == 0)
                    ui_text_format(label, 0x60, 0x60, "%s: %.2fx",
                                   g_camera_zoom_name,
                                   (double)snap.values[0] / 100.0);
                else
                    ui_text_format(label, 0x60, 0x60, "%s: %d",
                                   (const char *)(const void *)(
                                       (const uint8_t *)g_camera_slider_off_tbl
                                       + g_camera_slider_off_tbl[i]),
                                   snap.values[i]);

                rich_panel_label_render(-255.0f /* 0xc37f0000 */,
                                        fmaf((float)i, 45.0f,
                                             -105.0f /* 0xc2d20000 */),
                                        22.0f /* 0x41b00000 */,
                                        &g_camera_slider_labels[i], label);
            }

            if (tap_committed)
                script_port_camera_control(5, 0, 0);
            if (value_changed) {
                uint32_t under8 = g_camera_slider_changed_count < 8;

                g_camera_slider_changed_count =
                    g_camera_slider_changed_count + 1;
                if (under8) {
                    uint32_t under_cap = g_camera_log_count < 0x30;

                    g_camera_log_count = g_camera_log_count + 1;
                    if (under_cap && g_host_log != NULL)
                        g_host_log(g_host_ctx, "script_port_camera",
                                   "slider_changed", 0);
                }
            }

            /* re-cache the (possibly updated) snapshot */
            memcpy(g_camera_snap_cache, &snap, sizeof snap);
        }
    }
}
/* widgets chain 2 chunk 1: covers raw lines 4680-5057 */

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

/* ===== shared host/service globals (misc.c-owned) ===== */

extern void *g_host_ctx;                                          /* 0x1a7cc8 */
extern int (*g_host_read)(void *ctx, uint64_t addr, void *out,
                          uint32_t len);                          /* 0x1a7ce8 */
extern int (*g_host_style)(void *ctx, const char *name);          /* 0x1a7cf8 */
extern void (*g_host_log)(void *ctx, const char *cat, const char *event,
                          uint32_t value);                        /* 0x1a7d00 */
extern uint64_t g_game_base;                                      /* 0x1a7cd0 */
extern uint64_t g_ui_scene_root;                                  /* 0x1a7d28 */
extern uint64_t g_ui_context;                                     /* 0x1a7d30 */
extern uint32_t g_rich_stop_log_count;
                                   /* 0x1a7d20 — capped 0x40 host-log event counter (renderer.c def) */

/* ===== UI metrics (fonts.c/renderer.c) ===== */

extern float g_ui_density;      /* 0x1e53fc — design-density divisor */
extern float g_ui_center_x;     /* 0x1e5400 — UI center x */
extern float g_ui_center_y;     /* 0x1e5404 — UI center y */
extern float g_rich_row_height; /* 0x22d818 — row/overlay scale factor (renderer.c def; misc.c: g_diag_overlay_scale) */

/* ===== rich panel state (renderer.c/misc.c) ===== */

extern uint64_t g_rich_panel_root;        /* 0x283338 — rich panel root widget */
extern uint64_t g_rich_panel_row0_widget; /* 0x2833a0 — first kind-1 row widget (misc.c: g_rich_panel_rows_a[0].widget) */
extern uint64_t g_rich_panel_page_ctx;    /* 0x283980 — rich panel page-list ctx */
extern uint32_t g_rich_panel_open;        /* 0x283988 — panel open flag (lo of the qword) */
extern uint32_t g_rich_panel_built;       /* 0x28398c — panel built flag (hi of the qword) */
extern uint64_t g_rich_chat_rows[0x36];   /* 0x284220 — renderer.c rich_chat_row_t[2]; 0x1b-qword stride, widget first */

/* ===== battle-menu launcher cache (edit-controls block) ===== */

extern uint64_t g_edit_controls_widget;  /* 0x2843e0 — cached launcher widget (misc p2-owned word) */
extern uint64_t g_edit_controls_state_a; /* 0x2844c0 — captured protected-channel handle (misc p2) */

/* rich-binding record for the launcher: the widget word plus 26 binding
 * qwords (rich_binding_bind row index 0x3d4); cached at 0x2843e0 with the
 * tail at 0x2843e8 (the widget word itself is misc p2-owned) */
typedef struct {
    uint64_t binding[26];     /* +0x00..0xc8 — rich-binding fields */
} battle_button_tail_t;       /* 0xd0 bytes at 0x2843e8 */

typedef struct {
    uint64_t widget;          /* +0x00 — 0x260-byte launcher widget */
    battle_button_tail_t tail; /* +0x08 — rich-binding fields */
} battle_button_record_t;     /* 0xd8 bytes */

battle_button_tail_t g_battle_button_tail; /* 0x2843e8 — cached record tail */
uint64_t g_edit_controls_root;             /* 0x2844b8 — ctx the record was built under (misc.c: g_edit_controls_root) */
uint32_t g_battle_button_attached;         /* 0x2844c8 — record attached (lo of misc.c g_edit_controls_flag) */
uint32_t g_battle_button_failed;           /* 0x2844cc — contract failed (hi of the qword) */
uint32_t g_battle_button_visible;          /* 0x2844d0 — button visible (lo of misc.c g_edit_controls_state_b) */
uint32_t g_battle_button_phase;            /* 0x2844d4 — build phase 1..3 (hi of the qword) */

/* ===== cross-file engine/widget helpers ===== */

extern int protected_channel_acquire(uint64_t *handle_out);
                                      /* misc.c @ 0014d288 — capture the protected channel */
extern int sc_object_release(uint64_t obj);
                                      /* misc.c @ 0014e5ac — validate free SC node */
extern uint64_t engine_call_11a2840(uint64_t size);
                                      /* misc.c @ 0016cdf0 — allocator via engine base */
extern void engine_call_887c14(void); /* misc.c @ 0016ce34 — post-alloc bookkeeping */
extern uint64_t engine_sc_bundle_load(const char *path);
                                      /* misc.c @ 0016ce48 — loads "sc/ui.sc" */
extern void engine_call_5d7c30(uint64_t obj, int a);
                                      /* misc.c @ 0016ce84 */
extern void widget_set_text(uint64_t widget, uint64_t text_obj);
                                      /* misc.c @ 0016cec0 — engine 0x88836c */
extern void widget_set_scale(uint64_t widget, float sx, float sy);
                                      /* misc.c @ 0016cf00 — engine 0x595344 */
extern void widget_set_position(uint64_t widget, float x, float y);
                                      /* misc.c @ 0016cf5c — engine 0x595314 */
extern void widget_set_anchor(uint64_t widget, void *anchor_out);
                                      /* misc.c @ 0016cf84 — engine 0x595378 (a,b,0,0) */
extern void engine_call_5988ec(uint64_t parent, uint64_t child);
                                      /* misc.c @ 0016cfb8 — attach child to parent */
extern int widget_child_link_valid(uint64_t node, uint64_t parent);
                                      /* misc.c @ 0016d5cc — parent+0x50 slot[index] == node */
extern int ui_scene_fonts_ready(void);
                                      /* misc.c @ 0016d910 — scene root + fonts gate */
extern int movie_frame_count_check(uint64_t movie, int idx, int min);
                                      /* misc.c @ 0016dc20 — frame count (+0xbe) vs table */
extern int widget_flag48_equals(uint64_t widget, int expected);
                                      /* misc.c @ 0016e464 — +0x48 flag byte */
extern int engine_code_snapshot_verify(void);
                                      /* misc.c @ 00172da8 — 57 engine regions memcmp */
extern int widget_set_frame(uint64_t widget, float x, float y, float w,
                            float h); /* misc.c @ 0017b5e8 — frame with readback */
extern uint64_t ui_string_object(const char *text);
                                      /* misc.c @ 0017b858 — intern string object */
extern int widget_rich_panel_bound_check(uint64_t widget);
                                      /* misc.c @ 0017c598 — bound under rich panel ctx */
extern int ui_widget_pair_attached_check(void);
                                      /* misc.c @ 0017ce78 — cached panel pair attached */
extern void ui_cached_widget_clear_hide(void);
                                      /* misc.c @ 0017cfbc — clear state, hide 0x2843e0 */
extern const char *nexus_rich_asset(uint32_t index);
                                      /* renderer.c @ 0016a1dc — asset index -> rodata string */
extern int rich_binding_bind(uint64_t *row_slot, uint64_t display,
                             uint32_t row_index);
                                      /* renderer.c @ 0017c42c — bind widget to display */

/* ===== battle menu launcher (native BATTLE button) ===== */

/* capped "battle_menu_launcher" event log (counter 0x1a7d20, cap 0x40) */
static void battle_menu_log_event(const char *event, uint32_t value)
{
    uint32_t under_cap = g_rich_stop_log_count < 0x40;
    g_rich_stop_log_count = g_rich_stop_log_count + 1;
    if (under_cap && g_host_log != NULL)
        g_host_log(g_host_ctx, "battle_menu_launcher", event, value);
}

/*
 * hud_battle_button_render — render the native "BATTLE" launcher button.
 * Resets the cached launcher block (0x2843e0..0x2844d8) when its owning UI
 * context went stale; while the rich panel is not both built and open, the
 * protected channel is held and the HUD record's +9 suppress byte is clear,
 * builds the button from the "sc/ui.sc" bundle (0x260 widget, rich row
 * 0x3d4, "BATTLE", scene-root attach, vptr game+0x11c0a48 and +0x38/+0x40/
 * +0x48 gates) and frames it near the UI center; else parks it offscreen.
 * Also re-lays the two rich chat rows (0x284220) around the panel. Logs
 * "battle_menu_launcher" events (cap 0x40). state: HUD record (+9 only).
 * @ 00170db4
 */
void hud_battle_button_render(void *state)
{
    /* stale-owner reset of the whole cached launcher block */
    if (g_edit_controls_root != 0 && g_edit_controls_root != g_ui_context) {
        g_edit_controls_widget = 0;                              /* 0x2843e0 */
        memset(&g_battle_button_tail, 0, sizeof g_battle_button_tail);
        g_edit_controls_root = 0;                                /* 0x2844b8 */
        g_edit_controls_state_a = 0;                             /* 0x2844c0 */
        g_battle_button_attached = 0;                            /* 0x2844c8 */
        g_battle_button_failed = 0;                              /* 0x2844cc */
        g_battle_button_visible = 0;                             /* 0x2844d0 */
        g_battle_button_phase = 0;                               /* 0x2844d4 */
    }

    uint64_t channel_handle = 0;
    int channel_ok = protected_channel_acquire(&channel_handle);
    uint8_t suppress = ((const uint8_t *)state)[9];

    /* panel flag48 probe: panel built, page ctx live, not open */
    int panel_flag48 = 0;
    if (g_rich_panel_built != 0 && g_rich_panel_page_ctx != 0
        && g_rich_panel_open == 0
        && g_rich_panel_root == g_ui_context
        && widget_rich_panel_bound_check(g_rich_panel_row0_widget) != 0) {
        uint8_t flag = 0xff;
        if (0x1000 < g_rich_panel_row0_widget + 0x49
            && g_host_read(g_host_ctx, g_rich_panel_row0_widget + 0x48,
                           &flag, 1) == 1
            && flag == 1)
            panel_flag48 = 1;
    }

    /* each attached layer pushes the button 50*scale further down */
    int layers = panel_flag48;
    if (ui_widget_pair_attached_check() != 0)
        layers = layers + 1;

    /* eligible while the panel is not both built and open */
    bool eligible = false;
    if (channel_ok != 0 && suppress == 0)
        eligible = (g_rich_panel_built == 0 || g_rich_panel_open == 0)
                   && channel_handle != 0;

    /* re-lay the two rich chat rows around the panel */
    if (ui_widget_pair_attached_check() != 0) {
        float row_y = 96.0f;   /* 0x42c00000 — rows sit lower while the panel is up */
        if (g_rich_panel_built != 0 && g_rich_panel_page_ctx != 0
            && g_rich_panel_open == 0
            && g_rich_panel_root == g_ui_context
            && widget_rich_panel_bound_check(g_rich_panel_row0_widget) != 0) {
            uint8_t flag = 0xff;
            if (0x1000 < g_rich_panel_row0_widget + 0x49
                && g_host_read(g_host_ctx, g_rich_panel_row0_widget + 0x48,
                               &flag, 1) == 1
                && flag == 1)
                row_y = 146.0f;   /* 0x43120000 */
        }

        for (int i = 0; i < 2; i++) {
            uint64_t row_widget = g_rich_chat_rows[i * 0x1b]; /* 0x284220, stride 0x1b qwords */
            widget_set_position(row_widget, 0.0f, 0.0f);
            widget_set_scale(row_widget, 1.0f /* 0x3f800000 */,
                             1.0f /* 0x3f800000 */);
            float anchor[4] = {0};
            widget_set_anchor(row_widget, anchor);
            float width = anchor[2] - anchor[0];   /* anchor span */
            if (!isfinite(width) || width <= 0.0f)
                break;
            uint8_t size16[16] = {0};
            if (row_widget + 0x10 < 0x1000
                || (row_widget & 0xfffffffffffffff0ULL) == 0xffffffffffffffe0ULL
                || g_host_read(g_host_ctx, row_widget + 0x10, size16, 0x10) != 1)
                break;
            float size_a, size_b, size_c, size_d;
            memcpy(&size_a, size16, 4);
            memcpy(&size_b, size16 + 4, 4);
            memcpy(&size_c, size16 + 8, 4);
            memcpy(&size_d, size16 + 12, 4);
            if (!isfinite(size_a) || !isfinite(size_b) || !isfinite(size_c)
                || !isfinite(size_d))
                break;
            if (size_b != 0.0f || size_c != 0.0f || !(size_a > 0.0f)
                || !(size_d > 0.0f) || size_a > 128.0f || size_d > 128.0f)
                break;
            /* scale from the row size field: 76 * size * scale / (width * density) */
            float scale = (size_a * 76.0f * g_rich_row_height)
                          / (width * g_ui_density);
            if (!isfinite(scale) || scale <= 0.0f || scale > 8.0f)
                break;
            widget_set_scale(row_widget, scale, scale);
            widget_set_anchor(row_widget, anchor);
            float x_off = fmaf((float)i, 84.0f /* 0x42a80000 */,
                               20.0f /* 0x41a00000 */);
            float x = g_ui_center_x - x_off * g_rich_row_height;
            float y = g_ui_center_y - row_y * g_rich_row_height;
            widget_set_position(row_widget, x / g_ui_density - anchor[0],
                                y / g_ui_density - anchor[1]);
        }
    }

    /* scene-root +0x19c byte must read 0 and no prior failure */
    bool launcher_active = false;
    if (eligible && ui_scene_fonts_ready() != 0) {
        uint8_t scene_flag = 1;
        launcher_active = g_ui_scene_root + 0x19d >= 0x1001
                          && g_host_read(g_host_ctx, g_ui_scene_root + 0x19c,
                                         &scene_flag, 1) == 1
                          && scene_flag == 0
                          && g_battle_button_failed == 0;
    }

    if (launcher_active) {
        if (g_battle_button_attached == 0) {
            g_battle_button_phase = 1;   /* loading the bundle */
            if (g_host_style(g_host_ctx, nexus_rich_asset(5)) != 0
                && engine_code_snapshot_verify() != 0) {
                g_battle_button_phase = 2;   /* building the widget */
                uint64_t bundle = engine_sc_bundle_load(nexus_rich_asset(5));
                if (movie_frame_count_check(bundle, 5, 0) != 0) {
                    /* bundle +0x38 parent must be 0 and +0x40 index -1 */
                    uint64_t parent = 1;
                    float index_f = 0.0f;
                    if (0xfff < bundle + 0x38
                        && (bundle & 0xfffffffffffffff8ULL) != 0xffffffffffffffc0ULL
                        && g_host_read(g_host_ctx, bundle + 0x38, &parent, 8) == 1
                        && parent == 0
                        && 0xfff < bundle + 0x40
                        && (bundle & 0xfffffffffffffffcULL) != 0xffffffffffffffbcULL
                        && g_host_read(g_host_ctx, bundle + 0x40, &index_f, 4) == 1
                        && index_f == -1.0f /* raw: -NAN, sign-extended -1 */) {
                        engine_call_5d7c30(bundle, 0);
                        uint64_t widget = engine_call_11a2840(0x260);
                        if (widget != 0) {
                            engine_call_887c14();
                            /* widget vptr must be game_base + 0x11c0a48 */
                            uint64_t vptr = 0;
                            int read_ok = 0;
                            if ((widget + 8) >> 3 >= 0x201)
                                read_ok = g_host_read(g_host_ctx, widget,
                                                      &vptr, 8) == 1;
                            int plausible = 0xfff < vptr && read_ok
                                            && (vptr & 7) == 0;
                            if ((plausible ? vptr : 0)
                                == g_game_base + 0x11c0a48U) {
                                /* widget +0x38 parent 0, +0x40 index -1 */
                                parent = 1;
                                index_f = 0.0f;
                                if (0xfff < widget + 0x38
                                    && (widget & 0xfffffffffffffff8ULL) != 0xffffffffffffffc0ULL
                                    && g_host_read(g_host_ctx, widget + 0x38,
                                                   &parent, 8) == 1
                                    && parent == 0
                                    && 0xfff < widget + 0x40
                                    && (widget & 0xfffffffffffffffcULL) != 0xffffffffffffffbcULL
                                    && g_host_read(g_host_ctx, widget + 0x40,
                                                   &index_f, 4) == 1
                                    && index_f == -1.0f
                                    && sc_object_release(widget) != 0) {
                                    battle_button_record_t rec;
                                    memset(&rec, 0, sizeof rec);
                                    rec.widget = widget;
                                    if (rich_binding_bind(&rec.widget, bundle,
                                                          0x3d4) != 0) {
                                        uint64_t text_obj = ui_string_object("BATTLE");
                                        widget_set_text(widget, text_obj);
                                        widget_set_position(widget,
                                                            -10000.0f /* 0xc61c3c00 */,
                                                            -10000.0f /* 0xc61c3c00 */);
                                        engine_call_5988ec(g_ui_scene_root, widget);
                                        if (widget_child_link_valid(widget,
                                                                    g_ui_context) != 0
                                            && widget_flag48_equals(widget, 1) != 0) {
                                            /* cache the record and control words */
                                            g_edit_controls_widget = widget;     /* 0x2843e0 */
                                            g_battle_button_tail = rec.tail;     /* 0x2843e8 */
                                            g_edit_controls_root = g_ui_context; /* 0x2844b8 */
                                            g_battle_button_attached = 1;        /* 0x2844c8 */
                                            g_battle_button_phase = 3;           /* 0x2844d4 */
                                            battle_menu_log_event("native_button_attached", 0);
                                            goto have_record;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            /* build failed: report through the styled host, then clear+hide */
            if (g_host_style(g_host_ctx, nexus_rich_asset(5)) != 0) {
                g_battle_button_failed = 1;
                battle_menu_log_event("native_button_contract_failed",
                                      g_battle_button_phase);
            }
            ui_cached_widget_clear_hide();
            return;
        }

have_record:
        /* record exists: frame the button on-screen */
        if (widget_child_link_valid(g_edit_controls_widget, g_ui_context) != 0) {
            uint8_t flag = 0xff;
            if (0x1000 < g_edit_controls_widget + 0x49
                && g_host_read(g_host_ctx, g_edit_controls_widget + 0x48,
                               &flag, 1) == 1
                && flag == 1) {
                float x = g_ui_center_x
                          - g_rich_row_height * 75.0f /* 0x42960000 */;
                float y = g_ui_center_y
                          - ((float)(uint8_t)(layers * 50 /* '2' */
                                              + 96 /* 0x60 */) + 21.0f)
                            * g_rich_row_height;
                int framed = widget_set_frame(g_edit_controls_widget,
                                              x / g_ui_density,
                                              y / g_ui_density,
                                              (g_rich_row_height * 110.0f) / g_ui_density,
                                              (g_rich_row_height * 42.0f) / g_ui_density);
                if (framed != 0) {
                    g_battle_button_visible = 1;
                    g_edit_controls_state_a = channel_handle;
                    return;
                }
                g_battle_button_failed = 1;
                ui_cached_widget_clear_hide();
                return;
            }
        }
        /* frame not set: restore the rodata defaults (0x10f860 pair) */
        g_edit_controls_state_a = 0;
        uint64_t defaults = *(const uint64_t *)(const void *)(uintptr_t)0x10f860;
        g_battle_button_failed = (uint32_t)defaults;
        g_battle_button_visible = (uint32_t)(defaults >> 32);
    } else {
        /* launcher inactive: clear the per-frame state */
        g_battle_button_visible = 0;
        g_edit_controls_state_a = 0;
    }

    /* hide tail: park the button offscreen while the record is still live */
    if (g_battle_button_attached != 0 && g_edit_controls_widget != 0
        && widget_child_link_valid(g_edit_controls_widget, g_ui_context) != 0)
        widget_set_position(g_edit_controls_widget,
                            -10000.0f /* 0xc61c3c00 */,
                            -10000.0f /* 0xc61c3c00 */);
}
/* widgets chain 2 chunk 2: covers raw lines 5058-5601 (hud_fps_limit_panel_render
 * complete; the nominal 5058-5500 slice ends mid-function, so this chunk closes it) */

/* ===== chain-2 additions: theme view / fps panel state ===== */

/* ---- theme view record (0x2844e8 block, widgets.c-owned; misc.c externs) ---- */

uint64_t g_theme_view_ctx;    /* 0x2844e8 — owning UI context (misc.c: g_theme_view_ctx) */
uint64_t g_theme_view;        /* 0x2844f0 — popover_text_left view widget (misc.c: g_theme_view) */
uint64_t g_theme_view_flag;   /* 0x2844f8 — view built flag (misc.c: g_theme_view_flag) */
uint64_t g_theme_view_extra;  /* 0x284500 — styled text subwidget (misc.c: g_theme_view_extra) */
char     g_theme_view_text[0x140];  /* 0x284508 — styled text cache (cap 0x140) */
uint32_t g_theme_view_failed;       /* 0x284648 — style contract failed */

/* ---- edit-controls block (0x282150 group, widgets.c-owned; misc.c externs) ---- */

uint64_t g_edit_controls_block_root;  /* 0x282150 — ctx the block was built for
                                        * (renderer.c names the same word g_fps_popup_owner) */
uint64_t g_edit_controls_ctx;         /* 0x282158 — style ctx widget (renderer: g_fps_popup_edit_ctx) */
uint64_t g_edit_controls_slot;        /* 0x282160 — kind-0 slider track slot */
uint64_t g_edit_controls_panel;       /* 0x282168 — slider value widget (kind 2) */
uint64_t g_edit_controls_row_a;       /* 0x282178 — kind-1 slider label row */
uint64_t g_edit_controls_row_b;       /* 0x2822d0 — kind-1 scale label row */
uint64_t g_edit_controls_row_c;       /* 0x282428 — SAVE button row */
uint64_t g_edit_controls_row_d;       /* 0x282580 — RESET button row */

/* status label records: the 0x158-stride tail of the block. rich_label_render
 * reads {ctx @+0x08, widget @+0x10, cache @+0x18}; misc.c resolves the
 * +0x08/+0x10 words as g_edit_controls_status_{a,b,c}(_x). */
typedef struct {
    uint64_t head;         /* +0x00 — record head */
    uint64_t ctx_slot;     /* +0x08 — ctx slot (misc.c: g_edit_controls_status_a) */
    uint64_t widget_slot;  /* +0x10 — widget slot (misc.c: g_edit_controls_status_a_x) */
    char     text[0x140];  /* +0x18 — styled text cache (cap 0x140) */
} status_label_rec_t;      /* 0x158 bytes */

status_label_rec_t g_edit_controls_status_rec_a;  /* 0x2826d8 — "FPS LIMIT" title record */
status_label_rec_t g_edit_controls_status_rec_b;  /* 0x282830 — "Limit: %d FPS" record */
status_label_rec_t g_edit_controls_status_rec_c;  /* 0x282988 — save note record */

uint64_t g_edit_controls_extra_ctx;      /* 0x282ae0 — styles-ready ctx (renderer: g_fps_popup_field_ae0) */
uint32_t g_edit_controls_slider_created; /* 0x282ae4 — native slider created flag */
uint32_t g_edit_controls_open_mirror;    /* 0x282ae8 — popup-open mirror */
uint64_t g_edit_controls_slider_ms;      /* 0x282af0 — slider clock (ms) */

/* ---- fps popup tail (renderer.c-owned) ---- */

extern uint32_t g_fps_popup_open;      /* 0x282af8 — 1 while the fps popup is up */
extern uint32_t g_fps_popup_limit;     /* 0x282afc — selected fps limit (min 0x91 = 145) */
extern uint32_t g_fps_popup_applied;   /* 0x282b00 — applied limit mirror */
extern uint32_t g_fps_popup_field_b04; /* 0x282b04 — zeroed on open */
extern uint32_t g_fps_popup_opens;     /* 0x282b0c — capped 24: popup_open event counter */

/* ===== chain-2 additions: cross-file helpers ===== */

extern int script_port_client_performance_query(void *out);
                                      /* misc.c @ 00191c90 — forwarded query */
extern uint64_t engine_singleton_1307e20_read(void);
                                      /* misc.c (b-chain) @ 0017ec28 — deep singleton chain */
extern int remote_vtable_matches(uint64_t obj, uint64_t rva);
                                      /* misc.c (b-chain) @ 0016debc — vptr == game base + rva */
extern int widget_detached_check(uint64_t node);
                                      /* misc.c (b-chain) @ 0016ddc8 — parent NULL, index -1 */
extern uint64_t engine_call_5d8ad4(uint64_t a, uint64_t b);
                                      /* misc.c @ 0016ce98 — engine style lookup */
extern int widget_chain_links_unique_check(uint64_t node, uint64_t parent);
                                      /* misc.c @ 0016e020 — chain links each appear once */
extern void engine_call_594e80(uint64_t a, uint64_t b);
                                      /* misc.c @ 0016ce20 — engine forwarder */
extern void engine_call_594130(uint64_t a, uint64_t b);
                                      /* misc.c @ 0016cfa4 — engine attach forwarder */
extern int widget_ancestor_of(uint64_t node, uint64_t target);
                                      /* misc.c (b-chain) @ 0016d800 — <=16-level parent walk */
extern int online_probe_poll(void);
                                      /* misc.c @ 0014cd5c — rate-limited online probe */
extern void ui_timing_line_format(char *buf, int ms, const char *note,
                                  int delta);
                                      /* misc.c @ 0017f1e4 — "N ms / note" status line */
extern void engine_call_66ae58(uint64_t out, const char *text);
                                      /* misc.c @ 0016ced8 — build engine string object */
extern void engine_call_5921a0(uint64_t a, uint64_t b);
                                      /* misc.c @ 0016ceac — set widget text object */
extern void game_call_66ad48(void *arg);
                                      /* misc.c (a-chain) @ 0014ec30 — engine base + 0x66ad48 */
extern void ui_text_style_format(char *buf, size_t fortify_slen,
                                 size_t write_cap, const char *fmt, ...);
                                      /* misc.c @ 00176a24 — checked text formatter */
extern void engine_call_592250(uint64_t a, uint64_t b);
                                      /* misc.c @ 0016ceec — invalidate widget */
extern void widget_set_font_size(uint64_t widget, float size);
                                      /* misc.c @ 0016cf70 — engine 0x5958e8 */
extern void rich_label_render(float x, float y, float font, void *record,
                              const char *text);
                                      /* misc.c @ 0017e998 — rich view text label */
extern int ui_edit_controls_styles_ready(void);
                                      /* widgets.c (chain 2, below) @ 0017e0b0 */
extern int ui_edit_controls_ctx_valid(void);
                                      /* misc.c @ 0016e7c0 — 0x282168 ctx: class 0x11c0c58, bound */

/* ---- rodata ---- */

extern const uint64_t PERF_QUERY_MAGIC;  /* 0x10f850 — performance query magic qword */
extern const char FMT_1343dc[];          /* 0x1343dc — styled label format */
extern const char FMT_13aa70[];          /* 0x13aa70 — limit label format (slider at the 145 floor) */
extern const char RODATA_13784a[];       /* 0x13784a — engine_call_5d8ad4 style name */

/*
 * perf_query_t — the 0x98-byte performance query record (renderer.c sizes
 * it 0x98; this caller only initializes the 0x58-byte head).
 */
typedef struct {
    uint64_t magic;        /* +0x00 — query magic (rodata 0x10f850) */
    uint64_t flags;        /* +0x08 — bit0 popup wanted, bit1 popover wanted, hi dword busy */
    uint32_t field_10;     /* +0x10 */
    int32_t  ms;           /* +0x14 — measured frame time (ms) */
    uint64_t field_18;     /* +0x18 */
    const char *note;      /* +0x20 — timing note */
    uint8_t   tail[0x70];  /* +0x28..0x98 — query-private */
} perf_query_t;

/*
 * widget_reaches_w2 — walk the parent chain from `node` toward `target`
 * (at most 16 levels), validating every link through the host read (the
 * raw inlines this walk four times). *link_ok is cleared when a link check
 * fails (the raw exits to the edit-controls tail / resets the block).
 * Returns 1 when the walk lands exactly on target.
 */
static int widget_reaches_w2(uint64_t node, uint64_t target, int *link_ok)
{
    for (uint32_t depth = 0; ; depth++) {
        if (node == target || depth > 0xf)
            return node == target;
        uint64_t parent_raw = 0;
        int rd = (node + 0x40) >> 3 >= 0x201
                 && g_host_read(g_host_ctx, node + 0x38, &parent_raw, 8) == 1;
        int plausible = rd && 0xfff < parent_raw && (parent_raw & 7) == 0;
        if (widget_child_link_valid(node, plausible ? parent_raw : 0) == 0) {
            *link_ok = 0;
            return 0;
        }
        if (!plausible)
            return 0;   /* walked off: the raw clears the node */
        node = parent_raw;
    }
}

/* reset the whole cached theme-view block (0x2844e8..0x28464c) */
static void theme_view_block_reset(void)
{
    g_theme_view_failed = 0;                              /* 0x284648 */
    g_theme_view = 0;                                     /* 0x2844f0 */
    g_theme_view_ctx = 0;                                 /* 0x2844e8 */
    g_theme_view_extra = 0;                               /* 0x284500 */
    g_theme_view_flag = 0;                                /* 0x2844f8 */
    memset(g_theme_view_text, 0, sizeof g_theme_view_text); /* 0x284508 */
}

/* ===== fps limit panel (script-port popup) ===== */

/*
 * hud_fps_limit_panel_render — render the FPS LIMIT popup panel: keep the
 * cached popover_text_left view (0x2844e8 block) in sync with the engine
 * singleton (build it styled when absent, update its timing line from the
 * performance query + online probe, park it at (120,90) off its anchor;
 * reset the block on stale owner or broken lineage), then lay out the
 * edit-controls block (0x282150): scale min(w/620, h/420, 1), center it,
 * frame the slider track/label rows and SAVE/RESET buttons, feed the
 * slider dt (capped 0.1s) through engine base + 0x889940 and, when the
 * slider register (panel+0xe8, raw bits) lands in 1..145 with the popup
 * open, mirror it into the limit and refresh the "FPS LIMIT"/"Limit: %d
 * FPS"/save-note labels and the SAVE/SAVED and RESET/DEFAULT button
 * texts. Logs 'script_port_fps' native_slider_created /
 * native_ui_contract_failed (capped 24 opens). params: (width, height,
 * frame_ms). @ 00171d68
 */
void hud_fps_limit_panel_render(float width, float height, uint64_t frame_ms)
{
    if (ui_scene_fonts_ready() == 0)
        return;

    /* scene root +0x19c byte must read 0 */
    uint8_t scene_flag = 1;
    if (g_ui_scene_root + 0x19d < 0x1001
        || g_host_read(g_host_ctx, g_ui_scene_root + 0x19c, &scene_flag, 1) != 1
        || scene_flag != 0)
        return;

    perf_query_t query = {0};              /* raw zeroes the 0x58-byte head */
    query.magic = PERF_QUERY_MAGIC;        /* 0x10f850 */
    if (script_port_client_performance_query(&query) == 0)
        memset(&query, 0, 0x58);           /* raw clears the head on failure */

    uint64_t singleton = engine_singleton_1307e20_read();
    uint64_t ui_context = g_ui_context;

    /* stale-owner reset of the theme-view block */
    if (g_theme_view_ctx != 0 && g_theme_view_ctx != g_ui_context)
        theme_view_block_reset();

    if (singleton == 0) {
        /* no singleton: park the cached view when its lineage still holds */
        if (g_theme_view_flag != 0) {
            int link_ok = 1;
            int reached = g_theme_view != 0
                          && widget_reaches_w2(g_theme_view, ui_context, &link_ok);
            if (!link_ok)
                goto panel_tail;
            if (reached) {
                int r2 = widget_reaches_w2(g_theme_view_flag, g_theme_view,
                                           &link_ok);
                if (!link_ok)
                    goto panel_tail;
                if (r2)
                    widget_set_position(g_theme_view_flag,
                                        -10000.0f /* 0xc61c3c00 */,
                                        -10000.0f /* 0xc61c3c00 */);
            }
        }
    } else {
        if (g_theme_view != 0 && g_theme_view != singleton)
            theme_view_block_reset();

        if ((query.flags >> 1 & 1) == 0 || (uint32_t)(query.flags >> 32) != 0) {
            /* popover not wanted (or busy): park the cached view */
            if (g_theme_view_flag != 0 && singleton == g_theme_view) {
                int link_ok = 1;
                int reached = widget_reaches_w2(g_theme_view_flag, singleton,
                                                &link_ok);
                if (!link_ok)
                    goto panel_tail;
                if (reached)
                    widget_set_position(g_theme_view_flag,
                                        -10000.0f /* 0xc61c3c00 */,
                                        -10000.0f /* 0xc61c3c00 */);
            }
        } else if (g_theme_view_failed == 0) {
            if (g_theme_view_flag == 0) {
                /* build the popover_text_left view from the sc bundle */
                if (g_host_style(g_host_ctx, "popover_text_left") != 0
                    && engine_code_snapshot_verify() != 0) {
                    g_theme_view_ctx = g_ui_context;          /* 0x2844e8 */
                    g_theme_view = singleton;                 /* 0x2844f0 */
                    g_theme_view_flag = engine_sc_bundle_load(
                        "popover_text_left");                 /* 0x2844f8 */
                    if (remote_vtable_matches(g_theme_view_flag, 0x11ad208) != 0
                        && widget_detached_check(g_theme_view_flag) != 0) {
                        engine_call_5d7c30(g_theme_view_flag, 0);
                        g_theme_view_extra = engine_call_5d8ad4(
                            g_theme_view_flag,
                            (uint64_t)(uintptr_t)RODATA_13784a);         /* 0x284500 */
                        if (remote_vtable_matches(g_theme_view_extra,
                                                  0x11abad8) != 0
                            && widget_chain_links_unique_check(
                                   g_theme_view_flag, g_theme_view_extra) != 0) {
                            /* subwidget child count (+0xb0) and class byte (+0x90) */
                            uint16_t count = 0;
                            uint8_t class_byte = 0;
                            if (0xfff < g_theme_view_extra + 0xb0
                                && (g_theme_view_extra
                                    & 0xfffffffffffffffeULL)
                                       != 0xffffffffffffff4eULL
                                && g_host_read(g_host_ctx,
                                               g_theme_view_extra + 0xb0,
                                               &count, 2) == 1
                                && 0 < count && count < 0x101
                                && 0x1000 < g_theme_view_extra + 0x91
                                && g_host_read(g_host_ctx,
                                               g_theme_view_extra + 0x90,
                                               &class_byte, 1) == 1
                                && class_byte < 2) {
                                /* force 14 children, class 1 */
                                *(uint16_t *)(uintptr_t)(g_theme_view_extra
                                                         + 0xb0) = 0xe;
                                *(uint8_t *)(uintptr_t)(g_theme_view_extra
                                                        + 0x90) = 1;
                                widget_set_scale(g_theme_view_flag, 1.0f,
                                                 1.0f /* 0x3f800000 */);
                                widget_set_scale(g_theme_view_extra, 1.0f,
                                                 1.0f /* 0x3f800000 */);
                                engine_call_594e80(g_theme_view_flag, 0);
                                widget_set_position(g_theme_view_flag,
                                                    -10000.0f /* 0xc61c3c00 */,
                                                    -10000.0f /* 0xc61c3c00 */);
                                engine_call_594130(singleton,
                                                   g_theme_view_flag);
                                if (widget_ancestor_of(g_theme_view_flag,
                                                       singleton) != 0
                                    && g_theme_view_flag != 0)
                                    goto theme_text_update;
                            }
                        }
                    }
                    g_theme_view_failed = 1;                    /* 0x284648 */
                }
            } else {
theme_text_update:
                /* update the popover timing line */
                {
                    int link_ok = 1;
                    int reached = widget_reaches_w2(g_theme_view_flag,
                                                    singleton, &link_ok);
                    if (!link_ok) {
                        theme_view_block_reset();
                        goto panel_tail;
                    }
                    if (reached
                        && widget_ancestor_of(g_theme_view_extra,
                                              g_theme_view_flag) != 0) {
                        char line[0x140];
                        int delta = online_probe_poll();
                        ui_timing_line_format(line, query.ms, query.note,
                                              delta);
                        if (strcmp(line, g_theme_view_text) != 0) {
                            uint64_t str_obj = 0;
                            engine_call_66ae58((uint64_t)&str_obj, line);
                            engine_call_5921a0(g_theme_view_extra,
                                               (uint64_t)&str_obj);
                            game_call_66ad48(&str_obj);
                            ui_text_style_format(g_theme_view_text, 0x140,
                                                 0x140, FMT_1343dc, line);
                            engine_call_592250(g_theme_view_extra,
                                              0xffffffffu);
                        }
                        /* park the subwidget off its anchor */
                        uint16_t count = 0;
                        if (0xfff < g_theme_view_extra + 0xb0
                            && (g_theme_view_extra & 0xfffffffffffffffeULL)
                                   != 0xffffffffffffff4eULL
                            && g_host_read(g_host_ctx,
                                           g_theme_view_extra + 0xb0,
                                           &count, 2) == 1
                            && 0 < count && count < 0x101) {
                            widget_set_scale(g_theme_view_extra, 1.0f,
                                             1.0f /* 0x3f800000 */);
                            widget_set_position(g_theme_view_flag, 0.0f,
                                                0.0f);
                            widget_set_position(g_theme_view_extra, 0.0f,
                                                0.0f);
                            float anchor[4];
                            widget_set_anchor(g_theme_view_extra, anchor);
                            if (isfinite(anchor[0]) && isfinite(anchor[1]))
                                widget_set_position(g_theme_view_extra,
                                                    120.0f - anchor[0],
                                                    90.0f - anchor[1]);
                        }
                    } else {
                        theme_view_block_reset();
                    }
                }
            }
        }
    }

panel_tail:
    /* edit-controls block: stale-owner reset (raw memsets 0x9b8 bytes) */
    if (g_edit_controls_block_root != 0
        && g_edit_controls_block_root != g_ui_context)
        memset((void *)(uintptr_t)0x282150, 0, 0x9b8);  /* 0x282150 block */

    if ((query.flags & 1) == 0) {
        g_fps_popup_open = 0;                              /* 0x282af8 */
    } else {
        g_edit_controls_open_mirror = g_fps_popup_open;    /* 0x282ae8 */
        if (g_fps_popup_open != 0) {
            if (g_edit_controls_slider_created != 0)
                return;

            if (g_edit_controls_extra_ctx == 0) {
                /* first open: verify the styles and create the slider */
                if (g_host_style(g_host_ctx, "edit_controls_ui") == 0
                    || g_host_style(g_host_ctx, "popup_generic") == 0
                    || g_host_style(g_host_ctx, "popover_text_left") == 0)
                    return;
                if (ui_edit_controls_styles_ready() == 0) {
                    g_edit_controls_slider_created = 1;
                    uint32_t opens = g_fps_popup_opens + 1;
                    bool under_cap = g_fps_popup_opens < 0x18;
                    g_fps_popup_opens = opens;
                    if (under_cap && g_host_log != NULL)
                        g_host_log(g_host_ctx, "script_port_fps",
                                   "native_ui_contract_failed", 0);
                    return;
                }
                uint32_t opens = g_fps_popup_opens + 1;
                bool under_cap = g_fps_popup_opens < 0x18;
                g_fps_popup_opens = opens;
                if (under_cap && g_host_log != NULL)
                    g_host_log(g_host_ctx, "script_port_fps",
                               "native_slider_created", 1);
            }

            /* lay out the panel */
            if (widget_child_link_valid(g_edit_controls_ctx,
                                        g_ui_context) != 0
                && ui_edit_controls_ctx_valid() != 0) {
                float w_n = width / g_ui_density;
                float h_n = height / g_ui_density;
                float s = fminf(fminf(w_n / 620.0f, h_n / 420.0f),
                                1.0f /* 0x3f800000 */);
                if (isfinite(s) && s > 0.0f) {
                    widget_set_scale(g_edit_controls_ctx, s, s);
                    float x = g_ui_center_x - width * 0.5f;   /* 0x3f000000 */
                    float y = g_ui_center_y - height * 0.5f;  /* 0x3f000000 */
                    widget_set_position(g_edit_controls_ctx,
                                        x / g_ui_density,
                                        y / g_ui_density);
                    widget_set_frame(g_edit_controls_row_a, 0.0f, 0.0f,
                                     w_n / s, h_n / s);
                    widget_set_font_size(g_edit_controls_row_a,
                                         0.65f /* 0x3f266666 */);
                    widget_set_frame(g_edit_controls_slot, 0.0f, 0.0f,
                                     600.0f /* 0x44160000 */,
                                     92.5f /* 0x43b90000 */);
                    widget_set_frame(g_edit_controls_row_b,
                                     260.0f /* 0x43838000 */,
                                     -27.5f /* 0xc31b0000 */,
                                     48.0f /* 0x42400000 */,
                                     42.0f /* 0x42280000 */);
                    widget_set_frame(g_edit_controls_row_c,
                                     -110.0f /* 0xc2dc0000 */,
                                     135.0f /* 0x43070000 */,
                                     150.0f /* 0x43160000 */,
                                     44.0f /* 0x42300000 */);
                    widget_set_frame(g_edit_controls_row_d,
                                     110.0f /* 0x42dc0000 */,
                                     135.0f /* 0x43070000 */,
                                     150.0f /* 0x43160000 */,
                                     44.0f /* 0x42300000 */);
                    float dt = 0.0f;
                    if (g_edit_controls_slider_ms - 1 < frame_ms)
                        dt = (float)(frame_ms - g_edit_controls_slider_ms)
                             / 1000.0f;
                    float dt_c = fminf(dt, 0.1f /* 0x3dcccccd */);
                    g_edit_controls_slider_ms = frame_ms;
                    /* engine slider-value call at game base + 0x889940 */
                    ((void (*)(float, uint64_t))(uintptr_t)(g_game_base
                                                            + 0x889940))(
                        dt_c, g_edit_controls_panel);

                    /* slider register (raw bits at panel+0xe8) in 1..145 */
                    uint32_t slider_raw = 0;
                    if (0xfff < g_edit_controls_panel + 0xe8
                        && (g_edit_controls_panel & 0xfffffffffffffffcULL)
                               != 0xffffffffffffff14ULL
                        && g_host_read(g_host_ctx,
                                       g_edit_controls_panel + 0xe8,
                                       &slider_raw, 4) == 1
                        && 1 <= slider_raw && slider_raw <= 145
                        /* raw: (int)f - 0x92 > -146 unsigned */
                        && g_fps_popup_open != 0
                        && g_fps_popup_field_b04 == 0) {
                        g_fps_popup_limit = slider_raw;      /* 0x282afc */
                        widget_set_position(g_edit_controls_panel, 0.0f,
                                            -30.0f /* 0xc1f00000 */);
                        rich_label_render(-85.0f /* 0xc2aa0000 */,
                                          -153.0f /* 0xc3190000 */,
                                          26.0f /* 0x41d00000 */,
                                          &g_edit_controls_status_rec_a,
                                          "FPS LIMIT");
                        char label[0x60];
                        if (slider_raw == 0x91) {
                            /* raw compares the register read as float to
                             * 2.03188e-43 (bits 0x91 = the 145 floor) */
                            ui_text_style_format(label, 0x60, 0x60,
                                                 FMT_13aa70);
                        } else {
                            ui_text_style_format(label, 0x60, 0x60,
                                                 "Limit: %d FPS",
                                                 slider_raw);
                        }
                        rich_label_render(-110.0f /* 0xc2dc0000 */,
                                          -92.0f /* 0xc2b80000 */,
                                          24.0f /* 0x41c00000 */,
                                          &g_edit_controls_status_rec_b,
                                          label);
                        rich_label_render(-210.0f /* 0xc3520000 */, 0.0f,
                                          16.0f /* 0x41800000 */,
                                          &g_edit_controls_status_rec_c,
                                          "Actual FPS depends on your device."
                                          "\nPress SAVE to apply.");
                        widget_set_text(g_edit_controls_row_c,
                                        ui_string_object(
                                            g_fps_popup_limit
                                                    != g_fps_popup_applied
                                                ? "SAVE" : "SAVED"));
                        widget_set_text(g_edit_controls_row_d,
                                        ui_string_object(
                                            g_fps_popup_applied != 0x91
                                                ? "RESET" : "DEFAULT"));
                    }
                }
            }
            return;
        }
    }

    /* popup closed: mirror and park the ctx offscreen */
    g_edit_controls_open_mirror = g_fps_popup_open;
    if (g_edit_controls_extra_ctx != 0
        && widget_child_link_valid(g_edit_controls_ctx, g_ui_context) != 0)
        widget_set_position(g_edit_controls_ctx,
                            -10000.0f /* 0xc61c3c00 */,
                            -10000.0f /* 0xc61c3c00 */);
}
/* widgets chain 2 chunk 3: covers raw lines 5602-6377 (widget_clip_create_attach
 * complete — chunk 2 closed at raw 5601, so this chunk starts the nominal
 * 5501-5950 slice at 5602 and finishes the clip_create_attach straddler past 5950) */

/* ===== chain-2 additions: clip cache / menu entry tables ===== */

/* clip cache record (mirrors misc.c's clip_cache_t at 0x22d4e0) */
typedef struct {
    uint64_t clip_a;   /* +0x00 0x22d4e0 — cached clip view widget */
    uint64_t clip_b;   /* +0x08 0x22d4e8 — clip inner container */
    uint64_t clip_c;   /* +0x10 0x22d4f0 — clip content widget */
    uint64_t flag;     /* +0x18 0x22d4f8 — clip attach parent (view object) */
    uint64_t state;    /* +0x20 0x22d500 — game base latched at create time */
    float    frame_w;  /* +0x28 0x22d508 — frame pair from the 0x10f8c8 literal */
    float    frame_h;  /* +0x2c 0x22d50c */
} clip_cache_t;
extern clip_cache_t g_clip_cache;                             /* 0x22d4e0 */

extern uint64_t g_clip_span_pair;   /* 0x22d510 — span-pair copy (armed: lo = 1;
                                     * reset to the 0x10f740 literal on failure) */
extern uint64_t g_view_object_a;    /* 0x1a7d38 — view object (clip attach parent) */
extern int32_t  g_rich_stage_state; /* 0x1a7d10 — 0 idle / 1 building / 2 attached / -1 stopped */
extern const char *g_rich_status;   /* 0x1a7d50 — current status rodata ptr */
extern uint32_t RICH_ROW_MODES[];   /* 0x1a7d68 — per-config record head: the
                                     * 0xb0-byte config records grow from here */
extern uint8_t  g_menu_entry_table[]; /* 0x1c3568 — 42 records, 0xd8-byte stride */

/* rodata */
extern const char RICH_TOAST_STYLE[]; /* 0x134a97 — style name for the toast label */
extern const char g_empty_text[];     /* 0x134f22 — shared "" rodata */

/*
 * menu_config_rec_t — one 0xb0-byte record of the config table at 0x1a7d68
 * (renderer.c anchors it as RICH_ROW_MODES): mode/asset/min head followed by
 * the template block copied verbatim into the entry record.
 */
typedef struct {
    uint32_t mode;          /* +0x00 (0x1a7d68) — 2 = button widget, 3 = font export */
    uint32_t asset_index;   /* +0x04 (0x1a7d6c) — nexus_rich_asset index */
    uint64_t field_08;      /* +0x08 (0x1a7d70) */
    uint32_t min_frames;    /* +0x10 (0x1a7d78) — movie frame-count minimum */
    uint32_t _pad_14;       /* +0x14 */
    uint8_t  tail[0x98];    /* +0x18..0xb0 — template qwords */
} menu_config_rec_t;

/*
 * menu_entry_rec_t — one 0xd8-byte record of the entry table at 0x1c3568:
 * widget/bundle/styled head, the 0xb0-byte config template copy and the
 * 0x10f860 defaults literal at +0xd0.
 */
typedef struct {
    uint64_t widget;       /* +0x00 — entry widget (bundle or button) */
    uint64_t bundle;       /* +0x08 — asset bundle */
    uint64_t styled;       /* +0x10 — styled text object (mode 3) */
    uint64_t _pad;         /* +0x18 — unwritten gap */
    uint8_t  tpl[0xb0];    /* +0x20..0xd0 — config record copy */
    uint64_t defaults;     /* +0xd0 — 0x10f860 literal */
} menu_entry_rec_t;

/*
 * clip_ops_t — the ops channel bound to the game base (mirrors misc.c's
 * record; every slot takes the channel handle first and drops it). The
 * call_11a2840 slot returns the allocation (the raw assigns its result).
 */
typedef struct {
    uint64_t game_base;   /* +0x00 */
    uint64_t ctx;         /* +0x08 — channel handle (0 when unbound) */
    int  (*read_gate)(uint64_t ctx, uint64_t addr, void *out, uint64_t len);   /* +0x10 */
    int  (*write_gate)(uint64_t ctx, void *dst, const void *src, size_t len);  /* +0x18 */
    int  (*writable_check)(uint64_t ctx, void *addr, size_t len);              /* +0x20 */
    int  (*integrity_check)(uint64_t ctx);                                     /* +0x28 */
    uint64_t (*call_11a2840)(uint64_t ctx, uint64_t a);                        /* +0x30 */
    void (*call_88b344)(uint64_t ctx, uint64_t a, uint32_t b);                 /* +0x38 */
    void (*call_593f14)(uint64_t ctx, uint64_t a, uint32_t b);                 /* +0x40 */
    void (*call_594130)(uint64_t ctx, uint64_t a, uint64_t b);                 /* +0x48 */
    void (*call_59567c)(uint64_t ctx, uint64_t a);                             /* +0x50 */
    void (*call_5940d0)(uint64_t ctx, uint64_t a);                             /* +0x58 */
    void (*call_59531c)(int x, int y, uint64_t ctx, uint64_t clip);            /* +0x60 */
    void (*call_88d290)(uint64_t ctx, uint64_t a, uint32_t b);                 /* +0x68 */
    void (*call_594e80)(uint64_t ctx, uint64_t a, uint32_t b);                 /* +0x70 */
    void (*probe_log)(uint64_t ctx, const char *event);                        /* +0x78 */
} clip_ops_t;

/* ===== chain-2 additions: ops-channel helpers (misc.c / menu_engine.c) ===== */

extern void menu_probe_log(uint64_t ctx, const char *event);
                                      /* menu_engine.c @ 0017fbc8 — capped probe log */
extern int host_read_gate(uint64_t ctx, uint64_t addr, void *out,
                          uint64_t len);   /* misc.c @ 0017fe9c */
extern int host_write_gate(uint64_t ctx, void *dst, const void *src,
                           size_t len);   /* misc.c @ 0017fee0 */
extern int menu_clip_writable_check(uint64_t ctx, void *addr,
                                    size_t len);   /* menu_engine.c @ 00180034 */
extern int clip_ops_integrity_check(uint64_t ctx);   /* misc.c @ 001802bc */
extern uint64_t engine_call_11a2840_adapter(uint64_t ctx, uint64_t size);
                                      /* misc.c @ 00180540 — veneer to the allocator
                                        * (returns the allocation; misc.c declares
                                        * it void — corrected here) */
extern void engine_call_88b344_adapter(uint64_t ctx, uint64_t a,
                                       uint32_t b);   /* misc.c @ 00180548 */
extern void engine_call_593f14_adapter(uint64_t ctx, uint64_t a,
                                       uint32_t b);   /* misc.c @ 00180564 */
extern void engine_call_594130_adapter(uint64_t ctx, uint64_t a,
                                       uint64_t b);   /* misc.c @ 00180580 */
extern void engine_call_59567c_adapter(uint64_t ctx,
                                       uint64_t a);   /* misc.c @ 0018058c */
extern void engine_call_5940d0_gated(uint64_t ctx,
                                     uint64_t a);     /* misc.c @ 001805a4 */
extern void engine_call_59531c_adapter(int x, int y, uint64_t ctx,
                                       uint64_t clip); /* misc.c @ 00180724 */
extern void engine_call_88d290_adapter(uint64_t ctx, uint64_t a,
                                       uint32_t b);   /* misc.c @ 0018073c */
extern void engine_call_594e80_adapter(uint64_t ctx, uint64_t a,
                                       uint32_t b);   /* misc.c @ 00180758 */
extern int remote_ptr_field_check(const uint64_t *record, uint64_t addr,
                                  int64_t rva, uint64_t len);
                                      /* misc.c @ 00180764 */
extern int remote_widget_detached_check(const void *rec, uint64_t widget);
                                      /* misc.c @ 00180814 */
extern int remote_widget_link_check(const void *rec, uint64_t node,
                                    uint64_t parent); /* misc.c @ 001808ec */
extern int remote_record_validate(const void *rec, const uint64_t *triple);
                                      /* misc.c @ 00180b44 */
extern int clip_slot_precondition_check(void *ops, void *record,
                                        uint64_t root, uint32_t index);
                                      /* misc.c @ 00180d80 */
extern int widget_clip_move(float x, float y, void *ops, uint64_t root,
                            uint32_t entry_index, uint32_t mode);
                                      /* misc.c @ 0017fc3c */

/* ===== clip creation (menu entry 0x2b) ===== */

/* errno-preserving capped 'nexus_menu_probe' event (counter 0x1a7d20) */
static void clip_probe_log_w2(const char *event)
{
    int saved_errno = errno;

    uint32_t under_cap = g_rich_stop_log_count < 0x40;
    g_rich_stop_log_count = g_rich_stop_log_count + 1;
    if (under_cap && g_host_log != NULL)
        g_host_log(g_host_ctx, "nexus_menu_probe", event, 0);
    errno = saved_errno;
}

/* stop the rich stage: status pointer + capped 'nexus_rich_stopped' event */
static void rich_stopped_log_w2(const char *status, const char *event,
                                uint32_t value)
{
    g_rich_stage_state = -1;              /* 0x1a7d10 */
    g_rich_status = status;               /* 0x1a7d50 */
    uint32_t under_cap = g_rich_stop_log_count < 0x40;
    g_rich_stop_log_count = g_rich_stop_log_count + 1;
    if (under_cap && g_host_log != NULL)
        g_host_log(g_host_ctx, "nexus_rich_stopped", event, value);
}

/* shared clip-create failure tail: freeze the span copy (0x10f740 literal),
 * log the specific reason through the ops channel, then the partial event */
static int clip_create_partial_w2(clip_ops_t *ops, const char *reason)
{
    g_clip_span_pair = *(const uint64_t *)(const void *)(uintptr_t)0x10f740;
    ops->probe_log(ops->ctx, reason);
    clip_probe_log_w2("clip_create_partial");
    return 0;
}

/*
 * clip_create_attach_stage_w2 — the 0x2b clip-create stage: build the ops
 * channel record (ctx unbound), require a completely clean clip cache
 * (0x22d4e0 triple + flag) and span copy, allocate the 0x2c8 clip view and
 * 0x80 content widget, validate both through the ops channel (vtables
 * game+0x11c11e8/0x11c1360/0x11abcf0, detached parents, child ids,
 * writability), write the policy bytes, attach the view under
 * g_view_object_a and move both clips to the origin. Every failure logs
 * its 'clip_*' probe reason and returns 0; returns 1 only when both
 * moves succeed. @ 00172fd4 (stage half, raw 5698-6020)
 */
static int clip_create_attach_stage_w2(void)
{
    clip_ops_t ops;

    ops.game_base = g_game_base;                    /* 0x1a7cd0 */
    ops.ctx = 0;                                    /* channel unbound */
    ops.read_gate = host_read_gate;                 /* 0017fe9c */
    ops.write_gate = host_write_gate;               /* 0017fee0 */
    ops.writable_check = menu_clip_writable_check;  /* 00180034 */
    ops.integrity_check = clip_ops_integrity_check; /* 001802bc */
    ops.call_11a2840 = engine_call_11a2840_adapter; /* 00180540 */
    ops.call_88b344 = engine_call_88b344_adapter;   /* 00180548 */
    ops.call_593f14 = engine_call_593f14_adapter;   /* 00180564 */
    ops.call_594130 = engine_call_594130_adapter;   /* 00180580 */
    ops.call_59567c = engine_call_59567c_adapter;   /* 0018058c */
    ops.call_5940d0 = engine_call_5940d0_gated;     /* 001805a4 */
    ops.call_59531c = engine_call_59531c_adapter;   /* 00180724 */
    ops.call_88d290 = engine_call_88d290_adapter;   /* 0018073c */
    ops.call_594e80 = engine_call_594e80_adapter;   /* 00180758 */
    ops.probe_log = menu_probe_log;                 /* 0017fbc8 */

    /* contract: the clip cache triple, flag and span copy must all be clean */
    if (g_clip_cache.clip_a != 0 || g_clip_cache.clip_b != 0
        || g_clip_cache.clip_c != 0 || g_clip_cache.flag != 0
        || g_clip_span_pair != 0
        || g_game_base - 0x1000 > 0xfffffffffecfefffULL) {  /* raw guard */
        clip_probe_log_w2("clip_create_contract");
        clip_probe_log_w2("clip_create_refused");
        return 0;
    }

    if (ops.integrity_check(ops.ctx) == 0) {
        ops.probe_log(ops.ctx, "clip_create_pins");
        clip_probe_log_w2("clip_create_refused");
        return 0;
    }

    /* frame pair {w, h} from the rodata literal at 0x10f8c8; state = base */
    g_clip_cache.frame_w =
        *(const float *)(const void *)(uintptr_t)0x10f8c8;
    g_clip_cache.frame_h =
        *(const float *)(const void *)(uintptr_t)(0x10f8c8 + 4);
    g_clip_cache.state = g_game_base;               /* 0x22d500 */

    /* the 0x2c8-byte clip view */
    g_clip_cache.clip_a = ops.call_11a2840(ops.ctx, 0x2c8);   /* 0x22d4e0 */
    if (g_clip_cache.clip_a == 0) {
        ops.probe_log(ops.ctx, "clip_view_allocate");
        clip_probe_log_w2("clip_create_refused");
        return 0;
    }
    if (g_clip_cache.clip_a + 0x2c8 < 0x12c8        /* raw: >> 3 < 0x259 */
        || (g_clip_cache.clip_a & 7) != 0)
        return clip_create_partial_w2(&ops, "clip_view_address");
    if (ops.writable_check(ops.ctx, (void *)(uintptr_t)g_clip_cache.clip_a,
                           0x2c8) == 0)
        return clip_create_partial_w2(&ops, "clip_view_writable");
    ops.call_88b344(ops.ctx, g_clip_cache.clip_a, 1);

    uint64_t vptr = 0;
    if (g_clip_cache.clip_a - 0x1000 > 0xffffffffffffed37ULL  /* raw guard */
        || ops.read_gate(ops.ctx, g_clip_cache.clip_a, &vptr, 8) != 1
        || vptr != ops.game_base + 0x11c11e8)
        return clip_create_partial_w2(&ops, "clip_view_type");

    /* view must sit detached: +0x38 parent 0, +0x40 index -1 */
    uint64_t parent = 1;
    int32_t index = 0;
    if (g_clip_cache.clip_a + 0x40 < 0x1008         /* raw: >> 3 < 0x201 */
        || ops.read_gate(ops.ctx, g_clip_cache.clip_a + 0x38, &parent, 8) != 1
        || parent != 0
        || ops.read_gate(ops.ctx, g_clip_cache.clip_a + 0x40, &index, 4) != 1
        || index != -1)
        return clip_create_partial_w2(&ops, "clip_view_detached");

    /* the inner container at view+0xd0 must be a distinct linked child */
    if (g_clip_cache.clip_a + 0xd8 < 0x1008         /* raw: >> 3 < 0x201 */
        || ops.read_gate(ops.ctx, g_clip_cache.clip_a + 0xd0,
                         &g_clip_cache.clip_b, 8) != 1
        || g_clip_cache.clip_b == g_clip_cache.clip_a)
        return clip_create_partial_w2(&ops, "clip_inner_pointer");
    if (remote_ptr_field_check((const uint64_t *)(const void *)&ops,
                               g_clip_cache.clip_b, 0x11c1360, 0x80) == 0)
        return clip_create_partial_w2(&ops, "clip_inner_type");
    if (remote_widget_link_check(&ops, g_clip_cache.clip_b,
                                 g_clip_cache.clip_a) == 0)
        return clip_create_partial_w2(&ops, "clip_inner_membership");

    uint16_t count = 0;
    if (g_clip_cache.clip_a + 0x50 < 0x1002         /* raw: >> 1 < 0x801 */
        || ops.read_gate(ops.ctx, g_clip_cache.clip_a + 0x4e, &count, 2) != 1
        || count != 1)
        return clip_create_partial_w2(&ops, "clip_view_count");
    count = 0;
    if (g_clip_cache.clip_b + 0x50 < 0x1002
        || ops.read_gate(ops.ctx, g_clip_cache.clip_b + 0x4e, &count, 2) != 1
        || count != 0)
        return clip_create_partial_w2(&ops, "clip_inner_count");

    /* the 0x80-byte content widget under the inner container */
    g_clip_cache.clip_c = ops.call_11a2840(ops.ctx, 0x80);    /* 0x22d4f0 */
    if (g_clip_cache.clip_c == 0)
        return clip_create_partial_w2(&ops, "clip_content_allocate");
    if (g_clip_cache.clip_c + 0x80 < 0x1080         /* raw: >> 7 < 0x21 */
        || (g_clip_cache.clip_c & 7) != 0)
        return clip_create_partial_w2(&ops, "clip_content_address");
    if (g_clip_cache.clip_c == g_clip_cache.clip_a
        || g_clip_cache.clip_c == g_clip_cache.clip_b)
        return clip_create_partial_w2(&ops, "clip_content_alias");
    if (ops.writable_check(ops.ctx, (void *)(uintptr_t)g_clip_cache.clip_c,
                           0x80) == 0)
        return clip_create_partial_w2(&ops, "clip_content_writable");
    ops.call_593f14(ops.ctx, g_clip_cache.clip_c, 0);
    if (remote_ptr_field_check((const uint64_t *)(const void *)&ops,
                               g_clip_cache.clip_c, 0x11abcf0, 0x80) == 0)
        return clip_create_partial_w2(&ops, "clip_content_type");
    if (remote_widget_detached_check(&ops, g_clip_cache.clip_c) == 0)
        return clip_create_partial_w2(&ops, "clip_content_detached");
    count = 0;
    if (g_clip_cache.clip_c + 0x50 < 0x1002
        || ops.read_gate(ops.ctx, g_clip_cache.clip_c + 0x4e, &count, 2) != 1
        || count != 0)
        return clip_create_partial_w2(&ops, "clip_content_count");
    ops.call_594130(ops.ctx, g_clip_cache.clip_b, g_clip_cache.clip_c);
    if (remote_record_validate(&ops, &g_clip_cache.clip_a) == 0)
        return clip_create_partial_w2(&ops, "clip_content_parenting");

    ops.call_594e80(ops.ctx, g_clip_cache.clip_a, 1);
    ops.call_594e80(ops.ctx, g_clip_cache.clip_b, 1);
    ops.call_594e80(ops.ctx, g_clip_cache.clip_c, 1);

    /* policy words on the view: clip on at +0xe0, axes/controller pairs */
    uint8_t clip_on = 1;
    uint16_t axes = 0;    /* raw: the low half of the scratch word (zero) */
    if (ops.writable_check(ops.ctx,
                           (void *)(uintptr_t)(g_clip_cache.clip_a + 0xe0),
                           1) == 0
        || ops.writable_check(ops.ctx,
                              (void *)(uintptr_t)(g_clip_cache.clip_a + 0xe7),
                              2) == 0
        || ops.writable_check(ops.ctx,
                              (void *)(uintptr_t)(g_clip_cache.clip_a + 0x280),
                              2) == 0
        || ops.writable_check(ops.ctx,
                              (void *)(uintptr_t)(g_clip_cache.clip_a + 0x27e),
                              1) == 0)
        return clip_create_partial_w2(&ops, "clip_policy_writable");
    if (ops.write_gate(ops.ctx,
                       (void *)(uintptr_t)(g_clip_cache.clip_a + 0xe0),
                       &clip_on, 1) == 0)
        return clip_create_partial_w2(&ops, "clip_policy_clip");
    if (ops.write_gate(ops.ctx,
                       (void *)(uintptr_t)(g_clip_cache.clip_a + 0xe7),
                       &axes, 2) == 0)
        return clip_create_partial_w2(&ops, "clip_policy_axes");
    if (ops.write_gate(ops.ctx,
                       (void *)(uintptr_t)(g_clip_cache.clip_a + 0x280),
                       &axes, 2) == 0)
        return clip_create_partial_w2(&ops, "clip_policy_controller");
    ops.call_88d290(ops.ctx, g_clip_cache.clip_a + 0xf0, 0);
    g_clip_span_pair = (g_clip_span_pair & 0xffffffff00000000ULL) | 1;
                                     /* raw: lo word = 1 (slot armed) */

    if (clip_slot_precondition_check(&ops, &g_clip_cache, 0, 0) == 0)
        return clip_create_partial_w2(&ops, "clip_create_postcondition");

    /* attach contract: re-validate the slot, flag word still clear */
    if (clip_slot_precondition_check(&ops, &g_clip_cache, 0, 0) == 0
        || g_clip_cache.flag != 0) {
        ops.probe_log(ops.ctx, "clip_attach_contract");
        clip_probe_log_w2("clip_attach_refused");
        return 0;
    }

    /* attach the view under the view object (vptr game+0x11abcf0) */
    uint64_t view = g_view_object_a;                /* 0x1a7d38 */
    if (view + 0x80 < 0x1080                        /* raw: >> 7 < 0x21 */
        || (view & 7) != 0
        || g_clip_cache.clip_a == view || g_clip_cache.clip_b == view
        || g_clip_cache.clip_c == view) {
        ops.probe_log(ops.ctx, "clip_attach_parent_address");
        clip_probe_log_w2("clip_attach_refused");
        return 0;
    }
    uint64_t parent_vptr = 0;
    if (ops.read_gate(ops.ctx, view, &parent_vptr, 8) != 1
        || parent_vptr != g_game_base + 0x11abcf0) {
        ops.probe_log(ops.ctx, "clip_attach_parent_type");
        clip_probe_log_w2("clip_attach_refused");
        return 0;
    }
    uint64_t parent_ctx = 0;
    if (ops.read_gate(ops.ctx, view + 0x30, &parent_ctx, 8) != 1
        || parent_ctx != g_ui_scene_root) {
        ops.probe_log(ops.ctx, "clip_attach_parent_context");
        clip_probe_log_w2("clip_attach_refused");
        return 0;
    }
    if (ops.integrity_check(ops.ctx) == 0) {
        ops.probe_log(ops.ctx, "clip_attach_pins");
        clip_probe_log_w2("clip_attach_refused");
        return 0;
    }
    ops.call_594130(ops.ctx, view, g_clip_cache.clip_a);
    g_clip_cache.flag = view;                       /* 0x22d4f8 — attach parent */
    if (clip_slot_precondition_check(&ops, &g_clip_cache,
                                     g_ui_scene_root, 0) == 0) {
        g_clip_span_pair =
            *(const uint64_t *)(const void *)(uintptr_t)0x10f740;
        ops.probe_log(ops.ctx, "clip_attach_postcondition");
        clip_probe_log_w2("clip_attach_partial");
        return 0;
    }

    /* move both clips to the origin through the ops channel (the raw passes
     * the coordinates only in the float registers — not recovered) */
    int moved = widget_clip_move(0.0f, 0.0f, &ops, g_ui_scene_root, 0, 0);
    if (moved != 1) {
        clip_probe_log_w2(moved == 0 ? "clip_view_move_refused"
                                     : "clip_view_move_partial");
        return 0;
    }
    int moved_c = widget_clip_move(0.0f, 0.0f, &ops, g_ui_scene_root, 0, 1);
    if (moved_c != 1) {
        ops.probe_log((uint64_t)moved_c, moved_c == 0
                          ? "clip_content_move_refused"
                          : "clip_content_move_partial");
        return 0;
    }
    return 1;
}

/*
 * widget_clip_create_attach — build one menu entry: for entry 0x2b first run
 * the clip-create stage (clean-cache contract, allocation, vtable/detach/
 * count/writability probes, policy bytes, attach under g_view_object_a,
 * origin moves); entries past 0x2a require the armed clip slot. Then load
 * the entry's asset bundle, gate its frame count, and either validate the
 * bundle as the entry widget (modes 0/1/3; mode 3 also builds the styled
 * text object) or allocate a 0x260 button (mode 2: constructor contract,
 * rich_binding_bind row = entry index, empty text). The widget is parked at
 * (0,0), attached to the view object (the clip parent past 0x2a) and the
 * 0xb0-byte config template is copied into the 0xd8-stride entry table.
 * Failures stop the rich stage ('nexus_rich_stopped', cap 0x40); probe
 * failures log 'nexus_menu_probe' (cap 0x40, errno preserved).
 * @ 00172fd4
 */
void widget_clip_create_attach(uint32_t entry_index)
{
    if (entry_index == 0x2b) {
        int stage_ok;
        if (ui_scene_fonts_ready() == 0) {
            clip_probe_log_w2("clip_stage_changed");
            stage_ok = 0;
        } else {
            uint8_t scene_flag = 1;
            if (g_ui_scene_root + 0x19d < 0x1001
                || g_host_read(g_host_ctx, g_ui_scene_root + 0x19c,
                               &scene_flag, 1) != 1
                || scene_flag != 0) {
                clip_probe_log_w2("clip_stage_busy");
                stage_ok = 0;
            } else {
                stage_ok = clip_create_attach_stage_w2();
            }
        }
        if (stage_ok == 0) {
            rich_stopped_log_w2("main page content construction",
                                "main_page_content_construction",
                                entry_index);   /* raw 0x138b2e */
            return;
        }
        /* clip created: fall through to build menu entry 0x2b */
    } else if (entry_index > 0x2a && g_clip_span_pair == 0) {
        /* entries past the clip need the armed slot */
        clip_probe_log_w2("clip_missing_live");
        rich_stopped_log_w2("main page content construction",
                            "main_page_content_construction",
                            entry_index);       /* raw 0x138b2e */
        return;
    }

    /* ===== shared menu-entry build ===== */

    const menu_config_rec_t *cfg =
        (const menu_config_rec_t *)(const void *)RICH_ROW_MODES + entry_index;
    menu_entry_rec_t *entry =
        (menu_entry_rec_t *)(void *)(g_menu_entry_table
                                     + (size_t)entry_index * 0xd8);
    uint32_t mode = cfg->mode;

    /* load the entry's asset bundle and gate its frame count */
    uint64_t bundle =
        engine_sc_bundle_load(nexus_rich_asset(cfg->asset_index));
    entry->bundle = bundle;                         /* 0x1c3570 */
    if (movie_frame_count_check(bundle, (int)cfg->asset_index,
                                (int)cfg->min_frames) == 0) {
        rich_stopped_log_w2("asset movie contract",
                            "asset_movie_contract", entry_index); /* 0x133d1c */
        return;
    }

    /* the bundle must sit detached: +0x38 parent 0, +0x40 index -1 */
    {
        uint64_t parent = 1;
        int32_t index = 0;
        if (bundle + 0x38 < 0x1000
            || (bundle & 0xfffffffffffffff8ULL) == 0xffffffffffffffc0ULL
            || g_host_read(g_host_ctx, bundle + 0x38, &parent, 8) != 1
            || parent != 0
            || bundle + 0x40 < 0x1000
            || (bundle & 0xfffffffffffffffcULL) == 0xffffffffffffffbcULL
            || g_host_read(g_host_ctx, bundle + 0x40, &index, 4) != 1
            || index != -1) {
            rich_stopped_log_w2("asset movie contract",
                                "asset_movie_contract",
                                entry_index);       /* raw 0x133d1c */
            return;
        }
    }

    if (mode != 2) {
        /* the bundle itself is the entry widget */
        entry->widget = bundle;
        engine_call_5d7c30(bundle, (int)cfg->min_frames);

        if (mode == 3) {
            /* font export: styled text object under the bundle */
            uint64_t styled = engine_call_5d8ad4(
                bundle, (uint64_t)(uintptr_t)RICH_TOAST_STYLE);  /* 0x134a97 */
            entry->styled = styled;                 /* 0x1c3578 */

            uint64_t vptr = 0;
            int rd = (styled + 8) >> 3 >= 0x201
                     && g_host_read(g_host_ctx, styled, &vptr, 8) == 1;
            int plausible = 0xfff < vptr && rd && (vptr & 7) == 0;
            if ((plausible ? vptr : 0) == g_game_base + 0x11abad8) {
                uint64_t owner = 0;
                if (0xfff < styled + 0x30
                    && (styled & 0xfffffffffffffff8ULL)
                           != 0xffffffffffffffc8ULL
                    && g_host_read(g_host_ctx, styled + 0x30, &owner, 8) == 1
                    && (owner == 0 || owner == g_ui_scene_root)
                    && widget_chain_links_unique_check(bundle, styled) != 0) {
                    uint16_t count = 0;
                    if (styled + 0xb0 < 0x1000
                        || (styled & 0xfffffffffffffffeULL)
                               == 0xffffffffffffff4eULL
                        || g_host_read(g_host_ctx, styled + 0xb0,
                                       &count, 2) != 1
                        || count < 1 || 0x100 < count) {
                        rich_stopped_log_w2("font export size",
                                            "font_export_size",
                                            entry_index);   /* 0x13b662 */
                        return;
                    }
                    goto shared_attach;
                }
            }
            rich_stopped_log_w2("font export owned txt",
                                "font_export_owned_txt",
                                entry_index);       /* raw 0x1326dc */
            return;
        }
    } else {
        /* mode 2: allocate the button widget */
        uint64_t widget = engine_call_11a2840(0x260);
        entry->widget = widget;
        if (widget == 0) {
            rich_stopped_log_w2("button allocation",
                                "button_allocation",
                                entry_index);       /* raw 0x13685f */
            return;
        }
        engine_call_887c14();
        if (sc_object_release(widget) == 0) {
            rich_stopped_log_w2("button sound cache contract",
                                "button_sound_cache_contract",
                                entry_index);       /* raw 0x136d39 */
            return;
        }

        /* constructor contract: vptr, detach state and scratch fields */
        uint64_t vptr = 0;
        int rd = (widget + 8) >> 3 >= 0x201
                 && g_host_read(g_host_ctx, widget, &vptr, 8) == 1;
        int plausible = 0xfff < vptr && rd && (vptr & 7) == 0;
        uint64_t parent = 1;
        int32_t index = 0;
        uint64_t scratch_a8 = 1, scratch_120 = 1;
        uint8_t flag_1f0 = 1;
        if ((plausible ? vptr : 0) != g_game_base + 0x11c0a48
            || widget + 0x38 < 0x1000
            || (widget & 0xfffffffffffffff8ULL) == 0xffffffffffffffc0ULL
            || g_host_read(g_host_ctx, widget + 0x38, &parent, 8) != 1
            || parent != 0
            || widget + 0x40 < 0x1000
            || (widget & 0xfffffffffffffffcULL) == 0xffffffffffffffbcULL
            || g_host_read(g_host_ctx, widget + 0x40, &index, 4) != 1
            || index != -1
            || widget + 0xa8 < 0x1000
            || (widget & 0xfffffffffffffff8ULL) == 0xffffffffffffff50ULL
            || g_host_read(g_host_ctx, widget + 0xa8, &scratch_a8, 8) != 1
            || scratch_a8 != 0
            || widget + 0x120 < 0x1000
            || (widget & 0xfffffffffffffff8ULL) == 0xfffffffffffffed8ULL
            || g_host_read(g_host_ctx, widget + 0x120, &scratch_120, 8) != 1
            || scratch_120 != 0
            || widget + 0x1f1 < 0x1001
            || g_host_read(g_host_ctx, widget + 0x1f0, &flag_1f0, 1) != 1
            || flag_1f0 != 0) {
            rich_stopped_log_w2("button constructor contract",
                                "button_constructor_contract",
                                entry_index);       /* raw 0x133d31 */
            return;
        }

        if (rich_binding_bind(&entry->widget, bundle, entry_index) == 0) {
            rich_stopped_log_w2("button movie binding",
                                "button_movie_binding",
                                entry_index);       /* raw 0x137df9 */
            return;
        }
        engine_call_5d7c30(bundle, (int)cfg->min_frames);
        uint64_t text_obj = ui_string_object(g_empty_text);   /* 0x134f22 */
        if (text_obj == 0) {
            rich_stopped_log_w2("string budget",
                                "string_budget",
                                entry_index);       /* raw 0x136871 */
            return;
        }
        widget_set_text(entry->widget, text_obj);
    }

shared_attach:
    /* the +0x48 display flag must match the widget kind (1 for buttons) */
    {
        uint8_t flag = 0xff;
        if (entry->widget + 0x49 < 0x1001
            || g_host_read(g_host_ctx, entry->widget + 0x48, &flag, 1) != 1
            || flag != (uint8_t)(mode == 2)) {
            rich_stopped_log_w2("display input flag",
                                "display_input_flag",
                                entry_index);       /* raw 0x134480 */
            return;
        }
    }

    /* scale matrix at +0x10 must read {sx, 0, 0, sy} with sx, sy in
     * (0, 128], and the owner at +0x30 must be NULL or the scene root */
    {
        float m[4];
        uint64_t owner = 0;
        if (entry->widget + 0x20 < 0x1000
            || (entry->widget & 0xfffffffffffffff0ULL)
                   == 0xffffffffffffffe0ULL
            || g_host_read(g_host_ctx, entry->widget + 0x10, m, 0x10) != 1
            || !isfinite(m[0]) || !isfinite(m[1]) || !isfinite(m[2])
            || !isfinite(m[3])
            || m[1] != 0.0f || m[2] != 0.0f
            || !(m[0] > 0.0f) || !(m[3] > 0.0f)
            || m[0] > 128.0f || m[3] > 128.0f
            || entry->widget + 0x38 < 0x1000
            || (entry->widget & 0xfffffffffffffff8ULL)
                   == 0xffffffffffffffc8ULL
            || g_host_read(g_host_ctx, entry->widget + 0x30, &owner, 8) != 1
            || (owner != 0 && owner != g_ui_scene_root)) {
            rich_stopped_log_w2("display matrix context",
                                "display_matrix_context",
                                entry_index);       /* raw 0x13b13d */
            return;
        }
    }

    /* park at the origin and attach to the carrier */
    widget_set_position(entry->widget, 0.0f, 0.0f);
    uint64_t carrier = g_view_object_a;             /* 0x1a7d38 */
    if (entry_index > 0x2a)
        carrier = g_clip_cache.flag;                /* 0x22d4f8 — clip parent */
    engine_call_594130(carrier, entry->widget);
    if (widget_child_link_valid(entry->widget, carrier) == 0) {
        rich_stopped_log_w2("carrier membership",
                            "carrier_membership",
                            entry_index);           /* raw 0x1373c8 */
        return;
    }

    /* copy the 0xb0-byte config template into the entry record (the raw
     * explodes this into 22 qword stores) and stamp the defaults literal */
    memcpy(entry->tpl, cfg, 0xb0);
    entry->defaults = *(const uint64_t *)(const void *)(uintptr_t)0x10f860;
}
/* widgets chain 2 chunk 4: covers raw lines 6379-7123 (rich_button_style_apply
 * complete + nexus_rich_action complete; the nominal 6379-6850 slice ends
 * mid-function, so this chunk closes both straddlers) */

/* ===== chain-2 additions: chooser / camera action state ===== */

/* chooser block view (base 0x281b08; misc.c externs the same record as
 * g_chooser_state) — only the words the action dispatch touches */
typedef struct {
    uint64_t owner_context;  /* +0x000 0x281b08 */
    uint64_t root;           /* +0x008 0x281b10 */
    uint64_t _r10;           /* +0x010 */
    uint64_t canvas;         /* +0x018 0x281b20 */
    uint64_t _gap[0x19];     /* +0x020..0xb0 */
    uint64_t hint;           /* +0x0b0 0x281bb8 */
    uint64_t _gap2[0x12];    /* +0x0b8..0x148 */
    uint64_t buttons[6];     /* +0x148 0x281c50, 0x98 stride */
    uint64_t _gap3[0xab];    /* +0x178..0x608 */
    uint32_t built;          /* +0x608 0x282110 — built flag */
    uint32_t open;           /* +0x60c 0x282114 — reserved */
    uint32_t open_flag;      /* +0x610 0x282118 — g_chooser_open (digest) */
    uint32_t flag_614;       /* +0x614 0x28211c — built pair hi word */
    uint32_t _pad[5];        /* +0x618..0x624 */
    uint32_t save_enabled;   /* +0x624 0x28212c — font-chooser save gate */
} chooser_state_view_t;      /* base 0x281b08 */

extern chooser_state_view_t g_chooser_state;  /* 0x281b08 — chooser block
                                               * (misc.c mirror: g_chooser_state) */

/* camera / rich panel action counters and page state (widgets.c-owned) */
uint32_t g_script_action_count;  /* 0x282b08 — capped 0x20: script_port_font_ui
                                  * save event counter */
uint32_t g_camera_log_count;     /* 0x2839f4 — capped 0x30: script_port_camera
                                  * event counter (always incremented) */
int32_t  g_camera_page_counter;  /* 0x2839c8 — camera popup page-step counter */

/* rich panel rows (renderer.c/misc.c block; kind-1 rows at 0x2833a0 and
 * kind-0 rows at 0x283678, 0x78-byte stride) */
/* panel_row_t defined earlier in the file (e-chain header region) */
extern panel_row_t g_rich_panel_rows_a[6];  /* 0x2833a0 — kind-1 rows
                                             * (misc.c: g_rich_panel_rows_a) */
extern panel_row_t g_rich_panel_rows_b[6];  /* 0x283678 — kind-0 rows
                                             * (misc.c: g_rich_panel_rows_b) */
extern uint64_t g_rich_panel_ctx;           /* 0x283340 — camera panel widget
                                             * (misc.c: g_rich_panel_ctx) */

/* ===== chain-2 additions: action/render helpers ===== */

extern int (*g_host_ready)(void *ctx);  /* 0x1a7cf0 — host ready gate (digest) */
extern int g_plus_service_installed;    /* 0x1a7d0c — entitlement service gate (misc.c) */
extern uint32_t g_host_init_latch;      /* 0x1a7d08 — init/render latch (renderer.c) */
extern uint32_t g_rich_hidden_latch;    /* 0x1a7d14 — rich scene parked hidden (renderer.c) */
extern int32_t g_menu_entry_index;      /* 0x1a7d18 — live menu entry count (misc.c) */
extern uint32_t g_rich_chat_ready;      /* 0x2839fc — rich chat rows positioned (renderer.c) */

extern int menu_entry_live_check(uint64_t *entry, const int32_t *config);
                                      /* renderer.c @ 0017609c — live ownership/type */
extern int menu_entry_toggle_apply(uint64_t widget, int value);
                                      /* menu_engine.c @ 00176418 — toggle, return value */
extern int android_toggle_queue(void);
                                      /* misc.c @ 0014d350 — queue the android toggle */
extern void script_port_chat_row_action(uint32_t row);
                                      /* script port @ 00192300 — chat row action (1|2) */
extern int script_port_font_choice_save(uint32_t choice);
                                      /* script port @ 00191e38 — save font choice 0..5 */
extern int script_port_client_performance_apply(int mode);
                                      /* script port @ 00191cd0 — apply fps limit
                                        * (0 = default/145, -1 = current) */
extern void script_port_camera_page_select(uint32_t page, uint32_t a,
                                           int32_t step);
                                      /* script port @ 00191c34 — camera page select */
extern int ui_latch_test_and_set(int value, volatile int *latch);
                                      /* menu_engine.c @ 00193f80 */
extern int widget_edit_controls_bound_check(uint64_t widget);
                                      /* misc.c @ 00179dfc — bound under 0x282158 */
extern int widget_popover_bound_check(uint64_t widget);
                                      /* misc.c @ 0017df44 — bound under popover ctx */

/*
 * menu_config_live_t — live view of one 0xb0-byte config record at 0x1a7d68
 * (renderer.c anchors the table as RICH_ROW_MODES): mode/asset/color head,
 * frame floats, persistent flag and caption text.
 */
typedef struct {
    uint32_t mode;         /* +0x00 (0x1a7d68) — 2 = button, 3 = font export */
    uint32_t asset_index;  /* +0x04 (0x1a7d6c) — nexus_rich_asset index */
    uint32_t field_08;     /* +0x08 (0x1a7d70) — value dword */
    uint32_t color;        /* +0x0c (0x1a7d74) — color/outline dword */
    int32_t  min_frames;   /* +0x10 (0x1a7d78) — movie frame-count minimum */
    float    x;            /* +0x14 (0x1a7d7c) — frame x */
    float    y;            /* +0x18 (0x1a7d80) — frame y */
    float    scale;        /* +0x1c (0x1a7d84) — frame scale */
    float    field_20;     /* +0x20 (0x1a7d88) */
    float    width;        /* +0x24 (0x1a7d8c) — target width */
    float    height;       /* +0x28 (0x1a7d90) — target height */
    uint8_t  persistent;   /* +0x2c (0x1a7d94) — persistent/live-refresh flag */
    uint8_t  _pad_2d;      /* +0x2d */
    char     text[0x82];   /* +0x2e (0x1a7d96) — caption text */
} menu_config_live_t;      /* 0xb0 bytes */

/*
 * menu_entry_live_t — live view of one 0xd8-byte entry record at 0x1c3568:
 * widget/bundle/styled head, the cached config template (read back through
 * menu_config_live_t) and the live state byte at +0xd4.
 */
typedef struct {
    uint64_t widget;       /* +0x00 — entry widget (bundle or button) */
    uint64_t bundle;       /* +0x08 — asset bundle */
    uint64_t styled;       /* +0x10 — styled text object (mode 3) */
    uint8_t  tpl[0xb0];    /* +0x20..0xd0 — cached config template */
    uint32_t defaults_lo;  /* +0xd0 — 0x10f860 literal lo */
    uint32_t state;        /* +0xd4 — live state (hi dword of the defaults pair) */
} menu_entry_live_t;       /* 0xd8 bytes */

/* capped 'script_port_camera' event log (counter 0x2839f4, cap 0x30) */
static void camera_log_event_w2(const char *event, uint32_t value)
{
    uint32_t under_cap = g_camera_log_count < 0x30;
    g_camera_log_count = g_camera_log_count + 1;
    if (under_cap && g_host_log != NULL)
        g_host_log(g_host_ctx, "script_port_camera", event, value);
}

/* ===== menu entry live refresh ===== */

/*
 * rich_button_style_apply — refresh one live menu entry (0x1c3568 table,
 * 0xd8 stride) against its config record (0x1a7d68, 0xb0 stride): rebind the
 * palette bundle when the asset changed (mode 2), refresh the caption /
 * styled text when it differs, re-frame the widget bounds from the anchor
 * extents (scale (sx*w)/dx in [0.0001, 128], midpoint positioned, |delta|
 * <= 0.5) or fall back to the configured frame floats, and re-stamp the
 * whole config template into the entry record. Failures stop the rich stage
 * ('nexus_rich_stopped', cap 0x40). Returns 1 on success, -1 on failure.
 * @ 0017474c
 */
int rich_button_style_apply(uint32_t entry_index)
{
    menu_entry_live_t *entry =
        (menu_entry_live_t *)(void *)(g_menu_entry_table
                                      + (size_t)entry_index * 0xd8);
    const menu_config_live_t *cfg =
        (const menu_config_live_t *)(const void *)RICH_ROW_MODES
        + entry_index;
    const menu_config_live_t *cached =
        (const menu_config_live_t *)(const void *)entry->tpl;
    uint64_t widget = entry->widget;

    if (menu_entry_live_check((uint64_t *)(void *)entry,
                              (const int32_t *)(const void *)cfg) == 0) {
        rich_stopped_log_w2("live ownership or type",
                            "live_ownership_or_type",
                            entry_index);              /* raw 0x13b70b */
        return -1;
    }

    /* mode 2 with a changed asset: rebind the palette bundle */
    if (cfg->mode == 2 && cfg->asset_index != cached->asset_index) {
        uint64_t bundle =
            engine_sc_bundle_load(nexus_rich_asset(cfg->asset_index));
        if (movie_frame_count_check(bundle, (int)cfg->asset_index,
                                    cfg->min_frames) != 0) {
            uint64_t parent = 1;
            float index_f = 0.0f;
            if (0xfff < bundle + 0x38
                && (bundle & 0xfffffffffffffff8ULL) != 0xffffffffffffffc0ULL
                && g_host_read(g_host_ctx, bundle + 0x38, &parent, 8) == 1
                && parent == 0
                && 0xfff < bundle + 0x40
                && (bundle & 0xfffffffffffffffcULL) != 0xffffffffffffffbcULL
                && g_host_read(g_host_ctx, bundle + 0x40, &index_f, 4) == 1
                && index_f == -1.0f /* raw: -NAN, sign-extended -1 */) {
                if (rich_binding_bind(&entry->widget, bundle,
                                      entry_index) == 0) {
                    rich_stopped_log_w2("palette binding contract",
                                        "palette_binding_contract",
                                        entry_index);   /* raw 0x1363c8 */
                    return -1;
                }
                engine_call_5d7c30(bundle, cfg->min_frames);
                widget_set_text(entry->widget,
                                ui_string_object(g_empty_text)); /* 0x134f22 */
                entry->state = 0;                       /* raw +0xd4 */
                widget = entry->widget;
                goto nonpersistent_copy;
            }
        }
        rich_stopped_log_w2("palette movie contract",
                            "palette_movie_contract",
                            entry_index);               /* raw 0x1351e4 */
        return -1;
    }

nonpersistent_copy:
    /* non-persistent entries: park and re-stamp */
    if (cfg->persistent == 0) {
        if (entry->state == 0 || cached->persistent != 0)
            widget_set_position(widget, 0.0f, 0.0f);  /* raw drops the (0,0) */
        memcpy(entry->tpl, cfg, 0xa8);   /* raw: 21 exploded qword stores */
        entry->state = 1;                            /* raw +0xd4 */
        return 1;
    }

    /* persistent entries: refresh by mode */
    uint32_t mode = cfg->mode;
    if (mode == 2) {
        if (entry->state == 0
            || strcmp(cfg->text, cached->text) != 0) {
            uint64_t str_obj = ui_string_object(cfg->text);
            if (str_obj == 0) {
                rich_stopped_log_w2("persistent button caption budget",
                                    "persistent_button_caption_budget",
                                    entry_index);       /* raw 0x13b722 */
                return -1;
            }
            widget_set_text(widget, str_obj);
            if (menu_entry_live_check((uint64_t *)(void *)entry,
                                      (const int32_t *)(const void *)cfg) == 0) {
                rich_stopped_log_w2("button caption ownership",
                                    "button_caption_ownership",
                                    entry_index);       /* raw 0x13a5d3 */
                return -1;
            }
            mode = cfg->mode;   /* raw re-reads the mode word */
        }
    }

    if (mode != 3)
        goto bounds_reframe;

    /* mode 3: styled text object refresh */
    if (!(entry->state != 0 && strcmp(cfg->text, cached->text) == 0)) {
        uint64_t str_obj = ui_string_object(cfg->text);
        if (str_obj == 0) {
            rich_stopped_log_w2("persistent string budget",
                                "persistent_string_budget",
                                entry_index);           /* raw 0x139568 */
            return -1;
        }
        engine_call_5921a0(entry->styled, str_obj);
        if (entry->state != 0)
            goto color_check;
    }

color_refresh:
    {
        uint64_t styled = entry->styled;
        uint32_t color = 0;
        if (styled + 0x84 < 0x1000
            || (styled & 0xfffffffffffffffcULL) == 0xffffffffffffff78ULL
            || g_host_read(g_host_ctx, styled + 0x84, &color, 4) != 1) {
            rich_stopped_log_w2("outline read",
                                "outline_read",
                                entry_index);           /* raw 0x138be0 */
            return -1;
        }
        engine_call_592250(styled, cfg->color);
        uint32_t color_rb = 0, color_80 = 0;
        if (0xfff < styled + 0x84
            && (styled & 0xfffffffffffffffcULL) != 0xffffffffffffff78ULL
            && g_host_read(g_host_ctx, styled + 0x84, &color_rb, 4) == 1
            && color_rb == color) {
            if (0xfff < styled + 0x80
                && (styled & 0xfffffffffffffffcULL) != 0xffffffffffffff7cULL
                && g_host_read(g_host_ctx, styled + 0x80, &color_80, 4) == 1
                && color_80 == cfg->color) {
                if (entry->state != 0)
                    goto styled_frame_check;
                goto styled_reframe;
            }
        }
        rich_stopped_log_w2("text color or outline",
                            "text_color_or_outline",
                            entry_index);               /* raw 0x13c897 */
        return -1;
    }

color_check:
    if (cfg->color != cached->color)
        goto color_refresh;

styled_frame_check:
    if (cached->persistent == 0
        || cfg->x != *(const float *)(const void *)&cached->x
        || cfg->y != cached->y
        || cfg->scale != cached->scale)
        goto styled_reframe;
    goto template_copy;

styled_reframe:
    widget_set_position(widget, 0.0f, 0.0f);            /* raw: (0, ZEXT(0), w) */
    /* re-frame the STYLED object with the configured floats */
    widget_set_scale(entry->styled, cfg->scale, cfg->scale);   /* raw: (cfg+0x1c, w) */
    widget_set_position(entry->styled, cfg->x, cfg->y);        /* raw: (cfg+0x14, w) */
    goto template_copy;

bounds_reframe:
    if (entry->state == 0 || cached->persistent == 0
        || cfg->x != cached->x || cfg->y != cached->y
        || cfg->scale != cached->scale || cfg->field_20 != cached->field_20
        || cfg->width != cached->width || cfg->height != cached->height) {
        float w = cfg->width;                     /* raw +0x24 */
        if (w != 0.0f && w > 0.0f) {
            if (ui_scene_fonts_ready() != 0) {
                float m[4];
                if (0xfff < widget + 0x20
                    && (widget & 0xfffffffffffffff0ULL)
                           != 0xffffffffffffffe0ULL
                    && g_host_read(g_host_ctx, widget + 0x10, m, 0x10) == 1
                    && isfinite(m[0]) && isfinite(m[1]) && isfinite(m[2])
                    && isfinite(m[3])
                    && m[1] == 0.0f && m[2] == 0.0f
                    && m[0] > 0.0f && m[3] > 0.0f
                    && m[0] <= 128.0f && m[3] <= 128.0f) {
                    float anchor[4];
                    widget_set_anchor(widget, anchor);
                    float dx = anchor[2] - anchor[0];
                    float dy = anchor[3] - anchor[1];
                    /* raw: NEON range check of {dx, dy} vs the 0x10f9f0
                     * literal pair — 0 < d <= upper bound */
                    if (dx > 0.0f && dx <= 128.0f
                        && dy > 0.0f && dy <= 128.0f) {
                        float new_sx = (m[0] * w) / dx;
                        float new_sy = (m[3] * cfg->height) / dy;
                        if (new_sx >= 0.0001f && new_sx <= 128.0f
                            && new_sy >= 0.0001f && new_sy <= 128.0f) {
                            widget_set_scale(widget, new_sx, new_sy);
                            float anchor2[4];
                            widget_set_anchor(widget, anchor2);
                            float ddx = fabsf((anchor2[2] - anchor2[0]) - w);
                            float ddy = fabsf((anchor2[3] - anchor2[1])
                                              - cfg->height);
                            if (ddx <= 0.5f && ddy <= 0.5f) {
                                widget_set_position(
                                    widget,
                                    (anchor2[2] + anchor2[0]) * 0.5f - cfg->x,
                                    (anchor2[3] + anchor2[1]) * 0.5f - cfg->y);
                                if (ui_scene_fonts_ready() != 0)
                                    goto template_copy;
                            } else {
                                /* restore the original scale matrix */
                                widget_set_scale(widget, m[0], m[3]);
                            }
                        }
                    }
                }
            }
            rich_stopped_log_w2("native bounds sizing",
                                "native_bounds_sizing",
                                entry_index);           /* raw 0x13b743 */
            return -1;
        }
        /* degenerate width: plain configured frame */
        widget_set_scale(widget, cfg->scale, cfg->scale);   /* raw: (cfg+0x1c, w) */
        widget_set_position(widget, cfg->x, cfg->y);        /* raw: (cfg+0x14, w) */
    }

template_copy:
    memcpy(entry->tpl, cfg, 0xa8);       /* raw: 21 exploded qword stores */
    entry->state = 1;                                /* raw +0xd4 */
    return 1;
}

/* ===== rich action dispatch (button/toggle callbacks) ===== */

/*
 * nexus_rich_action — dispatch a widget activation from the rich view:
 * the native BATTLE launcher (protected-channel android toggle, logged
 * 'battle_menu_launcher'), the chat rows (row 1/2 actions), the fps popup
 * rows (close / SAVE / RESET with script_port_client_performance_apply and the
 * slider register push at panel+0xe8), the font chooser (hint close,
 * choices 0-5 saved through script_port_font_choice_save, logged
 * 'script_port_font_ui'), the camera popup rows (open, close/page select
 * through script_port_camera_page_select) and, under the stage-2 +
 * init-latch gate, the live menu-entry toggles (menu_entry_toggle_apply,
 * writing the entry value to *out). Returns 1 when handled (*out = value
 * or 0), 0 when the gates refused. @ 00175738
 */
int nexus_rich_action(uint64_t widget, uint32_t *out)
{
    uint32_t action_count = g_script_action_count;    /* 0x282b08 */

    /* --- native BATTLE launcher: queue the android toggle --- */
    if (out != NULL && g_plus_service_installed != 0
        && g_host_ready(g_host_ctx) != 0
        && g_battle_button_visible != 0
        && g_edit_controls_widget == widget
        && widget_child_link_valid(widget, g_ui_context) != 0) {
        uint8_t flag = 0xff;
        if (widget + 0x49 < 0x1001
            || g_host_read(g_host_ctx, widget + 0x48, &flag, 1) != 1
            || flag != 1)
            goto chat_rows;

        uint64_t handle = 0;
        const char *event = "android_toggle_rejected";
        if (protected_channel_acquire(&handle) != 0 && handle != 0
            && handle == g_edit_controls_state_a) {   /* 0x2844c0 */
            event = android_toggle_queue() != 0 ? "android_toggle_queued"
                                                : "android_toggle_rejected";
        }
        battle_menu_log_event(event, 0);
        action_count = g_script_action_count;
        goto dispatch_default;
    }

    /* --- chat rows (premium path) --- */
chat_rows:
    if (out == NULL || g_plus_service_installed == 0) {
        if (out != NULL)
            goto fps_popup;
        return 0;
    }
    if (g_host_ready(g_host_ctx) != 0 && g_rich_chat_ready == 1
        && ui_scene_fonts_ready() != 0) {
        if (g_rich_chat_rows[0] == widget
            && widget_child_link_valid(widget, g_ui_context) != 0) {
            uint8_t flag = 0xff;
            if (widget + 0x49 >= 0x1001
                && g_host_read(g_host_ctx, widget + 0x48, &flag, 1) == 1
                && flag == 1) {
                script_port_chat_row_action(1);
                goto dispatch_default;
            }
        }
        if (g_rich_chat_rows[0x1b] == widget
            && widget_child_link_valid(widget, g_ui_context) != 0) {
            uint8_t flag = 0xff;
            if (0x1000 < widget + 0x49
                && g_host_read(g_host_ctx, widget + 0x48, &flag, 1) == 1
                && flag == 1) {
                script_port_chat_row_action(2);
                goto dispatch_default;
            }
        }
    }

fps_popup:
    if (g_host_ready(g_host_ctx) == 0)
        return 0;

    /* --- fps popup rows --- */
    if (g_edit_controls_open_mirror != 0
        && g_edit_controls_extra_ctx != 0
        && ui_scene_fonts_ready() != 0
        && widget_child_link_valid(g_edit_controls_ctx, g_ui_context) != 0) {
        if (widget != g_edit_controls_row_a
            || widget_edit_controls_bound_check(widget) == 0) {
            if (widget == g_edit_controls_row_b
                && widget_edit_controls_bound_check(widget) != 0) {
                g_fps_popup_open = 0;                       /* 0x282af8 */
            } else if (widget == g_edit_controls_row_c
                       && widget_edit_controls_bound_check(widget) != 0) {
                if (g_fps_popup_limit != g_fps_popup_applied) {
                    bool at_floor = (uint32_t)(g_fps_popup_limit - 1) < 0x91;
                    int apply_arg = (int)g_fps_popup_limit;
                    int new_limit = (int)g_fps_popup_limit;
                    if (g_fps_popup_limit == 0x91) {
                        apply_arg = 0;
                        new_limit = 0x91;
                    }
                    g_fps_popup_field_b04 = 1;
                    if (!at_floor)
                        apply_arg = -1;
                    int applied = script_port_client_performance_apply(apply_arg);
                    if ((uint32_t)(new_limit - 0x92) > 0xffffff6eU
                        && g_fps_popup_open != 0) {
                        if (applied != 0) {
                            g_fps_popup_limit = (uint32_t)new_limit;
                            g_fps_popup_applied = (uint32_t)new_limit;
                        }
                        g_fps_popup_field_b04 = 0;
                    }
                    uint32_t opens = g_fps_popup_opens + 1;
                    bool under_cap = g_fps_popup_opens < 0x18;
                    g_fps_popup_opens = opens;
                    if (under_cap && g_host_log != NULL)
                        g_host_log(g_host_ctx, "script_port_fps",
                                   applied != 0 ? "saved" : "save_failed",
                                   (uint32_t)new_limit);
                    action_count = g_script_action_count;
                    if (applied != 0
                        && ui_edit_controls_ctx_valid() != 0) {
                        *(int32_t *)(uintptr_t)(g_edit_controls_panel
                                                + 0xe8) = new_limit;
                        goto dispatch_default;
                    }
                }
            } else {
                if (widget != g_edit_controls_row_d
                    || widget_edit_controls_bound_check(widget) == 0)
                    goto chooser_dispatch;
                if (g_fps_popup_applied != 0x91) {
                    /* RESET: run the save path at the 145 floor */
                    g_fps_popup_field_b04 = 1;
                    int applied = script_port_client_performance_apply(0);
                    if ((uint32_t)(0x91 - 0x92) > 0xffffff6eU
                        && g_fps_popup_open != 0) {
                        if (applied != 0) {
                            g_fps_popup_limit = 0x91;
                            g_fps_popup_applied = 0x91;
                        }
                        g_fps_popup_field_b04 = 0;
                    }
                    uint32_t opens = g_fps_popup_opens + 1;
                    bool under_cap = g_fps_popup_opens < 0x18;
                    g_fps_popup_opens = opens;
                    if (under_cap && g_host_log != NULL)
                        g_host_log(g_host_ctx, "script_port_fps",
                                   applied != 0 ? "saved" : "save_failed",
                                   0x91);
                    action_count = g_script_action_count;
                    if (applied != 0
                        && ui_edit_controls_ctx_valid() != 0) {
                        *(int32_t *)(uintptr_t)(g_edit_controls_panel
                                                + 0xe8) = 0x91;
                        goto dispatch_default;
                    }
                    goto dispatch_default;
                }
                if (ui_edit_controls_ctx_valid() == 0)
                    goto dispatch_default;
                g_fps_popup_limit = 0x91;
                *(int32_t *)(uintptr_t)(g_edit_controls_panel + 0xe8) = 0x91;
            }
        }
        goto dispatch_default;
    }

chooser_dispatch:
    /* --- font chooser (hint + 6 choice buttons) --- */
    if (g_chooser_state.built != 0 && g_chooser_state.flag_614 != 0
        && ui_scene_fonts_ready() != 0
        && widget_child_link_valid(g_chooser_state.root, g_ui_context) != 0) {
        if (widget != g_chooser_state.canvas
            || widget_popover_bound_check(widget) == 0) {
            if (widget == g_chooser_state.hint
                && widget_popover_bound_check(widget) != 0) {
                g_chooser_state.open_flag = 0;              /* 0x282118 */
            } else {
                uint32_t choice = 0;
                int hit = 0;
                for (int k = 0; k < 6; k++) {
                    if (widget == g_chooser_state.buttons[k]
                        && widget_popover_bound_check(widget) != 0) {
                        choice = (uint32_t)k;
                        hit = 1;
                        break;
                    }
                }
                if (!hit)
                    goto camera_dispatch;
                if (g_chooser_state.save_enabled != 0) {    /* 0x28212c */
                    int saved = script_port_font_choice_save(choice);
                    action_count = g_script_action_count + 1;
                    if (g_script_action_count < 0x20
                        && g_host_log != NULL) {
                        g_script_action_count =
                            g_script_action_count + 1;
                        g_host_log(g_host_ctx, "script_port_font_ui",
                                   saved != 0 ? "saved" : "save_failed",
                                   choice);
                    }
                    if (saved != 0)
                        g_chooser_state.open_flag = 0;
                }
            }
        }
        goto dispatch_default;
    }

camera_dispatch:
    /* --- camera popup rows --- */
    if (g_rich_panel_built == 0 || g_rich_panel_page_ctx == 0
        || ui_scene_fonts_ready() == 0
        || widget_child_link_valid(g_rich_panel_ctx, g_ui_context) == 0)
        goto entry_dispatch;

    if (widget == g_rich_panel_rows_a[0].widget
        && widget_rich_panel_bound_check(widget) != 0) {
        if (g_rich_panel_open == 0) {
            g_rich_panel_open = 1;                          /* 0x283988 */
            camera_log_event_w2("popup_open", 0);
        }
        goto dispatch_default;
    }
    {
        uint32_t row = 0;
        if (widget == g_rich_panel_rows_a[1].widget
            && widget_rich_panel_bound_check(widget) != 0)
            row = 1;
        else if (widget == g_rich_panel_rows_a[2].widget
                 && widget_rich_panel_bound_check(widget) != 0)
            row = 2;
        else if (widget == g_rich_panel_rows_a[3].widget
                 && widget_rich_panel_bound_check(widget) != 0)
            row = 3;
        else if (widget == g_rich_panel_rows_a[4].widget
                 && widget_rich_panel_bound_check(widget) != 0)
            row = 4;
        else if (widget == g_rich_panel_rows_a[5].widget
                 && widget_rich_panel_bound_check(widget) != 0)
            row = 5;
        else
            goto entry_dispatch;

        if (g_rich_panel_open == 0)
            goto dispatch_default;
        uint32_t page;
        int32_t step = 0;
        switch (row) {
        case 2:  /* close */
            g_rich_panel_open = 0;
            page = 5;
            break;
        case 3:  /* MODE 1 */
            page = 2;
            step = ((int)g_camera_page_counter + 1) % 4;   /* raw: signed mod */
            break;
        case 4:  /* ZOOM 0.60x */
            page = 3;
            break;
        case 5:  /* RESET */
            page = 4;
            break;
        default: /* row 1: no page call */
            goto dispatch_default;
        }
        script_port_camera_page_select(page, 0, step);
    }
    goto dispatch_default;

entry_dispatch:
    /* --- live menu-entry toggles (stage 2 + init latch) --- */
    if (g_rich_stage_state != 2
        || (ui_latch_test_and_set(1,
             (volatile int *)&g_host_init_latch) & 1) != 0)
        return 0;

    if (g_rich_hidden_latch == 0) {
        if (ui_scene_fonts_ready() != 0
            && widget_child_link_valid(g_view_object_a, g_ui_context) != 0
            && widget_flag48_equals(g_view_object_a, 1) != 0) {
            int32_t count = g_menu_entry_index;             /* 0x1a7d18 */
            menu_entry_live_t *ent =
                (menu_entry_live_t *)(void *)g_menu_entry_table;
            for (int32_t i = 0; i < count; i++) {
                const menu_config_live_t *cfg =
                    (const menu_config_live_t *)(const void *)RICH_ROW_MODES
                    + i;
                if (ent->state != 0
                    && *(const uint32_t *)(const void *)ent->tpl == 2
                    && ent->tpl[0x2c] != 0
                    && ent->widget == widget) {
                    if (menu_entry_live_check((uint64_t *)(void *)ent,
                                              (const int32_t *)(const void *)cfg) != 0) {
                        int value =
                            *(const int32_t *)(const void *)(ent->tpl + 8);
                        *out = menu_entry_toggle_apply(widget, value) != 0
                                   ? (uint32_t)value : 0;
                        g_host_init_latch = 0;
                        return 1;
                    }
                    count = g_menu_entry_index;   /* raw refreshes the count */
                }
                ent = (menu_entry_live_t *)((uint8_t *)ent + 0xd8);
            }
        }
    }
    g_host_init_latch = 0;
    return 0;

dispatch_default:
    g_script_action_count = action_count;                  /* 0x282b08 */
    *out = 0;
    return 1;
}
/* widgets chain 2 chunk 5: covers raw lines 7124-7663 (hud_camera_settings_build
 * complete — the nominal 6851-7300 slice ends mid-function, so this chunk
 * closes the straddler through raw 7663) */

/* ===== chain-2 additions: camera panel state ===== */

uint32_t g_camera_build_stage;   /* 0x2839a8 — camera panel build stage
                                  * (1..5, 8+slider, 0x10|row) */

extern uint64_t g_rich_panel_ctx_slot;    /* 0x283348 — popup_generic canvas
                                           * (misc.c: g_rich_panel_ctx_slot) */
extern uint64_t g_rich_panel_k2_slots[5]; /* 0x283350 — slider widgets
                                           * (misc.c: g_rich_panel_k2_slots) */
extern uint64_t g_rich_panel_k4_slots[5]; /* 0x283378 — slider bundles
                                           * (misc.c: g_rich_panel_k4_slots) */

/* rodata: slider config pair + label texts */
extern const uint64_t RODATA_10f8d0;  /* 0x10f8d0 — slider 0 config qword */
extern const uint64_t RODATA_10f8b0;  /* 0x10f8b0 — slider 1..4 config qword */
extern const char RODATA_136344[];    /* 0x136344 — camera close label text */

/* ===== chain-2 additions: camera build helpers ===== */

extern int camera_panel_ready_check(void);
                                      /* misc.c @ 0017bd68 — stage-1 gate */
extern int camera_option_label_build(uint64_t *slot, uint32_t kind,
                                     const char *text);
                                      /* renderer.c @ 0017bf48 — build one
                                        * kind-1 option row */
extern uint64_t widget_style_child_lookup(uint64_t widget, const char *name);
                                      /* misc.c @ 0017c284 — styled child */
extern void engine_post_alloc_init(void);
                                      /* misc.c @ 0016ce08 — post-alloc
                                        * bookkeeping for the 0x80 class */
extern uint64_t widget_field_ptr_read(uint64_t addr);
                                      /* misc.c @ 0016f79c — plausible field
                                        * pointer read */
extern int camera_slider_ownership_check(uint32_t index);
                                      /* misc.c @ 0017bb9c — slider owned */

/* ===== camera settings panel (native sliders) ===== */

/*
 * hud_camera_settings_build — build the camera settings panel: gate on
 * camera_panel_ready_check, allocate the 0x80 panel (vptr game+0x11abcf0,
 * detached), attach it to the scene root for the current UI context, build
 * the CAMERA/clear option rows (camera_option_label_build kinds 5/6/3/1),
 * load the popup_generic canvas, clear its five button styles and the
 * 0x134a97/"title" styled children, then build the five edit_controls_ui
 * sliders (0x178 widget, constructor game+0x889288 with slider_bg/button
 * children, vptr game+0x11c0c58, +0x128 link to the button, config pair
 * 0x10f8d0/0x10f8b0 at +0xe8, 200/10000 at +0xf0, bounds value recentered
 * through the "bounds" style child at +0x138) and the six kind-0 label
 * rows (sc bundle 5 + styled 0x134a97 text, attached under the panel).
 * Every failure logs its 'script_port_camera' reason (cap 0x30) and
 * returns 0; success sets the page-list flag (0x283980) and returns 1.
 * @ 0017a478
 */
int hud_camera_settings_build(void)
{
    g_camera_build_stage = 1;                          /* 0x2839a8 */
    if (camera_panel_ready_check() == 0)
        return 0;

    g_camera_build_stage = 2;
    uint64_t panel = engine_call_11a2840(0x80);
    if (panel == 0)
        return 0;
    engine_post_alloc_init();

    /* panel vptr must be game base + 0x11abcf0, detached */
    {
        uint64_t vptr = 0;
        int rd = (panel + 8) >> 3 >= 0x201
                 && g_host_read(g_host_ctx, panel, &vptr, 8) == 1;
        int plausible = 0xfff < vptr && rd && (vptr & 7) == 0;
        uint64_t parent = 1;
        float index_f = 0.0f;
        if ((plausible ? vptr : 0) != g_game_base + 0x11abcf0U
            || panel + 0x38 < 0x1000
            || (panel & 0xfffffffffffffff8ULL) == 0xffffffffffffffc0ULL
            || g_host_read(g_host_ctx, panel + 0x38, &parent, 8) != 1
            || parent != 0
            || panel + 0x40 < 0x1000
            || (panel & 0xfffffffffffffffcULL) == 0xffffffffffffffbcULL
            || g_host_read(g_host_ctx, panel + 0x40, &index_f, 4) != 1
            || index_f != -1.0f /* raw: -NAN, sign-extended -1 */)
            return 0;
    }

    g_rich_panel_root = g_ui_context;                  /* 0x283338 */
    g_rich_panel_ctx = panel;                          /* 0x283340 */
    engine_call_594e80(panel, 1);
    widget_set_position(panel, -10000.0f /* 0xc61c3c00 */,
                        -10000.0f /* 0xc61c3c00 */);
    engine_call_5988ec(g_ui_scene_root, panel);
    if (widget_child_link_valid(panel, g_ui_context) == 0)
        return 0;

    g_camera_build_stage = 3;
    if (camera_option_label_build(&g_rich_panel_rows_a[0].widget, 5,
                                  "CAMERA") == 0
        || camera_option_label_build(&g_rich_panel_rows_a[1].widget, 6,
                                     g_empty_text) == 0)
        return 0;

    g_camera_build_stage = 4;
    g_rich_panel_ctx_slot = engine_sc_bundle_load("popup_generic"); /* 0x283348 */
    {
        uint64_t canvas = g_rich_panel_ctx_slot;
        uint64_t vptr = 0;
        int rd = (canvas + 8) >> 3 >= 0x201
                 && g_host_read(g_host_ctx, canvas, &vptr, 8) == 1;
        int plausible = 0xfff < vptr && rd && (vptr & 7) == 0;
        uint64_t parent = 1;
        float index_f = 0.0f;
        if ((plausible ? vptr : 0) != g_game_base + 0x11ad208U
            || canvas + 0x38 < 0x1000
            || (canvas & 0xfffffffffffffff8ULL) == 0xffffffffffffffc0ULL
            || g_host_read(g_host_ctx, canvas + 0x38, &parent, 8) != 1
            || parent != 0
            || canvas + 0x40 < 0x1000
            || (canvas & 0xfffffffffffffffcULL) == 0xffffffffffffffbcULL
            || g_host_read(g_host_ctx, canvas + 0x40, &index_f, 4) != 1
            || index_f != -1.0f)
            return 0;

        engine_call_5d7c30(canvas, 0);

        /* clear the five button styles and the styled title children */
        static const char *const BUTTON_STYLES[5] = {
            "button_negative", "button_no", "button_yes",
            "button_ok", "button_close"
        };
        for (int k = 0; k < 5; k++) {
            uint64_t child = widget_style_child_lookup(canvas,
                                                       BUTTON_STYLES[k]);
            if (child != 0)
                *(uint8_t *)(uintptr_t)(child + 8) = 0;
        }
        uint64_t styled = engine_call_5d8ad4(canvas,
                                             (uint64_t)(uintptr_t)RICH_TOAST_STYLE);
        if (styled != 0) {
            int link_ok = 1;
            if (widget_reaches_w2(styled, canvas, &link_ok))
                *(uint8_t *)(uintptr_t)(styled + 8) = 0;
        }
        uint64_t title = engine_call_5d8ad4(canvas,
                                            (uint64_t)(uintptr_t)"title");
        if (title != 0) {
            int link_ok = 1;
            if (widget_reaches_w2(title, canvas, &link_ok))
                *(uint8_t *)(uintptr_t)(title + 8) = 0;
        }

        engine_call_594e80(canvas, 0);
        engine_call_594130(panel, canvas);
    }

    g_camera_build_stage = 5;
    if (camera_option_label_build(&g_rich_panel_rows_a[2].widget, 3,
                                  RODATA_136344) == 0
        || camera_option_label_build(&g_rich_panel_rows_a[3].widget, 1,
                                     "MODE 1") == 0
        || camera_option_label_build(&g_rich_panel_rows_a[4].widget, 1,
                                     "ZOOM 0.60x") == 0
        || camera_option_label_build(&g_rich_panel_rows_a[5].widget, 1,
                                     "RESET") == 0)
        return 0;

    /* five edit_controls_ui sliders */
    for (uint32_t i = 0; i < 5; i++) {
        g_camera_build_stage = 8 + i;                  /* raw: idx + 8 */
        uint64_t bundle = engine_sc_bundle_load("edit_controls_ui");
        {
            uint64_t vptr = 0;
            int rd = (bundle + 8) >> 3 >= 0x201
                     && g_host_read(g_host_ctx, bundle, &vptr, 8) == 1;
            int plausible = 0xfff < vptr && rd && (vptr & 7) == 0;
            uint64_t parent = 1;
            float index_f = 0.0f;
            if ((plausible ? vptr : 0) != g_game_base + 0x11ad208U
                || bundle + 0x38 < 0x1000
                || (bundle & 0xfffffffffffffff8ULL)
                       == 0xffffffffffffffc0ULL
                || g_host_read(g_host_ctx, bundle + 0x38, &parent, 8) != 1
                || parent != 0
                || bundle + 0x40 < 0x1000
                || (bundle & 0xfffffffffffffffcULL)
                       == 0xffffffffffffffbcULL
                || g_host_read(g_host_ctx, bundle + 0x40, &index_f, 4) != 1
                || index_f != -1.0f) {
                camera_log_event_w2("slider_asset_contract", i);
                return 0;
            }
        }

        g_rich_panel_k4_slots[i] = bundle;             /* 0x283378 */
        engine_call_5d7c30(bundle, 0);
        uint64_t scale_child = widget_style_child_lookup(bundle,
                                                         "slider_scale");
        if (scale_child == 0) {
            camera_log_event_w2("slider_scale_contract", i);
            return 0;
        }
        uint64_t slider_bg = widget_style_child_lookup(scale_child,
                                                       "slider_bg");
        uint64_t slider_button = widget_style_child_lookup(scale_child,
                                                           "slider_button");
        if (slider_bg == 0 || slider_button == 0) {
            camera_log_event_w2("slider_children_contract", i);
            return 0;
        }

        widget_set_position(slider_bg, 0.0f, 0.0f);
        widget_set_position(slider_button, 0.0f, 0.0f);
        float btn_anchor[4];
        widget_set_anchor(slider_button, btn_anchor);

        /* the "bounds" style child of the slider button (if linked) */
        bool have_bounds = false;
        float bounds_anchor[4];
        uint64_t bounds = ((uint64_t (*)(uint64_t, const char *))(
                               uintptr_t)(g_game_base + 0x5d76d4))(
            slider_button, "bounds");
        if (bounds != 0) {
            int link_ok = 1;
            if (widget_reaches_w2(bounds, slider_button, &link_ok)) {
                widget_set_anchor(bounds, bounds_anchor);
                have_bounds = true;
            }
        }

        uint64_t slider = engine_call_11a2840(0x178);
        if (slider == 0)
            return 0;
        /* slider constructor: (slider, bg, button, 0, 1) */
        ((void (*)(uint64_t, uint64_t, uint64_t, int, int))(
             uintptr_t)(g_game_base + 0x889288))(slider, slider_bg,
                                                 slider_button, 0, 1);

        {
            uint64_t vptr = 0;
            int rd = (slider + 8) >> 3 >= 0x201
                     && g_host_read(g_host_ctx, slider, &vptr, 8) == 1;
            int plausible = 0xfff < vptr && rd && (vptr & 7) == 0;
            if ((plausible ? vptr : 0) != g_game_base + 0x11c0c58U) {
                camera_log_event_w2("slider_constructor_contract", i);
                return 0;
            }
        }
        {
            int link_ok = 1;
            if (!widget_reaches_w2(slider, bundle, &link_ok)) {
                camera_log_event_w2("slider_constructor_contract", i);
                return 0;
            }
        }
        {
            uint64_t linked = 0;
            if (slider + 0x130 < 0x1008                 /* raw: >> 3 < 0x201 */
                || g_host_read(g_host_ctx, slider + 0x128, &linked, 8) != 1
                || linked != slider_button) {
                camera_log_event_w2("slider_constructor_contract", i);
                return 0;
            }
        }

        engine_call_594130(g_rich_panel_ctx, slider);
        uint32_t span = (i == 0) ? 200 : 10000;
        g_rich_panel_k2_slots[i] = slider;              /* 0x283350 */
        *(uint32_t *)(uintptr_t)(slider + 0xf0) = span;
        /* raw: NEON cmlt/bsl select between the 0x10f8d0/0x10f8b0 qwords */
        *(uint64_t *)(uintptr_t)(slider + 0xe8) =
            (i == 0) ? RODATA_10f8d0 : RODATA_10f8b0;

        if (have_bounds) {
            /* recenter the bounds value at +0x138 by half the width
             * difference between the button and its bounds child */
            float *val = (float *)(uintptr_t)widget_field_ptr_read(
                slider + 0x138);
            float cur = 0.0f;
            if ((uint64_t)(uintptr_t)(val + 1) >> 2 >= 0x401  /* implausible */
                || g_host_read(g_host_ctx, (uint64_t)(uintptr_t)val,
                               &cur, 4) != 1
                || !isfinite(cur)) {
                camera_log_event_w2("slider_bounds_contract", i);
                return 0;
            }
            float off = ((btn_anchor[2] - btn_anchor[0])
                         - (bounds_anchor[2] - bounds_anchor[0]))
                        * 0.5f;
            if (!isfinite(off) || fabsf(off) > 512.0f) {
                camera_log_event_w2("slider_bounds_contract", i);
                return 0;
            }
            *val = cur - off;
        }

        widget_set_position(slider, -10000.0f /* 0xc61c3c00 */,
                            -10000.0f /* 0xc61c3c00 */);
        if (camera_slider_ownership_check(i) == 0) {
            camera_log_event_w2("slider_ownership_contract", i);
            return 0;
        }
    }

    /* six kind-0 label rows (bundle 5 + styled 0x134a97 text) */
    for (uint32_t r = 0; r < 6; r++) {
        g_camera_build_stage = 0x10 | r;                /* raw: idx | 0x10 */
        panel_row_t *row = &g_rich_panel_rows_b[r];
        uint64_t bundle = engine_sc_bundle_load(nexus_rich_asset(5));
        row->widget = bundle;
        if (movie_frame_count_check(bundle, 5, 0) == 0)
            return 0;

        {
            uint64_t parent = 1;
            float index_f = 0.0f;
            if (bundle + 0x38 < 0x1000
                || (bundle & 0xfffffffffffffff8ULL)
                       == 0xffffffffffffffc0ULL
                || g_host_read(g_host_ctx, bundle + 0x38, &parent, 8) != 1
                || parent != 0
                || bundle + 0x40 < 0x1000
                || (bundle & 0xfffffffffffffffcULL)
                       == 0xffffffffffffffbcULL
                || g_host_read(g_host_ctx, bundle + 0x40, &index_f, 4) != 1
                || index_f != -1.0f)
                return 0;
        }

        engine_call_5d7c30(bundle, 0);
        uint64_t styled = engine_call_5d8ad4(
            bundle, (uint64_t)(uintptr_t)RICH_TOAST_STYLE);  /* 0x134a97 */
        row->extra = styled;
        {
            uint64_t vptr = 0;
            int rd = (styled + 8) >> 3 >= 0x201
                     && g_host_read(g_host_ctx, styled, &vptr, 8) == 1;
            int plausible = 0xfff < vptr && rd && (vptr & 7) == 0;
            if ((plausible ? vptr : 0) != g_game_base + 0x11abad8U)
                return 0;
        }
        if (widget_chain_links_unique_check(bundle, styled) == 0)
            return 0;
        engine_call_594e80(bundle, 0);
        widget_set_position(bundle, -10000.0f /* 0xc61c3c00 */,
                            -10000.0f /* 0xc61c3c00 */);
        engine_call_594130(g_rich_panel_ctx, bundle);
        if (widget_rich_panel_bound_check(bundle) == 0)
            return 0;
    }

    g_rich_panel_page_ctx = 1;                          /* 0x283980 */
    camera_log_event_w2("native_sliders_created", 5);
    return 1;
}
/* widgets chain 2 chunk 6: covers raw lines 7665-8297 (ui_popover_styles_ready,
 * chooser_status_label_build and ui_edit_controls_styles_ready complete; the
 * nominal 7665-8150 slice ends mid-function, so this chunk closes the
 * ui_edit_controls_styles_ready straddler through raw 8297) */

/* ===== chain-2 corrections (recovered from the rebuilt name map / pushed files) ===== */

/* The rebuilt map + renderer.c name 0017474c `rich_button_style_apply`
 * (chunk 4 defined it as menu_entry_live_refresh) and 00191cd0
 * `script_port_client_performance_apply` (chunk 4 externed it as
 * script_port_fps_limit_apply). The coordinator should rename at assembly;
 * the raw addresses in the doc comments are the join keys. */

/* ===== chain-2 additions: chooser block (fonts.c owns the layout) ===== */

/* chooser widget state block, 0x648 bytes at 0x281b08 — built here,
 * rendered by fonts.c (see its chooser_state_t for the full layout).
 * 0x28211c/0x28212c are the flag/save words inside fonts.c's snap_cache
 * area; this builder only writes them through the raw block base. */
extern uint64_t g_chooser_root;         /* 0x281b10 — chooser root widget */
extern uint64_t g_chooser_panel;        /* 0x281b18 — popup_generic panel */
extern uint64_t g_chooser_canvas;       /* 0x281b20 — chooser canvas */
extern uint64_t g_chooser_hint;         /* 0x281bb8 — hint/close label */
extern uint64_t g_chooser_buttons[6];   /* 0x281c50 — 6 choice buttons, 0x98 stride */
extern uint64_t g_chooser_status_a;     /* 0x281fe0 — title label */
extern uint64_t g_chooser_status_b;     /* 0x282078 — status label */
extern uint32_t g_chooser_built;        /* 0x282110 — built flag (fonts.c: built) */
extern uint32_t g_chooser_flag_114;     /* 0x28211c — built pair hi (chunk-4 name) */
extern uint32_t g_chooser_save_gate;    /* 0x28212c — font-chooser save gate */

/* chunk 4 declared a view struct over the same block; keep the extern
 * declarations it used (address comments are the join keys) */
extern uint32_t g_chooser_open_word;    /* 0x282118 — open flag (digest g_chooser_open) */

/* raw word 0x281b08 = g_ui_context (chooser owner word, fonts.c layout) */
static void g_chooser_state_owner_set(void)
{
    *(uint64_t *)(uintptr_t)0x281b08 = g_ui_context;
}

/* chooser status label rows (popover_text_left records) */
extern int chooser_status_label_build(uint64_t *record, uint16_t kind);
                                      /* this chain (chunk 6, below) @ 0017dc00 */

/* ===== chain-2 additions: chooser build helpers ===== */

extern int chooser_ready_check(void);
                                      /* misc.c @ 0017bd68 — stage-1 gate
                                        * (chunk 5 named camera_panel_ready_check) */
extern int chooser_option_label_build(uint64_t *slot, uint32_t kind,
                                      const char *text);
                                      /* renderer.c @ 0017d8c4 — build one
                                        * chooser option row (chunk 5:
                                        * camera_option_label_build) */
extern int edit_controls_option_label_build(uint64_t *slot, uint32_t kind,
                                            const char *text);
                                      /* renderer.c @ 0017f2f8 — build one
                                        * edit-controls option row */

/* ===== font chooser tree (popover styles) ===== */

/*
 * ui_popover_styles_ready — build the font-chooser widget tree under the
 * chooser root (0x281b10): allocate the 0x80 root (vptr game+0x11abcf0,
 * detached), park it offscreen and attach it to the scene root, build the
 * clear option row (kind 6), load the popup_generic panel, clear its five
 * button styles and its 0x134a97/title styled children, attach it under
 * the root, then build the hint row (kind 3, "DEFAULT"-family text at
 * 0x136344) and the six choice rows (DEFAULT, PUSIA BOLD, NICE BRAWL,
 * GHOUL STARS, IMPACT, GENSHIN IMPACT) and the two status labels through
 * chooser_status_label_build (kinds 0x18/0x10). Returns 1 on success and
 * logs 'script_port_font_ui' chooser_built (cap 0x20); 0 on any gate.
 * @ 0017d01c
 */
int ui_popover_styles_ready(void)
{
    if (chooser_ready_check() == 0)
        return 0;

    g_chooser_root = engine_call_11a2840(0x80);       /* 0x281b10 */
    if (g_chooser_root == 0)
        return 0;
    engine_post_alloc_init();

    /* root vptr must be game base + 0x11abcf0, detached */
    {
        uint64_t vptr = 0;
        int rd = (g_chooser_root + 8) >> 3 >= 0x201
                 && g_host_read(g_host_ctx, g_chooser_root, &vptr, 8) == 1;
        int plausible = 0xfff < vptr && rd && (vptr & 7) == 0;
        uint64_t parent = 1;
        int32_t index = 0;
        if ((plausible ? vptr : 0) != g_game_base + 0x11abcf0U
            || g_chooser_root + 0x38 < 0x1000
            || (g_chooser_root & 0xfffffffffffffff8ULL)
                   == 0xffffffffffffffc0ULL
            || g_host_read(g_host_ctx, g_chooser_root + 0x38, &parent, 8) != 1
            || parent != 0
            || g_chooser_root + 0x40 < 0x1000
            || (g_chooser_root & 0xfffffffffffffffcULL)
                   == 0xffffffffffffffbcULL
            || g_host_read(g_host_ctx, g_chooser_root + 0x40, &index, 4) != 1
            || index != -1)
            return 0;
    }

    g_chooser_state_owner_set();
    engine_call_594e80(g_chooser_root, 1);
    widget_set_position(g_chooser_root, -10000.0f /* 0xc61c3c00 */,
                        -10000.0f /* 0xc61c3c00 */);
    engine_call_5988ec(g_ui_scene_root, g_chooser_root);
    if (widget_child_link_valid(g_chooser_root, g_ui_context) == 0)
        return 0;

    if (chooser_option_label_build(&g_chooser_canvas, 6, g_empty_text) == 0)
        return 0;

    /* popup_generic panel: clear styles + styled children, attach */
    g_chooser_panel = engine_sc_bundle_load("popup_generic");  /* 0x281b18 */
    {
        uint64_t vptr = 0;
        int rd = (g_chooser_panel + 8) >> 3 >= 0x201
                 && g_host_read(g_host_ctx, g_chooser_panel, &vptr, 8) == 1;
        int plausible = 0xfff < vptr && rd && (vptr & 7) == 0;
        uint64_t parent = 1;
        int32_t index = 0;
        if ((plausible ? vptr : 0) != g_game_base + 0x11ad208U
            || g_chooser_panel + 0x38 < 0x1000
            || (g_chooser_panel & 0xfffffffffffffff8ULL)
                   == 0xffffffffffffffc0ULL
            || g_host_read(g_host_ctx, g_chooser_panel + 0x38, &parent, 8) != 1
            || parent != 0
            || g_chooser_panel + 0x40 < 0x1000
            || (g_chooser_panel & 0xfffffffffffffffcULL)
                   == 0xffffffffffffffbcULL
            || g_host_read(g_host_ctx, g_chooser_panel + 0x40, &index, 4) != 1
            || index != -1)
            return 0;

        engine_call_5d7c30(g_chooser_panel, 0);

        static const char *const BUTTON_STYLES[5] = {
            "button_negative", "button_no", "button_yes",
            "button_ok", "button_close"
        };
        for (int k = 0; k < 5; k++) {
            uint64_t child = widget_style_child_lookup(g_chooser_panel,
                                                       BUTTON_STYLES[k]);
            if (child != 0)
                *(uint8_t *)(uintptr_t)(child + 8) = 0;
        }
        uint64_t styled = engine_call_5d8ad4(
            g_chooser_panel, (uint64_t)(uintptr_t)RICH_TOAST_STYLE);
        if (styled != 0) {
            int link_ok = 1;
            if (widget_reaches_w2(styled, g_chooser_panel, &link_ok))
                *(uint8_t *)(uintptr_t)(styled + 8) = 0;
        }
        uint64_t title = engine_call_5d8ad4(g_chooser_panel,
                                            (uint64_t)(uintptr_t)"title");
        if (title != 0) {
            int link_ok = 1;
            if (widget_reaches_w2(title, g_chooser_panel, &link_ok))
                *(uint8_t *)(uintptr_t)(title + 8) = 0;
        }

        engine_call_594e80(g_chooser_panel, 0);
        engine_call_594130(g_chooser_root, g_chooser_panel);
    }

    /* hint row + the six choice rows */
    if (chooser_option_label_build(&g_chooser_hint, 3, RODATA_136344) == 0
        || chooser_option_label_build(&g_chooser_buttons[0], 1, "DEFAULT") == 0
        || chooser_option_label_build(&g_chooser_buttons[1], 1,
                                      "PUSIA BOLD") == 0
        || chooser_option_label_build(&g_chooser_buttons[2], 1,
                                      "NICE BRAWL") == 0
        || chooser_option_label_build(&g_chooser_buttons[3], 1,
                                      "GHOUL STARS") == 0
        || chooser_option_label_build(&g_chooser_buttons[4], 1,
                                      "IMPACT") == 0
        || chooser_option_label_build(&g_chooser_buttons[5], 1,
                                      "GENSHIN IMPACT") == 0
        || chooser_status_label_build(&g_chooser_status_a, 0x18) == 0
        || chooser_status_label_build(&g_chooser_status_b, 0x10) == 0)
        return 0;

    g_chooser_built = 1;                              /* 0x282110 */
    {
        uint32_t under_cap = g_script_action_count < 0x20;
        g_script_action_count = g_script_action_count + 1;
        if (under_cap && g_host_log != NULL)
            g_host_log(g_host_ctx, "script_port_font_ui", "chooser_built", 6);
    }
    return 1;
}

/* the raw word 0x281b08 setter is defined above ui_popover_styles_ready */

/* ===== chooser status labels (popover_text_left rows) ===== */

/*
 * chooser_status_label_build — build one popover_text_left status row for
 * the chooser: record holds {head, view at +0x08, styled text at +0x10}.
 * The view must carry vptr game+0x11ad208 and sit detached; its styled
 * text (vptr game+0x11abad8) must be linked to it, have a child count at
 * +0xb0 in 1..0x100 and a class byte at +0x90 < 2, which are then forced
 * to (kind, 1). Both widgets are scaled to 1, the view parked offscreen
 * and attached under the chooser root; success requires the view to pass
 * widget_popover_bound_check. Returns 1 on success, 0 on any gate.
 * @ 0017dc00
 */
int chooser_status_label_build(uint64_t *record, uint16_t kind)
{
    uint64_t view = engine_sc_bundle_load("popover_text_left");
    record[1] = view;                                 /* record +0x08 */
    {
        uint64_t vptr = 0;
        int rd = (view + 8) >> 3 >= 0x201
                 && g_host_read(g_host_ctx, view, &vptr, 8) == 1;
        int plausible = 0xfff < vptr && rd && (vptr & 7) == 0;
        uint64_t parent = 1;
        int32_t index = 0;
        if ((plausible ? vptr : 0) != g_game_base + 0x11ad208U
            || view + 0x38 < 0x1000
            || (view & 0xfffffffffffffff8ULL) == 0xffffffffffffffc0ULL
            || g_host_read(g_host_ctx, view + 0x38, &parent, 8) != 1
            || parent != 0
            || view + 0x40 < 0x1000
            || (view & 0xfffffffffffffffcULL) == 0xffffffffffffffbcULL
            || g_host_read(g_host_ctx, view + 0x40, &index, 4) != 1
            || index != -1)
            return 0;
    }

    engine_call_5d7c30(view, 0);
    uint64_t styled = engine_call_5d8ad4(view,
                                         (uint64_t)(uintptr_t)RODATA_13784a);
    record[2] = styled;                               /* record +0x10 */
    {
        uint64_t vptr = 0;
        int rd = (styled + 8) >> 3 >= 0x201
                 && g_host_read(g_host_ctx, styled, &vptr, 8) == 1;
        int plausible = 0xfff < vptr && rd && (vptr & 7) == 0;
        if ((plausible ? vptr : 0) != g_game_base + 0x11abad8U)
            return 0;
    }
    if (widget_chain_links_unique_check(view, styled) == 0)
        return 0;

    /* styled text child count (+0xb0) 1..0x100 and class byte (+0x90) < 2 */
    {
        uint16_t count = 0;
        uint8_t class_byte = 0;
        if (styled + 0xb0 < 0x1000
            || (styled & 0xfffffffffffffffeULL) == 0xffffffffffffff4eULL
            || g_host_read(g_host_ctx, styled + 0xb0, &count, 2) != 1
            || count < 1 || 0x100 < count)
            return 0;
        if (styled + 0x91 >= 0x1001
            || g_host_read(g_host_ctx, styled + 0x90, &class_byte, 1) != 1
            || 2 <= class_byte)
            return 0;

        /* force the requested kind and class 1 */
        *(uint16_t *)(uintptr_t)(styled + 0xb0) = kind;
        *(uint8_t *)(uintptr_t)(styled + 0x90) = 1;
    }

    widget_set_scale(view, 1.0f /* 0x3f800000 */,
                     1.0f /* 0x3f800000 */);
    widget_set_scale(styled, 1.0f /* 0x3f800000 */,
                     1.0f /* 0x3f800000 */);
    engine_call_594e80(view, 0);
    widget_set_position(view, -10000.0f /* 0xc61c3c00 */,
                        -10000.0f /* 0xc61c3c00 */);
    engine_call_594130(g_chooser_root, view);
    return widget_popover_bound_check(view);
}

/* ===== edit-controls styles (fps popup ctx) ===== */

extern const char RODATA_132558[];  /* 0x132558 — SAVE row label text */
extern uint64_t g_edit_controls_flag_slot;  /* 0x282170 — kind-4 slot (chunk 7) */
extern int edit_controls_slider_build(void);
                                      /* this chain (chunk 7, below) @ 0017f634 */

/*
 * ui_edit_controls_styles_ready — build the edit-controls widget block
 * (0x282150 group): allocate the 0x80 style ctx (vptr game+0x11abcf0,
 * detached), park it offscreen, attach it to the scene root and build the
 * clear option row (kind 6); load the popup_generic canvas, clear its five
 * button styles and its 0x134a97/"title" styled children and attach it
 * under the ctx; build the option rows (kind 3 close text at 0x136344,
 * kind 1 SAVE text at 0x132558, kind 1 "RESET"), the slider
 * (edit_controls_slider_build) and the three status records (0x158 stride
 * from 0x2826d8: the first two from the asset-5 bundle with styled
 * 0x134a97 text, the third from popover_text_left with styled 0x13784a
 * text and 16 forced children/class 1). Returns 1 on success (styles flag
 * 0x282ae0 = 1), 0 on any gate. @ 0017e0b0 (raw 7991-8297, finished here)
 */
int ui_edit_controls_styles_ready(void)
{
    if (chooser_ready_check() == 0)
        return 0;

    g_edit_controls_ctx = engine_call_11a2840(0x80);  /* 0x282158 */
    if (g_edit_controls_ctx == 0)
        return 0;
    engine_post_alloc_init();

    /* ctx vptr must be game base + 0x11abcf0, detached */
    {
        uint64_t vptr = 0;
        int rd = (g_edit_controls_ctx + 8) >> 3 >= 0x201
                 && g_host_read(g_host_ctx, g_edit_controls_ctx, &vptr, 8) == 1;
        int plausible = 0xfff < vptr && rd && (vptr & 7) == 0;
        uint64_t parent = 1;
        int32_t index = 0;
        if ((plausible ? vptr : 0) != g_game_base + 0x11abcf0U
            || g_edit_controls_ctx + 0x38 < 0x1000
            || (g_edit_controls_ctx & 0xfffffffffffffff8ULL)
                   == 0xffffffffffffffc0ULL
            || g_host_read(g_host_ctx, g_edit_controls_ctx + 0x38,
                           &parent, 8) != 1
            || parent != 0
            || g_edit_controls_ctx + 0x40 < 0x1000
            || (g_edit_controls_ctx & 0xfffffffffffffffcULL)
                   == 0xffffffffffffffbcULL
            || g_host_read(g_host_ctx, g_edit_controls_ctx + 0x40,
                           &index, 4) != 1
            || index != -1)
            return 0;
    }

    g_edit_controls_block_root = g_ui_context;        /* 0x282150 */
    engine_call_594e80(g_edit_controls_ctx, 1);
    widget_set_position(g_edit_controls_ctx, -10000.0f /* 0xc61c3c00 */,
                        -10000.0f /* 0xc61c3c00 */);
    engine_call_5988ec(g_ui_scene_root, g_edit_controls_ctx);
    if (widget_child_link_valid(g_edit_controls_ctx, g_ui_context) == 0)
        return 0;

    if (edit_controls_option_label_build(&g_edit_controls_row_a, 6,
                                         g_empty_text) == 0)
        return 0;

    /* popup_generic canvas: clear styles + styled children, attach */
    g_edit_controls_slot = engine_sc_bundle_load("popup_generic"); /* 0x282160 */
    {
        uint64_t vptr = 0;
        int rd = (g_edit_controls_slot + 8) >> 3 >= 0x201
                 && g_host_read(g_host_ctx, g_edit_controls_slot, &vptr, 8) == 1;
        int plausible = 0xfff < vptr && rd && (vptr & 7) == 0;
        uint64_t parent = 1;
        int32_t index = 0;
        if ((plausible ? vptr : 0) != g_game_base + 0x11ad208U
            || g_edit_controls_slot + 0x38 < 0x1000
            || (g_edit_controls_slot & 0xfffffffffffffff8ULL)
                   == 0xffffffffffffffc0ULL
            || g_host_read(g_host_ctx, g_edit_controls_slot + 0x38,
                           &parent, 8) != 1
            || parent != 0
            || g_edit_controls_slot + 0x40 < 0x1000
            || (g_edit_controls_slot & 0xfffffffffffffffcULL)
                   == 0xffffffffffffffbcULL
            || g_host_read(g_host_ctx, g_edit_controls_slot + 0x40,
                           &index, 4) != 1
            || index != -1)
            return 0;

        engine_call_5d7c30(g_edit_controls_slot, 0);

        static const char *const BUTTON_STYLES[5] = {
            "button_negative", "button_no", "button_yes",
            "button_ok", "button_close"
        };
        for (int k = 0; k < 5; k++) {
            uint64_t child = widget_style_child_lookup(g_edit_controls_slot,
                                                       BUTTON_STYLES[k]);
            if (child != 0)
                *(uint8_t *)(uintptr_t)(child + 8) = 0;
        }
        uint64_t styled = engine_call_5d8ad4(
            g_edit_controls_slot, (uint64_t)(uintptr_t)RICH_TOAST_STYLE);
        if (styled != 0) {
            int link_ok = 1;
            if (widget_reaches_w2(styled, g_edit_controls_slot, &link_ok))
                *(uint8_t *)(uintptr_t)(styled + 8) = 0;
        }
        uint64_t title = engine_call_5d8ad4(g_edit_controls_slot,
                                            (uint64_t)(uintptr_t)"title");
        if (title != 0) {
            int link_ok = 1;
            if (widget_reaches_w2(title, g_edit_controls_slot, &link_ok))
                *(uint8_t *)(uintptr_t)(title + 8) = 0;
        }

        engine_call_594e80(g_edit_controls_slot, 0);
        engine_call_594130(g_edit_controls_ctx, g_edit_controls_slot);
    }

    /* option rows + slider */
    if (edit_controls_option_label_build(&g_edit_controls_row_b, 3,
                                         RODATA_136344) == 0
        || edit_controls_option_label_build(&g_edit_controls_row_c, 1,
                                            RODATA_132558) == 0
        || edit_controls_option_label_build(&g_edit_controls_row_d, 1,
                                            "RESET") == 0
        || edit_controls_slider_build() == 0)
        return 0;

    /* the three status records (0x158 stride from 0x2826d8): records 1-2
     * use the asset-5 bundle + styled 0x134a97, record 3 the
     * popover_text_left bundle + styled 0x13784a with 16 children */
    for (int r = 0; r < 3; r++) {
        status_label_rec_t *rec =
            (status_label_rec_t *)(void *)((uintptr_t)0x2826d8
                                           + (size_t)r * 0x158);
        uint64_t widget;
        const char *style_name;
        if (r == 2) {
            widget = engine_sc_bundle_load("popover_text_left");
            style_name = RODATA_13784a;
        } else {
            widget = engine_sc_bundle_load(nexus_rich_asset(5));
            style_name = RICH_TOAST_STYLE;
        }
        rec->ctx_slot = widget;                      /* record +0x08 */
        if (movie_frame_count_check(widget, 5, 0) == 0)
            return 0;

        {
            uint64_t parent = 1;
            float index_f = 0.0f;
            if (widget + 0x38 < 0x1000
                || (widget & 0xfffffffffffffff8ULL)
                       == 0xffffffffffffffc0ULL
                || g_host_read(g_host_ctx, widget + 0x38, &parent, 8) != 1
                || parent != 0
                || widget + 0x40 < 0x1000
                || (widget & 0xfffffffffffffffcULL)
                       == 0xffffffffffffffbcULL
                || g_host_read(g_host_ctx, widget + 0x40, &index_f, 4) != 1
                || index_f != -1.0f /* raw: -NAN, sign-extended -1 */)
                return 0;
        }

        engine_call_5d7c30(widget, 0);
        uint64_t styled = engine_call_5d8ad4(widget,
                                             (uint64_t)(uintptr_t)style_name);
        rec->widget_slot = styled;                   /* record +0x10 */
        {
            uint64_t vptr = 0;
            int rd = (styled + 8) >> 3 >= 0x201
                     && g_host_read(g_host_ctx, styled, &vptr, 8) == 1;
            int plausible = 0xfff < vptr && rd && (vptr & 7) == 0;
            if ((plausible ? vptr : 0) != g_game_base + 0x11abad8U)
                return 0;
        }
        if (widget_chain_links_unique_check(widget, styled) == 0)
            return 0;

        if (r == 2) {
            /* force 16 children and class 1 on the styled text */
            uint16_t count = 0;
            uint8_t class_byte = 0;
            if (styled + 0xb0 < 0x1000
                || (styled & 0xfffffffffffffffeULL)
                       == 0xffffffffffffff4eULL
                || g_host_read(g_host_ctx, styled + 0xb0, &count, 2) != 1
                || count < 1 || 0x100 < count
                || styled + 0x91 < 0x1001
                || g_host_read(g_host_ctx, styled + 0x90, &class_byte, 1) != 1
                || 2 <= class_byte)
                return 0;
            *(uint16_t *)(uintptr_t)(styled + 0xb0) = 0x10;
            *(uint8_t *)(uintptr_t)(styled + 0x90) = 1;
            widget_set_scale(widget, 1.0f /* 0x3f800000 */,
                             1.0f /* 0x3f800000 */);
            widget_set_scale(styled, 1.0f /* 0x3f800000 */,
                             1.0f /* 0x3f800000 */);
        }

        engine_call_594e80(widget, 0);
        widget_set_position(widget, -10000.0f /* 0xc61c3c00 */,
                            -10000.0f /* 0xc61c3c00 */);
        engine_call_594130(g_edit_controls_ctx, widget);
        if (widget_edit_controls_bound_check(widget) == 0)
            return 0;
    }

    g_edit_controls_extra_ctx = 1;                    /* 0x282ae0 */
    return 1;
}
/* widgets chain 2 chunk 7: covers raw lines 8299-8738 (edit_controls_slider_
 * build complete + menu_row_state_init complete; the nominal 8151-8600 slice
 * starts after the chunk-6 straddler and ends mid-function, so this chunk
 * closes the menu_row_state_init straddler through raw 8738) */

/* ===== chain-2 additions: menu row state ===== */

/* menu row state (themes.c owns the layout: 0x18 bytes, see its
 * menu_row_state_t) — built from a 0x48-stride feature entry */
typedef struct {
    uint32_t font;        /* +0x00 — 0x18 */
    uint32_t enabled_bit; /* +0x04 — feature enable bit */
    uint32_t word_08;     /* +0x08 — always 0 */
    uint32_t feature_id;  /* +0x0c — feature mask / id */
    uint32_t state_10;    /* +0x10 — 1 plain / 3 locked */
    uint8_t  parent_flag; /* +0x14 — 1 when remapped to a parent */
    uint8_t  byte_15;     /* +0x15 */
    uint8_t  _pad16[2];   /* +0x16 */
} menu_row_state_t;       /* 0x18 bytes */

/* feature entry table (0x48 stride: name +0, bit index +0x10, kind +0x44,
 * flag +0x47, display kind +0x11) */
extern const uint8_t *g_feature_entry_table; /* 0x1a36f0 — GOT (themes.c) */

/* capability bits service entry (external to the whole dump) */
extern int capability_bits_read(void);
                                      /* external @ 0018d15c — not in dump */

/* feature bitmask pages (8 x 0x40-byte qwords, 0x40 bytes apart) */
extern uint64_t g_feature_mask_pages[8][8];  /* 0x284890..0x284a58 — page
                                              * bases at +0x00, fns at
                                              * +0x20 (0x2848b0..0x284a78) */

/* feature handler slots: {context, callback} pairs, 0x10 bytes at 0x2848b0 */
typedef struct {
    uint64_t ctx;   /* +0x00 — handler context */
    void    *fn;    /* +0x08 — handler callback */
} feature_handler_slot_t;
extern feature_handler_slot_t g_feature_handlers[8];  /* 0x2848b0 — 8 slots,
                                                       * 0x40-byte stride pages */

/* remap table: display id -> feature entry index (7 -> 0x16, 8 -> 0x17,
 * 9/10/0xd -> self, 0xb -> 0x17, 0xc -> 0x1e, 0xe -> 0x19, 0x43 -> 0x44,
 * 0x83 -> 0x84) — see the switch in menu_row_state_init */

/*
 * menu_row_state_init — initialize one 0x18-byte menu row state from a
 * 0x48-stride feature entry: state {0x18, 0, 1, 0}, then query the
 * capability service (external): on success latch enabled_bit (+0x04) and
 * its copy (+0x0c) from the per-id bitmask word at 0x2846b0 (shifted by
 * the entry's bit index when non-negative), mark +0x15 = 1; on failure
 * leave the mask at 0. Locked entries (flag +0x47) force state_10 = 3.
 * Display-kind remap (+0x11 == 1 and remapped id != id) restarts on the
 * parent entry with parent_flag = 1. Kind 2..4 rows dispatch: display ids
 * 0x2f/0x30/0x31/0x32/0x45..0x48/0x6c/0x82 zero state_10, mirror the
 * enabled bit and set parent_flag; ids 0x40/0x4a/0x5c/0x70/0x91 derive
 * state_10 and parent_flag from the 8 latch words (0x2848c0..0x284a80)
 * and push the mask word at +0x08 for 0x70/0x4a; all other ids call the
 * matching feature handler (pages 0x284890..0x284a58, fns at
 * 0x2848b0..0x284a78) which may rewrite {+0x00..0x14} when it returns 1
 * with size 0x18. @ 00184e28
 */
void menu_row_state_init(menu_row_state_t *state, const uint8_t *entry)
{
    uint32_t *words = (uint32_t *)(void *)state;

    words[3] = 0;                     /* +0x0c */
    words[1] = 0;                     /* +0x04 */
    words[0] = 0x18;                  /* +0x00 */
    words[2] = 1;                     /* +0x08 (always 0 per themes.c: the
                                       * raw stores 1 here, themes reads 0
                                       * through the copy at +0x20+8) */
    words[5] = 0;                     /* +0x14 */
    state->parent_flag = 0;
    state->byte_15 = 0;

    /* capability query: external to the dump */
    int cap = capability_bits_read();   /* external @ 0018d15c — not in dump */
    uint32_t mask_word;
    if (cap < 0) {
        mask_word = 0;
    } else {
        uint32_t bit_index = *(const uint32_t *)(const void *)(entry + 0x10);
        state->byte_15 = 1;                    /* +0x15 */
        mask_word =
            *(const uint32_t *)(const void *)(uintptr_t)(0x2846b0
                                                         + (uint32_t)cap * 4);
        words[1] = mask_word;                  /* +0x04 */
        words[3] = mask_word;                  /* +0x0c */
        if ((int32_t)bit_index >= 0)
            words[1] = (mask_word >> (bit_index & 0x1f)) & 1;
    }

    char flag47 = *(const char *)(const void *)(entry + 0x47);
    /* the raw re-reads the 8 latch words (0x2848c0..0x284a80) around the
     * branch — the latches are not modified here, only inspected later */
    if (flag47 != 0) {
        words[2] = 3;                          /* +0x08 */
        return;
    }

    uint32_t display_id = *(const uint32_t *)(const void *)entry;
    uint32_t remap = 0x11;
    switch (display_id) {
    case 7:
        break;                       /* stays 0x11 */
    case 8:
        remap = 0x16;
        break;
    case 9:
    case 10:
    case 0xd:
identity_remap:
        remap = display_id;
        break;
    case 0xb:
        remap = 0x17;
        break;
    case 0xc:
        remap = 0x1e;
        break;
    case 0xe:
        remap = 0x19;
        break;
    default:
        if (display_id == 0x43)
            remap = 0x44;
        else if (display_id != 0x83)
            goto identity_remap;
        else
            remap = 0x84;
        break;
    }

    uint8_t kind = *(const uint8_t *)(const void *)(entry + 0x11);
    if (kind == 1) {
        if (remap != display_id) {
            /* remapped display: restart on the parent entry */
            menu_row_state_init(state,
                                g_feature_entry_table
                                    + (size_t)remap * 0x48);
            state->parent_flag = 1;
        }
        return;
    }
    if (kind >= 5)
        return;

    switch (display_id) {
    case 0x2f:
    case 0x30:
    case 0x31:
    case 0x32:
    case 0x45:
    case 0x46:
    case 0x47:
    case 0x48:
    case 0x6c:
    case 0x82:
        words[2] = 0;                          /* +0x08 */
        words[1] = mask_word;                  /* +0x04 mirror */
        state->parent_flag = 1;
        return;
    default:
        break;
    }

    if (display_id == 0x40 || display_id == 0x4a || display_id == 0x5c
        || display_id == 0x70 || display_id == 0x91) {
        /* state from the 8 latch words (0x2848c0..0x284a80): any latch
         * set -> locked (state 0, parent_flag 1); all clear -> state 1 */
        uint64_t l0 = *(const uint64_t *)(const void *)(uintptr_t)0x2848c0;
        uint64_t l1 = *(const uint64_t *)(const void *)(uintptr_t)0x284900;
        uint64_t l2 = *(const uint64_t *)(const void *)(uintptr_t)0x284940;
        uint64_t l3 = *(const uint64_t *)(const void *)(uintptr_t)0x284980;
        uint64_t l4 = *(const uint64_t *)(const void *)(uintptr_t)0x2849c0;
        uint64_t l5 = *(const uint64_t *)(const void *)(uintptr_t)0x284a00;
        uint64_t l6 = *(const uint64_t *)(const void *)(uintptr_t)0x284a40;
        uint64_t l7 = *(const uint64_t *)(const void *)(uintptr_t)0x284a80;
        int any = (l0 != 0 || l1 != 0 || l2 != 0 || l3 != 0)
                  || (l4 != 0 || l5 != 0 || l6 != 0) || l7 != 0;
        state->parent_flag = (uint8_t)any;
        words[2] = any ? 0 : 1;                /* +0x08 */
        if (any && (display_id == 0x70 || display_id == 0x4a))
            words[1] = mask_word;              /* +0x04 mirror */
        return;
    }

    /* feature handler dispatch: find the page whose bit covers display_id */
    {
        uint32_t page = display_id >> 6;
        uint64_t bit = 1ULL << (display_id & 0x3f);
        int slot = -1;
        if ((g_feature_mask_pages[0][page] & bit) != 0)
            slot = 0;
        else if ((g_feature_mask_pages[1][page] & bit) != 0)
            slot = 1;
        else if ((g_feature_mask_pages[2][page] & bit) != 0)
            slot = 2;
        else if ((g_feature_mask_pages[3][page] & bit) != 0)
            slot = 3;
        else if ((g_feature_mask_pages[4][page] & bit) != 0)
            slot = 4;
        else if ((g_feature_mask_pages[5][page] & bit) != 0)
            slot = 5;
        else if ((g_feature_mask_pages[6][page] & bit) != 0)
            slot = 6;
        else if ((g_feature_mask_pages[7][page] & bit) != 0)
            slot = 7;
        if (slot < 0)
            return;

        /* handler record {ctx at page+0, fn at page+0x20} (0x284890 base) */
        uint64_t ctx =
            *(const uint64_t *)(const void *)(uintptr_t)(0x284890
                                                          + slot * 0x40);
        int (*fn)(uint64_t, uint64_t, menu_row_state_t *) =
            (int (*)(uint64_t, uint64_t, menu_row_state_t *))(
                *(const void **)(const void *)(uintptr_t)(0x2848b0
                                                           + slot * 0x40));
        if (fn == NULL)
            return;
        menu_row_state_t out;
        memset(&out, 0, sizeof out);
        out.font = 0x18;
        if (fn(ctx, display_id, &out) == 1 && out.font == 0x18)
            *state = out;   /* raw copies the 3 qwords back */
    }
}

/* ===== edit-controls slider (fps limit knob) ===== */

extern const uint64_t RODATA_10f810;  /* 0x10f810 — slider config qword */
uint64_t g_edit_controls_flag_slot;   /* 0x282170 — edit-controls kind-4
                                       * flag slot (bundle word) */

/*
 * edit_controls_slider_build — build the fps-limit slider for the
 * edit-controls block: load the edit_controls_ui bundle (vptr
 * game+0x11ad208, detached), cache it in the kind-4 flag slot (0x282170),
 * resolve its slider_scale/slider_bg/slider_button children, park the bg
 * and button at (0,0) and read their anchors, look up the button's
 * "bounds" style child (engine game+0x5d76d4, lineage-validated), then
 * allocate the 0x178 slider widget through the constructor at
 * game+0x889288 (slider, bg, button, 0, 1), require vptr game+0x11c0c58,
 * lineage to the scale child and the +0x128 link to the button, attach it
 * under the style ctx, seed +0xe8 with the current fps limit and +0xec
 * with the 0x10f810 config qword, recenter the bounds value at +0x138 by
 * half the button/bounds width difference (|off| <= 512), park it
 * offscreen and finish with ui_edit_controls_ctx_valid. Returns 1 on
 * success, 0 on any gate. @ 0017f634 (raw 8299-8519)
 */
int edit_controls_slider_build(void)
{
    uint64_t bundle = engine_sc_bundle_load("edit_controls_ui");
    {
        uint64_t vptr = 0;
        int rd = (bundle + 8) >> 3 >= 0x201
                 && g_host_read(g_host_ctx, bundle, &vptr, 8) == 1;
        int plausible = 0xfff < vptr && rd && (vptr & 7) == 0;
        uint64_t parent = 1;
        float index_f = 0.0f;
        if ((plausible ? vptr : 0) != g_game_base + 0x11ad208U
            || bundle + 0x38 < 0x1000
            || (bundle & 0xfffffffffffffff8ULL) == 0xffffffffffffffc0ULL
            || g_host_read(g_host_ctx, bundle + 0x38, &parent, 8) != 1
            || parent != 0
            || bundle + 0x40 < 0x1000
            || (bundle & 0xfffffffffffffffcULL) == 0xffffffffffffffbcULL
            || g_host_read(g_host_ctx, bundle + 0x40, &index_f, 4) != 1
            || index_f != -1.0f /* raw: -NAN, sign-extended -1 */)
            return 0;
    }

    g_edit_controls_flag_slot = bundle;               /* 0x282170 */
    engine_call_5d7c30(bundle, 0);
    uint64_t scale_child = widget_style_child_lookup(bundle, "slider_scale");
    if (scale_child == 0)
        return 0;
    uint64_t slider_bg = widget_style_child_lookup(scale_child, "slider_bg");
    uint64_t slider_button = widget_style_child_lookup(scale_child,
                                                       "slider_button");
    if (slider_bg == 0 || slider_button == 0)
        return 0;

    widget_set_position(slider_bg, 0.0f, 0.0f);
    widget_set_position(slider_button, 0.0f, 0.0f);
    float btn_anchor[4];
    widget_set_anchor(slider_button, btn_anchor);

    /* the "bounds" style child of the slider button (if linked) */
    bool have_bounds = false;
    float bounds_anchor[4];
    uint64_t bounds = ((uint64_t (*)(uint64_t, const char *))(
                           uintptr_t)(g_game_base + 0x5d76d4))(
        slider_button, "bounds");
    if (bounds != 0) {
        int link_ok = 1;
        if (widget_reaches_w2(bounds, slider_button, &link_ok)) {
            widget_set_anchor(bounds, bounds_anchor);
            have_bounds = true;
        }
    }

    uint64_t slider = engine_call_11a2840(0x178);
    if (slider == 0)
        return 0;
    /* slider constructor: (slider, bg, button, 0, 1) */
    ((void (*)(uint64_t, uint64_t, uint64_t, int, int))(
         uintptr_t)(g_game_base + 0x889288))(slider, slider_bg, slider_button,
                                             0, 1);

    {
        uint64_t vptr = 0;
        int rd = (slider + 8) >> 3 >= 0x201
                 && g_host_read(g_host_ctx, slider, &vptr, 8) == 1;
        int plausible = 0xfff < vptr && rd && (vptr & 7) == 0;
        if ((plausible ? vptr : 0) != g_game_base + 0x11c0c58U)
            return 0;
    }
    {
        int link_ok = 1;
        if (!widget_reaches_w2(slider, scale_child, &link_ok))
            return 0;
    }
    {
        uint64_t linked = 0;
        if (slider + 0x130 < 0x1008                 /* raw: >> 3 < 0x201 */
            || g_host_read(g_host_ctx, slider + 0x128, &linked, 8) != 1
            || linked != slider_button)
            return 0;
    }

    engine_call_594130(g_edit_controls_ctx, slider);
    g_edit_controls_panel = slider;                   /* 0x282168 */
    *(uint32_t *)(uintptr_t)(slider + 0xe8) = g_fps_popup_limit;  /* 0x282afc */
    *(uint64_t *)(uintptr_t)(slider + 0xec) = RODATA_10f810;

    if (have_bounds) {
        /* recenter the bounds value at +0x138 by half the width
         * difference between the button and its bounds child */
        float *val = (float *)(uintptr_t)widget_field_ptr_read(slider + 0x138);
        float cur = 0.0f;
        if ((uint64_t)(uintptr_t)(val + 1) >> 2 >= 0x401  /* implausible */
            || g_host_read(g_host_ctx, (uint64_t)(uintptr_t)val, &cur, 4) != 1
            || !isfinite(cur))
            return 0;
        float off = ((btn_anchor[2] - btn_anchor[0])
                     - (bounds_anchor[2] - bounds_anchor[0])) * 0.5f;
        if (!isfinite(off) || fabsf(off) > 512.0f)
            return 0;
        *val = cur - off;
    }

    widget_set_position(slider, -10000.0f /* 0xc61c3c00 */,
                        -10000.0f /* 0xc61c3c00 */);
    return ui_edit_controls_ctx_valid();
}
/* widgets chain 2 chunk 8: covers raw lines 8740-9174 (nexus_menu_dispatch
 * complete + ui_char_classify complete; the nominal 8740-9200 slice ends
 * mid-function at 9200 — the menu_button_label_set straddler moves to chunk 9) */

#define _GNU_SOURCE
#include <pthread.h>

/* ===== chain-2 additions: menu dispatch state (renderer.c/menu_engine.c) ===== */

extern volatile int g_theme_latch;   /* 0x284688 — registry busy latch (misc.c) */
extern int32_t g_theme_menu_id;      /* 0x284694 — owning theme menu id (misc.c) */
extern uint8_t g_menu_visible;       /* 0x28469c — menu visible flag (menu_engine.c) */
extern uint8_t g_menu_battle_row;    /* 0x28469f — battle scroll row (menu_engine.c) */
extern uint32_t g_theme_revision;    /* 0x2846a0 — registry revision (misc.c) */
extern uint32_t g_theme_last_feature;/* 0x2846a4 — last feature id (renderer.c) */
extern uint32_t g_theme_last_status; /* 0x2846a8 — last set_value result (renderer.c) */
extern uint32_t g_theme_row_kind;    /* 0x2846ac — last row kind (renderer.c) */
extern int32_t g_settings_value_cache[118];
                                      /* 0x2846b0 — 118-slot value cache (misc.c) */
extern void *g_plus_active_fn;       /* 0x28dd00 — cached plus_active resolver (renderer.c) */
extern pthread_once_t g_plus_once;   /* 0x28dcfc — plus_active once gate (renderer.c) */

/* spin/settings registry: slot 8 query hooks at 0x2848b0+i*0x40 (chunk 7
 * named them g_feature_handlers) and the debug-open/impor hooks at
 * 0x284a90..0x284aa0 */
extern uint64_t g_debug_open_ctx;    /* 0x284a90 — debug-open channel ctx */
extern int (*g_debug_open_hook)(uint64_t ctx, void *blob,
                                uint32_t size);      /* 0x284a98 */
extern int (*g_import_hook)(uint64_t ctx, void *blob, uint32_t size,
                            void *extra);            /* 0x284aa0 */

/* ===== chain-2 additions: dispatch helpers ===== */

extern int ui_char_classify(uint32_t code);
                                      /* this chain (chunk 8, below) @ 0018d0fc */
extern int menu_settings_blob_verify(void *blob, uint64_t len,
                                     void *values_out);
                                      /* menu_engine.c @ 0018447c — CRC + range verify */
extern void nexus_menu_debug_open(void *blob_out);
                                      /* menu_engine.c @ 001842d0 — build debug blob */
extern int settings_values_commit(void *values);
                                      /* menu_engine.c @ 001846b8 — 29x16B commit */
extern void menu_feature_preset_apply(int preset_id);
                                      /* menu_engine.c @ 0018d64c — preset slots */
extern int settings_blob_push(void);
                                      /* misc.c @ 0018d334 — build + submit blob */
extern long spin_group_record_find(uint32_t feature);
                                      /* misc.c @ 0018d3b4 — spin channel group */
extern int plus_active_refresh_alt(void);
                                      /* misc.c @ 0018d500 — resolve plus_active */
extern int plus_active_query(uint32_t feature);
                                      /* external @ 0018d15c — not in dump */
extern uint32_t settings_apply_now(void);
                                      /* external @ 0018d48c — not in dump */
extern void theme_machine_pump(void);
                                      /* external @ 00185d10 — not in dump */
extern void menu_row_state_init(menu_row_state_t *state,
                                const uint8_t *entry);
                                      /* this chain (chunk 7) @ 00184e28 */
extern int ui_latch_test_and_set(int value, volatile int *latch);
                                      /* menu_engine.c @ 00193f80 */

/* rodata: per-feature classification table (dwords at 0x140830) */
extern const uint32_t FEATURE_CLASS_TABLE[0x9d];  /* 0x140830 — 157 dwords
                                                   * (feature id -> class) */

/*
 * ui_char_classify — classify a feature id for the menu dispatch: the
 * hardcoded ids 0x20/0x2d/0x2f/0x6d/0x6e/0x6f/0x70/0x20002 return 1
 * outright; every other id up to 0x9c reads its class from the rodata
 * table at 0x140830; ids past 0x9c return 0.
 * @ 0018d0fc
 */
int ui_char_classify(uint32_t code)
{
    switch (code) {
    case 0x20:
    case 0x2d:
    case 0x2f:
    case 0x6d:
    case 0x6e:
    case 0x6f:
    case 0x70:
        return 1;
    case 0x22:
    case 0x23:
    case 0x24:
    case 0x26:
    case 0x79:
    case 0x9a:
        return 0;
    default:
        if (code == 0x20002)
            return 1;
        if (0x9c < code)
            return 0;
        return (int)FEATURE_CLASS_TABLE[code];
    }
}

/* ===== menu dispatch (set_value / debug / import / presets) ===== */

/*
 * nexus_menu_dispatch — dispatch a menu operation from the script port:
 * gates (feature <= 0xa7, extra <= 0x100, (a,b) not (0, non-0), theme
 * latch free, tid == g_theme_menu_id), then by feature id: 0x30 opens the
 * debug blob (nexus_menu_debug_open + hook, 0x3c4 bytes), 0x31 imports a
 * settings blob (hook + menu_settings_blob_verify + settings_values_commit),
 * 0x32 pushes the whole 0x1d0 value cache (copied from the spin record
 * table through the register), 0x40/0x4a/0x5c/0x70/0x91 apply feature
 * presets (menu_feature_preset_apply), and every other id queries the
 * capability service (plus_active_query): kinds 5/2 toggle the cached
 * bitmask bit (entry +0x40 bit index), kinds < 8 with the 0xd8 kind mask
 * clamp the requested value into the spin record's min/max (kinds 7/4
 * clamp toward min, others toward max), then the kinds < 5 without the
 * 0x2f-family feature mask build a 0x40-byte spin request (row state via
 * menu_row_state_init, entry text/icon words, spin record head +0x20,
 * feature id/kind/value, params, settings_apply_now stamp) and call the
 * spin group hook at +0x30. Accepted values land in the value cache and
 * settings_blob_push runs. Stores the result in g_theme_last_status and
 * bumps g_theme_revision; the report-pending byte (0x28469f) is cleared
 * on the plain paths. Returns 0 ok, 1/2 channel codes, 3 latch, 4 invalid.
 * @ 00185158
 */
uint32_t nexus_menu_dispatch(uint32_t feature, long param_2,
                             uint64_t param_3, int tid)
{
    uint64_t verdict = 4;

    if (0xa7 < feature || 0x100 < param_3
        || (param_2 == 0 && param_3 != 0))
        return 4;                                     /* invalid */
    if (ui_latch_test_and_set(1, (volatile int *)&g_theme_latch) & 1)
        return 3;                                     /* busy */
    verdict = 4;
    if (tid < 1 || g_theme_menu_id != tid)
        goto unlatch;                                 /* not the owner */

    g_theme_last_feature = feature;                   /* 0x2846a4 */
    if (ui_char_classify(feature) == 0
        && pthread_once(&g_plus_once,
                        (void (*)(void))plus_active_refresh_alt) == 0
        && (g_plus_active_fn == NULL
            || ((int (*)(void))g_plus_active_fn)() != 1)) {
        /* plain feature: report pending cleared, visible flag armed */
        verdict = 1;
        g_menu_battle_row = 0;                        /* 0x28469f */
        g_menu_visible = 1;                           /* raw: word 0x28469c
                                                        * gets 0x5901 (lo 'R'-
                                                        * family screen char,
                                                        * hi visible) */
        goto report;
    }

    theme_machine_pump();                             /* external 0x185d10 */
    {
        const uint8_t *entry = g_feature_entry_table
                               + (size_t)feature * 0x48;
        if (entry[0x47] != 0)
            goto unlatch_zero;                        /* locked -> 0 */

        if (feature == 0x82) {
            verdict = 1;
            /* raw: word 0x28469c &= 0xff00 (clear the screen char) */
            *(uint16_t *)(uintptr_t)0x28469c =
                (uint16_t)(*(const uint16_t *)(uintptr_t)0x28469c & 0xff00);
            goto report;
        }

        uint8_t kind = entry[0x44];
        if (kind == 1) {
            verdict = 1;
            g_menu_battle_row = 0;                    /* 0x28469f */
            /* raw: 0x28469c = (entry[0x45] << 8) | 1 */
            *(uint16_t *)(uintptr_t)0x28469c =
                (uint16_t)(((uint16_t)entry[0x45] << 8) | 1);
            goto report;
        }

        switch (feature) {
        case 0x30:
            /* debug open: build the blob and push it through the hook */
            if (g_debug_open_hook == NULL) {
unlatch_zero:
                verdict = 0;
                goto unlatch;
            }
            {
                uint8_t blob[0x3c4] = {0};
                nexus_menu_debug_open(blob);
                verdict = g_debug_open_hook(g_debug_open_ctx, blob,
                                            0x3c4) != 0 ? 1 : 0xffffffff;
            }
            break;

        case 0x31:
            /* import: verify the blob then commit the values */
            {
                uint64_t extra = 0;
                uint8_t blob[0x3c4] = {0};
                if (g_import_hook == NULL)
                    goto unlatch_zero;
                if (g_import_hook(g_debug_open_ctx, blob, 0x3c4,
                                  &extra) == 0) {
                    verdict = 0xffffffff;
                } else {
                    int32_t values[0x76] = {0};
                    verdict = menu_settings_blob_verify(blob, extra,
                                                        values);
                    if (verdict == 1)
                        verdict = settings_values_commit(values);
                }
            }
            break;

        case 0x32: {
            /* push the whole value cache: raw copies the 0x1d0 block from
             * the spin record table (0x1a36f8 + 0x28, 0x30-stride rows)
             * plus the two tail dwords at +0xaf0/+0xb08 */
            int32_t values[0x76];
            for (int i = 0; i < 0x3a; i++)   /* 58 qwords = 0x1d0 bytes */
                ((uint64_t *)(void *)values)[i] =
                    *(const uint64_t *)(const void *)(
                        *(uintptr_t *)(const void *)(uintptr_t)0x1a36f8
                        + 0x28 + (size_t)i * 0x18);
            values[0x67] = *(const int32_t *)(const void *)(
                *(uintptr_t *)(const void *)(uintptr_t)0x1a36f8 + 0xaf0);
            values[0x69] = *(const int32_t *)(const void *)(
                *(uintptr_t *)(const void *)(uintptr_t)0x1a36f8 + 0xb08);
            verdict = settings_values_commit(values);
            goto blob_push;
        }

        default:
            if (feature == 0x40 || feature == 0x4a || feature == 0x5c
                || feature == 0x70 || feature == 0x91) {
                verdict = 0;   /* menu_feature_preset_apply returns void in
                                * the raw; its verdict word is not consumed */
                menu_feature_preset_apply((int)feature);
blob_push:
                if ((uint32_t)(verdict - 1) < 2)
                    verdict = settings_blob_push();
                break;
            }
            {
                /* generic feature: query the capability service */
                int cap = plus_active_query(feature);   /* external */
                uint32_t cached = cap < 0
                    ? 0
                    : (uint32_t)g_settings_value_cache[cap];
                uint32_t value;

                if (kind == 5 || kind == 2) {
                    /* toggle family: flip the cached bit */
                    value = (uint32_t)(cached == 0);
                    int32_t bit = *(const int32_t *)(const void *)(
                        entry + 0x40);
                    if (bit >= 0)
                        value = (1u << (bit & 0x1f)) ^ cached;
                } else {
                    value = cached;
                    if (cap >= 0 && kind < 8
                        && ((1u << (kind & 0x1f)) & 0xd8U) != 0) {
                        /* spinner family: clamp param into the record
                         * range (entry +0x3c delta, spin record +8/+0xc) */
                        long req = (long)*(const int32_t *)(const void *)(
                                       entry + 0x3c)
                                   + (long)(int32_t)cached;
                        long lo = *(const int32_t *)(const void *)(
                            *(uintptr_t *)(const void *)(uintptr_t)0x1a36f8
                            + (size_t)cap * 0x18 + 8);
                        long hi = *(const int32_t *)(const void *)(
                            *(uintptr_t *)(const void *)(uintptr_t)0x1a36f8
                            + (size_t)cap * 0x18 + 0x10);
                        if (kind == 7 || kind == 4)
                            value = (uint32_t)(lo <= req ? req : lo
                                               <= (hi <= req ? hi : req)
                                                   ? (hi <= req ? hi : req)
                                                   : req);
                        else
                            value = (uint32_t)(req <= hi ? req : hi);
                    }
                }

                uint32_t channel_result = 1;
                if (kind < 5
                    && (0x3d < feature - 0x2f
                        || ((1ULL << ((feature - 0x2f) & 0x3f)
                             & 0x2000000003c00001ULL) == 0))) {
                    /* spin request through the owning group hook */
                    long rec = spin_group_record_find(feature);
                    menu_row_state_t row_state;
                    uint8_t blob[0x40];

                    memset(&row_state, 0, sizeof row_state);
                    menu_row_state_init(&row_state, entry);
                    g_theme_row_kind = row_state.enabled_bit;  /* raw: the
                        * first out dword of the state (0x2846ac) */
                    if (rec == 0 || ((uint8_t *)&row_state)[4] == 0) {
                        verdict = 0;
                        break;
                    }

                    memset(blob, 0, sizeof blob);
                    {
                        struct {
                            uint64_t text_pair;    /* entry +0x30 */
                            uint64_t icon_pair;    /* entry +0x28 */
                            uint64_t head;         /* spin record head
                                                    * (entry +0x20, or the
                                                    * group ctx record) */
                            uint32_t size;         /* 0x40 */
                            uint32_t feature;      /* param_1 */
                            uint32_t kind;         /* entry +0x44 */
                            uint32_t value;        /* computed value */
                            uint64_t a;            /* param_2 */
                            uint64_t b;            /* param_3 */
                            uint32_t stamp;        /* apply-now stamp */
                            uint32_t _pad;
                        } req;
                        memset(&req, 0, sizeof req);
                        req.text_pair = *(const uint64_t *)(const void *)(
                            entry + 0x30);
                        req.icon_pair = *(const uint64_t *)(const void *)(
                            entry + 0x28);
                        req.head = cap < 0
                            ? *(const uint64_t *)(const void *)(entry + 0x20)
                            : *(const uint64_t *)(const void *)(
                                *(uintptr_t *)(const void *)(uintptr_t)0x1a36f8
                                + (size_t)cap * 0x18);
                        req.size = 0x40;
                        req.feature = feature;
                        req.kind = kind;
                        req.value = value;
                        req.a = (uint64_t)param_2;
                        req.b = param_3;
                        req.stamp = settings_apply_now();  /* external */
                        memcpy(blob, &req, sizeof req);
                    }
                    /* group hook: fn at rec+0x30, ctx at rec+8 */
                    channel_result =
                        ((uint32_t (*)(uint64_t, void *))(
                             *(void **)(const void *)(uintptr_t)(
                                 rec + 0x30)))(
                            *(uint64_t *)(const void *)(uintptr_t)(rec + 8),
                            blob);
                }

                if (cap >= 0 && channel_result - 1 < 2) {
                    g_settings_value_cache[cap] = (int32_t)value;
                    verdict = settings_blob_push();
                    if (verdict != 1)
                        break;
                }
                verdict = channel_result;
            }
            break;
        }
    }

report:
    g_theme_last_status = (uint32_t)verdict;          /* 0x2846a8 */
    g_theme_revision = g_theme_revision + 1;          /* 0x2846a0 */
unlatch:
    g_theme_latch = 0;                                /* 0x284688 */
    return (uint32_t)verdict;
}
/* widgets chain 2 chunk 9: covers raw lines 9176-9645 (menu_button_label_set
 * complete, menu_empty_panel_observe complete, ui_named_state_query complete,
 * ui_named_action_invoke complete and the nexus_rich_plan export thunk —
 * the file tail) */

/* ===== chain-2 additions: engine ops block / label cache ===== */

extern uint64_t g_engine_ctx;         /* 0x28fac8 — engine ctx handle (menu_engine.c) */
extern uint64_t g_engine_stage_fn;    /* 0x28fb18 — stage fn */
extern uint64_t g_engine_pos_fn;      /* 0x28fb20 — set-position fn */
extern uint64_t g_engine_log_fn;      /* 0x28fb38 — engine log sink */
extern int32_t g_engine_state;        /* 0x28fb48 — observe state (-1..2) */
extern uint32_t g_engine_owner_id;    /* 0x28fb4c — owning tid */
extern uint32_t g_engine_log_count;   /* 0x28fb54 — capped-0x80 counter */
extern uint64_t g_engine_buttons[0x18];
                                      /* 0x28fb80 — 24 button handles */

/* persistent label cache: 0x200 records of 0x90 bytes at 0x28fed8 (text at
 * record-0x80, i.e. 0x28fe58 + i*0x90), count at 0x28fb58 */
extern char g_menu_label_cache[0x200][0x90];  /* 0x28fe58 — label texts */
extern uint32_t g_menu_label_count;           /* 0x28fb58 — cached label count */

/* ===== chain-2 additions: engine observe helpers ===== */

extern void theme_menu_status_set(int code, uint32_t owner,
                                  const char *reason);
                                      /* menu_engine.c @ 00183cdc */
extern int channel_child_link_valid(const void *handle[2], uint64_t node,
                                    uint64_t parent);  /* misc.c @ 001905a4 */
extern int channel_ancestor_chain_check(const void *handle[2],
                                        uint64_t node,
                                        uint64_t target); /* misc.c @ 001907b0 */
extern int channel_root_node_check(const void *base_and_handle[2],
                                   uint64_t node);       /* misc.c @ 001908c0 */
extern int channel_button_style_apply(void *state, const void *ops[13],
                                      uint64_t button, const char *style,
                                      uint64_t *slot,
                                      const char *text);  /* misc.c @ 00190ad0 */
extern uint64_t channel_read_ptr(const void *handle[2],
                                 uint64_t addr);          /* misc.c @ 00190510 */

/* ===== persistent button labels ===== */

/*
 * menu_button_label_set — set the persistent label of menu button `slot`:
 * with the text shorter than 0x80 bytes the label cache (0x200 x 0x90
 * records at 0x28fe58, count at 0x28fb58) is searched for the same text
 * and reused; on a miss (count < 0x200) the text is appended, the record
 * staged through the engine stage fn (0x28fb18) and the new record head
 * adopted. The button handle at 0x28fb80+slot*8 is re-pointed through the
 * engine position fn (0x28fb20) whenever it differs from the record head
 * (0x28fcc0+slot*8). Overlong text or a full cache fails with -1, latches
 * the observe state to -1 and reports 'persistent_label_budget' through
 * theme_menu_status_set and the capped 'nexus_menu_stopped' log (cap 0x80).
 * Returns 1 on success. @ 0018f988
 */
int menu_button_label_set(int slot, const char *text)
{
    size_t len = strlen(text);
    uint32_t count = g_menu_label_count;             /* 0x28fb58 */

    if (len >= 0x80) {
label_budget:
        g_engine_state = -1;                          /* 0x28fb48 */
        theme_menu_status_set(-1, g_engine_owner_id,
                              "persistent_label_budget");
        uint32_t under_cap = g_engine_log_count < 0x80;
        g_engine_log_count = g_engine_log_count + 1;
        if (under_cap && g_engine_log_fn != 0)
            ((void (*)(uint64_t, const char *, const char *,
                       uint32_t))(uintptr_t)g_engine_log_fn)(
                g_engine_ctx, "nexus_menu_stopped",
                "persistent_label_budget", 0);
        return -1;
    }

    uint64_t rec_head = 0;
    if (count != 0) {
        uint64_t rec = 0x28fed8;
        uint32_t left = count;
        do {
            if (strcmp((const char *)(uintptr_t)(rec - 0x80), text) == 0) {
                rec_head = rec;
                goto have_record;
            }
            rec += 0x90;
            left--;
        } while (left != 0);
        if (count == 0x200)
            goto label_budget;
    }

    /* append the text as a fresh record */
    g_menu_label_count = count + 1;
    memcpy((void *)(uintptr_t)(0x28fe58 + (size_t)count * 0x90), text,
           len + 1);
    rec_head = (size_t)count * 0x90 + 0x28fed8;
    ((void (*)(uint64_t, uint64_t,
               const void *))(uintptr_t)g_engine_stage_fn)(
        g_engine_ctx, rec_head,
        (const void *)(uintptr_t)(0x28fe58 + (size_t)count * 0x90));

have_record:
    if (*(const uint64_t *)(const void *)(uintptr_t)(0x28fcc0
                                                    + (size_t)slot * 8)
        == rec_head)
        return 1;
    ((void (*)(uint64_t, uint64_t,
               uint64_t))(uintptr_t)g_engine_pos_fn)(
        g_engine_ctx, g_engine_buttons[slot], rec_head);
    *(uint64_t *)(uintptr_t)(0x28fcc0 + (size_t)slot * 8) = rec_head;
    return 1;
}

/* ===== empty-panel observer (native launcher buttons) ===== */

/* engine ops vtable handed to the observer (13 slots at param_2):
 * {ctx, read, stage_fn, alloc, ctor_a, ctor_b, movie_load, movie_bind,
 *  frame_set, attach, set_text_fn, add_child, log}. */
typedef struct {
    uint64_t ctx;          /* +0x00 */
    uint64_t read;         /* +0x08 — channel read */
    uint64_t stage_fn;     /* +0x10 */
    uint64_t alloc;        /* +0x18 */
    uint64_t ctor_a;       /* +0x20 */
    uint64_t ctor_b;       /* +0x28 */
    uint64_t movie_load;   /* +0x30 */
    uint64_t movie_bind;   /* +0x38 */
    uint64_t frame_set;    /* +0x40 */
    uint64_t attach;       /* +0x48 */
    uint64_t set_text_fn;  /* +0x50 */
    uint64_t add_child;    /* +0x58 */
    uint64_t log;          /* +0x60 */
} engine_ops_vt_t;

/* observer state (0x58 bytes): the raw walks it as qwords */
typedef struct {
    uint64_t game_base;    /* +0x00 — stage root base */
    uint64_t stage_root;   /* +0x08 */
    uint64_t stage_view;   /* +0x10 */
    uint64_t button_a;     /* +0x18 */
    uint64_t button_b;     /* +0x20 */
    uint64_t text_a;       /* +0x28 — 0x28fe58 record head */
    uint64_t deadline;     /* +0x30 — asset readiness deadline */
    int32_t  state;        /* +0x38 — 1 constructed / 2 attached / 3 stopped */
    uint32_t clicks;       /* +0x3c */
    int32_t  last_depth;   /* +0x40 — scheduling depth gate */
    uint32_t flags;        /* +0x44 — latch bits */
    int32_t  probes;       /* +0x48 — asset probe count */
    int32_t  waits;        /* +0x4c — waiting_assets count */
    uint32_t in_use;       /* +0x50 — scheduling re-entrancy flag */
    uint32_t open;         /* +0x54 — panel open mirror */
} menu_empty_panel_state_t;

/*
 * menu_empty_panel_observe — per-frame observer for the engine's empty
 * panel (native launcher): under the scheduling gate (state != 3, text
 * given, depth <= 1, not re-entered, "Mainloop" scheduling matches the
 * depth gate) it reads the stage root (game base + 0x12eb9f0) and its
 * +0x90 view through the channel read, then: state 2 revalidates the
 * stage/view pair and both buttons (channel_child_link_valid), toggling
 * the panel open on a click of button a (positions 128.0/-10000.0) and
 * logging click/open/close; state 0 waits for the rooted text (channel
 * vtable game+0x11abad8, channel_ancestor_chain_check) with a 32-probe /
 * 0x7520-unit budget, then constructs the two 0x260 buttons
 * (channel_button_style_apply with "popover_button_blue"/"NEXUS" and
 * "popover_button_spectate"/""), parks them at (-10000,-10000) /
 * (90.0, 40.0), attaches both to the stage root and revalidates —
 * reaching state 2 ("attached") or 3 ("stopped") with the exact reason
 * string logged through the ops log slot. @ 0018fe44
 */
void menu_empty_panel_observe(long *state, const uint64_t *ops_vt,
                              uint32_t action, long param_4, int depth,
                              char *text, uint64_t now)
{
    menu_empty_panel_state_t *st = (menu_empty_panel_state_t *)state;
    const engine_ops_vt_t *ops = (const engine_ops_vt_t *)ops_vt;
    int (*read_fn)(uint64_t, uint64_t, void *, uint64_t) =
        (int (*)(uint64_t, uint64_t, void *, uint64_t))ops->read;

    if (st->state == 3 || text == NULL || 1 < action
        || (st->in_use != 0 || (strcmp(text, "Mainloop") != 0 || depth < 1))
        || (strcmp(text, "Mainloop") == 0 && st->last_depth != 0
            && st->last_depth != depth))
        return;

    st->in_use = 1;
    if (action == 0) {
        st->text_a = (uint64_t)param_4;
        if ((st->flags & 1) == 0) {
            st->flags |= 1;
            ((void (*)(uint64_t, const char *, const char *,
                       void *))(uintptr_t)ops->log)(ops->ctx,
                "scheduling_seen", "Mainloop_TextField_setText", state);
        }
    }

    /* stage root: game base + 0x12eb9f0 (game_base word at state +0) */
    uint64_t stage_root = 0;
    if (st->game_base + 0x12eb9f8 >= 0x1008          /* raw: >> 3 < 0x201 */
        && read_fn(ops->ctx, st->game_base + 0x12eb9f0, &stage_root, 8) == 1
        && 0xfff < stage_root && (stage_root & 7) == 0) {
        uint64_t stage_view = 0;
        int view_ok = st->stage_root + 0x98 >= 0x1008
                      && read_fn(ops->ctx, stage_root + 0x90,
                                 &stage_view, 8) != 0
                      && 0xfff < stage_view && (stage_view & 7) == 0;

        if (st->state == 2) {
            /* attached: revalidate both buttons against the stage */
            if (st->stage_root != stage_root || st->stage_view != stage_view
                || channel_child_link_valid((const void **)ops,
                                            st->button_a, stage_view) == 0
                || channel_child_link_valid((const void **)ops,
                                            st->button_b, stage_view) == 0) {
                st->state = 3;
                ((void (*)(uint64_t, const char *, const char *,
                           void *))(uintptr_t)ops->log)(ops->ctx, "stopped",
                    "stage_generation_or_attachment_changed", state);
                goto out;
            }
            if (action == 1 && st->button_a == (uint64_t)param_4) {
                /* click on the launcher: toggle the panel */
                ((void (*)(uint64_t, const char *, const char *,
                           void *))(uintptr_t)ops->log)(ops->ctx, "click",
                                                       "own_launcher",
                                                       state);
                st->clicks = st->clicks == 0;
                float x = st->clicks ? 0x43200000 /* 178.0 */ :
                                       0xc61c3c00 /* -10000.0 */;
                float y = st->clicks ? 0x433e0000 /* 190.0 */ :
                                       0xc61c3c00 /* -10000.0 */;
                ((void (*)(float, float, uint64_t,
                           uint64_t))(uintptr_t)ops->frame_set)(
                    y, x, ops->ctx, st->button_b);
                ((void (*)(uint64_t, const char *, const char *,
                           void *))(uintptr_t)ops->log)(
                    ops->ctx, st->clicks ? "open" : "close",
                    "empty_panel", state);
            }
        } else if (action == 0) {
            /* constructing: wait for the rooted text field */
            int rooted = 0;
            if (stage_view != 0 && view_ok) {
                uint64_t vptr_obj = channel_read_ptr((const void **)ops,
                                                     (uint64_t)param_4);
                if (vptr_obj == st->game_base + 0x11abad8) {
                    if (channel_ancestor_chain_check((const void **)ops,
                                                     (uint64_t)param_4,
                                                     stage_view) != 0) {
                        rooted = 1;
                        /* asset readiness budget: 32 probes / 0x7520 units */
                        uint64_t first = st->text_a == 0 ? now : st->text_a;
                        if (st->deadline <= now) {
                            if (0x1f < st->probes
                                || 0x752 < (now - first) >> 5) {
                                st->state = 3;
                                ((void (*)(uint64_t, const char *,
                                           const char *,
                                           void *))(uintptr_t)ops->log)(
                                    ops->ctx, "stopped",
                                    "asset_readiness_budget", state);
                                goto out;
                            }
                            st->deadline = now + 500;
                            st->probes++;
                            if (((int (*)(uint64_t))(uintptr_t)ops->ctor_a)(
                                    ops->ctx) != 0) {
                                st->stage_root = stage_root;
                                st->stage_view = stage_view;
                                st->state = 1;
                                st->last_depth = depth;
                                ((void (*)(uint64_t, const char *,
                                           const char *,
                                           void *))(uintptr_t)ops->log)(
                                    ops->ctx, "engine_thread",
                                    "rooted_TextField_setText", state);
                                st->button_a =
                                    ((uint64_t (*)(uint64_t,
                                                   uint64_t))(uintptr_t)
                                         ops->alloc)(ops->ctx, 0x260);
                                st->button_b =
                                    ((uint64_t (*)(uint64_t,
                                                   uint64_t))(uintptr_t)
                                         ops->alloc)(ops->ctx, 0x260);
                                if (st->button_a == 0 || st->button_b == 0) {
                                    st->state = 3;
                                    ((void (*)(uint64_t, const char *,
                                               const char *,
                                               void *))(uintptr_t)ops->log)(
                                        ops->ctx, "stopped",
                                        "allocation_failed", state);
                                } else {
                                    ((void (*)(uint64_t,
                                               ...))(uintptr_t)ops->ctor_b)(
                                        ops->ctx);
                                    ((void (*)(uint64_t,
                                               ...))(uintptr_t)ops->ctor_b)(
                                        ops->ctx, st->button_b);
                                    if (channel_root_node_check(
                                            (const void **)ops,
                                            st->button_a) == 0
                                        || channel_root_node_check(
                                               (const void **)ops,
                                               st->button_b) == 0) {
                                        st->state = 3;
                                        ((void (*)(uint64_t, const char *,
                                                   const char *,
                                                   void *))(
                                             uintptr_t)ops->log)(
                                            ops->ctx, "stopped",
                                            "initial_button_state", state);
                                    } else if (
                                        channel_button_style_apply(
                                            state, (const void **)ops,
                                            st->button_a,
                                            "popover_button_blue",
                                            &st->text_a,
                                            "NEXUS") == 0
                                        || channel_button_style_apply(
                                               state, (const void **)ops,
                                               st->button_b,
                                               "popover_button_spectate",
                                               (uint64_t *)&st->deadline,
                                               g_empty_text) == 0) {
                                        goto out;   /* raw logs inside */
                                    } else if (
                                        channel_root_node_check(
                                            (const void **)ops,
                                            st->button_a) == 0
                                        || channel_root_node_check(
                                               (const void **)ops,
                                               st->button_b) == 0) {
                                        st->state = 3;
                                        ((void (*)(uint64_t, const char *,
                                                   const char *,
                                                   void *))(
                                             uintptr_t)ops->log)(
                                            ops->ctx, "stopped",
                                            "configured_button_state",
                                            state);
                                    } else {
                                        ((void (*)(uint64_t, const char *,
                                                   const char *,
                                                   void *))(
                                             uintptr_t)ops->log)(
                                            ops->ctx, "constructed",
                                            "two_0x260_buttons_no_features",
                                            state);
                                        /* stage identity re-check */
                                        if (channel_read_ptr(
                                                (const void **)ops,
                                                st->game_base + 0x12eb9f0)
                                                == stage_root
                                            && channel_read_ptr(
                                                   (const void **)ops,
                                                   stage_root + 0x90)
                                                   == stage_view) {
                                            ((void (*)(float, float,
                                                       uint64_t,
                                                       uint64_t))(
                                                 uintptr_t)ops->frame_set)(
                                                0xc61c3c00 /* -10000.0 */,
                                                0xc61c3c00 /* -10000.0 */,
                                                ops->ctx, st->button_b);
                                            ((void (*)(float, float,
                                                       uint64_t,
                                                       uint64_t))(
                                                 uintptr_t)ops->frame_set)(
                                                0x42200000 /* 40.0 */,
                                                0x42b40000 /* 90.0 */,
                                                ops->ctx, st->button_a);
                                            ((void (*)(uint64_t, uint64_t,
                                                       uint64_t))(
                                                 uintptr_t)ops->movie_load)(
                                                ops->ctx, stage_root,
                                                st->button_b);
                                            ((void (*)(uint64_t, uint64_t,
                                                       uint64_t))(
                                                 uintptr_t)ops->movie_load)(
                                                ops->ctx, stage_root,
                                                st->button_a);
                                            if (channel_child_link_valid(
                                                    (const void **)ops,
                                                    st->button_b,
                                                    stage_view) == 0
                                                || channel_child_link_valid(
                                                       (const void **)ops,
                                                       st->button_a,
                                                       stage_view) == 0) {
                                                st->state = 3;
                                                ((void (*)(uint64_t,
                                                           const char *,
                                                           const char *,
                                                           void *))(
                                                     uintptr_t)ops->log)(
                                                    ops->ctx, "stopped",
                                                    "attachment_postcondition",
                                                    state);
                                            } else {
                                                st->state = 2;
                                                ((void (*)(uint64_t,
                                                           const char *,
                                                           const char *,
                                                           void *))(
                                                     uintptr_t)ops->log)(
                                                    ops->ctx, "attached",
                                                    "exact_root_parent_and_"
                                                    "vector_membership",
                                                    state);
                                            }
                                        } else {
                                            st->state = 3;
                                            ((void (*)(uint64_t,
                                                       const char *,
                                                       const char *,
                                                       void *))(
                                                 uintptr_t)ops->log)(
                                                ops->ctx, "stopped",
                                                "stage_changed_during_"
                                                "construction", state);
                                        }
                                    }
                                }
                                goto out;
                            }
                            /* assets not ready yet: waiting_assets (once) */
                            st->waits++;
                            if (st->waits == 1)
                                ((void (*)(uint64_t, const char *,
                                           const char *,
                                           void *))(uintptr_t)ops->log)(
                                    ops->ctx, "waiting_assets",
                                    "ui_sc_exports", state);
                            goto out;
                        }
                        rooted = rooted;   /* deadline not reached: wait */
                    } else if ((st->flags & 8) == 0) {
                        st->flags |= 8;
                        ((void (*)(uint64_t, const char *, const char *,
                                   void *))(uintptr_t)ops->log)(ops->ctx,
                            "waiting_receiver",
                            "base_TextField_vtable_required", state);
                    }
                } else if ((st->flags & 8) == 0) {
                    st->flags |= 8;
                    ((void (*)(uint64_t, const char *, const char *,
                               void *))(uintptr_t)ops->log)(ops->ctx,
                        "waiting_receiver",
                        "base_TextField_vtable_required", state);
                }
            } else if ((st->flags & 2) == 0) {
                st->flags |= 2;
                ((void (*)(uint64_t, const char *, const char *,
                           void *))(uintptr_t)ops->log)(ops->ctx,
                    "waiting_stage", "stage_or_root_null", state);
            }
            (void)rooted;
        }
    } else if ((st->flags & 2) == 0) {
        st->flags |= 2;
        ((void (*)(uint64_t, const char *, const char *,
                   void *))(uintptr_t)ops->log)(ops->ctx,
            "waiting_stage", "stage_or_root_null", state);
    }

out:
    st->in_use = 0;
}

/* ===== named state / action (script-port bridge) ===== */

extern void script_port_table_load(void);
                                      /* misc.c @ 0019198c */
extern int battle_end_feature_arm(const char *name);
                                      /* widgets.c (chain 1) @ 0014f5b8 —
                                        * invoke a named feature action */
extern uint64_t g_named_state_pair;   /* 0x10f960 — rodata {lo, hi} pair */
extern const char *g_named_actions[]; /* 0x1a2fb8 — 3-stride action names */
extern void *g_named_query_fn;        /* 0x2a1f28 — named query hook */
extern void *g_named_apply_fn;        /* 0x2a1f30 — named apply hook */
extern void *g_named_reset_fn;        /* 0x2a1f10 — named reset hook */
extern void *g_named_apply2_fn;       /* 0x2a1e70 — apply (4,0,0) hook */

/*
 * ui_named_state_query — fill an 0x18-byte state record for a named
 * script-port popup: the five known names (BattleServersPopup,
 * ThemesPopup, BSDDebugPopup, ProfilePopup, MapEditorPopup) return the
 * plain record {0x18, 0, 1, 0, parent_flag 0}; "CameraReset" loads the
 * script-port table and returns the rodata pair state (with the reset
 * hook's availability at +0x10/+0x14); anything else queries the named
 * hook (0x2a1f28) and folds {value, valid} into the record with
 * parent_flag 1. Returns 1, or 4 when out is NULL. @ 00192340
 */
int ui_named_state_query(const char *name, void *out)
{
    uint32_t *rec = (uint32_t *)out;

    if (out == NULL)
        return 4;

    if (strcmp(name, "BattleServersPopup") == 0
        || strcmp(name, "ThemesPopup") == 0
        || strcmp(name, "BSDDebugPopup") == 0
        || strcmp(name, "ProfilePopup") == 0
        || strcmp(name, "MapEditorPopup") == 0) {
        /* raw: {+0x0c, +0x04} = 0, +0x00 = 0x18, +0x14 = 1 */
        rec[3] = 0;
        rec[1] = 0;
        rec[0] = 0x18;
        rec[5] = 1;
        ((uint8_t *)rec)[0x14 + 1] = 0;
        return 1;
    }

    if (strcmp(name, "CameraReset") == 0) {
        script_port_table_load();
        ((uint8_t *)rec)[0x15] = 0;
        /* raw: state pair from the rodata literal at 0x10f960/0x10f968 */
        rec[1] = (uint32_t)(g_named_state_pair >> 32);   /* rodata 0x10f968 hi */
        rec[0] = (uint32_t)g_named_state_pair;           /* rodata 0x10f960 lo */
        bool reset_ready = g_named_reset_fn == 0;        /* 0x2a1f10 */
        rec[2] = (uint32_t)reset_ready;
        ((uint8_t *)rec)[0x14] = !reset_ready;
    } else {
        uint32_t valid = 0;
        uint32_t value = 0;
        if (g_named_query_fn != 0) {
            uint32_t out_value = 0, out_valid = 1;
            if (((int (*)(const char *, uint32_t *,
                          uint32_t *))(uintptr_t)g_named_query_fn)(
                    name, &out_value, &out_valid) != 0)
                valid = 1;
            value = valid ? out_value : 0;
        }
        rec[1] = value;                     /* +0x04 */
        rec[3] = valid ? value : 0;         /* raw: the scratch value */
        ((char *)rec)[0x14] = (char)valid;
        ((uint8_t *)rec)[0x15] = 1;
        rec[0] = 0x18;
        rec[1] = valid ? value : 0;
        rec[2] = valid ^ 1;
    }
    *(uint16_t *)((uint8_t *)rec + 0x16) = 0;
    return 1;
}

/*
 * ui_named_action_invoke — invoke a named script-port action by id
 * (0x21000-based): out-of-range ids return 4; "CameraReset" loads the
 * table and runs the reset hook (then the apply hook with (4,0,0)); ids
 * covered by the 0x29811000000000 bit mask run their name through
 * battle_end_feature_arm (0x14f5b8); all others compute the slot index from
 * the named query hook (value+1 / divisor) and run the named apply hook
 * on the remainder. Returns the hook verdict (0/1), 4 on range failure.
 * @ 001924cc
 */
int ui_named_action_invoke(int action_id)
{
    if ((uint32_t)(action_id - 0x21037) > 0xffffffc8)
        return 4;

    script_port_table_load();
    const char *name =
        g_named_actions[(size_t)(action_id - 0x21000) * 3];
    if (strcmp(name, "CameraReset") == 0) {
        script_port_table_load();
        if (g_named_reset_fn == 0)
            return 0;
        uint64_t verdict =
            ((uint64_t (*)(int))(uintptr_t)g_named_reset_fn)(1);
        if ((int)verdict != 0
            && (g_named_apply2_fn == 0
                || (verdict = ((uint64_t (*)(int, int, int))(
                                   uintptr_t)g_named_apply2_fn)(4, 0, 0),
                     (int)verdict != 0)))
            verdict = 1;
        return (int)verdict;
    }

    if ((0x29811000000000ULL >> ((action_id - 0x21000) & 0x3f) & 1) != 0) {
        battle_end_feature_arm(name);
        return 0;
    }

    uint64_t verdict = 0;
    if (g_named_query_fn != 0 && g_named_apply_fn != 0) {
        uint32_t lo = 0, hi = 0;
        if (((int (*)(const char *, uint32_t *,
                      uint32_t *))(uintptr_t)g_named_query_fn)(name, &hi,
                                                                &lo) != 0
            && 0 < (int32_t)lo) {
            int32_t div = (int32_t)lo + 1;
            int32_t rem = 0;
            if (div != 0)
                rem = ((int32_t)hi + 1) / div;
            verdict = ((uint64_t (*)(const char *,
                                     int))(uintptr_t)g_named_apply_fn)(
                name, (int32_t)hi + 1 - rem * div) != 0;
        }
    }
    return (int)verdict;
}

/* ===== export thunk ===== */

extern void nexus_rich_plan_impl(uint32_t width, uint32_t height,
                                 uint64_t frame, uint32_t *view);
                                      /* renderer.c p2 @ 0016a3ec — the
                                        * rich-menu plan this thunk forwards
                                        * to through PTR 0x1a3b50 */

/*
 * nexus_rich_plan — dlsym export thunk: forwards through the PTR slot at
 * 0x1a3b50 to the rich-menu plan implementation (renderer.c). The raw
 * tail-call passes no arguments through (the slot call receives the
 * caller's registers untouched). — dlsym export thunk @ 00194890
 */
void nexus_rich_plan__export(void)
{
    ((void (*)(void))(uintptr_t)(*(uintptr_t *)(const void *)(uintptr_t)
                                     0x1a3b50))();
}
