#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#define EDITOR_CMD_COUNT 20
#define EDITOR_ACTION_BASE 0x29020u
#define EDITOR_ACTION_CLOSE 0x29000u
#define EDITOR_ACTION_CONFIRM 0x29001u
#define EDITOR_ACTION_VALUE_PLACEMENT 0x29024u
#define EDITOR_ACTION_VALUE_REGION 0x29029u

typedef struct {
    uint32_t cmd;
    uint32_t _pad0[3];
    const char *name;
    uint32_t flags[4];
} editor_command_slot_t;

static const editor_command_slot_t k_editor_commands[EDITOR_CMD_COUNT] = {
    {  0, {0,0,0}, "OPEN MAP EDITOR",           {0,0,0,0x1} },
    {  1, {0,0,0}, "GRID",                      {0,0,0,0x2} },
    {  2, {0,0,0}, "MIRROR NEXT",               {0,0,0,0x3} },
    {  3, {0,0,0}, "MIRROR PREVIOUS",           {0,0,0,0x4} },
    {  4, {0,0,0}, "PLACEMENT MODE",            {0,0,2,0x5} },
    {  5, {0,0,0}, "UNDO",                      {0,0,0,0x6} },
    {  6, {0,0,0}, "REDO",                      {0,0,0,0x7} },
    {  7, {0,0,0}, "ERASER",                    {0,0,0,0x8} },
    {  8, {0,0,0}, "FILL ALL",                  {0,0,0,0x9} },
    {  9, {0,0,0}, "FILL REGION",               {0,0,4,0xa} },
    { 10, {0,0,0}, "ERASE ALL",                 {0,0,0,0xb} },
    { 11, {0,0,0}, "SAVE MAP",                  {0,0,0,0xc} },
    { 12, {0,0,0}, "CLEAR MAP",                 {0,0,0,0xd} },
    { 13, {0,0,0}, "REFRESH COUNTS",            {0,0,0,0xe} },
    { 14, {0,0,0}, "SAVE VALIDATION BYPASS",    {0,0,0,0xf} },
    { 15, {0,0,0}, "PLACEMENT ZONES BYPASS",    {0,0,0,0x10} },
    { 16, {0,0,0}, "FULL PALETTE",              {0,0,0,0x11} },
    { 17, {0,0,0}, "MAP MODIFIERS",             {0,0,0,0x12} },
    { 18, {0,0,0}, "CANCEL FILL",               {0,0,0,0x13} },
    { 19, {0,0,0}, "GO HOME",                   {0,0,0,0x7} },
};

#define EDITOR_SCREEN_CHAR 'e'

typedef struct {
    uint32_t magic;
    uint32_t size;
    uint64_t session;
    uint32_t count;
    uint32_t bitmap;
    uint32_t flags;
    uint32_t min_a;
    uint32_t min_b;
    uint32_t map_w;
    uint32_t map_h;
    uint32_t pad_2c;
    uint32_t range_lo;
    uint32_t range_hi;
    uint32_t pad_38;
    uint32_t pad_3c;
} editor_availability_t;

_Static_assert(sizeof(editor_availability_t) == 0x40, "editor_availability_t size");

extern uint64_t g_menu_guard;
extern int32_t  g_menu_owner_tid;
extern uint8_t  g_menu_screen_open;
extern char     g_menu_screen;
extern uint8_t  g_menu_flag_9f;
extern uint64_t g_menu_revision;

extern uint32_t g_editor_guard;
extern uint32_t g_editor_saved_screen;
extern uint64_t g_editor_state_packed;
extern uint32_t g_editor_phase;
extern uint32_t g_editor_slot;
extern uint64_t g_editor_session;
extern uint32_t g_editor_slot_bitmap;
extern int64_t  g_editor_open_rev;
extern uint64_t g_editor_last_pump_ms;
extern uint64_t g_editor_dialog;
extern uint32_t g_editor_rect[4];
extern char     g_editor_message[0x80];
extern char     g_editor_banner[0x100];
extern editor_availability_t g_editor_avail;
extern uint64_t g_editor_scroll_rev;

extern int menu_guard_busy(int op, uint64_t *guard);
extern int editor_guard_busy(int op, uint32_t *guard);
extern int menu_text_set(char *dst, int64_t reserved, size_t cap,
                         const char *fmt, ...);
extern int editor_availability_read(void *out, uint32_t size);
extern uint64_t dialog_session_create(void);
extern int dialog_value_open(uint64_t handle, const char *name,
                             const char *fmt, int a, int b);
extern int dialog_input_poll(uint64_t handle, int32_t *state,
                             uint8_t *buf, uint32_t cap);
extern void dialog_session_close(uint64_t handle);
extern int editor_value_parse(uint32_t cmd, const uint8_t *buf, uint32_t out[4]);
extern int editor_command_run(uint32_t cmd, uint32_t x1, uint32_t y1,
                              uint32_t x2, uint32_t y2);
extern void editor_scroll_bump(int op, uint64_t *rev);
extern void editor_close_notify(void);

#define EDITOR_PHASE_IDLE 0
#define EDITOR_PHASE_VALUE 1
#define EDITOR_PHASE_PENDING 2
#define EDITOR_PHASE_REVIEW 3
#define EDITOR_PHASE_DONE 4
#define EDITOR_PHASE_APPLY 5

uint64_t nexus_menu_editor_scroll_revision(void)
{
    if (g_menu_screen_open != 0 && g_menu_screen == EDITOR_SCREEN_CHAR)
        return g_editor_scroll_rev;
    return 0;
}

uint32_t nexus_menu_editor_open(int owner_tid)
{
    if ((menu_guard_busy(1, &g_menu_guard) & 1) != 0)
        return 3;

    uint32_t result = 4;
    if (owner_tid > 0 && g_menu_owner_tid == owner_tid
        && g_menu_screen_open != 0) {

        if (g_menu_screen != EDITOR_SCREEN_CHAR)
            g_editor_saved_screen = (uint32_t)(uint8_t)g_menu_screen;

        g_editor_state_packed = 0;
        g_editor_phase = 0;
        g_editor_last_pump_ms = 0;
        memset(g_editor_message, 0, sizeof g_editor_message);
        g_editor_open_rev++;
        g_editor_scroll_rev = 0;
        g_menu_flag_9f = 0;
        result = 1;
        g_menu_revision++;
        g_menu_screen = EDITOR_SCREEN_CHAR;
        editor_scroll_bump(1, &g_editor_scroll_rev);
    }

    g_menu_guard = 0;
    return result;
}

static int editor_cmd_available(uint32_t cmd)
{
    if (cmd > 0x13)
        return 0;
    if (cmd >= g_editor_avail.count)
        return 0;
    if ((g_editor_avail.bitmap >> (cmd & 0x1f) & 1u) == 0)
        return 0;
    return 1;
}

uint32_t nexus_menu_editor_action(uint32_t action_id, int owner_tid)
{
    if (action_id - 0x29200u < 0xfffffe00u)
        return 4;

    if ((menu_guard_busy(1, &g_menu_guard) & 1) != 0)
        return 3;

    if (owner_tid < 1 || g_menu_owner_tid != owner_tid
        || g_menu_screen_open == 0 || g_menu_screen != EDITOR_SCREEN_CHAR) {
        g_menu_guard = 0;
        return 4;
    }

    if (action_id == EDITOR_ACTION_CLOSE) {
        g_editor_open_rev++;
        if (g_editor_phase - 3u < 2u) {
            g_editor_slot_bitmap = 0;
            g_editor_scroll_rev = 0;
        } else {
            g_menu_screen = (char)(uint8_t)g_editor_saved_screen;
        }
        g_menu_flag_9f = 0;
        g_editor_phase = 0;
        g_menu_revision++;
        editor_scroll_bump(1, &g_editor_scroll_rev);
        g_menu_guard = 0;
        return 1;
    }

    if (g_editor_guard == 0) {
        if (action_id == EDITOR_ACTION_CONFIRM) {
            if (g_editor_phase != EDITOR_PHASE_DONE) {
                g_menu_guard = 0;
                return 4;
            }
            if ((g_editor_slot_bitmap & 1u) == 0) {
                g_menu_guard = 0;
                return 4;
            }
            g_editor_phase = EDITOR_PHASE_APPLY;
            menu_text_set(g_editor_message, 0x80, 0x80, "APPLYING...");
        } else {
            if (g_editor_phase != EDITOR_PHASE_IDLE) {
                g_menu_guard = 0;
                return 2;
            }

            uint32_t slot = action_id - EDITOR_ACTION_BASE;
            if (slot > 0x13) {
                g_menu_guard = 0;
                return 4;
            }
            if ((uint32_t)g_editor_state_packed == 0) {
                g_menu_guard = 0;
                return 4;
            }
            if ((g_editor_slot_bitmap >> (slot & 0x1f) & 1u) == 0) {
                g_menu_guard = 0;
                return 4;
            }

            uint32_t cmd = k_editor_commands[slot].cmd;
            if (!editor_cmd_available(cmd)) {
                g_menu_guard = 0;
                return 0;
            }

            g_editor_phase = EDITOR_PHASE_VALUE;
            g_editor_rect[0] = 0;
            g_editor_rect[1] = 0;
            g_editor_session = g_editor_avail.session;

            const char *message = "ENTER A VALUE";
            if (action_id != EDITOR_ACTION_VALUE_PLACEMENT
                && action_id != EDITOR_ACTION_VALUE_REGION) {
                if (cmd - 8u < 5u && (0x17u >> ((cmd - 8) & 0x1f) & 1u) != 0) {
                    g_editor_phase = EDITOR_PHASE_REVIEW;
                    message = "REVIEW ACTION";
                } else {
                    g_editor_phase = EDITOR_PHASE_REVIEW;
                    if (cmd != 0x13) {
                        g_editor_phase = EDITOR_PHASE_APPLY;
                        message = "APPLYING...";
                    }
                }
            }

            g_editor_slot = slot;
            menu_text_set(g_editor_message, 0x80, 0x80, "%s", message);
        }
        g_menu_revision++;
    }

    g_menu_guard = 0;
    return 2;
}

static uint32_t editor_supported(const editor_availability_t *av)
{
    if (!(av->magic == 1 && av->size == 0x40 && av->session != 0))
        return 0;
    if (!(av->count - 0x15u > 0xffffffebu && av->pad_3c < 4))
        return 0;
    if ((av->bitmap >> (av->count & 0x1f)) != 0)
        return 0;
    if (!(av->min_a > 1u && av->min_b > 4u && av->map_w > 128u && av->map_h > 128u))
        return 0;
    if (!(av->pad_2c < 2u && (av->flags & 0xfffe3fffu) == 0 && av->pad_38 < 2u))
        return 0;
    if (!(av->range_lo <= av->range_hi && av->range_hi < 0x4001u))
        return 0;
    return (av->pad_38 < 5u) ? 1u : 0u;
}

uint32_t nexus_menu_editor_pump(int owner_tid, uint64_t now_ms)
{
    if ((menu_guard_busy(1, &g_menu_guard) & 1) != 0)
        return 3;

    if (owner_tid < 1 || g_menu_owner_tid != owner_tid) {
        g_menu_guard = 0;
        return 4;
    }

    if (editor_guard_busy(1, &g_editor_guard) != 0) {
        g_menu_guard = 0;
        return 3;
    }

    uint64_t dialog = g_editor_dialog;
    int64_t open_rev = g_editor_open_rev;
    uint64_t session = g_editor_session;
    uint32_t slot = g_editor_slot;
    uint32_t phase = g_editor_phase;
    uint32_t rect[4] = { g_editor_rect[0], g_editor_rect[1],
                         g_editor_rect[2], g_editor_rect[3] };

    if (g_menu_screen_open == 0 || g_menu_screen != EDITOR_SCREEN_CHAR) {
        g_editor_open_rev++;
        g_editor_dialog = 0;
        g_editor_state_packed = (g_editor_state_packed & 0xffffffff00000000ull);
        g_editor_slot_bitmap = 0;
        g_menu_guard = 0;
        editor_close_notify();
        if (dialog != 0)
            dialog_session_close(dialog);
        return 1;
    }

    int refresh = 1;
    if (g_editor_phase == EDITOR_PHASE_IDLE && g_editor_last_pump_ms != 0
        && g_editor_last_pump_ms <= now_ms)
        refresh = (now_ms - g_editor_last_pump_ms > 0xf9);

    if (g_editor_dialog != 0 && g_editor_phase == EDITOR_PHASE_IDLE) {
        g_editor_dialog = 0;
        g_editor_state_packed &= 0xffffffffull;
        g_menu_guard = 0;
        editor_close_notify();
        dialog_session_close(dialog);
        return 1;
    }

    g_menu_guard = 0;
    editor_close_notify();

    if (!refresh)
        return 1;

    editor_availability_t avail;
    memset(&avail, 0, sizeof avail);
    int got = editor_availability_read(&avail, 0x40);

    uint32_t supported = 0;
    if (got != 0)
        supported = editor_supported(&avail);

    uint8_t input_buf[0x401];
    memset(input_buf, 0, sizeof input_buf);

    uint32_t result_code = 0;
    uint64_t new_dialog = 0;
    int progressed = 0;
    int execute = 0;
    int close_input = 1;

    if (phase == EDITOR_PHASE_IDLE) {
        result_code = 1;
        progressed = 0;
        execute = 0;
        close_input = 0;
    } else if (slot < 0x14 && supported != 0) {
        uint32_t cmd = k_editor_commands[slot].cmd;
        if (cmd >= 0x14) {
            result_code = 0;
            close_input = 1;
        } else if (cmd < avail.count
                   && (avail.bitmap >> (cmd & 0x1f) & 1u) != 0
                   && (cmd == 0x12 || (uint64_t)avail.session == session)) {
            progressed = 1;
            result_code = 1;
            close_input = 1;

            switch (phase) {
            case EDITOR_PHASE_VALUE:
                new_dialog = dialog_session_create();
                if (new_dialog == 0) {
                    result_code = 0;
                } else {
                    int is_region = (cmd == 9);
                    const char *prompt = is_region
                        ? "RECTANGLE x1,y1,x2,y2" : "PLACEMENT MODE 0-4";
                    uint32_t width = is_region ? 0x60 : 1;
                    result_code = (uint32_t)(dialog_value_open(
                        new_dialog, prompt, "", !is_region, width) != 0);
                }
                progressed = 0;
                execute = 0;
                close_input = result_code ^ 1;
                break;

            case EDITOR_PHASE_PENDING: {
                int32_t poll_state = 3;
                result_code = dialog_input_poll(dialog, &poll_state,
                                                input_buf, 0x401);
                progressed = 0;
                if (result_code == 0 || poll_state == 3) {
                    close_input = 1;
                } else if (poll_state == 2) {
                    result_code = 2;
                    close_input = 1;
                } else if (poll_state == 1) {
                    if (memchr(input_buf, 0, 0x401) == NULL
                        || editor_value_parse(cmd, input_buf, rect) == 0) {
                        result_code = 0;
                        close_input = 1;
                    } else {
                        int review = (cmd - 8u < 5u
                                      && (0x17u >> ((cmd - 8) & 0x1f) & 1u) != 0)
                                     || cmd == 0x13;
                        execute = !review;
                        progressed = 1;
                        result_code = 1;
                        close_input = 1;
                    }
                } else {
                    result_code = 0;
                    close_input = 0;
                }
                break;
            }

            case EDITOR_PHASE_REVIEW:
                break;

            case EDITOR_PHASE_APPLY:
                execute = 1;
                progressed = 1;
                result_code = 1;
                close_input = 1;
                break;

            default:
                result_code = 1;
                progressed = 0;
                execute = 0;
                close_input = 0;
                break;
            }
        } else {
            result_code = 0;
            close_input = 1;
        }
    } else {
        result_code = 0;
        close_input = 1;
    }

    if ((menu_guard_busy(1, &g_menu_guard) & 1) == 0) {
        if (g_menu_screen_open == 0 || g_menu_screen != EDITOR_SCREEN_CHAR
            || g_editor_open_rev != open_rev) {
            g_menu_guard = 0;
            if (new_dialog != 0)
                dialog_session_close(new_dialog);
            return 4;
        }

        g_editor_state_packed = (g_editor_state_packed & 0xffffffff00000000ull)
                              | supported;
        if (supported != 0)
            g_editor_avail = avail;

        g_editor_last_pump_ms = now_ms;

        if (phase == EDITOR_PHASE_IDLE) {
            g_editor_state_packed = (g_editor_state_packed
                                     & 0xffffffff00000000ull) | supported;
        } else if (progressed) {
            g_editor_rect[0] = rect[0];
            g_editor_rect[1] = rect[1];
            g_editor_rect[2] = rect[2];
            g_editor_rect[3] = rect[3];
            g_editor_dialog = 0;
            g_editor_slot_bitmap = 0;
            g_editor_state_packed = (4ull << 32) | supported;
            g_menu_flag_9f = 0;
            editor_scroll_bump(1, &g_editor_scroll_rev);

            if (k_editor_commands[slot].cmd == 9) {
                menu_text_set(g_editor_banner, 0x100, 0x100,
                              "RECTANGLE %d,%d TO %d,%d | MAP %dx%d | CONFIRM TO EDIT",
                              rect[0], rect[1], rect[2], rect[3],
                              avail.map_w, avail.map_h);
            } else {
                const char *warn = "UNSAVED EDITS ARE NOT SAVED";
                if (k_editor_commands[slot].cmd != 0x13)
                    warn = "CHANGES THE CURRENT MAP; NO AUTO-SAVE";
                menu_text_set(g_editor_banner, 0x100, 0x100, "%s | MAP %dx%d | %s",
                              k_editor_commands[slot].name,
                              avail.map_w, avail.map_h, warn);
            }
        } else if (close_input == 0) {
            if (phase < 3) {
                g_editor_state_packed = (2ull << 32) | supported;
                if (new_dialog != 0)
                    g_editor_dialog = new_dialog;
            }
        } else {
            const char *msg = "EDITOR OR INPUT CHANGED - SELECT ACTION AGAIN";
            if (result_code != 0)
                msg = "APPLYING...";
            if (result_code == 2)
                msg = "CANCELLED";
            g_editor_state_packed = (g_editor_state_packed
                                     & 0xffffffff00000000ull) | supported;
            g_editor_dialog = 0;
            menu_text_set(g_editor_message, 0x80, 0x80, "%s", msg);
        }

        uint32_t cmd = k_editor_commands[slot].cmd;
        if (execute && cmd < 0x14
            && ((1u << (cmd & 0x1f)) & 0xa0801u) != 0) {
            g_menu_screen_open = 0;
            g_editor_state_packed &= 0xffffffff00000000ull;
        }

        g_menu_revision++;
        g_menu_guard = 0;

        if (close_input && dialog != 0)
            dialog_session_close(dialog);
        if (close_input && new_dialog != 0)
            dialog_session_close(new_dialog);

        if (execute) {
            uint32_t ok = (uint32_t)(editor_command_run(
                cmd, rect[0], rect[1], rect[2], rect[3]) != 0);

            if ((menu_guard_busy(1, &g_menu_guard) & 1) == 0) {
                if (g_editor_open_rev == open_rev
                    && g_menu_screen == EDITOR_SCREEN_CHAR) {
                    g_editor_last_pump_ms = 0;
                    if (ok == 0) {
                        g_menu_screen_open = 1;
                        menu_text_set(g_editor_message, 0x80, 0x80,
                                      "ACTION COULD NOT BE APPLIED");
                    } else {
                        const char *msg;
                        if (cmd == 0x10) {
                            msg = "REOPEN THE EDITOR TO APPLY PALETTE";
                        } else if (cmd == 0x12) {
                            msg = "FILL CANCELLED; COMPLETED TILES KEPT";
                        } else {
                            msg = "FILL QUEUED; NO AUTO-SAVE";
                            if ((cmd & 0xfffffffEu) != 8u && cmd != 10)
                                msg = "APPLIED";
                        }
                        menu_text_set(g_editor_message, 0x80, 0x80, "%s", msg);
                    }
                    g_menu_revision++;
                }
                g_menu_guard = 0;
            }
            return ok != 0;
        }

        return result_code != 0;
    }

    if (new_dialog != 0)
        dialog_session_close(new_dialog);
    return 3;
}
