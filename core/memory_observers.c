#define _GNU_SOURCE 1

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>

#define GAME_IMAGE_SPAN 0x1400000
#define OBSERVER_RING_STEP 0x100000
#define OBSERVER_RING_LIMIT 0x7000001
#define OBSERVER_BRANCH_REACH 0x8000000
#define OBSERVER_MIN_ADDR 0x10000
#define OBSERVER_PLAN_MAGIC 0x0000003000000001ull
#define OBSERVER_STATUS_MAGIC 0x000000d000000001ull
#define OBSERVER_FEED_MAGIC 0x0000002800000001ull
#define OBSERVER_ANCHOR_WORD 0xd10243ffu
#define OBSERVER_TPL_SIZE 0x158
#define OBSERVER_TPL_TAIL_OFF 0x148
#define OBSERVER_TPL_WORD_OFF 0x144
#define OBSERVER_TPL_BODY_OFF 0x150
#define OBSERVER_SLOTS_MAX 64
#define OBSERVER_MAPS_MAX_LINES 0x8000
#define OBSERVER_MAPS_MAX_BYTES 0x800000
#define OBSERVER_MAPS_LINE_MAX 0x2000
#define OBSERVER_ADDR_CEILING 0xfffffffffffff000ull
#define OBSERVER_PAIR_OFFSET 0xfac
#define OBSERVER_VALUE_BASE 0xea61
#define OBSERVER_VALUE_MASK 0xfffe2b3eu

typedef struct {
    const char *phase;
    uint32_t err;
} observer_phase_cell_t;

typedef struct {
    uintptr_t game_base;
    uintptr_t target;
    uintptr_t lo;
    uintptr_t hi;
    uintptr_t slots[OBSERVER_SLOTS_MAX];
    size_t size;
    size_t tail;
    uint32_t count;
    uint32_t index;
} observer_gap_window_t;

typedef struct {
    uint64_t magic;
    uintptr_t entry;
    uintptr_t home;
    uintptr_t resume;
    uint32_t original_word;
    uint32_t enter_branch;
    uint32_t template_size;
    uint32_t tail_pad;
} observer_plan_t;

typedef struct {
    uint64_t magic;
    uint64_t reserved1[4];
    uint64_t expect_addr;
    uint64_t reserved2[6];
    uint64_t bound_entry;
    uint64_t bound_observer;
    uint64_t reserved3[12];
} observer_status_t;

extern size_t g_page_size;
extern uintptr_t g_game_base;
extern int g_observer_ready;
extern int g_observer_thread;
extern int g_runtime_ready;
extern uint32_t g_remote_write_fd;
extern uint32_t ng_saved_entry_word;
extern int32_t g_binder_pin_fd;
extern int32_t g_binder_spray_fd;
extern char ng_phase_key[];
extern char ng_latch_key[];
extern char ng_prepare_key[];
extern void *cell_lock(void *key);
extern void cache_flush(void *begin, void *end);
extern void ng_phase_fail(const char *label);
extern long remote_write(uint32_t fd, const void *buf, long len, uintptr_t addr);
extern void observer_gap_offer(observer_gap_window_t *window, uintptr_t prev_end,
                               uintptr_t cur_start);
extern int ng_status_v1(uint64_t *desc);
extern uint64_t ng_current_entry(void);
extern void ng_feed_v1(void *record);
extern void ng_observer_body(uint64_t *regs);
extern void nexus_evasion_unbind(void);

static const uint32_t ng_observer_template[OBSERVER_TPL_SIZE / 4] = {
    0xd10c43ff, 0xa90007e0, 0xa9010fe2, 0xa90217e4,
    0xa9031fe6, 0xa90427e8, 0xa9052fea, 0xa90637ec,
    0xa9073fee, 0xa90847f0, 0xa9094ff2, 0xa90a57f4,
    0xa90b5ff6, 0xa90c67f8, 0xa90d6ffa, 0xa90e77fc,
    0xf9007bfe, 0xd53b4209, 0xf9007fe9, 0xd53b4409,
    0xf90083e9, 0xd53b4429, 0xf90087e9, 0xad0887e0,
    0xad098fe2, 0xad0a97e4, 0xad0b9fe6, 0xad0ca7e8,
    0xad0dafea, 0xad0eb7ec, 0xad0fbfee, 0xad10c7f0,
    0xad11cff2, 0xad12d7f4, 0xad13dff6, 0xad14e7f8,
    0xad15effa, 0xad16f7fc, 0xad17fffe, 0x910003e0,
    0x58000590, 0xd63f0200, 0xad4887e0, 0xad498fe2,
    0xad4a97e4, 0xad4b9fe6, 0xad4ca7e8, 0xad4dafea,
    0xad4eb7ec, 0xad4fbfee, 0xad50c7f0, 0xad51cff2,
    0xad52d7f4, 0xad53dff6, 0xad54e7f8, 0xad55effa,
    0xad56f7fc, 0xad57fffe, 0xf94087e9, 0xd51b4429,
    0xf94083e9, 0xd51b4409, 0xf9407fe9, 0xd51b4209,
    0xf9407bfe, 0xa94e77fc, 0xa94d6ffa, 0xa94c67f8,
    0xa94b5ff6, 0xa94a57f4, 0xa9494ff2, 0xa94847f0,
    0xa9473fee, 0xa94637ec, 0xa9452fea, 0xa94427e8,
    0xa9431fe6, 0xa94217e4, 0xa9410fe2, 0xa94007e0,
    0x910c43ff, 0xd503201f, 0x14000000, 0xd503201f,
    0x00000000, 0x00000000,
};

static uint64_t ng_observer_hits;

static void close_fd_slot(int32_t *slot)
{
    int32_t old = __atomic_exchange_n(slot, -1, __ATOMIC_ACQ_REL);

    if (old >= 0)
        close(old);
}

void JNI_OnUnload(void)
{
    close_fd_slot(&g_binder_pin_fd);
    close_fd_slot(&g_binder_spray_fd);
    nexus_evasion_unbind();
}

void ng_observe_registers_v1(uint64_t *regs)
{
    uint64_t record[5];
    int saved_errno = errno;

    if (regs != NULL) {
        record[0] = OBSERVER_FEED_MAGIC;
        record[1] = __atomic_add_fetch(&ng_observer_hits, 1, __ATOMIC_SEQ_CST);
        record[2] = regs[0];
        record[3] = regs[1];
        record[4] = regs[2];
        ng_feed_v1(record);
    }
    errno = saved_errno;
}

long ng_observer_template_size(void)
{
    return OBSERVER_TPL_SIZE;
}

static int branch_reachable(uintptr_t from, uintptr_t to)
{
    if (from < to)
        return (to - from) < OBSERVER_BRANCH_REACH;
    if (to < from)
        return (from - to) <= OBSERVER_BRANCH_REACH;
    return 1;
}

static int branch_word(uintptr_t from, uintptr_t to, uint32_t *out)
{
    uint64_t diff;
    uint32_t imm;

    if (((uint32_t)from | (uint32_t)to) & 3u)
        return 0;
    if (to > from) {
        diff = to - from;
        if (diff >> 0x1b != 0)
            return 0;
        imm = (uint32_t)(diff >> 2);
    } else if (to < from) {
        diff = from - to;
        if (diff > OBSERVER_BRANCH_REACH)
            return 0;
        imm = (uint32_t)(0u - (uint32_t)(diff >> 2)) & 0x3ffffffu;
    } else {
        imm = 0;
    }
    *out = 0x14000000u | imm;
    return 1;
}

int ng_prepare_observer_v1(uintptr_t home, void *dest, size_t size, observer_plan_t *plan)
{
    observer_status_t status;
    uintptr_t entry;
    uint32_t resume_branch;
    const char *stage = "plan_contract";

    if (plan == NULL || plan->magic != OBSERVER_PLAN_MAGIC || dest == NULL
        || home < OBSERVER_MIN_ADDR || (home & 0xf) != 0)
        goto fail;
    memset(&status, 0, sizeof status);
    status.magic = OBSERVER_STATUS_MAGIC;
    if (ng_status_v1((uint64_t *)&status) == 0) {
        stage = "plan_status";
        goto fail;
    }
    if (status.bound_entry == 0) {
        stage = "plan_unbound";
        goto fail;
    }
    entry = ng_current_entry();
    if (entry == 0 || plan->entry != entry) {
        stage = "plan_current_entry";
        goto fail;
    }
    if (size < OBSERVER_TPL_SIZE || home > UINT64_MAX - OBSERVER_TPL_SIZE) {
        stage = "plan_capacity";
        goto fail;
    }
    if (!branch_word(entry, home, &plan->enter_branch)) {
        stage = "enter";
        goto fail;
    }
    if (!branch_word(home + OBSERVER_TPL_TAIL_OFF, entry + 4, &resume_branch)) {
        stage = "resume";
        goto fail;
    }
    memcpy(dest, ng_observer_template, OBSERVER_TPL_SIZE);
    *(uint32_t *)((char *)dest + OBSERVER_TPL_WORD_OFF) = OBSERVER_ANCHOR_WORD;
    *(uint32_t *)((char *)dest + OBSERVER_TPL_TAIL_OFF) = resume_branch;
    *(void **)((char *)dest + OBSERVER_TPL_BODY_OFF) = ng_observe_registers_v1;
    plan->home = home;
    plan->resume = entry + 4;
    plan->original_word = OBSERVER_ANCHOR_WORD;
    plan->template_size = OBSERVER_TPL_SIZE;
    plan->tail_pad = 0;
    return 1;
fail:
    ng_phase_fail(stage);
    return 0;
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

static int maps_getline(FILE *f, char *buf, size_t size)
{
    for (int tries = 0; tries < 9; tries++) {
        errno = 0;
        if (fgets(buf, (int)size, f) != NULL)
            return 1;
        if (!(ferror(f) && errno == EINTR))
            break;
        clearerr(f);
    }
    return ferror(f) ? -1 : 0;
}

static int observer_home_try(uintptr_t entry, void *m, size_t page,
                             observer_plan_t *plan, observer_phase_cell_t *phase,
                             int *latch, int *in_prepare, void **home_out)
{
    int r;

    memset(plan, 0, sizeof *plan);
    plan->magic = OBSERVER_PLAN_MAGIC;
    plan->entry = entry;

    phase->err = 0;
    phase->phase = "prepare";
    *latch = 0;
    *in_prepare = 1;
    r = ng_prepare_observer_v1((uintptr_t)m, m, page, plan);
    *in_prepare = 0;
    if (r) {
        *(void **)((char *)m + OBSERVER_TPL_BODY_OFF) = ng_observer_body;
        cache_flush(m, (char *)m + plan->template_size);
        if (mprotect(m, page, PROT_READ | PROT_EXEC) == 0) {
            ng_saved_entry_word = plan->original_word;
            *home_out = m;
            return 1;
        }
        phase->err = errno;
        phase->phase = "seal";
    }
    if (munmap(m, page) != 0) {
        phase->err = errno;
        phase->phase = "gap_unmap";
        return -1;
    }
    return 0;
}

void *ng_install_observer(const void *entry_ptr)
{
    uintptr_t entry = (uintptr_t)entry_ptr;
    size_t page = g_page_size;
    uintptr_t aligned = entry & -page;
    uintptr_t tail_off = OBSERVER_TPL_TAIL_OFF;
    observer_phase_cell_t *phase = cell_lock(ng_phase_key);
    int *latch = cell_lock(ng_latch_key);
    int *in_prepare = cell_lock(ng_prepare_key);
    observer_plan_t plan;
    observer_gap_window_t window;
    void *home = NULL;
    void *m;
    uintptr_t off;
    int r;

    phase->err = 0;
    phase->phase = "mmap";

    off = OBSERVER_RING_STEP;
    do {
        if (aligned + off != 0) {
            m = mmap((void *)(aligned + off), page, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
            if (m == MAP_FAILED) {
                phase->err = errno;
                phase->phase = "mmap";
            } else {
                r = observer_home_try(entry, m, page, &plan, phase, latch, in_prepare, &home);
                if (r == 1)
                    return home;
                if (r < 0)
                    return NULL;
            }
        }
        if (off < aligned) {
            m = mmap((void *)(aligned - off), page, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
            if (m == MAP_FAILED) {
                phase->err = errno;
                phase->phase = "mmap";
            } else {
                r = observer_home_try(entry, m, page, &plan, phase, latch, in_prepare, &home);
                if (r == 1)
                    return home;
                if (r < 0)
                    return NULL;
            }
        }
        off += OBSERVER_RING_STEP;
    } while (off < OBSERVER_RING_LIMIT);

    memset(&window, 0, sizeof window);

    if (g_game_base <= entry && (entry - g_game_base) >> 0x16 < 5
        && g_game_base < 0xfffffffffec00000ull
        && (page == 0x1000 || (page == 0x4000 && (entry & 3) == 0))
        && tail_off > 3 && tail_off <= page - 4 && (tail_off & 3) == 0) {
        uintptr_t lo_raw;
        uintptr_t hi_raw;
        uintptr_t max_tail;
        FILE *f;

        lo_raw = entry - OBSERVER_BRANCH_REACH;
        if (lo_raw < 0x10001)
            lo_raw = 0x10000;
        if (entry >> 0x1b == 0)
            lo_raw = 0x10000;
        max_tail = tail_off - 4;
        if (max_tail < 4)
            max_tail = 4;
        hi_raw = entry + (OBSERVER_BRANCH_REACH - max_tail);
        if (entry > ~(OBSERVER_BRANCH_REACH - max_tail))
            hi_raw = UINT64_MAX;
        if (~page <= hi_raw)
            hi_raw = ~page;

        window.game_base = g_game_base;
        window.target = entry;
        window.size = page;
        window.tail = tail_off;
        window.lo = (page - 1 + lo_raw) & -page;
        window.hi = hi_raw & -page;

        if (window.lo > window.hi) {
            phase->err = 0;
            phase->phase = "gap_window";
            return NULL;
        }
        f = open_proc_maps();
        if (f == NULL) {
            phase->err = errno;
            phase->phase = "gap_open";
            return NULL;
        }
        {
            char line[OBSERVER_MAPS_LINE_MAX];
            uint32_t lines = 0;
            size_t total = 0;
            uintptr_t prev_end = 0;
            int covered = 0;
            int gr;

            while ((gr = maps_getline(f, line, sizeof line)) == 1) {
                size_t len = strlen(line);
                uintptr_t start = 0;
                uintptr_t end = 0;
                const char *perms;
                int i = 0;
                int j = 0;
                int c;

                if (len == 0 || line[len - 1] != '\n')
                    goto gap_bounds;
                lines++;
                if (lines > OBSERVER_MAPS_MAX_LINES)
                    goto gap_bounds;
                if (len > OBSERVER_MAPS_MAX_BYTES - total)
                    goto gap_bounds;
                total += len;
                while ((c = hex_value((unsigned char)line[i])) >= 0) {
                    start = start * 16 + (uintptr_t)c;
                    i++;
                    if (start >> 60)
                        goto gap_parse;
                }
                if (i == 0 || line[i] != '-')
                    goto gap_parse;
                i++;
                while ((c = hex_value((unsigned char)line[i + j])) >= 0) {
                    end = end * 16 + (uintptr_t)c;
                    j++;
                    if (end >> 60)
                        goto gap_parse;
                }
                if (j == 0 || line[i + j] != ' ')
                    goto gap_parse;
                perms = line + i + j + 1;
                if (strlen(perms) < 5
                    || (perms[0] != 'r' && perms[0] != '-')
                    || (perms[1] != 'w' && perms[1] != '-')
                    || (perms[2] != 'x' && perms[2] != '-')
                    || (perms[3] != 's' && perms[3] != 'p')
                    || perms[4] != ' '
                    || end <= start
                    || start < prev_end
                    || ((page - 1) & (start | end)) != 0)
                    goto gap_parse;
                observer_gap_offer(&window, prev_end, start);
                if (entry >= start && entry <= UINT64_MAX - 4 && entry + 4 <= end)
                    covered = 1;
                prev_end = end;
                continue;
gap_bounds:
                phase->err = 0;
                phase->phase = "gap_bounds";
                goto scan_fail;
gap_parse:
                phase->err = 0;
                phase->phase = "gap_parse";
                goto scan_fail;
            }
            if (gr < 0) {
                phase->err = ferror(f) ? errno : 0;
                phase->phase = "gap_read";
                goto scan_fail;
            }
            if (lines == 0 || !covered) {
                phase->err = 0;
                phase->phase = "gap_maps";
                goto scan_fail;
            }
            observer_gap_offer(&window, prev_end, UINT64_MAX);
            if (fclose(f) != 0) {
                phase->err = errno;
                phase->phase = "gap_close";
                return NULL;
            }
        }
        phase->err = 0;
        phase->phase = "gap_empty";
        {
            uintptr_t limit = window.game_base + GAME_IMAGE_SPAN;
            uintptr_t page_mask = page - 1;
            uintptr_t top = ~page;
            uintptr_t resume = entry + 4;
            uint32_t idx;

            for (idx = 0; idx < window.count; idx++) {
                uintptr_t cand = window.slots[idx];

                if (cand < OBSERVER_MIN_ADDR || (cand & page_mask) != 0 || cand > top
                    || (cand < limit && window.game_base < cand + page)
                    || ((uint32_t)(entry | cand) & 3u) != 0
                    || !branch_reachable(entry, cand)
                    || !branch_reachable(cand + tail_off, resume)) {
                    phase->err = 0;
                    phase->phase = "gap_range";
                    return NULL;
                }
                m = mmap((void *)cand, page, PROT_READ | PROT_WRITE,
                         MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);
                if (m == MAP_FAILED) {
                    phase->err = errno;
                    phase->phase = "gap_mmap";
                    continue;
                }
                if ((uintptr_t)m != cand) {
                    phase->err = 0;
                    phase->phase = "gap_return";
                    if (munmap(m, page) != 0) {
                        phase->err = errno;
                        phase->phase = "gap_unmap";
                        return NULL;
                    }
                    continue;
                }
                r = observer_home_try(entry, m, page, &plan, phase, latch, in_prepare, &home);
                if (r == 1)
                    return home;
                if (r < 0)
                    return NULL;
            }
        }
        return NULL;
scan_fail:
        fclose(f);
        return NULL;
    }
    phase->err = 0;
    phase->phase = "gap_window";
    return NULL;
}

static int value_plausible(int v)
{
    return (uint32_t)(v - OBSERVER_VALUE_BASE) > OBSERVER_VALUE_MASK;
}

void ng_remote_store_pair(uint64_t token, uintptr_t addr, int lo, int hi)
{
    observer_status_t status;
    uint32_t pair[2];

    (void)token;
    if (g_observer_ready == 0 || g_runtime_ready == 0)
        return;
    if (g_observer_thread != gettid())
        return;
    memset(&status, 0, sizeof status);
    status.magic = OBSERVER_STATUS_MAGIC;
    if (ng_status_v1((uint64_t *)&status) == 0)
        return;
    if (status.bound_observer == 0)
        return;
    if (!value_plausible(hi) || !value_plausible(lo))
        return;
    if (addr >= OBSERVER_ADDR_CEILING || status.expect_addr != addr)
        return;
    pair[0] = (uint32_t)lo;
    pair[1] = (uint32_t)hi;
    remote_write(g_remote_write_fd, pair, 8, addr + OBSERVER_PAIR_OFFSET);
}
