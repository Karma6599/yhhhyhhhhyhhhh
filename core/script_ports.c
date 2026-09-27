#define _GNU_SOURCE 1

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#define SP_TAG_ALPHABET "0289PYLQGRJCUV"
#define SP_TAG_BASE 13
#define SP_TAG_MAX_DIGITS 11
#define SP_PROFILE_SNAPSHOT_SIZE 0x1dd0
#define SP_THEME_SNAPSHOT_SIZE 0x2534
#define SP_BATTLE_SNAPSHOT_SIZE 0x30
#define SP_BATTLE_FRAME_OFF 0x1307e20u

extern int  runtime_state_ready(void);
extern int  runtime_battle_gate(void);
extern int  runtime_reader_open(uint64_t handle, uint64_t addr, void *out, uint32_t len);
extern uint64_t runtime_reader_handle(void);
extern uint64_t runtime_game_base(void);
extern int  runtime_read_game(uint64_t handle, uint64_t addr, void *out, uint32_t len);
extern int camera_commit(void);
extern int  profile_slots_ready(void);
extern int  profile_name_valid(const void *name, size_t len);
extern int  client_debug_context_open(void *ctx_out);
extern int  client_debug_command_context(void *ctx_out);
extern int  client_debug_command_valid(void *ctx);
extern int  client_debug_snapshot_context(void *ctx_out);
extern int  client_debug_apply_context(void *ctx_out);
extern int  client_editor_context_open(void *ctx_out);
extern int  client_editor_command_valid(void *ctx);
extern int  client_editor_state_valid(void *ctx);
extern int  client_editor_map_context(void *ctx_out);
extern int  theme_store_count(void);
extern int  theme_store_entry(int index, void *entry_out);
extern int  theme_entry_name_valid(const void *entry);
extern int  theme_store_flush(void);
extern int  theme_file_load(void);
extern int  theme_file_save(void);

static const int32_t camera_axis_min[5] = { 0, -10000, -10000, -10000, -10000 };
static const int32_t camera_axis_max[5] = { 200, 10000, 10000, 10000, 10000 };

static volatile uint8_t camera_lock_byte;
static int32_t camera_axis[5];
static uint64_t camera_generation;
static uint32_t camera_mode;
static uint32_t camera_dirty;

static volatile uint8_t battle_lock_byte;
static uint32_t battle_seq;
static uint32_t battle_ready;
static uint32_t battle_phase;
static uint64_t battle_epoch;
static uint64_t battle_last_frame;
static uint64_t battle_ctx;
static uint64_t battle_stamp;

static volatile uint8_t profile_lock_byte;
static uint64_t profile_generation;
static uint64_t profile_last_open_ms;
static uint32_t profile_slot_tag;
static uint32_t profile_slot_hi;
static char profile_name[0x200];
static uint8_t profile_cache[0x285d0];
static uint8_t profile_name_dirty;

static uint32_t editor_availability;
static uint32_t editor_toggle_bits;
static uint32_t editor_toggle_extra;
static uint32_t editor_state_kind;

static void sp_camera_lock(void)
{
    while (__atomic_test_and_set(&camera_lock_byte, __ATOMIC_ACQUIRE)) {
    }
}

static void sp_camera_unlock(void)
{
    __atomic_clear(&camera_lock_byte, __ATOMIC_RELEASE);
}

static void sp_battle_lock(void)
{
    while (__atomic_test_and_set(&battle_lock_byte, __ATOMIC_ACQUIRE)) {
    }
}

static void sp_battle_unlock(void)
{
    __atomic_clear(&battle_lock_byte, __ATOMIC_RELEASE);
}

static void sp_profile_lock(void)
{
    while (__atomic_test_and_set(&profile_lock_byte, __ATOMIC_ACQUIRE)) {
    }
}

static void sp_profile_unlock(void)
{
    __atomic_clear(&profile_lock_byte, __ATOMIC_RELEASE);
}

static uint64_t sp_now_ms(void)
{
    struct timespec ts;

    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
        return 0;
    }
    return (uint64_t)ts.tv_sec * 1000 + (uint64_t)ts.tv_nsec / 1000000;
}

int nexus_script_port_camera_control(uint32_t op, uint32_t axis, int32_t value)
{
    int ok;

    if (op < 1 || op > 5) {
        return 0;
    }
    if (camera_lock_byte == 1) {
        return 0;
    }
    if (op == 1) {
        if (axis > 4) {
            return 0;
        }
        if (value < camera_axis_min[axis] || value > camera_axis_max[axis]) {
            return 0;
        }
    } else if (op == 2 && value > 3) {
        return 0;
    }
    sp_camera_lock();
    if (op == 1) {
        camera_axis[axis] = value;
        camera_dirty = 1;
        camera_generation++;
        sp_camera_unlock();
        return 1;
    }
    if (op == 4) {
        camera_axis[1] = 0;
        camera_axis[2] = 0;
        camera_axis[3] = 0;
        camera_axis[4] = 0;
        camera_axis[0] = 100;
        camera_dirty = 1;
    } else if (op == 3) {
        int was_default = camera_axis[0] != 100;
        camera_axis[0] = 60;
        if (was_default) {
            camera_axis[0] = 100;
        }
        camera_dirty = 1;
    }
    if (op == 2) {
        camera_mode = (uint32_t)value;
    }
    camera_generation++;
    ok = camera_commit();
    sp_camera_unlock();
    return ok;
}

int nexus_script_port_battle_snapshot(uint32_t *desc, size_t size)
{
    uint64_t now_ms;
    uint64_t frame_a = 0;
    uint64_t frame_b = 0;
    uint64_t sub_a = 0;
    uint64_t sub_b = 0;
    uint32_t kind_a = 0;
    uint32_t kind_b = 0;
    uint32_t ready = 0;
    uint32_t phase = 0;
    uint64_t ctx = 0;
    uint64_t epoch = 0;
    uint64_t handle;
    pthread_t self;
    char name[16];

    if (desc == NULL || size != SP_BATTLE_SNAPSHOT_SIZE) {
        return 0;
    }
    if (desc[0] != 1 || desc[1] != SP_BATTLE_SNAPSHOT_SIZE) {
        return 0;
    }
    now_ms = sp_now_ms();
    handle = runtime_reader_handle();
    if (handle == 0) {
        self = pthread_self();
        if (pthread_getname_np(self, name, sizeof(name)) == 0 &&
            strcmp(name, "MainPool") == 0) {
            goto probe;
        }
        phase = 5;
        goto emit;
    }
    if (handle != (uint64_t)gettid()) {
        phase = 5;
        goto emit;
    }
probe:
    if (!runtime_battle_gate()) {
        phase = 5;
        goto emit;
    }
    if (runtime_read_game(handle, runtime_game_base() + SP_BATTLE_FRAME_OFF, &frame_a, 8) != 1 ||
        frame_a < 0x10000 || (frame_a & 7) != 0 ||
        runtime_read_game(handle, frame_a + 0x50, &kind_a, 4) != 1 ||
        runtime_read_game(handle, frame_a + 0x48, &sub_a, 8) != 1 ||
        sub_a < 0x10000 || (sub_a & 7) != 0) {
        phase = 4;
        goto emit;
    }
    if (runtime_read_game(handle, runtime_game_base() + SP_BATTLE_FRAME_OFF, &frame_b, 8) != 1 ||
        frame_b < 0x10000 || (frame_b & 7) != 0 ||
        runtime_read_game(handle, frame_b + 0x50, &kind_b, 4) != 1) {
        phase = 4;
        goto emit;
    }
    if (kind_b == 5) {
        if (runtime_read_game(handle, frame_b + 0x48, &sub_b, 8) != 1 ||
            sub_b < 0x10000 || (sub_b & 7) != 0) {
            phase = 4;
            goto emit;
        }
    } else {
        sub_b = 0;
    }
    if (frame_a != frame_b || kind_a != kind_b || sub_a != sub_b) {
        phase = 4;
        goto emit;
    }
    if (kind_a == 5) {
        if (sub_b == 0) {
            phase = 2;
            goto emit;
        }
        ctx = sub_b;
        ready = 1;
        epoch = 1;
    } else {
        ctx = 0;
        ready = 0;
        epoch = 1;
        phase = 2;
    }
emit:
    sp_battle_lock();
    if (ready && epoch) {
        if (ctx == battle_last_frame) {
            epoch = battle_epoch | 0x8000000000000000ull;
        } else {
            battle_epoch++;
            if (battle_epoch == 0) {
                battle_epoch = 1;
            }
            battle_last_frame = ctx;
            epoch = battle_epoch | 0x8000000000000000ull;
        }
    } else {
        if (battle_epoch != 0) {
            epoch = battle_epoch | 0x8000000000000000ull;
        }
        if (ready == 0) {
            battle_last_frame = 0;
        }
    }
    if (battle_ready == ready && battle_phase == phase && battle_epoch == epoch &&
        battle_seq != 0) {
        /* keep seq */
    } else {
        battle_seq++;
        if (battle_seq == 0) {
            battle_seq = 1;
        }
    }
    battle_ready = ready;
    battle_phase = phase;
    battle_epoch = epoch;
    battle_ctx = ctx;
    battle_stamp = now_ms;
    desc[0] = 0x54524142u;
    desc[2] = battle_seq;
    desc[3] = ready;
    desc[4] = epoch >> 32;
    desc[5] = phase;
    desc[6] = epoch & 0xffffffff;
    desc[7] = (uint32_t)(ctx >> 32);
    desc[8] = (uint32_t)ctx;
    desc[9] = (uint32_t)(now_ms >> 32);
    desc[10] = (uint32_t)now_ms;
    sp_battle_unlock();
    return 1;
}

static uint64_t sp_tag_decode(const char *tag, size_t len)
{
    uint64_t value = 0;
    size_t i;

    for (i = 0; i < len; i++) {
        const char *p = memchr(SP_TAG_ALPHABET, tag[i], SP_TAG_BASE);
        if (p == NULL) {
            return 0;
        }
        value = value * SP_TAG_BASE + (uint64_t)(p - SP_TAG_ALPHABET);
    }
    return value;
}

int nexus_script_port_profile_open(const char *tag, size_t len)
{
    uint64_t decoded;
    uint64_t now_ms;
    int saved_errno;
    uint64_t acquire_fn;
    uint64_t obj_b;

    if ((runtime_state_ready() & 7) != 7) {
        return 0;
    }
    if (tag == NULL || len < 0x21 || len > 0x40) {
        return 0;
    }
    {
        size_t start = 0;
        size_t end = len;
        while (start < len && (tag[start] == '\n' || tag[start] == '\r' ||
                               tag[start] == ' ' || tag[start] == '\t')) {
            start++;
        }
        while (end > start && (tag[end - 1] == '\n' || tag[end - 1] == '\r' ||
                               tag[end - 1] == ' ' || tag[end - 1] == '\t')) {
            end--;
        }
        if (tag[start] == '#') {
            start++;
        }
        if (end <= start || end - start > SP_TAG_MAX_DIGITS) {
            return 0;
        }
        decoded = sp_tag_decode(tag + start, end - start);
        if (decoded == 0) {
            return 0;
        }
        len = end - start;
        tag += start;
    }
    now_ms = sp_now_ms();
    if (profile_last_open_ms != 0 && now_ms >= profile_last_open_ms &&
        now_ms - profile_last_open_ms <= 499) {
        return 0;
    }
    acquire_fn = ((uint64_t (*)(uint32_t))(runtime_game_base() + 0x11a2840))(0x270);
    if (acquire_fn == 0) {
        return 0;
    }
    obj_b = ((uint64_t (*)(uint32_t))(runtime_game_base() + 0x11a2840))(8);
    if (obj_b == 0) {
        ((void (*)(uint64_t))(runtime_game_base() + 0x11a2830))(acquire_fn);
        return 0;
    }
    saved_errno = errno;
    sp_profile_lock();
    profile_generation++;
    profile_slot_tag = (uint32_t)(decoded & 0xff);
    profile_slot_hi = (uint32_t)(decoded >> 8);
    memcpy(profile_name, tag, len);
    profile_name[len] = 0;
    profile_name_dirty = 1;
    memset(profile_cache, 0, sizeof(profile_cache));
    profile_last_open_ms = now_ms;
    sp_profile_unlock();
    ((void (*)(uint64_t, uint32_t, uint32_t))(runtime_game_base() + 0x541848))
        (acquire_fn, (uint32_t)decoded & 0xff, (uint32_t)(decoded >> 8));
    ((void (*)(uint64_t, uint64_t, int, int, int, int, int))(runtime_game_base() + 0x8a3138))
        (obj_b, acquire_fn, 4, 0, 0, 0, 0);
    ((void (*)(uint64_t))(runtime_game_base() + 0x89922c))(obj_b);
    errno = saved_errno;
    return 1;
}

extern int profile_dir_fd(void);
extern char *profile_cache_name_slot(void);
extern uint32_t profile_entry_count(void);
extern const void *profile_entry_at(uint32_t index);
extern int client_debug_context_valid(void);
extern uint32_t debug_availability_mask(void);
extern int debug_command_execute(uint32_t command, uint32_t arg, int32_t a3,
                                 uint32_t a4, int32_t a5);
extern int editor_session_active(void);
extern int editor_tick(void);
extern int editor_snapshot_fill(uint32_t *desc, const void *ctx);
extern int editor_command_confirm(void *ctx, const void *map_ctx);
extern int editor_command_flush(void *ctx, const void *map_ctx);
extern int editor_command_execute(uint32_t command, uint32_t arg, int32_t a3,
                                  uint32_t a4, int32_t a5, void *ctx);
extern uint64_t theme_generation(void);
extern int theme_count_cached(void);
extern int theme_command_execute(uint32_t command, uint32_t arg);

int nexus_script_port_profile_set_name(const void *name, size_t len)
{
    char tmp[0x208];
    int fd;
    int saved_errno;

    if ((runtime_state_ready() >> 3 & 1) == 0) {
        return 0;
    }
    if (!profile_slots_ready()) {
        return 0;
    }
    if (!profile_name_valid(name, len)) {
        return 0;
    }
    saved_errno = errno;
    sp_profile_lock();
    memcpy(tmp, "NPCN69 1\n", 9);
    if (len != 0) {
        memcpy(tmp + 9, name, len);
    }
    fd = openat(profile_dir_fd(), "script-port-profile-name.v1.tmp",
                O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (fd < 0) {
        sp_profile_unlock();
        errno = saved_errno;
        return 0;
    }
    if (write(fd, tmp, len + 9) != (ssize_t)(len + 9)) {
        close(fd);
        sp_profile_unlock();
        errno = saved_errno;
        return 0;
    }
    {
        int sync_ok = fsync(fd);
        close(fd);
        if (sync_ok != 0 ||
            renameat(profile_dir_fd(), "script-port-profile-name.v1.tmp",
                     profile_dir_fd(), "script-port-profile-name.v1") != 0) {
            sp_profile_unlock();
            errno = saved_errno;
            return 0;
        }
        fsync(profile_dir_fd());
    }
    memcpy(profile_cache_name_slot(), name, len);
    profile_cache_name_slot()[len] = 0;
    profile_name_dirty = 1;
    profile_generation++;
    sp_profile_unlock();
    errno = saved_errno;
    return 1;
}

int nexus_script_port_profile_snapshot(uint32_t *desc, size_t size, uint32_t from_index)
{
    uint32_t flags;
    uint32_t copied;
    uint32_t total;
    uint32_t i;

    if (desc == NULL || size != SP_PROFILE_SNAPSHOT_SIZE) {
        return 0;
    }
    if (desc[0] != 1 || desc[1] != SP_PROFILE_SNAPSHOT_SIZE) {
        return 0;
    }
    flags = 0;
    if ((runtime_state_ready() & 7) == 7) {
        flags |= 3;
    }
    if ((runtime_state_ready() >> 3 & 1) != 0 && profile_slots_ready()) {
        flags |= 4;
    }
    if (flags == 0) {
        return 0;
    }
    sp_profile_lock();
    memcpy(desc, profile_cache, 0x113 * 4);
    desc[8] = from_index;
    desc[9] = 0;
    desc[0] = 0x4f525043u;
    memset(&desc[0x113], 0, 0x1980);
    total = profile_entry_count();
    if (from_index <= total && total - from_index != 0) {
        copied = 0;
        for (i = from_index; i < total && copied < 0x20; i++, copied++) {
            memcpy(&desc[0x113] + copied * 0xcc / 4, profile_entry_at(i), 0xcc);
        }
        desc[9] = copied;
    }
    desc[2] = (desc[0x113] != 0 || copied != 0) ? 1 : 0;
    sp_profile_unlock();
    if (!theme_store_flush()) {
        desc[2] = 0;
    }
    return desc[2] != 0;
}

int nexus_script_port_client_debug_mask(uint32_t *mask)
{
    if (mask == NULL) {
        return 0;
    }
    if (!client_debug_context_valid()) {
        return 0;
    }
    *mask = debug_availability_mask();
    return 1;
}

int nexus_script_port_client_debug_snapshot(uint32_t *desc, size_t size)
{
    uint8_t ctx[0x90];

    if (desc == NULL || size != 0x1dd0) {
        return 0;
    }
    if (!client_debug_snapshot_context(ctx)) {
        return 0;
    }
    {
        uint64_t view = 0;
        uint64_t hud = 0;
        uint32_t fps = 0;
        if (!runtime_read_game(runtime_reader_handle(), ((uint64_t *)ctx)[3] + 0x58,
                               &view, 8) ||
            view < 0x10000 || (view & 7) != 0) {
            return 0;
        }
        if (runtime_read_game(runtime_reader_handle(), ((uint64_t *)ctx)[4] + 0x980,
                              &fps, 1) != 1 || fps != 0) {
            return 0;
        }
        if (runtime_read_game(runtime_reader_handle(), view + 0xc4, &hud, 4) != 1 ||
            runtime_read_game(runtime_reader_handle(), view + 0xc8, &hud + 1, 4) != 1) {
            return 0;
        }
        if (runtime_read_game(runtime_reader_handle(), ((uint64_t *)ctx)[4] + 0xab0,
                              &fps, 4) != 1 || fps >= 2) {
            return 0;
        }
        if (runtime_read_game(runtime_reader_handle(), ((uint64_t *)ctx)[4] + 0x9f8,
                              &fps, 4) != 1 || (int32_t)fps < -1 || (int32_t)fps > 4) {
            return 0;
        }
        desc[0] = (uint32_t)((uint64_t *)ctx)[0];
        desc[1] = (uint32_t)(((uint64_t *)ctx)[0] >> 32);
        desc[2] = (uint32_t)((uint64_t *)ctx)[2];
        desc[3] = (uint32_t)((uint64_t *)ctx)[1];
        desc[4] = (uint32_t)view;
        desc[5] = (uint32_t)(((uint64_t *)ctx)[3] >> 32);
        desc[6] = (uint32_t)(((uint64_t *)ctx)[1] >> 32);
        desc[7] = (uint32_t)(((uint64_t *)ctx)[1] >> 32);
        desc[8] = (uint32_t)hud;
        desc[9] = (uint32_t)(hud >> 32);
        desc[10] = fps;
        return 1;
    }
}

int nexus_script_port_client_debug_apply(uint32_t command, uint32_t arg,
                                         int32_t a3, uint32_t a4, int32_t a5)
{
    if (command > 39) {
        return 0;
    }
    if (command != 9 && (a4 != 0 || a3 != 0 || a5 != 0)) {
        return 0;
    }
    if (command == 4 && (arg > 4 || a4 != 0 || a3 != 0 || a5 != 0)) {
        return 0;
    }
    return debug_command_execute(command, arg, a3, a4, a5);
}

int nexus_script_port_client_editor_tick(void)
{
    if (!editor_session_active()) {
        return 0;
    }
    return editor_tick();
}

int nexus_script_port_client_editor_snapshot(uint32_t *desc, size_t size)
{
    uint8_t ctx[0x58];

    if (desc == NULL || size != 0x1dd0) {
        return 0;
    }
    if (!client_editor_context_open(ctx)) {
        return 0;
    }
    if (!client_editor_command_valid(ctx)) {
        return 0;
    }
    return editor_snapshot_fill(desc, ctx);
}

int nexus_script_port_client_editor_apply(uint32_t command, uint32_t arg,
                                          int32_t a3, uint32_t a4, int32_t a5)
{
    uint8_t ctx[0x120];
    uint8_t map_ctx[0x58];

    if (command > 0x13) {
        return 0;
    }
    if (command != 9 && (a4 != 0 || a3 != 0 || a5 != 0 || arg != 0)) {
        return 0;
    }
    if (command == 4 && (arg > 4 || a4 != 0 || a3 != 0 || a5 != 0)) {
        return 0;
    }
    if (!editor_session_active() || !runtime_battle_gate()) {
        return 0;
    }
    if (!client_editor_map_context(map_ctx)) {
        return 0;
    }
    if ((editor_availability & (1u << (command & 0x1f))) == 0) {
        return 0;
    }
    if (command >= 0xe && command <= 0x10) {
        while (__atomic_test_and_set(&editor_toggle_bits, __ATOMIC_RELAXED)) {
        }
        editor_toggle_bits ^= 1u << (command & 0x1f);
        if (command == 0x10) {
            editor_toggle_extra = 1;
        }
        __atomic_clear(&editor_toggle_bits, __ATOMIC_RELEASE);
        editor_state_kind = 1;
        return 1;
    }
    if (command == 0) {
        return editor_command_confirm(ctx, map_ctx);
    }
    if (command == 0x12) {
        return editor_command_flush(ctx, map_ctx);
    }
    if (!client_editor_state_valid(ctx)) {
        return 0;
    }
    return editor_command_execute(command, arg, a3, a4, a5, ctx);
}

int nexus_script_port_theme_snapshot(uint32_t *desc, size_t size, uint32_t max_themes)
{
    int count;
    int i;
    uint32_t named = 0;

    if (desc == NULL || size != SP_THEME_SNAPSHOT_SIZE) {
        return 0;
    }
    if (desc[0] != 1 || desc[1] != SP_THEME_SNAPSHOT_SIZE) {
        return 0;
    }
    if (!theme_store_flush()) {
        return 0;
    }
    theme_file_load();
    theme_file_save();
    desc[4] = 0;
    desc[5] = max_themes;
    desc[2] = 0;
    desc[3] = theme_generation();
    desc[0] = 0x4d454854u;
    desc[6] = 0;
    desc[7] = theme_count_cached() != 0 ? 0xf : 3;
    memset(&desc[0xd], 0, 0x2500);
    count = theme_store_count();
    if (count < 0x1001 && count > 0) {
        for (i = 0; i < count; i++) {
            uint8_t entry[0x128];
            if (theme_store_entry(i, entry) < 0) {
                desc[4] = 0;
                desc[6] = 0;
                goto done;
            }
            if (theme_entry_name_valid(entry)) {
                if (named < max_themes && named < 8) {
                    memcpy(&desc[0xd] + named * 0x4a, entry, 0x128);
                    named++;
                    desc[4] = named;
                }
            }
        }
        desc[2] = desc[4] != 0;
    }
done:
    if (!theme_store_flush()) {
        desc[2] = 0;
    }
    return desc[2] != 0;
}

int nexus_script_port_theme_command(uint32_t command, uint32_t arg)
{
    if (command > 4) {
        return 0;
    }
    return theme_command_execute(command, arg);
}

extern uint64_t theme_generation(void);
extern int theme_count_cached(void);
extern int theme_command_execute(uint32_t command, uint32_t arg);
extern int runtime_installed(void);
extern int runtime_acknowledged(void);
extern uint32_t runtime_owner_tid(void);
extern uint64_t runtime_callbacks(void);
extern uint32_t runtime_accepted(void);
extern uint32_t runtime_rejected(void);
extern uint32_t runtime_remaining(void);
extern uint32_t runtime_interval_ms(void);
extern uint64_t runtime_max_duration_us(void);
extern uint32_t runtime_capture_queue(void);
extern uint32_t runtime_capture_bytes(void);
extern void runtime_capture_set(int remaining, int interval_ms);
extern uint64_t runtime_frame_telemetry[26];

static volatile uint8_t runtime_consumer_lock;
static uint64_t runtime_consumer_word0;
static uint64_t runtime_consumer_ctx;
static uint64_t runtime_consumer_fn;

int nexus_evasion_runtime_template_v1(uint64_t *desc)
{
    if (desc == NULL) {
        return 0;
    }
    if ((uint32_t)desc[0] != 1 || (uint32_t)(desc[0] >> 32) != 0x28) {
        return 0;
    }
    if (0x1cb6c0 - 0x1cb6b0 != 0x150 || 0x1cb6b8 - 0x1cb6b0 != 0x148 ||
        0x1cb6c8 - 0x1cb6b0 != 0x144 || 0x1cb6d0 - 0x1cb6b0 != 0x158) {
        return 0;
    }
    desc[1] = 0x1cb6b0;
    desc[0] = 0x54434e52u;
    desc[2] = 0x504d5554u;
    desc[3] = 0x112a50;
    desc[4] = 0x10e6d8;
    return 1;
}

int nexus_evasion_runtime_frame_consumer_v1(const uint64_t *desc)
{
    if (desc == NULL) {
        while (__atomic_test_and_set(&runtime_consumer_lock, __ATOMIC_ACQUIRE)) {
        }
        runtime_consumer_fn = 0;
        runtime_consumer_ctx = 0;
        runtime_consumer_word0 = 0;
        __atomic_clear(&runtime_consumer_lock, __ATOMIC_RELEASE);
        return 1;
    }
    if ((uint32_t)desc[0] != 1 || (uint32_t)(desc[0] >> 32) != 0x18 || desc[2] == 0) {
        return 0;
    }
    while (__atomic_test_and_set(&runtime_consumer_lock, __ATOMIC_ACQUIRE)) {
    }
    runtime_consumer_word0 = desc[0];
    runtime_consumer_ctx = desc[1];
    runtime_consumer_fn = desc[2];
    __atomic_clear(&runtime_consumer_lock, __ATOMIC_RELEASE);
    return 1;
}

size_t nexus_evasion_runtime_diagnostics_v1(char *buf, size_t n)
{
    int len;

    if (buf == NULL && n != 0) {
        return 0;
    }
    while (__atomic_test_and_set(&runtime_consumer_lock, __ATOMIC_ACQUIRE)) {
    }
    len = snprintf(buf, n,
                   "{\"schema\":1,\"component\":\"nexus_evasion_runtime\","
                   "\"installed\":%d,\"callback_acknowledged\":%d,"
                   "\"owner_tid\":%u,\"callbacks\":%llu,"
                   "\"accepted\":%u,\"rejected\":%u,\"remaining\":%u,"
                   "\"interval_ms\":%u,\"max_duration_us\":%llu,"
                   "\"gameplay_effective\":false}",
                   runtime_installed(), runtime_acknowledged(), runtime_owner_tid(),
                   (unsigned long long)runtime_callbacks(), runtime_accepted(),
                   runtime_rejected(), runtime_remaining(), runtime_interval_ms(),
                   (unsigned long long)runtime_max_duration_us());
    __atomic_clear(&runtime_consumer_lock, __ATOMIC_RELEASE);
    if (len < 0) {
        return 0;
    }
    return (size_t)len + 1;
}

int nexus_evasion_runtime_capture_v1(int remaining, int interval_ms)
{
    if (remaining < 0x101 || remaining > 0x200) {
        return 0;
    }
    if (interval_ms < 0x7d1 || interval_ms > 0x860) {
        return 0;
    }
    if (!runtime_battle_gate()) {
        return 0;
    }
    while (__atomic_test_and_set(&runtime_consumer_lock, __ATOMIC_ACQUIRE)) {
    }
    if (runtime_capture_queue() >= 0x13f || runtime_capture_bytes() >= 520000) {
        __atomic_clear(&runtime_consumer_lock, __ATOMIC_RELEASE);
        return 0;
    }
    runtime_capture_set(remaining, interval_ms);
    __atomic_clear(&runtime_consumer_lock, __ATOMIC_RELEASE);
    return 1;
}

int nexus_evasion_runtime_frame_v1(uint64_t *desc)
{
    if (desc == NULL) {
        return 0;
    }
    if ((uint32_t)desc[0] != 1 || (uint32_t)(desc[0] >> 32) != 0xd0) {
        return 0;
    }
    while (__atomic_test_and_set(&runtime_consumer_lock, __ATOMIC_ACQUIRE)) {
    }
    memcpy(desc, runtime_frame_telemetry, 0xd0);
    __atomic_clear(&runtime_consumer_lock, __ATOMIC_RELEASE);
    return 1;
}
