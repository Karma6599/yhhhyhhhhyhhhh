#define _GNU_SOURCE 1

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <sys/mman.h>
#include <link.h>



#define NEAR_ADRP_REACH 0x8000000
#define NEAR_RING_MAX 0x6f00001
#define MIRROR_MIN_PAGE 0x170
#define GAME_IMAGE_SPAN 0x1400000
#define MIRROR_OBJECT_OFFSET 0x15c
#define GAME_KNOWN_OBJECT_OFFSET 0xb2e674
#define MAPS_MAX_LINES 0x8000
#define MAPS_MAX_BYTES 0x800000
#define MAPS_LINE_MAX 0x2000
#define UNIQUE_MAPS_LINE_MAX 0x10c0
#define BRAND_SSO_THRESHOLD 8
#define BRAND_NAME_MAX 0x140
#define BRAND_THROTTLE_US 100
#define BRAND_ATTEMPT_MAX 20
#define MAP_TILE_MAX 0x78
#define MAP_CHUNK 0x200
#define WEAPON_TABLE_LEN 246
#define ALTERNATOR_TABLE_LEN 246
#define TARGET_LIST_MAX 16
#define WEAPON_MIN_LEN 7
#define WEAPON_MAX_LEN 63
#define WEAPON_DAMAGE_MAX 5000
#define WEAPON_VERSION_EXPECTED 0x34
#define WEAPON_SPEED_DEFAULT_BITS 0x3E3851ECu
#define CONTROLLER_ENTRY_MAX 0x2c8

typedef struct {
    uintptr_t game_base;
    int (*read)(uintptr_t ctx, uintptr_t addr, void *out, uint32_t len);
    uintptr_t ctx;
} game_reader_t;

typedef struct {
    const char *name;
    uint32_t value;
} alternator_damage_entry_t;

typedef struct {
    const char *name;
    uint16_t damage;
    uint8_t elite;
} weapon_desc_t;

typedef struct {
    uintptr_t lo;
    uintptr_t hi;
    uintptr_t slots[64];
    int32_t count;
    uint32_t pad;
    size_t size;
    int32_t validate_with_scan;
} gap_window_t;

typedef struct {
    uintptr_t obj;
    uintptr_t vtable;
    uintptr_t pair;
} controller_slot_t;

typedef struct {
    controller_slot_t slot[2];
    uintptr_t speed_obj;
    uintptr_t generation;
} controller_state_t;

typedef struct {
    uint32_t valid;
    uint32_t damage;
    uint32_t elite;
    uint32_t targets;
    float speed_scale;
    char name[68];
    uintptr_t obj;
} weapon_info_t;

typedef struct {
    uintptr_t start;
    uintptr_t end;
    uintptr_t offset;
    uintptr_t dev;
    uintptr_t inode;
    uintptr_t path;
} maps_entry_t;

typedef struct {
    const char *phase;
    uint32_t err;
} near_phase_cell_t;

typedef struct {
    uint32_t unk;
    uint32_t len;
    uintptr_t ptr;
} name_header_t;

typedef struct {
    uintptr_t list1;
    int32_t cap1;
    uint32_t count1;
    uintptr_t list2;
    int32_t cap2;
    uint32_t count2;
    uintptr_t extra;
} target_arrays_t;

extern uintptr_t g_game_base;
extern size_t g_page_size;
extern uint64_t g_mem_reads;
extern uintptr_t g_world_ctx;
extern uintptr_t g_world_token;
extern int g_world_index;
extern uintptr_t g_profile_token;
extern char g_maps_parse_fmt[];
extern char near_phase_cell[];
extern char near_label_cell[];

extern uintptr_t runtime_read_handle(void);
extern int mem_read(uintptr_t handle, uintptr_t addr, void *out, uint32_t len);
extern int mem_read_ptr(const game_reader_t *reader, uintptr_t addr, uintptr_t *out);
extern int mem_read_cstr(const game_reader_t *reader, uintptr_t addr, char *out);
extern int mem_read_array(uintptr_t vec_addr, uintptr_t *data_out);
extern uintptr_t projectile_read_handle(uintptr_t token_slot, uintptr_t *obj_out);
extern int resolve_weapon_name(uintptr_t game_base, uintptr_t obj,
                               int (*read_fn)(uintptr_t, uintptr_t, void *, uint32_t),
                               int mode, char *out);
extern int parse_maps_line(const char *line, void *fmt, void *out);
extern void map_cache_publish(void);
extern uint64_t source_now_us(void);
extern int mirror_mapping_valid(uintptr_t addr);
extern void gap_window_offer(gap_window_t *window, uintptr_t target,
                             uintptr_t prev_end, uintptr_t cur_start);
extern void *cell_lock(void *cell);
extern void near_alloc_report(const char *stage, uintptr_t target, int err);
extern void log_event(const char *category, const char *event, const char *json);
extern int verify_code_sha256(uintptr_t offset, size_t len, const char *sha256_hex);
extern void branding_reset(void);
extern char *branding_sanitize_name(const char *raw);
extern int branding_avatar_ok(uintptr_t players, uintptr_t player);
extern void branding_finish(int verified, uint64_t now);
extern int branding_image_check(uintptr_t cache_field, uintptr_t players);
extern int branding_image_create(uintptr_t players);
extern void branding_post(uintptr_t player, int verified);
extern int branding_identity_fast(uintptr_t player, uintptr_t players);
extern void branding_once_init(void);
extern void branding_report_refused(uint32_t code, const char *label);
extern int branding_phdr_scan(struct dl_phdr_info *info, size_t size, void *data);

static const char *const near_phase_names[26] = {
    "mmap", "prepare", "plan_contract", "plan_status",
    "plan_unbound", "plan_current_entry", "plan_capacity", "enter",
    "resume", "seal", "entry_busy", "entry_unbound",
    "entry_read", "entry_word", "gap_window", "gap_open",
    "gap_read", "gap_maps", "gap_bounds", "gap_parse",
    "gap_close", "gap_empty", "gap_range", "gap_mmap",
    "gap_return", "gap_unmap",
};

static const alternator_damage_entry_t alternator_damage_table[246] = {
    { "AlternatorWeaponDamage", 25 }, { "AlternatorWeaponHeal", 25 }, { "AlternatorWeaponSlow", 25 },
    { "AlternatorWeaponSpeed", 25 }, { "AmbusherWeapon", 6 }, { "ArcadeWeapon", 30 },
    { "ArcadeWeaponOvercharged", 30 }, { "ArtilleryDudeWeapon", 26 }, { "AssaultShotgunOverchargedWeapon", 25 },
    { "AssaultShotgunWeapon", 25 }, { "AttacherWeapon", 11 }, { "AttacherWeaponAlternate", 22 },
    { "AttacherWeaponAlternateOvercharge", 30 }, { "AttractorWeapon", 27 }, { "AxeJugglerWeapon", 24 },
    { "BarkeepWeapon", 22 }, { "BarrelBotWeapon", 18 }, { "BarrelBotWeaponNanoPower", 18 },
    { "BaseballOverchargedWeapon", 11 }, { "BaseballWeapon", 11 }, { "BeamerWeapon", 27 },
    { "BeeSniperWeapon", 30 }, { "BlackHoleWeapon", 24 }, { "BlowerWeapon", 25 },
    { "BoneThrowerWeapon", 27 }, { "BowDudeOverchargedWeapon", 26 }, { "BowDudeWeapon", 26 },
    { "BullDudeOverchargedWeapon", 14 }, { "BullDudeWeapon", 16 }, { "BulletstormWeapon", 30 },
    { "BulletstormWeaponLastShot", 30 }, { "CactusOverchargedWeapon", 23 }, { "CactusWeapon", 23 },
    { "CannonGirlSmallWeapon", 15 }, { "CannonGirlWeapon", 27 }, { "CannonGirlWeaponNanoPower", 27 },
    { "ChronomancerWeapon", 25 }, { "ClusterBombDudeWeapon", 26 }, { "CocoonerPetWeapon", 6 },
    { "CocoonerWeapon", 27 }, { "ConductorWeapon", 18 }, { "ConductorWeaponOvercharged", 18 },
    { "ControllerWeapon", 26 }, { "CookerWeapon", 27 }, { "CookerWeaponNanoPower", 27 },
    { "CrabWeapon1", 23 }, { "CrabWeapon2", 23 }, { "CrabWeapon3", 23 },
    { "CrossBomberWeapon", 23 }, { "CrowOverchargedWeapon", 26 }, { "CrowWeapon", 26 },
    { "DancerWeaponDouble", 18 }, { "DancerWeaponSingle", 24 }, { "DancerWeaponTriple", 14 },
    { "DaredevilWeapon", 10 }, { "DeadMariachiWeapon", 21 }, { "DeadMariachiWeaponNanoPower", 21 },
    { "DeadMariachiWeaponOvercharged", 21 }, { "DiggerDrillWeapon", 5 }, { "DiggerWeapon", 15 },
    { "DomainWeapon", 10 }, { "DoorManWeapon", 27 }, { "DoorManWeaponNanoPower", 33 },
    { "DragonRiderDragonWeapon", 17 }, { "DragonRiderWeapon", 12 }, { "DrillerWeapon", 10 },
    { "DuelistWeapon", 16 }, { "DuplicatorOverchargedPetWeapon", 27 }, { "DuplicatorWeapon", 27 },
    { "ElectroSniperWeapon", 30 }, { "EnragerOverchargedWeapon", 6 }, { "EnragerWeapon", 6 },
    { "FireDudeOverchargedWeapon", 25 }, { "FireDudeWeapon", 25 }, { "FishTankWeapon", 10 },
    { "FleaPetWeapon", 8 }, { "FleaWeapon", 28 }, { "FuryWeapon", 22 },
    { "FuryWeaponSp", 22 }, { "FutureGirlWeapon", 24 }, { "GeishaTransformedWeapon", 20 },
    { "GeishaWeapon", 8 }, { "GhostWeapon", 11 }, { "GladiatorWeapon", 8 },
    { "GladiatorWeaponFirePunch", 8 }, { "GodzillaWeapon1", 13 }, { "GodzillaWeapon2", 13 },
    { "GunslingerOverchargedWeapon", 27 }, { "GunslingerWeapon", 27 }, { "HammerDudeOverchargedWeapon", 18 },
    { "HammerDudeWeapon", 18 }, { "HookWeapon", 17 }, { "IceDudeWeapon", 28 },
    { "InsectManWeapon", 30 }, { "InsectManWeaponNanoPower", 30 }, { "InsectManWeaponPoison", 30 },
    { "InstagibWeapon", 30 }, { "JesterWeapon", 25 }, { "JetpackGirlWeapon", 12 },
    { "KatanaKidWeaponFullChargeRanged", 24 }, { "KatanaKidWeaponRanged", 24 }, { "KatanaKidWeaponSlash", 11 },
    { "KickerDudeWeapon", 8 }, { "KnightWeapon", 14 }, { "LeaperWeapon", 12 },
    { "LightyearFlightWeapon", 20 }, { "LightyearSwordWeapon", 11 }, { "LightyearWeapon", 27 },
    { "LuchadorOverchargedWeapon", 10 }, { "LuchadorWeapon", 9 }, { "MagicalGirlWeapon", 17 },
    { "MagicalGirlWeaponTransformed", 11 }, { "MaisieWeapon", 26 }, { "MaisieWeaponNanoPower", 26 },
    { "MechaDudeBigWeapon", 22 }, { "MechaDudeBigWeaponBuddy", 22 }, { "MechaDudeWeapon", 27 },
    { "MechaDudeWeaponOverChargedBuddy", 27 }, { "MechaVanFriendlyMegBossWeapon", 22 }, { "MechanicWeapon", 27 },
    { "MeepleWallWeapon", 23 }, { "MeepleWeapon", 23 }, { "MegaBossAssaultShotgunWeapon", 50 },
    { "MegaBossAssaultShotgunWeaponForth", 50 }, { "MegaBossAssaultShotgunWeaponSecond", 50 }, { "MegaBossAssaultShotgunWeaponThird", 50 },
    { "MegaBossBlackHoleWeapon", 50 }, { "MegaBossCactusSecondWeapon", 30 }, { "MegaBossCactusWeapon", 30 },
    { "MegaBossCrossBombWeapon360", 32 }, { "MegaBossCrossBomberWeapon", 32 }, { "MegaBossCrossBomberWeaponSecond", 32 },
    { "MegaBossCrowSecondWeapon", 40 }, { "MegaBossCrowWeapon", 40 }, { "MegaBossDragonCrowWeaponL1", 26 },
    { "MegaBossDragonCrowWeaponL2", 30 }, { "MegaBossDragonCrowWeaponL3", 24 }, { "MegaBossDragonCrowWeaponSecondL1", 26 },
    { "MegaBossDragonCrowWeaponSecondL2", 30 }, { "MegaBossDragonCrowWeaponSecondL3", 42 }, { "MegaBossDuoWeapon", 100 },
    { "MegaBossDuoWeapon360", 100 }, { "MegaBossFinxKittenWeapon", 7 }, { "MegaBossFinxReturnedWeaponSkill", 50 },
    { "MegaBossFinxWeaponSkill", 45 }, { "MegaBossFrankPetWeapon", 8 }, { "MegaBossInsectManWeapon", 100 },
    { "MegaBossInsectManWeaponSecond", 100 }, { "MegaBossMaisieWeapon", 45 }, { "MegaBossNitaWeapon", 80 },
    { "MegaBossPercenterWeapon", 35 }, { "MegaBossPercenterWeapon360", 30 }, { "MegaBossPercenterWeaponLarge", 100 },
    { "MegaBossRocketGirlWeaponBasic", 45 }, { "MegaBossRocketGirlWeaponRotatingStream", 45 }, { "MegaBossRocketGirlWeaponRotatingStreamOvercharged", 45 },
    { "MegaBossSplitterWeapon", 30 }, { "MegaBossSplitterWeaponRange", 200 }, { "MegaBossStickyBombWeapon", 35 },
    { "MegaBossStickyBombWeaponSecond", 55 }, { "MegaBossTrickshot2Weapon", 25 }, { "MegaBossTrickshot2WeaponSecond", 25 },
    { "MegaBossTrickshotWeapon", 25 }, { "MegaBossTrickshotWeaponSecond", 25 }, { "MegaKatanaKidWeaponRanged", 40 },
    { "MegaKatanaKidWeaponSlash", 25 }, { "MegaTickWeapon", 15 }, { "MegaTickWeaponSecond", 30 },
    { "MenderWeapon", 22 }, { "MinigunDudeWeapon", 27 }, { "MinigunDudeWeaponNano", 27 },
    { "MorningstarWeapon", 24 }, { "MummyOverchargedWeapon", 20 }, { "MummyWeapon", 20 },
    { "NinjaOverchargedWeapon", 29 }, { "NinjaWeapon", 29 }, { "PainterWeapon", 19 },
    { "PercenterOverchargedWeapon", 26 }, { "PercenterWeapon", 26 }, { "PowerLevelerWeapon", 20 },
    { "PowerLevelerWeaponOverchargedBuddy", 20 }, { "PuppeteerWeapon", 22 }, { "RedirecterSnakePetWeapon", 8 },
    { "RedirecterWeapon", 18 }, { "RedirecterWeaponSecondary", 20 }, { "ReviverWeapon", 10 },
    { "RocketGirlWeapon", 27 }, { "RocketGirlWeaponBuddy", 27 }, { "RollerWeapon", 23 },
    { "RopeDudeWeapon", 8 }, { "RosaWeapon", 11 }, { "RuffsWeapon", 27 },
    { "SamuraiWeaponDash", 8 }, { "SamuraiWeaponSlash", 11 }, { "SandstormWeapon", 18 },
    { "ShadowdemonWeapon", 22 }, { "ShamanOverchargedWeapon", 20 }, { "ShamanWeapon", 18 },
    { "ShieldTankWeapon", 16 }, { "ShotgunGirlOverchargedWeapon", 23 }, { "ShotgunGirlWeapon", 23 },
    { "SilencerWeapon", 27 }, { "SkaterWeapon", 19 }, { "SnakeOilWeapon", 30 },
    { "SniperWeapon", 30 }, { "SniperWeaponNanoPower", 30 }, { "SoulCollectorOverchargedWeapon", 28 },
    { "SoulCollectorWeapon", 28 }, { "SpawnerDudeWeapon", 21 }, { "SpeedyWeapon", 25 },
    { "SpeedyWeaponOverchargedBuddy", 25 }, { "SplitterLegsWeapon", 10 }, { "SplitterWeapon", 30 },
    { "SplitterWeaponNanoPower", 30 }, { "StackerWeapon", 25 }, { "StalkerWeaponDash", 8 },
    { "StalkerWeaponJump", 11 }, { "StickyBombWeapon", 23 }, { "SuperNovaBeeSniperWeapon", 28 },
    { "SuperNovaBeeSniperWeaponSecond", 40 }, { "SuperNovaCactusWeapon", 11 }, { "SuperNovaChronomancerWeapon", 25 },
    { "SuperNovaFireDudeWeapon", 17 }, { "SuperNovaMagicalGirlWeaponTransformed", 11 }, { "SuperNovaPercenterWeapon", 26 },
    { "SuperNovaVoodooPetWeapon", 16 }, { "SuperNovaVoodooWeaponEarth", 10 }, { "SuperNovaVoodooWeaponForest", 10 },
    { "SuperNovaVoodooWeaponWater", 10 }, { "TntDudeWeapon", 22 }, { "TrickshotDudeWeapon", 29 },
    { "TrickshotDudeWeaponBuddyOvercharged", 29 }, { "TwinsWeaponShotgun", 20 }, { "TwinsWeaponThrower", 22 },
    { "UndertakerOverchargedWeapon", 8 }, { "UndertakerWeapon", 8 }, { "VoodooWeaponAll", 27 },
    { "VoodooWeaponEarth", 19 }, { "VoodooWeaponForest", 27 }, { "VoodooWeaponWater", 19 },
    { "WallyWeapon", 15 }, { "WeaponThrowerUlti", 26 }, { "WeaponThrowerUlti2", 26 },
    { "WeaponThrowerWeapon", 9 }, { "WeaponThrowerWeapon2", 9 }, { "WhirlwindWeapon", 25 },
};

static const weapon_desc_t weapon_table[246] = {
    { "AlternatorWeaponDamage", 225, 0 }, { "AlternatorWeaponHeal", 225, 0 }, { "AlternatorWeaponSlow", 225, 0 },
    { "AlternatorWeaponSpeed", 225, 0 }, { "AmbusherWeapon", 300, 0 }, { "ArcadeWeapon", 50, 0 },
    { "ArcadeWeaponOvercharged", 50, 0 }, { "ArtilleryDudeWeapon", 150, 0 }, { "AssaultShotgunOverchargedWeapon", 80, 0 },
    { "AssaultShotgunWeapon", 80, 0 }, { "AttacherWeapon", 0, 0 }, { "AttacherWeaponAlternate", 0, 1 },
    { "AttacherWeaponAlternateOvercharge", 0, 1 }, { "AttractorWeapon", 0, 1 }, { "AxeJugglerWeapon", 125, 0 },
    { "BarkeepWeapon", 0, 1 }, { "BarrelBotWeapon", 0, 0 }, { "BarrelBotWeaponNanoPower", 0, 0 },
    { "BaseballOverchargedWeapon", 0, 0 }, { "BaseballWeapon", 0, 0 }, { "BeamerWeapon", 100, 0 },
    { "BeeSniperWeapon", 150, 0 }, { "BlackHoleWeapon", 100, 0 }, { "BlowerWeapon", 50, 0 },
    { "BoneThrowerWeapon", 0, 1 }, { "BowDudeOverchargedWeapon", 70, 0 }, { "BowDudeWeapon", 70, 0 },
    { "BullDudeOverchargedWeapon", 250, 0 }, { "BullDudeWeapon", 0, 0 }, { "BulletstormWeapon", 100, 0 },
    { "BulletstormWeaponLastShot", 110, 0 }, { "CactusOverchargedWeapon", 150, 0 }, { "CactusWeapon", 150, 0 },
    { "CannonGirlSmallWeapon", 100, 0 }, { "CannonGirlWeapon", 150, 0 }, { "CannonGirlWeaponNanoPower", 150, 0 },
    { "ChronomancerWeapon", 100, 0 }, { "ClusterBombDudeWeapon", 0, 1 }, { "CocoonerPetWeapon", 0, 0 },
    { "CocoonerWeapon", 150, 0 }, { "ConductorWeapon", 200, 0 }, { "ConductorWeaponOvercharged", 200, 0 },
    { "ControllerWeapon", 40, 0 }, { "CookerWeapon", 100, 0 }, { "CookerWeaponNanoPower", 100, 0 },
    { "CrabWeapon1", 150, 0 }, { "CrabWeapon2", 150, 0 }, { "CrabWeapon3", 150, 0 },
    { "CrossBomberWeapon", 0, 1 }, { "CrowOverchargedWeapon", 50, 0 }, { "CrowWeapon", 50, 0 },
    { "DancerWeaponDouble", 200, 0 }, { "DancerWeaponSingle", 150, 0 }, { "DancerWeaponTriple", 150, 0 },
    { "DaredevilWeapon", 0, 0 }, { "DeadMariachiWeapon", 150, 0 }, { "DeadMariachiWeaponNanoPower", 150, 0 },
    { "DeadMariachiWeaponOvercharged", 150, 0 }, { "DiggerDrillWeapon", 200, 0 }, { "DiggerWeapon", 150, 0 },
    { "DomainWeapon", 0, 0 }, { "DoorManWeapon", 50, 0 }, { "DoorManWeaponNanoPower", 50, 0 },
    { "DragonRiderDragonWeapon", 250, 0 }, { "DragonRiderWeapon", 200, 0 }, { "DrillerWeapon", 0, 0 },
    { "DuelistWeapon", 100, 0 }, { "DuplicatorOverchargedPetWeapon", 100, 0 }, { "DuplicatorWeapon", 100, 0 },
    { "ElectroSniperWeapon", 101, 0 }, { "EnragerOverchargedWeapon", 300, 0 }, { "EnragerWeapon", 300, 0 },
    { "FireDudeOverchargedWeapon", 200, 0 }, { "FireDudeWeapon", 200, 0 }, { "FishTankWeapon", 0, 0 },
    { "FleaPetWeapon", 0, 0 }, { "FleaWeapon", 200, 0 }, { "FuryWeapon", 0, 1 },
    { "FuryWeaponSp", 0, 1 }, { "FutureGirlWeapon", 250, 0 }, { "GeishaTransformedWeapon", 100, 0 },
    { "GeishaWeapon", 0, 0 }, { "GhostWeapon", 0, 1 }, { "GladiatorWeapon", 300, 0 },
    { "GladiatorWeaponFirePunch", 300, 0 }, { "GodzillaWeapon1", 0, 0 }, { "GodzillaWeapon2", 0, 0 },
    { "GunslingerOverchargedWeapon", 50, 0 }, { "GunslingerWeapon", 50, 0 }, { "HammerDudeOverchargedWeapon", 150, 0 },
    { "HammerDudeWeapon", 150, 0 }, { "HookWeapon", 150, 0 }, { "IceDudeWeapon", 60, 0 },
    { "InsectManWeapon", 100, 0 }, { "InsectManWeaponNanoPower", 100, 0 }, { "InsectManWeaponPoison", 100, 0 },
    { "InstagibWeapon", 101, 0 }, { "JesterWeapon", 100, 0 }, { "JetpackGirlWeapon", 100, 0 },
    { "KatanaKidWeaponFullChargeRanged", 130, 0 }, { "KatanaKidWeaponRanged", 130, 0 }, { "KatanaKidWeaponSlash", 0, 0 },
    { "KickerDudeWeapon", 300, 0 }, { "KnightWeapon", 250, 0 }, { "LeaperWeapon", 0, 0 },
    { "LightyearFlightWeapon", 100, 0 }, { "LightyearSwordWeapon", 0, 0 }, { "LightyearWeapon", 101, 0 },
    { "LuchadorOverchargedWeapon", 300, 0 }, { "LuchadorWeapon", 150, 0 }, { "MagicalGirlWeapon", 250, 0 },
    { "MagicalGirlWeaponTransformed", 0, 0 }, { "MaisieWeapon", 101, 0 }, { "MaisieWeaponNanoPower", 101, 0 },
    { "MechaDudeBigWeapon", 100, 0 }, { "MechaDudeBigWeaponBuddy", 100, 0 }, { "MechaDudeWeapon", 150, 0 },
    { "MechaDudeWeaponOverChargedBuddy", 150, 0 }, { "MechaVanFriendlyMegBossWeapon", 100, 0 }, { "MechanicWeapon", 150, 0 },
    { "MeepleWallWeapon", 0, 1 }, { "MeepleWeapon", 101, 0 }, { "MegaBossAssaultShotgunWeapon", 50, 0 },
    { "MegaBossAssaultShotgunWeaponForth", 70, 0 }, { "MegaBossAssaultShotgunWeaponSecond", 70, 0 }, { "MegaBossAssaultShotgunWeaponThird", 50, 0 },
    { "MegaBossBlackHoleWeapon", 100, 0 }, { "MegaBossCactusSecondWeapon", 195, 0 }, { "MegaBossCactusWeapon", 195, 0 },
    { "MegaBossCrossBombWeapon360", 0, 1 }, { "MegaBossCrossBomberWeapon", 0, 1 }, { "MegaBossCrossBomberWeaponSecond", 0, 1 },
    { "MegaBossCrowSecondWeapon", 40, 0 }, { "MegaBossCrowWeapon", 40, 0 }, { "MegaBossDragonCrowWeaponL1", 50, 0 },
    { "MegaBossDragonCrowWeaponL2", 50, 0 }, { "MegaBossDragonCrowWeaponL3", 225, 0 }, { "MegaBossDragonCrowWeaponSecondL1", 50, 0 },
    { "MegaBossDragonCrowWeaponSecondL2", 50, 0 }, { "MegaBossDragonCrowWeaponSecondL3", 150, 0 }, { "MegaBossDuoWeapon", 50, 0 },
    { "MegaBossDuoWeapon360", 0, 1 }, { "MegaBossFinxKittenWeapon", 0, 0 }, { "MegaBossFinxReturnedWeaponSkill", 100, 0 },
    { "MegaBossFinxWeaponSkill", 100, 0 }, { "MegaBossFrankPetWeapon", 0, 0 }, { "MegaBossInsectManWeapon", 110, 0 },
    { "MegaBossInsectManWeaponSecond", 180, 0 }, { "MegaBossMaisieWeapon", 101, 0 }, { "MegaBossNitaWeapon", 0, 1 },
    { "MegaBossPercenterWeapon", 140, 0 }, { "MegaBossPercenterWeapon360", 0, 1 }, { "MegaBossPercenterWeaponLarge", 250, 0 },
    { "MegaBossRocketGirlWeaponBasic", 0, 1 }, { "MegaBossRocketGirlWeaponRotatingStream", 75, 0 }, { "MegaBossRocketGirlWeaponRotatingStreamOvercharged", 75, 0 },
    { "MegaBossSplitterWeapon", 50, 0 }, { "MegaBossSplitterWeaponRange", 0, 1 }, { "MegaBossStickyBombWeapon", 140, 0 },
    { "MegaBossStickyBombWeaponSecond", 200, 0 }, { "MegaBossTrickshot2Weapon", 85, 0 }, { "MegaBossTrickshot2WeaponSecond", 150, 0 },
    { "MegaBossTrickshotWeapon", 85, 0 }, { "MegaBossTrickshotWeaponSecond", 150, 0 }, { "MegaKatanaKidWeaponRanged", 250, 0 },
    { "MegaKatanaKidWeaponSlash", 0, 0 }, { "MegaTickWeapon", 0, 1 }, { "MegaTickWeaponSecond", 0, 1 },
    { "MenderWeapon", 150, 0 }, { "MinigunDudeWeapon", 50, 0 }, { "MinigunDudeWeaponNano", 50, 0 },
    { "MorningstarWeapon", 170, 0 }, { "MummyOverchargedWeapon", 250, 0 }, { "MummyWeapon", 200, 0 },
    { "NinjaOverchargedWeapon", 100, 0 }, { "NinjaWeapon", 100, 0 }, { "PainterWeapon", 0, 1 },
    { "PercenterOverchargedWeapon", 150, 0 }, { "PercenterWeapon", 150, 0 }, { "PowerLevelerWeapon", 150, 0 },
    { "PowerLevelerWeaponOverchargedBuddy", 150, 0 }, { "PuppeteerWeapon", 0, 1 }, { "RedirecterSnakePetWeapon", 0, 0 },
    { "RedirecterWeapon", 100, 0 }, { "RedirecterWeaponSecondary", 80, 0 }, { "ReviverWeapon", 0, 0 },
    { "RocketGirlWeapon", 101, 0 }, { "RocketGirlWeaponBuddy", 101, 0 }, { "RollerWeapon", 150, 0 },
    { "RopeDudeWeapon", 250, 0 }, { "RosaWeapon", 150, 0 }, { "RuffsWeapon", 70, 0 },
    { "SamuraiWeaponDash", 0, 0 }, { "SamuraiWeaponSlash", 0, 0 }, { "SandstormWeapon", 200, 0 },
    { "ShadowdemonWeapon", 70, 0 }, { "ShamanOverchargedWeapon", 300, 0 }, { "ShamanWeapon", 250, 0 },
    { "ShieldTankWeapon", 250, 0 }, { "ShotgunGirlOverchargedWeapon", 0, 0 }, { "ShotgunGirlWeapon", 0, 0 },
    { "SilencerWeapon", 100, 0 }, { "SkaterWeapon", 100, 0 }, { "SnakeOilWeapon", 150, 0 },
    { "SniperWeapon", 101, 0 }, { "SniperWeaponNanoPower", 101, 0 }, { "SoulCollectorOverchargedWeapon", 0, 1 },
    { "SoulCollectorWeapon", 150, 0 }, { "SpawnerDudeWeapon", 200, 0 }, { "SpeedyWeapon", 50, 0 },
    { "SpeedyWeaponOverchargedBuddy", 50, 0 }, { "SplitterLegsWeapon", 0, 0 }, { "SplitterWeapon", 50, 0 },
    { "SplitterWeaponNanoPower", 50, 0 }, { "StackerWeapon", 150, 0 }, { "StalkerWeaponDash", 0, 0 },
    { "StalkerWeaponJump", 0, 0 }, { "StickyBombWeapon", 50, 0 }, { "SuperNovaBeeSniperWeapon", 105, 0 },
    { "SuperNovaBeeSniperWeaponSecond", 0, 1 }, { "SuperNovaCactusWeapon", 150, 0 }, { "SuperNovaChronomancerWeapon", 100, 0 },
    { "SuperNovaFireDudeWeapon", 200, 0 }, { "SuperNovaMagicalGirlWeaponTransformed", 0, 0 }, { "SuperNovaPercenterWeapon", 150, 0 },
    { "SuperNovaVoodooPetWeapon", 150, 0 }, { "SuperNovaVoodooWeaponEarth", 0, 1 }, { "SuperNovaVoodooWeaponForest", 0, 1 },
    { "SuperNovaVoodooWeaponWater", 0, 1 }, { "TntDudeWeapon", 0, 1 }, { "TrickshotDudeWeapon", 101, 0 },
    { "TrickshotDudeWeaponBuddyOvercharged", 101, 0 }, { "TwinsWeaponShotgun", 0, 0 }, { "TwinsWeaponThrower", 0, 1 },
    { "UndertakerOverchargedWeapon", 0, 0 }, { "UndertakerWeapon", 0, 0 }, { "VoodooWeaponAll", 0, 1 },
    { "VoodooWeaponEarth", 0, 1 }, { "VoodooWeaponForest", 0, 1 }, { "VoodooWeaponWater", 0, 1 },
    { "WallyWeapon", 0, 1 }, { "WeaponThrowerUlti", 250, 0 }, { "WeaponThrowerUlti2", 0, 1 },
    { "WeaponThrowerWeapon", 150, 0 }, { "WeaponThrowerWeapon2", 150, 0 }, { "WhirlwindWeapon", 250, 0 },
};

static const char *const controller_kind_names[2] = { "ControllerUltiProjectile", "ControllerUltiOverchargedProjectile" };


static const struct {
    uintptr_t offset;
    const char *sha256;
} branding_abi_hashes[13] = {
    { 0x80f09c, "ceaed7de0bf298f5764c6e7c23a9e61f0ef78d62461e51956e3ba81f858a05b5" },
    { 0x5d8ad4, "284c790c5c501cf083a6f4d323aa30275b074eb44bef7b1d64f70d9250bd6a23" },
    { 0x5921a0, "14b79d91644b7f17357a86b8ec6d36e17be6a1c815083ec6c2fa531bee369974" },
    { 0x66ae58, "dbee7aa155f50e7bbcb598d7292a63cdcc6b00e7bef42df179bba8e717a01cb2" },
    { 0x66ad48, "23f6ad205540b5dd1d4a6dfec22810176034231102bb53dc25ce7b774778e92c" },
    { 0xb7c41c, "40459ad7d604600054570f79962f208d94703d75b3f5903e63af68cbc4ee756a" },
    { 0x5955dc, "c715b651533d6c19dec4381cfd0ed1002d060d936ca20c8b6bdd10aea0d24859" },
    { 0x595314, "0558f75486a424fcf1edb8b5b3cace86a0dc5620506aab1273a1df1dc7ad2ad8" },
    { 0x594138, "f74689171f152d63ffdcb66f59b8532476d7dd17a559c44c6fb98cfc919d42ee" },
    { 0x592368, "74116921e171c3edbd6ae1a89ccc993046c5a12e87b8dc8fa3d44200102f34c6" },
    { 0xb7c73c, "30c08799bf93e6dc1dc368fb371a3076f43bee2d3ca800b2ea1e1aff828e000b" },
    { 0x59567c, "9e18529b88a6285f2675ad92537b12875a9735c6fc16cbdc947b58b5e235b5f9" },
    { 0x11a2840, "7624512f3481536b03735eb7d1bad9138b0288494a9458e050b9bd27feef8f66" },
};

static uintptr_t mirror_base;
static uint32_t mirror_slot;

static uintptr_t rw_mapping_start;
static uintptr_t rw_mapping_end;

static uint64_t branding_throttle_until;
static int32_t branding_abi_state;
static int (*branding_linker_validate)(uintptr_t);
static uint32_t branding_once_bits;
static uint32_t branding_attempts;
static pthread_once_t branding_once_control = PTHREAD_ONCE_INIT;
static int (*branding_verify)(int);
static uintptr_t branding_token;
static uintptr_t branding_vt1;
static uintptr_t branding_vt2;
static uintptr_t branding_player;
static uint8_t branding_avatar_cache[0x230];
static uint32_t branding_player_id;
static int branding_name_valid;
static char branding_name[0x80];
static char branding_last_name[0x140];
static int branding_image_refused;

static int map_known;
static uint32_t map_change_frame;
static uint8_t map_scan_active;
static uint32_t map_scan_progress;
static uint64_t map_refresh_us;
static uint32_t map_failure_stage;
static uint32_t map_failure_index;
static uint32_t map_attempts;
static uint64_t map_last_log_ms;
static uintptr_t map_cached_map;
static uintptr_t map_cached_tiles;
static uint32_t map_cached_h;
static uint32_t map_cached_w;
static uintptr_t map_scan_map;
static uintptr_t map_scan_tiles;
static uint32_t map_scan_h;
static uint32_t map_scan_w;
static uint32_t map_complete_frame;
static uint32_t map_tile_count;
static uint32_t map_scanned_frame;
static uint64_t map_read_us_acc;
static uint64_t map_read_count_acc;
static uint8_t map_wall_bitmap[MAP_TILE_MAX * MAP_TILE_MAX];
static uint8_t source_wall_cache[MAP_TILE_MAX * MAP_TILE_MAX];

static float bits_to_float(uint32_t bits)
{
    float f;
    memcpy(&f, &bits, sizeof f);
    return f;
}

static uint64_t monotonic_us(void)
{
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
        return 0;
    return (uint64_t)ts.tv_sec * 1000000u + (uint64_t)ts.tv_nsec / 1000u;
}

static uint64_t monotonic_ms(void)
{
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
        return 0;
    return (uint64_t)ts.tv_sec * 1000u + (uint64_t)ts.tv_nsec / 1000000u;
}

static int within_adrp(uintptr_t a, uintptr_t b)
{
    return (uint64_t)((int64_t)b - (int64_t)a + (int64_t)NEAR_ADRP_REACH)
           < 2u * NEAR_ADRP_REACH;
}

static int mirror_candidate_ok(uintptr_t cand, uintptr_t target, size_t page)
{
    if (cand < 0x10000 || (cand & 0xf) != 0)
        return 0;
    if (page < MIRROR_MIN_PAGE)
        return 0;
    if (target > UINT64_MAX - 0x10)
        return 0;
    if (cand > UINT64_MAX - page)
        return 0;
    if (g_game_base > UINT64_MAX - GAME_IMAGE_SPAN)
        return 0;
    if (!(page + cand <= g_game_base || g_game_base + GAME_IMAGE_SPAN <= cand))
        return 0;
    if (((cand | target) & 3) != 0)
        return 0;
    if (!within_adrp(target, cand))
        return 0;
    if (!within_adrp(cand + MIRROR_OBJECT_OFFSET, target + 0x10))
        return 0;
    return 1;
}

static int hex_value(int c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return -1;
}

static int targets_push(uintptr_t *list, uint32_t *count, uintptr_t value)
{
    uint32_t n = *count;
    if (n != 0) {
        for (uint32_t i = 0; i < n; i++)
            if (list[i] == value)
                return 1;
        if (n == TARGET_LIST_MAX)
            return 0;
    }
    list[n] = value;
    *count = n + 1;
    return 1;
}

const char *near_alloc_phase_label(void)
{
    near_phase_cell_t *cell = cell_lock(near_phase_cell);
    const char *phase = cell->phase;
    const char *known = "prepare";
    uint32_t err = 0;

    if (phase != NULL) {
        for (int i = 0; i < 26; i++) {
            if (strcmp(near_phase_names[i], phase) == 0) {
                known = phase;
                break;
            }
        }
    }
    if (strcmp(known, "mmap") == 0 || strcmp(known, "seal") == 0
        || strcmp(known, "gap_open") == 0 || strcmp(known, "gap_read") == 0
        || strcmp(known, "gap_close") == 0 || strcmp(known, "gap_mmap") == 0
        || strcmp(known, "gap_unmap") == 0) {
        uint32_t v = cell->err;
        err = (v >= 1 && v <= 0xfff) ? v : 0;
    }
    char *buf = cell_lock(near_label_cell);
    snprintf(buf, 0x32, "near_%s_b337c4_e%u", known, (unsigned)err);
    return buf;
}

static void *mirror_map_in_gap(const void *target, gap_window_t *win,
                               const char **stage, int *err, int *fatal)
{
    if (win->count == 0)
        return NULL;
    for (int32_t i = 0; i < win->count; i++) {
        uintptr_t hint = win->slots[i];
        size_t size = win->size;
        if (win->validate_with_scan == 0) {
            if (!mirror_candidate_ok(hint, (uintptr_t)target, size)) {
                *stage = "gap_range";
                *err = 0;
                *fatal = 1;
                return NULL;
            }
        } else if (!mirror_mapping_valid(hint)) {
            *stage = "gap_range";
            *err = 0;
            *fatal = 1;
            return NULL;
        }
        void *m = mmap((void *)hint, size, PROT_READ | PROT_WRITE,
                       MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);
        if (m == MAP_FAILED) {
            *stage = "gap_mmap";
            *err = errno;
        } else if ((uintptr_t)m == hint) {
            return (void *)hint;
        } else {
            *stage = "gap_return";
            *err = 0;
            if (munmap(m, size) != 0) {
                *stage = "gap_unmap";
                *err = errno;
            }
        }
    }
    return NULL;
}

static FILE *open_proc_maps(void)
{
    FILE *f;
    for (int tries = 0; tries < 8; tries++) {
        f = fopen("/proc/self/maps", "r");
        if (f != NULL)
            return f;
        if (errno != EINTR)
            return NULL;
    }
    return NULL;
}

void *near_mirror_alloc(const void *target)
{
    uintptr_t t = (uintptr_t)target;
    size_t page = g_page_size;
    uintptr_t aligned = t & -page;
    int err = 0;
    int unmapped = 0;
    const char *stage;
    void *m;
    FILE *f;
    gap_window_t window1;
    gap_window_t window2;
    int have_window2 = 0;

    if (mirror_base != 0 && mirror_slot <= 2) {
        uintptr_t cand = mirror_base + page * mirror_slot;
        if (mirror_candidate_ok(cand, t, page)) {
            mirror_slot++;
            return (void *)cand;
        }
    }

    if (mirror_base == 0) {
        size_t size3 = page * 3;
        uintptr_t hi_cap = UINT64_MAX - 0x1000000;
        uintptr_t off = 0x100000;
        do {
            if (aligned <= hi_cap && aligned + off != 0) {
                m = mmap((void *)(aligned + off), size3, PROT_READ | PROT_WRITE,
                         MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
                if (m == MAP_FAILED) {
                    err = errno;
                } else if (mirror_mapping_valid((uintptr_t)m)) {
                    mirror_slot = 1;
                    mirror_base = (uintptr_t)m;
                    return m;
                } else {
                    munmap(m, size3);
                    unmapped = 1;
                }
            }
            if (off < aligned) {
                m = mmap((void *)(aligned - off), size3, PROT_READ | PROT_WRITE,
                         MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
                if (m == MAP_FAILED) {
                    err = errno;
                } else if (mirror_mapping_valid((uintptr_t)m)) {
                    mirror_slot = 1;
                    mirror_base = (uintptr_t)m;
                    return m;
                } else {
                    munmap(m, size3);
                    unmapped = 1;
                }
            }
            hi_cap -= 0x100000;
            off += 0x100000;
        } while (off < NEAR_RING_MAX);
    }

    {
        uintptr_t hi_cap = UINT64_MAX - 0x1000000;
        uintptr_t off = 0x100000;
        do {
            if (aligned <= hi_cap && aligned + off != 0) {
                m = mmap((void *)(aligned + off), page, PROT_READ | PROT_WRITE,
                         MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
                if (m == MAP_FAILED) {
                    err = errno;
                } else if (mirror_candidate_ok((uintptr_t)m, t, page)) {
                    return m;
                } else {
                    munmap(m, page);
                    unmapped = 1;
                }
            }
            if (off < aligned) {
                m = mmap((void *)(aligned - off), page, PROT_READ | PROT_WRITE,
                         MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
                if (m == MAP_FAILED) {
                    err = errno;
                } else if (mirror_candidate_ok((uintptr_t)m, t, page)) {
                    return m;
                } else {
                    munmap(m, page);
                    unmapped = 1;
                }
            }
            hi_cap -= 0x100000;
            off += 0x100000;
        } while (off < NEAR_RING_MAX);
    }

    if (unmapped) {
        err = 0;
        stage = "alloc_range";
    } else {
        stage = "alloc_mmap";
    }

    memset(&window1, 0, sizeof window1);
    memset(&window2, 0, sizeof window2);

    if ((t & 3) == 0 && (page == 0x1000 || page == 0x4000)
        && g_game_base <= UINT64_MAX - GAME_IMAGE_SPAN
        && g_game_base <= t && (t - g_game_base) >> 0x16 < 5) {
        uintptr_t lo_raw = (t >> 0x1b == 0) ? 0 : t - NEAR_ADRP_REACH;
        uintptr_t hi_raw = t + 0x7fffeb4;
        if (t > UINT64_MAX - 0x7fffeb5)
            hi_raw = UINT64_MAX;
        if (lo_raw < 0x10001)
            lo_raw = 0x10000;
        uintptr_t hi_lim = (hi_raw > UINT64_MAX - page) ? UINT64_MAX - page : hi_raw;
        window1.lo = (lo_raw + page - 1) & -page;
        window1.hi = hi_lim & -page;
        window1.size = page;
        window1.validate_with_scan = 0;

        if (window1.lo <= window1.hi) {
            if (mirror_base == 0 && t == g_game_base + GAME_KNOWN_OBJECT_OFFSET) {
                uintptr_t lo_a = (g_game_base >= 0x74d1880)
                                     ? g_game_base - 0x74d1880 : 0;
                uintptr_t lo_b = (g_game_base >= 0x74d166c)
                                     ? g_game_base - 0x74d166c : 0;
                uintptr_t lo_1 = (lo_a >= 2 * page) ? lo_a - 2 * page : 0;
                uintptr_t lo_2 = (lo_b >= page) ? lo_b - page : 0;
                if (lo_2 < lo_raw)
                    lo_2 = lo_raw;
                window2.size = page * 3;
                window2.validate_with_scan = 1;
                if (lo_1 < lo_2)
                    lo_1 = lo_2;
                if (lo_1 <= UINT64_MAX - page) {
                    uintptr_t hi_a = g_game_base + 0x8b2e634;
                    uintptr_t hi_b = g_game_base + 0x8b2e848;
                    uintptr_t hi_c = hi_raw;
                    if (g_game_base > UINT64_MAX - 0x8b2e635)
                        hi_a = UINT64_MAX;
                    if (g_game_base > UINT64_MAX - 0x8b2e849)
                        hi_b = UINT64_MAX;
                    if (hi_c > UINT64_MAX - page * 3)
                        hi_c = UINT64_MAX - page * 3;
                    uintptr_t hi = hi_a - 2 * page;
                    uintptr_t hi2 = hi_b - page;
                    if (hi_c < hi2)
                        hi2 = hi_c;
                    if (hi2 < hi)
                        hi = hi2;
                    window2.lo = (lo_1 + page - 1) & -page;
                    window2.hi = hi & -page;
                    if (window2.lo <= window2.hi)
                        have_window2 = 1;
                }
            }

            f = open_proc_maps();
            if (f == NULL) {
                stage = "gap_open";
                err = errno;
                goto report;
            }

            {
                char line[MAPS_LINE_MAX];
                uint32_t lines = 0;
                uint64_t total_bytes = 0;
                uintptr_t prev_end = 0;
                int keep = 0;
                int read_tries = 0;
                uintptr_t start = 0;
                uintptr_t end = 0;
                const char *perms;
                size_t i, j, len;
                int c;
                int ferr;

                for (;;) {
                    errno = 0;
                    if (fgets(line, MAPS_LINE_MAX, f) == NULL) {
                        if (ferror(f) && errno == EINTR && read_tries < 8) {
                            read_tries++;
                            clearerr(f);
                            continue;
                        }
                        break;
                    }
                    read_tries = 0;
                    len = strlen(line);
                    if (len == 0 || total_bytes + len > MAPS_MAX_BYTES) {
                        stage = "gap_bounds";
                        err = 0;
                        goto scan_done;
                    }
                    if (line[len - 1] != '\n') {
                        stage = "gap_bounds";
                        err = 0;
                        goto scan_done;
                    }
                    lines++;
                    if (lines > MAPS_MAX_LINES) {
                        stage = "gap_bounds";
                        err = 0;
                        goto scan_done;
                    }
                    total_bytes += len;

                    start = 0;
                    i = 0;
                    while ((c = hex_value((unsigned char)line[i])) >= 0) {
                        start = start * 16 + (uintptr_t)c;
                        i++;
                        if (start >> 60) {
                            stage = "gap_parse";
                            err = 0;
                            goto scan_done;
                        }
                    }
                    if (i == 0 || line[i] != '-') {
                        stage = "gap_parse";
                        err = 0;
                        goto scan_done;
                    }
                    i++;
                    end = 0;
                    j = 0;
                    while ((c = hex_value((unsigned char)line[i + j])) >= 0) {
                        end = end * 16 + (uintptr_t)c;
                        j++;
                        if (end >> 60) {
                            stage = "gap_parse";
                            err = 0;
                            goto scan_done;
                        }
                    }
                    if (j == 0 || line[i + j] != ' ') {
                        stage = "gap_parse";
                        err = 0;
                        goto scan_done;
                    }
                    perms = line + i + j + 1;
                    if (strlen(perms) < 5
                        || (perms[0] != 'r' && perms[0] != '-')
                        || (perms[1] != 'w' && perms[1] != '-')
                        || (perms[2] != 'x' && perms[2] != '-')
                        || (perms[3] != 's' && perms[3] != 'p')
                        || perms[4] != ' '
                        || end <= start
                        || start < prev_end
                        || ((g_page_size - 1) & (start | end)) != 0) {
                        stage = "gap_parse";
                        err = 0;
                        goto scan_done;
                    }

                    if (have_window2)
                        gap_window_offer(&window2, t, prev_end, start);
                    gap_window_offer(&window1, t, prev_end, start);
                    {
                        int new_keep = keep || (t + 0x10 <= end);
                        prev_end = end;
                        if (t < start || t > UINT64_MAX - 0x10)
                            new_keep = keep;
                        keep = new_keep;
                    }
                }

                ferr = ferror(f);
                if (ferr || !feof(f)) {
                    stage = "gap_read";
                    err = ferr ? errno : 0;
                    goto scan_done;
                }
                if (lines == 0 || !keep) {
                    stage = "gap_maps";
                    err = 0;
                    goto scan_done;
                }
                if (have_window2)
                    gap_window_offer(&window2, t, prev_end, UINT64_MAX);
                gap_window_offer(&window1, t, prev_end, UINT64_MAX);
                if (fclose(f) != 0) {
                    stage = "gap_close";
                    err = errno;
                    goto report;
                }
                {
                    int fatal = 0;
                    err = 0;
                    stage = "gap_empty";
                    if (have_window2) {
                        m = mirror_map_in_gap(target, &window2, &stage, &err, &fatal);
                        if (m != NULL) {
                            mirror_slot = 1;
                            mirror_base = (uintptr_t)m;
                            return m;
                        }
                        if (fatal)
                            goto report;
                    }
                    m = mirror_map_in_gap(target, &window1, &stage, &err, &fatal);
                    if (m != NULL)
                        return m;
                    goto report;
                }

scan_done:
                fclose(f);
                goto report;
            }
        }
    }

    stage = "gap_window";
    err = 0;

report:
    near_alloc_report(stage, t, err);
    return NULL;
}

void locate_rw_mapping(const void *object)
{
    uintptr_t addr = (uintptr_t)object & 0xffffffffffffffULL;
    char line[512];
    unsigned long long lo, hi;
    char perms[8];
    int budget;

    rw_mapping_start = 0;
    rw_mapping_end = 0;
    if (addr < 0x10000)
        return;
    FILE *f = fopen("/proc/self/maps", "r");
    if (f == NULL)
        return;
    budget = 0x1000;
    while (budget-- > 0) {
        if (fgets(line, sizeof line, f) == NULL)
            break;
        if (sscanf(line, "%llx-%llx %4s", &lo, &hi, perms) == 3
            && lo <= addr + 0xa8 && addr + 0xb8 <= hi
            && perms[0] == 'r' && perms[1] == 'w') {
            rw_mapping_start = lo;
            rw_mapping_end = hi;
            break;
        }
    }
    fclose(f);
}

int locate_unique_mapping(uintptr_t addr, uintptr_t out[6])
{
    char line[UNIQUE_MAPS_LINE_MAX];
    maps_entry_t found;
    maps_entry_t entry;
    uint32_t lines = 0;
    uint64_t total = 0;
    uint32_t matches = 0;
    int bad = 0;
    int ferr;
    size_t len;

    if (out == NULL)
        return 0;
    if (addr != g_game_base + 0x123e0c8)
        return 0;
    FILE *f = fopen("/proc/self/maps", "re");
    if (f == NULL)
        return 0;
    for (;;) {
        if (fgets(line, UNIQUE_MAPS_LINE_MAX, f) == NULL)
            break;
        len = strlen(line);
        if (len == 0 || total + len > MAPS_MAX_BYTES) {
            bad = 1;
            break;
        }
        total += len;
        if (line[len - 1] != '\n') {
            bad = 1;
            break;
        }
        if (parse_maps_line(line, g_maps_parse_fmt, &entry)) {
            if (entry.start <= addr && addr + 8 <= entry.end) {
                found = entry;
                matches++;
            }
        }
        lines++;
        if (lines >= MAPS_MAX_LINES) {
            bad = 1;
            break;
        }
    }
    ferr = ferror(f);
    fclose(f);
    if (bad || ferr || matches != 1)
        return 0;
    memcpy(out, &found, sizeof found);
    return 1;
}

static int branding_abi_ready(void)
{
    if (!verify_code_sha256(branding_abi_hashes[0].offset, 0x20, branding_abi_hashes[0].sha256))
        return 0;
    if (!verify_code_sha256(branding_abi_hashes[1].offset, 0x20, branding_abi_hashes[1].sha256))
        return 0;
    if (!verify_code_sha256(branding_abi_hashes[2].offset, 0x20, branding_abi_hashes[2].sha256)) {
        if (branding_linker_validate == NULL) {
            uintptr_t probe[2] = { 0, 0 };
            dl_iterate_phdr(branding_phdr_scan, probe);
            if (probe[0] != 1 || probe[1] == 0)
                return 0;
            branding_linker_validate = (int (*)(uintptr_t))probe[1];
        }
        if (branding_linker_validate(g_game_base) != 1)
            return 0;
    }
    for (int i = 3; i < 13; i++) {
        if (!verify_code_sha256(branding_abi_hashes[i].offset, 0x20,
                                branding_abi_hashes[i].sha256))
            return 0;
    }
    return 1;
}

void branding_tick(uintptr_t tick_ctx)
{
    uintptr_t h;
    uint64_t now;
    uint32_t slot;
    uintptr_t vt1 = 0;
    uintptr_t players = 0;
    uintptr_t player = 0;
    uintptr_t aux = 0;
    uintptr_t check = 0;
    uintptr_t tok = 0;
    int32_t meta = -1;
    int own;
    name_header_t hdr;
    char raw_name[BRAND_NAME_MAX + 8];
    uintptr_t game_str[2];
    int verified;

    if (tick_ctx == 0)
        return;
    h = runtime_read_handle();
    if (h == 0)
        return;

    if (branding_abi_state != 1) {
        if (branding_abi_state != 0)
            return;
        branding_abi_state = -1;
        if (!branding_abi_ready()) {
            if (branding_once_bits & 1)
                return;
            branding_once_bits |= 1;
            if (++branding_attempts > BRAND_ATTEMPT_MAX)
                return;
            log_event("branding", "abi_refused", NULL);
            return;
        }
        branding_abi_state = 1;
    }

    now = *(const uint64_t *)(const void *)(tick_ctx + 0x28);
    if (branding_throttle_until != 0 && now < branding_throttle_until)
        return;
    branding_throttle_until = now + BRAND_THROTTLE_US;

    slot = 0xffffffffu;
    if (!mem_read(h, g_world_ctx + 0xe0, &slot, 4)
        || slot > 0x3f || (int32_t)slot != g_world_index) {
        if (branding_once_bits & 2)
            return;
        branding_once_bits |= 2;
        if (++branding_attempts > BRAND_ATTEMPT_MAX)
            return;
        log_event("branding", "own_identity_refused", NULL);
        return;
    }

    if (g_world_token == 0
        || !mem_read(h, g_world_token + 0x20, &vt1, 8)
        || vt1 < 0x10000 || (vt1 & 7) != 0
        || !mem_read(h, vt1, &check, 8) || check != g_game_base + 0x11be3f0
        || !mem_read(h, vt1 + 0x10, &tok, 8)
        || tok < 0x10000 || tok > UINT64_MAX - 0x2000 || (tok & 7) != 0
        || tok != g_world_token
        || !mem_read(h, vt1 + 0x590, &players, 8)
        || players < 0x10000 || (players & 7) != 0
        || !mem_read(h, players, &check, 8) || check != g_game_base + 0x11ad208
        || (player = ((uintptr_t (*)(uintptr_t, const char *))
                      (g_game_base + 0x5d8ad4))(players, "player_name")) == 0
        || !mem_read(h, player, &check, 8) || check != g_game_base + 0x11abad8
        || !mem_read(h, player + 0x38, &aux, 8)
        || !mem_read(h, player + 0x40, &meta, 4)) {
        goto nameplate_fail;
    }

    own = branding_identity_fast(player, players);
    if (own == 0 && aux == 0 && meta == -1) {
        uint16_t count = 0;
        uintptr_t list = 0;
        own = 0;
        if (mem_read(h, players + 0xc0, &count, 2)
            && count >= 1 && count < 0x201
            && mem_read_array(players + 0x90, &list)) {
            uint32_t hits = 0;
            for (uint32_t i = 0; i < count; i++) {
                uintptr_t p = 0;
                if (!mem_read(h, list + (uintptr_t)i * 8, &p, 8))
                    goto nameplate_fail;
                if (p == player)
                    hits++;
            }
            own = (hits == 1);
        }
    }
    if (own == 0) {
        branding_report_refused(8, "name_read_refused");
        return;
    }

    if (!mem_read(h, player + 0xe0, &hdr, 0x10)
        || hdr.len == 0 || hdr.len >= BRAND_NAME_MAX) {
        branding_report_refused(8, "name_read_refused");
        return;
    }
    {
        uintptr_t src = (hdr.len > BRAND_SSO_THRESHOLD) ? hdr.ptr : player + 0xe8;
        if (src == 0 || !mem_read(h, src, raw_name, hdr.len)) {
            branding_report_refused(8, "name_read_refused");
            return;
        }
        if (memchr(raw_name, 0, hdr.len) == NULL)
            raw_name[hdr.len] = 0;
    }

    if (branding_token != g_profile_token || branding_vt1 != vt1
        || branding_vt2 != players || branding_player != player) {
        branding_reset();
        memset(branding_avatar_cache, 0, sizeof branding_avatar_cache);
        branding_player = player;
        branding_token = g_profile_token;
        branding_vt1 = vt1;
        branding_vt2 = players;
        branding_name_valid = mem_read(h, player + 0x80, &branding_player_id, 4);
    }

    if (branding_last_name[0] == 0 || strcmp(raw_name, branding_last_name) != 0) {
        const char *clean = branding_sanitize_name(raw_name);
        if (clean == NULL || strlen(clean) > 0x7f)
            return;
        snprintf(branding_name, sizeof branding_name, "%s", clean);
    }
    if (!branding_name_valid)
        return;

    pthread_once(&branding_once_control, branding_once_init);
    verified = 0;
    if (branding_verify != NULL)
        verified = (branding_verify(0) == 1);

    if (strcmp(raw_name, branding_name) != 0) {
        ((void (*)(void *, const char *))(g_game_base + 0x66ae58))(game_str, branding_name);
        ((void (*)(uintptr_t, void *))(g_game_base + 0x5921a0))(player, game_str);
        ((void (*)(void *))(g_game_base + 0x66ad48))(game_str);
        branding_attempts++;
        if (branding_attempts <= BRAND_ATTEMPT_MAX)
            log_event("branding", "name_restored", NULL);
    }
    snprintf(branding_last_name, sizeof branding_last_name, "%s", branding_name);

    if (!branding_avatar_ok(players, player))
        return;
    branding_finish(verified, now);
    if (!branding_image_check(*(const uintptr_t *)(const void *)branding_avatar_cache,
                              players)
        && !branding_image_refused) {
        *(uintptr_t *)(void *)branding_avatar_cache = 0;
        if (branding_image_create(players) == 0) {
            branding_image_refused = 1;
            branding_attempts++;
            if (branding_attempts <= BRAND_ATTEMPT_MAX)
                log_event("branding", "image_refused", NULL);
        } else {
            branding_attempts++;
            if (branding_attempts <= BRAND_ATTEMPT_MAX)
                log_event("branding", "image_created", NULL);
        }
    }
    branding_post(player, verified);
    return;

nameplate_fail:
    if (branding_once_bits & 4)
        return;
    branding_once_bits |= 4;
    if (++branding_attempts > BRAND_ATTEMPT_MAX)
        return;
    log_event("branding", "nameplate_refused", NULL);
}

void map_wall_scan_tick(uintptr_t world, uintptr_t expected_map, uint32_t frame,
                        int refresh)
{
    uint64_t t0 = monotonic_us();
    uint64_t reads0 = g_mem_reads;
    uint32_t prev_stage = map_failure_stage;
    uintptr_t map_ptr = 0;
    uintptr_t tiles = 0;
    uint32_t w = 0;
    uint32_t h = 0;
    uint32_t stage;
    uint32_t index = 0;
    uintptr_t h2 = 0;
    char json[600];
    uint64_t now_ms;
    uintptr_t handle;

    if (refresh == 0) {
        stage = 1;
        goto finish_reset;
    }

    handle = runtime_read_handle();
    if (!mem_read(handle, world + 0xf8, &map_ptr, 8)) {
        stage = 2;
        goto finish_reset;
    }
    if (map_ptr < 0x10000 || (map_ptr & 7) != 0 || map_ptr != expected_map) {
        stage = 2;
        goto finish_reset;
    }
    if (!mem_read(handle, map_ptr + 0x20, &tiles, 8)) {
        stage = 3;
        goto finish_reset;
    }
    if (tiles < 0x10000 || (tiles & 7) != 0) {
        stage = 3;
        goto finish_reset;
    }
    if (!mem_read(handle, map_ptr + 0xc4, &h, 4)) {
        stage = 3;
        goto finish_reset;
    }
    if (!mem_read(handle, map_ptr + 0xc8, &w, 4)) {
        stage = 3;
        goto finish_reset;
    }
    if (h < 8 || w < 8 || h > MAP_TILE_MAX || w > MAP_TILE_MAX) {
        stage = 3;
        goto finish_reset;
    }

    {
        int changed = (map_ptr != map_cached_map) || (tiles != map_cached_tiles)
                      || (h != map_cached_h) || (w != map_cached_w);

        if (map_scan_active == 1) {
            if (map_ptr != map_scan_map || tiles != map_scan_tiles
                || h != map_scan_h || w != map_scan_w) {
                map_scan_active = 0;
                map_scan_progress = 0;
                goto start_scan;
            }
        } else {
start_scan:
            if ((!changed && map_known != 0 && frame - map_change_frame < 30u)
                || ((changed | map_known) != 1 && map_change_frame != 0
                    && frame - map_change_frame < 5u))
                return;
            if (changed)
                map_known = 0;
            map_scan_map = map_ptr;
            map_tile_count = w * h;
            map_scan_tiles = tiles;
            map_scan_h = h;
            map_scan_w = w;
            map_scan_progress = 0;
            map_scanned_frame = UINT32_MAX;
            map_read_us_acc = 0;
            map_read_count_acc = 0;
            map_scan_active = 1;
            map_change_frame = frame;
        }
    }

    if (map_scanned_frame == frame)
        return;

    {
        uint32_t from = map_scan_progress;
        uint32_t to = map_scan_progress + MAP_CHUNK;
        if (map_tile_count < to)
            to = map_tile_count;
        map_scanned_frame = frame;
        for (uint32_t i = from; i < to; i++) {
            uintptr_t tile = 0;
            uint32_t wall = 0;
            if (!mem_read(handle, tiles + (uintptr_t)i * 8, &tile, 8)) {
                stage = 4;
                index = i;
                goto finish_reset;
            }
            if (tile < 0x10000 || (tile & 7) != 0) {
                stage = 4;
                index = i;
                goto finish_reset;
            }
            if (!mem_read(handle, tile + 0x54, &wall, 4)) {
                stage = 5;
                index = i;
                goto finish_reset;
            }
            map_wall_bitmap[i] = (uint8_t)(((wall & 0xff000000u) != 0) << 6
                                           | ((wall & 0x00ff0000u) != 0) << 7);
        }
        map_scan_progress = to;
        map_read_us_acc += source_now_us() - t0;
        map_read_count_acc += g_mem_reads - reads0;
        if (map_scan_progress < map_tile_count)
            return;
    }

    if (!mem_read_array(map_ptr + 0x20, &h2) || h2 != tiles
        || !mem_read(handle, map_ptr + 0xc4, &h, 4) || h != map_scan_h
        || !mem_read(handle, map_ptr + 0xc8, &w, 4) || w != map_scan_w) {
        stage = 7;
        goto finish_reset;
    }
    memcpy(source_wall_cache, map_wall_bitmap, map_tile_count);
    map_cached_tiles = tiles;
    map_cached_map = map_ptr;
    map_cached_h = h;
    map_cached_w = w;
    map_known = 1;
    map_refresh_us = map_read_us_acc;
    map_scan_active = 0;
    map_complete_frame = frame;
    map_cache_publish();
    stage = 0;
    goto finish;

finish_reset:
    map_scan_active = 0;
    map_read_count_acc += g_mem_reads - reads0;
    map_refresh_us = map_read_us_acc + (monotonic_us() - t0);
    map_scan_progress = 0;
    map_read_us_acc = map_refresh_us;
    if (map_ptr != map_cached_map || tiles != map_cached_tiles
        || h != map_cached_h || w != map_cached_w)
        map_known = 0;

finish:
    map_attempts++;
    map_failure_stage = stage;
    map_failure_index = index;
    now_ms = monotonic_ms();
    if (map_attempts < 5 || stage != prev_stage || map_last_log_ms == 0
        || now_ms - map_last_log_ms > 999) {
        snprintf(json, sizeof json,
                 ",\"map_known\":%u,\"failure_stage\":%u,\"failure_index\":%u,"
                 "\"width\":%d,\"height\":%d,\"map\":\"0x%llx\",\"tiles\":\"0x%llx\","
                 "\"refresh_us\":%llu,\"refresh_reads\":%llu,\"refresh_attempts\":%llu",
                 (unsigned)map_known, (unsigned)stage, (unsigned)index,
                 (int)w, (int)h, (unsigned long long)map_ptr,
                 (unsigned long long)tiles, (unsigned long long)map_refresh_us,
                 (unsigned long long)map_read_count_acc,
                 (unsigned long long)map_attempts);
        log_event("function_map",
                  stage != 0 ? "current_map_unknown" : "complete_source_wall_cache",
                  json);
        map_last_log_ms = now_ms;
    }
}

float alternator_damage_scale(uintptr_t token_slot)
{
    float result = 1.0f;
    char namebuf[96];
    uintptr_t obj = 0;
    uintptr_t inner = 0;
    uintptr_t handle = projectile_read_handle(token_slot, &obj);

    if (handle != 0
        && mem_read(handle, obj, &inner, 8)
        && inner >= 0x10000 && (inner & 7) == 0
        && resolve_weapon_name(g_game_base, inner, mem_read, 0, namebuf)) {
        const char *name = namebuf + 20;
        int lo = 0;
        int hi = ALTERNATOR_TABLE_LEN;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            int c = strcmp(name, alternator_damage_table[mid].name);
            if (c == 0) {
                float v = (float)alternator_damage_table[mid].value;
                float t = v / 3.0f;
                float capped = (t < 20.0f) ? t : 20.0f;
                result = (t >= 1.0f) ? capped : 1.0f;
                break;
            }
            if (c > 0)
                lo = mid + 1;
            else
                hi = mid;
        }
    }
    return result;
}

static const weapon_desc_t *weapon_table_lookup(const char *name)
{
    int lo = 0;
    int hi = WEAPON_TABLE_LEN;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        int c = strcmp(name, weapon_table[mid].name);
        if (c == 0)
            return &weapon_table[mid];
        if (c > 0)
            lo = mid + 1;
        else
            hi = mid;
    }
    return NULL;
}

int weapon_controller_read(const game_reader_t *reader, uintptr_t obj,
                           weapon_info_t *out)
{
    uintptr_t vt = 0;
    uintptr_t inner = 0;
    uintptr_t mid = 0;
    uintptr_t holder = 0;
    uintptr_t entries = 0;
    uintptr_t tail = 0;
    uintptr_t stats_holder = 0;
    uintptr_t stats = 0;
    uintptr_t active;
    int32_t count = 0;
    name_header_t hdr;
    uint32_t len;
    const weapon_desc_t *entry;
    uintptr_t ver_addr;
    uint32_t ver = 0;
    uint8_t stats_byte = 0;
    target_arrays_t arr;
    uintptr_t targets[TARGET_LIST_MAX];
    uint32_t ntargets = 0;
    uint32_t max_clear = 0;
    uint32_t flagged;
    uint32_t elite;
    float speed;

    if (obj < 0x10000 || reader->read == NULL)
        return 0;
    if (!reader->read(reader->ctx, obj, &vt, 8))
        return 0;
    if (vt < 0x10000 || (vt & 7) != 0 || vt != reader->game_base + 0x121f350)
        return 0;
    if (!reader->read(reader->ctx, obj + 8, &inner, 8))
        return 0;
    if (inner < 0x10000 || (inner & 7) != 0)
        return 0;
    if (!reader->read(reader->ctx, inner + 8, &count, 4))
        return 0;
    if (count < 0 || count > CONTROLLER_ENTRY_MAX)
        return 0;
    if (!reader->read(reader->ctx, inner, &mid, 8))
        return 0;
    if (mid < 0x10000 || (mid & 7) != 0)
        return 0;
    if (!reader->read(reader->ctx, mid + 0x38, &holder, 8))
        return 0;
    if (holder < 0x10000 || (holder & 7) != 0)
        return 0;
    if (!reader->read(reader->ctx, holder, &entries, 8))
        return 0;
    if (entries < 0x10000 || (entries & 7) != 0)
        return 0;
    if (!reader->read(reader->ctx, entries + 8, &tail, 8))
        return 0;
    if (tail < 0x10000 || (tail & 7) != 0)
        return 0;

    if (tail > UINT64_MAX - (uintptr_t)count * 0x10 - 0x10)
        return 0;
    active = tail + (uintptr_t)count * 0x10;
    if (active < 0x10000)
        return 0;
    if (!reader->read(reader->ctx, active, &hdr, 0x10))
        return 0;
    len = hdr.len;
    if (len < WEAPON_MIN_LEN || len > WEAPON_MAX_LEN)
        return 0;
    if (len < BRAND_SSO_THRESHOLD) {
        memcpy(out->name, &hdr.ptr, 8);
    } else {
        if (hdr.ptr < 0x10000 || hdr.ptr > UINT64_MAX - len)
            return 0;
        if (!reader->read(reader->ctx, hdr.ptr, out->name, len + 1))
            return 0;
    }
    if (out->name[len] != '\0')
        return 0;
    for (uint32_t i = 0; i < len; i++) {
        char c = out->name[i];
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
              || c == '_' || (c >= '0' && c <= '9')))
            return 0;
    }
    if (strstr(out->name, "Weapon") == NULL)
        return 0;
    entry = weapon_table_lookup(out->name);
    if (entry == NULL)
        return 0;

    ver_addr = reader->game_base + 0x1244c3c;
    if (ver_addr < 0x10000 || ver_addr > UINT64_MAX - 3)
        return 0;
    if (!reader->read(reader->ctx, ver_addr, &ver, 4) || ver != WEAPON_VERSION_EXPECTED)
        return 0;
    if (!mem_read_ptr(reader, holder + 0x1a0, &stats_holder))
        return 0;
    if (!mem_read_ptr(reader, stats_holder + 0x28, &stats))
        return 0;
    if (stats + (uintptr_t)count + 1 < 0x10001)
        return 0;
    if (!reader->read(reader->ctx, stats + (uintptr_t)count, &stats_byte, 1))
        return 0;

    if (!reader->read(reader->ctx, obj + 0x58, &arr, 0x28))
        return 0;
    if (arr.count1 > TARGET_LIST_MAX)
        return 0;
    if (arr.cap1 < (int32_t)arr.count1 || arr.cap1 > 0x400)
        return 0;
    if (arr.count1 != 0) {
        if (arr.list1 < 0x10000 || (arr.list1 & 7) != 0)
            return 0;
        if (arr.list1 > UINT64_MAX - (uint64_t)arr.count1 * 8)
            return 0;
        for (uint32_t i = 0; i < arr.count1; i++) {
            uintptr_t t = 0;
            if (!mem_read_ptr(reader, arr.list1 + (uintptr_t)i * 8, &t))
                return 0;
            if (!targets_push(targets, &ntargets, t))
                return 0;
        }
    }
    if (arr.count2 > TARGET_LIST_MAX)
        return 0;
    if (arr.cap2 < (int32_t)arr.count2 || arr.cap2 > 0x400)
        return 0;
    if (arr.count2 != 0) {
        if (arr.list2 < 0x10000 || (arr.list2 & 7) != 0)
            return 0;
        if (arr.list2 > UINT64_MAX - (uint64_t)arr.count2 * 8)
            return 0;
        for (uint32_t i = 0; i < arr.count2; i++) {
            uintptr_t t = 0;
            if (!mem_read_ptr(reader, arr.list2 + (uintptr_t)i * 8, &t))
                return 0;
            if (!targets_push(targets, &ntargets, t))
                return 0;
        }
    }
    if (arr.extra != 0) {
        if (arr.extra < 0x10000 || arr.extra > UINT64_MAX - 0x1000
            || (arr.extra & 7) != 0)
            return 0;
        if (!targets_push(targets, &ntargets, arr.extra))
            return 0;
    }

    flagged = (ntargets != 0);
    for (uint32_t i = 0; i < ntargets; i++) {
        uintptr_t t = targets[i];
        uintptr_t tvt = 0;
        uint32_t dmg = 0;
        uint8_t f1 = 0;
        uint8_t f2 = 0;
        uint8_t f3 = 0;
        uint32_t m;
        if (!mem_read_ptr(reader, t, &tvt))
            return 0;
        if (tvt != reader->game_base + 0x121ea60)
            return 0;
        if (t < 0x10000 - 0x130 || t > UINT64_MAX - 0x130)
            return 0;
        if (!reader->read(reader->ctx, t + 0x130, &dmg, 4))
            return 0;
        if (dmg > WEAPON_DAMAGE_MAX)
            return 0;
        if (!reader->read(reader->ctx, t + 0x108, &f1, 1))
            return 0;
        if (!reader->read(reader->ctx, t + 0x1e5, &f2, 1))
            return 0;
        if (!reader->read(reader->ctx, t + 0x23c, &f3, 1))
            return 0;
        if (f1 > 1 || f2 > 1 || f3 > 1)
            return 0;
        m = (dmg > max_clear) ? dmg : max_clear;
        if (f1 == 0 && f2 == 0 && f3 == 0) {
            flagged = 0;
            max_clear = m;
        }
    }
    elite = flagged;
    if (stats_byte == 1)
        elite = 1;
    if (max_clear != entry->damage || entry->elite != (uint8_t)elite)
        return 0;

    out->damage = max_clear;
    out->elite = elite;
    out->obj = obj;
    out->valid = 1;
    out->targets = ntargets;
    speed = bits_to_float(WEAPON_SPEED_DEFAULT_BITS);
    if (max_clear != 0) {
        float f = (float)max_clear / 300.0f;
        if (f >= 0.08f && f < 1.25f)
            speed = f;
    }
    out->speed_scale = speed;
    return 1;
}

int controller_pair_register(controller_state_t *state, const game_reader_t *reader,
                             uintptr_t a, uintptr_t b)
{
    char name[64];
    uintptr_t vt = 0;
    int slot = -1;

    if (state == NULL || reader == NULL || reader->read == NULL)
        return 0;
    if (a < 0x10000)
        return 0;
    if (!reader->read(reader->ctx, a, &vt, 8))
        return 0;
    if (vt < 0x10000 || (vt & 7) != 0)
        return 0;
    if (!mem_read_cstr(reader, a, name))
        return 0;

    if (strcmp(name, controller_kind_names[0]) == 0)
        slot = 0;
    else if (strcmp(name, controller_kind_names[1]) == 0)
        slot = 1;

    if (slot >= 0
        && mem_read_cstr(reader, b, name)
        && strcmp(name, controller_kind_names[slot]) == 0) {
        state->generation = 0;
        state->slot[slot].obj = a;
        state->slot[slot].vtable = vt;
        state->slot[slot].pair = b;
    }

    if (state->speed_obj == 0
        && mem_read_cstr(reader, b, name)
        && strcmp(name, "SpeedProjectile") == 0)
        state->speed_obj = b;

    return slot >= 0;
}

void controller_projectiles_collect(controller_state_t *state,
                                    const game_reader_t *reader,
                                    uint32_t mode, uintptr_t *out)
{
    char name[64];

    if (state == NULL || reader == NULL || out == NULL)
        return;
    if (mode >= 2)
        return;
    if (state->slot[0].obj == 0)
        return;
    if (state->slot[1].obj == 0)
        return;
    if (state->slot[0].obj == state->slot[1].obj)
        return;
    if (mode != 0) {
        if (state->speed_obj == 0)
            return;
        if (!mem_read_cstr(reader, state->speed_obj, name))
            return;
        if (strcmp(name, "SpeedProjectile") != 0)
            return;
    }

    for (int slot = 0; slot < 2; slot++) {
        uintptr_t obj = state->slot[slot].obj;
        uintptr_t vt = 0;
        uintptr_t active = 0;

        if (obj + 8 < 0x10008 || reader->read == NULL)
            return;
        if (!reader->read(reader->ctx, obj, &vt, 8))
            return;
        if (vt < 0x10000 || (vt & 7) != 0)
            return;
        if (vt != state->slot[slot].vtable)
            return;
        if (!mem_read_cstr(reader, obj, name))
            return;
        if (strcmp(name, controller_kind_names[slot]) != 0)
            return;
        if (!mem_read_cstr(reader, state->slot[slot].pair, name))
            return;
        if (strcmp(name, controller_kind_names[slot]) != 0)
            return;
        if (obj + 0x70 < 0x10008)
            return;
        if (!reader->read(reader->ctx, obj + 0x68, &active, 8))
            return;
        if (active < 0x10000 || (active & 7) != 0)
            return;
        if (active != state->slot[slot].pair && active != state->speed_obj)
            return;
        out[slot] = (mode == 0) ? state->slot[slot].pair : state->speed_obj;
    }
}
