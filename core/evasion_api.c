#define _GNU_SOURCE 1

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <dlfcn.h>

#define EV_LIVE_KEYS 142
#define EV_FEATURE_KEYS 69
#define EV_STATE_COUNT 134

#define EV_BUILD_ID "a10aeb6b4085fb2a15d969a130cd9608231998b5269a40aed41129d99624ede3"
#define EV_IMAGE_ID "fdf834103d333f9f8a1947b3a405b32da6ebb651"
#define EV_NOT_FOUND 0x80000000u

#define EV_GAME_ANCHOR_OFF 0xb337c4u
#define EV_GAME_ISLAND_REF_OFF 0xaa2830u
#define EV_REGION1_OFF 0xb2e674u
#define EV_REGION1_REF 0x104f4du
#define EV_REGION2_OFF 0xb2e994u
#define EV_REGION2_REF 0x104dfdu
#define EV_REGION3_OFF 0xb2ea94u
#define EV_REGION3_REF 0x104efdu
#define EV_REGION4_OFF 0xb2e780u
#define EV_REGION4_REF 0x104be9u
#define EV_REGION5_OFF 0xb2e880u
#define EV_REGION5_REF 0x104ce9u
#define EV_MAGIC1_OFF 0xb2e774u
#define EV_MAGIC2_OFF 0xb2e980u
#define EV_ISLAND_TEMPLATE 0x104a91u
#define EV_ISLAND_TEMPLATE_LEN 0x150u

extern int  evasion_state_lazy_init(void);
extern int  evasion_key_gated(const char *name);
extern int  evasion_dependencies_hold(const char *name);
extern int  evasion_contract_validate(const void *contract, void *binding_out);
extern uint32_t evasion_stamp_source(void);
extern int  evasion_token_fetch(void *token_out);
extern int  evasion_stamp_compose(uint32_t source, uint32_t token, void *stamp_out, int flags);
extern int  evasion_lease_compose(uint32_t slot_a, uint32_t slot_b, void *out);
extern int  evasion_route_check_a(void *ctx, uint32_t anchor_insn);
extern int  evasion_route_check_b(void *ctx, uint32_t anchor_insn);
extern int  evasion_spin_route_validate(void *ctx, const void *post_b, const void *post_a,
                                        const void *routes);
extern int  evasion_once_init(void);
extern void log_event(const char *state, const char *detail, const char *extra);
extern int  hook_slot_unpatch(void *slot);
extern int  hook_slot_commit(void *slot);

extern int  evasion_route_check_ok(void *routes);

typedef struct {
    const char *name;
    uint32_t state_index;
    int32_t vmin;
    int32_t vmax;
} ev_key_t;

typedef struct {
    uint64_t offset;
    uint32_t len;
    uint8_t bytes[24];
} ev_anchor_t;

static const ev_key_t ev_keys[EV_LIVE_KEYS] = {
    { "isSpinEnabled", 0, 0, 1 },
    { "cameraEnabled", 1, 0, 1 },
    { "antiAfkEnabled", 2, 0, 1 },
    { "autofarmEnabled", 3, 0, 1 },
    { "autofarmAttackEnemies", 4, 0, 1 },
    { "coltModEnabled", 5, 0, 1 },
    { "koltModEnabled", 5, 0, 1 },
    { "boltWallAvoidEnabled", 6, 0, 1 },
    { "boltPredictionEnabled", 7, 0, 1 },
    { "pinEnabled", 8, 0, 1 },
    { "sprayEnabled", 9, 0, 1 },
    { "isXrayEnabled", 10, 0, 1 },
    { "xRayEnabled", 10, 0, 1 },
    { "xrayEnabled", 10, 0, 1 },
    { "xrayShowTargetName", 11, 0, 1 },
    { "aopAimEnabled", 12, 0, 1 },
    { "aopPredictEnabled", 13, 0, 1 },
    { "aopDebugEnabled", 14, 0, 1 },
    { "espEnabled", 15, 0, 1 },
    { "attackRangeIndicator", 16, 0, 1 },
    { "hitboxRenderer", 17, 0, 1 },
    { "enemyTracer", 18, 0, 1 },
    { "teammateHpIndicator", 19, 0, 1 },
    { "trophiesAboveHead", 20, 0, 1 },
    { "allyRespawnTimer", 21, 0, 1 },
    { "tileGridOverlay", 22, 0, 1 },
    { "smoothHudGraph", 23, 0, 1 },
    { "smoothHud", 24, 0, 1 },
    { "serverIPEnabled", 25, 0, 1 },
    { "characterOutlineEnabled", 26, 0, 1 },
    { "killauraEnabled", 27, 0, 1 },
    { "killauraMainAttack", 28, 0, 1 },
    { "killauraGadget", 29, 0, 1 },
    { "killauraSuper", 30, 0, 1 },
    { "killauraNoWall", 31, 0, 1 },
    { "killauraNoBall", 32, 0, 1 },
    { "ballAssistEnabled", 33, 0, 1 },
    { "holdToShootEnabled", 34, 0, 1 },
    { "holdToShootAim", 35, 0, 1 },
    { "holdToShootRangeCheck", 36, 0, 1 },
    { "combatDodgeRequireHold", 37, 0, 1 },
    { "followEnabled", 38, 0, 1 },
    { "followAntiAfkEnabled", 38, 0, 1 },
    { "followClosestAllyEnabled", 38, 0, 1 },
    { "followModeEnemy", 39, 0, 1 },
    { "followModeTeam", 39, 0, 1 },
    { "speedExploitEnabled", 40, 0, 1 },
    { "speedLocalMoveEnabled", 41, 0, 1 },
    { "flyEnabled", 42, 0, 1 },
    { "kitNaniModEnabled", 43, 0, 1 },
    { "boltModEnabled", 44, 0, 1 },
    { "boltAutoAttackEnabled", 45, 0, 1 },
    { "boltSafeExitEnabled", 46, 0, 1 },
    { "autododgeEnabled", 47, 0, 1 },
    { "dynaJumpEnabled", 48, 0, 1 },
    { "epsteinEnabled", 49, 0, 1 },
    { "espShowNames", 50, 0, 1 },
    { "espShowDistance", 51, 0, 1 },
    { "espShowTracer", 52, 0, 1 },
    { "espShowHitbox", 53, 0, 1 },
    { "espShowCircle", 54, 0, 1 },
    { "espShowAimLine", 55, 0, 1 },
    { "espTeamFilterEnemies", 56, 0, 1 },
    { "espTeamFilterSelf", 57, 0, 1 },
    { "espTeamFilterTeammates", 58, 0, 1 },
    { "combatDodgeContinuous", 59, 0, 1 },
    { "combatDodgeCircular", 60, 0, 1 },
    { "combatDodgeAlways", 61, 0, 1 },
    { "combatDodgeQuickAccess", 62, 0, 1 },
    { "spinSpeed", 63, 0, 62832 },
    { "spinRadius", 64, 0, 300 },
    { "cameraMode", 65, 0, 8 },
    { "cameraZoom", 66, 25, 400 },
    { "cameraOffsetX", 67, 4294965296, 2000 },
    { "cameraOffsetY", 68, 4294965296, 2000 },
    { "followDistance", 69, 100, 5000 },
    { "pinIntervalMs", 70, 100, 5000 },
    { "sprayIntervalMs", 71, 100, 5000 },
    { "aopPredictVersion", 72, 1, 2 },
    { "aopProjectileSpeed", 73, 500, 9000 },
    { "aopLeadScale", 74, 0, 200 },
    { "aopMaxRange", 75, 500, 12000 },
    { "aopReactionMs", 76, 0, 500 },
    { "aopTargetMode", 77, 0, 2 },
    { "combatAuraRange", 78, 500, 12000 },
    { "killauraDisableBelowHealthPercent", 79, 0, 100 },
    { "killauraDisableIfHealthIsBelow", 79, 0, 100 },
    { "autofarmFollowTarget", 80, 0, 1 },
    { "boltSmoothPercent", 81, 50, 100 },
    { "boltWallLookahead", 82, 150, 600 },
    { "boltTargetRange", 83, 600, 2000 },
    { "boltExitHoldMs", 84, 300, 1200 },
    { "combatDodgeRange", 85, 300, 3000 },
    { "combatFireInterval", 86, 50, 2000 },
    { "dodgePrecision", 87, 8, 32 },
    { "dodgeReactionPercent", 88, 50, 180 },
    { "dodgeSafetyPercent", 89, 50, 180 },
    { "dodgePowerPercent", 90, 60, 140 },
    { "dodgeDistancePercent", 91, 70, 140 },
    { "dodgeCommitPercent", 92, 50, 150 },
    { "dodgeBurstHoldMs", 93, 300, 1600 },
    { "v2DodgeReactionPercent", 94, 50, 180 },
    { "v2DodgeSafetyPercent", 95, 50, 180 },
    { "v2DodgePowerPercent", 96, 60, 140 },
    { "v2DodgeDistancePercent", 97, 70, 140 },
    { "v2DodgeCommitPercent", 98, 50, 150 },
    { "v2DodgeBurstHoldMs", 99, 300, 1600 },
    { "v3DodgeReactionPercent", 100, 50, 180 },
    { "v3DodgeSafetyPercent", 101, 50, 180 },
    { "v3DodgePowerPercent", 102, 60, 140 },
    { "v3DodgeDistancePercent", 103, 70, 140 },
    { "v3DodgeCommitPercent", 104, 50, 150 },
    { "v3DodgeBurstHoldMs", 105, 300, 1600 },
    { "v4DodgeReactionPercent", 106, 50, 180 },
    { "v4DodgeSafetyPercent", 107, 50, 180 },
    { "v4DodgePowerPercent", 108, 60, 140 },
    { "v4DodgeDistancePercent", 109, 70, 140 },
    { "v4DodgeCommitPercent", 110, 50, 150 },
    { "v4DodgeBurstHoldMs", 111, 300, 1600 },
    { "v5DodgeReactionPercent", 112, 50, 180 },
    { "v5DodgeSafetyPercent", 113, 50, 180 },
    { "v5DodgePowerPercent", 114, 60, 140 },
    { "v5DodgeDistancePercent", 115, 70, 140 },
    { "v5DodgeCommitPercent", 116, 50, 150 },
    { "v5DodgeBurstHoldMs", 117, 300, 1600 },
    { "dodgeBlacklistMask", 118, 0, 1023 },
    { "holdToShootDelayMs", 119, 0, 1000 },
    { "ballAssistSearchDegrees", 120, 5, 180 },
    { "ballAssistBounceLimit", 121, 0, 8 },
    { "outlineOpacity", 122, 0, 1000 },
    { "outlineColorR", 123, 0, 1000 },
    { "outlineColorG", 124, 0, 1000 },
    { "outlineColorB", 125, 0, 1000 },
    { "dodgeVersion", 126, 1, 5 },
    { "speedLevel", 127, 4, 4 },
    { "aopAimTargetMode", 128, 1, 2 },
    { "aimTargetMode", 128, 1, 2 },
    { "xrayTargetMode", 129, 1, 2 },
    { "spinMovementMode", 130, 0, 1 },
    { "spinOnlineOnlyIdle", 131, 0, 1 },
    { "aopAimUltimate", 132, 0, 1 },
    { "aopAimGadget", 133, 0, 1 },
};

static const ev_anchor_t ev_anchor_patches[10] = {
    { 0xb337c4u, 16u, { 0xff, 0x43, 0x02, 0xd1, 0xfd, 0x7b, 0x03, 0xa9, 0xfc, 0x6f, 0x04, 0xa9, 0xfa, 0x67, 0x05, 0xa9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0xb533fcu, 12u, { 0x08, 0x8c, 0x44, 0xf9, 0x00, 0x15, 0x40, 0xf9, 0xc0, 0x03, 0x5f, 0xd6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0xcf5044u, 16u, { 0xfd, 0x7b, 0xbe, 0xa9, 0xf3, 0x0b, 0x00, 0xf9, 0xfd, 0x03, 0x00, 0x91, 0xab, 0x27, 0x00, 0x94, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0xf86e6cu, 16u, { 0x08, 0x14, 0x40, 0xf9, 0x01, 0x00, 0x41, 0xb9, 0x02, 0x00, 0x80, 0x12, 0xe0, 0x03, 0x08, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0xf86fd0u, 16u, { 0x28, 0x00, 0x80, 0x52, 0x01, 0x0c, 0x01, 0xb9, 0x02, 0x10, 0x01, 0xb9, 0x03, 0x50, 0x04, 0x39, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0xf4845cu, 16u, { 0x1f, 0x20, 0x00, 0xf9, 0x1f, 0x7c, 0x00, 0xa9, 0x1f, 0x08, 0x00, 0xf9, 0x1f, 0x70, 0x01, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0xaa2830u, 16u, { 0xfd, 0x7b, 0xba, 0xa9, 0xfc, 0x6f, 0x01, 0xa9, 0xfa, 0x67, 0x02, 0xa9, 0xf8, 0x5f, 0x03, 0xa9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0xcfeefcu, 12u, { 0x48, 0x30, 0x00, 0xb0, 0x00, 0x11, 0x47, 0xf9, 0xc0, 0x03, 0x5f, 0xd6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0xcffd10u, 8u, { 0x00, 0x24, 0x40, 0xf9, 0xc0, 0x03, 0x5f, 0xd6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
    { 0xcffd20u, 16u, { 0x08, 0x50, 0x40, 0xb9, 0x1f, 0x01, 0x01, 0x6b, 0xe0, 0x17, 0x9f, 0x1a, 0xc0, 0x03, 0x5f, 0xd6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } },
};

static const uint64_t ev_magic1_words[2] = {
    0xa8c47bfda94257f6ull, 0xd65f03c0ull
};
static const uint64_t ev_magic2_words[3] = {
    0xa9425ff8a94357f6ull, 0xa8c57bfda94167faull, 0xd65f03c0ull
};
static const uint8_t ev_guard_region1[0x100] = {
    0xfd, 0x7b, 0xbc, 0xa9, 0xf7, 0x0b, 0x00, 0xf9,
    0xf6, 0x57, 0x02, 0xa9, 0xf4, 0x4f, 0x03, 0xa9,
    0xfd, 0x03, 0x00, 0x91, 0xf3, 0x03, 0x01, 0xaa,
    0xf4, 0x03, 0x00, 0xaa, 0xe0, 0x03, 0x01, 0xaa,
    0xe1, 0x03, 0x1f, 0x2a, 0x86, 0x2c, 0x0d, 0x94,
    0x61, 0xba, 0x4c, 0x39, 0xf5, 0x03, 0x00, 0xaa,
    0xd4, 0x84, 0x0e, 0x94, 0x02, 0xab, 0x0c, 0x94,
    0xf6, 0x03, 0x1f, 0x2a, 0xc0, 0x05, 0x00, 0x36,
    0x68, 0x06, 0x42, 0xf9, 0x61, 0xba, 0x4c, 0x39,
    0xe0, 0x03, 0x15, 0xaa, 0x1f, 0x01, 0x00, 0xf1,
    0x76, 0x02, 0x88, 0x9a, 0xcb, 0x84, 0x0e, 0x94,
    0xed, 0xa9, 0x0c, 0x94, 0xc0, 0x00, 0x00, 0x37,
    0x61, 0xba, 0x4c, 0x39, 0xe0, 0x03, 0x15, 0xaa,
    0xc6, 0x84, 0x0e, 0x94, 0xf3, 0xa9, 0x0c, 0x94,
    0x60, 0x00, 0x00, 0x36, 0x68, 0xf2, 0x4a, 0x39,
    0x88, 0x00, 0x00, 0x34, 0x83, 0xae, 0x4f, 0xb9,
    0x84, 0xb2, 0x4f, 0xb9, 0x11, 0x00, 0x00, 0x14,
    0xe0, 0x03, 0x16, 0xaa, 0x91, 0x0e, 0x0e, 0x94,
    0xf7, 0x03, 0x00, 0x2a, 0xe0, 0x03, 0x13, 0xaa,
    0xef, 0x2a, 0x0d, 0x94, 0x00, 0x68, 0x01, 0x11,
    0x8d, 0xcf, 0xe9, 0x97, 0x17, 0x00, 0x17, 0x0b,
    0xe0, 0x03, 0x16, 0xaa, 0x8b, 0x0e, 0x0e, 0x94,
    0xf6, 0x03, 0x00, 0x2a, 0xe0, 0x03, 0x13, 0xaa,
    0xe7, 0x2a, 0x0d, 0x94, 0x86, 0xcf, 0xe9, 0x97,
    0xe3, 0x03, 0x17, 0x2a, 0x04, 0x00, 0x16, 0x0b,
    0x86, 0x06, 0x4f, 0xb9, 0xe0, 0x03, 0x14, 0xaa,
    0xe1, 0x03, 0x13, 0xaa, 0xe2, 0x03, 0x15, 0xaa,
    0x25, 0x00, 0x80, 0x52, 0x36, 0x00, 0x80, 0x52,
    0x89, 0x3f, 0x00, 0x94, 0x80, 0x00, 0x00, 0x37,
    0x60, 0x12, 0x40, 0xf9, 0xdb, 0xc2, 0xf3, 0x97,
    0xf6, 0x03, 0x1f, 0x2a, 0xe0, 0x03, 0x16, 0x2a,
    0xf4, 0x4f, 0x43, 0xa9, 0xf7, 0x0b, 0x40, 0xf9,
};
static const uint8_t ev_guard_region2[0x100] = {
    0xfd, 0x7b, 0xbc, 0xa9, 0xf7, 0x0b, 0x00, 0xf9,
    0xf6, 0x57, 0x02, 0xa9, 0xf4, 0x4f, 0x03, 0xa9,
    0xfd, 0x03, 0x00, 0x91, 0xf4, 0x03, 0x01, 0xaa,
    0xf3, 0x03, 0x00, 0xaa, 0xa5, 0x19, 0x07, 0x94,
    0x15, 0x14, 0x40, 0xf9, 0xa3, 0x19, 0x07, 0x94,
    0x00, 0x14, 0x40, 0xf9, 0x3f, 0x60, 0x11, 0x94,
    0xe1, 0x03, 0x1f, 0xaa, 0xc0, 0x00, 0xf8, 0x37,
    0xa8, 0x0e, 0x40, 0xb9, 0x1f, 0x01, 0x00, 0x6b,
    0x6d, 0x00, 0x00, 0x54, 0xa8, 0x02, 0x40, 0xf9,
    0x01, 0x59, 0x60, 0xf8, 0xe0, 0x03, 0x14, 0xaa,
    0xd7, 0x38, 0x0d, 0x94, 0xe0, 0x03, 0x00, 0xb4,
    0x81, 0xba, 0x4c, 0x39, 0xf5, 0x03, 0x00, 0xaa,
    0x00, 0x84, 0x0e, 0x94, 0x2e, 0xaa, 0x0c, 0x94,
    0x60, 0x02, 0x00, 0x36, 0x88, 0x06, 0x42, 0xf9,
    0x81, 0xba, 0x4c, 0x39, 0xe0, 0x03, 0x15, 0xaa,
    0x1f, 0x01, 0x00, 0xf1, 0x96, 0x02, 0x88, 0x9a,
    0xf8, 0x83, 0x0e, 0x94, 0x1a, 0xa9, 0x0c, 0x94,
    0xc0, 0x00, 0x00, 0x37, 0x81, 0xba, 0x4c, 0x39,
    0xe0, 0x03, 0x15, 0xaa, 0xf3, 0x83, 0x0e, 0x94,
    0x20, 0xa9, 0x0c, 0x94, 0x60, 0x00, 0x00, 0x36,
    0x88, 0xf2, 0x4a, 0x39, 0x28, 0x02, 0x00, 0x34,
    0x61, 0xae, 0x4f, 0xb9, 0x62, 0xb2, 0x4f, 0xb9,
    0x1e, 0x00, 0x00, 0x14, 0x3f, 0xc2, 0xf3, 0x97,
    0x68, 0x8e, 0x4f, 0xb9, 0x1f, 0x09, 0x00, 0x71,
    0x81, 0x00, 0x00, 0x54, 0x60, 0x06, 0x45, 0xf9,
    0xe1, 0x03, 0x1f, 0x2a, 0x32, 0x16, 0xf5, 0x97,
    0xe0, 0x03, 0x1f, 0x2a, 0xf4, 0x4f, 0x43, 0xa9,
    0xf7, 0x0b, 0x40, 0xf9, 0xf6, 0x57, 0x42, 0xa9,
    0xfd, 0x7b, 0xc4, 0xa8, 0xc0, 0x03, 0x5f, 0xd6,
    0xe0, 0x03, 0x16, 0xaa, 0xb1, 0x0d, 0x0e, 0x94,
    0xf7, 0x03, 0x00, 0x2a, 0xe0, 0x03, 0x14, 0xaa,
    0x0f, 0x2a, 0x0d, 0x94, 0x00, 0x68, 0x01, 0x11,
};
static const uint8_t ev_guard_region3[0x50] = {
    0xad, 0xce, 0xe9, 0x97, 0x17, 0x00, 0x17, 0x0b,
    0xe0, 0x03, 0x16, 0xaa, 0xab, 0x0d, 0x0e, 0x94,
    0xf6, 0x03, 0x00, 0x2a, 0xe0, 0x03, 0x14, 0xaa,
    0x07, 0x2a, 0x0d, 0x94, 0xa6, 0xce, 0xe9, 0x97,
    0xe1, 0x03, 0x17, 0x2a, 0x02, 0x00, 0x16, 0x0b,
    0x66, 0x06, 0x4f, 0xb9, 0xe0, 0x03, 0x13, 0xaa,
    0xe3, 0x03, 0x14, 0xaa, 0xe4, 0x03, 0x15, 0xaa,
    0x25, 0x00, 0x80, 0x52, 0xf4, 0x4f, 0x43, 0xa9,
    0xf7, 0x0b, 0x40, 0xf9, 0xf6, 0x57, 0x42, 0xa9,
    0xfd, 0x7b, 0xc4, 0xa8, 0x28, 0xff, 0xff, 0x17,
};
static const uint8_t ev_guard_region4[0x100] = {
    0xfd, 0x7b, 0xbb, 0xa9, 0xfa, 0x67, 0x01, 0xa9,
    0xf8, 0x5f, 0x02, 0xa9, 0xf6, 0x57, 0x03, 0xa9,
    0xf4, 0x4f, 0x04, 0xa9, 0xfd, 0x03, 0x00, 0x91,
    0x08, 0x2c, 0x49, 0xb9, 0xf5, 0x03, 0x06, 0x2a,
    0xf6, 0x03, 0x04, 0xaa, 0xf3, 0x03, 0x00, 0xaa,
    0xf4, 0x03, 0x03, 0xaa, 0xf8, 0x03, 0x02, 0x2a,
    0x08, 0x0d, 0x00, 0x51, 0xf9, 0x03, 0x01, 0x2a,
    0x1f, 0x05, 0x00, 0x71, 0xc8, 0x00, 0x00, 0x54,
    0x68, 0xb2, 0x65, 0x39, 0x88, 0x00, 0x00, 0x34,
    0x68, 0x36, 0x49, 0xb9, 0x1f, 0x0d, 0x00, 0x71,
    0xc1, 0x02, 0x00, 0x54, 0xb7, 0x00, 0x00, 0x12,
    0xe0, 0x03, 0x13, 0xaa, 0xe1, 0x03, 0x14, 0xaa,
    0xe2, 0x03, 0x16, 0xaa, 0xe3, 0x03, 0x19, 0x2a,
    0xe4, 0x03, 0x18, 0x2a, 0xe5, 0x03, 0x17, 0x2a,
    0xe6, 0x03, 0x15, 0x2a, 0x96, 0x3b, 0x00, 0x94,
    0x00, 0x01, 0x00, 0x36, 0x20, 0x00, 0x80, 0x52,
    0xf4, 0x4f, 0x44, 0xa9, 0xf6, 0x57, 0x43, 0xa9,
    0xf8, 0x5f, 0x42, 0xa9, 0xfa, 0x67, 0x41, 0xa9,
    0xfd, 0x7b, 0xc5, 0xa8, 0xc0, 0x03, 0x5f, 0xd6,
    0x88, 0xf2, 0x4a, 0x39, 0x48, 0x01, 0x00, 0x34,
    0x80, 0x12, 0x40, 0xf9, 0xaa, 0xc2, 0xf3, 0x97,
    0xe0, 0x03, 0x1f, 0x2a, 0xf4, 0x4f, 0x44, 0xa9,
    0xf6, 0x57, 0x43, 0xa9, 0xf8, 0x5f, 0x42, 0xa9,
    0xfa, 0x67, 0x41, 0xa9, 0xfd, 0x7b, 0xc5, 0xa8,
    0xc0, 0x03, 0x5f, 0xd6, 0x9a, 0x3e, 0x40, 0xb9,
    0xba, 0x01, 0xf8, 0x37, 0xfe, 0x19, 0x07, 0x94,
    0x08, 0x14, 0x40, 0xf9, 0x09, 0x0d, 0x40, 0xb9,
    0x3f, 0x01, 0x1a, 0x6b, 0x0d, 0x01, 0x00, 0x54,
    0x08, 0x01, 0x40, 0xf9, 0x01, 0x79, 0x7a, 0xf8,
    0xe0, 0x03, 0x16, 0xaa, 0xe2, 0x03, 0x14, 0xaa,
    0xc1, 0x84, 0x0e, 0x94, 0xe0, 0x00, 0x00, 0x37,
    0xea, 0xff, 0xff, 0x17, 0xe1, 0x03, 0x1f, 0xaa,
};
static const uint8_t ev_guard_region5[0x100] = {
    0xe0, 0x03, 0x16, 0xaa, 0xe2, 0x03, 0x14, 0xaa,
    0xbb, 0x84, 0x0e, 0x94, 0xa0, 0xfc, 0x07, 0x36,
    0x81, 0xba, 0x4c, 0x39, 0xe0, 0x03, 0x16, 0xaa,
    0x57, 0x84, 0x0e, 0x94, 0x08, 0x00, 0x42, 0xb9,
    0x1f, 0x31, 0x00, 0x71, 0x81, 0x04, 0x00, 0x54,
    0x81, 0xba, 0x4c, 0x39, 0xe0, 0x03, 0x16, 0xaa,
    0x51, 0x84, 0x0e, 0x94, 0x00, 0x40, 0x07, 0x91,
    0xe1, 0x03, 0x1f, 0xaa, 0x33, 0xc1, 0x0b, 0x94,
    0x81, 0xba, 0x4c, 0x39, 0xf5, 0x03, 0x00, 0xaa,
    0xe0, 0x03, 0x16, 0xaa, 0x4a, 0x84, 0x0e, 0x94,
    0x00, 0x80, 0x07, 0x91, 0xe1, 0x03, 0x1f, 0xaa,
    0x2c, 0xc1, 0x0b, 0x94, 0xf6, 0x03, 0x00, 0xaa,
    0xe0, 0x03, 0x14, 0xaa, 0xfb, 0x2b, 0x0d, 0x94,
    0x08, 0x04, 0x40, 0xf9, 0x09, 0xe0, 0xa7, 0x52,
    0xe0, 0x03, 0x14, 0xaa, 0x7f, 0x92, 0x07, 0xf9,
    0xbf, 0x02, 0x08, 0xeb, 0x69, 0x32, 0x0f, 0xb9,
    0xc8, 0x02, 0x95, 0x9a, 0x68, 0x96, 0x07, 0xf9,
    0xf2, 0x2b, 0x0d, 0x94, 0xe8, 0x03, 0x00, 0xaa,
    0x29, 0x00, 0x80, 0x52, 0xe0, 0x03, 0x1f, 0x2a,
    0x09, 0x81, 0x00, 0x39, 0xf4, 0x4f, 0x44, 0xa9,
    0xf6, 0x57, 0x43, 0xa9, 0xf8, 0x5f, 0x42, 0xa9,
    0xfa, 0x67, 0x41, 0xa9, 0xfd, 0x7b, 0xc5, 0xa8,
    0xc0, 0x03, 0x5f, 0xd6, 0x81, 0xba, 0x4c, 0x39,
    0xe0, 0x03, 0x16, 0xaa, 0x79, 0x12, 0x0f, 0xb9,
    0x78, 0x16, 0x0f, 0xb9, 0x2c, 0x84, 0x0e, 0x94,
    0x4a, 0x33, 0x93, 0x52, 0x69, 0x96, 0x47, 0xf9,
    0x00, 0x10, 0x2c, 0x1e, 0x2a, 0xd3, 0xa7, 0x72,
    0xe8, 0x03, 0x00, 0xaa, 0xe0, 0x03, 0x1f, 0x2a,
    0x41, 0x01, 0x27, 0x1e, 0x3f, 0x01, 0x00, 0xf1,
    0x77, 0x62, 0x3c, 0x39, 0x68, 0x92, 0x07, 0xf9,
    0x75, 0x1e, 0x0f, 0xb9, 0x20, 0x0c, 0x20, 0x1e,
    0x60, 0x36, 0x0f, 0xbd, 0xf4, 0x4f, 0x44, 0xa9,
};
static const uint8_t ev_island_prologue[0x150] = {
    0xff, 0x43, 0x0c, 0xd1, 0xe0, 0x07, 0x00, 0xa9,
    0xe2, 0x0f, 0x01, 0xa9, 0xe4, 0x17, 0x02, 0xa9,
    0xe6, 0x1f, 0x03, 0xa9, 0xe8, 0x27, 0x04, 0xa9,
    0xea, 0x2f, 0x05, 0xa9, 0xec, 0x37, 0x06, 0xa9,
    0xee, 0x3f, 0x07, 0xa9, 0xf0, 0x47, 0x08, 0xa9,
    0xf2, 0x4f, 0x09, 0xa9, 0xf4, 0x57, 0x0a, 0xa9,
    0xf6, 0x5f, 0x0b, 0xa9, 0xf8, 0x67, 0x0c, 0xa9,
    0xfa, 0x6f, 0x0d, 0xa9, 0xfc, 0x77, 0x0e, 0xa9,
    0xfe, 0x7b, 0x00, 0xf9, 0x09, 0x42, 0x3b, 0xd5,
    0xe9, 0x7f, 0x00, 0xf9, 0x09, 0x44, 0x3b, 0xd5,
    0xe9, 0x83, 0x00, 0xf9, 0x29, 0x44, 0x3b, 0xd5,
    0xe9, 0x87, 0x00, 0xf9, 0xe0, 0x87, 0x08, 0xad,
    0xe2, 0x8f, 0x09, 0xad, 0xe4, 0x97, 0x0a, 0xad,
    0xe6, 0x9f, 0x0b, 0xad, 0xe8, 0xa7, 0x0c, 0xad,
    0xea, 0xaf, 0x0d, 0xad, 0xec, 0xb7, 0x0e, 0xad,
    0xee, 0xbf, 0x0f, 0xad, 0xf0, 0xc7, 0x10, 0xad,
    0xf2, 0xcf, 0x11, 0xad, 0xf4, 0xd7, 0x12, 0xad,
    0xf6, 0xdf, 0x13, 0xad, 0xf8, 0xe7, 0x14, 0xad,
    0xfa, 0xef, 0x15, 0xad, 0xfc, 0xf7, 0x16, 0xad,
    0xfe, 0xff, 0x17, 0xad, 0xe0, 0x03, 0x00, 0x91,
    0x90, 0x05, 0x00, 0x58, 0x00, 0x02, 0x3f, 0xd6,
    0xe0, 0x87, 0x48, 0xad, 0xe2, 0x8f, 0x49, 0xad,
    0xe4, 0x97, 0x4a, 0xad, 0xe6, 0x9f, 0x4b, 0xad,
    0xe8, 0xa7, 0x4c, 0xad, 0xea, 0xaf, 0x4d, 0xad,
    0xec, 0xb7, 0x4e, 0xad, 0xee, 0xbf, 0x4f, 0xad,
    0xf0, 0xc7, 0x50, 0xad, 0xf2, 0xcf, 0x51, 0xad,
    0xf4, 0xd7, 0x52, 0xad, 0xf6, 0xdf, 0x53, 0xad,
    0xf8, 0xe7, 0x54, 0xad, 0xfa, 0xef, 0x55, 0xad,
    0xfc, 0xf7, 0x56, 0xad, 0xfe, 0xff, 0x57, 0xad,
    0xe9, 0x87, 0x40, 0xf9, 0x29, 0x44, 0x1b, 0xd5,
    0xe9, 0x83, 0x40, 0xf9, 0x09, 0x44, 0x1b, 0xd5,
    0xe9, 0x7f, 0x40, 0xf9, 0x09, 0x42, 0x1b, 0xd5,
    0xfe, 0x7b, 0x40, 0xf9, 0xfc, 0x77, 0x4e, 0xa9,
    0xfa, 0x6f, 0x4d, 0xa9, 0xf8, 0x67, 0x4c, 0xa9,
    0xf6, 0x5f, 0x4b, 0xa9, 0xf4, 0x57, 0x4a, 0xa9,
    0xf2, 0x4f, 0x49, 0xa9, 0xf0, 0x47, 0x48, 0xa9,
    0xee, 0x3f, 0x47, 0xa9, 0xec, 0x37, 0x46, 0xa9,
    0xea, 0x2f, 0x45, 0xa9, 0xe8, 0x27, 0x44, 0xa9,
    0xe6, 0x1f, 0x43, 0xa9, 0xe4, 0x17, 0x42, 0xa9,
    0xe2, 0x0f, 0x41, 0xa9, 0xe0, 0x07, 0x40, 0xa9,
    0xff, 0x43, 0x0c, 0x91, 0x1f, 0x20, 0x03, 0xd5,
    0x00, 0x00, 0x00, 0x14, 0x1f, 0x20, 0x03, 0xd5,
};
static const uint8_t ev_branch_template[0x218] = {
    0xfc, 0x6f, 0x01, 0xa9, 0xfa, 0x67, 0x02, 0xa9,
    0xf8, 0x5f, 0x03, 0xa9, 0xf6, 0x57, 0x04, 0xa9,
    0xf4, 0x4f, 0x05, 0xa9, 0xfd, 0x03, 0x00, 0x91,
    0x29, 0x08, 0x40, 0xb9, 0xf3, 0x03, 0x01, 0xaa,
    0xf4, 0x03, 0x00, 0xaa, 0x3f, 0x51, 0x00, 0x71,
    0xa8, 0x00, 0x00, 0x54, 0xe8, 0xff, 0x85, 0x52,
    0xe8, 0x03, 0xa0, 0x72, 0x08, 0x25, 0xc9, 0x1a,
    0x88, 0x05, 0x00, 0x37, 0x96, 0x82, 0x00, 0x91,
    0xd5, 0x02, 0x40, 0xf9, 0xa8, 0x0e, 0x40, 0xb9,
    0x1f, 0x79, 0x00, 0x71, 0xea, 0x02, 0x00, 0x54,
    0x89, 0x16, 0x40, 0xb9, 0xaa, 0x0a, 0x40, 0xb9,
    0x29, 0x05, 0x00, 0x11, 0x1f, 0x01, 0x0a, 0x6b,
    0x89, 0x16, 0x00, 0xb9, 0x69, 0x02, 0x00, 0xb9,
    0x41, 0x01, 0x00, 0x54, 0x09, 0x79, 0x1f, 0x53,
    0xaa, 0x00, 0x80, 0x52, 0x3f, 0x01, 0x00, 0x71,
    0x41, 0x01, 0x89, 0x1a, 0x1f, 0x01, 0x01, 0x6b,
    0x8a, 0x00, 0x00, 0x54, 0xe0, 0x03, 0x15, 0xaa,
    0x75, 0x02, 0x00, 0x94, 0xa8, 0x0e, 0x40, 0xb9,
    0xa9, 0x02, 0x40, 0xf9, 0x0a, 0x05, 0x00, 0x11,
    0x9f, 0x0a, 0x00, 0xb9, 0xaa, 0x0e, 0x00, 0xb9,
    0x33, 0xd9, 0x28, 0xf8, 0x0a, 0x00, 0x00, 0x14,
    0x33, 0x01, 0x00, 0xb4, 0xe0, 0x03, 0x13, 0xaa,
    0xf4, 0x4f, 0x45, 0xa9, 0xf6, 0x57, 0x44, 0xa9,
    0xf8, 0x5f, 0x43, 0xa9, 0xfa, 0x67, 0x42, 0xa9,
    0xfc, 0x6f, 0x41, 0xa9, 0xfd, 0x7b, 0xc6, 0xa8,
    0xcd, 0xff, 0x1b, 0x14, 0xf4, 0x4f, 0x45, 0xa9,
    0xf6, 0x57, 0x44, 0xa9, 0xf8, 0x5f, 0x43, 0xa9,
    0xfa, 0x67, 0x42, 0xa9, 0xfc, 0x6f, 0x41, 0xa9,
    0xfd, 0x7b, 0xc6, 0xa8, 0xc0, 0x03, 0x5f, 0xd6,
    0xf6, 0x03, 0x14, 0xaa, 0xd5, 0x0e, 0x42, 0xf8,
    0xa8, 0x0e, 0x40, 0xb9, 0x1f, 0x01, 0x00, 0x71,
    0xcd, 0xfa, 0xff, 0x54, 0x18, 0x05, 0x00, 0x51,
    0x6b, 0xbb, 0xff, 0xb0, 0x6b, 0x81, 0x07, 0x91,
    0x0a, 0x7f, 0x7d, 0xd3, 0x77, 0x79, 0x69, 0xb8,
    0xf9, 0x03, 0x28, 0x2a, 0xf5, 0x03, 0x1f, 0x2a,
    0x7b, 0xbb, 0xff, 0xb0, 0x7b, 0xd3, 0x08, 0x91,
    0x5a, 0x41, 0x00, 0x91, 0xfc, 0x03, 0x19, 0x2a,
    0x09, 0x00, 0x00, 0x14, 0xb4, 0xff, 0x1b, 0x94,
    0xe8, 0x03, 0x18, 0x2a, 0x1f, 0x03, 0x00, 0xf1,
    0x18, 0x07, 0x00, 0xd1, 0xb5, 0x06, 0x00, 0x11,
    0x9c, 0x07, 0x00, 0x11, 0x5a, 0x23, 0x00, 0xd1,
    0xcd, 0xf7, 0xff, 0x54, 0xcb, 0x02, 0x40, 0xf9,
    0x6a, 0x01, 0x40, 0xf9, 0x49, 0x0d, 0x18, 0x8b,
    0x20, 0x01, 0x40, 0xf9, 0x0c, 0x08, 0x40, 0xb9,
    0x9f, 0x51, 0x00, 0x71, 0xa8, 0x00, 0x00, 0x54,
    0x6c, 0x7b, 0x6c, 0xb8, 0xff, 0x02, 0x0c, 0x6b,
    0x01, 0xfe, 0xff, 0x54, 0x04, 0x00, 0x00, 0x14,
    0x0c, 0x00, 0x80, 0x12, 0xff, 0x02, 0x0c, 0x6b,
    0x81, 0xfd, 0xff, 0x54, 0x6d, 0x0d, 0x40, 0xb9,
    0xac, 0x05, 0x00, 0x51, 0xa8, 0x01, 0x08, 0x6b,
    0x6c, 0x0d, 0x00, 0xb9, 0xcd, 0xfc, 0xff, 0x54,
    0x1f, 0x05, 0x00, 0x71, 0x8b, 0xfc, 0xff, 0x54,
    0xab, 0x02, 0x19, 0x0b, 0xab, 0x01, 0x0b, 0x0b,
    0x7f, 0x0d, 0x00, 0x71, 0x23, 0x02, 0x00, 0x54,
    0x6b, 0x05, 0x00, 0x91, 0xad, 0x01, 0x1c, 0x0b,
    0x4a, 0x01, 0x1a, 0x8b, 0x6c, 0x79, 0x7e, 0x92,
    0xad, 0x05, 0x00, 0x91, 0x29, 0x0d, 0x0c, 0x8b,
    0x08, 0x01, 0x0c, 0x4b, 0xad, 0x79, 0x7e, 0x92,
    0x40, 0x81, 0xdf, 0x3c, 0x41, 0x81, 0xc0, 0x3c,
    0xad, 0x11, 0x00, 0xf1, 0x40, 0x85, 0x3f, 0xad,
    0x4a, 0x81, 0x00, 0x91, 0x61, 0xff, 0xff, 0x54,
    0x7f, 0x01, 0x0c, 0xeb, 0x20, 0x01, 0x00, 0x54,
    0x08, 0x05, 0x00, 0x11, 0x29, 0x21, 0x00, 0x91,
    0x2a, 0x01, 0x40, 0xf9, 0x08, 0x05, 0x00, 0x51,
    0x1f, 0x05, 0x00, 0x71, 0x2a, 0x81, 0x1f, 0xf8,
    0x29, 0x21, 0x00, 0x91, 0x68, 0xff, 0xff, 0x54,
    0xe0, 0xf8, 0xff, 0xb5, 0xc7, 0xff, 0xff, 0x17,
};
static const uint8_t ev_adapter_required[16] = { 0x10, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00 };

static int ev_adapter_present(const uint64_t *adapters, uint32_t idx)
{
    return adapters[idx] != 0;
}

static uint32_t ev_state[EV_STATE_COUNT];
static uint32_t ev_settings_generation;
static volatile uint8_t ev_state_lock;

struct ev_bindings {
    uint64_t tag_a;
    uint64_t tag_b;
    const char *build_id;
    const char *image_id;
    int (*read_mem)(void *ctx, uint64_t addr, void *out, uint32_t len);
    void *read_ctx;
};

struct ev_observer {
    uint64_t bound;
    uint64_t module_tag;
    uint64_t dispatch_fn;
    uint64_t reserved3;
    uint64_t word4;
    uint64_t primary_fn;
    uint64_t word6;
};

struct ev_handlers {
    uint64_t version_word;
    uint64_t bind_epoch;
    uint64_t dispatch_fn;
    uint64_t word3;
    uint64_t word4;
    uint64_t word5;
    uint64_t word6;
    uint64_t word7;
};

struct ev_slot5 {
    uint64_t version_word;
    uint64_t bind_epoch;
    uint64_t dispatch_fn;
    uint64_t callback;
    uint64_t ctx;
};

struct ev_routes {
    uint64_t version_word;
    uint64_t bind_epoch;
    uint64_t dispatch_fn;
    uint64_t word3;
    uint64_t word4;
    uint64_t word5;
    uint64_t word6;
    uint64_t word7;
    uint64_t word8;
    uint64_t word9;
    uint64_t word10;
    uint64_t word11;
    uint64_t word12;
    uint64_t word13;
    uint64_t word14;
    uint64_t word15;
    uint64_t word16;
    uint64_t word17;
    uint64_t word18;
    uint64_t word19;
    uint64_t word20;
};

struct ev_spin_post {
    uint64_t version_word;
    uint64_t bind_epoch;
    uint64_t dispatch_fn;
    uint64_t callback;
    uint64_t ctx;
    uint64_t word5;
    uint64_t word6;
    uint64_t word7;
    uint64_t word8;
};

struct ev_functions {
    uint64_t version_word;
    uint64_t bind_epoch;
    uint64_t dispatch_fn;
    uint64_t word3;
    uint64_t word4;
    uint64_t adapters[10];
};

struct ev_package_query {
    uint64_t hw_a;
    uint64_t word0;
    uint64_t epoch;
    uint64_t hw_b;
    uint64_t word4;
    uint64_t ctx;
    uint64_t word5;
    uint64_t word6;
    uint64_t word7;
    uint64_t fn;
    uint64_t word9;
};

struct ev_spin_route {
    uint64_t hw_a;
    uint64_t word1;
    uint64_t epoch;
    uint64_t hw_b;
    uint64_t word4;
    uint64_t word5;
    uint64_t word6;
    uint64_t word7;
    uint64_t fn;
    uint64_t word9;
};

struct ev_registries {
    struct ev_bindings bindings;
    struct ev_observer observer;
    uint64_t bind_generation;
    struct ev_handlers handlers;
    uint64_t family_ctx;
    uint64_t family_fn;
    struct ev_slot5 visual;
    struct ev_slot5 periodic;
    uint8_t restored_desc[0x250];
    uint8_t restored_aux[0x228];
    struct ev_routes routes;
    uint64_t routes_seq;
    uint64_t routes_last_epoch;
    uint64_t routes_last_ready;
    uint64_t routes_last_phase;
    struct ev_spin_post spin_post;
    uint64_t spin_post_seq;
    struct ev_spin_post spin_movement_post;
    uint64_t spin_movement_seq;
    struct ev_spin_route spin_route;
    struct ev_package_query package_query;
    uint64_t package_identity[14];
    uint64_t functions_epoch_hold;
    struct ev_functions functions;
    uint64_t functions_version_hold;
    uint64_t reset_generation;
    uint64_t package_fail_sticky;
    uint64_t downstream_generation;
    uint64_t teardown_generation;
};

static struct ev_registries R;
static void *ev_default_state_template = (void *)0x1047a0;
static uint64_t ev_frame_observed;
static uint64_t ev_frame_accepted;
static uint64_t ev_frame_status_word4;
static uint64_t ev_frame_status_word6;
static uint64_t ev_frame_stable;
static uint32_t ev_frame_reason;
static uint32_t ev_frame_ready;
static uint64_t ev_frame_epoch;

static int ev_anchor_verify(const void *desc, uint8_t aux_out[0x228]);
static int ev_spin_routes_ready(void);
static int ev_package_identity_recheck(void);
static int ev_routes_stable(const struct ev_routes *snapshot);
static int ev_read_game(const uint64_t *d, uint64_t addr, void *out, uint32_t len);

static void ev_lock(void)
{
    while (__atomic_test_and_set(&ev_state_lock, __ATOMIC_ACQUIRE)) {
    }
}

static void ev_unlock(void)
{
    __atomic_clear(&ev_state_lock, __ATOMIC_RELEASE);
}

static const ev_key_t *ev_find_key(const char *name)
{
    size_t i;

    if (name == NULL) {
        return NULL;
    }
    for (i = 0; i < EV_LIVE_KEYS; i++) {
        if (strcmp(name, ev_keys[i].name) == 0) {
            return &ev_keys[i];
        }
    }
    return NULL;
}

static void ev_teardown_downstream(void)
{
    memset(&R.visual, 0, sizeof(R.visual));
    memset(&R.periodic, 0, sizeof(R.periodic));
    memset(&R.restored_desc, 0, sizeof(R.restored_desc));
    memset(&R.restored_aux, 0, sizeof(R.restored_aux));
    if (R.package_query.hw_a < R.package_query.hw_b) {
        R.package_query.hw_a = R.package_query.hw_b;
    }
    R.package_query.word0 = 0;
    R.package_query.epoch = 0;
    R.package_query.hw_b = 0;
    R.package_query.word4 = 0;
    R.package_query.ctx = 0;
    R.package_query.word5 = 0;
    R.package_query.word6 = 0;
    R.package_query.word7 = 0;
    R.package_query.fn = 0;
    R.package_query.word9 = 0;
    R.teardown_generation++;
    memset(&R.package_identity, 0, sizeof(R.package_identity));
    R.package_fail_sticky = 0;
    memset(&R.spin_post, 0, sizeof(R.spin_post));
    memset(&R.spin_movement_post, 0, sizeof(R.spin_movement_post));
    memset(&R.spin_route, 0, sizeof(R.spin_route));
    R.downstream_generation++;
    R.reset_generation++;
}

static void ev_teardown_all(void)
{
    memset(&R.bindings, 0, sizeof(R.bindings));
    memset(&R.observer, 0, sizeof(R.observer));
    memset(&R.handlers, 0, sizeof(R.handlers));
    ev_teardown_downstream();
    memset(&R.routes, 0, sizeof(R.routes));
    R.routes_seq = 0;
    R.routes_last_epoch = 0;
    R.routes_last_ready = 0;
    R.routes_last_phase = 0;
    R.functions_epoch_hold = 0;
    memset(&R.functions, 0, sizeof(R.functions));
    R.functions_version_hold = 0;
}

int32_t nexus_evasion_get_requested(const char *name)
{
    const ev_key_t *key;
    uint32_t v;

    if (name == NULL) {
        return (int32_t)EV_NOT_FOUND;
    }
    key = ev_find_key(name);
    if (key == NULL) {
        return (int32_t)EV_NOT_FOUND;
    }
    ev_lock();
    evasion_state_lazy_init();
    v = ev_state[key->state_index];
    ev_unlock();
    return (int32_t)v;
}

int32_t nexus_evasion_get_effective(const char *name)
{
    const ev_key_t *key;
    uint32_t v;

    if (name == NULL) {
        return (int32_t)EV_NOT_FOUND;
    }
    key = ev_find_key(name);
    if (key == NULL) {
        return (int32_t)EV_NOT_FOUND;
    }
    if (evasion_key_gated(name) == 0 && key < &ev_keys[EV_FEATURE_KEYS]) {
        int held;
        ev_lock();
        evasion_state_lazy_init();
        if (ev_state[key->state_index] == 0) {
            ev_unlock();
            return 0;
        }
        held = evasion_dependencies_hold(name);
        ev_unlock();
        return held != 0;
    }
    ev_lock();
    evasion_state_lazy_init();
    v = ev_state[key->state_index];
    ev_unlock();
    return (int32_t)v;
}

int nexus_evasion_initialize(void)
{
    ev_lock();
    evasion_state_lazy_init();
    ev_unlock();
    return 1;
}

int nexus_evasion_restore_v1(const char **names, const int32_t *values, uint32_t count)
{
    uint32_t state_map[142];
    uint32_t i;
    int any_set = 0;

    if (count >= EV_LIVE_KEYS + 1) {
        return 0xfffffffe;
    }
    if (count != 0 && (names == NULL || values == NULL)) {
        return 0xfffffffe;
    }
    for (i = 0; i < count; i++) {
        const ev_key_t *key;
        int32_t v;

        if (names[i] == NULL) {
            return 0xfffffffe;
        }
        key = ev_find_key(names[i]);
        if (key == NULL) {
            return 0xffffffff;
        }
        v = values[i];
        if (v < key->vmin || v > key->vmax) {
            return 0xfffffffe;
        }
        state_map[i] = key->state_index;
        {
            uint32_t j;
            for (j = 0; j < i; j++) {
                if (state_map[i] == state_map[j] && v != values[j]) {
                    return 0xfffffffe;
                }
            }
        }
        if (v != 0) {
            any_set = 1;
        }
    }
    ev_lock();
    evasion_state_lazy_init();
    for (i = 0; i < count; i++) {
        uint32_t v;

        if (strcmp(names[i], "combatDodgeRequireHold") == 0) {
            const ev_key_t *k = ev_find_key("combatDodgeRequireHold");
            v = (state_map[i] == k->state_index) ? 0 : (uint32_t)values[i];
        } else {
            v = (uint32_t)values[i];
        }
        ev_state[state_map[i]] = v;
    }
    ev_settings_generation++;
    ev_unlock();
    return 1 + any_set;
}

void nexus_evasion_unbind(void)
{
    ev_lock();
    R.bind_generation++;
    ev_teardown_all();
    ev_default_state_template = (void *)0x1047a0;
    ev_unlock();
}

void nexus_evasion_reset(void)
{
    ev_lock();
    R.bind_generation++;
    memset(&R.bindings, 0, sizeof(R.bindings));
    memset(&R.observer, 0, sizeof(R.observer));
    memset(&R.handlers, 0, sizeof(R.handlers));
    ev_teardown_downstream();
    memset(&R.routes, 0, sizeof(R.routes));
    R.routes_seq = 0;
    R.routes_last_epoch = 0;
    R.routes_last_ready = 0;
    R.routes_last_phase = 0;
    memset(&ev_state, 0, sizeof(ev_state));
    ev_settings_generation = 0;
    ev_frame_observed = 0;
    ev_frame_accepted = 0;
    ev_frame_stable = 0;
    ev_frame_reason = 0;
    ev_frame_ready = 0;
    R.downstream_generation++;
    R.teardown_generation++;
    ev_default_state_template = (void *)0x1047a0;
    ev_unlock();
}

static int ev_dladdr_same_module(const void *a, const void *b, const void *c)
{
    Dl_info ia, ib, ic;

    if (dladdr(a, &ia) == 0 || ia.dli_fbase == NULL) {
        return 0;
    }
    if (dladdr(b, &ib) == 0 || ib.dli_fbase != ia.dli_fbase) {
        return 0;
    }
    if (dladdr(c, &ic) == 0 || ic.dli_fbase != ia.dli_fbase) {
        return 0;
    }
    return 1;
}

int nexus_evasion_publish_observer_v1(const uint64_t *desc)
{
    struct ev_observer snapshot;
    uint64_t generation;

    if (desc == NULL) {
        return 0;
    }
    generation = R.bind_generation;
    ev_lock();
    snapshot = R.observer;
    ev_unlock();
    if (snapshot.bound == 0 ||
        (snapshot.bound == desc[0] && snapshot.module_tag == desc[1] &&
         snapshot.dispatch_fn == desc[2] && snapshot.reserved3 == desc[3] &&
         snapshot.word4 == desc[4] && snapshot.primary_fn == desc[5] &&
         snapshot.word6 == desc[6])) {
        ev_lock();
        if (generation == R.bind_generation && R.observer.bound == snapshot.bound &&
            R.observer.module_tag == snapshot.module_tag &&
            R.observer.dispatch_fn == snapshot.dispatch_fn &&
            R.observer.reserved3 == snapshot.reserved3 &&
            R.observer.word4 == snapshot.word4 &&
            R.observer.primary_fn == snapshot.primary_fn &&
            R.observer.word6 == snapshot.word6) {
            R.observer.bound = desc[0];
            R.observer.module_tag = desc[1];
            R.observer.dispatch_fn = desc[2];
            R.observer.reserved3 = desc[3];
            R.observer.word4 = desc[4];
            R.observer.primary_fn = desc[5];
            R.observer.word6 = desc[6];
            ev_unlock();
            return 1;
        }
        ev_unlock();
        return 0;
    }
    return 0;
}

static int ev_register_slot5(struct ev_slot5 *slot, const uint64_t *desc)
{
    uint64_t generation;

    if (desc == NULL) {
        ev_lock();
        memset(slot, 0, sizeof(*slot));
        ev_unlock();
        return 0;
    }
    if ((uint32_t)desc[0] != 1 || (uint32_t)(desc[0] >> 32) != 0x28) {
        return 0;
    }
    if (desc[1] == 0 || (void *)desc[2] == NULL || (void *)desc[3] == NULL ||
        (void *)desc[4] == NULL) {
        return 0;
    }
    if (!ev_dladdr_same_module((void *)desc[2], (void *)desc[3], (void *)desc[4])) {
        return 0;
    }
    generation = R.bind_generation;
    ev_lock();
    if (R.bindings.read_mem == 0 || (void *)desc[2] != (void *)R.observer.primary_fn) {
        ev_unlock();
        return 0;
    }
    ev_unlock();
    ev_lock();
    if (generation == R.bind_generation && desc[1] == R.handlers.bind_epoch &&
        slot->version_word != 0 && slot->version_word == desc[0] &&
        slot->bind_epoch == desc[1] && slot->dispatch_fn == desc[2] &&
        slot->callback == desc[3] && slot->ctx == desc[4]) {
        ev_unlock();
        return 1;
    }
    if (generation == R.bind_generation && desc[1] == R.handlers.bind_epoch) {
        slot->version_word = desc[0];
        slot->bind_epoch = desc[1];
        slot->dispatch_fn = desc[2];
        slot->callback = desc[3];
        slot->ctx = desc[4];
        ev_unlock();
        return 1;
    }
    ev_unlock();
    return 0;
}

int nexus_evasion_register_visual_v1(const uint64_t *desc)
{
    return ev_register_slot5(&R.visual, desc);
}

int nexus_evasion_register_periodic_v1(const uint64_t *desc)
{
    return ev_register_slot5(&R.periodic, desc);
}

int nexus_evasion_register_restored_v1(const void *desc)
{
    const uint32_t *h = desc;
    const uint64_t *d = desc;
    uint64_t generation;

    if (desc == NULL) {
        ev_lock();
        memset(&R.restored_desc, 0, sizeof(R.restored_desc));
        memset(&R.restored_aux, 0, sizeof(R.restored_aux));
        ev_unlock();
        return 0;
    }
    if (h[0] != 1 || h[1] != 0x250) {
        return 0;
    }
    if (d[1] == 0 || d[2] == 0 || d[3] == 0 || d[4] == 0 || d[5] == 0) {
        return 0;
    }
    {
        Dl_info i2, i3, i5;
        if (dladdr((void *)d[2], &i2) == 0 || dladdr((void *)d[3], &i3) == 0 ||
            dladdr((void *)d[5], &i5) == 0 || i2.dli_fbase == NULL ||
            i2.dli_fbase != i3.dli_fbase || i2.dli_fbase != i5.dli_fbase) {
            return 0;
        }
    }
    generation = R.bind_generation;
    ev_lock();
    if (R.bindings.read_mem == 0 || (void *)d[2] != (void *)R.observer.primary_fn) {
        ev_unlock();
        return 0;
    }
    ev_unlock();
    {
        uint8_t aux[0x228];
        if (!ev_anchor_verify(desc, aux)) {
            return 0;
        }
        ev_lock();
        if (generation == R.bind_generation && R.restored_desc[0] != 0) {
            if (memcmp(&R.restored_desc, desc, 0x250) != 0) {
                ev_unlock();
                return 0;
            }
        }
        memcpy(&R.restored_desc, desc, 0x250);
        memcpy(&R.restored_aux, aux, 0x228);
        ev_unlock();
        return 1;
    }
}

static int ev_read_game(const uint64_t *d, uint64_t addr, void *out, uint32_t len)
{
    return ((int (*)(void *, uint64_t, void *, uint32_t))d[4])(NULL, addr, out, len);
}

static int ev_island_descriptor_ok(const uint64_t *d, uint8_t *aux)
{
    uint64_t island = ((uint64_t *)aux)[0];
    uint64_t island_end = ((uint64_t *)aux)[1];
    uint64_t target = ((uint64_t *)aux)[2];
    uint32_t *w = (uint32_t *)aux;
    uint64_t fwd_dist;
    uint64_t bwd_dist;
    uint8_t probe[0x220];

    if (island != d[1] + EV_GAME_ISLAND_REF_OFF) {
        return 0;
    }
    if (target < 0x10000 || (target & 3) != 0) {
        return 0;
    }
    if (island_end < island || island_end - island >= 0x8000000ull) {
        return 0;
    }
    if ((island_end + 0x148) - (island + 4) >= 0x8000000ull) {
        return 0;
    }
    fwd_dist = (island_end + 0x148) - (island + 4);
    bwd_dist = (island + 4) - (island_end + 0x148);
    w[0x5a] = 0x14000000u | ((uint32_t)(fwd_dist >> 2) & 0x3ffffffu);
    w[0x60] = 0x14000000u | ((uint32_t)(bwd_dist >> 2) & 0x3ffffffu);
    if (memcmp(aux, ev_branch_template, 0x218) != 0) {
        return 0;
    }
    if (ev_read_game(d, island, probe, 0x21c) != 1) {
        return 0;
    }
    if (memcmp(probe, aux, 4) != 0) {
        return 0;
    }
    if (ev_read_game(d, island_end, probe, 0x208) != 1) {
        return 0;
    }
    if (memcmp(probe, aux + 0x20, 0x208) != 0) {
        return 0;
    }
    return 1;
}

static int ev_anchor_verify(const void *desc, uint8_t aux_out[0x228])
{
    const uint32_t *h = (const uint32_t *)desc;
    const uint64_t *d = (const uint64_t *)desc;
    uint64_t game_base;
    uint64_t anchor_addr;
    uint64_t island;
    uint64_t read_back;
    uint8_t buf[0x220];
    uint32_t i;

    if (aux_out != NULL && aux_out[0] != 0) {
        if (!ev_island_descriptor_ok(d, aux_out)) {
            return 0;
        }
    }
    if (h[0] != 1 || h[2] != 0x30) {
        return 0;
    }
    game_base = d[1];
    if (game_base < 0x10000 || (game_base & 0xfff) != 0) {
        return 0;
    }
    if (d[4] == 0) {
        return 0;
    }
    if ((void *)d[2] == NULL || (void *)d[3] == NULL) {
        return 0;
    }
    if (strcmp((const char *)d[2], EV_BUILD_ID) != 0) {
        return 0;
    }
    if (strcmp((const char *)d[3], EV_IMAGE_ID) != 0) {
        return 0;
    }
    anchor_addr = d[4];
    if (anchor_addr != game_base + EV_GAME_ANCHOR_OFF) {
        return 0;
    }
    island = d[5];
    if (island != anchor_addr + 4 || island < 0x10000 || (island & 0xf) != 0) {
        return 0;
    }
    read_back = d[7];
    if (read_back < 0x10000 || (read_back & 3) != 0) {
        return 0;
    }
    if (h[0x16] != (uint32_t)0xd10243ff) {
        return 0;
    }
    if (((island | anchor_addr) & 3) != 0) {
        return 0;
    }
    {
        uint32_t insn = h[0x16];
        uint64_t target_addr;
        if ((insn & 0xfc000000) != 0x14000000) {
            return 0;
        }
        target_addr = (uint64_t)(insn & 0x3ffffff) << 2;
        if ((insn & 0x3ffffff) >> 25 != 0) {
            target_addr |= 0xfffffffff0000000ull;
        }
        if (island - anchor_addr != target_addr) {
            return 0;
        }
    }
    for (i = 0; i < 10; i++) {
        const ev_anchor_t *a = &ev_anchor_patches[i];
        uint8_t expect[24];
        if (a->len > 0x10) {
            return 0;
        }
        if (ev_read_game(d, a->offset + game_base, buf, a->len) != 1) {
            return 0;
        }
        memcpy(expect, a->bytes, a->len);
        if (a->offset == EV_GAME_ANCHOR_OFF) {
            memcpy(expect, &h[0x16], 4);
        } else if (a->offset == EV_GAME_ISLAND_REF_OFF && aux_out != NULL &&
                   aux_out[0] != 0) {
            memcpy(expect, &aux_out[0x1c], 4);
        }
        if (memcmp(buf, expect, a->len) != 0) {
            return 0;
        }
    }
    if (ev_read_game(d, island, buf, 0x158) != 1) {
        return 0;
    }
    if (memcmp(buf, ev_island_prologue, 0x150) != 0) {
        return 0;
    }
    return 1;
}

int nexus_evasion_register_handlers_v1(const uint64_t *desc)
{
    uint64_t generation;
    Dl_info d2, d7, dx;

    if (desc == NULL) {
        ev_lock();
        ev_teardown_all();
        ev_unlock();
        return 1;
    }
    if ((uint32_t)desc[0] != 1 || (uint32_t)(desc[0] >> 32) != 0x40) {
        return 0;
    }
    if (desc[1] == 0 || desc[2] == 0 || desc[7] == 0 ||
        (desc[4] == 0 && desc[5] == 0 && desc[6] == 0)) {
        return 0;
    }
    if (dladdr((void *)desc[2], &d2) == 0 || d2.dli_fbase == NULL) {
        return 0;
    }
    if (dladdr((void *)desc[7], &d7) == 0 || d7.dli_fbase != d2.dli_fbase) {
        return 0;
    }
    if (desc[4] != 0) {
        if (dladdr((void *)desc[4], &dx) == 0 || dx.dli_fbase != d2.dli_fbase) {
            return 0;
        }
    }
    if (desc[5] != 0) {
        if (dladdr((void *)desc[5], &dx) == 0 || dx.dli_fbase != d2.dli_fbase) {
            return 0;
        }
    }
    if (desc[6] != 0) {
        if (dladdr((void *)desc[6], &dx) == 0 || dx.dli_fbase != d2.dli_fbase) {
            return 0;
        }
    }
    generation = R.bind_generation;
    ev_lock();
    if (R.bindings.read_mem == 0 || desc[1] != R.handlers.bind_epoch ||
        (void *)desc[2] != (void *)R.observer.primary_fn) {
        ev_unlock();
        return 0;
    }
    ev_unlock();
    ev_lock();
    if (generation != R.bind_generation || R.handlers.bind_epoch == 0) {
        ev_unlock();
        return 0;
    }
    if (desc[1] < R.handlers.bind_epoch) {
        ev_unlock();
        return 0;
    }
    if (desc[1] != R.handlers.bind_epoch) {
        ev_teardown_downstream();
        R.handlers.version_word = desc[0];
        R.handlers.bind_epoch = desc[1];
        R.handlers.dispatch_fn = desc[2];
        R.handlers.word3 = desc[3];
        R.handlers.word4 = desc[4];
        R.handlers.word5 = desc[5];
        R.handlers.word6 = desc[6];
        R.handlers.word7 = desc[7];
        ev_unlock();
        return 1;
    }
    {
        int same = R.handlers.version_word == desc[0] &&
                   R.handlers.bind_epoch == desc[1] &&
                   R.handlers.dispatch_fn == desc[2] &&
                   R.handlers.word3 == desc[3] &&
                   R.handlers.word4 == desc[4] &&
                   R.handlers.word5 == desc[5] &&
                   R.handlers.word6 == desc[6] &&
                   R.handlers.word7 == desc[7];
        ev_unlock();
        return same;
    }
}

static int ev_attempt_stamp(uint64_t *stamp_out)
{
    uint32_t source;
    uint32_t token;

    evasion_state_lazy_init();
    source = evasion_stamp_source();
    token = 0;
    if (R.family_fn != 0) {
        token = ((uint32_t (*)(uint64_t))R.family_fn)(R.family_ctx);
    }
    return evasion_stamp_compose(source, token, stamp_out, 0);
}

int nexus_evasion_attempt_stamp_v1(uint64_t *desc)
{
    uint64_t stamp[5];

    if (desc == NULL) {
        return 0;
    }
    if ((uint32_t)desc[0] != 1 || (uint32_t)(desc[0] >> 32) != 0x28) {
        return 0;
    }
    ev_lock();
    if (ev_attempt_stamp(stamp)) {
        memcpy(desc, stamp, 0x28);
        ev_unlock();
        return 1;
    }
    ev_unlock();
    return 0;
}

int nexus_evasion_attempt_recheck_v1(const uint64_t *desc)
{
    uint64_t stamp[5];

    if (desc == NULL) {
        return 0;
    }
    if ((uint32_t)desc[0] != 1 || (uint32_t)(desc[0] >> 32) != 0x28) {
        return 0;
    }
    ev_lock();
    if (ev_attempt_stamp(stamp)) {
        int same = desc[0] == stamp[0] && desc[1] == stamp[1] && desc[2] == stamp[2] &&
                   desc[3] == stamp[3] && desc[4] == stamp[4];
        ev_unlock();
        return same;
    }
    ev_unlock();
    return 0;
}

static int ev_routes_stable(const struct ev_routes *snapshot)
{
    return R.routes.version_word == snapshot->version_word &&
           R.routes.bind_epoch == snapshot->bind_epoch &&
           R.routes.dispatch_fn == snapshot->dispatch_fn &&
           R.routes.word3 == snapshot->word3 &&
           R.routes.word4 == snapshot->word4 &&
           R.routes.word5 == snapshot->word5 &&
           R.routes.word6 == snapshot->word6 &&
           R.routes.word7 == snapshot->word7 &&
           R.routes.word8 == snapshot->word8;
}

int nexus_evasion_publish_event_routes_v2(const uint64_t *desc)
{
    struct ev_routes snapshot;
    struct ev_routes candidate;
    uint64_t generation;

    if (desc == NULL) {
        return 0;
    }
    generation = R.bind_generation;
    ev_lock();
    snapshot = R.routes;
    {
        struct ev_slot5 v = R.visual;
        struct ev_slot5 p = R.periodic;
        (void)v;
        (void)p;
    }
    ev_unlock();
    if (snapshot.version_word != 0) {
        if (desc[3] != R.observer.primary_fn || desc[1] != R.handlers.bind_epoch) {
            return 0;
        }
    }
    memcpy(&candidate, desc, sizeof(candidate));
    if (!evasion_route_check_ok(&candidate)) {
        return 0;
    }
    {
        uint64_t hook_addr = ((uint64_t *)&snapshot)[8];
        if (desc[9] + 0x170 <= hook_addr || hook_addr + 0x158 <= desc[9] ||
            desc[0xe] + 0x170 <= hook_addr || hook_addr + 0x158 <= desc[0xe] ||
            desc[0x13] + 0x170 <= hook_addr || hook_addr + 0x158 <= desc[0x13]) {
            ev_lock();
            if (generation == R.bind_generation && ev_routes_stable(&snapshot) &&
                memcmp(&snapshot, &R.routes, 0xa8) == 0) {
                R.routes = candidate;
                ev_unlock();
                return 1;
            }
            ev_unlock();
            return 0;
        }
    }
    return 0;
}

int nexus_evasion_event_routes_v2(uint64_t *desc)
{
    if (desc == NULL) {
        return 0;
    }
    if ((uint32_t)desc[0] != 2 || (uint32_t)(desc[0] >> 32) != 0xa8) {
        return 0;
    }
    ev_lock();
    memcpy(desc, &R.routes, 0xa8);
    ev_unlock();
    return 1;
}

static int ev_publish_spin_post(struct ev_spin_post *slot, const uint64_t *desc,
                                int movement)
{
    uint64_t generation;
    struct ev_spin_post snapshot;

    if (desc == NULL) {
        return 0;
    }
    generation = R.bind_generation;
    ev_lock();
    snapshot = *slot;
    ev_unlock();
    if (desc[1] != R.handlers.bind_epoch) {
        return 0;
    }
    if (snapshot.version_word != 0 &&
        memcmp(&snapshot, desc, 0x48) != 0) {
        return 0;
    }
    if (movement) {
        if (!evasion_spin_route_validate((void *)slot, &R.spin_post, &R.bindings,
                                         &R.routes)) {
            return 0;
        }
    } else {
        if (!evasion_spin_route_validate((void *)slot, &snapshot, &R.bindings, &R.routes)) {
            return 0;
        }
    }
    ev_lock();
    if (generation == R.bind_generation && ev_routes_stable(&R.routes) &&
        memcmp(&R.routes, &R.routes, 0xa8) == 0) {
        memcpy(slot, desc, 0x48);
        ev_unlock();
        return 1;
    }
    ev_unlock();
    return 0;
}

int nexus_evasion_publish_spin_post_v1(const uint64_t *desc)
{
    return ev_publish_spin_post(&R.spin_post, desc, 0);
}

int nexus_evasion_spin_post_v1(uint64_t *desc)
{
    if (desc == NULL) {
        return 0;
    }
    if ((uint32_t)desc[0] != 1 || (uint32_t)(desc[0] >> 32) != 0x48) {
        return 0;
    }
    ev_lock();
    memcpy(desc, &R.spin_post, 0x48);
    ev_unlock();
    return 1;
}

int nexus_evasion_spin_validate_post_v1(const uint64_t *desc)
{
    if (desc == NULL) {
        return 0;
    }
    if ((uint32_t)desc[0] != 1 || (uint32_t)(desc[0] >> 32) != 0x48) {
        return 0;
    }
    ev_lock();
    if (R.bindings.read_mem != 0) {
        uint8_t aux[0x228];
        int ok = ev_anchor_verify(&R.restored_desc, aux);
        ev_unlock();
        return ok;
    }
    ev_unlock();
    return 0;
}

int nexus_evasion_publish_spin_movement_post_v1(const uint64_t *desc)
{
    return ev_publish_spin_post(&R.spin_movement_post, desc, 1);
}

int nexus_evasion_spin_movement_post_v1(uint64_t *desc)
{
    if (desc == NULL) {
        return 0;
    }
    if ((uint32_t)desc[0] != 1 || (uint32_t)(desc[0] >> 32) != 0x48) {
        return 0;
    }
    ev_lock();
    memcpy(desc, &R.spin_movement_post, 0x48);
    ev_unlock();
    return 1;
}

int nexus_evasion_spin_validate_movement_post_v1(const uint64_t *desc)
{
    return nexus_evasion_spin_validate_post_v1(desc);
}

int nexus_evasion_spin_register_v1(const uint64_t *desc)
{
    Dl_info d2, d6, d7;

    if (desc == NULL) {
        ev_lock();
        memset(&R.spin_route, 0, sizeof(R.spin_route));
        memset(&R.package_query, 0, sizeof(R.package_query));
        memset(&R.package_identity, 0, sizeof(R.package_identity));
        R.package_fail_sticky = 0;
        R.teardown_generation++;
        R.downstream_generation++;
        ev_unlock();
        return 0;
    }
    if ((uint32_t)desc[0] != 1 || (uint32_t)(desc[0] >> 32) != 0x48) {
        return 0;
    }
    if (desc[1] == 0 || desc[6] == 0 || desc[7] == 0 || desc[8] == 0) {
        return 0;
    }
    if (dladdr((void *)desc[6], &d6) == 0 || d6.dli_fbase == NULL) {
        return 0;
    }
    if (dladdr((void *)desc[7], &d7) == 0 || d7.dli_fbase != d6.dli_fbase) {
        return 0;
    }
    if (dladdr((void *)desc[2], &d2) == 0 || d2.dli_fbase != d6.dli_fbase) {
        return 0;
    }
    ev_lock();
    if (R.handlers.bind_epoch != 0) {
        if (R.spin_route.epoch == 0) {
            if (!ev_spin_routes_ready()) {
                R.package_fail_sticky = 0;
                R.teardown_generation++;
                ev_unlock();
                return 1;
            }
        } else if (R.spin_route.epoch == R.handlers.bind_epoch &&
                   ev_anchor_verify(&R.restored_desc, NULL)) {
            if (R.spin_route.hw_a <= R.package_query.hw_b) {
                R.spin_route.hw_a = R.package_query.hw_b;
            }
            ev_unlock();
            return 1;
        }
        ev_unlock();
        return 0;
    }
    ev_unlock();
    return 0;
}

static int ev_spin_routes_ready(void)
{
    static pthread_once_t once = PTHREAD_ONCE_INIT;
    int ok;

    pthread_once(&once, (void (*)(void))evasion_once_init);
    if (R.package_query.fn == 0) {
        return 0;
    }
    if (R.spin_route.hw_a == 0 || R.spin_route.word1 == 0 ||
        R.bindings.read_mem == 0 || R.routes.version_word == 0 ||
        R.spin_post.version_word == 0 || R.spin_route.epoch != R.handlers.bind_epoch ||
        R.spin_movement_seq != R.handlers.bind_epoch) {
        return 0;
    }
    if ((R.package_fail_sticky & 1) != 0) {
        return 0;
    }
    ok = ((int (*)(uint64_t, void *))R.package_query.fn)(R.package_query.ctx,
                                                         (void *)0);
    if (ok != 1) {
        return 0;
    }
    return 1;
}

int nexus_evasion_spin_lease_v1(uint64_t slot_a, uint64_t slot_b, uint64_t *lease_out)
{
    if ((uint32_t)slot_a != 1 || (uint32_t)(slot_a >> 32) != 0x60) {
        return 0;
    }
    ev_lock();
    if (evasion_lease_compose(slot_a, slot_b, lease_out)) {
        ev_unlock();
        return 1;
    }
    ev_unlock();
    return 0;
}

int nexus_evasion_spin_recheck_v1(const uint64_t *lease)
{
    uint64_t fresh[12];

    if (lease == NULL) {
        return 0;
    }
    if ((uint32_t)lease[0] != 1 || (uint32_t)(lease[0] >> 32) != 0x60) {
        return 0;
    }
    ev_lock();
    if (evasion_lease_compose(lease[0], lease[1], fresh)) {
        int same = memcmp(lease, fresh, 0x60) == 0;
        ev_unlock();
        return same;
    }
    ev_unlock();
    return 0;
}


int nexus_evasion_functions_register_v1(const uint64_t *desc)
{
    const uint64_t *adapters;
    Dl_info d2, dx;
    uint64_t generation;
    uint32_t i;

    if (desc == NULL) {
        ev_lock();
        memset(&R.functions, 0, sizeof(R.functions));
        R.functions_epoch_hold = 0;
        R.functions_version_hold = 0;
        ev_unlock();
        return 0;
    }
    if ((uint32_t)desc[0] != 1 || (uint32_t)(desc[0] >> 32) != 0x80) {
        return 0;
    }
    if (desc[1] == 0 || desc[2] == 0 || desc[7] == 0 || desc[4] == 0) {
        return 0;
    }
    adapters = desc + 5;
    if (dladdr((void *)desc[4], &dx) == 0 || dx.dli_fbase == NULL) {
        return 0;
    }
    if (dladdr((void *)desc[2], &d2) == 0 || d2.dli_fbase != dx.dli_fbase) {
        return 0;
    }
    if (dladdr((void *)desc[7], &dx) == 0 || dx.dli_fbase != d2.dli_fbase) {
        return 0;
    }
    for (i = 0; i < 10; i++) {
        if (adapters[i] != 0) {
            if (dladdr((void *)adapters[i], &dx) == 0 || dx.dli_fbase != d2.dli_fbase) {
                return 0;
            }
        }
    }
    if ((adapters[3] != 0) && adapters[2] == 0) {
        return 0;
    }
    if (adapters[0] == 0 && adapters[1] == 0 && adapters[2] == 0 && adapters[3] == 0 &&
        adapters[4] == 0 && adapters[5] == 0 && adapters[6] == 0 && adapters[7] == 0 &&
        adapters[8] == 0 && adapters[9] == 0) {
        return 0;
    }
    if ((adapters[0] != 0) != (adapters[1] != 0)) {
        return 0;
    }
    if (!ev_adapter_present(adapters, 4) && ev_adapter_present(adapters, 5)) {
        return 0;
    }
    if (!ev_adapter_present(adapters, 6) && ev_adapter_present(adapters, 7)) {
        return 0;
    }
    if (ev_adapter_required[4] && !ev_adapter_present(adapters, 4)) {
        return 0;
    }
    if (ev_adapter_required[12] && !ev_adapter_present(adapters, 6)) {
        return 0;
    }
    generation = R.bind_generation;
    ev_lock();
    if (R.bindings.read_mem == 0 || desc[1] != R.handlers.bind_epoch) {
        ev_unlock();
        return 0;
    }
    if ((void *)desc[4] != (void *)R.observer.primary_fn) {
        ev_unlock();
        return 0;
    }
    ev_unlock();
    ev_lock();
    if (generation == R.bind_generation && ev_routes_stable(&R.routes) &&
        memcmp(&R.routes, &R.routes, 0xa8) == 0) {
        if (R.functions_version_hold != 0) {
            if (memcmp(&R.functions, desc, 0x80) != 0) {
                ev_unlock();
                return 0;
            }
            ev_unlock();
            return 1;
        }
        memcpy(&R.functions, desc, 0x80);
        R.functions_epoch_hold = desc[1];
        ev_unlock();
        return 1;
    }
    ev_unlock();
    return 0;
}

int nexus_evasion_functions_lease_v1(uint64_t *lease_out)
{
    ev_lock();
    if (!ev_package_identity_recheck()) {
        ev_unlock();
        return 0;
    }
    if (R.functions_version_hold != 0) {
        memcpy(lease_out, &R.functions, 0x80);
        ev_unlock();
        return 1;
    }
    ev_unlock();
    return 0;
}

int nexus_evasion_functions_recheck_v1(const uint64_t *lease)
{
    ev_lock();
    if (!ev_package_identity_recheck()) {
        ev_unlock();
        return 0;
    }
    if (R.functions_version_hold != 0) {
        int same = memcmp(lease, &R.functions, 0x80) == 0;
        ev_unlock();
        return same;
    }
    ev_unlock();
    return 0;
}

static int ev_package_identity_recheck(void)
{
    uint8_t response[0x90];
    const uint32_t *h = (const uint32_t *)response;
    const uint64_t *w = (const uint64_t *)response;
    int ok;

    if (R.package_query.epoch == 0 || R.package_query.fn == 0 ||
        R.bindings.read_mem == 0 || R.observer.bound == 0 ||
        R.routes.version_word == 0) {
        return 0;
    }
    if (R.package_query.epoch != R.handlers.bind_epoch ||
        R.package_query.epoch != R.routes.bind_epoch) {
        return 0;
    }
    ok = ((int (*)(uint64_t, void *))R.package_query.fn)(R.package_query.ctx, response);
    if (ok != 1) {
        return 0;
    }
    if (h[0] != 1 || h[2] != 0x90 || h[4] != 2 || h[5] != 0x1d) {
        return 0;
    }
    if (w[7] != R.package_query.epoch) {
        return 0;
    }
    if (w[6] == R.package_query.word4 || w[5] == 0 || (uint32_t)w[8] != 1 ||
        (uint32_t)(w[8] >> 32) > 1) {
        return 0;
    }
    if (w[0x10] != 0x746975732e647362ull ||
        w[0x11] != 0x78656e2e65736163ull ||
        w[0x12] != 0x32767375ull || w[0x13] != 0) {
        return 0;
    }
    if (w[0x14] != 0x3530316162343131ull || w[0x15] != 0x3531346462353338ull) {
        return 0;
    }
    if (R.package_identity[8] <= w[5]) {
        if (w[5] == R.package_identity[8]) {
            if ((R.package_fail_sticky & 1) != 0 ||
                memcmp(response, &R.package_identity, 0x90) != 0) {
                R.package_fail_sticky = 1;
                return 0;
            }
        } else {
            R.package_fail_sticky = 0;
            memcpy(&R.package_identity, response, 0x90);
        }
        return (uint32_t)w[8] == 0;
    }
    return 0;
}

static int ev_region_guard_ok(const uint64_t *d, uint64_t game_off, uint32_t size,
                              const uint8_t *ref)
{
    uint8_t buf[0x220];

    if (ev_read_game(d, game_off, buf, size) != 1) {
        return 0;
    }
    if (memcmp(buf, ref, size) != 0) {
        return 0;
    }
    return 1;
}

static int ev_region_magic_ok(const uint64_t *d, uint64_t game_off,
                              const uint64_t *words, uint32_t n_bytes)
{
    uint8_t buf[0x20];

    if (ev_read_game(d, game_off, buf, n_bytes) != 1) {
        return 0;
    }
    if (memcmp(buf, words, n_bytes) != 0) {
        return 0;
    }
    return 1;
}

int nexus_evasion_bind_image_v1(const void *contract)
{
    const uint32_t *h = (const uint32_t *)contract;
    const uint64_t *d = (const uint64_t *)contract;
    uint64_t game_base;
    uint64_t generation;
    uint32_t i;
    int bound_now;

    if (contract == NULL) {
        return 0;
    }
    if (h[0] != 1 || h[2] != 0x30) {
        return 0;
    }
    game_base = d[1];
    if (game_base < 0x10000 || (game_base & 0xfff) != 0) {
        return 0;
    }
    if (d[4] == 0) {
        return 0;
    }
    if ((void *)d[2] == NULL || (void *)d[3] == NULL) {
        return 0;
    }
    if (strcmp((const char *)d[2], EV_BUILD_ID) != 0) {
        return 0;
    }
    if (strcmp((const char *)d[3], EV_IMAGE_ID) != 0) {
        return 0;
    }
    ev_lock();
    bound_now = R.bindings.read_mem != 0;
    ev_unlock();
    if (!bound_now) {
        if (game_base + EV_REGION1_OFF < 0x10008) {
            return 0;
        }
        if (!ev_region_guard_ok(d, game_base + EV_REGION1_OFF, 0x100, ev_guard_region1)) {
            return 0;
        }
        if (!ev_region_magic_ok(d, game_base + EV_MAGIC1_OFF, ev_magic1_words, 0xc)) {
            return 0;
        }
        if (game_base + EV_REGION2_OFF < 0x10008) {
            return 0;
        }
        if (!ev_region_guard_ok(d, game_base + EV_REGION2_OFF, 0x100, ev_guard_region2)) {
            return 0;
        }
        if (!ev_region_guard_ok(d, game_base + EV_REGION3_OFF, 0x50, ev_guard_region3)) {
            return 0;
        }
        if (!ev_region_guard_ok(d, game_base + EV_REGION4_OFF, 0x100, ev_guard_region4)) {
            return 0;
        }
        if (!ev_region_guard_ok(d, game_base + EV_REGION5_OFF, 0x100, ev_guard_region5)) {
            return 0;
        }
        if (!ev_region_magic_ok(d, game_base + EV_MAGIC2_OFF, ev_magic2_words, 0x14)) {
            return 0;
        }
        goto verified;
    }
    {
        struct ev_bindings candidate;
        if (!evasion_contract_validate(contract, &candidate)) {
            return 0;
        }
        ev_lock();
        {
            int stable = candidate.read_mem == R.bindings.read_mem &&
                         candidate.read_ctx == R.bindings.read_ctx &&
                         candidate.tag_a == R.bindings.tag_a &&
                         candidate.tag_b == R.bindings.tag_b;
            ev_unlock();
            if (stable) {
                goto verified;
            }
        }
        return 0;
    }

verified:
    if (!ev_anchor_verify(contract, NULL)) {
        return 0;
    }
    generation = R.bind_generation;
    ev_lock();
    if (R.bindings.read_mem != 0) {
        int same;
        ev_unlock();
        ev_lock();
        same = generation == R.bind_generation && R.observer.bound != 0 &&
               R.bindings.read_mem != 0 && R.bindings.tag_b == generation;
        ev_unlock();
        return same;
    }
    ev_unlock();
    nexus_evasion_unbind();
    if (h[0] != 1 || h[2] != 0x30) {
        return 0;
    }
    if (game_base < 0x12000 || (game_base & 7) != 0) {
        return 0;
    }
    if (game_base >= 0xfffffffffec00000ull) {
        return 0;
    }
    for (i = 0; i < 10; i++) {
        const ev_anchor_t *a = &ev_anchor_patches[i];
        uint8_t buf[24];
        if (a->len > 0x10) {
            return 0;
        }
        if (ev_read_game(d, a->offset + game_base, buf, a->len) != 1) {
            return 0;
        }
        if (memcmp(buf, a->bytes, a->len) != 0) {
            return 0;
        }
    }
    ev_lock();
    R.bindings.build_id = EV_BUILD_ID;
    R.bindings.image_id = EV_IMAGE_ID;
    ev_unlock();
    R.bind_generation++;
    return 1;
}

int nexus_evasion_observe_frame_v1(const uint64_t *desc)
{
    uint64_t consumer_fn;
    uint64_t consumer_ctx;
    uint64_t frame_obj;
    uint64_t sub_obj;
    uint32_t kind;
    uint32_t reason = 0;
    int ok = 0;

    if (desc == NULL) {
        return 0;
    }
    if ((uint32_t)desc[0] != 1 || (uint32_t)(desc[0] >> 32) != 0x28) {
        return 0;
    }
    consumer_fn = (uint64_t)(uintptr_t)R.bindings.read_mem;
    consumer_ctx = (uint64_t)(uintptr_t)R.bindings.read_ctx;
    if (consumer_fn == 0) {
        reason = 1;
        goto reject;
    }
    if (desc[2] < 0x10000 || (desc[2] & 7) != 0 || desc[3] < 0x10000 ||
        (desc[3] & 7) != 0) {
        reason = 1;
        goto reject;
    }
    {
        struct {
            int (*read_mem)(void *, uint64_t, void *, uint32_t);
            void *ctx;
        } rd;
        rd.read_mem = (void *)consumer_fn;
        rd.ctx = (void *)consumer_ctx;
        if (rd.read_mem(rd.ctx, 0x1307e20ull, &frame_obj, 8) != 1) {
            reason = 2;
            goto reject;
        }
        if (frame_obj < 0x10000 || (frame_obj & 7) != 0) {
            reason = 2;
            goto reject;
        }
        if (rd.read_mem(rd.ctx, frame_obj + 0x50, &kind, 4) != 1 || kind != 5) {
            reason = 2;
            goto reject;
        }
        if (rd.read_mem(rd.ctx, frame_obj + 0x48, &sub_obj, 8) != 1) {
            reason = 2;
            goto reject;
        }
        if (sub_obj < 0x10000 || (sub_obj & 7) != 0) {
            reason = 2;
            goto reject;
        }
        if (desc[2] + 0x920 < 0x10008) {
            reason = 3;
            goto reject;
        }
        {
            uint64_t surface_a;
            if (rd.read_mem(rd.ctx, desc[2] + 0x918, &surface_a, 8) != 1) {
                reason = 3;
                goto reject;
            }
            if (surface_a != desc[3]) {
                reason = 3;
                goto reject;
            }
        }
        ok = 1;
    }
    ev_lock();
    if (ok) {
        ev_frame_stable++;
        ev_frame_epoch++;
    } else {
reject:
        ev_lock();
        ev_frame_observed++;
        ev_frame_stable = 0;
        ev_frame_ready = 0;
        ev_frame_reason = reason;
        ev_unlock();
        return 0;
    }
    ev_frame_observed++;
    ev_frame_accepted++;
    ev_frame_reason = 0;
    ev_frame_ready = 1;
    ev_unlock();
    return 1;
}

int nexus_evasion_get_frame_status_v1(uint64_t *desc)
{
    if (desc == NULL) {
        return 0;
    }
    if ((uint32_t)desc[0] != 1 || (uint32_t)(desc[0] >> 32) != 0x48) {
        return 0;
    }
    ev_lock();
    desc[0] = ev_frame_observed;
    desc[1] = ev_frame_status_word4;
    desc[2] = ev_frame_status_word6;
    desc[3] = ev_frame_stable;
    desc[4] = ev_frame_epoch;
    desc[5] = ev_frame_accepted;
    desc[6] = ev_frame_ready;
    desc[7] = ev_frame_reason;
    desc[8] = (uint64_t)(uintptr_t)ev_default_state_template;
    ev_unlock();
    return 1;
}

size_t nexus_evasion_diagnostics(char *buf, size_t n)
{
    char tmp[0x400];
    const char *guards;
    const char *ready;
    int len;

    ev_lock();
    guards = "false";
    if (R.bindings.read_mem != 0) {
        guards = "true";
    }
    ready = "false";
    if (ev_frame_ready != 0) {
        ready = "true";
    }
    len = snprintf(tmp, sizeof(tmp),
                   "{\"schema\":1,\"component\":\"nexus-evasion-69252\","
                   "\"settings_generation\":%llu,\"keys\":%u,"
                   "\"image_instruction_guards\":%s,"
                   "\"host_file_identity_required\":true,"
                   "\"hooks_installed\":0,\"gameplay_capabilities\":0,"
                   "\"frame_ready\":%s,\"stable_frames\":%llu,"
                   "\"accepted_frames\":%llu,\"rejected_frames\":%llu,"
                   "\"frame_reason\":%u,\"replay_block\":%d}",
                   (unsigned long long)ev_settings_generation, EV_LIVE_KEYS, guards,
                   ready, (unsigned long long)ev_frame_stable, (unsigned long long)ev_frame_accepted,
                   (unsigned long long)(ev_frame_observed - ev_frame_accepted),
                   ev_frame_reason, (int)(R.package_fail_sticky & 1));
    ev_unlock();
    if (len < 0) {
        return 0;
    }
    if (buf != NULL && n != 0) {
        size_t copy = (size_t)len;
        if (n <= copy) {
            copy = n - 1;
        }
        memcpy(buf, tmp, copy);
        buf[copy] = 0;
    }
    return (size_t)len + 1;
}

int nexus_evasion_register_frame_family_v1(const uint64_t *desc)
{
    if (desc == NULL) {
        ev_lock();
        ev_teardown_all();
        ev_unlock();
        return 1;
    }
    if ((uint32_t)desc[0] != 1 || (uint32_t)(desc[0] >> 32) != 0x18 || desc[2] == 0) {
        return 0;
    }
    ev_lock();
    if (desc[1] == R.family_ctx && desc[2] == R.family_fn) {
        ev_unlock();
        return 1;
    }
    ev_unlock();
    ev_lock();
    ev_teardown_all();
    ev_unlock();
    return 1;
}
