/*
 * fonts — UI subsystem [libNexusUI69252.so]
 * Related menu entries (from embedded nexus-overlay-wire/v1): the font chooser
 * surfaces ("Select a font in the lobby." / font body selection, 233-entry wire,
 * see docs/feature_list.json).
 *
 * Reconstructed from Ghidra 11.3.2 arm64 pseudocode. Contents:
 *   - nexus_script_port_fonts_open()   — chooser open gate (host ready + scene
 *     identity + snapshot valid, stale widget-tree reset, "chooser_open" event)
 *   - fonts_chooser_frame()            — per-frame chooser render (scale-to-fit
 *     640x450 design, 6 font buttons, status line, reload debounce) @ 00171808
 *   - nexus_script_port_fonts_open()   — dlsym export thunk @ 001946b0
 *
 * The chooser widget tree itself is built by widgets.c (chooser_build: DEFAULT,
 * PUSIA BOLD, NICE BRAWL, GHOUL STARS, IMPACT, GENSHIN IMPACT buttons + title +
 * status labels); this file only opens it and renders it every frame.
 */

#define _GNU_SOURCE

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/* ---- script-port host service table (misc.c) ---- */
extern void  *g_host_ctx;        /* 0x1a7cc8 */
extern int  (*g_host_read)(void *ctx, uint64_t addr, void *out, uint32_t len);  /* 0x1a7ce8 */
extern int  (*g_host_ready)(void *ctx);                                 /* 0x1a7cf0 */
extern int  (*g_host_style)(void *ctx, const char *name);             /* 0x1a7cf8 */
extern void (*g_host_log)(void *ctx, const char *cat, const char *event,
                          uint32_t value);                             /* 0x1a7d00 */

extern uint64_t g_game_base;     /* 0x1a7cd0 */
extern uint64_t g_ui_scene_root; /* 0x1a7d28 — SC scene root node */
extern uint64_t g_ui_context;    /* 0x1a7d30 — UI widget context root */

extern int ui_scene_fonts_ready(void);            /* misc.c  @ 0016d910 */
extern int font_chooser_snapshot_read(void *out); /* misc.c  @ 00191df8 — 0x30-byte snapshot */

extern void widget_set_position(uint64_t widget, float x, float y);  /* misc.c @ 0016cf5c */
extern void widget_set_scale(uint64_t widget, float sx, float sy);   /* misc.c @ 0016cf00 */
extern void widget_set_frame(uint64_t widget, float x, float y,
                             float w, float h);                      /* misc.c @ 0017b5e8 */
extern void widget_set_font_size(uint64_t widget, float size);       /* misc.c @ 0016cf70 */
extern void widget_set_text(uint64_t widget, uint64_t text_obj);     /* misc.c @ 0016cec0 */
extern int  widget_child_link_valid(uint64_t node, uint64_t parent); /* misc.c @ 0016d5cc */
extern void ui_text_format(char *buf, size_t cap_a, size_t cap_b,
                           const char *fmt, const char *style,
                           const char *text);                        /* misc.c @ 00176a24 */
extern uint64_t ui_string_object(const char *text);                  /* misc.c @ 0017b858 */
extern void ui_label_render(float x, float y, uint64_t widget,
                            const char *text);                       /* misc.c @ 0017d6b0 */
extern int  ui_popover_styles_ready(void);                           /* widgets.c @ 0017d01c */
extern int  nexus_script_port_ui_reload_request(void);               /* renderer.c @ 0014d054 */

/* ---- rodata ---- */
extern const uint64_t FONT_SNAPSHOT_MAGIC;      /* 0x10f748 */
extern const char    g_font_label_fmt[];        /* 0x135833 — button label format */
extern const char    g_style_button_active[];   /* 0x1326cc */
extern const char    g_style_button_inactive[]; /* 0x134f22 */
extern const char    g_chooser_title_text[];    /* 0x135038 — "Select font" */
extern const char   *const g_font_names[6][3];  /* 0x19c080 — [i][0] = label */

/* ---- runtime layout globals (SC context) ---- */
extern float g_ui_density;   /* 0x1e53fc — design-density divisor */
extern float g_ui_center_x;  /* 0x1e5400 */
extern float g_ui_center_y;  /* 0x1e5404 */

/* ---- chooser widget state block (built by widgets.c), 0x648 bytes @ 0x281b08 ---- */
#define CHOOSER_BLOCK_SIZE 0x648u

/* 0x30-byte font chooser snapshot (reader: font_chooser_snapshot_read, misc.c) */
typedef struct {
    uint64_t magic;          /* +0x00 FONT_SNAPSHOT_MAGIC */
    int32_t  status;         /* +0x08 nonzero = snapshot valid */
    int32_t  engageable;     /* +0x0c fonts engageable (lobby state) */
    int32_t  selected;       /* +0x10 current selection, 0..5 */
    int32_t  reload_pending; /* +0x14 choice committed, game reload needed */
    uint32_t _r18;           /* +0x18 */
    int32_t  load_failed;    /* +0x1c last font failed to load */
    uint32_t _r20;           /* +0x20 */
    uint64_t revision;       /* +0x28 snapshot revision */
} font_chooser_snapshot_t;

typedef struct {
    uint64_t owner_context;   /* +0x000 0x281b08 — context the tree was built for */
    uint64_t root;            /* +0x008 0x281b10 */
    uint64_t panel;           /* +0x010 0x281b18 */
    uint64_t canvas;          /* +0x018 0x281b20 */
    uint64_t hint;            /* +0x0b0 0x281bb8 */
    uint64_t buttons[6];      /* +0x148 0x281c50, 0x98 stride */
    uint64_t title;           /* +0x4d8 0x281fe0 */
    uint64_t status_label;    /* +0x570 0x282078 */
    uint32_t built;           /* +0x608 0x282110 */
    uint32_t contract_failed; /* +0x60c 0x282114 */
    uint32_t open;            /* +0x610 0x282118 */
    font_chooser_snapshot_t snap_cache; /* +0x618 0x282120 */
} chooser_state_t;

extern chooser_state_t g_chooser_state;  /* 0x281b08 */

static uint32_t g_font_event_count;  /* 0x282b08 — script_port_font_ui events, 32 logged */
static uint64_t g_font_reload_rev;   /* 0x2844d8 — last revision a reload was queued for */
static uint64_t g_font_reload_frame; /* 0x2844e0 — frame of the last queued reload */

#define FONT_EVENT_LOG_CAP 0x20u
#define FONT_RELOAD_DEBOUNCE_FRAMES 500u
#define FONT_CHOOSER_DESIGN_W 640.0f
#define FONT_CHOOSER_DESIGN_H 450.0f

static void font_log_event(const char *event, uint32_t value)
{
    uint32_t prev = g_font_event_count;
    g_font_event_count = prev + 1;
    if (prev >= FONT_EVENT_LOG_CAP || g_host_log == NULL)
        return;
    g_host_log(g_host_ctx, "script_port_font_ui", event, value);
}

/*
 * nexus_script_port_fonts_open — open the font chooser.
 *
 * Gates: host channel present and ready, SC scene identity + fonts flag +
 * identity context scale (ui_scene_fonts_ready), and a valid font snapshot
 * (status != 0). On success the 0x648-byte chooser widget block is reset when
 * it belongs to a stale UI context, the open flag is raised and a
 * "chooser_open" event is emitted with the current selection.
 * Returns 1 when the chooser is open, 0 otherwise. @ 0016e4f8
 */
int nexus_script_port_fonts_open(void)
{
    font_chooser_snapshot_t snap;

    if (g_host_ctx == NULL || g_host_read == NULL)
        return 0;

    if (g_host_ready == NULL || !g_host_ready(g_host_ctx))
        return 0;
    if (!ui_scene_fonts_ready())
        return 0;

    memset(&snap, 0, sizeof snap);
    snap.magic = FONT_SNAPSHOT_MAGIC;
    if (font_chooser_snapshot_read(&snap) == 0)
        return 0;
    if (snap.status == 0)
        return 0;

    if (g_chooser_state.owner_context != 0
        && g_chooser_state.owner_context != g_ui_context)
        memset(&g_chooser_state, 0, CHOOSER_BLOCK_SIZE);

    g_chooser_state.open = 1;
    font_log_event("chooser_open", (uint32_t)snap.selected);
    return 1;
}

/*
 * chooser_park_and_reload — park the chooser tree off-screen and run the
 * game-reload debounce. Shared tail of both non-rendering frame paths.
 *
 * The tree is parked at (-9999, -9999) when it was built and still linked into
 * the live context. A game reload is queued when the snapshot says a choice is
 * committed (status + engageable + reload_pending), its revision differs from
 * the last one reloaded, and at least FONT_RELOAD_DEBOUNCE_FRAMES passed since
 * the previous request. @ LAB_00171bfc
 */
static void chooser_park_and_reload(const font_chooser_snapshot_t *snap,
                                    uint64_t frame)
{
    g_chooser_state.snap_cache = *snap;

    if (g_chooser_state.built != 0
        && widget_child_link_valid(g_chooser_state.root, g_ui_context))
        widget_set_position(g_chooser_state.root, -9999.0f, -9999.0f);

    if (snap->status == 0 || snap->engageable == 0 || snap->reload_pending == 0)
        return;
    if (snap->revision == g_font_reload_rev)
        return;
    if (frame >= g_font_reload_frame
        && frame - g_font_reload_frame < FONT_RELOAD_DEBOUNCE_FRAMES)
        return;

    g_font_reload_frame = frame;
    if (nexus_script_port_ui_reload_request() == 0)
        return;

    g_font_reload_rev = snap->revision;
    font_log_event("reload_queued", (uint32_t)snap->selected);
}

/*
 * chooser_render — the actual frame paint. Scales the tree so the 640x450
 * design fits the screen (never above 1:1), centers it on the SC anchor,
 * lays out the six font buttons in a 2x3 grid and updates the status line. @ LAB_00171994
 */
static void chooser_render(const font_chooser_snapshot_t *snap,
                           float width, float height)
{
    char label[0x60];

    if (!widget_child_link_valid(g_chooser_state.root, g_ui_context))
        return;

    float sw = width / g_ui_density;
    float sh = height / g_ui_density;
    float scale = fminf(fminf(sw / FONT_CHOOSER_DESIGN_W,
                              sh / FONT_CHOOSER_DESIGN_H), 1.0f);
    if (!isfinite(scale) || !(scale > 0.0f))
        return;

    widget_set_scale(g_chooser_state.root, scale, scale);
    widget_set_position(g_chooser_state.root,
                        (g_ui_center_x - width * 0.5f) / g_ui_density,
                        (g_ui_center_y - height * 0.5f) / g_ui_density);

    widget_set_frame(g_chooser_state.canvas, 0.0f, 0.0f, sw / scale, sh / scale);
    widget_set_font_size(g_chooser_state.canvas, 0.65f /* 0x3f266666 */);
    widget_set_frame(g_chooser_state.panel, 0.0f, 0.0f, 620.0f, 410.0f);
    widget_set_frame(g_chooser_state.hint, 270.0f, -46.0f, 48.0f, 42.0f);
    ui_label_render(-37.0f, -45.0f, g_chooser_state.title, g_chooser_title_text);

    for (uint32_t i = 0; i < 6; i++) {
        const char *style = (i == (uint32_t)snap->selected)
                                ? g_style_button_active
                                : g_style_button_inactive;
        ui_text_format(label, sizeof label, sizeof label, g_font_label_fmt,
                       style, g_font_names[i][0]);
        widget_set_text(g_chooser_state.buttons[i], ui_string_object(label));

        float slide = (i & 1) == 0 ? 150.0f : -150.0f;
        float y = fmaf((float)(i >> 1), 88.0f, -95.0f);
        widget_set_frame(g_chooser_state.buttons[i], slide, y, 130.0f, 62.0f);
    }

    const char *status_text = "Select a font in the lobby.";
    if (snap->engageable != 0) {
        status_text = snap->load_failed != 0
                          ? "Font could not load. Choose another or DEFAULT."
                          : "Selecting a font reloads the game.";
    }
    ui_label_render(-64.0f, 152.0f, g_chooser_state.status_label, status_text);
}

/*
 * fonts_chooser_frame — per-frame font chooser pump (renderer entry).
 *
 * Revalidates the scene (fonts-ready gate plus the scene "fonts swapped" byte
 * at scene_root+0x19c which must be clear), re-reads the 0x30 font snapshot and
 * resets the widget block when its owner context went stale. With a valid
 * snapshot (status, selection 0..5) the tree is rendered when already built;
 * otherwise the popup styles ("popup_generic" + "popover_text_left" +
 * ui_popover_styles_ready) gate the first build — a failed style contract
 * latches contract_failed and emits "chooser_contract_failed". Invalid or
 * closed states fall through to chooser_park_and_reload. @ 00171808
 */
void fonts_chooser_frame(float width, float height, uint64_t frame)
{
    font_chooser_snapshot_t snap;
    char swapped = 1;

    if (!ui_scene_fonts_ready())
        return;
    if (g_ui_scene_root + 0x19d < 0x1001)
        return;
    if (g_host_read(g_host_ctx, g_ui_scene_root + 0x19c, &swapped, 1) != 1)
        return;
    if (swapped != 0)
        return;

    memset(&snap, 0, sizeof snap);
    snap.magic = FONT_SNAPSHOT_MAGIC;
    if (font_chooser_snapshot_read(&snap) == 0)
        memset(&snap, 0, sizeof snap);

    if (g_chooser_state.owner_context != 0
        && g_chooser_state.owner_context != g_ui_context)
        memset(&g_chooser_state, 0, CHOOSER_BLOCK_SIZE);

    if (snap.status == 0 || (uint32_t)snap.selected > 5) {
        g_chooser_state.open = 0;
        chooser_park_and_reload(&snap, frame);
        return;
    }

    g_chooser_state.snap_cache = snap;

    if (g_chooser_state.open == 0) {
        chooser_park_and_reload(&snap, frame);
        return;
    }
    if (g_chooser_state.contract_failed != 0)
        return;

    if (g_chooser_state.built != 0) {
        chooser_render(&snap, width, height);
        return;
    }

    /* first-frame build contract: the popup styles must resolve */
    if (g_host_style(g_host_ctx, "popup_generic") == 0)
        return;
    if (g_host_style(g_host_ctx, "popover_text_left") == 0)
        return;
    if (ui_popover_styles_ready()) {
        chooser_render(&snap, width, height);
        return;
    }

    g_chooser_state.contract_failed = 1;
    font_log_event("chooser_contract_failed", 0);
}

/*
 * nexus_script_port_fonts_open — dlsym export thunk: forwards through the
 * GOT slot installed by the loader (export pointer table at 0x1a3a60). @ 001946b0
 */
void nexus_script_port_fonts_open__export(void)
{
    nexus_script_port_fonts_open();
}
