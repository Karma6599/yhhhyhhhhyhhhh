/*
 * gl_hooks - Core plumbing (mixed: reconstructed GL pipeline + raw remainder)
 * Decompiled with Ghidra 11.3.2 (arm64 pseudocode) from: libNexusEvasionRuntime69252.so
 * Layout note: the Evasion published API moved to core/evasion_api.c, script ports +
 * runtime registrars moved to core/script_ports.c (both reconstructed). This file holds
 * the EvasionRuntime visual pipeline: ng_* observers, the GL hook installer, the overlay
 * renderer + HUD builders, ChaCha20/SHA-256/HMAC crypto, the SDF angular-coverage kernel,
 * snapshot/entity ingest, SuperAim, and the state/route/periodic machinery - all
 * reconstructed below. Still raw at the end: the guarded-hook install plumbing family
 * (FUN_0014a9a0, FUN_0014b8e8, FUN_0014dbbc, FUN_0014ea08, FUN_00152844, FUN_00156308,
 * FUN_00156238, FUN_00158260, FUN_0016c1e8, FUN_0016ce3c, FUN_0016d2dc, FUN_00179be0,
 * FUN_0017e37c, FUN_00183164, FUN_00183598, FUN_001a8430) plus their callees.
 */

#define _GNU_SOURCE 1

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <dlfcn.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/sysmacros.h>

extern int __android_log_print(int prio, const char *tag, const char *fmt, ...);
extern uint64_t source_now_us(void);
extern char ng_prepare_key[];
extern char ng_latch_key[];
extern void *cell_lock(void *key);
extern void cache_flush(void *begin, void *end);
extern int mem_read(long handle, uintptr_t addr, void *out, uint32_t len);
extern void route_descriptor_stub_a(void);
extern void route_descriptor_stub_b(void);
extern void route_descriptor_stub_c(void);
extern void route_descriptor_stub_d(void);
extern void route_event_dispatch(void);
extern void route_frame_prepare(void);
extern void route_frame_consume(void);
extern void route_kind_report(void);
extern void natural_skill_registers_committed(void);
extern void frame_observer_publish(void);
extern void ability_capture_callback(void);
extern const uint32_t *gl_contract_block(void);
extern void esp_frame_publish(void *frame);
extern int periodic_enqueue(uint32_t type);


#define GL_BUILD_ID "a10aeb6b4085fb2a15d969a130cd9608231998b5269a40aed41129d99624ede3"
#define GL_IMAGE_ID "fdf834103d333f9f8a1947b3a405b32da6ebb651"
#define GL_ANCHOR_INSN 0xd10243ffu

extern int  gl_license_ok(void);
extern void *gl_dlsym_lookup(void *handle, const char *symbol);
extern int  gl_symbol_table_install(void *table, const uint32_t *contract, void **fns);
extern void gl_symbol_table_teardown(void *table);
extern int  gl_hook_slot_commit(void *slot, const void *contract, void *original,
                                void *replacement);
extern int  gl_hook_slot_enable(void *slot);
extern int  gl_hook_slot_unpatch(void *slot);
extern void gl_swap_trampoline(void);
extern int  ng_signature_scan(const void *contract, const void *pattern);
extern int  ng_vtable_check(const void *contract, const uint32_t *pairs, uint32_t count);
extern void ng_observer_feed(void *observer, const void *packet, uint32_t len);
extern int  rect_emit(void *batch, int x, int y, int w, int h, int kind, int flags);

static volatile uint8_t ng_lock_byte;
static uint64_t ng_contract_word0;
static uint64_t ng_game_base;
static const char *ng_build_id;
static const char *ng_image_id;
static void *ng_read_ctx;
static int (*ng_read_mem)(void *ctx, uint64_t addr, void *out, uint32_t len);
static void *ng_word_c;
static void *ng_word_e;
static uint64_t ng_telemetry[26];
static uint64_t ng_projectile_registry[40][5];
static uint64_t ng_projectile_count;

static void ng_lock(void)
{
    while (__atomic_test_and_set(&ng_lock_byte, __ATOMIC_ACQUIRE)) {
    }
}

static void ng_unlock(void)
{
    __atomic_clear(&ng_lock_byte, __ATOMIC_RELEASE);
}

static void ng_telemetry_reset(void)
{
    memset(ng_telemetry, 0, sizeof(ng_telemetry));
    memset(ng_projectile_registry, 0, sizeof(ng_projectile_registry));
    ng_projectile_count = 0;
    ng_telemetry[4] = (ng_telemetry[4] & 0xffffffff00000000ull) | 1;
    ng_telemetry[5]++;
}

int ng_unbind_v1(void)
{
    ng_lock();
    ng_contract_word0 = 0;
    ng_game_base = 0;
    ng_build_id = NULL;
    ng_image_id = NULL;
    ng_read_ctx = NULL;
    ng_read_mem = NULL;
    ng_word_c = NULL;
    ng_word_e = NULL;
    ng_telemetry_reset();
    ng_unlock();
    return 1;
}

uint64_t ng_anchor_probe(void)
{
    uint32_t insn = 0;
    uint64_t result = 0;

    ng_lock();
    if (ng_read_mem != NULL) {
        if (ng_game_base + 0xb337c4 >= 0x10000) {
            if (ng_read_mem(ng_read_ctx, ng_game_base + 0xb337c4, &insn, 4) == 1 &&
                insn == GL_ANCHOR_INSN) {
                result = ng_game_base + 0xb337c4;
            }
        }
    }
    ng_unlock();
    return result;
}

static volatile uint8_t ng_entry_probe_busy;

uint64_t ng_current_entry(void)
{
    uint64_t result = 0;
    uint32_t insn = 0;

    if (!__atomic_test_and_set(&ng_entry_probe_busy, __ATOMIC_ACQUIRE)) {
        if (ng_read_mem != NULL) {
            uint64_t anchor = ng_game_base + 0xb337c4;
            if (anchor >= 0x10000
                && (ng_game_base & 0xfffffffffffffffcull) != 0xffffffffff4cc838ull) {
                if (ng_read_mem(ng_read_ctx, anchor, &insn, 4) == 1 && insn == GL_ANCHOR_INSN)
                    result = anchor;
            }
        }
        __atomic_clear(&ng_entry_probe_busy, __ATOMIC_RELEASE);
    }
    return result;
}

int ng_bind_v1(const uint32_t *contract)
{
    const uint32_t *h = contract;
    const uint64_t *d = (const uint64_t *)contract;
    uint32_t i;

    ng_lock();
    ng_telemetry_reset();
    if (contract == NULL || h[0] != 1 || h[2] != 0x40) {
        ng_unlock();
        return 0;
    }
    if (d[1] < 0x10000 || (d[1] & 7) != 0 || d[1] >= 0xfffffffffec00000ull) {
        ng_unlock();
        return 0;
    }
    if ((void *)d[2] == NULL || (void *)d[3] == NULL || (void *)d[4] == NULL ||
        (void *)d[6] == NULL || (void *)d[7] == NULL) {
        ng_unlock();
        return 0;
    }
    if (strcmp((const char *)d[2], GL_BUILD_ID) != 0) {
        ng_unlock();
        return 0;
    }
    if (strcmp((const char *)d[3], GL_IMAGE_ID) != 0) {
        ng_unlock();
        return 0;
    }
    for (i = 0; i < 32; i++) {
        if (!ng_signature_scan(contract, (const void *)(uintptr_t)(0x1225f0 + i * 0x28))) {
            ng_unlock();
            return 0;
        }
    }
    if (!ng_signature_scan(contract, (const void *)(uintptr_t)0x122af0) ||
        !ng_signature_scan(contract, (const void *)(uintptr_t)0x122b18) ||
        !ng_signature_scan(contract, (const void *)(uintptr_t)0x122b40)) {
        ng_unlock();
        return 0;
    }
    if (!ng_vtable_check(contract, (const uint32_t *)(uintptr_t)0x122b68, 8)) {
        ng_unlock();
        return 0;
    }
    ng_read_ctx = (void *)(uintptr_t)d[5];
    ng_read_mem = (int (*)(void *, uint64_t, void *, uint32_t))(uintptr_t)d[4];
    ng_word_e = (void *)(uintptr_t)d[7];
    ng_word_c = (void *)(uintptr_t)d[6];
    ng_game_base = d[1];
    ng_contract_word0 = d[0];
    ng_telemetry[7] = (ng_telemetry[7] & 0xffffffffull) | 0x100000000ull;
    ng_build_id = GL_BUILD_ID;
    ng_image_id = GL_IMAGE_ID;
    ng_unlock();
    return 1;
}

int ng_status_v1(uint64_t *desc)
{
    if (desc == NULL) {
        return 0;
    }
    if ((uint32_t)desc[0] != 1 || (uint32_t)(desc[0] >> 32) != 0xd0) {
        return 0;
    }
    ng_lock();
    memcpy(desc, ng_telemetry, 0xd0);
    ng_unlock();
    return 1;
}

int ng_projectiles_v1(void *out, uint64_t count, uint64_t *stamp_out)
{
    uint64_t available;

    if (out == NULL && count != 0) {
        return -1;
    }
    if (count > 0x40) {
        return -1;
    }
    ng_lock();
    available = ng_projectile_count;
    if (count < available) {
        ng_unlock();
        return -1;
    }
    if (available != 0) {
        memcpy(out, ng_projectile_registry, (size_t)available * 0x28);
    }
    *stamp_out = ng_telemetry[3];
    ng_unlock();
    return (int)available;
}

void ng_query_response_init(uint64_t *desc)
{
    memset(desc, 0, 14 * sizeof(uint64_t));
    desc[0] = 0x1213e8;
    desc[1] = 0x1213f0;
    desc[2] = 0x1213f8;
    desc[3] = 0x121400;
}

void ng_observer_serialize(void *observer, uint8_t out[32])
{
    uint64_t count = *(uint64_t *)((char *)observer + 0x20);
    uint8_t packet[0x80];
    uint32_t i;
    uint64_t slot;
    uint32_t payload;

    memset(packet, 0, sizeof(packet));
    slot = 0x38;
    if (*(uint64_t *)((char *)observer + 0x28) >= 0x37) {
        slot = 0x78;
    }
    slot -= *(uint64_t *)((char *)observer + 0x28);
    payload = (uint32_t)(count * 8 * 29);
    packet[0] = 0x80;
    *(uint32_t *)&packet[slot] = payload;
    packet[slot + 4] = (uint8_t)(count >> 0x15);
    packet[slot + 5] = (uint8_t)(count >> 0xd);
    packet[slot + 6] = (uint8_t)(count >> 5);
    packet[slot + 7] = (uint8_t)(count << 3);
    ng_observer_feed(observer, packet, (uint32_t)slot + 8);
    for (i = 0; i < 0x20; i++) {
        uint32_t word = *(uint32_t *)((char *)observer + (i & ~3u));
        out[i] = (uint8_t)(word >> ((~i & 3u) * 8));
    }
}

int hitbox_frame_emit(const uint32_t *box, void *batch)
{
    uint32_t x = box[0];
    uint32_t y = box[1];
    uint32_t w = box[2];
    uint32_t h = box[3];
    int y2 = (int)y + (int)h + (int)x;

    if (batch == NULL) {
        return 0;
    }
    if ((int32_t)x < 0 || (int32_t)y < 0 || (int32_t)w <= 0 || (int32_t)h <= 0) {
        return 0;
    }
    if (x >= 0x2001 || y >= 0x2001 || w >= 0x2001 || h >= 0x2001) {
        return 0;
    }
    if (w < 0x3f || h < 0x3f) {
        return 0;
    }
    rect_emit(batch, (int)x + 0x10, y2 - 0x12, 0x20, 2, 2, 0);
    rect_emit(batch, (int)x + 0x10, y2 - 0x30, 0x20, 2, 2, 0);
    rect_emit(batch, (int)x + 0x10, y2 - 0x2e, 2, 0x1c, 2, 0);
    rect_emit(batch, (int)x + 0x2e, y2 - 0x2e, 2, 0x1c, 2, 0);
    return 1;
}

static volatile uint8_t gl_install_lock;
static uint32_t gl_install_state;
static uint32_t gl_install_status;
static uint8_t gl_committed;
static void *gl_egl_handle;
static void *gl_gles_handle;
static void *gl_original_swap;
static void *gl_hook_slot;
static void *gl_symbol_table;
static uint64_t gl_game_swap_slot;
static uint32_t gl_surface_w;
static uint32_t gl_surface_h;
static uint64_t gl_surface_stamp;
static uint64_t gl_hook_table_stamp;
static uint64_t gl_hook_slot_addr;
static uint64_t gl_game_context;
static uint64_t gl_frame_gate;
static uint64_t gl_swap_target;
static uint64_t gl_contract_stamp;
static void *gl_fn_egl_get_current_display;
static void *gl_fn_egl_get_current_surface;
static void *gl_fn_egl_get_current_context;
static void *gl_fn_egl_query_api;
static void *gl_fn_egl_query_surface;
static void *gl_fn_gl_get_integerv;
static void *gl_fn_gl_get_floatv;
static void *gl_fn_gl_get_booleanv;
static void *gl_fn_gl_is_enabled;
static void *gl_fn_gl_enable;
static void *gl_fn_gl_disable;
static void *gl_fn_gl_scissor;
static void *gl_fn_gl_clear_color;
static void *gl_fn_gl_clear;
static void *gl_fn_egl_swap_buffers;
static uint64_t gl_state_block[0x22];

static int gl_trylock(void)
{
    return !__atomic_test_and_set(&gl_install_lock, __ATOMIC_ACQUIRE);
}

static void gl_unlock(void)
{
    __atomic_clear(&gl_install_lock, __ATOMIC_RELEASE);
}

int nexus_visual_gl_install_v1(const uint32_t *contract)
{
    const uint32_t *h = contract;
    const uint64_t *d = (const uint64_t *)contract;
    int ok = 0;

    if (!gl_trylock()) {
        return 0;
    }
    gl_install_state = 0;
    gl_install_status = 0;
    gl_original_swap = NULL;
    gl_committed = 0;
    gl_install_state = 2;
    if (contract == NULL || h[0] != 1 || h[2] != 0x58) {
        goto out;
    }
    if ((void *)(uintptr_t)d[2] == NULL) {
        goto out;
    }
    if (strcmp((const char *)(uintptr_t)d[2], GL_BUILD_ID) != 0) {
        goto out;
    }
    if (strcmp((const char *)(uintptr_t)d[3], GL_IMAGE_ID) != 0 || d[7] == 0) {
        goto out;
    }
    if (!gl_license_ok()) {
        goto out;
    }
    if (gl_committed) {
        gl_install_state = 8;
        if (h[6] != gl_surface_w || h[7] != gl_surface_h ||
            d[4] != gl_surface_stamp) {
            goto out;
        }
        if (d[5] == gl_hook_table_stamp && d[6] == gl_hook_slot_addr &&
            d[9] == gl_game_context && d[7] == gl_frame_gate &&
            d[8] == gl_swap_target && d[7] == gl_contract_stamp) {
            gl_install_state = 7;
            if (gl_hook_slot_enable(gl_hook_slot) == 1) {
                gl_install_state = 1;
                ok = 1;
            }
        }
        goto out;
    }
    gl_install_state = 3;
    if (gl_egl_handle == NULL) {
        gl_egl_handle = dlopen("libEGL.so", RTLD_NOW);
    }
    if (gl_gles_handle == NULL) {
        gl_gles_handle = dlopen("libGLESv2.so", RTLD_NOW);
    }
    if (gl_egl_handle == NULL || gl_gles_handle == NULL) {
        goto out;
    }
    gl_install_state = 4;
    gl_fn_egl_get_current_display = gl_dlsym_lookup(gl_egl_handle, "eglGetCurrentDisplay");
    gl_fn_egl_get_current_surface = gl_dlsym_lookup(gl_egl_handle, "eglGetCurrentSurface");
    gl_fn_egl_get_current_context = gl_dlsym_lookup(gl_egl_handle, "eglGetCurrentContext");
    gl_fn_egl_query_api = gl_dlsym_lookup(gl_egl_handle, "eglQueryAPI");
    gl_fn_egl_query_surface = gl_dlsym_lookup(gl_egl_handle, "eglQuerySurface");
    gl_fn_gl_get_integerv = gl_dlsym_lookup(gl_gles_handle, "glGetIntegerv");
    gl_fn_gl_get_floatv = gl_dlsym_lookup(gl_gles_handle, "glGetFloatv");
    gl_fn_gl_get_booleanv = gl_dlsym_lookup(gl_gles_handle, "glGetBooleanv");
    gl_fn_gl_is_enabled = gl_dlsym_lookup(gl_gles_handle, "glIsEnabled");
    gl_fn_gl_enable = gl_dlsym_lookup(gl_gles_handle, "glEnable");
    gl_fn_gl_disable = gl_dlsym_lookup(gl_gles_handle, "glDisable");
    gl_fn_gl_scissor = gl_dlsym_lookup(gl_gles_handle, "glScissor");
    gl_fn_gl_clear_color = gl_dlsym_lookup(gl_gles_handle, "glClearColor");
    gl_fn_gl_clear = gl_dlsym_lookup(gl_gles_handle, "glClear");
    gl_fn_egl_swap_buffers = gl_dlsym_lookup(gl_egl_handle, "eglSwapBuffers");
    if (gl_fn_egl_swap_buffers == NULL) {
        goto out;
    }
    gl_install_state = 5;
    gl_original_swap = gl_fn_egl_swap_buffers;
    gl_surface_w = h[6];
    gl_surface_h = h[7];
    gl_surface_stamp = d[4];
    if (!gl_symbol_table_install(gl_symbol_table, contract + 6,
                                 &gl_fn_egl_get_current_display)) {
        goto out;
    }
    gl_install_state = 6;
    ((void (*)(void *, uint64_t, const void *, uint32_t))(uintptr_t)d[3])
        ((void *)(uintptr_t)d[2], d[5] + 0x123e0c8, &gl_game_swap_slot, 8);
    if (gl_hook_slot_commit(gl_hook_slot, contract, gl_original_swap,
                            (void *)gl_swap_trampoline)) {
        gl_committed = 1;
        gl_install_state = 7;
        if (gl_hook_slot_enable(gl_hook_slot) == 1) {
            gl_install_state = 1;
            ok = 1;
            goto out;
        }
        gl_symbol_table_teardown(gl_symbol_table);
    }
out:
    gl_install_status = gl_committed ? gl_install_status : 0;
    gl_unlock();
    return ok;
}

uint32_t nexus_visual_gl_hook_status_v1(void)
{
    uint32_t status;

    if (!gl_trylock()) {
        return 2;
    }
    status = gl_committed ? gl_install_status : 0;
    gl_unlock();
    return status;
}

int nexus_visual_gl_android_status_v1(void *desc)
{
    uint32_t status;

    if (desc == NULL) {
        return 0;
    }
    if (!gl_trylock()) {
        return 0;
    }
    memcpy(desc, gl_state_block, 0x110);
    status = gl_committed ? gl_install_status : 0;
    *(uint32_t *)((char *)desc + 0xc) = status;
    gl_unlock();
    return 1;
}

int nexus_visual_gl_remove_v1(void)
{
    int result;

    if (gl_committed != 1) {
        return 4;
    }
    gl_symbol_table_teardown(gl_symbol_table);
    if (!gl_trylock()) {
        return 2;
    }
    result = gl_hook_slot_unpatch(gl_hook_slot);
    gl_unlock();
    return result;
}

#define REMOTE_WRITE_SELF 0x7fff0001u
#define REMOTE_WRITE_RETRIES 8

typedef struct {
    const char *method;
    const char *op;
    const char *stage;
    long len;
    long result;
    long code;
    int err;
    int err2;
} remote_write_cell_t;

extern char remote_write_key[];
extern void *cell_lock(void *key);

static long syscall3(long nr, long a, long b, long c)
{
    return syscall(nr, a, b, c);
}

static long syscall4(long nr, long a, long b, long c, long d)
{
    return syscall(nr, a, b, c, d);
}

static long syscall6(long nr, long a, long b, long c, long d, long e, long f)
{
    return syscall(nr, a, b, c, d, e, f);
}

static long remote_pwrite(int fd, const void *buf, long len, uintptr_t addr)
{
    return syscall4(68, fd, (long)buf, len, (long)addr);
}

static long remote_pvwrite(pid_t pid, const void *buf, long len, uintptr_t addr)
{
    struct iovec local;
    struct iovec remote;

    local.iov_base = (void *)buf;
    local.iov_len = (size_t)len;
    remote.iov_base = (void *)addr;
    remote.iov_len = (size_t)len;
    return syscall6(271, pid, (long)&local, 1, (long)&remote, 1, 0);
}

static long remote_pvread(pid_t pid, void *buf, long len, uintptr_t addr)
{
    struct iovec local;
    struct iovec remote;

    local.iov_base = buf;
    local.iov_len = (size_t)len;
    remote.iov_base = (void *)addr;
    remote.iov_len = (size_t)len;
    return syscall6(270, pid, (long)&local, 1, (long)&remote, 1, 0);
}

long remote_write(uint32_t fd, const void *buf, long len, uintptr_t addr)
{
    remote_write_cell_t *cell = cell_lock(remote_write_key);
    long total = 0;
    long r;

    cell->result = 0;
    cell->code = 0;
    cell->method = (fd == REMOTE_WRITE_SELF) ? "vm" : "proc";
    cell->op = "write";
    cell->stage = "none";
    cell->len = len;
    cell->err = 0;

    for (int tries = 0; tries <= REMOTE_WRITE_RETRIES; tries++) {
        if (fd == REMOTE_WRITE_SELF)
            r = remote_pvwrite(getpid(), buf, len, addr);
        else
            r = remote_pwrite((int)fd, buf, len, addr);
        if (r >= 0) {
            cell->result = r;
            cell->err = 0;
            return r;
        }
        total = r;
        if (errno != EINTR)
            break;
    }
    cell->result = total;
    cell->err = errno;

    if (fd != REMOTE_WRITE_SELF || errno == EINTR)
        return total;

    cell->err2 = 0;
    cell->result = -1;
    cell->stage = "range";
    if (len == 4 && (addr & 3) == 0) {
        long page = sysconf(_SC_PAGESIZE);
        if (page == 0x4000 || page == 0x1000) {
            uintptr_t base = -(uintptr_t)page & addr;
            if (base <= UINT64_MAX - page) {
                FILE *f = NULL;

                for (int tries = 0; tries <= REMOTE_WRITE_RETRIES; tries++) {
                    f = fopen("/proc/self/maps", "r");
                    if (f != NULL)
                        break;
                    if (errno != EINTR)
                        break;
                }
                if (f == NULL) {
                    cell->result = -1;
                    cell->err2 = errno;
                    cell->stage = "maps";
                } else {
                    char line[512];
                    char perms[8];
                    uintptr_t start;
                    uintptr_t end;
                    int matched = 0;
                    int bad = 0;

                    for (int tries = 0; tries <= REMOTE_WRITE_RETRIES; tries++) {
                        errno = 0;
                        if (fgets(line, sizeof line, f) != NULL) {
                            if (sscanf(line, "%lx-%lx %4s", &start, &end, perms) == 3
                                && start <= base && base + page <= end
                                && perms[0] == 'r' && perms[1] == 'x' && perms[2] == 'p') {
                                matched = 1;
                                break;
                            }
                            bad = 0;
                            tries = -1;
                            continue;
                        }
                        if (ferror(f) && errno == EINTR) {
                            clearerr(f);
                            continue;
                        }
                        break;
                    }
                    fclose(f);
                    if (matched) {
                        uint8_t *staging = mmap(NULL, (size_t)page, PROT_READ | PROT_WRITE,
                                                MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
                        if ((long)staging < 0) {
                            cell->result = (long)staging;
                            cell->stage = "alloc";
                            cell->err2 = errno;
                        } else {
                            long got = -1;

                            for (int tries = 0; tries <= REMOTE_WRITE_RETRIES; tries++) {
                                got = remote_pvread(getpid(), staging, page, base);
                                if (got >= 0)
                                    break;
                                if (errno != EINTR)
                                    break;
                            }
                            if (got != page) {
                                cell->result = got;
                                cell->err2 = errno;
                                cell->stage = "copy";
                            } else {
                                *(uint32_t *)(staging + (addr - base)) = *(const uint32_t *)buf;
                                cache_flush(staging, staging + page);
                                long prot = syscall3(226, (long)staging, page, 5);
                                if (prot < 0 || prot != 0) {
                                    cell->result = prot;
                                    cell->err2 = errno;
                                    cell->stage = "seal";
                                } else {
                                    long moved = -1;

                                    for (int tries = 0; tries <= REMOTE_WRITE_RETRIES; tries++) {
                                        moved = syscall6(216, (long)staging, page, page, 3,
                                                         (long)base, 0);
                                        if (moved >= 0)
                                            break;
                                        if (errno != EINTR)
                                            break;
                                    }
                                    if (moved == (long)base) {
                                        cache_flush((void *)base, (void *)(base + page));
                                        cell->result = 4;
                                        cell->code = 0;
                                        cell->stage = "none";
                                        return 4;
                                    }
                                    cell->result = moved;
                                    cell->err2 = errno;
                                    cell->stage = "remap";
                                }
                            }
                            syscall3(215, (long)staging, page, 0);
                        }
                    } else {
                        cell->result = -1;
                        cell->err2 = bad ? errno : 0;
                        cell->stage = "maps";
                    }
                }
            }
        }
    }
    return total;
}

static const uint32_t chacha_seed[12] = {
    0x00000000u, 0x4047af50u, 0x2781a7a1u, 0x5e387615u,
    0x64f0f4f3u, 0x1857704au, 0xb29ffa12u, 0xdaf11772u,
    0xceef3e76u, 0x543246b4u, 0xd19e7de5u, 0xf07423d5u,
};

#define CHACHA_CONSTANT0 0x61707865u
#define CHACHA_CONSTANT1 0x3320646eu
#define CHACHA_CONSTANT2 0x79622d32u
#define CHACHA_CONSTANT3 0x6b206574u

static uint32_t chacha_rotl(uint32_t v, int n)
{
    return (v << n) | (v >> (32 - n));
}

static void chacha_quarter(uint32_t *s, int a, int b, int c, int d)
{
    s[a] += s[b];
    s[d] ^= s[a];
    s[d] = chacha_rotl(s[d], 16);
    s[c] += s[d];
    s[b] ^= s[c];
    s[b] = chacha_rotl(s[b], 12);
    s[a] += s[b];
    s[d] ^= s[a];
    s[d] = chacha_rotl(s[d], 8);
    s[c] += s[d];
    s[b] ^= s[c];
    s[b] = chacha_rotl(s[b], 7);
}

static void chacha20_xor(uint8_t *buf, size_t len)
{
    uint32_t state[16];
    uint32_t working[16];
    uint32_t counter = chacha_seed[0];
    size_t done = 0;

    if (len == 0)
        return;
    state[0] = CHACHA_CONSTANT0;
    state[1] = CHACHA_CONSTANT1;
    state[2] = CHACHA_CONSTANT2;
    state[3] = CHACHA_CONSTANT3;
    for (int i = 0; i < 12; i++)
        state[4 + i] = chacha_seed[i];
    do {
        uint8_t block[64];
        size_t chunk = len - done;

        if (chunk > 64)
            chunk = 64;
        state[4] = counter;
        for (int i = 0; i < 16; i++)
            working[i] = state[i];
        for (int round = 0; round < 10; round++) {
            chacha_quarter(working, 0, 4, 8, 12);
            chacha_quarter(working, 1, 5, 9, 13);
            chacha_quarter(working, 2, 6, 10, 14);
            chacha_quarter(working, 3, 7, 11, 15);
            chacha_quarter(working, 0, 5, 10, 15);
            chacha_quarter(working, 1, 6, 11, 12);
            chacha_quarter(working, 2, 7, 8, 13);
            chacha_quarter(working, 3, 4, 9, 14);
        }
        for (int i = 0; i < 16; i++) {
            uint32_t v = working[i] + state[i];
            block[i * 4 + 0] = (uint8_t)v;
            block[i * 4 + 1] = (uint8_t)(v >> 8);
            block[i * 4 + 2] = (uint8_t)(v >> 16);
            block[i * 4 + 3] = (uint8_t)(v >> 24);
        }
        for (size_t i = 0; i < chunk; i++)
            buf[done + i] ^= block[i];
        counter++;
        done += chunk;
    } while (done < len);
}

void nexus_crypt_profile_stream(uint8_t *buf, size_t len)
{
    chacha20_xor(buf, len);
}

void nexus_crypt_state_stream(uint8_t *buf, size_t len)
{
    chacha20_xor(buf, len);
}

static const uint32_t sha256_k[64] = {
    0x428a2f98u, 0x71374491u, 0xb5c0fbcfu, 0xe9b5dba5u,
    0x3956c25bu, 0x59f111f1u, 0x923f82a4u, 0xab1c5ed5u,
    0xd807aa98u, 0x12835b01u, 0x243185beu, 0x550c7dc3u,
    0x72be5d74u, 0x80deb1feu, 0x9bdc06a7u, 0xc19bf174u,
    0xe49b69c1u, 0xefbe4786u, 0x0fc19dc6u, 0x240ca1ccu,
    0x2de92c6fu, 0x4a7484aau, 0x5cb0a9dcu, 0x76f988dau,
    0x983e5152u, 0xa831c66du, 0xb00327c8u, 0xbf597fc7u,
    0xc6e00bf3u, 0xd5a79147u, 0x06ca6351u, 0x14292967u,
    0x27b70a85u, 0x2e1b2138u, 0x4d2c6dfcu, 0x53380d13u,
    0x650a7354u, 0x766a0abbu, 0x81c2c92eu, 0x92722c85u,
    0xa2bfe8a1u, 0xa81a664bu, 0xc24b8b70u, 0xc76c51a3u,
    0xd192e819u, 0xd6990624u, 0xf40e3585u, 0x106aa070u,
    0x19a4c116u, 0x1e376c08u, 0x2748774cu, 0x34b0bcb5u,
    0x391c0cb3u, 0x4ed8aa4au, 0x5b9cca4fu, 0x682e6ff3u,
    0x748f82eeu, 0x78a5636fu, 0x84c87814u, 0x8cc70208u,
    0x90befffau, 0xa4506cebu, 0xbef9a3f7u, 0xc67178f2u,
};

typedef struct {
    uint32_t h[8];
    uint64_t bytes;
    uint8_t block[64];
    size_t fill;
} sha256_ctx_t;

static void sha256_init(sha256_ctx_t *ctx)
{
    ctx->h[0] = 0x6a09e667u;
    ctx->h[1] = 0xbb67ae85u;
    ctx->h[2] = 0x3c6ef372u;
    ctx->h[3] = 0xa54ff53au;
    ctx->h[4] = 0x510e527fu;
    ctx->h[5] = 0x9b05688cu;
    ctx->h[6] = 0x1f83d9abu;
    ctx->h[7] = 0x5be0cd19u;
    ctx->bytes = 0;
    ctx->fill = 0;
}

static void sha256_compress(sha256_ctx_t *ctx, const uint8_t *p)
{
    uint32_t w[64];
    uint32_t a, b, c, d, e, f, g, h;

    for (int i = 0; i < 16; i++)
        w[i] = ((uint32_t)p[i * 4] << 24) | ((uint32_t)p[i * 4 + 1] << 16)
               | ((uint32_t)p[i * 4 + 2] << 8) | (uint32_t)p[i * 4 + 3];
    for (int i = 16; i < 64; i++) {
        uint32_t s0 = chacha_rotl(w[i - 15], 7) ^ chacha_rotl(w[i - 15], 18) ^ (w[i - 15] >> 3);
        uint32_t s1 = chacha_rotl(w[i - 2], 17) ^ chacha_rotl(w[i - 2], 19) ^ (w[i - 2] >> 10);
        w[i] = w[i - 16] + s0 + w[i - 7] + s1;
    }
    a = ctx->h[0]; b = ctx->h[1]; c = ctx->h[2]; d = ctx->h[3];
    e = ctx->h[4]; f = ctx->h[5]; g = ctx->h[6]; h = ctx->h[7];
    for (int i = 0; i < 64; i++) {
        uint32_t s1 = chacha_rotl(e, 6) ^ chacha_rotl(e, 11) ^ chacha_rotl(e, 25);
        uint32_t ch = (e & f) ^ (~e & g);
        uint32_t t1 = h + s1 + ch + sha256_k[i] + w[i];
        uint32_t s0 = chacha_rotl(a, 2) ^ chacha_rotl(a, 13) ^ chacha_rotl(a, 22);
        uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
        uint32_t t2 = s0 + maj;
        h = g; g = f; f = e; e = d + t1;
        d = c; c = b; b = a; a = t1 + t2;
    }
    ctx->h[0] += a; ctx->h[1] += b; ctx->h[2] += c; ctx->h[3] += d;
    ctx->h[4] += e; ctx->h[5] += f; ctx->h[6] += g; ctx->h[7] += h;
}

static void sha256_update(sha256_ctx_t *ctx, const void *data, size_t len)
{
    const uint8_t *p = data;

    ctx->bytes += len;
    while (len > 0) {
        size_t take = 64 - ctx->fill;

        if (take > len)
            take = len;
        memcpy(ctx->block + ctx->fill, p, take);
        ctx->fill += take;
        p += take;
        len -= take;
        if (ctx->fill == 64) {
            sha256_compress(ctx, ctx->block);
            ctx->fill = 0;
        }
    }
}

static void sha256_final(sha256_ctx_t *ctx, uint8_t out[32])
{
    uint64_t bits = ctx->bytes * 8;
    uint8_t pad = 0x80;

    sha256_update(ctx, &pad, 1);
    pad = 0;
    while (ctx->fill != 56)
        sha256_update(ctx, &pad, 1);
    uint8_t len_bytes[8];
    for (int i = 0; i < 8; i++)
        len_bytes[i] = (uint8_t)(bits >> (56 - i * 8));
    sha256_update(ctx, len_bytes, 8);
    for (int i = 0; i < 8; i++) {
        out[i * 4 + 0] = (uint8_t)(ctx->h[i] >> 24);
        out[i * 4 + 1] = (uint8_t)(ctx->h[i] >> 16);
        out[i * 4 + 2] = (uint8_t)(ctx->h[i] >> 8);
        out[i * 4 + 3] = (uint8_t)ctx->h[i];
    }
}

void nexus_sha256_clone_a_init(sha256_ctx_t *ctx)
{
    sha256_init(ctx);
}

void nexus_sha256_clone_a_final(sha256_ctx_t *ctx, uint8_t out[32])
{
    sha256_final(ctx, out);
}

void nexus_sha256_clone_b_init(sha256_ctx_t *ctx)
{
    sha256_init(ctx);
}

void nexus_sha256_clone_b_final(sha256_ctx_t *ctx, uint8_t out[32])
{
    sha256_final(ctx, out);
}

void nexus_sha256_clone_c_init(sha256_ctx_t *ctx)
{
    sha256_init(ctx);
}

void nexus_sha256_clone_c_final(sha256_ctx_t *ctx, uint8_t out[32])
{
    sha256_final(ctx, out);
}

static const uint8_t hmac_custom_ipad[16] = {
    0x7c, 0x4d, 0x1a, 0xab, 0xd7, 0xc5, 0x60, 0xbe,
    0xf2, 0xe3, 0x58, 0x39, 0x94, 0x85, 0x21, 0x0f,
};
static const uint8_t hmac_custom_opad[16] = {
    0x16, 0x27, 0x70, 0xc1, 0xbd, 0xaf, 0x0a, 0xd4,
    0x98, 0x89, 0x32, 0x53, 0xfe, 0xef, 0x4b, 0x65,
};

static void hmac_sha256(const uint8_t key[16], const void *msg, size_t len,
                        const uint8_t ipad_prefix[16], const uint8_t opad_prefix[16],
                        uint8_t out[32])
{
    uint8_t block[64];
    sha256_ctx_t ctx;

    for (int i = 0; i < 64; i++) {
        uint8_t k = (i < 16) ? key[i] : 0;
        uint8_t pad;

        if (i < 16 && ipad_prefix != NULL)
            pad = ipad_prefix[i];
        else
            pad = 0x36;
        block[i] = k ^ pad;
    }
    sha256_init(&ctx);
    sha256_update(&ctx, block, 64);
    sha256_update(&ctx, msg, len);
    sha256_final(&ctx, out);
    for (int i = 0; i < 64; i++) {
        uint8_t k = (i < 16) ? key[i] : 0;
        uint8_t pad;

        if (i < 16 && opad_prefix != NULL)
            pad = opad_prefix[i];
        else
            pad = 0x5c;
        block[i] = k ^ pad;
    }
    sha256_init(&ctx);
    sha256_update(&ctx, block, 64);
    sha256_update(&ctx, out, 32);
    sha256_final(&ctx, out);
}

#define IPC_MESSAGE_MAGIC 0x1c2d3e4fu
#define IPC_MESSAGE_BYTES 20

uint32_t nexus_ipc_message_id(const uint8_t key[16], void *message)
{
    uint8_t digest[32];
    uint32_t id;

    hmac_sha256(key, message, IPC_MESSAGE_BYTES, NULL, NULL, digest);
    if (digest[0] == 0 && digest[1] == 0) {
        id = 1;
    } else {
        id = (uint32_t)digest[0] | ((uint32_t)(digest[1] & 0x7f) << 8);
        if (id == 0)
            id = 1;
    }
    *(uint32_t *)((char *)message + 0x34) = id;
    return id;
}

uint32_t nexus_stable_id(const uint8_t key[16], const void *msg, size_t len)
{
    uint8_t digest[32];
    uint32_t id;

    hmac_sha256(key, msg, len, hmac_custom_ipad, hmac_custom_opad, digest);
    id = (uint32_t)digest[0] | ((uint32_t)(digest[1] & 0x7f) << 8);
    if (id == 0)
        id = 1;
    return id;
}

#define SDF_MAX_SAMPLES 130
#define SDF_MAX_ARCS 8
#define SDF_SHAPE_OFFSETS_BASE 0x87c
#define SDF_SHAPE_OFFSET_STRIDE 0x3c
#define SDF_CONTOUR_STRIDE 0x10
#define SDF_WORLD_LIMIT 40.0f
#define SDF_MARCH_RANGE_MAX 4.7f

typedef struct {
    float x;
    float y;
    float rmin;
    float rmax;
    float reserved[3];
    float count;
    float rounding;
    float contours[8][4];
} sdf_shape_t;

typedef struct {
    uint32_t error;
    uint8_t pad[0x51d4];
    void *callbacks;
} sdf_march_ctx_t;

extern int sdf_ray_step_validate(float x, float y, float dx, float dy, float step,
                                 float remaining, float out[4]);
extern float sdf_contour_distance(const float *contour, const float *sample, uint32_t kind);
extern int sdf_ray_march(uint32_t a, uint32_t b, float dx, float dy, float range,
                         float arcs, sdf_march_ctx_t *ctx, void *out, uint32_t *mask);

static int sdf_value_ok(float v, float limit)
{
    return fabsf(v) != INFINITY && !isnan(v) && v <= limit;
}

int sdf_angular_coverage(const void *shape_owner, const void *shape_table, float step,
                         sdf_march_ctx_t *ctx, const sdf_shape_t *shape, long offsets,
                         uint32_t sample_count, uint32_t arc_limit, uint32_t forced_index,
                         int forced_valid, uint8_t out_slots[SDF_MAX_ARCS][16])
{
    (void)shape_owner;
    (void)shape_table;
    float distances[SDF_MAX_SAMPLES];
    uint8_t inside[SDF_MAX_SAMPLES];
    float sample[4];
    uint32_t best_index = 0xffffffffu;
    float best_distance = -1e30f;
    uint32_t marked_count;
    uint32_t farthest;
    uint32_t i;

    if (sample_count <= arc_limit - 1)
        return 0;
    if (out_slots == NULL)
        return 0;
    for (i = 0; i < SDF_MAX_ARCS; i++) {
        *(uint64_t *)&out_slots[i][0] = 0;
        *(uint64_t *)&out_slots[i][8] = 0;
    }
    if (arc_limit == sample_count) {
        memset(out_slots, 1, arc_limit);
        return 1;
    }

    memset(distances, 0, sizeof distances);
    memset(inside, 0, sizeof inside);

    for (i = 0; i < sample_count; i++) {
        float angle = ((float)i * 6.2831855f) / (float)sample_count;
        uint32_t all_inside = 1;
        float nearest = 1e30f;
        uint32_t contour_count;
        uint32_t c;

        sincosf(angle, &sample[1], &sample[0]);
        sample[2] = 0.0f;
        sample[3] = 0.0f;
        if (shape == NULL || offsets == 0 || shape->count == 0.0f)
            continue;
        contour_count = (uint32_t)shape->count;
        for (c = 0; c < contour_count; c++) {
            const float *offsets_row =
                (const float *)((const char *)offsets + SDF_SHAPE_OFFSETS_BASE)
                + (size_t)c * (SDF_SHAPE_OFFSET_STRIDE / 4);
            float step_out[4] = {0};
            float march_out[2];
            uint32_t march_mask = 0;
            float d;
            float rounding = 0.0f;

            if (step < 0.0f
                || !sdf_ray_step_validate(sample[0], sample[1], sample[2], sample[3],
                                          step, 0.0f, step_out)) {
                c = contour_count;
                all_inside = 0;
                break;
            }
            d = sdf_contour_distance(&shape->contours[c][0], step_out, 0);
            if (shape->rounding != 0.0f && contour_count > 1) {
                float span = hypotf(shape->x - shape->rmin, shape->y - shape->rmax);
                float r = shape->rmin;

                if (r <= shape->rmax)
                    r = shape->rmax;
                if (r > 0.0f)
                    r = 0.0f;
                rounding = span * 0.5f * r;
            }
            d = d + rounding + *offsets_row;
            if (!sdf_value_ok(d, SDF_WORLD_LIMIT) || d < 0.0f) {
                ctx->error = 1;
                return 0;
            }
            if (sdf_ray_march(0, 0, sample[0], sample[1], d, (float)contour_count,
                              ctx, march_out, &march_mask) != 0) {
                all_inside &= march_mask;
                if (march_out[1] < nearest)
                    nearest = march_out[1];
            } else {
                if (ctx->error != 0)
                    return 0;
                all_inside = 0;
            }
        }
        if (all_inside != 0) {
            distances[i] = nearest;
            if (nearest != best_distance
                && (nearest < best_distance) == (isnan(nearest) || isnan(best_distance))) {
                best_index = i;
                best_distance = nearest;
            }
            inside[i] = 1;
        }
    }

    marked_count = 0;
    if ((int)forced_index >= 0 && forced_index < sample_count && arc_limit != 0
        && out_slots[0][forced_index] == 0) {
        marked_count = 1;
        out_slots[0][forced_index] = 1;
    }
    if ((int)best_index >= 0 && best_index < sample_count && marked_count < arc_limit
        && out_slots[0][best_index] == 0) {
        marked_count++;
        out_slots[0][best_index] = 1;
    }

    farthest = 0xffffffffu;
    if ((int)best_index >= 0 && sample_count != 0) {
        float far_best = -1e30f;
        uint32_t walk = best_index;

        for (i = 0; i < sample_count; i++) {
            if (inside[i] != 0) {
                uint32_t span = (i <= best_index) ? walk : (i - best_index);

                if (sample_count - span <= span)
                    span = sample_count - span;
                if (span > 1 && distances[i] > far_best) {
                    farthest = i;
                    far_best = distances[i];
                }
            }
            walk--;
        }
    }
    if (arc_limit > 3 && (int)farthest >= 0 && farthest < sample_count
        && marked_count < arc_limit && out_slots[0][farthest] == 0) {
        marked_count++;
        out_slots[0][farthest] = 1;
    }

    if ((int)forced_index >= 0 && forced_valid != 0) {
        uint32_t next;
        uint32_t prev;

        if (marked_count + 1 < arc_limit) {
            next = (forced_index + 1) - (forced_index + 1) / sample_count * sample_count;
            if (next < sample_count && out_slots[0][next] == 0 && marked_count < arc_limit) {
                out_slots[0][next] = 1;
                marked_count++;
            }
        }
        if (marked_count + 1 < arc_limit) {
            prev = (sample_count + forced_index - 1)
                   - (sample_count + forced_index - 1) / sample_count * sample_count;
            if ((int)prev >= 0 && prev < sample_count && out_slots[0][prev] == 0
                && marked_count < arc_limit) {
                out_slots[0][prev] = 1;
                marked_count++;
            }
        }
    }

    while (marked_count < arc_limit) {
        int best = -1;
        float best_span = -1e30f;

        for (uint32_t cand = 0; cand < sample_count; cand++) {
            float span = (float)sample_count;

            if (out_slots[0][cand] != 0)
                continue;
            for (uint32_t ref = 0; ref < sample_count; ref++) {
                float d;

                if (out_slots[0][ref] == 0)
                    continue;
                if (cand > ref) {
                    d = (float)(cand - ref);
                } else {
                    d = (float)((int)cand - (int)sample_count) + (float)ref;
                }
                if ((float)sample_count - d <= d)
                    d = (float)sample_count - d;
                if (d < span)
                    span = d;
            }
            if (span > best_span) {
                best_span = span;
                best = (int)cand;
            }
        }
        if (best < 0)
            break;
        out_slots[0][best] = 1;
        marked_count++;
    }
    return 1;
}

#define OVERLAY_MAX_MODE1_FRAMES 600
#define OVERLAY_PERF_WINDOW 120
#define OVERLAY_HIST_CAP 90
#define OVERLAY_COORD_DIV 300.0f

typedef void *(*overlay_egl_fn)(void);
typedef long (*overlay_game_getter)(void *handle);

typedef int (*overlay_game_request)(void *handle, int mode, void *header, long size);
typedef int (*overlay_remote_read)(void *handle, uintptr_t addr, void *out, uint32_t len);

typedef struct {
    overlay_egl_fn egl_get_current_display;
    overlay_egl_fn egl_get_current_surface;
    overlay_egl_fn egl_get_current_context;
    overlay_egl_fn egl_query_api;
    void *egl_query_surface;
    void *gl_get_integerv;
    void *gl_get_floatv;
    void *gl_get_booleanv;
    void *gl_is_enabled;
    void *gl_enable;
    void *gl_disable;
    void *gl_scissor;
    void *gl_clear_color;
    void *gl_clear;
    void *game_handle;
    void *game_base;
    overlay_remote_read remote_read;
    overlay_game_getter game_get_frame;
    overlay_game_getter game_get_echo;
    overlay_game_request game_request;
    uint8_t snapshot_busy;
    uint8_t rendering;
    uint8_t stats_busy;
    uint8_t pad1[0xa54 - 0xab];
    int mode1_frames;
    int master_enable;
    int state_corrupt;
    uint8_t pad2[4];
    uint8_t template_snapshot[0x880];
    uint8_t tail[0xa58 - 0xb0 - 0x880];
} overlay_ctx_t;

#define OV_MODE(p) (*(uint32_t *)((char *)(p) + 0xa28))
#define OV_FRAME_CAP(p) (*(uint64_t *)((char *)(p) + 0xa30))
#define OV_SURF_W(p) (*(uint64_t *)((char *)(p) + 0xa20))
#define OV_SURF_H(p) (*(uint64_t *)((char *)(p) + 0xa24))
#define OV_FRAMES(p) (*(uint64_t *)((char *)(p) + 0x9b0))
#define OV_META(p) ((uint8_t *)(p) + 0x930)

typedef struct {
    int vp_x;
    int vp_y;
    int vp_w;
    int vp_h;
    float color[4];
    int count;
    uint32_t tag;
    uint32_t flags;
    int ok;
    uint8_t rect_data[8 * 64];
    int rect_count;
} hud_cursor_t;

extern int hud_template_validate(const void *tpl, long session, long frame);
extern int hud_waypoint_validate(const void *tpl, long session, long frame);
extern void hud_emit_rect(hud_cursor_t *cur, int x, int y, int w, int h, int layer);
extern void hud_emit_line(hud_cursor_t *cur, int x0, int y0, int x1, int y1, int thick);
extern void hud_emit_label(hud_cursor_t *cur, const char *text, int x, int y);
extern void hud_emit_value(hud_cursor_t *cur, int value, int x, int y);
extern int hud_build_live(hud_cursor_t *cur, int *count);
int hud_build_snapshot(int *tpl, int *meta, long frame_val, uint32_t flags,
                       hud_cursor_t *cur, int *count_out);
void hud_build_status(hud_cursor_t *cur, int *tpl, int *meta, uint64_t frame,
                      uint32_t flags);
extern void overlay_stats_publish(overlay_ctx_t *ctx, uint32_t result, uint32_t mode,
                                  int rect_total, long frame_val, long echo_val,
                                  void *dpy, void *surface, const int viewport[4]);
extern int overlay_query_surface(overlay_ctx_t *ctx, void *dpy, void *surface,
                                 uint32_t key, int *out);
extern int overlay_save_gl_state(overlay_ctx_t *ctx, int viewport[4], int scissor[4],
                                 float blend[4], uint8_t writemask[4], int *fb,
                                 int *scissor_enabled);
extern void overlay_gl_enable_scissor(overlay_ctx_t *ctx);
extern int overlay_draw_rects(overlay_ctx_t *ctx, hud_cursor_t *cur);
extern void overlay_gl_restore(overlay_ctx_t *ctx, const float blend[4],
                               const int scissor[4], int scissor_enabled);
extern int overlay_verify_gl_state(overlay_ctx_t *ctx, const float blend[4],
                                   const int scissor[4], const uint8_t writemask[4],
                                   int fb, int scissor_enabled);
extern void hud_build_extra(hud_cursor_t *cur, const void *tpl, const void *meta,
                            uint32_t flags);
extern void grid_emit_line(int x0, int y0, int x1, int y1, hud_cursor_t *cur,
                           const void *tpl);

static uint64_t perf_pre_us_total;
static uint64_t perf_geometry_us_total;
static uint64_t perf_gl_us_total;
static uint64_t perf_restore_us_total;
static uint64_t perf_rects_total;
static uint64_t perf_frames;
static uint64_t hist_session;
static uint64_t hist_last_frame;
static uint64_t hist_packed;
static float hist_ring[OVERLAY_HIST_CAP];

static uint64_t now_us(void)
{
    struct timespec ts;

    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
        return 0;
    return (uint64_t)ts.tv_sec * 1000000ull + (uint64_t)ts.tv_nsec / 1000ull;
}

static int overlay_surface_size_ok(int w, int h)
{
    const int32_t bounds[4] = {0, 0, 0x2000, 0x2000};

    return w >= bounds[2] && h >= bounds[3] && w <= bounds[0] + 0x10000
           && h <= bounds[1] + 0x10000;
}

int nexus_overlay_render_frame(overlay_ctx_t *ctx, void *dpy, void *surface, void *token)
{
    uint64_t t0;
    uint64_t t1 = 0;
    uint64_t t2 = 0;
    uint64_t t3 = 0;
    uint64_t t4 = 0;
    uint32_t result = 0xb;
    uint32_t mode;
    uint32_t flags = 0;
    long frame_val;
    long echo_val = 0;
    int rect_total = 0;
    int viewport[4];
    int scissor[4];
    float blend[4];
    uint8_t writemask[4];
    int fb_binding;
    int scissor_enabled;
    hud_cursor_t cursor;

    if (ctx == NULL || ctx->master_enable == 0)
        return 10;
    if (__atomic_exchange_n(&ctx->rendering, 1, __ATOMIC_ACQ_REL) != 0)
        return 8;
    t0 = now_us();
    mode = OV_MODE(ctx);
    if (ctx->state_corrupt != 0) {
        result = 9;
        goto done;
    }
    if ((char *)ctx->game_base + 0x7316b8 != token) {
        result = 2;
        goto done;
    }
    frame_val = ctx->game_get_frame(ctx->game_handle);
    echo_val = ctx->game_get_echo(ctx->game_handle);
    if (frame_val < 0 || dpy == NULL || surface == NULL || echo_val == 0) {
        result = 3;
        goto done;
    }

    if (mode == 2) {
        uint8_t header[24];
        long checksum;
        int st;

        if (__atomic_exchange_n(&ctx->snapshot_busy, 1, __ATOMIC_ACQ_REL) == 0) {
            memcpy(ctx->template_snapshot, (char *)ctx + 0xb0, 0x880);
            __atomic_clear(&ctx->snapshot_busy, __ATOMIC_RELEASE);
        }
        memcpy(header, (char *)ctx + 0x930 + 24, 24);
        if (!hud_template_validate(ctx->template_snapshot, *(long *)header, frame_val)) {
            result = 7;
            goto done;
        }
        checksum = *(long *)((char *)ctx + 0x938);
        if (checksum != *(long *)((char *)ctx + 0x938)) {
            result = 7;
            goto done;
        }
        st = ctx->game_request(ctx->game_handle, 2, header, checksum);
        flags = (uint32_t)st & 3;
        if (flags == 0) {
            result = 6;
            goto done;
        }
    } else {
        if ((uint64_t)frame_val >= OV_FRAME_CAP(ctx)
            || (uint32_t)ctx->mode1_frames >= OVERLAY_MAX_MODE1_FRAMES) {
            result = 0xb;
            goto done;
        }
        if (ctx->game_request(ctx->game_handle, 1, NULL, 0) != 1) {
            result = 6;
            goto done;
        }
    }

    {
        uint64_t game_dpy = 0;
        uint64_t game_surf = 0;

        if (ctx->remote_read(ctx->game_handle, (uintptr_t)ctx->game_base + 0x12fd788,
                             &game_dpy, 8) != 1
            || ctx->remote_read(ctx->game_handle, (uintptr_t)ctx->game_base + 0x12fd790,
                                &game_surf, 8) != 1
            || game_dpy != (uint64_t)dpy || game_surf != (uint64_t)surface) {
            result = 4;
            goto done;
        }
    }
    if (ctx->egl_get_current_display() != dpy
        || ctx->egl_get_current_surface() != surface) {
        result = 3;
        goto done;
    }
    {
        int w = 0;
        int h = 0;

        if (!overlay_query_surface(ctx, dpy, surface, 0x3057, &w)
            || !overlay_query_surface(ctx, dpy, surface, 0x3056, &h)
            || !overlay_surface_size_ok(w, h)) {
            result = 5;
            goto done;
        }
        OV_SURF_W(ctx) = (uint64_t)w;
        OV_SURF_H(ctx) = (uint64_t)h;
    }
    if (!overlay_save_gl_state(ctx, viewport, scissor, blend, writemask, &fb_binding,
                               &scissor_enabled)) {
        result = 5;
        goto done;
    }
    if (viewport[0] < 0 || viewport[1] < 0 || viewport[2] <= 0 || viewport[3] <= 0
        || viewport[0] > (int)OV_SURF_W(ctx) - viewport[2]
        || viewport[1] + viewport[3] > (int)OV_SURF_H(ctx)
        || scissor[0] == INT32_MIN || scissor[1] == INT32_MIN
        || scissor[2] < 0 || scissor[3] < 0
        || !isfinite(blend[0]) || !isfinite(blend[1]) || !isfinite(blend[2])
        || !isfinite(blend[3])) {
        result = 5;
        goto done;
    }
    t1 = now_us();

    memset(&cursor, 0, sizeof cursor);
    cursor.vp_x = viewport[0];
    cursor.vp_y = viewport[1];
    cursor.vp_w = viewport[2];
    cursor.vp_h = viewport[3];
    cursor.ok = 1;
    if (mode == 2) {
        if (!hud_build_snapshot((int *)ctx->template_snapshot, (int *)OV_META(ctx),
                                frame_val, flags, &cursor, &cursor.rect_count)) {
            result = 7;
            goto done;
        }
    } else {
        if (!hud_build_live(&cursor, &cursor.rect_count)) {
            result = 5;
            goto done;
        }
    }
    if (cursor.rect_count != 0) {
        int st = ctx->game_request(ctx->game_handle, (int)mode, NULL, 0);
        int expected = (mode == 1) ? 1 : (int)flags;

        if (st != expected || OV_MODE(ctx) != mode) {
            result = 0xb;
            goto done;
        }
        frame_val = ctx->game_get_frame(ctx->game_handle);
        if (frame_val < 0 || (mode == 1 && (uint64_t)frame_val >= OV_FRAME_CAP(ctx))) {
            result = 0xb;
            goto done;
        }
        if (ctx->egl_get_current_display() != dpy
            || ctx->egl_get_current_surface() != surface) {
            result = 3;
            goto done;
        }
        t2 = now_us();
        overlay_gl_enable_scissor(ctx);
        rect_total = overlay_draw_rects(ctx, &cursor);
        t3 = now_us();
        if (ctx->egl_get_current_display() != dpy
            || ctx->egl_get_current_surface() != surface) {
            ctx->state_corrupt = 1;
            result = 9;
            goto done;
        }
        overlay_gl_restore(ctx, blend, scissor, scissor_enabled);
        t4 = now_us();
        if ((OV_FRAMES(ctx) & 0xf) == 0) {
            if (!overlay_verify_gl_state(ctx, blend, scissor, writemask, fb_binding,
                                         scissor_enabled)) {
                ctx->state_corrupt = 1;
                result = 9;
                goto done;
            }
        }
        if (mode == 1)
            ctx->mode1_frames++;
        result = 1;
    } else {
        result = 1;
    }

done:
    if (result == 1 && t1 != 0 && t2 != 0 && t3 != 0 && t4 != 0) {
        perf_pre_us_total += t1 - t0;
        perf_geometry_us_total += t2 - t1;
        perf_gl_us_total += t3 - t2;
        perf_restore_us_total += t4 - t3;
        perf_rects_total += (uint64_t)rect_total;
        perf_frames++;
        if (perf_frames > OVERLAY_PERF_WINDOW - 1) {
            __android_log_print(4, "NexusPerfGL",
                                "frames=%u pre_us=%llu geometry_us=%llu gl_us=%llu "
                                "restore_us=%llu rects=%llu",
                                (unsigned)perf_frames,
                                (unsigned long long)(perf_pre_us_total / perf_frames),
                                (unsigned long long)(perf_geometry_us_total / perf_frames),
                                (unsigned long long)(perf_gl_us_total / perf_frames),
                                (unsigned long long)(perf_restore_us_total / perf_frames),
                                (unsigned long long)(perf_rects_total / perf_frames));
            perf_pre_us_total = 0;
            perf_geometry_us_total = 0;
            perf_gl_us_total = 0;
            perf_restore_us_total = 0;
            perf_rects_total = 0;
            perf_frames = 0;
        }
    }
    if (__atomic_exchange_n(&ctx->stats_busy, 1, __ATOMIC_ACQ_REL) == 0) {
        overlay_stats_publish(ctx, result, mode, rect_total, frame_val, echo_val,
                              dpy, surface, viewport);
        __atomic_clear(&ctx->stats_busy, __ATOMIC_RELEASE);
    }
    __atomic_clear(&ctx->rendering, __ATOMIC_RELEASE);
    return (int)result;
}

int hud_build_snapshot(int *tpl, int *meta, long frame_val, uint32_t flags,
                       hud_cursor_t *cur, int *count_out)
{
    int vp_x = cur->vp_x;
    int vp_y = cur->vp_y;
    int vp_w = cur->vp_w;
    int vp_h = cur->vp_h;
    float world_w;
    float world_h;
    int count;
    int ok = 1;

    if (count_out == NULL)
        return 0;
    *count_out = 0;
    if (tpl == NULL || meta == NULL || cur == NULL)
        return 0;
    if (vp_x < 0 || vp_y < 0 || vp_w < 1 || vp_h < 1)
        return 0;
    if (vp_x > 0x2000 || vp_y > 0x2000 || vp_w > 0x2000 || vp_h > 0x2000)
        return 0;
    if (frame_val < 0)
        return 0;
    if (!hud_template_validate(tpl, *(long *)(tpl + 0x34), frame_val))
        return 0;
    if (*(long *)(meta + 2) != *(long *)(tpl + 0x38))
        return 0;
    cur->tag = 2;
    cur->flags = flags & 3;
    world_w = *(float *)((char *)tpl + 0xf8);
    world_h = *(float *)((char *)tpl + 0xfc);
    if (world_w <= 0.0f || world_h <= 0.0f)
        return 0;

    if ((flags & 1) != 0 && meta[8] == 1) {
        int thick = (meta[13] >> 2) & 0x3fff;
        float scale = *(float *)(meta + 12);
        int size;
        int entries = *(int *)((char *)tpl + 0x11c);

        thick = thick / 0x19;
        if (thick < 2)
            thick = 2;
        if (thick > 7)
            thick = 7;
        size = (int)((scale / 100.0f) * (float)vp_w * 0.038f);
        if (size < 0x16)
            size = 0x16;
        if (size > 0x5a)
            size = 0x5a;
        for (int k = 0; k < entries; k++) {
            float type_bits = *(float *)((char *)tpl + 0x140 + (size_t)k * 0x48);
            float x_norm = *(float *)((char *)tpl + 0x150 + (size_t)k * 0x48);
            float y_norm = *(float *)((char *)tpl + 0x154 + (size_t)k * 0x48);
            int sel;
            int sx;
            int sy;
            int inner;
            uint32_t color_rb;
            uint32_t color_ga;

            if (type_bits == 2.8026e-45f)
                sel = meta[11];
            else if (type_bits == 1.4013e-45f)
                sel = meta[10];
            else if (type_bits == 0.0f)
                sel = meta[9];
            else
                continue;
            if (sel != 1)
                continue;
            sx = vp_x + (int)((x_norm * (float)vp_w) / world_w);
            sy = (vp_h + vp_y) - (size / 2) - (int)((y_norm * (float)vp_h) / world_h);
            inner = size - 2 * thick;
            if (type_bits == 2.8026e-45f) {
                color_rb = 0x3e800000u;
                color_ga = *(uint32_t *)(void *)0;
            } else if (type_bits == 1.4013e-45f) {
                color_rb = 0x3f800000u;
            } else {
                color_rb = 0x3e3851ecu;
            }
            (void)color_ga;
            cur->color[0] = *(float *)&color_rb;
            cur->color[3] = 1.0f;
            hud_emit_rect(cur, sx, sy, size, thick, 0);
            hud_emit_rect(cur, sx, sy + size - thick, size, thick, 0);
            hud_emit_rect(cur, sx, sy + thick, thick, inner, 0);
            hud_emit_rect(cur, sx + size - thick, sy + thick, thick, inner, 0);
            if (meta[14] == 1)
                hud_emit_rect(cur, sx + size / 2 - thick / 2, vp_y, thick, sy - vp_y, 0);
        }
    }

    if ((flags & 2) != 0 && meta[4] == 1
        && hud_waypoint_validate(tpl, *(long *)(tpl + 0x34), frame_val)) {
        float ax_norm = *(float *)((char *)tpl + 0x808);
        float ay_norm = *(float *)((char *)tpl + 0x80c);
        int arrow = *(int *)((char *)tpl + 0x804);
        int style = *(int *)((char *)tpl + 0x7fc);
        int value = *(int *)((char *)tpl + 0x7f8);
        int ax = vp_x + (int)((ax_norm * (float)vp_w) / world_w);
        int ay = (vp_h + vp_y) - (int)((ay_norm * (float)vp_h) / world_h);
        int size = (int)(((float)vp_w * 30.0f) / world_w);
        int dy;

        if (size < 0x16)
            size = 0x16;
        if (size > 0x3c)
            size = 0x3c;
        if (style == 1) {
            int outer = 2 * size;

            hud_emit_rect(cur, ax - outer / 2, ay - outer / 2, outer, 4, 0);
            hud_emit_rect(cur, ax - outer / 2, ay + outer / 2 - 4, outer, 4, 0);
            hud_emit_rect(cur, ax - outer / 2, ay - outer / 2, 4, outer, 0);
            hud_emit_rect(cur, ax + outer / 2 - 4, ay - outer / 2, 4, outer, 0);
        } else {
            int leg = (size * 3) / 4;

            hud_emit_rect(cur, ax - size, ay - size, leg, 4, 0);
            hud_emit_rect(cur, ax + size - leg, ay - size, leg, 4, 0);
            hud_emit_rect(cur, ax - size, ay - size, 4, leg, 0);
            hud_emit_rect(cur, ax - size, ay + size - leg, 4, leg, 0);
            hud_emit_rect(cur, ax - size, ay + size - 4, leg, 4, 0);
            hud_emit_rect(cur, ax + size - leg, ay + size - 4, leg, 4, 0);
            hud_emit_rect(cur, ax + size - 4, ay - size, 4, leg, 0);
            hud_emit_rect(cur, ax + size - 4, ay + size - leg, 4, leg, 0);
        }
        {
            int c1 = (size & 0xff) / 3;
            int c2 = ((size & 0x7f) * 2) / 3;

            hud_emit_rect(cur, ax - c1, ay - 2, c2, 4, 0);
            hud_emit_rect(cur, ax - 2, ay - c1, 4, c2, 0);
        }
        if (arrow == 1) {
            hud_emit_line(cur, ax, ay, ax + 20, ay + 12, 4);
            hud_emit_line(cur, ax, ay, ax + 20, ay - 12, 4);
        } else if (arrow == 2) {
            hud_emit_line(cur, ax, ay, ax - 20, ay + 12, 4);
            hud_emit_line(cur, ax, ay, ax - 20, ay - 12, 4);
        } else if (arrow == 3) {
            hud_emit_line(cur, ax, ay, ax + 12, ay - 20, 4);
            hud_emit_line(cur, ax, ay, ax - 12, ay - 20, 4);
        } else if (arrow == 4) {
            hud_emit_line(cur, ax, ay, ax + 12, ay + 20, 4);
            hud_emit_line(cur, ax, ay, ax - 12, ay + 20, 4);
        }
        dy = (arrow == 3) ? (-0xf - size) : (size + 5);
        hud_emit_value(cur, value, ax, ay + dy);
    }

    hud_build_status(cur, tpl, meta, (uint64_t)frame_val, flags);

    if ((flags & 1) != 0) {
        char buf[0x20];

        if (meta[1] != 0) {
            int dps = *(int *)((char *)tpl + 4);

            snprintf(buf, sizeof buf, "DPS %d", dps);
            cur->color[0] = 0.025f;
            cur->color[1] = 0.025f;
            cur->color[2] = 0.8f;
            cur->color[3] = 1.0f;
            hud_emit_rect(cur, vp_x + 0xe, vp_y + vp_h - 0xb6, 0xbe, 0x17, 1);
            cur->color[0] = (dps > 1000) ? 1.0f : 0.8f;
            cur->color[1] = 0.8f;
            cur->color[2] = 0.8f;
            hud_emit_label(cur, buf, vp_x + 0x14, vp_y + vp_h - 0xb2);
        }
        if (meta[0] != 0 && *(int *)tpl > 0) {
            int ping = *(int *)tpl;

            snprintf(buf, sizeof buf, "%d MS", ping);
            cur->color[0] = 0.025f;
            cur->color[1] = 0.025f;
            cur->color[2] = 0.035f;
            cur->color[3] = 1.0f;
            hud_emit_rect(cur, vp_x + vp_w - 0x9c, vp_y + vp_h - 0x2e, 0x8c, 0x17, 1);
            cur->color[0] = (ping > 100) ? 1.0f : 0.2f;
            cur->color[1] = (ping > 100) ? 0.2f : 1.0f;
            cur->color[2] = 0.2f;
            cur->color[3] = 1.0f;
            hud_emit_label(cur, buf, vp_x + vp_w - 0x96, vp_y + vp_h - 0x2a);
        }
    }
    hud_build_extra(cur, tpl, meta, flags);

    count = cur->rect_count;
    if (cur->ok == 0) {
        *count_out = 0;
        return 1;
    }
    *count_out = count;
    return ok;
}

void hud_build_status(hud_cursor_t *cur, int *tpl, int *meta, uint64_t frame, uint32_t flags)
{
    int vp_x = cur->vp_x;
    (void)flags;
    int vp_y = cur->vp_y;
    int vp_w = cur->vp_w;
    int vp_h = cur->vp_h;
    float world_w = *(float *)((char *)tpl + 0xf8);
    float world_h = *(float *)((char *)tpl + 0xfc);

    if (meta[25] != 0 && *(int *)((char *)tpl + 0x820) != 0) {
        int cols = *(int *)((char *)tpl + 8);
        int rows = *(int *)((char *)tpl + 0xc);

        if (cols >= 1 && cols <= 100 && rows >= 1 && rows <= 100) {
            for (int i = 0; i < cols; i++)
                grid_emit_line(i, 0, i, rows, cur, tpl);
            for (int j = 0; j < rows; j++)
                grid_emit_line(0, j, cols, j, cur, tpl);
        }
    }

    if (*(int *)(meta + 0x18) != 0) {
        int count = *(int *)((char *)tpl + 0x18);
        float pulse_s = sinf((float)(frame % 600) * 6.2831855f / 600.0f);
        float u = 0.5f + 0.5f * pulse_s;
        float pulse = 0.4f + 0.6f * u;

        if (count > 8)
            count = 8;
        for (int k = 0; k < count; k++) {
            float x = *(float *)((char *)tpl + 0x1c + (size_t)k * 20);
            float y = *(float *)((char *)tpl + 0x20 + (size_t)k * 20);
            float progress = *(float *)((char *)tpl + 0x24 + (size_t)k * 20);
            int state = *(int *)((char *)tpl + 0x28 + (size_t)k * 20);
            int sx = vp_x + (int)((x * (float)vp_w) / world_w);
            int sy = (vp_h + vp_y) - (int)((y * (float)vp_h) / world_h);
            int ny = (state != 0) ? sy + 0x2c : sy;
            float r;
            float g;

            cur->color[0] = 0.025f;
            cur->color[1] = 0.025f;
            cur->color[2] = 0.8f;
            cur->color[3] = 1.0f;
            hud_emit_rect(cur, sx - 0x1d, ny - 4, 58, 10, 1);
            if (progress <= 0.25f) {
                r = pulse;
                g = 0.0f;
            } else {
                float t = (progress - 0.25f) / 0.75f;

                if (t <= 0.5f) {
                    r = 1.0f;
                    g = 2.0f * t;
                } else {
                    r = 2.0f * (1.0f - t);
                    g = 1.0f;
                }
            }
            cur->color[0] = r;
            cur->color[1] = g;
            cur->color[2] = 0.0f;
            cur->color[3] = 1.0f;
            hud_emit_rect(cur, sx - 0x1a, ny - 1, (int)(progress * 52.0f), 4, 1);
            if (state == 1) {
                hud_emit_line(cur, sx - 0x10, ny + 9, sx - 7, ny + 0x17, 2);
                hud_emit_line(cur, sx - 0x10, ny + 9, sx - 7, ny + 1, 2);
            } else if (state == 2) {
                hud_emit_line(cur, sx + 0x10, ny + 9, sx + 7, ny + 0x17, 2);
                hud_emit_line(cur, sx + 0x10, ny + 9, sx + 7, ny + 1, 2);
            } else if (state == 3) {
                hud_emit_line(cur, sx - 7, ny + 1, sx, ny + 9, 2);
                hud_emit_line(cur, sx + 7, ny + 1, sx, ny + 9, 2);
            } else if (state == 4) {
                hud_emit_line(cur, sx - 7, ny + 0x17, sx, ny + 9, 2);
                hud_emit_line(cur, sx + 7, ny + 0x17, sx, ny + 9, 2);
            }
        }
    }

    {
        int fps_gate = *(int *)((char *)meta + 0x68);
        int graph_gate = *(int *)((char *)meta + 0x6c);

        if (fps_gate == 0 && graph_gate == 0) {
            memset(hist_ring, 0, sizeof hist_ring);
            hist_packed = 0;
            hist_last_frame = 0;
        } else {
            uint32_t hist_count = (uint32_t)hist_packed;
            uint32_t hist_write = (uint32_t)(hist_packed >> 32);
            uint64_t session = *(uint64_t *)((char *)tpl + 0xd0);

            if (hist_session != session || frame - hist_last_frame >= 0x3e9) {
                memset(hist_ring, 0, sizeof hist_ring);
                hist_packed = 0;
                hist_session = session;
                hist_count = 0;
                hist_write = 0;
            } else if (hist_last_frame == 0 || frame <= hist_last_frame) {
                hist_last_frame = frame;
            } else {
                hist_ring[hist_write] = (float)(frame - hist_last_frame);
                if (hist_count < OVERLAY_HIST_CAP)
                    hist_count++;
                hist_write = (hist_write + 1) % OVERLAY_HIST_CAP;
                hist_last_frame = frame;
            }
            hist_packed = (uint64_t)hist_count | ((uint64_t)hist_write << 32);
            if (fps_gate != 0 && hist_count != 0) {
                float sum = 0.0f;
                float avg;
                int fps;
                char buf[0x20];

                for (uint32_t i = 0; i < hist_count; i++)
                    sum += hist_ring[i];
                avg = sum / (float)hist_count;
                fps = (avg <= 0.0f || isnan(avg)) ? 0 : (int)(1000.0f / avg + 0.5f);
                snprintf(buf, sizeof buf, "%d FPS  %.1f MS", fps, (double)avg);
                cur->color[0] = 0.025f;
                cur->color[1] = 0.025f;
                cur->color[2] = 0.8f;
                cur->color[3] = 1.0f;
                hud_emit_rect(cur, vp_x + 0xe, vp_y + vp_h - 0x35, 260, 24, 1);
                cur->color[0] = 1.0f;
                cur->color[1] = 1.0f;
                cur->color[2] = 1.0f;
                cur->color[3] = 1.0f;
                hud_emit_label(cur, buf, vp_x + 0x14, vp_y + vp_h - 0x30);
            }
            if (graph_gate != 0 && hist_count != 0) {
                int wr = vp_w;
                int lx;
                int by = vp_y + vp_h;
                int bw;

                if (wr < 0x168)
                    wr = 0x168;
                if (wr > 0x21c)
                    wr = 0x21c;
                lx = vp_x + vp_w - wr;
                bw = (wr > 0x1c1) ? 2 : 1;
                cur->color[0] = 0.025f;
                cur->color[1] = 0.025f;
                cur->color[2] = 0.035f;
                cur->color[3] = 1.0f;
                hud_emit_rect(cur, lx + 0xf0, by - 0x96, wr - 0x102, 0x3c, 1);
                cur->color[0] = 0.2f;
                cur->color[1] = 0.2f;
                cur->color[2] = 0.2f;
                cur->color[3] = 1.0f;
                hud_emit_rect(cur, lx + 0xf6, by - 0x81, 270, 1, 1);
                hud_emit_rect(cur, lx + 0xf6, by - 0x71, 270, 1, 1);
                for (uint32_t i = 0; i < hist_count; i++) {
                    uint32_t idx = (hist_write + OVERLAY_HIST_CAP - hist_count + i)
                                   % OVERLAY_HIST_CAP;
                    float v = hist_ring[idx];
                    int h = (int)fminf(v, 50.0f);

                    if (h < 1)
                        h = 1;
                    if (h > 50)
                        h = 50;
                    cur->color[0] = (v >= 33.0f) ? 1.0f : 0.2f;
                    cur->color[1] = (v >= 33.0f) ? 0.2f : 1.0f;
                    cur->color[2] = 0.2f;
                    cur->color[3] = 1.0f;
                    hud_emit_rect(cur, lx + 0xf6 + (int)(i * 270 / 90), by - 0x92, bw, h, 1);
                }
            }
        }
    }

    if (*(int *)(meta + 0x1c) != 0) {
        char buf[0x20];

        snprintf(buf, sizeof buf, "X %.1f  Y %.1f",
                 (double)*(float *)((char *)tpl + 0x870),
                 (double)*(float *)((char *)tpl + 0x874));
        cur->color[0] = 0.025f;
        cur->color[1] = 0.025f;
        cur->color[2] = 0.8f;
        cur->color[3] = 1.0f;
        hud_emit_rect(cur, vp_x + 0xe, vp_y + vp_h - 0x98, 282, 23, 1);
        cur->color[0] = 1.0f;
        cur->color[1] = 1.0f;
        cur->color[2] = 1.0f;
        cur->color[3] = 1.0f;
        hud_emit_label(cur, buf, vp_x + 0x14, vp_y + vp_h - 0x94);
    }

    if (*(int *)(meta + 0x1d) != 0) {
        int respawn = *(int *)((char *)tpl + 0x10);

        if (respawn >= 1 && respawn <= 0xb4) {
            char buf[0x20];
            int len;
            int cx;

            snprintf(buf, sizeof buf, "RESPAWN %d", respawn);
            len = (int)strlen(buf);
            cx = vp_x + (vp_w / 2) - len * 6;
            if (cx < 0)
                cx += 1;
            cur->color[0] = 0.025f;
            cur->color[1] = 0.025f;
            cur->color[2] = 0.035f;
            cur->color[3] = 1.0f;
            hud_emit_rect(cur, cx - 6, vp_y + vp_h - 0x82, len * 0xc + 0xc, 0x17, 1);
            cur->color[0] = 1.0f;
            cur->color[1] = (*(int *)((char *)tpl + 0x14) < 2) ? 0.6f : 0.2f;
            cur->color[2] = 0.2f;
            cur->color[3] = 1.0f;
            hud_emit_label(cur, buf, cx, vp_y + vp_h - 0x7e);
        }
    }
}

extern void *g_host_dl_handle;
extern void *g_fn_register;
extern void *g_fn_snapshot;
extern void *g_fn_lease;
extern void *g_fn_recheck;
extern void *g_fn_publish_routes;
extern void *g_expected_module_base;
extern const char *g_expected_module_name;
extern int g_functions_armed;
extern uint8_t g_route[0x7c8];
extern uint8_t g_route_islands[0x1000];
extern uint8_t g_functions_subsys_a[0x1000];
extern uint8_t g_functions_subsys_b[0x1000];
extern void *g_task_ctx;
extern void *g_island_a;
extern void *g_island_b;
extern void *g_island_c;
extern int g_functions_published;
extern long g_remote_handle;
extern uint64_t g_boot_epoch_ms;
extern uintptr_t g_game_base;
extern int mem_read(long handle, uintptr_t addr, void *out, uint32_t len);
extern void diag_emit(const char *event, const char *reason, const char *json);
extern void guard_report(const char *tag, uintptr_t addr, int code);
extern int region_sha_verify(uintptr_t rva, uint32_t len, const char *sha_hex);
extern void *island_alloc_near(uintptr_t target);
extern int route_bind_contract_init(void *route, size_t capacity, uintptr_t game_base,
                                    const char *build_id, const char *image_id,
                                    const void *callbacks, const void *descriptor);
extern int route_prepare_islands(void *route, void *a, void *b, void *c, void *out);
extern int route_publish(void *route);
extern size_t route_descriptor_bytes(void);
extern void subsys_a_init(void *subsys);
extern void task_ctx_init(void *ctx);
extern void subsys_b_init(void *subsys);

typedef struct {
    uintptr_t rva;
    uint32_t len;
    uint32_t pad;
    uint64_t sha[4];
} guard_region_t;

extern const guard_region_t g_guard_regions[15];
extern int theme_apply(int theme_id, int variant_id);
extern void theme_rollback(int theme_id, int variant_id);
extern int g_camera_target_lock;
extern uint64_t g_camera_target;
extern uint32_t g_camera_mode;
extern uint32_t g_camera_generation;
extern void script_port_camera_flush(void);
extern int camera_target_lock_holder(void);
extern uint32_t g_capture_rejected;
extern uint32_t g_capture_rejected_log;

int evasion_functions_route_boot(void)
{
    void *fns[5];
    static const char *const names[5] = {
        "nexus_evasion_functions_register_v1",
        "nexus_evasion_functions_snapshot_v1",
        "nexus_evasion_functions_lease_v1",
        "nexus_evasion_functions_recheck_v1",
        "nexus_evasion_publish_event_routes_v2",
    };
    Dl_info info;
    sha256_ctx_t ctx;
    uint8_t digest[32];
    uint8_t chunk[0x100];
    uint64_t desc[9];
    uint64_t callbacks[6];

    *(int *)cell_lock(ng_prepare_key) = 1;
    *(int *)cell_lock(ng_latch_key) = 0;
    *(int *)cell_lock(remote_write_key) = 0;

    for (int i = 0; i < 5; i++) {
        fns[i] = dlsym(g_host_dl_handle, names[i]);
        if (fns[i] == NULL || dladdr(fns[i], &info) == 0
            || info.dli_fbase != g_expected_module_base
            || strcmp(info.dli_fname, g_expected_module_name) != 0) {
            guard_report("exports", 0, 0);
            diag_emit("functions", "exports failed", NULL);
            return 0;
        }
    }
    if (route_descriptor_bytes() > 0x1000) {
        guard_report("exports", 0, 0);
        return 0;
    }
    g_fn_register = fns[0];
    g_fn_snapshot = fns[1];
    g_fn_lease = fns[2];
    g_fn_recheck = fns[3];
    g_fn_publish_routes = fns[4];
    g_functions_armed = 1;

    for (int i = 0; i < 15; i++) {
        const guard_region_t *region = &g_guard_regions[i];
        uintptr_t addr = g_game_base + region->rva;
        uint32_t done = 0;

        sha256_init(&ctx);
        while (done < region->len) {
            uint32_t take = region->len - done;

            if (take > sizeof chunk)
                take = sizeof chunk;
            if (mem_read(g_remote_handle, addr + done, chunk, take) != 1) {
                if ((long)g_remote_handle < 0 || addr < 0x10000)
                    guard_report("read_argument", addr, 0);
                diag_emit("functions", "guard read failed", NULL);
                return 0;
            }
            sha256_update(&ctx, chunk, take);
            done += take;
        }
        sha256_final(&ctx, digest);
        if (memcmp(digest, region->sha, 32) != 0) {
            guard_report("guard_hash", addr, 0);
            diag_emit("functions", "guard hash failed", NULL);
            return 0;
        }
    }

    subsys_a_init(&g_functions_subsys_a);
    task_ctx_init(&g_task_ctx);
    subsys_b_init(&g_functions_subsys_b);

    callbacks[0] = 2 | ((uint64_t)0x30 << 32);
    callbacks[1] = 0;
    callbacks[2] = (uint64_t)(uintptr_t)route_descriptor_stub_a;
    callbacks[3] = (uint64_t)(uintptr_t)route_descriptor_stub_b;
    callbacks[4] = (uint64_t)(uintptr_t)route_descriptor_stub_c;
    callbacks[5] = (uint64_t)(uintptr_t)route_descriptor_stub_d;
    desc[0] = 0;
    desc[1] = g_boot_epoch_ms;
    desc[2] = 0;
    desc[3] = (uint64_t)(uintptr_t)route_event_dispatch;
    desc[4] = (uint64_t)(uintptr_t)route_frame_prepare;
    desc[5] = (uint64_t)(uintptr_t)frame_observer_publish;
    desc[6] = (uint64_t)(uintptr_t)route_frame_consume;
    desc[7] = (uint64_t)(uintptr_t)route_kind_report;
    desc[8] = (uint64_t)(uintptr_t)natural_skill_registers_committed;

    if (!route_bind_contract_init(g_route, 0x1000, g_game_base, GL_BUILD_ID, GL_IMAGE_ID,
                                  callbacks, desc)) {
        guard_report("bind", 0, 0);
        diag_emit("functions", "route bind failed", NULL);
        return 0;
    }
    g_island_a = island_alloc_near(g_game_base + 0xb2e674);
    g_island_b = island_alloc_near(g_game_base + 0xb2e994);
    g_island_c = island_alloc_near(g_game_base + 0xb2e780);
    if (g_island_a == NULL || g_island_b == NULL || g_island_c == NULL) {
        guard_report("allocate", 0, 0);
        diag_emit("functions", "island allocation failed", NULL);
        return 0;
    }
    if (!route_prepare_islands(g_route, g_island_a, g_island_b, g_island_c, g_route_islands)) {
        diag_emit("functions", "route prepare failed", NULL);
        return 0;
    }
    int published = route_publish(g_route);

    if (published == 1) {
        g_functions_published = 1;
        return 1;
    }
    if (published == -1) {
        diag_emit("fatal", "event_route_rollback_unverified", NULL);
        abort();
    }
    guard_report("publish", 0, 0);
    diag_emit("functions", "route publish failed", NULL);
    return 0;
}

extern int g_ability_armed;
extern uint32_t g_ability_bound_entity_id;
extern uint64_t g_bound_obj;
extern uint64_t g_bound_a;
extern uint64_t g_bound_b;
extern uint64_t g_bound_flags;
extern uint64_t g_bound_self;
extern uint64_t g_bound_id2;
extern uint32_t g_resource_cur;
extern uint32_t g_resource_max;
extern float g_anchor_x;
extern float g_anchor_y;
extern int g_ability_busy;
extern uint32_t g_ability_state;
extern uint32_t g_ability_ctx;
extern uint32_t g_ability_ctx2;
extern uint32_t g_ability_last_id;
extern void *g_ability_queue;
extern const uint32_t g_ability_kinds[3];
extern int local_player_resolve(void *snap);
extern int task_capture_run(void *ctx, const void *cfg, const void *req, void *callback,
                            int flag, void *cap, void *cap_buf);
extern int capture_validate(uintptr_t game_base, long handle, int (*read_fn)(long, uintptr_t, void *, uint32_t),
                            int mode, void *buf);
extern int anchor_verify(uint64_t obj, int (*read_fn)(long, uintptr_t, void *, uint32_t),
                         int mode, uint32_t flags[2]);
extern uint32_t obj_checksum32(const void *obj);
extern int ability_queue_begin(void *queue, const void *snap);
extern int kind_available(const void *queue, const void *snap, uint32_t kind);
extern int ability_record_validate(const void *snap, const void *record);
extern void kind_report(void *queue, const void *snap, uint32_t kind, int result);
extern int target_validate(uintptr_t game_base, uint64_t obj, uint64_t target,
                           int (*read_fn)(long, uintptr_t, void *, uint32_t), int mode, void *out);

void ability_command_frame_dispatch(void *frame)
{
    uint64_t snap[13];
    uint8_t cap_buf[0x140];
    void *cap = cap_buf;
    uint32_t flags[2] = {0};
    uint64_t target;
    struct timespec ts = {2, 0};
    char json[0xdc];
    uint32_t input_type;
    uint64_t game_fn;
    int ret;

    if (frame == NULL || *(int *)((char *)frame + 0x24) == 0)
        return;
    if (!local_player_resolve(snap)) {
        ability_queue_begin(g_ability_queue, NULL);
        return;
    }
    if (g_ability_armed != 0 && g_ability_bound_entity_id == (uint32_t)snap[2]) {
        void *req[9];

        req[0] = (void *)(uintptr_t)g_bound_b;
        req[1] = (void *)g_bound_self;
        req[2] = (void *)g_bound_id2;
        req[3] = (void *)g_bound_a;
        req[4] = (void *)g_bound_obj;
        req[5] = (void *)g_bound_flags;
        req[6] = (void *)(uintptr_t)*(uint32_t *)&ts;
        req[7] = (void *)snap[2];
        if (task_capture_run(g_task_ctx, req, &ts, ability_capture_callback, 0, cap, cap_buf) == 1
            && capture_validate(g_game_base, *(long *)cap_buf, mem_read, 0, cap_buf)
            && *(uint32_t *)((char *)cap_buf + 8) != 0
            && *(uint32_t *)((char *)cap_buf + 12) == *(uint32_t *)((char *)cap_buf + 4)
            && anchor_verify(g_bound_obj, mem_read, 0, flags)) {
            clock_gettime(CLOCK_MONOTONIC, &ts);
            if (!ability_queue_begin(g_ability_queue, snap))
                return;
            for (int k = 0; k < 3; k++) {
                uint32_t kind = g_ability_kinds[k];

                if (!kind_available(g_ability_queue, snap, kind)
                    || !ability_record_validate(snap, cap_buf)
                    || g_ability_state != 0)
                    continue;
                g_ability_busy = 1;
                if (k == 0) {
                    target = ((uint64_t (*)(uint64_t)) (g_game_base + 0xe7cddc))(snap[4]);
                    if (!target_validate(g_game_base, snap[4], target, mem_read, 0, NULL)) {
                        g_ability_busy = 0;
                        kind_report(g_ability_queue, snap, kind, -1);
                        continue;
                    }
                    g_ability_ctx = 1;
                    g_ability_ctx2 = 1;
                    g_ability_state = 6;
                    ret = ((int (*)(uint64_t, uint64_t, uint64_t, int, int, int, uint32_t))
                           (g_game_base + 0xb3d64c))(g_bound_obj, snap[4], target,
                                                     (int)(*(float *)((char *)cap_buf + 0x30) * 300.0f),
                                                     (int)(*(float *)((char *)cap_buf + 0x34) * 300.0f),
                                                     1, 0x5f3f356bu);
                    input_type = 1;
                } else {
                    input_type = (k == 1) ? 8 : 0x11;
                    game_fn = g_game_base + ((input_type == 8) ? 0x8580d4 : 0x858894);
                    g_ability_ctx = 0;
                    g_ability_ctx2 = 0;
                    g_ability_state = 6;
                    ret = ((int (*)(void)) game_fn)() ? 1 : 0;
                }
                if (ret & 1) {
                    g_ability_state = 0;
                    g_ability_ctx = 0;
                    g_ability_ctx2 = 0;
                    g_ability_last_id = (uint32_t)g_bound_id2;
                    snprintf(json, sizeof json,
                             ",\"kind\":%u,\"farm\":%u,\"target_gid\":%u,\"input_type\":%u,"
                             "\"server_acceptance_proven\":false",
                             kind, (uint32_t)snap[7], *(uint32_t *)((char *)cap_buf + 12),
                             input_type);
                    diag_emit("ability_command", "original_game_command", json);
                    kind_report(g_ability_queue, snap, kind, 1);
                } else {
                    g_ability_busy = 0;
                    kind_report(g_ability_queue, snap, kind, -1);
                }
            }
            g_ability_busy = 0;
        }
    }
}

extern void *g_register_visual_fn;
extern void script_port_theme_publish(const char *payload);
extern uint64_t g_theme_seq;
extern void *g_snapshot_keys_fn;
extern const char *g_game_file_path;
extern uint64_t g_visual_keyslot;
extern int visual_keyslot_bind(uint64_t keyslot, const void *identity);
extern int game_file_identity_pin(const char *path, uint64_t *identity);
extern int frame_callback_register(void (*cb)(void *));

void visual_esp_subsystem_boot(int enable)
{
    void *fns[2];
    static const char *const names[2] = {
        "nexus_evasion_register_visual_v1",
        "nexus_evasion_snapshot_keys_v1",
    };
    Dl_info info;
    struct stat st;
    uint64_t identity[4];

    if (enable == 0)
        return;
    for (int i = 0; i < 2; i++) {
        fns[i] = dlsym(g_host_dl_handle, names[i]);
        if (fns[i] == NULL || dladdr(fns[i], &info) == 0
            || info.dli_fbase != g_expected_module_base
            || strcmp(info.dli_fname, g_expected_module_name) != 0) {
            diag_emit("visual", "exports failed", NULL);
            return;
        }
    }
    g_register_visual_fn = fns[0];
    g_snapshot_keys_fn = fns[1];
    if (stat(g_game_file_path, &st) != 0
        || st.st_size != 0x1340c90
        || (st.st_mode & 0xf000) != 0x8000) {
        diag_emit("visual", "game file identity mismatch", NULL);
        return;
    }
    identity[0] = (uint64_t)st.st_size;
    identity[1] = (uint64_t)st.st_ino;
    identity[2] = (uint64_t)major(st.st_dev);
    identity[3] = (uint64_t)minor(st.st_dev);
    if (!visual_keyslot_bind(g_visual_keyslot, identity)) {
        diag_emit("visual", "keyslot bind failed", NULL);
        return;
    }
    if (nexus_visual_gl_install_v1((const uint32_t *)gl_contract_block()) != 1) {
        diag_emit("visual", "gl install failed", NULL);
        return;
    }
    frame_callback_register(esp_frame_publish);
}

extern uint64_t g_esp_state[32];
extern uint64_t g_esp_epoch;
extern uint64_t g_esp_generation;
extern uint64_t g_esp_last_log_us;
extern int esp_packet_prepare(void *packet);
extern int esp_packet_publish(void *packet);

void esp_frame_publish(void *frame)
{
    uint64_t now_us;
    (void)frame;
    char json[0x100];

    memset(g_esp_state, 0, sizeof g_esp_state);
    if (!esp_packet_prepare(&g_esp_state))
        return;
    if (!esp_packet_publish(&g_esp_state))
        return;
    g_esp_epoch++;
    g_esp_generation++;
    now_us = source_now_us();
    if (now_us - g_esp_last_log_us >= 2000000ull) {
        g_esp_last_log_us = now_us;
        snprintf(json, sizeof json,
                 ",\"prepared\":%d,\"published\":%d,\"markers\":%u,\"epoch\":%llu",
                 1, 1, (unsigned)g_esp_epoch, (unsigned long long)g_esp_generation);
        diag_emit("visual_frame", "shared_ESP_packet", json);
    }
}

extern int g_periodic_owner_tid;
extern uint64_t g_pin_queued;
extern uint64_t g_spray_queued;
extern uint64_t g_periodic_last_log_us;

void periodic_command_tick(void)
{
    uint64_t now_us;
    char json[0x100];
    static const uint32_t types[2] = {9, 15};

    if (g_periodic_owner_tid != gettid())
        return;
    for (int i = 0; i < 2; i++) {
        if (periodic_enqueue(types[i]))
            continue;
        return;
    }
    now_us = source_now_us();
    if (now_us - g_periodic_last_log_us >= 1000000ull) {
        g_periodic_last_log_us = now_us;
        snprintf(json, sizeof json,
                 ",\"pin_queued\":%llu,\"spray_queued\":%llu,\"types\":[9,15]",
                 (unsigned long long)g_pin_queued, (unsigned long long)g_spray_queued);
        diag_emit("game_owned_periodic_inputs", "periodic_command", json);
    }
}

extern long g_remote_write_fd;

int env_override_apply(void)
{
    static const char fmt[] = ",\"rva\":\"0x1308444\",\"old\":%u,\"new\":3";
    uintptr_t addr = g_game_base + 0x1308444;
    uint32_t old_value = 0;
    uint32_t probe = 0;
    uint32_t writeback[2];
    char json[0x60];
    int state;

    if (mem_read(g_remote_handle, addr, &old_value, 4) != 1) {
        diag_emit("env_override", "read failed", NULL);
        return 0;
    }
    if (old_value == 3) {
        diag_emit("env_override", "already_production", NULL);
        return 1;
    }
    probe = old_value;
    writeback[0] = 3;
    writeback[1] = 0x308444;
    (void)probe;
    if (remote_write((uint32_t)g_remote_write_fd, writeback, 4, addr) != 4) {
        snprintf(json, sizeof json, ",\"old\":%u", old_value);
        diag_emit("env_override", "write failed", json);
        return 0;
    }
    if (mem_read(g_remote_handle, addr, &probe, 4) != 1 || probe != 3) {
        diag_emit("env_override", "unexpected_current_value", NULL);
        return 0;
    }
    state = 1;
    snprintf(json, sizeof json, fmt, old_value);
    diag_emit("env_override", "env_override_ready", json);
    return state;
}

extern uint8_t g_suitcase_slot_a[0x90];
extern uint8_t g_suitcase_slot_b[0x90];
extern uint64_t g_suitcase_gen_a;
extern uint64_t g_suitcase_gen_b;
extern const uint8_t suitcase_header_a[0x40];
extern const uint8_t suitcase_header_b[0x40];

void suitcase_snapshot_build_a(void)
{
    g_suitcase_gen_a++;
    memset(g_suitcase_slot_a, 0, 0x90);
    memcpy(g_suitcase_slot_a, suitcase_header_a, 0x40);
    *(uint64_t *)(g_suitcase_slot_a + 0x40) = g_suitcase_gen_a;
    *(uint64_t *)(g_suitcase_slot_a + 0x48) = g_boot_epoch_ms;
}

void suitcase_snapshot_build_b(void)
{
    g_suitcase_gen_b++;
    memset(g_suitcase_slot_b, 0, 0x90);
    memcpy(g_suitcase_slot_b, suitcase_header_b, 0x40);
    *(uint64_t *)(g_suitcase_slot_b + 0x40) = g_suitcase_gen_b;
    *(uint64_t *)(g_suitcase_slot_b + 0x48) = g_boot_epoch_ms;
}

extern int g_theme_apply_lock;
extern uint64_t g_theme_manager;
extern uint64_t g_theme_db;

static void theme_seq_publish(int theme_id, int variant_id, uint32_t seq)
{
    char payload[0x40];

    snprintf(payload, sizeof payload, "NST69 2\n%d %d %u %u %u\n",
             theme_id, variant_id, seq, (unsigned)g_boot_epoch_ms, 0u);
    script_port_theme_publish(payload);
}

int theme_select_request(int theme_id, int variant_id)
{
    uint32_t seq;
    int ok;

    if (__atomic_exchange_n(&g_theme_apply_lock, 1, __ATOMIC_ACQ_REL) != 0)
        return 0;
    seq = (uint32_t)++g_theme_seq;
    ok = theme_apply(theme_id, variant_id);
    if (ok)
        theme_seq_publish(theme_id, variant_id, seq);
    else
        theme_rollback(theme_id, variant_id);
    __atomic_clear(&g_theme_apply_lock, __ATOMIC_RELEASE);
    return ok;
}


int camera_target_set(uint64_t entity)
{
    uint64_t previous;

    if (__atomic_exchange_n(&g_camera_target_lock, 1, __ATOMIC_ACQ_REL) != 0)
        return 0;
    previous = g_camera_target;
    if (previous != entity) {
        g_camera_target = entity;
        g_camera_mode = 0;
        g_camera_generation++;
        script_port_camera_flush();
    }
    __atomic_clear(&g_camera_target_lock, __ATOMIC_RELEASE);
    return previous != entity;
}

extern int g_camera_target_lock;
extern uint64_t g_camera_target;
extern uint32_t g_camera_mode;
extern uint32_t g_camera_generation;
extern void script_port_camera_flush(void);

extern uint64_t g_aura_dispatch_fn;
extern uint64_t g_aura_commit_fn;
extern uint32_t g_aura_counters[8];
extern uint8_t g_aura_staging[0x98];
extern uint8_t g_frame_snapshot[0x70];
extern int g_observer_lock;
extern uint64_t g_aura_window_us;

void aura_staged_state_clear(void)
{
    memset(g_aura_staging, 0, 0x98);
    if (g_ability_state == 2)
        g_ability_state = 0;
}

void frame_snapshot_clear(void)
{
    memset(g_frame_snapshot, 0, 0x30);
}

void aura_signed_local_free_dispatch(uint32_t id)
{
    for (int i = 0; i < 8; i++)
        g_aura_counters[i] += *(uint32_t *)(g_aura_staging + i * 4);
    if (g_aura_dispatch_fn != 0)
        ((void (*)(uint32_t, uint32_t, void *)) g_aura_dispatch_fn)(29, id, NULL);
    if (g_aura_commit_fn != 0)
        ((void (*)(void)) g_aura_commit_fn)();
    aura_staged_state_clear();
}

void aura_frame_watchdog(void *frame)
{
    uint64_t now_us = source_now_us();
    char json[0x80];

    if (frame == NULL)
        return;
    if (now_us - g_aura_window_us >= 501000ull)
        return;
    if (__atomic_exchange_n(&g_observer_lock, 1, __ATOMIC_ACQ_REL) != 0)
        return;
    snprintf(json, sizeof json, ",\"action\":29,\"tag\":\"SIGNED_LOCAL_FREE\"");
    diag_emit("aura", "aura_frame", json);
    __atomic_clear(&g_observer_lock, __ATOMIC_RELEASE);
}

void capture_reject_note(int count, int use_lock)
{
    uint64_t now_us = source_now_us();
    char json[0x60];

    g_capture_rejected += count;
    if (g_capture_rejected_log >= 8)
        return;
    if (use_lock && __atomic_exchange_n(&g_observer_lock, 1, __ATOMIC_ACQ_REL) != 0)
        return;
    g_capture_rejected_log++;
    snprintf(json, sizeof json, ",\"count\":%u", (unsigned)g_capture_rejected);
    diag_emit("capture", "capture_request_rejected", json);
    if (use_lock)
        __atomic_clear(&g_observer_lock, __ATOMIC_RELEASE);
    (void)now_us;
}

extern uint32_t g_capture_rejected;
extern uint32_t g_capture_rejected_log;


int respawn_timer_ready(void)
{
    return *(int *)0x220784 == 1 ? 1 : 0;
}

int respawn_phase_ready(void)
{
    return *(int *)0x2208a0 == 1 ? 1 : 0;
}

extern void profile_tag_table_update(const uint64_t record[3]);
extern int http_socket_connect(const char *host, int port, int deadline_ms);
extern int http_send_all(int fd, const void *buf, size_t len, int deadline_ms);
extern uint8_t g_esp_config[10];
extern void aim_command_write(uint32_t mode, uint32_t duration, int angle,
                              float x, float y, float spread);
extern uint64_t g_stick_hold_start;
extern float g_trajectory_min;
extern float g_trajectory_exposure;
extern uint64_t g_floating_last_us;
extern uint32_t g_floating_record;
extern int ndc1_object_track(void *state, const void *record, uint32_t index);
extern int remote_entity_fetch(void *vtable, uint64_t remote_addr, void *out);
extern void entity_bank_update(void *payload);
extern void player_bank_update(void *payload);
extern void render_cache_update(void *payload);
extern void aim_plan_callback(int index, float value);
extern int frame_context_verify(const uint32_t *ctx, void *snapshot);
extern void config_float_record(uint32_t id, float value);
extern uint64_t g_response_last_ms;
extern int g_response_owner_tid;
extern uint8_t g_response_record[0x28];
extern int g_response25_owner_tid;
extern int g_response29_owner_tid;
extern uint8_t g_response29_record[0x28];
extern uint64_t g_modal_verify_ms;
extern int g_modal_last_result;
extern uint64_t g_local_profile_slot;
extern int g_theme_last_id;
extern int g_theme_last_variant;
extern uint64_t g_local_profile;
extern uint64_t g_aura_plan_id;
extern void font_detour_handler(void);
extern const char FONT_DETOUR_SHA[];
extern uint64_t g_glyph_serial;
extern uint64_t g_font_context_serial;
extern uint64_t g_font_pair_a;
extern uint64_t g_font_pair_b;
extern int g_font_override_armed;
extern uint64_t g_action25_last_ms;
extern uint64_t g_action25_window;
extern uint64_t g_action25_pin_mask;
extern uint32_t g_action25_last_id;
extern void aura_worker_plan_commit(void *bank);
extern uint64_t g_entity_array;
extern uint32_t g_entity_count;
extern uint64_t g_selection_slot;
extern uint8_t g_battle_state[0x1ce0];
extern float g_dps_scope_value;
extern uint64_t g_dps_last_log_us;


#define PROFILE_TAG_ALPHABET "0289PYLQGRJCUV"
#define PROFILE_TAG_BASE 13
#define PROFILE_TAG_BASE14 14
#define PROFILE_TAG_MAX_DIGITS 23
#define HTTP_MAX_BODY 0x4000
#define HTTP_DEADLINE_MS 2000

extern uint8_t g_profile_bank[0x285d0];
extern uint64_t g_profile_bank_serial;
extern pthread_mutex_t g_profile_tag_lock;
extern void profile_spawn_plusapi_query(uint64_t mask);
extern int plusapi_http_post(const char *host, const char *path, const char *body,
                             char *out, size_t out_cap);

static int tag_digit_value(char c)
{
    const char *p = strchr(PROFILE_TAG_ALPHABET, c);

    if (c == '\0' || p == NULL)
        return -1;
    return (int)(p - PROFILE_TAG_ALPHABET);
}

static uint64_t profile_tag_decode14(const char *tag, int digits)
{
    uint64_t value = 0;

    for (int i = 0; i < digits; i++) {
        int d = tag_digit_value(tag[i]);

        if (d < 0)
            return 0;
        value = value * PROFILE_TAG_BASE14 + (uint64_t)d;
    }
    return value;
}

void profile_tag_encode(const char *tag, int digits, uint64_t record[3])
{
    if (digits <= 0 || digits > PROFILE_TAG_MAX_DIGITS) {
        record[0] = 0;
        record[1] = 0;
        record[2] = 0;
        return;
    }
    pthread_mutex_lock(&g_profile_tag_lock);
    record[0] = 1;
    record[1] = profile_tag_decode14(tag, digits);
    record[2] = (uint64_t)digits;
    profile_tag_table_update(record);
    pthread_mutex_unlock(&g_profile_tag_lock);
}


void profile_tag_table_validate(const uint32_t *ids, const char *const *tags, uint32_t count)
{
    int dirty = 0;
    uint64_t mask = 0;

    for (uint32_t i = 0; i < count; i++) {
        uint32_t id = ids[i];

        if (id < 16000000u || id >= 18000000u)
            continue;
        for (const char *p = tags[i]; *p != '\0'; p++) {
            if (tag_digit_value(*p) < 0)
                return;
        }
        mask |= 1ull << (id % 64);
        dirty = 1;
    }
    if (dirty)
        profile_spawn_plusapi_query(mask);
}

void profile_snapshot_capture(const void *frame)
{
    uint32_t count = 0;
    uint8_t *bank = g_profile_bank;
    char tag[PROFILE_TAG_MAX_DIGITS + 1];

    g_profile_bank_serial++;
    memset(bank, 0, 0x40);
    *(uint64_t *)(bank + 0x20) = g_profile_bank_serial;
    for (uint32_t i = 0; i < 512; i++) {
        const uint8_t *entry = (const uint8_t *)frame + (size_t)i * 0x48;
        uint32_t id = *(const uint32_t *)(entry + 0x38);

        if (id == 0)
            continue;
        if (count >= 512)
            break;
        tag[0] = '#';
        int pos = 1;
        uint64_t v = id;
        char tmp[PROFILE_TAG_MAX_DIGITS];

        for (int d = 0; d < 8; d++) {
            tmp[d] = PROFILE_TAG_ALPHABET[v % PROFILE_TAG_BASE];
            v /= PROFILE_TAG_BASE;
        }
        for (int d = 7; d >= 0; d--)
            tag[pos++] = tmp[d];
        tag[pos] = '\0';
        uint8_t *slot = bank + 0x40 + (size_t)count * 0x20;
        memcpy(slot, entry, 0x18);
        strcpy((char *)(slot + 0x18), tag);
        count++;
    }
    *(uint32_t *)(bank + 0x28) = count;
}

static size_t http_parse_chunked(char *body, size_t len)
{
    size_t read_pos = 0;
    size_t write_pos = 0;

    while (read_pos < len) {
        unsigned long chunk = 0;
        int digits = 0;

        while (read_pos < len) {
            char c = body[read_pos];

            if (c == '\r' || c == '\n') {
                read_pos++;
                if (c == '\n')
                    break;
                continue;
            }
            if (c >= '0' && c <= '9') {
                chunk = chunk * 16 + (unsigned long)(c - '0');
                digits++;
                read_pos++;
                continue;
            }
            if (c >= 'a' && c <= 'f') {
                chunk = chunk * 16 + (unsigned long)(c - 'a' + 10);
                digits++;
                read_pos++;
                continue;
            }
            return write_pos;
        }
        if (digits == 0 || chunk == 0)
            break;
        for (unsigned long i = 0; i < chunk && read_pos < len; i++)
            body[write_pos++] = body[read_pos++];
    }
    return write_pos;
}

int plusapi_http_post(const char *host, const char *path, const char *body,
                      char *out, size_t out_cap)
{
    char request[0x200];
    char header[0x400];
    int fd;
    uint64_t deadline;
    size_t total = 0;
    size_t body_len = strlen(body);
    int chunked = 0;
    long content_length = -1;

    snprintf(request, sizeof request,
             "POST /%s HTTP/1.1\r\nHost: %s\r\nContent-Type: application/json\r\n"
             "Content-Length: %zu\r\n\r\n",
             path, host, body_len);
    fd = http_socket_connect(host, 443, HTTP_DEADLINE_MS);
    if (fd < 0)
        return -1;
    if (!http_send_all(fd, request, strlen(request), HTTP_DEADLINE_MS)
        || !http_send_all(fd, body, body_len, HTTP_DEADLINE_MS)) {
        close(fd);
        return -2;
    }
    deadline = source_now_us() + HTTP_DEADLINE_MS * 1000ull;
    while (total < sizeof header - 1) {
        long got = recv(fd, header + total, 1, 0);

        if (got <= 0 || source_now_us() > deadline) {
            close(fd);
            return -3;
        }
        if (header[total] == '\n' && total >= 1 && header[total - 1] == '\n'
            && total >= 3 && header[total - 2] == '\r' && header[total - 3] == '\n')
            break;
        total++;
    }
    header[total] = '\0';
    for (char *line = strtok(header, "\r\n"); line != NULL; line = strtok(NULL, "\r\n")) {
        if (strncasecmp(line, "Content-Length:", 15) == 0)
            content_length = strtol(line + 15, NULL, 10);
        if (strncasecmp(line, "Transfer-Encoding:", 18) == 0
            && strstr(line, "chunked") != NULL)
            chunked = 1;
    }
    if (content_length == 0) {
        close(fd);
        out[0] = '\0';
        return 0;
    }
    size_t have = 0;
    size_t want = (chunked || content_length < 0) ? HTTP_MAX_BODY : (size_t)content_length;

    if (want > out_cap - 1)
        want = out_cap - 1;
    while (have < want) {
        long got = recv(fd, out + have, want - have, 0);

        if (got <= 0 || source_now_us() > deadline)
            break;
        have += (size_t)got;
    }
    close(fd);
    out[have] = '\0';
    if (chunked)
        have = http_parse_chunked(out, have);
    return (int)have;
}


extern float g_dps_scope_value;

void dps_scope_update(float value, uint32_t captured, uint32_t scope_wait)
{
    uint64_t now_us = source_now_us();
    char json[0x80];

    g_dps_scope_value = value;
    if (now_us - g_dps_last_log_us < 1000000ull)
        return;
    g_dps_last_log_us = now_us;
    snprintf(json, sizeof json,
             ",\"callback_tid\":%u,\"last_call_tid\":%u,\"owner_tid\":%u,\"value\":%.1f",
             (unsigned)captured, (unsigned)scope_wait, (unsigned)gettid(), (double)value);
    diag_emit("script_port_dps", "floating_number_captured", json);
}

void esp_config_parse(const char *json)
{
    static const char *const keys[] = {
        "espEnabled", "espBoxesEnabled", "espHealthEnabled", "espNameEnabled",
        "espDistanceEnabled", "espSnaplinesEnabled", "hitboxRenderer", "enemyTracer",
        "attackRangeIndicator", "trophiesAboveHead",
    };
    const char *p = json;

    for (int i = 0; i < 10; i++) {
        const char *hit = strstr(p, keys[i]);

        if (hit == NULL)
            continue;
        hit += strlen(keys[i]);
        while (*hit == ' ' || *hit == ':' || *hit == '"')
            hit++;
        g_esp_config[i] = (*hit == 't' || *hit == '1');
    }
}


#define NDC1_MAGIC 0x4e444331u
#define SNAP_MAGIC 0x4e444132u
#define ENTITY_MAGIC 0x4e444152u

typedef struct {
    uint32_t magic;
    uint32_t count;
    uint64_t nonce;
    uint8_t records[256][0x78];
} ndc1_frame_t;


int ndc1_frame_ingest(void *state, const ndc1_frame_t *frame, void *out)
{
    static uint64_t last_nonce;
    uint8_t pingpong[0xef58];
    uint32_t rr;

    if (state == NULL || frame == NULL || out == NULL)
        return 0;
    if (__atomic_exchange_n((uint8_t *)state + 4, 1, __ATOMIC_ACQ_REL) != 0)
        return 0;
    if (frame->magic != NDC1_MAGIC || frame->count > 256) {
        __atomic_clear((uint8_t *)state + 4, __ATOMIC_RELEASE);
        return 0;
    }
    if (frame->nonce == last_nonce) {
        __atomic_clear((uint8_t *)state + 4, __ATOMIC_RELEASE);
        return 0;
    }
    last_nonce = frame->nonce;
    memcpy(pingpong, frame->records, 0xef58 > sizeof frame->records ? sizeof frame->records : 0xef58);
    rr = *(uint32_t *)((char *)state + 8);
    for (uint32_t i = 0; i < frame->count; i++) {
        ndc1_object_track(state, pingpong + (size_t)i * 0x20, (rr + i) % 48);
    }
    *(uint32_t *)((char *)state + 8) = (rr + frame->count) % 48;
    memset(out, 0, 56);
    __atomic_clear((uint8_t *)state + 4, __ATOMIC_RELEASE);
    return 1;
}

int nexus_ingest_entities(void *cache, const void *desc, void *out)
{
    if (cache == NULL || desc == NULL || out == NULL)
        return 0;
    if (*(const uint32_t *)desc != ENTITY_MAGIC)
        return 0;
    ((uint32_t *)out)[0] = 1;
    ((uint32_t *)out)[1] = 0xdc0;
    for (int i = 0; i < 64; i++) {
        const uint8_t *rec = (const uint8_t *)desc + 0x20 + (size_t)i * 0x40;
        uint64_t ptr = *(const uint64_t *)rec;
        uint32_t id = *(const uint32_t *)(rec + 8);
        int status;

        if (ptr < 0x10000 || (ptr & 7) != 0)
            continue;
        if (id >= 1000000u && id < 2000000u)
            status = 5;
        else if (id >= 16000000u && id < 18000000u)
            status = 6;
        else
            continue;
        uint8_t *slot = (uint8_t *)out + 8 + (size_t)i * 0x28;

        memset(slot, 0, 0x28);
        *(uint32_t *)slot = status;
        *(uint64_t *)(slot + 8) = ptr;
        *(uint32_t *)(slot + 0x10) = id;
    }
    return 1;
}

#define SUPERAIM_POOL_STRIDE 0x50
#define SUPERAIM_TICKS_PER_SEC 30.0f

void superaim_update(void *sa)
{
    float *pool = (float *)((char *)sa + 0x10);
    uint32_t count = *(uint32_t *)((char *)sa + 8);
    float priority[80];
    uint32_t order[80];

    if (count > 80)
        count = 80;
    for (uint32_t i = 0; i < count; i++)
        priority[i] = pool[(size_t)i * (SUPERAIM_POOL_STRIDE / 4)];
    for (uint32_t i = 0; i < count; i++) {
        uint32_t best = i;

        for (uint32_t j = i + 1; j < count; j++)
            if (priority[j] > priority[best])
                best = j;
        if (best != i) {
            float f = priority[i];
            priority[i] = priority[best];
            priority[best] = f;
        }
        order[i] = best;
    }
    for (uint32_t slot = 0; slot < count && slot < 12; slot++) {
        float *rec = pool + (size_t)order[slot] * (SUPERAIM_POOL_STRIDE / 4);
        float vx = rec[2] * SUPERAIM_TICKS_PER_SEC;
        float vy = rec[3] * SUPERAIM_TICKS_PER_SEC;
        float lead_x = rec[0] + vx * rec[4];
        float lead_y = rec[1] + vy * rec[4];
        float spread = rec[5];

        if (slot & 1) {
            lead_x = rec[0] - vx * rec[4];
            lead_y = rec[1] - vy * rec[4];
            spread = -spread;
        }
        uint32_t duration = (uint32_t)((rec[6] * 4.0f + 50.0f) * 1311.0f) >> 17;
        int angle = (int)(rec[7] / 100.0f * 180.0f);

        aim_command_write(2, duration, angle, lead_x, lead_y, spread);
    }
    *(uint32_t *)((char *)sa + 0xc) = 1 + (count % 12);
}


#define GLLCAL_MAGIC "NEXUS_GLLCAL_1V "

int gllcal_descriptor_parse(const char *line, uint32_t out[8])
{
    const char *p = line;
    uint32_t values[5];
    int count = 0;

    if (strncmp(p, GLLCAL_MAGIC, strlen(GLLCAL_MAGIC)) != 0)
        return 0;
    p += strlen(GLLCAL_MAGIC);
    while (count < 5) {
        uint32_t v = 0;
        int digits = 0;

        while (*p >= 'a' && *p <= 'f') {
            v = v * 16 + (uint32_t)(*p - 'a' + 10);
            digits++;
            p++;
        }
        if (digits == 0)
            return 0;
        values[count++] = v;
        if (*p == ' ') {
            p++;
            continue;
        }
        break;
    }
    if (count != 5 || *p != '\n')
        return 0;
    if (values[0] > 10000)
        return 0;
    out[0] = values[0];
    for (int i = 1; i < 5; i++)
        out[i] = values[i];
    return 1;
}

void stick_event_record(void *state, int raw_x, int raw_y, int pressed)
{
    float x = (float)raw_x / 300.0f;
    float y = (float)raw_y / 300.0f;
    uint64_t now_us = source_now_us();

    if (fabsf(x) < 0.5f && fabsf(y) < 0.5f)
        pressed = 0;
    if (pressed) {
        if (g_stick_hold_start == 0)
            g_stick_hold_start = now_us;
        else if (now_us - g_stick_hold_start < 351000ull)
            return;
    }
    g_stick_hold_start = pressed ? now_us : 0;
    *(uint32_t *)((char *)state + 0x10) = 0x1e;
    *(float *)((char *)state + 0x14) = x;
    *(float *)((char *)state + 0x18) = y;
    *(uint32_t *)((char *)state + 0x1c) = pressed;
}


void target_motion_track(void *state, float x, float y, uint32_t frame)
{
    float *ring = (float *)state;
    uint32_t *meta = (uint32_t *)((char *)state + 0xa0);
    uint32_t idx = meta[0] % 10;
    float dx;
    float dy;
    float vx;
    float vy;

    if (frame - meta[1] > 30) {
        memset(ring, 0, 0xa0);
        meta[1] = frame;
    }
    dx = x - ring[idx * 2];
    dy = y - ring[idx * 2 + 1];
    vx = dx / 30.0f;
    vy = dy / 30.0f;
    ring[idx * 2] = x;
    ring[idx * 2 + 1] = y;
    meta[0] = idx + 1;
    meta[1] = frame;
    {
        float prev_vx = *(float *)&meta[2];
        float prev_vy = *(float *)&meta[3];
        float evx = 0.78f * prev_vx + 0.22f * vx;
        float evy = 0.78f * prev_vy + 0.22f * vy;
        float blend_x = 0.35f * evx + 0.65f * vx;
        float blend_y = 0.35f * evy + 0.65f * vy;
        float decay = 0.9f * 0.88f * 0.35f;
        float spare = 0.55f;

        meta[2] = *(uint32_t *)&evx;
        meta[3] = *(uint32_t *)&evy;
        meta[4] = *(uint32_t *)&blend_x;
        meta[5] = *(uint32_t *)&blend_y;
        meta[6] = *(uint32_t *)&decay;
        meta[7] = *(uint32_t *)&spare;
    }
}

void trajectory_exposure_update(void *state, float rx, float ry, float vx, float vy,
                                float t_max, float window, float penetration)
{
    (void)state;
    float v2 = vx * vx + vy * vy;
    float t = 0.0f;

    if (v2 > 1e-9f) {
        t = -(rx * vx + ry * vy) / v2;
        if (t < 0.0f)
            t = 0.0f;
        if (t > t_max)
            t = t_max;
    }
    float cx = rx + vx * t;
    float cy = ry + vy * t;
    float dist = hypotf(cx, cy) * 300.0f;

    if (dist < g_trajectory_min)
        g_trajectory_min = dist;
    g_trajectory_exposure += (1.0f + penetration) * window / (t + 0.0001f);
}


#define CONTACT_SLOTS 24
#define CONTACT_RADIUS_DEFAULT 2.65f

void autododge_contacts_ingest(void *state, const void *records, uint32_t count)
{
    uint8_t *slots = (uint8_t *)state;
    uint32_t *rr = (uint32_t *)((char *)state + 0x380 - 4);

    for (uint32_t i = 0; i < count; i++) {
        const uint8_t *rec = (const uint8_t *)records + (size_t)i * 0x20;
        uint8_t *slot = slots + (size_t)(*rr % CONTACT_SLOTS) * 0x20;
        uint32_t status = *(const uint32_t *)(rec + 0x1c);

        if (status == 8 || status == 9) {
            memset(slot, 0, 0x20);
        } else {
            memcpy(slot, rec, 0x20);
            *(float *)(slot + 0x18) = CONTACT_RADIUS_DEFAULT;
            *(uint32_t *)(slot + 0x1c) = 10;
        }
        *rr = *rr + 1;
    }
}

extern void profile_tag_table_update(const uint64_t record[3]);
extern int http_socket_connect(const char *host, int port, int deadline_ms);
extern int http_send_all(int fd, const void *buf, size_t len, int deadline_ms);
extern uint8_t g_esp_config[10];
extern void aim_command_write(uint32_t mode, uint32_t duration, int angle,
                              float x, float y, float spread);
extern uint64_t g_stick_hold_start;
extern float g_trajectory_min;
extern float g_trajectory_exposure;
extern uint64_t g_floating_last_us;
extern uint32_t g_floating_record;
extern int ndc1_object_track(void *state, const void *record, uint32_t index);
extern int remote_entity_fetch(void *vtable, uint64_t remote_addr, void *out);
extern void entity_bank_update(void *payload);
extern void player_bank_update(void *payload);
extern void render_cache_update(void *payload);
extern void aim_plan_callback(int index, float value);
extern int frame_context_verify(const uint32_t *ctx, void *snapshot);
extern void config_float_record(uint32_t id, float value);
extern uint64_t g_response_last_ms;
extern int g_response_owner_tid;
extern uint8_t g_response_record[0x28];
extern int g_response25_owner_tid;
extern int g_response29_owner_tid;
extern uint8_t g_response29_record[0x28];
extern uint64_t g_modal_verify_ms;
extern int g_modal_last_result;
extern uint64_t g_local_profile_slot;
extern int g_theme_last_id;
extern int g_theme_last_variant;
extern uint64_t g_local_profile;
#define THEME_BGR_DARK 0
#define THEME_BGR_GHOSTTRAIN 1
extern uint64_t g_aura_plan_id;
extern int guarded_far_hook_install(uintptr_t rva, uint32_t len, const char *sha,
                                    void *orig_word, void *handler, int flags);
extern void font_detour_handler(void);
extern const char FONT_DETOUR_SHA[];
extern uint64_t g_glyph_serial;
extern uint64_t g_font_context_serial;
extern uint64_t g_font_pair_a;
extern uint64_t g_font_pair_b;
extern int g_font_override_armed;
extern uint64_t g_action25_last_ms;
extern uint64_t g_action25_window;
extern uint64_t g_action25_pin_mask;
extern uint32_t g_action25_last_id;
extern void aura_worker_plan_commit(void *bank);
extern uint64_t g_entity_array;
extern uint32_t g_entity_count;
extern uint64_t g_selection_slot;
extern uint8_t g_battle_state[0x1ce0];
extern float g_dps_scope_value;
extern uint64_t g_dps_last_log_us;

typedef int (*game_call6)(uint64_t, uint64_t, uint64_t, int, int, int);
typedef int (*game_call4)(uint64_t, uint64_t, uint64_t, uint64_t);
typedef int (*game_call1)(uint64_t);


void battle_restore_rebind(void)
{
    uint64_t *array = (uint64_t *)g_entity_array;
    uint32_t count = g_entity_count;

    for (uint32_t i = 0; i < count && i < 0x50; i++) {
        uint64_t obj = array[(size_t)i * 0x90 / 8 + 0xd8 / 8];

        if (obj == 0)
            continue;
        g_selection_slot = obj;
        break;
    }
}

void dps_meter_capture(const uint32_t *frame_ctx)
{
    uint8_t snapshot[0xd0];
    uint32_t damage;
    uintptr_t obj;

    if (frame_ctx == NULL)
        return;
    if (!frame_context_verify(frame_ctx, snapshot))
        return;
    obj = *(uintptr_t *)snapshot;
    if (obj == 0)
        return;
    if (mem_read(g_remote_handle, obj + 0x3c, &damage, 4) != 1)
        return;
    dps_scope_update((float)damage, 1, 0);
}


void feed_status_latch(void)
{
    uint64_t now_ms = source_now_us() / 1000ull;

    *(uint64_t *)0x214948 = now_ms;
    *(uint64_t *)0x214950 = now_ms;
    if (g_ability_state == 2)
        g_ability_last_id = (uint32_t)g_bound_id2;
}

void action29_antitamper_fill(void *record)
{
    memset(record, 0, 0x60);
    *(uint32_t *)record = 0x1d;
    *(uint32_t *)((char *)record + 4) = (uint32_t)g_bound_id2;
    *(uint64_t *)((char *)record + 8) = g_bound_obj;
    *(uint32_t *)((char *)record + 0x10) = g_aura_counters[0];
    *(uint32_t *)((char *)record + 0x14) = g_aura_counters[1];
}

int action29_dispatch_and_verify(const void *record)
{
    uint8_t response[0x60];
    uint64_t dispatch = *(uint64_t *)0x214d98;
    uint64_t commit = *(uint64_t *)0x214da0;

    if (dispatch == 0 || commit == 0)
        return 0;
    ((void (*)(const void *)) dispatch)(record);
    ((void (*)(void)) commit)();
    if (mem_read(g_remote_handle, *(uintptr_t *)((char *)record + 8), response, 0x60) != 1)
        return 0;
    return memcmp(response, record, 0x60) == 0;
}

float config_float_get(uint32_t id)
{
    float value;

    if (id == 0 || id > 100000)
        return 0.0f;
    value = *(float *)(void *)(uintptr_t)0x22d180;
    config_float_record(id, value);
    return value;
}


static const uint32_t cached_response_actions[] = {22, 23, 26, 42, 68, 121, 122, 132};

int cached_response_fetch(uint32_t action, void *out)
{
    uint64_t now_ms = source_now_us() / 1000ull;

    if (action == 0x17 && now_ms - g_response_last_ms < 501)
        return 0;
    for (size_t i = 0; i < sizeof cached_response_actions / sizeof cached_response_actions[0]; i++) {
        if (cached_response_actions[i] != action)
            continue;
        if (g_response_owner_tid != gettid())
            return 0;
        memcpy(out, g_response_record, 0x28);
        g_response_last_ms = now_ms;
        return 1;
    }
    return 0;
}


int cached_response_fetch_25(void *out)
{
    if (g_response25_owner_tid != gettid())
        return 0;
    memcpy(out, (void *)0x2170b8, 0x28);
    return 1;
}


int cached_response_fetch_29(void *out)
{
    if (g_response29_owner_tid != gettid())
        return 0;
    memcpy(out, g_response29_record, 0x28);
    return 1;
}


int game_call6_block(uint64_t a, uint64_t b, uint64_t c, int d, int e, int f)
{
    (void)a; (void)b; (void)c; (void)d; (void)e; (void)f;
    return 0;
}

int game_call4_block(uint64_t a, uint64_t b, uint64_t c, uint64_t d)
{
    (void)a; (void)b; (void)c; (void)d;
    return 0;
}

int game_call1_block(uint64_t a)
{
    (void)a;
    return 1;
}

int ui_modal_state_guard(int mode)
{
    uint64_t now_ms = source_now_us() / 1000ull;
    const uint32_t *expect;

    if (mode != 2)
        return 1;
    if (now_ms - g_modal_verify_ms < 500)
        return g_modal_last_result;
    g_modal_verify_ms = now_ms;
    expect = (const uint32_t *)0x220788;
    if (*(const uint32_t *)(void *)(uintptr_t)0x220790 != expect[0])
        g_modal_last_result = 0;
    else if (*(const uint32_t *)(void *)(uintptr_t)0x220798 != expect[1])
        g_modal_last_result = 0;
    else
        g_modal_last_result = 1;
    return g_modal_last_result;
}


int local_player_profile_detect(uint64_t profile)
{
    uint64_t account_id;

    if ((uintptr_t)profile != g_game_base + 0x11f1a20
        && (uintptr_t)profile != g_game_base + 0x121f9e0)
        return -1;
    if (mem_read(g_remote_handle, profile + 0x18, &account_id, 8) != 1)
        return -1;
    if (account_id < 41000000ull || account_id >= 42000000ull)
        return -1;
    g_local_profile_slot = account_id - 41000000ull;
    theme_select_request(g_theme_last_id, g_theme_last_variant);
    return (int)g_local_profile_slot;
}


void game_671860_intercept(uint64_t obj, float *args, uint32_t mode)
{
    float scale;

    if (mode != 2)
        return;
    if (mem_read(g_remote_handle, obj + 0xd0, &scale, 4) != 1)
        return;
    if (args[0] < 1.0f || args[0] > 30000.0f)
        return;
    args[0] *= scale * 0.5f;
    args[1] *= scale * 7.5f;
}

int remote_write_and_verify(uintptr_t addr, uint64_t value)
{
    uint32_t current;
    uint32_t patch[2];

    if (mem_read(g_remote_handle, addr, &current, 4) != 1)
        return 0;
    patch[0] = (uint32_t)(value >> 32);
    patch[1] = 0;
    if (remote_write((uint32_t)g_remote_write_fd, patch, 4, addr + 0x1f4) != 4)
        return 0;
    if (mem_read(g_remote_handle, addr + 0x1f4, &current, 4) != 1)
        return 0;
    return current == patch[0];
}

void theme_input_toggle(int idx, int code)
{
    uint64_t bg;

    if (idx >= 2 || code > 1)
        return;
    if (mem_read(g_remote_handle, g_local_profile + 0x930, &bg, 8) != 1)
        return;
    if (idx == 0)
        theme_apply(THEME_BGR_DARK, 0);
    else
        theme_apply(THEME_BGR_GHOSTTRAIN, 0);
}

#define THEME_BGR_DARK 0
#define THEME_BGR_GHOSTTRAIN 1

int aura_plan_validity_gate(const void *plan)
{
    const uint64_t *flags = (const uint64_t *)0x2284e8;
    uint64_t acc = 0;

    if (plan == NULL)
        return 0;
    if (*(const uint64_t *)plan != g_aura_plan_id)
        return 0;
    for (int i = 0; i < 8; i++)
        acc |= flags[i] & *(const uint64_t *)((char *)plan + 8 + i * 8);
    return acc == *(const uint64_t *)((char *)plan + 0x48);
}


void fonts_detour_commit(void)
{
    uint64_t orig_word = 0xd103c3ffu;
    void *handler = (void *)(uintptr_t)font_detour_handler;

    if (!guarded_far_hook_install(0xd06d98, 4, FONT_DETOUR_SHA, &orig_word, handler, 0))
        diag_emit("script_port_fonts", "install_rolled_back", NULL);
}


void gl_upload_hook(void *ctx)
{
    uint64_t serial;

    if (ctx == NULL)
        return;
    serial = ++g_glyph_serial;
    if (serial != g_font_context_serial) {
        g_font_context_serial = serial;
        memset((void *)0x285d0, 0, 0x285d0);
    }
}


void font_context_destroy_hook(void)
{
    g_font_pair_a = 0;
    g_font_pair_b = 0;
}


void counter_tick_forward(void (*original)(void))
{
    __atomic_add_fetch((uint32_t *)0x209c24, 1, __ATOMIC_SEQ_CST);
    __atomic_clear((uint8_t *)0x209c2c, __ATOMIC_RELEASE);
    if (original != NULL)
        original();
}

void font_resource_name_callback(const char *name, int len)
{
    if (len >= 6 || strncmp(name, "Font", 4) != 0)
        return;
    g_font_override_armed = 1;
    __atomic_add_fetch((uint32_t *)0x1cfaf8, 1, __ATOMIC_SEQ_CST);
}


void event_route_register(uint32_t game_mode)
{
    if (g_fn_register == NULL || g_fn_publish_routes == NULL)
        return;
    ((int (*)(uint32_t)) g_fn_register)(game_mode);
    ((int (*)(void)) g_fn_publish_routes)();
}

int action25_dispatch_window(uint64_t now_ms)
{
    if (now_ms - g_action25_last_ms < g_action25_window)
        return 0;
    g_action25_last_ms = now_ms;
    return 1;
}


void action25_dispatch_pinned(uint64_t pins)
{
    if ((pins & g_action25_pin_mask) != g_action25_pin_mask)
        return;
    g_action25_last_id++;
}


void aura_snapshot_emit(void)
{
    uint64_t *bank = (uint64_t *)0x216f18;

    if (bank[0] == 0)
        return;
    aura_worker_plan_commit(bank);
}


void battle_profile_capture(const void *event)
{
    uint64_t *bank = (uint64_t *)0x216f18;
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);
    memset(bank, 0, 0x198);
    bank[0] = 1;
    for (int i = 0; i < 26; i++)
        bank[1 + i] = *(const uint64_t *)((const char *)event + (size_t)i * 8);
    bank[27] = (uint64_t)ts.tv_sec * 1000ull + (uint64_t)ts.tv_nsec / 1000000ull + 2000ull;
}

void battle_session_publish(void)
{
    uint64_t *bank = (uint64_t *)0x21e730;

    bank[0] = 1;
    for (int i = 0; i < 0x1ce0 / 8; i += 0x90 / 8)
        bank[1 + i / (0x90 / 8)] = *(uint64_t *)((char *)g_battle_state + (size_t)i);
}

void team_info_refresh(uint64_t world)
{
    uint32_t count;

    if (world == 0)
        return;
    if (pthread_mutex_lock((pthread_mutex_t *)(void *)(g_game_base + 0x12fd3d0)) != 0)
        return;
    if (mem_read(g_remote_handle, g_game_base + 0x12fd3fc, &count, 4) == 1 && count <= 10)
        mem_read(g_remote_handle, g_game_base + 0x12fd4c8, (void *)(g_game_base + 0x12fd4c8), count * 200);
    pthread_mutex_unlock((pthread_mutex_t *)(void *)(g_game_base + 0x12fd3d0));
}

void floating_number_gate(float value, uint32_t record)
{
    uint64_t now_us = source_now_us();

    if (value > 220.0f || value < 12.0f)
        return;
    if (now_us - g_floating_last_us < 501000ull)
        return;
    g_floating_last_us = now_us;
    g_floating_record = record;
}


static const uint32_t autododge_event_ids[] = {0x4e87, 0x4e88, 0x4e89, 0x5eed, 0x5f4f, 0x60c9};

void autododge_event_dispatch(uint32_t event_id, void *payload)
{
    int which = -1;

    for (size_t i = 0; i < sizeof autododge_event_ids / sizeof autododge_event_ids[0]; i++) {
        if (autododge_event_ids[i] == event_id)
            which = (int)i;
    }
    if (which < 0)
        return;
    switch (which) {
    case 0:
        entity_bank_update(payload);
        break;
    case 1:
        player_bank_update(payload);
        break;
    case 2:
        render_cache_update(payload);
        break;
    case 3:
    case 4:
        entity_bank_update(payload);
        break;
    case 5:
        render_cache_update(payload);
        break;
    }
}


void aim_plan_record_emit(const float *plan)
{
    float bounded[5];

    for (int i = 0; i < 5; i++) {
        bounded[i] = plan[i];
        if (bounded[i] > 200.0f)
            bounded[i] = 200.0f;
        if (bounded[i] < -200.0f)
            bounded[i] = -200.0f;
    }
    aim_plan_callback(0, bounded[0] / 300.0f);
    aim_plan_callback(1, bounded[1] / 300.0f);
    aim_plan_callback(2, bounded[2] * 300.0f);
    aim_plan_callback(3, bounded[3] / 300.0f);
    aim_plan_callback(4, bounded[4] * 300.0f);
}


int event_read_context_fingerprint(const uint32_t *event, uint64_t bank[7])
{
    uint64_t chain[3];

    if (event == NULL)
        return 0;
    if (mem_read(g_remote_handle, g_game_base + 0x1307e20, chain, 8) != 1)
        return 0;
    if (chain[0] == 0)
        return 0;
    if (mem_read(g_remote_handle, chain[0] + 0x90, &chain[1], 8) != 1)
        return 0;
    bank[0] = chain[0];
    bank[1] = chain[1];
    bank[2] = (uint64_t)(uintptr_t)event;
    bank[3] = *(const uint64_t *)event;
    bank[4] = *(const uint64_t *)(event + 2);
    bank[5] = *(const uint64_t *)(event + 4);
    bank[6] = 0;
    return 1;
}

void aim_touch_capture(const float *world, int out[2])
{
    float px;
    float py;

    if (world[0] < 200.0f || world[1] < 200.0f)
        return;
    px = world[0] * 300.0f;
    py = world[1] * 300.0f;
    out[0] = (int)px;
    out[1] = (int)py;
}


/* ===== FUN_00149fc4 @ 00149fc4 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00149fc4(int *param_1)

{
  long lVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  undefined8 *puVar5;
  int local_3c;
  long local_38;
  
  iVar3 = DAT_00209cd8;
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  if ((DAT_00209cd8 != 0) && (iVar2 = gettid(), iVar3 == iVar2)) {
    local_3c = -1;
    if (((((param_1 == (int *)0x0) ||
          (((*param_1 != 1 || (param_1[1] != 0x30)) || (param_1[9] == 0)))) ||
         (((piVar4 = *(int **)(param_1 + 4), piVar4 == (int *)0x0 || (*(long *)(param_1 + 2) == 0))
          || (*piVar4 != 1)))) || ((piVar4[1] != 0xd0 || (piVar4[0x1a] == 0)))) ||
       ((*(long *)(*(long *)(param_1 + 2) + 8) != *(long *)(piVar4 + 6) ||
        (iVar3 = FUN_0014a4ac(piVar4,&local_3c), iVar3 == 0)))) {
      iVar3 = FUN_0014a4ac(&DAT_001e00e0,&local_3c);
      if ((iVar3 == 0) || (local_3c < DAT_001e01b8)) {
        DAT_001e01c0 = 0;
        uRam00000000001e00e8 = 0;
        _DAT_001e00e0 = 0;
        uRam00000000001e00f8 = 0;
        _DAT_001e00f0 = 0;
        DAT_001e0108 = 0;
        DAT_001e0100 = 0;
        DAT_001e0118 = 0;
        DAT_001e0110 = 0;
        uRam00000000001e0128 = 0;
        _DAT_001e0120 = 0;
        uRam00000000001e0138 = 0;
        _DAT_001e0130 = 0;
        uRam00000000001e0148 = 0;
        _DAT_001e0140 = 0;
        uRam00000000001e0158 = 0;
        _DAT_001e0150 = 0;
        uRam00000000001e0168 = 0;
        _DAT_001e0160 = 0;
        uRam00000000001e0178 = 0;
        _DAT_001e0170 = 0;
        uRam00000000001e0188 = 0;
        _DAT_001e0180 = 0;
        uRam00000000001e0198 = 0;
        _DAT_001e0190 = 0;
        uRam00000000001e01a8 = 0;
        _DAT_001e01a0 = 0;
        _DAT_001e01b8 = 0;
        DAT_001e01b0 = 0;
      }
    }
    else {
      puVar5 = *(undefined8 **)(param_1 + 4);
      if ((((DAT_001e0100 != puVar5[4]) || (DAT_001e0108 != puVar5[5])) ||
          (DAT_001e0110 != puVar5[6])) || ((DAT_001e0118 != puVar5[7] || (local_3c < DAT_001e01b8)))
         ) {
        DAT_001e01c0 = 0;
        _DAT_001e01b8 = 0;
        DAT_001e01b0 = 0;
      }
      uRam00000000001e00e8 = puVar5[1];
      _DAT_001e00e0 = *puVar5;
      DAT_001e0118 = puVar5[7];
      DAT_001e0110 = puVar5[6];
      uRam00000000001e0128 = puVar5[9];
      _DAT_001e0120 = puVar5[8];
      uRam00000000001e00f8 = puVar5[3];
      _DAT_001e00f0 = puVar5[2];
      DAT_001e0108 = puVar5[5];
      DAT_001e0100 = puVar5[4];
      uRam00000000001e0158 = puVar5[0xf];
      _DAT_001e0150 = puVar5[0xe];
      uRam00000000001e0168 = puVar5[0x11];
      _DAT_001e0160 = puVar5[0x10];
      uRam00000000001e0138 = puVar5[0xb];
      _DAT_001e0130 = puVar5[10];
      uRam00000000001e0148 = puVar5[0xd];
      _DAT_001e0140 = puVar5[0xc];
      uRam00000000001e0198 = puVar5[0x17];
      _DAT_001e0190 = puVar5[0x16];
      uRam00000000001e01a8 = puVar5[0x19];
      _DAT_001e01a0 = puVar5[0x18];
      uRam00000000001e0178 = puVar5[0x13];
      _DAT_001e0170 = puVar5[0x12];
      uRam00000000001e0188 = puVar5[0x15];
      _DAT_001e0180 = puVar5[0x14];
      _DAT_001e01b8 = CONCAT44(DAT_001e01bc,local_3c);
    }
  }
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


/* ===== FUN_0014a9a0 @ 0014a9a0 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0014a9a0(void)

{
  ulong uVar1;
  undefined *puVar2;
  long lVar3;
  ulong uVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  uint uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  FILE *__stream;
  char *pcVar11;
  int *piVar12;
  undefined8 *puVar13;
  long lVar14;
  long *plVar15;
  int local_2e8;
  int iStack_2e4;
  undefined8 *local_2e0;
  long lStack_2d8;
  undefined8 uStack_2d0;
  undefined8 uStack_2c8;
  undefined8 local_2c0;
  undefined8 uStack_2b8;
  undefined8 local_2b0;
  byte local_2a0;
  char local_29f;
  undefined4 local_29e;
  undefined2 uStack_29a;
  undefined2 local_27c;
  long local_270;
  long lStack_268;
  long local_260;
  long lStack_258;
  long local_70;
  
  uVar4 = DAT_0020af68;
  lVar3 = tpidr_el0;
  local_70 = *(long *)(lVar3 + 0x28);
  if ((DAT_0020af64 != '\x01') || (DAT_0020af68 != DAT_0020af70)) goto LAB_0014acb4;
  __stream = fopen("/proc/self/maps","r");
  if (__stream == (FILE *)0x0) goto LAB_0014acb8;
  pcVar11 = fgets((char *)&local_270,0x200,__stream);
  if (pcVar11 == (char *)0x0) {
    fclose(__stream);
    goto LAB_0014acb4;
  }
  iVar7 = 0;
  local_2e8 = 0;
  iStack_2e4 = 0;
  do {
    local_2b0 = 0;
    local_2e0 = (undefined8 *)0x0;
    local_2c0 = local_2c0 & 0xffffff0000000000;
    if ((((uVar4 < 0xffffffffffffffe0) &&
         (iVar5 = sscanf((char *)&local_270,"%llx-%llx %4s",&local_2b0,&local_2e0,&local_2c0),
         iVar5 == 3)) && (local_2b0 <= uVar4)) && ((undefined8 *)(uVar4 + 0x20) <= local_2e0)) {
      if ((int)local_2c0 == 0x70782d72 && local_2c0._4_1_ == '\0') {
        iVar7 = 1;
      }
      else if ((int)local_2c0 == 0x702d2d72 && local_2c0._4_1_ == '\0') {
        iVar7 = 2;
      }
    }
    lVar14 = DAT_001e0978;
    local_2b0 = 0;
    local_2e0 = (undefined8 *)0x0;
    local_2c0 = local_2c0 & 0xffffff0000000000;
    uVar1 = DAT_001e0978 + 0xd06d98;
    if (((uVar1 < 0xfffffffffffffa54) &&
        (iVar5 = sscanf((char *)&local_270,"%llx-%llx %4s",&local_2b0,&local_2e0,&local_2c0),
        iVar5 == 3)) && ((local_2b0 <= uVar1 && ((undefined8 *)(lVar14 + 0xd07344U) <= local_2e0))))
    {
      if ((int)local_2c0 == 0x70782d72 && local_2c0._4_1_ == '\0') {
        local_2e8 = 1;
      }
      else if ((int)local_2c0 == 0x702d2d72 && local_2c0._4_1_ == '\0') {
        local_2e8 = 2;
      }
    }
    local_2b0 = 0;
    local_2e0 = (undefined8 *)0x0;
    local_2c0 = local_2c0 & 0xffffff0000000000;
    iVar5 = sscanf((char *)&local_270,"%llx-%llx %4s",&local_2b0,&local_2e0,&local_2c0);
    if (((iVar5 == 3) && (local_2b0 < 0x14b2ed)) && ((undefined8 *)0x14b2ef < local_2e0)) {
      if ((int)local_2c0 == 0x70782d72 && local_2c0._4_1_ == '\0') {
        iStack_2e4 = 1;
      }
      else if ((int)local_2c0 == 0x702d2d72 && local_2c0._4_1_ == '\0') {
        iStack_2e4 = 2;
      }
    }
    pcVar11 = fgets((char *)&local_270,0x200,__stream);
  } while (pcVar11 != (char *)0x0);
  uVar6 = fclose(__stream);
  __stream = (FILE *)(ulong)uVar6;
  if (iVar7 == 1) {
LAB_0014ac40:
    __stream = (FILE *)FUN_001428fc(__stream,DAT_0020af68,&local_270,0x20);
    if ((int)__stream == 0) goto LAB_0014acb8;
    if (((local_270 == _DAT_0020af78 && lStack_268 == DAT_0020af80) && local_260 == DAT_0020af88) &&
        lStack_258 == DAT_0020af90) {
      iVar7 = FUN_0014b1d0(0xd06d9c,0x5a8,
                           "f1c12195f81d03ce0277573babf6abbf105d048a0e50fd1e123fa82f16a326de");
      __stream = (FILE *)(ulong)(iVar7 != 0);
      goto LAB_0014acb8;
    }
  }
  else {
    if ((((iVar7 != 2) || (local_2e8 != 2)) || (iStack_2e4 != 2)) ||
       (uVar6 = open("/proc/self/exe",0x80000), (int)uVar6 < 0)) goto LAB_0014acb4;
    piVar12 = (int *)__errno();
    iVar7 = *piVar12;
    puVar13 = (undefined8 *)FUN_001bd828(&DAT_001cfcd8);
    puVar2 = &DAT_00116561;
    if (uVar6 != 0x7fff0001) {
      puVar2 = &DAT_00117c1b;
    }
    puVar13[5] = 0;
    puVar13[6] = 0;
    *puVar13 = puVar2;
    puVar13[1] = &DAT_00116571;
    local_2e0 = &local_2b0;
    plVar15 = puVar13 + 4;
    *plVar15 = 0;
    puVar13[2] = &DAT_0011a7d3;
    puVar13[3] = 0x40;
    lStack_2d8 = 0x40;
    local_2c0 = 0;
    uStack_2b8 = 0x40;
    if (uVar6 != 0x7fff0001) {
      lVar14 = syscall(0x43,(ulong)uVar6,&local_2b0,0x40,0);
    }
    else {
      uVar8 = getpid();
      lVar14 = syscall(0x10e,(ulong)uVar8,&local_2e0,1,&local_2c0,1,0);
    }
    if (lVar14 < 0) {
      iVar5 = *piVar12;
      if (iVar5 == 4) {
        if (uVar6 == 0x7fff0001) {
          uVar8 = getpid();
          lVar14 = syscall(0x10e,(ulong)uVar8,&local_2e0,1,&local_2c0,1,0);
        }
        else {
          lVar14 = syscall(0x43,(ulong)uVar6,&local_2b0,0x40,0);
        }
        if (-1 < lVar14) goto LAB_0014addc;
        iVar5 = *piVar12;
        if (iVar5 != 4) goto LAB_0014b08c;
        if (uVar6 == 0x7fff0001) {
          uVar8 = getpid();
          lVar14 = syscall(0x10e,(ulong)uVar8,&local_2e0,1,&local_2c0,1,0);
        }
        else {
          lVar14 = syscall(0x43,(ulong)uVar6,&local_2b0,0x40,0);
        }
        if (-1 < lVar14) goto LAB_0014addc;
        iVar5 = *piVar12;
        if (iVar5 != 4) goto LAB_0014b08c;
        if (uVar6 == 0x7fff0001) {
          uVar8 = getpid();
          lVar14 = syscall(0x10e,(ulong)uVar8,&local_2e0,1,&local_2c0,1,0);
        }
        else {
          lVar14 = syscall(0x43,(ulong)uVar6,&local_2b0,0x40,0);
        }
        if (-1 < lVar14) goto LAB_0014addc;
        iVar5 = *piVar12;
        if (iVar5 != 4) goto LAB_0014b08c;
        if (uVar6 == 0x7fff0001) {
          uVar8 = getpid();
          lVar14 = syscall(0x10e,(ulong)uVar8,&local_2e0,1,&local_2c0,1,0);
        }
        else {
          lVar14 = syscall(0x43,(ulong)uVar6,&local_2b0,0x40,0);
        }
        if (-1 < lVar14) goto LAB_0014addc;
        iVar5 = *piVar12;
        if (iVar5 != 4) goto LAB_0014b08c;
        if (uVar6 == 0x7fff0001) {
          uVar8 = getpid();
          lVar14 = syscall(0x10e,(ulong)uVar8,&local_2e0,1,&local_2c0,1,0);
        }
        else {
          lVar14 = syscall(0x43,(ulong)uVar6,&local_2b0,0x40,0);
        }
        if (-1 < lVar14) goto LAB_0014addc;
        iVar5 = *piVar12;
        if (iVar5 != 4) goto LAB_0014b08c;
        if (uVar6 == 0x7fff0001) {
          uVar8 = getpid();
          lVar14 = syscall(0x10e,(ulong)uVar8,&local_2e0,1,&local_2c0,1,0);
        }
        else {
          lVar14 = syscall(0x43,(ulong)uVar6,&local_2b0,0x40,0);
        }
        if (-1 < lVar14) goto LAB_0014addc;
        iVar5 = *piVar12;
        if (iVar5 != 4) goto LAB_0014b08c;
        if (uVar6 == 0x7fff0001) {
          uVar8 = getpid();
          lVar14 = syscall(0x10e,(ulong)uVar8,&local_2e0,1,&local_2c0,1,0);
        }
        else {
          lVar14 = syscall(0x43,(ulong)uVar6,&local_2b0,0x40,0);
        }
        *plVar15 = lVar14;
        goto joined_r0x0014b0c4;
      }
LAB_0014b08c:
      *plVar15 = lVar14;
    }
    else {
LAB_0014addc:
      *plVar15 = lVar14;
joined_r0x0014b0c4:
      if (-1 < lVar14) {
        *(undefined4 *)(puVar13 + 6) = 0;
        *piVar12 = iVar7;
        close(uVar6);
        if (((((lVar14 == 0x40) && ((int)local_2b0 == 0x464c457f)) &&
             ((local_2b0._4_1_ == '\x02' &&
              ((local_2b0._5_1_ == '\x01' && (local_2b0._6_1_ == '\x01')))))) &&
            ((local_2a0 & 0xfe) == 2)) &&
           ((CONCAT26(local_27c,CONCAT24(uStack_29a,local_29e)) == 0x4000000001003e &&
            (local_29f == '\0')))) {
          lStack_2d8 = 0;
          local_2e0 = (undefined8 *)0x0;
          uStack_2c8 = 0;
          uStack_2d0 = 0;
          iVar7 = dladdr(FUN_0014b2ec,&local_2e0);
          if ((iVar7 != 0) && (lStack_2d8 != 0)) {
            uVar9 = FUN_0014b700(DAT_001e0978,DAT_001e0978 + 0xd06d98,0x5ac);
            uVar10 = FUN_0014b700(lStack_2d8,FUN_0014b2ec,4);
            __stream = (FILE *)FUN_0014b640(&local_2b0,uVar9,uVar10);
            if ((int)__stream == 0) goto LAB_0014acb8;
            goto LAB_0014ac40;
          }
        }
        goto LAB_0014acb4;
      }
      iVar5 = *piVar12;
    }
    *(int *)(puVar13 + 6) = iVar5;
    close(uVar6);
  }
LAB_0014acb4:
  __stream = (FILE *)0x0;
LAB_0014acb8:
  if (*(long *)(lVar3 + 0x28) == local_70) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(__stream);
}


/* ===== FUN_0014b8e8 @ 0014b8e8 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0014b8e8(uint param_1)

{
  long lVar1;
  undefined1 auVar2 [16];
  long lVar3;
  int iVar4;
  uint uVar5;
  ulong uVar6;
  size_t __n;
  char *pcVar7;
  ulong *puVar8;
  ulong uVar9;
  uint uVar10;
  char *pcVar11;
  uint uVar12;
  long lVar13;
  undefined1 auVar14 [16];
  ulong local_9b8;
  ulong local_9b0;
  int local_9a4;
  ulong local_9a0;
  long local_998;
  long local_990;
  undefined8 uStack_988;
  uint local_980;
  char local_978 [128];
  undefined4 local_8f8;
  undefined1 uStack_8f4;
  undefined1 uStack_8f3;
  undefined1 uStack_8f2;
  undefined4 uStack_8f1;
  undefined2 uStack_8ed;
  undefined1 uStack_8eb;
  undefined2 uStack_8ea;
  char local_8e8;
  ulong auStack_878 [256];
  long local_78;
  
  lVar1 = tpidr_el0;
  local_78 = *(long *)(lVar1 + 0x28);
  local_9a0 = 0;
  local_998 = 0;
  local_9a4 = 0;
  uVar6 = FUN_0014cd8c(&local_998,&local_9a0,&local_9a4);
  if ((int)uVar6 == 0) goto LAB_0014bd80;
  if (local_9a4 < 1) {
    uVar10 = 0;
    uVar12 = 1;
  }
  else {
    lVar13 = 0;
    uVar10 = 0;
    uVar12 = 1;
    do {
      local_9b8 = 0;
      local_9b0 = 0;
      uVar6 = FUN_001428fc(uVar6,local_9a0 + lVar13 * 8,&local_9b0,8);
      if ((((((int)uVar6 != 0) && (0x11fff < local_9b0 + 0x2000)) && ((local_9b0 & 7) == 0)) &&
          ((uVar6 = FUN_001428fc(uVar6,local_9b0,&local_9b8,8), (int)uVar6 != 0 &&
           (0x11fff < local_9b8 + 0x2000)))) &&
         (((local_9b8 & 7) == 0 &&
          ((local_9b8 == DAT_001e0978 + 0x11ab720U &&
           (uVar6 = FUN_0014a6ac(local_9b0 + 0x28,&local_8f8,0x80), (int)uVar6 != 0)))))) {
        lVar3 = CONCAT17((undefined1)uStack_8f1,
                         CONCAT16(uStack_8f2,CONCAT15(uStack_8f3,CONCAT14(uStack_8f4,local_8f8))));
        uVar12 = (uint)(lVar3 == 0x65522d6e6f636f43 &&
                       CONCAT17(uStack_8eb,CONCAT25(uStack_8ed,CONCAT41(uStack_8f1,uStack_8f2))) ==
                       0x72616c75676552) << 2 |
                 (uint)(lVar3 == 0x6f42206169737550 &&
                       CONCAT44(uStack_8f1,
                                CONCAT13(uStack_8f2,
                                         CONCAT12(uStack_8f3,CONCAT11(uStack_8f4,local_8f8._3_1_))))
                       == 0x646c6f42206169) << 1 |
                 (uint)(lVar3 == 0x6e776f445f6b6a6c &&
                       CONCAT26(uStack_8ed,CONCAT42(uStack_8f1,CONCAT11(uStack_8f2,uStack_8f3))) ==
                       0x656d6f636e776f) << 3 |
                 (uint)(local_8f8 == 0x61706d49 &&
                       CONCAT13(uStack_8f2,CONCAT12(uStack_8f3,CONCAT11(uStack_8f4,local_8f8._3_1_))
                               ) == 0x746361) << 4 |
                 (uint)((lVar3 == 0x575f43535f4b4453 &&
                        CONCAT26(uStack_8ea,
                                 CONCAT15(uStack_8eb,CONCAT23(uStack_8ed,uStack_8f1._1_3_))) ==
                        0x7976616548206265) && local_8e8 == '\0') << 5 | uVar12;
        if ((param_1 != 0) &&
           (((uVar6 = FUN_0014a6ac(local_9b0 + 0x18,local_978,0x80), param_1 < 6 &&
             ((int)uVar6 != 0)) && (local_978[0] != '\0')))) {
          __n = strlen(local_978);
          pcVar11 = (&PTR_DAT_001c2d80)[(ulong)param_1 * 3];
          uVar6 = __n;
          do {
            if (*pcVar11 == '\0') break;
            pcVar7 = strchr(pcVar11,0x20);
            if (pcVar7 == (char *)0x0) {
              uVar6 = strlen(pcVar11);
            }
            else {
              uVar6 = (long)pcVar7 - (long)pcVar11;
            }
            if (uVar6 == __n) {
              uVar5 = memcmp(pcVar11,local_978,__n);
              uVar6 = (ulong)uVar5;
              if (uVar5 == 0) {
                uVar6 = FUN_001428fc(uVar6,local_9b0 + 0x4c,&local_990,0x14);
                if ((((int)uVar6 == 0) ||
                    (auVar2._8_8_ = uStack_988, auVar2._0_8_ = local_990, auVar14._8_4_ = 0x1000,
                    auVar14._0_8_ = 0x100000001000, auVar14._12_4_ = 0x1000,
                    auVar14 = NEON_cmhi(auVar2,auVar14,4),
                    (((auVar14 & (undefined1  [16])0x1) != (undefined1  [16])0x0 ||
                     (auVar14 & (undefined1  [16])0x100000000) != (undefined1  [16])0x0) ||
                    (auVar14 & (undefined1  [16])0x1) != (undefined1  [16])0x0) ||
                    (auVar14 & (undefined1  [16])0x100000000) != (undefined1  [16])0x0)) ||
                   (0x1000 < local_980)) goto LAB_0014bd7c;
                auStack_878[uVar10] = local_9b0;
                uVar10 = uVar10 + 1;
                break;
              }
            }
            pcVar11 = pcVar7 + 1;
          } while (pcVar7 != (char *)0x0);
        }
      }
      lVar13 = lVar13 + 1;
    } while (lVar13 < local_9a4);
  }
  DAT_001e12d8._0_4_ = uVar12;
  if ((uVar12 >> (ulong)(param_1 & 0x1f) & 1) != 0) {
    local_990 = 0;
    local_9b0 = 0;
    local_9b8 = local_9b8 & 0xffffffff00000000;
    uVar6 = FUN_0014cd8c(&local_990,&local_9b0,&local_9b8);
    auVar14 = _DAT_0010ef90;
    if ((int)uVar6 == 0) goto LAB_0014bd80;
    if (((local_990 == local_998) && (local_9b0 == local_9a0)) && ((int)local_9b8 == local_9a4)) {
      if (uVar10 != 0) {
        uVar6 = (ulong)uVar10;
        puVar8 = auStack_878;
        do {
          uVar9 = *puVar8;
          uVar6 = uVar6 - 1;
          *(undefined4 *)(uVar9 + 0x5c) = 0x1e;
          *(long *)(uVar9 + 0x54) = auVar14._8_8_;
          *(long *)(uVar9 + 0x4c) = auVar14._0_8_;
          puVar8 = puVar8 + 1;
        } while (uVar6 != 0);
      }
      pcVar11 = (&PTR_DAT_001c2d80)[(long)(int)param_1 * 3];
      (*(code *)(DAT_001e0978 + 0x66ae58))(local_978,pcVar11);
      (*(code *)(DAT_001e0978 + 0x600250))(local_998,local_978);
      (*(code *)(DAT_001e0978 + 0x66ad48))(local_978);
      uVar6 = FUN_0014a6ac(local_998 + 0x60,&local_8f8,0x80);
      if ((int)uVar6 != 0) {
        iVar4 = strcmp((char *)&local_8f8,pcVar11);
        uVar6 = (ulong)(iVar4 == 0);
      }
      goto LAB_0014bd80;
    }
  }
LAB_0014bd7c:
  uVar6 = 0;
LAB_0014bd80:
  if (*(long *)(lVar1 + 0x28) == local_78) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar6);
}


/* ===== FUN_0014dbbc @ 0014dbbc [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0014dbbc(ulong *param_1)

{
  long lVar1;
  int iVar2;
  undefined8 uVar3;
  char local_84 [4];
  ulong local_80;
  ulong uStack_78;
  ulong local_70;
  ulong uStack_68;
  ulong local_60;
  ulong uStack_58;
  undefined8 local_50;
  ulong local_40;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  local_50 = 0;
  uStack_68 = 0;
  local_70 = 0;
  uStack_58 = 0;
  local_60 = 0;
  local_84[0] = '\x01';
  uStack_78 = 0;
  local_80 = 0;
  if (((int)_DAT_00209b90 != 0) && ((int)DAT_001dfff0 != 0)) {
    uVar3 = FUN_0013c82c();
    if (((int)uVar3 == 0) ||
       (uVar3 = FUN_001428fc(uVar3,DAT_001e0978 + 0x1307e20,&local_80,8), (int)uVar3 == 0))
    goto LAB_0014de24;
    uVar3 = 0;
    if ((local_80 + 0x2000 < 0x12000) || ((local_80 & 7) != 0)) goto LAB_0014de24;
    iVar2 = FUN_001428fc(0,local_80 + 0x50,&local_50,4);
    uVar3 = 0;
    if ((iVar2 == 0) ||
       ((((int)local_50 < 4 || (6 < (int)local_50)) ||
        (uVar3 = FUN_001428fc(0,local_80 + 0x48,(ulong)&local_80 | 8,8), (int)uVar3 == 0))))
    goto LAB_0014de24;
    uVar3 = 0;
    if (((uStack_78 + 0x2000 < 0x12000) || ((uStack_78 & 7) != 0)) ||
       (uVar3 = FUN_001428fc(0,DAT_001e0978 + 0x12ff038,&local_70,8), (int)uVar3 == 0))
    goto LAB_0014de24;
    uVar3 = 0;
    if ((local_70 + 0x2000 < 0x12000) || ((local_70 & 7) != 0)) goto LAB_0014de24;
    local_40 = 0;
    iVar2 = FUN_001428fc(0,local_70,&local_40,8);
    if ((iVar2 != 0) &&
       (((0x11fff < local_40 + 0x2000 && ((local_40 & 7) == 0)) &&
        (local_40 == DAT_001e0978 + 0x11baa20U)))) {
      uVar3 = FUN_0013a78c(DAT_001e0978 + 0x1303f80,&uStack_68);
      if ((int)uVar3 != 0) {
        iVar2 = FUN_001428fc(uVar3,uStack_68 + 0xe4,(long)&local_50 + 4,4);
        uVar3 = 0;
        if (((iVar2 != 0) && (-1 < (long)local_50)) &&
           ((local_50._4_4_ < 0x21 &&
            ((uVar3 = FUN_0013a78c(DAT_001e0978 + 0x12eb9f0,&local_60), (int)uVar3 != 0 &&
             (uVar3 = FUN_0013a78c(local_60 + 0x90,&uStack_58), (int)uVar3 != 0)))))) {
          iVar2 = FUN_001428fc(uVar3,local_60 + 0x19c,local_84,1);
          uVar3 = 0;
          if ((iVar2 != 0) && (local_84[0] == '\0')) {
            uVar3 = 1;
            param_1[1] = uStack_78;
            *param_1 = local_80;
            param_1[3] = uStack_68;
            param_1[2] = local_70;
            param_1[5] = uStack_58;
            param_1[4] = local_60;
            param_1[6] = local_50;
          }
        }
      }
      goto LAB_0014de24;
    }
  }
  uVar3 = 0;
LAB_0014de24:
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar3);
}


/* ===== FUN_0014ea08 @ 0014ea08 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0014ea08(undefined8 param_1,int param_2,int param_3)

{
  long lVar1;
  ulong uVar2;
  char *pcVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  long lVar7;
  void *pvVar8;
  size_t sVar9;
  long lVar10;
  int iVar11;
  ulong uVar12;
  long local_2d8;
  long local_2d0;
  long local_2c8;
  undefined8 local_2c0;
  ulong local_2b8;
  ulong local_2b0;
  byte bStack_2a4;
  undefined8 local_2a8;
  char acStack_240 [96];
  undefined1 auStack_1e0 [96];
  ulong local_180;
  char *local_178 [36];
  long local_58;
  
  lVar1 = tpidr_el0;
  local_58 = *(long *)(lVar1 + 0x28);
  lVar7 = 0;
  if ((((DAT_00209ca0 != '\x01') || ((DAT_0020b06c & 1) != 0)) ||
      (lVar7 = FUN_0013c82c(0), (int)lVar7 == 0)) || (lVar7 = FUN_0013fdb0(), (int)lVar7 == 0))
  goto LAB_0014eba8;
  if (param_2 < 0) {
    param_2 = (int)DAT_001cfb58;
  }
  local_180 = 0;
  local_2a8 = 0;
  local_2b0 = local_2b0 & 0xffffffff00000000;
  iVar4 = FUN_0014fba8(&local_180,&stack0xfffffffffffffd58,&local_2b0);
  lVar7 = 0;
  if ((iVar4 == 0) || (iVar4 = (int)local_2b0, (int)local_2b0 < 1)) goto LAB_0014eba8;
  iVar11 = 0;
  while( true ) {
    local_2b0 = 0;
    iVar5 = FUN_0014fccc(iVar11,0xffffffff,&local_180,&local_2b0);
    if (iVar5 < 0) break;
    if ((iVar5 != 0) && ((int)local_180 == param_2)) {
      pvVar8 = memcpy(&stack0xfffffffffffffd58,&local_180,0x128);
      uVar2 = local_2b0;
      if ((local_2b0 != 0) && ((bStack_2a4 >> 1 & 1) != 0)) {
        uVar12 = local_2b0;
        if (param_3 < 0) goto LAB_0014ebd4;
        local_180 = 0;
        local_2b0 = 0;
        local_2b8 = local_2b8 & 0xffffffff00000000;
        pvVar8 = (void *)FUN_0014fba8(&local_180,&local_2b0,&local_2b8);
        uVar12 = 0;
        if (((int)pvVar8 == 0) || (iVar4 = (int)local_2b8, (int)local_2b8 < 1)) goto LAB_0014eb98;
        iVar11 = 0;
        goto LAB_0014eb54;
      }
      break;
    }
    iVar11 = iVar11 + 1;
    if (iVar4 == iVar11) break;
  }
  goto LAB_0014eba4;
  while( true ) {
    if (((int)pvVar8 != 0) && (uVar12 = local_2b0, (int)local_180 == param_3)) goto LAB_0014eb98;
    iVar11 = iVar11 + 1;
    if (iVar4 == iVar11) break;
LAB_0014eb54:
    local_2b0 = 0;
    pvVar8 = (void *)FUN_0014fccc(iVar11,0xffffffff,&local_180,&local_2b0);
    if ((int)pvVar8 < 0) break;
  }
  uVar12 = 0;
LAB_0014eb98:
  local_2b8 = 0;
  local_2b0 = 0;
  if ((param_3 < 0) || (uVar12 != 0)) {
LAB_0014ebd4:
    local_2b0 = 0;
    local_2b8 = 0;
    lVar7 = FUN_001428fc(pvVar8,DAT_001e0978 + 0x1307cc0,&local_2b0,8);
    if (((int)lVar7 == 0) || ((lVar7 = 0, local_2b0 + 0x2000 < 0x12000 || ((local_2b0 & 7) != 0))))
    goto LAB_0014eba8;
    if (param_3 != -2) {
      lVar7 = FUN_001428fc(0,uVar12 + 0x58,&local_2b8,8);
      if ((int)lVar7 == 0) goto LAB_0014eba8;
      if ((local_2b8 != 0) &&
         ((((local_180 = local_180 & 0xffffffff00000000, local_2b8 + 0x2000 < 0x12000 ||
            ((local_2b8 & 7) != 0)) ||
           (iVar4 = FUN_001428fc(lVar7,local_2b8 + 0x20,&local_180,4), iVar4 == 0)) ||
          (((int)local_180 < 12000000 || (12999999 < (int)local_180)))))) goto LAB_0014eba4;
    }
    sVar9 = strlen(acStack_240);
    iVar4 = (int)sVar9;
    local_178[0] = (char *)0x0;
    local_180 = CONCAT44(iVar4,iVar4);
    pcVar3 = acStack_240;
    if (iVar4 < 8) {
      memcpy(local_178,acStack_240,(long)iVar4);
      pcVar3 = local_178[0];
    }
    local_178[0] = pcVar3;
    (*(code *)(DAT_001e0978 + 0x75da50))(&local_180,0);
    uVar6 = FUN_0014f498(&stack0xfffffffffffffd58);
    lVar7 = 0;
    if (((uVar6 >> 2 & 1) == 0) ||
       (lVar10 = (*(code *)(DAT_001e0978 + 0xd1763c))(acStack_240,auStack_1e0,0), lVar7 = lVar10,
       lVar10 == 0)) goto LAB_0014eba8;
    local_2c8 = 0;
    local_2c0 = 0;
    local_2d8 = 0;
    local_2d0 = 0;
    lVar7 = FUN_0013a78c(lVar10,&local_2d0);
    if ((int)lVar7 == 0) goto LAB_0014eba8;
    if (local_2d0 == DAT_001e0978 + 0x11ad208) {
      iVar4 = FUN_001428fc(lVar7,lVar10 + 0x38,&local_2d8,8);
      lVar7 = 0;
      if ((iVar4 != 0) && (local_2d8 == 0)) {
        if ((((DAT_0020b088 == '\x01') &&
             (((int)_DAT_0020b08c != 0 && (iVar4 = FUN_0014f5d8(lVar10,1), iVar4 == 0)))) ||
            (iVar4 = FUN_0013a78c(DAT_0020b078 + 0x930,&local_2c8), iVar4 == 0)) ||
           (iVar4 = FUN_0013a78c(local_2c8 + 0x38,&local_2c0), iVar4 == 0)) {
          (*(code *)(DAT_001e0978 + 0x5d4864))(lVar10);
          (*(code *)(DAT_001e0978 + 0x11a2830))(lVar10);
          lVar7 = 0;
        }
        else {
          DAT_0020b06c = 1;
          (*(code *)(DAT_001e0978 + 0x594414))(local_2c0,local_2c8);
          (*(code *)(DAT_001e0978 + 0x594138))(local_2c0,lVar10,0);
          *(long *)(DAT_0020b078 + 0x930) = lVar10;
          *(ulong *)(DAT_0020b078 + 0x9a8) = uVar2;
          *(undefined4 *)(DAT_0020b078 + 0x938) = 0xbf800000;
          (*(code *)(DAT_001e0978 + 0xb5b2bc))(DAT_0020b078);
          (*(code *)(DAT_001e0978 + 0x5d4864))(local_2c8);
          (*(code *)(DAT_001e0978 + 0x11a2830))(local_2c8);
          if (local_2b8 == 0) {
            (*(code *)(DAT_001e0978 + 0xcea930))(local_2b0);
          }
          else {
            (*(code *)(DAT_001e0978 + 0xceb908))();
          }
          lVar7 = 1;
          DAT_0020b06c = 0;
          DAT_0020b080 = lVar10;
        }
      }
      goto LAB_0014eba8;
    }
  }
LAB_0014eba4:
  lVar7 = 0;
LAB_0014eba8:
  if (*(long *)(lVar1 + 0x28) != local_58) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(lVar7);
  }
  return;
}


/* ===== FUN_00151bb4 @ 00151bb4 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00151bb4(undefined8 param_1,int *param_2)

{
  char cVar1;
  bool bVar2;
  long lVar3;
  char cVar4;
  int iVar5;
  int iVar6;
  undefined8 uVar7;
  ulong uVar8;
  timespec local_70;
  undefined8 uStack_60;
  undefined8 uStack_58;
  undefined8 local_50;
  undefined8 uStack_48;
  char local_3c [4];
  long local_38;
  
  iVar6 = DAT_00209cd8;
  lVar3 = tpidr_el0;
  local_38 = *(long *)(lVar3 + 0x28);
  if (((((param_2 != (int *)0x0) && (DAT_0020d158 == 0)) && (DAT_00214940 == '\x01')) &&
      (((*param_2 == 2 && (param_2[1] == 0x98)) &&
       ((param_2[2] == 4 &&
        ((*(long *)(param_2 + 4) == DAT_00209d00 && (*(long *)(param_2 + 8) == DAT_001e0978))))))))
     && (((*(long *)(param_2 + 10) == *(long *)(param_2 + 8) + 0xb2e780 &&
          (((*(long *)(param_2 + 0x14) == *(long *)(param_2 + 0xc) &&
            (*(long *)(param_2 + 0x1a) == *(long *)(param_2 + 0xe))) &&
           (*(long *)(param_2 + 0x1c) == *(long *)(param_2 + 0x24))))) &&
         (((*(long *)(param_2 + 0x1e) == 1 && (param_2[0x21] == 0)) &&
          ((((*(long *)(param_2 + 0x14) == DAT_00214958 &&
             ((*(long *)(param_2 + 0x1a) == DAT_00214960 && (DAT_00214cf8 == 0)))) &&
            (DAT_00214c10 == 0)) &&
           ((((DAT_00214c14 == 0 && (DAT_00209cd8 != 0)) && (iVar5 = gettid(), iVar6 == iVar5)) &&
            (DAT_00214b50 != '\0')))))))))) {
    do {
      cVar4 = DAT_001cfe94;
      cVar1 = '\x01';
      bVar2 = (bool)ExclusiveMonitorPass(0x1cfe94,0x10);
      if (bVar2) {
        _DAT_001cfe94 = CONCAT31(DAT_001cfe94_1,1);
        cVar1 = ExclusiveMonitorsStatus();
      }
    } while (cVar1 != '\0');
    if (cVar4 == '\0') {
      iVar6 = clock_gettime(1,&local_70);
      if (iVar6 == 0) {
        uVar8 = local_70.tv_sec * 1000 + (ulong)local_70.tv_nsec / 1000000;
      }
      else {
        uVar8 = 0;
      }
      local_3c[0] = '\0';
      if ((((uVar8 != 0) && (DAT_00214970 != 0)) &&
          ((DAT_00214970 <= uVar8 &&
           ((uVar8 - DAT_00214970 < 0x1f5 &&
            (uVar7 = FUN_00150bf0(*(undefined8 *)(param_2 + 0xc),*(undefined8 *)(param_2 + 0xe)),
            (int)uVar7 != 0)))))) &&
         ((iVar6 = FUN_001428fc(uVar7,*(long *)(param_2 + 0xc) + 0xf7a,local_3c,1), iVar6 != 0 &&
          ((local_3c[0] == '\x01' &&
           (iVar6 = FUN_00157280(*(undefined8 *)(param_2 + 0xe),*(undefined8 *)(param_2 + 0x24),
                                 &local_70), iVar6 != 0)))))) {
        DAT_00214e00 = local_70.tv_nsec;
        DAT_00214df8 = local_70.tv_sec;
        DAT_00214e10 = uStack_58;
        DAT_00214e08 = uStack_60;
        DAT_00214e20 = uStack_48;
        DAT_00214e18 = local_50;
        DAT_00214de8 = DAT_00214a68;
        DAT_00214db8 = *(undefined8 *)(param_2 + 0xe);
        DAT_00214db0 = *(undefined8 *)(param_2 + 0xc);
        DAT_00214df0 = DAT_00214ab8;
        DAT_00214dc0 = *(undefined8 *)(param_2 + 0x24);
        DAT_00214dd8 = *(undefined8 *)(param_2 + 0x20);
        DAT_00214dd0 = *(undefined8 *)(param_2 + 0x1e);
        DAT_00214df4 = DAT_00214be4;
        _DAT_00214de0 = 0x100000001;
        DAT_00214dc8 = uVar8;
      }
      _DAT_001cfe94 = 0;
    }
  }
  iVar6 = DAT_00209cd8;
  if (((DAT_002147dc == '\x01') && (DAT_00209cd8 != 0)) && (iVar5 = gettid(), iVar6 == iVar5)) {
    iVar6 = (uint)(DAT_0020d158 == 1) << 1;
    if (DAT_0020d158 == 0) {
      iVar6 = 1;
    }
  }
  else {
    iVar6 = 0;
  }
  if (*(long *)(lVar3 + 0x28) != local_38) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar6);
  }
  return;
}


/* ===== FUN_00152844 @ 00152844 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00152844(undefined8 param_1,void *param_2,long *param_3,int param_4)

{
  char *pcVar1;
  char *pcVar2;
  long lVar3;
  bool bVar4;
  int iVar5;
  int iVar6;
  undefined8 local_100;
  undefined8 local_f8;
  undefined8 uStack_f0;
  undefined8 local_e8;
  undefined8 uStack_e0;
  undefined8 local_d8;
  undefined8 uStack_d0;
  undefined8 local_c8;
  undefined8 uStack_c0;
  undefined8 local_b8;
  undefined8 uStack_b0;
  undefined8 local_a8;
  undefined8 uStack_a0;
  undefined8 local_98;
  undefined8 uStack_90;
  undefined8 local_88;
  undefined8 uStack_80;
  undefined8 local_78;
  undefined8 uStack_70;
  undefined8 local_68;
  undefined8 uStack_60;
  undefined8 local_58;
  undefined8 uStack_50;
  long local_48;
  
  lVar3 = tpidr_el0;
  local_48 = *(long *)(lVar3 + 0x28);
  if (DAT_002158f8 == 0) {
    if (DAT_00215678 == 0) goto LAB_00152ac4;
    iVar5 = 0;
    bVar4 = false;
    if ((param_2 != (void *)0x0) && (param_3 != (long *)0x0)) {
      iVar6 = memcmp(param_2,&DAT_00215680,0x98);
      iVar5 = 0;
      if (iVar6 == 0) {
        bVar4 = ((*param_3 == DAT_00215718 && param_3[1] == DAT_00215720) &&
                (param_3[2] == DAT_00215728 && param_3[3] == DAT_00215730)) &&
                ((param_3[4] == DAT_00215738 && param_3[5] == DAT_00215740) &&
                (param_3[6] == DAT_00215748 && param_3[7] == DAT_00215750));
        if ((param_4 == 1) &&
           (((*param_3 == DAT_00215718 && param_3[1] == DAT_00215720) &&
            (param_3[2] == DAT_00215728 && param_3[3] == DAT_00215730)) &&
            ((param_3[4] == DAT_00215738 && param_3[5] == DAT_00215740) &&
            (param_3[6] == DAT_00215748 && param_3[7] == DAT_00215750)))) {
          iVar5 = 1;
          bVar4 = true;
          DAT_002159f0 = DAT_002159f0 + 1;
        }
      }
      else {
        bVar4 = false;
      }
    }
    uStack_50 = 0;
    local_58 = 0;
    uStack_60 = 0;
    local_68 = 0;
    uStack_70 = 0;
    local_78 = 0;
    uStack_80 = 0;
    local_88 = 0;
    uStack_90 = 0;
    local_98 = 0;
    uStack_a0 = 0;
    local_a8 = 0;
    uStack_b0 = 0;
    local_b8 = 0;
    uStack_c0 = 0;
    local_c8 = 0;
    uStack_d0 = 0;
    local_d8 = 0;
    uStack_e0 = 0;
    local_e8 = 0;
    uStack_f0 = 0;
    local_f8 = 0;
    local_100 = DAT_0010e7d0;
    (*DAT_0020d160)(&local_100);
    pcVar1 = "register_commit_refused";
    if (!bVar4) {
      pcVar1 = "register_completion_mismatch";
    }
    pcVar2 = "natural_skill_registers_committed";
    if (iVar5 == 0) {
      pcVar2 = pcVar1;
    }
    FUN_00157680(&DAT_00215680,&DAT_002157e0,&DAT_002158b0,&local_100,pcVar2,iVar5);
    memset(&DAT_00215678,0,0x280);
  }
  else {
    if (((param_2 != (void *)0x0) && (param_3 != (long *)0x0)) &&
       (iVar5 = memcmp(param_2,&DAT_00215900,0x98), iVar5 == 0)) {
      if ((param_4 != 0) &&
         (((((((*param_3 == DAT_00215998 && param_3[1] == DAT_002159a0) &&
              param_3[2] == DAT_002159a8) && param_3[3] == DAT_002159b0) &&
            param_3[4] == DAT_002159b8) && param_3[5] == DAT_002159c0) && param_3[6] == DAT_002159c8
          ) && param_3[7] == DAT_002159d0)) {
        DAT_00214d90 = DAT_00214d90 + 1;
      }
    }
    DAT_002159e8 = 0;
    _DAT_00215910 = 0;
    _DAT_00215908 = 0;
    _DAT_00215920 = 0;
    _DAT_00215918 = 0;
    _DAT_00215930 = 0;
    _DAT_00215928 = 0;
    _DAT_00215940 = 0;
    _DAT_00215938 = 0;
    _DAT_00215950 = 0;
    _DAT_00215948 = 0;
    _DAT_00215960 = 0;
    _DAT_00215958 = 0;
    _DAT_00215970 = 0;
    _DAT_00215968 = 0;
    _DAT_00215980 = 0;
    _DAT_00215978 = 0;
    DAT_00215990 = 0;
    _DAT_00215988 = 0;
    DAT_002159a0 = 0;
    DAT_00215998 = 0;
    DAT_002159b0 = 0;
    DAT_002159a8 = 0;
    DAT_002159c0 = 0;
    DAT_002159b8 = 0;
    DAT_002159d0 = 0;
    DAT_002159c8 = 0;
    uRam00000000002159e0 = 0;
    _DAT_002159d8 = 0;
    _DAT_00215900 = 0;
    _DAT_002158f8 = 0;
  }
  _DAT_001cfe94 = 0;
LAB_00152ac4:
  if (*(long *)(lVar3 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


/* ===== FUN_00155f58 @ 00155f58 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00155f58(undefined8 param_1,long param_2,int *param_3,int *param_4)

{
  long *plVar1;
  long lVar2;
  int iVar3;
  undefined8 uVar4;
  undefined4 uVar5;
  char local_98;
  char local_97;
  undefined8 local_88;
  undefined8 local_80;
  long lStack_78;
  long local_70;
  undefined8 uStack_68;
  ulong local_60;
  ulong local_58;
  undefined8 local_50;
  long local_48;
  
  lVar2 = tpidr_el0;
  local_48 = *(long *)(lVar2 + 0x28);
  param_4[0] = 0;
  param_4[1] = 0;
  param_4[2] = 0;
  if (((((param_3 != (int *)0x0) && (DAT_00214cf8 == 2)) && (DAT_00214c10 != 0)) &&
      ((*(long *)(param_3 + 4) == *(long *)(param_2 + 0x10) &&
       (*(long *)(param_3 + 6) == *(long *)(param_2 + 0x18))))) &&
     ((*param_3 - 1U < 2 && (iVar3 = FUN_00155c04(param_1,param_2), iVar3 != 0)))) {
    iVar3 = *param_3;
    local_50 = 0;
    uStack_68 = 0;
    local_70 = 0;
    local_58 = 0;
    local_60 = 0;
    lStack_78 = 0;
    local_80 = 0;
    if (iVar3 == 2) {
      if (((DAT_00214c14 == 0) || (iVar3 = FUN_0015703c(param_2,&local_80), iVar3 == 0)) ||
         ((*(long *)(param_3 + 8) != lStack_78 ||
          (((*(long *)(param_3 + 10) != local_70 || (local_60 != (uint)param_3[0xe])) ||
           (local_58 != (uint)param_3[0xf])))))) goto LAB_0015620c;
      iVar3 = *param_3;
    }
    if ((iVar3 != 1) || ((param_3[2] != 0 && (iVar3 = FUN_00157358(param_2,param_3), iVar3 != 0))))
    {
      local_88 = 0;
      DAT_00214d88 = 0;
      _DAT_00214d40 = 0;
      _DAT_00214d38 = 0;
      _DAT_00214d50 = 0;
      _DAT_00214d48 = 0;
      _DAT_00214d60 = 0;
      _DAT_00214d58 = 0;
      _DAT_00214d70 = 0;
      _DAT_00214d68 = 0;
      _DAT_00214d80 = 0;
      _DAT_00214d78 = 0;
      DAT_00214d30 = DAT_0010e720;
      iVar3 = FUN_00156308(*param_3,&local_88);
      if (iVar3 != 0) {
        uVar5 = 1;
        if (*param_3 != 1) {
          uVar5 = 2;
        }
        iVar3 = FUN_00156a78(param_2,uVar5,&DAT_00214d30);
        if ((iVar3 != 0) &&
           (uVar4 = FUN_001bc828(*(undefined8 *)(param_2 + 0x10),FUN_001428fc,0,&local_98),
           (int)uVar4 != 0)) {
          if (*param_3 == 1) {
            local_97 = local_98;
          }
          if ((local_97 == '\x01') && (iVar3 = FUN_00155c04(uVar4,param_2), iVar3 != 0)) {
            iVar3 = *param_3;
            if ((iVar3 == 1) && (PTR_FUN_001cb6f8 != (undefined *)0x0)) {
              iVar3 = FUN_0018077c(0);
              if (iVar3 != 1) goto LAB_0015620c;
              iVar3 = *param_3;
            }
            uVar5 = DAT_0020d158;
            DAT_0020d158 = 3;
            if (iVar3 == 1) {
              FUN_0015494c(*(undefined8 *)(param_3 + 4),*(undefined8 *)(param_3 + 6));
            }
            FUN_00157480(local_88,param_3,param_4);
            DAT_0020d158 = uVar5;
            if (*param_4 != 0) {
              DAT_00214cf0 = DAT_0020f650;
              plVar1 = &DAT_00214e28;
              if (*param_3 != 1) {
                plVar1 = &DAT_00214e30;
              }
              *plVar1 = *plVar1 + 1;
              if (param_3[1] == 2) {
                DAT_00214e38 = DAT_00214e38 + 1;
              }
              if (*(int *)(param_2 + 0x30) == 0) {
                DAT_00214e40 = DAT_00214e40 + 1;
              }
            }
          }
        }
      }
    }
  }
LAB_0015620c:
  if (*(long *)(lVar2 + 0x28) != local_48) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}


/* ===== FUN_00156238 @ 00156238 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00156238(void)

{
  long lVar1;
  undefined8 uVar2;
  undefined8 local_50;
  undefined4 local_44;
  undefined8 local_40;
  undefined8 uStack_38;
  undefined *local_30;
  long local_28;
  
  lVar1 = tpidr_el0;
  local_28 = *(long *)(lVar1 + 0x28);
  local_50 = 0;
  if (DAT_00214c20 == '\x01' && DAT_00214c38 != 0) {
    uVar2 = FUN_0018dc40(&DAT_00214c28,DAT_001e0978 + 0xb2e994,FUN_001428fc,0);
    if ((int)uVar2 != 0) {
      local_44 = 0;
      uStack_38 = _UNK_001c3210;
      local_40 = _DAT_001c3208;
      local_30 = PTR_FUN_001c3218;
      uVar2 = FUN_0018e55c(DAT_001e0978,&DAT_00214c40,&local_40,1,&local_50,&local_44);
    }
  }
  else {
    uVar2 = 0;
  }
  if (*(long *)(lVar1 + 0x28) == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar2);
}


/* ===== FUN_00156308 @ 00156308 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00156308(int param_1,undefined8 param_2)

{
  long lVar1;
  long lVar2;
  undefined8 uVar3;
  undefined4 local_54;
  undefined8 local_50;
  undefined8 uStack_48;
  undefined *local_40;
  long local_38;
  
  lVar2 = tpidr_el0;
  local_38 = *(long *)(lVar2 + 0x28);
  if (DAT_00214c20 == '\x01' && DAT_00214c38 != 0) {
    lVar1 = 0xb2e994;
    if (param_1 != 1) {
      lVar1 = 0xb2e780;
    }
    uVar3 = FUN_0018dc40(&DAT_00214c28,DAT_001e0978 + lVar1,FUN_001428fc,0);
    if ((int)uVar3 != 0) {
      local_54 = 0;
      uStack_48 = _UNK_001c3210;
      local_50 = _DAT_001c3208;
      local_40 = PTR_FUN_001c3218;
      uVar3 = FUN_0018e55c(DAT_001e0978,&DAT_00214c40,&local_50,param_1,param_2,&local_54);
    }
  }
  else {
    uVar3 = 0;
  }
  if (*(long *)(lVar2 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar3);
}


/* ===== FUN_00158080 @ 00158080 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00158080(undefined8 param_1)

{
  long lVar1;
  undefined1 auVar2 [16];
  undefined1 auVar3 [16];
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  int iVar8;
  ulong uVar9;
  undefined8 *puVar10;
  ushort uVar11;
  undefined1 auVar12 [16];
  undefined1 auVar13 [16];
  undefined1 auVar14 [16];
  undefined1 auVar15 [16];
  undefined1 auVar16 [16];
  undefined1 auVar17 [16];
  long local_58;
  long local_50;
  long local_48;
  
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  if (DAT_00215a48 == '\x01') {
    uVar9 = FUN_001428fc(param_1,DAT_001e0978 + 0x123d750,&local_58,0x10);
    lVar7 = _UNK_00112988;
    lVar6 = _DAT_00112980;
    lVar5 = _UNK_001128e8;
    lVar4 = _DAT_001128e0;
    if ((int)uVar9 == 0) goto LAB_00158214;
    if (((local_58 == DAT_00215a30 + 0xba7b4) && (local_50 == DAT_00215a30 + 0xba6cc)) &&
       (DAT_00215a48 == '\x01')) {
      auVar15._0_8_ = DAT_001e0978 + _DAT_00112980;
      auVar15._8_8_ = DAT_001e0978 + _UNK_00112988;
      auVar12._0_8_ = DAT_001e0978 + _DAT_001128e0;
      auVar12._8_8_ = DAT_001e0978 + _UNK_001128e8;
      auVar13._8_8_ = local_58;
      auVar13._0_8_ = local_58;
      auVar15 = NEON_cmeq(auVar15,auVar13,8);
      auVar17._8_8_ = local_58;
      auVar17._0_8_ = local_58;
      auVar13 = NEON_cmeq(auVar12,auVar17,8);
      uVar11 = NEON_umaxv(CONCAT26(auVar15._8_2_,
                                   CONCAT24(auVar15._0_2_,CONCAT22(auVar13._8_2_,auVar13._0_2_))),2)
      ;
      puVar10 = (undefined8 *)&DAT_00215a10;
      if ((uVar11 & 1) == 0 && DAT_001e0978 + 0x11a2830 != local_58) {
        puVar10 = &DAT_00215a30;
      }
      uVar9 = FUN_0018dc40(puVar10,local_58,FUN_001428fc,0);
      if ((int)uVar9 == 0) goto LAB_00158214;
      if (DAT_00215a48 == '\x01') {
        puVar10 = (undefined8 *)&DAT_00215a10;
        auVar16._0_8_ = DAT_001e0978 + lVar6;
        auVar16._8_8_ = DAT_001e0978 + lVar7;
        auVar14._0_8_ = DAT_001e0978 + lVar4;
        auVar14._8_8_ = DAT_001e0978 + lVar5;
        auVar2._8_8_ = local_50;
        auVar2._0_8_ = local_50;
        auVar17 = NEON_cmeq(auVar16,auVar2,8);
        auVar3._8_8_ = local_50;
        auVar3._0_8_ = local_50;
        auVar13 = NEON_cmeq(auVar14,auVar3,8);
        uVar11 = NEON_umaxv(CONCAT26(auVar17._8_2_,
                                     CONCAT24(auVar17._0_2_,CONCAT22(auVar13._8_2_,auVar13._0_2_))),
                            2);
        if (((uVar11 & 1) == 0) && (DAT_001e0978 + 0x11a2830 != local_50)) {
          if (DAT_00215a30 + 0xba6cc == local_50) {
            puVar10 = &DAT_00215a30;
          }
          else {
            puVar10 = &DAT_00215a30;
            if (DAT_00215a30 + 0xba7b4 != local_50) goto LAB_00158210;
          }
        }
        iVar8 = FUN_0018dc40(puVar10,local_50,FUN_001428fc,0);
        uVar9 = (ulong)(iVar8 != 0);
        goto LAB_00158214;
      }
    }
  }
LAB_00158210:
  uVar9 = 0;
LAB_00158214:
  if (*(long *)(lVar1 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar9);
}


/* ===== FUN_00158260 @ 00158260 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_00158260(undefined8 param_1,long param_2)

{
  undefined8 *puVar1;
  undefined8 uVar2;
  ushort uVar3;
  undefined1 auVar4 [16];
  undefined1 auVar5 [16];
  undefined1 auVar6 [16];
  undefined1 auVar7 [16];
  
  if (DAT_00215a48 != '\x01') {
    return 0;
  }
  puVar1 = (undefined8 *)&DAT_00215a10;
  auVar4._0_8_ = DAT_001e0978 + _DAT_00112980;
  auVar4._8_8_ = DAT_001e0978 + _UNK_00112988;
  auVar6._0_8_ = DAT_001e0978 + _DAT_001128e0;
  auVar6._8_8_ = DAT_001e0978 + _UNK_001128e8;
  auVar5._8_8_ = param_2;
  auVar5._0_8_ = param_2;
  auVar5 = NEON_cmeq(auVar4,auVar5,8);
  auVar7._8_8_ = param_2;
  auVar7._0_8_ = param_2;
  auVar7 = NEON_cmeq(auVar6,auVar7,8);
  uVar3 = NEON_umaxv(CONCAT26(auVar5._8_2_,
                              CONCAT24(auVar5._0_2_,CONCAT22(auVar7._8_2_,auVar7._0_2_))),2);
  if (((((uVar3 & 1) == 0) && (DAT_001e0978 + 0x11a2830 != param_2)) &&
      (puVar1 = &DAT_00215a30, DAT_00215a30 + 0xba6cc != param_2)) &&
     (DAT_00215a30 + 0xba7b4 != param_2)) {
    return 0;
  }
  uVar2 = FUN_0018dc40(puVar1,param_2,FUN_001428fc,0);
  return uVar2;
}


/* ===== FUN_0016c1e8 @ 0016c1e8 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0016c1e8(void)

{
  long lVar1;
  long lVar2;
  long lVar3;
  int iVar4;
  undefined4 *__addr;
  long lVar5;
  undefined8 uVar6;
  uint uVar7;
  uint uVar8;
  uint local_a4;
  uint local_a0;
  uint local_9c [25];
  long local_38;
  
  lVar2 = tpidr_el0;
  local_38 = *(long *)(lVar2 + 0x28);
  iVar4 = FUN_0014b1d0(0xdf0b28,8,"953c57623b1a6c3113b825a80bfb71ef19f1f810675f8ef6217a3d64c2cd5f1b"
                      );
  if (iVar4 == 0) {
    uVar7 = 0;
    goto LAB_0016c49c;
  }
  iVar4 = FUN_001419f8();
  if ((iVar4 == 0) ||
     (iVar4 = FUN_0014b1d0(0xf5bac4,0xb4,
                           "ddef01ce91b21b0e58719ff1f4364349c5fed89517de91bcdf079ca597ba0412"),
     lVar3 = DAT_001e0978, iVar4 == 0)) {
LAB_0016c414:
    uVar7 = 0;
  }
  else {
    lVar1 = DAT_001e0978 + 0xf5bac4;
    __addr = (undefined4 *)FUN_00152af0(lVar1);
    if (((__addr == (undefined4 *)0x0) ||
        (((iVar4 = FUN_001428fc(__addr,lVar1,local_9c,4), uVar6 = DAT_0010e5d0, iVar4 == 0 ||
          (local_9c[0] != 0xa9bc7bfd)) || ((((uint)__addr | (uint)lVar3) & 3) != 0)))) ||
       (((long)__addr - lVar3) - 0x8f5bab1U < 0xfffffffff0000003)) goto LAB_0016c414;
    iVar4 = (int)((long)__addr - lVar3);
    uVar8 = iVar4 - 0xf5bab4;
    uVar7 = iVar4 - 0xf5bab1;
    if (-1 < (int)uVar8) {
      uVar7 = uVar8;
    }
    local_a0 = uVar7 >> 2 & 0x3ffffff | 0x14000000;
    if ((lVar3 - (long)__addr) - 0x70a4539U < 0xfffffffff0000003) goto LAB_0016c414;
    iVar4 = (int)(lVar3 - (long)__addr);
    uVar7 = iVar4 + 0xf5bac4;
    uVar8 = iVar4 + 0xf5bac7;
    if (-1 < (int)uVar7) {
      uVar8 = uVar7;
    }
    *(undefined8 *)(__addr + 2) = 0;
    *(code **)(__addr + 6) = FUN_001774c8;
    *(undefined8 *)(__addr + 4) = uVar6;
    *__addr = 0xa9bc7bfd;
    __addr[1] = uVar8 >> 2 & 0x3ffffff | 0x14000000;
    FUN_001bdaf0(__addr,__addr + 8);
    iVar4 = mprotect(__addr,DAT_00209d88,5);
    if ((iVar4 != 0) || (iVar4 = FUN_001419f8(), iVar4 == 0)) goto LAB_0016c414;
    unique0x1000029d = __addr;
    lVar5 = FUN_0014bef8(DAT_001cfb4c,&local_a0,4,lVar1);
    if (lVar5 != 4) {
LAB_0016c3c8:
      lVar5 = FUN_0014bef8(DAT_001cfb4c,local_9c,4,lVar1);
      if (lVar5 != 4) {
                    /* WARNING: Subroutine does not return */
        abort();
      }
      uVar6 = FUN_001bdaf0(lVar1,lVar3 + 0xf5bac8);
      iVar4 = FUN_001428fc(uVar6,lVar1,&local_a4,4);
      if ((iVar4 == 0) || (local_a4 != local_9c[0])) {
                    /* WARNING: Subroutine does not return */
        abort();
      }
      goto LAB_0016c414;
    }
    uVar6 = FUN_001bdaf0(lVar1,lVar3 + 0xf5bac8);
    iVar4 = FUN_001428fc(uVar6,lVar1,&local_a4,4);
    if ((iVar4 == 0) || (local_a4 != local_a0)) goto LAB_0016c3c8;
    uVar7 = 1;
  }
  iVar4 = FUN_00173c58(0xb018a0,0x2458,
                       "9897d72af824cfca4ee08360aef2aae245bfa9f20d06a8b58253ec825496c329",0xb018a0,
                       0xd106c3ff,FUN_00176ed4);
  if (iVar4 != 0) {
    uVar7 = uVar7 | 2;
  }
  iVar4 = FUN_00173c58(0x8df640,0xe40,
                       "71cce5de997c192a5f708868b1900747180b197fa763e6731cd97616bfde80e6",0x8df640,
                       0xd10483ff,FUN_00177044);
  if (iVar4 != 0) {
    uVar7 = uVar7 | 4;
  }
LAB_0016c49c:
  iVar4 = FUN_0014b1d0(0xe205d0,0x1c,
                       "92ff11070e1ccf7740074459f0d236b86249cd1cba1dd31ec14f87d62b728c1f");
  if (((iVar4 == 0) ||
      (iVar4 = FUN_0014b1d0(0x66ae58,0x28,
                            "802b13cec7cc1e772239843c553196f0178188ec9e71fa4caf59e3a63dc6dbe0"),
      iVar4 == 0)) ||
     (iVar4 = FUN_0014b1d0(0x66ad48,0x28,
                           "6e12f8c92b811f89d8764aadd2deba2f299200de57188d615dc9281ae33869db"),
     iVar4 == 0)) {
    uVar8 = 0;
  }
  else {
    iVar4 = FUN_00173c58(0xe359c4,0x70,
                         "14f50c19389dbe871c316dc9193b14c631c7e10b862282ca4cc5c5221b5dfce8",0xe35a0c
                         ,0xf9002e60,FUN_00177244);
    uVar8 = (uint)(iVar4 != 0);
  }
  DAT_0022d064._0_4_ = uVar7;
  DAT_0022d064._4_4_ = uVar8;
  iVar4 = FUN_0014b1d0(0x80a358,0x8f8,
                       "b06948096dd05c2890273a517f0d5b595fb321868df66e0a98305eb401bb99ec");
  if (iVar4 == 0) {
    DAT_0022d06c._0_4_ = 0;
  }
  else {
    iVar4 = FUN_00173c58(0x7e82bc,0x14,
                         "a79e409643832a2b780b15757fdc707537089c5d44f8202d72c0f9750b7fca52",0x7e82bc
                         ,0xbd40a801,FUN_00177410);
    DAT_0022d06c._0_4_ = (uint)(iVar4 != 0);
  }
  snprintf((char *)local_9c,100,",\"skins_mask\":%u,\"environment\":%u,\"shake\":%u",(ulong)uVar7,
           (ulong)uVar8);
  FUN_001417c8("script_port_options","v69_skin_and_location_pipeline",local_9c);
  if (*(long *)(lVar2 + 0x28) != local_38) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}


/* ===== FUN_0016ce3c @ 0016ce3c [libNexusEvasionRuntime69252.so] ===== */

void FUN_0016ce3c(void)

{
  long lVar1;
  int iVar2;
  undefined8 uVar3;
  uint uVar4;
  undefined *puVar5;
  long lVar6;
  ulong local_b0;
  char acStack_a8 [80];
  long local_58;
  
  lVar1 = tpidr_el0;
  local_58 = *(long *)(lVar1 + 0x28);
  iVar2 = FUN_001419f8();
  if ((iVar2 != 0) && ((DAT_0022d298 & 1) == 0)) {
    DAT_0022d298 = 1;
    iVar2 = FUN_0014b1d0(0xf3d450,0x10c,
                         "115c49e75df52d04a601f30cf814bc721ec4b60249b1a66296fee3e7860f3a78");
    if ((iVar2 == 0) ||
       (iVar2 = FUN_00179be0(0xf3d2e0,0xb4,
                             "7bd900467c59afc254384a82ec72b9a1d03691f4a94e93ed416c06134d61daa1",
                             FUN_0017a99c,&DAT_0022d2a0,0), iVar2 == 0)) {
      uVar4 = 0;
    }
    else {
      iVar2 = FUN_00173c58(0xf3d450,0x10c,
                           "115c49e75df52d04a601f30cf814bc721ec4b60249b1a66296fee3e7860f3a78",
                           0xf3d54c,0xa9424ff4,FUN_0017ace4);
      uVar4 = 3;
      if (iVar2 == 0) {
        uVar4 = 1;
      }
    }
    local_b0 = 0;
    uVar3 = FUN_0014b1d0(0xf331f8,8,
                         "0df8bce72fee89e1d7434fae8b57ee87b468f9e6db7949b035ac4a0fea715c42");
    if (((((int)uVar3 != 0) &&
         (iVar2 = FUN_001428fc(uVar3,DAT_001e0978 + 0x12257a8,&local_b0,8), iVar2 != 0)) &&
        (0x11fff < local_b0 + 0x2000)) &&
       (((local_b0 & 7) == 0 && (local_b0 == DAT_001e0978 + 0xf331f8U)))) {
      iVar2 = FUN_00173c58(0xf33148,0x44,
                           "8c37c1fc4ca661c5f4698c4d6af93737686a0913474547f4cf21852bc2ccb75f",
                           0xf33148,0xa9bd7bfd,FUN_0017b058);
      if (iVar2 != 0) {
        uVar4 = uVar4 | 4;
      }
    }
    iVar2 = FUN_00173c58(0xb673e0,0x4464,
                         "b1d9f14372fd47ac4caf966590b6721953fa8454f255ed54c7f1107c355f15b7",0xb678a8
                         ,0xb901df48,FUN_0017b11c);
    if (iVar2 != 0) {
      uVar4 = uVar4 | 8;
    }
    iVar2 = FUN_0014b1d0(0x5d8874,0xb0,
                         "77399cc53dfa8af7bca45327002bfaaab847dbaea6a1eeba62dec6aaeecc46a4");
    if (((iVar2 != 0) &&
        (iVar2 = FUN_0014b1d0(0x5d8ad4,0xb0,
                              "c6efba629b3b804cb58f1c683f81864f03d7533bfd84b669edc3ea36e8edaf50"),
        iVar2 != 0)) &&
       (iVar2 = FUN_0014b1d0(0x59567c,0x20,
                             "9e18529b88a6285f2675ad92537b12875a9735c6fc16cbdc947b58b5e235b5f9"),
       iVar2 != 0)) {
      iVar2 = FUN_00173c58(0xa5a3e8,0x164,
                           "6810e00634204636f69a890f22e85c1b906f8c3128e9881960dadbfbd16e587b",
                           0xa5a534,0xa9444ff4,FUN_0017b1ec);
      if (iVar2 != 0) {
        uVar4 = uVar4 | 0x10;
      }
    }
    iVar2 = FUN_0014b1d0(0x66ae58,0x80,
                         "d8e23d16dc9125a73ecbc044b9c4180c7c679f55a8b8252364510f5add8f7f88");
    if (((iVar2 != 0) &&
        (iVar2 = FUN_0014b1d0(0xd173ac,0x40,
                              "db320214a371dfc1490b506f34c873b70f76cf16a7660ebce8f0ef320ecf8c6c"),
        iVar2 != 0)) &&
       (iVar2 = FUN_0014b1d0(0xd170a8,0x120,
                             "0fdc1b331b8e0cb571f0a1a19be2dc6e6894683bdfab71b0bb93c1d44120c016"),
       iVar2 != 0)) {
      lVar6 = 0x10;
      puVar5 = &DAT_0022d2a8;
      do {
        (*(code *)(DAT_001e0978 + 0x66ae58))(puVar5,*(undefined8 *)(&UNK_001ca200 + lVar6));
        (*(code *)(DAT_001e0978 + 0x66ae58))(puVar5 + 0x10,*(undefined8 *)(&UNK_001ca208 + lVar6));
        (*(code *)(DAT_001e0978 + 0x66ae58))
                  (puVar5 + 0x20,*(undefined8 *)((long)&PTR_s_GLOWBERT_001ca210 + lVar6));
        puVar5 = puVar5 + 0x30;
        lVar6 = lVar6 + 0x18;
      } while (lVar6 != 0xd0);
      iVar2 = FUN_00179be0(0xd170a8,0x120,
                           "0fdc1b331b8e0cb571f0a1a19be2dc6e6894683bdfab71b0bb93c1d44120c016",
                           FUN_0017b4c0,&DAT_0022d428,0);
      if (iVar2 != 0) {
        uVar4 = uVar4 | 0x20;
      }
    }
    DAT_0020afa0._0_4_ = uVar4;
    snprintf(acStack_a8,0x50,"installed=%u expected=63",(ulong)uVar4);
    FUN_001417c8("script_port_extended_ready",acStack_a8,0);
  }
  if (*(long *)(lVar1 + 0x28) != local_58) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}


/* ===== FUN_0016d2dc @ 0016d2dc [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0016d2dc(void)

{
  int iVar1;
  
  if (((DAT_002fd520 & 1) == 0) && (iVar1 = FUN_001419f8(), iVar1 != 0)) {
    DAT_002fd520 = 1;
    iVar1 = FUN_0014b1d0(0x89922c,0x24,
                         "a2a821980e59b0e7521ba36058ff7abd97d2e6a83875108fb7c49f4dacb5205f");
    if ((((iVar1 != 0) &&
         ((((iVar1 = FUN_0014b1d0(0x899250,0x78,
                                  "0d8dfdbe3826572d4e774a24f71de62f4bf1811ad0785742cf699bf548cf5630"
                                 ), iVar1 != 0 &&
            (iVar1 = FUN_0014b1d0(0x8f6548,0x334,
                                  "ba05d1914f4ff88b82295dd11f46443cf6919442b987b91aa06fed4cc4f2d0e7"
                                 ), iVar1 != 0)) &&
           (iVar1 = FUN_0014b1d0(0x541848,8,
                                 "73109c1402501dc23c92eec28460f747d6b135f585b5baa3d3ec3efc2b0a3ddc")
           , iVar1 != 0)) &&
          ((iVar1 = FUN_0014b1d0(0xf52f4c,0x9c,
                                 "5e3d3daa0ba27dfc89bc711b3be8dd1a3df06be286e85f051d58c9034cf549be")
           , iVar1 != 0 &&
           (iVar1 = FUN_0014b1d0(0x533ed0,0x10,
                                 "973b0b6c717e63a25e73b844ae6247337486d78d42915daed5a0c963a79549c8")
           , iVar1 != 0)))))) &&
        (iVar1 = FUN_0014b1d0(0x66ae58,0x80,
                              "d8e23d16dc9125a73ecbc044b9c4180c7c679f55a8b8252364510f5add8f7f88"),
        iVar1 != 0)) &&
       ((iVar1 = FUN_0014b1d0(0x66ad48,0x1c,
                              "9edb63b79febb9496a60828270f995d5b618902d80ee1d7caca7a98df8ad3863"),
        iVar1 != 0 &&
        (iVar1 = FUN_0014b1d0(0xa5b198,0x58,
                              "29be0904da5b5baf0101eabc8f9b5f83e72c68891308bd62d09a3244c359e33d"),
        iVar1 != 0)))) {
      FUN_0017bdc8();
      iVar1 = FUN_00179be0(0xf66088,0x2f8,
                           "c38189e9438ecb0eca6d9641f3ddf8848a435f5ca3cb2574766dd547e02d7a5e",
                           FUN_0017bf60,&DAT_002fd528,0);
      if (iVar1 != 0) {
        _DAT_0020af9c = _DAT_0020af9c | 1;
      }
      iVar1 = FUN_00179be0(0x8a3138,0xe6c,
                           "335346ef1b154299af40ad6cbcb25daf5ffc457e0cf185dead0f6cb69ca3bfd3",
                           FUN_0017c5b8,&DAT_002fd530,0);
      if (iVar1 != 0) {
        _DAT_0020af9c = _DAT_0020af9c | 2;
      }
      iVar1 = FUN_00179be0(0x8a4798,0x494,
                           "fa0548207da2b48453e5de1b0a0507185791939c30c2aa46a3b689e3e6d8487b",
                           FUN_0017c720,&DAT_002fd538,0);
      if (iVar1 != 0) {
        _DAT_0020af9c = _DAT_0020af9c | 4;
      }
      iVar1 = FUN_00179be0(0xf178ec,0xa0,
                           "cf70e115dfa15e1dcb02aaaa8274efafc1a3eb7b0b57ed3f5a690efe447f1316",
                           FUN_0017c7ac,&DAT_002fd540,0);
      if (iVar1 != 0) {
        _DAT_0020af9c = _DAT_0020af9c | 8;
      }
    }
  }
  return;
}


/* ===== FUN_00179be0 @ 00179be0 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00179be0(long param_1,undefined4 param_2,undefined8 param_3,undefined8 param_4,
                 undefined8 *param_5,uint param_6)

{
  uint uVar1;
  char cVar2;
  bool bVar3;
  long lVar4;
  undefined8 uVar5;
  int iVar6;
  uint *puVar7;
  uint *__addr;
  long lVar8;
  undefined8 uVar9;
  uint uVar10;
  uint uVar11;
  uint local_64;
  uint local_60;
  uint local_5c;
  long local_58;
  
  lVar4 = tpidr_el0;
  local_58 = *(long *)(lVar4 + 0x28);
  puVar7 = (uint *)FUN_001419f8();
  if (((int)puVar7 != 0) &&
     (puVar7 = (uint *)FUN_0014b1d0(param_1,param_2,param_3), (int)puVar7 != 0)) {
    param_1 = DAT_001e0978 + param_1;
    __addr = (uint *)FUN_00152af0(param_1);
    puVar7 = __addr;
    if ((__addr != (uint *)0x0) &&
       (puVar7 = (uint *)FUN_001428fc(__addr,param_1,&local_5c,4), uVar5 = _UNK_00112b98,
       uVar9 = _DAT_00112b90, (int)puVar7 != 0)) {
      if ((((((uint)param_1 | (uint)__addr) & 3) == 0) &&
          (0xfffffffff0000002 < (param_1 - (long)__addr) - 0x7fffffdU)) &&
         (0xfffffffff0000002 < ((long)__addr - param_1) - 0x7ffffedU)) {
        uVar10 = (uint)(param_1 - (long)__addr);
        iVar6 = (int)((long)__addr - param_1);
        uVar11 = iVar6 + 0x10;
        uVar1 = uVar10 + 3;
        if (-1 < (int)uVar10) {
          uVar1 = uVar10;
        }
        uVar10 = iVar6 + 0x13;
        if (-1 < (int)uVar11) {
          uVar10 = uVar11;
        }
        local_60 = uVar10 >> 2 & 0x3ffffff | 0x14000000;
        *(undefined8 *)(__addr + 6) = param_4;
        *__addr = local_5c;
        __addr[1] = uVar1 >> 2 & 0x3ffffff | 0x14000000;
        *(undefined8 *)(__addr + 4) = uVar5;
        *(undefined8 *)(__addr + 2) = uVar9;
        FUN_001bdaf0(__addr,__addr + 8);
        iVar6 = mprotect(__addr,DAT_00209d88,5);
        if (iVar6 == 0) {
          *param_5 = __addr;
          puVar7 = (uint *)FUN_001419f8();
          if ((int)puVar7 == 0) goto LAB_00179ddc;
          lVar8 = FUN_0014bef8(DAT_001cfb4c,&local_60,4,param_1);
          if (((lVar8 == 4) && (iVar6 = FUN_001428fc(4,param_1,&local_64,4), iVar6 != 0)) &&
             (local_64 == local_60)) {
            FUN_001bdaf0(param_1,param_1 + 4);
            do {
              cVar2 = '\x01';
              bVar3 = (bool)ExclusiveMonitorPass(0x22d14c,0x10);
              if (bVar3) {
                cVar2 = ExclusiveMonitorsStatus();
                DAT_0022d14c._0_4_ = (uint)DAT_0022d14c | param_6;
              }
            } while (cVar2 != '\0');
            puVar7 = (uint *)0x1;
            goto LAB_00179ddc;
          }
          lVar8 = FUN_0014bef8(DAT_001cfb4c,&local_5c,4,param_1);
          if (lVar8 != 4) {
                    /* WARNING: Subroutine does not return */
            abort();
          }
          uVar9 = FUN_001bdaf0(param_1,param_1 + 4);
          iVar6 = FUN_001428fc(uVar9,param_1,&local_64,4);
          if ((iVar6 == 0) || (local_64 != local_5c)) {
                    /* WARNING: Subroutine does not return */
            abort();
          }
        }
      }
      puVar7 = (uint *)0x0;
    }
  }
LAB_00179ddc:
  if (*(long *)(lVar4 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(puVar7);
}


/* ===== FUN_0017e37c @ 0017e37c [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8
FUN_0017e37c(long param_1,undefined8 param_2,undefined8 param_3,undefined4 param_4,
            undefined4 param_5,undefined8 *param_6,ulong *param_7,undefined8 param_8,
            undefined8 param_9)

{
  undefined4 uVar1;
  long lVar2;
  bool bVar3;
  char *pcVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  undefined4 *puVar8;
  undefined8 uVar9;
  size_t sVar10;
  ulong uVar11;
  int local_418;
  ulong local_3f8;
  ulong local_3f0;
  char *local_3e8;
  undefined8 local_3e0;
  char acStack_378 [96];
  char acStack_318 [96];
  char local_2b8 [96];
  char local_258 [96];
  long local_1f8;
  long local_1f0;
  short local_1e8;
  undefined1 uStack_1e6;
  undefined5 uStack_1e5;
  undefined3 uStack_1e0;
  undefined8 local_198;
  char *local_190 [36];
  long local_70;
  
  lVar2 = tpidr_el0;
  local_70 = *(long *)(lVar2 + 0x28);
  puVar8 = (undefined4 *)__errno();
  uVar1 = *puVar8;
  if (((DAT_0020b088 == '\x01') && ((int)DAT_001dfff0 != 0)) && (iVar5 = FUN_0013c82c(), iVar5 != 0)
     ) {
    if ((((int)_DAT_0020b094 == 0) || (iVar5 = FUN_0014a6ac(param_3,&local_1f8,0x60), iVar5 == 0))
       || (iVar5 = FUN_0014a6ac(param_6,local_258,0x60), iVar5 == 0)) {
LAB_0017e798:
      bVar3 = true;
    }
    else {
      iVar5 = FUN_0014a6ac(param_7,local_2b8,0x60);
      bVar3 = true;
      if (((iVar5 != 0) && (local_258[0] == '\0')) &&
         ((local_2b8[0] == '\0' &&
          ((((local_1f8 != 0x6e656e6f73616573 || local_1f0 != 0x7075706f705f64 &&
             ((local_1f8 != 0x6e5f657461657263 || local_1f0 != 0x75706f705f656d61) ||
              local_1e8 != 0x70)) &&
            (local_1f8 != 0x657461675f656761 || local_1f0 != 0x676f6c6169645f)) &&
           (((local_1f8 != 0x657461675f656761 || local_1f0 != 0x5f7265626d756e5f) ||
            CONCAT53(uStack_1e5,CONCAT12(uStack_1e6,local_1e8)) != 0x6c6169645f646170) ||
            CONCAT35(uStack_1e0,uStack_1e5) != 0x676f6c6169645f)))))) {
        local_418 = DAT_001cfb04;
        if (DAT_001cfb04 < 0) {
          local_418 = (int)DAT_001cfb58;
        }
        local_198 = 0;
        local_3e0 = 0;
        local_3f0 = local_3f0 & 0xffffffff00000000;
        iVar5 = FUN_0014fba8(&local_198,&local_3e0,&local_3f0);
        if ((iVar5 != 0) && (iVar5 = (int)local_3f0, 0 < (int)local_3f0)) {
          iVar6 = 0;
          do {
            local_3f0 = 0;
            iVar7 = FUN_0014fccc(iVar6,0xffffffff,&local_198,&local_3f0);
            if (iVar7 < 0) goto LAB_0017e798;
            if ((iVar7 != 0) && ((int)local_198 == local_418)) {
              memcpy(&local_3e0,&local_198,0x128);
              if ((local_3f0 != 0) && ((local_3e0._4_1_ >> 1 & 1) != 0)) {
                sVar10 = strlen(acStack_378);
                iVar5 = (int)sVar10;
                local_190[0] = (char *)0x0;
                local_198 = CONCAT44(iVar5,iVar5);
                pcVar4 = acStack_378;
                if (iVar5 < 8) {
                  memcpy(local_190,acStack_378,(long)iVar5);
                  pcVar4 = local_190[0];
                }
                local_190[0] = pcVar4;
                sVar10 = strlen(acStack_318);
                iVar5 = (int)sVar10;
                local_3e8 = (char *)0x0;
                local_3f0 = CONCAT44(iVar5,iVar5);
                pcVar4 = acStack_318;
                if (iVar5 < 8) {
                  memcpy(&local_3e8,acStack_318,(long)iVar5);
                  pcVar4 = local_3e8;
                }
                local_3e8 = pcVar4;
                (*(code *)(DAT_001e0978 + 0x75da50))(&local_198,0);
                uVar11 = FUN_0014f498(&local_3e0);
                if ((uVar11 & 4) != 0) {
                  param_7 = &local_3f0;
                  param_6 = &local_198;
                }
              }
              goto LAB_0017e798;
            }
            iVar6 = iVar6 + 1;
            bVar3 = true;
          } while (iVar5 != iVar6);
        }
      }
    }
  }
  else {
    bVar3 = false;
  }
  *puVar8 = uVar1;
  uVar9 = (*DAT_002fd5c8)(param_1,param_2,param_3,param_4,param_5,param_6,param_7,param_8,param_9);
  uVar1 = *puVar8;
  local_3f8 = 0;
  if ((((bVar3) && ((int)_DAT_0020b08c != 0)) &&
      (iVar5 = FUN_001428fc(uVar9,param_1 + 0x150,&local_3f8,8), iVar5 != 0)) &&
     ((0x11fff < local_3f8 + 0x2000 && ((local_3f8 & 7) == 0)))) {
    FUN_0014f5d8(local_3f8,1);
  }
  *puVar8 = uVar1;
  if (*(long *)(lVar2 + 0x28) != local_70) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return uVar9;
}


/* ===== FUN_00183164 @ 00183164 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8
FUN_00183164(int *param_1,ulong param_2,ulong param_3,char *param_4,char *param_5,int *param_6,
            undefined8 *param_7)

{
  int iVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  
  if (((((((param_1 == (int *)0x0) || (param_2 < 0x7c8)) || (((ulong)param_1 & 7) != 0)) ||
        ((*param_1 != 0 || (param_4 == (char *)0x0)))) ||
       ((0xfffffffffebeffff < param_3 - 0x10000 ||
        (((param_3 & 7) != 0 ||
         (iVar1 = strcmp(param_4,"a10aeb6b4085fb2a15d969a130cd9608231998b5269a40aed41129d99624ede3")
         , param_5 == (char *)0x0)))))) || (iVar1 != 0)) ||
     (((((iVar1 = strcmp(param_5,"fdf834103d333f9f8a1947b3a405b32da6ebb651"), param_6 == (int *)0x0
         || (iVar1 != 0)) || (*param_6 != 2)) ||
       (((param_6[1] != 0x30 || (*(long *)(param_6 + 4) == 0)) ||
        ((*(long *)(param_6 + 6) == 0 ||
         ((*(long *)(param_6 + 8) == 0 || (*(long *)(param_6 + 10) == 0)))))))) ||
      (iVar1 = FUN_0018330c(param_7), iVar1 == 0)))) {
    FUN_00137ca4("bind_contract",0,0);
  }
  else {
    memset(param_1,0,0x7c8);
    *(ulong *)(param_1 + 2) = param_3;
    uVar2 = *(undefined8 *)param_6;
    uVar4 = *(undefined8 *)(param_6 + 6);
    uVar3 = *(undefined8 *)(param_6 + 4);
    uVar6 = *(undefined8 *)(param_6 + 10);
    uVar5 = *(undefined8 *)(param_6 + 8);
    *(undefined8 *)(param_1 + 6) = *(undefined8 *)(param_6 + 2);
    *(undefined8 *)(param_1 + 4) = uVar2;
    *(undefined8 *)(param_1 + 10) = uVar4;
    *(undefined8 *)(param_1 + 8) = uVar3;
    *(undefined8 *)(param_1 + 0xe) = uVar6;
    *(undefined8 *)(param_1 + 0xc) = uVar5;
    uVar7 = param_7[4];
    uVar4 = param_7[7];
    uVar3 = param_7[6];
    uVar2 = param_7[8];
    uVar6 = param_7[3];
    uVar5 = param_7[2];
    *(undefined8 *)(param_1 + 0x1a) = param_7[5];
    *(undefined8 *)(param_1 + 0x18) = uVar7;
    *(undefined8 *)(param_1 + 0x1e) = uVar4;
    *(undefined8 *)(param_1 + 0x1c) = uVar3;
    *(undefined8 *)(param_1 + 0x20) = uVar2;
    *(undefined8 *)(param_1 + 0x16) = uVar6;
    *(undefined8 *)(param_1 + 0x14) = uVar5;
    uVar2 = *param_7;
    *(undefined8 *)(param_1 + 0x12) = param_7[1];
    *(undefined8 *)(param_1 + 0x10) = uVar2;
    iVar1 = FUN_00183370(param_1,0);
    uVar3 = _UNK_00112808;
    uVar2 = _DAT_00112800;
    if (iVar1 != 0) {
      param_1[0x1d0] = 1;
      param_1[0x1ee] = 0;
      param_1[0x1ef] = 0;
      param_1[0x1ec] = 0;
      param_1[0x1ed] = 0;
      *(undefined8 *)(param_1 + 0x1ce) = uVar3;
      *(undefined8 *)(param_1 + 0x1cc) = uVar2;
      *param_1 = 0x4e534132;
      param_1[0x1d3] = 0;
      param_1[0x1d4] = 0;
      param_1[0x1d1] = 0;
      param_1[0x1d2] = 0;
      param_1[0x1d7] = 0;
      param_1[0x1d8] = 0;
      param_1[0x1d5] = 0;
      param_1[0x1d6] = 0;
      param_1[0x1db] = 0;
      param_1[0x1dc] = 0;
      param_1[0x1d9] = 0;
      param_1[0x1da] = 0;
      param_1[0x1df] = 0;
      param_1[0x1e0] = 0;
      param_1[0x1dd] = 0;
      param_1[0x1de] = 0;
      param_1[0x1e3] = 0;
      param_1[0x1e4] = 0;
      param_1[0x1e1] = 0;
      param_1[0x1e2] = 0;
      param_1[0x1e7] = 0;
      param_1[0x1e8] = 0;
      param_1[0x1e5] = 0;
      param_1[0x1e6] = 0;
      param_1[0x1eb] = 0;
      param_1[0x1ec] = 0;
      param_1[0x1e9] = 0;
      param_1[0x1ea] = 0;
      return 1;
    }
    memset(param_1,0,0x7c8);
  }
  return 0;
}


/* ===== FUN_00183598 @ 00183598 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_00183598(long param_1,uint param_2,ulong param_3,int *param_4)

{
  ulong uVar1;
  int *piVar2;
  int *piVar3;
  ulong uVar4;
  int iVar5;
  char *pcVar6;
  ulong uVar7;
  long lVar8;
  uint uVar9;
  undefined8 uVar10;
  undefined8 uVar11;
  
  if ((((((long)PTR_DAT_001cb708 - (long)PTR_DAT_001cb700 == 0x168 &&
         (long)PTR_DAT_001cb710 - (long)PTR_DAT_001cb700 == 0x160) &&
        (long)PTR_DAT_001cb718 - (long)PTR_DAT_001cb700 == 0x15c) &&
       (long)PTR_DAT_001cb720 - (long)PTR_DAT_001cb700 == 0x14c) &&
      (long)PTR_DAT_001cb728 - (long)PTR_DAT_001cb700 == 0xa4) &&
      (long)PTR_FUN_001cb730 - (long)PTR_DAT_001cb700 == 0x170) {
    iVar5 = memcmp(PTR_DAT_001cb700,&DAT_00121a54,0x170);
    if (iVar5 == 0) {
      uVar7 = *(ulong *)(param_1 + 8);
      if (((0xfffffffffffedfff < param_3 - 0x10000) || ((param_3 & 0xf) != 0)) ||
         ((uVar7 < param_3 + 0x170 && (param_3 < uVar7 + 0x1400000)))) {
        pcVar6 = "island_address";
        uVar7 = *(long *)(&DAT_00121a30 + (ulong)param_2 * 8) + uVar7;
        goto LAB_0018368c;
      }
      memset(param_4,0,0x238);
      uVar11 = _UNK_00112a68;
      uVar10 = _DAT_00112a60;
      iVar5 = *(int *)(&DAT_00121a48 + (ulong)param_2 * 4);
      uVar7 = *(long *)(&DAT_00121a30 + (ulong)param_2 * 8) + *(long *)(param_1 + 8);
      uVar1 = uVar7 + 0x10;
      *param_4 = iVar5;
      *(ulong *)(param_4 + 6) = uVar7;
      *(ulong *)(param_4 + 8) = param_3;
      *(ulong *)(param_4 + 10) = uVar1;
      *(undefined8 *)(param_4 + 3) = uVar11;
      *(undefined8 *)(param_4 + 1) = uVar10;
      param_4[5] = 0x168;
      if ((((uint)uVar7 | (uint)param_3) & 3) == 0) {
        uVar4 = uVar7 - param_3;
        if (uVar7 < param_3 || uVar4 == 0) {
          if (param_3 - uVar7 >> 0x1b == 0) {
            uVar9 = (uint)(param_3 - uVar7 >> 2);
            goto LAB_001837a0;
          }
        }
        else if (uVar4 < 0x8000001) {
          uVar9 = -((uint)uVar4 >> 2) & 0x3ffffff;
LAB_001837a0:
          param_3 = param_3 + 0x15c;
          param_4[0xd] = uVar9 | 0x14000000;
          if (uVar1 < param_3) {
            if (param_3 - uVar1 < 0x8000001) {
              uVar9 = -((uint)(param_3 - uVar1) >> 2) & 0x3ffffff;
LAB_001837ec:
              piVar2 = (int *)&UNK_00121cd0;
              if (param_2 != 1) {
                piVar2 = &DAT_00121e20;
              }
              piVar3 = (int *)&UNK_00121bc4;
              if (param_2 != 0) {
                piVar3 = piVar2;
              }
              param_4[0xc] = *piVar3;
              memcpy(param_4 + 0xe,PTR_DAT_001cb700,0x150);
              uVar11 = *(undefined8 *)(piVar3 + 2);
              uVar10 = *(undefined8 *)piVar3;
              param_4[0x37] = iVar5 << 5 | 0x52800002;
              param_4[0x65] = uVar9 | 0x14000000;
              *(undefined8 *)(param_4 + 99) = uVar11;
              *(undefined8 *)(param_4 + 0x61) = uVar10;
              *(long *)(param_4 + 0x66) = param_1;
              *(code **)(param_4 + 0x68) = FUN_00184450;
              return 1;
            }
          }
          else if (uVar1 - param_3 >> 0x1b == 0) {
            uVar9 = (uint)(uVar1 - param_3 >> 2);
            goto LAB_001837ec;
          }
          pcVar6 = "branch_resume";
          goto LAB_0018368c;
        }
      }
      pcVar6 = "branch_entry";
      goto LAB_0018368c;
    }
    lVar8 = *(long *)(param_1 + 8);
    pcVar6 = "template_bytes";
  }
  else {
    lVar8 = *(long *)(param_1 + 8);
    pcVar6 = "template_layout";
  }
  uVar7 = *(long *)(&DAT_00121a30 + (ulong)param_2 * 8) + lVar8;
LAB_0018368c:
  FUN_00137ca4(pcVar6,uVar7,0);
  return 0;
}


/* ===== FUN_001a8430 @ 001a8430 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_001a8430(long param_1,long param_2,long *param_3)

{
  undefined1 auVar1 [16];
  long lVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  int iVar6;
  long lVar7;
  ulong uVar8;
  ushort uVar9;
  float fVar10;
  undefined1 auVar11 [16];
  undefined1 auVar12 [16];
  float fVar13;
  undefined1 auVar14 [16];
  long local_b8;
  uint local_b0;
  ulong local_a8;
  long local_88;
  long local_7c;
  
  lVar5 = _UNK_001129f8;
  lVar4 = _DAT_001129f0;
  lVar3 = _UNK_001128d8;
  lVar2 = _DAT_001128d0;
  if (*(int *)(param_2 + 0x5c) != 0) {
    uVar8 = 0;
    do {
      param_3[3] = 0;
      param_3[2] = 0;
      param_3[5] = 0;
      param_3[4] = 0;
      param_3[1] = 0;
      *param_3 = 0;
      lVar7 = *(long *)(*(long *)(param_2 + 0x68) + uVar8 * 8);
      *param_3 = lVar7;
      if ((lVar7 + 0x48U < 0x10048) ||
         (iVar6 = (**(code **)(param_1 + 0x28))
                            (*(undefined8 *)(param_1 + 0x20),lVar7,&local_b8,0x48), iVar6 != 1)) {
        return 3;
      }
      param_3[2] = local_b8;
      lVar7 = *(long *)(param_1 + 8);
      auVar14._0_8_ = lVar7 + lVar2;
      auVar14._8_8_ = lVar7 + lVar3;
      auVar11._0_8_ = lVar7 + lVar4;
      auVar11._8_8_ = lVar7 + lVar5;
      auVar12._8_8_ = local_b8;
      auVar12._0_8_ = local_b8;
      auVar14 = NEON_cmeq(auVar14,auVar12,8);
      auVar1._8_8_ = local_b8;
      auVar1._0_8_ = local_b8;
      auVar12 = NEON_cmeq(auVar11,auVar1,8);
      uVar9 = NEON_umaxv(CONCAT26(auVar14._8_2_,
                                  CONCAT24(auVar14._0_2_,CONCAT22(auVar12._8_2_,auVar12._0_2_))),2);
      if (((uVar9 & 1) == 0) && (lVar7 + 0x12205d0 != local_b8)) {
        return 5;
      }
      *(uint *)(param_3 + 3) = local_b0;
      param_3[1] = local_a8;
      param_3[5] = local_88;
      param_3[4] = local_7c;
      if (((*param_3 != *(long *)(param_2 + 0x38)) &&
          ((((fVar10 = (float)(int)local_88 / 300.0, local_b0 < 1000000 || (1999999 < local_b0)) &&
            (fVar10 != -200.0 && fVar10 < -200.0 == NAN(fVar10))) &&
           ((fVar10 < 200.0 &&
            (fVar13 = (float)(int)((ulong)local_88 >> 0x20) / 300.0,
            fVar13 != -200.0 && fVar13 < -200.0 == NAN(fVar13))))))) &&
         ((fVar13 < 200.0 &&
          (((fVar10 == -0.5 || fVar10 < -0.5 != NAN(fVar10) || (0.5 <= fVar10)) ||
           ((fVar13 == -0.5 || fVar13 < -0.5 != NAN(fVar13) || (0.5 <= fVar13)))))))) {
        if (local_a8 + 0x2000 < 0x12000) {
          return 3;
        }
        if ((local_a8 & 7) != 0) {
          return 3;
        }
        if (local_a8 + 0x24 < 0x10004) {
          return 3;
        }
        iVar6 = (**(code **)(param_1 + 0x28))
                          (*(undefined8 *)(param_1 + 0x20),local_a8 + 0x20,(long)param_3 + 0x1c,4);
        if (iVar6 != 1) {
          return 3;
        }
      }
      uVar8 = uVar8 + 1;
      param_3 = param_3 + 6;
    } while (uVar8 < *(uint *)(param_2 + 0x5c));
  }
  return 0;
}


/* ===== FUN_001ba20c @ 001ba20c [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_001ba20c(undefined8 *param_1)

{
  undefined8 uVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  
  uVar4 = _UNK_00121400;
  uVar3 = _DAT_001213f8;
  uVar2 = _UNK_001213f0;
  uVar1 = _DAT_001213e8;
  param_1[5] = 0;
  param_1[4] = 0;
  param_1[7] = 0;
  param_1[6] = 0;
  param_1[9] = 0;
  param_1[8] = 0;
  param_1[0xb] = 0;
  param_1[10] = 0;
  param_1[0xd] = 0;
  param_1[0xc] = 0;
  param_1[1] = uVar2;
  *param_1 = uVar1;
  param_1[3] = uVar4;
  param_1[2] = uVar3;
  return;
}


/* ===== strtoull @ 0031c120 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

ulonglong strtoull(char *__nptr,char **__endptr,int __base)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

