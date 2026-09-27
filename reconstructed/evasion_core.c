/*
 * ============================================================================
 *  evasion_core.c — RECONSTRUCTION of libNexusEvasion69252.so (control plane)
 * ============================================================================
 *
 *  Source      : Ghidra 11.3.2 pseudocode (see ../core/evasion_core.c and
 *                ../features/autododge.c for the raw decompilation) plus direct
 *                extraction of the three data tables from the module binary
 *                (R_AARCH64_RELATIVE addends, Ghidra image base 0x100000).
 *  Module role : movement/combat cheat control plane — property store shared by
 *                every feature (spin, dodge, aura, follow, xray, ESP, bolt...),
 *                the mod-menu backend protocol, JNI surface, projectile math.
 *
 *  Original layout (vaddr = ghidra_addr - 0x100000):
 *    .text        0x0ec28 .. 0x1d480      code
 *    .data.rel.ro 0x21698 .. 0x24d48      the three tables below
 *    .data        0x28ff0 .. 0x29130      small initialized globals
 *    .bss         0x29130 .. 0x29d01      state array, revision, locks
 *
 *  PROTOCOL SUMMARY
 *  ----------------
 *  The Java layer (nexus/brawl/evasion/AdvancedEvasionController facade +
 *                nexus/evasion/EvasionBackend API) and the native menu
 *  (libNexusUI) both converge on one property store:
 *
 *      set_feature(name, value)     -> kind must be PROP_FEATURE
 *      set_parameter(name, value)   -> kind must be PROP_PARAM   (value clamped)
 *      get_requested(name)          -> last requested value
 *      get_effective(name)          -> value currently honored by the runtime
 *      get_port_state(name)         -> 0 idle / 1,2 active / 3 detached
 *
 *  The menu drives it through evasion_menu_backend_v1(): a 0x40-byte vtable-ish
 *  descriptor with a 168-bit capability bitmap (bit i set == action slot i
 *  supported).  Action slots ARE the wire actionIds of the tweak-flag entries
 *  (17..167).  Queries use id space 0x10000+ (118 more slots).
 *
 *  RETURN CODES (as observed crossing the backend boundary)
 *      -1  unknown key / bad arguments
 *       2  rejected (entitlement gate or activation gate refused)
 *       1  applied
 *  backend_apply maps:  1 -> 1 (applied), 2 -> 2 (applied-active param),
 *                       anything else -> 4 (rejected);  port==3 -> 0.
 * ============================================================================
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/* ---- activation-gate externs (bodies in sibling reconstructions) ---- */
extern int  evasion_is_free_scope(const char *name);   /* FUN_0011b320 */
extern int  evasion_entitlement_check(void);           /* pthread_once + DAT_00129ce0 */
extern void evasion_commit(void);                       /* FUN_0010f580 */
extern int  evasion_key_is_visual_only(const char *name);   /* FUN_0010f880 */
extern int  evasion_key_adapter_family(const char *name);   /* FUN_00112940 */
extern int  evasion_key_is_aim_family(const char *name);   /* FUN_00112b5c */
extern int  evasion_hold_family_ok(const char *name);     /* FUN_00117314 */
extern int  evasion_spin_gate(void);                      /* FUN_001158a0 */
extern int  evasion_brawler_gate_index(const char *name); /* FUN_00112d44 */
extern int  evasion_brawler_gate_set(const char *n, int v);/* FUN_0011b5d0 */
extern uint32_t evasion_brawler_bitmap(void);             /* FUN_00113200 */
extern int  evasion_brawler_bitmap_index(const char *n);  /* FUN_001130c0 */
extern int  evasion_prediction_alias_ok(const char *n);
extern int  periodic_adapter_ready(uint32_t adapter_id_slot, uint32_t bind_fn_slot);
extern int  adapter_family_ready(uint32_t id_slot, uint32_t bind_fn_slot);
extern int32_t evasion_get_requested(const char *name);
extern int32_t evasion_get_effective(const char *name);
extern int32_t evasion_get_port_state(const char *name);
extern void    nexus_evasion_diagnostics(char *buf, size_t n);
extern void    nexus_evasion_initialize(void);
extern void    nexus_evasion_restore_v1(void);
void backend_misc(void);   /* FUN_0011c828 @ 0x11c828 — third descriptor fn, not yet tied */

/* ============================================================================
 * 1. Constants
 * ============================================================================ */

/* property kinds */
#define PROP_FEATURE 0   /* boolean toggle                      */
#define PROP_PARAM   1   /* clamped numeric parameter            */

/* coercion classes (property field `coerce`, == special index paths in the
 * original dispatcher: follow family < slot 46, dodgeVersion 133,
 * speedLevel 134, target modes 135..137) */
#define COERCE_NONE           0
#define COERCE_FOLLOW_ALIAS   1   /* followEnabled/followMode* share state 38/39 */
#define COERCE_DODGE_VERSION  2   /* value in [1,5] else default 3              */
#define COERCE_TARGET_MODE    3   /* binary 1|2                                 */
#define COERCE_SPEED_LEVEL    4   /* forced to 4                                */

/* set() result codes */
#define EV_UNKNOWN_KEY   (-1)
#define EV_REJECTED      2
#define EV_APPLIED       1
#define EV_APPLIED_ACTIVE 2   /* param with active flag, non-zero value */

/* backend protocol codes (evasion_menu_backend_v1 / backend_apply) */
#define BK_OK            1
#define BK_APPLIED       1
#define BK_ACTIVE        2
#define BK_REJECTED      4
#define BK_PORT_DOWN     0
#define BK_BAD_REQUEST   4

/* port states (get_port_state) */
#define PORT_IDLE     0
#define PORT_ACTIVE_1 1     /* observed in query status derivation; exact      */
#define PORT_ACTIVE_2 2     /* naming uncertain — 1/2 gate "effective" queries */
#define PORT_DETACHED 3     /* backend returns 0 / query status 3              */

/* action tokens (action table field `token`) */
#define TOK_TOGGLE   2      /* feature on/off                                */
#define TOK_SLIDER   3      /* +/- increment pair (adjacent slots, same name)  */
#define TOK_MODE     4      /* option/mode selector                          */

/* protocol struct sizes */
#define EVASION_BACKEND_SIZE  0x40
#define EVASION_REQUEST_MAGIC 0x40   /* request header word 0                 */
#define EVASION_QUERY_SIZE    0x18
#define EVASION_QUERY_MAGIC   0x18

#define EV_MAX_NAME 0x60              /* keys are at most 95 chars            */
#define EV_STATE_COUNT 134            /* .bss 0x29130..0x29348 (uint32 each)  */

/* ============================================================================
 * 2. Types
 * ============================================================================ */

/* Property-sheet entry — 40 bytes in .data.rel.ro @ 0x21698 (144 slots).
 * name@+0 (relocated ptr) state_index@+8 kind@+12 vmin@+16 vmax@+20
 * default@+24 coerce@+28 active@+32 reserved@+36 */
typedef struct {
    const char *name;
    int32_t     state_index;   /* index into state[] (aliases share an index)  */
    int32_t     kind;          /* PROP_FEATURE / PROP_PARAM                    */
    int32_t     vmin;          /* clamp lower bound (params)                   */
    uint32_t    vmax;          /* clamp upper bound (params)                   */
    uint32_t    default_value; /* used by initialize()/defaults restore        */
    int32_t     coerce;        /* COERCE_* class                               */
    bool        active;        /* param: non-zero value -> EV_APPLIED_ACTIVE   */
} nexus_property_t;

/* Menu action slot — 32 bytes in .data.rel.ro @ 0x22d38 (168 slots).
 * name@+0 is_parameter@+0x10 token@+0x14 blacklist_bit@+0x18.
 * slot == wire actionId of the tweak-flag entries. */
typedef struct {
    const char *name;          /* NULL = unsupported slot (capability bit 0)   */
    bool        is_parameter;  /* route to set_parameter instead of feature    */
    int32_t     token;         /* TOK_TOGGLE / TOK_SLIDER / TOK_MODE           */
    int32_t     blacklist_bit; /* dodgeBlacklistMask slots 95..104: brawler 0..9 */
} evasion_action_t;

/* Rich-UI query slot — 24 bytes in .data.rel.ro @ 0x24238 (118 slots).
 * Query ids are 0x10000 + slot.  kind==PROP_FEATURE queries also fetch
 * get_effective(); params report the requested value only. */
typedef struct {
    const char *name;
    int32_t     kind;
} evasion_query_slot_t;

/* Menu -> module request (backend_apply argument, 0x40 bytes observed) */
typedef struct {
    uint32_t magic;            /* must be EVASION_REQUEST_MAGIC (0x40)         */
    uint32_t slot;             /* action slot 0..167                           */
    int32_t  token;            /* must match action table token                */
    int32_t  value;            /* payload for set_feature/set_parameter        */
    uint32_t _pad0[8];
    uint32_t arg_e;            /* word 0xe: dodge version for name resolving   */
    uint32_t _pad1;
    uint64_t cookie;           /* word 0xc (long): must be 0                   */
} evasion_request_t;

/* Module -> menu query result (0x18 bytes) */
typedef struct {
    uint32_t magic;            /* EVASION_QUERY_SIZE                           */
    int32_t  value;            /* [1] requested value (or blacklist bit)       */
    int32_t  effective;        /* [2] effective value (features)               */
    int32_t  requested;        /* [3] raw requested                           */
    uint32_t status;           /* [4] 0 ok / 3 port-detached                   */
    uint8_t  valid;            /* byte @0x15: 1 = fields meaningful            */
    uint8_t  _pad[3];
} evasion_query_t;

/* Backend descriptor built by evasion_menu_backend_v1 (0x40 bytes) */
typedef struct {
    uint32_t magic;            /* must be 1 on input                           */
    uint32_t size;             /* must be EVASION_BACKEND_SIZE on input        */
    uint64_t capability[3];    /* 168-bit action support bitmap                */
    void   (*query)(uint32_t id, evasion_query_t *out);        /* @+0x28 */
    void   (*apply)(evasion_request_t *req);                   /* @+0x30 */
    void   (*unknown)(void);   /* @+0x38 — third fn ptr, purpose not yet tied */
    uint64_t _pad;
} evasion_backend_t;

/* Periodic adapter vtable exchanged with libNexusEvasionRuntime69252.so
 * (see evasion_bind_periodic below) */
typedef struct {
    const char *name;          /* DAT_0010e668                                */
    void       *ctx;           /* DAT_00209d00                                */
    int       (*on_register)(void *);   /* FUN_001455a4  pin/spray commands   */
    void      (*on_tick)(void *);       /* FUN_0015d3c4                       */
    void      (*on_unregister)(void *); /* FUN_00166188                       */
} evasion_periodic_callbacks_t;

/* ============================================================================
 * 3. The property sheet — 144 slots @ 0x21698 (walk covers 0..141)
 *    71 features + 73 parameters; aliases intentionally share state indexes.
 * ============================================================================ */

static const nexus_property_t nexus_properties[144] = {
  { "isSpinEnabled",   0, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 0 */
  { "cameraEnabled",   1, PROP_FEATURE, 0, 1, 0, COERCE_NONE, true }, /* slot 1 */
  { "antiAfkEnabled",   2, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 2 */
  { "autofarmEnabled",   3, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 3 */
  { "autofarmAttackEnemies",   4, PROP_FEATURE, 0, 1, 1, COERCE_NONE, false }, /* slot 4 */
  { "coltModEnabled",   5, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 5 */
  { "koltModEnabled",   5, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 6 */
  { "boltWallAvoidEnabled",   6, PROP_FEATURE, 0, 1, 1, COERCE_NONE, false }, /* slot 7 */
  { "boltPredictionEnabled",   7, PROP_FEATURE, 0, 1, 1, COERCE_NONE, false }, /* slot 8 */
  { "pinEnabled",   8, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 9 */
  { "sprayEnabled",   9, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 10 */
  { "isXrayEnabled",  10, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 11 */
  { "xRayEnabled",  10, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 12 */
  { "xrayEnabled",  10, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 13 */
  { "xrayShowTargetName",  11, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 14 */
  { "aopAimEnabled",  12, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 15 */
  { "aopPredictEnabled",  13, PROP_FEATURE, 0, 1, 1, COERCE_NONE, false }, /* slot 16 */
  { "aopDebugEnabled",  14, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 17 */
  { "espEnabled",  15, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 18 */
  { "attackRangeIndicator",  16, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 19 */
  { "hitboxRenderer",  17, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 20 */
  { "enemyTracer",  18, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 21 */
  { "teammateHpIndicator",  19, PROP_FEATURE, 0, 1, 0, COERCE_NONE, true }, /* slot 22 */
  { "trophiesAboveHead",  20, PROP_FEATURE, 0, 1, 1, COERCE_NONE, false }, /* slot 23 */
  { "allyRespawnTimer",  21, PROP_FEATURE, 0, 1, 0, COERCE_NONE, true }, /* slot 24 */
  { "tileGridOverlay",  22, PROP_FEATURE, 0, 1, 0, COERCE_NONE, true }, /* slot 25 */
  { "smoothHudGraph",  23, PROP_FEATURE, 0, 1, 0, COERCE_NONE, true }, /* slot 26 */
  { "smoothHud",  24, PROP_FEATURE, 0, 1, 0, COERCE_NONE, true }, /* slot 27 */
  { "serverIPEnabled",  25, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 28 */
  { "characterOutlineEnabled",  26, PROP_FEATURE, 0, 1, 1, COERCE_NONE, false }, /* slot 29 */
  { "killauraEnabled",  27, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 30 */
  { "killauraMainAttack",  28, PROP_FEATURE, 0, 1, 1, COERCE_NONE, false }, /* slot 31 */
  { "killauraGadget",  29, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 32 */
  { "killauraSuper",  30, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 33 */
  { "killauraNoWall",  31, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 34 */
  { "killauraNoBall",  32, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 35 */
  { "ballAssistEnabled",  33, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 36 */
  { "holdToShootEnabled",  34, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 37 */
  { "holdToShootAim",  35, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 38 */
  { "holdToShootRangeCheck",  36, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 39 */
  { "combatDodgeRequireHold",  37, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 40 */
  { "followEnabled",  38, PROP_FEATURE, 0, 1, 0, COERCE_FOLLOW_ALIAS, false }, /* slot 41 */
  { "followAntiAfkEnabled",  38, PROP_FEATURE, 0, 1, 0, COERCE_FOLLOW_ALIAS, false }, /* slot 42 */
  { "followClosestAllyEnabled",  38, PROP_FEATURE, 0, 1, 0, COERCE_FOLLOW_ALIAS, false }, /* slot 43 */
  { "followModeEnemy",  39, PROP_FEATURE, 0, 1, 0, COERCE_FOLLOW_ALIAS, false }, /* slot 44 */
  { "followModeTeam",  39, PROP_FEATURE, 0, 1, 0, COERCE_FOLLOW_ALIAS, false }, /* slot 45 */
  { "speedExploitEnabled",  40, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 46 */
  { "speedLocalMoveEnabled",  41, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 47 */
  { "flyEnabled",  42, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 48 */
  { "kitNaniModEnabled",  43, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 49 */
  { "boltModEnabled",  44, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 50 */
  { "boltAutoAttackEnabled",  45, PROP_FEATURE, 0, 1, 1, COERCE_NONE, false }, /* slot 51 */
  { "boltSafeExitEnabled",  46, PROP_FEATURE, 0, 1, 1, COERCE_NONE, false }, /* slot 52 */
  { "autododgeEnabled",  47, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 53 */
  { "dynaJumpEnabled",  48, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 54 */
  { "epsteinEnabled",  49, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 55 */
  { "espShowNames",  50, PROP_FEATURE, 0, 1, 1, COERCE_NONE, false }, /* slot 56 */
  { "espShowDistance",  51, PROP_FEATURE, 0, 1, 1, COERCE_NONE, false }, /* slot 57 */
  { "espShowTracer",  52, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 58 */
  { "espShowHitbox",  53, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 59 */
  { "espShowCircle",  54, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 60 */
  { "espShowAimLine",  55, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 61 */
  { "espTeamFilterEnemies",  56, PROP_FEATURE, 0, 1, 1, COERCE_NONE, false }, /* slot 62 */
  { "espTeamFilterSelf",  57, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 63 */
  { "espTeamFilterTeammates",  58, PROP_FEATURE, 0, 1, 0, COERCE_NONE, false }, /* slot 64 */
  { "combatDodgeContinuous",  59, PROP_FEATURE, 0, 1, 1, COERCE_NONE, false }, /* slot 65 */
  { "combatDodgeCircular",  60, PROP_FEATURE, 0, 1, 1, COERCE_NONE, false }, /* slot 66 */
  { "combatDodgeAlways",  61, PROP_FEATURE, 0, 1, 1, COERCE_NONE, false }, /* slot 67 */
  { "combatDodgeQuickAccess",  62, PROP_FEATURE, 0, 1, 1, COERCE_NONE, false }, /* slot 68 */
  { "spinSpeed",  63, PROP_PARAM, 0, 62832, 785, COERCE_NONE, false }, /* slot 69 */
  { "spinRadius",  64, PROP_PARAM, 0, 300, 25, COERCE_NONE, false }, /* slot 70 */
  { "cameraMode",  65, PROP_PARAM, 0, 8, 3, COERCE_NONE, true }, /* slot 71 */
  { "cameraZoom",  66, PROP_PARAM, 25, 400, 100, COERCE_NONE, true }, /* slot 72 */
  { "cameraOffsetX",  67, PROP_PARAM, -2000, 2000, 0, COERCE_NONE, true }, /* slot 73 */
  { "cameraOffsetY",  68, PROP_PARAM, -2000, 2000, 0, COERCE_NONE, true }, /* slot 74 */
  { "followDistance",  69, PROP_PARAM, 100, 5000, 250, COERCE_NONE, false }, /* slot 75 */
  { "pinIntervalMs",  70, PROP_PARAM, 100, 5000, 800, COERCE_NONE, false }, /* slot 76 */
  { "sprayIntervalMs",  71, PROP_PARAM, 100, 5000, 600, COERCE_NONE, false }, /* slot 77 */
  { "aopPredictVersion",  72, PROP_PARAM, 1, 2, 1, COERCE_NONE, false }, /* slot 78 */
  { "aopProjectileSpeed",  73, PROP_PARAM, 500, 9000, 3200, COERCE_NONE, false }, /* slot 79 */
  { "aopLeadScale",  74, PROP_PARAM, 0, 200, 100, COERCE_NONE, false }, /* slot 80 */
  { "aopMaxRange",  75, PROP_PARAM, 500, 12000, 9000, COERCE_NONE, false }, /* slot 81 */
  { "aopReactionMs",  76, PROP_PARAM, 0, 500, 0, COERCE_NONE, false }, /* slot 82 */
  { "aopTargetMode",  77, PROP_PARAM, 0, 2, 0, COERCE_NONE, false }, /* slot 83 */
  { "combatAuraRange",  78, PROP_PARAM, 500, 12000, 4200, COERCE_NONE, false }, /* slot 84 */
  { "killauraDisableBelowHealthPercent",  79, PROP_PARAM, 0, 100, 0, COERCE_NONE, false }, /* slot 85 */
  { "killauraDisableIfHealthIsBelow",  79, PROP_PARAM, 0, 100, 0, COERCE_NONE, false }, /* slot 86 */
  { "autofarmFollowTarget",  80, PROP_PARAM, 0, 1, 0, COERCE_NONE, false }, /* slot 87 */
  { "boltSmoothPercent",  81, PROP_PARAM, 50, 100, 85, COERCE_NONE, false }, /* slot 88 */
  { "boltWallLookahead",  82, PROP_PARAM, 150, 600, 500, COERCE_NONE, false }, /* slot 89 */
  { "boltTargetRange",  83, PROP_PARAM, 600, 2000, 1400, COERCE_NONE, false }, /* slot 90 */
  { "boltExitHoldMs",  84, PROP_PARAM, 300, 1200, 650, COERCE_NONE, false }, /* slot 91 */
  { "combatDodgeRange",  85, PROP_PARAM, 300, 3000, 900, COERCE_NONE, false }, /* slot 92 */
  { "combatFireInterval",  86, PROP_PARAM, 50, 2000, 1000, COERCE_NONE, false }, /* slot 93 */
  { "dodgePrecision",  87, PROP_PARAM, 8, 32, 24, COERCE_NONE, false }, /* slot 94 */
  { "dodgeReactionPercent",  88, PROP_PARAM, 50, 180, 180, COERCE_NONE, false }, /* slot 95 */
  { "dodgeSafetyPercent",  89, PROP_PARAM, 50, 180, 100, COERCE_NONE, false }, /* slot 96 */
  { "dodgePowerPercent",  90, PROP_PARAM, 60, 140, 100, COERCE_NONE, false }, /* slot 97 */
  { "dodgeDistancePercent",  91, PROP_PARAM, 70, 140, 100, COERCE_NONE, false }, /* slot 98 */
  { "dodgeCommitPercent",  92, PROP_PARAM, 50, 150, 100, COERCE_NONE, false }, /* slot 99 */
  { "dodgeBurstHoldMs",  93, PROP_PARAM, 300, 1600, 1060, COERCE_NONE, false }, /* slot 100 */
  { "v2DodgeReactionPercent",  94, PROP_PARAM, 50, 180, 180, COERCE_NONE, false }, /* slot 101 */
  { "v2DodgeSafetyPercent",  95, PROP_PARAM, 50, 180, 100, COERCE_NONE, false }, /* slot 102 */
  { "v2DodgePowerPercent",  96, PROP_PARAM, 60, 140, 100, COERCE_NONE, false }, /* slot 103 */
  { "v2DodgeDistancePercent",  97, PROP_PARAM, 70, 140, 100, COERCE_NONE, false }, /* slot 104 */
  { "v2DodgeCommitPercent",  98, PROP_PARAM, 50, 150, 100, COERCE_NONE, false }, /* slot 105 */
  { "v2DodgeBurstHoldMs",  99, PROP_PARAM, 300, 1600, 1060, COERCE_NONE, false }, /* slot 106 */
  { "v3DodgeReactionPercent", 100, PROP_PARAM, 50, 180, 180, COERCE_NONE, false }, /* slot 107 */
  { "v3DodgeSafetyPercent", 101, PROP_PARAM, 50, 180, 100, COERCE_NONE, false }, /* slot 108 */
  { "v3DodgePowerPercent", 102, PROP_PARAM, 60, 140, 100, COERCE_NONE, false }, /* slot 109 */
  { "v3DodgeDistancePercent", 103, PROP_PARAM, 70, 140, 100, COERCE_NONE, false }, /* slot 110 */
  { "v3DodgeCommitPercent", 104, PROP_PARAM, 50, 150, 100, COERCE_NONE, false }, /* slot 111 */
  { "v3DodgeBurstHoldMs", 105, PROP_PARAM, 300, 1600, 1060, COERCE_NONE, false }, /* slot 112 */
  { "v4DodgeReactionPercent", 106, PROP_PARAM, 50, 180, 180, COERCE_NONE, false }, /* slot 113 */
  { "v4DodgeSafetyPercent", 107, PROP_PARAM, 50, 180, 100, COERCE_NONE, false }, /* slot 114 */
  { "v4DodgePowerPercent", 108, PROP_PARAM, 60, 140, 100, COERCE_NONE, false }, /* slot 115 */
  { "v4DodgeDistancePercent", 109, PROP_PARAM, 70, 140, 100, COERCE_NONE, false }, /* slot 116 */
  { "v4DodgeCommitPercent", 110, PROP_PARAM, 50, 150, 100, COERCE_NONE, false }, /* slot 117 */
  { "v4DodgeBurstHoldMs", 111, PROP_PARAM, 300, 1600, 1060, COERCE_NONE, false }, /* slot 118 */
  { "v5DodgeReactionPercent", 112, PROP_PARAM, 50, 180, 180, COERCE_NONE, false }, /* slot 119 */
  { "v5DodgeSafetyPercent", 113, PROP_PARAM, 50, 180, 100, COERCE_NONE, false }, /* slot 120 */
  { "v5DodgePowerPercent", 114, PROP_PARAM, 60, 140, 100, COERCE_NONE, false }, /* slot 121 */
  { "v5DodgeDistancePercent", 115, PROP_PARAM, 70, 140, 100, COERCE_NONE, false }, /* slot 122 */
  { "v5DodgeCommitPercent", 116, PROP_PARAM, 50, 150, 100, COERCE_NONE, false }, /* slot 123 */
  { "v5DodgeBurstHoldMs", 117, PROP_PARAM, 300, 1600, 1060, COERCE_NONE, false }, /* slot 124 */
  { "dodgeBlacklistMask", 118, PROP_PARAM, 0, 1023, 1023, COERCE_NONE, false }, /* slot 125 */
  { "holdToShootDelayMs", 119, PROP_PARAM, 0, 1000, 50, COERCE_NONE, false }, /* slot 126 */
  { "ballAssistSearchDegrees", 120, PROP_PARAM, 5, 180, 90, COERCE_NONE, false }, /* slot 127 */
  { "ballAssistBounceLimit", 121, PROP_PARAM, 0, 8, 3, COERCE_NONE, false }, /* slot 128 */
  { "outlineOpacity", 122, PROP_PARAM, 0, 1000, 1000, COERCE_NONE, false }, /* slot 129 */
  { "outlineColorR", 123, PROP_PARAM, 0, 1000, 1000, COERCE_NONE, false }, /* slot 130 */
  { "outlineColorG", 124, PROP_PARAM, 0, 1000, 1000, COERCE_NONE, false }, /* slot 131 */
  { "outlineColorB", 125, PROP_PARAM, 0, 1000, 1000, COERCE_NONE, false }, /* slot 132 */
  { "dodgeVersion", 126, PROP_PARAM, 1, 5, 3, COERCE_DODGE_VERSION, false }, /* slot 133 */
  { "speedLevel", 127, PROP_PARAM, 4, 4, 4, COERCE_SPEED_LEVEL, false }, /* slot 134 */
  { "aopAimTargetMode", 128, PROP_PARAM, 1, 2, 1, COERCE_TARGET_MODE, false }, /* slot 135 */
  { "aimTargetMode", 128, PROP_PARAM, 1, 2, 1, COERCE_TARGET_MODE, false }, /* slot 136 */
  { "xrayTargetMode", 129, PROP_PARAM, 1, 2, 1, COERCE_TARGET_MODE, false }, /* slot 137 */
  { "spinMovementMode", 130, PROP_PARAM, 0, 1, 0, COERCE_NONE, false }, /* slot 138 */
  { "spinOnlineOnlyIdle", 131, PROP_PARAM, 0, 1, 0, COERCE_NONE, false }, /* slot 139 */
  { "aopAimUltimate", 132, PROP_PARAM, 0, 1, 1, COERCE_NONE, false }, /* slot 140 */
  { "aopAimGadget", 133, PROP_PARAM, 0, 1, 1, COERCE_NONE, false }, /* slot 141 */
  { "aopAimEnabled",   0, PROP_FEATURE, 0, 0, 0, COERCE_NONE, false }, /* slot 142 */
  { "coltModEnabled",   0, PROP_FEATURE, 0, 0, 0, COERCE_NONE, false }, /* slot 143 */
};

/* entries reachable by the original linear walk (slots 142/143 are
 * min==max==0 sentinels: aopAimEnabled/coltModEnabled legacy dupes) */
#define EV_PROPERTY_WALK_COUNT 142

/* ============================================================================
 * 4. Menu action table — 168 slots @ 0x22d38
 *    slot == wire actionId; 101 slots named, rest NULL (capability bit 0)
 * ============================================================================ */

static const evasion_action_t evasion_actions[168] = {
  { NULL, false, 0, -1 }, /* slot 0 / wire actionId 0 */
  { NULL, false, 1, -1 }, /* slot 1 / wire actionId 1 */
  { NULL, false, 1, -1 }, /* slot 2 / wire actionId 2 */
  { NULL, false, 1, -1 }, /* slot 3 / wire actionId 3 */
  { NULL, false, 1, -1 }, /* slot 4 / wire actionId 4 */
  { NULL, false, 1, -1 }, /* slot 5 / wire actionId 5 */
  { NULL, false, 1, -1 }, /* slot 6 / wire actionId 6 */
  { NULL, false, 1, -1 }, /* slot 7 / wire actionId 7 */
  { NULL, false, 1, -1 }, /* slot 8 / wire actionId 8 */
  { NULL, false, 1, -1 }, /* slot 9 / wire actionId 9 */
  { NULL, false, 1, -1 }, /* slot 10 / wire actionId 10 */
  { NULL, false, 1, -1 }, /* slot 11 / wire actionId 11 */
  { NULL, false, 1, -1 }, /* slot 12 / wire actionId 12 */
  { NULL, false, 1, -1 }, /* slot 13 / wire actionId 13 */
  { NULL, false, 1, -1 }, /* slot 14 / wire actionId 14 */
  { NULL, false, 1, -1 }, /* slot 15 / wire actionId 15 */
  { NULL, false, 1, -1 }, /* slot 16 / wire actionId 16 */
  { "killauraEnabled", false, 2, -1 }, /* slot 17 / wire actionId 17 */
  { "aopPredictEnabled", false, 2, -1 }, /* slot 18 / wire actionId 18 */
  { "killauraMainAttack", false, 2, -1 }, /* slot 19 / wire actionId 19 */
  { "killauraNoWall", false, 2, -1 }, /* slot 20 / wire actionId 20 */
  { "killauraNoBall", false, 2, -1 }, /* slot 21 / wire actionId 21 */
  { "autododgeEnabled", false, 2, -1 }, /* slot 22 / wire actionId 22 */
  { "aopAimEnabled", false, 2, -1 }, /* slot 23 / wire actionId 23 */
  { "aopPredictEnabled", false, 2, -1 }, /* slot 24 / wire actionId 24 */
  { "isSpinEnabled", false, 2, -1 }, /* slot 25 / wire actionId 25 */
  { "followEnabled", false, 2, -1 }, /* slot 26 / wire actionId 26 */
  { "followClosestAllyEnabled", false, 2, -1 }, /* slot 27 / wire actionId 27 */
  { "ballAssistEnabled", false, 2, -1 }, /* slot 28 / wire actionId 28 */
  { "holdToShootEnabled", false, 2, -1 }, /* slot 29 / wire actionId 29 */
  { "isXrayEnabled", false, 2, -1 }, /* slot 30 / wire actionId 30 */
  { "espEnabled", false, 2, -1 }, /* slot 31 / wire actionId 31 */
  { "characterOutlineEnabled", false, 2, -1 }, /* slot 32 / wire actionId 32 */
  { "cameraEnabled", false, 2, -1 }, /* slot 33 / wire actionId 33 */
  { "attackRangeIndicator", false, 2, -1 }, /* slot 34 / wire actionId 34 */
  { "hitboxRenderer", false, 2, -1 }, /* slot 35 / wire actionId 35 */
  { "enemyTracer", false, 2, -1 }, /* slot 36 / wire actionId 36 */
  { "teammateHpIndicator", false, 2, -1 }, /* slot 37 / wire actionId 37 */
  { "trophiesAboveHead", false, 2, -1 }, /* slot 38 / wire actionId 38 */
  { "allyRespawnTimer", false, 2, -1 }, /* slot 39 / wire actionId 39 */
  { "tileGridOverlay", false, 2, -1 }, /* slot 40 / wire actionId 40 */
  { "smoothHudGraph", false, 2, -1 }, /* slot 41 / wire actionId 41 */
  { "smoothHud", false, 2, -1 }, /* slot 42 / wire actionId 42 */
  { "pinEnabled", false, 2, -1 }, /* slot 43 / wire actionId 43 */
  { "sprayEnabled", false, 2, -1 }, /* slot 44 / wire actionId 44 */
  { "antiAfkEnabled", false, 2, -1 }, /* slot 45 / wire actionId 45 */
  { "speedExploitEnabled", false, 2, -1 }, /* slot 46 / wire actionId 46 */
  { NULL, false, 2, -1 }, /* slot 47 / wire actionId 47 */
  { NULL, false, 0, -1 }, /* slot 48 / wire actionId 48 */
  { NULL, false, 0, -1 }, /* slot 49 / wire actionId 49 */
  { NULL, false, 0, -1 }, /* slot 50 / wire actionId 50 */
  { NULL, false, 0, -1 }, /* slot 51 / wire actionId 51 */
  { NULL, false, 0, -1 }, /* slot 52 / wire actionId 52 */
  { NULL, false, 0, -1 }, /* slot 53 / wire actionId 53 */
  { NULL, false, 0, -1 }, /* slot 54 / wire actionId 54 */
  { "aopTargetMode", true, 4, -1 }, /* slot 55 / wire actionId 55 */
  { "combatAuraRange", true, 3, -1 }, /* slot 56 / wire actionId 56 */
  { "combatAuraRange", true, 3, -1 }, /* slot 57 / wire actionId 57 */
  { "combatFireInterval", true, 3, -1 }, /* slot 58 / wire actionId 58 */
  { "combatFireInterval", true, 3, -1 }, /* slot 59 / wire actionId 59 */
  { "aopReactionMs", true, 3, -1 }, /* slot 60 / wire actionId 60 */
  { "aopReactionMs", true, 3, -1 }, /* slot 61 / wire actionId 61 */
  { "aopLeadScale", true, 3, -1 }, /* slot 62 / wire actionId 62 */
  { "aopLeadScale", true, 3, -1 }, /* slot 63 / wire actionId 63 */
  { NULL, false, 0, -1 }, /* slot 64 / wire actionId 64 */
  { NULL, false, 0, -1 }, /* slot 65 / wire actionId 65 */
  { "aopTargetMode", true, 4, -1 }, /* slot 66 / wire actionId 66 */
  { NULL, false, 1, -1 }, /* slot 67 / wire actionId 67 */
  { "autofarmEnabled", false, 2, -1 }, /* slot 68 / wire actionId 68 */
  { NULL, false, 3, -1 }, /* slot 69 / wire actionId 69 */
  { NULL, false, 4, -1 }, /* slot 70 / wire actionId 70 */
  { NULL, false, 3, -1 }, /* slot 71 / wire actionId 71 */
  { NULL, false, 4, -1 }, /* slot 72 / wire actionId 72 */
  { "autofarmAttackEnemies", false, 2, -1 }, /* slot 73 / wire actionId 73 */
  { NULL, false, 2, -1 }, /* slot 74 / wire actionId 74 */
  { "followDistance", true, 3, -1 }, /* slot 75 / wire actionId 75 */
  { "followDistance", true, 3, -1 }, /* slot 76 / wire actionId 76 */
  { "killauraDisableBelowHealthPercent", true, 3, -1 }, /* slot 77 / wire actionId 77 */
  { "killauraDisableBelowHealthPercent", true, 3, -1 }, /* slot 78 / wire actionId 78 */
  { "dodgeReactionPercent", true, 3, -1 }, /* slot 79 / wire actionId 79 */
  { "dodgeReactionPercent", true, 3, -1 }, /* slot 80 / wire actionId 80 */
  { "dodgeSafetyPercent", true, 3, -1 }, /* slot 81 / wire actionId 81 */
  { "dodgeSafetyPercent", true, 3, -1 }, /* slot 82 / wire actionId 82 */
  { "dodgePowerPercent", true, 3, -1 }, /* slot 83 / wire actionId 83 */
  { "dodgePowerPercent", true, 3, -1 }, /* slot 84 / wire actionId 84 */
  { "dodgeDistancePercent", true, 3, -1 }, /* slot 85 / wire actionId 85 */
  { "dodgeDistancePercent", true, 3, -1 }, /* slot 86 / wire actionId 86 */
  { "dodgeCommitPercent", true, 3, -1 }, /* slot 87 / wire actionId 87 */
  { "dodgeCommitPercent", true, 3, -1 }, /* slot 88 / wire actionId 88 */
  { "dodgeBurstHoldMs", true, 3, -1 }, /* slot 89 / wire actionId 89 */
  { "dodgeBurstHoldMs", true, 3, -1 }, /* slot 90 / wire actionId 90 */
  { "dodgeVersion", true, 4, -1 }, /* slot 91 / wire actionId 91 */
  { NULL, false, 0, -1 }, /* slot 92 / wire actionId 92 */
  { NULL, false, 1, -1 }, /* slot 93 / wire actionId 93 */
  { "combatDodgeRequireHold", false, 2, -1 }, /* slot 94 / wire actionId 94 */
  { "dodgeBlacklistMask", true, 2, 0 }, /* slot 95 / wire actionId 95 */
  { "dodgeBlacklistMask", true, 2, 1 }, /* slot 96 / wire actionId 96 */
  { "dodgeBlacklistMask", true, 2, 2 }, /* slot 97 / wire actionId 97 */
  { "dodgeBlacklistMask", true, 2, 3 }, /* slot 98 / wire actionId 98 */
  { "dodgeBlacklistMask", true, 2, 4 }, /* slot 99 / wire actionId 99 */
  { "dodgeBlacklistMask", true, 2, 5 }, /* slot 100 / wire actionId 100 */
  { "dodgeBlacklistMask", true, 2, 6 }, /* slot 101 / wire actionId 101 */
  { "dodgeBlacklistMask", true, 2, 7 }, /* slot 102 / wire actionId 102 */
  { "dodgeBlacklistMask", true, 2, 8 }, /* slot 103 / wire actionId 103 */
  { "dodgeBlacklistMask", true, 2, 9 }, /* slot 104 / wire actionId 104 */
  { "xrayShowTargetName", false, 2, -1 }, /* slot 105 / wire actionId 105 */
  { "xrayTargetMode", true, 4, -1 }, /* slot 106 / wire actionId 106 */
  { NULL, false, 0, -1 }, /* slot 107 / wire actionId 107 */
  { NULL, false, 2, -1 }, /* slot 108 / wire actionId 108 */
  { NULL, false, 1, -1 }, /* slot 109 / wire actionId 109 */
  { "outlineOpacity", true, 3, -1 }, /* slot 110 / wire actionId 110 */
  { "outlineOpacity", true, 3, -1 }, /* slot 111 / wire actionId 111 */
  { NULL, false, 0, -1 }, /* slot 112 / wire actionId 112 */
  { NULL, false, 1, -1 }, /* slot 113 / wire actionId 113 */
  { NULL, false, 5, -1 }, /* slot 114 / wire actionId 114 */
  { NULL, false, 7, -1 }, /* slot 115 / wire actionId 115 */
  { NULL, false, 7, -1 }, /* slot 116 / wire actionId 116 */
  { NULL, false, 6, -1 }, /* slot 117 / wire actionId 117 */
  { NULL, false, 6, -1 }, /* slot 118 / wire actionId 118 */
  { NULL, false, 6, -1 }, /* slot 119 / wire actionId 119 */
  { NULL, false, 6, -1 }, /* slot 120 / wire actionId 120 */
  { "kitNaniModEnabled", false, 2, -1 }, /* slot 121 / wire actionId 121 */
  { "coltModEnabled", false, 2, -1 }, /* slot 122 / wire actionId 122 */
  { NULL, false, 1, -1 }, /* slot 123 / wire actionId 123 */
  { NULL, false, 1, -1 }, /* slot 124 / wire actionId 124 */
  { NULL, false, 0, -1 }, /* slot 125 / wire actionId 125 */
  { NULL, false, 2, -1 }, /* slot 126 / wire actionId 126 */
  { NULL, false, 0, -1 }, /* slot 127 / wire actionId 127 */
  { NULL, false, 0, -1 }, /* slot 128 / wire actionId 128 */
  { NULL, false, 1, -1 }, /* slot 129 / wire actionId 129 */
  { NULL, false, 0, -1 }, /* slot 130 / wire actionId 130 */
  { NULL, false, 1, -1 }, /* slot 131 / wire actionId 131 */
  { "boltModEnabled", false, 2, -1 }, /* slot 132 / wire actionId 132 */
  { "boltAutoAttackEnabled", false, 2, -1 }, /* slot 133 / wire actionId 133 */
  { "boltWallAvoidEnabled", false, 2, -1 }, /* slot 134 / wire actionId 134 */
  { "boltPredictionEnabled", false, 2, -1 }, /* slot 135 / wire actionId 135 */
  { "boltSafeExitEnabled", false, 2, -1 }, /* slot 136 / wire actionId 136 */
  { "boltSmoothPercent", true, 3, -1 }, /* slot 137 / wire actionId 137 */
  { "boltSmoothPercent", true, 3, -1 }, /* slot 138 / wire actionId 138 */
  { "boltWallLookahead", true, 3, -1 }, /* slot 139 / wire actionId 139 */
  { "boltWallLookahead", true, 3, -1 }, /* slot 140 / wire actionId 140 */
  { "boltTargetRange", true, 3, -1 }, /* slot 141 / wire actionId 141 */
  { "boltTargetRange", true, 3, -1 }, /* slot 142 / wire actionId 142 */
  { "boltExitHoldMs", true, 3, -1 }, /* slot 143 / wire actionId 143 */
  { "boltExitHoldMs", true, 3, -1 }, /* slot 144 / wire actionId 144 */
  { NULL, false, 0, -1 }, /* slot 145 / wire actionId 145 */
  { "aopAimTargetMode", true, 4, -1 }, /* slot 146 / wire actionId 146 */
  { NULL, false, 1, -1 }, /* slot 147 / wire actionId 147 */
  { NULL, false, 0, -1 }, /* slot 148 / wire actionId 148 */
  { NULL, false, 0, -1 }, /* slot 149 / wire actionId 149 */
  { NULL, false, 0, -1 }, /* slot 150 / wire actionId 150 */
  { NULL, false, 0, -1 }, /* slot 151 / wire actionId 151 */
  { "spinSpeed", true, 3, -1 }, /* slot 152 / wire actionId 152 */
  { "spinSpeed", true, 3, -1 }, /* slot 153 / wire actionId 153 */
  { "dynaJumpEnabled", false, 2, -1 }, /* slot 154 / wire actionId 154 */
  { "aopPredictVersion", true, 4, -1 }, /* slot 155 / wire actionId 155 */
  { "holdToShootAim", false, 2, -1 }, /* slot 156 / wire actionId 156 */
  { NULL, false, 0, -1 }, /* slot 157 / wire actionId 157 */
  { NULL, false, 0, -1 }, /* slot 158 / wire actionId 158 */
  { NULL, false, 0, -1 }, /* slot 159 / wire actionId 159 */
  { NULL, false, 0, -1 }, /* slot 160 / wire actionId 160 */
  { NULL, false, 0, -1 }, /* slot 161 / wire actionId 161 */
  { "speedLocalMoveEnabled", false, 2, -1 }, /* slot 162 / wire actionId 162 */
  { "flyEnabled", false, 2, -1 }, /* slot 163 / wire actionId 163 */
  { "spinMovementMode", true, 2, -1 }, /* slot 164 / wire actionId 164 */
  { "spinOnlineOnlyIdle", true, 2, -1 }, /* slot 165 / wire actionId 165 */
  { "aopAimUltimate", true, 2, -1 }, /* slot 166 / wire actionId 166 */
  { "aopAimGadget", true, 2, -1 }, /* slot 167 / wire actionId 167 */
};

/* ============================================================================
 * 5. Rich-UI query table — 118 slots @ 0x24238, ids 0x10000+slot
 * ============================================================================ */

static const evasion_query_slot_t evasion_query_slots[118] = {
  { "isSpinEnabled", PROP_FEATURE }, /* queryId 0x10000 */
  { "followEnabled", PROP_FEATURE }, /* queryId 0x10001 */
  { "followModeTeam", PROP_FEATURE }, /* queryId 0x10002 */
  { "pinEnabled", PROP_FEATURE }, /* queryId 0x10003 */
  { "sprayEnabled", PROP_FEATURE }, /* queryId 0x10004 */
  { "antiAfkEnabled", PROP_FEATURE }, /* queryId 0x10005 */
  { "speedExploitEnabled", PROP_FEATURE }, /* queryId 0x10006 */
  { "speedLocalMoveEnabled", PROP_FEATURE }, /* queryId 0x10007 */
  { "flyEnabled", PROP_FEATURE }, /* queryId 0x10008 */
  { "kitNaniModEnabled", PROP_FEATURE }, /* queryId 0x10009 */
  { "coltModEnabled", PROP_FEATURE }, /* queryId 0x1000a */
  { "boltModEnabled", PROP_FEATURE }, /* queryId 0x1000b */
  { "boltAutoAttackEnabled", PROP_FEATURE }, /* queryId 0x1000c */
  { "boltWallAvoidEnabled", PROP_FEATURE }, /* queryId 0x1000d */
  { "boltPredictionEnabled", PROP_FEATURE }, /* queryId 0x1000e */
  { "boltSafeExitEnabled", PROP_FEATURE }, /* queryId 0x1000f */
  { NULL, PROP_FEATURE }, /* queryId 0x10010 */
  { "isXrayEnabled", PROP_FEATURE }, /* queryId 0x10011 */
  { "xrayShowTargetName", PROP_FEATURE }, /* queryId 0x10012 */
  { "aopAimEnabled", PROP_FEATURE }, /* queryId 0x10013 */
  { "aopPredictEnabled", PROP_FEATURE }, /* queryId 0x10014 */
  { "espEnabled", PROP_FEATURE }, /* queryId 0x10015 */
  { "characterOutlineEnabled", PROP_FEATURE }, /* queryId 0x10016 */
  { "attackRangeIndicator", PROP_FEATURE }, /* queryId 0x10017 */
  { "hitboxRenderer", PROP_FEATURE }, /* queryId 0x10018 */
  { "enemyTracer", PROP_FEATURE }, /* queryId 0x10019 */
  { "trophiesAboveHead", PROP_FEATURE }, /* queryId 0x1001a */
  { "serverIPEnabled", PROP_FEATURE }, /* queryId 0x1001b */
  { "killauraEnabled", PROP_FEATURE }, /* queryId 0x1001c */
  { "killauraMainAttack", PROP_FEATURE }, /* queryId 0x1001d */
  { "killauraGadget", PROP_FEATURE }, /* queryId 0x1001e */
  { "killauraSuper", PROP_FEATURE }, /* queryId 0x1001f */
  { "killauraNoWall", PROP_FEATURE }, /* queryId 0x10020 */
  { "killauraNoBall", PROP_FEATURE }, /* queryId 0x10021 */
  { "autododgeEnabled", PROP_FEATURE }, /* queryId 0x10022 */
  { "combatDodgeRequireHold", PROP_FEATURE }, /* queryId 0x10023 */
  { "autofarmEnabled", PROP_FEATURE }, /* queryId 0x10024 */
  { NULL, PROP_FEATURE }, /* queryId 0x10025 */
  { "autofarmAttackEnemies", PROP_FEATURE }, /* queryId 0x10026 */
  { NULL, PROP_FEATURE }, /* queryId 0x10027 */
  { NULL, PROP_FEATURE }, /* queryId 0x10028 */
  { "ballAssistEnabled", PROP_FEATURE }, /* queryId 0x10029 */
  { "holdToShootEnabled", PROP_FEATURE }, /* queryId 0x1002a */
  { "dynaJumpEnabled", PROP_FEATURE }, /* queryId 0x1002b */
  { "holdToShootAim", PROP_FEATURE }, /* queryId 0x1002c */
  { "holdToShootRangeCheck", PROP_FEATURE }, /* queryId 0x1002d */
  { "spinSpeed", PROP_PARAM }, /* queryId 0x1002e */
  { "spinRadius", PROP_PARAM }, /* queryId 0x1002f */
  { "followDistance", PROP_PARAM }, /* queryId 0x10030 */
  { "pinIntervalMs", PROP_PARAM }, /* queryId 0x10031 */
  { "sprayIntervalMs", PROP_PARAM }, /* queryId 0x10032 */
  { "xrayTargetMode", PROP_PARAM }, /* queryId 0x10033 */
  { "aopAimTargetMode", PROP_PARAM }, /* queryId 0x10034 */
  { "aopPredictVersion", PROP_PARAM }, /* queryId 0x10035 */
  { "aopProjectileSpeed", PROP_PARAM }, /* queryId 0x10036 */
  { "aopLeadScale", PROP_PARAM }, /* queryId 0x10037 */
  { "aopMaxRange", PROP_PARAM }, /* queryId 0x10038 */
  { "aopReactionMs", PROP_PARAM }, /* queryId 0x10039 */
  { "aopTargetMode", PROP_PARAM }, /* queryId 0x1003a */
  { "combatAuraRange", PROP_PARAM }, /* queryId 0x1003b */
  { "combatDodgeRange", PROP_PARAM }, /* queryId 0x1003c */
  { "combatFireInterval", PROP_PARAM }, /* queryId 0x1003d */
  { "killauraDisableBelowHealthPercent", PROP_PARAM }, /* queryId 0x1003e */
  { "dodgeReactionPercent", PROP_PARAM }, /* queryId 0x1003f */
  { "dodgeSafetyPercent", PROP_PARAM }, /* queryId 0x10040 */
  { "dodgePowerPercent", PROP_PARAM }, /* queryId 0x10041 */
  { "dodgeDistancePercent", PROP_PARAM }, /* queryId 0x10042 */
  { "dodgeCommitPercent", PROP_PARAM }, /* queryId 0x10043 */
  { "dodgeBurstHoldMs", PROP_PARAM }, /* queryId 0x10044 */
  { "v2DodgeReactionPercent", PROP_PARAM }, /* queryId 0x10045 */
  { "v2DodgeSafetyPercent", PROP_PARAM }, /* queryId 0x10046 */
  { "v2DodgePowerPercent", PROP_PARAM }, /* queryId 0x10047 */
  { "v2DodgeDistancePercent", PROP_PARAM }, /* queryId 0x10048 */
  { "v2DodgeCommitPercent", PROP_PARAM }, /* queryId 0x10049 */
  { "v2DodgeBurstHoldMs", PROP_PARAM }, /* queryId 0x1004a */
  { "v3DodgeReactionPercent", PROP_PARAM }, /* queryId 0x1004b */
  { "v3DodgeSafetyPercent", PROP_PARAM }, /* queryId 0x1004c */
  { "v3DodgePowerPercent", PROP_PARAM }, /* queryId 0x1004d */
  { "v3DodgeDistancePercent", PROP_PARAM }, /* queryId 0x1004e */
  { "v3DodgeCommitPercent", PROP_PARAM }, /* queryId 0x1004f */
  { "v3DodgeBurstHoldMs", PROP_PARAM }, /* queryId 0x10050 */
  { "v4DodgeReactionPercent", PROP_PARAM }, /* queryId 0x10051 */
  { "v4DodgeSafetyPercent", PROP_PARAM }, /* queryId 0x10052 */
  { "v4DodgePowerPercent", PROP_PARAM }, /* queryId 0x10053 */
  { "v4DodgeDistancePercent", PROP_PARAM }, /* queryId 0x10054 */
  { "v4DodgeCommitPercent", PROP_PARAM }, /* queryId 0x10055 */
  { "v4DodgeBurstHoldMs", PROP_PARAM }, /* queryId 0x10056 */
  { "v5DodgeReactionPercent", PROP_PARAM }, /* queryId 0x10057 */
  { "v5DodgeSafetyPercent", PROP_PARAM }, /* queryId 0x10058 */
  { "v5DodgePowerPercent", PROP_PARAM }, /* queryId 0x10059 */
  { "v5DodgeDistancePercent", PROP_PARAM }, /* queryId 0x1005a */
  { "v5DodgeCommitPercent", PROP_PARAM }, /* queryId 0x1005b */
  { "v5DodgeBurstHoldMs", PROP_PARAM }, /* queryId 0x1005c */
  { "dodgeBlacklistMask", PROP_PARAM }, /* queryId 0x1005d */
  { "dodgeVersion", PROP_PARAM }, /* queryId 0x1005e */
  { "holdToShootDelayMs", PROP_PARAM }, /* queryId 0x1005f */
  { "ballAssistSearchDegrees", PROP_PARAM }, /* queryId 0x10060 */
  { "ballAssistBounceLimit", PROP_PARAM }, /* queryId 0x10061 */
  { NULL, PROP_FEATURE }, /* queryId 0x10062 */
  { NULL, PROP_FEATURE }, /* queryId 0x10063 */
  { "autofarmFollowTarget", PROP_PARAM }, /* queryId 0x10064 */
  { "boltSmoothPercent", PROP_PARAM }, /* queryId 0x10065 */
  { "boltWallLookahead", PROP_PARAM }, /* queryId 0x10066 */
  { "boltTargetRange", PROP_PARAM }, /* queryId 0x10067 */
  { "boltExitHoldMs", PROP_PARAM }, /* queryId 0x10068 */
  { "outlineOpacity", PROP_PARAM }, /* queryId 0x10069 */
  { "outlineColorR", PROP_PARAM }, /* queryId 0x1006a */
  { "outlineColorG", PROP_PARAM }, /* queryId 0x1006b */
  { "outlineColorB", PROP_PARAM }, /* queryId 0x1006c */
  { NULL, PROP_FEATURE }, /* queryId 0x1006d */
  { NULL, PROP_FEATURE }, /* queryId 0x1006e */
  { NULL, PROP_FEATURE }, /* queryId 0x1006f */
  { NULL, PROP_FEATURE }, /* queryId 0x10070 */
  { NULL, PROP_FEATURE }, /* queryId 0x10071 */
  { "spinMovementMode", PROP_PARAM }, /* queryId 0x10072 */
  { "spinOnlineOnlyIdle", PROP_PARAM }, /* queryId 0x10073 */
  { "aopAimUltimate", PROP_PARAM }, /* queryId 0x10074 */
  { "aopAimGadget", PROP_PARAM }, /* queryId 0x10075 */
};

/* ============================================================================
 * 6. Runtime state (.bss)
 * ============================================================================ */

static uint32_t        state[EV_STATE_COUNT];   /* 0x29130 */
static uint32_t        state_revision;          /* 0x29348, bumped on change */
static volatile int    state_lock;              /* 0x29ce8, exclusivity-monitor spinlock */

/* --- protocol callbacks (defined in section 9; typed after section-2 structs) --- */
static void backend_query(uint32_t id, evasion_query_t *out);
static void backend_apply(evasion_request_t *req);
static int  activation_gates_pass(const char *name);

static const nexus_property_t *find_property(const char *name);
static void  state_lock_acquire(void);
static void  state_lock_release(void);

/* ============================================================================
 * 7. Core dispatcher — reconstruction of FUN_0010ec90
 *    (nexus_evasion_set_feature / nexus_evasion_set_parameter funnel here)
 * ============================================================================ */

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

    /* linear walk, exactly like the original strcmp loop */
    const nexus_property_t *prop = find_property(name);
    if (prop == NULL)
        return EV_UNKNOWN_KEY;
    if (prop->kind != (is_parameter ? PROP_PARAM : PROP_FEATURE))
        return EV_UNKNOWN_KEY;

    /* deprecated key: value is discarded, always resets to 0 */
    uint32_t v = (strcmp(name, "combatDodgeRequireHold") == 0) ? 0 : value;

    /* ---- entitlement gate ------------------------------------------------ */
    if (v != 0 || is_parameter) {
        if (!is_parameter) {
            if (!evasion_is_free_scope(name) && !evasion_entitlement_check())
                return EV_REJECTED;
        } else {
            /* parameter keys exempt from entitlement: outlineOpacity,
             * outlineColor{R,G,B}, aop*, killaura*, combatAura*, combatFire*,
             * combatDodge*, dodge*, v2Dodge*..v5Dodge*, holdToShoot*       */
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

    /* ---- locked section --------------------------------------------------- */
    state_lock_acquire();
    evasion_commit();

    /* per-key activation gates: only when a FEATURE switches ON */
    if (!is_parameter && v != 0 && !activation_gates_pass(name)) {
        state_lock_release();
        return EV_REJECTED;
    }

    /* ---- value coercion ---------------------------------------------------- */
    uint32_t coerced;
    if (is_parameter) {
        coerced = (uint32_t)clamp_u32((int32_t)v, prop->vmin, prop->vmax);
    } else {
        coerced = (v != 0) ? 1u : 0u;
    }
    switch (prop->coerce) {
    case COERCE_DODGE_VERSION:                       /* slot 133 */
        coerced = (v >= 1 && v <= 5) ? v : 3;
        break;
    case COERCE_SPEED_LEVEL:                         /* slot 134: fixed at 4 */
        coerced = 4;
        break;
    case COERCE_TARGET_MODE:                         /* slots 135..137 */
        coerced = (v == 2) ? 2 : 1;
        break;
    default:
        break;
    }

    /* ---- follow-mode alias sync (slots 41..45 share state 38/39) ----------- */
    if (prop->coerce == COERCE_FOLLOW_ALIAS) {
        /* followModeEnemy/followModeTeam are mirrored so both keys report the
         * same state; the original walks the table for the sibling name. */
        const nexus_property_t *sib = find_property(
            strcmp(name, "followModeTeam") == 0 ? "followModeEnemy" : "followModeTeam");
        if (sib && state[sib->state_index] != coerced) {
            state[sib->state_index] = coerced;
            state_revision++;
        }
    }

    /* ---- mutual exclusion: speedExploitEnabled <-> speedLocalMoveEnabled --- */
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

    /* ---- commit ------------------------------------------------------------ */
    if (state[prop->state_index] != coerced) {
        state[prop->state_index] = coerced;
        state_revision++;
    }
    state_lock_release();

    /* ---- result ------------------------------------------------------------
     * features always report EV_APPLIED; parameters with the `active` flag
     * report EV_APPLIED_ACTIVE when a non-zero value was stored.             */
    if (!is_parameter)
        return EV_APPLIED;
    return (coerced != 0 && prop->active) ? EV_APPLIED_ACTIVE : EV_APPLIED;
}

/* published entry points @ 0x10ec88 / 0x10f444 (thunks over evasion_set) */
int nexus_evasion_set_feature(const char *name, uint32_t value)
{
    return evasion_set(name, value, 0);
}

int nexus_evasion_set_parameter(const char *name, uint32_t value)
{
    return evasion_set(name, value, 1);
}

/* ============================================================================
 * 8. Activation-gate chain (feature switching ON, lock held)
 *    Reconstruction of the branch tree in FUN_0010ec90's locked section.
 *    Every adapter handshake must succeed exactly once or the enable fails.
 * ============================================================================ */

static int activation_gates_pass(const char *name)
{
    /* visual/effective-only keys need no runtime binding */
    if (evasion_key_is_visual_only(name))
        return 1;

    if (evasion_key_adapter_family(name) == 0) {
        /* pin/spray: periodic adapter (see evasion_bind_periodic) */
        if (strcmp(name, "pinEnabled") == 0 || strcmp(name, "sprayEnabled") == 0)
            return periodic_adapter_ready(/*DAT_00129450*/ 0, /*DAT_00129468*/ 0);

        /* aim/aura family: FUN_00112b5c */
        if (evasion_key_is_aim_family(name))                       /* FUN_00112b5c */
            return adapter_family_ready(/*DAT_00129428*/ 0, /*DAT_00129440*/ 0);

        /* speed family passes straight through (server-visible movement) */
        if (strcmp(name, "speedExploitEnabled") == 0 ||
            strcmp(name, "speedLocalMoveEnabled") == 0)
            return 1;

        /* holdToShoot family: FUN_00117314 */
        if (strcmp(name, "holdToShootEnabled") == 0 ||
            strcmp(name, "holdToShootAim") == 0 ||
            strcmp(name, "holdToShootRangeCheck") == 0)
            return evasion_hold_family_ok(name);                   /* FUN_00117314 */

        /* isSpinEnabled: dedicated gate FUN_001158a0 */
        if (strcmp(name, "isSpinEnabled") == 0)
            return evasion_spin_gate();                            /* FUN_001158a0 */

        /* generic: brawler allowlist / bitmap gates */
        int idx = evasion_brawler_gate_index(name);                /* FUN_00112d44 */
        if (idx >= 0)
            return evasion_brawler_gate_set(name, 1);              /* FUN_0011b5d0 */
        int bit = evasion_brawler_bitmap_index(name);              /* FUN_001130c0 */
        if (bit >= 0)
            return (evasion_brawler_bitmap() >> (bit & 31)) & 1;   /* FUN_00113200 */
        /* fallback: prediction-keyed alias check (aopPredictEnabled) */
        return evasion_prediction_alias_ok(name);
    }
    /* second adapter family (DAT_00129478/DAT_00129490) + free-scope recheck */
    return adapter_family_ready(/*DAT_00129478*/ 1, /*DAT_00129490*/ 1)
        && (evasion_is_free_scope(name) || evasion_entitlement_check());
}

/* ============================================================================
 * 9. Menu backend protocol — FUN_0011c4a8 / FUN_0011c72c / FUN_0011c560
 * ============================================================================ */

static const char *resolve_action_key(const evasion_action_t *act,
                                      uint32_t dodge_version, char *buf, size_t bufsz);

int nexus_evasion_menu_backend_v1(evasion_backend_t *out)
{
    if (out == NULL || out->magic != 1 || out->size != EVASION_BACKEND_SIZE)
        return BK_REJECTED;

    /* zero the descriptor, keep a pristine copy of the header word */
    uint32_t magic = out->magic, size = out->size;
    memset(out, 0, sizeof *out);
    out->magic = magic;
    out->size  = size;

    /* capability bitmap: slot supported == table name != NULL */
    for (int slot = 0; slot < 168; slot++)
        if (evasion_actions[slot].name != NULL)
            out->capability[slot >> 6] |= 1ULL << (slot & 63);

    out->query   = backend_query;    /* FUN_0011c560 @ 0x11c560 */
    out->apply   = backend_apply;    /* FUN_0011c72c @ 0x11c72c */
    out->unknown = backend_misc;     /* FUN_0011c828 @ 0x11c828 — not yet tied  */
    return BK_OK;
}

/* FUN_0011c72c: apply a menu request */
static void backend_apply(evasion_request_t *req)
{
    if (req == NULL || req->magic != EVASION_REQUEST_MAGIC)
        return;                                   /* result 4 (bad request)   */
    if (req->slot > 167)
        return;                                   /* result 4                 */
    if (req->cookie != 0)
        return;                                   /* result 4                 */

    const evasion_action_t *act = &evasion_actions[req->slot];
    if (act->name == NULL)
        return;                                   /* result 4 (unsupported)   */
    if (act->token != req->token)
        return;                                   /* result 4 (token mismatch)*/

    if (evasion_get_port_state(NULL) == PORT_DETACHED)
        { /* result 0 */ return; }

    char keybuf[96];
    const char *key = resolve_action_key(act, req->arg_e, keybuf, sizeof keybuf);

    int32_t r = act->is_parameter
        ? nexus_evasion_set_parameter(key, (uint32_t)req->value)
        : nexus_evasion_set_feature(key, (uint32_t)req->value);

    /* result mapping observed in the original:
     *   r == EV_APPLIED -> 1, r == EV_REJECTED -> 2, else -> 4               */
    (void)r;
}

/* FUN_0011c560: query current values for the rich UI */
static void backend_query(uint32_t id, evasion_query_t *out)
{
    if (out == NULL || out->magic != EVASION_QUERY_SIZE)
        { out->status = BK_REJECTED; return; }

    memset(out, 0, sizeof *out);
    out->magic = EVASION_QUERY_SIZE;

    if (id >= 0x10000 && id < 0x10000 + 118) {
        /* rich-UI query table */
        const evasion_query_slot_t *qs = &evasion_query_slots[id - 0x10000];
        if (qs->name == NULL)
            return;                               /* result 0                 */
        int32_t req = evasion_get_requested(qs->name);
        if (req == INT32_MIN)
            return;                               /* result 0                 */
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
        /* action-table query: dodgeVersion-aware key resolution */
        const evasion_action_t *act = &evasion_actions[id];
        if (act->name == NULL)
            return;
        char keybuf[96];
        int32_t dv = evasion_get_requested("dodgeVersion");
        const char *key = resolve_action_key(act, (uint32_t)dv, keybuf, sizeof keybuf);
        int32_t req = evasion_get_requested(key);
        if (req == INT32_MIN)
            return;
        /* blacklist slots report their single bit */
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
    /* out of range: result 4 */
}

/* FUN_0011c914: resolve an action-table key against the active dodge version.
 * "dodgeXxx" ( Xxx != Version/BlacklistMask/Precision ) with version in [2,5]
 * becomes "v{N}DodgeXxx" so the versioned property slots are used. */
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

/* ============================================================================
 * 10. Projectile math (clean, self-contained — @ 0x11a2e0 / 0x11a478)
 * ============================================================================ */

/*
 * nexus_evasion_intercept — solve for the interception time of a moving target.
 *
 * Input (2-D, relative frame):
 *   px, py : target position relative to the shooter
 *   vx, vy : target velocity relative to the shooter
 *   s      : projectile speed (s > 0)
 * Output:
 *   *t_out : flight time in seconds, valid window [0, 1.55)
 *
 * Solves  |P + V t|^2 = (s t)^2   i.e. the quadratic
 *   (vx^2 + vy^2 - s^2) t^2 + 2 (px vx + py vy) t + (px^2 + py^2) = 0
 * picking the smallest non-negative root.  The 1.55 s ceiling matches the
 * longest projectile flight the game models; roots beyond it are rejected.
 * All inputs are NaN/Inf-guarded exactly like the original.
 */
int nexus_evasion_intercept(float px, float py, float vx, float vy, float s, float *t_out)
{
    if (t_out == NULL || !(s > 0.0f) || isinff(s) || isnanf(s))
        return 0;
    const float in[4] = { px, py, vx, vy };
    for (int i = 0; i < 4; i++)
        if (isinff(in[i]) || isnanf(in[i]))
            return 0;

    float pp = fmaf(px, px, py * py);          /* |P|^2   (c)                */
    if (pp < 1e-4f)                            /* target at the muzzle       */
        { *t_out = 0.0f; return 1; }

    float vv = fmaf(vx, vx, vy * vy);          /* |V|^2                      */
    float bp = fmaf(px, vx, py * vy);          /* P . V   (half of b)        */
    float a  = fmaf(s, s, -vv);                /* s^2 - |V|^2                */

    float t;
    if (a >= -5e-4f && a <= 5e-4f) {           /* (near-)linear: b t + c = 0 */
        if (bp >= -5e-4f)                       /* no solution ahead          */
            return 0;
        t = -pp / (bp + bp);
    } else {
        float disc = fmaf(bp, bp, -a * pp);    /* b'^2 - 4ac                 */
        if (disc < 0.0f)
            return 0;                           /* no real root               */
        float sq = sqrtf(disc);
        float t0 = (-bp - sq) / (a + a);
        float t1 = (-bp + sq) / (a + a);
        /* smallest non-negative root, with the original's tie handling */
        t = (t1 <= t0) ? t1 : t0;
        if (t1 < 0.0f || t1 == 0.0f) t = t0;
        if (t0 < 0.0f || t0 == 0.0f) t = t1;
    }

    if (!(t >= 0.0f && t < 1.55f))             /* validity window            */
        return 0;
    if (isinff(t) || isnanf(t))
        return 0;
    *t_out = t;
    return 1;
}

/*
 * nexus_evasion_segment_distance — distance from point P to segment AB.
 * Inputs: P=(px,py), A=(ax,ay), B=(bx,by).  Outputs (all optional except dist):
 *   *dist    : minimum distance
 *   *cx,*cy  : closest point on AB
 *   *t_ab    : position of the closest point along AB, clamped to [0,1]
 * NaN/Inf-guarded; returns 0 on bad input, 1 on success.
 */
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
    float dd = fmaf(dx, dx, dy * dy);          /* |AB|^2                     */
    float t  = 0.0f;
    if (dd > 1e-6f) {
        t = fmaf(px - ax, dx, (py - ay) * dy) / dd;
        if (t < 0.0f) t = 0.0f;
        if (t > 1.0f) t = 1.0f;
    }
    float qx = fmaf(dy, t, ay);                /* closest point (x, y)       */
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

/* ============================================================================
 * 11. JNI surface — JNI_OnLoad @ 0x11c9d4
 *     facade: nexus/brawl/evasion/AdvancedEvasionController
 *     api   : nexus/evasion/EvasionBackend
 * ============================================================================ */

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
    char buf[1024];                            /* original: 0x400 buffer     */
    nexus_evasion_diagnostics(buf, sizeof buf);
    return (*env)->NewStringUTF(env, buf);
}

/* initNativeHooks: re-run initialize (idempotent), report state 2 */
static jint jni_initNativeHooks(JNIEnv *env, jclass cls)
{
    (void)env; (void)cls;
    nexus_evasion_initialize();
    return 2;
}

static const JNINativeMethod facade_methods[] = {
    { "initNativeHooks",          "()I",                                   jni_initNativeHooks },
    { "setEvasionVersion",        "(Ljava/lang/String;II)I",               NULL /* loader-side */ },
    { "setNativeCrashDownloadFd", "(Ljava/lang/String;II)I",               NULL /* loader-side */ },
    { "setNativeCrashPrivateFd",  "(Ljava/lang/String;II)I",               NULL /* loader-side */ },
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
        return JNI_ERR;                        /* tag: jni_environment       */

    jclass facade = (*env)->FindClass(env, "nexus/brawl/evasion/AdvancedEvasionController");
    if (facade == NULL)
        return JNI_ERR;                        /* tag: facade_class          */
    jclass api = (*env)->FindClass(env, "nexus/evasion/EvasionBackend");
    if (api == NULL)
        return JNI_ERR;                        /* tag: api_class             */

    if ((*env)->RegisterNatives(env, facade, facade_methods,
                                sizeof facade_methods / sizeof *facade_methods) != 0)
        return JNI_ERR;                        /* tag: facade_signatures     */
    if ((*env)->RegisterNatives(env, api, backend_methods,
                                sizeof backend_methods / sizeof *backend_methods) != 0)
        return JNI_ERR;                        /* tag: api_signatures        */

    /* original registers the backend first, then the facade, then drops the
     * local refs and runs the module initializer                               */
    (*env)->DeleteLocalRef(env, facade);
    (*env)->DeleteLocalRef(env, api);
    nexus_evasion_initialize();
    return JNI_VERSION_1_6;
}
#endif /* NEXUS_WITH_JNI */

/* ============================================================================
 * 12. Periodic adapter binder — FUN_00147aec in libNexusEvasionRuntime69252.so
 *     Wires the pin & spray native commands (types 9 and 15) into the Evasion
 *     module's periodic callback system via dlsym'd exports:
 *       nexus_evasion_register_periodic_v1
 *       nexus_evasion_snapshot_keys_v1
 *     Both symbols must dladdr() back to the pinned Evasion module path, or
 *     the binder refuses and pin/spray stay disabled.
 * ============================================================================ */

/* periodic binder plumbing (section 12) — resolved via dlsym/dladdr at runtime */
extern void *dlsym_lookup(void *handle, const char *symbol);
extern int   dladdr_same_module(const void *addr);
extern void  periodic_bind_start(void);
extern void  log_periodic_state(const char *state, const char *detail, const char *extra);
extern int   periodic_ready;
#define PERIODIC_NAME         ((const char *)0x1e0978)   /* DAT_001e0978       */
#define PERIODIC_CTX          ((void *)0x209d00)         /* DAT_00209d00       */
extern int   pin_spray_on_register(void *ctx);           /* FUN_001455a4       */
extern void  pin_spray_on_tick(void *ctx);               /* FUN_0015d3c4       */
extern void  pin_spray_on_unregister(void *ctx);         /* FUN_00166188       */

void evasion_bind_periodic(void *evasion_module_handle)
{
    /* resolved via dlsym(DAT_0020d0f0, ...) in the original */
    int (*register_periodic)(const void *) = (void *)dlsym_lookup(
        evasion_module_handle, "nexus_evasion_register_periodic_v1");
    void *(*snapshot_keys)(void) = (void *)dlsym_lookup(
        evasion_module_handle, "nexus_evasion_snapshot_keys_v1");

    if (register_periodic == NULL || snapshot_keys == NULL)
        return;
    if (!dladdr_same_module(register_periodic) || !dladdr_same_module(snapshot_keys))
        return;                                /* provenance check failed    */

    /* adapter descriptor at DAT_00228e48 (see periodic_callbacks_t) */
    periodic_bind_start();                     /* FUN_001a16a0(&desc, &list) */

    evasion_periodic_callbacks_t cb = {
        .name         = PERIODIC_NAME,         /* DAT_0010e668               */
        .ctx          = PERIODIC_CTX,          /* DAT_00209d00               */
        .on_register  = pin_spray_on_register, /* FUN_001455a4               */
        .on_tick      = pin_spray_on_tick,     /* FUN_0015d3c4               */
        .on_unregister= pin_spray_on_unregister,/* FUN_00166188              */
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

/* ============================================================================
 * 13. Thin accessors (state-array backed; original bodies in sibling files)
 * ============================================================================ */

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

/* get_effective applies port/battle-state masking implemented by the runtime
 * observers (ng_* family) — see ../core/game_state_readers.c               */

/* ============================================================================
 * 14. Spinlock (LDXR/STXR exclusivity monitor on .bss 0x29ce8)
 * ============================================================================ */

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

/*
 * RECONSTRUCTION NOTES / OPEN ITEMS
 * ---------------------------------
 *  - FUN_0011c828 (backend descriptor's third fn) is not yet tied to a caller;
 *    named backend_misc() here.
 *  - The activation-gate externs in section 7/8 are named after their observed
 *    behavior; their bodies live in the raw exports (FUN_0010f880 etc.) and
 *    still need their own reconstruction pass.
 *  - get_effective / get_port_state / diagnostics bodies are in sibling files
 *    (see features/autododge.c, core/game_state_readers.c).
 *  - Property slots 142/143 (aopAimEnabled, coltModEnabled with min==max==0)
 *    are unreachable legacy dupes of slots 15/5.
 *  - koltModEnabled (slot 6) is a typo-alias of coltModEnabled sharing state 5.
 *  - Query status derivation (0 vs 1) is reproduced as observed; the exact
 *    meaning of port states 1/2 needs a runtime trace to confirm.
 */
