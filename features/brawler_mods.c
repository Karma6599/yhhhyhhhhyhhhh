#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

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
extern uint64_t g_los_world;
extern uint64_t g_los_ctrl;
extern uintptr_t g_engine_base;

extern int evasion_runtime_gate(void);
extern uintptr_t game_read(uintptr_t handle, uintptr_t addr, void *out, uint32_t len);
extern long remote_write_buf(void *channel, const void *buf, size_t len, uintptr_t addr);
extern int remote_guarded_apply(void *record, uintptr_t target,
                                const void *in, void *out);
extern uintptr_t plan_for_triple(const void *query_block);
extern int movement_ctx_valid(uint64_t ctx, uint64_t obj);
extern void remote_object_fetch(void *ctx, void *world, void *request,
                                void *read_cb, int flags, void **out_obj,
                                void **out_status);
extern int game_object_resolve(uintptr_t engine, uintptr_t obj, void *read_fn,
                               int flags, void *out);
extern int colt_avoid_step(uintptr_t world, uintptr_t ctrl, uint32_t target_x,
                           uint32_t target_y, void *state, uint64_t ctrl_pair,
                           int64_t counter, int64_t now_ms, uint64_t frame,
                           float *out_plan_a, float *out_plan_b);
extern void colt_write_apply(void *payload);
extern int dyna_state_read(uintptr_t ctx, void *read_fn, int flags, void *out);
extern int page_perm_check(uintptr_t addr, int64_t *out);
extern int self_resolve(void *self_ctx, void *out);
extern int feature_epoch_query(const char *key, int64_t *epoch);
extern int dyna_engine_gate(void);
extern uint64_t engine_frame_time(int kind);
extern int dyna_target_resolve(void *state, void *query, void *out);
extern void dyna_result_publish(void *state, uint64_t frame, uint32_t result);
extern void fetch_read_cb(void);
extern void overlay_draw_marker(uint32_t a, uint32_t b, uint32_t c,
                                void *ctx, void *view, int mode);
extern void overlay_draw_line(float x0, float y0, float x1, float y1,
                              void *ctx, int mode);
extern void overlay_draw_rect(void *ctx, int x, int y, int w, int h, int mode);
extern void overlay_draw_text(void *ctx, const char *text, int x, int y);

extern uint32_t g_colt_gate;
extern uint64_t g_colt_epoch;
extern uint64_t g_colt_ctx;
extern uint64_t g_colt_pair;
extern uint32_t g_colt_req_field;
extern void *g_fetch_ctx;
extern uint64_t g_fetch_world;
extern int64_t g_colt_hold_counter;
extern uint64_t g_colt_state[8];
extern uint64_t g_colt_avoid_state;
extern uint64_t g_colt_aux_state;
extern void *g_write_channel;

extern uint8_t g_restore_armed;
extern uintptr_t g_restore_probe_addr;
extern uint32_t g_restore_probe_val;
extern uintptr_t g_restore_sig_addr;
extern uint64_t g_restore_sig[4];

extern uint64_t g_dyna_ctx;
extern uint64_t g_dyna_own_obj;
extern uint64_t g_dyna_pair16[2];
extern uint64_t g_dyna_world_last;
extern uint64_t g_dyna_guard_a;
extern uint64_t g_dyna_guard_b;
extern uint64_t g_dyna_guard_c;
extern uint64_t g_dyna_guard_d;
extern uint64_t g_dyna_state[14];

#define ROD_COLOR_ENEMY   (*(const uint64_t *)(uintptr_t)0x10e538)
#define ROD_COLOR_ALLY    (*(const uint64_t *)(uintptr_t)0x10e578)
#define ROD_COLOR_TRACER  (*(const uint64_t *)(uintptr_t)0x112b00)
#define ROD_COLOR_NAMETAG (*(const uint64_t *)(uintptr_t)0x1127e0)

#define COLT_MARKER   0x00f42401u
#define DYNA_MARKER   0x00f42409u

#define MOVE_RESTORE_SPEED_SECTION  0x2e
#define MOVE_RESTORE_NANI_SECTION   0x79

#define DYNA_ENGINE_SUBMIT_OFF 0xb3d64cu

static uint32_t marker_adjusted(void)
{
    uint32_t marker = (uint32_t)g_ctx_bolt_marker;
    uint32_t adj = marker - 1000000u;
    if (marker + 0xfefc99c0u > 999999u)
        adj = marker;
    return adj;
}

static uint64_t monotonic_ms(void)
{
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
        return 0;
    return (uint64_t)ts.tv_sec * 1000 + (uint64_t)ts.tv_nsec / 1000000;
}

int brawler_mod_key_family(const char *name)
{
    if (name == NULL)
        return 0;

    if (strcmp(name, "attackRangeIndicator") == 0 ||
        strcmp(name, "hitboxRenderer") == 0 ||
        strcmp(name, "enemyTracer") == 0 ||
        strcmp(name, "trophiesAboveHead") == 0 ||
        strcmp(name, "kitNaniModEnabled") == 0 ||
        strcmp(name, "dynaJumpEnabled") == 0)
        return 0;

    if (strcmp(name, "characterOutlineEnabled") == 0 ||
        strcmp(name, "antiAfkEnabled") == 0 ||
        strcmp(name, "autododgeEnabled") == 0 ||
        strcmp(name, "combatDodgeRequireHold") == 0 ||
        strncmp(name, "aop", 3) == 0 ||
        strncmp(name, "killaura", 8) == 0 ||
        strcmp(name, "coltModEnabled") == 0 ||
        strcmp(name, "koltModEnabled") == 0)
        return 1;

    return strncmp(name, "holdToShoot", 11) == 0;
}

int colt_write_verify(void *payload)
{
    uint64_t *rec = payload;
    const char *keys[1] = { "coltModEnabled" };
    int32_t triples[3];
    int64_t epoch = 0;
    int32_t status = -1;
    snapshot_keys_fn_t query = (snapshot_keys_fn_t)g_snapshot_keys_fn;

    if (marker_adjusted() != COLT_MARKER)
        return 0;
    if (g_snapshot_keys_ready != 1 || query == NULL)
        return 0;
    if (query(keys, triples, 1, &epoch, &status) != 1)
        return 0;
    if (epoch == 0 || status != 0 || triples[2] != 2 || triples[1] != 1)
        return 0;
    if (evasion_runtime_gate() == 0)
        return 0;
    if ((uint64_t)epoch != rec[2])
        return 0;

    uintptr_t plan = plan_for_triple(triples);
    if (plan == 0)
        return 0;

    return movement_ctx_valid(rec[0], rec[1]) != 0;
}

void colt_movement_apply(uint64_t ctx, uint64_t target_obj)
{
    const char *keys[1] = { "coltModEnabled" };
    int32_t triples[3];
    int64_t epoch = 0;
    int32_t status = -1;
    snapshot_keys_fn_t query = (snapshot_keys_fn_t)g_snapshot_keys_fn;
    uint64_t request[12];
    uint64_t payload[3];
    float plan_a = 0.0f;
    float plan_b = 0.0f;

    if (marker_adjusted() != COLT_MARKER)
        return;
    if (g_snapshot_keys_ready != 1 || query == NULL)
        return;
    if (query(keys, triples, 1, &epoch, &status) != 1)
        return;
    if (epoch == 0 || status != 0 || triples[2] != 2 || triples[1] != 1)
        return;
    if (evasion_runtime_gate() == 0)
        return;
    if (movement_ctx_valid(ctx, target_obj) == 0 || g_colt_gate == 0)
        return;
    if (g_colt_epoch != (uint64_t)epoch)
        return;

    uint64_t now_ms = monotonic_ms();
    uintptr_t plan = plan_for_triple(triples);

    if (plan == 0) {
        memset(g_colt_state, 0, sizeof g_colt_state);
        g_colt_avoid_state = 0;
        g_colt_aux_state = 0;

        memset(request, 0, sizeof request);
        *(uint32_t *)((char *)request + 0x00) = 2;
        *(uint64_t *)((char *)request + 0x14) = g_colt_req_field;
        *(uint64_t *)((char *)request + 0x20) = ctx;
        *(uint64_t *)((char *)request + 0x28) = g_colt_ctx;
        *(uint32_t *)((char *)request + 0x30) = (uint32_t)target_obj;
        *(float *)((char *)request + 0x34) = *(const float *)&((const uint32_t *)&target_obj)[1];
        *(float *)((char *)request + 0x38) = *(const float *)&g_ctx_c;
        *(uint32_t *)((char *)request + 0x3c) = (uint32_t)(g_ctx_c >> 32);
        *(uint64_t *)((char *)request + 0x40) = g_ctx_a;
        *(uint64_t *)((char *)request + 0x48) = g_ctx_frame_no;
        *(uint32_t *)((char *)request + 0x4c) = (uint32_t)(g_colt_pair >> 32);
        *(uint64_t *)((char *)request + 0x50) = (uint64_t)epoch;

        void *fetched = NULL;
        void *fetch_status = NULL;
        remote_object_fetch(g_fetch_ctx, (void *)(uintptr_t)g_fetch_world, request,
                            fetch_read_cb, 0, &fetched, &fetch_status);

        int32_t resolve_out[12];
        memset(resolve_out, 0, sizeof resolve_out);

        if (fetch_status != (void *)1
            || game_object_resolve(g_engine_base, (uintptr_t)fetched,
                                   (void *)game_read, 0, resolve_out) == 0
            || resolve_out[11] == 0
            || resolve_out[4] != (int32_t)(uint32_t)(target_obj >> 32))
            return;

        if (g_colt_hold_counter == -1)
            g_colt_hold_counter = 1;
        else
            g_colt_hold_counter = g_colt_hold_counter + 1;

        if (colt_avoid_step(g_los_world, g_los_ctrl,
                            (uint32_t)(target_obj >> 32),
                            (uint32_t)(uint64_t)epoch,
                            g_colt_state, g_ctx_c, g_colt_hold_counter,
                            (int64_t)now_ms, g_ctx_frame_no,
                            &plan_a, &plan_b) == 0)
            return;
    } else {
        g_colt_state[2] = (int64_t)now_ms + 0x334;
    }

    if (game_object_resolve(g_engine_base, target_obj,
                            (void *)game_read, 0, request) == 0)
        return;
    if (*(uint32_t *)((char *)request + 0x2c) == 0)
        return;

    float resolved_a = *(float *)((char *)request + 0x34);
    float resolved_b = *(float *)((char *)request + 0x38);
    float dist = hypotf(plan_a - resolved_a, plan_b - resolved_b);
    if (dist <= 5.0f)
        dist = 5.0f;
    dist = fminf(dist, 16.0f);

    float vel_lo = *(const float *)&g_colt_state[5];
    float vel_hi = *(const float *)((char *)&g_colt_state[5] + 4);

    union { int32_t i; float f; } cx, cy;
    cx.f = (resolved_b + vel_hi * dist) * 300.0f;
    cy.f = (resolved_a + vel_lo * dist) * 300.0f;
    uint64_t new_pos = ((uint64_t)(uint32_t)cx.i << 32) | (uint32_t)cy.i;

    payload[0] = ctx;
    payload[1] = target_obj;
    payload[2] = (uint64_t)epoch;

    struct {
        uint64_t *payload;
        void *read;
        void (*apply)(void *);
        int (*verify)(void *);
        uint64_t slots[8];
    } record;

    record.payload = payload;
    record.read = (void *)game_read;
    record.apply = colt_write_apply;
    record.verify = colt_write_verify;
    memset(record.slots, 0, sizeof record.slots);

    int result = remote_guarded_apply(&record, ctx, NULL, &new_pos);
    if (result == -1) {
        log_event("fatal", "colt_xy_restore_unverified", NULL);
        abort();
    }
}

int dyna_jump_armed(void)
{
    const char *keys[1] = { "dynaJumpEnabled" };
    int32_t triples[3];
    int64_t epoch = 0;
    int32_t status = -1;
    uint8_t state[16];
    snapshot_keys_fn_t query = (snapshot_keys_fn_t)g_snapshot_keys_fn;

    if (marker_adjusted() != DYNA_MARKER)
        return 0;
    if (g_snapshot_keys_ready != 1 || query == NULL)
        return 0;
    if (query(keys, triples, 1, &epoch, &status) != 1)
        return 0;
    if (epoch == 0 || status != 0 || triples[2] != 2 || triples[1] != 1)
        return 0;
    if (evasion_runtime_gate() == 0)
        return 0;

    memset(state, 0, sizeof state);
    if (dyna_state_read(g_dyna_ctx, (void *)game_read, 0, state) == 0)
        return 0;

    return state[0] == 1;
}

int movement_restore_verify(uint64_t *record, void *frame)
{
    const char *key;
    int32_t triples[3];
    int64_t epoch = 0;
    int32_t status = -1;
    snapshot_keys_fn_t query = (snapshot_keys_fn_t)g_snapshot_keys_fn;
    uint32_t probe[1];
    uint64_t sig[4];
    uint32_t rd32;
    uint64_t rd64;
    uint32_t expect[4];
    uintptr_t base;
    uintptr_t addr_a;
    uintptr_t addr_b;
    uintptr_t addr_c;
    int32_t orig_a;
    int32_t orig_b;
    int64_t orig_c;
    int a_failed;
    int b_failed;
    uintptr_t h;

    if (*(int32_t *)((char *)record + 0x10) != 4)
        return 1;

    key = NULL;
    if (*(int32_t *)((char *)record + 0x14) == MOVE_RESTORE_SPEED_SECTION) {
        if (g_restore_armed != 1)
            return 0;
        h = game_read((uintptr_t)record, g_restore_probe_addr, probe, 4);
        if (h == 0 || probe[0] != g_restore_probe_val)
            return 0;
        if (game_read(h, g_restore_sig_addr, sig, 0x20) == 0)
            return 0;
        if (sig[0] != g_restore_sig[0] || sig[1] != g_restore_sig[1]
            || sig[2] != g_restore_sig[2] || sig[3] != g_restore_sig[3])
            return 0;
        key = "speedExploitEnabled";
    } else if (*(int32_t *)((char *)record + 0x14) == MOVE_RESTORE_NANI_SECTION) {
        key = "kitNaniModEnabled";
    }

    if (key == NULL)
        return 0;

    if (g_snapshot_keys_ready != 1 || query == NULL)
        return 0;
    if (query(&key, triples, 1, &epoch, &status) != 1)
        return 0;
    if (epoch == 0 || status != 0 || triples[2] != 2 || triples[1] != 1)
        return 0;
    if ((uint64_t)epoch != record[9])
        return 0;

    if (*(int32_t *)((char *)frame + 0xb0) != 0) {
        int32_t expected = *(int32_t *)((char *)record + 0xc0);
        if (expected == 0)
            return 0;
        if (*(int32_t *)((char *)frame + 0xb0) != 1)
            return 0;
        if ((*(uint8_t *)((char *)frame + 0x29) >> 1 & 1) == 0)
            return 0;
        if (g_restore_armed == 0 || evasion_runtime_gate() == 0)
            return 0;

        int32_t brawler_id = 0;
        if (game_read(g_dyna_own_obj, g_dyna_own_obj + 0x24c, &rd32, 4) != 0
            && rd32 > 100 && rd32 < 0x7d1)
            brawler_id = (int32_t)rd32;
        if (expected != brawler_id)
            return 0;
    }

    if (game_read(0, (uintptr_t)record[11] + 0xf9e, &rd32, 1) == 0)
        return 0;
    if (*(uint8_t *)&rd32 != 1)
        return 0;
    if (game_read(0, (uintptr_t)record[11] + 0x918, &rd64, 8) == 0)
        return 0;

    uint64_t region = rd64;
    if (region + 0x2000u >= 0x12000u || (region & 7) != 0)
        return 0;
    if (region != record[12])
        return 0;

    int64_t perm = 0;
    if (page_perm_check(region + 0x28, &perm) == 0)
        return 0;
    if ((uint64_t)perm != record[13])
        return 0;

    base = (uintptr_t)record[11];
    addr_a = base + 0xf74;
    addr_b = base + 4000;
    addr_c = base + 0xfcc;

    if (game_read(0, addr_a, &orig_a, 4) == 0)
        return 0;
    if (game_read(0, addr_b, &orig_b, 4) == 0)
        return 0;
    if (game_read(0, addr_c, &orig_c, 8) == 0)
        return 0;

    expect[0] = 0x3d4ccccd;
    expect[1] = 0;
    expect[2] = 0;
    expect[3] = 0;

    a_failed = 0;
    b_failed = 0;

    if (remote_write_buf(g_write_channel, expect, 4, addr_a) == 4
        && game_read(4, addr_a, &rd32, 4) != 0 && rd32 == expect[0]) {
        if (remote_write_buf(g_write_channel, expect + 2, 4, addr_b) == 4
            && game_read(4, addr_b, &rd32, 4) != 0 && rd32 == expect[2]) {
            if (remote_write_buf(g_write_channel, &record[3], 8, addr_c) == 8
                && game_read(8, addr_c, &rd64, 8) != 0 && rd64 == record[3])
                return 1;
        } else {
            b_failed = 1;
        }
    } else {
        a_failed = 1;
    }

    if (remote_write_buf(g_write_channel, &orig_a, 4, addr_a) != 4
        || game_read(4, addr_a, &rd32, 4) == 0
        || rd32 != (uint32_t)orig_a
        || (!a_failed
            && (remote_write_buf(g_write_channel, &orig_b, 4, addr_b) != 4
                || game_read(4, addr_b, &rd32, 4) == 0
                || rd32 != (uint32_t)orig_b))
        || (!b_failed
            && (remote_write_buf(g_write_channel, &orig_c, 8, addr_c) != 8
                || game_read(8, addr_c, &rd64, 8) == 0
                || rd64 != (uint64_t)orig_c))) {
        log_event("fatal", "movement_restore_unverified", NULL);
        abort();
    }

    return 1;
}

void dyna_engine_tick(void *frame)
{
    const char *keys[1] = { "dynaJumpEnabled" };
    int32_t triples[3];
    int64_t epoch = 0;
    int32_t status = -1;
    snapshot_keys_fn_t query = (snapshot_keys_fn_t)g_snapshot_keys_fn;
    int32_t resolve[20];
    uint8_t dyna_state[16];
    uint64_t fq[10];

    if (g_dyna_world_last == g_ctx_a)
        return;

    if (frame == NULL) {
        memset(g_dyna_state, 0, sizeof g_dyna_state);
        return;
    }

    if (g_snapshot_keys_ready != 1 || query == NULL)
        goto reset;
    if (query(keys, triples, 1, &epoch, &status) != 1)
        goto reset;
    if (epoch == 0 || status != 0 || triples[2] != 2 || triples[1] != 1)
        goto reset;
    if (evasion_runtime_gate() == 0)
        goto reset;

    memset(resolve, 0, sizeof resolve);
    if (game_object_resolve(g_engine_base, g_dyna_own_obj,
                            (void *)game_read, 0, resolve) == 0)
        goto reset;

    memset(dyna_state, 0, sizeof dyna_state);
    if (dyna_state_read(g_dyna_ctx, (void *)game_read, 0, dyna_state) == 0)
        goto reset;

    uint64_t stamp = *(uint64_t *)((char *)frame + 0x28);

    memset(fq, 0, sizeof fq);
    fq[0] = g_dyna_pair16[1];
    fq[1] = g_dyna_pair16[0];
    fq[2] = stamp;
    fq[3] = g_dyna_ctx;
    fq[4] = g_dyna_own_obj;
    *(uint32_t *)((char *)fq + 0x30) = 1;
    *(uint32_t *)((char *)fq + 0x34) = dyna_state[0];
    *(uint32_t *)((char *)fq + 0x38) = (uint32_t)resolve[7];
    *(uint32_t *)((char *)fq + 0x3c) = (uint32_t)resolve[5];
    *(int32_t *)((char *)fq + 0x40) = (int32_t)(*(float *)((char *)resolve + 0x34) * 300.0f);
    *(int32_t *)((char *)fq + 0x44) = (int32_t)(*(float *)((char *)resolve + 0x38) * 300.0f);

    int32_t target_out[8];
    memset(target_out, 0, sizeof target_out);

    if (dyna_target_resolve(g_dyna_state, fq, target_out) == 0)
        return;

    if (g_dyna_guard_a == 0 && g_dyna_guard_b == 0 && g_dyna_guard_c == 0) {
        int64_t epoch_now = 0;
        uint64_t self_out[2] = { 0, 0 };
        uint64_t now = engine_frame_time(1);
        uint64_t guard_saved = g_dyna_guard_d;

        if (g_dyna_own_obj == fq[4]
            && self_resolve((void *)(uintptr_t)g_ctx_self, self_out) != 0
            && dyna_jump_armed() != 0
            && feature_epoch_query("dynaJumpEnabled", &epoch_now) != 0
            && (uint64_t)epoch_now == (uint64_t)epoch
            && stamp <= now
            && now - stamp < 0x1f5
            && dyna_engine_gate() != 0) {

            g_dyna_guard_b = 1;
            g_dyna_guard_a = 3;
            g_dyna_guard_d = 3;

            uint32_t result = ((uint32_t (*)(uint64_t, uint64_t, void *, int32_t,
                                             int32_t, int32_t, int32_t))
                (g_engine_base + DYNA_ENGINE_SUBMIT_OFF))(
                    g_dyna_ctx, g_dyna_own_obj, self_out,
                    target_out[0], target_out[1], target_out[2], target_out[3]);

            g_dyna_guard_b = 0;
            g_dyna_guard_a = 0;
            g_dyna_world_last = g_ctx_a;
            g_dyna_guard_d = guard_saved;

            dyna_result_publish(g_dyna_state, engine_frame_time(1), result);

            if ((result & 1) != 0) {
                char json[300];
                snprintf(json, sizeof json,
                         ",\"shot\":%u,\"raw_x\":%d,\"raw_y\":%d,\"own_gid\":%u",
                         (uint32_t)(target_out[4] + 1),
                         (uint32_t)target_out[0], (uint32_t)target_out[1],
                         (uint32_t)resolve[2]);
                log_event("dyna_accepted", "engine_submit_skill", json);
            }
        } else {
            dyna_result_publish(g_dyna_state, stamp, 0xffffffffu);
        }
    } else {
        dyna_result_publish(g_dyna_state, stamp, 0);
    }
    return;

reset:
    memset(g_dyna_state, 0, sizeof g_dyna_state);
}

typedef struct {
    float w_x;
    float w_y;
    float w_z;
    float w_off;
    float u_x;
    float u_y;
    float u_z;
    float u_off;
    float u_scale;
    float v_x;
    float v_y;
    float v_z;
    float v_off;
    float v_scale;
} brawler_view_t;

static void brawler_view_load(brawler_view_t *m, const uint8_t *view)
{
    m->u_x = *(const float *)(view + 0x828);
    m->v_x = *(const float *)(view + 0x82c);
    m->w_x = *(const float *)(view + 0x834);
    m->u_y = *(const float *)(view + 0x838);
    m->v_y = *(const float *)(view + 0x83c);
    m->w_y = *(const float *)(view + 0x844);
    m->u_z = *(const float *)(view + 0x848);
    m->v_z = *(const float *)(view + 0x84c);
    m->w_z = *(const float *)(view + 0x854);
    m->u_off = *(const float *)(view + 0x858);
    m->v_off = *(const float *)(view + 0x85c);
    m->w_off = *(const float *)(view + 0x864);
    m->u_scale = *(const float *)(view + 0x868);
    m->v_scale = *(const float *)(view + 0x86c);
}

static int brawler_project(const brawler_view_t *m, float x, float y, float z_off,
                           float *u_out, float *v_out)
{
    float w = m->w_off + m->w_z * z_off + (m->w_x * x + m->w_y * y);
    float u;
    float v;

    if (!isfinite(w) || w <= 1e-6f)
        return 0;

    u = (m->u_off + m->u_z * z_off + (m->u_x * x + m->u_y * y)) / w;
    v = (m->v_off + m->v_z * z_off + (m->v_x * x + m->v_y * y)) / w;
    u = m->u_scale * (u * 0.5f + 0.5f);
    v = m->v_scale * (1.0f - (v * 0.5f + 0.5f));

    if (!isfinite(u) || fabsf(u) >= 1e6f || !isfinite(v) || fabsf(v) >= 1e6f)
        return 0;

    *u_out = u;
    *v_out = v;
    return 1;
}

void brawler_overlay_render(void *ctx, uint8_t *view, int32_t *features,
                            uint32_t render_flags)
{
    const int *dims = *(const int **)((char *)ctx + 8);
    brawler_view_t m;

    if ((render_flags & 1) != 0) {
        if (*(int32_t *)(features + 0x58) != 0) {
            overlay_draw_marker(*(uint32_t *)(view + 0x870),
                                *(uint32_t *)(view + 0x874),
                                *(uint32_t *)(view + 0x878), ctx, view, 2);
        }

        if (*(int32_t *)(view + 0x11c) != 0) {
            uint32_t count = *(uint32_t *)(view + 0x11c);

            for (uint32_t e = 0; e < count; e++) {
                uint8_t *ent = view + 0x144 + e * 0x48;

                if (*(int32_t *)(ent + 0x04) != 0)
                    continue;

                if (*(int32_t *)(features + 0x58) != 0
                    && *(int32_t *)(ent + 0x00) != 2
                    && *(int32_t *)(ent + 0x08) != 0) {
                    overlay_draw_marker(*(uint32_t *)(ent + 0x18),
                                        *(uint32_t *)(ent + 0x1c),
                                        *(uint32_t *)(ent + 0x24), ctx, view, 2);
                }

                if (*(int32_t *)(features + 0x50) != 0) {
                    int32_t type = *(int32_t *)(ent + 0x00);

                    *(float *)((char *)ctx + 0x1c) = 1.0f;
                    *(float *)((char *)ctx + 0x18) = (type == 1) ? 1.0f : 0.2f;
                    *(uint64_t *)((char *)ctx + 0x10) =
                        (type == 0) ? ROD_COLOR_ENEMY : ROD_COLOR_ALLY;

                    if (*(int32_t *)(view + 0x820) != 0) {
                        brawler_view_load(&m, view);

                        float ent_x = *(float *)(ent + 0x18);
                        float ent_y = *(float *)(ent + 0x1c);
                        float x0 = ent_x * 300.0f;
                        float y0 = ent_y * -300.0f;
                        float u0, v0, u1, v1, u2, v2;
                        float edge_a, edge_b, box;

                        if (!brawler_project(&m, x0, y0, 0.0f, &u0, &v0))
                            goto tracer;
                        if (!brawler_project(&m, x0 + 300.0f, y0, 0.0f, &u1, &v1))
                            goto tracer;
                        if (!brawler_project(&m, x0, y0 - 300.0f, 0.0f, &u2, &v2))
                            goto tracer;

                        edge_a = hypotf(u1 - u0, v1 - v0);
                        edge_b = hypotf(u2 - u0, v2 - v0);
                        box = (edge_a + edge_b) * 0.5f;

                        if (isfinite(box) && box >= 1.0f) {
                            float scale_x = (float)dims[2] / *(const float *)(view + 0xf8);
                            float scale_y = (float)dims[3] / *(const float *)(view + 0xfc);
                            float origin_y = (float)(dims[1] + dims[3]);
                            float step = fminf(*(const float *)(ent + 0x20) / box, 1.5f);
                            float neg = -step;
                            float z_off = step * 3.0f * 300.0f;
                            float near_u[4], near_v[4];
                            float far_u[4], far_v[4];
                            int bad = 0;

                            for (int i = 0; i < 4; i++) {
                                float cx = (i == 0 || i == 3) ? neg : step;
                                float cy = (i < 2) ? neg : step;
                                float px = (ent_x + cx) * 300.0f;
                                float py = (ent_y + cy) * -300.0f;
                                float u, v;

                                if (!brawler_project(&m, px, py, 0.0f, &u, &v)) {
                                    bad = 1;
                                    break;
                                }
                                near_u[i] = u * scale_x + (float)dims[0];
                                near_v[i] = v * scale_y - origin_y;
                            }

                            if (!bad) {
                                for (int i = 0; i < 4; i++) {
                                    float cx = (i == 0 || i == 3) ? neg : step;
                                    float cy = (i < 2) ? neg : step;
                                    float px = (ent_x + cx) * 300.0f;
                                    float py = (ent_y + cy) * -300.0f;
                                    float u, v;

                                    if (!brawler_project(&m, px, py, z_off, &u, &v)) {
                                        bad = 1;
                                        break;
                                    }
                                    far_u[i] = u * scale_x + (float)dims[0];
                                    far_v[i] = v * scale_y - origin_y;
                                }
                            }

                            if (!bad) {
                                overlay_draw_line(near_u[0], near_v[0], near_u[1], near_v[1], ctx, 2);
                                overlay_draw_line(far_u[0], far_v[0], far_u[1], far_v[1], ctx, 2);
                                overlay_draw_line(near_u[0], near_v[0], far_u[0], far_v[0], ctx, 2);
                                overlay_draw_line(near_u[1], near_v[1], near_u[2], near_v[2], ctx, 2);
                                overlay_draw_line(far_u[1], far_v[1], far_u[2], far_v[2], ctx, 2);
                                overlay_draw_line(near_u[1], near_v[1], far_u[1], far_v[1], ctx, 2);
                                overlay_draw_line(near_u[2], near_v[2], near_u[3], near_v[3], ctx, 2);
                                overlay_draw_line(far_u[2], far_v[2], far_u[3], far_v[3], ctx, 2);
                                overlay_draw_line(near_u[2], near_v[2], far_u[2], far_v[2], ctx, 2);
                                overlay_draw_line(near_u[3], near_v[3], near_u[0], near_v[0], ctx, 2);
                                overlay_draw_line(far_u[3], far_v[3], far_u[0], far_v[0], ctx, 2);
                                overlay_draw_line(near_u[3], near_v[3], far_u[3], far_v[3], ctx, 2);
                            }
                        }
                    }
                }

tracer:
                if (*(int32_t *)(features + 0x54) != 0
                    && *(int32_t *)(ent + 0x00) == 0
                    && *(int32_t *)(ent + 0x08) != 0) {
                    float ent_px = *(const float *)(ent + 0x10);
                    float ent_py = *(const float *)(ent + 0x14);
                    float view_x = *(const float *)(view + 0xf8);
                    float view_y = *(const float *)(view + 0xfc);

                    *(uint64_t *)((char *)ctx + 0x18) = ROD_COLOR_TRACER >> 32;
                    *(uint64_t *)((char *)ctx + 0x10) = ROD_COLOR_TRACER & 0xffffffffu;

                    overlay_draw_line(
                        (*(const float *)(view + 0x100) * (float)dims[2]) / view_x
                            + (float)dims[0],
                        (float)(dims[3] + dims[1])
                            - (*(const float *)(view + 0x104) * (float)dims[3]) / view_y,
                        (float)(dims[0] + (int)((ent_px * (float)dims[2]) / view_x)),
                        (float)((dims[3] + dims[1])
                            - (int)((ent_py * (float)dims[3]) / view_y)),
                        ctx, 2);
                }
            }
        }
    }

    if ((render_flags & 2) == 0 || *(int32_t *)(features + 0x10) == 0
        || *(int32_t *)(features + 0x1c) == 0 || *(int32_t *)(view + 0x7e8) == 0)
        return;

    {
        uint32_t marker = *(uint32_t *)(view + 0x7ec);
        uint32_t adj = marker - 1000000u;
        const char *name;

        if (marker + 0xfefc99c0u > 999999u)
            adj = marker;

        switch (adj) {
        case 16000000:
        case 0xf42421:
        case 0xf42437:
        case 0xf424ac:
        case 0xf424ae:
        case 0xf424b6:
        case 0xf424cc:
            name = "SHELLY";
            break;
        case 0xf42401: name = "COLT"; break;
        case 0xf42402: name = "BULL"; break;
        case 0xf42403: name = "BROCK"; break;
        case 0xf42404: name = "RICO"; break;
        case 0xf42405:
        case 0xf42524: name = "SPIKE"; break;
        case 0xf42406: name = "BARLEY"; break;
        case 0xf42407: name = "JESSIE"; break;
        case 0xf42408: name = "NITA"; break;
        case 0xf42409: name = "DYNAMIKE"; break;
        case 0xf4240a: name = "EL PRIMO"; break;
        case 0xf4240b: name = "MORTIS"; break;
        case 0xf4240c: name = "CROW"; break;
        case 0xf4240d: name = "POCO"; break;
        case 0xf4240e: name = "BO"; break;
        case 0xf4240f: name = "PIPER"; break;
        case 0xf42410: name = "PAM"; break;
        case 0xf42411: name = "TARA"; break;
        case 0xf42412: name = "DARRYL"; break;
        case 0xf42413: name = "PENNY"; break;
        case 0xf42414: name = "FRANK"; break;
        case 0xf42415: name = "GENE"; break;
        case 0xf42416: name = "TICK"; break;
        case 0xf42417:
        case 0xf424fe: name = "LEON"; break;
        case 0xf42418: name = "ROSA"; break;
        case 0xf42419: name = "CARL"; break;
        case 0xf4241a: name = "BIBI"; break;
        case 0xf4241b:
        case 0xf424e3: name = "8-BIT"; break;
        case 0xf4241c: name = "SANDY"; break;
        case 0xf4241d: name = "BEA"; break;
        case 0xf4241e: name = "EMZ"; break;
        case 0xf4241f: name = "MR. P"; break;
        case 0xf42420: name = "MAX"; break;
        case 0xf42422: name = "JACKY"; break;
        case 0xf42423: name = "GALE"; break;
        case 0xf42424: name = "NANI"; break;
        case 0xf42425: name = "SPROUT"; break;
        case 0xf42426: name = "SURGE"; break;
        case 0xf42427: name = "COLETTE"; break;
        case 0xf42428: name = "AMBER"; break;
        case 0xf42429: name = "LOU"; break;
        case 0xf4242a: name = "BYRON"; break;
        case 0xf4242b: name = "EDGAR"; break;
        case 0xf4242c: name = "RUFFS"; break;
        case 0xf4242d: name = "STU"; break;
        case 0xf4242e: name = "BELLE"; break;
        case 0xf4242f: name = "SQUEAK"; break;
        case 0xf42430: name = "GROM"; break;
        case 0xf42431: name = "BUZZ"; break;
        case 0xf42432: name = "GRIFF"; break;
        case 0xf42433: name = "ASH"; break;
        case 0xf42434:
        case 0xf424c9: name = "MEG"; break;
        case 0xf42435: name = "LOLA"; break;
        case 0xf42436: name = "FANG"; break;
        case 0xf42438: name = "EVE"; break;
        case 0xf42439: name = "JANET"; break;
        case 0xf4243a:
        case 0xf424d5: name = "BONNIE"; break;
        case 0xf4243b: name = "OTIS"; break;
        case 0xf4243c: name = "SAM"; break;
        case 0xf4243d: name = "GUS"; break;
        case 0xf4243e: name = "BUSTER"; break;
        case 0xf4243f: name = "CHESTER"; break;
        case 0xf42440: name = "GRAY"; break;
        case 0xf42441: name = "MANDY"; break;
        case 0xf42442:
        case 0xf424e4: name = "R-T"; break;
        case 0xf42443: name = "WILLOW"; break;
        case 0xf42444: name = "MAISIE"; break;
        case 0xf42445: name = "HANK"; break;
        case 0xf42446: name = "CORDELIUS"; break;
        case 0xf42447: name = "DOUG"; break;
        case 0xf42448: name = "PEARL"; break;
        case 0xf42449: name = "CHUCK"; break;
        case 0xf4244a: name = "CHARLIE"; break;
        case 0xf4244b: name = "MICO"; break;
        case 0xf4244c:
        case 0xf4257d: name = "KIT"; break;
        case 0xf4244e: name = "MELODIE"; break;
        case 0xf4244f: name = "ANGELO"; break;
        case 0xf42450: name = "DRACO"; break;
        case 0xf42451: name = "LILY"; break;
        case 0xf42452: name = "BERRY"; break;
        case 0xf42453: name = "CLANCY"; break;
        case 0xf42454: name = "MOE"; break;
        case 0xf42455:
        case 0xf424fd: name = "KENJI"; break;
        case 0xf42456: name = "SHADE"; break;
        case 0xf42457: name = "JUJU"; break;
        case 0xf42458: name = "BUZZ LIGHTYEAR"; break;
        case 0xf42459: name = "MEEPLE"; break;
        case 0xf4245a: name = "OLLIE"; break;
        case 0xf4245b: name = "LUMI"; break;
        case 0xf4245c: name = "FINX"; break;
        case 0xf4245d: name = "JAE-YONG"; break;
        case 0xf4245e: name = "KAZE"; break;
        case 0xf4245f: name = "ALLI"; break;
        case 0xf42460: name = "TRUNK"; break;
        case 0xf42461:
        case 0xf425b7: name = "MINA"; break;
        case 0xf42462: name = "ZIGGY"; break;
        case 0xf42463: name = "PIERCE"; break;
        case 0xf42464: name = "GIGI"; break;
        case 0xf42465: name = "GLOWY"; break;
        case 0xf42466:
        case 0xf425b8: name = "SIRIUS"; break;
        case 0xf42467: name = "NAJIA"; break;
        case 0xf42468: name = "DAMIAN"; break;
        case 0xf42469: name = "STARR NOVA"; break;
        case 0xf4246a: name = "BOLT"; break;
        case 0xf4246b: name = "NORI"; break;
        case 0xf4246c: name = "WENDY"; break;
        case 0xf4246d: name = "COSMO"; break;
        case 0xf4246e: name = "VINCE"; break;
        case 0xf424f2: name = "DRILLER"; break;
        default: return;
        }

        int half = dims[2];
        if (half < 0)
            half = half + 1;
        int x = dims[0] + (half >> 1) - (int)strlen(name) * 6;
        int y = dims[3] + dims[1];

        *(uint64_t *)((char *)ctx + 0x18) = ROD_COLOR_NAMETAG >> 32;
        *(uint64_t *)((char *)ctx + 0x10) = ROD_COLOR_NAMETAG & 0xffffffffu;

        overlay_draw_rect(ctx, x - 8, y - 0x6e, (int)strlen(name) * 12 + 0x10, 0x18, 1);

        *(uint64_t *)((char *)ctx + 0x18) = 0x3f8000003f800000ull;
        *(uint64_t *)((char *)ctx + 0x10) = 0x3f8000003f800000ull;

        overlay_draw_text(ctx, name, x, y - 0x69);
    }
}

int overlay_section_lookup(const char *name)
{
    if (strcmp(name, "KILL AURA") == 0)
        return 0;
    if (strcmp(name, "SMART AIM") == 0)
        return 1;
    if (strcmp(name, "AUTO DODGE") == 0)
        return 2;
    if (strcmp(name, "AUTO FARM") == 0)
        return 3;
    if (strcmp(name, "X-RAY") == 0)
        return 4;
    if (strcmp(name, "FOLLOW") == 0)
        return 5;
    if (strcmp(name, "SPIN") == 0)
        return 6;
    if (strcmp(name, "HOLD FIRE") == 0)
        return 7;
    if (strcmp(name, "BALL ASSIST") == 0)
        return 8;
    if (strcmp(name, "NANI ULTI MOD") == 0)
        return 9;
    if (strcmp(name, "ESP") == 0)
        return 10;
    if (strcmp(name, "SPEED EXPLOIT") == 0)
        return 11;
    if (strcmp(name, "AUTO PIN") == 0)
        return 12;
    if (strcmp(name, "COLT MOD") == 0)
        return 13;
    if (strcmp(name, "BOLT MOD") == 0)
        return 14;
    return -1;
}
