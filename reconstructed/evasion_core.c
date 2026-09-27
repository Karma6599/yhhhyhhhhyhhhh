#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

extern int  evasion_is_free_scope(const char *name);
extern int  evasion_entitlement_check(void);
extern void evasion_commit(void);
extern int  evasion_key_is_visual_only(const char *name);
extern int  evasion_key_adapter_family(const char *name);
extern int  evasion_key_is_aim_family(const char *name);
extern int  evasion_hold_family_ok(const char *name);
extern int  evasion_spin_gate(void);
extern int  evasion_brawler_gate_index(const char *name);
extern int  evasion_brawler_gate_set(const char *n, int v);
extern uint32_t evasion_brawler_bitmap(void);
extern int  evasion_brawler_bitmap_index(const char *n);
extern int  evasion_prediction_alias_ok(const char *n);
extern int  periodic_adapter_ready(uint32_t adapter_id_slot, uint32_t bind_fn_slot);
extern int  adapter_family_ready(uint32_t id_slot, uint32_t bind_fn_slot);
extern int32_t evasion_get_requested(const char *name);
extern int32_t evasion_get_effective(const char *name);
extern int32_t evasion_get_port_state(const char *name);
extern void    nexus_evasion_diagnostics(char *buf, size_t n);
extern void    nexus_evasion_initialize(void);
extern void    nexus_evasion_restore_v1(void);
void backend_misc(void);

#define PROP_FEATURE 0
#define PROP_PARAM   1

#define COERCE_NONE           0
#define COERCE_FOLLOW_ALIAS   1
#define COERCE_DODGE_VERSION  2
#define COERCE_TARGET_MODE    3
#define COERCE_SPEED_LEVEL    4

#define EV_UNKNOWN_KEY   (-1)
#define EV_REJECTED      2
#define EV_APPLIED       1
#define EV_APPLIED_ACTIVE 2

#define BK_OK            1
#define BK_APPLIED       1
#define BK_ACTIVE        2
#define BK_REJECTED      4
#define BK_PORT_DOWN     0
#define BK_BAD_REQUEST   4

#define PORT_IDLE     0
#define PORT_ACTIVE_1 1
#define PORT_ACTIVE_2 2
#define PORT_DETACHED 3

#define TOK_TOGGLE   2
#define TOK_SLIDER   3
#define TOK_MODE     4

#define EVASION_BACKEND_SIZE  0x40
#define EVASION_REQUEST_MAGIC 0x40
#define EVASION_QUERY_SIZE    0x18
#define EVASION_QUERY_MAGIC   0x18

#define EV_MAX_NAME 0x60
#define EV_STATE_COUNT 134

typedef struct {
    const char *name;
    int32_t     state_index;
    int32_t     kind;
    int32_t     vmin;
    uint32_t    vmax;
    uint32_t    default_value;
    int32_t     coerce;
    bool        active;
} nexus_property_t;

typedef struct {
    const char *name;
    bool        is_parameter;
    int32_t     token;
    int32_t     blacklist_bit;
} evasion_action_t;

typedef struct {
    const char *name;
    int32_t     kind;
} evasion_query_slot_t;

typedef struct {
    uint32_t magic;
    uint32_t slot;
    int32_t  token;
    int32_t  value;
    uint32_t _pad0[8];
    uint32_t arg_e;
    uint32_t _pad1;
    uint64_t cookie;
} evasion_request_t;

typedef struct {
    uint32_t magic;
    int32_t  value;
    int32_t  effective;
    int32_t  requested;
    uint32_t status;
    uint8_t  valid;
    uint8_t  _pad[3];
} evasion_query_t;

typedef struct {
    uint32_t magic;
    uint32_t size;
    uint64_t capability[3];
    void   (*query)(uint32_t id, evasion_query_t *out);
    void   (*apply)(evasion_request_t *req);
    void   (*unknown)(void);
    uint64_t _pad;
} evasion_backend_t;

typedef struct {
    const char *name;
    void       *ctx;
    int       (*on_register)(void *);
    void      (*on_tick)(void *);
    void      (*on_unregister)(void *);
} evasion_periodic_callbacks_t;

static const nexus_property_t nexus_properties[144] = {
  { "isSpinEnabled",   0, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "cameraEnabled",   1, PROP_FEATURE, 0, 1, 0, COERCE_NONE, true },
  { "antiAfkEnabled",   2, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "autofarmEnabled",   3, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "autofarmAttackEnemies",   4, PROP_FEATURE, 0, 1, 1, COERCE_NONE, false },
  { "coltModEnabled",   5, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "koltModEnabled",   5, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "boltWallAvoidEnabled",   6, PROP_FEATURE, 0, 1, 1, COERCE_NONE, false },
  { "boltPredictionEnabled",   7, PROP_FEATURE, 0, 1, 1, COERCE_NONE, false },
  { "pinEnabled",   8, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "sprayEnabled",   9, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "isXrayEnabled",  10, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "xRayEnabled",  10, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "xrayEnabled",  10, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "xrayShowTargetName",  11, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "aopAimEnabled",  12, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "aopPredictEnabled",  13, PROP_FEATURE, 0, 1, 1, COERCE_NONE, false },
  { "aopDebugEnabled",  14, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "espEnabled",  15, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "attackRangeIndicator",  16, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "hitboxRenderer",  17, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "enemyTracer",  18, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "teammateHpIndicator",  19, PROP_FEATURE, 0, 1, 0, COERCE_NONE, true },
  { "trophiesAboveHead",  20, PROP_FEATURE, 0, 1, 1, COERCE_NONE, false },
  { "allyRespawnTimer",  21, PROP_FEATURE, 0, 1, 0, COERCE_NONE, true },
  { "tileGridOverlay",  22, PROP_FEATURE, 0, 1, 0, COERCE_NONE, true },
  { "smoothHudGraph",  23, PROP_FEATURE, 0, 1, 0, COERCE_NONE, true },
  { "smoothHud",  24, PROP_FEATURE, 0, 1, 0, COERCE_NONE, true },
  { "serverIPEnabled",  25, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "characterOutlineEnabled",  26, PROP_FEATURE, 0, 1, 1, COERCE_NONE, false },
  { "killauraEnabled",  27, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "killauraMainAttack",  28, PROP_FEATURE, 0, 1, 1, COERCE_NONE, false },
  { "killauraGadget",  29, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "killauraSuper",  30, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "killauraNoWall",  31, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "killauraNoBall",  32, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "ballAssistEnabled",  33, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "holdToShootEnabled",  34, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "holdToShootAim",  35, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "holdToShootRangeCheck",  36, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "combatDodgeRequireHold",  37, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "followEnabled",  38, PROP_FEATURE, 0, 1, 0, COERCE_FOLLOW_ALIAS, false },
  { "followAntiAfkEnabled",  38, PROP_FEATURE, 0, 1, 0, COERCE_FOLLOW_ALIAS, false },
  { "followClosestAllyEnabled",  38, PROP_FEATURE, 0, 1, 0, COERCE_FOLLOW_ALIAS, false },
  { "followModeEnemy",  39, PROP_FEATURE, 0, 1, 0, COERCE_FOLLOW_ALIAS, false },
  { "followModeTeam",  39, PROP_FEATURE, 0, 1, 0, COERCE_FOLLOW_ALIAS, false },
  { "speedExploitEnabled",  40, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "speedLocalMoveEnabled",  41, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "flyEnabled",  42, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "kitNaniModEnabled",  43, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "boltModEnabled",  44, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "boltAutoAttackEnabled",  45, PROP_FEATURE, 0, 1, 1, COERCE_NONE, false },
  { "boltSafeExitEnabled",  46, PROP_FEATURE, 0, 1, 1, COERCE_NONE, false },
  { "autododgeEnabled",  47, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "dynaJumpEnabled",  48, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "epsteinEnabled",  49, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "espShowNames",  50, PROP_FEATURE, 0, 1, 1, COERCE_NONE, false },
  { "espShowDistance",  51, PROP_FEATURE, 0, 1, 1, COERCE_NONE, false },
  { "espShowTracer",  52, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "espShowHitbox",  53, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "espShowCircle",  54, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "espShowAimLine",  55, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "espTeamFilterEnemies",  56, PROP_FEATURE, 0, 1, 1, COERCE_NONE, false },
  { "espTeamFilterSelf",  57, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "espTeamFilterTeammates",  58, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false },
  { "combatDodgeContinuous",  59, PROP_FEATURE, 0, 1, 1, COERCE_NONE, false },
  { "combatDodgeCircular",  60, PROP_FEATURE, 0, 1, 1, COERCE_NONE, false },
  { "combatDodgeAlways",  61, PROP_FEATURE, 0, 1, 1, COERCE_NONE, false },
  { "combatDodgeQuickAccess",  62, PROP_FEATURE, 0, 1, 1, COERCE_NONE, false },
  { "spinSpeed",  63, PROP_PARAM, 0, 62832, 785, COERCE_NONE, false },
  { "spinRadius",  64, PROP_PARAM, 0, 300, 25, COERCE_NONE, false },
  { "cameraMode",  65, PROP_PARAM, 0, 8, 3, COERCE_NONE, true },
  { "cameraZoom",  66, PROP_PARAM, 25, 400, 100, COERCE_NONE, true },
  { "cameraOffsetX",  67, PROP_PARAM, -2000, 2000, 0, COERCE_NONE, true },
  { "cameraOffsetY",  68, PROP_PARAM, -2000, 2000, 0, COERCE_NONE, true },
  { "followDistance",  69, PROP_PARAM, 100, 5000, 250, COERCE_NONE, false },
  { "pinIntervalMs",  70, PROP_PARAM, 100, 5000, 800, COERCE_NONE, false },
  { "sprayIntervalMs",  71, PROP_PARAM, 100, 5000, 600, COERCE_NONE, false },
  { "aopPredictVersion",  72, PROP_PARAM, 1, 2, 1, COERCE_NONE, false },
  { "aopProjectileSpeed",  73, PROP_PARAM, 500, 9000, 3200, COERCE_NONE, false },
  { "aopLeadScale",  74, PROP_PARAM, 0, 200, 100, COERCE_NONE, false },
  { "aopMaxRange",  75, PROP_PARAM, 500, 12000, 9000, COERCE_NONE, false },
  { "aopReactionMs",  76, PROP_PARAM, 0, 500, 0, COERCE_NONE, false },
  { "aopTargetMode",  77, PROP_PARAM, 0, 2, 0, COERCE_NONE, false },
  { "combatAuraRange",  78, PROP_PARAM, 500, 12000, 4200, COERCE_NONE, false },
  { "killauraDisableBelowHealthPercent",  79, PROP_PARAM, 0, 100, 0, COERCE_NONE, false },
  { "killauraDisableIfHealthIsBelow",  79, PROP_PARAM, 0, 100, 0, COERCE_NONE, false },
  { "autofarmFollowTarget",  80, PROP_PARAM, 0, 1, 0, COERCE_NONE, false },
  { "boltSmoothPercent",  81, PROP_PARAM, 50, 100, 85, COERCE_NONE, false },
  { "boltWallLookahead",  82, PROP_PARAM, 150, 600, 500, COERCE_NONE, false },
  { "boltTargetRange",  83, PROP_PARAM, 600, 2000, 1400, COERCE_NONE, false },
  { "boltExitHoldMs",  84, PROP_PARAM, 300, 1200, 650, COERCE_NONE, false },
  { "combatDodgeRange",  85, PROP_PARAM, 300, 3000, 900, COERCE_NONE, false },
  { "combatFireInterval",  86, PROP_PARAM, 50, 2000, 1000, COERCE_NONE, false },
  { "dodgePrecision",  87, PROP_PARAM, 8, 32, 24, COERCE_NONE, false },
  { "dodgeReactionPercent",  88, PROP_PARAM, 50, 180, 180, COERCE_NONE, false },
  { "dodgeSafetyPercent",  89, PROP_PARAM, 50, 180, 100, COERCE_NONE, false },
  { "dodgePowerPercent",  90, PROP_PARAM, 60, 140, 100, COERCE_NONE, false },
  { "dodgeDistancePercent",  91, PROP_PARAM, 70, 140, 100, COERCE_NONE, false },
  { "dodgeCommitPercent",  92, PROP_PARAM, 50, 150, 100, COERCE_NONE, false },
  { "dodgeBurstHoldMs",  93, PROP_PARAM, 300, 1600, 1060, COERCE_NONE, false },
  { "v2DodgeReactionPercent",  94, PROP_PARAM, 50, 180, 180, COERCE_NONE, false },
  { "v2DodgeSafetyPercent",  95, PROP_PARAM, 50, 180, 100, COERCE_NONE, false },
  { "v2DodgePowerPercent",  96, PROP_PARAM, 60, 140, 100, COERCE_NONE, false },
  { "v2DodgeDistancePercent",  97, PROP_PARAM, 70, 140, 100, COERCE_NONE, false },
  { "v2DodgeCommitPercent",  98, PROP_PARAM, 50, 150, 100, COERCE_NONE, false },
  { "v2DodgeBurstHoldMs",  99, PROP_PARAM, 300, 1600, 1060, COERCE_NONE, false },
  { "v3DodgeReactionPercent", 100, PROP_PARAM, 50, 180, 180, COERCE_NONE, false },
  { "v3DodgeSafetyPercent", 101, PROP_PARAM, 50, 180, 100, COERCE_NONE, false },
  { "v3DodgePowerPercent", 102, PROP_PARAM, 60, 140, 100, COERCE_NONE, false },
  { "v3DodgeDistancePercent", 103, PROP_PARAM, 70, 140, 100, COERCE_NONE, false },
  { "v3DodgeCommitPercent", 104, PROP_PARAM, 50, 150, 100, COERCE_NONE, false },
  { "v3DodgeBurstHoldMs", 105, PROP_PARAM, 300, 1600, 1060, COERCE_NONE, false },
  { "v4DodgeReactionPercent", 106, PROP_PARAM, 50, 180, 180, COERCE_NONE, false },
  { "v4DodgeSafetyPercent", 107, PROP_PARAM, 50, 180, 100, COERCE_NONE, false },
  { "v4DodgePowerPercent", 108, PROP_PARAM, 60, 140, 100, COERCE_NONE, false },
  { "v4DodgeDistancePercent", 109, PROP_PARAM, 70, 140, 100, COERCE_NONE, false },
  { "v4DodgeCommitPercent", 110, PROP_PARAM, 50, 150, 100, COERCE_NONE, false },
  { "v4DodgeBurstHoldMs", 111, PROP_PARAM, 300, 1600, 1060, COERCE_NONE, false },
  { "v5DodgeReactionPercent", 112, PROP_PARAM, 50, 180, 180, COERCE_NONE, false },
  { "v5DodgeSafetyPercent", 113, PROP_PARAM, 50, 180, 100, COERCE_NONE, false },
  { "v5DodgePowerPercent", 114, PROP_PARAM, 60, 140, 100, COERCE_NONE, false },
  { "v5DodgeDistancePercent", 115, PROP_PARAM, 70, 140, 100, COERCE_NONE, false },
  { "v5DodgeCommitPercent", 116, PROP_PARAM, 50, 150, 100, COERCE_NONE, false },
  { "v5DodgeBurstHoldMs", 117, PROP_PARAM, 300, 1600, 1060, COERCE_NONE, false },
  { "dodgeBlacklistMask", 118, PROP_PARAM, 0, 1023, 1023, COERCE_NONE, false },
  { "holdToShootDelayMs", 119, PROP_PARAM, 0, 1000, 50, COERCE_NONE, false },
  { "ballAssistSearchDegrees", 120, PROP_PARAM, 5, 180, 90, COERCE_NONE, false },
  { "ballAssistBounceLimit", 121, PROP_PARAM, 0, 8, 3, COERCE_NONE, false },
  { "outlineOpacity", 122, PROP_PARAM, 0, 1000, 1000, COERCE_NONE, false },
  { "outlineColorR", 123, PROP_PARAM, 0, 1000, 1000, COERCE_NONE, false },
  { "outlineColorG", 124, PROP_PARAM, 0, 1000, 1000, COERCE_NONE, false },
  { "outlineColorB", 125, PROP_PARAM, 0, 1000, 1000, COERCE_NONE, false },
  { "dodgeVersion", 126, PROP_PARAM, 1, 5, 3, COERCE_DODGE_VERSION, false },
  { "speedLevel", 127, PROP_PARAM, 4, 4, 4, COERCE_SPEED_LEVEL, false },
  { "aopAimTargetMode", 128, PROP_PARAM, 1, 2, 1, COERCE_TARGET_MODE, false },
  { "aimTargetMode", 128, PROP_PARAM, 1, 2, 1, COERCE_TARGET_MODE, false },
  { "xrayTargetMode", 129, PROP_PARAM, 1, 2, 1, COERCE_TARGET_MODE, false },
  { "spinMovementMode", 130, PROP_PARAM, 0, 1, 0, COERCE_NONE, false },
  { "spinOnlineOnlyIdle", 131, PROP_PARAM, 0, 1, 0, COERCE_NONE, false },
  { "aopAimUltimate", 132, PROP_PARAM, 0, 1, 1, COERCE_NONE, false },
  { "aopAimGadget", 133, PROP_PARAM, 0, 1, 1, COERCE_NONE, false },
  { "aopAimEnabled",   0, PROP_FEATURE, 0, 0, 0, COERCE_NONE, false },
  { "coltModEnabled",   0, PROP_FEATURE, 0, 0, 0, COERCE_NONE, false },
};

#define EV_PROPERTY_WALK_COUNT 142

static const evasion_action_t evasion_actions[168] = {
  { NULL, false, 0, -1 },
  { NULL, false, 1, -1 },
  { NULL, false, 1, -1 },
  { NULL, false, 1, -1 },
  { NULL, false, 1, -1 },
  { NULL, false, 1, -1 },
  { NULL, false, 1, -1 },
  { NULL, false, 1, -1 },
  { NULL, false, 1, -1 },
  { NULL, false, 1, -1 },
  { NULL, false, 1, -1 },
  { NULL, false, 1, -1 },
  { NULL, false, 1, -1 },
  { NULL, false, 1, -1 },
  { NULL, false, 1, -1 },
  { NULL, false, 1, -1 },
  { NULL, false, 1, -1 },
  { "killauraEnabled", false, 2, -1 },
  { "aopPredictEnabled", false, 2, -1 },
  { "killauraMainAttack", false, 2, -1 },
  { "killauraNoWall", false, 2, -1 },
  { "killauraNoBall", false, 2, -1 },
  { "autododgeEnabled", false, 2, -1 },
  { "aopAimEnabled", false, 2, -1 },
  { "aopPredictEnabled", false, 2, -1 },
  { "isSpinEnabled", false, 2, -1 },
  { "followEnabled", false, 2, -1 },
  { "followClosestAllyEnabled", false, 2, -1 },
  { "ballAssistEnabled", false, 2, -1 },
  { "holdToShootEnabled", false, 2, -1 },
  { "isXrayEnabled", false, 2, -1 },
  { "espEnabled", false, 2, -1 },
  { "characterOutlineEnabled", false, 2, -1 },
  { "cameraEnabled", false, 2, -1 },
  { "attackRangeIndicator", false, 2, -1 },
  { "hitboxRenderer", false, 2, -1 },
  { "enemyTracer", false, 2, -1 },
  { "teammateHpIndicator", false, 2, -1 },
  { "trophiesAboveHead", false, 2, -1 },
  { "allyRespawnTimer", false, 2, -1 },
  { "tileGridOverlay", false, 2, -1 },
  { "smoothHudGraph", false, 2, -1 },
  { "smoothHud", false, 2, -1 },
  { "pinEnabled", false, 2, -1 },
  { "sprayEnabled", false, 2, -1 },
  { "antiAfkEnabled", false, 2, -1 },
  { "speedExploitEnabled", false, 2, -1 },
  { NULL, false, 2, -1 },
  { NULL, false, 0, -1 },
  { NULL, false, 0, -1 },
  { NULL, false, 0, -1 },
  { NULL, false, 0, -1 },
  { NULL, false, 0, -1 },
  { NULL, false, 0, -1 },
  { NULL, false, 0, -1 },
  { "aopTargetMode", true, 4, -1 },
  { "combatAuraRange", true, 3, -1 },
  { "combatAuraRange", true, 3, -1 },
  { "combatFireInterval", true, 3, -1 },
  { "combatFireInterval", true, 3, -1 },
  { "aopReactionMs", true, 3, -1 },
  { "aopReactionMs", true, 3, -1 },
  { "aopLeadScale", true, 3, -1 },
  { "aopLeadScale", true, 3, -1 },
  { NULL, false, 0, -1 },
  { NULL, false, 0, -1 },
  { "aopTargetMode", true, 4, -1 },
  { NULL, false, 1, -1 },
  { "autofarmEnabled", false, 2, -1 },
  { NULL, false, 3, -1 },
  { NULL, false, 4, -1 },
  { NULL, false, 3, -1 },
  { NULL, false, 4, -1 },
  { "autofarmAttackEnemies", false, 2, -1 },
  { NULL, false, 2, -1 },
  { "followDistance", true, 3, -1 },
  { "followDistance", true, 3, -1 },
  { "killauraDisableBelowHealthPercent", true, 3, -1 },
  { "killauraDisableBelowHealthPercent", true, 3, -1 },
  { "dodgeReactionPercent", true, 3, -1 },
  { "dodgeReactionPercent", true, 3, -1 },
  { "dodgeSafetyPercent", true, 3, -1 },
  { "dodgeSafetyPercent", true, 3, -1 },
  { "dodgePowerPercent", true, 3, -1 },
  { "dodgePowerPercent", true, 3, -1 },
  { "dodgeDistancePercent", true, 3, -1 },
  { "dodgeDistancePercent", true, 3, -1 },
  { "dodgeCommitPercent", true, 3, -1 },
  { "dodgeCommitPercent", true, 3, -1 },
  { "dodgeBurstHoldMs", true, 3, -1 },
  { "dodgeBurstHoldMs", true, 3, -1 },
  { "dodgeVersion", true, 4, -1 },
  { NULL, false, 0, -1 },
  { NULL, false, 1, -1 },
  { "combatDodgeRequireHold", false, 2, -1 },
  { "dodgeBlacklistMask", true, 2, 0 },
  { "dodgeBlacklistMask", true, 2, 1 },
  { "dodgeBlacklistMask", true, 2, 2 },
  { "dodgeBlacklistMask", true, 2, 3 },
  { "dodgeBlacklistMask", true, 2, 4 },
  { "dodgeBlacklistMask", true, 2, 5 },
  { "dodgeBlacklistMask", true, 2, 6 },
  { "dodgeBlacklistMask", true, 2, 7 },
  { "dodgeBlacklistMask", true, 2, 8 },
  { "dodgeBlacklistMask", true, 2, 9 },
  { "xrayShowTargetName", false, 2, -1 },
  { "xrayTargetMode", true, 4, -1 },
  { NULL, false, 0, -1 },
  { NULL, false, 2, -1 },
  { NULL, false, 1, -1 },
  { "outlineOpacity", true, 3, -1 },
  { "outlineOpacity", true, 3, -1 },
  { NULL, false, 0, -1 },
  { NULL, false, 1, -1 },
  { NULL, false, 5, -1 },
  { NULL, false, 7, -1 },
  { NULL, false, 7, -1 },
  { NULL, false, 6, -1 },
  { NULL, false, 6, -1 },
  { NULL, false, 6, -1 },
  { NULL, false, 6, -1 },
  { "kitNaniModEnabled", false, 2, -1 },
  { "coltModEnabled", false, 2, -1 },
  { NULL, false, 1, -1 },
  { NULL, false, 1, -1 },
  { NULL, false, 0, -1 },
  { NULL, false, 2, -1 },
  { NULL, false, 0, -1 },
  { NULL, false, 0, -1 },
  { NULL, false, 1, -1 },
  { NULL, false, 0, -1 },
  { NULL, false, 1, -1 },
  { "boltModEnabled", false, 2, -1 },
  { "boltAutoAttackEnabled", false, 2, -1 },
  { "boltWallAvoidEnabled", false, 2, -1 },
  { "boltPredictionEnabled", false, 2, -1 },
  { "boltSafeExitEnabled", false, 2, -1 },
  { "boltSmoothPercent", true, 3, -1 },
  { "boltSmoothPercent", true, 3, -1 },
  { "boltWallLookahead", true, 3, -1 },
  { "boltWallLookahead", true, 3, -1 },
  { "boltTargetRange", true, 3, -1 },
  { "boltTargetRange", true, 3, -1 },
  { "boltExitHoldMs", true, 3, -1 },
  { "boltExitHoldMs", true, 3, -1 },
  { NULL, false, 0, -1 },
  { "aopAimTargetMode", true, 4, -1 },
  { NULL, false, 1, -1 },
  { NULL, false, 0, -1 },
  { NULL, false, 0, -1 },
  { NULL, false, 0, -1 },
  { NULL, false, 0, -1 },
  { "spinSpeed", true, 3, -1 },
  { "spinSpeed", true, 3, -1 },
  { "dynaJumpEnabled", false, 2, -1 },
  { "aopPredictVersion", true, 4, -1 },
  { "holdToShootAim", false, 2, -1 },
  { NULL, false, 0, -1 },
  { NULL, false, 0, -1 },
  { NULL, false, 0, -1 },
  { NULL, false, 0, -1 },
  { NULL, false, 0, -1 },
  { "speedLocalMoveEnabled", false, 2, -1 },
  { "flyEnabled", false, 2, -1 },
  { "spinMovementMode", true, 2, -1 },
  { "spinOnlineOnlyIdle", true, 2, -1 },
  { "aopAimUltimate", true, 2, -1 },
  { "aopAimGadget", true, 2, -1 },
};

static const evasion_query_slot_t evasion_query_slots[118] = {
  { "isSpinEnabled", PROP_FEATURE },
  { "followEnabled", PROP_FEATURE },
  { "followModeTeam", PROP_FEATURE },
  { "pinEnabled", PROP_FEATURE },
  { "sprayEnabled", PROP_FEATURE },
  { "antiAfkEnabled", PROP_FEATURE },
  { "speedExploitEnabled", PROP_FEATURE },
  { "speedLocalMoveEnabled", PROP_FEATURE },
  { "flyEnabled", PROP_FEATURE },
  { "kitNaniModEnabled", PROP_FEATURE },
  { "coltModEnabled", PROP_FEATURE },
  { "boltModEnabled", PROP_FEATURE },
  { "boltAutoAttackEnabled", PROP_FEATURE },
  { "boltWallAvoidEnabled", PROP_FEATURE },
  { "boltPredictionEnabled", PROP_FEATURE },
  { "boltSafeExitEnabled", PROP_FEATURE },
  { NULL, PROP_FEATURE },
  { "isXrayEnabled", PROP_FEATURE },
  { "xrayShowTargetName", PROP_FEATURE },
  { "aopAimEnabled", PROP_FEATURE },
  { "aopPredictEnabled", PROP_FEATURE },
  { "espEnabled", PROP_FEATURE },
  { "characterOutlineEnabled", PROP_FEATURE },
  { "attackRangeIndicator", PROP_FEATURE },
  { "hitboxRenderer", PROP_FEATURE },
  { "enemyTracer", PROP_FEATURE },
  { "trophiesAboveHead", PROP_FEATURE },
  { "serverIPEnabled", PROP_FEATURE },
  { "killauraEnabled", PROP_FEATURE },
  { "killauraMainAttack", PROP_FEATURE },
  { "killauraGadget", PROP_FEATURE },
  { "killauraSuper", PROP_FEATURE },
  { "killauraNoWall", PROP_FEATURE },
  { "killauraNoBall", PROP_FEATURE },
  { "autododgeEnabled", PROP_FEATURE },
  { "combatDodgeRequireHold", PROP_FEATURE },
  { "autofarmEnabled", PROP_FEATURE },
  { NULL, PROP_FEATURE },
  { "autofarmAttackEnemies", PROP_FEATURE },
  { NULL, PROP_FEATURE },
  { NULL, PROP_FEATURE },
  { "ballAssistEnabled", PROP_FEATURE },
  { "holdToShootEnabled", PROP_FEATURE },
  { "dynaJumpEnabled", PROP_FEATURE },
  { "holdToShootAim", PROP_FEATURE },
  { "holdToShootRangeCheck", PROP_FEATURE },
  { "spinSpeed", PROP_PARAM },
  { "spinRadius", PROP_PARAM },
  { "followDistance", PROP_PARAM },
  { "pinIntervalMs", PROP_PARAM },
  { "sprayIntervalMs", PROP_PARAM },
  { "xrayTargetMode", PROP_PARAM },
  { "aopAimTargetMode", PROP_PARAM },
  { "aopPredictVersion", PROP_PARAM },
  { "aopProjectileSpeed", PROP_PARAM },
  { "aopLeadScale", PROP_PARAM },
  { "aopMaxRange", PROP_PARAM },
  { "aopReactionMs", PROP_PARAM },
  { "aopTargetMode", PROP_PARAM },
  { "combatAuraRange", PROP_PARAM },
  { "combatDodgeRange", PROP_PARAM },
  { "combatFireInterval", PROP_PARAM },
  { "killauraDisableBelowHealthPercent", PROP_PARAM },
  { "dodgeReactionPercent", PROP_PARAM },
  { "dodgeSafetyPercent", PROP_PARAM },
  { "dodgePowerPercent", PROP_PARAM },
  { "dodgeDistancePercent", PROP_PARAM },
  { "dodgeCommitPercent", PROP_PARAM },
  { "dodgeBurstHoldMs", PROP_PARAM },
  { "v2DodgeReactionPercent", PROP_PARAM },
  { "v2DodgeSafetyPercent", PROP_PARAM },
  { "v2DodgePowerPercent", PROP_PARAM },
  { "v2DodgeDistancePercent", PROP_PARAM },
  { "v2DodgeCommitPercent", PROP_PARAM },
  { "v2DodgeBurstHoldMs", PROP_PARAM },
  { "v3DodgeReactionPercent", PROP_PARAM },
  { "v3DodgeSafetyPercent", PROP_PARAM },
  { "v3DodgePowerPercent", PROP_PARAM },
  { "v3DodgeDistancePercent", PROP_PARAM },
  { "v3DodgeCommitPercent", PROP_PARAM },
  { "v3DodgeBurstHoldMs", PROP_PARAM },
  { "v4DodgeReactionPercent", PROP_PARAM },
  { "v4DodgeSafetyPercent", PROP_PARAM },
  { "v4DodgePowerPercent", PROP_PARAM },
  { "v4DodgeDistancePercent", PROP_PARAM },
  { "v4DodgeCommitPercent", PROP_PARAM },
  { "v4DodgeBurstHoldMs", PROP_PARAM },
  { "v5DodgeReactionPercent", PROP_PARAM },
  { "v5DodgeSafetyPercent", PROP_PARAM },
  { "v5DodgePowerPercent", PROP_PARAM },
  { "v5DodgeDistancePercent", PROP_PARAM },
  { "v5DodgeCommitPercent", PROP_PARAM },
  { "v5DodgeBurstHoldMs", PROP_PARAM },
  { "dodgeBlacklistMask", PROP_PARAM },
  { "dodgeVersion", PROP_PARAM },
  { "holdToShootDelayMs", PROP_PARAM },
  { "ballAssistSearchDegrees", PROP_PARAM },
  { "ballAssistBounceLimit", PROP_PARAM },
  { NULL, PROP_FEATURE },
  { NULL, PROP_FEATURE },
  { "autofarmFollowTarget", PROP_PARAM },
  { "boltSmoothPercent", PROP_PARAM },
  { "boltWallLookahead", PROP_PARAM },
  { "boltTargetRange", PROP_PARAM },
  { "boltExitHoldMs", PROP_PARAM },
  { "outlineOpacity", PROP_PARAM },
  { "outlineColorR", PROP_PARAM },
  { "outlineColorG", PROP_PARAM },
  { "outlineColorB", PROP_PARAM },
  { NULL, PROP_FEATURE },
  { NULL, PROP_FEATURE },
  { NULL, PROP_FEATURE },
  { NULL, PROP_FEATURE },
  { NULL, PROP_FEATURE },
  { "spinMovementMode", PROP_PARAM },
  { "spinOnlineOnlyIdle", PROP_PARAM },
  { "aopAimUltimate", PROP_PARAM },
  { "aopAimGadget", PROP_PARAM },
};

static uint32_t        state[EV_STATE_COUNT];
static uint32_t        state_revision;
static volatile int    state_lock;

static void backend_query(uint32_t id, evasion_query_t *out);
static void backend_apply(evasion_request_t *req);
static int  activation_gates_pass(const char *name);

static const nexus_property_t *find_property(const char *name);
static void  state_lock_acquire(void);
static void  state_lock_release(void);

static int32_t clamp_u32(int32_t v, int32_t lo, uint32_t hi)
{
    if (v < lo)  return lo;
    if ((uint32_t)v > hi) return (int32_t)hi;
    return v;
}

int evasion_set(const char *name, uint32_t value, int is_parameter)
{
    if (name == NULL)
        return EV_UNKNOWN_KEY;

    size_t len = strlen(name);
    if (len == 0 || len >= EV_MAX_NAME)
        return EV_UNKNOWN_KEY;

    const nexus_property_t *prop = find_property(name);
    if (prop == NULL)
        return EV_UNKNOWN_KEY;
    if (prop->kind != (is_parameter ? PROP_PARAM : PROP_FEATURE))
        return EV_UNKNOWN_KEY;

    uint32_t v = (strcmp(name, "combatDodgeRequireHold") == 0) ? 0 : value;

    if (v != 0 || is_parameter) {
        if (!is_parameter) {
            if (!evasion_is_free_scope(name) && !evasion_entitlement_check())
                return EV_REJECTED;
        } else {

            static const char *exempt_prefixes[] = {
                "outlineOpacity", "outlineColorR", "outlineColorG", "outlineColorB",
                "aop", "killaura", "combatAura", "combatFire", "combatDodge",
                "dodge", "v2Dodge", "v3Dodge", "v4Dodge", "v5Dodge", "holdToShoot",
            };
            int exempt = 0;
            for (size_t i = 0; i < sizeof exempt_prefixes / sizeof *exempt_prefixes; i++)
                if (strncmp(name, exempt_prefixes[i], strlen(exempt_prefixes[i])) == 0)
                    { exempt = 1; break; }
            if (!exempt && !evasion_entitlement_check())
                return EV_REJECTED;
        }
    }

    state_lock_acquire();
    evasion_commit();

    if (!is_parameter && v != 0 && !activation_gates_pass(name)) {
        state_lock_release();
        return EV_REJECTED;
    }

    uint32_t coerced;
    if (is_parameter) {
        coerced = (uint32_t)clamp_u32((int32_t)v, prop->vmin, prop->vmax);
    } else {
        coerced = (v != 0) ? 1u : 0u;
    }
    switch (prop->coerce) {
    case COERCE_DODGE_VERSION:
        coerced = (v >= 1 && v <= 5) ? v : 3;
        break;
    case COERCE_SPEED_LEVEL:
        coerced = 4;
        break;
    case COERCE_TARGET_MODE:
        coerced = (v == 2) ? 2 : 1;
        break;
    default:
        break;
    }

    if (prop->coerce == COERCE_FOLLOW_ALIAS) {

        const nexus_property_t *sib = find_property(
            strcmp(name, "followModeTeam") == 0 ? "followModeEnemy" : "followModeTeam");
        if (sib && state[sib->state_index] != coerced) {
            state[sib->state_index] = coerced;
            state_revision++;
        }
    }

    static const char *mex_pairs[2][2] = {
        { "speedExploitEnabled",   "speedLocalMoveEnabled" },
        { "speedLocalMoveEnabled", "speedExploitEnabled"   },
    };
    for (int i = 0; i < 2; i++) {
        if (strcmp(name, mex_pairs[i][0]) == 0 && coerced != 0) {
            const nexus_property_t *other = find_property(mex_pairs[i][1]);
            if (other && state[other->state_index] != 0) {
                state[other->state_index] = 0;
                state_revision++;
            }
        }
    }

    if (state[prop->state_index] != coerced) {
        state[prop->state_index] = coerced;
        state_revision++;
    }
    state_lock_release();

    if (!is_parameter)
        return EV_APPLIED;
    return (coerced != 0 && prop->active) ? EV_APPLIED_ACTIVE : EV_APPLIED;
}

int nexus_evasion_set_feature(const char *name, uint32_t value)
{
    return evasion_set(name, value, 0);
}

int nexus_evasion_set_parameter(const char *name, uint32_t value)
{
    return evasion_set(name, value, 1);
}

static int activation_gates_pass(const char *name)
{

    if (evasion_key_is_visual_only(name))
        return 1;

    if (evasion_key_adapter_family(name) == 0) {

        if (strcmp(name, "pinEnabled") == 0 || strcmp(name, "sprayEnabled") == 0)
            return periodic_adapter_ready(  0,   0);

        if (evasion_key_is_aim_family(name))
            return adapter_family_ready(  0,   0);

        if (strcmp(name, "speedExploitEnabled") == 0 ||
            strcmp(name, "speedLocalMoveEnabled") == 0)
            return 1;

        if (strcmp(name, "holdToShootEnabled") == 0 ||
            strcmp(name, "holdToShootAim") == 0 ||
            strcmp(name, "holdToShootRangeCheck") == 0)
            return evasion_hold_family_ok(name);

        if (strcmp(name, "isSpinEnabled") == 0)
            return evasion_spin_gate();

        int idx = evasion_brawler_gate_index(name);
        if (idx >= 0)
            return evasion_brawler_gate_set(name, 1);
        int bit = evasion_brawler_bitmap_index(name);
        if (bit >= 0)
            return (evasion_brawler_bitmap() >> (bit & 31)) & 1;

        return evasion_prediction_alias_ok(name);
    }

    return adapter_family_ready(  1,   1)
        && (evasion_is_free_scope(name) || evasion_entitlement_check());
}

static const char *resolve_action_key(const evasion_action_t *act,
                                      uint32_t dodge_version, char *buf, size_t bufsz);

int nexus_evasion_menu_backend_v1(evasion_backend_t *out)
{
    if (out == NULL || out->magic != 1 || out->size != EVASION_BACKEND_SIZE)
        return BK_REJECTED;

    uint32_t magic = out->magic, size = out->size;
    memset(out, 0, sizeof *out);
    out->magic = magic;
    out->size  = size;

    for (int slot = 0; slot < 168; slot++)
        if (evasion_actions[slot].name != NULL)
            out->capability[slot >> 6] |= 1ULL << (slot & 63);

    out->query   = backend_query;
    out->apply   = backend_apply;
    out->unknown = backend_misc;
    return BK_OK;
}

static void backend_apply(evasion_request_t *req)
{
    if (req == NULL || req->magic != EVASION_REQUEST_MAGIC)
        return;
    if (req->slot > 167)
        return;
    if (req->cookie != 0)
        return;

    const evasion_action_t *act = &evasion_actions[req->slot];
    if (act->name == NULL)
        return;
    if (act->token != req->token)
        return;

    if (evasion_get_port_state(NULL) == PORT_DETACHED)
        {   return; }

    char keybuf[96];
    const char *key = resolve_action_key(act, req->arg_e, keybuf, sizeof keybuf);

    int32_t r = act->is_parameter
        ? nexus_evasion_set_parameter(key, (uint32_t)req->value)
        : nexus_evasion_set_feature(key, (uint32_t)req->value);

    (void)r;
}

static void backend_query(uint32_t id, evasion_query_t *out)
{
    if (out == NULL || out->magic != EVASION_QUERY_SIZE)
        { out->status = BK_REJECTED; return; }

    memset(out, 0, sizeof *out);
    out->magic = EVASION_QUERY_SIZE;

    if (id >= 0x10000 && id < 0x10000 + 118) {

        const evasion_query_slot_t *qs = &evasion_query_slots[id - 0x10000];
        if (qs->name == NULL)
            return;
        int32_t req = evasion_get_requested(qs->name);
        if (req == INT32_MIN)
            return;
        out->requested = req;
        out->value    = req;
        out->effective = (qs->kind == PROP_FEATURE)
                          ? evasion_get_effective(qs->name) : req;
        int port = evasion_get_port_state(qs->name);
        out->valid  = (port != PORT_DETACHED);
        out->status = (port == PORT_DETACHED) ? 3
                    : ((qs->kind == PROP_FEATURE && port != PORT_ACTIVE_2
                        && port != PORT_ACTIVE_1) ? 0 : 1);
        return;
    }

    if (id <= 167) {

        const evasion_action_t *act = &evasion_actions[id];
        if (act->name == NULL)
            return;
        char keybuf[96];
        int32_t dv = evasion_get_requested("dodgeVersion");
        const char *key = resolve_action_key(act, (uint32_t)dv, keybuf, sizeof keybuf);
        int32_t req = evasion_get_requested(key);
        if (req == INT32_MIN)
            return;

        if (act->blacklist_bit >= 0)
            req = (req >> (act->blacklist_bit & 31)) & 1;
        out->requested = req;
        out->value    = req;
        out->effective = (act->is_parameter == false)
                          ? evasion_get_effective(key) : req;
        int port = evasion_get_port_state(key);
        out->valid  = (port != PORT_DETACHED);
        out->status = (port == PORT_DETACHED) ? 3
                    : ((act->is_parameter == false && port != PORT_ACTIVE_2
                        && port != PORT_ACTIVE_1) ? 0 : 1);
        return;
    }

}

static const char *resolve_action_key(const evasion_action_t *act,
                                      uint32_t dodge_version, char *buf, size_t bufsz)
{
    const char *name = act->name;
    if (name == NULL || !act->is_parameter)
        return name;
    if (strncmp(name, "dodge", 5) == 0
        && strcmp(name, "dodgeVersion") != 0
        && strcmp(name, "dodgeBlacklistMask") != 0
        && strcmp(name, "dodgePrecision") != 0
        && dodge_version >= 2 && dodge_version <= 5) {
        snprintf(buf, bufsz, "v%uDodge%s", dodge_version, name + 5);
        return buf;
    }
    return name;
}

int nexus_evasion_intercept(float px, float py, float vx, float vy, float s, float *t_out)
{
    if (t_out == NULL || !(s > 0.0f) || isinff(s) || isnanf(s))
        return 0;
    const float in[4] = { px, py, vx, vy };
    for (int i = 0; i < 4; i++)
        if (isinff(in[i]) || isnanf(in[i]))
            return 0;

    float pp = fmaf(px, px, py * py);
    if (pp < 1e-4f)
        { *t_out = 0.0f; return 1; }

    float vv = fmaf(vx, vx, vy * vy);
    float bp = fmaf(px, vx, py * vy);
    float a  = fmaf(s, s, -vv);

    float t;
    if (a >= -5e-4f && a <= 5e-4f) {
        if (bp >= -5e-4f)
            return 0;
        t = -pp / (bp + bp);
    } else {
        float disc = fmaf(bp, bp, -a * pp);
        if (disc < 0.0f)
            return 0;
        float sq = sqrtf(disc);
        float t0 = (-bp - sq) / (a + a);
        float t1 = (-bp + sq) / (a + a);

        t = (t1 <= t0) ? t1 : t0;
        if (t1 < 0.0f || t1 == 0.0f) t = t0;
        if (t0 < 0.0f || t0 == 0.0f) t = t1;
    }

    if (!(t >= 0.0f && t < 1.55f))
        return 0;
    if (isinff(t) || isnanf(t))
        return 0;
    *t_out = t;
    return 1;
}

int nexus_evasion_segment_distance(float px, float py, float ax, float ay,
                                   float bx, float by,
                                   float *dist, float *cx, float *cy, float *t_ab)
{
    const float in[6] = { px, py, ax, ay, bx, by };
    for (int i = 0; i < 6; i++)
        if (isinff(in[i]) || isnanf(in[i]))
            return 0;
    if (dist == NULL)
        return 0;

    float dx = bx - ax, dy = by - ay;
    float dd = fmaf(dx, dx, dy * dy);
    float t  = 0.0f;
    if (dd > 1e-6f) {
        t = fmaf(px - ax, dx, (py - ay) * dy) / dd;
        if (t < 0.0f) t = 0.0f;
        if (t > 1.0f) t = 1.0f;
    }
    float qx = fmaf(dy, t, ay);
    float qy = fmaf(dx, t, ax);
    float ex = px - qx, ey = py - qy;
    float d2 = fmaf(ex, ex, ey * ey);
    float d  = sqrtf(d2);
    if (isinff(d) || isnanf(d))
        return 0;
    *dist = d;
    if (cx)   *cx = qx;
    if (cy)   *cy = qy;
    if (t_ab) *t_ab = t;
    return 1;
}

#ifdef NEXUS_WITH_JNI
#include <jni.h>

static jint jni_setFeature(JNIEnv *env, jclass cls, jstring name, jint value)
{
    (void)cls;
    if (name == NULL) return EV_UNKNOWN_KEY;
    const char *utf = (*env)->GetStringUTFChars(env, name, NULL);
    if (utf == NULL) return -2;
    jint r = nexus_evasion_set_feature(utf, (uint32_t)value);
    (*env)->ReleaseStringUTFChars(env, name, utf);
    return r;
}

static jint jni_setParameter(JNIEnv *env, jclass cls, jstring name, jint value)
{
    (void)cls;
    if (name == NULL) return EV_UNKNOWN_KEY;
    const char *utf = (*env)->GetStringUTFChars(env, name, NULL);
    if (utf == NULL) return -2;
    jint r = nexus_evasion_set_parameter(utf, (uint32_t)value);
    (*env)->ReleaseStringUTFChars(env, name, utf);
    return r;
}

static jint jni_getRequested(JNIEnv *env, jclass cls, jstring name)
{
    (void)cls;
    if (name == NULL) return INT32_MIN;
    const char *utf = (*env)->GetStringUTFChars(env, name, NULL);
    if (utf == NULL) return INT32_MIN;
    jint r = evasion_get_requested(utf);
    (*env)->ReleaseStringUTFChars(env, name, utf);
    return r;
}

static jint jni_getEffective(JNIEnv *env, jclass cls, jstring name)
{
    (void)cls;
    if (name == NULL) return INT32_MIN;
    const char *utf = (*env)->GetStringUTFChars(env, name, NULL);
    if (utf == NULL) return INT32_MIN;
    jint r = evasion_get_effective(utf);
    (*env)->ReleaseStringUTFChars(env, name, utf);
    return r;
}

static jint jni_getPortState(JNIEnv *env, jclass cls, jstring name)
{
    (void)cls;
    if (name == NULL) return EV_UNKNOWN_KEY;
    const char *utf = (*env)->GetStringUTFChars(env, name, NULL);
    if (utf == NULL) return EV_UNKNOWN_KEY;
    jint r = evasion_get_port_state(utf);
    (*env)->ReleaseStringUTFChars(env, name, utf);
    return r;
}

static jstring jni_getDiagnostics(JNIEnv *env, jclass cls)
{
    (void)cls;
    char buf[1024];
    nexus_evasion_diagnostics(buf, sizeof buf);
    return (*env)->NewStringUTF(env, buf);
}

static jint jni_initNativeHooks(JNIEnv *env, jclass cls)
{
    (void)env; (void)cls;
    nexus_evasion_initialize();
    return 2;
}

static const JNINativeMethod facade_methods[] = {
    { "initNativeHooks",          "()I",                                   jni_initNativeHooks },
    { "setEvasionVersion",        "(Ljava/lang/String;II)I",               NULL   },
    { "setNativeCrashDownloadFd", "(Ljava/lang/String;II)I",               NULL   },
    { "setNativeCrashPrivateFd",  "(Ljava/lang/String;II)I",               NULL   },
};

static const JNINativeMethod backend_methods[] = {
    { "setFeature",     "(Ljava/lang/String;I)I",  jni_setFeature     },
    { "setParameter",   "(Ljava/lang/String;I)I",  jni_setParameter   },
    { "getRequested",   "(Ljava/lang/String;)I",   jni_getRequested   },
    { "getEffective",   "(Ljava/lang/String;)I",   jni_getEffective   },
    { "getPortState",   "(Ljava/lang/String;)I",   jni_getPortState   },
    { "getDiagnostics", "()Ljava/lang/String;",    jni_getDiagnostics },
};

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    (void)reserved;
    JNIEnv *env = NULL;
    if ((*vm)->GetEnv(vm, (void **)&env, JNI_VERSION_1_6) != JNI_OK || env == NULL)
        return JNI_ERR;

    jclass facade = (*env)->FindClass(env, "nexus/brawl/evasion/AdvancedEvasionController");
    if (facade == NULL)
        return JNI_ERR;
    jclass api = (*env)->FindClass(env, "nexus/evasion/EvasionBackend");
    if (api == NULL)
        return JNI_ERR;

    if ((*env)->RegisterNatives(env, facade, facade_methods,
                                sizeof facade_methods / sizeof *facade_methods) != 0)
        return JNI_ERR;
    if ((*env)->RegisterNatives(env, api, backend_methods,
                                sizeof backend_methods / sizeof *backend_methods) != 0)
        return JNI_ERR;

    (*env)->DeleteLocalRef(env, facade);
    (*env)->DeleteLocalRef(env, api);
    nexus_evasion_initialize();
    return JNI_VERSION_1_6;
}
#endif

extern void *dlsym_lookup(void *handle, const char *symbol);
extern int   dladdr_same_module(const void *addr);
extern void  periodic_bind_start(void);
extern void  log_periodic_state(const char *state, const char *detail, const char *extra);
extern int   periodic_ready;
#define PERIODIC_NAME         ((const char *)0x1e0978)
#define PERIODIC_CTX          ((void *)0x209d00)
extern int   pin_spray_on_register(void *ctx);
extern void  pin_spray_on_tick(void *ctx);
extern void  pin_spray_on_unregister(void *ctx);

void evasion_bind_periodic(void *evasion_module_handle)
{

    int (*register_periodic)(const void *) = (void *)dlsym_lookup(
        evasion_module_handle, "nexus_evasion_register_periodic_v1");
    void *(*snapshot_keys)(void) = (void *)dlsym_lookup(
        evasion_module_handle, "nexus_evasion_snapshot_keys_v1");

    if (register_periodic == NULL || snapshot_keys == NULL)
        return;
    if (!dladdr_same_module(register_periodic) || !dladdr_same_module(snapshot_keys))
        return;

    periodic_bind_start();

    evasion_periodic_callbacks_t cb = {
        .name         = PERIODIC_NAME,
        .ctx          = PERIODIC_CTX,
        .on_register  = pin_spray_on_register,
        .on_tick      = pin_spray_on_tick,
        .on_unregister= pin_spray_on_unregister,
    };
    if (register_periodic(&cb) != 1) {
        periodic_ready = 0;
        if (register_periodic) register_periodic(NULL);
        log_periodic_state("periodic_refused",
                           "optional_periodic_adapter_refused",
                           ",\"types\":[9,15],\"new_hooks\":0,\"native_calls_during_bind\":0");
        return;
    }
    periodic_ready = 1;
    log_periodic_state("periodic_ready",
                       "pin_and_spray_native_commands_bound",
                       ",\"types\":[9,15],\"new_hooks\":0,\"native_calls_during_bind\":0");
}

static const nexus_property_t *find_property(const char *name)
{
    for (int i = 0; i < EV_PROPERTY_WALK_COUNT; i++)
        if (strcmp(nexus_properties[i].name, name) == 0)
            return &nexus_properties[i];
    return NULL;
}

int32_t evasion_get_requested(const char *name)
{
    const nexus_property_t *p = find_property(name);
    return p ? (int32_t)state[p->state_index] : INT32_MIN;
}

static void state_lock_acquire(void)
{
    int expected;
    do {
        expected = __atomic_load_n(&state_lock, __ATOMIC_RELAXED);
    } while (expected != 0 ||
             !__atomic_compare_exchange_n(&state_lock, &expected, 1,
                                          false, __ATOMIC_ACQUIRE, __ATOMIC_RELAXED));
}

static void state_lock_release(void)
{
    __atomic_store_n(&state_lock, 0, __ATOMIC_RELEASE);
}
