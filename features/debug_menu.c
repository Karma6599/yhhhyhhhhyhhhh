#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#define DEBUG_CMD_COUNT 40
#define DEBUG_ACTION_BASE 0x27020u
#define DEBUG_ACTION_CLOSE 0x27000u
#define DEBUG_ACTION_WINDOW_LO 0x27120u
#define DEBUG_ACTION_WINDOW_SPAN 0xfffffee0u

typedef struct {
    uint32_t cmd;
    uint32_t _pad0[3];
    const char *name;
    uint32_t flags[4];
} debug_command_slot_t;

static const debug_command_slot_t k_debug_commands[DEBUG_CMD_COUNT] = {
    {  0, {0,0,0}, "ABOUT SCREEN",               {0,0,0,0} },
    {  1, {0,0,0}, "GENERIC INFO",               {0,0,0,0} },
    {  2, {0,0,0}, "ESPORTS",                    {0,0,0,0} },
    {  3, {0,0,0}, "NOTIFICATIONS",              {0,0,0,0} },
    {  4, {0,0,0}, "FAME PREVIEW",               {0,0,2,0} },
    {  5, {0,0,0}, "GO TO HOME",                 {0,0,0,0} },
    {  6, {0,0,0}, "GO TO QUESTS",               {0,0,0,0} },
    {  7, {0,0,0}, "GO TO BRAWL PASS",           {0,0,0,0} },
    {  8, {0,0,0}, "GO TO CLUBS",                {0,0,0,0} },
    {  9, {0,0,0}, "GO TO PRO PASS",             {0,0,0,0} },
    { 10, {0,0,0}, "GO TO CLAN",                 {0,0,0,0} },
    { 11, {0,0,0}, "SCID REWARDS",               {0,0,0,0} },
    { 12, {0,0,0}, "CHAT OPTIONS",               {0,0,0,0} },
    { 13, {0,0,0}, "BRAWLER REWARD PREVIEW",     {0,0,0,0} },
    { 14, {0,0,0}, "GEMS POPUP",                 {0,0,0,0} },
    { 15, {0,0,0}, "FAME LEVEL UP",              {0,0,0,0} },
    { 16, {0,0,0}, "RANKED SEASON END",          {0,0,0,0} },
    { 17, {0,0,0}, "MOVIE PLAYER",               {0,0,0,0} },
    { 18, {0,0,0}, "BRAWL TV INTRO",             {0,0,0,0} },
    { 19, {0,0,0}, "PRESTIGE INTRO",             {0,0,0,0} },
    { 20, {0,0,0}, "INVITE FRIEND CODE",         {0,0,0,0} },
    { 21, {0,0,0}, "UNLOCK ACCOUNT SCREEN",      {0,0,0,0} },
    { 22, {0,0,0}, "COUNTRY",                    {0,0,0,0} },
    { 23, {0,0,0}, "STOP ALL SOUND EFFECTS",     {0,0,0,0} },
    { 24, {0,0,0}, "STOP MUSIC",                 {0,0,0,0} },
    { 25, {0,0,0}, "PAUSE / RESUME MUSIC",       {0,0,0,0} },
    { 26, {0,0,0}, "MUSIC VOLUME",               {0,0,0,0} },
    { 27, {0,0,0}, "BOSS MUSIC",                 {0,0,0,0} },
    { 28, {0,0,0}, "CLOSE ALL POPUPS",           {0,0,0,0} },
    { 29, {0,0,0}, "TEST FLOATER",               {0,0,0,0} },
    { 30, {0,0,0}, "OPEN CLUB WINDOW",           {0,0,0,0} },
    { 31, {0,0,0}, "OPEN TEAM WINDOW",           {0,0,0,0} },
    { 32, {0,0,0}, "LATENCY TEST",               {0,0,0,0} },
    { 33, {0,0,0}, "NEXT LANGUAGE",              {0,0,0,0} },
    { 34, {0,0,0}, "SHOW / HIDE TEXT KEYS",      {0,0,0,0} },
    { 35, {0,0,0}, "SKIP GACHA ANIMATION",       {0,0,0,0} },
    { 36, {0,0,0}, "GRAPHICS QUALITY",           {0,0,0,0} },
    { 37, {0,0,0}, "MEMORY QUALITY",             {0,0,0,0} },
    { 38, {0,0,0}, "RELOAD GAME",                {0,0,0,0} },
    { 39, {0,0,0}, "PLAY TIME DIALOG",           {0,0,0,0} },
};

#define DEBUG_SCREEN_CHAR 'd'
#define DEBUG_SCREEN_DEFAULT 100

typedef struct {
    uint32_t magic;
    uint32_t size;
    uint32_t entry;
    uint32_t aux;
    uint32_t count;
    uint32_t bitmap[8];
} debug_availability_t;

_Static_assert(sizeof(debug_availability_t) == 0x34, "debug_availability_t size");

extern uint64_t g_menu_guard;
extern int32_t  g_menu_owner_tid;
extern uint8_t  g_menu_screen_open;
extern char     g_menu_screen;
extern uint8_t  g_menu_battle;
extern uint8_t  g_menu_flag_9f;
extern uint64_t g_menu_revision;
extern uint32_t g_menu_last_action;
extern uint32_t g_menu_last_result;
extern uint32_t g_menu_blocked_reason;
extern uint64_t g_menu_diag_state;
extern uint32_t g_menu_diag_valid;
extern char     g_menu_reason[64];

extern debug_availability_t g_debug_avail;
extern uint32_t g_debug_slot_bitmap[2];
extern uint32_t g_debug_saved_screen;
extern uint64_t g_debug_state_packed;
extern uint32_t g_debug_action_state;
extern uint32_t g_debug_slot;
extern uint32_t g_debug_entry;
extern uint32_t g_debug_perf_mode;
extern int64_t  g_debug_open_rev;
extern uint64_t g_debug_last_pump_ms;
extern uint64_t g_debug_dialog;
extern char     g_debug_message[0x60];
extern int64_t  g_debug_report_rev;
extern uint32_t g_debug_report_action;
extern uint32_t g_debug_report_result;

extern int menu_guard_busy(int op, uint64_t *guard);
extern int menu_text_set(char *dst, int64_t reserved, size_t cap,
                         const char *fmt, ...);
extern int debug_availability_read(void *out, uint32_t size);
extern uint64_t dialog_session_create(void);
extern int dialog_value_open(uint64_t handle, const char *name,
                             const char *fmt, int a, int b);
extern int dialog_input_poll(uint64_t handle, int32_t *state,
                             uint8_t *buf, uint32_t cap);
extern void dialog_session_close(uint64_t handle);
extern int debug_command_dispatch(uint32_t cmd, uint32_t value);
extern uint32_t nexus_ui_performance_mode(void);

#define DEBUG_DIALOG_STATE_NONE 0
#define DEBUG_DIALOG_STATE_VALUE 1
#define DEBUG_DIALOG_STATE_PENDING 2
#define DEBUG_DIALOG_STATE_QUEUED 3
#define DEBUG_DIALOG_STATE_DONE 4

int nexus_menu_diagnostics(char *out, size_t cap)
{
    int result = 0;

    if (out == NULL || cap == 0)
        return 0;

    if ((menu_guard_busy(1, &g_menu_guard) & 1) == 0) {
        uint64_t state = g_menu_diag_valid != 0 ? g_menu_diag_state : 0;
        result = menu_text_set(
            out, -1, cap,
            "{\"schema\":1,\"state\":%d,\"screen\":\"%c\",\"open\":%u,"
            "\"battle\":%u,\"revision\":%u,\"owner_tid\":%d,"
            "\"last_action\":%u,\"last_result\":%u,\"blocked_reason\":%u,"
            "\"reason\":\"%s\"}",
            (uint32_t)state, g_menu_screen, g_menu_screen_open,
            g_menu_battle, (uint32_t)g_menu_revision, g_menu_owner_tid,
            g_menu_last_action, g_menu_last_result, g_menu_blocked_reason,
            g_menu_reason);
        g_menu_guard = 0;
    } else {
        result = menu_text_set(out, -1, cap, "{\"state\":2,\"reason\":\"busy\"}");
    }

    return result > 0 ? result : 0;
}

uint32_t nexus_menu_debug_open(int owner_tid)
{
    if ((menu_guard_busy(1, &g_menu_guard) & 1) != 0)
        return 3;

    uint32_t result = 4;
    if (owner_tid > 0 && g_menu_owner_tid == owner_tid
        && g_menu_battle == 0 && g_menu_screen_open != 0) {

        if (g_menu_screen != DEBUG_SCREEN_DEFAULT)
            g_debug_saved_screen = (uint32_t)(uint8_t)g_menu_screen;

        g_debug_action_state = 0;
        g_debug_state_packed = 0;
        g_debug_last_pump_ms = 0;
        memset(g_debug_message, 0, sizeof g_debug_message);
        g_debug_slot_bitmap[0] = 0;
        g_debug_slot_bitmap[1] = 0;
        g_debug_open_rev++;
        g_menu_revision++;
        g_menu_flag_9f = 0;
        g_menu_screen = DEBUG_SCREEN_DEFAULT;
        result = 1;
    }

    g_menu_guard = 0;
    return result;
}

static int debug_cmd_available(uint32_t cmd)
{
    if (cmd > 0xff)
        return 0;
    if (cmd >= g_debug_avail.count)
        return 0;
    if ((g_debug_avail.bitmap[cmd >> 5] >> (cmd & 0x1f) & 1u) == 0)
        return 0;
    return 1;
}

uint32_t nexus_menu_debug_action(uint32_t action_id, int owner_tid)
{
    if (action_id - DEBUG_ACTION_WINDOW_LO < DEBUG_ACTION_WINDOW_SPAN)
        return 4;

    if ((menu_guard_busy(1, &g_menu_guard) & 1) != 0)
        return 3;

    if (owner_tid < 1 || g_menu_owner_tid != owner_tid
        || g_menu_screen_open == 0 || g_menu_battle != 0
        || g_menu_screen != DEBUG_SCREEN_CHAR) {
        g_menu_guard = 0;
        return 4;
    }

    uint32_t result;

    if (action_id == DEBUG_ACTION_CLOSE) {
        g_debug_action_state = 0;
        g_menu_flag_9f = 0;
        g_debug_open_rev++;
        g_menu_screen = (char)(uint8_t)g_debug_saved_screen;
        result = 1;
    } else {
        if (g_debug_action_state != 0) {
            g_menu_guard = 0;
            return 2;
        }

        uint32_t slot = action_id - DEBUG_ACTION_BASE;
        if (slot > 0x27) {
            g_menu_guard = 0;
            return 4;
        }
        if ((uint32_t)g_debug_state_packed == 0) {
            g_menu_guard = 0;
            return 4;
        }
        if ((g_debug_slot_bitmap[slot >> 5] >> (action_id & 0x1f) & 1u) == 0) {
            g_menu_guard = 0;
            return 4;
        }

        uint32_t cmd = k_debug_commands[slot].cmd;
        if (!debug_cmd_available(cmd)) {
            g_menu_guard = 0;
            return 0;
        }

        const char *message = "OPENING...";
        g_debug_action_state = DEBUG_DIALOG_STATE_QUEUED;
        if (slot == 4) {
            g_debug_action_state = DEBUG_DIALOG_STATE_VALUE;
            message = "ENTER A VALUE";
        }

        g_debug_entry = g_debug_avail.entry;
        g_debug_slot = slot;
        menu_text_set(g_debug_message, 0x60, 0x60, "%s", message);
        result = 2;
    }

    g_menu_revision++;
    g_menu_guard = 0;
    return result;
}

static uint32_t availability_supported(const debug_availability_t *avail)
{
    if (avail->count < 0x100) {
        if ((avail->bitmap[avail->count >> 5] >> (avail->count & 0x1f) & 1u) != 0)
            return 0;
        uint32_t probe = avail->count;
        if (probe == 0xff)
            return 0;
        uint32_t next = probe + 1;
        while ((avail->bitmap[next >> 5] >> (next & 0x1f) & 1u) == 0) {
            probe = next;
            if (probe == 0xff)
                break;
            next = probe + 1;
        }
        return (0xfe < probe) ? 1u : 0u;
    }
    return 1;
}

static uint32_t parse_digits(const uint8_t *buf, uint32_t *value_out)
{
    if (buf[0] == 0)
        return 0;

    uint32_t value = 0;
    const uint8_t *p = buf + 1;
    uint8_t c = buf[0];

    do {
        if ((uint32_t)(c - 0x3a) >= 0xfffffff6u || (0x8000002fu - c) / 10 < value) {
            *value_out = 0;
            return 0;
        }
        value = c + value * 10;
        c = *p++;
        value -= 0x30;
    } while (c != 0);

    *value_out = value;
    return 1;
}

uint32_t nexus_menu_debug_pump(int owner_tid, uint64_t now_ms)
{
    uint64_t dialog = g_debug_dialog;
    int64_t open_rev = g_debug_open_rev;
    uint32_t entry = g_debug_entry;
    uint32_t slot = g_debug_slot;

    if ((menu_guard_busy(1, &g_menu_guard) & 1) != 0)
        return 3;

    if (owner_tid < 1 || g_menu_owner_tid != owner_tid) {
        g_menu_guard = 0;
        return 4;
    }

    if (g_menu_screen_open == 0 || g_menu_battle != 0
        || g_menu_screen != DEBUG_SCREEN_CHAR) {
        g_debug_dialog = 0;
        g_debug_state_packed = (g_debug_state_packed & 0xffffffff00000000ull);
        g_debug_open_rev++;
        g_menu_guard = 0;
        if (dialog != 0)
            dialog_session_close(dialog);
        return 1;
    }

    int32_t state = (int32_t)g_debug_action_state;

    if (g_debug_dialog != 0 && g_debug_action_state == 0) {
        g_debug_state_packed &= 0xffffffffull;
        g_debug_dialog = 0;
        g_menu_guard = 0;
        dialog_session_close(dialog);
        return 1;
    }

    if (g_debug_action_state == DEBUG_DIALOG_STATE_DONE) {
        g_menu_guard = 0;
        return 3;
    }

    if (g_debug_action_state == 0 && g_debug_last_pump_ms != 0
        && g_debug_last_pump_ms <= now_ms && now_ms - g_debug_last_pump_ms < 500) {
        g_menu_guard = 0;
        return 1;
    }

    g_debug_state_packed = (g_debug_state_packed & 0xffffffffull) | (4ull << 32);
    g_menu_guard = 0;

    uint32_t perf_mode = nexus_ui_performance_mode();

    debug_availability_t avail;
    memset(&avail, 0, sizeof avail);
    int got = debug_availability_read(&avail, 0x34);

    uint32_t supported = 0;
    if (got != 0 && avail.magic == 1 && avail.size == 0x34) {
        if (avail.count < 0x101 && avail.aux < 4)
            supported = availability_supported(&avail);
    }

    uint8_t input_buf[0x401];
    memset(input_buf, 0, sizeof input_buf);

    uint32_t value = 0;
    uint64_t new_dialog = 0;
    uint32_t result_code = 0;
    int progressed = 0;
    int do_execute = 0;

    if (state == 0) {
        result_code = 1;
        progressed = 0;
    } else {
        progressed = 1;
        if (supported == 0 || slot > 0x27) {
            result_code = 0;
            progressed = 0;
        } else if (avail.entry == entry) {
            uint32_t cmd = k_debug_commands[slot].cmd;
            if (cmd > 0xff) {
                result_code = 0;
                progressed = 0;
            } else if (cmd < avail.count
                       && (avail.bitmap[cmd >> 5] >> (cmd & 0x1f) & 1u) != 0) {
                if (state == DEBUG_DIALOG_STATE_QUEUED) {
                    result_code = 1;
                    do_execute = 1;
                    progressed = 1;
                } else if (state == DEBUG_DIALOG_STATE_PENDING) {
                    int32_t poll_state = 3;
                    result_code = dialog_input_poll(dialog, &poll_state,
                                                    input_buf, 0x401);
                    progressed = 1;
                    if (result_code == 0 || poll_state == 3) {
                        value = 0;
                    } else if (poll_state == 2) {
                        result_code = 2;
                    } else if (poll_state == 1) {
                        if (parse_digits(input_buf, &value) != 0) {
                            result_code = 1;
                            do_execute = 1;
                        } else {
                            result_code = 0;
                        }
                    } else {
                        progressed = 0;
                        result_code = 0;
                    }
                } else if (state == DEBUG_DIALOG_STATE_VALUE) {
                    new_dialog = dialog_session_create();
                    result_code = (uint32_t)(new_dialog != 0);
                    if (new_dialog != 0) {
                        result_code = (uint32_t)(dialog_value_open(
                            new_dialog, k_debug_commands[slot].name, "", 1, 10) != 0);
                    }
                    progressed = result_code ^ 1;
                } else {
                    result_code = 1;
                    progressed = 0;
                }
            } else {
                result_code = 0;
                progressed = 0;
            }
        } else {
            result_code = 0;
            progressed = 0;
        }
    }

    if ((menu_guard_busy(1, &g_menu_guard) & 1) != 0) {
        if (new_dialog != 0)
            dialog_session_close(new_dialog);
        return 3;
    }

    if (g_menu_screen_open == 0 || g_menu_battle != 0
        || g_menu_screen != DEBUG_SCREEN_CHAR
        || g_debug_open_rev != open_rev
        || g_debug_action_state != DEBUG_DIALOG_STATE_DONE) {
        g_menu_guard = 0;
        if (new_dialog != 0)
            dialog_session_close(new_dialog);
        return 4;
    }

    if (supported != 0)
        g_debug_avail = avail;

    g_debug_perf_mode = perf_mode;
    g_debug_last_pump_ms = now_ms;

    if (state == 0) {
        g_debug_state_packed = (g_debug_state_packed & 0xffffffff00000000ull)
                             | supported;
    } else if (progressed == 0) {
        g_debug_state_packed = (2ull << 32) | supported;
        if (new_dialog != 0)
            g_debug_dialog = new_dialog;
    } else {
        g_debug_state_packed = (g_debug_state_packed & 0xffffffff00000000ull)
                             | supported;
        g_debug_dialog = 0;

        if ((result_code | 2u) == 2u) {
            g_debug_report_action = slot + DEBUG_ACTION_BASE;
            g_debug_report_rev++;
            g_debug_report_result = 2;
            if (result_code == 2)
                g_debug_report_result = 3;
        }

        const char *message = "UNAVAILABLE IN THE CURRENT SCREEN";
        if ((result_code | 2u) != 2u || result_code != 0)
            message = "OPENING...";
        if (result_code == 2)
            message = "CANCELLED";
        menu_text_set(g_debug_message, 0x60, 0x60, "%s", message);
    }

    if (do_execute) {
        g_menu_screen_open = 0;
        g_debug_state_packed &= 0xffffffff00000000ull;
    }

    g_menu_revision++;
    g_menu_guard = 0;

    if (progressed && dialog != 0)
        dialog_session_close(dialog);
    if (progressed && new_dialog != 0)
        dialog_session_close(new_dialog);

    if (do_execute) {
        uint32_t cmd = k_debug_commands[slot].cmd;
        uint32_t ok = (uint32_t)(debug_command_dispatch(cmd, value) != 0);

        if ((menu_guard_busy(1, &g_menu_guard) & 1) == 0) {
            g_debug_report_result = ok != 0 ? 1 : 2;
            g_debug_report_action = slot + DEBUG_ACTION_BASE;
            g_debug_report_rev++;
            g_menu_guard = 0;
        }

        if (ok == 0) {
            if ((menu_guard_busy(1, &g_menu_guard) & 1) == 0) {
                if (g_debug_open_rev == open_rev && g_menu_battle == 0
                    && g_menu_screen == DEBUG_SCREEN_CHAR) {
                    g_menu_screen_open = 1;
                    menu_text_set(g_debug_message, 0x60, 0x60,
                                  "SCREEN COULD NOT BE OPENED");
                    g_menu_revision++;
                }
                g_menu_guard = 0;
            }
        }
        return ok != 0;
    }

    return result_code != 0;
}
