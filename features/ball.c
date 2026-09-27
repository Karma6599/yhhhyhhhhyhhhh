#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>

extern void log_event(const char *category, const char *event, const char *json);

extern uintptr_t g_game_base;
extern uintptr_t game_read(uintptr_t handle, uintptr_t addr, void *out, uint32_t len);
extern uintptr_t runtime_read_handle(void);
extern uintptr_t reader_open_ctx(uintptr_t world, uintptr_t ctrl);
extern int ball_ctx_gate(int mode, const void *ctx);
extern int ball_held_check(uintptr_t pos_pair, uintptr_t ctrl);
extern int game_read_scaled_position(uintptr_t game_base, uintptr_t ctrl,
                                     void *read_fn, int flags, void *out);

extern pthread_once_t g_snapshot_plus_once;
extern long (*snapshot_plus_active_fn)(void);
extern void snapshot_plus_once_init(void);
extern void *g_snapshot_keys_fn;
extern int   g_snapshot_keys_ready;

typedef int (*snapshot_keys_fn_t)(const char *const *keys, int32_t *triples,
                                  int count, int64_t *epoch, int32_t *status);
typedef int (*guarded_stage_fn_t)(void *payload);

#define SNAPSHOT_TRIPLE_EFFECTIVE 1
#define SNAPSHOT_TRIPLE_FLAGS     2
#define SNAPSHOT_FLAG_ACTIVE      2

extern uint64_t g_ball_gate_a;
extern uint64_t g_ball_gate_b;
extern uint64_t g_ball_gate_c;
extern uint64_t g_ball_pack_mode_e0;
extern uint64_t g_ball_pack_mode_d8;
extern uint64_t g_ball_pack_tag_88;
extern uint64_t g_ctx_pos_pair;
extern uint64_t g_ctx_2;
extern uint64_t g_ball_flags_b24;
extern uintptr_t g_ball_object;
extern uint64_t g_ball_apply_count;
extern uint64_t g_ball_pipeline_a[16];
extern uint64_t g_ball_pipeline_b[16];

extern uint8_t g_ball_state_valid;
extern volatile uint8_t g_ball_state_lock;

typedef struct {
    uint64_t src[19];
    uint64_t dst[8];
    uint64_t world;
    uint64_t ctrl;
    uint64_t ctx;
} ball_state_mirror_t;

extern ball_state_mirror_t g_ball_mirror;

typedef uintptr_t (*game_read_fn_t)(uintptr_t handle, uintptr_t addr, void *out, uint32_t len);

typedef struct {
    uint64_t *payload;
    game_read_fn_t read;
    guarded_stage_fn_t apply;
    guarded_stage_fn_t verify;
    uint64_t payload_slots[8];
} guarded_record_t;

typedef int (*ball_worker_fn_t)(void *state);

typedef struct {
    uint64_t tag_a;
    uint32_t mode_e0;
    uint32_t _pad0;
    uint64_t tag_b;
    uint32_t mode_d8;
    uint32_t zero;
    uint64_t world;
    uint64_t pos_pair;
    uint64_t ctrl;
    uint64_t ctx_0;
    uint64_t ctx_1;
    uint32_t ctx_2;
    uint32_t flags;
    uint64_t tag_c;
    float    x;
    float    y;
} ball_trajectory_pack_t;

typedef struct {
    uint64_t a;
    uint64_t b;
    float    x;
    float    y;
} ball_trajectory_out_t;

extern int ball_trajectory_run(uint64_t *pipeline_a, uint64_t *pipeline_b,
                               ball_trajectory_pack_t *pack, ball_worker_fn_t worker,
                               int flags, ball_trajectory_out_t *out, uint64_t *status);
extern int ball_trajectory_collect(int run_result, uint64_t *collect_out,
                                   ball_trajectory_out_t *out);
extern int ball_trajectory_worker(void *state);
extern int game_action_dispatch(const void *ctx, int mode, void *record);
extern int remote_guarded_apply(guarded_record_t *record, uintptr_t target,
                                const void *in, void *out);
extern int ball_held_apply(void *payload);
extern int ball_held_verify(void *payload);
extern int ball_write_apply(void *payload);

#define BALL_TAG_MODE_2   0x0000000000000002ull
#define BALL_TAG_60       0x0000006000000001ull
#define BALL_TAG_0        0x0000000000000001ull
#define BALL_TAG_40       0x0000004000000001ull
#define BALL_DST_TAG_A    0x0000004000000002ull
#define BALL_DST_TAG_B    0x0000000000000001ull

static int snapshot_single(const char *key, int32_t *triple,
                           int64_t *epoch, int32_t *status)
{
    if (__atomic_load_n(&g_snapshot_keys_ready, __ATOMIC_ACQUIRE) != 1
        || g_snapshot_keys_fn == NULL)
        return 0;

    pthread_once(&g_snapshot_plus_once, snapshot_plus_once_init);
    if (snapshot_plus_active_fn == NULL || snapshot_plus_active_fn() != 1)
        return 0;

    snapshot_keys_fn_t query = (snapshot_keys_fn_t)g_snapshot_keys_fn;
    return query(&key, triple, 1, epoch, status) == 1;
}

static int ball_assist_active(int64_t *epoch_out)
{
    int32_t triple[3];
    int32_t status = -1;
    int64_t epoch = 0;

    if (!snapshot_single("ballAssistEnabled", triple, &epoch, &status))
        return 0;
    if (epoch == 0 || status != 0
        || triple[SNAPSHOT_TRIPLE_FLAGS] != SNAPSHOT_FLAG_ACTIVE
        || triple[SNAPSHOT_TRIPLE_EFFECTIVE] != 1)
        return 0;

    if (epoch_out)
        *epoch_out = epoch;
    return 1;
}

int ball_write_verify(void *payload)
{
    uint64_t *rec = payload;

    if (runtime_read_handle() == 0)
        return 0;

    int32_t triple[3];
    int32_t status = -1;
    int64_t epoch = 0;

    if (!snapshot_single("ballAssistEnabled", triple, &epoch, &status))
        return 0;
    if (epoch == 0 || status != 0
        || triple[SNAPSHOT_TRIPLE_FLAGS] != SNAPSHOT_FLAG_ACTIVE
        || triple[SNAPSHOT_TRIPLE_EFFECTIVE] != 1
        || (uint64_t)epoch != rec[2])
        return 0;

    return reader_open_ctx((uintptr_t)rec[0], (uintptr_t)rec[1]) != 0;
}

int ball_intercept_compute(uintptr_t world, uintptr_t ctrl,
                           int32_t xy_out[2], int64_t *epoch_out)
{
    if (!ball_assist_active(epoch_out))
        return 0;

    uintptr_t handle = runtime_read_handle();
    if (handle == 0)
        return 0;
    handle = reader_open_ctx(world, ctrl);
    if (handle == 0)
        return 0;

    struct {
        uint64_t field_0;
        uint32_t ctx_2;
        float    x;
        float    y;
    } pos = { 0, 0, 0.0f, 0.0f };

    if (game_read_scaled_position(g_game_base, ctrl, (void *)game_read, 0, &pos) == 0)
        return 0;
    if (pos.ctx_2 == 0)
        return 0;

    int32_t ball_x = 0;
    int32_t ball_y = 0;
    uintptr_t probe = game_read(0, g_ball_object + 0xc4, &ball_x, 4);
    if (probe == 0)
        return 0;
    if (game_read(probe, g_ball_object + 0xc8, &ball_y, 4) == 0)
        return 0;

    if (!(4 < ball_x && ball_x < 0x79 && 4 < ball_y && ball_y < 0x79))
        return 0;

    float dy = ((float)ball_y - 2.25f) - pos.y;
    float dx = pos.x - ((float)ball_x - 1.0f) * 0.5f;

    float dist_sq = dx * dx + dy * dy;
    if (!isfinite(dist_sq) || dist_sq < 1e-6f)
        return 0;

    float scale = 12.0f / sqrtf(dist_sq);
    float new_x = dx * scale + pos.x;
    float new_y = dy * scale + pos.y;

    xy_out[0] = (int32_t)(new_x * 300.0f);
    xy_out[1] = (int32_t)(new_y * 300.0f);

    int over_x = (xy_out[0] + 60000) > 120000;
    return (!over_x && xy_out[1] != -60001 && xy_out[1] > -60002) && xy_out[1] < 60001;
}

int ball_assist_position(uintptr_t world, uintptr_t ctrl,
                         int32_t *x_out, int32_t *y_out)
{
    int32_t xy[2] = { 0, 0 };
    int64_t epoch = 0;

    int valid = ball_intercept_compute(world, ctrl, xy, &epoch);
    if (valid == 0)
        return 0;

    uint64_t pair_word = 0;
    if (game_read((uintptr_t)valid, world + 0xfac, &pair_word, 8) == 0)
        return 0;

    guarded_record_t record;
    record.payload = record.payload_slots;
    record.read = (game_read_fn_t)game_read;
    record.apply = ball_write_apply;
    record.verify = ball_write_verify;
    record.payload_slots[0] = world;
    record.payload_slots[1] = ctrl;
    record.payload_slots[2] = epoch;

    int result = remote_guarded_apply(&record, world, &pair_word, xy);
    if (result == 1) {
        if (x_out)
            *x_out = xy[0];
        if (y_out)
            *y_out = xy[1];
        g_ball_apply_count++;
        return 1;
    }
    if (result == -1) {
        log_event("fatal", "ball_xy_restore_unverified", NULL);
        abort();
    }
    return 0;
}

int ball_assist_held_adjust(const void *ctx, int32_t *x, int32_t *y)
{
    if (ctx == NULL || x == NULL || y == NULL)
        return 0;
    if (!ball_ctx_gate(0, ctx))
        return 0;

    const uint64_t *c = (const uint64_t *)ctx;
    uintptr_t handle = reader_open_ctx((uintptr_t)c[2], (uintptr_t)c[3]);
    if (handle == 0)
        return 0;

    int64_t epoch;
    if (ball_assist_active(&epoch))
        return ball_assist_position((uintptr_t)c[2], (uintptr_t)c[3], x, y);

    if (g_ball_gate_a == 0 && g_ball_gate_b == 0)
        return 1;

    if (game_read_scaled_position(g_game_base, (uintptr_t)c[3],
                                  (void *)game_read, 0, &g_ctx_pos_pair) == 0)
        return 0;
    if (!ball_held_check(g_ctx_pos_pair, (uintptr_t)c[3]))
        return 0;

    ball_trajectory_pack_t pack;
    memset(&pack, 0, sizeof pack);
    pack.tag_a = BALL_TAG_MODE_2;
    pack.mode_e0 = (uint32_t)g_ball_pack_mode_e0;
    pack.tag_b = BALL_TAG_0;
    pack.mode_d8 = (uint32_t)g_ball_pack_mode_d8;
    pack.world = c[2];
    pack.pos_pair = g_ctx_pos_pair;
    pack.ctrl = c[3];
    pack.ctx_0 = c[0];
    pack.ctx_1 = c[1];
    pack.ctx_2 = (uint32_t)g_ctx_2;
    pack.flags = (uint32_t)g_ball_flags_b24;
    pack.tag_c = g_ball_pack_tag_88;
    pack.x = (float)*x / 300.0f;
    pack.y = (float)*y / 300.0f;

    ball_trajectory_out_t out = { 0, 0, 0.0f, 0.0f };
    uint64_t status = BALL_TAG_40;

    int run = ball_trajectory_run(g_ball_pipeline_a, g_ball_pipeline_b, &pack,
                                  ball_trajectory_worker, 0, &out, &status);
    if (run != 1)
        return (run == 0 && g_ball_gate_c == 0);

    uint64_t collect = 0;
    if (ball_trajectory_collect(run, &collect, &out) == 0)
        return 0;

    if (!isfinite(out.x) || !isfinite(out.y) || out.x < 0.0f || out.y < 0.0f
        || out.x >= 200.0f || out.y >= 200.0f)
        return 0;

    uint64_t dispatch_record[14];
    memset(dispatch_record, 0, sizeof dispatch_record);
    dispatch_record[0] = BALL_TAG_60;

    if (game_action_dispatch(ctx, 3, dispatch_record) == 0)
        return 0;

    guarded_record_t record;
    record.payload = record.payload_slots;
    record.read = (game_read_fn_t)game_read;
    record.apply = ball_held_apply;
    record.verify = ball_held_verify;
    record.payload_slots[0] = (uint64_t)(uintptr_t)ctx;
    record.payload_slots[1] = BALL_TAG_60;

    int32_t xy_in[2] = { *x, *y };
    int32_t xy_out[2] = { (int32_t)(out.y * 300.0f), (int32_t)(out.x * 300.0f) };

    int result = remote_guarded_apply(&record, (uintptr_t)c[2], xy_in, xy_out);
    if (result == 1) {
        *x = xy_out[0];
        *y = xy_out[1];
        return 1;
    }
    if (result == -1) {
        log_event("fatal", "held_xy_restore_unverified", NULL);
        abort();
    }
    return 0;
}

int ball_assist_frame_tick(const void *ctx, const void *frame)
{
    const uint64_t *c = (const uint64_t *)ctx;

    int64_t epoch;
    if (ball_assist_active(&epoch))
        return ball_assist_position((uintptr_t)c[2], (uintptr_t)c[3], NULL, NULL);

    if (g_ball_gate_b == 0
        && (g_ball_gate_a == 0 || *(const int32_t *)((const char *)frame + 4) != 2))
        return 1;

    int32_t x = *(const int32_t *)((const char *)frame + 0x30);
    int32_t y = *(const int32_t *)((const char *)frame + 0x34);
    return ball_assist_held_adjust(ctx, &x, &y);
}

int ball_assist_state_snapshot(const uint64_t *src, uint64_t *dst)
{
    int64_t epoch;
    if (!ball_assist_active(&epoch))
        return 0;

    uint8_t expected = 0;
    if (!__atomic_compare_exchange_n(&g_ball_state_lock, &expected, 1,
                                     false, __ATOMIC_ACQ_REL, __ATOMIC_RELAXED))
        return 0;

    int32_t xy[2] = { 0, 0 };
    int64_t ctx = 0;
    if (ball_intercept_compute(src[6], src[7], xy, &ctx) == 0) {
        __atomic_store_n(&g_ball_state_lock, 0, __ATOMIC_RELEASE);
        return 0;
    }

    dst[3] = src[3];
    dst[2] = src[2];
    dst[5] = src[0xc];
    dst[4] = src[0xb];
    dst[6] = (uint64_t)ctx;
    dst[7] = (uint32_t)xy[0] | ((uint64_t)(uint32_t)xy[1] << 32);
    dst[0] = BALL_DST_TAG_A;
    dst[1] = BALL_DST_TAG_B;

    g_ball_mirror.ctx = (uint64_t)ctx;
    memcpy(g_ball_mirror.src, src, sizeof g_ball_mirror.src);
    memcpy(g_ball_mirror.dst, dst, sizeof g_ball_mirror.dst);
    g_ball_mirror.world = src[6];
    g_ball_mirror.ctrl = src[7];
    g_ball_state_valid = 1;
    return 1;
}
