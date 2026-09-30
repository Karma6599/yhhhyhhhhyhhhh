#define _GNU_SOURCE

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#define TWEAK_KEY_COUNT 0x37
#define SETTINGS_MAX    0xdc
#define CHAT_LINES      10
#define CHAT_LINE_SIZE  0xc0
#define SERVER_SLOTS    0x20
#define SERVER_REC      0x48
#define DPS_SAMPLES     0x100

#define ROD_STATUS_HDR   (*(const uint64_t *)(uintptr_t)0x10e7f8)
#define ROD_CAPTURE_TAG  (*(const uint64_t *)(uintptr_t)0x10e628)
#define ROD_SPIN_STAMP   (*(const uint64_t *)(uintptr_t)0x10e670)
#define ROD_INPUT_TAG    (*(const uint64_t *)(uintptr_t)0x10e780)
#define ROD_FRAME_TAG    (*(const uint64_t *)(uintptr_t)0x10e580)
#define ROD_MOVE2_TAG    (*(const uint64_t *)(uintptr_t)0x10e590)
#define ROD_STATUS_TAG   (*(const uint64_t *)(uintptr_t)0x10e6a0)
#define ROD_JOURNAL_EMPTY ((const char *)(uintptr_t)0x117c20)
#define ROD_DEFAULT_NAME ((const char *)(uintptr_t)0x1e1311)

extern const char *g_tweak_keys[TWEAK_KEY_COUNT * 3];
extern int32_t g_tweak_values[0x38];
extern uint32_t g_tweak_limits[TWEAK_KEY_COUNT * 3];
extern const char *g_font_paths[6];
extern const char *g_env_table[0x2f4][6];
extern const char *g_legacy_names[3][7][0x30];

extern uintptr_t game_read(uintptr_t handle, uintptr_t addr, void *out,
                           uint32_t len);
extern int game_write(int fd, const void *buf, size_t len, uintptr_t addr);
extern void runtime_journal_write(const char *stage, const char *reason,
                                  const char *payload);
extern int game_region_verify(uint32_t rva, uint32_t size, const char *sha);
extern void *trampoline_alloc(uintptr_t target);
extern void icache_flush(void *start, void *end);
extern int game_deref_read(uintptr_t addr, void *out, uint32_t len);
extern int game_resolve(uintptr_t addr, uintptr_t *out);
extern int guarded_hook_install(uint32_t rva, uint32_t size, const char *sha,
                                uint32_t rva2, uint32_t original_word,
                                void *callback);
extern int guarded_observer_install(uint32_t rva, uint32_t size,
                                    const char *sha, void *callback,
                                    void *slot, int flags);
extern uint64_t frame_epoch_now(void);
extern int visual_pipeline_ready(void);
extern int main_thread_gate(void);
extern int battle_frame_ready(void);
extern int roster_entry_scan(uintptr_t entry, void *slot);
extern int entity_index_fetch(uintptr_t base, uintptr_t entity,
                              uintptr_t read_fn, int flags, void *out);
extern uint64_t server_refresh(void);
extern int server_entry_add(void *state, uint32_t id, uint32_t ping,
                            const char *name);
extern int font_asset_load(const char *path);
extern int font_entry_apply(uint32_t choice);
extern int font_mask_reload(void);
extern int owner_phdr_cb(void *info, void *data);
extern int queue_plan_verify(const void *plan, uint32_t word,
                             void *buf, size_t len);
extern int xray_input_publish(void *state, void *rec, void *out);
extern int xray_stage_scan(void *out);
extern int frame_gate_check(uintptr_t a, uintptr_t b);
extern int hud_record_build(void *ctx);
extern int hud_active_check(void *rec);
extern int hud_frame_gate(uintptr_t own, uintptr_t a, uintptr_t b,
                          uintptr_t c);
extern int respawn_timer_ready(uintptr_t ctx);
extern int arrow_find(uintptr_t parent, const char *name);
extern int arrow_color_build(float ratio, void *fn, void *out);
extern int default_skin_resolve(void);
extern int proxy_endpoint_decode(uintptr_t ep, void *out);
extern int proxy_handshake(void *ep, void *key, uint32_t len, int64_t skew,
                           void *out);
extern int proxy_tunnel(void *handshake, void *out);
extern int proxy_decode(void *tunnel, void *key, uint32_t *out);
extern int proxy_commit(void *session, uintptr_t ep, void *orig, uint32_t n,
                        void *key, uint32_t id);
extern int game_string_format(void *out, const char *fmt, ...);
extern int game_string_release(void *str);
extern void *game_font_load(const char *path, int a);
extern void *game_find_node(void *parent, const char *name);
extern void *game_find_child(void *parent, const char *name);
extern void game_release_node(void *node);
extern void *game_style_deep(void *style, uintptr_t parent);
extern void *game_default_skin(void);
extern void *game_arrow_lookup(uintptr_t parent, const char *name,
                               int a, int b, int c, int d, int e);
extern int64_t game_call_fps_limit(void);
extern int64_t game_call_fps_limit2(void);

extern uintptr_t g_game_base;
extern int32_t g_write_channel_fd;
extern int32_t g_files_dir_fd;
extern uint64_t g_page_size;
extern char g_visual_ready;
extern char g_battle_ready;
extern char g_restore_ready;
extern char g_bg_matchmaking;
extern uint32_t g_gl_flags;
extern uint64_t g_gl_status;
extern int64_t g_own_clock;
extern int64_t g_own_clock2;
extern uint64_t g_own_static_a;
extern uint64_t g_own_static_b;
extern uint64_t g_own_static_c;
extern uint64_t g_own_entity;
extern uint64_t g_frame_seq;
extern uint64_t g_battle_epoch;
extern uint64_t g_own_x;
extern uint64_t g_own_y;
extern uint64_t g_fps_frames;
extern uint64_t g_fps_time_ms;
extern uint32_t g_fps_smoothed;
extern uint64_t g_fps_last_ms;
extern volatile int g_settings_lock;
extern volatile int g_camera_lock;
extern volatile int g_chat_lock;
extern volatile int g_font_lock;
extern volatile int g_name_lock;
extern volatile int g_skin_lock;
extern volatile int g_server_lock;
extern volatile int g_dps_lock;
extern char g_settings_loaded;
extern char g_camera_loaded;
extern char g_chat_supported;
extern char g_chat_installed;
extern char g_font_preflight;
extern char g_install_flags;
extern uint32_t g_font_choice;
extern uint32_t g_font_error;
extern uint32_t g_font_loaded_mask;
extern uint32_t g_font_status;
extern uint64_t g_font_revision;
extern uint32_t g_chat_epoch;
extern uint64_t g_chat_frame_ms;
extern uint64_t g_chat_lines_used;
extern char g_chat_lines[CHAT_LINES][CHAT_LINE_SIZE];
extern uint64_t g_chat_owner;
extern int32_t g_server_region;
extern char g_server_ready;
extern uint64_t g_server_refresh_ms;
extern pthread_mutex_t g_server_mutex;
extern uint32_t g_server_count;
extern uint64_t g_server_state[0x231];
extern uint64_t g_dps_window_epoch;
extern uint64_t g_dps_window_ms;
extern uint64_t g_dps_samples[DPS_SAMPLES];
extern uint32_t g_dps_sample_count;
extern uint64_t g_dps_epoch;
extern uint64_t g_ping_epoch;
extern uint64_t g_ping_ms;
extern uint32_t g_ping_value;
extern uint64_t g_hud_epoch;
extern uint64_t g_hud_stats[14];
extern uint64_t g_hud_aux[8];
extern char g_leon_clone_ready;
extern uint32_t g_observer_mask;
extern uint32_t g_gameplay_hooks;
extern uint64_t g_slow_mode_hook;
extern uint64_t g_highlight_hook;
extern uint64_t g_chat_button_hook;
extern uint32_t g_chat_button_mask;
extern uint64_t g_disable_skins;
extern uint64_t g_default_env;
extern uint64_t g_disable_shake;
extern uint64_t g_battle_proxy_state;
extern pthread_mutex_t g_proxy_mutex;
extern uint64_t g_proxy_tag;
extern uint64_t g_proxy_endpoints[4];
extern int64_t g_proxy_skew;
extern uint64_t g_proxy_tag2;
extern uint64_t g_fps_lock;
extern uint64_t g_fps_limit_fn;
extern uint32_t g_fps_mode_count;
extern uint32_t g_fps_mode_sel;
extern uint64_t g_fps_modes[16];
extern uint64_t g_fps_apply_fn;
extern uint64_t g_name_state_fn;
extern uint64_t g_name_call_fn;
extern char g_chromatic_latch;
extern char g_name_display_state;
extern uint64_t g_name_epoch;
extern uint64_t g_name_fields[2];
extern char g_name_buffer[0xc0];
extern uint64_t g_name_cache_ms;
extern uint32_t g_skin_cache_count;
extern char g_skin_cache[0x1000][0xd0];
extern void *g_skin_cache_slots[0x1000];
extern uint64_t g_skin_name_fn;
extern uint64_t g_hud_extra_state;
extern uint64_t g_dps_meter_state;
extern uint32_t g_dps_reason;
extern uint32_t g_dps_hits;
extern uint64_t g_arrow_state[8][4];
extern uint64_t g_arrow_epoch;
extern uint64_t g_black_bars_state[4];
extern uint64_t g_black_bars_mask;
extern uint64_t g_black_bars_saved;
extern uint64_t g_bg_latch;
extern void *g_bg_fn;
extern void *g_bg_fn2;
extern void *g_server_fix_fn;
extern uint64_t g_env_1;
extern uint64_t g_env_2;
extern char g_env_ready;

static void lock_acquire(volatile int *lock)
{
    while (__atomic_test_and_set(lock, __ATOMIC_ACQUIRE))
        ;
}

static void lock_release(volatile int *lock)
{
    __atomic_clear(lock, __ATOMIC_RELEASE);
}

static int tweak_index(const char *name)
{
    for (int i = 0; i < TWEAK_KEY_COUNT; i++) {
        if (strcmp(name, g_tweak_keys[i * 3]) == 0)
            return i + 1;
    }
    return 0;
}

static int tweak_value(const char *name)
{
    int i = tweak_index(name);
    return (i > 0) ? g_tweak_values[i] : 0;
}

static int tweak_enabled(const char *name)
{
    return tweak_value(name) != 0;
}

static uint32_t tweak_max(const char *name)
{
    int i = tweak_index(name);
    return (i > 0) ? g_tweak_limits[(i - 1) * 3 + 2] : 0;
}

static uint64_t mono_ms(void)
{
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
        return 0;
    return (uint64_t)ts.tv_sec * 1000 + (uint64_t)ts.tv_nsec / 1000000;
}

extern uint64_t g_frame_record_lock;
extern uint64_t g_xray_state_ptr;
extern uint64_t g_xray_out_tag;
extern uint64_t g_hud_cache[0x1ce0 / 8];
extern int64_t respawn_timer_compute(void *rec);
extern void *leon_clone_getter_a;
extern void *leon_clone_getter_b;
extern void hud_update_gate_hook(void);
extern void pin_anim_disable(long self);
extern void friendly_room_opponents(long self);
extern void slow_mode_hook(uint64_t *out);
extern void highlight_suppress(uint64_t a, uint64_t b);
extern uint32_t chat_button_enforce(void);
extern void chat_button_hook(long self);
extern int xray_input_stage(void *rec);
extern uint64_t g_xray_rec[16];
extern uint64_t g_xray_world;
extern uint32_t g_xray_kind;
extern uint64_t g_xray_writes;
extern uint32_t g_xray_reason;
extern uint32_t g_xray_gid;
extern uint32_t g_xray_readback;
extern uint64_t g_xray_frame_ms;
extern uint64_t g_own_static_e;
extern uint64_t g_own_static_f;
extern uint64_t g_own_static_g;
extern uint64_t g_own_static_h;
extern uint64_t g_own_static_i;
extern uint64_t g_own_static_j;
extern uint64_t g_own_static_k;
extern uint64_t g_own_static_l;
extern uint64_t g_own_static_m;
extern uint64_t g_own_static_n;
extern uint64_t g_own_static_o;
extern uint64_t g_own_static_p;
extern uint64_t g_own_static_d;
extern void *g_name_original;
extern void *g_server_state_ptr;
int proxy_session_valid(const char *tag);
void name_state_sync(long self);
extern void *g_chat_original;
extern void *g_traj_original;
extern void *g_traj2_original;
extern void *g_traj_style_cache;
extern void *g_arrow_original;
extern void *g_skin_original;
extern void *g_proxy_original;
extern void *g_highlight_original;
extern void *g_chat_button_original;
extern void *g_bg_fn;
extern void *g_bg_fn2;
extern char g_proxy_tag_buf[0x48];
extern int (*g_proxy_entitlement_fn)(void);
extern void proxy_entitlement_init(void);
extern uint64_t g_bg_pending;
extern uint64_t g_black_bars_hook;
extern uint64_t g_black_bars_epoch;
extern uint64_t g_skin_extra_a;
extern uint64_t g_skin_extra_b;
extern uint64_t g_skin_extra_c;
extern uint64_t g_skin_extra_d;
extern const char *g_skin_map_in[64];
extern uintptr_t g_skin_map_out[64];

typedef struct {
    int32_t x;
    int32_t y;
    int32_t z;
    int32_t team;
    uint32_t speed;
    uint32_t radius;
} entity_index_t;

typedef struct {
    uint64_t tag;
    uint64_t aux;
} server_entry_t;

int script_port_available(const char *name);
static int script_port_hook_available(const char *name);
static int script_port_feature_available(const char *name);
static int g_camera_state_ready(void);
static int proxy_entitlement_ok(void);
static int script_port_settings_save(int idx, uint32_t value);
static void script_port_settings_load(void);
static uint32_t branch_word_build(uintptr_t from, uintptr_t to,
                                  uint32_t kind);
extern char g_ball_trajectory_ready;
extern char g_teammate_arrow_ready;
extern char g_ally_respawn_ready;
extern char g_dps_meter_hook;
extern char g_ping_hook;
extern char g_name_hook_armed;
extern char g_name_display_state;
extern uint64_t g_tweak_revision;
extern void (*g_tweak_notify_fn)(const char *, uint32_t);
extern uint64_t g_camera_frame_ms;
extern int32_t g_camera_zoom;
extern int32_t g_camera_x;
extern int32_t g_camera_y;
extern int32_t g_camera_z;
extern int32_t g_camera_tilt;
extern int32_t g_main_thread_id;
extern void hud_perf_refresh(void);
extern int server_list_refresh(void);
extern int server_state_reset(void);
extern int server_state_commit(void);
extern uint32_t g_server_ids[SERVER_SLOTS];
extern int font_reentrancy_enter(void);
extern void font_reentrancy_leave(void);
extern int font_guard_rollback(uint32_t word);
extern uint64_t g_font_guard_word;
extern uint64_t g_font_flush_fn;
extern int64_t respawn_timer_compute(void *rec);
extern int respawn_timer_ready(uintptr_t ctx);
extern int hud_active_check(void *rec);
extern int hud_record_build(void *ctx);
extern int hud_frame_gate(uintptr_t own, uintptr_t a, uintptr_t b,
                          uintptr_t c);
extern uint64_t g_hud_cache[0x1ce0 / 8];
extern uint64_t g_xray_state_ptr;
extern uint64_t g_xray_out_tag;
extern int xray_input_stage(void *rec);
extern uint64_t g_xray_rec[16];
extern uint64_t g_xray_world;
extern uint32_t g_xray_kind;
extern uint64_t g_xray_writes;
extern uint32_t g_xray_reason;
extern uint32_t g_xray_gid;
extern uint32_t g_xray_readback;
extern uint64_t g_xray_frame_ms;
extern uint64_t g_frame_record_lock;
extern uint64_t g_own_static_e;
extern uint64_t g_own_static_f;
extern uint64_t g_own_static_g;
extern uint64_t g_own_static_h;
extern uint64_t g_own_static_i;
extern uint64_t g_own_static_j;
extern uint64_t g_own_static_k;
extern uint64_t g_own_static_l;
extern uint64_t g_own_static_m;
extern uint64_t g_own_static_n;
extern uint64_t g_own_static_o;
extern uint64_t g_own_static_p;
extern uint64_t g_own_static_d;
extern void *g_name_original;
extern void *g_server_state_ptr;
int proxy_session_valid(const char *tag);
void name_state_sync(long self);
extern void *g_chat_original;
extern void *g_traj_original;
extern void *g_traj2_original;
extern void *g_traj_style_cache;
extern void *g_arrow_original;
extern void *g_skin_original;
extern void *g_proxy_original;
extern void *g_highlight_original;
extern void *g_chat_button_original;
extern void *leon_clone_getter_a;
extern void *leon_clone_getter_b;
extern void hud_update_gate_hook(void);
extern void pin_anim_disable(long self);
extern void friendly_room_opponents(long self);
extern void slow_mode_hook(uint64_t *out);
extern void highlight_suppress(uint64_t a, uint64_t b);
extern uint32_t chat_button_enforce(void);
extern void chat_button_hook(long self);
extern void battle_text_chat_hook(long self, uint64_t arg);
extern void ball_trajectory_hook_b(uint32_t, uint32_t, uint32_t, uint32_t,
                                   uint32_t, uint32_t, uintptr_t, uintptr_t,
                                   uint64_t, uint32_t, uint32_t, uint32_t);
extern void ball_trajectory_hook_a(long self, uint64_t arg);
extern void teammate_arrow_hook(uint32_t, uint64_t, int, uintptr_t,
                                uintptr_t, uint32_t);
extern char g_proxy_tag_buf[0x48];
extern int (*g_proxy_entitlement_fn)(void);
extern void proxy_entitlement_init(void);
extern uint64_t g_bg_pending;
extern uint64_t g_black_bars_hook;
extern uint64_t g_black_bars_epoch;
extern uint64_t g_skin_extra_a;
extern uint64_t g_skin_extra_b;
extern uint64_t g_skin_extra_c;
extern uint64_t g_skin_extra_d;
extern const char *g_skin_map_in[64];
extern uintptr_t g_skin_map_out[64];

static void script_port_settings_load(void)
{
    if ((g_settings_loaded & 1) != 0 || g_files_dir_fd < 0)
        return;
    g_settings_loaded = 1;

    int fd = openat(g_files_dir_fd, "script-port-settings.v1", 0x88000);
    if (fd < 0)
        return;

    struct stat sb;
    ssize_t r = -1;
    size_t len = 0;
    if (fstat(fd, &sb) == 0 && S_ISREG(sb.st_mode) &&
        sb.st_uid == getuid() && sb.st_size > 0 && sb.st_size < 0x2000) {
        char buf[0x2000];
        len = (size_t)sb.st_size;
        r = read(fd, buf, len);
        if (r == (ssize_t)len && len > 7 &&
            *(const uint64_t *)buf == 0xa3120393650534eull) {
            buf[len - 8] = '\0';
            char *p = buf;
            if (p[0] != '\0') {
                do {
                    char *nl = strchr(p, 10);
                    char *eq = strchr(p, 0x3d);
                    if (nl == NULL || eq == NULL || nl < eq)
                        break;
                    *eq = '\0';
                    *nl = '\0';
                    char *end = NULL;
                    long v = strtol(eq + 1, &end, 10);
                    if ((end == eq + 1 || *end != '\0') || v < 0 ||
                        v > 1000000)
                        break;
                    int idx = tweak_index(p);
                    if (idx > 0 && (int64_t)tweak_max(p) < v)
                        break;
                    g_tweak_values[idx] = (int32_t)v;
                    p = nl + 1;
                } while (*p != '\0');
            }
        }
    }
    close(fd);
}

int nexus_script_port_query(const char *name, uint32_t *value,
                            uint32_t *flags)
{
    if (name == NULL)
        return 0;
    int idx = tweak_index(name);
    if (idx == 0)
        return 0;
    if (flags == NULL || value == NULL)
        return 0;

    lock_acquire(&g_settings_lock);
    if ((g_settings_lock & 1) == 0) {
        script_port_settings_load();
        lock_release(&g_settings_lock);
    }
    idx = tweak_index(name);
    *value = (idx > 0) ? (uint32_t)g_tweak_values[idx] : 0;
    *flags = (uint32_t)g_tweak_limits[(idx - 1) * 3 + 2];
    return script_port_available(name) != 0;
}

int script_port_available(const char *name)
{
    if (name == NULL)
        return 0;
    if (strcmp(name, "Font") == 0 && g_font_status != 0)
        return 1;
    if (strcmp(name, "ShowSkinNamesInProfile") == 0 &&
        ((g_skin_cache_count ^ 0xffffffffu) & 7) == 0 &&
        (g_install_flags & 0x20) != 0)
        return 1;
    if (strcmp(name, "FPSLimit") == 0)
        return (g_gl_flags & 1) != 0;
    if (strcmp(name, "HideHomeScreenText") == 0)
        return (g_gl_flags >> 1 & 1) != 0;

    uint32_t bit = 0;
    if (strcmp(name, "ChromaticName") == 0)
        bit = 3;
    else if (strcmp(name, "HideBattlingStatusFromOthers") == 0)
        bit = 4;
    else if (strcmp(name, "InstantStarrDropOpening") == 0)
        bit = 8;
    else if (strcmp(name, "FriendListOptimization") == 0)
        bit = 0x10;
    else if (strcmp(name, "LegacyNames") == 0)
        bit = 0x20;
    if (bit != 0)
        return (bit & (g_install_flags ^ 0xffffffff)) == 0;

    if (strcmp(name, "BattleServerRegion") != 0) {
        if (strcmp(name, "BattleServersPopup") == 0)
            return g_server_ready != 0;
        if (strcmp(name, "ShowFastPlayAgainButton") == 0 ||
            strcmp(name, "BattleEndInstantExit") == 0)
            return g_chat_installed != 0;
        if (strcmp(name, "ShowBattleCameraButton") == 0)
            return g_camera_state_ready() != 0;
        if (script_port_hook_available(name))
            return 1;
        if (name != NULL && g_ball_trajectory_ready != 0 &&
            strcmp(name, "ExtendedTrajectory") == 0)
            return 1;
        if (script_port_feature_available(name))
            return 1;
        if (strcmp(name, "CameraZoom") != 0 &&
            strcmp(name, "CameraMode") != 0 &&
            strncmp(name, "Camera", 6) != 0) {
            if (strcmp(name, "HideSuperAim") == 0)
                return g_restore_ready == 1;
            if (strcmp(name, "HighlightLeonClone") == 0)
                return (g_restore_ready == 1) && g_leon_clone_ready != 0;
            if (strcmp(name, "DisablePinAnimation") == 0)
                return (g_restore_ready == 1) && (g_observer_mask & 1) != 0;
            if (strcmp(name, "ShowFriendlyRoomOpponents") == 0)
                return (g_restore_ready == 1) &&
                       (g_observer_mask >> 1 & 1) != 0;
            if (strcmp(name, "ShowEnemyAmmoStatus") == 0)
                return (g_restore_ready == 1) &&
                       (g_observer_mask >> 2 & 1) != 0;
            if (strcmp(name, "DisableSkins") == 0)
                return (g_restore_ready == 1) && g_disable_skins == 7;
            if (strcmp(name, "DefaultEnvironments") == 0)
                return (g_restore_ready == 1) && g_default_env != 0;
            if (strcmp(name, "DisableShake") == 0)
                return (g_restore_ready == 1) && g_disable_shake != 0;
            if (strcmp(name, "UseBattleProxy") == 0)
                return proxy_entitlement_ok() &&
                       (g_restore_ready == 1) && g_battle_proxy_state == 3;
            if (strcmp(name, "SlowMode") == 0)
                return g_slow_mode_hook != 0;
            if (strcmp(name, "DoNotShowBattleHighlight") == 0)
                return g_highlight_hook != 0;
            if (strcmp(name, "EnforceBattleChatButton") == 0)
                return (g_chat_button_hook == 7) &&
                       (g_highlight_hook >> 1 & 1) != 0;
            if (strcmp(name, "ShowDPS") == 0)
                return g_dps_meter_state != 0;
            if (strcmp(name, "ShowBattleConnectionIndicator") == 0)
                return g_ping_epoch != 0;
            if (strcmp(name, "BackgroundMatchmaking") == 0)
                return (g_bg_matchmaking != 0) && g_bg_latch != 0;
            if (strcmp(name, "HideBattleBlackBars") == 0)
                return g_black_bars_mask != 0;
            return 0;
        }
    }
    return 1;
}

int nexus_script_port_set(const char *name, uint32_t value)
{
    if (name == NULL)
        return 0;
    int idx = tweak_index(name);
    if (idx < 0)
        idx = -1;
    if ((int32_t)(idx | (int32_t)value) < 0 ||
        value > tweak_max(name))
        return 0;
    if (!script_port_available(name))
        return 0;

    lock_acquire(&g_settings_lock);
    if ((g_settings_lock & 1) == 0) {
        script_port_settings_load();
        if (script_port_settings_save((uint32_t)idx, value)) {
            g_tweak_values[idx] = (int32_t)value;
            __atomic_fetch_add(&g_tweak_revision, 1, __ATOMIC_ACQ_REL);
            if (g_tweak_notify_fn != NULL)
                g_tweak_notify_fn(name, value);
            lock_release(&g_settings_lock);
            return 1;
        }
        lock_release(&g_settings_lock);
        return 0;
    }
    lock_release(&g_settings_lock);
    return 0;
}

static int script_port_settings_save(int idx, uint32_t value)
{
    if (g_files_dir_fd < 0)
        return 0;

    char buf[0x2000];
    size_t n = 8;
    memcpy(buf, "NSP69 1\n", 8);
    for (int i = 0; i < TWEAK_KEY_COUNT; i++) {
        const char *key = g_tweak_keys[i * 3];
        uint32_t v;
        if (idx == -1 || (idx == -2 && strncmp(key, "Camera", 6) == 0))
            v = 0;
        else
            v = (i + 1 == idx) ? value : (uint32_t)g_tweak_values[i + 1];
        int w = snprintf(buf + n, sizeof buf - n, "%s=%d\n", key, v);
        if (w < 0 || (size_t)w >= sizeof buf - n)
            return 0;
        n += (size_t)w;
    }

    int fd = openat(g_files_dir_fd, "script-port-settings.v1.tmp",
                    0x88241, 0x180);
    if (fd < 0)
        return 0;
    if (write(fd, buf, n) != (ssize_t)n) {
        close(fd);
        return 0;
    }
    int fs = fsync(fd);
    close(fd);
    if (fs != 0)
        return 0;
    if (renameat(g_files_dir_fd, "script-port-settings.v1.tmp",
                 g_files_dir_fd, "script-port-settings.v1") != 0)
        return 0;
    fsync(g_files_dir_fd);
    return 1;
}

int nexus_script_port_reset(uint32_t scope)
{
    if (scope >= 2)
        return 0;

    lock_acquire(&g_settings_lock);
    if ((g_settings_lock & 1) == 0) {
        script_port_settings_load();
        int idx = (scope == 0) ? -1 : -2;
        if (script_port_settings_save(idx, 0)) {
            for (int i = 0; i < TWEAK_KEY_COUNT; i++) {
                if (scope == 0 ||
                    strncmp(g_tweak_keys[i * 3], "Camera", 6) == 0) {
                    g_tweak_values[i + 1] = 0;
                    if (g_tweak_notify_fn != NULL)
                        g_tweak_notify_fn(g_tweak_keys[i * 3], 0);
                }
            }
            __atomic_fetch_add(&g_tweak_revision, 1, __ATOMIC_ACQ_REL);
            lock_release(&g_settings_lock);
            return 1;
        }
        lock_release(&g_settings_lock);
        return 0;
    }
    lock_release(&g_settings_lock);
    return 0;
}

extern uint64_t g_tweak_revision;
extern void (*g_tweak_notify_fn)(const char *, uint32_t);
extern char g_camera_state;
extern char g_ball_trajectory_ready;
extern char g_teammate_arrow_ready;
extern char g_ally_respawn_ready;
extern char g_dps_meter_hook;
extern char g_ping_hook;
int script_port_available(const char *name);
static int script_port_settings_save(int idx, uint32_t value);
static int script_port_hook_available(const char *name);
static int script_port_feature_available(const char *name);
static int g_camera_state_ready(void);
static int proxy_entitlement_ok(void);

static const uint32_t camera_zoom_table[9] = {
    100, 100, 100, 100, 100, 100, 100, 100, 100,
};

static void script_port_camera_state_load(void)
{
    if ((g_camera_loaded & 1) != 0 || g_files_dir_fd < 0)
        return;
    g_camera_loaded = 1;

    uint32_t zoom = (uint32_t)tweak_value("CameraZoom");
    uint32_t z = (zoom < 9) ? zoom : 9;
    uint32_t zoom_val = camera_zoom_table[z];
    g_camera_z = (int32_t)zoom_val;

    uint32_t cz = (uint32_t)tweak_value("CameraZ");
    int32_t zf = 0;
    if (cz > 0x50) {
        zf = (int32_t)cz;
    } else {
        uint32_t half = (cz & 1) ? (cz + 1) >> 1 : cz >> 1;
        zf = -(int32_t)(half >> 1);
    }
    g_camera_z = zf;

    uint32_t cx = (uint32_t)tweak_value("CameraX");
    if (cx > 0x50)
        zf = (int32_t)cx;
    else {
        uint32_t half = (cx & 1) ? (cx + 1) >> 1 : cx >> 1;
        zf = -(int32_t)(half >> 1);
    }
    g_camera_x = zf * 250;

    uint32_t cy = (uint32_t)tweak_value("CameraY");
    if (cy > 0x50)
        zf = (int32_t)cy;
    else {
        uint32_t half = (cy & 1) ? (cy + 1) >> 1 : cy >> 1;
        zf = -(int32_t)(half >> 1);
    }
    g_camera_y = zf * 250;

    uint32_t ct = (uint32_t)tweak_value("CameraTilt");
    if (ct > 0x50)
        zf = (int32_t)ct;
    else {
        uint32_t half = (ct & 1) ? (ct + 1) >> 1 : ct >> 1;
        zf = -(int32_t)(half >> 1);
    }
    g_camera_tilt = zf * 250;

    int fd = openat(g_files_dir_fd, "script-port-camera.v1", 0x88000);
    if (fd < 0)
        return;
    struct stat sb;
    char buf[0xc0];
    ssize_t r = -1;
    if (fstat(fd, &sb) == 0 && S_ISREG(sb.st_mode) &&
        sb.st_uid == getuid() && sb.st_size > 0 && sb.st_size < 0xc0)
        r = read(fd, buf, (size_t)sb.st_size);
    close(fd);
    if (r > 0 && (size_t)r == (size_t)sb.st_size) {
        int a = 0, b = 0, c = 0, d = 0, e = 0;
        int consumed = 0;
        int n = sscanf(buf, "NSPC69 1\n%d %d %d %d %d\n%n",
                       &a, &b, &c, &d, &e, &consumed);
        if (n == 5 && (size_t)consumed == (size_t)sb.st_size &&
            a >= 0 && a < 0xc9 &&
            (uint32_t)(b - 0x2711) > 0xffffb1ddu &&
            (uint32_t)(c - 0x2711) > 0xffffb1ddu &&
            (uint32_t)(d - 0x2711) > 0xffffb1ddu &&
            (uint32_t)(e - 0x2711) > 0xffffb1ddu) {
            g_camera_z = b;
            g_camera_x = c;
            g_camera_y = d;
            g_camera_tilt = e;
            g_camera_zoom = a;
        }
    }
}

void nexus_script_port_camera_snapshot(int *out)
{
    if (out == NULL)
        return;
    if (out[0] != 1 || out[1] != 0x38 || g_camera_state == 0)
        return;

    lock_acquire(&g_camera_lock);
    if ((g_camera_lock & 1) == 0) {
        script_port_camera_state_load();
        uint64_t now = mono_ms();

        int show = tweak_enabled("ShowBattleCameraButton");
        int fresh = (g_camera_frame_ms != 0 && g_camera_frame_ms <= now &&
                     now - g_camera_frame_ms < 0x5dc);

        *(uint64_t *)(void *)out = ROD_INPUT_TAG;
        out[4] = g_camera_zoom;
        out[2] = show;
        out[3] = fresh;
        out[10] = g_camera_x;
        out[11] = g_camera_y;
        out[9] = g_camera_tilt;
        out[12] = g_camera_z;
        lock_release(&g_camera_lock);
    }
}

void nexus_script_port_hud_snapshot(char *out, size_t cap)
{
    if (out == NULL || cap == 0)
        return;
    *out = '\0';

    int main_thread = 0;
    if (g_main_thread_id == 0) {
        char name[16] = {0};
        pthread_t self = pthread_self();
        if (pthread_getname_np(self, name, sizeof name) == 0 &&
            strcmp(name, "Mainloop") == 0)
            main_thread = 1;
    } else if (g_main_thread_id == gettid()) {
        main_thread = 1;
    }
    if (!main_thread || g_hud_extra_state == 0)
        return;

    int saved_errno = errno;
    hud_perf_refresh();
    uint64_t now = mono_ms();
    size_t len = 0;

    if (tweak_enabled("ShowFPSCounter") || tweak_enabled("SmoothHud")) {
        g_fps_frames = 0;
        g_fps_time_ms = 0;
        g_fps_smoothed = 0;
    } else if (g_gl_status != 0) {
        if (g_fps_last_ms - 1 < now && now - g_fps_last_ms < 0x9c5 &&
            g_gl_status >= g_fps_time_ms) {
            uint32_t fps;
            if (now - g_fps_last_ms < 500) {
                fps = g_fps_smoothed;
            } else {
                fps = (g_gl_status >= g_fps_time_ms && now != g_fps_last_ms)
                    ? (uint32_t)((g_gl_status - g_fps_time_ms) * 1000 /
                                 (now - g_fps_last_ms))
                    : 0;
                g_fps_smoothed = fps;
                g_fps_last_ms = now;
                g_fps_time_ms = g_gl_status;
            }
            int n = snprintf(out + len, cap - len, "FPS: %d\n", fps);
            if (n < 0 || (size_t)n >= cap - len)
                goto done;
            len += (size_t)n;
        } else {
            g_fps_time_ms = g_gl_status;
            g_fps_smoothed = 0;
            g_fps_last_ms = now;
        }
    }

    if (frame_epoch_now() != 0) {
        if (tweak_enabled("ShowOwnPlayerCoordinates")) {
            int n = snprintf(out + len, cap - len, "X: %d  Y: %d\n",
                             (int)g_own_x, (int)g_own_y);
            if (n < 0 || (size_t)n >= cap - len)
                goto done;
            len += (size_t)n;
        }
        if (tweak_enabled("ShowDPS")) {
            int64_t dps = 0;
            lock_acquire(&g_dps_lock);
            if ((g_dps_lock & 1) == 0) {
                if (!tweak_enabled("ShowDPS")) {
                    memset(g_dps_samples, 0, 0x1008);
                    g_dps_window_epoch = g_battle_epoch;
                } else if (g_dps_window_epoch == g_battle_epoch &&
                           g_dps_sample_count != 0) {
                    for (uint32_t i = 0; i < g_dps_sample_count; i++) {
                        if (g_dps_samples[i * 2] <= now &&
                            now - g_dps_samples[i * 2] < 0x3e9)
                            dps += (int64_t)g_dps_samples[i * 2 + 1];
                    }
                }
                lock_release(&g_dps_lock);
                if (dps < -9999999)
                    dps = -10000000;
                if (dps > 9999999)
                    dps = 10000000;
            } else {
                lock_release(&g_dps_lock);
            }
            int n = snprintf(out + len, cap - len, "DPS: %d\n", (int)dps);
            if (n < 0 || (size_t)n >= cap - len)
                goto done;
            len += (size_t)n;
        }
        if (tweak_enabled("ShowBattleConnectionIndicator")) {
            int n;
            if (g_ping_epoch == g_battle_epoch && g_ping_ms <= now &&
                g_ping_value >= 1) {
                n = snprintf(out + len, cap - len, "PING: %d MS\n",
                             g_ping_value);
            } else {
                g_ping_epoch = 0;
                g_ping_ms = 0;
                g_ping_value = 0;
                n = snprintf(out + len, cap - len, "PING: --\n");
            }
            if (n < 0 || (size_t)n >= cap - len)
                goto done;
            len += (size_t)n;
        }
    }

done:
    errno = saved_errno;
}

void nexus_script_port_chat_snapshot(char *out, size_t cap)
{
    if (out == NULL || cap == 0)
        return;
    *out = '\0';

    uint64_t frame_ms = g_chat_frame_ms;
    uint64_t owner = g_chat_owner;
    uint64_t now = mono_ms();
    if (owner == 0 || frame_ms == 0 || now < frame_ms ||
        now - frame_ms > 0x5dc)
        return;
    if (tweak_value("BattleTextChat") != 1)
        return;

    lock_acquire(&g_chat_lock);
    if ((g_chat_lock & 1) == 0) {
        if (g_chat_epoch == owner) {
            *out = 0;
            if (g_chat_lines_used != 0) {
                size_t pos = 0;
                for (uint64_t i = 0; i < g_chat_lines_used; i++) {
                    size_t l = strlen(g_chat_lines[i]);
                    if (cap < pos + l + 2)
                        break;
                    memcpy(out + pos, g_chat_lines[i], l);
                    out[pos + l] = 10;
                    pos += l + 1;
                }
                out[pos] = 0;
            }
        }
        lock_release(&g_chat_lock);
    }
}

void nexus_script_port_chat_action(int action)
{
    if (g_chat_supported != 1)
        return;
    if (tweak_enabled("BattleTextChat") == 0)
        return;
    uint64_t epoch = frame_epoch_now();
    if (epoch == 0)
        return;

    if (action == 2) {
        lock_acquire(&g_chat_lock);
        if ((g_chat_lock & 1) == 0) {
            memset(g_chat_lines, 0, 0x784);
            g_chat_epoch = 0;
            lock_release(&g_chat_lock);
        }
        return;
    }

    if (action != 1 || g_name_state_fn == 0)
        return;
    uint64_t obj = 0;
    if (game_read(0, g_game_base + 0x1304c00, &obj, 8) == 0 ||
        obj + 0x2000 < 0x12000 || (obj & 7) != 0)
        return;
    uint64_t vtable = 0;
    if (game_read(0, obj, &vtable, 8) == 0 ||
        vtable + 0x2000 < 0x12000 || (vtable & 7) != 0)
        return;
    if (vtable != g_game_base + 0x11c4e30)
        return;
    uint64_t a = 0;
    if (game_resolve(obj + 0x28, &a) != 0) {
        uint64_t b = 0;
        if (game_resolve(obj + 0x58, &b) != 0) {
            ((void (*)(uint64_t, uint64_t))(g_game_base + 0x8d8ab8))(obj, a);
        }
    }
}

void nexus_script_port_server_snapshot(uint32_t *out, size_t cap)
{
    if (out == NULL || cap != 0x918)
        return;
    int saved_errno = errno;
    if (g_server_ready != 0) {
        uint64_t now = mono_ms();
        if (now <= g_server_refresh_ms - 1 ||
            now - g_server_refresh_ms > 499) {
            if (server_refresh() != 0) {
                g_server_refresh_ms = now;
                uint64_t statics = 0, list = 0, arr = 0;
                uint32_t count = 0;
                if (game_read(0, g_game_base + 0x13053c0, &statics, 8) != 0 &&
                    statics + 0x2000 > 0x11fff && (statics & 7) == 0 &&
                    game_read(0, statics, &list, 8) != 0 &&
                    list + 0x2000 > 0x11fff && (list & 7) == 0 &&
                    list == g_game_base + 0x11ebcd0 &&
                    game_read(0, statics + 0x214, &count, 4) != 0 &&
                    (int32_t)count > 0 && (int32_t)count < 0x41 &&
                    game_read(0, statics + 0x208, &arr, 8) != 0 &&
                    arr + 0x2000 > 0x11fff && (arr & 7) == 0) {
                    uint32_t slots = (count < SERVER_SLOTS) ? count
                                                            : SERVER_SLOTS;
                    (void)slots;
                    server_state_reset();
                    for (uint32_t i = 0; i < count && i < 0x20; i++) {
                        uint64_t entry = 0;
                        if (game_read(0, arr + i * 8, &entry, 8) == 0 ||
                            entry + 0x2000 < 0x12000 || (entry & 7) != 0)
                            break;
                        uint32_t id = 0, ping = 0;
                        uint64_t name_arr = 0;
                        if (game_read(0, entry, &id, 4) == 0 ||
                            game_read(0, entry + 4, &ping, 4) == 0 ||
                            game_read(0, entry + 0x28, &name_arr, 8) == 0)
                            break;
                        uint32_t sane = (ping == 1000 || (int32_t)ping < 1000)
                            ? ping : 0xffffffffu;
                        if (ping > 0x497c8)
                            sane = 0xffffffffu;
                        if (sane - 2 > 0x493deu)
                            sane = 0xffffffffu;
                        if (server_entry_add(&g_server_state, id, sane,
                                             NULL) == 0)
                            break;
                    }
                    uint64_t verify = 0;
                    if (game_read(0, g_game_base + 0x13053c0, &verify, 8) != 0 &&
                        verify == statics &&
                        game_read(0, statics + 0x208, &verify, 8) != 0 &&
                        verify == arr &&
                        game_read(0, statics + 0x214, &count, 4) != 0 &&
                        count == (uint32_t)g_server_count) {
                        pthread_mutex_lock(&g_server_mutex);
                        server_state_commit();
                        pthread_mutex_unlock(&g_server_mutex);
                    }
                }
            }
        }
    }

    pthread_mutex_lock(&g_server_mutex);
    memcpy(out, (void *)g_server_state, 0x918);
    pthread_mutex_unlock(&g_server_mutex);
    out[0] = 0x918;
    out[3] = g_server_ready;
    out[4] = tweak_value("BattleServerRegion") - 1;
    errno = saved_errno;
}

int nexus_script_port_server_select(int region)
{
    int saved_errno = errno;
    int result;
    if (region == -2) {
        result = server_list_refresh();
    } else if (region - 1000000u > 0xfff0bdbeu) {
        if (region < 0) {
            int current = tweak_value("BattleServerRegion");
            result = nexus_script_port_set("BattleServerRegion",
                                           region + 1);
            if (result != 0) {
                if (current != region + 1)
                    g_server_region = 0;
                server_list_refresh();
            }
        } else {
            pthread_mutex_lock(&g_server_mutex);
            int found = 0;
            for (uint32_t i = 0; i < g_server_count; i++) {
                if ((int32_t)g_server_ids[i] == region) {
                    found = 1;
                    break;
                }
            }
            pthread_mutex_unlock(&g_server_mutex);
            if (found)
                goto negative;
            result = 0;
        }
    } else {
        result = 0;
    }
    errno = saved_errno;
    return result;
negative:
    {
        int current2 = tweak_value("BattleServerRegion");
        int r = nexus_script_port_set("BattleServerRegion", region + 1);
        if (r != 0) {
            if (current2 != region + 1)
                g_server_region = 0;
            r = server_list_refresh();
        }
        errno = saved_errno;
        return r;
    }
}

void nexus_script_port_font_query(int *out)
{
    if (out == NULL)
        return;
    if (out[0] != 1 || out[1] != 0x30)
        return;
    int saved_errno = errno;
    if (g_font_status == 0 || g_hud_extra_state == 0) {
        out[0] = 0;
        return;
    }
    if (font_mask_reload() != 0) {
        if (!g_font_preflight) {
            g_font_preflight = 1;
            char path[0x60];
            game_string_format(path, "font/Pusia-Bold.otf");
            uint32_t m1 = (game_font_load(path, 0) != 0) ? 1 : 0;
            game_string_release(path);
            game_string_format(path, "font/Cocon-Regular.otf");
            uint32_t m2 = (game_font_load(path, 0) != 0) ? 1 : 0;
            game_string_release(path);
            game_string_format(path, "font/Downcome.otf");
            uint32_t m3 = (game_font_load(path, 0) != 0) ? 1 : 0;
            game_string_release(path);
            game_string_format(path, "font/Impact.ttf");
            uint32_t m4 = (game_font_load(path, 0) != 0) ? 1 : 0;
            game_string_release(path);
            g_font_loaded_mask = (m2 & 1) << 2 | (m1 & 1) << 1 |
                                 (m3 & 1) << 3 | (m4 & 1) << 4 | 1;
            char log[0x60];
            snprintf(log, sizeof log,
                     "asset_preflight new_mask=%u loaded_mask=%u",
                     g_font_loaded_mask, g_font_loaded_mask);
            runtime_journal_write("script_port_fonts", log, NULL);
        }
    }
    *(uint64_t *)(void *)out = ROD_STATUS_HDR;
    out[4] = tweak_value("Font");
    out[5] = g_font_status;
    out[6] = g_font_loaded_mask;
    out[7] = g_font_error;
    out[8] = g_font_choice;
    out[9] = 0;
    out[10] = g_font_revision;
    errno = saved_errno;
}

void nexus_script_port_client_performance_query(int *out)
{
    if (out == NULL)
        return;
    if (out[0] != 1 || out[1] != 0x60)
        return;
    int saved_errno = errno;
    uint32_t flags = (g_hud_extra_state != 0) ? (uint32_t)g_gl_flags : 0;
    uint32_t fps = (uint32_t)tweak_value("FPSLimit");
    uint32_t hide = tweak_enabled("HideHomeScreenText") ? 1u : 0u;

    char best_name[0x40] = {0};
    int32_t best_ping = -1;
    {
        uint32_t state[0x246];
        memset(state, 0, 0x918);
        nexus_script_port_server_snapshot(state, 0x918);
        if (state[2] != 0 && state[3] < 0x21) {
            for (uint32_t i = 0; i < state[3]; i++) {
                int32_t ping = *(int32_t *)((char *)state + 0x10 +
                                            i * SERVER_REC);
                if (ping >= 0 && (best_ping < 0 || ping < best_ping)) {
                    best_ping = ping;
                    snprintf(best_name, sizeof best_name, "%s",
                             (char *)state + 0x18 + i * SERVER_REC);
                }
            }
        }
    }

    *(uint64_t *)(void *)out = ROD_CAPTURE_TAG;
    out[2] = flags | ((uint64_t)hide << 32);
    out[3] = 0;
    out[4] = fps | ((uint32_t)best_ping << 0) | ((uint64_t)(uint32_t)best_ping << 32 >> 32);
    out[5] = (uint32_t)best_ping;
    out[4] = fps | ((uint64_t)(uint32_t)best_ping << 0);
    out[6] = g_tweak_revision;
    memcpy(out + 8, best_name, sizeof best_name);
    out[0x16] = 0;
    errno = saved_errno;
}

void chat_frame_record(void *ctx)
{
    uint64_t *frame = ctx;
    if (ctx == NULL)
        goto reset;
    if (tweak_value("BattleTextChat") != 1)
        goto reset;
    uint64_t epoch = frame_epoch_now();
    if (epoch == 0)
        goto reset;
    g_chat_owner = epoch;
    g_chat_frame_ms = frame[5];
    return;

reset:
    g_chat_owner = 0;
    g_chat_frame_ms = 0;
    lock_acquire(&g_chat_lock);
    if ((g_chat_lock & 1) == 0) {
        memset(g_chat_lines, 0, 0x784);
        g_chat_epoch = 0;
        lock_release(&g_chat_lock);
    }
}

void font_apply_tick(void)
{
    int saved_errno = errno;
    uint32_t guard = 0;
    if (game_read(0, g_game_base + 0xd06d98, &guard, 4) == 0 ||
        guard != g_font_guard_word || main_thread_gate() == 0) {
        g_font_status = 0;
        g_font_error = 3;
        g_font_loaded_mask = 0;
        int rolled = font_guard_rollback(g_font_guard_word);
        runtime_journal_write("script_port_fonts",
                              rolled ? "init_guard_rolled_back"
                                     : "init_guard_owner_changed",
                              NULL);
        if (rolled)
            ((void (*)(void))(g_game_base + 0xd06d98))();
        errno = saved_errno;
        return;
    }

    int reentrant = font_reentrancy_enter();
    uint32_t choice = (uint32_t)tweak_value("Font");
    int do_apply = 0;
    if (reentrant == 0) {
        if (g_font_status != 0 && g_hud_extra_state != 0 && choice < 6 &&
            choice - 1 < 5) {
            char path[0x60];
            game_string_format(path, g_font_paths[choice]);
            game_font_load(path, 0);
            game_string_release(path);
            do_apply = 1;
        }
    }
    errno = saved_errno;
    ((void (*)(void))g_font_flush_fn)();

    if (do_apply) {
        int ok = font_entry_apply(choice);
        g_font_error = (ok == 0) ? 2 : 0;
        g_font_choice = ok ? choice : 0xffffffffu;
        g_font_revision++;
        g_font_status = 0;
        char log[0x60];
        snprintf(log, sizeof log,
                 "choice=%d applied=%d error=%u loaded_mask=%u",
                 choice, g_font_choice, g_font_error, g_font_error << 0);
        runtime_journal_write("script_port_fonts", log, NULL);
        if (reentrant == 0) {
            g_font_loaded_mask = font_mask_reload();
            g_font_preflight = 0;
        }
    } else if (reentrant == 0) {
        g_font_loaded_mask = font_mask_reload();
        g_font_preflight = 0;
        g_font_revision++;
    }
    font_reentrancy_leave();
    errno = saved_errno;
}
static void pin_anim_disable_stub(long self);
static void friendly_room_opponents_stub(long self);
static void slow_mode_hook_stub(uint64_t *out);
static void highlight_suppress_stub(uint64_t a, uint64_t b);
static uint32_t chat_button_enforce_stub(void);
static void chat_button_hook_stub(long self);
static void hud_update_gate_hook_stub(void);



int hud_frame_record(void *ctx)
{
    uint64_t *frame = ctx;
    if (g_main_thread_id == 0)
        return 0;
    if (g_main_thread_id != gettid())
        return 0;
    if (g_own_static_c == 0 || (int32_t)g_own_static_a == 0 ||
        g_own_static_d > 0x20)
        return 0;
    if (g_own_static_b != frame[4] || g_own_static_a != frame[3] ||
        g_battle_epoch != frame[5] ||
        !frame_gate_check(g_own_static_e, g_own_static_f))
        return 0;

    int64_t rec[0x1ce0 / 8];
    if (g_hud_epoch != g_own_static_b ||
        g_hud_stats[0] != g_own_static_b ||
        g_hud_stats[2] != frame[5]) {
        rec[0] = 0;
        rec[1] = g_own_static_b;
        rec[2] = g_own_static_a;
        rec[3] = frame[5];
        rec[4] = g_own_static_d;
        memset((char *)rec + 0x34, 0, 0x1ba4);
        hud_record_build(rec);
        if (respawn_timer_ready(g_own_static_c)) {
            int64_t timer = respawn_timer_compute(rec);
            if (timer > 0)
                *(int32_t *)((char *)rec + 0x3c) = (int32_t)timer;
        }
        if (g_env_1 != 0) {
            if (game_read(0, g_env_1 + 0xc4, &rec[0x34 / 8], 4) != 0 &&
                game_read(0, g_env_1 + 200, &rec[0x38 / 8], 4) != 0 &&
                (int32_t)rec[0x34 / 8] > 0 &&
                (int32_t)rec[0x34 / 8] < 0x65 &&
                (int32_t)rec[0x38 / 8] > 0 &&
                (int32_t)rec[0x38 / 8] < 0x65) {
                *(int32_t *)((char *)rec + 0x34) = (int32_t)rec[0x34 / 8];
                *(uint32_t *)((char *)rec + 0x38) = (uint32_t)rec[0x38 / 8];
            }
        }
        *(uint32_t *)rec = g_ping_value;
        if (tweak_enabled("ShowDPS")) {
            int64_t dps = 0;
            lock_acquire(&g_dps_lock);
            if ((g_dps_lock & 1) == 0 &&
                g_dps_window_epoch == (uint64_t)rec[1]) {
                for (uint32_t i = 0; i < g_dps_sample_count; i++)
                    if (g_dps_samples[i * 2] <= (uint64_t)frame[5] &&
                        (uint64_t)frame[5] - g_dps_samples[i * 2] < 0x3e9)
                        dps += (int64_t)g_dps_samples[i * 2 + 1];
                lock_release(&g_dps_lock);
                if (dps < -9999999)
                    dps = -10000000;
                if (dps > 9999999)
                    dps = 10000000;
                *(int32_t *)((char *)rec + 4) = (int32_t)dps;
            } else {
                lock_release(&g_dps_lock);
            }
        }
        int published = xray_input_publish((void *)g_xray_state_ptr, rec,
                                           (void *)g_xray_out_tag);
        g_hud_epoch = published != 0;
        return published != 0;
    }
    memcpy(rec, (void *)g_hud_cache, 0x1ce0);
    hud_record_build(rec);
    return 1;
}

int hud_state_publish(void *ctx, uint64_t *rec)
{
    (void)ctx;
    if (hud_active_check(rec) == 0)
        return 0;
    *(uint32_t *)((char *)rec + 0x5c) = 0;
    *(uint32_t *)(rec + 0xc) = 0;
    *(uint32_t *)((char *)rec + 100) =
        (uint32_t)(tweak_value("TileGrid") == 1);
    *(uint32_t *)(rec + 0xd) = 0;
    *(uint32_t *)((char *)rec + 0x6c) =
        (uint32_t)(tweak_value("SmoothHudGraph") == 1);
    *(uint32_t *)(rec + 0xe) = 0;
    *(uint32_t *)((char *)rec + 0x74) =
        (uint32_t)(tweak_value("AllyRespawnTimer") == 1);
    g_hud_aux[5] = rec[5];
    g_hud_aux[4] = rec[4];
    g_hud_aux[7] = rec[7];
    g_hud_aux[6] = rec[6];
    rec[0] = 0;
    g_hud_stats[1] = rec[1];
    g_hud_stats[0] = 0;
    g_hud_stats[3] = rec[3];
    g_hud_stats[2] = rec[2];
    g_hud_stats[0xb] = rec[0xb];
    g_hud_stats[10] = rec[10];
    g_hud_stats[0xd] = rec[0xd];
    g_hud_stats[0xc] = rec[0xc];
    g_hud_stats[0xe] = rec[0xe];
    g_hud_stats[9] = rec[9];
    g_hud_stats[8] = rec[8];
    return 1;
}

void hud_stage_gate(void *ctx, uintptr_t a, uintptr_t b, uintptr_t c)
{
    (void)ctx;
    if (g_main_thread_id == 0 || g_main_thread_id != gettid())
        return;
    if (g_own_static_b != a || g_own_static_a != b)
        return;
    if (!hud_frame_gate(0, a, b, c))
        return;
    if (tweak_enabled("ShowBattleConnectionIndicator") ||
        tweak_enabled("ShowDPS") || tweak_enabled("TileGrid") ||
        tweak_enabled("SmoothHud") || tweak_enabled("SmoothHudGraph") ||
        tweak_enabled("ShowFPSCounter") ||
        tweak_enabled("ShowOwnPlayerCoordinates") ||
        tweak_enabled("AllyRespawnTimer"))
        frame_gate_check(g_own_static_e, g_own_static_f);
}

uint32_t hud_visibility_check(void *ctx, int kind, uintptr_t a,
                              uintptr_t b)
{
    (void)ctx;
    if (kind != 2 || a == 0 || b == 0)
        return 0;
    if (g_hud_extra_state == 0 || g_battle_ready != 1)
        return 0;
    if (g_ping_epoch != a || g_ping_value != b)
        return 0;
    uint64_t now = mono_ms();
    if (now <= g_ping_ms - 1 || now - g_ping_ms > 500)
        return 0;
    if (!hud_active_check(&g_hud_cache))
        return 0;
    if (tweak_enabled("ShowBattleConnectionIndicator") ||
        tweak_enabled("ShowDPS") || tweak_enabled("TileGrid") ||
        tweak_enabled("SmoothHud") || tweak_enabled("SmoothHudGraph") ||
        tweak_enabled("ShowFPSCounter") ||
        tweak_enabled("ShowOwnPlayerCoordinates") ||
        tweak_enabled("AllyRespawnTimer"))
        return (tweak_enabled("AllyRespawnTimer") ? 1u : 0u) << 1;
    return 0;
}

int optional_gameplay_hook_guard_install(void)
{
    int ok = 0;
    if (visual_pipeline_ready() != 0 &&
        game_region_verify(0xe45000, 0xab8,
            "f490a4b6045692dcb24a39b754bfeb2d0c668b17555b84f7fdb68d9f493d4d57") != 0 &&
        game_region_verify(0xe45f28, 0x20,
            "afe806a653e90c4c55961d4f5f9ebe3251e529d563cb3d4d72e6eda4666540eb") != 0 &&
        game_region_verify(0x533eb0, 0x10,
            "431504d966d6b81b5ba5442c844791b524748bfcc4b28de510001b2096172b73") != 0 &&
        game_region_verify(0x533ec0, 0x10,
            "4ead9da4fcd8c8d8f3e63f9b03c292b64c990bbe11ff82af6bf945c01178a658") != 0) {
        uintptr_t target_a = g_game_base + 0xe45f28;
        void *island_a = trampoline_alloc(target_a);
        uint32_t saved_a = 0;
        if (island_a != NULL &&
            game_read(0, target_a, &saved_a, 4) != 0 &&
            (((uint32_t)(uintptr_t)island_a | (uint32_t)target_a) & 3) == 0) {
            uint32_t b = branch_word_build(target_a,
                                           (uintptr_t)island_a, 0x14000000u);
            *(uint64_t *)island_a = 0xd61f020058000050ull;
            ((void **)island_a)[1] = leon_clone_getter_a;
            icache_flush(island_a, (char *)island_a + 0x10);
            if (mprotect(island_a, g_page_size, PROT_READ | PROT_EXEC) == 0) {
                uintptr_t target_b = g_game_base + 0xe45f38;
                void *island_b = trampoline_alloc(target_b);
                uint32_t saved_b = 0;
                if (island_b != NULL &&
                    game_read(0, target_b, &saved_b, 4) != 0 &&
                    game_write(g_write_channel_fd, &b, 4, target_a) == 4) {
                    icache_flush((void *)target_a, (void *)(target_a + 4));
                    uint32_t check = 0;
                    if (game_read(0, target_a, &check, 4) != 0 &&
                        check == b) {
                        uint32_t b2 = branch_word_build(
                            target_b, (uintptr_t)island_b, 0x14000000u);
                        *(uint64_t *)island_b = 0xd61f020058000050ull;
                        ((void **)island_b)[1] = leon_clone_getter_b;
                        icache_flush(island_b, (char *)island_b + 0x10);
                        if (mprotect(island_b, g_page_size,
                                     PROT_READ | PROT_EXEC) == 0 &&
                            game_write(g_write_channel_fd, &b2, 4,
                                       target_b) == 4) {
                            icache_flush((void *)target_b,
                                         (void *)(target_b + 4));
                            g_leon_clone_ready = 1;
                            runtime_journal_write(
                                "script_port_ready",
                                "HighlightLeonClone_v69_native_getters",
                                NULL);
                            ok = 1;
                        }
                    }
                }
            }
        }
    }

    int a = guarded_hook_install(0xbb5200, 0x274,
        "26b7d49685350c821950a694f2735baac26701992baf92a9160b72ed48d3c158",
        0xbb5200, 0xfc1c0fe8, pin_anim_disable_stub);
    int b = guarded_hook_install(0xa64818, 0xd60,
        "bed76fecd36c87ca211df8766a16a8ab4de9a1e379e4a7114809645086baa92f",
        0xa64818, 0xd104c3ff, friendly_room_opponents_stub);
    g_observer_mask = (uint32_t)((a != 0) | ((b != 0) << 1));
    int c = guarded_hook_install(0x81b298, 0x1484,
        "1dd1fef35059b52de083648e1a7e1ffdd95d66a9acdd3dfb68058a4dca03b82f",
        0x81c6f4, 0xa94b4ff4, hud_update_gate_hook_stub);
    if (c != 0)
        g_observer_mask |= 4;
    char log[0x50];
    snprintf(log, sizeof log, ",\"clone\":%d,\"observer_mask\":%u", ok,
             g_observer_mask);
    runtime_journal_write("script_port_hooks", "v69_native_gameplay", log);
    return ok != 0 && g_observer_mask == 7;
}

void visual_install_3(void)
{
    if (game_region_verify(0xd0ba10, 0x25c,
            "f2bb7422387baf6f531909504141f1cd3034f4e57bf30cb54253e180f454371a") != 0) {
        g_slow_mode_hook = guarded_hook_install(
            0x75e6a0, 0xc,
            "7ce713049f8d31080501c810638e08a9a353cef97c534803d3139266c5818af5",
            0x75e6a4, 0x393fa100, slow_mode_hook_stub);
    }
    g_name_state_fn = (uint64_t)(uintptr_t)&name_state_sync;
    if (g_slow_mode_hook != 0 && tweak_enabled("SlowMode"))
        ((void (*)(int))(g_game_base + 0x75e6a0))(1);

    guarded_observer_install(0x922ffc, 0x524,
        "ceba13cf13c0ed00e0da2e2452ec5f02dc998da79622bee05604aab64f2ed204",
        highlight_suppress_stub, &g_highlight_hook, 1);
    guarded_observer_install(0xb3fcec, 0xa8,
        "fed298499238ac622efdd657a7a50599617132f0907306189b30825f8b5dcd9f",
        chat_button_enforce_stub, &g_chat_button_hook, 2);

    if (game_region_verify(0x8683c0, 0x332c,
            "0bfac2bff9221607fe0eaba0d6406e19a4078c307d545165e170f0e4a67bdda7") != 0) {
        if (guarded_hook_install(0x86a214, 4,
                "b95c3d0b050224c2030dfff0e05768712c3d549ebf5ba905623e9e037a76544b",
                0x86a214, 0x39002117, chat_button_hook_stub) != 0)
            g_chat_button_mask |= 1;
        if (guarded_hook_install(0x86a270, 4,
                "b95c3d0b050224c2030dfff0e05768712c3d549ebf5ba905623e9e037a76544b",
                0x86a270, 0x39002117, chat_button_hook_stub) != 0)
            g_chat_button_mask |= 2;
        if (guarded_hook_install(0x86a284, 4,
                "b95c3d0b050224c2030dfff0e05768712c3d549ebf5ba905623e9e037a76544b",
                0x86a284, 0x39002117, chat_button_hook_stub) != 0)
            g_chat_button_mask |= 4;
    }
}

void queue_plan_callback(void *ctx)
{
    int saved_errno = errno;
    uint64_t *rec = ctx;
    if (ctx == NULL || g_restore_ready != 1 || g_main_thread_id == 0 ||
        g_main_thread_id != gettid())
        return;

    uint64_t fr_prev = __atomic_exchange_n(&g_frame_record_lock, 1ull,
                                           __ATOMIC_ACQ_REL);
    if ((fr_prev & 1) == 0 || g_dps_meter_hook == 3 ||
        g_dps_meter_hook == 1) {
        uint64_t now = mono_ms();
        if (g_dps_meter_hook == 0 && g_own_static_c != 0) {
            uint64_t world = 0;
            if (frame_gate_check(g_own_static_e, g_own_static_f) &&
                game_read(0, g_own_static_g + 0x58, &world, 8) != 0 &&
                world + 0x2000 > 0x11fff && (world & 7) == 0 &&
                world == rec[0]) {
                int32_t xy[6];
                if (game_read(0, rec[1] + 8, xy, 0xc) != 0 &&
                    xy[0] == 2 && xy[1] > -60000 && xy[1] < 60000 &&
                    xy[2] > -60000 && xy[2] < 60000) {
                    g_own_static_h = g_own_static_b;
                    g_own_static_i = g_own_static_a;
                    g_own_static_j = xy[2];
                    g_own_static_k = xy[1];
                    g_own_static_l = 1;
                    g_own_static_m = now;
                }
            }
        }
        if (tweak_enabled("HideSuperAim")) {
            if (main_thread_gate() != 0 && xray_stage_scan(&g_xray_rec) != 0 &&
                game_read(0, g_own_static_g + 0x58, &g_xray_world, 8) != 0 &&
                g_xray_world == rec[0] &&
                game_read(0, rec[1] + 8, &g_xray_kind, 4) != 0 &&
                g_xray_kind == 5) {
                ((void (*)(uint64_t))(g_game_base + 0x11a2830))(rec[1]);
                __atomic_store_n(&g_frame_record_lock, 0ull, __ATOMIC_RELEASE);
                errno = saved_errno;
                return;
            }
        }
        if (xray_input_stage(rec)) {
            g_xray_writes++;
            char log[0x100];
            snprintf(log, sizeof log,
                     ",\"reason\":%u,\"gid\":%u,\"writes\":%llu,"
                     "\"readback\":%u,\"extra_inputs\":0",
                     g_xray_reason, g_xray_gid,
                     (unsigned long long)g_xray_writes, g_xray_readback);
            runtime_journal_write("xray_input", "existing_input_target",
                                  log);
        }
        g_xray_frame_ms = mono_ms();
        g_own_static_n = 0;
        g_own_static_o = 0;
        __atomic_store_n(&g_frame_record_lock, 0ull, __ATOMIC_RELEASE);
    }
    errno = saved_errno;
}

static void pin_anim_disable_stub(long self) { pin_anim_disable(self); }
static void friendly_room_opponents_stub(long self)
{
    friendly_room_opponents(self);
}
static void slow_mode_hook_stub(uint64_t *out) { slow_mode_hook(out); }
static void highlight_suppress_stub(uint64_t a, uint64_t b)
{
    highlight_suppress(a, b);
}
static uint32_t chat_button_enforce_stub(void)
{
    return chat_button_enforce();
}
static void chat_button_hook_stub(long self) { chat_button_hook(self); }
static void hud_update_gate_hook_stub(void) { hud_update_gate_hook(); }

extern uint64_t g_frame_record_lock;
extern uint64_t g_xray_state_ptr;
extern uint64_t g_xray_out_tag;
extern uint64_t g_hud_cache[0x1ce0 / 8];
extern int64_t respawn_timer_compute(void *rec);
extern int xray_input_stage(void *rec);
extern uint64_t g_xray_rec[16];
extern uint64_t g_xray_world;
extern uint32_t g_xray_kind;
extern uint64_t g_xray_writes;
extern uint32_t g_xray_reason;
extern uint32_t g_xray_gid;
extern uint32_t g_xray_readback;
extern uint64_t g_xray_frame_ms;
extern uint64_t g_own_static_e;
extern uint64_t g_own_static_f;
extern uint64_t g_own_static_g;
extern uint64_t g_own_static_h;
extern uint64_t g_own_static_i;
extern uint64_t g_own_static_j;
extern uint64_t g_own_static_k;
extern uint64_t g_own_static_l;
extern uint64_t g_own_static_m;
extern uint64_t g_own_static_n;
extern uint64_t g_own_static_o;

static uint32_t branch_word_build(uintptr_t from, uintptr_t to, uint32_t kind)
{
    uint32_t delta = (uint32_t)(to - from);
    uint32_t q = (int32_t)delta < 0 ? delta + 3 : delta;
    return kind | ((q >> 2) & 0x3ffffff);
}

static void chat_line_push(const char *text, size_t len)
{
    if (g_chat_lines_used == CHAT_LINES) {
        memmove(g_chat_lines, g_chat_lines + 1, 0x6c0);
        g_chat_lines_used = 9;
    }
    if (len >= CHAT_LINE_SIZE - 1)
        len = CHAT_LINE_SIZE - 1;
    memcpy(g_chat_lines[g_chat_lines_used], text, len);
    g_chat_lines[g_chat_lines_used][len] = 0;
    g_chat_lines_used++;
}

void battle_text_chat_hook(long self, uint64_t arg)
{
    int saved_errno = errno;
    ((void (*)(long, uint64_t))g_chat_original)(self, arg);
    uint64_t now = mono_ms();

    if (g_chat_installed != 1 || g_chat_owner == 0 || g_chat_frame_ms == 0 ||
        g_chat_frame_ms > now || now - g_chat_frame_ms >= 0x5dd)
        goto out;
    if (tweak_value("BattleTextChat") != 1)
        goto out;

    {
        uint64_t sender = 0, text_obj = 0;
        char sender_buf[0x180], text_buf[0x400];
        if (game_read(0, self + 0x18, &sender, 8) == 0 ||
            sender + 0x2000 < 0x12000 || (sender & 7) != 0 ||
            game_read(0, self + 0x30, &text_obj, 8) == 0 ||
            text_obj + 0x2000 < 0x12000 || (text_obj & 7) != 0 ||
            game_deref_read(sender, sender_buf, 0x180) == 0 ||
            game_deref_read(text_obj, text_buf, 0x400) == 0 ||
            text_buf[0] == '\0')
            goto out;

        lock_acquire(&g_chat_lock);
        if ((g_chat_lock & 1) == 0) {
            if (g_chat_epoch != g_chat_owner) {
                memset(g_chat_lines, 0, 0x784);
                g_chat_epoch = g_chat_owner;
            }
            char line[0x600];
            int n = snprintf(line, sizeof line, "[%s]: %s", sender_buf,
                             text_buf);
            if (n > 0) {
                size_t len = (size_t)n;
                size_t start = 0;
                for (size_t i = 0; i <= len; i++) {
                    if (i == len || line[i] == '\n' || line[i] == '\r') {
                        if (i > start)
                            chat_line_push(line + start, i - start);
                        start = i + 1;
                    }
                }
            }
            lock_release(&g_chat_lock);
        }
    }
out:
    errno = saved_errno;
}

void ball_trajectory_hook_b(uint32_t a1, uint32_t a2, uint32_t a3,
                            uint32_t a4, uint32_t a5, uint32_t a6,
                            uintptr_t self, uintptr_t a8, uint64_t a9,
                            uint32_t a10, uint32_t a11, uint32_t a12)
{
    int saved_errno = errno;
    int active = 0;
    if (g_ball_trajectory_ready == 1 &&
        tweak_value("ExtendedTrajectory") == 1 &&
        main_thread_gate() != 0 && g_own_static_e == self)
        active = (g_own_static_f == a8);

    ((void (*)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t,
               uintptr_t, uintptr_t, uint64_t, uint32_t, uint32_t,
               uint32_t))g_traj_original)(a1, a2, a3, a4, a5, a6, self, a8,
                                          a9, a10, a11, a12);
    (void)active;
    errno = saved_errno;
}

void ball_trajectory_hook_a(long self, uint64_t arg)
{
    int saved_errno = errno;
    ((void (*)(long, uint64_t))g_traj2_original)(self, arg);
    char custom[0x191];
    uint32_t mask = g_disable_skins;
    if ((mask >> 3 & 1) != 0)
        mask = g_install_flags & 0xff;
    else
        mask = g_install_flags & 1;

    if (((mask & 3) == 0 && (mask != 0) && g_bg_matchmaking != 0) ||
        mask == 0 || g_hud_extra_state == 0)
        goto out;
    if (g_name_display_state == 0)
        goto out;

    {
        char name[0xc0];
        uint64_t style = 0;
        if (game_read(0, self + 0x40, &style, 8) != 0 &&
            style == (uint64_t)g_traj_style_cache) {
            if (game_read(0, self + 0x270, &g_xray_world, 8) != 0 &&
                game_deref_read(g_xray_world, name, 0x191) != 0) {
                game_string_release((void *)name);
                game_string_format(name, custom);
            }
        }
    }
out:
    errno = saved_errno;
}

void teammate_arrow_hook(uint32_t a1, uint64_t a2, int a3, uintptr_t a4,
                         uintptr_t a5, uint32_t a6)
{
    int saved_errno = errno;
    int active = 0;
    if (g_teammate_arrow_ready == 1 && main_thread_gate() != 0 &&
        g_main_thread_id != 0 && g_main_thread_id == gettid() &&
        g_own_static_f == (uint64_t)a4 && (int64_t)g_own_static_a == a3) {
        if (frame_gate_check(g_own_static_e, a4)) {
            if (g_arrow_epoch != g_own_static_b) {
                g_arrow_epoch = g_own_static_b;
                memset(g_arrow_state, 0, sizeof g_arrow_state);
            }
            active = 1;
        }
    }

    uint64_t ret = ((uint64_t (*)(uint32_t, uint64_t, int, uintptr_t,
                                  uintptr_t, uint32_t))g_arrow_original)(
        a1, a2, a3, a4, a5, a6);

    if (active && tweak_value("TeammateHPIndicator") == 1 &&
        respawn_timer_ready(g_own_static_c) != 0 && g_own_static_d != 0) {
        int64_t slots[0x20];
        uint32_t count = 0;
        for (uint32_t i = 0; i < g_own_static_d && count < 0x20; i++) {
            int64_t *rec = (int64_t *)(g_own_static_p + i * 0x40);
            if ((uint64_t)rec[0] != (uint64_t)a4 && rec[5] != 0 &&
                *(int32_t *)((char *)rec + 0x2c) != 0 &&
                *(uint32_t *)(rec + 3) < 10 &&
                *(int32_t *)((char *)rec + 0x1c) == a3 &&
                *(uint32_t *)(rec + 4) <= *(uint32_t *)((char *)rec + 0x24) &&
                *(uint32_t *)((char *)rec + 0x24) != 0)
                slots[count++] = (int64_t)(uintptr_t)rec;
        }
        if (count != 0) {
            uint32_t pick = (a6 <= count) ? count - 1 : a6;
            int64_t *rec = (int64_t *)slots[pick];
            entity_index_t idx;
            if (entity_index_fetch(g_game_base, (uintptr_t)rec[0],
                                   (uintptr_t)game_read, 0, &idx) != 0 &&
                idx.x == *(int *)(rec + 2) && idx.team != 0 &&
                idx.speed != 0 && idx.z <= idx.team) {
                uintptr_t arrow = (uintptr_t)game_arrow_lookup(
                    a5, "arrow", 0, 0, 0, 0, 0);
                if (arrow != 0) {
                    int slot = -1;
                    for (int i = 0; i < 8; i++) {
                        if (g_arrow_state[i][0] == 0) {
                            slot = i;
                            break;
                        }
                    }
                    if (slot >= 0) {
                        uint8_t color[7];
                        if (game_read(0, arrow + 9, color, 7) != 0) {
                            arrow_color_build((float)idx.z / (float)idx.team,
                                              (void *)(uintptr_t)0x1391f0,
                                              color);
                            if (game_write(g_write_channel_fd, color, 7,
                                           arrow + 9) == 7) {
                                g_arrow_state[slot][0] = a5;
                                g_arrow_state[slot][1] = arrow;
                                memcpy(&g_arrow_state[slot][2], color, 7);
                            } else {
                                game_write(g_write_channel_fd, color, 7,
                                           arrow + 9);
                            }
                        }
                    }
                }
            }
        }
    }
    errno = saved_errno;
    (void)ret;
}

void pin_anim_disable(long self)
{
    if (self == 0 || (g_observer_mask & 1) == 0 || g_restore_ready != 1)
        return;
    if (tweak_enabled("DisablePinAnimation")) {
        *(uint32_t *)(self + 0x110) = 0;
        *(uint64_t *)(self + 8) = 0;
        *(uint64_t *)(self + 0x10) = 0;
    }
}

void friendly_room_opponents(long self)
{
    if (self == 0 || (g_observer_mask >> 1 & 1) == 0 || g_restore_ready != 1)
        return;
    if (tweak_enabled("ShowFriendlyRoomOpponents"))
        *(uint64_t *)(self + 0x30) = 0xff;
}

int leon_clone_detect(uintptr_t entity)
{
    if (g_leon_clone_ready == 0 || g_restore_ready != 1)
        return 0;
    if (!tweak_enabled("HighlightLeonClone"))
        return 0;
    uint64_t vtable = 0;
    if (game_read(0, entity, &vtable, 8) == 0 ||
        vtable + 0x2000 < 0x12000 || (vtable & 7) != 0)
        return 0;
    if (vtable != g_game_base + 0x121c438)
        return 0;
    uint32_t id = 0;
    if (game_read(0, entity + 0x20, &id, 4) == 0)
        return 0;
    if (id != 0x10366f9 && id != 0xf424b9)
        return 0;
    uint64_t pair = 0;
    if (game_read(0, g_game_base + 0x1244690, &pair, 8) != 0 &&
        pair == 0x91 && (uint32_t)(pair >> 32) == 0x92) {
        uint64_t inner = 0;
        if (game_read(0, entity + 8, &inner, 8) != 0 &&
            inner + 0x2000 > 0x11fff && (inner & 7) == 0) {
            uint32_t kind = 0;
            if (game_read(0, inner + 8, &kind, 4) != 0 && kind == 0xb9) {
                uint64_t a = 0, b = 0, c = 0, d = 0;
                if (game_read(0, inner, &a, 8) != 0 &&
                    a + 0x2000 > 0x11fff && (a & 7) == 0 &&
                    game_read(0, a + 0x38, &b, 8) != 0 &&
                    b + 0x2000 > 0x11fff && (b & 7) == 0 &&
                    game_read(0, b, &c, 8) != 0 &&
                    c + 0x2000 > 0x11fff && (c & 7) == 0 &&
                    game_read(0, c + 8, &d, 8) != 0 &&
                    d + 0x2000 > 0x11fff && (d & 7) == 0) {
                    uint32_t desc[4] = {0, 0, 0, 0};
                    if (game_read(0, d + (uint64_t)(int32_t)kind * 0x10,
                                  desc, 0x10) != 0 &&
                        desc[0] == 9 &&
                        game_read(0, desc[1], desc + 1, 10) != 0)
                        return *(uint64_t *)(desc + 1) ==
                               0x6b6146616a6e694cull;
                }
            }
        }
    }
    return 0;
}

void disable_skins_static(long self)
{
    if (self == 0 || (g_disable_skins >> 1 & 1) == 0 ||
        g_restore_ready != 1)
        return;
    if (!tweak_enabled("DisableSkins"))
        return;
    uint64_t a = 0, b = 0;
    if (game_read(0, *(uintptr_t *)(self + 8) + 0x10, &a, 8) != 0 &&
        a + 0x2000 > 0x11fff && (a & 7) == 0 &&
        game_read(0, a + 0x10, &b, 8) != 0 &&
        b + 0x2000 > 0x11fff && (b & 7) == 0) {
        int64_t skin = default_skin_resolve();
        if (skin != 0)
            game_write(g_write_channel_fd, &skin, 8,
                       *(uintptr_t *)(self + 8) + 0xb18);
    }
}

void disable_skins_dynamic(long self)
{
    if (self == 0 || (g_disable_skins >> 2 & 1) == 0 ||
        g_restore_ready != 1)
        return;
    if (!tweak_enabled("DisableSkins"))
        return;
    uintptr_t base = *(uintptr_t *)(self + 8);
    uint64_t arr = 0;
    int32_t count = 0;
    if (game_read(0, base + 0x70, &arr, 8) != 0 &&
        arr + 0x2000 > 0x11fff && (arr & 7) == 0 &&
        game_read(0, base + 0x7c, &count, 4) != 0 &&
        count > 0 && count < 5) {
        uint32_t sel = 0;
        if (game_read(0, base + 0x80, &sel, 4) != 0 &&
            (int32_t)sel >= 0 && (int32_t)sel < count) {
            uint64_t entry = 0;
            if (game_read(0, arr + (uint64_t)sel * 8, &entry, 8) != 0 &&
                entry + 0x2000 > 0x11fff && (entry & 7) == 0) {
                uint64_t inner = 0;
                if (game_read(0, entry, &inner, 8) != 0 &&
                    inner + 0x2000 > 0x11fff && (inner & 7) == 0) {
                    int64_t skin = default_skin_resolve();
                    if (skin != 0) {
                        game_write(g_write_channel_fd, &skin, 8,
                                   base + 0x218);
                        game_write(g_write_channel_fd, &skin, 8,
                                   entry + 0x20);
                    }
                }
            }
        }
    }
}

void default_environments(long *self)
{
    if (self == NULL || g_default_env == 0 || g_restore_ready != 1)
        return;
    if (!tweak_enabled("DefaultEnvironments"))
        return;
    uintptr_t b = self[0x13];
    int32_t mode = 0, idx = 0;
    if (game_read(0, b + 0x20, &mode, 4) != 0 &&
        game_read(0, self[0] + 0x20, &idx, 4) != 0) {
        for (int i = 0; i < 0x2f4; i++) {
            const int *row = (const int *)g_env_table[i];
            if (row[4] == mode && row[5] == idx) {
                char name[0x60];
                game_string_format((void *)name, "%s", row[1]);
                uintptr_t next = (uintptr_t)game_style_deep(name, b);
                game_string_release((void *)name);
                if (next != 0) {
                    int32_t check = 0;
                    if (game_read(0, next + 0x20, &check, 4) != 0 &&
                        check == row[0])
                        self[0] = next;
                }
                break;
            }
        }
    }
}

void disable_shake(long self)
{
    if (self == 0 || g_disable_shake == 0 || g_restore_ready != 1)
        return;
    if (tweak_enabled("DisableShake"))
        *(uint32_t *)(self + 0x110) = 0;
}

int64_t skin_get_override(void *ctx, uint64_t a, uint64_t b)
{
    int64_t ret = ((int64_t (*)(void *, uint64_t, uint64_t))
                       g_skin_original)(ctx, a, b);
    if ((g_disable_skins & 1) != 0 && g_restore_ready == 1 &&
        tweak_enabled("DisableSkins")) {
        int64_t skin = default_skin_resolve();
        if (skin != 0)
            ret = skin;
    }
    return ret;
}

extern void *g_chat_original;
extern void *g_traj_original;
extern void *g_traj2_original;
extern void *g_traj_style_cache;
extern void *g_arrow_original;
extern void *g_skin_original;
extern uint64_t g_own_static_p;
extern char g_name_display_state;
extern char g_name_hook_armed;

void battle_proxy_apply(long self)
{
    int saved_errno = errno;
    ((void (*)(void))g_proxy_original)();
    if (g_battle_proxy_state == 3 && proxy_entitlement_ok() &&
        g_restore_ready == 1 && tweak_enabled("UseBattleProxy")) {
        char tag[0x48];
        uint64_t endpoints[4];
        int64_t skew = 0;
        pthread_mutex_lock(&g_proxy_mutex);
        memcpy(tag, g_proxy_tag_buf, sizeof tag);
        memcpy(endpoints, g_proxy_endpoints, sizeof endpoints);
        skew = g_proxy_skew;
        pthread_mutex_unlock(&g_proxy_mutex);

        uint64_t now = mono_ms();
        size_t tag_len = strlen(tag);
        int valid_tag = 0;
        if (tag_len >= 0x18) {
            valid_tag = 1;
            for (size_t i = 0; i < tag_len; i++) {
                if (memchr("0289PYLQGRJCUV", tag[i], 0xf) == NULL) {
                    valid_tag = 0;
                    break;
                }
            }
        }
        uint32_t count = 0;
        uint64_t ep = 0;
        uint8_t orig[0x10], key[0x30];
        if (valid_tag && skew != 0 && now >= (uint64_t)skew &&
            game_read(0, self + 0x90, &count, 4) != 0 &&
            (int32_t)count >= 1 && count <= 0xffff &&
            game_read(0, self + 0x98, &ep, 8) != 0 &&
            ep + 0x2000 > 0x11fff && (ep & 7) == 0 &&
            game_read(0, ep, orig, 0x10) != 0 &&
            proxy_endpoint_decode(ep, key) != 0) {
            char handshake[0xc00], tunnel[0x3e80], session[0x70];
            uint32_t session_id = 0;
            if (proxy_handshake(tag, key, count, (int64_t)now - skew,
                                handshake) != 0 &&
                proxy_tunnel(handshake, tunnel) != 0 &&
                proxy_decode(tunnel, session, &session_id) != 0 &&
                proxy_session_valid(tag) != 0 &&
                proxy_commit((void *)self, ep, orig, count, session,
                             session_id) != 0) {
                runtime_journal_write("battle_proxy_applied",
                                      "provider_endpoint_committed", NULL);
            } else {
                runtime_journal_write("battle_proxy_unavailable",
                                      "original_BSD_service_or_entitlement_"
                                      "required", NULL);
            }
        } else {
            runtime_journal_write("battle_proxy_refused",
                                  "session_or_udp_endpoint_unavailable",
                                  NULL);
        }
    }
    errno = saved_errno;
}

int proxy_session_valid(const char *tag)
{
    pthread_mutex_lock(&g_proxy_mutex);
    if (g_proxy_tag2 != *(const uint64_t *)(tag + 0x28)) {
        pthread_mutex_unlock(&g_proxy_mutex);
        return 0;
    }
    int same = strcmp(tag, g_proxy_tag_buf) == 0;
    pthread_mutex_unlock(&g_proxy_mutex);
    if (!same || !proxy_entitlement_ok() || g_restore_ready != 1)
        return 0;
    return tweak_enabled("UseBattleProxy");
}

void slow_mode_hook(uint64_t *out)
{
    if (out != NULL && g_slow_mode_hook != 0 &&
        tweak_enabled("SlowMode"))
        *out = 1;
}

void highlight_suppress(uint64_t a, uint64_t b)
{
    int saved_errno = errno;
    if ((g_highlight_hook & 1) != 0 &&
        !tweak_enabled("DoNotShowBattleHighlight")) {
        ((void (*)(uint64_t, uint64_t))g_highlight_original)(a, b);
    }
    errno = saved_errno;
}

uint32_t chat_button_enforce(void)
{
    uint32_t ret = ((uint32_t (*)(void))g_chat_button_original)();
    if ((g_highlight_hook >> 1 & 1) != 0 &&
        tweak_enabled("EnforceBattleChatButton"))
        ret = 1;
    return ret;
}

void chat_button_hook(long self)
{
    if (self != 0 && g_chat_button_mask == 7 &&
        tweak_enabled("EnforceBattleChatButton") &&
        frame_epoch_now() != 0)
        *(uint64_t *)(self + 0xb8) = 1;
}

double fps_limit_a(void)
{
    double v = (double)game_call_fps_limit();
    if ((g_gl_flags & 1) != 0 && g_hud_extra_state != 0 &&
        (uintptr_t)__builtin_return_address(0) == g_game_base + 0x7316cc) {
        int limit = tweak_value("FPSLimit");
        v = ((uint32_t)limit - 1 < 0x8fu) ? (double)limit : 600.0;
    }
    return v;
}

double fps_limit_b(void)
{
    double v = (double)game_call_fps_limit2();
    int limit_b = 0;
    if ((g_gl_flags & 1) != 0 && g_hud_extra_state != 0 &&
        (uintptr_t)__builtin_return_address(0) == g_game_base + 0x7316d4) {
        int limit = tweak_value("FPSLimit");
        limit_b = limit;
        v = ((uint32_t)limit - 1 < 0x8fu) ? (double)limit : 600.0;
    }
    (void)limit_b;
    return v;
}

void fps_limit_apply(uint32_t mode)
{
    lock_acquire(&g_dps_lock);
    if ((g_dps_lock & 1) == 0) {
        if ((g_gl_flags & 1) != 0 && g_hud_extra_state != 0 &&
            g_fps_mode_count - 0x21 > 0xffffffdfu) {
            int limit = tweak_value("FPSLimit");
            float target = ((uint32_t)limit - 1 < 0x8fu) ? (float)limit
                                                          : 600.0f;
            int32_t best = -1;
            for (uint32_t i = 0; i < g_fps_mode_count; i++) {
                float mode_hz = (float)g_fps_modes[i * 2];
                if (target <= mode_hz &&
                    (best < 0 || mode_hz < (float)g_fps_modes[best * 2]))
                    best = (int32_t)i;
            }
            if (best >= 0)
                mode = (uint32_t)g_fps_modes[best * 2];
        }
        g_fps_mode_count = 0;
        lock_release(&g_dps_lock);
    }
    ((void (*)(uint32_t))g_fps_apply_fn)(mode);
}

void name_display_hook(long self, uint64_t *name, uint32_t a3,
                       uint64_t a4, uint64_t a5, uint32_t a6,
                       uint32_t a7, uint32_t a8)
{
    int saved_errno = errno;
    uint64_t *effective = name;
    uint64_t own_copy[2] = {0, 0};
    int replaced = 0;

    if (g_name_hook_armed != 0 && (g_install_flags & 3) == 0 &&
        g_install_flags != 0 && g_bg_matchmaking != 0 &&
        g_hud_extra_state != 0) {
        lock_acquire(&g_name_lock);
        if ((g_name_lock & 1) == 0 && g_name_display_state != 0) {
            game_string_format(own_copy, ROD_DEFAULT_NAME);
            effective = own_copy;
            replaced = 1;
        }
        lock_release(&g_name_lock);
    }
    if ((g_install_flags & 3) == 0 && g_hud_extra_state != 0 &&
        tweak_enabled("ChromaticName"))
        a6 = 0xfffffffe;

    ((void (*)(long, uint64_t *, uint32_t, uint64_t, uint64_t, uint32_t,
               uint32_t, uint32_t))g_name_original)(self, effective, a3,
                                                     a4, a5, a6, a7, a8);

    char text[0xc0];
    int matched = 0;
    if (game_deref_read((uintptr_t)name, text, 0xc0) != 0 && text[0] != '\0') {
        uint64_t fields[2] = {0, 0};
        if (game_read(0, self + 0x14, &fields[1], 4) != 0 &&
            game_read(0, self + 0x18, &fields[0], 4) != 0)
            matched = 1;
        lock_acquire(&g_name_lock);
        if ((g_name_lock & 1) == 0) {
            g_name_cache_ms = 0;
            if (matched && g_name_epoch == (uint64_t)g_own_clock) {
                size_t l = strlen(text);
                memcpy(g_name_buffer, text, l + 1);
                g_name_fields[0] = fields[0];
                g_name_fields[1] = fields[1];
                g_name_cache_ms = 1;
            }
            lock_release(&g_name_lock);
        }
    }
    if (replaced)
        game_string_release(own_copy);
    errno = saved_errno;
}

void name_state_sync(long self)
{
    int saved_errno = errno;
    if (self == 0)
        return;
    uintptr_t name_obj = *(uintptr_t *)(self + 0x98);
    if (name_obj == 0)
        return;

    if ((g_install_flags & 3) == 0 && g_hud_extra_state != 0 &&
        tweak_enabled("ChromaticName")) {
        char text[0xc0];
        uint64_t f0 = 0;
        int32_t f1 = 0, f2 = 0;
        if (game_deref_read(name_obj, text, 0xc0) != 0 &&
            game_read(0, name_obj + 0x14, &f1, 4) != 0 &&
            game_read(0, name_obj + 0x18, &f2, 4) != 0 &&
            game_read(0, name_obj + 0x1c, &f0, 4) != 0) {
            lock_acquire(&g_name_lock);
            if ((g_name_lock & 1) == 0) {
                if (g_name_cache_ms != 0 &&
                    g_name_epoch == (uint64_t)g_own_clock &&
                    f1 == (int32_t)g_name_fields[0] &&
                    f2 == (int32_t)g_name_fields[1] &&
                    strcmp(text, g_name_buffer) == 0)
                    *(uint32_t *)(name_obj + 0x1c) = 0xfffffffe;
                lock_release(&g_name_lock);
            }
        }
    }

    if ((g_install_flags >> 3 & 1) != 0 && g_install_flags != 0 &&
        g_bg_matchmaking != 0 && g_hud_extra_state != 0) {
        char text[0x191];
        lock_acquire(&g_name_lock);
        if ((g_name_lock & 1) == 0) {
            memcpy(text, ROD_DEFAULT_NAME, 0x191);
            lock_release(&g_name_lock);
            char current[0xc0];
            int32_t b = 0, c = 0;
            if (text[0] != '\0' &&
                game_deref_read(name_obj, current, 0xc0) != 0 &&
                game_read(0, name_obj + 0x14, &b, 4) != 0 &&
                game_read(0, name_obj + 0x18, &c, 4) != 0) {
                lock_acquire(&g_name_lock);
                if ((g_name_lock & 1) == 0) {
                    if (g_name_cache_ms != 0 &&
                        g_name_epoch == (uint64_t)g_own_clock &&
                        b == (int32_t)g_name_fields[0] &&
                        c == (int32_t)g_name_fields[1] &&
                        strcmp(current, g_name_buffer) == 0) {
                        game_string_release((void *)name_obj);
                        game_string_format((void *)name_obj, "%s", text);
                    }
                    lock_release(&g_name_lock);
                }
            }
        } else {
            lock_release(&g_name_lock);
        }
    }
    errno = saved_errno;
}

void hide_battling_status(long self)
{
    if (self == 0 || (g_install_flags >> 2 & 1) == 0 ||
        g_hud_extra_state == 0)
        return;
    if (tweak_enabled("HideBattlingStatusFromOthers") &&
        *(int *)(self + 8) == 1)
        *(uint64_t *)(self + 8) = 3;
}

void instant_starr_drop(long self)
{
    if (self == 0 || *(long *)(self + 0xb8) == 0 ||
        *(long *)(self + 0xd0) == 0)
        return;
    if ((g_install_flags >> 3 & 1) == 0 || g_hud_extra_state == 0)
        return;
    if (tweak_enabled("InstantStarrDropOpening") &&
        *(int *)(self + 0x40) == 4)
        *(uint64_t *)(self + 0x40) = 0;
}

void friend_list_opt(long self)
{
    int saved_errno = errno;
    if (self == 0)
        return;
    uintptr_t list = *(uintptr_t *)(self + 0x98);
    if (list == 0 || (g_install_flags >> 4 & 1) == 0 ||
        g_hud_extra_state == 0)
        return;
    if (!tweak_enabled("FriendListOptimization"))
        return;
    uint64_t view = 0, child = 0;
    if (game_read(0, list, &view, 8) != 0 &&
        view + 0x2000 > 0x11fff && (view & 7) == 0 &&
        view == g_game_base + 0x11e28d8 &&
        game_read(0, list + 0x80, &child, 8) != 0 &&
        child + 0x2000 > 0x11fff && (child & 7) == 0) {
        uintptr_t player = (uintptr_t)game_find_child((void *)child, "player_icon");
        uintptr_t rank = (uintptr_t)game_find_child((void *)child, "rank_icon");
        uintptr_t picture = (uintptr_t)game_find_node((void *)child,
                                                      "friend_picture");
        uint64_t parent = 0;
        uint8_t flag = 0;
        if (player != 0 &&
            game_read(0, picture + 0x38, &parent, 8) != 0 &&
            parent + 0x10000 > 0x12000 && (parent & 7) == 0 &&
            parent == child &&
            game_read(0, player + 8, &flag, 1) != 0) {
            flag = 0;
            game_write(g_write_channel_fd, &flag, 1, player + 8);
        }
        if (rank != 0 &&
            game_read(0, rank + 0x38, &parent, 8) != 0 &&
            parent + 0x10000 > 0x12000 && (parent & 7) == 0 &&
            parent == child)
            game_release_node((void *)rank);
        if (picture != 0 &&
            game_read(0, picture + 0x38, &parent, 8) != 0 &&
            parent + 0x10000 > 0x12000 && (parent & 7) == 0 &&
            parent == child)
            game_release_node((void *)picture);
    }
    errno = saved_errno;
}

const char *skin_name_resolve(uintptr_t self)
{
    int saved_errno = errno;
    uintptr_t result = self;
    char name[0xc0];

    if (g_hud_extra_state != 0 && (g_skin_cache_count ^ 0xffffffffu) & 7) {
        if (g_install_flags & 0x20) {
            if (tweak_enabled("ShowSkinNamesInProfile") &&
                game_deref_read(self, name, 0x60) != 0) {
                lock_acquire(&g_skin_lock);
                if ((g_skin_lock & 1) == 0) {
                    if (g_skin_extra_a != 0 && g_env_2 != 0 &&
                        (uint64_t)g_env_ready == g_skin_extra_b &&
                        g_skin_extra_c != 0) {
                        for (uint32_t i = 0; i < g_skin_extra_c; i++) {
                            uintptr_t mapped = g_skin_map_out[i];
                            if (mapped != 0 &&
                                strcmp(name, g_skin_map_in[i]) == 0) {
                                result = mapped;
                                break;
                            }
                            result = self;
                        }
                    }
                    lock_release(&g_skin_lock);
                }
            }
        }
    }

    const char *out = ((const char *(*)(uintptr_t))g_skin_name_fn)(result);

    if (g_skin_extra_d != 0 && g_hud_extra_state != 0 &&
        game_deref_read(self, name, 0xc0) != 0 && name[0] != '\0') {
        lock_acquire(&g_skin_lock);
        if ((g_skin_lock & 1) == 0) {
            uint32_t count = g_skin_cache_count;
            if (count == 0) {
                if (server_refresh() != 0) {
                    size_t l = strlen(name);
                    memcpy(g_skin_cache[count], name, l + 1);
                    game_string_format(g_skin_cache_slots[count], name);
                    g_skin_cache_count = count + 1;
                    out = (const char *)g_skin_cache_slots[count];
                    lock_release(&g_skin_lock);
                    goto legacy;
                }
            } else {
                for (uint32_t i = 0; i < count; i++) {
                    if (strcmp(g_skin_cache[i], name) == 0) {
                        out = (const char *)g_skin_cache_slots[i];
                        lock_release(&g_skin_lock);
                        goto legacy;
                    }
                }
                if (count < 0x1000 && server_refresh() != 0) {
                    size_t l = strlen(name);
                    memcpy(g_skin_cache[count], name, l + 1);
                    game_string_format(g_skin_cache_slots[count], name);
                    g_skin_cache_count = count + 1;
                    out = (const char *)g_skin_cache_slots[count];
                    lock_release(&g_skin_lock);
                    goto legacy;
                }
            }
            lock_release(&g_skin_lock);
        }
    }

legacy:
    if ((g_install_flags >> 5 & 1) != 0 && g_hud_extra_state != 0 &&
        tweak_enabled("LegacyNames") &&
        game_deref_read(result, name, 0x40) != 0) {
        int table = -1;
        if (memcmp(name, "TID_MAP_D", 9) == 0 &&
            memcmp(name + 9, "_MAP_DE", 7) == 0)
            table = 0;
        else if (memcmp(name, "TID_MAP_D", 9) == 0 &&
                 memcmp(name + 9, "KSHOT_DU", 8) == 0)
            table = 1;
        else if (memcmp(name, "TID_MAP_D", 9) == 0 &&
                 memcmp(name + 9, "S", 1) == 0)
            table = 2;
        if (table >= 0) {
            char lang[8] = {0};
            uintptr_t def = (uintptr_t)game_default_skin();
            if (game_deref_read(def, lang, 0x10) == 0) {
                table = -1;
            } else {
                for (char *p = lang + 1; *p; p++) {
                    if ((uint8_t)p[-1] - 0x41u < 0x1au)
                        p[-1] = (char)(p[-1] + 0x20);
                }
                int l;
                if (memcmp(lang, "ru", 2) == 0 && lang[2] == 0)
                    l = 1;
                else if (memcmp(lang, "cn", 2) == 0 && lang[2] == 0)
                    l = 2;
                else if (memcmp(lang, "cnt", 3) == 0)
                    l = 3;
                else if (memcmp(lang, "tr", 2) == 0 && lang[2] == 0)
                    l = 4;
                else if (memcmp(lang, "pl", 2) == 0 && lang[2] == 0)
                    l = 5;
                else if (memcmp(lang, "it", 2) == 0 && lang[2] == 0)
                    l = 6;
                else if (memcmp(lang, "de", 2) == 0 && lang[2] == 0)
                    l = 7;
                else
                    l = 0;
                out = (const char *)g_legacy_names[table][l];
            }
        }
    }
    errno = saved_errno;
    return out;
}

void server_snapshot_fix(long self)
{
    int saved_errno = errno;
    int restored = 1;
    if (g_server_ready != 0) {
        uint32_t buf[5];
        if (game_read(0, self + 0x90, buf, 0x14) != 0) {
            uint32_t ping = buf[4];
            uint32_t sane = (ping == 1000 || (int32_t)ping < 1000)
                ? ping : 0xffffffffu;
            if (ping > 0x497c8)
                sane = 0xffffffffu;
            if (sane - 2 > 0x493deu)
                sane = 0xffffffffu;
            pthread_mutex_lock(&g_server_mutex);
            if (buf[0] < 1000000 && g_server_count < 0x21) {
                server_entry_add(&g_server_state, buf[0], sane, NULL);
            }
            pthread_mutex_unlock(&g_server_mutex);
            int region = tweak_value("BattleServerRegion") - 1;
            if ((uint32_t)region < 1000000) {
                restored = 0;
                if (buf[0] > 999999 || buf[4] > 0x7ffffc17)
                    goto call;
                uint32_t new_ping = (buf[0] != (uint32_t)region)
                    ? buf[4] + 1000 : buf[4];
                uint32_t repl[5] = {buf[0], new_ping, 5, 5, 10};
                uint32_t verify[5];
                if (game_write(g_write_channel_fd, repl, 0x14,
                               self + 0x90) == 0x14 &&
                    game_read(0, self + 0x90, verify, 0x14) != 0 &&
                    memcmp(verify, repl, 0x14) == 0) {
                    goto call;
                }
                uint32_t back[5] = {buf[0], buf[4], 5, 5, 10};
                if (game_write(g_write_channel_fd, back, 0x14,
                               self + 0x90) != 0x14 ||
                    game_read(0, self + 0x90, verify, 0x14) == 0 ||
                    memcmp(verify, back, 0x14) != 0)
                    abort();
            }
        }
    }
call:
    ((void (*)(long))g_server_fix_fn)(self);
    if (!restored) {
        uint32_t back[5];
        if (game_read(0, self + 0x90, back, 0x14) != 0) {
            if (game_write(g_write_channel_fd, back, 0x14,
                           self + 0x90) != 0x14)
                abort();
        }
    }
    errno = saved_errno;
}

void guarded_hook_cb_a(long *self)
{
    __atomic_fetch_add(&g_dps_meter_state, 1, __ATOMIC_ACQ_REL);
    g_dps_reason = (uint32_t)gettid();
    if (self != NULL && g_dps_meter_hook != 0 &&
        tweak_enabled("ShowDPS")) {
        uint64_t now = mono_ms();
        lock_acquire(&g_dps_lock);
        if ((g_dps_lock & 1) == 0) {
            if (g_dps_window_epoch != 0 && g_dps_sample_count != 0 &&
                g_dps_window_epoch <= now && now - g_dps_window_epoch < 0x1f5 &&
                (int32_t)g_dps_hits >= 0) {
                uint64_t pos = g_dps_sample_count & 0xff;
                g_dps_samples[pos * 2] = now;
                g_dps_samples[pos * 2 + 1] = -(uint64_t)self[1];
                if (g_dps_sample_count < 0x100)
                    g_dps_sample_count++;
            }
            lock_release(&g_dps_lock);
        }
    }
}

void ping_meter(long self)
{
    int saved_errno = errno;
    uint64_t epoch = frame_epoch_now();
    if (self != 0 && g_ping_hook != 0 &&
        tweak_enabled("ShowBattleConnectionIndicator") && epoch != 0) {
        int mode = *(int *)(self + 0xb8);
        if ((uint32_t)(mode - 1) >> 4 < 0x753) {
            g_ping_epoch = epoch;
            g_ping_value = mode;
            g_ping_ms = mono_ms();
        }
    }
    errno = saved_errno;
}

void background_matchmaking_a(uint64_t a, uint32_t b, uint32_t c)
{
    if (g_bg_matchmaking != 0 && g_bg_latch != 0 &&
        tweak_enabled("BackgroundMatchmaking"))
        return;
    ((void (*)(uint64_t, uint32_t, uint32_t))g_bg_fn)(a, b, c);
}

void background_matchmaking_b(uint64_t a, uint64_t b, uint64_t c,
                              uint32_t d, uint32_t e, uint32_t f)
{
    if (g_bg_matchmaking != 0 && g_bg_latch != 0 &&
        tweak_enabled("BackgroundMatchmaking")) {
        g_bg_pending = 0;
        ((void (*)(uint64_t))(g_game_base + 0xf481f4))(a);
        return;
    }
    g_bg_pending = 0;
    ((void (*)(uint64_t, uint64_t, uint64_t, uint32_t, uint32_t, uint32_t))
         g_bg_fn2)(a, b, c, d, e, f);
}

void guarded_hook_cb_b(long *self)
{
    int saved_errno = errno;
    if (self == NULL || g_black_bars_hook == 0 || g_main_thread_id == 0 ||
        g_main_thread_id != gettid())
        return;
    uintptr_t id = *self;
    if (g_black_bars_epoch != id) {
        memset(g_black_bars_state, 0, sizeof g_black_bars_state);
        g_black_bars_epoch = id;
    }
    int hide = (tweak_value("HideBattleBlackBars") == 0);
    for (int i = 0; i < 4; i++) {
        uint64_t bar = 0;
        if (game_read(0, id + 0x120 + i * 8, &bar, 8) != 0 &&
            bar + 0x2000 > 0x11fff && (bar & 7) == 0) {
            uint8_t flag = 0;
            if (game_read(0, bar + 8, &flag, 1) != 0) {
                if (g_black_bars_state[i] != bar) {
                    g_black_bars_state[i] = bar;
                    g_black_bars_mask &= ~(1u << i);
                }
                uint32_t bit = 1u << i;
                if (hide) {
                    if ((g_black_bars_mask & bit) != 0 && flag == 0) {
                        game_write(g_write_channel_fd,
                                   &g_black_bars_saved, 1, bar + 8);
                        g_black_bars_mask &= ~bit;
                    }
                } else {
                    if ((g_black_bars_mask & bit) == 0) {
                        g_black_bars_saved = flag;
                        g_black_bars_mask |= bit;
                    }
                    uint8_t zero = 0;
                    game_write(g_write_channel_fd, &zero, 1, bar + 8);
                }
            }
        }
    }
    errno = saved_errno;
}

static int script_port_hook_available(const char *name)
{
    if (name == NULL || g_install_flags == 0)
        return 0;
    if (strcmp(name, "ShowCharactersInNames") == 0 ||
        strcmp(name, "ShamePlayersWithThumbsdownPin") == 0)
        return 1;
    if (g_bg_matchmaking != 1)
        return 0;
    return strcmp(name, "ShowFriendsInBattle") == 0 ||
           strcmp(name, "ShowAllianceMembersInBattle") == 0;
}

static int script_port_feature_available(const char *name)
{
    if (name == NULL)
        return 0;
    if (strcmp(name, "TeammateHPIndicator") == 0)
        return g_teammate_arrow_ready != 0;
    if (strcmp(name, "AllyRespawnTimer") == 0)
        return (g_ally_respawn_ready != 0 && g_battle_ready == 1);
    if (g_battle_ready != 1)
        return 0;
    if (strcmp(name, "TileGrid") == 0 ||
        strcmp(name, "ShowOwnPlayerCoordinates") == 0 ||
        strcmp(name, "ShowFPSCounter") == 0 ||
        strcmp(name, "SmoothHud") == 0)
        return 1;
    return strcmp(name, "SmoothHudGraph") == 0;
}

static int g_camera_state_ready(void)
{
    return g_camera_state != 0;
}

static int proxy_entitlement_ok(void)
{
    static pthread_once_t once = PTHREAD_ONCE_INIT;
    extern void proxy_entitlement_init(void);
    pthread_once(&once, proxy_entitlement_init);
    if (g_proxy_entitlement_fn == NULL || g_proxy_entitlement_fn() != 1)
        return 0;
    return 1;
}
