#define _GNU_SOURCE 1

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>

extern void log_event(const char *category, const char *event, const char *json);

extern uintptr_t g_game_base;
extern uintptr_t game_read(uintptr_t handle, uintptr_t addr, void *out, uint32_t len);
extern uintptr_t runtime_read_handle(void);
extern uintptr_t reader_open_ctx(uintptr_t world, uintptr_t ctrl);
extern int runtime_gate_b(void);
extern int game_read_scaled_position(uintptr_t game_base, uintptr_t ctrl,
                                     void *read_fn, int flags, void *out);
extern int bolt_wall_probe(uintptr_t world, void *read_fn, int flags, void *out_flags);

extern pthread_once_t g_snapshot_plus_once;
extern long (*snapshot_plus_active_fn)(void);
extern void snapshot_plus_once_init(void);
extern void *g_snapshot_keys_fn;
extern int   g_snapshot_keys_ready;

typedef int (*snapshot_keys_fn_t)(const char *const *keys, int32_t *triples,
                                  int count, int64_t *epoch, int32_t *status);
typedef uintptr_t (*game_read_fn_t)(uintptr_t handle, uintptr_t addr, void *out, uint32_t len);

#define SNAPSHOT_TRIPLE_EFFECTIVE 1
#define SNAPSHOT_TRIPLE_FLAGS     2
#define SNAPSHOT_FLAG_ACTIVE      2

extern uint64_t g_ctx_a;
extern uint64_t g_ctx_c;
extern uint64_t g_ctx_bolt_marker;
extern uint64_t g_ctx_own_x;
extern uint64_t g_ctx_own_y;
extern uint64_t g_ctx_own_radius;
extern uint32_t g_ctx_hp_cur;
extern uint32_t g_ctx_hp_max;
extern uint64_t g_ctx_2;
extern uint32_t g_ctx_enemy_count;
extern uintptr_t g_ctx_enemy_list;

extern uint64_t g_plan_mode;
extern uint64_t g_plan_base;
extern uint64_t g_plan_peer_check;
extern uint64_t g_plan_aux;
extern uint64_t g_plan_kind;
extern uint64_t g_bolt_record_epoch;
extern uint64_t g_bolt_record_stamp;
extern uint64_t g_bolt_target_id;
extern float    g_bolt_aim_x;
extern float    g_bolt_aim_y;
extern uint64_t g_bolt_state_word;
extern uint64_t g_bolt_sub_state;
extern uint64_t g_bolt_state_c14;
extern uint64_t g_bolt_last_fire_ms;
extern uint64_t g_bolt_busy;
extern uint64_t g_bolt_state_f0;

extern int bolt_target_position(uint32_t a, uint32_t b, uint32_t c, uint32_t d);
extern int bolt_write_apply(void *payload);

#define BOLT_ARMED_MARKER 17000010u

#define BOLT_GAME_ATTACK_OFF 0xb2e994u

int evasion_key_adapter_family(const char *name)
{
    if (name == NULL)
        return 0;

    if (strcmp(name, "isXrayEnabled") == 0 ||
        strcmp(name, "xRayEnabled") == 0 ||
        strcmp(name, "xrayEnabled") == 0 ||
        strcmp(name, "xrayShowTargetName") == 0 ||
        strcmp(name, "antiAfkEnabled") == 0 ||
        strcmp(name, "hitboxRenderer") == 0 ||
        strcmp(name, "enemyTracer") == 0 ||
        strcmp(name, "dynaJumpEnabled") == 0 ||
        strcmp(name, "attackRangeIndicator") == 0 ||
        strcmp(name, "ballAssistEnabled") == 0 ||
        strcmp(name, "coltModEnabled") == 0 ||
        strcmp(name, "koltModEnabled") == 0 ||
        strcmp(name, "autofarmEnabled") == 0 ||
        strcmp(name, "autofarmAttackEnemies") == 0 ||
        strcmp(name, "characterOutlineEnabled") == 0 ||
        strcmp(name, "boltModEnabled") == 0 ||
        strcmp(name, "boltWallAvoidEnabled") == 0 ||
        strcmp(name, "boltPredictionEnabled") == 0 ||
        strcmp(name, "boltAutoAttackEnabled") == 0 ||
        strcmp(name, "boltSafeExitEnabled") == 0 ||
        strcmp(name, "trophiesAboveHead") == 0 ||
        strcmp(name, "kitNaniModEnabled") == 0 ||
        strcmp(name, "killauraSuper") == 0 ||
        strcmp(name, "killauraGadget") == 0)
        return 1;

    return strcmp(name, "speedLocalMoveEnabled") == 0;
}

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

static int bolt_marker_armed(void)
{
    uint32_t marker = (uint32_t)g_ctx_bolt_marker;
    uint32_t adjusted = marker - 1000000u;
    if (marker + 0xfefc99c0u > 999999u)
        adjusted = marker;
    return adjusted == 0xf4246au;
}

static int feature_active(const char *key, int64_t *epoch_out)
{
    int32_t triple[3];
    int32_t status = -1;
    int64_t epoch = 0;

    if (!snapshot_single(key, triple, &epoch, &status))
        return 0;
    if (epoch == 0 || status != 0
        || triple[SNAPSHOT_TRIPLE_FLAGS] != SNAPSHOT_FLAG_ACTIVE
        || triple[SNAPSHOT_TRIPLE_EFFECTIVE] != 1)
        return 0;

    if (epoch_out)
        *epoch_out = epoch;
    return 1;
}

int bolt_mod_armed(void)
{
    if (!bolt_marker_armed())
        return 0;

    int64_t epoch;
    if (!feature_active("boltModEnabled", &epoch))
        return 0;

    return feature_active("boltAutoAttackEnabled", NULL)
        && runtime_read_handle() != 0;
}

int bolt_write_verify(void *payload)
{
    uint64_t *rec = payload;

    if (runtime_read_handle() == 0)
        return 0;
    if (!bolt_marker_armed())
        return 0;

    int64_t epoch;
    if (!feature_active("boltModEnabled", &epoch))
        return 0;
    if ((uint64_t)epoch != rec[2])
        return 0;

    if (!feature_active("boltAutoAttackEnabled", &epoch))
        return 0;
    if ((uint64_t)epoch != rec[2])
        return 0;

    return reader_open_ctx((uintptr_t)rec[0], rec[1]) != 0;
}

typedef struct {
    uint64_t *payload;
    game_read_fn_t read;
    int (*apply)(void *payload);
    int (*verify)(void *payload);
    uint64_t payload_slots[8];
} bolt_guarded_record_t;

extern int remote_guarded_apply(void *record, uintptr_t target,
                                const void *in, void *out);

void bolt_auto_attack_tick(void *frame)
{
    uint64_t now_ms = monotonic_ms();

    if (frame == NULL)
        return;
    if (!bolt_marker_armed())
        return;

    int64_t epoch;
    if (!feature_active("boltModEnabled", &epoch))
        return;
    if (!feature_active("boltAutoAttackEnabled", &epoch))
        return;

    uintptr_t handle = runtime_read_handle();
    if (handle == 0)
        return;
    if ((uint64_t)epoch != g_bolt_record_epoch)
        return;
    if (g_bolt_record_stamp != g_plan_base || g_bolt_target_id == 0)
        return;
    if (g_plan_aux != 0x84)
        return;

    uint32_t lanes[4] = { (uint32_t)g_plan_mode, (uint32_t)g_bolt_state_word,
                          (uint32_t)g_bolt_sub_state, (uint32_t)g_bolt_state_c14 };
    uint32_t tag[4] = { 2, 0, 0, 0 };
    if (memcmp(lanes, tag, sizeof tag) != 0)
        return;
    if (g_plan_kind != 3 || g_plan_peer_check != g_bolt_record_stamp)
        return;
    if (g_bolt_state_f0 == g_bolt_record_stamp)
        return;
    if (now_ms <= g_bolt_last_fire_ms - 1 || now_ms - g_bolt_last_fire_ms > 0x351)
        return;
    if (runtime_gate_b() == 0)
        return;

    uint8_t wall_flags[2];
    if (bolt_wall_probe(g_ctx_a, (void *)game_read, 0, wall_flags) == 0)
        return;
    if (wall_flags[0] != 0 || wall_flags[1] != 0)
        return;

    if (g_ctx_enemy_count == 0)
        return;

    uint32_t *entry = (uint32_t *)(g_ctx_enemy_list + 0x10);
    for (uint32_t i = (uint32_t)g_ctx_enemy_count; i != 0; i--, entry += 0x10) {
        if (*entry != (uint32_t)g_bolt_target_id)
            continue;

        uint64_t entity = *(uint64_t *)((char *)entry - 0x10);
        if (entity == 0)
            break;

        uint32_t target_id = *entry;
        struct {
            uint64_t field_0;
            uint32_t gid;
            uint32_t field_c;
            float    x;
            float    y;
        } pos = { 0, 0, 0, 0.0f, 0.0f };

        if (game_read_scaled_position(g_game_base, entity,
                                      (void *)game_read, 0, &pos) == 0)
            break;
        if (pos.gid != target_id || pos.field_c == 0)
            break;

        uint32_t nav[2] = { 0, 0 };
        if (bolt_target_position((uint32_t)g_ctx_own_radius,
                                 target_id, nav[0], nav[1]) == 0)
            break;

        float dx = pos.x - (float)(int32_t)g_ctx_own_x;
        float dy = pos.y - (float)(int32_t)g_ctx_own_y;
        float dist = hypotf(dx, dy);

        float r_own = (float)(uint32_t)g_ctx_own_radius / 300.0f;
        float r_tgt = (float)pos.field_c / 300.0f;
        if (r_own <= 0.25f)
            r_own = 0.25f;
        if (r_tgt <= 0.25f)
            r_tgt = 0.25f;
        r_own = fminf(r_own, 1.2f);
        r_tgt = fminf(r_tgt, 1.2f);

        float aim = -1.0f;
        if (dist >= 0.0001f) {
            aim = (g_bolt_aim_x * dx + dy * g_bolt_aim_y) / dist;
        }

        float reach = r_own + r_tgt + 3.25f;
        int in_range = dist <= reach;
        if (!in_range && aim >= 0.15f)
            in_range = dist <= (r_own + r_tgt + 0.85f);
        if (!in_range)
            break;

        uintptr_t target_handle = (uintptr_t)bolt_target_position(
            (uint32_t)g_ctx_own_radius, target_id, nav[0], nav[1]);
        if (target_handle == 0)
            break;

        int32_t xy[2] = { (int32_t)(pos.x * 300.0f), (int32_t)(pos.y * 300.0f) };
        uint64_t pair_word = 0;
        if (game_read(target_handle, g_ctx_a + 0xfac, &pair_word, 8) == 0)
            break;

        uint64_t payload[3] = { g_ctx_a, g_ctx_c, (uint64_t)epoch };
        bolt_guarded_record_t record;
        record.payload = payload;
        record.read = game_read;
        record.apply = bolt_write_apply;
        record.verify = bolt_write_verify;

        int result = remote_guarded_apply(&record, g_ctx_a, &pair_word, xy);
        if (result == 1) {
            if (bolt_write_verify(payload) != 0) {
                uint64_t saved_busy = g_bolt_busy;
                g_bolt_state_word = 5;
                g_bolt_busy = 1;
                g_bolt_sub_state = 1;
                g_bolt_last_fire_ms = now_ms;

                uint64_t attack_result = ((uint64_t (*)(uint64_t, uint64_t))
                    (g_game_base + BOLT_GAME_ATTACK_OFF))(payload[0], payload[1]);

                g_bolt_state_word = 0;
                g_bolt_sub_state = 0;
                g_bolt_state_f0 = g_plan_base;
                g_bolt_busy = saved_busy;

                char json[0xb4];
                snprintf(json, sizeof json,
                         ",\"target_gid\":%u,\"result\":%d",
                         pos.gid, (uint32_t)attack_result);
                log_event("bolt_fire", "original_wrapper", json);
            }
        } else if (result == -1) {
            abort();
        }
        break;
    }
}

extern void wall_grid_acquire(void *grid);
extern void feature_section_begin(void *frame, const char *key, uint32_t section,
                                  void *state0, void *state1, void *state2);
extern int evasion_runtime_gate(void);
extern int entity_team_block(uintptr_t world, uintptr_t ctrl,
                             uint32_t team_a, uint32_t team_b);
extern uintptr_t plan_for_triple(const int32_t *triple);
extern void aim_solution_resolve(void *solution, uintptr_t plan,
                                 int32_t *aim_dx, int32_t *aim_dy, void **aux);
extern float aim_travel_time(void *ctx, uint64_t marker, void *state, int flags);
extern float aim_lead_solve(float dx, float dy, float vx, float vy,
                            float travel, float min_arc, float max_arc, void *out);
extern void aim_path_walk(void *aux, float dist, float lead, void *solution,
                          float *px, float *py);
extern void aim_path_refine(float ox, float oy, float tx, float ty,
                            float vx, float vy, float lead, float *px, float *py);
extern int wall_avoid_step(float ox, float oy, float px, float py, void *state,
                           uint32_t exit_hold_ms, float smooth,
                           float *out_dx, float *out_dy, uint32_t *out_flags);
extern void wall_avoid_adjust(float ox, float oy, void *state, const void *params,
                              uint32_t tick, float *out_dx, float *out_dy);
extern int autofarm_plan_seed(uint64_t epoch, void *out);
extern int entity_los_check(uintptr_t world, uintptr_t ctrl, float x, float y,
                            int32_t aim_a, int32_t aim_b, void *read_fn,
                            int flags, int range);
extern float entity_threat_score(uintptr_t entity);
extern int autofarm_target_pick(void *state, void *query, uint64_t *epoch);
extern void los_read_stub(void);

extern uint64_t g_ctx_frame_no;
extern uint64_t g_ctx_self;
extern uint64_t g_team_world;
extern uint64_t g_team_ctrl;
extern uint64_t g_los_world;
extern uint64_t g_los_ctrl;
extern uint32_t g_ctx_odometer;
extern int32_t g_ctx_aim_a;
extern int32_t g_ctx_aim_b;
extern uint32_t g_evasion_gate;
extern void *g_aim_ctx;
extern void *g_colt_aim_state;
extern uint64_t g_colt_avoid_state;

extern uint8_t g_wall_grid[0x68];
extern uint8_t g_threat_table[32 * 0xf8];
extern uint8_t g_bolt_frame_published[0x110];
extern uint32_t g_bolt_vel_tick;
extern uint32_t g_bolt_vel_frame;
extern float g_bolt_vel_last_x;
extern float g_bolt_vel_last_y;
extern uint64_t g_bolt_track_epoch;
extern int64_t g_bolt_hold_counter;
extern uint64_t g_colt_state[8];
extern uint64_t g_autofarm_state[13];
extern uint64_t g_kitnani_state[3];
extern uint64_t g_speedexp_state[3];

#define GRID_TICK     (*(uint32_t *)(g_wall_grid + 0x0c))
#define GRID_WORLD    (*(uint64_t *)(g_wall_grid + 0x18))
#define GRID_CTRL     (*(uint64_t *)(g_wall_grid + 0x20))
#define GRID_EPOCH    (*(uint64_t *)(g_wall_grid + 0x28))
#define GRID_AUX      (*(uint64_t *)(g_wall_grid + 0x30))
#define GRID_FRAME    (*(uint64_t *)(g_wall_grid + 0x60))
#define GRID_POS_RAW  (*(int64_t *)(g_wall_grid + 0x64))

#define THREAT_ENTITY(i)    (*(uint64_t *)(g_threat_table + (i) * 0xf8))
#define THREAT_ID(i)        (*(uint32_t *)(g_threat_table + (i) * 0xf8 + 0x08))
#define THREAT_VELOCITY(i)  (*(uint64_t *)(g_threat_table + (i) * 0xf8 + 0x14))
#define THREAT_LAST_SEEN(i) (*(int32_t *)(g_threat_table + (i) * 0xf8 + 0x20))

#define ROD_BOLT_TAG_A    (*(const uint64_t *)(uintptr_t)0x112950)
#define ROD_BOLT_TAG_B    (*(const uint64_t *)(uintptr_t)0x112958)
#define ROD_BOLT_PARAM_A  (*(const uint64_t *)(uintptr_t)0x112840)
#define ROD_BOLT_PARAM_B  (*(const uint64_t *)(uintptr_t)0x112848)
#define ROD_BOLT_AUX_A    (*(const uint64_t *)(uintptr_t)0x112c40)
#define ROD_BOLT_AUX_B    (*(const uint64_t *)(uintptr_t)0x112c48)
#define ROD_BOLT_PLAN_ID  (*(const uint64_t *)(uintptr_t)0x10e680)
#define ROD_COLT_TAG_A    (*(const uint64_t *)(uintptr_t)0x112a20)
#define ROD_COLT_TAG_B    (*(const uint64_t *)(uintptr_t)0x112a28)
#define ROD_COLT_PLAN     (*(const uint64_t *)(uintptr_t)0x10e6a8)
#define ROD_COLT_ROUTE    (*(const uint64_t *)(uintptr_t)0x10e720)
#define ROD_AF_TAG_A      (*(const uint64_t *)(uintptr_t)0x112850)
#define ROD_AF_TAG_B      (*(const uint64_t *)(uintptr_t)0x112858)
#define ROD_ADJUST_HI     (*(const uint64_t *)(uintptr_t)0x1c57e0)
#define ROD_ADJUST_FN     (*(const uint64_t *)(uintptr_t)0x1c57e8)
#define ROD_AF_KEYS_LO    (*(const uint64_t *)(uintptr_t)0x1c57f0)
#define ROD_AF_KEYS_HI    (*(const uint64_t *)(uintptr_t)0x1c57f8)

#define BOLT_MOVE_MARKER   0x00f4246au
#define COLT_MOVE_MARKER   0x00f42401u

typedef struct {
    uint32_t id;
    uint32_t team_block;
    uint32_t age;
    uint32_t los;
    uint64_t entity;
    uint64_t pos_pair;
    uint64_t velocity;
    float hp_ratio;
    float threat;
    float confidence;
    uint32_t pad;
} farm_record_t;

static uint64_t pack_vec2(float x, float y)
{
    union { float f; uint32_t u; } cx, cy;
    cx.f = x;
    cy.f = y;
    return ((uint64_t)cy.u << 32) | cx.u;
}

void bolt_movement_tick(void *frame)
{
    snapshot_keys_fn_t query;
    uint32_t marker;
    uint32_t marker_adj;

    if (frame == NULL)
        return;

    memset(frame, 0, 0x110);
    wall_grid_acquire(g_wall_grid);
    feature_section_begin(frame, "kitNaniModEnabled", 0x79,
                          g_kitnani_state, g_kitnani_state + 1, g_kitnani_state + 2);
    if (*(int32_t *)((char *)frame + 0x0c) != 0)
        return;

    marker = (uint32_t)g_ctx_bolt_marker;
    marker_adj = marker - 1000000u;
    if (marker + 0xfefc99c0u > 999999u)
        marker_adj = marker;

    if (marker_adj == BOLT_MOVE_MARKER && g_snapshot_keys_fn != NULL) {
        const char *keys[8] = {
            "boltModEnabled",
            "boltWallAvoidEnabled",
            "boltPredictionEnabled",
            "boltSafeExitEnabled",
            "boltSmoothPercent",
            "boltWallLookahead",
            "boltExitHoldMs",
            "boltTargetRange",
        };
        int32_t triples[24];
        int64_t epoch = 0;
        int32_t status = -1;

        query = (snapshot_keys_fn_t)g_snapshot_keys_fn;
        int query_ret = query(keys, triples, 8, &epoch, &status);

        if (query_ret == 1 && epoch != 0 && status == 0
            && triples[1] == 1
            && evasion_runtime_gate() != 0
            && triples[21] != 0
            && GRID_WORLD == g_ctx_a
            && (GRID_CTRL == g_ctx_c && (uint64_t)epoch == GRID_EPOCH)
            && GRID_FRAME == g_ctx_frame_no
            && (g_evasion_gate & 1) != 0) {

            float own_x = (float)(int32_t)(uint32_t)GRID_POS_RAW / 300.0f;
            float own_y = (float)(int32_t)(uint32_t)((uint64_t)GRID_POS_RAW >> 32) / 300.0f;
            uint64_t velocity = 0;
            float odometer = (float)g_ctx_odometer / 300.0f;

            if (g_bolt_vel_frame == (uint32_t)GRID_FRAME && g_bolt_vel_tick != 0) {
                uint32_t elapsed = GRID_TICK - g_bolt_vel_tick;
                if (g_bolt_vel_tick <= GRID_TICK && elapsed != 0 && elapsed < 9) {
                    float vx = (own_x - g_bolt_vel_last_x) * (30.0f / (float)elapsed);
                    float vy = (own_y - g_bolt_vel_last_y) * (30.0f / (float)elapsed);
                    float speed_sq = vy * vy + vx * vx;
                    if (speed_sq <= 400.0f)
                        velocity = pack_vec2(vx, vy);
                }
            }
            g_bolt_vel_frame = (uint32_t)GRID_FRAME;
            g_bolt_vel_tick = GRID_TICK;
            g_bolt_vel_last_x = own_x;
            g_bolt_vel_last_y = own_y;

            uint32_t target_id = 0;
            float target_x = 0.0f;
            float target_y = 0.0f;
            float target_radius = 0.0f;
            uint64_t threat_velocity = 0;
            float best_score = 1e9f;

            if (g_ctx_enemy_count != 0) {
                float range = (float)triples[21] / 100.0f;
                float range_sq = range * range;

                for (uint32_t i = 0; i < g_ctx_enemy_count; i++) {
                    uint8_t *entry = (uint8_t *)(g_ctx_enemy_list + (uintptr_t)i * 0x40);

                    if (*(int32_t *)(entry + 0x2c) == 0
                        || *(int32_t *)(entry + 0x28) == 0
                        || *(int32_t *)(entry + 0x30) == 0
                        || *(uint64_t *)entry == g_ctx_self)
                        continue;

                    if (entity_team_block(g_team_world, g_team_ctrl,
                                          *(uint32_t *)(entry + 0x18),
                                          *(uint32_t *)(entry + 0x1c)) != 0)
                        continue;

                    float dy = *(float *)(entry + 0x38) - own_y;
                    float dx = *(float *)(entry + 0x34) - own_x;
                    float dist_sq = dx * dx + dy * dy;
                    if (dist_sq < 0.04f || dist_sq > range_sq)
                        continue;

                    uint32_t id = *(uint32_t *)(entry + 0x10);
                    uint32_t age = 0;
                    uint64_t vel = 0;
                    int skip_enemy = 0;

                    for (int t = 0; t < 32; t++) {
                        if (THREAT_ENTITY(t) != *(uint64_t *)entry
                            || THREAT_ID(t) != id)
                            continue;
                        if ((uint32_t)(THREAT_LAST_SEEN(t) - 1) < GRID_TICK) {
                            age = GRID_TICK - (uint32_t)THREAT_LAST_SEEN(t);
                            if (age > 0x12) {
                                skip_enemy = 1;
                            } else {
                                vel = THREAT_VELOCITY(t);
                            }
                            break;
                        }
                    }
                    if (skip_enemy)
                        continue;

                    float aged = (float)age * 0.55f + dist_sq;
                    float score = aged;
                    if (id == *(uint32_t *)(g_bolt_frame_published + 0x14))
                        score = aged * 0.72f;

                    if (score < best_score) {
                        target_radius = (float)*(uint32_t *)(entry + 0x3c) / 300.0f;
                        threat_velocity = vel;
                        best_score = score;
                        target_id = id;
                        target_y = *(float *)(entry + 0x38);
                        target_x = *(float *)(entry + 0x34);
                    }
                }
            }

            uint64_t cont_ctrl;
            uint64_t cont_epoch;
            int reacquire;

            if (*(uint32_t *)(g_bolt_frame_published + 0x38) == 0
                || *(uint64_t *)(g_bolt_frame_published + 0x20) != GRID_CTRL
                || *(uint64_t *)(g_bolt_frame_published + 0x10) != GRID_FRAME
                || *(uint32_t *)(g_bolt_frame_published + 0x14) != target_id) {
                cont_ctrl = GRID_CTRL;
                cont_epoch = (uint64_t)epoch;
                reacquire = 1;
            } else {
                cont_ctrl = *(uint64_t *)(g_bolt_frame_published + 0x20);
                cont_epoch = g_bolt_track_epoch;
                reacquire = g_bolt_track_epoch != (uint64_t)epoch;
                if (reacquire)
                    cont_epoch = (uint64_t)epoch;
            }
            if (reacquire) {
                if (g_bolt_hold_counter == -1)
                    g_bolt_hold_counter = 1;
                else
                    g_bolt_hold_counter = g_bolt_hold_counter + 1;
            }
            g_bolt_track_epoch = cont_epoch;

            *(uint64_t *)((char *)frame + 0x00) = ROD_BOLT_TAG_A;
            *(uint64_t *)((char *)frame + 0x08) = ROD_BOLT_TAG_B;
            *(uint32_t *)((char *)frame + 0x10) = (uint32_t)GRID_FRAME;
            *(uint32_t *)((char *)frame + 0x14) = target_id;
            *(uint32_t *)((char *)frame + 0x18) = (uint32_t)(target_id == 0) << 1;
            *(uint32_t *)((char *)frame + 0x1c) = 0;
            *(uint64_t *)((char *)frame + 0x20) = GRID_CTRL;
            *(uint64_t *)((char *)frame + 0x28) = GRID_WORLD;
            *(uint64_t *)((char *)frame + 0x30) = cont_epoch;
            *(uint64_t *)((char *)frame + 0x38) = (uint64_t)g_bolt_hold_counter;
            *(uint64_t *)((char *)frame + 0x40) = ROD_BOLT_PARAM_A;
            *(uint64_t *)((char *)frame + 0x48) = ROD_BOLT_PARAM_B;
            *(uint64_t *)((char *)frame + 0x50) = ROD_BOLT_PLAN_ID;
            *(uint32_t *)((char *)frame + 0x58) = 0;
            *(uint32_t *)((char *)frame + 0x5c) = GRID_TICK;
            *(uint64_t *)((char *)frame + 0x60) = cont_ctrl;
            *(uint64_t *)((char *)frame + 0x68) = GRID_WORLD;
            *(uint64_t *)((char *)frame + 0x70) = cont_epoch;
            *(uint64_t *)((char *)frame + 0x78) = ROD_BOLT_AUX_A;
            *(uint64_t *)((char *)frame + 0x80) = ROD_BOLT_AUX_B;
            *(uint64_t *)((char *)frame + 0x88) = 0;
            *(uint64_t *)((char *)frame + 0x90) = 0;
            *(uint64_t *)((char *)frame + 0x98) = 0;
            *(uint64_t *)((char *)frame + 0xa0) = GRID_CTRL;
            *(uint64_t *)((char *)frame + 0xa8) = (uint64_t)epoch;
            *(uint64_t *)((char *)frame + 0xb0) = GRID_AUX;
            *(uint32_t *)((char *)frame + 0xb8) = GRID_TICK;
            *(uint32_t *)((char *)frame + 0xbc) = (uint32_t)GRID_FRAME;
            *(uint32_t *)((char *)frame + 0xc0) = target_id;
            *(uint32_t *)((char *)frame + 0xc4) = (uint32_t)triples[4];
            *(uint32_t *)((char *)frame + 0xc8) = (uint32_t)triples[5];
            *(uint32_t *)((char *)frame + 0xcc) = (uint32_t)triples[10];
            *(uint32_t *)((char *)frame + 0xd0) = (uint32_t)triples[12];
            *(uint32_t *)((char *)frame + 0xd4) = (uint32_t)triples[15];
            *(uint32_t *)((char *)frame + 0xd8) = (uint32_t)triples[18];
            *(uint64_t *)((char *)frame + 0xdc) = pack_vec2(own_x, own_y);
            *(uint64_t *)((char *)frame + 0xe4) = velocity;
            *(float *)((char *)frame + 0xec) = odometer;
            *(float *)((char *)frame + 0xf0) = (float)g_ctx_aim_a;
            *(float *)((char *)frame + 0xf4) = (float)g_ctx_aim_b;
            *(float *)((char *)frame + 0xf8) = target_x;
            *(float *)((char *)frame + 0xfc) = target_y;
            *(uint64_t *)((char *)frame + 0x100) = threat_velocity;
            *(float *)((char *)frame + 0x108) = target_radius;
            *(uint32_t *)((char *)frame + 0x10c) = 0;

            memcpy(g_bolt_frame_published, frame, 0x110);
        } else {
            memset(g_bolt_frame_published, 0, 0x110);
            g_bolt_vel_tick = 0;
        }
    } else {
        memset(g_bolt_frame_published, 0, 0x110);
        g_bolt_vel_tick = 0;
    }

    if (*(int32_t *)((char *)frame + 0x0c) != 0)
        return;

    memset(frame, 0, 0x110);

    marker = (uint32_t)g_ctx_bolt_marker;
    marker_adj = marker - 1000000u;
    if (marker + 0xfefc99c0u > 999999u)
        marker_adj = marker;

    int colt_active = 0;

    if (marker_adj == COLT_MOVE_MARKER) {
        const char *ckey[1] = { "coltModEnabled" };
        int32_t ct[3];
        int64_t c_epoch = 0;
        int32_t c_status = -1;

        query = (snapshot_keys_fn_t)g_snapshot_keys_fn;

        if (g_snapshot_keys_ready == 1 && query != NULL
            && query(ckey, ct, 1, &c_epoch, &c_status) == 1
            && c_epoch != 0 && c_status == 0
            && ct[2] == 2 && ct[1] == 1
            && evasion_runtime_gate() != 0
            && (uint64_t)c_epoch == GRID_EPOCH
            && GRID_WORLD == g_ctx_a
            && GRID_FRAME == g_ctx_frame_no
            && GRID_CTRL == g_ctx_c) {

            uintptr_t plan = plan_for_triple(ct);

            if (plan != 0) {
                colt_active = 1;

                uint8_t sol[0xa8];
                int32_t aim_dx = *(int32_t *)(plan + 0x14);
                int32_t aim_dy = *(int32_t *)(plan + 0x18);
                void *aux = NULL;

                aim_solution_resolve(sol, plan, &aim_dx, &aim_dy, &aux);

                float own_x = (float)(int32_t)(uint32_t)GRID_POS_RAW / 300.0f;
                float own_y = (float)(int32_t)(uint32_t)((uint64_t)GRID_POS_RAW >> 32) / 300.0f;
                float aim_tx = *(float *)(sol + 0x6c);
                float aim_ty = *(float *)(sol + 0x70);
                float dx = aim_tx - own_x;
                float dy = aim_ty - own_y;

                float travel = aim_travel_time(g_aim_ctx, g_ctx_bolt_marker,
                                               g_colt_aim_state, 0);
                float lead;
                float aim_vx;
                float aim_vy;
                float pred_x;
                float pred_y;
                float dist;
                float move_dx = dx;
                float move_dy = dy;
                uint32_t avoid_flags = 0;
                int wall_status;

                aim_vx = *(float *)&aim_dx;
                aim_vy = *(float *)&aim_dy;
                lead = aim_lead_solve(dx, dy, aim_vx, aim_vy, travel,
                                      0.035f, 1.0f, NULL);
                pred_x = aim_vx * lead + aim_tx;
                pred_y = aim_vy * lead + aim_ty;
                dist = hypotf(dx, dy);

                aim_path_walk(aux, dist, lead, sol, &pred_x, &pred_y);
                aim_path_refine(own_x, own_y, aim_tx, aim_ty, aim_vx, aim_vy, lead,
                                &pred_x, &pred_y);

                wall_status = wall_avoid_step(own_x, own_y, pred_x, pred_y,
                                              g_colt_state,
                                              *(uint32_t *)(sol + 0x30),
                                              *(float *)(sol + 0x2c),
                                              &move_dx, &move_dy, &avoid_flags);

                if (wall_status >= 0) {
                    uint32_t mode;

                    if (wall_status == 0) {
                        mode = 1;
                    } else {
                        wall_avoid_adjust(own_x, own_y, &g_colt_avoid_state,
                                          (const void *)(uintptr_t)0x1c57d8,
                                          GRID_TICK, &move_dx, &move_dy);
                        mode = 2;
                    }

                    *(uint64_t *)((char *)frame + 0x00) = ROD_COLT_TAG_A;
                    *(uint64_t *)((char *)frame + 0x08) = ROD_COLT_TAG_B;
                    *(uint32_t *)((char *)frame + 0x10) = (uint32_t)GRID_FRAME;
                    *(uint32_t *)((char *)frame + 0x14) = *(uint32_t *)(sol + 0x60);
                    *(uint64_t *)((char *)frame + 0x18) = 0;
                    *(uint64_t *)((char *)frame + 0x20) = GRID_CTRL;
                    *(uint64_t *)((char *)frame + 0x28) = GRID_WORLD;
                    *(uint64_t *)((char *)frame + 0x30) = (uint64_t)c_epoch;
                    *(uint64_t *)((char *)frame + 0x38) = g_colt_state[1];
                    *(uint64_t *)((char *)frame + 0x40) = ROD_COLT_ROUTE;
                    *(uint32_t *)((char *)frame + 0x48) = mode;
                    *(uint64_t *)((char *)frame + 0x4c) = ROD_COLT_PLAN;
                    *(uint32_t *)((char *)frame + 0x54) = 2;
                    *(uint32_t *)((char *)frame + 0x58) = (uint32_t)(wall_status == 0);
                    *(uint32_t *)((char *)frame + 0x5c) = GRID_TICK;
                    *(uint64_t *)((char *)frame + 0x60) = GRID_CTRL;
                    *(uint64_t *)((char *)frame + 0x68) = GRID_WORLD;
                    *(uint64_t *)((char *)frame + 0x70) = (uint64_t)c_epoch;
                    *(float *)((char *)frame + 0x78) = move_dx;
                    *(float *)((char *)frame + 0x7c) = move_dy;
                    *(float *)((char *)frame + 0x80) = 165.0f;
                    *(uint32_t *)((char *)frame + 0x84) = avoid_flags;
                    memset((char *)frame + 0x88, 0, 0x88);
                }
            }
        }
    }

    if (!colt_active)
        memset(g_colt_state, 0, sizeof g_colt_state);

    if (*(int32_t *)((char *)frame + 0x0c) == 0) {
        const char *akey[1] = { "autofarmEnabled" };
        int32_t at[3];
        int64_t a_epoch = 0;
        int32_t a_status = -1;
        int farm_ok = 0;

        query = (snapshot_keys_fn_t)g_snapshot_keys_fn;

        if (g_snapshot_keys_ready == 1 && query != NULL
            && query(akey, at, 1, &a_epoch, &a_status) == 1
            && a_epoch != 0 && a_status == 0
            && at[2] == 2 && at[1] == 1
            && evasion_runtime_gate() != 0
            && (uint64_t)a_epoch == GRID_EPOCH
            && GRID_WORLD == g_ctx_a
            && GRID_CTRL == g_ctx_c
            && GRID_FRAME == g_ctx_frame_no
            && g_evasion_gate != 0) {

            const char *fkeys[2] = { "autofarmFollowTarget", "combatFireInterval" };
            int32_t ft[6];
            int64_t f_epoch = 0;
            int32_t f_status = -1;

            if (query(fkeys, ft, 2, &f_epoch, &f_status) == 1
                && (uint64_t)f_epoch == (uint64_t)a_epoch
                && f_status == 0
                && ft[0] >= 0 && ft[0] < 2) {
                farm_ok = 1;

                if (g_autofarm_state[12] != (uint64_t)a_epoch) {
                    memset(g_autofarm_state, 0, sizeof g_autofarm_state);
                    g_autofarm_state[12] = (uint64_t)a_epoch;
                }

                uint64_t seed[3] = { 0, 0, 0 };
                if (ft[0] == 0)
                    autofarm_plan_seed((uint64_t)a_epoch, seed);

                farm_record_t records[32];
                uint32_t record_count = 0;

                for (uint32_t i = 0;
                     i < g_ctx_enemy_count && record_count < 32;
                     i++) {
                    uint8_t *entry = (uint8_t *)(g_ctx_enemy_list + (uintptr_t)i * 0x40);

                    if (*(int32_t *)(entry + 0x28) == 0
                        || *(int32_t *)(entry + 0x2c) == 0
                        || *(int32_t *)(entry + 0x30) == 0
                        || *(uint64_t *)entry == g_ctx_self)
                        continue;

                    farm_record_t *rec = &records[record_count];

                    rec->id = *(uint32_t *)(entry + 0x10);
                    rec->team_block = (uint32_t)entity_team_block(
                        g_team_world, g_team_ctrl,
                        *(uint32_t *)(entry + 0x18),
                        *(uint32_t *)(entry + 0x1c));
                    rec->los = (uint32_t)(entity_los_check(
                        g_los_world, g_los_ctrl,
                        *(float *)(entry + 0x34), *(float *)(entry + 0x38),
                        g_ctx_aim_a, g_ctx_aim_b, los_read_stub, 0, 0x40) == 1);

                    uint32_t hp_max = *(uint32_t *)(entry + 0x24);
                    float hp_ratio = 1.0f;
                    if (hp_max != 0 && *(uint32_t *)(entry + 0x20) <= hp_max)
                        hp_ratio = (float)*(uint32_t *)(entry + 0x20) / (float)hp_max;

                    rec->entity = *(uint64_t *)entry;
                    rec->pos_pair = *(uint64_t *)(entry + 0x34);
                    rec->threat = entity_threat_score(*(uint64_t *)entry);
                    rec->age = 0;
                    rec->velocity = 0;
                    rec->confidence = 1.0f;

                    for (int t = 0; t < 32; t++) {
                        if (THREAT_ENTITY(t) != rec->entity
                            || THREAT_ID(t) != rec->id)
                            continue;
                        if ((uint32_t)(THREAT_LAST_SEEN(t) - 1) < GRID_TICK
                            && GRID_TICK - (uint32_t)THREAT_LAST_SEEN(t) < 0x13) {
                            uint32_t age = GRID_TICK - (uint32_t)THREAT_LAST_SEEN(t);
                            float decay = 1.0f - 0.08f * (float)age;
                            if (decay <= 0.15f)
                                decay = 0.15f;
                            rec->age = age;
                            rec->velocity = THREAT_VELOCITY(t);
                            rec->confidence = (4 < age) ? decay : 1.0f;
                            break;
                        }
                    }

                    rec->hp_ratio = hp_ratio;
                    record_count++;
                }

                float own_x = (float)(int32_t)(uint32_t)GRID_POS_RAW / 300.0f;
                float own_y = (float)(int32_t)(uint32_t)((uint64_t)GRID_POS_RAW >> 32) / 300.0f;
                float hp_ratio_own = 1.0f;
                if (g_ctx_hp_max != 0 && g_ctx_hp_cur <= g_ctx_hp_max)
                    hp_ratio_own = (float)g_ctx_hp_cur / (float)g_ctx_hp_max;

                uint8_t fq[0x50];
                memset(fq, 0, sizeof fq);
                *(uint64_t *)(fq + 0x00) = GRID_CTRL;
                *(uint64_t *)(fq + 0x08) = GRID_WORLD;
                *(uint64_t *)(fq + 0x10) = GRID_AUX;
                *(uint64_t *)(fq + 0x18) = (1ull << 32) | (uint32_t)GRID_FRAME;
                *(uint32_t *)(fq + 0x20) = (uint32_t)ft[0];
                *(float *)(fq + 0x24) = own_x;
                *(float *)(fq + 0x28) = own_y;
                *(float *)(fq + 0x2c) = hp_ratio_own;
                *(float *)(fq + 0x30) = entity_threat_score((uintptr_t)g_ctx_self);
                *(float *)(fq + 0x34) = (float)g_ctx_aim_a;
                *(float *)(fq + 0x38) = (float)g_ctx_aim_b;
                *(farm_record_t **)(fq + 0x40) = records;
                *(uint32_t *)(fq + 0x44) = (uint32_t)seed[1];
                *(uint32_t *)(fq + 0x48) = record_count;

                int pick = autofarm_target_pick(g_autofarm_state, fq, (uint64_t *)&f_epoch);
                uint32_t mode = (pick != 0) ? 2u : 1u;

                *(uint64_t *)((char *)frame + 0x00) = ROD_AF_TAG_A;
                *(uint64_t *)((char *)frame + 0x08) = ROD_AF_TAG_B;
                *(uint32_t *)((char *)frame + 0x10) = (uint32_t)GRID_FRAME;
                *(uint32_t *)((char *)frame + 0x14) = (uint32_t)ROD_ADJUST_HI;
                *(uint32_t *)((char *)frame + 0x1c) = (uint32_t)((uint64_t)f_epoch >> 32);
                *(uint64_t *)((char *)frame + 0x20) = GRID_CTRL;
                *(uint64_t *)((char *)frame + 0x28) = GRID_WORLD;
                *(uint64_t *)((char *)frame + 0x30) = (uint64_t)a_epoch;
                *(uint64_t *)((char *)frame + 0x38) = g_autofarm_state[3];
                *(uint64_t *)((char *)frame + 0x40) = ROD_COLT_ROUTE;
                *(uint32_t *)((char *)frame + 0x48) = mode;
                *(uint64_t *)((char *)frame + 0x4c) = ROD_COLT_PLAN;
                *(uint32_t *)((char *)frame + 0x54) = (uint32_t)ROD_ADJUST_FN;
                *(uint32_t *)((char *)frame + 0x58) = (uint32_t)(pick == 0);
                *(uint32_t *)((char *)frame + 0x5c) = GRID_TICK;
                *(uint64_t *)((char *)frame + 0x60) = GRID_CTRL;
                *(uint64_t *)((char *)frame + 0x68) = GRID_WORLD;
                *(uint64_t *)((char *)frame + 0x70) = (uint64_t)a_epoch;
                *(uint64_t *)((char *)frame + 0x78) =
                    ((ROD_AF_KEYS_LO & 0xffffffffu) << 32) | (ROD_ADJUST_FN >> 32);
                *(uint64_t *)((char *)frame + 0x80) =
                    (ROD_AF_KEYS_LO >> 32) | ((ROD_AF_KEYS_HI & 0xffffffffu) << 32);
                memset((char *)frame + 0x90, 0, 0x78);
                return;
            }
        }

        if (!farm_ok)
            memset(g_autofarm_state, 0, sizeof g_autofarm_state);

        if (*(int32_t *)((char *)frame + 0x0c) == 0)
            feature_section_begin(frame, "speedExploitEnabled", 0x2e,
                                  g_speedexp_state, g_speedexp_state + 1,
                                  g_speedexp_state + 2);
    }
}
