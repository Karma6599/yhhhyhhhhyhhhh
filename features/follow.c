#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#define UI_QUERY_COUNT     118
#define UI_ACTION_COUNT    168
#define UI_PROVIDER_MAX    8
#define UI_QUERY_MAGIC     0x18
#define UI_QUERY_ID_BASE   0x10000

typedef struct {
    const char *name;
    int32_t     vmin;
    int32_t     vmax;
    uint32_t    default_value;
    uint32_t    flags;
} ui_query_slot_t;

typedef struct {
    uint32_t size;
    uint32_t reserved0;
    uint32_t reserved1;
    int32_t  value;
    uint32_t status;
    uint16_t reserved2;
    uint8_t  valid;
    uint8_t  reserved3;
} ui_query_request_t;

typedef int (*ui_provider_query_fn)(void *ctx, uint32_t id, ui_query_request_t *req);
typedef int (*ui_provider_apply_fn)(void *ctx, const int32_t *values, uint32_t count);

typedef struct {
    void                *ctx;
    uint64_t             mask[3];
    ui_provider_query_fn query;
    ui_provider_apply_fn apply;
    uint64_t             reserved[2];
} ui_provider_slot_t;

typedef struct {
    uint64_t    id;
    const char *enum_name;
    const char *label;
    const char *description;
    const char *query_key;
    uint64_t    reserved;
    const char *wire_alias;
    int32_t     query_slot;
    int32_t     aux;
    int32_t     terminator;
    uint32_t    kind;
} ui_action_record_t;

_Static_assert(sizeof(ui_query_request_t) == 0x18, "ui_query_request_t size");
_Static_assert(sizeof(ui_provider_slot_t) == 0x40, "ui_provider_slot_t size");
_Static_assert(sizeof(ui_action_record_t) == 0x48, "ui_action_record_t size");

extern const ui_action_record_t g_ui_action_records[UI_ACTION_COUNT];
extern void ui_action_value_refresh(const ui_action_record_t *record);
extern int  ui_action_query_slot(const ui_action_record_t *record);
extern bool ui_key_is_local(const char *name);

static const ui_query_slot_t g_ui_query_slots[UI_QUERY_COUNT] = {
    { "nexus_sx_spin",                   0,     1,   0, 1 },
    { "nexus_sx_follow",                 0,     1,   0, 1 },
    { "nexus_sx_follow_team",            0,     1,   0, 1 },
    { "nexus_sx_pin",                    0,     1,   0, 1 },
    { "nexus_sx_spray",                  0,     1,   0, 1 },
    { "nexus_sx_anti_afk",               0,     1,   0, 1 },
    { "nexus_sx_speed_exploit",          0,     1,   0, 1 },
    { "nexus_sx_speed_local_move",       0,     1,   0, 0x101 },
    { "nexus_sx_fly",                    0,     1,   0, 0x101 },
    { "nexus_sx_kit_nani",               0,     1,   0, 1 },
    { "nexus_sx_kolt_mod",               0,     1,   0, 1 },
    { "nexus_sx_bolt_mod",               0,     1,   0, 1 },
    { "nexus_sx_bolt_auto_attack",       0,     1,   1, 1 },
    { "nexus_sx_bolt_wall_avoid",        0,     1,   1, 1 },
    { "nexus_sx_bolt_prediction",        0,     1,   1, 1 },
    { "nexus_sx_bolt_safe_exit",         0,     1,   1, 1 },
    { "nexus_social_spectate_as_brawltv", 0,    1,   0, 1 },
    { "nexus_sx_xray",                   0,     1,   0, 1 },
    { "nexus_sx_xray_show_name",         0,     1,   0, 1 },
    { "nexus_sx_aim",                    0,     1,   0, 1 },
    { "nexus_sx_predict",                0,     1,   1, 1 },
    { "nexus_sx_esp",                    0,     1,   0, 1 },
    { "nexus_sx_character_outline",      0,     1,   1, 1 },
    { "nexus_vis_attack_range",          0,     1,   0, 1 },
    { "nexus_vis_hitbox",                0,     1,   0, 1 },
    { "nexus_vis_enemy_tracer",          0,     1,   0, 1 },
    { "nexus_vis_trophies",              0,     1,   0, 1 },
    { "nexus_sx_server_ip",              0,     1,   0, 1 },
    { "nexus_sx_aura",                   0,     1,   0, 1 },
    { "nexus_sx_aura_main",              0,     1,   1, 1 },
    { "nexus_sx_aura_gadget",            0,     1,   0, 1 },
    { "nexus_sx_aura_super",             0,     1,   0, 1 },
    { "nexus_sx_aura_nowall",            0,     1,   0, 1 },
    { "nexus_sx_aura_noball",            0,     1,   0, 1 },
    { "nexus_autododge_enabled",         0,     1,   0, 1 },
    { "nexus_autododge_require_hold",    0,     1,   0, 1 },
    { "nexus_autofarm_enabled",          0,     1,   0, 1 },
    { "nexus_auto_play_again_enabled",   0,     1,   0, 1 },
    { "nexus_autofarm_attack_enemies",   0,     1,   1, 1 },
    { "nexus_autofarm_show_stats",       0,     1,   1, 1 },
    { "nexus_quick_menu_disabled",       0,     1,   0, 1 },
    { "nexus_sx_ball_assist",            0,     1,   0, 1 },
    { "nexus_sx_hold_to_shoot",          0,     1,   0, 1 },
    { "nexus_sx_dyna_jump",              0,     1,   0, 1 },
    { "nexus_sx_hold_aim",               0,     1,   0, 1 },
    { "nexus_sx_hold_range_check",       0,     1,   0, 1 },
    { "nexus_sx_spin_speed",             0, 62832, 785, 0 },
    { "nexus_sx_spin_radius",            0,   300,  25, 0 },
    { "nexus_sx_follow_distance",      100,  5000, 250, 0 },
    { "nexus_sx_pin_interval",         100,  5000, 1000, 0 },
    { "nexus_sx_spray_interval",       100,  5000, 1000, 0 },
    { "nexus_sx_xray_target_mode",       1,     2,   1, 0 },
    { "nexus_sx_aim_target_mode",        1,     2,   1, 0 },
    { "nexus_sx_predict_version",        1,     2,   1, 0 },
    { "nexus_sx_aop_projectile_speed", 500,  9000, 3200, 0 },
    { "nexus_sx_aop_lead_scale",         0,   200, 100, 0 },
    { "nexus_sx_aop_max_range",        500, 12000, 9000, 0 },
    { "nexus_sx_aop_reaction_ms",        0,   500,   0, 0 },
    { "nexus_sx_aop_target_mode",        0,     2,   0, 0 },
    { "nexus_sx_combat_aura_range",    500, 12000, 4200, 0 },
    { "nexus_sx_combat_dodge_range",   300,  3000, 900, 0 },
    { "nexus_sx_combat_fire_interval",  50,  2000, 1000, 0 },
    { "nexus_sx_killaura_health_pct",   0,   100,   0, 0 },
    { "nexus_dodge_reaction_pct",      50,   180, 180, 0 },
    { "nexus_dodge_safety_pct",        50,   180, 100, 0 },
    { "nexus_dodge_power_pct",         60,   140, 100, 0 },
    { "nexus_dodge_distance_pct",      70,   140, 100, 0 },
    { "nexus_dodge_commit_pct",        50,   150, 100, 0 },
    { "nexus_dodge_burst_hold_ms",   300,  1600, 1060, 0 },
    { "nexus_v2_dodge_reaction_pct",   50,   180, 180, 0 },
    { "nexus_v2_dodge_safety_pct",     50,   180, 100, 0 },
    { "nexus_v2_dodge_power_pct",      60,   140, 100, 0 },
    { "nexus_v2_dodge_distance_pct",   70,   140, 100, 0 },
    { "nexus_v2_dodge_commit_pct",     50,   150, 100, 0 },
    { "nexus_v2_dodge_burst_hold_ms",300,  1600, 1060, 0 },
    { "nexus_v3_dodge_reaction_pct",   50,   180, 180, 0 },
    { "nexus_v3_dodge_safety_pct",     50,   180, 100, 0 },
    { "nexus_v3_dodge_power_pct",      60,   140, 100, 0 },
    { "nexus_v3_dodge_distance_pct",   70,   140, 100, 0 },
    { "nexus_v3_dodge_commit_pct",     50,   150, 100, 0 },
    { "nexus_v3_dodge_burst_hold_ms",300,  1600, 1060, 0 },
    { "nexus_v4_dodge_reaction_pct",   50,   180, 180, 0 },
    { "nexus_v4_dodge_safety_pct",     50,   180, 100, 0 },
    { "nexus_v4_dodge_power_pct",      60,   140, 100, 0 },
    { "nexus_v4_dodge_distance_pct",   70,   140, 100, 0 },
    { "nexus_v4_dodge_commit_pct",     50,   150, 100, 0 },
    { "nexus_v4_dodge_burst_hold_ms",300,  1600, 1060, 0 },
    { "nexus_v5_dodge_reaction_pct",   50,   180, 180, 0 },
    { "nexus_v5_dodge_safety_pct",     50,   180, 100, 0 },
    { "nexus_v5_dodge_power_pct",      60,   140, 100, 0 },
    { "nexus_v5_dodge_distance_pct",   70,   140, 100, 0 },
    { "nexus_v5_dodge_commit_pct",     50,   150, 100, 0 },
    { "nexus_v5_dodge_burst_hold_ms",300,  1600, 1060, 0 },
    { "nexus_dodge_blacklist_mask",     0,  1023, 1023, 0 },
    { "nexus_autododge_version",        1,     5,   3, 0 },
    { "nexus_sx_hold_delay_ms",         0,  1000,  50, 0 },
    { "nexus_sx_ball_search_degrees",   5,   180,  90, 0 },
    { "nexus_sx_ball_bounce_limit",     0,     8,   3, 0 },
    { "nexus_autofarm_post_delay_ms",1000, 10000, 3500, 0 },
    { "nexus_autofarm_click_gap_ms",  500,  5000, 2000, 0 },
    { "nexus_autofarm_follow_target",   0,     1,   0, 0 },
    { "nexus_sx_bolt_smooth",          50,   100,  85, 0 },
    { "nexus_sx_bolt_wall_lookahead", 150,   600, 500, 0 },
    { "nexus_sx_bolt_target_range",  600,  2000, 1400, 0 },
    { "nexus_sx_bolt_exit_hold",      300,  1200, 650, 0 },
    { "nexus_sx_outline_opacity",       0,  1000, 1000, 0 },
    { "nexus_sx_outline_r",             0,  1000, 1000, 0 },
    { "nexus_sx_outline_g",             0,  1000, 1000, 0 },
    { "nexus_sx_outline_b",             0,  1000, 1000, 0 },
    { "nexus_sx_outline_color_preset",  0,     6,   0, 0 },
    { "nexus_quick_menu_style",         0,     5,   0, 0 },
    { "nexus_quick_menu_preset",        0,     3,   0, 0 },
    { "nexus_quick_menu_offset_x",   -400,   400,   0, 0 },
    { "nexus_quick_menu_offset_y",      0,   500,   0, 0 },
    { "nexus_sx_spin_online",           0,     1,   0, 1 },
    { "nexus_sx_spin_online_idle",      0,     1,   0, 1 },
    { "nexus_sx_aim_ultimate",          0,     1,   1, 1 },
    { "nexus_sx_aim_gadget",            0,     1,   1, 1 },
};

ui_provider_slot_t g_ui_provider_slots[UI_PROVIDER_MAX];
int32_t g_ui_query_values[UI_QUERY_COUNT];
uint64_t g_ui_query_revision;

void ui_provider_poll(void)
{
    for (int slot = 0; slot < UI_PROVIDER_MAX; slot++) {
        ui_provider_slot_t *prov = &g_ui_provider_slots[slot];
        if (prov->query == NULL)
            continue;

        for (int i = 0; i < UI_QUERY_COUNT; i++) {
            const ui_query_slot_t *entry = &g_ui_query_slots[i];
            if (ui_key_is_local(entry->name))
                continue;

            ui_query_request_t req;
            memset(&req, 0, sizeof req);
            req.size = UI_QUERY_MAGIC;

            if (prov->query(prov->ctx, UI_QUERY_ID_BASE + (uint32_t)i, &req) != 1)
                continue;
            if (req.size != UI_QUERY_MAGIC || req.valid == 0)
                continue;
            if (req.value < entry->vmin || req.value > entry->vmax)
                continue;
            if (g_ui_query_values[i] == req.value)
                continue;

            g_ui_query_values[i] = req.value;
            g_ui_query_revision++;
        }
    }

    ui_action_value_refresh(&g_ui_action_records[91]);
    for (int i = 0; i < UI_ACTION_COUNT; i++)
        if (i != 91)
            ui_action_value_refresh(&g_ui_action_records[i]);
}

int ui_values_set_key(int32_t *values, const char *key, int32_t value)
{
    for (int i = 0; i < UI_QUERY_COUNT; i++) {
        if (strcmp(g_ui_query_slots[i].name, key) != 0)
            continue;
        if (value < g_ui_query_slots[i].vmin || value > g_ui_query_slots[i].vmax)
            return 0;
        values[i] = value;
        return 1;
    }
    return 0;
}
