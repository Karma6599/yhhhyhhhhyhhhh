#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>

extern int *__errno(void);
extern void log_event(const char *category, const char *event, const char *json);

extern uintptr_t g_game_base;
extern uintptr_t game_read(uintptr_t handle, uintptr_t addr, void *out, uint32_t len);
extern uintptr_t runtime_read_handle(void);
extern uintptr_t battle_reader_open(void);
extern long remote_write(uint32_t fd, const void *buf, long len, uintptr_t addr);
extern int32_t g_game_fd;
extern long *tls_cell_get(void *key);
extern char g_tls_key_spectate_name[];
extern char g_tls_key_spectate_camera[];

extern pthread_once_t g_snapshot_plus_once;
extern long (*snapshot_plus_active_fn)(void);
extern void snapshot_plus_once_init(void);
extern void *g_snapshot_keys_fn;
extern int   g_snapshot_keys_ready;

typedef int (*snapshot_keys_fn_t)(const char *const *keys, int32_t *triples,
                                  int count, int64_t *epoch, int32_t *status);

#define SNAPSHOT_TRIPLE_EFFECTIVE 1
#define SNAPSHOT_TRIPLE_FLAGS     2
#define SNAPSHOT_FLAG_ACTIVE      2

extern uint64_t g_ctx_token;
extern uint64_t g_ctx_self;
extern uint64_t g_ctx_2;
extern uint64_t g_ctx_5;
extern uint64_t g_ctx_b;

extern uint64_t g_plan_peer;
extern uint64_t g_plan_base;
extern uint64_t g_plan_last_ms;
extern uint64_t g_plan_mode;
extern uint64_t g_plan_zero;
extern uint64_t g_plan_ok;
extern uint64_t g_plan_kind;

extern uint8_t g_spectate_active;
extern uint8_t g_spectate_busy;
extern uint8_t g_spectate_reject;
extern uint8_t g_runtime_in_call;
extern uint64_t g_spectate_state[12];
extern uintptr_t g_spectate_session_addr;
extern uintptr_t g_spectate_session_buf;
extern uintptr_t g_camera_state;
extern uintptr_t g_brawltv_obj;
extern int32_t g_screen_w;
extern int32_t g_screen_h;

extern int32_t g_spectate_cfg_scale;
extern int32_t g_spectate_cfg_off_a;
extern int32_t g_spectate_cfg_off_b;
extern int32_t g_spectate_cfg_off_c;
extern int32_t g_spectate_mode;

extern uint64_t (*g_spectate_name_orig)(void *obj);
extern void (*g_spectate_mode_orig)(void *obj);

extern int spectate_name_transform(uint64_t *state, uint64_t *record, void *out12);
extern int spectate_camera_plan(uint64_t *state, float *a, float *b, float *out);
extern int spectate_session_resolve(uint64_t *state, uint32_t handle,
                                    void *buf, uint32_t size);

#define SPECTATE_CALLER_CHECK_OFF 0xb3062cu
#define SPECTATE_CAMERA_ORIG_OFF  0x671630u
#define SPECTATE_BRAWLTV_ENTER_OFF 0x11a2830u
#define SPECTATE_BATTLE_FLAG_OFF  0x1303f20u

static const uint64_t k_spectate_clean_tag[2] = { 100, 0 };

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

static int local_move_active(void)
{
    int32_t triple[3];
    int32_t status = -1;
    int64_t epoch = 0;

    if (!snapshot_single("speedLocalMoveEnabled", triple, &epoch, &status))
        return 0;
    return epoch != 0 && status == 0
        && triple[SNAPSHOT_TRIPLE_FLAGS] == SNAPSHOT_FLAG_ACTIVE
        && triple[SNAPSHOT_TRIPLE_EFFECTIVE] == 1;
}

static int spectate_config_dirty(void)
{
    struct {
        uint64_t scale_pair;
        uint32_t off_a;
        uint32_t mode;
    } cfg;

    cfg.scale_pair = (uint64_t)(uint32_t)g_spectate_cfg_scale
                   | ((uint64_t)(uint32_t)g_spectate_cfg_off_a << 32);
    cfg.off_a = (uint32_t)g_spectate_cfg_off_b;
    cfg.mode = (uint32_t)g_spectate_mode;

    if (memcmp(&cfg, k_spectate_clean_tag, sizeof cfg) != 0)
        return 1;
    if (g_spectate_cfg_off_b != 0 || g_spectate_cfg_off_c != 0)
        return 1;
    return 0;
}

void spectate_state_reset(void)
{
    if (g_spectate_active == 1 && local_move_active()
        && runtime_read_handle() != 0)
        return;

    memset(g_spectate_state, 0, sizeof g_spectate_state);
}

uint64_t spectate_name_hook(void *obj)
{
    int *errno_ptr = __errno();
    int saved_errno = *errno_ptr;
    long *tls = tls_cell_get(g_tls_key_spectate_name);
    int patched = 0;
    uint64_t target = 0;
    uint8_t name_orig[12];
    uint8_t name_new[12];
    uint64_t result;

    if (*tls != 0 || g_spectate_active != 1 || !local_move_active())
        goto out;

    uintptr_t handle = runtime_read_handle();
    if (handle == 0)
        goto out;

    if (game_read(handle, (uintptr_t)obj + 0x10, &target, 8) == 0)
        goto out;
    if (target - 0x10000 > 0xfffffffffffedfffull || (target & 7) != 0
        || target != g_ctx_self)
        goto out;

    if (game_read(handle, target + 0x30, name_orig, 12) == 0)
        goto out;

    uint64_t now_ms = monotonic_ms();

    uint64_t record[8];
    record[0] = g_ctx_token;
    record[1] = now_ms;
    record[2] = g_ctx_2;
    record[3] = 0;
    record[4] = 0;
    record[5] = 0;
    memcpy(&record[6], name_orig, 12);

    if (g_plan_peer != g_plan_base || now_ms < g_plan_last_ms
        || now_ms - g_plan_last_ms > 0xfa) {
        record[3] = 0;
        record[4] = 0;
        record[5] = 0;
    }

    if (spectate_name_transform(g_spectate_state, record, name_new) != 0) {
        patched = 1;
        *tls = 1;

        if (remote_write(g_game_fd, name_new, 12, target + 0x30) != 12) {
            if (remote_write(g_game_fd, name_orig, 12, target + 0x30) != 12)
                abort();
            uint8_t verify[12];
            if (game_read(12, target + 0x30, verify, 12) == 0
                || memcmp(verify, name_orig, 12) != 0)
                abort();
            patched = 0;
            *tls = 0;
        }
    }

out:
    *errno_ptr = saved_errno;
    result = g_spectate_name_orig(obj);
    saved_errno = *errno_ptr;

    if (patched) {
        if (remote_write(g_game_fd, name_orig, 12, target + 0x30) != 12) {
            uint8_t verify[12];
            if (game_read(12, target + 0x30, verify, 12) == 0
                || memcmp(verify, name_orig, 12) != 0) {
                log_event("fatal", "spectator_restore_unverified", NULL);
                abort();
            }
        }
        *tls = 0;
    }

    *errno_ptr = saved_errno;
    return result;
}

void spectate_camera_mode_hook(void *obj)
{
    int *errno_ptr = __errno();
    int saved_errno = *errno_ptr;
    long *tls = tls_cell_get(g_tls_key_spectate_camera);
    long tls_prev = *tls;
    int patched = 0;
    int32_t mode_orig = 0;

    if ((g_game_base + SPECTATE_CALLER_CHECK_OFF) == (uintptr_t)__builtin_return_address(0)
        && g_ctx_5 == (uint64_t)(uintptr_t)obj
        && g_spectate_active == 1
        && local_move_active()
        && runtime_read_handle() != 0) {

        *tls = (long)(uintptr_t)obj;
        uintptr_t handle = battle_reader_open();

        if (handle != 0 && spectate_config_dirty()) {
            if (game_read(handle, (uintptr_t)obj + 0x92c, &mode_orig, 4) != 0) {
                int32_t mode_new = (g_spectate_mode == 3) ? 4 : 0;
                if (remote_write(g_game_fd, &mode_new, 4,
                                 (uintptr_t)obj + 0x92c) == 4) {
                    patched = 1;
                } else {
                    if (remote_write(g_game_fd, &mode_orig, 4,
                                     (uintptr_t)obj + 0x92c) != 4)
                        abort();
                }
            }
        } else {
            *tls = 0;
        }
    }

    *errno_ptr = saved_errno;
    g_spectate_mode_orig(obj);
    saved_errno = *errno_ptr;

    if (patched) {
        if (remote_write(g_game_fd, &mode_orig, 4, (uintptr_t)obj + 0x92c) != 4) {
            int32_t verify = 0;
            if (game_read(4, (uintptr_t)obj + 0x92c, &verify, 4) == 0
                || verify != mode_orig)
                abort();
        }
    }

    *errno_ptr = saved_errno;
    *tls = tls_prev;
}

static int vec3_finite(const float *v)
{
    for (int i = 0; i < 3; i++) {
        float a = v[i] < 0.0f ? -v[i] : v[i];
        if (!isfinite(a))
            return 0;
    }
    return 1;
}

void spectate_camera_transform_hook(void *obj, float *vec_a, float *vec_b, uint64_t arg4)
{
    int *errno_ptr = __errno();
    int saved_errno = *errno_ptr;
    long *tls = tls_cell_get(g_tls_key_spectate_camera);

    if (*tls != 0 && g_spectate_active == 1 && local_move_active()
        && runtime_read_handle() != 0
        && g_spectate_state[0] == g_ctx_token
        && g_spectate_state[2] == g_ctx_self
        && (float *)(*tls + 0x8e8) == vec_a
        && (float *)(*tls + 0x8f4) == vec_b) {

        float plan_out[6];
        if (spectate_camera_plan(g_spectate_state, vec_a, vec_b, plan_out) != 0) {
            vec_a = plan_out + 3;
            vec_b = plan_out;
        }
    }

    if (*tls != 0) {
        uintptr_t handle = battle_reader_open();
        if (handle != 0) {
            int32_t cam_w = 0, cam_h = 0;
            if (g_camera_state == 0
                || game_read(handle, g_camera_state + 0xcc, &cam_w, 4) == 0) {
                cam_w = 0;
                cam_h = 0;
            } else {
                if (game_read(handle, g_camera_state + 0xd0, &cam_h, 4) == 0)
                    cam_h = 0;
            }

            uint8_t battle_flag = 0;
            uint32_t battle = 0xffffffff;
            if (game_read(handle, g_game_base + SPECTATE_BATTLE_FLAG_OFF,
                          &battle_flag, 1) != 0)
                battle = (battle_flag <= 1) ? battle_flag : 0xffffffffu;

            if (spectate_config_dirty() && vec3_finite(vec_a) && vec3_finite(vec_b)) {
                float a_x = vec_a[0], a_y = vec_a[1], a_z = vec_a[2];
                float b_x = vec_b[0], b_y = vec_b[1], b_z = vec_b[2];
                float scale = (float)g_spectate_cfg_scale / 100.0f;
                float out_a0 = a_x, out_a1 = a_y, out_a2;
                float out_b0 = b_x, out_b1 = b_y, out_b2 = b_z;

                float w = (float)g_screen_w / 300.0f;
                float h = (float)g_screen_h / 300.0f;

                if (g_spectate_mode == 1) {
                    if (!isfinite(w) || !isfinite(h))
                        goto call_orig;

                    out_b1 = h * -300.0f;
                    out_a1 = h * -300.0f + (a_y - b_y);
                    out_a0 = w * 300.0f + (a_x - b_x);
                    scale = scale * 4000.0f;
                    out_b0 = w * 300.0f;
                    out_b2 = 300.0f;
                    out_a2 = a_z;
                } else if (g_spectate_mode == 2) {
                    if (!((uint32_t)(cam_w - 0x7531) < 0xffff8ad0u
                          && (uint32_t)(cam_h - 0x7531) < 0xffff8ad0u
                          && battle <= 1))
                        goto call_orig;

                    float hh = (float)cam_h;
                    float sn = (battle != 0) ? 0.58779f : -0.58779f;
                    out_b1 = hh * -0.5f;
                    out_b0 = (float)cam_w * 0.5f;
                    scale = hh * 4.0451f * scale;
                    out_a1 = hh * -0.5f + hh * 5.0f * sn;
                    out_b2 = 0.0f;
                    out_a0 = out_b0;
                    out_a2 = a_z;
                } else {
                    out_a1 = a_y;
                    scale = scale * a_z;
                    out_a2 = a_z;
                }

                float off_a = (float)g_spectate_cfg_off_a;
                float off_b = (float)g_spectate_cfg_off_b;
                float off_c = (float)g_spectate_cfg_off_c;

                out_a0 += (g_spectate_mode == 1) ? off_a : 0.0f;
                out_a2 = scale + (float)g_spectate_cfg_off_a;
                out_b0 += (g_spectate_mode == 1) ? off_b : 0.0f;
                out_a1 += off_b;
                out_b1 += (g_spectate_mode == 1) ? off_b : 0.0f;
                out_b2 += off_c;

                static float s_vec_a[3], s_vec_b[3];
                s_vec_a[0] = out_a0;
                s_vec_a[1] = out_a1;
                s_vec_a[2] = out_a2;
                s_vec_b[0] = out_b0;
                s_vec_b[1] = out_b1;
                s_vec_b[2] = out_b2;
                vec_a = s_vec_a;
                vec_b = s_vec_b;
            }
        }
    }

call_orig:
    *errno_ptr = saved_errno;
    ((void (*)(void *, float *, float *, uint64_t))
     (g_game_base + SPECTATE_CAMERA_ORIG_OFF))(obj, vec_a, vec_b, arg4);
}

int spectate_brawltv_enter(const uint64_t *rec)
{
    if (g_spectate_busy != 0 || g_spectate_active != 1 || !local_move_active())
        return 0;

    uintptr_t handle = runtime_read_handle();
    if (handle == 0)
        return 0;

    uint32_t session = 0;
    if (game_read(handle, g_spectate_session_addr, &session, 4) == 0)
        return 0;

    uint8_t session_buf[0x208];
    if (game_read(handle, g_spectate_session_buf, session_buf, 0x208) == 0)
        return 0;

    uintptr_t resolved = (uintptr_t)spectate_session_resolve(
        (uint64_t *)&g_spectate_session_addr, session, session_buf, 0x208);
    if (resolved == 0)
        return 0;

    uint64_t position_handle = 0;
    if (game_read(resolved, g_ctx_b + 0x58, &position_handle, 8) == 0)
        return 0;
    if (position_handle + 0x2000 >= 0x12000 || (position_handle & 7) != 0)
        return 0;

    if (position_handle != rec[0])
        return 0;

    int32_t mode = 0;
    if (game_read(0, rec[1] + 8, &mode, 4) == 0 || mode != 2)
        return 0;

    if (g_plan_kind == 4) {
        if (g_spectate_reject != 0)
            return 0;
    } else {
        if (g_plan_mode == 2 || g_plan_zero != 0 || g_plan_ok != 0
            || g_spectate_reject != 0)
            return 0;
    }

    if (g_runtime_in_call == 0) {
        ((void (*)(uint64_t))(g_game_base + SPECTATE_BRAWLTV_ENTER_OFF))(rec[1]);
        return 1;
    }
    return 0;
}
