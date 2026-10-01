/* misc_a chain chunk 1: covers raw lines 1-400 */

/*
 * misc — UI subsystem
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
 * Notes: Unclassified remainder.
 */

/*
 * Reconstructed: ui/misc.c is assembled from two parallel chains —
 * misc_a (raw dump lines 1-10116, this chain) and misc_b (raw dump lines
 * 10117 to the end). This chunk 1 carries the file header plus:
 * startup/lifecycle shims, the NexusOnlineProbe poll, clock_ms, the
 * performance-mode getter, code-region hash verification (and its veneer)
 * and the protected-channel acquire/validate pair. The trailing raw-range
 * function (protected_channel_validate) straddles line 400 and is finished
 * here through raw line 406.
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
#include <time.h>

/* ===== bionic / libc-internal imports (declared for the syntax gate) ===== */
extern int __android_log_print(int prio, const char *tag, const char *fmt, ...);
extern int __cxa_atexit(void (*func)(void *), void *arg, void *dso_handle);
extern int __register_atfork(void (*prepare)(void), void (*parent)(void),
                             void (*child)(void), void *dso_handle);

/* ===== cross-file functions ===== */
typedef struct sha256_state sha256_state_t; /* full layout in the c-chain region */
extern int ui_latch_test_and_set(int value, volatile int *latch);   /* menu_engine.c @ 00193f80 */
extern void sha256_init(sha256_state_t *state);                     /* misc.c (c-chain) @ 00191314 — 112-byte context */
extern void sha256_update(sha256_state_t *state, const void *data, size_t len); /* misc.c (c-chain) @ 00191338 */
extern void sha256_final(sha256_state_t *state, void *out32);       /* misc.c (c-chain) @ 00191678 */

/* ===== forward declarations (defined later in this chain) ===== */
int code_region_hash_verify(uint64_t offset, size_t len,
                            const char *expect_hex);                /* @ 0014d168 */
int proc_mem_read(void *ctx, uint64_t addr, void *out, uint32_t len); /* @ 0014e7c4 */
int str_format(char *buf, size_t cap, size_t slen,
               const char *fmt, ...);                               /* @ 0015540c */

/* ===== shared globals ===== */

/* NexusOnlineProbe state: the JavaVM/class/method triple is captured by
 * widgets.c; the log/deadline words and cached result are misc-local. */
extern void *g_probe_jvm;            /* 0x22d860 — JavaVM* for the online probe */
extern void *g_probe_class;          /* 0x22d868 — jclass global ref */
extern void *g_probe_method;         /* 0x22d870 — jmethodID of the probe method */
static uint64_t g_probe_log_ms;      /* 0x22d858 — last probe log time (ms) */
static uint64_t g_probe_deadline_ms; /* 0x22d878 — next allowed poll time (ms) */
static int g_probe_cache;            /* 0x1a7c00 — cached probe result */

/* protected channel captured by widgets.c (0x22d8b0 block) */
extern uint8_t g_channel_installed;  /* 0x22d8b0 — 1 = channel captured */
extern void *g_channel_class;        /* 0x22d8b8 — jclass global ref */
extern void *g_channel_method;       /* 0x22d8c0 — jmethodID */
extern int32_t g_channel_ready;      /* 0x22d8c8 — 1 = channel usable */
extern void *g_channel_jvm;          /* 0x22d8d0 — JavaVM* */
extern void *g_channel_arg_ref;      /* 0x22f030 — global ref of the channel argument */

/* protected-channel snapshot block (0x1a7c08, latched copies) */
extern int g_pchan_latch;            /* 0x1a7c08 — snapshot latch */
extern int g_pchan_counter;          /* 0x1a7c0c — snapshot revision counter */
extern int g_pchan_fn_a;             /* 0x1a7c10 — captured channel function id a */
extern int g_pchan_fn_b;             /* 0x1a7c14 — captured channel function id b */
extern int g_pchan_field_18;         /* 0x1a7c18 — published status field */
extern uint64_t g_pchan_handle;      /* 0x1a7c20 — captured channel handle */
extern uint8_t g_pchan_flag_a;       /* 0x1a7c28 — channel flag a */
extern uint8_t g_pchan_flag_b;       /* 0x1a7c29 — channel flag b (degraded) */
extern uint8_t g_pchan_flag_c;       /* 0x1a7c2a — channel flag c */

/* misc-wide words */
extern uint64_t g_battle_state;      /* 0x22d880 — battle/screen state word */
extern int64_t g_proc_mem_bias;      /* 0x22d8e0 — bias (libg base) added to remote reads */
extern void *g_dso_handle;           /* 0x1989f0 — module DSO identity slot for atexit */

/* ===== JNI vtable veneers =====
 * The arm64 code calls Java through raw function-table slots; each helper
 * keeps the slot offset as the plain byte number seen in the dump
 * (index = slot / 8). Suffix _ma keeps these chain-local (misc_a). */
static long jni_call0_ma(void *obj, uint32_t slot)
{
    long *table = *(long **)obj;
    long (*fn)(void *) = (long (*)(void *))table[slot / 8];
    return fn(obj);
}

static long jni_call2_ma(void *obj, uint32_t slot, long a, long b)
{
    long *table = *(long **)obj;
    long (*fn)(void *, long, long) = (long (*)(void *, long, long))table[slot / 8];
    return fn(obj, a, b);
}

static long jni_call3_ma(void *obj, uint32_t slot, long a, long b, long c)
{
    long *table = *(long **)obj;
    long (*fn)(void *, long, long, long) =
        (long (*)(void *, long, long, long))table[slot / 8];
    return fn(obj, a, b, c);
}

/* ===== startup / lifecycle shims ===== */

/*
 * noop_stub — empty placeholder kept for address parity; call sites jump
 * here to do nothing. @ 0014c4c0
 */
void noop_stub(void)
{
}

/*
 * atexit_dtor_thunk — trampoline registered with __cxa_atexit: calls the
 * destructor that was handed to atexit_dtor_register when it is non-NULL.
 * (Ghidra could not recover the indirect jump as a table.) @ 0014c4d0
 */
void atexit_dtor_thunk(void *dtor)
{
    if (dtor != NULL)
        ((void (*)(void))dtor)();
}

/*
 * atexit_dtor_register — register atexit_dtor_thunk for arg with
 * __cxa_atexit, bound to the module DSO identity slot. @ 0014c4e4
 */
void atexit_dtor_register(void *arg)
{
    __cxa_atexit(atexit_dtor_thunk, arg, &g_dso_handle);
}

/*
 * atfork_handlers_register — register fork handlers via __register_atfork.
 * Ghidra could not recover the argument list (empty shim); the handler
 * slots are passed as NULL. @ 0014c500
 */
void atfork_handlers_register(void)
{
    __register_atfork(NULL, NULL, NULL, NULL);
}

/* ===== NexusOnlineProbe ===== */

/*
 * online_probe_poll — NexusOnlineProbe: rate-limited (1 Hz) JNI
 * CallStaticIntMethod poll through the captured JavaVM/class/method triple
 * (widgets.c), cached in g_probe_cache (0x1a7c00). Attaches a detached
 * thread, clears pending exceptions (result clamped to -1) and detaches
 * again. The probe state is logged at most once per 5 s; errno is
 * preserved. Returns the cached/fresh count (-1 when unavailable).
 * @ 0014cd5c
 */
int online_probe_poll(void)
{
    struct timespec ts;
    void *env = NULL;
    bool attached = false;
    bool poll_due;
    int result = -1;
    int rc;
    int saved_errno = errno;
    uint64_t now_ms = 0;

    if (g_probe_jvm != NULL && g_probe_class != NULL && g_probe_method != NULL) {
        if (clock_gettime(1 /* CLOCK_MONOTONIC */, &ts) == 0)
            now_ms = (uint64_t)ts.tv_sec * 1000u + (uint64_t)ts.tv_nsec / 1000000u;
        poll_due = g_probe_deadline_ms <= now_ms;
        if (!poll_due) {
            result = g_probe_cache;
            poll_due = 1000 < g_probe_deadline_ms - now_ms;
        }
        if (poll_due) {
            g_probe_deadline_ms = now_ms + 1000;
            if (0xfffffffffffffc17u < now_ms) /* within 1000 of wrap: saturate */
                g_probe_deadline_ms = 0xffffffffffffffffu;
            g_probe_cache = -1;
            rc = (int)jni_call2_ma(g_probe_jvm, 0x30 /* GetEnv */,
                                   (long)(void *)&env, 0x10006 /* JNI_VERSION_1_6 */);
            if (rc == 0) {
                attached = true;
            } else {
                result = -1;
                if (rc != -2 /* JNI_EDETACHED */)
                    goto log_tail;
                rc = (int)jni_call2_ma(g_probe_jvm, 0x20 /* AttachCurrentThread */,
                                       (long)(void *)&env, 0);
                if (rc != 0)
                    goto log_tail;
                attached = false;
            }
            result = (int)jni_call2_ma(env, 0x408 /* CallStaticIntMethod */,
                                       (long)g_probe_class, (long)g_probe_method);
            if (jni_call0_ma(env, 0x720 /* ExceptionCheck */) != 0) {
                jni_call0_ma(env, 0x88 /* ExceptionClear */);
                result = -1;
            }
            if (result < 0)
                result = -1;
            g_probe_cache = result;
            if (!attached)
                jni_call0_ma(g_probe_jvm, 0x28 /* DetachCurrentThread */);
        }
    }
log_tail:
    /* re-stamp and log at most once per 5 s (or when the clock jumps back) */
    if (clock_gettime(1 /* CLOCK_MONOTONIC */, &ts) == 0) {
        now_ms = (uint64_t)ts.tv_sec * 1000u + (uint64_t)ts.tv_nsec / 1000000u;
        if (g_probe_log_ms - 1 < now_ms && (now_ms - g_probe_log_ms) >> 3 < 0x271)
            goto out; /* logged within the last 5 s */
    } else {
        now_ms = 0;
    }
    g_probe_log_ms = now_ms;
    __android_log_print(4, "NexusOnlineProbe", "vm=%d class=%d method=%d count=%d",
                        g_probe_jvm != NULL, g_probe_class != NULL,
                        g_probe_method != NULL, result);
out:
    errno = saved_errno;
    return result;
}

/* ===== time utilities ===== */

/*
 * clock_ms — clock_gettime(clk) converted to milliseconds; returns 0 when
 * the clock read fails. @ 0014cfcc
 */
uint64_t clock_ms(clockid_t clk)
{
    struct timespec ts;

    if (clock_gettime(clk, &ts) != 0)
        return 0;
    return (uint64_t)ts.tv_sec * 1000u + (uint64_t)ts.tv_nsec / 1000000u;
}

/* ===== exported getters ===== */

/*
 * nexus_ui_performance_mode — exported getter for the battle-state word
 * (0x22d880): low half carries the current screen identity, bit 4 flags
 * in-battle. @ 0014d044
 */
uint64_t nexus_ui_performance_mode(void)
{
    return g_battle_state;
}

/* ===== code-region integrity ===== */

/*
 * code_region_hash_verify_thunk — four-byte arm64 veneer that tail-jumps
 * into code_region_hash_verify (the raw auto-generated name for this
 * veneer carried a banned token, so it was renamed). @ 0014d164
 */
int code_region_hash_verify_thunk(uint64_t offset, size_t len,
                                  const char *expect_hex)
{
    return code_region_hash_verify(offset, len, expect_hex);
}

/*
 * code_region_hash_verify — SHA-256 a libg code region and compare the hex
 * digest. offset is module-relative and gets g_proc_mem_bias (0x22d8e0)
 * added; the region is read through proc_mem_read in <= 0x100-byte chunks,
 * the 32-byte digest is rendered as 64 lowercase hex characters and
 * compared against expect_hex. Returns 1 on match, 0 on mismatch or read
 * failure. @ 0014d168
 */
int code_region_hash_verify(uint64_t offset, size_t len, const char *expect_hex)
{
    uint8_t state[112]; /* SHA-256 context (sha256_state_t layout) */
    uint8_t digest[32];
    uint8_t chunk[256];
    char hex[68]; /* 64 hex chars + NUL (68 reserved) */
    size_t done, n;
    int i;

    sha256_init((sha256_state_t *)state);
    for (done = 0; done < len; done += n) {
        n = len - done;
        if (n > 0x100)
            n = 0x100;
        if (!proc_mem_read(NULL /* ctx unused by proc_mem_read */,
                           offset + (uint64_t)g_proc_mem_bias + done, chunk, (uint32_t)n))
            return 0;
        sha256_update((sha256_state_t *)state, chunk, n);
    }
    sha256_final((sha256_state_t *)state, digest);
    for (i = 0; i < 32; i++)
        str_format(hex + 2 * i, (size_t)-1, 3, "%02x" /* rodata 0x139d9f */,
                   digest[i]);
    return strcmp(hex, expect_hex) == 0;
}

/* ===== protected channel ===== */

/*
 * protected_channel_acquire — snapshot the protected channel captured by
 * widgets.c (installed flag, jclass/jmethodID, ready word). Under the
 * 0x1a7c08 latch: when the captured function ids, flags and handle all
 * look valid, *out receives g_pchan_handle and 1 is returned; the latch is
 * always released. Returns 0 (with *out cleared) when anything is missing.
 * @ 0014d288
 */
int protected_channel_acquire(uint64_t *out)
{
    int ok = 0;

    if (out != NULL)
        *out = 0;
    if (g_channel_installed == 1 && g_channel_class != NULL &&
        g_channel_method != NULL && out != NULL && g_channel_ready != 0) {
        if ((ui_latch_test_and_set(1, &g_pchan_latch) & 1) == 0) {
            if (g_pchan_fn_b != 0 && g_pchan_fn_a != 0 && g_pchan_flag_a != 0 &&
                g_pchan_flag_c != 0 && g_pchan_handle != 0) {
                ok = 1;
                *out = g_pchan_handle;
            }
            g_pchan_latch = 0;
        }
    }
    return ok;
}

/*
 * protected_channel_validate — check that handle is the captured protected
 * channel: latch-protected re-validation of the 0x1a7c08 block (the handle
 * must match), then a Java-side CallStaticBooleanMethod(class, method,
 * handle) through the channel JavaVM, attaching the thread when detached.
 * The verdict only counts when no exception is pending; exceptions are
 * cleared either way. Returns 0 on any gate failure. @ 0014d350
 */
bool protected_channel_validate(uint64_t handle)
{
    void *env = NULL;
    bool attached = false;
    bool verdict = false;
    int rc;

    if (handle == 0)
        return false;
    if (g_channel_installed != 1 || g_channel_class == NULL ||
        g_channel_method == NULL)
        return false;
    if (g_channel_ready == 0)
        return false;
    if ((ui_latch_test_and_set(1, &g_pchan_latch) & 1) != 0)
        return false;
    g_pchan_latch = 0;
    if (g_pchan_fn_b == 0 || g_pchan_fn_a == 0 || g_pchan_flag_a == 0 ||
        g_pchan_flag_c == 0 || g_pchan_handle != handle)
        return false;
    if (g_channel_installed == 1 && g_channel_jvm != NULL &&
        g_channel_arg_ref != NULL) {
        rc = (int)jni_call2_ma(g_channel_jvm, 0x30 /* GetEnv */,
                               (long)(void *)&env, 0x10006 /* JNI_VERSION_1_6 */);
        if (rc == 0) {
            attached = true;
        } else if (rc == -2 /* JNI_EDETACHED */) {
            rc = (int)jni_call2_ma(g_channel_jvm, 0x20 /* AttachCurrentThread */,
                                   (long)(void *)&env, 0);
            if (rc != 0)
                return false;
            attached = false;
        } else {
            return false;
        }
        if (env != NULL) {
            long v = jni_call3_ma(env, 0x3a8 /* CallStaticBooleanMethod */,
                                  (long)g_channel_class, (long)g_channel_method,
                                  (long)handle);
            if (v == 1)
                verdict = jni_call0_ma(env, 0x720 /* ExceptionCheck */) == 0;
            if (jni_call0_ma(env, 0x720 /* ExceptionCheck */) != 0)
                jni_call0_ma(env, 0x88 /* ExceptionClear */);
            if (!attached)
                jni_call0_ma(g_channel_jvm, 0x28 /* DetachCurrentThread */);
        }
    }
    return verdict;
}
/* misc_a chain chunk 2: covers raw lines 407-900 */

/* ===== rodata: end-screen label tables ===== */
extern const char g_end_label_again_b[];  /* 0x1347da — class-1 alias */
extern const char g_end_label_again_c[];  /* 0x13a2b0 — class-1 alias */
extern const char g_end_label_again_d[];  /* 0x138299 — class-1 alias */
extern const char g_end_label_again_e[];  /* 0x13937f — class-1 alias */
extern const char g_end_label_class2_a[]; /* 0x13b993 — class-2 label */
extern const char g_end_label_class2_b[]; /* 0x1376bf — class-2 label */
extern const char g_end_label_victory_c[];/* 0x137b79 — victory alias */
extern const char g_end_label_defeat_c[]; /* 0x13a8fa — defeat alias */
extern const char g_end_label_class5_a[]; /* 0x13bf80 — class-5 label */
extern const char g_end_label_class5_b[]; /* 0x138dba — class-5 label */
extern const char g_rank_prefix_4[];      /* 0x13bfa4 — 4-char rank prefix */
extern const char g_rank_prefix_10[];     /* 0x131dae — 10-char rank prefix */
extern const uint64_t OUTCOME_TALLY_IDS;  /* 0x10f7f8 — {lo,hi} outcome ids
                                             * compared as 32-bit lanes by the
                                             * tally counters */

/* ===== results-screen tracker types =====
 * The tracker state block lives at 0x22d8f0 (0x1e8 bytes). The screen
 * record is the 7-word snapshot produced by the screen scan (screen_scan_
 * pump); the captured records mirror the layout the archive output uses. */

typedef struct {
    uint64_t stamp;                /* +0x00 screen stamp (frame counter) */
    uint64_t identity;             /* +0x08 screen identity object */
    uint64_t node;                 /* +0x10 screen node object */
    uint64_t flag;                 /* +0x18 pending-event flag (must be 0 to archive) */
    int32_t  count_a;              /* +0x20 primary count */
    int32_t  count_a_hi;           /* +0x24 nonzero = primary count valid */
    int32_t  count_b;              /* +0x28 secondary count */
    int32_t  count_b_hi;           /* +0x2c */
    uint32_t close_window_again;   /* +0x30 min age of the play-again capture,
                                    * clamped to [1000, 10000] */
    uint32_t close_window_refresh; /* +0x34 min age of the refresh capture,
                                    * clamped to [500, 5000] */
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
    uint64_t close_stamp;          /* +0x0e0 screen-close base time (0x7531 window) */
    uint64_t first_seen;           /* +0x0e8 first event stamp on this screen */
    uint64_t outcome_stamp;        /* +0x0f0 stamp of the last outcome change */
    uint64_t delta_first;          /* +0x0f8 first delta-note stamp */
    uint64_t refresh_base;         /* +0x100 staleness base for the refresh capture */
    uint64_t again_anchor;         /* +0x108 play-again anchor stamp */
    uint64_t refresh_anchor;       /* +0x110 refresh anchor stamp */
    uint64_t last_seen;            /* +0x118 last event stamp */
    uint64_t outcome_last;         /* +0x120 last outcome-class stamp */
    uint64_t delta_last;           /* +0x128 last delta-note stamp */
    uint64_t screen_changes;       /* +0x130 identity-change counter */
    uint64_t tally_count_hi;       /* +0x138 net count of outcome ==
                                    * OUTCOME_TALLY_IDS high dword (lane 1) */
    uint64_t tally_count_lo;       /* +0x140 net count of outcome ==
                                    * OUTCOME_TALLY_IDS low dword (lane 0) */
    uint64_t tally_count_two;      /* +0x148 net count of outcome == 2 */
    uint64_t tally_identity;       /* +0x150 identity the tallies belong to */
    uint64_t delta_sum;            /* +0x158 applied delta sum */
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

_Static_assert(sizeof(screen_record_t) == 0x38, "screen record layout");
_Static_assert(sizeof(tracker_record_t) == 0x50, "tracker record layout");
_Static_assert(offsetof(results_tracker_t, again) == 0x38, "tracker layout");
_Static_assert(offsetof(results_tracker_t, refresh) == 0x88, "tracker layout");
_Static_assert(offsetof(results_tracker_t, last_event_node) == 0xd8, "tracker layout");
_Static_assert(offsetof(results_tracker_t, outcome) == 0x160, "tracker layout");
_Static_assert(offsetof(results_tracker_t, refresh_active) == 0x188, "tracker layout");
_Static_assert(offsetof(results_tracker_t, adopted) == 0x194, "tracker layout");
_Static_assert(offsetof(results_tracker_t, archive) == 0x198, "tracker layout");
_Static_assert(sizeof(results_tracker_t) == 0x1e8, "tracker layout");

results_tracker_t g_results_tracker;  /* 0x22d8f0 — results-screen tracker state */

/* whitespace masks used by the text classifiers (bit per char code) */
#define WS_MASK       0x100002600UL   /* {9 tab, 10 LF, 13 CR, 32 space} */
#define WS_TAIL_MASK  0x300002600UL   /* {9, 10, 13, 32, 33 '!'} */
#define SIGN_MASK     0x280000000000UL /* {43 '+', 45 '-'} */
#define RANK_SEP_MASK 0x400000100000200UL /* {9 tab, 32 space, 58 ':'} */

/* forward declarations within this chain */
int text_casefold_equals(const char *a, const char *b);  /* @ 0014d7d8 */
int rank_number_parse(const char *text);                 /* @ 0014d990 */

/* ===== end-screen text classification ===== */

/*
 * end_text_classify — classify an end-screen label. text must be <= 0x9f
 * bytes. Returns the class: 1 play-again, 2 refresh label, 3 victory,
 * 4 defeat, 5 other end label, 6 numeric trophy delta (value in *value_out,
 * signed, magnitude <= 30), 7 rank/placement number (value in *value_out),
 * 0 unknown. @ 0014d544
 */
int end_text_classify(const char *text, int32_t *value_out)
{
    const unsigned char *q;
    unsigned char sign, c;
    int acc, rank;

    if (text == NULL || value_out == NULL)
        return 0;
    if (strlen(text) > 0x9f)
        return 0;
    if (text_casefold_equals(text, "PLAY AGAIN") ||
        text_casefold_equals(text, g_end_label_again_b) ||
        text_casefold_equals(text, g_end_label_again_c) ||
        text_casefold_equals(text, g_end_label_again_d) ||
        text_casefold_equals(text, g_end_label_again_e))
        return 1;
    if (text_casefold_equals(text, g_end_label_class2_a) ||
        text_casefold_equals(text, g_end_label_class2_b))
        return 2;
    if (text_casefold_equals(text, "VICTORY") ||
        text_casefold_equals(text, "YOU WIN") ||
        text_casefold_equals(text, g_end_label_victory_c))
        return 3;
    if (text_casefold_equals(text, "DEFEAT") ||
        text_casefold_equals(text, "YOU LOSE") ||
        text_casefold_equals(text, g_end_label_defeat_c))
        return 4;
    if (text_casefold_equals(text, g_end_label_class5_a) ||
        text_casefold_equals(text, g_end_label_class5_b))
        return 5;
    rank = rank_number_parse(text);
    if (rank != 0) {
        *value_out = rank;
        return 7;
    }

    /* numeric delta: [+-] <spaces/tabs> <digits> <trailing ws> NUL */
    q = (const unsigned char *)text + 2;
    for (;;) {
        sign = q[-2];
        if (0x2d < sign)
            return 0;               /* above '-': neither ws nor a sign */
        if (((1UL << (sign & 0x3f)) & WS_MASK) == 0)
            break;                  /* first non-ws char: the sign */
        q++;
    }
    if (((1UL << (sign & 0x3f)) & SIGN_MASK) == 0)
        return 0;                   /* sign must be '+' (43) or '-' (45) */
    for (c = q[-1]; c == 0x20 || c == 9; c = *(++q - 1))
        ;                           /* skip spaces/tabs after the sign */
    if (c >= 0x30 && c <= 0x39) {   /* 0xfffffff5 < c - 0x3a */
        acc = 0;
        for (;;) {
            unsigned raw;

            c = q[-1];
            if (c - 0x30 > 9)
                break;
            raw = c + (unsigned)acc * 10;
            q++;
            acc = (int)raw - 0x30;
            if (0x4e < raw)         /* magnitude cap: about 30 */
                return 0;
        }
        c = q[-1];
        if (c < 0x21) {
            for (;;) {
                if (((1UL << (c & 0x3f)) & WS_MASK) == 0) {
                    if (c != 0)
                        return 0;
                    *value_out = (sign == 0x2d) ? -acc : acc;
                    return 6;
                }
                c = *q;
                q++;
                if (0x21 <= c)
                    return 0;
            }
        }
    }
    return 0;
}

/* ===== case-folded text comparison ===== */

/*
 * fold_next — consume one character from *pp (ASCII or 2-byte UTF-8
 * Cyrillic) and return its case-folded code point: a-z folds to A-Z,
 * U+0430..U+044F folds to U+0410..U+042F and U+0451 (ё) to U+0401 (Ё).
 * Advances *pp past the consumed bytes. @ helper for 0014d7d8
 */
static uint32_t fold_next(const unsigned char **pp)
{
    const unsigned char *p = *pp;
    uint32_t c = *p;

    if (c - 0x61 < 0x1a) {
        c = c - 0x20;               /* ASCII a-z -> A-Z */
    } else if ((c & 0xfe) == 0xd0 && (p[1] & 0xc0) == 0x80) {
        p += 2;                     /* 2-byte UTF-8: U+0400..U+07FF */
        c = (uint32_t)(p[1] & 0x3f) | ((uint32_t)(p[-2] & 0x1f) << 6);
        if (c - 0x430 < 0x20)
            c = c - 0x20;           /* а-я -> А-Я */
        else if (c == 0x451)
            c = 0x401;              /* ё -> Ё */
    }
    *pp = p + 1;
    return c;
}

/*
 * text_casefold_equals — trimmed case-insensitive equality. Leading
 * whitespace (tab/LF/CR/space) and trailing whitespace+'!' are ignored;
 * comparison folds ASCII and Cyrillic case. Returns 1 when the trimmed
 * strings are equal, 0 otherwise (NULL a returns 0). @ 0014d7d8
 */
int text_casefold_equals(const char *a, const char *b)
{
    const unsigned char *pa = (const unsigned char *)a;
    const unsigned char *pb = (const unsigned char *)b;
    size_t la, lb;

    if (a == NULL)
        return 0;
    while (*pa < 0x21 && ((1UL << (*pa & 0x3f)) & WS_MASK) != 0)
        pa++;
    la = strlen((const char *)pa);
    while (la != 0 && pa[la - 1] < 0x22 &&
           ((1UL << (pa[la - 1] & 0x3f)) & WS_TAIL_MASK) != 0)
        la--;
    lb = strlen(b);
    if (la != lb)
        return 0;
    for (;;) {
        uint32_t ca, cb;

        if ((size_t)(pa - (const unsigned char *)a) >= la)
            return *pb == 0;
        ca = fold_next(&pa);
        cb = fold_next(&pb);
        if (ca != cb)
            return 0;
    }
}

/* ===== rank/placement parsing ===== */

/*
 * rank_number_parse — parse a placement number: either a known 4-char or
 * 10-char prefix (followed by tab/space/':'), or a leading '#', then an
 * optional ':' with surrounding blanks, then 1-9 as the first digit and at
 * most a two-digit number (raw sum cap 0x3b, so 1..10). Trailing garbage
 * other than whitespace/'!' rejects the parse. Returns the rank, or 0.
 * @ 0014d990
 */
int rank_number_parse(const char *text)
{
    const char *p = text;
    const char *num = NULL;
    size_t len;

    while ((unsigned char)*p < 0x21 &&
           ((1UL << ((unsigned char)*p & 0x3f)) & WS_MASK) != 0)
        p++;
    len = strlen(p);
    if (len >= 4) {
        char w4[8];

        memcpy(w4, p, 4);
        w4[4] = 0;                  /* 5-byte window over the first 4 chars */
        if (text_casefold_equals(w4, g_rank_prefix_4) &&
            ((1UL << ((unsigned char)p[4] & 0x3f)) & RANK_SEP_MASK) != 0 &&
            (unsigned char)p[4] <= 0x3a) {
            num = p + 4;
            goto parse;
        }
    }
    len = strlen(p);
    if (len > 9) {
        char w10[11];

        memcpy(w10, p, 10);
        w10[10] = 0;                /* 11-byte window over the first 10 chars */
        if (text_casefold_equals(w10, g_rank_prefix_10) &&
            ((1UL << ((unsigned char)p[10] & 0x3f)) & RANK_SEP_MASK) != 0) {
            num = p + 10;
            goto parse;
        }
    }
    if ((unsigned char)*p == 0x23) {  /* '#' */
        num = p + 1;
        goto parse;
    }
    return 0;

parse:
    while (*num == 9 || *num == 0x20)
        num++;
    if (*num == 0x3a)
        num++;
    while (*num == 0x20 || *num == 9)
        num++;
    if ((unsigned char)*num >= 0x31 && (unsigned char)*num <= 0x39) {
        /* 0xfffffff6 < c - 0x3a: first digit is 1..9 */
        int acc = 0;
        unsigned char c;

        for (;;) {
            unsigned raw;

            c = (unsigned char)*num;
            if (c - 0x30 > 9)
                break;
            raw = c + (unsigned)acc * 10;
            num++;
            acc = (int)raw - 0x30;
            if (raw >= 0x3b)
                return 0;           /* cap: only 1..10 fit */
        }
        for (;;) {
            if (c > 0x21)
                return 0;           /* printable garbage after the number */
            if (((1UL << (c & 0x3f)) & WS_TAIL_MASK) == 0)
                return c == 0 ? acc : 0;
            c = (unsigned char)*++num;
        }
    }
    return 0;
}

/* ===== results-screen tracker ===== */

/*
 * results_tracker_screen_reset — adopt a new result-screen identity. When
 * the tracker is archived it is left alone. A record with no counts clears
 * the whole window (adopted = 0); a record with counts clears the window
 * only when the tracker was not adopted yet or the identity changed, and
 * seeds last_event_node/close_stamp from the record. The 7-word screen
 * identity is always copied in. @ 0014db9c
 */
void results_tracker_screen_reset(results_tracker_t *trk,
                                  const screen_record_t *rec)
{
    int mode;

    if (trk == NULL || rec == NULL || trk->archived != 0)
        return;
    if (rec->count_a == 0 || (rec->count_a_hi == 0 && rec->count_b == 0)) {
        mode = 0;
        goto clear;
    }
    if (trk->adopted == 0 || rec->stamp < trk->rec.stamp ||
        rec->node != trk->rec.node) {
        mode = 1;
        goto clear;
    }
    mode = 1;
    if (rec->identity != trk->rec.identity)
        goto clear;
    trk->adopted = 1;
    goto seed;

clear:
    trk->outcome_stamp = 0;               /* +0xf0 */
    trk->first_seen = 0;                  /* +0xe8 */
    memset(&trk->archive, 0, sizeof trk->archive); /* +0x198..+0x1e8 */
    trk->rank_value = 0;                  /* +0x168 */
    trk->outcome = 0;                     /* +0x160 */
    trk->note_latch = 0;                  /* +0x17c */
    trk->delta_latch = 0;                 /* +0x174 */
    trk->refresh_base = 0;                /* +0x100 */
    trk->delta_first = 0;                 /* +0xf8 */
    trk->refresh_anchor = 0;              /* +0x110 */
    trk->again_anchor = 0;                /* +0x108 */
    trk->outcome_last = 0;                /* +0x120 */
    trk->last_seen = 0;                   /* +0x118 */
    trk->delta_last = 0;                  /* +0x128 */
    trk->tally_active = 0;                /* +0x184 (8-byte clear with 0x188) */
    trk->refresh_active = 0;              /* +0x188 */
    trk->refresh_mode = 0;                /* +0x18c (8-byte clear with 0x190) */
    trk->archived = 0;                    /* +0x190 */
    trk->last_event_node = 0;             /* +0xd8 */
    /* one 0x98-byte clear over +0x40..+0xd8 (again.word1 .. refresh.flag);
     * again.subject (+0x38) is intentionally left intact (dump behavior) */
    memset((char *)trk + 0x40, 0, 0x98);
    trk->adopted = mode;
    if (mode == 0)
        goto adopt;

seed:
    if (rec->flag != 0 && rec->identity != 0) {
        trk->last_event_node = rec->identity;
        trk->close_stamp = rec->stamp;
    }
adopt:
    trk->rec = *rec;
}

/*
 * results_tracker_text_event — feed one classified end-screen text event
 * (from the screen record rec, text, and the captured event record ev)
 * into the tracker. Requires the tracker to be adopted, not archived, and
 * the screen identity to match. Class 2 with a valid primary count and no
 * pending flag captures the refresh record (mode 2), dropping a stale
 * play-again capture; class 1 (from the tracked node) captures the
 * play-again record (mode 1); class 6 records the delta value; class 7 the
 * rank; classes 3/4/5 record the outcome (1 win, -1 loss, 2 other) with a
 * 0x7531 close-stamp window. @ 0014dca4
 */
void results_tracker_text_event(results_tracker_t *trk,
                                const screen_record_t *rec,
                                const char *text, const tracker_record_t *ev)
{
    int value = 0;
    int klass;
    uint64_t stamp;

    if (trk == NULL || rec == NULL)
        return;
    if (trk->adopted == 0 || trk->archived != 0)
        return;
    if (rec->identity != trk->rec.identity)
        return;
    klass = end_text_classify(text, &value);

    if (ev != NULL && klass == 2 && rec->count_a_hi != 0 && rec->flag == 0) {
        /* refresh-mode capture */
        if (trk->refresh_active != 0 && trk->refresh_mode == 1) {
            if (trk->refresh_base - 1 < rec->stamp &&
                0x270 < (rec->stamp - trk->refresh_base) >> 3) {
                /* play-again capture went stale (0x270 * 8 frames): drop it */
                trk->refresh_active = 0;
                trk->first_seen = 0;
                /* clear subject..stamp (0x40 bytes); mode/flag stay */
                memset(&trk->again, 0, 0x40);
            }
        }
        if (trk->refresh_anchor == 0 || trk->refresh.subject != ev->subject ||
            trk->refresh.word5 != ev->word5)
            trk->refresh_anchor = rec->stamp;
        trk->refresh = *ev;
        trk->refresh.identity = rec->identity;
        trk->refresh.stamp = rec->stamp;
        trk->refresh.mode = 2;
        return;
    }

    if (trk->last_event_node == 0 || trk->last_event_node != rec->identity)
        return;
    if (ev != NULL && klass == 1) {
        /* play-again capture */
        stamp = rec->stamp;
        if (trk->first_seen == 0)
            trk->first_seen = stamp;
        trk->last_seen = stamp;
        if (trk->again_anchor == 0 || trk->again.subject != ev->subject ||
            trk->refresh.word5 != ev->word5) /* dump checks 0xb0 (refresh) here */
            trk->again_anchor = stamp;
        trk->again = *ev;
        trk->again.identity = rec->identity;
        trk->again.stamp = stamp;
        trk->again.mode = 1;
        return;
    }

    if (2 < (uint32_t)(klass - 3)) {
        if (klass == 6) {
            /* trophy delta */
            if (trk->note_latch == 0) {
                stamp = rec->stamp;
                if (trk->delta_latch == 0 || trk->delta_value != value) {
                    trk->delta_value = value;
                    trk->delta_first = stamp;
                }
                trk->delta_last = stamp;
                trk->delta_latch = 1;
            }
            return;
        }
        if (klass != 7)
            return;
        /* rank/placement */
        stamp = rec->stamp;
        if (trk->first_seen == 0) {
            if (trk->close_stamp - 1 < stamp &&
                stamp - trk->close_stamp < 0x7531) {
                trk->first_seen = stamp;
                trk->last_seen = stamp;
            }
        } else {
            trk->last_seen = stamp;
        }
        trk->rank_value = value;
        return;
    }

    /* outcome classes 3/4/5 -> 1 win, -1 loss, 2 other */
    {
        int outcome = 2;

        if (klass == 4)
            outcome = -1;
        if (klass == 3)
            outcome = 1;
        stamp = rec->stamp;
        if (trk->first_seen == 0) {
            if (trk->close_stamp - 1 < stamp &&
                stamp - trk->close_stamp < 0x7531) {
                trk->first_seen = stamp;
                trk->last_seen = stamp;
            }
        } else {
            trk->last_seen = stamp;
        }
        trk->outcome_last = stamp;
        if (trk->outcome != outcome) {
            trk->outcome = outcome;
            trk->outcome_stamp = stamp;
        }
    }
}
/* misc_a chain chunk 3: covers raw lines 901-1400 */

#include <pthread.h>
#include <sys/uio.h>

/* ===== forward declarations within this chain ===== */
void results_tracker_tally_update(results_tracker_t *trk,
                                  const screen_record_t *rec);        /* below @ 0014e330 */
int  proc_mem_write64(void *ctx, uint64_t addr, const void *src,
                      uint32_t len);                                  /* @ 0014ebe8 */
struct remote_accessor;
int  sc_object_free_check(struct remote_accessor *acc, uint64_t obj); /* @ 00154b50 */

/* ===== MainPool / remote-memory globals ===== */
extern int32_t g_main_pool_tid;   /* 0x22f038 — MainPool (main game) thread id */
uint8_t g_postbattle_installed;   /* 0x22d8d8 — 1 = postbattle islands installed */
uint32_t g_host_channel_fd = 0x7fff0001; /* 0x1a7c38 — remote-memory channel fd;
                                          * 0x7fff0001 is the "self" marker:
                                          * read own process via process_vm_readv */

/* ===== results-screen tracker (continued) ===== */

/*
 * results_tracker_delta_note — external trophy-delta note: feed a numeric
 * delta value with the screen record it applies to. Gates like a text
 * event (adopted, not archived, identity matches, tracked node live).
 * Records the value with first/last stamps unless an external note was
 * already taken this screen. @ 0014dfa8
 */
void results_tracker_delta_note(results_tracker_t *trk,
                                const screen_record_t *rec, int32_t value)
{
    uint64_t stamp;

    if (trk == NULL || rec == NULL)
        return;
    if (trk->adopted == 0 || trk->archived != 0)
        return;
    if (rec->identity != trk->rec.identity)
        return;
    if (trk->last_event_node == 0 || trk->last_event_node != rec->identity)
        return;
    stamp = rec->stamp;
    if (trk->first_seen == 0)
        trk->first_seen = stamp;
    trk->last_seen = stamp;
    if (trk->note_latch == 0 || trk->delta_latch == 0 ||
        trk->delta_value != value)
        trk->delta_first = stamp;
    trk->delta_value = value;
    trk->delta_last = stamp;
    trk->note_latch = 1;
    trk->delta_latch = 1;
}

/*
 * results_tracker_screen_archive — validate the screen-close time windows
 * and archive the last-screen capture into *out. Requires the tracker
 * adopted and unarchived, counts on the record, matching node/identity and
 * a fresh-enough stamp; the tallies are updated first (side effect even on
 * failure). The play-again capture is preferred (subject live, capture
 * within 15000 of the close, anchored >= the [1000,10000] clamp window and
 * consistent with the delta notes); otherwise the refresh capture is used
 * (within 15000, anchored >= 1000). The chosen capture must still name the
 * current screen node/identity. Returns 1 on archive (also stored in
 * trk->archive, refresh_base re-stamped), 0 otherwise. @ 0014e030
 */
int results_tracker_screen_archive(results_tracker_t *trk,
                                   const screen_record_t *rec,
                                   tracker_record_t *out)
{
    tracker_record_t cap;
    uint32_t window;
    uint64_t stamp;

    if (trk == NULL || rec == NULL || out == NULL)
        return 0;
    if (trk->adopted == 0 || trk->archived != 0)
        return 0;
    if (rec->count_a == 0 || rec->count_b_hi != 0)
        return 0;
    if (rec->count_a_hi == 0 && rec->count_b == 0)
        return 0;
    if (rec->node != trk->rec.node || rec->identity != trk->rec.identity ||
        rec->stamp < trk->rec.stamp)
        return 0;
    results_tracker_tally_update(trk, rec);  /* side effect before the windows */
    if (trk->refresh_active != 0)
        return 0;

    /* staleness gate on the refresh-capture base */
    if (trk->refresh_base != 0) {
        window = rec->close_window_refresh;
        if (4999 < window)
            window = 5000;
        if (window < 0x1f5)
            window = 500;
        if (rec->stamp <= trk->refresh_base - 1 ||
            rec->stamp - trk->refresh_base < window)
            return 0;
    }
    /* min age of the play-again capture */
    window = rec->close_window_again;
    if (9999 < window)
        window = 10000;
    if (window < 1000)
        window = 1000;

    if (trk->again.subject != 0 && trk->last_event_node == rec->identity) {
        /* play-again capture path */
        stamp = rec->stamp;
        if (stamp <= trk->again.stamp - 1 || 15000 < stamp - trk->again.stamp ||
            stamp <= trk->again_anchor - 1 ||
            stamp - trk->again_anchor < window)
            goto refresh_path;
        if ((stamp - trk->again_anchor) >> 4 < 0x271) {
            if (trk->outcome == 0) {
                if (trk->delta_latch == 0)
                    return 0;
            } else if (trk->delta_latch == 0) {
                goto take_again;   /* no delta notes: skip their windows */
            }
            if (stamp <= trk->delta_last - 1 || 15000 < stamp - trk->delta_last)
                return 0;
            if (stamp - trk->delta_first < 0x2ee)
                return 0;
        }
take_again:
        cap = trk->again;
        goto commit;
    }

refresh_path:
    if (rec->count_a_hi == 0 || rec->flag != 0 || trk->refresh.subject == 0)
        return 0;
    stamp = rec->stamp;
    if (stamp <= trk->refresh.stamp - 1 || 15000 < stamp - trk->refresh.stamp)
        return 0;
    if (stamp <= trk->refresh_anchor - 1 ||
        stamp - trk->refresh_anchor < 1000)
        return 0;
    cap = trk->refresh;

commit:
    if (cap.word1 != rec->node || cap.identity != rec->identity)
        return 0;
    trk->archive = cap;
    trk->archived = 1;
    trk->refresh_base = rec->stamp;
    *out = cap;
    return 1;
}

/*
 * results_tracker_tally_update — update the win/loss/streak tallies for
 * the observed screen. Requires a known identity, a stamp within [last_
 * seen, last_seen+15000], first_seen set and >= 0x2ee ago; an identity
 * change resets the per-screen tallies (and bumps screen_changes). A
 * pending outcome (or a fresh outcome, 0x3a99 window and > 0x2ed past its
 * stamp) is counted into the OUTCOME_TALLY_IDS lane counters and the
 * "== 2" streak counter on every transition. A contradictory delta (gain
 * on a loss, loss on a win) rolls the applied delta sum back; a fresh
 * delta note (within 0x3a99 of delta_last, > 0x2ed past delta_first) is
 * applied into delta_sum. @ 0014e330
 */
void results_tracker_tally_update(results_tracker_t *trk,
                                  const screen_record_t *rec)
{
    int32_t id0 = (int32_t)(OUTCOME_TALLY_IDS & 0xffffffffu);         /* lane 0 */
    int32_t id1 = (int32_t)((OUTCOME_TALLY_IDS >> 32) & 0xffffffffu); /* lane 1 */
    uint64_t stamp;
    uint64_t delta_fresh = 0;
    int outcome, prev;

    if (rec->identity == 0)
        return;
    if (rec->identity < trk->tally_identity)
        return;
    stamp = rec->stamp;
    if (stamp < trk->last_seen)
        return;
    if (15000 < stamp - trk->last_seen)
        return;
    if (trk->first_seen == 0)
        return;
    if (stamp - trk->first_seen < 0x2ee)
        return;
    if (trk->tally_identity != rec->identity) {
        trk->tally_identity = rec->identity;
        trk->outcome_pending = 0;
        trk->delta_applied = 0;
        trk->pending_valid = 0;
        trk->tally_active = 0;
        trk->screen_changes++;
    }
    trk->tally_active = 1;

    /* is there a fresh (non-stale) delta note for this screen? */
    if (trk->delta_latch != 0 && trk->delta_last - 1 < stamp &&
        stamp - trk->delta_last < 0x3a99 && trk->delta_first != 0)
        delta_fresh = 0x2ed < stamp - trk->delta_first;

    outcome = trk->outcome;
    if (outcome == 0) {
        outcome = trk->pending_valid ? trk->outcome_pending : 0;
    } else if (trk->outcome_last - 1 < stamp &&
               stamp - trk->outcome_last < 0x3a99 &&
               trk->outcome_stamp != 0 && 0x2ed < stamp - trk->outcome_stamp) {
        prev = trk->outcome_pending;
        if (outcome != prev) {
            /* lane counters (NEON cmeq on 32-bit lanes in the dump) */
            trk->tally_count_two -= (prev == 2);
            trk->outcome_pending = outcome;
            if (outcome == 2)
                trk->tally_count_two++;
            trk->tally_count_hi += (prev == id1) + (outcome == id1);
            trk->tally_count_lo += (prev == id0) + (outcome == id0);
        }
        trk->pending_valid = 1;
    }

    if (trk->note_latch == 0) {
        /* contradictory delta vs outcome: roll the applied delta back */
        if ((outcome == -1 && 0 < trk->delta_value) ||
            (outcome == 1 && trk->delta_value < 0)) {
            if (trk->delta_applied == 0)
                return;
            trk->delta_sum -= trk->delta_applied_value;
            trk->delta_applied = 0;
            trk->delta_applied_value = 0;
            return;
        }
    }
    if (delta_fresh != 0) {
        int value = trk->delta_value;
        int64_t prev_applied;

        if (trk->delta_applied == 0) {
            prev_applied = 0;
        } else {
            prev_applied = trk->delta_applied_value;
            if (value == trk->delta_applied_value)
                return;           /* already applied this value */
        }
        trk->delta_applied_value = value;
        trk->delta_sum += value - prev_applied;
        trk->delta_applied = 1;
    }
}

/*
 * results_tracker_window_reset — after an archive: clear the archived flag
 * and the stored capture; when rearm is nonzero the refresh capture is
 * re-armed (refresh_active = 1) with the archived record's mode.
 * @ 0014e548
 */
void results_tracker_window_reset(results_tracker_t *trk, int rearm)
{
    if (trk == NULL || trk->archived == 0)
        return;
    trk->archived = 0;
    if (rearm != 0) {
        trk->refresh_active = 1;
        trk->refresh_mode = trk->archive.mode;
    }
    memset(&trk->archive, 0, sizeof trk->archive);
}

/*
 * results_tracker_widget_seen — mark the refresh capture live again when
 * one of the tracked capture subjects (play-again or refresh widget) is
 * seen again. @ 0014e584
 */
void results_tracker_widget_seen(results_tracker_t *trk, uint64_t node)
{
    if (trk == NULL)
        return;
    if (trk->again.subject == node || trk->refresh.subject == node)
        trk->refresh_active = 1;
}

/* ===== remote SC-object access ===== */

/*
 * remote_accessor_t — the 4-word bundle the SC-object checks operate on:
 * {module base, context, read fn, write64 fn}. Built on the stack by
 * callers; sc_object_free_check (later in this chain) consumes it.
 */
typedef struct remote_accessor {
    uint64_t base;     /* +0x00 module base (g_proc_mem_bias), must be >= 0x1000 */
    void    *ctx;      /* +0x08 context handed to the read/write fns (0) */
    int    (*read)(void *ctx, uint64_t addr, void *out, uint32_t len);          /* +0x10 */
    int    (*write64)(void *ctx, uint64_t addr, const void *src, uint32_t len); /* +0x18 */
} remote_accessor_t;

/*
 * sc_object_release — MainPool-only release for a remote SC object: after
 * validating the node with sc_object_free_check, the +0x218 state word is
 * read; when nonzero it is zeroed with a remote 8-byte write and re-read,
 * then the node is validated again. Returns 1 when the object ends up
 * free and unclaimed, 0 otherwise. @ 0014e5ac
 */
int sc_object_release(uint64_t obj)
{
    remote_accessor_t acc;
    char name[16];
    uint64_t state, zero = 0;

    if (g_postbattle_installed != 1)
        return 0;
    if (!(g_main_pool_tid > 0 && gettid() == g_main_pool_tid))
        return 0;
    if (pthread_getname_np(pthread_self(), name, 0x10) != 0)
        return 0;
    if (memcmp(name, "MainPool", 9) != 0)  /* 0x706f6f6c6e69614d + NUL */
        return 0;

    acc.base = (uint64_t)g_proc_mem_bias;
    acc.ctx = NULL;
    acc.read = proc_mem_read;
    acc.write64 = proc_mem_write64;
    if (!sc_object_free_check(&acc, obj))
        return 0;

    if (!acc.read(acc.ctx, obj + 0x218, &state, 8))
        return 0;
    if (state != 0) {
        if (!acc.write64(acc.ctx, obj + 0x218, &zero, 8))
            return 0;
    }
    if (!acc.read(acc.ctx, obj + 0x218, &state, 8))
        return 0;
    if (state == 0)
        return sc_object_free_check(&acc, obj) != 0;
    return 0;
}

/*
 * on_main_pool_thread — true when the current thread is the game's
 * MainPool thread: known tid (g_main_pool_tid) and thread name
 * "MainPool". @ 0014e720
 */
bool on_main_pool_thread(void)
{
    char name[16] = {0};

    if (!(g_main_pool_tid > 0 && gettid() == g_main_pool_tid))
        return false;
    if (pthread_getname_np(pthread_self(), name, 0x10) != 0)
        return false;
    return memcmp(name, "MainPool", 9) == 0;  /* 0x706f6f6c6e69614d */
}

/* ===== remote process-memory primitives ===== */

/*
 * proc_mem_read — read up to 0x1000 bytes of (own-process) game memory.
 * ctx is a dummy (callers pass anything, including the SHA-256 state).
 * Gates: nonzero addr, channel fd >= 0, no 64-bit wrap of addr+len, len
 * <= 0x1000. Reads via pread64 on the channel fd, or process_vm_readv on
 * own pid when the fd is the 0x7fff0001 self marker; interrupted reads
 * are retried (8 attempts total). errno is preserved on success.
 * Returns 1 when exactly len bytes were read, 0 otherwise. @ 0014e7c4
 */
int proc_mem_read(void *ctx, uint64_t addr, void *out, uint32_t len)
{
    uint32_t fd = g_host_channel_fd;
    long n;
    int saved_errno = errno;
    int attempts = 8;  /* initial attempt + 7 EINTR retries */

    (void)ctx;
    if (addr == 0 || (int32_t)fd < 0)
        return 0;
    if (addr + (uint64_t)len < addr)  /* CARRY8(addr, len): wrap */
        return 0;
    if (len > 0x1000)
        return 0;

    /* the dump routes this through a fortified TLS stream cookie that only
     * carries the result and errno — dropped here */
    for (;;) {
        if (fd != 0x7fff0001) {
            n = syscall(0x43 /* arm64 pread64 */, (uint64_t)fd, out,
                        (uint64_t)len, addr);
        } else {
            struct iovec local_iov = { out, len };
            struct iovec remote_iov = { (void *)(uintptr_t)addr, len };

            n = syscall(0x10e /* process_vm_readv */, (long)getpid(),
                        &local_iov, 1UL, &remote_iov, 1UL, 0UL);
        }
        if (n >= 0)
            break;
        if (errno != EINTR || --attempts == 0)
            break;
    }
    if (n >= 0) {
        errno = saved_errno;
        return n == (long)len;
    }
    return 0;
}
/* misc_a chain chunk 4: covers raw lines 1517-2000 */

/* ===== forward declarations within this chain ===== */
int  postbattle_island_verify(void);                                  /* below @ 0014fac4 */
uint64_t button_lineage_check(uint64_t node, uint64_t screen);        /* @ 00154eb4 */
int  remote_child_link_valid(uint64_t node, uint64_t parent);         /* @ 00154d5c */
void protected_plus_resolve_once(void);                               /* @ 001552c0 */

/* ===== cross-file functions ===== */
extern int  ui_widget_pair_release(uint64_t parent, uint64_t *pair,
                                   uint32_t mode);          /* misc.c (b-chain) @ 00176ad4 */
extern uint64_t script_port_fast_replay_snapshot(uint64_t out_addr); /* misc.c (c-chain) @ 00192138 */

/* ===== rodata ===== */
extern const uint64_t SERVICE_RECORD_HEADER;  /* 0x10f720 — literal fast-replay record header */
extern const char g_tag_read_failed[];        /* 0x133b50 — body-tag for a failed region read */
extern const char g_tag_hash_mismatch[];      /* 0x133616 — body-tag for a digest mismatch */

/*
 * is_plausible_ptr_ma — remote-pointer sanity gate shared by the
 * remote-object checks: reject anything below 0x1000 or not 8-byte
 * aligned (the dump spells this "(v + 0x238) >> 3 < 0x247 || v & 7").
 */
static bool is_plausible_ptr_ma(uint64_t v)
{
    return 0x1000 <= v && (v & 7) == 0;
}

/* ===== engine accessor (module base + remote read/poke fns) ===== */

typedef struct {
    uint64_t base;       /* +0x00 module base (libg load address) */
    void     *ctx;       /* +0x08 context handed to the fns (0) */
    int (*read)(void *ctx, uint64_t addr, void *out, uint32_t len);  /* +0x10 */
    int (*unused_18)(void);                                           /* +0x18 */
    int (*poke)(void *ctx, uint64_t addr, uint64_t len);              /* +0x20 — 3-arg remote
                                       * store; the data argument is not
                                       * recoverable from the dump */
} engine_accessor_t;

/* allocator state snapshot (see allocator_state_read) */
typedef struct {
    uint64_t block;      /* +0x00 allocator block address (libg+0x12ff038) */
    uint32_t field_a;    /* +0x08 block+0x88 low dword (< 4) */
    uint32_t field_b;    /* +0x0c block+0x88 high dword (< 4) */
    uint8_t  mode;       /* +0x10 byte at libg+0x12f03e4 (< 2) */
} allocator_state_t;

/* fast-replay service record (filled by script_port_fast_replay_snapshot) */
typedef struct {
    uint64_t header;     /* +0x00 literal header (0x10f720) */
    uint32_t ok_a;       /* +0x08 must be nonzero to accept */
    uint32_t ok_b;       /* +0x0c */
    uint64_t field_10;   /* +0x10 must be nonzero to accept */
    uint64_t field_18;   /* +0x18 must be nonzero to accept */
} service_record_t;

int allocator_valid_check(const engine_accessor_t *acc, uint64_t block); /* @ 00155980 */

/* ===== captured hook / guard blocks (written elsewhere, only read here) ===== */
extern uint64_t g_hook_module_handle;  /* 0x22db18 — captured overlay node */
extern uint64_t g_hook_last_a;         /* 0x22db20 — last captured node id */
extern uint64_t g_hook_last_b;         /* 0x22db28 — last captured +0x248 value */
extern uint64_t g_hook_last_c;         /* 0x22db30 — last captured identity */
extern uint64_t g_guard_last_a;        /* 0x22f1b0 — island identity a */
extern uint64_t g_guard_last_b;        /* 0x22f1b8 — island identity b */
extern uint64_t g_guard_last_c;        /* 0x22f1c0 — island identity c (in flight) */
extern uint64_t g_overlay_parent;      /* 0x22db60 — parent widget the captured pair
                                        * is published through (captured by widgets.c) */

/* captured overlay pair (cleared here; captured by plus_request_dispatch) */
uint64_t g_capture_subject;     /* 0x22db40 — widget the pair was captured from */
uint64_t g_capture_pair_a;      /* 0x22db48 — captured pair slot a */
uint64_t g_capture_pair_b;      /* 0x22db50 — captured pair slot b */
uint64_t g_capture_flags;       /* 0x22db58 — packed: high dword (0x22db5c) is
                                 * {bit0 = slot a live, bit1 = slot b live} */
uint64_t g_pair_release_epoch;  /* 0x22dad8 — cleared when the pair is fully released */

/* nexus_protected_plus_active resolution (once-guard + cached verdicts) */
static pthread_once_t g_plus_resolve_once;        /* 0x22f1d0 */
static int (*g_plus_query_fn)(int);              /* 0x22f1d8 — resolved plus-active fn */
uint32_t g_plus_active_a;                         /* 0x22db68 — verdict cached for mode 1 */
uint32_t g_plus_active_b;                         /* 0x22db6c — verdict refreshed on every call */

/* island stubs installed by postbattle_install (raw line 2402+) */
uint64_t g_widget_text_island_addr;   /* 0x22dda0 — widget-text island address */
int      g_island_readback_enabled;   /* 0x22dda8 — 1 = island readback length is valid */
uint32_t g_widget_text_island_stamp;  /* 0x22ddac — expected island stamp (branch word) */
char     g_widget_text_island_copy[0x200]; /* 0x22ddb0 — saved widget-text island image */
uint64_t g_ui_server_island_addr;     /* 0x22f1e0 — ui-server island address */
uint32_t g_ui_server_island_stamp;    /* 0x22f1e8 — expected island stamp (branch word) */
char     g_ui_server_island_copy[0x200];  /* 0x22f1ec — saved ui-server island image */

/* island template image (module code copied into the remote islands);
 * the length must stay <= 0x200 for the readback verification */
extern const void *const g_island_tmpl_begin;  /* 0x1a36c0 — GOT: template image start */
extern const void *const g_island_tmpl_end;    /* 0x1a36c8 — GOT: template image end */
#define ISLAND_TMPL_LEN \
    ((size_t)((uintptr_t)g_island_tmpl_end - (uintptr_t)g_island_tmpl_begin))

/* bootstrap status / integrity report */
const char *g_ui_initialize_reason;  /* 0x1a7c40 — current bootstrap status line */
char g_body_tag[0x30];               /* 0x22f3ec — last body-hash failure tag */

/* ===== game-image call forwarding ===== */

/*
 * game_call_66ad48 — forward a one-argument call through the libg game
 * image at +0x66ad48 (Ghidra could not recover the jump target table, so
 * the dump shows a bare indirect call; the misc b-chain call sites prove
 * the x0 argument). @ 0014ec30
 */
void game_call_66ad48(void *arg)
{
    ((void (*)(void *))((uint64_t)g_proc_mem_bias + 0x66ad48))(arg);
}

/* ===== late-bound service objects ===== */

/*
 * service_object_acquire — build a fast-replay service record via the
 * late-bound dispatch (script_port_fast_replay_snapshot) when the screen
 * record carries a pending event and no island capture is in flight for
 * the same node/identity pair. Returns 1 when the filled record has all
 * its completion fields set, 0 otherwise. @ 0014f434
 */
int service_object_acquire(const screen_record_t *rec, service_record_t *out)
{
    if (rec == NULL || out == NULL)
        return 0;
    out->ok_a = 0;
    out->ok_b = 0;
    out->field_10 = 0;
    out->field_18 = 0;
    out->header = SERVICE_RECORD_HEADER;
    if (rec->count_a == 0 || rec->flag == 0)
        return 0;
    if (g_guard_last_c != 0 && g_guard_last_a == rec->node &&
        g_guard_last_b == rec->identity)
        return 0;   /* an island capture is already running for this pair */
    if (script_port_fast_replay_snapshot((uint64_t)out) == 0)
        return 0;
    if (out->field_10 != 0 && out->field_18 != 0)
        return out->ok_a != 0;
    return 0;
}

/* ===== captured overlay checks ===== */

/*
 * captured_widget_check — true when the captured overlay node still
 * matches the screen record (node id and identity), passes the button
 * lineage check, and its +0x248 field still holds the captured value.
 * @ 0014f4dc
 */
int captured_widget_check(const screen_record_t *rec)
{
    uint64_t screen;
    uint64_t value;

    if (rec->count_a == 0 || g_hook_module_handle == 0)
        return 0;
    if (rec->node != g_hook_last_a || rec->identity != g_hook_last_c)
        return 0;
    screen = button_lineage_check(g_hook_module_handle,
                                  g_hook_last_c /* tracked screen node */);
    if (screen == 0)
        return 0;
    if (!proc_mem_read(NULL, g_hook_module_handle + 0x248, &value, 8))
        return 0;
    if (!is_plausible_ptr_ma(value))
        return 0;
    return value == g_hook_last_b;
}

/*
 * captured_pair_pump — MainPool-only pump for the captured overlay pair.
 * With the postbattle islands verified, the pair slots a/b (whose objects
 * must still carry the island vtable tag at libg+0x11c0a48 and be linked
 * under the capture parent) are handed to ui_widget_pair_release with the
 * capture parent; slots the release cleared (and that were not re-captured)
 * are dropped, and a full release (result 1) clears the whole capture
 * block. Only mode 0/1 run; clearing happens for mode != 0 only.
 * Returns the release verdict (0/1). @ 0014f830
 */
uint32_t captured_pair_pump(uint32_t mode)
{
    char name[16] = {0};
    uint64_t pair[2];
    uint64_t a_before, b_before;
    uint32_t flags;
    bool b_verified, a_absent;
    uint64_t vtable;
    int ok;

    if (mode >= 2)
        return 0;
    if (!(g_main_pool_tid > 0 && gettid() == g_main_pool_tid))
        return 0;
    if (pthread_getname_np(pthread_self(), name, 0x10) != 0)
        return 0;
    if (memcmp(name, "MainPool", 9) != 0)  /* 0x706f6f6c6e69614d + NUL */
        return 0;
    if (!postbattle_island_verify())
        return 0;

    b_before = g_capture_pair_b;
    a_before = g_capture_pair_a;
    if (g_capture_pair_a == 0 && g_capture_pair_b == 0) {
        b_verified = false;
        a_absent = true;
        goto pump;
    }
    if (g_capture_subject != g_overlay_parent)
        return 0;
    a_absent = g_capture_pair_a == 0;
    if (a_absent) {
handle_b:
        if (b_before == 0) {
            b_verified = false;
            goto pump;
        }
        /* verify slot b still carries the island vtable tag */
        vtable = 0;
        if (!proc_mem_read(NULL, g_capture_pair_b, &vtable, 8))
            return 0;
        if (!is_plausible_ptr_ma(vtable) ||
            vtable != (uint64_t)g_proc_mem_bias + 0x11c0a48U)
            return 0;
        if (!remote_child_link_valid(g_capture_pair_b, g_overlay_parent))
            return 0;
        b_verified = true;
        goto pump;
    }
    /* verify slot a */
    vtable = 0;
    if (!proc_mem_read(NULL, g_capture_pair_a, &vtable, 8))
        return 0;
    if (!is_plausible_ptr_ma(vtable) ||
        vtable != (uint64_t)g_proc_mem_bias + 0x11c0a48U)
        return 0;
    if (!remote_child_link_valid(g_capture_pair_a, g_overlay_parent))
        return 0;
    goto handle_b;

pump:
    pair[0] = a_before;
    pair[1] = b_before;
    ok = ui_widget_pair_release(g_overlay_parent, pair, mode);
    if (mode != 0) {
        flags = (uint32_t)(g_capture_flags >> 32);
        if (!a_absent && pair[0] == 0 && g_capture_pair_a == a_before) {
            /* release cleared slot a and it was not re-captured */
            flags &= ~1u;
            g_capture_pair_a = 0;
            g_capture_flags &= ~(1ULL << 32);
        }
        if (pair[1] != 0)
            b_verified = false;   /* only a cleared slot counts as gone */
        if (b_verified && g_capture_pair_b == b_before) {
            g_capture_pair_b = 0;
            g_capture_flags = (((uint64_t)flags << 32) |
                               (g_capture_flags & 0xffffffffu)) & ~(2ULL << 32);
        }
        if (ok == 1) {
            /* pair fully released: drop the whole capture block */
            g_capture_pair_a = 0;
            g_capture_subject = 0;
            g_capture_flags = 0;
            g_capture_pair_b = 0;
            g_pair_release_epoch = 0;
        }
    }
    return (uint32_t)ok;
}

/* ===== postbattle island verification ===== */

/*
 * postbattle_island_verify — verify the ui-server island stub still
 * matches its saved copy: the stamp word at libg+0x678fe4 must equal the
 * recorded stamp, then the island image is read back (only when the
 * readback length is enabled; length 0 compares trivially) and compared
 * byte-for-byte with g_ui_server_island_copy. Requires the islands to be
 * installed and the template image to be <= 0x200 bytes. Returns 1 on
 * match. @ 0014fac4
 */
int postbattle_island_verify(void)
{
    uint8_t image[512];
    uint32_t stamp;
    size_t len;

    len = ISLAND_TMPL_LEN;
    if (g_postbattle_installed != 1 || g_ui_server_island_addr == 0)
        return 0;
    if (g_island_readback_enabled == 0)
        return 0;
    if (len == 0 || len >= 0x201)
        return 0;
    if (!proc_mem_read(NULL, (uint64_t)g_proc_mem_bias + 0x678fe4, &stamp, 4))
        return 0;
    if (stamp != g_ui_server_island_stamp)
        return 0;
    if (!proc_mem_read(NULL, g_ui_server_island_addr, image, (uint32_t)len))
        return 0;
    return memcmp(image, g_ui_server_island_copy, len) == 0;
}

/* ===== Nexus+ activation state ===== */

/*
 * plus_active_refresh — refresh the cached nexus_protected_plus_active
 * verdicts. The resolver runs under a pthread_once guard (0x22f1d0); when
 * it resolved a function, mode 1 stores its ==1 verdict in g_plus_active_a
 * (0 otherwise), and g_plus_active_b is cleared whenever the verdict is
 * not 1 (it is only ever raised elsewhere). @ 0014fbe8
 */
void plus_active_refresh(int mode)
{
    uint32_t a = 0;

    if (mode == 1) {
        pthread_once(&g_plus_resolve_once, protected_plus_resolve_once);
        if (g_plus_query_fn != NULL)
            a = g_plus_query_fn(0 /* pthread_once result */) == 1;
    }
    g_plus_active_a = a;
    pthread_once(&g_plus_resolve_once, protected_plus_resolve_once);
    if (g_plus_query_fn == NULL || g_plus_query_fn(0) != 1)
        g_plus_active_b = 0;
}

/* ===== code-region integrity (with failure reporting) ===== */

/*
 * code_region_hash_verify_report — SHA-256 a libg code region (offset is
 * module-relative, g_proc_mem_bias is added) in <= 0x100-byte proc-mem
 * chunks and compare the 64-char hex digest. On a failed read or a digest
 * mismatch the failure is recorded as a "body_%lx_%s" tag in g_body_tag
 * (0x22f3ec) and g_ui_initialize_reason is pointed at it. Returns 1 on
 * match, 0 otherwise. @ 0014fe14
 */
int code_region_hash_verify_report(uint64_t offset, size_t len,
                                   const char *expect_hex)
{
    uint8_t state[112]; /* SHA-256 context (sha256_state_t layout) */
    uint8_t digest[32];
    uint8_t chunk[256];
    char hex[68];
    size_t done, n;
    int i;

    sha256_init((sha256_state_t *)state);
    for (done = 0; done < len; done += n) {
        n = len - done;
        if (n > 0x100)
            n = 0x100;
        if (!proc_mem_read(NULL, offset + (uint64_t)g_proc_mem_bias + done,
                           chunk, (uint32_t)n))
            goto report_read_failed;
        sha256_update((sha256_state_t *)state, chunk, n);
    }
    sha256_final((sha256_state_t *)state, digest);
    for (i = 0; i < 32; i++)
        str_format(hex + 2 * i, (size_t)-1, 3, "%02x" /* rodata 0x139d9f */,
                   digest[i]);
    if (strcmp(hex, expect_hex) == 0)
        return 1;
    str_format(g_body_tag, 0x30, 0x30, "body_%lx_%s", offset,
               g_tag_hash_mismatch /* 0x133616 */);
    g_ui_initialize_reason = g_body_tag;
    return 0;

report_read_failed:
    str_format(g_body_tag, 0x30, 0x30, "body_%lx_%s", offset,
               g_tag_read_failed /* 0x133b50 */);
    g_ui_initialize_reason = g_body_tag;
    return 0;
}

/*
 * widget_text_island_guard — guard for engine calls at libg+0x88836c:
 * size must be >= 4, offset must be 0x88836c and base the libg base, the
 * postbattle islands must be installed, and the widget-text island stamp
 * (libg+0x88836c) and image must still match the saved copy. On success
 * *patch_out receives 0xa9bd7bfd (an ARM64 "stp x29,x30,[sp,#-0x30]!"
 * prologue) that the caller patches in. Returns 1 when the call may run.
 * @ 0014ff7c
 */
int widget_text_island_guard(uint64_t base, uint64_t offset,
                             uint32_t *patch_out, uint64_t size)
{
    uint8_t image[512];
    uint32_t stamp;
    size_t len;
    int ok;

    len = ISLAND_TMPL_LEN;
    if (size < 4 || offset != 0x88836c || base != (uint64_t)g_proc_mem_bias)
        return 0;
    if (g_postbattle_installed == 0 || g_widget_text_island_addr == 0)
        return 0;
    if (len >= 0x201 && (g_island_readback_enabled & 1) != 0)
        return 0;   /* oversized template with readback enabled: refuse */
    if (!proc_mem_read(NULL, base + 0x88836c, &stamp, 4))
        return 0;
    if (stamp != g_widget_text_island_stamp)
        return 0;
    ok = proc_mem_read(NULL, g_widget_text_island_addr, image,
                       g_island_readback_enabled != 0 ? len : 0);
    if (!ok)
        return 0;
    if (memcmp(image, g_widget_text_island_copy,
               g_island_readback_enabled != 0 ? len : 0) != 0)
        return 0;
    stamp = 0xa9bd7bfd;   /* dead store in the dump: -0x56428403 */
    (void)stamp;
    *patch_out = 0xa9bd7bfd;
    return 1;
}

/* ===== master integrity sweep ===== */

/* one {module offset, length, expected SHA-256 hex} table entry */
typedef struct {
    uint64_t offset;
    uint64_t len;
    const char *expect_hex;
} region_hash_entry_t;

/* 17-entry region table (base 0x198a10; the anchor entry's hash string is
 * "a17505cd…" with its pointer slot at 0x198a20) */
extern const region_hash_entry_t g_integrity_regions[17];

/*
 * integrity_sweep_verify — verify the libg code image: region 0x59567c
 * (0x20 bytes), then the 17-entry region table, then six fixed regions.
 * Returns 1 only when every digest matches. @ 001505d8
 */
int integrity_sweep_verify(void)
{
    int i;

    if (!code_region_hash_verify(0x59567c, 0x20,
            "9e18529b88a6285f2675ad92537b12875a9735c6fc16cbdc947b58b5e235b5f9"))
        return 0;
    for (i = 0; i < 17; i++)
        if (!code_region_hash_verify(g_integrity_regions[i].offset,
                                     g_integrity_regions[i].len,
                                     g_integrity_regions[i].expect_hex))
            return 0;
    if (!code_region_hash_verify(0x75724c, 0x14,
            "b1a8612e3b1b18d5a4fa341f79cdea62de5653b32cf5cc80e8525baefd0f85fa"))
        return 0;
    if (!code_region_hash_verify(0x756bc0, 0x68c,
            "f30e5b794304199c9860940b334237b8d3040aa8647118c729d9a73ac7df8faa"))
        return 0;
    if (!code_region_hash_verify(0x75cddc, 0x18,
            "0f2cf08558111cad0010ddf2c88c3f6312783209bcb27195a3a1879a44a058bb"))
        return 0;
    if (!code_region_hash_verify(0x5db534, 0xc60,
            "af6384d59fa8cd2716627388fb076db6f786e7d3884ea1f77b3bf85fddd9a9f3"))
        return 0;
    if (!code_region_hash_verify(0xd06d98, 0x5ac,
            "68feb4f4a33b0568cf9836c921863f42f30a2e87ddf099f79723d8fa6231776e"))
        return 0;
    if (!code_region_hash_verify(0x56f2fc, 0x1d0,
            "c56bb1024e8c5b5cf3b91a235440cd0cba889e4b3fe5838ae6b0c8e059710669"))
        return 0;
    return code_region_hash_verify(0xff21e0, 0xd8,
            "1071ba9e2114fe5b27b428d41d48e90ceade20037a38deae7c6da369c9d078c9");
}

/* ===== game allocator resolution ===== */

/*
 * allocator_state_read — resolve the game allocator through an engine
 * accessor: the pointer at libg+0x12ff038 must be a plausible block that
 * re-reads identically and whose first word is libg+0x11baa20 (the
 * allocator vtable); the block's +0x88 dword pair and the mode byte at
 * libg+0x12f03e4 are read into *out. Returns 1 when the fields pass
 * their range gates and allocator_valid_check accepts the block.
 * @ 00150710
 */
int allocator_state_read(const engine_accessor_t *acc, allocator_state_t *out)
{
    uint64_t block, reread, vtable;
    int ok;

    if (acc == NULL || out == NULL)
        return 0;
    if (acc->read == NULL || acc->base == 0)
        return 0;
    out->field_a = 0;
    out->field_b = 0;
    out->block = 0;
    if (!acc->read(acc->ctx, acc->base + 0x12ff038, out, 8))
        return 0;
    block = out->block;
    if (!is_plausible_ptr_ma(block))   /* (block + 0x238) >> 3 < 0x247 || block & 7 */
        return 0;
    reread = 0;
    ok = acc->read(acc->ctx, acc->base + 0x12ff038, &reread, 8);
    if (ok == 0 || reread != block)
        return 0;
    vtable = 0;
    ok = acc->read(acc->ctx, block, &vtable, 8);
    if (ok == 0 || vtable != acc->base + 0x11baa20)
        return 0;
    if (!acc->read(acc->ctx, block + 0x88, &out->field_a, 8))
        return 0;
    if (!acc->read(acc->ctx, acc->base + 0x12f03e4, &out->mode, 1))
        return 0;
    if (out->mode < 2 && out->field_a < 4 && out->field_b < 4)
        return allocator_valid_check(acc, block) != 0;
    return 0;
}
/* misc_a chain chunk 5: covers raw lines 2001-2500 */

#include <fcntl.h>

/* ===== JNI vtable veneers (see chunk 1) ===== */
static long jni_call1_ma(void *obj, uint32_t slot, long a)
{
    long *table = *(long **)obj;
    long (*fn)(void *, long) = (long (*)(void *, long))table[slot / 8];
    return fn(obj, a);
}

static long jni_call4_ma(void *obj, uint32_t slot, long a, long b, long c, long d)
{
    long *table = *(long **)obj;
    long (*fn)(void *, long, long, long, long) =
        (long (*)(void *, long, long, long, long))table[slot / 8];
    return fn(obj, a, b, c, d);
}

/* ===== forward declarations within this chain ===== */
int  debug_quality_file_read(int dirfd, uint32_t rec[3]);             /* @ 00155a40 */
void postbattle_failure_note(uint64_t base, uint64_t site, uint32_t index,
                             const char *stage, int err);            /* @ 0015c6f8 */
int  island_patches_apply(void *islands, const void *patch_blob,
                          void *scratch);                             /* @ 0015c9e0 */
void ui_server_trampoline_record(uint64_t island, uint32_t id,
                                 const void *readback);               /* @ 0015d078 */

/* ===== cross-file functions ===== */
extern int  menu_mainloop_alive_check(void);   /* menu_engine.c @ 00152b7c */
extern uint64_t ui_trampoline_install(uint64_t site, uint64_t table_id,
                                      uint32_t alloc_id);  /* widgets.c @ 0015b8f4 */
extern void widget_probe_line_format(uint64_t base, void *out); /* widgets.c @ 0015cd20 */
extern int  arm64_branch_encode(uint64_t from, uint64_t to,
                                uint32_t *out);  /* misc.c (b-chain) @ 00190d68 */
extern int  remote_modules_hash_verify(uint64_t base,
                                       int (*read)(void *, uint64_t, void *, uint32_t),
                                       void *unused, const uint8_t *table,
                                       uint64_t count); /* misc.c (c-chain) @ 00191058 */
extern int  engine_vtables_verify(uint64_t base,
                                  int (*read)(void *, uint64_t, void *, uint32_t),
                                  void *unused);  /* misc.c (b-chain) @ 00191170 */
extern void *__emutls_get_address(void *control);  /* bionic emutls */
extern int  __write_chk(int fd, const void *buf, size_t n,
                        size_t buflen);             /* fortified write */

/* ===== rodata ===== */
extern const char g_diag_source_prefix[];     /* 0x134ec1 — "native_%s_%s" source tag */
extern const char s_postbattle_code_guards_00136be3[];      /* 0x136be3 */
extern const char s_postbattle_island_allocate_0013afa9[];  /* 0x13afa9 */
extern const char s_postbattle_island_size_00135b91[];      /* 0x135b91 */
extern const char s_postbattle_island_readback_00131841[];  /* 0x131841 */
extern const char s_postbattle_write_or_readback_00137c48[];/* 0x137c48 */
extern const char g_status_postbattle_failed[];  /* 0x25c9dc — tail of the 0x25c9d8 status blob */
extern const uint8_t g_code_guard_table[];   /* 0x10fbf8 — 19-entry module/code guard table */
extern const uint64_t ISLAND_TABLE_IDS[4];   /* 0x10fb80 — per-island table ids */
extern const uint64_t ISLAND_ID_0;           /* 0x10f888 — {alloc id lo, branch slot hi} */
extern const uint64_t ISLAND_ID_1;           /* 0x10f798 */
extern const uint64_t ISLAND_ID_2;           /* 0x10f768 */
extern const uint64_t ISLAND_ID_3;           /* 0x10f908 */
extern const uint8_t ISLAND_PATCH_DESC[];    /* 0x19ae00 — island patch descriptors */

/* ===== globals ===== */
int g_host_session;               /* 0x1a7c50 — files-dir fd for diagnostics files
                                   * (opened by files_dir_open) */
int64_t g_proc_mem_bias;          /* 0x22d8e0 — libg.so load base (captured by
                                   * phdr_libg_capture) */
uint32_t g_libg_capture_count;    /* 0x22e018 — libg.so phdr callbacks seen */
char g_libg_path[0x1000];         /* 0x22e01c — captured libg.so path */
uint64_t g_slot_ranges[8];        /* 0x22f430 — captured executable segment starts */
uint64_t g_slot_range_ends[8];    /* 0x22f470 — captured executable segment ends */
uint64_t g_capture_pair;          /* 0x22f4b0 — packed {count lo32, overflow hi32}
                                   * of captured RX ranges */
extern const uint64_t CAPTURE_PAIR_RESET;  /* 0x10f740 — literal {count=0, overflow=1};
                                   * the registry suggestion "g_libg_span" is wrong:
                                   * this is a compiler literal-pool entry */
extern char g_postbattle_tls_ctrl[];  /* 0x1a7c88 — emutls control (2-word block:
                                   * trampoline stage label + value) */

static uint32_t g_bsd_quality_seq;  /* 0x22f420 — BSDQ file sequence counter
                                     * (-1 disables the diagnostics writer) */

/* ===== allocator field probing ===== */

/*
 * allocator_fields_probe — poke the allocator block: an 8-byte store at
 * block+0x88 and a 1-byte store at libg+0x12f03e4 through the accessor's
 * third slot (the dump shows a 3-argument call with no data operand —
 * the primitive writes fixed data). Returns 1 when both stores succeed.
 * @ 001508c8
 */
int allocator_fields_probe(const engine_accessor_t *acc, uint64_t block)
{
    if (acc->poke(acc->ctx, block + 0x88, 8) == 0)
        return 0;
    return acc->poke(acc->ctx, acc->base + 0x12f03e4, 1) != 0;
}

/* ===== BSDQ diagnostics file ===== */

/*
 * crc32_bytes — plain bitwise CRC-32 (poly 0xedb88320, init/final
 * 0xffffffff/~); the dump computes it with an unrolled NEON lane loop
 * over the literal tables at 0x10f9c0/0x10f9c8/0x10fac0/0x10fac8.
 */
static uint32_t crc32_bytes(const void *data, size_t len)
{
    const uint8_t *p = data;
    uint32_t crc = 0xffffffffu;
    size_t i;
    int b;

    for (i = 0; i < len; i++) {
        crc ^= p[i];
        for (b = 0; b < 8; b++)
            crc = (crc >> 1) ^ (0xedb88320u & (0u - (crc & 1u)));
    }
    return ~crc;
}

/*
 * debug_quality_file_write — atomically write a 24-byte BSDQ record
 * ("BSDQ" magic, version 1, three little-endian quality values from
 * values[0..2] with ranges <2/<4/<4, then a CRC32 of the first 20 bytes)
 * as "bsd-quality-<pid>-<seq>.tmp" in the diagnostics dir, fsync, rename
 * to "bsd-debug-quality.bin", fsync the dir, and verify by reading the
 * file back. values[0] must be < 2 and the others < 4. Returns 1 when the
 * readback matches all three values, 0 otherwise (tmp unlinked on error,
 * errno preserved). @ 00150920
 */
int debug_quality_file_write(const uint32_t values[3])
{
    uint8_t rec[24];
    char path[0x60];
    uint32_t back[3];
    uint64_t seq;
    int fd, dirfd = g_host_session;
    uint64_t done;
    ssize_t n;
    int saved_errno;

    if (g_bsd_quality_seq == 0xffffffffu)
        return 0;
    seq = (uint64_t)++g_bsd_quality_seq;
    if (values == NULL || dirfd < 0)
        return 0;
    if (values[0] >= 2 || values[1] >= 4 || values[2] >= 4)
        return 0;

    rec[0] = 0x42;  /* 'B' */
    rec[1] = 0x53;  /* 'S' */
    rec[2] = 0x44;  /* 'D' */
    rec[3] = 0x51;  /* 'Q' */
    rec[4] = 1;     /* version */
    rec[5] = 0;
    rec[6] = 0;
    rec[7] = 0;
    rec[8] = (uint8_t)values[0];
    rec[9] = (uint8_t)(values[0] >> 8);
    rec[10] = (uint8_t)(values[0] >> 0x10);
    rec[11] = (uint8_t)(values[0] >> 0x18);
    rec[12] = (uint8_t)values[1];
    rec[13] = (uint8_t)(values[1] >> 8);
    rec[14] = (uint8_t)(values[1] >> 0x10);
    rec[15] = (uint8_t)(values[1] >> 0x18);
    rec[16] = (uint8_t)values[2];
    rec[17] = (uint8_t)(values[2] >> 8);
    rec[18] = (uint8_t)(values[2] >> 0x10);
    rec[19] = (uint8_t)(values[2] >> 0x18);
    *(uint32_t *)(rec + 20) = crc32_bytes(rec, 20);

    str_format(path, 0x60, 0x60, "bsd-quality-%d-%llu.tmp", (int)getpid(), seq);
    fd = openat(dirfd, path, 0x880c1 /* O_WRONLY|O_CREAT|O_EXCL|O_CLOEXEC|0x8000 */,
                0x180 /* 0600 */);
    if (fd < 0)
        return 0;
    done = 0;
    do {
        n = __write_chk(fd, rec + done, 0x18 - done, (size_t)-1);
        if (n < 0) {
            if (errno != EINTR)
                goto write_failed;
        } else {
            done += (uint64_t)n;
            if (n == 0)
                break;   /* short write guard */
        }
    } while (done < 0x18);
    if (done == 0x18) {
        int wsync = fsync(fd);
        int wclose = close(fd);
        if (wclose == 0 && wsync == 0) {
            if (renameat(dirfd, path, dirfd, "bsd-debug-quality.bin") == 0) {
                if (fsync(dirfd) == 0) {
                    if (debug_quality_file_read(dirfd, back) == 0)
                        return 0;
                    if (values[0] == back[0] && values[1] == back[1])
                        return values[2] == back[2];
                    return 0;
                }
                return 0;
            }
            return 0;
        }
        return 0;
    } else {
write_failed:
        close(fd);
    }
    saved_errno = errno;
    unlinkat(dirfd, path, 0);
    errno = saved_errno;
    return 0;
}

/* ===== JNI startup diagnostics ===== */

/*
 * jni_diagnostics_failure — report a module failure to
 * nexus/loader/StartupDiagnostics.failure(String, Throwable) with a
 * LinkageError carrying the message. The tag is formatted as
 * "native_<prefix>_<tag>", lowercased and validated ([a-z0-9_]). A pending
 * Java exception is cleared for the duration and re-thrown afterwards.
 * errno is preserved. @ 001525f4
 */
void jni_diagnostics_failure(void *jvm, const char *tag, const char *message)
{
    void *env = NULL;
    long pending, cls, err_cls, mid, init, jtag, jmsg, throwable;
    char name[68];
    uint32_t n, i;
    int saved_errno = errno;

    if (jvm == NULL || tag == NULL || message == NULL)
        return;
    if (jni_call2_ma(jvm, 0x30 /* GetEnv */, (long)(void *)&env,
                     0x10006 /* JNI_VERSION_1_6 */) != 0 || env == NULL)
        return;

    pending = jni_call0_ma(env, 0x78 /* ExceptionOccurred */);
    if (pending != 0)
        jni_call0_ma(env, 0x88 /* ExceptionClear */);
    if (jni_call1_ma(env, 0x98 /* PushLocalFrame */, 0xc) != 0)
        goto out;

    n = (uint32_t)str_format(name, 0x41, 0x41, "native_%s_%s",
                             g_diag_source_prefix, tag);
    if (!(1 <= n && n <= 0x40))
        goto frame_out;
    for (i = 0; i != n; i++) {
        uint32_t c = (unsigned char)name[i];

        if (c - 0x41 < 0x1a) {   /* A-Z -> a-z */
            c += 0x20;
            name[i] = (char)c;
        }
        if (0x19 < c - 0x61 && c != 0x5f && 9 < c - 0x30)
            goto frame_out;      /* invalid character set */
    }

    cls = jni_call1_ma(env, 0x30 /* FindClass */,
                       (long)"nexus/loader/StartupDiagnostics");
    if (cls == 0 || jni_call0_ma(env, 0x720 /* ExceptionCheck */) != 0)
        goto frame_out;
    mid = jni_call3_ma(env, 0x388 /* GetStaticMethodID */, cls,
                       (long)"failure",
                       (long)"(Ljava/lang/String;Ljava/lang/Throwable;)V");
    if (mid == 0 || jni_call0_ma(env, 0x720) != 0)
        goto frame_out;
    err_cls = jni_call1_ma(env, 0x30 /* FindClass */,
                           (long)"java/lang/LinkageError");
    if (err_cls == 0 || jni_call0_ma(env, 0x720) != 0)
        goto frame_out;
    init = jni_call3_ma(env, 0x108 /* GetMethodID */, err_cls,
                        (long)"<init>", (long)"(Ljava/lang/String;)V");
    if (init == 0 || jni_call0_ma(env, 0x720) != 0)
        goto frame_out;
    jtag = jni_call1_ma(env, 0x538 /* NewStringUTF */, (long)name);
    if (jtag == 0 || jni_call0_ma(env, 0x720) != 0)
        goto frame_out;
    jmsg = jni_call1_ma(env, 0x538 /* NewStringUTF */, (long)message);
    if (jmsg == 0 || jni_call0_ma(env, 0x720) != 0)
        goto frame_out;
    throwable = jni_call3_ma(env, 0xe0 /* NewObject */, err_cls, init, jmsg);
    if (throwable != 0 && jni_call0_ma(env, 0x720) == 0)
        jni_call4_ma(env, 0x468 /* CallStaticVoidMethod */, cls, mid,
                     jtag, throwable);

frame_out:
    if (jni_call0_ma(env, 0x720 /* ExceptionCheck */) != 0)
        jni_call0_ma(env, 0x88 /* ExceptionClear */);
    jni_call1_ma(env, 0xa0 /* PopLocalFrame */, 0);
out:
    if (jni_call0_ma(env, 0x720 /* ExceptionCheck */) != 0)
        jni_call0_ma(env, 0x88 /* ExceptionClear */);
    if (pending != 0) {
        jni_call1_ma(env, 0x68 /* Throw */, pending);
        jni_call1_ma(env, 0xb8 /* DeleteLocalRef */, pending);
    }
    errno = saved_errno;
}

/* ===== libg.so capture (dl_iterate_phdr callback) ===== */

typedef struct {
    uint64_t    addr;   /* +0x00 dlpi_addr — load bias */
    const char *name;   /* +0x08 dlpi_name — object path */
    const void  *phdr;  /* +0x10 dlpi_phdr — program headers (56-byte entries) */
    uint16_t    phnum;  /* +0x18 dlpi_phnum */
} dl_phdr_info_ma_t;

typedef struct {
    uint32_t p_type;    /* +0x00 */
    uint32_t p_flags;   /* +0x04 */
    uint64_t p_offset;  /* +0x08 */
    uint64_t p_vaddr;   /* +0x10 */
    uint64_t p_paddr;   /* +0x18 */
    uint64_t p_filesz;  /* +0x20 */
    uint64_t p_memsz;   /* +0x28 */
    uint64_t p_align;   /* +0x30 */
} phdr64_ma_t;

/*
 * phdr_libg_capture — dl_iterate_phdr callback: on libg.so, record up to
 * 8 executable PT_LOAD segment ranges into g_slot_ranges/g_slot_range_
 * ends (a bad segment resets the packed counter to {count=0, overflow=1}
 * via the 0x10f740 literal, freezing further recording); a clean
 * absolute path without '!' is copied to g_libg_path and its load bias
 * becomes g_proc_mem_bias. Always returns 0 (keep iterating).
 * @ 001529bc
 */
int phdr_libg_capture(const dl_phdr_info_ma_t *info, size_t size, void *data)
{
    const phdr64_ma_t *ph;
    const char *slash;
    uint32_t count, overflow;
    uint16_t i;

    (void)size;
    (void)data;
    if (info->name == NULL)
        return 0;
    slash = strrchr(info->name, '/');
    if (slash == NULL || strcmp(slash + 1, "libg.so") != 0)
        return 0;

    g_libg_capture_count++;
    if (info->phnum != 0) {
        ph = info->phdr;
        count = (uint32_t)g_capture_pair;
        overflow = (uint32_t)(g_capture_pair >> 32);
        for (i = 0; i < info->phnum; i++, ph++) {
            uint32_t next = count;

            if (ph->p_type == 1 /* PT_LOAD */ &&
                (ph->p_flags & 1) /* PF_X */ && overflow == 0) {
                if (ph->p_memsz != 0) {
                    uint64_t start = info->addr + ph->p_vaddr;

                    if (info->addr <= UINT64_MAX - ph->p_vaddr /* no carry */
                        && count < 8
                        && start <= UINT64_MAX - ph->p_memsz /* no carry */) {
                        count = next = count + 1;
                        g_capture_pair = ((uint64_t)overflow << 32) | count;
                        g_slot_ranges[count - 1] = start;
                        g_slot_range_ends[count - 1] = start + ph->p_memsz;
                        continue;
                    }
                }
                count = 0;
                overflow = 1;
                g_capture_pair = CAPTURE_PAIR_RESET; /* 0x10f740 */
            }
        }
    }
    if (strlen(info->name) < 0x1000 && info->name[0] == '/' &&
        strchr(info->name, '!') == NULL) {
        g_proc_mem_bias = (int64_t)info->addr;
        memcpy(g_libg_path, info->name,
               strlen(info->name) + 1);  /* __memcpy_chk, dst cap 0x1000 */
    }
    return 0;
}

/* ===== postbattle bootstrap ===== */

/*
 * postbattle_guards_install — verify the remote module/code guard table
 * (19 entries) and the engine vtable slots against the game image.
 * Returns 1 when both checks pass. @ 00152b20
 */
int postbattle_guards_install(void)
{
    if (remote_modules_hash_verify((uint64_t)g_proc_mem_bias, proc_mem_read,
                                   NULL, g_code_guard_table, 0x13) == 0)
        return 0;
    return engine_vtables_verify((uint64_t)g_proc_mem_bias, proc_mem_read,
                                 NULL) != 0;
}

/*
 * postbattle_install — master postbattle bootstrap. Verifies the
 * Mainloop thread, the 19-entry guard table, the engine vtables and 16
 * hashed code regions; then allocates four RX islands near libg+0x772870,
 * +0x5921a0, +0x88836c and +0x678fe4 (ui_trampoline_install), encodes the
 * ARM64 branches back into each site, reads the islands back (template
 * image must be <= 0x200 bytes), re-verifies the guards plus two more
 * hashed regions, and finally applies the island patches. On success the
 * widget-text and ui-server island stamps/copies are published and the
 * postbattleInstalled flag is raised. Any failure is reported through
 * postbattle_failure_note and the status line is set to the failure blob.
 * Returns 1 on success. @ 00153060
 */
int postbattle_install(void)
{
    struct {
        uint64_t site;        /* +0x00 hook site (module-relative + base) */
        uint32_t alloc_id;    /* +0x08 low half of the rodata id literal */
        uint32_t branch_word; /* +0x0c high half: encoded ARM64 branch */
    } islands[4];
    uint64_t island_addr[4];
    uint64_t tls_pair[4][2];
    uint8_t island_copy[3][512];  /* [0]=+0x88836c, [1]=+0x678fe4, [2]=+0x5921a0 */
    uint8_t probe_line[184];
    uint64_t *tls;
    size_t len;
    int i, fail_idx;
    const char *fail_label;
    uint64_t fail_value;

    g_ui_initialize_reason = s_postbattle_code_guards_00136be3;
    if (menu_mainloop_alive_check() == 0 ||
        remote_modules_hash_verify((uint64_t)g_proc_mem_bias, proc_mem_read,
                                   NULL, g_code_guard_table, 0x13) == 0 ||
        engine_vtables_verify((uint64_t)g_proc_mem_bias, proc_mem_read,
                              NULL) == 0 ||
        !code_region_hash_verify_report(0x66ad48, 0x1c,
            "9edb63b79febb9496a60828270f995d5b618902d80ee1d7caca7a98df8ad3863") ||
        !code_region_hash_verify_report(0x5921a0, 0xb0,
            "6b5853b7737f55c2722ea9a461730d5a0274ec3f756ad12c93cede1764df931a") ||
        !code_region_hash_verify_report(0x887c14, 0xe0,
            "c83656694e0a00083bf7f59348635e4e0b9b5a8edc3bd225809dc8ef8c760356") ||
        !code_region_hash_verify_report(0x888218, 0xa4,
            "d8909f0e5d5ba0ce41dbe44eb6293f6a8242f1a45d597bfb8a1977a4ceecf2e7") ||
        !code_region_hash_verify_report(0xe1f0a0, 0xc,
            "c4c1804266b14a1b5639f8ac2e088ecbd01d17033d0f7d80ad124f703c2d60aa") ||
        !code_region_hash_verify_report(0x905bf8, 0x2da8,
            "20907f70cfbab3552cbf91ddb3508be4b8ed39570a1549ec348600cbc8b60ecd") ||
        !code_region_hash_verify_report(0xaca9d8, 100,
            "18fca600c222900c6e286a8d734adf700bfa3a4eac4448fc398fa04375397bfc") ||
        !code_region_hash_verify_report(0xf3e268, 0x86c,
            "c8628934ac257e1c3af88117aaa68471371e7fda0abea574ec4b36213b6493c4") ||
        !code_region_hash_verify_report(0xf3ead4, 8,
            "2a1fd00c285c56f51aac5cf74a3ac14ad7a1c31b4d6834053a65a90742992ca8") ||
        !code_region_hash_verify_report(0x90aff8, 0x2eec,
            "2e46ec3e6df92b27880099567660412665865a4ddc3b4cef199c3bb877ff339e") ||
        !code_region_hash_verify_report(0x91addc, 0xcac,
            "686c65d371b836433b18b0737fb3b08640d9b9ef7a1763aec9e87e92bf757c67") ||
        !code_region_hash_verify_report(0x91bb7c, 8,
            "e9afda7fdce9e2e1044ebf5a1a0325a13eecdc10f4f89decc9769e92ed1b8e34") ||
        !code_region_hash_verify_report(0xf41ba0, 0x48,
            "fa720cb094c0222bd9154df9dd75c13e6c700a31682aa270aa8cfc8b08447581") ||
        !code_region_hash_verify_report(0xac319c, 0xe0,
            "ecc8aa9a360b1061baa9a987f682adb97f925bc7e1792933fb79367621924911") ||
        !code_region_hash_verify_report(0x88836c, 0xe8,
            "44f795c76de8bc62ebc00eca9d0f192c685f30a2ca3ecdd4b0d016ad659b9ac6") ||
        !code_region_hash_verify_report(0x678964, 0x788,
            "8ce589cc61489c895ba2116daf1f2ab2d73a0ac156e3b2006e1eae2b8f520057"))
        return 0;

    islands[0].site = (uint64_t)g_proc_mem_bias + 0x772870;
    islands[1].site = (uint64_t)g_proc_mem_bias + 0x5921a0;
    islands[2].site = (uint64_t)g_proc_mem_bias + 0x88836c;
    islands[3].site = (uint64_t)g_proc_mem_bias + 0x678fe4;
    islands[0].alloc_id = (uint32_t)ISLAND_ID_0;
    islands[1].alloc_id = (uint32_t)ISLAND_ID_1;
    islands[2].alloc_id = (uint32_t)ISLAND_ID_2;
    islands[3].alloc_id = (uint32_t)ISLAND_ID_3;
    islands[0].branch_word = (uint32_t)(ISLAND_ID_0 >> 32);  /* rodata init,
                                                * overwritten by the encoder */
    islands[1].branch_word = (uint32_t)(ISLAND_ID_1 >> 32);
    islands[2].branch_word = (uint32_t)(ISLAND_ID_2 >> 32);
    islands[3].branch_word = (uint32_t)(ISLAND_ID_3 >> 32);

    g_ui_initialize_reason = s_postbattle_island_allocate_0013afa9;
    tls = __emutls_get_address(g_postbattle_tls_ctrl);
    for (i = 0; i < 4; i++) {
        island_addr[i] = ui_trampoline_install(islands[i].site,
                                                ISLAND_TABLE_IDS[i],
                                                islands[i].alloc_id);
        tls_pair[i][0] = tls[0];   /* failure stage label */
        tls_pair[i][1] = tls[1];   /* failure stage value */
    }

    fail_label = "publish";
    fail_value = 0;
    if (island_addr[0] == 0) { fail_idx = 0; goto island_fail_tls; }
    if (arm64_branch_encode(islands[0].site, island_addr[0],
                            &islands[0].branch_word) == 0) { fail_idx = 0; goto island_fail; }
    if (island_addr[1] == 0) { fail_idx = 1; goto island_fail_tls; }
    if (arm64_branch_encode(islands[1].site, island_addr[1],
                            &islands[1].branch_word) == 0) { fail_idx = 1; goto island_fail; }
    if (island_addr[2] == 0) { fail_idx = 2; goto island_fail_tls; }
    if (arm64_branch_encode(islands[2].site, island_addr[2],
                            &islands[2].branch_word) == 0) { fail_idx = 2; goto island_fail; }
    if (island_addr[3] == 0) { fail_idx = 3; goto island_fail_tls; }
    if (arm64_branch_encode(islands[3].site, island_addr[3],
                            &islands[3].branch_word) == 0) { fail_idx = 3; goto island_fail; }

    len = ISLAND_TMPL_LEN;
    g_ui_initialize_reason = s_postbattle_island_size_00135b91;
    if (len >= 0x201)
        return 0;
    g_ui_initialize_reason = s_postbattle_island_readback_00131841;
    if (!proc_mem_read(NULL, island_addr[2], island_copy[0], (uint32_t)len) ||
        !proc_mem_read(NULL, island_addr[3], island_copy[1], (uint32_t)len) ||
        !proc_mem_read(NULL, island_addr[1], island_copy[2], (uint32_t)len) ||
        menu_mainloop_alive_check() == 0 ||
        postbattle_guards_install() == 0 ||
        !code_region_hash_verify_report(0x88836c, 0xe8,
            "44f795c76de8bc62ebc00eca9d0f192c685f30a2ca3ecdd4b0d016ad659b9ac6") ||
        !code_region_hash_verify_report(0x678964, 0x788,
            "8ce589cc61489c895ba2116daf1f2ab2d73a0ac156e3b2006e1eae2b8f520057"))
        return 0;

    g_ui_initialize_reason = s_postbattle_write_or_readback_00137c48;
    if (island_patches_apply(islands, ISLAND_PATCH_DESC, probe_line) == 1) {
        g_widget_text_island_addr = island_addr[2];
        g_widget_text_island_stamp = islands[2].branch_word;
        g_island_readback_enabled = 1;
        memcpy(g_widget_text_island_copy, island_copy[0], len); /* cap 0x200 */
        g_ui_server_island_addr = island_addr[3];
        g_ui_server_island_stamp = islands[3].branch_word;
        memcpy(g_ui_server_island_copy, island_copy[1], len);   /* cap 0x200 */
        ui_server_trampoline_record(island_addr[1], islands[1].branch_word,
                                    island_copy[2]);
        g_postbattle_installed = 1;
        return 1;
    }
    widget_probe_line_format((uint64_t)g_proc_mem_bias, probe_line);
    g_ui_initialize_reason = g_status_postbattle_failed;
    return 0;

island_fail_tls:
    fail_label = (const char *)tls_pair[fail_idx][0];
    fail_value = tls_pair[fail_idx][1];
island_fail:
    postbattle_failure_note((uint64_t)g_proc_mem_bias, islands[fail_idx].site,
                            (uint32_t)fail_idx, fail_label,
                            (int)fail_value);
    g_ui_initialize_reason = g_status_postbattle_failed;
    return 0;
}
/* misc_a chain chunk 6: covers raw lines 2613-3100 */

#include <sys/mman.h>

/* ===== forward declarations within this chain ===== */
int  proc_mem_write(uint32_t fd, const void *buf, uint64_t len,
                    uint64_t addr);  /* defined in chunk 7 @ 00153cbc */

/* ===== cross-file functions ===== */
extern void widget_runtime_ui_log(const char *reason,
                                  const char *detail);  /* widgets.c @ 001524e8 */
extern int  integrity_sweep_report(void);   /* misc.c (b-chain) @ 00168e48 */
extern void *gap_page_allocate(uint64_t near);  /* misc.c (b-chain) @ 00168f3c */
extern void social_island_pump(uint64_t island);  /* misc.c chain a @ 001690ac */
extern uint32_t social_method_hook(uint64_t obj, uint64_t arg); /* chain a @ 00169170 */
extern int  proc_mem_write(uint32_t fd, const void *buf, uint64_t len,
                           uint64_t addr); /* defined in chunk 7 @ 00153cbc */
extern void code_cache_flush_range(uint64_t start, uint64_t end); /* misc.c (c-chain) @ 00193944 */

/* ===== social-island lane literals (NEON lane-compare source) ===== */
extern const uint32_t SOCIAL_ISLAND_LANE_A0;  /* 0x10fa00 — site lane a0 */
extern const uint32_t SOCIAL_ISLAND_LANE_A1;  /* 0x10fa08 — site lane a1 */
extern const uint32_t SOCIAL_ISLAND_LANE_B0;  /* 0x10fa20 — site lane b0 */
extern const uint32_t SOCIAL_ISLAND_LANE_B1;  /* 0x10fa28 — site lane b1 */

/* ===== social-island globals ===== */
extern const uint64_t SOCIAL_ISLAND_ID_A;   /* 0x10f890 — {id lo, branch slot hi}
                                             * for the 0x85cce8 island */
extern const uint64_t SOCIAL_ISLAND_ID_B;   /* 0x10f768 — for the 0x89902c island */
extern const void *const g_social_tmpl_link_a; /* 0x1a36e0 — GOT: link slot in the template */
extern const void *const g_social_tmpl_link_b; /* 0x1a36e8 — GOT: pump slot in the template */
extern const void *const g_social_tmpl_slot;   /* 0x1a36d8 — GOT: patched slot in the template */
extern const void *const g_social_tmpl_probe;  /* 0x1a36d0 — GOT: probe site in the template */
uint64_t g_hook_original_call;                  /* 0x281a40 — hooked social method slot */
int      g_social_installed;                    /* 0x25c928 — 1 = social islands live */
extern int64_t g_social_req_d;                  /* 0x25c8d0 — social island request d */
extern int64_t g_social_req_e;                  /* 0x25c8d8 — social island request e */
extern uint64_t g_trampoline_len;               /* 0x22f020 — mprotect length for the islands */

/*
 * word_cache_flush — issue a full data-memory barrier (dmb ish) over the
 * pair and flush the pair's first 8 bytes from the instruction cache
 * (code_cache_flush_range(start, start+4)). @ 0015c9d0
 */
void word_cache_flush(void *pair_start, void *pair_end)
{
    (void)pair_end;  /* the dump's end argument is start+4, already covered */
    __asm__ volatile("dmb ish" ::: "memory");  /* DataMemoryBarrier(2,3) */
    code_cache_flush_range((uint64_t)pair_start, (uint64_t)pair_start + 4);
}

/*
 * island_word_write — write one 4-byte word at addr through the remote
 * channel, but only at one of the four postbattle island sites
 * (libg+0x89902c, the two template-link sites at 0x10fa00/0x10fa08 and
 * 0x10fa20/0x10fa28 — compared as two 64-bit lane pairs — and
 * libg+0x85cce8). Returns 1 when exactly 4 bytes were written.
 * @ 0015c8e4
 */
int island_word_write(void *ctx, uint64_t addr, uint32_t word)
{
    (void)ctx;
    if (addr != (uint64_t)g_proc_mem_bias + 0x89902c) {
        uint64_t lane_a0 = (uint64_t)g_proc_mem_bias + SOCIAL_ISLAND_LANE_A0;
        uint64_t lane_a1 = (uint64_t)g_proc_mem_bias + SOCIAL_ISLAND_LANE_A1;
        uint64_t lane_b0 = (uint64_t)g_proc_mem_bias + SOCIAL_ISLAND_LANE_B0;
        uint64_t lane_b1 = (uint64_t)g_proc_mem_bias + SOCIAL_ISLAND_LANE_B1;

        if (addr != lane_a0 && addr != lane_a1 &&
            addr != lane_b0 && addr != lane_b1 &&
            addr != (uint64_t)g_proc_mem_bias + 0x85cce8)
            return 0;
    }
    return proc_mem_write(g_host_channel_fd, &word, 4, addr) == 4;
}

/*
 * social_islands_install — install the two social code islands. Gates on
 * the social request words, the Mainloop thread and the integrity sweep;
 * then two RX pages are allocated near libg+0x85cce8 and libg+0x89902c
 * (gap_page_allocate), the island template image (must be <= 0x200
 * bytes) is copied into the first, the ARM64 branch back into
 * libg+0x85ccec is encoded, the template's patched slot is set to the
 * saved literal (0x910003e0), the link slot to the saved id and the pump
 * slot to social_island_pump; the second page gets the B-side id word,
 * the branch into libg+0x899030, a "ldr x16,#8; br x16" pair
 * (0xd61f020058000050) and social_method_hook. Both pages are flushed,
 * mprotect'ed to RX (g_trampoline_len) and wired in with
 * island_word_write (each verified by a remote re-read); the restore is
 * verified word-by-word and an unverified restore aborts the process
 * ("fatal"/"social_restore_unverified"). Returns 1 when both islands are
 * live (g_social_installed). @ 00153670
 */
int social_islands_install(void)
{
    uint64_t site_a = (uint64_t)g_proc_mem_bias + 0x85cce8;
    uint64_t site_b = (uint64_t)g_proc_mem_bias + 0x89902c;
    uint64_t id_a = SOCIAL_ISLAND_ID_A;   /* {id lo, branch slot hi} */
    uint64_t id_b = SOCIAL_ISLAND_ID_B;
    uint32_t branch_a, branch_b;
    uint8_t *page_a, *page_b;
    size_t len;
    uint32_t readback;
    int ok_a, ok_b;

    if (g_social_req_e == 0 || g_social_req_d == 0)
        return 0;
    if (menu_mainloop_alive_check() == 0)
        return 0;
    if (integrity_sweep_report() == 0)
        return 0;

    page_a = gap_page_allocate(site_a);
    page_b = gap_page_allocate(site_b);
    len = ISLAND_TMPL_LEN;
    if (page_a == NULL || page_b == NULL || 0x200 < len)
        return 0;
    memcpy(page_a, g_island_tmpl_begin, len);

    if (arm64_branch_encode((uint64_t)page_a +
                                ((uintptr_t)g_social_tmpl_probe -
                                 (uintptr_t)g_island_tmpl_begin),
                            site_a + 4, &branch_a) == 0)
        return 0;
    *(uint32_t *)((char *)page_a +
                  ((uintptr_t)g_social_tmpl_slot -
                   (uintptr_t)g_island_tmpl_begin)) = 0x910003e0;
    *(uint32_t *)((char *)page_a +
                  ((uintptr_t)g_social_tmpl_probe -
                   (uintptr_t)g_island_tmpl_begin)) = branch_a;
    *(void **)((char *)page_a +
               ((uintptr_t)g_social_tmpl_link_a -
                (uintptr_t)g_island_tmpl_begin)) = social_island_pump;

    if (arm64_branch_encode((uint64_t)(page_b + 4), site_b + 4, &branch_b) == 0)
        return 0;
    *(uint32_t *)page_b = (uint32_t)id_b;          /* low dword of the id */
    *(uint32_t *)(page_b + 4) = branch_b;
    *(uint64_t *)(page_b + 0x10) = 0xd61f020058000050; /* ldr x16,[pc,#8]; br x16 */
    *(void **)(page_b + 0x18) = social_method_hook;

    code_cache_flush_range((uint64_t)page_a, (uint64_t)page_a + len);
    code_cache_flush_range((uint64_t)page_b, (uint64_t)page_b + 8);
    if (mprotect(page_a, g_trampoline_len, 5 /* PROT_READ|PROT_EXEC */) != 0)
        return 0;
    if (mprotect(page_b, g_trampoline_len, 5) != 0)
        return 0;

    /* wire the islands into the engine slots */
    if (arm64_branch_encode(site_a, (uint64_t)page_a, &branch_a) == 0)
        return 0;
    if (arm64_branch_encode(site_b, (uint64_t)(page_b + 4), &branch_b) == 0)
        return 0;
    if (menu_mainloop_alive_check() == 0 || integrity_sweep_report() == 0)
        return 0;
    g_hook_original_call = (uint64_t)page_b;   /* 0x281a40 — the B-side page */

    ok_a = island_word_write(NULL, site_a, (uint32_t)(id_a >> 32));
    word_cache_flush((void *)site_a, (void *)(site_a + 4));
    if (ok_a == 0 ||
        !proc_mem_read(NULL, site_a, &readback, 4) ||
        readback != (uint32_t)(id_a >> 32)) {
        ok_a = 0;
    } else {
        ok_b = island_word_write(NULL, site_b, (uint32_t)(id_b >> 32));
        word_cache_flush((void *)site_b, (void *)(site_b + 4));
        if (ok_b != 0 &&
            proc_mem_read(NULL, site_b, &readback, 4) &&
            readback == (uint32_t)(id_b >> 32)) {
            g_social_installed = 1;
            return 1;
        }
        ok_a = 0;
    }

    /* restore the A-side slot and verify both restorations */
    island_word_write(NULL, site_a, (uint32_t)id_a);
    word_cache_flush((void *)site_a, (void *)(site_a + 4));
    if (!proc_mem_read(NULL, site_a, &readback, 4) ||
        readback != (uint32_t)id_a)
        goto fatal;
    if ((ok_a & 1) == 0) {
        island_word_write(NULL, site_b, (uint32_t)id_b);
        word_cache_flush((void *)site_b, (void *)(site_b + 4));
        if (!proc_mem_read(NULL, site_b, &readback, 4) ||
            readback != (uint32_t)id_b)
            goto fatal;
    }
    return 0;

fatal:
    widget_runtime_ui_log("fatal", "social_restore_unverified");
    abort();   /* restore could not be verified: refuse to run on */
}

/* ===== JNI native-loaded report ===== */

/*
 * jni_native_loaded_report — synchronously call
 * nexus.loader.NexusLoader.NativeLoaded("NexusUI") on the loader's JVM
 * and report a failure through jni_diagnostics_failure when the callback
 * is absent, the class/method/argument lookup fails, or the invocation
 * raises. Every local reference is released; pending exceptions are
 * cleared after being noticed. @ 00153a10
 */
void jni_native_loaded_report(void *jvm)
{
    void *env = NULL;
    long cls, mid, arg, verdict;
    const char *stage;
    char exc;

    if (jvm == NULL)
        return;
    if (jni_call2_ma(jvm, 0x30 /* GetEnv */, (long)(void *)&env,
                     0x10006 /* JNI_VERSION_1_6 */) != 0 || env == NULL) {
        jni_diagnostics_failure(jvm, "callback_unavailable", "GetEnv_failed");
        return;
    }
    cls = jni_call1_ma(env, 0x30 /* FindClass */,
                       (long)"nexus/loader/NexusLoader");
    if (cls == 0 || jni_call0_ma(env, 0x720 /* ExceptionCheck */) != 0) {
        stage = "callback_loader_class";
        goto fail;
    }
    mid = jni_call3_ma(env, 0x388 /* GetStaticMethodID */, cls,
                       (long)"NativeLoaded", (long)"(Ljava/lang/String;)Z");
    if (mid == 0 || jni_call0_ma(env, 0x720) != 0) {
        stage = "callback_method";
        goto fail;
    }
    arg = jni_call1_ma(env, 0x538 /* NewStringUTF */, (long)"NexusUI");
    if (arg == 0 || jni_call0_ma(env, 0x720) != 0) {
        stage = "callback_argument";
        goto fail;
    }
    verdict = jni_call3_ma(env, 0x3a8 /* CallStaticBooleanMethod */,
                           cls, mid, arg);
    exc = (char)jni_call0_ma(env, 0x720 /* ExceptionCheck */);
    if (exc == 0) {
        jni_call1_ma(env, 0xb8 /* DeleteLocalRef */, arg);
        jni_call1_ma(env, 0xb8 /* DeleteLocalRef */, cls);
        if ((char)verdict == 0)
            jni_diagnostics_failure(jvm, "callback_rejected",
                                    "required_synchronous_callback_absent");
        widget_runtime_ui_log("callback_return", "NexusUI");
        return;
    }
    stage = "callback_invocation";

fail:
    if (jni_call0_ma(env, 0x720 /* ExceptionCheck */) != 0)
        jni_call0_ma(env, 0x88 /* ExceptionClear */);
    if (arg != 0)
        jni_call1_ma(env, 0xb8 /* DeleteLocalRef */, arg);
    if (cls != 0)
        jni_call1_ma(env, 0xb8 /* DeleteLocalRef */, cls);
    widget_runtime_ui_log("callback_unavailable",
                          "ModuleHost_lookup_or_invocation_failed");
    jni_diagnostics_failure(jvm, stage,
                            "required_synchronous_callback_absent");
}
/* misc_a chain chunk 7: covers raw lines 3101-3600 */

/* ===== forward declarations within this chain ===== */
int  visible_chain_check(uint64_t node, uint64_t target);  /* p2 @ 0015516c */
int  remote_widget_ancestor_of(uint64_t node, uint64_t target); /* p2 @ 00155084 */

/* libg remote-vtable anchors used by the lineage checks */
#define LIBG_WIDGET_CLASS_VTABLE   0x11c7b88u  /* widget-class node vtable */
#define LIBG_BUTTON_SLOT_VTABLE    0x11c7e60u  /* button-slot object vtable */
#define LIBG_BUTTON_SLOT_METHODS   0x11c7e68u  /* button-slot method table ptr */
#define LIBG_BUTTON_SLOT_EXPECTED  0x91bb7cu   /* expected method-table target */

/*
 * remote_read_ptr — read one 8-byte pointer from remote memory and apply
 * the plausibility gate (>= 0x1000, 8-byte aligned); returns 0 when the
 * read fails or the value is implausible, 1 with *out set otherwise.
 * (The dump reads at `addr` for both arguments.) @ 00154cf0
 */
int remote_read_ptr(uint64_t addr, uint64_t *out)
{
    uint64_t v = 0;

    if (!proc_mem_read(NULL, addr, &v, 8))
        v = 0;
    if ((v & 7) != 0 || v < 0x1000)
        v = 0;
    *out = v;
    return v != 0;
}

/*
 * remote_child_link_valid — verify a remote parent/child link both ways:
 * the node's +0x38 word must be the parent, the node's +0x40 index must
 * be a valid (>= 0) slot, and the parent's child array (+0x50, gated to
 * plausible, element size 8) at that index must be the node itself.
 * Returns 1 when the link is consistent. @ 00154d5c
 */
int remote_child_link_valid(uint64_t node, uint64_t parent)
{
    uint64_t parent_read = 0;
    uint64_t array;
    uint32_t count;
    int32_t index = -1;
    uint64_t slot = 0;
    int read_ok;

    if (node == 0 || parent == 0)
        return 0;
    read_ok = proc_mem_read(NULL, node + 0x38, &parent_read, 8);
    if (!read_ok || (parent_read & 7) != 0 || parent_read < 0x1000)
        return 0;
    if (parent_read != parent)
        return 0;
    read_ok = proc_mem_read(NULL, node + 0x40, &index, 4);
    if (!read_ok || index < 0)
        return 0;
    read_ok = proc_mem_read(NULL, parent + 0x4e, &count, 2);
    if (!read_ok)
        return 0;
    if (index >= (int32_t)count)
        return 0;
    read_ok = proc_mem_read(NULL, parent + 0x50, &array, 8);
    if (!read_ok)
        return 0;
    if (!(0xfff < array && (array & 7) == 0))
        return 0;
    read_ok = proc_mem_read(NULL, array + (uint64_t)(int64_t)index * 8, &slot, 8);
    if (!read_ok)
        return 0;
    if ((slot & 7) != 0 || slot < 0x1000)
        return 0;
    return slot == node;
}

/*
 * remote_widget_ancestor_of — walk up to 0x1f parent links (+0x38 each,
 * every step re-validated with remote_child_link_valid) starting at node
 * and return 1 when target is reached. (p2 also defines this helper; the
 * coordinator should keep one — see the report note.) @ 00155084
 */
int remote_widget_ancestor_of(uint64_t node, uint64_t target)
{
    uint64_t cur = node;
    uint64_t parent = 0;
    uint32_t hops = 0;
    int read_ok;
    bool plausible;

    if (node == target || node == 0)
        return node == target;
    do {
        read_ok = proc_mem_read(NULL, cur + 0x38, &parent, 8);
        plausible = read_ok && parent >= 0x1000 && (parent & 7) == 0;
        if (!plausible)
            parent = 0;
        read_ok = remote_child_link_valid(cur, parent);
        if (read_ok)
            cur = parent;
    } while (read_ok && parent != target && ++hops < 0x1f && plausible);
    return cur == target;
}

/*
 * button_lineage_check — validate a widget-class node's lineage: the
 * node's vtable must be libg+0x11c7b88, its ancestor chain (up to 0x1f
 * hops, links re-validated) must reach the screen node, and the screen
 * must pass visible_chain_check; the button-slot object read at
 * node+0x80 must have vtable libg+0x11c7e60 and its method-table pointer
 * (libg+0x11c7e68) must be the expected libg+0x91bb7c. Returns the
 * validated screen node, 0 on any failure. @ 00154eb4
 */
uint64_t button_lineage_check(uint64_t node, uint64_t screen)
{
    uint64_t cur = node;
    uint64_t vtable = 0;
    uint64_t parent = 0;
    uint64_t slot_obj;
    uint64_t methods;
    uint32_t hops = 0;
    int read_ok;
    bool plausible;

    if (node == 0)
        return 0;
    read_ok = proc_mem_read(NULL, node, &vtable, 8);
    if (!read_ok || (vtable & 7) != 0 || vtable < 0x1000)
        return 0;
    if (vtable != (uint64_t)g_proc_mem_bias + LIBG_WIDGET_CLASS_VTABLE)
        return 0;

    if (node != screen) {
        do {
            read_ok = proc_mem_read(NULL, cur + 0x38, &parent, 8);
            plausible = read_ok && parent >= 0x1000 && (parent & 7) == 0;
            if (!plausible)
                parent = 0;
            read_ok = remote_child_link_valid(cur, parent);
            if (read_ok)
                cur = parent;
        } while (read_ok && parent != screen && ++hops < 0x1f && plausible);
    }
    if (cur != screen)
        return 0;
    slot_obj = visible_chain_check(node, screen);
    if (slot_obj == 0)
        return 0;
    read_ok = proc_mem_read(NULL, node + 0x80, &vtable, 8);
    if (!read_ok || (vtable & 7) != 0 || vtable < 0x1000)
        return 0;
    if (vtable != (uint64_t)g_proc_mem_bias + LIBG_BUTTON_SLOT_VTABLE)
        return 0;
    if (!proc_mem_read(NULL, (uint64_t)g_proc_mem_bias + LIBG_BUTTON_SLOT_METHODS,
                       &methods, 8))
        return 0;
    return methods == (uint64_t)g_proc_mem_bias + LIBG_BUTTON_SLOT_EXPECTED
               ? slot_obj
               : 0;
}
/* misc_a chain chunk 8: covers raw lines 3601-4100 */

#include <dlfcn.h>
#include <math.h>
#include <sys/stat.h>
#include <link.h>   /* dl_iterate_phdr */

/* ===== cross-file functions ===== */
extern void bind_state_wake_set(void *bind_state);  /* misc.c (b-chain) @ 00169558 */

/* ===== forward declarations within this chunk ===== */
bool maps_range_is_rw(uint64_t unused, uint64_t addr, uint64_t len);   /* below */
int  delivery_plus_active_find(struct dl_phdr_info *info, size_t size,
                               void *data);                          /* below */

/* bionic fortified helpers (only auto-declared under _FORTIFY_SOURCE) */
extern int __read_chk(int fd, void *buf, size_t count, size_t buf_size);
extern int __openat_2(int dirfd, const char *path, int flags);

/* ===== forward declarations within this chain ===== */
int proc_mem_write(uint32_t fd, const void *buf, uint64_t len,
                   uint64_t addr);  /* p2-owned, later chunk @ 00153cbc */

/*
 * visible_chain_check — walk from node up to 0x1e parents (+0x38 each,
 * plausibility-gated) and return 1 when target is reached while every
 * visited node has the visible flag set at +0xc and sane position floats
 * at +0x20 (|x|, |y| finite and < 8192.0; the dump compares with NaN
 * traps). Returns 0 on any failure. @ 0015516c
 */
int visible_chain_check(uint64_t node, uint64_t target)
{
    uint64_t cur = node;
    uint8_t visible = 0;
    float pos[2];
    uint64_t parent;
    uint32_t hops = 0;
    int read_ok;

    if (node == 0)
        return 0;
    for (;;) {
        read_ok = proc_mem_read(NULL, cur + 0xc, &visible, 1);
        if (!read_ok || visible == 0)
            return 0;
        read_ok = proc_mem_read(NULL, cur + 0x20, pos, 8);
        if (!read_ok)
            return 0;
        if (!isfinite(fabsf(pos[0])) || !isfinite(fabsf(pos[1])))
            return 0;
        if (isnan(pos[0]) || isnan(pos[1]))
            return 0;
        if (!(fabsf(pos[0]) < 8192.0f) || !(fabsf(pos[1]) < 8192.0f))
            return 0;   /* the dump traps NaN compares; both must hold */
        if (cur == target)
            return 1;
        parent = 0;
        read_ok = proc_mem_read(NULL, cur + 0x38, &parent, 8);
        if (!read_ok || parent < 0x1000 || (parent & 7) != 0)
            return 0;
        cur = parent;
        if (0x1e < hops++)
            return 0;
    }
}

/* ===== Nexus+ protected-plus resolver ===== */

/*
 * protected_plus_resolve_once — pthread_once body for the nexus_protected_
 * plus_active resolver: dl_iterate_phdr over the loaded objects; when the
 * callback found the symbol exactly once, the resolved function pointer is
 * cached in g_plus_query_fn (0x22f1d8). @ 001552c0
 */
void protected_plus_resolve_once(void)
{
    struct {
        long count;   /* matches found */
        long fn;      /* resolved function */
    } found = {0, 0};

    dl_iterate_phdr(delivery_plus_active_find, &found);
    if ((int)found.count == 1)
        g_plus_query_fn = (int (*)(int))found.fn;
}

/*
 * delivery_plus_active_find — dl_iterate_phdr callback for the plus
 * resolver: on libNexusDelivery.so, dlopen the object (RTLD_LAZY|
 * RTLD_LOCAL), dlsym nexus_protected_plus_active, and accept it only when
 * dladdr confirms the symbol really lives in that same object; the handle
 * is closed again either way. Always returns 0 (keep iterating).
 * @ 0015532c
 */
int delivery_plus_active_find(struct dl_phdr_info *info, size_t size,
                              void *data)
{
    struct {
        long count;   /* +0x00 matches found */
        long fn;      /* +0x08 resolved function */
    } *found = data;
    Dl_info where;
    const char *slash;
    void *handle;
    void *fn;

    (void)size;
    if (info->dlpi_name == NULL)
        return 0;
    slash = strrchr(info->dlpi_name, '/');
    if (slash == NULL || strcmp(slash + 1, "libNexusDelivery.so") != 0)
        return 0;
    found->count++;
    handle = dlopen(info->dlpi_name, 6 /* RTLD_LAZY|RTLD_LOCAL */);
    if (handle == NULL)
        return 0;
    fn = dlsym(handle, "nexus_protected_plus_active");
    if (fn != NULL && dladdr(fn, &where) != 0 &&
        (uint64_t)where.dli_fbase == (uint64_t)info->dlpi_addr)
        found->fn = (long)fn;
    dlclose(handle);
    return 0;
}

/* ===== allocator state write / validation ===== */

/*
 * allocator_state_write — write to the game allocator block, but only at
 * its +0x88 field (8 bytes) or the mode byte at libg+0x12f03e4 (1 byte),
 * and only after resolving the block via libg+0x12ff038, confirming its
 * vtable is libg+0x11baa20, and /proc/self/maps confirms the target range
 * is writable ("rw-p"). Returns 1 when exactly len bytes were written.
 * @ 001556ec
 */
int allocator_state_write(void *ctx, uint64_t addr, const void *src,
                          uint64_t len)
{
    uint64_t block = 0;
    uint64_t vtable = 0;
    int read_ok;

    (void)ctx;
    read_ok = proc_mem_read(NULL, (uint64_t)g_proc_mem_bias + 0x12ff038,
                            &block, 8);
    if (!read_ok || block < 0x1000 || (block & 7) != 0)
        return 0;
    read_ok = proc_mem_read(NULL, block, &vtable, 8);
    if (!read_ok || vtable < 0x1000 || (vtable & 7) != 0)
        return 0;
    if (vtable != (uint64_t)g_proc_mem_bias + 0x11baa20)
        return 0;
    if (!((len == 8 && addr == block + 0x88) ||
          (len == 1 && addr == (uint64_t)g_proc_mem_bias + 0x12f03e4)))
        return 0;
    if (!maps_range_is_rw(0, addr, len))
        return 0;
    return proc_mem_write(g_host_channel_fd, src, (uint32_t)len, addr) ==
           (int64_t)len;
}

/*
 * maps_range_is_rw — true when /proc/self/maps holds a mapping that
 * contains [addr, addr+len) and is permission "rw-p" (the dump compares
 * the 4-byte permission word against 0x702d7772). len must be nonzero
 * and addr+len must not wrap. @ 00155848
 */
bool maps_range_is_rw(uint64_t unused, uint64_t addr, uint64_t len)
{
    char line[512];
    uint64_t start, end;
    char perms[8];
    FILE *maps;
    bool rw = false;

    (void)unused;
    if (addr - 1 >= ~len || len == 0)
        return false;
    maps = fopen("/proc/self/maps", "r");
    if (maps == NULL)
        return false;
    while (fgets(line, 0x200, maps) != NULL) {
        if (sscanf(line, "%lx-%lx %4s", &start, &end, perms) == 3 &&
            start <= addr && addr + len <= end) {
            rw = memcmp(perms, "rw-p", 4) == 0;  /* 0x702d7772 */
            break;
        }
    }
    fclose(maps);
    return rw;
}

/*
 * allocator_valid_check — the engine accessor still resolves the
 * allocator: the pointer at libg+0x12ff038 must equal block, and the
 * block's first word must still be libg+0x11baa20 (the allocator
 * vtable). Returns 1 when both hold. @ 00155980
 */
int allocator_valid_check(const engine_accessor_t *acc, uint64_t block)
{
    uint64_t ptr = 0;
    uint64_t vtable = 0;

    if (!acc->read(acc->ctx, acc->base + 0x12ff038, &ptr, 8))
        return 0;
    if (ptr != block)
        return 0;
    if (!acc->read(acc->ctx, block, &vtable, 8))
        return 0;
    return vtable == acc->base + 0x11baa20;
}

/* ===== BSDQ diagnostics file read ===== */

/*
 * debug_quality_file_read — read back "bsd-debug-quality.bin" from the
 * diagnostics dir and validate it: regular file, owned by us, link count
 * 1 (high) and 0 (low) with no extra permission bits, exactly 0x18 bytes,
 * "BSDQ" magic, version 1, and a CRC32 (poly 0xedb88320, init/final
 * 0xffffffff/~) over the first 20 bytes matching the stored one. A
 * missing file (ENOENT) counts as ok with all values cleared to 0; a
 * tampered file fails. values[0] must be < 2 and the others < 4.
 * Returns 1 when the values were accepted into values[]. @ 00155a40
 */
int debug_quality_file_read(int dirfd, uint32_t values[3])
{
    struct stat st;
    uint8_t rec[24];
    uint64_t done = 0;
    ssize_t n;
    int fd;
    int saved_errno;

    if (dirfd < 0 || values == NULL)
        return 0;
    fd = __openat_2(dirfd, "bsd-debug-quality.bin", 0x88000 /* O_RDONLY|O_CLOEXEC|0x8000 */);
    if (fd < 0) {
        if (errno == 2 /* ENOENT */) {
            values[0] = 0;
            values[1] = 0;
            values[2] = 0;
            return 1;   /* missing file: nominal */
        }
        return 0;
    }
    if (fstat(fd, &st) != 0 || (st.st_mode & S_IFMT) != S_IFREG ||
        (uint32_t)st.st_uid != (uint32_t)getuid() ||
        (uint32_t)st.st_nlink != 1 || (st.st_nlink >> 32) != 0 ||
        (st.st_mode & 07777) != 0 || st.st_size != 0x18) {
        close(fd);
        return 0;
    }
    for (;;) {
        n = __read_chk(fd, rec + done, 0x18 - done, (size_t)-1);
        if (n < 0 && errno == EINTR)
            continue;
        if (n > 0) {
            done += (uint64_t)n;
            if (done < 0x18)
                continue;
        }
        break;
    }
    saved_errno = errno;
    close(fd);
    errno = saved_errno;
    if (done != 0x18)
        return 0;
    if (*(uint32_t *)(rec + 0) != 0x51445342 /* "BSDQ" */ ||
        *(uint32_t *)(rec + 4) != 1)
        return 0;
    if (*(uint32_t *)(rec + 20) != crc32_bytes(rec, 20))
        return 0;
    if (rec[8] >= 2 || rec[12] >= 4 || rec[16] >= 4)
        return 0;
    values[0] = *(uint32_t *)(rec + 8);
    values[1] = *(uint32_t *)(rec + 12);
    values[2] = *(uint32_t *)(rec + 16);
    return 1;
}

/* ===== delivery-module process-memory channel ===== */

/*
 * delivery_process_memory_find — dl_iterate_phdr callback: on
 * libNexusDelivery.so (second and later passes only reset the channel:
 * the fd is closed and set to -1), dlopen the object, dlsym
 * nexus_protected_process_memory, and accept it when dladdr confirms the
 * symbol lives in that object; the resolved function is then called with
 * the callback cookie and its result stored as the new channel fd.
 * Always returns 0 (keep iterating). @ 00155cf0
 */
int delivery_process_memory_find(struct dl_phdr_info *info, size_t size,
                                 void *data)
{
    struct {
        uint32_t cookie;  /* +0x00 callback cookie */
        int32_t  fd;      /* +0x04 channel fd */
        uint32_t passes;  /* +0x08 phdr passes seen */
    } *state = data;
    Dl_info where;
    const char *slash;
    void *handle;
    void *fn;

    (void)size;
    if (info->dlpi_name == NULL)
        return 0;
    slash = strrchr(info->dlpi_name, '/');
    if (slash == NULL || strcmp(slash + 1, "libNexusDelivery.so") != 0)
        return 0;
    if (state->passes++ != 0) {
        /* a second delivery object: drop the channel and stop using it */
        if (state->fd >= 0)
            close(state->fd);
        state->fd = -1;
        return 0;
    }
    handle = dlopen(info->dlpi_name, 6 /* RTLD_LAZY|RTLD_LOCAL */);
    if (handle == NULL)
        return 0;
    fn = dlsym(handle, "nexus_protected_process_memory");
    if (fn != NULL && dladdr(fn, &where) != 0 &&
        (uint64_t)where.dli_fbase == (uint64_t)info->dlpi_addr)
        state->fd = ((int (*)(uint32_t))fn)(state->cookie);
    dlclose(handle);
    return 0;
}

/*
 * bind_state_wake_signal — arm the global bind state at 0x22f040 by
 * calling bind_state_wake_set on it (sets its wake flag).
 * @ 00155dfc
 */
uint64_t g_bind_state;  /* 0x22f040 — bind state block (owned by p2/b-chain) */

void bind_state_wake_signal(void)
{
    bind_state_wake_set(&g_bind_state);
}
/* misc_a chain chunk 9: covers raw lines 4101-4600 */

#include <dirent.h>
#include <sys/stat.h>

/* ===== forward declarations within this chain ===== */
int  memfd_path_resolve(const char *memfd_name,
                        char *out);              /* below @ 001574ec */
int  memfd_module_open(const char *path, const char *name,
                       const uint8_t expect[32]); /* below @ 00157740 */

/* ===== cross-file functions ===== */
extern int  memfd_module_verify(void *env, const char *family,
                                const char *name,
                                const void *expect);  /* misc.c chain a @ 00169ae0 */
extern char *__strchr_chk(const char *s, int c, size_t s_len);  /* fortified strchr */
extern int   __open_2(const char *path, int flags);             /* fortified open */
extern char *__realpath64_chk; /* (unused placeholder removed) */

/*
 * files_dir_open — open the sealed "bsd.suitcase.nexusv2/files" directory
 * after path validation: path must be "/data/user/<digits>/bsd.suitcase.
 * nexusv2/files" (single-digit user id, no leading zero unless it is the
 * only digit) or "/data/data/bsd.suitcase.nexusv2/files", at most 0xfff
 * bytes, must resolve to itself through realpath (no symlinks), and both
 * the package dir and its "files" child must be directories (fstat) owned
 * by the effective uid. Returns the O_RDONLY|O_DIRECTORY|O_CLOEXEC fd of
 * the "files" dir, or -1. @ 00155e94
 */
int files_dir_open(char *path)
{
    char real[4096];
    char trimmed[4096];
    struct stat st;
    const char *pkg;
    size_t len;
    int fd, sub_fd;

    if (__strchr_chk("bsd.suitcase.nexusv2", '/', 0x15) != NULL)
        return -1;   /* the sealed name must not contain '/' */
    if (strncmp(path, "/data/user/", 0xb) == 0) {
        size_t digits = 0;

        while ((unsigned char)path[0xb + digits] - 0x30 < 10)
            digits++;
        /* single non-'0'-prefixed digit followed by '/' */
        if (digits != 1 || path[0xb] == '/' ||
            (0xc <= 0xb + digits && path[0xb] == '0'))
            return -1;
        pkg = path + 0xb + digits + 1;
    } else if (strncmp(path, "/data/data/", 0xb) == 0) {
        pkg = path + 0xb;
    } else {
        return -1;
    }
    len = strlen(path);
    if (strncmp(pkg, "bsd.suitcase.nexusv2", 0x14) != 0)
        return -1;
    if (strcmp(pkg + 0x14, "/files") != 0)
        return -1;
    if (0xfff < len)
        return -1;
    if (realpath(path, real) == NULL)
        return -1;
    if (strcmp(path, real) != 0)
        return -1;   /* symlinked path: refuse */

    /* open "<path minus 'files'>" (the package dir), then its "files" */
    memcpy(trimmed, path, len - 6);   /* drop "files" (6 bytes) */
    trimmed[len - 6] = 0;
    fd = __open_2(trimmed, 0x8c000 /* O_RDONLY|O_DIRECTORY|O_CLOEXEC|0x8000 */);
    if (fd < 0)
        return -1;
    if (fstat(fd, &st) != 0 || (st.st_mode & S_IFMT) != S_IFDIR ||
        (uint32_t)st.st_uid != (uint32_t)geteuid()) {
        close(fd);
        return -1;
    }
    sub_fd = __openat_2(fd, "files", 0x8c000);
    close(fd);
    if (sub_fd < 0)
        return -1;
    if (fstat(sub_fd, &st) != 0 || (st.st_mode & S_IFMT) != S_IFDIR ||
        (uint32_t)st.st_uid != (uint32_t)geteuid()) {
        close(sub_fd);
        return -1;
    }
    return sub_fd;
}

/*
 * dlclose_thunk — plain dlclose wrapper. @ 00156c48
 */
void dlclose_thunk(void *handle)
{
    dlclose(handle);
}

/*
 * memfd_module_callback — dl_iterate_phdr callback for the memfd module
 * scan: resolve the object's fd path with memfd_path_resolve; only
 * "/memfd:nexus-… (deleted)" links are considered (prefix must match
 * 0x17 bytes "/memfd:nexus-d:" and the suffix must be exactly
 * " (deleted)", total length < 0x117). The extracted tag (bytes 0x17..
 * len-0xa) must be <= 0x41 bytes and match the wanted tag; the module is
 * then verified with memfd_module_verify (family "bsd.suitcase.nexusv2",
 * tag, fd from the phdr info). On success the module path and load bias
 * are recorded into state->path/state->base and the count capped at 2.
 * Always returns 0 (keep iterating). @ 00156d68
 */
int memfd_module_callback(struct dl_phdr_info *info, size_t size, void *data)
{
    struct {
        uint64_t base;     /* +0x00 load bias to record */
        uint32_t fd;       /* +0x08 fd from the phdr info */
        uint32_t count;    /* +0x10 matches accepted (capped at 2) */
        const char *tag;   /* +0x18 wanted memfd tag */
        char path[0x1000]; /* +0x20 recorded module path */
    } *state = data;
    char fd_path[64];
    char link[0x140];
    char tag[0x100];
    ssize_t n;

    (void)size;
    if (memfd_path_resolve(info->dlpi_name, fd_path) == 0)
        return 0;
    n = readlink(fd_path, link, 0x13f);
    if (n < 0x18 || 0x13f < n)
        return 0;
    link[n] = 0;
    if (memcmp(link, "/memfd:nexus-d:" /* 0x17 bytes, 0x6e3a64666d656d2f… */,
               0x17) != 0)
        return 0;
    if (strcmp(link + n - 0xa, " (deleted)") != 0)
        return 0;
    if ((size_t)(n - 0x17) >= 0x100)
        return 0;
    memcpy(tag, link + 0x17, (size_t)(n - 0x17));
    tag[n - 0x17] = 0;
    if (0x41 < strlen(tag))
        return 0;
    if (strcmp(tag, state->tag) != 0)
        return 0;
    if (state->count < 2)
        state->count++;
    if (memfd_module_verify(NULL, "bsd.suitcase.nexusv2",
                            info->dlpi_name, NULL) != 0 &&
        strlen(info->dlpi_name) < 0x1000) {
        memcpy(state->path, info->dlpi_name, strlen(info->dlpi_name) + 1);
        state->base = (uint64_t)info->dlpi_addr;
    }
    return 0;
}

/*
 * mapped_file_hash_check — stream a remote file through the channel and
 * compare its SHA-256. mod is the module record (name at +0, tag at +8,
 * fd cookie at +0x10, path pointer at +0x20); memfd_module_open must
 * yield a regular file owned by us with the expected dev/ino pair, size
 * == len and zeroed link count. The file is read in <= 0x4000-byte
 * chunks (pread64 or process_vm_readv on self, EINTR retried), each chunk
 * fed into a SHA-256 context; a short read fails. The dev/ino/size are
 * re-stat'ed afterwards and must be unchanged. Returns 1 when the digest
 * matches expect (4 words), 0 otherwise. @ 00156f1c
 */
int mapped_file_hash_check(void *mod, const uint64_t expect[4], uint64_t len)
{
    struct stat st_before, st_after;
    uint8_t chunk[0x4000];
    uint8_t state[112]; /* SHA-256 context (sha256_state_t layout) */
    uint64_t digest[4];
    uint64_t done = 0;
    size_t n;
    long r;
    int fd;

    /* raw call: memfd_module_open(mod+0x20 fields, mod[0], mod[1]) */
    {
        const char *const *fields = mod;

        fd = memfd_module_open(fields[4], (const char *)fields[0],
                               (const uint8_t *)fields[1]);
    }
    if (fd < 0)
        return 0;
    if (fstat(fd, &st_before) != 0 ||
        (st_before.st_mode & S_IFMT) != S_IFREG ||
        (uint32_t)st_before.st_uid != (uint32_t)getuid() ||
        st_before.st_nlink != 0 || (uint64_t)st_before.st_size != len) {
        close(fd);
        return 0;
    }

    sha256_init((sha256_state_t *)state);
    while (done < len) {
        n = (size_t)(len - done);
        if (0x3fff < n)
            n = 0x4000;
        for (;;) {
            if (fd != 0x7fff0001)
                r = syscall(0x43 /* arm64 pread64 */, (uint64_t)fd, chunk,
                            (uint64_t)n, done);
            else
                r = syscall(0x10e /* process_vm_readv */, (long)getpid(),
                            chunk, 1UL, &done, 1UL, 0UL); /* simplified: local
                                                      * iov over the chunk */
            if (r >= 0)
                break;
            if (errno != EINTR)
                break;
        }
        if (r < 0)
            break;
        if (r == 0)
            break;   /* short read: fail */
        done += (uint64_t)r;
        sha256_update((sha256_state_t *)state, chunk, (size_t)r);
        if (len <= done)
            break;
    }
    if (fstat(fd, &st_after) != 0 ||
        st_after.st_dev != st_before.st_dev ||
        st_after.st_ino != st_before.st_ino ||
        (uint64_t)st_after.st_size != len ||
        st_after.st_nlink != st_before.st_nlink) {
        close(fd);
        sha256_final((sha256_state_t *)state, digest);
        return 0;
    }
    close(fd);
    sha256_final((sha256_state_t *)state, digest);
    if (st_after.st_nlink == 0 && done == len)
        return digest[0] == expect[0] && digest[1] == expect[1] &&
               digest[2] == expect[2] && digest[3] == expect[3];
    return 0;
}

/*
 * memfd_path_resolve — resolve a path to a concrete /proc/self/fd entry:
 * a path that already is "/proc/self/fd/<digits>" (length < 0x40) is
 * copied verbatim; a "/memfd:nexus-…" name (length < 0x140) is matched by
 * scanning /proc/self/fd readlinks — the first unique match (or a second
 * entry sharing the first match's dev/ino, which is then preferred as the
 * older one) wins; ambiguity without resolution returns 0. Returns 1 with
 * out filled, 0 otherwise. @ 001574ec
 */
int memfd_path_resolve(const char *path, char *out)
{
    char fd_path[64];
    char link[0x140];
    struct dirent *de;
    struct stat st;
    DIR *dir;
    ino_t first_ino = 0;
    dev_t first_dev = 0;
    bool have_match = false, ambiguous = false, prev_ambiguous = false;
    const char *s;
    size_t len;
    size_t i;

    if (path == NULL)
        return 0;
    if (strncmp(path, "/proc/self/fd/", 0xe) == 0 && path[0xe] != 0) {
        for (i = 0xf;; i++) {
            unsigned char c = (unsigned char)path[i - 1];

            if ((unsigned int)(c - 0x3a) >= 0xfffffff6u && c != 0)
                break;   /* not a digit */
            if (path[i] == 0)
                break;
        }
        len = strlen(path);
        if (len < 0x40) {
            strcpy(out, path);
            return 1;
        }
        return 0;
    }
    if (strncmp(path, "/memfd:nexus-", 0xd) != 0)
        return 0;
    len = strlen(path);
    if (0x140 <= len)
        return 0;
    dir = opendir("/proc/self/fd");
    if (dir == NULL)
        return 0;
    for (;;) {
        de = readdir(dir);
        if (de == NULL)
            break;
        s = de->d_name;
        if (*s == '.' || 0x15 <= strlen(s))
            continue;
        str_format(fd_path, 0x40, 0x40, "/proc/self/fd/%s", s);
        if (readlink(fd_path, link, 0x13f) < 0)
            continue;
        link[readlink(fd_path, link, 0x13f)] = 0;
        if (strcmp(link, path) != 0)
            continue;
        if (stat(fd_path, &st) != 0) {
            ambiguous = true;
            prev_ambiguous = ambiguous;
            continue;
        }
        if (have_match) {
            /* second match: prefer the earlier (kept) one when the ids
             * differ; equal dev/ino means the same file via another fd */
            if (st.st_dev == first_dev) {
                if (st.st_ino == first_ino) {
                    strcpy(out, fd_path);   /* prefer the newest alias */
                    have_match = true;
                    first_ino = st.st_ino;
                    first_dev = st.st_dev;
                    ambiguous = prev_ambiguous;
                    continue;
                }
                ambiguous = true;
            } else {
                ambiguous = true;
            }
            prev_ambiguous = ambiguous;
        } else {
            strcpy(out, fd_path);
            have_match = true;
            first_ino = st.st_ino;
            first_dev = st.st_dev;
            ambiguous = false;
        }
        prev_ambiguous = ambiguous;
        if (ambiguous)
            break;
    }
    closedir(dir);
    return have_match && !ambiguous;
}
/* misc_a chain chunk 10: covers raw lines 4601-5111 (straddler region of
 * the interrupted increment: memfd_module_open @ 00157740 started at raw
 * 4594 and is finished here through raw 4717; then jni_pending_records_
 * publish @ 00157b0c begins at raw 4719) */

#include <sys/stat.h>

/* ===== cross-file functions ===== */

/*
 * memfd_module_open — resolve and validate the fd of a loaded memfd
 * module. path is resolved with memfd_path_resolve and its readlink
 * target must be a deleted "/memfd:nexus-d:" file (0x17-byte prefix,
 * " (deleted)" suffix, tag < 0x100 bytes). For "script-" tags the module
 * is a script bundle: name must end in ".js" or ".ngs", the tag must be
 * exactly 0x27 chars with the last 0x20 lowercase-hex, and the expected
 * sha256 (0x40 hex chars) must match the given digest bytes. For other
 * tags the tag must be < 0x42 chars starting with '-', all 0x40 chars
 * lowercase-hex, the name must match the recorded one and the expected
 * sha256 must equal the given 32-byte digest. The fd number is parsed
 * from the resolved /proc/self/fd/<n> path (strtol, 0..0x7fffffff,
 * fully consumed), reopened with fcntl(F_DUPFD, 0), and the dup must be a
 * regular file owned by us with link count 0 and fd flags 0xf (RDONLY).
 * Returns the dup'ed fd, or -1. @ 00157740
 */
int memfd_module_open(const char *path, const char *name,
                      const uint8_t expect[32])
{
    char fd_path[14];
    char link[0x140];
    char tag[0x100];
    struct stat st;
    char *end;
    long fd_val, dup_fd;
    long flags;
    ssize_t n;
    size_t len, i;

    if (memfd_path_resolve(path, fd_path) == 0)
        return -1;
    n = readlink(fd_path, link, 0x13f);
    if (n < 0x18 || 0x13f < n)
        return -1;
    link[n] = 0;
    if (memcmp(link, "/memfd:nexus-d:" /* 0x6e3a64666d656d2f… */, 0x17) != 0)
        return -1;
    if (strcmp(link + n - 0xa, " (deleted)") != 0)
        return -1;
    if ((size_t)(n - 0x17) >= 0x100)
        return -1;
    memcpy(tag, link + 0x17, (size_t)(n - 0x17));
    tag[n - 0x17] = 0;
    len = strlen(tag);

    if (len >= 4 && memcmp(tag, "scri", 4) == 0 &&
        memcmp(tag, "script-", 7) == 0) {
        /* script bundle module */
        if (name == NULL || expect == NULL)
            return -1;
        if (strlen((const char *)expect) != 0x40 || strlen(name) <= 2)
            return -1;
        if (strcmp(name + strlen(name) - 3, ".js") != 0 &&
            !(strlen(name) > 3 &&
              strcmp(name + strlen(name) - 4, ".ngs") == 0))
            return -1;
        if (len != 0x27)
            return -1;
        for (i = 0; i < 0x20; i++) {
            unsigned c = (unsigned char)tag[7 + i];

            if (9 < c - 0x30 && 5 < c - 0x61)  /* not [0-9a-f] */
                return -1;
        }
        goto open_fd;
    }

    if (0x42 <= len || tag[4] != '-')
        return -1;
    for (i = 0; i < 0x40; i++) {
        unsigned c = (unsigned char)tag[i];

        if (9 < c - 0x30 && 5 < c - 0x61)  /* not [0-9a-f] */
            return -1;
    }
    if (name != NULL && strcmp(name, tag + 5) != 0)
        return -1;
    if (expect != NULL) {
        /* the expected digest is 32 raw bytes; the tag carries it as
         * 0x40 hex chars — compare the hex rendering */
        char hex[0x41];
        int j;

        for (j = 0; j < 32; j++)
            str_format(hex + 2 * j, (size_t)-1, 3, "%02x", expect[j]);
        if (memcmp(hex, tag, 0x40) != 0)
            return -1;
    }

open_fd:
    if (memfd_path_resolve(path, fd_path) == 0)
        return -1;
    errno = 0;
    fd_val = strtol(fd_path + 0xe /* past "/proc/self/fd/" */, &end, 10);
    if (errno != 0 || end == NULL || *end != 0 || fd_val < 0 ||
        0x7fffffff < fd_val)
        return -1;
    dup_fd = fcntl((int)fd_val, 0x406 /* F_DUPFD */, 0);
    if (dup_fd < 0)
        return -1;
    flags = fcntl((int)dup_fd, 0x40a /* F_GETFD */);
    if (fstat((int)dup_fd, &st) == 0 &&
        (st.st_mode & S_IFMT) == S_IFREG &&
        (uint32_t)st.st_uid == (uint32_t)getuid() &&
        st.st_nlink == 0 && flags >= 0 && (flags & 0xf) == 0xf)
        return -1;   /* sealed-file checks failed (dump semantics) */
    close((int)dup_fd);
    return -1;
}
/* misc_a chain chunk 11: covers raw lines 4719-5200 */

/* ===== JNI vtable veneers (see chunk 1) ===== */
static long jni_call1_ma(void *obj, uint32_t slot, long a);
static long jni_call3_ma(void *obj, uint32_t slot, long a, long b, long c);
static long jni_call4_ma(void *obj, uint32_t slot, long a, long b, long c, long d);

/* ===== cross-file functions ===== */
extern uint32_t capability_bits_read(void *out48);  /* external @ 001592cc — not in dump */

/* ===== pending-records ring (shared with widgets.c) ===== */
static int g_pending_records_latch;       /* 0x22f500 — publish latch */
extern int32_t g_pending_record_count;   /* 0x233ecc — accepted record count */
extern int32_t g_pending_record_cap;     /* 0x233ee4 — nonzero = records exist */
extern int64_t g_pending_record_stamp;   /* 0x233ed0 — record identity stamp */
extern int64_t g_pending_record_field8;  /* 0x233ed8 */
extern int32_t g_pending_record_field10; /* 0x233ee0 */
extern uint32_t g_pending_records[0x1e0]; /* 0x22f504 — 32-slot ring: id */
extern int64_t g_pending_arg_a[0x20];    /* 0x22f584 — per-slot arg a */
extern int64_t g_pending_arg_b[0x20];    /* 0x22f604 — per-slot arg b */
extern uint8_t g_pending_flag_a[0x20];   /* 0x22f684 — per-slot flag a */
extern uint8_t g_pending_flag_b[0x20];   /* 0x22f6a4 — per-slot flag b */
extern uint32_t g_ring_commit_count;     /* 0x22f6c4 — ring head (mod 32) */
extern uint32_t g_ring_pending_count;    /* 0x22f6c8 — ring entries in flight */
extern const uint32_t g_records_template[0x1e0]; /* 0x233f00 — staging copy */

/* rodata: NEON range-check lane pairs for the event-id gate */
extern const int32_t EVENT_ID_RANGES_A[2];  /* 0x10fa70/74 — inclusive lo/hi */
extern const int32_t EVENT_ID_RANGES_B[2];  /* 0x10fa78/7c */

/*
 * jni_pending_records_publish — publish the staged pending-event records
 * to Java. Under the 0x22f500 latch: the staging block (count at
 * 0x233ee0, stamp pair 0x233ed0/ed8, then count * 9 words of records)
 * is copied out, a String[count * 9 + 8] array is allocated and filled:
 * element 0 "NXOD1", then the stamp rendered as 4 hex words ("%%08x",
 * rodata 0x133bc0), then per record 4 hex words plus the raw 4th/8th
 * fields as strings. A PushLocalFrame(0x10) scope is used; any failure
 * clears the pending exception and pops the frame. Returns the array, or
 * 0. @ 00157b0c
 */
long jni_pending_records_publish(void *env)
{
    char hex[32];
    long arr, cls, str;
    uint32_t count;
    long *words;

    if ((ui_latch_test_and_set(1, &g_pending_records_latch) & 1) != 0)
        return 0;
    if (g_pending_record_cap == 0) {
        g_pending_records_latch = 0;
        return 0;
    }
    count = (uint32_t)g_pending_record_count;
    if (count * 9 >= 0x1201) {
        g_pending_records_latch = 0;
        return 0;
    }

    /* snapshot the staging block */
    words = __builtin_alloca(sizeof(long) * (4 + (size_t)count * 9));
    words[0] = 1;
    words[2] = g_pending_record_stamp;
    words[1] = g_pending_record_field8;
    words[3] = g_pending_record_field10;
    words[4] = (long)(uint32_t)count;
    if (count != 0)
        memcpy(words + 4, g_records_template, (size_t)count * 9 * sizeof(long));
    g_pending_records_latch = 0;

    cls = jni_call1_ma(env, 0x30 /* FindClass */, (long)"java/lang/String");
    if (cls == 0)
        return 0;
    arr = jni_call3_ma(env, 0x560 /* NewObjectArray */,
                       count * 5 + 5, cls, 0);
    jni_call1_ma(env, 0xb8 /* DeleteLocalRef */, cls);
    if (arr == 0)
        goto fail;

    /* the four stamp words rendered as hex (rodata 0x133bc0 "%%08x") */
    str_format(hex, 0x20, 0x20, "%08x", (uint32_t)((uint64_t)words[2] & 0xffffffffu));
    if (jni_call1_ma(env, 0x98 /* PushLocalFrame */, 0x10) != 0)
        goto fail_pop;

    str = jni_call1_ma(env, 0x538 /* NewStringUTF */, (long)"NXOD1");
    if (str == 0)
        goto fail_frame;
    jni_call4_ma(env, 0x570 /* SetObjectArrayElement */, arr, 0, str, 0);
    jni_call1_ma(env, 0xb8, str);
    /* elements 1..4: the four hex words (rendered above in sequence) */
    {
        const uint32_t stamp_words[4] = {
            (uint32_t)((uint64_t)words[2] & 0xffffffffu),
            (uint32_t)((uint64_t)words[2] >> 32),
            (uint32_t)((uint64_t)words[1] & 0xffffffffu),
            (uint32_t)((uint64_t)words[1] >> 32),
        };
        char buf[32];
        int k;

        for (k = 0; k < 4; k++) {
            str_format(buf, k == 0 ? 0x20 : 0x10, k == 0 ? 0x20 : 0x10,
                       "%08x", stamp_words[k]);
            str = jni_call1_ma(env, 0x538, (long)buf);
            if (str == 0)
                goto fail_frame;
            jni_call4_ma(env, 0x570, arr, k + 1, str, 0);
            jni_call1_ma(env, 0xb8, str);
        }
    }
    if ((uint32_t)((uint64_t)words[1] >> 32) != 0) {
        char buf[32];
        uint32_t j;
        long *rec;
        int slot = 9;

        for (j = 0, rec = words + 4; j < count; j++, rec += 9) {
            uint32_t w;

            w = (uint32_t)rec[0];
            str_format(buf, 0x10, 0x10, "%08x", w);
            str = jni_call1_ma(env, 0x538, (long)buf);
            if (str == 0) goto fail_frame;
            jni_call4_ma(env, 0x570, arr, slot - 4, str, 0);
            jni_call1_ma(env, 0xb8, str);
            w = (uint32_t)((uint64_t)rec[0] >> 32);
            str_format(buf, 0x10, 0x10, "%08x", w);
            str = jni_call1_ma(env, 0x538, (long)buf);
            if (str == 0) goto fail_frame;
            jni_call4_ma(env, 0x570, arr, slot - 3, str, 0);
            jni_call1_ma(env, 0xb8, str);
            str = jni_call1_ma(env, 0x538, (long)(char *)&rec[1]);
            if (str == 0) goto fail_frame;
            jni_call4_ma(env, 0x570, arr, slot - 2, str, 0);
            jni_call1_ma(env, 0xb8, str);
            str = jni_call1_ma(env, 0x538, (long)(char *)&rec[2]);
            if (str == 0) goto fail_frame;
            jni_call4_ma(env, 0x570, arr, slot - 1, str, 0);
            jni_call1_ma(env, 0xb8, str);
            w = (uint32_t)rec[3];
            str_format(buf, 0x10, 0x10, "%08x", w);
            str = jni_call1_ma(env, 0x538, (long)buf);
            if (str == 0) goto fail_frame;
            jni_call4_ma(env, 0x570, arr, slot, str, 0);
            jni_call1_ma(env, 0xb8, str);
            slot += 5;
        }
    }
    jni_call1_ma(env, 0xa0 /* PopLocalFrame */, 0);
    return arr;

fail_frame:
    if (jni_call0_ma(env, 0x720 /* ExceptionCheck */) != 0)
        jni_call0_ma(env, 0x88 /* ExceptionClear */);
fail_pop:
    jni_call1_ma(env, 0xa0 /* PopLocalFrame */, 0);
fail:
    jni_call1_ma(env, 0xb8 /* DeleteLocalRef */, arr);
    return 0;
}

/*
 * command_ring_push — push one event onto the 32-slot pending-command
 * ring. event ids outside the accepted windows (NEON lane check against
 * the 0x10fa70/78 rodata ranges plus the explicit id checks) return 4.
 * Event 0x82 clears the ring instead. Other events need the staging block
 * to exist, the stamp to match, and the ring not to overflow (0x1f in
 * flight). On success the slot fields are zeroed, flag_a records
 * "event == 0x82", and the event/arg pair is stored; returns 2.
 * @ 00157c88
 */
uint32_t command_ring_push(void *ctx, uint64_t arg, uint32_t event,
                           int32_t stamp)
{
    uint32_t slot;
    int lane;

    (void)ctx;
    (void)arg;
    if ((int32_t)event < 1)
        return 4;
    /* NEON cmhi lane check: event must fall outside both rodata ranges */
    lane = (int32_t)(event + EVENT_ID_RANGES_A[0]) < EVENT_ID_RANGES_A[1] ||
           (int32_t)(event + EVENT_ID_RANGES_B[0]) < EVENT_ID_RANGES_B[1];
    if (((lane & 1) == 0 && 2 < event - 0x20000) && 0xa7 < event &&
        (event & 0xffffffc0) != 0x26000 && (event & 0xfffffe00) != 0x29000 &&
        0x90 < event - 0x2e101 && (event & 0xfffffffc) != 0x2e000 &&
        0x15 < event - 0x2e010)
        return 4;
    if ((ui_latch_test_and_set(1, &g_pending_records_latch) & 1) != 0)
        return 3;
    if (event == 0x82) {
        g_ring_pending_count = 0;
        g_ring_commit_count = 0;
    } else {
        if (g_pending_record_cap == 0) {
            g_pending_records_latch = 0;
            return 3;
        }
        if (g_pending_record_stamp != stamp) {
            g_pending_records_latch = 0;
            return 3;
        }
        if (0x1f < g_ring_pending_count) {
            g_pending_records_latch = 0;
            return 3;
        }
    }
    slot = (g_ring_pending_count + g_ring_commit_count) & 0x1f;
    g_pending_flag_b[slot] = 0;
    g_pending_flag_a[slot] = event == 0x82;
    g_pending_records[slot] = event;
    g_pending_arg_a[slot] = stamp;
    g_pending_arg_b[slot] = 0;
    g_pending_records_latch = 0;
    g_ring_pending_count++;
    return 2;
}

/*
 * command_ring_push_value — like command_ring_push but carries a 4-byte
 * value in the slot's arg_b field, and flags the slot as externally
 * pushed (flag_a = 1, flag_b = 0). The accepted-event window is the
 * complement of command_ring_push's (in-range events only). Returns 2 on
 * success, 3 when gated (no staging block, stamp mismatch, ring full or
 * latch held), 4 when the event is out of range. @ 00157e00
 */
uint32_t command_ring_push_value(void *ctx, uint64_t arg, uint32_t event,
                                 uint32_t value, int32_t stamp)
{
    uint32_t slot;
    int lane;

    (void)ctx;
    (void)arg;
    if ((int32_t)event <= 0)
        return 4;
    lane = (int32_t)(event + EVENT_ID_RANGES_A[0]) < EVENT_ID_RANGES_A[1] ||
           (int32_t)(event + EVENT_ID_RANGES_B[0]) < EVENT_ID_RANGES_B[1];
    if ((lane & 1) == 0 || event - 0x20000 < 3 || event < 0xa8 ||
        (event & 0xffffffc0) == 0x26000 || (event & 0xfffffe00) == 0x29000 ||
        event - 0x2e101 < 0x91 || (event & 0xfffffffc) == 0x2e000 ||
        event - 0x2e010 < 0x16)
        return 4;
    if ((ui_latch_test_and_set(1, &g_pending_records_latch) & 1) != 0)
        return 3;
    if (g_pending_record_cap != 0 && g_pending_record_stamp == stamp &&
        g_ring_pending_count < 0x20) {
        slot = (g_ring_commit_count + g_ring_pending_count) & 0x1f;
        g_ring_pending_count++;
        g_pending_flag_b[slot] = 1;
        g_pending_flag_a[slot] = 0;
        g_pending_records[slot] = event;
        g_pending_arg_a[slot] = stamp;
        g_pending_arg_b[slot] = value;
        g_pending_records_latch = 0;
        return 2;
    }
    g_pending_records_latch = 0;
    return 3;
}
/* misc_a chain chunk 12: covers raw lines 5201-5700 */

/* ===== JNI vtable veneers (see chunk 1) ===== */
static long jni_call2_ma(void *obj, uint32_t slot, long a, long b);

/* forward declarations within this chain */
size_t str_to_java_utf8(char *out, size_t cap,
                        const char *in);              /* below @ 00158f70 */
bool protected_channel_ready(void);                   /* below @ 0015922c */
bool nri1_header_valid(const int32_t *hdr);           /* below @ 001597b8 */

/* ===== overlay-wire diagnostic staging (filled by widgets.c) ===== */
extern int g_overlay_diag_latch;             /* 0x233eec — staging latch */
extern uint64_t g_overlay_diag_stamp;        /* 0x233ef0 — stamp pair */
extern uint64_t g_overlay_diag_records;      /* 0x233ef8 — {count lo, cap hi} */
extern const uint64_t g_overlay_diag_template[0x75]; /* 0x233f00 — staging copy
                                                      * (0x75 * 9 = 0x270/8 words) */

/* ===== overlay-wire status blob staging ===== */
int g_wire_blob_latch;                       /* 0x237e60 — blob staging latch */
extern const int32_t g_wire_blob_staging[];  /* 0x237e64 — 0x24a68-byte blob */
uint32_t g_wire_row_count_cache;             /* 0x25c8cc — cached row count */

/* ===== online-diagnostics string table ===== */
extern const char g_diag_hex_fmt[];   /* 0x133bc0 — "%08x" */
extern const char g_diag_str_fmt[];   /* 0x13c694 — string field format */

/*
 * jni_online_diag_publish — publish the online diagnostics staging block
 * to Java as a String[] array. Under the 0x233eec latch: the {stamp,
 * records} header and count * 0x270 bytes of records are copied out, a
 * String[count * 5 + 5] array is allocated ("java/lang/String") and
 * filled: element 0 "NXOD1", elements 1..4 the stamp words rendered as
 * hex ("%08x"), then per record five entries (four hex words plus the
 * 4th/8th-field strings). Uses a PushLocalFrame(0x10) scope and releases
 * every element. Returns the array, or 0. @ 00157f58
 */
long jni_online_diag_publish(void *env)
{
    char buf[32];
    uint64_t header[3];
    uint64_t *records;
    uint32_t count, i;
    long cls, arr, str;
    int slot;

    memset(buf, 0, sizeof buf);
    if ((ui_latch_test_and_set(1, &g_overlay_diag_latch) & 1) != 0)
        return 0;
    count = (uint32_t)(g_overlay_diag_records >> 32);
    header[0] = g_overlay_diag_stamp;
    header[1] = g_overlay_diag_records & 0xffffffffu;
    header[2] = g_overlay_diag_records >> 32;
    if (count != 0)
        records = __builtin_alloca((size_t)count * 0x270);
    else
        records = NULL;
    if (count != 0)
        memcpy(records, g_overlay_diag_template, (size_t)count * 0x270);
    g_overlay_diag_latch = 0;

    cls = jni_call1_ma(env, 0x30 /* FindClass */, (long)"java/lang/String");
    if (cls == 0)
        return 0;
    arr = jni_call3_ma(env, 0x560 /* NewObjectArray */,
                       (long)((uint64_t)header[2] * 5 + 5), cls, 0);
    jni_call1_ma(env, 0xb8 /* DeleteLocalRef */, cls);
    if (arr == 0)
        return 0;
    if (jni_call1_ma(env, 0x98 /* PushLocalFrame */, 0x10) != 0)
        goto fail;

    str = jni_call1_ma(env, 0x538 /* NewStringUTF */, (long)"NXOD1");
    if (str == 0)
        goto fail_frame;
    jni_call4_ma(env, 0x570 /* SetObjectArrayElement */, arr, 0, str, 0);
    jni_call1_ma(env, 0xb8, str);
    {
        const uint32_t stamp_words[4] = {
            (uint32_t)(header[0] & 0xffffffffu),
            (uint32_t)(header[0] >> 32),
            (uint32_t)(header[1] & 0xffffffffu),
            (uint32_t)(header[1] >> 32),
        };
        int k;

        for (k = 0; k < 4; k++) {
            str_format(buf, 0x20, 0x20, "%08x", stamp_words[k]);
            str = jni_call1_ma(env, 0x538, (long)buf);
            if (str == 0)
                goto fail_frame;
            jni_call4_ma(env, 0x570, arr, k + 1, str, 0);
            jni_call1_ma(env, 0xb8, str);
        }
    }
    if ((uint32_t)(header[1] >> 32) != 0) {
        slot = 9;
        for (i = 0; i < count; i++) {
            uint64_t *rec = (uint64_t *)((char *)records + (size_t)i * 0x270);
            uint32_t w;

            w = (uint32_t)rec[0];
            str_format(buf, 0x10, 0x10, "%08x", w);
            str = jni_call1_ma(env, 0x538, (long)buf);
            if (str == 0) goto fail_frame;
            jni_call4_ma(env, 0x570, arr, slot - 4, str, 0);
            jni_call1_ma(env, 0xb8, str);
            w = (uint32_t)(rec[0] >> 32);
            str_format(buf, 0x10, 0x10, "%08x", w);
            str = jni_call1_ma(env, 0x538, (long)buf);
            if (str == 0) goto fail_frame;
            jni_call4_ma(env, 0x570, arr, slot - 3, str, 0);
            jni_call1_ma(env, 0xb8, str);
            str = jni_call1_ma(env, 0x538, (long)(char *)&rec[1]);
            if (str == 0) goto fail_frame;
            jni_call4_ma(env, 0x570, arr, slot - 2, str, 0);
            jni_call1_ma(env, 0xb8, str);
            str = jni_call1_ma(env, 0x538, (long)(char *)&rec[2]);
            if (str == 0) goto fail_frame;
            jni_call4_ma(env, 0x570, arr, slot - 1, str, 0);
            jni_call1_ma(env, 0xb8, str);
            w = (uint32_t)rec[3];
            str_format(buf, 0x10, 0x10, "%08x", w);
            str = jni_call1_ma(env, 0x538, (long)buf);
            if (str == 0) goto fail_frame;
            jni_call4_ma(env, 0x570, arr, slot, str, 0);
            jni_call1_ma(env, 0xb8, str);
            slot += 5;
        }
    }
    jni_call1_ma(env, 0xa0 /* PopLocalFrame */, 0);
    return arr;

fail_frame:
    if (jni_call0_ma(env, 0x720 /* ExceptionCheck */) != 0)
        jni_call0_ma(env, 0x88 /* ExceptionClear */);
    jni_call1_ma(env, 0xa0 /* PopLocalFrame */, 0);
    return 0;
fail:
    return 0;
}

/*
 * jni_overlay_wire_publish — publish the 0x24a68-byte overlay-wire status
 * blob (magic 1, size 0x24a68, up to 0x100 rows) to Java as a String[]
 * array of count * 0xb + 0x1f entries. Row 0 is "NXOS1"; rows 1..0x1e are
 * the header words (rendered as hex "%08x" or with the string format
 * 0x13c694 depending on the slot), rows 0x1f.. are the 0x92-byte wire
 * rows (up to 0x100, 6 fixed fields then string fields). Every element
 * is released; an exception between elements aborts. The blob is copied
 * out of the staging area under the 0x237e60 latch. Returns the array,
 * or 0. @ 00158500
 */
long jni_overlay_wire_publish(void *env)
{
    char buf[32];
    int32_t *blob;
    uint32_t count, row;
    long cls, arr, str;

    blob = malloc(0x24a68);
    if (blob == NULL)
        return 0;
    if ((ui_latch_test_and_set(1, &g_wire_blob_latch) & 1) != 0) {
        free(blob);
        return 0;
    }
    memcpy(blob, g_wire_blob_staging, 0x24a68);
    g_wire_row_count_cache = (uint32_t)blob[0xc];  /* 0x25c8cc cache */
    g_wire_blob_latch = 0;

    if (blob[0] != 1 || blob[1] != 0x24a68) {
        free(blob);
        return 0;
    }
    count = (uint32_t)blob[0xc];
    if (0x100 < count)
        count = (uint32_t)blob[0xc];  /* gate below re-checks */
    cls = jni_call1_ma(env, 0x30 /* FindClass */, (long)"java/lang/String");
    if (cls == 0) {
        free(blob);
        return 0;
    }
    arr = jni_call3_ma(env, 0x560 /* NewObjectArray */,
                       (long)(count * 0xb + 0x1f), cls, 0);
    jni_call1_ma(env, 0xb8 /* DeleteLocalRef */, cls);
    if (arr == 0) {
        free(blob);
        return 0;
    }

    for (row = 0; row < count * 0xb + 0x1f; row++) {
        /* re-validate the blob every iteration (raw 5126) */
        if (blob[0] != 1 || blob[1] != 0x24a68 ||
            0x100 < (uint32_t)blob[0xc] ||
            (uint32_t)blob[0xc] * 0xb + 0x1f <= row)
            goto abort_publish;
        if (row == 0) {
            str = jni_call1_ma(env, 0x538, (long)"NXOS1");
        } else if (row < 0x1f) {
            /* header word: hex or string format by slot */
            uint32_t w;
            const char *fmt = g_diag_hex_fmt;

            if (row < 0xe) {
                w = ((const uint32_t *)blob)[row];
            } else if (row < 0x13) {
                w = ((const uint32_t *)blob)[row];
                fmt = g_diag_str_fmt;
            } else if (row < 0x1a) {
                w = ((const uint32_t *)blob)[row];
            } else {
                str = jni_call1_ma(env, 0x538,
                                   (long)*(char **)((char *)blob +
                                                    4 * (0x1a + 0x20 * (row - 0x1a))));
                if (str == 0)
                    goto abort_publish;
                goto set_element;
            }
            str_format(buf, 0x20, 0x20, fmt, w);
            str = jni_call1_ma(env, 0x538, (long)buf);
        } else {
            /* wire rows: 0x92 bytes each, 0xb elements per row */
            uint32_t r = (row - 0x1f) / 0xb;
            uint32_t f = (row - 0x1f) % 0xb;
            const int32_t *rec = (const int32_t *)((char *)blob +
                                                   (size_t)r * 0x92 + 0x9e * 4);

            if (f < 6) {
                if (f == 4 || f == 2) {
                    str_format(buf, 0x20, 0x20, g_diag_str_fmt,
                               rec[(f == 2) ? -1 : 0]);
                } else {
                    uint64_t pair = *(const uint64_t *)((const char *)rec + 4 * f);

                    str_format(buf, 0x20, 0x20, g_diag_hex_fmt,
                               (uint32_t)((pair >> (32 * (f & 1))) & 0xffffffffu));
                }
                str = jni_call1_ma(env, 0x538, (long)buf);
            } else {
                str = jni_call1_ma(env, 0x538,
                                   (long)*(char **)((const char *)rec +
                                                    0x1c * (f - 6)));
            }
        }
        if (str == 0)
            goto abort_publish;
set_element:
        jni_call4_ma(env, 0x570 /* SetObjectArrayElement */, arr, row, str, 0);
        jni_call1_ma(env, 0xb8 /* DeleteLocalRef */, str);
        if (jni_call0_ma(env, 0x720 /* ExceptionCheck */) != 0)
            goto abort_publish;
    }
    free(blob);
    return arr;

abort_publish:
    free(blob);
    if (jni_call0_ma(env, 0x720 /* ExceptionCheck */) != 0)
        jni_call0_ma(env, 0x88 /* ExceptionClear */);
    jni_call1_ma(env, 0xb8 /* DeleteLocalRef */, arr);
    return 0;
}

/* ===== protected channel status ===== */

/*
 * jni_channel_status_publish — publish the protected-channel status to
 * Java: under the 0x1a7c08 latch the channel snapshot {1, revision, fn
 * pair, handle, flags, status} is built (with a second latch pass to
 * refresh it when the channel is captured) and written into a 10-element
 * int array (NewIntArray + SetIntArrayRegion); the publish fails when an
 * exception is pending. Returns the array, or 0. @ 001589b0
 */
long jni_channel_status_publish(void *env)
{
    int32_t snap[10];
    uint32_t ok = 1;
    long arr;

    if ((ui_latch_test_and_set(1, &g_pchan_latch) & 1) != 0)
        return 0;
    snap[0] = 1;
    snap[1] = g_pchan_counter;
    snap[2] = g_pchan_fn_a;
    snap[3] = g_pchan_fn_b;
    snap[4] = (int32_t)(g_pchan_handle & 0xffffffffu);
    snap[5] = (int32_t)(g_pchan_handle >> 32);
    snap[6] = g_pchan_flag_a;
    snap[7] = g_pchan_flag_b;
    snap[8] = (g_pchan_fn_b == 0 || g_pchan_fn_a == 0 || g_pchan_flag_a == 0 ||
               g_pchan_flag_c == 0 || g_pchan_flag_b != 0);
    snap[9] = g_pchan_field_18;
    g_pchan_latch = 0;
    if (g_channel_installed == 1 && g_channel_class != NULL &&
        g_channel_ready != 0 && g_channel_method != NULL &&
        (ui_latch_test_and_set(1, &g_pchan_latch) & 1) == 0) {
        g_pchan_latch = 0;
        ok = (g_pchan_fn_b == 0 || g_pchan_fn_a == 0 || g_pchan_flag_a == 0 ||
              g_pchan_flag_c == 0 || g_pchan_flag_b != 0);
    }
    snap[8] = (int32_t)ok;

    arr = jni_call1_ma(env, 0x598 /* NewIntArray */, 10);
    if (arr == 0)
        return 0;
    jni_call4_ma(env, 0x698 /* SetIntArrayRegion */, arr, 0, 10,
                 (long)(void *)snap);
    if (jni_call0_ma(env, 0x720 /* ExceptionCheck */) == 0) {
        jni_call0_ma(env, 0x88 /* ExceptionClear */);
        jni_call1_ma(env, 0xb8 /* DeleteLocalRef */, arr);
        return arr;
    }
    return 0;
}

/* ===== protected-channel flag setters ===== */

/*
 * protected_channel_flags_set — set both channel flags (a = flag_a == 1,
 * b = flag_b == 1) under the 0x1a7c08 latch (spinning while held). A
 * change of flag a also clears flag c (0x1a7c2b) and bumps the revision
 * (resetting it to 1 when it was -1). Returns 1, or 4 when either flag
 * is >= 2. @ 00158b60
 */
uint32_t protected_channel_flags_set(void *ctx, uint64_t arg,
                                     uint8_t flag_a, uint8_t flag_b)
{
    bool a = flag_a == 1;
    bool b = flag_b == 1;
    bool was_min1;

    (void)ctx;
    (void)arg;
    if ((flag_b | flag_a) >= 2)
        return 4;
    while ((ui_latch_test_and_set(1, &g_pchan_latch) & 1) != 0)
        ;
    if (g_pchan_flag_a != a || g_pchan_flag_b != b) {
        if (g_pchan_flag_a != a)
            g_pchan_flag_c = 0;   /* 0x1a7c2b */
        was_min1 = g_pchan_counter == -1;
        g_pchan_counter++;
        g_pchan_flag_a = a;
        g_pchan_flag_b = b;
        if (was_min1)
            g_pchan_counter = 1;
    }
    g_pchan_latch = 0;
    return 1;
}

/*
 * protected_channel_flag_set — set channel flag c (0x1a7c2a) under the
 * 0x1a7c08 latch (spinning while held); a change bumps the revision
 * (reset to 1 when it was -1). Returns 1, or 4 when flag >= 2.
 * @ 00158c1c
 */
uint32_t protected_channel_flag_set(void *ctx, uint64_t arg, uint8_t flag)
{
    bool c = flag == 1;
    bool was_min1;

    (void)ctx;
    (void)arg;
    if (flag >= 2)
        return 4;
    while ((ui_latch_test_and_set(1, &g_pchan_latch) & 1) != 0)
        ;
    if (g_pchan_flag_c != c) {
        was_min1 = g_pchan_counter == -1;
        g_pchan_counter++;
        g_pchan_flag_c = c;
        if (was_min1)
            g_pchan_counter = 1;
    }
    g_pchan_latch = 0;
    return 1;
}

/* ===== capability vector ===== */

/*
 * capability_bit_query — read bit `bit` (< 0x6c) of the 0x6c-bit
 * capability vector produced by the delivery module (capability_bits_
 * read, external). Returns the bit value, or 0xffffffff when the bit is
 * out of range or the vector could not be read. @ 00158ca4
 */
uint32_t capability_bit_query(void *ctx, uint64_t arg, uint32_t bit)
{
    uint8_t vector[12];   /* 0x6c bits = 3 words + header */
    uint32_t *words;

    (void)ctx;
    (void)arg;
    if (bit < 0x6c && capability_bits_read(vector) != 0) {
        words = (uint32_t *)vector;
        return (words[bit >> 5] >> (bit & 0x1f)) & 1;
    }
    return 0xffffffff;
}

/* ===== UTF-8 sanitization ===== */

/*
 * str_to_java_utf8 — copy a NUL-terminated UTF-8 string into out (cap
 * bytes, always NUL-terminated) while sanitizing it for JNI
 * NewStringUTF: modified-UTF-8 sequences are decoded (invalid sequences
 * become U+FFFD), control characters below 0x20 become spaces, and any
 * code point >= 0x10000 is re-encoded as a CESU-8 surrogate pair (two
 * 3-byte sequences with 0xed lead bytes). Stops at cap. Returns nothing;
 * the output is always terminated. @ 00158f70
 */
size_t str_to_java_utf8(char *out, size_t cap, const char *in)
{
    uint32_t cp;
    uint8_t seq[6];
    size_t used = 0, n, adv;
    const uint8_t *p = (const uint8_t *)in;

    if (in != NULL) {
        cp = *p;
        while (cp != 0) {
            if (cp >> 7 == 0) {
                adv = 1;
            } else if (((cp + 0x3e) & 0xff) < 0x1e) {
                if ((p[1] & 0xc0) != 0x80)
                    goto invalid;
                adv = 2;
                cp = (p[1] & 0x3f) | ((cp & 0x1f) << 6);
            } else if ((cp & 0xf0) == 0xe0) {
                uint8_t b1 = p[1];

                if ((b1 & 0xc0) != 0x80 || (p[2] & 0xc0) != 0x80 ||
                    (cp == 0xe0 && b1 < 0xa0) ||
                    (cp == 0xed && 0x9f < b1))
                    goto invalid;
                adv = 3;
                cp = ((cp & 0xf) << 0xc) | ((b1 & 0x3f) << 6) | (p[2] & 0x3f);
            } else {
                uint8_t b1 = p[1];

                if (4 < ((cp + 0x10) & 0xff) || (b1 & 0xc0) != 0x80 ||
                    (p[2] & 0xc0) != 0x80 || (p[3] & 0xc0) != 0x80 ||
                    (cp == 0xf0 && b1 < 0x90) ||
                    (cp == 0xf4 && 0x8f < b1))
                    goto invalid;
                adv = 4;
                cp = ((cp & 7) << 0x12) | ((b1 & 0x3f) << 0xc) |
                     ((p[2] & 0x3f) << 6) | (p[3] & 0x3f);
            }
            /* control characters become spaces */
            if (0xfffffffd < cp - 0xb || 0x1f < cp)
                cp = cp;
            else
                cp = 0x20;
            if (cp < 0x80) {
                n = 1;
                seq[0] = (uint8_t)cp;
            } else if (cp < 0x800) {
                n = 2;
                seq[0] = (uint8_t)(cp >> 6) | 0xc0;
                seq[1] = ((uint8_t)cp & 0x3f) | 0x80;
            } else if (cp >> 0x10 == 0) {
                n = 3;
                seq[0] = (uint8_t)(cp >> 0xc) | 0xe0;
                seq[1] = (uint8_t)((cp >> 6) & 0x3f) | 0x80;
                seq[2] = ((uint8_t)cp & 0x3f) | 0x80;
            } else {
                /* CESU-8 surrogate pair */
                uint32_t hi = (cp - 0x10000) >> 10;

                n = 6;
                seq[0] = 0xed;
                seq[1] = (uint8_t)((hi & 0x3f) | 0x80);
                seq[2] = (uint8_t)(((hi + 0x800) >> 6) | 0x80);
                seq[3] = (uint8_t)(((cp >> 6) & 0xf) | 0xb0);
                seq[4] = ((uint8_t)cp & 0x3f) | 0x80;
                seq[5] = 0xed;
            }
            if (cap <= used + n)
                break;
            memcpy(out + used, seq, n);
            p += adv;
            used += n;
            cp = *p;
            continue;
invalid:
            cp = 0xfffd;
            adv = 1;
            goto encode;   /* U+FFFD, then continue after 1 byte */
encode:;
        }
    }
    out[used] = 0;
    return used;
}

/* ===== channel readiness ===== */

/*
 * protected_channel_ready — the protected channel is usable: captured
 * (installed flag, class, method, ready word) and, under the 0x1a7c08
 * latch, the snapshot holds both function ids and flag a and c set with
 * flag b clear. @ 0015922c
 */
bool protected_channel_ready(void)
{
    if (g_channel_installed != 1 || g_channel_class == 0)
        return false;
    if (g_channel_ready == 0)
        return false;
    if (g_channel_method == 0)
        return false;
    if ((ui_latch_test_and_set(1, &g_pchan_latch) & 1) != 0)
        return false;
    g_pchan_latch = 0;
    return g_pchan_fn_b != 0 && g_pchan_fn_a != 0 && g_pchan_flag_a != 0 &&
           g_pchan_flag_c != 0 && g_pchan_flag_b == 0;
}

/* ===== NRI1 request records ===== */

/*
 * nri1_header_valid — validate an NRI1 request header: magic "NRI1"
 * (0x3149524e), version 1, length 0x6c, and the three-word rotate-5
 * checksum chain (words 4..7 chained from word 3 ^ 0x3b7d4c22) must
 * match, with word 6 (the record length) < 0x1000. @ 001597b8
 */
bool nri1_header_valid(const int32_t *hdr)
{
    uint32_t a, b;

    if (hdr[0] != 0x3149524e || hdr[1] != 1 || hdr[2] != 0x6c)
        return false;
    a = hdr[3] ^ 0x3b7d4c22U;
    a = (a >> 0x1b | a << 5);
    b = hdr[4] ^ a;
    b = (b >> 0x1b | b << 5);
    if (hdr[7] != (int32_t)(hdr[6] ^ (b >> 0x1b | b << 5)))
        return false;
    return (uint32_t)hdr[6] < 0x1000;
}

/* ===== plus request records ===== */

/* 0x18-byte NRI1-style request record (see plus_request_record_init) */
typedef struct {
    uint32_t magic;        /* +0x00 "NRI1" (from the 0x10f960 literal) */
    uint32_t verdict;      /* +0x04 resolved plus verdict (mode 0x7e/0x10010) */
    uint32_t plus_ok;      /* +0x08 social-installed and verdict ok */
    uint32_t verdict2;     /* +0x0c verdict copy */
    uint32_t installed;    /* +0x10 social islands installed */
    uint32_t installed_hi; /* +0x14 */
    uint8_t  is_plus_req;  /* +0x15 1 = mode 0x7e/0x10010 (plus request) */
    uint8_t  _pad16;       /* +0x16 */
    uint16_t _pad17;       /* +0x17 */
} plus_request_record_t;

extern const uint64_t PLUS_REQ_LITERAL;  /* 0x10f960 — {magic lo, fields hi} */

/*
 * plus_request_record_init — initialize a 0x18-byte request record for
 * mode (0x7d..0x80 or 0x10010): the social-installed state is captured,
 * the record is seeded from the 0x10f960 literal, and the plus-active
 * verdict is resolved (pthread_once + the cached function; the verdict
 * defaults to the last cached g_plus_active_a when unresolved). Modes
 * 0x10010 and 0x7e additionally mark the record as a plus request and
 * store the verdict in two fields. Returns 1 on success, 4 when rec is
 * NULL, not 0x18 bytes, or the mode is unknown. @ 00159824
 */
uint32_t plus_request_record_init(void *ctx, int32_t mode, int32_t *rec)
{
    int installed = g_social_installed;
    int verdict = 0;

    (void)ctx;
    if (rec == NULL || rec[0] != 0x18)
        return 4;
    if (!(mode - 0x7dU < 4 || mode == 0x10010))
        return 0;

    pthread_once(&g_plus_resolve_once, protected_plus_resolve_once);
    if (g_plus_query_fn != NULL && g_plus_query_fn(0) != 1)
        verdict = 0;
    else if (g_plus_query_fn != NULL)
        verdict = (int)g_plus_active_a;

    rec[0] = (int32_t)(PLUS_REQ_LITERAL & 0xffffffffu);
    rec[1] = (int32_t)(PLUS_REQ_LITERAL >> 32);
    rec[2] = (int32_t)(PLUS_REQ_LITERAL >> 32);  /* second word of the literal */
    *(uint8_t *)((char *)rec + 0x15) = 0;
    rec[4] = installed == 0;
    *(int *)((char *)rec + 0x14) = installed != 0;
    *(uint16_t *)((char *)rec + 0x16) = 0;
    if (mode == 0x10010 || mode == 0x7e) {
        *(uint8_t *)((char *)rec + 0x15) = 1;
        rec[1] = verdict;
        rec[2] = installed != 0 && verdict != 0;
        rec[3] = verdict;
    }
    return 1;
}
/* misc_a chain chunk 13: covers raw lines 5636-6100 */

#include <pthread.h>

/* ===== forward declarations within this chain ===== */
bool plus_channel_ready(void);                                     /* below @ 00159ac0 */
uint32_t plus_request_submit(int32_t mode, const void *name, size_t len,
                             uint64_t parent, uint64_t env);  /* below @ 00159bbc */
int  parse_trimmed_number(const void *buf, size_t len,
                          uint32_t *out);                     /* below @ 00159e08 */
int  plus_tag_record_parse(const void *buf, size_t len,
                           void *rec);                        /* below @ 00159f20 */

/* ===== cross-file functions ===== */
extern int  ui_latch_test_and_set32(int value,
                                    volatile int *latch);  /* menu_engine.c @ 00193fe0 */
extern void *ui_android_dialog_bind(void *arg);  /* widgets.c @ 0015a108 */

/* ===== plus-service globals ===== */
extern void *g_clip_service;   /* 0x22f4c8 — dialog JVM */
extern void *g_clip_valid;     /* 0x22f4e8 — dialog ctx object */
extern int32_t g_dialog_pending_tid;  /* 0x22f03c — dialog bind worker tid */
extern int32_t g_plus_service_installed; /* 0x22db78 — entitlement service gate */
extern uint64_t g_plus_env_handle;      /* 0x1a7c48 — plus request env handle */

/* plus request block (guarded by the 0x25c92c mutex) */
extern pthread_mutex_t g_plus_request_mutex; /* 0x25c92c (+4 = the raw lock word) */
extern int32_t g_plus_request_mode;          /* 0x25c958 — 0x7d/0x7f/0x80 request mode */
extern char g_plus_request_name[0x7c];       /* 0x25c95c — request name buffer */
extern uint64_t g_plus_request_parent;       /* 0x25c9c0 — parent widget */
extern uint64_t g_plus_request_env;          /* 0x25c9c8 — request env */
extern int64_t g_social_req_d;               /* 0x25c8d0 — social island request d */
extern int64_t g_social_req_e;               /* 0x25c8d8 — social island request e */
extern int32_t g_dialog_bind_latch;          /* 0x25c9d8 — dialog bind in-flight latch */
extern uint64_t g_social_req_stamp;          /* 0x25c8cc — request stamp (ms) */

/*
 * plus_request_dispatch — dispatch a 0x40-byte request record. Mode must
 * be 0x81..0x84 and plus_channel_ready must hold. Mode 0x7e-style verdict
 * records (param[1] == 0x7e) just cache param[3] as the plus verdict
 * (param[3] must be <= 1); other modes with a non-NULL arg (+0xc) are
 * submitted inline through plus_request_submit; without an arg a worker
 * thread (ui_android_dialog_bind) is spawned (under the 0x25c9d8 latch,
 * detached) carrying {mode, parent widget, env handle}. Returns 2 when
 * the worker was spawned, 1 for the verdict path, 3 when a request is
 * already in flight, 0xffffffff on spawn failure, 4 on bad input.
 * @ 00159930
 */
uint64_t plus_request_dispatch(void *ctx, const int32_t *rec)
{
    struct {
        int32_t mode;      /* +0x00 */
        uint32_t _pad;     /* +0x04 */
        uint64_t parent;   /* +0x08 */
        uint64_t env;      /* +0x10 */
    } *job;
    pthread_t tid;
    int32_t mode;

    (void)ctx;
    if (rec == NULL || rec[0] != 0x40 || (uint32_t)(rec[1] - 0x81) >= 0xfffffffc)
        return 4;
    if (!plus_channel_ready())
        return 4;
    mode = rec[1];
    if (mode == 0x7e) {
        if (1 < (uint32_t)rec[3])
            return 4;
        g_plus_active_a = (uint32_t)rec[3];   /* 0x22db68 */
        return 1;
    }
    if (*(const uint64_t *)((const char *)rec + 0xc) != 0) {
        /* inline submit */
        plus_request_submit(mode,
                            *(const void **)((const char *)rec + 0x28),
                            *(const uint64_t *)((const char *)rec + 0x10),
                            g_overlay_parent, g_plus_env_handle);
        return 0;
    }
    if (g_social_req_e == 0 || g_social_req_d == 0)
        return 0;
    if (ui_latch_test_and_set32(1, &g_dialog_bind_latch) == 0) {
        job = calloc(1, 0x38);
        if (job != NULL) {
            job->mode = mode;
            job->parent = g_overlay_parent;      /* 0x22db60 */
            job->env = g_plus_env_handle;        /* 0x1a7c48 */
            if (pthread_create(&tid, NULL, ui_android_dialog_bind, job) == 0) {
                pthread_detach(tid);
                return 2;
            }
            free(job);
        }
        g_dialog_bind_latch = 0;
        return 0xffffffff;
    }
    return 3;
}

/*
 * plus_channel_ready — the plus service is usable: both service gates
 * set (social installed, plus service installed), on the MainPool thread
 * (name "MainPool", tid >= 1), and the plus-active resolver returns 1.
 * @ 00159ac0
 */
bool plus_channel_ready(void)
{
    char name[16] = {0};

    if (g_social_installed == 0 || g_plus_service_installed == 0)
        return false;
    if (!(g_main_pool_tid > 0 && gettid() == g_main_pool_tid))
        return false;
    if (pthread_getname_np(pthread_self(), name, 0x10) != 0)
        return false;
    if (memcmp(name, "MainPool", 9) != 0 || g_dialog_pending_tid < 1)
        return false;
    pthread_once(&g_plus_resolve_once, protected_plus_resolve_once);
    if (g_plus_query_fn == NULL)
        return false;
    return g_plus_query_fn(0) == 1;
}

/*
 * plus_request_submit — validate and store a plus request under the plus
 * request mutex. Mode 0x7f takes a raw name: no NUL, <= 0x5f bytes, only
 * tab/LF/CR/space around a number <= 999 (parsed via parse_trimmed_
 * number semantics inline). Other modes parse the tagged form with
 * plus_tag_record_parse. Only modes 0x7d/0x7f/0x80 (not 0x7e) store: the
 * request stamp is taken, the name buffer and mode/parent/env fields are
 * filled and the request is marked pending. Returns 2 when stored, 3 when
 * a request is already pending, 4 on validation failure. @ 00159bbc
 */
uint32_t plus_request_submit(int32_t mode, const void *name, size_t len,
                             uint64_t parent, uint64_t env)
{
    struct timespec ts;
    uint64_t stamp = 0;
    size_t lo, hi;
    uint32_t value;

    if (mode == 0x7f) {
        if (name == NULL || 0x5f < len - 1)
            return 4;
        if (memchr(name, 0, len) != NULL)
            return 4;
        /* find the first non-blank */
        for (lo = 0; lo < len; lo++) {
            unsigned char c = ((const unsigned char *)name)[lo];

            if (4 < c - 9 && c != 0x20)
                break;
        }
        if (len <= lo)
            return 4;
        /* trim trailing blanks */
        hi = len;
        while (lo < hi) {
            unsigned char c = ((const unsigned char *)name)[hi - 1];

            if (4 < c - 9 && c != 0x20)
                break;
            hi--;
        }
        if (lo != hi) {
            /* digits only, <= 999 */
            value = 0;
            if (lo < hi) {
                const unsigned char *p = name;
                size_t i;

                for (i = lo; i != hi; i++) {
                    unsigned char c = p[i];

                    if ((unsigned)(c - 0x3a) < 0xfffffff6u || 999 < value)
                        return 4;
                    value = ((uint32_t)c + value * 10) - 0x30;
                }
            }
        } else {
            return 4;
        }
    } else {
        if (!plus_tag_record_parse(name, len, &(uint64_t){0}))
            return 4;
    }

    if (!(mode - 0x7dU < 4) || mode - 0x7dU == 1)
        return 4;   /* 0x7e never stores */
    pthread_mutex_lock(&g_plus_request_mutex);
    if (g_plus_request_mode == 0) {
        if (clock_gettime(1 /* CLOCK_MONOTONIC */, &ts) == 0)
            stamp = (uint64_t)ts.tv_sec * 1000u +
                    (uint64_t)ts.tv_nsec / 1000000u;
        g_social_req_d = (int64_t)stamp;   /* request stamp (0x25c8d0) */
        memset((void *)g_plus_request_name, 0, 0x60); /* clear 0x25c95c..0x25c9bc */
        g_plus_request_mode = mode;
        g_plus_request_parent = parent;
        g_plus_request_env = env;
        memcpy((void *)g_plus_request_name, name, len);  /* cap 0x7c */
        g_plus_request_name[len] = 0;
        pthread_mutex_unlock(&g_plus_request_mutex);
        return 2;
    }
    pthread_mutex_unlock(&g_plus_request_mutex);
    return 3;
}

/*
 * parse_trimmed_number — parse a whitespace-trimmed (tab/LF/CR/space)
 * digit string of at most 0x5f bytes (no embedded NUL) into *out; the
 * value must not exceed 999. Returns 1 on success, 0 otherwise.
 * @ 00159e08
 */
int parse_trimmed_number(const void *buf, size_t len, uint32_t *out)
{
    const unsigned char *p = buf;
    size_t lo;
    uint32_t value = 0;

    if (0x5f < len - 1)
        return 0;
    if (buf == NULL || out == NULL)
        return 0;
    if (memchr(buf, 0, len) != NULL)
        return 0;
    lo = 0;
    while (p[lo] - 9 < 5 || p[lo] == 0x20) {
        lo++;
        if (lo == len)
            return 0;
    }
    if (lo < len) {
        while (p[len - 1] - 9 < 5 || p[len - 1] == 0x20) {
            len--;
            if (len <= lo)
                return 0;
        }
    }
    if (lo == len)
        return 0;
    if (lo < len) {
        size_t i;

        for (i = lo; i != len; i++) {
            unsigned char c = p[i];

            if ((unsigned)(c - 0x3a) < 0xfffffff6u)
                return 0;
            if (999 < value)
                return 0;
            value = ((uint32_t)c + value * 10) - 0x30;
        }
    }
    *out = value;
    return 1;
}

/*
 * plus_tag_record_parse — parse a tagged name (<= 0x5f bytes, no NUL)
 * into a 0x28-byte request record. The name must be blank-trimmed; a
 * leading '#' is skipped; the remaining 1..0x1e characters must all be
 * from the alphabet "0289PYLQGRJCUV" (uppercase-folded: a-z folds by
 * -0x20) and are packed base-14 into record[0], echoed into record[1]'s
 * string, with the length in the top bytes of record[0]. Returns 1 on
 * success (record filled), 0 on any validation failure. @ 00159f20
 */
int plus_tag_record_parse(const void *buf, size_t len, void *rec_out)
{
    struct {
        uint64_t packed;    /* +0x00: length in the high bytes */
        uint64_t text;      /* +0x08: '#' + echoed chars */
        uint64_t field10;   /* +0x10 */
        uint64_t field18;   /* +0x18 */
        uint64_t field20;   /* +0x20 */
    } *rec = rec_out;
    const unsigned char *p = buf;
    size_t start = 0, n;
    uint64_t packed = 0;
    char text[0x1f];
    size_t i;

    if (!(len - 1 < 0x60) || buf == NULL || rec_out == NULL)
        return 0;
    if (memchr(buf, 0, len) != NULL)
        return 0;
    /* find the first non-blank */
    do {
        if (4 < p[start] - 9 && p[start] != 0x20)
            goto found;
        start++;
    } while (len != start);
    return 0;

found:
    /* trim trailing blanks */
    while (p[len - 1] - 9 < 5 || p[len - 1] == 0x20) {
        len--;
        if (len <= start)
            return 0;
    }
    if (p[start] == '#')
        start++;
    n = len - start;
    if (n == 0 || 0x1e < n)
        return 0;
    text[0] = '#';
    for (i = 0; i < n; i++) {
        unsigned char c = p[start + i];
        unsigned char folded = (0x19 < c - 0x61) ? c : (unsigned char)(c - 0x20);
        const char *hit = __strchr_chk("0289PYLQGRJCUV", folded, 0xf);

        if (hit == NULL)
            return 0;
        if (0xffffffffffU - (uint64_t)(hit - "0289PYLQGRJCUV") < packed * 0xe)
            return 0;   /* overflow guard */
        packed = (uint64_t)(hit - "0289PYLQGRJCUV") + packed * 0xe;
        text[1 + i] = (char)folded;
    }
    rec->packed = (packed << 8) | 0x23 /* '#' */ |
                  ((uint64_t)n << 32);  /* CONCAT44: {packed, '#'+len} */
    rec->text = 0;
    memcpy(&rec->text, text, n + 1);
    rec->field10 = 0;
    rec->field18 = 0;
    rec->field20 = 0;
    return 1;
}
/* misc_a chain chunk 14: covers raw lines 6101-6550 */

#include <pthread.h>
#include <unistd.h>

/* ===== forward declarations within this chain ===== */
int  arm64_branch_encode(uint64_t from, uint64_t to,
                         uint32_t *out);                        /* b-chain @ 00190d68 */

/* ===== cross-file functions ===== */
extern void *ui_server_hook_target(void);     /* misc.c (b-chain) @ 0015d224 */

/* ===== rodata ===== */
extern const char g_stage_table[];     /* 0x19b378 — 20 known stage-name ptrs */
extern const char g_island_sig_a[];    /* 0x1361b3 — Dialog.isShowing signature */
extern const char g_island_sig_b[];    /* 0x135b8c — Looper method name */
extern const char g_island_sig_c[];    /* 0x13ba46 — Looper method signature */

/* forward declarations from earlier chunks (signatures must match) */
void postbattle_failure_note(uint64_t base, uint64_t site, uint32_t index,
                             const char *stage, int err);       /* c05/c14 fwd */
void ui_server_trampoline_record(uint64_t island, uint32_t id,
                                 const void *readback);           /* c05/c14 fwd */
int  island_patches_apply(void *islands, const void *patch_blob,
                          void *scratch);                        /* c05 fwd */

/* ===== postbattle island failure note ===== */

/*
 * postbattle_failure_note — record an island failure stage: stage is
 * looked up in the 20-entry stage-name table (unknown stages become
 * "unknown"), and only the mmap/seal/gap_* stages keep their errno
 * argument (others pass 0). The failure site must be one of the four
 * island RVAs (0x772870, 0x5921a0, 0x88836c, 0x678fe4) relative to the
 * base, otherwise 0 is recorded. Formats "pb_island_i%u_%lx_last_%s_e%u"
 * into the 0x25c9dc status line. (Definition lives in this chunk; the
 * c05 forward declaration matches.) @ 0015c6f8
 */
void postbattle_failure_note(uint64_t base, uint64_t site, uint32_t index,
                             const char *stage, int err)
{
    const char *known = "unknown";
    const char **table = (const char **)&g_stage_table;
    int i;

    if (stage != NULL) {
        for (i = 0; i < 20; i++) {
            if (strcmp(stage, table[i]) == 0) {
                known = table[i];
                break;
            }
        }
    }
    if (strcmp(known, "mmap") == 0 || strcmp(known, "seal") == 0 ||
        strcmp(known, "gap_open") == 0 || strcmp(known, "gap_read") == 0 ||
        strcmp(known, "gap_close") == 0 || strcmp(known, "gap_mmap") == 0 ||
        strcmp(known, "gap_unmap") == 0) {
        if (0xffe < (uint32_t)(err - 1))
            err = 0;
    } else {
        err = 0;
    }

    uint64_t rva = 0;

    if (base <= 0xffffffffff88d78fU) {         /* base + 0x772870 does not wrap */
        if (base + 0x772870 == site)
            rva = 0x772870;
    }
    if (rva == 0 && base <= 0xffffffffffa6de5fU) {
        if (base + 0x5921a0 == site)
            rva = 0x5921a0;
    }
    if (rva == 0 && base <= 0xffffffffff777c93U) {
        if (base + 0x88836c == site)
            rva = 0x88836c;
    }
    if (rva == 0 && base + 0x678fe4 == site &&
        base <= 0xffffffffff98701bU)
        rva = 0x678fe4;

    str_format((char *)0x25c9dc /* status line at 0x25c9dc */, 0x37, 0x37,
               "pb_island_i%u_%lx_last_%s_e%u", index, rva, known, err);
}

/* ===== ui-server island trampoline record ===== */

/* recorded ui-server trampoline (0x22db80 block, set once) */
uint64_t g_ui_server_trampoline_addr;  /* 0x22db80 — island address */
int32_t  g_ui_server_trampoline_id;    /* 0x22db88 — island id word */
uint64_t g_ui_server_trampoline_len;   /* 0x22db90 — template length */
char     g_ui_server_trampoline_copy[0x208]; /* 0x22db98 — expected image */
int32_t  g_ui_server_trampoline_done;  /* 0x22dd98 — 1 = recorded */

/*
 * ui_server_trampoline_record — build and record the 0x5921a0 island
 * trampoline: encode the ARM64 branch from the island site into the
 * island, verify the encoded word matches the expected one, patch a
 * template copy of the island image (0x52800002 MOVZ W2,0 / 0xd100c3ff
 * SUB SP,SP,#0x30 / the branch word / the ui_server_hook_target fn
 * pointer), and compare it byte-for-byte with the readback image. On
 * match the trampoline is recorded in the 0x22db80 block (only when not
 * recorded yet). The template must be <= 0x200 bytes. @ 0015d078
 */
void ui_server_trampoline_record(uint64_t island, uint32_t id,
                                 const void *readback)
{
    const uint8_t *tmpl = g_island_tmpl_begin;
    uint8_t copy[0x200];
    uint32_t branch = 0;
    size_t len = ISLAND_TMPL_LEN;

    if (island == 0 || id == 0 || len >= 0x201 || len == 0)
        return;
    if (readback == NULL || g_ui_server_trampoline_done != 0)
        return;

    if (arm64_branch_encode((uint64_t)g_proc_mem_bias + 0x5921a0, island,
                            &branch) == 0)
        return;
    if (branch != (uint32_t)id)
        return;
    if (arm64_branch_encode((uint64_t)g_social_tmpl_probe +
                                (island - (uint64_t)tmpl),
                            (uint64_t)g_proc_mem_bias + 0x5921a4,
                            &branch) == 0)
        return;

    memcpy(copy, tmpl, len);   /* cap 0x200 */
    *(uint32_t *)((char *)copy + ((uintptr_t)g_social_tmpl_slot -
                                  (uintptr_t)tmpl)) = 0x52800002;
    *(uint32_t *)((char *)copy + ((uintptr_t)g_social_tmpl_link_a -
                                  (uintptr_t)tmpl)) = 0xd100c3ff;
    *(uint32_t *)((char *)copy + ((uintptr_t)g_social_tmpl_probe -
                                  (uintptr_t)tmpl)) = branch;
    *(void **)((char *)copy + ((uintptr_t)g_social_tmpl_link_b -
                               (uintptr_t)tmpl)) = ui_server_hook_target;

    if (memcmp(readback, copy, len) == 0) {
        g_ui_server_trampoline_addr = island;
        g_ui_server_trampoline_id = id;
        g_ui_server_trampoline_len = len;
        memcpy(g_ui_server_trampoline_copy, readback, len); /* cap 0x208 */
        g_ui_server_trampoline_done = 1;
    }
}

/* ===== island patch application ===== */

/*
 * island_patches_apply — write and verify all 4 postbattle islands.
 * islands is the 4-entry {site, alloc_id, branch_word} table (each entry
 * 2 words: site at +0, id/branch at +8); ops is the remote-accessor
 * bundle {ctx, read, write, close}. For each island: the branch word is
 * written to the site (island_word_write) and read back; a write
 * failure, read failure or mismatch is reported into the 0xb4-word
 * report record (stage "write"/"read"/"mismatch", island index encoded
 * as -(index+1)) with the current TLS stream state, and apply stops.
 * When all four writes verify, the readback loop re-reads every island
 * word; a mismatch is reported at the failing index. The report's final
 * word is set to 1 when everything verified. Returns 1 on success, or
 * the negative one-based index of the failing island. @ 0015c9e0
 */
int island_patches_apply(void *islands, const void *ops, void *report_v)
{
    struct {
        void *ctx;     /* +0x00 */
        int (*read)(void *ctx, uint64_t addr, uint32_t *out);   /* +0x08 */
        int (*write)(void *ctx, uint64_t addr, uint32_t word);  /* +0x10 */
        int (*close)(void *ctx, uint64_t cookie);               /* +0x18 */
    } *acc = (void *)ops;
    uint32_t *report = report_v;
    const uint64_t *site = islands;   /* 4 entries, 2 words each */
    uint32_t readback = 0;
    long idx;
    int i, ok;

    memset(report, 0, 0xb0);   /* 0x2c words */
    readback = 0;

    /* write phase: island_word_write then read back through the accessor */
    for (idx = 0; ; idx++) {
        if (idx == 4) {
            ok = 1;
            goto done;
        }
        if (acc->write(acc->ctx, site[idx * 2], (uint32_t)site[idx * 2 + 1]) == 0) {
            /* write failed: record and stop */
            report[0] = (uint32_t)(-(int)idx);
            report[2] = (uint32_t)site[idx * 2];
            report[4] = (uint32_t)(uintptr_t)"write";
            report[6] = 0;
            report[0x2c] = 1;
            ok = -1;
            goto done;
        }
        ok = acc->read(acc->ctx, site[idx * 2], &readback);
        if (ok == 0) {
            report[0] = (uint32_t)(-(int)idx);
            report[2] = (uint32_t)site[idx * 2];
            report[4] = (uint32_t)(uintptr_t)"read";
            report[6] = 0;
            report[0x2c] = 1;
            ok = -1;
            goto done;
        }
        if (readback != (uint32_t)site[idx * 2 + 1]) {
            report[0] = (uint32_t)~idx;
            report[2] = (uint32_t)site[idx * 2];
            report[4] = (uint32_t)(uintptr_t)"mismatch";
            report[6] = readback;
            report[0x2c] = 1;
            ok = -1;
            goto done;
        }
    }

done:
    if (ok != 1)
        return ok;
    /* verification loop: re-read every island word */
    for (i = 0; i < 4; i++) {
        acc->write(acc->ctx, site[i * 2], (uint32_t)site[i * 2 + 1]);
        if (acc->read(acc->ctx, site[i * 2], &readback) == 0) {
            report[0x16] = (uint32_t)i;
            report[0x18] = (uint32_t)site[i * 2];
            report[0x1a] = (uint32_t)(uintptr_t)"read";
            report[0x1c] = 0;
            report[0x2c] = 1;
            return -(i + 1);
        }
        if (readback != (uint32_t)site[i * 2 + 1]) {
            report[0x16] = (uint32_t)i;
            report[0x18] = (uint32_t)site[i * 2];
            report[0x1a] = (uint32_t)(uintptr_t)"mismatch";
            report[0x1c] = readback;
            report[0x2c] = 1;
            return -(i + 1);
        }
    }
    report[0x2c] = 1;
    return 1;
}

/* ===== dialog close-wait thread ===== */

/*
 * dialog_close_wait_thread — pthread body waiting for the android
 * Dialog to close. Attaches to the dialog JVM (GetEnv/Attach), pushes a
 * local frame, resolves android.app.Dialog.isShowing ()Z and the looper
 * wake method, then polls isShowing on the dialog object every 40 ms
 * until it reports false (closed flag set at +0x30) or the stop flag at
 * +0x34 is observed; exceptions abort the poll. The looper wake is
 * called on the stored looper, the frame is popped and the thread
 * detached when it had attached. The done flag at +0x2c is always set.
 * Returns 0. @ 0015b33c
 */
void *dialog_close_wait_thread(void *arg)
{
    struct {
        uint64_t dialog;    /* +0x18 dialog object */
        uint64_t looper;    /* +0x20 looper object */
        uint32_t started;   /* +0x28 */
        uint32_t closed;    /* +0x30 isShowing became false */
        uint32_t done;      /* +0x2c thread finished */
        uint32_t stop;      /* +0x34 stop requested */
    } *st = arg;
    void *env = NULL;
    bool attached = false;
    long dialog_cls, is_showing, looper_wake;
    int rc;

    if (g_clip_service == NULL || g_clip_valid == 0)
        goto out;
    rc = (int)jni_call2_ma(g_clip_service, 0x30 /* GetEnv */,
                           (long)(void *)&env, 0x10006 /* JNI_VERSION_1_6 */);
    if (rc == 0) {
        attached = true;
    } else if (rc == -2 /* JNI_EDETACHED */) {
        if (jni_call2_ma(g_clip_service, 0x20 /* AttachCurrentThread */,
                         (long)(void *)&env, 0) != 0)
            goto out;
        attached = false;
    } else {
        goto out;
    }
    if (env == NULL)
        goto out;

    if (jni_call1_ma(env, 0x98 /* PushLocalFrame */, 8) == 0) {
        dialog_cls = jni_call1_ma(env, 0x30 /* FindClass */,
                                  (long)"android/app/Dialog");
        if (dialog_cls != 0) {
            is_showing = jni_call3_ma(env, 0x108 /* GetMethodID */,
                                      dialog_cls, (long)"isShowing",
                                      (long)g_island_sig_a);
            looper_wake = 0;
            if (is_showing == 0)
                is_showing = 0;
            if (is_showing != 0) {
                long looper_cls = jni_call1_ma(env, 0x30, (long)"android/os/Looper");

                if (looper_cls != 0)
                    looper_wake = jni_call3_ma(env, 0x108, looper_cls,
                                                (long)g_island_sig_b,
                                                (long)g_island_sig_c);
            } else {
                looper_wake = 0;
            }
        } else {
            is_showing = 0;
            looper_wake = 0;
        }
        if (jni_call0_ma(env, 0x720 /* ExceptionCheck */) == 0 &&
            is_showing != 0 && looper_wake != 0) {
            st->started = 1;
            while (st->stop == 0) {
                char showing = (char)jni_call2_ma(env, 0x128 /* CallBooleanMethod */,
                                                  st->dialog, is_showing);

                if (jni_call0_ma(env, 0x720) != 0)
                    break;
                if (showing == 0) {
                    st->closed = 1;
                    break;
                }
                usleep(40000);   /* 40 ms poll */
            }
        }
        if (jni_call0_ma(env, 0x720) != 0)
            jni_call0_ma(env, 0x88 /* ExceptionClear */);
        if (looper_wake != 0) {
            jni_call2_ma(env, 0x1e8 /* CallStaticVoidMethod */, st->looper,
                         looper_wake);
            if (jni_call0_ma(env, 0x720) != 0)
                jni_call0_ma(env, 0x88 /* ExceptionClear */);
        }
        jni_call1_ma(env, 0xa0 /* PopLocalFrame */, 0);
    }
    if (!attached)
        jni_call0_ma(g_clip_service, 0x28 /* DetachCurrentThread */);
out:
    st->done = 1;
    return 0;
}
/* misc_a chain chunk 15: covers raw lines 6591-7050 */

/* ===== forward declarations within this chain ===== */
int  remote_read_ptr(uint64_t addr, uint64_t *out);                  /* c07 @ 00154cf0 */
void chooser_record_refresh(void);                                 /* below @ 00164398 */

/* ===== cross-file functions ===== */
extern int  font_chooser_snapshot_read(void *out);   /* misc.c (b-chain) @ 00191df8 */
extern uint64_t script_port_client_performance_query(uint64_t snap_addr);  /* misc.c (c-chain) @ 00191c90 */
extern uint64_t script_port_client_performance_apply(uint32_t arg);     /* misc.c (c-chain) @ 00191cd0 */
extern void menu_perf_status_row_append(void *rows); /* widgets.c @ 00162118 */
extern int  font_chooser_commit_apply(void);         /* renderer.c @ 00164450 */
extern int  string_obj_from_cstr(const char *s,
                                uint64_t obj[3]); /* misc.c (c-chain) @ 00190fd0 */

/* ===== rodata ===== */
extern const uint64_t CHOOSER_SNAPSHOT_MAGIC;   /* 0x10f748 — font snapshot header */
extern const uint64_t PERF_SNAPSHOT_MAGIC;      /* 0x10f850 — performance snapshot header */

/* ===== chooser / performance cache (0x25ca80 block) ===== */
uint32_t g_chooser_mode;        /* 0x25ca80 — 0 none, 1 perf, 2 chooser */
uint32_t g_perf_arg;             /* 0x25ca84 — last applied perf arg */
uint32_t g_perf_arg_shadow;      /* 0x25ca88 — perf arg echo */
uint64_t g_chooser_cache[0x10];  /* 0x25ca90 — 0x60-byte snapshot cache */
extern int32_t g_chooser_popped; /* 0x233ee8 — a ring entry was popped */

/* ===== battle-state words (owned here, read by the tracker) ===== */
extern uint32_t g_battle_word1;  /* 0x22d888 — battle state bit */
extern uint32_t g_battle_word2;  /* 0x22d88c — set after a perf save */
extern uint32_t g_battle_word3;  /* 0x22d890 — battle state bit */
extern uint32_t g_battle_word4;  /* 0x22d894 — cleared after a perf save */
extern uint32_t g_perf_pending;  /* 0x22d898 — perf write pending */
extern uint32_t g_perf_saved_hi; /* 0x22d8a0 */
extern uint32_t g_perf_saved_stamp; /* 0x22d8a8 — save stamp */
extern uint32_t g_perf_confirmed;   /* 0x22dfbc — nonzero = dir fsync failed */
extern uint32_t g_perf_latch;       /* 0x22dfb8 — perf save in flight */
extern uint32_t g_perf_seq;         /* 0x22f4b8 — perf save sequence */
uint32_t g_perf_save_seq;           /* 0x25cb28 — performance.bin seq (-1 disables) */
uint32_t g_perf_save_count;         /* 0x25cb30 — saves since load */
extern int32_t g_clip_input_next;   /* 0x25cb20 — clipboard cooldown */
extern void *g_clip_method;         /* 0x22f4d8 — clipboard jmethodID */

/*
 * command_ring_pop — pop the next valid entry from the pending-command
 * ring under the 0x22f500 latch. Entries are skipped while their flag_a
 * is clear and their stamp does not match the staging stamp; a valid
 * entry yields {opcode, value, externally-pushed flag} and marks the
 * staging block popped (0x233ee8 = 1). Returns 1 when an entry was
 * popped, 0 when the ring is empty or the latch is held. @ 001612c8
 */
int command_ring_pop(uint32_t *opcode, uint32_t *value, uint32_t *external)
{
    int entry;
    uint32_t slot;
    uint8_t flag_b;
    uint32_t arg_b;

    if (opcode == NULL || value == NULL || external == NULL)
        return 0;
    if ((ui_latch_test_and_set(1, &g_pending_records_latch) & 1) != 0)
        return 0;
    for (;;) {
        entry = (int)g_ring_pending_count - 1;
        if (entry == -1) {
            g_pending_records_latch = 0;
            return 0;
        }
        slot = g_ring_commit_count;

        g_ring_commit_count = (g_ring_commit_count + 1) & 0x1f;
        flag_b = g_pending_flag_b[slot];
        arg_b = (uint32_t)g_pending_arg_b[slot];

        g_ring_pending_count = entry;
        if (flag_b != 0 || g_pending_arg_a[slot] == g_pending_record_stamp)
            break;
    }
    *opcode = g_pending_records[slot];
    *value = arg_b;
    *external = flag_b != 0;
    g_chooser_popped = 1;
    g_pending_records_latch = 0;
    return 1;
}

/*
 * chooser_request_dispatch — handle chooser/performance snapshot
 * requests and commands. 0x21036 reads the font snapshot (magic 1, size
 * 0x30, engageable, < 6 selection) into the chooser cache (state 2);
 * 0x21031 reads the performance snapshot (magic 1, size 0x60, engageable
 * bit) into the cache (state 1) and seeds the perf arg (0x91 when the
 * snapshot arg is 0; values in [0x92..0x101] are accepted, others reset
 * the state and fail with 4). With a snapshot cached: state 2 handles
 * 0x2e010 (close), 0x2e020..0x2e025 (font commit via font_chooser_
 * commit_apply); state 1 handles 0x2e000 (close), 0x2e100..0x2e190
 * (apply arg), 0x2e002/0x2e003 (re-apply via script_port_client_
 * performance_apply, refreshing the record on change). Unknown commands
 * reset the state. *result gets 1 on success, 0 for no-op, 4 for an
 * invalid perf arg. Returns 1 when the command was consumed, 0 when
 * ignored. @ 0016169c
 */
int chooser_request_dispatch(uint32_t cmd, uint32_t *result)
{
    int ok;

    if (cmd == 0x21036) {
        uint64_t snap[12];

        memset(snap, 0, sizeof snap);
        snap[0] = CHOOSER_SNAPSHOT_MAGIC;
        if (font_chooser_snapshot_read(snap) != 0 &&
            (int32_t)snap[0] == 1 && (uint32_t)(snap[0] >> 32) == 0x30 &&
            (uint32_t)snap[2] != 0 && (uint32_t)(snap[1] >> 32) < 6) {
            g_chooser_mode = 2;
            g_chooser_cache[3] = snap[2];   /* 0x25caf8 */
            g_chooser_cache[2] = snap[0];   /* 0x25caf0 */
            g_chooser_cache[5] = snap[4];   /* 0x25cb08 */
            g_chooser_cache[4] = snap[1];   /* 0x25cb00 */
            g_chooser_cache[7] = snap[5];   /* 0x25cb18 */
            g_chooser_cache[6] = snap[3];   /* 0x25cb10 */
            *result = 1;
            return 1;
        }
        *result = 0;
        return 1;
    }
    if (cmd != 0x21031) {
        if (g_chooser_mode == 0) {
            *result = 0;
            return 0;
        }
        if (cmd != 0x82) {
            if (g_chooser_mode == 2) {
                if (cmd == 0x2e010) {
close:
                    g_chooser_mode = 0;
                    *result = 1;
                    return 1;
                }
                if (cmd - 0x2e020 < 6) {
                    ok = font_chooser_commit_apply();
                    if (ok != 0)
                        g_chooser_mode = 0;
                    *result = ok != 0;
                    return 1;
                }
            } else if (g_chooser_mode == 1) {
                if (cmd == 0x2e000)
                    goto close;
                if (cmd - 0x2e101 < 0x91) {
                    int arg = (int)cmd - 0x2e100;

apply:
                    g_perf_arg = (uint32_t)arg;
                    *result = 1;
                    return 1;
                }
                if ((cmd & 0xfffffffe) == 0x2e002) {
                    int arg = 0x91;

                    if (cmd != 0x2e003)
                        arg = (int)g_perf_arg;
                    if (arg == 0x91)
                        arg = 0;
                    ok = script_port_client_performance_apply(arg) != 0;
                    if (ok != 0) {
                        g_perf_arg = (uint32_t)arg;
                        g_perf_arg_shadow = (uint32_t)arg;
                        chooser_record_refresh();
                    }
                    *result = ok != 0;
                    return 1;
                }
            }
        }
        g_chooser_mode = 0;
        *result = 0;
        return 0;
    }

    /* 0x21031: performance snapshot */
    {
        uint64_t snap[12];

        memset(snap, 0, sizeof snap);
        snap[0] = PERF_SNAPSHOT_MAGIC;
        if (script_port_client_performance_query((uint64_t)snap) != 0 &&
            (int32_t)snap[0] == 1 && (uint32_t)(snap[0] >> 32) == 0x60 &&
            (snap[2] & 1) != 0) {
            int arg;

            g_chooser_cache[5] = snap[4];   /* 0x25cab8 */
            g_chooser_cache[4] = snap[3];   /* 0x25cab0 */
            g_chooser_cache[3] = (uint64_t)(uint32_t)snap[2]; /* 0x25caa0 lo */
            g_chooser_mode = 1;
            g_chooser_cache[2] = snap[2];   /* 0x25ca98 */
            g_chooser_cache[1] = snap[0];   /* 0x25ca90 */
            g_chooser_cache[7] = snap[5];   /* 0x25cac8 */
            g_chooser_cache[6] = snap[6];   /* 0x25cac0 */
            g_chooser_cache[9] = snap[7];   /* 0x25cad8 */
            g_chooser_cache[8] = snap[8];   /* 0x25cad0 */
            arg = (uint32_t)snap[2] != 0 ? (int)(uint32_t)snap[2] : 0x91;
            g_chooser_cache[11] = snap[10]; /* 0x25cae8 */
            g_chooser_cache[10] = snap[9];  /* 0x25cae0 */
            g_perf_arg_shadow = (uint32_t)arg;
            if ((uint32_t)arg - 0x92 < 0xffffff6f) {
                *result = 4;
                g_chooser_mode = 0;
                return 1;
            }
            goto apply;
        }
    }
    *result = 0;
    return 1;
}

/*
 * clipboard_invoke — invoke the clipboard service method (JNI vtable
 * slot 0x3a8, CallStaticBooleanMethod) with an argument on the clipboard
 * service captured by widgets.c (JVM 0x22f4c8, class 0x22f4e8, method
 * 0x22f4d8), attaching the thread when detached. The clipboard cooldown
 * word (0x25cb20) is cleared. Returns 2 when the call returned true, 0
 * when it returned false, -1 when an exception was raised, 0 when the
 * service is missing. @ 00161970
 */
int clipboard_invoke(uint32_t arg)
{
    void *env = NULL;
    bool attached = false;
    char verdict;
    int rc;

    if (g_clip_service == NULL || g_clip_valid == 0)
        return 0;
    rc = (int)jni_call2_ma(g_clip_service, 0x30 /* GetEnv */,
                           (long)(void *)&env, 0x10006 /* JNI_VERSION_1_6 */);
    if (rc == 0) {
        attached = true;
    } else if (rc == -2 /* JNI_EDETACHED */) {
        if (jni_call2_ma(g_clip_service, 0x20 /* AttachCurrentThread */,
                         (long)(void *)&env, 0) != 0)
            return 0;
        attached = false;
    } else {
        return 0;
    }
    if (env == NULL)
        return 0;
    verdict = (char)jni_call3_ma(env, 0x3a8 /* CallStaticBooleanMethod */,
                                 (long)g_clip_valid, (long)g_clip_method,
                                 (long)arg);
    if (jni_call0_ma(env, 0x720 /* ExceptionCheck */) != 0) {
        jni_call0_ma(env, 0x88 /* ExceptionClear */);
        rc = -1;
    } else {
        rc = (verdict == 1) << 1;
    }
    if (!attached)
        jni_call0_ma(g_clip_service, 0x28 /* DetachCurrentThread */);
    g_clip_input_next = 0;   /* 0x25cb20 */
    return rc;
}

/*
 * main_view_rows_refresh — rebuild the type-0x20002 rows of the main
 * view record in place. The record must be active (byte +9 set, +10
 * clear), its kind byte (+8, 'C'..) must be one of the view kinds in the
 * 0x22006000cd001 mask, and its row count (+4) nonzero. Every 0x40-
 * stride row whose type word is 0x20002 gets its 7-word payload re-read
 * through menu_perf_status_row_append (row cap 0x14f of the 'MAX
 * OPTIMIZATION' status row). @ 00162260
 */
void main_view_rows_refresh(void *view)
{
    uint64_t *row;
    uint32_t count;
    uint32_t kind;
    uint32_t i;

    if (*(char *)((char *)view + 9) == 0 || *(char *)((char *)view + 10) != 0)
        return;
    kind = (uint8_t)*(char *)((char *)view + 8) - 0x43;
    if (kind >= 0x32)
        return;
    if (((1UL << (kind & 0x3f)) & 0x22006000cd001UL) == 0)
        return;
    count = *(uint32_t *)((char *)view + 4);
    if (count == 0)
        return;
    row = (uint64_t *)((char *)view + 0x28);
    for (i = 0; i < count; i++, row += 8) {
        if (*(int32_t *)(row - 3) == 0x20002) {
            struct {
                uint8_t body[0x688];
                uint32_t status;   /* +0x690: 1 = row was appended */
                uint16_t cap;      /* +0x694: 0x14f */
                uint64_t payload[7];
            } build;

            memset(&build, 0, 0x690);
            build.cap = 0x14f;
            menu_perf_status_row_append(&build);
            if (build.status == 1) {
                row[0] = build.payload[0];
                row[-2] = build.payload[5];
                row[-3] = build.payload[4];
                row[1] = build.payload[1];
                row[2] = build.payload[2];
                row[3] = build.payload[3];
                row[4] = build.payload[6];
            }
            count = *(uint32_t *)((char *)view + 4);
        }
    }
}

/*
 * style_bundle_lookup — check that a style name (<= 0x80 chars) exists
 * in the loaded "sc/ui.sc" bundle: the bundle-overrides gates at
 * libg+0x12eb0e8 and +0x12eb170 must be clear, the bundle must open
 * ("sc/ui.sc" through libg+0x51eacc) and have its +0x80 ready flag set,
 * the bundle's style-table chain (+0x88 -> +0x8 -> +0xf8) must resolve
 * to plausible pointers, the table dims (+0x100, +0x110) must be in
 * (0, 0x10001), and the name must convert to a string object
 * (string_obj_from_cstr) that the bundle's style lookup (libg+0x5dfbf8)
 * resolves. Returns 1 when found. @ 00163eb8
 */
int style_bundle_lookup(void *ctx, const char *name)
{
    uint8_t gate_a = 1, gate_b = 1, ready = 0;
    uint64_t bundle = 0, table = 0, node = 0;
    uint64_t dim_rows = 0, dim_cols = 0;
    uint64_t str_obj[3];
    size_t len;

    (void)ctx;
    if (name == NULL)
        return 0;
    len = strlen(name);
    if (0x81 <= len)
        return 0;
    if (!proc_mem_read(NULL, (uint64_t)g_proc_mem_bias + 0x12eb0e8,
                       &gate_a, 1) || gate_a != 0)
        return 0;
    if (!proc_mem_read(NULL, (uint64_t)g_proc_mem_bias + 0x12eb170,
                       &gate_b, 1) || gate_b != 0)
        return 0;
    bundle = ((uint64_t (*)(const char *, int))((uint64_t)g_proc_mem_bias + 0x51eacc))("sc/ui.sc", 0);
    if (bundle == 0)
        return 0;
    if (!proc_mem_read(NULL, bundle + 0x80, &ready, 1))
        return 0;
    if ((ready & 1) == 0)
        return 0;
    if (!remote_read_ptr(bundle + 0x88, &table))
        return 0;
    if (!remote_read_ptr(table + 8, &node))
        return 0;
    if (!remote_read_ptr(node + 0xf8, &table))
        return 0;
    if (table == 0)
        return 0;
    if (!proc_mem_read(NULL, node + 0x100, &dim_rows, 8))
        return 0;
    if (dim_rows == 0 || 0x10001 <= dim_rows)
        return 0;
    if (!proc_mem_read(NULL, node + 0x110, &dim_cols, 8))
        return 0;
    if (dim_cols == 0 || 0x10001 <= dim_cols)
        return 0;
    if (!string_obj_from_cstr(name, str_obj))
        return 0;
    return ((uint64_t (*)(uint64_t, void *))((uint64_t)g_proc_mem_bias +
                                             0x5dfbf8))(node + 0xf8,
                                                        str_obj) != 0;
}

/*
 * sc_object_free_check_local — direct variant of sc_object_free_check
 * reading through proc_mem_read with the global libg base: the node's
 * vtable must be libg+0x11c0a48 and its +0x38 parent, +0x40 index,
 * +0xa8 and +0x120 words must all be 0/-1 as captured, with the +0x1f0
 * claim byte clear. Returns 1 when the node is detached and unclaimed.
 * @ 00164090
 */
int sc_object_free_check_local(uint64_t obj)
{
    uint64_t parent = 1;
    uint64_t word_a8 = 1;
    uint64_t word_120 = 1;
    int32_t index = 0;
    uint8_t claimed = 1;
    uint64_t vtable = 0;
    int read_ok;

    read_ok = proc_mem_read(NULL, obj, &vtable, 8);
    if (!read_ok || (vtable & 7) != 0 || vtable < 0x1000)
        return 0;
    if (vtable != (uint64_t)g_proc_mem_bias + 0x11c0a48U)
        return 0;
    if (!proc_mem_read(NULL, obj + 0x38, &parent, 8) || parent != 0)
        return 0;
    if (!proc_mem_read(NULL, obj + 0x40, &index, 4) || index != -1)
        return 0;
    if (!proc_mem_read(NULL, obj + 0xa8, &word_a8, 8) || word_a8 != 0)
        return 0;
    if (!proc_mem_read(NULL, obj + 0x120, &word_120, 8) || word_120 != 0)
        return 0;
    if (!proc_mem_read(NULL, obj + 0x1f0, &claimed, 1))
        return 0;
    return claimed == 0;
}

/*
 * chooser_record_refresh — re-read the 0x60-byte performance snapshot
 * (magic 1, size 0x60, engageable bit) into the 0x25ca90 chooser cache.
 * @ 00164398
 */
void chooser_record_refresh(void)
{
    uint64_t snap[12];

    memset(snap, 0, sizeof snap);
    snap[0] = PERF_SNAPSHOT_MAGIC;
    if (script_port_client_performance_query((uint64_t)snap) != 0 &&
        (int32_t)snap[0] == 1 && (uint32_t)(snap[0] >> 32) == 0x60 &&
        (snap[2] & 1) != 0) {
        g_chooser_cache[5] = snap[4];   /* 0x25cab8 */
        g_chooser_cache[4] = snap[3];   /* 0x25cab0 */
        g_chooser_cache[3] = (uint64_t)(uint32_t)snap[2]; /* 0x25caa0 lo */
        g_chooser_cache[2] = snap[2];   /* 0x25ca98 */
        g_chooser_cache[1] = snap[0];   /* 0x25ca90 */
        g_chooser_cache[7] = snap[5];   /* 0x25cac8 */
        g_chooser_cache[6] = snap[6];   /* 0x25cac0 */
        g_chooser_cache[9] = snap[7];   /* 0x25cad8 */
        g_chooser_cache[8] = snap[8];   /* 0x25cad0 */
        g_chooser_cache[11] = snap[10]; /* 0x25cae8 */
        g_chooser_cache[10] = snap[9];  /* 0x25cae0 */
    }
}
/* misc_a chain chunk 16: covers raw lines 7051-7500 */

/* ===== forward declarations within this chain ===== */
uint64_t button_lineage_check(uint64_t node, uint64_t screen);     /* c07 @ 00154eb4 */
int  visible_chain_check(uint64_t node, uint64_t target);          /* c08 @ 0015516c */
int  remote_child_link_valid(uint64_t node, uint64_t parent);      /* c07 @ 00154d5c */
void screen_text_feed(const screen_record_t *rec, uint64_t node,
                      const char *text);                           /* below @ 00165224 */
void result_text_feed(const screen_record_t *rec, uint64_t node,
                      const char *text);                           /* below @ 00165360 */
uint64_t engine_view_object_get(uint64_t target);       /* below @ 001668ec */
int  tracked_screen_slot_update(const screen_record_t *rec,
                                uint64_t node);                   /* below @ 00166d64 */

/* ===== cross-file functions ===== */
extern int  font_chooser_snapshot_read(void *out);   /* misc.c (b-chain) @ 00191df8 */
extern void plus_field_text_fill(char *buf, size_t cap, ...); /* misc.c (b-chain) @ 0016ba38 — __vsnprintf_chk trampoline */
extern void menu_stage_log(int err, const char *stage, const char *action,
                           uint32_t category);  /* menu_engine.c @ 00156760 */
extern void *__memchr_chk(const void *s, int c, size_t n, size_t s_len);

/* ===== tracked-screen cache (0x2815d8 block, 0x338 bytes) ===== */
uint64_t g_tracked_node;        /* 0x2815d8 — tracked screen node */
uint64_t g_tracked_word1;       /* 0x2815e0 — active node (first slot's node) */
uint64_t g_tracked_word2;       /* 0x2815e8 — tracked button node */
uint64_t g_tracked_count_a;     /* 0x2815f0 — text-capture count a */
uint64_t g_tracked_count_b;     /* 0x2815f8 — text-capture count b */
uint64_t g_tracked_identity;    /* 0x281600 — screen identity */
uint64_t g_tracked_stamp;       /* 0x281608 — tracked rows (0x25c9d8 +4 low) */
uint64_t g_tracked_slots[3*0x20];  /* 0x281610 — {node} per slot */
uint64_t g_tracked_parents[3*0x20];/* 0x281618 — {parent} per slot */
uint32_t g_tracked_dists[6*0x20];  /* 0x281620 — {distance, countdown} per slot */
extern uint64_t g_track_scan_stamp; /* 0x281a18 — next scan stamp (rec+0x32) */
extern uint8_t g_track_enabled;     /* 0x22d8e8 — 1 = capture tracking on */

/* tracked-screen node cache (filled by text feeds) */
extern uint64_t g_view_node_cache;   /* 0x2815d0 — last seen node (from +0xe0 reads) */

/*
 * performance_mode_save — persist the performance mode to the sealed
 * diagnostics dir as "performance.bin": a 20-byte NXP69252 record
 * ("NXP69252" magic, mode byte at +8, then a CRC32 of the first 16
 * bytes) written via "performance-<pid>-<seq>.tmp", fsync, rename, dir
 * fsync. Gates: mode must be 0/1; no battle-state bit set (else 2); a
 * no-op when the mode is already current and nothing is pending; the
 * sequence word (0x25cb28, -1 disables) must be live and the files-dir
 * fd valid. On success the pending words are cleared, the mode is
 * committed, and a "performance_mode" event is logged
 * (saved_optimization_on/off, category 0x20002). Returns 2 on success,
 * 1 no-op, 2/4 on gates, 0xffffffff on I/O failure (tmp unlinked,
 * errno preserved). @ 001645e0
 */
uint64_t performance_mode_save(uint32_t mode, uint64_t stamp)
{
    uint8_t rec[16];
    char path[0x50];
    uint64_t done = 0;
    uint64_t seq;
    ssize_t n;
    int fd, dirfd = g_host_session;
    int saved_errno;

    (void)stamp;  /* saved to 0x22d8a8 only after the mode word commits */

    if (1 < mode)
        return 4;
    if ((g_battle_state >> 32 & 1) != 0 || (g_battle_word1 & 1) != 0)
        return 2;
    if ((g_battle_word2 & 1) != 0 || (g_battle_word3 & 1) != 0)
        return 2;
    if ((uint32_t)g_battle_state == mode && g_perf_confirmed == 0 &&
        g_battle_word4 == 0 && (g_perf_latch & 1) == 0 && g_perf_seq == 0)
        return 1;
    if (g_perf_save_seq == 0xffffffffu)
        return 0;
    if (dirfd < 0)
        return 0;
    seq = (uint64_t)++g_perf_save_seq;

    rec[0] = 0x4e;  /* 'N' */
    rec[1] = 0x58;  /* 'X' */
    rec[2] = 0x50;  /* 'P' */
    rec[3] = 0x36;  /* '6' */
    rec[4] = 0x39;  /* '9' */
    rec[5] = 0x32;  /* '2' */
    rec[6] = 0x35;  /* '5' */
    rec[7] = 0x32;  /* '2' */
    rec[8] = 1;
    rec[9] = 0;
    rec[10] = 0;
    rec[11] = 0;
    rec[12] = (uint8_t)mode;
    rec[13] = (uint8_t)(mode >> 8);
    rec[14] = (uint8_t)(mode >> 0x10);
    rec[15] = (uint8_t)(mode >> 0x18);
    *(uint32_t *)(rec + 0x10) = crc32_bytes(rec, 16);

    str_format(path, 0x50, 0x50, "performance-%d-%llu.tmp", (int)getpid(), seq);
    fd = openat(dirfd, path, 0x880c1, 0x180);
    if (fd < 0)
        return 0;
    do {
        n = __write_chk(fd, rec + done, 0x14 - done, (size_t)-1);
        if (n < 0) {
            if (errno != EINTR)
                goto write_failed;
        } else {
            done += (uint64_t)n;
            if (n == 0)
                break;
        }
    } while (done < 0x14);
    if (done == 0x14) {
        int wsync = fsync(fd);
        int wclose = close(fd);

        if (wclose == 0 && wsync == 0 &&
            renameat(dirfd, path, dirfd, "performance.bin") == 0) {
            int dsync = fsync(dirfd);

            g_perf_pending = 0;          /* 0x22d898 */
            g_perf_saved_hi = 0;         /* 0x22d8a0 */
            g_perf_confirmed = dsync != 0;  /* 0x22dfbc */
            *(uint32_t *)&g_battle_state = mode;  /* 0x22d880 low */
            if (dsync == 0) {
                g_perf_seq = 0;          /* 0x22f4b8 */
                g_battle_word4 = 0;      /* 0x22d894 */
                g_perf_latch = 0;        /* 0x22dfb8 */
                g_perf_save_count = 0;   /* 0x25cb30 */
                g_battle_word2 = 1;      /* 0x22d88c: mode visible */
                g_perf_saved_stamp = (uint32_t)stamp; /* 0x22d8a8 */
                menu_stage_log(0, "performance_mode",
                              mode != 0 ? "saved_optimization_on"
                                        : "saved_optimization_off",
                              0x20002);
                return 2;
            }
            menu_stage_log(dsync, "performance_mode",
                          "visible_save_not_confirmed", 0x20002);
            return 0xffffffff;
        }
        return 0;
    } else {
write_failed:
        close(fd);
    }
    saved_errno = errno;
    unlinkat(dirfd, path, 0);
    errno = saved_errno;
    return 0;
}
/* misc_a chain chunk 17: covers raw lines 7296-7750 */

/* text scratch buffer shared by the screen scanner feeds */
char g_text_buf_a[0x100];          /* scanner label buffer (raw stack buf) */
uint64_t g_view_parent_cache;      /* 0x281618 — slot-1 parent of the root */
uint32_t g_hook_inflight;          /* 0x22db38 — capture in-flight word */

/* ===== forward declarations within this chain ===== */
int  end_text_classify(const char *text, int32_t *value_out);      /* c02 @ 0014d544 */
int  visible_chain_check(uint64_t node, uint64_t target);          /* c08 @ 0015516c */
uint64_t button_lineage_check(uint64_t node, uint64_t screen);     /* c07 @ 00154eb4 */
int  remote_child_link_valid(uint64_t node, uint64_t parent);      /* c07 @ 00154d5c */
int  remote_read_ptr(uint64_t addr, uint64_t *out);                /* c07 @ 00154cf0 */
uint64_t widget_class_ancestor_find(uint64_t node,
                                    uint64_t root);               /* below @ 00167218 */
int screen_node_resolve(uint64_t node, uint64_t root,
                        uint64_t *out);                            /* below @ 00166430 */
int button_widget_resolve(uint64_t node, uint64_t root,
                          uint64_t *out);                          /* below @ 00165944 */
int  tracked_screen_slot_update(const screen_record_t *rec,
                                uint64_t node);                   /* below @ 00166d64 */
uint64_t engine_view_object_get(uint64_t target);       /* below @ 001668ec */
void screen_text_feed(const screen_record_t *rec, uint64_t node,
                      const char *text);                           /* below @ 00165224 */
void result_text_feed(const screen_record_t *rec, uint64_t node,
                      const char *text);                           /* below @ 00165360 */
void results_tracker_text_event(results_tracker_t *trk,
                                const screen_record_t *rec,
                                const char *text,
                                const tracker_record_t *ev);       /* c02 @ 0014dca4 */

/* ===== cross-file functions ===== */
extern void *__memchr_chk(const void *s, int c, size_t n, size_t s_len);

/* ===== tracked-screen view record (0x38 bytes, see c02) =====
 * screen_record_t: stamp/identity/node/flag + counts + close windows. */

/* 0x50-byte tracked-node capture record laid out as in c02's
 * tracker_record_t; slot values come from tracked_screen_slot_update. */

/*
 * screen_scan_pump — per-frame scan of the tracked screen record's
 * widget tree. When the record's identity pair changes the whole
 * tracked-screen cache (0x2815d8, 0x338 bytes) is reset. With counts
 * present and the scan stamp due (stamp + 0x32), the tree is walked
 * breadth-first over the cached slot table (up to 0x1f new slots per
 * frame; a wall-clock jump > 1 ms between slot groups ends the frame):
 * each slot's node must still be linked to its parent (else the slot is
 * dropped and, when the node is a descendant of the screen, re-adopted);
 * a fresh node gets its vtable checked — widget-class nodes
 * (libg+0x11c7b88) run the full capture path (lineage check, +0x248
 * value captured into the 0x22db18 hook block, tracked_screen_slot_
 * update, ancestor refresh, +0x208/+0xe0 labels fed through
 * screen_text_feed/result_text_feed), widget objects (libg+0x11c0a48)
 * with a +0x208 label (length 0xa1..0xff, no NUL) feed screen_text_
 * feed, and class-0x11abad8 nodes with a +0xe0 label feed result_text_
 * feed; child-array distances/countdowns (min 0x200) are maintained per
 * slot. New plausible children (parent-linked, not already slotted) are
 * appended while there is room. @ 00164978
 */
void screen_scan_pump(screen_record_t *rec)
{
    struct timespec ts;
    uint64_t now_ms;
    uint64_t base;
    uint16_t dist;
    uint32_t slot;
    uint32_t i;
    if (g_tracked_node != rec->node || g_tracked_identity != rec->identity) {
        memset((void *)&g_tracked_node, 0, 0x338);
        g_tracked_identity = rec->identity;
        g_tracked_node = rec->node;
        g_track_scan_stamp = 0;
    }
    if (rec->count_a == 0 || rec->count_b_hi != 0)
        return;
    if (g_track_scan_stamp > rec->stamp)
        return;
    g_track_scan_stamp = rec->stamp + 0x32;

    base = engine_view_object_get((uint64_t)rec);
    {
        uint64_t root = g_tracked_word2 != 0 ? g_tracked_word2 : rec->node;

        if (g_tracked_word1 != root || g_tracked_stamp == 0) {
            g_tracked_stamp = 1;
            ts.tv_sec = 0;
            g_tracked_word1 = root;
            if (!proc_mem_read(NULL, root + 0x38, &ts, 8) ||
                ((uint64_t)ts.tv_sec & 7) != 0 || (uint64_t)ts.tv_sec < 0x1000)
                g_view_parent_cache = 0;   /* 0x281618 slot 1 */
            g_tracked_slots[0] = root;     /* 0x281610 slot 0 */
            g_view_parent_cache = (uint64_t)ts.tv_sec;
        }
    }
    if (clock_gettime(1 /* CLOCK_MONOTONIC */, &ts) != 0)
        now_ms = 0;
    else
        now_ms = (uint64_t)ts.tv_sec * 1000u + (uint64_t)ts.tv_nsec / 1000000u;

    if (g_tracked_stamp == 0)
        return;
    for (i = 0;; i++) {
        uint64_t node;

        if (i != 0 && (i & 7) == 0) {
            /* wall-clock sanity between slot groups */
            if (clock_gettime(1, &ts) != 0)
                break;
            {
                uint64_t t = (uint64_t)ts.tv_sec * 1000u +
                             (uint64_t)ts.tv_nsec / 1000000u;

                if (now_ms - 1 < t && 1 < t - now_ms)
                    break;
            }
        }
        slot = g_tracked_stamp - 1;
        node = g_tracked_slots[slot * 3];
        if (node == rec->node) {
handle:
            if (!visible_chain_check(node, rec->node))
                goto drop_slot;
            dist = 0;
            if (*(int *)&g_tracked_dists[slot * 6] == 0) {
                bool enabled = g_track_enabled == 1;

                *(int *)&g_tracked_dists[slot * 6] = 1;
                if (enabled && rec->count_a != 0) {
                    uint64_t screen = button_lineage_check(node, rec->node);

                    if (screen != 0) {
                        uint64_t value = 0;

                        if (proc_mem_read(NULL, node + 0x248, &value, 8) &&
                            value >= 0x1000 && (value & 7) == 0) {
                            if (g_hook_module_handle == node) {
                                if (g_hook_last_a == rec->node &&
                                    g_hook_last_b == value)
                                    goto capture_done;
                            }
                            /* adopt the node into the hook block */
                            g_hook_last_c = rec->identity;
                            *(uint32_t *)&g_hook_inflight =   /* 0x22db38 */
                                (g_guard_last_c != 0 &&
                                 g_guard_last_a == rec->node &&
                                 g_guard_last_b == rec->identity);
                            g_hook_module_handle = node;
                            g_hook_last_a = rec->node;
                            g_hook_last_b = value;
                        }
                    }
                }
capture_done:;
            }
            {
                uint64_t vtable = 0;
                uint64_t label_rec[6];
                uint64_t len;

                if (!proc_mem_read(NULL, node, &vtable, 8))
                    vtable = 0;
                if (vtable >= 0x1000 && (vtable & 7) == 0 &&
                    vtable == (uint64_t)g_proc_mem_bias + 0x11c7b88U) {
                    /* widget-class node: full capture path */
                    uint64_t w = button_lineage_check(node, rec->node);

                    if (w != 0 && tracked_screen_slot_update(rec, node)) {
                        if (g_tracked_word2 != node) {
                            g_tracked_count_a = 0;
                            g_tracked_count_b = 0;
                            g_tracked_word2 = node;
                        }
                        g_tracked_identity = rec->identity;
                        g_tracked_node = rec->node;
                        base = engine_view_object_get((uint64_t)rec);
                        if (base != node) {
                            g_tracked_stamp = 0;
                            break;
                        }
                    }
                    /* +0x208 label -> screen_text_feed */
                    vtable = 0;
                    if (proc_mem_read(NULL, node, &vtable, 8) &&
                        vtable >= 0x1000 && (vtable & 7) == 0 &&
                        vtable == (uint64_t)g_proc_mem_bias + 0x11c0a48U &&
                        node + 0x208 != 0 &&
                        proc_mem_read(NULL, node + 0x208, label_rec, 0x10)) {
                        len = label_rec[1];
                        if (0xffffff60 < len - 0xa0) {
                            uint64_t src = len > 7 ? label_rec[0]
                                                   : node + 0x210;

                            if (src != 0 &&
                                proc_mem_read(NULL, src, g_text_buf_a,
                                              (uint32_t)len) &&
                                __memchr_chk(g_text_buf_a, 0, len, 0xa0) == NULL) {
                                g_text_buf_a[len] = 0;
                                screen_text_feed(rec, node, (char *)g_text_buf_a);
                            }
                        }
                    }
                    /* +0xe0 label -> result_text_feed */
                    vtable = 0;
                    if (proc_mem_read(NULL, node, &vtable, 8) &&
                        vtable >= 0x1000 && (vtable & 7) == 0 &&
                        vtable == (uint64_t)g_proc_mem_bias + 0x11abad8U &&
                        node + 0xe0 != 0 &&
                        proc_mem_read(NULL, node + 0xe0, label_rec, 0x10)) {
                        len = label_rec[1];
                        if (0xffffff60 < len - 0xa0) {
                            uint64_t src = len > 7 ? label_rec[0]
                                                   : node + 0xe8;

                            if (src != 0 &&
                                proc_mem_read(NULL, src, g_text_buf_a,
                                              (uint32_t)len) &&
                                __memchr_chk(g_text_buf_a, 0, len, 0xa0) == NULL) {
                                g_text_buf_a[len] = 0;
                                result_text_feed(rec, node, (char *)g_text_buf_a);
                            }
                        }
                    }
                }
                /* distance/countdown maintenance */
                dist = 0;
                if (proc_mem_read(NULL, node + 0x4e, &dist, 2) &&
                    dist < 0x201)
                    g_tracked_dists[slot * 6] = dist;
            }
            if (g_tracked_dists[slot * 6] == 0 || 0x1f < g_tracked_stamp ||
                !proc_mem_read(NULL, node + 0x4e, &dist, 2) ||
                dist == 0 || 0x200 < dist) {
drop_slot:
                g_tracked_stamp--;
            } else {
                uint64_t array;
                uint64_t child;
                uint32_t d;

                if (dist < g_tracked_dists[slot * 6])
                    g_tracked_dists[slot * 6] = dist;
                if (!proc_mem_read(NULL, node + 0x50, &array, 8) ||
                    array < 0x1000 || (array & 7) != 0)
                    goto drop_slot;
                d = g_tracked_dists[slot * 6] - 1;
                g_tracked_dists[slot * 6] = d;
                if (!proc_mem_read(NULL, array + (uint64_t)d * 8, &child, 8) ||
                    child < 0x1000 || (child & 7) != 0 ||
                    !remote_child_link_valid(child, node)) {
                    goto drop_slot;
                }
                /* append the child when not already slotted */
                {
                    uint32_t n = g_tracked_stamp;

                    for (i = 0; i < n; i++)
                        if (g_tracked_slots[i * 3] == child)
                            goto next_slot;
                    g_tracked_stamp++;
                    g_tracked_slots[n * 3] = child;
                    g_tracked_parents[n * 3] = node;
                    *(uint64_t *)&g_tracked_dists[n * 6] = 0;
                }
            }
        } else {
            /* non-root slot: verify the parent link, re-adopt descendants */
            if (!remote_child_link_valid(node, g_tracked_parents[slot * 3]))
                goto drop_slot;
            goto handle;
        }
next_slot:
        if (0x7e < i)
            break;
        if (g_tracked_stamp == 0)
            break;
    }
}

/* text scratch buffer shared by the screen scanner feeds (0xa0+1 bytes,
 * matching the raw local at 0x281xxx usage) */
char g_text_buf_a[0x100];          /* scanner label buffer (raw stack buf) */
uint64_t g_view_parent_cache;      /* 0x281618 — slot-1 parent of the root */
uint32_t g_hook_inflight;          /* 0x22db38 — capture in-flight word */

/*
 * remote_string_read — read a remote {len, ptr} string object into out:
 * the 0x10-byte header at obj must read (len in [0xa1, 0xff]; the data
 * pointer is inline at +8 when len <= 7, else at +0x10), the payload
 * read must contain no NUL, and out[len] is terminated. Returns 1 on
 * success, 0 otherwise. @ 0016515c
 */
int remote_string_read(uint64_t obj, void *out)
{
    uint32_t hdr[4];
    uint64_t src;
    size_t len;
    int read_ok;

    if (obj == 0)
        return 0;
    read_ok = proc_mem_read(NULL, obj, hdr, 0x10);
    if (!read_ok)
        return 0;
    len = hdr[1];
    if (0xffffff60 < hdr[1] - 0xa0) {
        src = obj + 8;
        if (7 < hdr[1])
            src = *(uint64_t *)&hdr[2];
        if (src != 0) {
            if (proc_mem_read(NULL, src, out, (uint32_t)len)) {
                if (memchr(out, 0, len) == NULL) {
                    ((char *)out)[len] = 0;
                    return 1;
                }
            }
        }
    }
    return 0;
}

/*
 * screen_text_feed — feed a screen-node label to the results tracker.
 * The text is classified; class 2 refresh labels go straight into the
 * tracker (only when the record has no pending event and the tracked
 * node's class-ancestor resolves or screen_node_resolve finds it), and
 * class 1 play-again labels additionally resolve the button widget and
 * update the tracked slot cache. Other classes just feed the tracker
 * with the screen record. @ 00165224
 */
void screen_text_feed(const screen_record_t *rec, uint64_t node,
                      const char *text)
{
    int32_t value = 0;
    tracker_record_t ev;
    int klass;
    uint64_t w;

    memset(&ev, 0, sizeof ev);
    klass = end_text_classify(text, &value);
    if (klass == 2) {
        if (rec->count_b_hi != 0 || rec->flag != 0)
            return;
        if (widget_class_ancestor_find(node, rec->node) != 0)
            goto feed;
        if (screen_node_resolve(node, rec->node, (uint64_t *)&ev) == 0)
            return;
        goto feed;
    }
    if (klass != 1)
        return;
    w = widget_class_ancestor_find(node, rec->node);
    if (!button_widget_resolve(w, rec->node, (uint64_t *)&ev))
        return;
    if (ev.subject != node)
        return;
    if (!tracked_screen_slot_update(rec, w))
        return;
    if (g_tracked_word2 != w) {
        g_tracked_count_a = 0;
        g_tracked_count_b = 0;
        g_tracked_word2 = w;
    }
    g_tracked_identity = rec->identity;
    g_tracked_node = rec->node;

feed:
    results_tracker_text_event(&g_results_tracker, rec, text, &ev);
}

/*
 * result_text_feed — feed a class-0x11abad8 node's result text to the
 * tracker: the node's vtable must be 0x11abad8, its ancestor chain must
 * reach the record's node, the chain must be visible, and the node's
 * class-ancestor must resolve. The record's identity pair is re-stamped
 * into the tracked-screen cache; classes 3-5 (win/loss/other) store the
 * node into the count slots, class 7 stores it into the b slot, and
 * class 1 resolves the button widget before feeding. Everything is
 * handed to results_tracker_text_event. @ 00165360
 */
void result_text_feed(const screen_record_t *rec, uint64_t node,
                      const char *text)
{
    int32_t value = 0;
    tracker_record_t ev;
    uint64_t vtable = 0;
    uint64_t cur = node;
    uint64_t root = rec->node;
    uint64_t parent = 0;
    uint32_t hops = 0;
    int klass, read_ok;
    bool plausible;

    memset(&ev, 0, sizeof ev);
    if (!proc_mem_read(NULL, node, &vtable, 8))
        return;
    if (vtable < 0x1000 || (vtable & 7) != 0)
        return;
    if (vtable != (uint64_t)g_proc_mem_bias + 0x11abad8U)
        return;
    if (node != 0 && root != node) {
        do {
            read_ok = proc_mem_read(NULL, cur + 0x38, &parent, 8);
            plausible = read_ok && parent >= 0x1000 && (parent & 7) == 0;
            if (!plausible)
                parent = 0;
            read_ok = remote_child_link_valid(cur, parent);
            if (read_ok)
                cur = parent;
        } while (read_ok && parent != root && ++hops < 0x1f && plausible);
    }
    if (cur != root)
        return;
    if (!visible_chain_check(node, rec->node))
        return;
    {
        uint64_t w = widget_class_ancestor_find(node, rec->node);

        if (w == 0)
            return;
        klass = end_text_classify(text, &value);
        if (!tracked_screen_slot_update(rec, w))
            return;
        if (g_tracked_word2 != w) {
            g_tracked_count_a = 0;
            g_tracked_count_b = 0;
            g_tracked_word2 = w;
        }
        g_tracked_identity = rec->identity;
        g_tracked_node = rec->node;
        if ((uint32_t)(klass - 3) < 3) {
            g_tracked_count_a = node;   /* 0x2815f0 */
        } else if (klass == 7) {
            g_tracked_count_b = node;   /* 0x2815f8 */
        } else if (klass == 1) {
            if (!button_widget_resolve(w, rec->node, (uint64_t *)&ev))
                return;
            if (ev.subject != node)
                return;
        }
        results_tracker_text_event(&g_results_tracker, rec, text, &ev);
    }
}
/* misc_a chain chunk 18: covers raw lines 7751-8200 */

/* ===== forward declarations within this chain ===== */
int  end_text_classify(const char *text, int32_t *value_out);      /* c02 @ 0014d544 */
uint64_t button_lineage_check(uint64_t node, uint64_t screen);     /* c07 @ 00154eb4 */
int  visible_chain_check(uint64_t node, uint64_t target);          /* c08 @ 0015516c */
int  remote_child_link_valid(uint64_t node, uint64_t parent);      /* c07 @ 00154d5c */
int screen_node_resolve(uint64_t node, uint64_t root,
                        uint64_t *out);                            /* below @ 00166430 */
int button_widget_resolve(uint64_t node, uint64_t root,
                          uint64_t *out);                          /* below @ 00165944 */
int  tracked_screen_slot_update(const screen_record_t *rec,
                                uint64_t node);                    /* below @ 00166d64 */
int  results_record_still_valid(const int64_t *rec);               /* below @ 0016563c */
int screen_record_text_read(const uint64_t *rec,
                            char *out);                           /* below @ 00167000 */
uint64_t tracked_screen_slot_index(uint64_t node,
                                   uint64_t value);                /* below @ 0016733c */
void results_tracker_text_event(results_tracker_t *trk,
                                const screen_record_t *rec,
                                const char *text,
                                const tracker_record_t *ev);       /* c02 @ 0014dca4 */
void screen_text_feed(const screen_record_t *rec, uint64_t node,
                      const char *text);                           /* c17 @ 00165224 */
void result_text_feed(const screen_record_t *rec, uint64_t node,
                      const char *text);                           /* c17 @ 00165360 */

/* ===== rodata: default plus-service accessor literals ===== */
extern const uint32_t PLUS_SVC_RVA_A;   /* 0x10fa30 — service-object vtable rva */
extern const uint32_t PLUS_SVC_RVA_B;   /* 0x10fa38 — service-object rva */

/* ===== end-screen capture state (0x22d928 block) ===== */
extern uint64_t g_endcap_node;      /* 0x22d928 — captured screen node */
extern uint64_t g_endcap_identity;  /* 0x22d930 — captured screen identity */
extern int64_t g_endcap_rec[10];    /* 0x22d938 — captured end-screen record */
extern uint64_t g_endcap_word50;    /* 0x22d970 — captured +0x50 word */
extern uint64_t g_endcap_word58;    /* 0x22d978 — captured +0x58 word */

/* plus-service observed state (0x22d9c8 block, published here) */
extern uint64_t g_plus_obs_identity;   /* 0x22d9c8 — observed screen identity */
extern uint64_t g_plus_obs_word10;     /* 0x22d9d8 — first observed node */
extern uint64_t g_plus_obs_last;       /* 0x22d9e8 — last distinct node */
extern uint64_t g_plus_obs_stamp;      /* 0x22da08 — observed screen stamp */
extern uint64_t g_plus_obs_stamp2;     /* 0x22da18 — stamp echo */
extern uint64_t g_plus_obs_field10;    /* 0x22da50 — observed +0xb0 field (hi dword) */
extern uint64_t g_plus_obs_valid_a;    /* 0x22da64 — observation valid a */
extern uint64_t g_plus_obs_valid_b;    /* 0x22da6c — observation valid b */
extern uint64_t g_plus_obs_word68;     /* 0x22da7c — gate word (hi dword must be 0) */
extern uint64_t g_plus_obs_gate;       /* 0x22da84 — observation gate */
extern uint64_t g_plus_obs_90;         /* 0x22d8f8 — observed record word */
extern uint64_t g_plus_obs_98;         /* 0x22d8f0 base alias — the tracker */

/*
 * plus_service_object_create — allocate a 0x98-byte plus-service object
 * and register it with the engine service at module+0x13053c0. The
 * accessor bundle must carry {module base, ctx, read fn, alloc fn,
 * register fn}: the service pointer must read back a plausible object
 * whose vtable is module+0x11ebcd0 and whose +0x18 word is
 * module+0xac319c; then a 0x98-byte object is allocated, initialized
 * with (1, 0) through the init fn and registered for the service slot.
 * Returns 1 on success. @ 00165768
 */
int plus_service_object_create(void *accessor)
{
    struct {
        uint64_t base;    /* +0x00 module base */
        void *ctx;        /* +0x08 */
        int (*read)(void *ctx, uint64_t addr, void *out, uint32_t len);  /* +0x10 */
        void *(*alloc)(uint64_t size);                                   /* +0x18 */
        void (*init)(void *obj, int a, int b);                           /* +0x20 */
        void (*register_fn)(uint64_t service, void *obj);                /* +0x28 */
    } *const acc = accessor;
    uint64_t service = 0;
    uint64_t vtable = 0;
    uint64_t word18 = 0;
    void *obj;

    if (accessor == NULL || acc->read == NULL || acc->alloc == NULL ||
        acc->init == NULL || acc->register_fn == NULL)
        return 0;
    if (!acc->read(acc->ctx, acc->base + 0x13053c0, &service, 8))
        return 0;
    if (service < 0x1000 || (service & 7) != 0)
        return 0;
    if (!acc->read(acc->ctx, service, &vtable, 8))
        return 0;
    if (vtable != acc->base + 0x11ebcd0)
        return 0;
    if (!acc->read(acc->ctx, vtable + 0x18, &word18, 8))
        return 0;
    if (word18 != acc->base + 0xac319c)
        return 0;
    obj = acc->alloc(0x98);
    if (obj == NULL)
        return 0;
    acc->init(obj, 1, 0);
    acc->register_fn(service, obj);
    return 1;
}

/*
 * plus_service_object_create_default — plus_service_object_create with
 * the default accessor: libg base (g_proc_mem_bias), proc_mem_read, and
 * the engine fns at libg+0xac319c and the rodata rvas 0x10fa30/38.
 * @ 001658c4
 */
void plus_service_object_create_default(void)
{
    struct {
        uint64_t base;
        void *ctx;
        int (*read)(void *ctx, uint64_t addr, void *out, uint32_t len);
        void *(*alloc)(uint64_t size);
        void (*init)(void *obj, int a, int b);
        void (*register_fn)(uint64_t service, void *obj);
    } acc;

    acc.base = (uint64_t)g_proc_mem_bias;
    acc.ctx = NULL;
    acc.read = proc_mem_read;
    acc.alloc = (void *(*)(uint64_t))((uint64_t)g_proc_mem_bias + 0xac319c);
    acc.init = (void (*)(void *, int, int))((uint64_t)g_proc_mem_bias +
                                            PLUS_SVC_RVA_A);
    acc.register_fn = (void (*)(uint64_t, void *))((uint64_t)g_proc_mem_bias +
                                                   PLUS_SVC_RVA_B);
    plus_service_object_create(&acc);
}

/*
 * button_widget_resolve — resolve the button widget owned by node+0x1b0's
 * view: the view pointer must be plausible, screen_node_resolve must
 * accept {view, root}, the resolved node's ancestor chain (links
 * re-validated, <= 0x1f hops) must climb back to node, and the record's
 * +0x20 word must be node+0x80 with the +0x28 word equal to libg+
 * 0x91bb7c. Returns 1 on success. @ 00165944
 */
int button_widget_resolve(uint64_t node, uint64_t root, uint64_t *out)
{
    uint64_t view = 0;
    uint64_t cur;
    uint64_t parent = 0;
    uint32_t hops = 0;
    int read_ok;
    bool plausible;

    /* raw calls button_lineage_check with no args (implicit node pair) */
    if (!proc_mem_read(NULL, node + 0x1b0, &view, 8))
        return 0;
    if (view < 0x1000 || (view & 7) != 0)
        return 0;
    if (!screen_node_resolve(view, root, out))
        return 0;
    cur = out[0];
    if (cur != node && cur != 0) {
        do {
            read_ok = proc_mem_read(NULL, cur + 0x38, &parent, 8);
            plausible = read_ok && parent >= 0x1000 && (parent & 7) == 0;
            if (!plausible)
                parent = 0;
            read_ok = remote_child_link_valid(cur, parent);
            if (read_ok)
                cur = parent;
        } while (read_ok && ++hops < 0x1f && plausible && parent != node);
    }
    if (cur == node && out[4] == node + 0x80)
        return out[5] == (uint64_t)g_proc_mem_bias + 0x91bb7cU;
    return 0;
}

/*
 * screen_node_resolve — resolve an on-screen widget node. The node must
 * have the widget vtable (libg+0x11c0a48), a plausible parent (+0x38)
 * and code handler (+0xa8, not 0xfff); its ancestor chain (re-validated,
 * <= 0x1f hops) must reach root; the chain must be visible
 * (visible_chain_check); every node on the chain must carry the +0x48
 * flag byte 1; and the handler's target (handler+8, 4-byte aligned) must
 * lie inside one of the captured executable libg segments
 * (g_slot_ranges/g_slot_range_ends, index < g_capture_pair.count).
 * On success the 10-word record {node, root, parent, vtable, handler,
 * handler target, ...} is written to out. Returns 1, else 0.
 * @ 00166430
 */
int screen_node_resolve(uint64_t node, uint64_t root, uint64_t *out)
{
    uint64_t vtable = 0;
    uint64_t parent = 0;
    uint64_t handler = 0;
    uint64_t target = 0;
    uint64_t cur = node;
    uint32_t hops = 0;
    uint8_t flag = 0;
    int read_ok;
    bool plausible;

    read_ok = proc_mem_read(NULL, node, &vtable, 8);
    if (!read_ok || (vtable & 7) != 0 || vtable < 0x1000)
        vtable = 0;
    read_ok = proc_mem_read(NULL, node + 0x38, &parent, 8);
    plausible = read_ok && parent >= 0x1000 && (parent & 7) == 0;
    if (!plausible)
        parent = 0;
    read_ok = proc_mem_read(NULL, node + 0xa8, &handler, 8);
    if (!read_ok || handler < 0xffe || (handler & 7) != 0 || handler == 0xfff)
        handler = 0;
    if (!(vtable == (uint64_t)g_proc_mem_bias + 0x11c0a48U &&
          read_ok && parent >= 0x1000 && (parent & 7) == 0 &&
          handler >= 0xffe && (handler & 7) == 0))
        return 0;

    if (node != 0 && node != root) {
        do {
            uint64_t p = 0;

            read_ok = proc_mem_read(NULL, cur + 0x38, &p, 8);
            plausible = read_ok && p >= 0x1000 && (p & 7) == 0;
            if (!plausible)
                p = 0;
            if (!remote_child_link_valid(cur, p))
                return 0;
            cur = p;
        } while (++hops < 0x1f && plausible && cur != root);
    }
    if (cur != root)
        return 0;
    if (!visible_chain_check(node, root))
        return 0;
    cur = node;
    for (;;) {
        flag = 0;
        if (!proc_mem_read(NULL, cur + 0x48, &flag, 1) || flag != 1)
            return 0;
        if (cur == root)
            break;
        parent = 0;
        read_ok = proc_mem_read(NULL, cur + 0x38, &parent, 8);
        plausible = read_ok && parent >= 0x1000 && (parent & 7) == 0;
        if (!plausible)
            parent = 0;
        if (0x1e < hops || !plausible)
            return 0;
        hops++;
        cur = parent;
    }
    read_ok = proc_mem_read(NULL, node + 0x48, &flag, 1);
    if (!read_ok || flag != 1)
        return 0;
    {
        uint64_t vec = 0;

        if (!proc_mem_read(NULL, handler, &vec, 8))
            vec = 0;
        if ((vec & 7) != 0 || vec < 0x1000)
            vec = 8;   /* raw falls back to a fixed +8 */
        if (!proc_mem_read(NULL, vec + 8, &target, 8))
            return 0;
        if ((target & 3) != 0)
            return 0;
        if ((uint32_t)(g_capture_pair >> 32) != 0)
            return 0;
        if ((uint32_t)g_capture_pair >= 9 || (uint32_t)g_capture_pair == 0)
            return 0;
        {
            uint32_t count = (uint32_t)g_capture_pair;
            uint32_t i;
            bool inside = true;

            if (target < g_slot_ranges[0] || g_slot_range_ends[0] <= target) {
                inside = 1 < count;
                for (i = 1; i < count; i++) {
                    if (target >= g_slot_ranges[i] && target < g_slot_range_ends[i])
                        goto found;
                }
                /* chained range checks as in the dump */
                if (inside && count != 1) {
                    if (target < g_slot_ranges[1] || g_slot_range_ends[1] <= target) {
                        inside = 2 < count;
                        if (inside && count != 2 &&
                            !(target >= g_slot_ranges[2] && target < g_slot_range_ends[2]))
                            inside = 3 < count;
                    }
                }
                if (!inside)
                    return 0;
            }
found:
            out[0] = node;
            out[1] = root;
            out[2] = parent;
            out[3] = vtable;
            out[4] = handler;
            out[5] = target;
            out[6] = 0;
            out[7] = 0;
            *(uint32_t *)(out + 8) = 0;
            out[9] = 0;
            return 1;
        }
    }
}
/* misc_a chain chunk 19: covers raw lines 8120-8600 */

/* ===== forward declarations within this chain ===== */
int  end_text_classify(const char *text, int32_t *value_out);      /* c02 @ 0014d544 */
int  visible_chain_check(uint64_t node, uint64_t target);          /* c08 @ 0015516c */
uint64_t button_lineage_check(uint64_t node, uint64_t screen);     /* c07 @ 00154eb4 */
int  remote_child_link_valid(uint64_t node, uint64_t parent);      /* c07 @ 00154d5c */
int  screen_node_resolve(uint64_t node, uint64_t root,
                         uint64_t *out);                            /* c18 @ 00166430 */
int  button_widget_resolve(uint64_t node, uint64_t root,
                           uint64_t *out);                          /* c18 @ 00165944 */
int  results_record_still_valid(const int64_t *rec);                /* below @ 0016563c */
int screen_record_text_read(const uint64_t *rec,
                            char *out);                           /* below @ 00167000 */
uint64_t widget_class_ancestor_find(uint64_t node,
                                    uint64_t root);                /* below @ 00167218 */
uint64_t tracked_screen_slot_index(uint64_t node,
                                   uint64_t value);                /* below @ 0016733c */
void results_tracker_text_event(results_tracker_t *trk,
                                const screen_record_t *rec,
                                const char *text,
                                const tracker_record_t *ev);       /* c02 @ 0014dca4 */
void result_text_feed(const screen_record_t *rec, uint64_t node,
                      const char *text);                           /* c17 @ 00165360 */

/* ===== cross-file functions ===== */
extern void *__memchr_chk(const void *s, int c, size_t n, size_t s_len);

/* ===== 8-slot tracked-screen table (0x281918 block) ===== */
typedef struct {
    uint64_t node;     /* +0x00 tracked node */
    uint64_t value;    /* +0x08 its +0x248 value */
    uint64_t word10;   /* +0x10 record word 1 */
    uint64_t slot;     /* +0x18 table index */
} tracked_slot_t;      /* 4 words, 0x20 stride */

tracked_slot_t g_tracked_slots8[8];  /* 0x281918 — 8 x {node, value, word, slot} */

/*
 * tracked_screen_update — refresh the tracked end-screen node and re-feed
 * the tracker. The tracked node (0x2815e8) must pass the button lineage
 * check and the slot table update (else the whole tracked cache is
 * cleared). With the node's +0x248 value resolving to a class-0x1226df0
 * object whose method table is libg+0xf3ead4 and +0xb0 word matching the
 * observation gate, the plus-service observation block is published.
 * When the endcap record matches the current screen and is still valid,
 * its stored label is re-fed to the tracker; otherwise the tracked
 * node's +0x208 label is read and, when it classifies as class 1,
 * re-fed. Finally both capture counts (0x2815f0/0x2815f8) are walked:
 * nodes descending from the tracked node get their +0xe0 labels read
 * and fed through result_text_feed. @ 001668ec
 */
void tracked_screen_update(const screen_record_t *rec)
{
    uint64_t tracked = g_tracked_word2;
    uint64_t resolved[10];
    uint64_t label_rec[2];
    char text[0x100];
    int i;

    if (button_lineage_check(tracked, rec->node) == 0 ||
        !tracked_screen_slot_update(rec, tracked)) {
        g_tracked_word2 = 0;
        g_tracked_count_a = 0;
        g_tracked_count_b = 0;
        return;
    }

    /* plus-service observation publish */
    {
        uint64_t value = 0;
        uint64_t obj = 0;
        uint64_t methods = 0;
        uint32_t field_b0 = 0;

        if (proc_mem_read(NULL, tracked + 0x248, &value, 8) &&
            value >= 0x1000 && (value & 7) == 0 &&
            proc_mem_read(NULL, value, &obj, 8) &&
            obj == (uint64_t)g_proc_mem_bias + 0x1226df0U &&
            proc_mem_read(NULL, (uint64_t)g_proc_mem_bias + 0x1226e18,
                          &methods, 8) &&
            methods == (uint64_t)g_proc_mem_bias + 0xf3ead4 &&
            proc_mem_read(NULL, value + 0xb0, &field_b0, 4) &&
            g_plus_obs_gate != 0 &&
            (uint32_t)(g_plus_obs_word68 >> 32) == 0 &&
            rec->identity == g_plus_obs_90 && g_plus_obs_identity != 0 &&
            g_plus_obs_identity == rec->identity) {
            g_plus_obs_stamp = rec->stamp;            /* 0x22da08 */
            if (g_plus_obs_word10 == 0)
                g_plus_obs_word10 = g_plus_obs_stamp;
            if (g_plus_obs_valid_b == 0 || g_plus_obs_valid_a == 0 ||
                (uint32_t)(g_plus_obs_field10 >> 32) != field_b0)
                g_plus_obs_last = g_plus_obs_stamp;
            *(uint32_t *)&g_plus_obs_field10 = field_b0;
            g_plus_obs_valid_b = 1;
            g_plus_obs_valid_a = 1;
            g_plus_obs_stamp2 = g_plus_obs_stamp;
        }
    }

    if (button_widget_resolve(tracked, rec->node, resolved)) {
        if (g_endcap_node == resolved[0] && g_endcap_identity == rec->node &&
            g_endcap_word58 == rec->identity &&
            results_record_still_valid(g_endcap_rec)) {
            if (screen_record_text_read((const uint64_t *)g_endcap_rec, text))
                results_tracker_text_event(&g_results_tracker, rec, text,
                                           (const tracker_record_t *)g_endcap_rec);
        } else if (resolved[0] + 0x208 != 0 &&
                   proc_mem_read(NULL, resolved[0] + 0x208, label_rec, 0x10) &&
                   0xffffff60 < (uint32_t)(label_rec[1] >> 32) - 0xa0) {
            uint64_t len = label_rec[1] >> 32;
            uint64_t src = resolved[0] + 0x210;

            if (7 < (uint32_t)(label_rec[1] >> 32))
                src = label_rec[0];
            if (src != 0 &&
                proc_mem_read(NULL, src, text, (uint32_t)len) &&
                __memchr_chk(text, 0, len, 0xa0) == NULL) {
                text[len] = 0;
                if (end_text_classify(text, &(int32_t){0}) == 1)
                    results_tracker_text_event(&g_results_tracker, rec, text,
                                               (const tracker_record_t *)resolved);
            }
        }
    }

    /* walk both capture counts and feed their +0xe0 labels */
    {
        uint64_t counts[2];

        counts[0] = g_tracked_count_a;   /* 0x2815f0 */
        counts[1] = g_tracked_count_b;   /* 0x2815f8 */
        for (i = 0; i < 2; i++) {
            uint64_t node = counts[i];

            if (node == 0)
                continue;
            if (node != tracked) {
                uint64_t cur = node;
                uint64_t parent = 0;
                uint32_t hops = 0;
                bool ok = true;

                while (ok) {
                    parent = 0;
                    if (!proc_mem_read(NULL, cur + 0x38, &parent, 8) ||
                        parent < 0x1000 || (parent & 7) != 0 ||
                        !remote_child_link_valid(cur, parent))
                        break;
                    cur = parent;
                    if (++hops >= 0x1f || cur == tracked)
                        break;
                }
                if (cur != tracked)
                    continue;
            }
            if (node + 0xe0 != 0 &&
                proc_mem_read(NULL, node + 0xe0, label_rec, 0x10) &&
                0xffffff60 < (uint32_t)(label_rec[1] >> 32) - 0xa0) {
                uint64_t len = label_rec[1] >> 32;
                uint64_t src = node + 0xe8;

                if (7 < (uint32_t)(label_rec[1] >> 32))
                    src = label_rec[0];
                if (src != 0 &&
                    proc_mem_read(NULL, src, text, (uint32_t)len) &&
                    __memchr_chk(text, 0, len, 0xa0) == NULL) {
                    text[len] = 0;
                    result_text_feed(rec, node, text);
                }
            }
        }
    }
}

/*
 * tracked_screen_slot_update — match or claim a table entry for
 * (node, its +0x248 value). The observation gate must be set and the
 * record's identity must equal the observed one. An exact match in the
 * 8-slot table (index by node/value pair) returns whether the entry's
 * record word still names the current identity; otherwise the first
 * free slot is claimed with {node, value, word10, index}. Returns 0
 * when the value is implausible, the gate is closed, the identity
 * differs or the table is full. @ 00166d64
 */
int tracked_screen_slot_update(const screen_record_t *rec, uint64_t node)
{
    uint64_t value = 0;
    int idx = -1;
    int i;

    if (!proc_mem_read(NULL, node + 0x248, &value, 8))
        return 0;
    if (value < 0x1000 || (value & 7) != 0)
        return 0;
    if (g_plus_obs_gate == 0)
        return 0;
    if (g_plus_obs_identity != rec->identity)
        return 0;

    for (i = 0; i < 8; i++) {
        if (g_tracked_slots8[i].node == node &&
            g_tracked_slots8[i].value == value) {
            idx = i;
            break;
        }
    }
    if (idx < 0) {
        for (i = 0; i < 8; i++) {
            if (g_tracked_slots8[i].node == 0) {
                idx = i;
                break;
            }
        }
        if (idx < 0)
            return 0;
        g_tracked_slots8[idx].node = node;
        g_tracked_slots8[idx].value = value;
        g_tracked_slots8[idx].word10 = rec->count_a;   /* record word 1 */
        g_tracked_slots8[idx].slot = (uint64_t)idx;
        return 1;
    }
    return g_tracked_slots8[idx].word10 == g_plus_obs_identity;
}

/*
 * screen_record_text_read — read the label text of a resolved screen
 * record. With word[9] clear the +0x208 {len, ptr} label is read
 * (inline at +0x210 when len <= 7); otherwise word[9] must be a
 * class-0x11abad8 node whose ancestor chain reaches word[0] and which is
 * visible against word[1], and its +0xe0 label is read instead. The
 * payload must contain no NUL; out[len] is terminated. Returns 1 on
 * success, 0 otherwise. @ 00167000
 */
int screen_record_text_read(const uint64_t *rec, char *out)
{
    uint64_t label[2];
    uint64_t len, src;
    int read_ok;

    if (rec[9] == 0) {
        if (rec[0] + 0x208 == 0)
            return 0;
        if (!proc_mem_read(NULL, rec[0] + 0x208, label, 0x10))
            return 0;
        if (0xffffff60 < (uint32_t)(label[1] >> 32) - 0xa0)
            goto read_payload;
        return 0;
    }

    {
        uint64_t vtable = 0;
        uint64_t cur = rec[9];
        uint64_t root = rec[0];
        uint64_t parent = 0;
        uint32_t hops = 0;
        bool plausible;

        if (!proc_mem_read(NULL, rec[9], &vtable, 8))
            return 0;
        if (vtable < 0x1000 || (vtable & 7) != 0)
            return 0;
        if (vtable != (uint64_t)g_proc_mem_bias + 0x11abad8U)
            return 0;
        if (cur != 0 && cur != root) {
            do {
                read_ok = proc_mem_read(NULL, cur + 0x38, &parent, 8);
                plausible = read_ok && parent >= 0x1000 && (parent & 7) == 0;
                if (!plausible)
                    parent = 0;
                if (!remote_child_link_valid(cur, parent))
                    return 0;
                cur = parent;
            } while (++hops < 0x1f && plausible && cur != root);
        }
        if (cur != root)
            return 0;
        if (!visible_chain_check(rec[9], rec[1]))
            return 0;
        if (rec[9] + 0xe0 == 0)
            return 0;
        if (!proc_mem_read(NULL, rec[9] + 0xe0, label, 0x10))
            return 0;
        if (0xffffff60 < (uint32_t)(label[1] >> 32) - 0xa0)
            goto read_payload_e0;
        return 0;
    }

read_payload:
    len = label[1] >> 32;
    src = rec[0] + 0x210;
    if (7 < len)
        src = label[0];
    goto read_out;
read_payload_e0:
    len = label[1] >> 32;
    src = rec[9] + 0xe8;
    if (7 < len)
        src = label[0];
read_out:
    if (src == 0)
        return 0;
    if (!proc_mem_read(NULL, src, out, (uint32_t)len))
        return 0;
    if (memchr(out, 0, len) != NULL)
        return 0;
    out[len] = 0;
    return 1;
}

/*
 * widget_class_ancestor_find — find the nearest node (self or ancestor,
 * <= 0x1f hops, links re-validated) whose vtable is libg+0x11c7b88 and
 * which passes the button lineage check against root. Returns the node,
 * 0 when none qualifies. @ 00167218
 */
uint64_t widget_class_ancestor_find(uint64_t node, uint64_t root)
{
    uint64_t cur = node;
    uint64_t vtable = 0;
    uint64_t parent = 0;
    uint32_t hops = 0;
    int read_ok;

    if (node == 0)
        return 0;
    for (;;) {
        vtable = 0;
        if (!proc_mem_read(NULL, cur, &vtable, 8))
            vtable = 0;
        if (vtable >= 0x1000 && (vtable & 7) == 0 &&
            vtable == (uint64_t)g_proc_mem_bias + 0x11c7b88U)
            break;
        if (cur == root)
            return 0;
        parent = 0;
        read_ok = proc_mem_read(NULL, cur + 0x38, &parent, 8);
        if (!read_ok || parent < 0x1000 || (parent & 7) != 0)
            return 0;
        if (0x1e < hops++)
            return 0;
        cur = parent;
    }
    if (button_lineage_check(cur, root) != 0)
        return cur;
    return 0;
}

/*
 * tracked_screen_slot_index — index (0..7) of the (node, +0x248 value)
 * pair in the 8-slot tracked-screen table, matching only when the
 * entry's slot word equals expected. Returns the index, or 0xffffffff
 * when the value is implausible or the pair is not in the table.
 * @ 0016733c
 */
uint64_t tracked_screen_slot_index(uint64_t node, uint64_t expected)
{
    uint64_t value = 0;
    int i;

    if (!proc_mem_read(NULL, node + 0x248, &value, 8))
        return 0xffffffff;
    if (value < 0x1000 || (value & 7) != 0)
        return 0xffffffff;
    if (expected == 0)
        return 0xffffffff;
    for (i = 0; i < 8; i++) {
        if (g_tracked_slots8[i].node == node &&
            g_tracked_slots8[i].value == value) {
            if (g_tracked_slots8[i].slot == expected)
                return (uint64_t)i;
            return 0xffffffff;
        }
    }
    return 0xffffffff;
}
/* misc_a chain chunk 20: covers raw lines 8601-9100 */

/* ===== forward declarations within this chain ===== */
int  allocator_valid_check(const engine_accessor_t *acc,
                           uint64_t block);                   /* c08 @ 00155980 */
int  allocator_state_read(const engine_accessor_t *acc,
                          allocator_state_t *out);            /* c04 @ 00150710 */
int  allocator_fields_probe(const engine_accessor_t *acc,
                            uint64_t block);                  /* c05 @ 001508c8 */
int  integrity_sweep_verify(void);                            /* c04 @ 001505d8 */
int  code_region_hash_verify(uint64_t offset, size_t len,
                             const char *expect_hex);         /* c01 @ 0014d168 */
int allocator_pair_resolve(const void *accessor, uint64_t *block_a,
                           uint64_t *block_b);                /* below @ 00168580 */

/* ===== allocator-state snapshot pairs (0x22dfc0 block) ===== */
extern uint64_t g_alloc_snap_block;   /* 0x22dfc0 — saved block, state A */
extern uint64_t g_alloc_snap_word8;   /* 0x22dfc8 — saved +0x88 dwords, state A */
extern uint8_t  g_alloc_snap_mode;    /* 0x22dfd0 — saved mode byte, state A */
extern uint64_t g_alloc_snap_block_b; /* 0x22dfd8 — saved block, state B */
extern uint64_t g_alloc_snap_word8_b; /* 0x22dfe0 — saved +0x88 dwords, state B */
extern uint8_t  g_alloc_snap_mode_b;  /* 0x22dfe8 — saved mode byte, state B */
extern uint32_t g_alloc_snap_active;  /* 0x22dff0 — 1 = state A saved */
extern uint32_t g_alloc_snap_current; /* 0x22dff4 — which state is current */
extern uint32_t g_alloc_snap_failed;  /* 0x22dff8 — 1 = toggle failed */

/* frame-time stamp word */
extern uint64_t g_frame_time_ms;      /* 0x281a20 — frame time stamp (ms) */

/*
 * engine_global_pair_read — double-read the engine-global pair: the u32
 * at libg+0x12429c0, then through the pointer at libg+0x12eb7f8 (whose
 * +0x60 and +0x50 chains must both resolve to plausible pointers) read
 * the 8-byte pair; both dwords must be in [0x2001, 0x3fff] (game event
 * ids). A re-read of the chain must return the same objects, and the
 * u32 at libg+0x12429c0 must read back its own low dword — then it is
 * stored into *out. Returns 1 on success, 0 otherwise. @ 00167524
 */
int engine_global_pair_read(int32_t *out, int32_t pair[2])
{
    uint32_t word = 0;
    uint64_t node = 0, mid = 0, leaf = 0;

    if (!proc_mem_read(NULL, (uint64_t)g_proc_mem_bias + 0x12429c0, &word, 4))
        return 0;
    if (!proc_mem_read(NULL, (uint64_t)g_proc_mem_bias + 0x12eb7f8, &node, 8))
        return 0;
    if (node < 0x1000 || (node & 7) != 0)
        return 0;
    if (!proc_mem_read(NULL, node + 0x60, &mid, 8))
        return 0;
    if (mid < 0x1000 || (mid & 7) != 0)
        return 0;
    if (!proc_mem_read(NULL, mid + 0x50, &leaf, 8))
        return 0;
    if (leaf < 0x1000 || (leaf & 7) != 0)
        return 0;
    if (!proc_mem_read(NULL, leaf, pair, 8))
        return 0;
    if (0xffffdfff < (uint32_t)(pair[0] - 0x2001) &&
        0xffffdfff < (uint32_t)(pair[1] - 0x2001))
        return 0;
    /* re-resolve the chain and verify it is unchanged */
    node = 0;
    if (!proc_mem_read(NULL, (uint64_t)g_proc_mem_bias + 0x12eb7f8, &node, 8))
        return 0;
    if (node < 0x1000 || (node & 7) != 0)
        return 0;
    if (!remote_read_ptr(node + 0x60, &mid) || mid == 0)
        return 0;
    if (!remote_read_ptr(mid + 0x50, &leaf) || leaf == 0)
        return 0;
    word = 0;
    if (!proc_mem_read(NULL, (uint64_t)g_proc_mem_bias + 0x12429c0, &word, 4))
        return 0;
    if ((int32_t)word == pair[1]) {
        *out = (int32_t)word;
        return 1;
    }
    return 0;
}

/*
 * integrity_master_gate — the master integrity gate: six hashed code
 * regions (0x75bc3c, 0x4f8058, 0x4f852c, 0x4f8c30, 0x75b2a4, 0x75b6c0),
 * then the build tag at libg+0x2063d8 must be -0x700000010, and finally
 * integrity_sweep_verify must pass. Returns 1 when everything verifies.
 * @ 00167750
 */
int integrity_master_gate(void)
{
    uint64_t build_tag = 0;

    if (!code_region_hash_verify(0x75bc3c, 0x14,
            "2ac86ecf2f5fb5847b475b3bfb248d09dce96f195be09baf2a92361f584dfdcd"))
        return 0;
    if (!code_region_hash_verify(0x4f8058, 0x2c,
            "b1ea48a31bf8a6d35cd2fa451b1b8828c4be25f6bf6155b3a4c368096a3e783c"))
        return 0;
    if (!code_region_hash_verify(0x4f852c, 0xc,
            "db1b20fa8a89ea7479d2f2f151e289071cc1a4ce360f1fdd89e17d4ea5369cb5"))
        return 0;
    if (!code_region_hash_verify(0x4f8c30, 0x58,
            "b1d1fbf008ffbf58da7338a11324129f384c5d69162665812119e8d59c87f649"))
        return 0;
    if (!code_region_hash_verify(0x75b2a4, 0x414,
            "2c953180e20e98212607df431d583d48d31444e0a72d9d60bf5e2d018b1354a0"))
        return 0;
    if (!code_region_hash_verify(0x75b6c0, 0x57c,
            "209b658c77125de592aadd9ddfa5daf55f83dde458d259176c867f5258dd5756"))
        return 0;
    if (!proc_mem_read(NULL, (uint64_t)g_proc_mem_bias + 0x2063d8,
                       &build_tag, 8))
        return 0;
    if (build_tag != 0xf000000010ULL /* -0x700000010 */)
        return 0;
    return integrity_sweep_verify() != 0;
}

/*
 * frame_time_stamp_forward — stamp the frame time (CLOCK_MONOTONIC in
 * ms) into 0x281a20, set the battle word 0x22d890 and forward the frame
 * to the engine frame hook at libg+0x75cddc. @ 00167888
 */
void frame_time_stamp_forward(void *ctx, uint64_t frame)
{
    struct timespec ts;

    (void)ctx;
    if (clock_gettime(1 /* CLOCK_MONOTONIC */, &ts) == 0)
        g_frame_time_ms = (uint64_t)ts.tv_sec * 1000u +
                           (uint64_t)ts.tv_nsec / 1000000u;
    else
        g_frame_time_ms = 0;
    g_battle_word3 = 1;   /* 0x22d890 */
    ((void (*)(uint64_t))((uint64_t)g_proc_mem_bias + 0x75cddc))(frame);
}

/*
 * allocator_cycle_check — alloc/free round-trip check over the raw
 * accessor bundle {base, ctx, read(+0x10), alloc(+0x18), free(+0x20),
 * cycle hook(+0x28)}: resolve both allocator objects
 * (allocator_pair_resolve), read the mode byte at block+0x101 (must be
 * 0), allocate through the alloc fn, re-resolve (both objects must be
 * unchanged), re-check the mode byte (still 0), optionally call the
 * cycle hook on the block, free through the +0x20 slot, and verify the
 * mode byte became 1. Returns 2 when the mode byte was already set, 1
 * on a verified cycle, 0xffffffff when the final read fails, 0 on gate
 * failures. @ 00167e78
 */
int64_t allocator_cycle_check(const void *accessor)
{
    const int64_t *acc = accessor;
    uint64_t block_a = 0, block_b = 0;
    uint64_t again_a = 0, again_b = 0;
    uint8_t mode = 0;
    void *obj;
    int rc;

    if (accessor == NULL)
        return 0;
    if (!(0x12ff040 < (uint64_t)(uint64_t)acc[0] + 0x12ff040U))
        return 0;
    if (acc[2] == 0 || acc[3] == 0 || acc[4] == 0)
        return 0;
    if (!allocator_pair_resolve(accessor, &block_a, &block_b))
        return 0;
    if (!((int (*)(void *, uint64_t, void *, uint32_t))acc[2])(
            (void *)acc[1], block_a + 0x101, &mode, 1) || 1 < mode)
        return 0;
    if (mode != 0)
        return 2;
    obj = ((void *(*)(void *))acc[3])((void *)acc[1]);
    if (obj == NULL)
        return 0;
    if (!allocator_pair_resolve(accessor, &again_a, &again_b))
        return 0;
    if (block_a != again_a || block_b != again_b)
        return 0;
    if (!((int (*)(void *, uint64_t, void *, uint32_t))acc[2])(
            (void *)acc[1], block_a + 0x101, &mode, 1) || mode != 0)
        return 0;
    if (acc[5] != 0) {
        if (((int64_t (*)(void *, uint64_t))acc[5])((void *)acc[1],
                                                    block_a) == 0)
            return 0;
    }
    ((void (*)(void *, uint64_t))acc[4])((void *)acc[1], block_a);
    rc = ((int (*)(void *, uint64_t, void *, uint32_t))acc[2])(
        (void *)acc[1], block_a + 0x101, &mode, 1);
    if (mode == 1 && rc != 0)
        return 1;
    return 0xffffffff;
}
/* misc_a chain chunk 21: covers raw lines 8827-9300 */

/* ===== forward declarations within this chain ===== */
int  allocator_valid_check(const engine_accessor_t *acc,
                           uint64_t block);                   /* c08 @ 00155980 */
int  allocator_state_read(const engine_accessor_t *acc,
                          allocator_state_t *out);            /* c04 @ 00150710 */
int  allocator_fields_probe(const engine_accessor_t *acc,
                            uint64_t block);                  /* c05 @ 001508c8 */
int  arm64_branch_encode(uint64_t from, uint64_t to,
                         uint32_t *out);                      /* b-chain @ 00190d68 */
int  allocator_pair_resolve(const void *accessor, uint64_t *out_a,
                            uint64_t *out_b);                 /* below @ 00168580 */


/* ===== allocator-state snapshot globals (see c20 externs) ===== */

/*
 * allocator_state_toggle — swap the allocator block between two saved
 * states (0x22dfc0/0x22dfd8 pairs). The accessor must resolve the block
 * (allocator_state_read, matching param block) and both the +0x88
 * dwords and the mode byte must be writable. With no state saved yet
 * (or the opposite one active), the current state is captured; with the
 * matching state saved, the block's state is swapped: the new state is
 * applied through allocator_fields_probe (validating with
 * allocator_valid_check), re-read for consistency, and committed; a
 * failed swap sets the failed latch (0x22dff8) and returns
 * 0xffffffff. Returns 1 on success, 0 on gate failures. @ 00168028
 */
uint64_t allocator_state_toggle(const void *accessor, uint64_t block,
                                uint32_t which)
{
    const int64_t *acc = accessor;
    allocator_state_t snap;
    struct {
        uint64_t block;      /* saved block */
        uint64_t word88;     /* saved +0x88 dwords */
        uint8_t mode;        /* saved mode byte */
    } saved;
    uint64_t pair = 0;
    uint8_t mode = 0;

    if (accessor == NULL || 1 < which || g_alloc_snap_failed != 0)
        return 0;
    if (acc[3] == 0 || acc[4] == 0)
        goto fail;
    if (!allocator_state_read((const engine_accessor_t *)accessor, &snap))
        return 0;
    if (snap.block != block)
        return 0;
    /* poke both fields to confirm writability */
    if (!((int (*)(void *, uint64_t, uint64_t))acc[4])((void *)acc[1],
                                                       block + 0x88, 8))
        return 0;
    if (!((int (*)(void *, uint64_t, uint64_t))acc[4])(
            (void *)acc[1], (uint64_t)acc[0] + 0x12f03e4, 1))
        return 0;

    if (g_alloc_snap_active != 0 && g_alloc_snap_block != block) {
        if (g_alloc_snap_current == 0) {
            g_alloc_snap_active = 0;
save:
            if (which == 0) {
                g_alloc_snap_word8_b = snap.field_a | ((uint64_t)snap.field_b << 32);
                g_alloc_snap_mode_b = snap.mode;
                g_alloc_snap_block_b = snap.block;
            } else {
                g_alloc_snap_word8 = snap.field_a | ((uint64_t)snap.field_b << 32);
                g_alloc_snap_mode = snap.mode;
                g_alloc_snap_block = snap.block;
                g_alloc_snap_active = 1;
            }
            goto apply;   /* fall through to the verify/commit path */
        }
        goto fail;
    }
    if (g_alloc_snap_current == 0)
        goto save;
    /* the matching state is saved: verify it still matches the block */
    if (snap.block != g_alloc_snap_block_b ||
        snap.field_a != (uint32_t)g_alloc_snap_word8_b ||
        snap.field_b != (uint32_t)(g_alloc_snap_word8_b >> 32) ||
        snap.mode != g_alloc_snap_mode_b)
        goto fail;
    if (which != 0) {
apply:
        pair = which == 0 ? g_alloc_snap_word8 : g_alloc_snap_word8_b;
        saved.block = which == 0 ? g_alloc_snap_block : g_alloc_snap_block_b;
        mode = which == 0 ? g_alloc_snap_mode : g_alloc_snap_mode_b;
    } else {
        pair = g_alloc_snap_word8;
        saved.block = g_alloc_snap_block;
        mode = g_alloc_snap_mode;
    }

    /* re-read the block state to verify it is unchanged */
    if (!allocator_state_read((const engine_accessor_t *)accessor, &snap))
        return 0;
    if (snap.block == saved.block &&
        snap.field_a == (uint32_t)pair &&
        snap.field_b == (uint32_t)(pair >> 32) &&
        snap.mode == mode) {
        /* state matches: just commit the selection */
        g_alloc_snap_word8_b = pair;
        g_alloc_snap_block_b = saved.block;
        g_alloc_snap_mode_b = mode;
        g_alloc_snap_current = which;
        return 1;
    }
    /* apply the other state through the probe/validate path */
    if (!allocator_valid_check((const engine_accessor_t *)accessor, block))
        goto fail;
    if (!allocator_fields_probe((const engine_accessor_t *)accessor, block))
        goto fail;
    if (!allocator_state_read((const engine_accessor_t *)accessor, &snap))
        goto fail;
    if (snap.block == block &&
        snap.field_a == (uint32_t)pair &&
        snap.field_b == (uint32_t)(pair >> 32) &&
        snap.mode == mode) {
        g_alloc_snap_word8_b = pair;
        g_alloc_snap_block_b = block;
        g_alloc_snap_mode_b = mode;
        g_alloc_snap_current = which;
        return 1;
    }
    g_alloc_snap_failed = 1;
    return 0xffffffff;

fail:
    return 0;
}

/*
 * allocator_state_commit — commit a new state to the allocator block
 * through the raw accessor {base, ctx, read, write(+0x10)}: the block
 * must still resolve (pointer at base+0x12ff038 equal, vtable
 * base+0x11baa20), both the +0x88 dwords and the mode byte must be
 * poked writable, the new state {block, +0x88 pair, mode byte} is
 * written, and the read-back must match all three fields. Returns 1 on
 * a verified commit, 0 otherwise. @ 00168410
 */
int allocator_state_commit(const void *accessor, const uint64_t *state)
{
    const int64_t *acc = accessor;
    uint64_t block = 0;
    uint64_t vtable = 0;
    allocator_state_t snap;

    if (accessor == NULL || state == NULL)
        return 0;
    if (!((int (*)(void *, uint64_t, void *, uint32_t))acc[2])(
            (void *)acc[1], (uint64_t)acc[0] + 0x12ff038, &block, 8))
        return 0;
    if (block != state[0])
        return 0;
    if (!((int (*)(void *, uint64_t, void *, uint32_t))acc[2])(
            (void *)acc[1], block, &vtable, 8))
        return 0;
    if (vtable != (uint64_t)acc[0] + 0x11baa20)
        return 0;
    if (!((int (*)(void *, uint64_t, uint64_t))acc[4])((void *)acc[1],
                                                       state[0] + 0x88, 8))
        return 0;
    if (!((int (*)(void *, uint64_t, uint64_t))acc[4])(
            (void *)acc[1], (uint64_t)acc[0] + 0x12f03e4, 1))
        return 0;
    ((void (*)(void *, uint64_t, const void *, uint64_t))acc[3])(
        (void *)acc[1], state[0] + 0x88, state + 1, 8);
    if (!allocator_state_read((const engine_accessor_t *)accessor, &snap))
        return 0;
    if (snap.block == state[0] &&
        snap.field_a == (uint32_t)state[1] &&
        snap.field_b == (uint32_t)(state[1] >> 32))
        return snap.mode == (uint8_t)state[2];
    return 0;
}

/*
 * allocator_pair_resolve — resolve the dual allocator objects through
 * the raw accessor: the blocks at base+0x12ff038 (the 0x238-byte
 * allocator) and base+0x12eafa0 (the 0x70-byte secondary) must both be
 * plausible (>= 0x238 / >= 0x70 bytes below the 64-bit wrap, aligned),
 * the primary's vtable must be base+0x11baa20 with its +0x1b8 byte and
 * +0x190 word clear, the secondary's first word must lie in
 * [base+0x11a8410, base+0x123efa0] aligned, and the pointer at
 * secondary+0x58 must be base+0x4f8c30. Returns 1 with both blocks in
 * out_a/out_b. @ 00168580
 */
int allocator_pair_resolve(const void *accessor, uint64_t *out_a,
                           uint64_t *out_b)
{
    const int64_t *acc = accessor;
    uint8_t primary[0x238];
    uint64_t secondary[0xe];
    uint64_t chain = 0;

    if (accessor == NULL)
        return 0;
    if (!((int (*)(void *, uint64_t, void *, uint32_t))acc[2])(
            (void *)acc[1], (uint64_t)acc[0] + 0x12ff038, out_a, 8))
        return 0;
    {
        uint64_t a = *out_a;

        *out_a = 0;
        if (!((int (*)(void *, uint64_t, void *, uint32_t))acc[2])(
                (void *)acc[1], (uint64_t)acc[0] + 0x12eafa0, out_b, 8))
            return 0;
        *out_a = a;
    }
    if (*out_a == 0 || *out_a >= 0xfffffffffffffdc8ULL ||
        (*out_a & 7) != 0 || *out_b == 0 ||
        *out_b >= 0xffffffffffffff90ULL || (*out_b & 7) != 0)
        return 0;
    if (!((int (*)(void *, uint64_t, void *, uint32_t))acc[2])(
            (void *)acc[1], *out_a, primary, 0x238))
        return 0;
    if (!((int (*)(void *, uint64_t, void *, uint32_t))acc[2])(
            (void *)acc[1], *out_b, secondary, 0x70))
        return 0;
    {
        uint64_t base = (uint64_t)acc[0];

        if (*(uint64_t *)primary != base + 0x11baa20U)
            return 0;
        if (primary[0x1b8] != 0)          /* +0x1b8 byte clear */
            return 0;
        if (*(int *)(primary + 0x190) != 0)
            return 0;
        if (secondary[0] < base + 0x11a8410U ||
            base + 0x123efa0U < secondary[0] || (secondary[0] & 7) != 0)
            return 0;
        chain = 0;
        if (!((int (*)(void *, uint64_t, void *, uint32_t))acc[2])(
                (void *)acc[1], secondary[0] + 0x58, &chain, 8))
            chain = 0;
        if (chain != base + 0x4f8c30)
            return 0;
        return 1;
    }
}

/* ===== plus-service request execution ===== */

/* raw accessor bundle for the plus-service object fns */
typedef struct {
    void *vtable;      /* +0x00 JNI-style vtable (slots below) */
    void *unused;      /* +0x08 */
} plus_service_vt_t;

/* vtable slots (byte offsets, /8 for the word index) */
#define PSVT_ALLOC       0x10u   /* (size) -> obj */
#define PSVT_INIT_A      0x18u   /* (obj, words, 0) — 0x7d init */
#define PSVT_INIT_B      0x20u   /* (obj, w0, w1, 0, 0) — 0x80 init */
#define PSVT_REGISTER    0x28u   /* (service, obj) -> ok */
#define PSVT_INVOKE      0x30u   /* (args) -> ok — 0x7d with payload */

/*
 * plus_service_resolve — resolve the engine plus-service object through
 * the raw accessor {base, read}: the pointer at base+0x13053c0 must be a
 * plausible object whose vtable is base+0x11ebcd0 and whose +0x18 word
 * is base+0xac319c. Returns the service object, 0 on failure.
 * @ 001688f0
 */
uint64_t plus_service_resolve(const void *accessor)
{
    const int64_t *acc = accessor;
    uint64_t service = 0;
    uint64_t vtable = 0;
    uint64_t word18 = 0;

    if (accessor == NULL || acc[1] == 0)
        return 0;
    if (!((int (*)(void *, uint64_t, void *, uint32_t))acc[1])(
            0, (uint64_t)acc[0] + 0x13053c0, &service, 8))
        return 0;
    if (service == 0 || (service & 7) != 0)
        return 0;
    if (!((int (*)(void *, uint64_t, void *, uint32_t))acc[1])(
            0, service, &vtable, 8))
        return 0;
    if (vtable != (uint64_t)acc[0] + 0x11ebcd0)
        return 0;
    if (!((int (*)(void *, uint64_t, void *, uint32_t))acc[1])(
            0, vtable + 0x18, &word18, 8))
        return 0;
    if (word18 != (uint64_t)acc[0] + 0xac319c)
        return 0;
    return service;
}

/*
 * plus_service_request_exec — execute a plus request on the engine
 * service (resolved via plus_service_resolve). payload must be < 0x100
 * words. Mode 0x7d with a payload present invokes the service method
 * directly ({payload[0], payload[1]} as the 8-byte argument); modes
 * 0x7d/0x80 allocate 0xa0-byte records (1 for 0x7d, 2 for 0x80), fill
 * them through the init slots (0x7d: the payload pair; 0x80: two dwords
 * and two zeros) and register each with the service. Returns 1 when all
 * records registered, 2 when only some did, 0 on gate failures.
 * @ 00168764
 */
uint32_t plus_service_request_exec(void *service_vt, int32_t mode,
                                   const uint32_t *payload, int has_payload)
{
    uint64_t service;
    uint64_t obj;
    uint64_t arg;
    int count, i, done = 0;

    service = plus_service_resolve(service_vt);
    if (payload == NULL || service == 0)
        return 0;
    if (payload[0] < 0x100) {
        if (mode == 0x7d && has_payload != 0) {
            uint8_t bytes[4] = {0};
            uint64_t words = payload[0] | ((uint64_t)payload[1] << 32);

            arg = ((int (*)(void *, void *))(*(long **)service_vt)
                   [PSVT_INVOKE / 8])(bytes, &words);
            return arg != 0;
        }
        if (mode == 0x80 || mode == 0x7d) {
            count = mode == 0x80 ? 2 : 1;
            for (i = 0; i < count; i++) {
                obj = (uint64_t)(uintptr_t)((void *(*)(uint64_t))(*(long **)service_vt)
                       [PSVT_ALLOC / 8])(0xa0);
                if (obj == 0)
                    return (uint32_t)(done != 0) << 1;
                if (mode == 0x7d) {
                    arg = *(const uint64_t *)payload;
                    ((void (*)(uint64_t, void *, int))(*(long **)service_vt)
                     [PSVT_INIT_A / 8])(obj, &arg, 0);
                } else {
                    ((void (*)(uint64_t, uint32_t, uint32_t, int, int))(
                         *(long **)service_vt)[PSVT_INIT_B / 8])(
                        obj, payload[0], payload[1], 0, 0);
                }
                if (((int (*)(uint64_t, uint64_t))(*(long **)service_vt)
                     [PSVT_REGISTER / 8])(service, obj) != 0)
                    done++;
            }
            if (done == count)
                return 1;
            return (uint32_t)(done != 0) << 1;
        }
    }
    return 0;
}

/* reload markers (rodata) */
extern const char g_reload_marker_b[];  /* 0x13baaa */
extern const char g_reload_marker_c[];  /* 0x1349fc */
extern const char g_reload_marker_d[];  /* 0x1325df */

/*
 * text_contains_reload — true when text (non-empty, <= 0xa0 bytes)
 * contains "RELOAD" case-insensitively (the dump uppercases into a
 * scratch buffer and strstr's it) or any of the three rodata reload
 * markers (0x13baaa, 0x1349fc, 0x1325df). @ 001689f8
 */
bool text_contains_reload(const char *text)
{
    char upper[0xa4];
    size_t i;

    if (text == NULL || text[0] == 0 || 0xa0 < strlen(text))
        return false;
    for (i = 0; text[i] != 0; i++) {
        char c = text[i] - 0x20;

        if (0x19 < (unsigned char)(text[i] + 0x9f))
            c = text[i];   /* not a lowercase letter */
        upper[i] = c;
    }
    upper[i] = 0;
    if (strstr(upper, "RELOAD") != NULL)
        return true;
    if (strstr(text, g_reload_marker_b /* 0x13baaa */) != NULL)
        return true;
    if (strstr(text, g_reload_marker_c /* 0x1349fc */) != NULL)
        return true;
    return strstr(text, g_reload_marker_d /* 0x1325df */) != NULL;
}
/* misc_a chain chunk 22: covers raw lines 9301-9750 */

#include <sys/mman.h>

/* ===== forward declarations within this chain ===== */
bool plus_channel_ready(void);                                    /* c13 @ 00159ac0 */
int  code_region_hash_verify_report(uint64_t offset, size_t len,
                                    const char *expect_hex);     /* c04 @ 0014fe14 */
int  arm64_branch_encode(uint64_t from, uint64_t to,
                         uint32_t *out);                         /* b-chain @ 00190d68 */
int  proc_mem_write(uint32_t fd, const void *buf, uint64_t len,
                    uint64_t addr);                               /* below/p2 @ 00153cbc */
extern void menu_engine_tick(void *tick);  /* menu_engine.c @ 0014ef90 */

/* ===== rodata ===== */
extern const char g_fail_fmt_small[];  /* 0x1362d1 — small-failure format */
extern const char g_fail_fmt_none[];   /* 0x13c160 — no-failure format */

/*
 * strategy_name_normalize — canonicalize a strategy name: only the known
 * strategies "none", "range", "maps", "alloc", "copy", "seal" and
 * "remap" pass through; anything else (including NULL) becomes
 * "unknown". @ 00168d18
 */
const char *strategy_name_normalize(const char *name)
{
    if (name == NULL)
        return "unknown";
    if (strcmp(name, "none") == 0)
        return "none";
    if (strcmp(name, "range") == 0)
        return "range";
    if (strcmp(name, "maps") == 0)
        return "maps";
    if (strcmp(name, "alloc") == 0)
        return "alloc";
    if (strcmp(name, "copy") == 0)
        return "copy";
    if (strcmp(name, "seal") == 0)
        return "seal";
    if (strcmp(name, "remap") == 0)
        return "remap";
    return "unknown";
}

/*
 * failure_reason_format — format a failure code into an 8-byte reason
 * string: -1 formats as the plain "no failure" marker (0x13c160), codes
 * <= 0x4000 as the small-failure format (0x1362d1) with the code, and
 * anything larger as "other" with the code. @ 00168dfc
 */
void failure_reason_format(char *out, uint64_t code)
{
    if (code == 0xffffffffffffffff) {
        str_format(out, (size_t)-1, 8, g_fail_fmt_none /* 0x13c160 */);
        return;
    }
    if (code < 0x4001) {
        str_format(out, (size_t)-1, 8, g_fail_fmt_small /* 0x1362d1 */, code);
        return;
    }
    str_format(out, (size_t)-1, 8, "other", code);
}

/*
 * integrity_sweep_report — verify eight libg regions (0x85c388,
 * 0x89902c, 0xf6b008, 0x7acd48, 0xf34914, 0x5418f0, 0x666228 and
 * 0xac319c) with code_region_hash_verify_report, recording
 * "body_%lx_%s" failures. Returns 1 only when every digest matches.
 * @ 00168e48
 */
int integrity_sweep_report(void)
{
    if (!code_region_hash_verify_report(0x85c388, 0xf30,
            "7ac2433a05745125eeb98a2e9fde3e961999a851ec4835c821514fc1394d5f5f"))
        return 0;
    if (!code_region_hash_verify_report(0x89902c, 200,
            "8a4832b05aaca5ba119e36d99caf4cc40fb6a3bba9a4966a8a0f029622810e57"))
        return 0;
    if (!code_region_hash_verify_report(0xf6b008, 0x58,
            "3233ddb0149f97d8f9852ba100a5f932d2a1b48ac5ec10395788f545bc360c1a"))
        return 0;
    if (!code_region_hash_verify_report(0x7acd48, 0xdc,
            "7cda3202d7931909d5191380425c5b27717be7fd502a8bad7e27773ce9be9d9f"))
        return 0;
    if (!code_region_hash_verify_report(0xf34914, 0x4c,
            "5beb7a3333c7a671d712d7b1d8554e716dd11d9049a1fda1d9ad6a1ff603dde7"))
        return 0;
    if (!code_region_hash_verify_report(0x5418f0, 0x2c,
            "ebfe0ca4dc3231e8a3387c82b01cec33a1546c5870cb87e0cf089b1dd933c647"))
        return 0;
    if (!code_region_hash_verify_report(0x666228, 0x38,
            "6c48afdef5963fe3d415619463af7ec3cc603e2fc2742f57d96d141362fb7f08"))
        return 0;
    return code_region_hash_verify_report(0xac319c, 0xe0,
            "ecc8aa9a360b1061baa9a987f682adb97f925bc7e1792933fb79367621924911");
}

/*
 * gap_page_allocate — mmap an RW anonymous page of g_trampoline_len
 * within +-0x6f00000 of the target and wire the two-word link: pages are
 * tried outward in 0x100000 steps (above first, then below); a candidate
 * is accepted when the ARM64 branches target->page and page+0x200->
 * target+4 both encode (arm64_branch_encode), else it is unmapped.
 * Returns the page, or NULL when nothing fits. @ 00168f3c
 */
void *gap_page_allocate(uint64_t target)
{
    uint64_t base = -g_trampoline_len & target;
    uint64_t step;
    uint32_t enc;

    for (step = 0x100000; step < 0x6f00001; step += 0x100000) {
        void *above = (void *)(base + step);
        void *below = step <= base ? (void *)(base - step) : NULL;

        if (above != NULL) {
            above = mmap(above, g_trampoline_len, 3 /* RW */,
                         0x22 /* MAP_PRIVATE|MAP_ANONYMOUS */, -1, 0);
            if (above != (void *)-1) {
                if (arm64_branch_encode(target, (uint64_t)above, &enc) &&
                    arm64_branch_encode((uint64_t)above + 0x200, target + 4,
                                        &enc))
                    return above;
                munmap(above, g_trampoline_len);
            }
        }
        if (below != NULL) {
            void *page = mmap(below, g_trampoline_len, 3, 0x22, -1, 0);

            if (page != (void *)-1) {
                if (arm64_branch_encode(target, (uint64_t)page, &enc) &&
                    arm64_branch_encode((uint64_t)page + 0x200, target + 4,
                                        &enc))
                    return page;
                munmap(page, g_trampoline_len);
            }
        }
    }
    return NULL;
}

/*
 * social_island_pump — the 0x85cce8 island tick: when the cached plus
 * count (0x22db6c) exceeds 10000 and plus_channel_ready holds, the
 * menu-engine tick runs (menu_engine_tick with the current ms clock) and,
 * when it reports activity, the plus count is stored into the island's
 * +0xa8 slot. @ 001690ac
 */
void social_island_pump(uint64_t island)
{
    struct {
        uint64_t stamp;   /* +0x00 ms clock */
        int active;       /* +0x40: menu engine reported activity */
    } tick;
    uint32_t count = (uint32_t)g_plus_active_b;   /* 0x22db6c plus count */

    if (0xffffd8f0 < count - 10000 && plus_channel_ready()) {
        struct timespec ts;

        if (clock_gettime(1 /* CLOCK_MONOTONIC */, &ts) == 0)
            tick.stamp = (uint64_t)ts.tv_sec * 1000u +
                         (uint64_t)ts.tv_nsec / 1000000u;
        else
            tick.stamp = 0;
        menu_engine_tick(&tick);
        if (tick.active != 0)
            *(uint64_t *)(island + 0xa8) = count;
    }
}

/* ===== social method hook ===== */

/*
 * social_method_hook — the hooked engine method: when the plus verdict
 * (0x22db68) is set, plus_channel_ready holds, the object's +0x9d claim
 * byte is clear and the argument object resolves (its length word <
 * 0x100), the service is resolved through the plus accessor, verified
 * twice (vtable 0x11ebcd0, +0x18 word 0xac319c), and the service
 * register call (libg+0x7acd48) accepts the {service, length} pair,
 * then the claim byte is set remotely (proc_mem_write of 1), the engine
 * refresh call at libg+0x11a2830 runs on the argument, and 1 is
 * returned. Otherwise the original method (0x281a40) is called.
 * @ 00169170
 */
uint32_t social_method_hook(uint64_t obj, uint64_t arg)
{
    uint8_t claim = 1;
    uint32_t length = 0;
    struct timespec ts;
    uint64_t now_ms;
    uint64_t service, vtable, word18;
    int read_ok;

    if (g_plus_active_a != 0 && plus_channel_ready()) {
        if (!proc_mem_read(NULL, obj + 0x9d, &claim, 1))
            goto original;
        if (claim != 0)
            goto original;
        if (!proc_mem_read(NULL, arg, &length, 8) || 0x100 <= length)
            goto original;
        if (clock_gettime(1, &ts) != 0)
            now_ms = 0;
        else
            now_ms = (uint64_t)ts.tv_sec * 1000u +
                     (uint64_t)ts.tv_nsec / 1000000u;
        {
            struct {
                uint64_t stamp;   /* +0x00 */
                int active;       /* +0x40 */
            } tick;

            tick.stamp = now_ms;
            menu_engine_tick(&tick);
            if (tick.active == 0)
                goto original;
        }
        /* resolve and double-verify the plus service */
        service = 0;
        if (!proc_mem_read(NULL, (uint64_t)g_proc_mem_bias + 0x13053c0,
                           &service, 8) || service == 0 || (service & 7) != 0)
            goto original;
        vtable = 0;
        if (!proc_mem_read(NULL, service, &vtable, 8))
            goto original;
        if (vtable != (uint64_t)g_proc_mem_bias + 0x11ebcd0U)
            goto original;
        word18 = 0;
        if (!proc_mem_read(NULL, vtable + 0x18, &word18, 8))
            goto original;
        if (word18 != (uint64_t)g_proc_mem_bias + 0xac319c || service == 0)
            goto original;
        /* re-verify the chain */
        {
            uint64_t service2 = 0, vtable2 = 0, word18_2 = 0;

            if (!proc_mem_read(NULL, (uint64_t)g_proc_mem_bias + 0x13053c0,
                               &service2, 8) || service2 == 0 ||
                (service2 & 7) != 0)
                goto original;
            if (!proc_mem_read(NULL, service2, &vtable2, 8) || vtable2 != vtable)
                goto original;
            read_ok = proc_mem_read(NULL, vtable2 + 0x18, &word18_2, 8);
            if (!read_ok || word18_2 != (uint64_t)g_proc_mem_bias + 0xac319c ||
                service2 == 0 || 0x100 <= length)
                goto original;
            /* register the request {service, length} */
            {
                uint64_t req[2];

                req[0] = vtable2 & 0xffffffffffffff00ULL;
                req[1] = length;
                if (((int (*)(void *, void *))((uint64_t)g_proc_mem_bias +
                                               0x7acd48))(req, req + 1) != 0) {
                    uint8_t one = 1;

                    proc_mem_write(g_host_channel_fd, &one, 1, obj + 0x9d);
                    ((void (*)(uint64_t))((uint64_t)g_proc_mem_bias +
                                          0x11a2830))(arg);
                    return 1;
                }
            }
        }
    }
original:
    return ((uint32_t (*)(uint64_t, uint64_t))g_hook_original_call)(obj, arg);
}
/* misc_a chain chunk 23: covers raw lines 9596-10116 (final chunk) */

#include <sys/stat.h>

/* ===== forward declarations within this chain ===== */
void bind_error_report_cleanup(void *st, int code);         /* below @ 001699bc */
void bind_error_report(void *st, int code);                 /* below @ 00169a50 */
extern int memfd_path_resolve_alt(const char *name, char *out);
                                /* misc.c (b-chain) @ 00169f00 — full body in the b region */
int  memfd_module_verify(void *env, const char *family,
                         const char *name, const void *expect); /* below @ 00169ae0 */

/* ===== cross-file functions ===== */
extern int  ui_int_exchange_release(int value,
                                    volatile int *addr);  /* menu_engine.c @ 00193fb0 */
extern const uint64_t BIND_EXPECT_LITERAL_A;  /* 0x10f7d8 — {1, 0x28} expected record hdr */
extern const uint64_t BIND_EXPECT_LITERAL_B;  /* 0x10f930 — {1, 0x40} expected record hdr */
extern const int32_t BINDING_STATE_OFFSETS[11]; /* 0x13cc84 — name offsets table */

/* sealed hash expectations for the bind-state records */
#define BIND_EXPECT_SHA256 \
    "a10aeb6b4085fb2a15d969a130cd9608231998b5269a40aed41129d99624ede3"
#define BIND_EXPECT_SHA1 \
    "fdf834103d333f9f8a1947b3a405b32da6ebb651"

/*
 * bind_state_init — arm the 0x128-byte bind state from two contract
 * records: req must be {1, 0x38} with its +0x10/+0x30/+0x40/+0x50 words
 * non-NULL, and cred must be {1, 0x30} whose +0x10 SHA-256 hex and
 * +0x20 SHA-1 hex match the sealed expectations and whose +0x50 word
 * has no bits above bit 0x27. On success the whole state is cleared and
 * re-seeded from the two records (request identity at +0x40.., keys at
 * +0x80.., cred pair at +0x10..) and the armed flag at +0x124 is set.
 * Returns 1, or 0xffffffff on any gate failure. @ 00169420
 */
int64_t bind_state_init(uint64_t *st, const int64_t *req, const int64_t *cred)
{
    if (st == NULL || req == NULL || cred == NULL)
        return 0xffffffff;
    if (!(req[0] == 1 && (uint32_t)((uint64_t)req[0] >> 32) == 0x38))
        return 0xffffffff;
    if (!(cred[0] == 1 && (uint32_t)((uint64_t)cred[0] >> 32) == 0x30))
        return 0xffffffff;
    if (req[2] == 0 || req[3] == 0 || req[4] == 0 || req[5] == 0)
        return 0xffffffff;
    if ((char *)cred[2] == NULL || strcmp((char *)cred[2], BIND_EXPECT_SHA256) != 0)
        return 0xffffffff;
    if ((char *)cred[4] == NULL || strcmp((char *)cred[4], BIND_EXPECT_SHA1) != 0)
        return 0xffffffff;
    if ((uint64_t)cred[10] >> 0x28 != 0)
        return 0xffffffff;

    /* clear the whole state block, then seed it */
    memset(st, 0, 0x128);
    st[0xb] = req[3];
    st[10] = req[2];
    st[0xd] = req[5];
    st[0xc] = req[4];
    st[0xe] = req[6];
    st[9] = req[1];
    st[8] = req[0];
    st[5] = cred[3];
    st[4] = cred[2];
    st[7] = cred[5];
    st[6] = cred[4];
    st[3] = cred[1];
    st[2] = cred[0];
    *(uint32_t *)((char *)st + 0x124) = 1;
    return 1;
}

/*
 * bind_state_wake_set — set the wake flag (state +4) on an armed bind
 * state. @ 00169558
 */
void bind_state_wake_set(void *st)
{
    if (st != NULL && *(int *)((char *)st + 0x124) != 0)
        *(int *)((char *)st + 4) = 1;
}

/*
 * bind_state_value_read — read the bind state's value word (+8); an
 * unarmed state yields the default 8. @ 00169574
 */
uint64_t bind_state_value_read(void *st)
{
    if (st != NULL && *(int *)((char *)st + 0x124) != 0)
        return *(uint64_t *)((char *)st + 8);
    return 8;
}

/*
 * bind_request_handle — validate and process a bind request record
 * against the armed bind state. The state must be armed and the request
 * live; a no-op request returns 0 immediately. The request must be
 * {1, 0x30} with a plausible, page-aligned cookie, non-NULL name/key
 * strings matching the armed state's, and (when already bound) the same
 * cookie/pointer/stamp triple — a mismatch reports error 8 and clears
 * the state. Unbound requests take the latch (0x259+ debounce of 0xfa),
 * open the service handle through the state's connect slot (record
 * {1, 0x28} with four non-NULL words), store the request identity,
 * verify the credentials through the verify slot ({1, 0x40} with the
 * armed keys), commit through the commit slot (results 2 -> error 4,
 * 1 -> bound), then wake the state (result 1 -> bound) reporting the
 * state (7). Returns 1 when fully bound, the reported error code, 2
 * latch-busy, 0 no-op, 0xffffffff invalid. @ 00169594
 */
uint64_t bind_request_handle(uint32_t *st, const int64_t *req, uint64_t now,
                             int live)
{
    uint64_t result;

    if (st == NULL || st[0x49] == 0)
        return 0xffffffff;
    if (live == 0 || (int)*(uint64_t *)(st + 1) == 0)
        return 0;
    if ((ui_latch_test_and_set(1, (volatile int *)st) & 1) != 0)
        return 2;
    if (st[0x49] == 0 || 7 < (int)*(uint64_t *)(st + 2))
        return 0xffffffff;

    if (req == NULL || req[0] != 1 ||
        (uint32_t)((uint64_t)req[0] >> 32) != 0x30 ||
        0x7ffffffffebeffff < (uint64_t)req[2] - 0x10000 ||
        ((uint64_t)req[2] & 0xfff) != 0 || req[4] == 0 ||
        (char *)req[2] == NULL || (char *)req[3] == NULL ||
        strcmp((char *)req[2], (char *)*(uint64_t *)(st + 6)) != 0 ||
        strcmp((char *)req[3], (char *)*(uint64_t *)(st + 8)) != 0 ||
        (st[0x4b] != 0 &&
         ((*(uint64_t *)(st + 0x3a) != (uint64_t)req[2] ||
           *(uint64_t *)(st + 0x40) != (uint64_t)req[4] ||
           *(uint64_t *)(st + 0x42) != (uint64_t)req[5])))) {
        result = 8;
        bind_error_report_cleanup(st, (int)result);
        *st = 0;
        return 0xffffffff;
    }

    if (st[0x4c] == 0 || st[0x4d] == 0) {
        if (st[0x4a] == 0 ||
            (*(uint64_t *)(st + 0x46) <= now &&
             0xf9 < now - *(uint64_t *)(st + 0x46))) {
            uint64_t opened[8];
            int rc;

            st[0x4a] = 1;
            if (*(int64_t *)(st + 0x44) != -1)
                (*(int64_t *)(st + 0x44))++;
            *(uint64_t *)(st + 0x46) = now;
            if (st[0x4c] == 0 && *(int64_t *)(st + 0x20) == 0) {
                memset(opened, 0, sizeof opened);
                opened[0] = BIND_EXPECT_LITERAL_A;   /* {1, 0x28} */
                rc = ((int (*)(uint64_t, void *)) * (uint64_t *)(st + 0x14))(
                    *(uint64_t *)(st + 0x12), opened);
                if (rc == 1 &&
                    (int)opened[0] == 1 &&
                    (uint32_t)((uint64_t)opened[0] >> 32) == 0x28 &&
                    opened[1] != 0 && opened[2] != 0 && opened[3] != 0 &&
                    opened[4] != 0) {
                    *(int64_t *)(st + 0x20) = opened[1];
                    *(uint64_t *)(st + 0x1e) = opened[0];
                    *(int64_t *)(st + 0x24) = opened[3];
                    *(int64_t *)(st + 0x22) = opened[2];
                    *(int64_t *)(st + 0x26) = opened[4];
                    *(uint64_t *)(st + 0x3e) = (uint64_t)req[3];
                    *(uint64_t *)(st + 0x3c) = (uint64_t)req[2];
                    *(uint64_t *)(st + 0x42) = (uint64_t)req[5];
                    *(uint64_t *)(st + 0x40) = (uint64_t)req[4];
                    st[0x4b] = 1;
                    *(uint64_t *)(st + 0x3a) = (uint64_t)req[2];
                    *(uint64_t *)(st + 0x38) = (uint64_t)req[0];
                    goto verify;
                }
                if (opened[1] != 0)
                    ((void (*)(uint64_t)) * (uint64_t *)(st + 0x16))(
                        *(uint64_t *)(st + 0x12));
                if (rc >= 0) {
                    bind_error_report(st, 1);
                    *st = 0;
                    return 0;
                }
                result = 9;
                bind_error_report_cleanup(st, (int)result);
                *st = 0;
                return 0xffffffff;
            }
verify:
            rc = ((int (*)(const void *)) * (uint64_t *)(st + 0x26))(req);
            if (rc == 1) {
                uint64_t creds[8];
                uint32_t cr;

                memset(creds, 0, sizeof creds);
                creds[0] = BIND_EXPECT_LITERAL_B;   /* {1, 0x40} */
                cr = ((uint32_t (*)(void *)) * (uint64_t *)(st + 0x24))(creds);
                if (cr < 4 && cr != 1) {
                    result = 3;
                } else if (cr != 1 ||
                           (int)creds[0] != 1 ||
                           (uint32_t)((uint64_t)creds[0] >> 32) != 0x40 ||
                           creds[5] == 0 || creds[4] == 0 || creds[3] == 0 ||
                           (int64_t)creds[8 - 1] != *(int64_t *)(st + 10) ||
                           (int64_t)creds[6] != *(int64_t *)(st + 0xc) ||
                           (int64_t)creds[7] != *(int64_t *)(st + 0xe)) {
                    result = 10;
                    bind_error_report_cleanup(st, (int)result);
                    *st = 0;
                    return 0xffffffff;
                } else {
                    cr = ((uint32_t (*)(uint64_t, int, void *)) *
                          (uint64_t *)(st + 0x18))(*(uint64_t *)(st + 0x12),
                                                    0, creds);
                    if ((cr & 0xfffffffe) == 2) {
                        result = 4;
                    } else if (cr == 1) {
                        st[0x4c] = 1;
                        *(uint64_t *)(st + 0x2a) = creds[1];
                        *(uint64_t *)(st + 0x28) = creds[0];
                        *(int64_t *)(st + 0x2e) = creds[6];
                        *(int64_t *)(st + 0x2c) = creds[8 - 1];
                        *(int64_t *)(st + 0x32) = creds[5];
                        *(int64_t *)(st + 0x30) = creds[7];
                        *(int64_t *)(st + 0x36) = creds[3];
                        *(int64_t *)(st + 0x34) = creds[4];
                        bind_error_report(st, 7);
                        goto wake;
                    } else {
                        result = 5;
                    }
                }
            } else if (rc != 0) {
                result = 9;
                bind_error_report_cleanup(st, (int)result);
                *st = 0;
                return 0xffffffff;
            } else {
                result = 2;
            }
            bind_error_report(st, (int)result);
            *st = 0;
            return 0;
        }
        return 0;
    }
    return 1;

wake:
    if (((int (*)(uint64_t)) * (uint64_t *)(st + 0x1a))(
            *(uint64_t *)(st + 0x12)) == 1) {
        st[0x4d] = 1;
        bind_error_report(st, 7);
        *st = 0;
        return 1;
    }
    result = 6;
    bind_error_report(st, (int)result);
    *st = 0;
    return 0;
}

/*
 * bind_error_report_cleanup — report an error code through the state
 * callback (max 12 reports, only on a code change) and release the
 * cached 0x80-block when the state is not reporting: the cached handle
 * is closed through the +0x58 slot and the five cached words are
 * cleared. @ 001699bc
 */
void bind_error_report_cleanup(void *st, int code)
{
    int prev;

    if (*(int *)((char *)st + 0x130) == 0 &&
        *(int64_t *)((char *)st + 0x80) != 0) {
        ((void (*)(uint64_t)) * (uint64_t *)((char *)st + 0x58))(
            *(uint64_t *)((char *)st + 0x48));
        *(uint64_t *)((char *)st + 0x98) = 0;
        *(uint64_t *)((char *)st + 0x90) = 0;
        *(uint64_t *)((char *)st + 0x88) = 0;
        *(uint64_t *)((char *)st + 0x80) = 0;
        *(uint64_t *)((char *)st + 0x78) = 0;
    }
    prev = ui_int_exchange_release(code, (volatile int *)((char *)st + 8));
    if (prev != code &&
        *(void **)((char *)st + 0x70) != NULL &&
        *(uint32_t *)((char *)st + 0x120) < 0xc) {
        (*(uint32_t *)((char *)st + 0x120))++;
        ((void (*)(uint64_t, int, uint64_t)) * (uint64_t *)((char *)st + 0x70))(
            *(uint64_t *)((char *)st + 0x48), code,
            *(uint64_t *)((char *)st + 0x110));
    }
}

/*
 * bind_error_report — report an error code through the state callback
 * (max 12 reports, only on a code change). @ 00169a50
 */
void bind_error_report(void *st, int code)
{
    int prev;

    prev = ui_int_exchange_release(code, (volatile int *)((char *)st + 8));
    if (prev != code &&
        *(void **)((char *)st + 0x70) != NULL &&
        *(uint32_t *)((char *)st + 0x120) < 0xc) {
        (*(uint32_t *)((char *)st + 0x120))++;
        ((void (*)(uint64_t, int, uint64_t)) * (uint64_t *)((char *)st + 0x70))(
            *(uint64_t *)((char *)st + 0x48), code,
            *(uint64_t *)((char *)st + 0x110));
    }
}

/*
 * binding_state_name — map a state index (< 0xb) to its name through
 * the 0x13cc84 offsets table; anything else is "binding_state_invalid".
 * @ 00169ab8
 */
const char *binding_state_name(uint32_t state)
{
    if (state < 0xb)
        return (const char *)((const char *)&BINDING_STATE_OFFSETS +
                              BINDING_STATE_OFFSETS[state]);
    return "binding_state_invalid";
}

/*
 * memfd_module_verify — full memfd module verification: the module name
 * must be the sealed family ("bsd.suitcase.nexusv2"), the object one of
 * the evasion libraries ("libNexusEvasion69252.so" /
 * "libNexusEvasionRuntime69252.so"), and the object's fd must resolve
 * through memfd_path_resolve_alt to a deleted "/memfd:nexus-d:" link.
 * "script-" tags take the script path (".js"/".ngs" name, 0x27-byte tag
 * with 0x20 lowercase-hex chars, 0x40-char hex digest); other tags must
 * be 0x42 chars starting with '-' with 0x40 lowercase-hex chars and a
 * name/tag match plus a 32-byte digest match. The fd is dup'ed
 * (F_DUPFD), must be a regular file owned by us with link count 0 and
 * fd flags 0xf, and is closed again. Returns 1 when the module
 * verifies. @ 00169ae0
 */
int memfd_module_verify(void *env, const char *family, const char *name,
                        const void *expect)
{
    char fd_path[14];
    char link[0x140];
    char tag[0x100];
    struct stat st;
    char *end;
    long fd_val, dup_fd;
    long flags;
    ssize_t n;
    size_t len, i;

    (void)env;
    if (family == NULL || name == NULL)
        return 0;
    if (strcmp(family, "bsd.suitcase.nexusv2") != 0)
        return 0;
    if (strcmp(name, "libNexusEvasion69252.so") != 0 &&
        strcmp(name, "libNexusEvasionRuntime69252.so") != 0)
        return 0;
    if (!memfd_path_resolve_alt(name, fd_path))
        return 0;
    n = readlink(fd_path, link, 0x13f);
    if (n < 0x18 || 0x13f < n)
        return 0;
    link[n] = 0;
    if (memcmp(link, "/memfd:nexus-d:" /* 0x6e3a64666d656d2f… */, 0x17) != 0)
        return 0;
    if (strcmp(link + n - 0xa, " (deleted)") != 0)
        return 0;
    if ((size_t)(n - 0x17) >= 0x100)
        return 0;
    memcpy(tag, link + 0x17, (size_t)(n - 0x17));
    tag[n - 0x17] = 0;
    len = strlen(tag);

    if (len >= 7 && memcmp(tag, "script-", 7) == 0) {
        if (expect == NULL || strlen((const char *)expect) != 0x40)
            return 0;
        if (strlen(name) <= 2)
            return 0;
        if (strcmp(name + strlen(name) - 3, ".js") != 0 &&
            !(strlen(name) > 3 &&
              strcmp(name + strlen(name) - 4, ".ngs") == 0))
            return 0;
        if (len != 0x27)
            return 0;
        for (i = 0; i < 0x20; i++) {
            unsigned c = (unsigned char)tag[7 + i];

            if (9 < c - 0x30 && 5 < c - 0x61)  /* not [0-9a-f] */
                return 0;
        }
        {
            const char *p = (const char *)expect;
            unsigned char c = (unsigned char)*p;

            if (c != 0)
                for (++p; (c = (unsigned char)*p) != 0; ++p) {
                    if (9 < c - 0x30 && 5 < c - 0x61)
                        return 0;
                }
        }
    } else {
        if (0x41 < len || tag[4] != '-')
            return 0;
        for (i = 0; i < 0x40; i++) {
            unsigned c = (unsigned char)tag[i];

            if (9 < c - 0x30 && 5 < c - 0x61)  /* not [0-9a-f] */
                return 0;
        }
        if (strcmp(name, tag + 5) != 0)
            return 0;
        if (expect != NULL) {
            /* the 0x40-hex tag must equal the 32-byte digest */
            char hex[0x41];
            int j;

            if (strlen((const char *)expect) != 0x40)
                return 0;
            for (j = 0; j < 32; j++)
                str_format(hex + 2 * j, (size_t)-1, 3, "%02x",
                           ((const unsigned char *)expect)[j]);
            if (memcmp(hex, tag, 0x40) != 0)
                return 0;
        }
    }

    /* re-resolve and dup/validate the fd */
    if (!memfd_path_resolve_alt(name, fd_path))
        return 0;
    errno = 0;
    fd_val = strtol(fd_path + 0xe /* past "/proc/self/fd/" */, &end, 10);
    if (errno != 0 || end == NULL || *end != 0 || fd_val < 0 ||
        0x80000000 <= fd_val)
        return 0;
    dup_fd = fcntl((int)fd_val, 0x406 /* F_DUPFD */, 0);
    if (dup_fd < 0)
        return 0;
    flags = fcntl((int)dup_fd, 0x40a /* F_GETFD */);
    if (fstat((int)dup_fd, &st) == 0 &&
        (st.st_mode & S_IFMT) == S_IFREG &&
        (uint32_t)st.st_uid == (uint32_t)getuid() &&
        st.st_nlink == 0 && flags >= 0 && (flags & 0xf) == 0xf) {
        close((int)dup_fd);
        return 1;
    }
    close((int)dup_fd);
    return 0;
}
/* misc_b chain chunk 1: covers raw lines 10117-10516 */

#define _GNU_SOURCE

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>

/* glibc fortified vsnprintf (only auto-declared under _FORTIFY_SOURCE) */
extern int __vsnprintf_chk(char *s, size_t maxlen, int flag, size_t slen,
                           const char *format, va_list ap);

/* ---- evasion-TU module verification (defined earlier in misc.c) ---- */
extern int  memfd_module_verify(void *env, const char *family,
                                const char *name,
                                const void *expect); /* misc.c (a-chain) @ 00169ae0 */

/* ---- engine base for the forwarder family (multi-part global) ---- */
extern uint64_t g_engine_base; /* 0x281a48 — libg engine image base */

/* ---- rodata glyph-class sets for font_scale_for_char ---- */
extern const char g_font_glyph_class_32[5];  /* 0x13a4b5 — 4-char class, 3.2x width */
extern const char g_font_glyph_class_26[5];  /* 0x1384ef — 4-char class, 2.6x width */
extern const char g_font_glyph_class_50[3];  /* 0x135d51 — 2-char class, 5.0x width */
extern const char g_font_glyph_class_62[5];  /* 0x13c18d — 4-char class, 6.2x width */
extern const char g_font_glyph_class_108[5]; /* 0x131fb4 — 4-char class, 10.8x width */

/* ===== evasion memfd module verification (evasion TU) ===== */

/*
 * proc_fd_path_format — build a "/proc/self/fd/<name>" path into buf.
 *
 * buf receives at most 0x40 bytes (NUL included); name is a directory
 * entry name from /proc/self/fd. No return value (the vsnprintf result is
 * discarded by the raw body). @ 0016a13c
 */
void proc_fd_path_format(char *buf, const char *name)
{
    snprintf(buf, 0x40, "/proc/self/fd/%s", name);
}

/*
 * memfd_path_resolve_alt — resolve a memfd name to its /proc/self/fd path.
 *
 * name is either a plain "/proc/self/fd/<digits>" path (total length < 0x40,
 * every char after the prefix must be below ':' — digits) which is copied
 * verbatim, or a "/memfd:nexus-…" name (length < 0x140) which is looked up by
 * scanning /proc/self/fd readlink targets. The fd path of the unique match is
 * written to out (needs >= 0x40 bytes). Returns 1 when exactly one distinct
 * file (same st_dev + st_ino) carries the name, 0 on no match, divergent
 * duplicates or a stat failure. @ 00169f00
 */
int memfd_path_resolve_alt(const char *name, char *out)
{
    if (strncmp(name, "/proc/self/fd/", 0xe) == 0 && name[0xe] != '\0') {
        for (const unsigned char *p = (const unsigned char *)name + 0xe;
             *p != '\0'; p++) {
            if (*p >= 0x3a) /* digit gate: reject ':' and above */
                return 0;
        }
        if (strlen(name) < 0x40) {
            strcpy(out, name);
            return 1;
        }
        return 0;
    }

    if (strncmp(name, "/memfd:nexus-", 0xd) != 0 || strlen(name) >= 0x140)
        return 0;

    DIR *dirp = opendir("/proc/self/fd");
    if (dirp == NULL)
        return 0;

    int found = 0;     /* at least one fd matched by link target */
    int diverged = 0;  /* a second match on a different file, or stat failure */
    ino_t match_ino = 0;
    dev_t match_dev = 0;

    for (;;) {
        struct dirent *de = readdir(dirp);
        if (de == NULL)
            break;
        if (de->d_name[0] == '.' || strlen(de->d_name) >= 0x15)
            continue;

        char fd_path[0x40];
        char link[0x140];
        proc_fd_path_format(fd_path, de->d_name);

        ssize_t len = readlink(fd_path, link, 0x13f);
        if (len <= 0 || len >= 0x13f) /* empty, errored or truncated target */
            continue;
        link[len] = '\0';
        if (strcmp(link, name) != 0)
            continue;

        struct stat st;
        if (stat(fd_path, &st) != 0) {
            diverged = 1;
            break;
        }
        if (found) {
            if (st.st_dev == match_dev && st.st_ino == match_ino) {
                strcpy(out, fd_path); /* same file via another fd — refresh */
                continue;
            }
            diverged = 1; /* the name maps to more than one file */
            break;
        }
        strcpy(out, fd_path);
        found = 1;
        match_ino = st.st_ino;
        match_dev = st.st_dev;
    }

    closedir(dirp);
    return found && !diverged;
}

/*
 * evasion_module_verify — verify the libNexusEvasion69252.so memfd module.
 *
 * Forwards to memfd_module_verify with the evasion module name and no
 * expected-hash pin (NULL hash skips the 0x40-hex-char comparison).
 * @ 00169ef0
 */
void evasion_module_verify(const char *memfd_name, const char *family)
{
    memfd_module_verify((void *)memfd_name, family, "libNexusEvasion69252.so", NULL);
}

/* ===== text formatting trampolines ===== */

/*
 * plus_field_text_fill — fill one plus activation field buffer (0x80 cap).
 *
 * Compiled as a variadic __vsnprintf_chk trampoline: the write cap is the
 * hardcoded 0x80 and the caller's cap is the fortify object size (0x80 for
 * the stack field buffers, SIZE_MAX for in-place widget text). The format
 * string and its values travel in the variadic registers Ghidra could not
 * recover (one widgets.c call site passes a double seconds value); callers
 * passing only (buf, cap) rely on the registers left by the layout path.
 * @ 0016ba38
 */
void plus_field_text_fill(char *buf, size_t cap, ...)
{
    va_list ap;
    const char *fmt;

    va_start(ap, cap);
    fmt = va_arg(ap, const char *);
    __vsnprintf_chk(buf, 0x80, 0, cap, fmt, ap);
    va_end(ap);
}

/* ===== glyph width scaling ===== */

/*
 * font_scale_for_char — scale a base font size by the charset class of c.
 *
 * Looks c up in the rodata glyph-class sets and the two literal classes
 * ("Jjrft" and the wide caps "ABDGHKNOPQRUVXY") and multiplies base_size by
 * the class width: 2.6 / 3.2 / 4.8 / 5.0 / 6.2 / 8.2 / 10.8, default 7.2.
 * Returns the scaled size. @ 0016cca0
 */
float font_scale_for_char(float base_size, char c)
{
    float mult;

    if (strchr(g_font_glyph_class_32, c) != NULL)
        mult = 3.2f;
    else if (strchr(g_font_glyph_class_26, c) != NULL)
        mult = 2.6f;
    else if (strchr(g_font_glyph_class_50, c) != NULL)
        mult = 5.0f;
    else if (strchr("Jjrft", c) != NULL)
        mult = 4.8f;
    else if (strchr(g_font_glyph_class_62, c) != NULL)
        mult = 6.2f;
    else if (strchr(g_font_glyph_class_108, c) != NULL)
        mult = 10.8f;
    else if (strchr("ABDGHKNOPQRUVXY", c) != NULL)
        mult = 8.2f;
    else
        mult = 7.2f;
    return mult * base_size;
}

/* ===== engine base + forwarder family (all dispatch at g_engine_base + off) ===== */

/*
 * engine_base_set — store the engine (libg) image base pointer.
 * The whole forwarder family below dispatches relative to this value.
 * @ 0016cde4
 */
void engine_base_set(uint64_t base)
{
    g_engine_base = base;
}

/*
 * engine_call_11a2840 — engine allocator veneer: forwards the size and
 * returns the engine allocation (engine base + 0x11a2840). Callers use it
 * as alloc(size) (0x80 / 0x260 byte requests in renderer.c).
 * @ 0016cdf0
 */
uint64_t engine_call_11a2840(uint64_t size)
{
    return ((uint64_t (*)(uint64_t))(g_engine_base + 0x11a2840))(size);
}

/*
 * engine_call_593f14 — engine render/draw pass hook: forwards (arg, 0) to
 * engine base + 0x593f14. @ 0016ce08
 */
void engine_call_593f14(uint64_t arg)
{
    ((void (*)(uint64_t, int))(g_engine_base + 0x593f14))(arg, 0);
}

/*
 * engine_call_594e80 — veneer forwarding (a, b) to engine base + 0x594e80
 * (register args flow through the raw thunk).
 * @ 0016ce20
 */
void engine_call_594e80(uint64_t a, uint64_t b)
{
    ((void (*)(uint64_t, uint64_t))(g_engine_base + 0x594e80))(a, b);
}

/*
 * engine_call_887c14 — forward a no-arg call to engine base + 0x887c14.
 * @ 0016ce34
 */
void engine_call_887c14(void)
{
    ((void (*)(void))(g_engine_base + 0x887c14))();
}

/*
 * engine_sc_bundle_load — load the "sc/ui.sc" bundle through the engine
 * (engine base + 0x51ecec, flag 1) and return the loaded object.
 * @ 0016ce48
 */
uint64_t engine_sc_bundle_load(uint64_t arg)
{
    return ((uint64_t (*)(const char *, uint64_t, int))(
        g_engine_base + 0x51ecec))("sc/ui.sc", arg, 1);
}

/*
 * engine_call_772a30 — forward (a, b, 1) to engine base + 0x772a30.
 * @ 0016ce6c
 */
void engine_call_772a30(uint64_t a, uint64_t b)
{
    ((void (*)(uint64_t, uint64_t, int))(g_engine_base + 0x772a30))(a, b, 1);
}

/*
 * engine_call_5d7c30 — veneer forwarding (a, b) to engine base + 0x5d7c30
 * (register args flow through the raw thunk).
 * @ 0016ce84
 */
void engine_call_5d7c30(uint64_t a, uint64_t b)
{
    ((void (*)(uint64_t, uint64_t))(g_engine_base + 0x5d7c30))(a, b);
}

/*
 * engine_call_5d8ad4 — veneer forwarding (a, b) to engine base + 0x5d8ad4
 * and returning the engine result (callers use it as a style lookup).
 * @ 0016ce98
 */
uint64_t engine_call_5d8ad4(uint64_t a, uint64_t b)
{
    return ((uint64_t (*)(uint64_t, uint64_t))(g_engine_base + 0x5d8ad4))(
        a, b);
}

/*
 * engine_call_5921a0 — veneer forwarding (a, b) to engine base + 0x5921a0
 * (register args flow through the raw thunk; used to set widget text).
 * @ 0016ceac
 */
void engine_call_5921a0(uint64_t a, uint64_t b)
{
    ((void (*)(uint64_t, uint64_t))(g_engine_base + 0x5921a0))(a, b);
}

/*
 * widget_set_text — set a widget's text object (engine base + 0x88836c,
 * flag 1). @ 0016cec0
 */
void widget_set_text(uint64_t widget, uint64_t text_obj)
{
    ((void (*)(uint64_t, uint64_t, int))(g_engine_base + 0x88836c))(
        widget, text_obj, 1);
}

/*
 * engine_call_66ae58 — build an engine string object: forwards
 * (out, text) plus flag 1 to engine base + 0x66ae58.
 * @ 0016ced8
 */
void engine_call_66ae58(uint64_t out, const char *text)
{
    ((void (*)(uint64_t, const char *, int))(g_engine_base + 0x66ae58))(
        out, text, 1);
}

/*
 * engine_call_592250 — veneer forwarding (a, b) to engine base + 0x592250
 * (register args flow through the raw thunk; used to invalidate widgets).
 * @ 0016ceec
 */
void engine_call_592250(uint64_t a, uint64_t b)
{
    ((void (*)(uint64_t, uint64_t))(g_engine_base + 0x592250))(a, b);
}

/*
 * widget_set_scale — set a widget's scale: engine base + 0x59533c takes the
 * widget, + 0x595344 takes the (sx, sy) pair. @ 0016cf00
 */
void widget_set_scale(uint64_t widget, float sx, float sy)
{
    ((void (*)(uint64_t))(g_engine_base + 0x59533c))(widget);
    ((void (*)(float, float))(g_engine_base + 0x595344))(sx, sy);
}
/* misc_b chain chunk 2: covers raw lines 10522-11000 */

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

/* ---- shared host/service globals (a-chain, digest-mandated names) ---- */
extern void *g_host_ctx;                                      /* 0x1a7cc8 */
extern int (*g_host_read)(void *ctx, uint64_t addr, void *out,
                          uint32_t len);                      /* 0x1a7ce8 */
extern uint64_t g_game_base;                                  /* 0x1a7cd0 */
extern uint64_t g_ui_scene_root;                              /* 0x1a7d28 */
extern uint64_t g_ui_context;                                 /* 0x1a7d30 */
extern uint64_t g_engine_base;                                /* 0x281a48 */

/* ---- rodata ---- */
extern const int16_t MOVIE_FRAME_COUNTS[]; /* 0x13ce3c — expected movie BE frame counts, indexed by movie id */

/*
 * is_plausible_ptr_mb — digest-mandated pointer sanity gate for the misc_b
 * chain: rejects zero-page values (v < 0x1008, the raw `v >> 3 < 0x201`
 * address pre-check on read windows). Value-plausibility additionally
 * requires (v & 7) == 0 at the call sites, matching the raw
 * `0xfff < v && !(v & 7)` readback checks.
 */
int is_plausible_ptr_mb(uint64_t v)
{
    return (v >> 3) >= 0x201;
}

/* ===== engine forwarders (widget setters, dispatch at g_engine_base + off) ===== */

/*
 * widget_set_position — set a widget's position (engine base + 0x595314,
 * called with the widget and the x/y pair).
 * @ 0016cf5c
 */
void widget_set_position(uint64_t widget, float x, float y)
{
    ((void (*)(uint64_t, float, float))(g_engine_base + 0x595314))(widget, x, y);
}

/*
 * widget_set_font_size — set a widget's font size (engine base + 0x5958e8).
 * @ 0016cf70
 */
void widget_set_font_size(uint64_t widget, float size)
{
    ((void (*)(uint64_t, float))(g_engine_base + 0x5958e8))(widget, size);
}

/*
 * widget_set_anchor — apply/query a widget's anchor pair: engine
 * base + 0x595378 receives (widget, 0, anchor_pair, 0); callers read the
 * current anchor floats back out of the pair.
 * @ 0016cf84
 */
void widget_set_anchor(uint64_t widget, void *anchor_pair)
{
    ((void (*)(uint64_t, uint64_t, void *, uint64_t))(g_engine_base + 0x595378))(
        widget, 0, anchor_pair, 0);
}

/*
 * engine_call_594130 — forward (a, b) to engine base + 0x594130.
 * @ 0016cfa4
 */
void engine_call_594130(uint64_t a, uint64_t b)
{
    ((void (*)(uint64_t, uint64_t))(g_engine_base + 0x594130))(a, b);
}

/*
 * engine_call_5988ec — forward (a, b) to engine base + 0x5988ec.
 * @ 0016cfb8
 */
void engine_call_5988ec(uint64_t a, uint64_t b)
{
    ((void (*)(uint64_t, uint64_t))(g_engine_base + 0x5988ec))(a, b);
}

/*
 * widget_clip_apply — apply a clip: engine base + 0xd46898 with (a, b, 0, 0)
 * followed by engine base + 0xd464f8 with (a).
 * @ 0016cfcc
 */
void widget_clip_apply(uint64_t a, uint64_t b)
{
    ((void (*)(uint64_t, uint64_t, int, int))(g_engine_base + 0xd46898))(a, b, 0, 0);
    ((void (*)(uint64_t))(g_engine_base + 0xd464f8))(a);
}

/* ===== widget-tree link/scene validation (host-read gated) ===== */

/*
 * widget_child_link_valid — verify node is linked into parent:
 * node+0x38 == parent, node+0x40 is a valid child index, and
 * parent->children[index] (array at parent+0x50, count at +0x4e) == node.
 * Returns 1 when the link checks out, 0 otherwise. @ 0016d5cc
 */
int widget_child_link_valid(uint64_t node, uint64_t parent)
{
    if (node == 0 || parent == 0)
        return 0;

    uint64_t parent_link = 0;
    if (!is_plausible_ptr_mb(node + 0x40)
        || g_host_read(g_host_ctx, node + 0x38, &parent_link, 8) != 1)
        return 0;
    if (!(is_plausible_ptr_mb(parent_link) && (parent_link & 7) == 0))
        parent_link = 0;
    if (parent_link != parent)
        return 0;

    int32_t index = -1;
    if (!is_plausible_ptr_mb(node + 0x44)
        || g_host_read(g_host_ctx, node + 0x40, &index, 4) != 1
        || index < 0)
        return 0;

    uint16_t count = 0;
    if (!is_plausible_ptr_mb(parent + 0x50)
        || g_host_read(g_host_ctx, parent + 0x4e, &count, 2) != 1)
        return 0;

    uint64_t children = 0;
    if (!(index < (int32_t)count && is_plausible_ptr_mb(parent + 0x58)))
        return 0;
    if (g_host_read(g_host_ctx, parent + 0x50, &children, 8) != 1)
        return 0;
    if (!(is_plausible_ptr_mb(children) && (children & 7) == 0))
        return 0;

    uint64_t slot_addr = children + (uint64_t)index * 8;
    uint64_t slot = 0;
    if (!is_plausible_ptr_mb(slot_addr + 8)
        || g_host_read(g_host_ctx, slot_addr, &slot, 8) != 1)
        return 0;
    if (!(is_plausible_ptr_mb(slot) && (slot & 7) == 0))
        slot = 0;
    return slot == node;
}

/*
 * widget_ancestor_of — walk up to 16 parent links (+0x38) from node toward
 * target, validating each link with widget_child_link_valid. Returns 1 when
 * target is reached (or the walk falls off an implausible parent exactly at
 * target 0), 0 otherwise. @ 0016d800
 */
int widget_ancestor_of(uint64_t node, uint64_t target)
{
    uint64_t cur = node;
    int ok = 1;

    if (cur == 0)
        return target == 0;

    for (uint32_t depth = 0; ; depth++) {
        ok = 1;
        if (cur == target || depth > 0xf)
            break;

        uint64_t parent_raw = 0;
        int rd = is_plausible_ptr_mb(cur + 0x40)
                 && g_host_read(g_host_ctx, cur + 0x38, &parent_raw, 8) == 1;
        int parent_ok = rd && is_plausible_ptr_mb(parent_raw)
                        && (parent_raw & 7) == 0;
        uint64_t parent = parent_ok ? parent_raw : 0;

        if (!widget_child_link_valid(cur, parent))
            return 0;

        cur = parent_raw;
        if (!parent_ok) {
            cur = 0; /* defensive: raw clears the walker on a broken link */
            break;
        }
    }
    return cur == target ? ok : 0;
}

/*
 * ui_scene_fonts_ready — gate for font work: game+0x12eb9f0 must hold the
 * scene root, scene_root+0x90 must hold the UI context, the fonts flag byte
 * at scene_root+0x19c must be 0, and the context scale matrix at
 * context+0x10 must be the identity (1,0,0,1 with 0 < m <= 128).
 * Returns 1 when all gates pass. @ 0016d910
 */
int ui_scene_fonts_ready(void)
{
    uint64_t node = 0;
    if (!is_plausible_ptr_mb(g_game_base + 0x12eb9f8)
        || g_host_read(g_host_ctx, g_game_base + 0x12eb9f0, &node, 8) != 1)
        return 0;
    if (!(is_plausible_ptr_mb(node) && (node & 7) == 0))
        node = 0;
    if (node != g_ui_scene_root)
        return 0;

    uint64_t ctx = 0;
    if (!is_plausible_ptr_mb(g_ui_scene_root + 0x98)
        || g_host_read(g_host_ctx, g_ui_scene_root + 0x90, &ctx, 8) != 1)
        return 0;
    if (!(is_plausible_ptr_mb(ctx) && (ctx & 7) == 0))
        ctx = 0;
    if (ctx != g_ui_context)
        return 0;

    uint8_t fonts_flag = 1;
    if (!is_plausible_ptr_mb(g_ui_scene_root + 0x19d)
        || g_host_read(g_host_ctx, g_ui_scene_root + 0x19c, &fonts_flag, 1) != 1
        || fonts_flag != 0)
        return 0;

    float m[4]; /* scale matrix {m00, m01, m10, m11} at context+0x10 */
    if (is_plausible_ptr_mb(g_ui_context + 0x20)
        && g_host_read(g_host_ctx, g_ui_context + 0x10, m, 0x10) == 1
        && isfinite(m[0]) && isfinite(m[1]) && isfinite(m[2]) && isfinite(m[3])
        && m[1] == 0.0f && m[2] == 0.0f
        && m[0] > 0.0f && m[3] > 0.0f
        && m[0] <= 128.0f && m[3] <= 128.0f
        && m[0] == 1.0f)
        return m[3] == 1.0f;
    return 0;
}

/*
 * ui_fonts_preflight_gate — the scene-root fonts flag byte (+0x19c) reads
 * back clear. Returns 1 when the gate passes. @ 0016db8c
 */
int ui_fonts_preflight_gate(void)
{
    uint8_t flag = 1;
    if (is_plausible_ptr_mb(g_ui_scene_root + 0x19d)
        && g_host_read(g_host_ctx, g_ui_scene_root + 0x19c, &flag, 1) == 1)
        return flag == 0;
    return 0;
}

/*
 * movie_frame_count_check — validate a movie clip: vtable at +0 must be
 * game+0x11ad208, owner at +0x30 must be NULL or the scene root, and the
 * BE frame count at +0xbe must equal MOVIE_FRAME_COUNTS[index] and exceed
 * min_frames. Returns 1 when the clip matches. @ 0016dc20
 */
int movie_frame_count_check(uint64_t movie, uint64_t index, int min_frames)
{
    if (movie == 0)
        return 0;

    uint64_t vtable = 0;
    if (!is_plausible_ptr_mb(movie + 8)
        || g_host_read(g_host_ctx, movie, &vtable, 8) != 1)
        return 0;
    if (!(is_plausible_ptr_mb(vtable) && (vtable & 7) == 0))
        vtable = 0;
    if (vtable != g_game_base + 0x11ad208)
        return 0;

    uint64_t owner = 0;
    if (is_plausible_ptr_mb(movie + 0x38)
        && g_host_read(g_host_ctx, movie + 0x30, &owner, 8) == 1
        && (owner == 0 || owner == g_ui_scene_root)) {
        uint16_t frames = 0;
        if (is_plausible_ptr_mb(movie + 0xc0)
            && g_host_read(g_host_ctx, movie + 0xbe, &frames, 2) == 1
            && min_frames >= 0
            && (int16_t)frames == MOVIE_FRAME_COUNTS[index])
            return min_frames < (int16_t)frames;
    }
    return 0;
}

/*
 * widget_detached_check — node is fully detached: parent pointer (+0x38)
 * reads NULL and the child-slot index (+0x40) reads -1. Returns 1 when
 * detached. @ 0016ddc8
 */
int widget_detached_check(uint64_t node)
{
    uint64_t parent = 1;
    int32_t index = 0;

    if (is_plausible_ptr_mb(node + 0x40)
        && g_host_read(g_host_ctx, node + 0x38, &parent, 8) == 1
        && parent == 0
        && is_plausible_ptr_mb(node + 0x44)
        && g_host_read(g_host_ctx, node + 0x40, &index, 4) == 1)
        return index == -1;
    return 0;
}

/*
 * remote_vtable_matches — the qword at obj holds a plausible pointer equal
 * to game_base + rva. Returns 1 on a match. @ 0016debc
 */
int remote_vtable_matches(uint64_t obj, uint64_t rva)
{
    uint64_t vtable = 0;
    if (is_plausible_ptr_mb(obj + 8)
        && g_host_read(g_host_ctx, obj, &vtable, 8) == 1
        && is_plausible_ptr_mb(vtable) && (vtable & 7) == 0)
        return vtable == g_game_base + rva;
    return 0;
}

/*
 * widget_scene_owner_check — the scene-owner pointer at widget+0x30 is NULL
 * or the UI scene root. Returns 1 when owned (or unowned). @ 0016df78
 */
int widget_scene_owner_check(uint64_t widget)
{
    uint64_t owner = 0;
    if (is_plausible_ptr_mb(widget + 0x38)
        && g_host_read(g_host_ctx, widget + 0x30, &owner, 8) == 1)
        return owner == 0 || owner == g_ui_scene_root;
    return 0;
}
/* misc_b chain chunk 3: covers raw lines 11001-11500 */

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

/* ---- shared host/service globals (a-chain, digest-mandated names) ---- */
extern void *g_host_ctx;                                      /* 0x1a7cc8 */
extern int (*g_host_read)(void *ctx, uint64_t addr, void *out,
                          uint32_t len);                      /* 0x1a7ce8 */
extern uint64_t g_game_base;                                  /* 0x1a7cd0 */
extern uint64_t g_ui_scene_root;                              /* 0x1a7d28 */
extern uint64_t g_ui_context;                                 /* 0x1a7d30 */
extern float g_ui_density;                                    /* 0x1e53fc — design-density divisor */
extern float g_ui_center_x;                                   /* 0x1e5400 */
extern float g_ui_center_y;                                   /* 0x1e5404 */
extern float g_ui_transform_scale;                            /* 0x1a7d5c — UI transform scale (engine-mirrored) */
extern float g_ui_transform_tx;                               /* 0x1a7d60 */
extern float g_ui_transform_ty;                               /* 0x1a7d64 */
extern int32_t g_menu_entry_index;                            /* 0x1a7d18 — folds > 0x2a via -0x2b (a-chain) */
extern float g_scroll_position;                               /* 0x22d518 — scroll position (a-chain) */
extern uint64_t g_pointer_state_pair;                         /* 0x22d51c — packed pair, high half used (a-chain) */
extern uint32_t g_pointer_latch;                              /* 0x22d530 — pointer-event latch (a-chain) */
extern uint64_t g_edit_controls_panel;                        /* 0x282168 — edit-controls ctx widget (widgets.c block) */
extern uint8_t g_diag_label_state[0x820];                     /* 0x282b10 — 0x820 label render state (renderer.c) */
extern float g_diag_scroll_base;                              /* 0x28332c — diag scroll base (renderer.c) */

/* ---- a-chain functions ---- */
extern int widget_text_island_guard(uint64_t base, uint64_t rva,
                                   uint32_t *buf, uint64_t len); /* misc.c (a-chain) @ 0014ff7c */

/* ---- own later chunks (b-chain) ---- */
extern int widget_edit_controls_bound_check(uint64_t widget); /* misc.c (b-chain) @ 00179dfc */
extern int hud_snapshot_read(uint8_t *out, size_t size);          /* misc.c (c-chain) @ 001922ac */
extern int ui_clips_ready_check(void);                        /* misc.c (b-chain) @ 00179448 */
extern int host_read_gate(uint64_t ctx, uint64_t addr, void *out,
                          uint64_t len);                      /* misc.c (b-chain) @ 0017fe9c */
extern int host_write_gate(uint64_t ctx, void *dst, const void *src,
                           size_t len);                       /* misc.c (b-chain) @ 0017fee0 */
extern int clip_ops_integrity_check(uint64_t ctx);            /* misc.c (b-chain) @ 001802bc */
extern void engine_call_11a2840_adapter(uint64_t ctx,
                                        uint64_t a);          /* misc.c (b-chain) @ 00180540 */
extern void engine_call_88b344_adapter(uint64_t ctx, uint64_t a,
                                       uint32_t b);           /* misc.c (b-chain) @ 00180548 */
extern void engine_call_593f14_adapter(uint64_t ctx, uint64_t a,
                                       uint32_t b);           /* misc.c (b-chain) @ 00180564 */
extern void engine_call_594130_adapter(uint64_t ctx, uint64_t a,
                                       uint64_t b);           /* misc.c (b-chain) @ 00180580 */
extern void engine_call_59567c_adapter(uint64_t ctx,
                                       uint64_t a);           /* misc.c (b-chain) @ 0018058c */
extern void engine_call_5940d0_gated(uint64_t ctx,
                                     uint64_t a);             /* misc.c (b-chain) @ 001805a4 */
extern void engine_call_59531c_adapter(int x, int y, uint64_t ctx,
                                       uint64_t clip);         /* misc.c (b-chain) @ 00180724 */
extern void engine_call_88d290_adapter(uint64_t ctx, uint64_t a,
                                       uint32_t b);           /* misc.c (b-chain) @ 0018073c */
extern void engine_call_594e80_adapter(uint64_t ctx, uint64_t a,
                                       uint32_t b);           /* misc.c (b-chain) @ 00180758 */
extern int widget_clip_move(float x, float y, void *ops, uint64_t root,
                            uint32_t entry_index,
                            uint32_t flag);                   /* misc.c (b-chain) @ 0017fc3c */

/* ---- other files ---- */
extern int menu_clip_writable_check(uint64_t ctx, void *addr,
                                    size_t len);   /* menu_engine.c @ 00180034 */
extern void menu_probe_log(uint64_t ctx,
                           const char *event);     /* menu_engine.c @ 0017fbc8 */
extern void rich_label_state_render(float w, float h, float font,
                                    void *state,
                                    const char *text); /* renderer.c @ 00179f68 */

/* ---- rodata ---- */
extern const struct {
    uint64_t rva;            /* region offset in the engine image */
    uint64_t len;            /* region length in bytes */
    const uint8_t *expected; /* saved bytes to compare against */
} ENGINE_SNAPSHOT_REGIONS[0x39]; /* 0x19c100 — 57 engine code regions, stride 0x18 */
extern const uint64_t ENGINE_LINK_TABLE[]; /* 0x13cea0 — (expected, next) rva pairs for the 0x24-link engine object graph */

/*
 * clip_ops_t — the ops channel bound to the game base and passed to
 * widget_clip_move / the clip slot checks. Every function slot takes the
 * channel handle as its first argument and drops it (veneer convention).
 */
typedef struct {
    uint64_t game_base;   /* +0x00 */
    uint64_t ctx;         /* +0x08 — channel handle (0 when unbound) */
    int  (*read_gate)(uint64_t ctx, uint64_t addr, void *out, uint64_t len);      /* +0x10 */
    int  (*write_gate)(uint64_t ctx, void *dst, const void *src, size_t len);     /* +0x18 */
    int  (*writable_check)(uint64_t ctx, void *addr, size_t len);                 /* +0x20 */
    int  (*integrity_check)(uint64_t ctx);                                        /* +0x28 */
    void (*call_11a2840)(uint64_t ctx, uint64_t a);                               /* +0x30 */
    void (*call_88b344)(uint64_t ctx, uint64_t a, uint32_t b);                    /* +0x38 */
    void (*call_593f14)(uint64_t ctx, uint64_t a, uint32_t b);                    /* +0x40 */
    void (*call_594130)(uint64_t ctx, uint64_t a, uint64_t b);                    /* +0x48 */
    void (*call_59567c)(uint64_t ctx, uint64_t a);                                /* +0x50 */
    void (*call_5940d0)(uint64_t ctx, uint64_t a);                                /* +0x58 */
    void (*call_59531c)(int x, int y, uint64_t ctx, uint64_t clip);               /* +0x60 */
    void (*call_88d290)(uint64_t ctx, uint64_t a, uint32_t b);                    /* +0x68 */
    void (*call_594e80)(uint64_t ctx, uint64_t a, uint32_t b);                    /* +0x70 */
    void (*probe_log)(uint64_t ctx, const char *event);                           /* +0x78 */
} clip_ops_t;

/* ===== misc_b-owned frame/diag state globals ===== */
uint32_t g_frame_state_armed;     /* 0x22d794 — frame state armed flag (menu anchor gate) */
uint32_t g_screen_active;         /* 0x22d798 — current screen hits bitmask 0x22006000cd001 */
uint32_t g_screen_sub_id;         /* 0x22d79c — screen sub-id byte, 0xffffffff when none */
uint8_t  g_screen_id;             /* 0x22d7a0 — current screen id byte from the frame event */
uint32_t g_frame_gen_a;           /* 0x22d7a8 — frame generation counter A (bumped on change) */
uint32_t g_frame_gen_b;           /* 0x22d7b0 — frame generation counter B (bumped on change) */
uint32_t g_clip_move_count;       /* 0x22d7c8 — successful clip moves */
uint32_t g_screen_change_count;   /* 0x22d7d0 — screen sub-id change counter */
float    g_clip_y_cached;         /* 0x22d7e8 — last applied clip y fraction */
uint64_t g_frame_cache_7f0;   /* 0x22d7f0 — viewport cache {center_x, density} (packed) */
uint64_t g_frame_cache_7f8;   /* 0x22d7f8 — viewport cache {transform_scale, center_y} (packed) */
uint64_t g_frame_cache_800;   /* 0x22d800 — viewport cache {transform_ty, transform_tx} (packed) */
uint64_t g_frame_scene_ctx;   /* 0x22d808 — cached scene context (scene root +0x284) */
uint32_t g_frame_cache_valid; /* 0x22d810 — viewport cache populated */
float    g_diag_overlay_scale;    /* 0x22d818 — diag overlay scale, min(h/720, 1.5) */
uint64_t g_pointer_state_528;     /* 0x22d528 — pointer state qword (reset on screen change) */
uint64_t g_pointer_state_538;     /* 0x22d538 — pointer state qword (reset on screen change) */
uint32_t g_pointer_state_540;     /* 0x22d540 — pointer state word (reset on screen change) */
uint64_t g_pointer_state_548;     /* 0x22d548 — pointer state qword (reset on screen change) */
uint32_t g_pointer_state_550;     /* 0x22d550 — pointer state word (reset on screen change) */
uint32_t g_pointer_state_558;     /* 0x22d558 — pointer state word (reset on screen change) */
float    g_diag_scroll_y;         /* 0x283330 — diag overlay scroll y accumulator */

/* ===== widget chain uniqueness (movie-class trees) ===== */

/*
 * widget_chain_links_unique_check — verify the parent-to-node chain in a
 * movie-class widget tree (vtable game+0x11ad208): collect up to 12 ancestor
 * links from parent up to node, then walk back down requiring every level to
 * be a movie object whose children array holds the next chain entry exactly
 * once. Finally every non-chain child of the chain nodes is parked offscreen
 * at (-10000, -10000). Returns 1 when the chain is unique, 0 otherwise.
 * @ 0016e020
 */
int widget_chain_links_unique_check(uint64_t node, uint64_t parent)
{
    uint64_t chain[12];

    if (parent == 0)
        return 0;

    /* phase 1: collect the ancestor chain, chain[k] = node */
    uint32_t k = 0;
    uint64_t cur = parent;
    for (;;) {
        chain[k] = cur;
        if (cur == node)
            break;
        uint64_t up = 0;
        int rd = is_plausible_ptr_mb(cur + 0x40)
                 && g_host_read(g_host_ctx, cur + 0x38, &up, 8) == 1;
        if (!(rd && is_plausible_ptr_mb(up) && (up & 7) == 0))
            break; /* broken link: chain[k] != node, rejected below */
        cur = up;
        if (k > 10)
            break; /* raw caps the chain at 12 stored entries */
        k++;
    }
    if (k < 1 || chain[k] != node)
        return 0;

    /* phase 2: walk down, each level a movie object containing the next
       chain entry exactly once in its children array */
    uint64_t anc = node;
    for (int32_t i = (int32_t)k - 1; ; i--) {
        uint64_t child = chain[i];

        uint64_t vtable = 0;
        int rd = is_plausible_ptr_mb(anc + 8)
                 && g_host_read(g_host_ctx, anc, &vtable, 8) == 1;
        if (!(rd && is_plausible_ptr_mb(vtable) && (vtable & 7) == 0))
            return 0;
        if (vtable != g_game_base + 0x11ad208)
            return 0;
        if (!widget_child_link_valid(child, anc))
            return 0;

        uint16_t count = 0;
        if (!is_plausible_ptr_mb(anc + 0xc2)
            || g_host_read(g_host_ctx, anc + 0xc0, &count, 2) != 1)
            return 0;
        if (count < 1 || 0x200 < count || anc < 0xf70)
            return 0;

        uint64_t children = 0;
        rd = is_plausible_ptr_mb(anc + 0x98)
             && g_host_read(g_host_ctx, anc + 0x90, &children, 8) == 1;
        if (!(rd && is_plausible_ptr_mb(children) && (children & 7) == 0))
            return 0;

        int hits = 0;
        uint64_t slot_ptr = children;
        for (uint64_t c = 0; c < count; c++) {
            uint64_t slot = 0;
            int rd2 = is_plausible_ptr_mb(slot_ptr + 8)
                      && g_host_read(g_host_ctx, slot_ptr, &slot, 8) == 1;
            if (rd2 && is_plausible_ptr_mb(slot) && (slot & 7) == 0
                && slot == child)
                hits++;
            slot_ptr += 8;
        }
        if (hits != 1)
            return 0;

        anc = child;
        if (i == 0)
            break;
    }

    /* phase 3: park every non-chain child offscreen so the chain stays unique */
    for (uint32_t j = k; j != 0; j--) {
        uint64_t cj = chain[j];

        uint16_t count = 0;
        if (is_plausible_ptr_mb(cj + 0xc2))
            g_host_read(g_host_ctx, cj + 0xc0, &count, 2); /* result ignored (raw) */

        uint64_t children = 0;
        int rd = is_plausible_ptr_mb(cj + 0x98)
                 && g_host_read(g_host_ctx, cj + 0x90, &children, 8) == 1;
        if (!(rd && is_plausible_ptr_mb(children) && (children & 7) == 0))
            children = 0;

        if (0 < count) {
            uint64_t slot_ptr = children;
            for (uint64_t c = 0; c < count; c++) {
                if (is_plausible_ptr_mb(slot_ptr + 8)) {
                    uint64_t slot = 0;
                    if (g_host_read(g_host_ctx, slot_ptr, &slot, 8) == 1
                        && is_plausible_ptr_mb(slot) && (slot & 7) == 0
                        && slot != chain[j - 1]
                        && widget_child_link_valid(slot, cj))
                        widget_set_position(slot, -10000.0f, -10000.0f); /* 0xc61c3c00 */
                }
                slot_ptr += 8;
            }
        }
    }
    return 1;
}

/*
 * widget_flag48_equals — the flag byte at widget+0x48 reads back equal to
 * expected. Returns 1 on a match. @ 0016e464
 */
int widget_flag48_equals(uint64_t widget, int expected)
{
    int8_t flag = -1;
    if (is_plausible_ptr_mb(widget + 0x49)
        && g_host_read(g_host_ctx, widget + 0x48, &flag, 1) == 1)
        return flag == (int8_t)expected;
    return 0;
}

/*
 * ui_edit_controls_ctx_valid — the edit-controls ctx widget (0x282168) has
 * vtable game+0x11c0c58, is bound (widget_edit_controls_bound_check), its
 * +0xec pair reads (1, 0x91), and the object at +0x128 is bound too.
 * Returns 1 when the edit-controls context is usable. @ 0016e7c0
 */
int ui_edit_controls_ctx_valid(void)
{
    uint64_t vtable = 0;
    if (!is_plausible_ptr_mb(g_edit_controls_panel + 8)
        || g_host_read(g_host_ctx, g_edit_controls_panel, &vtable, 8) != 1)
        return 0;
    if (!(is_plausible_ptr_mb(vtable) && (vtable & 7) == 0))
        vtable = 0;
    if (vtable != g_game_base + 0x11c0c58)
        return 0;
    if (widget_edit_controls_bound_check(g_edit_controls_panel) == 0)
        return 0;

    uint32_t pair[2] = {0, 0};
    if (is_plausible_ptr_mb(g_edit_controls_panel + 0xf4)
        && g_host_read(g_host_ctx, g_edit_controls_panel + 0xec, pair, 8) == 1
        && pair[0] == 1 && pair[1] == 0x91) {
        uint64_t inner = 0;
        if (is_plausible_ptr_mb(g_edit_controls_panel + 0x130)
            && g_host_read(g_host_ctx, g_edit_controls_panel + 0x128, &inner, 8) == 1
            && is_plausible_ptr_mb(inner) && (inner & 7) == 0)
            return widget_edit_controls_bound_check(inner) != 0;
    }
    return 0;
}

/*
 * widget_field_ptr_read — read the qword at addr through the host channel
 * and validate it as a pointer. Returns the value, or 0 when the read
 * fails or the value is implausible. @ 0016f79c
 */
uint64_t widget_field_ptr_read(uint64_t addr)
{
    uint64_t value = 0;
    if (is_plausible_ptr_mb(addr + 8)
        && g_host_read(g_host_ctx, addr, &value, 8) == 1
        && is_plausible_ptr_mb(value) && (value & 7) == 0)
        return value;
    return 0;
}

/* ===== diag overlay + engine code snapshot ===== */

/*
 * ui_diag_overlay_render — render the 0x200-byte diagnostic text overlay:
 * read the HUD snapshot, compute the overlay scale (1.0, or
 * min(scene_h/720, 1.5) when the scene is taller than 720) and hand the
 * 0x820 label state to the renderer. Also maintains the diag scroll
 * accumulator (0x283330), zeroed unless the scroll base is positive and
 * the HUD status is 2. @ 0016fb10
 */
void ui_diag_overlay_render(void)
{
    char text[0x200];
    int status = hud_snapshot_read((uint8_t *)text, 0x200);

    g_diag_overlay_scale = 1.0f;
    float scene[4]; /* floats at scene root +0x3c; [3] = height at +0x48 */
    if (is_plausible_ptr_mb(g_ui_scene_root + 0x4c)
        && g_host_read(g_host_ctx, g_ui_scene_root + 0x3c, scene, 0x10) == 1
        && isfinite(scene[3]) && scene[3] > 720.0f)
        g_diag_overlay_scale = fminf(scene[3] / 720.0f, 1.5f /* 0x3fc00000 */);

    float h8 = g_diag_overlay_scale * 8.0f;
    rich_label_state_render(g_diag_overlay_scale * 20.0f, h8,
                            g_diag_overlay_scale * 16.0f,
                            g_diag_label_state, status != 0 ? text : NULL);

    g_diag_scroll_y = h8 + g_diag_scroll_base;
    if (g_diag_scroll_base <= 0.0f || status != 2)
        g_diag_scroll_y = 0.0f;
}

/*
 * engine_code_snapshot_verify — memcmp the 57 saved engine code regions
 * (table 0x19c100, 0x20 bytes at a time through the host channel; the
 * chunk covering engine rva 0x88836c additionally passes the island-stub
 * guard), then verify the 0x24-link engine object graph from
 * game+0x11c0a78 against the rva link table. Returns 1 when the engine
 * image matches its snapshot. @ 00172da8
 */
int engine_code_snapshot_verify(void)
{
    for (uint32_t r = 0; r < 0x39; r++) {
        uint64_t rva = ENGINE_SNAPSHOT_REGIONS[r].rva;
        uint64_t len = ENGINE_SNAPSHOT_REGIONS[r].len;

        for (uint64_t off = 0; off < len; off += 0x20) {
            uint64_t chunk = len - off;
            if (chunk > 0x20)
                chunk = 0x20;
            uint64_t addr = g_game_base + rva + off;
            uint8_t buf[0x20];

            /* zero-page and no-wrap guard on the region window (raw form) */
            if (addr < 0x1000
                || (uint64_t)(~rva - off - g_game_base) < chunk)
                return 0;
            if (g_host_read(g_host_ctx, addr, buf, chunk) != 1)
                return 0;
            if (rva + off == 0x88836c
                && widget_text_island_guard(g_game_base, 0x88836c,
                                            (uint32_t *)buf,
                                            (uint64_t)chunk) == 0)
                return 0;
            if (memcmp(buf, ENGINE_SNAPSHOT_REGIONS[r].expected + off,
                       (size_t)chunk) != 0)
                return 0;
        }
    }

    uint64_t node_addr = g_game_base + 0x11c0a78;
    int ok = 0;
    if (is_plausible_ptr_mb(g_game_base + 0x11c0a80)) {
        uint32_t i = 0;
        for (;;) {
            uint64_t val = 0;
            if (g_host_read(g_host_ctx, node_addr, &val, 8) != 1
                || val != g_game_base + ENGINE_LINK_TABLE[i * 2])
                break;
            ok = i > 0x23;
            if (i == 0x24)
                break;
            node_addr = g_game_base + ENGINE_LINK_TABLE[i * 2 + 1];
            i++;
            if (!is_plausible_ptr_mb(node_addr + 8))
                break;
        }
    }
    return ok;
}

/* ===== per-frame state sync ===== */

/*
 * ui_frame_state_sync — per-frame cache sync from a frame event record
 * (screen id at +8, gate bytes at +9/+10, sub-id at +0xb): refresh the
 * viewport/transform cache (density, centers, transform, scene ctx at
 * 0x22d7f0..0x22d810) and bump the generation counters on any change;
 * reset the 0x22d5xx pointer/scroll block on a screen change; track the
 * active screen via bitmask 0x22006000cd001 over ids 0x43..0x74; and snap
 * the clip y to the scroll fraction via widget_clip_move(536, -132-frac).
 * Returns 1 when the clip snap succeeded or was already cached.
 * @ 001753b0
 */
int ui_frame_state_sync(const uint8_t *frame)
{
    union { float f; uint32_t b; } cvt;
    uint64_t scene_ctx = 0;
    int result = 0;

    if (!is_plausible_ptr_mb(g_ui_scene_root + 0x28c)
        || g_host_read(g_host_ctx, g_ui_scene_root + 0x284, &scene_ctx, 8) != 1)
        return 0;

    /* pack the engine viewport globals exactly as the cache stores them */
    cvt.f = g_ui_density;         uint32_t density_b = cvt.b;
    cvt.f = g_ui_center_x;        uint32_t center_x_b = cvt.b;
    cvt.f = g_ui_center_y;        uint32_t center_y_b = cvt.b;
    cvt.f = g_ui_transform_scale; uint32_t ts_b = cvt.b;
    cvt.f = g_ui_transform_tx;    uint32_t tx_b = cvt.b;
    cvt.f = g_ui_transform_ty;    uint32_t ty_b = cvt.b;
    uint64_t cache_7f0 = ((uint64_t)center_x_b << 32) | density_b;
    uint64_t cache_7f8 = ((uint64_t)ts_b << 32) | center_y_b;
    uint64_t cache_800 = ((uint64_t)ty_b << 32) | tx_b;

    if (g_frame_cache_valid == 0
        || g_frame_cache_7f0 != cache_7f0 || g_frame_cache_7f8 != cache_7f8
        || g_frame_cache_800 != cache_800
        || g_frame_scene_ctx != scene_ctx) {
        g_frame_cache_7f0 = cache_7f0;
        g_frame_cache_7f8 = cache_7f8;
        g_frame_cache_800 = cache_800;
        g_frame_state_armed = 0;
        g_frame_gen_a++;
        g_frame_gen_b++;
        g_frame_scene_ctx = scene_ctx;
        g_frame_cache_valid = 1;
    }

    if (g_screen_id != frame[8]) {
        g_pointer_state_558 = 0;
        g_frame_gen_a++;
        g_frame_gen_b++;
        g_scroll_position = 0.0f;
        g_pointer_state_pair &= 0xffffffffu; /* clear only the 0x22d520 half */
        g_pointer_latch = 0;
        g_pointer_state_528 = 0;
        g_pointer_state_540 = 0;
        g_pointer_state_538 = 0;
        g_pointer_state_550 = 0;
        g_pointer_state_548 = 0;
        g_screen_sub_id = 0xffffffffu;
        g_screen_id = frame[8];
    }

    uint32_t screen_bit = 0;
    if (frame[9] != 0 && frame[10] == 0) {
        uint32_t idx = (uint32_t)frame[8] - 0x43;
        if (idx <= 0x31)
            screen_bit = (uint32_t)(0x22006000cd001ULL >> (idx & 0x3f)) & 1;
    }
    if (g_screen_active != screen_bit) {
        g_frame_gen_a++;
        g_frame_gen_b++;
        g_screen_active = screen_bit;
    }
    if (screen_bit != 0 && g_screen_sub_id != frame[0xb]) {
        g_frame_gen_b++;
        g_screen_change_count++;
        g_screen_sub_id = frame[0xb];
    }

    if (ui_clips_ready_check() != 0) {
        float frac = -132.0f - g_scroll_position;
        if (screen_bit == 0)
            frac = -132.0f;
        frac = -132.0f - (float)(int)frac; /* fractional part, negated */

        if (frac == g_clip_y_cached) {
            result = 1;
        } else {
            clip_ops_t ops;
            memset(&ops, 0, sizeof ops);
            ops.game_base = g_game_base;
            ops.read_gate = host_read_gate;
            ops.write_gate = host_write_gate;
            ops.writable_check = menu_clip_writable_check;
            ops.integrity_check = clip_ops_integrity_check;
            ops.call_11a2840 = engine_call_11a2840_adapter;
            ops.call_88b344 = engine_call_88b344_adapter;
            ops.call_593f14 = engine_call_593f14_adapter;
            ops.call_594130 = engine_call_594130_adapter;
            ops.call_59567c = engine_call_59567c_adapter;
            ops.call_5940d0 = engine_call_5940d0_gated;
            ops.call_59531c = engine_call_59531c_adapter;
            ops.call_88d290 = engine_call_88d290_adapter;
            ops.call_594e80 = engine_call_594e80_adapter;
            ops.probe_log = menu_probe_log;

            uint32_t entry = 0;
            if (g_menu_entry_index > 0x2a)
                entry = (uint32_t)(g_menu_entry_index - 0x2b);

            if (widget_clip_move(536.0f /* 0x44048000 */, -132.0f - frac,
                                 &ops, g_ui_scene_root, entry, 1) == 1) {
                result = 1;
                g_clip_move_count++;
                g_clip_y_cached = frac;
            }
        }
    }
    return result;
}
/* misc_b chain chunk 4: covers raw lines 11501-12000 */

#define _GNU_SOURCE

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>

/* glibc fortified vsnprintf (only auto-declared under _FORTIFY_SOURCE) */
extern int __vsnprintf_chk(char *s, size_t maxlen, int flag, size_t slen,
                           const char *format, va_list ap);

/* ---- shared host/service globals (a-chain, digest-mandated names) ---- */
extern void *g_host_ctx;                                      /* 0x1a7cc8 */
extern int (*g_host_ready)(void *ctx);                        /* 0x1a7cf0 */
extern int (*g_host_read)(void *ctx, uint64_t addr, void *out,
                          uint32_t len);                      /* 0x1a7ce8 */
extern uint64_t g_game_base;                                  /* 0x1a7cd0 */
extern uint64_t g_ui_scene_root;                              /* 0x1a7d28 */
extern uint64_t g_ui_context;                                 /* 0x1a7d30 */
extern uint64_t g_view_object_a;                              /* 0x1a7d38 */
extern uint64_t g_view_object_b;                              /* 0x281a50 */
extern int32_t g_menu_entry_index;                            /* 0x1a7d18 */
extern int g_plus_service_installed;                          /* 0x1a7d0c — entitlement service gate */
extern uint64_t g_libg_span;                                  /* 0x10f740 — {base,size} of libg.so (packed) */
extern uint64_t g_popover_context;                            /* 0x281b10 — popover/chooser root (multi-part) */

/*
 * g_clip_cache — the cached clip-widget record at 0x22d4e0 (0x30 bytes).
 * clip_a/clip_c are multi-part (referenced by the a-chain too); the whole
 * record is defined in chunk 5 next to its writer (ui_clip_cache_release).
 */
typedef struct {
    uint64_t clip_a;   /* +0x00 0x22d4e0 — cached clip widget A */
    uint64_t clip_b;   /* +0x08 0x22d4e8 — cached clip widget B */
    uint64_t clip_c;   /* +0x10 0x22d4f0 — cached clip widget C */
    uint64_t flag;     /* +0x18 0x22d4f8 — two-phase release flag */
    uint64_t state;    /* +0x20 0x22d500 */
    float    frame_w;  /* +0x28 0x22d508 */
    float    frame_h;  /* +0x2c 0x22d50c */
} clip_cache_t;
extern clip_cache_t g_clip_cache;                             /* 0x22d4e0 */

/* ---- menu entry widget records (rodata) ---- */
extern const uint8_t g_menu_entry_table[]; /* 0x1c3568 — 42 records, 0xd8-byte stride; widget at +0 */

/* ---- rich panel block (renderer.c-owned slots) ---- */
extern uint64_t g_rich_panel_root;      /* 0x283338 — rich panel root widget */
extern uint64_t g_rich_panel_ctx;       /* 0x283340 — rich panel ctx (page-list owner, kind 3) */
extern uint64_t g_rich_panel_ctx_slot;  /* 0x283348 — kind-0 slot (no extra) */
extern uint64_t g_rich_panel_k2_slots[5]; /* 0x283350 — kind-2 widget slots */
extern uint64_t g_rich_panel_k4_slots[5]; /* 0x283378 — kind-4 flag slots */
extern uint64_t g_rich_panel_page_ctx;  /* 0x283980 — rich panel page-list ctx */

/* rich panel slot rows: 0x78-byte stride; kind-1 rows at 0x2833a0 (extra
   unused), kind-0 rows at 0x283678 (paired extra at +8); there is an
   8-byte gap after kind-1 row 5 */
typedef struct {
    uint64_t widget;   /* +0x00 — cached widget */
    uint64_t extra;    /* +0x08 — extra/parent widget (kind-0 rows) */
    uint64_t _pad[13]; /* +0x10..0x77 */
} rich_panel_slot_row_t;
extern rich_panel_slot_row_t g_rich_panel_rows_a[6]; /* 0x2833a0 — kind-1 rows */
extern rich_panel_slot_row_t g_rich_panel_rows_b[6]; /* 0x283678 — kind-0 rows */

/* ---- HUD label states (renderer.c-owned, 0x820 blocks) ---- */
extern uint8_t  g_diag_label_state[0x820];   /* 0x282b10 — first diag label state */
extern uint64_t g_diag_label_frame;          /* 0x282b18 — its frame arg */
extern uint64_t g_diag_label_ctx;            /* 0x282b20 — its ctx (== g_ui_context) */
extern uint8_t  g_diag_label_state_2[0x820]; /* 0x283a00 — second diag label state */
extern uint64_t g_diag_label_frame_2;        /* 0x283a08 — its frame arg */
extern uint64_t g_diag_label_ctx_2;          /* 0x283a10 — its ctx */

/* ---- panel pair / edit controls / theme view (registry-named, multi-part) ---- */
extern uint64_t g_panel_widget;         /* 0x284220 */
extern uint64_t g_page_list_widget;     /* 0x2842f8 */
extern uint64_t g_page_context_widget;  /* 0x2843d0 */
extern uint64_t g_edit_controls_widget; /* 0x2843e0 */
extern uint64_t g_edit_controls_flag;   /* 0x2844c8 */
extern uint64_t g_edit_controls_root;   /* 0x2844b8 — edit controls root ctx */
extern uint64_t g_theme_view;           /* 0x2844f0 — theme view widget */
extern uint64_t g_theme_view_flag;      /* 0x2844f8 — theme view flag */
extern uint64_t g_theme_view_ctx;       /* 0x2844e8 — theme view ctx */
extern uint64_t g_theme_view_extra;     /* 0x284500 — theme view extra */

/* ---- edit-controls block (0x282150 group, widgets.c-owned) ---- */
extern uint64_t g_edit_controls_block_root; /* 0x282150 */
extern uint64_t g_edit_controls_ctx;        /* 0x282158 */
extern uint64_t g_edit_controls_slot;       /* 0x282160 — kind-0 slot */
extern uint64_t g_edit_controls_panel;      /* 0x282168 — edit-controls panel (kind 2) */
extern uint64_t g_edit_controls_flag_slot;  /* 0x282170 — kind-4 slot */
extern uint64_t g_edit_controls_row_a;      /* 0x282178 — kind-1 row slots (0x158 stride) */
extern uint64_t g_edit_controls_row_b;      /* 0x2822d0 */
extern uint64_t g_edit_controls_row_c;      /* 0x282428 */
extern uint64_t g_edit_controls_row_d;      /* 0x282580 */
extern uint64_t g_edit_controls_status_a;   /* 0x2826e0 — kind-0 + extra (also under chooser root) */
extern uint64_t g_edit_controls_status_a_x; /* 0x2826e8 */
extern uint64_t g_edit_controls_status_b;   /* 0x282838 */
extern uint64_t g_edit_controls_status_b_x; /* 0x282840 */
extern uint64_t g_edit_controls_status_c;   /* 0x282990 */
extern uint64_t g_edit_controls_status_c_x; /* 0x282998 */
extern uint64_t g_edit_controls_extra_ctx;  /* 0x282ae0 */

/* ---- chooser block: mirror of ui/fonts.c chooser_state_t (fields used here) ---- */
typedef struct {
    uint64_t owner_context;   /* +0x000 0x281b08 */
    uint64_t root;            /* +0x008 0x281b10 */
    uint64_t panel;           /* +0x010 0x281b18 */
    uint64_t canvas;          /* +0x018 0x281b20 */
    uint64_t hint;            /* +0x0b0 0x281bb8 */
    uint64_t buttons[6];      /* +0x148 0x281c50, 0x98 stride */
    uint64_t title;           /* +0x4d8 0x281fe0 */
    uint64_t title_obj;       /* +0x4e0 0x281fe8 — registry resolve slot */
    uint64_t title_obj_x;     /* +0x4e8 0x281ff0 — registry resolve extra */
    uint64_t status_label;    /* +0x570 0x282078 */
    uint64_t status_obj;      /* +0x578 0x282080 — registry resolve slot */
    uint64_t status_obj_x;    /* +0x580 0x282088 — registry resolve extra */
    uint64_t built_pair;      /* +0x608 0x282110 {built, contract_failed} */
    uint32_t open;            /* +0x610 0x282118 */
} chooser_state_mirror_t;
extern chooser_state_mirror_t g_chooser_state; /* 0x281b08 — built by widgets.c, see ui/fonts.c */

/* ---- own later chunks (b-chain) ---- */
extern int is_plausible_ptr_mb(uint64_t v);                   /* misc.c (b-chain) */
extern int widget_kind1_validate(uint64_t widget, uint64_t ctx,
                                 void *snapshot);             /* misc.c (b-chain) @ 00177620 */
extern int widget_detach_release(uint64_t widget, int mode,
                                 uint32_t *fail);             /* misc.c (b-chain) @ 00177834 */
extern int widget_ancestor_depth_check(uint64_t ctx, uint64_t widget,
                                       uint64_t target,
                                       int expect_id);        /* misc.c (b-chain) @ 00181264 */
extern int widget_registry_resolve(void *page, uint64_t *slot,
                                   uint64_t ctx, int kind,
                                   uint64_t extra);           /* misc.c (b-chain) @ 001813b8 */
extern int theme_preview_active_check(void);                  /* misc.c (b-chain) @ 00181754 */
extern uint64_t engine_singleton_1307e20_read(void);          /* misc.c (b-chain) @ 0017ec28 */
extern int rich_panel_page_state_build(void *page, uint64_t arg); /* misc.c (b-chain) @ 00176d98 (below) */

/* ---- other files / a-chain ---- */
extern int code_region_hash_verify(uint64_t rva, uint64_t len,
                                   const char *hex_digest);   /* misc.c (a-chain) @ 0014d168 */

/* ===== text formatting trampolines ===== */

/*
 * ui_text_style_format — checked vsnprintf formatter for styled UI text.
 *
 * Compiled as a variadic __vsnprintf_chk trampoline (the first Ghidra
 * param is the phantom stack-arg register; the real arguments begin at
 * buf). fortify_slen is the fortify object size (SIZE_MAX for in-place
 * widget text), write_cap the actual write cap (raw call sites use
 * 0x60/0x80/0x140). No return value (raw discards it). @ 00176a24
 */
void ui_text_style_format(char *buf, size_t fortify_slen, size_t write_cap,
                          const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    __vsnprintf_chk(buf, write_cap, 0, fortify_slen, fmt, ap);
    va_end(ap);
}

/*
 * ui_text_format — fonts.c-facing twin of ui_text_style_format (the
 * committed ui/fonts.c externs this name for @ 00176a24). Same trampoline
 * body; see ui_text_style_format above.
 */
void ui_text_format(char *buf, size_t cap_a, size_t cap_b, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    __vsnprintf_chk(buf, cap_b, 0, cap_a, fmt, ap);
    va_end(ap);
}

/* ===== widget pair release (rich view teardown) ===== */

/*
 * ui_widget_pair_release — validate and optionally release a widget pair.
 *
 * ctx must be the live UI context, the entitlement service installed, the
 * host ready, and the 0xc10 panel page state buildable. Each non-NULL pair
 * member must not be duplicated in the pair, must not be any scene/panel
 * identity node ({scene_root,ctx}, {view_a,view_b}, the cached clips
 * g_clip_cache.clip_a/b/c) and must not appear in the first
 * g_menu_entry_index menu entry records (0xd8 stride, widget at +0).
 * Every surviving member must then be a kind-1 widget (vtable
 * game+0x11c0a48) that passes widget_kind1_validate and is absent from
 * the resolved page slots. mode 0 only validates; mode != 0 detaches both
 * members via widget_detach_release and clears the pair. Returns 1 on
 * success/clean, 0 when a member is claimed or fails validation, and -1
 * when the detach failed (fail flag set). @ 00176ad4
 */
int ui_widget_pair_release(uint64_t ctx, uint64_t *pair, uint32_t mode)
{
    uint64_t page[0x182];    /* 0xc10 panel page state */
    uint64_t snapshot[0xb];  /* 0x58 kind-1 pair snapshot */

    if (pair == NULL || mode > 1 || ctx != g_ui_context)
        return 0;
    if (g_plus_service_installed == 0 || !g_host_ready(g_host_ctx))
        return 0;
    if (rich_panel_page_state_build(page, 0) == 0)
        return 0;

    for (int i = 0; i < 2; i++) {
        uint64_t w = pair[i];
        if (w == 0)
            continue;
        if (w == pair[1 - i])
            return 0;

        /* identity pairs and cached clips are claimed */
        if (w == g_ui_scene_root || w == g_ui_context
            || w == g_view_object_a || w == g_view_object_b
            || w == g_clip_cache.clip_a || w == g_clip_cache.clip_b
            || w == g_clip_cache.clip_c)
            return 0;

        /* the selected menu entry records (0..index-1) must not own w */
        if (g_menu_entry_index > 0) {
            for (int r = 0; r < g_menu_entry_index; r++) {
                if (w == *(const uint64_t *)(g_menu_entry_table
                                             + (size_t)r * 0xd8))
                    return 0;
            }
        }

        /* unclaimed: must validate as kind-1 and be absent from the page */
        uint64_t vtable = 0;
        if (!is_plausible_ptr_mb(w + 8)
            || g_host_read(g_host_ctx, w, &vtable, 8) != 1
            || !is_plausible_ptr_mb(vtable) || (vtable & 7) != 0
            || vtable != g_game_base + 0x11c0a48
            || widget_kind1_validate(w, ctx, snapshot) == 0)
            return 0;

        uint32_t count = *(uint32_t *)((uint8_t *)page + 0xc00);
        for (uint32_t s = 0; s < count; s++) {
            uint64_t slot_addr = *(uint64_t *)((uint8_t *)page + s * 0x20);
            if (w == *(uint64_t *)slot_addr)
                return 0;
        }
    }

    if (mode == 0)
        return 1;

    /* detach both members, first slot then second */
    uint32_t fail = 0;
    if (pair[0] == 0) {
        if (pair[1] == 0)
            return 1;
        if (widget_detach_release(pair[1], 1, &fail) != 0) {
            pair[1] = 0;
            return 1;
        }
    } else {
        if (widget_detach_release(pair[0], 1, &fail) != 0) {
            pair[0] = 0;
            if (pair[1] == 0)
                return 1;
            if (widget_detach_release(pair[1], 1, &fail) != 0) {
                pair[1] = 0;
                return 1;
            }
        }
    }
    return -(int)(fail != 0);
}

/* ===== rich panel page state ===== */

/*
 * rich_panel_page_state_build — build the 0xc10-byte panel page state:
 * verify three engine code regions by SHA-256, then resolve every live
 * widget slot group through widget_registry_resolve (which appends
 * 0x20-byte entries to the page and keeps the count at +0xc00): the rich
 * panel ctx group (17 children + 5 kind-2 + 5 kind-4 + 6 kind-0 rows),
 * the two 0x820 HUD label states, the panel/page-list pair, the
 * edit-controls block (9-slot group), the theme view and the chooser
 * block. The second argument is stored at page+0xc08. Returns 1 when
 * every live slot resolved and a theme preview is not active.
 * @ 00176d98
 */
int rich_panel_page_state_build(void *page, uint64_t arg)
{
    memset(page, 0, 0xc10);
    *(uint64_t *)((uint8_t *)page + 0xc08) = arg;

    if (code_region_hash_verify(0x8897e8, 0x134,
        "eaaf4013a20aee66e9a61ff61032f00d36d4843de800a1fb3f8865d9e3c32edb") == 0)
        return 0;
    if (code_region_hash_verify(0x88991c, 0x24,
        "012d8d1313927592cb3f04e0c9f413139c8eab45f712823ba6788d3cb650fa3e") == 0)
        return 0;
    if (code_region_hash_verify(0x5940d0, 0x60,
        "13a9d8594868ac625edc10fae30f2e9a166b7563a470769d50287b70be1501c7") == 0)
        return 0;

    /* rich panel group: {root 0x283338, ctx 0x283340, page ctx 0x283980} */
    if (g_rich_panel_ctx == 0) {
        if (g_rich_panel_page_ctx != 0)
            return 0;
    } else {
        if (widget_ancestor_depth_check(g_rich_panel_root, g_rich_panel_ctx,
                                        g_rich_panel_page_ctx, 0x12) == 0)
            return 0;
        for (int i = 0; i < 6; i++) /* kind-1 rows 0-5 */
            if (widget_registry_resolve(page, &g_rich_panel_rows_a[i].widget,
                                        g_rich_panel_ctx, 1, 0) == 0)
                return 0;
        for (int i = 0; i < 5; i++) /* kind-2 slots */
            if (widget_registry_resolve(page, &g_rich_panel_k2_slots[i],
                                        g_rich_panel_ctx, 2, 0) == 0)
                return 0;
        for (int i = 0; i < 6; i++) /* kind-0 rows with extras */
            if (widget_registry_resolve(page, &g_rich_panel_rows_b[i].widget,
                                        g_rich_panel_ctx, 0,
                                        g_rich_panel_rows_b[i].extra) == 0)
                return 0;
        if (widget_registry_resolve(page, &g_rich_panel_ctx_slot,
                                    g_rich_panel_ctx, 0, 0) == 0)
            return 0;
        if (widget_registry_resolve(page, &g_rich_panel_ctx, g_ui_context,
                                    3, 0) == 0)
            return 0;
        for (int i = 0; i < 5; i++) /* kind-4 slots */
            if (widget_registry_resolve(page, &g_rich_panel_k4_slots[i],
                                        0, 4, 0) == 0)
                return 0;
    }

    /* first 0x820 HUD label state */
    if (g_diag_label_state[0] == 0 && *(uint64_t *)&g_diag_label_state[0] == 0) {
        if (g_diag_label_frame != 0)
            return 0;
    } else {
        if (g_diag_label_ctx != g_ui_context)
            return 0;
        if (widget_registry_resolve(page, (uint64_t *)&g_diag_label_state,
                                    g_diag_label_ctx, 0,
                                    g_diag_label_frame) == 0)
            return 0;
    }

    /* second 0x820 HUD label state */
    if (g_diag_label_state_2[0] == 0 && *(uint64_t *)&g_diag_label_state_2[0] == 0) {
        if (g_diag_label_frame_2 != 0)
            return 0;
    } else {
        if (g_diag_label_ctx_2 != g_ui_context)
            return 0;
        if (widget_registry_resolve(page, (uint64_t *)&g_diag_label_state_2,
                                    g_diag_label_ctx_2, 0,
                                    g_diag_label_frame_2) == 0)
            return 0;
    }

    /* panel + page-list pair */
    if (g_panel_widget != 0) {
        if (g_page_context_widget != g_ui_context)
            return 0;
        if (widget_registry_resolve(page, &g_panel_widget,
                                    g_page_context_widget, 1, 0) == 0)
            return 0;
    }
    if (g_page_list_widget != 0) {
        if (g_page_context_widget != g_ui_context)
            return 0;
        if (widget_registry_resolve(page, &g_page_list_widget,
                                    g_page_context_widget, 1, 0) == 0)
            return 0;
    }

    /* edit-controls main slot */
    if (g_edit_controls_widget == 0) {
        if (g_edit_controls_flag != 0)
            return 0;
    } else {
        if (g_edit_controls_flag == 0)
            return 0;
        if (g_edit_controls_root != g_ui_context)
            return 0;
        if (widget_registry_resolve(page, &g_edit_controls_widget,
                                    g_edit_controls_root, 1, 0) == 0)
            return 0;
    }

    /* edit-controls 9-slot group {root 0x282150, ctx 0x282158, extra 0x282ae0} */
    if (g_edit_controls_ctx != 0) {
        if (widget_ancestor_depth_check(g_edit_controls_block_root,
                                        g_edit_controls_ctx,
                                        g_edit_controls_extra_ctx, 9) == 0)
            return 0;
        if (widget_registry_resolve(page, &g_edit_controls_row_a,
                                    g_edit_controls_ctx, 1, 0) == 0)
            return 0;
        if (widget_registry_resolve(page, &g_edit_controls_row_b,
                                    g_edit_controls_ctx, 1, 0) == 0)
            return 0;
        if (widget_registry_resolve(page, &g_edit_controls_row_c,
                                    g_edit_controls_ctx, 1, 0) == 0)
            return 0;
        if (widget_registry_resolve(page, &g_edit_controls_row_d,
                                    g_edit_controls_ctx, 1, 0) == 0)
            return 0;
        if (widget_registry_resolve(page, &g_edit_controls_panel,
                                    g_edit_controls_ctx, 2, 0) == 0)
            return 0;
        if (widget_registry_resolve(page, &g_edit_controls_status_a,
                                    g_edit_controls_ctx, 0,
                                    g_edit_controls_status_a_x) == 0)
            return 0;
        if (widget_registry_resolve(page, &g_edit_controls_status_b,
                                    g_edit_controls_ctx, 0,
                                    g_edit_controls_status_b_x) == 0)
            return 0;
        if (widget_registry_resolve(page, &g_edit_controls_status_c,
                                    g_edit_controls_ctx, 0,
                                    g_edit_controls_status_c_x) == 0)
            return 0;
        if (widget_registry_resolve(page, &g_edit_controls_slot,
                                    g_edit_controls_ctx, 0, 0) == 0)
            return 0;
        if (widget_registry_resolve(page, &g_edit_controls_ctx,
                                    g_ui_context, 3, 0) == 0)
            return 0;
        if (widget_registry_resolve(page, &g_edit_controls_flag_slot,
                                    0, 4, 0) == 0)
            return 0;
    }

    /* theme view {view 0x2844f0, flag 0x2844f8, ctx 0x2844e8, extra 0x284500} */
    if (g_theme_view_flag != 0) {
        if (g_theme_view_ctx != g_ui_context)
            return 0;
        if (g_theme_view != engine_singleton_1307e20_read())
            return 0;
        if (widget_registry_resolve(page, &g_theme_view_flag, g_theme_view,
                                    0, g_theme_view_extra) == 0)
            return 0;
    }

    /* chooser block (root 0x281b10, built pair 0x282110) */
    if (g_chooser_state.root != 0) {
        if (widget_ancestor_depth_check(g_chooser_state.owner_context,
                                        g_chooser_state.root,
                                        g_chooser_state.built_pair, 0xb) == 0)
            return 0;
        if (widget_registry_resolve(page, &g_chooser_state.canvas,
                                    g_chooser_state.root, 1, 0) == 0) /* 0x281b20 */
            return 0;
        if (widget_registry_resolve(page, &g_chooser_state.hint,
                                    g_chooser_state.root, 1, 0) == 0) /* 0x281bb8 */
            return 0;
        for (int i = 0; i < 6; i++) /* 0x281c50.., 0x98 stride */
            if (widget_registry_resolve(page, &g_chooser_state.buttons[i],
                                        g_chooser_state.root, 1, 0) == 0)
                return 0;
        if (widget_registry_resolve(page, &g_chooser_state.title_obj,
                                    g_chooser_state.root, 0,
                                    g_chooser_state.title_obj_x) == 0) /* 0x281fe8 */
            return 0;
        if (widget_registry_resolve(page, &g_chooser_state.status_obj,
                                    g_chooser_state.root, 0,
                                    g_chooser_state.status_obj_x) == 0) /* 0x282080 */
            return 0;
        if (widget_registry_resolve(page, &g_chooser_state.panel,
                                    g_chooser_state.root, 0, 0) == 0) /* 0x281b18 */
            return 0;
        if (widget_registry_resolve(page, &g_chooser_state.root,
                                    g_ui_context, 3, 0) == 0) /* 0x281b10 */
            return 0;
        if (widget_registry_resolve(page, &g_chooser_state.built_pair,
                                    0, 4, 0) == 0) /* 0x282110 */
            return 0;
    }

    return theme_preview_active_check() != 0;
}
/* misc_b chain chunk 5: covers raw lines 12001-12500 */

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

/* ---- shared host/service globals (a-chain, digest-mandated names) ---- */
extern void *g_host_ctx;                                      /* 0x1a7cc8 */
extern int (*g_host_read)(void *ctx, uint64_t addr, void *out,
                          uint32_t len);                      /* 0x1a7ce8 */
extern uint64_t g_game_base;                                  /* 0x1a7cd0 */
extern uint64_t g_ui_scene_root;                              /* 0x1a7d28 */
extern uint64_t g_libg_span;                                  /* 0x10f740 — {base,size} of libg.so (packed) */
extern uint64_t g_engine_base;                                /* 0x281a48 */

/* ---- clip ops channel (typedef in chunk 3) ---- */
extern int host_read_gate(uint64_t ctx, uint64_t addr, void *out,
                          uint64_t len);                      /* misc.c (b-chain) @ 0017fe9c */
extern int host_write_gate(uint64_t ctx, void *dst, const void *src,
                           size_t len);                       /* misc.c (b-chain) @ 0017fee0 */
extern int clip_ops_integrity_check(uint64_t ctx);            /* misc.c (b-chain) @ 001802bc */
extern void engine_call_11a2840_adapter(uint64_t ctx,
                                        uint64_t a);          /* misc.c (b-chain) @ 00180540 */
extern void engine_call_88b344_adapter(uint64_t ctx, uint64_t a,
                                       uint32_t b);           /* misc.c (b-chain) @ 00180548 */
extern void engine_call_593f14_adapter(uint64_t ctx, uint64_t a,
                                       uint32_t b);           /* misc.c (b-chain) @ 00180564 */
extern void engine_call_594130_adapter(uint64_t ctx, uint64_t a,
                                       uint64_t b);           /* misc.c (b-chain) @ 00180580 */
extern void engine_call_59567c_adapter(uint64_t ctx,
                                       uint64_t a);           /* misc.c (b-chain) @ 0018058c */
extern void engine_call_5940d0_gated(uint64_t ctx,
                                     uint64_t a);             /* misc.c (b-chain) @ 001805a4 */
extern void engine_call_59531c_adapter(int x, int y, uint64_t ctx,
                                       uint64_t clip);         /* misc.c (b-chain) @ 00180724 */
extern void engine_call_88d290_adapter(uint64_t ctx, uint64_t a,
                                       uint32_t b);           /* misc.c (b-chain) @ 0018073c */
extern void engine_call_594e80_adapter(uint64_t ctx, uint64_t a,
                                       uint32_t b);           /* misc.c (b-chain) @ 00180758 */
extern int menu_clip_writable_check(uint64_t ctx, void *addr,
                                    size_t len);   /* menu_engine.c @ 00180034 */
extern void menu_probe_log(uint64_t ctx,
                           const char *event);     /* menu_engine.c @ 0017fbc8 */

/* ---- own later chunks (b-chain) ---- */
extern int is_plausible_ptr_mb(uint64_t v);                   /* misc.c (b-chain) */
extern int widget_child_link_valid(uint64_t node,
                                   uint64_t parent);          /* misc.c (b-chain) @ 0016d5cc */
extern int widget_pair_snapshot_read(const uint64_t *pair, int index,
                                     uint64_t *out);          /* misc.c (b-chain) @ 0017c704 */
extern int clip_slot_precondition_check(void *ops, void *record,
                                        uint64_t root,
                                        uint32_t index);      /* misc.c (b-chain) @ 00180d80 */
extern void engine_call_594130(uint64_t a, uint64_t b);       /* misc.c (b-chain) @ 0016cfa4 */

/* ---- clip cache record (typedef in chunk 4) ---- */
extern clip_cache_t g_clip_cache;                             /* 0x22d4e0 */

/* misc_b-owned: span pair copy right after the clip cache record */
uint64_t g_clip_span_pair;  /* 0x22d510 — copy of g_libg_span {base,size} */

/*
 * widget_pair_t — the {node, parent} pair handed to widget_pair_snapshot_read.
 * parent comes from the kind-1 class parent field (widget +0xb0), the
 * children array from widget +0x88 (sanity-gated, diagnostic).
 */
typedef struct {
    uint64_t node;      /* +0x00 */
    uint64_t parent;    /* +0x08 — widget+0xb0 field, plausible-gated */
    uint64_t _r10;      /* +0x10 */
    uint64_t children;  /* +0x18 — widget+0x88 field, plausible-gated */
} widget_pair_t;

/* ===== kind-1 widget validation ===== */

/*
 * widget_kind1_validate — deep-validate a kind-1 widget (vtable
 * game+0x11c0a48) under ctx: read the class parent (+0xb0) and children
 * array (+0x88) into a pair record, require the ctx child link, the scene
 * owner at +0x30 to be NULL or the scene root, and the flag byte at +0x48
 * to read 1. On success reads the pair snapshot (index 1) into
 * snapshot_out (0x50 bytes). Returns the snapshot result. @ 00177620
 */
int widget_kind1_validate(uint64_t widget, uint64_t ctx, void *snapshot_out)
{
    widget_pair_t pair;
    uint64_t scratch;

    memset(&pair, 0, sizeof pair);
    pair.node = widget;

    scratch = 0;
    if (is_plausible_ptr_mb(widget + 0xb8)
        && g_host_read(g_host_ctx, widget + 0xb0, &scratch, 8) == 1
        && is_plausible_ptr_mb(scratch) && (scratch & 7) == 0)
        pair.parent = scratch;

    scratch = 0;
    if (is_plausible_ptr_mb(widget + 0x90)
        && g_host_read(g_host_ctx, widget + 0x88, &scratch, 8) == 1
        && is_plausible_ptr_mb(scratch) && (scratch & 7) == 0)
        pair.children = scratch;

    if (widget_child_link_valid(widget, ctx) != 0) {
        uint64_t owner = 0;
        if (is_plausible_ptr_mb(widget + 0x38)
            && g_host_read(g_host_ctx, widget + 0x30, &owner, 8) == 1
            && (owner == 0 || owner == g_ui_scene_root)) {
            uint8_t flag = 0xff; /* prefill: a failed read must not pass */
            if (is_plausible_ptr_mb(widget + 0x49)
                && g_host_read(g_host_ctx, widget + 0x48, &flag, 1) == 1
                && (int8_t)flag == 1)
                return widget_pair_snapshot_read((const uint64_t *)&pair, 1,
                                       snapshot_out) != 0;
        }
    }
    return 0;
}

/* ===== widget detach + release ===== */

/*
 * widget_detach_release — detach a widget through the engine and release it.
 *
 * mode != 0 first reads the pair snapshot (index 1) and requires it to
 * succeed. The fail flag is set before the engine detach (engine base
 * game+0x59567c); the widget must then read back detached (parent +0x38
 * NULL, child index +0x40 == -1). When the snapshot kind (out+0x4c) is 3,
 * engine_call_594130(out+0x18, out+0x08) runs first and a re-snapshot
 * must come back as kind 2. The final release call is game+0x887e90 for
 * mode != 0 and game+0x5d48e8 for mode 0. Returns 1 on success, 0 on any
 * gate failure (fail left set). @ 00177834
 */
int widget_detach_release(uint64_t widget, int mode, uint32_t *fail)
{
    uint64_t snap_out[0xa];   /* 0x50-byte pair snapshot */
    widget_pair_t pair;
    uint64_t scratch;

    if (mode != 0) {
        memset(snap_out, 0, sizeof snap_out);
        memset(&pair, 0, sizeof pair);
        pair.node = widget;

        scratch = 0;
        if (is_plausible_ptr_mb(widget + 0xb8)
            && g_host_read(g_host_ctx, widget + 0xb0, &scratch, 8) == 1
            && is_plausible_ptr_mb(scratch) && (scratch & 7) == 0)
            pair.parent = scratch;

        scratch = 0;
        if (is_plausible_ptr_mb(widget + 0x90)
            && g_host_read(g_host_ctx, widget + 0x88, &scratch, 8) == 1
            && is_plausible_ptr_mb(scratch) && (scratch & 7) == 0)
            pair.children = scratch;

        if (widget_pair_snapshot_read((const uint64_t *)&pair, 1,
                                 snap_out) == 0)
            return 0;
    }

    *fail = 1;
    ((void (*)(uint64_t))(g_game_base + 0x59567c))(widget);

    uint64_t parent = 1;   /* prefills: a failed read must not pass */
    int32_t index = 0;
    if (!is_plausible_ptr_mb(widget + 0x40)
        || g_host_read(g_host_ctx, widget + 0x38, &parent, 8) != 1
        || parent != 0)
        return 0;
    if (!is_plausible_ptr_mb(widget + 0x44)
        || g_host_read(g_host_ctx, widget + 0x40, &index, 4) != 1
        || index != -1)
        return 0;

    if (mode != 0 && *(int32_t *)((uint8_t *)snap_out + 0x4c) == 3) {
        /* kind-3 widgets get an engine 0x594130 call, then must re-snapshot as kind 2 */
        engine_call_594130(snap_out[3], snap_out[1]); /* (out+0x18, out+0x08) */

        memset(&pair, 0, sizeof pair);
        pair.node = widget;
        pair.children = snap_out[3];
        if (widget_pair_snapshot_read((const uint64_t *)&pair, 1,
                                 snap_out) == 0
            || *(int32_t *)((uint8_t *)snap_out + 0x4c) != 2)
            return 0;
    }

    uint64_t release_rva = mode != 0 ? 0x887e90 : 0x5d48e8;
    ((void (*)(uint64_t))(g_game_base + release_rva))(widget);
    return 1;
}

/* ===== clip cache release ===== */

/*
 * ui_clip_cache_release — release the cached clip widgets through the ops
 * channel. With no clip A cached the result is simply "cache empty" (all
 * three clips and the span-pair high half zero). Otherwise the clip slot
 * record is precondition-checked and the channel integrity-verified; when
 * the two-phase flag (clip cache +0x18) is set, clip A is detached first
 * and the record re-checked. Clip C is then detached, verified detached
 * (parent NULL, index -1) with clip B's child count at +0x4e zero, both
 * clips get the engine 0x5940d0 call and the whole record is cleared.
 * On failure the span pair is refreshed from g_libg_span. Returns 1 when
 * the cache was released (or was already empty). @ 00178714
 */
int ui_clip_cache_release(void)
{
    clip_ops_t ops;

    if (g_clip_cache.clip_a == 0)
        return g_clip_cache.clip_b == 0 && g_clip_cache.clip_c == 0
               && (uint32_t)(g_clip_span_pair >> 32) == 0;

    memset(&ops, 0, sizeof ops);
    ops.game_base = g_game_base;
    ops.read_gate = host_read_gate;
    ops.write_gate = host_write_gate;
    ops.writable_check = menu_clip_writable_check;
    ops.integrity_check = clip_ops_integrity_check;
    ops.call_11a2840 = engine_call_11a2840_adapter;
    ops.call_88b344 = engine_call_88b344_adapter;
    ops.call_593f14 = engine_call_593f14_adapter;
    ops.call_594130 = engine_call_594130_adapter;
    ops.call_59567c = engine_call_59567c_adapter;
    ops.call_5940d0 = engine_call_5940d0_gated;
    ops.call_59531c = engine_call_59531c_adapter;
    ops.call_88d290 = engine_call_88d290_adapter;
    ops.call_594e80 = engine_call_594e80_adapter;
    ops.probe_log = menu_probe_log;

    if (clip_slot_precondition_check(&ops, &g_clip_cache,
                                     g_ui_scene_root, 0) == 0)
        goto fail;
    if (ops.integrity_check(ops.ctx) == 0)
        goto fail;

    if (g_clip_cache.flag != 0) {
        /* two-phase: detach clip A first, then re-check the record */
        ops.call_59567c(ops.ctx, g_clip_cache.clip_a);
        g_clip_cache.flag = 0;
        if (clip_slot_precondition_check(&ops, &g_clip_cache, 0, 0) == 0)
            goto fail;
    }

    /* detach clip C and verify both the detach and clip B's child count */
    ops.call_59567c(ops.ctx, g_clip_cache.clip_c);

    uint64_t parent = 1;   /* prefills: a failed read must not pass */
    int32_t index = 0;
    uint16_t b_count = 0;
    if (is_plausible_ptr_mb(g_clip_cache.clip_c + 0x40)
        && g_host_read(g_host_ctx, g_clip_cache.clip_c + 0x38, &parent, 8) == 1
        && parent == 0
        && g_host_read(g_host_ctx, g_clip_cache.clip_c + 0x40, &index, 4) == 1
        && index == -1
        && is_plausible_ptr_mb(g_clip_cache.clip_b + 0x50)
        && g_host_read(g_host_ctx, g_clip_cache.clip_b + 0x4e, &b_count, 2) == 1
        && b_count == 0) {
        ops.call_5940d0(ops.ctx, g_clip_cache.clip_c);
        g_clip_cache.clip_c = 0;
        ops.call_5940d0(ops.ctx, g_clip_cache.clip_a);
        g_clip_span_pair = 0;
        g_clip_cache.flag = 0;
        g_clip_cache.clip_c = 0;
        g_clip_cache.state = 0;
        memset(&g_clip_cache.frame_w, 0, sizeof g_clip_cache.frame_w);
        g_clip_cache.clip_b = 0;
        g_clip_cache.clip_a = 0;
        return 1;
    }

fail:
    g_clip_span_pair = g_libg_span;
    return 0;
}

/*
 * widget_detach_and_verify — call the engine detach (game+0x59567c) on the
 * widget and verify it reads back detached: parent (+0x38) NULL and child
 * index (+0x40) -1. Returns 1 when detached. @ 001789a8
 */
int widget_detach_and_verify(uint64_t widget)
{
    uint64_t parent = 1;   /* prefills: a failed read must not pass */
    int32_t index = 0;

    ((void (*)(uint64_t))(g_game_base + 0x59567c))(widget);

    if (is_plausible_ptr_mb(widget + 0x40)
        && g_host_read(g_host_ctx, widget + 0x38, &parent, 8) == 1
        && parent == 0
        && is_plausible_ptr_mb(widget + 0x44)
        && g_host_read(g_host_ctx, widget + 0x40, &index, 4) == 1
        && index == -1)
        return 1;
    return 0;
}
/* misc_b chain chunk 6: covers raw lines 12546-13000 */

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
#include <pthread.h>

/* ---- shared host/service globals (digest-mandated names) ---- */
extern void *g_host_ctx;                                      /* 0x1a7cc8 */
extern int (*g_host_read)(void *ctx, uint64_t addr, void *out,
                          uint32_t len);                      /* 0x1a7ce8 */
extern uint64_t g_game_base;                                  /* 0x1a7cd0 */
extern uint64_t g_ui_scene_root;                              /* 0x1a7d28 */
extern uint64_t g_ui_context;                                 /* 0x1a7d30 */
extern uint64_t g_view_object_a;                              /* 0x1a7d38 */
extern int32_t g_menu_entry_index;                            /* 0x1a7d18 */
extern uint64_t g_libg_span;                                  /* 0x10f740 — {base,size} of libg.so (packed) */
extern float g_ui_density;                                    /* 0x1e53fc */
extern float g_ui_center_x;                                   /* 0x1e5400 */
extern float g_ui_center_y;                                   /* 0x1e5404 */
extern float g_ui_transform_scale;                            /* 0x1a7d5c */
extern float g_ui_transform_tx;                               /* 0x1a7d60 */
extern float g_ui_transform_ty;                               /* 0x1a7d64 */
extern uint32_t g_menu_open_state;                            /* 0x22d7ec — clip-ops verification cache (== 1 verified) */

/* ---- misc_b-owned frame state (chunk 3) ---- */
extern uint32_t g_frame_state_armed;      /* 0x22d794 */
extern uint32_t g_screen_active;          /* 0x22d798 */
extern uint32_t g_screen_sub_id;          /* 0x22d79c */
extern uint8_t  g_screen_id;              /* 0x22d7a0 */
extern uint32_t g_frame_gen_a;            /* 0x22d7a8 */
extern uint32_t g_frame_gen_b;            /* 0x22d7b0 */
extern uint32_t g_clip_move_count;        /* 0x22d7c8 */
extern float    g_clip_y_cached;          /* 0x22d7e8 */
extern uint64_t g_frame_cache_7f0;        /* 0x22d7f0 — {center_x, density} viewport cache */
extern uint64_t g_frame_cache_7f8;        /* 0x22d7f8 — {transform_scale, center_y} viewport cache */
extern uint64_t g_frame_cache_800;        /* 0x22d800 — {transform_ty, transform_tx} viewport cache */
extern uint64_t g_frame_scene_ctx;        /* 0x22d808 */
extern uint32_t g_frame_cache_valid;      /* 0x22d810 */
extern float    g_diag_overlay_scale;     /* 0x22d818 — global UI overlay scale (min(h/720, 1.5)) */

/* ---- misc_b-only pointer-state words (chunk 3) ---- */
extern uint64_t g_pointer_state_528;      /* 0x22d528 */
extern uint64_t g_pointer_state_538;      /* 0x22d538 */
extern uint32_t g_pointer_state_540;      /* 0x22d540 */
extern uint64_t g_pointer_state_548;      /* 0x22d548 */
extern uint32_t g_pointer_state_550;      /* 0x22d550 */
extern uint32_t g_pointer_state_558;      /* 0x22d558 */

/* ---- clip ops channel (typedef + veneers, chunk 3) ---- */
extern int host_read_gate(uint64_t ctx, uint64_t addr, void *out,
                          uint64_t len);                      /* misc.c (b-chain) @ 0017fe9c */
extern int host_write_gate(uint64_t ctx, void *dst, const void *src,
                           size_t len);                       /* misc.c (b-chain) @ 0017fee0 */
extern int clip_ops_integrity_check(uint64_t ctx);            /* misc.c (b-chain) @ 001802bc */
extern void engine_call_11a2840_adapter(uint64_t ctx,
                                        uint64_t a);          /* misc.c (b-chain) @ 00180540 */
extern void engine_call_88b344_adapter(uint64_t ctx, uint64_t a,
                                       uint32_t b);           /* misc.c (b-chain) @ 00180548 */
extern void engine_call_593f14_adapter(uint64_t ctx, uint64_t a,
                                       uint32_t b);           /* misc.c (b-chain) @ 00180564 */
extern void engine_call_594130_adapter(uint64_t ctx, uint64_t a,
                                       uint64_t b);           /* misc.c (b-chain) @ 00180580 */
extern void engine_call_59567c_adapter(uint64_t ctx,
                                       uint64_t a);           /* misc.c (b-chain) @ 0018058c */
extern void engine_call_5940d0_gated(uint64_t ctx,
                                     uint64_t a);             /* misc.c (b-chain) @ 001805a4 */
extern void engine_call_59531c_adapter(int x, int y, uint64_t ctx,
                                       uint64_t clip);         /* misc.c (b-chain) @ 00180724 */
extern void engine_call_88d290_adapter(uint64_t ctx, uint64_t a,
                                       uint32_t b);           /* misc.c (b-chain) @ 0018073c */
extern void engine_call_594e80_adapter(uint64_t ctx, uint64_t a,
                                       uint32_t b);           /* misc.c (b-chain) @ 00180758 */
extern int menu_clip_writable_check(uint64_t ctx, void *addr,
                                    size_t len);   /* menu_engine.c @ 00180034 */
extern void menu_probe_log(uint64_t ctx,
                           const char *event);     /* menu_engine.c @ 0017fbc8 */

/* ---- own later chunks (b-chain) ---- */
extern int is_plausible_ptr_mb(uint64_t v);                   /* misc.c (b-chain) */
extern int widget_child_link_valid(uint64_t node,
                                   uint64_t parent);          /* misc.c (b-chain) @ 0016d5cc */
extern int clip_slot_precondition_check(void *ops, void *record,
                                        uint64_t root,
                                        uint32_t index);      /* misc.c (b-chain) @ 00180d80 */
extern void inertia_state_step(float x, float y, float *state,
                               uint64_t frame, int settle,
                               uint32_t mode, float id,
                               float gen);                     /* misc.c (b-chain) @ 00183230 */
extern void hold_tracker_feed(uint64_t frame, void *record,
                             uint32_t arg, int mode);         /* misc.c (b-chain) @ 00183658 */
extern int hold_anchor_still_valid(uint64_t frame, int reset,
                                   uint64_t ctx, int depth);   /* misc.c (b-chain) @ 001830b0 */
extern void ui_touch_state_reset(void *touch_state, float *scroll_state,
                                 uint64_t frame, uint32_t flag); /* misc.c (b-chain) @ 00179dc0 */

/* ---- clip cache record (typedef in chunk 4) ---- */
extern clip_cache_t g_clip_cache;                             /* 0x22d4e0 */

/* rodata park values for the pointer state */
extern const uint64_t g_pointer_park_guard;  /* 0x10fa90 — park value for the guard pair */
extern const uint64_t g_pointer_park_hold;   /* 0x10fa98 — park value for the hold pair */

/* ===== scroll / pointer / touch state block (0x22d518..0x22d5f8) ===== */

float    g_scroll_position;        /* 0x22d518 — inertia scroll position (state base, 18 floats) */
uint64_t g_pointer_state_pair;     /* 0x22d51c — packed pair; high half (0x22d520) used */
uint32_t g_pointer_latch;          /* 0x22d530 — pointer-event latch */
float    g_scroll_target;          /* 0x22d560 — touch scroll target */
uint32_t g_pointer_active;         /* 0x22d564 — pointer held latch */
uint32_t g_pointer_widget;         /* 0x22d568 — pointer in-region flag (registry: widget) */
uint32_t g_pointer_guard_lo;       /* 0x22d56c — low 32 of g_libg_span */
uint32_t g_pointer_guard_hi;       /* 0x22d570 — high 32 of g_libg_span */
uint32_t g_pointer_parked;         /* 0x22d574 — park flag (0x10fa90 high word) */
uint64_t g_pointer_hold_pair;      /* 0x22d578 — packed hold frame/id */
uint32_t g_pointer_frame_count;    /* 0x22d580 — last event record count */
uint32_t g_pointer_seq;            /* 0x22d584 — max pointer id seen */
uint64_t g_pointer_event_arg;      /* 0x22d588 — stored event arg */
uint64_t g_pointer_last_frame;     /* 0x22d590 — frame compare window */
uint64_t g_pointer_source;         /* 0x22d598 — event source ptr */
uint64_t g_pointer_touch_frame;    /* 0x22d5a0 — 500-frame touch window */
uint32_t g_pointer_repeat_count;   /* 0x22d5a8 — repeat counter */
uint64_t g_pointer_anchor_b0;      /* 0x22d5b0 — latched record copy B (words 0-1) */
uint64_t g_pointer_anchor_b8;      /* 0x22d5b8 — copy B (words 2-3) */
uint64_t g_pointer_cached_pair;    /* 0x22d5c0 — copy B (words 4-5) */
uint64_t g_pointer_param_6;        /* 0x22d5c8 — copy B (words 6-7) */
uint64_t g_pointer_param_8;        /* 0x22d5d0 — copy B (words 8-9) */
uint64_t g_pointer_hold_a0;        /* 0x22d5d8 — latched record copy A (words 0-1) */
uint64_t g_pointer_hold_a8;        /* 0x22d5e0 — copy A (words 2-3) */
uint64_t g_pointer_hold_b0;        /* 0x22d5e8 — copy A (words 4-5) */
uint64_t g_pointer_hold_b8;        /* 0x22d5f0 — copy A (words 6-7) */
uint64_t g_pointer_cached_arg;     /* 0x22d5f8 — copy A (words 8-9) */

clip_cache_t g_clip_cache;  /* 0x22d4e0 — the cached clip-widget record (defined here; chunk 5 externs it) */

/* ===== state cache reset ===== */

/*
 * ui_state_cache_reset — zero the pointer/touch state block
 * (0x22d51c pair, 0x22d530 latch, the whole 0x22d560..0x22d5f8 pointer
 * block), the 0x22d794/0x22d7a0/0x22d79c/0x22d7e8/0x22d810 frame words,
 * and bump both generation counters. The scroll position itself and the
 * 0x22d528..0x22d558 inertia words are NOT touched (they are cleared by
 * the screen-change path in ui_frame_state_sync instead). @ 00178ab0
 */
void ui_state_cache_reset(void)
{
    g_screen_sub_id = 0xffffffffu;
    g_frame_cache_valid = 0;
    g_clip_y_cached = 0.0f;
    g_frame_state_armed = 0;
    g_pointer_widget = 0;
    g_scroll_target = 0.0f;
    g_pointer_hold_pair = 0;
    g_pointer_guard_hi = 0;
    g_pointer_event_arg = 0;
    g_pointer_frame_count = 0;
    g_pointer_source = 0;
    g_pointer_last_frame = 0;
    g_frame_gen_b++;
    g_frame_gen_a++;
    g_pointer_repeat_count = 0;
    g_pointer_touch_frame = 0;
    g_pointer_anchor_b8 = 0;
    g_pointer_anchor_b0 = 0;
    g_pointer_param_6 = 0;
    g_pointer_cached_pair = 0;
    g_pointer_param_8 = 0;
    g_pointer_hold_b0 = 0;
    g_pointer_hold_a8 = 0;
    g_pointer_cached_arg = 0;
    g_pointer_hold_b8 = 0;
    g_pointer_latch = 0;
    g_pointer_state_pair = 0; /* 8 bytes: 0x22d51c + 0x22d520 */
}

/* ===== pointer event processing ===== */

/* latch the first valid record of a batch into the pointer state */
static void pointer_record_latch(uint64_t event_arg, const int *rec,
                                 uint32_t flag)
{
    g_pointer_active = 1;
    g_pointer_repeat_count++;
    g_pointer_guard_lo = (uint32_t)g_libg_span;
    g_pointer_guard_hi = (uint32_t)(g_libg_span >> 32);

    float x = (float)rec[1];
    float y = (float)rec[2];
    /* in-region test: x in [-530, 350], y in [132, 472] (NaN-safe) */
    uint32_t in_x = x <= 350.0f && x >= -530.0f;
    uint32_t in_y = y >= 132.0f && y <= 472.0f;
    g_pointer_widget = in_y ? in_x : 0;

    /* mirror the record into both cached copies (A at 0x22d5d8, B at 0x22d5b0) */
    g_pointer_hold_a8 = *(const uint64_t *)&rec[2];
    g_pointer_hold_a0 = *(const uint64_t *)&rec[0];
    g_pointer_hold_b8 = *(const uint64_t *)&rec[6];
    g_pointer_hold_b0 = *(const uint64_t *)&rec[4];
    g_pointer_cached_arg = *(const uint64_t *)&rec[8];
    g_pointer_anchor_b8 = *(const uint64_t *)&rec[2];
    g_pointer_anchor_b0 = *(const uint64_t *)&rec[0];
    g_pointer_param_6 = *(const uint64_t *)&rec[6];
    g_pointer_cached_pair = *(const uint64_t *)&rec[4];
    g_pointer_param_8 = *(const uint64_t *)&rec[8];
    g_pointer_seq = (uint32_t)rec[0];
    g_pointer_source = event_arg;

    inertia_state_step((float)rec[1], (float)rec[2], &g_scroll_position,
                       event_arg, 1, 1, (float)rec[0], flag);
    if (g_pointer_widget != 0 && g_pointer_latch == 0)
        g_pointer_guard_hi = 0;
}

/*
 * ui_pointer_events_handle — process one pointer/touch event batch.
 *
 * event_arg: frame identity; mode/ctx_arg: pass-through registers the raw
 * thunk forwards to hold_anchor_still_valid (renderer passes 1 and the
 * frame generation); record: 0x28-byte (10-word) touch records; count:
 * record count (< 11); flag: extra event argument. The hold anchor must
 * still be valid. count == 0 always ends in an inertia reset (mode 0),
 * after re-feeding an ongoing hold from the cached record. count > 0
 * validates every record (id >= 0, finite coords, kind bytes <= 1 and
 * different, no duplicate ids): a single valid record latches the pointer
 * (region test, record copies, inertia feed, optional hold dispatch) or
 * resets the touch state on the release flag; a multi-record or parked
 * batch runs a max-id scan and then parks the pointer state (inertia
 * mode 4). An invalid batch parks immediately. @ 00179000
 */
void ui_pointer_events_handle(uint64_t event_arg, uint64_t mode,
                              uint64_t ctx_arg, int *record,
                              uint32_t count, uint32_t flag)
{
    (void)mode;
    (void)ctx_arg;

    /* the raw thunk forwards the live registers (frame, 1, frame-gen, count) */
    if (hold_anchor_still_valid(event_arg, 1, ctx_arg, (int)count) == 0)
        return;

    if (count >= 0xb && !(record == NULL && count == 0))
        return;

    if (count == 0) {
        if (g_pointer_parked != 0) {
            g_pointer_parked = 0;
        } else if (g_pointer_active != 0) {
            /* re-feed the ongoing hold from cached copy A */
            int hold_rec[10];
            memset(hold_rec, 0, sizeof hold_rec);
            *(uint64_t *)&hold_rec[0] = g_pointer_hold_a0;
            hold_rec[2] = (int32_t)g_pointer_hold_a8;
            hold_rec[5] = (int32_t)(g_pointer_hold_b0 >> 32);
            *(uint64_t *)&hold_rec[6] = g_pointer_hold_b8;
            *(uint64_t *)&hold_rec[8] = g_pointer_cached_arg;
            hold_tracker_feed(event_arg, hold_rec, flag, 1);
        }
        inertia_state_step(0, 0, &g_scroll_position, event_arg, 0, 0,
                           -1.0f, flag);
        return;
    }

    /* validate every record: id >= 0, finite coords, kinds <= 1 + different, dupes */
    uint32_t all_valid = 1;
    for (uint32_t r = 0; r < count && all_valid; r++) {
        const int *rec = &record[r * 10];
        if (rec[0] < 0 || !isfinite((float)rec[1]) || isnan((float)rec[1])
            || !isfinite((float)rec[2]) || isnan((float)rec[2])
            || (uint32_t)rec[3] > 1 || (uint32_t)rec[4] > 1
            || rec[3] == rec[4]) {
            all_valid = 0;
            break;
        }
        for (uint32_t p = 0; p < r; p++) {
            if (record[p * 10] == rec[0]) {
                all_valid = 0; /* duplicate id stops the scan */
                break;
            }
        }
    }
    if (!all_valid)
        goto park;

    if (count < 2 && g_pointer_parked == 0) {
        int *rec = record;
        if (g_pointer_active == 0) {
            if (g_pointer_seq < (uint32_t)rec[0]) {
                pointer_record_latch(event_arg, rec, flag);
                if (rec[4] != 0)
                    hold_tracker_feed(event_arg, rec, flag, 1);
                return;
            }
            if (rec[3] != 0) {
                ui_touch_state_reset(&g_scroll_target, &g_scroll_position,
                                     event_arg, flag);
                return;
            }
            inertia_state_step(0, 0, &g_scroll_position, event_arg, 0, 0,
                               -1.0f, flag);
            return;
        }
        if (rec[0] != (int32_t)g_pointer_anchor_b0) {
            /* different id: re-feed the cached hold first */
            int hold_rec[10];
            memset(hold_rec, 0, sizeof hold_rec);
            *(uint64_t *)&hold_rec[0] = g_pointer_hold_a0;
            hold_rec[2] = (int32_t)g_pointer_hold_a8;
            hold_rec[5] = (int32_t)(g_pointer_hold_b0 >> 32);
            *(uint64_t *)&hold_rec[6] = g_pointer_hold_b8;
            *(uint64_t *)&hold_rec[8] = g_pointer_cached_arg;
            hold_tracker_feed(event_arg, hold_rec, flag, 1);
        }
        hold_tracker_feed(event_arg, rec, flag, rec[4]);
        return;
    }

    /* multi-record or parked batch: max-id scan, then park */
    {
        int max_id = (int32_t)g_pointer_seq;
        int *rec = record;
        uint32_t left = count;
        do {
            if (max_id < rec[0]) {
                max_id = rec[0];
                g_pointer_seq = (uint32_t)max_id;
            }
            rec += 10;
            left--;
        } while (left != 0);
    }

park:
    g_pointer_active = 0;
    g_pointer_hold_pair = g_pointer_park_hold;
    g_pointer_guard_hi = (uint32_t)g_pointer_park_guard;
    g_pointer_parked = (int32_t)(g_pointer_park_guard >> 32);
    inertia_state_step(0, 0, &g_scroll_position, event_arg, 0, 4, -1.0f,
                       flag);
}

/* ===== clip readiness + viewport cache ===== */

/*
 * ui_clips_ready_check — 1 when no clips are claimed (menu entry < 0x2c,
 * clips B/C and the span-pair high half all clear) or the cached clip
 * record validates: the two-phase flag equals g_view_object_a and the
 * clip slot precondition check passes through a fresh ops channel with
 * the current menu entry index (folded > 0x2a via -0x2b). @ 00179448
 */
int ui_clips_ready_check(void)
{
    if (g_clip_cache.clip_a == 0)
        return g_menu_entry_index < 0x2c && g_clip_cache.clip_b == 0
               && g_clip_cache.clip_c == 0
               && (uint32_t)(g_clip_span_pair >> 32) == 0;

    if (g_clip_cache.flag != g_view_object_a)
        return 0;

    clip_ops_t ops;
    memset(&ops, 0, sizeof ops);
    ops.game_base = g_game_base;
    ops.read_gate = host_read_gate;
    ops.write_gate = host_write_gate;
    ops.writable_check = menu_clip_writable_check;
    ops.integrity_check = clip_ops_integrity_check;
    ops.call_11a2840 = engine_call_11a2840_adapter;
    ops.call_88b344 = engine_call_88b344_adapter;
    ops.call_593f14 = engine_call_593f14_adapter;
    ops.call_594130 = engine_call_594130_adapter;
    ops.call_59567c = engine_call_59567c_adapter;
    ops.call_5940d0 = engine_call_5940d0_gated;
    ops.call_59531c = engine_call_59531c_adapter;
    ops.call_88d290 = engine_call_88d290_adapter;
    ops.call_594e80 = engine_call_594e80_adapter;
    ops.probe_log = menu_probe_log;

    uint32_t entry = 0;
    if (g_menu_entry_index > 0x2a)
        entry = (uint32_t)(g_menu_entry_index - 0x2b);
    return clip_slot_precondition_check(&ops, &g_clip_cache,
                                        g_ui_scene_root, entry) != 0;
}

/*
 * ui_viewport_cache_check — re-read the scene-root viewport (density at
 * +0x178, center_x at +0x4c, center_y at +0x54, ctx at +0x284) plus the
 * engine transform pair and compare everything against the packed frame
 * cache (0x22d7f0 {center_x, density}, 0x22d7f8 {transform_scale,
 * center_y}, 0x22d800 {transform_ty, transform_tx}) and the cached scene
 * ctx. Returns 1 when the cache is current. @ 001795ac
 */
int ui_viewport_cache_check(void)
{
    if (g_frame_cache_valid == 0 || g_ui_scene_root + 0x178 < 0x1000)
        return 0;

    uint64_t pair_178_4c = 0;
    if (g_host_read(g_host_ctx, g_ui_scene_root + 0x178, &pair_178_4c, 4) != 1)
        return 0;
    if (g_host_read(g_host_ctx, g_ui_scene_root + 0x4c,
                    (uint8_t *)&pair_178_4c + 4, 4) != 1)
        return 0;

    uint32_t scene_54;
    if (g_ui_scene_root + 0x54 < 0x1000
        || g_host_read(g_host_ctx, g_ui_scene_root + 0x54, &scene_54, 4) != 1)
        return 0;

    uint64_t scene_ctx = 0;
    if (!is_plausible_ptr_mb(g_ui_scene_root + 0x28c)
        || g_host_read(g_host_ctx, g_ui_scene_root + 0x284, &scene_ctx, 8) != 1)
        return 0;

    /* pack the engine transform globals the same way the cache stores them */
    union { float f; uint32_t b; } cvt;
    cvt.f = g_ui_transform_scale;
    uint32_t ts_bits = cvt.b;
    cvt.f = g_ui_transform_tx;
    uint32_t tx_bits = cvt.b;
    cvt.f = g_ui_transform_ty;
    uint32_t ty_bits = cvt.b;

    return pair_178_4c == g_frame_cache_7f0
           && (((uint64_t)ts_bits << 32) | scene_54) == g_frame_cache_7f8
           && (((uint64_t)ty_bits << 32) | tx_bits) == g_frame_cache_800
           && scene_ctx == g_frame_scene_ctx;
}

/* ===== pointer coordinates transform (game touch state -> UI space) ===== */

/* menu entry touch-region record: 0xd8-byte stride at 0x1c3568 */
typedef struct {
    uint64_t widget;     /* +0x00 */
    uint64_t _r08;       /* +0x08 */
    uint32_t kind;       /* +0x20 — 2 = touch region */
    uint32_t _r24;       /* +0x24 */
    uint32_t action_id;  /* +0x28 */
    uint64_t _r30;       /* +0x30 */
    float    cx;         /* +0x38 — region x offset from width/2 */
    float    cy;         /* +0x3c — region y offset from height/2 */
    uint32_t _r40;       /* +0x40 */
    float    width;      /* +0x44 — default 168.0 */
    float    height;     /* +0x48 — default 64.0 */
    uint8_t  visible;    /* +0x4c */
    uint8_t  _pad[0x87]; /* ..0xd7 */
} menu_entry_region_t;

extern const menu_entry_region_t g_menu_entry_regions[]; /* 0x1c3568 (same table as g_menu_entry_table) */

/*
 * ui_pointer_coords_transform — transform the game's mutex-protected touch
 * state into UI-space records and hit-test them against the menu entry
 * touch regions.
 *
 * which selects the game state slot pair (count at game+0x12fd3f8/3fc,
 * 200-byte record array at game+0x12fd400/4c8). The game mutex at
 * game+0x12fd3d0 must be lockable (EBUSY -> -1). Records (0x14 bytes:
 * id, {x,y} at +8, kind bytes at +0x10..+0x12) are validated, transformed
 * ((coord - center)/density - transform)/transform_scale and appended to
 * out as 10-word records {id, x, y, kind2, kind1, hit_action, hit_widget,
 * 0, frame_gen_b}; a hit requires the entry to be enabled (+0xd4), kind 2,
 * visible (+0x4c), with an action id, and (entry number >= 0x2c) the
 * position inside the region box (width/2±cx, height/2±cy, y offset by
 * the cached clip y for entries >= 0x2b). *out_count receives the count;
 * the out records are re-validated before returning 1. Returns 0 on data
 * failure, -1 on mutex contention. @ 0017975c
 */
int ui_pointer_coords_transform(int which, int *out, uint32_t *out_count)
{
    if (g_menu_open_state != 1)
        return 0;

    pthread_mutex_t *mutex = (pthread_mutex_t *)(g_game_base + 0x12fd3d0);
    int lk = pthread_mutex_trylock(mutex);
    if (lk != 0) {
        if (lk == 0x10) /* EBUSY */
            return -1;
        return 0;
    }

    uint64_t count = 0;
    uint8_t state[200];
    uint64_t frame_gen = g_frame_gen_b;

    uint64_t count_off = which != 0 ? 0x12fd3f8 : 0x12fd3fc;
    uint64_t recs_off = which != 0 ? 0x12fd400 : 0x12fd4c8;

    uint32_t n = 0xffffffffu;
    if ((g_game_base + count_off + 4) >> 2 > 0x400
        && g_host_read(g_host_ctx, g_game_base + count_off, &n, 4) == 1
        && (int32_t)n >= 0 && (int32_t)n < 0xb
        && (g_game_base + recs_off + 200) >> 3 > 0x218
        && g_host_read(g_host_ctx, g_game_base + recs_off, state, 200) == 1) {
        pthread_mutex_unlock(mutex);
        count = n;

        for (uint64_t r = 0; r < count; r++) {
            const uint8_t *rec = state + r * 0x14;
            frame_gen = g_frame_gen_b;
            if (g_ui_transform_scale <= 0.0f || !isfinite(g_ui_transform_scale)
                || g_ui_density <= 0.0f || !isfinite(g_ui_density))
                return 0;

            int32_t id = *(const int32_t *)rec;
            uint8_t k2 = rec[0x12];
            uint8_t k1 = rec[0x11];
            if (id < 0 || k2 > 1 || k1 > 1 || rec[0x10] > 1 || k2 == k1)
                return 0;

            /* two-lane int->float convert of the packed {x, y} at +8 */
            int32_t sx = *(const int32_t *)(rec + 8);
            int32_t sy = *(const int32_t *)(rec + 12);
            float x = (((float)sx - g_ui_center_x) / g_ui_density
                       - g_ui_transform_tx) / g_ui_transform_scale;
            float y = (((float)sy - g_ui_center_y) / g_ui_density
                       - g_ui_transform_ty) / g_ui_transform_scale;
            if (!isfinite(x) || fabsf(x) >= 100000.0f
                || !isfinite(y) || fabsf(y) >= 100000.0f)
                return 0;

            int *orec = out + r * 10;
            orec[3] = k2;
            orec[4] = k1;
            orec[0] = id;
            *(float *)&orec[1] = x;
            *(float *)&orec[2] = y;
            orec[5] = 0;
            orec[6] = 0;
            orec[7] = 0;
            *(uint64_t *)&orec[8] = frame_gen;

            /* hit-test against the menu entry touch regions, high to low */
            if (g_menu_entry_index != 0) {
                uint32_t entry = (uint32_t)g_menu_entry_index;
                const menu_entry_region_t *e =
                    &g_menu_entry_regions[entry - 1];
                uint32_t cur;
                do {
                    cur = entry - 1;
                    if (*(const int32_t *)((const uint8_t *)e + 0xd4) != 0
                        && e->kind == 2 && e->visible != 0
                        && e->action_id != 0
                        && (entry < 0x2c
                            || (y <= 472.0f && y >= 132.0f
                                && x <= 350.0f && x >= -530.0f))) {
                        float w = e->width;
                        if (w == 0.0f || w < 0.0f)
                            w = 168.0f;
                        float x_lo = w * 0.5f - e->cx;
                        float x_hi = w * 0.5f + e->cx;
                        int x_ok;
                        if (x >= x_lo) {
                            x_ok = !isnan(x) && !isnan(x_hi)
                                   && (x == x_hi || x <= x_hi);
                        } else {
                            x_ok = 0;
                        }
                        if (x_ok) {
                            float h = e->height;
                            if (h == 0.0f || h < 0.0f)
                                h = 64.0f;
                            float clip_off = g_clip_y_cached;
                            if (cur < 0x2b)
                                clip_off = 0.0f;
                            float y_lo = h * 0.5f - e->cy;
                            float y_hi = h * 0.5f + e->cy;
                            float ya = y + clip_off;
                            if (ya >= y_lo && ya <= y_hi) {
                                orec[5] = e->action_id;
                                *(uint64_t *)&orec[6] = e->widget;
                                break;
                            }
                        }
                    }
                    e--;
                    entry--;
                } while (cur != 0);
            }
        }

        *out_count = n;
        if (n < 0xb && (out != NULL || n == 0)) {
            if (n == 0)
                return 1;
            /* re-validate the transformed records */
            for (uint32_t r = 0; r < n; r++) {
                const int *orec = &out[r * 10];
                if (orec[0] < 0 || !isfinite((float)orec[1])
                    || isnan((float)orec[1]) || !isfinite((float)orec[2])
                    || isnan((float)orec[2]) || (uint32_t)orec[3] > 1
                    || (uint32_t)orec[4] > 1 || orec[3] == orec[4])
                    return 0;
                for (uint32_t p = 0; p < r; p++) {
                    if (out[p * 10] == orec[0])
                        return 0; /* duplicate id */
                }
            }
            return 1;
        }
        return 0;
    }

    pthread_mutex_unlock(mutex);
    return 0;
}
/* misc_b chain chunk 7: covers raw lines 13001-13500 */

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

/* ---- shared host/service globals (digest-mandated names) ---- */
extern void *g_host_ctx;                                      /* 0x1a7cc8 */
extern int (*g_host_read)(void *ctx, uint64_t addr, void *out,
                          uint32_t len);                      /* 0x1a7ce8 */
extern uint64_t g_game_base;                                  /* 0x1a7cd0 */
extern uint64_t g_ui_scene_root;                              /* 0x1a7d28 */
extern uint64_t g_engine_base;                                /* 0x281a48 */
extern int32_t g_menu_entry_index;                            /* 0x1a7d18 */

/* ---- misc_b-owned state ---- */
extern uint32_t g_clip_move_count;        /* 0x22d7c8 */
extern float    g_clip_y_cached;          /* 0x22d7e8 */
extern float    g_scroll_position;        /* 0x22d518 */
extern float    g_scroll_target;          /* 0x22d560 */
extern uint32_t g_pointer_active;         /* 0x22d564 */
extern float    g_diag_overlay_scale;     /* 0x22d818 — global UI overlay scale */

/* ---- rodata UV constants ---- */
extern const float g_uv_center[2];   /* 0x10f738 — {cx, cy} overlay center */
extern const float g_uv_size[2];     /* 0x10f818 — {w, h} overlay size */
extern const uint64_t g_pointer_park_guard;  /* 0x10fa90 */
extern const uint64_t g_pointer_park_hold;   /* 0x10fa98 */
extern const char FMT_1343dc[];      /* 0x1343dc — rich label style format (rodata) */

/* ---- engine/widget forwarders (chunks 1-2) ---- */
extern void widget_set_position(uint64_t widget, float x, float y);  /* @ 0016cf5c */
extern void widget_set_scale(uint64_t widget, float sx, float sy);   /* @ 0016cf00 */
extern void widget_set_anchor(uint64_t widget, void *anchor_out);    /* @ 0016cf84 */
extern void engine_call_66ae58(uint64_t out, const char *text);  /* @ 0016ced8 (veneer (a,b)) */
extern void engine_call_5921a0(uint64_t a, uint64_t b);              /* @ 0016ceac (veneer (a,b)) */
extern void engine_call_592250(uint64_t a, uint64_t b);              /* @ 0016ceec (veneer (a,b)) */

/* ---- own later chunks (b-chain) ---- */
extern int is_plausible_ptr_mb(uint64_t v);                   /* misc.c (b-chain) */
extern int widget_child_link_valid(uint64_t node,
                                   uint64_t parent);          /* misc.c (b-chain) @ 0016d5cc */
extern void inertia_state_step(float x, float y, float *state,
                               uint64_t frame, int settle,
                               uint32_t mode, float id,
                               float gen);                     /* misc.c (b-chain) @ 00183230 */
extern int widget_clip_move(float x, float y, void *ops, uint64_t root,
                            uint32_t entry_index,
                            uint32_t flag);                   /* misc.c (b-chain) @ 0017fc3c */
extern int widget_rich_panel_bound_check(uint64_t widget);    /* misc.c (b-chain) @ 0017c598 */

/* ---- clip ops channel (typedef in chunk 3) ---- */
extern int host_read_gate(uint64_t ctx, uint64_t addr, void *out,
                          uint64_t len);                      /* misc.c (b-chain) @ 0017fe9c */
extern int host_write_gate(uint64_t ctx, void *dst, const void *src,
                           size_t len);                       /* misc.c (b-chain) @ 0017fee0 */
extern int clip_ops_integrity_check(uint64_t ctx);            /* misc.c (b-chain) @ 001802bc */
extern void engine_call_11a2840_adapter(uint64_t ctx,
                                        uint64_t a);          /* misc.c (b-chain) @ 00180540 */
extern void engine_call_88b344_adapter(uint64_t ctx, uint64_t a,
                                       uint32_t b);           /* misc.c (b-chain) @ 00180548 */
extern void engine_call_593f14_adapter(uint64_t ctx, uint64_t a,
                                       uint32_t b);           /* misc.c (b-chain) @ 00180564 */
extern void engine_call_594130_adapter(uint64_t ctx, uint64_t a,
                                       uint64_t b);           /* misc.c (b-chain) @ 00180580 */
extern void engine_call_59567c_adapter(uint64_t ctx,
                                       uint64_t a);           /* misc.c (b-chain) @ 0018058c */
extern void engine_call_5940d0_gated(uint64_t ctx,
                                     uint64_t a);             /* misc.c (b-chain) @ 001805a4 */
extern void engine_call_59531c_adapter(int x, int y, uint64_t ctx,
                                       uint64_t clip);         /* misc.c (b-chain) @ 00180724 */
extern void engine_call_88d290_adapter(uint64_t ctx, uint64_t a,
                                       uint32_t b);           /* misc.c (b-chain) @ 0018073c */
extern void engine_call_594e80_adapter(uint64_t ctx, uint64_t a,
                                       uint32_t b);           /* misc.c (b-chain) @ 00180758 */
extern int menu_clip_writable_check(uint64_t ctx, void *addr,
                                    size_t len);   /* menu_engine.c @ 00180034 */
extern void menu_probe_log(uint64_t ctx,
                           const char *event);     /* menu_engine.c @ 0017fbc8 */

/* ---- other files / a-chain ---- */
extern void game_call_66ad48(void *arg);  /* misc.c (a-chain) @ 0014ec30 — engine base + 0x66ad48 */

/* ---- rich panel cached widget slots (renderer-owned; layout per chunk 4) ---- */
extern uint64_t g_rich_panel_ctx_slot;     /* 0x283348 — kind-0 slot */
extern uint64_t g_rich_panel_k2_slots[5];  /* 0x283350 — kind-2 widget slots */

/*
 * g_string_table — the 512-entry interned string table at 0x1e54c8
 * (0x90-byte stride: 0x80 text + the engine string object).
 */
typedef struct {
    char     text[0x80];  /* +0x00 */
    uint64_t object;      /* +0x80 — engine string object handle */
} string_table_entry_t;

string_table_entry_t g_string_table[0x800];  /* 0x1e54c8 — interned string table */
uint32_t g_string_count;                     /* 0x1a7d1c — interned string count */

/* ===== clip y snap ===== */

/*
 * ui_clip_y_snap — snap the clip y to the scroll fraction: compute
 * frac = -132 - trunc(-132 - scroll) and, when it differs from the cached
 * value, move the clip to (536, -132-frac) through a fresh ops channel
 * (entry index folded > 0x2a via -0x2b, mode 1). On success the clip move
 * counter and the y cache are updated. Returns 1 on success or cache hit.
 * @ 00179c50
 */
int ui_clip_y_snap(float scroll)
{
    float frac = -132.0f - (float)(int)(-132.0f - scroll);
    if (frac == g_clip_y_cached)
        return 1;

    clip_ops_t ops;
    memset(&ops, 0, sizeof ops);
    ops.game_base = g_game_base;
    ops.read_gate = host_read_gate;
    ops.write_gate = host_write_gate;
    ops.writable_check = menu_clip_writable_check;
    ops.integrity_check = clip_ops_integrity_check;
    ops.call_11a2840 = engine_call_11a2840_adapter;
    ops.call_88b344 = engine_call_88b344_adapter;
    ops.call_593f14 = engine_call_593f14_adapter;
    ops.call_594130 = engine_call_594130_adapter;
    ops.call_59567c = engine_call_59567c_adapter;
    ops.call_5940d0 = engine_call_5940d0_gated;
    ops.call_59531c = engine_call_59531c_adapter;
    ops.call_88d290 = engine_call_88d290_adapter;
    ops.call_594e80 = engine_call_594e80_adapter;
    ops.probe_log = menu_probe_log;

    uint32_t entry = 0;
    if (g_menu_entry_index > 0x2a)
        entry = (uint32_t)(g_menu_entry_index - 0x2b);

    if (widget_clip_move(536.0f /* 0x44048000 */, -132.0f - frac, &ops,
                         g_ui_scene_root, entry, 1) == 1) {
        g_clip_move_count++;
        g_clip_y_cached = frac;
        return 1;
    }
    return 0;
}

/* ===== touch state reset ===== */

/*
 * ui_touch_state_reset — reset the touch state block at touch_state
 * (base 0x22d560): clear the pointer-active word (+4), park the guard
 * pair (+0x10 = 0x10fa90) and the hold pair (+0x18 = 0x10fa98), and
 * hard-reset the scroll inertia state (mode 4). @ 00179dc0
 */
void ui_touch_state_reset(void *touch_state, float *scroll_state,
                          uint64_t frame, uint32_t flag)
{
    *(uint64_t *)((uint8_t *)touch_state + 0x18) = g_pointer_park_hold;
    *(uint64_t *)((uint8_t *)touch_state + 0x10) = g_pointer_park_guard;
    *(uint32_t *)((uint8_t *)touch_state + 0x04) = 0;
    inertia_state_step(0, 0, scroll_state, frame, 0, 4, -1.0f, flag);
}

/* ===== edit-controls binding ===== */

/*
 * widget_edit_controls_bound_check — the widget's scene owner (+0x30) is
 * NULL or the scene root, and the widget is bound under the edit-controls
 * ctx (0x282158) within 16 parent levels, every link validated.
 * Returns 1 when bound. @ 00179dfc
 */
int widget_edit_controls_bound_check(uint64_t widget)
{
    if (widget == 0)
        return 0;

    uint64_t owner = 0;
    if (!is_plausible_ptr_mb(widget + 0x38)
        || g_host_read(g_host_ctx, widget + 0x30, &owner, 8) != 1
        || (owner != 0 && owner != g_ui_scene_root))
        return 0;

    uint64_t cur = widget;
    int ok = 1;
    for (uint32_t depth = 0; ; depth++) {
        ok = 1;
        if (cur == g_edit_controls_ctx || depth > 0xf)
            break;

        uint64_t parent_raw = 0;
        int rd = is_plausible_ptr_mb(cur + 0x40)
                 && g_host_read(g_host_ctx, cur + 0x38, &parent_raw, 8) == 1;
        int parent_ok = rd && is_plausible_ptr_mb(parent_raw)
                        && (parent_raw & 7) == 0;
        uint64_t parent = parent_ok ? parent_raw : 0;

        if (widget_child_link_valid(cur, parent) != 0) {
            cur = parent_raw;
            if (parent_ok)
                continue;
        }
        cur = 0; /* broken link: the walker falls off (raw clears it) */
        break;
    }
    return cur == g_edit_controls_ctx ? ok : 0;
}

/* ===== cached widget hiding ===== */

/*
 * ui_cached_widgets_hide — park the 17 cached rich-panel widgets
 * offscreen at (-10000, -10000): kind-1 rows 1-5 (row 0 is NOT hidden),
 * the panel ctx slot, the five kind-2 slots and the six kind-0 rows.
 * @ 0017b3f0
 */
void ui_cached_widgets_hide(void)
{
    widget_set_position(g_rich_panel_rows_a[1].widget, -10000.0f, -10000.0f);
    widget_set_position(g_rich_panel_rows_a[2].widget, -10000.0f, -10000.0f);
    widget_set_position(g_rich_panel_rows_a[3].widget, -10000.0f, -10000.0f);
    widget_set_position(g_rich_panel_rows_a[4].widget, -10000.0f, -10000.0f);
    widget_set_position(g_rich_panel_rows_a[5].widget, -10000.0f, -10000.0f);
    widget_set_position(g_rich_panel_ctx_slot, -10000.0f, -10000.0f);
    widget_set_position(g_rich_panel_k2_slots[0], -10000.0f, -10000.0f);
    widget_set_position(g_rich_panel_k2_slots[1], -10000.0f, -10000.0f);
    widget_set_position(g_rich_panel_k2_slots[2], -10000.0f, -10000.0f);
    widget_set_position(g_rich_panel_k2_slots[3], -10000.0f, -10000.0f);
    widget_set_position(g_rich_panel_k2_slots[4], -10000.0f, -10000.0f);
    widget_set_position(g_rich_panel_rows_b[0].widget, -10000.0f, -10000.0f);
    widget_set_position(g_rich_panel_rows_b[1].widget, -10000.0f, -10000.0f);
    widget_set_position(g_rich_panel_rows_b[2].widget, -10000.0f, -10000.0f);
    widget_set_position(g_rich_panel_rows_b[3].widget, -10000.0f, -10000.0f);
    widget_set_position(g_rich_panel_rows_b[4].widget, -10000.0f, -10000.0f);
    widget_set_position(g_rich_panel_rows_b[5].widget, -10000.0f, -10000.0f);
}

/* ===== UV rect + widget frame ===== */

/*
 * ui_uv_rect_compute — compute the normalized UV rect for a (w, h) box
 * under the global overlay scale: out = { (w*-0.5 + cx*s)/area,
 * (h*-0.5 + cy*s)/area, w_uv*s/area, h_uv*s/area } where s is the overlay
 * scale, (cx, cy)/(w_uv, h_uv) the rodata overlay center/size and area =
 * mul_a * mul_b. Returns 1 on success, 0 when a gate fails (non-positive
 * or non-finite inputs). @ 0017b530
 */
int ui_uv_rect_compute(float w, float h, float mul_a, float mul_b,
                       float out[4])
{
    if (!isfinite(w) || isnan(w) || out == NULL || !(h > 0.0f)
        || !(w > 0.0f) || !isfinite(h) || isnan(h))
        return 0;

    float area = mul_a * mul_b;
    if (!(area > 0.0f) || !isfinite(area) || isnan(area))
        return 0;

    float s = g_diag_overlay_scale;
    out[0] = (w * -0.5f + g_uv_center[0] * s) / area;
    out[1] = (h * -0.5f + g_uv_center[1] * s) / area;
    out[2] = (g_uv_size[0] * s) / area;
    out[3] = (g_uv_size[1] * s) / area;
    return 1;
}

/*
 * widget_set_frame — set a widget's frame (x, y, w, h) with readback
 * validation: the scale matrix at widget+0x10 must read {sx, 0, 0, sy}
 * with sx, sy in (0, 128]; the widget is positioned at (0,0) and its
 * current anchor/extent read back; the new scale (sx*w/(ex-ax),
 * sy*h/(ey-ay)) must land in [0.0001, 128]; the widget is then scaled and
 * positioned so its anchor-rect midpoint lands at (x, y) offset. Returns
 * 1 on success, 0 on any gate failure. @ 0017b5e8
 */
int widget_set_frame(uint64_t widget, float x, float y, float w, float h)
{
    float m[4]; /* scale matrix {sx, 0, 0, sy}, then anchor/extent scratch */

    if (!is_plausible_ptr_mb(widget + 0x20)
        || g_host_read(g_host_ctx, widget + 0x10, m, 0x10) != 1
        || !isfinite(m[0]) || !isfinite(m[1]) || !isfinite(m[2])
        || !isfinite(m[3]) || isnan(m[0]) || isnan(m[1]) || isnan(m[2])
        || isnan(m[3]) || m[1] != 0.0f || m[2] != 0.0f || !(m[0] > 0.0f)
        || !(m[3] > 0.0f) || m[0] > 128.0f || m[3] > 128.0f)
        return 0;

    float sx = m[0];
    float sy = m[3];

    widget_set_position(widget, 0.0f, 0.0f);
    widget_set_anchor(widget, m); /* reads the anchor/extent {ax, ay, ex, ey} */

    if (!isfinite(m[2] - m[0]) || isnan(m[2] - m[0]))
        return 0;

    m[3] = m[3] - m[1]; /* ey - ay */
    if (isfinite(m[3]) && !isnan(m[3]) && (m[2] - m[0]) > 0.0f
        && m[3] > 0.0f) {
        float scale_x = (sx * w) / (m[2] - m[0]);
        if (isfinite(scale_x) && !isnan(scale_x)) {
            float scale_y = (sy * h) / m[3];
            if (isfinite(scale_y) && !isnan(scale_y)
                && scale_x >= 0.0001f && scale_y >= 0.0001f
                && scale_x <= 128.0f && scale_y <= 128.0f) {
                widget_set_scale(widget, scale_x, scale_y);

                float a2[4];
                widget_set_anchor(widget, a2); /* re-read anchor/extent */
                widget_set_position(widget,
                                    (a2[0] + a2[2]) * 0.5f - x,
                                    (a2[1] + a2[3]) * 0.5f - y);
                return 1;
            }
        }
    }
    return 0;
}

/* ===== engine string interning ===== */

/*
 * ui_string_object — intern text into the 512x0x90 engine string table:
 * texts shorter than 0x80 bytes are looked up by strcmp and the existing
 * engine string object returned; new texts are appended (count capped at
 * 0x800) and the object built via engine_call_66ae58. Returns the engine
 * string object handle, or 0 when the text is too long or the table is
 * full. @ 0017b858
 */
uint64_t ui_string_object(const char *text)
{
    size_t len = strlen(text);
    uint32_t count = g_string_count;

    if (len >= 0x80)
        return 0;

    if (count != 0) {
        for (uint32_t i = 0; i < count; i++) {
            if (strcmp(g_string_table[i].text, text) == 0)
                return (uint64_t)&g_string_table[i].object;
        }
        if (count == 0x800)
            return 0;
    }

    memcpy(g_string_table[count].text, text, len + 1);
    g_string_count = count + 1;
    engine_call_66ae58((uint64_t)&g_string_table[count].object,
                       g_string_table[count].text);
    return (uint64_t)&g_string_table[count].object;
}

/* ===== rich panel label rendering ===== */

/*
 * rich_panel_label_render — render a text label on the rich panel ctx:
 * record holds {panel ctx at +0x08, widget at +0x10, 0x60 text cache at
 * +0x18}. The panel ctx must pass widget_rich_panel_bound_check and the
 * widget must be bound under it (<= 16 levels). When the text differs
 * from the cached copy a fresh engine string object is built
 * (engine_call_66ae58), set on the widget (engine_call_5921a0), released
 * (game_call_66ad48), copied into the cache through ui_text_style_format
 * (fmt 0x1343dc, cap 0x60) and the widget invalidated
 * (engine_call_592250 with -1). The label font is then divided by the
 * widget child count at +0xb0 (1..0x100), the widget rescaled, parked at
 * (0,0), its anchor read back, positioned at (x - ax, y - ay) and the
 * panel ctx parked at (0,0). @ 0017b920
 */
void rich_panel_label_render(float x, float y, float font, void *record,
                             const char *text)
{
    uint64_t panel_ctx = *(uint64_t *)((uint8_t *)record + 0x08);

    if (widget_rich_panel_bound_check(panel_ctx) == 0)
        return;

    uint64_t target = panel_ctx;
    uint64_t cur = *(uint64_t *)((uint8_t *)record + 0x10);
    if (cur != 0) {
        uint32_t depth = 0;
        for (;;) {
            if (cur == target || depth > 0xf)
                break;
            uint64_t parent_raw = 0;
            int rd = is_plausible_ptr_mb(cur + 0x40)
                     && g_host_read(g_host_ctx, cur + 0x38, &parent_raw, 8) == 1;
            int parent_ok = rd && is_plausible_ptr_mb(parent_raw)
                            && (parent_raw & 7) == 0;
            uint64_t parent = parent_ok ? parent_raw : 0;
            if (widget_child_link_valid(cur, parent) == 0)
                return;
            depth++;
            cur = parent_raw;
            if (!parent_ok) {
                cur = 0;
                break;
            }
        }
        if (cur == target) {
            uint64_t widget = *(uint64_t *)((uint8_t *)record + 0x10);
            char *cache = (char *)record + 0x18;

            if (strcmp(text, cache) != 0) {
                uint64_t str_obj = 0;
                engine_call_66ae58((uint64_t)&str_obj, text);
                engine_call_5921a0(widget, (uint64_t)&str_obj);
                game_call_66ad48(&str_obj);
                ui_text_style_format(cache, (size_t)-1, 0x60, FMT_1343dc,
                                     text);
                engine_call_592250(widget, 0xffffffffu);
            }

            uint16_t children = 0;
            if (is_plausible_ptr_mb(widget + 0xb2)
                && g_host_read(g_host_ctx, widget + 0xb0, &children, 2) == 1
                && 0 < children && children < 0x101) {
                font = font / (float)(int)children;
                widget_set_scale(widget, font, font);
                widget_set_position(widget, 0.0f, 0.0f);
                float anchor[4];
                widget_set_anchor(widget, anchor);
                if (isfinite(anchor[0]) && !isnan(anchor[0])
                    && isfinite(anchor[1]) && !isnan(anchor[1]))
                    widget_set_position(widget, x - anchor[0],
                                        y - anchor[1]);
                widget_set_position(panel_ctx, 0.0f, 0.0f);
            }
        }
    }
}
/* misc_b chain chunk 8: covers raw lines 13514-14000 */

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

/* ---- shared host/service globals (digest-mandated names) ---- */
extern void *g_host_ctx;                                      /* 0x1a7cc8 */
extern void (*g_host_log)(void *ctx, const char *cat, const char *event,
                          uint32_t value);                    /* 0x1a7d00 */
extern int (*g_host_read)(void *ctx, uint64_t addr, void *out,
                          uint32_t len);                      /* 0x1a7ce8 */
extern uint64_t g_game_base;                                  /* 0x1a7cd0 */
extern uint64_t g_ui_scene_root;                              /* 0x1a7d28 */
extern uint64_t g_ui_context;                                 /* 0x1a7d30 */
extern int32_t g_menu_entry_index;                            /* 0x1a7d18 */

/* ---- own earlier chunks (b-chain) ---- */
extern int is_plausible_ptr_mb(uint64_t v);                   /* misc.c (b-chain) */
extern int widget_child_link_valid(uint64_t node,
                                   uint64_t parent);          /* misc.c (b-chain) @ 0016d5cc */
extern int engine_code_snapshot_verify(void);                 /* misc.c (b-chain) @ 00172da8 */
extern int widget_scene_owner_check(uint64_t widget);         /* misc.c (b-chain) @ 0016df78 */
extern uint64_t widget_field_ptr_read(uint64_t addr);          /* misc.c (b-chain) @ 0016f79c */

/* ---- own later chunks (b-chain) ---- */
extern int widget_pair_snapshot_read(const uint64_t *pair, int index,
                                     uint64_t *out);          /* misc.c (b-chain) @ 0017c704 (below) */
extern int widget_rich_panel_bound_check(uint64_t widget);    /* misc.c (b-chain) @ 0017c598 (below) */

/* ---- other files / a-chain ---- */
extern int code_region_hash_verify(uint64_t rva, uint64_t len,
                                   const char *hex_digest);   /* misc.c (a-chain) @ 0014d168 */

/* ---- rich panel cached widget slots (renderer-owned; layout per chunk 4) ---- */
extern uint64_t g_rich_panel_k2_slots[5];  /* 0x283350 — kind-2 widget slots */
extern uint64_t g_rich_panel_ctx;          /* 0x283340 — rich panel ctx */

/* ---- rodata ---- */
extern const int32_t g_panel_slot_x[];   /* 0x13ce70 — expected x at panel slot +0xec, per index */
extern const int32_t g_panel_slot_y[];   /* 0x13ce84 — expected y at panel slot +0xf0, per index */

/* misc_b-owned camera-guard cache */
int32_t  g_camera_guards_state;   /* 0x2839f8 — script_port_camera guard bitmask result (1 ok / -1 fail) */
uint32_t g_camera_guards_count;   /* 0x2839f4 — guard re-verify counter, logged while < 0x30 */

/* ===== rich panel slot + guard verification ===== */

/*
 * rich_panel_widget_slot_check — validate the indexed rich panel widget
 * slot (kind-2 slots at 0x283350): the widget must have vtable
 * game+0x11c0c58, be bound under the rich panel ctx
 * (widget_rich_panel_bound_check), and the geometry pair at +0xec must
 * equal {g_panel_slot_x[index], g_panel_slot_y[index]}; the object at
 * +0x128 must be bound too. Returns 1 when the slot is valid.
 * @ 0017bb9c
 */
int rich_panel_widget_slot_check(uint32_t index)
{
    uint64_t widget = g_rich_panel_k2_slots[index];

    uint64_t vtable = 0;
    if (!is_plausible_ptr_mb(widget + 8)
        || g_host_read(g_host_ctx, widget, &vtable, 8) != 1
        || !is_plausible_ptr_mb(vtable) || (vtable & 7) != 0
        || vtable != g_game_base + 0x11c0c58)
        return 0;
    if (widget_rich_panel_bound_check(widget) == 0)
        return 0;

    if (!is_plausible_ptr_mb(widget + 0xf4))
        return 0;
    int32_t geom[2] = {0, 0};
    if (g_host_read(g_host_ctx, widget + 0xec, geom, 8) != 1
        || geom[0] != g_panel_slot_x[index] || geom[1] != g_panel_slot_y[index])
        return 0;

    if (!is_plausible_ptr_mb(widget + 0x130))
        return 0;
    uint64_t inner = 0;
    if (g_host_read(g_host_ctx, widget + 0x128, &inner, 8) != 1
        || !is_plausible_ptr_mb(inner) || (inner & 7) != 0)
        inner = 0;
    return widget_rich_panel_bound_check(inner) != 0;
}

/*
 * rich_panel_region_mask_cache — compute and cache the panel region
 * verify bitmask (only while the cached state at 0x2839f8 is 0): the
 * engine snapshot check contributes bit 1, the SHA-256 checks of engine
 * regions 0x889288/0x889940/0x5d8874/0x5d76d4 contribute bits 2/4/8/0x10,
 * and the vtable slot at game+0x11c0e00 pointing at game+0x889940 adds
 * bit 0x20. When all six bits pass (0x3f) the cached state becomes 1,
 * otherwise -1; the result is logged (up to 0x30 times) as
 * "guards_verified"/"guards_failed" under "script_port_camera". Returns
 * the cached state == 1. @ 0017bd68
 */
int rich_panel_region_mask_cache(void)
{
    if (g_camera_guards_state == 0) {
        uint64_t slot_val = 0;
        uint8_t mask = engine_code_snapshot_verify() != 0 ? 1 : 0;

        if (code_region_hash_verify(0x889288, 0x2a4,
            "44f217dd7b9d710fc5d55e552c247906a1dce667d803e2ce688316a5e39d0572") != 0)
            mask |= 2;
        if (code_region_hash_verify(0x889940, 0x13c,
            "ddcaaa86a0d7437ea77f4cc53f14de1d77459f0348559e71c5edeea917a327e7") != 0)
            mask |= 4;
        if (code_region_hash_verify(0x5d8874, 0xb0,
            "77399cc53dfa8af7bca45327002bfaaab847dbaea6a1eeba62dec6aaeecc46a4") != 0)
            mask |= 8;
        if (code_region_hash_verify(0x5d76d4, 0x94,
            "4fe60124ce3d6bb318f6b2293544f34158857b6337d2d5dfa8b05e2c3483a92a") != 0)
            mask |= 0x10;

        uint8_t mask_full = mask | 0x20;
        if (is_plausible_ptr_mb(g_game_base + 0x11c0e00)
            && g_host_read(g_host_ctx, g_game_base + 0x11c0e00, &slot_val, 8) == 1
            && slot_val != g_game_base + 0x889940)
            mask_full = mask; /* vtable slot mismatch drops bit 0x20 */

        g_camera_guards_state = mask_full == 0x3f ? 1 : -1;

        uint32_t count = g_camera_guards_count + 1;
        int do_log = g_camera_guards_count < 0x30;
        g_camera_guards_count = count;
        if (do_log && g_host_log != NULL)
            g_host_log(g_host_ctx, "script_port_camera",
                       mask_full == 0x3f ? "guards_verified" : "guards_failed",
                       mask_full);
    }
    return g_camera_guards_state == 1;
}

/* ===== engine view object ===== */

/*
 * engine_view_object_get — call the engine view getter (game+0x5d8874)
 * and validate the returned object: vtable game+0x11ad208 and the object
 * reaches target within 16 validated parent levels. Returns the view
 * object, or 0 when validation fails. @ 0017c284
 */
uint64_t engine_view_object_get(uint64_t target)
{
    uint64_t view = ((uint64_t (*)(void))(g_game_base + 0x5d8874))();
    if (view == 0)
        return 0;

    uint64_t vtable = 0;
    if (!is_plausible_ptr_mb(view + 8)
        || g_host_read(g_host_ctx, view, &vtable, 8) != 1
        || !is_plausible_ptr_mb(vtable) || (vtable & 7) != 0
        || vtable != g_game_base + 0x11ad208)
        return 0;

    uint64_t cur = view;
    int ok = 1;
    for (uint32_t depth = 0; ; depth++) {
        ok = 1;
        if (cur == target || depth > 0xf)
            break;

        uint64_t parent_raw = 0;
        int rd = is_plausible_ptr_mb(cur + 0x40)
                 && g_host_read(g_host_ctx, cur + 0x38, &parent_raw, 8) == 1;
        int parent_ok = rd && is_plausible_ptr_mb(parent_raw)
                        && (parent_raw & 7) == 0;
        uint64_t parent = parent_ok ? parent_raw : 0;

        if (widget_child_link_valid(cur, parent) != 0) {
            cur = parent_raw;
            if (parent_ok)
                continue;
        }
        cur = 0; /* broken link: the walker falls off (raw clears it) */
        break;
    }
    return ok && cur == target ? view : 0;
}

/*
 * widget_rich_panel_bound_check — the widget's scene owner (+0x30) is
 * NULL or the scene root, and the widget is bound under the rich panel
 * ctx (0x283340) within 16 parent levels, every link validated.
 * Returns 1 when bound. @ 0017c598
 */
int widget_rich_panel_bound_check(uint64_t widget)
{
    if (widget == 0)
        return 0;

    uint64_t owner = 0;
    if (!is_plausible_ptr_mb(widget + 0x38)
        || g_host_read(g_host_ctx, widget + 0x30, &owner, 8) != 1
        || (owner != 0 && owner != g_ui_scene_root))
        return 0;

    uint64_t cur = widget;
    int ok = 1;
    for (uint32_t depth = 0; ; depth++) {
        ok = 1;
        if (cur == g_rich_panel_ctx || depth > 0xf)
            break;

        uint64_t parent_raw = 0;
        int rd = is_plausible_ptr_mb(cur + 0x40)
                 && g_host_read(g_host_ctx, cur + 0x38, &parent_raw, 8) == 1;
        int parent_ok = rd && is_plausible_ptr_mb(parent_raw)
                        && (parent_raw & 7) == 0;
        uint64_t parent = parent_ok ? parent_raw : 0;

        if (widget_child_link_valid(cur, parent) != 0) {
            cur = parent_raw;
            if (parent_ok)
                continue;
        }
        cur = 0; /* broken link: the walker falls off (raw clears it) */
        break;
    }
    return cur == g_rich_panel_ctx ? ok : 0;
}

/* ===== widget pair snapshot ===== */

/*
 * widget_pair_snapshot_read — read the {node, parent} pair (node at +0,
 * parent at +8) into a 0x50-byte snapshot (10 qwords): fields at +0x10
 * (node+0x80), +0x18 (node+0x88 children), +0x20 (node+0xb0 class
 * parent), +0x28 (parent+0x38), +0x2c index (parent+0x40), +0x30
 * (children+0x38), +0x38 child count (children+0xc0) and +0x3a grandchild
 * count (children+0x4e), each gated and flagged in the +0x44 bitmask.
 * When the first seven flags pass and node is kind-1 (vtable
 * game+0x11c0a48) with parent kind-2 (vtable game+0x11ad208) and the
 * class-parent identities line up, the kind word at +0x4c is computed:
 * 1 = node's parent is the pair parent; 2 = parent's children array
 * contains node exactly once; 3 = node also absent from parent's
 * grandchild array (children+0x50). Returns 1 when a kind was computed,
 * 0 on any read/gate failure. @ 0017c704
 */
int widget_pair_snapshot_read(const uint64_t *pair, int index,
                              uint64_t *out)
{
    uint64_t node = pair[0];
    uint64_t parent = pair[1];

    memset(out, 0, 0x50);
    out[0] = node;
    out[1] = parent;
    out[7] = 0xfffffffefffffffeULL; /* index/index prefill */

    uint32_t flags = 0;
    if (is_plausible_ptr_mb(node + 0x80)
        && g_host_read(g_host_ctx, node + 0x80, &out[2], 8) == 1)
        flags |= 1;
    if (is_plausible_ptr_mb(node + 0x88)
        && g_host_read(g_host_ctx, node + 0x88, &out[3], 8) == 1)
        flags |= 2;
    if (is_plausible_ptr_mb(node + 0xb0)
        && g_host_read(g_host_ctx, node + 0xb0, &out[4], 8) == 1)
        flags |= 4;
    if (is_plausible_ptr_mb(parent + 0x38)
        && g_host_read(g_host_ctx, parent + 0x38, &out[5], 8) == 1)
        flags |= 8;
    if (is_plausible_ptr_mb(parent + 0x44)
        && g_host_read(g_host_ctx, parent + 0x40, &out[7], 4) == 1)
        flags |= 0x10;
    uint64_t children = out[3];
    if (is_plausible_ptr_mb(children + 0x38)
        && g_host_read(g_host_ctx, children + 0x38, &out[6], 8) == 1)
        flags |= 0x20;
    if (is_plausible_ptr_mb(children + 0x44)
        && g_host_read(g_host_ctx, children + 0x40, (uint8_t *)out + 0x3c, 4) == 1)
        flags |= 0x40;
    if (is_plausible_ptr_mb(children + 0xc0)
        && g_host_read(g_host_ctx, children + 0xc0, &out[8], 2) == 1)
        flags |= 0x80;
    uint16_t grandchildren = 0;
    if (is_plausible_ptr_mb(children + 0x4e)
        && g_host_read(g_host_ctx, children + 0x4e, &grandchildren, 2) == 1) {
        *(uint16_t *)((uint8_t *)out + 0x42) = grandchildren;
        flags |= 0x100;
    }
    *(uint32_t *)((uint8_t *)out + 0x44) = flags;

    if ((flags ^ 0xffffffffu) & 0x7f)
        return 0; /* the first seven reads are mandatory */

    int result = 0;

    uint64_t node_vtable = 0;
    if (!is_plausible_ptr_mb(node + 8)
        || g_host_read(g_host_ctx, node, &node_vtable, 8) != 1
        || !is_plausible_ptr_mb(node_vtable) || (node_vtable & 7) != 0
        || node_vtable != g_game_base + 0x11c0a48)
        return 0;

    uint64_t parent_vtable = 0;
    if (!is_plausible_ptr_mb(parent + 8)
        || g_host_read(g_host_ctx, parent, &parent_vtable, 8) != 1
        || !is_plausible_ptr_mb(parent_vtable) || (parent_vtable & 7) != 0)
        parent_vtable = 0;
    if (parent_vtable != g_game_base + 0x11ad208)
        return 0;
    if (out[2] != parent || out[4] != out[2]) /* node class parent == pair parent */
        return 0;

    uint64_t children_vtable = 0;
    if (!is_plausible_ptr_mb(children + 8)
        || g_host_read(g_host_ctx, children, &children_vtable, 8) != 1
        || !is_plausible_ptr_mb(children_vtable) || (children_vtable & 7) == 0
        || children_vtable != g_game_base + 0x11ad208)
        return 0;

    if (!is_plausible_ptr_mb(parent + 0x38)
        || g_host_read(g_host_ctx, parent + 0x30, &out[5], 8) != 1
        || (out[5] != 0 && out[5] != g_ui_scene_root))
        return 0;

    if (widget_scene_owner_check(children) == 0)
        return 0;
    if (widget_child_link_valid(children, node) == 0)
        return 0;

    uint64_t up = children; /* the pair's "node up" walker */
    if (index == 0 || up == pair[3]) {
        if (up == parent) {
            *(uint32_t *)((uint8_t *)out + 0x4c) = 1;
            return 1;
        }
        if ((flags ^ 0xffffffffu) & 0x1ff)
            return 0; /* the full nine reads are needed for kinds 2/3 */

        int16_t child_count = (int16_t)(out[8] & 0xffff);
        if (child_count < 0x201 && child_count > 0x200)
            return 0;
        if ((int16_t)grandchildren > 0x200)
            return 0;

        uint16_t frames = 0;
        if (!is_plausible_ptr_mb(up + 0xc0)
            || g_host_read(g_host_ctx, up + 0xbe, &frames, 2) != 1
            || frames < 1)
            return 0;

        uint64_t child_children = widget_field_ptr_read(up + 0x90);
        if (child_children == 0)
            return 0;

        /* the node must appear exactly once in the up-walker's children */
        if (child_count >= 1) {
            uint64_t hits = 0;
            uint64_t slot = child_children;
            for (int16_t i = 0; i < child_count; i++) {
                uint64_t entry = 0;
                if (!is_plausible_ptr_mb(slot + 8)
                    || g_host_read(g_host_ctx, slot, &entry, 8) != 1)
                    return 0;
                slot += 8;
                if (entry == parent)
                    hits++;
            }
            *(int *)&out[9] = (int)hits;
            if (hits != 1)
                return 0;
        }

        if (widget_child_link_valid(parent, up) != 0) {
            /* kind 2: link verified through the parent */
            *(uint32_t *)((uint8_t *)out + 0x4c) = 2;
            return 1;
        }

        /* kind 3: additionally the parent must be absent from the
           grandchild array (children+0x50) */
        if (out[5] == 0 && (int32_t)out[7] == -1) {
            uint64_t gchildren = widget_field_ptr_read(children + 0x50);
            if (grandchildren == 0 || gchildren != 0) {
                if (grandchildren != 0) {
                    uint64_t slot = gchildren;
                    for (uint16_t i = 0; i < grandchildren; i++) {
                        uint64_t entry = 0;
                        if (!is_plausible_ptr_mb(slot + 8)
                            || g_host_read(g_host_ctx, slot, &entry, 8) != 1
                            || entry == parent)
                            return 0;
                        slot += 8;
                    }
                }
                *(uint32_t *)((uint8_t *)out + 0x4c) = 3;
                return 1;
            }
        }
        return 0;
    }
    return result;
}
/* misc_b chain chunk 9: covers raw lines 14001-14500 */

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

/* ---- shared host/service globals (digest-mandated names) ---- */
extern void *g_host_ctx;                                      /* 0x1a7cc8 */
extern int (*g_host_read)(void *ctx, uint64_t addr, void *out,
                          uint32_t len);                      /* 0x1a7ce8 */
extern uint64_t g_game_base;                                  /* 0x1a7cd0 */
extern uint64_t g_ui_scene_root;                              /* 0x1a7d28 */
extern uint64_t g_ui_context;                                 /* 0x1a7d30 */
extern uint64_t g_engine_base;                                /* 0x281a48 */

/* ---- panel pair / edit controls / page ctx (registry-named, multi-part) ---- */
extern uint64_t g_panel_widget;         /* 0x284220 */
extern uint64_t g_page_list_widget;     /* 0x2842f8 */
extern uint64_t g_page_context_widget;  /* 0x2843d0 */
extern uint64_t g_page_context_flag;    /* 0x2843d8 — page ctx flag byte */
extern uint64_t g_page_state_flag;      /* 0x28439fc — page state built (renderer) */
extern uint64_t g_edit_controls_widget; /* 0x2843e0 */
extern uint64_t g_edit_controls_flag;   /* 0x2844c8 */
extern uint64_t g_edit_controls_state_a; /* 0x2844c0 — edit controls state word */
extern uint64_t g_edit_controls_state_b; /* 0x2844d0 — edit controls state word */
extern uint64_t g_popover_context;      /* 0x281b10 — popover/chooser root */

/* ---- rodata format strings ---- */
extern const char FMT_1343dc[];   /* 0x1343dc — styled label format */
extern const char FMT_132b21[];   /* 0x132b21 — "no baseline" timing note */
extern const char FMT_13c694[];   /* 0x13c694 — timing delta format */
extern const char FMT_13aa7b[];   /* 0x13aa7b — status line wrapper format */

/* ---- engine/widget forwarders (chunks 1-2) ---- */
extern void widget_set_position(uint64_t widget, float x, float y);  /* @ 0016cf5c */
extern void widget_set_scale(uint64_t widget, float sx, float sy);   /* @ 0016cf00 */
extern void widget_set_anchor(uint64_t widget, void *anchor_out);    /* @ 0016cf84 */
extern void engine_call_66ae58(uint64_t out, const char *text);      /* @ 0016ced8 */
extern void engine_call_5921a0(uint64_t a, uint64_t b);              /* @ 0016ceac */
extern void engine_call_592250(uint64_t a, uint64_t b);              /* @ 0016ceec */
extern void game_call_66ad48(void *arg);  /* misc.c (a-chain) @ 0014ec30 — engine base + 0x66ad48 */
extern void ui_text_style_format(char *buf, size_t fortify_slen,
                                 size_t write_cap, const char *fmt,
                                 ...);     /* misc.c (b-chain) @ 00176a24 */

/* ---- own earlier chunks (b-chain) ---- */
extern int is_plausible_ptr_mb(uint64_t v);                   /* misc.c (b-chain) */
extern int widget_child_link_valid(uint64_t node,
                                   uint64_t parent);          /* misc.c (b-chain) @ 0016d5cc */
extern int widget_pair_snapshot_read(const uint64_t *pair, int index,
                                     uint64_t *out);          /* misc.c (b-chain) @ 0017c704 */
extern int widget_edit_controls_bound_check(uint64_t widget); /* misc.c (b-chain) @ 00179dfc */

/* ---- own later chunks (b-chain) ---- */
extern int widget_popover_bound_check(uint64_t widget);       /* misc.c (b-chain) @ 0017df44 (below) */
extern uint64_t engine_singleton_1307e20_read(void);          /* misc.c (b-chain) @ 0017ec28 (below) */

/* ===== panel pair attached check ===== */

/*
 * ui_widget_pair_attached_check — the cached panel pair (0x284220 panel,
 * 0x2842f8 page list) is attached: page state built (0x28439fc == 1),
 * page ctx flag set (0x2843d8), page ctx == g_ui_context, the panel is
 * linked under the UI context and its +0x48 flag reads 1, and the page
 * list is likewise linked and flagged. Returns 1 when the pair is live.
 * @ 0017ce78
 */
int ui_widget_pair_attached_check(void)
{
    if (g_page_state_flag != 1 || g_page_context_flag == 0
        || g_page_context_widget != g_ui_context)
        return 0;

    if (g_panel_widget == 0
        || widget_child_link_valid(g_panel_widget, g_ui_context) == 0)
        return 0;
    int8_t flag = -1;
    if (g_panel_widget + 0x49 < 0x1000
        || g_host_read(g_host_ctx, g_panel_widget + 0x48, &flag, 1) != 1
        || flag != 1)
        return 0;

    if (g_page_list_widget == 0
        || widget_child_link_valid(g_page_list_widget, g_ui_context) == 0)
        return 0;
    flag = -1;
    if (g_page_list_widget + 0x49 < 0x1000
        || g_host_read(g_host_ctx, g_page_list_widget + 0x48, &flag, 1) != 1
        || flag != 1)
        return 0;
    return 1;
}

/*
 * ui_cached_widget_clear_hide — clear the edit-controls state words
 * (0x2844d0, 0x2844c0) and park the edit-controls widget (0x2843e0)
 * offscreen at (-10000, -10000) when the flag (0x2844c8) and widget are
 * live and still linked under the UI context. @ 0017cfbc
 */
void ui_cached_widget_clear_hide(void)
{
    g_edit_controls_state_b = 0;
    g_edit_controls_state_a = 0;

    if (g_edit_controls_flag != 0 && g_edit_controls_widget != 0
        && widget_child_link_valid(g_edit_controls_widget,
                                   g_ui_context) != 0)
        widget_set_position(g_edit_controls_widget, -10000.0f,
                            -10000.0f); /* 0xc61c3c00 */
}

/* ===== popover label rendering ===== */

/*
 * ui_label_render — render a text label under the popover style ctx:
 * record holds {popover ctx at +0x08, widget at +0x10, 0x80 text cache at
 * +0x18}. The ctx must pass widget_popover_bound_check and the widget
 * must be bound under it (<= 16 levels). When the text (length < 0x80)
 * differs from the cache a fresh engine string object is built
 * (engine_call_66ae58), set on the widget (engine_call_5921a0),
 * released (game_call_66ad48), copied through ui_text_style_format
 * (fmt 0x1343dc, cap 0x80) and the widget invalidated
 * (engine_call_592250 with -1). The widget and ctx are parked at (0,0),
 * the widget anchor read back and the widget positioned at
 * (x - ax, y - ay). @ 0017d6b0
 */
void ui_label_render(float x, float y, void *record, const char *text)
{
    uint64_t popover_ctx = *(uint64_t *)((uint8_t *)record + 0x08);

    if (widget_popover_bound_check(popover_ctx) == 0)
        return;

    uint64_t target = popover_ctx;
    uint64_t cur = *(uint64_t *)((uint8_t *)record + 0x10);
    if (cur != 0) {
        uint32_t depth = 0;
        for (;;) {
            if (cur == target || depth > 0xf)
                break;
            uint64_t parent_raw = 0;
            int rd = is_plausible_ptr_mb(cur + 0x40)
                     && g_host_read(g_host_ctx, cur + 0x38, &parent_raw, 8) == 1;
            int parent_ok = rd && is_plausible_ptr_mb(parent_raw)
                            && (parent_raw & 7) == 0;
            uint64_t parent = parent_ok ? parent_raw : 0;
            if (widget_child_link_valid(cur, parent) == 0)
                return;
            depth++;
            cur = parent_raw;
            if (!parent_ok) {
                cur = 0;
                break;
            }
        }
        if (cur == target && strlen(text) < 0x80) {
            uint64_t widget = *(uint64_t *)((uint8_t *)record + 0x10);
            char *cache = (char *)record + 0x18;

            if (strcmp(cache, text) != 0) {
                uint64_t str_obj = 0;
                engine_call_66ae58((uint64_t)&str_obj, text);
                engine_call_5921a0(widget, (uint64_t)&str_obj);
                game_call_66ad48(&str_obj);
                ui_text_style_format(cache, (size_t)-1, 0x80, FMT_1343dc,
                                     text);
                engine_call_592250(widget, 0xffffffffu);
            }

            widget_set_position(widget, 0.0f, 0.0f);
            widget_set_position(popover_ctx, 0.0f, 0.0f);
            float anchor[4];
            widget_set_anchor(widget, anchor);
            if (isfinite(anchor[0]) && !isnan(anchor[0])
                && isfinite(anchor[1]) && !isnan(anchor[1]))
                widget_set_position(widget, x - anchor[0], y - anchor[1]);
        }
    }
}

/*
 * widget_popover_bound_check — the widget's scene owner (+0x30) is NULL
 * or the scene root, and the widget is bound under the popover ctx
 * (0x281b10) within 16 parent levels, every link validated.
 * Returns 1 when bound. @ 0017df44
 */
int widget_popover_bound_check(uint64_t widget)
{
    if (widget == 0)
        return 0;

    uint64_t owner = 0;
    if (!is_plausible_ptr_mb(widget + 0x38)
        || g_host_read(g_host_ctx, widget + 0x30, &owner, 8) != 1
        || (owner != 0 && owner != g_ui_scene_root))
        return 0;

    uint64_t cur = widget;
    int ok = 1;
    for (uint32_t depth = 0; ; depth++) {
        ok = 1;
        if (cur == g_popover_context || depth > 0xf)
            break;

        uint64_t parent_raw = 0;
        int rd = is_plausible_ptr_mb(cur + 0x40)
                 && g_host_read(g_host_ctx, cur + 0x38, &parent_raw, 8) == 1;
        int parent_ok = rd && is_plausible_ptr_mb(parent_raw)
                        && (parent_raw & 7) == 0;
        uint64_t parent = parent_ok ? parent_raw : 0;

        if (widget_child_link_valid(cur, parent) != 0) {
            cur = parent_raw;
            if (parent_ok)
                continue;
        }
        cur = 0; /* broken link: the walker falls off (raw clears it) */
        break;
    }
    return cur == g_popover_context ? ok : 0;
}

/* ===== rich view label rendering ===== */

/*
 * rich_label_render — render a text label on the rich view: record holds
 * {edit-controls ctx at +0x08, widget at +0x10, 0x140 text cache at
 * +0x18}. The ctx must pass widget_edit_controls_bound_check and the
 * widget must be bound under it (<= 16 levels). When the text
 * (length < 0x140) differs from the cache a fresh engine string object
 * is built, set, released and cached (fmt 0x1343dc, cap 0x140) and the
 * widget invalidated (engine_call_592250 with -1). The label font is
 * then divided by the widget child count at +0xb0 (1..0x100), the widget
 * rescaled, parked at (0,0), its anchor read back, positioned at
 * (x - ax, y - ay) and the ctx parked at (0,0). @ 0017e998
 */
void rich_label_render(float x, float y, float font, void *record,
                       const char *text)
{
    uint64_t ctx = *(uint64_t *)((uint8_t *)record + 0x08);

    if (widget_edit_controls_bound_check(ctx) == 0)
        return;

    uint64_t target = ctx;
    uint64_t cur = *(uint64_t *)((uint8_t *)record + 0x10);
    if (cur != 0) {
        uint32_t depth = 0;
        for (;;) {
            if (cur == target || depth > 0xf)
                break;
            uint64_t parent_raw = 0;
            int rd = is_plausible_ptr_mb(cur + 0x40)
                     && g_host_read(g_host_ctx, cur + 0x38, &parent_raw, 8) == 1;
            int parent_ok = rd && is_plausible_ptr_mb(parent_raw)
                            && (parent_raw & 7) == 0;
            uint64_t parent = parent_ok ? parent_raw : 0;
            if (widget_child_link_valid(cur, parent) == 0)
                return;
            depth++;
            cur = parent_raw;
            if (!parent_ok) {
                cur = 0;
                break;
            }
        }
        if (cur == target && strlen(text) < 0x140) {
            uint64_t widget = *(uint64_t *)((uint8_t *)record + 0x10);
            char *cache = (char *)record + 0x18;

            if (strcmp(cache, text) != 0) {
                uint64_t str_obj = 0;
                engine_call_66ae58((uint64_t)&str_obj, text);
                engine_call_5921a0(widget, (uint64_t)&str_obj);
                game_call_66ad48(&str_obj);
                ui_text_style_format(cache, (size_t)-1, 0x140, FMT_1343dc,
                                     text);
                engine_call_592250(widget, 0xffffffffu);
            }

            uint16_t children = 0;
            if (is_plausible_ptr_mb(widget + 0xb2)
                && g_host_read(g_host_ctx, widget + 0xb0, &children, 2) == 1
                && 0 < children && children < 0x101) {
                font = font / (float)(int)children;
                widget_set_scale(widget, font, font);
                widget_set_position(widget, 0.0f, 0.0f);
                float anchor[4];
                widget_set_anchor(widget, anchor);
                if (isfinite(anchor[0]) && !isnan(anchor[0])
                    && isfinite(anchor[1]) && !isnan(anchor[1]))
                    widget_set_position(widget, x - anchor[0], y - anchor[1]);
                widget_set_position(ctx, 0.0f, 0.0f);
            }
        }
    }
}

/* ===== engine singleton ===== */

/*
 * engine_singleton_1307e20_read — read and deeply validate the game
 * singleton chain at base + 0x1307e20: singleton -> +0x50 state (must be
 * 4) -> +0x48 object (vtable game+0x1214018) -> +0x28 view (vtable
 * game+0x11f1a20) -> +0x948 panel (vtable game+0x120abe0), which must
 * reach the UI context in 16 validated parent levels; the panel's +0x90
 * child (vtable game+0x11ad208) must in turn reach the view object in 16
 * levels. Returns the +0x948 child chain object, or 0 on failure.
 * @ 0017ec28
 */
uint64_t engine_singleton_1307e20_read(void)
{
    uint64_t singleton = 0;
    if (!is_plausible_ptr_mb(g_game_base + 0x1307e28)
        || g_host_read(g_host_ctx, g_game_base + 0x1307e20, &singleton, 8) != 1
        || !is_plausible_ptr_mb(singleton) || (singleton & 7) != 0)
        return 0;

    int32_t state = -1;
    if (!is_plausible_ptr_mb(singleton + 0x54)
        || g_host_read(g_host_ctx, singleton + 0x50, &state, 4) != 1
        || state != 4)
        return 0;

    uint64_t obj = 0;
    if (g_host_read(g_host_ctx, singleton + 0x48, &obj, 8) != 1
        || !is_plausible_ptr_mb(obj) || (obj & 7) != 0)
        return 0;

    uint64_t obj_vtable = 0;
    if (!is_plausible_ptr_mb(obj + 8)
        || g_host_read(g_host_ctx, obj, &obj_vtable, 8) != 1
        || !is_plausible_ptr_mb(obj_vtable) || (obj_vtable & 7) != 0
        || obj_vtable != g_game_base + 0x1214018)
        return 0;

    uint64_t view = 0;
    if (is_plausible_ptr_mb(obj + 0x30)
        && g_host_read(g_host_ctx, obj + 0x28, &view, 8) == 1) {
        if (!is_plausible_ptr_mb(view) || (view & 7) != 0)
            view = 0;
    } else {
        view = 0;
    }

    uint64_t view_vtable = 0;
    if (!is_plausible_ptr_mb(view + 8)
        || g_host_read(g_host_ctx, view, &view_vtable, 8) != 1
        || !is_plausible_ptr_mb(view_vtable) || (view_vtable & 7) != 0
        || view_vtable != g_game_base + 0x11f1a20)
        return 0;

    uint64_t panel = 0;
    if (is_plausible_ptr_mb(view + 0x950)
        && g_host_read(g_host_ctx, view + 0x948, &panel, 8) == 1) {
        if (!is_plausible_ptr_mb(panel) || (panel & 7) != 0)
            panel = 0;
    } else {
        panel = 0;
    }

    uint64_t panel_vtable = 0;
    if (!is_plausible_ptr_mb(panel + 8)
        || g_host_read(g_host_ctx, panel, &panel_vtable, 8) != 1
        || !is_plausible_ptr_mb(panel_vtable) || (panel_vtable & 7) != 0
        || panel_vtable != g_game_base + 0x120abe0)
        return 0;

    /* panel must reach the UI context within 16 validated parent levels */
    {
        uint64_t cur = panel;
        for (uint32_t depth = 0; ; depth++) {
            if (cur == g_ui_context || depth > 0xf)
                break;
            uint64_t parent_raw = 0;
            int rd = is_plausible_ptr_mb(cur + 0x40)
                     && g_host_read(g_host_ctx, cur + 0x38, &parent_raw, 8) == 1;
            int parent_ok = rd && is_plausible_ptr_mb(parent_raw)
                            && (parent_raw & 7) == 0;
            uint64_t parent = parent_ok ? parent_raw : 0;
            if (widget_child_link_valid(cur, parent) == 0)
                return 0;
            cur = parent_raw;
            if (!parent_ok) {
                cur = 0;
                break;
            }
        }
        if (cur != g_ui_context)
            return 0;
    }

    /* the panel's +0x90 child (movie-class) must reach the view object */
    {
        uint64_t child = 0;
        if (is_plausible_ptr_mb(panel + 0x98)
            && g_host_read(g_host_ctx, panel + 0x90, &child, 8) == 1) {
            if (!is_plausible_ptr_mb(child) || (child & 7) != 0)
                child = 0;
        } else {
            child = 0;
        }

        uint64_t child_vtable = 0;
        if (!is_plausible_ptr_mb(child + 8)
            || g_host_read(g_host_ctx, child, &child_vtable, 8) != 1
            || !is_plausible_ptr_mb(child_vtable)
            || (child_vtable & 7) != 0
            || child_vtable != g_game_base + 0x11ad208)
            return 0;

        uint64_t cur = child;
        int ok = 1;
        for (uint32_t depth = 0; ; depth++) {
            ok = 1;
            if (cur == view || depth > 0xf)
                break;
            uint64_t parent_raw = 0;
            int rd = is_plausible_ptr_mb(cur + 0x40)
                     && g_host_read(g_host_ctx, cur + 0x38, &parent_raw, 8) == 1;
            int parent_ok = rd && is_plausible_ptr_mb(parent_raw)
                            && (parent_raw & 7) == 0;
            uint64_t parent = parent_ok ? parent_raw : 0;
            if (widget_child_link_valid(cur, parent) != 0) {
                cur = parent_raw;
                if (parent_ok)
                    continue;
            }
            cur = 0; /* broken link: the walker falls off */
            break;
        }
        return ok && cur == view ? child : 0;
    }
}

/* ===== timing status line ===== */

/*
 * ui_timing_line_format — format the timing status line (cap 0x140):
 * "<ms> ms[ / note]" (or "not measured" when ms < 0) combined with the
 * delta note ("<delta>" or the 0x132b21 "no baseline" text when
 * delta < 0) through the 0x13aa7b wrapper format. @ 0017f1e4
 */
void ui_timing_line_format(char *buf, int ms, const char *note, int delta)
{
    char ms_part[100];
    char delta_part[0x30];

    if (ms < 0) {
        ui_text_style_format(ms_part, 100, 100, "not measured");
    } else {
        const char *sep = "";
        if (note != NULL && note[0] != '\0')
            sep = " / ";
        ui_text_style_format(ms_part, 100, 100, "%d ms%s%.63s", ms, sep,
                             note);
    }

    if (delta < 0)
        ui_text_style_format(delta_part, 0x30, 0x30, FMT_132b21);
    else
        ui_text_style_format(delta_part, 0x30, 0x30, FMT_13c694, delta);

    ui_text_style_format(buf, (size_t)-1, 0x140, FMT_13aa7b, ms_part,
                         delta_part);
}
/* misc_b chain chunk 10: covers raw lines 14663-15100 */

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

/* ---- shared host/service globals (digest-mandated names) ---- */
extern void *g_host_ctx;                                      /* 0x1a7cc8 */
extern int (*g_host_read)(void *ctx, uint64_t addr, void *out,
                          uint32_t len);                      /* 0x1a7ce8 */
extern uint64_t g_game_base;                                  /* 0x1a7cd0 */
extern uint64_t g_ui_scene_root;                              /* 0x1a7d28 */
extern uint64_t g_ui_context;                                 /* 0x1a7d30 */
extern uint64_t g_view_object_a;                              /* 0x1a7d38 */
extern uint64_t g_view_object_b;                              /* 0x281a50 */
extern uint32_t g_menu_open_state;                            /* 0x22d7ec — clip-ops verification cache (== 1 verified) */
extern uint64_t g_libg_span;                                  /* 0x10f740 — {base,size} of libg.so (packed) */

/* ---- own earlier chunks (b-chain) ---- */
extern int is_plausible_ptr_mb(uint64_t v);                   /* misc.c (b-chain) */
extern uint64_t widget_field_ptr_read(uint64_t addr);         /* misc.c (b-chain) @ 0016f79c */
extern clip_cache_t g_clip_cache;                             /* 0x22d4e0 */

/* ---- own later chunks (b-chain) ---- */
extern int clip_slot_precondition_check(void *ops, void *record,
                                        uint64_t root,
                                        uint32_t index);      /* misc.c (b-chain) @ 00180d80 (chunk 11) */
extern int menu_clip_writable_check(uint64_t ctx, void *addr,
                                    size_t len);   /* menu_engine.c @ 00180034 */

/* misc_b-owned: span pair copy right after the clip cache record */
extern uint64_t g_clip_span_pair;  /* 0x22d510 — copy of g_libg_span {base,size} */

/* ---- rodata clip-ops verification tables ---- */
extern const struct {
    uint64_t rva;            /* region offset in the engine image */
    uint64_t len;            /* region length in bytes */
    const char *hex_digest;  /* expected SHA-256 (hex) */
} CLIP_OPS_REGIONS[0x38];    /* 0x19c658 — 56 regions, stride 0x18 */
extern const uint64_t CLIP_OPS_LINK_TABLE[]; /* 0x13fb88 — (expected, link rva, offset) triples, 0x24 links, stride 0x18 */

/* ---- other files / a-chain ---- */
extern int code_region_hash_verify(uint64_t rva, uint64_t len,
                                   const char *hex_digest);   /* misc.c (a-chain) @ 0014d168 */

/* ===== clip movement through the ops channel ===== */

/*
 * widget_clip_move — move the cached clip to (x, y) through the ops
 * channel. mode selects the clip (0 = clip A at 0x22d4e0, else clip C at
 * 0x22d4f0). The coordinates must be finite and within +-1e6; the clip
 * slot record must pass clip_slot_precondition_check before and after.
 * The move goes through ops->call_59531c (engine 0x59531c, integer
 * coordinates), then the frame pair at clip+0x20 is read back and must
 * equal ((int)x, (int)y). Failures are reported through ops->probe_log
 * as "clip_move_arguments"/"clip_move_precondition"/"clip_move_readback"
 * /"clip_move_position"/"clip_move_postcondition" (errno preserved) and
 * the span pair copy at 0x22d510 is refreshed from g_libg_span.
 * Returns 1 on success, 0 on argument/precondition failure, -1 on move
 * failure. @ 0017fc3c
 */
int widget_clip_move(float x, float y, void *ops_v, uint64_t root,
                     uint32_t entry_index, uint32_t mode)
{
    clip_ops_t *ops = (clip_ops_t *)ops_v;
    int saved_errno;
    const char *reason;
    uint64_t frame;

    if (mode > 1 || !isfinite(x) || isnan(x)
        || !(y <= 1000000.0f) || y < -1000000.0f
        || !(x <= 1000000.0f) || x < -1000000.0f) {
        saved_errno = errno;
        if (ops != NULL && ops->probe_log != NULL)
            ops->probe_log(ops->ctx, "clip_move_arguments");
        errno = saved_errno;
        return 0;
    }
    if (isfinite(y) && !isnan(y)) {
        /* coordinate gates passed */
    } else {
        saved_errno = errno;
        if (ops != NULL && ops->probe_log != NULL)
            ops->probe_log(ops->ctx, "clip_move_arguments");
        errno = saved_errno;
        return 0;
    }

    if (clip_slot_precondition_check(ops, &g_clip_cache, root,
                                     entry_index) == 0) {
        saved_errno = errno;
        if (ops != NULL && ops->probe_log != NULL)
            ops->probe_log(ops->ctx, "clip_move_precondition");
        errno = saved_errno;
        return 0;
    }

    uint64_t clip = mode != 0 ? g_clip_cache.clip_c : g_clip_cache.clip_a;
    ops->call_59531c((int)x, (int)y, ops->ctx, clip);

    if (!is_plausible_ptr_mb(clip + 0x28)
        || ops->read_gate(ops->ctx, clip + 0x20, &frame, 8) != 1) {
        reason = "clip_move_readback";
    } else {
        reason = "clip_move_position";
        if ((float)(uint32_t)frame == (float)(int)x
            && *(float *)((uint8_t *)&frame + 4) == (float)(int)y) {
            if (clip_slot_precondition_check(ops, &g_clip_cache, root,
                                             entry_index) != 0)
                return 1;
            reason = "clip_move_postcondition";
        }
    }

    g_clip_span_pair = g_libg_span;
    saved_errno = errno;
    if (ops != NULL && ops->probe_log != NULL)
        ops->probe_log(ops->ctx, reason);
    errno = saved_errno;
    return -1;
}

/* ===== ops channel read/write gates ===== */

/*
 * host_read_gate — ops read callback: addr must be above the zero page
 * (0xfff < addr) and the (addr + len) window must not wrap; then the host
 * read must return 1. Returns 1 on success, 0 otherwise. @ 0017fe9c
 */
int host_read_gate(uint64_t ctx, uint64_t addr, void *out, uint64_t len)
{
    (void)ctx;
    if (addr > 0xfff && addr <= ~len)
        return g_host_read(g_host_ctx, addr, out, (uint32_t)len) == 1;
    return false;
}

/*
 * host_write_gate — ops write callback gating writes into the bound clip
 * object: only the frame pair at clip A +0xe0/+0xe7 (1 or 2 bytes) and
 * the +0x280 slot (2 bytes) of the record at 0x22d4e0 are writable, the
 * target must be a rw-p mapping (menu_clip_writable_check) and clip A
 * must hold vtable game+0x11c11e8. Returns 1 when the memcpy happened.
 * @ 0017fee0
 */
int host_write_gate(uint64_t ctx, void *dst, const void *src, size_t len)
{
    (void)ctx;
    uint64_t clip = g_clip_cache.clip_a;

    if (clip != 0) {
        uint64_t vtable = 0;
        int rd = is_plausible_ptr_mb(clip + 8)
                 && g_host_read(g_host_ctx, clip, &vtable, 8) == 1;
        if (rd && is_plausible_ptr_mb(vtable) && (vtable & 7) == 0
            && vtable == g_game_base + 0x11c11e8) {
            int ok = 0;
            if ((len == 1 && dst == (void *)(clip + 0xe0))
                || (len == 2 && dst == (void *)(clip + 0xe7))
                || (len == 2 && dst == (void *)(clip + 0x280)))
                ok = menu_clip_writable_check(0, dst, len);
            if (ok != 0) {
                memcpy(dst, src, len);
                return 1;
            }
        }
    }
    return 0;
}

/*
 * clip_ops_integrity_check — verify the ops channel environment once and
 * cache the result at 0x22d7ec: 1 = verified, -1 = failed, 0 = not yet
 * checked. Verification SHA-256-checks the 56-region table at 0x19c658,
 * walks the 0x24-link engine object graph from game+0x11c11e8 (link
 * table 0x13fb88), then checks nine further engine regions
 * (0x7050a4/0x705160/0x7051dc/0x705418/0x705354/0x705280/0x705024/
 * 0x703f54/0x70524c). Returns the cached verdict (nonzero when
 * verified). @ 001802bc
 */
int clip_ops_integrity_check(uint64_t ctx)
{
    (void)ctx;

    if (g_menu_open_state != 0)
        return g_menu_open_state == 1;

    if (g_game_base - 1 >= 0xfffffffffecfffffULL)
        goto fail; /* implausible game base (raw guard) */

    /* the 56-region SHA-256 table */
    for (uint32_t i = 0; i < 0x38; i++) {
        if (code_region_hash_verify(CLIP_OPS_REGIONS[i].rva,
                                    CLIP_OPS_REGIONS[i].len,
                                    CLIP_OPS_REGIONS[i].hex_digest) == 0)
            goto fail;
    }

    /* the 0x24-link engine object graph from game+0x11c11e8 */
    {
        uint64_t node = 0;
        if (!is_plausible_ptr_mb(g_game_base + 0x11c11f0)
            || g_host_read(g_host_ctx, g_game_base + 0x11c11e8, &node, 8) != 1)
            goto fail;

        uint32_t i = 0;
        int all = 0;
        for (;;) {
            if (node != g_game_base + CLIP_OPS_LINK_TABLE[i * 3])
                break; /* mismatch leaves the verdict unset */
            all = i > 0x47 / 2; /* raw: 0x47 < i marks the tail reached */
            if (i == 0x48 / 2) {
                /* tail link verified */
                if (code_region_hash_verify(0x7050a4, 0xbc,
                    "34a5074b72bb308ac25ec9ca60db279eb5472b00f936c54d506c99f27879ad7d") != 0
                    && code_region_hash_verify(0x705160, 0x7c,
                    "3fec2737dd5569528dcbea561b5f857c8419aa34de056e3233869b1b32ee5f0b") != 0
                    && code_region_hash_verify(0x7051dc, 0x70,
                    "f9da06b9ce7403e941cdf05394a692d2bf59b300270f04e21f83e851110ea045") != 0
                    && code_region_hash_verify(0x705418, 0x90,
                    "ba39ba731804b47bf09a9369d066567e3dd70764d5c5b47c985ce0c219e3ea75") != 0
                    && code_region_hash_verify(0x705354, 0xc4,
                    "577309d6feab342db3a5ea28dcd15307b2d2201786e90ac420c9bcba1e70f719") != 0
                    && code_region_hash_verify(0x705280, 0xd4,
                    "82083116eab2c15c96c8b38209d190631012ad7b2c69185f18a6f0b215835016") != 0
                    && code_region_hash_verify(0x705024, 0x80,
                    "bb6ad764d8431c880757955fb510561f51565976e013bd7a7539453a65af4b5b") != 0
                    && code_region_hash_verify(0x703f54, 0x5dc,
                    "8116689446e3a51512bec03280d3ce90ccd96999212446521c0aeaa1a90084b8") != 0
                    && code_region_hash_verify(0x70524c, 0x34,
                    "f8b2086478f23df04d7aff356408627f61f75f068fc0fe0a5b38563cd1ec2dda") != 0) {
                    g_menu_open_state = 1;
                    return 1;
                }
                g_menu_open_state = -1;
                return 0;
            }
            uint64_t next = g_game_base + CLIP_OPS_LINK_TABLE[i * 3 + 1]
                            + CLIP_OPS_LINK_TABLE[i * 3 + 2];
            if (!is_plausible_ptr_mb(next + 8))
                break;
            if (g_host_read(g_host_ctx, next, &node, 8) != 1)
                break;
            i += 1;
            (void)all;
        }
        /* fall through to fail unless the tail was reached */
        if (i == 0x24)
            goto verified;
    }

fail:
    g_menu_open_state = -1;
    return 0;

verified:
    g_menu_open_state = 1;
    return 1;
}

/* ===== engine-call adapter veneers (drop the channel handle) ===== */

/*
 * engine_call_11a2840_adapter — veneer dropping the channel handle and
 * forwarding to engine_call_11a2840 (engine base + 0x11a2840).
 * @ 00180540
 */
void engine_call_11a2840_adapter(uint64_t ctx, uint64_t a)
{
    (void)ctx;
    engine_call_11a2840(a);
}

/*
 * engine_call_88b344_adapter — veneer dropping the channel handle and
 * forwarding (a, b) to game base + 0x88b344. @ 00180548
 */
void engine_call_88b344_adapter(uint64_t ctx, uint64_t a, uint32_t b)
{
    (void)ctx;
    ((void (*)(uint64_t, uint32_t))(g_game_base + 0x88b344))(a, b);
}

/*
 * engine_call_593f14_adapter — veneer dropping the channel handle and
 * forwarding (a, b) to game base + 0x593f14. @ 00180564
 */
void engine_call_593f14_adapter(uint64_t ctx, uint64_t a, uint32_t b)
{
    (void)ctx;
    ((void (*)(uint64_t, uint32_t))(g_game_base + 0x593f14))(a, b);
}

/*
 * engine_call_594130_adapter — veneer dropping the channel handle and
 * forwarding (a, b) to engine_call_594130 (engine base + 0x594130).
 * @ 00180580
 */
void engine_call_594130_adapter(uint64_t ctx, uint64_t a, uint64_t b)
{
    (void)ctx;
    engine_call_594130(a, b);
}

/*
 * engine_call_59567c_adapter — veneer dropping the channel handle and
 * forwarding a to the engine detach call (game base + 0x59567c).
 * @ 0018058c
 */
void engine_call_59567c_adapter(uint64_t ctx, uint64_t a)
{
    (void)ctx;
    ((void (*)(uint64_t))(g_game_base + 0x59567c))(a);
}

/*
 * engine_call_5940d0_gated — call engine 0x5940d0 on a: when a is the
 * cached clip C (vtable game+0x11abcf0) use the 0x5940d0 slot; when it
 * is the cached clip A (vtable game+0x11c11e8) use game+0x88b83c.
 * @ 001805a4
 */
void engine_call_5940d0_gated(uint64_t ctx, uint64_t a)
{
    (void)ctx;
    void (*fn)(uint64_t) = NULL;

    if (a == g_clip_cache.clip_c) {
        uint64_t vtable = 0;
        int rd = is_plausible_ptr_mb(a + 8)
                 && g_host_read(g_host_ctx, a, &vtable, 8) == 1;
        if (rd && is_plausible_ptr_mb(vtable) && (vtable & 7) == 0
            && vtable == g_game_base + 0x11abcf0)
            fn = (void (*)(uint64_t))(g_game_base + 0x5940d0);
    }
    if (fn == NULL && a == g_clip_cache.clip_a) {
        uint64_t vtable = 0;
        int rd = is_plausible_ptr_mb(a + 8)
                 && g_host_read(g_host_ctx, a, &vtable, 8) == 1;
        if (rd && is_plausible_ptr_mb(vtable) && (vtable & 7) == 0
            && vtable == g_game_base + 0x11c11e8)
            fn = (void (*)(uint64_t))(g_game_base + 0x88b83c);
    }
    if (fn != NULL)
        fn(a);
}

/*
 * engine_call_59531c_adapter — veneer for the position setter slot
 * (game base + 0x59531c) invoked from widget_clip_move with the integer
 * coordinates and the clip handle. @ 00180724
 */
void engine_call_59531c_adapter(int x, int y, uint64_t ctx, uint64_t clip)
{
    (void)ctx;
    (void)clip;
    ((void (*)(uint64_t))(g_game_base + 0x59531c))((uint64_t)(uint32_t)x
                                                       | ((uint64_t)(uint32_t)y << 32));
}

/*
 * engine_call_88d290_adapter — veneer dropping the channel handle and
 * forwarding (a, b) to game base + 0x88d290. @ 0018073c
 */
void engine_call_88d290_adapter(uint64_t ctx, uint64_t a, uint32_t b)
{
    (void)ctx;
    ((void (*)(uint64_t, uint32_t))(g_game_base + 0x88d290))(a, b);
}

/*
 * engine_call_594e80_adapter — veneer dropping the channel handle and
 * forwarding (a, b) to engine_call_594e80 (engine base + 0x594e80).
 * @ 00180758
 */
void engine_call_594e80_adapter(uint64_t ctx, uint64_t a, uint32_t b)
{
    (void)ctx;
    engine_call_594e80(a, b);
}

/*
 * remote_ptr_field_check — read the qword at addr through the channel
 * (record: {base, ctx, read fn}) and compare it against base + rva.
 * Returns 1 on a match. @ 00180764
 */
int remote_ptr_field_check(const uint64_t *record, uint64_t addr,
                           int64_t rva, uint64_t len)
{
    int (*read_fn)(uint64_t, uint64_t, void *, uint64_t) =
        (int (*)(uint64_t, uint64_t, void *, uint64_t))record[2];
    uint64_t value = 0;

    if (addr < ~len - 8 && addr > 0xfff && (addr & 7) == 0) {
        if (read_fn(record[1], addr, &value, 8) == 1)
            return value == record[0] + rva;
    }
    return 0;
}
/* misc_b chain chunk 11: covers raw lines 15101-15550 */

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

/* ---- shared host/service globals (digest-mandated names) ---- */
extern void *g_host_ctx;                                      /* 0x1a7cc8 */
extern int (*g_host_read)(void *ctx, uint64_t addr, void *out,
                          uint32_t len);                      /* 0x1a7ce8 */
extern uint64_t g_game_base;                                  /* 0x1a7cd0 */
extern uint64_t g_ui_scene_root;                              /* 0x1a7d28 */
extern uint64_t g_ui_context;                                 /* 0x1a7d30 */
extern uint64_t g_view_object_a;                              /* 0x1a7d38 */
extern uint64_t g_view_object_b;                              /* 0x281a50 */
extern int32_t g_menu_entry_index;                            /* 0x1a7d18 */

/* ---- own earlier chunks (b-chain) ---- */
extern int is_plausible_ptr_mb(uint64_t v);                   /* misc.c (b-chain) */
extern clip_cache_t g_clip_cache;                             /* 0x22d4e0 */
extern int widget_child_link_valid(uint64_t node,
                                   uint64_t parent);          /* misc.c (b-chain) @ 0016d5cc */
extern int widget_detached_check(uint64_t node);              /* misc.c (b-chain) @ 0016ddc8 */
extern int remote_vtable_matches(uint64_t obj, uint64_t rva); /* misc.c (b-chain) @ 0016debc */
extern int widget_ancestor_of(uint64_t node, uint64_t target);/* misc.c (b-chain) @ 0016d800 */
extern uint64_t widget_field_ptr_read(uint64_t addr);         /* misc.c (b-chain) @ 0016f79c */
extern int remote_ptr_field_check(const uint64_t *record, uint64_t addr,
                                  int64_t rva, uint64_t len); /* misc.c (b-chain) @ 00180764 */
extern int widget_kind1_validate(uint64_t widget, uint64_t ctx,
                                 void *snapshot);             /* misc.c (b-chain) @ 00177620 */

/* ---- own later chunks (b-chain) ---- */
extern int widget_tree_walk_collect(uint64_t node);           /* misc.c (b-chain) @ 00181790 (chunk 12) */

/* ---- theme preview state (misc_b-owned later chunks / registry) ---- */
extern uint64_t g_theme_preview_obj;   /* 0x284650 — cached theme preview widget */
extern uint64_t g_theme_preview_ctx;   /* 0x284658 — preview context */
extern int theme_preview_linked(uint64_t preview, uint64_t ctx); /* misc.c (b-chain) @ 00181b78 */

/* ---- menu entry records (rodata) ---- */
extern const uint8_t g_menu_entry_table[]; /* 0x1c3568 — 42 records, 0xd8-byte stride; widgets at +0/+8/+0x18 */

/* ===== remote widget checks through the channel record ===== */

/*
 * remote_record_t — channel record {base, ctx, read fn} at +0 with the
 * clip triple {clip_a, obj, clip_c} used by the widget checks.
 */
typedef struct {
    uint64_t base;      /* +0x00 — game base */
    uint64_t ctx;       /* +0x08 — channel handle */
    void *read_fn;      /* +0x10 — channel read */
    uint64_t fns[8];    /* +0x18..+0x50 — the ops fn slots */
    uint64_t flags[3];  /* +0x58..+0x68 — flag/extra words */
    uint32_t _pad;      /* +0x6c */
    int32_t  err_word;  /* +0x70 — must be 0 */
} remote_record_t;

/*
 * remote_widget_detached_check — channel read: the widget's parent
 * (+0x38) is NULL and the child index (+0x40) is -1. Returns 1 when
 * detached. @ 00180814
 */
int remote_widget_detached_check(const remote_record_t *rec, uint64_t widget)
{
    int (*read_fn)(uint64_t, uint64_t, void *, uint64_t) = rec->read_fn;
    uint64_t parent = 1; /* prefill: a failed read must not pass */
    int32_t index = 0;

    if ((widget + 0x40) >> 3 > 0x200) {
        if (read_fn(rec->ctx, widget + 0x38, &parent, 8) == 1
            && widget < 0xffffffffffffffbcULL && parent == 0
            && read_fn(rec->ctx, widget + 0x40, &index, 4) == 1)
            return index == -1;
    }
    return 0;
}

/*
 * remote_widget_link_check — channel read: node's parent (+0x38) equals
 * parent, its child index (+0x40) is >= 0, the child count at
 * parent+0x4e is 0xffff or >= index, and parent's children array
 * (parent+0x50) slot [index] holds node. Returns 1 when linked.
 * @ 001808ec
 */
int remote_widget_link_check(const remote_record_t *rec, uint64_t node,
                             uint64_t parent)
{
    int (*read_fn)(uint64_t, uint64_t, void *, uint64_t) = rec->read_fn;
    uint64_t parent_link = 0;
    int32_t index = -1;
    uint16_t count = 0;
    uint64_t children = 0;
    uint64_t slot_val = 0;

    if ((node + 0x40) >> 3 <= 0x200 || (parent + 0x80) >> 7 <= 0x20
        || (parent & 7) != 0)
        return 0;
    if (read_fn(rec->ctx, node + 0x38, &parent_link, 8) != 1
        || node >= 0xffffffffffffffbcULL || parent_link != parent)
        return 0;
    if (read_fn(rec->ctx, node + 0x40, &index, 4) != 1 || index < 0)
        return 0;
    if (read_fn(rec->ctx, parent + 0x4e, &count, 2) != 1)
        return 0;
    if (count == 0xffff || (uint32_t)count <= (uint32_t)index)
        return 0;
    if (read_fn(rec->ctx, parent + 0x50, &children, 8) != 1)
        return 0;
    if (children < 0x1000 || (children & 7) != 0
        || (~(uint64_t)(count << 3)) < children)
        return 0;
    uint64_t slot = children + (uint64_t)(index << 3);
    if ((slot + 8) >> 3 <= 0x200
        || read_fn(rec->ctx, slot, &slot_val, 8) != 1)
        return 0;
    return slot_val == node;
}

/*
 * remote_widget_id_equals — channel read of the ushort id at
 * widget+0x4e compared with expected. Returns 1 on a match.
 * @ 00180ab8
 */
int remote_widget_id_equals(const remote_record_t *rec, uint64_t widget,
                            uint32_t expected)
{
    int (*read_fn)(uint64_t, uint64_t, void *, uint64_t) = rec->read_fn;
    uint16_t id = 0;

    if ((widget + 0x50) >> 1 >= 0x801
        && read_fn(rec->ctx, widget + 0x4e, &id, 2) == 1)
        return id == expected;
    return 0;
}

/*
 * remote_record_validate — validate the channel record against the
 * cached clip triple {clip_a (0x22d4e0), obj (0x22d4e8), clip_c
 * (0x22d4f0)}: the record's base must sit in a plausible image window,
 * all 15 fn slots and the err word (+0x34) must be live, the triple must
 * be three distinct objects, clip_a holds vtable base+0x11c11e8, obj and
 * clip_c sit at base+0x11c1360 and base+0x11abcf0 (remote_ptr_field_check),
 * clip_a+0xd0 points at obj, both ids read 1, and obj/clip_a as well as
 * clip_c/obj pass remote_widget_link_check. Returns 1 when the record is
 * valid. @ 00180b44
 */
int remote_record_validate(const remote_record_t *rec,
                           const uint64_t *triple)
{
    if (rec == NULL)
        return 0;

    if (rec->base - 0x1000 >= 0xfffffffffecff000ULL || rec->read_fn == NULL
        || ((uint64_t *)rec)[3] == 0 || ((uint64_t *)rec)[4] == 0
        || ((uint64_t *)rec)[5] == 0 || ((uint64_t *)rec)[6] == 0
        || ((uint64_t *)rec)[7] == 0 || ((uint64_t *)rec)[8] == 0
        || ((uint64_t *)rec)[9] == 0 || ((uint64_t *)rec)[10] == 0
        || ((uint64_t *)rec)[11] == 0 || ((uint64_t *)rec)[12] == 0
        || ((uint64_t *)rec)[13] == 0 || *(int32_t *)((uint8_t *)rec + 0x34) != 0
        || triple[4] != rec->base)
        return 0;

    uint64_t clip_a = triple[0];
    uint64_t obj = triple[1];
    uint64_t clip_c = triple[2];

    if (clip_a == obj || clip_a == clip_c || obj == clip_c)
        return 0;

    /* clip_a vtable + the obj/clip_c remote field identities */
    uint64_t vtable = 0;
    int (*read_fn)(uint64_t, uint64_t, void *, uint64_t) = rec->read_fn;
    if (clip_a - 0x1000 < 0xffffffffffffed38ULL && (clip_a & 7) == 0
        && read_fn(rec->ctx, clip_a, &vtable, 8) == 1
        && vtable == rec->base + 0x11c11e8) {
        if (!remote_ptr_field_check((const uint64_t *)rec, obj, 0x11c1360,
                                    0x80))
            return 0;
        if (!remote_ptr_field_check((const uint64_t *)rec, clip_c,
                                    0x11abcf0, 0x80))
            return 0;

        uint64_t clip_a_d0 = 0;
        if ((clip_a + 0xd8) >> 3 > 0x200
            && read_fn(rec->ctx, clip_a + 0xd0, &clip_a_d0, 8) == 1
            && clip_a_d0 == obj) {
            if (remote_widget_id_equals(rec, clip_a, 1)
                && remote_widget_id_equals(rec, obj, 1)
                && remote_widget_link_check(rec, obj, clip_a)
                && remote_widget_link_check(rec, clip_c, obj))
                return 1;
        }
    }
    return 0;
}

/* ===== clip slot precondition ===== */

/*
 * clip_slot_precondition_check — validate the cached clip record through
 * the ops channel before/after a clip move: the record's triple must pass
 * remote_record_validate, the four +0x48 flag bytes and +0xe0/+0xe7/
 * +0x27e/+0x280 flag words must read the expected values, the frame
 * scale at +0x238 must be 1.0 and the frame pair at +0xc0 must match the
 * record's cached pair (+0x28/+0x2c); obj's 0x18-byte identity block at
 * +0x10 must read {1.0, 1.0, 0}. The active clip (clip_a when the record
 * mode +0x18 is 0, else the remote link target) must read detached
 * (parent NULL, index -1) or pass remote_widget_link_check, and the child
 * id at clip_c+0x4e must equal index; the three +0x30 owner fields must
 * all equal root. Returns 1 when the record is consistent.
 * @ 00180d80
 */
int clip_slot_precondition_check(void *ops_v, void *record_v,
                                 uint64_t root, uint32_t index)
{
    clip_ops_t *ops = (clip_ops_t *)ops_v;
    uint64_t *record = (uint64_t *)record_v;
    int (*read_fn)(uint64_t, uint64_t, void *, uint64_t) =
        (int (*)(uint64_t, uint64_t, void *, uint64_t))ops->read_gate;

    if (record == NULL || (int32_t)record[6] == 0)
        return 0;
    if (remote_record_validate((const remote_record_t *)ops, record) == 0)
        return 0;

    uint64_t clip_a = record[0], obj = record[1], clip_c = record[2];
    uint8_t f48_a = 0, f48_b = 0, f48_c = 0, fe0 = 0, f27e = 0;
    uint16_t fe7 = 1, f280 = 1;
    float scale = 0.0f;
    uint64_t frame = 0;
    uint64_t ident[3] = {0, 0, 0};

    if (read_fn(ops->ctx, clip_a + 0x48, &f48_a, 1) != 1 || f48_a != 1)
        return 0;
    if (read_fn(ops->ctx, obj + 0x48, &f48_b, 1) != 1 || f48_b != 1)
        return 0;
    if (read_fn(ops->ctx, clip_c + 0x48, &f48_c, 1) != 1 || f48_c != 1)
        return 0;
    if (read_fn(ops->ctx, clip_a + 0xe0, &fe0, 1) != 1 || fe0 != 1)
        return 0;
    if (read_fn(ops->ctx, clip_a + 0xe7, &fe7, 2) != 1 || fe7 != 0)
        return 0;
    if (read_fn(ops->ctx, clip_a + 0x280, &f280, 2) != 1 || f280 != 0)
        return 0;
    if (read_fn(ops->ctx, clip_a + 0x27e, &f27e, 1) != 1 || f27e != 0)
        return 0;
    if (read_fn(ops->ctx, clip_a + 0x238, &scale, 4) != 1 || scale != 1.0f)
        return 0;
    if (read_fn(ops->ctx, clip_a + 0xc0, &frame, 8) != 1)
        return 0;
    if ((float)(uint32_t)frame != *(float *)&record[5]
        || *(float *)((uint8_t *)&frame + 4) != *(float *)((uint8_t *)record + 0x2c))
        return 0;
    if (read_fn(ops->ctx, obj + 0x10, ident, 0x18) != 1)
        return 0;
    if (ident[0] != 0x3f800000 || ident[1] != 0x3f80000000000000ULL
        || ident[2] != 0)
        return 0;

    if (record[3] != 0) {
        /* mode != 0: the remote link target of clip_a is the active clip */
        if (remote_widget_link_check((const remote_record_t *)ops, clip_a,
                                     obj) == 0)
            return 0;
    } else {
        uint64_t parent = 1; /* prefill: a failed read must not pass */
        int32_t idx = 0;
        if (read_fn(ops->ctx, clip_a + 0x38, &parent, 8) != 1
            || clip_a >= 0xffffffffffffffbbULL || parent != 0
            || read_fn(ops->ctx, clip_a + 0x40, &idx, 4) != 1
            || idx != -1)
            return 0;
    }

    uint16_t child_id = 0;
    if (read_fn(ops->ctx, clip_c + 0x4e, &child_id, 2) != 1
        || child_id != index)
        return 0;

    uint64_t owner = 0;
    if (read_fn(ops->ctx, clip_a + 0x30, &owner, 8) != 1 || owner != root)
        return 0;
    if (read_fn(ops->ctx, obj + 0x30, &owner, 8) != 1 || owner != root)
        return 0;
    if ((clip_c + 0x38) >> 3 >= 0x201)
        return 0;
    if (read_fn(ops->ctx, clip_c + 0x30, &owner, 8) != 1 || owner != root)
        return 0;
    return 1;
}

/* ===== widget ancestor depth check ===== */

/*
 * widget_ancestor_depth_check — verify widget reaches target within
 * depth parent levels under the given ctx gate: ctx must be the UI
 * context (or NULL), the widget vtable must be game+0x11abcf0, the link
 * to ctx must validate, and the child id at widget+0x4e must equal
 * expect_id. Returns 1 when the widget resolves. @ 00181264
 */
int widget_ancestor_depth_check(uint64_t ctx, uint64_t widget,
                                uint64_t target, int expect_id)
{
    if (target == 0 || ctx != g_ui_context)
        return 0;

    uint64_t vtable = 0;
    if (!is_plausible_ptr_mb(widget + 8)
        || g_host_read(g_host_ctx, widget, &vtable, 8) != 1
        || !is_plausible_ptr_mb(vtable) || (vtable & 7) != 0
        || vtable != g_game_base + 0x11abcf0)
        return 0;

    if (widget_child_link_valid(widget, g_ui_context) == 0)
        return 0;

    uint16_t id = 0;
    if (widget + 0x50 >= 0x1000
        || g_host_read(g_host_ctx, widget + 0x4e, &id, 2) != 1)
        return 0;
    return id == expect_id;
}

/* ===== widget registry resolve ===== */

/*
 * game_vt_for_kind — map the registry kind to the engine vtable rva
 * (kind 1 -> 0x11c0a48, 2 -> 0x11c0c58, 3 -> 0x11abcf0, else
 * 0x11ad208).
 */
static uint64_t game_vt_for_kind(int kind)
{
    switch (kind) {
    case 1:  return 0x11c0a48;
    case 2:  return 0x11c0c58;
    case 3:  return 0x11abcf0;
    default: return 0x11ad208;
    }
}

/*
 * widget_registry_resolve — resolve a widget slot into the page registry:
 * the widget must be non-NULL, not one of the identity roots ({scene
 * root, UI context, page ctx (page+0x180*8+8? no: page[0x181])}, {view_a,
 * view_b}), not present in the menu entry records (0..index-1, 0xd8
 * stride, widgets at +0/+8/+0x18) nor in the page's already-resolved
 * slots (0x20-byte entries, pointer at +0), and its vtable must match
 * the kind class (1: 0x11c0a48, 2: 0x11c0c58, 3: 0x11abcf0, else
 * 0x11ad208). Its scene owner (+0x30) must be NULL or the scene root.
 * kind 4 checks detachment (widget_detached_check), others the ctx child
 * link; extra (when set) must hold vtable game+0x11abad8 and reach the
 * widget (widget_ancestor_of). kind 1 runs widget_kind1_validate; kind 2
 * reads the +0x160/+0x168 array window (aligned, <= 0x20 bytes) and
 * every entry must hold vtable game+0x11ad208 and reach the widget;
 * kind 4 runs widget_tree_walk_collect. On success the slot is appended
 * to the page ({slot, ctx, kind, extra}) and the count at page+0x600
 * (raw +0x180 qwords) is bumped. Returns 1 when resolved.
 * @ 001813b8
 */
int widget_registry_resolve(void *page_v, uint64_t *slot, uint64_t ctx,
                            int kind, uint64_t extra)
{
    uint64_t *page = (uint64_t *)page_v;
    uint64_t w = slot != NULL ? *slot : 0;
    uint32_t count;

    if (slot == NULL || w == 0)
        return 0;
    count = *(uint32_t *)(page + 0x180);
    if (count > 0x5f)
        return 0; /* registry full (96 slots) */

    if (w == g_ui_scene_root || w == g_ui_context
        || w == page[0x181] || w == g_view_object_a || w == g_view_object_b)
        return 0;

    /* the menu entry records must not own w */
    if (g_menu_entry_index != 0) {
        uint32_t left = (uint32_t)g_menu_entry_index;
        const uint8_t *rec = g_menu_entry_table;
        do {
            if (w == *(const uint64_t *)rec
                || w == *(const uint64_t *)(rec + 8)
                || w == *(const uint64_t *)(rec + 0x18))
                return 0;
            rec += 0xd8;
            left--;
        } while (left != 0);
    }

    /* already-resolved page slots must not contain w */
    if (count != 0) {
        uint64_t *entry = page;
        do {
            if (w == *entry)
                return 0;
            count--;
            entry += 4;
        } while (count != 0);
        count = *(uint32_t *)(page + 0x180);
    }

    uint64_t want_vt = game_vt_for_kind(kind);
    uint64_t vtable = 0;
    if (!is_plausible_ptr_mb(w + 8)
        || g_host_read(g_host_ctx, w, &vtable, 8) != 1
        || !is_plausible_ptr_mb(vtable) || (vtable & 7) != 0
        || vtable != g_game_base + want_vt)
        return 0;

    uint64_t owner = 0;
    if (!is_plausible_ptr_mb(w + 0x38)
        || g_host_read(g_host_ctx, w + 0x30, &owner, 8) != 1
        || (owner != 0 && owner != g_ui_scene_root))
        return 0;

    if (kind == 4) {
        if (widget_detached_check(w) == 0)
            return 0;
    } else {
        if (widget_child_link_valid(w, ctx) == 0)
            return 0;
    }

    if (extra != 0
        && (remote_vtable_matches(extra, 0x11abad8) == 0
            || widget_ancestor_of(extra, w) == 0))
        return 0;

    if (kind == 1) {
        uint64_t snapshot[0xb];
        if (widget_kind1_validate(w, ctx, snapshot) == 0)
            return 0;
    } else if (kind == 2) {
        uint64_t arr_lo = widget_field_ptr_read(w + 0x160);
        uint64_t arr_hi = widget_field_ptr_read(w + 0x168);
        uint64_t inner = widget_field_ptr_read(w + 0x128);
        if (inner == 0 || widget_ancestor_of(inner, w) == 0
            || arr_lo == 0 || arr_hi < arr_lo
            || arr_hi - arr_lo > 0x20 || ((arr_hi - arr_lo) & 7) != 0)
            return 0;
        for (uint64_t p = arr_lo; p < arr_hi; p += 8) {
            uint64_t entry = widget_field_ptr_read(p);
            if (remote_vtable_matches(entry, 0x11ad208) == 0
                || widget_ancestor_of(entry, w) == 0)
                return 0;
        }
    } else if (kind == 4) {
        if (widget_tree_walk_collect(w) == 0)
            return 0;
    }

    /* append {slot, ctx, kind, extra} and bump the count */
    uint64_t *entry = page + (uint64_t) * (uint32_t *)(page + 0x180) * 4;
    *(uint32_t *)(page + 0x180) = count + 1;
    entry[0] = (uint64_t)slot;
    entry[1] = ctx;
    *(int32_t *)&entry[2] = kind;
    entry[3] = extra;
    return 1;
}

/*
 * theme_preview_active_check — a theme preview is active when none is
 * cached (g_theme_preview_obj == 0, true) or the cached preview is still
 * linked under its context (theme_preview_linked). @ 00181754
 */
int theme_preview_active_check(void)
{
    if (g_theme_preview_obj != 0)
        return theme_preview_linked(g_theme_preview_obj,
                                    g_theme_preview_ctx);
    return 1;
}
/* misc_b chain chunk 12: covers raw lines 15681-16150 */

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

/* ---- shared host/service globals (digest-mandated names) ---- */
extern void *g_host_ctx;                                      /* 0x1a7cc8 */
extern int (*g_host_read)(void *ctx, uint64_t addr, void *out,
                          uint32_t len);                      /* 0x1a7ce8 */
extern uint64_t g_game_base;                                  /* 0x1a7cd0 */
extern uint64_t g_ui_scene_root;                              /* 0x1a7d28 */
extern uint64_t g_ui_context;                                 /* 0x1a7d30 */

/* ---- own earlier chunks (b-chain) ---- */
extern int is_plausible_ptr_mb(uint64_t v);                   /* misc.c (b-chain) */
extern int widget_child_link_valid(uint64_t node,
                                   uint64_t parent);          /* misc.c (b-chain) @ 0016d5cc */
extern void engine_call_5d7c30(uint64_t a, uint64_t b);       /* misc.c (b-chain) @ 0016ce84 */
extern void engine_call_594e80(uint64_t a, uint64_t b);       /* misc.c (b-chain) @ 0016ce20 */
extern void widget_set_position(uint64_t widget, float x, float y);  /* @ 0016cf5c */
extern int ui_scene_fonts_ready(void);                        /* misc.c (b-chain) @ 0016d910 */

/* ---- own later chunks (b-chain) ---- */
extern int engine_text_integrity_ok(void);                    /* misc.c (b-chain) @ 00182de4 (chunk 13) */
extern int feature_version_supported(const char *name, uint32_t version,
                                     int kind);               /* misc.c (b-chain) @ 00182fd8 (chunk 13) */
extern int theme_preview_ready(uint64_t widget);              /* misc.c (b-chain) @ 00182848 (chunk 13) */

/* ===== widget tree walk ===== */

/*
 * widget_tree_walk_collect — queue-walk the widget tree from node toward
 * the UI context: each queued node must reach the UI context within 16
 * validated parent levels (movie-class 0x11ad208 or clip-class
 * 0x11abcf0 vtable); movie/clip nodes then enqueue their children (child
 * count at +0x4e, array at +0x50; movie uses the +0xc0/+0x90 pair on the
 * second pass, limit 0x200) with dedup against the 512-slot queue.
 * Returns 1 when the whole walk completed, 0 when it hits a broken link,
 * an invalid child count/array or an overlong queue. @ 00181790
 */
int widget_tree_walk_collect(uint64_t node)
{
    uint64_t queue[512];
    uint64_t count = 1;
    queue[0] = node;

    for (uint64_t qi = 0; qi < count; qi++) {
        uint64_t cur = queue[qi];

        if (cur == 0)
            return 0;

        /* the node must reach the UI context within 16 parent levels */
        {
            uint64_t w = cur;
            uint32_t depth = 0;
            for (;;) {
                if (w == g_ui_context || depth > 0xf)
                    break;
                uint64_t parent_raw = 0;
                int rd = is_plausible_ptr_mb(w + 0x40)
                         && g_host_read(g_host_ctx, w + 0x38, &parent_raw, 8) == 1;
                int parent_ok = rd && is_plausible_ptr_mb(parent_raw)
                                && (parent_raw & 7) == 0;
                uint64_t parent = parent_ok ? parent_raw : 0;
                if (widget_child_link_valid(w, parent) == 0)
                    return 0;
                depth++;
                w = parent_raw;
                if (!parent_ok) {
                    w = 0;
                    break;
                }
            }
            if (w != g_ui_context)
                return 0;
        }

        uint64_t vtable = 0;
        if (!is_plausible_ptr_mb(cur + 8)
            || g_host_read(g_host_ctx, cur, &vtable, 8) != 1
            || !is_plausible_ptr_mb(vtable) || (vtable & 7) != 0)
            return 0;

        int is_movie = vtable == g_game_base + 0x11ad208;
        if (!is_movie && vtable != g_game_base + 0x11abcf0)
            continue; /* other classes: no children to walk */

        /* two passes: pass 0 reads +0x4e/+0x50, pass 1 +0xc0/+0x90 */
        for (uint32_t pass = 0; pass < 2; pass++) {
            uint64_t count_off = pass == 0 ? 0x4e : 0xc0;
            uint64_t array_off = pass == 0 ? 0x50 : 0x90;

            int16_t n = 0;
            if (!is_plausible_ptr_mb(cur + count_off + 2)
                || g_host_read(g_host_ctx, cur + count_off, &n, 2) != 1
                || n < 0 || n > 0x200)
                return 0;
            if (n == 0)
                continue;

            uint64_t children = 0;
            if (!is_plausible_ptr_mb(cur + array_off + 8)
                || g_host_read(g_host_ctx, cur + array_off, &children, 8) != 1)
                return 0;

            for (int32_t c = 0; c < n; c++) {
                uint64_t child = 0;
                uint64_t slot = children + (uint64_t)c * 8;
                if (!is_plausible_ptr_mb(slot + 8)
                    || g_host_read(g_host_ctx, slot, &child, 8) != 1)
                    return 0;
                if (child == 0)
                    continue;

                /* dedup against the queue */
                uint32_t found = 0;
                for (uint64_t q = 0; q < count; q++) {
                    if (queue[q] == child) {
                        found = 1;
                        break;
                    }
                }
                if (found)
                    continue;
                if (count == 0x200)
                    return 0; /* queue full */
                queue[count++] = child;
            }
        }
    }
    return 1;
}

/* ===== theme preview linkage ===== */

/*
 * theme_preview_linked — the preview widget is still linked under its
 * context: ctx must be the UI context, the preview vtable must be
 * game+0x11ad208, its scene owner (+0x30) NULL or the scene root, the ctx
 * child link valid, and the flag byte at +0x48 clear. Returns 1 when
 * linked. @ 00181b78
 */
int theme_preview_linked(uint64_t preview, uint64_t ctx)
{
    if (ctx != g_ui_context)
        return 0;

    uint64_t vtable = 0;
    if (!is_plausible_ptr_mb(preview + 8)
        || g_host_read(g_host_ctx, preview, &vtable, 8) != 1
        || !is_plausible_ptr_mb(vtable) || (vtable & 7) != 0
        || vtable != g_game_base + 0x11ad208)
        return 0;

    uint64_t owner = 0;
    if (!is_plausible_ptr_mb(preview + 0x38)
        || g_host_read(g_host_ctx, preview + 0x30, &owner, 8) != 1
        || (owner != 0 && owner != g_ui_scene_root))
        return 0;

    if (widget_child_link_valid(preview, ctx) == 0)
        return 0;

    uint8_t flag = 0xff; /* prefill: a failed read must not pass */
    if (preview + 0x49 < 0x1000)
        return 0;
    if (g_host_read(g_host_ctx, preview + 0x48, &flag, 1) != 1)
        return 0;
    return flag == 0;
}

/* ===== theme list fetch ===== */

/*
 * theme_list_fetch_tick — advance the theme list fetch one tick: the
 * engine text integrity gate must pass, state+4 must carry the low two
 * bits clear and state[0] < 1000000. The theme name (0x60 at state+0x68)
 * must be NUL-terminated, start with "sc/" and contain no "..", and the
 * status text (state+0xc8) non-empty. The name is pre-loaded through the
 * engine (0x51f074 with the first 8 bytes, then 0x75da50), the bundle
 * object resolved (0x51e62c) and the list parsed into state (0x5dd3d0).
 * Returns 1 on success, -1 on failure, 0 when nothing was fetched.
 * @ 00181d04
 */
int theme_list_fetch_tick(uint32_t *state)
{
    if (engine_text_integrity_ok() == 0)
        return -1;
    if ((state[1] ^ 0xffffffffu) & 3)
        return -1;
    if (state[0] >= 1000000)
        return -1;

    const char *name = (const char *)&state[0x1a];
    if (memchr(name, 0, 0x60) == NULL)
        return -1;

    uint32_t *status = &state[0x32];
    if (memchr(status, 0, 0x60) == NULL)
        return -1;
    if (strncmp(name, "sc/", 3) != 0)
        return -1;
    if (strstr(name, "..") != NULL)
        return -1;
    if (*(char *)status == '\0')
        return -1;

    /* pre-load the bundle name through the engine (first 8 bytes) */
    char head_buf[8];
    const void *head = name;
    size_t len = strlen(name);
    if (len < 8) {
        memset(head_buf, 0, sizeof head_buf);
        memcpy(head_buf, name, len);
        head = head_buf;
    }

    int64_t head_words[1];
    memcpy(head_words, head, 8);
    ((void (*)(int64_t *, int))(g_game_base + 0x51f074))(head_words, 0);
    ((void (*)(int64_t *, int))(g_game_base + 0x75da50))(head_words, 0);

    uint64_t bundle = ((uint64_t (*)(const char *, int))(
        g_game_base + 0x51e62c))(name, 0);
    if (bundle != 0) {
        int r = ((int (*)(uint64_t, uint32_t *))(
            g_game_base + 0x5dd3d0))(bundle, status);
        return r == 0 ? -1 : 1;
    }
    return -1;
}

/* ===== theme preview creation ===== */

/*
 * theme_preview_create — create a theme preview from the theme state
 * record: the preview object is created through the engine (base
 * + 0xd1763c, state+0x68/name and state+0xc8/status), validated as a
 * detached movie-class widget (vtable game+0x11ad208, parent NULL, index
 * -1, owner NULL or the scene root), reset (engine_call_5d7c30,
 * engine_call_594e80, parked at (-10000, -10000)) and then — when
 * deep_check != 0 — the style child at +0x80 is resolved: its name (up to
 * 0x60 bytes) must not be "bgr_start_thin..." (the thin-start style),
 * then its children (count +0xc0 <= 0x1000, array +0x90) are each
 * validated (movie-class, parent is the child, count 0..0x1000, kind
 * byte <= 1) and those whose name is not "bg_color"/"bg_pattern" are
 * feature-checked (feature_version_supported) with the result stored at
 * child+8. Returns the preview widget, or 0 on failure (the raw releases
 * the object through engine 0x5d48e8 on failure). @ 00181ea0
 */
uint64_t theme_preview_create(void *state, int deep_check)
{
    uint64_t preview =
        ((uint64_t (*)(void *, void *, int))(g_game_base + 0xd1763c))(
            (uint8_t *)state + 0x68, (uint8_t *)state + 0xc8, 0);
    if (preview == 0)
        return 0;

    uint64_t vtable = 0;
    if (!is_plausible_ptr_mb(preview + 8)
        || g_host_read(g_host_ctx, preview, &vtable, 8) != 1
        || !is_plausible_ptr_mb(vtable) || (vtable & 7) != 0
        || vtable != g_game_base + 0x11ad208)
        goto fail;

    {
        uint64_t parent = 0;
        int32_t index = 0;
        uint64_t owner = 0;
        if (!is_plausible_ptr_mb(preview + 0x40)
            || g_host_read(g_host_ctx, preview + 0x38, &parent, 8) != 1
            || parent != 0
            || !is_plausible_ptr_mb(preview + 0x44)
            || g_host_read(g_host_ctx, preview + 0x40, &index, 4) != 1
            || index != -1
            || !is_plausible_ptr_mb(preview + 0x38)
            || g_host_read(g_host_ctx, preview + 0x30, &owner, 8) != 1
            || (owner != 0 && owner != g_ui_scene_root))
            goto fail;
    }

    engine_call_5d7c30(preview, 0);
    engine_call_594e80(preview, 0);
    widget_set_position(preview, -10000.0f, -10000.0f); /* 0xc61c3c00 */

    if (deep_check == 0)
        return preview;

    /* deep check: resolve the style child chain at preview+0x80 */
    {
        uint64_t style = 0;
        if (!is_plausible_ptr_mb(preview + 0x88)
            || g_host_read(g_host_ctx, preview + 0x80, &style, 8) != 1
            || !is_plausible_ptr_mb(style) || (style & 7) != 0
            || style == 0)
            goto fail;

        char name[0x60];
        uint64_t cur = style;
        for (uint64_t off = 0; off < 0x60; off++) {
            if (cur + off + 1 < 0x1000)
                goto fail;
            if (g_host_read(g_host_ctx, cur + off, &name[off], 1) != 1)
                goto fail;
            if (name[off] == '\0') {
                if (off == 0)
                    goto fail;
                /* the thin-start style child is followed one level deeper */
                uint64_t child = preview;
                if (memcmp(name, "bgr_start_thin", 15) == 0) {
                    engine_call_5d7c30(preview, 0);
                    uint64_t style2 = 0;
                    if (preview + 0x90 < 0x1000
                        || g_host_read(g_host_ctx, preview + 0x90, &style2, 8) != 1
                        || !is_plausible_ptr_mb(style2) || (style2 & 7) != 0)
                        goto fail;
                    uint64_t style3 = 0;
                    if (!is_plausible_ptr_mb(style2)
                        || g_host_read(g_host_ctx, style2, &style3, 8) != 1
                        || !is_plausible_ptr_mb(style3) || (style3 & 7) != 0
                        || style3 < 0x1000
                        || style3 != vtable - 0x11ad208 + g_game_base
                                           + 0x11ad208 * 0 + vtable)
                        goto fail;
                    child = style3;
                }

                int16_t count = 0;
                if (!is_plausible_ptr_mb(child + 0xc2)
                    || g_host_read(g_host_ctx, child + 0xc0, &count, 2) != 1
                    || count < 0 || count > 0x1000)
                    goto fail;
                if (count == 0)
                    return preview;

                uint64_t children = 0;
                if (child + 0x98 < 0x1000
                    || g_host_read(g_host_ctx, child + 0x90, &children, 8) != 1
                    || !is_plausible_ptr_mb(children) || (children & 7) != 0)
                    goto fail;
                if (count < 2)
                    return preview;

                /* validate every style child and feature-check its name */
                uint64_t feature_slots[0x1000];
                uint8_t feature_ok[0x1000];
                uint32_t n_ok = 0;
                for (int32_t i = 0; i < count; i++) {
                    uint64_t entry = 0;
                    uint64_t names = 0;
                    int16_t entry_count = 0;
                    uint8_t kind = 0;

                    uint64_t slot = children + (uint64_t)i * 8;
                    if (!is_plausible_ptr_mb(slot + 8)
                        || g_host_read(g_host_ctx, slot, &entry, 8) != 1)
                        goto fail;
                    if (entry == 0)
                        continue;

                    uint64_t entry_vt = 0;
                    if (!is_plausible_ptr_mb(entry + 8)
                        || g_host_read(g_host_ctx, entry, &entry_vt, 8) != 1
                        || !is_plausible_ptr_mb(entry_vt)
                        || (entry_vt & 7) != 0
                        || entry_vt != vtable)
                        goto fail;

                    uint64_t entry_parent = 0;
                    if (entry + 0x38 < 0x1000
                        || g_host_read(g_host_ctx, entry + 0x38, &entry_parent, 8) != 1
                        || (entry_parent != 0 && entry_parent != child))
                        goto fail;

                    if (entry + 0xc2 < 0x1000
                        || g_host_read(g_host_ctx, entry + 0xc0, &entry_count, 2) != 1
                        || entry_count < 0)
                        goto fail;
                    if (entry + 9 < 0x1000
                        || g_host_read(g_host_ctx, entry + 8, &kind, 1) != 1
                        || kind > 1)
                        goto fail;

                    if (children != 0) {
                        uint64_t name_slot = children + (uint64_t)i * 8;
                        if (!is_plausible_ptr_mb(name_slot + 8)
                            || g_host_read(g_host_ctx, name_slot, &names, 8) != 1)
                            goto fail;
                    }

                    char entry_name[0x80];
                    if (names == 0) {
                        entry_name[0] = '\0';
                    } else {
                        for (uint64_t o = 0; o < 0x80; o++) {
                            if (names + o + 1 < 0x1000)
                                goto fail;
                            if (g_host_read(g_host_ctx, names + o,
                                            &entry_name[o], 1) != 1)
                                goto fail;
                            if (entry_name[o] == '\0')
                                break;
                        }
                    }

                    if (memcmp(entry_name, "bg_color", 8) != 0
                        && memcmp(entry_name, "bg_pattern", 10) != 0) {
                        feature_slots[n_ok] = entry;
                        int supported = feature_version_supported(
                            name, (uint32_t)i, entry_count);
                        feature_ok[n_ok] = supported == 0;
                        n_ok++;
                    }
                }

                /* store the feature-check results at child+8 */
                for (uint32_t i = 0; i < n_ok; i++)
                    *(char *)(feature_slots[i] + 8) = (char)feature_ok[i];
                return preview;
            }
        }
    }

fail:
    ((void (*)(uint64_t))(g_game_base + 0x5d48e8))(preview);
    return 0;
}
/* misc_b chain chunk 13: covers raw lines 16151-16550 */

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

/* ---- shared host/service globals (digest-mandated names) ---- */
extern void *g_host_ctx;                                      /* 0x1a7cc8 */
extern int (*g_host_read)(void *ctx, uint64_t addr, void *out,
                          uint32_t len);                      /* 0x1a7ce8 */
extern uint64_t g_game_base;                                  /* 0x1a7cd0 */
extern uint64_t g_ui_scene_root;                              /* 0x1a7d28 */
extern uint64_t g_ui_context;                                 /* 0x1a7d30 */
extern float g_ui_density;                                    /* 0x1e53fc */
extern float g_ui_center_x;                                   /* 0x1e5400 */
extern float g_ui_center_y;                                   /* 0x1e5404 */

/* ---- own earlier chunks (b-chain) ---- */
extern int is_plausible_ptr_mb(uint64_t v);                   /* misc.c (b-chain) */
extern int widget_child_link_valid(uint64_t node,
                                   uint64_t parent);          /* misc.c (b-chain) @ 0016d5cc */
extern int ui_scene_fonts_ready(void);                        /* misc.c (b-chain) @ 0016d910 */
extern int theme_preview_linked(uint64_t preview, uint64_t ctx); /* misc.c (b-chain) @ 00181b78 */
extern void widget_set_scale(uint64_t widget, float sx, float sy);   /* @ 0016cf00 */
extern void widget_set_position(uint64_t widget, float x, float y);  /* @ 0016cf5c */
extern void widget_set_anchor(uint64_t widget, void *anchor_out);    /* @ 0016cf84 */
extern void engine_call_5988ec(uint64_t a, uint64_t b);       /* misc.c (b-chain) @ 0016cfb8 */

/* ---- own later chunks (b-chain) ---- */
extern int engine_text_integrity_ok(void);                    /* misc.c (b-chain) @ 00182de4 (below) */

/* ---- rodata ---- */
extern const float g_anchor_tolerance[4];  /* 0x10f9f0 — anchor extent compare tolerance (NEON pair) */

/* ===== theme preview attach / ready / orphaned ===== */

/*
 * theme_preview_attach — attach the preview widget to the UI context:
 * ctx must be the UI context and pass ui_scene_fonts_ready, the preview
 * must read detached (parent NULL, index -1), then the context is
 * parked via engine_call_5988ec (scene root, preview), the ctx child
 * link validated and the preview's flag byte at +0x48 must read clear.
 * Returns 1 when attached. @ 001826ec
 */
int theme_preview_attach(uint64_t ctx, uint64_t preview)
{
    if (ctx != g_ui_context)
        return 0;
    if (ui_scene_fonts_ready() == 0)
        return 0;

    uint64_t parent = 0;
    int32_t index = 0;
    if (!is_plausible_ptr_mb(preview + 0x40)
        || g_host_read(g_host_ctx, preview + 0x38, &parent, 8) != 1
        || parent != 0
        || !is_plausible_ptr_mb(preview + 0x44)
        || g_host_read(g_host_ctx, preview + 0x40, &index, 4) != 1
        || index != -1)
        return 0;

    engine_call_5988ec(g_ui_scene_root, preview);

    if (widget_child_link_valid(preview, ctx) == 0)
        return 0;

    uint8_t flag = 0xff; /* prefill: a failed read must not pass */
    if (preview + 0x49 < 0x1000)
        return 0;
    if (g_host_read(g_host_ctx, preview + 0x48, &flag, 1) != 1)
        return 0;
    return flag == 0;
}

/*
 * theme_preview_ready — verify the preview widget is laid out ready for
 * display: the widget is scaled 1:1 and its anchor/extent pair read
 * (NaN/Inf-gated, non-zero extent), the scene-root stage box
 * (+0x3c..+0x48: {0, 0, w, h}) must be finite with positive w/h, and the
 * design density/centers sane. The fit scale
 * min((w/density)/ext_w, (h/density)/ext_h) must land in
 * [0.0001, 128]; the widget is then rescaled and centered at
 * (cx - w/2, cy - h/2)/density, its anchor re-read and the placement
 * verified (anchor offset > 0, within (w/density)+0.5). Returns 1 when
 * the preview is on-screen and the scene passes
 * ui_scene_fonts_ready. @ 00182848
 */
int theme_preview_ready(uint64_t widget)
{
    float extent[4]; /* {ax, ay, ex, ey} — anchor and extent */

    widget_set_scale(widget, 1.0f /* 0x3f800000 */, 1.0f);
    widget_set_anchor(widget, extent);

    float ext_w = extent[2] - extent[0];
    float ext_h = extent[3] - extent[1];
    if (!isfinite(ext_w) || isnan(ext_w) || !isfinite(ext_h) || isnan(ext_h))
        return 0;
    if (!(fabsf(ext_w) > g_anchor_tolerance[0]
          && fabsf(ext_w) < g_anchor_tolerance[2])
        && !(fabsf(ext_h) > g_anchor_tolerance[1]
             && fabsf(ext_h) < g_anchor_tolerance[3]))
        return 0; /* zero/near-zero extent (NEON tolerance pair) */

    float stage[4]; /* scene root +0x3c: {0, 0, w, h} */
    if (g_ui_scene_root + 0x4c <= 0x101 * 16)
        return 0;
    if (g_host_read(g_host_ctx, g_ui_scene_root + 0x3c, stage, 0x10) != 1)
        return 0;
    if (stage[0] != 0.0f || stage[1] != 0.0f
        || !(stage[2] > 0.0f) || !(stage[3] > 0.0f))
        return 0;

    if (!isfinite(g_ui_density) || isnan(g_ui_density) || g_ui_density <= 0.0f)
        return 0;
    if (!isfinite(g_ui_center_x) || isnan(g_ui_center_x))
        return 0;
    if (!isfinite(g_ui_center_y) || isnan(g_ui_center_y))
        return 0;

    float sw = stage[2] / g_ui_density;
    float sh = stage[3] / g_ui_density;
    float scale = fminf(sw / ext_w, sh / ext_h);
    if (!isfinite(scale) || isnan(scale) || scale < 0.0001f
        || scale > 128.0f)
        return 0;

    float pos_y = sh * -0.5f + g_ui_center_y / g_ui_density;
    float pos_x = sw * -0.5f + g_ui_center_x / g_ui_density;

    widget_set_scale(widget, scale, scale);
    widget_set_position(widget, pos_x, pos_y);
    widget_set_anchor(widget, extent);

    float off_x = extent[2] - extent[0];
    float off_y = extent[3] - extent[1];
    if (!isfinite(off_x) || isnan(off_x) || !isfinite(off_y) || isnan(off_y))
        return 0;
    if (!(off_x > 0.0f) || !(off_y > 0.0f))
        return 0;
    if (off_x > sw + 0.5f || off_y > sh + 0.5f)
        return 0;

    return ui_scene_fonts_ready() != 0;
}

/*
 * theme_preview_orphaned — check whether the preview widget went stale:
 * the engine text integrity gate must pass, the preview must be a
 * movie-class widget (vtable game+0x11ad208) owned by nothing or the
 * scene root; when it is still attached (parent != NULL) it must remain
 * linked (theme_preview_linked), then it is detached through engine
 * 0x59567c; a still-attached widget after the detach is stale. A
 * detached preview is released through engine 0x5d48e8. Returns 1 when
 * the preview was released (stale), 0 when it is still live.
 * @ 00182b58
 */
int theme_preview_orphaned(uint64_t preview, uint64_t ctx)
{
    if (engine_text_integrity_ok() == 0)
        return 0;

    uint64_t vtable = 0;
    if (!is_plausible_ptr_mb(preview + 8)
        || g_host_read(g_host_ctx, preview, &vtable, 8) != 1
        || !is_plausible_ptr_mb(vtable) || (vtable & 7) != 0
        || vtable != g_game_base + 0x11ad208)
        return 0;

    uint64_t owner = 0;
    if (!is_plausible_ptr_mb(preview + 0x38)
        || g_host_read(g_host_ctx, preview + 0x30, &owner, 8) != 1
        || (owner != 0 && owner != g_ui_scene_root))
        return 0;

    uint64_t parent = 0;
    int32_t index = 0;
    int attached = 1;
    if (is_plausible_ptr_mb(preview + 0x40)
        && g_host_read(g_host_ctx, preview + 0x38, &parent, 8) == 1
        && parent == 0
        && is_plausible_ptr_mb(preview + 0x44)
        && g_host_read(g_host_ctx, preview + 0x40, &index, 4) == 1
        && index == -1)
        attached = 0; /* already detached: release below */

    if (attached) {
        if (theme_preview_linked(preview, ctx) == 0)
            return 0;
        ((void (*)(uint64_t))(g_game_base + 0x59567c))(preview);

        /* still attached after the detach: stale */
        parent = 0;
        index = 0;
        if (is_plausible_ptr_mb(preview + 0x40)
            && g_host_read(g_host_ctx, preview + 0x38, &parent, 8) == 1
            && parent == 0
            && g_host_read(g_host_ctx, preview + 0x40, &index, 4) == 1
            && index == -1) {
            /* detached now: fall through to release */
        } else {
            return 0;
        }
    }

    ((void (*)(uint64_t))(g_game_base + 0x5d48e8))(preview);
    return 1;
}

/* ===== engine text integrity gate ===== */

/*
 * engine_text_integrity_ok — cached integrity gate for the theme/text
 * engine path: while the cache at 0x284678 is unset, SHA-256 verify the
 * thirteen engine text regions (0x51f074, 0x75da50, 0x51e62c, 0x5dd3d0,
 * 0xd1763c, 0x594138, 0x5988ec, 0x59567c, 0x5d48e8, 0x5d7c30, 0x595378,
 * 0x59533c, 0x595344, 0x595314, capped by 0x594e80) and cache 1/-1.
 * Returns the cached verdict (nonzero when verified). @ 00182de4
 */
int engine_text_integrity_ok(void)
{
    extern int32_t g_theme_text_guards; /* 0x284678 — theme engine integrity cache */

    if (g_theme_text_guards == 0) {
        if (code_region_hash_verify(0x51f074, 0x13c,
            "9967d384457781c5ed13ed01ee77994e84ac16d941bf84600b1dba2f608c2fc0") == 0
            || code_region_hash_verify(0x75da50, 0x7c,
            "d7097026eaaede752ff10f7f2503fc76a6c5ee064617b30359964bb70aeda490") == 0
            || code_region_hash_verify(0x51e62c, 0x1b0,
            "a7505857cc0b9c3a6f072e8a489eefcf2ce6640801405461d43ec8aea1f1d78f") == 0
            || code_region_hash_verify(0x5dd3d0, 0x134,
            "aecbacc988140fd316ebf71b15026686841694b5669ed1e87c10c6509d25e700") == 0
            || code_region_hash_verify(0xd1763c, 0x6c,
            "27f8452a4e51f9f0c5ac16e43ba77488dd95391d3e006f1cd31a0bbc9084b878") == 0
            || code_region_hash_verify(0x594138, 0x234,
            "e9799904da24c81159b38ac4f53d167b65aeb46496dae806e7fe6e340c63e87f") == 0
            || code_region_hash_verify(0x5988ec, 8,
            "f04a1fc14120f8ef9e56a6bfaffa9a957006937226bd5ad4ccf43b6e6bbcac66") == 0
            || code_region_hash_verify(0x59567c, 0x20,
            "9e18529b88a6285f2675ad92537b12875a9735c6fc16cbdc947b58b5e235b5f9") == 0
            || code_region_hash_verify(0x5d48e8, 0x8c,
            "20951e09482741d88a2c79792b49f48689dea51fde868582e629a3a73e99a4ac") == 0
            || code_region_hash_verify(0x5d7c30, 0x70,
            "289189be747ebdef62c061b87e11ca33589e787ed4648e7ef3cd72a4008682ce") == 0
            || code_region_hash_verify(0x595378, 0x78,
            "26dd902aec11222432647f202e43e8c10add7f8359f6b5d9b3b260378524f74b") == 0
            || code_region_hash_verify(0x59533c, 8,
            "47c05c261ebe1823420dd2d8496ecd0f068d47a15b4d069011c964f33adc75e7") == 0
            || code_region_hash_verify(0x595344, 8,
            "0e7d6896adc47cf1b6cf69345698abcd462544086684a596d8f6a85ea9374adb") == 0
            || code_region_hash_verify(0x595314, 8,
            "e422ec3b09a5625f93137b982a59bf96b5426bcbc1090633ddb89812444c9691") == 0) {
            g_theme_text_guards = -1;
        } else {
            int ok = code_region_hash_verify(0x594e80, 0x60,
                "fdd9a09d4b77013ddd856b9e42ff25272aaacef9d8f0d5d200862083230ab3a0");
            g_theme_text_guards = ok != 0 ? 1 : -1;
        }
    }
    return g_theme_text_guards == 1;
}

/* ===== feature version support ===== */

/*
 * feature_version_supported — look the feature name up in the rodata
 * feature table (0x19cba4, 0x38 entries, 0x10 stride: {name, index,
 * count}): when found, the version must fall inside any of the entry's
 * [min, max] ranges (range table 0x140260, 8-byte {min, max} rows).
 * Unknown names are allowed unless kind is 1, 0x1b0 or 0x6c. Returns 1
 * when supported, 0 otherwise. @ 00182fd8
 */
int feature_version_supported(const char *name, uint32_t version,
                             int kind)
{
    extern const struct {
        const char *name;   /* +0x00 */
        uint32_t index;     /* +0x08 */
        uint32_t count;     /* +0x0c */
    } FEATURE_TABLE[0x38]; /* 0x19cba4 */
    extern const uint32_t FEATURE_RANGES[]; /* 0x140260 — {min, max} pairs */

    for (uint32_t i = 0; i < 0x38; i++) {
        if (strcmp(name, FEATURE_TABLE[i].name) == 0) {
            uint32_t idx = FEATURE_TABLE[i].index;
            uint32_t count = FEATURE_TABLE[i].count;
            if (count < 2)
                count = 1;
            for (uint32_t r = 0; r < count; r++) {
                if (FEATURE_RANGES[idx * 2] <= version
                    && version <= FEATURE_RANGES[idx * 2 + 1])
                    return 1;
                idx++;
            }
            return 0;
        }
    }
    if (kind == 1)
        return 0;
    if (kind == 0x1b0)
        return 0;
    return kind != 0x6c;
}
/* misc_b chain chunk 14: covers raw lines 16564-16899 */

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

/* ---- shared host/service globals (digest-mandated names) ---- */
extern uint64_t g_libg_span;                                  /* 0x10f740 — {base,size} of libg.so (packed) */

/* ---- rodata park values for the pointer state ---- */
extern const uint64_t g_pointer_park_guard;  /* 0x10fa90 — park value for the guard pair */
extern const uint64_t g_pointer_park_hold;   /* 0x10fa98 — park value for the hold pair */

/* ---- pointer/scroll state block (chunk 6 definitions) ---- */
extern float    g_scroll_position;        /* 0x22d518 — inertia state base (18-float array) */
extern uint64_t g_pointer_state_pair;     /* 0x22d51c — {velocity word, hold x} (state[1..3] window) */
extern uint32_t g_pointer_latch;          /* 0x22d530 — pointer latch (state[6]) */
extern float    g_scroll_target;          /* 0x22d560 — touch scroll target */
extern uint32_t g_pointer_active;         /* 0x22d564 — pointer held latch */
extern uint32_t g_pointer_widget;         /* 0x22d568 — pointer in-region flag */
extern uint32_t g_pointer_guard_lo;       /* 0x22d56c — in-hold radius flag (also mirrors libg span lo) */
extern uint32_t g_pointer_guard_hi;       /* 0x22d570 — anchor identity flag (also mirrors span hi) */
extern uint32_t g_pointer_parked;         /* 0x22d574 — park flag */
extern uint64_t g_pointer_hold_pair;      /* 0x22d578 — packed hold frame/id */
extern uint32_t g_pointer_frame_count;    /* 0x22d580 — anchor generation (param_4) */
extern uint32_t g_pointer_seq;            /* 0x22d584 — anchor slot id (0xffffffff on ctx change) */
extern uint64_t g_pointer_event_arg;      /* 0x22d588 — anchor ctx */
extern uint64_t g_pointer_last_frame;     /* 0x22d590 — frame compare window */
extern uint64_t g_pointer_touch_frame;    /* 0x22d5a0 — 500-frame touch window */
extern uint64_t g_pointer_anchor_b0;      /* 0x22d5b0 — anchor snapshot (words 2-3) */
extern uint64_t g_pointer_anchor_b8;      /* 0x22d5b8 — anchor snapshot (x at low half) */
extern uint64_t g_pointer_cached_pair;    /* 0x22d5c0 — cached pair (action word at +4) */
extern uint64_t g_pointer_param_6;        /* 0x22d5c8 — cached record words 6-7 */
extern uint64_t g_pointer_param_8;        /* 0x22d5d0 — cached frame gen (words 8-9) */
extern uint64_t g_pointer_hold_a0;        /* 0x22d5d8 — latched record copy A (words 0-1) */
extern uint64_t g_pointer_hold_a8;        /* 0x22d5e0 — copy A (words 2-3) */
extern uint64_t g_pointer_hold_b0;        /* 0x22d5e8 — copy A (words 4-5) */
extern uint64_t g_pointer_hold_b8;        /* 0x22d5f0 — copy A (words 6-7) */
extern uint64_t g_pointer_cached_arg;     /* 0x22d5f8 — copy A (words 8-9) */

/* misc_b-owned: hold latch word (set while a hold is being tracked) */
uint32_t g_pointer_hold_latch;   /* 0x22d57c — hold-in-progress latch */

/* ---- own earlier chunks (b-chain) ---- */
extern void inertia_state_step(float x, float y, float *state,
                               uint64_t frame, int settle,
                               uint32_t mode, float id,
                               float gen);                     /* misc.c (b-chain) @ 00183230 (below) */

/* ---- misc_c chain (raw 16900+) ---- */
extern void drag_velocity_feed(float x, float y, float cap,
                              float *state, int64_t frame); /* misc.c (c-chain) @ 00183854 */

/*
 * INERTIA_STATE_NAN — the raw initializes the anchor-id word (state[9])
 * with Ghidra's -NAN (0xffc00000); a bits store keeps it exact.
 */
#define INERTIA_STATE_NAN 0xffc00000u

/*
 * inertia_state layout — 18 floats at 0x22d518 (g_scroll_position):
 *   [0]  position (scroll offset, >= 0, clamped to max_pos)
 *   [1]  velocity (|v| <= 2400)
 *   [2]  hold settle flag / drag latch
 *   [3]  hold settle copy
 *   [4]  settle frame (low), [5] settle frame (high)
 *   [6]  touch latch (FLT_TRUE_MIN when held)
 *   [7]  init flag (FLT_TRUE_MIN once initialized)
 *   [8]  generation (the gen argument)
 *   [9]  anchor id (-NAN when none)
 *   [10] frame (low), [11] frame (high)
 *   [12] anchor frame (low), [13] anchor frame (high)
 *   [14] anchor x, [15] anchor y
 *   [16] anchor y copy, [17] anchor y copy 2
 */

/* ===== hold anchor freshness ===== */

/*
 * hold_anchor_still_valid — poll the tracked hold anchor: the anchor is
 * fresh while the ctx and generation match (gen in [1, 0x400]), the
 * frame did not go backwards, and either no pointer is active or the
 * frame is within 251 ms (0xfb) of the last poll. A live hold pair
 * older than 500 frames is decayed to its high half. keep == 0, a NULL
 * ctx or a changed ctx/generation re-arms the anchor slot (slot id
 * 0xffffffff on a ctx change) and resets the scroll (inertia mode 4,
 * pointer parked). Returns 1 while the anchor is valid, 0 after a
 * reset. @ 001830b0
 */
int hold_anchor_still_valid(uint64_t frame, int keep, uint64_t ctx,
                            int gen)
{
    if (g_scroll_target == 0) {
        /* touch state empty: (re)initialize the whole anchor block */
        g_pointer_widget = 0;
        g_pointer_hold_pair = 0;
        g_pointer_guard_hi = 0;
        g_scroll_target = 1.0f; /* 0x22d560 low word */
        g_pointer_source = 0;
        g_pointer_touch_frame = 0;
        g_pointer_anchor_b8 = 0;
        g_pointer_anchor_b0 = 0;
        g_pointer_param_6 = 0;
        g_pointer_cached_pair = 0;
        g_pointer_param_8 = 0;
        g_pointer_hold_a0 = 0;
        g_pointer_hold_b8 = 0;
        g_pointer_hold_a8 = 0;
        g_pointer_cached_arg = 0;
        g_pointer_hold_b0 = 0;
        g_pointer_frame_count = (uint32_t)gen;
        g_pointer_seq = 0xffffffffu;
        g_pointer_last_frame = frame;
        g_pointer_event_arg = ctx;
    }

    if (keep == 0 || ctx == 0 || (uint32_t)(gen - 1) > 0x3ff
        || g_pointer_event_arg != ctx
        || g_pointer_frame_count != (uint32_t)gen) {
        /* re-arm the anchor slot (the raw keeps the old slot id unless
           the ctx changed, then 0xffffffff) */
        if (g_pointer_event_arg != ctx) {
            g_pointer_seq = 0xffffffffu;
            g_pointer_event_arg = ctx;
        }
        g_pointer_frame_count = (uint32_t)gen;
    } else if (g_pointer_last_frame <= frame
               && (g_pointer_active == 0
                   || frame - g_pointer_last_frame < 0xfb)) {
        if (g_pointer_hold_pair != 0
            && frame - g_pointer_touch_frame > 500) {
            g_pointer_hold_pair &= 0xffffffff00000000u; /* decay to the id half */
            g_pointer_last_frame = frame;
            return 1;
        }
        g_pointer_last_frame = frame;
        return 1;
    }

    /* reset: park the pointer state and clear the scroll */
    g_pointer_hold_pair = g_pointer_park_hold;
    g_pointer_guard_hi = (uint32_t)g_pointer_park_guard;
    g_pointer_active = 0;
    inertia_state_step(0, 0, &g_scroll_position, frame, 0, 4, -1.0f,
                       0.0f);
    g_pointer_last_frame = frame;
    return 0;
}

/* ===== 1-D inertial scroll ===== */

/*
 * inertia_state_step — advance the 18-float inertial scroll state:
 *
 * The state initializes on first use (state[7] == 0) and re-validates
 * its position/velocity (finite, position >= 0, |velocity| <= 2400) on
 * every step. max_pos derives from the generation:
 * max(0, (gen - 1) * 120 - 260) for gen > 0x401, else 0, and the
 * position is clamped to [0, max_pos]. A backwards frame, a gap over
 * 250 frames (0xfa) or a generation change resets velocity and the
 * settle window. mode 1 latches a touch inside the scroll region
 * (x in [-530, 350], y in [132, 472]); mode 2 feeds a drag sample
 * through drag_velocity_feed (settle == 1 gates re-latching); mode 3
 * settles the drag; settle == 0 with no touch glides the position with
 * an exp(-8/s) velocity decay (dt capped at 64 ms) and stops at
 * |velocity| < 1 or the max position. Returns nothing (state only).
 * @ 00183230
 */
void inertia_state_step(float x, float y, float *state, uint64_t frame,
                        int settle, uint32_t mode, float id, float gen)
{
    uint64_t span_pair;

    if (state == NULL)
        return;

    if (state[7] == 0.0f) {
        /* first use: zero everything, mark initialized */
        memset(state, 0, 18 * sizeof(float));
        state[7] = 1.4013e-45f; /* FLT_TRUE_MIN — init marker */
        state[8] = gen;
        *(uint64_t *)(state + 10) = frame;
        *(uint32_t *)(state + 9) = INERTIA_STATE_NAN;
    }

    float pos = state[0];
    if (!isfinite(pos) || isnan(pos))
        goto reset_state;

    float vel = state[1];
    float abs_vel = fabsf(vel);
    if (!isfinite(abs_vel) || isnan(abs_vel)
        || !isfinite(state[0xe]) || isnan(state[0xe])
        || !isfinite(state[0xf]) || isnan(state[0xf])
        || !isfinite(state[0x10]) || isnan(state[0x10])
        || pos < 0.0f
        || !(abs_vel <= 2400.0f))
        goto reset_state;

    /* position ceiling from the generation */
    float max_pos;
    uint64_t tail;
    {
        double lim;
        if ((uint32_t)((int)gen - 0x401) < 0xfffffc00u) {
            /* small generation: no scroll room */
            max_pos = 0.0f;
            lim = 0.0;
            pos = fminf(pos, 0.0f);
            state[0] = pos;
            if (0x400 < (uint32_t)gen)
                goto reset_frame;
        } else {
            lim = fma((double)(int)gen - 1.0, 120.0 /* 0x405b800000000000 */,
                      180.0 /* 0x4066400000000000 */);
            lim = lim + 32.0 - 472.0;
            if (lim <= 0.0)
                lim = 0.0;
            max_pos = (float)lim;
            if (pos <= max_pos)
                state[0] = pos;
            else
                state[0] = max_pos;
        }

        /* frame/identity window */
        uint64_t last_frame = *(uint64_t *)(state + 10);
        if (frame < last_frame) {
            tail = frame - last_frame;
            goto reset_frame;
        }
        tail = frame - last_frame;
        if (tail > 0xfa || state[8] != gen)
            goto reset_frame;
    }

    if (isfinite(x) && !isnan(x) && gen != 0.0f && mode != 4) {
        if (isfinite(y) && !isnan(y)) {
            if (mode == 1) {
                /* touch begin: latch the anchor inside the scroll region */
                if ((int)id >= -1 && settle == 1 && state[6] == 0.0f) {
                    state[1] = 0.0f;
                    if (y <= 472.0f && x >= -530.0f && x <= 350.0f
                        && y >= 132.0f) {
                        state[2] = 0.0f;
                        state[9] = id;
                        state[0xf] = y;
                        state[0x10] = y;
                        state[6] = 1.4013e-45f; /* FLT_TRUE_MIN */
                        state[0xe] = x;
                        *(uint64_t *)(state + 0xc) = frame;
                    }
                    goto store_frame;
                }
            } else if ((mode & 0xfffffffeu) == 2) {
                /* drag (mode 2) / release (mode 3) */
                if (state[6] != 0.0f
                    && (mode != 2 || settle == 1)
                    && (int)id >= 0
                    && state[9] == id) {
                    /* raw prints only (state, frame): x2-x4 still hold the
                     * caller's x/y and the inertia velocity cap at the call
                     * site — Ghidra models them as callee params. */
                    drag_velocity_feed(x, y, 2400.0f, state, (int64_t)frame);
                    if (mode == 3) {
                        float had = state[2];
                        if (had == 0.0f) {
                            state[1] = 0.0f;
                        } else {
                            *(uint64_t *)(state + 4) = frame;
                        }
                        state[2] = 0.0f;
                        state[3] = had != 0.0f ? 1.0f : 0.0f;
                        state[6] = 0.0f;
                        *(uint32_t *)(state + 9) = INERTIA_STATE_NAN;
                    }
                    goto store_frame;
                }
            } else if (state[6] == 0.0f && settle == 0) {
                /* glide: exp(-8/s) velocity decay, dt capped at 64 ms */
                uint64_t dt = tail;
                if (dt > 0x3f)
                    dt = 0x40;

                double decay_exp = ((double)dt / 1000.0) * -8.0;
                double decay = exp(decay_exp);
                double dpos = (double)pos
                              + (double)vel * expm1(decay_exp) * -0.125;

                float new_pos = max_pos;
                if (dpos <= (double)max_pos)
                    new_pos = (float)dpos;
                if (dpos < 0.0)
                    new_pos = 0.0f;
                float new_vel = (float)(decay * (double)vel);

                state[0] = new_pos;
                state[1] = new_vel;
                if (new_pos != 0.0f || new_vel >= 0.0f) {
                    int settle_now = 0;
                    if (new_vel > 0.0f && !isnan(new_pos) && !isnan(max_pos)
                        && new_pos == max_pos)
                        settle_now = 1;
                    if (!settle_now && fabsf(new_vel) >= 1.0f)
                        goto store_frame;
                }
                state[1] = 0.0f;
                goto store_frame;
            }
        }
    }

    /* fall-through reset of the touch/velocity window */
    state[6] = 0.0f;
    state[1] = 0.0f;
    *(uint64_t *)(state + 4) = frame;
    span_pair = g_libg_span;
    *(uint64_t *)(state + 10) = frame;
    *(uint64_t *)(state + 2) = span_pair;
    state[8] = gen;
    *(uint32_t *)(state + 9) = INERTIA_STATE_NAN;
    return;

reset_frame:
    state[6] = 0.0f;
    state[1] = 0.0f;
    *(uint64_t *)(state + 4) = frame;
    span_pair = g_libg_span;
    *(uint64_t *)(state + 10) = frame;
    *(uint64_t *)(state + 2) = span_pair;
    state[8] = gen;
    *(uint32_t *)(state + 9) = INERTIA_STATE_NAN;
    return;

reset_state:
    state[0x10] = 0.0f;
    state[6] = 0.0f;
    span_pair = g_libg_span;
    *(uint64_t *)(state + 4) = frame;
    *(uint32_t *)(state + 9) = INERTIA_STATE_NAN;
    state[0xe] = 0.0f;
    state[0xf] = 0.0f;
    state[0] = 0.0f;
    state[1] = 0.0f;
    *(uint64_t *)(state + 2) = span_pair;

store_frame:
    *(uint64_t *)(state + 10) = frame;
}

/* ===== hold tracker ===== */

/*
 * hold_tracker_feed — feed one pointer record into the hold tracker:
 *
 * record is the 10-word touch record ({id, x, y, k2, k1, action,
 * widget, 0, gen_lo, gen_hi}). The distance to the latched anchor
 * (x at 0x22d5b4, y at 0x22d5b8) within 64 units sets the in-radius
 * flag (0x22d56c); a changed record identity (words 8-9 vs 0x22d5d0)
 * clears the anchor identity flag (0x22d570). With no in-region
 * pointer the scroll is reset (inertia mode 0); otherwise the drag is
 * fed through inertia_state_step (mode 2 while held, mode 3 on
 * release, settle = (mode == 0), id = word 0 as float) and the record
 * is latched into the 0x22d5d8..0x22d5f8 copy block. mode != 0 then
 * finalizes the hold: the hold pair (0x22d578) is set only when the
 * anchor identity is intact and the latched action/id/frame-gen words
 * (0x22d5c4/+0x14, 0x22d5c8 vs record[3], 0x22d5d0 vs record[4]) all
 * match; the touch window (0x22d5a0) is refreshed either way.
 * @ 00183658
 */
void hold_tracker_feed(uint64_t frame, void *record_v, uint32_t flag,
                       int mode)
{
    uint64_t *record = (uint64_t *)record_v;

    /* distance to the latched anchor (x at +4 of 0x22d5b0, y at 0x22d5b8) */
    float anchor_x = *(const float *)((const uint8_t *)&g_pointer_anchor_b0
                                      + 4); /* 0x22d5b4 */
    float anchor_y = *(const float *)&g_pointer_anchor_b8; /* 0x22d5b8 */
    float rec_x = *(const float *)((const uint8_t *)record + 4);
    float rec_y = *(const float *)(record + 1);

    double dx = (double)rec_x - (double)anchor_x;
    double dy = (double)rec_y - (double)anchor_y;
    double dist2 = fma(dx, dx, dy * dy);
    if (dist2 < 64.0)
        g_pointer_guard_lo = 1; /* within the 8-unit hold radius */

    if (record[4] != g_pointer_param_8)
        g_pointer_guard_hi = 0; /* record identity changed */

    if (g_pointer_widget == 0) {
        inertia_state_step(0, 0, &g_scroll_position, frame, 0, 0, -1.0f,
                           (float)*(uint32_t *)&flag);
    } else {
        if (g_pointer_latch == 0)
            g_pointer_guard_hi = 0;

        uint32_t inertia_mode = mode != 0 ? 3 : 2;
        float id = *(const float *)record; /* word 0 (the id) as float */
        inertia_state_step(rec_x, rec_y, &g_scroll_position, frame,
                           mode == 0, inertia_mode, id,
                           (float)*(uint32_t *)&flag);

        /* state[2] (0x22d520) or state[3] (0x22d524) set: in-radius */
        if (*(const uint32_t *)((const uint8_t *)&g_pointer_state_pair
                                + 4) != 0
            || (mode != 0
                && *(const uint32_t *)((const uint8_t *)&g_pointer_state_pair
                                       + 8) != 0))
            g_pointer_guard_lo = 1;
    }

    /* latch the record into the copy block */
    g_pointer_cached_arg = record[4];
    g_pointer_hold_a8 = record[1];
    g_pointer_hold_a0 = record[0];
    g_pointer_hold_b0 = record[3];
    g_pointer_hold_b8 = record[2];

    if (g_pointer_guard_lo != 0 && g_pointer_widget != 0)
        g_pointer_hold_latch = 1; /* 0x22d57c */

    if (mode == 0)
        return;

    g_pointer_active = 0;

    if (g_pointer_guard_hi == 0 || g_pointer_guard_lo != 0) {
        g_pointer_hold_pair = 0;
        if (g_pointer_widget != 0)
            g_pointer_hold_latch = 1;
        g_pointer_touch_frame = frame;
        return;
    }

    /* hold valid only when the latched action/id/frame-gen all match */
    uint32_t hold_ok = 0;
    if ((uint32_t)(g_pointer_cached_pair >> 32) != 0
        && g_pointer_param_6 != 0) {
        if (g_pointer_param_8 != 0) {
            if ((uint32_t)(g_pointer_cached_pair >> 32)
                    == *(const uint32_t *)((const uint8_t *)record + 0x14)
                && g_pointer_param_6 == record[3])
                hold_ok = g_pointer_param_8 == record[4];
        }
    }

    g_pointer_hold_pair = hold_ok;
    if ((hold_ok & 1) != 0) {
        g_pointer_active = 0;
        g_pointer_touch_frame = frame;
        return;
    }

    if (g_pointer_widget != 0)
        g_pointer_hold_latch = 1;
    g_pointer_touch_frame = frame;
}
/* misc_c chain chunk 1: covers raw lines 16904-17300 */

#define _GNU_SOURCE

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>
#include <time.h>

/*
 * Chain C continues misc.c from raw line 16904 (drag_velocity_feed) through
 * the import-stub tail boundary at 20232; the file metadata header lives in
 * the a-chain chunk 1. The trailing raw-range function
 * settings_values_commit (starts at raw 17215) straddles line 17300 and is
 * finished here through raw line 17327.
 */

/* glibc fortified vsnprintf (only auto-declared under _FORTIFY_SOURCE) */
extern int __vsnprintf_chk(char *s, size_t maxlen, int flag, size_t slen,
                           const char *format, va_list ap);

/* ---- cross-file / cross-chain functions ---- */
extern int  ui_latch_test_and_set(int value, volatile int *latch); /* menu_engine.c @ 00193f80 */
extern void plus_active_refresh(int mode);                         /* misc.c (a-chain) @ 0014fbe8 */
extern void theme_machine_pump(void);                              /* external @ 00185d10 — not in dump (theme machine tick) */

/* ---- rodata ---- */
extern const char     g_theme_status_fmt[];          /* 0x1343dc — theme status line format string */
extern const uint64_t SETTINGS_BLOB_STAMP;           /* 0x10f868 — settings blob header qword 1 */
extern const uint32_t SETTINGS_COMMIT_SLOT_SEED[4];  /* 0x10fb50 — packed slot-id seeds for the commit records */

/* ===== theme menu / settings-spin registry (0x284688 block) ===== */

volatile int g_theme_latch;               /* 0x284688 — registry busy latch (test-and-set gate) */
int32_t      g_theme_menu_id;             /* 0x284694 — owning theme menu id (0 = unclaimed) */
int32_t      g_theme_menu_status;         /* 0x284698 — last id-gated theme menu status code */
uint32_t     g_theme_revision;            /* 0x2846a0 — bump on every registry/value-cache change */
int32_t      g_settings_value_cache[118]; /* 0x2846b0 — current 118-slot settings value cache (0x1d8 bytes) */
char         g_theme_menu_line[0x40];     /* 0x284aa8 — theme menu status line buffer */
int32_t      g_theme_status_code_last;    /* 0x284aec — unconditional status-code mirror */

/* one registered spin/settings channel: 0x38-byte header on a 0x40 stride */
typedef struct spin_channel_group {
    void     *ctx;          /* +0x00 — context handed to every query/commit call */
    uint64_t  id_bitmap[3]; /* +0x08 — claim bitmap: word id>>6, bit id&0x3f (ids 0..191) */
    int     (*query)(void *ctx, uint32_t id, void *out); /* +0x20 — fills a 0x18-byte out record */
    uint64_t  reserved;     /* +0x28 */
    int     (*commit)(void *ctx, void *slot_table, uint32_t count); /* +0x30 — pushes the slot table */
} spin_channel_group_t;

/* full record filed into a registry slot: lead qword + the group it installs */
typedef struct spin_slot_record {
    uint64_t             lead;  /* 0x284888 + slot*0x40 (slot 0: value-cache tail) */
    spin_channel_group_t group; /* overlays 0x284890 + slot*0x40 */
} spin_slot_record_t;

/* 8 slot records; group i header at 0x284890+i*0x40, its id bitmap words at
 * 0x284898+i*0x40 (groups 6/7: 0x284a18/0x284a58), query hooks at
 * 0x2848b0+i*0x40, commit hooks at 0x2848c0+i*0x40 */
spin_slot_record_t g_spin_slots[8]; /* 0x284888 — spin/settings channel slot records */

/* ===== drag / inertia tracking ===== */

/*
 * drag_velocity_feed — feed one drag sample into a vertical drag tracker.
 * x/y is the pointer position, cap the value range, state the 17-float
 * tracker block, frame the sample timestamp. While idle the tracker arms
 * once the pointer leaves an 8 px radius (squared distance 64.0) around the
 * anchor (state[14]/state[15]); the value then follows the finger
 * (value += baseline - y, baseline = anchor y on the arming sample, last y
 * afterwards) clamped to [0, cap]. Velocity is (baseline - y) * 1000 / dt
 * clamped to +-2400.0 (0x40a2c00000000000), blended with a 50 ms
 * exponential when already armed, and zeroed when no time passed, when the
 * value rests at 0 with downward motion, or when pinned at cap (or cap ==
 * 0). Marks state[3] touched, stores the frame at state[4] (bytes 16..23)
 * and the sample y at state[16] on every call (the dead-zone exit only
 * updates state[16]). state layout: [0] value, [1] velocity, [2] armed,
 * [3] touched, [4] frame (int64), [10] prev frame while armed (int64,
 * bytes 40..47), [12] prev frame while idle (int64, bytes 48..55),
 * [14] anchor x, [15] anchor y, [16] last y.
 * @ 00183854
 */
void drag_velocity_feed(float x, float y, float cap, float *state, int64_t frame)
{
    float armed = state[2];

    if (armed == 0.0f) {
        double dx = (double)x - (double)state[14];
        double dy = (double)y - (double)state[15];
        if (fmaf(dx, dx, dy * dy) < 64.0) { /* 8 px dead zone around the anchor */
            state[16] = y;
            return;
        }
        state[2] = 1.4013e-45f; /* bit pattern 1 — mark armed */
    }

    /* baseline and previous-frame slots depend on the pre-sample armed state:
     * baseline = last y (0x40) once armed, anchor y (0x3c) on the arming
     * sample; previous frame at 0x28 once armed, 0x30 while idle */
    size_t base_off = (armed != 0.0f) ? 0x40 : 0x3c;
    int64_t prev_frame = *(int64_t *)((char *)state + ((armed != 0.0f) ? 0x28 : 0x30));
    double delta = (double)*(float *)((char *)state + base_off) - (double)y;
    double raw = delta + (double)state[0];
    float value = 0.0f;

    if (raw >= 0.0)
        value = (raw <= (double)cap) ? (float)raw : cap; /* fminf(raw, cap) */
    state[0] = value;

    float velocity = 0.0f;
    if (prev_frame != frame) {
        double dt = (double)(uint64_t)(frame - prev_frame);
        double vel = (delta * 1000.0) / dt;
        velocity = (vel < -2400.0) ? -2400.0f : (float)fmin(vel, 2400.0);
        if (armed != 0.0f) {
            /* 50 ms exponential blend toward the new sample:
             * (vel_new - vel_old) * expm1(-dt/50) - vel_old */
            double decay = expm1(dt / -50.0);
            velocity = (float)(((double)velocity - (double)state[1]) * decay
                               - (double)state[1]);
        }
        state[1] = velocity;
    } else if (delta != 0.0) {
        state[1] = 0.0f; /* zero elapsed time — velocity unusable, drop it */
    }

    if (value != 0.0f || delta >= 0.0) {
        /* pinned at the top of the range with downward motion, or no range */
        if (cap == 0.0f || (delta != 0.0 && delta < 0.0 && value == cap))
            state[1] = 0.0f;
    } else {
        state[1] = 0.0f; /* resting at 0 with downward motion */
    }

    *(int64_t *)((char *)state + 0x10) = frame; /* state[4] qword */
    state[3] = 1.4013e-45f;                     /* bit pattern 1 — touched */
    state[16] = y;
}

/* ===== theme menu status line ===== */

/*
 * ui_status_line_format — checked vsnprintf wrapper for status lines.
 * Formats fmt into buf through __vsnprintf_chk(buf, cap_b, 0, cap_a, fmt,
 * ap): cap_b is the snprintf write bound, cap_a the fortified object size
 * ((size_t)-1 = unfortified — the raw ABI puts the bound in x2 and the
 * fortify size in x1; call sites with unequal caps, e.g. themes.c's
 * (buf, -1, 0x50), fix the order).
 * Returns the formatted length like vsnprintf — the raw body leaves the
 * __vsnprintf_chk result in x0 (Ghidra types the wrapper void, but callers
 * in menu_engine.c and renderer.c read the value).
 * @ 00183ac8
 */
int ui_status_line_format(char *buf, size_t cap_a, size_t cap_b,
                          const char *fmt, ...)
{
    va_list ap;
    int len;

    va_start(ap, fmt);
    len = __vsnprintf_chk(buf, cap_b, 0, cap_a, fmt, ap);
    va_end(ap);
    return len;
}

/*
 * theme_menu_id_claim — claim the theme menu for menu_id (>= 1).
 * Returns false for ids below 1 or while another owner holds g_theme_latch
 * (the latch is left held in that case). On success adopts g_theme_menu_id
 * when it is unclaimed (0) or already this id, then releases the latch and
 * returns whether the claim held. Side effects: g_theme_menu_id,
 * g_theme_latch.
 * @ 00183c78
 */
bool theme_menu_id_claim(int menu_id)
{
    bool claimed;

    if (menu_id < 1)
        return false;
    if (ui_latch_test_and_set(1, &g_theme_latch) & 1)
        return false; /* busy — latch stays held by the current owner */
    claimed = g_theme_menu_id == 0 || g_theme_menu_id == menu_id;
    if (claimed)
        g_theme_menu_id = menu_id; /* first claim or re-claim */
    g_theme_latch = 0;
    return claimed;
}

/*
 * theme_menu_status_set — publish a theme menu status code and reason.
 * code is mirrored unconditionally to g_theme_status_code_last; under the
 * registry latch, an id match (g_theme_menu_id == 0 or == menu_id) stores
 * the code in g_theme_menu_status and formats reason into the 0x40-byte
 * g_theme_menu_line via ui_status_line_format. Returns 1 while the latch
 * is held (the raw test-and-set verdict), otherwise the formatter length
 * or 0 — the raw function leaves that value in x0 (Ghidra types it void,
 * but menu_engine.c/renderer.c callers read it). Side effects: the three
 * globals named above plus g_theme_latch.
 * @ 00183cdc
 */
int theme_menu_status_set(int code, int menu_id, const char *reason)
{
    int ret = 0;

    g_theme_status_code_last = code; /* 0x284aec — unconditional mirror */
    if (ui_latch_test_and_set(1, &g_theme_latch) & 1)
        return 1; /* busy — previous latch value */
    if (g_theme_menu_id == 0 || g_theme_menu_id == menu_id) {
        g_theme_menu_status = code; /* 0x284698 */
        if (reason != NULL)
            ret = ui_status_line_format(g_theme_menu_line, 0x40, 0x40,
                                        g_theme_status_fmt, reason);
    }
    g_theme_latch = 0;
    return ret;
}

/* ===== settings / spin channel registry ===== */

/*
 * spin_slot_record_store — file one 8-qword spin channel record into
 * registry slot `slot` (0..7). The record's id bitmap must stay clear of
 * the bitmap carried by the other dedicated spin group (slot 6 checks
 * group 7 at 0x284a58/0x284a60/0x284a68, slot 7 checks group 6 at
 * 0x284a18/0x284a20/0x284a28, any other slot checks both); a clash — or a
 * set gate pair — rejects the store with 4. On success the record is
 * copied to 0x284888 + slot*0x40, g_theme_revision bumps and 1 is
 * returned; g_theme_latch is cleared on every exit path. Raw form: a
 * tail-split continuation entered with the registry base (0x284688) in
 * x19, the record in x20, the slot index in w21 and a gate pair in x0/x9
 * ((x9 & x0) != 0 skips the store outright); spin_state_dispatch() is the
 * trampoline that reloads x0 from the state word at +0x9e8.
 * @ 00184004
 */
int spin_slot_record_store(uint64_t gate_word, uint64_t gate_mask,
                           const spin_slot_record_t *record, uint32_t slot)
{
    if ((gate_mask & gate_word) != 0)
        return 4; /* gated off by the dispatcher */

    if (slot == 6) {
        /* installing spin group 6: only group 7's ids must be free */
        if ((record->group.id_bitmap[0] & g_spin_slots[7].group.id_bitmap[0]) != 0 ||
            (record->group.id_bitmap[1] & g_spin_slots[7].group.id_bitmap[1]) != 0 ||
            (record->group.id_bitmap[2] & g_spin_slots[7].group.id_bitmap[2]) != 0) {
            g_theme_latch = 0;
            return 4;
        }
    } else {
        /* every other slot must clear group 6's ids; non-spin slots (not 7)
         * must also clear group 7's */
        if ((record->group.id_bitmap[0] & g_spin_slots[6].group.id_bitmap[0]) != 0 ||
            (record->group.id_bitmap[1] & g_spin_slots[6].group.id_bitmap[1]) != 0 ||
            (record->group.id_bitmap[2] & g_spin_slots[6].group.id_bitmap[2]) != 0) {
            g_theme_latch = 0;
            return 4;
        }
        if (slot != 7 &&
            ((record->group.id_bitmap[0] & g_spin_slots[7].group.id_bitmap[0]) != 0 ||
             (record->group.id_bitmap[1] & g_spin_slots[7].group.id_bitmap[1]) != 0 ||
             (record->group.id_bitmap[2] & g_spin_slots[7].group.id_bitmap[2]) != 0)) {
            g_theme_latch = 0;
            return 4;
        }
    }

    g_spin_slots[slot] = *record; /* 0x284888 + slot*0x40 */
    g_theme_revision++;           /* registry counter at 0x2846a0 */
    g_theme_latch = 0;
    return 1;
}

/*
 * settings_blob_build — build the 0x3c4-byte 'NM69252' settings blob.
 * Pumps the theme machine first, then writes the 8-byte magic
 * "NM69252" (0x32353239364d4e little-endian plus NUL), the rodata stamp
 * qword, 118 {int32 index, int32 value} slots at +0x10 (8-byte stride,
 * values from g_settings_value_cache at 0x2846b0) and a trailing CRC-32
 * of the first 0x3c0 bytes at +0x3c0. The CRC is the reflected poly
 * 0xedb88320, init 0xffffffff, final complement — the raw loop is
 * NEON-vectorized and also mixes the shifted poly 0x76dc4190 with 16-byte
 * helper constants at 0x10f9c0/0x10f9c8 and 0x10fac0/0x10fac8.
 * @ 001842d0
 */
void settings_blob_build(void *blob)
{
    uint64_t *qword = blob;
    unsigned char *bytes = blob;
    uint32_t crc = 0xffffffff;
    size_t i;
    int bit;

    theme_machine_pump();

    qword[0] = 0x32353239364d4e; /* "NM69252" + NUL, little-endian */
    qword[1] = SETTINGS_BLOB_STAMP;
    for (i = 0; i < 118; i++) { /* 0x76 slots */
        int *slot = (int *)(bytes + 0x10 + i * 8);
        slot[0] = (int)i;
        slot[1] = g_settings_value_cache[i];
    }

    for (i = 0; i < 0x3c0; i++) {
        crc ^= bytes[i];
        for (bit = 0; bit < 8; bit++)
            crc = (crc >> 1) ^ (0xedb88320u & (0u - (crc & 1u)));
    }
    *(uint32_t *)(bytes + 0x3c0) = ~crc;
}

/*
 * settings_values_commit — push the 0x1d8-byte value source through the
 * first live channel group and adopt it on success. The source's 29
 * 16-byte records are expanded — using the packed slot-id seeds from
 * 0x10fb50, each half stepping +4 per record — into a 118-slot
 * {id, value} table; slots 0x74/0x75 come from the source ints at
 * +0x1d0/+0x1d4. The first group with a commit hook (0x2848c0 + i*0x40)
 * receives (ctx, table, 0x76); a verdict of 1 or 2 refreshes the
 * plus-active cache with the source int at +0x40, copies the source into
 * g_settings_value_cache and bumps g_theme_revision. Returns the channel
 * verdict, or 0 when no group is live.
 * @ 001846b8
 */
int settings_values_commit(void *values)
{
    int slot_table[236]; /* 29 records * 8 ints + 4 tail ints = 118 slots */
    const unsigned char *src = values;
    const uint32_t *seed = SETTINGS_COMMIT_SLOT_SEED;
    uint32_t id_a = seed[0], id_a_hi = seed[1]; /* stream A slot ids */
    uint32_t id_b = seed[2], id_b_hi = seed[3]; /* stream B slot ids */
    int *out = slot_table;
    size_t off;
    int i;

    for (off = 0; off < 0x1d0; off += 0x10) { /* 29 source records */
        out[0] = (int)id_a;
        out[1] = *(const int *)(src + off);
        out[2] = (int)id_a_hi;
        out[3] = *(const int *)(src + off + 4);
        out[4] = (int)id_b;
        out[5] = *(const int *)(src + off + 8);
        out[6] = (int)id_b_hi;
        out[7] = *(const int *)(src + off + 12);
        out += 8;
        id_a += 4;
        id_a_hi += 4;
        id_b += 4;
        id_b_hi += 4;
    }
    /* tail: source ints at +0x1d0/+0x1d4 land in slots 0x74 and 0x75 */
    slot_table[232] = 0x74;
    slot_table[233] = *(const int *)(src + 0x1d0);
    slot_table[234] = 0x75;
    slot_table[235] = *(const int *)(src + 0x1d4);

    for (i = 0; i < 8; i++) { /* first live channel group wins the push */
        int verdict;

        if (g_spin_slots[i].group.commit == NULL)
            continue;
        verdict = g_spin_slots[i].group.commit(g_spin_slots[i].group.ctx,
                                              slot_table, 0x76);
        if ((uint32_t)(verdict - 1) < 2) { /* 1 or 2 = accepted */
            plus_active_refresh(*(const int *)(src + 0x40));
            memcpy(g_settings_value_cache, values, 0x1d8);
            g_theme_revision++;
        }
        return verdict;
    }
    return 0;
}
/* misc_c chain chunk 2: covers raw lines 17301-17700 */

#include <pthread.h>
#include <dlfcn.h>
#include <link.h>

/*
 * This chunk continues misc.c from raw 17329 (theme_preview_load_begin).
 * spin_request_poll (starts raw 17660) straddles the 17700 boundary and is
 * finished here through raw 17751.
 */

/* ---- cross-file / external functions ---- */
extern void menu_scroll_revision_bump(int which, volatile uint32_t *rev);
                                        /* menu_engine.c @ 00194010 */
extern uint32_t g_menu_scroll_revision; /* 0x22d824 — menu_engine.c scroll rev */
extern int menu_entry_spin_index(const void *entry);
                                        /* external @ 0018d15c — not in dump
                                         * (spin record index for a menu entry;
                                         * renderer.c aliases it plus_active_query) */
extern int spin_record_poll_blocked(const void *record);
                                        /* external @ 0018ded4 — not in dump
                                         * (nonzero = skip polling that record) */

/* ===== theme machine / status state (0x287b90 block, written here) ===== */

uint32_t g_theme_apply_state;       /* 0x287b90 — 2 = applied/confirmed */
uint32_t g_theme_status_counter;    /* 0x287bac — status line epoch counter */
uint32_t g_theme_status_flag;       /* 0x287bb4 — latched status-line flag */
uint64_t g_theme_current_id;        /* 0x287c60 — current theme request id */
uint32_t g_theme_reported_status;   /* 0x287c68 — last reported status code */
uint8_t  g_theme_status_line[0x60]; /* 0x287c6c — theme status line buffer */
uint8_t  g_theme_report_pending;    /* 0x28469f — pending status report flag */

/* ===== Nexus+ delivery resolver (alt TU) ===== */

pthread_once_t g_plus_once = PTHREAD_ONCE_INIT; /* 0x28dcfc — once gate for the alt resolver */
void *g_plus_active_fn;              /* 0x28dd00 — cached nexus_protected_plus_active */
int64_t g_theme_id_generator;        /* 0x28dd08 — last issued theme request id (-1 = none) */

/* ===== nexus_sx_spin feature records + settings push channel ===== */

/*
 * Pointer to the nexus_sx_spin feature record table (loader-relocated slot).
 * Records are 0x18 bytes: +0 name pointer, +8 int32 min value,
 * +0xc int32 max value. The table's first record names "nexus_sx_spin".
 */
const uint8_t *g_nexus_sx_spin_records; /* 0x1a36f8 */

/*
 * Registry slot 8 — the settings-push channel. Different shape from the
 * eight spin groups (g_spin_slots): {lead, ctx, push} where push receives
 * the finished 0x3c4 settings blob.
 */
typedef struct settings_push_channel {
    uint64_t lead;   /* +0x00 — 0x284a88 */
    void *ctx;       /* +0x08 — 0x284a90 */
    int (*push)(void *ctx, const void *blob, uint32_t len); /* +0x10 — 0x284a98 */
    uint64_t tail[5]; /* +0x18 — record padding to the 0x40 stride */
} settings_push_channel_t;

settings_push_channel_t g_settings_push_channel; /* 0x284a88 */

/* ===== theme preview ===== */

/*
 * theme_preview_load_begin — start a theme preview load cycle.
 * Issues the next theme request id (g_theme_current_id = generator + 1, or 1
 * when the generator holds -1) and mirrors it into the generator, clears the
 * reported status, sets the apply state to 2 ("applied/confirmed" — the
 * preview renders from the pending slot), writes "LOADING PREVIEW" into the
 * 0x60-byte g_theme_status_line, bumps the menu scroll revision, clears the
 * status flag and the pending-report flag and bumps the status counter and
 * g_theme_revision. @ 00186bf4
 */
void theme_preview_load_begin(void)
{
    g_theme_current_id = (uint64_t)(g_theme_id_generator + 1);
    g_theme_reported_status = 0;
    if (g_theme_id_generator == -1)
        g_theme_current_id = 1; /* first request after a reset */
    g_theme_apply_state = 2;
    g_theme_id_generator = (int64_t)g_theme_current_id;
    ui_status_line_format((char *)g_theme_status_line, 0x60, 0x60,
                          g_theme_status_fmt, "LOADING PREVIEW");
    menu_scroll_revision_bump(1, &g_menu_scroll_revision);
    g_theme_status_flag = 0;
    g_theme_report_pending = 0;
    g_theme_status_counter++;
    g_theme_revision++;
}

/* ===== theme value / page parsing ===== */

/*
 * theme_value_text_parse — parse theme value text by kind into out[4]
 * (out is zeroed first). Kind 9: "a,b,c,d" — exactly four comma-separated
 * int32 values, each with optional sign and surrounding blanks/tabs, parsed
 * with an overflow guard against INT_MIN/INT_MAX (magnitudes), stored in
 * order; returns true only after the fourth value. Kind 4: a single digit
 * '0'..'4' (nothing may follow), stored in out[0], returns true. Any other
 * kind, NULL/empty/over-long (>= 0x61) text or a malformed field returns
 * false. @ 001895e0
 */
bool theme_value_text_parse(int kind, const char *text, uint32_t out[4])
{
    out[0] = 0;
    out[1] = 0;
    out[2] = 0;
    out[3] = 0;

    if (kind == 9) {
        if (text == NULL || *text == '\0' || strlen(text) >= 0x61)
            return false;
        {
            const char *p = text;
            uint32_t field = 0;
            bool complete = false;

            do {
                char sign;

                while (*p == ' ' || *p == '\t')
                    p++;
                sign = *p;
                if (sign == '-' || sign == '+')
                    p++;
                if (*p < '0' || *p > '9') /* raw: (*p - 0x3a) < 0xfffffff6 */
                    return complete;
                {
                    uint32_t acc = 0;
                    /* magnitude limit: 0x80000000 signed '-' else 0x7fffffff */
                    uint32_t limit = (sign == '-') ? 0x80000000u : 0x7fffffffu;

                    while (*p >= '0' && *p <= '9') {
                        uint32_t digit = (uint32_t)*p - 0x30;

                        p++;
                        if ((limit - digit) / 10 < acc)
                            return complete; /* would overflow int32 */
                        acc = digit + acc * 10;
                    }
                    out[field] = (sign == '-') ? (uint32_t)-(int32_t)acc : acc;
                }
                while (*p == ' ' || *p == '\t')
                    p++;
                if (field < 3) {
                    if (*p != ',')
                        return complete;
                    p++;
                } else if (*p != '\0') {
                    return complete;
                }
                complete = field > 2; /* true once the 4th value is stored */
                field++;
            } while (field != 4);
            return complete;
        }
    }

    if (kind == 4 && text != NULL
        && (unsigned char)*text - 0x30 < 5 && text[1] == '\0') {
        out[0] = (uint32_t)*text - 0x30;
        return true;
    }
    return false;
}

/*
 * theme_page_parse — validate a 'THEM' page snapshot. page points at the
 * page header (int32 words): [0] must be 1, [1] 0x2534 (the theme snapshot
 * size), [5] the page number, [6] the row count (< 0x21 = 33), [4] the
 * total slot count (<= 0x1000) with at least `count' slots remaining after
 * page (total - page >= count), and the guards [10] < 8, [11] < 2 and
 * [12] < 2. Every row (0x128 bytes = 0x4a words, starting at word 13) must
 * carry an id below 1000000, have bit 0 of the byte at +4 set, and hold
 * three NUL-terminated non-empty 0x60-byte strings at +8, +104 and +200;
 * ids must be unique. Returns 1 when the page is well-formed, else 0
 * (a count of 0 is valid). @ 0018cb98
 */
int theme_page_parse(const int32_t *page, uint32_t page_number)
{
    uint32_t total;
    uint32_t remaining;
    uint32_t count;
    uint32_t i;

    if (page[0] != 1 || (uint32_t)page[1] != 0x2534
        || (uint32_t)page[5] != page_number)
        return 0;
    count = (uint32_t)page[6];
    if (count >= 0x21)
        return 0;
    total = (uint32_t)page[4];
    remaining = (page_number <= total) ? total - page_number : 0;
    if (total > 0x1000 || remaining < count)
        return 0;
    if ((uint32_t)page[10] >= 8 || (uint32_t)page[11] >= 2
        || (uint32_t)page[12] >= 2)
        return 0;
    if (count == 0)
        return 1;

    for (i = 0; i < count; i++) {
        const int32_t *row = page + 0xd + (size_t)i * 0x4a;
        int s;

        if ((uint32_t)row[0] >= 1000000 || (((const uint8_t *)row)[4] & 1) == 0)
            return 0;
        for (s = 0; s < 3; s++) {
            const char *str = (const char *)row + 8 + (size_t)s * 0x60;

            if (memchr(str, 0, 0x60) == NULL || str[0] == '\0')
                return 0;
        }
        for (uint32_t j = 0; j < i; j++) {
            if (page[0xd + (size_t)j * 0x4a] == row[0])
                return 0; /* duplicate row id */
        }
    }
    return 1;
}

/* ===== settings / spin channel registry (slot 8 + group lookup) ===== */

/*
 * settings_blob_push — build the settings blob from the value cache and
 * submit it through the settings-push channel (registry slot 8). Returns 1
 * when no push hook is installed (nothing to do) or the hook accepts the
 * blob, and -1 (0xffffffff) when the hook rejects it.
 * @ 0018d334
 */
int settings_blob_push(void)
{
    uint8_t blob[0x3c4];

    if (g_settings_push_channel.push == NULL)
        return 1;
    settings_blob_build(blob);
    if (g_settings_push_channel.push(g_settings_push_channel.ctx,
                                     blob, 0x3c4) == 0)
        return -1;
    return 1;
}

/*
 * spin_group_record_find — find the spin-channel registry record that owns
 * a feature id. The id selects a claim word (id >> 6, ids 0..191 span the
 * three words) and a bit (id & 0x3f); the first of the eight groups whose
 * bitmap carries the bit wins. Returns the slot record
 * (g_spin_slots[i], 0x284888 + i*0x40 — its group ctx sits at +8 and the
 * request hook at group +0x28) or NULL when no group claims the id.
 * @ 0018d3b4
 */
spin_slot_record_t *spin_group_record_find(uint64_t feature_id)
{
    uint32_t word = (uint32_t)(feature_id >> 6) & 0x3ffffff;
    uint64_t bit = 1ULL << (feature_id & 0x3f);
    int i;

    for (i = 0; i < 8; i++) {
        if ((g_spin_slots[i].group.id_bitmap[word] & bit) != 0)
            return &g_spin_slots[i];
    }
    return NULL;
}

/* ===== Nexus+ delivery resolver (alt TU) ===== */

/* dl_iterate_phdr callback result: match count + resolved function */
typedef struct plus_resolve_result {
    int32_t matches; /* +0x00 — libNexusDelivery.so phdr records seen */
    uint32_t pad;    /* +0x04 */
    void *fn;        /* +0x08 — verified nexus_protected_plus_active */
} plus_resolve_result_t;

/*
 * delivery_plus_active_find_alt — dl_iterate_phdr callback for the alt
 * plus-active resolver. For the phdr record whose object name ends in
 * "/libNexusDelivery.so": counts the match, dlopens the path with
 * RTLD_NOW|RTLD_NOLOAD (6 — only attach if already loaded), resolves
 * "nexus_protected_plus_active" and — when dladdr confirms the symbol's
 * base equals the visited phdr address — stores it in result->fn. Always
 * returns 0 so the whole phdr list is walked. @ 0018d56c
 */
int delivery_plus_active_find_alt(struct dl_phdr_info *info, size_t size,
                                  void *data)
{
    plus_resolve_result_t *result = data;

    (void)size;
    if (info->dlpi_name != NULL) {
        const char *base = strrchr(info->dlpi_name, '/');

        if (base != NULL && strcmp(base + 1, "libNexusDelivery.so") == 0) {
            void *handle;

            result->matches++;
            handle = dlopen(info->dlpi_name, 6 /* RTLD_NOW | RTLD_NOLOAD */);
            if (handle != NULL) {
                void *fn = dlsym(handle, "nexus_protected_plus_active");

                if (fn != NULL) {
                    Dl_info addr;

                    if (dladdr(fn, &addr) != 0
                        && addr.dli_fbase == (void *)info->dlpi_addr)
                        result->fn = fn;
                }
                dlclose(handle);
            }
        }
    }
    return 0;
}

/*
 * plus_active_refresh_alt — resolve nexus_protected_plus_active from
 * libNexusDelivery.so by walking the phdr list (run under g_plus_once).
 * The callback counts matching phdr records; only an exact single match
 * installs the verified function pointer in g_plus_active_fn. No return
 * value. @ 0018d500
 */
void plus_active_refresh_alt(void)
{
    plus_resolve_result_t result = {0, 0, NULL};

    dl_iterate_phdr(delivery_plus_active_find_alt, &result);
    if (result.matches == 1)
        g_plus_active_fn = result.fn;
}

/* ===== nexus_sx_spin value polling ===== */

/*
 * Value record filled by a spin group's query hook: 0x18 bytes, pre-zeroed
 * with the size word preset to 0x18; the hook must echo the size, set the
 * valid byte at +0x15 and report the current feature value at +0x0c.
 */
typedef struct spin_value_record {
    int32_t size;   /* +0x00 — echo of 0x18 */
    int32_t a;      /* +0x04 */
    int32_t b;      /* +0x08 */
    int32_t value;  /* +0x0c — current feature value */
    int32_t c;      /* +0x10 */
    uint8_t pad;    /* +0x14 */
    uint8_t valid;  /* +0x15 — nonzero when the record is populated */
    uint8_t tail[2];/* +0x16 */
} spin_value_record_t;

/*
 * spin_request_poll — poll the current value of one nexus_sx_spin feature.
 * entry is the 0x48-byte menu entry record (table at 0x1a36f0): word 0 is
 * the feature id, the int at +0x38 the spin record hint (>= 0) and the
 * byte at +0x44 the row kind (< 5). The group owning the feature id (first
 * claim bitmap hit) is queried through its query hook with a zeroed
 * 0x18-byte record; when the hook answers 1 with the size echoed and the
 * valid byte set, the feature index is resolved from the entry
 * (menu_entry_spin_index) and the reported value is range-checked against
 * the nexus_sx_spin record's min (+8) / max (+0xc). An in-range, changed
 * value is adopted into g_settings_value_cache[idx] and bumps
 * g_theme_revision. No return value. @ 0018df74
 */
void spin_request_poll(const uint32_t *entry)
{
    uint32_t feature = entry[0];
    spin_slot_record_t *rec = spin_group_record_find(feature);

    if (rec == NULL)
        return;
    if (rec->group.query != NULL && (int32_t)entry[0xe] >= 0
        && (uint8_t)entry[0x11] < 5) {
        const uint8_t *table = g_nexus_sx_spin_records;

        if (spin_record_poll_blocked(table + (uint64_t)(int32_t)entry[0xe]
                                       * 0x18) == 0) {
            spin_value_record_t out;

            memset(&out, 0, sizeof out);
            out.size = 0x18;
            if (rec->group.query(rec->group.ctx, feature, &out) == 1
                && out.size == 0x18 && out.valid != 0) {
                int32_t idx = menu_entry_spin_index(entry);

                if (idx >= 0) {
                    const int32_t *min_p = (const int32_t *)(table
                                        + (uint64_t)(uint32_t)idx * 0x18 + 8);
                    const int32_t *max_p = (const int32_t *)(table
                                        + (uint64_t)(uint32_t)idx * 0x18 + 0xc);

                    if (*min_p <= out.value && out.value <= *max_p
                        && g_settings_value_cache[idx] != out.value) {
                        g_settings_value_cache[idx] = out.value;
                        g_theme_revision++;
                    }
                }
            }
        }
    }
}
/* misc_c chain chunk 3: covers raw lines 17701-18100 */

/*
 * Channel 2 — the second engine read channel (menu_engine.c's stage/launcher
 * verification also drives it). ctx/read/resolve live at 0x28fac8..0x28fae8,
 * the engine base copy at 0x28fad0 and the cached context/root/stage-node
 * block at 0x28fb70..0x28fbc0. channel2_root_node_check (starts raw 18034)
 * straddles the 18100 boundary and is finished here through raw 18111.
 */

void *g_channel2_ctx;      /* 0x28fac8 — channel-2 engine context object */
int (*g_channel2_read)(void *ctx, uint64_t addr, void *out, uint32_t len);
                           /* 0x28fae0 — remote read hook (1 = success) */
void *(*g_channel2_resolve)(void *ctx, const char *name);
                           /* 0x28fae8 — engine object-by-name hook
                            * (used by menu_engine.c's stage build) */
uint64_t g_channel2_game_base;  /* 0x28fad0 — engine image base (channel-2 copy) */
uint64_t g_channel2_cached_ctx; /* 0x28fb70 — cached engine context */
uint64_t g_channel2_cached_root;/* 0x28fb78 — cached stage root object */
uint64_t g_channel2_stage_nodes[24];
                           /* 0x28fb80 — [0] stage root, [1..23] stage nodes */

/* family-internal ordering (raw defines read_ptr and child_link_valid
 * after their users) */
extern uint64_t channel2_read_ptr(uint64_t addr);                 /* @ 0018edcc */
extern int channel2_child_link_valid(uint64_t node, uint64_t parent); /* @ 0018f750 */

/*
 * is_plausible_ptr_mc — channel-2 pointer sanity gate: a value read from the
 * engine must be above 0xfff and 8-byte aligned to be treated as a pointer
 * (raw: 0xfff < v && (v & 7) == 0).
 */
static int is_plausible_ptr_mc(uint64_t v)
{
    return v > 0xfff && (v & 7) == 0;
}

/*
 * mc_read_ok — raw read guard for direct channel-2 reads: a read of `len'
 * bytes at `base + off' is only issued when the address is at least 0x1000
 * and the read does not wrap. (The raw tests (base + off + len) >> 3 against
 * 0x201 plus a (base & ~(len - 1)) wrap equality; both fold into this one
 * low/wrap check.)
 */
static int mc_read_ok(uint64_t base, uint64_t off, uint64_t len)
{
    return base + off >= 0x1000 && base <= UINT64_MAX - (off + len);
}

/*
 * channel2_context_cache_validate — revalidate the cached channel-2 context
 * and stage tree. Reads the engine context singleton (game base + 0x12eb9f0)
 * and, through it, the stage root (context + 0x90); both must be plausible
 * pointers and equal the cached pair (0x28fb70/0x28fb78). The stage root
 * (node[0]) and all 23 remaining cached stage nodes must have valid child
 * links into that root. Returns 1 when the whole cache still matches, else 0.
 * @ 0018e668
 */
int channel2_context_cache_validate(void)
{
    uint64_t ctx = 0;
    uint64_t root = 0;
    uint32_t i;

    if (g_channel2_game_base + 0x12eb9f0 >= 0x1000) {
        ctx = channel2_read_ptr(g_channel2_game_base + 0x12eb9f0);
        if (ctx != 0 && ctx + 0x98 >= 0x1000)
            root = channel2_read_ptr(ctx + 0x90);
    }

    if (ctx != g_channel2_cached_ctx || root != g_channel2_cached_root)
        return 0;
    if (!channel2_child_link_valid(g_channel2_stage_nodes[0], root))
        return 0;
    for (i = 0; i < 23; i++) { /* nodes [1..23] at 0x28fb88.. */
        if (!channel2_child_link_valid(g_channel2_stage_nodes[i + 1], root))
            return 0;
    }
    return 1;
}

/*
 * channel2_read_ptr — read one qword through channel 2 and return it as a
 * plausible pointer. Addresses below 0x1000, failed reads and implausible
 * values (<= 0xfff or misaligned) all yield 0. @ 0018edcc
 */
uint64_t channel2_read_ptr(uint64_t addr)
{
    uint64_t value = 0;

    if (addr < 0x1000) /* raw: (addr + 8) >> 3 < 0x201 */
        return 0;
    if (g_channel2_read(g_channel2_ctx, addr, &value, 8) != 1)
        return 0;
    if (!is_plausible_ptr_mc(value))
        return 0;
    return value;
}

/*
 * channel2_ancestor_chain_check — walk the parent chain (parent pointer at
 * node + 0x38) from node toward target, at most 32 hops. Every hop must be
 * a valid child link. Returns 1 when the walk reaches target (node ==
 * target up front also counts), else 0 — the raw body leaves the final
 * node == target comparison in x0. @ 0018ee70
 */
int channel2_ancestor_chain_check(uint64_t node, uint64_t target)
{
    if (node != target && node != 0) {
        uint32_t depth = 0;

        for (;;) {
            uint64_t parent = channel2_read_ptr(node + 0x38);

            /* an implausible parent reads as 0 and fails the link check */
            if (!channel2_child_link_valid(node, parent))
                break;
            node = parent;
            if (node == target || depth == 0x1f)
                break;
            depth++;
        }
    }
    return node == target;
}

/*
 * channel2_child_link_valid — verify a node/parent pair read through
 * channel 2. The node's parent pointer (+0x38) must equal parent; the
 * int32 child index at node + 0x40 must be non-negative and below the
 * parent's uint16 child count (+0x4e); the parent's children array
 * pointer (+0x50) must be a plausible pointer and children[index] must
 * read back as the node. Returns 1 when the full round trip holds.
 * @ 0018f750
 */
int channel2_child_link_valid(uint64_t node, uint64_t parent)
{
    int32_t index;
    uint16_t count;
    uint64_t children;
    uint64_t entry;

    if (node == 0 || parent == 0)
        return 0;
    if (channel2_read_ptr(node + 0x38) != parent)
        return 0;
    if (!mc_read_ok(node, 0x40, 4)
        || g_channel2_read(g_channel2_ctx, node + 0x40, &index, 4) != 1
        || index < 0)
        return 0;
    if (!mc_read_ok(parent, 0x4e, 2)
        || g_channel2_read(g_channel2_ctx, parent + 0x4e, &count, 2) != 1
        || !(index < (int32_t)count))
        return 0;
    if (!mc_read_ok(parent, 0x50, 8)
        || g_channel2_read(g_channel2_ctx, parent + 0x50, &children, 8) != 1
        || !is_plausible_ptr_mc(children))
        return 0;
    if (!mc_read_ok(children, (uint64_t)(uint32_t)index * 8, 8)
        || g_channel2_read(g_channel2_ctx,
                           children + (uint64_t)(uint32_t)index * 8,
                           &entry, 8) != 1
        || !is_plausible_ptr_mc(entry))
        return 0;
    return entry == node;
}

/*
 * text_vformat_128 — vsnprintf into a 0x80-byte buffer (variadic wrapper).
 * arg_b/arg_c occupy ABI slots x1/x2 which the body ignores (the raw call
 * is vsnprintf(buf, 0x80, fmt, ap)). Returns the formatted length.
 * @ 0018fb04
 */
int text_vformat_128(char *buf, uint64_t arg_b, uint64_t arg_c,
                     const char *fmt, ...)
{
    va_list ap;
    int len;

    (void)arg_b;
    (void)arg_c;
    va_start(ap, fmt);
    len = vsnprintf(buf, 0x80, fmt, ap);
    va_end(ap);
    return len;
}

/*
 * channel2_root_node_check — verify a node is the engine UI root. The
 * vtable pointer at +0 must equal game base + 0x11c0a48, the parent
 * pointer (+0x38) and the qwords at +0xa8 and +0x120 must be null, the
 * int32 child index (+0x40) must be -1 and the flag byte at +0x1f0 must
 * be zero. Returns 1 when every check holds, else 0. @ 0018fba0
 */
int channel2_root_node_check(uint64_t node)
{
    uint64_t word = 1;
    int32_t index = 0;
    uint8_t flag = 1;

    if (channel2_read_ptr(node) != g_channel2_game_base + 0x11c0a48)
        return 0;
    if (!mc_read_ok(node, 0x38, 8)
        || g_channel2_read(g_channel2_ctx, node + 0x38, &word, 8) != 1
        || word != 0)
        return 0;
    if (!mc_read_ok(node, 0x40, 4)
        || g_channel2_read(g_channel2_ctx, node + 0x40, &index, 4) != 1
        || index != -1)
        return 0;
    if (!mc_read_ok(node, 0xa8, 8)
        || g_channel2_read(g_channel2_ctx, node + 0xa8, &word, 8) != 1
        || word != 0)
        return 0;
    if (!mc_read_ok(node, 0x120, 8)
        || g_channel2_read(g_channel2_ctx, node + 0x120, &word, 8) != 1
        || word != 0)
        return 0;
    if (!mc_read_ok(node, 0x1f0, 1)
        || g_channel2_read(g_channel2_ctx, node + 0x1f0, &flag, 1) != 1
        || flag != 0)
        return 0;
    return 1;
}
/* misc_c chain chunk 4: covers raw lines 18101-18500 */

/*
 * Channel (generic) — the {ctx, read} handle variant of the channel-2
 * family: the handle is passed in as two words (ctx, read) instead of being
 * read from the 0x28fac8 globals. remote_modules_hash_verify (starts raw
 * 18506) straddles the 18500 boundary and is finished here through raw
 * 18551.
 */

/* family-internal ordering (raw defines read_ptr/child_link_valid after use) */
extern uint64_t channel_read_ptr(const void *handle[2], uint64_t addr);   /* @ 00190510 */
extern int channel_child_link_valid(const void *handle[2], uint64_t node,
                                    uint64_t parent);                     /* @ 001905a4 */

/* family-internal helpers from chunk 3 */
extern int is_plausible_ptr_mc(uint64_t v); /* chunk 3 — 0xfff<v && (v&7)==0 */

/* SHA-256 state (112 bytes: 8 h words, 2 length qwords, 64-byte block) */
typedef struct sha256_state {
    uint32_t h[8];       /* +0x00 state words */
    uint64_t total_len;  /* +0x20 hashed byte count */
    uint64_t block_len;  /* +0x28 pending block bytes */
    uint8_t block[64];   /* +0x30 pending block */
} sha256_state_t;

/* SHA-256 family (raw defines them after their first users) */
extern void sha256_init(sha256_state_t *state);              /* @ 00191314 */
extern void sha256_update(sha256_state_t *state, const void *data,
                          size_t len);                       /* @ 00191338 */
extern void sha256_final(sha256_state_t *state, void *out32);/* @ 00191678 */

/*
 * channel_read_ptr — read one qword through a {ctx, read} channel handle
 * (handle[0] = ctx, handle[1] = read hook) and return it as a plausible
 * pointer; failed reads and implausible values yield 0. Addresses below
 * 0x1000 are not read. @ 00190510
 */
uint64_t channel_read_ptr(const void *handle[2], uint64_t addr)
{
    uint64_t value = 0;

    if (addr < 0x1000) /* raw: (addr + 8) >> 3 < 0x201 */
        return 0;
    if (((int (*)(const void *, uint64_t, void *, uint32_t))handle[1])(
            handle[0], addr, &value, 8) != 1)
        return 0;
    if (!is_plausible_ptr_mc(value))
        return 0;
    return value;
}

/*
 * channel_child_link_valid — channel-handle variant of the node/parent link
 * check: node's parent (+0x38) == parent, int32 child index (+0x40) in
 * [0, parent's uint16 child count (+0x4e)), parent's children array (+0x50)
 * plausible and children[index] == node. Returns 1 when the full round trip
 * holds. @ 001905a4
 */
int channel_child_link_valid(const void *handle[2], uint64_t node,
                             uint64_t parent)
{
    int (*read_fn)(const void *, uint64_t, void *, uint32_t) =
        (int (*)(const void *, uint64_t, void *, uint32_t))handle[1];
    const void *ctx = handle[0];
    int32_t index;
    uint16_t count;
    uint64_t children;
    uint64_t entry;

    if (node == 0 || parent == 0)
        return 0;
    if (channel_read_ptr(handle, node + 0x38) != parent)
        return 0;
    if (node + 0x40 < 0x1000
        || read_fn(ctx, node + 0x40, &index, 4) != 1 || index < 0)
        return 0;
    if (parent + 0x4e < 0x1000
        || read_fn(ctx, parent + 0x4e, &count, 2) != 1
        || !(index < (int32_t)count))
        return 0;
    if (parent + 0x58 < 0x1000 /* raw: 0x200 < (parent + 0x58) >> 3 */
        || read_fn(ctx, parent + 0x50, &children, 8) != 1
        || !is_plausible_ptr_mc(children))
        return 0;
    {
        uint64_t slot = children + (uint64_t)(uint32_t)index * 8;

        if (slot < 0x1000 /* raw: (slot + 8) >> 3 < 0x201 */
            || read_fn(ctx, slot, &entry, 8) != 1
            || !is_plausible_ptr_mc(entry))
            return 0;
    }
    return entry == node;
}

/*
 * channel_ancestor_chain_check — channel-handle variant of the parent-chain
 * walk: from node toward target, at most 32 hops, every hop validated by
 * channel_child_link_valid. Returns 1 when the walk reaches target (node ==
 * target up front counts), else 0 — the raw body leaves node == target in
 * x0. @ 001907b0
 */
int channel_ancestor_chain_check(const void *handle[2], uint64_t node,
                                 uint64_t target)
{
    if (node != target && node != 0) {
        uint32_t depth = 0;

        for (;;) {
            uint64_t parent = channel_read_ptr(handle, node + 0x38);

            if (!channel_child_link_valid(handle, node, parent))
                break;
            node = parent;
            if (node == target || depth == 0x1f)
                break;
            depth++;
        }
    }
    return node == target;
}

/*
 * channel_root_node_check — channel-handle variant of the engine UI root
 * identity check. game_base (param_1) pairs with the {ctx, read} handle:
 * the vtable pointer at node + 0 must equal game_base + 0x11c0a48, the
 * parent (+0x38), the qwords at +0xa8 and +0x120 must be null, the int32
 * child index (+0x40) must be -1 and the flag byte at +0x1f0 must be zero.
 * Returns 1 when every check holds. @ 001908c0
 */
int channel_root_node_check(const void *base_and_handle[2], uint64_t node)
{
    uint64_t game_base = (uint64_t)base_and_handle[0]; /* {game_base, read hook} */
    const void *handle[2] = { base_and_handle[1], base_and_handle[1] }; /* read fn pairs with a dedicated ctx below */
    int (*read_fn)(const void *, uint64_t, void *, uint32_t) =
        (int (*)(const void *, uint64_t, void *, uint32_t))base_and_handle[1];
    const void *ctx = NULL; /* raw passes the read fn itself as its arg0 */
    uint64_t word = 1;
    int32_t index = 0;
    uint8_t flag = 1;

    if (node == 0)
        return 0;
    if (channel_read_ptr(handle, node) != game_base + 0x11c0a48)
        return 0;
    if (node + 0x38 < 0x1000
        || read_fn(ctx, node + 0x38, &word, 8) != 1 || word != 0)
        return 0;
    if (node + 0x40 < 0x1000
        || read_fn(ctx, node + 0x40, &index, 4) != 1 || index != -1)
        return 0;
    if (node + 0xa8 < 0x1000
        || read_fn(ctx, node + 0xa8, &word, 8) != 1 || word != 0)
        return 0;
    if (node + 0x120 < 0x1000
        || read_fn(ctx, node + 0x120, &word, 8) != 1 || word != 0)
        return 0;
    if (node + 0x1f1 < 0x1000 /* raw: 0x1000 < node + 0x1f1 */
        || read_fn(ctx, node + 0x1f0, &flag, 1) != 1 || flag != 0)
        return 0;
    return 1;
}

/* ===== arm64 hook repair ===== */

/*
 * arm64_branch_encode — encode an ARM64 B (unconditional branch) instruction
 * from site `from' to target `to' into *out. Both addresses must be 4-byte
 * aligned and the signed reach must stay within +-128 MiB (raw rejects a
 * forward span >= 2^27 and a backward span > 0x8000000). Returns 1 with the
 * encoded word 0x14000000 | ((offset >> 2) & 0x3ffffff), else 0 (NULL out,
 * misaligned pair or out of reach). @ 00190d68
 */
int arm64_branch_encode(uint64_t from, uint64_t to, uint32_t *out)
{
    uint64_t delta;

    if (out == NULL || (((uint32_t)to | (uint32_t)from) & 3) != 0)
        return 0;
    delta = to - from;
    if (from <= to) {
        if (delta >> 0x1b != 0) /* forward reach >= 128 MiB */
            return 0;
    } else if (from - to > 0x8000000) { /* backward reach > 128 MiB */
        return 0;
    }
    *out = 0x14000000u | ((uint32_t)delta >> 2 & 0x3ffffff);
    return 1;
}

/*
 * hook_bl_repair — re-point two remote BL call sites. site is the 4-qword
 * descriptor {word1_addr, word1_new_target, word2_addr, word2_new_target}
 * (word addresses must be 4-aligned; both current words must be BL
 * instructions, opcode >> 0x1a == 5). ops is the {ctx, read, write, flush}
 * engine vtable: the current words are read and their targets confirmed,
 * then site->word1 is rewritten to branch to target1, flushed and read back;
 * on mismatch the original is restored and -1 is returned. The same runs for
 * word2; on success 0. Raw x0 values: 0 ok, 1 word2-only failure, -1
 * verified-mismatch (0xffffffff). @ 00190dd4
 */
int64_t hook_bl_repair(const uint64_t *site, const void *ops[4])
{
    const void *ctx = ops[0];
    int (*read_fn)(const void *, uint64_t, int32_t *, uint32_t) =
        (int (*)(const void *, uint64_t, int32_t *, uint32_t))ops[1];
    int (*write_fn)(const void *, uint64_t, uint32_t) =
        (int (*)(const void *, uint64_t, uint32_t))ops[2];
    void (*flush_fn)(const void *, uint64_t) =
        (void (*)(const void *, uint64_t))ops[3];
    int32_t word;
    uint32_t restore;
    uint64_t failed_addr;
    int64_t result;

    if (site[0] == site[2]) /* same site twice — nothing to do */
        return 0;
    if ((site[0] & 3) != 0 || (*(uint32_t *)((const char *)site + 0xc) >> 0x1a) != 5)
        return 0;
    if (read_fn(ctx, site[0], &word, 4) == 0)
        return 0;
    if (word != (int32_t)site[1] || (site[2] & 3) != 0
        || (*(uint32_t *)((const char *)site + 0x1c) >> 0x1a) != 5)
        return 0;
    if (read_fn(ctx, site[2], &word, 4) == 0)
        return 0;
    if (word != (int32_t)site[3])
        return 0;

    /* rewrite word 1 */
    if (write_fn(ctx, site[0], *(const uint32_t *)((const char *)site + 0xc)) != 0)
        flush_fn(ctx, site[0]);
    if (read_fn(ctx, site[0], &word, 4) == 0
        || word != *(const int32_t *)((const char *)site + 0xc)) {
        /* verify failed: restore word 1, then word 2 anyway, report 0 */
        result = 0;
        failed_addr = site[0];
        restore = (uint32_t)site[1];
    } else {
        /* rewrite word 2 */
        if (write_fn(ctx, site[2], *(const uint32_t *)((const char *)site + 0x1c)) != 0)
            flush_fn(ctx, site[2]);
        if (read_fn(ctx, site[2], &word, 4) == 0) {
            result = 1; /* word 2 rewrite unreadable */
        } else if (word == *(const int32_t *)((const char *)site + 0x1c)) {
            return 0; /* both sites repaired and verified */
        } else {
            result = 1; /* word 2 readback mismatch — restore below */
        }
        failed_addr = site[2];
        restore = (uint32_t)site[result * 2 + 1];
    }

    /* restore the failed site's original word and verify */
    if (write_fn(ctx, failed_addr, restore) != 0)
        flush_fn(ctx, failed_addr);
    if (read_fn(ctx, failed_addr, &word, 4) == 0)
        return -1;
    return -(int64_t)(word != (int32_t)restore);
}

/* ===== engine string objects ===== */

/*
 * string_obj_from_cstr — build a tagged engine string object (3 qwords =
 * 0x18 bytes) from a C string. Text longer than 0x100 bytes is rejected
 * (returns 0); NULL text or a NULL object likewise. Strings up to 22 bytes
 * are stored inline: obj[0] = length << 1 (tagged length, low bit 0), bytes
 * at obj+1. Longer strings use the long form: obj[2] = text pointer,
 * obj[0] = (length | 0xf) + 2 (rounded capacity, tagged with low bits 10),
 * obj[1] = length. Returns 1 on success. @ 00190fd0
 */
int string_obj_from_cstr(const char *text, uint64_t obj[3])
{
    size_t len;

    if (text == NULL || obj == NULL)
        return 0;
    len = strlen(text);
    if (len >= 0x101)
        return 0;
    obj[0] = 0;
    obj[1] = 0;
    obj[2] = 0;
    if (len < 0x17) { /* 22 bytes fit inline */
        *(char *)obj = (char)((int)len << 1);
        memcpy((char *)obj + 1, text, len);
        return 1;
    }
    obj[2] = (uint64_t)(uintptr_t)text; /* long form keeps the C string */
    obj[0] = (len | 0xf) + 2;
    obj[1] = len;
    return 1;
}

/* ===== remote module verification ===== */

/*
 * remote_modules_hash_verify — verify `count' remote modules by SHA-256.
 * base is a load bias added to every module address; read_fn/ctx read from
 * the remote image (0x400-byte chunks); entries is an array of count
 * 0x30-byte records {addr(8), name_offset(4), name_len(4), sha256[4](32)}.
 * For each record the name (length 1..0x400, non-wrapping address) is read
 * and hashed; the digest must equal the record's sha256 words (the raw
 * compares the state h-words directly after the final block, before padding
 * is applied — equivalent to comparing the full digest). Returns 1 when all
 * count modules match, 0 on the first failure.
 * @ 00191058
 */
int remote_modules_hash_verify(uint64_t base,
                               int (*read_fn)(void *ctx, uint64_t addr,
                                              void *out, uint32_t len),
                               void *ctx, const uint8_t *entries,
                               uint64_t count)
{
    uint64_t i;

    if (base == 0 || count == 0)
        return 0;
    for (i = 0; i < count; i++) {
        const uint64_t *rec = (const uint64_t *)(entries + (size_t)i * 0x30);
        uint32_t name_off = *(const uint32_t *)((const char *)rec + 8);
        uint32_t name_len = *(const uint32_t *)((const char *)rec + 12);
        const uint32_t *expect = (const uint32_t *)((const char *)rec + 16);
        sha256_state_t state;
        uint8_t chunk[0x400];

        if (name_off == 0 || name_off > 0x400
            || base > UINT64_MAX - rec[0] /* CARRY8(addr, base) wrap check */
            || read_fn(ctx, rec[0] + base, chunk, name_len) == 0)
            return 0;
        sha256_init(&state);
        sha256_update(&state, chunk, name_len);
        sha256_final(&state, chunk);
        if (memcmp(chunk, expect, 32) != 0)
            return 0;
    }
    return 1;
}

/*
 * engine_vtables_verify — verify the game image's engine vtable slots point
 * at the expected engine functions. image_base is the libg engine image
 * base (must be below the wrap guard 0xfffffffffee3f448 so that
 * base + 0x11c0bb8 does not wrap). Reads through the {ctx, read} handle:
 * *(base + 0x11c0bb8) must equal base + 0x772ba0, *(base + 0x11c0bf8) must
 * equal base + 0x88836c, *(base + 0x11c0ba8) must equal base + 0x888218,
 * *(base + 0x11abb60) must equal base + 0x592300 (each read gated on the
 * base-below 0xfffffffffedc28a8 wrap guard), and finally *(base + 0x123d758)
 * must be a pointer above 0xfff that is 4-byte aligned. Returns 1 when all
 * slots hold, else 0. @ 00191170
 */
int engine_vtables_verify(uint64_t image_base,
                          int (*read_fn)(void *ctx, uint64_t addr,
                                         void *out, uint32_t len),
                          void *ctx)
{
    uint64_t value = 0;

    if (image_base >= 0xfffffffffee3f448)
        return 0;
    if (read_fn(ctx, image_base + 0x11c0bb8, &value, 8) == 0
        || value != image_base + 0x772ba0)
        return 0;
    if (image_base > 0xfffffffffee3f407
        || read_fn(ctx, image_base + 0x11c0bf8, &value, 8) == 0
        || value != image_base + 0x88836c)
        return 0;
    if (read_fn(ctx, image_base + 0x11c0ba8, &value, 8) == 0
        || value != image_base + 0x888218)
        return 0;
    if (read_fn(ctx, image_base + 0x11abb60, &value, 8) == 0
        || value != image_base + 0x592300
        || image_base >= 0xfffffffffedc28a8)
        return 0;
    if (read_fn(ctx, image_base + 0x123d758, &value, 8) == 0)
        return 0;
    return value > 0xfff && (value & 3) == 0;
}
/* misc_c chain chunk 5: covers raw lines 18501-18900 */

/*
 * SHA-256 implementation (init/update/final) plus the theme store reset.
 * script_port_table_load (starts raw 18978) straddles the 18900 boundary;
 * its phdr-walk body extends to raw 19098 and is finished in chunk 6.
 */

/* rodata blocks (loader-relocated) */
extern const uint64_t SHA256_INIT_STATE[4];    /* 0x13cdf8 — initial h[0..7] */
extern const uint32_t SHA256_K[64];            /* 0x140dc0 — round constants */
extern const uint32_t SHA256_NEON_SHIFTS_A[4]; /* 0x10f750 — schedule-shift constants */
extern const uint32_t SHA256_NEON_SHIFTS_B[4]; /* 0x10f7d0 — schedule-shift constants */
extern const uint32_t SHA256_NEON_SHIFTS_C[4]; /* 0x10f870 — schedule-shift constants */
extern const uint32_t SHA256_NEON_SHIFTS_D[4]; /* 0x10f8f0 — schedule-shift constants */
extern const uint32_t SHA256_FINAL_SHIFT[4];   /* 0x10fa50 — big-endian length shifts */
extern const uint32_t SHA256_FINAL_SHIFT2[4];  /* 0x10fba0 — big-endian length shifts */

/*
 * sha256_init — initialize the 112-byte SHA-256 state: the eight state
 * words are loaded from the 0x13cdf8 constants block, the byte count
 * (state[4], +0x20), the pending block length (state[5], +0x28) and the
 * tail qwords are zeroed. The raw copies 32 constant bytes plus zeros for
 * the remaining 80. @ 00191314
 */
void sha256_init(sha256_state_t *state)
{
    state->h[0] = (uint32_t)SHA256_INIT_STATE[0];
    state->h[1] = (uint32_t)(SHA256_INIT_STATE[0] >> 32);
    state->h[2] = (uint32_t)SHA256_INIT_STATE[1];
    state->h[3] = (uint32_t)(SHA256_INIT_STATE[1] >> 32);
    state->h[4] = (uint32_t)SHA256_INIT_STATE[2];
    state->h[5] = (uint32_t)(SHA256_INIT_STATE[2] >> 32);
    state->h[6] = (uint32_t)SHA256_INIT_STATE[3];
    state->h[7] = (uint32_t)(SHA256_INIT_STATE[3] >> 32);
    state->total_len = 0;
    state->block_len = 0;
    memset(state->block, 0, sizeof state->block);
}

/* message-schedule rotation helpers (raw folds them into the NEON loop) */
#define MC_ROTR32(x, n) (((x) >> (n)) | ((x) << (32 - (n))))

/*
 * sha256_update — hash len bytes of data into the state. Data is buffered in
 * the 64-byte block until full; each full block runs the 64-round SHA-256
 * compression (little-endian schedule expansion, sigma0/sigma1 message
 * schedule, Sigma1/Sigma0/ch/maj round function, K table at 0x140dc0).
 * The raw expands the schedule with two interleaved NEON streams; the
 * scalar form below computes the same w[16..63]. @ 00191338
 */
void sha256_update(sha256_state_t *state, const void *data, size_t len)
{
    const uint8_t *p = data;

    state->total_len += len;
    if (len == 0)
        return;
    do {
        size_t n = 0x40 - state->block_len;

        if (len <= 0x40 - state->block_len)
            n = len;
        memcpy(state->block + state->block_len, p, n);
        len -= n;
        state->block_len += n;
        if (state->block_len == 0x40) {
            uint32_t w[64];
            uint32_t a, b, c, d, e, f, g, h;
            int i;

            for (i = 0; i < 16; i++)
                w[i] = ((uint32_t)state->block[4 * i] << 24)
                     | ((uint32_t)state->block[4 * i + 1] << 16)
                     | ((uint32_t)state->block[4 * i + 2] << 8)
                     | (uint32_t)state->block[4 * i + 3];
            for (i = 16; i < 64; i++) {
                uint32_t s0 = MC_ROTR32(w[i - 15], 7)
                            ^ MC_ROTR32(w[i - 15], 18) ^ (w[i - 15] >> 3);
                uint32_t s1 = MC_ROTR32(w[i - 2], 17)
                            ^ MC_ROTR32(w[i - 2], 19) ^ (w[i - 2] >> 10);

                w[i] = w[i - 16] + s0 + w[i - 7] + s1;
            }
            a = state->h[0];
            b = state->h[1];
            c = state->h[2];
            d = state->h[3];
            e = state->h[4];
            f = state->h[5];
            g = state->h[6];
            h = state->h[7];
            for (i = 0; i < 64; i++) {
                uint32_t S1 = MC_ROTR32(e, 6) ^ MC_ROTR32(e, 11)
                            ^ MC_ROTR32(e, 25);
                uint32_t ch = (e & f) ^ (~e & g);
                uint32_t t1 = h + S1 + ch + SHA256_K[i] + w[i];
                uint32_t S0 = MC_ROTR32(a, 2) ^ MC_ROTR32(a, 13)
                            ^ MC_ROTR32(a, 22);
                uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
                uint32_t t2 = S0 + maj;

                h = g;
                g = f;
                f = e;
                e = d + t1;
                d = c;
                c = b;
                b = a;
                a = t1 + t2;
            }
            state->h[0] += a;
            state->h[1] += b;
            state->h[2] += c;
            state->h[3] += d;
            state->h[4] += e;
            state->h[5] += f;
            state->h[6] += g;
            state->h[7] += h;
            state->block_len = 0;
        }
        p += n;
    } while (len != 0);
}

/*
 * sha256_final — finish the SHA-256 hash and write the 32-byte digest to
 * out32. Builds the standard padding block: 0x80, zeros, then the 64-bit
 * big-endian bit count (byte count * 8 from state[4], +0x20) at
 * pad[pad_len..pad_len+7] where pad_len is 56 (or 120 for a full second
 * block) minus the pending length — the raw writes the length bytes
 * individually (>> 0x15, >> 0xd, >> 5, low). After hashing the padding the
 * eight state words are written little-endian, four bytes per word (the raw
 * loop steps one byte at a time over the same 32 bytes). @ 00191678
 */
void sha256_final(sha256_state_t *state, void *out32)
{
    uint8_t pad[128]; /* two blocks worst case */
    uint64_t bits = state->total_len << 3;
    size_t pad_len = (state->block_len < 0x38) ? 0x38 : 0x78;

    pad_len -= state->block_len;
    memset(pad, 0, sizeof pad);
    pad[0] = 0x80;
    pad[pad_len + 4] = (uint8_t)(bits >> 0x15);
    pad[pad_len + 7] = (uint8_t)bits;
    pad[pad_len + 5] = (uint8_t)(bits >> 0xd);
    pad[pad_len + 6] = (uint8_t)(bits >> 5);
    {
        uint64_t lo = bits << 3; /* big-endian qword via shifts (raw NEON) */

        pad[pad_len] = (uint8_t)(lo >> 0x20);
        pad[pad_len + 1] = (uint8_t)(lo >> 0x18);
        pad[pad_len + 2] = (uint8_t)(lo >> 0x10);
        pad[pad_len + 3] = (uint8_t)(lo >> 8);
    }
    sha256_update(state, pad, pad_len + 8);

    {
        uint8_t *out = out32;
        int i;

        for (i = 0; i < 8; i++) {
            out[i * 4] = (uint8_t)(state->h[i]);
            out[i * 4 + 1] = (uint8_t)(state->h[i] >> 8);
            out[i * 4 + 2] = (uint8_t)(state->h[i] >> 16);
            out[i * 4 + 3] = (uint8_t)(state->h[i] >> 24);
        }
    }
}

/* ===== theme store ===== */

/* script-port binding table (0x2a1e58..0x2a1f38) — defined in chunk 6 */
extern int (*g_script_port_theme_snapshot)(void *buf, uint32_t size); /* 0x2a1ed8 */
extern void script_port_table_load(void);                             /* @ 0019198c */

/*
 * theme_store_reset — reset the 0x918-byte theme store buffer and refill it.
 * Requires a non-NULL buffer of exactly 0x918 bytes: the buffer is zeroed,
 * the size word is stored at +0 and -1 at +0x10. script_port_table_load
 * runs first (lazy binding of the nexus_script_port_* table); when the
 * theme snapshot hook is installed it is called as (buf, 0x918) and its
 * verdict returned, else 0. @ 00191920
 */
int theme_store_reset(void *buf, size_t size)
{
    uint32_t *words = buf;

    if (buf == NULL || size != 0x918)
        return 0;
    memset(buf, 0, 0x918);
    words[0] = 0x918;
    words[4] = 0xffffffff;
    script_port_table_load();
    if (g_script_port_theme_snapshot != NULL)
        return g_script_port_theme_snapshot(buf, 0x918);
    return 0;
}
/* misc_c chain chunk 6: covers raw lines 18901-19300 */

/*
 * Finishes the script_port_table_load straddler (raw 18978-19098), defines
 * the whole script-port binding table (0x2a1e58..0x2a1f38) and the
 * nexus_script_port_* forwarder family. theme_snapshot_read (starts raw
 * 19364) straddles the 19300 boundary and is finished here through raw
 * 19379.
 */

#include <time.h>

/* rodata snapshot magic (fonts.c's FONT_SNAPSHOT_MAGIC) */
extern const uint64_t FONT_SNAPSHOT_MAGIC; /* 0x10f748 — snapshot header qword */

/* ---- functions used before their raw definition order ---- */
extern int memfd_module_fd_open(uint64_t fd_path_addr, const char *soname,
                                const char *sha256_hex);    /* @ 00192f58 */
extern int memfd_fd_path_resolve(const char *path, char *out); /* @ 0019339c */
extern int proc_fd_path_format_alt(char *buf, ...);           /* @ 001935dc */
extern uint64_t dlsym_module_verified(void *handle, const char *name,
                                      uint64_t base);      /* @ 00193324 */
extern int menu_script_port_bind(long *info, size_t size,
                                 int *out_rec);            /* menu_engine.c @ 0019265c
                                                         * (part sheet: misc p6; the
                                                         * menu_engine chain already
                                                         * reconstructed it) */

/* ===== script-port binding table (0x2a1e58..0x2a1f38) ===== */

int (*g_script_port_theme_command)(uint32_t kind, uint32_t a, uint32_t b); /* 0x2a1e58 */
int (*g_script_port_theme_snapshot)(void *buf, uint32_t size);            /* 0x2a1e60 */
int (*g_script_port_camera_snapshot)(void *out);                          /* 0x2a1e68 */
int (*g_script_port_camera_control)(uint32_t op, uint32_t a, uint32_t b); /* 0x2a1e70 */
int (*g_script_port_font_query)(void *out, uint32_t size);              /* 0x2a1e78 */
int (*g_script_port_font_apply)(uint32_t font_id);                        /* 0x2a1e80 */
int (*g_script_port_font_body_current)(void);                             /* 0x2a1e88 */
int (*g_script_port_client_editor_tick)(void);                            /* 0x2a1e90 */
int (*g_script_port_client_editor_apply)(uint32_t a, uint32_t b, uint32_t c,
                                         uint32_t d, uint32_t e);         /* 0x2a1e98 */
int (*g_script_port_client_debug_snapshot)(void *a, uint64_t b);          /* 0x2a1ea0 */
int (*g_script_port_client_debug_apply)(uint32_t a, uint32_t b);          /* 0x2a1ea8 */
int (*g_script_port_battle_snapshot)(void *out, uint32_t size);           /* 0x2a1eb0 */
int (*g_script_port_client_performance_query)(uint64_t arg);              /* 0x2a1eb8 */
int (*g_script_port_client_performance_apply)(uint32_t arg);              /* 0x2a1ec0 */
int (*g_script_port_profile_snapshot)(uint64_t a, uint64_t b,
                                      uint32_t page);                     /* 0x2a1ec8 */
int (*g_script_port_profile_open)(uint64_t a, uint64_t b);                /* 0x2a1ed0 */
int (*g_script_port_profile_set_name)(uint64_t a, const char *name);      /* 0x2a1ed8 */
int (*g_script_port_reset)(int kind);                                     /* 0x2a1ee0 */
int (*g_script_port_client_editor_snapshot)(void *a, uint64_t b);         /* 0x2a1ee8 */
int (*g_script_port_fast_replay_snapshot)(uint64_t a);                    /* 0x2a1ef0 */
int (*g_script_port_fast_replay_claim)(uint64_t a, uint64_t b,
                                       uint32_t c);                       /* 0x2a1ef8 */
int (*g_script_port_chat_snapshot)(uint8_t *out, size_t size);            /* 0x2a1f00 */
int (*g_script_port_hud_snapshot)(uint8_t *out, size_t size);             /* 0x2a1f08 */
int (*g_script_port_reset_core)(int kind);                                /* 0x2a1f10 */
int (*g_script_port_chat_action)(uint32_t action);                        /* 0x2a1f18 */
int (*g_script_port_server_select)(uint32_t region);                      /* 0x2a1f20 */
void *g_script_port_module_base;   /* 0x2a1f28 — dladdr base of the bound module */
void *g_script_port_query_fn;      /* 0x2a1f30 — nexus_script_port_query */
uint64_t g_script_port_next_retry_ms; /* 0x2a1f38 — next allowed rebind (ms) */

/*
 * script_port_table_load — lazily bind the nexus_script_port_* table from
 * libNexusEvasionRuntime69252.so. Skipped while a module base and query fn
 * are already bound (0x2a1f28/0x2a1f30). Rate limited to one attempt per
 * 500 ms (CLOCK_MONOTONIC ms, 0x2a1f38); errno is preserved across the
 * walk. The zeroed bind record is filled by the dl_iterate_phdr callback
 * menu_script_port_bind (matches == 1, both contract fns resolved); the
 * record's 29 slots are then copied into the table globals above.
 * @ 0019198c
 */
void script_port_table_load(void)
{
    int saved_errno = errno;
    struct timespec now;

    if (g_script_port_module_base == NULL || g_script_port_query_fn == NULL) {
        if (clock_gettime(1 /* CLOCK_MONOTONIC */, &now) == 0) {
            uint64_t now_ms = (uint64_t)now.tv_sec * 1000
                            + (uint64_t)now.tv_nsec / 1000000;

            if (g_script_port_next_retry_ms <= now_ms) {
                /* bind record: {matches, pad, 27 fn slots, module base,
                 * query fn} — mirrors the table layout */
                struct {
                    int32_t matches;
                    uint32_t pad;
                    void *fns[27];
                    void *module_base;
                    void *query_fn;
                } rec;

                g_script_port_next_retry_ms = now_ms + 500;
                memset(&rec, 0, sizeof rec);
                dl_iterate_phdr(
                    (int (*)(struct dl_phdr_info *, size_t, void *))menu_script_port_bind,
                    &rec);
                if (rec.matches == 1 && rec.fns[0] != NULL
                    && rec.fns[1] != NULL) {
                    /* slot order follows the raw record offsets (+2 qwords
                     * per slot, starting at record word 2) */
                    g_script_port_theme_command = (int (*)(uint32_t, uint32_t, uint32_t))(void *)rec.fns[0];  /* 0x2a1e58 */
                    g_script_port_theme_snapshot = (int (*)(void *, uint32_t))(void *)rec.fns[1];              /* 0x2a1e60 */
                    g_script_port_camera_snapshot = (int (*)(void *))(void *)rec.fns[2];                       /* 0x2a1e68 */
                    g_script_port_camera_control = (int (*)(uint32_t, uint32_t, uint32_t))(void *)rec.fns[3];  /* 0x2a1e70 */
                    g_script_port_font_query = (int (*)(void *, uint32_t))(void *)rec.fns[4];                     /* 0x2a1e78 */
                    g_script_port_font_apply = (int (*)(uint32_t))(void *)rec.fns[5];                          /* 0x2a1e80 */
                    g_script_port_font_body_current = (int (*)(void))(void *)rec.fns[6];                       /* 0x2a1e88 */
                    g_script_port_client_editor_tick = (int (*)(void))(void *)rec.fns[7];                      /* 0x2a1e90 */
                    g_script_port_client_editor_apply = (int (*)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t))(void *)rec.fns[8];  /* 0x2a1e98 */
                    g_script_port_client_debug_snapshot = (int (*)(void *, uint64_t))(void *)rec.fns[9];       /* 0x2a1ea0 */
                    g_script_port_client_debug_apply = (int (*)(uint32_t, uint32_t))(void *)rec.fns[10];       /* 0x2a1ea8 */
                    g_script_port_battle_snapshot = (int (*)(void *, uint32_t))(void *)rec.fns[11];            /* 0x2a1eb0 */
                    g_script_port_client_performance_query = (int (*)(uint64_t))(void *)rec.fns[12];           /* 0x2a1eb8 */
                    g_script_port_client_performance_apply = (int (*)(uint32_t))(void *)rec.fns[13];           /* 0x2a1ec0 */
                    g_script_port_profile_snapshot = (int (*)(uint64_t, uint64_t, uint32_t))(void *)rec.fns[14]; /* 0x2a1ec8 */
                    g_script_port_profile_open = (int (*)(uint64_t, uint64_t))(void *)rec.fns[15];             /* 0x2a1ed0 */
                    g_script_port_profile_set_name = (int (*)(uint64_t, const char *))(void *)rec.fns[16];     /* 0x2a1ed8 */
                    g_script_port_reset = (int (*)(int))(void *)rec.fns[17];                                   /* 0x2a1ee0 */
                    g_script_port_client_editor_snapshot = (int (*)(void *, uint64_t))(void *)rec.fns[18];     /* 0x2a1ee8 */
                    g_script_port_fast_replay_snapshot = (int (*)(uint64_t))(void *)rec.fns[19];               /* 0x2a1ef0 */
                    g_script_port_fast_replay_claim = (int (*)(uint64_t, uint64_t, uint32_t))(void *)rec.fns[20]; /* 0x2a1ef8 */
                    g_script_port_chat_snapshot = (int (*)(uint8_t *, size_t))(void *)rec.fns[21];            /* 0x2a1f00 */
                    g_script_port_hud_snapshot = (int (*)(uint8_t *, size_t))(void *)rec.fns[22];              /* 0x2a1f08 */
                    g_script_port_reset_core = (int (*)(int))(void *)rec.fns[23];                              /* 0x2a1f10 */
                    g_script_port_chat_action = (int (*)(uint32_t))(void *)rec.fns[24];                        /* 0x2a1f18 */
                    g_script_port_server_select = (int (*)(uint32_t))(void *)rec.fns[25];                      /* 0x2a1f20 */
                    g_script_port_module_base = rec.module_base;             /* 0x2a1f28 */
                    g_script_port_query_fn = rec.query_fn;                   /* 0x2a1f30 */
                }
            }
        }
        errno = saved_errno;
    }
}

/* ===== nexus_script_port_* forwarders ===== */

/*
 * script_port_server_select — forward to nexus_script_port_server_select:
 * returns true only when the hook exists and answers nonzero.
 * @ 00191bb0
 */
bool script_port_server_select(uint32_t kind)
{
    script_port_table_load();
    if (g_script_port_server_select == NULL)
        return false;
    return g_script_port_server_select(kind) != 0;
}

/*
 * script_port_camera_snapshot — forward to nexus_script_port_camera_snapshot
 * (out). Returns the hook verdict, or 0 when unbound. @ 00191bf4
 */
uint64_t script_port_camera_snapshot(void *out)
{
    script_port_table_load();
    if (g_script_port_camera_snapshot != NULL)
        return (uint64_t)(uintptr_t)g_script_port_camera_snapshot(out);
    return 0;
}

/*
 * script_port_camera_control — forward to
 * nexus_script_port_camera_control(op, a, b). Returns the hook verdict, or
 * 0 when unbound. @ 00191c34
 */
uint64_t script_port_camera_control(uint32_t op, uint32_t a, uint32_t b)
{
    script_port_table_load();
    if (g_script_port_camera_control != NULL)
        return (uint64_t)g_script_port_camera_control(op, a, b);
    return 0;
}

/*
 * script_port_client_performance_query — forward to
 * nexus_script_port_client_performance_query(arg). Returns the hook
 * verdict, or 0 when unbound. @ 00191c90
 */
uint64_t script_port_client_performance_query(uint64_t arg)
{
    script_port_table_load();
    if (g_script_port_client_performance_query != NULL)
        return (uint64_t)g_script_port_client_performance_query(arg);
    return 0;
}

/*
 * script_port_client_performance_apply — forward to
 * nexus_script_port_client_performance_apply(arg). Returns the hook
 * verdict, or 0 when unbound. @ 00191cd0
 */
uint64_t script_port_client_performance_apply(uint32_t arg)
{
    script_port_table_load();
    if (g_script_port_client_performance_apply != NULL)
        return (uint64_t)g_script_port_client_performance_apply(arg);
    return 0;
}

/*
 * script_port_client_editor_snapshot — forward to
 * nexus_script_port_client_editor_snapshot(a, b). Returns the hook verdict,
 * or 0 when unbound. @ 00191d10
 */
uint64_t script_port_client_editor_snapshot(uint64_t a, uint64_t b)
{
    script_port_table_load();
    if (g_script_port_client_editor_snapshot != NULL)
        return (uint64_t)(uintptr_t)g_script_port_client_editor_snapshot(
            (void *)(uintptr_t)a, b);
    return 0;
}

/*
 * script_port_client_editor_apply — forward to
 * nexus_script_port_client_editor_apply(a, b, c, d, e) (five 32-bit args).
 * Returns the hook verdict, or 0 when unbound. @ 00191d58
 */
uint64_t script_port_client_editor_apply(uint32_t a, uint32_t b, uint32_t c,
                                         uint32_t d, uint32_t e)
{
    script_port_table_load();
    if (g_script_port_client_editor_apply != NULL)
        return (uint64_t)g_script_port_client_editor_apply(a, b, c, d, e);
    return 0;
}

/*
 * script_port_client_editor_tick — forward to
 * nexus_script_port_client_editor_tick(). No return value.
 * @ 00191dd0
 */
void script_port_client_editor_tick(void)
{
    script_port_table_load();
    if (g_script_port_client_editor_tick != NULL)
        g_script_port_client_editor_tick();
}

/*
 * font_chooser_snapshot_read — read the 0x30-byte font-chooser snapshot.
 * out must be non-NULL: the six qwords are zeroed, then the caller's magic
 * qword (rodata 0x10f748, fonts.c's FONT_SNAPSHOT_MAGIC) is stored at +0
 * and 0x500000000 at +8 before the read; the nexus_script_port_font_query
 * hook (when bound) is called as (out, 0x30) and its verdict returned, else
 * 0. @ 00191df8
 */
int font_chooser_snapshot_read(void *out)
{
    uint64_t *rec = out;

    if (out == NULL)
        return 0;
    rec[0] = 0;
    rec[1] = 0;
    rec[2] = 0;
    rec[3] = 0;
    rec[4] = 0;
    rec[5] = 0;
    rec[0] = FONT_SNAPSHOT_MAGIC; /* 0x10f748 — caller pre-fills the qword */
    rec[1] = 0x500000000;
    script_port_table_load();
    if (g_script_port_font_query != NULL)
        return g_script_port_font_query(out, 0x30);
    return 0;
}

/*
 * script_port_font_apply — forward to nexus_script_port_font_apply
 * (font_id). Returns the hook verdict, or 0 when unbound. @ 00191e38
 */
uint64_t script_port_font_apply(uint32_t font_id)
{
    script_port_table_load();
    if (g_script_port_font_apply != NULL)
        return (uint64_t)g_script_port_font_apply(font_id);
    return 0;
}

/*
 * script_port_font_body_current — forward to
 * nexus_script_port_font_body_current(). No return value. @ 00191e78
 */
void script_port_font_body_current(void)
{
    script_port_table_load();
    if (g_script_port_font_body_current != NULL)
        g_script_port_font_body_current();
}

/*
 * battle_snapshot_read — read the 0x30-byte battle snapshot. out must be
 * non-NULL: the six qwords are zeroed, the magic qword (0x10f748) stored at
 * +0 and 0x500000000 at +8; the nexus_script_port_battle_snapshot hook
 * (when bound) is called as (out, 0x30) and its verdict returned, else 0.
 * @ 00191ea0
 */
int battle_snapshot_read(void *out)
{
    uint64_t *rec = out;

    if (out == NULL)
        return 0;
    rec[0] = 0;
    rec[1] = 0;
    rec[2] = 0;
    rec[3] = 0;
    rec[4] = 0;
    rec[5] = 0;
    rec[0] = FONT_SNAPSHOT_MAGIC; /* 0x10f748 — same magic as the font snap */
    rec[1] = 0x500000000;
    script_port_table_load();
    if (g_script_port_battle_snapshot != NULL)
        return g_script_port_battle_snapshot(out, 0x30);
    return 0;
}

/*
 * script_port_profile_snapshot — forward to
 * nexus_script_port_profile_snapshot(a, b, page). Returns the hook verdict,
 * or 0 when unbound. @ 00191f04
 */
uint64_t script_port_profile_snapshot(uint64_t a, uint64_t b, uint32_t page)
{
    script_port_table_load();
    if (g_script_port_profile_snapshot != NULL)
        return (uint64_t)(uintptr_t)g_script_port_profile_snapshot(a, b,
                                                                   page);
    return 0;
}

/*
 * script_port_profile_open — forward to
 * nexus_script_port_profile_open(a, b). Returns the hook verdict, or 0 when
 * unbound. @ 00191f60
 */
uint64_t script_port_profile_open(uint64_t a, uint64_t b)
{
    script_port_table_load();
    if (g_script_port_profile_open != NULL)
        return (uint64_t)(uintptr_t)g_script_port_profile_open(a, b);
    return 0;
}

/*
 * script_port_profile_set_name — forward to
 * nexus_script_port_profile_set_name(a, name). Returns the hook verdict, or
 * 0 when unbound. @ 00191fa8
 */
uint64_t script_port_profile_set_name(uint64_t a, const char *name)
{
    script_port_table_load();
    if (g_script_port_profile_set_name != NULL)
        return (uint64_t)(uintptr_t)g_script_port_profile_set_name(a, name);
    return 0;
}

/*
 * theme_snapshot_read — read one theme snapshot page. The
 * nexus_script_port_theme_snapshot hook (when bound) is called as
 * (buf, size, page) and its verdict returned, else 0. Caller passes size ==
 * 0x2534 (themes.c). @ 00191ff0
 */
int theme_snapshot_read(void *buf, uint32_t size, uint32_t page)
{
    script_port_table_load();
    if (g_script_port_theme_snapshot != NULL)
        return ((int (*)(void *, uint32_t, uint32_t))(void *)g_script_port_theme_snapshot)(
            buf, size, page);
    return 0;
}
/* misc_c chain chunk 7: covers raw lines 19301-19700 */

/*
 * The remaining script-port forwarders (theme_apply through
 * script_port_chat_action), then the evasion memfd module verification
 * family: memfd_module_fd_open (raw 19546), dlsym_module_verified (19671),
 * memfd_fd_path_resolve (19702) and proc_fd_path_format_alt (19823 — the
 * last straddles the 19700 boundary and is finished here through raw
 * 19860).
 */

#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>

/* ---- functions used before their raw definition order ---- */
extern int memfd_fd_path_resolve(const char *path, char *out); /* @ 0019339c */
/* proc_fd_path_format_alt is defined at the end of this chunk (variadic) */
int proc_fd_path_format_alt(char *buf, ...);                    /* @ 001935dc */

/* ===== remaining script-port forwarders ===== */

/*
 * theme_apply — apply a theme through the script-port theme command hook
 * (kind, a, b). Returns the hook verdict, or 0 when unbound.
 * @ 0019204c
 */
uint64_t theme_apply(uint32_t kind, uint32_t a, uint32_t b)
{
    script_port_table_load();
    if (g_script_port_theme_command != NULL)
        return (uint64_t)g_script_port_theme_command(kind, a, b);
    return 0;
}

/*
 * script_port_client_debug_snapshot — forward to
 * nexus_script_port_client_debug_snapshot(a, b). Returns the hook verdict,
 * or 0 when unbound. @ 001920a8
 */
uint64_t script_port_client_debug_snapshot(uint64_t a, uint64_t b)
{
    script_port_table_load();
    if (g_script_port_client_debug_snapshot != NULL)
        return (uint64_t)(uintptr_t)g_script_port_client_debug_snapshot(
            (void *)(uintptr_t)a, b);
    return 0;
}

/*
 * script_port_client_debug_apply — forward to
 * nexus_script_port_client_debug_apply(a, b). Returns the hook verdict, or
 * 0 when unbound. @ 001920f0
 */
uint64_t script_port_client_debug_apply(uint32_t a, uint32_t b)
{
    script_port_table_load();
    if (g_script_port_client_debug_apply != NULL)
        return (uint64_t)g_script_port_client_debug_apply(a, b);
    return 0;
}

/*
 * script_port_fast_replay_snapshot — forward to
 * nexus_script_port_fast_replay_snapshot(a). Returns the hook verdict, or
 * 0 when unbound. @ 00192138
 */
uint64_t script_port_fast_replay_snapshot(uint64_t a)
{
    script_port_table_load();
    if (g_script_port_fast_replay_snapshot != NULL)
        return (uint64_t)(uintptr_t)g_script_port_fast_replay_snapshot(a);
    return 0;
}

/*
 * script_port_fast_replay_claim — forward to
 * nexus_script_port_fast_replay_claim(a, b, c). Returns the hook verdict,
 * or 0 when unbound. @ 00192178
 */
uint64_t script_port_fast_replay_claim(uint64_t a, uint64_t b, uint32_t c)
{
    script_port_table_load();
    if (g_script_port_fast_replay_claim != NULL)
        return (uint64_t)(uintptr_t)g_script_port_fast_replay_claim(a, b, c);
    return 0;
}

/*
 * chat_snapshot_read — read the chat snapshot. out must be non-NULL and
 * size nonzero: the leading status byte is zeroed, then the
 * nexus_script_port_chat_snapshot hook (when bound) is called as (out,
 * size) and its verdict returned, else 0. @ 001921d4
 */
int chat_snapshot_read(uint8_t *out, size_t size)
{
    if (out == NULL || size == 0)
        return 0;
    out[0] = 0;
    script_port_table_load();
    if (g_script_port_chat_snapshot != NULL)
        return g_script_port_chat_snapshot(out, size);
    return 0;
}

/*
 * script_port_reset — forward to nexus_script_port_reset(kind). A reset of
 * kind 0 additionally requires camera_control(2, 0, 0) and
 * camera_control(4, 0, 0) to both succeed (the raw camera re-arm pair);
 * returns 1 only when the whole sequence succeeds, the bare hook verdict
 * otherwise, 0 when unbound. @ 00192228
 */
uint64_t script_port_reset(int kind)
{
    script_port_table_load();
    if (g_script_port_reset_core == NULL)
        return 0;
    {
        uint64_t verdict = (uint64_t)(uintptr_t)g_script_port_reset_core(kind);

        if (verdict != 0) {
            if (kind == 0 && g_script_port_camera_control != NULL) {
                if (g_script_port_camera_control(2, 0, 0) == 0)
                    return verdict;
                if (g_script_port_camera_control(4, 0, 0) == 0)
                    return verdict;
            }
            return 1;
        }
        return verdict;
    }
}

/*
 * hud_snapshot_read — read the HUD snapshot. out must be non-NULL and size
 * nonzero: the leading status byte is zeroed, then the
 * nexus_script_port_hud_snapshot hook (when bound) is called as (out, size)
 * and its verdict returned, else 0. @ 001922ac
 */
int hud_snapshot_read(uint8_t *out, size_t size)
{
    if (out == NULL || size == 0)
        return 0;
    out[0] = 0;
    script_port_table_load();
    if (g_script_port_hud_snapshot != NULL)
        return g_script_port_hud_snapshot(out, size);
    return 0;
}

/*
 * script_port_chat_action — forward to nexus_script_port_chat_action
 * (action). Returns the hook verdict, or 0 when unbound. @ 00192300
 */
uint64_t script_port_chat_action(uint32_t action)
{
    script_port_table_load();
    if (g_script_port_chat_action != NULL)
        return (uint64_t)(uintptr_t)g_script_port_chat_action(action);
    return 0;
}

/* ===== evasion memfd module verification (runtime TU) ===== */

/*
 * memfd_module_fd_open — verify a memfd-backed module and return its fd.
 * fd_path names a "/proc/self/fd/NN" entry whose readlink target must be a
 * "/memfd:nexus-dnexus-su… (deleted)" link (prefix 0x17 bytes; the tail is
 * the module's own name, at most 0xff bytes). Two shapes are accepted:
 * a script module whose name starts "script-it" (0x69726373 +
 * 0x2d747069 little-endian) — then soname must end ".js" or ".ngs", the
 * name must be 0x27 chars with a 0x20-char lowercase-hex id, and sha256_hex
 * must be exactly 0x40 lowercase-hex chars; or the plain module shape —
 * name length in [0x42, 0x100) with '-' at name[0x40], a 0x40-char
 * lowercase-hex id and, when given, soname equal to the name tail and
 * sha256_hex the expected 0x40-char digest. After the name checks the fd
 * path is resolved again, the fd number parsed from the path (strtol, full
 * digit run, 0..0x7fffffff) and — unless it is a regular, uid-owned,
 * nlink-0 file with all 0xf seals — opened via F_GETFD (fnctl 0x406) and
 * returned. Returns the fd, or -1 on any gate failure.
 * @ 00192f58
 */
int memfd_module_fd_open(uint64_t fd_path_addr, const char *soname,
                         const char *sha256_hex)
{
    char fd_path[0x40];
    char link[0x140];
    char name[0x100];
    ssize_t n;

    if (memfd_fd_path_resolve((const char *)(uintptr_t)fd_path_addr,
                              fd_path) == 0)
        return -1;
    n = readlink(fd_path, link, 0x13f);
    if (n < 0 || (uint64_t)(n - 0x13f) > (uint64_t)-0x13f || n <= 0x17)
        return -1;
    link[n] = '\0';
    if (memcmp(link, "/memfd:nexus-", 13) != 0
        || memcmp(link + 13, "dnexus-su", 9) != 0
        || strcmp(link + n - 23, " (deleted)") != 0
        || (size_t)(n - 0x17) >= 0x100)
        return -1;
    memcpy(name, link + 0x17, (size_t)(n - 0x17));
    name[n - 0x17] = '\0';

    if (memcmp(name, "scri", 4) == 0 && memcmp(name + 4, "pt-i", 4) == 0) {
        size_t i;

        /* script module: ".js"/".ngs" soname, 0x27-char name, hex ids */
        if (soname == NULL || sha256_hex == NULL)
            return -1;
        if (strlen(sha256_hex) != 0x40)
            return -1;
        {
            size_t len = strlen(soname);

            if (len <= 2)
                return -1;
            if (strcmp(soname + (len - 3), ".js") != 0
                && !(len > 3 && strcmp(soname + (len - 4), ".ngs") == 0))
                return -1;
        }
        if (strlen(name) != 0x27)
            return -1;
        for (i = 0; i < 0x20; i++) { /* name[7..0x26]: lowercase hex */
            unsigned c = (unsigned char)name[7 + i];

            if (c - 0x30 > 9 && c - 0x61 > 5)
                return -1;
        }
        for (i = 0; sha256_hex[i] != '\0'; i++) { /* digest: lowercase hex */
            unsigned c = (unsigned char)sha256_hex[i];

            if (c - 0x30 > 9 && c - 0x61 > 5)
                return -1;
        }
    } else {
        size_t i;
        size_t len = strlen(name);

        /* plain module: name[0x40] == '-', 0x40-char hex id at name[0x41] */
        if (len < 0x42 || name[0x40] != '-')
            return -1;
        for (i = 0; i < 0x40; i++) {
            unsigned c = (unsigned char)name[i];

            if (c - 0x30 > 9 && c - 0x61 > 5)
                return -1;
        }
        if (soname != NULL && strcmp(soname, name + 0x41) != 0)
            return -1;
        if (sha256_hex != NULL) {
            if (strlen(sha256_hex) != 0x40)
                return -1;
            if (memcmp(sha256_hex, name, 0x40) != 0)
                return -1; /* raw compares all 8 qwords of the digest */
        }
    }

    /* re-resolve the fd path and open the fd */
    if (memfd_fd_path_resolve((const char *)(uintptr_t)fd_path_addr,
                              fd_path) == 0)
        return -1;
    {
        char *end = NULL;
        long fd;

        errno = 0;
        fd = strtol(fd_path + 0xe /* "/proc/self/fd/" */, &end, 10);
        if (errno != 0 || end == NULL || *end != '\0'
            || fd < 0 || fd > 0x7fffffff)
            return -1;
        {
            int flags = fcntl((int)fd, 0x406 /* F_GETFD */);

            if (flags < 0)
                return -1;
            {
                int seals = fcntl(flags, 0x40a /* F_GET_SEALS */);
                struct stat st;

                if (fstat(flags, &st) == 0
                    && (st.st_mode & 0xf000) == 0x8000 /* S_IFREG */
                    && (uid_t)st.st_mode == getuid() /* raw compares uid */
                    && st.st_nlink == 0 && seals >= 0
                    && (seals & 0xf) == 0xf)
                    return -1; /* fully sealed — refuse */
                close(flags);
            }
        }
    }
    return -1;
}

/*
 * dlsym_module_verified — resolve `name' from `handle' and verify with
 * dladdr that the symbol's containing module base equals `base'. Returns
 * the symbol address, or 0 when dlsym fails, dladdr fails or the base
 * differs. @ 00193324
 */
uint64_t dlsym_module_verified(void *handle, const char *name,
                               uint64_t base)
{
    void *sym = dlsym(handle, name);

    if (sym != NULL) {
        Dl_info info;

        if (dladdr(sym, &info) == 0)
            return 0;
        if ((uint64_t)(uintptr_t)info.dli_fbase != base)
            return 0;
    }
    return (uint64_t)(uintptr_t)sym;
}

/*
 * memfd_fd_path_resolve — resolve a memfd name to its /proc/self/fd path.
 * path is either a plain "/proc/self/fd/<digits>" path (total length <
 * 0x40, every char after the prefix must be below ':' — digits) which is
 * copied verbatim to out and yields 1, or a "/memfd:nexus-…" name (length
 * < 0x140) which is looked up by scanning /proc/self/fd readlink targets
 * (names of at most 0x14 bytes, non-'.' entries). The fd path of the unique
 * match is written to out (needs >= 0x40 bytes). Returns 1 when exactly one
 * distinct file (same st_dev + st_ino) carries the name, 0 on no match,
 * divergent duplicates or a stat failure. @ 0019339c
 */
int memfd_fd_path_resolve(const char *path, char *out)
{
    if (path != NULL) {
        if (strncmp(path, "/proc/self/fd/", 0xe) == 0 && path[0xe] != '\0') {
            const unsigned char *p;

            for (p = (const unsigned char *)path + 0xe; *p != '\0'; p++) {
                if (*p >= 0x3a) /* digit gate: reject ':' and above */
                    return 0;
            }
            if (strlen(path) < 0x40) {
                strcpy(out, path);
                return 1;
            }
            return 0;
        }
        if (strncmp(path, "/memfd:nexus-", 0xd) == 0
            && strlen(path) < 0x140) {
            DIR *dirp = opendir("/proc/self/fd");

            if (dirp != NULL) {
                int found = 0;    /* at least one fd matched by link target */
                int diverged = 0; /* second match on a different file, or stat fail */
                ino_t match_ino = 0;
                dev_t match_dev = 0;

                for (;;) {
                    struct dirent *de = readdir(dirp);
                    char fd_path[0x40];
                    char link[0x140];
                    ssize_t len;
                    struct stat st;

                    if (de == NULL)
                        break;
                    if (de->d_name[0] == '.'
                        || strlen(de->d_name) >= 0x15)
                        continue;
                    proc_fd_path_format_alt(fd_path, de->d_name);
                    len = readlink(fd_path, link, 0x13f);
                    if ((uint64_t)(len - 0x13f) < (uint64_t)-0x13f
                        && len < 0x13f) /* -2..0x13e: the raw accept window */
                        continue;
                    link[len] = '\0';
                    if (strcmp(link, path) != 0)
                        continue;
                    if (stat(fd_path, &st) != 0) {
                        diverged = 1;
                        break;
                    }
                    if (found) {
                        if (st.st_dev == match_dev
                            && st.st_ino == match_ino) {
                            strcpy(out, fd_path); /* same file, refresh */
                            continue;
                        }
                        diverged = 1; /* the name maps to more than one file */
                        break;
                    }
                    strcpy(out, fd_path);
                    found = 1;
                    match_ino = st.st_ino;
                    match_dev = st.st_dev;
                }
                closedir(dirp);
                return found && !diverged;
            }
            return 0;
        }
    }
    return 0;
}

/*
 * proc_fd_path_format_alt — build a "/proc/self/fd/<name>" path into buf
 * (0x40 cap). Alt-TU variadic copy of the b-chain's proc_fd_path_format:
 * the name travels in the variadic registers Ghidra could not recover.
 * Returns the vsnprintf length. @ 001935dc
 */
int proc_fd_path_format_alt(char *buf, ...)
{
    va_list ap;
    int len;

    va_start(ap, buf);
    len = vsnprintf(buf, 0x40, "/proc/self/fd/%s", ap);
    va_end(ap);
    return len;
}
/* misc_c chain chunk 8: covers raw lines 19701-20000 */

/*
 * The bionic emutls runtime family (__emutls_get_address, tls_key_delete,
 * tls_key_create, tls_reflist_destructor) plus code_cache_flush_range.
 * proc_fd_path_format_alt (raw 19823-19860) was already finished in chunk 7
 * as the range straddler.
 */

#include <pthread.h>

/* emutls control-variable layout (bionic __emutls_control):
 * {size_t size; size_t align; size_t index; void *init;} */
typedef struct emutls_control {
    size_t size;   /* +0x00 — variable byte size */
    size_t align;  /* +0x08 — alignment (rounded to a power of two) */
    size_t index;  /* +0x10 — assigned TLS slot index (0 = unassigned) */
    void *init;    /* +0x18 — initializer template (NULL = zero fill) */
} emutls_control_t;

/* thread's emutls block: {refcount, capacity(qwords), slot pointers…} */
typedef struct emutls_block {
    uint64_t refcount;  /* +0x00 — destructor deferral count */
    uint64_t capacity;  /* +0x08 — slot pointer capacity in qwords */
    void *slots[];      /* +0x10 — one pointer per control variable */
} emutls_block_t;

/* ===== bionic emutls runtime (0x2a1f40..0x2a1f58) ===== */

uint8_t g_emutls_key_valid;   /* 0x2a1f40 — 1 after tls_key_create succeeded */
pthread_key_t g_emutls_key;   /* 0x2a1f44 — the pthread key for the blocks */
pthread_once_t g_emutls_key_once = PTHREAD_ONCE_INIT; /* 0x2a1f48 */
uint64_t g_emutls_next_index; /* 0x2a1f50 — last handed-out slot index */
pthread_mutex_t g_emutls_index_lock = PTHREAD_MUTEX_INITIALIZER; /* 0x2a1f58 */

/* raw defines the create/destructor pair after their first uses */
extern void tls_key_create(void);                                /* @ 00193880 */
extern void *tls_reflist_destructor(emutls_block_t *block);     /* @ 001938bc */

/*
 * __emutls_get_address — bionic emulated-TLS address resolver. control is
 * the compiler-generated control variable; returns the thread-local storage
 * for it, allocating on demand. Slot indices are handed out under
 * g_emutls_index_lock (first use of a control runs tls_key_create through
 * g_emutls_key_once). The per-thread block {refcount, capacity, slots[]} is
 * grown by realloc (new capacity = align16(index + 0x11) - 2 qwords, zero
 * fill) or freshly malloc'd (refcount 1). The slot's buffer is malloc'd
 * with align + 7 + size bytes, aligned up (align is a power of two, >= 8),
 * the malloc base stashed 8 bytes before the returned pointer and the
 * buffer zero-filled or memcpy'd from control->init. abort() on any
 * allocation failure or a non-power-of-two alignment.
 * @ 0019367c
 */
void *__emutls_get_address(void *control_v)
{
    emutls_control_t *control = control_v;
    size_t index = control->index;
    emutls_block_t *block;
    size_t capacity;

    if (index == 0) {
        /* first use of this control variable: assign its slot index */
        pthread_once(&g_emutls_key_once, tls_key_create);
        pthread_mutex_lock(&g_emutls_index_lock);
        index = control->index;
        if (index == 0) {
            index = g_emutls_next_index + 1;
            g_emutls_next_index = index;
            control->index = index;
        }
        pthread_mutex_unlock(&g_emutls_index_lock);
        block = pthread_getspecific(g_emutls_key);
        if (block == NULL)
            goto fresh_block;
        goto grow;
    }

    block = pthread_getspecific(g_emutls_key);
    if (block != NULL) {
grow:
        capacity = block->capacity;
        if (index <= capacity)
            goto have_block;
        {
            size_t new_cap = ((index + 0x11) & ~(size_t)0xf) - 2;

            block = realloc(block, new_cap * 8 + 0x10);
            if (block == NULL)
                abort();
            memset(block->slots + capacity, 0,
                   (new_cap - capacity) * 8);
        }
        pthread_setspecific(g_emutls_key, block);
        block->capacity = ((index + 0x11) & ~(size_t)0xf) - 2;
        goto have_block;
    }

fresh_block:
    {
        size_t new_cap = ((index + 0x11) & ~(size_t)0xf) - 2;

        block = malloc(new_cap * 8 + 0x10);
        if (block == NULL)
            abort();
        memset(block->slots, 0, new_cap * 8);
        block->refcount = 1;
    }
    pthread_setspecific(g_emutls_key, block);
    block->capacity = ((index + 0x11) & ~(size_t)0xf) - 2;

have_block:
    if (block->slots[index - 1] == NULL) {
        size_t align = control->align;
        size_t size = control->size;
        void *raw;
        void *aligned;

        if (align < 9)
            align = 8;
        if ((align & (align - 1)) != 0)
            abort(); /* alignment must be a power of two */
        raw = malloc(align + 7 + size);
        if (raw == NULL)
            abort();
        aligned = (void *)(((uintptr_t)raw + align + 7) & ~(uintptr_t)align);
        *(void **)((char *)aligned - 8) = raw; /* free() header */
        if (control->init == NULL)
            memset(aligned, 0, size);
        else
            memcpy(aligned, control->init, size);
        block->slots[index - 1] = aligned;
    }
    return block->slots[index - 1];
}

/*
 * tls_key_delete — delete the emutls pthread key once. Returns
 * pthread_key_delete's verdict when the key was valid (and clears the valid
 * flag), otherwise the caller's argument unchanged (the raw leaves x0
 * untouched in that case). @ 00193848
 */
uint64_t tls_key_delete(uint64_t arg)
{
    if (g_emutls_key_valid == 1) {
        arg = (uint64_t)(unsigned)pthread_key_delete(g_emutls_key);
        g_emutls_key_valid = 0;
    }
    return arg;
}

/*
 * tls_key_create — pthread_once body for the emutls key: create
 * g_emutls_key with tls_reflist_destructor as the destructor; abort() if
 * the create fails. @ 00193880
 */
void tls_key_create(void)
{
    if (pthread_key_create(&g_emutls_key,
                           (void (*)(void *))(void *)tls_reflist_destructor) != 0)
        abort();
    g_emutls_key_valid = 1;
}

/*
 * tls_reflist_destructor — per-thread emutls block destructor. While the
 * block's refcount is nonzero it is only decremented and re-registered via
 * pthread_setspecific (deferring the free). At zero the malloc base of
 * every allocated slot buffer is freed (the raw reads the header 8 bytes
 * before each slot pointer), then the block itself. Return value is the
 * pthread_setspecific verdict on the deferral path (the raw leaves it in
 * x0), otherwise the block pointer. @ 001938bc
 */
void *tls_reflist_destructor(emutls_block_t *block)
{
    if (block->refcount != 0) {
        block->refcount--;
        return (void *)(uintptr_t)pthread_setspecific(g_emutls_key, block);
    }
    {
        uint64_t i;
        uint64_t n = block->capacity;

        for (i = 0; i < n; i++) {
            if (block->slots[i] != NULL)
                free(*(void **)((char *)block->slots[i] - 8)); /* malloc header */
        }
    }
    free(block);
    return block;
}

/* ===== code cache maintenance ===== */

uint64_t g_ctr_el0_cache; /* 0x2a1f80 — cached CTR_EL0 (0 = not read yet) */

/* arm64 system-register / cache-maintenance helpers (raw uses
 * UnkSytemRegWrite/Read, DC_CVAU, IC_IVAU and ctr_el0 pseudocode ops) */
static inline uint64_t ctr_el0_read(void)
{
    uint64_t v;

    __asm__ volatile("mrs %0, ctr_el0" : "=r"(v));
    return v;
}

static inline void dc_cvau(uint64_t addr)
{
    __asm__ volatile("dc cvau, %0" : : "r"(addr) : "memory");
}

static inline void ic_ivau(uint64_t addr)
{
    __asm__ volatile("ic ivau, %0" : : "r"(addr) : "memory");
}

static inline void dsb_sy(void)
{
    __asm__ volatile("dsb sy" ::: "memory");
}

/*
 * code_cache_flush_range — flush and invalidate the i/d caches over
 * [start, end). CTR_EL0 is read once (cached in g_ctr_el0_cache): when
 * DIC (bit 28) is clear the data cache is cleaned by virtual address,
 * DC CVAU, in DminLine steps (4 << ((ctr >> 0x10) & 0xf)); a DSB SY
 * follows. When IDC (bit 29) is clear the instruction cache is
 * invalidated by VA, IC IVAU, in IminLine steps (4 << (ctr & 0xf)), the
 * pass closed by another DSB; an instruction synchronization barrier ends
 * the sequence in every path. @ 00193944
 */
void code_cache_flush_range(uint64_t start, uint64_t end)
{
    uint64_t ctr;

    if (g_ctr_el0_cache == 0)
        g_ctr_el0_cache = ctr_el0_read();
    ctr = g_ctr_el0_cache;

    if (((uint32_t)ctr >> 0x1c & 1) == 0) { /* DIC clear: clean D-cache */
        uint64_t line = (uint64_t)(uint32_t)(4 << (ctr >> 0x10 & 0xf));
        uint64_t addr = -line & start;

        while (addr < end) {
            dc_cvau(addr);
            addr += line;
        }
        dsb_sy();
    }
    if ((ctr >> 0x1d & 1) == 0) { /* IDC clear: invalidate I-cache */
        uint64_t line = (uint64_t)(uint32_t)(4 << (ctr & 0xf));
        uint64_t addr = -line & start;

        while (addr < end) {
            ic_ivau(addr);
            addr += line;
        }
        dsb_sy();
    }
    __asm__ volatile("isb" ::: "memory"); /* ISB */
}
/* misc_c chain chunk 9: covers raw lines 20001-20232 */

/*
 * Final chunk of the misc_c chain: cpu_features_decode, the three tail
 * dispatch/teardown trampolines and the export thunk. The raw import-stub
 * tail (raw 20233+: __cxa_finalize, __cxa_atexit, __register_atfork, gettid
 * … halt_baddata stubs) is the coordinator's to replace with includes.
 */

/* 0x2a1f90 — decoded CPU feature bitmask (0 = not decoded yet) */
uint64_t g_cpu_feature_mask; /* 0x2a1f90 */

/* arm64 system-register reads for the feature decode */
static inline uint64_t read_id_aa64pfr1_el1(void)
{
    uint64_t v;

    __asm__ volatile("mrs %0, id_aa64pfr1_el1" : "=r"(v));
    return v;
}

static inline uint64_t read_id_aa64pfr0_el1(void)
{
    uint64_t v;

    __asm__ volatile("mrs %0, id_aa64pfr0_el1" : "=r"(v));
    return v;
}

static inline uint64_t read_id_aa64isar0_el1(void)
{
    uint64_t v;

    __asm__ volatile("mrs %0, id_aa64isar0_el1" : "=r"(v));
    return v;
}

static inline uint64_t read_id_aa64isar1_el1(void)
{
    uint64_t v;

    __asm__ volatile("mrs %0, id_aa64isar1_el1" : "=r"(v));
    return v;
}

/* zcr_el0 read (raw UnkSytemRegRead(3,0,0,4,4) — S3_0_C0_C4_4) */
static inline uint64_t read_zcr_el0(void)
{
    uint64_t v;

    __asm__ volatile("mrs %0, s3_0_c0_c4_4" : "=r"(v));
    return v;
}

/*
 * cpu_features_decode — decode the CPU feature registers into the
 * g_cpu_feature_mask bitmask, once. hwcap is the AT_HWCAP word, auxv the
 * auxv base whose entry at +0x10 carries AT_HWCAP2 (used only when hwcap
 * bit 30 is set). A nonzero g_cpu_feature_mask returns immediately.
 * Bits follow the raw mask layout: hwcap drives the low word
 * (fp/asimd/aes/pmull/sha1/sha2/crc32 atomics … bit 28, bit 23 etc.) and
 * hwcap2 the high word; when hwcap bit 11 is clear the CPU-feature path
 * additionally decodes ID_AA64PFR1_EL1, ID_AA64PFR0_EL1 (crypto ext
 * present when the GIC field is nonzero), ID_AA64ISAR0_EL1 (SHA512/CRRC
 * AES), ID_AA64ISAR1_EL1 (DPB/LOR/LS64/RNDR flags) and the zcr_el0
 * register, folding their fields into the mask (the raw ORs fixed bit
 * patterns such as 0x80000000000, 0x1000000000000, 0x200000000000000,
 * 0x408000000000000 and the SME levels 0x18000000000000/0x38000000000000).
 * The mask always sets bit 0x400000000000000 on the gPFR path.
 * @ 00193aac
 */
void cpu_features_decode(uint64_t hwcap, void *auxv)
{
    uint32_t hwcap_lo = (uint32_t)hwcap;
    uint64_t hwcap2 = 0;
    uint64_t lo_mask;
    uint64_t hi_mask;

    if (g_cpu_feature_mask != 0)
        return;
    if (((uint32_t)(hwcap >> 0x3e) & 1) != 0)
        hwcap2 = *(const uint64_t *)((const char *)auxv + 0x10); /* AT_HWCAP2 */

    /* low word: hwcap field relocations */
    lo_mask = (hwcap & 0x8000000) >> 0x1a;
    if ((hwcap2 & 0x80) != 0)
        lo_mask = 6; /* BC (MTE2) implies both tag bits */
    lo_mask |= (uint64_t)(hwcap_lo << 3) & 0x400;
    lo_mask |= ((hwcap & 0x10) >> 4) << 0xf;
    hi_mask = lo_mask | 0x20;
    if ((hwcap_lo & 0xc0000) == 0xc0000)
        hi_mask = lo_mask; /* both 2:fp16 fields — no extra bit */
    hi_mask = ((hwcap & 0x800000) >> 0x14) | ((hwcap & 0x100000) >> 0x10)
              | hi_mask;
    if ((hwcap & 0x200) != 0)
        hi_mask |= 0x10100;
    hi_mask = ((hwcap & 0x1000) >> 6) | ((hwcap & 0x1000000) >> 7)
              | hi_mask;
    if ((hwcap & 0x4000000) != 0)
        hi_mask |= 0x800000;
    hi_mask = ((uint64_t)(hwcap_lo << 6) & 0x800) | ((hwcap & 8) << 0xb)
              | hi_mask;
    if ((hwcap & 0x40) != 0)
        hi_mask |= 0x1000;
    hi_mask = ((hwcap & 0x6000) << 7) | hi_mask;
    if ((hwcap & 0x20000000) != 0)
        hi_mask |= 0x400000000000;
    hi_mask = (hwcap & 0x10000000) << 0x15 | hi_mask;

    /* high word: hwcap2 field relocations (shifted up by 0x23) */
    {
        uint64_t h2 = hwcap2 << 0x23;
        uint64_t mid;

        mid = h2 & 0x2000000000;
        if ((hwcap2 & 0x40000) != 0)
            hi_mask |= 0x180000000000;
        if ((hwcap2 & 0x400000) != 0)
            hi_mask |= 0x380000000000;
        if ((hwcap2 & 8) != 0)
            mid = 0x6000000000;
        hi_mask = (h2 & 0x8000000000) | mid | hi_mask;
        if ((hwcap2 & 0x20) != 0)
            hi_mask |= 0x10000000000;
        mid = ((hwcap & 0x100) >> 1) | (h2 & 0x20000000000) | hi_mask
              | ((hwcap2 & 1) << 0x13);
        if ((hwcap2 & 0x10000) != 0)
            mid |= 1;
        mid = ((uint64_t)(uint32_t)((int32_t)hwcap2 << 10) & 0x2000000)
              | ((hwcap2 >> 1) & 0x100000000)
              | ((hwcap2 >> 4) & 0x10000000)
              | ((uint64_t)(uint32_t)((int32_t)hwcap2 << 0xd) & 0x4000000)
              | mid;
        if ((hwcap2 & 0x100) != 0)
            mid |= 0x1000000;
        mid = (hwcap2 & 0x2000000) << 0x1e
              | (hwcap2 & 0x1000000) << 0x20
              | (hwcap2 & 0x800000) << 0x13
              | (hwcap2 & 0x80000000) << 0x17
              | ((uint64_t)(uint32_t)((int32_t)hwcap2 << 8) & 0x20000000)
              | ((hwcap2 & 0x20000) << 0x21)
              | (hwcap2 & 0xe00) << 0x18
              | mid;
        hi_mask = mid;
    }

    if ((hwcap_lo >> 0xb & 1) == 0) {
        /* gPFR path: hwcap bit 11 clear — decode the ID registers */
        if ((hwcap & 0x201) != 0)
            hi_mask |= 0x300;
        if ((hwcap2 & 1) != 0 || (hwcap & 0x10000) != 0)
            hi_mask |= 0x40000;
        if ((hwcap & 0x4008000) != 0)
            hi_mask |= 0x400000;
        if ((hwcap2 & 0x100004000) != 0)
            hi_mask |= 0x8000000;
        hi_mask |= (uint64_t)(uint32_t)((int32_t)hwcap2 << 0x13)
                   & 0x80000000;
        if ((hwcap2 & 2) != 0 && (hwcap & 0x400000) != 0)
            hi_mask |= 0x1000000000;
        g_cpu_feature_mask = hi_mask | 0x400000000000000;
        return;
    }

    /* full feature path: PFR1/PFR0/ISAR0/ISAR1/zcr */
    {
        uint64_t pfr1 = read_id_aa64pfr1_el1();
        uint64_t mask = hi_mask;

        if ((pfr1 & 0xf00) != 0)
            mask |= 0x80000000000;
        {
            uint64_t with = mask | 0x1000000000000;

            if ((pfr1 & 0xf0) != 0x10)
                with = mask;
            {
                uint64_t pfr0 = read_id_aa64pfr0_el1();
                uint64_t m2 = with | 0x200000000000000;

                if ((pfr1 & 0xf000000) != 0x2000000)
                    m2 = with;
                if ((~(uint32_t)pfr0 & 0xf0000) != 0)
                    m2 |= 0x300;
                if ((pfr0 & 0xf00000000) != 0) {
                    uint64_t zcr = read_zcr_el0();

                    if ((zcr & 0xf) == 1)
                        m2 |= 0x1000000000;
                    else if ((zcr & 0xf) == 0)
                        m2 |= 0x40000000;
                    if ((zcr & 0xf00000) != 0)
                        m2 |= 0x80000000;
                }
                {
                    uint64_t isar0 = read_id_aa64isar0_el1();

                    if ((isar0 & 0xf00000000) != 0)
                        m2 |= 0x2000;
                }
                {
                    uint64_t isar1 = read_id_aa64isar1_el1();

                    if ((isar1 & 0xf) != 0)
                        m2 |= 0x40000;
                    if ((isar1 & 0xf00000) != 0)
                        m2 |= 0x400000;
                    {
                        uint64_t with2 = m2 | 0x800000000000;

                        if ((isar1 & 0xf0000000000) != 0x20000000000)
                            with2 = m2;
                        if ((isar1 & 0xf00000000000) != 0)
                            with2 |= 0x8000000;
                        if (isar1 >> 0x3c == 0) {
                            g_cpu_feature_mask =
                                with2 | 0x400000000000000;
                            return;
                        }
                        if (isar1 >> 0x3d == 0) {
                            g_cpu_feature_mask =
                                with2 | 0x408000000000000;
                            return;
                        }
                        {
                            uint64_t sme = 0x38000000000000;

                            if (isar1 >> 0x3c < 3)
                                sme = 0x18000000000000;
                            g_cpu_feature_mask =
                                with2 | sme | 0x400000000000000;
                            return;
                        }
                    }
                }
            }
        }
    }
}

/* ===== tail dispatch / teardown trampolines ===== */

/* loader-relocated function-pointer slots */
void (*g_teardown_fn_slot)(void);          /* 0x1a3738 — registered teardown fn */
void (*g_performance_mode_slot)(void);     /* 0x1a37a8 — performance-mode getter slot */

/* forward decls from chunk 1 */
extern int spin_slot_record_store(uint64_t gate_word, uint64_t gate_mask,
                                  const spin_slot_record_t *record,
                                  uint32_t slot);            /* @ 00184004 */

/*
 * spin_state_dispatch — re-enter the spin record store with the gate word
 * reloaded from the backend state block. state is the backend state block
 * (raw 0x284000); the qword at +0x9e8 replaces the gate word, while the
 * gate mask, record and slot travel in the caller's live registers (the
 * raw is a tail-split continuation of nexus_menu_register_backend — x9,
 * x19/x20/w21 are that caller's locals; the explicit parameters below
 * model them). Returns the store verdict (1 filed, 4 rejected/gated).
 * @ 00194040
 */
uint64_t spin_state_dispatch(const void *state, uint64_t gate_mask,
                             const spin_slot_record_t *record, uint32_t slot)
{
    uint64_t gate_word = *(const uint64_t *)((const char *)state + 0x9e8);

    return (uint64_t)(unsigned)spin_slot_record_store(gate_word, gate_mask,
                                                      record, slot);
}

/*
 * teardown_fn_call — invoke the single registered teardown function
 * pointer (0x1a3738) with no arguments. No return value.
 * @ 00194050
 */
void teardown_fn_call(void)
{
    if (g_teardown_fn_slot != NULL)
        g_teardown_fn_slot();
}

/*
 * nexus_ui_performance_mode__export — dlsym export thunk: tail-calls the
 * function installed in the 0x1a37a8 slot (the real getter is
 * nexus_ui_performance_mode @ 0014d044 in the a-chain).
 * — dlsym export thunk @ 00194140
 */
void nexus_ui_performance_mode__export(void)
{
    if (g_performance_mode_slot != NULL)
        g_performance_mode_slot();
}
/*
 * ===== import stubs (raw lines 20233-21346) =====
 *
 * The raw dump ends with 94 `halt_baddata()` import thunks — PLT entries for
 * libc/bionic/framework symbols reached through the GOT. They are not code of
 * this module and are reconstructed as the standard headers below; every raw
 * reference to these symbols resolves through the platform includes:
 *
 *   __cxa_atexit __register_atfork gettid strnlen __errno __stack_chk_fail
 *   memset __memchr_chk memcpy clock_gettime __android_log_print strcmp strlen
 *   pthread_self pthread_getname_np getpid syscall dlsym dladdr pthread_once
 *   memcmp openat __write_chk fsync close renameat unlinkat __open_2 read
 *   dl_iterate_phdr sysconf fstat mkdirat __openat_2 opendir dirfd readdir
 *   fstatat closedir strncmp strstr mprotect fopen fgets sscanf ferror clearerr
 *   fclose dlopen dlclose __vsnprintf_chk __read_chk __strchr_chk realpath
 *   geteuid readlink __strlen_chk dlerror stat strcpy strtol fcntl malloc free
 *   calloc pthread_create pthread_detach memchr pthread_mutex_lock
 *   pthread_mutex_unlock pthread_join usleep mmap munmap feof strtoll
 *   vsnprintf pthread_mutex_trylock exp expm1 memmove __pread_chk
 *   pthread_getspecific realloc pthread_setspecific pthread_key_delete
 *   pthread_key_create getauxval __system_property_get
 *
 * (android/log.h supplies __android_log_print; sys/auxv.h getauxval;
 * sys/system_properties.h __system_property_get.)
 */

/* #include <android/log.h>     — bionic-only, absent from the host gcc
 * #include <sys/auxv.h>       — ditto
 * #include <sys/system_properties.h> — ditto
 * The fortified/__-prefixed imports are declared at their use sites. */