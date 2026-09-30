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
#include <dlfcn.h>
#include <link.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <math.h>

#define TRAMPOLINE_SLOTS  4
#define HUD_FIELD_SLOTS   64
#define DEDUP_SLOTS       64
#define ROSTER_MAX        64
#define GUARD_COUNT       23
#define BODY_FONT_SIZE    0x10

#define ROD_STATUS_HDR    (*(const uint64_t *)(uintptr_t)0x10e7f8)
#define ROD_FRAME_TAG     (*(const uint64_t *)(uintptr_t)0x10e590)
#define ROD_QUEUE_TAG     (*(const uint64_t *)(uintptr_t)0x10e740)
#define ROD_REGISTRY_TAG  (*(const uint64_t *)(uintptr_t)0x10e700)
#define ROD_MOVE_TAG      (*(const uint64_t *)(uintptr_t)0x10e670)
#define ROD_JOURNAL_EMPTY ((const char *)(uintptr_t)0x117c20)

extern int __android_log_print(int prio, const char *tag, const char *fmt, ...);

extern uintptr_t game_read(uintptr_t handle, uintptr_t addr, void *out,
                          uint32_t len);
extern int game_write(int fd, const void *buf, size_t len, uintptr_t addr);
extern void runtime_journal_write(const char *stage, const char *reason,
                                  const char *payload);
extern int game_region_verify(uint32_t rva, uint32_t size, const char *sha);
extern void *trampoline_alloc(uintptr_t target);
extern void icache_flush(void *start, void *end);
extern int game_resolve(uintptr_t addr, uintptr_t *out);
extern int roster_entry_read(uintptr_t entry, uint32_t index, void *slot);
extern int entity_index_fetch(uintptr_t base, uintptr_t entity,
                              uintptr_t read_fn, int flags, void *out);
extern int evasion_key_epoch_query(const char *key, uint64_t *epoch);
extern uint64_t frame_epoch_now(void);
extern int visual_pipeline_ready(void);
extern void trophies_frame_callback(void);
extern int owner_phdr_cb(struct dl_phdr_info *info, size_t size, void *data);
extern int guarded_hook_install(uint32_t rva, uint32_t size, const char *sha,
                                uint32_t rva2, uint32_t original_word,
                                void *callback);
extern int hud_slot_validate(uint64_t *slot);
extern int hud_update_gate(uint64_t x);
extern int ui_node_create(uintptr_t node, uintptr_t parent);
extern int ui_node_adopt(uintptr_t donor, uintptr_t target);
extern int ui_node_detach(uintptr_t node, uintptr_t parent);
extern int ui_node_order(uintptr_t a, uintptr_t b);
extern int ui_node_reparent(uintptr_t a, uintptr_t b, uintptr_t c);
extern int json_field_int(const char *start, const char *end, const char *key,
                          int *out);
extern void json_prepare(void *buf, size_t len);
extern uint64_t xray_state_size(void);
extern int xray_state_init(void *state, uint64_t len, const void *rec);
extern int queue_plan_install(void *plan, uintptr_t addr, void *island,
                              void *callback, void *template, size_t tlen,
                              uint32_t original_word);
extern int queue_readback_check(void);
extern int dlsym_owner_check(void *fn);
extern void speed_hook_target(void);
extern void battle_text_chat_hook(void);
extern void ball_trajectory_hook_a(void);
extern void ball_trajectory_hook_b(void);
extern void teammate_arrow_hook(void);
extern void afk_visual_observer(void);
extern void trophies_owner_release(void *hud);
extern void trophies_hud_update_callback(void *hud);

extern uintptr_t g_game_base;
extern int32_t g_write_channel_fd;
extern uint64_t g_page_size;

extern char g_install_flags;
extern char g_trophies_native_fields;
extern char g_battle_text_chat_ready;
extern char g_ball_trajectory_ready;
extern char g_teammate_arrow_ready;
extern char g_ally_respawn_ready;
extern char g_speed_hook_installed;
extern char g_afk_observer_installed;
extern char g_xray_state_ready;
extern char g_restored_owner_registered;
extern void *g_register_restored_fn;
extern void *g_snapshot_keys_fn;
extern void *g_xray_state;
extern void *g_speed_trampoline;
extern void *g_afk_trampoline;
extern uint32_t g_afk_saved_word;
extern uint64_t g_tramp_target_table[TRAMPOLINE_SLOTS];
extern void *g_tramp_callbacks[TRAMPOLINE_SLOTS];
extern uint64_t g_tramp_records[TRAMPOLINE_SLOTS][7];
extern void *g_tramp_islands[TRAMPOLINE_SLOTS];
extern uint64_t g_hud_field_table[HUD_FIELD_SLOTS][10];
extern uint64_t g_hud_dedup_a[DEDUP_SLOTS];
extern uint64_t g_hud_dedup_b[DEDUP_SLOTS];
extern uint64_t g_hud_update_counter;
extern uint64_t g_hud_release_counter;
extern uint64_t g_hud_icon_counter;
extern uint32_t g_hud_journal_count;
extern uint32_t g_hud_owner_mask;
extern uint64_t g_hud_gate_counters[32];
extern uint64_t g_hud_gate_mask;
extern uint64_t g_trophies_roster_table[ROSTER_MAX][19];
extern uint64_t g_trophies_roster_count;
extern uint64_t g_trophies_roster_epoch;
extern uint64_t g_trophies_roster_ms;
extern uint64_t g_trophies_player_table[64][14];
extern uint32_t g_trophies_player_count;
extern uint64_t g_trophies_battle_epoch;
extern uint64_t g_trophies_capture_ms;
extern uint64_t g_trophies_legacy_complete;
extern uint64_t g_trophies_hud_owner_mask;
extern char g_trophies_hud_guards_done;
extern uint32_t g_trophies_hud_owner_state;
extern uint32_t g_trophies_hud_journal_count;
extern void *g_trophies_hud_owner_fn;
extern char g_leon_clone_latch;
extern char g_pin_anim_latch;
extern uint64_t g_hud_extra_latch;

extern pthread_mutex_t g_plusapi_mutex;
extern uint64_t g_plusapi_cache[0x60];
extern uint32_t g_plusapi_complete;
extern uint32_t g_plusapi_inflight;

static const uint64_t queue_guard_regions[][3] = {
    { 0xaa2830, 0x21c, 0 },
    { 0xfa9df0, 0xd4, 0 },
    { 0xfa9c24, 0xc, 0 },
    { 0xf9abec, 0x144, 0 },
    { 0xfa6ab4, 0xa0, 0 },
    { 0xcfeefc, 0xc, 0 },
};

static const char queue_guard_shas[6][65] = {
    "a292999d187dfc3c166d5ec83fceb7b0015e131b9c668312f6bbcbb27e8717d9",
    "a3ef960a37385ad90bc30ba317bb41b50bfc33ae04a9fb3458041fa5c1364d8b",
    "4d454fed2f9a182732bf8d0f650d46add5fe1931f842356001787be8c67d341f",
    "33dc3006e0b78ddc38bdcea7ffc76c82da74c9e9d54ec3bc05fc56dbf8a9879a",
    "1675ebe0ee43bbce74310153f6b89ca6eafd6b94e9867b74076ca101ccc81bc8",
    "6f33912a3f98b62d143fb59caef52681365a5b8834467f0c4abc5f4551249c3b",
};

static const uint32_t touch_guard_regions[][2] = {
    { 0xb3d64c, 0xf2c }, { 0xe7cd40, 0x9c }, { 0xecfb74, 0x118 },
    { 0xe7cddc, 0x48 }, { 0xe7979c, 0x9c }, { 0xe798b0, 0x20 },
    { 0xecf9f4, 0x60 }, { 0x8580d4, 0x7c0 }, { 0x858894, 0x7b8 },
    { 0x8741d8, 0x150 }, { 0xcf5044, 0x44 }, { 0x7050a4, 0xbc },
    { 0x705160, 0x7c }, { 0x7051dc, 0x70 },
};

static const char touch_guard_shas[14][65] = {
    "c6b6c1dcf3826b1ec58c25d2eccca25eed40cdacd658adf11fa2d5e4303bdd0d",
    "db1c4d4bcdd4178b8c864845ac63b1c7e7bc9bf3f2413ef56c292a99bdccdb73",
    "ea4c6b63a891e5697089f97b35e9a9393dacad09f0a7418e29d1d262a8e1625a",
    "757d89fc95443887343488c0d8f239595d21e089408aac54391c603750c91517",
    "a48b29bfe6642800e1874e63eae8f032f8c6ce9120c3426bcdad0cfa4971927b",
    "82be5b703b18215654561531e7a33d979743afc1b9a026472523cc167dd45801",
    "0407300dadcaaf0171830745ded639ef00e298f50019eb8ff4ed6a52c2818673",
    "3333ee1d5b1e8d5a20be1ff24f7c735e10048fb01796389954e0928ad1de1120",
    "1a411f9af22f7c7d59a9599a5dfcc60a44070b4b0121116d1f410d02749e26ec",
    "083d3765e11ad69526df0c046efdbf125848e295419eeffd36c9d07000ca252b",
    "e79643035de49ffa02823584702d294402aabd6b1d852c3fed52c40d15c114b7",
    "34a5074b72bb308ac25ec9ca60db279eb5472b00f936c54d506c99f27879ad7d",
    "3fec2737dd5569528dcbea561b5f857c8419aa34de052e3233869b1b32ee5f0b",
    "f9da06b9ce7403e941cdf05394a692d2bf59b300270f04e21f83e851110ea045",
};

static const uint32_t spectator_guard_regions[][2] = {
    { 0x80c1e0, 0x108 }, { 0xb28f2c, 0x1674 },
    { 0x671630, 0x1bc }, { 0x671860, 0x120 },
};

static const char spectator_guard_shas[4][65] = {
    "bb0388a1ee28b984b003b99cd571cab63afe3631f2138b78962fc1a06348b050",
    "2ada8e2e8d844459416abff76610f32f7f3a7fc233d7acd18690ecacfee44b18",
    "4863dfa470d3525e6e383d72f1e00b98bbd1eef1232423876fe226462486d682",
    "f9c0fbbfafdc423f98ffb766e2fff8d5ef3f9ad537a9084f20591b182eb0d1b6",
};

extern int social_name_metadata_install(void);
int trophies_hud_guards_install(void);
void trophies_capture_journal(uint32_t reason);

extern void *g_evasion_lib_handle;
extern void *g_trophies_frame_callback_slot;
extern uint32_t g_install_flags_hi;
extern char g_battle_text_chat_hooked;
extern char g_ball_trajectory_hook_a_flag;
extern char g_ball_trajectory_hook_b_flag;

extern int outline_guards_install(void);
extern int optional_gameplay_hook_guard_install(void);
extern void visual_install_1(void);
extern void visual_install_2(void);
extern void visual_install_3(void);
extern void visual_install_4(void);
extern void visual_install_5(void);
extern void visual_install_6(void);
extern void visual_install_7(void);
extern void visual_install_8(void);
extern void visual_install_9(void);
extern void visual_install_10(void);
extern void visual_install_11(void);
extern void visual_install_12(void);
extern void visual_install_13(void);
extern void queue_plan_callback(void);
extern void xray_stage_a(uintptr_t a, uint64_t b);
extern void xray_stage_b(uintptr_t a, uint64_t b);
extern void xray_stage_c(uintptr_t a, uint64_t b, uintptr_t c, uint64_t d);
extern void xray_stage_d(uintptr_t a, uint64_t b);
extern void guarded_hook_cb_a(void);
extern void guarded_hook_cb_b(void);

static int island_range_ok(uintptr_t island, uintptr_t target)
{
    if (((uint32_t)island | (uint32_t)target) & 3)
        return 0;
    if (island - target - 0x7fffffdull <= 0xfffffffff0000002ull)
        return 0;
    if (target - island - 0x7ffffedull <= 0xfffffffff0000002ull)
        return 0;
    return 1;
}

static uint32_t branch_word(uintptr_t from, uintptr_t to, uint32_t kind)
{
    uint32_t delta = (uint32_t)(to - from);
    uint32_t q = (int32_t)delta < 0 ? delta + 3 : delta;
    return kind | ((q >> 2) & 0x3ffffff);
}

static int island_install(uintptr_t target, void *callback, int bl_hook,
                          uint32_t *saved_out, uint64_t record[4])
{
    void *island = trampoline_alloc(target);
    if (island == NULL)
        return 0;
    if (game_read(0, target, saved_out, 4) == 0)
        return 0;
    if (!island_range_ok((uintptr_t)island, target))
        return 0;

    uint32_t b_resume = branch_word((uintptr_t)island + 4, target + 4,
                                    0x14000000u);
    uint32_t hook_word = branch_word(target,
                                     (uintptr_t)island + 0x10,
                                     bl_hook ? 0x94000000u : 0x14000000u);

    record[0] = ((uint64_t)b_resume << 32) | (uint64_t)*saved_out;
    record[1] = hook_word;
    record[2] = 0xd61f020058000050ull;
    record[3] = (uint64_t)(uintptr_t)callback;

    uint64_t *island64 = (uint64_t *)(uintptr_t)island;
    island64[0] = record[0];
    island64[2] = record[2];
    island64[3] = record[3];
    icache_flush(island, (char *)island + 0x20);
    if (mprotect(island, g_page_size, PROT_READ | PROT_EXEC) != 0)
        return 0;
    return 1;
}

static int hook_word_publish(uintptr_t target, uint32_t hook_word,
                             uint32_t saved)
{
    if (game_write(g_write_channel_fd, &hook_word, 4, target) != 4)
        goto restore;
    icache_flush((void *)target, (void *)(target + 4));
    return 1;

restore:
    if (game_write(g_write_channel_fd, &saved, 4, target) != 4)
        abort();
    icache_flush((void *)target, (void *)(target + 4));
    uint32_t check = 0;
    if (game_read(0, target, &check, 4) == 0 || check != saved)
        abort();
    return 0;
}

static int hook_word_verify(uintptr_t target, uint32_t hook_word,
                            uint32_t saved)
{
    uint32_t check = 0;
    if (game_read(0, target, &check, 4) != 0 && check == hook_word)
        return 1;
    if (game_write(g_write_channel_fd, &saved, 4, target) != 4)
        abort();
    icache_flush((void *)target, (void *)(target + 4));
    if (game_read(0, target, &check, 4) == 0 || check != saved)
        abort();
    return 0;
}

static int spectator_islands_install(void)
{
    static const uint32_t targets[TRAMPOLINE_SLOTS] = {
        0x11be3f0, 0x11be3f8, 0x11be400, 0x11ad210,
    };
    for (int i = 0; i < TRAMPOLINE_SLOTS; i++) {
        g_tramp_target_table[i] = g_game_base + targets[i];
        uint32_t saved = 0;
        if (!island_install(g_tramp_target_table[i], g_tramp_callbacks[i],
                            i >= 2, &saved, g_tramp_records[i]))
            return 0;
        g_tramp_records[i][4] = saved;
        if (!hook_word_publish(g_tramp_target_table[i],
                               (uint32_t)g_tramp_records[i][1], saved))
            return 0;
    }
    for (int i = 0; i < TRAMPOLINE_SLOTS; i++) {
        if (!hook_word_verify(g_tramp_target_table[i],
                              (uint32_t)g_tramp_records[i][1],
                              (uint32_t)g_tramp_records[i][4]))
            return 0;
    }
    return 1;
}

void trophies_install(int stage)
{
    (void)stage;

    if (visual_pipeline_ready() == 0)
        goto hero_refused;

    g_trophies_frame_callback_slot = (void *)(uintptr_t)trophies_frame_callback;

    for (int i = 0; i < 5; i++) {
        if (game_region_verify((uint32_t)queue_guard_regions[i][0],
                               (uint32_t)queue_guard_regions[i][1],
                               queue_guard_shas[i]) == 0)
            goto hero_refused;
    }
    if (game_region_verify((uint32_t)queue_guard_regions[5][0],
                           (uint32_t)queue_guard_regions[5][1],
                           queue_guard_shas[5]) != 0) {
        g_trophies_native_fields = 1;
    } else {
        g_trophies_native_fields = 0;
        runtime_journal_write("restored_optional_refused",
                              "trophies_native_fields", NULL);
    }

    for (int i = 0; i < 10; i++) {
        if (game_region_verify((uint32_t)touch_guard_regions[i][0],
                               (uint32_t)touch_guard_regions[i][1],
                               touch_guard_shas[i]) == 0)
            goto hero_refused;
    }
    g_install_flags_hi =
        (game_region_verify(0xcf5044, 0x44,
            "e79643035de49ffa02823584702d294402aabd6b1d852c3fed52c40d15c114b7") != 0);
    if (g_install_flags_hi != 0) {
        for (int i = 11; i < 14; i++) {
            if (game_region_verify((uint32_t)touch_guard_regions[i][0],
                                   (uint32_t)touch_guard_regions[i][1],
                                   touch_guard_shas[i]) == 0)
                goto touch_refused;
        }
    }

    for (int i = 0; i < 4; i++) {
        if (game_region_verify((uint32_t)spectator_guard_regions[i][0],
                               (uint32_t)spectator_guard_regions[i][1],
                               spectator_guard_shas[i]) == 0)
            goto spectator_refused;
    }
    if (!spectator_islands_install())
        goto spectator_refused;

    if (game_region_verify(0xf39550, 0x58,
            "e4f08a4e7ec1aa3cc89435521ce00675a294f6452b74792f908dc3c4b9bbbf84") != 0 &&
        game_region_verify(0xf3a6e0, 0xc4,
            "dc94a3d1df8b4d4eca9d98c6c378ffdff9966a3e336ab164f24d9371a901781a") != 0) {
        if (game_region_verify(0x8d8ab8, 0x1ec,
                "ca2e9333fa1f07663da460b1202fe569b68c6c45e5d9b1733d9b87bad2f861f8") != 0 &&
            game_region_verify(0x8d9180, 0x90,
                "ad4bad5ae27aa2211471eabdef3bc6d939bffd321b22e29129edda7c1c31fd84") != 0) {
            g_battle_text_chat_ready =
                (char)game_region_verify(0x8d95a8, 0xc,
                    "2e5a53d684c71177c39ca818dac4f2733319116c45b1f38331d18caafdd13050");
        } else {
            g_battle_text_chat_ready = 0;
        }

        uintptr_t target = g_game_base + 0xf39550;
        uint64_t rec[4];
        uint32_t saved = 0;
        if (island_install(target, battle_text_chat_hook, 0, &saved, rec) &&
            saved == 0xa9be7bfd &&
            hook_word_publish(target, (uint32_t)rec[1], saved) &&
            hook_word_verify(target, (uint32_t)rec[1], saved))
            g_battle_text_chat_hooked = 1;
        else
            runtime_journal_write("script_port_unavailable",
                                  "battle_text_chat", NULL);
    } else {
        runtime_journal_write("script_port_unavailable", "battle_text_chat",
                              NULL);
    }

    if (game_region_verify(0xb444a0, 0x2fac,
            "bd0ebb55201a288b9fb12128fdc0b4747ade4ca5c158ab666d00ecf5551f6f25") != 0 &&
        game_region_verify(0xb4071c, 0x3b6c,
            "dd11a7757ec957fc308cb010fd27ddfc85779f70a71dae6a64864c64b5199efe") != 0 &&
        game_region_verify(0xd4c490, 0xd24,
            "9cb53aaae7005672d808c034dfc90dedf38a4d61a84da198870788098de224ad") != 0) {
        uintptr_t target_a = g_game_base + 0xb444a0;
        uintptr_t target_b = g_game_base + 0xb4071c;
        uint64_t rec_a[4], rec_b[4];
        uint32_t saved_a = 0, saved_b = 0;
        if (island_install(target_a, ball_trajectory_hook_a, 0, &saved_a,
                           rec_a) &&
            saved_a == 0x6db63bef &&
            hook_word_publish(target_a, (uint32_t)rec_a[1], saved_a) &&
            island_install(target_b, ball_trajectory_hook_b, 0, &saved_b,
                           rec_b) &&
            saved_b == 0x6db63bef &&
            hook_word_publish(target_b, (uint32_t)rec_b[1], saved_b) &&
            hook_word_verify(target_a, (uint32_t)rec_a[1], saved_a) &&
            hook_word_verify(target_b, (uint32_t)rec_b[1], saved_b)) {
            g_ball_trajectory_ready = 1;
            if (social_name_metadata_install() == 0)
                goto social_refused;
        } else {
            runtime_journal_write("script_port_unavailable",
                                  "extended_ball_trajectory", NULL);
        }
    } else {
        runtime_journal_write("script_port_unavailable",
                              "extended_ball_trajectory", NULL);
    }

    if (game_region_verify(0x86bdfc, 0x174c,
            "e5b9679a102c6c0c387d4429aeea2ce5be6401bc95de89aeb40a9947aba62917") != 0 &&
        game_region_verify(0xd49da8, 0x3c,
            "cd09749187c98f768df22dd7ad5ca56d51a007c8b9a7ed5580a95dd0d23be2ed") != 0 &&
        game_region_verify(0xfd46fc, 0xa8,
            "272deb9d7c88eea890a37806decb7cf85d7495cf196322576c01082d7c07cf5d") != 0) {
        uintptr_t target = g_game_base + 0x86bdfc;
        uint64_t rec[4];
        uint32_t saved = 0;
        if (island_install(target, teammate_arrow_hook, 0, &saved, rec) &&
            saved == 0xd10343ff &&
            hook_word_publish(target, (uint32_t)rec[1], saved) &&
            hook_word_verify(target, (uint32_t)rec[1], saved))
            g_teammate_arrow_ready = 1;
        else
            runtime_journal_write("script_port_unavailable",
                                  "native_teammate_arrow", NULL);
    } else {
        runtime_journal_write("script_port_unavailable",
                              "native_teammate_arrow", NULL);
    }

    if (game_region_verify(0xfd4c54, 0x94,
            "80062db5e5955bd4a396675ce1a90487546030590c7ed366391b87c8fff2dd94") != 0 &&
        game_region_verify(0xf86d6c, 0x40,
            "4063cbdeb9d71a6dab1568d64fd767c7ee3ae7a0f4e9a7178eb7cac1908ed42e") != 0 &&
        game_region_verify(0xfd3cbc, 0x14,
            "6dbc0bc720cd22e1374bcef7325a2163b44671e30d064e2aebc85e1bc0e09bf2") != 0) {
        g_ally_respawn_ready =
            (char)game_region_verify(0xfd46fc, 0xa8,
                "272deb9d7c88eea890a37806decb7cf85d7495cf196322576c01082d7c07cf5d");
        if (g_ally_respawn_ready == 0)
            runtime_journal_write("script_port_unavailable", "ally_respawn",
                                  NULL);
    } else {
        g_ally_respawn_ready = 0;
        runtime_journal_write("script_port_unavailable", "ally_respawn",
                              NULL);
    }

    if (game_region_verify(0xe7af70, 0x8d4,
            "4d6f74916dc6929e7ba905dece78bde36b56eefc5c7651ba17aa53b8f32487ea") != 0 &&
        game_region_verify(0xe7bd74, 0x20c,
            "bf8fe1ca284cb69aa0e45baa8db2df7fdd7be2c1a1b522d7367f08fcacc7b500") != 0 &&
        game_region_verify(0xb35608, 0x1bf4,
            "b98e10a40d5f978e31b292d3962e6a64374b7c9fab60c4cb622a2756f8db73ab") != 0 &&
        game_region_verify(0xb533fc, 0xc,
            "585c75735a026b32b00785df02d8889b85e3925d335841a19644e3e5e557b565") != 0) {
        uintptr_t target = g_game_base + 0xf86758;
        g_speed_trampoline = trampoline_alloc(target);
        uint32_t saved = 0;
        uint32_t expect_bl = 0x94000000u |
            ((((g_game_base + 0xe7af70 - target) + 3) & ~3u) >> 2 & 0x3ffffff);
        if (g_speed_trampoline != NULL &&
            game_read(0, target, &saved, 4) != 0 &&
            saved == expect_bl) {
            uint64_t *island64 = (uint64_t *)g_speed_trampoline;
            island64[0] = 0xd61f020058000050ull;
            island64[1] = (uint64_t)(uintptr_t)speed_hook_target;
            icache_flush(g_speed_trampoline,
                         (char *)g_speed_trampoline + 0x10);
            uint32_t bl = branch_word(target,
                                      (uintptr_t)g_speed_trampoline + 0x10,
                                      0x94000000u);
            if (mprotect(g_speed_trampoline, g_page_size,
                         PROT_READ | PROT_EXEC) == 0 &&
                hook_word_publish(target, bl, saved) &&
                hook_word_verify(target, bl, saved))
                g_speed_hook_installed = 1;
            else
                runtime_journal_write("restored_optional_refused",
                                      "speed_code_guards", NULL);
        } else {
            runtime_journal_write("restored_optional_refused",
                                  "speed_code_guards", NULL);
        }
    } else {
        runtime_journal_write("restored_optional_refused",
                              "speed_code_guards", NULL);
    }

    if (game_region_verify(0x863fbc, 0x75c,
            "b3894c39e8de7b2bfa31e0c0e6dc312a3fda0d1d2f5e02950cc7c99f73a70cbb") != 0 &&
        game_region_verify(0xd49da8, 0x3c,
            "cd09749187c98f768df22dd7ad5ca56d51a007c8b9a7ed5580a95dd0d23be2ed") != 0 &&
        game_region_verify(0xfd3cbc, 0x14,
            "6dbc0bc720cd22e1374bcef7325a2163b44671e30d064e2aebc85e1bc0e09bf2") != 0) {
        uintptr_t target = g_game_base + 0x8640fc;
        g_afk_trampoline = trampoline_alloc(target);
        uint32_t saved = 0;
        if (g_afk_trampoline != NULL &&
            game_read(0, target, &saved, 4) != 0 && saved == 0x360013e8) {
            uint8_t template[0x158];
            memcpy(template, (const void *)(uintptr_t)0x1cb6b0, 0x158);
            uint32_t b_entry = branch_word(target,
                                           (uintptr_t)g_afk_trampoline,
                                           0x14000000u);
            uint32_t b_back = branch_word(
                (uintptr_t)g_afk_trampoline + 0x144,
                target + 0x144 - 0x144 + 0, 0x14000000u);
            uint32_t b_cb = branch_word(
                (uintptr_t)g_afk_trampoline + 0x148,
                g_game_base + 0x86422c, 0x14000000u);
            *(uint32_t *)(void *)(template + 0x148) = b_back;
            *(uint32_t *)(void *)(template + 0x14c) = b_cb;
            *(uint32_t *)(void *)(template + 0x144) = 0x36000048;
            *(uint64_t *)(void *)(template + 0x150) =
                (uint64_t)(uintptr_t)afk_visual_observer;
            memcpy(g_afk_trampoline, template, 0x158);
            icache_flush(g_afk_trampoline,
                         (char *)g_afk_trampoline + 0x158);
            if (mprotect(g_afk_trampoline, g_page_size,
                         PROT_READ | PROT_EXEC) == 0 &&
                hook_word_publish(target, b_entry, saved) &&
                hook_word_verify(target, b_entry, saved))
                g_afk_observer_installed = 1;
        }
        if (g_afk_observer_installed == 0)
            runtime_journal_write("restored_refused",
                                  "afk_visual_code_guards", NULL);
    } else {
        runtime_journal_write("restored_refused", "afk_visual_code_guards",
                              NULL);
    }

    if (outline_guards_install() == 0)
        runtime_journal_write("restored_refused", "outline_code_guards",
                              NULL);

    if (trophies_hud_guards_install() == 0)
        runtime_journal_write("restored_optional_refused",
                              "trophies_hud_native_guards", NULL);

    if (optional_gameplay_hook_guard_install() == 0)
        runtime_journal_write("script_port_refused",
                              "optional_gameplay_hook_guard", NULL);

    visual_install_1();
    visual_install_2();
    visual_install_3();
    visual_install_4();
    visual_install_5();
    visual_install_6();
    visual_install_7();
    visual_install_8();
    visual_install_9();
    visual_install_10();
    visual_install_11();
    guarded_hook_install(0x821428, 0x2f0,
        "978a122086fb2bc3e68ea887cde5158da0284963edb2483111865403cc52ff8e",
        0x821428, 0xf9431408, guarded_hook_cb_a);
    visual_install_12();
    visual_install_13();
    guarded_hook_install(0x85d6d8, 0x3d8,
        "be9fca6debb3dcce563144f32c278edbf12eb2587bc56f20432e8b74dbfeced7",
        0x85d6d8, 0x6dbc23e9, guarded_hook_cb_b);

    g_register_restored_fn = dlsym(g_evasion_lib_handle,
                                   "nexus_evasion_register_restored_v1");
    g_snapshot_keys_fn = dlsym(g_evasion_lib_handle,
                               "nexus_evasion_snapshot_keys_v1");
    if (dlsym_owner_check(g_register_restored_fn) == 0 ||
        dlsym_owner_check(g_snapshot_keys_fn) == 0) {
        g_register_restored_fn = NULL;
        return;
    }

    {
        uint64_t size = xray_state_size();
        if (size - 0x10001 > 0xffffffffffff0000ULL)
            goto queue_failed;
        g_xray_state = mmap(NULL, size, 3, 0x22, -1, 0);
        if (g_xray_state == MAP_FAILED) {
            g_xray_state = NULL;
            goto queue_failed;
        }
        uint64_t rec[10];
        rec[0] = g_game_base;
        rec[1] = 0;
        rec[2] = (uint64_t)(uintptr_t)game_read;
        rec[3] = (uint64_t)(uintptr_t)queue_plan_callback;
        rec[4] = ROD_STATUS_HDR;
        rec[5] = (uint64_t)(uintptr_t)xray_stage_a;
        rec[6] = (uint64_t)(uintptr_t)xray_stage_b;
        rec[7] = (uint64_t)(uintptr_t)xray_stage_c;
        rec[8] = (uint64_t)(uintptr_t)xray_stage_d;
        rec[9] = 0;
        if (xray_state_init(g_xray_state, size, rec) == 0) {
            runtime_journal_write("restored_refused", "xray_guarded_binding",
                                  NULL);
            goto queue_failed;
        }

        uintptr_t qtarget = g_game_base + 0xaa2830;
        void *qisland = trampoline_alloc(qtarget);
        uint32_t qsaved = 0;
        if (qisland == NULL ||
            game_read(0, qtarget, &qsaved, 4) == 0 ||
            queue_plan_install((void *)(uintptr_t)0x228908, qtarget, qisland,
                               (void *)(uintptr_t)queue_plan_callback,
                               (void *)(uintptr_t)0x1cb6b0, 0x158,
                               qsaved) == 0) {
            runtime_journal_write("restored_refused", "queue_plan", NULL);
            goto queue_failed;
        }
        memcpy(qisland, (const void *)(uintptr_t)0x228928, 0x208);
        icache_flush(qisland, (char *)qisland + 0x208);
        if (mprotect(qisland, g_page_size, PROT_READ | PROT_EXEC) != 0) {
            runtime_journal_write("restored_refused", "queue_seal", NULL);
            goto queue_failed;
        }
        if (game_write(g_write_channel_fd,
                       (void *)(uintptr_t)0x228924, 4, qtarget) != 4) {
            runtime_journal_write("restored_refused", "queue_publication",
                                  NULL);
            goto queue_restore;
        }
        icache_flush((void *)qtarget, (void *)(qtarget + 4));
        if (queue_readback_check() == 0) {
            runtime_journal_write("restored_refused", "queue_readback",
                                  NULL);
            goto queue_restore;
        }
        g_xray_state_ready = 1;

        uint64_t owner_rec[0x228 / 8];
        memcpy(owner_rec, (const void *)(uintptr_t)0x228908, 0x228);
        if (((int (*)(const void *))g_register_restored_fn)(owner_rec) != 1) {
            runtime_journal_write("restored_refused",
                                  "queue_owner_registration", NULL);
            goto queue_restore;
        }
        runtime_journal_write("restored_ready",
                              "xray_existing_input_and_visual_bound",
                              ",\"queue_observers\":1,\"new_inputs\":0");
        return;
    }

queue_restore:
    {
        uintptr_t qtarget = g_game_base + 0xaa2830;
        uint32_t qsaved = 0;
        if (game_read(0, qtarget, &qsaved, 4) != 0) {
            if (game_write(g_write_channel_fd, &qsaved, 4, qtarget) != 4) {
                runtime_journal_write("fatal", "queue_restore_failed", NULL);
                abort();
            }
            icache_flush((void *)qtarget, (void *)(qtarget + 4));
            uint32_t qcheck = 0;
            if (game_read(0, qtarget, &qcheck, 4) == 0 || qcheck != qsaved) {
                runtime_journal_write("fatal", "queue_restore_unverified",
                                      NULL);
                abort();
            }
        }
    }
queue_failed:
    if (g_register_restored_fn != NULL)
        ((void (*)(void *))g_register_restored_fn)(NULL);
    return;

touch_refused:
    runtime_journal_write("restored_refused", "touch_code_guards", NULL);
    return;
spectator_refused:
    runtime_journal_write("restored_refused", "spectator_code_guards", NULL);
    return;
social_refused:
    runtime_journal_write("script_port_unavailable", "social_name_metadata",
                          NULL);
    return;
hero_refused:
    runtime_journal_write("restored_refused", "hero_code_guards", NULL);
}

extern int32_t (*g_keys_snapshot_fn)(const char *, void *, int, void *,
                                     int *);
extern int32_t g_main_thread_id;
extern uint64_t g_frame_epoch2;
extern uint64_t g_frame_clock;

extern const uint32_t g_body_guard_regions[GUARD_COUNT][2];
extern const char *g_body_guard_shas[GUARD_COUNT];

typedef struct {
    uint32_t *count;
    uintptr_t *found;
} owner_discover_ctx;

extern int game_fn_string_format(void *out, const char *fmt, ...);
extern int game_fn_string_release(void *str);
extern void *game_fn_style_lookup(const char *file, const char *name, int a);
extern void *game_fn_find_node(void *parent, const char *name);
extern void game_fn_add_child(void *parent, void *child, int a);
extern void game_fn_release_node(void *node);
extern void game_fn_release_text(void *node);
extern void game_fn_set_position(float x, float y, void *node);
extern void game_fn_set_color(uint32_t color, void *node);
extern void game_fn_node_set_text(void *node, void *text);
extern int game_fn_node_set_text_verified(void *node, void *str);
extern int game_region_verify2(uint32_t rva, uint32_t size, const char *sha);

typedef struct {
    uint32_t stamp;
    uint32_t zero;
    int32_t x;
    int32_t y;
    int32_t z;
    int32_t team;
    uint32_t speed;
    uint32_t radius;
} entity_index_t;

void trophies_battle_tick(void *ctx)
{
    int saved_errno = errno;
    uint64_t *frame = ctx;

    g_trophies_battle_epoch = 0;
    uint64_t epoch = frame_epoch_now();

    if (ctx == NULL || g_trophies_native_fields == 0 ||
        frame[9] == 0 || frame[2] == 0 || frame[1] == 0 || epoch == 0) {
        g_trophies_roster_epoch = 0;
        g_trophies_player_count = 0;
        g_trophies_roster_count = 0;
        memset((void *)g_trophies_roster_table, 0,
               sizeof g_trophies_roster_table);
        errno = saved_errno;
        return;
    }

    uint64_t snap_epoch = 0;
    int64_t snap_flags = -1;
    char *key = "trophiesAboveHead";
    entity_index_t own_idx;
    if (g_xray_state_ready != 1 || g_snapshot_keys_fn == NULL ||
        ((int32_t (*)(const char *, void *, int, void *, int *))
             g_snapshot_keys_fn)(key, &snap_epoch, 1, &snap_flags,
                                 (int *)&own_idx) != 1 ||
        snap_epoch == 0 || snap_flags != 0 ||
        ((int32_t *)ctx)[0x1a] != 2 ||
        *(uint32_t *)((char *)ctx + 0x44) != 1 ||
        frame[4] != epoch || *(uint64_t *)(frame[3] + 6) != g_frame_epoch2) {
        g_trophies_roster_epoch = 0;
        g_trophies_player_count = 0;
        g_trophies_roster_count = 0;
        memset((void *)g_trophies_roster_table, 0,
               sizeof g_trophies_roster_table);
        errno = saved_errno;
        return;
    }

    uintptr_t battle = frame[2];
    g_trophies_player_table[0][8] = frame[8];
    g_trophies_player_table[0][7] = frame[7];
    g_trophies_player_table[0][2] = frame[5];
    g_trophies_player_table[0][5] = frame[6];
    g_trophies_roster_epoch = snap_epoch;
    g_trophies_battle_epoch = battle;
    g_trophies_player_table[0][0] = epoch;
    g_trophies_player_table[0][1] = frame[3];
    memset((void *)g_trophies_roster_table, 0,
           sizeof g_trophies_roster_table);
    g_trophies_capture_ms = 0;
    g_trophies_legacy_complete = 0;

    uintptr_t roster_arr = 0;
    uint32_t roster_count = 0;
    uint32_t own_roster_index = 0;
    uint32_t reason = 1;

    if (game_resolve(*(uintptr_t *)(battle + 0x38), &roster_arr) == 0)
        goto journal;
    uint32_t count = 0;
    if (game_read(0, *(uintptr_t *)(battle + 0x38) + 0xc, &count, 4) == 0 ||
        (int32_t)count < 1 || (int32_t)count > 0x40)
        goto journal;
    if (game_read(0, *(uintptr_t *)(battle + 0x38) + 0xe0, &own_roster_index,
                  4) == 0 || (int32_t)own_roster_index < 0 ||
        (int32_t)count <= (int32_t)own_roster_index)
        goto journal;

    roster_count = count;
    for (uint32_t i = 0; i < roster_count; i++) {
        uint64_t entry = 0;
        if (game_read(0, (uintptr_t)i * 8 + roster_arr, &entry, 8) == 0 ||
            entry + 0x2000 < 0x12000 || (entry & 7) != 0)
            continue;
        roster_entry_read(entry, i, &g_trophies_roster_table[i]);
    }

    uintptr_t name_arr = 0, name_lst = 0;
    if (game_resolve(*(uintptr_t *)(battle + 0x38) + 0x28, &name_arr) == 0 ||
        game_resolve(name_arr, &name_lst) == 0)
        goto journal2;
    uint32_t name_count = 0;
    if (game_read(0, name_arr + 0xc, &name_count, 4) == 0 ||
        (int32_t)name_count < 1 || (int32_t)name_count > 0x200)
        goto journal2;
    uint64_t names[0x200];
    if (game_read(0, name_lst, names,
                  (size_t)name_count * 8) == 0)
        goto journal2;

    entity_index_t idx;
    if (entity_index_fetch(g_game_base, frame[8], (uintptr_t)game_read, 0,
                           &idx) == 0 ||
        idx.team == 0)
        goto journal3;

    if (idx.stamp == own_idx.stamp &&
        *(uint64_t *)((char *)ctx + 0x2c) == (uint64_t)own_roster_index &&
        g_trophies_roster_table[own_roster_index][0] != 0 &&
        g_trophies_roster_table[own_roster_index][1] != 0) {
        uint64_t colors = g_trophies_roster_table[own_roster_index][1];
        uint32_t color_a = (uint32_t)(colors >> 32);
        if (color_a != (uint32_t)idx.team) {
            uint32_t i = 0;
            while (i + 1 != color_a &&
                   g_trophies_roster_table[own_roster_index][2 + i / 2] !=
                       (uint64_t)idx.team)
                i++;
        }

        if ((int32_t)name_count >= 1) {
            uint32_t own_hits = 0;
            for (uint32_t i = 0; i < name_count; i++) {
                if (names[i] == frame[8]) {
                    own_hits++;
                } else {
                    entity_index_t other;
                    if (game_resolve(names[i], (uintptr_t *)&other) != 0 &&
                        (*(uintptr_t *)&other == g_game_base + 0x1220040 ||
                         *(uintptr_t *)&other == g_game_base + 0x12200a0) &&
                        entity_index_fetch(g_game_base, names[i],
                                           (uintptr_t)game_read, 0,
                                           &other) != 0 &&
                        other.team != 0) {
                        uint32_t slot = other.stamp;
                        if (slot < name_count && slot != own_roster_index &&
                            g_trophies_roster_table[slot][0] != 0 &&
                            g_trophies_roster_table[slot][1] != 0) {
                            uint64_t mask = 1ull << (slot & 0x3f);
                            if ((mask & g_trophies_player_count) == 0) {
                                uint32_t pc = (uint32_t)g_trophies_player_count;
                                if (pc < 0x20) {
                                    g_trophies_player_count = pc + 1;
                                    g_trophies_player_table[pc + 1][8] =
                                        other.stamp;
                                    g_trophies_player_table[pc + 1][7] =
                                        other.radius;
                                    g_trophies_player_table[pc + 1][6] =
                                        other.speed;
                                    g_trophies_player_table[pc + 1][5] =
                                        other.team;
                                    g_trophies_player_table[pc + 1][4] =
                                        other.z;
                                    g_trophies_player_table[pc + 1][3] =
                                        other.y;
                                    g_trophies_player_table[pc + 1][2] =
                                        other.x;
                                    g_trophies_player_table[pc + 1][1] =
                                        names[i];
                                    g_trophies_player_table[pc + 1][0] = 0;
                                }
                            }
                        }
                    }
                }
            }

            if (own_hits == 1 && frame_epoch_now() == epoch) {
                uint64_t stable_epoch = 0;
                if (evasion_key_epoch_query("trophiesAboveHead",
                                            &stable_epoch) != 0 &&
                    stable_epoch == snap_epoch &&
                    game_resolve(*(uintptr_t *)(battle + 0x38),
                                 &roster_arr) != 0 &&
                    roster_arr != 0) {
                    uint32_t recount = 0;
                    if (game_read(0, *(uintptr_t *)(battle + 0x38) + 0xc,
                                  &recount, 4) != 0 &&
                        recount == count &&
                        game_resolve(*(uintptr_t *)(battle + 0x38) + 0x28,
                                     &name_arr) != 0 &&
                        name_arr != 0 &&
                        game_resolve(name_arr, &name_lst) != 0 &&
                        name_lst != 0 &&
                        game_read(0, name_arr + 0xc, &recount, 4) != 0 &&
                        recount == name_count) {
                        g_trophies_player_table[0][9] = name_lst;
                        g_trophies_player_table[0][10] = name_arr;
                        reason = 0;
                        g_trophies_battle_epoch = 1;
                        g_trophies_roster_count = recount;
                    } else {
                        reason = 4;
                    }
                } else {
                    reason = 4;
                }
            } else {
                reason = 4;
            }
        }
    } else {
        reason = 3;
    }
    goto journal;

journal3:
    reason = 3;
    goto journal;
journal2:
    reason = 2;
    goto journal;
journal:
    trophies_capture_journal(reason);
    errno = saved_errno;
}

void trophies_capture_journal(uint32_t reason)
{
    struct timespec ts;
    uint64_t now = 0;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0)
        now = (uint64_t)ts.tv_sec * 1000 + (uint64_t)ts.tv_nsec / 1000000;
    if (g_trophies_capture_ms - 1 < now && now - g_trophies_capture_ms < 2000)
        return;

    uint64_t known = 0;
    uint64_t roster = g_trophies_roster_count;
    if (roster > 1) {
        for (uint32_t i = 0; i + 1 < roster; i++)
            known += g_trophies_roster_table[i][0] +
                     g_trophies_roster_table[i][1];
    } else if (roster == 1) {
        known = g_trophies_roster_table[0][0];
    }

    g_trophies_capture_ms = now;
    char buf[0xdc];
    snprintf(buf, sizeof buf,
             ",\"capture_reason\":%u,\"roster\":%u,\"known\":%u,"
             "\"actors\":%u,\"epoch\":%llu,\"legacy_complete\":%u",
             reason, (unsigned)roster, (unsigned)known,
             (unsigned)g_trophies_player_count,
             (unsigned long long)g_trophies_roster_epoch,
             (unsigned)g_trophies_legacy_complete);
    runtime_journal_write("trophies_native", "v69_battle_intro_total", buf);
}

int trophies_hud_guards_install(void)
{
    if (visual_pipeline_ready() == 0 || g_trophies_hud_guards_done != 0)
        return (g_trophies_hud_owner_state == 3);

    g_trophies_hud_guards_done = 1;

    for (uint32_t i = 0; i < GUARD_COUNT; i++) {
        if (game_region_verify(g_body_guard_regions[i][0],
                               g_body_guard_regions[i][1],
                               g_body_guard_shas[i]) == 0) {
            if (g_body_guard_regions[i][0] == 0x5921a0) {
                if (g_trophies_hud_owner_fn == NULL) {
                    uintptr_t found = 0;
                    uint32_t count = 0;
                    owner_discover_ctx ctx = { &count, &found };
                    dl_iterate_phdr(owner_phdr_cb, &ctx);
                    if (count != 1 || found == 0)
                        goto guard_failed;
                    g_trophies_hud_owner_fn = (void *)found;
                }
                if (((int (*)(uintptr_t))g_trophies_hud_owner_fn)(
                        g_game_base) == 1)
                    continue;
            }
guard_failed:
            if (g_trophies_hud_journal_count < 0x20) {
                char detail[0x50];
                snprintf(detail, sizeof detail, ",\"detail\":%u",
                         g_body_guard_regions[i][0]);
                runtime_journal_write("trophies_hud",
                                      "native_body_guard_failed", detail);
                g_trophies_hud_journal_count++;
            }
            return 0;
        }
    }

    static const struct { uint32_t rva; uintptr_t expect; } vtable_guards[] = {
        { 0x11be3f0, 0x814068 }, { 0x11be3f8, 0x814130 },
        { 0x11be400, 0x8142e8 }, { 0x11ad300, 0x5d6594 },
        { 0x11ad210, 0x5d48e8 }, { 0x11abae0, 0x58d1c4 },
    };
    for (size_t i = 0; i < sizeof vtable_guards / sizeof *vtable_guards;
         i++) {
        uintptr_t fn = 0;
        if (game_read(0, g_game_base + vtable_guards[i].rva, &fn, 8) == 0 ||
            fn != g_game_base + vtable_guards[i].expect) {
            if (g_trophies_hud_journal_count < 0x20) {
                char detail[0x50];
                snprintf(detail, sizeof detail, ",\"detail\":%u",
                         vtable_guards[i].rva);
                runtime_journal_write("trophies_hud",
                                      "native_vtable_guard_failed", detail);
                g_trophies_hud_journal_count++;
            }
            return 0;
        }
    }

    int a = guarded_hook_install(0x8142e8, 0x478,
        "9f53b0f37ccda2f0d7da4b6308e0b31c97f0a48697e3f84893b37938e8ae7495",
        0x8142e8, 0xa9bd7bfd, trophies_hud_update_callback);
    int b = guarded_hook_install(0x814068, 0xc0,
        "aadd8dd7c5744aeaac7c9f0ddefd7fe8fba04b4d2aa8f755d1579959554c906d",
        0x814068, 0xa9be7bfd, trophies_hud_update_callback);
    g_trophies_hud_owner_state = (uint32_t)((a != 0) | ((b != 0) << 1));
    if (g_trophies_hud_journal_count < 0x20) {
        char detail[0x50];
        snprintf(detail, sizeof detail, ",\"detail\":%u",
                 g_trophies_hud_owner_state);
        runtime_journal_write("trophies_hud", "native_owner_hooks", detail);
        g_trophies_hud_journal_count++;
    }
    return (g_trophies_hud_owner_state == 3);
}

void trophies_owner_release(void *hud)
{
    int saved_errno = errno;
    uint64_t hud_id = *(uint64_t *)hud;

    if (hud != NULL && hud_update_gate(hud_id) != 0 &&
        g_trophies_hud_owner_state != 0) {
        for (int i = 0; i < HUD_FIELD_SLOTS; i++) {
            uint64_t *slot = g_hud_field_table[i];
            if (slot[0] == hud_id && hud_id != 0) {
                if (hud_slot_validate(slot) == 0) {
                    if (g_trophies_hud_journal_count < 0x20) {
                        char detail[0x50];
                        snprintf(detail, sizeof detail, ",\"detail\":%u", 0);
                        runtime_journal_write("trophies_hud",
                                              "release_ownership_refused",
                                              detail);
                        g_trophies_hud_journal_count++;
                    }
                } else {
                    uintptr_t text_a = slot[3];
                    uintptr_t text_b = slot[4];
                    uint64_t ref = 1;
                    int32_t id = 0;
                    if (game_read(0, text_a + 0x38, &ref, 8) == 0 ||
                        ref != 0 ||
                        game_read(0, text_a + 0x40, &id, 4) == 0 || id != -1)
                        game_fn_release_node((void *)text_a);
                    ref = 1;
                    id = 0;
                    if (game_read(0, text_b + 0x38, &ref, 8) == 0 ||
                        ref != 0 ||
                        game_read(0, text_b + 0x40, &id, 4) == 0 || id != -1)
                        game_fn_release_node((void *)text_b);
                    ref = 1;
                    id = 0;
                    if (game_read(0, text_a + 0x38, &ref, 8) != 0 &&
                        ref == 0 &&
                        game_read(0, text_a + 0x40, &id, 4) != 0 &&
                        id == -1) {
                        ref = 1;
                        id = 0;
                        if (game_read(0, text_b + 0x38, &ref, 8) != 0 &&
                            ref == 0 &&
                            game_read(0, text_b + 0x40, &id, 4) != 0 &&
                            id == -1) {
                            memset(slot, 0, 10 * sizeof(uint64_t));
                            game_fn_release_text((void *)text_a);
                            game_fn_release_node((void *)text_b);
                            g_hud_release_counter++;
                        }
                    } else {
                        if (g_trophies_hud_journal_count < 0x20) {
                            char detail[0x50];
                            snprintf(detail, sizeof detail, ",\"detail\":%u",
                                     0);
                            runtime_journal_write(
                                "trophies_hud", "release_detach_refused",
                                detail);
                            g_trophies_hud_journal_count++;
                        }
                    }
                }
            }
        }
        for (int i = 0; i < DEDUP_SLOTS; i++) {
            if (g_hud_dedup_a[i] == hud_id)
                g_hud_dedup_a[i] = 0;
            if (g_hud_dedup_b[i] == hud_id)
                g_hud_dedup_b[i] = 0;
        }
    }
    errno = saved_errno;
}
extern const char *g_ui_key_table[0x76 * 3];
extern int32_t g_ui_key_values[0x76];


extern uint64_t g_hud_last_epoch;
extern uint64_t g_hud_last_stamp;
extern float g_hud_last_x;
extern float g_hud_last_y;
extern uint64_t g_hud_text_state;

static const uint32_t hud_name_offsets[8] = {
    0, 0, 0, 0, 0, 0, 0, 0,
};

void trophies_hud_update_callback(void *hud)
{
    int saved_errno = errno;
    if (hud == NULL)
        goto out;

    uint64_t hud_id = *(uint64_t *)((char *)hud + 0x98);
    uint64_t expected = 0;

    __atomic_fetch_add(&g_hud_update_counter, 1, __ATOMIC_ACQ_REL);
    if (__atomic_exchange_n(&expected, 0, __ATOMIC_ACQ_REL) == 0 &&
        g_trophies_hud_journal_count < 0x20) {
        char detail[0x50];
        snprintf(detail, sizeof detail, ",\"detail\":%u", 0x13);
        runtime_journal_write("trophies_hud", "update_callback_seen",
                              detail);
        g_trophies_hud_journal_count++;
    }

    if (g_trophies_hud_owner_state != 3) {
        __atomic_fetch_add(&g_hud_gate_counters[1], 1, __ATOMIC_ACQ_REL);
        __atomic_fetch_or(&g_hud_gate_mask, 1ull << 1, __ATOMIC_ACQ_REL);
        goto out;
    }

    if (g_main_thread_id == 0 || g_main_thread_id != gettid()) {
        __atomic_fetch_add(&g_hud_gate_counters[2], 1, __ATOMIC_ACQ_REL);
        __atomic_fetch_or(&g_hud_gate_mask, 1ull << 2, __ATOMIC_ACQ_REL);
        goto out;
    }

    uintptr_t view = 0, screen = 0, own = 0;
    if (game_read(0, hud_id, &view, 8) == 0 ||
        view + 0x2000 < 0x12000 || (view & 7) != 0 ||
        view != g_game_base + 0x11be3f0) {
        __atomic_fetch_add(&g_hud_gate_counters[3], 1, __ATOMIC_ACQ_REL);
        __atomic_fetch_or(&g_hud_gate_mask, 1ull << 3, __ATOMIC_ACQ_REL);
        goto out;
    }
    uintptr_t proj = 0;
    if (game_read(0, view + 0x590, &proj, 8) == 0 ||
        proj + 0x2000 < 0x12000 || (proj & 7) != 0 ||
        game_read(0, proj, &screen, 8) == 0 ||
        screen + 0x2000 < 0x12000 || (screen & 7) != 0 ||
        screen != g_game_base + 0x11ad208) {
        __atomic_fetch_add(&g_hud_gate_counters[4], 1, __ATOMIC_ACQ_REL);
        __atomic_fetch_or(&g_hud_gate_mask, 1ull << 4, __ATOMIC_ACQ_REL);
        goto out;
    }
    if (game_read(0, view + 0x10, &own, 8) == 0 ||
        own + 0x2000 < 0x12000 || (own & 7) != 0) {
        __atomic_fetch_add(&g_hud_gate_counters[5], 1, __ATOMIC_ACQ_REL);
        __atomic_fetch_or(&g_hud_gate_mask, 1ull << 5, __ATOMIC_ACQ_REL);
        goto out;
    }

    uint64_t *slot = NULL;
    uint64_t *free_slot = NULL;
    for (int i = 0; i < HUD_FIELD_SLOTS; i++) {
        if (g_hud_field_table[i][0] == hud_id)
            slot = g_hud_field_table[i];
        if (g_hud_field_table[i][0] == 0 && free_slot == NULL)
            free_slot = g_hud_field_table[i];
    }
    if (slot != NULL) {
        if (slot[2] == proj && hud_slot_validate(slot) != 0) {
            uint8_t zero = 0;
            game_write(g_write_channel_fd, &zero, 1, slot[3] + 8);
            game_write(g_write_channel_fd, &zero, 1, slot[4] + 8);
            slot[9] = 0;
        }
        if (slot[2] != proj || slot[1] != own ||
            hud_slot_validate(slot) == 0) {
            __atomic_fetch_add(&g_hud_gate_counters[6], 1, __ATOMIC_ACQ_REL);
            __atomic_fetch_or(&g_hud_gate_mask, 1ull << 6, __ATOMIC_ACQ_REL);
            goto out;
        }
    }

    uint64_t epoch = frame_epoch_now();
    if (epoch == 0) {
        __atomic_fetch_add(&g_hud_gate_counters[7], 1, __ATOMIC_ACQ_REL);
        __atomic_fetch_or(&g_hud_gate_mask, 1ull << 7, __ATOMIC_ACQ_REL);
        goto out;
    }

    uint64_t snap_epoch = 0;
    int64_t snap_flags = -1;
    char *key = "trophiesAboveHead";
    entity_index_t idx;
    if (g_xray_state_ready != 1 || g_snapshot_keys_fn == NULL ||
        ((int32_t (*)(const char *, void *, int, void *, int *))
             g_snapshot_keys_fn)(key, &snap_epoch, 1, &snap_flags,
                                 (int *)&idx) != 1 ||
        snap_epoch == 0 || snap_flags != 0 ||
        ((int32_t *)&idx)[0x1a] != 2 ||
        *(uint32_t *)((char *)&idx + 0x44) != 1) {
        __atomic_fetch_add(&g_hud_gate_counters[8], 1, __ATOMIC_ACQ_REL);
        __atomic_fetch_or(&g_hud_gate_mask, 1ull << 8, __ATOMIC_ACQ_REL);
        goto out;
    }

    entity_index_t battle_idx;
    if (entity_index_fetch(g_game_base, own, (uintptr_t)game_read, 0,
                           &battle_idx) == 0) {
        __atomic_fetch_add(&g_hud_gate_counters[9], 1, __ATOMIC_ACQ_REL);
        __atomic_fetch_or(&g_hud_gate_mask, 1ull << 9, __ATOMIC_ACQ_REL);
        goto out;
    }
    if (battle_idx.team == 0) {
        __atomic_fetch_add(&g_hud_gate_counters[10], 1, __ATOMIC_ACQ_REL);
        __atomic_fetch_or(&g_hud_gate_mask, 1ull << 10, __ATOMIC_ACQ_REL);
        goto out;
    }

    float own_y = 0.0f;
    float own_scale = 0.0f;
    uintptr_t own_slot = 0;
    uint32_t own_colors[4] = {0, 0, 0, 0};
    if (g_trophies_native_fields == 0 || battle_idx.stamp == 0 ||
        game_read(0, g_game_base + 0x1307e20, &own_slot, 8) == 0 ||
        own_slot + 0x2000 < 0x12000 || (own_slot & 7) != 0)
        goto gate_11;
    {
        uint32_t count = 0;
        if (game_read(0, own_slot + 0xc, &count, 4) == 0 ||
            (int32_t)count < 1 || (int32_t)count > 0x40 ||
            count <= battle_idx.stamp)
            goto gate_11;
        uintptr_t roster_arr = 0;
        if (game_read(0, own_slot + battle_idx.stamp * 8, &roster_arr, 8) == 0 ||
            roster_arr + 0x2000 < 0x12000 || (roster_arr & 7) != 0)
            goto gate_11;
        uintptr_t roster_entry = 0;
        if (roster_entry_read(roster_arr, battle_idx.stamp,
                              &roster_entry) == 0 ||
            *(uintptr_t *)&roster_entry == 0)
            goto gate_11;
        if (own_colors[0] != battle_idx.speed) {
            uint32_t i = 0;
            while (i + 1 != own_colors[0] && own_colors[1 + i] != battle_idx.speed)
                i++;
            if (i >= own_colors[0])
                goto gate_11;
        }
        if (game_read(0, own_slot, &roster_arr, 8) == 0 ||
            roster_arr + 0x10000 < 0x12000 || (roster_arr & 7) != 0 ||
            roster_arr != own_slot ||
            game_read(0, own_slot + 0xc, &count, 4) == 0 ||
            count != *(uint32_t *)((char *)&idx + 0x48))
            goto gate_11;
        uintptr_t name_arr = 0, name_lst = 0;
        uint32_t name_count = 0;
        if (game_read(0, own_slot + 0x28, &name_arr, 8) == 0 ||
            name_arr + 0x2000 < 0x12000 || (name_arr & 7) != 0 ||
            game_read(0, name_arr, &name_lst, 8) == 0 ||
            name_lst + 0x2000 < 0x12000 || (name_lst & 7) != 0 ||
            game_read(0, name_arr + 0xc, &name_count, 4) == 0 ||
            (int32_t)name_count < 1 || (int32_t)name_count > 0x200 ||
            game_read(0, name_lst, &own_colors, (size_t)name_count * 8) == 0)
            goto gate_11;
        uint32_t own_hits = 0;
        for (uint32_t i = 0; i < name_count; i++)
            if (own_colors[i] == epoch)
                own_hits++;
        if (own_hits != 1 ||
            game_read(0, name_arr, &name_lst, 8) == 0 ||
            name_lst != *(uintptr_t *)&own_colors[0])
            goto gate_11;
    }

    uintptr_t text_parent = 0, text_state = 0, text_screen = 0;
    if (game_read(0, own + 0x20, &text_parent, 8) == 0 ||
        text_parent + 0x10000 < 0x12000 || (text_parent & 7) != 0 ||
        text_parent != hud_id)
        goto gate_12;
    if (game_read(0, hud_id + 0x2f0, &text_state, 8) == 0 ||
        text_state + 0x2000 < 0x12000 || (text_state & 7) != 0 ||
        text_state != g_game_base + 0x11be1d8 ||
        game_read(0, hud_id + 0x388, &text_screen, 8) == 0 ||
        text_screen + 0x2000 < 0x12000 || (text_screen & 7) != 0 ||
        game_read(0, hud_id + 0x390, &own_slot, 8) == 0 ||
        own_slot + 0x10000 < 0x12000 || (own_slot & 7) != 0 ||
        own_slot != hud_id ||
        game_read(0, hud_id + 0x398, &proj, 8) == 0 ||
        proj + 0x10000 < 0x12000 || (proj & 7) != 0 ||
        proj != *(uintptr_t *)&battle_idx ||
        ui_node_order(text_screen, text_state) == 0)
        goto gate_12;

    {
        char name_buf[0x60];
        snprintf(name_buf, sizeof name_buf, "player_name");
        uintptr_t name_node = (uintptr_t)game_fn_find_node(
            (void *)*(uintptr_t *)&battle_idx, name_buf);
        uintptr_t name_base = 0;
        if (game_read(0, name_node, &name_base, 8) == 0 ||
            name_base + 0x2000 < 0x12000 || (name_base & 7) != 0 ||
            name_base != g_game_base + 0x11abad8 ||
            ui_node_create(name_node, *(uintptr_t *)&battle_idx) == 0)
            goto gate_15;

        uintptr_t glyph_slot = 0;
        if (game_read(0, name_node + 0x20, &glyph_slot, 8) == 0)
            goto gate_16;
        own_scale = *(float *)&glyph_slot;
        float abs_scale = (own_scale < 0.0f) ? -own_scale : own_scale;
        if (abs_scale == INFINITY || isnan(abs_scale))
            goto gate_16;
        if (!(abs_scale >= 4096.0f))
            goto gate_16;
        own_y = *(float *)((char *)&glyph_slot + 4);

        int team_mode = 0;
        for (int i = 0; i < 8; i++) {
            uintptr_t team_node = 0;
            if (game_read(0, hud_id + hud_name_offsets[i], &team_node, 8) == 0)
                goto gate_17;
            if (team_node != 0) {
                uintptr_t team_base = 0;
                if (game_read(0, team_node, &team_base, 8) == 0 ||
                    team_base + 0x2000 < 0x12000 ||
                    (team_base & 7) != 0 ||
                    team_base != g_game_base + 0x11ad208 ||
                    ui_node_create(team_node, *(uintptr_t *)&battle_idx) == 0)
                    goto gate_19;
                uint8_t flag = 0;
                if (game_read(0, team_node + 8, &flag, 1) == 0 || flag > 1)
                    goto gate_20;
                if (flag != 0)
                    team_mode = 1;
            }
        }

        if (slot == NULL) {
            for (int i = 0; i < DEDUP_SLOTS; i++) {
                if (g_hud_dedup_a[i] == hud_id)
                    goto gate_21;
            }
            if (free_slot == NULL) {
                __atomic_fetch_add(&g_hud_gate_counters[0x16], 1,
                                   __ATOMIC_ACQ_REL);
                __atomic_fetch_or(&g_hud_gate_mask, 1ull << 0x16,
                                  __ATOMIC_ACQ_REL);
                goto out;
            }

            uintptr_t style = (uintptr_t)game_fn_style_lookup(
                "sc/ui.sc", "popover_text_left", 1);
            uintptr_t style_base = 0;
            if (game_read(0, style, &style_base, 8) == 0 ||
                style_base + 0x2000 < 0x12000 || (style_base & 7) != 0 ||
                style_base != g_game_base + 0x11ad208)
                goto donor_failed;
            uint64_t ref = 0;
            int32_t id = 0;
            if (game_read(0, style + 0x38, &ref, 8) == 0 || ref != 0 ||
                game_read(0, style + 0x40, &id, 4) == 0 || id != -1)
                goto donor_failed;

            uintptr_t donor_text = (uintptr_t)game_fn_find_node(
                (void *)style, "\x11\xae\x2d");
            uintptr_t donor_base = 0;
            if (game_read(0, donor_text, &donor_base, 8) == 0 ||
                donor_base + 0x2000 < 0x12000 || (donor_base & 7) != 0 ||
                donor_base != g_game_base + 0x11abad8)
                goto donor_failed;
            if (ui_node_adopt(donor_text, donor_base) != 1) {
                ui_node_reparent(style, donor_text, 0);
                if (g_trophies_hud_journal_count < 0x20) {
                    char detail[0x50];
                    snprintf(detail, sizeof detail, ",\"detail\":%u", 0);
                    runtime_journal_write("trophies_hud",
                                          "donor_named_contract", detail);
                    g_trophies_hud_journal_count++;
                }
                goto donor_end;
            }

            ref = 0;
            id = 0;
            if (game_read(0, donor_text + 0x38, &ref, 8) != 0 && ref == 0 &&
                game_read(0, donor_text + 0x40, &id, 4) != 0 && id == -1) {
                uint16_t width = 0;
                if (game_read(0, style + 0x4e, &width, 2) == 0 ||
                    width > 0x1ff)
                    goto donor_failed;
                game_fn_add_child((void *)style, (void *)donor_text, 0);
            }
            if (ui_node_detach(donor_text, style) == 0)
                goto donor_failed;

            uintptr_t icon_style = (uintptr_t)game_fn_style_lookup(
                "sc/ui.sc", "icon_trophy", 1);
            uintptr_t icon_base = 0;
            if (game_read(0, icon_style, &icon_base, 8) == 0 ||
                icon_base + 0x2000 < 0x12000 || (icon_base & 7) != 0 ||
                icon_base != g_game_base + 0x11ad208)
                goto donor_failed;
            ref = 0;
            id = 0;
            if (game_read(0, icon_style + 0x38, &ref, 8) == 0 || ref != 0 ||
                game_read(0, icon_style + 0x40, &id, 4) == 0 || id != -1)
                goto donor_failed;

            uint32_t color = 0xffd700;
            uint16_t font = BODY_FONT_SIZE;
            uint8_t scale_b = 1;
            uint8_t visible = 0;
            if (game_write(g_write_channel_fd, &color, 4,
                           donor_text + 0x80) == 4 &&
                game_write(g_write_channel_fd, &font, 2,
                           donor_text + 0xb0) == 2 &&
                game_write(g_write_channel_fd, &scale_b, 1,
                           donor_text + 0x90) == 1 &&
                game_write(g_write_channel_fd, &visible, 1,
                           donor_text + 0x96) == 1 &&
                game_write(g_write_channel_fd, &visible, 1,
                           donor_text + 8) == 1 &&
                game_write(g_write_channel_fd, &visible, 1,
                           icon_style + 8) == 1) {
                game_fn_set_color(0x3e19999a, (void *)icon_style);
                game_fn_add_child((void *)text_state, (void *)donor_text, 0);
                if (ui_node_detach(donor_text, text_state) == 0 ||
                    ui_node_order(style, donor_text) == 0) {
                    if (ui_node_detach(donor_text, text_state) != 0) {
                        game_fn_release_node((void *)donor_text);
                        game_fn_add_child((void *)style, (void *)donor_text,
                                          0);
                    }
                    ui_node_reparent(style, donor_text, text_state);
                    game_fn_release_text((void *)icon_style);
                    if (g_trophies_hud_journal_count < 0x20) {
                        char detail[0x50];
                        snprintf(detail, sizeof detail, ",\"detail:%u", 0);
                        runtime_journal_write("trophies_hud",
                                              "donor_handoff_refused",
                                              detail);
                        g_trophies_hud_journal_count++;
                    }
                    goto donor_end;
                }
                game_fn_release_node((void *)style);
                game_fn_add_child((void *)text_state, (void *)icon_style, 0);
                if (ui_node_detach(icon_style, text_state) != 0) {
                    free_slot[7] = epoch;
                    free_slot[9] = 0;
                    free_slot[0] = hud_id;
                    free_slot[1] = own;
                    free_slot[4] = icon_style;
                    free_slot[2] = text_state;
                    free_slot[3] = donor_text;
                    free_slot[8] = ~(uint64_t)0;
                    free_slot[5] = text_parent;
                    *(int *)&free_slot[6] = (int32_t)text_screen;
                    g_hud_icon_counter++;
                    if (g_trophies_hud_journal_count < 0x20) {
                        char detail[0x50];
                        snprintf(detail, sizeof detail, ",\"detail\":%u", 0);
                        runtime_journal_write("trophies_hud",
                                              "native_field_icon_created",
                                              detail);
                        g_trophies_hud_journal_count++;
                    }
                    slot = free_slot;
                    goto slot_ready;
                }
                game_fn_release_node((void *)donor_text);
                ref = 0;
                id = 0;
                if (game_read(0, donor_text + 0x38, &ref, 8) != 0 &&
                    ref == 0 &&
                    game_read(0, donor_text + 0x40, &id, 4) != 0 &&
                    id == -1)
                    game_fn_release_text((void *)donor_text);
                ref = 0;
                id = 0;
                if (game_read(0, icon_style + 0x38, &ref, 8) == 0 ||
                    ref != 0 ||
                    game_read(0, icon_style + 0x40, &id, 4) == 0 ||
                    id != -1)
                    goto donor_end;
                game_fn_release_text((void *)icon_style);
                goto donor_end;
            } else {
                game_fn_release_text((void *)icon_style);
            }
        donor_failed:
            game_fn_release_text((void *)text_state);
        donor_end:
            __atomic_fetch_add(&g_hud_gate_counters[0x17], 1,
                               __ATOMIC_ACQ_REL);
            __atomic_fetch_or(&g_hud_gate_mask, 1ull << 0x17,
                              __ATOMIC_ACQ_REL);
            goto out;
        }

slot_ready:
        if (*(int *)&slot[6] != (int32_t)text_screen) {
            __atomic_fetch_add(&g_hud_gate_counters[0x18], 1,
                               __ATOMIC_ACQ_REL);
            __atomic_fetch_or(&g_hud_gate_mask, 1ull << 0x18,
                              __ATOMIC_ACQ_REL);
            goto out;
        }

        slot[7] = epoch;
        slot[5] = text_parent;
        uint64_t ref = 1;
        int32_t id = 0;
        if (game_read(0, slot[3] + 0x38, &ref, 8) != 0 && ref == 0 &&
            game_read(0, slot[3] + 0x40, &id, 4) != 0 && id == -1)
            game_fn_add_child((void *)*(uintptr_t *)&battle_idx,
                              (void *)slot[3], 0);
        ref = 1;
        id = 0;
        if (game_read(0, slot[4] + 0x38, &ref, 8) != 0 && ref == 0 &&
            game_read(0, slot[4] + 0x40, &id, 4) != 0 && id == -1)
            game_fn_add_child((void *)*(uintptr_t *)&battle_idx,
                              (void *)slot[4], 0);
        if (ui_node_detach(slot[3], *(uintptr_t *)&battle_idx) == 0 ||
            ui_node_detach(slot[4], *(uintptr_t *)&battle_idx) == 0) {
            __atomic_fetch_add(&g_hud_gate_counters[0x19], 1,
                               __ATOMIC_ACQ_REL);
            __atomic_fetch_or(&g_hud_gate_mask, 1ull << 0x19,
                              __ATOMIC_ACQ_REL);
            goto out;
        }

        if ((int64_t)slot[8] != (int64_t)epoch ||
            *(int32_t *)((char *)slot + 0x44) != -1) {
            if (game_region_verify(0x5921a0, 0xb0,
                    "6b5853b7737f55c2722ea9a461730d5a0274ec3f756ad12c93cede1764df931a") == 0) {
                if (g_trophies_hud_owner_fn == NULL) {
                    uintptr_t found = 0;
                    uint32_t count = 0;
                    owner_discover_ctx ctx = { &count, &found };
                    dl_iterate_phdr(owner_phdr_cb, &ctx);
                    if (count == 1 && found != 0) {
                        g_trophies_hud_owner_fn = (void *)found;
                        if (((int (*)(uintptr_t))g_trophies_hud_owner_fn)(
                                g_game_base) == 1)
                            goto text_ok;
                    }
                } else {
                    if (((int (*)(uintptr_t))g_trophies_hud_owner_fn)(
                            g_game_base) == 1)
                        goto text_ok;
                }
                __atomic_fetch_add(&g_hud_gate_counters[0x1a], 1,
                                   __ATOMIC_ACQ_REL);
                __atomic_fetch_or(&g_hud_gate_mask, 1ull << 0x1a,
                                  __ATOMIC_ACQ_REL);
                goto out;
            }
text_ok:
            {
                char num[0x20];
                snprintf(num, sizeof num, "%d", (int)epoch);
                void *str = NULL;
                game_fn_string_format(&str, "%s", num);
                game_fn_node_set_text((void *)slot[3], str);
                game_fn_string_release(&str);
                slot[8] = epoch;
                *(int32_t *)((char *)slot + 0x44) = -1;
            }
        }

        float y_base = own_scale;
        if (g_leon_clone_latch != 0 && g_hud_last_epoch == hud_id &&
            g_hud_last_stamp == epoch &&
            (own_scale - g_hud_last_x < 0.5f && own_scale - g_hud_last_x > -0.5f))
            y_base = g_hud_last_y;
        float y_off = team_mode ? -45.0f : -20.0f;
        y_off += own_y;
        game_fn_set_position(y_base - 6.0f, y_off, (void *)slot[3]);
        game_fn_set_position(y_base - 21.0f, y_off + 7.0f, (void *)slot[4]);

        if (frame_epoch_now() != epoch) {
            __atomic_fetch_add(&g_hud_gate_counters[0x1b], 1,
                               __ATOMIC_ACQ_REL);
            __atomic_fetch_or(&g_hud_gate_mask, 1ull << 0x1b,
                              __ATOMIC_ACQ_REL);
            goto out;
        }

        snap_epoch = 0;
        snap_flags = -1;
        if (g_xray_state_ready != 1 || g_snapshot_keys_fn == NULL ||
            ((int32_t (*)(const char *, void *, int, void *, int *))
                 g_snapshot_keys_fn)(key, &snap_epoch, 1, &snap_flags,
                                     (int *)&idx) != 1 ||
            snap_epoch == 0 || snap_flags != 0 ||
            ((int32_t *)&idx)[0x1a] != 2 ||
            *(uint32_t *)((char *)&idx + 0x44) != 1 ||
            *(uint64_t *)((char *)&idx + 0x48) != snap_epoch) {
            __atomic_fetch_add(&g_hud_gate_counters[0x1b], 1,
                               __ATOMIC_ACQ_REL);
            __atomic_fetch_or(&g_hud_gate_mask, 1ull << 0x1b,
                              __ATOMIC_ACQ_REL);
            goto out;
        }

        uint8_t one = 1;
        if (game_write(g_write_channel_fd, &one, 1, slot[3] + 8) != 1 ||
            game_write(g_write_channel_fd, &one, 1, slot[4] + 8) != 1) {
            __atomic_fetch_add(&g_hud_gate_counters[0x1b], 1,
                               __ATOMIC_ACQ_REL);
            __atomic_fetch_or(&g_hud_gate_mask, 1ull << 0x1b,
                              __ATOMIC_ACQ_REL);
            goto out;
        }
        g_hud_text_state++;
        slot[9] = 1;
        goto out;

gate_11:
        __atomic_fetch_add(&g_hud_gate_counters[0xb], 1, __ATOMIC_ACQ_REL);
        __atomic_fetch_or(&g_hud_gate_mask, 1ull << 0xb, __ATOMIC_ACQ_REL);
        goto out;
gate_12:
        __atomic_fetch_add(&g_hud_gate_counters[0xc], 1, __ATOMIC_ACQ_REL);
        __atomic_fetch_or(&g_hud_gate_mask, 1ull << 0xc, __ATOMIC_ACQ_REL);
        goto out;
gate_15:
        __atomic_fetch_add(&g_hud_gate_counters[0xf], 1, __ATOMIC_ACQ_REL);
        __atomic_fetch_or(&g_hud_gate_mask, 1ull << 0xf, __ATOMIC_ACQ_REL);
        goto out;
gate_16:
        __atomic_fetch_add(&g_hud_gate_counters[0x10], 1, __ATOMIC_ACQ_REL);
        __atomic_fetch_or(&g_hud_gate_mask, 1ull << 0x10, __ATOMIC_ACQ_REL);
        goto out;
gate_17:
        __atomic_fetch_add(&g_hud_gate_counters[0x11], 1, __ATOMIC_ACQ_REL);
        __atomic_fetch_or(&g_hud_gate_mask, 1ull << 0x11, __ATOMIC_ACQ_REL);
        goto out;
gate_19:
        __atomic_fetch_add(&g_hud_gate_counters[0x13], 1, __ATOMIC_ACQ_REL);
        __atomic_fetch_or(&g_hud_gate_mask, 1ull << 0x13, __ATOMIC_ACQ_REL);
        goto out;
gate_20:
        __atomic_fetch_add(&g_hud_gate_counters[0x14], 1, __ATOMIC_ACQ_REL);
        __atomic_fetch_or(&g_hud_gate_mask, 1ull << 0x14, __ATOMIC_ACQ_REL);
        goto out;
gate_21:
        __atomic_fetch_add(&g_hud_gate_counters[0x15], 1, __ATOMIC_ACQ_REL);
        __atomic_fetch_or(&g_hud_gate_mask, 1ull << 0x15, __ATOMIC_ACQ_REL);
        goto out;
    }

out:
    if ((g_hud_extra_latch & 4) != 0 && g_xray_state_ready == 1) {
        for (int i = 0; i < 0x37; i++) {
            if (strcmp("ShowEnemyAmmoStatus",
                       g_ui_key_table[i * 3]) == 0) {
                if (i >= 0 && g_ui_key_values[i] != 0 &&
                    g_main_thread_id != 0 &&
                    g_main_thread_id == gettid()) {
                    uintptr_t ammo_node = 0;
                    if (game_read(0,
                                  *(uintptr_t *)((char *)hud + 0x98) + 0xb48,
                                  &ammo_node, 8) != 0 &&
                        ammo_node + 0x2000 > 0x11fff && (ammo_node & 7) == 0) {
                        uint8_t one = 1;
                        game_write(g_write_channel_fd, &one, 1,
                                   ammo_node + 8);
                    }
                }
                break;
            }
        }
    }
    errno = saved_errno;
}

static const char PLUSAPI_HOST_IP[] = "87.228.126.116";
static const char PLUSAPI_HOST[] = "plusapi.bsd.meowfox.net";

static size_t json_string_unescape(char *out, const char *in, size_t max)
{
    size_t n = 0;
    while (n < max) {
        uint8_t c = (uint8_t)*in;
        if (c == '\\') {
            uint8_t e = (uint8_t)in[1];
            if (e == 0)
                break;
            in += 2;
            switch (e) {
            case 'n': out[n++] = 10; break;
            case 'b': out[n++] = 8; break;
            case 'f': out[n++] = 12; break;
            case 'r': out[n++] = 13; break;
            case 't': out[n++] = 9; break;
            default: out[n++] = (char)e; break;
            }
        } else if (c == 0 || c == '"') {
            break;
        } else {
            out[n++] = (char)c;
            in++;
        }
    }
    if (n < max)
        out[n] = 0;
    return n;
}

static int hex_val(uint8_t c)
{
    if (c - 0x30u < 10u)
        return c - 0x30;
    if (c - 0x61u < 6u)
        return c - 0x57;
    if (c - 0x41u < 6u)
        return c - 0x37;
    return -1;
}

static int body_unescape(char *out, const char *in, size_t max)
{
    size_t pos = 0;
    size_t written = 0;
    while (pos < max) {
        uint8_t c = (uint8_t)in[pos];
        if (c == 0)
            break;
        if (c == '\\') {
            uint8_t n = (uint8_t)in[pos + 1];
            if (n == 'x' && in[pos + 2] != 0 && in[pos + 3] != 0) {
                int hi = hex_val((uint8_t)in[pos + 2]);
                int lo = hex_val((uint8_t)in[pos + 3]);
                if (hi < 0 || lo < 0)
                    return -1;
                out[written++] = (char)((hi << 4) | lo);
                pos += 4;
                continue;
            }
            switch (n) {
            case 'n': out[written++] = 10; break;
            case 'r': out[written++] = 13; break;
            case 't': out[written++] = 9; break;
            case 'b': out[written++] = 8; break;
            case 'f': out[written++] = 12; break;
            case 0: return -1;
            default: out[written++] = (char)n; break;
            }
            pos += 2;
            continue;
        }
        out[written++] = (char)c;
        pos++;
    }
    out[written] = 0;
    return (int)written;
}

uint32_t plusapi_users_fetch(void *request)
{
    uint64_t *req = request;
    char body[0x1000];
    size_t pos = 9;

    memcpy(body, "{\"data\":{", 9);

    uint32_t count = (uint32_t)req[1];
    for (uint32_t i = 0; i < count; i++) {
        const char *tag = *(const char **)(req + 3 + i * 5);
        uint32_t mask = *(const uint32_t *)((char *)(req + 3 + i * 5) - 4);
        int n = snprintf(body + pos, sizeof body - pos, "%s\"#%s\":%u",
                         (i == 0) ? "" : ",", tag, mask);
        if (n < 0 || (size_t)n >= sizeof body - pos)
            return 0;
        pos += (size_t)n;
    }

    const char *last_tag = (count != 0)
        ? *(const char **)(req + 3 + (count - 1) * 5) : "";
    int n = snprintf(body + pos, sizeof body - pos,
                     "},\"magic_number\":0,\"signature\":\"0\","
                     "\"tag\":\"#%s\",\"gameMode\":\"NULL\"}", last_tag);
    if (n < 0 || (size_t)n >= sizeof body - pos)
        return 0;
    size_t body_len = pos + (size_t)n;

    json_prepare(body, body_len);

    char *wrapped = malloc(body_len * 5 + 0xc);
    if (wrapped == NULL)
        return 0;
    memcpy(wrapped, "{\"data\":", 8);
    wrapped[8] = '"';
    size_t esc = 0;
    for (size_t i = 0; i < body_len; i++) {
        snprintf(wrapped + 9 + esc, 5 + 2, "\\\\x%02x",
                 (unsigned)(uint8_t)body[i]);
        esc += 5;
    }
    wrapped[9 + esc] = '"';
    wrapped[10 + esc] = '}';
    wrapped[11 + esc] = '\0';

    char *resp = malloc(0x20000);
    uint32_t matched = 0;
    if (resp == NULL) {
        free(wrapped);
        return 0;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    if (inet_pton(AF_INET, PLUSAPI_HOST_IP, &addr.sin_addr) != 1) {
        free(wrapped);
        free(resp);
        return 0;
    }

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        free(wrapped);
        free(resp);
        return 0;
    }

    if (connect(fd, (struct sockaddr *)&addr, sizeof addr) == 0) {
        size_t body_size = strlen(wrapped);
        char head[0x300];
        int hn = snprintf(head, sizeof head,
            "POST /bsd/api/v1/get_bsd_users_by_mask HTTP/1.1\r\n"
            "Host: %s\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: %zu\r\n"
            "Connection: close\r\n\r\n",
            PLUSAPI_HOST, body_size);
        if (hn > 0 && (size_t)hn < sizeof head) {
            size_t sent = 0;
            while (sent < (size_t)hn) {
                ssize_t s = send(fd, head + sent, (size_t)hn - sent,
                                 MSG_NOSIGNAL);
                if (s < 1)
                    break;
                sent += (size_t)s;
            }
            sent = 0;
            while (sent < body_size) {
                ssize_t s = send(fd, wrapped + sent, body_size - sent,
                                 MSG_NOSIGNAL);
                if (s < 1)
                    break;
                sent += (size_t)s;
            }

            size_t got = 0;
            for (;;) {
                ssize_t r = recv(fd, resp + got, 0x1ffff - got, 0);
                if (r == 0)
                    break;
                if (r < 0) {
                    if (errno == EINTR)
                        continue;
                    break;
                }
                got += (size_t)r;
                if (got + 1 >= 0x1ffff)
                    break;
            }
            resp[got] = '\0';

            if (strncmp(resp, "HTTP/1.1 200 ", 13) == 0 ||
                strncmp(resp, "HTTP/1.0 200 ", 13) == 0) {
                char *body_start = strstr(resp, "\r\n\r\n");
                if (body_start != NULL) {
                    body_start += 4;
                    if (body_start != resp) {
                        size_t bl = strlen(body_start);
                        memmove(resp, body_start, bl + 1);
                    }
                    char *decoded = malloc(0x10000);
                    if (decoded != NULL) {
                        char *data = strstr(resp, "\"data\"");
                        if (data != NULL) {
                            char *cursor = data + 7;
                            while (*cursor != 0 && *cursor != ':' &&
                                   *cursor != ';')
                                cursor++;
                            if (*cursor == ':') {
                                cursor++;
                                char stage1[0x10000];
                                size_t s1 = json_string_unescape(
                                    stage1, cursor, 0xfffe);
                                (void)s1;
                                if (stage1[0] == 'b') {
                                    char *s = stage1;
                                    if (s[1] == '"' || s[1] == '\'')
                                        s += 2;
                                    else
                                        s += 1;
                                    int dl = body_unescape(decoded, s,
                                                           0xfffe);
                                    if (dl > 0) {
                                        char *users = strstr(decoded,
                                                             "\"bsd_users\"");
                                        if (users != NULL && count != 0) {
                                            for (uint32_t i = 0; i < count;
                                                 i++) {
                                                const char *tag =
                                                    *(const char **)(req + 3 +
                                                                     i * 5);
                                                char pat[0x20];
                                                snprintf(pat, sizeof pat,
                                                         "\"%s\"", tag);
                                                char *entry = strstr(users,
                                                                     pat);
                                                if (entry == NULL) {
                                                    snprintf(pat, sizeof pat,
                                                             "\"#%s\"", tag);
                                                    entry = strstr(users, pat);
                                                }
                                                if (entry != NULL) {
                                                    size_t pl = strlen(pat);
                                                    char *p = entry + pl;
                                                    while (*p != 0 &&
                                                           *p != ':' &&
                                                           *p != ';')
                                                        p++;
                                                    if (*p == ':') {
                                                        char *v = p + 2;
                                                        while (*v != 0 &&
                                                               *v < 0x21)
                                                            v++;
                                                        if (*v == '{') {
                                                            char *close =
                                                                strchr(v,
                                                                       '}');
                                                            if (close !=
                                                                NULL) {
                                                                int trophies =
                                                                    0;
                                                                int character =
                                                                    0;
                                                                if (json_field_int(
                                                                        v, close,
                                                                        "trophies",
                                                                        &trophies) !=
                                                                    0)
                                                                    *(int32_t *)((char *)req +
                                                                                 i * 0x28 +
                                                                                 0x34) =
                                                                        trophies;
                                                                if (json_field_int(
                                                                        v, close,
                                                                        "character",
                                                                        &character) !=
                                                                    0)
                                                                    *(int32_t *)((char *)req +
                                                                                 i * 0x28 +
                                                                                 0x38) =
                                                                        character;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        for (uint32_t i = 0; i < count;
                                             i++) {
                                            int32_t tr = *(int32_t *)(
                                                (char *)req + i * 0x28 +
                                                0x34);
                                            if (tr >= 0)
                                                matched++;
                                        }
                                    }
                                }
                            }
                        }
                        free(decoded);
                    }
                }
            }
        }
    }

    close(fd);

    pthread_mutex_lock(&g_plusapi_mutex);
    {
        uint32_t cached_count = (uint32_t)g_plusapi_cache[0x5c];
        int same = (g_plusapi_cache[0x5d] == req[0x34]) &&
                   (g_plusapi_cache[0x58] == req[0]) &&
                   (cached_count == count) &&
                   ((int)g_plusapi_cache[0x59] ==
                    *(int *)((char *)req + 0xc)) &&
                   ((int)g_plusapi_cache[0x5a] == (int)req[2]) &&
                   ((int)g_plusapi_cache[0x5b] ==
                    *(int *)((char *)req + 0x14)) &&
                   strcmp((char *)(uintptr_t)g_plusapi_cache[3],
                          (char *)(req + 3)) == 0;
        if (same && cached_count != 0) {
            for (uint32_t i = 1; i < cached_count; i++) {
                if (strcmp((char *)(uintptr_t)g_plusapi_cache[3 + i * 5],
                           (char *)(req + 3 + i * 5)) != 0) {
                    same = 0;
                    break;
                }
            }
        }
        if (same && cached_count != 0) {
            for (uint32_t i = 0; i < cached_count; i++) {
                int32_t a = *(int32_t *)((char *)req + i * 0x28 + 0x30);
                int32_t b = *(int32_t *)((char *)req + i * 0x28 + 0x34);
                if (a >= 0)
                    *(int32_t *)((char *)&g_plusapi_cache[2 + i * 5] +
                                 0x20) = a;
                if (b >= 0)
                    *(int32_t *)((char *)&g_plusapi_cache[2 + i * 5] +
                                 0x24) = b;
            }
            g_plusapi_complete = (cached_count == matched);
        }
        g_plusapi_inflight = 0;
    }
    pthread_mutex_unlock(&g_plusapi_mutex);

    free(wrapped);
    free(resp);
    free(request);
    return matched;
}
