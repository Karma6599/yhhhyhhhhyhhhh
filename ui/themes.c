/*
 * themes — UI subsystem [libNexusUI69252.so]
 * Related menu entries (from embedded nexus-overlay-wire/v1): the theme /
 * background chooser surfaces ("CHOOSE A BACKGROUND", 233-entry wire, see
 * docs/feature_list.json).
 *
 * Reconstructed from Ghidra 11.3.2 arm64 pseudocode. Contents:
 *   - theme_preview_pump()             — per-entry preview state machine @ 00172aac
 *   - nexus_menu_theme_preview()       — 0x140 theme preview record fill @ 00186108
 *   - nexus_menu_theme_scroll_revision() @ 001862a4
 *   - nexus_menu_theme_preview_report() — status line publish @ 001862dc
 *   - nexus_menu_theme_open()          — open the background chooser @ 0018640c
 *   - nexus_menu_theme_pump()          — refresh/confirm/apply state machine @ 00186d58
 *   - nexus_menu_main_view()           — the main view dispatcher (all screens) @ 00189884
 *   - four dlsym export thunks @ 001946xx
 *
 * Theme data lives in the 0x2534-byte 'THEM' snapshot (8 slots x 0x128:
 * {id, flags, name[3] x 0x60} per slot) stored at 0x285408; the preview
 * object chain and the theme list fetch run through misc.c helpers.
 */

#define _GNU_SOURCE

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* ---- script-port host service table + scene (misc.c) ---- */
extern uint64_t g_ui_context;    /* 0x1a7d30 — UI widget context root */
extern int      ui_scene_fonts_ready(void);          /* misc.c @ 0016d910 */
extern void     widget_set_position(uint64_t widget, float x, float y); /* misc.c @ 0016cf5c */
extern int      ui_latch_test_and_set(int value, volatile int *latch);  /* menu_engine.c @ 00193f80 */
extern void     menu_scroll_revision_bump(int a, volatile uint32_t *rev); /* menu_engine.c @ 00194010 */

/* ---- misc.c theme helper fleet ---- */
extern int  theme_preview_orphaned(uint64_t preview, uint64_t ctx);   /* @ 00182b58 */
extern int  theme_list_fetch_tick(void *state);                      /* @ 00181d04 */
extern uint64_t theme_preview_create(void *state, uint32_t kind);    /* @ 00181ea0 */
extern int  theme_preview_attach(uint64_t ctx, uint64_t preview);    /* @ 001826ec */
extern int  theme_preview_linked(uint64_t preview, uint64_t ctx);    /* @ 00181b78 */
extern int  theme_preview_ready(uint64_t preview);                   /* @ 00182848 */
extern int  theme_snapshot_read(void *buf, uint32_t size, uint32_t page); /* @ 00191ff0 */
extern int  theme_page_parse(void *buf, uint32_t page);              /* @ 0018cb98 */
extern int  theme_apply(uint32_t kind, uint32_t a, uint32_t b);      /* @ 0019204c */
extern void ui_status_line_format(char *buf, size_t cap_a, size_t cap_b,
                                  const char *fmt, ...); /* @ 00183ac8 —
                                  checked vsnprintf wrapper (name-map signature) */
extern void theme_store_reset(void *buf, uint32_t size);             /* @ 00191920 */

/* ---- rodata ---- */
extern const uint64_t THEME_PREVIEW_HDR;     /* 0x10f7f0 — {1, 0x140} */
extern const uint64_t THEME_SNAPSHOT_HDR;    /* 0x10f900 — 'THEM' header */
extern const char    g_theme_status_fmt[];   /* 0x1343dc */
extern const uint8_t g_theme_slot_default[0x128];  /* 0x28793c — current slot template */
extern const uint8_t g_theme_slot_previous[0x128]; /* 0x287a64 — previous slot template */

/* ---- theme subsystem state ---- */
extern uint8_t  g_theme_status_line[0x60];   /* 0x287c6c */
extern uint8_t  g_theme_store[0x2534];       /* 0x285408 — the 'THEM' snapshot mirror */

static uint32_t g_theme_menu_id;             /* 0x284694 */
static uint8_t  g_theme_installed;           /* 0x28469c */
static uint8_t  g_theme_tag;                 /* 0x28469d — 't' when the theme view owns the wire */
static uint8_t  g_theme_closed;              /* 0x28469e */
static uint8_t  g_theme_report_pending;      /* 0x28469f */
static uint32_t g_theme_revision;            /* 0x2846a0 */
static volatile int g_theme_latch;           /* 0x284688 — subsystem re-entrancy latch */

static uint64_t g_theme_preview_obj;         /* 0x284650 */
static uint64_t g_theme_preview_ctx;         /* 0x284658 */
static uint64_t g_theme_preview_id;          /* 0x284660 */
static uint8_t  g_theme_preview_built;       /* 0x284668 */
static uint8_t  g_theme_preview_done;        /* 0x28466c */
static uint32_t g_theme_preview_retries;     /* 0x284670 low */
static uint32_t g_theme_preview_gen;         /* 0x284670 high */

extern uint32_t g_menu_scroll_revision;      /* 0x22d824 (menu_engine.c) */
extern uint32_t g_theme_selection_epoch;     /* 0x287c40 */
extern uint32_t g_theme_apply_state;         /* 0x287b90 — 2 = applied/confirmed */
extern uint32_t g_theme_page_flags;          /* 0x287b98 */
extern uint32_t g_theme_page_flags2;         /* 0x287b9c */
extern uint32_t g_theme_page_flag3;          /* 0x287ba0 */
extern uint64_t g_theme_page_word;           /* 0x287b94 — low: page, high: list-ok flag */
extern uint32_t g_theme_machine_state;       /* 0x287ba4 — 0 refresh / 1 loading / 2 confirm / 3 transient */
extern uint32_t g_theme_request_kind;        /* 0x287ba8 */
extern uint32_t g_theme_status_counter;      /* 0x287bac */
extern uint8_t  g_theme_status_flag;         /* 0x287bb4 */
extern uint32_t g_theme_apply_arg_a;         /* 0x287bb8 */
extern uint32_t g_theme_apply_arg_b;         /* 0x287bbc */
extern uint32_t g_theme_slot_epoch;          /* 0x287c44 */
extern uint32_t g_theme_generation;          /* 0x287c48 */
static uint32_t g_theme_confirm_gen;         /* 0x287c50 */
static uint64_t g_theme_refresh_frame;       /* 0x287c58 */
extern uint64_t g_theme_current_id;          /* 0x287c60 */
static uint32_t g_theme_reported_status;     /* 0x287c68 */
extern uint32_t g_theme_slot_flag_word;      /* 0x285424 */
extern uint32_t g_theme_slot_flag_mask;      /* 0x285438 */
extern uint32_t g_theme_prev_tag;            /* 0x28dce8+4 — last dispatched view tag */

#define THEME_SNAPSHOT_SIZE 0x2534u
#define THEME_SLOT_SIZE     0x128u
#define THEME_PREVIEW_SIZE  0x140u
#define THEME_PREVIEW_RETRY_CAP 0xb4u   /* 180 */
#define THEME_REFRESH_DEBOUNCE 999u
#define THEME_LATCH &g_theme_latch

int nexus_menu_theme_preview(int *rec, long size);            /* below */
bool nexus_menu_theme_preview_report(long theme_id, int status); /* below */

/*
 * theme_preview_pump — per-frame preview pump for one menu entry.
 *
 * entry: wire entry ({tag at +8, disabled at +10}); only the theme entry
 * ('t', enabled) with a ready scene runs the machine. Re-reads the 0x140
 * preview record, then keeps/creates/parks the preview object for the current
 * theme id: same id + context continues the fetch (theme_list_fetch_tick,
 * retry cap 180), a changed id rebuilds, and a broken context tears the state down.
 * Results are reported through nexus_menu_theme_preview_report. @ 00172aac
 */
static void theme_preview_pump(void *entry)
{
    uint8_t rec[THEME_PREVIEW_SIZE];
    uint8_t fetch_state[THEME_SLOT_SIZE];
    uint64_t ctx;
    uint32_t result;
    int r;

    if (*(char *)((uint8_t *)entry + 10) != 0 || *(char *)((uint8_t *)entry + 8) != 't')
        goto teardown_check;
    if (!ui_scene_fonts_ready())
        goto teardown_check;

    memset(rec, 0, sizeof rec);
    memset(fetch_state, 0, sizeof fetch_state);
    *(uint64_t *)rec = THEME_PREVIEW_HDR;
    if (nexus_menu_theme_preview((int *)rec, THEME_PREVIEW_SIZE) == 0)
        return;
    ctx = g_ui_context;

    if (g_ui_context == 0 || *(int32_t *)(rec + 8) == 0) {
        /* record invalid: park a still-linked preview, else tear down */
        if (g_theme_preview_obj != 0
            && theme_preview_orphaned(g_theme_preview_obj, g_theme_preview_ctx) == 0)
            goto park;
        result = 0;
        goto teardown;
    }

    if (g_theme_preview_id == *(uint64_t *)(rec + 0x10)
        && g_theme_preview_ctx == g_ui_context) {
        /* same theme + context: continue the pending fetch */
        if (g_theme_preview_done != 0)
            goto park;
        goto continue_fetch;
    }

    /* theme changed: rebuild unless the old preview is still live */
    if (g_theme_preview_obj == 0
        || theme_preview_orphaned(g_theme_preview_obj, g_theme_preview_ctx) != 0) {
        g_theme_preview_retries = 0;
        g_theme_preview_gen = 0;
        g_theme_preview_obj = 0;
        g_theme_preview_built = 0;
        g_theme_preview_ctx = ctx;
        g_theme_preview_id = *(uint64_t *)(rec + 0x10);
        goto continue_fetch;
    }

park:
    if (g_theme_preview_obj != 0
        && theme_preview_linked(g_theme_preview_obj, g_theme_preview_ctx) != 0)
        widget_set_position(g_theme_preview_obj, -9999.0f, -9999.0f);
    result = 0xffffffffu;
    goto report;

continue_fetch:
    if (g_theme_preview_id == 0) {
        g_theme_preview_done = 1;
        goto park;
    }
    r = theme_list_fetch_tick(fetch_state);
    if (r < 0) {
        g_theme_preview_done = 1;
        goto park;
    }
    if (r == 0) {
        /* still loading: retry counter, capped */
        if (++g_theme_preview_retries > THEME_PREVIEW_RETRY_CAP) {
            g_theme_preview_done = 1;
            goto park;
        }
        result = 0;
        goto report;
    }

    /* list ready: create the preview widget once, attach, validate */
    g_theme_preview_retries = 0;
    if (g_theme_preview_obj == 0) {
        g_theme_preview_obj = theme_preview_create(fetch_state,
                                                   *(uint32_t *)(rec + 0xc));
        if (g_theme_preview_obj == 0) {
            result = 0xffffffffu;
            g_theme_preview_done = 1;
            goto report;
        }
        if (theme_preview_attach(ctx, g_theme_preview_obj) == 0) {
            g_theme_preview_done = 1;
            goto park;
        }
    }
    if (theme_preview_linked(g_theme_preview_obj, ctx) == 0
        || (g_theme_preview_built == 0
            && theme_preview_ready(g_theme_preview_obj) == 0)) {
        g_theme_preview_built = 0;
        g_theme_preview_done = 1;
        goto park;
    }
    result = 1;
    g_theme_preview_built = 1;
    goto report;

teardown_check:
    if (g_theme_preview_obj == 0
        || theme_preview_orphaned(g_theme_preview_obj, g_theme_preview_ctx) != 0) {
        g_theme_preview_obj = 0;
        g_theme_preview_ctx = 0;
        g_theme_preview_id = 0;
        g_theme_preview_built = 0;
        g_theme_preview_done = 0;
        g_theme_preview_retries = 0;
        g_theme_preview_gen = 0;
    }
    return;

teardown:
    g_theme_preview_obj = 0;
    g_theme_preview_ctx = 0;
    g_theme_preview_id = 0;
    g_theme_preview_built = 0;
    g_theme_preview_done = 0;
    g_theme_preview_retries = 0;
    g_theme_preview_gen = 0;

report:
    if (*(int32_t *)(rec + 8) != 0)
        nexus_menu_theme_preview_report(*(uint64_t *)(rec + 0x10), (int)result);
}

/*
 * nexus_menu_theme_preview — fill the 0x140 theme preview record.
 *
 * rec: caller record ({version 1, size 0x140} header); returns 1 on fill, 0
 * on contract mismatch or when the subsystem latch is held. Only fills when
 * the theme view is active (installed, open, tag 't') and the current
 * selection epoch matches: valid flag, the slot flag bit
 * (flag_word bit 3 & flag_mask), the current theme id pair and the 0x128
 * current-slot template. @ 00186108
 */
int nexus_menu_theme_preview(int *rec, long size)
{
    if (rec == NULL || size != THEME_PREVIEW_SIZE)
        return 0;
    if (rec[0] != 1 || rec[1] != THEME_PREVIEW_SIZE)
        return 0;
    if (ui_latch_test_and_set(1, THEME_LATCH) & 1)
        return 0;

    memset(rec + 2, 0, THEME_PREVIEW_SIZE - 8);
    *(uint64_t *)rec = THEME_PREVIEW_HDR;

    /* active + applied + page flags + store count all non-zero */
    int active = 0;
    if (g_theme_installed != 0)
        active = (g_theme_apply_state == 2)
                 && g_theme_page_flags != 0 && g_theme_page_flags2 != 0
                 && g_theme_page_flag3 != 0
                 && *(uint32_t *)&g_theme_store[8] != 0;
    if (g_theme_closed != 0)
        active = 0;

    if (active == 1 && g_theme_tag == 't'
        && g_theme_selection_epoch == *(uint32_t *)&g_theme_store[0xc]) {
        rec[2] = 1;
        rec[3] = g_theme_slot_flag_mask
                 & (uint32_t)((g_theme_slot_flag_word << 28) >> 31);  /* bit 3 */
        *(uint64_t *)(rec + 4) = g_theme_current_id;
        memcpy(rec + 6, g_theme_slot_default, THEME_SLOT_SIZE);
    }

    g_theme_latch = 0;
    return 1;
}

/*
 * nexus_menu_theme_scroll_revision — the theme view's scroll revision
 * (published from the menu engine's counter at 0x22d824), 0 when inactive. @ 001862a4
 */
uint64_t nexus_menu_theme_scroll_revision(void)
{
    if (g_theme_installed != 0 && g_theme_closed == 0 && g_theme_tag == 't')
        return g_menu_scroll_revision;
    return 0;
}

/*
 * nexus_menu_theme_preview_report — publish the preview status line.
 *
 * theme_id: the record's theme id; status: -1 failed / 0 loading / 1 ready.
 * Dedupes on the last reported status and refreshes the 0x60 status line
 * ("LOADING PREVIEW" / "PREVIEW COULD NOT LOAD" /
 * "PREVIEW READY - CONFIRM TO APPLY"), clearing the report latch and bumping
 * the status counters. @ 001862dc
 */
bool nexus_menu_theme_preview_report(long theme_id, int status)
{
    const char *text;

    if (theme_id == 0)
        return false;
    if (!(status == -1 || status == 0 || status == 1))
        return false;
    if (ui_latch_test_and_set(1, THEME_LATCH) & 1)
        return false;

    bool changed = false;
    if (g_theme_installed != 0 && g_theme_closed == 0 && g_theme_tag == 't'
        && g_theme_apply_state == 2
        && theme_id == (long)g_theme_current_id
        && g_theme_reported_status != (uint32_t)status) {
        text = "LOADING PREVIEW";
        if (status != 0)
            text = "PREVIEW COULD NOT LOAD";
        if (status >= 1)
            text = "PREVIEW READY - CONFIRM TO APPLY";
        g_theme_reported_status = (uint32_t)status;
        ui_status_line_format((char *)g_theme_status_line, 0x60, 0x60,
                              g_theme_status_fmt, text);
        changed = true;
        g_theme_status_flag = 0;
        g_theme_report_pending = 0;
        g_theme_status_counter++;
        g_theme_revision++;
    }
    g_theme_latch = 0;
    return changed;
}

/*
 * nexus_menu_theme_open — open the background chooser.
 *
 * menu_id must be positive, match the registered theme menu id and the
 * subsystem must be installed and not closed. Preserves the page word,
 * bumps the generation, clears the 0x4fc8 theme store region, sets the 't'
 * tag, publishes "CHOOSE A BACKGROUND", raises the machine state and bumps
 * the scroll revision. Returns 1 opened / 3 busy / 4 refused. @ 0018640c
 */
int nexus_menu_theme_open(int menu_id)
{
    uint32_t page_word;
    uint32_t gen;

    if (ui_latch_test_and_set(1, THEME_LATCH) & 1)
        return 3;

    int opened = 4;
    if (menu_id > 0 && g_theme_menu_id == (uint32_t)menu_id && g_theme_closed == 0
        && g_theme_installed != 0) {
        page_word = g_theme_apply_state;
        if (g_theme_tag != 't')
            page_word = (uint32_t)g_theme_tag;
        gen = g_theme_generation + 1;
        memset(&g_theme_store, 0, 0x4fc8);   /* store + status region */
        g_theme_tag = 't';
        g_theme_apply_state = page_word;
        g_theme_generation = gen;
        ui_status_line_format((char *)g_theme_status_line, 0x60, 0x60,
                              g_theme_status_fmt, "CHOOSE A BACKGROUND");
        opened = 1;
        g_theme_machine_state = 1;
        g_theme_confirm_gen = g_theme_generation;
        g_theme_page_word = 0;
        menu_scroll_revision_bump(1, &g_menu_scroll_revision);
        g_theme_status_flag = 0;
        g_theme_report_pending = 0;
        g_theme_status_counter++;
        g_theme_revision++;
    }
    g_theme_latch = 0;
    return opened;
}

/* slot view: {id +0x00, flags +0x04 (bit 1 = usable), names +0x08/+0x68/+0xc8} */
typedef struct {
    int32_t  id;
    uint32_t flags;
    char     name_a[0x60];
    char     name_b[0x60];
    char     name_c[0x60];
} theme_slot_t;

/* the 0x2534 paged theme snapshot */
typedef struct {
    uint64_t hdr;          /* +0x00 THEME_SNAPSHOT_HDR */
    uint32_t count;        /* +0x08 total theme count */
    uint32_t epoch;        /* +0x0c */
    uint32_t page_total;   /* +0x10 — paging bound, stable across pages */
    uint32_t _pad14;
    uint32_t page_count;   /* +0x18 — slots present in this page */
    uint8_t  _pad1c[0x18];
    theme_slot_t slots[];  /* +0x34, stride 0x128 */
} theme_snapshot_t;

/*
 * theme_slot_matches — compare a snapshot slot against a template slot
 * (id + all three names + the usable flag bit 1).
 */
static bool theme_slot_matches(const theme_slot_t *slot, const theme_slot_t *tmpl)
{
    if (slot->id != tmpl->id)
        return false;
    if (strcmp(slot->name_a, tmpl->name_a) != 0
        || strcmp(slot->name_b, tmpl->name_b) != 0
        || strcmp(slot->name_c, tmpl->name_c) != 0)
        return false;
    return (slot->flags >> 1) & 1;
}

/*
 * theme_confirm_slots_present — walk the paged 8-slot snapshots and verify
 * both the current and the previous theme slots are present and usable in
 * the same page. Pages advance by 8 slots while ids stay ordered.
 */
static bool theme_confirm_slots_present(uint32_t page, const theme_slot_t *cur,
                                        const theme_slot_t *prev)
{
    theme_snapshot_t snap;
    bool found_cur = false;
    bool found_prev = prev->id == -2;   /* no previous theme -> trivially present */
    uint32_t last_id = 0;
    uint32_t idx = page;

    for (;;) {
        memset(&snap, 0, sizeof snap);
        snap.hdr = THEME_SNAPSHOT_HDR;
        if (theme_snapshot_read(&snap, THEME_SNAPSHOT_SIZE, idx) == 0)
            return false;
        if (theme_page_parse(&snap, idx) == 0 || snap.count == 0)
            return false;
        if (snap.epoch != g_theme_slot_epoch)
            return false;
        if (idx != 0 && snap.page_total != last_id)
            return false;

        for (uint32_t s = 0; s < snap.page_count; s++) {
            const theme_slot_t *slot = (const theme_slot_t *)&snap.slots[s];
            if (slot->id == cur->id
                && theme_slot_matches(slot, cur))
                found_cur = true;
            if (slot->id == prev->id
                && strcmp(slot->name_a, prev->name_a) == 0
                && strcmp(slot->name_b, prev->name_b) == 0
                && strcmp(slot->name_c, prev->name_c) == 0)
                found_prev = true;
        }
        if (found_cur && found_prev)
            return true;
        if (0xff7 < idx)
            return false;
        last_id = snap.page_total;
        idx += 8;
        if (idx >= snap.page_total)
            return false;
    }
}

/*
 * nexus_menu_theme_pump — the theme machine refresh/confirm pump.
 *
 * menu_id: registered theme menu id; frame: frame counter. States
 * (g_theme_machine_state): 0 = wait for refresh (999-frame debounce),
 * 1 = loading, 2 = confirm/apply, 3 = transient. The confirm path re-reads
 * the 0x2534 snapshot for the current page, validates the slot epoch and
 * verifies the current + previous theme slots, then applies via theme_apply.
 * The refreshed snapshot is mirrored to the theme store; status lines:
 * "THEME LIST IS NOT AVAILABLE" / "APPLIED" / "NOT APPLIED - REFRESH OR
 * SELECT AGAIN". Kinds {1, 2, 6} trigger the auto-reload chain. Returns
 * 1 ok / 3 busy / 4 refused. @ 00186d58
 */
int nexus_menu_theme_pump(int menu_id, uint64_t frame)
{
    theme_snapshot_t snap;
    theme_slot_t cur, prev;
    uint32_t page, kind, arg_a, arg_b;
    uint32_t gen;
    bool confirm, applied, list_ok;
    uint32_t ret;
    int r;

    if (ui_latch_test_and_set(1, THEME_LATCH) & 1)
        return 3;

    if (menu_id < 1 || g_theme_menu_id != (uint32_t)menu_id) {
        g_theme_latch = 0;
        return 4;
    }
    if (g_theme_installed == 0 || g_theme_closed != 0 || g_theme_tag != 't') {
        g_theme_machine_state = 0;
        g_theme_page_word &= 0xffffffffu;
        g_theme_generation++;
        g_theme_latch = 0;
        return 1;
    }

    if (g_theme_machine_state == 0) {
        if (frame < g_theme_refresh_frame
            || frame - g_theme_refresh_frame > THEME_REFRESH_DEBOUNCE) {
            confirm = false;
            gen = g_theme_generation;
            goto run;
        }
        g_theme_latch = 0;
        return 1;
    }
    if (g_theme_machine_state == 3) {
        g_theme_latch = 0;
        return 3;
    }

    confirm = g_theme_machine_state == 2;
    gen = g_theme_confirm_gen;

run:
    g_theme_confirm_gen = gen;
    memcpy(&cur, g_theme_slot_default, sizeof cur);
    memcpy(&prev, g_theme_slot_previous, sizeof prev);
    page = (uint32_t)g_theme_page_word;
    kind = g_theme_request_kind;
    arg_a = g_theme_apply_arg_a;
    arg_b = g_theme_apply_arg_b;
    g_theme_machine_state = 3;
    g_theme_latch = 0;

    applied = false;
    if (confirm) {
        memset(&snap, 0, sizeof snap);
        snap.hdr = THEME_SNAPSHOT_HDR;
        if (theme_snapshot_read(&snap, THEME_SNAPSHOT_SIZE, page) != 0) {
            r = theme_page_parse(&snap, page);
            if (r != 0 && snap.count != 0 && snap.epoch == g_theme_slot_epoch) {
                if (kind == 1)
                    applied = theme_confirm_slots_present(page, &cur, &prev);
                else
                    applied = true;
                if (applied)
                    applied = theme_apply(kind, arg_a, arg_b) != 0;
            }
        }
    } else {
        applied = true;
    }

    /* re-read the snapshot to decide list availability */
    memset(&snap, 0, sizeof snap);
    snap.hdr = THEME_SNAPSHOT_HDR;
    list_ok = theme_snapshot_read(&snap, THEME_SNAPSHOT_SIZE, page) != 0
              && theme_page_parse(&snap, page) != 0;

    if (ui_latch_test_and_set(1, THEME_LATCH) & 1)
        return 3;

    ret = 4;
    if (g_theme_menu_id == (uint32_t)menu_id && g_theme_installed != 0
        && g_theme_closed == 0 && g_theme_tag == 't'
        && gen == g_theme_generation) {
        g_theme_machine_state = 0;
        g_theme_page_word = ((uint64_t)list_ok << 32)
                            | (g_theme_page_word & 0xffffffffu);
        g_theme_refresh_frame = frame;

        if (!list_ok) {
            memset(&g_theme_store, 0, THEME_SNAPSHOT_SIZE);
            if (!confirm)
                ui_status_line_format((char *)g_theme_status_line, 0x60, 0x60,
                                      g_theme_status_fmt,
                                      "THEME LIST IS NOT AVAILABLE");
            else
                goto applied_status;
        } else {
            memcpy(&g_theme_store, &snap, THEME_SNAPSHOT_SIZE);
            if (confirm) {
applied_status:
                ui_status_line_format((char *)g_theme_status_line, 0x60, 0x60,
                                      g_theme_status_fmt,
                                      applied ? "APPLIED"
                                              : "NOT APPLIED - REFRESH OR SELECT AGAIN");
                if (applied && kind < 7 && ((1u << kind) & 0x46u) != 0) {
                    ret = 1;
                    g_theme_apply_state = 0;
                    g_theme_page_flags = 0;
                    g_theme_page_flag3 = 0;
                    menu_scroll_revision_bump(1, &g_menu_scroll_revision);
                    if (g_theme_page_word != 0) {
                        g_theme_machine_state = 1;
                        g_theme_page_word = 0;
                        g_theme_confirm_gen = g_theme_generation;
                        menu_scroll_revision_bump(1, &g_menu_scroll_revision);
                        g_theme_status_flag = 0;
                        g_theme_report_pending = 0;
                        g_theme_status_counter++;
                        g_theme_revision++;
                        goto done;
                    }
                }
            }
        }
        g_theme_report_pending = 0;
        g_theme_status_flag = 0;
        g_theme_revision++;
        ret = applied ? 1 : 4;
        g_theme_status_counter++;
    } else {
        ret = 4;
        g_theme_machine_state = 0;
    }

done:
    g_theme_latch = 0;
    return ret;
}


/* themes chain chunk 1: covers raw lines 703-1150 (main_view) */
#define _GNU_SOURCE

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* ===== nexus_menu_main_view: the rich wire-view builder (raw 703-3597) ===== */

/*
 * View record layout (nexus-overlay-wire/v1), 0x690 bytes total. The wire
 * entry header seen by theme_preview_pump is the same record shape
 * ({tag at +8, disabled at +0xa}). Every dispatch zeroes the row area
 * (memset 0x684 from +0xc = pad dword + all 26 row slots), points every
 * content-row text slot at the shared empty string, fills 20 content rows
 * (row_count starts at 0x14), then appends the shared 6-entry bottom nav
 * rail (row_count ends at 0x1a).
 *
 * Row slots are 0x40 bytes, back to back from +0x10. The 0x18 bytes at row
 * +0x20 hold the menu row state built by menu_row_state_init (widgets.c)
 * for feature rows; the per-tag cases also poke it field by field (font at
 * +0x20, selected mirror at +0x28, disabled at +0x30).
 */
typedef struct {
    uint64_t id;          /* +0x00 — wire event id (view-specific base + row) */
    char    *text_a;      /* +0x08 — title */
    char    *text_b;      /* +0x10 — subtitle / status text */
    float    x;           /* +0x18 — grid column position */
    float    y;           /* +0x1c — grid row position */
    uint32_t font;        /* +0x20 — 0x18 (row state +0x00) */
    uint32_t enabled_bit; /* +0x24 — row state +0x04, feature enable bit */
    uint32_t selected;    /* +0x28 — selected u32 (row state +0x08) */
    uint32_t feature_id;  /* +0x2c — feature id / config value (row state +0x0c) */
    uint32_t state_10;    /* +0x30 — row state +0x10: 1 plain / 3 locked for
                             feature rows; the plain grid cases store !enabled */
    uint8_t  enabled;     /* +0x34 — enabled byte (row state +0x14) */
    uint8_t  _pad35[3];   /* +0x35 */
    uint8_t  kind;        /* +0x38 — row kind: 1 header/nav, 5 section, 2/4 toggles */
    uint8_t  selected_b;  /* +0x39 — selected mirror, u8 */
    uint8_t  _pad3a[2];   /* +0x3a */
    uint32_t word_3c;     /* +0x3c — always 0 */
} menu_view_row_t;        /* 0x40 */

typedef struct {
    uint32_t        rev;       /* +0x00 — theme revision (+ per-view counters) */
    uint32_t        row_count; /* +0x04 — 0x14 content rows, 0x1a after the nav rail */
    uint8_t         tag;       /* +0x08 — owning view tag */
    uint8_t         installed; /* +0x09 — subsystem installed echo */
    uint8_t         disabled;  /* +0x0a — whole-view disabled (0 while open) */
    uint8_t         page;      /* +0x0b — scroll page, low byte */
    uint32_t        _pad0c;    /* +0x0c — zeroed with the row area */
    menu_view_row_t rows[26];  /* +0x10 — 20 content rows + 6 nav rail rows */
} menu_view_record_t;          /* 0x690 */

/* 0x18-byte menu row state built by menu_row_state_init (widgets.c) from a
 * 0x48-stride feature entry; copied verbatim into the wire row at +0x20. */
typedef struct {
    uint32_t font;        /* +0x00 — 0x18 */
    uint32_t enabled_bit; /* +0x04 — feature enable bit */
    uint32_t word_08;     /* +0x08 — always 0 */
    uint32_t feature_id;  /* +0x0c — feature mask / id */
    uint32_t state_10;    /* +0x10 — 1 plain / 3 locked */
    uint8_t  parent_flag; /* +0x14 — 1 when the entry was remapped to a parent */
    uint8_t  byte_15;     /* +0x15 */
    uint8_t  _pad16[2];   /* +0x16 */
} menu_row_state_t;

/* bottom nav rail entry — {u16 feature-entry index, u16 tag, label} — 0x10 stride */
typedef struct {
    uint16_t entry;
    uint16_t tag;
    uint16_t _pad04[2];
    char    *label;
} nav_rail_entry_t;

/* battle-server chooser state — 0x918 record refilled through
 * theme_store_reset by nexus_menu_main_count (menu_engine.c). */
typedef struct {
    uint32_t marker;        /* +0x00 — 0x918 request marker (theme_store_reset) */
    uint32_t region_count;  /* +0x04 — >0x20 (table capacity) resets to 0 */
    uint32_t list_count;    /* +0x08 — added to the view revision */
    uint8_t  rows_enabled;  /* +0x0c — enables the REFRESH PING + region rows */
    uint8_t  _pad0d[3];
    int32_t  selected_id;   /* +0x10 — selected region id, negative = automatic */
    uint8_t  _pad14[4];
    struct {
        uint32_t id;        /* +0x00 */
        int32_t  ping_ms;   /* +0x04 — <0 = not measured */
        char     name[0x40];/* +0x08 */
    } regions[32];          /* +0x18, 0x48 stride */
} server_view_state_t;

/* ---- main view externs (shared by the per-tag case chunks) ---- */

extern const char g_empty_text[];            /* 0x134f22 — shared "" rodata for text slots */
extern const uint8_t *g_feature_entry_table; /* 0x1a36f0 — GOT: feature entry table base
                                                (0x48 stride: name +0, bit index +0x10,
                                                kind byte +0x44, flag +0x47) */
extern const nav_rail_entry_t *g_nav_rail_table; /* 0x1a3720 — GOT: 6 bottom-rail entries */
extern server_view_state_t g_server_view_state;  /* 0x284af0 — battle-server chooser state */
extern char g_server_ping_text[32][0x50];     /* 0x28dd10 — per-region ping status buffers */

extern int  nexus_menu_main_count(void);      /* menu_engine.c @ 0018975c — content row count
                                                for the active tag (refills g_server_view_state) */
extern void menu_row_state_init(menu_row_state_t *state, const uint8_t *entry); /* widgets.c @ 00184e28 */
extern void menu_features_refresh(void);      /* external @ 00185d10 — not in dump */

/* ---- per-tag case helpers (bodies across chunks 2-5) ---- */

static void main_view_case_q(menu_view_record_t *rec, uint32_t page);       /* 'Q' raw 791-1198 */
static void main_view_case_generic(menu_view_record_t *rec, uint32_t page); /* 'C','O','R','U','V' raw 1199-1701 */
static void main_view_case_d(menu_view_record_t *rec, uint32_t page);       /* 'd' raw 1702-2098 */
static void main_view_case_e(menu_view_record_t *rec, uint32_t page);       /* 'e' raw 2099-2546 */
static void main_view_case_p(menu_view_record_t *rec, uint32_t page);       /* 'p' raw 2547-2944 */
static void main_view_case_t(menu_view_record_t *rec, uint32_t page);       /* 't' raw 2945-3582 */
static void main_view_append_nav_rail(menu_view_record_t *rec, bool highlight); /* raw 1174-1197 shared */

/*
 * nexus_menu_main_view — the rich view dispatcher for the 0x690 wire record.
 *
 * rec: view record to fill; page: scroll page (reset to 0 when the tag
 * changes); menu_id: registered menu id. Returns 4 for a null record,
 * unknown menu, closed view, or a tag outside bitmap 0x22006000cd001
 * ('C','O','Q','R','U','V','d','e','p','t'); 3 while the subsystem latch is
 * held; 1 after a case filled the record. The switch dedicates 'Q','d','e',
 * 'p','t'; the other five bitmap tags share the generic feature-list case.
 * @ 00189884
 */
int nexus_menu_main_view(menu_view_record_t *rec, uint32_t page, int menu_id)
{
    uint32_t tag;
    int ret;

    if (rec == NULL)
        return 4;
    if (ui_latch_test_and_set(1, THEME_LATCH) & 1)
        return 3;

    ret = 4;
    if (menu_id > 0 && g_theme_menu_id == (uint32_t)menu_id && g_theme_closed == 0) {
        tag = g_theme_tag;
        /* rich-view tag bitmap, bits = tag - 'C' (the raw masks the shift
         * count with 0x3f — redundant after the 0x31 bound check) */
        if (tag - 'C' <= 0x31 && ((1ull << (tag - 'C')) & 0x22006000cd001ull) != 0) {
            if (g_theme_prev_tag != tag) {
                page = 0;              /* screen changed: restart at page 0 */
                g_theme_prev_tag = tag;
            }
            switch (tag) {
            case 'Q':
                main_view_case_q(rec, page);
                break;
            case 'd':
                main_view_case_d(rec, page);
                break;
            case 'e':
                main_view_case_e(rec, page);
                break;
            case 'p':
                main_view_case_p(rec, page);
                break;
            case 't':
                main_view_case_t(rec, page);
                break;
            default:
                /* 'C','O','R','U','V' — the bitmap tags with no dedicated
                 * case in the raw switch */
                main_view_case_generic(rec, page);
                break;
            }
            ret = 1;
        }
    }
    g_theme_latch = 0;
    return ret;
}

/* themes chain chunk 2: covers raw lines 1151-1600 (main_view 'Q' case + nav rail) */
#define _GNU_SOURCE

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* continues the themes_core.c + themes_c01.c translation unit: view record
 * typedefs, the per-tag helper prototypes and the shared externs live there */

extern const char g_section_header_text[];  /* 0x137c9c — shared header-row text */

/* ui_status_line_format — checked vsnprintf wrapper (name-map signature
 * "(buf, cap_a, cap_b, fmt, ...)"); the "%d MS - %s" region line below
 * passes two conversions whose register varargs Ghidra dropped at the
 * call site. */

/*
 * main_view_case_q — build the 'Q' battle-server chooser screen.
 *
 * Refills the 0x918 server chooser state (theme_store_reset), clamps the
 * region count to the 32-slot table, then lays out the 5x4 grid: row 0 the
 * "BATTLE SERVER SETTINGS" header (event 0x24001), row 1 "AUTOMATIC"
 * (0x24002, selected while the chosen id is negative), row 2 "REFRESH PING"
 * (0x24003, subtitle flips to "MEASURE AVAILABLE REGIONS" once any region
 * is loaded) and rows 3+ the region cells (event 0x2400d+i) with per-region
 * "NOT MEASURED - %s" / "%d MS - %s" status buffers at 0x28dd10. Cells sit
 * at x = (i%5)*175-440, y = (i/5)*110+178, font 0x18; a page exposes 5
 * grid rows, revealed from page*5 while i < count+3.
 * Raw body 791-1198 of nexus_menu_main_view (case 0x51).
 */
static void main_view_case_q(menu_view_record_t *rec, uint32_t page)
{
    menu_view_row_t *row;
    char *status;
    char *ping_text;
    uint32_t count, first, i, r, sel;
    uint8_t enable;

    /* refill the chooser state through the script-port slot */
    theme_store_reset(&g_server_view_state, 0x918);
    if (0x20u < g_server_view_state.region_count)   /* 32-slot table */
        g_server_view_state.region_count = 0;
    count = g_server_view_state.region_count;

    /* 5 region rows per page; out-of-range pages restart at 0 */
    if (((count + 7) & 0xff) / 5 <= page)
        page = 0;

    rec->rev = g_server_view_state.list_count + g_theme_revision;
    rec->row_count = 0x14;
    rec->tag = 'Q';
    rec->installed = g_theme_installed;
    rec->disabled = 0;
    rec->page = (uint8_t)page;
    memset(&rec->_pad0c, 0, 0x684);   /* pad dword + all 26 row slots */
    for (r = 0; r < 0x14; r++) {      /* empty text in the content rows */
        rec->rows[r].text_a = (char *)g_empty_text;
        rec->rows[r].text_b = (char *)g_empty_text;
    }

    first = page * 5;                  /* first visible grid row */
    if (first < count + 3 && first < 0xffffffecu) {
        for (i = first; i < count + 3 && i < first + 0x14; i++) {
            r = i % 20;                /* wraps into the 20 row slots */
            row = &rec->rows[r];
            sel = (uint32_t)g_server_view_state.selected_id;

            row->id = 0;
            row->text_a = NULL;
            row->text_b = NULL;
            row->y = (float)(i / 5) * 110.0f + 178.0f;  /* 0x42dc0000, 0x43320000 */
            row->x = (float)(i % 5) * 175.0f - 440.0f;  /* 0x432f0000, 0xc3dc0000 */
            row->font = 0x18;
            row->enabled_bit = 0;
            row->selected = 0;
            row->feature_id = 0;
            row->state_10 = 0;
            row->enabled = 1;
            row->kind = 0;
            row->selected_b = 0;
            row->word_3c = 0;

            if (i == 2) {              /* REFRESH PING */
                row->id = 0x24003;
                row->text_a = "REFRESH PING";
                status = "LOAD AVAILABLE REGIONS";
                if (count != 0)
                    status = "MEASURE AVAILABLE REGIONS";
                row->text_b = status;
                enable = g_server_view_state.rows_enabled;
            } else if (i == 1) {       /* AUTOMATIC */
                row->id = 0x24002;
                row->text_a = "AUTOMATIC";
                status = "SELECTED - BEST CONNECTION";
                if (-1 < (int32_t)sel)
                    status = "RESTORE GAME SERVER CHOICE";
                enable = 1;
                row->selected = sel >> 31;          /* automatic while id < 0 */
                row->selected_b = (uint8_t)(sel >> 31);
                row->text_b = status;
            } else if (i != 0) {       /* region cell i-3 */
                uint32_t idx = i - 3;
                uint32_t rid = g_server_view_state.regions[idx].id;
                int32_t ping = g_server_view_state.regions[idx].ping_ms;

                row->text_a = g_server_view_state.regions[idx].name;
                row->id = i + 0x2400d;
                ping_text = (char *)&g_server_ping_text[idx];  /* 0x28dd10 + idx*0x50 */
                status = "SELECTED";
                if (rid != sel)
                    status = "TAP TO SELECT";
                if (ping < 0)
                    ui_status_line_format(ping_text, (size_t)-1, 0x50,
                                          "NOT MEASURED - %s", status);
                else
                    ui_status_line_format(ping_text, (size_t)-1, 0x50,
                                          "%d MS - %s", ping, status);
                row->text_b = ping_text;
                row->selected_b = rid == sel;
                row->selected = rid == sel;
                enable = g_server_view_state.rows_enabled;
            } else {                   /* header */
                row->id = 0x24001;
                enable = 1;
                row->kind = 1;
                row->text_a = (char *)g_section_header_text;
                row->text_b = "BATTLE SERVER SETTINGS";
            }
            row->enabled = enable;
            row->state_10 = (enable == 0);   /* simple rows: !enabled */
        }
    }
    main_view_append_nav_rail(rec, false);   /* 'Q' rail: no tag highlight */
}

/*
 * main_view_append_nav_rail — append the shared 6-entry bottom nav rail.
 *
 * Reads the rail table (GOT 0x1a3720, 0x10 stride {u16 feature entry, u16
 * tag, label}) and appends one row per entry at y = 516, x = i*146-456, with
 * the 0x18 menu row state built from the feature entry and kind 1. When
 * highlight is set, the rail entry whose tag matches the active view tag gets
 * selected_b = 1 (the generic feature-list case); the dedicated cases pass
 * false. Every per-tag case ends with this rail; row_count lands at 0x1a.
 * Raw 1174-1197 / 1676-1700.
 */
static void main_view_append_nav_rail(menu_view_record_t *rec, bool highlight)
{
    menu_row_state_t state;
    menu_view_row_t *row;
    uint32_t i, r;

    for (i = 0; i < 6; i++) {
        r = rec->row_count++;
        row = &rec->rows[r];
        menu_row_state_init(&state, g_feature_entry_table
                            + (uintptr_t)g_nav_rail_table[i].entry * 0x48);
        row->id = g_nav_rail_table[i].entry;        /* {u16 entry, pad} */
        row->text_a = g_nav_rail_table[i].label;
        row->text_b = (char *)g_empty_text;
        row->x = (float)i * 146.0f - 456.0f;        /* 0x43120000, 0xc3e40000 */
        row->y = 516.0f;                            /* 0x44010000 */
        memcpy(&row->font, &state, sizeof state);   /* row state at +0x20 */
        row->kind = 1;
        row->selected_b = highlight
                          && g_nav_rail_table[i].tag == g_theme_tag;
    }
}

/* themes chain chunk 3: covers raw lines 1601-2050 read; implements the generic feature-list case (raw 1199-1701) and the 'd' BSD debug case (raw 1702-2098) */
#define _GNU_SOURCE

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* continues the themes_core.c + themes_c01/c02 translation unit */

/* ---- feature wire table (generic case) ---- */
typedef struct {
    uint32_t id;      /* +0x00 — feature/event id */
    uint32_t _pad04;
    char    *text_a;  /* +0x08 — title */
    char    *text_b;  /* +0x10 — subtitle */
    uint32_t tag;     /* +0x18 — owning view tag */
    uint32_t _pad1c;
    char    *config;  /* +0x20 — mod config name (>= 0x21000 entries) */
} feature_wire_entry_t; /* 0x28 stride, 74 entries */

/* ---- BSD debug about-entry table ('d' case) ---- */
typedef struct {
    uint32_t perm_id;  /* +0x00 — permission slot id */
    uint32_t _pad04;
    char    *config;   /* +0x08 — config name (quality-cycle lock check) */
    char    *title;    /* +0x10 */
    char    *desc;     /* +0x18 — text when the permission is granted */
    uint32_t _pad20;
    uint32_t _pad24;
} about_entry_t;       /* 0x28 stride, 39 entries */

/* ---- map-editor entry table ('e' case, chunk 4) ---- */
typedef struct {
    uint32_t kind;     /* +0x00 — row kind 0..0x12 */
    uint32_t _pad04;
    char    *_pad08;
    char    *title;    /* +0x10 */
    char    *desc;     /* +0x18 */
    uint32_t _pad20;
    uint32_t _pad24;
} editor_entry_t;      /* 0x28 stride, 20 entries */

extern const feature_wire_entry_t g_feature_wire_table[74]; /* 0x19ddd0 */
extern const about_entry_t   g_about_entries[39];           /* 0x19cf48 */
extern const editor_entry_t g_editor_entries[20];           /* 0x19dab0 */
extern const char *g_config_names[];      /* 0x19d588 — mod config names, 3-ptr stride */
extern const char *g_editor_tool_names[]; /* 0x1407d0 — self-relative name offsets */
extern const char *g_font_name_table[];   /* 0x1407e4 — self-relative font name offsets */
extern const float g_camera_zoom_steps[]; /* 0x1407fc — 9 zoom factors */
extern const char *g_camera_mode_names[]; /* 0x140820 — self-relative mode name offsets */
extern const char g_text_off[];           /* 0x13280a — "OFF" */
extern const char g_text_on[];            /* 0x1346a5 — "ON" */

extern uint8_t g_config_status[][0x50];   /* 0x28e710 — per-config status buffers */
extern uint8_t g_camera_status[8][0x50];  /* 0x28f840 — camera axis/zoom status buffers */

extern void ui_named_state_query(const char *name, void *state); /* widgets.c @ 00192340 —
                                  query a named control into an 0x18 state node */

/* ---- BSD debug screen state (d-view) ---- */
extern uint32_t g_debug_perm_cap;      /* 0x28a3e0 — permission slot bound */
extern uint32_t g_debug_perm_mask[];   /* 0x28a3e4 — granted permission bitmask */
extern uint32_t g_debug_visible[8];    /* 0x28a404 — 0x20-byte visible-entry bitmask */
extern uint32_t g_debug_perm_enabled;  /* 0x28a428 — 0: skip permission checks */
extern uint32_t g_debug_waiting;       /* 0x28a42c — "WAITING FOR INPUT OR SCREEN" */
extern uint32_t g_debug_opt_lock;      /* 0x28a438 — max-optimization quality lock */
extern char     g_debug_title[];       /* 0x28a454 — custom "BSD DEBUG" title buffer */

/* ---- map editor screen state (e-view, chunk 4) ---- */
extern uint32_t g_editor_kind_hi;      /* 0x28cbf8 — upper kind bound for arming */
extern uint32_t g_editor_kind_mask;    /* 0x28cbfc — armmable kinds bitmask */
extern uint32_t g_editor_flags14;      /* 0x28cc00 — kinds 14-16 toggle bits */
extern uint32_t g_editor_kind1;        /* 0x28cc04 — kind-1 row value */
extern uint32_t g_editor_tool;         /* 0x28cc08 — current tool index */
extern uint32_t g_editor_filling;      /* 0x28cc14 — fill operation in progress */
extern uint32_t g_editor_fill_done;    /* 0x28cc18 — tiles filled */
extern uint32_t g_editor_fill_total;   /* 0x28cc1c — total tiles */
extern uint32_t g_editor_reopen;       /* 0x28cc24 — config needs editor reopen */
extern uint32_t g_editor_armed;        /* 0x28cc2c — armed from the home screen */
extern uint32_t g_editor_mode;         /* 0x28cc30 — 4 = confirm step */
extern uint32_t g_editor_visible;      /* 0x28cc3c — visible-entry bitmask */
extern char     g_editor_title[];      /* 0x28cc64 — custom "BSD MAP EDITOR" title */
extern char     g_editor_confirm_text[]; /* 0x28dbe4 — confirm row subtitle buffer */
extern uint8_t  g_editor_status[][0xc0]; /* 0x28cce4 — per-entry status buffers */
extern uint32_t g_editor_block;        /* 0x28dce8 — non-zero blocks the confirm row */

/*
 * main_view_case_generic — the shared feature-list view for tags
 * 'C','O','R','U','V' (the bitmap tags without a dedicated raw case).
 *
 * Row count comes from nexus_menu_main_count (5 rows per page), then
 * menu_features_refresh() re-reads the feature state and the 74-entry wire
 * table (0x28 stride) is walked: every entry whose tag matches the active
 * view tag (tag 'R' matches everything) fills a grid cell at
 * x = (match%5)*175-440, y = (match/5)*110+178. Table entries 0..28 are
 * native features (menu_row_state_init from the feature table, remapping
 * ids 10 -> 0x1a, 0xd -> 0x1d, 0x6d -> 0x20 to their parents; special id
 * 0x20002 is a font-0x18 kind-5 section header); entries 29+ are mod
 * configs (ids >= 0x21000) whose row state comes from ui_named_state_query
 * and whose subtitle is formatted into the 0x28e710 buffers ("UNAVAILABLE",
 * "%d FPS", font/camera/tool/ON-OFF variants). The nav rail is appended
 * with the matching rail entry highlighted.
 * Raw body 1199-1701 of nexus_menu_main_view (switch default).
 */
static void main_view_case_generic(menu_view_record_t *rec, uint32_t page)
{
    menu_view_row_t *row;
    menu_row_state_t state;
    const feature_wire_entry_t *ent;
    const char *name, *text;
    char *buf;
    uint32_t count, id, remap, idx, m, v, axis;
    uint32_t match = 0;   /* index among tag-matching entries */
    uint32_t i;
    uint8_t kind;

    count = (uint32_t)nexus_menu_main_count();
    if ((count + 4u) / 5 <= page)
        page = 0;

    menu_features_refresh();

    rec->rev = g_theme_revision;
    rec->row_count = 0x14;
    rec->tag = g_theme_tag;
    rec->installed = g_theme_installed;
    rec->disabled = 0;
    rec->page = (uint8_t)page;
    memset(&rec->_pad0c, 0, 0x684);
    for (i = 0; i < 0x14; i++) {
        rec->rows[i].text_a = (char *)g_empty_text;
        rec->rows[i].text_b = (char *)g_empty_text;
    }

    for (i = 0; i < 74; i++) {
        ent = &g_feature_wire_table[i];
        if (g_theme_tag != 'R' && ent->tag != g_theme_tag)
            continue;                  /* no match: grid counter unchanged */
        if (!(page * 5 <= match && match < page * 5 + 0x14))
            goto next;                 /* matched but off-page: still counts */

        id = ent->id;
        row = &rec->rows[match % 20];
        row->id = id;
        row->text_a = ent->text_a;
        row->text_b = ent->text_b;
        row->y = (float)(match / 5) * 110.0f + 178.0f;  /* 0x42dc0000, 0x43320000 */
        row->x = (float)(match % 5) * 175.0f - 440.0f;  /* 0x432f0000, 0xc3dc0000 */
        row->selected = 0;
        row->feature_id = 0;
        row->state_10 = 0;
        row->enabled = 0;
        row->font = 0;
        row->enabled_bit = 0;
        row->kind = 0;
        row->selected_b = 0;

        if (i < 0x1d) {                /* native feature entries */
            if (id == 0x20002) {       /* section header row */
                row->kind = 5;
                row->font = 0x18;
            } else {
                row->kind = g_feature_entry_table[(uintptr_t)id * 0x48 + 0x44];
                remap = id;
                if (id == 10)
                    remap = 0x1a;      /* remap to parent feature */
                else if (id == 0xd)
                    remap = 0x1d;
                else if (id == 0x6d)
                    remap = 0x20;
                menu_row_state_init(&state, g_feature_entry_table
                                    + (uintptr_t)remap * 0x48);
                row->selected = state.word_08;
                row->feature_id = state.feature_id;
                row->font = state.font;
                row->enabled_bit = state.enabled_bit;
                row->state_10 = state.state_10;
                row->enabled = state.parent_flag;
                if (remap != id)
                    row->enabled = 1;
            }
        } else {                       /* mod config entries (id >= 0x21000) */
            idx = id - 0x21000;
            /* row kind: 2 toggle (default), 0 for the non-toggle set,
             * 4 for the action set — masks keyed by config index */
            m = (uint32_t)(0x0298110000000000ull >> (idx & 0x3f));
            kind = (uint8_t)(((m << 1) ^ 0xffu) & 2u);
            if ((0x042020ff0000000ull >> (idx & 0x3f)) & 1ull)
                kind = 4;
            row->kind = kind;
            ui_named_state_query(ent->config, &row->font);  /* 0x18 state */

            if (row->enabled == 0) {
                buf = (char *)&g_config_status[idx];
                ui_status_line_format(buf, (size_t)-1, 0x50,
                                      "UNAVAILABLE - %s", ent->text_b);
            } else {
                name = g_config_names[idx * 3];
                if (strcmp(name, "FPSLimit") == 0) {
                    buf = (char *)&g_config_status[idx];
                    if (row->feature_id == 0)
                        ui_status_line_format(buf, (size_t)-1, 0x50,
                                              "UNLIMITED - TAP TO CONFIGURE");
                    else
                        ui_status_line_format(buf, (size_t)-1, 0x50,
                                              "%d FPS - TAP TO CONFIGURE",
                                              row->feature_id);
                } else if (strcmp(name, "Font") == 0) {
                    v = row->feature_id;
                    text = "UNKNOWN";
                    if (v < 6)
                        text = (const char *)&g_font_name_table
                               + (ptrdiff_t)g_font_name_table[v];
                    buf = (char *)&g_config_status[idx];
                    ui_status_line_format(buf, (size_t)-1, 0x50,
                                          "%s - TAP TO CONFIGURE", text);
                } else if (strcmp(name, "CameraZoom") == 0) {
                    v = row->feature_id;
                    if (v < 9) {
                        /* raw caps are (0x280, 0x50) for this one call */
                        ui_status_line_format((char *)&g_camera_status[0], 0x280,
                                              0x50, "%.2fx - TAP TO CHANGE",
                                              g_camera_zoom_steps[v]);
                        axis = 0;
                        goto camera_text;
                    }
                    goto plain_status;
                } else if (strcmp(name, "CameraMode") == 0) {
                    v = row->feature_id;
                    if (v < 4) {
                        ui_status_line_format((char *)&g_camera_status[1], 0x230,
                                              0x50, "%s - TAP TO CHANGE",
                                              (const char *)&g_camera_mode_names
                                              + (ptrdiff_t)g_camera_mode_names[v]);
                        axis = 1;
                        goto camera_text;
                    }
                    goto plain_status;
                } else {
                    axis = 2;
                    if (strcmp(name, "CameraX") == 0)
                        axis = 2;
                    else if (strcmp(name, "CameraY") == 0)
                        axis = 3;
                    else if (strcmp(name, "CameraZ") == 0)
                        axis = 4;
                    else if (strcmp(name, "CameraTargetX") == 0)
                        axis = 5;
                    else if (strcmp(name, "CameraTargetY") == 0)
                        axis = 6;
                    else if (strcmp(name, "CameraTilt") == 0)
                        axis = 7;
                    else
                        goto plain_status;
                    v = row->feature_id;
                    if (v < 0x51) {
                        /* signed steps of 250 units, 2 per index */
                        int step = (v & 1) == 0 ? -250 : 250;
                        ui_status_line_format((char *)&g_camera_status[axis],
                                              (size_t)-1, 0x50,
                                              "%+d UNITS - STEP 250",
                                              (int)(((v & 1) + (v >> 1)) * (uint32_t)step));
                    camera_text:
                        buf = (char *)&g_camera_status[axis];
                        goto set_text;
                    }
                }
plain_status:
                buf = ent->text_b;
                if ((0x147ce00fffffffull >> (idx & 0x3f)) & 1ull) {
                    /* configs with an ON/OFF display */
                    buf = (char *)&g_config_status[idx];
                    ui_status_line_format(buf, (size_t)-1, 0x50, "%s - %s",
                                          row->feature_id != 0
                                              ? (const char *)g_text_on
                                              : (const char *)g_text_off,
                                          ent->text_b);
                }
set_text:
                row->text_b = buf;
            }
        }
next:
        match++;
    }

    main_view_append_nav_rail(rec, true);    /* highlight the active tag's rail entry */
}

/*
 * main_view_case_d — build the 'd' BSD debug screen.
 *
 * 41 rows (1 header + 39 about entries), page clamped to 8, five rows per
 * page. Row 0 is the "BSD DEBUG" header (event 0x27000, custom title from
 * 0x28a454); rows 1+ are the about entries (event 0x27020+i) gated by the
 * permission table: an entry is enabled only when the permission checks are
 * active, the entry's permission id is in range and the granted bitmask
 * (0x28a3e4) has the bit set — otherwise it shows "AVAILABLE FROM THE HOME
 * SCREEN" / "WAITING FOR INPUT OR SCREEN", or "DISABLE MAX OPTIMIZATION
 * FIRST" for the quality-cycle entries while the optimization lock is on.
 * Visible entries are recorded in the 0x28a404 bitmask. Grid at
 * x = (i%5)*175-440, y = (i/5)*110+178.
 * Raw body 1702-2098 of nexus_menu_main_view (case 100).
 */
static void main_view_case_d(menu_view_record_t *rec, uint32_t page)
{
    menu_view_row_t *row;
    const about_entry_t *ent;
    const char *text;
    uint32_t perm, bit, i, r;
    bool enabled;

    if (9 <= page)                     /* pages 0..8 for the 41 rows */
        page = 8;

    rec->rev = g_theme_revision;
    rec->row_count = 0x14;
    rec->tag = 'd';
    rec->installed = g_theme_installed;
    rec->disabled = 0;
    rec->page = (uint8_t)page;
    memset(&rec->_pad0c, 0, 0x684);
    for (i = 0; i < 0x14; i++) {
        rec->rows[i].text_a = (char *)g_empty_text;
        rec->rows[i].text_b = (char *)g_empty_text;
    }
    memset(g_debug_visible, 0, sizeof g_debug_visible);  /* 0x28a404, 0x20 bytes */

    i = page * 5;
    do {
        r = i % 20;
        row = &rec->rows[r];
        row->id = 0;
        row->text_a = NULL;
        row->text_b = NULL;
        row->x = (float)(i % 5) * 175.0f - 440.0f;  /* 0x432f0000, 0xc3dc0000 */
        row->y = (float)(i / 5) * 110.0f + 178.0f;  /* 0x42dc0000, 0x43320000 */
        row->font = 0x18;
        row->enabled_bit = 0;
        row->selected = 0;
        row->feature_id = 0;
        row->state_10 = 0;
        row->enabled = 1;
        row->kind = 0;
        row->selected_b = 0;
        row->word_3c = 0;

        if (i == 0) {                  /* header */
            row->id = 0x27000;
            row->text_a = (char *)g_section_header_text;
            text = "BSD DEBUG";
            if (g_debug_title[0] != '\0')
                text = g_debug_title;
            enabled = true;
            row->text_b = (char *)text;
            row->kind = 1;
        } else {
            ent = &g_about_entries[i - 1];
            row->id = i + 0x2701f;
            row->text_a = ent->title;
            perm = ent->perm_id;
            enabled = true;
            if (g_debug_perm_enabled == 0 || g_debug_waiting != 0
                || 0xff < perm || g_debug_perm_cap <= perm) {
unavailable:
                row->enabled = 0;
                text = "AVAILABLE FROM THE HOME SCREEN";
                if (g_debug_waiting != 0)
                    text = "WAITING FOR INPUT OR SCREEN";
                row->text_b = (char *)text;
                if (g_debug_opt_lock == 0) {
                    enabled = false;
                } else {
                    if (strcmp(ent->config, "GFX_QUALITY_CYCLE") == 0
                        || strcmp(ent->config, "MEM_QUALITY_CYCLE") == 0)
                        row->text_b = "DISABLE MAX OPTIMIZATION FIRST";
                    enabled = false;
                }
            } else {
                bit = g_debug_perm_mask[perm >> 5] & (1u << (perm & 0x1f));
                row->enabled = (uint8_t)(bit != 0);
                if (bit == 0)
                    goto unavailable;
                enabled = true;
                row->text_b = ent->desc;
            }
            g_debug_visible[(i - 1) >> 5] |= 1u << ((i - 1) & 0x1f);
        }
        row->state_10 = (uint32_t)!enabled;   /* simple rows: !enabled */
        i++;
    } while (i < 0x28 && i < page * 5 + 0x14);

    main_view_append_nav_rail(rec, false);    /* no tag highlight */
}

/* themes chain chunk 4: covers raw lines 2051-2546 read; implements the 'e' BSD map editor case (raw 2099-2546) */
#define _GNU_SOURCE

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* continues the themes_core.c + themes_c01/c02/c03 translation unit: the
 * editor_entry_t / g_editor_* externs live in themes_c03.c */

/*
 * main_view_case_e — build the 'e' BSD map editor screen.
 *
 * Two rows (header + CONFIRM) while the editor sits in its confirm step
 * (g_editor_mode == 4), else 21 rows (header + 20 editor entries). The page
 * clamp divides the pixel height by 256 ((count*0x34 + 0xd0) >> 8); five
 * rows per page. Row 0 is the "BSD MAP EDITOR" header (event 0x29000,
 * custom title from 0x28cc64). The confirm row (event 0x29001, "CONFIRM")
 * is enabled only while armed and unblocked. The editor entries
 * (event 0x29020+i) are enabled from the armed/kind window
 * (0x13 >= kind <= g_editor_kind_hi with the g_editor_kind_mask bit), and
 * their subtitles are formatted into the 0xc0 buffers at 0x28cce4:
 * toggle rows mirror g_editor_flags14 bits, kind 1 mirrors g_editor_kind1,
 * kinds 2-4 show the current tool, kind 0x10 the reopen notice and kind
 * 0x12 the fill progress. Disabled rows fall back to "AVAILABLE FROM
 * HOME" / "AVAILABLE IN THE MAP EDITOR" / "WAITING FOR INPUT OR
 * CONFIRMATION". Grid at x = (i%5)*175-440, y = (i/5)*110+178.
 * Raw body 2099-2546 of nexus_menu_main_view (case 0x65).
 */
static void main_view_case_e(menu_view_record_t *rec, uint32_t page)
{
    menu_view_row_t *row;
    const editor_entry_t *ent;
    const char *text;
    char *buf;
    uint32_t count, kind, v, i, r;
    uint32_t enabled;

    count = 2;
    if (g_editor_mode != 4)
        count = 0x15;
    if (((count * 0x34 + 0xd0) >> 8) <= page)
        page = 0;

    rec->rev = g_theme_revision;
    rec->row_count = 0x14;
    rec->tag = 'e';
    rec->installed = g_theme_installed;
    rec->disabled = 0;
    rec->page = (uint8_t)page;
    memset(&rec->_pad0c, 0, 0x684);
    for (i = 0; i < 0x14; i++) {
        rec->rows[i].text_a = (char *)g_empty_text;
        rec->rows[i].text_b = (char *)g_empty_text;
    }
    g_editor_visible = 0;

    page = page * 5;
    if (page < count && page < 0xffffffecu) {
        i = page;
        do {
            r = i % 20;
            row = &rec->rows[r];
            row->id = 0;
            row->text_a = NULL;
            row->text_b = NULL;
            row->x = (float)(i % 5) * 175.0f - 440.0f;  /* 0x432f0000, 0xc3dc0000 */
            row->y = (float)(i / 5) * 110.0f + 178.0f;  /* 0x42dc0000, 0x43320000 */
            row->font = 0x18;
            row->enabled_bit = 0;
            row->selected = 0;
            row->feature_id = 0;
            row->enabled = 1;
            row->kind = 0;
            row->selected_b = 0;
            row->word_3c = 0;

            if (i == 0) {              /* header */
                row->id = 0x29000;
                row->text_a = (char *)g_section_header_text;
                text = "BSD MAP EDITOR";
                if (g_editor_title[0] != '\0')
                    text = g_editor_title;
                enabled = 1;
                row->text_b = (char *)text;
                row->kind = 1;
            } else if (g_editor_mode == 4) {   /* confirm step */
                row->id = 0x29001;
                row->text_a = "CONFIRM";
                row->text_b = g_editor_confirm_text;
                enabled = g_editor_armed;
                if (g_editor_armed != 0)
                    enabled = (uint32_t)(g_editor_block == 0);
                row->enabled = (uint8_t)enabled;
                g_editor_visible |= 1u;
            } else {                   /* editor entry i-1 */
                ent = &g_editor_entries[i - 1];
                kind = ent->kind;
                row->id = i + 0x2901f;
                row->text_a = ent->title;
                uint8_t armed_kind = 0;
                if (g_editor_armed != 0 && g_editor_mode == 0)
                    armed_kind = (uint8_t)((0x13u >= kind && kind <= g_editor_kind_hi)
                                           && (0x13u < kind || g_editor_kind_hi != kind)
                                           & (uint8_t)(g_editor_kind_mask >> (kind & 0x1f)));
                row->enabled = armed_kind;
                buf = (char *)&g_editor_status[i - 1];   /* 0x28cce4, 0xc0 stride */

                if (kind - 0xe < 3) {  /* kinds 14-16: flag toggles */
                    row->kind = 2;
                    v = g_editor_flags14 >> (kind & 0x1f);
                    v &= 1u;
                    row->selected = v;        /* both mirrors carry the bit */
                    row->feature_id = v;
flag_status:
                    if (kind == 0x10) {
                        text = "APPLIES WHEN THE EDITOR OPENS";
                        if (g_editor_reopen != 0)
                            text = "REOPEN EDITOR TO APPLY";
plain_status:
                        ui_status_line_format(buf, (size_t)-1, 0xc0,
                                              g_theme_status_fmt, text);
                    } else {
                        if (kind != 0x12)
                            goto desc_status;
fill_status:
                        text = "LAST FILL";
                        if (g_editor_filling != 0)
                            text = "FILLING";
                        ui_status_line_format(buf, (size_t)-1, 0xc0,
                                              "%s %u/%u TILES; NO AUTO-SAVE",
                                              text, g_editor_fill_done,
                                              g_editor_fill_total);
                    }
                } else if (kind == 1) {        /* value row */
                    row->kind = 2;
                    v = g_editor_kind1;
                    row->selected = v & 0xff;
                    row->feature_id = v;
desc_status:
                    if (((kind & 0xfffffffeu) != 8 && kind != 10)
                        || g_editor_filling == 0) {
                        text = ent->desc;
                        goto plain_status;
                    }
                    goto fill_status;
                } else if (2 < kind - 2) {     /* kinds 5+ (and 0): reopen/desc */
                    goto flag_status;
                } else {                       /* kinds 2-4: current tool */
                    v = g_editor_tool;
                    if (4 < g_editor_tool)
                        v = 0;
                    ui_status_line_format(buf, (size_t)-1, 0xc0,
                                          "CURRENT: %s (%d)",
                                          (const char *)&g_editor_tool_names
                                          + (ptrdiff_t)g_editor_tool_names[v],
                                          v);
                }

                enabled = row->enabled;
                row->text_b = buf;
                if (enabled == 0 && g_editor_mode != 0) {
                    row->text_b = "WAITING FOR INPUT OR CONFIRMATION";
                } else if (enabled == 0 && g_editor_filling == 0) {
                    text = "AVAILABLE FROM HOME";
                    if (kind != 0)
                        text = "AVAILABLE IN THE MAP EDITOR";
                    row->text_b = (char *)text;
                }
                g_editor_visible |= 1u << ((i - 1) & 0x1f);
            }
            i++;
            row->state_10 = (uint32_t)(enabled == 0);  /* simple rows: !enabled */
        } while (i < count && i < page + 0x14);
    }

    main_view_append_nav_rail(rec, false);    /* no tag highlight */
}

/* themes chain chunk 5: covers raw lines 2547-2944 ('p' BSD player profiles case) */
#define _GNU_SOURCE

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* continues the themes_core.c + themes_c01-c04 translation unit */

/* ---- BSD player profiles screen state (p-view) ---- */
extern const int32_t g_profile_title_offsets[6]; /* 0x1407b8 — self-relative row title offsets */
extern uint8_t  g_profile_status[][0x1c0];   /* 0x28c168 — per-row status buffers */
extern uint32_t g_profile_request_pending;   /* 0x28a4d0 — profile fetch in flight */
extern uint32_t g_visual_name_pending;       /* 0x28a4d4 — saved visual name change pending */
extern uint32_t g_profile_enabled;           /* 0x28a4c0 — master profiles gate */
extern uint32_t g_profile_feature_mask;      /* 0x28a4c4 — per-row feature bits */
extern uint32_t g_profile_available;         /* 0x28c28c — profile list available */
extern uint32_t g_profile_busy;              /* 0x28c290 — profile operation busy */
extern uint64_t g_profile_skin_names;        /* 0x28c2b8 — pair: value low / show-names bit high */
extern char     g_profile_title_buf[];       /* 0x28c2c8 — custom header title buffer */
extern char     g_profile_local_name[];      /* 0x28a771 — custom local appearance name buffer */

/*
 * main_view_case_p — build the 'p' BSD player profiles screen.
 *
 * Six rows (0-5), page clamped to 1; five rows per page. Row 0 is the
 * "BSD PLAYER PROFILES" header (event 0x28000, custom title from
 * 0x28c2c8); rows 1-5 are the profile features (events 0x28001-0x28005)
 * with titles from the self-relative table at 0x1407b8 and subtitles from
 * the 0x1c0 buffers at 0x28c168 ("SEARCH BY PLAYER TAG" /
 * "PROFILE REQUEST PENDING", "LOCAL NAME APPEARANCE" or the custom name,
 * "RESTORE YOUR ACCOUNT NAME APPEARANCE", "%s - SHOW SKIN NAMES IN
 * PROFILES", "NO VISUAL NAME CHANGE PENDING" / "APPLY THE SAVED VISUAL
 * NAME"). Rows are enabled per-bit from g_profile_feature_mask
 * (row 1 bit 0, rows 2/3/5 bit 2, row 4 bit 3) while the profile list is
 * available and not busy; row 4 is a toggle (kind 2) mirroring the
 * 0x28c2b8 pair, row 5 is forced off when no visual name change is
 * pending. Top row sits at y = 178, row 5 at y = 288;
 * x = col*175-440. Raw body 2547-2944 of nexus_menu_main_view (case 0x70).
 */
static void main_view_case_p(menu_view_record_t *rec, uint32_t page)
{
    menu_view_row_t *row;
    const char *title;
    uint32_t colbits, col, i, n;
    uint32_t enabled;

    if (2 <= page)
        page = 1;                     /* pages 0-1 for the 6 rows */

    /* the five profile status buffers (row 0 has a plain header text) */
    ui_status_line_format((char *)&g_profile_status[1], 0x1c0, 0x1c0,
                          g_theme_status_fmt,
                          g_profile_request_pending != 0
                              ? "PROFILE REQUEST PENDING"
                              : "SEARCH BY PLAYER TAG");
    title = g_profile_local_name;
    if (g_profile_local_name[0] == '\0' || g_profile_available == 0)
        title = "LOCAL NAME APPEARANCE";
    ui_status_line_format((char *)&g_profile_status[2], 0x1c0, 0x1c0,
                          g_theme_status_fmt, title);
    ui_status_line_format((char *)&g_profile_status[3], 0x1c0, 0x1c0,
                          "RESTORE YOUR ACCOUNT NAME APPEARANCE");
    ui_status_line_format((char *)&g_profile_status[4], 0x1c0, 0x1c0,
                          "%s - SHOW SKIN NAMES IN PROFILES",
                          (uint32_t)(g_profile_skin_names >> 32) != 0
                              ? (const char *)g_text_on
                              : (const char *)g_text_off);
    ui_status_line_format((char *)&g_profile_status[5], 0x1c0, 0x1c0,
                          g_theme_status_fmt,
                          g_visual_name_pending != 0
                              ? "APPLY THE SAVED VISUAL NAME"
                              : "NO VISUAL NAME CHANGE PENDING");

    rec->rev = g_theme_revision;
    rec->row_count = 0x14;
    rec->tag = 'p';
    rec->installed = g_theme_installed;
    rec->disabled = 0;
    rec->page = (uint8_t)page;
    memset(&rec->_pad0c, 0, 0x684);
    for (i = 0; i < 0x14; i++) {
        rec->rows[i].text_a = (char *)g_empty_text;
        rec->rows[i].text_b = (char *)g_empty_text;
    }

    for (n = page * 5; n != 6; n++) {
        row = &rec->rows[n];
        colbits = 4;                   /* rows 2, 3, 5 */
        if (n == 4)
            colbits = 8;
        if (n == 1)
            colbits = 1;

        title = (const char *)&g_profile_title_offsets
                + (ptrdiff_t)g_profile_title_offsets[n];
        if (n == 0) {                 /* header text comes from the buffer */
            title = "BSD PLAYER PROFILES";
            if (g_profile_title_buf[0] != '\0')
                title = g_profile_title_buf;
        }

        col = n < 5 ? n : n - 5;       /* row 5 wraps to column 0 */
        enabled = 1;
        if (n != 0) {
            enabled = 0;
            if (g_profile_available != 0 && g_profile_enabled != 0
                && g_profile_busy == 0)
                enabled = (colbits & g_profile_feature_mask) != 0;
        }

        row->id = n + 0x28000;
        row->text_a = (char *)title;
        row->text_b = n == 0 ? (char *)title
                             : (char *)&g_profile_status[n];
        row->x = (float)col * 175.0f - 440.0f;       /* 0x432f0000, 0xc3dc0000 */
        row->y = (float)(n < 5 ? 0u : 1u) * 110.0f + 178.0f;
                                                    /* 0x42dc0000, 0x43320000 */
        row->font = 0x18;
        row->enabled_bit = 0;
        row->selected = 0;
        row->feature_id = 0;
        row->state_10 = 0;
        row->enabled = (uint8_t)enabled;
        row->kind = (uint8_t)(n == 0);
        row->selected_b = 0;
        row->word_3c = 0;

        if (n == 4) {                 /* toggle row mirrors the pair */
            row->kind = 2;
            memcpy(&row->selected, &g_profile_skin_names,
                   sizeof g_profile_skin_names);
        }
        if (n == 5 && g_visual_name_pending == 0) {
            row->enabled = 0;          /* nothing pending: row off, loop ends */
            row->state_10 = 1;
            break;
        }
        row->state_10 = enabled ^ 1;  /* simple rows: !enabled */
    }

    main_view_append_nav_rail(rec, false);    /* no tag highlight */
}

/* themes chain chunk 6: covers raw lines 2945-3582 ('t' theme chooser case; raw 3000-3420 assigned, finished past the boundary) */
#define _GNU_SOURCE

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* continues the themes_core.c + themes_c01-c05 translation unit */

/* ---- theme chooser state read straight from the snapshot mirror ---- */
extern uint32_t g_theme_slot_sel_bg;      /* 0x285428 — selected background theme id (store+0x20) */
extern uint32_t g_theme_slot_sel_music;   /* 0x28542c — selected music theme id (store+0x24) */
extern uint32_t g_theme_slot_flag_set;    /* 0x285430 — confirm-step rows 1-3 ON bits (store+0x28) */
extern uint32_t g_theme_slot_flag_layer;  /* 0x285434 — background-layer toggle value (store+0x2c) */
extern uint32_t g_theme_status_mirror;    /* 0x287bb0 — status counter mirror */
extern uint32_t g_theme_row_slot_ids[8];  /* 0x287bc0 — id mirror for the visible theme rows */
extern uint8_t  g_theme_row_status[][0xd0]; /* 0x287ccc — per-row status buffers */
extern const int32_t g_theme_row_event_ids[10]; /* 0x140790 — transient-step row event ids */
extern const char *g_theme_row_titles[];  /* 0x19e990 — transient-step row titles */
extern const char g_theme_slot_fmt[];     /* 0x135833 — slot status "%s%s" format */

/*
 * main_view_case_t — build the 't' theme chooser (this file's own screen).
 *
 * Row count depends on the apply state: 4 rows for the confirm step
 * (state 2), 10 for the transient options step (state 3), else 6 plus the
 * snapshot's page count (up to 8 theme slots per page). Rows are laid out
 * five per page at x = (n%5)*175-440, y = (n/5)*110+178. Row 0 is the
 * header ("LEAVE THEME SETTINGS" / "CANCEL / PREVIOUS STEP"); the confirm
 * step shows CONFIRM / BACKGROUND / MUSIC; the transient step shows the
 * option toggles (ON/OFF rows gated by the snapshot flag word at 0x285424,
 * "REQUIRES BACKGROUND LAYER SUPPORT", "APPLY NEIGHBORING THEME",
 * "RESTORE GAME BACKGROUND AND MUSIC", the status line); the chooser shows
 * the page range ("%u-%u / %u"), PREVIOUS/NEXT PAGE, THEME OPTIONS / NO
 * MUSIC, THEME STATUS / LOADING and one row per theme slot (event
 * 0x2601a+n, title from the slot name, subtitle "SELECTED - …" /
 * "CHOOSE MUSIC NEXT" / "BACKGROUND FILE UNAVAILABLE" / "USE THIS THEME'S
 * MUSIC" formatted into the 0xd0 buffers at 0x287ccc). The revision is
 * the snapshot epoch plus the theme revision; the status flag is reset
 * and the counter mirrored before the rows are built.
 * Raw body 2945-3582 of nexus_menu_main_view (case 0x74).
 */
static void main_view_case_t(menu_view_record_t *rec, uint32_t page)
{
    menu_view_row_t *row;
    theme_snapshot_t *snap = (theme_snapshot_t *)&g_theme_store;  /* mutable mirror */
    const char *title, *sub, *onoff;
    char *buf, *name;
    uint32_t count, interactive, flag_word, from, en, val;
    uint32_t sel_id, slot_id, flags, n, r, i;

    if (g_theme_apply_state == 2)
        count = 4;
    else if (g_theme_apply_state == 3)
        count = 10;
    else {
        count = 6;
        if (g_theme_page_flags2 != 0)
            count = snap->page_count + 6;
    }
    if ((count + 4u) / 5 <= page)
        page = 0;

    /* rows are live only while the list is available and the machine idle */
    interactive = 0;
    if (g_theme_page_flags2 != 0 && snap->count != 0)
        interactive = (uint32_t)(g_theme_machine_state == 0);
    flag_word = g_theme_slot_flag_word;

    rec->rev = snap->epoch + g_theme_revision;
    rec->row_count = 0x14;
    rec->tag = 't';
    rec->installed = g_theme_installed;
    rec->disabled = 0;
    rec->page = (uint8_t)page;
    memset(&rec->_pad0c, 0, 0x684);
    for (i = 0; i < 0x14; i++) {
        rec->rows[i].text_a = (char *)g_empty_text;
        rec->rows[i].text_b = (char *)g_empty_text;
    }
    g_theme_status_flag = 0;
    g_theme_status_mirror = g_theme_status_counter;

    for (n = page * 5; n < count && n < page * 5 + 0x14; n++) {
        r = n % 20;
        row = &rec->rows[r];
        row->id = 0;
        row->text_a = NULL;
        row->text_b = NULL;
        row->y = (float)(n / 5) * 110.0f + 178.0f;    /* 0x42dc0000, 0x43320000 */
        row->x = (float)(n % 5) * 175.0f - 440.0f;    /* 0x432f0000, 0xc3dc0000 */
        row->font = 0x18;
        row->enabled_bit = 0;
        row->selected = 0;
        row->feature_id = 0;
        row->state_10 = 0;
        row->enabled = 0;
        row->kind = 0;
        row->selected_b = 0;
        row->word_3c = 0;

        if (n == 0) {                 /* header */
            row->id = 0x26001;
            row->text_a = (char *)g_section_header_text;
            row->text_b = g_theme_apply_state != 0
                              ? "CANCEL / PREVIOUS STEP"
                              : "LEAVE THEME SETTINGS";
            row->enabled = 1;
            row->kind = 1;
            continue;
        }

        if (g_theme_apply_state == 3) {
            /* transient options step: rows 1-9 from the event/title tables */
            uint32_t en_bits = 0;
            bool kind2 = false;
            bool gate = true;         /* row 9 is always disabled */

            val = 0;
            title = NULL;
            if (n == 1 || n == 2 || n == 3) {
                uint32_t bit = n == 1 ? 1u : (n == 2 ? 4u : 2u);
                bool on = (g_theme_slot_flag_set & bit) != 0;
                val = (uint32_t)on;
                title = on ? "ON" : "OFF";
                en_bits = 2;
                kind2 = true;
            } else if (n == 6) {
                en_bits = 4;
                val = (uint32_t)(g_theme_slot_flag_layer != 0);
                title = val ? "ON" : "OFF";
                if ((en_bits & flag_word) == 0)
                    title = "REQUIRES BACKGROUND LAYER SUPPORT";
            } else {
                en_bits = (n == 7) ? 8u : 1u;
                if (g_theme_slot_flag_mask != 0)
                    val = (uint32_t)(n == 7);
                if ((n & 0xfffffffeu) == 4) {
                    title = "APPLY NEIGHBORING THEME";
                } else if (n == 8) {
                    title = "RESTORE GAME BACKGROUND AND MUSIC";
                } else if (n == 9) {
                    title = (const char *)g_theme_status_line;
                    gate = false;
                } else {             /* row 7 */
                    title = val ? "ON" : "OFF";
                    if ((en_bits & flag_word) == 0)
                        title = "REQUIRES BACKGROUND LAYER SUPPORT";
                }
            }

            en = 0;
            if (gate && interactive == 1)
                en = (uint32_t)((en_bits & flag_word) != 0);
            row->id = (uint32_t)g_theme_row_event_ids[n];
            row->text_a = (char *)g_theme_row_titles[n];
            row->text_b = (char *)title;
            row->selected = val;
            row->selected_b = (uint8_t)val;
            row->enabled = (uint8_t)en;
            row->state_10 = en ^ 1;
            if (kind2 || (n & 0xfffffffeu) == 6)
                row->kind = 2;
            continue;
        }

        if (g_theme_apply_state == 2) {
            /* confirm step: rows 1-3 */
            if (n == 1) {
                en = 0;
                if (g_theme_reported_status == 1)
                    en = interactive;
                en &= flag_word;
                row->id = 0x26007;
                row->text_a = "CONFIRM";
                row->text_b = (char *)g_theme_status_line;
                row->enabled = (uint8_t)en;
                row->state_10 = en ^ 1;
            } else if (n == 2) {
                row->id = 0x26011;
                row->text_a = "BACKGROUND";
                row->text_b = (char *)&g_theme_slot_default[8];  /* name_a */
            } else if (n == 3) {
                row->id = 0x26012;
                row->text_a = "MUSIC";
                row->text_b = (char *)&g_theme_slot_previous[8]; /* name_a */
            }
            if (n == 2 || n == 3) {
                row->enabled = 0;
                row->state_10 = 1;
            }
            continue;
        }

        /* plain chooser */
        switch (n) {
        case 1: {                     /* page range */
            from = 0;
            if (snap->page_count != 0)
                from = (uint32_t)g_theme_page_word + 1;
            ui_status_line_format((char *)&g_theme_row_status[0], 0x2704,
                                  0xd0, "%u-%u / %u", from,
                                  (uint32_t)g_theme_page_word + snap->page_count,
                                  snap->page_total);
            row->id = 0x26010;
            row->text_a = g_theme_apply_state != 0
                              ? "CHOOSE MUSIC" : "CHOOSE BACKGROUND";
            row->text_b = (char *)&g_theme_row_status[0];
            row->enabled = 0;
            row->state_10 = 1;
            break;
        }
        case 2:                       /* previous page */
            row->id = 0x26002;
            row->text_a = "PREVIOUS PAGE";
            row->text_b = "PREVIOUS 8 THEMES";
            en = 0;
            if ((uint32_t)g_theme_page_word != 0)
                en = interactive;
            row->enabled = (uint8_t)en;
            row->state_10 = en ^ 1;
            break;
        case 3:                       /* next page */
            row->id = 0x26003;
            row->text_a = "NEXT PAGE";
            row->text_b = "NEXT 8 THEMES";
            en = 0;
            if ((uint32_t)g_theme_page_word + 8u < snap->page_total)
                en = interactive;
            row->enabled = (uint8_t)en;
            row->state_10 = en ^ 1;
            break;
        case 4:                       /* theme options / no music */
            if (g_theme_apply_state == 0) {
                row->id = 0x26004;
                row->text_a = "THEME OPTIONS";
                row->text_b = "RANDOM / RESET / BACKGROUND";
            } else {
                row->id = 0x26006;
                row->text_a = "NO MUSIC";
                row->text_b = "CONTINUE WITHOUT MUSIC";
            }
            row->enabled = (uint8_t)interactive;
            row->state_10 = interactive ^ 1;
            break;
        case 5:                       /* status row */
            row->id = 0x26014;
            row->text_a = g_theme_machine_state != 0
                              ? "LOADING" : "THEME STATUS";
            row->text_b = (char *)g_theme_status_line;
            row->enabled = 0;
            row->state_10 = 1;
            break;
        default: {                    /* theme slot rows (n >= 6) */
            uint32_t idx = n - 6;

            sel_id = g_theme_apply_state != 0 ? g_theme_slot_sel_music
                                              : g_theme_slot_sel_bg;
            slot_id = (uint32_t)snap->slots[idx].id;
            flags = snap->slots[idx].flags;
            sub = NULL;
            en = 0;
            if (interactive == 0) {
                if (g_theme_apply_state == 1) {
                    en = 0;
                    sub = "USE THIS THEME'S MUSIC";
                } else {
                    sub = (flags & 2) != 0
                              ? "CHOOSE MUSIC NEXT"
                              : "BACKGROUND FILE UNAVAILABLE";
                }
            } else {
                if (g_theme_apply_state != 1) {
                    en = (flags >> 1) & 1;
                    sub = (flags & 2) != 0
                              ? "CHOOSE MUSIC NEXT"
                              : "BACKGROUND FILE UNAVAILABLE";
                } else {
                    en = 1;
                    sub = "USE THIS THEME'S MUSIC";
                }
            }
            onoff = slot_id == sel_id ? "SELECTED - " : "";
            buf = (char *)&g_theme_row_status[n - 5];
            ui_status_line_format(buf, (size_t)-1, 0xd0,
                                  g_theme_slot_fmt, onoff, sub);

            row->id = n + 0x2601a;
            name = snap->slots[idx].name_a;   /* slot title (mutable mirror) */
            row->text_a = name;
            row->text_b = buf;
            row->selected = (uint32_t)(slot_id == sel_id);
            row->selected_b = (uint8_t)(slot_id == sel_id);
            row->enabled = (uint8_t)en;
            row->state_10 = (uint32_t)(en == 0);
            g_theme_status_flag |= (uint8_t)(1u << (n - 6));
            g_theme_row_slot_ids[idx] = slot_id;
            break;
        }
        }
    }

    main_view_append_nav_rail(rec, false);    /* no tag highlight */
}

/* themes chain chunk 7: covers raw lines 3583-3641 (main_view tail: raw 3583-3595 was folded into the chunk-1 dispatcher as the latch release + return; verified nothing else remained) + the five dlsym export thunks */
#define _GNU_SOURCE

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* continues the themes_core.c + themes_c01-c06 translation unit */

/* ---- dlsym export slots (GOT entries the host resolves to the real
 * implementations above; each thunk below forwards with no arguments —
 * callers go through the dynamic symbol) ---- */
extern int  (*const nexus_menu_theme_open_impl)(int menu_id);              /* 0x1a3a58 */
extern int  (*const nexus_menu_main_view_impl)(void *, uint32_t, int);    /* 0x1a3b08 */
extern int  (*const nexus_menu_theme_preview_impl)(void *, long);          /* 0x1a3b70 */
extern bool (*const nexus_menu_theme_preview_report_impl)(long, int);     /* 0x1a3b78 */
extern uint64_t (*const nexus_menu_theme_scroll_revision_impl)(void);     /* 0x1a3b90 */

/*
 * nexus_menu_theme_open__export — forward through the dlsym slot.
 * — dlsym export thunk @ 001946a0
 */
void nexus_menu_theme_open__export(void)
{
    nexus_menu_theme_open_impl(0);
}

/*
 * nexus_menu_main_view__export — forward through the dlsym slot.
 * — dlsym export thunk @ 00194800
 */
void nexus_menu_main_view__export(void)
{
    nexus_menu_main_view_impl(NULL, 0, 0);
}

/*
 * nexus_menu_theme_preview__export — forward through the dlsym slot.
 * — dlsym export thunk @ 001948d0
 */
void nexus_menu_theme_preview__export(void)
{
    nexus_menu_theme_preview_impl(NULL, 0);
}

/*
 * nexus_menu_theme_preview_report__export — forward through the dlsym slot.
 * — dlsym export thunk @ 001948e0
 */
void nexus_menu_theme_preview_report__export(void)
{
    nexus_menu_theme_preview_report_impl(0, 0);
}

/*
 * nexus_menu_theme_scroll_revision__export — forward through the dlsym slot.
 * — dlsym export thunk @ 00194910
 */
void nexus_menu_theme_scroll_revision__export(void)
{
    nexus_menu_theme_scroll_revision_impl();
}
