#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>

extern int *__errno(void);

extern uintptr_t g_game_base;
extern uintptr_t game_read(uintptr_t handle, uintptr_t addr, void *out, uint32_t len);
extern uintptr_t runtime_read_handle(void);
extern int controller_read_name(const void *const *reader, uintptr_t obj, char *out);
extern int performance_client_ready(void);

extern uint32_t g_perf_flags;
extern uint32_t g_perf_active;

extern int32_t nexus_script_port_set(const char *key, uint32_t value);

extern pthread_once_t g_snapshot_plus_once;
extern long (*snapshot_plus_active_fn)(void);
extern void snapshot_plus_once_init(void);
extern void *g_snapshot_keys_fn;
extern int   g_snapshot_keys_ready;

typedef int (*snapshot_keys_fn_t)(const char *const *keys, int32_t *triples,
                                  int count, int64_t *epoch, int32_t *status);

extern uint8_t  g_speed_hook_installed;
extern uint32_t g_speed_hook_word;
extern uintptr_t g_speed_hook_site;
extern uintptr_t g_speed_trampoline;
extern uint64_t  g_speed_trampoline_expect[4];

extern uint64_t g_ctx_self;
extern uint64_t g_ctx_ctrl;
extern uint64_t g_ctx_token;

extern uint64_t g_plan_stamp;
extern uint64_t g_plan_peer;
extern uint64_t g_plan_base;
extern uint64_t g_plan_last_ms;
extern uint64_t g_plan_gate_a;
extern uint64_t g_plan_gate_b;
extern uint64_t g_plan_peer_check;
extern uint64_t g_plan_kind;
extern uint64_t g_plan_aux;
extern uint64_t g_plan_epoch;
extern uint64_t g_speed_pace_state[8];

extern uint32_t speed_pace_plan(uint64_t *state, uint64_t identity, uint64_t epoch,
                                uint64_t now_ms, int coherent);

#define SNAPSHOT_TRIPLE_REQUESTED 0
#define SNAPSHOT_TRIPLE_EFFECTIVE 1
#define SNAPSHOT_TRIPLE_FLAGS     2
#define SNAPSHOT_FLAG_ACTIVE      2

#define SPEED_GAME_ADVANCE_OFF 0xe7af70u
#define SPEED_MAX_FPS_LIMIT    0x91u

typedef void (*speed_game_advance_fn)(uint64_t mode, uint64_t self,
                                      uint64_t units, uint64_t sub_units,
                                      uint64_t ctrl);

uint32_t nexus_script_port_client_performance_apply(uint32_t fps_limit)
{
    int *errno_ptr = __errno();
    int saved_errno = *errno_ptr;
    uint32_t result = 0;

    if (fps_limit < SPEED_MAX_FPS_LIMIT) {
        if (performance_client_ready() == 0)
            goto out;
        if ((g_perf_flags & 1) != 0 && g_perf_active != 0) {
            result = (uint32_t)nexus_script_port_set("FPSLimit", fps_limit);
            goto out;
        }
    }

out:
    *errno_ptr = saved_errno;
    return result;
}

static uint64_t monotonic_ms(void)
{
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
        return 0;
    return (uint64_t)ts.tv_sec * 1000 + (uint64_t)ts.tv_nsec / 1000000;
}

static int snapshot_single(const char *key, int32_t *triples,
                           int64_t *epoch, int32_t *status)
{
    if (__atomic_load_n(&g_snapshot_keys_ready, __ATOMIC_ACQUIRE) != 1
        || g_snapshot_keys_fn == NULL)
        return 0;

    pthread_once(&g_snapshot_plus_once, snapshot_plus_once_init);
    if (snapshot_plus_active_fn == NULL || snapshot_plus_active_fn() != 1)
        return 0;

    snapshot_keys_fn_t query = (snapshot_keys_fn_t)g_snapshot_keys_fn;
    return query(&key, triples, 1, epoch, status) == 1;
}

uint64_t speed_hook_body(uint64_t self, uint64_t arg1, uint64_t arg2, uint64_t ctrl)
{
    speed_game_advance_fn advance =
        (speed_game_advance_fn)(g_game_base + SPEED_GAME_ADVANCE_OFF);
    uint64_t orig_ret = ((uint64_t (*)(uint64_t, uint64_t, uint64_t, uint64_t))
                         (g_game_base + SPEED_GAME_ADVANCE_OFF))(self, arg1, arg2, ctrl);

    int *errno_ptr = __errno();
    int saved_errno = *errno_ptr;
    uint64_t now_ms = monotonic_ms();

    int64_t epoch = 0;
    int coherent = 0;
    uintptr_t handle = runtime_read_handle();

    if (handle != 0 && g_ctx_self == self && g_ctx_ctrl == ctrl
        && g_speed_hook_installed) {
        uint32_t hook_word = 0;
        if (game_read(handle, g_speed_hook_site, &hook_word, 4) != 0
            && (int32_t)hook_word == (int32_t)g_speed_hook_word) {
            uint64_t tramp[4];
            if (game_read(handle, g_speed_trampoline, tramp, 0x20) != 0
                && tramp[0] == g_speed_trampoline_expect[0]
                && tramp[1] == g_speed_trampoline_expect[1]
                && tramp[2] == g_speed_trampoline_expect[2]
                && tramp[3] == g_speed_trampoline_expect[3]) {
                int32_t triple[3];
                int32_t status = -1;
                if (snapshot_single("speedExploitEnabled", triple, &epoch, &status)) {
                    if (epoch != 0 && status == 0
                        && triple[SNAPSHOT_TRIPLE_FLAGS] == SNAPSHOT_FLAG_ACTIVE
                        && triple[SNAPSHOT_TRIPLE_EFFECTIVE] == 1) {
                        if (g_plan_peer == g_plan_base && g_plan_last_ms != 0
                            && now_ms >= g_plan_last_ms
                            && now_ms - g_plan_last_ms <= 100
                            && g_plan_gate_a != 0 && g_plan_gate_b != 0
                            && g_plan_stamp == g_ctx_token) {
                            if (g_plan_peer_check == g_plan_peer
                                && g_plan_kind == 4 && g_plan_aux == 46)
                                coherent = (g_plan_epoch == (uint64_t)epoch);
                        }
                    }
                }
            }
        }
    }

    uint32_t budget =
        speed_pace_plan(g_speed_pace_state, g_ctx_token, (uint64_t)epoch, now_ms, coherent);

    if (budget != 0) {
        while (runtime_read_handle() != 0 && g_ctx_self == self && g_ctx_ctrl == ctrl) {
            int32_t triple[3];
            int32_t status = -1;
            int64_t snap_epoch = 0;
            if (!snapshot_single("speedExploitEnabled", triple, &snap_epoch, &status))
                break;
            if (snap_epoch == 0 || status != 0
                || triple[SNAPSHOT_TRIPLE_FLAGS] != SNAPSHOT_FLAG_ACTIVE
                || triple[SNAPSHOT_TRIPLE_EFFECTIVE] != 1
                || snap_epoch != epoch)
                break;

            int32_t speed_value = 0;
            if (game_read(handle, (uintptr_t)ctrl + 0xb8, &speed_value, 4) == 0
                || speed_value < 0)
                break;

            advance(0, self, (uint64_t)speed_value / 50,
                    ((uint64_t)speed_value % 50) * 20, ctrl);

            budget--;
            if (budget == 0)
                break;
        }
    }

    *errno_ptr = saved_errno;
    return orig_ret;
}

static const char *const k_ulti_projectile_names[2] = {
    "ControllerUltiProjectile",
    "ControllerUltiOverchargedProjectile",
};

int projectile_controller_switch(const uint64_t *table, const void *const *reader,
                                 uint64_t new_controller, uint64_t *state)
{
    if (new_controller == 0 || table == NULL || state == NULL)
        return 0;

    uint64_t stamp = state[0x13];
    int slot;
    if (stamp == table[0]) {
        slot = 0;
    } else if (stamp == table[3]) {
        slot = 1;
    } else {
        return 0;
    }

    if (state[0] != table[slot * 3 + 2])
        return 0;
    if (reader == NULL || state[0x14] != state[0])
        return 0;

    uintptr_t (*read_qword)(uintptr_t, uintptr_t, void *, uint32_t) =
        (uintptr_t (*)(uintptr_t, uintptr_t, void *, uint32_t))reader[1];

    uint64_t size = 0;
    if (read_qword((uintptr_t)reader[0], stamp, &size, 8) != 1)
        return 0;
    if (size < 0x1001 || size > 0x11000 || (size & 7) != 0)
        return 0;
    if (size != table[slot * 3 + 1])
        return 0;

    char name_a[64];
    if (controller_read_name(reader, table[slot * 3], name_a) == 0)
        return 0;
    if (strcmp(name_a, k_ulti_projectile_names[slot]) != 0)
        return 0;

    char name_b[64];
    if (controller_read_name(reader, new_controller, name_b) == 0)
        return 0;

    const char *expect;
    if (table[6] == new_controller) {
        expect = "SpeedyProjectile";
    } else if (table[slot * 3 + 2] == new_controller) {
        expect = k_ulti_projectile_names[slot];
    } else {
        return 0;
    }

    if (strcmp(name_b, expect) != 0)
        return 0;

    state[0] = new_controller;
    state[0x14] = new_controller;
    return 1;
}
