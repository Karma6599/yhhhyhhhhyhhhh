#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>

static pid_t gettid_(void) { return (pid_t)syscall(SYS_gettid); }

extern void log_event(const char *category, const char *event, const char *json);

extern uintptr_t g_game_base;
extern uintptr_t game_read(uintptr_t handle, uintptr_t addr, void *out, uint32_t len);
extern uintptr_t runtime_read_handle(void);
extern uintptr_t reader_open_ctx(uintptr_t world, uintptr_t ctrl);
extern int runtime_gate_a(int mode);
extern int runtime_gate_b(void);
extern int plan_busy_check(void);
extern int game_read_scaled_position(uintptr_t game_base, uintptr_t ctrl,
                                     void *read_fn, int flags, void *out);
extern int resolve_field_ptr(uintptr_t addr, uintptr_t *out);

extern pthread_once_t g_snapshot_plus_once;
extern long (*snapshot_plus_active_fn)(void);
extern void snapshot_plus_once_init(void);
extern void *g_snapshot_keys_fn;
extern int   g_snapshot_keys_ready;

typedef int (*snapshot_keys_fn_t)(const char *const *keys, int32_t *triples,
                                  int count, int64_t *epoch, int32_t *status);
typedef int (*game_read_fn_t)(uintptr_t handle, uintptr_t addr, void *out, uint32_t len);

#define SNAPSHOT_TRIPLE_EFFECTIVE 1
#define SNAPSHOT_TRIPLE_FLAGS     2
#define SNAPSHOT_FLAG_ACTIVE      2

extern uint32_t g_runtime_thread;
extern uint32_t g_battle_active;
extern uint32_t g_perf_active;

extern uint64_t g_ctx_token;
extern uint64_t g_ctx_3;
extern uint64_t g_ctx_2;
extern uint64_t g_ctx_a;
extern uint64_t g_ctx_b;
extern uint64_t g_ctx_c;
extern uint64_t g_ctx_ctrl;
extern int32_t  g_ctx_x;
extern int32_t  g_ctx_y;

extern uint64_t g_plan_stamp;
extern uint64_t g_plan_base;
extern uint64_t g_plan_peer_check;
extern uint64_t g_plan_mode;
extern uint64_t g_plan_zero;
extern uint64_t g_plan_ok;

extern uint8_t g_afk_visual_gate;
extern uintptr_t g_afk_probe_addr;
extern uint32_t g_afk_probe_word;

extern uint64_t g_afk_prev_token;
extern uint64_t g_afk_prev_ctx2;
extern uint64_t g_afk_last_ms;
extern int32_t  g_afk_prev_x;
extern int32_t  g_afk_prev_y;
extern uint64_t g_afk_queued;
extern uint8_t  g_afk_moved_valid;

typedef struct {
    uint64_t token;
    uint64_t ctx_3;
    uint64_t epoch;
    uint64_t deadline_ms;
    uint64_t ctx_a;
    uint64_t ctx_b;
    uint64_t ctx_c;
    uint64_t ctrl;
    uint64_t position_handle;
    uint64_t ctx_2;
    uint64_t reserved_50;
    uint64_t reserved_58;
    int32_t  x;
    int32_t  y;
    uint64_t active;
    uint64_t flags;
    uint64_t moved;
    uint64_t last_ms;
} afk_plan_t;

extern afk_plan_t g_afk_plan;
extern uint64_t g_afk_plan_queue[64];

extern int plan_submit(uint64_t *queue, afk_plan_t *plan, const void *callbacks);

extern void plan_thunk_1(void);
extern void plan_thunk_2(void);
extern void plan_thunk_3(void);
extern void plan_thunk_4(void);
extern void plan_thunk_5(void);

static uint64_t monotonic_ms(void)
{
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
        return 0;
    return (uint64_t)ts.tv_sec * 1000 + (uint64_t)ts.tv_nsec / 1000000;
}

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

int anti_afk_plan_ready(uint64_t mode, const uint64_t *plan, int kind)
{
    (void)mode;
    uint64_t now_ms = monotonic_ms();

    if (kind != 2 || g_runtime_thread == 0 || g_runtime_thread != (uint32_t)gettid_())
        return 0;
    if (g_perf_active == 0)
        return 0;

    int32_t triple[3];
    int32_t status = -1;
    int64_t epoch = 0;
    if (!snapshot_single("antiAfkEnabled", triple, &epoch, &status))
        return 0;
    if (epoch == 0 || status != 0
        || triple[SNAPSHOT_TRIPLE_FLAGS] != SNAPSHOT_FLAG_ACTIVE
        || triple[SNAPSHOT_TRIPLE_EFFECTIVE] != 1
        || (uint64_t)epoch != plan[2])
        return 0;

    if (now_ms < plan[3] || now_ms - plan[3] > 500)
        return 0;
    if (runtime_gate_a(0) == 0 || runtime_gate_b() == 0)
        return 0;
    if (plan_busy_check() != 0)
        return 0;

    if (g_ctx_token != plan[0] || g_ctx_3 != plan[1])
        return 0;

    uintptr_t handle = reader_open_ctx(plan[4], plan[7]);
    if (handle == 0)
        return 0;

    struct {
        uint64_t field_0;
        int32_t  ctx_2;
        float    x;
        float    y;
    } pos = { 0, 0, 0.0f, 0.0f };

    if (game_read_scaled_position(g_game_base, plan[7],
                                  (void *)game_read, 0, &pos) == 0)
        return 0;

    if (pos.ctx_2 != (int32_t)plan[9])
        return 0;

    float dx = pos.x + (float)g_afk_plan.x / -300.0f;
    if (!(dx == 1e-5f || dx < 1e-5f))
        return 0;
    float dy = pos.y + (float)g_afk_plan.y / -300.0f;
    if (!(dy == 1e-5f || dy < 1e-5f))
        return 0;

    uintptr_t resolved = 0;
    if (resolve_field_ptr(plan[5] + 0x58, &resolved) == 0)
        return 0;
    if (resolved != plan[8])
        return 0;

    uintptr_t counter_obj = 0;
    if (resolve_field_ptr(resolved + 0x20, &counter_obj) == 0)
        return 0;

    int32_t counter = -1;
    if (game_read(handle, counter_obj + 0xc, &counter, 4) == 0
        || counter < 0 || counter >= 0x1e)
        return 0;

    return (now_ms <= g_afk_last_ms - 1) || (now_ms - g_afk_last_ms > 350);
}

void anti_afk_plan_tick(void *frame)
{
    if (frame == NULL || *(int32_t *)((char *)frame + 0x24) == 0)
        goto cancel;
    if (g_runtime_thread == 0 || g_runtime_thread != (uint32_t)gettid_())
        goto cancel;
    if (g_battle_active == 0)
        goto cancel;

    int32_t triple[3];
    int32_t status = -1;
    int64_t epoch = 0;
    if (!snapshot_single("antiAfkEnabled", triple, &epoch, &status))
        goto cancel;
    if (epoch == 0 || status != 0
        || triple[SNAPSHOT_TRIPLE_FLAGS] != SNAPSHOT_FLAG_ACTIVE
        || triple[SNAPSHOT_TRIPLE_EFFECTIVE] != 1)
        goto cancel;

    uintptr_t handle = reader_open_ctx(g_ctx_a, g_ctx_c);
    if (handle == 0)
        goto cancel;

    uint64_t position_handle = 0;
    if (game_read(handle, g_ctx_b + 0x58, &position_handle, 8) == 0)
        goto cancel;
    if (position_handle + 0x2000 <= 0x11fff || (position_handle & 7) != 0)
        goto cancel;

    if (g_afk_prev_token == g_ctx_token && g_afk_prev_ctx2 == g_ctx_2) {
        g_afk_plan.ctx_2 = g_afk_prev_ctx2;
        if (g_afk_moved_valid == 1) {
            int64_t dx = (int64_t)g_ctx_y - (int64_t)g_afk_prev_y;
            int64_t dy = (int64_t)g_ctx_x - (int64_t)g_afk_prev_x;
            g_afk_plan.moved = (uint64_t)(dx * dx + dy * dy > 63);
        } else {
            g_afk_plan.moved = 0;
        }
    } else {
        g_afk_plan.moved = 0;
        g_afk_last_ms = 0;
        g_afk_moved_valid = 0;
        g_afk_plan.ctx_2 = g_ctx_2;
    }

    g_afk_plan.deadline_ms = *(uint64_t *)((char *)frame + 0x28);
    g_afk_plan.flags = 1;
    if (g_afk_visual_gate == 1) {
        if (g_plan_peer_check == g_plan_base) {
            g_afk_plan.flags = 1;
            if (g_plan_stamp == g_ctx_token && g_plan_mode != 2 && g_plan_zero == 0)
                g_afk_plan.flags = (g_plan_ok != 0);
        } else {
            g_afk_plan.flags = 1;
        }
    }

    g_afk_plan.ctx_c = g_ctx_c;
    g_afk_plan.ctx_b = g_ctx_b;
    g_afk_plan.epoch = (uint64_t)epoch;
    g_afk_plan.ctx_a = g_ctx_a;
    g_afk_plan.ctx_b = g_ctx_b;
    g_afk_plan.token = g_ctx_token;
    g_afk_plan.ctx_3 = g_ctx_3;
    g_afk_plan.reserved_50 = 0;
    g_afk_plan.reserved_58 = 0;
    g_afk_plan.x = g_ctx_x;
    g_afk_plan.y = g_ctx_y;
    g_afk_plan.active = 1;
    g_afk_plan.last_ms = g_afk_last_ms;
    g_afk_plan.position_handle = position_handle;

    static const struct {
        uint64_t zero;
        int (*validate)(uint64_t mode, const uint64_t *plan, int kind);
        void (*thunk_1)(void);
        void (*thunk_2)(void);
        void (*thunk_3)(void);
        void (*thunk_4)(void);
        void (*thunk_5)(void);
    } callbacks = {
        0,
        anti_afk_plan_ready,
        plan_thunk_1,
        plan_thunk_2,
        plan_thunk_3,
        plan_thunk_4,
        plan_thunk_5,
    };

    if (plan_submit(g_afk_plan_queue, &g_afk_plan, &callbacks) != 0) {
        char json[0xb4];
        snprintf(json, sizeof json,
                 ",\"queued\":%llu,\"x\":%d,\"y\":%d,\"local_moves\":0",
                 (unsigned long long)g_afk_queued, g_ctx_x, g_ctx_y);
        log_event("anti_afk", "idle_current_position", json);
    }
    return;

cancel:
    plan_submit(g_afk_plan_queue, NULL, NULL);
}

void anti_afk_flag_clear(void *target)
{
    if (target == NULL)
        return;

    int32_t triple[3];
    int32_t status = -1;
    int64_t epoch = 0;
    if (!snapshot_single("antiAfkEnabled", triple, &epoch, &status))
        return;
    if (epoch == 0 || status != 0
        || triple[SNAPSHOT_TRIPLE_FLAGS] != SNAPSHOT_FLAG_ACTIVE
        || triple[SNAPSHOT_TRIPLE_EFFECTIVE] != 1)
        return;

    uintptr_t handle = runtime_read_handle();
    if (handle == 0)
        return;

    uint32_t probe = 0;
    if (game_read(handle, g_afk_probe_addr, &probe, 4) == 0)
        return;
    if (probe != g_afk_probe_word)
        return;

    *(uint64_t *)((char *)target + 0x40) &= ~1ull;
}
