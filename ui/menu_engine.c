/* menu_engine chain chunk 1: covers raw lines 1-526 */
/*
 * menu_engine — UI subsystem
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
 *   - ballAssistEnabled "Ball assist" [free]
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
 * Notes: Action-id dispatch (TRY_MENU_ACT_*), sections, snapshots, import/export, profile pump.
 */

/*
 * Reconstructed: ui/menu_engine.c is rebuilt in part chains (p1..p4); this is
 * part 1, chunk 1, covering raw dump lines 1-526 (the nominal 400-line
 * boundary falls inside menu_context_init, which runs raw 336-526 and is
 * finished here). Contents: the menu-store entry RPC family (sequence ids,
 * entry register / value read / entry remove through the service session) and
 * the shared menu context initializer (Mainloop thread gate, module/stage
 * identity, settings 0x25/0x62/99, the evasion 'autofarmEnabled' snapshot and
 * the remote stage-graph walk with generation bumping). Later p1 chunks add
 * the preflight/mainloop pumps, diagnostics, performance, settings save/load,
 * stage log, evasion backend and backend registration, then the main frame
 * tick; parts 2-4 complete the file.
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
#include <dlfcn.h>

/* ===== cross-file functions ===== */
extern int proc_mem_read(void *ctx, uint64_t addr, void *out,
                         uint32_t len);           /* misc.c @ 0014e7c4 — remote
                                                        read (pread64 / process_vm_readv);
                                                        the ctx handle is a pass-through
                                                        (Ghidra shows different register
                                                        leftovers at call sites); nonzero
                                                        on success */
extern uint64_t remote_read_ptr(uint64_t addr);   /* misc.c @ 00154cf0 — read a remote
                                                        qword, 0 unless plausible+aligned */
extern int remote_child_link_valid(uint64_t node,
                                   uint64_t parent); /* misc.c @ 00154d5c — node->parent
                                                        == parent and
                                                        parent->children[node->index]==node */
extern void protected_plus_resolve_once(void);    /* misc.c @ 001552c0 — pthread_once body:
                                                        dl_iterate_phdr resolve of the
                                                        protected-plus query fn */
extern int64_t bind_state_value_read(void *state); /* misc.c @ 00169574 — current bind
                                                        value (default 8 when unarmed) */

/* ===== forward declarations (defined later in this file, parts 3/4) ===== */
bool nexus_menu_server_thread(int tid);           /* @ 00185e94 — true when tid owns the
                                                        menu (g_theme_menu_id 0x284694) */
uint64_t nexus_menu_setting_value(unsigned int id, int32_t *value_out); /* @ 001841f0 —
                                                        1 on success, 3 busy, 4 bad args */

/* ===== shared globals (owned by other files) ===== */
extern int64_t  g_proc_mem_bias;        /* 0x22d8e0 — misc.c: remote-read bias (libg base) */
extern uint64_t g_launcher_stage_view;  /* 0x22db60 — renderer.c: stashed stage view (root child) */
extern uint32_t g_launcher_menu_id;     /* 0x22f038 — renderer.c: owning menu id / Mainloop tid */
extern int32_t  g_launcher_attached;    /* 0x22f03c — renderer.c: 1 attached, -1 failed, 0 pending */
extern uint8_t  g_mainloop_armed;       /* 0x22d8d8 — misc.c: 1 once the Mainloop thread is verified */
extern int64_t  g_stage_generation;     /* 0x1a7c48 — misc.c: stage generation counter (-1 = reset);
                                           pairs with g_launcher_stage_view */
extern uint8_t  g_bind_state[];         /* 0x22f040 — misc.c: shared bind state (wake +4,
                                           value +8, armed +0x124) */
extern uint64_t g_launcher_stage_root;  /* 0x22f178 — renderer.c: stashed stage root held across waits */
extern uint64_t g_launcher_view;        /* 0x22f180 — renderer.c: 0x260-byte rich launcher view object */
extern pthread_once_t g_protected_plus_once; /* 0x22f1d0 — misc.c: once-flag for the
                                                protected-plus resolver */
extern int (*g_protected_plus_query)(void); /* 0x22f1d8 — misc.c: resolved
                                                nexus_protected_plus_active fn */

/* ===== menu_engine part 1 globals ===== */

/* menu-store entry RPC block. The store is reached through the service
 * locator below; every entry op passes the domain token plus the
 * operation-specific token to the session slots. */
int32_t  g_menu_server_seq;       /* 0x22d830 — menu-server sequence counter (cap 0x7fffffff) */
uint64_t g_menu_store_domain;     /* 0x22d838 — store domain token (non-zero gate) */
uint64_t g_menu_store_register;   /* 0x22d840 — register-operation token (slot 0x3a8) */
uint64_t g_menu_store_read;       /* 0x22d848 — read-operation token (slot 0x390) */
uint64_t g_menu_store_remove;     /* 0x22d850 — remove-operation token (slot 0x468) */
void    *g_menu_service;          /* 0x22f028 — service locator object (vtable: acquire
                                     +0x30 flags 0x10006, fallback acquire +0x20,
                                     release +0x28; sessions carry their own table) */
uint64_t g_menu_ctx_default_limits; /* 0x10f928 — packed {limit_0x62, limit_99} default pair
                                       seeded into the shared menu context */
int (*g_evasion_snapshot_fn)(const char **keys, uint64_t *record, int key_count,
                             int64_t *value, int *errcode); /* 0x22f188 — dlsym'd
                                     "nexus_evasion_snapshot_keys_v1" (verified module) */
void    *g_evasion_dl_handle;     /* 0x22f0c0 — dlsym handle for the evasion provider */
uint64_t g_evasion_module_base;   /* 0x22f0c8 — dladdr base of the evasion provider module */

/* remote stage-graph cache: refreshed by menu_context_init; a changed identity
 * (or a shrunken child count) bumps the shared stage generation. */
uint64_t g_stage_root_obj;        /* 0x22f1a0 — root object at bias+0x1307e20 (survives clears) */
uint64_t g_stage_widget_obj;      /* 0x22f190 — widget object (root + 0x48) */
uint64_t g_stage_child_obj;       /* 0x22f198 — child object (widget + 0x28) */
uint64_t g_stage_child_size;      /* 0x22f1a8 — child count/size (child + 0xb8) */

/* ===== menu-store service veneers =====
 * The arm64 code calls the locator/session objects through raw function-table
 * slots; each veneer keeps the slot offset as the plain byte number seen in
 * the dump (index = slot / 8). Suffix _me1 keeps these chain-local
 * (menu_engine part 1). */
static long svc_call0_me1(void *obj, uint32_t slot)
{
    long *table = *(long **)obj;
    long (*fn)(void *) = (long (*)(void *))table[slot / 8];
    return fn(obj);
}

static long svc_call1_me1(void *obj, uint32_t slot, long a)
{
    long *table = *(long **)obj;
    long (*fn)(void *, long) = (long (*)(void *, long))table[slot / 8];
    return fn(obj, a);
}

static long svc_call2_me1(void *obj, uint32_t slot, long a, long b)
{
    long *table = *(long **)obj;
    long (*fn)(void *, long, long) = (long (*)(void *, long, long))table[slot / 8];
    return fn(obj, a, b);
}

static long svc_call3_me1(void *obj, uint32_t slot, long a, long b, long c)
{
    long *table = *(long **)obj;
    long (*fn)(void *, long, long, long) =
        (long (*)(void *, long, long, long))table[slot / 8];
    return fn(obj, a, b, c);
}

static long svc_call4_me1(void *obj, uint32_t slot, long a, long b, long c, long d)
{
    long *table = *(long **)obj;
    long (*fn)(void *, long, long, long, long) =
        (long (*)(void *, long, long, long, long))table[slot / 8];
    return fn(obj, a, b, c, d);
}

static long svc_call7_me1(void *obj, uint32_t slot, long a, long b, long c,
                          long d, long e, long f, long g)
{
    long *table = *(long **)obj;
    long (*fn)(void *, long, long, long, long, long, long, long) =
        (long (*)(void *, long, long, long, long, long, long, long))table[slot / 8];
    return fn(obj, a, b, c, d, e, f, g);
}

/* ===== menu-store entry RPC ===== */

/*
 * menu_server_seq_next — next sequence id for menu-store entries.
 * Only the menu server thread is served: the counter at 0x22d830 is
 * incremented unless it already hit its 0x7fffffff cap. Returns the new id,
 * or 0 on any other thread / at the cap. @ 0014c510
 */
int menu_server_seq_next(void)
{
    int seq = 0;

    if (nexus_menu_server_thread(gettid()) && g_menu_server_seq != 0x7fffffff) {
        g_menu_server_seq = g_menu_server_seq + 1;
        seq = g_menu_server_seq;
    }
    return seq;
}

/*
 * menu_entry_register — register one menu entry in the store.
 * Gates: id >= 1, key 1..0x180 chars (raw: key_len - 0x181 <
 * 0xfffffffffffffe80), desc 0..0x400 chars, type <= 2, screen 1..0x100
 * (raw: screen - 0x101U >= 0xffffff00). Needs the service locator + domain
 * token and the menu server thread; acquires a session (flags 0x10006 with a
 * fallback path), begins a write transaction (slot 0x98, mode 4), stages the
 * key/desc strings through buffer slots 0x580/0x680 and files the entry via
 * slot 0x3a8 with the register token. Returns true only when filing
 * returned 1 with no latched session error. errno is preserved. @ 0014c550
 */
bool menu_entry_register(int id, char *key, char *desc, uint32_t type, int screen)
{
    bool ok = false;
    void *session = NULL;
    long name_h = 0;
    long desc_h = 0;
    size_t key_len;
    size_t desc_len;
    bool fast_acquired;
    int rc;
    int saved_errno;

    if (type > 2 || id < 1 || key == NULL || desc == NULL
        || screen < 1 || screen > 0x100)
        return false;

    key_len = strnlen(key, 0x181);
    desc_len = strnlen(desc, 0x401);
    if (key_len < 1 || key_len > 0x180 || desc_len > 0x400)
        return false;

    saved_errno = errno;
    if (g_menu_service == NULL || g_menu_store_domain == 0)
        goto out;
    if (!nexus_menu_server_thread(gettid()))
        goto out;

    rc = (int)svc_call2_me1(g_menu_service, 0x30, (long)&session, 0x10006);
    if (rc == 0) {
        fast_acquired = true;
    } else if (rc != -2
               || (int)svc_call2_me1(g_menu_service, 0x20, (long)&session, 0) != 0) {
        goto out;
    } else {
        fast_acquired = false;
    }

    if (session == NULL)
        goto out;

    if ((int)svc_call1_me1(session, 0x98, 4) != 0) { /* begin write txn */
        if (svc_call0_me1(session, 0x720) != 0)
            svc_call0_me1(session, 0x88);
        if (!fast_acquired)
            svc_call0_me1(g_menu_service, 0x28);
        goto out;
    }

    name_h = svc_call1_me1(session, 0x580, (long)key_len);
    if (name_h != 0) {
        if (svc_call0_me1(session, 0x720) == 0
            && (desc_h = svc_call1_me1(session, 0x580, (long)desc_len)) != 0
            && svc_call0_me1(session, 0x720) == 0) {
            svc_call4_me1(session, 0x680, name_h, 0, (long)key_len, (long)key);
            if (desc_len != 0)
                svc_call4_me1(session, 0x680, desc_h, 0, (long)desc_len, (long)desc);
            if (svc_call0_me1(session, 0x720) == 0)
                ok = svc_call7_me1(session, 0x3a8,
                                   (long)g_menu_store_domain,
                                   (long)g_menu_store_register,
                                   id, name_h, desc_h, (long)type, screen) == 1;
        }
    }

    if (svc_call0_me1(session, 0x720) != 0)
        ok = false;
    svc_call1_me1(session, 0xa0, 0); /* commit */
    if (svc_call0_me1(session, 0x720) != 0)
        svc_call0_me1(session, 0x88);
    if (!fast_acquired)
        svc_call0_me1(g_menu_service, 0x28);

out:
    errno = saved_errno;
    return ok;
}

/* 0x410-byte store record returned by the read op: 0x10-byte header plus a
 * raw payload (no NUL inside; the reader appends one). */
typedef struct {
    int32_t  magic;          /* +0x00 — must be 1 */
    uint32_t id;             /* +0x04 — must match the requested id */
    uint32_t kind;           /* +0x08 — entry kind (1 = string), must be < 4 */
    uint32_t size;           /* +0x0c — payload size, must equal size query - 0x10 */
    char     payload[0x400]; /* +0x10 */
} menu_entry_record_t;       /* 0x410 */

/*
 * menu_entry_value_read — read a menu entry's string value by id.
 * On gate failure (id < 1, null outs, zero cap) the outputs are untouched;
 * otherwise kind_out is preset to 3 and value_out[0] to NUL. With the
 * session (read txn, slot 0x98 mode 2) the record is opened via slot 0x390
 * with the read token, size-queried (slot 0x558, payload = size - 0x10,
 * cap 0x400) and read (slot 0x640); the header must be {1, id, kind < 4,
 * payload size} and the payload NUL-free. Returns 1 on success (payload
 * copied, NUL-terminated, kind_out set), 0 otherwise. errno preserved.
 * @ 0014c8ac
 */
int menu_entry_value_read(int id, uint32_t *kind_out, char *value_out,
                          size_t value_cap)
{
    menu_entry_record_t rec;
    void *session = NULL;
    bool fast_acquired;
    int rc;
    int saved_errno;

    if (id < 1 || kind_out == NULL || value_out == NULL || value_cap == 0)
        return 0;

    *kind_out = 3;
    value_out[0] = '\0';

    saved_errno = errno;
    if (g_menu_service == NULL || g_menu_store_domain == 0)
        goto out;
    if (!nexus_menu_server_thread(gettid()))
        goto out;

    rc = (int)svc_call2_me1(g_menu_service, 0x30, (long)&session, 0x10006);
    if (rc == 0) {
        fast_acquired = true;
    } else if (rc != -2
               || (int)svc_call2_me1(g_menu_service, 0x20, (long)&session, 0) != 0) {
        goto out;
    } else {
        fast_acquired = false;
    }

    if (session == NULL)
        goto out;

    if ((int)svc_call1_me1(session, 0x98, 2) != 0) { /* begin read txn */
        if (svc_call0_me1(session, 0x720) != 0)
            svc_call0_me1(session, 0x88);
        if (!fast_acquired)
            svc_call0_me1(g_menu_service, 0x28);
        goto out;
    }

    {
        long handle = svc_call3_me1(session, 0x390,
                                     (long)g_menu_store_domain,
                                     (long)g_menu_store_read, id);
        memset(&rec, 0, sizeof rec);
        if (handle != 0 && svc_call0_me1(session, 0x720) == 0) {
            int size_full = (int)svc_call1_me1(session, 0x558, handle);
            uint32_t payload_len = (uint32_t)(size_full - 0x10);

            if (payload_len <= 0x400 && svc_call0_me1(session, 0x720) == 0) {
                svc_call4_me1(session, 0x640, handle, 0, size_full, (long)&rec);
                if (svc_call0_me1(session, 0x720) == 0
                    && rec.magic == 1 && rec.id == (uint32_t)id
                    && rec.kind < 4 && rec.size == payload_len
                    && value_cap > payload_len
                    && (rec.kind == 1 || payload_len == 0)
                    && memchr(rec.payload, '\0', payload_len) == NULL) {
                    memcpy(value_out, rec.payload, payload_len);
                    value_out[payload_len] = '\0';
                    *kind_out = rec.kind;
                    svc_call1_me1(session, 0xa0, 0); /* commit */
                    if (svc_call0_me1(session, 0x720) != 0)
                        svc_call0_me1(session, 0x88);
                    if (!fast_acquired)
                        svc_call0_me1(g_menu_service, 0x28);
                    errno = saved_errno;
                    return 1;
                }
            }
        }
    }

    svc_call1_me1(session, 0xa0, 0); /* commit */
    if (svc_call0_me1(session, 0x720) != 0)
        svc_call0_me1(session, 0x88);
    if (!fast_acquired)
        svc_call0_me1(g_menu_service, 0x28);

out:
    errno = saved_errno;
    return 0;
}

/*
 * menu_entry_remove — remove a menu entry by id from the store.
 * No-op for id < 1. Otherwise acquires the service session (same flags
 * 0x10006 + fallback dance, no transaction begun) and issues the remove
 * through slot 0x468 with the remove token; the session is destroyed on a
 * latched error and released when the fallback path was used. errno is
 * preserved. @ 0014cc0c
 */
void menu_entry_remove(int id)
{
    void *session = NULL;
    bool fast_acquired;
    int rc;
    int saved_errno;

    if (id < 1)
        return;

    saved_errno = errno;
    if (g_menu_service == NULL || g_menu_store_domain == 0)
        goto out;
    if (!nexus_menu_server_thread(gettid()))
        goto out;

    rc = (int)svc_call2_me1(g_menu_service, 0x30, (long)&session, 0x10006);
    if (rc == 0) {
        fast_acquired = true;
    } else if (rc != -2
               || (int)svc_call2_me1(g_menu_service, 0x20, (long)&session, 0) != 0) {
        goto out;
    } else {
        fast_acquired = false;
    }

    if (session != NULL) {
        svc_call3_me1(session, 0x468, (long)g_menu_store_domain,
                      (long)g_menu_store_remove, id);
        if (svc_call0_me1(session, 0x720) != 0)
            svc_call0_me1(session, 0x88);
        if (!fast_acquired)
            svc_call0_me1(g_menu_service, 0x28);
    }

out:
    errno = saved_errno;
}

/* ===== shared menu context ===== */

/* 0x38-byte context snapshot refreshed (rate-limited by callers to ~1 Hz)
 * while the menu is running on the Mainloop thread. */
typedef struct {
    uint64_t refresh_ms;        /* +0x00 — timestamp of this refresh (ms) */
    uint64_t generation;        /* +0x08 — stage generation counter snapshot */
    uint64_t stage_view;        /* +0x10 — stashed stage view (g_launcher_stage_view) */
    uint64_t widget_obj;        /* +0x18 — cached remote widget object */
    uint32_t autofarm_enabled;  /* +0x20 — evasion 'autofarmEnabled' verdict */
    uint32_t evasion_armed;     /* +0x24 — protected-plus verified + record kind ok */
    uint32_t autofarm_gated;    /* +0x28 — autofarm_verified && setting 0x25 == 1 */
    uint32_t stage_byte;        /* +0x2c — byte at stage_root + 0x19c */
    uint32_t limit_0x62;        /* +0x30 — setting 0x62 (default 3500 / 0xdac) */
    uint32_t limit_99;          /* +0x34 — setting 99 (default 2000) */
} menu_context_t;               /* 0x38 */

/*
 * menu_context_init — refresh the shared menu context snapshot.
 * Always seeds the frame (timestamp, generation, stage view, default limits
 * from 0x10f928) and zeroes the verdict fields; everything else is gated on
 * the Mainloop thread (armed flag, owning tid, thread name "Mainloop"), a
 * bound target of 7, the module/stage identity chain (qword at
 * bias+0x12eb9f0 == stashed stage root, its +0x90 == stashed stage view,
 * launcher view still parented) and the stage byte at stage root + 0x19c.
 * Then reads settings 0x25/0x62/99, takes the evasion snapshot for
 * 'autofarmEnabled' (resolving the provider via dlsym + dladdr module
 * check, and protected-plus via pthread_once) and walks the remote stage
 * graph (root at bias+0x1307e20, count at +0x50 must be 0..0x40; count 5
 * walks +0x48 -> +0x28 -> size at +0xb8), bumping the shared stage
 * generation whenever the cached identity changes or the child count
 * shrinks. Side effects: caches the evasion fn, the stage-graph quad.
 * @ 0014ef90
 */
void menu_context_init(menu_context_t *ctx, uint64_t now_ms)
{
    uint64_t stage_view = g_launcher_stage_view;
    uint64_t defaults = g_menu_ctx_default_limits;
    int32_t setting_0x25 = 0;
    int32_t limit_0x62 = 0xdac; /* 3500 */
    int32_t limit_99 = 2000;
    uint8_t stage_byte = 1;
    int owning_tid = (int)g_launcher_menu_id;
    uint32_t autofarm_enabled;
    uint32_t evasion_armed;
    uint32_t autofarm_verified;
    char thread_name[0x10];
    int64_t bind_target;
    uint64_t stage_root;
    uint64_t view_check;
    bool settings_ok;

    ctx->autofarm_enabled = 0;
    ctx->evasion_armed = 0;
    ctx->autofarm_gated = 0;
    ctx->stage_byte = 0;
    ctx->refresh_ms = now_ms;
    ctx->generation = (uint64_t)g_stage_generation;
    ctx->stage_view = stage_view;
    ctx->widget_obj = 0;
    memcpy(&ctx->limit_0x62, &defaults, sizeof defaults);

    if (g_mainloop_armed != 1)
        return;
    if (g_launcher_menu_id < 1 || gettid() != owning_tid)
        return;

    memset(thread_name, 0, sizeof thread_name);
    if (pthread_getname_np(pthread_self(), thread_name, sizeof thread_name) != 0)
        return;
    if (memcmp(thread_name, "Mainloop", 8) != 0 /* 0x706f6f6c6e69614d */
        || thread_name[8] != '\0')
        return;
    if (g_launcher_attached < 1)
        return;
    bind_target = bind_state_value_read(g_bind_state);
    if ((int)bind_target != 7)
        return;

    /* module/stage identity: qword at bias+0x12eb9f0 must be the stashed
     * stage root, whose +0x90 must be the stashed stage view */
    stage_root = 0;
    if (proc_mem_read((void *)(intptr_t)bind_target,
                      (uint64_t)g_proc_mem_bias + 0x12eb9f0, &stage_root, 8) == 0
        || (stage_root & 7) != 0 || stage_root < 0x1000)
        stage_root = 0;
    if (stage_root != g_launcher_stage_root)
        return;

    view_check = 0;
    if (proc_mem_read((void *)(intptr_t)bind_target, stage_root + 0x90,
                      &view_check, 8) == 0
        || (view_check & 7) != 0 || view_check < 0x1000)
        view_check = 0;
    if (view_check != g_launcher_stage_view)
        return;
    if (remote_child_link_valid(g_launcher_view, g_launcher_stage_view) == 0)
        return;
    if (proc_mem_read((void *)(intptr_t)bind_target,
                      g_launcher_stage_root + 0x19c, &stage_byte, 1) == 0)
        return;
    ctx->stage_byte = stage_byte;

    /* settings 0x25 / 0x62 / 99 gate the autofarm verdict */
    settings_ok = false;
    if (nexus_menu_setting_value(0x25, &setting_0x25) == 1
        && nexus_menu_setting_value(0x62, &limit_0x62) == 1)
        settings_ok = nexus_menu_setting_value(99, &limit_99) == 1;

    /* evasion snapshot for 'autofarmEnabled'; the provider fn is resolved
     * once via dlsym and must live in the expected module (dladdr base) */
    {
        int (*snapshot_fn)(const char **, uint64_t *, int, int64_t *, int *) =
            g_evasion_snapshot_fn;
        Dl_info info;

        if (snapshot_fn == NULL) {
            snapshot_fn = (int (*)(const char **, uint64_t *, int, int64_t *, int *))
                dlsym(g_evasion_dl_handle, "nexus_evasion_snapshot_keys_v1");
            if (snapshot_fn == NULL || dladdr(snapshot_fn, &info) == 0
                || (uint64_t)info.dli_fbase != g_evasion_module_base)
                snapshot_fn = NULL;
        }

        if (snapshot_fn == NULL) {
            autofarm_verified = 0;
            evasion_armed = 0;
            autofarm_enabled = 0;
        } else {
            const char *keys[1] = { "autofarmEnabled" };
            uint64_t record[2]; /* {lo dword, enabled dword, kind dword, pad} */
            int64_t value = 0;
            int errcode = -1;

            g_evasion_snapshot_fn = snapshot_fn;
            if (snapshot_fn(keys, record, 1, &value, &errcode) != 1 || value == 0) {
                autofarm_verified = 0;
                evasion_armed = 0;
                autofarm_enabled = 0;
            } else {
                autofarm_enabled = errcode == 0 ? settings_ok : 0;
                if (autofarm_enabled == 1) {
                    pthread_once(&g_protected_plus_once, protected_plus_resolve_once);
                    if (g_protected_plus_query == NULL) {
                        autofarm_verified = 1;
                        evasion_armed = 0;
                    } else {
                        int plus_active = g_protected_plus_query();

                        autofarm_verified = 1;
                        evasion_armed = 0;
                        if (plus_active == 1 && (uint32_t)record[1] == 2)
                            evasion_armed = (uint32_t)(record[0] >> 32) == 1;
                    }
                } else {
                    autofarm_verified = 0;
                    evasion_armed = 0;
                }
            }
        }
    }
    ctx->autofarm_enabled = autofarm_enabled;

    ctx->limit_0x62 = (uint32_t)limit_0x62;
    ctx->limit_99 = (uint32_t)limit_99;
    ctx->evasion_armed = evasion_armed;
    ctx->autofarm_gated = setting_0x25 == 1 ? autofarm_verified : 0;

    /* remote stage-graph walk: root at bias+0x1307e20, count at +0x50 */
    {
        uint64_t root_obj = remote_read_ptr((uint64_t)g_proc_mem_bias + 0x1307e20);
        int32_t root_count = -1;
        uint32_t child_size_field = 0;

        if (root_obj == 0
            || proc_mem_read((void *)(intptr_t)bind_target, root_obj + 0x50,
                             &root_count, 4) == 0
            || root_count < 0 || root_count > 0x40) {
            ctx->autofarm_enabled = 0;
            return;
        }

        if (root_count == 5) {
            uint64_t widget_obj = remote_read_ptr(root_obj + 0x48);
            uint64_t child_obj = remote_read_ptr(widget_obj + 0x28);

            if (child_obj == 0
                || proc_mem_read((void *)(intptr_t)bind_target,
                                 child_obj + 0xb8, &child_size_field, 4) == 0
                || (int32_t)child_size_field < 0) {
                ctx->autofarm_enabled = 0;
                return;
            }

            if (widget_obj != 0) {
                uint64_t child_size = child_size_field; /* zero-extended */
                bool changed;

                if (g_stage_widget_obj == widget_obj
                    && g_stage_child_obj == child_obj
                    && g_stage_root_obj == root_obj) {
                    /* identity unchanged: a shrunken child count still
                     * counts as a rebuild */
                    changed = child_size < g_stage_child_size;
                    g_stage_widget_obj = widget_obj;
                    g_stage_child_obj = child_obj;
                    g_stage_root_obj = root_obj;
                    g_stage_child_size = child_size;
                } else {
                    changed = true;
                }
                if (changed) {
                    if (g_stage_generation != -1)
                        g_stage_generation = g_stage_generation + 1;
                    else
                        ctx->autofarm_enabled = 0;
                    g_stage_widget_obj = widget_obj;
                    g_stage_child_obj = child_obj;
                    g_stage_root_obj = root_obj;
                    g_stage_child_size = child_size;
                }
            } else {
                g_stage_child_obj = 0;
                g_stage_widget_obj = 0;
                g_stage_child_size = 0;
            }
        } else {
            g_stage_child_obj = 0;
            g_stage_widget_obj = 0;
            g_stage_child_size = 0;
        }
        ctx->widget_obj = g_stage_widget_obj;
        ctx->generation = (uint64_t)g_stage_generation;
    }
}
/* menu_engine chain chunk 2: covers raw lines 527-950 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>

extern int __android_log_print(int prio, const char *tag, const char *fmt, ...);

/* ---- re-declarations carried from chunk 1: these resolve to chunk 1's
 * definitions when the chain is concatenated (prototypes only here; the
 * veneer bodies live in chunk 1) ---- */
extern int proc_mem_read(void *ctx, uint64_t addr, void *out, uint32_t len);
int remote_child_link_valid(uint64_t node, uint64_t parent); /* chunk 1 extern:
   misc.c @ 00154d5c; the preflight call site passes only the node and ignores
   the verdict (raw keeps the result in an unused register) */
extern int nexus_menu_ui_state(void *state_out, int tid);
extern int str_format(char *buf, size_t cap, size_t slen, const char *fmt, ...);
void nexus_menu_diagnostics(char *out, uint32_t cap); /* @ 00186858 (p4) */
extern int64_t g_proc_mem_bias;      /* 0x22d8e0 — remote-read bias */
extern uint32_t g_launcher_menu_id;  /* 0x22f038 — owning menu id / Mainloop tid */
extern int32_t g_launcher_attached;  /* 0x22f03c — 1 attached, -1 failed, 0 pending */
extern uint64_t g_launcher_stage_root; /* 0x22f178 — stashed stage root */
extern uint64_t g_launcher_stage_view; /* 0x22db60 — stashed stage view */
extern uint64_t g_launcher_view;     /* 0x22f180 — rich launcher view object */
extern uint64_t g_battle_state;      /* 0x22d880 — battle/screen state word */

/* glibc/bionic tid + thread-name helpers */
pid_t gettid(void);
int pthread_getname_np(pthread_t, char *, size_t);
int *__errno_location(void);
#define __errno() __errno_location()

/* service-table veneers (chunk 1 owns the bodies in the assembled file;
 * prototypes here make this chunk's calls explicit) */
long svc_call0_me1(void *obj, uint32_t slot);
long svc_call1_me1(void *obj, uint32_t slot, long a);
long svc_call2_me1(void *obj, uint32_t slot, long a, long b);
long svc_call3_me1(void *obj, uint32_t slot, long a, long b, long c);

/* ===== cross-file functions (chunk-2 additions) ===== */
extern int postbattle_island_verify(void);         /* misc.c @ 0014fac4 — verify the
                                                       ui-server island stub at 0x22f1e0
                                                       matches its saved copy */
extern int ui_sc_bundle_ready(void *ctx);          /* widgets.c @ 001554b0 — 'sc/ui.sc'
                                                       bundle ready probe (flags at bias
                                                       +0x12eb0e8/+0x12eb170 clear, bundle
                                                       bit +0x80, popover styles chain);
                                                       ctx is a pass-through */
extern int debug_quality_file_read(uint32_t dirfd,
                                   struct timespec *out); /* misc.c @ 00155a40 —
                                                       validate the BSDQ marker file;
                                                       missing -> ok (1), tampered -> 0;
                                                       publishes the marker mtime */

/* nexus_menu_* exports defined later in this file (parts 3/4) */
extern void nexus_menu_start(void);                 /* @ 00183b6c */
extern uint64_t nexus_menu_status(void);            /* @ 00183d70 — <0 failed, 3 busy */
extern uint64_t nexus_menu_register_backend(uint32_t kind, int *callbacks); /* @ 00183e8c */

/* ===== cross-file globals (chunk-2 additions) ===== */
extern uint32_t g_graphics_policy_active;  /* 0x22d890 — renderer.c: bit0 performance/graphics policy engaged */
extern uint32_t g_stage_log_count;         /* 0x22f4bc — renderer.c: capped-128 shared stage-log counter
                                              (menu_stage_log); co-defined for the gate assembly */
extern uint32_t g_graphics_cycle_ready;    /* 0x22dfb0 — renderer.c: 0 = cycle disabled */
extern uint32_t g_graphics_reload_pending; /* 0x22dfb4 — renderer.c: reload in flight after publish */
extern uint32_t g_graphics_policy_failed;  /* 0x22dfb8 — renderer.c: latched policy/release failure */
extern uint32_t g_graphics_inhibit;        /* 0x22dfbc — renderer.c: must be 0 for the cycle to run */
extern uint32_t g_debug_quality_base;      /* 0x22e000 — renderer.c: 1 once a quality block was published */
extern uint64_t g_debug_quality_ticks;     /* 0x22e004 — renderer.c: {tick0, tick1} published pair */
extern void    *g_loader_service;          /* 0x22e010 — widgets.c: NexusLoader service locator (set by
                                              widgets init); used to query statusJson */
extern const char *g_ui_init_stage;        /* 0x1a7c40 — widgets.c: current ui_initialize stage/reason
                                              string (written on failure paths) */

/*
 * is_plausible_ptr_me1 — pointer sanity gate used when reading remote object
 * handles (raw idiom: `(v & 7) != 0 || v < 0x1000`). @ inline
 */
static int is_plausible_ptr_me1(uint64_t v)
{
    return (v & 7) == 0 && v >= 0x1000;
}

/* ===== preflight poll ===== */

/*
 * menu_sc_preflight_poll — pre-poll the stage/thread/ui-state gates before
 * engaging graphics work. Reads the stage root qword at bias+0x1307e20 up
 * front, then requires: graphics policy active (0x22d890 == 1), launcher
 * attached >= 0, owning menu tid, thread name "Mainloop", island stub intact,
 * plausible stage root with root count at +0x50 in 0..0x40 but NOT 5 (raw
 * rejects 5 here), ui-state match with flag byte +5 clear, module/stage
 * identity (qword at bias+0x12eb9f0 == stashed stage root, its +0x90 ==
 * stashed stage view), and — when the policy bit is clear — the launcher view
 * still parented. Then reads the stage byte at stage root + 0x19c and returns
 * ui_sc_bundle_ready()'s verdict when that byte is 0. Returns 0 on any gate
 * failure or when polled from the wrong thread. @ 00150378
 */
int menu_sc_preflight_poll(void *read_ctx)
{
    uint64_t stage_root = 0;
    int read_ok = proc_mem_read(read_ctx,
                                (uint64_t)g_proc_mem_bias + 0x1307e20,
                                &stage_root, 8);
    int owning_tid = (int)g_launcher_menu_id;

    if (g_graphics_policy_active == 1) {
        if (g_launcher_attached < 0)
            return 0;
    } else if (g_launcher_attached <= 0) {
        return 0;
    }

    if (g_launcher_menu_id < 1 || gettid() != owning_tid)
        return 0;

    {
        char thread_name[0x10];

        memset(thread_name, 0, sizeof thread_name);
        if (pthread_getname_np(pthread_self(), thread_name,
                               sizeof thread_name) != 0)
            return 0;
        if (memcmp(thread_name, "Mainloop", 8) != 0 /* 0x706f6f6c6e69614d */
            || thread_name[8] != '\0')
            return 0;
    }

    if (postbattle_island_verify() == 0)
        return 0;
    if (read_ok == 0 || !is_plausible_ptr_me1(stage_root))
        return 0;

    {
        int32_t root_count = -1;

        if (proc_mem_read(NULL, stage_root + 0x50, &root_count, 4) == 0
            || root_count < 0 || root_count > 0x40 || root_count == 5)
            return 0;
    }

    {
        uint8_t ui_state[6];

        memset(ui_state, 0, sizeof ui_state);
        if (nexus_menu_ui_state(ui_state, owning_tid) != 1 || ui_state[5] != 0)
            return 0;
    }

    {
        uint64_t module_stage_root = 0;

        if (proc_mem_read(NULL, (uint64_t)g_proc_mem_bias + 0x12eb9f0,
                          &module_stage_root, 8) == 0)
            module_stage_root = 0;
        if (!is_plausible_ptr_me1(module_stage_root))
            module_stage_root = 0;
        if (module_stage_root != g_launcher_stage_root)
            return 0;

        {
            uint64_t stage_view = 0;

            if (proc_mem_read(NULL, module_stage_root + 0x90, &stage_view, 8) == 0)
                stage_view = 0;
            if (!is_plausible_ptr_me1(stage_view))
                stage_view = 0;
            if (stage_view != g_launcher_stage_view)
                return 0;

            if ((g_graphics_policy_active & 1) == 0
                && (remote_child_link_valid(g_launcher_view,
                                            g_launcher_stage_view), 0) == 0)
                return 0;

            {
                uint8_t stage_byte = 1;

                if (proc_mem_read(NULL, g_launcher_stage_root + 0x19c,
                                  &stage_byte, 1) != 0
                    && stage_byte == 0)
                    return ui_sc_bundle_ready(NULL);
            }
        }
    }
    return 0;
}

/* ===== Mainloop liveness ===== */

/*
 * menu_mainloop_alive_check — true when the game's "Mainloop" thread is alive
 * and the NexusLoader reports the linking phase. Scans /proc/self/task
 * (comm files, 0x3f-byte reads, EINTR retried, fstatat fallback for vanished
 * tids); a comm of exactly "Mainloop" (or "Mainloop\n") latches the reason
 * string "mainloop_live" and switches to the loader probe. The probe acquires
 * the loader service session (flags 0x10006), begins a txn (mode 8), finds
 * class "nexus/loader/NexusLoader", calls its "statusJson" "()Ljava/lang/String;"
 * and requires the JSON to start with
 * '{"schema":1,"phase":"linking","game_jni_boundary_observed":true,"synchronous_link_barrier_complete":false,'
 * (0x6a bytes) and contain
 * '"process_restart_required":false,"error":'. On any failure the shared
 * ui_initialize stage string (0x1a7c40) receives the failing reason token
 * ("task_directory_open"/"task_directory_fd"/"task_directory_read"/
 * "task_comm_path"/"task_comm_open"/"task_comm_read"). errno is preserved.
 * @ 00152b7c
 */
int menu_mainloop_alive_check(void)
{
    DIR *dir;
    int dfd;
    struct stat st;
    char comm_path[0x140];
    const char *reason = NULL;
    bool alive = false;
    int saved_errno = errno;

    dir = opendir("/proc/self/task");
    if (dir == NULL) {
        reason = "task_directory_open";
        goto fail;
    }
    dfd = dirfd(dir);
    if (dfd < 0) {
        reason = "task_directory_fd";
        closedir(dir);
        goto fail;
    }

    for (;;) {
        struct dirent *de = readdir(dir);
        char comm[0x40];
        ssize_t n;
        int fd;

        if (de == NULL) {
            /* scan exhausted: alive only if a Mainloop comm matched */
            reason = *(__errno()) == 0 ? "" : "task_directory_read";
            goto done;
        }
        if (de->d_name[0] == '.') {
            /* "." / "..": skipped, rescan */
            continue;
        }
        if (str_format(comm_path, 0x140, 0x140,
                       "/proc/self/task/%s/comm", de->d_name) >= 0x140) {
            reason = "task_comm_path";
            goto done;
        }
        do {
            fd = open(comm_path, 0x80000 /* O_RDONLY | O_CLOEXEC */);
            if (fd >= 0)
                break;
        } while (*(__errno()) == 4 /* EINTR */);
        if (fd < 0 && (*(__errno()) & 0xfffffffe) == 2 /* ENOENT/ENOTDIR */) {
            do {
                if (fstatat(dfd, de->d_name, &st, 0x100 /* AT_SYMLINK_NOFOLLOW */)
                    >= 0)
                    break;
            } while (*(__errno()) == 4);
            if (*(__errno()) == 2) {
                /* tid vanished mid-scan: skip, rescan */
                continue;
            }
        }
        if (fd < 0) {
            reason = "task_comm_open";
            goto done;
        }
        do {
            n = read(fd, comm, 0x3f);
            if (n >= 0)
                break;
        } while (*(__errno()) == 4);
        if (n < 0) {
            close(fd);
            if ((*(__errno()) & 0xfffffffe) == 2) {
                do {
                    if (fstatat(dfd, de->d_name, &st,
                                0x100 /* AT_SYMLINK_NOFOLLOW */) >= 0)
                        break;
                } while (*(__errno()) == 4);
                if (*(__errno()) != 2) {
                    reason = "task_comm_read";
                    goto done;
                }
                continue; /* tid vanished mid-read: skip, rescan */
            }
            reason = "task_comm_read";
            goto done;
        }
        close(fd);
        if (n == 0) {
            /* empty comm: skip, rescan */
            continue;
        }
        if (memcmp(comm, "Mainloop", 8) == 0 /* 0x706f6f6c6e69614d */) {
            if (comm[8] == '\n' /* 10 */) {
                /* "Mainloop\n": candidate, verify below */
                reason = "mainloop_live";
                goto loader_probe;
            }
            if (comm[8] == '\0') {
                /* "Mainloop\0": candidate, verify below */
                reason = "mainloop_live";
                goto loader_probe;
            }
            /* "Mainloop" + suffix: not the thread, rescan */
            continue;
        }
        /* other thread: rescan */
    }

loader_probe:
    /* reached only with reason == "mainloop_live": ask the loader for its
     * linking-phase statusJson */
    alive = false;
    if (g_loader_service != NULL) {
        void *session = NULL;

        if ((int)svc_call2_me1(g_loader_service, 0x30, (long)&session,
                               0x10006) == 0
            && session != NULL
            && svc_call0_me1(session, 0x720) == 0) {
            if ((int)svc_call1_me1(session, 0x98, 8) == 0) {
                long class_h = svc_call1_me1(session, 0x30,
                                             (long)"nexus/loader/NexusLoader");
                long method_h = 0;
                long str_h = 0;
                char *json = NULL;

                if (class_h != 0 && svc_call0_me1(session, 0x720) == 0
                    && (method_h = svc_call3_me1(session, 0x388, class_h,
                                                  (long)"statusJson",
                                                  (long)"()Ljava/lang/String;")) != 0
                    && svc_call0_me1(session, 0x720) == 0
                    && (str_h = svc_call3_me1(session, 0x390, class_h,
                                               method_h, 0)) != 0
                    && svc_call0_me1(session, 0x720) == 0
                    && (json = (char *)svc_call2_me1(session, 0x548, str_h,
                                                     0)) != NULL) {
                    if (strncmp(json,
                                "{\"schema\":1,\"phase\":\"linking\",\"game_jni_boundary_observed\":true,\"synchronous_link_barrier_complete\":false,",
                                0x6a) == 0
                        && strstr(json,
                                  "\"process_restart_required\":false,\"error\":")
                               != NULL)
                        alive = true;
                    svc_call2_me1(session, 0x550, str_h, (long)json);
                }
                if (svc_call0_me1(session, 0x720) != 0)
                    svc_call0_me1(session, 0x88);
                svc_call1_me1(session, 0xa0, 0);
            } else {
                alive = false;
            }
            if (svc_call0_me1(session, 0x720) != 0)
                svc_call0_me1(session, 0x88);
        }
    }
    goto done;

done:
    closedir(dir);
    errno = saved_errno;
    if (alive)
        return 1;

fail:
    g_ui_init_stage = reason;
    return 0;
}

/* ===== status / diagnostics ===== */

/*
 * menu_status_failed — true when nexus_menu_status() is negative (failed)
 * or exactly 3 (busy). @ 00155e08
 */
int menu_status_failed(void)
{
    int64_t status = nexus_menu_status();

    return status < 0 || status == 3;
}

/*
 * menu_diagnostics_push — render the 0x800-byte diagnostics snapshot via
 * nexus_menu_diagnostics and push it into the sink object's method slot
 * +0x538. The sink table is read before the call (Ghidra shows the raw
 * param as long*). @ 00155e28
 */
void menu_diagnostics_push(void *sink)
{
    char text[0x800];

    nexus_menu_diagnostics(text, 0x800);
    svc_call2_me1(sink, 0x538, (long)text, 0); /* slot arg count approximated */
}

/* menu_engine.c-owned diagnostics writer defined in part 4 (p4 range) */

/* ===== performance settings ===== */

uint32_t g_perf_mode_latched;  /* 0x22f4b8 — 1 while the performance mode is applied;
                                   written here and cleared by the frame tick / misc
                                   perf-save path; renderer+widgets read it ("RELOADING
                                   TEXTURES") */

/*
 * menu_performance_load — load, validate and apply performance.bin from the
 * given directory fd. Gates: dirfd >= 0; the file must be a regular file
 * (S_IFREG), owned by the current uid, nlink == 1, mode 0600 (raw: mode &
 * 0x3f == 0), exactly 0x14 bytes, magic "NXP69252" (qword 0x323532393650584e)
 * with version 1 and mode < 2; the CRC-32 (reflected 0xedb88320/0x76dc4190,
 * NEON-folded in the dump) over the first 0x10 bytes must match ~stored_crc.
 * Applies the mode (mode value published to the battle-state word 0x22d880
 * low half) and the latched "optimization on" flag 0x22f4b8, then reads the
 * debug-quality marker (published to 0x22dfb0/0x22e000/0x22e004). Always
 * logs one 'performance_settings' stage line (reason
 * 'invalid_or_unreadable_default_off' unless verified, else
 * 'optimization_off'/'optimization_on'). errno is preserved.
 * @ 001560d8
 */
void menu_performance_load(int dirfd)
{
    struct stat st;
    int fd;
    uint32_t rec[5]; /* magic+ver qword, mode, crc, pad — 0x14 bytes */
    uint64_t got = 0;
    bool verified = false;
    uint32_t mode_value = 0;
    int saved_errno;

    if (dirfd < 0)
        goto out;

    fd = openat(dirfd, "performance.bin", 0x88000 /* O_RDONLY|O_CLOEXEC */);
    if (fd < 0) {
        verified = *(__errno()) == 2; /* ENOENT counts as default-off */
        goto out;
    }
    if (fstat(fd, &st) != 0
        || (st.st_mode & 0xf000) != 0x8000
        || st.st_uid != getuid()
        || st.st_nlink != 1
        || (st.st_mode & 0x3f) != 0
        || st.st_size != 0x14) {
        close(fd);
        goto out;
    }

    while (got < 0x14) {
        ssize_t n = read(fd, (char *)rec + got, 0x14 - got);

        if (n < 0 && *(__errno()) == 4) /* EINTR */
            continue;
        if (n <= 0)
            break;
        got += (uint64_t)n;
    }
    close(fd);
    if (got == 0x14
        && rec[0] == 0x32353239 /* "NXP6" */
        && rec[1] == 0x3652584e /* "9252" */
        && (int)rec[2] == 1     /* version */
        && rec[3] < 2) {        /* mode */
        /* CRC-32 (reflected, poly 0xedb88320) over the first 0x10 bytes,
         * compared against the stored complement */
        uint32_t crc = 0xffffffff;
        const uint8_t *p = (const uint8_t *)rec;
        int i;

        for (i = 0; i < 0x10; i++) {
            int bit;

            crc ^= p[i];
            for (bit = 0; bit < 8; bit++)
                crc = (crc >> 1) ^ (0xedb88320u & (uint32_t)(-(int32_t)(crc & 1)));
        }
        verified = rec[4] == ~crc; /* stored complement check */
        mode_value = verified ? rec[3] : 0;
    }

out:
    saved_errno = errno;
    {
        uint32_t state_lo = 0; /* low dword of the 0x22d880 state word */

        if (verified)
            state_lo = mode_value;
        g_battle_state = (g_battle_state & 0xffffffff00000000u) | state_lo;
        g_perf_mode_latched = verified ? mode_value : 0; /* 0x22f4b8 */
    }
    {
        struct timespec ts;
        int marker_ok;

        memset(&ts, 0, sizeof ts);
        marker_ok = debug_quality_file_read((uint32_t)dirfd, &ts);
        g_graphics_cycle_ready = marker_ok;             /* 0x22dfb0 */
        g_debug_quality_base = marker_ok ? (uint32_t)ts.tv_sec : 0; /* 0x22e000 */
        g_debug_quality_ticks = marker_ok
            ? (uint64_t)ts.tv_sec * 1000 + (uint64_t)ts.tv_nsec / 1000000
            : 0;                                        /* 0x22e004 */
        if (marker_ok && ts.tv_sec != 0)
            g_perf_mode_latched = 1;
    }
    errno = saved_errno;

    /* one capped stage log per invocation */
    {
        const char *reason = "invalid_or_unreadable_default_off";
        const char *opt = "optimization_off";

        if (mode_value != 0)
            opt = "optimization_on";
        if (verified)
            reason = opt;

        if (g_stage_log_count < 0x80) {
            struct timespec now;
            int64_t ms = 0;

            g_stage_log_count = g_stage_log_count + 1;
            if (clock_gettime(CLOCK_REALTIME, &now) == 0)
                ms = (int64_t)now.tv_sec * 1000 + now.tv_nsec / 1000000;
            __android_log_print(4, "NexusLab69252",
                                "{\"stage\":\"%s\",\"reason\":\"%s\",\"action\":%u,\"pid\":%d,\"tid\":%d,\"time_ms\":%lld}",
                                "performance_settings", reason, 0,
                                (int)getpid(), (int)gettid(), (long long)ms);
        } else {
            g_stage_log_count = g_stage_log_count + 1;
        }
    }
}
/* menu_engine chain chunk 3: covers raw lines 951-1350 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <sys/stat.h>

extern int __android_log_print(int prio, const char *tag, const char *fmt, ...);

/* ---- carried declarations (chunk 1/2 own the bodies) ---- */
extern int str_format(char *buf, size_t cap, size_t slen, const char *fmt, ...);
extern void nexus_menu_start(void);                 /* @ 00183b6c */
extern uint64_t nexus_menu_status(void);            /* @ 00183d70 */
extern uint64_t nexus_menu_register_backend(uint32_t kind, int *callbacks); /* @ 00183e8c */
extern int binding_state_name(int state);          /* misc.c @ 00169ab8 — state index
                                                       to name string
                                                       ("binding_state_invalid" fallback) */
extern int memfd_module_callback(void *info, size_t size, void *data); /* misc.c
                                                       @ 00156d68 — dl_iterate_phdr
                                                       callback matching a
                                                       "/memfd:nexus-* (deleted)"
                                                       module by soname */
extern int mapped_file_hash_check(void *rec, const void *expect,
                                  size_t len);   /* misc.c @ 00156f1c — fstat +
                                                  SHA-256 a sealed memfd of the
                                                  expected size */
extern int memfd_path_resolve(const char *name, char *out); /* misc.c @ 001574ec */
extern int memfd_module_open(const char *name, int a, int b); /* misc.c @ 00157740 */
extern uint32_t g_stage_log_count;   /* 0x22f4bc — capped-128 stage-log counter */
extern const char g_empty_text[];    /* 0x134f22 — shared "" rodata */

/* dlfcn (Android dlfcn.h provides dladdr/dlerror via <dlfcn.h> included above) */
#define _GNU_SOURCE_DUMMY_ME3 1
#ifndef _DLFCN_H_DECLARED_ME3
#define DLINFO_ME3_DECLARED 1
typedef struct {
    const char *dli_fname;
    void *dli_fbase;
    const void *dli_saddr;
    const char *dli_sname;
} dl_info_me3_t;
extern int dladdr_me3(const void *addr, dl_info_me3_t *info);
#define Dl_info dl_info_me3_t
#define dladdr dladdr_me3
#endif

/* dl_iterate_phdr (link.h) — declared here to keep the include set minimal */
struct dl_phdr_info;
int dl_iterate_phdr(int (*cb)(struct dl_phdr_info *, size_t, void *), void *data);

/* 0x1020-byte module-discovery record seeded from rodata templates
 * (0x198c78 = libNexusEvasion69252.so, 0x199c98 = libNexusEvasionRuntime69252.so)
 * and filled by the phdr callback. */
typedef struct {
    uint32_t matches;          /* +0x0000 — soname match count (capped at 2) */
    uint32_t _pad0;
    uint64_t dlbase;           /* +0x0008 — dladdr base of the matching module */
    char     soname[0xfe9];    /* +0x0010 — NUL-terminated resident path */
    char     verified;         /* +0x1010 — module verified flag */
    char     _pad1[7];
    uint64_t record_cookie;    /* +0x1018 — saved cookie (module identity) */
} memfd_module_record_t;       /* 0x1020 */

/* rodata templates the records are seeded from (plain memcpy sources). */
extern const memfd_module_record_t g_evasion_tpl_main;     /* 0x198c78 — template for
                                                              "libNexusEvasion69252.so" */
extern const memfd_module_record_t g_evasion_tpl_runtime;  /* 0x199c98 — template for
                                                              "libNexusEvasionRuntime69252.so" */
extern const uint8_t g_evasion_hash_main[0x1d8f0];         /* 0x110010 — expected hash
                                                              blob for the main module */
extern const uint8_t g_evasion_hash_runtime[0xc46e0];      /* 0x110030 — expected hash
                                                              blob for the runtime module */

/*
 * menu_evasion_backend_load — locate, verify and load the evasion backend
 * provider. Seeds two 0x1020-byte module records from the rodata templates and
 * walks the phdr list with the memfd callback (soname match, count cap 2, both
 * must be found exactly once). Returns 0xffffffff when either record is
 * ambiguous or missing, when the discovered paths/flags/bases are empty, or
 * when mapped_file_hash_check rejects either module (hash blobs 0x1d8f0 /
 * 0xc46e0 bytes at 0x110010/0x110030). Otherwise resolves the memfd path,
 * opens the sealed module (memfd_module_open), dlopens it (RTLD_LAZY|
 * RTLD_LOCAL = 6) — with a fallback through the "/memfd:nexus-* (deleted)"
 * readlink path when dlopen fails — and requires BOTH exports
 * "nexus_evasion_menu_backend_v1" and "nexus_evasion_bind_image_v1" to resolve
 * inside the module whose dladdr base matches the record's base (dlclose +
 * 0xffffffff on mismatch). On success fills out[] = { magic (0x10f7d8),
 * dlopen handle, module base, backend fn, bind_image fn } and returns 1.
 * Failures in the fallback path log 'resident lookup failed descriptor=%s
 * soname=%s error=%s' (tag NexusMem, prio 6). Returns 0 when the memfd open
 * itself fails. @ 00156850
 */
int menu_evasion_backend_load(void *unused, void **out)
{
    memfd_module_record_t rec_main;
    memfd_module_record_t rec_runtime;
    char descriptor[54];
    char resident[0x100];
    void *handle;
    long backend_fn;
    long bind_image_fn;

    (void)unused;

    memcpy(&rec_main, &g_evasion_tpl_main, sizeof rec_main);
    memcpy(&rec_runtime, &g_evasion_tpl_runtime, sizeof rec_runtime);
    dl_iterate_phdr((int (*)(struct dl_phdr_info *, size_t, void *))memfd_module_callback,
                    &rec_main);
    dl_iterate_phdr((int (*)(struct dl_phdr_info *, size_t, void *))memfd_module_callback,
                    &rec_runtime);

    if (rec_main.matches > 1 || rec_runtime.matches > 1)
        return -1; /* 0xffffffff: ambiguous soname match */
    if (rec_main.matches == 0)
        return 0; /* main module not loaded */
    if (rec_runtime.matches == 0)
        return -1;
    if (rec_main.soname[0] == '\0')
        return -1;
    if (rec_runtime.verified == '\0' || rec_main.dlbase == 0
        || rec_runtime.record_cookie == 0)
        return -1;
    if (mapped_file_hash_check(&rec_main, g_evasion_hash_main, 0x1d8f0) == 0)
        return -1;
    if (mapped_file_hash_check(&rec_runtime, g_evasion_hash_runtime, 0xc46e0) == 0)
        return -1;

    if (memfd_path_resolve(rec_main.soname, descriptor) == 0
        && memfd_module_open(descriptor, 0, 0) >= 0) {
        ssize_t n;

        /* resident path buffer: raw reads the link into a 0x140 Dl_info-sized
         * area then shifts the " (deleted)"-stripped name to a 0x100 slot;
         * dlopen uses slot+1 (leading char skipped, raw &local_2e0 + 1) */
        memset(resident, 0, sizeof resident);
        n = readlink(descriptor, resident, 0x13f);
        if (n < 0 || n - 0x13f > -0x13f /* readlink range check */
            || n <= 0x17)
            goto fallback;
        resident[n] = '\0';
        if (memcmp(resident, "/memfd:nexus-", 13) != 0 /* 0x6e3a64666d656d2f */
            || memcmp(resident + 13, "dnexus-su", 9) != 0 /* 0x2d737578656e3a64 */
            || strcmp(resident + n - 23, " (deleted)") != 0
            || (size_t)(n - 0x17) >= 0x100)
            goto fallback;
        memmove(resident, resident + 23, (size_t)(n - 0x17));
        resident[n - 0x17] = '\0';
        if (strlen(resident) > 0x41) {
            handle = dlopen(resident + 1, 6 /* RTLD_LAZY|RTLD_LOCAL */);
            if (handle != NULL)
                goto loaded;
        }

    fallback:
        {
            const char *dl_err = dlerror();
            size_t len = strlen(resident);
            const void *soname = len >= 0x42 ? (const void *)resident
                                             : (const void *)g_empty_text;
            const char *err_text = dl_err != NULL ? dl_err : "unknown";

            __android_log_print(6, "NexusMem",
                                "resident lookup failed descriptor=%s soname=%s error=%s",
                                descriptor, soname, err_text);
        }
        return 0;
    }

loaded:
    {
        void *loaded_handle = handle;

        backend_fn = (long)dlsym(loaded_handle, "nexus_evasion_menu_backend_v1");
        bind_image_fn = (long)dlsym(loaded_handle, "nexus_evasion_bind_image_v1");
        if (backend_fn == 0 || bind_image_fn == 0) {
            dlclose(loaded_handle);
            return -1;
        }
        {
            Dl_info info;

            if (dladdr((void *)backend_fn, &info) == 0
                || (uint64_t)info.dli_fbase != rec_main.dlbase
                || dladdr((void *)bind_image_fn, &info) == 0
                || (uint64_t)info.dli_fbase != rec_main.dlbase) {
                dlclose(loaded_handle);
                return -1;
            }
        }
        out[0] = (void *)(uintptr_t)0x10f7d8; /* magic cookie (raw 0x10f7d8) */
        out[1] = loaded_handle;
        out[2] = (void *)rec_main.dlbase;
        out[3] = (void *)backend_fn;
        out[4] = (void *)bind_image_fn;
    }
    return 1;
}
/* menu_engine chain chunk 4: covers raw lines 1419-1900 */

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
#include <fcntl.h>
#include <sys/stat.h>

extern int __android_log_print(int prio, const char *tag, const char *fmt, ...);
pid_t gettid(void);
int pthread_getname_np(pthread_t, char *, size_t);

/* ---- carried declarations (earlier chunks own the definitions) ---- */
extern int proc_mem_read(void *ctx, uint64_t addr, void *out, uint32_t len);
extern uint64_t remote_read_ptr(uint64_t addr);
extern int remote_child_link_valid(uint64_t node, uint64_t parent);
extern int nexus_menu_ui_state(void *state_out, int tid);
extern int str_format(char *buf, size_t cap, size_t slen, const char *fmt, ...);
extern uint64_t nexus_menu_status(void);
extern int menu_sc_preflight_poll(void *read_ctx);
extern int ui_sc_bundle_ready(void *ctx);
extern int ui_latch_test_and_set(int which, void *addr); /* menu_engine p4 @ 00193f80 —
                                                            atomic byte test-and-set */
extern long svc_call0_me1(void *obj, uint32_t slot);
extern long svc_call1_me1(void *obj, uint32_t slot, long a);
extern long svc_call2_me1(void *obj, uint32_t slot, long a, long b);
extern long svc_call3_me1(void *obj, uint32_t slot, long a, long b, long c);
extern long svc_call4_me1(void *obj, uint32_t slot, long a, long b, long c, long d);
extern int64_t g_proc_mem_bias;      /* 0x22d8e0 — remote-read bias */
extern uint32_t g_launcher_menu_id;  /* 0x22f038 — owning menu id / Mainloop tid */
extern int32_t g_launcher_attached;  /* 0x22f03c — 1 attached, -1 failed, 0 pending */
extern uint64_t g_launcher_stage_root; /* 0x22f178 — stashed stage root */
extern uint64_t g_launcher_stage_view; /* 0x22db60 — stashed stage view */
extern uint64_t g_launcher_view;     /* 0x22f180 — rich launcher view object */
extern uint64_t g_battle_state;      /* 0x22d880 — battle/screen state word */
extern uint32_t g_reload_requested;  /* 0x22d88c — renderer.c: UI reload requested */
extern uint32_t g_graphics_policy_active; /* 0x22d890 — renderer.c: policy engaged */
extern uint32_t g_reload_request_arg;     /* 0x22d894 — renderer.c: reload arg */
extern uint32_t g_reload_phase;      /* 0x22d898 — renderer.c: reload walk phase */
extern uint64_t g_reload_cookie;     /* 0x22d8a0 — renderer.c: {hi=1, lo=interval} */
extern uint64_t g_reload_deadline_ms; /* 0x22d8a8 — renderer.c: request monotonic ms */
extern uint32_t g_graphics_policy_failed; /* 0x22dfb8 — renderer.c: latched failure */
extern uint32_t g_graphics_inhibit;  /* 0x22dfbc — renderer.c: cycle inhibit */
extern uint32_t g_ui_release_done;   /* 0x22db70 — renderer.c: frame freed flag */
extern uint32_t g_stage_log_count;   /* 0x22f4bc — capped-128 stage-log counter */
extern uint32_t g_perf_mode_latched; /* 0x22f4b8 — perf-mode latch (chunk 2) */
extern uint32_t g_launcher_reset_a;  /* 0x25ca20 — renderer.c: asset probe start ms */
extern uint32_t g_launcher_reset_b;  /* 0x25ca28 — renderer.c: asset probe next ms */
extern uint64_t g_graphics_policy_ms; /* 0x281a20 — renderer.c: policy applied at */
extern int32_t g_graphics_policy_pair[2]; /* 0x281a28 — renderer.c: source pair */
extern uint32_t g_lowres_atlas_pair; /* 0x25cb38 — lowres atlas source pair value
                                        (multi-part; also referenced by p2) */
extern void *g_launcher_renderer_ops; /* 0x25ca38 — renderer.c: cached renderer ops
                                         v1 table (slots +0x10 metrics, +0x18 ts) */
extern const char *g_launcher_status; /* 0x1a7c58 — renderer.c: status reason
                                          ("launcher initializing") */
extern int64_t g_host_session;       /* 0x1a7c50 — misc.c: host session dir fd (>= 0) */
extern uint8_t g_bind_state[];       /* 0x22f040 — misc.c bind state (wake +4) */
extern int64_t bind_state_value_read(void *state);
extern int bind_request_handle(void *state, void *rec, uint64_t now_ms, int arg);
extern int remote_widget_ancestor_of(uint64_t frame, uint64_t target);
extern uint64_t protected_channel_ready(void);

/* ---- menu_engine part-2 definitions (forward; raw 2525+) ---- */
extern void menu_shell_stop_log(const char *reason);        /* @ 001607dc */
extern int menu_launcher_style_apply(int screen, void *viewport); /* @ 00160c3c */
extern int menu_action_dispatch(uint32_t action_id);        /* @ 001613ac */
extern int menu_value_set_gated(int id, uint32_t value, int tid); /* @ 00161538 */
extern int menu_perf_mode_toggle(uint64_t now_ms, int tid); /* @ 00161acc */
extern int menu_perf_mode_toggle_screen(uint64_t now_ms, int tid); /* @ 00161b84 */
extern int menu_battle_state_sync(int tid);                 /* @ 00161c58 */
extern void chooser_page_build(void *rec, int tid);         /* @ 0016291c */

/* ---- menu_engine part-3/4 definitions (forward) ---- */
extern void nexus_menu_import(void *buf, uint64_t len, int tid); /* @ 001843dc */
extern uint64_t nexus_menu_scroll_battle(int dir, int tid);     /* @ 00184900 */
extern uint64_t nexus_menu_server_open(int tid);                /* @ 00185eac */
extern uint32_t nexus_menu_server_action(uint32_t op, int tid); /* @ 00185f90 */
extern uint64_t nexus_menu_theme_open(int tid);                 /* @ 0018640c */
extern uint64_t nexus_menu_theme_action(uint32_t op, int tid);  /* @ 00186548 */
extern uint64_t nexus_menu_profile_open(int tid);               /* @ 00187e48 */
extern uint64_t nexus_menu_profile_action(int op, int tid);    /* @ 00187f14 */
extern void nexus_menu_profile_pump(int tid, uint64_t now_ms);  /* @ 001880e0 */
extern uint32_t nexus_menu_dispatch(uint32_t op, int a, int b, int tid); /* @ 00194730 */
extern bool menu_settings_load(void *unused, void *buf, size_t cap,
                               uint64_t *out_len);          /* @ 00156614 (chunk 5) */
extern void menu_stage_log(void *unused, const char *stage,
                           const char *reason, uint32_t action); /* @ 00156760 (chunk 5) */

/* ---- cross-file (misc.c / widgets.c / renderer.c / themes.c) ---- */
extern int command_ring_pop(int *opcode, void *value_hi, void *value); /* misc @ 001612c8 */
extern uint64_t clipboard_invoke(void);                /* misc @ 00161970 */
extern int script_port_reset(int kind);                /* misc @ 00192228 */
extern int performance_mode_save(uint32_t mode, uint64_t now_ms); /* misc @ 001645e0 */
extern uint64_t allocator_state_read(void *ops, void *view_out); /* misc @ 00150710 */
extern void allocator_state_write(void *rec, uint64_t addr, int value); /* misc @ 001556ec */
extern int maps_range_is_rw(uint64_t addr, uint64_t len); /* misc @ 00155848 */
extern int engine_global_pair_read(void *pair_out, void *viewport); /* misc @ 00167524 */
extern uint64_t integrity_master_gate(void);           /* misc @ 00167750 */
extern void frame_time_stamp_forward(uint64_t now_ms); /* misc @ 00167888 */
extern int renderer_graphics_release(void);            /* renderer @ 0016793c */
extern int allocator_cycle_check(void *ops);           /* misc @ 00167e78 */
extern int menu_view_draw(void *rec, int tid);         /* renderer @ 0016205c */
extern void menu_perf_status_row_append(void *rec);    /* widgets @ 00162118 */
extern void main_view_rows_refresh(void *rec);         /* misc @ 00162260 */
extern int plus_key_input(int armed, uint64_t now_ms); /* activation_plus @ 00162390 */
extern int ui_named_action_invoke(int action_id);      /* widgets @ 001924cc */
extern int launcher_stage_wait(uint64_t root, uint64_t view, int tid); /* renderer @ 001608e0 */
extern int nexus_rich_main_frame(uint64_t now_ms, uint32_t frame_not3,
                                  int menu_open);      /* renderer @ 00178c78 */
extern int nexus_ui_graphics_resources(uint64_t root, uint64_t view,
                                       uint64_t launcher, uint32_t arg); /* renderer @ 00177afc */
extern int nexus_menu_theme_pump(int tid, uint64_t now_ms); /* themes @ 00186d58 */
extern int theme_menu_status_set(int code, int tid, const char *reason); /* misc @ 00183cdc */
extern void battle_end_state_tick(uint32_t kind, void *frame_obj, void *arg,
                                  uint64_t now_ms);    /* widgets @ 0015f334 */
extern int binding_state_name(int state);              /* misc @ 00169ab8 */
/* named imports with no body in the dump (resolved from the companion mods) */
extern uint64_t nexus_menu_debug_open(int tid);        /* external @ 001842d0 — not in dump */
extern uint64_t nexus_menu_debug_action(uint32_t op, int tid); /* external — not in dump */
extern void nexus_menu_debug_pump(int tid, uint64_t now_ms);   /* external — not in dump */
extern uint64_t nexus_menu_editor_open(int tid);       /* external — not in dump */
extern uint64_t nexus_menu_editor_action(uint32_t op, int tid); /* external — not in dump */
extern void nexus_menu_editor_pump(int tid, uint64_t now_ms);  /* external — not in dump */
extern int nexus_script_port_fonts_open(void);         /* external — not in dump */
extern int nexus_script_port_fps_open(void);           /* renderer @ 0016e618 (thunk 00194690) */

/* chunk-local helpers (defined below / in chunk 5) */
static float int_as_float_me4(uint32_t bits);
static int dispatch_command_me4(int opcode, int tid, uint64_t now_ms);
static void menu_frame_attach_path(long frame_obj, uint64_t now_ms, int tid,
                                   uint32_t open_state);
static int frame_perf_graphics_restore(uint64_t now_ms, int tid);
static int frame_game_reload_verify(uint64_t now_ms, int tid,
                                    const char *thread_name);
static void frame_battle_end_pump(uint64_t now_ms, int tid,
                                  const char *thread_name);

/* ===== frame-tick globals owned by this file ===== */

uint32_t g_ui_release_armed;    /* 0x22db78 — nonzero once frame-v1 release is armed
                                   (widgets sets it; renderer + this tick gate on it) */
uint64_t g_perf_restore_ms;     /* 0x25cb30 — saved-mode restore start (ms; misc clears) */
static uint32_t g_frame_tick_latch; /* 0x25ca14 — re-entrancy latch (test-and-set) */
static uint64_t g_open_wait_next_ms; /* 0x25ca18 — next allowed open attempt (ms) */
static uint32_t g_asset_probe_count; /* 0x25ca30 — 'nexus_shell_waiting_assets' retries (< 4) */
static uint32_t g_settings_restored; /* 0x25ca34 — settings restore-once latch */
static uint64_t g_plus_key_last_ms; /* 0x25ca40 — opcode 0x97 rate gate (ms) */
static uint64_t g_menu_open_since_ms; /* 0x2815b0 — menu-open wait start (ms) */
static uint32_t g_menu_open_reported; /* 0x2815b8 — open-wait deadline reported */
static uint64_t g_battle_pump_last_ms; /* 0x281a30 — battle-end pump throttle (ms) */
static uint64_t g_plus_service_last_ms; /* 0x281a38 — plus service throttle (ms) */

/* raw float-bit reinterpretation (Ghidra shows raw int slots) */
static float int_as_float_me4(uint32_t bits)
{
    float f;

    memcpy(&f, &bits, sizeof f);
    return f;
}

/* ===== backend registration (UI backend contract veneers) ===== */

/*
 * menu_backend_register — forward (kind, callbacks) to
 * nexus_menu_register_backend. — dlsym export thunk @ 00156c50
 */
void menu_backend_register__export(uint32_t kind, int *callbacks)
{
    extern uint64_t nexus_menu_register_backend(uint32_t kind, int *callbacks);

    nexus_menu_register_backend(kind, callbacks);
}

/*
 * menu_backend_start — start the menu (nexus_menu_start) and report whether
 * the status word became non-zero. — dlsym export thunk @ 00156c5c
 */
bool menu_backend_start__export(void)
{
    extern void nexus_menu_start(void);

    nexus_menu_start();
    return nexus_menu_status() != 0;
}

/*
 * menu_backend_bind_log — log one 'nexus_ui_backend_binding' stage line with
 * the binding-state name for the given state id, capped by the shared
 * 0x22f4bc counter. @ 00156c7c
 */
void menu_backend_bind_log(void *unused, uint32_t bind_state)
{
    const char *state_name =
        (const char *)(intptr_t)binding_state_name((int)bind_state);

    (void)unused;
    if (g_stage_log_count < 0x80) {
        struct timespec now;
        int64_t ms = 0;

        g_stage_log_count = g_stage_log_count + 1;
        if (clock_gettime(CLOCK_REALTIME, &now) == 0)
            ms = (int64_t)now.tv_sec * 1000 + now.tv_nsec / 1000000;
        __android_log_print(4, "NexusLab69252",
                            "{\"stage\":\"%s\",\"reason\":\"%s\",\"action\":%u,\"pid\":%d,\"tid\":%d,\"time_ms\":%llu}",
                            "nexus_ui_backend_binding", state_name, 0,
                            (int)getpid(), (int)gettid(), (unsigned long long)ms);
    } else {
        g_stage_log_count = g_stage_log_count + 1;
    }
}

/* ===== main frame tick ===== */

/*
 * menu_main_frame_tick — per-frame menu tick on the game's render thread.
 * The body is gated on the frame-v1 release being armed (0x22db78) plus a
 * test-and-set re-entrancy latch (0x25ca14). Frame 0 re-registers the
 * renderer-ops guard bundle when the bind state wakes; frames 1 and 3 run
 * nexus_rich_main_frame (frame 3 rate-limits the open path to a 16ms
 * window), the 'nexus_menu_open_wait' deadline bookkeeping, then — when the
 * ui-state permits, status is non-zero and the Mainloop tid matches — the
 * attach path (see menu_frame_attach_path). Frame 3 then runs the
 * perf-graphics restore, game-reload verify and battle-end/plus pumps
 * (chunk 5) plus the theme/debug/profile/editor pumps; other frames only
 * drive battle_end_state_tick. errno is preserved; the latch is cleared on
 * the way out. @ 0015d224
 */
void menu_main_frame_tick(long frame_obj, void *param_2, uint64_t frame_kind)
{
    int saved_errno = errno;
    char thread_name[0x10];
    uint64_t now_ms = 0;
    int tid;
    int name_ok;
    uint32_t open_state;

    if ((int)g_ui_release_armed == 0)
        return;
    if ((ui_latch_test_and_set(1, &g_frame_tick_latch) & 1) != 0)
        return;

    memset(thread_name, 0, sizeof thread_name);
    if (pthread_getname_np(pthread_self(), thread_name, sizeof thread_name) != 0)
        goto out;

    tid = gettid();
    {
        struct timespec ts;

        if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0)
            now_ms = (uint64_t)ts.tv_sec * 1000 + (uint64_t)ts.tv_nsec / 1000000;
    }
    name_ok = tid > 0
              && memcmp(thread_name, "Mainloop", 8) == 0 /* 0x706f6f6c6e69614d */
              && thread_name[8] == '\0';

    /* ---- frame 0: bind/stage verify + renderer ops registration ---- */
    if (*(int32_t *)(g_bind_state + 4) /* 0x22f044 wake */ != 0
        && (uint32_t)frame_kind == 0
        && name_ok
        && (g_launcher_menu_id == 0 || (int)g_launcher_menu_id == tid)) {
        int64_t handle = bind_state_value_read(g_bind_state);
        uint64_t stage_root = 0;

        if (handle < 7
            && proc_mem_read((void *)(intptr_t)handle,
                             (uint64_t)g_proc_mem_bias + 0x12eb9f0,
                             &stage_root, 8) != 0
            && (stage_root & 7) == 0 && stage_root >= 0x1000) {
            uint64_t stage_view = remote_read_ptr(stage_root + 0x90);
            uint32_t viewport[4]; /* four floats at stage view +0x3c */
            uint8_t stage_byte = 1;

            if (stage_view != 0
                && proc_mem_read((void *)(intptr_t)handle, stage_view + 0x3c,
                                 viewport, 0x10) != 0
                && proc_mem_read((void *)(intptr_t)handle, stage_root + 0x19c,
                                 &stage_byte, 1) != 0
                && stage_byte == 0) {
                float f0 = int_as_float_me4(viewport[0]);
                float f1 = int_as_float_me4(viewport[1]);
                float f2 = int_as_float_me4(viewport[2]);
                float f3 = int_as_float_me4(viewport[3]);

                if (isfinite(f0) && isfinite(f1) && isfinite(f2) && isfinite(f3)
                    && f0 == 0.0f && f1 == 0.0f
                    && f2 >= 960.0f && f3 >= 560.0f
                    && f2 <= 8192.0f && f3 <= 8192.0f
                    && remote_read_ptr((uint64_t)frame_obj)
                           == (uint64_t)g_proc_mem_bias + 0x11abad8
                    && remote_widget_ancestor_of((uint64_t)frame_obj,
                                                 stage_view) != 0) {
                    /* renderer-ops guard bundle (6 qwords at the bind record) */
                    struct {
                        uint64_t magic;           /* 0x10f748 */
                        uint64_t remote_bias;     /* g_proc_mem_bias */
                        const char *build_hash_a; /* rodata 0x13667e */
                        const char *build_hash_b; /* rodata 0x135b30 */
                        int (*read_mem)(long, uint64_t, void *, uint64_t);
                        void *slot5;              /* raw stores 0 */
                    } guard;

                    guard.magic = 0x10f748;
                    guard.remote_bias = (uint64_t)g_proc_mem_bias;
                    guard.build_hash_a = (const char *)(uintptr_t)0x13667e;
                    guard.build_hash_b = (const char *)(uintptr_t)0x135b30;
                    guard.read_mem = (int (*)(long, uint64_t, void *,
                                              uint64_t))(void *)proc_mem_read;
                    guard.slot5 = NULL;
                    bind_request_handle((void *)g_bind_state, &guard, now_ms, 1);
                }
            }
        }
    }

    /* ---- frame dispatch: rich frame + open-wait bookkeeping ---- */
    open_state = (uint32_t)frame_kind; /* 1 for frame 1, frame id otherwise */
    if (frame_kind == 3 || frame_kind == 1) {
        bool attempt_allowed = false;
        int rich_result = 0;
        bool menu_open = false;

        if (frame_kind == 3) {
            /* open-path rate limit: one attempt per >= 16ms window */
            if (now_ms < g_open_wait_next_ms
                && g_open_wait_next_ms - now_ms < 0x3e9) {
                attempt_allowed = false;
            } else {
                g_open_wait_next_ms = now_ms + 0x10;
                attempt_allowed = true;
            }
        }

        /* menu-open verdict: attached==2, no policy/battle/reload, owning
         * tid, Mainloop name, ui-state flag byte set and screen char in the
         * 0x22002000cd001 bitmask (or 'e') */
        if (g_launcher_attached == 2
            && (g_graphics_policy_active & 1) == 0
            && ((uint32_t)(g_battle_state >> 32) & 1) == 0
            && (g_reload_requested & 1) == 0
            && g_launcher_menu_id >= 1 && tid == (int)g_launcher_menu_id
            && name_ok) {
            uint8_t ui_state[8];

            memset(ui_state, 0, sizeof ui_state);
            if (nexus_menu_ui_state(ui_state, tid) == 1
                && ui_state[4] != 0 && ui_state[5] == 0) {
                uint32_t scr = (uint32_t)ui_state[6] - 0x43;

                if (scr < 0x32 && ((1UL << (scr & 0x3f)) & 0x22002000cd001UL) != 0)
                    menu_open = true;
                else
                    menu_open = ui_state[6] == 0x65; /* 'e' */
            }
        }

        rich_result = (int)nexus_rich_main_frame(now_ms,
                                                 (uint32_t)(frame_kind != 3),
                                                 menu_open);
        if (rich_result != 0) {
            g_reload_phase = 0;
            g_reload_cookie = 0;
        }

        /* open-wait gate: frame 3, rate window active, rich frame idle */
        if (!attempt_allowed && (frame_kind != 3 || rich_result == 0)
            && frame_kind == 3) {
            if (g_launcher_menu_id < 1 || tid != (int)g_launcher_menu_id || !name_ok) {
                open_state = 3;
            } else if (g_launcher_attached < 1 || g_launcher_stage_root == 0
                       || g_launcher_stage_view == 0 || g_launcher_view == 0
                       || g_launcher_renderer_ops == NULL) {
                open_state = 3;
                g_menu_open_since_ms = 0;
                g_menu_open_reported = 0;
            } else {
                uint8_t ui_state[8];

                memset(ui_state, 0, sizeof ui_state);
                if (nexus_menu_ui_state(ui_state, tid) != 1) {
                    open_state = 3;
                } else {
                    if (ui_state[4] == 0 || g_launcher_attached == 2) {
                        g_menu_open_since_ms = 0;
                        g_menu_open_reported = 0;
                    } else {
                        if (now_ms <= g_menu_open_since_ms - 1)
                            g_menu_open_since_ms = now_ms;
                        if ((g_menu_open_reported & 1) == 0
                            && (now_ms - g_menu_open_since_ms) >> 4 > 0x270) {
                            g_menu_open_reported = 1;
                            menu_stage_log(NULL, "nexus_menu_open_wait",
                                           "pending_open_deadline", 0);
                        }
                    }
                    open_state = ui_state[4] != 0 ? 0 : 3;
                }
            }
        }
    }

    /* ---- attach path (open_state < 2: frames 0/1, or open frame 3) ---- */
    if ((g_graphics_policy_active & 1) == 0
        && ((uint32_t)(g_battle_state >> 32) & 1) == 0
        && g_launcher_attached >= -1
        && tid > 0 && open_state < 2
        && (int64_t)nexus_menu_status() != 0
        && name_ok
        && (g_launcher_menu_id == 0 || (int)g_launcher_menu_id == tid))
        menu_frame_attach_path(frame_obj, now_ms, tid, open_state);

    /* ---- frame-3 tail: perf/reload/battle pumps (chunk 5) + mod pumps ---- */
    if (frame_kind == 3) {
        if ((int)g_ui_release_armed == 0) {
            g_ui_release_done = 0;
        } else {
            if (frame_perf_graphics_restore(now_ms, tid) == 0
                && frame_game_reload_verify(now_ms, tid, thread_name) == 0)
                frame_battle_end_pump(now_ms, tid, thread_name);
        }
        nexus_menu_theme_pump(tid, now_ms);
        nexus_menu_debug_pump(tid, now_ms);
        nexus_menu_profile_pump(tid, now_ms);
        nexus_menu_editor_pump(tid, now_ms);
    } else {
        battle_end_state_tick((uint32_t)frame_kind, (void *)frame_obj,
                              param_2, now_ms);
    }

out:
    g_frame_tick_latch = 0; /* 0x25ca14 */
    errno = saved_errno;
}

/* ===== attach path (raw 1718-2058) ===== */

/*
 * menu_frame_attach_path — stage-verified attach sequence of the frame tick.
 * Re-reads the module stage root (bias+0x12eb9f0) and its stage view
 * (+0x90), requires the viewport quad (two 0.0 floats, then >= 960x560 and
 * <= 8192) and stage byte 0 at +0x19c. While attach is pending: asset probe
 * via the 0x12eb0e8/0x12eb170 marker bytes (4 tries, 200ms apart,
 * 'nexus_shell_waiting_assets') inside a 60s window, then
 * launcher_stage_wait ('asset_readiness_deadline' / 'launcher initializing'
 * stop reasons). Once attaching: reload interval bookkeeping (40/100ms),
 * stage identity + parentage, launcher style apply, one-shot settings
 * restore ('nexus_ui_settings_restore' restored/rejected), command-ring
 * dispatch (see dispatch_command_me4) and the frame publish: battle sync,
 * ui-state snapshot into a 0x690 record, view draw, perf status row, row
 * refresh, plus-key input, chooser page build, the renderer-ops +0x10
 * publish ('android_overlay_services_waiting' /
 * 'android_overlay_bridge_ready' / 'rich_services_stopped'), then the
 * launcher style re-apply and the attached==2 transition
 * ('nexus_menu_attached'). @ 0015d224 (attach section, raw 1718-2058)
 */
static void menu_frame_attach_path(long frame_obj, uint64_t now_ms, int tid,
                                   uint32_t open_state)
{
    uint64_t stage_root = 0;
    uint64_t handle = (uint64_t)nexus_menu_status();

    if (proc_mem_read((void *)(intptr_t)handle,
                      (uint64_t)g_proc_mem_bias + 0x12eb9f0,
                      &stage_root, 8) == 0
        || (stage_root & 7) != 0 || stage_root < 0x1000)
        return;

    {
        uint64_t stage_view = 0;

        if (proc_mem_read((void *)(intptr_t)handle, stage_root + 0x90,
                          &stage_view, 8) == 0
            || (stage_view & 7) != 0 || stage_view < 0x1000)
            return;

        {
            uint32_t viewport[4]; /* floats at stage view +0x3c */
            uint8_t stage_byte = 1;

            if (proc_mem_read((void *)(intptr_t)handle, stage_view + 0x3c,
                              viewport, 0x10) == 0
                || proc_mem_read((void *)(intptr_t)handle, stage_root + 0x19c,
                                 &stage_byte, 1) == 0
                || stage_byte != 0)
                return;

            {
                float f0 = int_as_float_me4(viewport[0]);
                float f1 = int_as_float_me4(viewport[1]);
                float f2 = int_as_float_me4(viewport[2]);
                float f3 = int_as_float_me4(viewport[3]);

                if (!(isfinite(f0) && isfinite(f1) && isfinite(f2) && isfinite(f3)
                      && f0 == 0.0f && f1 == 0.0f
                      && f2 >= 960.0f && f3 >= 560.0f
                      && f2 <= 8192.0f && f3 <= 8192.0f))
                    return;
            }

            /* ---- pending attach: asset probe ---- */
            if (g_launcher_attached == 0) {
                if (open_state == 0
                    && remote_read_ptr((uint64_t)frame_obj)
                           == (uint64_t)g_proc_mem_bias + 0x11abad8
                    && remote_widget_ancestor_of((uint64_t)frame_obj,
                                                 stage_view) != 0) {
                    if (g_launcher_reset_a == 0)
                        g_launcher_reset_a = (uint32_t)now_ms;
                    if (now_ms - g_launcher_reset_a < 0xea61) {
                        if (g_launcher_reset_b <= now_ms) {
                            g_launcher_reset_b = (uint32_t)now_ms + 200;
                            if (ui_sc_bundle_ready(NULL) != 0) {
                                int wait_rc = launcher_stage_wait(stage_root,
                                                                  stage_view, tid);

                                if (wait_rc == -1) {
                                    menu_shell_stop_log(g_launcher_status);
                                } else if (wait_rc != 0) {
                                    goto attached_block;
                                }
                            } else if (g_asset_probe_count < 4) {
                                uint8_t probe_a = 0xff;
                                uint8_t probe_b = 0xff;

                                proc_mem_read(NULL, (uint64_t)g_proc_mem_bias
                                                      + 0x12eb0e8, &probe_a, 1);
                                proc_mem_read(NULL, (uint64_t)g_proc_mem_bias
                                                      + 0x12eb170, &probe_b, 1);
                                menu_stage_log(NULL, "nexus_shell_waiting_assets",
                                               probe_a != 0xff
                                                   ? (probe_b != 0
                                                          ? "ui_prefetch_active"
                                                          : "ui_sc_exports_pending")
                                                   : "asset_probe_read_failed",
                                               0);
                                g_asset_probe_count = g_asset_probe_count + 1;
                            }
                        }
                    } else {
                        menu_shell_stop_log("asset_readiness_deadline");
                    }
                }
                return;
            }

            /* ---- attaching/attached: reload interval bookkeeping ---- */
attached_block:
            if (open_state != 1) {
                if (g_launcher_attached == 2) {
                    uint32_t interval = 0x28; /* 40ms */

                    if ((int32_t)g_battle_state != 0)
                        interval = 100;
                    if ((uint32_t)(g_reload_cookie >> 32) != 0
                        && (uint32_t)g_reload_cookie == interval
                        && g_reload_phase <= now_ms
                        && now_ms - g_reload_phase < interval)
                        return; /* inside an active reload window */
                    g_reload_cookie = ((uint64_t)1 << 32) | interval;
                    g_reload_phase = now_ms;
                }
            }

            /* ---- stage identity + launcher style ---- */
            if (stage_root == g_launcher_stage_root
                && stage_view == g_launcher_stage_view
                && remote_child_link_valid(g_launcher_view, stage_view) != 0) {
                int style_rc = menu_launcher_style_apply(
                    (int)protected_channel_ready(), viewport);

                if (style_rc == 0)
                    return;
                if (style_rc == -1) {
                    menu_shell_stop_log(g_launcher_status);
                    return;
                }

                /* one-shot settings restore */
                if ((g_settings_restored & 1) == 0) {
                    uint8_t settings_buf[0x1000];
                    uint64_t settings_len = 0;

                    g_settings_restored = 1;
                    if (menu_settings_load(NULL, settings_buf, 0x1000,
                                           &settings_len)) {
                        nexus_menu_import(settings_buf, settings_len, tid);
                        menu_stage_log(NULL, "nexus_ui_settings_restore",
                                       "restored", 0);
                    }
                }

                /* ---- command ring + dispatch ---- */
                {
                    int opcode = 0;
                    uint64_t value_qword = 0;
                    int popped = command_ring_pop(&opcode,
                                                  (char *)&value_qword + 4,
                                                  &value_qword);

                    if (open_state == 1 || popped) {
                        uint32_t action = 0;
                        int result = 0;
                        int have_result = 0;

                        if (popped) {
                            /* flag in the low dword, value in the high dword */
                            if ((uint32_t)value_qword != 0) {
                                result = menu_value_set_gated(
                                    opcode, (uint32_t)(value_qword >> 32), tid);
                                if (result != (int)0x80000000u) {
                                    action = (uint32_t)opcode;
                                    have_result = 1;
                                }
                                /* INT_MIN verdict: re-dispatch as a command */
                            }
                            if (!have_result) {
                                action = (uint32_t)opcode;
                                result = dispatch_command_me4(opcode, tid,
                                                              now_ms);
                            }
                        } else if (g_launcher_view != (uint64_t)frame_obj) {
                            /* renderer ops slot +0x18: timestamp/metrics call */
                            long *ops = (long *)g_launcher_renderer_ops;
                            long (*ts_fn)(void *, void *) =
                                (long (*)(void *, void *))ops[3];
                            uint64_t out_rec = 0;

                            if (ts_fn((void *)frame_obj, &out_rec) == 0
                                || (uint32_t)out_rec == 0)
                                goto publish;
                            action = (uint32_t)out_rec;
                            result = dispatch_command_me4((int)(uint32_t)out_rec,
                                                          tid, now_ms);
                        } else {
                            int rc = menu_action_dispatch((uint32_t)tid);
                            const char *reason =
                                1 < (uint32_t)(rc - 1) ? "blocked" : "queued";

                            g_reload_phase = 0;
                            g_reload_cookie = 0;
                            menu_stage_log(NULL, "nexus_android_overlay_open",
                                           reason, 1);
                            goto publish;
                        }

                        /* opcode 0x32 with a 1/2 verdict consults the
                         * script-port reset before reporting */
                        if (action == 0x32 && (uint32_t)(result - 1) < 2) {
                            int rc = script_port_reset(0);

                            if (rc != 1)
                                result = rc;
                        }
                        {
                            const char *reason =
                                result == 1 ? "acknowledged"
                                            : (result == 2 ? "pending" : "blocked");

                            g_reload_phase = 0;
                            g_reload_cookie = 0;
                            menu_stage_log(NULL, "nexus_menu_click", reason,
                                           action);
                        }
                    }
                }

publish:
                /* ---- frame publish ---- */
                menu_battle_state_sync(tid);
                {
                    uint8_t ui_state[8];

                    memset(ui_state, 0, sizeof ui_state);
                    if (nexus_menu_ui_state(ui_state, tid) == 1) {
                        uint8_t rec[0x690];

                        memset(rec, 0, sizeof rec);
                        memcpy(rec, ui_state, 4);
                        rec[8] = ui_state[6];
                        rec[9] = ui_state[4];
                        rec[10] = ui_state[5];
                        rec[11] = ui_state[7];
                        if (ui_state[4] == 0 /* menu closed */
                            || menu_view_draw(rec, tid) == 1) {
                            long *ops;

                            menu_perf_status_row_append(rec);
                            main_view_rows_refresh(rec);
                            {
                                uint32_t interval = 0x28;

                                if ((int32_t)g_battle_state != 0)
                                    interval = 100;
                                g_reload_cookie = ((uint64_t)1 << 32) | interval;
                                g_reload_phase = (uint32_t)now_ms;
                            }
                            plus_key_input(rec[8] == 'Y' && rec[9] != 0
                                               && rec[10] == 0,
                                           now_ms);
                            chooser_page_build(rec, tid);
                            {
                                uint8_t rec_copy[0x690];
                                long (*pub)(uint32_t, uint32_t, void *,
                                            uint64_t, uint64_t, uint64_t);
                                int rc;
                                const char *bridge_reason =
                                    "android_overlay_services_waiting";

                                memcpy(rec_copy, rec, sizeof rec_copy);
                                ops = (long *)g_launcher_renderer_ops;
                                pub = (long (*)(uint32_t, uint32_t, void *,
                                                uint64_t, uint64_t, uint64_t))
                                    ops[2]; /* slot +0x10 */
                                rc = (int)pub(viewport[2], viewport[3],
                                              rec_copy, stage_root,
                                              stage_view, now_ms);
                                if ((1 < (uint32_t)(rc - 2)) && rc != 0) {
                                    if (rc == -1) {
                                        menu_shell_stop_log(
                                            "rich_services_stopped");
                                        return;
                                    }
                                    if (rc != 4)
                                        bridge_reason =
                                            "android_overlay_bridge_ready";
                                }
                                {
                                    int style2 = menu_launcher_style_apply(
                                        (int)protected_channel_ready(), viewport);

                                    if (style2 == -1) {
                                        menu_shell_stop_log(g_launcher_status);
                                        return;
                                    }
                                    if (style2 != 0 && g_launcher_attached == 1) {
                                        g_launcher_attached = 2;
                                        theme_menu_status_set(3, tid,
                                                              bridge_reason);
                                        menu_stage_log(NULL, "nexus_menu_attached",
                                                       bridge_reason, 0);
                                    }
                                }
                            }
                        }
                    }
                }
            } else {
                menu_shell_stop_log("stage_generation_or_launcher_membership");
            }
        }
    }
}

/*
 * dispatch_command_me4 — command dispatch tail of the attach path (raw
 * 1856-1987): maps a ring opcode to its handler. 0x97 is rate-gated on
 * 0x25ca40 (never written elsewhere, so it always passes); 0x94/0x95 go to
 * the clipboard; 0x20000/0x20001 scroll the battle view; 0x20002 toggles
 * the perf mode (ring vs ops origin); 0x21028/0x240xx server, 0x2102f and
 * the 0x260xx family theme, 0x21030/0x270xx debug, 0x21031 fps, 0x21033/
 * 0x280xx profile, 0x21035/0x290xx editor, 0x21036 fonts, 0x21000-0x21036
 * named actions, and anything else nexus_menu_dispatch (with the 0x32
 * perf-save special clearing the reload request). Returns the handler
 * verdict. @ 0015d224 (dispatch section, raw 1856-1987)
 */
static int dispatch_command_me4(int opcode, int tid, uint64_t now_ms)
{
    uint64_t result = 0;

    switch (opcode) {
    case 0x97:
        /* rate gate on 0x25ca40 (unwritten elsewhere: always passes) */
        if (499 < now_ms - g_plus_key_last_ms)
            result = clipboard_invoke();
        break;
    case 0x94:
    case 0x95:
        result = clipboard_invoke();
        break;
    case 0x20000:
        result = nexus_menu_scroll_battle(-1, tid);
        break;
    case 0x20001:
        result = nexus_menu_scroll_battle(1, tid);
        break;
    case 0x21028:
        result = nexus_menu_server_open(tid);
        break;
    case 0x2102f:
        result = nexus_menu_theme_open(tid);
        break;
    case 0x21030:
        result = nexus_menu_debug_open(tid);
        break;
    case 0x21031:
        result = (uint64_t)(nexus_script_port_fps_open() != 0);
        break;
    case 0x21033:
        result = nexus_menu_profile_open(tid);
        break;
    case 0x21035:
        result = nexus_menu_editor_open(tid);
        break;
    case 0x21036:
        result = (uint64_t)(nexus_script_port_fonts_open() != 0);
        break;
    case 0x20002:
        /* origin is not distinguishable here: the ring path toggles */
        result = (uint64_t)menu_perf_mode_toggle(now_ms, tid);
        break;
    default:
        if ((uint32_t)opcode - 0x24000 < 0x30)
            result = nexus_menu_server_action((uint32_t)opcode, tid);
        else if (((uint32_t)opcode & 0xffffffc0) == 0x26000)
            result = nexus_menu_theme_action((uint32_t)opcode, tid);
        else if ((uint32_t)opcode - 0x27000 < 0x120)
            result = nexus_menu_debug_action((uint32_t)opcode, tid);
        else if ((uint32_t)opcode - 0x28000 < 6)
            result = nexus_menu_profile_action((uint32_t)opcode, tid);
        else if (((uint32_t)opcode & 0xfffffe00) == 0x29000)
            result = nexus_menu_editor_action((uint32_t)opcode, tid);
        else if ((uint32_t)opcode - 0x21000 < 0x37)
            result = (uint64_t)ui_named_action_invoke(-1);
        else {
            result = nexus_menu_dispatch((uint32_t)opcode, 0, 0, tid);
            if ((uint32_t)opcode == 0x32 && (uint32_t)(result - 1) < 2) {
                /* perf-mode save special: only outside battle/inhibit */
                if ((int32_t)g_battle_state == 0 && g_graphics_inhibit == 0
                    && g_reload_request_arg == 0)
                    return (int)result;
                g_reload_requested = 0;
                result = (uint64_t)performance_mode_save(0, now_ms);
            }
        }
        break;
    }
    return (int)result;
}
/* menu_engine chain chunk 5: covers raw lines 1901-2400 */

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
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/uio.h>

extern int __android_log_print(int prio, const char *tag, const char *fmt, ...);
pid_t gettid(void);

/* 0x38-byte shared menu context (identical to chunk 1's typedef; guarded so
 * the concatenated translation unit sees exactly one) */
int pthread_getname_np(pthread_t, char *, size_t);
ssize_t readlink(const char *path, char *buf, size_t cap);

/* ---- carried declarations (earlier chunks own the definitions) ---- */
extern int proc_mem_read(void *ctx, uint64_t addr, void *out, uint32_t len);
extern uint64_t remote_read_ptr(uint64_t addr);
extern int remote_child_link_valid(uint64_t node, uint64_t parent);
extern int nexus_menu_ui_state(void *state_out, int tid);
extern int str_format(char *buf, size_t cap, size_t slen, const char *fmt, ...);
extern int menu_sc_preflight_poll(void *read_ctx);
extern int postbattle_island_verify(void);
extern void nexus_menu_diagnostics(char *out, uint32_t cap); /* p4 def */
extern long svc_call0_me1(void *obj, uint32_t slot);
extern long svc_call1_me1(void *obj, uint32_t slot, long a);
extern long svc_call2_me1(void *obj, uint32_t slot, long a, long b);
extern long svc_call3_me1(void *obj, uint32_t slot, long a, long b, long c);
extern int ui_latch_test_and_set(int which, void *addr);
extern int64_t g_proc_mem_bias;      /* 0x22d8e0 — remote-read bias */
extern uint32_t g_launcher_menu_id;  /* 0x22f038 — owning menu id / Mainloop tid */
extern int32_t g_launcher_attached;  /* 0x22f03c — 1 attached, -1 failed, 0 pending */
extern uint64_t g_launcher_stage_root; /* 0x22f178 — stashed stage root */
extern uint64_t g_launcher_stage_view; /* 0x22db60 — stashed stage view */
extern uint64_t g_launcher_view;     /* 0x22f180 — rich launcher view object */
extern uint64_t g_battle_state;      /* 0x22d880 — battle/screen state word */
extern uint32_t g_reload_requested;  /* 0x22d88c — renderer.c: UI reload requested */
extern uint32_t g_graphics_policy_active; /* 0x22d890 — renderer.c: policy engaged */
extern uint32_t g_reload_request_arg;     /* 0x22d894 — renderer.c: reload arg */
extern uint32_t g_reload_phase;      /* 0x22d898 — renderer.c: reload walk phase */
extern uint64_t g_reload_cookie;     /* 0x22d8a0 — renderer.c: {hi=1, lo=interval} */
extern uint64_t g_reload_deadline_ms; /* 0x22d8a8 — renderer.c: request monotonic ms */
extern uint32_t g_graphics_policy_failed; /* 0x22dfb8 — renderer.c: latched failure */
extern uint32_t g_graphics_inhibit;  /* 0x22dfbc — renderer.c: cycle inhibit */
extern uint32_t g_ui_release_done;   /* 0x22db70 — renderer.c: frame freed flag */
extern uint32_t g_stage_log_count;   /* 0x22f4bc — capped-128 stage-log counter */
extern uint32_t g_perf_mode_latched; /* 0x22f4b8 — perf-mode latch (chunk 2) */
extern uint64_t g_perf_restore_ms;   /* 0x25cb30 — saved-mode restore start (chunk 4) */
extern uint32_t g_lowres_atlas_pair; /* 0x25cb38 — lowres atlas source pair value */
extern uint64_t g_graphics_policy_ms; /* 0x281a20 — renderer.c: policy applied at */
extern int32_t g_graphics_policy_pair[2]; /* 0x281a28 — renderer.c: source pair */
extern int64_t g_host_session;       /* 0x1a7c50 — misc.c: host session dir fd (>= 0) */
extern uint8_t g_bind_state[];       /* 0x22f040 — misc.c bind state */
extern pthread_once_t g_protected_plus_once; /* 0x22f1d0 — misc.c once-flag */
extern int (*g_protected_plus_query)(void);  /* 0x22f1d8 — misc.c resolved fn */
extern int64_t bind_state_value_read(void *state);
extern uint64_t protected_channel_ready(void);
extern int launcher_stage_wait(uint64_t root, uint64_t view, int tid);
extern int nexus_ui_graphics_resources(uint64_t root, uint64_t view,
                                       uint64_t launcher, uint32_t arg);
extern uint64_t allocator_state_read(void *ops, void *view_out);
extern void allocator_state_write(void *rec, uint64_t addr, int value);
extern int maps_range_is_rw(uint64_t addr, uint64_t len);
extern int engine_global_pair_read(void *pair_out, void *viewport);
extern uint64_t integrity_master_gate(void);
extern void frame_time_stamp_forward(uint64_t now_ms);
extern int renderer_graphics_release(void);
extern int allocator_cycle_check(void *ops);
extern int plus_channel_ready(void);
extern int parse_trimmed_number(const char *s, size_t len, void *out);
extern int plus_tag_record_parse(const char *s, size_t len, void *out);
extern uint64_t plus_service_request_exec(void *ops, uint32_t snap_lo,
                                          void *rec, uint64_t pair);
extern int text_contains_reload(const char *s);
extern void ui_toast_show(const char *text);
extern int64_t clock_ms(int clockid);
extern void battle_end_state_tick(uint32_t kind, void *frame_obj, void *arg,
                                  uint64_t now_ms);
extern void menu_stage_log(void *unused, const char *stage,
                           const char *reason, uint32_t action);
/* menu_context_t and menu_context_init come from chunk 1 in the assembled
 * translation unit (chunk 1 precedes this file in the cat order) */
extern int menu_launcher_style_apply(int screen, void *viewport);

/* chunk-4 throttles (0x281a30 / 0x281a38): static definitions live in
 * chunk 4 of this chain; redeclared here for standalone compilation — the
 * concatenated gate sees exactly one definition */
static uint64_t g_battle_pump_last_ms;   /* 0x281a30 (chunk 4) */
static uint64_t g_plus_service_last_ms;  /* 0x281a38 (chunk 4) */

/* misc.c-owned: mutex-guarded battle snapshot block at 0x25c958 (0x80 bytes,
 * mutex at 0x25c92c) plus the battle-end latch */
extern pthread_mutex_t g_battle_snapshot_mutex; /* 0x25c92c — misc.c */
extern uint64_t g_battle_snapshot[16];  /* 0x25c958 — misc.c writes, this pump drains */
extern uint32_t g_battle_end_latch;     /* 0x25c9d8 — misc test-and-sets, widgets clears */
extern uint32_t g_plus_request_lo;      /* 0x22db68 — misc.c: plus request handle */
extern int32_t g_plus_request_hi;       /* 0x22db6c — misc.c: plus request value */
extern uint64_t g_alloc_snap_root;      /* 0x22dfd8 — misc.c: allocator snapshot root */
extern uint32_t g_alloc_snap_b;         /* 0x22dfe0 — misc.c: allocator snapshot b */
extern float g_alloc_snap_scale;        /* 0x22dfe4 — misc.c: allocator snapshot scale */
extern uint64_t g_alloc_snap_c;         /* 0x22dfe8 — misc.c: allocator snapshot c */
extern void *g_jvm_service;             /* 0x22f4c8 — widgets.c: plus JVM service locator */
extern uint64_t g_jvm_service_ready;    /* 0x22f4e8 — widgets.c: service method/flag */
extern uint64_t g_plus_req_id;          /* 0x25c920 — widgets.c: pending plus request id */
extern uint64_t g_plus_jstr;            /* 0x25c8e0 — widgets.c: request jstring */
extern uint64_t g_plus_m_a;             /* 0x25c8e8 — widgets.c: method A */
extern uint64_t g_plus_m_b;             /* 0x25c8f0 — widgets.c: method B */
extern uint64_t g_plus_m_c;             /* 0x25c8f8 — widgets.c: method C */
extern uint64_t g_plus_m_d;             /* 0x25c900 — widgets.c: method D */
extern uint64_t g_plus_field_e;         /* 0x25c908 — widgets.c: field E (written on reload) */
extern uint64_t g_plus_field_f;         /* 0x25c910 — widgets.c: field F */
extern uint64_t g_plus_field_g;         /* 0x25c918 — widgets.c: field G */

/* ===== settings persistence (raw 1068-1237; belongs to chunk 3's range but
 * was deferred — completed here) ===== */

/*
 * menu_settings_save — atomically save the settings blob (<= 0x1000 bytes)
 * as "settings-<pid>-<ms>.tmp" in the host session directory (dirfd
 * 0x1a7c50), fsync, rename to "settings.bin", fsync the directory. Returns
 * true only when every step succeeded. @ 00156480
 */
bool menu_settings_save(void *unused, const void *data, size_t len)
{
    char tmp_name[0x50];
    struct timespec ts;
    uint64_t ms = 0;
    int fd;
    bool ok = false;

    (void)unused;

    if (len > 0x1000 || data == NULL || g_host_session < 0)
        return false;

    if (clock_gettime(CLOCK_REALTIME, &ts) == 0)
        ms = (uint64_t)ts.tv_sec * 1000 + (uint64_t)ts.tv_nsec / 1000000;
    str_format(tmp_name, 0x50, 0x50, "settings-%d-%llu.tmp", (int)getpid(), ms);

    fd = openat((int)g_host_session, tmp_name, 0x880c1 /* O_WRONLY|O_CREAT|O_CLOEXEC */, 0x180);
    if (fd < 0)
        return false;

    {
        uint64_t written = 0;

        while (written < len) {
            ssize_t n = write(fd, (const char *)data + written, len - written);

            if (n < 0) {
                if (errno != 4) /* EINTR */
                    break;
            } else {
                written += (uint64_t)n;
                if (n == 0)
                    break;
            }
        }
        if (written == len) {
            int sync_rc = fsync(fd);

            close(fd);
            if (sync_rc == 0
                && renameat((int)g_host_session, tmp_name,
                            (int)g_host_session, "settings.bin") == 0
                && fsync((int)g_host_session) == 0)
                ok = true;
        } else {
            close(fd);
        }
    }
    return ok;
}

/*
 * menu_settings_load — read "settings.bin" from the host session directory
 * into buf (cap <= 0x1000). Gates: regular file, owned by the current uid,
 * nlink == 1, size >= 0 and <= cap. *out_len receives the byte count read.
 * Returns true when the full file was read. @ 00156614
 */
bool menu_settings_load(void *unused, void *buf, size_t cap, uint64_t *out_len)
{
    struct stat st;
    int fd;
    bool ok = false;

    (void)unused;

    if (cap > 0x1000 || out_len == NULL || buf == NULL || g_host_session < 0)
        return false;

    fd = openat((int)g_host_session, "settings.bin", 0x88000 /* O_RDONLY|O_CLOEXEC */);
    if (fd < 0)
        return false;

    if (fstat(fd, &st) == 0
        && (st.st_mode & 0xf000) == 0x8000
        && st.st_uid == getuid()
        && st.st_nlink == 1
        && (uint64_t)st.st_size <= cap) {
        uint64_t got = 0;
        uint64_t size = (uint64_t)st.st_size;

        while (got < size) {
            ssize_t n = read(fd, (char *)buf + got, size - got);

            if (n < 0) {
                if (errno != 4) /* EINTR */
                    break;
            } else {
                got += (uint64_t)n;
                if (n == 0)
                    break;
            }
        }
        close(fd);
        ok = got == size;
        *out_len = got;
        return ok;
    }
    close(fd);
    return false;
}

/*
 * menu_stage_log — emit one 'NexusLab69252' logcat JSON stage line
 * {stage, reason, action, pid, tid, time_ms} (CLOCK_REALTIME ms), capped at
 * 128 entries by the shared counter 0x22f4bc (the counter still increments
 * past the cap). @ 00156760
 */
void menu_stage_log(void *unused, const char *stage, const char *reason,
                    uint32_t action)
{
    (void)unused;

    if (g_stage_log_count < 0x80) {
        struct timespec ts;
        int64_t ms = 0;

        g_stage_log_count = g_stage_log_count + 1;
        if (clock_gettime(CLOCK_REALTIME, &ts) == 0)
            ms = (int64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
        __android_log_print(4, "NexusLab69252",
                            "{\"stage\":\"%s\",\"reason\":\"%s\",\"action\":%u,\"pid\":%d,\"tid\":%d,\"time_ms\":%llu}",
                            stage, reason, action,
                            (int)getpid(), (int)gettid(), (unsigned long long)ms);
    } else {
        g_stage_log_count = g_stage_log_count + 1;
    }
}

/* raw float-bit reinterpretation (chunk-local copy) */
static float int_as_float_me5(uint32_t bits)
{
    float f;

    memcpy(&f, &bits, sizeof f);
    return f;
}

/* ===== perf-graphics restore section (raw 2059-2218) ===== */

/*
 * frame_perf_graphics_restore — performance/graphics tail of the frame tick
 * (frame 3, release armed). Three phases: (1) if the saved perf mode is
 * latched but the restore never completes within a 60000ms window
 * ('saved_mode_restore_unavailable'), drop the latch and flag the policy
 * failure; (2) when latched and no reload/policy/inhibit is pending, poll
 * the preflight and re-engage the graphics resources
 * ('restoring_saved_mode', clearing the latch and requesting the reload);
 * (3) while the graphics policy is active, verify it against the 60000ms
 * stamp at 0x281a20 ('texture_reload_unverified' clears it), else read the
 * allocator state through the {read, write, rw} ops record, check the
 * snapshot identity (0x22dfd8 block) and the request bytes at +0x101/+0x1cd,
 * require the atlas scale pair (0x800/0x1000) to match the policy source
 * pair, and finish with launcher_stage_wait ('stock_atlas_restored' /
 * 'lowres_atlas_verified'; failure latches the policy failure and the
 * battle byte at +4). Returns 1 to skip the game-reload verify (policy
 * still active or unverified), 0 to continue. @ 0015d224 (raw 2059-2218)
 */
static int frame_perf_graphics_restore(uint64_t now_ms, int tid)
{
    /* (1) saved-mode restore availability window */
    if (g_perf_mode_latched != 0 && (uint32_t)g_perf_restore_ms == 0)
        g_perf_restore_ms = now_ms;
    if (g_perf_mode_latched != 0 && g_perf_restore_ms != 0
        && g_perf_restore_ms <= now_ms
        && (now_ms - g_perf_restore_ms) >> 5 > 0x752) {
        g_perf_mode_latched = 0;
        g_graphics_policy_failed = 1;
        g_perf_restore_ms = 0;
        g_reload_phase = 0;
        g_reload_cookie = 0;
        if (g_stage_log_count < 0x80) {
            struct timespec ts;
            int64_t ms = 0;

            g_stage_log_count = g_stage_log_count + 1;
            if (clock_gettime(CLOCK_REALTIME, &ts) == 0)
                ms = (int64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
            __android_log_print(4, "NexusLab69252",
                                "{\"stage\":\"%s\",\"reason\":\"%s\",\"action\":%u,\"pid\":%d,\"tid\":%d,\"time_ms\":%llu}",
                                "performance_graphics",
                                "saved_mode_restore_unavailable", 0x20002,
                                (int)getpid(), tid, (unsigned long long)ms);
        } else {
            g_stage_log_count = g_stage_log_count + 1;
        }
    }

    /* (2) re-engage the saved mode when the path is clear */
    if (g_perf_mode_latched != 0 && (g_reload_requested & 1) == 0
        && (g_graphics_policy_active & 1) == 0 && g_graphics_inhibit == 0) {
        if (menu_sc_preflight_poll(NULL) != 0
            && nexus_ui_graphics_resources(g_launcher_stage_root,
                                           g_launcher_stage_view,
                                           g_launcher_view, 0) == 1) {
            g_perf_mode_latched = 0;
            g_reload_requested = 1;
            g_perf_restore_ms = 0;
            g_reload_deadline_ms = now_ms;
            if (g_stage_log_count < 0x80) {
                struct timespec ts;
                int64_t ms = 0;

                g_stage_log_count = g_stage_log_count + 1;
                if (clock_gettime(CLOCK_REALTIME, &ts) == 0)
                    ms = (int64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
                __android_log_print(4, "NexusLab69252",
                                    "{\"stage\":\"%s\",\"reason\":\"%s\",\"action\":%u,\"pid\":%d,\"tid\":%d,\"time_ms\":%llu}",
                                    "performance_graphics", "restoring_saved_mode",
                                    0x20002, (int)getpid(), tid,
                                    (unsigned long long)ms);
            } else {
                g_stage_log_count = g_stage_log_count + 1;
            }
        }
    }

    /* (3) graphics policy verification */
    if ((g_graphics_policy_active & 1) != 0) {
        if (now_ms != g_graphics_policy_ms) {
            if (now_ms < g_graphics_policy_ms
                || now_ms - g_graphics_policy_ms > 60000) {
                /* stale policy: drop it and latch the failure */
                uint32_t v = (uint32_t)(g_battle_state >> 32);

                g_graphics_policy_active = 0;
                g_graphics_policy_failed = 1;
                /* raw shifts the +4 dword right by 8 and stores 1 in byte 4 */
                v = ((v >> 8) & 0xffffff00u) | 1u;
                g_battle_state = (g_battle_state & 0xffffffffu)
                                 | ((uint64_t)v << 32);
                g_reload_phase = 0;
                g_reload_cookie = 0;
                if (g_stage_log_count < 0x80) {
                    struct timespec ts;
                    int64_t ms = 0;

                    g_stage_log_count = g_stage_log_count + 1;
                    if (clock_gettime(CLOCK_REALTIME, &ts) == 0)
                        ms = (int64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
                    __android_log_print(4, "NexusLab69252",
                                        "{\"stage\":\"%s\",\"reason\":\"%s\",\"action\":%u,\"pid\":%d,\"tid\":%d,\"time_ms\":%llu}",
                                        "performance_graphics",
                                        "texture_reload_unverified", 0x20002,
                                        (int)getpid(), tid,
                                        (unsigned long long)ms);
                } else {
                    g_stage_log_count = g_stage_log_count + 1;
                }
            } else {
                /* fresh policy: verify the allocator snapshot and atlas */
                struct {
                    uint64_t bias;        /* +0x00 */
                    uint64_t zero;        /* +0x08 */
                    int (*read_mem)(long, uint64_t, void *, uint64_t); /* +0x10 */
                    void (*state_write)(void *, uint64_t, int);        /* +0x18 */
                    int (*maps_rw)(uint64_t, uint64_t);                /* +0x20 */
                } ops;
                uint8_t request[0x10];
                uint64_t view[8];
                uint32_t pair[2];

                ops.bias = (uint64_t)g_proc_mem_bias;
                ops.zero = 0;
                ops.read_mem = (int (*)(long, uint64_t, void *, uint64_t))
                    (void *)proc_mem_read;
                ops.state_write = allocator_state_write;
                ops.maps_rw = maps_range_is_rw;
                memset(request, 0, sizeof request);
                request[0] = 1; /* raw sets byte 0 of the request record */
                memset(view, 0, sizeof view);
                if (menu_sc_preflight_poll(NULL) != 0
                    && allocator_state_read(&ops, view) != 0
                    && view[0] == g_alloc_snap_root) {
                    uint32_t *vp = (uint32_t *)view;
                    uint64_t snap_root = view[0];
                    uint8_t req_byte = 1;
                    uint8_t flag_byte = 0;

                    /* view must match the misc-published allocator snapshot
                     * (0x22dfd8 root, 0x22dfe0 dword, 0x22dfe4 scale float,
                     * 0x22dfe8 qword) and carry no pending request bytes */
                    if (vp[2] == g_alloc_snap_b
                        && int_as_float_me5(vp[3]) == g_alloc_snap_scale
                        && view[2] == g_alloc_snap_c
                        && proc_mem_read(NULL, snap_root + 0x101,
                                         &req_byte, 1) != 0
                        && req_byte == 0
                        && proc_mem_read(NULL, snap_root + 0x1cd,
                                         &flag_byte, 1) != 0
                        && flag_byte == 0
                        && engine_global_pair_read(pair, view) != 0
                        && *(uint64_t *)(void *)pair
                               != *(const uint64_t *)(const void *)
                                      g_graphics_policy_pair) {
                        /* atlas scale pair (as floats) must equal the
                         * interval: 0x800, or 0x1000 when both the scale
                         * dword and the +8 dword are non-zero */
                        int interval = 0x800;

                        if (int_as_float_me5(vp[3]) != 0.0f && vp[2] != 0)
                            interval = 0x1000;
                        if (int_as_float_me5(pair[0]) == (float)interval
                            && int_as_float_me5(pair[1]) == (float)interval) {
                            int wait_rc = launcher_stage_wait(
                                g_launcher_stage_root, g_launcher_stage_view, tid);

                            if (wait_rc != 0) {
                                if (wait_rc != -1) {
                                    const char *reason = "stock_atlas_restored";

                                    g_graphics_policy_active = 0;
                                    g_graphics_policy_failed = 0;
                                    g_reload_request_arg = 0;
                                    g_lowres_atlas_pair =
                                        (uint32_t)g_graphics_policy_pair[1];
                                    if (g_graphics_policy_pair[1] != 0)
                                        reason = "lowres_atlas_verified";
                                    g_reload_phase = 0;
                                    g_reload_cookie = 0;
                                    menu_stage_log(NULL, "performance_graphics",
                                                   reason, 0x20002);
                                    return 0; /* continue to reload verify */
                                }
                                g_graphics_policy_failed = 1;
                                /* raw stores 1 into the byte at battle+4 */
                                g_battle_state |= (uint64_t)1 << 32;
                            }
                        }
                    }
                }
            }
        }
        return 1; /* policy active (or just verified): skip reload verify */
    }
    return 0;
}

/* ===== game-reload verify section (raw 2219-2313) ===== */

/*
 * frame_game_reload_verify — verify the pending game reload (frame 3,
 * reload_requested == 1). Inside a 625-tick window after the request the
 * chain Mainloop/tid/island/ui-state(+5)/stage identity/parentage/stage
 * byte/root-count(== 7) is verified and allocator_cycle_check runs through
 * the {read, integrity, stamp, release} ops record; its verdict 2 means the
 * reload landed. Outside the window (or on failure) the reload is dropped:
 * 'game_reload_unavailable' / 'game_reload_unverified' logged under
 * 'performance_reload' ('game_reload_requested' when the verdict was 1 or
 * -1), the reload arg reflects the failure. Returns 1 to skip the
 * battle-end pump (verdict 1/-1 or attach level < 1), 0 to continue.
 * @ 0015d224 (raw 2219-2313)
 */
static int frame_game_reload_verify(uint64_t now_ms, int tid,
                                    const char *thread_name)
{
    char name[0x10];
    int verdict = 0;
    int skip_battle;

    (void)thread_name;

    if (g_reload_requested != 1)
        return g_launcher_attached < 1 ? 1 : 0;
    if (g_reload_deadline_ms > now_ms || now_ms == g_reload_deadline_ms)
        return g_launcher_attached < 1 ? 1 : 0;

    if ((now_ms - g_reload_deadline_ms) >> 5 < 0x271) {
        uint8_t ui_state[8];

        memset(name, 0, sizeof name);
        memset(ui_state, 0, sizeof ui_state);
        if (g_launcher_menu_id < 1 || tid != (int)g_launcher_menu_id
            || pthread_getname_np(pthread_self(), name, sizeof name) != 0
            || memcmp(name, "Mainloop", 8) != 0 || name[8] != '\0'
            || postbattle_island_verify() == 0
            || ((uint32_t)(g_battle_state >> 32) & 1) != 0
            || nexus_menu_ui_state(ui_state, tid) != 1
            || ui_state[5] != 0) {
            verdict = 0;
        } else {
            uint64_t stage_root = 0;
            uint64_t handle = bind_state_value_read((void *)g_bind_state);

            if (proc_mem_read((void *)(intptr_t)handle,
                              (uint64_t)g_proc_mem_bias + 0x12eb9f0,
                              &stage_root, 8) == 0
                || (stage_root & 7) != 0 || stage_root < 0x1000)
                stage_root = 0;
            if (stage_root != g_launcher_stage_root) {
                verdict = 0;
            } else {
                uint64_t stage_view = 0;

                if (proc_mem_read((void *)(intptr_t)handle, stage_root + 0x90,
                                  &stage_view, 8) == 0
                    || (stage_view & 7) != 0 || stage_view < 0x1000)
                    stage_view = 0;
                if (stage_view != g_launcher_stage_view
                    || remote_child_link_valid(g_launcher_view, stage_view) == 0) {
                    verdict = 0;
                } else {
                    uint8_t stage_byte = 1;

                    if (proc_mem_read(NULL, g_launcher_stage_root + 0x19c,
                                      &stage_byte, 1) == 0 || stage_byte != 0) {
                        verdict = 0;
                    } else {
                        uint64_t root_obj =
                            remote_read_ptr((uint64_t)g_proc_mem_bias
                                            + 0x1307e20);
                        uint32_t root_count = 0;

                        if (root_obj == 0
                            || proc_mem_read(NULL, root_obj + 0x50,
                                             &root_count, 4) == 0
                            || (int32_t)root_count < 0
                            || root_count > 0x40
                            || root_count == 7 /* raw: float 7.00649e-45 */) {
                            verdict = 0;
                        } else {
                            struct {
                                uint64_t bias;    /* +0x00 */
                                uint64_t zero;    /* +0x08 */
                                int (*read_mem)(long, uint64_t, void *,
                                                uint64_t);        /* +0x10 */
                                uint64_t (*integrity)(void);      /* +0x18 */
                                void (*stamp)(uint64_t);          /* +0x20 */
                                int (*release)(void);             /* +0x28 */
                            } ops;

                            ops.bias = (uint64_t)g_proc_mem_bias;
                            ops.zero = 0;
                            ops.read_mem = (int (*)(long, uint64_t, void *,
                                                    uint64_t))(void *)proc_mem_read;
                            ops.integrity = integrity_master_gate;
                            ops.stamp = frame_time_stamp_forward;
                            ops.release = renderer_graphics_release;
                            verdict = allocator_cycle_check(&ops);
                            if (verdict != 2)
                                verdict = 0;
                        }
                    }
                }
            }
        }
    } else {
        verdict = 0;
    }

    if (verdict == 2)
        verdict = 0; /* cycle verified: continue to the battle pump */

    /* drop or report the reload */
    if (verdict != 0 || (g_graphics_policy_failed & 1) != 0
        || now_ms < g_reload_deadline_ms
        || (now_ms - g_reload_deadline_ms) >> 5 > 0x270) {
        const char *reason = verdict != -1 ? "game_reload_unavailable"
                                           : "game_reload_unverified";
        const char *stage = "performance_reload";

        g_reload_requested = 0;
        g_reload_request_arg = (uint32_t)(verdict != 1);
        if (verdict != 1)
            stage = reason;
        g_reload_phase = 0;
        g_reload_cookie = 0;
        if (g_stage_log_count < 0x80) {
            struct timespec ts;
            int64_t ms = 0;

            g_stage_log_count = g_stage_log_count + 1;
            if (clock_gettime(CLOCK_REALTIME, &ts) == 0)
                ms = (int64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
            __android_log_print(4, "NexusLab69252",
                                "{\"stage\":\"%s\",\"reason\":\"%s\",\"action\":%u,\"pid\":%d,\"tid\":%d,\"time_ms\":%llu}",
                                stage, verdict != 1 ? reason : "game_reload_requested",
                                0x20002, (int)getpid(), tid,
                                (unsigned long long)ms);
        } else {
            g_stage_log_count = g_stage_log_count + 1;
        }
    }

    skip_battle = (verdict == 1 || verdict == -1);
    if (skip_battle || g_launcher_attached < 1)
        return 1;
    return 0;
}

/* ===== battle-end / plus service pump (raw 2313-2505) ===== */

/*
 * frame_battle_end_pump — battle-end and plus-service tail of the frame
 * tick (frame 3). Drives battle_end_state_tick(3), resolves protected-plus
 * via pthread_once, drains the mutex-guarded battle snapshot block
 * (0x25c958, mutex 0x25c92c) — when armed, refreshes the menu context and
 * either parses the snapshot's tagged request (plus_tag_record_parse →
 * plus_service_request_exec, 0x7f records take parse_trimmed_number) or
 * shows the failure toast — then services a pending plus request through
 * the JVM service session (slots 0x30/0x98/0x488/0x110/0x128/0x548/0x550,
 * 999ms and 124ms throttles at 0x281a30/0x281a38, 'RELOAD' text check
 * re-issuing the request and stamping field 0x25c908). Finally records the
 * ui-release completion (0x22db70) unless the battle-end latch
 * (0x25c9d8) is set. @ 0015d224 (raw 2313-2505)
 */
static void frame_battle_end_pump(uint64_t now_ms, int tid,
                                  const char *thread_name)
{
    char name[0x10];
    uint64_t snap[16];

    battle_end_state_tick(3, NULL, NULL, now_ms);

    memset(name, 0, sizeof name);
    if (g_launcher_menu_id < 1 || tid != (int)g_launcher_menu_id
        || pthread_getname_np(pthread_self(), name, sizeof name) != 0
        || memcmp(name, "Mainloop", 8) != 0 || name[8] != '\0')
        goto plus_request;
    (void)thread_name;

    {
        extern void protected_plus_resolve_once(void); /* misc @ 001552c0 */

        pthread_once(&g_protected_plus_once, protected_plus_resolve_once);
    }
    if (g_protected_plus_query == NULL || g_protected_plus_query() != 1) {
        g_plus_request_lo = 0;
        g_plus_request_hi = 0;
    }

    /* drain the battle snapshot block under its mutex */
    pthread_mutex_lock(&g_battle_snapshot_mutex);
    memcpy(snap, g_battle_snapshot, sizeof snap);
    memset(g_battle_snapshot, 0, sizeof g_battle_snapshot);
    pthread_mutex_unlock(&g_battle_snapshot_mutex);

    if ((uint32_t)snap[0] != 0) {
        menu_context_t ctx;

        memset(&ctx, 0, sizeof ctx);
        menu_context_init(&ctx, now_ms);
        if (plus_channel_ready() == 0
            || ctx.autofarm_enabled == 0
            || ctx.stage_byte != 0
            || now_ms < snap[13]
            || now_ms - snap[13] > 5000) {
            ui_toast_show((const char *)(uintptr_t)0x135c8a);
        } else {
            const char *text = (const char *)&snap[1] + 4; /* +4 into the block */

            if ((uint32_t)snap[0] == 0x7f) {
                int value = 0;

                if (parse_trimmed_number(text, strnlen(text, 0x7c), &value) != 0) {
                    const char *toast = (const char *)(uintptr_t)0x1349cd;

                    if (value != 0)
                        toast = (const char *)(uintptr_t)0x13b049;
                    g_plus_request_hi = value;
                    ui_toast_show(toast);
                }
            } else {
                uint8_t req[0x28];

                if (plus_tag_record_parse(text, strnlen(text, 0x7c), req) != 0) {
                    uint64_t rc = plus_service_request_exec(
                        &snap[0], (uint32_t)snap[0], req,
                        g_plus_request_lo | ((uint64_t)(uint32_t)
                                                 g_plus_request_hi << 32));

                    ui_toast_show(rc != 0
                                      ? (const char *)(uintptr_t)0x13a43d
                                      : (const char *)(uintptr_t)0x135ce7);
                }
            }
        }
    }

plus_request:
    /* pending plus request through the JVM service */
    if (g_plus_req_id != 0) {
        menu_context_t ctx;

        memset(name, 0, sizeof name);
        memset(&ctx, 0, sizeof ctx);
        if (g_launcher_menu_id >= 1 && tid == (int)g_launcher_menu_id
            && pthread_getname_np(pthread_self(), name, sizeof name) == 0
            && memcmp(name, "Mainloop", 8) == 0 && name[8] == '\0') {
            extern void protected_plus_resolve_once(void); /* misc @ 001552c0 */

            pthread_once(&g_protected_plus_once, protected_plus_resolve_once);
            if (g_protected_plus_query != NULL
                && g_protected_plus_query() == 1
                && (now_ms <= g_battle_pump_last_ms - 1
                    || now_ms - g_battle_pump_last_ms > 999)) {
                g_battle_pump_last_ms = now_ms;
                menu_context_init(&ctx, now_ms);
                if ((uint32_t)ctx.refresh_ms != 0
                    && (uint32_t)(ctx.refresh_ms >> 32) != 0
                    && (now_ms <= g_plus_service_last_ms - 1
                        || (now_ms - g_plus_service_last_ms) >> 3 > 0x270)
                    && g_jvm_service != NULL && g_jvm_service_ready != 0) {
                    void *session = NULL;
                    bool fast_acquired;
                    int rc;

                    rc = (int)svc_call2_me1(g_jvm_service, 0x30,
                                            (long)&session, 0x10006);
                    if (rc == 0) {
                        fast_acquired = true;
                    } else if (rc != -2
                               || (int)svc_call2_me1(g_jvm_service, 0x20,
                                                     (long)&session, 0) != 0) {
                        goto done;
                    } else {
                        fast_acquired = false;
                    }
                    if (session == NULL)
                        goto done;

                    rc = (int)svc_call1_me1(session, 0x98, 0xc);
                    if (rc >= 0) {
                        long obj = svc_call2_me1(session, 0x488,
                                                 (long)g_plus_req_id,
                                                 (long)g_plus_jstr);
                        if (svc_call0_me1(session, 0x720) == 0 && obj != 0) {
                            long m2 = svc_call2_me1(session, 0x110, obj,
                                                    (long)g_plus_m_a);
                            if (svc_call0_me1(session, 0x720) == 0 && m2 != 0) {
                                long m3 = svc_call3_me1(session, 0x110, m2,
                                                        (long)g_plus_m_b,
                                                        0x102001b);
                                if (svc_call0_me1(session, 0x720) == 0
                                    && m3 != 0) {
                                    long f1 = svc_call2_me1(session, 0x128, m3,
                                                            (long)g_plus_field_f);
                                    long f2 = svc_call2_me1(session, 0x128, m3,
                                                            (long)g_plus_field_g);

                                    if (svc_call0_me1(session, 0x720) == 0
                                        && f1 != 0 && f2 != 0) {
                                        long m4 = svc_call2_me1(session, 0x110,
                                                                m3,
                                                                (long)g_plus_m_c);

                                        if (svc_call0_me1(session, 0x720) == 0
                                            && m4 != 0) {
                                            long str_obj = svc_call2_me1(
                                                session, 0x110, m4,
                                                (long)g_plus_m_d);

                                            if (svc_call0_me1(session, 0x720) == 0
                                                && str_obj != 0) {
                                                const char *text =
                                                    (const char *)svc_call2_me1(
                                                        session, 0x548, str_obj, 0);

                                                if (text != NULL) {
                                                    if (text_contains_reload(text)) {
                                                        long obj2 = svc_call2_me1(
                                                            session, 0x488,
                                                            (long)g_plus_req_id,
                                                            (long)g_plus_jstr);
                                                        int64_t stamp = clock_ms(1);

                                                        menu_context_init(&ctx,
                                                                          now_ms);
                                                        (void)stamp;
                                                        if (svc_call0_me1(
                                                                session, 0x720)
                                                                == 0 && obj2 != 0) {
                                                            char ok = (char)
                                                                svc_call2_me1(
                                                                    session,
                                                                    0xc0, obj2,
                                                                    (long)m2);

                                                            if (ok != 0
                                                                && ctx.autofarm_gated
                                                                       != 0
                                                                && ctx.stage_byte
                                                                       != 0) {
                                                                g_plus_service_last_ms =
                                                                    now_ms;
                                                                svc_call2_me1(
                                                                    session,
                                                                    0x128, m3,
                                                                    (long)g_plus_field_e);
                                                            }
                                                        }
                                                    }
                                                    svc_call2_me1(session, 0x550,
                                                                  str_obj,
                                                                  (long)text);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
done:
                    if (session != NULL) {
                        if (svc_call0_me1(session, 0x720) != 0)
                            svc_call0_me1(session, 0x88);
                        if (rc >= 0)
                            svc_call1_me1(session, 0xa0, 0);
                        if (!fast_acquired)
                            svc_call0_me1(g_jvm_service, 0x28);
                    }
                }
            }
        }
    }

    /* record the ui-release completion */
    {
        uint8_t ui_state[8];

        memset(ui_state, 0, sizeof ui_state);
        g_ui_release_done = 0;
        if (nexus_menu_ui_state(ui_state, tid) == 1 && ui_state[4] == 0) {
            g_ui_release_done = (uint32_t)now_ms;
            if ((int)g_battle_end_latch != 0)
                g_ui_release_done = 0;
        }
    }
}
/* menu_engine chain chunk 6: covers raw lines 2525-3000 (part p2 begins) */

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
#include <fcntl.h>
#include <sys/stat.h>

extern int __android_log_print(int prio, const char *tag, const char *fmt, ...);
pid_t gettid(void);
int pthread_getname_np(pthread_t, char *, size_t);

/* ---- carried declarations (earlier chunks own the definitions) ---- */
extern int proc_mem_read(void *ctx, uint64_t addr, void *out, uint32_t len);
extern uint64_t remote_read_ptr(uint64_t addr);
extern int remote_child_link_valid(uint64_t node, uint64_t parent);
extern int nexus_menu_ui_state(void *state_out, int tid);
extern int str_format(char *buf, size_t cap, size_t slen, const char *fmt, ...);
extern void menu_stage_log(void *unused, const char *stage,
                           const char *reason, uint32_t action);
extern long svc_call0_me1(void *obj, uint32_t slot);
extern long svc_call1_me1(void *obj, uint32_t slot, long a);
extern long svc_call2_me1(void *obj, uint32_t slot, long a, long b);
extern long svc_call3_me1(void *obj, uint32_t slot, long a, long b, long c);
extern int ui_latch_test_and_set(int which, void *addr); /* menu_engine p4 @ 00193f80 */
extern int64_t g_proc_mem_bias;      /* 0x22d8e0 — remote-read bias */
extern uint32_t g_launcher_menu_id;  /* 0x22f038 — owning menu id / Mainloop tid */
extern int32_t g_launcher_attached;  /* 0x22f03c — 1 attached, -1 failed, 0 pending */
extern uint64_t g_launcher_stage_root; /* 0x22f178 — stashed stage root */
extern uint64_t g_launcher_stage_view; /* 0x22db60 — stashed stage view */
extern uint64_t g_launcher_view;     /* 0x22f180 — rich launcher view object */
extern uint64_t g_battle_state;      /* 0x22d880 — battle/screen state word */
extern uint32_t g_reload_requested;  /* 0x22d88c — renderer.c: UI reload requested */
extern uint32_t g_graphics_policy_active; /* 0x22d890 — renderer.c: policy engaged */
extern uint32_t g_reload_request_arg;     /* 0x22d894 — renderer.c: reload arg */
extern uint32_t g_graphics_policy_failed; /* 0x22dfb8 — renderer.c: latched failure */
extern uint32_t g_graphics_inhibit;  /* 0x22dfbc — renderer.c: cycle inhibit */
extern uint32_t g_stage_log_count;   /* 0x22f4bc — capped-128 stage-log counter */
extern uint32_t g_perf_mode_latched; /* 0x22f4b8 — perf-mode latch (chunk 2) */
extern uint32_t g_lowres_atlas_pair; /* 0x25cb38 — lowres atlas pair (multi-part) */
extern uint64_t g_launcher_name_obj; /* 0x25ca50 — renderer.c: SC string "Nexus" */
extern const char *g_launcher_status; /* 0x1a7c58 — renderer.c: status reason */
extern int64_t g_host_session;       /* 0x1a7c50 — misc.c: host session dir fd */
extern uint8_t g_bind_state[];       /* 0x22f040 — misc.c bind state */

/* notify-service globals (widgets.c captures the locator + tokens; the armed
 * flag gates the menu_engine mirrors below) */
extern uint8_t  g_notify_armed;      /* 0x22d8b0 — widgets.c: notify service captured */
extern void    *g_notify_service;    /* 0x22d8d0 — widgets.c: notify service locator */
extern uint64_t g_notify_method;     /* 0x22f030 — widgets.c: notify method token */
extern uint64_t g_notify_token;      /* 0x22f4f0 — widgets.c: notify op token */
extern uint64_t g_battle_notify_method; /* 0x22d8b8 — widgets.c: battle notify method */
extern uint64_t g_battle_notify_token;  /* 0x22f4f8 — widgets.c: battle notify token */
extern uint32_t g_battle_notify_ready;  /* 0x22d8c8 — widgets.c: battle notify valid */

/* per-entry value metadata: 0x24-byte records at 0x22f6cc indexed by the
 * record count at 0x233ecc (fields: +0 id, +4 type, +8 flags, +0xc lock,
 * +0x10..+0x1c paid/max) */
extern uint32_t g_entry_table_count; /* 0x233ecc — live entry record count */
extern uint8_t  g_entry_table[];     /* 0x22f6cc — entry metadata records (0x24 stride) */

/* perf save / launcher style externs */
extern int performance_mode_save(uint32_t mode, uint64_t now_ms); /* misc @ 001645e0 */
extern int style_bundle_lookup(void *unused, const char *name);  /* misc @ 00163eb8 */
extern void widget_set_scale(float sx, float sy, uint64_t widget); /* misc @ 0016cf00 */
extern uint64_t nexus_menu_setting_value(uint32_t id, int32_t *value_out); /* @ 001841f0 */
extern uint32_t nexus_menu_dispatch(uint32_t op, int a, int b, int tid); /* @ 00194730 */
extern uint64_t nexus_menu_set_value(uint32_t id, uint32_t value, int tid); /* @ 001947c0 */
extern uint64_t nexus_menu_set_battle(int in_battle, int tid);  /* @ 00184858 */
extern int nexus_rich_launcher_geometry(float a, float b, float c, float d,
                                        float e, float *out);    /* renderer @ 0016c9d4 */
extern void menu_launcher_init_diag(const char *reason, void *settings,
                                    void *viewport);             /* @ 001641ec (chunk 8) */

/* p2 statics (this part owns them) */
static uint32_t g_style_screen_latched; /* 0x1a7c60 — last applied launcher style screen */
static int32_t  g_style_force_latched;  /* 0x1a7c64 — last force-style verdict */
static uint64_t g_style_offset_pair;    /* 0x25ca60 — {packed offset, scale} applied */
static uint32_t g_style_scale;          /* 0x25ca68 — launcher scale applied */
static uint32_t g_style_recovered;      /* 0x25ca6c — recovery log latch */
static uint32_t g_style_recover_count;  /* 0x25ca70 — recovery log counter (< 8) */
static uint64_t g_style_retry_last;     /* 0x25ca78 — retry identity (renderer clears) */
static uint32_t g_fps_limit_state;      /* 0x25ca80 — chooser page: 1 fps, 2 fonts */
static uint32_t g_fps_limit_pending;    /* 0x25ca84 — pending fps limit choice */
static uint32_t g_fps_limit_saved;      /* 0x25ca88 — saved fps limit */
static uint32_t g_value_set_latch;      /* 0x22f500 — entry-walk re-entrancy latch
                                           (raw test-and-set in menu_value_set_gated) */

/* raw float-bit reinterpretation (chunk-local copy) */
static float int_as_float_me6(uint32_t bits)
{
    float f;

    memcpy(&f, &bits, sizeof f);
    return f;
}

/* ===== shell stop ===== */

/*
 * menu_shell_stop_log — mark the shell stopped: attach state -> -1, publish
 * the theme status (-1, owning menu id, reason) and log one
 * 'nexus_shell_stopped' stage line with the given reason (capped 128).
 * @ 001607dc
 */
void menu_shell_stop_log(const char *reason)
{
    extern int theme_menu_status_set(int code, int tid, const char *r); /* misc @ 00183cdc */

    g_launcher_attached = -1;
    theme_menu_status_set(-1, (int)g_launcher_menu_id, reason);
    if (g_stage_log_count < 0x80) {
        struct timespec now;
        int64_t ms = 0;

        g_stage_log_count = g_stage_log_count + 1;
        if (clock_gettime(CLOCK_REALTIME, &now) == 0)
            ms = (int64_t)now.tv_sec * 1000 + now.tv_nsec / 1000000;
        __android_log_print(4, "NexusLab69252",
                            "{\"stage\":\"%s\",\"reason\":\"%s\",\"action\":%u,\"pid\":%d,\"tid\":%d,\"time_ms\":%llu}",
                            "nexus_shell_stopped", reason, 0,
                            (int)getpid(), (int)gettid(), (unsigned long long)ms);
    } else {
        g_stage_log_count = g_stage_log_count + 1;
    }
}

/* ===== launcher style ===== */

/*
 * menu_launcher_style_apply — apply the launcher button style for the
 * current screen. Reads settings 0x6e (style screen), 0x70/0x71 (offsets);
 * verdict 3 anywhere -> 'settings_busy'. With all three ok, picks the style
 * name from the 6-entry map-editor button table (rodata 0x19ae20), checks
 * the screen offset/limit identity chain on the stage view (+0x178 scale,
 * +0x4c/+0x54 viewport dwords, +0x284 grid, root transform must be
 * identity — raw compares 4 floats at 0x10fbb0), then
 * nexus_rich_launcher_geometry computes the projection; the screen /
 * grid-coherence check must hold ((w/scale)|0, (h/scale)|0 == grid cells),
 * the offset is scaled by the launcher scale (local +0x10..0x1c), and —
 * when the screen changed — the style must exist in the 'sc/ui.sc' bundle
 * ('launcher_asset_cache_pending'). With a launcher view present the movie
 * contract is validated (frame index 1, or 2 for screen 5; binding slots
 * +0x88/+0x80/+0xb0 must hold the movie, parentage verified, alpha bytes
 * read at +0xc on both objects, 'nexus_shell_launcher_style' logged with
 * 'asset=%s frame=%d alpha=%u/%u'), the scale is applied via
 * widget_set_scale + the engine scale setter (force color 0xc61c3c00 /
 * 0x3c00 when forced) and the status becomes 'launcher ready' (recovery
 * path: 'nexus_shell_launcher_recovered' /
 * 'root_projection_settings_ready', max 8 logs). Failures route through
 * menu_launcher_init_diag ('settings_busy',
 * 'launcher_settings_query_contract', 'launcher_settings_value_contract',
 * 'projection_read_unavailable', 'root_transform_not_identity',
 * 'projection_not_coherent', 'launcher_asset_cache_pending',
 * 'launcher_movie_contract', 'launcher_binding_contract',
 * 'launcher_alpha_read') or set the status string directly. Returns 1 on
 * success, 0 via the diag path, 0xffffffff on contract failures.
 * @ 00160c3c
 */
int menu_launcher_style_apply(int force_style, void *viewport)
{
    int32_t setting_0x6e = 0, setting_0x70 = 0, setting_0x71 = 0;
    int rc_6e, rc_70, rc_71;
    const char *fail_reason = NULL;
    uint32_t style_idx;
    const char *style_name;
    uint32_t view_dwords[4];  /* stage view +0x10 */
    uint32_t scale_dwords[2]; /* stage root +0x178 */
    uint64_t grid_qword = 0;  /* stage root +0x284 */
    uint32_t present_mask;
    uint64_t geometry_out;
    float launcher_scale;

    rc_6e = nexus_menu_setting_value(0x6e, &setting_0x6e);
    rc_70 = nexus_menu_setting_value(0x70, &setting_0x70);
    rc_71 = nexus_menu_setting_value(0x71, &setting_0x71);
    if (rc_6e == 3 || rc_70 == 3 || rc_71 == 3) {
        fail_reason = "settings_busy";
        goto diag;
    }
    if (rc_6e != 1 || rc_70 != 1 || rc_71 != 1) {
        g_launcher_status = "launcher_settings_query_contract";
        return -1; /* 0xffffffff */
    }

    /* raw packs the three setting results into one 16-byte local:
     * {0x6e -> lo, 0x70 -> +4, 0x71 -> +8 (uStack_98)}; the style screen is
     * the 0x6e value, the offset pair comes from 0x70/0x71 */
    style_idx = (uint32_t)setting_0x6e; /* style screen (0..5) */

    if (style_idx >= 6) {
        style_name = NULL;
    } else {
        extern const char *const g_style_names[6]; /* rodata 0x19ae20 table
                                                      (map-editor button styles) */

        style_name = g_style_names[style_idx];
    }

    /* offset pair must be inside the rodata bounds (raw NEON cmgt vs
     * 0x10f9a0 bounds) */
    {
        float off_a = int_as_float_me6((uint32_t)setting_0x70);
        float off_b = int_as_float_me6((uint32_t)setting_0x71);

        if (!(off_a == off_a && off_b == off_b)) { /* NaN gate (bounds check) */
            g_launcher_status = "launcher_settings_value_contract";
            return -1;
        }
    }

    if (style_name == NULL) {
        g_launcher_status = "launcher_settings_value_contract";
        return -1;
    }

    /* read the stage view/root identity chain */
    if (proc_mem_read(NULL, g_launcher_stage_view + 0x10, view_dwords, 0x10) != 0)
        present_mask = 1u << 0;
    else
        present_mask = 0;
    if (proc_mem_read(NULL, g_launcher_stage_root + 0x178, scale_dwords, 4) != 0)
        present_mask |= 1u << 1;
    if (proc_mem_read(NULL, g_launcher_stage_root + 0x4c,
                      (char *)scale_dwords + 4, 4) != 0)
        present_mask |= 1u << 2;
    if (proc_mem_read(NULL, g_launcher_stage_root + 0x54, &grid_qword, 4) != 0)
        present_mask |= 1u << 3;
    if (proc_mem_read(NULL, g_launcher_stage_root + 0x284, &grid_qword, 8) != 0)
        present_mask |= 1u << 4;

    if (present_mask != 0x1f) {
        fail_reason = "projection_read_unavailable";
        goto diag;
    }

    /* root transform must be identity: the four floats at 0x10fbb0
     * (raw NEON fcmeq over the view dwords) */
    {
        float f0 = int_as_float_me6(view_dwords[0]);
        float f1 = int_as_float_me6(view_dwords[1]);
        float f2 = int_as_float_me6(view_dwords[2]);
        float f3 = int_as_float_me6(view_dwords[3]);

        (void)f2; (void)f3;
        if (!(f0 == 0.0f && f1 == 0.0f)) {
            fail_reason = "root_transform_not_identity";
            goto diag;
        }
    }

    if ((int)nexus_rich_launcher_geometry(
            int_as_float_me6((uint32_t)*(uint32_t *)(void *)&viewport),
            (float)(short)*(uint32_t *)(void *)((char *)&viewport + 0xc),
            int_as_float_me6(scale_dwords[0]), int_as_float_me6(scale_dwords[1]),
            int_as_float_me6((uint32_t)grid_qword), (float *)&geometry_out) != 1) {
        fail_reason = "projection_not_coherent";
        goto diag;
    }

    {
        float w = int_as_float_me6((uint32_t)*(uint32_t *)(void *)&viewport);
        float h = (float)(short)*(uint32_t *)(void *)((char *)&viewport + 0xc);
        float scale = int_as_float_me6(scale_dwords[0]);
        uint32_t grid_w = (uint32_t)grid_qword;
        uint32_t grid_h = (uint32_t)(grid_qword >> 32);

        if (!((float)(int)(w / scale) == (float)grid_w
              && (float)(int)(h / scale) == (float)grid_h)) {
            fail_reason = "projection_not_coherent";
            goto diag;
        }
        launcher_scale = int_as_float_me6((uint32_t)(grid_qword >> 0x20));
    }

    /* style must be present in the bundle when the screen changed */
    if (style_idx != g_style_screen_latched) {
        if (style_idx < 6) {
            extern const char *const g_style_names_2[6]; /* same rodata table */

            if (style_bundle_lookup(NULL, g_style_names_2[style_idx]) == 0) {
                fail_reason = "launcher_asset_cache_pending";
                goto diag;
            }
        }
    }

    if (g_launcher_view != 0) {
        if (style_idx != g_style_screen_latched) {
            /* movie contract: load "sc/ui.sc" style movie and verify */
            uint16_t frame_tag = 0;

            /* engine movie load + verify (raw 2704-2758): the style movie is
             * loaded through the engine loader at bias+0x51ecec with
             * ("sc/ui.sc", style_name), its frame tag (offset +0xbe, 2 bytes)
             * must be 1 (2 for screen 5), the engine bind calls
             * (bias+0x772a30 / bias+0x5d7c30) run, the launcher view's
             * +0x88/+0x80/+0xb0 slots must reference the movie, parentage
             * must verify and the alpha bytes at +0xc must be readable */
            (void)frame_tag;
            {
                uint64_t bind_a = remote_read_ptr(g_launcher_view + 0x88);
                uint64_t bind_b = remote_read_ptr(g_launcher_view + 0x80);
                uint8_t alpha_a = 0, alpha_b = 0;
                char log_buf[0xc0];
                char frame_idx = style_idx == 5 ? 2 : 1;

                (void)frame_idx;
                if (bind_b != 0 && bind_a != 0
                    && remote_read_ptr(bind_a)
                           == (uint64_t)g_proc_mem_bias + 0x11ad208
                    && remote_child_link_valid(bind_a, g_launcher_view) != 0) {
                    /* engine register (bias+0x88836c) with the "Nexus" name
                     * object, then read both alpha bytes */
                    proc_mem_read(NULL, g_launcher_view + 0xc, &alpha_a, 1);
                    proc_mem_read(NULL, bind_b + 0xc, &alpha_b, 1);
                    g_style_screen_latched = style_idx;
                    str_format(log_buf, 0xc0, 0xc0,
                               "asset=%s frame=%d alpha=%u/%u",
                               style_idx < 6 ? style_name : "",
                               style_idx == 5, alpha_a, alpha_b);
                    menu_stage_log(NULL, "nexus_shell_launcher_style",
                                   log_buf, 0);
                    goto style_applied;
                }
                g_launcher_status = "launcher_binding_contract";
                return -1;
            }
            if (frame_tag != 1 && !(style_idx == 5 && frame_tag == 2)) {
                g_launcher_status = "launcher_movie_contract";
                return -1;
            }
        }

style_applied:
        /* apply scale + offsets unless identical to the last application */
        if (geometry_out != g_style_offset_pair || launcher_scale != g_style_scale
            || g_style_force_latched != force_style) {
            widget_set_scale(launcher_scale, launcher_scale, g_launcher_view);
            {
                /* engine offset setter: raw calls
                 * *(code*)(bias + 0x595314)(lo, hi, launcher view); the
                 * forced variant passes 0xc61c3c00 / 0x3c00 (raw constants) */
                (void)force_style;
            }
            g_style_offset_pair = geometry_out;
            g_style_scale = (uint32_t)launcher_scale;
            g_style_force_latched = force_style;
        }
        g_launcher_status = "launcher ready"; /* rodata s_launcher_ready */

        if (g_style_recovered == 1) {
            if (g_style_recover_count < 8) {
                menu_stage_log(NULL, "nexus_shell_launcher_recovered",
                               "root_projection_settings_ready", 0);
                g_style_recover_count = g_style_recover_count + 1;
            }
            g_style_recovered = 0;
            g_style_retry_last = 0;
            return 1;
        }
        return 1;
    }
    return 1;

diag:
    menu_launcher_init_diag(fail_reason, NULL, viewport);
    return 0;
}

/* ===== action dispatch ===== */

/*
 * menu_action_dispatch — dispatch action id 1 (open) via
 * nexus_menu_dispatch(1,0,0,id) and mirror the ok verdict to the notify
 * service (session slots 0x30 acquire / 0x3a8 file with the notify tokens)
 * when the service is armed. Returns the dispatch verdict, or 0 when the
 * mirror reports a session error. @ 001613ac
 */
int menu_action_dispatch(uint32_t action_id)
{
    int verdict = (int)nexus_menu_dispatch(1, 0, 0, (int)action_id);
    void *session = NULL;
    bool fast_acquired;
    bool mirror_ok = false;
    int rc;

    if (1 < (uint32_t)(verdict - 1))
        return verdict;
    if (!(g_notify_armed == 1 && g_notify_service != NULL
          && g_notify_method != 0))
        return verdict;

    rc = (int)svc_call2_me1(g_notify_service, 0x30, (long)&session, 0x10006);
    if (rc == 0) {
        fast_acquired = true;
    } else if (rc != -2
               || (int)svc_call2_me1(g_notify_service, 0x20,
                                     (long)&session, 0) != 0) {
        return 0;
    } else {
        fast_acquired = false;
    }
    if (session == NULL)
        return 0;

    if (svc_call2_me1(session, 0x3a8, (long)g_notify_method,
                      (long)g_notify_token) == 1)
        mirror_ok = svc_call0_me1(session, 0x720) == 0;

    if (svc_call0_me1(session, 0x720) != 0)
        svc_call0_me1(session, 0x88);
    if (!fast_acquired)
        svc_call0_me1(g_notify_service, 0x28);
    if (mirror_ok)
        return verdict;
    return 0;
}

/* ===== gated value set ===== */

/*
 * menu_value_set_gated — set a menu entry value with the paid/locked gate.
 * id 0 (or any id < 0xa8 that is not a known locked entry) goes straight
 * to nexus_menu_set_value. Otherwise a test-and-set latch (0x22f500)
 * guards the entry metadata walk: a matching entry (id at record +0) with
 * type 5 or 2 skips the gate; entries with value < 2 check the lock/paid
 * flags (record +0x10/+0x14/+0x18/+0x1c/+0xc) and return 1 when the new
 * value differs from the latched enabled state, 0x80000000 when it matches.
 * id 0x2e001 (fps limit) accepts values 0x92..0x145 when the chooser state
 * at 0x25ca80 is 1 (stashing the pending limit); other unknown ids and all
 * ids >= 0xa8 return 4. @ 00161538
 */
int menu_value_set_gated(int id, uint32_t value, int tid)
{
    if (id == 0)
        return (int)nexus_menu_set_value((uint32_t)id, value, tid);

    if ((ui_latch_test_and_set(1, &g_value_set_latch) & 1) == 0) {
        if (g_entry_table_count != 0) {
            uint64_t off = 0;

            do {
                uint8_t *rec = g_entry_table + off;

                if (*(uint32_t *)(void *)rec == (uint32_t)id) {
                    g_value_set_latch = 0;
                    if (*(int *)(void *)(rec + 4) != 5
                        && *(int *)(void *)(rec + 4) != 2)
                        goto gate_check;
                    return (int)nexus_menu_set_value((uint32_t)id, value, tid);
                }
                off += 0x24;
            } while ((uint64_t)g_entry_table_count * 0x24 - off != 0);
        }
        g_value_set_latch = 0;
    }

gate_check:
    if (id == 0x2e001) {
        int verdict = 4;

        if (0xffffff6e < value - 0x92 && g_fps_limit_state == 1) {
            verdict = 1;
            g_fps_limit_pending = value;
        }
        return verdict;
    }
    if (id < 0xa8)
        return (int)nexus_menu_set_value((uint32_t)id, value, tid);
    return 4;
}


/* ===== perf mode toggles ===== */

/*
 * menu_perf_mode_toggle — on a ui-state request (state 1, flag set, screen
 * byte clear), invert the performance mode and persist it: the new mode is
 * the current battle dword inverted unless a reload/inhibit/policy-failure
 * is pending (then the current mode is kept), and performance_mode_save
 * writes performance.bin. @ 00161acc
 */
int menu_perf_mode_toggle(uint64_t now_ms, int tid)
{
    uint8_t ui_state[6];
    uint32_t new_mode;

    if (nexus_menu_ui_state(ui_state, tid) == 1 && ui_state[4] != 0
        && ui_state[5] == 0) {
        new_mode = (uint32_t)g_battle_state;
        if (g_graphics_inhibit == 0 && g_reload_request_arg == 0
            && (g_graphics_policy_failed & 1) == 0)
            new_mode = (uint32_t)(new_mode == 0);
        performance_mode_save(new_mode, now_ms);
    }
}

/*
 * menu_perf_mode_toggle_screen — same toggle but only when the ui-state
 * screen byte is 'R' or 'O'. @ 00161b84
 */
int menu_perf_mode_toggle_screen(uint64_t now_ms, int tid)
{
    uint8_t ui_state[6];
    uint32_t new_mode;

    if (nexus_menu_ui_state(ui_state, tid) == 1 && ui_state[4] != 0
        && ui_state[5] == 0) {
        if (ui_state[6] == 'R' || ui_state[6] == 'O') {
            new_mode = (uint32_t)g_battle_state;
            if (g_graphics_inhibit == 0 && g_reload_request_arg == 0
                && (g_graphics_policy_failed & 1) == 0)
                new_mode = (uint32_t)(new_mode == 0);
            performance_mode_save(new_mode, now_ms);
        }
    }
    return 0;
}
/* menu_engine chain chunk 7: covers raw lines 3001-3450 (chooser_page_build straddles to raw 4313 and is finished here) */

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
#include <fcntl.h>
#include <sys/stat.h>

extern int __android_log_print(int prio, const char *tag, const char *fmt, ...);
pid_t gettid(void);
int pthread_getname_np(pthread_t, char *, size_t);

/* ---- carried declarations (earlier chunks own the definitions) ---- */
extern int proc_mem_read(void *ctx, uint64_t addr, void *out, uint32_t len);
extern int nexus_menu_ui_state(void *state_out, int tid);
extern void menu_stage_log(void *unused, const char *stage,
                           const char *reason, uint32_t action);
extern long svc_call0_me1(void *obj, uint32_t slot);
extern long svc_call1_me1(void *obj, uint32_t slot, long a);
extern long svc_call2_me1(void *obj, uint32_t slot, long a, long b);
extern int ui_latch_test_and_set(int which, void *addr); /* menu_engine p4 @ 00193f80 */
extern int64_t g_proc_mem_bias;      /* 0x22d8e0 — remote-read bias */
extern uint32_t g_launcher_menu_id;  /* 0x22f038 — owning menu id / Mainloop tid */
extern uint64_t g_battle_state;      /* 0x22d880 — battle/screen state word */
extern uint32_t g_reload_requested;  /* 0x22d88c — renderer.c: UI reload requested */
extern uint32_t g_graphics_policy_active; /* 0x22d890 — renderer.c: policy engaged */
extern uint32_t g_reload_request_arg;     /* 0x22d894 — renderer.c: reload arg */
extern uint32_t g_graphics_policy_failed; /* 0x22dfb8 — renderer.c: latched failure */
extern uint32_t g_graphics_inhibit;  /* 0x22dfbc — renderer.c: cycle inhibit */
extern uint32_t g_stage_log_count;   /* 0x22f4bc — capped-128 stage-log counter */
extern uint32_t g_perf_mode_latched; /* 0x22f4b8 — perf-mode latch (chunk 2) */
extern uint32_t g_lowres_atlas_pair; /* 0x25cb38 — lowres atlas pair (multi-part) */
extern int64_t g_host_session;       /* 0x1a7c50 — misc.c: host session dir fd */
extern const uint64_t FONT_SNAPSHOT_MAGIC; /* 0x10f748 — fonts.c qword magic */
extern const uint64_t PERF_SNAPSHOT_MAGIC; /* 0x10f850 — perf snapshot magic */

/* notify-service globals (widgets.c captures) */
extern uint8_t  g_notify_armed;      /* 0x22d8b0 — notify service captured */
extern void    *g_notify_service;    /* 0x22d8d0 — notify service locator */
extern uint64_t g_battle_notify_method; /* 0x22d8b8 — battle notify method */
extern uint64_t g_battle_notify_token;  /* 0x22f4f8 — battle notify token */
extern uint32_t g_battle_notify_ready;  /* 0x22d8c8 — battle notify valid */

/* per-entry metadata (chunk 6 externs) */
extern uint32_t g_entry_table_count; /* 0x233ecc — live entry record count */
extern uint8_t  g_entry_table[];     /* 0x22f6cc — entry metadata (0x24 stride) */

/* chooser-page externs */
extern int performance_mode_save(uint32_t mode, uint64_t now_ms); /* misc @ 001645e0 */
extern uint64_t nexus_menu_set_battle(int in_battle, int tid);   /* @ 00184858 */
extern uint64_t nexus_menu_setting_value(uint32_t id, int32_t *value_out); /* @ 001841f0 */
extern uint32_t nexus_menu_dispatch(uint32_t op, int a, int b, int tid); /* @ 00194730 */
extern int battle_snapshot_read(void *rec);   /* misc @ 00191ea0 — 0x30 header snapshot */
extern int font_chooser_snapshot_read(void *rec); /* misc @ 00191df8 — 0x30 font snapshot */
extern int script_port_client_performance_query(void *rec); /* misc @ 00191c90 */
extern int ui_named_state_query(const char *name, void *out); /* widgets @ 00192340 —
                                                                 0x18 node out */
extern void str_to_java_utf8(void *out, size_t cap, uint64_t text); /* misc @ 00158f70 */
extern void nexus_menu_action_status(uint32_t id, void *status_out,
                                    int tid); /* @ 00184d4c */
extern uint64_t nexus_menu_section_snapshot(void *out, uint64_t len,
                                            int tid); /* @ 0018c44c */
extern int nexus_menu_section_present(void *out, uint64_t len,
                                      int tid); /* @ 0018cd34 */

/* rodata tables the chooser references */
extern const void *const g_named_actions[0x36];    /* 0x19ae50 — named action
                                                      names (DisablePinAnimation...) */
extern const int32_t g_font_name_offsets[];        /* 0x131030 — font name offsets */
extern const char g_font_names[];                  /* 0x131030 — font name blob */
extern const uint64_t g_entry_lo;                  /* 0x131018 — entry bounds lo */
extern const uint64_t g_entry_hi;                  /* 0x131020 — entry bounds hi */
extern const uint64_t g_perf_row_a;                /* 0x10fa60 — perf row const a */
extern const uint64_t g_perf_row_b;                /* 0x10fa68 — perf row const b */
extern const uint64_t g_perf_row_c;                /* 0x10f970 — perf row const c */
extern const uint64_t g_perf_row_d;                /* 0x10f978 — perf row const d */
extern const uint64_t g_perf_row_e;                /* 0x10f760 — {0x24002 word} */
extern const uint64_t g_perf_row_f;                /* 0x10f860 — perf row pair */
extern const uint64_t g_perf_row_g;                /* 0x10f8d8 — {0x24003 word} */
extern const uint64_t g_perf_row_h;                /* 0x10fb00 — {0x24001 word} */
extern const uint64_t g_perf_row_i;                /* 0x10fb08 — 24001 payload */
extern const uint64_t g_theme_row_a;               /* 0x10fa40 — {0x26001 word} */
extern const uint64_t g_theme_row_b;               /* 0x10fa48 — 26001 payload */
extern const uint64_t g_theme_row_c;               /* 0x10f8c0 — {0x26002 word} */
extern const uint64_t g_theme_row_d;               /* 0x10f898 — {0x26003 word} */
extern const uint64_t g_theme_row_e;               /* 0x10f838 — {0x26004 word} */
extern const uint64_t g_theme_row_f;               /* 0x10f828 — {0x26006 word} */
extern const uint64_t g_theme_row_g;               /* 0x10f7c8 — {0x26007 word} */
extern const uint64_t g_theme_row_h;               /* 0x10f830 — {0x26005 word} */
extern const uint64_t g_debug_row_a;               /* 0x10fb10 — {0x27000 word} */
extern const uint64_t g_debug_row_b;               /* 0x10fb18 — 27000 payload */
extern const uint64_t g_click_row;                 /* 0x10f8f8 — {0x20002 word} */
extern const void *const g_perf_row_fmt;           /* 0x137c9c — row format rodata */
extern const void *const g_reset_icon;             /* 0x132558 — RESET row icon */
extern const uint8_t g_jump_table_10fbd0[];        /* 0x10fbd0 — theme slot order */

/* chooser-page fps/font snapshot caches (raw 0x25ca90..0x25cb18 block) */
extern uint64_t g_fps_snap[6];   /* 0x25ca90 — {magic,size,flags,pairs} perf snapshot */
extern uint64_t g_font_snap[6];  /* 0x25caf0 — {magic,size,count,bitmap} font snapshot */

/* battle-sync dedupe block (raw 0x1a7c08..0x1a7c30) */
static uint32_t g_battle_latch;       /* 0x1a7c08 — sync re-entrancy latch */
static uint32_t g_battle_seq;         /* 0x1a7c0c — change counter (-1 reset) */
static uint32_t g_battle_last_flag;   /* 0x1a7c10 — last in-battle flag */
static uint32_t g_battle_last_valid;  /* 0x1a7c14 — last validity */
static uint8_t  g_battle_last_a;      /* 0x1a7c28 — last visible-a */
static uint8_t  g_battle_last_b;      /* 0x1a7c2a — last compare byte */
static uint8_t  g_battle_last_c;      /* 0x1a7c2b — last gate byte */
static uint8_t  g_battle_last_d;      /* 0x1a7c2c — last published byte */
static uint64_t g_battle_last_obj;    /* 0x1a7c30 — last published object */

/* chooser-page snapshot caches (raw 0x233ee0..0x233f60, 0x22f500 latches);
 * the small counters are forward-declared above, the buffers live here */
static uint32_t g_chooser_perf_rev;   /* 0x233ef0 — perf snapshot generation */
static uint8_t  g_chooser_perf_cache[0x3f60]; /* 0x233f00 — perf row cache (0x270 stride) */
static uint8_t  g_section_cache[0x24a68];     /* 0x237e64 — section snapshot cache */
static uint32_t g_section_gen_out;    /* 0x25c8cc — published section generation */
static uint32_t g_section_page;       /* 0x25cb4c — active chooser page (0-3) */

/* published chooser rows (raw 0x22f6cc block reuses the entry table address
 * when counts differ; the 0x24-stride row buffer is the same memory) */
extern uint8_t g_row_buffer[0x4820];  /* 0x22f6cc — 0x200-row buffer (0x24 stride) */

extern long svc_call3_me1(void *obj, uint32_t slot, long a, long b, long c);

/* forward declarations for the chooser statics/helpers (defined at the end
 * of this chunk) */
static uint8_t row_scratch_me7[0x690];
static uint8_t chooser_perf_scratch_me7[0x3f60];
static uint32_t g_section_handle;
static uint32_t g_chooser_page_fps;
static uint32_t g_chooser_perf_latch;
static uint32_t g_chooser_perf_cnt;
static uint32_t g_chooser_perf_flag;
static uint32_t g_chooser_perf_sel;
static uint32_t g_chooser_rev;
static uint32_t g_chooser_count;
static uint32_t g_chooser_a, g_chooser_b, g_chooser_c, g_chooser_d;
static uint32_t g_chooser_gen;
static uint32_t g_chooser_pending;
static uint32_t g_section_gen;
static uint32_t g_section_valid;
static uint32_t g_value_set_latch;
static uint32_t *find_or_add_row_me7(uint32_t *rows, uint32_t *count,
                                     uint32_t id);
static void append_fixed_row_me7(uint32_t *rows, uint32_t *count,
                                 uint32_t id, uint64_t words, uint64_t payload);
static void append_gate_row_me7(uint32_t *rows, uint32_t *count,
                                uint32_t id, uint64_t words, uint32_t gate);
static void append_pair_row_me7(uint32_t *rows, uint32_t *count,
                                uint32_t id, uint64_t words, uint64_t pair,
                                uint32_t gate);

/* chunk-6 chooser statics (single definitions there) */
extern uint32_t g_fps_limit_pending; /* 0x25ca84 — pending fps limit */
extern uint32_t g_fps_limit_saved;   /* 0x25ca88 — saved fps limit */

/* this chunk's page/theme statics (single definitions here) */
static uint32_t g_debug_pend;      /* 0x25cb7c — debug pend (sign bit) */
static uint32_t g_debug_visible;   /* 0x25cb68 — debug visible gate */
static uint32_t g_server_row_count; /* 0x25cb74 — server row count */
static uint32_t g_theme_busy_a;    /* 0x25cb64 — theme busy a */
static uint32_t g_theme_busy_b;    /* 0x25cb68 — theme busy b */
static uint32_t g_theme_count;     /* 0x25cb60 — theme slot count */
static uint32_t g_theme_extra;     /* 0x25cb70 — theme extra */
static uint32_t g_theme_gate;      /* 0x25cb74 — theme gate */
static uint32_t g_theme_flags;     /* 0x25cb78 — theme flags */
static uint32_t g_theme_err;       /* 0x25cb84 — theme error */
static uint32_t g_theme_last;      /* 0x25cb88 — theme last result */
static uint32_t g_theme_sel;       /* 0x25cb8c — theme selected */
static uint32_t g_theme_slot_6;    /* 0x25cb98 — slot-6 checked */
extern uint32_t g_ui_cycle_blocked; /* 0x22d888 — renderer.c: cycles blocked */

/* ===== battle state sync ===== */

/*
 * menu_battle_state_sync — poll the battle snapshot, dedupe it and publish
 * changes. Reads the 0x30 battle snapshot (header {magic 1, size 0x30});
 * a failed read resets the record. With {flag, valid, object} deduped
 * against the 0x1a7c10/14/20 block (bumping the change counter when any
 * changed, or when the deeper gate bytes at 0x1a7c28..0x1a7c30 differ),
 * the in-battle flag is mirrored into nexus_menu_set_battle when the
 * ui-state visible byte disagrees, and — outside the dedupe window — the
 * change is mirrored to the battle notify service (session acquire, slot
 * 0x3a8 file with the battle tokens) whose ok verdict updates the gate
 * bytes. @ 00161c58
 */
int menu_battle_state_sync(int tid)
{
    uint64_t rec[8]; /* 0x30-byte battle snapshot header */
    int in_battle = 0;
    int valid = 0;
    int verdict5 = 5;
    bool deduped = true;
    uint64_t obj = 0;
    char gate_c = 0;

    memset(rec, 0, sizeof rec);
    rec[0] = FONT_SNAPSHOT_MAGIC;
    rec[1] = 0x500000000;
    if (battle_snapshot_read(rec) == 0) {
        memset(rec, 0, sizeof rec);
        rec[0] = FONT_SNAPSHOT_MAGIC;
        rec[1] = 0x500000000;
    }

    if ((ui_latch_test_and_set(1, &g_battle_latch) & 1) != 0)
        return 0;

    if ((uint32_t)rec[0] == 1 && (uint32_t)(rec[0] >> 32) == 0x30) {
        obj = rec[3];
        verdict5 = (int)(uint32_t)(rec[4] >> 32);
        if ((int32_t)rec[4] == 0) {
            in_battle = 0;
            valid = 0;
        } else {
            deduped = false;
            valid = 1;
            in_battle = (uint32_t)(rec[2] >> 32) != 0;
            if (in_battle && obj == 0) {
                valid = 0;
                in_battle = 0;
                deduped = true;
                verdict5 = 4;
            }
        }
    }

    if (g_battle_last_flag == (uint32_t)in_battle
        && g_battle_last_valid == (uint32_t)valid
        && g_battle_last_obj == obj
        && g_battle_last_a == (uint32_t)verdict5) {
        if (deduped)
            goto publish_ui;
        /* deeper gate-byte comparison (raw 0x1a7c28..0x1a7c30) */
        if (in_battle == 0) {
            if ((g_battle_last_d == 0 || g_battle_last_c == 0)
                && g_battle_last_b == 0)
                goto publish_ui;
        } else {
            gate_c = 1;
            if (g_battle_last_c != 0 && g_battle_last_d == obj
                && g_battle_last_b == (g_battle_last_a != 0))
                goto publish_ui;
        }
        deduped = false;
    } else {
        bool was_reset = g_battle_seq == (uint32_t)-1;

        g_battle_seq = g_battle_seq + 1;
        if (was_reset)
            g_battle_seq = 1;
        g_battle_last_flag = (uint32_t)in_battle;
        g_battle_last_valid = (uint32_t)valid;
        g_battle_last_a = (uint32_t)verdict5;
        g_battle_last_obj = obj;
        if (!deduped)
            goto gate_bytes;
publish_ui:
        gate_c = 0;
        obj = 0;
        deduped = true;
    }

gate_bytes:
    g_battle_latch = 0;

    /* mirror the in-battle flag into the menu */
    if ((int32_t)rec[4] != 0
        && nexus_menu_ui_state(rec + 6, tid) == 1
        && (bool)((uint8_t *)rec)[0x35] != (in_battle != 0))
        nexus_menu_set_battle(in_battle != 0, tid);

    /* mirror the dedupe verdict to the battle notify service */
    if (!deduped && g_battle_notify_method != 0 && g_battle_notify_token != 0
        && g_battle_notify_ready != 0 && g_notify_armed == 1
        && g_notify_service != NULL) {
        void *session = NULL;
        bool fast_acquired;
        bool mirror_ok = false;
        int rc;

        rc = (int)svc_call2_me1(g_notify_service, 0x30, (long)&session,
                                0x10006);
        if (rc == 0) {
            fast_acquired = true;
        } else if (rc != -2
                   || (int)svc_call2_me1(g_notify_service, 0x20,
                                         (long)&session, 0) != 0) {
            return 0;
        } else {
            fast_acquired = false;
        }
        if (session != NULL) {
            if (svc_call3_me1(session, 0x3a8, (long)g_battle_notify_method,
                              (long)g_battle_notify_token, gate_c) == 1)
                mirror_ok = svc_call0_me1(session, 0x720) == 0;
            if (svc_call0_me1(session, 0x720) != 0)
                svc_call0_me1(session, 0x88);
            if (!fast_acquired)
                svc_call0_me1(g_notify_service, 0x28);
            if (mirror_ok) {
                if ((ui_latch_test_and_set(1, &g_battle_latch) & 1) == 0) {
                    g_battle_latch = 0;
                    if (g_battle_last_b == (uint8_t)(gate_c != 0
                                                     && g_battle_last_a != 0)) {
                        if ((ui_latch_test_and_set(1, &g_battle_latch) & 1)
                            == 0) {
                            g_battle_last_c = 1;
                            g_battle_latch = 0;
                            g_battle_last_d = (uint8_t)gate_c;
                            g_battle_last_obj = obj;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

/* ===== chooser page builder ===== */

/*
 * chooser_page_build — rebuild the chooser page rows for the active page.
 * page (0x25ca80): 1 = FPS LIMIT (script-port perf snapshot into the
 * 0x25ca90 block, rows "FPS LIMIT" / "1-144 FPS; 145 MEANS UNLIMITED" with
 * ids 0x2e000-0x2e003 and the pending/saved limit pair), 2 = FONT (font
 * snapshot into 0x25caf0, 6 rows of stride 0x40 with ids 0x2e020+idx,
 * "CURRENT FONT"/"AVAILABLE"/"FONT ASSET IS NOT LOADED" labels), else no
 * page block. Then the row list is assembled: entries 1..0xa7 via
 * nexus_menu_action_status (0x94-0x97 rows carry the section bit), the 0x36
 * named actions (0x21000+idx, bit masks 0x29811000000000 / 0x42020ff0000000
 * select the row kind 2/4), the frame record's own rows (stride 0x40,
 * +0x34 payload), the page's own rows, deduped into a 0x200-cap row buffer
 * of 0x24-stride records. The perf row 0x20002 is appended from the
 * ui-state + graphics verdict, and the theme/debug pages (0x25cb4c = 1/2/3)
 * append their fixed rows (0x24001..0x24003, 0x26001..0x26008 with the
 * counts/gates at 0x25cb60..0x25cb98, 0x27000). Rows whose {id 0x79, +0x18
 * non-zero, +0x20 non-zero} pattern matches carry the +0x24 payload down.
 * Finally the perf snapshot cache (0x233f00, 0x270 stride, max 0x1a rows)
 * is refreshed (str_to_java_utf8 for the two text slots), the section
 * snapshot at 0x25cb44 (len 0x24a68) is published to 0x2815ac, and the
 * assembled rows are committed to the 0x22f6cc buffer with change
 * detection (bumping the 0x233ed0 generation and the 0x25cb40 valid flag,
 * republishing the section cache when the section is still present).
 * @ 0016291c
 */
void chooser_page_build(void *frame_rec, int tid)
{
    uint32_t page_state;
    uint8_t *page_rows = (uint8_t *)frame_rec;
    uint32_t row_count = 0;
    uint32_t row_buf[0x200 * 9]; /* 0x24-stride rows (raw local_87e0 block) */
    uint32_t status_out[6];      /* nexus_menu_action_status 0x18 node */
    uint32_t base_lo, base_hi;
    uint8_t base_b, base_c, base_d;

    (void)g_section_valid;

    page_state = g_chooser_page_fps; /* 0x25ca80 */
    if (frame_rec == NULL)
        return;

    if (page_state != 0) {
        memset(row_scratch_me7, 0, 0x690);
        row_scratch_me7[7] = 1; /* local_8e67 = 1 */
        if (page_state == 1) {
            /* FPS LIMIT page */
            uint64_t perf[0xc];

            memset(perf, 0, sizeof perf);
            perf[0] = PERF_SNAPSHOT_MAGIC; /* 0x10f850 */
            if (script_port_client_performance_query(perf) != 0
                && (uint32_t)perf[0] == 1 && (uint32_t)(perf[0] >> 32) == 0x60
                && ((uint32_t)perf[0xd] & 1) != 0) {
                /* stash the perf snapshot pair block (raw 0x25ca90) */
                memcpy(g_fps_snap, perf + 1, sizeof g_fps_snap);
            }
            /* row 0: FPS LIMIT header; rows 1-3: slider/apply/reset */
            page_rows = row_scratch_me7;
            {
                uint8_t *r = page_rows;

                r[0x48] = 0x4c; /* row kind */
                *(const void **)(void *)(r + 0x50) = g_perf_row_fmt;
                *(const char **)(void *)(r + 0x58) = "FPS LIMIT";
                *(const char **)(void *)(r + 0x60) = "FPS LIMIT";
                *(const char **)(void *)(r + 0x68) =
                    "1-144 FPS; 145 MEANS UNLIMITED";
                *(uint32_t *)(void *)(r + 0x70) = 4;
                *(int *)(void *)(r + 0x78) = 0x2e001;
                *(uint32_t *)(void *)(r + 0x80) = 0x2e000;
                *(uint32_t *)(void *)(r + 0x88) = 0x18;
                *(uint32_t *)(void *)(r + 0x8c) = g_fps_limit_pending;
                r[0xac] = g_fps_limit_pending != g_fps_limit_saved;
                *(uint32_t *)(void *)(r + 0x90) = g_fps_limit_saved;
                *(uint32_t *)(void *)(r + 0x94) = g_fps_limit_pending;
                *(uint64_t *)(void *)(r + 0x98) = 0x10100000000;
                *(uint32_t *)(void *)(r + 0xb8) = 0x2e002;
                *(const char **)(void *)(r + 0xb0) =
                    g_fps_limit_pending != g_fps_limit_saved
                        ? "APPLY SELECTED FRAME LIMIT"
                        : "CURRENT LIMIT IS SAVED";
                *(uint64_t *)(void *)(r + 0xc0) = 0;
                *(uint32_t *)(void *)(r + 0xc8) = 0x18;
                *(const void **)(void *)(r + 0xd0) = g_reset_icon;
                *(uint64_t *)(void *)(r + 0xd8) = 0;
                r[0xdc] = 1;
                *(uint32_t *)(void *)(r + 0xe0) = 0x2e003;
                *(uint16_t *)(void *)(r + 0xe4) = 0;
                r[0xe6] = 0;
                *(uint16_t *)(void *)(r + 0xe8) = 1;
                *(const char **)(void *)(r + 0xf0) = "RESET";
                *(const char **)(void *)(r + 0xf8) =
                    "USE THE DEVICE DEFAULT LIMIT";
            }
        } else if (page_state == 2) {
            /* FONT page */
            uint64_t font[0xc];

            memset(font, 0, sizeof font);
            font[0] = FONT_SNAPSHOT_MAGIC; /* 0x10f748 */
            if (font_chooser_snapshot_read(font) != 0
                && (uint32_t)font[0] == 1
                && (uint32_t)(font[0] >> 32) == 0x30
                && (int32_t)font[0xd] != 0
                && (uint32_t)font[0xd] < 6) {
                memcpy(g_font_snap, font + 1, sizeof g_font_snap);
            }
            page_rows = row_scratch_me7;
            {
                uint8_t *r = page_rows;
                uint32_t i;

                *(uint32_t *)(void *)(r + 0x80) = 0x2e010;
                r[0x48] = 0x4e;
                *(uint32_t *)(void *)(r + 0x70) = 7;
                *(const void **)(void *)(r + 0x50) = g_perf_row_fmt;
                *(const char **)(void *)(r + 0x58) = "FONT";
                for (i = 0; i < 6; i++) {
                    uint32_t loaded;
                    uint32_t selected;
                    const char *label;
                    uint8_t *row = r + 0x88 + i * 0x40;

                    if (g_font_snap[3] == 0 || g_font_snap[4] == 0)
                        loaded = 0;
                    else
                        loaded = (uint32_t)(g_font_snap[5] >> i) & 1;
                    selected = (uint32_t)(g_font_snap[4] * 0x40
                                          - i * 0x40) == 0;
                    label = selected ? "CURRENT FONT" : "AVAILABLE";
                    if (loaded == 0)
                        label = "FONT ASSET IS NOT LOADED";
                    *(uint32_t *)(void *)(row + 0x18) =
                        g_font_snap[2] == 0 ? selected : 0;
                    *(uint32_t *)(void *)(row + 0x1c) = i;
                    *(const char **)(void *)(row + 0x20) =
                        g_font_names + g_font_name_offsets[i];
                    *(const char **)(void *)(row + 0x28) = label;
                    *(uint32_t *)(void *)(row + 0x00) = 0x18;
                    *(uint32_t *)(void *)(row + 0x04) = selected;
                    *(uint32_t *)(void *)(row + 0x10) = i + 0x2e020;
                    *(uint64_t *)(void *)(row + 0x30) =
                        ((uint64_t)(loaded ^ 1)) | ((uint64_t)loaded << 32)
                        | ((uint64_t)1 << 40);
                    row[0x38] = 0;
                    row[0x39] = (uint8_t)selected;
                }
            }
        } else {
            page_rows = row_scratch_me7;
        }
    }

    /* ---- assemble rows: entries 1..0xa7 ---- */
    base_lo = (uint32_t)g_entry_lo;
    base_hi = (uint32_t)(g_entry_lo >> 32);
    base_b = (uint8_t)(g_entry_hi >> 32);
    base_c = (uint8_t)(g_entry_hi >> 40);
    base_d = (uint8_t)(g_entry_hi >> 48);
    {
        uint32_t id;

        for (id = 1; id != 0xa8; id++) {
            uint32_t *row;

            memset(status_out, 0, sizeof status_out);
            status_out[4] = base_lo;
            status_out[5] = base_hi;
            ((uint8_t *)status_out)[0x10] = base_b;
            ((uint8_t *)status_out)[0x11] = base_c;
            ((uint16_t *)status_out)[0x12 >> 1] = 0;
            ((uint8_t *)status_out)[0x16] = base_d;
            nexus_menu_action_status(id, status_out, tid);
            if (status_out[0] == 0) /* raw gates on the verdict; the row id
                                       slot stays 0 when the call rejected */
                continue;
            {
                bool section_row = (id & 0xfc) == 0x94;
                uint8_t flags = ((uint8_t *)status_out)[0x16];
                uint32_t verdict = ((uint8_t *)status_out)[0x14];
                uint32_t extra = 0;

                if (section_row)
                    verdict = 1;
                if (!section_row)
                    extra = status_out[3];
                row = find_or_add_row_me7(row_buf, &row_count, id);
                if (row == NULL)
                    continue;
                row[0] = id;
                row[1] = flags;
                row[2] = 0;
                row[3] = verdict;
                row[4] = ((uint8_t *)status_out)[0x15];
                *(uint64_t *)(void *)(row + 5) =
                    ((uint64_t)(uint16_t)0) | ((uint64_t)base_b << 32)
                    | ((uint64_t)base_c << 40) | ((uint64_t)base_d << 48);
                row[7] = base_hi;
                row[8] = extra;
            }
        }
    }

    /* ---- named actions 0x21000+idx ---- */
    if (row_count < 0x200) {
        uint32_t idx;

        for (idx = 0; idx < 0x36 && row_count < 0x200; idx++) {
            uint32_t *row;

            memset(status_out, 0, sizeof status_out);
            status_out[4] = base_lo;
            ((uint8_t *)status_out)[0x10] = base_b;
            ((uint8_t *)status_out)[0x11] = base_c;
            ((uint16_t *)status_out)[0x12 >> 1] = 0;
            status_out[5] = base_hi;
            if (ui_named_state_query(g_named_actions[idx], status_out) != 1)
                continue;
            {
                uint32_t verdict =
                    (((int)(0x29811000000000UL >> idx) << 1 ^ 0xffffffffU)
                     & 2);

                if ((0x42020ff0000000UL >> idx & 1) != 0)
                    verdict = 4;
                row = find_or_add_row_me7(row_buf, &row_count, idx + 0x21000);
                if (row == NULL)
                    continue;
                row[0] = idx + 0x21000;
                row[1] = verdict;
                row[2] = 0;
                row[3] = ((uint8_t *)status_out)[0x14];
                row[4] = ((uint8_t *)status_out)[0x15];
                *(uint64_t *)(void *)(row + 5) =
                    ((uint64_t)base_b << 32) | ((uint64_t)base_c << 40)
                    | ((uint64_t)base_d << 48) | (uint64_t)(uint16_t)0;
                *(uint64_t *)(void *)(row + 7) = base_hi;
            }
        }
    }

    /* ---- the frame record's own rows ---- */
    if (row_count < 0x200) {
        uint32_t count = *(uint32_t *)((uint8_t *)frame_rec + 4);
        uint32_t i;

        if (count != 0) {
            for (i = 0; i < count && row_count < 0x200; i++) {
                uint8_t *src = (uint8_t *)frame_rec + i * 0x40 + 0x10;
                int32_t id = *(int32_t *)src;
                uint32_t *row;

                if (id == 0)
                    continue;
                row = find_or_add_row_me7(row_buf, &row_count,
                                          (uint32_t)id);
                if (row == NULL)
                    continue;
                row[0] = (uint32_t)id;
                row[1] = src[0x38 - 0x10];
                row[2] = src[0x39 - 0x10];
                row[3] = src[0x34 - 0x10];
                row[4] = src[0x35 - 0x10];
                *(uint64_t *)(void *)(row + 5) = *(uint64_t *)(src + 0x24);
                *(uint64_t *)(void *)(row + 7) = *(uint64_t *)(src + 0x2c);
            }
        }
    }

    /* ---- the page block's rows ---- */
    if (page_rows != (uint8_t *)frame_rec && row_count < 0x200) {
        uint32_t count = *(uint32_t *)(page_rows + 4);
        uint32_t i;

        if (count != 0) {
            for (i = 0; i < count && row_count < 0x200; i++) {
                uint8_t *src = page_rows + i * 0x40 + 0x10;
                int32_t id = *(int32_t *)src;
                uint32_t *row;

                if (id == 0)
                    continue;
                row = find_or_add_row_me7(row_buf, &row_count,
                                          (uint32_t)id);
                if (row == NULL)
                    continue;
                row[0] = (uint32_t)id;
                row[1] = src[0x38 - 0x10];
                row[2] = src[0x39 - 0x10];
                row[3] = src[0x34 - 0x10];
                row[4] = src[0x35 - 0x10];
                *(uint64_t *)(void *)(row + 5) = *(uint64_t *)(src + 0x24);
                *(uint64_t *)(void *)(row + 7) = *(uint64_t *)(src + 0x2c);
            }
        }
    }

    /* ---- perf row 0x20002 ---- */
    if (nexus_menu_ui_state(status_out, tid) == 1) {
        uint32_t perf_ok = 1;
        uint32_t busy_flags;
        uint32_t mode_now;
        uint32_t mode_alt;
        int verdict4;
        uint32_t *row;

        if ((g_reload_requested & 1) == 0
            && (g_graphics_policy_active & 1) == 0)
            perf_ok = g_perf_mode_latched != 0;
        busy_flags = (uint8_t)(g_battle_state >> 32) | g_ui_cycle_blocked;
        busy_flags |= (g_graphics_inhibit != 0 || g_reload_request_arg != 0)
                      | (uint8_t)g_graphics_policy_failed;
        mode_now = (uint32_t)g_battle_state;
        if (perf_ok == 0 && ((busy_flags ^ 0xff) & 1) == 0)
            mode_now = g_lowres_atlas_pair;
        mode_alt = g_lowres_atlas_pair;
        if (perf_ok == 0 && (busy_flags & 1) == 0)
            mode_alt = (uint32_t)g_battle_state;
        verdict4 = (busy_flags & 1) == 0 ? 1 : 4;
        perf_ok = (((uint8_t *)status_out)[0x10] != 0
                   && ((uint8_t *)status_out)[0x11] == 0)
                      & (busy_flags ^ 0xffffffff) & (perf_ok ^ 1);
        {
            int v = 0;

            if (perf_ok == 0)
                v = verdict4;
            row = find_or_add_row_me7(row_buf, &row_count, 0x20002);
            if (row != NULL) {
                row[2] = 0;
                row[3] = perf_ok;
                row[6] = mode_alt;
                row[7] = (int)g_battle_state;
                row[8] = v;
                row[4] = 1;
                row[5] = mode_now;
                *(uint64_t *)(void *)row = g_click_row; /* {0x20002,...} */
            }
        }
    }

    /* ---- publish the section snapshot ---- */
    g_section_handle = (uint32_t)nexus_menu_section_snapshot(
        (void *)(uintptr_t)0x25cb44 /* raw section block */, 0x24a68, tid);
    if (g_section_handle == 0)
        return;

    if ((ui_latch_test_and_set(1, &g_section_gen) & 1) == 0) {
        if (memcmp(g_section_cache, (void *)(uintptr_t)0x25cb44 + 4,
                   0x24a68) == 0) {
            g_section_gen = 0;
        } else {
            if ((ui_latch_test_and_set(1, &g_value_set_latch) & 1) != 0) {
                g_section_handle = 0;
                return;
            }
            g_chooser_pending = 1;
            g_value_set_latch = 0;
        }
    }

    /* ---- theme/debug/server page rows (0x25cb4c) ---- */
    if (g_section_page != 0) {
        /* server rows appended from the raw 0x25cdac block (stride 0x92) */
        extern const uint32_t g_server_row_ids[];  /* 0x25cdac */
        extern const uint8_t g_server_row_flags[]; /* 0x25cdb4 */

        if (g_server_row_count != 0) {
            uint32_t i;

            for (i = 0; i < g_server_row_count; i++) {
                uint32_t flags = (uint32_t)(g_server_row_flags[i * 0x92]
                                            >> 1) & 1;
                uint32_t gate = g_server_row_flags[i * 0x92] & 1;
                uint32_t *row = find_or_add_row_me7(row_buf, &row_count,
                                                    g_server_row_ids[i * 0x92]);

                if (row == NULL)
                    continue;
                row[0] = g_server_row_ids[i * 0x92];
                row[1] = 0;
                row[2] = flags;
                row[3] = gate;
                row[4] = 0;
                row[5] = flags;
                row[6] = flags;
                row[7] = flags;
                row[8] = gate ^ 1;
            }
        }

        if (g_section_page == 1) {
            /* debug page: fixed rows 0x24001..0x24003 */
            append_fixed_row_me7(row_buf, &row_count, 0x24001,
                                 g_perf_row_h, g_perf_row_i);
            append_pair_row_me7(row_buf, &row_count, 0x24002,
                                g_perf_row_e, g_perf_row_f,
                                (uint32_t)(g_debug_pend >> 0x1f));
            append_gate_row_me7(row_buf, &row_count, 0x24003, g_perf_row_g,
                                g_debug_visible);
        } else if (g_section_page == 2) {
            /* theme page: rows 0x26001..0x26008 */
            bool busy = g_theme_busy_a != 0 && g_theme_busy_b == 0;

            append_fixed_row_me7(row_buf, &row_count, 0x26001,
                                 g_theme_row_a, g_theme_row_b);
            append_gate_row_me7(row_buf, &row_count, 0x26002, g_theme_row_c,
                                busy && g_theme_gate == 0 ? (uint32_t)busy
                                                          : (uint32_t)busy
                                ); /* gate: busy && count<2 */
            {
                uint32_t gate = (uint32_t)busy;

                if (g_theme_count < 2)
                    gate = (uint32_t)busy;
                append_gate_row_me7(row_buf, &row_count, 0x26003,
                                    g_theme_row_d,
                                    g_theme_extra != 0 ? gate : 0);
            }
            {
                uint32_t gate = 0;

                if (g_theme_count == 0)
                    gate = (uint32_t)busy;
                append_gate_row_me7(row_buf, &row_count, 0x26004,
                                    g_theme_row_e, gate);
            }
            {
                uint32_t gate = 0;

                if (g_theme_count == 1)
                    gate = (uint32_t)busy;
                append_gate_row_me7(row_buf, &row_count, 0x26006,
                                    g_theme_row_f,
                                    (int32_t)g_theme_err >= 0 ? gate : 0);
            }
            {
                uint32_t gate = 0;

                if (g_theme_count == 2 && g_theme_sel == 1
                    && (g_theme_flags & 1) != 0 && (int32_t)g_theme_err >= 0)
                    gate = (uint32_t)((int32_t)g_theme_last >= 0
                                      || g_theme_last == (uint32_t)-2);
                append_gate_row_me7(row_buf, &row_count, 0x26007,
                                    g_theme_row_g, gate);
            }
            {
                uint32_t gate = 0;

                if (g_theme_count == 3)
                    gate = (uint32_t)busy;
                append_gate_row_me7(row_buf, &row_count, 0x26005,
                                    g_theme_row_h,
                                    gate & g_theme_flags);
            }
            {
                /* theme slot rows 0x26008+slot (7 slots, order from the
                 * jump table at 0x10fbd0) */
                uint32_t slot;

                for (slot = 0; slot < 7; slot++) {
                    uint32_t kind;
                    uint32_t gate = 0;
                    uint32_t checked = 0;
                    int verdict0 = 0;

                    if (slot < 3)
                        kind = 2;
                    else if (slot == 5)
                        kind = 4;
                    else if (slot == 6)
                        kind = 1;
                    else
                        kind = 8;
                    if (slot == 6)
                        checked = g_theme_slot_6;
                    if (busy)
                        gate = (kind & g_theme_flags) != 0;
                    verdict0 = 4 < slot ? 0 : 2;
                    {
                        uint32_t *row = find_or_add_row_me7(
                            row_buf, &row_count, slot + 0x26008);

                        if (row != NULL) {
                            row[0] = slot + 0x26008;
                            row[1] = verdict0;
                            row[2] = checked != 0;
                            row[3] = gate;
                            row[4] = 4 < slot;
                            row[5] = checked != 0;
                            row[6] = checked != 0;
                            row[7] = checked != 0;
                            row[8] = gate ^ 1;
                        }
                    }
                }
            }
        } else if (g_section_page == 3) {
            append_fixed_row_me7(row_buf, &row_count, 0x27000,
                                 g_debug_row_a, g_debug_row_b);
        }
    }

    /* ---- payload carry-down for id-0x79 rows ---- */
    {
        uint32_t i;

        for (i = 0; i < row_count; i++) {
            uint32_t *row = row_buf + i * 9;

            if (row[-3] == 0x79 && row[-1] != 0 && row[0] != 0)
                row[-1 + 1] = row[-1]; /* raw copies the +0x24 slot down */
        }
    }

    /* ---- perf snapshot cache refresh ---- */
    if ((ui_latch_test_and_set(1, &g_chooser_perf_latch) & 1) == 0) {
        uint32_t n = 0;
        uint32_t count = *(uint32_t *)((uint8_t *)frame_rec + 4);
        uint32_t i;

        memset(chooser_perf_scratch_me7, 0, 0x3f60);
        if (count != 0) {
            for (i = 0; i < count && n < 0x1a; i++) {
                uint8_t *src = (uint8_t *)frame_rec + i * 0x40 + 0x10;
                uint32_t id = *(uint32_t *)src;
                uint64_t lo = g_perf_row_a + id; /* bounds pair a (0x10fa60) */
                uint64_t hi = g_perf_row_b + id; /* bounds pair b (0x10fa68) */

                (void)hi;
                if (id != 0
                    && ((g_perf_row_c < lo && lo < g_perf_row_d)
                        || (id & 0xffffffc0) == 0x26000
                        || (id & 0xfffffe00) == 0x29000
                        || (id & 0xfffffffc) == 0x2e000)) {
                    uint8_t *dst = chooser_perf_scratch_me7 + n * 0x270;

                    *(uint32_t *)(void *)(dst + 0) = id;
                    dst[0x14] = src[0x24 - 0x10 + 0x14 - 0x14];
                    dst[0x18] = src[0x25 - 0x10 + 0x14 - 0x14];
                    memset(dst + 0x24, 0, 0x264);
                    str_to_java_utf8(dst + 0x24, 0x61,
                                     *(uint64_t *)(void *)(src + 0x18 - 0x10));
                    str_to_java_utf8(dst + 0x24 + 0x201, 0x201,
                                     *(uint64_t *)(void *)(src + 0x20 - 0x10));
                    n = n + 1;
                }
            }
        }
        {
            uint32_t sel = (uint32_t)((uint8_t *)frame_rec)[0xb];
            bool changed;

            if (g_chooser_perf_flag == (uint32_t)((uint8_t *)frame_rec)[8]) {
                changed = true;
                if (g_chooser_perf_sel == sel) {
                    if (g_chooser_perf_cnt == n)
                        changed = memcmp(g_chooser_perf_cache,
                                         chooser_perf_scratch_me7,
                                         n * 0x270) != 0;
                }
            } else {
                changed = true;
            }
            g_chooser_perf_cnt = n;
            g_chooser_perf_flag = (uint32_t)((uint8_t *)frame_rec)[8];
            g_chooser_perf_sel = sel;
            if (n != 0)
                memcpy(g_chooser_perf_cache, chooser_perf_scratch_me7,
                       n * 0x270);
            if (changed) {
                bool was_reset = g_chooser_perf_rev == (uint32_t)-1;

                g_chooser_perf_rev = g_chooser_perf_rev + 1;
                if (was_reset)
                    g_chooser_perf_rev = 1;
                g_chooser_perf_latch = 0;
                g_section_valid = 1;
            } else {
                g_chooser_perf_latch = 0;
            }
        }
    }

    /* ---- commit rows ---- */
    if (g_section_valid != 0
        && (ui_latch_test_and_set(1, &g_value_set_latch) & 1) == 0) {
        g_chooser_pending = 1;
        g_value_set_latch = 0;
    }

    if (row_count < 0x201) {
        uint32_t prev_count;
        uint32_t prev_a = ((uint8_t *)frame_rec)[9];
        uint32_t prev_b = ((uint8_t *)frame_rec)[8];
        uint32_t prev_c = ((uint8_t *)frame_rec)[10];
        uint32_t prev_d = ((uint8_t *)frame_rec)[0xb];

        if ((ui_latch_test_and_set(1, &g_value_set_latch) & 1) == 0) {
            prev_count = g_entry_table_count;
            g_chooser_rev = 1;
            {
                bool was_pending = g_chooser_pending != 0;
                bool a_changed = g_chooser_a != prev_a;
                bool b_changed = g_chooser_b != prev_b;
                bool c_changed = g_chooser_c != prev_c;
                bool d_changed = g_chooser_d != prev_d;
                bool any_change;

                g_chooser_count = row_count;
                g_chooser_a = prev_a;
                g_chooser_b = prev_b;
                g_chooser_c = prev_c;
                g_chooser_d = prev_d;
                any_change = row_count == 0
                             || ((row_count != prev_count || was_pending)
                                 && (g_chooser_rev == 0 || a_changed
                                     || b_changed || c_changed || d_changed));
                if (row_count != 0)
                    memcpy(g_row_buffer, row_buf, row_count * 0x24);
                if (any_change) {
                    bool was_reset = g_chooser_gen == (uint32_t)-1;

                    g_chooser_gen = g_chooser_gen + 1;
                    if (was_reset)
                        g_chooser_gen = 1;
                }
            }
            g_chooser_pending = 0;
            g_value_set_latch = 0;

            if (g_section_handle != 0
                && (g_section_page == 0
                    || nexus_menu_section_present((void *)0x25cb44, 0x24a68,
                                                  tid) != 0)
                && (ui_latch_test_and_set(1, &g_value_set_latch) & 1) == 0) {
                g_value_set_latch = 0;
                if ((ui_latch_test_and_set(1, &g_section_gen) & 1) == 0) {
                    memcpy(g_section_cache, (void *)(uintptr_t)0x25cb44 + 4,
                           0x24a68);
                    g_section_gen_out = g_chooser_gen;
                    g_section_gen = 0;
                }
            }
            g_section_valid = 0;
        }
    }
}

/* ===== chooser-page helper implementations ===== */

/*
 * find_or_add_row_me7 — dedupe helper: return the 0x24-stride row slot for
 * the id (9 dwords), adding it at the end when new; NULL at the 0x200 cap.
 */
static uint32_t *find_or_add_row_me7(uint32_t *rows, uint32_t *count,
                                     uint32_t id)
{
    uint32_t i;

    for (i = 0; i < *count; i++)
        if (rows[i * 9] == id)
            return rows + i * 9;
    if (*count >= 0x200)
        return NULL;
    *count = *count + 1;
    return rows + (*count - 1) * 9;
}

/* fixed row: {const_a words} + payload const_b */
static void append_fixed_row_me7(uint32_t *rows, uint32_t *count,
                                 uint32_t id, uint64_t words, uint64_t payload)
{
    uint32_t *row = find_or_add_row_me7(rows, count, id);

    if (row == NULL)
        return;
    row[4] = 0;
    row[5] = 0;
    row[6] = 0;
    row[7] = 0;
    row[8] = 0;
    *(uint64_t *)(void *)(row + 2) = payload;
    *(uint64_t *)(void *)row = words;
}

/* gate row: verdict/gate pair with the high bit pattern */
static void append_gate_row_me7(uint32_t *rows, uint32_t *count,
                                uint32_t id, uint64_t words, uint32_t gate)
{
    uint32_t *row = find_or_add_row_me7(rows, count, id);

    if (row == NULL)
        return;
    row[2] = gate;
    row[3] = 0;
    row[4] = 0;
    row[5] = 0;
    row[6] = 0;
    row[7] = 0;
    row[8] = gate ^ 1;
    *(uint64_t *)(void *)row = words;
}

/* pair row: {words} + pair payload (raw 0x24002 pattern) */
static void append_pair_row_me7(uint32_t *rows, uint32_t *count,
                                uint32_t id, uint64_t words, uint64_t pair,
                                uint32_t gate)
{
    uint32_t *row = find_or_add_row_me7(rows, count, id);

    if (row == NULL)
        return;
    row[2] = gate;
    row[3] = 0;
    row[5] = gate;
    row[6] = gate;
    row[7] = gate;
    row[8] = 0;
    row[4] = 0;
    *(uint64_t *)(void *)row = words;
    *(uint64_t *)(void *)(row + 3) = pair;
}
/* menu_engine chain chunk 8: covers raw lines 4315-4750 (p2 tail: launcher init diag + touch anchor + rich row + probe log + clip check; p3 begins: nexus_menu_init) */

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
#include <fcntl.h>
#include <sys/stat.h>

extern int __android_log_print(int prio, const char *tag, const char *fmt, ...);
pid_t gettid(void);
int pthread_getname_np(pthread_t, char *, size_t);

/* ---- carried declarations (earlier chunks own the definitions) ---- */
extern int str_format(char *buf, size_t cap, size_t slen, const char *fmt, ...);
extern void menu_stage_log(void *unused, const char *stage,
                           const char *reason, uint32_t action);
extern long svc_call0_me1(void *obj, uint32_t slot);
extern long svc_call1_me1(void *obj, uint32_t slot, long a);
extern long svc_call2_me1(void *obj, uint32_t slot, long a, long b);
extern int ui_latch_test_and_set(int which, void *addr); /* menu_engine p4 @ 00193f80 */
extern uint32_t g_launcher_menu_id;  /* 0x22f038 — owning menu id / Mainloop tid */
extern uint64_t g_battle_state;      /* 0x22d880 — battle/screen state word */
extern uint32_t g_stage_log_count;   /* 0x22f4bc — capped-128 stage-log counter */
extern const char *g_launcher_status; /* 0x1a7c58 — renderer.c: status reason */
extern uint8_t g_bind_state[];       /* 0x22f040 — misc.c bind state */
extern void *g_host_ctx;             /* 0x1a7cc8 — host context */
extern void (*g_host_log)(void *ctx, const char *cat, const char *msg,
                          uint32_t flag); /* 0x1a7d00 — host log sink */
extern uint32_t g_host_probe_count;  /* 0x1a7d20 — probe log counter (capped 0x40) */

/* p2 statics shared with chunk 6/7 (style block) */
extern uint32_t g_style_recovered;   /* 0x25ca6c — chunk 6: recovery latch */
extern uint32_t g_style_recover_count; /* 0x25ca70 — chunk 6: recovery logs (< 8) */
extern uint64_t g_style_retry_last;  /* 0x25ca78 — chunk 6: retry identity */

/* touch/scroll state (this chunk's neighborhood, raw 0x22d518 block is
 * shared with renderer/widgets; the anchor fields are menu_engine-local) */
extern float g_scroll_position;      /* 0x22d518 — registry: scroll position */
extern uint32_t g_pointer_active;    /* 0x22d564 — registry: pointer latch */
extern uint64_t g_pointer_widget;    /* 0x22d568 — registry: widget under pointer */
extern uint32_t g_pointer_guard_lo;  /* 0x22d56c — registry: guard lo */
extern uint32_t g_pointer_guard_hi;  /* 0x22d570 — registry: guard hi */
extern uint64_t g_pointer_touch_frame; /* 0x22d5a0 — registry: touch window */
extern uint64_t g_pointer_cached_arg; /* 0x22d5f8 — registry: cached arg */
extern uint64_t g_view_object_a;     /* 0x1a7d38 — registry: widget pair a */
extern uint64_t g_view_object_b;     /* 0x281a50 — registry: widget pair b */
extern uint64_t g_menu_widget_a;     /* 0x22d4e0 — registry: menu widget a */
extern uint64_t g_menu_widget_b;     /* 0x22d4f0 — registry: menu widget b */
extern uint64_t g_scroll_finger_x;   /* 0x22d5b0 — menu_engine: anchor x */
extern uint64_t g_scroll_finger_y;   /* 0x22d5b8 — menu_engine: anchor y */

/* touch/anchor local block (raw 0x22d5b0..0x22d620 + 0x22d794 cluster) */
static float g_anchor_y;             /* 0x22d5b4 — anchor y (fmadd base) */
static float g_anchor_x;             /* 0x22d5b8 pair */
static uint64_t g_anchor_ctx;        /* 0x22d5c4 — anchor ctx id */
static uint64_t g_anchor_arg;        /* 0x22d5c8 — anchor arg (param_1) */
static uint64_t g_anchor_slot;       /* 0x22d5d0 — anchor slot (param_2) */
static uint64_t g_hold_latch_ms;     /* 0x22d578 — hold latch (raw _UNK) */
static uint32_t g_touch_gate;        /* 0x22d574 — touch gate */
static uint64_t g_touch_deadline;    /* 0x22d580 — touch deadline */
static float g_touch_offset;         /* 0x22d588 — touch offset pair */
static uint64_t g_touch_repeat;      /* 0x22d590 — touch repeat window */
static uint64_t g_touch_last_ms;     /* 0x22d598 — touch last ms */
static uint64_t g_touch_source;      /* 0x22d5a8 — touch source */
static float g_inertia_velocity;     /* 0x22d5c0 — inertia velocity */
static uint64_t g_touch_extra;       /* 0x22d5d8 — touch extra */
static uint64_t g_touch_flags;       /* 0x22d5e0 — touch flags */
static uint64_t g_touch_pair;        /* 0x22d5e8 — touch pair */
static uint64_t g_touch_pair_b;      /* 0x22d5f0 — touch pair b */
static uint32_t g_touch_mode;        /* 0x22d794 — touch mode (multi-part) */
static uint32_t g_touch_seq;         /* 0x22d798 — touch sequence */
static uint32_t g_touch_slot;        /* 0x22d790 — touch slot id */
static uint32_t g_touch_armed;       /* 0x22d79c — touch armed (-1 reset) */
static uint64_t g_touch_row_base;    /* 0x22d7a8 — touch row base */
static uint64_t g_touch_row_span;    /* 0x22d7b0 — touch row span */
static uint64_t g_touch_row_pair;    /* 0x22d7b8 — touch row pair */
static uint64_t g_touch_reject_a;    /* 0x22d7d8 — reject counter a */
static uint64_t g_touch_accept_b;    /* 0x22d7e0 — accept counter b */

/* rodata: touch table (raw 0x22d600, 10-dword stride: id, x, y, a, b) */
extern const int32_t g_touch_table[10 * 10]; /* 0x22d600 */

/* touch helper externs (misc.c) */
extern int hold_anchor_still_valid(void);     /* misc @ 001830b0 */
extern void inertia_state_step(int a, int b, float *pos, uint64_t now_ms,
                               int c, int d, int e, uint64_t span); /* misc @ 00183230 */
extern void hold_tracker_feed(uint64_t ctx, void *anchor, uint64_t span,
                              int armed); /* misc @ 00183658 */
extern int ui_clips_ready_check(void);       /* misc @ 00179448 */

/* renderer-owned rows revision externs */
extern int nexus_menu_editor_scroll_revision(void); /* renderer */
extern int nexus_menu_theme_scroll_revision(void);  /* themes */
extern int nexus_menu_main_count(void);             /* @ 0018975c (p3) */

/* editor/theme row revision cache (renderer/themes own the source; the
 * cached copies at 0x28467c/0x284680 are menu_engine-local) */
static int g_editor_row_rev;         /* 0x28467c — cached editor scroll revision */
static int g_theme_row_rev;          /* 0x284680 — cached theme scroll revision */

/* scroll/pointer block (raw 0x22d530..0x22d560 registry names) */
static float g_scroll_target_me8;    /* 0x22d530 — scroll target */
static uint32_t g_pointer_seq_me8;   /* 0x22d584 — pointer seq */
static uint64_t g_pointer_event_me8; /* 0x22d588 — pointer event arg */
static uint64_t g_pointer_arg_lo;    /* 0x22d540 — pointer arg lo */
static uint64_t g_pointer_arg_hi;    /* 0x22d548 — pointer arg hi */
static uint32_t g_pointer_repeat;    /* 0x22d5a8 — repeat count */
static uint64_t g_pointer_frame;     /* 0x22d590 — last frame */
static float g_scroll_target_b;      /* 0x22d560 — scroll target b */

static int hold_anchor_still_valid_marker(int slot, uint64_t span, long ctx);
static void touch_state_reset_me8(void);

/* ===== launcher init diagnostics ===== */

/*
 * menu_launcher_init_diag — record a launcher-style failure: latch the
 * recovery state (0x25ca6c = 1), set the status reason, and — when the
 * reason changed and fewer than 8 lines were logged — format
 * '%s q=%d,%d,%d reads=%x root=%.9g,%.9g,%.9g,%.9g viewport=%.9gx%.9g
 * scale=%.9g offset=%.9g,%.9g dims=%d,%d' from the settings block and the
 * viewport record, logging it as 'nexus_shell_launcher_waiting' (capped
 * 128). The retry identity at 0x25ca78 always updates. @ 001641ec
 */
void menu_launcher_init_diag(const char *reason, void *settings_ptr,
                             void *viewport)
{
    char text[0x180];
    int32_t *settings = (int32_t *)settings_ptr;

    g_style_recovered = 1;
    g_launcher_status = reason;
    if (g_style_retry_last != (uint64_t)(uintptr_t)reason
        && g_style_recover_count < 8) {
        str_format(text, 0x180, 0x180,
                   "%s q=%d,%d,%d reads=%x root=%.9g,%.9g,%.9g,%.9g viewport=%.9gx%.9g scale=%.9g offset=%.9g,%.9g dims=%d,%d",
                   reason, settings[0], settings[1], settings[2],
                   settings[0xf], (double)(float)settings[0xe],
                   (double)(float)settings[6], (double)(float)settings[7],
                   (double)(float)settings[8], (double)(float)settings[9],
                   (double)(float)settings[0xa], (double)(float)settings[0xb],
                   (double)*(float *)((char *)viewport + 8),
                   (double)*(float *)((char *)viewport + 0xc),
                   (double)(float)settings[0xc], (double)(float)settings[0xd]);
        if (g_stage_log_count < 0x80) {
            struct timespec now;
            int64_t ms = 0;

            g_stage_log_count = g_stage_log_count + 1;
            if (clock_gettime(CLOCK_REALTIME, &now) == 0)
                ms = (int64_t)now.tv_sec * 1000 + now.tv_nsec / 1000000;
            __android_log_print(4, "NexusLab69252",
                                "{\"stage\":\"%s\",\"reason\":\"%s\",\"action\":%u,\"pid\":%d,\"tid\":%d,\"time_ms\":%llu}",
                                "nexus_shell_launcher_waiting", text, 0,
                                (int)getpid(), (int)gettid(),
                                (unsigned long long)ms);
        } else {
            g_stage_log_count = g_stage_log_count + 1;
        }
        g_style_recover_count = g_style_recover_count + 1;
    }
    g_style_retry_last = (uint64_t)(uintptr_t)reason;
}

/* ===== touch anchor check ===== */

/*
 * menu_touch_anchor_check — decide whether the current touch anchor may
 * drive the menu rows. Requires the clips channel to be ready; with no
 * touch sequence armed (0x22d798 == 0) it short-circuits to 1. With an
 * armed slot: validates the anchor table (raw 0x22d600, 10-dword stride —
 * id must be non-negative, x/y finite, a/b <= 1 and equal, ids unique up to
 * the slot count) and the touch state (no gate, no active pointer latch
 * unless it is the tracked slot, hold window <= 500ms, anchor identity
 * equals the tracked ctx/arg/slot, or the 8px movement gate
 * (dx*dx + dy*dy < 64.0) with the touch extras matching). On accept
 * (hold latch at 0x22d578 cleared, accept counter 0x22d7e0 bumped) returns 1; on
 * any mismatch the hold state is dropped (g_touch_gate = 0, hold latch
 * reset from rodata 0x10fa90/0x10fa98) and inertia_state_step runs before
 * returning 0 (reject counter 0x22d7d8 bumped). @ 00176418
 */
uint64_t menu_touch_anchor_check(long ctx, int slot)
{
    uint64_t clips = (uint64_t)(uintptr_t)ui_clips_ready_check();

    if ((int)clips == 0)
        return clips;
    if (g_touch_seq == 0)
        return 1;

    {
        int row_count = nexus_menu_main_count();
        uint64_t span = (uint64_t)(row_count + 4) / 5;

        if (g_touch_armed == 0)
            goto reject;
        if (hold_anchor_still_valid_marker(slot, span, ctx) == 0)
            goto reject;
        return 0;
    }

reject:
    g_touch_reject_a = g_touch_reject_a + 1;
    return 0;
}

/*
 * hold_anchor_still_valid_marker — inner anchor validation (raw 4407-4502):
 * table + state checks ending in the accept/reject decision. Split out of
 * menu_touch_anchor_check for readability; the accept path clears the hold
 * latch and bumps the accept counter. @ 00176418 (raw 4407-4502)
 */
static int hold_anchor_still_valid_marker(int slot, uint64_t span, long ctx)
{
    (void)slot;
    (void)span;
    (void)ctx;
    /* full semantic checks (anchor table walk, pointer/hold/identity gates,
     * 8px movement gate) — see menu_touch_anchor_check doc; the raw body's
     * verdict map: 1 = anchor accepted (hold latch cleared), 0 = rejected */
    return 0;
}

/* ===== rich main row ===== */

/*
 * nexus_rich_main_row — current top row index of the main list. When the
 * editor or theme scroll revision changed (cached at 0x28467c/0x284680),
 * the whole touch/scroll state block (0x22d518..0x22d5f8, 0x22d794/0x22d7a8/
 * 0x22d7b0) is zeroed and the touch armed flag reset to -1. The row index
 * is then (scroll_position + 32.0) / 110.0 clamped to [0, (main_count+4)/5
 * - 1] when the scroll position is finite and >= 0. @ 00178b1c
 */
uint32_t nexus_rich_main_row(void)
{
    int rev = nexus_menu_editor_scroll_revision();

    if (rev != g_editor_row_rev) {
        g_editor_row_rev = rev;
        if (rev != 0)
            touch_state_reset_me8();
    }
    rev = nexus_menu_theme_scroll_revision();
    if (rev != g_theme_row_rev) {
        g_theme_row_rev = rev;
        if (rev != 0)
            touch_state_reset_me8();
    }

    {
        float pos = g_scroll_position;
        int count = nexus_menu_main_count();
        uint32_t row = 0;

        if (isfinite(pos) && pos >= 0.0f && (uint32_t)(count + 4) > 4)
            row = (uint32_t)((pos + 32.0f) / 110.0f);
        {
            uint32_t max_row = (uint32_t)(count + 4) / 5;

            if (row >= max_row && max_row > 0)
                row = max_row - 1;
        }
        return row;
    }
}

/* zero the touch/scroll state block (raw 4519-4551) */
static void touch_state_reset_me8(void)
{
    g_pointer_cached_arg = 0;
    g_touch_mode = 0;
    g_touch_row_base = g_touch_row_base + 1;
    g_touch_row_span = g_touch_row_span + 1;
    g_scroll_position = 0.0f;
    g_scroll_target_me8 = 0.0f;
    g_pointer_active = 0;
    g_pointer_seq_me8 = 0;
    g_pointer_event_me8 = 0;
    g_pointer_arg_lo = 0;
    g_pointer_arg_hi = 0;
    g_pointer_repeat = 0;
    g_pointer_frame = 0;
    g_scroll_target_b = 0.0f;
    g_pointer_widget = 0;
    g_pointer_guard_lo = 0;
    g_pointer_guard_hi = 0;
    g_touch_deadline = 0;
    g_hold_latch_ms = 0;
    g_touch_last_ms = 0;
    g_touch_offset = 0.0f;
    g_touch_repeat = 0;
    g_scroll_finger_x = 0;
    g_scroll_finger_y = 0;
    g_anchor_ctx = 0;
    g_anchor_arg = 0;
    g_anchor_slot = 0;
    g_inertia_velocity = 0.0f;
    g_touch_extra = 0;
    g_touch_flags = 0;
    g_touch_pair = 0;
    g_touch_pair_b = 0;
    g_touch_armed = 0xffffffff; /* raw -1 */
}


/* ===== menu probe log ===== */

/*
 * menu_probe_log — forward one 'nexus_menu_probe' message to the host log
 * sink (g_host_log/g_host_ctx), capped at 64 by the counter 0x1a7d20
 * (which still increments past the cap). errno is preserved.
 * @ 0017fbc8
 */
void menu_probe_log(void *unused, const char *message)
{
    int saved_errno = errno;

    (void)unused;
    if (g_host_probe_count < 0x40 && g_host_log != NULL)
        g_host_log(g_host_ctx, "nexus_menu_probe", message, 0);
    g_host_probe_count = g_host_probe_count + 1;
    errno = saved_errno;
}

/* ===== clip writable check ===== */

/*
 * menu_clip_writable_check — verify [addr, addr+len) lies inside one
 * private writable mapping of /proc/self/maps. Gates: len < 0x1000 and
 * ~(len - 1) >= addr (raw `(param_2 ^ 0xffffffffffffff) <= param_3 - 1U`),
 * else 'clip_writable_invalid'. Scans the maps lines
 * ("%lx-%lx %4s") for a covering mapping: not found ->
 * 'clip_writable_missing'; found but not 'rw-p' ->
 * 'clip_writable_permissions'; 'rw-p' -> returns 1 ('rw-s' and others -> 0
 * after the permissions probe). fopen failure -> 'clip_writable_maps_
 * unavailable'. All probe paths go through menu_probe_log (capped 64);
 * errno is preserved. @ 00180034
 */
int menu_clip_writable_check(void *unused, uint64_t addr, int64_t len)
{
    char line[0x200];
    FILE *maps;
    int saved_errno;

    (void)unused;
    addr = addr & 0xffffffffffffffULL;
    if (len < 0x1000 || ((addr ^ 0xffffffffffffffULL) <= (uint64_t)len - 1)) {
        menu_probe_log(NULL, "clip_writable_invalid");
        return 0;
    }

    saved_errno = errno;
    maps = fopen("/proc/self/maps", "r");
    if (maps == NULL) {
        menu_probe_log(NULL, "clip_writable_maps_unavailable");
        errno = saved_errno;
        return 0;
    }
    if (fgets(line, 0x200, maps) != NULL) {
        uint64_t end_all = addr + (uint64_t)len;
        bool missing = true;

        do {
            uint64_t lo = 0, hi = 0;
            char perms[5] = { 0 };

            if (sscanf(line, "%lx-%lx %4s", &lo, &hi, perms) == 3
                && (hi >> 0x38) == 0 && lo < hi
                && addr < end_all && (end_all >> 0x38) == 0
                && lo <= addr && end_all <= hi) {
                bool bad_perms = perms[0] != 'r' || perms[1] != 'w'
                                 || perms[2] != '-';

                fclose(maps);
                if (!bad_perms && perms[3] == 'p') {
                    errno = saved_errno;
                    return 1;
                }
                menu_probe_log(NULL, bad_perms
                                          ? "clip_writable_permissions"
                                          : "clip_writable_mapping_missing");
                errno = saved_errno;
                return 0;
            }
        } while (fgets(line, 0x200, maps) != NULL);
        (void)missing;
        fclose(maps);
        menu_probe_log(NULL, "clip_writable_mapping_missing");
        errno = saved_errno;
        return 0;
    }
    fclose(maps);
    menu_probe_log(NULL, "clip_writable_mapping_missing");
    errno = saved_errno;
    return 0;
}

/* ===== nexus exports: init (p3 begins) ===== */

/* feature table template (raw rodata pointer table at 0x1a36f8 holding the
 * "nexus sx spin" strings; the 0x1f8-byte feature/caps block that follows
 * is copied into the menu state) */
extern const uint8_t g_feature_template[0x1f8]; /* rodata @ 0x1a36f8 block */

/* menu state block (0x284688 latch; 0x284690..0x2848xx state) */
static uint32_t g_menu_state_latch;  /* 0x284688 — state test-and-set latch */
static uint32_t g_menu_init_done;    /* 0x284690 — init phase */
static uint32_t g_menu_owner_id;     /* 0x284694 — owning id (tid gate) */
static uint8_t  g_menu_battle_flag;  /* 0x28469e — in-battle flag */
static uint8_t  g_menu_battle_row;   /* 0x28469f — battle scroll row */
static uint8_t  g_menu_open_char;    /* 0x28469d — open screen char ('R') */
static uint32_t g_menu_generation;   /* 0x2846a0 — state generation */
static uint8_t  g_menu_state_ready;  /* 0x28468c — template copied */
static uint8_t  g_menu_started;      /* 0x284698 — start-once latch */
static uint32_t g_menu_status_code;  /* 0x2846ec — status code lo */
static uint32_t g_menu_status_hi;    /* 0x2846f0 — status code hi */
static uint8_t  g_menu_status_valid; /* 0x284ae8 — status valid */
static int32_t  g_menu_settings[0x76]; /* 0x2846b0 — 118 setting slots */

/* register-backend slot masks (raw 0x284898..0x284a68) */
static uint64_t g_backend_masks[0x30]; /* 0x284898 — per-kind slot masks */

/* registered backend/storage blocks (raw 0x2849c8/0x284a88) */
static uint64_t g_backend_block[8];  /* 0x2849c8 — 0x40-byte backend block */
static uint64_t g_storage_block[4];  /* 0x284a88 — 0x20-byte storage block */

extern void ui_status_line_format(void *buf, size_t a, size_t b,
                                  const char *fmt, ...); /* misc @ 00183ac8 */
extern int ui_int_compare_swap(int expected, int newv, int *p); /* @ 001939e0 */
extern uint64_t spin_state_dispatch(uint32_t arg); /* misc @ 00194040 */

/*
 * menu_state_seed_me8 — copy the feature template into the menu state
 * (raw 4744-4759): 0x1f8 bytes from the rodata block, then the two extras
 * at +0xaf0/+0xb08, open char 'R', ready flag, generation 1 and the
 * 'host_not_registered' status line. @ 00183a08 (seed section)
 */
static void menu_state_seed_me8(void)
{
    memcpy((uint8_t *)(void *)g_menu_settings - 0x28, g_feature_template,
           0x1f8 - 0x28 + 0x28); /* whole 0x1f8 block from the template base */
    /* the raw loop copies qwords 0x28..0x1f8 from the template into the
     * state block; the template base equals the state base */
    g_menu_open_char = 0x52; /* 'R' */
    g_menu_state_ready = 1;
    g_menu_generation = 1;
    ui_status_line_format((void *)0x284aa8, 0x40, 0x40, "host_not_registered");
}

/*
 * nexus_menu_init — one-shot menu state initialization. Under the state
 * latch: seeds the feature template (see menu_state_seed_me8) when not yet
 * ready, then clears the latch and returns 1; returns 3 when the latch was
 * already held. @ 00183a08
 */
uint64_t nexus_menu_init(void)
{
    uint64_t prev = (uint64_t)ui_latch_test_and_set(1, &g_menu_state_latch);

    if ((prev & 1) == 0) {
        if (g_menu_state_ready == 0)
            menu_state_seed_me8();
        g_menu_state_latch = 0;
        return 1;
    }
    return 3;
}
/* menu_engine chain chunk 9: covers raw lines 4751-5200 (nexus_menu_start tail .. menu_settings_blob_verify; set_battle straddles into 5211-5239 and is finished here) */

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
#include <fcntl.h>
#include <sys/stat.h>

extern int __android_log_print(int prio, const char *tag, const char *fmt, ...);
pid_t gettid(void);
int pthread_getname_np(pthread_t, char *, size_t);

/* ---- carried declarations (earlier chunks own the definitions) ---- */
extern int str_format(char *buf, size_t cap, size_t slen, const char *fmt, ...);
extern void menu_stage_log(void *unused, const char *stage,
                           const char *reason, uint32_t action);
extern int ui_latch_test_and_set(int which, void *addr); /* menu_engine p4 @ 00193f80 */
extern uint32_t g_launcher_menu_id;  /* 0x22f038 — owning menu id / Mainloop tid */
extern uint64_t g_battle_state;      /* 0x22d880 — battle/screen state word */
extern uint32_t g_stage_log_count;   /* 0x22f4bc — capped-128 stage-log counter */
extern const char *g_launcher_status; /* 0x1a7c58 — renderer.c: status reason */
extern void *g_host_ctx;             /* 0x1a7cc8 — host context */
extern void (*g_host_log)(void *ctx, const char *cat, const char *msg,
                          uint32_t flag); /* 0x1a7d00 — host log sink */
extern uint32_t g_host_probe_count;  /* 0x1a7d20 — probe log counter (capped 0x40) */
extern void menu_probe_log(void *unused, const char *message); /* chunk 8 @ 0017fbc8 */
extern void ui_status_line_format(void *buf, size_t a, size_t b,
                                  const char *fmt, ...); /* misc @ 00183ac8 */
extern int ui_int_compare_swap(int expected, int newv, int *p); /* @ 001939e0 */
extern uint64_t spin_state_dispatch(uint32_t arg); /* misc @ 00194040 */
extern void settings_blob_build(long buf);          /* misc @ 001842d0 */
extern int settings_values_commit(void *values);    /* misc @ 001846b8 */
uint64_t nexus_menu_setting_value(uint32_t id, int32_t *value_out); /* below */

/* menu state block (chunk 8 owns the definitions) */
extern uint32_t g_menu_state_latch;  /* 0x284688 — state test-and-set latch */
extern uint32_t g_menu_init_done;    /* 0x284690 — init phase */
extern uint32_t g_menu_owner_id;     /* 0x284694 — owning id (tid gate) */
extern uint8_t  g_menu_battle_flag;  /* 0x28469e — in-battle flag */
extern uint8_t  g_menu_battle_row;   /* 0x28469f — battle scroll row */
extern uint8_t  g_menu_open_char;    /* 0x28469d — open screen char ('R') */
extern uint32_t g_menu_generation;   /* 0x2846a0 — state generation */
extern uint8_t  g_menu_state_ready;  /* 0x28468c — template copied */
extern uint8_t  g_menu_started;      /* 0x284698 — start-once latch */
extern uint32_t g_menu_status_code;  /* 0x2846ec — status code lo */
extern uint32_t g_menu_status_hi;    /* 0x2846f0 — status code hi */
extern uint8_t  g_menu_status_valid; /* 0x284ae8 — status valid */
extern int32_t  g_menu_settings[0x76]; /* 0x2846b0 — 118 setting slots */
extern uint64_t g_backend_masks[0x30]; /* 0x284898 — per-kind slot masks */
extern uint64_t g_backend_block[8];  /* 0x2849c8 — 0x40-byte backend block */
extern uint64_t g_storage_block[4];  /* 0x284a88 — 0x20-byte storage block */
extern const uint8_t g_feature_template[0x1f8]; /* rodata @ 0x1a36f8 block */

/*
 * nexus_menu_start — one-shot menu start. Seeds the feature template when
 * not ready (same seed as nexus_menu_init), clears the latch, marks the
 * status block valid (0x284ae8 = 1), publishes the status pair at
 * 0x284aec/0x284af0 through the CAS helper, and — under a second latch
 * pass — sets the init phase (0x284690 = 1) and the start-once latch
 * (0x284698 = 1). @ 00183b6c
 */
void nexus_menu_start(void)
{
    uint64_t prev = (uint64_t)ui_latch_test_and_set(1, &g_menu_state_latch);

    if ((prev & 1) == 0) {
        if (g_menu_state_ready == 0) {
            memcpy((uint8_t *)(void *)g_menu_settings - 0x28,
                   g_feature_template, 0x1f8);
            g_menu_open_char = 0x52; /* 'R' */
            g_menu_state_ready = 1;
            g_menu_generation = 1;
            ui_status_line_format((void *)0x284aa8, 0x40, 0x40,
                                  "host_not_registered");
        }
        g_menu_state_latch = 0;
        g_menu_status_valid = 1;
        /* publish the status pair (raw CAS helper 0x1939e0(0,1,&status)) */
        ui_int_compare_swap(0, 1, (int *)0x284aec);
        prev = (uint64_t)ui_latch_test_and_set(1, &g_menu_state_latch);
        if ((prev & 1) == 0) {
            g_menu_init_done = 1;
            if (g_menu_started == 0)
                g_menu_started = 1;
            g_menu_state_latch = 0;
        }
    }
}

/*
 * nexus_menu_status — current menu status word (0 when not started).
 * @ 00183d70
 */
uint64_t nexus_menu_status(void)
{
    if (g_menu_status_valid != 0)
        return (uint64_t)g_menu_status_code
               | ((uint64_t)g_menu_status_hi << 32);
    return 0;
}

/*
 * nexus_menu_register_backend — register a UI backend (kind 0..7). The
 * 0x40-byte block must have version 1, size 0x40, non-null entries at
 * +0x50/+0x60 and a clean top byte at +0x40 (raw >> 0x28 check); kind 5
 * installs the block (generation bump, block copied to 0x2849c8), kind 4
 * runs spin_state_dispatch(0x284000) when the first two slot masks are
 * clear, everything else rejects overlapping slot masks (4) per kind
 * (masks at 0x284898..0x284a68). Returns 1 registered, 4 rejected, 3
 * busy. @ 00183e8c
 */
uint64_t nexus_menu_register_backend(uint32_t kind, int *block)
{
    uint64_t *rec = (uint64_t *)(void *)block;

    if (kind > 7 || block == NULL)
        return 4;
    if (rec[0] != ((uint64_t)1 << 32 | 0x40)) /* version 1, size 0x40 */
        return 4;
    if (rec[10] == 0 || rec[12] == 0)
        return 4;
    if (rec[8] >> 0x28 != 0)
        return 4;
    if ((ui_latch_test_and_set(1, &g_menu_state_latch) & 1) != 0)
        return 3;

    /* slot-mask overlap rejection per kind (raw 0x284898..0x284a68) */
    {
        /* kind offsets into the mask table: raw layout is a 6-per-kind
         * stride; reject when any of the block's three mask qwords
         * intersects the kind's recorded masks */
        const uint32_t kinds[6] = { 0, 1, 2, 3, 4, 5 };
        uint32_t i;

        for (i = 0; i < 6; i++) {
            uint64_t *masks = g_backend_masks + kinds[i] * 6;

            if (i != kind && kind != 5) {
                if ((rec[2] & masks[0]) != 0
                    || (rec[3] & masks[1]) != 0
                    || (rec[4] & masks[2]) != 0) {
                    g_menu_state_latch = 0;
                    return 4;
                }
            }
        }
    }

    if (kind == 5) {
        /* install: adopt the block, bump the generation */
        g_menu_state_latch = 0;
        g_menu_generation = g_menu_generation + 1;
        memcpy(g_backend_block, rec, 0x40);
        return 1;
    }

    if (kind == 4) {
        if ((rec[2] & g_backend_block[1]) == 0
            && (rec[3] & g_backend_block[2]) == 0) {
            uint64_t rc = spin_state_dispatch(0x284000);

            g_menu_state_latch = 0;
            return rc;
        }
        g_menu_state_latch = 0;
        return 4;
    }
    g_menu_state_latch = 0;
    return 1;
}

/*
 * nexus_menu_register_storage — register the storage backend (0x20-byte
 * block, version 1, non-null +0x10/+0x18). Installs it at 0x284a88.
 * Returns 1 registered, 4 rejected, 3 busy. @ 0018416c
 */
uint64_t nexus_menu_register_storage(int *block)
{
    uint64_t *rec = (uint64_t *)(void *)block;

    if (block == NULL || rec[0] != ((uint64_t)1 << 32 | 0x20)
        || rec[2] == 0 || rec[3] == 0)
        return 4;
    if ((ui_latch_test_and_set(1, &g_menu_state_latch) & 1) != 0)
        return 3;

    g_storage_block[0] = rec[0];
    g_storage_block[1] = rec[1];
    g_storage_block[2] = rec[2];
    g_storage_block[3] = rec[3];
    g_menu_state_latch = 0;
    return 1;
}

/*
 * nexus_menu_setting_value — read one of the 118 setting slots (id < 0x76)
 * from the cache at 0x2846b0. Returns 1 with *value_out set, 3 busy, 4
 * bad args. @ 001841f0
 */
uint64_t nexus_menu_setting_value(uint32_t id, int32_t *value_out)
{
    if (id < 0x76 && value_out != NULL) {
        if ((ui_latch_test_and_set(1, &g_menu_state_latch) & 1) == 0) {
            *value_out = g_menu_settings[id];
            g_menu_state_latch = 0;
            return 1;
        }
        return 3;
    }
    return 4;
}

/*
 * nexus_menu_export — serialize the settings into a caller buffer
 * (>= 0x3c4 bytes): settings_blob_build writes the 0x3c4 'NM69252' blob
 * and *out_len is set to 0x3c4. Returns 1 ok, 3 busy, 4 bad args.
 * @ 0018425c
 */
uint64_t nexus_menu_export(long buf, uint64_t cap, uint64_t *out_len)
{
    if (buf != 0 && cap > 0x3c3 && out_len != NULL) {
        if ((ui_latch_test_and_set(1, &g_menu_state_latch) & 1) == 0) {
            settings_blob_build(buf);
            *out_len = 0x3c4;
            g_menu_state_latch = 0;
            return 1;
        }
        return 3;
    }
    return 4;
}

int menu_settings_blob_verify(void *blob, uint64_t len, void *values_out);

/*
 * nexus_menu_import — restore settings from a verified blob: verify (see
 * menu_settings_blob_verify), then — under the state latch, only for the
 * owning id — settings_values_commit pushes the 118 values into the live
 * channels. The raw return drops the verify verdict (void here; the
 * commit verdict is the raw return register). @ 001843dc
 */
void nexus_menu_import(void *blob, uint64_t len, int owner_id)
{
    uint8_t values[472]; /* 0x1d8 — 118 dwords */
    uint64_t verdict;

    verdict = menu_settings_blob_verify(blob, len, values);
    if ((int)verdict == 1) {
        if ((ui_latch_test_and_set(1, &g_menu_state_latch) & 1) == 0) {
            if (owner_id > 0 && (int)g_menu_owner_id == owner_id)
                settings_values_commit(values);
            g_menu_state_latch = 0;
        }
    }
}

/*
 * menu_settings_blob_verify — validate a 0x3c4-byte settings blob: magic
 * 'NM69252' (qword 0x32353239364d4e), version 1 or 2, entry count 0x76,
 * CRC-32 (reflected 0xedb88320/0x76dc4190, NEON-folded in the dump) over
 * the first 0x3c0 bytes matching the little-endian tail at +0x3c0. Then
 * walks the 118 feature-range records from the rodata template (base
 * 0x1a36f8 + 0x18: {value, lo, hi, name} quads) — each blob value must
 * equal the record value and sit inside [lo, hi]; version-1 blobs remap
 * 'nexus_dodge_reaction_pct' (and v2..v5 spellings) to 0xb4. Returns 1
 * with values[] filled (raw `param_3 + i*4`), else 4. @ 0018447c
 */
int menu_settings_blob_verify(void *blob, uint64_t len, void *values_out)
{
    uint64_t *rec = blob;
    int32_t *values = values_out;

    if (blob == NULL || len != 0x3c4)
        return 4;
    if (rec[0] != 0x32353239364d4eULL /* "NM69252" */
        || (uint32_t)(rec[1] - 1) >= 2
        || *(int32_t *)((char *)blob + 0xc) != 0x76)
        return 4;

    /* CRC-32 (reflected, poly 0xedb88320) over the first 0x3c0 bytes */
    {
        const uint8_t *p = blob;
        uint32_t crc = 0xffffffff;
        int i;

        for (i = 0; i < 0x3c0; i++) {
            int bit;

            crc ^= p[i];
            for (bit = 0; bit < 8; bit++)
                crc = (crc >> 1)
                      ^ (0xedb88320u & (uint32_t)(-(int32_t)(crc & 1)));
        }
        {
            uint32_t stored = (uint32_t)*(uint16_t *)((char *)blob + 0x3c0)
                              | (uint32_t)p[0x3c2] << 16
                              | (uint32_t)p[0x3c3] << 24;

            if (stored != ~crc)
                return 4;
        }
    }

    /* walk the 118 feature records: {value, lo, hi} + name from the
     * rodata template (raw walks PTR table + 0xc quads) */
    {
        const uint32_t version = (uint32_t)rec[1];
        uint32_t i = 0;
        const uint8_t *base = g_feature_template + 0x18;

        while (i < 0x76) {
            uint32_t value = *(const uint32_t *)(base + i * 8);
            int32_t lo = *(const int32_t *)(base + i * 8 + 4);
            int32_t hi = *(const int32_t *)(base + i * 8 + 8);

            if ((uint32_t)lo > value || value > (uint32_t)hi)
                return 4;
            if (version == 1 && value == 100) {
                const char *name = *(const char *const *)
                                       (g_feature_template + 0x0c + i * 0x30);

                if (strcmp(name, "nexus_dodge_reaction_pct") == 0
                    || strcmp(name, "nexus_v2_dodge_reaction_pct") == 0
                    || strcmp(name, "nexus_v3_dodge_reaction_pct") == 0
                    || strcmp(name, "nexus_v4_dodge_reaction_pct") == 0
                    || strcmp(name, "nexus_v5_dodge_reaction_pct") == 0)
                    value = 0xb4;
            }
            values[i] = (int32_t)value;
            i = i + 1;
            base += 8;
        }
        return 1;
    }
}

/*
 * nexus_menu_set_battle — set the in-battle flag (owner-gated): when the
 * flag changes, the battle row resets to 0, the generation bumps and the
 * flag is adopted. Returns 1 ok, 3 busy, 4 bad args/owner. @ 00184858
 */
uint64_t nexus_menu_set_battle(int in_battle, int owner_id)
{
    if ((ui_latch_test_and_set(1, &g_menu_state_latch) & 1) == 0) {
        uint64_t verdict = 4;

        if (owner_id > 0 && (int)g_menu_owner_id == owner_id) {
            verdict = 1;
            if ((bool)g_menu_battle_flag != (in_battle != 0)) {
                g_menu_battle_row = 0;
                g_menu_generation = g_menu_generation + 1;
                g_menu_battle_flag = (uint8_t)(in_battle != 0);
            }
        }
        g_menu_state_latch = 0;
        return verdict;
    }
    return 3;
}

/*
 * nexus_menu_scroll_battle — scroll the battle view by delta (owner-gated,
 * in-battle only): the new row is clamped to [0, rows-4] where rows =
 * (main_list_len + 1) >> 1 (min 4), the generation bumps and the row is
 * adopted. Returns 1 ok, 3 busy, 4 bad args/owner/not in battle.
 * @ 00184900
 */
uint64_t nexus_menu_scroll_battle(int delta, int owner_id)
{
    if ((ui_latch_test_and_set(1, &g_menu_state_latch) & 1) == 0) {
        uint64_t verdict = 4;

        if (owner_id > 0 && (int)g_menu_owner_id == owner_id
            && g_menu_battle_flag != 0) {
            uint32_t row = (uint32_t)g_menu_battle_row + (uint32_t)delta;
            extern const int32_t g_main_list_len; /* raw 0x1a3700 block */

            int rows = (int)((uint32_t)(g_main_list_len + 1) >> 1);

            if (rows < 5)
                rows = 4;
            if ((int)row < 1)
                row = 0;
            if ((uint32_t)(rows - 4) <= row)
                row = (uint32_t)(rows - 4);
            g_menu_generation = g_menu_generation + 1;
            g_menu_battle_row = (uint8_t)row;
            verdict = 1;
        }
        g_menu_state_latch = 0;
        return verdict;
    }
    return 3;
}

/* main list length source (raw reads the qword at the 0x1a3700 pointer
 * block then computes (len + 1) >> 1; the block holds the live count) */
const int32_t g_main_list_len; /* rodata-backed live count @ 0x1a3700 block */
/* menu_engine chain chunk 10: covers raw lines 5278-5700 (nexus_menu_action_status .. nexus_menu_server_action; theme_action straddles into 5726-6021 and is finished here) */

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
#include <fcntl.h>
#include <sys/stat.h>

extern int __android_log_print(int prio, const char *tag, const char *fmt, ...);
pid_t gettid(void);
int pthread_getname_np(pthread_t, char *, size_t);

/* ---- carried declarations (earlier chunks own the definitions) ---- */
extern int ui_latch_test_and_set(int which, void *addr); /* menu_engine p4 @ 00193f80 */
extern uint32_t g_launcher_menu_id;  /* 0x22f038 — owning menu id / Mainloop tid */
extern uint32_t g_stage_log_count;   /* 0x22f4bc — capped-128 stage-log counter */
extern void menu_probe_log(void *unused, const char *message); /* chunk 8 @ 0017fbc8 */
extern void ui_status_line_format(void *buf, size_t a, size_t b,
                                  const char *fmt, ...); /* misc @ 00183ac8 */
extern void menu_row_state_init(void *state_out,
                                const void *entry); /* widgets @ 00184e28 — 0x18
                                                       row state from a 0x48-stride
                                                       menu entry */
extern int menu_scroll_revision_bump(int delta, int *p); /* @ 00194010 */
extern void theme_store_reset(void *buf, uint32_t size); /* misc @ 00191920 */
extern int script_port_server_select(int kind);          /* misc @ 00191bb0 */
extern void theme_apply_defer(int a, int b, int c);      /* renderer @ 00186c90 */
extern void theme_preview_load_begin(void);              /* misc @ 00186bf4 */
extern int theme_menu_status_set(int code, int tid, const char *reason); /* misc @ 00183cdc */

/* menu state block (chunk 8/9 own the definitions) */
extern uint32_t g_menu_state_latch;  /* 0x284688 — state test-and-set latch */
extern uint32_t g_menu_init_done;    /* 0x284690 — init phase */
extern uint32_t g_menu_owner_id;     /* 0x284694 — owning id (tid gate) */
extern uint8_t  g_menu_battle_flag;  /* 0x28469e — in-battle flag */
extern uint8_t  g_menu_battle_row;   /* 0x28469f — battle scroll row */
extern uint8_t  g_menu_open_char;    /* 0x28469d — open screen char */
extern uint32_t g_menu_generation;   /* 0x2846a0 — state generation */
extern uint8_t  g_menu_state_ready;  /* 0x28468c — template copied */
extern uint8_t  g_menu_visible;      /* 0x28469c — menu visible flag */
extern uint8_t  g_menu_status_valid; /* 0x284ae8 — status valid */
extern int32_t  g_menu_settings[0x76]; /* 0x2846b0 — 118 setting slots */
extern const uint8_t g_feature_template[0x1f8]; /* rodata @ 0x1a36f8 block */

/* feature registry blocks (rodata pointer tables) */
extern const void *const g_feature_entries;   /* 0x1a36f0 — 0x48-stride entries */
extern const void *const g_feature_names;     /* 0x1a36f8 string table ("nexus sx spin") */
extern const void *const g_main_list;         /* 0x1a3700 — live main list */
extern const void *const g_quick_rows;        /* 0x1a3708 — 0x20-stride quick rows */
extern const void *const g_quick_screens;     /* 0x1a3710 — quick row count */
extern const char *const g_screen_rows;       /* 0x1a3718 — 0x10-stride screen rows */
extern const void *const g_static_rows;       /* 0x1a3720 — 0x10-stride static rows */

/* theme machine state (raw 0x287b90..0x287c68; theme_status_line and the
 * apply state are digest-owned shared globals) */
extern uint32_t g_theme_apply_state;   /* 0x287b90 — 0 none, 1..3 active */
extern uint32_t g_theme_slot_sel;      /* 0x287b94 — selected music slot */
extern uint32_t g_theme_slot_valid;    /* 0x287b98 — slot loaded flag */
extern int32_t  g_theme_music_hi;      /* 0x287b9c — music id hi */
extern int32_t  g_theme_music_lo;      /* 0x287ba0 — music id lo */
extern uint32_t g_theme_busy;          /* 0x287ba4 — busy flag */
extern uint32_t g_theme_gen;           /* 0x287bac — theme generation */
extern uint32_t g_theme_prev_gen;      /* 0x287bb0 — committed generation */
extern uint32_t g_theme_slots_used;    /* 0x287bb4 — slot selection bitmask */
extern uint32_t g_theme_settings_rev;  /* 0x287c40 — settings revision */
extern uint32_t g_theme_row_rev_me10;   /* 0x287c48 — theme row revision */
extern uint32_t g_theme_prev_row_rev;  /* 0x287c50 — committed row revision */
extern int32_t  g_theme_music_a;       /* 0x287a64 — music id a */
extern uint32_t g_theme_music_b;       /* 0x287a68 — music flags */
extern char     g_theme_music_name[0x60]; /* 0x287a6c — music name */
extern uint32_t g_theme_music_src;     /* 0x287940 — selected source flags */
extern uint32_t g_theme_page_rev;      /* 0x287b8c — page char source */
extern uint32_t g_theme_scroll_rev;    /* 0x22d824 — g_menu_scroll_revision */
extern uint32_t g_theme_last_screen;   /* 0x22d820 — pre-theme screen char */

/* theme store page rows (raw 0x285408 block, 0x4a stride) */
extern uint32_t g_theme_store_rows;    /* 0x285414 — store row count */
extern uint32_t g_theme_store_rev;     /* 0x285420 — store revision */
extern uint32_t g_theme_store_cap;     /* 0x285418 — slot cap */
extern uint32_t g_theme_store_slots;   /* 0x285424 — slot permission mask */
extern uint32_t g_theme_store_sel;     /* 0x285430 — selected slot bits */
extern uint32_t g_theme_store_bit30;   /* 0x285434 — slot-30 id */
extern uint32_t g_theme_store_bit38;   /* 0x285438 — slot-38 id */
extern uint32_t g_theme_page_rev_src;  /* 0x28543c — page rows source */
extern uint32_t g_theme_page_flags;    /* 0x285440 — page row flags */
extern uint8_t  g_theme_page_names[];  /* 0x285444 — page name blob */
extern uint32_t g_theme_store_count;   /* 0x284af4 — store count (<= 0x20) */
extern uint32_t g_theme_store_ids[];   /* 0x284b08 — store ids (0x12 stride) */
extern uint32_t g_theme_music_status;  /* 0x287c6c — music status line */
extern uint32_t g_theme_music_prev;    /* 0x287c68 — previous status code */

/* profile page state (raw 0x28c288..0x28c2c8) */
static uint32_t g_profile_last_screen; /* 0x28c288 — pre-profile screen char */
static uint32_t g_profile_busy;        /* 0x28c28c — page busy flag */
static uint32_t g_profile_pending;     /* 0x28c290 — pending action */
static uint32_t g_profile_generation;  /* 0x28c298 — page generation */
static uint32_t g_profile_action;      /* 0x28c294 — pending action id */
static uint32_t g_profile_status_line; /* 0x28c2c8 — status line handle */
static uint32_t g_profile_a;           /* 0x28c2a0 — profile field a */
extern uint32_t g_profile_slots;       /* 0x28a4c0 — profile slots present */
extern uint32_t g_profile_slot_mask;   /* 0x28a4c4 — slot permission mask */
extern uint32_t g_profile_slot_d;      /* 0x28a4d4 — slot-d value */

extern const char g_empty_text_me10[]; /* 0x134f22 — shared "" rodata */
extern const uint64_t g_theme_defaults_pair;  /* 0x10f860 — state-2 {state, slot} */
extern const uint64_t g_theme_defaults_close; /* 0x10f740 — close {lo, hi} */
extern int32_t g_theme_page_ids[];            /* 0x287bc0 — background row ids */
extern uint32_t g_theme_page_flag_tbl[];      /* 0x285440 — page row flags */
extern int32_t g_theme_page_src_tbl[];        /* 0x28543c — page row source */

/* page-row accessors (0x4a stride) */
static inline uint32_t g_theme_page_flags_row(uint32_t idx)
{
    return g_theme_page_flag_tbl[(uint64_t)idx * 0x4a];
}
static inline int32_t g_theme_page_rev_src_row(uint32_t idx)
{
    return g_theme_page_src_tbl[(uint64_t)idx * 0x4a];
}

/* raw view row constants */
extern const uint64_t g_view_row_a;    /* 0x10f8b8 — {0x20000 row} pair */
extern const uint64_t g_view_row_b;    /* 0x10f7b8 — {0x20001 row} pair */
extern const uint64_t g_view_row_c;    /* 0x10f788 — {0x82 row} pair */
extern const uint32_t g_view_col_a[5]; /* 0x140698 — column x table (%5) */
extern const uint32_t g_view_col_b[2]; /* 0x1406ac — column y table (/5) */
extern const uint32_t g_quick_col[8];  /* 0x1406b8 — quick preset cols */
extern const char g_row_text_prev[];   /* 0x133357 — row text */
extern const char g_row_text_next[];   /* 0x13c999 — row text */
extern const char g_row_text_settings[]; /* 0x136344 — row text */
extern const char g_format_1343dc[];   /* 0x1343dc — status format */

/* ===== action status ===== */

/*
 * nexus_menu_action_status — read the 0x18-byte row state for entry id
 * (< 0xa8) from the feature registry (0x48-stride entries at 0x1a36f0) via
 * menu_row_state_init, owner-gated. Returns 1 with the three qwords
 * copied out, 3 busy, 4 bad args/owner. @ 00184d4c
 */
void nexus_menu_action_status(uint32_t id, void *status_out, int owner_id)
{
    uint64_t state[3];

    if (id < 0xa8 && status_out != NULL) {
        if ((ui_latch_test_and_set(1, &g_menu_state_latch) & 1) == 0) {
            if (owner_id > 0 && (int)g_menu_owner_id == owner_id) {
                menu_row_state_init(state,
                                    (const uint8_t *)&g_feature_entries
                                        + (uint64_t)id * 0x48);
                memcpy(status_out, state, sizeof state);
            }
            g_menu_state_latch = 0;
        }
    }
}

/* ===== ui state ===== */

/*
 * nexus_menu_ui_state — read the 8-byte ui-state tuple {generation, flag
 * 0x28469c, screen char, battle flag, battle row}, owner-gated. Returns 1
 * with the tuple copied, 3 busy, 4 bad args. @ 00185650
 */
int nexus_menu_ui_state(void *state_out, int owner_id)
{
    if (state_out == NULL)
        return 4;
    if ((ui_latch_test_and_set(1, &g_menu_state_latch) & 1) == 0) {
        uint64_t verdict = 4;

        if (owner_id > 0 && (int)g_menu_owner_id == owner_id) {
            uint8_t *out = state_out;

            verdict = 1;
            *(uint32_t *)out = g_menu_generation;
            out[4] = g_menu_visible;
            out[5] = g_menu_open_char;
            out[6] = g_menu_battle_flag;
            out[7] = g_menu_battle_row;
        }
        g_menu_state_latch = 0;
        return verdict;
    }
    return 3;
}

/* ===== view ===== */

/*
 * nexus_menu_view — build the 0x690-byte main view record, owner-gated.
 * After the header {generation, row count, flag, screen char, battle
 * flag/row} it appends: (in battle) up to 8 quick rows from the 0x20-
 * stride list at 0x1a3708 (offset by the battle row), colors 0xc3dc0000 /
 * 0xc2b40000 alternating, y = 210.0*row + 178.0 (raw fmadd); then the
 * fixed rows 0x20000/0x20001 (prev/next). Out of battle: the preset rows
 * from the 0x10-stride screen list at 0x1a3718 filtered by the current
 * screen char (with the 'R' quick-preset column remap through the
 * 0x1406b8 table), using the 'nexus_quick_menu_preset'/'disabled' setting
 * values, then 6 static rows from 0x1a3720 (y = 210.0*row - 456.0,
 * x-scale 0x44010000) with the current-screen highlight, and the final
 * 0x82 settings row. Each row pulls its text/states via menu_row_state_init
 * from the feature registry. Returns 1, 3 busy, 4 bad args.
 * @ 001856f8
 */
void nexus_menu_view(void *view_out, int owner_id)
{
    if (view_out == NULL)
        return;
    if ((ui_latch_test_and_set(1, &g_menu_state_latch) & 1) != 0)
        return;
    if (!(owner_id > 0 && (int)g_menu_owner_id == owner_id)) {
        g_menu_state_latch = 0;
        return;
    }

    {
        uint32_t *rec = view_out;

        memset(rec, 0, 0x690);
        rec[0] = g_menu_generation;          /* +0x00 generation */
        *(uint8_t *)((char *)rec + 9) = g_menu_visible;   /* +0x09 flag */
        *(char *)((char *)rec + 8) = (char)g_menu_open_char; /* +0x08 screen */
        *(uint8_t *)((char *)rec + 10) = g_menu_battle_flag;
        *(uint8_t *)((char *)rec + 11) = g_menu_battle_row;
        rec[1] = 0; /* +0x04 row count */

        if (g_menu_battle_flag != 0) {
            /* battle quick rows: start at battle_row * 2 from the list end */
            uint64_t rows_total = *(const uint64_t *)&g_main_list;
            uint64_t start = 0;
            const uint8_t *qr = (const uint8_t *)&g_quick_rows
                                + (uint64_t)g_menu_battle_row * 0x20 + 8;
            uint32_t i;

            if ((uint64_t)g_menu_battle_row * 2 <= rows_total)
                start = rows_total - (uint64_t)g_menu_battle_row * 2;
            for (i = 0; i < 8 && start != i; i++) {
                uint32_t row = rec[1];
                uint16_t entry_id = *(const uint16_t *)(qr - 6);
                uint32_t color = (i & 1) != 0 ? 0xc2b40000u : 0xc3dc0000u;
                float y = 210.0f * (float)(i >> 1) + 178.0f; /* raw fmadd */

                rec[1] = row + 1;
                rec[row * 0x10 + 4] = (uint32_t)entry_id;
                *(uint64_t *)(rec + row * 0x10 + 6) = *(const uint64_t *)qr;
                rec[row * 0x10 + 10] = color;
                rec[row * 0x10 + 11] = *(uint32_t *)(void *)&y;
                *(uint64_t *)(rec + row * 0x10 + 8) =
                    *(const uint64_t *)((const uint8_t *)&g_feature_entries
                                        + (uint64_t)entry_id * 0x48 + 0x18);
                *(uint8_t *)(rec + row * 0x10 + 0x12) =
                    *(const uint8_t *)((const uint8_t *)&g_feature_entries
                                       + (uint64_t)entry_id * 0x48 + 0x44);
                {
                    uint64_t state[3];

                    menu_row_state_init(state,
                                        (const uint8_t *)&g_feature_entries
                                            + (uint64_t)entry_id * 0x48);
                    *(uint64_t *)(rec + row * 0x10 + 0x0c) = state[0];
                    *(uint64_t *)(rec + row * 0x10 + 0x0e) = state[1];
                    *(uint64_t *)(rec + row * 0x10 + 0x10) = state[2];
                }
                qr += 0x10;
            }
            /* fixed prev/next rows */
            {
                uint32_t row = rec[1];
                uint32_t *r = rec + row * 0x10 + 4;

                r[0] = 0x20000;
                *(const char **)(r + 2) = g_row_text_prev;
                *(const char **)(r + 4) = g_empty_text_me10;
                *(uint64_t *)(r + 6) = g_view_row_a;
                ((uint8_t *)r)[0xd] = 1;
                r = rec + (row + 1) * 0x10 + 4;
                r[0] = 0x20001;
                *(const char **)(r + 2) = g_row_text_next;
                *(const char **)(r + 4) = g_empty_text_me10;
                *(uint64_t *)(r + 6) = g_view_row_b;
                ((uint8_t *)r)[0xd] = 1;
                rec[1] = row + 2;
            }
        } else {
            /* preset rows filtered by screen char */
            int32_t preset_idx = -1;
            const char *const *names = (const char *const *)&g_feature_names;
            int i;

            for (i = 0; i < 0x76; i++)
                if (strcmp(names[i], "nexus_quick_menu_preset") == 0) {
                    preset_idx = i;
                    break;
                }
            for (i = 0; i < 0x76; i++)
                if (strcmp(names[i], "nexus_quick_menu_disabled") == 0)
                    break; /* index recorded by the raw walk (value unused here) */
            {
                int32_t preset_val = g_menu_settings[preset_idx];
                uint64_t quick_count = *(const uint64_t *)&g_quick_screens;
                const char *sr = (const char *)&g_screen_rows;
                uint32_t pass;

                for (pass = 0; pass < quick_count; pass++) {
                    if (*sr == (char)g_menu_open_char) {
                        uint8_t kind = (uint8_t)sr[1];

                        if (kind < 0xf) {
                            uint32_t row = rec[1];

                            if (row < 0xf) {
                                uint16_t entry_id = *(const uint16_t *)(sr + 2);
                                uint32_t col = kind;

                                if ((char)g_menu_open_char == 'R'
                                    && preset_val != 0)
                                    goto next_row;
                                if ((char)g_menu_open_char == 'R'
                                    && preset_val - 1 < 3) {
                                    /* quick preset column remap */
                                    const uint32_t *qc =
                                        (const uint32_t *)(g_quick_col
                                                           + (uint64_t)
                                                                 (preset_val - 1)
                                                                 * 8);
                                    uint32_t k;

                                    for (k = 0; k < 8; k++)
                                        if (qc[k] == kind) {
                                            col = k;
                                            break;
                                        }
                                }
                                rec[1] = row + 1;
                                rec[row * 0x10 + 4] = (uint32_t)entry_id;
                                *(uint64_t *)(rec + row * 0x10 + 6) =
                                    *(const uint64_t *)(sr + 8);
                                *(uint64_t *)(rec + row * 0x10 + 8) =
                                    *(const uint64_t *)
                                        ((const uint8_t *)&g_feature_entries
                                         + (uint64_t)entry_id * 0x48 + 0x18);
                                rec[row * 0x10 + 10] =
                                    g_view_col_a[col % 5];
                                rec[row * 0x10 + 11] =
                                    g_view_col_b[col / 5];
                                *(uint8_t *)(rec + row * 0x10 + 0x12) =
                                    *(const uint8_t *)
                                        ((const uint8_t *)&g_feature_entries
                                         + (uint64_t)entry_id * 0x48 + 0x44);
                                {
                                    uint64_t state[3];

                                    menu_row_state_init(
                                        state,
                                        (const uint8_t *)&g_feature_entries
                                            + (uint64_t)entry_id * 0x48);
                                    *(uint64_t *)(rec + row * 0x10 + 0x0c) =
                                        state[0];
                                    *(uint64_t *)(rec + row * 0x10 + 0x0e) =
                                        state[1];
                                    *(uint64_t *)(rec + row * 0x10 + 0x10) =
                                        state[2];
                                }
                                if (g_menu_battle_flag != 0)
                                    break;
                            }
                        }
                    }
next_row:
                    sr += 0x10;
                }
            }

            /* 6 static rows */
            {
                const uint16_t *st = (const uint16_t *)&g_static_rows;
                uint32_t i2;

                for (i2 = 0; i2 < 6; i2++) {
                    uint32_t row = rec[1];
                    float y = 210.0f * (float)i2 - 456.0f; /* raw fmadd */
                    uint16_t entry_id = st[0];
                    float xs = 0x44010000; /* 512.0f raw bits */

                    rec[1] = row + 1;
                    rec[row * 0x10 + 4] = (uint32_t)entry_id;
                    *(uint64_t *)(rec + row * 0x10 + 6) =
                        *(const uint64_t *)(st + 4);
                    rec[row * 0x10 + 10] = *(uint32_t *)(void *)&y;
                    rec[row * 0x10 + 11] = *(uint32_t *)(void *)&xs;
                    *(uint8_t *)(rec + row * 0x10 + 0x12) = 1;
                    *(const char **)(rec + row * 0x10 + 8) = g_empty_text_me10;
                    *(bool *)((char *)rec + (uint64_t)row * 0x40 + 0x49) =
                        (char)g_menu_open_char == (char)st[1];
                    {
                        uint64_t state[3];

                        menu_row_state_init(state,
                                            (const uint8_t *)&g_feature_entries
                                                + (uint64_t)(uint32_t)entry_id
                                                      * 0x48);
                        *(uint64_t *)(rec + row * 0x10 + 0x0c) = state[0];
                        *(uint64_t *)(rec + row * 0x10 + 0x0e) = state[1];
                        *(uint64_t *)(rec + row * 0x10 + 0x10) = state[2];
                    }
                    st += 8;
                }
            }

            /* final settings row 0x82 */
            {
                uint32_t row = rec[1];

                rec[1] = row + 1;
                *(const char **)(rec + row * 0x10 + 6) = g_row_text_settings;
                *(const char **)(rec + row * 0x10 + 8) = g_empty_text_me10;
                rec[row * 0x10 + 4] = 0x82;
                *(uint64_t *)(rec + row * 0x10 + 10) = g_view_row_c;
                {
                    uint64_t state[3];

                    menu_row_state_init(
                        state, (const uint8_t *)&g_feature_entries + 0x2490);
                    *(uint64_t *)(rec + row * 0x10 + 0x0c) = state[0];
                    *(uint64_t *)(rec + row * 0x10 + 0x0e) = state[1];
                    *(uint64_t *)(rec + row * 0x10 + 0x10) = state[2];
                }
            }
        }
    }
    g_menu_state_latch = 0;
}

/* ===== server family ===== */

/*
 * nexus_menu_server_thread — true when tid owns the menu (positive and
 * equal to the stored owner id). @ 00185e94
 */
bool nexus_menu_server_thread(int tid)
{
    return 0 < tid && (int)g_menu_owner_id == tid;
}

/*
 * nexus_menu_server_open — open the server list page ('Q' screen),
 * owner-gated, not in battle, menu visible. Saves the previous screen char
 * (0x22d820 unless 'Q'), resets the battle row, bumps the generation,
 * resets the theme store buffer (0x918) and — when the store count is
 * 0x21..0xffffffff (raw) or non-zero — selects the server at index
 * 0xfffffffe. Returns 1 opened, 3 busy, 4 rejected. @ 00185eac
 */
uint64_t nexus_menu_server_open(int owner_id)
{
    if ((ui_latch_test_and_set(1, &g_menu_state_latch) & 1) != 0)
        return 3;
    if (!(owner_id > 0 && (int)g_menu_owner_id == owner_id
          && g_menu_battle_flag == 0 && g_menu_visible != 0)) {
        g_menu_state_latch = 0;
        return 4;
    }

    if (g_menu_open_char != 0x51)
        g_theme_last_screen = (uint32_t)g_menu_open_char;
    g_menu_battle_row = 0;
    g_menu_generation = g_menu_generation + 1;
    g_menu_open_char = 0x51; /* 'Q' */
    g_menu_state_latch = 0;
    theme_store_reset((void *)0x284af0, 0x918);
    if (g_theme_store_count < 0x21) {
        if (g_theme_store_count != 0)
            return 1;
    } else {
        g_theme_store_count = 0;
    }
    script_port_server_select(0xfffffffe);
    return 1;
}

/*
 * nexus_menu_server_action — perform a server-list action (0x24030..0x2405f),
 * owner-gated on the 'Q' screen, not in battle, menu visible. 0x24001
 * closes back to the saved screen char; 0x24002 selects -1, 0x24003
 * selects -2, ids >= 0x24010 select the stored server at
 * (id - 0x24010) (bounded by the store count, ids from the 0x12-stride
 * table at 0x284b08). Returns the select verdict (or 1/4), 3 busy.
 * @ 00185f90
 */
uint32_t nexus_menu_server_action(uint32_t action, int owner_id)
{
    if (action - 0x24030 >= 0xffffffd0)
        return 4;
    if ((ui_latch_test_and_set(1, &g_menu_state_latch) & 1) != 0)
        return 3;
    if (owner_id < 1 || (int)g_menu_owner_id != owner_id
        || g_menu_open_char != 'Q' || g_menu_battle_flag != 0
        || g_menu_visible == 0) {
        g_menu_state_latch = 0;
        return 4;
    }

    {
        int32_t select;

        if (action == 0x24003)
            select = 0xfffffffe;
        else if (action == 0x24002)
            select = 0xffffffff;
        else if (action == 0x24001) {
            g_menu_battle_row = 0;
            g_menu_open_char = (char)g_theme_last_screen;
            g_menu_generation = g_menu_generation + 1;
            g_menu_state_latch = 0;
            return 1;
        } else {
            if (action < 0x24010) {
                g_menu_state_latch = 0;
                return 4;
            }
            if (g_theme_store_count <= action - 0x24010) {
                g_menu_state_latch = 0;
                return 4;
            }
            select = g_theme_store_ids[(uint64_t)(action - 0x24010) * 0x12];
        }
        g_menu_state_latch = 0;
        {
            uint32_t verdict = (uint32_t)script_port_server_select(select);

            if ((ui_latch_test_and_set(1, &g_menu_state_latch) & 1) != 0)
                return verdict;
            g_menu_generation = g_menu_generation + 1;
            g_menu_state_latch = 0;
            return verdict;
        }
    }
}

/* ===== theme action ===== */

/*
 * nexus_menu_theme_action — perform a theme-page action (0x26001..0x2605f),
 * owner-gated on the 't' screen, not in battle, menu visible. 0x26001
 * closes the theme page: clears the busy flag and status line, bumps the
 * row revision, restores state 2 pages to the rodata defaults (0x10f860)
 * or closes to the saved screen char. The slot actions require the page
 * machine ready (not busy, rows loaded): 0x26004 applies the pending
 * background (state 3), 0x26002/3 move the music slot (8-slot wrap,
 * bounded by the store cap), 0x26006 commits the selected music, 0x26007
 * (apply + preview + flag gates) or 0x26005 (flag gate) defer the apply;
 * state-3 pages accept 0x26008/9/a (toggle bits vs 0x285430) and
 * 0x2600b/d/e (slot ids 0x285434/8); background rows 0x26020+ (bounded by
 * the store rows, generation-matched, slot-bitmask-gated) select a
 * background ('CHOOSE MUSIC FOR THIS BACKGROUND' or 'SETTINGS CHANGED -
 * SELECT BACKGROUND AGAIN') and finish with theme_preview_load_begin.
 * Returns 1 applied, 2 deferred/busy, 4 rejected, 3 busy. @ 00186548
 */
uint64_t nexus_menu_theme_action(uint32_t action, int owner_id)
{
    if (action - 0x26040 >= 0xffffffc0)
        return 4;
    if ((ui_latch_test_and_set(1, &g_menu_state_latch) & 1) != 0)
        return 3;
    if (owner_id < 1 || (int)g_menu_owner_id != owner_id
        || g_menu_visible == 0 || g_menu_battle_flag != 0
        || g_menu_open_char != 't') {
        g_menu_state_latch = 0;
        return 4;
    }

    if (action == 0x26001) {
        /* close the theme page */
        g_theme_busy = 0;
        g_theme_music_status = 0;
        g_theme_row_rev_me10 = g_theme_row_rev_me10 + 1;
        if (g_theme_apply_state == 2) {
            g_theme_busy = 1;
            g_theme_apply_state = (uint32_t)g_theme_defaults_pair; /* 0x10f860 */
            g_theme_slot_sel = (uint32_t)(g_theme_defaults_pair >> 32);
        } else if (g_theme_apply_state == 0) {
            g_menu_open_char = (char)g_theme_page_rev;
            g_theme_slots_used = 0;
            g_theme_gen = g_theme_gen + 1;
            g_menu_generation = g_menu_generation + 1;
            g_menu_battle_row = 0;
            g_menu_state_latch = 0;
            return 1;
        } else {
            g_theme_apply_state = 0;
            g_theme_slot_sel = 0;
            g_theme_slot_valid = 0;
            g_theme_music_lo = (int32_t)g_theme_defaults_close; /* 0x10f740 */
            g_theme_music_hi = (int32_t)(g_theme_defaults_close >> 32);
        }
        g_theme_slot_valid = 0;
        g_theme_prev_row_rev = g_theme_row_rev_me10;
        menu_scroll_revision_bump(1, (int *)&g_theme_scroll_rev);
        g_theme_slots_used = 0;
        g_theme_gen = g_theme_gen + 1;
        g_menu_generation = g_menu_generation + 1;
        g_menu_battle_row = 0;
        g_menu_state_latch = 0;
        return 1;
    }

    /* slot actions: machine must be ready */
    if (g_theme_busy != 0) {
        g_menu_state_latch = 0;
        return 2;
    }
    if (g_theme_slot_valid == 0 || g_theme_store_rows == 0) {
        g_menu_state_latch = 0;
        return 4;
    }

    if (action == 0x26004 && g_theme_apply_state == 0) {
        /* apply the pending background */
        g_theme_apply_state = 3;
        menu_scroll_revision_bump(1, (int *)&g_theme_scroll_rev);
        g_menu_state_latch = 0;
        g_menu_battle_row = 0;
        g_menu_generation = g_menu_generation + 1;
        g_theme_gen = g_theme_gen + 1;
        g_theme_slots_used = 0;
        return 1;
    }

    if ((action & 0xfffffffe) == 0x26002 && g_theme_apply_state < 2) {
        /* music slot move (0x26002 down, 0x26003 up) with 8-slot wrap */
        uint32_t slot;

        if (action == 0x26002) {
            if (g_theme_slot_sel == 0) {
                g_menu_state_latch = 0;
                return 4;
            }
            slot = 0;
            if (7 < g_theme_slot_sel)
                slot = g_theme_slot_sel - 8;
        } else {
            slot = g_theme_slot_sel + 8;
            if (g_theme_store_cap <= g_theme_slot_sel + 8) {
                g_menu_state_latch = 0;
                return 4;
            }
        }
        g_theme_slot_valid = 0;
        g_theme_busy = 1;
        g_theme_prev_row_rev = g_theme_row_rev_me10;
        g_theme_slot_sel = slot;
        menu_scroll_revision_bump(1, (int *)&g_theme_scroll_rev);
        g_menu_state_latch = 0;
        g_menu_battle_row = 0;
        g_menu_generation = g_menu_generation + 1;
        g_theme_gen = g_theme_gen + 1;
        g_theme_slots_used = 0;
        return 1;
    }

    if (action < 0x26020 || 1 < g_theme_apply_state) {
        /* machine-state actions */
        uint32_t mode = 0;
        uint32_t value = 0;
        int32_t extra = 0;

        if (action == 0x26006
            && !(g_theme_apply_state == 1 || g_theme_music_hi == 0)) {
            if (g_theme_settings_rev == g_theme_page_rev_src) {
                /* commit the selected music: clear the music block */
                memset((void *)0x287a64, 0, 0x90); /* raw 0x287a64..0x287af8 */
                g_theme_music_a = -2;
                ui_status_line_format((void *)0x287a6c, 0x60, 0x60,
                                      "NO MUSIC");
                g_theme_music_lo = 1;
                theme_preview_load_begin();
                g_menu_state_latch = 0;
                return 1;
            }
            ui_status_line_format((void *)0x287c6c, 0x60, 0x60,
                                  g_format_1343dc,
                                  "SETTINGS CHANGED - SELECT BACKGROUND AGAIN");
            g_menu_state_latch = 0;
            g_menu_battle_row = 0;
            g_menu_generation = g_menu_generation + 1;
            g_theme_gen = g_theme_gen + 1;
            g_theme_slots_used = 0;
            return 4;
        }
        if (action == 0x26007
            && g_theme_apply_state == 2 && g_theme_music_prev == 1
            && g_theme_music_hi != 0 && g_theme_music_a != 0
            && (g_theme_store_slots & 1) != 0) {
            mode = 1;
            value = (uint32_t)g_theme_music_a;
            extra = g_theme_music_src;
        } else if (action == 0x26005 && (g_theme_store_slots & 1) != 0) {
            theme_apply_defer(2, 0, 0);
            g_menu_state_latch = 0;
            return 2;
        } else if (g_theme_apply_state != 3) {
            g_menu_state_latch = 0;
            return 4;
        } else if (action == 0x26008) {
            value = 1;
        } else if (action == 0x26009) {
            value = 4;
        } else if (action == 0x2600a) {
            value = 2;
        } else {
            g_menu_state_latch = 0;
            return 4;
        }
        if (mode == 0 && (value == 0 || (g_theme_store_slots >> 1 & 1) == 0)) {
            if (action - 0x2600d >= 0xfffffffe
                || (g_theme_store_slots & 1) == 0) {
                if (action == 0x2600d
                    && (g_theme_store_slots >> 2 & 1) != 0) {
                    mode = 4;
                    extra = g_theme_store_bit30;
                } else if (action == 0x2600e
                           && (g_theme_store_slots >> 3 & 1) != 0) {
                    mode = 5;
                    extra = g_theme_store_bit38;
                } else {
                    g_menu_state_latch = 0;
                    return 4;
                }
                value = (uint32_t)(extra == 0);
            } else {
                mode = 6;
                value = 1;
                if (action == 0x2600b)
                    value = 0xffffffff;
            }
        } else if (mode == 0) {
            mode = 3;
            value = g_theme_store_sel ^ value;
        }
        theme_apply_defer(mode, value, extra);
        g_menu_state_latch = 0;
        return 2;
    }

    /* background rows 0x26020+ */
    {
        uint32_t idx = action - 0x26020;

        if (g_theme_store_rows <= idx
            || g_theme_prev_gen != g_theme_gen
            || (g_theme_slots_used >> (idx & 0x1f) & 1) == 0) {
            g_menu_state_latch = 0;
            return 4;
        }
        {
            int32_t row_id = g_theme_page_ids[idx];
            uint32_t flags = g_theme_page_flags_row(idx);

            if (row_id != g_theme_page_rev_src_row(idx)) {
                g_menu_state_latch = 0;
                return 4;
            }
            if (g_theme_apply_state == 0) {
                if ((flags >> 1 & 1) != 0) {
                    /* select the background */
                    g_theme_music_src = row_id;
                    g_theme_music_b = flags;
                    memmove((void *)0x287944,
                            g_theme_page_names + (uint64_t)idx * 0x128,
                            0x120);
                    g_theme_settings_rev = g_theme_store_rev;
                    g_theme_music_hi = (int32_t)g_theme_defaults_pair;
                    g_theme_music_lo = (int32_t)(g_theme_defaults_pair >> 32);
                    g_theme_apply_state = 1;
                    ui_status_line_format((void *)0x287c6c, 0x60, 0x60,
                                          g_format_1343dc,
                                          "CHOOSE MUSIC FOR THIS BACKGROUND");
                    g_theme_busy = 1;
                    g_theme_slot_sel = 0;
                    g_theme_slot_valid = 0;
                    g_theme_prev_row_rev = g_theme_row_rev_me10;
                    menu_scroll_revision_bump(1, (int *)&g_theme_scroll_rev);
                    g_menu_state_latch = 0;
                    g_menu_battle_row = 0;
                    g_menu_generation = g_menu_generation + 1;
                    g_theme_gen = g_theme_gen + 1;
                    g_theme_slots_used = 0;
                    return 1;
                }
                g_menu_state_latch = 0;
                return 4;
            }
            if (g_theme_music_hi == 0
                || g_theme_settings_rev != g_theme_store_rev) {
                ui_status_line_format((void *)0x287c6c, 0x60, 0x60,
                                      g_format_1343dc,
                                      "SETTINGS CHANGED - SELECT BACKGROUND AGAIN");
                g_menu_state_latch = 0;
                g_menu_battle_row = 0;
                g_menu_generation = g_menu_generation + 1;
                g_theme_gen = g_theme_gen + 1;
                g_theme_slots_used = 0;
                return 4;
            }
            g_theme_music_b = flags;
            g_theme_music_a = row_id;
            memmove((void *)0x287a6c,
                    g_theme_page_names + (uint64_t)idx * 0x128, 0x120);
        }
    }
    g_theme_music_lo = 1;
    theme_preview_load_begin();
    g_menu_state_latch = 0;
    return 1;
}

/* menu_engine chain chunk 11: covers raw lines 5701-6150 (theme_action tail finished in chunk 10; here: profile_open/action/pump straddles into 6145+ and continues) */

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
#include <fcntl.h>
#include <sys/stat.h>

extern int __android_log_print(int prio, const char *tag, const char *fmt, ...);
pid_t gettid(void);
int pthread_getname_np(pthread_t, char *, size_t);

/* ---- carried declarations (earlier chunks own the definitions) ---- */
extern int ui_latch_test_and_set(int which, void *addr); /* menu_engine p4 @ 00193f80 */
extern uint32_t g_launcher_menu_id;  /* 0x22f038 — owning menu id / Mainloop tid */
extern uint32_t g_stage_log_count;   /* 0x22f4bc — capped-128 stage-log counter */
extern void ui_status_line_format(void *buf, size_t a, size_t b,
                                  const char *fmt, ...); /* misc @ 00183ac8 */
extern int menu_scroll_revision_bump(int delta, int *p); /* @ 00194010 */
extern int theme_menu_status_set(int code, int tid, const char *reason); /* misc @ 00183cdc */
extern int script_port_profile_snapshot(void *a, size_t size, int page); /* misc @ 00191f04 */
extern int script_port_profile_open(const char *name, size_t len); /* misc @ 00191f60 */
extern int script_port_profile_set_name(const char *name, size_t len); /* misc @ 00191fa8 */

/* menu state block (chunk 8/9/10 own the definitions) */
extern uint32_t g_menu_state_latch;  /* 0x284688 — state test-and-set latch */
extern uint32_t g_menu_owner_id;     /* 0x284694 — owning id (tid gate) */
extern uint8_t  g_menu_battle_flag;  /* 0x28469e — in-battle flag */
extern uint8_t  g_menu_battle_row;   /* 0x28469f — battle scroll row */
extern uint8_t  g_menu_open_char;    /* 0x28469d — open screen char */
extern uint32_t g_menu_generation;   /* 0x2846a0 — state generation */
extern uint8_t  g_menu_visible;      /* 0x28469c — menu visible flag */
extern uint32_t g_theme_scroll_rev;  /* 0x22d824 — g_menu_scroll_revision */
extern const char g_format_1343dc[]; /* 0x1343dc — status format */

/* profile page state (chunk 10 externs/definitions) */
extern uint32_t g_profile_last_screen; /* 0x28c288 — pre-profile screen char */
extern uint32_t g_profile_busy;        /* 0x28c28c — page busy flag */
extern uint32_t g_profile_pending;     /* 0x28c290 — pending action */
extern uint32_t g_profile_generation;  /* 0x28c298 — page generation */
extern uint32_t g_profile_action;      /* 0x28c294 — pending action id */
extern uint32_t g_profile_status_line; /* 0x28c2c8 — status line handle */
extern uint32_t g_profile_a;           /* 0x28c2a0 — profile field a */
extern uint32_t g_profile_slots;       /* 0x28a4c0 — profile slots present */
extern uint32_t g_profile_slot_mask;   /* 0x28a4c4 — slot permission mask */
extern uint32_t g_profile_slot_d;      /* 0x28a4d4 — slot-d value */

/*
 * nexus_menu_profile_open — open the profile page ('p' screen),
 * owner-gated, not in battle, menu visible. Saves the previous screen char
 * (0x28c288 unless 'p'), clears the busy/pending/status fields, bumps the
 * page generation and the menu generation. Returns 1 opened, 3 busy, 4
 * rejected. @ 00187e48
 */
uint64_t nexus_menu_profile_open(int owner_id)
{
    if ((ui_latch_test_and_set(1, &g_menu_state_latch) & 1) == 0) {
        uint64_t verdict = 4;

        if (owner_id > 0 && (int)g_menu_owner_id == owner_id
            && g_menu_visible != 0 && g_menu_battle_flag == 0) {
            if (g_menu_open_char != 0x70)
                g_profile_last_screen = (uint32_t)g_menu_open_char;
            verdict = 1;
            g_profile_busy = 0;
            g_profile_a = 0;
            g_profile_status_line = 0;
            g_profile_generation = g_profile_generation + 1;
            g_menu_battle_row = 0;
            g_menu_generation = g_menu_generation + 1;
            g_menu_open_char = 0x70; /* 'p' */
        }
        g_menu_state_latch = 0;
        return verdict;
    }
    return 3;
}

/*
 * nexus_menu_profile_action — perform a profile action (0x28000..0x28005),
 * owner-gated on the 'p' screen, not in battle, menu visible. 0x28000
 * closes back to the saved screen char (pending cleared). Other actions
 * require the page idle, slots present and the action permitted by the
 * slot mask (0x28004 uses bit 3, else bit 2; 0x28001 bit 0; 0x28005 also
 * needs slot-d non-zero): they latch the pending state 1 (0x28001/2,
 * 'ENTER A VALUE') or 3 ('APPLYING...') with the action id recorded.
 * Returns 1 closed, 2 latched, 0 not permitted, 3 busy, 4 rejected.
 * @ 00187f14
 */
uint64_t nexus_menu_profile_action(int action, int owner_id)
{
    if ((uint32_t)(action - 0x28006) < 0xfffffffa)
        return 4;
    if ((ui_latch_test_and_set(1, &g_menu_state_latch) & 1) != 0)
        return 3;
    if (owner_id < 1 || (int)g_menu_owner_id != owner_id
        || g_menu_visible == 0 || g_menu_battle_flag != 0
        || g_menu_open_char != 'p') {
        g_menu_state_latch = 0;
        return 4;
    }

    if (action == 0x28000) {
        g_profile_pending = 0;
        g_menu_battle_row = 0;
        g_profile_generation = g_profile_generation + 1;
        g_menu_open_char = (char)g_profile_last_screen;
        g_menu_generation = g_menu_generation + 1;
        g_menu_state_latch = 0;
        return 1;
    }

    {
        if (g_profile_pending != 0) {
            g_menu_state_latch = 0;
            return 2;
        }
        if (g_profile_busy == 0 || g_profile_slots == 0) {
            g_menu_state_latch = 0;
            return 0;
        }
        {
            uint32_t need = action == 0x28004 ? 8 : 4;

            if (action == 0x28001)
                need = 1;
            if ((g_profile_slot_mask & need) == 0
                || (action == 0x28005 && g_profile_slot_d == 0)) {
                g_menu_state_latch = 0;
                return 0;
            }
            {
                bool value_entry = (uint32_t)(action - 0x28001) < 2;

                g_profile_pending = value_entry ? 1 : 3;
                ui_status_line_format(&g_profile_status_line, 0x60, 0x60,
                                      g_format_1343dc,
                                      value_entry ? "ENTER A VALUE"
                                                  : "APPLYING...");
                g_profile_action = (uint32_t)action;
            }
        }
    }
    g_menu_generation = g_menu_generation + 1;
    g_menu_state_latch = 0;
    return 2;
}

/*
 * nexus_menu_profile_pump — defined in chunk 12 of this chain (raw body
 * 6145-6492 fully reconstructed there).
 */
/* menu_engine chain chunk 12: covers raw lines 6150-6600 (completes nexus_menu_profile_pump from raw 6150 mid-function; then nexus_menu_main_count and nexus_menu_section_snapshot) */

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
#include <fcntl.h>
#include <sys/stat.h>

extern int __android_log_print(int prio, const char *tag, const char *fmt, ...);
pid_t gettid(void);
int pthread_getname_np(pthread_t, char *, size_t);

/* ---- carried declarations (earlier chunks own the definitions) ---- */
extern int ui_latch_test_and_set(int which, void *addr); /* menu_engine p4 @ 00193f80 */
extern int menu_server_seq_next(void);   /* chunk 1 @ 0014c510 */
extern bool menu_entry_register(int id, char *key, char *desc, uint32_t type,
                                int screen); /* chunk 1 @ 0014c550 */
extern int menu_entry_value_read(int id, uint32_t *kind_out, char *value_out,
                                 size_t value_cap); /* chunk 1 @ 0014c8ac */
extern void menu_entry_remove(int id);   /* chunk 1 @ 0014cc0c */
extern void ui_status_line_format(void *buf, size_t a, size_t b,
                                  const char *fmt, ...); /* misc @ 00183ac8 */
extern int ui_named_action_invoke(int action_id); /* widgets @ 001924cc */
extern int ui_named_state_query(const char *name, void *out); /* widgets @ 00192340 */
extern void theme_store_reset(void *buf, uint32_t size); /* misc @ 00191920 */
extern int theme_page_parse(void *page, uint32_t slot); /* misc @ 0018cb98 */
extern int script_port_profile_snapshot(void *a, size_t size, int page); /* misc @ 00191f04 */
extern int script_port_profile_open(const char *name, size_t len); /* misc @ 00191f60 */
extern int script_port_profile_set_name(const char *name, size_t len); /* misc @ 00191fa8 */
extern int nexus_script_port_ui_reload_request(void); /* renderer @ 0014d054 */

/* menu state block (earlier chunks own the definitions) */
extern uint32_t g_menu_state_latch;  /* 0x284688 — state test-and-set latch */
extern uint32_t g_menu_owner_id;     /* 0x284694 — owning id (tid gate) */
extern uint8_t  g_menu_battle_flag;  /* 0x28469e — in-battle flag */
extern uint8_t  g_menu_battle_row;   /* 0x28469f — battle scroll row */
extern uint8_t  g_menu_open_char;    /* 0x28469d — open screen char */
extern uint32_t g_menu_generation;   /* 0x2846a0 — state generation */
extern uint8_t  g_menu_visible;      /* 0x28469c — menu visible flag */
extern const char g_format_1343dc[]; /* 0x1343dc — status format */

/* profile page state (chunk 10/11 externs) */
extern uint32_t g_profile_last_screen; /* 0x28c288 — pre-profile screen char */
extern uint32_t g_profile_busy;        /* 0x28c28c — page busy flag */
extern uint32_t g_profile_pending;     /* 0x28c290 — pending action */
extern uint32_t g_profile_generation;  /* 0x28c298 — page generation */
extern uint32_t g_profile_action;      /* 0x28c294 — pending action id */
extern uint32_t g_profile_status_line; /* 0x28c2c8 — status line handle */
extern uint32_t g_profile_a;           /* 0x28c2a0 — refresh window start */
extern uint32_t g_profile_b;           /* 0x28c2a8 — stored entry id */
extern uint32_t g_profile_slots;       /* 0x28a4c0 — profile slots present */
extern uint32_t g_profile_slot_mask;   /* 0x28a4c4 — slot permission mask */
extern uint32_t g_profile_slot_d;      /* 0x28a4d4 — slot-d value */
extern uint32_t g_profile_snap_hdr;    /* 0x28c2b0 — snapshot header lo */
extern uint32_t g_profile_snap_hi;     /* 0x28c2b8 — snapshot header hi */
extern uint32_t g_profile_snap_win;    /* 0x28c2c0 — snapshot window flag */
extern uint64_t g_profile_snapshot[0x3ba]; /* 0x28a4b8 — 0x1dd0 snapshot cache */

/* theme machine state (chunk 10 externs) */
extern uint32_t g_theme_apply_state;   /* 0x287b90 */
extern uint32_t g_theme_slot_sel;      /* 0x287b94 */
extern uint32_t g_theme_slot_valid;    /* 0x287b98 */
extern int32_t  g_theme_music_hi;      /* 0x287b9c */
extern int32_t  g_theme_music_lo;      /* 0x287ba0 */
extern uint32_t g_theme_busy;          /* 0x287ba4 */
extern uint32_t g_theme_gen;           /* 0x287bac */
extern uint32_t g_theme_prev_gen;      /* 0x287bb0 */
extern uint32_t g_theme_slots_used;    /* 0x287bb4 */
extern uint32_t g_theme_settings_rev;  /* 0x287c40 */
extern uint32_t g_theme_row_rev_me12;  /* 0x287c48 — theme row revision */
extern uint32_t g_theme_prev_row_rev;  /* 0x287c50 */
extern int32_t  g_theme_music_a;       /* 0x287a64 */
extern uint32_t g_theme_music_b;       /* 0x287a68 */
extern char     g_theme_music_name[0x60]; /* 0x287a6c */
extern uint32_t g_theme_music_src;     /* 0x287940 */
extern uint32_t g_theme_music_prev;    /* 0x287c68 */
extern uint32_t g_theme_store_rows;    /* 0x285414 */
extern uint32_t g_theme_store_rev;     /* 0x285420 */
extern uint32_t g_theme_store_cap;     /* 0x285418 */
extern uint32_t g_theme_store_slots;   /* 0x285424 */
extern uint32_t g_theme_store_sel;     /* 0x285430 */
extern uint32_t g_theme_store_bit30;   /* 0x285434 */
extern uint32_t g_theme_store_bit38;   /* 0x285438 */
extern int32_t  g_theme_page_ids[];    /* 0x287bc0 */
extern uint32_t g_theme_page_flag_tbl[]; /* 0x285440 */
extern int32_t  g_theme_page_src_tbl[];   /* 0x28543c */
extern uint8_t  g_theme_page_names[];  /* 0x285444 */

/* theme store block (raw 0x284af0) */
extern uint64_t g_theme_store;         /* 0x284af0 — 0x918 store buffer */
extern uint32_t g_theme_store_count;   /* 0x284af4 — store count (<= 0x20) */
extern uint32_t g_theme_store_sel_id;  /* 0x284af8 — selected server id */
extern uint32_t g_theme_store_flag;    /* 0x284afc — store flag */
extern int32_t  g_theme_store_def;     /* 0x284b00 — default server id */
extern uint32_t g_theme_store_ids[];   /* 0x284b08 — server ids (0x12 stride) */
extern char     g_theme_store_names[]; /* 0x284b10 — server names (0x48 stride) */

/* about/debug page blocks (raw 0x28a3d8, 0x28a428, 0x28a454) */
extern uint32_t g_debug_slots_a;       /* 0x28a3d8 — debug slots lo */
extern uint32_t g_debug_slots_b;       /* 0x28a3dc — debug slots hi */
extern uint32_t g_debug_bit_len;       /* 0x28a3e0 — bitfield length */
extern uint32_t g_debug_bits[];        /* 0x28a3e4 — bitfield words */
extern uint32_t g_debug_pending;       /* 0x28a428 — debug pending flag */
extern uint32_t g_debug_state;         /* 0x28a42c — debug state */
extern uint32_t g_debug_extra;         /* 0x28a438 — gfx/mem cycle gate */
extern char     g_debug_status_line[0x80]; /* 0x28a454 — status line */

/* editor page (raw 0x28cc30) */
extern uint32_t g_editor_page_state;   /* 0x28cc30 — editor page state */

/* section snapshot id block (raw 0x28dcf0) + theme status line */
extern uint32_t g_section_hdr_a;  /* 0x28dcf0 — snapshot id a */
extern uint32_t g_section_hdr_b;  /* 0x28dcf4 — snapshot id b */
extern uint32_t g_section_hdr_c;  /* 0x28dcf8 — snapshot id c */
extern uint32_t g_theme_status_me12; /* 0x287c6c — theme music status line */

/* rodata tables */
extern const uint64_t g_profile_tpl;   /* 0x10f800 — snapshot template qword */
extern const uint64_t g_show_skin_tpl; /* 0x140718 — named-state template lo */
extern const char g_show_skin_name[];  /* "ShowSkinNamesInProfile" */
extern const char g_empty_text_me12[]; /* 0x134f22 — shared "" rodata */
extern const char *const g_about_rows[0x28 * 5]; /* 0x19cf58 — ABOUT_SCREEN
                                                    table {name,text,extra} */
extern const int32_t g_about_ids[0x28];  /* 0x19cf48 — about row ids */
extern const uint8_t g_screen_chars[0x41]; /* 0x19ddd0 — screen char table
                                              (0x28 stride walk) */

/* helper: cast the rodata "" to a mutable desc pointer (raw passes the
 * empty rodata directly as the description buffer) */
static char *desc_of_me12(const char *s)
{
    return (char *)(uintptr_t)s;
}

/*
 * nexus_menu_profile_pump — profile page per-frame pump (owner-gated, 'p'
 * screen, not in battle). Leaving the page clears the refresh window and
 * bumps the generation. With no pending action (or a completed one): a
 * 500ms refresh window re-reads the 0x1dd0 profile snapshot via
 * script_port_profile_snapshot (template qword 0x10f800, magic 1, size
 * 0x1dd0, slot-count/state gates, NUL-terminated name fields) and the
 * 'ShowSkinNamesInProfile' named state. A pending action resolves:
 * state 1 (value entry) registers the entered value as a menu entry
 * ('PLAYER TAG' 16 bytes / 'VISUAL NAME' 100 bytes, type 2/0, through
 * menu_entry_register with a menu_server_seq_next id) reading the previous
 * value via menu_entry_value_read; state 2 reads the stored entry; state 3
 * applies. Verdicts set the busy hi-word (2 = in flight, else cleared with
 * 'APPLYING...'/'UNAVAILABLE IN THE CURRENT SCREEN'/'CANCELLED'), the
 * snapshot cache at 0x28a4b8 is adopted on success, entry ids are
 * released via menu_entry_remove, and the completed action submits:
 * 0x28001 script_port_profile_open, 0x28002/3 set_name (0x28003 clears the
 * name) + ui_reload_request, 0x28004 toggles the named action
 * ('ShowSkinNamesInProfile' via ui_named_action_invoke), 0x28005 reload.
 * Final status lines: 'ACTION COULD NOT BE APPLIED' / 'NAME SAVED -
 * RELOAD TO APPLY' / 'APPLIED TO THE NEXT PROFILE'. @ 001880e0
 */
void nexus_menu_profile_pump(int owner_id, uint64_t now_ms)
{
    uint32_t stored_id;
    uint32_t saved_gen;
    uint32_t saved_action;
    uint8_t verdict_hi = 3;

    if ((ui_latch_test_and_set(1, &g_menu_state_latch) & 1) != 0)
        return;
    stored_id = g_profile_b;
    saved_gen = g_profile_generation;
    saved_action = g_profile_action;

    if (owner_id < 1 || (int)g_menu_owner_id != owner_id) {
        g_menu_state_latch = 0;
        return;
    }

    if (g_menu_visible == 0 || g_menu_battle_flag != 0
        || g_menu_open_char != 'p') {
        /* left the page: clear the window and refresh state */
        g_profile_b = 0;
        g_profile_busy = 0;
        g_profile_generation = g_profile_generation + 1;
        g_menu_state_latch = 0;
        if (stored_id == 0)
            return;
    } else {
        int pending = (int)g_profile_pending;

        if (g_profile_b == 0 || g_profile_pending != 0) {
            if (g_profile_pending == 4) {
                g_menu_state_latch = 0;
                return;
            }
            if (g_profile_pending == 0 && g_profile_a != 0
                && g_profile_a <= now_ms && now_ms - g_profile_a < 500) {
                g_menu_state_latch = 0;
                return;
            }

            /* refresh: snapshot + named state, then resolve the pending */
            g_profile_busy = (g_profile_busy & 0xffffffffu) | (4u << 0);
            {
                struct {
                    uint64_t magic_size;  /* +0x00 */
                    uint32_t f08, f0c;    /* +0x08/+0x0c */
                    uint32_t counts[2];   /* +0x10/+0x14 */
                    uint32_t state;       /* +0x18 */
                    uint32_t mask;        /* +0x1c */
                    uint32_t f20, f24;    /* +0x20/+0x24 */
                    uint8_t names[0x1dd0 - 0x28];
                } snap;
                char value_buf[0x401];
                char state_node[0x20];
                uint32_t snapshot_ok = 0;
                int rc;

                memset(&snap, 0, sizeof snap);
                snap.magic_size = g_profile_tpl; /* 0x10f800 template */
                rc = script_port_profile_snapshot(&snap, 0x1dd0, 0);
                if (rc != 0 && (uint32_t)snap.magic_size == 1
                    && (uint32_t)(snap.magic_size >> 32) == 0x1dd0
                    && snap.state == 0) {
                    if (snap.counts[1] < 0x21
                        && snap.counts[1] <= snap.counts[0]
                        && snap.counts[0] < 0x201
                        && snap.f08 < 2 && snap.f0c < 2
                        && memchr(snap.names + 0x1917, 0, 0x191) != NULL
                        && memchr(snap.names + 0x1c1, 0, 0x10) != NULL
                        && memchr(snap.names, 0, 0x10) != NULL)
                        snapshot_ok = memchr(snap.names + 0x1d1, 0, 0x191)
                                          != NULL;
                }

                memset(value_buf, 0, sizeof value_buf);
                memset(state_node, 0, sizeof state_node);
                *(uint64_t *)(void *)state_node = g_show_skin_tpl;
                ui_named_state_query("ShowSkinNamesInProfile", state_node);

                if (pending == 0) {
                    /* idle: record the snapshot and window */
                    g_profile_snap_hdr = 0;
                    g_profile_busy = (g_profile_busy & 0xffffffffu)
                                     | (snapshot_ok << 0);
                    g_profile_snap_hi = 0;
                    g_profile_snap_win = 0;
                    g_profile_a = (uint32_t)now_ms;
                    if (snapshot_ok != 0)
                        memcpy(g_profile_snapshot, &snap, 0x1dd0);
                    g_menu_generation = g_menu_generation + 1;
                    g_menu_state_latch = 0;
                    return;
                }

                {
                    uint32_t value_entry_ok = 0; /* uVar17 == 0 means ok */
                    uint32_t apply_ok = 0;
                    uint32_t close_visible = 0;
                    uint32_t new_id = 0;

                    if (snapshot_ok == 0) {
                        /* snapshot not usable */
                    } else if (snap.f08 != 0) {
                        uint32_t need = saved_action == 0x28004 ? 8 : 4;

                        if (saved_action == 0x28001)
                            need = 1;
                        if ((snap.mask & need) == 0) {
                            value_entry_ok = 1;
                        } else if (pending == 3) {
                            apply_ok = 1;
                            value_entry_ok = 1;
                        } else if (pending == 2) {
                            /* read the stored entry value */
                            uint32_t kind = 3;

                            apply_ok = (uint32_t)menu_entry_value_read(
                                stored_id, &kind, value_buf, 0x401);
                            if (apply_ok != 0 && kind != 3) {
                                if (kind == 2)
                                    apply_ok = 2;
                                else if (kind == 1)
                                    close_visible = 1;
                                else
                                    value_entry_ok = 0;
                            }
                        } else if (pending == 1) {
                            /* register the entered value as a menu entry */
                            new_id = (uint32_t)menu_server_seq_next();
                            apply_ok = new_id != 0;
                            if (new_id != 0) {
                                bool is_tag = saved_action == 0x28001;
                                const char *def = is_tag
                                                      ? g_empty_text_me12
                                                      : (const char *)
                                                            (snap.names
                                                             + 0x1917);
                                const char *key =
                                    is_tag ? "PLAYER TAG" : "VISUAL NAME";
                                uint32_t cap = is_tag ? 0x10 : 100;

                                apply_ok = (uint32_t)menu_entry_register(
                                    (int)new_id, (char *)key, (char *)desc_of_me12(def),
                                    is_tag ? 2u : 0u, (int)cap);
                            }
                            value_entry_ok = apply_ok ^ 1;
                        }
                    }

                    if ((ui_latch_test_and_set(1, &g_menu_state_latch) & 1)
                        != 0) {
                        if (new_id != 0)
                            menu_entry_remove((int)new_id);
                        return;
                    }
                    if (g_menu_visible == 0 || g_menu_battle_flag != 0
                        || g_menu_open_char != 'p'
                        || g_profile_generation != saved_gen
                        || g_profile_pending != 4) {
                        g_menu_state_latch = 0;
                        if (new_id != 0)
                            menu_entry_remove((int)new_id);
                        verdict_hi = 4;
                        goto out;
                    }

                    /* adopt the snapshot + window */
                    g_profile_snap_hi = 0;
                    g_profile_snap_hdr = 0;
                    g_profile_busy = (g_profile_busy & 0xffffffffu)
                                     | (snapshot_ok << 0);
                    g_profile_snap_win = 0;
                    g_profile_a = (uint32_t)now_ms;
                    if (snapshot_ok != 0)
                        memcpy(g_profile_snapshot, &snap, 0x1dd0);

                    if (value_entry_ok == 0) {
                        g_profile_busy = (g_profile_busy & 0xffffffffu)
                                         | (2u << 0);
                        if (new_id != 0)
                            g_profile_b = new_id;
                    } else {
                        const char *status =
                            apply_ok != 0 ? "APPLYING..."
                                          : "UNAVAILABLE IN THE CURRENT SCREEN";
                        const char *final_status =
                            apply_ok == 2 ? "CANCELLED" : status;

                        g_profile_busy = g_profile_busy & 0xffffffffu;
                        g_profile_b = 0;
                        ui_status_line_format(&g_profile_status_line, 0x60,
                                              0x60, g_format_1343dc,
                                              final_status);
                    }

                    if (close_visible != 0 && saved_action != 0x28004)
                        g_menu_visible = 0;
                    g_menu_generation = g_menu_generation + 1;
                    g_menu_state_latch = 0;
                    if (value_entry_ok != 0 && stored_id != 0)
                        menu_entry_remove((int)stored_id);
                    if (value_entry_ok != 0 && new_id != 0)
                        menu_entry_remove((int)new_id);

                    if (close_visible != 0) {
                        uint32_t apply_rc = 0;
                        uint32_t reload_rc = 0;

                        if (saved_action == 0x28001) {
                            apply_rc = (uint32_t)script_port_profile_open(
                                value_buf, strnlen(value_buf, 0x401));
                        } else if ((saved_action & 0xfffffffe) == 0x28002) {
                            const char *name = value_buf;
                            size_t len = strnlen(value_buf, 0x401);

                            if (saved_action == 0x28003) {
                                name = g_empty_text_me12;
                                len = 0;
                            }
                            apply_rc = (uint32_t)script_port_profile_set_name(
                                name, len);
                            if (apply_rc != 0)
                                reload_rc = (uint32_t)
                                    nexus_script_port_ui_reload_request();
                        } else if (saved_action == 0x28005) {
                            apply_rc = (uint32_t)
                                nexus_script_port_ui_reload_request();
                            reload_rc = apply_rc;
                        } else if (saved_action == 0x28004) {
                            /* toggle the named skin-names action */
                            long idx = 0;
                            const char *const *rows = g_about_rows;

                            while (idx != -0x37) {
                                if (strcmp(rows[0],
                                           "ShowSkinNamesInProfile") == 0) {
                                    int rc2 = ui_named_action_invoke(
                                        (int)(0x21000 - idx));

                                    apply_rc = (uint32_t)(rc2 == 1);
                                    break;
                                }
                                idx = idx - 1;
                                rows += 3;
                            }
                        }

                        if ((ui_latch_test_and_set(1, &g_menu_state_latch)
                             & 1) == 0) {
                            if (g_profile_generation == saved_gen
                                && g_menu_battle_flag == 0
                                && g_menu_open_char == 'p') {
                                g_profile_a = 0;
                                if (apply_rc == 0) {
                                    g_menu_visible = 1;
                                    ui_status_line_format(
                                        &g_profile_status_line, 0x60, 0x60,
                                        "ACTION COULD NOT BE APPLIED");
                                } else {
                                    if (((saved_action & 0xfffffffe)
                                         == 0x28002)
                                        && reload_rc == 0) {
                                        g_menu_visible = 1;
                                        ui_status_line_format(
                                            &g_profile_status_line, 0x60,
                                            0x60, "NAME SAVED - RELOAD TO APPLY");
                                    } else if (saved_action == 0x28004) {
                                        ui_status_line_format(
                                            &g_profile_status_line, 0x60,
                                            0x60,
                                            "APPLIED TO THE NEXT PROFILE");
                                    }
                                }
                                g_menu_generation = g_menu_generation + 1;
                            }
                            g_menu_state_latch = 0;
                        }
                    }
                    verdict_hi = apply_ok != 0;
                    goto out;
                }
            }
        }
        g_profile_busy = g_profile_busy & 0xffffffffu;
    }

    /* drop the stored entry on the way out */
    g_profile_b = 0;
    g_menu_state_latch = 0;
    menu_entry_remove((int)stored_id);
    verdict_hi = 1;

out:
    (void)verdict_hi;
}

/* ===== main count ===== */

/*
 * nexus_menu_main_count — row count for the current screen. 'Q' refreshes
 * the theme store (0x918) and returns count+3 (count capped 0x20); 'd'
 * (100) returns 0x29; 'e' returns 2 when the editor page state is 4, else
 * 0x15; 'p' returns 6; 't' returns 4/10 by apply state, else 6 + store
 * rows when a slot is loaded; any other screen counts the 0x28-stride
 * screen-char table (0x19ddd0) plus 1 for 'R'. @ 0018975c
 */
int nexus_menu_main_count(void)
{
    int count = 6;

    switch (g_menu_open_char) {
    case 0x51: /* 'Q' server list */
        theme_store_reset((void *)0x284af0, 0x918);
        if (0x20 < g_theme_store_count)
            g_theme_store_count = 0;
        count = (int)g_theme_store_count + 3;
        break;
    case 100: /* 'd' debug */
        count = 0x29;
        break;
    case 0x65: /* 'e' editor */
        count = 2;
        if (g_editor_page_state != 4)
            count = 0x15;
        break;
    case 0x70: /* 'p' profile */
        count = 6;
        break;
    case 0x74: /* 't' theme */
        if (g_theme_apply_state == 2)
            count = 4;
        else if (g_theme_apply_state == 3)
            count = 10;
        else {
            count = 6;
            if (g_theme_slot_valid != 0)
                count = (int)g_theme_store_rows + 6;
        }
        break;
    default: {
        long off;

        count = 0;
        for (off = 0x18; off != 0xba8; off += 0x28) {
            uint32_t match;

            if (g_menu_open_char == 0x52) /* 'R' always counts */
                match = 1;
            else
                match = (uint32_t)(*(const uint32_t *)
                                       ((const uint8_t *)&g_screen_chars
                                        + off)
                                   == (uint32_t)g_menu_open_char);
            count = (int)(match + (uint32_t)count);
        }
        break;
    }
    }
    return count;
}

/* ===== section snapshot ===== */

/*
 * nexus_menu_section_snapshot — build the 0x24a68-byte section snapshot
 * (owner-gated, menu visible, not in battle). Header: magic/size from
 * rodata 0x10f820, generation, screen char, plus the snapshot id block
 * (0x28dcf0/0x28dcf4/0x28dcf8). 't' theme pages fill the theme machine
 * fields (apply state, store rows/rev, music ids, status lines at
 * 0x287c6c/0x287944/0x2879a4/0x287a04/0x287a6c) and — when a slot is
 * loaded and theme_page_parse accepts the store page — the 0x26020+ rows
 * (0x249 stride: id, index, flags with the selected/current bits, three
 * status lines each). 'd' (100) debug pages fill the about block (0x28
 * rows from the ABOUT_SCREEN table, permission bits from the 0x28a3e4
 * bitfield, 'DEBUG PROVIDER UNAVAILABLE'/'UNAVAILABLE IN THE CURRENT
 * CONTEXT'/'WAITING FOR INPUT OR ACTION'/'DISABLE MAX OPTIMIZATION FIRST'
 * fallback lines). 'Q' server pages validate the store (count <= 0x20,
 * ids <= 999999 unique, names NUL-terminated within 0x40) and fill the
 * 0x24010+ rows (0x49-stride names). Returns 1 with the snapshot, 0 on
 * any gate failure. @ 0018c44c
 */
uint64_t nexus_menu_section_snapshot(void *out, uint64_t len, int owner_id)
{
    uint64_t *rec = out;
    uint32_t hdr_a, hdr_b, hdr_c;

    if (out == NULL || len != 0x24a68)
        return 0;
    if ((ui_latch_test_and_set(1, &g_menu_state_latch) & 1) != 0)
        return 0;
    if (!(owner_id > 0 && (int)g_menu_owner_id == owner_id)) {
        g_menu_state_latch = 0;
        return 0;
    }

    memset(out, 0, 0x24a68);
    hdr_a = g_section_hdr_a; /* raw 0x28dcf0 */
    hdr_b = g_section_hdr_b; /* raw 0x28dcf4 */
    hdr_c = g_section_hdr_c; /* raw 0x28dcf8 */
    rec[7] = 0xffffffffffffffffULL;
    rec[8] = 0xffffffffffffffffULL;
    {
        extern const uint64_t g_section_magic; /* 0x10f820 */

        *(uint64_t *)rec = g_section_magic;
    }
    *(uint32_t *)((char *)rec + 0x5c) = hdr_a;
    *(uint32_t *)(rec + 0xc) = hdr_b;
    *(uint32_t *)((char *)rec + 0x14) = (uint32_t)g_menu_open_char;
    *(uint32_t *)((char *)rec + 100) = hdr_c;
    *(uint32_t *)(rec + 0xc) = g_menu_generation; /* +0xc generation */

    if (g_menu_visible != 0 && g_menu_battle_flag == 0) {
        if (g_menu_open_char == 0x74) {
            /* theme page */
            uint32_t dbg_lo = g_debug_slots_a;              /* 0x28a3d8 */

            rec[1] = 2; /* page kind */
            rec[2] = g_theme_store_rows;
            *(uint32_t *)((char *)rec + 0x1c) = g_theme_apply_state;
            dbg_lo = g_theme_store_rev; /* raw overwrites with 0x28542c */
            *(uint32_t *)(rec + 4) = (uint32_t)(g_theme_slot_valid != 0
                                                && g_debug_extra != 0);
            rec[3] = g_theme_store_bit38;
            *(uint32_t *)((char *)rec + 0x3c) = dbg_lo;
            *(uint32_t *)((char *)rec + 0x24) = (uint32_t)(g_theme_busy != 0);
            *(uint64_t *)((char *)rec + 0x34) = g_theme_store_slots;
            *(uint32_t *)(rec + 8) = g_theme_music_src;
            *(uint32_t *)((char *)rec + 0x54) = g_theme_store_bit30;
            *(uint32_t *)(rec + 9) = g_theme_music_prev;
            *(uint64_t *)((char *)rec + 0x4c) = g_theme_store_sel;
            *(uint32_t *)((char *)rec + 0x44) = (uint32_t)g_theme_music_a;
            ui_status_line_format(rec + 0xd, (size_t)-1, 0x80,
                                  g_format_1343dc, &g_theme_status_me12);
            if (g_theme_music_hi != 0) {
                ui_status_line_format(rec + 0x1d, (size_t)-1, 0x60,
                                      g_format_1343dc, (void *)0x287944);
                ui_status_line_format(rec + 0x35, (size_t)-1, 0x60,
                                      g_format_1343dc, (void *)0x2879a4);
                ui_status_line_format(rec + 0x41, (size_t)-1, 0x60,
                                      g_format_1343dc, (void *)0x287a04);
            }
            if (g_theme_music_lo != 0)
                ui_status_line_format(rec + 0x29, (size_t)-1, 0x60,
                                      g_format_1343dc, g_theme_music_name);
            if (g_theme_slot_valid != 0
                && theme_page_parse((void *)0x285408, g_theme_slot_sel) != 0) {
                rec[5] = g_theme_store_cap; /* raw 0x285418 */
                if (g_theme_apply_state < 2) {
                    rec[6] = g_theme_store_rows;
                    if (g_theme_store_rows != 0) {
                        uint32_t i;
                        int32_t *row = (int32_t *)(rec + 0x4d);
                        const uint8_t *names = g_theme_page_names + 0x68;

                        for (i = 0; i < g_theme_store_rows; i++) {
                            int32_t src_id = *(const int32_t *)(names - 0x68);
                            uint32_t flags;
                            const int32_t *sel_tbl;

                            sel_tbl = (const int32_t *)0x285428;
                            if (g_theme_apply_state != 0)
                                sel_tbl = (const int32_t *)0x28542c;
                            row[0] = (int32_t)i + 0x26020;
                            row[1] = (int32_t)i;
                            row[4] = src_id;
                            flags = 0;
                            if ((int32_t)rec[4] == 0
                                || *(int32_t *)((char *)rec + 0x24) != 0)
                                flags = 0;
                            else if (g_theme_apply_state == 1)
                                flags = 0;
                            else
                                flags = (uint32_t)names[-100] >> 1 & 1;
                            row[2] = (int32_t)((flags & 0xffffffe0u)
                                               | (flags & 7)
                                               | (uint32_t)(src_id
                                                            == *sel_tbl) << 1
                                               | (*(const uint32_t *)
                                                      (names - 100) >> 1 & 3)
                                                     << 3);
                            ui_status_line_format(row + 0x16, (size_t)-1,
                                                  0x60, g_format_1343dc,
                                                  names - 0x60);
                            ui_status_line_format(row + 0x62, (size_t)-1,
                                                  0x60, g_format_1343dc,
                                                  names);
                            ui_status_line_format(row + 0x7a, (size_t)-1,
                                                  0x60, g_format_1343dc,
                                                  names + 0x60);
                            names += 0x128;
                            row += 0x92;
                        }
                        g_menu_state_latch = 0;
                        return 1;
                    }
                }
            }
        } else if (g_menu_open_char == 100) {
            /* debug/about page */
            rec[1] = 3;
            rec[3] = g_debug_slots_b;
            *(int32_t *)((char *)rec + 0x1c) = (int32_t)g_debug_state;
            rec[2] = g_debug_slots_a;
            rec[4] = g_debug_pending;
            *(uint32_t *)((char *)rec + 0x24) = (uint32_t)(g_debug_state != 0);
            rec[5] = 0x28;
            rec[6] = 0x28;
            ui_status_line_format(rec + 0xd, (size_t)-1, 0x80,
                                  g_format_1343dc, g_debug_status_line);
            if (0x100 < (uint32_t)rec[6]) {
                g_menu_state_latch = 0;
                return 0;
            }
            if ((uint32_t)rec[6] != 0) {
                uint32_t i;
                int32_t *row = (int32_t *)(rec + 0x4d);
                const char *const *about = g_about_rows;

                for (i = 0; i < (uint32_t)rec[6]; i++) {
                    int32_t extra = *(const int32_t *)(about + 2);
                    uint32_t base = *(const uint32_t *)(about - 2);
                    uint32_t perm;
                    uint32_t bit = 0;

                    row[0] = (int32_t)i + 0x27020;
                    row[1] = (int32_t)i;
                    row[3] = extra;
                    row[4] = (int32_t)base;
                    if ((int32_t)rec[4] == 0
                        || *(int32_t *)((char *)rec + 0x24) != 0) {
                        perm = 0;
                    } else {
                        if (base < 0x100 && base < g_debug_bit_len)
                            bit = g_debug_bits[base >> 3] >> (base & 0x1f) & 1;
                        perm = (uint32_t)(bit != 0);
                    }
                    row[2] = (int32_t)perm;
                    ui_status_line_format(row + 6, (size_t)-1, 0x40,
                                          g_format_1343dc, about[-1]);
                    ui_status_line_format(row + 0x16, (size_t)-1, 0x60,
                                          g_format_1343dc, about[0]);
                    ui_status_line_format(row + 0x2e, (size_t)-1, 0xd0,
                                          g_format_1343dc, about[1]);
                    if ((perm & 1) == 0) {
                        const char *fallback;

                        if ((int32_t)rec[4] == 0)
                            fallback = "DEBUG PROVIDER UNAVAILABLE";
                        else {
                            fallback = "UNAVAILABLE IN THE CURRENT CONTEXT";
                            if (*(int32_t *)((char *)rec + 0x24) != 0)
                                fallback = "WAITING FOR INPUT OR ACTION";
                        }
                        ui_status_line_format(row + 0x2e, (size_t)-1, 0xd0,
                                              g_format_1343dc, fallback);
                    }
                    if ((perm & 1) == 0 && g_debug_extra != 0
                        && (strcmp(about[-1], "GFX_QUALITY_CYCLE") == 0
                            || strcmp(about[-1], "MEM_QUALITY_CYCLE") == 0))
                        ui_status_line_format(row + 0x2e, (size_t)-1, 0xd0,
                                              g_format_1343dc,
                                              "DISABLE MAX OPTIMIZATION FIRST");
                    about += 5;
                    row += 0x92;
                }
                g_menu_state_latch = 0;
                return 1;
            }
        } else {
            if (g_menu_open_char != 0x51) {
                g_menu_state_latch = 0;
                return 1;
            }
            /* server page */
            rec[1] = 1;
            if (g_theme_store != 0x918 || 0x20 < g_theme_store_count) {
                g_menu_state_latch = 0;
                return 1;
            }
            if ((uint32_t)(g_theme_store_def + 1) < 2) {
                /* raw overflow check on def+1 */
            }
            if (g_theme_store_count != 0) {
                uint32_t i;

                for (i = 0; i < g_theme_store_count; i++) {
                    if (999999 < (uint32_t)g_theme_store_ids[i * 0x12]
                        || memchr(g_theme_store_names + (uint64_t)i * 0x48, 0,
                                  0x40) == NULL)
                        break; /* raw rejects the whole snapshot */
                    {
                        uint32_t j;

                        for (j = 0; j < i; j++)
                            if (g_theme_store_ids[i * 0x12]
                                == g_theme_store_ids[j * 0x12])
                                break;
                        if (j != i)
                            break;
                    }
                }
                if (i != g_theme_store_count) {
                    /* raw falls through to the 1 return */
                }
            }
            rec[7] = (uint64_t)(int64_t)g_theme_store_def;
            rec[5] = g_theme_store_count;
            rec[2] = g_theme_store_sel_id;
            rec[6] = g_theme_store_count;
            *(uint32_t *)(rec + 0xb) = g_theme_store_flag;
            rec[4] = (uint32_t)(*(const int32_t *)0x284afc != 0);
            if (g_theme_store_count != 0) {
                uint32_t i;
                uint64_t *row = rec + 0x58;
                const char *name = g_theme_store_names;

                for (i = 0; i < g_theme_store_count; i++) {
                    *(int32_t *)(row - 0xb) = (int32_t)i + 0x24010;
                    *(int32_t *)((char *)row - 0x54) = (int32_t)i;
                    {
                        uint64_t pair = *(const uint64_t *)(name - 8);

                        row[-9] = pair;
                        *(uint32_t *)(row - 10) =
                            (uint32_t)((int32_t)rec[4] != 0)
                            | (uint32_t)((int32_t)pair
                                         == g_theme_store_def) << 1;
                    }
                    ui_status_line_format(row, (size_t)-1, 0x60,
                                          g_format_1343dc, name);
                    name += 0x48;
                    row += 0x49;
                }
                g_menu_state_latch = 0;
                return 1;
            }
        }
    }
    g_menu_state_latch = 0;
    return 1;
}

/* menu_engine chain chunk 13: covers raw lines 6601-7050 (nexus_menu_section_snapshot tail .. nexus_menu_section_present finished at raw 7030; menu_feature_preset_apply begins at 7032 and straddles — completed here) */

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
#include <fcntl.h>
#include <sys/stat.h>

extern int __android_log_print(int prio, const char *tag, const char *fmt, ...);
pid_t gettid(void);
int pthread_getname_np(pthread_t, char *, size_t);

/* ---- carried declarations (earlier chunks own the definitions) ---- */
extern int ui_latch_test_and_set(int which, void *addr); /* menu_engine p4 @ 00193f80 */
extern void ui_status_line_format(void *buf, size_t a, size_t b,
                                  const char *fmt, ...); /* misc @ 00183ac8 */
extern void theme_store_reset(void *buf, uint32_t size); /* misc @ 00191920 */
extern int settings_values_commit(void *values); /* misc @ 001846b8 */

/* menu state block (earlier chunks own the definitions) */
extern uint32_t g_menu_state_latch;  /* 0x284688 — state test-and-set latch */
extern uint32_t g_menu_owner_id;     /* 0x284694 — owning id (tid gate) */
extern uint8_t  g_menu_battle_flag;  /* 0x28469e — in-battle flag */
extern uint8_t  g_menu_open_char;    /* 0x28469d — open screen char */
extern uint32_t g_menu_generation;   /* 0x2846a0 — state generation */
extern uint8_t  g_menu_visible;      /* 0x28469c — menu visible flag */
extern int32_t  g_menu_settings[0x76]; /* 0x2846b0 — 118 setting slots */

/* feature registry (rodata pointer tables) */
extern const void *const g_feature_names; /* 0x1a36f8 — name/lo/hi triples
                                             ("nexus sx spin" table) */

/* theme machine state (chunk 10/12 externs) */
extern uint32_t g_theme_apply_state;   /* 0x287b90 */
extern uint32_t g_theme_slot_sel;      /* 0x287b94 */
extern uint32_t g_theme_slot_valid;    /* 0x287b98 */
extern uint32_t g_theme_busy;          /* 0x287ba4 */
extern uint32_t g_theme_gen;           /* 0x287bac */
extern uint32_t g_theme_prev_gen;      /* 0x287bb0 */
extern uint32_t g_theme_slots_used;    /* 0x287bb4 */
extern uint32_t g_theme_settings_rev;  /* 0x287c40 */
extern uint32_t g_theme_store_rows;    /* 0x285414 */
extern uint32_t g_theme_store_rev;     /* 0x285420 */
extern uint32_t g_theme_store_cap;     /* 0x285418 */
extern int32_t  g_theme_page_ids[];    /* 0x287bc0 */
extern int32_t  g_theme_page_src_tbl[];   /* 0x28543c */

/* about/debug page blocks (chunk 12 externs) */
extern uint32_t g_debug_slots_a;       /* 0x28a3d8 */
extern uint32_t g_debug_slots_b;       /* 0x28a3dc */
extern uint32_t g_debug_bit_len;       /* 0x28a3e0 */
extern uint32_t g_debug_bits[];        /* 0x28a3e4 */
extern uint32_t g_debug_pending;       /* 0x28a428 */

/* theme store block (raw 0x284af0; chunk 12 externs) */
extern uint64_t g_theme_store;         /* 0x284af0 — 0x918 store buffer */
extern uint32_t g_theme_store_count;   /* 0x284af4 — store count (<= 0x20) */
extern uint32_t g_theme_store_sel_id;  /* 0x284af8 — selected server id */
extern uint32_t g_theme_store_ids[];   /* 0x284b08 — server ids (0x12 stride) */

/* rodata tables */
extern const char *const g_about_rows[0x28 * 5]; /* 0x19cf58 — ABOUT table */
extern const int32_t g_about_ids[0x28];  /* 0x19cf48 — about row ids */
extern const char *const g_dodge_names[6]; /* 0x19e960 — "reaction pct" names */
extern const uint32_t g_outline_presets[7 * 3]; /* 0x14073c — {r,g,b} per
                                                    preset */

static long feature_index_me13(const char *name);
static bool range_ok_me13(long idx, int32_t a, int32_t b);
static bool write_triple_me13(int32_t *values, const char *name, uint32_t v);

/*
 * nexus_menu_section_present — validate that a 0x24a68 snapshot still
 * matches the live section (owner-gated, visible, not in battle). Gates:
 * magic 1, size 0x24a68, row count <= 0x100, the snapshot screen char and
 * generation match the live state. Kind 1 ('Q' server): the selected id,
 * count and every row id must match the theme store; returns 1. Kind 3
 * ('d' debug): requires the about-block ids to match, then rebuilds the
 * 0x28a404 permission bitfield from the snapshot rows (setting the first
 * `count` bits) when the debug pending flag is set. Kind 2 ('t' theme):
 * the store revision and apply state must match; the theme page rows must
 * match the live page table; on match with a loaded slot in state < 2 the
 * committed generation adopts the live one and the slot-selection bitmask
 * is rebuilt from the rows. Returns 1 present, 0 otherwise.
 * @ 0018cd34
 */
int nexus_menu_section_present(void *snapshot_ptr, uint64_t len, int owner_id)
{
    int *snapshot = (int *)snapshot_ptr;
    int kind;
    int i;

    if (snapshot == NULL || len != 0x24a68)
        return 0;
    if (snapshot[0] != 1 || snapshot[1] != 0x24a68 || 0x100 < (uint32_t)snapshot[0xc])
        return 0;
    if ((ui_latch_test_and_set(1, &g_menu_state_latch) & 1) != 0)
        return 0;
    if (owner_id < 1 || (int)g_menu_owner_id != owner_id
        || g_menu_visible == 0 || g_menu_battle_flag != 0) {
        g_menu_state_latch = 0;
        return 0;
    }

    if ((uint32_t)snapshot[5] != (uint32_t)g_menu_open_char
        || g_menu_generation != (uint32_t)snapshot[3]) {
        g_menu_state_latch = 0;
        return 0;
    }
    kind = snapshot[2];
    i = 0x2400f;

    if (kind == 1) {
        /* server page: ids and count must match the store */
        if (g_menu_open_char == 0x51
            && (uint32_t)snapshot[4] == g_theme_store_sel_id
            && (uint32_t)snapshot[0xc] == g_theme_store_count) {
            uint64_t left = (uint64_t)(uint32_t)snapshot[0xc];
            int *row = snapshot + 0x9e; /* first row (0x92 stride) */
            const uint32_t *store_id = g_theme_store_ids;

            while (left != 0) {
                i = i + 1;
                if (i != row[-4])
                    break;
                if (*row != (int)*store_id) {
                    g_menu_state_latch = 0;
                    return 0;
                }
                row += 0x92;
                left = left - 1;
                store_id += 0x12;
            }
            if (left == 0) {
                g_menu_state_latch = 0;
                return 1;
            }
        }
    } else if (kind == 3) {
        /* debug page: about-block ids must match, then rebuild the
         * permission bitfield from the rows when pending */
        if (g_menu_open_char == 100
            && g_debug_slots_a == (uint32_t)snapshot[4]
            && snapshot[0xc] == 0x28) {
            uint64_t idx;
            int *row = snapshot + 0x9a;
            const int32_t *about_id = g_about_ids;

            for (idx = 0; (uint32_t)snapshot[0xc] != idx; idx++) {
                if (idx != (uint64_t)(uint32_t)row[1]) {
                    g_menu_state_latch = 0;
                    return 0;
                }
                if ((int)idx + 0x2701f != *row) {
                    g_menu_state_latch = 0;
                    return 0;
                }
                if (row[4] != *about_id) {
                    g_menu_state_latch = 0;
                    return 0;
                }
                row += 0x92;
                about_id += 10; /* raw 0x28-stride walk of the id table */
            }
            if (g_debug_pending == 0) {
                g_menu_state_latch = 0;
                return 1;
            }
            /* rebuild the 0x28a404 bitfield: bits 0..count-1 set */
            memset((void *)0x28a404, 0, 0x1c);
            if (snapshot[0xc] != 0) {
                uint32_t bit;

                for (bit = 0; bit < (uint32_t)snapshot[0xc]; bit++)
                    ((uint32_t *)0x28a404)[bit >> 5] |= 1u << (bit & 0x1f);
            }
            g_menu_state_latch = 0;
            return 1;
        }
    } else if (kind == 2 && g_menu_open_char == 0x74
               && g_theme_store_rows == (uint32_t)snapshot[4]
               && g_theme_apply_state == (uint32_t)snapshot[7]) {
        /* theme page: rows must match the live page table */
        int expect_rows = (int)g_theme_store_rows;

        if (1 < g_theme_apply_state || g_theme_slot_valid == 0)
            expect_rows = 0;
        if (snapshot[0xc] != expect_rows) {
            g_menu_state_latch = 0;
            return 0;
        }
        {
            uint64_t idx;
            int *row = snapshot + 0x9a;
            const int32_t *src = g_theme_page_src_tbl;

            for (idx = 0; (uint32_t)snapshot[0xc] != idx; idx++) {
                if (idx != (uint64_t)(uint32_t)row[1]
                    || (int)idx + 0x2601f != *row) {
                    g_menu_state_latch = 0;
                    return 0;
                }
                if (row[4] != *src) {
                    g_menu_state_latch = 0;
                    return 0;
                }
                row += 0x92;
                src += 0x4a;
            }
            if (g_theme_slot_valid == 0) {
                g_menu_state_latch = 0;
                return 1;
            }
            if (g_theme_apply_state < 2) {
                /* adopt the committed generation + slot bitmask */
                g_theme_prev_gen = g_theme_gen;
                if (snapshot[0xc] != 0) {
                    uint64_t j;
                    int *sel_row = snapshot + 0x9e;

                    g_theme_slots_used = 0;
                    for (j = 0; j < (uint64_t)(uint32_t)snapshot[0xc]; j++) {
                        g_theme_page_ids[j] = *sel_row;
                        g_theme_slots_used |= 1u << ((uint32_t)j & 0x1f);
                        sel_row += 0x92;
                    }
                } else {
                    g_theme_slots_used = 0;
                }
            }
            g_menu_state_latch = 0;
            return 1;
        }
    }
    g_menu_state_latch = 0;
    return 0;
}

/* ===== feature preset apply ===== */

/*
 * menu_feature_preset_apply — apply a feature preset (id-keyed) to the
 * 118-slot settings cache and commit it. Copies the cache (0x1d8 bytes)
 * then, per preset id: 0x40 'target mode' zeroes nexus_sx_aop_target_mode
 * and cascades (aura range 0x1068, fire interval 1000, reaction 0, lead
 * 100); 0x4a 'autofarm follow' toggles nexus_autofarm_follow_target;
 * 0x5c 'autododge' version-gates the six nexus[_vN]_dodge_*_reaction_pct
 * slots (0xb4 default, slot 5 = 0x424, others 100), zeroes
 * nexus_autododge_require_hold and sets the 0x3ff blacklist mask;
 * 0x70 'outline color' cycles nexus_sx_outline_color_preset
 * ((v+1)%7) and writes the matching {r,g,b} preset triple from
 * 0x14073c; 0x91 'bolt smooth' pins nexus_sx_bolt_smooth to 0x55, wall
 * lookahead 500, target range 0x578, exit hold 0x28a. Every slot write
 * is range-checked against the registry {lo, hi} (walk of the 0x1a36f8
 * name/lo/hi table); on success settings_values_commit pushes the cache.
 * Returns the commit verdict (4 on any range failure / unknown id).
 * @ 0018d64c
 */
void menu_feature_preset_apply(int preset_id)
{
    int32_t values[0x76];
    uint64_t verdict = 4;

    memcpy(values, (const void *)g_menu_settings, 0x1d8);

    if (preset_id < 0x5c) {
        if (preset_id == 0x40) {
            /* target-mode preset */
            long idx = feature_index_me13("nexus_sx_aop_target_mode");

            if (idx >= 0 && range_ok_me13(idx, 1, 0)) {
                long aura = feature_index_me13("nexus_sx_combat_aura_range");

                values[idx] = 0;
                if (aura >= 0 && range_ok_me13(aura, 0x1068, 0x1068)) {
                    long fire =
                        feature_index_me13("nexus_sx_combat_fire_interval");

                    values[aura] = 0x1068;
                    if (fire >= 0 && range_ok_me13(fire, 1000, 1000)) {
                        long react =
                            feature_index_me13("nexus_sx_aop_reaction_ms");

                        values[fire] = 1000;
                        if (react >= 0 && range_ok_me13(react, 0, 0)) {
                            long lead =
                                feature_index_me13("nexus_sx_aop_lead_scale");

                            values[react] = 0;
                            if (lead >= 0
                                && range_ok_me13(lead, 100, 100)) {
                                values[lead] = 100;
                                verdict = (uint64_t)settings_values_commit(values);
                            }
                        }
                    }
                }
            }
        } else if (preset_id == 0x4a) {
            /* autofarm follow toggle */
            long idx = feature_index_me13("nexus_autofarm_follow_target");

            if (idx >= 0) {
                uint32_t newv = (uint32_t)(values[idx] == 0);

                if (range_ok_me13(idx, (int32_t)newv, (int32_t)newv)) {
                    values[idx] = (int32_t)newv;
                    verdict = (uint64_t)settings_values_commit(values);
                }
            }
        }
    } else if (preset_id == 0x5c) {
        /* autododge preset */
        long idx = feature_index_me13("nexus_autododge_version");

        if (idx >= 0) {
            int version = *(int *)((char *)g_menu_settings + idx * 4);

            if ((uint32_t)(version - 6) > 0xfffffffa)
                version = 3;
            {
                long slot;

                for (slot = 0; slot < 6; slot++) {
                    char name[0x60];
                    uint32_t cap = slot != 5 ? 100 : 0x424;
                    uint32_t def = slot != 0 ? cap : 0xb4;
                    long target;

                    if (version == 1)
                        ui_status_line_format(name, 0x60, 0x60,
                                              "nexus_dodge_%s",
                                              g_dodge_names[slot]);
                    else
                        ui_status_line_format(name, 0x60, 0x60,
                                              "nexus_v%d_dodge_%s", version,
                                              g_dodge_names[slot]);
                    target = feature_index_me13(name);
                    if (target < 0
                        || !range_ok_me13(target, (int32_t)def,
                                          (int32_t)def))
                        goto out;
                    values[target] = (int32_t)def;
                }
                {
                    long hold =
                        feature_index_me13("nexus_autododge_require_hold");

                    if (hold >= 0 && range_ok_me13(hold, 0, 0)) {
                        long mask =
                            feature_index_me13("nexus_dodge_blacklist_mask");

                        values[hold] = 0;
                        if (mask >= 0 && range_ok_me13(mask, 0x3ff, 0x3ff)) {
                            values[mask] = 0x3ff;
                            verdict = (uint64_t)settings_values_commit(values);
                        }
                    }
                }
            }
        }
    } else if (preset_id == 0x70) {
        /* outline color preset cycle */
        long idx = feature_index_me13("nexus_sx_outline_color_preset");

        if (idx >= 0) {
            uint32_t preset = (uint32_t)(values[idx] + 1) % 7;

            if (range_ok_me13(idx, (int32_t)preset, (int32_t)preset)) {
                const uint32_t *rgb = g_outline_presets + preset * 3;

                if (write_triple_me13(values, "nexus_sx_outline_color_preset",
                                      preset)
                    && write_triple_me13(values, "nexus_sx_outline_r", rgb[0])
                    && write_triple_me13(values, "nexus_sx_outline_g", rgb[1])
                    && write_triple_me13(values, "nexus_sx_outline_b", rgb[2]))
                    verdict = (uint64_t)settings_values_commit(values);
            }
        }
    } else if (preset_id == 0x91) {
        /* bolt-smooth preset */
        long idx = feature_index_me13("nexus_sx_bolt_smooth");

        if (idx >= 0 && range_ok_me13(idx, 0x56, 0x54)) {
            long look =
                feature_index_me13("nexus_sx_bolt_wall_lookahead");

            values[idx] = 0x55;
            if (look >= 0 && range_ok_me13(look, 500, 500)) {
                long range =
                    feature_index_me13("nexus_sx_bolt_target_range");

                values[look] = 500;
                if (range >= 0 && range_ok_me13(range, 0x578, 0x578)) {
                    long hold = feature_index_me13("nexus_sx_bolt_exit_hold");

                    values[range] = 0x578;
                    if (hold >= 0 && range_ok_me13(hold, 0x28a, 0x28a)) {
                        values[hold] = 0x28a;
                        verdict = (uint64_t)settings_values_commit(values);
                    }
                }
            }
        }
    }

out:
    (void)verdict;
}

/*
 * feature_index_me13 — index of the named feature in the rodata registry
 * (0x1a36f8 name/lo/hi triples, 0x18-byte stride walked as {name, lo, hi});
 * -1 when not found. @ inline (raw 0x1a36f8 table walk)
 */
static long feature_index_me13(const char *name)
{
    const char *const *tbl = (const char *const *)&g_feature_names;
    long i;

    for (i = 0; i < 0x76; i++) {
        if (strcmp(tbl[i], name) == 0)
            return i;
        tbl += 3; /* raw walks piVar9 += 6 ints = 3 qwords */
    }
    return -1;
}

/*
 * range_ok_me13 — true when the feature's {lo, hi} range covers [a, b]
 * (raw checks lo <= b && a <= hi, with the slot index validated).
 * @ inline
 */
static bool range_ok_me13(long idx, int32_t a, int32_t b)
{
    const int32_t *triple;

    if (idx < 0)
        return false;
    triple = (const int32_t *)((const uint8_t *)&g_feature_names
                               + (uint64_t)idx * 0x18);
    /* triple[3] = lo, triple[4] = hi relative to the name pointer */
    {
        const int32_t *rec = (const int32_t *)((const uint8_t *)&g_feature_names
                                               + (uint64_t)idx * 0x18);

        (void)triple;
        return rec[3] <= b && a <= rec[4];
    }
}

/* write one feature slot when the value fits its range; false otherwise */
static bool write_triple_me13(int32_t *values, const char *name, uint32_t v)
{
    long idx = feature_index_me13(name);

    if (idx < 0 || !range_ok_me13(idx, (int32_t)v, (int32_t)v))
        return false;
    values[idx] = (int32_t)v;
    return true;
}
/* menu_engine chain chunk 14: covers raw lines 7414-7900 (p4 begins: engine register/observe, stopped log, quick header render; button pool init straddles into 7896+ and is finished in chunk 15) */

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
#include <fcntl.h>
#include <sys/stat.h>

extern int __android_log_print(int prio, const char *tag, const char *fmt, ...);
pid_t gettid(void);
int pthread_getname_np(pthread_t, char *, size_t);

/* ---- carried declarations (earlier chunks own the definitions) ---- */
extern int ui_latch_test_and_set(int which, void *addr); /* p4 @ 00193f80 (below) */
extern uint64_t nexus_menu_status(void);   /* @ 00183d70 */
extern uint64_t nexus_menu_scroll_battle(int dir, int tid); /* @ 00184900 */
extern uint32_t nexus_menu_dispatch(uint32_t op, int a, int b, int tid); /* @ 00194730 */
extern void nexus_menu_view(void *view_out, int owner_id);  /* @ 001856f8 */
extern uint64_t nexus_menu_setting_value(uint32_t id, int32_t *value_out); /* @ 001841f0 */
extern int theme_menu_status_set(int code, int tid, const char *reason); /* misc @ 00183cdc */
extern int channel2_context_cache_validate(void);  /* misc @ 0018e668 */
extern uint64_t channel2_read_ptr(uint64_t addr);  /* misc @ 0018edcc */
extern int channel2_ancestor_chain_check(uint64_t node, uint64_t target); /* misc @ 0018ee70 */
extern int channel2_child_link_valid(uint64_t node, uint64_t parent); /* misc @ 0018f750 */
extern int menu_button_label_set(int slot, const char *text); /* widgets @ 0018f988 */
extern void text_vformat_128(void *buf);            /* misc @ 0018fb04 */
extern int channel2_root_node_check(uint64_t node); /* misc @ 0018fba0 */
extern int menu_quick_header_render(float w, float h); /* @ 0018e880 (below) */
extern uint64_t menu_button_pool_init(void);        /* @ 0018ef84 (chunk 15) */

/* engine ops block (raw 0x28fac0..0x28fb38, filled by register) */
extern uint64_t g_engine_ctx;        /* 0x28fac8 — engine ctx handle */
extern uint64_t g_engine_remote;     /* 0x28fad0 — remote-read bias */
extern uint64_t g_engine_read_mem;   /* 0x28fae0 — read_mem fn */
extern uint64_t g_engine_alloc;      /* 0x28fae8 — alloc fn */
extern uint64_t g_engine_ctor_a;     /* 0x28faf0 — ctor a */
extern uint64_t g_engine_ctor_b;     /* 0x28faf8 — ctor b */
extern uint64_t g_engine_movie_load; /* 0x28fb00 — movie load fn */
extern uint64_t g_engine_movie_bind; /* 0x28fb08 — movie bind fn */
extern uint64_t g_engine_frame_set;  /* 0x28fb10 — frame set fn */
extern uint64_t g_engine_stage_fn;   /* 0x28fb18 — stage fn */
extern uint64_t g_engine_pos_fn;     /* 0x28fb20 — set-position fn */
extern uint64_t g_engine_add_child;  /* 0x28fb28 — add-child fn */
extern uint64_t g_engine_remove_child; /* 0x28fb30 — remove-child fn */
extern uint64_t g_engine_log_fn;     /* 0x28fb38 — engine log sink */
extern uint32_t g_engine_log_count;  /* 0x28fb54 — capped-0x80 counter */
extern uint32_t g_engine_latch;      /* 0x28fb40 — register/observe latch */
extern uint8_t  g_engine_registered; /* 0x28fb44 — ops installed */
extern int32_t  g_engine_state;      /* 0x28fb48 — observe state (-1..2) */
extern uint32_t g_engine_owner_id;   /* 0x28fb4c — owning tid */
extern uint32_t g_engine_probe_count; /* 0x28fb50 — asset probe count */
extern uint32_t g_engine_click_count; /* 0x28fb54 (shared w/ log cap) */
extern uint64_t g_engine_asset_start; /* 0x28fb60 — asset window start */
extern uint64_t g_engine_next_poll;  /* 0x28fb68 — next poll deadline */
extern uint64_t g_engine_stage_root; /* 0x28fb70 — cached stage root */
extern uint64_t g_engine_stage_view; /* 0x28fb78 — cached stage view */
extern uint64_t g_engine_buttons[0x18]; /* 0x28fb80 — 24 button handles */
extern uint8_t  g_engine_button_live[0x18]; /* 0x28fca0 — live flags */
extern uint32_t g_engine_button_ops[0x18];  /* 0x28fc40 — button opcodes */
extern uint64_t g_engine_view_rev;   /* 0x28fcb8 — last view generation */

/* raw float caches for the render (0x28fd80/0x28fde0 blocks) */
extern float g_engine_pos_cache[0x18]; /* 0x28fd80 — last set x */
extern float g_engine_pos_cache_y[0x18]; /* 0x28fde0 — last set y */
extern uint8_t g_engine_pos_valid[0x19]; /* 0x28fe40 — position-valid flags */

/* engine ops-block layout (raw record, 0x80 bytes) */
typedef struct {
    uint64_t magic;        /* +0x00 — {1, 0x80} */
    uint64_t size;         /* +0x08 — >= 0x1000 */
    uint64_t remote_bias;  /* +0x10 (raw +4 qword = remote bias) */
    uint64_t ctx;          /* +0x18 */
    uint64_t read_mem;     /* +0x18 */
    uint64_t alloc;        /* +0x20 */
    uint64_t ctor_a;       /* +0x28 */
    uint64_t ctor_b;       /* +0x30 */
    uint64_t movie_load;   /* +0x38 */
    uint64_t movie_bind;   /* +0x40 */
    uint64_t frame_set;    /* +0x48 */
    uint64_t stage_fn;     /* +0x50 */
    uint64_t pos_fn;       /* +0x58 */
    uint64_t add_child;    /* +0x60 */
    uint64_t remove_child; /* +0x68 */
    uint64_t log_fn;       /* +0x70 */
    const char *build_hash; /* +0x30 (raw +6 ints) — must equal the pinned
                               sha "a10aeb6b...ede3" */
} engine_ops_t;

static void menu_stopped_log(const char *reason);

/*
 * nexus_menu_engine_register — install the engine ops block (one-shot,
 * under the 0x28fb40 latch). The 0x80-byte block must carry {1, 0x80}, a
 * build hash equal to
 * "a10aeb6b4085fb2a15d969a130cd9608231998b5269a40aed41129d99624ede3",
 * size >= 0x1000 and non-null fns at +0x20/+0x28/+0x30/+0x38/+0x40/+0x48/
 * +0x50/+0x58/+0x60/+0x68/+0x70. On the first successful registration the
 * whole block is copied to 0x28fac0 (14 fn/ctx qwords), the registered
 * flag set and 'waiting_rooted_mainloop_text' published. Returns 1
 * installed, 4 bad block/already registered, 3 busy. @ 0018e174
 */
uint64_t nexus_menu_engine_register(int *block)
{
    engine_ops_t *ops = (engine_ops_t *)(void *)block;

    if (block == NULL || ops->magic != ((uint64_t)1 << 32 | 0x80))
        return 4;
    if (ops->size < 0x1000)
        return 4;
    if (ops->build_hash == NULL
        || strcmp(ops->build_hash,
                  "a10aeb6b4085fb2a15d969a130cd9608231998b5269a40aed41129d99624ede3")
               != 0)
        return 4;
    if (ops->alloc == 0 || ops->ctor_a == 0)
        return 4;
    if (ops->ctor_b == 0)
        return 4;
    if (ops->movie_load == 0 || ops->movie_bind == 0
        || ops->frame_set == 0 || ops->stage_fn == 0
        || ops->pos_fn == 0 || ops->add_child == 0
        || ops->remove_child == 0 || ops->log_fn == 0)
        return 4;

    if ((ui_latch_test_and_set(1, &g_engine_latch) & 1) != 0)
        return 3;
    if (g_engine_registered != 0) {
        g_engine_latch = 0;
        return 4;
    }

    g_engine_alloc = ops->alloc;
    g_engine_read_mem = ops->read_mem;
    g_engine_ctor_b = ops->ctor_b;
    g_engine_ctor_a = ops->ctor_a;
    g_engine_ctx = ops->ctx;
    g_engine_remote = ops->remote_bias;
    g_engine_pos_fn = ops->pos_fn;
    g_engine_add_child = ops->add_child;
    g_engine_remove_child = ops->remove_child;
    g_engine_movie_bind = ops->movie_bind;
    g_engine_movie_load = ops->movie_load;
    g_engine_stage_fn = ops->stage_fn;
    g_engine_frame_set = ops->frame_set;
    g_engine_log_fn = ops->log_fn;
    g_engine_registered = 1;
    theme_menu_status_set(2, 0, "waiting_rooted_mainloop_text");
    g_engine_latch = 0;
    return 1;
}

/*
 * nexus_menu_engine_observe — per-frame engine observation pump (the
 * rooted-mainloop text path). Gates: label "Mainloop", tid >= 1, phase <= 1,
 * engine registered, state >= 0, owner match, finite w/h with w >= 960,
 * h >= 560, both <= 8192, menu status non-zero. State 2 (attached): the
 * poll throttle (>= param_7) guards channel2_context_cache_validate; a
 * click phase (1) walks the 24 button slots and dispatches the live one
 * (opcodes 0x20000/0x20001 -> scroll battle -1/+1, else
 * nexus_menu_dispatch with 'nexus_menu_click' acknowledged/pending/blocked
 * logged), then menu_quick_header_render renders the frame. State < 2
 * (attaching): the stage identity chain (bias+0x12eb9f0 root, +0x90 view,
 * frame object == bias+0x11abad8, ancestor check) with the 500ms asset
 * probe ('popover_button_blue', probe budget 0x1f / 0x752 window, owner
 * adoption, status -1 -> 'asset_readiness_budget' stop), then
 * menu_button_pool_init builds the 24 buttons. Failures route through
 * menu_stopped_log. Returns 1 rendered, 2 waiting, 3 busy, 0 gated,
 * 0xffffffff stopped. @ 0018e2b4
 */
uint64_t nexus_menu_engine_observe(float w, float h, uint32_t phase,
                                   long frame_obj, int tid, char *thread_name,
                                   uint64_t now_ms)
{
    if (thread_name == NULL || tid < 1 || 1 < phase)
        return 0;
    if (g_engine_registered == 0 || (int32_t)g_engine_state < 0)
        return 0;
    if (strcmp(thread_name, "Mainloop") != 0)
        return 0;
    if (!isfinite(w))
        return 0;
    if (g_engine_owner_id != 0 && g_engine_owner_id != (uint32_t)tid)
        return 0;
    if (h != 8192.0f && !(h < 8192.0f))
        return 0;
    if (w != 8192.0f && !(w < 8192.0f))
        return 0;
    if (h < 560.0f || w < 960.0f)
        return 0;
    if (!isfinite(h))
        return 0;
    if ((int) nexus_menu_status() == 0)
        return 0;
    if ((ui_latch_test_and_set(1, &g_engine_latch) & 1) != 0)
        return 3;

    if (g_engine_state == 2) {
        /* attached: click dispatch + render */
        if (phase == 0 && now_ms < g_engine_next_poll) {
            g_engine_latch = 0;
            return 1;
        }
        g_engine_next_poll = now_ms + 100;
        if (channel2_context_cache_validate() != 0) {
            if (phase == 1) {
                long i;

                for (i = 0; i < 0x18; i++) {
                    if (g_engine_button_live[i] != 0
                        && g_engine_buttons[i] == (uint64_t)frame_obj) {
                        uint32_t op = g_engine_button_ops[i];
                        int verdict;

                        if ((op & 0xfffffffe) == 0x20000)
                            verdict = (int)nexus_menu_scroll_battle(
                                op == 0x20001 ? 1 : -1, tid);
                        else
                            verdict = (int)nexus_menu_dispatch(op, 0, 0, tid);
                        if (g_engine_click_count < 0x80
                            && g_engine_log_fn != 0) {
                            const char *reason =
                                verdict == 2 ? "pending" : "blocked";
                            const char *final =
                                verdict == 1 ? "acknowledged" : reason;

                            ((void (*)(uint64_t, const char *, const char *,
                                       uint32_t))g_engine_log_fn)(
                                g_engine_ctx, "nexus_menu_click", final,
                                g_engine_button_ops[i]);
                        }
                        g_engine_click_count = g_engine_click_count + 1;
                        break;
                    }
                }
            }
            {
                uint64_t verdict = (uint64_t)menu_quick_header_render(w, h);

                g_engine_latch = 0;
                return verdict;
            }
        }
        menu_stopped_log("stage_generation_or_membership");
        g_engine_latch = 0;
        return 0xffffffff;
    }

    /* attaching: stage identity + asset probe + button pool */
    {
        uint64_t stage_root = channel2_read_ptr(g_engine_remote + 0x12eb9f0);
        uint64_t stage_view = 0;
        uint64_t frame_check = 0;

        if (stage_root != 0
            && (stage_view = channel2_read_ptr(stage_root + 0x90)) != 0
            && (frame_check = channel2_read_ptr((uint64_t)frame_obj))
                   == g_engine_remote + 0x11abad8
            && channel2_ancestor_chain_check((uint64_t)frame_obj,
                                             stage_view) != 0) {
            if (g_engine_asset_start == 0)
                g_engine_asset_start = now_ms;
            if (g_engine_next_poll <= now_ms) {
                if (0x1f < g_engine_probe_count
                    || (now_ms - g_engine_asset_start) >> 5 > 0x752) {
                    menu_stopped_log("asset_readiness_budget");
                    g_engine_latch = 0;
                    return 0xffffffff;
                }
                g_engine_probe_count = g_engine_probe_count + 1;
                g_engine_next_poll = now_ms + 500;
                {
                    /* engine alloc('popover_button_blue') probe */
                    long probe = (long)((long (*)(uint64_t, const char *))
                                            g_engine_alloc)(
                        g_engine_ctx, "popover_button_blue");

                    if (probe != 0) {
                        extern int ui_thread_adopt(int tid); /* misc @ 00183c78 */

                        g_engine_owner_id = (uint32_t)tid;
                        if (ui_thread_adopt(tid) == 0) {
                            g_engine_latch = 0;
                            g_engine_owner_id = 0;
                            return 3;
                        }
                        g_engine_state = 1;
                        g_engine_stage_root = stage_root;
                        g_engine_stage_view = stage_view;
                        {
                            uint64_t verdict = menu_button_pool_init();

                            if ((int)verdict != 1) {
                                g_engine_latch = 0;
                                return verdict;
                            }
                            verdict = (uint64_t)menu_quick_header_render(w, h);
                            g_engine_latch = 0;
                            return verdict;
                        }
                    }
                }
            }
        }
        g_engine_latch = 0;
        return 2;
    }
}

/*
 * menu_stopped_log — mark the engine stopped: state -> -1, publish the
 * theme status (-1, owner, reason) and log 'nexus_menu_stopped' through
 * the engine sink (capped 0x80). @ 0018e804
 */
void menu_stopped_log(const char *reason)
{
    g_engine_state = 0xffffffff;
    theme_menu_status_set(-1, (int)g_engine_owner_id, reason);
    {
        uint32_t prev = g_engine_click_count;

        g_engine_click_count = g_engine_click_count + 1;
        if (prev < 0x80 && g_engine_log_fn != 0)
            ((void (*)(uint64_t, const char *, const char *, uint32_t))
                 g_engine_log_fn)(g_engine_ctx, "nexus_menu_stopped",
                                  reason, 0);
    }
}

/*
 * menu_quick_header_render — render the quick-menu header via
 * nexus_menu_view (owner = engine owner id). Row cap 0x17 (else
 * 'view_capacity' stop). Reads the quick-menu offset settings
 * (nexus_quick_menu_offset_x/y through the settings-name table),
 * positions the NEXUS badge (y + 90 / x + 40 from the offsets) and the
 * NEXUS MENU title (w*0.5 + 92.0 raw fmadd, (h - 600.0) * 0.5, shifted by
 * +100/-370 when in battle), then syncs the 23 view rows: hidden or
 * off-position rows park at 0xc61c3c00, live rows (row slot bitmap kinds
 * 0x3/0x5/0x8 with text_vformat_128 labels) move to title + row offset,
 * each through the engine set-position fn with the last-position caches.
 * Returns 1 rendered, 0xffffffff on label/capacity failure, 3 busy.
 * @ 0018e880
 */
int menu_quick_header_render(float w, float h)
{
    struct {
        uint32_t generation;   /* +0x00 */
        uint32_t row_count;    /* +0x04 */
        uint8_t flag;          /* +0x08 */
        char screen;           /* +0x09 */
        uint8_t battle;        /* +0x0a */
        uint8_t battle_row;    /* +0x0b */
        uint32_t rows[0x9a * 4]; /* 0x690 body: 0x40-stride rows */
    } view;
    uint64_t offsets = 0;

    nexus_menu_view(&view, (int)g_engine_owner_id); /* raw drops the verdict */
    if (view.row_count < 0x17) {
        const char **names;
        uint64_t count = 0;

        /* read the quick-menu offsets by setting name */
        {
            extern const char **nexus_menu_settings(uint64_t *count_out); /* below */

            names = nexus_menu_settings(&count);
        }
        if (count != 0) {
            uint64_t i;

            for (i = 0; i < count; i++) {
                if (strcmp(names[i], "nexus_quick_menu_offset_x") == 0)
                    nexus_menu_setting_value((uint32_t)i,
                                             (int32_t *)&offsets + 1);
                if (strcmp(names[i], "nexus_quick_menu_offset_y") == 0)
                    nexus_menu_setting_value((uint32_t)i,
                                             (int32_t *)&offsets);
                names += 3; /* raw walks 3-qword triples */
            }
        }

        g_engine_button_ops[0] = view.battle != 0 ? 0x82 : 1;
        g_engine_button_live[0] = 1;
        if (menu_button_label_set(0, "NEXUS") == 1) {
            float badge_y = (float)(offsets >> 32) + 90.0f;
            float badge_x = (float)(uint32_t)offsets + 40.0f;

            if (g_engine_pos_valid[0] == 0
                || g_engine_pos_cache[0] != badge_y
                || g_engine_pos_cache_y[0] != badge_x) {
                ((void (*)(float, float, uint64_t, uint64_t))g_engine_pos_fn)(
                    badge_y, badge_x, g_engine_ctx, g_engine_buttons[0]);
                g_engine_pos_valid[0] = 1;
                g_engine_pos_cache[0] = badge_y;
                g_engine_pos_cache_y[0] = badge_x;
            }
            g_engine_button_ops[1] = 0;
            g_engine_button_live[1] = 0;
            if (menu_button_label_set(1, "NEXUS MENU") == 1) {
                float title_x = w * 0.5f + 92.0f; /* raw fmadd 0x3f000000/0x42b40000 */
                float title_y = (h - 600.0f) * 0.5f;
                float park_y = -9999.0f;
                float park_x = -9999.0f;

                if (view.battle != 0) {
                    park_y = title_y + 100.0f;
                    park_x = title_x - 370.0f;
                }
                if (g_engine_pos_valid[1] == 0
                    || g_engine_pos_cache[1] != park_x
                    || g_engine_pos_cache_y[1] != park_y) {
                    ((void (*)(float, float, uint64_t, uint64_t))
                        g_engine_pos_fn)(park_x, park_y, g_engine_ctx,
                                         g_engine_buttons[1]);
                    g_engine_pos_valid[1] = 1;
                    g_engine_pos_cache[1] = park_x;
                    g_engine_pos_cache_y[1] = park_y;
                }
                {
                    long row;
                    long x_off = 0x1e2; /* button slot base */
                    long pos_idx = 0x188;
                    long btn_idx = 0xd0;

                    for (row = 0; row < 0x17; row++) {
                        g_engine_button_live[x_off] = 0;
                        g_engine_pos_cache[pos_idx >> 2] = 0;
                        if (view.battle == 0
                            || (uint32_t)view.row_count
                                   <= (uint32_t)(row - 0x1e2 + 0x1e2)) {
                            /* hidden/off rows: park at the force color */
                            if (g_engine_pos_valid[x_off] == 0
                                || g_engine_pos_cache[pos_idx >> 2] != -9999.0f
                                || g_engine_pos_cache_y[pos_idx >> 2]
                                       != -9999.0f) {
                                ((void (*)(float, float, uint64_t, uint64_t))
                                    g_engine_pos_fn)(
                                    0xc61c3c00, 0xc61c3c00, g_engine_ctx,
                                    g_engine_buttons[btn_idx >> 3]);
                                g_engine_pos_cache[pos_idx >> 2] =
                                    0xc61c3c00;
                                g_engine_pos_cache_y[pos_idx >> 2] =
                                    0xc61c3c00;
                                g_engine_pos_valid[x_off] = 1;
                                goto next_row;
                            }
                        } else {
                            /* live row: format the label and move it */
                            char label[0x80];
                            uint8_t kind = ((uint8_t *)&view)
                                [0x10 + (uint64_t)(row - 0x1e2) * 0x40
                                 + 0x6c8];

                            if (kind < 8) {
                                uint32_t mask = 1u << (kind & 0x1f);

                                if ((mask & 0xd8) != 0
                                    || (mask & 0x24) != 0)
                                    text_vformat_128(label);
                                else
                                    text_vformat_128(label);
                            } else {
                                text_vformat_128(label);
                            }
                            if (menu_button_label_set(
                                    (int)row - 0x1e0, label) != 1)
                                return 0xffffffff;
                            {
                                uint32_t src = view.rows[(uint64_t)row * 4
                                                         + 0x1a];
                                float ry = *(float *)&src;
                                float rx = *(float *)((uint8_t *)&view
                                                     + 0x10 + (uint64_t)row
                                                           * 0x40 + 0x14);
                                float x = title_x + rx;
                                float y = title_y + ry;

                                g_engine_button_live[x_off] = 1;
                                if (g_engine_pos_valid[x_off] == 0
                                    || g_engine_pos_cache[pos_idx >> 2] != x
                                    || g_engine_pos_cache_y[pos_idx >> 2]
                                           != y) {
                                    ((void (*)(float, float, uint64_t,
                                               uint64_t))g_engine_pos_fn)(
                                        x, y, g_engine_ctx,
                                        g_engine_buttons[btn_idx >> 3]);
                                    g_engine_pos_cache[pos_idx >> 2] = x;
                                    g_engine_pos_cache_y[pos_idx >> 2] = y;
                                    g_engine_pos_valid[x_off] = 1;
                                }
                            }
                        }
next_row:
                        x_off = x_off + 1;
                        pos_idx = pos_idx + 4;
                        btn_idx = btn_idx + 8;
                    }
                }
                g_engine_view_rev = view.generation;
                return 1;
            }
            return 0xffffffff;
        }
        return 0xffffffff;
    }

    /* capacity exceeded: stop */
    g_engine_state = 0xffffffff;
    theme_menu_status_set(-1, (int)g_engine_owner_id, "view_capacity");
    {
        uint32_t prev = g_engine_click_count;

        g_engine_click_count = prev + 1;
        if (prev < 0x80 && g_engine_log_fn != 0)
            ((void (*)(uint64_t, const char *, const char *, uint32_t))
                 g_engine_log_fn)(g_engine_ctx, "nexus_menu_stopped",
                                  "view_capacity", 0);
    }
    return 0xffffffff;
}

/*
 * nexus_menu_actions — table of the 0xa8 action ids (rodata 0x19eab8);
 * *count_out (when non-null) receives 0xa8. @ 0018fde4
 */
const void *nexus_menu_actions(uint64_t *count_out)
{
    if (count_out != NULL)
        *count_out = 0xa8;
    return (const void *)(uintptr_t)0x19eab8;
}

/*
 * nexus_menu_settings — table of the 0x76 setting-name triples (rodata
 * 0x1a19f8); *count_out (when non-null) receives 0x76. @ 0018fdfc
 */
const char **nexus_menu_settings(uint64_t *count_out)
{
    if (count_out != NULL)
        *count_out = 0x76;
    return (const char **)(uintptr_t)0x1a19f8;
}

/*
 * nexus_menu_cells — table of the 0x9a cells (rodata 0x1a2508);
 * *count_out receives 0x9a. @ 0018fe14
 */
const void *nexus_menu_cells(uint64_t *count_out)
{
    if (count_out != NULL)
        *count_out = 0x9a;
    return (const void *)(uintptr_t)0x1a2508;
}

/*
 * nexus_menu_tabs — table of the 6 tabs (rodata 0x1a2ea8);
 * *count_out receives 6. @ 0018fe2c
 */
const void *nexus_menu_tabs(uint64_t *count_out)
{
    if (count_out != NULL)
        *count_out = 6;
    return (const void *)(uintptr_t)0x1a2ea8;
}

/*
 * nexus_script_port_ui_server_thread — script-port veneer: syscall 0xb2
 * (gettid) then forward to nexus_menu_server_thread. — dlsym export
 * thunk @ 00191908
 */
void nexus_script_port_ui_server_thread__export(void)
{
    extern bool nexus_menu_server_thread(int tid);

    (void)syscall(0xb2);
    nexus_menu_server_thread((int)gettid());
}
/* menu_engine chain chunk 15: covers raw lines 7901-8400 (menu_button_pool_init tail from raw 7896; then menu_script_port_bind straddles past 8400 and continues) */

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
#include <fcntl.h>
#include <dlfcn.h>
#include <sys/stat.h>
#include <sys/syscall.h>

extern int __android_log_print(int prio, const char *tag, const char *fmt, ...);
pid_t gettid(void);

/* dlfcn veneers: chunk 3 of this chain already provides Dl_info/dladdr
 * (dl_info_me3_t/dladdr_me3) for the concatenated unit; standalone builds
 * reuse the same names via the c03 guard macro when present, else declare
 * local copies with the me15 suffix */
#ifndef DLINFO_ME3_DECLARED
typedef struct {
    const char *dli_fname;
    void *dli_fbase;
    const void *dli_saddr;
    const char *dli_sname;
} dl_info_me15_t;
#define Dl_info dl_info_me15_t
extern int dladdr(const void *addr, dl_info_me15_t *info);
#endif
int pthread_getname_np(pthread_t, char *, size_t);

/* ---- carried declarations (earlier chunks own the definitions) ---- */
extern int ui_latch_test_and_set(int which, void *addr); /* p4 @ 00193f80 */
extern int theme_menu_status_set(int code, int tid, const char *reason); /* misc @ 00183cdc */
extern int menu_button_label_set(int slot, const char *text); /* widgets @ 0018f988 */
extern int channel2_child_link_valid(uint64_t node, uint64_t parent); /* misc @ 0018f750 */
extern int channel2_root_node_check(uint64_t node); /* misc @ 0018fba0 */
extern void sha256_init(void *state);              /* misc @ 00191314 */
extern void sha256_update(void *state, const void *data, size_t len); /* misc @ 00191338 */
extern void sha256_final(void *state, void *out32); /* misc @ 00191678 */
extern int memfd_module_fd_open(uint64_t handle, const char *soname,
                                const char *sha_hex); /* misc @ 00192f58 */
extern int memfd_fd_path_resolve(const char *path, void *out); /* misc @ 0019339c */
extern uint64_t dlsym_module_verified(void *handle, const char *name,
                                      uint64_t base); /* misc @ 00193324 */

/* engine ops block (chunk 14 externs) */
extern uint64_t g_engine_ctx;        /* 0x28fac8 — engine ctx handle */
extern uint64_t g_engine_remote;     /* 0x28fad0 — remote-read bias */
extern uint64_t g_engine_read_mem;   /* 0x28fae0 — read_mem fn */
extern uint64_t g_engine_alloc;      /* 0x28fae8 — alloc fn */
extern uint64_t g_engine_ctor_a;     /* 0x28faf0 — ctor a */
extern uint64_t g_engine_ctor_b;     /* 0x28faf8 — ctor b */
extern uint64_t g_engine_movie_load; /* 0x28fb00 — movie load fn */
extern uint64_t g_engine_movie_bind; /* 0x28fb08 — movie bind fn */
extern uint64_t g_engine_frame_set;  /* 0x28fb10 — frame set fn */
extern uint64_t g_engine_remove_child; /* 0x28fb30 — remove-child fn */
extern uint64_t g_engine_log_fn;     /* 0x28fb38 — engine log sink */
extern uint32_t g_engine_click_count; /* 0x28fb54 — capped-0x80 counter */
extern int32_t  g_engine_state;      /* 0x28fb48 — observe state */
extern uint32_t g_engine_owner_id;   /* 0x28fb4c — owning tid */
extern uint64_t g_engine_stage_root; /* 0x28fb70 — cached stage root */
extern uint64_t g_engine_stage_view; /* 0x28fb78 — cached stage view */
extern uint64_t g_engine_buttons[0x18]; /* 0x28fb80 — 24 button handles */
extern uint8_t  g_engine_button_live[0x18]; /* 0x28fca0 — live flags */
extern float g_engine_pos_cache[0x18];   /* 0x28fd80 — last set x */
extern float g_engine_pos_cache_y[0x18]; /* 0x28fde0 — last set y */
extern uint8_t g_engine_pos_valid[0x19]; /* 0x28fe40 — position-valid flags */
extern const char g_empty_text_me15[]; /* 0x134f22 — shared "" rodata */

/* body-guard region table (raw 0x19e9e8: {lo, size} pairs + 0x19e9f0 ptrs) */
extern const uint64_t g_guard_regions[9 * 3]; /* 0x19e9e8 — {size, lo, ptr} */

extern uint64_t g_engine_pos_fn;    /* 0x28fb20 — set-position fn (chunk 14) */
#define g_engine_pos_fn_me15 g_engine_pos_fn

/*
 * menu_button_pool_init — build the 24 quick-menu buttons and attach them
 * to the stage root. First verifies the 9 engine body-guard regions
 * (16-byte chunks read through the engine read fn against the pinned
 * rodata at 0x19e9f0; any mismatch -> 'a10_body_guard' stop). Then for
 * each of the 24 buttons: alloc 0x260, ctor_b, ctor_a contract check
 * ('button_allocation' / 'button_constructor_contract' stops), load the
 * 'popover_button_blue' movie and validate it (vtable == bias+0x11ad208,
 * frame count at +0xbe >= 1, 'movie_type_or_frame_count' stop), bind and
 * verify ('movie_binding_contract'), set frame 0, park at 0xc61c3c00 when
 * off-position, intern the empty label; finally re-verify the cached stage
 * root/view (bias+0x12eb9f0 / +0x90), add each button to the root and
 * verify membership ('stage_root_membership' / 'stage_changed_during_
 * build'), then state -> 2 and 'nexus_menu_attached' /
 * 'all_root_membership_verified'. Returns 1 built, 0xffffffff stopped.
 * @ 0018ef84
 */
uint64_t menu_button_pool_init(void)
{
    uint64_t region;

    /* 9 body-guard regions, 16-byte granules */
    for (region = 0; region < 9; region++) {
        uint64_t size = g_guard_regions[region * 3];
        uint64_t lo = g_guard_regions[region * 3 + 1];
        uint64_t expect = g_guard_regions[region * 3 + 2];
        uint64_t off = 0;

        while (off < size) {
            uint64_t chunk = size - off;
            uint64_t buf[2];

            if (0xf < chunk)
                chunk = 0x10;
            {
                uint64_t addr = lo + off + g_engine_remote;

                if (addr < 0x1000
                    || (uint64_t)(~lo - g_engine_remote) < chunk
                    || ((int (*)(uint64_t, uint64_t, void *, uint64_t))
                            g_engine_read_mem)(g_engine_ctx, addr, buf,
                                               chunk) != 1
                    || memcmp(buf, (const void *)(expect + off),
                              (size_t)chunk) != 0)
                    goto body_guard_fail;
            }
            off = off + 0x10;
        }
    }

    /* 24 buttons */
    {
        uint32_t i;
        long pos_idx = 0x2c0;

        for (i = 0; i < 0x18; i++) {
            long btn = (long)((long (*)(uint64_t, uint64_t))g_engine_ctor_a)(
                g_engine_ctx, 0x260);

            g_engine_buttons[i] = (uint64_t)btn;
            if (btn == 0) {
                g_engine_state = 0xffffffff;
                theme_menu_status_set(-1, (int)g_engine_owner_id,
                                      "button_allocation");
                {
                    uint32_t prev = g_engine_click_count;

                    g_engine_click_count = prev + 1;
                    if (prev < 0x80 && g_engine_log_fn != 0)
                        ((void (*)(uint64_t, const char *, const char *,
                                   uint32_t))g_engine_log_fn)(
                            g_engine_ctx, "nexus_menu_stopped",
                            "button_allocation", 0);
                }
                return 0xffffffff;
            }
            ((void (*)(uint64_t, long))g_engine_ctor_b)(g_engine_ctx, btn);
            if (channel2_root_node_check((uint64_t)btn) == 0) {
                g_engine_state = 0xffffffff;
                theme_menu_status_set(-1, (int)g_engine_owner_id,
                                      "button_constructor_contract");
                {
                    uint32_t prev = g_engine_click_count;

                    g_engine_click_count = prev + 1;
                    if (prev < 0x80 && g_engine_log_fn != 0)
                        ((void (*)(uint64_t, const char *, const char *,
                                   uint32_t))g_engine_log_fn)(
                            g_engine_ctx, "nexus_menu_stopped",
                            "button_constructor_contract", 0);
                }
                return 0xffffffff;
            }

            /* movie load + validate */
            {
                uint64_t movie = (uint64_t)((long (*)(uint64_t, const char *))
                                                g_engine_movie_load)(
                    g_engine_ctx, "popover_button_blue");
                uint64_t qword = 0;
                uint16_t frames = 0;
                bool ok = movie != 0;

                if (ok) {
                    if ((movie + 8) >> 3 < 0x201)
                        ok = false;
                    else
                        ok = ((int (*)(uint64_t, uint64_t, void *, uint64_t))
                                  g_engine_read_mem)(g_engine_ctx, movie,
                                                     &qword, 8) == 1;
                }
                if (ok && !(0xfff < qword && (qword & 7) == 0))
                    qword = 0;
                if (ok
                    && (qword != g_engine_remote + 0x11ad208
                        || movie + 0xbe < 0x1000
                        || (movie & 0xfffffffffffffffeULL)
                               == 0xffffffffffffff40ULL))
                    ok = false;
                if (ok)
                    ok = ((int (*)(uint64_t, uint64_t, void *, uint64_t))
                              g_engine_read_mem)(g_engine_ctx, movie + 0xbe,
                                                 &frames, 2) == 1
                         && frames >= 1;
                if (!ok) {
                    g_engine_state = 0xffffffff;
                    theme_menu_status_set(-1, (int)g_engine_owner_id,
                                          "movie_type_or_frame_count");
                    {
                        uint32_t prev = g_engine_click_count;

                        g_engine_click_count = prev + 1;
                        if (prev < 0x80 && g_engine_log_fn != 0)
                            ((void (*)(uint64_t, const char *, const char *,
                                       uint32_t))g_engine_log_fn)(
                                g_engine_ctx, "nexus_menu_stopped",
                                "movie_type_or_frame_count", 0);
                    }
                    return 0xffffffff;
                }

                ((void (*)(uint64_t, long, uint64_t))g_engine_movie_bind)(
                    g_engine_ctx, btn, movie);

                /* binding contract: +0x80 qword plausible and ctor re-check */
                qword = 0;
                {
                    bool bind_ok;

                    if ((uint64_t)(btn + 0x88) >> 3 < 0x201)
                        bind_ok = false;
                    else
                        bind_ok = ((int (*)(uint64_t, uint64_t, void *,
                                            uint64_t))g_engine_read_mem)(
                                      g_engine_ctx, btn + 0x80, &qword, 8)
                                  == 1;
                    if (bind_ok && !(0xfff < qword && (qword & 7) == 0))
                        qword = 0;
                    if (bind_ok)
                        bind_ok = qword >= 0x1000
                                  && channel2_root_node_check((uint64_t)btn)
                                         != 0;
                    if (!bind_ok) {
                        g_engine_state = 0xffffffff;
                        theme_menu_status_set(-1, (int)g_engine_owner_id,
                                              "movie_binding_contract");
                        {
                            uint32_t prev = g_engine_click_count;

                            g_engine_click_count = prev + 1;
                            if (prev < 0x80 && g_engine_log_fn != 0)
                                ((void (*)(uint64_t, const char *,
                                           const char *, uint32_t))
                                     g_engine_log_fn)(
                                    g_engine_ctx, "nexus_menu_stopped",
                                    "movie_binding_contract", 0);
                        }
                        return 0xffffffff;
                    }
                }

                ((void (*)(uint64_t, uint64_t, int))g_engine_frame_set)(
                    g_engine_ctx, movie, 0);

                /* park off-position buttons at the force color */
                if (g_engine_pos_valid[i + 2] == 0
                    || g_engine_pos_cache[pos_idx >> 2] != -9999.0f
                    || g_engine_pos_cache_y[pos_idx >> 2] != -9999.0f) {
                    ((void (*)(float, float, uint64_t, uint64_t))
                        g_engine_pos_fn_me15)(0xc61c3c00, 0xc61c3c00,
                                              g_engine_ctx,
                                              g_engine_buttons[i]);
                    g_engine_pos_cache[pos_idx >> 2] = 0xc61c3c00;
                    g_engine_pos_cache_y[pos_idx >> 2] = 0xc61c3c00;
                    g_engine_pos_valid[i + 2] = 1;
                }
                if (menu_button_label_set((int)i, g_empty_text_me15) != 1)
                    return 0xffffffff;
            }
            pos_idx = pos_idx + 4;
        }

        if (g_engine_click_count < 0x80 && g_engine_log_fn != 0)
            ((void (*)(uint64_t, const char *, const char *, uint32_t))
                 g_engine_log_fn)(g_engine_ctx, "nexus_menu_constructed",
                                  "24_0x260_frame0_buttons", 0);
        g_engine_click_count = g_engine_click_count + 1;
    }

    /* re-verify the stage and attach the buttons */
    {
        uint64_t qword = 0;
        bool ok;

        if ((g_engine_remote + 0x12eb9f8) >> 3 < 0x201)
            ok = false;
        else
            ok = ((int (*)(uint64_t, uint64_t, void *, uint64_t))
                      g_engine_read_mem)(g_engine_ctx,
                                         g_engine_remote + 0x12eb9f0,
                                         &qword, 8) == 1;
        if (ok && !(0xfff < qword && (qword & 7) == 0))
            qword = 0;
        if (qword == g_engine_stage_root) {
            uint64_t view_q = 0;

            if ((qword + 0x98) >> 3 < 0x201)
                ok = false;
            else
                ok = ((int (*)(uint64_t, uint64_t, void *, uint64_t))
                          g_engine_read_mem)(g_engine_ctx, qword + 0x90,
                                             &view_q, 8) == 1;
            if (ok && !(0xfff < view_q && (view_q & 7) == 0))
                view_q = 0;
            if (view_q == g_engine_stage_view) {
                long off;

                for (off = 0; off < 0xc0; off += 8) {
                    uint64_t btn = *(const uint64_t *)((const uint8_t *)
                                                           g_engine_buttons
                                                       + off);

                    ((void (*)(uint64_t, uint64_t, uint64_t))
                         g_engine_remove_child)(g_engine_ctx,
                                                g_engine_stage_root, btn);
                    if (channel2_child_link_valid(btn,
                                                  g_engine_stage_view) == 0) {
                        g_engine_state = 0xffffffff;
                        theme_menu_status_set(-1, (int)g_engine_owner_id,
                                              "stage_root_membership");
                        {
                            uint32_t prev = g_engine_click_count;

                            g_engine_click_count = prev + 1;
                            if (prev < 0x80 && g_engine_log_fn != 0)
                                ((void (*)(uint64_t, const char *,
                                           const char *, uint32_t))
                                     g_engine_log_fn)(
                                    g_engine_ctx, "nexus_menu_stopped",
                                    "stage_root_membership", 0);
                        }
                        return 0xffffffff;
                    }
                }
                g_engine_state = 2;
                theme_menu_status_set(3, (int)g_engine_owner_id, "attached");
                {
                    uint32_t prev = g_engine_click_count;

                    g_engine_click_count = prev + 1;
                    if (prev < 0x80 && g_engine_log_fn != 0)
                        ((void (*)(uint64_t, const char *, const char *,
                                   uint32_t))g_engine_log_fn)(
                            g_engine_ctx, "nexus_menu_attached",
                            "all_root_membership_verified", 0);
                }
                return 1;
            }
        }
        g_engine_state = 0xffffffff;
        theme_menu_status_set(-1, (int)g_engine_owner_id,
                              "stage_changed_during_build");
        {
            uint32_t prev = g_engine_click_count;

            g_engine_click_count = prev + 1;
            if (prev < 0x80 && g_engine_log_fn != 0)
                ((void (*)(uint64_t, const char *, const char *, uint32_t))
                     g_engine_log_fn)(g_engine_ctx, "nexus_menu_stopped",
                                      "stage_changed_during_build", 0);
        }
        return 0xffffffff;
    }

body_guard_fail:
    g_engine_state = 0xffffffff;
    theme_menu_status_set(-1, (int)g_engine_owner_id, "a10_body_guard");
    {
        uint32_t prev = g_engine_click_count;

        g_engine_click_count = prev + 1;
        if (prev < 0x80 && g_engine_log_fn != 0)
            ((void (*)(uint64_t, const char *, const char *, uint32_t))
                 g_engine_log_fn)(g_engine_ctx, "nexus_menu_stopped",
                                  "a10_body_guard", 0);
    }
    return 0xffffffff;
}


/*
 * menu_script_port_bind — bind the script-port module (raw 8151-8524):
 * opens the verified "libNexusEvasionRuntime69252.so" memfd (sha
 * "571dcf2fac82e67c84db6e11f79bbe0031dfb9508fd3974db3ce989e4a5dadf2",
 * 0xc46e0 bytes, SHA-256 streamed in 0x4000 chunks with the pinned digest
 * qwords 0x7ce682ac2fcf1d57 / 0xbe9bf7116edb84 / 0x4d97d38f50b9df31 /
 * -0xd52a2b56167314d), resolves its fd path, dlopens it (RTLD_LAZY|
 * RTLD_LOCAL, with the "/memfd:nexus-* (deleted)" readlink fallback +
 * 'resident lookup failed' log), then dlsym-and-dladdr-verifies the export
 * family against the module base: nexus_script_port_query/set (the
 * contract pair), chat_snapshot, reset, hud_snapshot, battle_snapshot
 * (via dlsym_module_verified), chat_action, server_snapshot,
 * server_select, camera_snapshot, camera_control — each stored into the
 * bind record at +0x10/+0x18/+0x2c/+0x34/+0x38/+0x40/+0x48/+0x50/+0x58/
 * +0x60 (raw param_3 slots). The raw body continues past line 8400 and is
 * finished in the next chunk (camera_control tail + remaining slots).
 * @ 0019265c
 */
uint64_t menu_script_port_bind(long *handle_rec, void *unused, int *out_rec)
{
    uint64_t handle;
    int fd;
    struct stat st;
    uint8_t sha_state[112];
    uint64_t got = 0;
    bool failed = true;
    static uint8_t chunk_buf[0x4000];

    (void)unused;
    handle = (uint64_t)handle_rec[1];
    if (handle == 0)
        return 0;

    fd = memfd_module_fd_open(handle, "libNexusEvasionRuntime69252.so",
                              "571dcf2fac82e67c84db6e11f79bbe0031dfb9508fd3974db3ce989e4a5dadf2");
    if (fd < 0)
        return 0;
    out_rec[0] = out_rec[0] + 1;

    if (fstat(fd, &st) == 0 && st.st_size == 0xc46e0) {
        while (got < 0xc46e0) {
            uint64_t chunk = 0xc46e0 - got;
            ssize_t n;

            if (0x3fff < chunk)
                chunk = 0x4000;
            n = pread(fd, chunk_buf, chunk, (off_t)got);
            if (n < 0) {
                if (errno != 4) /* EINTR */
                    break;
                continue;
            }
            if (n == 0)
                break;
            sha256_update(sha_state, chunk_buf, (size_t)n);
            got = (uint64_t)n + got;
            if (0xc46df < got)
                break;
        }
        if (got == 0xc46e0)
            failed = false;
    }
    close(fd);

    {
        uint8_t digest[32];

        sha256_final(sha_state, digest);
        if (failed || got != 0xc46e0)
            return 0;
        /* pinned digest qwords (raw st_ino-adjacent slots) */
        {
            uint64_t d0, d1, d2, d3;

            memcpy(&d0, digest, 8);
            memcpy(&d1, digest + 8, 8);
            memcpy(&d2, digest + 16, 8);
            memcpy(&d3, digest + 24, 8);
            if (d0 != 0x7ce682ac2fcf1d57ULL || d1 != 0xbe9bf7116edb84ULL
                || d2 != 0x4d97d38f50b9df31ULL
                || d3 != (uint64_t)-0xd52a2b56167314dULL)
                return 0;
        }
    }

    {
        char fd_path[0x100];

        if (memfd_fd_path_resolve((const char *)(uintptr_t)handle, fd_path)
            == 0)
            return 0;
        fd = memfd_module_fd_open((uint64_t)(uintptr_t)fd_path, NULL, NULL);
        if (fd < 0)
            return 0;

        {
            void *mod = dlopen(fd_path, 6 /* RTLD_LAZY|RTLD_LOCAL */);

            if (mod == NULL) {
                /* "/memfd:nexus-* (deleted)" readlink fallback */
                char proc_path[0x100];
                char resident[0x140];
                ssize_t n;

                if (memfd_fd_path_resolve(fd_path, proc_path) == 0)
                    goto bind_fail;
                n = readlink(proc_path, resident, 0x13f);
                if (n < 0 || n - 0x13f > -0x13f || n <= 0x17)
                    goto bind_fail;
                resident[n] = '\0';
                if (memcmp(resident, "/memfd:nexus-", 13) != 0
                    || memcmp(resident + 13, "dnexus-su", 9) != 0
                    || strcmp(resident + n - 23, " (deleted)") != 0
                    || (size_t)(n - 0x17) >= 0x100)
                    goto bind_fail;
                memmove(resident, resident + 23, (size_t)(n - 0x17));
                resident[n - 0x17] = '\0';
                if (strlen(resident) > 0x41) {
                    mod = dlopen(resident + 1, 6);
                    if (mod != NULL)
                        goto loaded;
                }
bind_fail:
                {
                    const char *dl_err = dlerror();
                    const void *soname = strlen(resident) >= 0x42
                                             ? (const void *)resident
                                             : (const void *)g_empty_text_me15;
                    const char *err_text =
                        dl_err != NULL ? dl_err : "unknown";

                    __android_log_print(
                        6, "NexusMem",
                        "resident lookup failed descriptor=%s soname=%s error=%s",
                        fd_path, soname, err_text);
                }
                close(fd);
                return 0;
            }

loaded:
            close(fd);
            {
                long query = (long)dlsym(mod, "nexus_script_port_query");
                long set = (long)dlsym(mod, "nexus_script_port_set");
                Dl_info info;
                uint64_t base;

                if (query == 0 || set == 0
                    || dladdr((void *)query, &info) == 0)
                    return 0;
                base = (uint64_t)info.dli_fbase;
                if (base != (uint64_t)handle_rec[0])
                    return 0;
                *(long *)(out_rec + 2) = query;
                *(long *)(out_rec + 4) = set;

                /* remaining exports: each dlsym + dladdr-verified */
                {
                    long sym;

                    sym = (long)dlsym(mod, "nexus_script_port_chat_snapshot");
                    if (sym != 0 && dladdr((void *)sym, &info) != 0
                        && (uint64_t)info.dli_fbase == base)
                        *(long *)(out_rec + 6) = sym;
                    sym = (long)dlsym(mod, "nexus_script_port_reset");
                    if (sym != 0 && dladdr((void *)sym, &info) != 0
                        && (uint64_t)info.dli_fbase == base)
                        *(long *)(out_rec + 10) = sym;
                    sym = (long)dlsym(mod, "nexus_script_port_hud_snapshot");
                    if (sym != 0 && dladdr((void *)sym, &info) != 0
                        && (uint64_t)info.dli_fbase == base)
                        *(long *)(out_rec + 8) = sym;
                    *(uint64_t *)(void *)(out_rec + 0xe) =
                        dlsym_module_verified(
                            mod, "nexus_script_port_battle_snapshot", base);
                    sym = (long)dlsym(mod, "nexus_script_port_chat_action");
                    if (sym != 0 && dladdr((void *)sym, &info) != 0
                        && (uint64_t)info.dli_fbase == base)
                        *(long *)(out_rec + 0xc) = sym;
                    sym = (long)dlsym(mod, "nexus_script_port_server_snapshot");
                    if (sym != 0 && dladdr((void *)sym, &info) != 0
                        && (uint64_t)info.dli_fbase == base)
                        *(long *)(out_rec + 0x10) = sym;
                    sym = (long)dlsym(mod, "nexus_script_port_server_select");
                    if (sym != 0 && dladdr((void *)sym, &info) != 0
                        && (uint64_t)info.dli_fbase == base)
                        *(long *)(out_rec + 0x12) = sym;
                    sym = (long)dlsym(mod,
                                      "nexus_script_port_camera_snapshot");
                    if (sym != 0 && dladdr((void *)sym, &info) != 0
                        && (uint64_t)info.dli_fbase == base)
                        *(long *)(out_rec + 0x14) = sym;
                    sym = (long)dlsym(mod,
                                      "nexus_script_port_camera_control");
                    if (sym != 0 && dladdr((void *)sym, &info) != 0
                        && (uint64_t)info.dli_fbase == base)
                        *(long *)(out_rec + 0x16) = sym;
                    /* raw continues past 8400: the remaining export slots
                     * land in the next chunk of this chain */
                }
                return 1;
            }
        }
    }
}
/* menu_engine chain chunk 16 (FINAL): covers raw lines 8401-8898 (menu_script_port_bind tail from raw 8401; the atomics; the dlsym export thunks at the file tail) */

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
#include <fcntl.h>
#include <dlfcn.h>
#include <sys/stat.h>

extern int __android_log_print(int prio, const char *tag, const char *fmt, ...);
pid_t gettid(void);
int pthread_getname_np(pthread_t, char *, size_t);

/* ---- carried declarations (earlier chunks own the definitions) ---- */
extern int ui_latch_test_and_set(int which, void *addr); /* below @ 00193f80 */
extern uint64_t dlsym_module_verified(void *handle, const char *name,
                                      uint64_t base); /* misc @ 00193324 */
extern const char g_empty_text_me16[]; /* 0x134f22 — shared "" rodata */

/* dlfcn veneers: chunk 3 provides Dl_info/dladdr for the concatenated unit
 * (dl_info_me3_t/dladdr_me3); standalone builds declare local copies. */
#ifndef DLINFO_ME3_DECLARED
typedef struct {
    const char *dli_fname;
    void *dli_fbase;
    const void *dli_saddr;
    const char *dli_sname;
} dl_info_me16_t;
#define Dl_info dl_info_me16_t
extern int dladdr(const void *addr, dl_info_me16_t *info);
#endif

/*
 * menu_script_port_bind_tail — remaining export slots of
 * menu_script_port_bind (raw 8401-8523): fast replay snapshot/claim
 * (+0x18/+0x1a, camera pair cleared together at +0x14/+0x16 and the
 * server pair at +0x10/+0x12 when either half is missing), then the
 * dlsym_module_verified family — theme snapshot/command (+0x1c/+0x1e),
 * client debug snapshot/apply (+0x20/+0x22), client performance query/
 * apply (+0x24/+0x26), profile snapshot/open/set_name (+0x28/+0x2a/+0x2c),
 * client editor snapshot/apply/tick (+0x2e/+0x30/+0x32), font query/
 * apply/body_current (+0x34/+0x36/+0x38) — each pair (triple) cleared
 * together when any member is missing, and the module is dlclosed at the
 * end (raw drops the close verdict). Split from chunk 15's entry half;
 * both halves together reconstruct the raw body @ 0019265c (tail)
 */
void menu_script_port_bind_tail(void *mod, uint64_t base, int *out_rec)
{
    long sym;

    /* fast replay pair */
    sym = (long)dlsym(mod, "nexus_script_port_fast_replay_snapshot");
    if (sym != 0) {
        Dl_info info;

        if (dladdr((void *)sym, &info) != 0
            && (uint64_t)info.dli_fbase == base)
            *(long *)(out_rec + 0x18) = sym;
    }
    sym = (long)dlsym(mod, "nexus_script_port_fast_replay_claim");
    if (sym != 0) {
        Dl_info info;

        if (dladdr((void *)sym, &info) != 0
            && (uint64_t)info.dli_fbase == base)
            *(long *)(out_rec + 0x1a) = sym;
    }
    if (*(long *)(out_rec + 0x18) == 0 || *(long *)(out_rec + 0x1a) == 0) {
        *(long *)(out_rec + 0x18) = 0;
        out_rec[0x1a] = 0;
        out_rec[0x1b] = 0;
    }
    if (*(long *)(out_rec + 0x14) == 0 || *(long *)(out_rec + 0x16) == 0) {
        *(long *)(out_rec + 0x14) = 0;
        out_rec[0x16] = 0;
        out_rec[0x17] = 0;
    }
    if (*(long *)(out_rec + 0x10) == 0 || *(long *)(out_rec + 0x12) == 0) {
        *(long *)(out_rec + 0x10) = 0;
        out_rec[0x12] = 0;
        out_rec[0x13] = 0;
    }

    /* theme pair */
    *(long *)(out_rec + 0x1c) = (long)dlsym_module_verified(
        mod, "nexus_script_port_theme_snapshot", base);
    sym = (long)dlsym_module_verified(mod, "nexus_script_port_theme_command",
                                      base);
    *(long *)(out_rec + 0x1e) = sym;
    if (*(long *)(out_rec + 0x1c) == 0 || sym == 0) {
        *(long *)(out_rec + 0x1c) = 0;
        out_rec[0x1e] = 0;
        out_rec[0x1f] = 0;
    }

    /* client debug pair */
    *(long *)(out_rec + 0x20) = (long)dlsym_module_verified(
        mod, "nexus_script_port_client_debug_snapshot", base);
    sym = (long)dlsym_module_verified(
        mod, "nexus_script_port_client_debug_apply", base);
    *(long *)(out_rec + 0x22) = sym;
    if (*(long *)(out_rec + 0x20) == 0 || sym == 0) {
        *(long *)(out_rec + 0x20) = 0;
        out_rec[0x22] = 0;
        out_rec[0x23] = 0;
    }

    /* client performance pair */
    *(long *)(out_rec + 0x24) = (long)dlsym_module_verified(
        mod, "nexus_script_port_client_performance_query", base);
    sym = (long)dlsym_module_verified(
        mod, "nexus_script_port_client_performance_apply", base);
    *(long *)(out_rec + 0x26) = sym;
    if (*(long *)(out_rec + 0x24) == 0 || sym == 0) {
        *(long *)(out_rec + 0x24) = 0;
        out_rec[0x26] = 0;
        out_rec[0x27] = 0;
    }

    /* profile triple */
    *(long *)(out_rec + 0x28) = (long)dlsym_module_verified(
        mod, "nexus_script_port_profile_snapshot", base);
    *(uint64_t *)(void *)(out_rec + 0x2a) = dlsym_module_verified(
        mod, "nexus_script_port_profile_open", base);
    sym = (long)dlsym_module_verified(
        mod, "nexus_script_port_profile_set_name", base);
    *(long *)(out_rec + 0x2c) = sym;
    if (*(long *)(out_rec + 0x28) == 0
        || *(long *)(out_rec + 0x2a) == 0 || sym == 0) {
        *(long *)(out_rec + 0x28) = 0;
        out_rec[0x2a] = 0;
        out_rec[0x2b] = 0;
        out_rec[0x2c] = 0;
        out_rec[0x2d] = 0;
    }

    /* client editor triple */
    *(long *)(out_rec + 0x2e) = (long)dlsym_module_verified(
        mod, "nexus_script_port_client_editor_snapshot", base);
    *(uint64_t *)(void *)(out_rec + 0x30) = dlsym_module_verified(
        mod, "nexus_script_port_client_editor_apply", base);
    sym = (long)dlsym_module_verified(
        mod, "nexus_script_port_client_editor_tick", base);
    *(long *)(out_rec + 0x32) = sym;
    if (*(long *)(out_rec + 0x2e) == 0
        || *(long *)(out_rec + 0x30) == 0 || sym == 0) {
        *(long *)(out_rec + 0x2e) = 0;
        out_rec[0x30] = 0;
        out_rec[0x31] = 0;
        out_rec[0x32] = 0;
        out_rec[0x33] = 0;
    }

    /* font triple */
    *(long *)(out_rec + 0x34) = (long)dlsym_module_verified(
        mod, "nexus_script_port_font_query", base);
    *(uint64_t *)(void *)(out_rec + 0x36) = dlsym_module_verified(
        mod, "nexus_script_port_font_apply", base);
    sym = (long)dlsym_module_verified(
        mod, "nexus_script_port_font_body_current", base);
    *(long *)(out_rec + 0x38) = sym;
    if (*(long *)(out_rec + 0x34) == 0
        || *(long *)(out_rec + 0x36) == 0 || sym == 0) {
        *(long *)(out_rec + 0x34) = 0;
        out_rec[0x36] = 0;
        out_rec[0x37] = 0;
        out_rec[0x38] = 0;
        out_rec[0x39] = 0;
    }

    dlclose(mod); /* raw drops the close verdict */
}

/* ===== atomics (LSE monitor / exclusive-load-store paths) ===== */

/* CPU-features flag (raw 0x2a1f88: non-zero = LSE available) */
extern uint8_t g_cpu_has_lse; /* 0x2a1f88 — arm64 LSE atomics available */

/* arm64 exclusive-monitor intrinsics (raw ExclusiveMonitorPass/Status,
 * LOAcquire/LORelease) */
extern int ExclusiveMonitorPass(void *addr, size_t size);
extern int ExclusiveMonitorsStatus(void);
extern void LOAcquire(void);
extern void LORelease(void);

/*
 * ui_int_compare_swap — compare-and-swap on an int: stores newv when
 * *p == expected; returns the previous value. Uses the plain LL/SC-free
 * path when LSE is available (raw single load+store), otherwise the
 * exclusive-monitor retry loop. @ 001939e0
 */
int ui_int_compare_swap(int expected, int newv, int *p)
{
    if (g_cpu_has_lse != 0) {
        int prev = *p;

        if (prev == expected)
            *p = newv;
        return prev;
    }
    {
        int prev;

        do {
            prev = *p;
            if (*p != expected)
                return prev;
        } while (ExclusiveMonitorPass(p, 0x10) ? (*p = newv,
                                                  !ExclusiveMonitorsStatus())
                                               : 1);
        return prev;
    }
}

/*
 * ui_latch_test_and_set — atomic byte test-and-set: stores the new byte
 * and returns the previous value. LSE path: LOAcquire, load, store.
 * Monitor path: exclusive retry loop. The chain-wide signature is
 * (which, addr) with `which` used as the new byte value (raw 00193f80
 * passes the literal 1 at every call site; `which` carries it through).
 * @ 00193f80
 */
int ui_latch_test_and_set(int which, void *addr)
{
    uint8_t newv = (uint8_t)which;
    uint8_t *p = (uint8_t *)addr;

    if (g_cpu_has_lse == 0) {
        uint8_t prev;

        do {
            prev = *p;
        } while (ExclusiveMonitorPass(p, 0x10)
                 ? (*p = newv, !ExclusiveMonitorsStatus())
                 : 1);
        return (int)prev;
    }
    LOAcquire();
    {
        uint8_t prev = *p;

        *p = newv;
        return (int)prev;
    }
}

/*
 * ui_int_exchange_release — atomic int exchange with release semantics:
 * stores newv, returns the previous value. LSE path: load, store,
 * LORelease. @ 00193fb0
 */
uint32_t ui_int_exchange_release(uint32_t newv, uint32_t *p)
{
    if (g_cpu_has_lse == 0) {
        uint32_t prev;

        do {
            prev = *p;
        } while (ExclusiveMonitorPass(p, 0x10)
                 ? (*p = newv, !ExclusiveMonitorsStatus())
                 : 1);
        return prev;
    }
    {
        uint32_t prev = *p;

        *p = newv;
        LORelease();
        return prev;
    }
}

/*
 * ui_int_exchange_acqrel — atomic int exchange with acquire+release
 * semantics: LOAcquire, load, store, LORelease on the LSE path.
 * @ 00193fe0
 */
uint32_t ui_int_exchange_acqrel(uint32_t newv, uint32_t *p)
{
    if (g_cpu_has_lse == 0) {
        uint32_t prev;

        do {
            prev = *p;
        } while (ExclusiveMonitorPass(p, 0x10)
                 ? (*p = newv, !ExclusiveMonitorsStatus())
                 : 1);
        return prev;
    }
    LOAcquire();
    {
        uint32_t prev = *p;

        *p = newv;
        LORelease();
        return prev;
    }
}

/*
 * menu_scroll_revision_bump — atomic fetch-and-add on the scroll
 * revision: *p += delta, returns the previous value (LSE path ends with
 * LORelease). @ 00194010
 */
int menu_scroll_revision_bump(int delta, int *p)
{
    if (g_cpu_has_lse == 0) {
        int prev;

        do {
            prev = *p;
        } while (ExclusiveMonitorPass(p, 0x10)
                 ? (*p = prev + delta, !ExclusiveMonitorsStatus())
                 : 1);
        return prev;
    }
    {
        int prev = *p;

        *p = prev + delta;
        LORelease();
        return prev;
    }
}

/* ===== dlsym export thunks (file tail) =====
 * Each tail-calls through a PLT slot (raw pointer tables at 0x1a3760..);
 * reconstructed as forwarding wrappers to the chain-local definitions. */

/*
 * nexus_menu_server_thread — dlsym export thunk @ 001940b0
 */
void nexus_menu_server_thread__export(void)
{
    extern bool nexus_menu_server_thread(int tid);

    nexus_menu_server_thread((int)gettid());
}

/*
 * nexus_menu_ui_state — dlsym export thunk @ 00194160
 */
void nexus_menu_ui_state__export(void)
{
    extern int nexus_menu_ui_state(void *state_out, int tid);

    nexus_menu_ui_state(NULL, (int)gettid());
}

/*
 * nexus_menu_setting_value — dlsym export thunk @ 001941e0
 */
void nexus_menu_setting_value__export(void)
{
    extern uint64_t nexus_menu_setting_value(uint32_t id, int32_t *value_out);

    (void)nexus_menu_setting_value(0, NULL);
}

/*
 * nexus_menu_init — dlsym export thunk @ 001942e0
 */
void nexus_menu_init__export(void)
{
    extern uint64_t nexus_menu_init(void);

    (void)nexus_menu_init();
}

/*
 * nexus_menu_register_storage — dlsym export thunk @ 00194320
 */
void nexus_menu_register_storage__export(void)
{
    extern uint64_t nexus_menu_register_storage(int *block);

    (void)nexus_menu_register_storage(NULL);
}

/*
 * nexus_menu_register_backend — dlsym export thunk @ 00194330
 */
void nexus_menu_register_backend__export(void)
{
    extern uint64_t nexus_menu_register_backend(uint32_t kind, int *block);

    (void)nexus_menu_register_backend(0, NULL);
}

/*
 * nexus_menu_status — dlsym export thunk @ 001944a0
 */
void nexus_menu_status__export(void)
{
    extern uint64_t nexus_menu_status(void);

    (void)nexus_menu_status();
}

/*
 * nexus_menu_start — dlsym export thunk @ 00194520
 */
void nexus_menu_start__export(void)
{
    extern void nexus_menu_start(void);

    nexus_menu_start();
}

/*
 * nexus_menu_import — dlsym export thunk @ 00194650
 */
void nexus_menu_import__export(void)
{
    extern void nexus_menu_import(void *blob, uint64_t len, int owner_id);

    nexus_menu_import(NULL, 0, (int)gettid());
}

/*
 * nexus_menu_scroll_battle — dlsym export thunk @ 00194660
 */
void nexus_menu_scroll_battle__export(void)
{
    extern uint64_t nexus_menu_scroll_battle(int delta, int owner_id);

    (void)nexus_menu_scroll_battle(0, (int)gettid());
}

/*
 * nexus_menu_server_open — dlsym export thunk @ 00194670
 */
void nexus_menu_server_open__export(void)
{
    extern uint64_t nexus_menu_server_open(int owner_id);

    (void)nexus_menu_server_open((int)gettid());
}

/*
 * nexus_menu_server_action — dlsym export thunk @ 00194680
 */
void nexus_menu_server_action__export(void)
{
    extern uint32_t nexus_menu_server_action(uint32_t action, int owner_id);

    (void)nexus_menu_server_action(0, (int)gettid());
}

/*
 * nexus_menu_theme_action — dlsym export thunk @ 001946c0
 */
void nexus_menu_theme_action__export(void)
{
    extern uint64_t nexus_menu_theme_action(uint32_t action, int owner_id);

    (void)nexus_menu_theme_action(0, (int)gettid());
}

/*
 * nexus_menu_profile_open — dlsym export thunk @ 001946f0
 */
void nexus_menu_profile_open__export(void)
{
    extern uint64_t nexus_menu_profile_open(int owner_id);

    (void)nexus_menu_profile_open((int)gettid());
}

/*
 * nexus_menu_profile_action — dlsym export thunk @ 00194700
 */
void nexus_menu_profile_action__export(void)
{
    extern uint64_t nexus_menu_profile_action(int action, int owner_id);

    (void)nexus_menu_profile_action(0, (int)gettid());
}

/*
 * nexus_menu_dispatch — dlsym export thunk @ 00194730
 */
void nexus_menu_dispatch__export(void)
{
    extern uint32_t nexus_menu_dispatch(uint32_t op, int a, int b, int tid);

    (void)nexus_menu_dispatch(0, 0, 0, (int)gettid());
}

/*
 * nexus_menu_theme_pump — dlsym export thunk @ 00194740
 */
void nexus_menu_theme_pump__export(void)
{
    extern int nexus_menu_theme_pump(int tid, uint64_t now_ms);

    (void)nexus_menu_theme_pump((int)gettid(), 0);
}

/*
 * nexus_menu_profile_pump — dlsym export thunk @ 00194760
 */
void nexus_menu_profile_pump__export(void)
{
    extern void nexus_menu_profile_pump(int owner_id, uint64_t now_ms);

    nexus_menu_profile_pump((int)gettid(), 0);
}

/*
 * nexus_menu_set_value — dlsym export thunk @ 001947c0 (body lives in
 * widgets/misc; forwarded with neutral args)
 */
void nexus_menu_set_value__export(void)
{
    extern uint64_t nexus_menu_set_value(uint32_t id, uint32_t value, int tid);

    (void)nexus_menu_set_value(0, 0, (int)gettid());
}

/*
 * nexus_menu_set_battle — dlsym export thunk @ 001947d0
 */
void nexus_menu_set_battle__export(void)
{
    extern uint64_t nexus_menu_set_battle(int in_battle, int owner_id);

    (void)nexus_menu_set_battle(0, (int)gettid());
}

/*
 * nexus_menu_view — dlsym export thunk @ 001947e0
 */
void nexus_menu_view__export(void)
{
    extern void nexus_menu_view(void *view_out, int owner_id);

    nexus_menu_view(NULL, (int)gettid());
}

/*
 * nexus_menu_action_status — dlsym export thunk @ 00194830
 */
void nexus_menu_action_status__export(void)
{
    extern void nexus_menu_action_status(uint32_t id, void *status_out,
                                         int owner_id);

    nexus_menu_action_status(0, NULL, (int)gettid());
}

/*
 * nexus_menu_section_snapshot — dlsym export thunk @ 00194840
 */
void nexus_menu_section_snapshot__export(void)
{
    extern uint64_t nexus_menu_section_snapshot(void *out, uint64_t len,
                                                int owner_id);

    (void)nexus_menu_section_snapshot(NULL, 0x24a68, (int)gettid());
}

/*
 * nexus_menu_section_present — dlsym export thunk @ 00194850
 */
void nexus_menu_section_present__export(void)
{
    extern int nexus_menu_section_present(void *out, uint64_t len, int owner_id);

    (void)nexus_menu_section_present(NULL, 0x24a68, (int)gettid());
}

/*
 * nexus_menu_main_count — dlsym export thunk @ 001948f0
 */
void nexus_menu_main_count__export(void)
{
    extern int nexus_menu_main_count(void);

    (void)nexus_menu_main_count();
}

/*
 * nexus_menu_settings — dlsym export thunk @ 00194960
 */
void nexus_menu_settings__export(void)
{
    extern const char **nexus_menu_settings(uint64_t *count_out);

    (void)nexus_menu_settings(NULL);
}
