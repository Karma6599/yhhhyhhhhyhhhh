/*
 * activation_plus — UI subsystem [libNexusUI69252.so]
 * Related menu entries (from embedded nexus-overlay-wire/v1): the Nexus+ PAID
 * surfaces (menu.follow/xray/spin/esp/tracers/..., 233-entry wire, see
 * docs/feature_list.json).
 *
 * Reconstructed from Ghidra 11.3.2 arm64 pseudocode. Contents:
 *   - plus_key_input()                — activation key input/paste path: focus
 *     handoff with the clipboard bridge, 250ms input cooldown, 4-field
 *     clipboard paste parse (mode / key / user-id / status) @ 00162390
 *   - nexus_rich_plus_layout()        — build the Nexus+ tab: clear the row
 *     cells, install the 4 section templates, 9 form rows @ 0016bc94
 *   - nexus_rich_header_entitlement() — the "+" header entitlement flag @ 0016c2f8
 *   - nexus_rich_set_plus_state()     — validated 0x80-byte plus-state publish @ 001767dc
 *   - three dlsym export thunks @ 00194810/001948a0/001948b0
 */

#define _GNU_SOURCE

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

/* ---- script-port host service table (misc.c) ---- */
extern void  *g_host_ctx;                            /* 0x1a7cc8 */
extern int  (*g_host_ready)(void *ctx);              /* 0x1a7cf0 */
extern int   g_plus_service_installed;               /* 0x1a7d0c — entitlement service gate */
extern int   g_plus_state_latch;                     /* 0x1a7d08 — publish in-flight latch */

extern int ui_latch_test_and_set(int value, volatile int *latch); /* menu_engine.c @ 00193f80 */

static inline float bits_to_float(uint32_t bits)
{
    float f;
    memcpy(&f, &bits, sizeof f);
    return f;
}

/*
 * The published plus state mirror (0x80 bytes @ 0x1e5168). Other UI files
 * (renderer header, widgets) read this to badge PAID rows.
 */
extern uint64_t g_plus_state_published[16];          /* 0x1e5168 */

/* ---- misc.c builders ---- */
extern void plus_field_text_fill(char *buf, size_t cap);            /* misc.c @ 0016ba38 */
extern int  rich_form_row_build(float x, float y, float sy, float fs,
                                float w, void *view, uint32_t *cap,
                                const char *text, uint32_t color,
                                uint32_t font, int flag, void *ctx); /* misc.c @ 0016c388 */

/* ---- clipboard bridge (Java-side clipboard service, JNI-style vtable) ---- */
extern void  *g_clip_service;   /* 0x22f4c8 — service object */
extern uint64_t g_clip_arg;     /* 0x22f4d0 */
extern int      g_clip_valid;   /* 0.22f4e0 — listener slot installed */
extern uint64_t g_clip_ctx;     /* 0.22f4e8 — clipboard context tag */

/* vtable slots of the bridge objects (observed call offsets) */
#define BRIDGE_SLOT_RELEASE        0x28u   /* service: drop reattached reference */
#define BRIDGE_SLOT_ATTACH         0x30u   /* service: (out, flags) -> 0 / -2 / err */
#define BRIDGE_SLOT_EXC_CLEAR      0x88u   /* clip: clear pending exception */
#define BRIDGE_SLOT_KIND_CHECK     0x98u   /* clip: (kind=8) -> 0 ok */
#define BRIDGE_SLOT_GET_ITEM       0x390u  /* clip: (ctx, arg) -> item */
#define BRIDGE_SLOT_LEN            0x540u  /* clip: (sub) -> char length */
#define BRIDGE_SLOT_CHARS          0x548u  /* clip: (sub, isCopy) -> chars */
#define BRIDGE_SLOT_CHARS_RELEASE  0x550u  /* clip: (sub, chars) */
#define BRIDGE_SLOT_COERCE         0x558u  /* clip: (item) -> 4 text fields */
#define BRIDGE_SLOT_ITEM_AT        0x568u  /* clip: (item, index) -> sub */
#define BRIDGE_SLOT_LISTENER       0x468u  /* clip: (ctx) */
#define BRIDGE_SLOT_EXC_CHECK      0x720u  /* clip: exception pending? */
#define BRIDGE_SLOT_CLOSE          0xa0u   /* clip: (0) */

typedef long bridge_obj_t;

static long bridge_call0(bridge_obj_t *obj, uint32_t slot)
{
    return ((long (*)(bridge_obj_t *))(*(long **)obj)[slot / 8])(obj);
}

static long bridge_call1(bridge_obj_t *obj, uint32_t slot, long a)
{
    return ((long (*)(bridge_obj_t *, long))(*(long **)obj)[slot / 8])(obj, a);
}

static long bridge_call2(bridge_obj_t *obj, uint32_t slot, long a, long b)
{
    return ((long (*)(bridge_obj_t *, long, long))(*(long **)obj)[slot / 8])(obj, a, b);
}

static long bridge_call3(bridge_obj_t *obj, uint32_t slot, long a, long b, long c)
{
    return ((long (*)(bridge_obj_t *, long, long, long))(*(long **)obj)[slot / 8])(obj, a, b, c);
}

/* ---- rodata ---- */
extern const uint64_t PLUS_STATE_DEFAULT_HDR;      /* 0x10f8a0 — default record header */
extern const uint64_t PLUS_TMPL_A0, PLUS_TMPL_A1, PLUS_TMPL_A2;  /* 0x10f8a8 / 0x10fb30 / 0x10fb38 */
extern const uint64_t PLUS_TMPL_B0, PLUS_TMPL_B1, PLUS_TMPL_B2;  /* 0x10f808 / 0x10fbc0 / 0x10fbc8 */
extern const uint64_t PLUS_TMPL_C0, PLUS_TMPL_C1, PLUS_TMPL_C2;  /* 0x10f758 / 0x10fb40 / 0x10fb48 */
extern const uint64_t PLUS_TMPL_D0, PLUS_TMPL_D1, PLUS_TMPL_D2;  /* 0x10f780 / 0x10f9e0 / 0x10f9e8 */
extern const char g_plus_title_default[];          /* 0x1350a0 — mode 0 header */
extern const char g_plus_title_inactive[];         /* 0x139efe — mode -1 header */
extern const char g_plus_title_active[];           /* 0x139ee0 — mode 1 + user header */
extern const char g_plus_label_key[];              /* 0x138a84 */
extern const char g_plus_label_uid[];              /* 0x13bade */
extern const char g_plus_label_status[];           /* 0x13a9e5 */
extern const char g_plus_label_action_a[];         /* 0x13a49c */
extern const char g_plus_label_action_b[];         /* 0x138abd */
extern const char g_plus_label_action_c[];         /* 0x134a70 */

/* ---- input state ---- */
static int      g_plus_input_prev;   /* 0x25cb3c — previous "input engaged" flag */
static uint64_t g_plus_input_next;   /* 0x25cb20 — next allowed input time */
static uint64_t g_plus_input_start;  /* 0x25ca40 — activation start time */

#define PLUS_INPUT_COOLDOWN 0xfau    /* 250 time units between input polls */
#define PLUS_FIELD_MAX      0x80u    /* per clipboard field length cap */
#define PLUS_KEY_MAX        0x60u    /* activation key length cap */
#define PLUS_UID_FLOOR      0x591c9u /* 365001 — user-id range base */
#define PLUS_UID_MAX        365000   /* highest accepted user id */

/*
 * plus_state_t — the 0x80-byte self-describing Nexus+ state record pasted in
 * via the clipboard (4 fields) and published through nexus_rich_set_plus_state.
 */
typedef struct {
    uint32_t size;        /* +0x00 = 0x80 */
    int32_t  mode;        /* +0x04: 1 = plus on, 0 = off, -1 = revoked ("1-") */
    int32_t  status;      /* +0x08: 1 = busy, 2 = editing, 3 = accepted, 0 = other */
    uint32_t _pad0c;
    int64_t  user_id;     /* +0x10: -1 = none, else 0..365000 */
    char     key[0x68];   /* +0x18: printable-ASCII activation key */
} plus_state_t;

/*
 * plus_key_valid — shared key gate: NUL-terminated within 0x61 bytes and every
 * byte printable ASCII (0x20..0x7e).
 */
static bool plus_key_valid(const char *key)
{
    if (memchr(key, 0, PLUS_KEY_MAX + 1) == NULL)
        return false;
    for (const unsigned char *p = (const unsigned char *)key; *p != 0; p++)
        if (*p < 0x20 || *p > 0x7e)
            return false;
    return true;
}

/*
 * plus_clipboard_fields_read — pull the 4 text fields off the clipboard bridge.
 * Attaches (or reattaches) the clip object, coerces it to text and copies each
 * field into line[] (0x81 stride). Returns false (and sets *bad) on any bridge
 * contract violation; the clip object is closed by the caller on success.
 */
static bool plus_clipboard_fields_read(bridge_obj_t *clip, char lines[4][0x81])
{
    long item, sub;
    char *chars;
    int len;

    if (bridge_call1(clip, BRIDGE_SLOT_KIND_CHECK, 8) != 0)
        return false;
    item = bridge_call2(clip, BRIDGE_SLOT_GET_ITEM, (long)g_clip_ctx, (long)g_clip_arg);
    if (bridge_call0(clip, BRIDGE_SLOT_EXC_CHECK) != 0)
        return false;
    if (item == 0)
        return false;
    if (bridge_call1(clip, BRIDGE_SLOT_COERCE, item) != 4)
        return false;

    for (uint32_t i = 0; i < 4; i++) {
        sub = bridge_call2(clip, BRIDGE_SLOT_ITEM_AT, item, i);
        if (sub == 0)
            return false;
        if (bridge_call0(clip, BRIDGE_SLOT_EXC_CHECK) != 0)
            return false;
        len = (int)bridge_call1(clip, BRIDGE_SLOT_LEN, sub);
        if (len < 0 || PLUS_FIELD_MAX < (uint32_t)len)
            return false;
        chars = (char *)bridge_call2(clip, BRIDGE_SLOT_CHARS, sub, 0);
        if (chars == NULL)
            return false;
        memcpy(lines[i], chars, strlen(chars) + 1);
        bridge_call2(clip, BRIDGE_SLOT_CHARS_RELEASE, sub, (long)chars);
    }
    return true;
}

/*
 * plus_parse_paste — parse the 4 clipboard fields into a plus state:
 * field 0 "1"/"0"/"1-" -> mode 1/0/-1 (anything else -> -2, rejected later),
 * field 1 the activation key, field 2 a full strtoll user id, field 3 the
 * status word ("busy"/"editing"/"accepted"). Returns false on parse failure.
 */
static bool plus_parse_paste(plus_state_t *state, char lines[4][0x81])
{
    char *end = NULL;
    int *errno_ptr = __errno_location();
    uint16_t m16;

    memset(state, 0, sizeof *state);
    state->size = 0x80;

    memcpy(&m16, lines[0], sizeof m16);
    if (m16 == 0x0031)            /* "1" */
        state->mode = 1;
    else if (m16 == 0x0030)       /* "0" */
        state->mode = 0;
    else
        state->mode = -2;
    if (m16 == 0x312d && lines[0][2] == '\0')   /* "1-" */
        state->mode = -1;

    *errno_ptr = 0;
    state->user_id = strtoll(lines[2], &end, 10);
    if (*errno_ptr != 0 || lines[2][0] == '\0' || end == NULL || *end != '\0')
        return false;

    if (strlen(lines[1]) > PLUS_KEY_MAX)
        return false;
    memcpy(state->key, lines[1], strlen(lines[1]) + 1);

    if (memcmp(lines[3], "busy\0", 5) == 0)
        state->status = 1;
    else if (memcmp(lines[3], "editing\0", 8) == 0)
        state->status = 2;
    else if (memcmp(lines[3], "accepted\0", 9) == 0)
        state->status = 3;
    else
        state->status = 0;
    return true;
}

int nexus_rich_set_plus_state(int *state);   /* defined below @ 001767dc */

/*
 * plus_key_input — the Nexus+ activation input/paste path.
 *
 * active: whether the activation input is engaged (focus flag from the key
 * pump); now: timestamp/frame counter. On the inactive transition the
 * clipboard bridge performs a focus handoff (attach/reattach + listener). The
 * clipboard itself is polled at most every 250 time units: a valid paste is
 * exactly 4 fields — mode ("1"/"0"/"1-"), key (printable ASCII, <= 0x60),
 * user id (full strtoll parse) and status ("busy"/"editing"/"accepted") —
 * published via nexus_rich_set_plus_state. Any contract violation publishes
 * the default (revoked, no-user) state instead. @ 00162390
 */
void plus_key_input(int active, uint64_t now)
{
    char lines[4][0x81];
    plus_state_t state, def;
    bridge_obj_t *clip = NULL;
    bool attached_fresh = false;
    bool bad, exc, ok;
    int rc;

    if (active == 0 || g_plus_input_prev != 0) {
        if (active == 0 && g_plus_input_prev != 0
            && g_clip_service != NULL && g_clip_ctx != 0) {
            /* focus lost while the activation input was engaged */
            rc = (int)bridge_call2((bridge_obj_t *)g_clip_service,
                                   BRIDGE_SLOT_ATTACH, (long)&clip, 0x10006);
            if (rc == 0) {
                attached_fresh = true;
            } else if (rc == -2
                       && bridge_call2((bridge_obj_t *)g_clip_service,
                                       BRIDGE_SLOT_ATTACH, (long)&clip, 0) == 0) {
                attached_fresh = false;
            } else {
                goto state_update;
            }
            if (clip != NULL && g_clip_valid != 0) {
                bridge_call1(clip, BRIDGE_SLOT_LISTENER, (long)g_clip_ctx);
                if (bridge_call0(clip, BRIDGE_SLOT_EXC_CHECK) != 0)
                    bridge_call0(clip, BRIDGE_SLOT_EXC_CLEAR);
            }
            if (!attached_fresh)
                bridge_call0((bridge_obj_t *)g_clip_service, BRIDGE_SLOT_RELEASE);
        }
    } else {
        /* activation input just engaged: clear the cooldown, stamp the start */
        g_plus_input_next = 0;
        g_plus_input_start = now;
    }

state_update:
    g_plus_input_prev = active;
    if (now < g_plus_input_next)
        return;
    g_plus_input_next = now + PLUS_INPUT_COOLDOWN;

    /* default (revoked) state for every failure path: header template, no user */
    memset(&def, 0, sizeof def);
    *(uint64_t *)&def = PLUS_STATE_DEFAULT_HDR;
    def.user_id = -1;

    if (g_clip_service == NULL || g_clip_ctx == 0)
        goto publish_default;

    rc = (int)bridge_call2((bridge_obj_t *)g_clip_service,
                           BRIDGE_SLOT_ATTACH, (long)&clip, 0x10006);
    if (rc == 0) {
        attached_fresh = true;
    } else if (rc == -2
               && bridge_call2((bridge_obj_t *)g_clip_service,
                               BRIDGE_SLOT_ATTACH, (long)&clip, 0) == 0) {
        attached_fresh = false;
    } else {
        goto publish_default;
    }

    if (clip == NULL)
        goto publish_default;

    bad = !plus_clipboard_fields_read(clip, lines);
    exc = bridge_call0(clip, BRIDGE_SLOT_EXC_CHECK) != 0;
    ok = false;
    if (!exc && !bad)
        ok = plus_parse_paste(&state, lines)
             && nexus_rich_set_plus_state((int *)&state) == 1;

    if (exc)
        bridge_call0(clip, BRIDGE_SLOT_EXC_CLEAR);
    if (!ok)
        nexus_rich_set_plus_state((int *)&def);

    bridge_call1(clip, BRIDGE_SLOT_CLOSE, 0);
    if (!attached_fresh)
        bridge_call0((bridge_obj_t *)g_clip_service, BRIDGE_SLOT_RELEASE);
    return;

publish_default:
    nexus_rich_set_plus_state((int *)&def);
}

/*
 * plus_section_install — install one 0x29-byte rich-view section template:
 * three rodata qwords (+0x00/+0x10/+0x18), an optional mid flag at +0x0c, two
 * zeroed words at +0x20/+0x24 and the visible byte at +0x28.
 */
static void plus_section_install(uint32_t *view, uint32_t byte_off,
                                 uint64_t t0, uint64_t t1, uint64_t t2,
                                 uint32_t mid_flag)
{
    uint8_t *base = (uint8_t *)view + byte_off;

    *(uint64_t *)(base + 0x00) = t0;
    *(uint32_t *)(base + 0x0c) = mid_flag;
    *(uint64_t *)(base + 0x10) = t1;
    *(uint64_t *)(base + 0x18) = t2;
    *(uint32_t *)(base + 0x20) = 0;
    *(uint32_t *)(base + 0x24) = 0;
    *(uint8_t  *)(base + 0x28) = 1;
}

/*
 * nexus_rich_plus_layout — build the Nexus+ tab of the rich view.
 *
 * view: rich view object (magic 0x25b at +0, doubling as the row count);
 * state: the published 0x80 plus state; ctx: opaque renderer context.
 * Validates the state record (size/mode/status/user-id/key), clears the 2-byte
 * row cells for rows 0x2b..count at stride 0xb0, installs the four section
 * templates and builds the 9 form rows (header, key, labels, uid, status,
 * action buttons). Returns 1 when every row built, 4 otherwise. @ 0016bc94
 */
int nexus_rich_plus_layout(uint32_t *view, int *state, void *ctx)
{
    char field_a[0x80], field_b[0x80];
    const char *title;
    uint32_t cap = 0x2c;
    uint32_t mode, color;
    int64_t uid;
    int r;

    if (view == NULL)
        return 4;
    if (state == NULL || *(uint32_t *)state != 0x80)
        return 4;
    mode = (uint32_t)state[1];
    if (mode != 0xffffffffu && mode != 0 && mode != 1)   /* {-1, 0, 1} */
        return 4;
    if ((uint32_t)state[2] > 3)
        return 4;
    uid = *(int64_t *)(state + 4);
    if (!(uid == -1 || (uid >= 0 && uid <= PLUS_UID_MAX)))
        return 4;
    if (!plus_key_valid((const char *)(state + 6)))
        return 4;

    /* clear the 2-byte row cells (field at row*0xb0 + 0x3c), rows 0x2b.. */
    uint32_t rows = view[0];
    for (uint32_t row = 0x2b; row < rows; row++)
        *(uint16_t *)((uint8_t *)view + row * 0xb0 + 0x3c) = 0;

    view[0x110c] = 0;   /* 0x4430 */
    view[0x15dc] = 0;   /* 0x5770 */

    plus_section_install(view, 0x1da4, PLUS_TMPL_A0, PLUS_TMPL_A1, PLUS_TMPL_A2, 1);
    plus_section_install(view, 0x30e4, PLUS_TMPL_B0, PLUS_TMPL_B1, PLUS_TMPL_B2, 1);
    plus_section_install(view, 0x4424, PLUS_TMPL_C0, PLUS_TMPL_C1, PLUS_TMPL_C2, 0);
    plus_section_install(view, 0x5664, PLUS_TMPL_D0, PLUS_TMPL_D1, PLUS_TMPL_D2, 0);

    /* header text by mode: active-with-user / inactive / default */
    title = g_plus_title_default;
    if (mode != 0)
        title = g_plus_title_inactive;
    if (mode == 1 && uid >= 0 && uid != 0)
        title = g_plus_title_active;

    plus_field_text_fill(field_a, sizeof field_a);
    plus_field_text_fill(field_b, sizeof field_b);

    color = (int32_t)mode >= 0 ? 0xffd8dbe2u : 0xffffd45au;       /* near-white / gold */

    r = rich_form_row_build(-54.5f, 205.0f,
                            (int32_t)mode >= 0 ? 1.7304f : 1.4142f,  /* 0x3fdc28f6 / 0x3fb5c28f */
                            12.443f, 640.0f, view, &cap, title, color,
                            0x30, mode == 1, ctx);
    if (r == 0)
        return 4;
    r = rich_form_row_build(-41.0f, 379.0f, 1.0f, 14.6f, 670.0f, view, &cap,
                            field_a, 0xffd8dbe2u, 0x30, 0, ctx);
    if (r == 0)
        return 4;
    r = rich_form_row_build(-41.0f, 258.0f, 0.88f, 13.86f, 670.0f, view, &cap,
                            g_plus_label_key, 0xffd8dbe2u, 0x40, 0, ctx);
    if (r == 0)
        return 4;
    r = rich_form_row_build(-41.0f, 278.0f, 0.95f, 14.0f, 550.0f, view, &cap,
                            g_plus_label_uid, 0xffd8dbe2u, 0x30, 0, ctx);
    if (r == 0)
        return 4;
    r = rich_form_row_build(-45.3f, 318.0f, 0.88f, 13.86f, 312.0f, view, &cap,
                            field_b, 0xffffffffu, 0x24, 0, ctx);
    if (r == 0)
        return 4;
    r = rich_form_row_build(86.0f, 318.0f, 0.82f, 13.42f, 104.0f, view, &cap,
                            g_plus_label_status, 0xffffffffu, 0x10, 0, ctx);
    if (r == 0)
        return 4;
    r = rich_form_row_build(-61.3f, 402.3f, 1.0f, 13.2f, 176.0f, view, &cap,
                            g_plus_label_action_a, 0xffffffffu, 0x14, 0, ctx);
    if (r == 0)
        return 4;
    r = rich_form_row_build(55.0f, 394.3f, 0.82f, 13.42f, 176.0f, view, &cap,
                            g_plus_label_action_b, 0xffffffffu, 0x14, 0, ctx);
    if (r == 0)
        return 4;
    r = rich_form_row_build(55.0f, 410.3f, 0.96f, 13.96f, 176.0f, view, &cap,
                            g_plus_label_action_c, 0xffffffffu, 0xc, 0, ctx);
    return r != 0 ? 1 : 4;
}

/*
 * nexus_rich_header_entitlement — set the "+" header entitlement flag.
 *
 * Only on the rich header view (magic 0x25b, view id 3 at +0x1140, literal
 * "+" text at +0x116e). kind 1 propagates the entitlement flag from view+0x3c
 * into the header badge slot at +0x12ac; kind 0 clears it. Returns 1 for
 * kind in {0,1}, 4 otherwise. @ 0016c2f8
 */
int nexus_rich_header_entitlement(int *view, int kind)
{
    bool entitled = false;

    if (view == NULL || *(uint32_t *)view != 0x25b)
        return 4;
    if (view[0x450] != 3)
        return 4;
    if (strcmp((const char *)view + 0x116e, "+") != 0)
        return 4;

    if (kind == 1)
        entitled = (char)view[0xf] != 0;

    if ((uint32_t)kind + 1 >= 3)
        return 4;

    *(bool *)((uint8_t *)view + 0x12ac) = entitled;
    return 1;
}

/*
 * nexus_rich_set_plus_state — validate and publish a plus state record.
 *
 * Contract: size 0x80, mode in {-1,0,1}, status <= 3, user id -1 or
 * 0..365000, key printable-ASCII NUL-terminated within 0x60. Requires the
 * entitlement service gate and a ready host; a publish already in flight
 * returns 3. On success the record is mirrored to the 0x80-byte published
 * state. Returns 1 published / 0 no service / 3 busy / 4 invalid. @ 001767dc
 */
int nexus_rich_set_plus_state(int *state)
{
    uint64_t *src, *dst;

    if (state == NULL || *(uint32_t *)state != 0x80)
        return 4;
    {
        uint32_t mode = (uint32_t)state[1];
        if (mode != 0xffffffffu && mode != 0 && mode != 1)
            return 4;
        if ((uint32_t)state[2] > 3)
            return 4;
        int64_t uid = *(int64_t *)(state + 4);
        if (!(uid == -1 || (uid >= 0 && uid <= PLUS_UID_MAX)))
            return 4;
        if (!plus_key_valid((const char *)(state + 6)))
            return 4;
    }

    if (g_plus_service_installed == 0)
        return 0;
    if (g_host_ready == NULL || !g_host_ready(g_host_ctx))
        return 0;
    if (ui_latch_test_and_set(1, &g_plus_state_latch) & 1)
        return 3;
    g_plus_state_latch = 0;

    src = (uint64_t *)state;
    dst = g_plus_state_published;
    for (int i = 0; i < 16; i++)
        dst[i] = src[i];
    return 1;
}

/* ---- dlsym export thunks (GOT slots 0x1a3b10 / 0x1a3b58 / 0x1a3b60) ---- */

int nexus_rich_set_plus_state__export(int *state)          /* @ 00194810 */
{
    return nexus_rich_set_plus_state(state);
}

int nexus_rich_plus_layout__export(uint32_t *view, int *state, void *ctx)  /* @ 001948a0 */
{
    return nexus_rich_plus_layout(view, state, ctx);
}

int nexus_rich_header_entitlement__export(int *view, int kind)            /* @ 001948b0 */
{
    return nexus_rich_header_entitlement(view, kind);
}
