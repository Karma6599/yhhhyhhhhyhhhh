#define _GNU_SOURCE 1

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
extern int __android_log_write(int prio, const char *tag, const char *text);

extern void log_event(const char *category, const char *event, const char *json);

extern int colt_movement_apply(uint64_t ctx, uint64_t target_obj);
extern int movement_ctx_valid(uint64_t ctx, uint64_t obj);
extern uintptr_t game_read(uintptr_t handle, uintptr_t addr, void *out, uint32_t len);
extern int game_object_resolve(uintptr_t engine, uintptr_t obj, void *read_fn,
                               int flags, void *out);
extern int remote_guarded_apply(void *record, uintptr_t target,
                                const void *in, void *out);
extern int page_perm_check(uintptr_t addr, int64_t *out);
extern int skill_context_resolve(uintptr_t engine, uintptr_t obj, uint64_t member,
                                 void *read_fn, int flags, void *out);
extern int ability_input_check(uint64_t ctx, uint64_t obj, void *context_out,
                               int32_t *ability_out);
extern int event_precheck(int zero, int32_t *event);
extern int skill_precheck(int32_t *event, uint64_t *out);
extern void input_event_refresh(int32_t *event);
extern void input_event_passthrough(int32_t *event);
extern int aim_proposal_solve(void *engine_state, void *request, void *inputs,
                              void **lease_slot, void *proposal);
extern uint64_t engine_frame_time(int kind);
extern int actor_refresh(uint64_t ctrl, uint64_t obj);
extern int smartaim_xy_apply(void *payload);
extern int smartaim_xy_verify(void *payload);
extern int skill_xy_apply(void *payload);
extern int skill_xy_verify(void *payload);
extern void map_publish(void *map_state, uint32_t generation, uint64_t region,
                        int64_t ctrl, int ok, void *out);
extern void map_clear(void);
extern int route_install_check(void);
extern int route_install(void *ctx, void *world, void *params, int flags);
extern int counters_snapshot(void *counters_ctx, void *out);
extern void aim_feed_route_install(uint64_t ctrl, uint64_t list,
                                   uint32_t kind, uint32_t flags);
extern void fetch_read_cb(void);
extern void smartaim_skill_log(int32_t *event, void *proposal, void *lease,
                            void *config, const char *reason, int mode);

extern char g_smartaim_armed;
extern char g_smartaim_active;
extern uint64_t g_smartaim_event_count;
extern uint64_t g_smartaim_xy_writes;
extern uint64_t g_smartaim_last_log_ms;
extern uint64_t g_fire_state;
extern uint64_t g_skill_event_count;
extern uint64_t g_skill_commits;
extern uint64_t g_skill_last_log_ms;
extern void *g_config_query_fn;
extern void *g_input_lease_acquire;
extern void *g_input_lease_commit;
extern void *g_proposal_engine_state;
extern void *g_counters_ctx;
extern char g_input_lock;
extern uint32_t g_ctx_ready;
extern uint32_t g_evasion_gate;
extern uint32_t g_map_epoch;
extern uint64_t g_map_generation;
extern uint64_t g_map_active;
extern uint64_t g_map_fail_a;
extern uint64_t g_map_fail_b;
extern uint64_t g_map_last_log_ms;
extern uint64_t g_route_active;
extern uint64_t g_route_gen;
extern char g_route_armed;
extern long g_main_tid;
extern uint64_t g_engine_epoch;
extern uint64_t g_colt_epoch;
extern uintptr_t g_engine_base;
extern uint64_t g_colt_ctx;
extern uint64_t g_proposal_state;

extern uint8_t g_ctx_block[0x1d0];

#define CTX64(a)  (*(uint64_t *)(g_ctx_block + ((a) - 0x20f628u)))
#define CTX32(a)  (*(uint32_t *)(g_ctx_block + ((a) - 0x20f628u)))
#define CTXFLT(a) (*(float *)(g_ctx_block + ((a) - 0x20f628u)))

extern uint8_t g_skill_blob[0x2c0];
#define SKB(a) (*(uint64_t *)(g_skill_blob + ((a) - 0x215680u)))

#define ROD_PROPOSAL_VT   (*(const uint64_t *)(uintptr_t)0x10e7f8)
#define ROD_CONFIG_VT     (*(const uint64_t *)(uintptr_t)0x10e7d0)
#define ROD_LEASE_VT      (*(const uint64_t *)(uintptr_t)0x10e700)
#define ROD_SKILL_REQ_VT  (*(const uint64_t *)(uintptr_t)0x10e6e0)
#define ROD_REQUEST_VT    (*(const uint64_t *)(uintptr_t)0x10e588)
#define ROD_SKILL_OUT_A   (*(const uint64_t *)(uintptr_t)0x112940)
#define ROD_SKILL_OUT_B   (*(const uint64_t *)(uintptr_t)0x112948)
#define ROD_SKILL_REQ2_A  (*(const uint64_t *)(uintptr_t)0x112910)
#define ROD_SKILL_REQ2_B  (*(const uint64_t *)(uintptr_t)0x112918)
#define ROD_MAP_VT        (*(const uint64_t *)(uintptr_t)0x10e550)
#define ROD_ROUTE_VT      (*(const uint64_t *)(uintptr_t)0x10e760)
#define ROD_COUNTERS_VT   (*(const uint64_t *)(uintptr_t)0x10e668)
#define ROD_XY_SENTINEL   ((const uint64_t *)(uintptr_t)0x112be0)

#define ENGINE_STATIC_CHAIN_OFF 0x1307e20u
#define ENGINE_ATTACK_OFF       0xb2e994u
#define ENGINE_CALLER_A_OFF     0xb2d5c8u
#define ENGINE_CALLER_B_OFF     0xb2d3d4u
#define ENGINE_CALLER_C_OFF     0xb380e0u

static void input_lock_acquire(void)
{
    while (__atomic_test_and_set(&g_input_lock, __ATOMIC_ACQUIRE))
        ;
}

static void input_lock_release(void)
{
    g_input_lock = 0;
}

static uint64_t monotonic_ms(void)
{
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
        return 0;
    return (uint64_t)ts.tv_sec * 1000 + (uint64_t)ts.tv_nsec / 1000000;
}

static int xy_matches_sentinel(uint32_t lo, uint32_t hi)
{
    uint64_t pair[2];

    pair[0] = ((uint64_t)hi << 32) | lo;
    pair[1] = ((uint64_t)hi << 32) | lo;
    return memcmp(pair, ROD_XY_SENTINEL, 16) == 0;
}

void smartaim_input_hook(void *ctx, int32_t *event)
{
    (void)ctx;

    const char *reason = "configuration_or_generation";
    uint64_t untouched = 0;
    int untouched_known = 0;
    uint32_t proposed_lo = 0;
    uint32_t proposed_hi = 0;
    int commit_status = 0;
    int committed = 0;
    uint64_t config[32];
    uint64_t lease[14];
    uint64_t request[24];
    uint64_t proposal[20];
    int32_t ability[2];
    int applied;
    uint64_t now;

    if (event == NULL || g_smartaim_armed == 0 || g_main_tid == 0)
        return;
    if (gettid() != g_main_tid)
        return;
    if (g_fire_state >= 4 || g_fire_state == 2)
        return;

    if (event[0] == 2 && event[1] == 0x98 && event[2] == 2
        && *(uint64_t *)(event + 4) == g_engine_epoch
        && *(uint64_t *)(event + 8) == g_engine_base
        && *(uint64_t *)(event + 10) == g_engine_base + ENGINE_ATTACK_OFF
        && *(uint64_t *)(event + 0x14) == *(uint64_t *)(event + 0xc)
        && *(uint64_t *)(event + 0x16) == *(uint64_t *)(event + 0xe))
        applied = colt_movement_apply(*(uint64_t *)(event + 0xc),
                                      *(uint64_t *)(event + 0xe));
    else
        applied = 0;

    if (g_fire_state != 0) {
        if (applied != 0)
            input_event_passthrough(event);
        return;
    }

    input_lock_acquire();
    g_smartaim_event_count = g_smartaim_event_count + 1;
    input_event_refresh(event);

    memset(proposal, 0, sizeof proposal);
    proposal[0] = ROD_PROPOSAL_VT;

    if (g_smartaim_active == 1
        && *(uint64_t *)(event + 4) == g_engine_epoch
        && *(uint64_t *)(event + 8) == g_engine_base
        && movement_ctx_valid(*(uint64_t *)(event + 0xc),
                              *(uint64_t *)(event + 0xe))) {
        uintptr_t h = movement_ctx_valid(*(uint64_t *)(event + 0xc),
                                         *(uint64_t *)(event + 0xe));

        if (game_read(h, *(uint64_t *)(event + 0xc) + 0xfac, &untouched, 8) != 0)
            untouched_known = 1;

        if (event_precheck(0, event) == 0) {
            memset(config, 0, sizeof config);
            config[0] = ROD_CONFIG_VT;

            if (((int (*)(void *))g_config_query_fn)(config) == 1
                && (uint32_t)config[7] != 0
                && (config[6] & 1) != 0
                && config[5] == g_colt_epoch) {
                int ultimate_ok = (uint32_t)(config[3] >> 32) != 0;
                int gadget_ok = (int32_t)config[2] != 0;
                int proceed;

                if (ultimate_ok || gadget_ok) {
                    proceed = 1;
                } else {
                    ability[0] = 0;
                    if (ability_input_check(*(uint64_t *)(event + 0xc),
                                            *(uint64_t *)(event + 0xe),
                                            NULL, ability) == 0) {
                        reason = "ability_input_unknown";
                        proceed = 0;
                    } else {
                        const char *sub;

                        sub = "gadget_smartaim_disabled";
                        if (ability[0] != 3)
                            sub = "ability_allowed";
                        reason = "ultimate_smartaim_disabled";
                        if (ability[0] != 2)
                            reason = sub;

                        if (ability[0] == 3)
                            proceed = (int32_t)config[2] != 0;
                        else if (ability[0] == 2)
                            proceed = (uint32_t)(config[3] >> 32) != 0;
                        else
                            proceed = 1;
                    }
                }

                if (proceed) {
                    if (game_object_resolve(g_engine_base,
                                            *(uint64_t *)(event + 0xe),
                                            (void *)game_read, 0,
                                            &CTX64(0x20f680)) == 0
                        || actor_refresh(g_colt_ctx,
                                         *(uint64_t *)(event + 0xe)) == 0) {
                        reason = "current_actor_refresh";
                    } else {
                        memset(lease, 0, sizeof lease);
                        lease[0] = ROD_LEASE_VT;

                        if (((int (*)(uint32_t, uint32_t, void *))g_input_lease_acquire)(0x17, 1, lease) == 0) {
                            reason = "input_lease_unavailable";
                        } else if (!untouched_known) {
                            reason = "untouched_xy_unknown";
                        } else {
                            void *lease_slot[5];

                            memset(request, 0, sizeof request);
                            request[0] = *(uint64_t *)(event + 0xc);
                            request[1] = *(uint64_t *)(event + 0xe);
                            *(uint32_t *)(request + 2) = 1;
                            request[3] = ROD_SKILL_REQ_VT;
                            request[4] = *(uint64_t *)(event + 6);
                            request[5] = g_colt_ctx;
                            *(uint32_t *)(request + 6) = (uint32_t)event[2];
                            request[7] = CTX64(0x20f6f8);
                            *(uint32_t *)(request + 8) = CTX32(0x20f690);
                            request[9] = engine_frame_time(1);
                            request[10] = 0;
                            request[11] = untouched;
                            request[13] = CTX64(0x20f694);
                            *(uint32_t *)(request + 14) = (uint32_t)(config[7] >> 32);
                            *(uint32_t *)(request + 15) = 1;
                            request[16] = 0;
                            request[17] = ROD_REQUEST_VT;
                            request[18] = 0;
                            request[19] = 1;
                            request[12] = 1;
                            request[20] = config[5];

                            lease_slot[0] = (void *)(uintptr_t)0x20d168;
                            lease_slot[1] = (void *)(uintptr_t)0x20f628;
                            lease_slot[2] = proposal;
                            lease_slot[3] = (void *)fetch_read_cb;
                            lease_slot[4] = NULL;

                            committed = aim_proposal_solve(g_proposal_engine_state,
                                                           &config[5],
                                                           request,
                                                           lease_slot,
                                                           proposal + 2);

                            reason = "proposal_refused";
                            if (committed == 1
                                && *(uint32_t *)((char *)proposal + 0x0c + 0x10) == 1
                                && ((uint64_t *)proposal)[3] == *(uint64_t *)(event + 0xc)
                                && ((uint64_t *)proposal)[5] == *(uint64_t *)(event + 0xe)) {

                                proposed_lo = *(uint32_t *)((char *)proposal + 0x5c);
                                proposed_hi = *(uint32_t *)((char *)proposal + 0x58);

                                if (xy_matches_sentinel(proposed_lo, proposed_hi)
                                    && ((uint64_t *)proposal)[13] == lease[2]
                                    && movement_ctx_valid(*(uint64_t *)(event + 0xc),
                                                          *(uint64_t *)(event + 0xe))
                                           != 0
                                    && ((int (*)(void *))g_input_lease_commit)(lease) != 0) {
                                    struct {
                                        uint64_t *payload;
                                        void *read;
                                        int (*apply)(void *);
                                        int (*verify)(void *);
                                        uint64_t slots[8];
                                    } record;
                                    uint64_t payload[4];
                                    int result;

                                    payload[0] = *(uint64_t *)(event + 0xc);
                                    payload[1] = *(uint64_t *)(event + 0xe);
                                    payload[2] = (uint64_t)proposed_lo
                                               | ((uint64_t)proposed_hi << 32);

                                    record.payload = payload;
                                    record.read = (void *)game_read;
                                    record.apply = smartaim_xy_apply;
                                    record.verify = smartaim_xy_verify;
                                    memset(record.slots, 0, sizeof record.slots);

                                    result = remote_guarded_apply(&record,
                                                                  payload[0],
                                                                  &untouched,
                                                                  &payload[2]);
                                    if (result == 1) {
                                        committed = 1;
                                        commit_status = 1;
                                        g_smartaim_xy_writes = g_smartaim_xy_writes + 1;
                                        reason = "natural_event_xy_committed";
                                    } else if (result == -1) {
                                        log_event("fatal",
                                                  "input_xy_restore_unverified",
                                                  NULL);
                                        __android_log_write(6, "NexusLab69252",
                                                            "input_xy_restore_unverified");
                                        abort();
                                    } else {
                                        committed = 0;
                                        commit_status = result;
                                        if (result == 3)
                                            reason = "untouched_xy_changed";
                                        else if (result == 2)
                                            reason = "xy_restored_after_failed_write";
                                        else
                                            reason = "xy_write_refused";
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        reason = "identity_or_active";
    }

    now = monotonic_ms();
    if (g_smartaim_event_count <= 0x20
        || (g_smartaim_xy_writes < 9 && committed != 0)
        || g_smartaim_last_log_ms == 0
        || now - g_smartaim_last_log_ms >= 1000) {
        char json[0x6a4];
        uint32_t committed_lo = proposed_lo;
        uint32_t committed_hi = proposed_hi;

        if (committed == 0) {
            committed_lo = (uint32_t)untouched;
            committed_hi = (uint32_t)(untouched >> 32);
        }

        snprintf(json, sizeof json,
                 ",\"action\":23,\"kind\":%u,\"delivery\":%llu,\"event_count\":%llu,"
                 "\"publication\":%llu,\"source_tick\":%u,\"clock_known\":%u,"
                 "\"map_ready\":%u,\"config_supported\":%u,\"active\":%u,"
                 "\"evidence\":%u,\"generation\":%llu,\"plan_reason\":%u,"
                 "\"selection_reason\":%u,\"target_gid\":%u,"
                 "\"untouched_xy_known\":%d,\"untouched_raw_x\":%d,"
                 "\"untouched_raw_y\":%d,\"proposed_raw_x\":%d,\"proposed_raw_y\":%d,"
                 "\"committed_xy_known\":%d,\"committed_raw_x\":%d,"
                 "\"committed_raw_y\":%d,\"commit_status\":%d,\"xy_written\":%d,"
                 "\"xy_writes\":%llu,\"own_gid\":%u,\"own_x\":%.6f,\"own_y\":%.6f,"
                 "\"ended\":%u,\"extra_fire_calls\":0",
                 (unsigned)event[2],
                 (unsigned long long)*(uint64_t *)(event + 6),
                 (unsigned long long)g_smartaim_event_count,
                 (unsigned long long)engine_frame_time(1),
                 (unsigned)CTX32(0x20f650),
                 (unsigned)CTX32(0x20f63c),
                 (unsigned)CTX32(0x20f638),
                 (unsigned)g_evasion_gate,
                 (unsigned)((uint32_t)g_proposal_state & 1u),
                 (unsigned)CTX32(0x20f7c4),
                 (unsigned long long)g_map_epoch,
                 (unsigned)(uint32_t)g_proposal_state,
                 (unsigned)CTX32(0x20f7c0),
                 (unsigned)(uint32_t)proposal[1],
                 (unsigned)untouched_known,
                 (int)(uint32_t)untouched,
                 (int)(uint32_t)(untouched >> 32),
                 (int)proposed_hi,
                 (int)proposed_lo,
                 (unsigned)(commit_status - 1U < 2),
                 (int)committed_hi,
                 (int)committed_lo,
                 commit_status,
                 committed,
                 (unsigned long long)g_smartaim_xy_writes,
                 (unsigned)CTX32(0x20f690),
                 CTXFLT(0x20f6b4),
                 CTXFLT(0x20f6b8),
                 (unsigned)CTX32(0x20f7e8));
        log_event("smartaim_input", reason, json);
        g_smartaim_last_log_ms = now;
    }

    input_lock_release();
    input_event_passthrough(event);
}

int smartaim_skill_hook(void *ctx, int32_t *event, uint64_t *out)
{
    (void)ctx;
    const char *reason;
    uint64_t config[32];
    uint64_t lease[14];
    uint64_t request[24];
    uint64_t proposal[20];
    uint64_t skill_ctx[8];
    int32_t ability;
    uintptr_t caller;

    if (out == NULL)
        return 0;
    memset(out, 0, 8 * sizeof(uint64_t));

    if (event == NULL || event[0] != 2 || event[1] != 0x98)
        return 0;
    if (event[2] != 4 || g_smartaim_active == 0)
        return 0;
    if (*(uint64_t *)(event + 4) != g_engine_epoch
        || *(uint64_t *)(event + 8) != g_engine_base
        || skill_precheck(event, out) != 1)
        return 0;

    caller = *(uint64_t *)(event + 0x12);

    {
        int route_a = 0;
        int route_b = 0;

        if (caller == g_engine_base + ENGINE_CALLER_A_OFF
            && *(uint64_t *)(event + 0x1e) == 0
            && *(uint64_t *)(event + 0x20) == 0)
            route_a = 1;

        if (caller != g_engine_base + ENGINE_CALLER_B_OFF
            && caller != g_engine_base + ENGINE_CALLER_C_OFF
            && *(uint64_t *)(event + 0x1e) == 1
            && event[0x21] == 0)
            route_b = 1;

        if (route_a || route_b) {
            if (skill_precheck(event, out) != 0)
                return 1;
        } else {
            return 0;
        }
    }

    input_lock_acquire();
    input_event_refresh(event);
    g_skill_event_count = g_skill_event_count + 1;
    memset(g_skill_blob, 0, 0x280);

    memset(proposal, 0, sizeof proposal);
    proposal[0] = ROD_PROPOSAL_VT;

    reason = "skill_identity_or_active";

    if (movement_ctx_valid(*(uint64_t *)(event + 0xc),
                           *(uint64_t *)(event + 0xe)) != 0) {
        memset(config, 0, sizeof config);
        config[0] = ROD_CONFIG_VT;

        reason = "skill_configuration_or_generation";
        if (((int (*)(void *))g_config_query_fn)(config) == 1
            && (uint32_t)config[8] != 0
            && (config[6] & 1) != 0
            && config[5] == g_colt_epoch) {

            if (game_object_resolve(g_engine_base, *(uint64_t *)(event + 0xe),
                                    (void *)game_read, 0,
                                    &CTX64(0x20f680)) == 0
                || actor_refresh(g_colt_ctx, *(uint64_t *)(event + 0xe)) == 0) {
                reason = "skill_current_actor_refresh";
            } else if (skill_context_resolve(g_engine_base,
                                             *(uint64_t *)(event + 0xe),
                                             *(uint64_t *)(event + 0x24),
                                             (void *)game_read, 0,
                                             skill_ctx) == 0) {
                reason = "skill_context_not_current_member";
            } else {
                ability = 0;
                if (ability_input_check(*(uint64_t *)(event + 0xc),
                                        *(uint64_t *)(event + 0xe),
                                        skill_ctx, &ability) == 0) {
                    reason = "skill_ability_input_unknown";
                } else if (ability == 2
                           && *(uint64_t *)(event + 0x1e) == 1
                           && event[0x21] == 0) {
                    reason = "skill_ultimate_route_not_live";
                } else {
                    const char *sub;

                    sub = "skill_gadget_smartaim_disabled";
                    if (ability != 3)
                        sub = "skill_ability_allowed";
                    reason = "skill_ultimate_smartaim_disabled";
                    if (ability != 2)
                        reason = sub;

                    if ((ability == 3 && (int32_t)config[2] != 0)
                        || (ability == 2 && (uint32_t)(config[3] >> 32) != 0)
                        || (ability != 2 && ability != 3)) {
                        memset(lease, 0, sizeof lease);
                        lease[0] = ROD_LEASE_VT;

                        if (((int (*)(uint32_t, uint32_t, void *))g_input_lease_acquire)(0x17, 4, lease) == 0) {
                            reason = "register_input_lease_unavailable";
                        } else {
                            void *lease_slot[5];

                            memset(request, 0, sizeof request);
                            request[0] = *(uint64_t *)(event + 0xc);
                            request[1] = *(uint64_t *)(event + 0xe);
                            request[4] = g_colt_ctx;
                            request[5] = *(uint64_t *)(event + 6);
                            request[6] = CTX64(0x20f6f8);
                            request[8] = ROD_SKILL_REQ2_A;
                            request[9] = ROD_SKILL_REQ2_B;
                            *(uint32_t *)(request + 10) = CTX32(0x20f690);
                            *(uint32_t *)(request + 11) = CTX32(0x20f638);
                            request[12] = engine_frame_time(1);
                            *(uint32_t *)(request + 13) = (uint32_t)(config[8] >> 32);
                            request[14] = ROD_REQUEST_VT;
                            *(uint32_t *)(request + 15) = 1;
                            *(uint32_t *)(request + 16) = CTX32(0x20f694);
                            request[17] = (*(uint64_t *)(event + 6) & 0xffffffffull)
                                        | 0x100000000ull;
                            *(uint32_t *)(request + 18) = 1;
                            request[2] = 0;
                            request[3] = config[5];
                            request[7] = 0;
                            request[19] = 0;
                            request[20] = 0;
                            request[21] = 0;
                            request[22] = 0;
                            request[23] = 0;

                            lease_slot[0] = (void *)(uintptr_t)0x20d168;
                            lease_slot[1] = (void *)(uintptr_t)0x20f628;
                            lease_slot[2] = proposal;
                            lease_slot[3] = (void *)fetch_read_cb;
                            lease_slot[4] = NULL;

                            if (aim_proposal_solve(g_proposal_engine_state,
                                                   &config[5],
                                                   request,
                                                   lease_slot,
                                                   proposal + 2) == 1
                                && *(uint32_t *)((char *)proposal + 0x18) == 2
                                && ((uint64_t *)proposal)[4] == *(uint64_t *)(event + 0xc)
                                && ((uint64_t *)proposal)[5] == *(uint64_t *)(event + 0xe)
                                && xy_matches_sentinel(*(uint32_t *)((char *)proposal + 0x44),
                                                       *(uint32_t *)((char *)proposal + 0x40))
                                && ((uint64_t *)proposal)[13] == lease[2]
                                && movement_ctx_valid(*(uint64_t *)(event + 0xc),
                                                      *(uint64_t *)(event + 0xe)) != 0
                                && ((int (*)(void *))g_input_lease_commit)(lease) != 0) {

                                out[3] = *(uint64_t *)(event + 6);
                                out[2] = *(uint64_t *)(event + 4);
                                out[5] = *(uint64_t *)(event + 0x18);
                                out[4] = *(uint64_t *)(event + 0x16);
                                out[6] = lease[2];
                                out[7] = ((uint64_t *)proposal)[8];
                                out[0] = ROD_SKILL_OUT_A;
                                out[1] = ROD_SKILL_OUT_B;

                                SKB(0x2157a8) = skill_ctx[3];
                                SKB(0x215678) = 1;

                                for (int i = 0; i < 0x25; i++)
                                    SKB(0x215680 + i * 8) =
                                        *(uint64_t *)((char *)event + i * 8);
                                SKB(0x215680 + 0x25 * 8) = *(uint64_t *)(event + 0x24);

                                SKB(0x215740) = out[5];
                                SKB(0x215738) = out[4];
                                SKB(0x215748) = out[6];
                                SKB(0x215750) = out[7];
                                SKB(0x215718) = out[0];
                                SKB(0x215720) = out[1];
                                SKB(0x215730) = out[3];
                                SKB(0x215728) = out[2];
                                SKB(0x215758) = lease[0];
                                SKB(0x2157b0) = skill_ctx[0];
                                SKB(0x2157b8) = proposal[4];
                                SKB(0x2157c0) = proposal[2];

                                input_lock_release();
                                return 1;
                            }
                            reason = "register_proposal_refused";
                        }
                    }
                }
            }
        }
    }

    smartaim_skill_log(event, proposal, lease, config, reason, 0);
    input_lock_release();
    return 0;
}

void smartaim_skill_log(int32_t *event, void *proposal, void *lease,
                        void *config, const char *reason, int mode)
{
    uint64_t now = monotonic_ms();

    if (g_skill_event_count < 0x21
        || (g_skill_commits < 9 && mode != 0)
        || g_skill_last_log_ms == 0
        || now - g_skill_last_log_ms > 999) {
        char json[0x708];
        uint32_t committed_lo = *(uint32_t *)((char *)event + 0x60);
        uint32_t committed_hi = *(uint32_t *)((char *)event + 0x58);

        if (mode != 0) {
            committed_lo = *(uint32_t *)((char *)lease + 0x58);
            committed_hi = *(uint32_t *)((char *)lease + 0x54);
        }

        snprintf(json, sizeof json,
                 ",\"action\":23,\"kind\":4,\"destination\":2,\"operation\":4,"
                 "\"delivery\":%llu,\"event_count\":%llu,\"caller_lr_rva\":\"0x%llx\","
                 "\"publication\":%llu,\"source_tick\":%u,\"clock_known\":%u,"
                 "\"map_ready\":%u,\"config_supported\":%u,\"active\":%u,"
                 "\"generation\":%llu,\"target_gid\":%u,\"plan_reason\":%d,"
                 "\"selection_reason\":%u,\"untouched_xy_known\":1,"
                 "\"untouched_raw_x\":%d,\"untouched_raw_y\":%d,"
                 "\"proposed_raw_x\":%d,\"proposed_raw_y\":%d,"
                 "\"committed_xy_known\":1,\"committed_raw_x\":%d,"
                 "\"committed_raw_y\":%d,\"register_xy_committed\":%d,"
                 "\"register_xy_commits\":%llu,\"own_gid\":%u,\"own_x\":%.6f,"
                 "\"own_y\":%.6f,\"ended\":%u,\"extra_fire_calls\":0,"
                 "\"screen_xy_writes\":0",
                 (unsigned long long)*(uint64_t *)(event + 0x18),
                 (unsigned long long)g_skill_event_count,
                 (unsigned long long)(*(uint64_t *)((char *)event + 0x48)
                                      - *(uint64_t *)((char *)event + 0x20)),
                 (unsigned long long)*(uint64_t *)(event + 6),
                 (unsigned)CTX32(0x20f650),
                 (unsigned)CTX32(0x20f63c),
                 (unsigned)CTX32(0x20f638),
                 (unsigned)g_evasion_gate,
                 (unsigned)(uint32_t)(*(uint64_t *)((char *)config + 0x28) & 1),
                 (unsigned long long)*(uint64_t *)((char *)config + 8),
                 (unsigned)*(uint32_t *)((char *)lease + 0x50),
                 (unsigned)*(uint32_t *)((char *)lease + 8),
                 (unsigned)*(uint32_t *)((char *)proposal + 8),
                 (unsigned)*(uint32_t *)((char *)event + 0x58),
                 (unsigned)*(uint32_t *)((char *)event + 0x60),
                 (unsigned)*(uint32_t *)((char *)lease + 0x54),
                 (unsigned)*(uint32_t *)((char *)lease + 0x58),
                 (int)committed_hi,
                 (int)committed_lo,
                 mode,
                 (unsigned long long)g_skill_commits,
                 (unsigned)CTX32(0x20f690),
                 CTXFLT(0x20f6b4),
                 CTXFLT(0x20f6b8),
                 (unsigned)CTX32(0x20f7e8));
        log_event("smartaim_input", reason, json);
        g_skill_last_log_ms = now;
    }
}

void smartaim_frame_update(void *frame)
{
    uint64_t region = 0;
    uint64_t obj = 0;
    int64_t ctrl = 0;
    int map_ok = 0;
    uint64_t resolve[16];
    uint64_t chain = 0;
    uint64_t list = 0;
    uint64_t ctrl_list = 0;
    int32_t type = 0;
    uint64_t *blob;
    uintptr_t h;

    if (g_smartaim_armed != 1)
        return;

    CTX64(0x20f7e8) = 0;
    g_proposal_state = 0;
    CTX64(0x20f7c0) = 0;
    CTX64(0x20f7d8) = 0;
    memset(g_ctx_block, 0, 0x1d0);

    memset(resolve, 0, sizeof resolve);

    if (frame == 0 || (blob = *(uint64_t **)((char *)frame + 8), blob == NULL)) {
        ctrl = 0;
        region = 0;
        map_ok = 0;
        g_route_active = 0;
    } else {
        region = *(uint64_t *)(blob + 0x10);
        if (region + 0x2000u < 0x12000u || (region & 7) != 0) {
            ctrl = 0;
            region = 0;
            map_ok = 0;
            g_route_active = 0;
        } else {
            obj = *(uint64_t *)(blob + 0x18);
            h = game_object_resolve(g_engine_base, obj, (void *)game_read, 0, resolve);

            if (h == 0
                || (uint32_t)(resolve[3] >> 32) == 0
                || (fabsf(*(float *)((char *)resolve + 0x10)) < 0.5f
                    && fabsf(*(float *)((char *)resolve + 0x18)) < 0.5f)
                || game_read(h, g_engine_base + ENGINE_STATIC_CHAIN_OFF, &chain, 8) == 0
                || chain + 0x2000u < 0x12000u
                || (chain & 7) != 0
                || game_read(h, chain + 0x50, &type, 4) == 0
                || type != 5
                || game_read(h, chain + 0x48, &list, 8) == 0
                || list + 0x2000u < 0x12000u
                || (list & 7) != 0
                || game_read(h, region + 0x918, &ctrl_list, 8) == 0
                || (ctrl_list - 0x10000u > 0xfffffffffffedfffu)
                || (ctrl_list & 7) != 0
                || ctrl_list != list
                || page_perm_check(ctrl_list + 0x28, &ctrl) == 0) {
                ctrl = 0;
                region = 0;
                map_ok = 0;
                g_route_active = 0;
            } else {
                map_ok = 1;
            }
        }
    }

    if (g_map_generation == (uint64_t)-1)
        return;
    g_map_generation = g_map_generation + 1;
    map_publish((void *)(uintptr_t)0x213060, (uint32_t)g_map_generation, region,
                ctrl, map_ok, resolve);

    CTX64(0x20f668) = (frame == 0) ? 0 : *(uint64_t *)((char *)frame + 0x28);
    CTX64(0x20f6d0) = CTX64(0x20f6d0) & 0xffffffff00000000ull;
    CTX64(0x20f6c0) = 0;
    CTX64(0x20f6c8) = 0;
    CTX64(0x20f688) = resolve[5];
    CTX64(0x20f680) = resolve[2];
    CTX64(0x20f698) = resolve[3];
    CTX64(0x20f690) = resolve[4];
    CTX64(0x20f6a8) = resolve[3];
    CTX64(0x20f6a0) = resolve[2];
    CTX64(0x20f6b8) = resolve[3];
    CTX64(0x20f6b0) = resolve[2];
    CTX64(0x20f628) = ROD_MAP_VT;
    CTX64(0x20f638) = resolve[7];
    CTX64(0x20f630) = resolve[0];
    CTX64(0x20f648) = resolve[6];
    CTX64(0x20f640) = resolve[5];
    CTX64(0x20f658) = resolve[4];
    CTX64(0x20f650) = resolve[3];
    CTX64(0x20f660) = resolve[1];
    CTX64(0x20f678) = (uint64_t)ctrl;
    CTX64(0x20f670) = region;

    if (map_ok == 0
        || *(int32_t *)((char *)frame + 0x24) == 0
        || blob == NULL
        || *(int32_t *)(blob + 0xd) == 0
        || resolve[7] == 0
        || blob[5] != region
        || blob[7] != (uint64_t)ctrl
        || blob[8] != obj
        || *(int32_t *)(blob + 0xe) != (int32_t)resolve[4]) {
        g_ctx_ready = 0;
        return;
    }

    if (CTX64(0x20f6f8) != blob[4] || CTX64(0x20f700) != region
        || CTX64(0x20f710) != (uint64_t)ctrl) {
        g_route_active = 0;
        g_evasion_gate = 0;
        g_map_fail_a = 0;
        g_map_fail_b = 0;
        g_map_active = 0;
        g_route_armed = 0;
        map_clear();
    }

    CTX64(0x20f6e0) = blob[1];
    CTX64(0x20f6d8) = blob[0];
    CTX64(0x20f710) = blob[7];
    CTX64(0x20f708) = blob[6];
    CTX64(0x20f720) = blob[9];
    CTX64(0x20f718) = blob[8];
    CTX64(0x20f6f0) = blob[3];
    CTX64(0x20f6e8) = blob[2];
    CTX64(0x20f700) = blob[5];
    CTX64(0x20f6f8) = blob[4];
    CTX64(0x20f750) = blob[0xf];
    CTX64(0x20f748) = blob[0xe];
    CTX64(0x20f760) = blob[0x11];
    CTX64(0x20f758) = blob[0x10];
    CTX64(0x20f730) = blob[0xb];
    CTX64(0x20f728) = blob[0xa];
    CTX64(0x20f740) = blob[0xd];
    CTX64(0x20f738) = blob[0xc];
    CTX64(0x20f790) = blob[0x17];
    CTX64(0x20f788) = blob[0x16];
    CTX64(0x20f7a0) = blob[0x19];
    CTX64(0x20f798) = blob[0x18];
    CTX64(0x20f770) = blob[0x13];
    CTX64(0x20f768) = blob[0x12];
    CTX64(0x20f780) = blob[0x15];
    CTX64(0x20f778) = blob[0x14];

    {
        uint64_t config2[21];
        int install_route = 0;
        uint32_t flags = 0x1a;

        memset(config2, 0, sizeof config2);
        config2[0] = ROD_CONFIG_VT;

        if (g_config_query_fn == NULL
            || ((int (*)(void *))g_config_query_fn)(config2) != 1
            || actor_refresh((uint64_t)ctrl, obj) == 0)
            return;

        CTX64(0x20f6c8) = (CTX64(0x20f6c8) & 0xffffffffull) | 0x100000000ull;

        {
            int valid = movement_ctx_valid(region, obj);

            if (valid == 0) {
                if (g_route_armed == 1) {
                    g_route_active = 0;
                    g_route_armed = 0;
                }
            } else {
                g_route_armed = 1;
                g_route_active = 1;
            }
        }

        CTX64(0x20f6d0) = ((CTX64(0x20f6d0) & 0xffffffff00000000ull)
                           | (uint32_t)g_route_active)
                        ^ 0xffffffffull;
        CTX64(0x20f6d0) = CTX64(0x20f6d0) & 0xffffffff00000001ull;

        if (config2[6] == 0) {
            if (config2[11] != 0)
                install_route = 1;
        } else if ((config2[5] & 1) != 0 || config2[11] != 0) {
            install_route = 1;
        }

        if (!install_route) {
            if (route_install_check() != 0 || config2[14] != 0
                || config2[13] != 0 || config2[15] != 0)
                install_route = 1;
            else
                g_ctx_ready = 0;
        }

        if (install_route) {
            aim_feed_route_install(CTX64(0x20f678), CTX64(0x20f728),
                                   CTX32(0x20f63c), CTX32(0x20f6cc));
            if (route_install((void *)(uintptr_t)0x20d168,
                              (void *)(uintptr_t)0x20f628,
                              (void *)(uintptr_t)0x10e7f8, 0) != 0)
                g_route_active = 1;
        }

        if (g_main_tid != 0 && gettid() == g_main_tid)
            flags = 0x1b;
        if (movement_ctx_valid(region, obj) != 0)
            flags |= 4;
        if (g_ctx_ready != 0 && g_evasion_gate != 0 && g_smartaim_armed != 0)
            flags |= 0x20;

        CTX64(0x20f7c0) = ((uint64_t)(uint32_t)resolve[3] << 32) | flags;
        CTX64(0x20f7c8) = ROD_COUNTERS_VT;

        if (config2[6] != 0
            && (g_map_last_log_ms == 0
                || *(uint64_t *)((char *)frame + 0x28) - g_map_last_log_ms > 999)) {
            uint64_t counters[18];

            memset(counters, 0, sizeof counters);
            counters[0] = ROD_ROUTE_VT;
            counters_snapshot(g_counters_ctx, counters);

            {
                char json[0x6a4];
                snprintf(json, sizeof json,
                         ",\"action\":23,\"publication\":%llu,\"source_tick\":%u,"
                         "\"source_known\":%u,\"actors\":%u,\"map_known\":%u,"
                         "\"evidence\":%u,\"generation\":%llu,\"xy_writes\":%llu,"
                         "\"manual_events\":%llu,\"wrapper_events\":%llu,"
                         "\"wrong_thread_events\":%llu,\"bad_receiver_events\":%llu,"
                         "\"route_status_known\":%d,\"route_phase\":%u,"
                         "\"route_disabled\":%llu,\"route_nested\":%llu,"
                         "\"skill_events\":%llu,\"skill_proposed\":%llu,"
                         "\"skill_committed\":%llu,\"skill_refused\":%llu,"
                         "\"wrapper_continuation\":%llu,\"unsupported_caller\":%llu,"
                         "\"unsupported_flags\":%llu,\"folded\":%llu,"
                         "\"unknown_origin\":%llu,\"map_refresh_us\":%llu,"
                         "\"map_failure_stage\":%u,\"map_failure_index\":%u",
                         (unsigned long long)resolve[3],
                         (unsigned)(uint32_t)(resolve[7] >> 32),
                         (unsigned)(uint32_t)resolve[7],
                         (unsigned)(uint32_t)CTX64(0x20f6c8),
                         (unsigned)g_evasion_gate,
                         (unsigned)flags,
                         (unsigned long long)counters[9],
                         (unsigned long long)g_smartaim_xy_writes,
                         (unsigned long long)counters[2],
                         (unsigned long long)counters[4],
                         (unsigned long long)counters[3],
                         (unsigned long long)counters[5],
                         (unsigned)(uint32_t)counters[1],
                         (unsigned)(uint32_t)counters[0],
                         (unsigned long long)counters[6],
                         (unsigned long long)counters[7],
                         (unsigned long long)counters[8],
                         (unsigned long long)counters[10],
                         (unsigned long long)counters[11],
                         (unsigned long long)counters[12],
                         (unsigned long long)counters[13],
                         (unsigned long long)counters[14],
                         (unsigned long long)counters[15],
                         (unsigned long long)counters[16],
                         (unsigned long long)counters[17],
                         (unsigned long long)g_map_fail_a,
                         (unsigned)(uint32_t)g_map_fail_b,
                         (unsigned)(uint32_t)g_route_gen);
                log_event("function_frame", "standalone_aim_feed", json);
            }
            g_map_last_log_ms = *(uint64_t *)((char *)frame + 0x28);
        }
    }
}
