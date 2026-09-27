/*
 * gl_hooks — Core plumbing (mixed: reconstructed GL core + raw remainder)
 * Decompiled with Ghidra 11.3.2 (arm64 pseudocode) from: libNexusEvasionRuntime69252.so
 * Related menu entries (from embedded nexus-overlay-wire/v1):
 *   - menu.xray "X-Ray" [Nexus+ PAID]
 *   - isXrayEnabled "X-Ray" [Nexus+ PAID]
 *   - espEnabled "ESP" [Nexus+ PAID]
 *   - characterOutlineEnabled "Character outline" [free]
 *   - attackRangeIndicator "Attack range" [Nexus+ PAID]
 *   - hitboxRenderer "Hitboxes" [Nexus+ PAID]
 *   - enemyTracer "Enemy tracer" [Nexus+ PAID]
 *   - trophiesAboveHead "Trophies" [Nexus+ PAID]
 *   - ... +225 more (see docs/feature_list.json)
 * Layout note: the Evasion published API (nexus_evasion_*) moved to core/evasion_api.c,
 * script ports + runtime registrars moved to core/script_ports.c (both reconstructed).
 * This file keeps the EvasionRuntime visual pipeline: ng_* observers and the GL hook
 * installer are reconstructed below; the overlay renderer, HUD builders and runtime
 * internals (FUN_00143180..FUN_001b65d8) are still raw Ghidra pseudocode.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <dlfcn.h>

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

/* ===== FUN_00143180 @ 00143180 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_00143180(void)

{
  uint uVar1;
  uint uVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  int iVar8;
  int iVar9;
  undefined4 *puVar10;
  undefined1 *puVar11;
  undefined8 uVar12;
  ulong uVar13;
  char *pcVar14;
  undefined4 uVar15;
  long lVar16;
  ulong uVar17;
  uint uVar18;
  long local_208;
  undefined8 local_200;
  undefined8 uStack_1f8;
  undefined *puStack_1f0;
  undefined *puStack_1e8;
  undefined *local_1e0;
  undefined *puStack_1d8;
  long local_190;
  long lStack_188;
  long local_180;
  long lStack_178;
  char *local_170;
  long local_168;
  undefined8 uStack_160;
  code *local_158;
  code *pcStack_150;
  code *local_148;
  code *pcStack_140;
  code *local_138;
  code *pcStack_130;
  long local_70;
  
  lVar3 = tpidr_el0;
  local_70 = *(long *)(lVar3 + 0x28);
  puVar10 = (undefined4 *)FUN_001bd828(&DAT_001cfbf8);
  *puVar10 = 1;
  puVar10 = (undefined4 *)FUN_001bd828(&DAT_001cfc18);
  *puVar10 = 0;
  puVar11 = (undefined1 *)FUN_001bd828(&DAT_001cfc38);
  *puVar11 = 0;
  DAT_00213038 = dlsym(DAT_0020d0f0,"nexus_evasion_functions_register_v1");
  DAT_0020d160 = dlsym(DAT_0020d0f0,"nexus_evasion_functions_snapshot_v1");
  DAT_00213040 = dlsym(DAT_0020d0f0,"nexus_evasion_functions_lease_v1");
  DAT_00213048 = dlsym(DAT_0020d0f0,"nexus_evasion_functions_recheck_v1");
  DAT_00213050 = dlsym(DAT_0020d0f0,"nexus_evasion_publish_event_routes_v2");
  if (((((((DAT_00213038 == 0) || (iVar8 = dladdr(DAT_00213038,&local_170), iVar8 == 0)) ||
         (local_168 != DAT_0020c0e8)) ||
        (((local_170 == (char *)0x0 || (iVar8 = strcmp(local_170,&DAT_0020c0f0), iVar8 != 0)) ||
         ((DAT_0020d160 == 0 ||
          ((iVar8 = dladdr(DAT_0020d160,&local_170), iVar8 == 0 || (local_168 != DAT_0020c0e8)))))))
        ) || (local_170 == (char *)0x0)) ||
      ((((iVar8 = strcmp(local_170,&DAT_0020c0f0), iVar8 != 0 || (DAT_00213040 == 0)) ||
        (iVar8 = dladdr(DAT_00213040,&local_170), iVar8 == 0)) ||
       (((local_168 != DAT_0020c0e8 || (local_170 == (char *)0x0)) ||
        ((iVar8 = strcmp(local_170,&DAT_0020c0f0), iVar8 != 0 ||
         ((DAT_00213048 == 0 || (iVar8 = dladdr(DAT_00213048,&local_170), iVar8 == 0)))))))))) ||
     ((local_168 != DAT_0020c0e8 ||
      (((((local_170 == (char *)0x0 || (iVar8 = strcmp(local_170,&DAT_0020c0f0), iVar8 != 0)) ||
         (DAT_00213050 == 0)) ||
        ((iVar8 = dladdr(DAT_00213050,&local_170), iVar8 == 0 || (local_168 != DAT_0020c0e8)))) ||
       ((local_170 == (char *)0x0 ||
        ((iVar8 = strcmp(local_170,&DAT_0020c0f0), iVar8 != 0 ||
         (uVar13 = FUN_0018315c(), 0x1000 < uVar13)))))))))) {
    FUN_00137ca4("exports",0,0);
    lVar7 = DAT_00213050;
    lVar6 = DAT_00213048;
    lVar5 = DAT_00213040;
    lVar4 = DAT_00213038;
    lVar16 = DAT_0020d160;
    uVar12 = FUN_0018315c();
    __android_log_print(6,"NexusMem",
                        "functions prepare exports failed register=%p snapshot=%p lease=%p recheck=%p publish=%p route_bytes=%zu"
                        ,lVar4,lVar16,lVar5,lVar6,lVar7,uVar12);
  }
  else {
    local_208 = 0;
    DAT_00213058 = 1;
    do {
      uVar12 = FUN_0018d348(&local_200);
      uVar18 = 0;
      uVar1 = *(uint *)(&DAT_0010f2f0 + local_208 * 0x30);
      lVar16 = *(long *)(&DAT_0010f2e8 + local_208 * 0x30);
      do {
        iVar8 = DAT_001cfb4c;
        uVar2 = uVar1 - uVar18;
        if (0xff < uVar2) {
          uVar2 = 0x100;
        }
        uVar17 = (ulong)uVar2;
        uVar13 = DAT_001e0978 + (ulong)uVar18 + lVar16;
        iVar9 = FUN_001428fc(uVar12,uVar13,&local_170,uVar17);
        if ((iVar8 < 0) || (uVar13 < 0x10000)) {
          if (iVar9 == 0) {
LAB_001436f0:
            FUN_00137ca4("read_argument",uVar13,0);
            goto LAB_00143710;
          }
        }
        else if (iVar9 == 0) {
          if (CARRY8(uVar17,uVar13)) goto LAB_001436f0;
          FUN_00153ca0(uVar13);
LAB_00143710:
          pcVar14 = "functions guard read failed index=%u rva=0x%lx";
          goto LAB_00143720;
        }
        uVar12 = FUN_0018d36c(&local_200,&local_170,uVar17);
        uVar18 = uVar2 + uVar18;
      } while (uVar18 < uVar1);
      FUN_0018d6ac(&local_200,&local_190);
      lVar16 = local_208 * 0x30;
      if (((local_190 != *(long *)(&DAT_0010f2f4 + lVar16) ||
           lStack_188 != *(long *)(&DAT_0010f2fc + lVar16)) ||
          local_180 != *(long *)(&DAT_0010f304 + lVar16)) ||
          lStack_178 != *(long *)(&DAT_0010f30c + lVar16)) {
        lVar16 = *(long *)(&DAT_0010f2e8 + local_208 * 0x30);
        FUN_00137ca4("guard_hash",lVar16 + DAT_001e0978,0);
        pcVar14 = "functions guard hash failed index=%u rva=0x%lx";
LAB_00143720:
        __android_log_print(6,"NexusMem",pcVar14,local_208,lVar16);
        goto LAB_00143310;
      }
      local_208 = local_208 + 1;
    } while (local_208 != 0xf);
    FUN_00189b48(&DAT_00213060);
    FUN_001857cc(&DAT_0020d168);
    FUN_00184cc0(&DAT_00213098);
    uStack_1f8 = _UNK_001c31e0;
    local_200 = _DAT_001c31d8;
    puStack_1e8 = PTR_FUN_001c31f0;
    puStack_1f0 = PTR_FUN_001c31e8;
    puStack_1d8 = PTR_FUN_001c3200;
    local_1e0 = PTR_FUN_001c31f8;
    local_170 = DAT_0010e788;
    local_158 = FUN_0013a250;
    pcStack_150 = FUN_001512f8;
    local_148 = FUN_00151bb4;
    pcStack_140 = FUN_00151ec4;
    local_168 = DAT_00209d00;
    uStack_160 = 0;
    local_138 = FUN_00152598;
    pcStack_130 = FUN_00152844;
    iVar8 = FUN_00183164(&DAT_00213130,0x1000,DAT_001e0978,
                         "a10aeb6b4085fb2a15d969a130cd9608231998b5269a40aed41129d99624ede3",
                         "fdf834103d333f9f8a1947b3a405b32da6ebb651",&local_200,&local_170);
    if (iVar8 == 0) {
      FUN_00137ca4(&DAT_0011f44f,0,0);
      pcVar14 = "functions route bind failed";
LAB_001437f0:
      __android_log_print(6,"NexusMem",pcVar14);
    }
    else {
      DAT_0020af28 = FUN_00152af0(DAT_001e0978 + 0xb2e674);
      DAT_0020af30 = FUN_00152af0(DAT_001e0978 + 0xb2e994);
      DAT_0020af38 = FUN_00152af0(DAT_001e0978 + 0xb2e780);
      if (((DAT_0020af28 == 0) || (DAT_0020af30 == 0)) || (DAT_0020af38 == 0)) {
        FUN_00137ca4("allocate",0,0);
        __android_log_print(6,"NexusMem","functions island allocation failed a=%p b=%p c=%p",
                            DAT_0020af28,DAT_0020af30,DAT_0020af38);
      }
      else {
        iVar8 = FUN_001833cc(&DAT_00213130,DAT_0020af28,DAT_0020af30,DAT_0020af38,&DAT_00214130);
        if (iVar8 == 0) {
          FUN_00137ca4("prepare",0,0);
          pcVar14 = "functions route prepare failed";
          goto LAB_001437f0;
        }
        iVar8 = FUN_0018386c(&DAT_00213130);
        if (iVar8 == 1) {
          DAT_002147d8 = 1;
          FUN_00153980();
          uVar15 = 1;
          goto LAB_00143314;
        }
        if (iVar8 == -1) {
          FUN_001417c8("fatal","event_route_rollback_unverified",0);
                    /* WARNING: Subroutine does not return */
          abort();
        }
        FUN_00137ca4("publish",0,0);
        __android_log_print(6,"NexusMem","functions route publish failed result=%d",iVar8);
      }
    }
  }
LAB_00143310:
  uVar15 = 0;
LAB_00143314:
  if (*(long *)(lVar3 + 0x28) == local_70) {
    return uVar15;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00144010 @ 00144010 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00144010(undefined8 param_1)

{
  long lVar1;
  long lVar2;
  int iVar3;
  char *pcVar4;
  long lVar5;
  char *pcVar6;
  undefined8 *puVar7;
  undefined4 local_9c;
  undefined8 local_98;
  undefined8 local_90;
  undefined3 uStack_88;
  char acStack_85 [5];
  char acStack_80 [3];
  char acStack_7d [85];
  long local_28;
  
  lVar2 = tpidr_el0;
  local_28 = *(long *)(lVar2 + 0x28);
  local_98 = 0;
  lVar1 = DAT_001e0978 + 0x1308444;
  iVar3 = FUN_001428fc(param_1,lVar1,(long)&local_98 + 4,4);
  if (iVar3 == 0 || 5 < local_98._4_4_) {
    pcVar6 = "unexpected_current_value";
LAB_00144134:
    pcVar4 = "env_override_rejected";
    puVar7 = (undefined8 *)0x0;
  }
  else {
    if (local_98._4_4_ == 3) {
      pcVar4 = "env_override_ready";
      pcVar6 = "already_production";
      uStack_88 = _UNK_00115f39;
      local_90 = _DAT_00115f31;
      acStack_7d[0] = s_308444___old__3_00115f3c[8];
      acStack_7d[1] = s_308444___old__3_00115f3c[9];
      acStack_7d[2] = s_308444___old__3_00115f3c[10];
      acStack_7d[3] = s_308444___old__3_00115f3c[0xb];
      acStack_7d[4] = s_308444___old__3_00115f3c[0xc];
      acStack_7d[5] = s_308444___old__3_00115f3c[0xd];
      acStack_7d[6] = s_308444___old__3_00115f3c[0xe];
      acStack_7d[7] = s_308444___old__3_00115f3c[0xf];
      acStack_85[0] = s_308444___old__3_00115f3c[0];
      acStack_85[1] = s_308444___old__3_00115f3c[1];
      acStack_85[2] = s_308444___old__3_00115f3c[2];
      acStack_85[3] = s_308444___old__3_00115f3c[3];
      acStack_85[4] = s_308444___old__3_00115f3c[4];
      acStack_80[0] = s_308444___old__3_00115f3c[5];
      acStack_80[1] = s_308444___old__3_00115f3c[6];
      acStack_80[2] = s_308444___old__3_00115f3c[7];
    }
    else {
      local_9c = 3;
      lVar5 = FUN_0014bef8(DAT_001cfb4c,&local_9c,4,lVar1);
      if (((lVar5 != 4) || (iVar3 = FUN_001428fc(4,lVar1,&local_98,4), iVar3 == 0)) ||
         ((int)local_98 != 3)) {
        pcVar6 = "write_or_readback_failed";
        goto LAB_00144134;
      }
      snprintf((char *)&local_90,0x60,",\"rva\":\"0x1308444\",\"old\":%u,\"new\":3",local_98 >> 0x20
              );
      pcVar4 = "env_override_applied";
      pcVar6 = "v69_252_environment_enum";
    }
    puVar7 = &local_90;
  }
  FUN_001417c8(pcVar4,pcVar6,puVar7);
  if (*(long *)(lVar2 + 0x28) == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00147cec @ 00147cec [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00147cec(int param_1)

{
  long lVar1;
  int iVar2;
  uint uVar3;
  char *pcVar4;
  char *pcVar5;
  undefined8 local_408;
  char *local_400;
  char *pcStack_3f8;
  undefined8 local_3f0;
  undefined8 local_3e8;
  long lStack_3e0;
  code *local_3d8;
  code *pcStack_3d0;
  code *local_3c8;
  code *pcStack_3c0;
  code *local_3b8;
  undefined8 local_3b0;
  undefined8 uStack_3a8;
  undefined *puStack_3a0;
  undefined *puStack_398;
  undefined *local_390;
  stat local_380;
  code *pcStack_2f0;
  code *local_2e8;
  code *pcStack_2e0;
  undefined8 local_170;
  ulong uStack_168;
  ulong local_160;
  undefined8 uStack_158;
  undefined8 local_150;
  undefined8 uStack_148;
  undefined8 local_140;
  undefined8 uStack_138;
  undefined8 uStack_130;
  undefined8 uStack_128;
  undefined8 local_120;
  undefined8 uStack_118;
  undefined8 uStack_110;
  undefined8 uStack_108;
  undefined8 local_100;
  undefined8 uStack_f8;
  undefined8 uStack_f0;
  undefined8 uStack_e8;
  undefined8 local_e0;
  undefined8 uStack_d8;
  undefined8 uStack_d0;
  undefined8 uStack_c8;
  undefined8 local_c0;
  undefined8 uStack_b8;
  undefined8 uStack_b0;
  undefined8 uStack_a8;
  undefined8 local_a0;
  undefined8 uStack_98;
  undefined8 uStack_90;
  undefined8 uStack_88;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined8 uStack_70;
  undefined8 uStack_68;
  long local_58;
  
  lVar1 = tpidr_el0;
  local_58 = *(long *)(lVar1 + 0x28);
  if ((param_1 == 0) || (iVar2 = FUN_001419f8(), iVar2 == 0)) goto LAB_00147da0;
  DAT_00228e78 = (code *)dlsym(DAT_0020d0f0,"nexus_evasion_register_visual_v1");
  DAT_00220918 = dlsym(DAT_0020d0f0,"nexus_evasion_snapshot_keys_v1");
  if ((DAT_00228e78 != (code *)0x0) &&
     (((iVar2 = dladdr(DAT_00228e78,local_380.__unused + 1), iVar2 != 0 &&
       (local_380.__unused[2] == DAT_0020c0e8)) && ((char *)local_380.__unused[1] != (char *)0x0))))
  {
    iVar2 = strcmp((char *)local_380.__unused[1],&DAT_0020c0f0);
    if ((((iVar2 == 0) && (DAT_00220918 != 0)) &&
        (iVar2 = dladdr(DAT_00220918,local_380.__unused + 1), iVar2 != 0)) &&
       ((local_380.__unused[2] == DAT_0020c0e8 && ((char *)local_380.__unused[1] != (char *)0x0))))
    {
      iVar2 = strcmp((char *)local_380.__unused[1],&DAT_0020c0f0);
      if (iVar2 == 0) {
        iVar2 = stat(&DAT_00209d98,&local_380);
        if (((iVar2 != 0) || (((uint)local_380.st_nlink & 0xf000) != 0x8000)) ||
           (local_380.st_size != 0x1340c90)) goto LAB_00147da0;
        uStack_3a8 = _UNK_001c58a0;
        local_3b0 = _DAT_001c5898;
        puStack_398 = PTR_FUN_001c58b0;
        puStack_3a0 = PTR_FUN_001c58a8;
        local_390 = PTR_FUN_001c58b8;
        iVar2 = FUN_001989d4(&DAT_00228e80,&local_3b0,DAT_001e0978 + 0x123e0c8,DAT_00209d88,
                             0x123a0c8,local_380.st_ino,
                             (uint)(local_380.st_dev >> 0x20) & 0xfffff000 |
                             (uint)local_380.st_dev >> 8 & 0xfff,
                             (uint)(local_380.st_dev >> 0xc) & 0xffffff00 |
                             (uint)local_380.st_dev & 0xff);
        if (iVar2 == 0) goto LAB_00147da0;
        local_408 = DAT_0010e700;
        local_3f0 = DAT_0010e7a8;
        local_400 = "a10aeb6b4085fb2a15d969a130cd9608231998b5269a40aed41129d99624ede3";
        pcStack_3f8 = "fdf834103d333f9f8a1947b3a405b32da6ebb651";
        local_3e8 = 0;
        lStack_3e0 = DAT_001e0978;
        local_3d8 = FUN_0016af58;
        pcStack_3d0 = FUN_0016b3f8;
        local_3c8 = gettid;
        pcStack_3c0 = FUN_0016b478;
        local_3b8 = FUN_0016b76c;
        iVar2 = nexus_visual_gl_install_v1(&local_408);
        if ((iVar2 == 1) && (DAT_00228ee0 == 0)) {
          uVar3 = 1;
          DAT_00220784 = 1;
          local_380.__unused[2] = DAT_00209d00;
          pcStack_2f0 = FUN_001455a4;
          local_2e8 = FUN_0015d5d4;
          pcStack_2e0 = FUN_00166084;
          local_380.__unused[1] = (long)DAT_0010e668;
          iVar2 = (*DAT_00228e78)(local_380.__unused + 1);
          if (iVar2 != 1) goto LAB_00147f9c;
          pcVar4 = "visual_ready";
          pcVar5 = "ESP_handler_and_guarded_renderer_bound";
        }
        else {
LAB_00147f9c:
          DAT_002208f8 = 0;
          DAT_00220784 = 0;
          nexus_visual_gl_disable_v1();
          if (DAT_00228e78 != (code *)0x0) {
            (*DAT_00228e78)(0);
          }
          uVar3 = 0;
          pcVar4 = "visual_refused";
          pcVar5 = "optional_ESP_adapter_refused";
        }
        uStack_158 = 0;
        local_160 = 0;
        uStack_148 = 0;
        local_150 = 0;
        uStack_138 = 0;
        local_140 = 0;
        uStack_128 = 0;
        uStack_130 = 0;
        uStack_118 = 0;
        local_120 = 0;
        uStack_108 = 0;
        uStack_110 = 0;
        uStack_f8 = 0;
        local_100 = 0;
        uStack_e8 = 0;
        uStack_f0 = 0;
        uStack_d8 = 0;
        local_e0 = 0;
        uStack_c8 = 0;
        uStack_d0 = 0;
        uStack_b8 = 0;
        local_c0 = 0;
        uStack_a8 = 0;
        uStack_b0 = 0;
        uStack_98 = 0;
        local_a0 = 0;
        uStack_88 = 0;
        uStack_90 = 0;
        uStack_78 = 0;
        local_80 = 0;
        uStack_68 = 0;
        uStack_70 = 0;
        uStack_168 = 0;
        local_170 = 0;
        nexus_visual_gl_android_status_v1(&local_170);
        snprintf((char *)(local_380.__unused + 1),400,
                 ",\"ready\":%d,\"binding_reason\":%u,\"hook_status\":%u,\"symbols\":%u,\"slot\":\"0x%llx\",\"resolved_swap\":\"0x%llx\",\"permission_uncertain\":%u"
                 ,(ulong)uVar3,uStack_168 & 0xffffffff,uStack_168 >> 0x20,local_160 & 0xffffffff,
                 local_150,uStack_158,DAT_00228ee0);
        FUN_001417c8(pcVar4,pcVar5,local_380.__unused + 1);
        goto LAB_00147da0;
      }
    }
  }
  DAT_00228e78 = (code *)0x0;
LAB_00147da0:
  if (*(long *)(lVar1 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
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

/* ===== FUN_0014a1c8 @ 0014a1c8 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0014a1c8(long param_1,undefined8 param_2)

{
  char cVar1;
  bool bVar2;
  uint uVar3;
  
  do {
    uVar3 = _DAT_001dff88;
    cVar1 = '\x01';
    bVar2 = (bool)ExclusiveMonitorPass(0x1dff88,0x10);
    if (bVar2) {
      _DAT_001dff88 = CONCAT31(DAT_001dff88_1,1);
      cVar1 = ExclusiveMonitorsStatus();
    }
  } while (cVar1 != '\0');
  if ((uVar3 & 1) == 0) {
    FUN_00138d54();
    if (DAT_001dff90 != param_1) {
      DAT_001dffa0 = 0;
      DAT_001cfad8 = DAT_001cfad8 + 1;
      FUN_001393f4();
    }
    DAT_001dff98 = 0;
    if (param_1 != 0) {
      DAT_001dff98 = param_2;
    }
    _DAT_001dff88 = 0;
    DAT_001dff90 = param_1;
  }
  return;
}

/* ===== FUN_0014a34c @ 0014a34c [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0014a34c(int *param_1)

{
  undefined8 uVar1;
  undefined8 uVar2;
  int *piVar3;
  
  if ((((((param_1 == (int *)0x0) || (*param_1 != 1)) || (param_1[1] != 0x30)) ||
       ((param_1[9] == 0 || (*(long *)(param_1 + 2) == 0)))) ||
      ((piVar3 = *(int **)(param_1 + 4), piVar3 == (int *)0x0 ||
       ((*piVar3 != 1 || (piVar3[1] != 0xd0)))))) ||
     ((piVar3[0x1a] == 0 ||
      ((*(long *)(piVar3 + 8) == 0 ||
       (*(long *)(*(long *)(param_1 + 2) + 8) != *(long *)(piVar3 + 6))))))) {
    uVar1 = 0;
    uVar2 = 0;
    DAT_001cfbf0 = 0xffffffff;
    DAT_001e0028 = 0;
    _DAT_001e0020 = 0;
    DAT_001e0038 = 0;
    DAT_001e0030 = 0;
    DAT_001e0048 = 0;
    DAT_001e0040 = 0;
    uRam00000000001e0058 = 0;
    DAT_001e0050 = 0;
    uRam00000000001e0068 = 0;
    DAT_001e0060 = 0;
    uRam00000000001e0078 = 0;
    _DAT_001e0070 = 0;
    _DAT_001e0088 = 0;
    _DAT_001e0080 = 0;
    uRam00000000001e0098 = 0;
    _DAT_001e0090 = 0;
    uRam00000000001e00a8 = 0;
    _DAT_001e00a0 = 0;
    uRam00000000001e00b8 = 0;
    _DAT_001e00b0 = 0;
    uRam00000000001e00c8 = 0;
    _DAT_001e00c0 = 0;
    uRam00000000001e00d8 = 0;
    _DAT_001e00d0 = 0;
    uRam00000000001e0018 = 0;
    _DAT_001e0010 = 0;
    DAT_0020af48 = 0;
  }
  else {
    DAT_001e0048 = *(undefined8 *)(piVar3 + 0xe);
    DAT_001e0040 = *(undefined8 *)(piVar3 + 0xc);
    uRam00000000001e0058 = *(undefined8 *)(piVar3 + 0x12);
    DAT_001e0050 = *(undefined8 *)(piVar3 + 0x10);
    DAT_001e0028 = *(undefined8 *)(piVar3 + 6);
    _DAT_001e0020 = *(undefined8 *)(piVar3 + 4);
    DAT_001e0038 = *(undefined8 *)(piVar3 + 10);
    DAT_001e0030 = *(undefined8 *)(piVar3 + 8);
    _DAT_001e0088 = *(undefined8 *)(piVar3 + 0x1e);
    _DAT_001e0080 = *(undefined8 *)(piVar3 + 0x1c);
    uRam00000000001e0098 = *(undefined8 *)(piVar3 + 0x22);
    _DAT_001e0090 = *(undefined8 *)(piVar3 + 0x20);
    uRam00000000001e0068 = *(undefined8 *)(piVar3 + 0x16);
    DAT_001e0060 = *(undefined8 *)(piVar3 + 0x14);
    uRam00000000001e0078 = *(undefined8 *)(piVar3 + 0x1a);
    _DAT_001e0070 = *(undefined8 *)(piVar3 + 0x18);
    uRam00000000001e00c8 = *(undefined8 *)(piVar3 + 0x2e);
    _DAT_001e00c0 = *(undefined8 *)(piVar3 + 0x2c);
    uRam00000000001e00d8 = *(undefined8 *)(piVar3 + 0x32);
    _DAT_001e00d0 = *(undefined8 *)(piVar3 + 0x30);
    uRam00000000001e00a8 = *(undefined8 *)(piVar3 + 0x26);
    _DAT_001e00a0 = *(undefined8 *)(piVar3 + 0x24);
    uRam00000000001e00b8 = *(undefined8 *)(piVar3 + 0x2a);
    _DAT_001e00b0 = *(undefined8 *)(piVar3 + 0x28);
    DAT_0020af48 = *(undefined8 *)(param_1 + 10);
    uRam00000000001e0018 = *(undefined8 *)(piVar3 + 2);
    _DAT_001e0010 = *(undefined8 *)piVar3;
    DAT_0020b070 = 1;
    DAT_001cfbf0 = 0xffffffff;
    FUN_001428fc(param_1,*(long *)(piVar3 + 0x10) + 0x3c,&DAT_001cfbf0,4);
    uVar1 = FUN_00139fcc();
    uVar2 = *(undefined8 *)(param_1 + 10);
  }
  FUN_0017f7c4(uVar1,uVar2,DAT_001cfbf0);
  return;
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

/* ===== FUN_0014bef8 @ 0014bef8 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

FILE * FUN_0014bef8(uint param_1,undefined4 *param_2,long param_3,ulong param_4)

{
  int iVar1;
  undefined *puVar2;
  long lVar3;
  undefined8 uVar4;
  bool bVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  int *piVar9;
  undefined8 *puVar10;
  FILE *pFVar11;
  FILE *pFVar12;
  char *pcVar13;
  FILE *pFVar14;
  int iVar15;
  FILE *pFVar16;
  FILE *pFVar17;
  ulong *puVar18;
  int iVar19;
  FILE *local_2c8;
  FILE *pFStack_2c0;
  FILE *local_2b8;
  FILE *pFStack_2b0;
  ulong local_2a8;
  FILE *local_2a0;
  ulong local_298;
  long lStack_290;
  undefined4 *local_288;
  long lStack_280;
  char local_278 [2];
  char local_276;
  char local_275;
  char acStack_270 [512];
  long local_70;
  
  lVar3 = tpidr_el0;
  local_70 = *(long *)(lVar3 + 0x28);
  piVar9 = (int *)__errno();
  iVar15 = *piVar9;
  puVar10 = (undefined8 *)FUN_001bd828(&DAT_001cfcd8);
  puVar2 = &DAT_00116561;
  if (param_1 != 0x7fff0001) {
    puVar2 = &DAT_00117c1b;
  }
  puVar10[5] = 0;
  puVar10[6] = 0;
  *puVar10 = puVar2;
  puVar10[1] = "write";
  puVar10[2] = &DAT_0011a7d3;
  puVar10[3] = param_3;
  puVar18 = puVar10 + 4;
  *puVar18 = 0;
  local_298 = param_4;
  lStack_290 = param_3;
  local_288 = param_2;
  lStack_280 = param_3;
  if (param_1 == 0x7fff0001) {
    uVar8 = getpid();
    pFVar11 = (FILE *)syscall(0x10f,(ulong)uVar8,&local_288,1,&local_298,1,0);
    if ((long)pFVar11 < 0) goto LAB_0014bff4;
LAB_0014c058:
    *puVar18 = (ulong)pFVar11;
    if ((long)pFVar11 < 0) {
LAB_0014c064:
      iVar19 = *piVar9;
      goto LAB_0014c288;
    }
LAB_0014cd48:
    *(undefined4 *)(puVar10 + 6) = 0;
    pFVar17 = pFVar11;
  }
  else {
    pFVar11 = (FILE *)syscall(0x44,(ulong)param_1,param_2,param_3,param_4);
    if (-1 < (long)pFVar11) goto LAB_0014c058;
LAB_0014bff4:
    iVar19 = *piVar9;
    if (iVar19 == 4) {
      if (param_1 == 0x7fff0001) {
        uVar8 = getpid();
        pFVar11 = (FILE *)syscall(0x10f,(ulong)uVar8,&local_288,1,&local_298,1,0);
      }
      else {
        pFVar11 = (FILE *)syscall(0x44,(ulong)param_1,param_2,param_3,param_4);
      }
      if (-1 < (long)pFVar11) goto LAB_0014c058;
      iVar19 = *piVar9;
      if (iVar19 != 4) goto LAB_0014c284;
      if (param_1 == 0x7fff0001) {
        uVar8 = getpid();
        pFVar11 = (FILE *)syscall(0x10f,(ulong)uVar8,&local_288,1,&local_298,1,0);
      }
      else {
        pFVar11 = (FILE *)syscall(0x44,(ulong)param_1,param_2,param_3,param_4);
      }
      if (-1 < (long)pFVar11) goto LAB_0014c058;
      iVar19 = *piVar9;
      if (iVar19 != 4) goto LAB_0014c284;
      if (param_1 == 0x7fff0001) {
        uVar8 = getpid();
        pFVar11 = (FILE *)syscall(0x10f,(ulong)uVar8,&local_288,1,&local_298,1,0);
      }
      else {
        pFVar11 = (FILE *)syscall(0x44,(ulong)param_1,param_2,param_3,param_4);
      }
      if (-1 < (long)pFVar11) goto LAB_0014c058;
      iVar19 = *piVar9;
      if (iVar19 != 4) goto LAB_0014c284;
      if (param_1 == 0x7fff0001) {
        uVar8 = getpid();
        pFVar11 = (FILE *)syscall(0x10f,(ulong)uVar8,&local_288,1,&local_298,1,0);
      }
      else {
        pFVar11 = (FILE *)syscall(0x44,(ulong)param_1,param_2,param_3,param_4);
      }
      if (-1 < (long)pFVar11) goto LAB_0014c058;
      iVar19 = *piVar9;
      if (iVar19 != 4) goto LAB_0014c284;
      if (param_1 == 0x7fff0001) {
        uVar8 = getpid();
        pFVar11 = (FILE *)syscall(0x10f,(ulong)uVar8,&local_288,1,&local_298,1,0);
      }
      else {
        pFVar11 = (FILE *)syscall(0x44,(ulong)param_1,param_2,param_3,param_4);
      }
      if (-1 < (long)pFVar11) goto LAB_0014c058;
      iVar19 = *piVar9;
      if (iVar19 != 4) goto LAB_0014c284;
      if (param_1 == 0x7fff0001) {
        uVar8 = getpid();
        pFVar11 = (FILE *)syscall(0x10f,(ulong)uVar8,&local_288,1,&local_298,1,0);
      }
      else {
        pFVar11 = (FILE *)syscall(0x44,(ulong)param_1,param_2,param_3,param_4);
      }
      if ((long)pFVar11 < 0) {
        iVar19 = *piVar9;
        if (iVar19 != 4) goto LAB_0014c284;
        if (param_1 == 0x7fff0001) {
          uVar8 = getpid();
          pFVar11 = (FILE *)syscall(0x10f,(ulong)uVar8,&local_288,1,&local_298,1,0);
        }
        else {
          pFVar11 = (FILE *)syscall(0x44,(ulong)param_1,param_2,param_3,param_4);
        }
      }
      *puVar18 = (ulong)pFVar11;
      if (-1 < (long)pFVar11) goto LAB_0014cd48;
      goto LAB_0014c064;
    }
LAB_0014c284:
    *puVar18 = (ulong)pFVar11;
LAB_0014c288:
    *(int *)(puVar10 + 6) = iVar19;
    pFVar17 = pFVar11;
    if ((param_1 != 0x7fff0001) || (iVar19 == 4)) goto LAB_0014cd54;
    *(undefined4 *)((long)puVar10 + 0x34) = 0;
    puVar10[5] = 0xffffffffffffffff;
    puVar10[2] = "range";
    if (((param_3 == 4) && ((param_4 & 3) == 0)) &&
       (((pFVar11 = (FILE *)sysconf(0x27), pFVar11 == (FILE *)0x4000 || (pFVar11 == (FILE *)0x1000))
        && (pFVar16 = (FILE *)(-(long)pFVar11 & param_4), !CARRY8((ulong)pFVar11,(ulong)pFVar16)))))
    {
      pFVar12 = fopen("/proc/self/maps","r");
      bVar5 = pFVar12 != (FILE *)0x0;
      if (pFVar12 == (FILE *)0x0) {
        iVar6 = *piVar9;
        if (iVar6 != 4) goto LAB_0014c534;
        pFVar12 = fopen("/proc/self/maps","r");
        bVar5 = pFVar12 != (FILE *)0x0;
        if (pFVar12 != (FILE *)0x0) goto LAB_0014c314;
        iVar6 = *piVar9;
        if (iVar6 != 4) goto LAB_0014c534;
        pFVar12 = fopen("/proc/self/maps","r");
        bVar5 = pFVar12 != (FILE *)0x0;
        if (pFVar12 != (FILE *)0x0) goto LAB_0014c314;
        iVar6 = *piVar9;
        if (iVar6 != 4) goto LAB_0014c534;
        pFVar12 = fopen("/proc/self/maps","r");
        bVar5 = pFVar12 != (FILE *)0x0;
        if (pFVar12 != (FILE *)0x0) goto LAB_0014c314;
        iVar6 = *piVar9;
        if (iVar6 != 4) goto LAB_0014c534;
        pFVar12 = fopen("/proc/self/maps","r");
        bVar5 = pFVar12 != (FILE *)0x0;
        if (pFVar12 != (FILE *)0x0) goto LAB_0014c314;
        iVar6 = *piVar9;
        if (iVar6 != 4) goto LAB_0014c534;
        pFVar12 = fopen("/proc/self/maps","r");
        bVar5 = pFVar12 != (FILE *)0x0;
        if (pFVar12 != (FILE *)0x0) goto LAB_0014c314;
        iVar6 = *piVar9;
        if (iVar6 != 4) goto LAB_0014c534;
        pFVar12 = fopen("/proc/self/maps","r");
        bVar5 = pFVar12 != (FILE *)0x0;
        if (pFVar12 != (FILE *)0x0) goto LAB_0014c314;
        iVar6 = *piVar9;
        if (iVar6 != 4) goto LAB_0014c534;
        pFVar12 = fopen("/proc/self/maps","r");
        if (pFVar12 == (FILE *)0x0) goto LAB_0014c530;
LAB_0014c31c:
        uVar8 = 0;
LAB_0014c338:
        do {
          *piVar9 = 0;
          pcVar13 = fgets(acStack_270,0x200,pFVar12);
          if (pcVar13 == (char *)0x0) {
            iVar6 = *piVar9;
            iVar7 = ferror(pFVar12);
            if ((iVar7 != 0) && (iVar6 == 4)) {
              uVar8 = uVar8 + 1;
              if (uVar8 < 8) {
                clearerr(pFVar12);
                goto LAB_0014c338;
              }
              iVar6 = 4;
            }
            iVar1 = 0;
            if (iVar7 != 0) {
              iVar1 = iVar6;
            }
            puVar10[5] = 0xffffffffffffffff;
            *(int *)((long)puVar10 + 0x34) = iVar1;
            puVar10[2] = &DAT_001153e1;
            uVar8 = fclose(pFVar12);
            pFVar11 = (FILE *)(ulong)uVar8;
            goto LAB_0014c57c;
          }
          iVar6 = sscanf(acStack_270,"%lx-%lx %4s",&local_2a0,&local_2a8,local_278);
          uVar8 = 0;
        } while (((iVar6 != 3) || (uVar8 = 0, pFVar16 < local_2a0)) ||
                ((uVar8 = 0, local_2a8 < (ulong)((long)&pFVar11->_flags + (long)&pFVar16->_flags) ||
                 (((local_278[0] != 'r' || (local_276 != 'x')) || (local_275 != 'p'))))));
        fclose(pFVar12);
        pFVar12 = (FILE *)syscall(0xde,0,pFVar11,3,0x22,0xffffffff,0);
        if ((long)pFVar12 < 0) {
          iVar6 = *piVar9;
          if (iVar6 == 4) {
            pFVar12 = (FILE *)syscall(0xde,0,pFVar11,3,0x22,0xffffffff,0);
            if (-1 < (long)pFVar12) {
LAB_0014c648:
              uVar8 = (uint)((ulong)pFVar12 >> 0x3f);
              goto LAB_0014c5c8;
            }
            iVar6 = *piVar9;
            if (iVar6 == 4) {
              pFVar12 = (FILE *)syscall(0xde,0,pFVar11,3,0x22,0xffffffff,0);
              if (-1 < (long)pFVar12) goto LAB_0014c648;
              iVar6 = *piVar9;
              if (iVar6 == 4) {
                pFVar12 = (FILE *)syscall(0xde,0,pFVar11,3,0x22,0xffffffff,0);
                if (-1 < (long)pFVar12) {
LAB_0014c718:
                  uVar8 = (uint)((ulong)pFVar12 >> 0x3f);
                  goto LAB_0014c5c8;
                }
                iVar6 = *piVar9;
                if (iVar6 == 4) {
                  pFVar12 = (FILE *)syscall(0xde,0,pFVar11,3,0x22,0xffffffff,0);
                  if (-1 < (long)pFVar12) goto LAB_0014c718;
                  iVar6 = *piVar9;
                  if (iVar6 == 4) {
                    pFVar12 = (FILE *)syscall(0xde,0,pFVar11,3,0x22,0xffffffff,0);
                    if (-1 < (long)pFVar12) goto LAB_0014c718;
                    iVar6 = *piVar9;
                    if (iVar6 == 4) {
                      pFVar12 = (FILE *)syscall(0xde,0,pFVar11,3,0x22,0xffffffff,0);
                      if (-1 < (long)pFVar12) goto LAB_0014c718;
                      iVar6 = *piVar9;
                      if (iVar6 == 4) {
                        pFVar12 = (FILE *)syscall(0xde,0,pFVar11,3,0x22,0xffffffff,0);
                        goto LAB_0014c648;
                      }
                    }
                  }
                }
              }
            }
          }
        }
        else {
          uVar8 = (uint)((ulong)pFVar12 >> 0x3f);
LAB_0014c5c8:
          if (0 < (long)pFVar12) {
            local_2c8 = pFVar16;
            pFStack_2c0 = pFVar11;
            local_2b8 = pFVar12;
            pFStack_2b0 = pFVar11;
            uVar8 = getpid();
            pFVar14 = (FILE *)syscall(0x10e,(ulong)uVar8,&local_2b8,1,&local_2c8,1,0);
            if ((long)pFVar14 < 0) {
              if (*piVar9 == 4) {
                uVar8 = getpid();
                pFVar14 = (FILE *)syscall(0x10e,(ulong)uVar8,&local_2b8,1,&local_2c8,1,0);
                if ((long)pFVar14 < 0) {
                  if (*piVar9 != 4) goto LAB_0014c798;
                  uVar8 = getpid();
                  pFVar14 = (FILE *)syscall(0x10e,(ulong)uVar8,&local_2b8,1,&local_2c8,1,0);
                  if (-1 < (long)pFVar14) goto LAB_0014c6a0;
                  if (*piVar9 == 4) {
                    uVar8 = getpid();
                    pFVar14 = (FILE *)syscall(0x10e,(ulong)uVar8,&local_2b8,1,&local_2c8,1,0);
                    if ((long)pFVar14 < 0) {
                      if (*piVar9 != 4) goto LAB_0014cb78;
                      uVar8 = getpid();
                      pFVar14 = (FILE *)syscall(0x10e,(ulong)uVar8,&local_2b8,1,&local_2c8,1,0);
                      if ((long)pFVar14 < 0) {
                        if (*piVar9 != 4) goto LAB_0014cb78;
                        uVar8 = getpid();
                        pFVar14 = (FILE *)syscall(0x10e,(ulong)uVar8,&local_2b8,1,&local_2c8,1,0);
                        if ((long)pFVar14 < 0) {
                          if (*piVar9 != 4) goto LAB_0014cb78;
                          uVar8 = getpid();
                          pFVar14 = (FILE *)syscall(0x10e,(ulong)uVar8,&local_2b8,1,&local_2c8,1,0);
                          if ((long)pFVar14 < 0) {
                            if (*piVar9 != 4) goto LAB_0014cb78;
                            uVar8 = getpid();
                            pFVar14 = (FILE *)syscall(0x10e,(ulong)uVar8,&local_2b8,1,&local_2c8,1,0
                                                     );
                          }
                        }
                      }
                    }
                    uVar8 = (uint)((ulong)pFVar14 >> 0x3f);
                  }
                  else {
LAB_0014cb78:
                    uVar8 = 1;
                  }
                }
                else {
LAB_0014c6a0:
                  uVar8 = (uint)((ulong)pFVar14 >> 0x3f);
                }
              }
              else {
LAB_0014c798:
                uVar8 = 1;
              }
              if (pFVar14 == pFVar11) goto LAB_0014c7a4;
LAB_0014cb8c:
              iVar6 = *piVar9;
              pcVar13 = "copy";
LAB_0014cb98:
              puVar10[2] = pcVar13;
              if (uVar8 == 0) {
                iVar6 = 0;
              }
            }
            else {
              uVar8 = (uint)((ulong)pFVar14 >> 0x3f);
              if (pFVar14 != pFVar11) goto LAB_0014cb8c;
LAB_0014c7a4:
              *(undefined4 *)((long)pFVar12 + (param_4 - (long)pFVar16)) = *param_2;
              FUN_001bdaf0(pFVar12,(long)&pFVar11->_flags + (long)&pFVar12->_flags);
              pFVar14 = (FILE *)syscall(0xe2,pFVar12,pFVar11,5);
              if (-1 < (long)pFVar14) {
                uVar8 = (uint)((ulong)pFVar14 >> 0x3f);
LAB_0014c7d8:
                if (pFVar14 != (FILE *)0x0) {
                  iVar6 = *piVar9;
                  if (uVar8 == 0) {
                    iVar6 = 0;
                  }
                  goto LAB_0014c9bc;
                }
                pFVar14 = (FILE *)syscall(0xd8,pFVar12,pFVar11,pFVar11,3,pFVar16,0);
                if ((long)pFVar14 < 0) {
                  if (*piVar9 == 4) {
                    pFVar14 = (FILE *)syscall(0xd8,pFVar12,pFVar11,pFVar11,3,pFVar16,0);
                    if ((long)pFVar14 < 0) {
                      if (*piVar9 == 4) {
                        pFVar14 = (FILE *)syscall(0xd8,pFVar12,pFVar11,pFVar11,3,pFVar16,0);
                        if (-1 < (long)pFVar14) {
LAB_0014c8e8:
                          uVar8 = (uint)((ulong)pFVar14 >> 0x3f);
                          goto LAB_0014ccc4;
                        }
                        if (*piVar9 == 4) {
                          pFVar14 = (FILE *)syscall(0xd8,pFVar12,pFVar11,pFVar11,3,pFVar16,0);
                          if (-1 < (long)pFVar14) goto LAB_0014c8e8;
                          if (*piVar9 == 4) {
                            pFVar14 = (FILE *)syscall(0xd8,pFVar12,pFVar11,pFVar11,3,pFVar16,0);
                            if (-1 < (long)pFVar14) goto LAB_0014c8e8;
                            if (*piVar9 == 4) {
                              pFVar14 = (FILE *)syscall(0xd8,pFVar12,pFVar11,pFVar11,3,pFVar16,0);
                              if (-1 < (long)pFVar14) goto LAB_0014c8e8;
                              if (*piVar9 == 4) {
                                pFVar14 = (FILE *)syscall(0xd8,pFVar12,pFVar11,pFVar11,3,pFVar16,0);
                                if (-1 < (long)pFVar14) goto LAB_0014c8e8;
                                if (*piVar9 == 4) {
                                  pFVar14 = (FILE *)syscall(0xd8,pFVar12,pFVar11,pFVar11,3,pFVar16,0
                                                           );
                                  goto LAB_0014c874;
                                }
                              }
                            }
                          }
                        }
                      }
                      uVar8 = 1;
                    }
                    else {
LAB_0014c874:
                      uVar8 = (uint)((ulong)pFVar14 >> 0x3f);
                    }
                  }
                  else {
                    uVar8 = 1;
                  }
                }
                else {
                  uVar8 = (uint)((ulong)pFVar14 >> 0x3f);
                }
LAB_0014ccc4:
                if (pFVar14 == pFVar16) {
                  pFVar11 = (FILE *)FUN_001bdaf0(pFVar16,(long)&pFVar11->_flags +
                                                         (long)&pFVar16->_flags);
                  puVar10[6] = 0;
                  uVar4 = _DAT_00112900;
                  puVar10[5] = _UNK_00112908;
                  puVar10[4] = uVar4;
                  puVar10[2] = &DAT_0011a7d3;
                  pFVar17 = (FILE *)0x4;
                  goto LAB_0014cd50;
                }
                iVar6 = *piVar9;
                pcVar13 = "remap";
                goto LAB_0014cb98;
              }
              iVar6 = *piVar9;
              if (iVar6 == 4) {
                pFVar14 = (FILE *)syscall(0xe2,pFVar12,pFVar11,5);
                if (-1 < (long)pFVar14) {
LAB_0014c810:
                  uVar8 = (uint)((ulong)pFVar14 >> 0x3f);
                  goto LAB_0014c7d8;
                }
                iVar6 = *piVar9;
                if (iVar6 == 4) {
                  pFVar14 = (FILE *)syscall(0xe2,pFVar12,pFVar11,5);
                  if (-1 < (long)pFVar14) {
LAB_0014c8a0:
                    uVar8 = (uint)((ulong)pFVar14 >> 0x3f);
                    goto LAB_0014c7d8;
                  }
                  iVar6 = *piVar9;
                  if (iVar6 == 4) {
                    pFVar14 = (FILE *)syscall(0xe2,pFVar12,pFVar11,5);
                    if (-1 < (long)pFVar14) goto LAB_0014c8a0;
                    iVar6 = *piVar9;
                    if (iVar6 == 4) {
                      pFVar14 = (FILE *)syscall(0xe2,pFVar12,pFVar11,5);
                      if (-1 < (long)pFVar14) goto LAB_0014c8a0;
                      iVar6 = *piVar9;
                      if (iVar6 == 4) {
                        pFVar14 = (FILE *)syscall(0xe2,pFVar12,pFVar11,5);
                        if (-1 < (long)pFVar14) goto LAB_0014c8a0;
                        iVar6 = *piVar9;
                        if (iVar6 == 4) {
                          pFVar14 = (FILE *)syscall(0xe2,pFVar12,pFVar11,5);
                          if (-1 < (long)pFVar14) goto LAB_0014c8a0;
                          iVar6 = *piVar9;
                          if (iVar6 == 4) {
                            pFVar14 = (FILE *)syscall(0xe2,pFVar12,pFVar11,5);
                            goto LAB_0014c810;
                          }
                        }
                      }
                    }
                  }
                }
              }
LAB_0014c9bc:
              puVar10[2] = &DAT_0011a7ce;
            }
            puVar10[5] = pFVar14;
            *(int *)((long)puVar10 + 0x34) = iVar6;
            pFVar11 = (FILE *)syscall(0xd7,pFVar12,pFVar11);
            goto LAB_0014c57c;
          }
          iVar6 = *piVar9;
          if (uVar8 == 0) {
            iVar6 = 0;
          }
        }
        puVar10[5] = pFVar12;
        puVar10[2] = "alloc";
        pFVar11 = pFVar12;
      }
      else {
LAB_0014c314:
        if (bVar5) goto LAB_0014c31c;
LAB_0014c530:
        iVar6 = *piVar9;
LAB_0014c534:
        puVar10[5] = 0xffffffffffffffff;
        puVar10[2] = &DAT_001153e1;
        pFVar11 = pFVar12;
      }
      *(int *)((long)puVar10 + 0x34) = iVar6;
    }
LAB_0014c57c:
    if (iVar19 != 0) {
      iVar15 = iVar19;
    }
  }
LAB_0014cd50:
  *piVar9 = iVar15;
LAB_0014cd54:
  if (*(long *)(lVar3 + 0x28) == local_70) {
    return pFVar17;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(pFVar11);
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

/* ===== FUN_0014f01c @ 0014f01c [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0014f01c(ulong *param_1)

{
  long lVar1;
  int iVar2;
  undefined8 uVar3;
  int iVar4;
  undefined1 auStack_160 [4];
  byte local_15c;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  iVar2 = (int)*param_1;
  if (iVar2 + 1 < 0 == SCARRY4(iVar2,1)) {
    iVar4 = *(int *)((long)param_1 + 4);
    if ((iVar4 + 2 < 0 == SCARRY4(iVar4,2)) && ((uint)param_1[1] < 8)) {
      if (-1 < iVar2) {
        uVar3 = FUN_0014f294(iVar2,auStack_160);
        if ((int)uVar3 == 0) goto LAB_0014f068;
        if ((local_15c >> 1 & 1) == 0) goto LAB_0014f064;
        iVar4 = *(int *)((long)param_1 + 4);
      }
      if (-1 < iVar4) {
        uVar3 = FUN_0014f294(iVar4,auStack_160);
        if ((int)uVar3 == 0) goto LAB_0014f068;
        iVar4 = *(int *)((long)param_1 + 4);
      }
      uVar3 = FUN_0014ea08(0,(int)*param_1,iVar4);
      if ((int)uVar3 == 0) goto LAB_0014f068;
      iVar2 = DAT_001cfb10 + 1;
      if (DAT_001cfb10 == -1) {
        iVar2 = 1;
      }
      *(int *)((long)param_1 + 0xc) = iVar2;
      iVar2 = FUN_0017dea8(0,param_1);
      if (iVar2 != 0) {
        _DAT_001cfb0c = param_1[1];
        _DAT_001cfb04 = *param_1;
        _DAT_001cfb14 = param_1[2];
        uVar3 = 1;
        goto LAB_0014f068;
      }
      FUN_0014ea08(0,_DAT_001cfb04 & 0xffffffff,DAT_001cfb08);
    }
  }
LAB_0014f064:
  uVar3 = 0;
LAB_0014f068:
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar3);
}

/* ===== FUN_00150860 @ 00150860 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00150860(undefined8 param_1,undefined8 param_2,undefined8 param_3,long param_4,int *param_5
                 ,int *param_6)

{
  long lVar1;
  undefined8 uVar2;
  int iVar3;
  int iVar4;
  undefined8 uVar5;
  undefined1 auStack_2d8 [16];
  int local_2c8;
  undefined4 local_2c0;
  undefined4 uStack_2bc;
  int local_2ac;
  undefined8 local_298;
  undefined8 local_290;
  undefined8 uStack_288;
  undefined8 local_280;
  undefined8 uStack_278;
  undefined8 uStack_270;
  undefined8 uStack_268;
  undefined8 local_260;
  undefined8 local_258;
  undefined8 local_250;
  undefined8 local_248;
  undefined8 local_240;
  undefined8 uStack_238;
  undefined8 local_230;
  undefined4 local_228;
  undefined8 local_220;
  undefined8 uStack_218;
  undefined8 local_210;
  undefined8 uStack_208;
  undefined8 local_200;
  undefined4 local_1f8;
  undefined4 uStack_1f4;
  long local_1f0;
  undefined8 uStack_1e8;
  undefined8 local_1d8;
  long local_1d0;
  undefined8 uStack_1c8;
  undefined8 local_1c0;
  undefined8 uStack_1b8;
  undefined8 local_1b0;
  undefined8 uStack_1a8;
  undefined8 local_1a0;
  undefined8 uStack_198;
  undefined8 local_190;
  undefined8 uStack_188;
  undefined8 local_180;
  undefined8 uStack_178;
  undefined8 local_170;
  undefined8 uStack_168;
  undefined8 local_160;
  undefined8 uStack_158;
  undefined8 local_150;
  undefined8 uStack_148;
  undefined8 local_140;
  undefined8 uStack_138;
  undefined8 local_130;
  undefined8 uStack_128;
  undefined8 local_120;
  long local_118;
  undefined8 uStack_110;
  undefined8 local_108;
  undefined8 uStack_100;
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
  long local_68;
  
  iVar4 = DAT_00209cd8;
  lVar1 = tpidr_el0;
  uVar5 = 0;
  local_68 = *(long *)(lVar1 + 0x28);
  if ((param_5 == (int *)0x0) || (param_6 == (int *)0x0)) goto LAB_00150b18;
  if (DAT_00209cd8 != 0) {
    iVar3 = gettid(0);
    uVar5 = 0;
    if ((iVar4 != iVar3) || (DAT_0020d158 != 1)) goto LAB_00150b18;
    iVar4 = FUN_00150bf0(param_2,param_3);
    uVar2 = DAT_0010e7d0;
    uVar5 = 0;
    if ((iVar4 == 0) || (DAT_0020d15c == '\0')) goto LAB_00150b18;
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
    uStack_100 = 0;
    local_108 = 0;
    uStack_110 = 0;
    local_118 = 0;
    local_120 = DAT_0010e7d0;
    if (DAT_0020d160 != (code *)0x0) {
      iVar4 = (*DAT_0020d160)(&local_120);
      uVar5 = 0;
      if ((((iVar4 == 0) || (local_c8._4_4_ == 0)) || ((int)uStack_c0 == 0)) ||
         (((uStack_f0._4_4_ == 0 || ((int)local_e8 < 1)) ||
          ((2 < (int)local_e8 || ((uVar5 = 0, local_118 != param_4 || (DAT_0020f624 == 0))))))))
      goto LAB_00150b18;
      uStack_238 = _UNK_00112978;
      local_240 = _DAT_00112970;
      uStack_218 = DAT_0020f678;
      local_230 = DAT_0010e588;
      uStack_208 = DAT_0020f6f8;
      local_200 = DAT_0020f650;
      local_228 = 0;
      local_1f8 = DAT_0020f690;
      uStack_1f4 = DAT_0020f638._4_4_;
      uStack_1e8 = 0;
      uStack_278 = 0;
      local_280 = 0;
      uStack_268 = 0;
      uStack_270 = 0;
      uStack_288 = 0;
      local_290 = 0;
      local_258 = 0;
      local_250 = 0;
      local_248 = 0;
      local_260 = 0;
      local_298 = DAT_0010e7f8;
      local_220 = param_2;
      local_210 = param_3;
      local_1f0 = param_4;
      iVar4 = FUN_00185c68(&DAT_0020d168,&DAT_0020f628,&local_240,FUN_00150f7c,0,&local_258,
                           &local_298);
      uVar5 = 0;
      if (iVar4 != 1) goto LAB_00150b18;
      uVar5 = 0;
      if ((ABS(local_250._4_4_) == INFINITY) || (NAN(ABS(local_250._4_4_)))) goto LAB_00150b18;
      uVar5 = 0;
      if (((ABS((float)local_248) == INFINITY) ||
          (((NAN(ABS((float)local_248)) || (local_250._4_4_ < 0.0)) || ((float)local_248 < 0.0))))
         || ((local_250._4_4_ < 200.0 == NAN(local_250._4_4_) ||
             ((float)local_248 < 200.0 == NAN((float)local_248))))) goto LAB_00150b18;
      iVar4 = FUN_00189c88(DAT_001e0978,local_258,FUN_001428fc,0,auStack_2d8);
      uVar5 = 0;
      if ((iVar4 == 0) || (local_2ac == 0)) goto LAB_00150b18;
      if ((local_2c8 == (int)local_250) &&
         (iVar4 = FUN_0017fba8(DAT_0020f698,DAT_0020f69c,local_2c0,uStack_2bc), iVar4 == 0)) {
        uVar5 = FUN_00150bf0(param_2,param_3);
        if ((int)uVar5 != 0) {
          local_1d8 = uVar2;
          uStack_128 = 0;
          local_130 = 0;
          uStack_138 = 0;
          local_140 = 0;
          uStack_148 = 0;
          local_150 = 0;
          uStack_158 = 0;
          local_160 = 0;
          uStack_168 = 0;
          local_170 = 0;
          uStack_178 = 0;
          local_180 = 0;
          uStack_188 = 0;
          local_190 = 0;
          uStack_198 = 0;
          local_1a0 = 0;
          uStack_1a8 = 0;
          local_1b0 = 0;
          uStack_1b8 = 0;
          local_1c0 = 0;
          uStack_1c8 = 0;
          local_1d0 = 0;
          uVar5 = (*DAT_0020d160)(&local_1d8);
          if (((((int)uVar5 != 0) && (uVar5 = 0, local_1d0 == param_4)) && (local_180._4_4_ != 0))
             && (uStack_1a8._4_4_ != 0)) {
            uVar5 = 1;
            *param_5 = (int)(local_250._4_4_ * 300.0);
            *param_6 = (int)((float)local_248 * 300.0);
          }
        }
        goto LAB_00150b18;
      }
    }
  }
  uVar5 = 0;
LAB_00150b18:
  if (*(long *)(lVar1 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar5);
}

/* ===== FUN_00150bf0 @ 00150bf0 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00150bf0(long param_1,long param_2)

{
  long lVar1;
  int iVar2;
  undefined8 uVar3;
  undefined8 local_138;
  undefined8 uStack_130;
  undefined8 uStack_128;
  undefined8 uStack_120;
  undefined8 uStack_118;
  undefined8 uStack_110;
  undefined8 local_108;
  undefined8 local_100;
  undefined8 local_f8;
  undefined8 uStack_f0;
  long local_e8;
  ulong uStack_e0;
  ulong local_d8;
  long lStack_d0;
  int local_c8;
  undefined1 auStack_c0 [16];
  int local_b0;
  int local_ac;
  int local_94;
  int local_7c;
  int local_78;
  int local_74;
  long local_70;
  long local_68;
  ulong local_60;
  ulong local_58;
  ulong local_50;
  long local_48;
  
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  uVar3 = 0;
  if ((((DAT_0020f6cc != 0) && ((int)DAT_0020f638 != 0)) && (DAT_0020f670 == param_1)) &&
     ((DAT_0020f680 == param_2 &&
      (uVar3 = FUN_001428fc(0,DAT_001e0978 + 0x1307e20,&local_50,8), (int)uVar3 != 0)))) {
    uVar3 = 0;
    if ((0x11fff < local_50 + 0x2000) && ((local_50 & 7) == 0)) {
      iVar2 = FUN_001428fc(0,local_50 + 0x50,&local_74,4);
      uVar3 = 0;
      if (((iVar2 != 0) && (local_74 == 5)) &&
         (uVar3 = FUN_001428fc(0,local_50 + 0x48,&local_58,8), (int)uVar3 != 0)) {
        uVar3 = 0;
        if ((((local_58 - 0x10000 < 0xfffffffffffee000) && ((local_58 & 7) == 0)) &&
            (local_58 == DAT_0020f708)) &&
           (uVar3 = FUN_001428fc(0,param_1 + 0x918,&local_60,8), (int)uVar3 != 0)) {
          uVar3 = 0;
          if (((local_60 - 0x10000 < 0xfffffffffffee000) && ((local_60 & 7) == 0)) &&
             ((local_60 == local_58 &&
              (uVar3 = FUN_0013a78c(local_60 + 0x28,&local_60), (int)uVar3 != 0)))) {
            if (local_60 == DAT_0020f678) {
              uVar3 = FUN_0013a78c(local_60 + 0x28,&local_68);
              if ((int)uVar3 == 0) goto LAB_00150f30;
              if (local_68 == DAT_0020f7a8) {
                uVar3 = FUN_0013a78c(local_68,&local_70);
                if ((int)uVar3 == 0) goto LAB_00150f30;
                if (local_70 == DAT_0020f7b0) {
                  uVar3 = FUN_001428fc(uVar3,local_68 + 0xc,&local_78,4);
                  if ((int)uVar3 == 0) goto LAB_00150f30;
                  if (local_78 == DAT_0020f760) {
                    uVar3 = FUN_001428fc(uVar3,local_60 + 0x100,&local_7c,4);
                    if ((int)uVar3 == 0) goto LAB_00150f30;
                    if (local_7c == DAT_0020f690) {
                      iVar2 = FUN_00189c88(DAT_001e0978,param_2,FUN_001428fc,0,auStack_c0);
                      uVar3 = 0;
                      if ((iVar2 == 0) || (local_94 == 0)) goto LAB_00150f30;
                      if ((local_b0 == DAT_0020f690) && (local_ac == DAT_0020f694)) {
                        local_c8 = local_7c;
                        local_100 = DAT_0010e7f8;
                        uStack_f0 = DAT_0020f6f8;
                        local_f8 = DAT_0020f6f0;
                        uStack_118 = 0;
                        uStack_120 = 0;
                        local_108 = 0;
                        uStack_110 = 0;
                        local_d8 = local_60;
                        uStack_e0 = local_58;
                        uStack_128 = 0;
                        uStack_130 = 0;
                        local_138 = DAT_0010e7a8;
                        local_e8 = param_1;
                        lStack_d0 = param_2;
                        if (((DAT_0020adec != '\x01') ||
                            (iVar2 = FUN_0018a8cc(&DAT_0020adf0,&local_100,&local_138), iVar2 == 0))
                           || ((int)uStack_130 == 0)) {
                          uVar3 = 0;
                          _DAT_0020f7e8 = 0;
                          uRam000000000020f7d0 = 0;
                          _DAT_0020f7c8 = 0;
                          uRam000000000020f7e0 = 0;
                          _DAT_0020f7d8 = 0;
                          _DAT_0020f7c0 = 0;
                          _DAT_0020f7b8 = 0;
                          goto LAB_00150f30;
                        }
                        uVar3 = 0;
                        _DAT_0020f7c0 = uStack_130;
                        _DAT_0020f7b8 = local_138;
                        uRam000000000020f7d0 = uStack_120;
                        _DAT_0020f7c8 = uStack_128;
                        uRam000000000020f7e0 = uStack_110;
                        _DAT_0020f7d8 = uStack_118;
                        _DAT_0020f7e8 = local_108;
                        if ((uStack_130._4_4_ == 0) || ((int)local_108 != 0)) goto LAB_00150f30;
                        if ((DAT_001cfe98 == (code *)0x0) ||
                           (iVar2 = (*DAT_001cfe98)(0,&local_100), iVar2 == 1)) {
                          uVar3 = 1;
                          goto LAB_00150f30;
                        }
                      }
                    }
                  }
                }
              }
            }
            uVar3 = 0;
          }
        }
      }
    }
  }
LAB_00150f30:
  if (*(long *)(lVar1 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar3);
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

/* ===== FUN_00153edc @ 00153edc [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00153edc(long param_1)

{
  char cVar1;
  bool bVar2;
  char cVar3;
  int iVar4;
  int iVar5;
  ulong uVar6;
  long lVar7;
  
  iVar4 = DAT_00209cd8;
  if (((((DAT_00214940 == '\x01') && (*(int *)(param_1 + 8) == 1)) && (DAT_0020d158 == 0)) &&
      (((*(long *)(param_1 + 0x10) == DAT_00209d00 && (*(long *)(param_1 + 0x20) == DAT_001e0978))
       && ((*(long *)(param_1 + 0x28) == *(long *)(param_1 + 0x20) + 0xb2e674 &&
           ((*(long *)(param_1 + 0x30) == DAT_00214958 &&
            (*(long *)(param_1 + 0x38) == DAT_00214960)))))))) &&
     ((DAT_00209cd8 != 0 &&
      ((((iVar5 = gettid(), iVar4 == iVar5 && (DAT_00214970 != 0)) &&
        (uVar6 = FUN_001391f0(1), DAT_00214970 <= uVar6)) &&
       (lVar7 = FUN_001391f0(1), lVar7 - DAT_00214970 < 0x1f5)))))) {
    do {
      cVar3 = DAT_001cfe94;
      cVar1 = '\x01';
      bVar2 = (bool)ExclusiveMonitorPass(0x1cfe94,0x10);
      if (bVar2) {
        _DAT_001cfe94 = CONCAT31(DAT_001cfe94_1,1);
        cVar1 = ExclusiveMonitorsStatus();
      }
    } while (cVar1 != '\0');
    if (cVar3 == '\0') {
      FUN_0015525c(2);
      _DAT_001cfe94 = 0;
    }
  }
  return;
}

/* ===== FUN_00155ebc @ 00155ebc [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00155ebc(void)

{
  DAT_00214cfc = 0;
  DAT_00214d28 = 0;
  DAT_00214d00 = 0;
  _DAT_00214d10 = 0;
  DAT_00214d08 = 0;
  DAT_00214d20 = 0;
  DAT_00214d18 = 0;
  _DAT_00214d38 = 0;
  DAT_00214d30 = 0;
  _DAT_00214d48 = 0;
  _DAT_00214d40 = 0;
  _DAT_00214d58 = 0;
  _DAT_00214d50 = 0;
  _DAT_00214d68 = 0;
  _DAT_00214d60 = 0;
  _DAT_00214d78 = 0;
  _DAT_00214d70 = 0;
  DAT_00214d88 = 0;
  _DAT_00214d80 = 0;
  if (DAT_00214cf8 == 2) {
    DAT_00214cf8 = 0;
  }
  return;
}

/* ===== FUN_00155f28 @ 00155f28 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00155f28(void)

{
  _DAT_00214de0 = 0;
  DAT_00214dc8 = 0;
  DAT_00214dc0 = 0;
  DAT_00214dd8 = 0;
  DAT_00214dd0 = 0;
  DAT_00214db8 = 0;
  DAT_00214db0 = 0;
  return;
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

/* ===== FUN_00156a78 @ 00156a78 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulong FUN_00156a78(undefined8 param_1,undefined4 param_2,undefined8 param_3)

{
  int iVar1;
  ulong uVar2;
  
  uVar2 = FUN_00155c04(param_1,param_1);
  if ((int)uVar2 != 0) {
    _DAT_00214d10 = CONCAT44(DAT_00214b24,0x3f);
    DAT_00214d18 = DAT_00214b38;
    DAT_00214d20 = DAT_00214a68;
    DAT_00214d28 = (ulong)DAT_00214ab8;
    DAT_00214d08 = DAT_0010e668;
    DAT_00214cfc = 1;
    DAT_00214d00 = param_2;
    iVar1 = (*DAT_00214d98)(0x1d,param_2,param_3);
    uVar2 = 0;
    if (iVar1 == 1) {
      iVar1 = (*DAT_00214da0)(param_3);
      uVar2 = (ulong)(iVar1 == 1);
    }
    DAT_00214cfc = 0;
    DAT_00214d00 = 0;
    DAT_00214d28 = 0;
    _DAT_00214d10 = 0;
    DAT_00214d08 = 0;
    DAT_00214d20 = 0;
    DAT_00214d18 = 0;
  }
  return uVar2;
}

/* ===== FUN_00156ba0 @ 00156ba0 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00156ba0(undefined8 *param_1)

{
  long lVar1;
  int iVar2;
  ulong uVar3;
  undefined8 local_a8;
  undefined8 local_a0;
  undefined8 uStack_98;
  undefined8 local_90;
  undefined8 uStack_88;
  undefined8 uStack_80;
  undefined8 uStack_78;
  undefined8 local_70;
  undefined8 uStack_68;
  undefined8 uStack_60;
  undefined8 uStack_58;
  undefined8 local_50;
  long local_48;
  
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  local_50 = 0;
  uStack_88 = 0;
  local_90 = 0;
  uStack_78 = 0;
  uStack_80 = 0;
  uStack_68 = 0;
  local_70 = 0;
  uStack_58 = 0;
  uStack_60 = 0;
  uStack_98 = 0;
  local_a0 = 0;
  local_a8 = DAT_0010e720;
  uVar3 = FUN_00155c04(param_1,*param_1);
  if ((int)uVar3 != 0) {
    _DAT_00214d10 = CONCAT44(DAT_00214b24,0x3f);
    DAT_00214d18 = DAT_00214b38;
    DAT_00214d20 = DAT_00214a68;
    DAT_00214d28 = (ulong)DAT_00214ab8;
    DAT_00214d08 = DAT_0010e668;
    DAT_00214d00 = 3;
    DAT_00214cfc = 1;
    iVar2 = (*DAT_00214d98)(0x1d,3,&local_a8);
    if (iVar2 == 1) {
      iVar2 = (*DAT_00214da0)(&local_a8);
      uVar3 = 0;
      DAT_00214cfc = 0;
      DAT_00214d00 = 0;
      _DAT_00214d10 = 0;
      DAT_00214d08 = 0;
      DAT_00214d20 = 0;
      DAT_00214d18 = 0;
      DAT_00214d28 = 0;
      if (iVar2 == 1) {
        iVar2 = memcmp(&local_a8,param_1 + 1,0x60);
        uVar3 = (ulong)(iVar2 == 0);
      }
    }
    else {
      uVar3 = 0;
      DAT_00214cfc = 0;
      DAT_00214d00 = 0;
      DAT_00214d28 = 0;
      _DAT_00214d10 = 0;
      DAT_00214d08 = 0;
      DAT_00214d20 = 0;
      DAT_00214d18 = 0;
    }
  }
  if (*(long *)(lVar1 + 0x28) == local_48) {
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

/* ===== FUN_0015851c @ 0015851c [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0015851c(undefined8 param_1,long param_2)

{
  long lVar1;
  int iVar2;
  undefined4 uVar3;
  undefined8 uVar4;
  undefined4 uVar5;
  ulong uVar6;
  undefined8 local_210;
  undefined8 local_208;
  undefined8 uStack_200;
  undefined8 local_1f8;
  undefined8 uStack_1f0;
  undefined8 local_1e8;
  undefined8 uStack_1e0;
  undefined8 local_1d8;
  undefined8 uStack_1d0;
  undefined8 local_1c8;
  undefined8 uStack_1c0;
  undefined8 local_1b8;
  undefined8 local_1b0;
  undefined8 uStack_1a8;
  undefined8 uStack_1a0;
  undefined8 uStack_198;
  undefined8 local_190;
  undefined8 uStack_188;
  undefined8 local_180;
  undefined8 uStack_178;
  undefined8 uStack_170;
  undefined8 local_168;
  ulong local_160;
  ulong local_158;
  undefined8 uStack_150;
  int local_148;
  int local_140;
  int local_13c;
  int local_138;
  int local_134;
  long local_130;
  long local_128;
  long local_120;
  ulong local_118;
  ulong local_110;
  ulong local_108;
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
  
  local_100 = DAT_0010e7d0;
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  if (param_2 == 0) {
    uVar6 = 0;
  }
  else {
    uVar6 = (ulong)(*(int *)(param_2 + 8) == 2);
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
  (&DAT_00215c48)[uVar6] = (&DAT_00215c48)[uVar6] + 1;
  if (((param_2 != 0) && (iVar2 = FUN_00158310(param_1,param_2 + 0x20,param_2 + 200), iVar2 != 0))
     && (uVar4 = FUN_00158080(), (int)uVar4 != 0)) {
    local_140 = 0;
    uVar4 = FUN_001428fc(uVar4,DAT_001e0978 + 0x1307e20,&local_108,8);
    if ((((((((int)uVar4 != 0) && (0x11fff < local_108 + 0x2000)) &&
           (((local_108 & 7) == 0 &&
            ((uVar4 = FUN_001428fc(uVar4,local_108 + 0x50,&local_134,4), (int)uVar4 != 0 &&
             (local_134 == 5)))))) &&
          (uVar4 = FUN_001428fc(uVar4,local_108 + 0x48,&local_110,8), (int)uVar4 != 0)) &&
         (((((((0x11fff < local_110 + 0x2000 && ((local_110 & 7) == 0)) &&
              (local_110 == *(ulong *)(param_2 + 0x60))) &&
             ((iVar2 = FUN_001428fc(uVar4,*(long *)(param_2 + 0x58) + 0x918,&local_118,8),
              iVar2 != 0 && (local_118 - 0x10000 < 0xfffffffffffee000)))) && ((local_118 & 7) == 0))
           && ((local_118 == local_110 &&
               (iVar2 = FUN_0013a78c(local_118 + 0x28,&local_118), iVar2 != 0)))) &&
          ((local_118 == *(ulong *)(param_2 + 0x68) &&
           (((iVar2 = FUN_0013a78c(local_110 + 0x58,&local_130), iVar2 != 0 &&
             (local_130 == *(long *)(param_2 + 0x78))) &&
            (iVar2 = FUN_0013a78c(local_118 + 0x28,&local_120), iVar2 != 0)))))))) &&
        (((((local_120 == DAT_00215ce0 &&
            (uVar4 = FUN_0013a78c(local_120,&local_128), (int)uVar4 != 0)) &&
           (local_128 == DAT_00215ce8)) &&
          (((uVar4 = FUN_001428fc(uVar4,local_120 + 0xc,&local_13c,4), (int)uVar4 != 0 &&
            (local_13c == DAT_00215cf4)) &&
           ((iVar2 = FUN_001428fc(uVar4,local_118 + 0x100,&local_138,4), iVar2 != 0 &&
            (((local_138 == *(int *)(param_2 + 0x80) &&
              (uVar4 = FUN_00189c88(DAT_001e0978,*(undefined8 *)(param_2 + 0x70),FUN_001428fc,0,
                                    &local_210), (int)uVar4 != 0)) && (local_1e8._4_4_ != 0))))))))
         && ((int)uStack_200 == local_138)))) &&
       ((iVar2 = (int)uStack_200, *(int *)(param_2 + 0x30) != 5 ||
        (((*(ulong *)(param_2 + 0x70) < 0xfffffffffffffdb4 &&
          (iVar2 = FUN_001428fc(uVar4,*(ulong *)(param_2 + 0x70) + 0x24c,&local_140,4), iVar2 != 0))
         && (iVar2 = local_138, local_140 == *(int *)(param_2 + 0x34))))))) {
      uStack_170 = *(undefined8 *)(param_2 + 0x40);
      uStack_178 = *(undefined8 *)(param_2 + 0x38);
      local_168 = *(undefined8 *)(param_2 + 0x58);
      local_180 = DAT_0010e7f8;
      local_160 = local_110;
      uStack_188 = 0;
      local_190 = 0;
      uStack_150 = *(undefined8 *)(param_2 + 0x70);
      uStack_1a8 = 0;
      local_1b0 = 0;
      uStack_198 = 0;
      uStack_1a0 = 0;
      local_158 = local_118;
      local_1b8 = DAT_0010e7a8;
      local_148 = iVar2;
      if (((((DAT_0020adec == '\x01') &&
            (iVar2 = FUN_0018a8cc(&DAT_0020adf0,&local_180,&local_1b8), iVar2 == 1)) &&
           (((int)local_1b0 != 0 && ((local_1b0._4_4_ != 0 && ((int)uStack_188 == 0)))))) &&
          ((DAT_001cfe98 == (code *)0x0 || (iVar2 = (*DAT_001cfe98)(0,&local_180), iVar2 == 1)))) &&
         ((iVar2 = (*DAT_0020d160)(&local_100), iVar2 == 1 &&
          (iVar2 = FUN_0018dd4c(param_2,&DAT_00215aa0,&DAT_00215c00,&local_100,&local_180),
          iVar2 != 0)))) {
        uRam0000000000215c70 = uStack_178;
        _DAT_00215c68 = local_180;
        uRam0000000000215c80 = local_168;
        _DAT_00215c78 = uStack_170;
        uStack_1c0 = 0;
        local_1c8 = 0;
        uStack_1d0 = 0;
        local_1d8 = 0;
        DAT_00215c88 = local_160;
        uStack_1e0 = 0;
        local_1e8 = 0;
        uStack_1f0 = 0;
        local_1f8 = 0;
        uStack_200 = 0;
        local_208 = 0;
        uVar5 = 2;
        if (*(int *)(param_2 + 8) == 2) {
          uVar5 = 3;
        }
        DAT_00215c90 = 1;
        local_210 = DAT_0010e700;
        if (*(int *)(param_2 + 0x10) - 3U < 2) {
          uVar3 = *(undefined4 *)(param_2 + 0x14);
        }
        else if (*(int *)(param_2 + 0x10) == 2) {
          uVar3 = 0x1a;
        }
        else {
          uVar3 = 0x16;
        }
        iVar2 = (*DAT_00213040)(uVar3,uVar5,&local_210);
        if (iVar2 == 1) {
          iVar2 = (*DAT_00213048)(&local_210);
          DAT_00215c90 = 0;
          DAT_00215c88 = 0;
          uRam0000000000215c70 = 0;
          _DAT_00215c68 = 0;
          uRam0000000000215c80 = 0;
          _DAT_00215c78 = 0;
          if (iVar2 == 1) {
            if (*(int *)(param_2 + 8) != 1) {
              uVar4 = 1;
              goto LAB_00158a04;
            }
            uVar4 = FUN_00158cd4(param_2,&local_100);
            if ((int)uVar4 != 0) goto LAB_00158a04;
          }
        }
        else {
          DAT_00215c90 = 0;
          DAT_00215c88 = 0;
          uRam0000000000215c70 = 0;
          _DAT_00215c68 = 0;
          uRam0000000000215c80 = 0;
          _DAT_00215c78 = 0;
        }
      }
    }
  }
  uVar4 = 0;
  (&DAT_00215c58)[uVar6] = (&DAT_00215c58)[uVar6] + 1;
LAB_00158a04:
  if (*(long *)(lVar1 + 0x28) != local_48) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar4);
  }
  return;
}

/* ===== FUN_00159434 @ 00159434 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00159434(undefined8 param_1,long param_2)

{
  long lVar1;
  int iVar2;
  ulong uVar3;
  long lVar4;
  undefined1 auVar5 [16];
  undefined1 auVar6 [16];
  undefined8 local_128;
  undefined8 local_120;
  undefined8 uStack_118;
  undefined8 local_110;
  undefined8 uStack_108;
  undefined8 uStack_100;
  undefined8 uStack_f8;
  undefined8 local_f0;
  undefined8 uStack_e8;
  undefined8 uStack_e0;
  undefined8 uStack_d8;
  undefined8 local_d0;
  undefined8 local_c8;
  long local_c0;
  long lStack_b8;
  long local_b0;
  undefined8 uStack_a8;
  undefined8 local_a0;
  int iStack_98;
  undefined4 uStack_94;
  undefined4 uStack_90;
  int iStack_8c;
  int iStack_88;
  undefined4 uStack_84;
  undefined4 local_80;
  undefined4 uStack_7c;
  undefined8 uStack_78;
  undefined8 local_70;
  undefined8 local_68;
  undefined8 uStack_60;
  undefined8 local_58;
  undefined8 uStack_50;
  undefined8 uStack_48;
  undefined8 local_40;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  uVar3 = 0;
  local_70 = DAT_0010e7a8;
  uStack_50 = 0;
  local_58 = 0;
  local_40 = 0;
  uStack_48 = 0;
  uStack_60 = 0;
  local_68 = 0;
  uStack_78 = 0;
  local_80 = 0;
  uStack_7c = 0;
  iStack_88 = 0;
  uStack_84 = 0;
  uStack_90 = 0;
  iStack_8c = 0;
  iStack_98 = 0;
  uStack_94 = 0;
  local_a0 = 0;
  lStack_b8 = 0;
  local_c0 = 0;
  uStack_a8 = 0;
  local_b0 = 0;
  local_c8 = DAT_0010e700;
  if ((DAT_00216f04 == '\x01') && (DAT_00216f08 != '\0')) {
    iVar2 = FUN_0015acc4(param_2,&local_70);
    uVar3 = 0;
    if ((iVar2 != 0) && ((local_68._4_4_ != 0 && ((int)local_40 == 0)))) {
      iVar2 = (*DAT_00215d80)(&local_c8);
      uVar3 = 0;
      if (iVar2 == 1) {
        auVar6._12_4_ = local_80;
        auVar6._8_4_ = uStack_84;
        auVar6._4_4_ = uStack_90;
        auVar6._0_4_ = uStack_94;
        auVar5 = NEON_cmtst(auVar6,auVar6,4);
        auVar6 = NEON_cmeq(auVar6,0,2);
        if (((((auVar6 & (undefined1  [16])0x1) == (undefined1  [16])0x0 &&
              (auVar6 & (undefined1  [16])0x100000000) == (undefined1  [16])0x0) &&
             (auVar5 & (undefined1  [16])0x1) == (undefined1  [16])0x0) &&
             (auVar5 & (undefined1  [16])0x100000000) == (undefined1  [16])0x0) && (iStack_98 != 0))
        {
          if (((local_c0 == DAT_00217050) &&
              (((iStack_8c == *(int *)(*(long *)(param_2 + 0x18) + 0x18) &&
                (iStack_88 == *(int *)(*(long *)(param_2 + 0x18) + 0x1c))) &&
               (lStack_b8 == DAT_00209d00)))) && (local_b0 == DAT_00214c18)) {
            uVar3 = FUN_001391f0(1);
            if (uVar3 == 0) goto LAB_001595c8;
            if (*(ulong *)(*(long *)(param_2 + 8) + 0x28) <= uVar3) {
              lVar4 = *(long *)(*(long *)(param_2 + 8) + 0x10);
              DAT_002170d0 = *(undefined8 *)(lVar4 + 0x20);
              DAT_002170b8 = DAT_0010e668;
              DAT_002170c8 = *(undefined8 *)(*(long *)(param_2 + 0x10) + 0x20);
              _DAT_002170c0 = CONCAT44(*(undefined4 *)(*(long *)(param_2 + 0x10) + 0xc),0x3f);
              DAT_002170d8 = (ulong)*(uint *)(lVar4 + 0x70);
              local_d0 = 0;
              uStack_108 = 0;
              local_110 = 0;
              uStack_f8 = 0;
              uStack_100 = 0;
              uStack_e8 = 0;
              local_f0 = 0;
              uStack_d8 = 0;
              uStack_e0 = 0;
              uStack_118 = 0;
              local_120 = 0;
              local_128 = DAT_0010e720;
              DAT_002170e0 = 1;
              iVar2 = (*DAT_00215d88)(0x19,1,&local_128);
              uVar3 = 0;
              if (iVar2 == 1) {
                iVar2 = (*DAT_00215d90)(&local_128);
                uVar3 = (ulong)(iVar2 == 1);
              }
              DAT_002170e0 = 0;
              DAT_002170d8 = 0;
              _DAT_002170c0 = 0;
              DAT_002170b8 = 0;
              DAT_002170d0 = 0;
              DAT_002170c8 = 0;
              goto LAB_001595c8;
            }
          }
          uVar3 = 0;
        }
      }
    }
  }
LAB_001595c8:
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar3);
}

/* ===== FUN_0015981c @ 0015981c [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0015981c(undefined8 param_1,long param_2,int param_3)

{
  long lVar1;
  int iVar2;
  int iVar3;
  ulong uVar4;
  undefined4 uVar5;
  undefined1 auVar6 [16];
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
  undefined8 local_a0;
  long local_98;
  long lStack_90;
  long local_88;
  undefined8 uStack_80;
  undefined8 local_78;
  int iStack_70;
  undefined4 uStack_6c;
  undefined4 uStack_68;
  int iStack_64;
  undefined4 uStack_60;
  undefined4 uStack_5c;
  undefined4 local_58;
  int iStack_54;
  undefined8 uStack_50;
  long local_48;
  
  iVar3 = DAT_00209cd8;
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  uVar4 = 0;
  uStack_50 = 0;
  local_58 = 0;
  iStack_54 = 0;
  uStack_60 = 0;
  uStack_5c = 0;
  uStack_68 = 0;
  iStack_64 = 0;
  iStack_70 = 0;
  uStack_6c = 0;
  local_78 = 0;
  uStack_80 = 0;
  local_88 = 0;
  lStack_90 = 0;
  local_98 = 0;
  local_a0 = DAT_0010e700;
  if (((DAT_002170f8 != '\x01') || (DAT_00216f04 == '\0')) || (DAT_00216f08 == '\0'))
  goto LAB_00159980;
  if (DAT_00209cd8 != 0) {
    iVar2 = gettid(0);
    if (iVar3 == iVar2) {
      iVar3 = FUN_00158080();
      uVar4 = 0;
      if ((param_2 == 0) || (iVar3 == 0)) goto LAB_00159980;
      if ((((*(long *)(param_2 + 0x18) == DAT_00217118) &&
           ((*(long *)(param_2 + 0x20) == DAT_00217120 &&
            (*(long *)(param_2 + 0x28) == DAT_00217128)))) &&
          (((DAT_00217138 ^ *(ulong *)(param_2 + 0x38)) & 0xffffffffffffff) == 0)) &&
         ((((((DAT_00217140 ^ *(ulong *)(param_2 + 0x40)) & 0xffffffffffffff) == 0 &&
            (((DAT_00217148 ^ *(ulong *)(param_2 + 0x48)) & 0xffffffffffffff) == 0)) &&
           (((DAT_00217150 ^ *(ulong *)(param_2 + 0x50)) & 0xffffffffffffff) == 0)) &&
          (((DAT_00217158 ^ *(ulong *)(param_2 + 0x58)) & 0xffffffffffffff) == 0)))) {
        uVar4 = 0;
        if ((*(int *)(param_2 + 0x60) != DAT_00217160) ||
           (DAT_0020f650 != *(long *)(param_2 + 0x18))) goto LAB_00159980;
        if (DAT_0020f638._4_4_ == *(int *)(param_2 + 0xc)) {
          uVar4 = FUN_0015b1c0(param_2);
          if ((int)uVar4 == 0) goto LAB_00159980;
          iVar3 = (*DAT_00215d80)(&local_a0);
          uVar4 = 0;
          if (iVar3 != 1) goto LAB_00159980;
          auVar6._4_4_ = uStack_68;
          auVar6._0_4_ = uStack_6c;
          auVar6[8] = (char)uStack_5c;
          auVar6[9] = (char)((uint)uStack_5c >> 8);
          auVar6[10] = (char)((uint)uStack_5c >> 0x10);
          auVar6[0xb] = (char)((uint)uStack_5c >> 0x18);
          auVar6[0xc] = (char)local_58;
          auVar6[0xd] = (char)((uint)local_58 >> 8);
          auVar6[0xe] = (char)((uint)local_58 >> 0x10);
          auVar6[0xf] = (char)((uint)local_58 >> 0x18);
          auVar6 = NEON_cmeq(auVar6,_DAT_00112a90,4);
          if (((((auVar6 & (undefined1  [16])0x1) != (undefined1  [16])0x0 ||
                (auVar6 & (undefined1  [16])0x100000000) != (undefined1  [16])0x0) ||
               (~auVar6[8] & 1) != 0) || (~auVar6[0xc] & 1) != 0) || (iStack_70 == 0))
          goto LAB_00159980;
          if (((local_98 == *(long *)(param_2 + 0x28)) &&
              (((iStack_64 == (int)DAT_00217084 && (iStack_54 == DAT_00217094)) &&
               ((int)uStack_50 == (int)DAT_00217098)))) &&
             ((lStack_90 == DAT_00209d00 && (local_88 == DAT_00214c18)))) {
            DAT_002170c8 = *(undefined8 *)(param_2 + 0x18);
            DAT_002170d0 = *(undefined8 *)(param_2 + 0x20);
            _DAT_002170c0 = CONCAT44(*(undefined4 *)(param_2 + 0xc),0xff);
            DAT_002170d8 = (ulong)*(uint *)(param_2 + 0x60);
            DAT_002170e0 = 1;
            DAT_002170b8 = DAT_0010e668;
            local_a8 = 0;
            uVar5 = 2;
            if (param_3 == 2) {
              uVar5 = 3;
            }
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
            local_100 = DAT_0010e720;
            iVar3 = (*DAT_00215d88)(0x19,uVar5,&local_100);
            uVar4 = 0;
            if (iVar3 == 1) {
              iVar3 = (*DAT_00215d90)(&local_100);
              uVar4 = (ulong)(iVar3 == 1);
            }
            DAT_002170e0 = 0;
            DAT_002170d8 = 0;
            _DAT_002170c0 = 0;
            DAT_002170b8 = 0;
            DAT_002170d0 = 0;
            DAT_002170c8 = 0;
            goto LAB_00159980;
          }
        }
      }
    }
  }
  uVar4 = 0;
LAB_00159980:
  if (*(long *)(lVar1 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar4);
}

/* ===== FUN_0015a3f0 @ 0015a3f0 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0015a3f0(long param_1)

{
  ulong uVar1;
  uint uVar2;
  uint uVar3;
  undefined4 uVar4;
  long lVar5;
  undefined8 uVar6;
  bool bVar7;
  int iVar8;
  int iVar9;
  undefined4 *puVar10;
  undefined8 local_280;
  undefined8 local_278;
  undefined8 uStack_270;
  undefined8 local_268;
  undefined8 uStack_260;
  undefined8 local_258;
  undefined8 uStack_250;
  undefined8 local_248;
  undefined8 *local_240;
  undefined8 *puStack_238;
  undefined8 *local_230;
  undefined8 *puStack_228;
  undefined8 local_220;
  undefined8 *local_218;
  timespec *ptStack_210;
  undefined8 local_208;
  undefined8 local_200;
  ulong local_1f8;
  undefined8 local_1f0;
  int local_1e8;
  undefined4 local_1e4;
  undefined8 local_1e0;
  undefined8 uStack_1d8;
  undefined8 local_1d0;
  ulong local_1c8;
  undefined8 local_1c0;
  undefined4 local_1b8;
  undefined4 uStack_1b4;
  undefined8 local_1b0;
  undefined8 local_1a8;
  undefined8 local_1a0;
  undefined8 uStack_198;
  undefined8 uStack_190;
  undefined8 uStack_188;
  long local_180;
  undefined8 uStack_178;
  undefined8 local_170;
  undefined8 local_160;
  undefined8 uStack_158;
  undefined8 uStack_150;
  ulong local_148;
  undefined8 local_140;
  timespec local_130;
  undefined8 local_120;
  undefined8 uStack_118;
  undefined8 local_110;
  undefined8 uStack_108;
  undefined8 local_100;
  undefined8 uStack_f8;
  undefined8 local_f0;
  undefined8 uStack_e8;
  undefined8 local_e0;
  undefined8 uStack_d8;
  undefined8 local_d0;
  undefined8 uStack_c8;
  undefined8 local_c0;
  undefined8 uStack_b8;
  undefined8 local_b0;
  undefined8 uStack_a8;
  undefined8 local_a0;
  undefined8 uStack_98;
  undefined8 local_90;
  undefined8 uStack_88;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined8 local_70;
  undefined8 uStack_68;
  long local_58;
  
  lVar5 = tpidr_el0;
  local_58 = *(long *)(lVar5 + 0x28);
  puVar10 = (undefined4 *)__errno();
  iVar9 = DAT_00209cd8;
  uVar4 = *puVar10;
  if ((DAT_00216f04 == '\x01') && ((int)DAT_001dfff0 != 0)) {
    if (DAT_00209cd8 == 0) {
      bVar7 = true;
    }
    else {
      iVar8 = gettid();
      bVar7 = iVar9 != iVar8;
    }
    if ((((((param_1 != 0) && (!bVar7)) && ((DAT_002170e4 & 1) == 0)) &&
         ((DAT_00216f10 != 0 && (DAT_00217090 == 0)))) &&
        ((((DAT_00216f30 ^ *(ulong *)(param_1 + 0x98)) & 0xffffffffffffff) == 0 &&
         ((DAT_00217030 == DAT_0020f650 &&
          (iVar9 = clock_gettime(1,&local_130), uVar6 = DAT_0010e580, iVar9 == 0)))))) &&
       ((uVar1 = local_130.tv_sec * 1000 + (ulong)local_130.tv_nsec / 1000000, uVar1 != 0 &&
        (DAT_002170b0 <= uVar1)))) {
      uStack_158 = DAT_00216f20;
      local_160 = _DAT_00216f18;
      local_148 = DAT_00216f30;
      uStack_150 = DAT_00216f28;
      uStack_78 = uRam0000000000216ff8;
      local_80 = _DAT_00216ff0;
      uStack_68 = uRam0000000000217008;
      local_70 = _DAT_00217000;
      uStack_c8 = uRam0000000000216fa8;
      local_d0 = _DAT_00216fa0;
      uStack_a8 = uRam0000000000216fc8;
      local_b0 = _DAT_00216fc0;
      uStack_98 = uRam0000000000216fd8;
      local_a0 = _DAT_00216fd0;
      uStack_108 = uRam0000000000216f68;
      local_110 = DAT_00216f60;
      uStack_e8 = uRam0000000000216f88;
      local_f0 = _DAT_00216f80;
      uStack_d8 = uRam0000000000216f98;
      local_e0 = _DAT_00216f90;
      uStack_88 = uRam0000000000216fe8;
      local_90 = _DAT_00216fe0;
      uStack_118 = uRam0000000000216f58;
      local_120 = _DAT_00216f50;
      local_130.tv_nsec = uRam0000000000216f48;
      local_130.tv_sec = _DAT_00216f40;
      uStack_b8 = uRam0000000000216fb8;
      local_c0 = _DAT_00216fb0;
      uStack_178 = uRam0000000000217038;
      local_180 = DAT_00217030;
      local_1c0 = DAT_0010e520;
      uStack_f8 = DAT_00216f78;
      local_100 = _DAT_00216f70;
      local_140 = DAT_00216f38;
      local_1a8 = DAT_00217084;
      local_1b8 = DAT_00217080;
      uStack_1b4 = 0;
      uStack_198 = uRam0000000000217018;
      local_1a0 = _DAT_00217010;
      uStack_188 = uRam0000000000217028;
      uStack_190 = _DAT_00217020;
      local_170 = DAT_00217040;
      local_1b0 = DAT_00217050;
      local_1f0 = DAT_0010e580;
      local_1e8 = DAT_00209cd8;
      local_1e4 = gettid();
      local_218 = &local_160;
      local_220 = uVar6;
      uStack_1d8 = _UNK_00112a98;
      local_1e0 = _DAT_00112a90;
      local_1c8 = local_148;
      ptStack_210 = &local_130;
      local_208 = 0;
      local_1d0 = DAT_0010e748;
      local_240 = &local_220;
      local_200 = DAT_0010e588;
      DAT_00217200 = DAT_00217200 + 1;
      uStack_250 = 0;
      local_258 = 0;
      puStack_238 = &local_1a0;
      uStack_260 = 0;
      local_268 = 0;
      local_230 = &local_1c0;
      puStack_228 = &local_1f0;
      local_248 = DAT_0010e668;
      uStack_270 = 0;
      local_278 = 0;
      DAT_002170e4 = 1;
      local_280 = DAT_0010e7a8;
      local_1f8 = uVar1;
      iVar9 = FUN_0015acc4(&local_248,&local_280);
      uVar2 = 0;
      if (local_278._4_4_ != 0) {
        uVar2 = (uint)(iVar9 != 0);
      }
      uVar3 = 0;
      if ((int)uStack_250 != 0) {
        uVar3 = (uint)(iVar9 != 0);
      }
      local_1e0 = CONCAT44(uVar2,iVar9);
      uStack_1d8 = CONCAT44(uStack_1d8._4_4_,uVar3);
      FUN_0015b524(local_148);
      FUN_001bbda8(&DAT_00215de0,&local_248,&DAT_00217208);
      DAT_002170e4 = 0;
      DAT_002170e0 = 0;
      DAT_002170e8 = 0;
      DAT_002170f0 = 0;
    }
  }
  *puVar10 = uVar4;
  if (*(long *)(lVar5 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0015cfd8 @ 0015cfd8 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0015cfd8(undefined8 *param_1)

{
  long lVar1;
  int iVar2;
  int iVar3;
  undefined8 uVar4;
  undefined8 *puVar5;
  ulong uVar6;
  int *piVar7;
  undefined8 local_148;
  int local_140;
  int iStack_13c;
  undefined8 local_138;
  undefined8 local_130;
  undefined8 uStack_128;
  undefined8 local_120;
  undefined8 uStack_118;
  undefined *local_110;
  undefined *puStack_108;
  undefined *local_100;
  undefined8 *puStack_f8;
  undefined8 *local_f0;
  undefined *puStack_e8;
  undefined8 local_e0;
  undefined8 uStack_d8;
  undefined8 local_d0;
  undefined8 uStack_c8;
  undefined8 local_c0;
  undefined8 uStack_b8;
  undefined8 local_b0;
  undefined8 uStack_a8;
  undefined8 uStack_a0;
  undefined8 uStack_98;
  undefined8 local_90;
  undefined8 uStack_88;
  undefined8 uStack_80;
  undefined8 uStack_78;
  undefined8 local_70;
  long local_68;
  
  lVar1 = tpidr_el0;
  local_68 = *(long *)(lVar1 + 0x28);
  if ((int)DAT_00214938 == 1) {
    local_70 = 0;
    uStack_a8 = 0;
    local_b0 = 0;
    uStack_98 = 0;
    uStack_a0 = 0;
    uStack_88 = 0;
    local_90 = 0;
    uStack_78 = 0;
    uStack_80 = 0;
    uStack_b8 = 0;
    local_c0 = 0;
    uStack_d8 = 0;
    local_e0 = 0;
    uStack_c8 = 0;
    local_d0 = 0;
    DAT_0021ca28 = (undefined *)0x0;
    _DAT_0021ca20 = 0;
    DAT_0021ca38 = 0;
    DAT_0021ca30 = (undefined *)0x0;
    DAT_0021ca48 = 0;
    _DAT_0021ca40 = 0;
    memset(&DAT_0021ca50,0,0x1ce0);
    iVar2 = DAT_00209cd8;
    if ((((param_1 != (undefined8 *)0x0) && (*(int *)((long)param_1 + 0x24) != 0)) &&
        (param_1[1] != 0)) &&
       (((param_1[2] != 0 && (DAT_00209cd8 != 0)) &&
        ((iVar3 = gettid(), iVar2 == iVar3 && (uVar4 = FUN_00164c78(&local_e0), (int)uVar4 != 0)))))
       ) {
      DAT_0021ca48 = param_1[5];
      _DAT_0021ca20 = *param_1;
      DAT_0021e860 = 1;
      puVar5 = (undefined8 *)param_1[1];
      uRam000000000021e738 = puVar5[1];
      _DAT_0021e730 = *puVar5;
      uRam000000000021e748 = puVar5[3];
      _DAT_0021e740 = puVar5[2];
      DAT_0021e750 = puVar5[4];
      puVar5 = (undefined8 *)param_1[2];
      uRam000000000021e7f0 = puVar5[0x13];
      _DAT_0021e7e8 = puVar5[0x12];
      uRam000000000021e800 = puVar5[0x15];
      _DAT_0021e7f8 = puVar5[0x14];
      uRam000000000021e810 = puVar5[0x17];
      _DAT_0021e808 = puVar5[0x16];
      uRam000000000021e820 = puVar5[0x19];
      _DAT_0021e818 = puVar5[0x18];
      DAT_0021e7b0 = puVar5[0xb];
      _DAT_0021e7a8 = puVar5[10];
      uRam000000000021e7c0 = puVar5[0xd];
      _DAT_0021e7b8 = puVar5[0xc];
      uRam000000000021e7d0 = puVar5[0xf];
      _DAT_0021e7c8 = puVar5[0xe];
      uRam000000000021e7e0 = puVar5[0x11];
      _DAT_0021e7d8 = puVar5[0x10];
      uRam000000000021e770 = puVar5[3];
      _DAT_0021e768 = puVar5[2];
      DAT_0021e780 = puVar5[5];
      _DAT_0021e778 = puVar5[4];
      uRam000000000021e790 = puVar5[7];
      DAT_0021e788 = puVar5[6];
      uRam000000000021e7a0 = puVar5[9];
      DAT_0021e798 = puVar5[8];
      uRam000000000021e760 = puVar5[1];
      _DAT_0021e758 = *puVar5;
      uRam000000000021e830 = DAT_0020f638;
      _DAT_0021e828 = _DAT_0020f630;
      uRam000000000021e840 = _DAT_0020f648;
      _DAT_0021e838 = _DAT_0020f640;
      uRam000000000021e850 = _DAT_0020f658;
      _DAT_0021e848 = DAT_0020f650;
      DAT_0021ca28 = &DAT_0021e730;
      DAT_0021ca30 = &DAT_0021e758;
      DAT_0021e858 = DAT_0020f660;
      DAT_0021ca38 = 0;
      _DAT_0021ca40 = param_1[4] & 0xffffffff00000000;
      iVar3 = FUN_00164dfc(uVar4,param_1,&DAT_0021ca50);
      DAT_0021e860 = 0;
      if (iVar3 != 0) {
        uVar6 = (ulong)DAT_0021e628;
        DAT_0021e698 = 1;
        if (DAT_0021e628 != 0) {
          piVar7 = &DAT_0021cb28;
          do {
            if (*piVar7 == DAT_0021e898) {
              uRam000000000021e6a8 = *(undefined8 *)(piVar7 + 2);
              _DAT_0021e6a0 = *(undefined8 *)piVar7;
              uRam000000000021e6d8 = *(undefined8 *)(piVar7 + 0xe);
              _DAT_0021e6d0 = *(undefined8 *)(piVar7 + 0xc);
              uRam000000000021e6e8 = *(undefined8 *)(piVar7 + 0x12);
              _DAT_0021e6e0 = *(undefined8 *)(piVar7 + 0x10);
              uRam000000000021e6b8 = *(undefined8 *)(piVar7 + 6);
              _DAT_0021e6b0 = *(undefined8 *)(piVar7 + 4);
              uRam000000000021e6c8 = *(undefined8 *)(piVar7 + 10);
              _DAT_0021e6c0 = *(undefined8 *)(piVar7 + 8);
              uRam000000000021e718 = *(undefined8 *)(piVar7 + 0x1e);
              _DAT_0021e710 = *(undefined8 *)(piVar7 + 0x1c);
              uRam000000000021e728 = *(undefined8 *)(piVar7 + 0x22);
              _DAT_0021e720 = *(undefined8 *)(piVar7 + 0x20);
              uRam000000000021e6f8 = *(undefined8 *)(piVar7 + 0x16);
              _DAT_0021e6f0 = *(undefined8 *)(piVar7 + 0x14);
              uRam000000000021e708 = *(undefined8 *)(piVar7 + 0x1a);
              _DAT_0021e700 = *(undefined8 *)(piVar7 + 0x18);
            }
            piVar7 = piVar7 + 0x24;
            uVar6 = uVar6 - 1;
          } while (uVar6 != 0);
        }
        if ((int)local_d0 != 0) {
          FUN_00161098(*(undefined8 *)(param_1[2] + 0x38),*(undefined8 *)(param_1[2] + 0x50),
                       DAT_0021e834,1);
        }
        local_140 = DAT_00209cd8;
        iStack_13c = iVar2;
        local_148 = DAT_0010e7a0;
        local_130 = 0;
        uStack_128 = DAT_00220790;
        puStack_f8 = &local_e0;
        local_110 = &DAT_0021ca20;
        puStack_108 = &DAT_0021e828;
        local_138 = DAT_0010e740;
        local_100 = &DAT_0021ca50;
        local_120 = DAT_00220798;
        uStack_118 = DAT_0021ca48;
        local_f0 = &DAT_00214e50;
        puStack_e8 = &DAT_001cfbc8;
        FUN_0019fd78(DAT_0021e8d0,&local_148,&DAT_0021e868);
        goto LAB_0015d1c0;
      }
    }
    FUN_0019fd78(DAT_0021e8d0,0,&DAT_0021e868);
    DAT_00214e58 = 0;
    DAT_00214e50 = 0;
    _DAT_00214e68 = 0;
    DAT_00214e60 = 0;
  }
LAB_0015d1c0:
  if (*(long *)(lVar1 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0015d3c4 @ 0015d3c4 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0015d3c4(long param_1)

{
  long lVar1;
  int iVar2;
  int iVar3;
  undefined1 auVar4 [16];
  undefined8 local_220;
  undefined8 uStack_218;
  undefined *puStack_210;
  undefined *puStack_208;
  undefined8 local_200;
  undefined8 uStack_1f8;
  undefined *local_1f0;
  char acStack_1e0 [320];
  undefined8 local_a0;
  undefined8 uStack_98;
  undefined8 local_90;
  long local_88;
  undefined8 local_80;
  long lStack_78;
  undefined8 local_70;
  undefined8 uStack_68;
  undefined8 local_60;
  undefined8 uStack_58;
  ulong uStack_50;
  ulong uStack_48;
  long local_38;
  
  iVar3 = DAT_00209cd8;
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  if ((int)_DAT_002208a0 == 1) {
    uStack_98 = 0;
    local_a0 = 0;
    local_88 = 0;
    local_90 = 0;
    lStack_78 = 0;
    local_80 = 0;
    uStack_68 = 0;
    local_70 = 0;
    uStack_58 = 0;
    local_60 = 0;
    uStack_48 = 0;
    uStack_50 = 0;
    if (((((((param_1 == 0) || (*(int *)(param_1 + 0x24) == 0)) || (DAT_00209cd8 == 0)) ||
          ((iVar2 = gettid(), iVar3 != iVar2 || (DAT_0020f6cc == 0)))) ||
         (((int)DAT_0020f638 == 0 ||
          ((DAT_0020f6f0 != *(long *)(*(long *)(param_1 + 0x10) + 0x18) ||
           (DAT_0020f6f8 != *(long *)(*(long *)(param_1 + 0x10) + 0x20))))))) ||
        (DAT_0020f668 != *(long *)(param_1 + 0x28))) ||
       ((iVar3 = FUN_001661a0(&local_a0), iVar3 == 0 ||
        (iVar3 = FUN_0013a78c(DAT_0020f708 + 0x58,&local_60), iVar3 == 0)))) {
      FUN_00199410(&DAT_00220808,0,0);
    }
    else {
      auVar4._8_8_ = DAT_0020f6f8;
      auVar4._0_8_ = DAT_0020f6f0;
      auVar4 = NEON_ext(auVar4,auVar4,8,1);
      local_88 = *(long *)(param_1 + 0x28);
      uStack_68 = DAT_0020f680;
      local_70 = DAT_0020f678;
      lStack_78 = DAT_0020f708;
      uStack_98 = auVar4._8_8_;
      local_a0 = auVar4._0_8_;
      local_80 = DAT_0020f670;
      uStack_58 = CONCAT44(uStack_58._4_4_,DAT_0020f690);
      local_1f0 = PTR_FUN_001c4318;
      uStack_218 = _UNK_001c42f0;
      local_220 = _DAT_001c42e8;
      puStack_208 = PTR_FUN_001c4300;
      puStack_210 = PTR_FUN_001c42f8;
      uStack_1f8 = SUB168(_PTR_FUN_001c4308,8);
      local_200 = SUB168(_PTR_FUN_001c4308,0);
      iVar3 = FUN_00199410(&DAT_00220808,&local_a0,&local_220);
      if ((iVar3 != 0) && ((DAT_00220898 == 0 || (999 < (ulong)(local_88 - DAT_00220898))))) {
        snprintf(acStack_1e0,0x140,
                 ",\"pin_queued\":%llu,\"spray_queued\":%llu,\"types\":[9,15],\"interval_ms\":[%u,%u],\"epoch\":%llu,\"generation\":%llu,\"server_acceptance_proven\":false"
                 ,DAT_00220878,DAT_00220880,uStack_50 >> 0x20,uStack_48 & 0xffffffff,local_a0,
                 local_90);
        FUN_001417c8("periodic_command","game_owned_periodic_inputs",acStack_1e0);
        DAT_00220898 = local_88;
      }
    }
  }
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0015d5d4 @ 0015d5d4 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0015d5d4(long param_1)

{
  long lVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  long lVar5;
  undefined8 local_c30;
  undefined8 uStack_c28;
  undefined *puStack_c20;
  undefined *puStack_c18;
  timespec local_c10 [44];
  undefined8 local_950;
  undefined8 uStack_948;
  undefined8 uStack_940;
  undefined8 uStack_938;
  undefined8 local_930;
  undefined8 uStack_928;
  undefined8 uStack_920;
  undefined8 local_918;
  undefined8 local_910;
  undefined8 uStack_908;
  undefined8 uStack_900;
  undefined8 uStack_8f8;
  undefined8 local_8f0;
  undefined8 uStack_8e8;
  undefined8 uStack_8e0;
  undefined8 uStack_8d8;
  undefined1 auStack_8c8 [208];
  undefined8 local_7f8;
  uint local_7ac;
  long local_48;
  
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  if ((int)_DAT_00220784 == 1) {
    DAT_002208f8 = 0;
    DAT_002205c8 = 0;
    _DAT_002205c0 = 0;
    uRam00000000002205d8 = 0;
    _DAT_002205d0 = 0;
    uRam00000000002205e8 = 0;
    _DAT_002205e0 = 0;
    uRam00000000002205f8 = 0;
    _DAT_002205f0 = 0;
    uRam0000000000220608 = 0;
    _DAT_00220600 = 0;
    _DAT_00220618 = 0;
    _DAT_00220610 = 0;
    uRam0000000000220628 = 0;
    _DAT_00220620 = 0;
    DAT_00220630 = 0;
    uStack_c28 = _UNK_001c4348;
    local_c30 = _DAT_001c4340;
    puStack_c18 = PTR_FUN_001c4358;
    puStack_c20 = PTR_FUN_001c4350;
    uVar2 = FUN_00191d10(&DAT_00220638,param_1,&local_c30,auStack_8c8);
    uVar3 = nexus_visual_gl_publish_v1(auStack_8c8,&DAT_002205c0);
    if (uVar2 != 0 && uVar3 != 0) {
      DAT_00220900 = *(undefined8 *)(param_1 + 0x28);
      DAT_00220908 = DAT_002205c8;
      DAT_002208f8 = local_7f8;
    }
    nexus_visual_gl_features_v1(uVar2 != 0 && uVar3 != 0);
    iVar4 = clock_gettime(1,local_c10);
    if (iVar4 == 0) {
      lVar5 = local_c10[0].tv_sec * 1000 + (ulong)local_c10[0].tv_nsec / 1000000;
    }
    else {
      lVar5 = 0;
    }
    if ((DAT_002205e0 != 0 || DAT_0022061c != 0) &&
       ((DAT_00220910 == 0 || (1999 < (ulong)(lVar5 - DAT_00220910))))) {
      uStack_948 = 0;
      local_950 = 0;
      uStack_938 = 0;
      uStack_940 = 0;
      uStack_928 = 0;
      local_930 = 0;
      local_918 = 0;
      uStack_920 = 0;
      uStack_908 = 0;
      local_910 = 0;
      uStack_8f8 = 0;
      uStack_900 = 0;
      uStack_8e8 = 0;
      local_8f0 = 0;
      uStack_8d8 = 0;
      uStack_8e0 = 0;
      nexus_visual_gl_status_v1(&local_950);
      snprintf((char *)local_c10,700,
               ",\"prepared\":%d,\"published\":%d,\"markers\":%u,\"epoch\":%llu,\"generation\":%llu,\"render_reason\":%u,\"draw_passes\":%llu,\"rectangles\":%u,\"restored\":%u,\"swaps\":%llu"
               ,(ulong)uVar2,(ulong)uVar3,(ulong)local_7ac,local_7f8,DAT_002205c8,
               (undefined4)local_918,uStack_948,(undefined4)local_910,local_910._4_4_,local_950);
      FUN_001417c8("visual_frame","shared_ESP_packet",local_c10);
      DAT_00220910 = lVar5;
    }
  }
  if (*(long *)(lVar1 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0015d7dc @ 0015d7dc [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0015d7dc(long param_1)

{
  long lVar1;
  int iVar2;
  int iVar3;
  undefined8 *puVar4;
  undefined1 auStack_d0 [8];
  undefined8 local_c8;
  undefined4 local_bc;
  int local_a4;
  undefined8 local_90;
  undefined8 uStack_88;
  undefined8 uStack_80;
  undefined8 local_78;
  undefined8 uStack_70;
  undefined8 local_68;
  undefined8 uStack_60;
  undefined8 local_58;
  undefined8 uStack_50;
  undefined8 local_48;
  undefined8 uStack_40;
  long local_38;
  
  iVar3 = DAT_00209cd8;
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  DAT_002170b0 = 0;
  _DAT_00216f18 = 0;
  _DAT_00216f10 = 0;
  DAT_00216f28 = 0;
  DAT_00216f20 = 0;
  DAT_00216f38 = 0;
  DAT_00216f30 = 0;
  uRam0000000000216f48 = 0;
  _DAT_00216f40 = 0;
  uRam0000000000216f58 = 0;
  _DAT_00216f50 = 0;
  uRam0000000000216f68 = 0;
  DAT_00216f60 = 0;
  DAT_00216f78 = 0;
  _DAT_00216f70 = 0;
  uRam0000000000216f88 = 0;
  _DAT_00216f80 = 0;
  uRam0000000000216f98 = 0;
  _DAT_00216f90 = 0;
  uRam0000000000216fa8 = 0;
  _DAT_00216fa0 = 0;
  uRam0000000000216fb8 = 0;
  _DAT_00216fb0 = 0;
  uRam0000000000216fc8 = 0;
  _DAT_00216fc0 = 0;
  uRam0000000000216fd8 = 0;
  _DAT_00216fd0 = 0;
  uRam0000000000216fe8 = 0;
  _DAT_00216fe0 = 0;
  uRam0000000000216ff8 = 0;
  _DAT_00216ff0 = 0;
  uRam0000000000217008 = 0;
  _DAT_00217000 = 0;
  uRam0000000000217018 = 0;
  _DAT_00217010 = 0;
  uRam0000000000217028 = 0;
  _DAT_00217020 = 0;
  uRam0000000000217038 = 0;
  DAT_00217030 = 0;
  _DAT_00217048 = 0;
  DAT_00217040 = 0;
  _DAT_00217058 = 0;
  DAT_00217050 = 0;
  _DAT_00217068 = 0;
  _DAT_00217060 = 0;
  _DAT_00217078 = 0;
  _DAT_00217070 = 0;
  ram0x00217088 = 0;
  _DAT_00217080 = 0;
  DAT_00217098 = 0;
  _DAT_00217090 = 0;
  _DAT_002170a8 = 0;
  DAT_002170a0 = 0;
  if (((((DAT_00216f04 == '\x01') && (DAT_00209cd8 != 0)) && (iVar2 = gettid(), param_1 != 0)) &&
      (((iVar3 == iVar2 && (*(long *)(param_1 + 8) != 0)) &&
       ((*(long *)(param_1 + 0x10) != 0 && (*(int *)(param_1 + 0x24) != 0)))))) &&
     ((iVar3 = *(int *)(*(long *)(param_1 + 0x10) + 0x6c), iVar3 == 10 || (iVar3 == 0)))) {
    uStack_40 = 0;
    local_48 = 0;
    uStack_50 = 0;
    local_58 = 0;
    uStack_60 = 0;
    local_68 = 0;
    uStack_70 = 0;
    local_78 = 0;
    uStack_80 = 0;
    uStack_88 = 0;
    local_90 = DAT_0010e700;
    iVar3 = (*DAT_00215d80)(&local_90);
    if (((iVar3 == 1) &&
        ((((int)uStack_60 != 0 && (uStack_60._4_4_ != 0)) &&
         (iVar3 = FUN_00189c88(DAT_001e0978,*(undefined8 *)(*(long *)(param_1 + 8) + 0x18),
                               FUN_001428fc,0,auStack_d0), iVar3 != 0)))) && (local_a4 != 0)) {
      puVar4 = *(undefined8 **)(param_1 + 8);
      DAT_00216f20 = puVar4[1];
      _DAT_00216f18 = *puVar4;
      DAT_00216f30 = puVar4[3];
      DAT_00216f28 = puVar4[2];
      DAT_00216f38 = puVar4[4];
      puVar4 = *(undefined8 **)(param_1 + 0x10);
      DAT_00216f78 = puVar4[7];
      _DAT_00216f70 = puVar4[6];
      uRam0000000000216f88 = puVar4[9];
      _DAT_00216f80 = puVar4[8];
      uRam0000000000216f58 = puVar4[3];
      _DAT_00216f50 = puVar4[2];
      uRam0000000000216f68 = puVar4[5];
      DAT_00216f60 = puVar4[4];
      uRam0000000000216fb8 = puVar4[0xf];
      _DAT_00216fb0 = puVar4[0xe];
      uRam0000000000216fc8 = puVar4[0x11];
      _DAT_00216fc0 = puVar4[0x10];
      uRam0000000000216f98 = puVar4[0xb];
      _DAT_00216f90 = puVar4[10];
      uRam0000000000216fa8 = puVar4[0xd];
      _DAT_00216fa0 = puVar4[0xc];
      uRam0000000000216ff8 = puVar4[0x17];
      _DAT_00216ff0 = puVar4[0x16];
      uRam0000000000217008 = puVar4[0x19];
      _DAT_00217000 = puVar4[0x18];
      uRam0000000000216fd8 = puVar4[0x13];
      _DAT_00216fd0 = puVar4[0x12];
      uRam0000000000216fe8 = puVar4[0x15];
      _DAT_00216fe0 = puVar4[0x14];
      uRam0000000000216f48 = puVar4[1];
      _DAT_00216f40 = *puVar4;
      uRam0000000000217028 = _DAT_0020f648;
      _DAT_00217020 = _DAT_0020f640;
      uRam0000000000217038 = _DAT_0020f658;
      DAT_00217030 = DAT_0020f650;
      uRam0000000000217018 = DAT_0020f638;
      _DAT_00217010 = _DAT_0020f630;
      DAT_00217040 = DAT_0020f660;
      DAT_00217050 = uStack_88;
      _DAT_00217048 = local_90;
      _DAT_00217060 = local_78;
      _DAT_00217058 = uStack_80;
      _DAT_00217080 = local_58;
      _DAT_00217078 = uStack_60;
      _DAT_00217090 = local_48;
      ram0x00217088 = uStack_50;
      _DAT_00217070 = local_68;
      _DAT_00217068 = uStack_70;
      DAT_00217098 = uStack_40;
      DAT_002170a0 = local_c8;
      _DAT_002170a8 = CONCAT44(DAT_002170a8_4,local_bc);
      DAT_002170b0 = *(undefined8 *)(param_1 + 0x28);
      _DAT_00216f10 = CONCAT44(DAT_00216f14,1);
    }
  }
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0015efa8 @ 0015efa8 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0015efa8(long param_1)

{
  long lVar1;
  bool bVar2;
  int iVar3;
  ulong uVar4;
  undefined8 uVar5;
  uint uVar6;
  ulong uVar7;
  long lVar8;
  float fVar9;
  undefined4 uVar10;
  float fVar11;
  undefined8 local_268;
  uint local_260;
  char local_250;
  char local_24f;
  undefined8 local_240;
  long lStack_238;
  long local_230;
  long local_228;
  undefined8 local_220;
  undefined8 uStack_218;
  ulong local_210;
  ulong local_208;
  undefined8 local_200;
  undefined8 uStack_1f8;
  undefined8 local_1f0;
  undefined8 local_1e8;
  undefined1 auStack_1d8 [16];
  uint local_1c8;
  uint local_1b8;
  uint local_1b4;
  int local_1ac;
  float local_1a4;
  float fStack_1a0;
  undefined1 auStack_198 [48];
  timespec local_168;
  undefined4 uStack_158;
  undefined4 local_154;
  undefined4 local_150;
  undefined8 local_148;
  undefined8 uStack_140;
  undefined8 local_138;
  undefined8 uStack_130;
  long local_128;
  undefined4 local_120;
  undefined4 local_11c;
  long local_118;
  undefined8 uStack_110;
  long local_88;
  
  lVar1 = tpidr_el0;
  local_88 = *(long *)(lVar1 + 0x28);
  lStack_238 = 0;
  local_240 = 0;
  local_228 = 0;
  local_230 = 0;
  uStack_218 = 0;
  local_220 = 0;
  local_208 = 0;
  local_210 = 0;
  uStack_1f8 = 0;
  local_200 = 0;
  local_1e8 = 0;
  local_1f0 = 0;
  if (((param_1 == 0) || (*(int *)(param_1 + 0x24) == 0)) ||
     (iVar3 = FUN_00169084(&local_240), iVar3 == 0)) {
    if (*(long *)(lVar1 + 0x28) == local_88) {
      FUN_0019eb58(&DAT_00228b30,0);
      return;
    }
  }
  else {
    if ((DAT_0020f624 != 0) && (DAT_0020f5e8 == local_230)) {
      local_168.tv_nsec._4_4_ = 0;
      uStack_158 = 0;
      local_168.tv_sec._4_4_ = 0;
      local_168.tv_nsec._0_4_ = 0;
      local_168.tv_sec._0_4_ = 2;
      local_154 = DAT_002148a4;
      local_138 = DAT_0020f680;
      uStack_130 = DAT_0020f6f8;
      local_128 = DAT_0020f650;
      local_150 = 0;
      uStack_140 = DAT_0020f678;
      local_148 = DAT_0020f670;
      local_120 = DAT_0020f690;
      local_11c = DAT_0020f638._4_4_;
      local_118 = local_230;
      uStack_110 = 0;
      iVar3 = FUN_00185c68(&DAT_0020d168,&DAT_0020f628,&local_168,FUN_00150f7c,0,&local_268,
                           auStack_1d8);
      if ((((iVar3 == 1) &&
           ((iVar3 = FUN_00189c88(DAT_001e0978,local_268,FUN_001428fc,0,auStack_1d8), iVar3 != 0 &&
            (local_1ac != 0)))) && (local_1c8 == local_260)) &&
         (iVar3 = FUN_001bc828(DAT_0020f670,FUN_001428fc,0,&local_250), lVar8 = DAT_0020f650,
         iVar3 != 0)) {
        local_240 = DAT_0020f6f8;
        lStack_238 = DAT_0020f650;
        iVar3 = clock_gettime(1,&local_168);
        if (iVar3 == 0) {
          local_228 = CONCAT44(local_168.tv_sec._4_4_,(undefined4)local_168.tv_sec) * 1000 +
                      CONCAT44(local_168.tv_nsec._4_4_,(undefined4)local_168.tv_nsec) / 1000000;
        }
        else {
          local_228 = 0;
        }
        fVar9 = 1.0;
        fVar11 = 1.0;
        local_220 = DAT_0020f680;
        uStack_218 = CONCAT44(1,DAT_0020f690);
        local_210 = (ulong)CONCAT14((DAT_00214cf8 != 0 || (DAT_00214c10 != 0 || DAT_00214c14 != 0))
                                    || DAT_00214cf0 == lVar8,
                                    (uint)(local_250 != '\0' || local_24f != '\0'));
        if ((DAT_0020f6a4 != 0) && (DAT_0020f6a0 <= DAT_0020f6a4)) {
          fVar11 = (float)DAT_0020f6a0 / (float)DAT_0020f6a4;
        }
        if ((local_1b4 != 0) && (local_1b8 <= local_1b4)) {
          fVar9 = (float)local_1b8 / (float)local_1b4;
        }
        uStack_1f8 = CONCAT44(fVar9,fVar11);
        uVar10 = FUN_00165c60(&DAT_0020f680);
        local_1f0 = CONCAT44(local_1f0._4_4_,uVar10);
        uVar10 = FUN_00165c60(auStack_1d8);
        local_1f0 = CONCAT44(uVar10,(undefined4)local_1f0);
        fVar9 = hypotf(local_1a4 - DAT_0020f6b4,fStack_1a0 - DAT_0020f6b8);
        local_1e8 = CONCAT44(local_1e8._4_4_,fVar9);
        iVar3 = FUN_0019eb58(&DAT_00228b30,&local_240);
        if (iVar3 != 0) {
          lVar8 = 0;
          do {
            uVar7 = (ulong)*(uint *)((long)&DAT_00111870 + lVar8);
            iVar3 = FUN_0019ec98(&DAT_00228b30,&local_240,uVar7);
            if (iVar3 != 0) {
              iVar3 = FUN_0016922c(&local_240,auStack_1d8);
              uVar10 = DAT_0020d158;
              uVar5 = 0xffffffff;
              if ((((iVar3 != 0) && (DAT_00214cf8 == 0)) && (DAT_00214c10 == 0)) &&
                 (DAT_00214c14 == 0)) {
                if (lVar8 != 0) {
                  bVar2 = (int)lVar8 != 4;
                  lVar8 = 0x8580d4;
                  if (bVar2) {
                    lVar8 = 0x858894;
                  }
                  DAT_0020d158 = 1;
                  DAT_00214cf8 = 6;
                  _DAT_00214c10 = DAT_0010e740;
                  uVar6 = 8;
                  if (bVar2) {
                    uVar6 = 0x11;
                  }
                  (*(code *)(DAT_001e0978 + lVar8))();
                  DAT_0020d158 = uVar10;
LAB_0015f468:
                  DAT_00214cf8 = 0;
                  _DAT_00214c10 = 0;
                  DAT_00214cf0 = lStack_238;
                  snprintf((char *)&local_168,0xdc,
                           ",\"kind\":%u,\"farm\":%u,\"target_gid\":%u,\"input_type\":%u,\"server_acceptance_proven\":false"
                           ,uVar7,local_208 & 0xffffffff,(ulong)local_1c8,(ulong)uVar6);
                  FUN_001417c8("ability_command","original_game_command",&local_168);
                  FUN_0019ee44(&DAT_00228b30,&local_240,uVar7,1);
                  break;
                }
                uVar5 = (*(code *)(DAT_001e0978 + 0xe7cddc))(local_220);
                iVar3 = FUN_0018a244(DAT_001e0978,local_220,uVar5,FUN_001428fc,0,auStack_198);
                if ((iVar3 == 0) ||
                   (iVar3 = FUN_0016922c(&local_240,auStack_1d8), uVar10 = DAT_0020d158, iVar3 == 0)
                   ) {
                  uVar5 = 0xffffffff;
                }
                else {
                  uVar6 = 1;
                  DAT_0020d158 = 1;
                  _DAT_00214c10 = 0x100000001;
                  DAT_00214cf8 = 6;
                  uVar4 = (*(code *)(DAT_001e0978 + 0xb3d64c))
                                    (DAT_0020f670,local_220,uVar5,(int)(local_1a4 * 300.0),
                                     (int)(fStack_1a0 * 300.0),1,0x5f3f356b);
                  uVar5 = 0;
                  _DAT_00214c10 = 0;
                  DAT_00214cf8 = 0;
                  DAT_0020d158 = uVar10;
                  if ((uVar4 & 1) != 0) goto LAB_0015f468;
                }
              }
              FUN_0019ee44(&DAT_00228b30,&local_240,uVar7,uVar5);
            }
            lVar8 = lVar8 + 4;
          } while (lVar8 != 0xc);
        }
      }
    }
    if (*(long *)(lVar1 + 0x28) == local_88) {
      return;
    }
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00160b54 @ 00160b54 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00160b54(long param_1)

{
  int iVar1;
  long lVar2;
  undefined8 *puVar3;
  long lVar4;
  
  if (DAT_00214940 != '\x01') {
    return;
  }
  DAT_00214b50 = 0;
  if ((((((param_1 != 0) && (lVar2 = *(long *)(param_1 + 8), lVar2 != 0)) &&
        (lVar4 = *(long *)(param_1 + 0x10), lVar4 != 0)) &&
       ((*(int *)(param_1 + 0x24) != 0 && (*(int *)(lVar4 + 0x60) != 0)))) &&
      ((*(int *)(lVar4 + 0x6c) == 10 || (*(int *)(lVar4 + 0x6c) == 0)))) &&
     (((*(long *)(lVar2 + 8) == *(long *)(lVar4 + 0x18) &&
       (*(long *)(lVar2 + 0x10) == *(long *)(lVar4 + 0x28))) &&
      (*(long *)(lVar2 + 0x18) == *(long *)(lVar4 + 0x40))))) {
    iVar1 = FUN_00189c88(DAT_001e0978,*(long *)(lVar2 + 0x18),FUN_001428fc,0,&DAT_00214bd0);
    if (iVar1 != 0) {
      puVar3 = *(undefined8 **)(param_1 + 8);
      DAT_00214950 = puVar3[1];
      _DAT_00214948 = *puVar3;
      DAT_00214960 = puVar3[3];
      DAT_00214958 = puVar3[2];
      DAT_00214968 = puVar3[4];
      puVar3 = *(undefined8 **)(param_1 + 0x10);
      uRam0000000000214ae0 = puVar3[0x13];
      _DAT_00214ad8 = puVar3[0x12];
      uRam0000000000214af0 = puVar3[0x15];
      _DAT_00214ae8 = puVar3[0x14];
      uRam0000000000214b00 = puVar3[0x17];
      _DAT_00214af8 = puVar3[0x16];
      uRam0000000000214b10 = puVar3[0x19];
      _DAT_00214b08 = puVar3[0x18];
      uRam0000000000214aa0 = puVar3[0xb];
      _DAT_00214a98 = puVar3[10];
      uRam0000000000214ab0 = puVar3[0xd];
      _DAT_00214aa8 = puVar3[0xc];
      uRam0000000000214ac0 = puVar3[0xf];
      _DAT_00214ab8 = puVar3[0xe];
      uRam0000000000214ad0 = puVar3[0x11];
      _DAT_00214ac8 = puVar3[0x10];
      uRam0000000000214a60 = puVar3[3];
      _DAT_00214a58 = puVar3[2];
      uRam0000000000214a70 = puVar3[5];
      DAT_00214a68 = puVar3[4];
      DAT_00214a80 = puVar3[7];
      DAT_00214a78 = puVar3[6];
      uRam0000000000214a90 = puVar3[9];
      _DAT_00214a88 = puVar3[8];
      uRam0000000000214a50 = puVar3[1];
      _DAT_00214a48 = *puVar3;
      DAT_00214b48 = DAT_0020f660;
      uRam0000000000214b30 = _DAT_0020f648;
      _DAT_00214b28 = _DAT_0020f640;
      uRam0000000000214b40 = _DAT_0020f658;
      DAT_00214b38 = DAT_0020f650;
      DAT_00214970 = *(undefined8 *)(param_1 + 0x28);
      uRam0000000000214b20 = DAT_0020f638;
      _DAT_00214b18 = _DAT_0020f630;
      DAT_00214b50 = 1;
      goto LAB_00160cdc;
    }
    if ((DAT_00214b50 & 1) != 0) goto LAB_00160cdc;
  }
  DAT_00214b60 = 0;
  DAT_00214b68 = 0;
  DAT_00214db8 = 0;
  DAT_00214db0 = 0;
  DAT_00214dc8 = 0;
  DAT_00214dc0 = 0;
  DAT_00214dd8 = 0;
  DAT_00214dd0 = 0;
  _DAT_00214de0 = 0;
LAB_00160cdc:
  FUN_0015525c(1);
  return;
}

/* ===== FUN_00160f44 @ 00160f44 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00160f44(undefined8 param_1,int param_2)

{
  bool bVar1;
  int iVar2;
  char cVar3;
  char cVar4;
  
  if (param_2 == 0) {
    do {
      cVar4 = DAT_001cfe94;
      cVar3 = '\x01';
      bVar1 = (bool)ExclusiveMonitorPass(0x1cfe94,0x10);
      if (bVar1) {
        _DAT_001cfe94 = CONCAT31(DAT_001cfe94_1,1);
        cVar3 = ExclusiveMonitorsStatus();
      }
    } while (cVar3 != '\0');
    if (cVar4 != '\0') {
      return;
    }
  }
  iVar2 = DAT_00217324 + 1;
  bVar1 = DAT_00217324 < 8;
  DAT_00217324 = iVar2;
  if (bVar1) {
    FUN_001417c8("capture_request_rejected",param_1,0);
  }
  if (param_2 == 0) {
    _DAT_001cfe94 = 0;
  }
  return;
}

/* ===== FUN_001658b0 @ 001658b0 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_001658b0(long param_1)

{
  undefined8 uVar1;
  int iVar2;
  ulong uVar3;
  int *piVar4;
  int iVar5;
  int iVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  undefined8 uVar10;
  undefined8 uVar11;
  undefined8 uVar12;
  undefined8 uVar13;
  
  uVar1 = _DAT_0021e898;
  if ((((DAT_0021e860 & 1) == 0) && ((int)DAT_00214938 == 1)) &&
     (DAT_0021e878 == *(long *)(param_1 + 8))) {
    uVar3 = (ulong)*(uint *)(param_1 + 0x1bd8);
    _DAT_00220700 = CONCAT44(uRam000000000021e8ac,DAT_0021e8a8);
    DAT_00220650 = _DAT_0021e898;
    uRam0000000000220708 = uRam000000000021e8b0;
    DAT_00220710 = DAT_0021e8b8;
    *(undefined4 *)(param_1 + 0x1c48) = 1;
    iVar2 = DAT_0021e8a8;
    if (*(uint *)(param_1 + 0x1bd8) != 0) {
      iVar5 = *(int *)(param_1 + 0x1c3c);
      piVar4 = (int *)(param_1 + 0xd8);
      do {
        iVar6 = *piVar4;
        if (iVar6 == (int)uVar1) {
          uVar7 = *(undefined8 *)piVar4;
          *(undefined8 *)(param_1 + 0x1c58) = *(undefined8 *)(piVar4 + 2);
          *(undefined8 *)(param_1 + 0x1c50) = uVar7;
          uVar9 = *(undefined8 *)(piVar4 + 0xc);
          uVar8 = *(undefined8 *)(piVar4 + 0x12);
          uVar7 = *(undefined8 *)(piVar4 + 0x10);
          uVar13 = *(undefined8 *)(piVar4 + 6);
          uVar12 = *(undefined8 *)(piVar4 + 4);
          uVar11 = *(undefined8 *)(piVar4 + 10);
          uVar10 = *(undefined8 *)(piVar4 + 8);
          *(undefined8 *)(param_1 + 0x1c88) = *(undefined8 *)(piVar4 + 0xe);
          *(undefined8 *)(param_1 + 0x1c80) = uVar9;
          *(undefined8 *)(param_1 + 0x1c98) = uVar8;
          *(undefined8 *)(param_1 + 0x1c90) = uVar7;
          *(undefined8 *)(param_1 + 0x1c68) = uVar13;
          *(undefined8 *)(param_1 + 0x1c60) = uVar12;
          *(undefined8 *)(param_1 + 0x1c78) = uVar11;
          *(undefined8 *)(param_1 + 0x1c70) = uVar10;
          uVar9 = *(undefined8 *)(piVar4 + 0x1c);
          uVar8 = *(undefined8 *)(piVar4 + 0x22);
          uVar7 = *(undefined8 *)(piVar4 + 0x20);
          uVar13 = *(undefined8 *)(piVar4 + 0x16);
          uVar12 = *(undefined8 *)(piVar4 + 0x14);
          uVar11 = *(undefined8 *)(piVar4 + 0x1a);
          uVar10 = *(undefined8 *)(piVar4 + 0x18);
          *(undefined8 *)(param_1 + 0x1cc8) = *(undefined8 *)(piVar4 + 0x1e);
          *(undefined8 *)(param_1 + 0x1cc0) = uVar9;
          *(undefined8 *)(param_1 + 0x1cd8) = uVar8;
          *(undefined8 *)(param_1 + 0x1cd0) = uVar7;
          *(undefined8 *)(param_1 + 0x1ca8) = uVar13;
          *(undefined8 *)(param_1 + 0x1ca0) = uVar12;
          *(undefined8 *)(param_1 + 0x1cb8) = uVar11;
          *(undefined8 *)(param_1 + 0x1cb0) = uVar10;
          iVar6 = *piVar4;
        }
        if (((iVar6 != iVar5) && (iVar6 == iVar2)) &&
           ((piVar4[0x18] != 0 && ((piVar4[0x19] != 0 && (0 < piVar4[0x12])))))) {
          uVar7 = *(undefined8 *)(piVar4 + 10);
          *(int *)(param_1 + 0x1c3c) = iVar2;
          *(undefined4 *)(param_1 + 0x1c38) = 1;
          *(undefined8 *)(param_1 + 0x1c40) = uVar7;
          iVar5 = iVar2;
        }
        piVar4 = piVar4 + 0x24;
        uVar3 = uVar3 - 1;
      } while (uVar3 != 0);
    }
  }
  return;
}

/* ===== FUN_00166084 @ 00166084 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

bool FUN_00166084(void)

{
  return (int)_DAT_00220784 == 1;
}

/* ===== FUN_00166188 @ 00166188 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

bool FUN_00166188(void)

{
  return (int)_DAT_002208a0 == 1;
}

/* ===== FUN_00166934 @ 00166934 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00166934(undefined8 *param_1)

{
  long lVar1;
  undefined8 uVar2;
  int iVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  int local_12c;
  long local_128;
  int local_11c;
  long local_118;
  undefined1 auStack_110 [4];
  uint local_10c;
  int local_108;
  uint local_100;
  int local_fc;
  uint local_f4;
  int local_f0;
  uint local_e8;
  int local_e4;
  uint local_dc;
  int local_d8;
  undefined *local_d0;
  undefined *puStack_c8;
  undefined *puStack_c0;
  undefined *puStack_b8;
  undefined1 auStack_b0 [4];
  uint local_ac;
  int local_a8;
  undefined4 uStack_a4;
  uint local_a0;
  int iStack_9c;
  undefined4 uStack_98;
  uint uStack_94;
  int local_90;
  uint local_88;
  int local_84;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  uVar4 = 0;
  local_128 = 0;
  local_12c = -1;
  if (((int)_DAT_00220784 != 1) || (DAT_00220918 == (code *)0x0)) goto LAB_00166cfc;
  iVar3 = (*DAT_00220918)(&PTR_s_espEnabled_001c4360,auStack_110,5,&local_128,&local_12c);
  uVar4 = 0;
  if ((iVar3 != 1) || ((local_128 == 0 || (local_12c != 0)))) goto LAB_00166cfc;
  iVar3 = pthread_once((pthread_once_t *)&DAT_0021ca04,FUN_00164598);
  if ((DAT_0021ca08 == (code *)0x0) || (iVar3 = (*DAT_0021ca08)(iVar3), iVar3 != 1)) {
    local_10c = 0;
    local_e8 = 0;
    local_f4 = 0;
    local_dc = 0;
    local_100 = 1;
  }
  else {
    uVar4 = 0;
    if ((((((1 < local_10c) || (local_108 != 2)) || (uVar4 = 0, 1 < local_100)) ||
         ((local_fc != 2 || (uVar4 = 0, 1 < local_f4)))) || (local_f0 != 2)) ||
       (((uVar4 = 0, 1 < local_e8 || (local_e4 != 2)) ||
        ((uVar4 = 0, 1 < local_dc || (local_d8 != 2)))))) goto LAB_00166cfc;
  }
  *(uint *)(param_1 + 4) = local_10c;
  *(uint *)((long)param_1 + 0x24) = local_100;
  uVar2 = DAT_0010e5c0;
  *(uint *)(param_1 + 5) = local_f4;
  *(uint *)((long)param_1 + 0x2c) = local_e8;
  uVar5 = _UNK_00112838;
  uVar4 = _DAT_00112830;
  *param_1 = 0;
  param_1[1] = local_128;
  *(uint *)(param_1 + 7) = local_dc;
  param_1[6] = uVar2;
  param_1[3] = uVar5;
  param_1[2] = uVar4;
  *(undefined8 *)((long)param_1 + 0x44) = 0;
  *(undefined8 *)((long)param_1 + 0x3c) = 0;
  *(undefined8 *)((long)param_1 + 0x54) = 0;
  *(undefined8 *)((long)param_1 + 0x4c) = 0;
  *(undefined8 *)((long)param_1 + 100) = 0;
  *(undefined8 *)((long)param_1 + 0x5c) = 0;
  param_1[0xe] = 0;
  param_1[0xd] = 0;
  if ((int)DAT_00214938 != 1) {
    uVar4 = 1;
    goto LAB_00166cfc;
  }
  iVar3 = pthread_once((pthread_once_t *)&DAT_0021ca04,FUN_00164598);
  if ((DAT_0021ca08 == (code *)0x0) || (iVar3 = (*DAT_0021ca08)(iVar3), iVar3 != 1)) {
LAB_00166b50:
    param_1[3] = uVar5;
    param_1[2] = uVar4;
    iVar3 = pthread_once((pthread_once_t *)&DAT_0021ca04,FUN_00164598);
    if ((DAT_0021ca08 == (code *)0x0) || (iVar3 = (*DAT_0021ca08)(iVar3), iVar3 != 1)) {
      local_118 = 0;
      local_11c = -1;
      puStack_c8 = PTR_s_antiAfkEnabled_001c4390;
      local_d0 = PTR_s_characterOutlineEnabled_001c4388;
      if ((DAT_00214930 != (code *)0x0) &&
         (iVar3 = (*DAT_00214930)(&local_d0,auStack_b0,2,&local_118,&local_11c), iVar3 == 1)) {
        uVar4 = 0;
        if ((((local_118 == param_1[1]) && (local_11c == 0)) && (local_a8 == 2)) &&
           (((uVar4 = 0, local_ac < 2 && (iStack_9c == 2)) && (local_a0 < 2)))) {
          uVar4 = 1;
          param_1[10] = 0;
          param_1[0xb] = 0;
        }
        goto LAB_00166cfc;
      }
    }
    else {
      puStack_c8 = PTR_s_enemyTracer_001c43a0;
      local_d0 = PTR_s_hitboxRenderer_001c4398;
      puStack_b8 = PTR_s_trophiesAboveHead_001c43b0;
      puStack_c0 = PTR_s_attackRangeIndicator_001c43a8;
      if ((DAT_00214930 != (code *)0x0) &&
         (iVar3 = (*DAT_00214930)(&local_d0,auStack_b0,4,&local_118,&local_11c), iVar3 == 1)) {
        uVar4 = 0;
        if (((((local_118 != param_1[1]) ||
              (((local_11c != 0 || (local_a8 != 2)) || (uVar4 = 0, 1 < local_ac)))) ||
             ((iStack_9c != 2 || (uVar4 = 0, 1 < local_a0)))) || (local_90 != 2)) ||
           ((uVar4 = 0, 1 < uStack_94 || (local_84 != 2)))) goto LAB_00166cfc;
        if (local_88 < 2) {
          uVar4 = 1;
          *(uint *)(param_1 + 10) = local_ac;
          *(uint *)((long)param_1 + 0x54) = local_a0;
          *(uint *)(param_1 + 0xb) = uStack_94;
          *(uint *)((long)param_1 + 0x5c) = local_88;
          goto LAB_00166cfc;
        }
      }
    }
  }
  else {
    iVar3 = FUN_00164c78(auStack_b0);
    if ((iVar3 != 0) && (CONCAT44(uStack_a4,local_a8) == param_1[1])) {
      uVar5 = CONCAT44(uStack_94,uStack_98);
      uVar4 = CONCAT44(iStack_9c,local_a0);
      goto LAB_00166b50;
    }
  }
  uVar4 = 0;
LAB_00166cfc:
  if (*(long *)(lVar1 + 0x28) != local_38) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar4);
  }
  return;
}

/* ===== FUN_00168604 @ 00168604 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00168604(long param_1)

{
  pthread_mutex_t *__mutex;
  long lVar1;
  int iVar2;
  undefined4 uVar3;
  undefined8 uVar4;
  undefined8 *puVar5;
  int local_19c;
  undefined8 local_198;
  undefined8 uStack_190;
  undefined8 local_188;
  undefined8 uStack_180;
  undefined8 local_178;
  undefined8 uStack_170;
  undefined8 local_168;
  undefined8 uStack_160;
  undefined8 local_158;
  undefined8 uStack_150;
  undefined8 local_148;
  undefined8 uStack_140;
  undefined8 local_138;
  undefined8 uStack_130;
  undefined8 local_128;
  undefined8 uStack_120;
  undefined8 local_118;
  undefined8 uStack_110;
  undefined8 local_108;
  undefined8 uStack_100;
  undefined8 uStack_f8;
  undefined8 uStack_f0;
  undefined8 local_e8;
  undefined8 uStack_e0;
  undefined8 uStack_d8;
  undefined8 uStack_d0;
  undefined8 local_c8;
  undefined8 uStack_c0;
  undefined8 uStack_b8;
  undefined8 uStack_b0;
  undefined8 local_a8;
  undefined8 uStack_a0;
  undefined8 uStack_98;
  undefined8 uStack_90;
  undefined8 local_88;
  undefined8 uStack_80;
  undefined8 uStack_78;
  undefined8 uStack_70;
  undefined8 local_68;
  undefined8 uStack_60;
  undefined8 uStack_58;
  undefined8 uStack_50;
  long local_48;
  
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  if (DAT_00228598 == *(long *)(param_1 + 0x18)) goto LAB_001687f0;
  local_198 = *(undefined8 *)(param_1 + 0x20);
  uStack_190 = *(undefined8 *)(param_1 + 0x30);
  uStack_100 = 0;
  local_108 = 0;
  uStack_f0 = 0;
  uStack_f8 = 0;
  uStack_e0 = 0;
  local_e8 = 0;
  uStack_d0 = 0;
  uStack_d8 = 0;
  uStack_c0 = 0;
  local_c8 = 0;
  uStack_b0 = 0;
  uStack_b8 = 0;
  uStack_a0 = 0;
  local_a8 = 0;
  uStack_90 = 0;
  uStack_98 = 0;
  uStack_80 = 0;
  local_88 = 0;
  uStack_70 = 0;
  uStack_78 = 0;
  uStack_60 = 0;
  local_68 = 0;
  uStack_50 = 0;
  uStack_58 = 0;
  uStack_110 = 0;
  local_118 = 0;
  uStack_120 = 0;
  local_128 = 0;
  uStack_130 = 0;
  local_138 = 0;
  uStack_140 = 0;
  local_148 = 0;
  uStack_150 = 0;
  local_158 = 0;
  uStack_160 = 0;
  local_168 = 0;
  uStack_170 = 0;
  uStack_180 = 0;
  local_188 = 0;
  local_178 = *(undefined8 *)(param_1 + 100);
  _DAT_002285a8 = 0;
  DAT_002285b0 = 0;
  DAT_002285a0 = 0;
  DAT_00228598 = *(long *)(param_1 + 0x18);
  iVar2 = FUN_001550fc();
  if ((iVar2 == 0) ||
     (iVar2 = FUN_0018f704(0,FUN_001428fc,DAT_001e0978,DAT_0020f700,*(undefined8 *)(param_1 + 0x20),
                           &local_168), iVar2 == 0)) goto LAB_001687d8;
  __mutex = (pthread_mutex_t *)(DAT_001e0978 + 0x12fd3d0);
  iVar2 = pthread_mutex_trylock(__mutex);
  if (iVar2 != 0) goto LAB_001687f0;
  local_19c = -1;
  uVar4 = FUN_001428fc(0,DAT_001e0978 + 0x12fd3fc,&local_19c,4);
  if ((((int)uVar4 == 0) || (local_19c < 0)) || (10 < local_19c)) {
    pthread_mutex_unlock(__mutex);
LAB_001687d8:
    puVar5 = (undefined8 *)0x0;
  }
  else {
    iVar2 = FUN_001428fc(uVar4,DAT_001e0978 + 0x12fd4c8,&uStack_110,200);
    pthread_mutex_unlock(__mutex);
    if (iVar2 == 0) goto LAB_001687d8;
    uStack_170._4_4_ = local_19c;
    uVar3 = FUN_00196eac(*(undefined8 *)(param_1 + 0x30));
    uStack_170 = CONCAT44(uStack_170._4_4_,uVar3);
    if ((DAT_002285e0 == *(long *)(param_1 + 0x20)) && (DAT_002285e8 == *(int *)(param_1 + 0x60))) {
      local_188 = DAT_002285f0;
      uStack_180 = CONCAT44(DAT_002285fc,DAT_002285f8);
    }
    puVar5 = &local_198;
  }
  FUN_0019eed4(&DAT_002285b8,puVar5,&DAT_002285a0);
LAB_001687f0:
  if (*(long *)(lVar1 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0016881c @ 0016881c [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0016881c(undefined8 *param_1,undefined8 param_2,int param_3,long *param_4,ulong *param_5,
                 int *param_6)

{
  long lVar1;
  undefined8 uVar2;
  long lVar3;
  undefined4 uVar4;
  int iVar5;
  undefined4 uVar6;
  ulong uVar7;
  undefined4 uVar8;
  undefined8 uVar9;
  undefined8 uVar10;
  long local_80;
  int local_74;
  undefined1 auStack_70 [4];
  int local_6c;
  int local_68;
  undefined8 local_60;
  long local_58;
  
  lVar1 = tpidr_el0;
  local_58 = *(long *)(lVar1 + 0x28);
  local_80 = 0;
  FUN_00168604(&DAT_00215aa0);
  local_74 = -1;
  local_60 = param_2;
  if (((((int)DAT_00214938 == 1) && (DAT_00214930 != (code *)0x0)) &&
      (iVar5 = (*DAT_00214930)(&local_60,auStack_70,1,&local_80,&local_74), iVar5 == 1)) &&
     (((local_80 != 0 && (local_74 == 0)) && ((local_68 == 2 && (local_6c == 1)))))) {
    if (*param_4 != DAT_00215ac0) {
      *param_4 = DAT_00215ac0;
      *param_5 = 0;
      *param_6 = 0;
    }
    iVar5 = DAT_002285ac;
    uVar7 = DAT_00215ad0;
    if (local_80 != DAT_00215ac8) goto LAB_00168a18;
    if (DAT_002285ac != 0) {
      uVar6 = 0;
      uVar8 = 0x41400000;
      *param_6 = 1;
      *param_5 = 0;
LAB_00168930:
      uVar4 = DAT_00215b00;
      uVar2 = _UNK_00112848;
      uVar10 = _DAT_00112840;
      param_1[0xe] = local_80;
      lVar3 = DAT_00215ac0;
      *(undefined4 *)((long)param_1 + 0x84) = uVar8;
      uVar9 = DAT_00215ab8;
      *(undefined4 *)((long)param_1 + 0x1c) = uVar6;
      param_1[9] = uVar2;
      param_1[8] = uVar10;
      uVar2 = DAT_002285a0;
      param_1[4] = lVar3;
      param_1[5] = uVar9;
      uVar10 = DAT_0010e568;
      param_1[0xc] = lVar3;
      param_1[0xd] = uVar9;
      uVar6 = DAT_00215aac;
      param_1[6] = local_80;
      param_1[7] = uVar2;
      param_1[10] = uVar10;
      uVar2 = DAT_002285b0;
      uVar10 = DAT_0010e598;
      uVar9 = NEON_cmlt(CONCAT44((uint)(iVar5 == 0) << 0x1f,(uint)(iVar5 == 0) << 0x1f),0,4);
      *(undefined4 *)(param_1 + 0xb) = 0;
      *(undefined4 *)((long)param_1 + 0x5c) = uVar6;
      *(undefined4 *)(param_1 + 2) = uVar4;
      uVar10 = NEON_bsl(uVar9,uVar10,uVar2,1);
      param_1[0x21] = 0;
      uVar2 = DAT_0010e5e0;
      *(int *)(param_1 + 1) = param_3;
      *(undefined4 *)((long)param_1 + 0xc) = 1;
      param_1[0xf] = uVar10;
      uVar10 = DAT_0010e5c8;
      *param_1 = uVar2;
      *(undefined4 *)(param_1 + 0x10) = 0x435c0000;
      *(undefined8 *)((long)param_1 + 0x14) = uVar10;
      param_1[0x20] = 0;
      param_1[0x1f] = 0;
      param_1[0x1e] = 0;
      param_1[0x1d] = 0;
      param_1[0x1c] = 0;
      param_1[0x1b] = 0;
      param_1[0x1a] = 0;
      param_1[0x19] = 0;
      param_1[0x18] = 0;
      param_1[0x17] = 0;
      param_1[0x16] = 0;
      param_1[0x15] = 0;
      param_1[0x14] = 0;
      param_1[0x13] = 0;
      param_1[0x12] = 0;
      param_1[0x11] = 0;
      if (iVar5 == 0) {
        param_1[7] = DAT_002285c0;
      }
      goto LAB_00168a20;
    }
    if (((*param_6 == 0) || (DAT_00228488 != 4)) || (DAT_00228394 != param_3)) {
      uVar7 = *param_5;
    }
    else {
      *param_5 = DAT_00215ad0;
    }
    *param_6 = 0;
    if (uVar7 == 0) goto LAB_00168a20;
    if ((((DAT_00228488 != 4) || (DAT_002284bc != 0.0)) || (DAT_002284ec == 0)) &&
       ((uVar7 <= DAT_00215ad0 && (DAT_00215ad0 - uVar7 < 0x1f5)))) {
      uVar8 = 0;
      uVar6 = 1;
      goto LAB_00168930;
    }
  }
  else {
LAB_00168a18:
    *param_6 = 0;
  }
  *param_5 = 0;
LAB_00168a20:
  if (*(long *)(lVar1 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0016a17c @ 0016a17c [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_0016a17c(undefined8 param_1,int param_2,int *param_3)

{
  uint uVar1;
  int iVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  int iVar7;
  ulong uVar8;
  
  uVar6 = DAT_00214818;
  uVar5 = DAT_00214810;
  uVar4 = DAT_00214808;
  uVar3 = DAT_002147f8;
  iVar2 = DAT_00209cd8;
  if (param_3 == (int *)0x0) {
    return 0;
  }
  if (*param_3 != 1) {
    return 0;
  }
  if (param_3[1] != 0x28) {
    return 0;
  }
  uVar1 = param_2 - 0x16;
  if (uVar1 < 0x2f) {
    if ((1L << ((ulong)uVar1 & 0x3f) & 0x400001000011U) != 0) goto LAB_0016a25c;
    if ((ulong)uVar1 == 1) {
      *(undefined8 *)(param_3 + 2) = _DAT_00214800;
      *(undefined8 *)param_3 = uVar3;
      *(undefined8 *)(param_3 + 6) = uVar5;
      *(undefined8 *)(param_3 + 4) = uVar4;
      *(undefined8 *)(param_3 + 8) = uVar6;
      uVar8 = FUN_001391f0(1);
      iVar2 = DAT_00209cd8;
      if ((((DAT_00209cd8 != 0) && (iVar7 = gettid(), iVar2 == iVar7)) && (DAT_0020f668 <= uVar8))
         && (uVar8 - DAT_0020f668 < 0x1f5)) {
        return 1;
      }
      param_3[2] = 0;
      return 1;
    }
  }
  if (0xb < param_2 - 0x79U) {
    return 0;
  }
  if ((1 << (ulong)(param_2 - 0x79U & 0x1f) & 0x803U) == 0) {
    return 0;
  }
LAB_0016a25c:
  if (DAT_00215c90 != '\x01') {
    return 0;
  }
  if (DAT_00215a64 != '\0') {
    if ((DAT_00209cd8 != 0) &&
       (iVar7 = gettid(0), uVar6 = DAT_00215c88, uVar5 = uRam0000000000215c80, uVar4 = _DAT_00215c78
       , uVar3 = _DAT_00215c68, iVar2 == iVar7)) {
      *(undefined8 *)(param_3 + 2) = uRam0000000000215c70;
      *(undefined8 *)param_3 = uVar3;
      *(undefined8 *)(param_3 + 6) = uVar5;
      *(undefined8 *)(param_3 + 4) = uVar4;
      *(undefined8 *)(param_3 + 8) = uVar6;
      return 1;
    }
    return 0;
  }
  return 0;
}

/* ===== FUN_0016aa6c @ 0016aa6c [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_0016aa6c(undefined8 param_1,undefined8 *param_2)

{
  undefined8 uVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  byte bVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  ulong uVar9;
  
  uVar9 = DAT_00228e20;
  if (param_2 != (undefined8 *)0x0) {
    *(undefined4 *)((long)param_2 + 0x4c) = 0;
    bVar4 = DAT_00216f08;
    uVar3 = DAT_00214c18;
    uVar2 = DAT_00209d00;
    uVar1 = _DAT_00112aa0;
    param_2[1] = _UNK_00112aa8;
    *param_2 = uVar1;
    param_2[2] = uVar2;
    param_2[3] = uVar3;
    param_2[4] = uVar9 >> 0x20;
    *(uint *)(param_2 + 5) = (uint)bVar4;
    *(uint *)((long)param_2 + 0x2c) = (uint)((uVar9 & 1) == 0);
    *(undefined8 *)((long)param_2 + 0x45) = 0;
    uVar2 = s_bsd_suitcase_nexusv2_0011a7e3._0_8_;
    uVar1 = CONCAT53(s_bsd_suitcase_nexusv2_0011a7e3._16_5_,s_bsd_suitcase_nexusv2_0011a7e3._13_3_);
    param_2[7] = CONCAT35(s_bsd_suitcase_nexusv2_0011a7e3._13_3_,
                          s_bsd_suitcase_nexusv2_0011a7e3._8_5_);
    param_2[6] = uVar2;
    *(undefined8 *)((long)param_2 + 0x3d) = uVar1;
    uVar8 = uRam0000000000228e18;
    uVar7 = _DAT_00228e10;
    uVar6 = _DAT_00228e00;
    uVar5 = uRam0000000000228df8;
    uVar3 = _DAT_00228df0;
    uVar2 = uRam0000000000228de8;
    uVar1 = _DAT_00228de0;
    param_2[0xf] = uRam0000000000228e08;
    param_2[0xe] = uVar6;
    param_2[0x11] = uVar8;
    param_2[0x10] = uVar7;
    param_2[0xb] = uVar2;
    param_2[10] = uVar1;
    param_2[0xd] = uVar5;
    param_2[0xc] = uVar3;
    return 1;
  }
  return 0;
}

/* ===== FUN_0016ab00 @ 0016ab00 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_0016ab00(undefined8 param_1,int param_2,undefined8 *param_3)

{
  int iVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  int iVar6;
  undefined8 uVar7;
  
  iVar1 = DAT_00209cd8;
  uVar7 = 0;
  if ((((param_2 == 0x19) && (param_3 != (undefined8 *)0x0)) &&
      (uVar7 = 0, ((DAT_002170e4 | DAT_002170f8) & 1) != 0)) && (DAT_002170e0 != '\0')) {
    if ((DAT_00209cd8 == 0) ||
       (iVar6 = gettid(0), uVar5 = DAT_002170d8, uVar4 = DAT_002170d0, uVar3 = DAT_002170c8,
       uVar2 = DAT_002170b8, iVar1 != iVar6)) {
      uVar7 = 0;
    }
    else {
      uVar7 = 1;
      param_3[1] = _DAT_002170c0;
      *param_3 = uVar2;
      param_3[3] = uVar4;
      param_3[2] = uVar3;
      param_3[4] = uVar5;
    }
  }
  return uVar7;
}

/* ===== FUN_0016ab90 @ 0016ab90 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_0016ab90(undefined8 param_1,undefined8 *param_2)

{
  undefined8 uVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  byte bVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  ulong uVar9;
  
  uVar9 = DAT_00228e38;
  if (param_2 != (undefined8 *)0x0) {
    *(undefined4 *)((long)param_2 + 0x4c) = 0;
    bVar4 = DAT_00216f08;
    uVar3 = DAT_00214c18;
    uVar2 = DAT_00209d00;
    uVar1 = _DAT_00112c50;
    param_2[1] = _UNK_00112c58;
    *param_2 = uVar1;
    param_2[2] = uVar2;
    param_2[3] = uVar3;
    param_2[4] = uVar9 >> 0x20;
    *(uint *)(param_2 + 5) = (uint)bVar4;
    *(uint *)((long)param_2 + 0x2c) = (uint)((uVar9 & 1) == 0);
    *(undefined8 *)((long)param_2 + 0x45) = 0;
    uVar2 = s_bsd_suitcase_nexusv2_0011a7e3._0_8_;
    uVar1 = CONCAT53(s_bsd_suitcase_nexusv2_0011a7e3._16_5_,s_bsd_suitcase_nexusv2_0011a7e3._13_3_);
    param_2[7] = CONCAT35(s_bsd_suitcase_nexusv2_0011a7e3._13_3_,
                          s_bsd_suitcase_nexusv2_0011a7e3._8_5_);
    param_2[6] = uVar2;
    *(undefined8 *)((long)param_2 + 0x3d) = uVar1;
    uVar8 = uRam0000000000228e18;
    uVar7 = _DAT_00228e10;
    uVar6 = _DAT_00228e00;
    uVar5 = uRam0000000000228df8;
    uVar3 = _DAT_00228df0;
    uVar2 = uRam0000000000228de8;
    uVar1 = _DAT_00228de0;
    param_2[0xf] = uRam0000000000228e08;
    param_2[0xe] = uVar6;
    param_2[0x11] = uVar8;
    param_2[0x10] = uVar7;
    param_2[0xb] = uVar2;
    param_2[10] = uVar1;
    param_2[0xd] = uVar5;
    param_2[0xc] = uVar3;
    return 1;
  }
  return 0;
}

/* ===== FUN_0016ac24 @ 0016ac24 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_0016ac24(undefined8 param_1,int param_2,int param_3,undefined8 *param_4)

{
  int iVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  int iVar6;
  undefined8 uVar7;
  
  iVar1 = DAT_00209cd8;
  uVar7 = 0;
  if ((((param_2 == 0x1d) && (param_4 != (undefined8 *)0x0)) && (uVar7 = 0, DAT_00214d00 == param_3)
      ) && ((DAT_00214cfc != '\0' && (DAT_00214978 != '\0')))) {
    if ((DAT_00209cd8 == 0) ||
       (iVar6 = gettid(0), uVar5 = DAT_00214d28, uVar4 = DAT_00214d20, uVar3 = DAT_00214d18,
       uVar2 = DAT_00214d08, iVar1 != iVar6)) {
      uVar7 = 0;
    }
    else {
      uVar7 = 1;
      param_4[1] = _DAT_00214d10;
      *param_4 = uVar2;
      param_4[3] = uVar4;
      param_4[2] = uVar3;
      param_4[4] = uVar5;
    }
  }
  return uVar7;
}

/* ===== FUN_0016b778 @ 0016b778 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ushort FUN_0016b778(long param_1)

{
  ushort uVar1;
  ushort uVar2;
  undefined1 auVar3 [16];
  
  if (param_1 != 0) {
    if ((int)DAT_00214938 != 1) {
      return 0;
    }
    if (DAT_00215a60 == '\0') {
      return 0;
    }
    if (((DAT_00228398 == *(long *)(param_1 + 0x18)) && (DAT_002283a0 == *(long *)(param_1 + 0x20)))
       && (FUN_00168604(), DAT_002285ac == 0)) {
      auVar3._8_4_ = DAT_00228534;
      auVar3._0_8_ = _DAT_002284e8;
      auVar3._12_4_ = DAT_0022856c;
      auVar3 = NEON_cmtst(auVar3,auVar3,4);
      uVar2 = NEON_umaxv(CONCAT26(auVar3._12_2_,
                                  CONCAT24(auVar3._8_2_,CONCAT22(auVar3._4_2_,auVar3._0_2_))),2);
      uVar1 = 0;
      if (DAT_00228480 != 2) {
        uVar1 = (ushort)(DAT_002284e4 == 0) & (uVar2 ^ 0xffff);
      }
      uVar2 = 0;
      if (DAT_0022851c == 0) {
        uVar2 = uVar1;
      }
      if (DAT_0022853c != 0) {
        return 0;
      }
      return uVar2 & DAT_00228538 == 0;
    }
  }
  return 0;
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

/* ===== FUN_0016e3b0 @ 0016e3b0 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0016e3b0(undefined8 param_1,long param_2,int param_3)

{
  long lVar1;
  int iVar2;
  int iVar3;
  ulong uVar4;
  ulong uVar5;
  long local_c8;
  timespec local_c0;
  int local_b0;
  long local_48;
  
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  iVar2 = clock_gettime(1,&local_c0);
  iVar3 = DAT_00209cd8;
  if (iVar2 == 0) {
    uVar5 = local_c0.tv_sec * 1000 + (ulong)local_c0.tv_nsec / 1000000;
  }
  else {
    uVar5 = 0;
  }
  if ((((param_2 != 0) && ((int)DAT_00214938 == 1)) && ((int)DAT_001dfff0 != 0)) &&
     ((DAT_00209cd8 != 0 && (iVar2 = gettid(), iVar3 == iVar2)))) {
    iVar3 = FUN_00164c78(&local_c0);
    uVar4 = 0;
    if ((iVar3 == 0) || (local_b0 == 0)) goto LAB_0016e5a8;
    if (local_c0.tv_nsec == *(ulong *)(*(long *)(param_2 + 0x50) + 8)) {
      uVar4 = FUN_00150bf0(DAT_0021e780,DAT_0021e798);
      if ((int)uVar4 == 0) goto LAB_0016e5a8;
      if (((DAT_0020f6f8 == *(long *)(*(long *)(param_2 + 0x48) + 8)) &&
          (DAT_0020f6f0 == *(long *)(*(long *)(param_2 + 0x48) + 0x10))) &&
         (DAT_0020f650 == *(long *)(*(long *)(param_2 + 0x40) + 0x20))) {
        uVar4 = 0;
        if (((uVar5 < DAT_0021ca48) || (500 < uVar5 - DAT_0021ca48)) ||
           (uVar4 = FUN_0013a78c(DAT_0021e788 + 0x58,&local_c8), (int)uVar4 == 0))
        goto LAB_0016e5a8;
        if (local_c8 == DAT_0021e7b0) {
          if (param_3 != 2) {
            uVar4 = (ulong)(param_3 == 1);
            goto LAB_0016e5a8;
          }
          uVar4 = 0;
          if ((*(int *)(param_2 + 0x10) != 2) || (DAT_00220798 == 0)) goto LAB_0016e5a8;
          if (*(long *)(param_2 + 0x28) == DAT_00220798) {
            uVar4 = 0;
            if ((*(long *)(param_2 + 0x20) != DAT_00220790) || (local_c8 != DAT_00220790))
            goto LAB_0016e5a8;
            if (*(long *)(param_2 + 0x18) == _DAT_00220788) {
              iVar3 = FUN_00168fe8(0);
              uVar4 = (ulong)(iVar3 != 0);
              goto LAB_0016e5a8;
            }
          }
        }
      }
    }
  }
  uVar4 = 0;
LAB_0016e5a8:
  if (*(long *)(lVar1 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar4);
}

/* ===== FUN_0016f8e8 @ 0016f8e8 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0016f8e8(undefined8 param_1,undefined4 param_2,undefined4 param_3,float param_4,
                 float param_5)

{
  undefined4 uVar1;
  char cVar2;
  bool bVar3;
  long lVar4;
  uint uVar5;
  int iVar6;
  undefined4 *puVar7;
  long *plVar8;
  long lVar9;
  undefined8 uVar10;
  undefined8 local_70;
  long local_68;
  
  lVar4 = tpidr_el0;
  local_68 = *(long *)(lVar4 + 0x28);
  puVar7 = (undefined4 *)__errno();
  uVar1 = *puVar7;
  plVar8 = (long *)FUN_001bd828(&DAT_001cfd38);
  if ((*plVar8 != 0) && (lVar9 = FUN_00139fcc(), lVar9 != 0)) {
    local_70 = 0;
    do {
      uVar5 = _DAT_001dff88;
      cVar2 = '\x01';
      bVar3 = (bool)ExclusiveMonitorPass(0x1dff88,0x10);
      if (bVar3) {
        _DAT_001dff88 = CONCAT31(DAT_001dff88_1,1);
        cVar2 = ExclusiveMonitorsStatus();
      }
    } while (cVar2 != '\0');
    if ((((((uVar5 & 1) == 0) && (_DAT_001dff88 = 0, DAT_001dffa0 == 2)) && (DAT_001e0060 != 0)) &&
        ((uVar10 = FUN_001428fc(lVar9,DAT_001e0060 + 0xcc,(long)&local_70 + 4,4), (int)uVar10 != 0
         && (iVar6 = FUN_001428fc(uVar10,DAT_001e0060 + 0xd0,&local_70,4), iVar6 != 0)))) &&
       ((0 < (int)local_70._4_4_ &&
        ((0xffff8acf < (int)local_70 - 0x7531U && (local_70._4_4_ < 0x7531)))))) {
      param_4 = (float)(int)local_70 * 0.5;
      param_5 = (float)(int)local_70 * 7.5;
    }
  }
  *puVar7 = uVar1;
  (*(code *)(DAT_001e0978 + 0x671860))(param_1,param_2,param_3,param_4,param_5);
  if (*(long *)(lVar4 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_001726a0 @ 001726a0 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_001726a0(undefined8 param_1,ulong param_2,undefined4 param_3)

{
  undefined4 uVar1;
  char cVar2;
  bool bVar3;
  uint uVar4;
  long lVar5;
  int iVar6;
  int iVar7;
  undefined4 *puVar8;
  undefined8 uVar9;
  uint uVar10;
  ulong local_20b8;
  ulong local_20b0;
  ulong local_20a8;
  ulong local_20a0;
  undefined8 local_2098;
  int local_208c;
  ulong local_2088;
  ulong local_2080;
  uint local_2078;
  int local_2074;
  ulong local_2070 [1024];
  long local_70;
  
  lVar5 = tpidr_el0;
  local_70 = *(long *)(lVar5 + 0x28);
  puVar8 = (undefined4 *)__errno();
  uVar1 = *puVar8;
  local_20b8 = 0;
  local_20b0 = 0;
  if ((((((DAT_0020af50 != '\x01') ||
         (uVar9 = FUN_001428fc(puVar8,param_2,&local_20b0,8), (int)uVar9 == 0)) ||
        (local_20b0 + 0x2000 < 0x12000)) ||
       (((local_20b0 & 7) != 0 ||
        (uVar9 = FUN_001428fc(uVar9,local_20b0 + 0x28,&local_20b8,8), (int)uVar9 == 0)))) ||
      (local_20b8 + 0x2000 < 0x12000)) ||
     ((((local_20b8 & 7) != 0 || (local_20b8 < DAT_001e0978)) ||
      ((DAT_001e0978 + 0x1200000 <= local_20b8 ||
       (((uVar9 = FUN_001428fc(uVar9,local_20b8,&local_2078,8), (int)uVar9 == 0 ||
         ((local_2078 & 0xffe0001f) != 0x52800000)) || (local_2074 != -0x29a0fc40))))))))
  goto LAB_00172d00;
  uVar4 = local_2078 >> 5 & 0xffff;
  if (uVar4 < 0x5eed) {
    if (uVar4 == 0x4e88) {
      do {
        cVar2 = '\x01';
        bVar3 = (bool)ExclusiveMonitorPass(0x1cfbe8,0x10);
        if (bVar3) {
          cVar2 = ExclusiveMonitorsStatus();
          DAT_001cfbe8 = DAT_001cfbe8 + 1;
        }
      } while (cVar2 != '\0');
      local_2070[0] = 0;
      local_2080 = 0;
      uVar9 = FUN_001428fc(uVar9,param_2 + 0x90,local_2070,8);
      if ((((int)uVar9 != 0) && (0x11fff < local_2070[0] + 0x2000)) && ((local_2070[0] & 7) == 0)) {
        FUN_001428fc(uVar9,local_2070[0],&local_2080,8);
      }
      do {
        uVar10 = _DAT_001e12e8;
        cVar2 = '\x01';
        bVar3 = (bool)ExclusiveMonitorPass(0x1e12e8,0x10);
        if (bVar3) {
          _DAT_001e12e8 = CONCAT31(DAT_001e12e8_1,1);
          cVar2 = ExclusiveMonitorsStatus();
        }
      } while (cVar2 != '\0');
      if ((uVar10 & 1) == 0) {
        DAT_002290f0 = local_2080;
        DAT_0022b558 = 0;
        DAT_001e1310 = 0;
        DAT_0022b560 = 0;
        DAT_001e12f8 = 0;
        DAT_001e1300 = 0;
        DAT_001e14a4 = 0;
        memset(&DAT_001e14a8,0,0x285d0);
        DAT_001e14dc = 0xffffffff;
        DAT_001cfb00 = DAT_001cfb00 + 1;
        _DAT_001e12e8 = 0;
      }
      local_2088 = 0;
      local_208c = 0;
      local_20a0 = 0;
      local_2098 = 0;
      local_20a8 = 0;
      do {
        uVar10 = _DAT_002290f8;
        cVar2 = '\x01';
        bVar3 = (bool)ExclusiveMonitorPass(0x2290f8,0x10);
        if (bVar3) {
          _DAT_002290f8 = CONCAT31(DAT_002290f8_1,1);
          cVar2 = ExclusiveMonitorsStatus();
        }
      } while (cVar2 != '\0');
      if ((uVar10 & 1) == 0) {
        DAT_0022b500 = 0;
        DAT_0022b504 = 0;
        DAT_0022b550 = 0;
LAB_00172b88:
        _DAT_002290f8 = 0;
      }
    }
    else {
      uVar10 = 0x4e89;
LAB_001727f0:
      if (uVar4 == uVar10) goto LAB_001727f8;
    }
  }
  else {
    if (uVar4 != 0x5eed) {
      uVar10 = 0x5f4f;
      goto LAB_001727f0;
    }
LAB_001727f8:
    local_2088 = 0;
    local_208c = 0;
    local_20a0 = 0;
    local_2098 = 0;
    local_20a8 = 0;
    if (uVar4 == 0x4e89) {
      iVar6 = FUN_001428fc(uVar9,param_2 + 0x90,&local_2098,8);
      if ((iVar6 != 0) &&
         (iVar7 = FUN_00172d54(local_2098,0,local_2070,0x400,&local_208c), iVar6 = local_208c,
         iVar7 != 0)) {
        do {
          uVar10 = _DAT_002290f8;
          cVar2 = '\x01';
          bVar3 = (bool)ExclusiveMonitorPass(0x2290f8,0x10);
          if (bVar3) {
            _DAT_002290f8 = CONCAT31(DAT_002290f8_1,1);
            cVar2 = ExclusiveMonitorsStatus();
          }
        } while (cVar2 != '\0');
        if ((uVar10 & 1) == 0) {
          memcpy(&DAT_00229500,local_2070,(ulong)(uint)(local_208c << 3));
          DAT_0022b504 = iVar6;
          goto LAB_00172b88;
        }
      }
    }
    else if (uVar4 == 0x5eed) {
      uVar9 = FUN_001428fc(uVar9,param_2 + 0x98,&local_20a0,8);
      if (((((int)uVar9 != 0) && (0x11fff < local_20a0 + 0x2000)) && ((local_20a0 & 7) == 0)) &&
         (((uVar9 = FUN_001428fc(uVar9,local_20a0,&local_20a8,8), (int)uVar9 != 0 &&
           (0x11fff < local_20a8 + 0x2000)) && ((local_20a8 & 7) == 0)))) {
        local_2080 = 0;
        uVar9 = FUN_001428fc(uVar9,local_20a8,&local_2080,8);
        if ((((int)uVar9 != 0) && (0x11fff < local_2080 + 0x2000)) &&
           (((local_2080 & 7) == 0 &&
            ((((uVar9 = FUN_001428fc(uVar9,local_2080,&local_2088,8), (int)uVar9 != 0 &&
               (local_2088 != 0)) &&
              (iVar6 = FUN_001428fc(uVar9,local_20a0 + 8,&local_2098,8), iVar6 != 0)) &&
             (iVar6 = FUN_00172d54(local_2098,0x28,local_2070,0x80,&local_208c), iVar6 != 0)))))) {
          do {
            uVar10 = _DAT_002290f8;
            cVar2 = '\x01';
            bVar3 = (bool)ExclusiveMonitorPass(0x2290f8,0x10);
            if (bVar3) {
              _DAT_002290f8 = CONCAT31(DAT_002290f8_1,1);
              cVar2 = ExclusiveMonitorsStatus();
            }
          } while (cVar2 != '\0');
          if ((uVar10 & 1) == 0) goto LAB_00172b44;
        }
      }
    }
    else {
      if (uVar4 == 0x5f4f) {
        uVar9 = FUN_001428fc(uVar9,param_2 + 0xa0,&local_20a8,8);
        if ((int)uVar9 == 0) goto LAB_00172b8c;
        if (local_20a8 != 0) {
          local_2080 = 0;
          uVar9 = FUN_001428fc(uVar9,local_20a8,&local_2080,8);
          if ((((int)uVar9 == 0) || (local_2080 + 0x2000 < 0x12000)) ||
             (((local_2080 & 7) != 0 ||
              ((iVar6 = FUN_001428fc(uVar9,local_2080,&local_2088,8), iVar6 == 0 ||
               (local_2088 == 0)))))) goto LAB_00172b8c;
        }
      }
      do {
        uVar10 = _DAT_002290f8;
        cVar2 = '\x01';
        bVar3 = (bool)ExclusiveMonitorPass(0x2290f8,0x10);
        if (bVar3) {
          _DAT_002290f8 = CONCAT31(DAT_002290f8_1,1);
          cVar2 = ExclusiveMonitorsStatus();
        }
      } while (cVar2 != '\0');
      if ((uVar10 & 1) == 0) {
        if (uVar4 == 0x5f4f) {
          if (DAT_0022b550 != local_2088) {
            DAT_0022b500 = 0;
          }
          DAT_0022b550 = local_2088;
          goto LAB_00172b88;
        }
LAB_00172b44:
        iVar6 = local_208c;
        if (((uVar4 == 0x5eed) && (local_2088 != 0)) && (local_2088 == DAT_0022b550)) {
          memcpy(&DAT_00229100,local_2070,(ulong)(uint)(local_208c << 3));
          DAT_0022b500 = iVar6;
        }
        goto LAB_00172b88;
      }
    }
  }
LAB_00172b8c:
  iVar6 = DAT_00209cd8;
  if ((DAT_0022b548 != 0) && ((uVar4 == 0x5e2c || (uVar4 == 0x4e88)))) {
    DAT_002172e0 = 0;
  }
  if ((DAT_00209cd8 != 0) && (iVar7 = gettid(), iVar6 == iVar7)) {
    if (uVar4 - 0x4e87 < 2) {
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
    else if ((uVar4 == 0x60c9) && (DAT_0020af50 != '\0')) {
      local_2088 = local_2088 & 0xffffffff00000000;
      local_2080 = CONCAT44(local_2080._4_4_,0xffffffff);
      uVar9 = FUN_0014a4ac(&DAT_001e00e0,&local_2080);
      if ((int)uVar9 != 0) {
        if ((((DAT_001e01b8 <= (int)local_2080) && (local_2070[0] = 0, 0xfff < param_2)) &&
            (uVar9 = FUN_001428fc(uVar9,param_2 + 0x90,local_2070,8), (int)uVar9 != 0)) &&
           (((0xfff < local_2070[0] && ((local_2070[0] & 7) == 0)) &&
            ((iVar6 = FUN_001428fc(uVar9,local_2070[0],&local_2088,4), iVar6 != 0 &&
             ((DAT_001e01b0 == 0 && (DAT_0022b568 != -1)))))))) {
          DAT_001e01b0 = DAT_0022b568 + 1;
          _DAT_001e01b8 = CONCAT44((int)local_2088,DAT_001e01b8);
          DAT_001e01c0 = (ulong)((int)local_2088 - 4U < 0xfffffffd) << 0x20;
          DAT_0022b568 = DAT_001e01b0;
          if (((int)local_2088 - 4U < 0xfffffffd) && (iVar6 = FUN_00172f00(), iVar6 != 0)) {
            DAT_001e01c0 = CONCAT44(DAT_001e01c0._4_4_,1);
          }
        }
      }
    }
  }
LAB_00172d00:
  *puVar8 = uVar1;
  (*DAT_0022b540)(param_1,param_2,param_3);
  if (*(long *)(lVar5 + 0x28) != local_70) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

/* ===== FUN_0017791c @ 0017791c [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0017791c(long param_1)

{
  undefined4 uVar1;
  long lVar2;
  int iVar3;
  undefined4 *puVar4;
  undefined8 uVar5;
  ulong uVar6;
  ulong uVar7;
  undefined1 *puVar8;
  ulong uVar9;
  ulong uVar10;
  undefined1 (*pauVar11) [16];
  ulong uVar12;
  ulong uVar13;
  undefined8 *puVar14;
  undefined8 *puVar15;
  long lVar16;
  undefined1 auVar17 [16];
  int local_94;
  ulong local_90;
  uint local_88;
  uint uStack_84;
  undefined8 local_80 [3];
  undefined8 local_68;
  ulong local_60;
  long lStack_58;
  timespec local_50;
  long local_38;
  
  lVar2 = tpidr_el0;
  local_38 = *(long *)(lVar2 + 0x28);
  (*DAT_0022d078)();
  puVar4 = (undefined4 *)__errno();
  uVar1 = *puVar4;
  local_60 = 0;
  local_80[1] = 0;
  local_80[0] = 0;
  local_68 = 0;
  local_80[2] = 0;
  uVar5 = FUN_001428fc(puVar4,param_1 + 0x90,&local_90,8);
  if (((((int)uVar5 == 0) || (local_90 + 0x2000 < 0x12000)) || ((local_90 & 7) != 0)) ||
     (((uVar5 = FUN_001428fc(uVar5,local_90,&local_88,8), (int)uVar5 == 0 ||
       (iVar3 = FUN_001428fc(uVar5,param_1 + 0xe4,&local_94,4), iVar3 == 0)) || (local_94 < 0))))
  goto LAB_00177b70;
  uVar12 = (ulong)local_88 + (ulong)uStack_84 * 0x100;
  if (uVar12 == 0) {
    uVar7 = 0;
  }
  else {
    uVar7 = 0;
    do {
      uVar6 = uVar7;
      uVar7 = uVar6 + 1;
      *(char *)((long)&local_50.tv_sec + uVar6) = "0289PYLQGRJCUV"[uVar12 % 0xe];
      if (uVar12 < 0xe) break;
      uVar12 = uVar12 / 0xe;
    } while (uVar7 < 0x17);
    if (uVar7 < 8) {
      uVar9 = 0;
      uVar10 = 1;
      uVar13 = uVar7;
    }
    else {
      if (uVar7 < 0x10) {
        uVar12 = 0;
      }
      else {
        pauVar11 = (undefined1 (*) [16])((long)&local_50.tv_sec + uVar6 + 1);
        uVar9 = uVar7 & 0x7ffffffffffffff0;
        uVar12 = uVar7 & 0xfffffffffffffff0;
        puVar14 = local_80;
        do {
          pauVar11 = pauVar11 + -1;
          uVar12 = uVar12 - 0x10;
          auVar17 = NEON_rev64(*pauVar11,1);
          auVar17 = NEON_ext(auVar17,auVar17,8,1);
          puVar14[1] = auVar17._8_8_;
          *puVar14 = auVar17._0_8_;
          puVar14 = puVar14 + 2;
        } while (uVar12 != 0);
        if (uVar7 == uVar9) goto LAB_00177b08;
        uVar12 = uVar9;
        if (((uint)uVar7 >> 3 & 1) == 0) {
          uVar13 = uVar7 & 0xf;
          uVar10 = uVar9 | 1;
          goto LAB_00177ae0;
        }
      }
      lVar16 = uVar12 - (uVar7 & 0xfffffffffffffff8);
      uVar9 = uVar7 & 0x7ffffffffffffff8;
      puVar14 = (undefined8 *)((long)&local_50 + (uVar7 - uVar12));
      uVar10 = uVar9 | 1;
      uVar13 = uVar7 & 7;
      puVar15 = (undefined8 *)((long)local_80 + uVar12);
      do {
        puVar14 = puVar14 + -1;
        lVar16 = lVar16 + 8;
        uVar5 = NEON_rev64(*puVar14,1);
        *puVar15 = uVar5;
        puVar15 = puVar15 + 1;
      } while (lVar16 != 0);
      if (uVar7 == uVar9) goto LAB_00177b08;
    }
LAB_00177ae0:
    puVar8 = (undefined1 *)((long)&local_50.tv_sec + uVar13);
    do {
      puVar8 = puVar8 + -1;
      *(undefined1 *)((long)local_80 + uVar9) = *puVar8;
      uVar12 = uVar10 + 1;
      uVar9 = uVar10;
      uVar10 = uVar12;
    } while (uVar6 + 2 != uVar12);
  }
LAB_00177b08:
  *(undefined1 *)((long)local_80 + uVar7) = 0;
  local_68 = CONCAT44(local_68._4_4_,local_94);
  iVar3 = clock_gettime(0,&local_50);
  if (iVar3 == 0) {
    local_60 = (local_50.tv_sec * 1000 + (ulong)local_50.tv_nsec / 1000000) / 1000;
  }
  else {
    local_60 = 0;
  }
LAB_00177b70:
  pthread_mutex_lock((pthread_mutex_t *)&DAT_0022d08c);
  uRam000000000022d0c0 = local_80[1];
  _DAT_0022d0b8 = local_80[0];
  uRam000000000022d0d0 = local_68;
  _DAT_0022d0c8 = local_80[2];
  DAT_0022d0e0 = DAT_0022d0e0 + 1;
  _DAT_0022d0d8 = local_60;
  lStack_58 = DAT_0022d0e0;
  iVar3 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0022d08c);
  *puVar4 = uVar1;
  if (*(long *)(lVar2 + 0x28) != local_38) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar3);
  }
  return;
}

/* ===== FUN_00178390 @ 00178390 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_00178390(char *param_1,void *param_2)

{
  long lVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  int *piVar5;
  size_t sVar6;
  ssize_t sVar7;
  char *pcVar8;
  char *pcVar9;
  char *pcVar10;
  ulong uVar11;
  undefined4 uVar12;
  long lVar13;
  ulong uVar14;
  char *pcVar15;
  socklen_t local_50d0;
  int local_50cc;
  pthread_t pStack_50c8;
  undefined5 local_50c0;
  undefined3 uStack_50bb;
  undefined5 uStack_50b8;
  char acStack_10c0 [4096];
  sockaddr local_c0;
  pthread_attr_t local_b0;
  long local_70;
  
  lVar1 = tpidr_el0;
  uVar12 = 0;
  local_70 = *(long *)(lVar1 + 0x28);
  pcVar15 = param_1;
  if ((param_1 == (char *)0x0) || (param_2 == (void *)0x0)) goto LAB_00178490;
  uVar2 = socket(2,1,0);
  pcVar15 = (char *)(ulong)uVar2;
  if ((int)uVar2 < 0) {
    uVar12 = 0;
    goto LAB_00178490;
  }
  uVar3 = fcntl(uVar2,3,0);
  iVar4 = clock_gettime(1,(timespec *)&local_b0);
  if (iVar4 == 0) {
    lVar13 = local_b0._8_8_ / 1000000 + local_b0.__align * 1000;
  }
  else {
    lVar13 = 0;
  }
  if ((-1 < (int)uVar3) && (iVar4 = fcntl(uVar2,4,(ulong)(uVar3 | 0x800)), -1 < iVar4)) {
    local_c0.sa_data[6] = UNK_001128a8;
    local_c0.sa_data[7] = UNK_001128a9;
    local_c0.sa_data[8] = UNK_001128aa;
    local_c0.sa_data[9] = UNK_001128ab;
    local_c0.sa_data[10] = UNK_001128ac;
    local_c0.sa_data[0xb] = UNK_001128ad;
    local_c0.sa_data[0xc] = UNK_001128ae;
    local_c0.sa_data[0xd] = UNK_001128af;
    local_c0.sa_family = _DAT_001128a0;
    local_c0.sa_data[0] = UNK_001128a2;
    local_c0.sa_data[1] = UNK_001128a3;
    local_c0.sa_data[2] = UNK_001128a4;
    local_c0.sa_data[3] = UNK_001128a5;
    local_c0.sa_data[4] = UNK_001128a6;
    local_c0.sa_data[5] = UNK_001128a7;
    iVar4 = clock_gettime(1,(timespec *)&local_b0);
    if (iVar4 == 0) {
      uVar11 = lVar13 + 2000;
      do {
        uVar14 = local_b0._8_8_ / 1000000 + local_b0.__align * 1000;
        if (uVar14 == 0 || uVar11 <= uVar14) break;
        pthread_mutex_lock((pthread_mutex_t *)&DAT_0022d0e8);
        if (((DAT_0022d128 != 0) && (DAT_0022d118 != 0)) && (uVar14 < DAT_0022d118)) {
          local_c0.sa_data._2_4_ = DAT_0022d110;
          pthread_mutex_unlock((pthread_mutex_t *)&DAT_0022d0e8);
          iVar4 = connect(uVar2,&local_c0,0x10);
          if (((iVar4 == 0) || (piVar5 = (int *)__errno(), *piVar5 == 0x73)) &&
             (iVar4 = FUN_00179820(uVar2,4,uVar11), iVar4 != 0)) {
            local_50d0 = 4;
            local_50cc = 0;
            iVar4 = getsockopt(uVar2,1,4,&local_50cc,&local_50d0);
            uVar12 = 0;
            if (iVar4 != 0 || local_50cc != 0) goto LAB_00178488;
            sVar6 = strlen(param_1);
            uVar3 = snprintf(acStack_10c0,0x1000,
                             "POST /bsd/api/v1/get_proxy HTTP/1.1\r\nHost: plusapi.bsd.meowfox.net\r\nContent-Type: application/json\r\nContent-Length: %zu\r\nConnection: close\r\n\r\n%s"
                             ,sVar6,param_1);
            if (uVar3 < 0x1000) {
              if (uVar3 == 0) goto LAB_00178730;
              uVar14 = 0;
              goto LAB_001786a4;
            }
          }
          break;
        }
        if ((DAT_0022d130 & 1) == 0) {
          if (DAT_0022d120 <= uVar14) {
            iVar4 = pthread_attr_init(&local_b0);
            if (iVar4 == 0) {
              DAT_0022d130 = 1;
              iVar4 = pthread_attr_setdetachstate(&local_b0,1);
              if (iVar4 == 0) {
                iVar4 = pthread_create(&pStack_50c8,&local_b0,FUN_00179934,(void *)0x0);
                pthread_attr_destroy(&local_b0);
                if (iVar4 == 0) goto LAB_001784e8;
              }
              else {
                pthread_attr_destroy(&local_b0);
              }
            }
            DAT_0022d120 = uVar14 + 5000;
            if (0xffffffffffffec77 < uVar14) {
              DAT_0022d120 = 0xffffffffffffffff;
            }
            DAT_0022d130 = 0;
          }
          pthread_mutex_unlock((pthread_mutex_t *)&DAT_0022d0e8);
          break;
        }
LAB_001784e8:
        pthread_mutex_unlock((pthread_mutex_t *)&DAT_0022d0e8);
        local_b0._8_8_ = _UNK_00112bb8;
        local_b0.__align = (long)_DAT_00112bb0;
        nanosleep((timespec *)&local_b0,(timespec *)0x0);
        iVar4 = clock_gettime(1,(timespec *)&local_b0);
      } while (iVar4 == 0);
    }
  }
LAB_00178484:
  uVar12 = 0;
  goto LAB_00178488;
  while( true ) {
    sVar7 = send(uVar2,acStack_10c0 + uVar14,(long)(int)uVar3 - uVar14,0x4000);
    if (sVar7 < 0) {
      piVar5 = (int *)__errno();
      if ((*piVar5 != 4) && (*piVar5 != 0xb)) goto LAB_00178484;
    }
    else {
      if (sVar7 == 0) goto LAB_00178484;
      uVar14 = sVar7 + uVar14;
    }
    if ((ulong)(long)(int)uVar3 <= uVar14) break;
LAB_001786a4:
    iVar4 = FUN_00179820(uVar2,4,uVar11);
    if (iVar4 == 0) goto LAB_00178484;
  }
LAB_00178730:
  pcVar15 = (char *)0x0;
  do {
    iVar4 = FUN_00179820(uVar2,1,uVar11);
    if (iVar4 == 0) goto LAB_00178484;
    sVar7 = recv(uVar2,(char *)((long)&local_50c0 + (long)pcVar15),0x3fff - (long)pcVar15,0);
    if (sVar7 < 0) {
      piVar5 = (int *)__errno();
      if ((*piVar5 != 4) && (*piVar5 != 0xb)) goto LAB_00178484;
    }
    else {
      if (sVar7 == 0) break;
      pcVar15 = pcVar15 + sVar7;
      *(char *)((long)&local_50c0 + (long)pcVar15) = '\0';
      pcVar8 = strstr((char *)&local_50c0,"\r\n\r\n");
      if (pcVar8 != (char *)0x0) {
        pcVar9 = strstr((char *)&local_50c0,"Content-Length:");
        if (pcVar9 == (char *)0x0) {
          pcVar9 = strstr((char *)&local_50c0,"content-length:");
        }
        if ((pcVar9 != (char *)0x0) && (pcVar9 < pcVar8)) {
          uVar14 = strtoul(pcVar9 + 0xf,(char **)&local_b0.__align,10);
          if (((char *)local_b0.__align == pcVar9 + 0xf) || (15000 < uVar14)) goto LAB_00178484;
          if (pcVar8 + (uVar14 - (long)&local_50c0) + 4 <= pcVar15) break;
        }
      }
    }
  } while (pcVar15 + 1 < (char *)0x4000);
  uVar12 = 0;
  pcVar8 = (char *)((long)&local_50c0 + (long)pcVar15);
  *pcVar8 = '\0';
  if ((char *)0xffffffffffffbfff < pcVar15 + -0x3fff) {
    if (((CONCAT35(uStack_50bb,local_50c0) == 0x312e312f50545448 &&
          CONCAT53(uStack_50b8,uStack_50bb) == 0x2030303220312e31) ||
        (CONCAT35(uStack_50bb,local_50c0) == 0x302e312f50545448 &&
         CONCAT53(uStack_50b8,uStack_50bb) == 0x2030303220302e31)) &&
       (pcVar9 = strstr((char *)&local_50c0,"\r\n\r\n"), pcVar9 != (char *)0x0)) {
      pcVar9 = pcVar9 + 4;
      pcVar10 = strstr((char *)&local_50c0,"Transfer-Encoding: chunked");
      if (pcVar10 == (char *)0x0) {
        pcVar10 = strstr((char *)&local_50c0,"transfer-encoding: chunked");
      }
      if ((pcVar10 == (char *)0x0) || (pcVar9 <= pcVar10)) {
        pcVar15 = (char *)((long)&local_50c0 + (long)(pcVar15 + -(long)pcVar9));
        if ((ulong)pcVar15 >> 7 < 0x7d) {
          memcpy(param_2,pcVar9,(size_t)pcVar15);
          uVar12 = 1;
          *(char *)((long)param_2 + (long)pcVar15) = '\0';
          goto LAB_00178488;
        }
      }
      else {
        pcVar15 = strstr(pcVar9,"\r\n");
        if ((pcVar15 != (char *)0x0) && (pcVar15 < pcVar8)) {
          piVar5 = (int *)__errno();
          lVar13 = 0;
          while( true ) {
            *piVar5 = 0;
            uVar11 = strtoul(pcVar9,(char **)&local_b0.__align,0x10);
            if (((*piVar5 != 0) || ((char *)local_b0.__align == pcVar9)) ||
               ((char *)local_b0.__align != pcVar15)) break;
            if (uVar11 == 0) {
              uVar12 = 1;
              *(undefined1 *)((long)param_2 + lVar13) = 0;
              goto LAB_00178488;
            }
            if (16000U - lVar13 <= uVar11) break;
            pcVar15 = pcVar15 + 2;
            if ((((ulong)((long)pcVar8 - (long)pcVar15) < uVar11) ||
                ((long)(((long)pcVar8 - (long)pcVar15) - uVar11) < 2)) ||
               ((pcVar15[uVar11] != '\r' || (pcVar15[uVar11 + 1] != '\n')))) break;
            memcpy((void *)((long)param_2 + lVar13),pcVar15,uVar11);
            pcVar9 = pcVar15 + uVar11 + 2;
            pcVar15 = strstr(pcVar9,"\r\n");
            if ((pcVar15 == (char *)0x0) || (lVar13 = uVar11 + lVar13, pcVar8 <= pcVar15)) break;
          }
        }
      }
    }
    goto LAB_00178484;
  }
LAB_00178488:
  uVar2 = close(uVar2);
  pcVar15 = (char *)(ulong)uVar2;
LAB_00178490:
  if (*(long *)(lVar1 + 0x28) != local_70) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(pcVar15);
  }
  return uVar12;
}

/* ===== FUN_00179568 @ 00179568 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00179568(long param_1,ulong param_2)

{
  long lVar1;
  undefined1 auVar2 [16];
  uint3 uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  undefined8 uVar9;
  undefined8 uVar10;
  undefined8 uVar11;
  undefined8 uVar12;
  undefined8 uVar13;
  undefined8 uVar14;
  undefined8 uVar15;
  int iVar16;
  ulong uVar17;
  ulong uVar18;
  ulong uVar19;
  ulong uVar20;
  byte *pbVar21;
  undefined1 (*pauVar22) [16];
  long lVar23;
  undefined8 *puVar24;
  byte *pbVar25;
  int iVar26;
  int iVar27;
  int iVar28;
  int iVar29;
  int iVar30;
  byte bVar31;
  int iVar47;
  undefined8 uVar32;
  byte bVar43;
  byte bVar44;
  byte bVar45;
  byte bVar46;
  byte bVar48;
  byte bVar49;
  byte bVar50;
  byte bVar51;
  byte bVar52;
  byte bVar53;
  byte bVar54;
  undefined1 auVar33 [12];
  undefined1 auVar34 [12];
  undefined1 auVar36 [12];
  byte bVar55;
  byte bVar56;
  byte bVar57;
  byte bVar58;
  undefined1 auVar39 [16];
  undefined1 auVar35 [12];
  undefined1 auVar41 [16];
  undefined1 auVar42 [16];
  uint uVar59;
  int iVar60;
  undefined8 uVar63;
  byte bVar88;
  uint uVar83;
  int iVar84;
  int iVar90;
  undefined1 auVar65 [12];
  int iVar91;
  undefined1 auVar72 [16];
  int iVar61;
  int iVar62;
  undefined1 auVar73 [16];
  undefined1 auVar66 [12];
  undefined1 auVar67 [12];
  int iVar85;
  undefined1 auVar68 [12];
  undefined1 auVar69 [12];
  undefined1 auVar78 [16];
  int iVar92;
  int iVar96;
  int iVar97;
  int iVar98;
  undefined1 auVar93 [16];
  undefined1 auVar94 [16];
  undefined1 auVar95 [16];
  byte bVar99;
  byte bVar100;
  byte bVar101;
  byte bVar102;
  byte bVar103;
  byte bVar104;
  byte bVar105;
  byte bVar106;
  byte bVar107;
  byte bVar108;
  byte bVar109;
  byte bVar110;
  int iVar111;
  int iVar112;
  int iVar113;
  int iVar114;
  undefined1 auVar115 [16];
  int iVar117;
  undefined1 auVar116 [16];
  int iVar118;
  byte local_58 [19];
  undefined1 uStack_45;
  undefined1 uStack_44;
  undefined1 uStack_43;
  undefined1 uStack_42;
  undefined1 uStack_41;
  undefined1 uStack_40;
  undefined1 uStack_3f;
  undefined1 uStack_3e;
  undefined1 uStack_3d;
  undefined1 uStack_3c;
  undefined1 uStack_3b;
  undefined1 uStack_3a;
  undefined1 uStack_39;
  undefined1 uStack_38;
  undefined1 uStack_37;
  undefined1 uStack_36;
  undefined1 uStack_35;
  undefined1 uStack_34;
  undefined1 uStack_33;
  undefined1 uStack_32;
  undefined1 uStack_31;
  undefined1 uStack_30;
  undefined1 uStack_2f;
  undefined1 uStack_2e;
  undefined1 uStack_2d;
  undefined1 uStack_2c;
  undefined1 uStack_2b;
  undefined1 uStack_2a;
  undefined1 uStack_29;
  undefined1 uStack_28;
  undefined1 uStack_27;
  undefined1 uStack_26;
  undefined1 uStack_25;
  undefined1 uStack_24;
  undefined1 uStack_23;
  undefined1 uStack_22;
  undefined1 uStack_21;
  undefined1 uStack_20;
  undefined1 uStack_1f;
  undefined1 uStack_1e;
  undefined1 uStack_1d;
  undefined1 uStack_1c;
  undefined1 uStack_1b;
  undefined1 uStack_1a;
  undefined1 uStack_19;
  long local_18;
  undefined1 auVar37 [16];
  undefined1 auVar38 [16];
  undefined1 auVar40 [16];
  undefined1 auVar64 [12];
  undefined1 auVar70 [16];
  undefined1 auVar71 [16];
  undefined1 auVar74 [16];
  undefined1 auVar75 [16];
  undefined1 auVar76 [16];
  undefined1 auVar77 [16];
  byte bVar79;
  byte bVar80;
  byte bVar81;
  byte bVar82;
  byte bVar86;
  byte bVar87;
  byte bVar89;
  
  uVar15 = _UNK_00112c08;
  uVar14 = _DAT_00112c00;
  uVar13 = _UNK_00112bf8;
  uVar12 = _DAT_00112bf0;
  uVar11 = _UNK_00112b88;
  uVar10 = s_expand_32_byte_kff__fff__00112ab0._8_8_;
  uVar9 = s_expand_32_byte_kff__fff__00112ab0._0_8_;
  lVar1 = tpidr_el0;
  local_18 = *(long *)(lVar1 + 0x28);
  if (param_2 != 0) {
    iVar16 = 0;
    uVar17 = 0;
    uVar32 = _DAT_00112b80;
    do {
      iVar30 = 10;
      auVar39._8_8_ = uVar11;
      auVar39._0_8_ = uVar32;
      auVar94._8_8_ = uVar10;
      auVar94._0_8_ = uVar9;
      auVar73._8_8_ = uVar15;
      auVar73._0_8_ = uVar14;
      auVar115._8_8_ = uVar13;
      auVar115._0_8_ = uVar12;
      do {
        iVar92 = auVar94._0_4_ + auVar73._0_4_;
        iVar96 = auVar94._4_4_ + auVar73._4_4_;
        iVar97 = auVar94._8_4_ + auVar73._8_4_;
        iVar98 = auVar94._12_4_ + auVar73._12_4_;
        iVar30 = iVar30 + -1;
        bVar31 = auVar39[0] ^ (byte)iVar92;
        bVar43 = auVar39[1] ^ (byte)((uint)iVar92 >> 8);
        bVar44 = auVar39[2] ^ (byte)((uint)iVar92 >> 0x10);
        bVar45 = auVar39[3] ^ (byte)((uint)iVar92 >> 0x18);
        bVar46 = auVar39[4] ^ (byte)iVar96;
        bVar48 = auVar39[5] ^ (byte)((uint)iVar96 >> 8);
        bVar49 = auVar39[6] ^ (byte)((uint)iVar96 >> 0x10);
        bVar50 = auVar39[7] ^ (byte)((uint)iVar96 >> 0x18);
        bVar51 = auVar39[8] ^ (byte)iVar97;
        bVar52 = auVar39[9] ^ (byte)((uint)iVar97 >> 8);
        bVar53 = auVar39[10] ^ (byte)((uint)iVar97 >> 0x10);
        bVar54 = auVar39[0xb] ^ (byte)((uint)iVar97 >> 0x18);
        bVar55 = auVar39[0xc] ^ (byte)iVar98;
        bVar56 = auVar39[0xd] ^ (byte)((uint)iVar98 >> 8);
        bVar57 = auVar39[0xe] ^ (byte)((uint)iVar98 >> 0x10);
        bVar58 = auVar39[0xf] ^ (byte)((uint)iVar98 >> 0x18);
        iVar61 = CONCAT13(bVar43,CONCAT12(bVar31,CONCAT11(bVar45,bVar44)));
        uVar63 = CONCAT17(bVar48,CONCAT16(bVar46,CONCAT15(bVar50,CONCAT14(bVar49,iVar61))));
        auVar33[8] = bVar53;
        auVar33._0_8_ = uVar63;
        auVar33[9] = bVar54;
        auVar33[10] = bVar51;
        auVar33[0xb] = bVar52;
        auVar37[0xc] = bVar57;
        auVar37._0_12_ = auVar33;
        auVar37[0xd] = bVar58;
        auVar37[0xe] = bVar55;
        auVar37[0xf] = bVar56;
        iVar61 = auVar115._0_4_ + iVar61;
        iVar4 = auVar115._4_4_ + (int)((ulong)uVar63 >> 0x20);
        iVar62 = auVar115._8_4_ + auVar33._8_4_;
        iVar85 = auVar115._12_4_ + auVar37._12_4_;
        bVar80 = (byte)((uint)iVar61 >> 0x18) ^ auVar73[3];
        uVar59 = CONCAT13(bVar80,CONCAT12((byte)((uint)iVar61 >> 0x10) ^ auVar73[2],
                                          CONCAT11((byte)((uint)iVar61 >> 8) ^ auVar73[1],
                                                   (byte)iVar61 ^ auVar73[0])));
        bVar88 = (byte)((uint)iVar4 >> 0x18) ^ auVar73[7];
        auVar64._0_8_ =
             CONCAT17(bVar88,CONCAT16((byte)((uint)iVar4 >> 0x10) ^ auVar73[6],
                                      CONCAT15((byte)((uint)iVar4 >> 8) ^ auVar73[5],
                                               CONCAT14((byte)iVar4 ^ auVar73[4],uVar59))));
        auVar64[8] = (byte)iVar62 ^ auVar73[8];
        auVar64[9] = (byte)((uint)iVar62 >> 8) ^ auVar73[9];
        auVar64[10] = (byte)((uint)iVar62 >> 0x10) ^ auVar73[10];
        auVar64[0xb] = (byte)((uint)iVar62 >> 0x18) ^ auVar73[0xb];
        auVar70[0xc] = (byte)iVar85 ^ auVar73[0xc];
        auVar70._0_12_ = auVar64;
        auVar70[0xd] = (byte)((uint)iVar85 >> 8) ^ auVar73[0xd];
        auVar70[0xe] = (byte)((uint)iVar85 >> 0x10) ^ auVar73[0xe];
        auVar70[0xf] = (byte)((uint)iVar85 >> 0x18) ^ auVar73[0xf];
        uVar83 = (uint)((ulong)auVar64._0_8_ >> 0x20);
        iVar60 = uVar59 << 0xc;
        iVar84 = uVar83 << 0xc;
        iVar90 = auVar64._8_4_ << 0xc;
        iVar91 = auVar70._12_4_ << 0xc;
        bVar87 = (byte)(uVar59 >> 0x14);
        bVar80 = (byte)((uint)iVar60 >> 8) | bVar80 >> 4;
        bVar79 = (byte)((uint)iVar60 >> 0x10);
        bVar81 = (byte)((uint)iVar60 >> 0x18);
        iVar60 = CONCAT13(bVar81,CONCAT12(bVar79,CONCAT11(bVar80,bVar87)));
        bVar89 = (byte)(uVar83 >> 0x14);
        bVar88 = (byte)((uint)iVar84 >> 8) | bVar88 >> 4;
        bVar82 = (byte)((uint)iVar84 >> 0x10);
        bVar86 = (byte)((uint)iVar84 >> 0x18);
        uVar63 = CONCAT17(bVar86,CONCAT16(bVar82,CONCAT15(bVar88,CONCAT14(bVar89,iVar60))));
        bVar99 = (byte)(auVar64._8_4_ >> 0x14);
        auVar65[8] = bVar99;
        auVar65._0_8_ = uVar63;
        auVar65[9] = (byte)((uint)iVar90 >> 8) | auVar64[0xb] >> 4;
        auVar65[10] = (byte)((uint)iVar90 >> 0x10);
        auVar65[0xb] = (byte)((uint)iVar90 >> 0x18);
        bVar100 = (byte)(auVar70._12_4_ >> 0x14);
        auVar71[0xc] = bVar100;
        auVar71._0_12_ = auVar65;
        auVar71[0xd] = (byte)((uint)iVar91 >> 8) | auVar70[0xf] >> 4;
        auVar71[0xe] = (byte)((uint)iVar91 >> 0x10);
        auVar71[0xf] = (byte)((uint)iVar91 >> 0x18);
        auVar93._0_4_ = iVar60 + iVar92;
        auVar93._4_4_ = (int)((ulong)uVar63 >> 0x20) + iVar96;
        auVar93._8_4_ = auVar65._8_4_ + iVar97;
        auVar93._12_4_ = auVar71._12_4_ + iVar98;
        auVar94 = NEON_ext(auVar93,auVar93,8,1);
        iVar60 = CONCAT13((byte)((uint)auVar93._0_4_ >> 0x10) ^ bVar31,
                          CONCAT12((byte)((uint)auVar93._0_4_ >> 8) ^ bVar45,
                                   CONCAT11((byte)auVar93._0_4_ ^ bVar44,
                                            (byte)((uint)auVar93._0_4_ >> 0x18) ^ bVar43)));
        uVar63 = CONCAT17((byte)((uint)auVar93._4_4_ >> 0x10) ^ bVar46,
                          CONCAT16((byte)((uint)auVar93._4_4_ >> 8) ^ bVar50,
                                   CONCAT15((byte)auVar93._4_4_ ^ bVar49,
                                            CONCAT14((byte)((uint)auVar93._4_4_ >> 0x18) ^ bVar48,
                                                     iVar60))));
        auVar34[8] = (byte)((uint)auVar93._8_4_ >> 0x18) ^ bVar52;
        auVar34._0_8_ = uVar63;
        auVar34[9] = (byte)auVar93._8_4_ ^ bVar53;
        auVar34[10] = (byte)((uint)auVar93._8_4_ >> 8) ^ bVar54;
        auVar34[0xb] = (byte)((uint)auVar93._8_4_ >> 0x10) ^ bVar51;
        auVar38[0xc] = (byte)((uint)auVar93._12_4_ >> 0x18) ^ bVar56;
        auVar38._0_12_ = auVar34;
        auVar38[0xd] = (byte)auVar93._12_4_ ^ bVar57;
        auVar38[0xe] = (byte)((uint)auVar93._12_4_ >> 8) ^ bVar58;
        auVar38[0xf] = (byte)((uint)auVar93._12_4_ >> 0x10) ^ bVar55;
        iVar60 = iVar60 + iVar61;
        iVar4 = (int)((ulong)uVar63 >> 0x20) + iVar4;
        iVar62 = auVar34._8_4_ + iVar62;
        iVar85 = auVar38._12_4_ + iVar85;
        auVar39 = NEON_ext(auVar38,auVar38,4,1);
        auVar72[0] = (byte)iVar60 ^ bVar87;
        auVar72[1] = (byte)((uint)iVar60 >> 8) ^ bVar80;
        auVar72[2] = (byte)((uint)iVar60 >> 0x10) ^ bVar79;
        auVar72[3] = (byte)((uint)iVar60 >> 0x18) ^ bVar81;
        auVar72[4] = (byte)iVar4 ^ bVar89;
        auVar72[5] = (byte)((uint)iVar4 >> 8) ^ bVar88;
        auVar72[6] = (byte)((uint)iVar4 >> 0x10) ^ bVar82;
        auVar72[7] = (byte)((uint)iVar4 >> 0x18) ^ bVar86;
        auVar72[8] = (byte)iVar62 ^ bVar99;
        auVar72[9] = (byte)((uint)iVar62 >> 8) ^ auVar65[9];
        auVar72[10] = (byte)((uint)iVar62 >> 0x10) ^ auVar65[10];
        auVar72[0xb] = (byte)((uint)iVar62 >> 0x18) ^ auVar65[0xb];
        auVar72[0xc] = (byte)iVar85 ^ bVar100;
        auVar72[0xd] = (byte)((uint)iVar85 >> 8) ^ auVar71[0xd];
        auVar72[0xe] = (byte)((uint)iVar85 >> 0x10) ^ auVar71[0xe];
        auVar72[0xf] = (byte)((uint)iVar85 >> 0x18) ^ auVar71[0xf];
        auVar73 = NEON_ext(auVar72,auVar72,0xc,1);
        iVar61 = auVar73._0_4_ << 7;
        iVar84 = auVar73._4_4_ << 7;
        iVar90 = auVar73._8_4_ << 7;
        iVar91 = auVar73._12_4_ << 7;
        bVar80 = (byte)iVar61 | auVar73[3] >> 1;
        bVar79 = (byte)((uint)iVar61 >> 8);
        bVar81 = (byte)((uint)iVar61 >> 0x10);
        bVar88 = (byte)((uint)iVar61 >> 0x18);
        iVar61 = CONCAT13(bVar88,CONCAT12(bVar81,CONCAT11(bVar79,bVar80)));
        bVar82 = (byte)iVar84 | auVar73[7] >> 1;
        bVar86 = (byte)((uint)iVar84 >> 8);
        bVar87 = (byte)((uint)iVar84 >> 0x10);
        bVar89 = (byte)((uint)iVar84 >> 0x18);
        uVar63 = CONCAT17(bVar89,CONCAT16(bVar87,CONCAT15(bVar86,CONCAT14(bVar82,iVar61))));
        auVar66[8] = (byte)iVar90 | auVar73[0xb] >> 1;
        auVar66._0_8_ = uVar63;
        auVar66[9] = (byte)((uint)iVar90 >> 8);
        auVar66[10] = (byte)((uint)iVar90 >> 0x10);
        auVar66[0xb] = (byte)((uint)iVar90 >> 0x18);
        auVar74[0xc] = (byte)iVar91 | auVar73[0xf] >> 1;
        auVar74._0_12_ = auVar66;
        auVar74[0xd] = (byte)((uint)iVar91 >> 8);
        auVar74[0xe] = (byte)((uint)iVar91 >> 0x10);
        auVar74[0xf] = (byte)((uint)iVar91 >> 0x18);
        iVar61 = auVar94._0_4_ + iVar61;
        iVar91 = auVar94._4_4_ + (int)((ulong)uVar63 >> 0x20);
        iVar92 = auVar94._8_4_ + auVar66._8_4_;
        iVar96 = auVar94._12_4_ + auVar74._12_4_;
        bVar31 = auVar39[0] ^ (byte)iVar61;
        bVar43 = auVar39[1] ^ (byte)((uint)iVar61 >> 8);
        bVar44 = auVar39[2] ^ (byte)((uint)iVar61 >> 0x10);
        bVar45 = auVar39[3] ^ (byte)((uint)iVar61 >> 0x18);
        bVar46 = auVar39[4] ^ (byte)iVar91;
        bVar48 = auVar39[5] ^ (byte)((uint)iVar91 >> 8);
        bVar49 = auVar39[6] ^ (byte)((uint)iVar91 >> 0x10);
        bVar50 = auVar39[7] ^ (byte)((uint)iVar91 >> 0x18);
        bVar51 = auVar39[8] ^ (byte)iVar92;
        bVar52 = auVar39[9] ^ (byte)((uint)iVar92 >> 8);
        bVar53 = auVar39[10] ^ (byte)((uint)iVar92 >> 0x10);
        bVar54 = auVar39[0xb] ^ (byte)((uint)iVar92 >> 0x18);
        bVar55 = auVar39[0xc] ^ (byte)iVar96;
        bVar56 = auVar39[0xd] ^ (byte)((uint)iVar96 >> 8);
        bVar57 = auVar39[0xe] ^ (byte)((uint)iVar96 >> 0x10);
        bVar58 = auVar39[0xf] ^ (byte)((uint)iVar96 >> 0x18);
        iVar84 = CONCAT13(bVar43,CONCAT12(bVar31,CONCAT11(bVar45,bVar44)));
        uVar63 = CONCAT17(bVar48,CONCAT16(bVar46,CONCAT15(bVar50,CONCAT14(bVar49,iVar84))));
        auVar35[8] = bVar53;
        auVar35._0_8_ = uVar63;
        auVar35[9] = bVar54;
        auVar35[10] = bVar51;
        auVar35[0xb] = bVar52;
        auVar40[0xc] = bVar57;
        auVar40._0_12_ = auVar35;
        auVar40[0xd] = bVar58;
        auVar40[0xe] = bVar55;
        auVar40[0xf] = bVar56;
        iVar60 = iVar60 + iVar84;
        iVar4 = iVar4 + (int)((ulong)uVar63 >> 0x20);
        bVar99 = (byte)iVar4;
        bVar100 = (byte)((uint)iVar4 >> 8);
        bVar101 = (byte)((uint)iVar4 >> 0x10);
        bVar102 = (byte)((uint)iVar4 >> 0x18);
        iVar62 = iVar62 + auVar35._8_4_;
        bVar103 = (byte)iVar62;
        bVar104 = (byte)((uint)iVar62 >> 8);
        bVar105 = (byte)((uint)iVar62 >> 0x10);
        bVar106 = (byte)((uint)iVar62 >> 0x18);
        iVar85 = iVar85 + auVar40._12_4_;
        bVar107 = (byte)iVar85;
        bVar108 = (byte)((uint)iVar85 >> 8);
        bVar109 = (byte)((uint)iVar85 >> 0x10);
        bVar110 = (byte)((uint)iVar85 >> 0x18);
        bVar88 = (byte)((uint)iVar60 >> 0x18) ^ bVar88;
        uVar59 = CONCAT13(bVar88,CONCAT12((byte)((uint)iVar60 >> 0x10) ^ bVar81,
                                          CONCAT11((byte)((uint)iVar60 >> 8) ^ bVar79,
                                                   (byte)iVar60 ^ bVar80)));
        bVar89 = bVar102 ^ bVar89;
        auVar67._0_8_ =
             CONCAT17(bVar89,CONCAT16(bVar101 ^ bVar87,
                                      CONCAT15(bVar100 ^ bVar86,CONCAT14(bVar99 ^ bVar82,uVar59))));
        auVar67[8] = bVar103 ^ auVar66[8];
        auVar67[9] = bVar104 ^ auVar66[9];
        auVar67[10] = bVar105 ^ auVar66[10];
        auVar67[0xb] = bVar106 ^ auVar66[0xb];
        auVar75[0xc] = bVar107 ^ auVar74[0xc];
        auVar75._0_12_ = auVar67;
        auVar75[0xd] = bVar108 ^ auVar74[0xd];
        auVar75[0xe] = bVar109 ^ auVar74[0xe];
        auVar75[0xf] = bVar110 ^ auVar74[0xf];
        uVar83 = (uint)((ulong)auVar67._0_8_ >> 0x20);
        iVar62 = uVar59 << 0xc;
        iVar85 = uVar83 << 0xc;
        iVar84 = auVar67._8_4_ << 0xc;
        iVar90 = auVar75._12_4_ << 0xc;
        auVar116[4] = bVar99;
        auVar116._0_4_ = iVar60;
        auVar116[5] = bVar100;
        auVar116[6] = bVar101;
        auVar116[7] = bVar102;
        auVar116[8] = bVar103;
        auVar116[9] = bVar104;
        auVar116[10] = bVar105;
        auVar116[0xb] = bVar106;
        auVar116[0xc] = bVar107;
        auVar116[0xd] = bVar108;
        auVar116[0xe] = bVar109;
        auVar116[0xf] = bVar110;
        auVar2[4] = bVar99;
        auVar2._0_4_ = iVar60;
        auVar2[5] = bVar100;
        auVar2[6] = bVar101;
        auVar2[7] = bVar102;
        auVar2[8] = bVar103;
        auVar2[9] = bVar104;
        auVar2[10] = bVar105;
        auVar2[0xb] = bVar106;
        auVar2[0xc] = bVar107;
        auVar2[0xd] = bVar108;
        auVar2[0xe] = bVar109;
        auVar2[0xf] = bVar110;
        auVar115 = NEON_ext(auVar116,auVar2,8,1);
        bVar87 = (byte)(uVar59 >> 0x14);
        bVar80 = (byte)((uint)iVar62 >> 8) | bVar88 >> 4;
        bVar79 = (byte)((uint)iVar62 >> 0x10);
        bVar81 = (byte)((uint)iVar62 >> 0x18);
        iVar62 = CONCAT13(bVar81,CONCAT12(bVar79,CONCAT11(bVar80,bVar87)));
        bVar99 = (byte)(uVar83 >> 0x14);
        bVar88 = (byte)((uint)iVar85 >> 8) | bVar89 >> 4;
        bVar82 = (byte)((uint)iVar85 >> 0x10);
        bVar86 = (byte)((uint)iVar85 >> 0x18);
        uVar63 = CONCAT17(bVar86,CONCAT16(bVar82,CONCAT15(bVar88,CONCAT14(bVar99,iVar62))));
        bVar89 = (byte)(auVar67._8_4_ >> 0x14);
        auVar68[8] = bVar89;
        auVar68._0_8_ = uVar63;
        auVar68[9] = (byte)((uint)iVar84 >> 8) | auVar67[0xb] >> 4;
        auVar68[10] = (byte)((uint)iVar84 >> 0x10);
        auVar68[0xb] = (byte)((uint)iVar84 >> 0x18);
        bVar100 = (byte)(auVar75._12_4_ >> 0x14);
        auVar76[0xc] = bVar100;
        auVar76._0_12_ = auVar68;
        auVar76[0xd] = (byte)((uint)iVar90 >> 8) | auVar75[0xf] >> 4;
        auVar76[0xe] = (byte)((uint)iVar90 >> 0x10);
        auVar76[0xf] = (byte)((uint)iVar90 >> 0x18);
        auVar95._0_4_ = iVar62 + iVar61;
        auVar95._4_4_ = (int)((ulong)uVar63 >> 0x20) + iVar91;
        auVar95._8_4_ = auVar68._8_4_ + iVar92;
        auVar95._12_4_ = auVar76._12_4_ + iVar96;
        auVar41[0] = (byte)auVar95._0_4_ ^ bVar44;
        auVar41[1] = (byte)((uint)auVar95._0_4_ >> 8) ^ bVar45;
        auVar41[2] = (byte)((uint)auVar95._0_4_ >> 0x10) ^ bVar31;
        auVar41[3] = (byte)((uint)auVar95._0_4_ >> 0x18) ^ bVar43;
        auVar41[4] = (byte)auVar95._4_4_ ^ bVar49;
        auVar41[5] = (byte)((uint)auVar95._4_4_ >> 8) ^ bVar50;
        auVar41[6] = (byte)((uint)auVar95._4_4_ >> 0x10) ^ bVar46;
        auVar41[7] = (byte)((uint)auVar95._4_4_ >> 0x18) ^ bVar48;
        auVar41[8] = (byte)auVar95._8_4_ ^ bVar53;
        auVar41[9] = (byte)((uint)auVar95._8_4_ >> 8) ^ bVar54;
        auVar41[10] = (byte)((uint)auVar95._8_4_ >> 0x10) ^ bVar51;
        auVar41[0xb] = (byte)((uint)auVar95._8_4_ >> 0x18) ^ bVar52;
        auVar41[0xc] = (byte)auVar95._12_4_ ^ bVar57;
        auVar41[0xd] = (byte)((uint)auVar95._12_4_ >> 8) ^ bVar58;
        auVar41[0xe] = (byte)((uint)auVar95._12_4_ >> 0x10) ^ bVar55;
        auVar41[0xf] = (byte)((uint)auVar95._12_4_ >> 0x18) ^ bVar56;
        auVar73 = NEON_ext(auVar41,auVar41,0xc,1);
        auVar94 = NEON_ext(auVar95,auVar95,8,1);
        iVar61 = CONCAT13(auVar73[2],CONCAT12(auVar73[1],CONCAT11(auVar73[0],auVar73[3])));
        uVar63 = CONCAT17(auVar73[6],
                          CONCAT16(auVar73[5],CONCAT15(auVar73[4],CONCAT14(auVar73[7],iVar61))));
        auVar36[8] = auVar73[0xb];
        auVar36._0_8_ = uVar63;
        auVar36[9] = auVar73[8];
        auVar36[10] = auVar73[9];
        auVar36[0xb] = auVar73[10];
        auVar39[0xc] = auVar73[0xf];
        auVar39._0_12_ = auVar36;
        auVar39[0xd] = auVar73[0xc];
        auVar39[0xe] = auVar73[0xd];
        auVar39[0xf] = auVar73[0xe];
        auVar73 = NEON_ext(auVar39,auVar39,4,1);
        auVar116 = NEON_ext(auVar39,auVar39,0xc,1);
        iVar60 = auVar73._0_4_ + iVar60;
        iVar4 = auVar73._4_4_ + iVar4;
        bVar43 = (byte)((uint)iVar4 >> 8);
        bVar44 = (byte)((uint)iVar4 >> 0x10);
        bVar31 = (byte)((uint)iVar4 >> 0x18);
        iVar91 = auVar116._0_4_ + auVar115._0_4_;
        iVar92 = auVar116._4_4_ + auVar115._4_4_;
        auVar115._12_4_ = iVar92;
        auVar115._8_4_ = iVar91;
        auVar115[4] = (byte)iVar4;
        auVar115._0_4_ = iVar60;
        auVar115[5] = bVar43;
        auVar115[6] = bVar44;
        auVar115[7] = bVar31;
        bVar81 = (byte)((uint)iVar60 >> 0x18) ^ bVar81;
        iVar62 = CONCAT13(bVar81,CONCAT12((byte)((uint)iVar60 >> 0x10) ^ bVar79,
                                          CONCAT11((byte)((uint)iVar60 >> 8) ^ bVar80,
                                                   (byte)iVar60 ^ bVar87)));
        bVar31 = bVar31 ^ bVar86;
        auVar69._0_8_ =
             CONCAT17(bVar31,CONCAT16(bVar44 ^ bVar82,
                                      CONCAT15(bVar43 ^ bVar88,CONCAT14((byte)iVar4 ^ bVar99,iVar62)
                                              )));
        auVar69[8] = (byte)iVar91 ^ bVar89;
        auVar69[9] = (byte)((uint)iVar91 >> 8) ^ auVar68[9];
        auVar69[10] = (byte)((uint)iVar91 >> 0x10) ^ auVar68[10];
        auVar69[0xb] = (byte)((uint)iVar91 >> 0x18) ^ auVar68[0xb];
        auVar77[0xc] = (byte)iVar92 ^ bVar100;
        auVar77._0_12_ = auVar69;
        auVar77[0xd] = (byte)((uint)iVar92 >> 8) ^ auVar76[0xd];
        auVar77[0xe] = (byte)((uint)iVar92 >> 0x10) ^ auVar76[0xe];
        auVar77[0xf] = (byte)((uint)iVar92 >> 0x18) ^ auVar76[0xf];
        iVar62 = iVar62 << 7;
        iVar85 = (int)((ulong)auVar69._0_8_ >> 0x20) << 7;
        iVar84 = auVar69._8_4_ << 7;
        iVar90 = auVar77._12_4_ << 7;
        auVar78[0] = (byte)iVar62 | bVar81 >> 1;
        auVar78[1] = (undefined1)((uint)iVar62 >> 8);
        auVar78[2] = (undefined1)((uint)iVar62 >> 0x10);
        auVar78[3] = (undefined1)((uint)iVar62 >> 0x18);
        auVar78[4] = (byte)iVar85 | bVar31 >> 1;
        auVar78[5] = (undefined1)((uint)iVar85 >> 8);
        auVar78[6] = (undefined1)((uint)iVar85 >> 0x10);
        auVar78[7] = (undefined1)((uint)iVar85 >> 0x18);
        auVar78[8] = (byte)iVar84 | auVar69[0xb] >> 1;
        auVar78[9] = (undefined1)((uint)iVar84 >> 8);
        auVar78[10] = (undefined1)((uint)iVar84 >> 0x10);
        auVar78[0xb] = (undefined1)((uint)iVar84 >> 0x18);
        auVar78[0xc] = (byte)iVar90 | auVar77[0xf] >> 1;
        auVar78[0xd] = (undefined1)((uint)iVar90 >> 8);
        auVar78[0xe] = (undefined1)((uint)iVar90 >> 0x10);
        auVar78[0xf] = (undefined1)((uint)iVar90 >> 0x18);
        auVar73 = NEON_ext(auVar78,auVar78,4,1);
      } while (iVar30 != 0);
      uVar18 = param_2 - uVar17;
      iVar85 = (int)uVar9 + auVar94._0_4_;
      iVar84 = SUB84(uVar9,4) + auVar94._4_4_;
      iVar90 = (int)uVar10 + auVar94._8_4_;
      iVar96 = SUB84(uVar10,4) + auVar94._12_4_;
      iVar97 = (int)uVar14 + auVar73._0_4_;
      iVar30 = (int)((ulong)uVar14 >> 0x20);
      iVar98 = iVar30 + auVar73._4_4_;
      iVar117 = (int)uVar15 + auVar73._8_4_;
      iVar62 = (int)((ulong)uVar15 >> 0x20);
      iVar118 = iVar62 + auVar73._12_4_;
      if (0x3f < uVar18) {
        uVar18 = 0x40;
      }
      iVar111 = (int)uVar32 + iVar61;
      iVar26 = (int)((ulong)uVar32 >> 0x20);
      iVar47 = (int)((ulong)uVar63 >> 0x20);
      iVar112 = iVar26 + iVar47;
      iVar113 = (int)uVar11 + auVar36._8_4_;
      iVar27 = (int)((ulong)uVar11 >> 0x20);
      iVar114 = iVar27 + auVar39._12_4_;
      iVar5 = (int)uVar12 + iVar60;
      iVar28 = (int)((ulong)uVar12 >> 0x20);
      iVar6 = iVar28 + iVar4;
      iVar7 = (int)uVar13 + iVar91;
      iVar29 = (int)((ulong)uVar13 >> 0x20);
      iVar8 = iVar29 + iVar92;
      uVar3 = CONCAT12((char)((uint)(iVar28 + iVar4) >> 0x10),
                       (short)((uint)((int)uVar12 + iVar60) >> 0x10)) & 0xff00ff;
      local_58[0] = (byte)iVar85;
      local_58[1] = (char)((uint)iVar85 >> 8);
      local_58[2] = (char)((uint)((int)uVar9 + auVar94._0_4_) >> 0x10);
      local_58[3] = (char)((uint)iVar85 >> 0x18);
      local_58[4] = (char)iVar84;
      local_58[5] = (char)((uint)iVar84 >> 8);
      local_58[6] = (char)((uint)(SUB84(uVar9,4) + auVar94._4_4_) >> 0x10);
      local_58[7] = (char)((uint)iVar84 >> 0x18);
      local_58[8] = (char)iVar90;
      local_58[9] = (char)((uint)iVar90 >> 8);
      local_58[10] = (char)((uint)((int)uVar10 + auVar94._8_4_) >> 0x10);
      local_58[0xb] = (char)((uint)iVar90 >> 0x18);
      local_58[0xc] = (char)iVar96;
      local_58[0xd] = (char)((uint)iVar96 >> 8);
      local_58[0xe] = (char)((uint)(SUB84(uVar10,4) + auVar94._12_4_) >> 0x10);
      local_58[0xf] = (char)((uint)iVar96 >> 0x18);
      local_58[0x10] = (char)iVar97;
      local_58[0x11] = (char)((uint)iVar97 >> 8);
      local_58[0x12] = (char)((uint)((int)uVar14 + auVar73._0_4_) >> 0x10);
      uStack_45 = (char)((uint)iVar97 >> 0x18);
      uStack_44 = (char)iVar98;
      uStack_43 = (char)((uint)iVar98 >> 8);
      uStack_42 = (char)((uint)(iVar30 + auVar73._4_4_) >> 0x10);
      uStack_41 = (char)((uint)iVar98 >> 0x18);
      uStack_40 = (char)iVar117;
      uStack_3f = (char)((uint)iVar117 >> 8);
      uStack_3e = (char)((uint)((int)uVar15 + auVar73._8_4_) >> 0x10);
      uStack_3d = (char)((uint)iVar117 >> 0x18);
      uStack_3c = (char)iVar118;
      uStack_3b = (char)((uint)iVar118 >> 8);
      uStack_3a = (char)((uint)(iVar62 + auVar73._12_4_) >> 0x10);
      uStack_39 = (char)((uint)iVar118 >> 0x18);
      uStack_38 = (char)iVar5;
      uStack_37 = (char)((uint)iVar5 >> 8);
      uStack_36 = (char)uVar3;
      uStack_35 = (char)((uint)iVar5 >> 0x18);
      uStack_34 = (char)iVar6;
      uStack_33 = (char)((uint)iVar6 >> 8);
      uStack_32 = (char)(uVar3 >> 0x10);
      uStack_31 = (char)((uint)iVar6 >> 0x18);
      uStack_30 = (char)iVar7;
      uStack_2f = (char)((uint)iVar7 >> 8);
      uStack_2e = (char)((uint)((int)uVar13 + iVar91) >> 0x10);
      uStack_2d = (char)((uint)iVar7 >> 0x18);
      uStack_2c = (char)iVar8;
      uStack_2b = (char)((uint)iVar8 >> 8);
      uStack_2a = (char)((uint)(iVar29 + iVar92) >> 0x10);
      uStack_29 = (char)((uint)iVar8 >> 0x18);
      uStack_28 = (char)iVar111;
      uStack_27 = (char)((uint)iVar111 >> 8);
      uStack_26 = (char)((uint)((int)uVar32 + iVar61) >> 0x10);
      uStack_25 = (char)((uint)iVar111 >> 0x18);
      uStack_24 = (char)iVar112;
      uStack_23 = (char)((uint)iVar112 >> 8);
      uStack_22 = (char)((uint)(iVar26 + iVar47) >> 0x10);
      uStack_21 = (char)((uint)iVar112 >> 0x18);
      uStack_20 = (char)iVar113;
      uStack_1f = (char)((uint)iVar113 >> 8);
      uStack_1e = (char)((uint)((int)uVar11 + auVar36._8_4_) >> 0x10);
      uStack_1d = (char)((uint)iVar113 >> 0x18);
      uStack_1c = (char)iVar114;
      uStack_1b = (char)((uint)iVar114 >> 8);
      uStack_1a = (char)((uint)(iVar27 + auVar39._12_4_) >> 0x10);
      uStack_19 = (char)((uint)iVar114 >> 0x18);
      if (uVar18 != 0) {
        if (uVar18 < 8) {
          uVar19 = 0;
        }
        else {
          if (uVar18 < 0x10) {
            uVar20 = 0;
          }
          else {
            uVar19 = uVar18 & 0x70;
            pauVar22 = (undefined1 (*) [16])(param_1 + uVar17);
            pbVar21 = local_58;
            uVar20 = uVar19;
            do {
              auVar39 = *pauVar22;
              uVar20 = uVar20 - 0x10;
              auVar42._0_8_ =
                   CONCAT17(auVar39[7] ^ pbVar21[7],
                            CONCAT16(auVar39[6] ^ pbVar21[6],
                                     CONCAT15(auVar39[5] ^ pbVar21[5],
                                              CONCAT14(auVar39[4] ^ pbVar21[4],
                                                       CONCAT13(auVar39[3] ^ pbVar21[3],
                                                                CONCAT12(auVar39[2] ^ pbVar21[2],
                                                                         CONCAT11(auVar39[1] ^
                                                                                  pbVar21[1],
                                                                                  auVar39[0] ^
                                                                                  *pbVar21)))))));
              auVar42[8] = auVar39[8] ^ pbVar21[8];
              auVar42[9] = auVar39[9] ^ pbVar21[9];
              auVar42[10] = auVar39[10] ^ pbVar21[10];
              auVar42[0xb] = auVar39[0xb] ^ pbVar21[0xb];
              auVar42[0xc] = auVar39[0xc] ^ pbVar21[0xc];
              auVar42[0xd] = auVar39[0xd] ^ pbVar21[0xd];
              auVar42[0xe] = auVar39[0xe] ^ pbVar21[0xe];
              auVar42[0xf] = auVar39[0xf] ^ pbVar21[0xf];
              *(long *)(*pauVar22 + 8) = auVar42._8_8_;
              *(undefined8 *)*pauVar22 = auVar42._0_8_;
              pauVar22 = pauVar22 + 1;
              pbVar21 = pbVar21 + 0x10;
            } while (uVar20 != 0);
            if (uVar18 == uVar19) goto LAB_001795b8;
            uVar20 = uVar19;
            if (((uint)uVar18 >> 3 & 1) == 0) goto LAB_001797d4;
          }
          uVar19 = uVar18 & 0x78;
          lVar23 = uVar20 - uVar19;
          pbVar21 = local_58 + uVar20;
          puVar24 = (undefined8 *)(param_1 + uVar20 + uVar17);
          do {
            uVar32 = *(undefined8 *)pbVar21;
            uVar63 = *puVar24;
            lVar23 = lVar23 + 8;
            *puVar24 = CONCAT17((byte)((ulong)uVar63 >> 0x38) ^ (byte)((ulong)uVar32 >> 0x38),
                                CONCAT16((byte)((ulong)uVar63 >> 0x30) ^
                                         (byte)((ulong)uVar32 >> 0x30),
                                         CONCAT15((byte)((ulong)uVar63 >> 0x28) ^
                                                  (byte)((ulong)uVar32 >> 0x28),
                                                  CONCAT14((byte)((ulong)uVar63 >> 0x20) ^
                                                           (byte)((ulong)uVar32 >> 0x20),
                                                           CONCAT13((byte)((ulong)uVar63 >> 0x18) ^
                                                                    (byte)((ulong)uVar32 >> 0x18),
                                                                    CONCAT12((byte)((ulong)uVar63 >>
                                                                                   0x10) ^
                                                                             (byte)((ulong)uVar32 >>
                                                                                   0x10),
                                                                             CONCAT11((byte)((ulong)
                                                  uVar63 >> 8) ^ (byte)((ulong)uVar32 >> 8),
                                                  (byte)uVar63 ^ (byte)uVar32)))))));
            pbVar21 = pbVar21 + 8;
            puVar24 = puVar24 + 1;
          } while (lVar23 != 0);
          if (uVar18 == uVar19) goto LAB_001795b8;
        }
LAB_001797d4:
        lVar23 = uVar18 - uVar19;
        pbVar21 = local_58 + uVar19;
        pbVar25 = (byte *)(param_1 + uVar19 + uVar17);
        do {
          lVar23 = lVar23 + -1;
          *pbVar25 = *pbVar25 ^ *pbVar21;
          pbVar21 = pbVar21 + 1;
          pbVar25 = pbVar25 + 1;
        } while (lVar23 != 0);
      }
LAB_001795b8:
      iVar16 = iVar16 + 1;
      uVar17 = uVar18 + uVar17;
      uVar32 = CONCAT44(iVar26,iVar16);
    } while (uVar17 < param_2);
  }
  if (*(long *)(lVar1 + 0x28) != local_18) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
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

/* ===== FUN_0017a048 @ 0017a048 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0017a048(void)

{
  uint uVar1;
  long lVar2;
  uint uVar3;
  long lVar4;
  undefined8 uVar5;
  long lVar6;
  int iVar7;
  uint *puVar8;
  size_t __n;
  char *pcVar9;
  int iVar10;
  uint uVar11;
  uint local_44;
  uint local_40;
  uint local_3c;
  long local_38;
  
  lVar6 = DAT_001e0978;
  lVar4 = tpidr_el0;
  local_38 = *(long *)(lVar4 + 0x28);
  local_3c = 0;
  puVar8 = (uint *)FUN_001419f8();
  if (((int)puVar8 == 0) ||
     (puVar8 = (uint *)FUN_0014b1d0(0xd06d98,0x5ac,
                                    "68feb4f4a33b0568cf9836c921863f42f30a2e87ddf099f79723d8fa6231776e"
                                   ), (int)puVar8 == 0)) goto LAB_0017a234;
  lVar2 = lVar6 + 0xd06d98;
  iVar7 = FUN_001428fc(puVar8,lVar2,&local_3c,4);
  puVar8 = (uint *)0x0;
  if ((iVar7 == 0) ||
     ((local_3c != 0xd103c3ff ||
      (puVar8 = (uint *)FUN_00152af0(lVar2), uVar5 = _DAT_00112b90, puVar8 == (uint *)0x0))))
  goto LAB_0017a234;
  if (((((uint)puVar8 | (uint)lVar6) & 3) == 0) &&
     ((0xfffffffff0000002 < (lVar6 - (long)puVar8) - 0x72f9265U &&
      (0xfffffffff0000002 < ((long)puVar8 - lVar6) - 0x8d06d85U)))) {
    iVar7 = (int)((long)puVar8 - lVar6);
    iVar10 = (int)(lVar6 - (long)puVar8);
    uVar1 = iVar10 + 0xd06d98;
    uVar11 = iVar7 - 0xd06d88;
    uVar3 = iVar10 + 0xd06d9b;
    if (-1 < (int)uVar1) {
      uVar3 = uVar1;
    }
    uVar1 = iVar7 - 0xd06d85;
    if (-1 < (int)uVar11) {
      uVar1 = uVar11;
    }
    local_40 = uVar1 >> 2 & 0x3ffffff | 0x14000000;
    *(undefined8 *)(puVar8 + 4) = _UNK_00112b98;
    *(undefined8 *)(puVar8 + 2) = uVar5;
    *puVar8 = local_3c;
    puVar8[1] = uVar3 >> 2 & 0x3ffffff | 0x14000000;
    *(code **)(puVar8 + 6) = FUN_0014b2ec;
    FUN_001bdaf0(puVar8,puVar8 + 8);
    iVar7 = mprotect(puVar8,DAT_00209d88,5);
    if (iVar7 == 0) {
      DAT_0020af70 = puVar8;
      iVar7 = FUN_0017a344();
      if (((iVar7 == 0) || (iVar7 = FUN_001419f8(), iVar7 == 0)) ||
         (iVar7 = FUN_0014b1d0(0xd06d98,0x5ac,
                               "68feb4f4a33b0568cf9836c921863f42f30a2e87ddf099f79723d8fa6231776e"),
         iVar7 == 0)) {
        pcVar9 = "install_rejected_before_entry";
      }
      else {
        __n = FUN_0014bef8(DAT_001cfb4c,&local_40,4,lVar2);
        if (__n == 4) {
          FUN_001bdaf0(lVar2,lVar6 + 0xd06d9c);
          iVar7 = FUN_0013b320();
          if (iVar7 != 0) {
            puVar8 = (uint *)0x1;
            goto LAB_0017a234;
          }
LAB_0017a284:
          puVar8 = &local_40;
        }
        else {
          if (2 < __n - 1) goto LAB_0017a284;
          puVar8 = &local_44;
          local_44 = local_3c;
          memcpy(&local_44,&local_40,__n);
        }
        iVar7 = FUN_0014bdb4(*puVar8);
        pcVar9 = "install_owner_changed";
        if (iVar7 != 0) {
          pcVar9 = "install_rolled_back";
        }
      }
      FUN_001417c8("script_port_fonts",pcVar9,0);
    }
  }
  puVar8 = (uint *)0x0;
LAB_0017a234:
  if (*(long *)(lVar4 + 0x28) != local_38) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(puVar8);
  }
  return;
}

/* ===== FUN_0017a2b8 @ 0017a2b8 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0017a2b8(char *param_1,uint param_2)

{
  char cVar1;
  bool bVar2;
  int iVar3;
  
  if (DAT_0022d158 != (code *)0x0) {
    (*DAT_0022d158)(param_1,param_2);
  }
  if (((param_1 != (char *)0x0) && (iVar3 = strcmp(param_1,"Font"), param_2 < 6)) && (iVar3 == 0)) {
    DAT_001e12d0._4_4_ = 1;
    DAT_001e12d8._4_4_ = 0;
    do {
      cVar1 = '\x01';
      bVar2 = (bool)ExclusiveMonitorPass(0x1cfaf8,0x10);
      if (bVar2) {
        cVar1 = ExclusiveMonitorsStatus();
        unique0x10000033 = ram0x001cfaf8 + 1;
      }
    } while (cVar1 != '\0');
  }
  return;
}

/* ===== FUN_0017a344 @ 0017a344 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_0017a344(void)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  undefined8 uVar4;
  int iVar5;
  int iVar6;
  
  if ((((DAT_0020af70 != 0) && ((((uint)DAT_001e0978 | (uint)DAT_0020af70) & 3) == 0)) &&
      (0xfffffffff0000002 < (DAT_001e0978 - DAT_0020af70) - 0x72f9265U)) &&
     (0xfffffffff0000002 < (DAT_0020af70 - DAT_001e0978) - 0x8d06d85U)) {
    iVar6 = (int)(DAT_001e0978 - DAT_0020af70);
    uVar1 = iVar6 + 0xd06d98;
    iVar5 = (int)(DAT_0020af70 - DAT_001e0978);
    uVar3 = iVar5 - 0xd06d88;
    uVar2 = iVar6 + 0xd06d9b;
    if (-1 < (int)uVar1) {
      uVar2 = uVar1;
    }
    uVar1 = iVar5 - 0xd06d85;
    if (-1 < (int)uVar3) {
      uVar1 = uVar3;
    }
    DAT_0020af7c = uVar2 >> 2 & 0x3ffffff | 0x14000000;
    DAT_0020af60 = uVar1 >> 2 & 0x3ffffff | 0x14000000;
    DAT_0020af78 = 0xd103c3ff;
    DAT_0020af88 = _UNK_00112b98;
    DAT_0020af80 = _DAT_00112b90;
    DAT_0020af68 = DAT_0020af70;
    DAT_0020af90 = FUN_0014b2ec;
    DAT_0020af64 = 1;
    uVar4 = FUN_0014a9a0();
    return uVar4;
  }
  return 0;
}

/* ===== FUN_0017a634 @ 0017a634 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

float FUN_0017a634(long param_1)

{
  undefined4 uVar1;
  char cVar2;
  bool bVar3;
  long lVar4;
  uint uVar5;
  int iVar6;
  float fVar7;
  undefined4 *puVar8;
  ulong uVar9;
  float *pfVar10;
  ulong uVar11;
  float fVar12;
  ulong local_50;
  long local_48;
  
  lVar4 = tpidr_el0;
  local_48 = *(long *)(lVar4 + 0x28);
  fVar12 = (float)(*DAT_0022d180)();
  puVar8 = (undefined4 *)__errno();
  uVar1 = *puVar8;
  local_50 = 0;
  if ((((((DAT_00209a78 & 1) == 0) || (param_1 == 0)) || ((int)DAT_001dfff0 == 0)) ||
      ((((iVar6 = FUN_001428fc(puVar8,param_1,&local_50,8), iVar6 == 0 ||
         (local_50 + 0x2000 < 0x12000)) ||
        (((local_50 & 7) != 0 || ((ABS(fVar12) == INFINITY || (NAN(ABS(fVar12)))))))) ||
       (fVar12 < 1.0 != NAN(fVar12))))) || (1000.0 < fVar12)) goto LAB_0017a68c;
  fVar7 = (float)(*(code *)(DAT_001e0978 + 0x748654))(param_1);
  do {
    uVar5 = _DAT_0022d190;
    cVar2 = '\x01';
    bVar3 = (bool)ExclusiveMonitorPass(0x22d190,0x10);
    if (bVar3) {
      _DAT_0022d190 = CONCAT31(DAT_0022d190_1,1);
      cVar2 = ExclusiveMonitorsStatus();
    }
  } while (cVar2 != '\0');
  if ((uVar5 & 1) != 0) goto LAB_0017a68c;
  if (((0xfffe795f < (int)fVar7 - 0x186a1U) && (fVar12 == 1000.0 || fVar12 < 1000.0 != NAN(fVar12)))
     && (1.0 <= fVar12)) {
    uVar9 = (ulong)DAT_0022d194;
    if (DAT_0022d194 == 0) {
      uVar9 = 0;
    }
    else {
      pfVar10 = (float *)&DAT_0022d19c;
      uVar11 = uVar9;
      do {
        if (pfVar10[-1] == fVar7) goto LAB_0017a7f8;
        pfVar10 = pfVar10 + 2;
        uVar11 = uVar11 - 1;
      } while (uVar11 != 0);
      if (0x1f < DAT_0022d194) goto LAB_0017a7fc;
    }
    DAT_0022d194 = DAT_0022d194 + 1;
    pfVar10 = (float *)(&DAT_0022d19c + uVar9 * 2);
    (&DAT_0022d198)[uVar9 * 2] = fVar7;
LAB_0017a7f8:
    *pfVar10 = fVar12;
  }
LAB_0017a7fc:
  _DAT_0022d190 = 0;
LAB_0017a68c:
  *puVar8 = uVar1;
  if (*(long *)(lVar4 + 0x28) == local_48) {
    return fVar12;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0017bf60 @ 0017bf60 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0017bf60(long param_1)

{
  ulong uVar1;
  int *piVar2;
  undefined4 uVar3;
  char cVar4;
  bool bVar5;
  bool bVar6;
  long lVar7;
  uint uVar8;
  long lVar9;
  int iVar10;
  undefined4 *puVar11;
  void *__src;
  undefined8 uVar12;
  ulong uVar13;
  ulong uVar14;
  long lVar15;
  ulong uVar16;
  undefined1 *puVar17;
  long lVar18;
  long lVar19;
  long local_110;
  long local_108;
  long local_100;
  long local_f8;
  long local_f0;
  long local_e8;
  long local_e0;
  long local_d8;
  long local_d0;
  long local_c8;
  long local_c0;
  ulong local_b8;
  int local_ac;
  long local_a8;
  ulong local_a0;
  ulong local_98;
  ulong local_90;
  uint local_88;
  uint uStack_84;
  ulong local_80 [2];
  long local_70;
  
  lVar7 = tpidr_el0;
  local_70 = *(long *)(lVar7 + 0x28);
  (*DAT_002fd528)();
  puVar11 = (undefined4 *)__errno();
  uVar3 = *puVar11;
  local_d8 = 0;
  iVar10 = FUN_001428fc(puVar11,param_1,&local_d8,8);
  lVar9 = DAT_0022b558;
  if (iVar10 == 0) goto LAB_0017c574;
  do {
    uVar8 = _DAT_001e12e8;
    cVar4 = '\x01';
    bVar5 = (bool)ExclusiveMonitorPass(0x1e12e8,0x10);
    if (bVar5) {
      _DAT_001e12e8 = CONCAT31(DAT_001e12e8_1,1);
      cVar4 = ExclusiveMonitorsStatus();
    }
  } while (cVar4 != '\0');
  if ((((uVar8 & 1) != 0) || (_DAT_001e12e8 = 0, DAT_0022b560 == 0)) || (local_d8 != DAT_0022b558))
  goto LAB_0017c574;
  lVar19 = 0x1b5d0;
  __src = calloc(1,0x285d0);
  if (__src == (void *)0x0) {
LAB_0017c4cc:
    bVar5 = false;
  }
  else {
    local_98 = 0;
    local_90 = 0;
    local_a8 = 0;
    local_a0 = 0;
    local_ac = 0;
    *(undefined4 *)((long)__src + 0x34) = 0xffffffff;
    uVar12 = FUN_001428fc(__src,param_1,&local_88,8);
    if ((((int)uVar12 == 0) || (0xff < local_88)) ||
       (uVar13 = (ulong)local_88 | (ulong)uStack_84 << 8, uVar13 == 0)) goto LAB_0017c4cc;
    uVar1 = 0;
    do {
      uVar16 = uVar1;
      uVar14 = uVar13;
      uVar1 = uVar16 + 1;
      *(char *)((long)local_80 + uVar16) = "0289PYLQGRJCUV"[uVar14 % 0xd];
      if (uVar14 < 0xd) break;
      uVar13 = uVar14 / 0xd;
    } while (uVar16 < 0xc);
    if (0xc < uVar14) goto LAB_0017c4cc;
    lVar15 = uVar16 - 1;
    *(undefined1 *)((long)__src + 0x48) = 0x23;
    puVar17 = (undefined1 *)((long)__src + 0x49);
    do {
      uVar13 = uVar16 & 0xffffffff;
      lVar15 = lVar15 + -1;
      uVar16 = uVar16 - 1;
      *puVar17 = *(undefined1 *)((long)local_80 + uVar13);
      puVar17 = puVar17 + 1;
    } while (lVar15 != -2);
    ((undefined1 *)((long)__src + 0x49))[uVar1] = 0;
    iVar10 = FUN_001428fc(uVar12,param_1 + 0x30,&local_90,8);
    if (iVar10 == 0) goto LAB_0017c4cc;
    bVar5 = false;
    if ((0x10fff < local_90 + 0x1000) && ((local_90 & 7) == 0)) {
      uVar12 = FUN_0014a6ac(local_90,(long)__src + 0x68,0x191);
      if (((int)uVar12 == 0) ||
         (((uVar12 = FUN_001428fc(uVar12,local_90 + 0x14,(long)__src + 0x2c,4), (int)uVar12 == 0 ||
           (uVar12 = FUN_001428fc(uVar12,local_90 + 0x18,(long)__src + 0x30,4), (int)uVar12 == 0))
          || (uVar12 = FUN_001428fc(uVar12,param_1 + 0x18,&local_98,8), (int)uVar12 == 0))))
      goto LAB_0017c4cc;
      bVar5 = false;
      if ((0x10fff < local_98 + 0x1000) && ((local_98 & 7) == 0)) {
        uVar12 = FUN_001428fc(uVar12,local_98 + 0xc,&local_ac,4);
        bVar5 = false;
        if (((int)uVar12 != 0) && ((-1 < local_ac && (local_ac < 0x201)))) {
          if (local_ac != 0) {
            uVar12 = FUN_001428fc(uVar12,local_98,&local_a0,8);
            if ((int)uVar12 == 0) goto LAB_0017c4cc;
            bVar5 = false;
            if ((local_a0 + 0x1000 < 0x11000) || ((local_a0 & 7) != 0)) goto LAB_0017c4d4;
          }
          *(int *)((long)__src + 0x28) = local_ac;
          *(ulong *)((long)__src + 0x38) = CONCAT44(uStack_84,local_88);
          lVar15 = FUN_001428fc(uVar12,param_1 + 0x78,&local_a8,8);
          if ((int)lVar15 == 0) goto LAB_0017c4cc;
          if (local_a8 != 0) {
            local_80[0] = local_80[0] & 0xffffffff00000000;
            uVar12 = FUN_001428fc(lVar15,local_a8 + 0x20,local_80,4);
            if ((((int)uVar12 == 0) || ((int)local_80[0] < 76000000)) ||
               (76999999 < (int)local_80[0])) goto LAB_0017c4cc;
            *(int *)((long)__src + 0x34) = (int)local_80[0];
            lVar15 = FUN_0017c800(uVar12,local_a8,0x4c,(long)__src + 0x1f9,0xc0,0);
          }
          if (0 < local_ac) {
            lVar18 = 0;
            local_110 = 0;
            local_108 = 0x275d0;
            local_f0 = 0x1ddc;
            local_e8 = 0x1dd8;
            local_e0 = 0x1dd0;
            local_100 = 0x1e3c;
            local_f8 = 0x1dd4;
            do {
              local_80[0] = 0;
              piVar2 = (int *)((long)__src + local_e0);
              local_c0 = 0;
              local_b8 = 0;
              local_d0 = 0;
              local_c8 = 0;
              piVar2[1] = -1;
              uVar12 = FUN_001428fc(lVar15,lVar18 + local_a0,local_80,8);
              if ((((((int)uVar12 == 0) || (local_80[0] + 0x1000 < 0x11000)) ||
                   (((local_80[0] & 7) != 0 ||
                    ((uVar12 = FUN_001428fc(uVar12,local_80[0],&local_b8,8), (int)uVar12 == 0 ||
                     (local_b8 + 0x1000 < 0x11000)))))) || ((local_b8 & 7) != 0)) ||
                 ((((uVar12 = FUN_001428fc(uVar12,local_80[0] + 8,&local_c0,8), (int)uVar12 == 0 ||
                    (uVar12 = FUN_001428fc(uVar12,local_80[0] + 0x24,(long)__src + local_e8,4),
                    (int)uVar12 == 0)) || (10000000 < *(uint *)((long)__src + local_e0 + 8))) ||
                  ((uVar12 = FUN_001428fc(uVar12,local_b8 + 0x20,piVar2,4), (int)uVar12 == 0 ||
                   (*piVar2 + 0xfefc99c0U < 0xfff0bdc0)))))) goto LAB_0017c4cc;
              uVar12 = FUN_0017c800(uVar12,local_b8,0x10,(long)__src + local_f0,0x60,&local_c8);
              if (local_c0 != 0) {
                uVar12 = FUN_001428fc(uVar12,local_c0 + 0x20,(long)__src + local_f8,4);
                if (((int)uVar12 == 0) || (piVar2[1] + 0xfe363c80U < 0xfff0bdc0)) goto LAB_0017c4cc;
                FUN_0017c800(uVar12,local_c0,0x1d,(long)__src + local_100,0x60,&local_d0);
              }
              lVar15 = local_c8;
              if (((local_c8 != 0) && (local_d0 != 0)) &&
                 (lVar15 = FUN_0014a6ac(local_c8,(long)__src + lVar19,0x60), (int)lVar15 != 0)) {
                *(long *)((long)__src + local_108) = local_d0;
              }
              lVar18 = lVar18 + 8;
              lVar19 = lVar19 + 0x60;
              local_110 = local_110 + 1;
              local_108 = local_108 + 8;
              local_100 = local_100 + 0xcc;
              local_f0 = local_f0 + 0xcc;
              local_e8 = local_e8 + 0xcc;
              local_e0 = local_e0 + 0xcc;
              local_f8 = local_f8 + 0xcc;
            } while (local_110 < local_ac);
          }
          bVar5 = true;
          *(undefined4 *)((long)__src + 0x14) = 1;
        }
      }
    }
  }
LAB_0017c4d4:
  do {
    uVar8 = _DAT_001e12e8;
    cVar4 = '\x01';
    bVar6 = (bool)ExclusiveMonitorPass(0x1e12e8,0x10);
    if (bVar6) {
      _DAT_001e12e8 = CONCAT31(DAT_001e12e8_1,1);
      cVar4 = ExclusiveMonitorsStatus();
    }
  } while (cVar4 != '\0');
  if ((uVar8 & 1) == 0) {
    if ((DAT_0022b560 != 0) && (DAT_0022b558 == lVar9)) {
      memset(&DAT_001e14a8,0,0x285d0);
      DAT_001e14dc = 0xffffffff;
      DAT_001cfb00 = DAT_001cfb00 + 1;
      if ((bVar5) && (memcpy(&DAT_001e14a8,__src,0x285d0), DAT_001e12f8 == local_d8)) {
        DAT_001e1310 = 0;
      }
    }
    _DAT_001e12e8 = 0;
  }
  free(__src);
LAB_0017c574:
  *puVar11 = uVar3;
  if (*(long *)(lVar7 + 0x28) != local_70) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

/* ===== FUN_0017c5b8 @ 0017c5b8 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0017c5b8(undefined8 param_1,undefined8 param_2,undefined4 param_3,undefined8 param_4,
                 undefined4 param_5,undefined8 param_6,undefined4 param_7)

{
  undefined4 uVar1;
  char cVar2;
  bool bVar3;
  long lVar4;
  uint uVar5;
  long lVar6;
  int iVar7;
  undefined4 *puVar8;
  long local_70;
  long local_68;
  
  lVar4 = tpidr_el0;
  local_68 = *(long *)(lVar4 + 0x28);
  puVar8 = (undefined4 *)__errno();
  uVar1 = *puVar8;
  local_70 = 0;
  iVar7 = FUN_001428fc(puVar8,param_2,&local_70,8);
  lVar6 = local_70;
  if ((iVar7 != 0) && (local_70 != 0)) {
    do {
      uVar5 = _DAT_001e12e8;
      cVar2 = '\x01';
      bVar3 = (bool)ExclusiveMonitorPass(0x1e12e8,0x10);
      if (bVar3) {
        _DAT_001e12e8 = CONCAT31(DAT_001e12e8_1,1);
        cVar2 = ExclusiveMonitorsStatus();
      }
    } while (cVar2 != '\0');
    if ((uVar5 & 1) == 0) {
      DAT_0022b558 = local_70;
      DAT_0022b560 = param_1;
      memset(&DAT_001e14a8,0,0x285d0);
      DAT_001e14dc = 0xffffffff;
      DAT_001cfb00 = DAT_001cfb00 + 1;
      if (DAT_001e12f8 != lVar6) {
        DAT_001e1310 = 0;
        DAT_001e1300 = 0;
      }
      _DAT_001e12e8 = 0;
    }
  }
  *puVar8 = uVar1;
  (*DAT_002fd530)(param_1,param_2,param_3,param_4,param_5,param_6,param_7);
  if (*(long *)(lVar4 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

/* ===== FUN_0017c720 @ 0017c720 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0017c720(long param_1)

{
  char cVar1;
  bool bVar2;
  uint uVar3;
  code *UNRECOVERED_JUMPTABLE;
  undefined4 *puVar4;
  
  puVar4 = (undefined4 *)__errno();
  UNRECOVERED_JUMPTABLE = DAT_002fd538;
  do {
    uVar3 = _DAT_001e12e8;
    cVar1 = '\x01';
    bVar2 = (bool)ExclusiveMonitorPass(0x1e12e8,0x10);
    if (bVar2) {
      _DAT_001e12e8 = CONCAT31(DAT_001e12e8_1,1);
      cVar1 = ExclusiveMonitorsStatus();
    }
  } while (cVar1 != '\0');
  if ((uVar3 & 1) == 0) {
    if (DAT_0022b560 == param_1) {
      DAT_0022b560 = 0;
      DAT_0022b558 = 0;
      DAT_001e1310 = 0;
      DAT_001cfb00 = DAT_001cfb00 + 1;
    }
    _DAT_001e12e8 = 0;
  }
  *puVar4 = *puVar4;
                    /* WARNING: Could not recover jumptable at 0x0017c7a8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*UNRECOVERED_JUMPTABLE)(param_1);
  return;
}

/* ===== FUN_0017ca1c @ 0017ca1c [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0017ca1c(long param_1,uint param_2)

{
  undefined4 uVar1;
  long lVar2;
  bool bVar3;
  int iVar4;
  undefined4 *puVar5;
  long lVar6;
  ulong uVar7;
  uint local_50;
  uint local_4c;
  long local_48;
  
  lVar2 = tpidr_el0;
  local_48 = *(long *)(lVar2 + 0x28);
  local_4c = param_2;
  puVar5 = (undefined4 *)__errno();
  uVar1 = *puVar5;
  if (((CONCAT44(DAT_00209b98._4_4_,(undefined4)DAT_00209b98) == param_1) && (DAT_00209b90 != 0)) &&
     (iVar4 = FUN_0013e1a8(param_1,0x11d1a70), iVar4 != 0)) {
    bVar3 = true;
    uVar7 = CONCAT44((undefined4)DAT_00209b98,_DAT_00209b94);
  }
  else {
    bVar3 = false;
    uVar7 = (ulong)local_4c;
  }
  *puVar5 = uVar1;
  (*DAT_002fd550)(param_1,uVar7);
  uVar1 = *puVar5;
  if (bVar3) {
    local_50 = 0xffffffff;
    lVar6 = FUN_0014bef8(DAT_001cfb4c,&local_4c,4,param_1 + 500);
    if (((lVar6 != 4) || (iVar4 = FUN_001428fc(4,param_1 + 500,&local_50,4), iVar4 == 0)) ||
       (local_50 != local_4c)) {
      DAT_00209b98._0_4_ = 0;
      DAT_00209b98._4_4_ = 0;
    }
  }
  *puVar5 = uVar1;
  if (*(long *)(lVar2 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0017d8e4 @ 0017d8e4 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0017d8e4(undefined8 param_1)

{
  char cVar1;
  bool bVar2;
  code *UNRECOVERED_JUMPTABLE;
  undefined4 *puVar3;
  
  puVar3 = (undefined4 *)__errno();
  UNRECOVERED_JUMPTABLE = DAT_002fd5a0;
  do {
    cVar1 = '\x01';
    bVar2 = (bool)ExclusiveMonitorPass(0x209c24,0x10);
    if (bVar2) {
      cVar1 = ExclusiveMonitorsStatus();
      _DAT_00209c24 = _DAT_00209c24 + 1;
    }
  } while (cVar1 != '\0');
  DAT_00209c28._4_4_ = 0;
  *puVar3 = *puVar3;
                    /* WARNING: Could not recover jumptable at 0x0017d93c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*UNRECOVERED_JUMPTABLE)(param_1);
  return;
}

/* ===== FUN_0017d940 @ 0017d940 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_0017d940(long param_1,long param_2,undefined4 param_3,int param_4)

{
  undefined4 uVar1;
  long lVar2;
  int iVar3;
  undefined4 *puVar4;
  undefined8 uVar5;
  long unaff_x30;
  undefined1 auStack_b0 [88];
  long local_58;
  
  lVar2 = tpidr_el0;
  local_58 = *(long *)(lVar2 + 0x28);
  puVar4 = (undefined4 *)__errno();
  uVar1 = *puVar4;
  if (((((int)_DAT_00209c20 != 0) && (param_2 != 0)) && (param_1 != 0)) &&
     ((param_4 == 1 && (((uint)DAT_00209c28 >> 0xe & 1) != 0)))) {
    if (((DAT_001e0978 + 0xb98a4c == unaff_x30) || (DAT_001e0978 + 0xb98ab0 == unaff_x30)) &&
       (iVar3 = FUN_0014e61c(auStack_b0), iVar3 != 0)) {
      *puVar4 = uVar1;
      if (*(long *)(lVar2 + 0x28) == local_58) {
        return 0;
      }
      goto LAB_0017da68;
    }
  }
  *puVar4 = uVar1;
  if (*(long *)(lVar2 + 0x28) == local_58) {
                    /* WARNING: Could not recover jumptable at 0x0017da64. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar5 = (*DAT_002fd5a8)(param_1,param_2,param_3,param_4);
    return uVar5;
  }
LAB_0017da68:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0017da6c @ 0017da6c [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0017da6c(long param_1,uint param_2,uint param_3,int param_4,int param_5,int param_6)

{
  undefined4 uVar1;
  long lVar2;
  int iVar3;
  undefined4 *puVar4;
  undefined8 uVar5;
  long unaff_x30;
  ushort uVar6;
  undefined1 auVar7 [16];
  undefined1 auVar8 [16];
  undefined1 auVar9 [16];
  undefined1 auVar10 [16];
  int local_d4;
  undefined8 local_d0;
  ulong local_c8;
  undefined1 auStack_c0 [16];
  long local_b0;
  long local_a8;
  long local_68;
  
  lVar2 = tpidr_el0;
  local_68 = *(long *)(lVar2 + 0x28);
  puVar4 = (undefined4 *)__errno();
  uVar1 = *puVar4;
  if (((int)_DAT_00209c20 != 0) && (param_1 != 0)) {
    auVar7._0_8_ = DAT_001e0978 + _DAT_001129d0;
    auVar7._8_8_ = DAT_001e0978 + _UNK_001129d8;
    auVar9._0_8_ = DAT_001e0978 + _DAT_00112890;
    auVar9._8_8_ = DAT_001e0978 + _UNK_00112898;
    auVar8._8_8_ = unaff_x30;
    auVar8._0_8_ = unaff_x30;
    auVar8 = NEON_cmeq(auVar7,auVar8,8);
    auVar10._8_8_ = unaff_x30;
    auVar10._0_8_ = unaff_x30;
    auVar10 = NEON_cmeq(auVar9,auVar10,8);
    uVar6 = NEON_umaxv(CONCAT26(auVar8._8_2_,
                                CONCAT24(auVar8._0_2_,CONCAT22(auVar10._8_2_,auVar10._0_2_))),2);
    if (((((uint)uVar6 | (uint)(DAT_001e0978 + 0xb990b4 == unaff_x30)) &
         ((uint)DAT_00209c28 & 0x8000) >> 0xf) != 0) &&
       (uVar5 = FUN_0014e61c(auStack_c0), (int)uVar5 != 0)) {
      local_d0 = 0;
      local_c8 = 0;
      local_d4 = -1;
      uVar5 = FUN_001428fc(uVar5,local_a8 + 0x58,&local_c8,8);
      if ((((((int)uVar5 != 0) && (0x11fff < local_c8 + 0x2000)) && ((local_c8 & 7) == 0)) &&
          ((((uVar5 = FUN_001428fc(uVar5,local_c8 + 0xc4,(long)&local_d0 + 4,4), (int)uVar5 != 0 &&
             (uVar5 = FUN_001428fc(uVar5,local_c8 + 200,&local_d0,4), (int)uVar5 != 0)) &&
            (iVar3 = FUN_001428fc(uVar5,local_b0 + 0x998,&local_d4,4), iVar3 != 0)) &&
           (((local_d0._4_4_ == param_4 && ((int)param_2 < param_4)) &&
            (((-1 < (int)(param_3 | param_2) &&
              ((((param_4 < 0x81 && (0 < (int)local_d0)) && (0 < param_4)) &&
               (((int)local_d0 == param_5 && ((int)local_d0 < 0x81)))))) &&
             ((int)param_3 < (int)local_d0)))))))) && (local_d4 == param_6)) {
        uVar5 = 0;
        *puVar4 = uVar1;
        goto LAB_0017dc60;
      }
    }
  }
  *puVar4 = uVar1;
  uVar5 = (*DAT_002fd5b0)(param_1,param_2,param_3,param_4,param_5,param_6);
LAB_0017dc60:
  if (*(long *)(lVar2 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar5);
}

/* ===== FUN_0017dc94 @ 0017dc94 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_0017dc94(long param_1)

{
  undefined4 uVar1;
  long lVar2;
  int iVar3;
  undefined4 *puVar4;
  undefined8 uVar5;
  long unaff_x30;
  undefined1 auStack_a0 [88];
  long local_48;
  
  lVar2 = tpidr_el0;
  local_48 = *(long *)(lVar2 + 0x28);
  puVar4 = (undefined4 *)__errno();
  uVar1 = *puVar4;
  if (((((int)_DAT_00209c20 == 0) || (param_1 == 0)) || (((uint)DAT_00209c28 >> 0x10 & 1) == 0)) ||
     ((DAT_001e0978 + 0xa695c8 != unaff_x30 || (iVar3 = FUN_0014e61c(auStack_a0), iVar3 == 0)))) {
    *puVar4 = uVar1;
    if (*(long *)(lVar2 + 0x28) == local_48) {
                    /* WARNING: Could not recover jumptable at 0x0017dd7c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      uVar5 = (*DAT_002fd5b8)(param_1);
      return uVar5;
    }
  }
  else {
    *puVar4 = uVar1;
    if (*(long *)(lVar2 + 0x28) == local_48) {
      return 1;
    }
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0017e094 @ 0017e094 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0017e094(long param_1)

{
  undefined4 uVar1;
  long lVar2;
  int iVar3;
  undefined4 *puVar4;
  undefined8 uVar5;
  undefined8 local_68;
  int local_5c;
  ulong local_58;
  ulong local_50;
  long local_48;
  
  lVar2 = tpidr_el0;
  local_48 = *(long *)(lVar2 + 0x28);
  (*DAT_002fd5c0)();
  puVar4 = (undefined4 *)__errno();
  uVar1 = *puVar4;
  local_50 = 0;
  local_58 = 0;
  local_5c = 0;
  uVar5 = FUN_001428fc(puVar4,param_1,&local_50,8);
  if (((((((int)uVar5 != 0) && (0x11fff < local_50 + 0x2000)) && ((local_50 & 7) == 0)) &&
       ((local_50 == DAT_001e0978 + 0x11f1a20U &&
        (uVar5 = FUN_001428fc(uVar5,param_1 + 0x9a8,&local_58,8), (int)uVar5 != 0)))) &&
      ((0x11fff < local_58 + 0x2000 &&
       (((local_58 & 7) == 0 && (uVar5 = FUN_001428fc(uVar5,local_58,&local_50,8), (int)uVar5 != 0))
       )))) && ((0x11fff < local_50 + 0x2000 &&
                (((((local_50 & 7) == 0 && (local_50 == DAT_001e0978 + 0x121f9e0U)) &&
                  (iVar3 = FUN_001428fc(uVar5,local_58 + 0x20,&local_5c,4), iVar3 != 0)) &&
                 ((40999999 < local_5c && (local_5c < 42000000)))))))) {
    DAT_001cfb58._0_4_ = local_5c + -41000000;
    DAT_0020b078 = param_1;
    iVar3 = FUN_0013c82c();
    if (iVar3 != 0) {
      DAT_00209cb0 = 0;
      FUN_0013f7e0();
    }
    iVar3 = FUN_0013c82c();
    if ((iVar3 != 0) && ((int)_DAT_0020b08c != 0)) {
      local_68 = 0;
      iVar3 = FUN_0013a78c(param_1 + 0x930,&local_68);
      if (iVar3 != 0) {
        FUN_0014f5d8(local_68,1);
      }
    }
  }
  *puVar4 = uVar1;
  if (*(long *)(lVar2 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
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

/* ===== FUN_0017e7a4 @ 0017e7a4 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0017e7a4(undefined8 param_1,uint param_2,uint param_3)

{
  long lVar1;
  int iVar2;
  undefined8 uVar3;
  ulong local_40;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  if (DAT_0020b088 == '\x01') {
    uVar3 = 0;
    if ((((1 < (param_3 | param_2)) || ((int)DAT_001dfff0 == 0)) ||
        (uVar3 = FUN_0013c82c(0), (int)uVar3 == 0)) || (uVar3 = FUN_0013fdb0(), (int)uVar3 == 0))
    goto LAB_0017e888;
    if (DAT_0020b08c != param_3) {
      local_40 = 0;
      iVar2 = FUN_001428fc(uVar3,DAT_0020b078 + 0x930,&local_40,8);
      if (((iVar2 == 0) || (local_40 + 0x2000 < 0x12000)) || ((local_40 & 7) != 0))
      goto LAB_0017e884;
      uVar3 = FUN_0014f5d8(local_40,param_3);
      if ((int)uVar3 == 0) goto LAB_0017e888;
    }
    uVar3 = 1;
    DAT_0020b08c = param_3;
    DAT_0020b094 = param_2;
  }
  else {
LAB_0017e884:
    uVar3 = 0;
  }
LAB_0017e888:
  if (*(long *)(lVar1 + 0x28) != local_38) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar3);
  }
  return;
}

/* ===== FUN_0017f7c4 @ 0017f7c4 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0017f7c4(long param_1,undefined8 param_2,uint param_3)

{
  char *pcVar1;
  char cVar2;
  bool bVar3;
  long lVar4;
  uint uVar5;
  uint uVar6;
  char acStack_178 [288];
  long local_58;
  
  lVar4 = tpidr_el0;
  local_58 = *(long *)(lVar4 + 0x28);
  do {
    uVar5 = _DAT_0020b098;
    cVar2 = '\x01';
    bVar3 = (bool)ExclusiveMonitorPass(0x20b098,0x10);
    if (bVar3) {
      _DAT_0020b098 = CONCAT31(DAT_0020b098_1,1);
      cVar2 = ExclusiveMonitorsStatus();
    }
  } while (cVar2 != '\0');
  if ((uVar5 & 1) == 0) {
    DAT_0020c0b8 = 0;
    if (param_1 != 0 && param_3 < 10) {
      DAT_0020c0b8 = param_2;
    }
    if (param_1 == 0 || param_3 >= 10) {
      param_3 = 0xffffffff;
      param_1 = 0;
    }
    DAT_0020c0b0 = param_1;
    DAT_0020c0c0 = param_3;
    if (DAT_0020b0a0 != param_1) {
      memset(&DAT_0020b0a8,0,0x1008);
      DAT_0020b0a0 = param_1;
    }
    uVar6 = DAT_002fd5dc;
    uVar5 = DAT_002fd5d4;
    _DAT_0020b098 = 0;
    if (((DAT_002fd648 < DAT_002fd5dc) && (DAT_002fd648 < 4)) ||
       ((DAT_002fd64c < DAT_002fd5d4 && (DAT_002fd64c < 4)))) {
      snprintf(acStack_178,0x120,
               ",\"callback_tid\":%u,\"last_call_tid\":%u,\"owner_tid\":%u,\"callbacks\":%u,\"captured\":%u,\"player_index\":%d,\"damage\":%d,\"epoch\":%llu"
               ,(ulong)DAT_002fd5e0,(ulong)DAT_002fd5d8,(ulong)DAT_00209cd8,(ulong)DAT_002fd5d4,
               (ulong)DAT_002fd5dc,DAT_002fd5e4,DAT_002fd5e8,param_1);
      pcVar1 = "floating_number_scope_wait";
      if (uVar6 != 0) {
        pcVar1 = "floating_number_captured";
      }
      FUN_001417c8("script_port_dps",pcVar1,acStack_178);
      DAT_002fd648 = uVar6;
      DAT_002fd64c = uVar5;
    }
  }
  if (*(long *)(lVar4 + 0x28) != local_58) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
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

/* ===== FUN_00184ce0 @ 00184ce0 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00184ce0(long *param_1,undefined8 *param_2,long param_3,undefined8 *param_4,
                 undefined8 *param_5)

{
  int iVar1;
  undefined4 uVar2;
  long lVar3;
  bool bVar4;
  bool bVar5;
  bool bVar6;
  int iVar7;
  uint uVar8;
  undefined8 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  code *pcVar12;
  long lVar13;
  long lVar14;
  long lVar15;
  long lVar16;
  uint uVar17;
  uint uVar18;
  long lVar19;
  uint uVar20;
  long lVar21;
  float fVar22;
  undefined8 uVar23;
  undefined1 auVar24 [16];
  float fVar25;
  float fVar26;
  float fVar27;
  undefined1 auVar28 [16];
  long local_60;
  int local_58;
  float local_54;
  float fStack_50;
  undefined4 uStack_4c;
  long local_48;
  
  lVar3 = tpidr_el0;
  local_48 = *(long *)(lVar3 + 0x28);
  uVar11 = 3;
  if (param_5 == (undefined8 *)0x0) goto LAB_001850dc;
  param_5[0x19] = 0;
  uVar23 = DAT_0010e808;
  param_5[2] = 0;
  param_5[1] = 0;
  param_5[0x18] = 0;
  param_5[0x17] = 0;
  param_5[0x16] = 0;
  param_5[0x15] = 0;
  param_5[0x14] = 0;
  param_5[0x13] = 0;
  param_5[0x12] = 0;
  param_5[0x11] = 0;
  param_5[0x10] = 0;
  param_5[0xf] = 0;
  param_5[0xe] = 0;
  param_5[0xd] = 0;
  param_5[0xc] = 0;
  param_5[0xb] = 0;
  param_5[10] = 0;
  param_5[9] = 0;
  param_5[8] = 0;
  param_5[7] = 0;
  param_5[6] = 0;
  param_5[5] = 0;
  param_5[4] = 0;
  param_5[3] = 0;
  *param_5 = uVar23;
  *(undefined4 *)(param_5 + 1) = 3;
  if (param_1 == (long *)0x0) goto LAB_001850dc;
  iVar7 = FUN_001853dc(param_2,param_3);
  uVar11 = 3;
  if (((param_4 == (undefined8 *)0x0) || (iVar7 == 0)) ||
     (pcVar12 = (code *)param_4[4], pcVar12 == (code *)0x0)) goto LAB_001850dc;
  uVar23 = *(undefined8 *)(param_3 + 0x10);
  param_5[4] = *(undefined8 *)(param_3 + 0x18);
  param_5[3] = uVar23;
  param_5[5] = *(undefined8 *)(param_3 + 0x20);
  *(undefined4 *)(param_5 + 9) = *(undefined4 *)(param_3 + 0x38);
  uVar23 = *(undefined8 *)(param_3 + 0x28);
  param_5[8] = *(undefined8 *)(param_3 + 0x30);
  param_5[7] = uVar23;
  iVar7 = *(int *)(param_3 + 0x40);
  *(int *)((long)param_5 + 0x4c) = iVar7;
  param_5[0xc] = *(undefined8 *)(param_3 + 0x48);
  param_5[0xd] = *param_2;
  if (*(int *)((long)param_1 + 0x7c) == 0) {
    if (((*(int *)(param_3 + 0xc) != 0) && (*(int *)(param_3 + 0x54) != 0)) &&
       ((*(int *)(param_3 + 0x3c) != 0 && (iVar7 != 0)))) {
      *(undefined4 *)((long)param_1 + 0x7c) = 1;
      iVar7 = (*pcVar12)(*param_4);
      *(undefined4 *)((long)param_1 + 0x7c) = 0;
      if (iVar7 == 1) {
        lVar13 = *param_1;
        if (((lVar13 != *(long *)(param_3 + 0x10)) ||
            (lVar14 = param_1[1], lVar14 != *(long *)(param_3 + 0x18))) ||
           ((lVar15 = param_1[2], lVar15 != *(long *)(param_3 + 0x20) ||
            ((lVar16 = param_1[3], lVar16 != *(long *)(param_3 + 0x28) ||
             (iVar7 = (int)param_1[4], iVar7 != *(int *)(param_3 + 0x38))))))) {
          if ((int)param_1[0x10] != 0) goto LAB_00184dd0;
          param_1[3] = 0;
          param_1[2] = 0;
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
          param_1[0xf] = 0;
          param_1[0xe] = 0;
          param_1[0x11] = 0;
          param_1[0x10] = 0;
          param_1[1] = 0;
          *param_1 = 0;
          lVar13 = *(long *)(param_3 + 0x10);
          *param_1 = lVar13;
          lVar14 = *(long *)(param_3 + 0x18);
          param_1[1] = lVar14;
          lVar15 = *(long *)(param_3 + 0x20);
          param_1[2] = lVar15;
          lVar16 = *(long *)(param_3 + 0x28);
          param_1[3] = lVar16;
          iVar7 = *(int *)(param_3 + 0x38);
          *(int *)(param_1 + 4) = iVar7;
        }
        if (*(int *)((long)param_2 + 0x24) != 0) {
          uVar11 = 4;
          goto LAB_001850d8;
        }
        iVar1 = *(int *)(param_3 + 8);
        if ((iVar1 == 1) && (*(int *)(param_3 + 0x50) != 0)) goto LAB_00184dd0;
        if (*(int *)(param_2 + 1) == 0) {
          if (*(int *)(param_2 + 2) == 0) {
            bVar5 = *(int *)(param_2 + 4) != 0;
            bVar4 = bVar5;
          }
          else {
            if (*(int *)(param_2 + 4) != 0) goto LAB_00184ed4;
            bVar5 = true;
            bVar4 = *(int *)((long)param_2 + 0x14) != 0;
          }
        }
        else {
LAB_00184ed4:
          bVar5 = true;
          bVar4 = true;
        }
        bVar6 = bVar5;
        if ((((iVar1 == 4) || (iVar1 == 2)) && (*(int *)(param_2 + 1) == 0)) &&
           (bVar6 = false, *(int *)(param_2 + 4) != 0)) {
          bVar6 = bVar5;
        }
        if (!bVar6) {
          uVar11 = 2;
          goto LAB_001850d8;
        }
        fVar22 = 0.0;
        if (*(int *)(param_3 + 0x68) == 0) {
          fVar26 = 0.0;
          uVar18 = 0;
        }
        else {
          uVar23 = NEON_scvtf(*(undefined8 *)(param_3 + 0x6c),4);
          fVar25 = (float)uVar23 / 300.0;
          fVar26 = (float)((ulong)uVar23 >> 0x20) / 300.0;
          auVar24._4_4_ = fVar26;
          auVar24._0_4_ = fVar25;
          auVar24._12_4_ = fVar26;
          auVar24._8_4_ = fVar25;
          auVar28 = NEON_fcmgt(_DAT_00112810,auVar24,4);
          auVar24 = NEON_fcmgt(auVar24,_DAT_00112810,4);
          if ((((auVar28._0_2_ & 1 | (auVar28._4_2_ & 1) << 1 | (auVar24._8_2_ & 1) << 2 |
                (uint)auVar24._12_2_ << 3) ^ 0xffffffff) & 0xf) == 0) {
            fVar27 = -fVar25;
            if (0.0 <= fVar25) {
              fVar27 = fVar25;
            }
            if (fVar27 < 0.5) {
              fVar27 = -fVar26;
              if (0.0 <= fVar26) {
                fVar27 = fVar26;
              }
              if (fVar27 < 0.5) goto LAB_00184f60;
            }
            uVar18 = 1;
            fVar22 = fVar25;
          }
          else {
LAB_00184f60:
            fVar26 = 0.0;
            uVar18 = 0;
          }
        }
        if (iVar1 == 3) {
          bVar5 = true;
          lVar21 = *(long *)(param_3 + 0x48);
          uVar18 = 0;
          param_1[5] = lVar13;
          fVar22 = 0.0;
          fVar26 = 0.0;
          param_1[8] = 0;
          *(undefined4 *)(param_1 + 0xf) = 1;
LAB_001850a0:
          param_1[10] = lVar21;
        }
        else if (iVar1 == 2) {
          lVar21 = *(long *)(param_3 + 0x48);
          if (((lVar13 == 0) || (param_1[5] != lVar13)) ||
             ((lVar19 = param_1[10], lVar19 == 0 || ((lVar21 < lVar19 || (0x15e < lVar21 - lVar19)))
              ))) {
            bVar5 = false;
            if (uVar18 != 0) goto LAB_0018508c;
          }
          else {
            fVar22 = *(float *)(param_1 + 8);
            fVar26 = *(float *)((long)param_1 + 0x44);
            bVar6 = (int)param_1[0xf] == 0;
            bVar5 = !bVar6;
            uVar18 = (uint)bVar6;
          }
        }
        else {
          bVar5 = false;
          if (uVar18 != 0) {
            lVar21 = *(long *)(param_3 + 0x48);
LAB_0018508c:
            bVar5 = false;
            uVar18 = 1;
            param_1[5] = lVar13;
            *(float *)(param_1 + 8) = fVar22;
            *(float *)((long)param_1 + 0x44) = fVar26;
            *(undefined4 *)(param_1 + 0xf) = 0;
            goto LAB_001850a0;
          }
        }
        if (bVar4) {
          if ((param_4[1] == 0) || (param_4[2] == 0)) goto LAB_001850d4;
          if (iVar1 == 3) {
LAB_001850cc:
            uVar20 = 1;
LAB_00185144:
            uVar11 = *(undefined4 *)(param_3 + 0x50);
            uVar17 = uVar20;
          }
          else {
            if (iVar1 == 2) {
              if (bVar5) goto LAB_001850cc;
              uVar20 = (uint)(*(int *)(param_2 + 4) != 0);
              goto LAB_00185144;
            }
            uVar20 = 0;
            uVar17 = 0;
            uVar11 = 0;
            if (iVar1 != 1) goto LAB_00185144;
          }
          if (*(int *)((long)param_2 + 0xc) == 0) {
            uVar20 = *(uint *)(param_2 + 2);
            if (uVar20 != 0) {
              uVar20 = (uint)(*(int *)((long)param_2 + 0x14) != 0);
            }
          }
          else {
            uVar20 = 1;
          }
          if (*(int *)(param_2 + 4) == 0) {
            uVar8 = *(uint *)(param_2 + 2);
            if (uVar8 != 0) {
              uVar8 = (uint)(*(int *)((long)param_2 + 0x14) != 0);
            }
          }
          else {
            uVar8 = 1;
          }
          uVar23 = *(undefined8 *)(param_3 + 0x30);
          uVar2 = *(undefined4 *)(param_3 + 0x40);
          uVar9 = *param_2;
          param_5[0x12] = lVar13;
          param_5[0x13] = lVar14;
          uVar10 = 1;
          *(int *)(param_5 + 0xe) = iVar1;
          *(uint *)((long)param_5 + 0x74) = uVar17;
          *(undefined4 *)(param_5 + 0xf) = 0;
          *(uint *)((long)param_5 + 0x7c) = uVar18;
          *(undefined4 *)(param_5 + 0x10) = uVar11;
          *(uint *)((long)param_5 + 0x84) = uVar20;
          *(uint *)(param_5 + 0x11) = uVar8;
          param_5[0x14] = lVar15;
          param_5[0x15] = lVar16;
          param_5[0x16] = uVar23;
          *(int *)(param_5 + 0x17) = iVar7;
          *(undefined4 *)((long)param_5 + 0xbc) = uVar2;
          param_5[0x18] = uVar9;
          *(float *)(param_5 + 0x19) = fVar22;
          *(float *)((long)param_5 + 0xcc) = fVar26;
          if ((uVar17 == 0) && (uVar10 = 1, *(int *)(param_2 + 3) == 2)) {
            uVar10 = 2;
          }
          *(undefined4 *)(param_5 + 0xf) = uVar10;
          *(undefined4 *)((long)param_1 + 0x7c) = 1;
          local_60 = 0;
          local_58 = 0;
          local_54 = 0.0;
          fStack_50 = 0.0;
          uStack_4c = 0;
          iVar7 = (*(code *)param_4[1])(*param_4,param_5 + 0xe,&local_60);
          if (iVar7 == 1) {
            if ((((local_60 == 0) || (local_58 == 0)) || (200.0 <= fStack_50)) ||
               (((local_54 == -200.0 || local_54 < -200.0 != NAN(local_54) || (200.0 <= local_54))
                || (fStack_50 == -200.0 || fStack_50 < -200.0 != NAN(fStack_50))))) {
LAB_001852fc:
              *(undefined4 *)(param_5 + 1) = 8;
              goto LAB_00185304;
            }
            fVar22 = -local_54;
            if (0.0 <= local_54) {
              fVar22 = local_54;
            }
            if (fVar22 < 0.5) {
              fVar22 = -fStack_50;
              if (0.0 <= fStack_50) {
                fVar22 = fStack_50;
              }
              if (fVar22 < 0.5) goto LAB_001852fc;
            }
            iVar7 = (*(code *)param_4[2])(*param_4,param_3,&local_60);
            if (iVar7 != 1) goto LAB_001852fc;
            if (local_60 != 0) {
              lVar13 = *(long *)(param_3 + 0x48);
              if (((lVar13 < 0) || (param_1[6] != local_60)) || (param_1[0xb] <= lVar13)) {
                param_1[6] = local_60;
                param_1[9] = CONCAT44(fStack_50,local_54);
                param_1[0xb] = lVar13 + 0x32;
              }
              else {
                local_54 = (float)param_1[9];
                fStack_50 = (float)((ulong)param_1[9] >> 0x20);
              }
            }
            if ((*(int *)(param_2 + 4) == 0) ||
               (((code *)param_4[3] != (code *)0x0 &&
                (iVar7 = (*(code *)param_4[3])(*param_4,param_3,&local_60), iVar7 == 1)))) {
              iVar7 = (*(code *)param_4[4])(*param_4,*param_2);
              if (iVar7 != 1) {
                *(undefined4 *)(param_5 + 1) = 5;
                goto LAB_00185320;
              }
              *(int *)(param_5 + 10) = local_58;
              param_5[6] = local_60;
              *(ulong *)((long)param_5 + 0x54) =
                   CONCAT44((int)(fStack_50 * 300.0),(int)(local_54 * 300.0));
              uVar11 = 1;
              if (*(int *)(param_3 + 8) == 4) {
                uVar11 = 2;
              }
              *(undefined4 *)(param_5 + 1) = 1;
              *(undefined4 *)((long)param_5 + 0xc) = uVar11;
              goto LAB_0018530c;
            }
            *(undefined4 *)(param_5 + 1) = 9;
          }
          else {
            uVar11 = 7;
            if (iVar7 != 0) {
              uVar11 = 5;
            }
            *(undefined4 *)(param_5 + 1) = uVar11;
            if (iVar7 != 0) goto LAB_00185320;
LAB_00185304:
            if (*(int *)(param_2 + 4) == 0) {
LAB_0018530c:
              FUN_001854b4(param_1,param_2,param_3,param_5);
            }
          }
LAB_00185320:
          *(undefined4 *)((long)param_1 + 0x7c) = 0;
        }
        else {
          *(undefined4 *)(param_5 + 1) = 0;
          FUN_001854b4(param_1,param_2,param_3,param_5);
        }
        uVar11 = *(undefined4 *)(param_5 + 1);
        goto LAB_001850dc;
      }
    }
LAB_001850d4:
    uVar11 = 5;
  }
  else {
LAB_00184dd0:
    uVar11 = 6;
  }
LAB_001850d8:
  *(undefined4 *)(param_5 + 1) = uVar11;
LAB_001850dc:
  if (*(long *)(lVar3 + 0x28) != local_48) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar11);
  }
  return;
}

/* ===== FUN_00188478 @ 00188478 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00188478(float param_1,float param_2,ulong param_3,float param_4,long param_5,int param_6,
                 undefined4 param_7,undefined4 param_8,uint param_9)

{
  long lVar1;
  int iVar2;
  bool bVar3;
  ulong uVar4;
  uint uVar5;
  float fVar6;
  uint uVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  float fVar11;
  
  fVar10 = (float)param_3;
  if (ABS(param_4) == INFINITY) {
    return;
  }
  if (NAN(ABS(param_4))) {
    return;
  }
  if (ABS(fVar10) == INFINITY) {
    return;
  }
  if (NAN(ABS(fVar10))) {
    return;
  }
  if (ABS(param_2) == INFINITY) {
    return;
  }
  if (NAN(ABS(param_2))) {
    return;
  }
  if (ABS(param_1) == INFINITY) {
    return;
  }
  if (NAN(ABS(param_1))) {
    return;
  }
  if (param_5 == 0) {
    return;
  }
  if (param_6 == 0) {
    return;
  }
  if (param_9 == 0) {
    return;
  }
  if ((*(int *)(param_5 + 8) == param_6) && (uVar7 = *(uint *)(param_5 + 0x20), uVar7 <= param_9)) {
    uVar5 = *(uint *)(param_5 + 0xc4);
    if (uVar5 == 0) goto LAB_00188538;
    bVar3 = false;
joined_r0x00188594:
    if (uVar7 == 0) goto LAB_0018866c;
LAB_001885ac:
    if (param_9 <= uVar7) goto LAB_0018866c;
    uVar7 = 0;
    if (uVar5 <= param_9) {
      uVar7 = param_9 - uVar5;
    }
    if (uVar7 - 1 < 0x3c) {
      fVar10 = param_2 - *(float *)(param_5 + 0xc0);
      fVar9 = param_1 - *(float *)(param_5 + 0xbc);
      fVar11 = (float)NEON_fmadd(fVar9,fVar9,fVar10 * fVar10);
      if (fVar11 == 1e-06 || fVar11 < 1e-06 != NAN(fVar11)) goto LAB_00188638;
      fVar11 = 30.0 / (float)uVar7;
      fVar10 = fVar10 * fVar11;
      fVar9 = fVar9 * fVar11;
      fVar11 = (float)NEON_fmadd(fVar9,fVar9,fVar10 * fVar10);
      if (fVar11 <= 178.0) {
        if (1 < *(uint *)(param_5 + 0x1c)) {
          fVar11 = (float)uVar7 / 30.0;
          fVar6 = (fVar10 - *(float *)(param_5 + 0xcc)) / fVar11;
          fVar11 = (fVar9 - *(float *)(param_5 + 200)) / fVar11;
          fVar8 = (float)NEON_fmadd(fVar11,fVar11,fVar6 * fVar6);
          if (fVar8 <= 1600.0) {
            fVar8 = (float)NEON_fmadd(*(float *)(param_5 + 0xb4),0x3f47ae14,fVar11 * 0.22);
            fVar11 = (float)NEON_fmadd(*(undefined4 *)(param_5 + 0xb8),0x3f47ae14,fVar6 * 0.22);
          }
          else {
            fVar8 = *(float *)(param_5 + 0xb4) * 0.9;
            fVar11 = *(float *)(param_5 + 0xb8) * 0.9;
          }
          *(float *)(param_5 + 0xb4) = fVar8;
          *(float *)(param_5 + 0xb8) = fVar11;
        }
        *(float *)(param_5 + 200) = fVar9;
        *(float *)(param_5 + 0xcc) = fVar10;
        *(undefined4 *)(param_5 + 0xd0) = 0;
        uVar7 = NEON_fmadd(*(undefined4 *)(param_5 + 0xac),0x3eb33333,fVar9 * 0.65);
        param_3 = (ulong)uVar7;
        param_4 = (float)NEON_fmadd(*(undefined4 *)(param_5 + 0xb0),0x3eb33333,fVar10 * 0.65);
        *(uint *)(param_5 + 0xac) = uVar7;
        *(float *)(param_5 + 0xb0) = param_4;
      }
      else {
        *(ulong *)(param_5 + 0xb4) =
             CONCAT44((float)((ulong)*(undefined8 *)(param_5 + 0xb4) >> 0x20) * 0.88,
                      (float)*(undefined8 *)(param_5 + 0xb4) * 0.88);
      }
    }
    else {
LAB_00188638:
      if (uVar7 < 3) {
        fVar10 = *(float *)(param_5 + 0xac);
        param_4 = *(float *)(param_5 + 0xb0);
        goto LAB_0018866c;
      }
      *(undefined8 *)(param_5 + 200) = 0;
      param_4 = (float)((ulong)*(undefined8 *)(param_5 + 0xac) >> 0x20) *
                (float)((ulong)_DAT_00112870 >> 0x20);
      param_3 = CONCAT44(param_4,(float)*(undefined8 *)(param_5 + 0xac) * (float)_DAT_00112870);
      fVar10 = (float)_UNK_00112878;
      uVar4 = (ulong)_UNK_00112878 >> 0x20;
      *(undefined4 *)(param_5 + 0xd0) = 1;
      *(ulong *)(param_5 + 0xb4) =
           CONCAT44((float)((ulong)*(undefined8 *)(param_5 + 0xb4) >> 0x20) * (float)uVar4,
                    (float)*(undefined8 *)(param_5 + 0xb4) * fVar10);
      *(ulong *)(param_5 + 0xac) = param_3;
    }
    fVar10 = (float)param_3;
    *(float *)(param_5 + 0xbc) = param_1;
    *(float *)(param_5 + 0xc0) = param_2;
    *(uint *)(param_5 + 0xc4) = param_9;
  }
  else {
    uVar7 = 0;
    *(undefined8 *)(param_5 + 0x10) = 0;
    *(undefined8 *)(param_5 + 8) = 0;
    *(int *)(param_5 + 8) = param_6;
    *(undefined8 *)(param_5 + 0x20) = 0;
    *(undefined8 *)(param_5 + 0x18) = 0;
    *(undefined8 *)(param_5 + 0x30) = 0;
    *(undefined8 *)(param_5 + 0x28) = 0;
    *(undefined8 *)(param_5 + 0x40) = 0;
    *(undefined8 *)(param_5 + 0x38) = 0;
    *(undefined8 *)(param_5 + 0x50) = 0;
    *(undefined8 *)(param_5 + 0x48) = 0;
    *(undefined8 *)(param_5 + 0x60) = 0;
    *(undefined8 *)(param_5 + 0x58) = 0;
    *(undefined8 *)(param_5 + 0x70) = 0;
    *(undefined8 *)(param_5 + 0x68) = 0;
    *(undefined8 *)(param_5 + 0x80) = 0;
    *(undefined8 *)(param_5 + 0x78) = 0;
    *(undefined8 *)(param_5 + 0x90) = 0;
    *(undefined8 *)(param_5 + 0x88) = 0;
    *(undefined8 *)(param_5 + 0xa0) = 0;
    *(undefined8 *)(param_5 + 0x98) = 0;
    *(undefined8 *)(param_5 + 0xb0) = 0;
    *(undefined8 *)(param_5 + 0xa8) = 0;
    *(undefined8 *)(param_5 + 0xc0) = 0;
    *(undefined8 *)(param_5 + 0xb8) = 0;
    *(undefined8 *)(param_5 + 0xd0) = 0;
    *(undefined8 *)(param_5 + 200) = 0;
    *(undefined8 *)(param_5 + 0xe0) = 0;
    *(undefined8 *)(param_5 + 0xd8) = 0;
    *(undefined8 *)(param_5 + 0xf0) = 0;
    *(undefined8 *)(param_5 + 0xe8) = 0;
LAB_00188538:
    *(float *)(param_5 + 0xbc) = param_1;
    *(float *)(param_5 + 0xc0) = param_2;
    *(uint *)(param_5 + 0xc4) = param_9;
    fVar9 = (float)NEON_fmadd(fVar10,fVar10,param_4 * param_4);
    uVar5 = param_9;
    if (fVar9 <= 178.0) {
      *(float *)(param_5 + 0xac) = fVar10;
      *(float *)(param_5 + 0xb0) = param_4;
      *(float *)(param_5 + 200) = fVar10;
      *(float *)(param_5 + 0xcc) = param_4;
      bVar3 = true;
      *(uint *)(param_5 + 0xd0) = (uint)(fVar9 < 0.0324);
      goto joined_r0x00188594;
    }
    bVar3 = true;
    if (uVar7 != 0) goto LAB_001885ac;
LAB_0018866c:
    if (!bVar3) goto LAB_001887a8;
  }
  uVar7 = *(uint *)(param_5 + 0xa4);
  uVar5 = *(uint *)(param_5 + 0xa8) % 10;
  iVar2 = uVar5 - 9;
  lVar1 = param_5 + (ulong)uVar5 * 4;
  if (uVar5 < 9) {
    iVar2 = uVar5 + 1;
  }
  *(float *)(lVar1 + 0x2c) = param_1;
  *(float *)(lVar1 + 0x54) = param_2;
  *(uint *)(lVar1 + 0x7c) = param_9;
  *(int *)(param_5 + 0xa8) = iVar2;
  if (uVar7 < 10) {
    *(uint *)(param_5 + 0xa4) = uVar7 + 1;
  }
LAB_001887a8:
  *(float *)(param_5 + 0xc) = param_1;
  *(float *)(param_5 + 0x10) = param_2;
  *(float *)(param_5 + 0x14) = fVar10;
  *(float *)(param_5 + 0x18) = param_4;
  *(undefined4 *)(param_5 + 0x24) = param_7;
  *(undefined4 *)(param_5 + 0x28) = param_8;
  *(int *)(param_5 + 0x1c) = *(int *)(param_5 + 0x1c) + 1;
  *(uint *)(param_5 + 0x20) = param_9;
  return;
}

/* ===== FUN_00191a20 @ 00191a20 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
FUN_00191a20(long *param_1,long param_2,long param_3,undefined8 param_4,undefined8 *param_5,
            long param_6,undefined8 *param_7)

{
  uint uVar1;
  float fVar2;
  bool bVar3;
  bool bVar4;
  int iVar5;
  undefined4 uVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  long lVar9;
  float fVar10;
  undefined8 uVar11;
  undefined8 uVar12;
  float fVar14;
  undefined8 uVar15;
  undefined8 uVar16;
  undefined1 auVar17 [16];
  undefined1 auVar18 [16];
  undefined2 uVar19;
  ulong uVar13;
  
  if (param_6 != 0x48) {
    return 0;
  }
  if (param_5 == (undefined8 *)0x0) {
    return 0;
  }
  if (param_1 == (long *)0x0) {
    return 0;
  }
  if (param_2 == 0) {
    return 0;
  }
  if (param_3 == 0) {
    return 0;
  }
  if (param_7 == (undefined8 *)0x0) {
    return 0;
  }
  param_7[0x16] = 0;
  param_7[3] = 0;
  param_7[2] = 0;
  param_7[5] = 0;
  param_7[4] = 0;
  param_7[7] = 0;
  param_7[6] = 0;
  param_7[9] = 0;
  param_7[8] = 0;
  param_7[0xb] = 0;
  param_7[10] = 0;
  param_7[0xd] = 0;
  param_7[0xc] = 0;
  param_7[0xf] = 0;
  param_7[0xe] = 0;
  param_7[0x11] = 0;
  param_7[0x10] = 0;
  param_7[0x13] = 0;
  param_7[0x12] = 0;
  param_7[0x15] = 0;
  param_7[0x14] = 0;
  param_7[1] = 0;
  *param_7 = 0;
  uVar15 = *(undefined8 *)(param_2 + 0x10);
  uVar12 = *(undefined8 *)(param_2 + 8);
  *(undefined4 *)param_7 = 0x1e;
  param_7[3] = uVar15;
  param_7[2] = uVar12;
  param_7[4] = *(undefined8 *)(param_3 + 8);
  uVar12 = *param_5;
  param_7[6] = param_5[1];
  param_7[5] = uVar12;
  uVar16 = param_5[5];
  uVar12 = param_5[4];
  uVar11 = param_5[6];
  uVar15 = param_5[2];
  uVar8 = param_5[3];
  uVar7 = param_5[8];
  param_7[0xc] = param_5[7];
  param_7[0xb] = uVar11;
  param_7[10] = uVar16;
  param_7[9] = uVar12;
  param_7[8] = uVar8;
  param_7[7] = uVar15;
  param_7[0xd] = uVar7;
  uVar15 = param_5[4];
  uVar11 = param_5[7];
  uVar7 = param_5[6];
  uVar8 = param_5[8];
  uVar16 = param_5[3];
  uVar12 = param_5[2];
  param_7[0x13] = param_5[5];
  param_7[0x12] = uVar15;
  param_7[0x15] = uVar11;
  param_7[0x14] = uVar7;
  param_7[0x16] = uVar8;
  param_7[0x11] = uVar16;
  param_7[0x10] = uVar12;
  uVar12 = *param_5;
  param_7[0xf] = param_5[1];
  param_7[0xe] = uVar12;
  if ((*(int *)(param_3 + 0x10) == 0) && (*(int *)(param_3 + 0x14) == 0)) {
    return 0;
  }
  if (*(int *)(param_2 + 0x24) != 1) {
    return 0;
  }
  if (*(int *)(param_2 + 0x30) != 1) {
    return 0;
  }
  if (*(int *)(param_2 + 0x2c) != 0) {
    return 0;
  }
  if (*param_1 != *(long *)(param_2 + 8)) {
    return 0;
  }
  iVar5 = *(int *)((long)param_5 + 4);
  uVar1 = *(uint *)(param_5 + 1);
  uVar12 = *(undefined8 *)((long)param_5 + 0xc);
  if (((uVar1 == 0 && iVar5 == 0) && (*(char *)(param_5 + 3) != '\0')) &&
     (lVar9 = *(long *)(param_2 + 0x18), -1 < lVar9)) {
    param_1[0x15] = *param_1;
    param_1[0x16] = lVar9;
    param_1[0x17] = 0;
    *(undefined4 *)(param_1 + 0x18) = 1;
  }
  if (1 < uVar1) {
    return 0;
  }
  if (iVar5 != 0) {
    return 0;
  }
  if (uVar1 == 0) {
    bVar3 = *(char *)(param_5 + 3) != '\0';
  }
  else {
    bVar3 = false;
  }
  uVar12 = NEON_scvtf(uVar12,4);
  fVar10 = (float)uVar12 / 300.0;
  fVar14 = (float)((ulong)uVar12 >> 0x20) / 300.0;
  uVar13 = CONCAT44(fVar14,fVar10);
  if (((param_1[0x15] == *(long *)(param_2 + 8)) && (lVar9 = param_1[0x16], lVar9 != 0)) &&
     ((lVar9 <= *(long *)(param_2 + 0x18) && (*(long *)(param_2 + 0x18) - lVar9 < 0x15f)))) {
    bVar4 = (int)param_1[0x18] == 0;
    fVar2 = 0.0;
    if (bVar4) {
      fVar2 = fVar10;
    }
    uVar13 = (ulong)(uint)fVar2;
    if (!bVar4) {
      bVar3 = true;
    }
    fVar10 = 0.0;
    if (bVar4) {
      fVar10 = fVar14;
    }
    fVar14 = fVar10;
    if (bVar3) {
LAB_00191bf8:
      bVar3 = true;
      goto LAB_00191c88;
    }
    if (*(int *)(param_3 + 0x10) == 0) {
      return 0;
    }
    uVar13 = (ulong)*(uint *)(param_1 + 0x17);
    uVar19 = (undefined2)*(undefined4 *)((long)param_1 + 0xbc);
  }
  else {
    if (bVar3) goto LAB_00191bf8;
    auVar18._8_8_ = uVar13;
    auVar18._0_8_ = uVar13;
    auVar17 = NEON_fcmgt(_DAT_00112810,auVar18,4);
    auVar18 = NEON_fcmgt(auVar18,_DAT_00112810,4);
    if (((((auVar17._0_2_ & 1 | (auVar17._4_2_ & 1) << 1 | (auVar18._8_2_ & 1) << 2 |
           (uint)auVar18._12_2_ << 3) ^ 0xffffffff) & 0xf) == 0) &&
       ((0.5 <= ABS(fVar10) || (0.5 <= ABS(fVar14))))) {
      FUN_001918e4(uVar13,SUB42(fVar14,0),param_1,*(long *)(param_2 + 8),
                   *(undefined8 *)(param_2 + 0x18),0);
    }
    bVar3 = false;
LAB_00191c88:
    uVar19 = SUB42(fVar14,0);
    if (*(int *)(param_3 + 0x10) == 0) {
      return 0;
    }
    if (bVar3) {
      uVar6 = 1;
      goto LAB_00191ca0;
    }
  }
  uVar6 = *(undefined4 *)(param_3 + 0x18);
LAB_00191ca0:
  iVar5 = FUN_00190e74(uVar13,uVar19,param_1,param_2,param_3,uVar6,param_4);
  if (iVar5 == 0) {
    return 0;
  }
  *(int *)((long)param_7 + 0x84) = iVar5;
  *(undefined2 *)(param_7 + 0x11) = 0x101;
  lVar9 = param_1[8];
  uVar6 = *(undefined4 *)((long)param_1 + 0x44);
  uVar12 = *(undefined8 *)(param_2 + 0x18);
  *(undefined4 *)((long)param_7 + 4) = 1;
  *(int *)(param_7 + 1) = iVar5;
  FUN_00191974((int)lVar9,(short)uVar6,param_1,iVar5,0,uVar12);
  return 1;
}

/* ===== FUN_00192528 @ 00192528 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00192528(undefined8 *param_1,long param_2,long param_3,long param_4)

{
  byte *pbVar1;
  byte *pbVar2;
  char cVar3;
  bool bVar4;
  ulong uVar5;
  ulong uVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  byte bVar11;
  byte bVar12;
  int iVar13;
  uint uVar14;
  long lVar15;
  ulong uVar16;
  long lVar17;
  ulong uVar18;
  undefined4 uVar19;
  undefined4 uVar20;
  long lVar21;
  undefined4 *puVar22;
  uint uVar23;
  long lVar24;
  long lVar25;
  ushort uVar26;
  ulong uVar27;
  ulong uVar29;
  undefined1 auVar28 [16];
  undefined1 auVar30 [16];
  ulong uStack_40ac8;
  int iStack_40a88;
  int iStack_40a84;
  long lStack_40a80;
  long lStack_40a78;
  undefined8 uStack_40a70;
  long lStack_40a68;
  undefined8 uStack_40a60;
  undefined8 uStack_40a58;
  undefined8 uStack_40a50;
  undefined8 uStack_40a48;
  undefined8 uStack_40a40;
  undefined8 uStack_40a38;
  undefined8 uStack_40a30;
  undefined8 uStack_40a28;
  undefined8 uStack_40a20;
  undefined8 uStack_40a18;
  undefined8 uStack_40a10;
  undefined8 uStack_40a08;
  undefined8 uStack_40a00;
  int iStack_409f8;
  int iStack_409f4;
  ulong uStack_409f0;
  ulong uStack_409e8;
  long lStack_409e0;
  ulong uStack_409d8;
  undefined8 uStack_409d0;
  undefined8 uStack_409c8;
  undefined8 uStack_409c0;
  undefined8 uStack_409b8;
  undefined8 uStack_409b0;
  undefined8 uStack_409a8;
  timespec tStack_409a0;
  undefined4 auStack_40990 [3];
  undefined8 uStack_40984;
  undefined8 uStack_4097c;
  undefined1 auStack_990 [208];
  undefined8 local_8c0;
  undefined8 uStack_8b8;
  long local_8b0;
  long local_110;
  long lStack_108;
  long local_100;
  undefined8 uStack_f8;
  undefined8 local_f0;
  long lStack_e8;
  ulong local_e0;
  undefined8 uStack_d8;
  undefined8 local_d0;
  undefined8 uStack_c8;
  long local_c0;
  long lStack_b8;
  long local_b0;
  ulong uStack_a8;
  ulong local_a0;
  undefined8 uStack_98;
  timespec local_90;
  long local_78;
  
  lVar7 = tpidr_el0;
  local_78 = *(long *)(lVar7 + 0x28);
  if ((param_1 == (undefined8 *)0x0) || (*(int *)(param_1 + 0x14b) == 0)) {
    iVar13 = 10;
    goto LAB_00192e98;
  }
  pbVar1 = (byte *)((long)param_1 + 0xa9);
  do {
    bVar11 = *pbVar1;
    cVar3 = '\x01';
    bVar4 = (bool)ExclusiveMonitorPass(pbVar1,0x10);
    if (bVar4) {
      *pbVar1 = 1;
      cVar3 = ExclusiveMonitorsStatus();
    }
  } while (cVar3 != '\0');
  if ((bVar11 & 1) != 0) {
    iVar13 = 8;
    goto LAB_00192e98;
  }
  clock_gettime(1,&tStack_409a0);
  lVar10 = tStack_409a0.tv_nsec;
  lVar8 = CONCAT44(tStack_409a0.tv_sec._4_4_,(uint)tStack_409a0.tv_sec);
  uStack_98 = 0;
  local_a0 = 0;
  uStack_a8 = 0;
  local_b0 = 0;
  uStack_c8 = 0;
  local_d0 = 0;
  local_e0 = 0;
  lStack_e8 = 0;
  local_f0 = 0;
  uStack_f8 = 0;
  local_100 = 0;
  lStack_108 = 0;
  local_110 = 0;
  lVar24 = param_1[0x145];
  uStack_d8 = lVar24 << 0x20;
  local_c0 = param_2;
  lStack_b8 = param_3;
  memset(auStack_990,0,0x880);
  uVar18 = _UNK_00112a78;
  lVar9 = _DAT_00112a70;
  uStack_40a00 = 0;
  lStack_40a80 = 0;
  lStack_40a78 = 0;
  uStack_40a58 = 0;
  uStack_40a60 = 0;
  uStack_40a48 = 0;
  uStack_40a50 = 0;
  uStack_40a38 = 0;
  uStack_40a40 = 0;
  uStack_40a28 = 0;
  uStack_40a30 = 0;
  uStack_40a18 = 0;
  uStack_40a20 = 0;
  uStack_40a08 = 0;
  uStack_40a10 = 0;
  lStack_40a68 = 0;
  uStack_40a70 = 0;
  uStack_409c8 = _UNK_00112a78;
  uStack_409d0 = _DAT_00112a70;
  uVar29 = SUB168(_DAT_001127c0,8);
  uVar27 = SUB168(_DAT_001127c0,0);
  iStack_40a84 = -0x80000000;
  iStack_409f4 = -1;
  uStack_409c0 = uVar27;
  uStack_409b8 = uVar29;
  uStack_409b0 = uVar27;
  uStack_409a8 = uVar29;
  if ((int)lVar24 == 0) {
    lVar25 = 0;
    lVar21 = 0;
    lVar24 = 0;
  }
  else {
    uVar16 = uVar27;
    uVar5 = uVar29;
    if (*(int *)((long)param_1 + 0xa5c) == 0) {
      if (param_1[0x10] + 0x7316b8 == param_4) {
        local_e0 = (*(code *)param_1[0x12])(param_1[0xf]);
        iVar13 = (*(code *)param_1[0x13])(param_1[0xf]);
        uVar20 = 3;
        uVar19 = 3;
        uStack_c8 = CONCAT44(uStack_c8._4_4_,iVar13);
        if (((param_2 == 0) || (param_3 == 0)) || (iVar13 == 0)) goto LAB_00192cb0;
        lVar21 = 0;
        lVar24 = 0;
        lVar25 = 0;
        uVar16 = uStack_409b0;
        uVar5 = uStack_409a8;
        if (-1 < (long)local_e0) {
          if (uStack_d8._4_4_ == 2) {
            pbVar2 = (byte *)(param_1 + 0x15);
            do {
              bVar11 = *pbVar2;
              cVar3 = '\x01';
              bVar4 = (bool)ExclusiveMonitorPass(pbVar2,0x10);
              if (bVar4) {
                *pbVar2 = 1;
                cVar3 = ExclusiveMonitorsStatus();
              }
            } while (cVar3 != '\0');
            if ((bVar11 & 1) == 0) {
              memcpy(auStack_990,param_1 + 0x16,0x880);
              uStack_40a30 = param_1[0x12e];
              uStack_40a28 = param_1[0x12f];
              uStack_40a18 = param_1[0x131];
              uStack_40a20 = param_1[0x130];
              lStack_40a68 = param_1[0x127];
              uStack_40a70 = param_1[0x126];
              uStack_40a00 = param_1[0x134];
              uStack_40a60 = param_1[0x128];
              uStack_40a58 = param_1[0x129];
              uStack_40a48 = param_1[299];
              uStack_40a50 = param_1[0x12a];
              uStack_40a40 = param_1[300];
              uStack_40a38 = param_1[0x12d];
              uStack_40a08 = SUB168(*(undefined1 (*) [16])(param_1 + 0x132),8);
              uStack_40a10 = SUB168(*(undefined1 (*) [16])(param_1 + 0x132),0);
              *(undefined4 *)(param_1 + 0x15) = 0;
              local_f0 = uStack_8b8;
              uStack_f8 = local_8c0;
              lStack_e8 = local_8b0;
              iVar13 = FUN_001902ec(auStack_990,local_8c0,local_e0);
              if ((iVar13 == 0) || (local_8b0 != lStack_40a68)) {
                lVar25 = 0;
                lVar21 = 0;
                lVar24 = 0;
                uVar19 = 7;
                uVar16 = uStack_409b0;
                uVar5 = uStack_409a8;
              }
              else {
                uVar14 = (*(code *)param_1[0x14])(param_1[0xf],2,local_8c0);
                uVar23 = uVar14 & 3;
                if ((uVar14 & 3) == 0) goto LAB_00192af4;
LAB_00192864:
                iVar13 = (*(code *)param_1[0x11])
                                   (param_1[0xf],param_1[0x10] + 0x12fd788,&lStack_40a78,8);
                if (iVar13 == 1) {
                  iVar13 = (*(code *)param_1[0x11])
                                     (param_1[0xf],param_1[0x10] + 0x12fd790,&lStack_40a80,8);
                  uVar19 = 4;
                  uVar20 = 4;
                  if ((iVar13 != 1) || (lStack_40a78 != param_2)) goto LAB_00192cb0;
                  lVar21 = 0;
                  lVar24 = 0;
                  lVar25 = 0;
                  uVar16 = uStack_409b0;
                  uVar5 = uStack_409a8;
                  if (lStack_40a80 == param_3) {
                    lVar15 = (*(code *)param_1[2])();
                    local_b0 = lVar15;
                    if (((((lVar15 == 0) || (lVar24 = (*(code *)*param_1)(), lVar24 != param_2)) ||
                         (lVar24 = (*(code *)param_1[1])(0x3059), lVar24 != param_3)) ||
                        ((lVar24 = (*(code *)param_1[2])(), lVar24 != lVar15 ||
                         (iVar13 = (*(code *)param_1[3])(), iVar13 != 0x30a0)))) ||
                       ((param_1[0x149] != 0 &&
                        (((param_1[0x149] != lVar15 || (param_1[0x147] != param_2)) ||
                         ((param_1[0x148] != param_3 ||
                          (*(int *)(param_1 + 0x14a) != (int)uStack_c8)))))))) {
                      lVar21 = 0;
                      lVar25 = 0;
                      lVar24 = 0;
                      uVar19 = 3;
                      uVar16 = uStack_409b0;
                      uVar5 = uStack_409a8;
                    }
                    else {
                      iVar13 = (*(code *)param_1[4])(param_2,param_3,0x3057,&uStack_98);
                      if (iVar13 != 1) goto LAB_00192b08;
                      iVar13 = (*(code *)param_1[4])(param_2,param_3,0x3056,(long)&uStack_98 + 4);
                      auVar28._8_8_ = uStack_98;
                      auVar28._0_8_ = uStack_98;
                      uVar19 = 4;
                      auVar30 = NEON_cmgt(auVar28,_DAT_00112b50,4);
                      auVar28 = NEON_cmgt(_DAT_00112b50,auVar28,4);
                      uVar26 = NEON_umaxv(CONCAT26(auVar28._12_2_,
                                                   CONCAT24(auVar28._8_2_,
                                                            CONCAT22(auVar30._4_2_,auVar30._0_2_))),
                                          2);
                      uVar20 = 4;
                      if ((uVar26 & 1) == 0) {
                        lVar21 = 0;
                        lVar24 = 0;
                        lVar25 = 0;
                        uVar16 = uStack_409b0;
                        uVar5 = uStack_409a8;
                        if (iVar13 != 1) goto LAB_00192cb8;
                        (*(code *)param_1[5])(0x8ca6,&iStack_40a84);
                        (*(code *)param_1[7])(0xc23,&iStack_409f4);
                        (*(code *)param_1[5])(0xba2,&uStack_409b0);
                        (*(code *)param_1[5])(0xc10,&uStack_409c0);
                        (*(code *)param_1[6])(0xc22,&uStack_409d0);
                        bVar11 = (*(code *)param_1[8])(0xc11);
                        uVar20 = 5;
                        uVar19 = 5;
                        local_a0 = uStack_409a8;
                        uStack_a8 = uStack_409b0;
                        if (iStack_409f4 == 0x1010101) {
                          if ((((-1 < (int)((uint)uStack_409b0 | uStack_409b0._4_4_)) &&
                               (0 < (int)uStack_409a8)) && (iStack_40a84 == 0)) &&
                             (0 < uStack_409a8._4_4_)) {
                            lVar21 = 0;
                            lVar24 = 0;
                            lVar25 = 0;
                            uVar16 = uStack_409b0;
                            uVar5 = uStack_409a8;
                            if (bVar11 < 2) {
                              lVar25 = 0;
                              if ((int)uStack_98 < (int)uStack_409a8) {
                                lVar21 = 0;
                                lVar24 = 0;
                                uVar19 = 5;
                              }
                              else {
                                uVar20 = 5;
                                uVar19 = 5;
                                if (uStack_98._4_4_ < uStack_409a8._4_4_) goto LAB_00192cb0;
                                lVar21 = 0;
                                lVar24 = 0;
                                if ((int)(uint)uStack_409b0 <= (int)uStack_98 - (int)uStack_409a8) {
                                  if (((uStack_98._4_4_ - uStack_409a8._4_4_ <
                                        (int)uStack_409b0._4_4_) ||
                                      ((int)uStack_409c0 == -0x80000000)) ||
                                     ((uStack_409c0._4_4_ == -0x80000000 || ((int)uStack_409b8 < 0))
                                     )) goto LAB_00192cb0;
                                  lVar21 = 0;
                                  lVar24 = 0;
                                  lVar25 = 0;
                                  if (-1 < (long)uStack_409b8) {
                                    lVar21 = 0;
                                    lVar24 = 0;
                                    uVar19 = 5;
                                    lVar25 = 0;
                                    if ((ABS((float)uStack_409d0) != INFINITY) &&
                                       (!NAN(ABS((float)uStack_409d0)))) {
                                      lVar21 = 0;
                                      lVar24 = 0;
                                      uVar19 = 5;
                                      lVar25 = 0;
                                      if ((ABS(uStack_409d0._4_4_) != INFINITY) &&
                                         (!NAN(ABS(uStack_409d0._4_4_)))) {
                                        lVar21 = 0;
                                        lVar24 = 0;
                                        uVar19 = 5;
                                        lVar25 = 0;
                                        if ((ABS((float)uStack_409c8) != INFINITY) &&
                                           (!NAN(ABS((float)uStack_409c8)))) {
                                          lVar21 = 0;
                                          lVar24 = 0;
                                          uVar19 = 5;
                                          lVar25 = 0;
                                          if ((ABS(uStack_409c8._4_4_) != INFINITY) &&
                                             (!NAN(ABS(uStack_409c8._4_4_)))) {
                                            clock_gettime(1,&tStack_409a0);
                                            lVar24 = CONCAT44(tStack_409a0.tv_sec._4_4_,
                                                              (uint)tStack_409a0.tv_sec) * 1000000 +
                                                     (ulong)tStack_409a0.tv_nsec / 1000;
                                            if (uStack_d8._4_4_ == 1) {
                                              iVar13 = FUN_00195f34(&uStack_409b0,&tStack_409a0);
                                              if (iVar13 == 0) {
                                                lVar25 = 0;
                                                lVar21 = 0;
                                                uVar19 = 5;
                                                uVar16 = uStack_409b0;
                                                uVar5 = uStack_409a8;
                                              }
                                              else {
LAB_00192ef0:
                                                if ((uint)tStack_409a0.tv_sec == 0) {
                                                  lVar25 = 0;
                                                  lVar21 = 0;
                                                  goto LAB_00192cbc;
                                                }
                                                uVar14 = (*(code *)param_1[0x14])
                                                                   (param_1[0xf],uStack_d8._4_4_,
                                                                    uStack_f8,lStack_e8);
                                                if (uStack_d8._4_4_ == 1) {
                                                  uVar23 = 1;
                                                }
                                                if ((uVar14 == uVar23) &&
                                                   ((int)param_1[0x145] == uStack_d8._4_4_)) {
                                                  uVar16 = (*(code *)param_1[0x12])(param_1[0xf]);
                                                  lVar25 = 0;
                                                  uVar19 = 0xb;
                                                  if (((long)uVar16 < 0) || (uVar16 < local_e0)) {
                                                    lVar21 = 0;
                                                    uVar16 = uStack_409b0;
                                                    uVar5 = uStack_409a8;
                                                  }
                                                  else if (((uStack_d8._4_4_ == 1) &&
                                                           ((ulong)param_1[0x146] <= uVar16)) ||
                                                          ((uStack_d8._4_4_ == 2 &&
                                                           (iVar13 = FUN_001902ec(auStack_990,
                                                                                  local_8c0),
                                                           iVar13 == 0)))) {
                                                    lVar25 = 0;
                                                    lVar21 = 0;
                                                    uVar19 = 0xb;
                                                    uVar16 = uStack_409b0;
                                                    uVar5 = uStack_409a8;
                                                  }
                                                  else {
                                                    lVar21 = (*(code *)*param_1)();
                                                    if (((lVar21 == param_2) &&
                                                        (lVar21 = (*(code *)param_1[1])(0x3059),
                                                        lVar21 == param_3)) &&
                                                       (lVar21 = (*(code *)param_1[2])(),
                                                       lVar21 == lVar15)) {
                                                      iVar13 = (*(code *)param_1[0x13])
                                                                         (param_1[0xf]);
                                                      if (iVar13 == (int)uStack_c8) {
                                                        if (param_1[0x149] == 0) {
                                                          param_1[0x147] = param_2;
                                                          param_1[0x148] = param_3;
                                                          *(int *)(param_1 + 0x14a) = iVar13;
                                                          param_1[0x149] = lVar15;
                                                        }
                                                        clock_gettime(1,&local_90);
                                                        lVar21 = local_90.tv_sec * 1000000 +
                                                                 (ulong)local_90.tv_nsec / 1000;
                                                        (*(code *)param_1[9])(0xc11);
                                                        uStack_409d8 = uVar18;
                                                        lStack_409e0 = lVar9;
                                                        if ((uint)tStack_409a0.tv_sec != 0) {
                                                          uStack_40ac8 = 0;
                                                          puVar22 = auStack_40990 + 1;
                                                          do {
                                                            if (lStack_409e0 !=
                                                                *(long *)*(undefined1 (*) [16])
                                                                          (puVar22 + 2) ||
                                                                uStack_409d8 !=
                                                                *(ulong *)(puVar22 + 4)) {
                                                              (*(code *)param_1[0xc])
                                                                        (puVar22[2],
                                                                         (short)puVar22[3],
                                                                         puVar22[4],puVar22[5]);
                                                              auVar28 = *(undefined1 (*) [16])
                                                                         (puVar22 + 2);
                                                              uStack_409d8 = auVar28._8_8_;
                                                              lStack_409e0 = auVar28._0_8_;
                                                            }
                                                            (*(code *)param_1[0xb])
                                                                      (puVar22[-2],puVar22[-1],
                                                                       *puVar22,puVar22[1]);
                                                            (*(code *)param_1[0xd])(0x4000);
                                                            uStack_40ac8 = uStack_40ac8 + 1;
                                                            local_d0 = CONCAT44(local_d0._4_4_,
                                                                                (int)local_d0 + 1);
                                                            puVar22 = puVar22 + 8;
                                                          } while (uStack_40ac8 <
                                                                   (uint)tStack_409a0.tv_sec);
                                                        }
                                                        iVar13 = clock_gettime(1,&local_90);
                                                        lVar25 = local_90.tv_sec * 1000000 +
                                                                 (ulong)local_90.tv_nsec / 1000;
                                                        lVar17 = (*(code *)*param_1)(iVar13);
                                                        if (((lVar17 != param_2) ||
                                                            (lVar17 = (*(code *)param_1[1])(0x3059),
                                                            lVar17 != param_3)) ||
                                                           (lVar17 = (*(code *)param_1[2])(),
                                                           lVar17 != lVar15)) {
                                                          uVar19 = 9;
                                                          *(undefined4 *)((long)param_1 + 0xa5c) = 1
                                                          ;
                                                          uVar16 = uStack_409b0;
                                                          uVar5 = uStack_409a8;
                                                          goto LAB_00192cb8;
                                                        }
                                                        (*(code *)param_1[0xc])
                                                                  ((float)uStack_409d0,
                                                                   (short)((ulong)uStack_409d0 >>
                                                                          0x20),(float)uStack_409c8,
                                                                   uStack_409c8._4_4_);
                                                        (*(code *)param_1[0xb])
                                                                  (uStack_409c0 & 0xffffffff,
                                                                   uStack_409c0._4_4_,
                                                                   uStack_409b8 & 0xffffffff,
                                                                   uStack_409b8._4_4_);
                                                        if (bVar11 == 0) {
                                                          (*(code *)param_1[10])(0xc11);
                                                        }
                                                        if ((*(byte *)(param_1 + 0x136) & 0xf) == 0)
                                                        {
                                                          local_90.tv_nsec = uVar18;
                                                          local_90.tv_sec = lVar9;
                                                          iStack_40a88 = -0x80000000;
                                                          iStack_409f8 = -1;
                                                          uStack_409f0 = uVar27;
                                                          uStack_409e8 = uVar29;
                                                          (*(code *)param_1[6])(0xc22,&local_90);
                                                          (*(code *)param_1[5])(0xc10,&uStack_409f0)
                                                          ;
                                                          (*(code *)param_1[5])
                                                                    (0x8ca6,&iStack_40a88);
                                                          (*(code *)param_1[7])(0xc23,&iStack_409f8)
                                                          ;
                                                          if (((uStack_409d0 == local_90.tv_sec &&
                                                                uStack_409c8 == local_90.tv_nsec) &&
                                                              (uStack_409c0 == uStack_409f0 &&
                                                               uStack_409b8 == uStack_409e8)) &&
                                                             ((bVar12 = (*(code *)param_1[8])(0xc11)
                                                              , bVar12 == bVar11 &&
                                                              (iStack_40a88 == iStack_40a84)))) {
                                                            local_d0 = (ulong)CONCAT14(iStack_409f4
                                                                                       == 
                                                  iStack_409f8,(int)local_d0);
                                                  if (iStack_409f4 == iStack_409f8)
                                                  goto LAB_00193220;
                                                  }
                                                  else {
                                                    local_d0 = local_d0 & 0xffffffff;
                                                  }
                                                  uVar19 = 9;
                                                  *(undefined4 *)((long)param_1 + 0xa5c) = 1;
                                                  uVar16 = uStack_409b0;
                                                  uVar5 = uStack_409a8;
                                                  }
                                                  else {
                                                    local_d0 = CONCAT44(1,(int)local_d0);
LAB_00193220:
                                                    uVar16 = uStack_409b0;
                                                    uVar5 = uStack_409a8;
                                                    if (uStack_d8._4_4_ == 1) {
                                                      uVar19 = 1;
                                                      *(int *)((long)param_1 + 0xa54) =
                                                           *(int *)((long)param_1 + 0xa54) + 1;
                                                    }
                                                    else {
                                                      uVar19 = 1;
                                                    }
                                                  }
                                                  goto LAB_00192cb8;
                                                  }
                                                  }
                                                  lVar25 = 0;
                                                  lVar21 = 0;
                                                  uVar19 = 3;
                                                  uVar16 = uStack_409b0;
                                                  uVar5 = uStack_409a8;
                                                  }
                                                }
                                                else {
                                                  lVar25 = 0;
                                                  lVar21 = 0;
                                                  uVar19 = 6;
                                                  uVar16 = uStack_409b0;
                                                  uVar5 = uStack_409a8;
                                                }
                                              }
                                            }
                                            else {
                                              iVar13 = FUN_001933dc(auStack_990,&uStack_40a70,
                                                                    local_e0,uVar23,&uStack_409b0,
                                                                    &tStack_409a0);
                                              if (iVar13 != 0) goto LAB_00192ef0;
                                              lVar25 = 0;
                                              lVar21 = 0;
                                              uVar19 = 7;
                                              uVar16 = uStack_409b0;
                                              uVar5 = uStack_409a8;
                                            }
                                          }
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                            goto LAB_00192cb8;
                          }
                        }
                      }
LAB_00192cb0:
                      uVar19 = uVar20;
                      lVar21 = 0;
                      lVar24 = 0;
                      lVar25 = lVar21;
                      uVar16 = uStack_409b0;
                      uVar5 = uStack_409a8;
                    }
                  }
                }
                else {
LAB_00192b08:
                  lVar25 = 0;
                  lVar21 = 0;
                  lVar24 = 0;
                  uVar19 = 4;
                  uVar16 = uStack_409b0;
                  uVar5 = uStack_409a8;
                }
              }
            }
            else {
              lVar25 = 0;
              lVar21 = 0;
              lVar24 = 0;
              uVar19 = 8;
            }
          }
          else if (uStack_d8._4_4_ == 1) {
            if ((local_e0 < (ulong)param_1[0x146]) && (*(uint *)((long)param_1 + 0xa54) < 600)) {
              iVar13 = (*(code *)param_1[0x14])(param_1[0xf],1,0,0);
              if (iVar13 == 1) {
                uVar23 = 0;
                goto LAB_00192864;
              }
LAB_00192af4:
              lVar25 = 0;
              lVar21 = 0;
              lVar24 = 0;
              uVar19 = 6;
              uVar16 = uStack_409b0;
              uVar5 = uStack_409a8;
            }
            else {
              lVar21 = 0;
              lVar24 = 0;
              uVar19 = 0xb;
              lVar25 = 0;
            }
          }
          else {
            lVar25 = 0;
            lVar21 = 0;
            lVar24 = 0;
            uVar19 = 10;
          }
        }
      }
      else {
        lVar25 = 0;
        lVar21 = 0;
        lVar24 = 0;
        uVar19 = 2;
      }
    }
    else {
      lVar21 = 0;
      lVar24 = 0;
      uVar19 = 9;
      lVar25 = 0;
    }
LAB_00192cb8:
    uStack_409a8 = uVar5;
    uStack_409b0 = uVar16;
    uStack_d8 = CONCAT44(uStack_d8._4_4_,uVar19);
  }
LAB_00192cbc:
  if (((((int)uStack_d8 == 1) && (lVar24 != 0)) && (lVar21 != 0)) && (lVar25 != 0)) {
    DAT_0031a0b8 = (lVar8 * -1000000 - (ulong)lVar10 / 1000) + lVar24 + DAT_0031a0b8;
    DAT_0031a0c0 = (lVar21 - lVar24) + DAT_0031a0c0;
    DAT_0031a0c8 = (lVar25 - lVar21) + DAT_0031a0c8;
    clock_gettime(1,&local_90);
    DAT_0031a0d8 = DAT_0031a0d8 + (local_d0 & 0xffffffff);
    DAT_0031a0d0 = (local_90.tv_sec * 1000000 - lVar25) + (ulong)local_90.tv_nsec / 1000 +
                   DAT_0031a0d0;
    DAT_0031a0e0 = DAT_0031a0e0 + 1;
    uVar18 = (ulong)DAT_0031a0e0;
    if (0x77 < DAT_0031a0e0) {
      uVar27 = 0;
      if (uVar18 != 0) {
        uVar27 = DAT_0031a0d0 / uVar18;
      }
      uVar29 = 0;
      if (uVar18 != 0) {
        uVar29 = DAT_0031a0b8 / uVar18;
      }
      uVar16 = 0;
      if (uVar18 != 0) {
        uVar16 = DAT_0031a0c0 / uVar18;
      }
      uVar5 = 0;
      if (uVar18 != 0) {
        uVar5 = DAT_0031a0c8 / uVar18;
      }
      uVar6 = 0;
      if (uVar18 != 0) {
        uVar6 = DAT_0031a0d8 / uVar18;
      }
      __android_log_print(4,"NexusPerfGL",
                          "frames=%u pre_us=%llu geometry_us=%llu gl_us=%llu restore_us=%llu rects=%llu"
                          ,uVar18,uVar29,uVar16,uVar5,uVar27,uVar6);
      DAT_0031a0e0 = 0;
      DAT_0031a0d8 = 0;
      DAT_0031a0d0 = 0;
      DAT_0031a0c8 = 0;
      DAT_0031a0c0 = 0;
      DAT_0031a0b8 = 0;
    }
  }
  pbVar2 = (byte *)((long)param_1 + 0xaa);
  do {
    bVar11 = *pbVar2;
    cVar3 = '\x01';
    bVar4 = (bool)ExclusiveMonitorPass(pbVar2,0x10);
    if (bVar4) {
      *pbVar2 = 1;
      cVar3 = ExclusiveMonitorsStatus();
    }
  } while (cVar3 != '\0');
  if ((bVar11 & 1) == 0) {
    lStack_108 = param_1[0x136];
    param_1[0x13e] = uStack_c8;
    param_1[0x13d] = local_d0;
    param_1[0x140] = lStack_b8;
    param_1[0x13f] = local_c0;
    if ((int)uStack_d8 == 1) {
      lStack_108 = lStack_108 + 1;
    }
    local_110 = param_1[0x135] + 1;
    local_100 = param_1[0x137] + (ulong)(((int)uStack_d8 == 1) != ((int)uStack_d8 != 0));
    param_1[0x142] = uStack_a8;
    param_1[0x141] = local_b0;
    param_1[0x144] = uStack_98;
    param_1[0x143] = local_a0;
    param_1[0x13a] = lStack_e8;
    param_1[0x139] = local_f0;
    param_1[0x13c] = uStack_d8;
    param_1[0x13b] = local_e0;
    param_1[0x136] = lStack_108;
    param_1[0x135] = local_110;
    param_1[0x138] = uStack_f8;
    param_1[0x137] = local_100;
    *(undefined4 *)((long)param_1 + 0xaa) = 0;
  }
  pbVar1[0] = 0;
  pbVar1[1] = 0;
  pbVar1[2] = 0;
  pbVar1[3] = 0;
  iVar13 = (int)uStack_d8;
LAB_00192e98:
  if (*(long *)(lVar7 + 0x28) == local_78) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar13);
}

/* ===== FUN_001933dc @ 001933dc [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_001933dc(int *param_1,int *param_2,long param_3,uint param_4,uint *param_5,
                 undefined8 *param_6)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  long lVar9;
  undefined8 uVar10;
  int iVar11;
  undefined8 uVar12;
  int *piVar13;
  uint uVar14;
  float *pfVar15;
  ulong uVar16;
  undefined8 uVar17;
  float fVar18;
  undefined8 uVar19;
  float fVar20;
  char acStack_d8 [32];
  undefined8 *local_b8;
  uint *local_b0;
  undefined4 local_a8;
  undefined4 uStack_a4;
  undefined4 uStack_a0;
  undefined4 uStack_9c;
  int local_98;
  long local_90;
  
  lVar9 = tpidr_el0;
  local_90 = *(long *)(lVar9 + 0x28);
  if (param_6 == (undefined8 *)0x0) {
LAB_001934dc:
    uVar12 = 0;
    goto LAB_001934e0;
  }
  uVar12 = 0;
  *param_6 = 0;
  *(undefined4 *)(param_6 + 1) = 0;
  if (((param_1 == (int *)0x0) || (param_2 == (int *)0x0)) || (param_5 == (uint *)0x0))
  goto LAB_001934e0;
  if ((((int)*param_5 < 0) || ((int)param_5[1] < 0)) || ((int)param_5[2] < 1)) goto LAB_001934dc;
  uVar12 = 0;
  if ((((int)param_5[3] < 1) || (0x2000 < *param_5)) ||
     ((0x2000 < param_5[1] ||
      ((((0x2000 < param_5[2] || (uVar12 = 0, param_3 < 0)) || (0x2000 < param_5[3])) ||
       (uVar12 = FUN_001902ec(param_1,*(undefined8 *)(param_1 + 0x34),param_3), (int)uVar12 == 0))))
     )) goto LAB_001934e0;
  if (((*(long *)(param_2 + 2) != *(long *)(param_1 + 0x38)) ||
      (iVar11 = param_2[0xc], iVar11 - 0x191U < 0xfffffe88)) ||
     (uVar14 = param_2[0xd], uVar14 - 0x4b1 < 0xfffffb68)) goto LAB_001934dc;
  local_a8 = 0;
  uStack_a4 = 0;
  uStack_a0 = 0;
  uStack_9c = 0;
  local_98 = 1;
  *(undefined4 *)((long)param_6 + 4) = 2;
  *(uint *)(param_6 + 1) = param_4 & 3;
  uVar10 = DAT_0010e790;
  uVar19 = DAT_0010e728;
  uVar12 = DAT_0010e5a8;
  local_b8 = param_6;
  local_b0 = param_5;
  if (((param_4 & 1) != 0) && (param_2[8] == 1)) {
    uVar4 = (uVar14 >> 2 & 0x3fff) / 0x19;
    uVar14 = (uint)(((float)iVar11 / 100.0) * (float)(int)param_5[2] * 0.038);
    if ((int)uVar14 < 0x17) {
      uVar14 = 0x16;
    }
    if (0x59 < uVar14) {
      uVar14 = 0x5a;
    }
    if (uVar4 < 3) {
      uVar4 = 2;
    }
    if (6 < uVar4) {
      uVar4 = 7;
    }
    if (param_1[0x47] != 0) {
      uVar16 = 0;
      pfVar15 = (float *)(param_1 + 0x55);
      iVar11 = uVar14 + uVar4 * -2;
      do {
        fVar20 = pfVar15[-5];
        piVar13 = param_2 + 9;
        if ((fVar20 != 0.0) && (piVar13 = param_2 + 10, fVar20 != 1.4013e-45)) {
          piVar13 = param_2 + 0xb;
        }
        if (*piVar13 == 1) {
          iVar1 = *param_5 + (int)((pfVar15[-1] * (float)(int)param_5[2]) / (float)param_1[0x3e]);
          iVar5 = iVar1 - (uVar14 >> 1);
          if (fVar20 == 2.8026e-45) {
            uStack_a0 = 0x3e800000;
            uVar17 = uVar19;
          }
          else if (fVar20 == 1.4013e-45) {
            uStack_a0 = 0x3f800000;
            uVar17 = uVar10;
          }
          else {
            uStack_a0 = 0x3e3851ec;
            uVar17 = uVar12;
          }
          local_a8 = (undefined4)uVar17;
          uStack_a4 = (undefined4)((ulong)uVar17 >> 0x20);
          iVar6 = ((param_5[3] + param_5[1]) - (uVar14 >> 1)) -
                  (int)((*pfVar15 * (float)(int)param_5[3]) / (float)param_1[0x3f]);
          uStack_9c = 0x3f800000;
          FUN_00193d88(&local_b8,iVar5,(uVar14 - uVar4) + iVar6,uVar14,uVar4,0);
          FUN_00193d88(&local_b8,iVar5,iVar6,uVar14,uVar4,0);
          FUN_00193d88(&local_b8,iVar5,iVar6 + uVar4,uVar4,iVar11,0);
          FUN_00193d88(&local_b8,(uVar14 - uVar4) + iVar5,iVar6 + uVar4,uVar4,iVar11,0);
          if (param_2[0xe] == 1) {
            iVar6 = iVar6 - param_5[1];
            if (0 < iVar6) {
              FUN_00193d88(&local_b8,iVar1 - (uVar4 >> 1),param_5[1],uVar4,iVar6,0);
            }
          }
        }
        uVar16 = uVar16 + 1;
        pfVar15 = pfVar15 + 0x12;
      } while (uVar16 < (uint)param_1[0x47]);
    }
  }
  if ((((param_4 >> 1 & 1) == 0) || (param_2[4] != 1)) ||
     (iVar11 = FUN_00190ad0(param_1,*(undefined8 *)(param_1 + 0x34),param_3), iVar11 == 0))
  goto LAB_00193bb8;
  iVar11 = *param_5 + (int)(((float)param_1[0x202] * (float)(int)param_5[2]) / (float)param_1[0x3e])
  ;
  uVar14 = (uint)(((float)(int)param_5[2] * 30.0) / (float)param_1[0x3e]);
  if ((int)uVar14 < 0x17) {
    uVar14 = 0x16;
  }
  iVar1 = (param_5[3] + param_5[1]) -
          (int)(((float)param_1[0x203] * (float)(int)param_5[3]) / (float)param_1[0x3f]);
  if (0x3b < uVar14) {
    uVar14 = 0x3c;
  }
  if (param_1[0x1ff] == 1) {
    iVar5 = iVar11 - uVar14;
    uVar4 = uVar14 << 1;
    uStack_a0 = (undefined4)_UNK_001129e8;
    uStack_9c = (undefined4)((ulong)_UNK_001129e8 >> 0x20);
    local_a8 = (undefined4)_DAT_001129e0;
    uStack_a4 = (undefined4)((ulong)_DAT_001129e0 >> 0x20);
    FUN_00193d88(&local_b8,iVar5,(uVar14 - 4) + iVar1,uVar4,4,1);
    iVar6 = iVar1 - uVar14;
    FUN_00193d88(&local_b8,iVar5,iVar6,uVar4,4,1);
    FUN_00193d88(&local_b8,iVar5,iVar6,4,uVar4,1);
    iVar5 = (uVar14 - 4) + iVar11;
  }
  else {
    uVar4 = uVar14 * 3 >> 2;
    iVar5 = iVar1 + uVar14 + -4;
    iVar7 = iVar11 - uVar14;
    uStack_a0 = (undefined4)_UNK_00112a88;
    uStack_9c = (undefined4)((ulong)_UNK_00112a88 >> 0x20);
    local_a8 = (undefined4)_DAT_00112a80;
    uStack_a4 = (undefined4)((ulong)_DAT_00112a80 >> 0x20);
    FUN_00193d88(&local_b8,iVar7,iVar5,uVar4,4,1);
    iVar6 = (uVar14 + iVar11) - uVar4;
    FUN_00193d88(&local_b8,iVar6,iVar5,uVar4,4,1);
    iVar8 = iVar1 - uVar14;
    FUN_00193d88(&local_b8,iVar7,iVar8,uVar4,4,1);
    FUN_00193d88(&local_b8,iVar6,iVar8,uVar4,4,1);
    FUN_00193d88(&local_b8,iVar7,iVar8,4,uVar4,1);
    iVar6 = (iVar1 + uVar14) - uVar4;
    FUN_00193d88(&local_b8,iVar7,iVar6,4,uVar4,1);
    iVar5 = uVar14 + iVar11 + -4;
    FUN_00193d88(&local_b8,iVar5,iVar8,4,uVar4,1);
  }
  FUN_00193d88(&local_b8,iVar5,iVar6,4,uVar4,1);
  uVar3 = (uVar14 & 0xff) / 3;
  uVar4 = ((uVar14 & 0x7f) * 2) / 3;
  FUN_00193d88(&local_b8,iVar11 - uVar3,iVar1 + -2,uVar4,4,1);
  FUN_00193d88(&local_b8,iVar11 + -2,iVar1 - uVar3,4,uVar4,1);
  switch(param_1[0x201]) {
  case 1:
    fVar18 = 20.0;
    break;
  case 2:
    fVar18 = -20.0;
    break;
  case 3:
    fVar20 = -20.0;
    goto LAB_00193b4c;
  case 4:
    fVar20 = 20.0;
LAB_00193b4c:
    fVar18 = (float)iVar11;
    fVar20 = (float)iVar1 + fVar20;
    FUN_00193efc(fVar18,(float)iVar1,fVar18 + -12.0,fVar20,&local_b8,4);
    fVar18 = fVar18 + 12.0;
    goto LAB_00193b80;
  default:
    goto switchD_00193acc_default;
  }
  fVar20 = (float)iVar1;
  fVar18 = (float)iVar11 + fVar18;
  FUN_00193efc((float)iVar11,fVar20,fVar18,fVar20 + 12.0,&local_b8,4);
  fVar20 = fVar20 + -12.0;
LAB_00193b80:
  FUN_00193efc((float)iVar11,(float)iVar1,fVar18,fVar20,&local_b8,4);
switchD_00193acc_default:
  iVar5 = -0xf - uVar14;
  if (param_1[0x201] != 3) {
    iVar5 = uVar14 + 5;
  }
  FUN_001940d0(&local_b8,param_1[0x1fe],iVar11,iVar5 + iVar1);
LAB_00193bb8:
  FUN_001941a8(&local_b8,param_1,param_2,param_3,param_4);
  if ((param_4 & 1) != 0) {
    if (param_2[1] != 0) {
      snprintf(acStack_d8,0x20,"DPS %d",(ulong)(uint)param_1[1]);
      uVar3 = local_b0[3];
      uVar14 = *local_b0;
      uVar4 = local_b0[1];
      local_a8 = 0x3ccccccd;
      uStack_9c = 0x3f800000;
      uStack_a4 = (undefined4)DAT_0010e798;
      uStack_a0 = (undefined4)((ulong)DAT_0010e798 >> 0x20);
      FUN_00193d88(&local_b8,uVar14 + 0xe,uVar3 + uVar4 + -0xb6,0xbe,0x17,1);
      uVar19 = NEON_fmov(0x3f800000,4);
      local_a8 = 0x3f800000;
      uVar12 = NEON_cmgt(CONCAT44(param_1[1],param_1[1]),DAT_0010e6e8,4);
      uStack_9c = 0x3f800000;
      uVar12 = NEON_bsl(uVar12,DAT_0010e650,uVar19,1);
      uStack_a4 = (undefined4)uVar12;
      uStack_a0 = (undefined4)((ulong)uVar12 >> 0x20);
      FUN_00194be4(&local_b8,acStack_d8,uVar14 + 0x14,uVar3 + uVar4 + -0xb2);
    }
    if ((*param_2 != 0) && (0 < *param_1)) {
      snprintf(acStack_d8,0x20,"%d MS");
      uVar14 = *local_b0;
      uVar3 = local_b0[1];
      uVar4 = local_b0[2];
      uVar2 = local_b0[3];
      local_a8 = 0x3ccccccd;
      uStack_a4 = 0x3ccccccd;
      uStack_a0 = 0x3d0f5c29;
      uStack_9c = 0x3f800000;
      FUN_00193d88(&local_b8,uVar4 + uVar14 + -0x9c,uVar2 + uVar3 + -0x2e,0x8c,0x17,1);
      uVar12 = NEON_cmgt(DAT_0010e7b8,CONCAT44(*param_1,*param_1),4);
      uVar12 = NEON_bsl(uVar12,DAT_0010e558,DAT_0010e538,1);
      local_a8 = (undefined4)uVar12;
      uStack_a4 = (undefined4)((ulong)uVar12 >> 0x20);
      uStack_a0 = (undefined4)DAT_0010e770;
      uStack_9c = (undefined4)((ulong)DAT_0010e770 >> 0x20);
      FUN_00194be4(&local_b8,acStack_d8,uVar4 + uVar14 + -0x96,uVar2 + uVar3 + -0x2a);
    }
  }
  FUN_00194ddc(&local_b8,param_1,param_2,param_4);
  if (local_98 == 0) {
    uVar12 = 0;
    *param_6 = 0;
    *(undefined4 *)(param_6 + 1) = 0;
  }
  else {
    uVar12 = 1;
  }
LAB_001934e0:
  if (*(long *)(lVar9 + 0x28) != local_90) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar12);
  }
  return;
}

/* ===== FUN_001941a8 @ 001941a8 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_001941a8(long param_1,long param_2,long param_3,ulong param_4,ulong param_5)

{
  bool bVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  long lVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  size_t sVar9;
  int iVar10;
  int *piVar11;
  int *piVar12;
  uint uVar13;
  ulong uVar14;
  long lVar15;
  ulong uVar16;
  float *pfVar17;
  int iVar18;
  ulong uVar19;
  int iVar20;
  float fVar21;
  undefined4 uVar22;
  float fVar23;
  undefined8 uVar24;
  int iVar25;
  float fVar26;
  float fVar27;
  char acStack_c8 [32];
  long local_a8;
  
  uVar6 = _DAT_00112a30;
  lVar5 = tpidr_el0;
  local_a8 = *(long *)(lVar5 + 0x28);
  if ((param_5 & 1) == 0) goto LAB_00194874;
  if ((((*(int *)(param_3 + 100) != 0) && (*(int *)(param_2 + 0x820) != 0)) &&
      (0xffffff9b < *(int *)(param_2 + 8) - 0x65U)) &&
     (0xffffff9b < *(int *)(param_2 + 0xc) - 0x65U)) {
    iVar18 = -1;
    *(undefined8 *)(param_1 + 0x18) = _UNK_00112a38;
    *(undefined8 *)(param_1 + 0x10) = uVar6;
    do {
      iVar18 = iVar18 + 1;
      FUN_001961dc((float)iVar18,0,(float)iVar18,(float)*(int *)(param_2 + 0xc),param_1,param_2);
    } while (iVar18 < *(int *)(param_2 + 8));
    if (-1 < *(int *)(param_2 + 0xc)) {
      iVar18 = -1;
      do {
        iVar18 = iVar18 + 1;
        FUN_001961dc(0,(float)iVar18,(float)*(int *)(param_2 + 8),(float)iVar18,param_1,param_2);
      } while (iVar18 < *(int *)(param_2 + 0xc));
    }
  }
  if ((*(int *)(param_3 + 0x60) != 0) && (*(int *)(param_2 + 0x18) != 0)) {
    fVar21 = sinf(((float)(param_4 % 600) * 6.2831855) / 600.0);
    uVar8 = _UNK_00112928;
    uVar7 = _DAT_00112920;
    uVar6 = DAT_0010e770;
    uVar22 = NEON_fmadd(fVar21,0x3f000000,0x3f000000);
    uVar19 = 0;
    piVar12 = (int *)(param_2 + 0x28);
    fVar21 = (float)NEON_fmadd(uVar22,0x3f19999a,0x3ecccccd);
    do {
      piVar11 = *(int **)(param_1 + 8);
      iVar4 = *piVar12;
      iVar25 = piVar11[2];
      iVar18 = *piVar11;
      fVar26 = (float)piVar12[-3];
      fVar27 = *(float *)(param_2 + 0xf8);
      iVar20 = (piVar11[3] + piVar11[1]) -
               (int)(((float)piVar12[-2] * (float)piVar11[3]) / *(float *)(param_2 + 0xfc));
      *(undefined8 *)(param_1 + 0x18) = uVar8;
      *(undefined8 *)(param_1 + 0x10) = uVar7;
      iVar3 = iVar20 + 0x2c;
      if (iVar4 != 0) {
        iVar3 = iVar20;
      }
      iVar18 = iVar18 + (int)((fVar26 * (float)iVar25) / fVar27);
      FUN_00193d88(param_1,iVar18 + -0x1d,iVar3 + -4,0x3a,10,1);
      fVar26 = fVar21;
      fVar27 = 0.0;
      if (0.25 < (float)piVar12[-1]) {
        fVar23 = ((float)piVar12[-1] + -0.25) / 0.75;
        fVar26 = (1.0 - fVar23) + (1.0 - fVar23);
        fVar27 = 1.0;
        if (fVar23 == 0.5 || fVar23 < 0.5 != NAN(fVar23)) {
          fVar26 = 1.0;
          fVar27 = fVar23 + fVar23;
        }
      }
      *(float *)(param_1 + 0x10) = fVar26;
      *(float *)(param_1 + 0x14) = fVar27;
      *(undefined8 *)(param_1 + 0x18) = uVar6;
      FUN_00193d88(param_1,iVar18 + -0x1a,iVar3 + -1,(int)((float)piVar12[-1] * 52.0),4,1);
      iVar4 = *piVar12;
      if (iVar4 == 1) {
        FUN_00193efc((float)(iVar18 + -0x10),(float)(iVar3 + 9),(float)(iVar18 + -0x19),
                     (float)(iVar3 + 0x10),param_1,2);
        FUN_00193efc((float)(iVar18 + -0x19),(float)(iVar3 + 0x10),(float)(iVar18 + -0x10),
                     (float)(iVar3 + 0x17),param_1,2);
        iVar4 = *piVar12;
      }
      if (iVar4 == 2) {
        FUN_00193efc((float)(iVar18 + 0x10),(float)(iVar3 + 9),(float)(iVar18 + 0x19),
                     (float)(iVar3 + 0x10),param_1,2);
        FUN_00193efc((float)(iVar18 + 0x19),(float)(iVar3 + 0x10),(float)(iVar18 + 0x10),
                     (float)(iVar3 + 0x17),param_1,2);
        iVar4 = *piVar12;
      }
      if (iVar4 == 3) {
        FUN_00193efc((float)(iVar18 + -7),(float)(iVar3 + -0x10),(float)iVar18,
                     (float)(iVar3 + -0x19),param_1,2);
        FUN_00193efc((float)iVar18,(float)(iVar3 + -0x19),(float)(iVar18 + 7),(float)(iVar3 + -0x10)
                     ,param_1,2);
        iVar4 = *piVar12;
      }
      if (iVar4 == 4) {
        FUN_00193efc((float)(iVar18 + -7),(float)(iVar3 + 0x10),(float)iVar18,(float)(iVar3 + 0x19),
                     param_1,2);
        FUN_00193efc((float)iVar18,(float)(iVar3 + 0x19),(float)(iVar18 + 7),(float)(iVar3 + 0x10),
                     param_1,2);
      }
      if (6 < uVar19) break;
      uVar19 = uVar19 + 1;
      piVar12 = piVar12 + 5;
    } while (uVar19 < *(uint *)(param_2 + 0x18));
  }
  iVar18 = *(int *)(param_3 + 0x68);
  if ((iVar18 == 0) && (*(int *)(param_3 + 0x6c) == 0)) {
    DAT_0031a0f0 = 0;
    DAT_0031a0e8 = 0;
    _DAT_0031a100 = 0;
    _DAT_0031a0f8 = 0;
    uRam000000000031a110 = 0;
    _DAT_0031a108 = 0;
    uRam000000000031a120 = 0;
    _DAT_0031a118 = 0;
    uRam000000000031a130 = 0;
    _DAT_0031a128 = 0;
    uRam000000000031a140 = 0;
    _DAT_0031a138 = 0;
    uRam000000000031a150 = 0;
    _DAT_0031a148 = 0;
    uRam000000000031a160 = 0;
    _DAT_0031a158 = 0;
    uRam000000000031a170 = 0;
    _DAT_0031a168 = 0;
    uRam000000000031a180 = 0;
    _DAT_0031a178 = 0;
    uRam000000000031a190 = 0;
    _DAT_0031a188 = 0;
    uRam000000000031a1a0 = 0;
    _DAT_0031a198 = 0;
    uRam000000000031a1b0 = 0;
    _DAT_0031a1a8 = 0;
    uRam000000000031a1c0 = 0;
    _DAT_0031a1b8 = 0;
    uRam000000000031a1d0 = 0;
    _DAT_0031a1c8 = 0;
    uRam000000000031a1e0 = 0;
    _DAT_0031a1d8 = 0;
    uRam000000000031a1f0 = 0;
    _DAT_0031a1e8 = 0;
    uRam000000000031a200 = 0;
    _DAT_0031a1f8 = 0;
    uRam000000000031a210 = 0;
    _DAT_0031a208 = 0;
    uRam000000000031a220 = 0;
    _DAT_0031a218 = 0;
    uRam000000000031a230 = 0;
    _DAT_0031a228 = 0;
    uRam000000000031a240 = 0;
    _DAT_0031a238 = 0;
    uRam000000000031a250 = 0;
    _DAT_0031a248 = 0;
    uRam000000000031a260 = 0;
    _DAT_0031a258 = 0;
  }
  else if ((DAT_0031a0e8 == *(long *)(param_2 + 0xd0)) &&
          ((DAT_0031a0f0 <= param_4 && (param_4 - DAT_0031a0f0 < 0x3e9)))) {
    uVar13 = DAT_0031a0f8;
    if ((DAT_0031a0f0 == 0) || (param_4 <= DAT_0031a0f0)) {
      DAT_0031a0f0 = param_4;
      if (DAT_0031a0f8 == 0) goto LAB_001946e8;
    }
    else {
      uVar2 = DAT_0031a0fc + 1;
      (&DAT_0031a100)[DAT_0031a0fc] = (float)(param_4 - DAT_0031a0f0);
      if (uVar13 < 0x5a) {
        uVar13 = uVar13 + 1;
        DAT_0031a0f8 = uVar13;
      }
      _DAT_0031a0f8 = CONCAT44(uVar2 % 0x5a,DAT_0031a0f8);
    }
    uVar19 = (ulong)uVar13;
    if (uVar13 < 2) {
      fVar21 = 0.0;
      uVar14 = 0;
LAB_0019495c:
      lVar15 = uVar19 - uVar14;
      pfVar17 = (float *)(&DAT_0031a100 + uVar14);
      do {
        lVar15 = lVar15 + -1;
        fVar21 = fVar21 + *pfVar17;
        pfVar17 = pfVar17 + 1;
      } while (lVar15 != 0);
    }
    else {
      uVar14 = uVar19 & 0xfffffffe;
      fVar21 = 0.0;
      uVar16 = uVar14;
      pfVar17 = (float *)&DAT_0031a104;
      do {
        uVar16 = uVar16 - 2;
        fVar21 = fVar21 + pfVar17[-1] + *pfVar17;
        pfVar17 = pfVar17 + 2;
      } while (uVar16 != 0);
      if (uVar14 != uVar19) goto LAB_0019495c;
    }
    DAT_0031a0f0 = param_4;
    if (iVar18 != 0) {
      piVar12 = *(int **)(param_1 + 8);
      iVar18 = *piVar12;
      iVar3 = piVar12[1];
      fVar21 = fVar21 / (float)uVar19;
      iVar4 = piVar12[3];
      if (fVar21 == 0.0 || fVar21 < 0.0 != NAN(fVar21)) {
        uVar19 = 0;
      }
      else {
        uVar19 = (ulong)(uint)(int)(1000.0 / fVar21 + 0.5);
      }
      snprintf(acStack_c8,0x20,"%d FPS  %.1f MS",(double)fVar21,uVar19);
      uVar6 = _DAT_001127d0;
      *(undefined8 *)(param_1 + 0x18) = _UNK_001127d8;
      *(undefined8 *)(param_1 + 0x10) = uVar6;
      FUN_00193d88(param_1,iVar18 + 0xe,iVar4 + iVar3 + -0x35,0x104,0x18,1);
      uVar6 = _DAT_00112bd0;
      *(undefined8 *)(param_1 + 0x18) = _UNK_00112bd8;
      *(undefined8 *)(param_1 + 0x10) = uVar6;
      FUN_00194be4(param_1,acStack_c8,iVar18 + 0x14,iVar4 + iVar3 + -0x30);
    }
    uVar6 = _DAT_001127d0;
    if (*(int *)(param_3 + 0x6c) != 0) {
      piVar12 = *(int **)(param_1 + 8);
      iVar3 = piVar12[1];
      uVar2 = piVar12[2];
      iVar4 = *piVar12;
      iVar18 = piVar12[3];
      *(undefined8 *)(param_1 + 0x18) = _UNK_001127d8;
      *(undefined8 *)(param_1 + 0x10) = uVar6;
      uVar13 = uVar2;
      if (0x21b < (int)uVar2) {
        uVar13 = 0x21c;
      }
      iVar18 = iVar18 + iVar3;
      if ((int)uVar13 < 0x169) {
        uVar13 = 0x168;
      }
      iVar20 = (iVar4 + uVar2) - uVar13;
      iVar4 = uVar13 - 0x10e;
      iVar3 = iVar20 + 0xf6;
      FUN_00193d88(param_1,iVar20 + 0xf0,iVar18 + -0x96,uVar13 - 0x102,0x3c,1);
      uVar6 = _DAT_00112c20;
      *(undefined8 *)(param_1 + 0x18) = _UNK_00112c28;
      *(undefined8 *)(param_1 + 0x10) = uVar6;
      FUN_00193d88(param_1,iVar3,iVar18 + -0x81,iVar4,1,1);
      FUN_00193d88(param_1,iVar3,iVar18 + -0x71,iVar4,1,1);
      uVar8 = DAT_0010e758;
      uVar7 = DAT_0010e698;
      uVar6 = DAT_0010e618;
      if (DAT_0031a0f8 != 0) {
        iVar20 = 0;
        uVar22 = 1;
        if (0x1c1 < uVar13) {
          uVar22 = 2;
        }
        iVar25 = 0x5a;
        do {
          fVar26 = (float)(&DAT_0031a100)
                          [iVar25 + (DAT_0031a0fc - DAT_0031a0f8) +
                                    (((DAT_0031a0fc + iVar25) - DAT_0031a0f8) / 0x5a) * -0x5a];
          *(undefined8 *)(param_1 + 0x18) = uVar8;
          fVar21 = (float)NEON_fminnm(fVar26,0x42480000);
          iVar10 = (int)fVar21;
          if (iVar10 < 2) {
            iVar10 = 1;
          }
          bVar1 = fVar26 != 33.0 && fVar26 < 33.0 == NAN(fVar26);
          uVar24 = NEON_cmlt(CONCAT44((uint)bVar1 << 0x1f,(uint)bVar1 << 0x1f),0,4);
          uVar24 = NEON_bsl(uVar24,uVar7,uVar6,1);
          *(undefined8 *)(param_1 + 0x10) = uVar24;
          FUN_00193d88(param_1,iVar20 / 0x5a + iVar3,iVar18 + -0x92,uVar22,iVar10,1);
          uVar13 = iVar25 - 0x59;
          iVar20 = iVar20 + iVar4;
          iVar25 = iVar25 + 1;
        } while (uVar13 < DAT_0031a0f8);
      }
    }
  }
  else {
    uRam000000000031a110 = 0;
    _DAT_0031a108 = 0;
    uRam000000000031a120 = 0;
    _DAT_0031a118 = 0;
    uRam000000000031a130 = 0;
    _DAT_0031a128 = 0;
    uRam000000000031a140 = 0;
    _DAT_0031a138 = 0;
    uRam000000000031a150 = 0;
    _DAT_0031a148 = 0;
    uRam000000000031a160 = 0;
    _DAT_0031a158 = 0;
    uRam000000000031a170 = 0;
    _DAT_0031a168 = 0;
    uRam000000000031a180 = 0;
    _DAT_0031a178 = 0;
    uRam000000000031a190 = 0;
    _DAT_0031a188 = 0;
    uRam000000000031a1a0 = 0;
    _DAT_0031a198 = 0;
    uRam000000000031a1b0 = 0;
    _DAT_0031a1a8 = 0;
    uRam000000000031a1c0 = 0;
    _DAT_0031a1b8 = 0;
    uRam000000000031a1d0 = 0;
    _DAT_0031a1c8 = 0;
    uRam000000000031a1e0 = 0;
    _DAT_0031a1d8 = 0;
    uRam000000000031a1f0 = 0;
    _DAT_0031a1e8 = 0;
    uRam000000000031a200 = 0;
    _DAT_0031a1f8 = 0;
    uRam000000000031a210 = 0;
    _DAT_0031a208 = 0;
    uRam000000000031a220 = 0;
    _DAT_0031a218 = 0;
    uRam000000000031a230 = 0;
    _DAT_0031a228 = 0;
    uRam000000000031a240 = 0;
    _DAT_0031a238 = 0;
    uRam000000000031a250 = 0;
    _DAT_0031a248 = 0;
    uRam000000000031a260 = 0;
    _DAT_0031a258 = 0;
    _DAT_0031a100 = 0;
    _DAT_0031a0f8 = 0;
    DAT_0031a0e8 = *(long *)(param_2 + 0xd0);
    DAT_0031a0f0 = param_4;
  }
LAB_001946e8:
  if (*(int *)(param_3 + 0x70) != 0) {
    snprintf(acStack_c8,0x20,"X %.1f  Y %.1f",(double)*(float *)(param_2 + 0x870),
             (double)*(float *)(param_2 + 0x874));
    uVar6 = _DAT_001127d0;
    piVar12 = *(int **)(param_1 + 8);
    iVar4 = piVar12[3];
    iVar18 = *piVar12;
    iVar3 = piVar12[1];
    *(undefined8 *)(param_1 + 0x18) = _UNK_001127d8;
    *(undefined8 *)(param_1 + 0x10) = uVar6;
    FUN_00193d88(param_1,iVar18 + 0xe,iVar4 + iVar3 + -0x98,0x11a,0x17,1);
    uVar6 = s_expand_32_byte_kff__fff__00112ab0._16_8_;
    *(undefined8 *)(param_1 + 0x18) = ram0x00112ac8;
    *(undefined8 *)(param_1 + 0x10) = uVar6;
    FUN_00194be4(param_1,acStack_c8,iVar18 + 0x14,iVar4 + iVar3 + -0x94);
  }
  if ((*(int *)(param_3 + 0x74) != 0) && (*(int *)(param_2 + 0x10) - 1U < 0xb4)) {
    snprintf(acStack_c8,0x20,"RESPAWN %d");
    piVar12 = *(int **)(param_1 + 8);
    iVar18 = piVar12[2];
    iVar3 = *piVar12;
    if (iVar18 < 0) {
      iVar18 = iVar18 + 1;
    }
    sVar9 = strlen(acStack_c8);
    iVar4 = piVar12[1];
    iVar20 = piVar12[3];
    iVar18 = iVar3 + (iVar18 >> 1) + (int)sVar9 * -6;
    *(undefined8 *)(param_1 + 0x10) = 0x3ccccccd3ccccccd;
    *(undefined8 *)(param_1 + 0x18) = 0x3f8000003d0f5c29;
    FUN_00193d88(param_1,iVar18 + -6,iVar20 + iVar4 + -0x82,(int)sVar9 * 0xc + 0xc,0x17,1);
    uVar6 = DAT_0010e770;
    uVar22 = 0x3e4ccccd;
    if (*(int *)(param_2 + 0x14) < 2) {
      uVar22 = 0x3f19999a;
    }
    *(undefined4 *)(param_1 + 0x10) = 0x3f800000;
    *(undefined4 *)(param_1 + 0x14) = uVar22;
    *(undefined8 *)(param_1 + 0x18) = uVar6;
    FUN_00194be4(param_1,acStack_c8,iVar18,iVar20 + iVar4 + -0x7e);
  }
LAB_00194874:
  if (*(long *)(lVar5 + 0x28) == local_a8) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_001983dc @ 001983dc [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_001983dc(long *param_1,ulong param_2,undefined4 *param_3)

{
  char *pcVar1;
  byte bVar2;
  long lVar3;
  undefined8 uVar4;
  void *pvVar5;
  ulong uVar6;
  char *pcVar7;
  long lVar8;
  ulong uVar9;
  uint uVar10;
  long local_160 [5];
  ulong local_138 [5];
  undefined1 auStack_10c [15];
  char local_fd [181];
  long local_48;
  
  lVar3 = tpidr_el0;
  uVar4 = 0;
  local_48 = *(long *)(lVar3 + 0x28);
  if (((0xbf < param_2 - 1) || (param_1 == (long *)0x0)) || (param_3 == (undefined4 *)0x0))
  goto LAB_00198480;
  pvVar5 = memchr(param_1,0,param_2);
  uVar4 = 0;
  if ((param_2 < 0x10) || (pvVar5 != (void *)0x0)) goto LAB_00198480;
  if (*param_1 == 0x4c475f535558454e && *(long *)((long)param_1 + 7) == 0x2031565f4c41434c) {
    memcpy(auStack_10c,param_1,param_2);
    uVar6 = 0;
    pcVar7 = local_fd;
    local_160[0] = 0x7fffffff;
    local_160[1] = 0xffffffffffffffff;
    auStack_10c[param_2] = 0;
    local_160[2] = 0xffffffffffffffff;
    local_160[4] = _UNK_00112828;
    local_160[3] = _DAT_00112820;
LAB_00198508:
    lVar8 = 0;
    uVar9 = 0;
    do {
      bVar2 = pcVar7[lVar8];
      uVar10 = bVar2 - 0x30;
      if (9 < uVar10) {
        if (5 < bVar2 - 0x61) goto LAB_0019855c;
        uVar10 = bVar2 - 0x57;
      }
      if (9 < uVar10) goto LAB_0019855c;
      if ((local_160[uVar6] - (ulong)uVar10) / 10 < uVar9) break;
      uVar9 = (ulong)uVar10 + uVar9 * 10;
      lVar8 = lVar8 + 1;
    } while( true );
  }
LAB_0019847c:
  uVar4 = 0;
LAB_00198480:
  if (*(long *)(lVar3 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar4);
LAB_0019855c:
  if ((int)lVar8 == 0) goto LAB_0019847c;
  pcVar1 = pcVar7 + lVar8;
  local_138[uVar6] = uVar9;
  pcVar7 = pcVar1;
  if ((uVar6 < 4) && (pcVar7 = pcVar1 + 1, *pcVar1 != ' ')) goto LAB_0019847c;
  uVar6 = uVar6 + 1;
  if (uVar6 == 5) goto LAB_00198580;
  goto LAB_00198508;
LAB_00198580:
  if (*pcVar7 == '\n') {
    uVar4 = 0;
    if ((((pcVar7[1] == '\0') && (local_138[0] != 0)) && (local_138[1] != 0)) &&
       ((local_138[2] != 0 && (local_138[3] != 0)))) {
      uVar4 = 1;
      *(ulong *)(param_3 + 2) = local_138[1];
      *(ulong *)(param_3 + 4) = local_138[2];
      *(ulong *)(param_3 + 6) = local_138[3];
      *param_3 = (int)local_138[0];
      param_3[1] = (int)local_138[4];
    }
    goto LAB_00198480;
  }
  goto LAB_0019847c;
}

/* ===== FUN_00198fa8 @ 00198fa8 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_00198fa8(undefined8 *param_1,long param_2,uint *param_3)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  long lVar5;
  undefined8 uVar6;
  long lVar7;
  byte bVar8;
  byte bVar9;
  byte bVar10;
  byte bVar11;
  byte bVar12;
  byte bVar13;
  byte bVar14;
  byte bVar16;
  byte bVar17;
  byte bVar18;
  byte bVar19;
  byte bVar20;
  byte bVar21;
  undefined8 uVar15;
  byte bVar22;
  undefined1 local_168 [112];
  byte local_f8;
  byte local_f7;
  undefined1 local_f6;
  undefined1 local_f5;
  undefined1 local_f4;
  undefined1 local_f3;
  undefined1 local_f2;
  undefined1 local_f1;
  undefined1 local_f0;
  undefined1 local_ef;
  undefined1 local_ee;
  undefined1 local_ed;
  undefined1 local_ec;
  undefined1 local_eb;
  undefined1 local_ea;
  undefined1 local_e9;
  undefined1 local_e8;
  undefined1 local_e7;
  undefined1 local_e6;
  undefined1 local_e5;
  undefined1 local_e4;
  undefined1 local_e3;
  undefined1 local_e2;
  undefined1 local_e1;
  undefined1 local_e0;
  undefined1 local_df;
  undefined1 local_de;
  undefined1 local_dd;
  undefined1 local_dc;
  undefined1 local_db;
  undefined1 local_da;
  undefined1 local_d9;
  undefined8 local_d8;
  undefined4 local_d0;
  undefined4 local_cc;
  undefined1 local_c8;
  undefined1 local_c7;
  undefined1 local_c6;
  undefined1 local_c5;
  undefined8 local_c0;
  undefined8 uStack_b8;
  undefined8 local_b0;
  undefined8 local_a8;
  undefined8 local_a0;
  undefined8 uStack_98;
  undefined8 local_90;
  undefined8 local_88;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined8 local_70;
  undefined8 local_68;
  undefined8 local_60;
  undefined8 uStack_58;
  undefined8 local_50;
  undefined8 local_48;
  long local_38;
  
  lVar5 = tpidr_el0;
  uVar6 = 0;
  local_38 = *(long *)(lVar5 + 0x28);
  if (((param_1 != (undefined8 *)0x0) && (param_2 != 0)) && (param_3 != (uint *)0x0)) {
    iVar4 = 0x6b7c8d9e;
    iVar3 = *(int *)(param_2 + 8);
    if (iVar3 != 0xf) {
      iVar4 = 0;
    }
    iVar2 = -0x736251f1;
    if (iVar3 != 9) {
      iVar2 = iVar4;
    }
    if (iVar2 == 0) {
      uVar6 = 0;
    }
    else {
      uVar15 = param_1[1];
      uVar6 = *param_1;
      local_c8 = (undefined1)iVar2;
      uStack_58 = 0x3636363636363636;
      local_60 = 0x3636363636363636;
      local_48 = 0x3636363636363636;
      local_50 = 0x3636363636363636;
      local_d8 = *(undefined8 *)(param_2 + 8);
      bVar8 = (byte)((ulong)uVar6 >> 8);
      bVar9 = (byte)((ulong)uVar6 >> 0x10);
      bVar10 = (byte)((ulong)uVar6 >> 0x18);
      bVar11 = (byte)((ulong)uVar6 >> 0x20);
      bVar12 = (byte)((ulong)uVar6 >> 0x28);
      bVar13 = (byte)((ulong)uVar6 >> 0x30);
      bVar14 = (byte)((ulong)uVar6 >> 0x38);
      local_80 = CONCAT17(bVar14 ^ (byte)((ulong)_DAT_001128b0 >> 0x38),
                          CONCAT16(bVar13 ^ (byte)((ulong)_DAT_001128b0 >> 0x30),
                                   CONCAT15(bVar12 ^ (byte)((ulong)_DAT_001128b0 >> 0x28),
                                            CONCAT14(bVar11 ^ (byte)((ulong)_DAT_001128b0 >> 0x20),
                                                     CONCAT13(bVar10 ^ (byte)((ulong)_DAT_001128b0
                                                                             >> 0x18),
                                                              CONCAT12(bVar9 ^ (byte)((ulong)
                                                  _DAT_001128b0 >> 0x10),
                                                  CONCAT11(bVar8 ^ (byte)((ulong)_DAT_001128b0 >> 8)
                                                           ,(byte)uVar6 ^ (byte)_DAT_001128b0)))))))
      ;
      bVar16 = (byte)((ulong)uVar15 >> 8);
      bVar17 = (byte)((ulong)uVar15 >> 0x10);
      bVar18 = (byte)((ulong)uVar15 >> 0x18);
      bVar19 = (byte)((ulong)uVar15 >> 0x20);
      bVar20 = (byte)((ulong)uVar15 >> 0x28);
      bVar21 = (byte)((ulong)uVar15 >> 0x30);
      bVar22 = (byte)((ulong)uVar15 >> 0x38);
      uStack_78 = CONCAT17(bVar22 ^ (byte)((ulong)_UNK_001128b8 >> 0x38),
                           CONCAT16(bVar21 ^ (byte)((ulong)_UNK_001128b8 >> 0x30),
                                    CONCAT15(bVar20 ^ (byte)((ulong)_UNK_001128b8 >> 0x28),
                                             CONCAT14(bVar19 ^ (byte)((ulong)_UNK_001128b8 >> 0x20),
                                                      CONCAT13(bVar18 ^ (byte)((ulong)_UNK_001128b8
                                                                              >> 0x18),
                                                               CONCAT12(bVar17 ^ (byte)((ulong)
                                                  _UNK_001128b8 >> 0x10),
                                                  CONCAT11(bVar16 ^ (byte)((ulong)_UNK_001128b8 >> 8
                                                                          ),
                                                           (byte)uVar15 ^ (byte)_UNK_001128b8)))))))
      ;
      local_d0 = *(undefined4 *)(param_2 + 0x10);
      local_68 = 0x3636363636363636;
      local_70 = 0x3636363636363636;
      uStack_98 = 0x5c5c5c5c5c5c5c5c;
      local_a0 = 0x5c5c5c5c5c5c5c5c;
      local_88 = 0x5c5c5c5c5c5c5c5c;
      local_90 = 0x5c5c5c5c5c5c5c5c;
      uStack_b8 = CONCAT17(bVar22 ^ (byte)((ulong)_UNK_00112b68 >> 0x38),
                           CONCAT16(bVar21 ^ (byte)((ulong)_UNK_00112b68 >> 0x30),
                                    CONCAT15(bVar20 ^ (byte)((ulong)_UNK_00112b68 >> 0x28),
                                             CONCAT14(bVar19 ^ (byte)((ulong)_UNK_00112b68 >> 0x20),
                                                      CONCAT13(bVar18 ^ (byte)((ulong)_UNK_00112b68
                                                                              >> 0x18),
                                                               CONCAT12(bVar17 ^ (byte)((ulong)
                                                  _UNK_00112b68 >> 0x10),
                                                  CONCAT11(bVar16 ^ (byte)((ulong)_UNK_00112b68 >> 8
                                                                          ),
                                                           (byte)uVar15 ^ (byte)_UNK_00112b68)))))))
      ;
      local_c0 = CONCAT17(bVar14 ^ (byte)((ulong)_DAT_00112b60 >> 0x38),
                          CONCAT16(bVar13 ^ (byte)((ulong)_DAT_00112b60 >> 0x30),
                                   CONCAT15(bVar12 ^ (byte)((ulong)_DAT_00112b60 >> 0x28),
                                            CONCAT14(bVar11 ^ (byte)((ulong)_DAT_00112b60 >> 0x20),
                                                     CONCAT13(bVar10 ^ (byte)((ulong)_DAT_00112b60
                                                                             >> 0x18),
                                                              CONCAT12(bVar9 ^ (byte)((ulong)
                                                  _DAT_00112b60 >> 0x10),
                                                  CONCAT11(bVar8 ^ (byte)((ulong)_DAT_00112b60 >> 8)
                                                           ,(byte)uVar6 ^ (byte)_DAT_00112b60)))))))
      ;
      local_a8 = 0x5c5c5c5c5c5c5c5c;
      local_b0 = 0x5c5c5c5c5c5c5c5c;
      local_cc = *(undefined4 *)(param_2 + 4);
      local_c7 = (undefined1)((uint)iVar2 >> 8);
      local_c6 = (undefined1)((uint)iVar2 >> 0x10);
      local_c5 = (undefined1)((uint)iVar2 >> 0x18);
      FUN_001a1c68(local_168);
      FUN_001a1c8c(local_168,&local_80,0x40);
      FUN_001a1c8c(local_168,&local_d8,0x14);
      FUN_001a1fac(local_168,&local_f8);
      FUN_001a1c68(local_168);
      FUN_001a1c8c(local_168,&local_c0,0x40);
      FUN_001a1c8c(local_168,&local_f8,0x20);
      FUN_001a1fac(local_168,&local_f8);
      lVar7 = 0;
      uVar1 = (uint)local_f8 | (local_f7 & 0x7f) << 8;
      if (uVar1 == 0) {
        uVar1 = 1;
      }
      *param_3 = uVar1;
      local_80 = 0;
      uStack_78 = 0;
      local_70 = 0;
      local_68 = 0;
      local_60 = 0;
      uStack_58 = 0;
      local_50 = 0;
      local_48 = 0;
      local_c0 = 0;
      uStack_b8 = 0;
      local_b0 = 0;
      local_a8 = 0;
      local_a0 = 0;
      uStack_98 = 0;
      local_90 = 0;
      local_88 = 0;
      local_d8 = 0;
      local_d0 = 0;
      local_cc = 0;
      local_c8 = 0;
      local_c7 = 0;
      local_c6 = 0;
      local_c5 = 0;
      local_f8 = 0;
      local_f7 = 0;
      local_f6 = 0;
      local_f5 = 0;
      local_f4 = 0;
      local_f3 = 0;
      local_f2 = 0;
      local_f1 = 0;
      local_f0 = 0;
      local_ef = 0;
      local_ee = 0;
      local_ed = 0;
      local_ec = 0;
      local_eb = 0;
      local_ea = 0;
      local_e9 = 0;
      local_e8 = 0;
      local_e7 = 0;
      local_e6 = 0;
      local_e5 = 0;
      local_e4 = 0;
      local_e3 = 0;
      local_e2 = 0;
      local_e1 = 0;
      local_e0 = 0;
      local_df = 0;
      local_de = 0;
      local_dd = 0;
      local_dc = 0;
      local_db = 0;
      local_da = 0;
      local_d9 = 0;
      do {
        local_168[lVar7] = 0;
        lVar7 = lVar7 + 1;
      } while (lVar7 != 0x70);
      uVar6 = 1;
    }
  }
  if (*(long *)(lVar5 + 0x28) == local_38) {
    return uVar6;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0019c6e0 @ 0019c6e0 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void * FUN_0019c6e0(long *param_1,ulong param_2)

{
  long *plVar1;
  int iVar2;
  uint uVar3;
  long lVar4;
  byte *__s2;
  bool bVar5;
  uint uVar6;
  int iVar7;
  size_t sVar8;
  void *pvVar9;
  long lVar10;
  ulong uVar11;
  ulong uVar12;
  undefined8 *puVar13;
  ulong uVar14;
  int *piVar15;
  int *piVar16;
  byte *pbVar17;
  byte *local_78;
  pthread_t local_70;
  long local_68;
  
  lVar4 = tpidr_el0;
  local_68 = *(long *)(lVar4 + 0x28);
  if ((((param_1 == (long *)0x0) || (*param_1 == 0)) ||
      (uVar6 = *(uint *)(param_1 + 1), uVar6 - 0xb < 0xfffffff6)) ||
     (0x3f < *(uint *)((long)param_1 + 0xc))) {
    pthread_mutex_lock((pthread_mutex_t *)&DAT_0031ae50);
LAB_0019c744:
    if (DAT_0031ae78 != 0) {
      DAT_0031b018 = DAT_0031b018 + 1;
      _DAT_0031ae80 = 0;
      DAT_0031ae78 = 0;
      _DAT_0031ae90 = 0;
      _DAT_0031ae88 = 0;
      uRam000000000031aea0 = 0;
      _DAT_0031ae98 = 0;
      _DAT_0031aeb0 = 0;
      DAT_0031aea8 = 0;
      uRam000000000031aec0 = 0;
      _DAT_0031aeb8 = 0;
      DAT_0031aed0 = 0;
      _DAT_0031aec8 = 0;
      uRam000000000031aee0 = 0;
      _DAT_0031aed8 = 0;
      uRam000000000031aef0 = 0;
      _DAT_0031aee8 = 0;
      uRam000000000031af00 = 0;
      DAT_0031aef8 = 0;
      uRam000000000031af10 = 0;
      _DAT_0031af08 = 0;
      DAT_0031af20 = 0;
      _DAT_0031af18 = 0;
      uRam000000000031af30 = 0;
      _DAT_0031af28 = 0;
      uRam000000000031af40 = 0;
      _DAT_0031af38 = 0;
      uRam000000000031af50 = 0;
      _DAT_0031af48 = 0;
      uRam000000000031af60 = 0;
      _DAT_0031af58 = 0;
      uRam000000000031af70 = 0;
      _DAT_0031af68 = 0;
      uRam000000000031af80 = 0;
      _DAT_0031af78 = 0;
      uRam000000000031af90 = 0;
      _DAT_0031af88 = 0;
      uRam000000000031afa0 = 0;
      _DAT_0031af98 = 0;
      uRam000000000031afb0 = 0;
      _DAT_0031afa8 = 0;
      uRam000000000031afc0 = 0;
      _DAT_0031afb8 = 0;
      uRam000000000031afd0 = 0;
      _DAT_0031afc8 = 0;
      uRam000000000031afe0 = 0;
      _DAT_0031afd8 = 0;
      uRam000000000031aff0 = 0;
      _DAT_0031afe8 = 0;
      uRam000000000031b000 = 0;
      _DAT_0031aff8 = 0;
      uRam000000000031b010 = 0;
      _DAT_0031b008 = 0;
    }
    DAT_0031b020 = 0;
    DAT_0031b028 = 0;
  }
  else {
    uVar14 = 0;
    bVar5 = false;
    plVar1 = param_1 + 2;
    local_78 = (byte *)(param_1 + 3);
    while( true ) {
      sVar8 = strnlen((char *)(plVar1 + uVar14 * 5 + 1),0x18);
      if (((sVar8 == 0 || sVar8 == 0x18) || (uVar3 = *(uint *)(plVar1 + uVar14 * 5), 0x3f < uVar3))
         || (pbVar17 = local_78,
            *(int *)((long)plVar1 + uVar14 * 0x28 + 4) + 0xfeed5780U < 0xffe17b80)) break;
      do {
        pvVar9 = memchr("0289PYLQGRJCUV",(uint)*pbVar17,0xf);
        if (pvVar9 == (void *)0x0) goto LAB_0019c8ec;
        sVar8 = sVar8 - 1;
        pbVar17 = pbVar17 + 1;
        __s2 = (byte *)(param_1 + 3);
        uVar12 = uVar14;
      } while (sVar8 != 0);
      for (; uVar12 != 0; uVar12 = uVar12 - 1) {
        if ((uVar3 == *(uint *)(__s2 + -8)) ||
           (iVar7 = strcmp((char *)(plVar1 + uVar14 * 5 + 1),(char *)__s2), iVar7 == 0))
        goto LAB_0019c8ec;
        __s2 = __s2 + 0x28;
      }
      uVar14 = uVar14 + 1;
      bVar5 = uVar6 <= uVar14;
      local_78 = local_78 + 0x28;
      if (uVar14 == uVar6) break;
    }
LAB_0019c8ec:
    pthread_mutex_lock((pthread_mutex_t *)&DAT_0031ae50);
    if (!bVar5) goto LAB_0019c744;
    if (*param_1 == DAT_0031ae78) {
      uVar6 = *(uint *)(param_1 + 1);
      if ((uVar6 != DAT_0031ae80) || (*(int *)((long)param_1 + 0xc) != DAT_0031ae84))
      goto LAB_0019c9d4;
      if (uVar6 != 0) {
        if (((int)*plVar1 == DAT_0031ae88) && (*(int *)((long)param_1 + 0x14) == DAT_0031ae8c)) {
          bVar5 = false;
          uVar14 = 1;
          piVar15 = &DAT_0031aeb4;
          piVar16 = (int *)((long)param_1 + 0x3c);
          do {
            iVar7 = strcmp((char *)(piVar16 + -9),(char *)(piVar15 + -9));
            if (((iVar7 != 0) || (bVar5 = uVar6 <= uVar14, uVar6 == uVar14)) ||
               (piVar16[-1] != piVar15[-1])) break;
            iVar7 = *piVar16;
            iVar2 = *piVar15;
            uVar14 = uVar14 + 1;
            piVar15 = piVar15 + 10;
            piVar16 = piVar16 + 10;
          } while (iVar7 == iVar2);
          if (bVar5) goto LAB_0019ca64;
        }
        goto LAB_0019c9d4;
      }
    }
    else {
LAB_0019c9d4:
      memcpy(&DAT_0031ae78,param_1,0x1a0);
      DAT_0031b020 = 0;
      DAT_0031b028 = 0;
      DAT_0031b018 = DAT_0031b018 + 1;
      uVar14 = _DAT_0031ae80 & 0xffffffff;
      if (DAT_0031ae80 != 0) {
        if (DAT_0031ae80 == 1) {
          uVar11 = 0;
        }
        else {
          uVar11 = _DAT_0031ae80 & 0xfffffffe;
          uVar12 = uVar11;
          puVar13 = &DAT_0031aed0;
          do {
            uVar12 = uVar12 - 2;
            puVar13[-5] = 0xffffffffffffffff;
            *puVar13 = 0xffffffffffffffff;
            puVar13 = puVar13 + 10;
          } while (uVar12 != 0);
          if (uVar11 == uVar14) goto LAB_0019ca64;
        }
        lVar10 = uVar14 - uVar11;
        puVar13 = &DAT_0031aea8 + uVar11 * 5;
        do {
          lVar10 = lVar10 + -1;
          *puVar13 = 0xffffffffffffffff;
          puVar13 = puVar13 + 5;
        } while (lVar10 != 0);
      }
    }
LAB_0019ca64:
    if ((((param_2 != 0) && ((DAT_0031b030 & 1) == 0)) &&
        ((DAT_0031b020 == 0 &&
         ((param_2 <= DAT_0031b028 - 1 || (0x270 < param_2 - DAT_0031b028 >> 4)))))) &&
       (pvVar9 = malloc(0x1a8), pvVar9 != (void *)0x0)) {
      memcpy(pvVar9,&DAT_0031ae78,0x1a0);
      DAT_0031b028 = param_2;
      *(long *)((long)pvVar9 + 0x1a0) = DAT_0031b018;
      DAT_0031b030 = 1;
      pthread_mutex_unlock((pthread_mutex_t *)&DAT_0031ae50);
      iVar7 = pthread_create(&local_70,(pthread_attr_t *)0x0,FUN_0019cb5c,pvVar9);
      if (iVar7 == 0) {
        uVar6 = pthread_detach(local_70);
        pvVar9 = (void *)(ulong)uVar6;
      }
      else {
        pthread_mutex_lock((pthread_mutex_t *)&DAT_0031ae50);
        DAT_0031b030 = 0;
        pthread_mutex_unlock((pthread_mutex_t *)&DAT_0031ae50);
        free(pvVar9);
      }
      if (*(long *)(lVar4 + 0x28) == local_68) {
        return pvVar9;
      }
      goto LAB_0019cb58;
    }
  }
  if (*(long *)(lVar4 + 0x28) == local_68) {
    uVar6 = pthread_mutex_unlock((pthread_mutex_t *)&DAT_0031ae50);
    return (void *)(ulong)uVar6;
  }
LAB_0019cb58:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0019d76c @ 0019d76c [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0019d76c(long param_1,ulong param_2)

{
  long lVar1;
  undefined1 auVar2 [16];
  uint3 uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  undefined8 uVar9;
  undefined8 uVar10;
  undefined8 uVar11;
  undefined8 uVar12;
  undefined8 uVar13;
  undefined8 uVar14;
  undefined8 uVar15;
  int iVar16;
  ulong uVar17;
  ulong uVar18;
  ulong uVar19;
  ulong uVar20;
  byte *pbVar21;
  undefined1 (*pauVar22) [16];
  long lVar23;
  undefined8 *puVar24;
  byte *pbVar25;
  int iVar26;
  int iVar27;
  int iVar28;
  int iVar29;
  int iVar30;
  byte bVar31;
  int iVar47;
  undefined8 uVar32;
  byte bVar43;
  byte bVar44;
  byte bVar45;
  byte bVar46;
  byte bVar48;
  byte bVar49;
  byte bVar50;
  byte bVar51;
  byte bVar52;
  byte bVar53;
  byte bVar54;
  undefined1 auVar33 [12];
  undefined1 auVar34 [12];
  undefined1 auVar36 [12];
  byte bVar55;
  byte bVar56;
  byte bVar57;
  byte bVar58;
  undefined1 auVar39 [16];
  undefined1 auVar35 [12];
  undefined1 auVar41 [16];
  undefined1 auVar42 [16];
  uint uVar59;
  int iVar60;
  undefined8 uVar63;
  byte bVar88;
  uint uVar83;
  int iVar84;
  int iVar90;
  undefined1 auVar65 [12];
  int iVar91;
  undefined1 auVar72 [16];
  int iVar61;
  int iVar62;
  undefined1 auVar73 [16];
  undefined1 auVar66 [12];
  undefined1 auVar67 [12];
  int iVar85;
  undefined1 auVar68 [12];
  undefined1 auVar69 [12];
  undefined1 auVar78 [16];
  int iVar92;
  int iVar96;
  int iVar97;
  int iVar98;
  undefined1 auVar93 [16];
  undefined1 auVar94 [16];
  undefined1 auVar95 [16];
  byte bVar99;
  byte bVar100;
  byte bVar101;
  byte bVar102;
  byte bVar103;
  byte bVar104;
  byte bVar105;
  byte bVar106;
  byte bVar107;
  byte bVar108;
  byte bVar109;
  byte bVar110;
  int iVar111;
  int iVar112;
  int iVar113;
  int iVar114;
  undefined1 auVar115 [16];
  int iVar117;
  undefined1 auVar116 [16];
  int iVar118;
  byte local_58 [19];
  undefined1 uStack_45;
  undefined1 uStack_44;
  undefined1 uStack_43;
  undefined1 uStack_42;
  undefined1 uStack_41;
  undefined1 uStack_40;
  undefined1 uStack_3f;
  undefined1 uStack_3e;
  undefined1 uStack_3d;
  undefined1 uStack_3c;
  undefined1 uStack_3b;
  undefined1 uStack_3a;
  undefined1 uStack_39;
  undefined1 uStack_38;
  undefined1 uStack_37;
  undefined1 uStack_36;
  undefined1 uStack_35;
  undefined1 uStack_34;
  undefined1 uStack_33;
  undefined1 uStack_32;
  undefined1 uStack_31;
  undefined1 uStack_30;
  undefined1 uStack_2f;
  undefined1 uStack_2e;
  undefined1 uStack_2d;
  undefined1 uStack_2c;
  undefined1 uStack_2b;
  undefined1 uStack_2a;
  undefined1 uStack_29;
  undefined1 uStack_28;
  undefined1 uStack_27;
  undefined1 uStack_26;
  undefined1 uStack_25;
  undefined1 uStack_24;
  undefined1 uStack_23;
  undefined1 uStack_22;
  undefined1 uStack_21;
  undefined1 uStack_20;
  undefined1 uStack_1f;
  undefined1 uStack_1e;
  undefined1 uStack_1d;
  undefined1 uStack_1c;
  undefined1 uStack_1b;
  undefined1 uStack_1a;
  undefined1 uStack_19;
  long local_18;
  undefined1 auVar37 [16];
  undefined1 auVar38 [16];
  undefined1 auVar40 [16];
  undefined1 auVar64 [12];
  undefined1 auVar70 [16];
  undefined1 auVar71 [16];
  undefined1 auVar74 [16];
  undefined1 auVar75 [16];
  undefined1 auVar76 [16];
  undefined1 auVar77 [16];
  byte bVar79;
  byte bVar80;
  byte bVar81;
  byte bVar82;
  byte bVar86;
  byte bVar87;
  byte bVar89;
  
  uVar15 = _UNK_00112c08;
  uVar14 = _DAT_00112c00;
  uVar13 = _UNK_00112bf8;
  uVar12 = _DAT_00112bf0;
  uVar11 = _UNK_00112b88;
  uVar10 = s_expand_32_byte_kff__fff__00112ab0._8_8_;
  uVar9 = s_expand_32_byte_kff__fff__00112ab0._0_8_;
  lVar1 = tpidr_el0;
  local_18 = *(long *)(lVar1 + 0x28);
  if (param_2 != 0) {
    iVar16 = 0;
    uVar17 = 0;
    uVar32 = _DAT_00112b80;
    do {
      iVar30 = 10;
      auVar39._8_8_ = uVar11;
      auVar39._0_8_ = uVar32;
      auVar94._8_8_ = uVar10;
      auVar94._0_8_ = uVar9;
      auVar73._8_8_ = uVar15;
      auVar73._0_8_ = uVar14;
      auVar115._8_8_ = uVar13;
      auVar115._0_8_ = uVar12;
      do {
        iVar92 = auVar94._0_4_ + auVar73._0_4_;
        iVar96 = auVar94._4_4_ + auVar73._4_4_;
        iVar97 = auVar94._8_4_ + auVar73._8_4_;
        iVar98 = auVar94._12_4_ + auVar73._12_4_;
        iVar30 = iVar30 + -1;
        bVar31 = auVar39[0] ^ (byte)iVar92;
        bVar43 = auVar39[1] ^ (byte)((uint)iVar92 >> 8);
        bVar44 = auVar39[2] ^ (byte)((uint)iVar92 >> 0x10);
        bVar45 = auVar39[3] ^ (byte)((uint)iVar92 >> 0x18);
        bVar46 = auVar39[4] ^ (byte)iVar96;
        bVar48 = auVar39[5] ^ (byte)((uint)iVar96 >> 8);
        bVar49 = auVar39[6] ^ (byte)((uint)iVar96 >> 0x10);
        bVar50 = auVar39[7] ^ (byte)((uint)iVar96 >> 0x18);
        bVar51 = auVar39[8] ^ (byte)iVar97;
        bVar52 = auVar39[9] ^ (byte)((uint)iVar97 >> 8);
        bVar53 = auVar39[10] ^ (byte)((uint)iVar97 >> 0x10);
        bVar54 = auVar39[0xb] ^ (byte)((uint)iVar97 >> 0x18);
        bVar55 = auVar39[0xc] ^ (byte)iVar98;
        bVar56 = auVar39[0xd] ^ (byte)((uint)iVar98 >> 8);
        bVar57 = auVar39[0xe] ^ (byte)((uint)iVar98 >> 0x10);
        bVar58 = auVar39[0xf] ^ (byte)((uint)iVar98 >> 0x18);
        iVar61 = CONCAT13(bVar43,CONCAT12(bVar31,CONCAT11(bVar45,bVar44)));
        uVar63 = CONCAT17(bVar48,CONCAT16(bVar46,CONCAT15(bVar50,CONCAT14(bVar49,iVar61))));
        auVar33[8] = bVar53;
        auVar33._0_8_ = uVar63;
        auVar33[9] = bVar54;
        auVar33[10] = bVar51;
        auVar33[0xb] = bVar52;
        auVar37[0xc] = bVar57;
        auVar37._0_12_ = auVar33;
        auVar37[0xd] = bVar58;
        auVar37[0xe] = bVar55;
        auVar37[0xf] = bVar56;
        iVar61 = auVar115._0_4_ + iVar61;
        iVar4 = auVar115._4_4_ + (int)((ulong)uVar63 >> 0x20);
        iVar62 = auVar115._8_4_ + auVar33._8_4_;
        iVar85 = auVar115._12_4_ + auVar37._12_4_;
        bVar80 = (byte)((uint)iVar61 >> 0x18) ^ auVar73[3];
        uVar59 = CONCAT13(bVar80,CONCAT12((byte)((uint)iVar61 >> 0x10) ^ auVar73[2],
                                          CONCAT11((byte)((uint)iVar61 >> 8) ^ auVar73[1],
                                                   (byte)iVar61 ^ auVar73[0])));
        bVar88 = (byte)((uint)iVar4 >> 0x18) ^ auVar73[7];
        auVar64._0_8_ =
             CONCAT17(bVar88,CONCAT16((byte)((uint)iVar4 >> 0x10) ^ auVar73[6],
                                      CONCAT15((byte)((uint)iVar4 >> 8) ^ auVar73[5],
                                               CONCAT14((byte)iVar4 ^ auVar73[4],uVar59))));
        auVar64[8] = (byte)iVar62 ^ auVar73[8];
        auVar64[9] = (byte)((uint)iVar62 >> 8) ^ auVar73[9];
        auVar64[10] = (byte)((uint)iVar62 >> 0x10) ^ auVar73[10];
        auVar64[0xb] = (byte)((uint)iVar62 >> 0x18) ^ auVar73[0xb];
        auVar70[0xc] = (byte)iVar85 ^ auVar73[0xc];
        auVar70._0_12_ = auVar64;
        auVar70[0xd] = (byte)((uint)iVar85 >> 8) ^ auVar73[0xd];
        auVar70[0xe] = (byte)((uint)iVar85 >> 0x10) ^ auVar73[0xe];
        auVar70[0xf] = (byte)((uint)iVar85 >> 0x18) ^ auVar73[0xf];
        uVar83 = (uint)((ulong)auVar64._0_8_ >> 0x20);
        iVar60 = uVar59 << 0xc;
        iVar84 = uVar83 << 0xc;
        iVar90 = auVar64._8_4_ << 0xc;
        iVar91 = auVar70._12_4_ << 0xc;
        bVar87 = (byte)(uVar59 >> 0x14);
        bVar80 = (byte)((uint)iVar60 >> 8) | bVar80 >> 4;
        bVar79 = (byte)((uint)iVar60 >> 0x10);
        bVar81 = (byte)((uint)iVar60 >> 0x18);
        iVar60 = CONCAT13(bVar81,CONCAT12(bVar79,CONCAT11(bVar80,bVar87)));
        bVar89 = (byte)(uVar83 >> 0x14);
        bVar88 = (byte)((uint)iVar84 >> 8) | bVar88 >> 4;
        bVar82 = (byte)((uint)iVar84 >> 0x10);
        bVar86 = (byte)((uint)iVar84 >> 0x18);
        uVar63 = CONCAT17(bVar86,CONCAT16(bVar82,CONCAT15(bVar88,CONCAT14(bVar89,iVar60))));
        bVar99 = (byte)(auVar64._8_4_ >> 0x14);
        auVar65[8] = bVar99;
        auVar65._0_8_ = uVar63;
        auVar65[9] = (byte)((uint)iVar90 >> 8) | auVar64[0xb] >> 4;
        auVar65[10] = (byte)((uint)iVar90 >> 0x10);
        auVar65[0xb] = (byte)((uint)iVar90 >> 0x18);
        bVar100 = (byte)(auVar70._12_4_ >> 0x14);
        auVar71[0xc] = bVar100;
        auVar71._0_12_ = auVar65;
        auVar71[0xd] = (byte)((uint)iVar91 >> 8) | auVar70[0xf] >> 4;
        auVar71[0xe] = (byte)((uint)iVar91 >> 0x10);
        auVar71[0xf] = (byte)((uint)iVar91 >> 0x18);
        auVar93._0_4_ = iVar60 + iVar92;
        auVar93._4_4_ = (int)((ulong)uVar63 >> 0x20) + iVar96;
        auVar93._8_4_ = auVar65._8_4_ + iVar97;
        auVar93._12_4_ = auVar71._12_4_ + iVar98;
        auVar94 = NEON_ext(auVar93,auVar93,8,1);
        iVar60 = CONCAT13((byte)((uint)auVar93._0_4_ >> 0x10) ^ bVar31,
                          CONCAT12((byte)((uint)auVar93._0_4_ >> 8) ^ bVar45,
                                   CONCAT11((byte)auVar93._0_4_ ^ bVar44,
                                            (byte)((uint)auVar93._0_4_ >> 0x18) ^ bVar43)));
        uVar63 = CONCAT17((byte)((uint)auVar93._4_4_ >> 0x10) ^ bVar46,
                          CONCAT16((byte)((uint)auVar93._4_4_ >> 8) ^ bVar50,
                                   CONCAT15((byte)auVar93._4_4_ ^ bVar49,
                                            CONCAT14((byte)((uint)auVar93._4_4_ >> 0x18) ^ bVar48,
                                                     iVar60))));
        auVar34[8] = (byte)((uint)auVar93._8_4_ >> 0x18) ^ bVar52;
        auVar34._0_8_ = uVar63;
        auVar34[9] = (byte)auVar93._8_4_ ^ bVar53;
        auVar34[10] = (byte)((uint)auVar93._8_4_ >> 8) ^ bVar54;
        auVar34[0xb] = (byte)((uint)auVar93._8_4_ >> 0x10) ^ bVar51;
        auVar38[0xc] = (byte)((uint)auVar93._12_4_ >> 0x18) ^ bVar56;
        auVar38._0_12_ = auVar34;
        auVar38[0xd] = (byte)auVar93._12_4_ ^ bVar57;
        auVar38[0xe] = (byte)((uint)auVar93._12_4_ >> 8) ^ bVar58;
        auVar38[0xf] = (byte)((uint)auVar93._12_4_ >> 0x10) ^ bVar55;
        iVar60 = iVar60 + iVar61;
        iVar4 = (int)((ulong)uVar63 >> 0x20) + iVar4;
        iVar62 = auVar34._8_4_ + iVar62;
        iVar85 = auVar38._12_4_ + iVar85;
        auVar39 = NEON_ext(auVar38,auVar38,4,1);
        auVar72[0] = (byte)iVar60 ^ bVar87;
        auVar72[1] = (byte)((uint)iVar60 >> 8) ^ bVar80;
        auVar72[2] = (byte)((uint)iVar60 >> 0x10) ^ bVar79;
        auVar72[3] = (byte)((uint)iVar60 >> 0x18) ^ bVar81;
        auVar72[4] = (byte)iVar4 ^ bVar89;
        auVar72[5] = (byte)((uint)iVar4 >> 8) ^ bVar88;
        auVar72[6] = (byte)((uint)iVar4 >> 0x10) ^ bVar82;
        auVar72[7] = (byte)((uint)iVar4 >> 0x18) ^ bVar86;
        auVar72[8] = (byte)iVar62 ^ bVar99;
        auVar72[9] = (byte)((uint)iVar62 >> 8) ^ auVar65[9];
        auVar72[10] = (byte)((uint)iVar62 >> 0x10) ^ auVar65[10];
        auVar72[0xb] = (byte)((uint)iVar62 >> 0x18) ^ auVar65[0xb];
        auVar72[0xc] = (byte)iVar85 ^ bVar100;
        auVar72[0xd] = (byte)((uint)iVar85 >> 8) ^ auVar71[0xd];
        auVar72[0xe] = (byte)((uint)iVar85 >> 0x10) ^ auVar71[0xe];
        auVar72[0xf] = (byte)((uint)iVar85 >> 0x18) ^ auVar71[0xf];
        auVar73 = NEON_ext(auVar72,auVar72,0xc,1);
        iVar61 = auVar73._0_4_ << 7;
        iVar84 = auVar73._4_4_ << 7;
        iVar90 = auVar73._8_4_ << 7;
        iVar91 = auVar73._12_4_ << 7;
        bVar80 = (byte)iVar61 | auVar73[3] >> 1;
        bVar79 = (byte)((uint)iVar61 >> 8);
        bVar81 = (byte)((uint)iVar61 >> 0x10);
        bVar88 = (byte)((uint)iVar61 >> 0x18);
        iVar61 = CONCAT13(bVar88,CONCAT12(bVar81,CONCAT11(bVar79,bVar80)));
        bVar82 = (byte)iVar84 | auVar73[7] >> 1;
        bVar86 = (byte)((uint)iVar84 >> 8);
        bVar87 = (byte)((uint)iVar84 >> 0x10);
        bVar89 = (byte)((uint)iVar84 >> 0x18);
        uVar63 = CONCAT17(bVar89,CONCAT16(bVar87,CONCAT15(bVar86,CONCAT14(bVar82,iVar61))));
        auVar66[8] = (byte)iVar90 | auVar73[0xb] >> 1;
        auVar66._0_8_ = uVar63;
        auVar66[9] = (byte)((uint)iVar90 >> 8);
        auVar66[10] = (byte)((uint)iVar90 >> 0x10);
        auVar66[0xb] = (byte)((uint)iVar90 >> 0x18);
        auVar74[0xc] = (byte)iVar91 | auVar73[0xf] >> 1;
        auVar74._0_12_ = auVar66;
        auVar74[0xd] = (byte)((uint)iVar91 >> 8);
        auVar74[0xe] = (byte)((uint)iVar91 >> 0x10);
        auVar74[0xf] = (byte)((uint)iVar91 >> 0x18);
        iVar61 = auVar94._0_4_ + iVar61;
        iVar91 = auVar94._4_4_ + (int)((ulong)uVar63 >> 0x20);
        iVar92 = auVar94._8_4_ + auVar66._8_4_;
        iVar96 = auVar94._12_4_ + auVar74._12_4_;
        bVar31 = auVar39[0] ^ (byte)iVar61;
        bVar43 = auVar39[1] ^ (byte)((uint)iVar61 >> 8);
        bVar44 = auVar39[2] ^ (byte)((uint)iVar61 >> 0x10);
        bVar45 = auVar39[3] ^ (byte)((uint)iVar61 >> 0x18);
        bVar46 = auVar39[4] ^ (byte)iVar91;
        bVar48 = auVar39[5] ^ (byte)((uint)iVar91 >> 8);
        bVar49 = auVar39[6] ^ (byte)((uint)iVar91 >> 0x10);
        bVar50 = auVar39[7] ^ (byte)((uint)iVar91 >> 0x18);
        bVar51 = auVar39[8] ^ (byte)iVar92;
        bVar52 = auVar39[9] ^ (byte)((uint)iVar92 >> 8);
        bVar53 = auVar39[10] ^ (byte)((uint)iVar92 >> 0x10);
        bVar54 = auVar39[0xb] ^ (byte)((uint)iVar92 >> 0x18);
        bVar55 = auVar39[0xc] ^ (byte)iVar96;
        bVar56 = auVar39[0xd] ^ (byte)((uint)iVar96 >> 8);
        bVar57 = auVar39[0xe] ^ (byte)((uint)iVar96 >> 0x10);
        bVar58 = auVar39[0xf] ^ (byte)((uint)iVar96 >> 0x18);
        iVar84 = CONCAT13(bVar43,CONCAT12(bVar31,CONCAT11(bVar45,bVar44)));
        uVar63 = CONCAT17(bVar48,CONCAT16(bVar46,CONCAT15(bVar50,CONCAT14(bVar49,iVar84))));
        auVar35[8] = bVar53;
        auVar35._0_8_ = uVar63;
        auVar35[9] = bVar54;
        auVar35[10] = bVar51;
        auVar35[0xb] = bVar52;
        auVar40[0xc] = bVar57;
        auVar40._0_12_ = auVar35;
        auVar40[0xd] = bVar58;
        auVar40[0xe] = bVar55;
        auVar40[0xf] = bVar56;
        iVar60 = iVar60 + iVar84;
        iVar4 = iVar4 + (int)((ulong)uVar63 >> 0x20);
        bVar99 = (byte)iVar4;
        bVar100 = (byte)((uint)iVar4 >> 8);
        bVar101 = (byte)((uint)iVar4 >> 0x10);
        bVar102 = (byte)((uint)iVar4 >> 0x18);
        iVar62 = iVar62 + auVar35._8_4_;
        bVar103 = (byte)iVar62;
        bVar104 = (byte)((uint)iVar62 >> 8);
        bVar105 = (byte)((uint)iVar62 >> 0x10);
        bVar106 = (byte)((uint)iVar62 >> 0x18);
        iVar85 = iVar85 + auVar40._12_4_;
        bVar107 = (byte)iVar85;
        bVar108 = (byte)((uint)iVar85 >> 8);
        bVar109 = (byte)((uint)iVar85 >> 0x10);
        bVar110 = (byte)((uint)iVar85 >> 0x18);
        bVar88 = (byte)((uint)iVar60 >> 0x18) ^ bVar88;
        uVar59 = CONCAT13(bVar88,CONCAT12((byte)((uint)iVar60 >> 0x10) ^ bVar81,
                                          CONCAT11((byte)((uint)iVar60 >> 8) ^ bVar79,
                                                   (byte)iVar60 ^ bVar80)));
        bVar89 = bVar102 ^ bVar89;
        auVar67._0_8_ =
             CONCAT17(bVar89,CONCAT16(bVar101 ^ bVar87,
                                      CONCAT15(bVar100 ^ bVar86,CONCAT14(bVar99 ^ bVar82,uVar59))));
        auVar67[8] = bVar103 ^ auVar66[8];
        auVar67[9] = bVar104 ^ auVar66[9];
        auVar67[10] = bVar105 ^ auVar66[10];
        auVar67[0xb] = bVar106 ^ auVar66[0xb];
        auVar75[0xc] = bVar107 ^ auVar74[0xc];
        auVar75._0_12_ = auVar67;
        auVar75[0xd] = bVar108 ^ auVar74[0xd];
        auVar75[0xe] = bVar109 ^ auVar74[0xe];
        auVar75[0xf] = bVar110 ^ auVar74[0xf];
        uVar83 = (uint)((ulong)auVar67._0_8_ >> 0x20);
        iVar62 = uVar59 << 0xc;
        iVar85 = uVar83 << 0xc;
        iVar84 = auVar67._8_4_ << 0xc;
        iVar90 = auVar75._12_4_ << 0xc;
        auVar116[4] = bVar99;
        auVar116._0_4_ = iVar60;
        auVar116[5] = bVar100;
        auVar116[6] = bVar101;
        auVar116[7] = bVar102;
        auVar116[8] = bVar103;
        auVar116[9] = bVar104;
        auVar116[10] = bVar105;
        auVar116[0xb] = bVar106;
        auVar116[0xc] = bVar107;
        auVar116[0xd] = bVar108;
        auVar116[0xe] = bVar109;
        auVar116[0xf] = bVar110;
        auVar2[4] = bVar99;
        auVar2._0_4_ = iVar60;
        auVar2[5] = bVar100;
        auVar2[6] = bVar101;
        auVar2[7] = bVar102;
        auVar2[8] = bVar103;
        auVar2[9] = bVar104;
        auVar2[10] = bVar105;
        auVar2[0xb] = bVar106;
        auVar2[0xc] = bVar107;
        auVar2[0xd] = bVar108;
        auVar2[0xe] = bVar109;
        auVar2[0xf] = bVar110;
        auVar115 = NEON_ext(auVar116,auVar2,8,1);
        bVar87 = (byte)(uVar59 >> 0x14);
        bVar80 = (byte)((uint)iVar62 >> 8) | bVar88 >> 4;
        bVar79 = (byte)((uint)iVar62 >> 0x10);
        bVar81 = (byte)((uint)iVar62 >> 0x18);
        iVar62 = CONCAT13(bVar81,CONCAT12(bVar79,CONCAT11(bVar80,bVar87)));
        bVar99 = (byte)(uVar83 >> 0x14);
        bVar88 = (byte)((uint)iVar85 >> 8) | bVar89 >> 4;
        bVar82 = (byte)((uint)iVar85 >> 0x10);
        bVar86 = (byte)((uint)iVar85 >> 0x18);
        uVar63 = CONCAT17(bVar86,CONCAT16(bVar82,CONCAT15(bVar88,CONCAT14(bVar99,iVar62))));
        bVar89 = (byte)(auVar67._8_4_ >> 0x14);
        auVar68[8] = bVar89;
        auVar68._0_8_ = uVar63;
        auVar68[9] = (byte)((uint)iVar84 >> 8) | auVar67[0xb] >> 4;
        auVar68[10] = (byte)((uint)iVar84 >> 0x10);
        auVar68[0xb] = (byte)((uint)iVar84 >> 0x18);
        bVar100 = (byte)(auVar75._12_4_ >> 0x14);
        auVar76[0xc] = bVar100;
        auVar76._0_12_ = auVar68;
        auVar76[0xd] = (byte)((uint)iVar90 >> 8) | auVar75[0xf] >> 4;
        auVar76[0xe] = (byte)((uint)iVar90 >> 0x10);
        auVar76[0xf] = (byte)((uint)iVar90 >> 0x18);
        auVar95._0_4_ = iVar62 + iVar61;
        auVar95._4_4_ = (int)((ulong)uVar63 >> 0x20) + iVar91;
        auVar95._8_4_ = auVar68._8_4_ + iVar92;
        auVar95._12_4_ = auVar76._12_4_ + iVar96;
        auVar41[0] = (byte)auVar95._0_4_ ^ bVar44;
        auVar41[1] = (byte)((uint)auVar95._0_4_ >> 8) ^ bVar45;
        auVar41[2] = (byte)((uint)auVar95._0_4_ >> 0x10) ^ bVar31;
        auVar41[3] = (byte)((uint)auVar95._0_4_ >> 0x18) ^ bVar43;
        auVar41[4] = (byte)auVar95._4_4_ ^ bVar49;
        auVar41[5] = (byte)((uint)auVar95._4_4_ >> 8) ^ bVar50;
        auVar41[6] = (byte)((uint)auVar95._4_4_ >> 0x10) ^ bVar46;
        auVar41[7] = (byte)((uint)auVar95._4_4_ >> 0x18) ^ bVar48;
        auVar41[8] = (byte)auVar95._8_4_ ^ bVar53;
        auVar41[9] = (byte)((uint)auVar95._8_4_ >> 8) ^ bVar54;
        auVar41[10] = (byte)((uint)auVar95._8_4_ >> 0x10) ^ bVar51;
        auVar41[0xb] = (byte)((uint)auVar95._8_4_ >> 0x18) ^ bVar52;
        auVar41[0xc] = (byte)auVar95._12_4_ ^ bVar57;
        auVar41[0xd] = (byte)((uint)auVar95._12_4_ >> 8) ^ bVar58;
        auVar41[0xe] = (byte)((uint)auVar95._12_4_ >> 0x10) ^ bVar55;
        auVar41[0xf] = (byte)((uint)auVar95._12_4_ >> 0x18) ^ bVar56;
        auVar73 = NEON_ext(auVar41,auVar41,0xc,1);
        auVar94 = NEON_ext(auVar95,auVar95,8,1);
        iVar61 = CONCAT13(auVar73[2],CONCAT12(auVar73[1],CONCAT11(auVar73[0],auVar73[3])));
        uVar63 = CONCAT17(auVar73[6],
                          CONCAT16(auVar73[5],CONCAT15(auVar73[4],CONCAT14(auVar73[7],iVar61))));
        auVar36[8] = auVar73[0xb];
        auVar36._0_8_ = uVar63;
        auVar36[9] = auVar73[8];
        auVar36[10] = auVar73[9];
        auVar36[0xb] = auVar73[10];
        auVar39[0xc] = auVar73[0xf];
        auVar39._0_12_ = auVar36;
        auVar39[0xd] = auVar73[0xc];
        auVar39[0xe] = auVar73[0xd];
        auVar39[0xf] = auVar73[0xe];
        auVar73 = NEON_ext(auVar39,auVar39,4,1);
        auVar116 = NEON_ext(auVar39,auVar39,0xc,1);
        iVar60 = auVar73._0_4_ + iVar60;
        iVar4 = auVar73._4_4_ + iVar4;
        bVar43 = (byte)((uint)iVar4 >> 8);
        bVar44 = (byte)((uint)iVar4 >> 0x10);
        bVar31 = (byte)((uint)iVar4 >> 0x18);
        iVar91 = auVar116._0_4_ + auVar115._0_4_;
        iVar92 = auVar116._4_4_ + auVar115._4_4_;
        auVar115._12_4_ = iVar92;
        auVar115._8_4_ = iVar91;
        auVar115[4] = (byte)iVar4;
        auVar115._0_4_ = iVar60;
        auVar115[5] = bVar43;
        auVar115[6] = bVar44;
        auVar115[7] = bVar31;
        bVar81 = (byte)((uint)iVar60 >> 0x18) ^ bVar81;
        iVar62 = CONCAT13(bVar81,CONCAT12((byte)((uint)iVar60 >> 0x10) ^ bVar79,
                                          CONCAT11((byte)((uint)iVar60 >> 8) ^ bVar80,
                                                   (byte)iVar60 ^ bVar87)));
        bVar31 = bVar31 ^ bVar86;
        auVar69._0_8_ =
             CONCAT17(bVar31,CONCAT16(bVar44 ^ bVar82,
                                      CONCAT15(bVar43 ^ bVar88,CONCAT14((byte)iVar4 ^ bVar99,iVar62)
                                              )));
        auVar69[8] = (byte)iVar91 ^ bVar89;
        auVar69[9] = (byte)((uint)iVar91 >> 8) ^ auVar68[9];
        auVar69[10] = (byte)((uint)iVar91 >> 0x10) ^ auVar68[10];
        auVar69[0xb] = (byte)((uint)iVar91 >> 0x18) ^ auVar68[0xb];
        auVar77[0xc] = (byte)iVar92 ^ bVar100;
        auVar77._0_12_ = auVar69;
        auVar77[0xd] = (byte)((uint)iVar92 >> 8) ^ auVar76[0xd];
        auVar77[0xe] = (byte)((uint)iVar92 >> 0x10) ^ auVar76[0xe];
        auVar77[0xf] = (byte)((uint)iVar92 >> 0x18) ^ auVar76[0xf];
        iVar62 = iVar62 << 7;
        iVar85 = (int)((ulong)auVar69._0_8_ >> 0x20) << 7;
        iVar84 = auVar69._8_4_ << 7;
        iVar90 = auVar77._12_4_ << 7;
        auVar78[0] = (byte)iVar62 | bVar81 >> 1;
        auVar78[1] = (undefined1)((uint)iVar62 >> 8);
        auVar78[2] = (undefined1)((uint)iVar62 >> 0x10);
        auVar78[3] = (undefined1)((uint)iVar62 >> 0x18);
        auVar78[4] = (byte)iVar85 | bVar31 >> 1;
        auVar78[5] = (undefined1)((uint)iVar85 >> 8);
        auVar78[6] = (undefined1)((uint)iVar85 >> 0x10);
        auVar78[7] = (undefined1)((uint)iVar85 >> 0x18);
        auVar78[8] = (byte)iVar84 | auVar69[0xb] >> 1;
        auVar78[9] = (undefined1)((uint)iVar84 >> 8);
        auVar78[10] = (undefined1)((uint)iVar84 >> 0x10);
        auVar78[0xb] = (undefined1)((uint)iVar84 >> 0x18);
        auVar78[0xc] = (byte)iVar90 | auVar77[0xf] >> 1;
        auVar78[0xd] = (undefined1)((uint)iVar90 >> 8);
        auVar78[0xe] = (undefined1)((uint)iVar90 >> 0x10);
        auVar78[0xf] = (undefined1)((uint)iVar90 >> 0x18);
        auVar73 = NEON_ext(auVar78,auVar78,4,1);
      } while (iVar30 != 0);
      uVar18 = param_2 - uVar17;
      iVar85 = (int)uVar9 + auVar94._0_4_;
      iVar84 = SUB84(uVar9,4) + auVar94._4_4_;
      iVar90 = (int)uVar10 + auVar94._8_4_;
      iVar96 = SUB84(uVar10,4) + auVar94._12_4_;
      iVar97 = (int)uVar14 + auVar73._0_4_;
      iVar30 = (int)((ulong)uVar14 >> 0x20);
      iVar98 = iVar30 + auVar73._4_4_;
      iVar117 = (int)uVar15 + auVar73._8_4_;
      iVar62 = (int)((ulong)uVar15 >> 0x20);
      iVar118 = iVar62 + auVar73._12_4_;
      if (0x3f < uVar18) {
        uVar18 = 0x40;
      }
      iVar111 = (int)uVar32 + iVar61;
      iVar26 = (int)((ulong)uVar32 >> 0x20);
      iVar47 = (int)((ulong)uVar63 >> 0x20);
      iVar112 = iVar26 + iVar47;
      iVar113 = (int)uVar11 + auVar36._8_4_;
      iVar27 = (int)((ulong)uVar11 >> 0x20);
      iVar114 = iVar27 + auVar39._12_4_;
      iVar5 = (int)uVar12 + iVar60;
      iVar28 = (int)((ulong)uVar12 >> 0x20);
      iVar6 = iVar28 + iVar4;
      iVar7 = (int)uVar13 + iVar91;
      iVar29 = (int)((ulong)uVar13 >> 0x20);
      iVar8 = iVar29 + iVar92;
      uVar3 = CONCAT12((char)((uint)(iVar28 + iVar4) >> 0x10),
                       (short)((uint)((int)uVar12 + iVar60) >> 0x10)) & 0xff00ff;
      local_58[0] = (byte)iVar85;
      local_58[1] = (char)((uint)iVar85 >> 8);
      local_58[2] = (char)((uint)((int)uVar9 + auVar94._0_4_) >> 0x10);
      local_58[3] = (char)((uint)iVar85 >> 0x18);
      local_58[4] = (char)iVar84;
      local_58[5] = (char)((uint)iVar84 >> 8);
      local_58[6] = (char)((uint)(SUB84(uVar9,4) + auVar94._4_4_) >> 0x10);
      local_58[7] = (char)((uint)iVar84 >> 0x18);
      local_58[8] = (char)iVar90;
      local_58[9] = (char)((uint)iVar90 >> 8);
      local_58[10] = (char)((uint)((int)uVar10 + auVar94._8_4_) >> 0x10);
      local_58[0xb] = (char)((uint)iVar90 >> 0x18);
      local_58[0xc] = (char)iVar96;
      local_58[0xd] = (char)((uint)iVar96 >> 8);
      local_58[0xe] = (char)((uint)(SUB84(uVar10,4) + auVar94._12_4_) >> 0x10);
      local_58[0xf] = (char)((uint)iVar96 >> 0x18);
      local_58[0x10] = (char)iVar97;
      local_58[0x11] = (char)((uint)iVar97 >> 8);
      local_58[0x12] = (char)((uint)((int)uVar14 + auVar73._0_4_) >> 0x10);
      uStack_45 = (char)((uint)iVar97 >> 0x18);
      uStack_44 = (char)iVar98;
      uStack_43 = (char)((uint)iVar98 >> 8);
      uStack_42 = (char)((uint)(iVar30 + auVar73._4_4_) >> 0x10);
      uStack_41 = (char)((uint)iVar98 >> 0x18);
      uStack_40 = (char)iVar117;
      uStack_3f = (char)((uint)iVar117 >> 8);
      uStack_3e = (char)((uint)((int)uVar15 + auVar73._8_4_) >> 0x10);
      uStack_3d = (char)((uint)iVar117 >> 0x18);
      uStack_3c = (char)iVar118;
      uStack_3b = (char)((uint)iVar118 >> 8);
      uStack_3a = (char)((uint)(iVar62 + auVar73._12_4_) >> 0x10);
      uStack_39 = (char)((uint)iVar118 >> 0x18);
      uStack_38 = (char)iVar5;
      uStack_37 = (char)((uint)iVar5 >> 8);
      uStack_36 = (char)uVar3;
      uStack_35 = (char)((uint)iVar5 >> 0x18);
      uStack_34 = (char)iVar6;
      uStack_33 = (char)((uint)iVar6 >> 8);
      uStack_32 = (char)(uVar3 >> 0x10);
      uStack_31 = (char)((uint)iVar6 >> 0x18);
      uStack_30 = (char)iVar7;
      uStack_2f = (char)((uint)iVar7 >> 8);
      uStack_2e = (char)((uint)((int)uVar13 + iVar91) >> 0x10);
      uStack_2d = (char)((uint)iVar7 >> 0x18);
      uStack_2c = (char)iVar8;
      uStack_2b = (char)((uint)iVar8 >> 8);
      uStack_2a = (char)((uint)(iVar29 + iVar92) >> 0x10);
      uStack_29 = (char)((uint)iVar8 >> 0x18);
      uStack_28 = (char)iVar111;
      uStack_27 = (char)((uint)iVar111 >> 8);
      uStack_26 = (char)((uint)((int)uVar32 + iVar61) >> 0x10);
      uStack_25 = (char)((uint)iVar111 >> 0x18);
      uStack_24 = (char)iVar112;
      uStack_23 = (char)((uint)iVar112 >> 8);
      uStack_22 = (char)((uint)(iVar26 + iVar47) >> 0x10);
      uStack_21 = (char)((uint)iVar112 >> 0x18);
      uStack_20 = (char)iVar113;
      uStack_1f = (char)((uint)iVar113 >> 8);
      uStack_1e = (char)((uint)((int)uVar11 + auVar36._8_4_) >> 0x10);
      uStack_1d = (char)((uint)iVar113 >> 0x18);
      uStack_1c = (char)iVar114;
      uStack_1b = (char)((uint)iVar114 >> 8);
      uStack_1a = (char)((uint)(iVar27 + auVar39._12_4_) >> 0x10);
      uStack_19 = (char)((uint)iVar114 >> 0x18);
      if (uVar18 != 0) {
        if (uVar18 < 8) {
          uVar19 = 0;
        }
        else {
          if (uVar18 < 0x10) {
            uVar20 = 0;
          }
          else {
            uVar19 = uVar18 & 0x70;
            pauVar22 = (undefined1 (*) [16])(param_1 + uVar17);
            pbVar21 = local_58;
            uVar20 = uVar19;
            do {
              auVar39 = *pauVar22;
              uVar20 = uVar20 - 0x10;
              auVar42._0_8_ =
                   CONCAT17(auVar39[7] ^ pbVar21[7],
                            CONCAT16(auVar39[6] ^ pbVar21[6],
                                     CONCAT15(auVar39[5] ^ pbVar21[5],
                                              CONCAT14(auVar39[4] ^ pbVar21[4],
                                                       CONCAT13(auVar39[3] ^ pbVar21[3],
                                                                CONCAT12(auVar39[2] ^ pbVar21[2],
                                                                         CONCAT11(auVar39[1] ^
                                                                                  pbVar21[1],
                                                                                  auVar39[0] ^
                                                                                  *pbVar21)))))));
              auVar42[8] = auVar39[8] ^ pbVar21[8];
              auVar42[9] = auVar39[9] ^ pbVar21[9];
              auVar42[10] = auVar39[10] ^ pbVar21[10];
              auVar42[0xb] = auVar39[0xb] ^ pbVar21[0xb];
              auVar42[0xc] = auVar39[0xc] ^ pbVar21[0xc];
              auVar42[0xd] = auVar39[0xd] ^ pbVar21[0xd];
              auVar42[0xe] = auVar39[0xe] ^ pbVar21[0xe];
              auVar42[0xf] = auVar39[0xf] ^ pbVar21[0xf];
              *(long *)(*pauVar22 + 8) = auVar42._8_8_;
              *(undefined8 *)*pauVar22 = auVar42._0_8_;
              pauVar22 = pauVar22 + 1;
              pbVar21 = pbVar21 + 0x10;
            } while (uVar20 != 0);
            if (uVar18 == uVar19) goto LAB_0019d7bc;
            uVar20 = uVar19;
            if (((uint)uVar18 >> 3 & 1) == 0) goto LAB_0019d9d8;
          }
          uVar19 = uVar18 & 0x78;
          lVar23 = uVar20 - uVar19;
          pbVar21 = local_58 + uVar20;
          puVar24 = (undefined8 *)(param_1 + uVar20 + uVar17);
          do {
            uVar32 = *(undefined8 *)pbVar21;
            uVar63 = *puVar24;
            lVar23 = lVar23 + 8;
            *puVar24 = CONCAT17((byte)((ulong)uVar63 >> 0x38) ^ (byte)((ulong)uVar32 >> 0x38),
                                CONCAT16((byte)((ulong)uVar63 >> 0x30) ^
                                         (byte)((ulong)uVar32 >> 0x30),
                                         CONCAT15((byte)((ulong)uVar63 >> 0x28) ^
                                                  (byte)((ulong)uVar32 >> 0x28),
                                                  CONCAT14((byte)((ulong)uVar63 >> 0x20) ^
                                                           (byte)((ulong)uVar32 >> 0x20),
                                                           CONCAT13((byte)((ulong)uVar63 >> 0x18) ^
                                                                    (byte)((ulong)uVar32 >> 0x18),
                                                                    CONCAT12((byte)((ulong)uVar63 >>
                                                                                   0x10) ^
                                                                             (byte)((ulong)uVar32 >>
                                                                                   0x10),
                                                                             CONCAT11((byte)((ulong)
                                                  uVar63 >> 8) ^ (byte)((ulong)uVar32 >> 8),
                                                  (byte)uVar63 ^ (byte)uVar32)))))));
            pbVar21 = pbVar21 + 8;
            puVar24 = puVar24 + 1;
          } while (lVar23 != 0);
          if (uVar18 == uVar19) goto LAB_0019d7bc;
        }
LAB_0019d9d8:
        lVar23 = uVar18 - uVar19;
        pbVar21 = local_58 + uVar19;
        pbVar25 = (byte *)(param_1 + uVar19 + uVar17);
        do {
          lVar23 = lVar23 + -1;
          *pbVar25 = *pbVar25 ^ *pbVar21;
          pbVar21 = pbVar21 + 1;
          pbVar25 = pbVar25 + 1;
        } while (lVar23 != 0);
      }
LAB_0019d7bc:
      iVar16 = iVar16 + 1;
      uVar17 = uVar18 + uVar17;
      uVar32 = CONCAT44(iVar26,iVar16);
    } while (uVar17 < param_2);
  }
  if (*(long *)(lVar1 + 0x28) != local_18) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

/* ===== FUN_001a184c @ 001a184c [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_001a184c(undefined8 *param_1,long param_2,uint *param_3)

{
  uint uVar1;
  undefined4 uVar2;
  long lVar3;
  byte bVar5;
  byte bVar6;
  byte bVar7;
  byte bVar8;
  byte bVar9;
  byte bVar10;
  undefined8 uVar4;
  byte bVar11;
  byte bVar13;
  byte bVar14;
  byte bVar15;
  byte bVar16;
  byte bVar17;
  byte bVar18;
  undefined8 uVar12;
  byte bVar19;
  undefined1 local_158 [112];
  byte local_e8;
  byte local_e7;
  undefined8 local_c8;
  undefined4 local_c0;
  undefined4 local_bc;
  undefined4 local_b8;
  undefined8 local_b0;
  undefined8 uStack_a8;
  undefined8 local_a0;
  undefined8 local_98;
  undefined8 local_90;
  undefined8 uStack_88;
  undefined8 local_80;
  undefined8 local_78;
  undefined8 local_70;
  undefined8 uStack_68;
  undefined8 local_60;
  undefined8 local_58;
  undefined8 local_50;
  undefined8 uStack_48;
  undefined8 local_40;
  undefined8 local_38;
  
  uVar2 = 0;
  if (((param_1 != (undefined8 *)0x0) && (param_2 != 0)) && (param_3 != (uint *)0x0)) {
    if (*(int *)(param_2 + 8) == 2) {
      uVar12 = param_1[1];
      uVar4 = *param_1;
      local_c8 = *(undefined8 *)(param_2 + 8);
      uStack_48 = 0x3636363636363636;
      local_50 = 0x3636363636363636;
      local_38 = 0x3636363636363636;
      local_40 = 0x3636363636363636;
      bVar5 = (byte)((ulong)uVar4 >> 8);
      bVar6 = (byte)((ulong)uVar4 >> 0x10);
      bVar7 = (byte)((ulong)uVar4 >> 0x18);
      bVar8 = (byte)((ulong)uVar4 >> 0x20);
      bVar9 = (byte)((ulong)uVar4 >> 0x28);
      bVar10 = (byte)((ulong)uVar4 >> 0x30);
      bVar11 = (byte)((ulong)uVar4 >> 0x38);
      local_70 = CONCAT17(bVar11 ^ (byte)((ulong)_DAT_001128b0 >> 0x38),
                          CONCAT16(bVar10 ^ (byte)((ulong)_DAT_001128b0 >> 0x30),
                                   CONCAT15(bVar9 ^ (byte)((ulong)_DAT_001128b0 >> 0x28),
                                            CONCAT14(bVar8 ^ (byte)((ulong)_DAT_001128b0 >> 0x20),
                                                     CONCAT13(bVar7 ^ (byte)((ulong)_DAT_001128b0 >>
                                                                            0x18),
                                                              CONCAT12(bVar6 ^ (byte)((ulong)
                                                  _DAT_001128b0 >> 0x10),
                                                  CONCAT11(bVar5 ^ (byte)((ulong)_DAT_001128b0 >> 8)
                                                           ,(byte)uVar4 ^ (byte)_DAT_001128b0)))))))
      ;
      bVar13 = (byte)((ulong)uVar12 >> 8);
      bVar14 = (byte)((ulong)uVar12 >> 0x10);
      bVar15 = (byte)((ulong)uVar12 >> 0x18);
      bVar16 = (byte)((ulong)uVar12 >> 0x20);
      bVar17 = (byte)((ulong)uVar12 >> 0x28);
      bVar18 = (byte)((ulong)uVar12 >> 0x30);
      bVar19 = (byte)((ulong)uVar12 >> 0x38);
      uStack_68 = CONCAT17(bVar19 ^ (byte)((ulong)_UNK_001128b8 >> 0x38),
                           CONCAT16(bVar18 ^ (byte)((ulong)_UNK_001128b8 >> 0x30),
                                    CONCAT15(bVar17 ^ (byte)((ulong)_UNK_001128b8 >> 0x28),
                                             CONCAT14(bVar16 ^ (byte)((ulong)_UNK_001128b8 >> 0x20),
                                                      CONCAT13(bVar15 ^ (byte)((ulong)_UNK_001128b8
                                                                              >> 0x18),
                                                               CONCAT12(bVar14 ^ (byte)((ulong)
                                                  _UNK_001128b8 >> 0x10),
                                                  CONCAT11(bVar13 ^ (byte)((ulong)_UNK_001128b8 >> 8
                                                                          ),
                                                           (byte)uVar12 ^ (byte)_UNK_001128b8)))))))
      ;
      local_c0 = *(undefined4 *)(param_2 + 0x10);
      local_58 = 0x3636363636363636;
      local_60 = 0x3636363636363636;
      uStack_a8 = CONCAT17(bVar19 ^ (byte)((ulong)_UNK_00112b68 >> 0x38),
                           CONCAT16(bVar18 ^ (byte)((ulong)_UNK_00112b68 >> 0x30),
                                    CONCAT15(bVar17 ^ (byte)((ulong)_UNK_00112b68 >> 0x28),
                                             CONCAT14(bVar16 ^ (byte)((ulong)_UNK_00112b68 >> 0x20),
                                                      CONCAT13(bVar15 ^ (byte)((ulong)_UNK_00112b68
                                                                              >> 0x18),
                                                               CONCAT12(bVar14 ^ (byte)((ulong)
                                                  _UNK_00112b68 >> 0x10),
                                                  CONCAT11(bVar13 ^ (byte)((ulong)_UNK_00112b68 >> 8
                                                                          ),
                                                           (byte)uVar12 ^ (byte)_UNK_00112b68)))))))
      ;
      local_b0 = CONCAT17(bVar11 ^ (byte)((ulong)_DAT_00112b60 >> 0x38),
                          CONCAT16(bVar10 ^ (byte)((ulong)_DAT_00112b60 >> 0x30),
                                   CONCAT15(bVar9 ^ (byte)((ulong)_DAT_00112b60 >> 0x28),
                                            CONCAT14(bVar8 ^ (byte)((ulong)_DAT_00112b60 >> 0x20),
                                                     CONCAT13(bVar7 ^ (byte)((ulong)_DAT_00112b60 >>
                                                                            0x18),
                                                              CONCAT12(bVar6 ^ (byte)((ulong)
                                                  _DAT_00112b60 >> 0x10),
                                                  CONCAT11(bVar5 ^ (byte)((ulong)_DAT_00112b60 >> 8)
                                                           ,(byte)uVar4 ^ (byte)_DAT_00112b60)))))))
      ;
      local_98 = 0x5c5c5c5c5c5c5c5c;
      local_a0 = 0x5c5c5c5c5c5c5c5c;
      uStack_88 = 0x5c5c5c5c5c5c5c5c;
      local_90 = 0x5c5c5c5c5c5c5c5c;
      local_78 = 0x5c5c5c5c5c5c5c5c;
      local_80 = 0x5c5c5c5c5c5c5c5c;
      local_bc = *(undefined4 *)(param_2 + 4);
      local_b8 = 0x1c2d3e4f;
      FUN_001a1c68(local_158);
      FUN_001a1c8c(local_158,&local_70,0x40);
      FUN_001a1c8c(local_158,&local_c8,0x14);
      FUN_001a1fac(local_158,&local_e8);
      FUN_001a1c68(local_158);
      FUN_001a1c8c(local_158,&local_b0,0x40);
      FUN_001a1c8c(local_158,&local_e8,0x20);
      FUN_001a1fac(local_158,&local_e8);
      lVar3 = 0;
      uVar1 = (uint)local_e8 | (local_e7 & 0x7f) << 8;
      if (uVar1 == 0) {
        uVar1 = 1;
      }
      *param_3 = uVar1;
      do {
        local_158[lVar3] = 0;
        lVar3 = lVar3 + 1;
      } while (lVar3 != 0x70);
      uVar2 = 1;
    }
    else {
      uVar2 = 0;
    }
  }
  return uVar2;
}

/* ===== FUN_001a1c68 @ 001a1c68 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_001a1c68(undefined8 *param_1)

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

/* ===== FUN_001a1fac @ 001a1fac [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_001a1fac(long param_1,long param_2)

{
  uint uVar1;
  uint uVar2;
  ulong uVar3;
  long lVar4;
  long lVar5;
  undefined1 auVar6 [16];
  undefined1 auVar7 [16];
  undefined1 auVar8 [16];
  undefined8 local_a0;
  undefined8 uStack_98;
  undefined8 uStack_90;
  undefined8 uStack_88;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined8 uStack_70;
  undefined8 uStack_68;
  undefined8 local_60;
  undefined8 uStack_58;
  undefined8 uStack_50;
  undefined8 uStack_48;
  undefined8 local_40;
  undefined8 uStack_38;
  undefined8 uStack_30;
  undefined8 uStack_28;
  
  uVar3 = *(ulong *)(param_1 + 0x20);
  lVar5 = uVar3 << 3;
  lVar4 = 0x38;
  if (0x37 < *(ulong *)(param_1 + 0x28)) {
    lVar4 = 0x78;
  }
  auVar7._8_8_ = lVar5;
  auVar7._0_8_ = lVar5;
  lVar4 = lVar4 - *(ulong *)(param_1 + 0x28);
  auVar8._8_8_ = _UNK_00112bc8;
  auVar8._0_8_ = _DAT_00112bc0;
  auVar6 = NEON_ushl(auVar7,_DAT_00112990,8);
  uStack_98 = 0;
  uStack_88 = 0;
  uStack_90 = 0;
  uStack_78 = 0;
  local_80 = 0;
  uStack_68 = 0;
  uStack_70 = 0;
  auVar8 = NEON_ushl(auVar7,auVar8,8);
  uStack_58 = 0;
  local_60 = 0;
  uStack_48 = 0;
  uStack_50 = 0;
  uStack_38 = 0;
  local_40 = 0;
  uStack_28 = 0;
  uStack_30 = 0;
  local_a0 = 0x80;
  *(char *)((long)&local_a0 + lVar4 + 4) = (char)(uVar3 >> 0x15);
  *(char *)((long)&local_a0 + lVar4 + 7) = (char)lVar5;
  *(char *)((long)&local_a0 + lVar4 + 5) = (char)(uVar3 >> 0xd);
  *(char *)((long)&local_a0 + lVar4 + 6) = (char)(uVar3 >> 5);
  *(uint *)((long)&local_a0 + lVar4) =
       CONCAT13(auVar8[8],CONCAT12(auVar8[0],CONCAT11(auVar6[8],auVar6[0])));
  FUN_001a1c8c(param_1,&local_a0,lVar4 + 8);
  uVar2 = 0;
  uVar3 = 0;
  do {
    uVar1 = uVar2 ^ 0xffffffff;
    uVar2 = uVar2 + 8;
    *(char *)(param_2 + uVar3) =
         (char)(*(uint *)(param_1 + (uVar3 & 0xfffffffc)) >> (ulong)(uVar1 & 0x18));
    uVar3 = uVar3 + 1;
  } while (uVar3 != 0x20);
  return;
}

/* ===== FUN_001a5640 @ 001a5640 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_001a5640(int *param_1,int *param_2,int *param_3,ulong param_4)

{
  long *plVar1;
  byte *pbVar2;
  byte bVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  char cVar10;
  bool bVar11;
  int iVar12;
  long *plVar13;
  ulong *puVar14;
  ulong uVar15;
  undefined4 uVar16;
  ulong uVar17;
  undefined8 uVar18;
  ulong *puVar19;
  int *piVar20;
  int *piVar21;
  long lVar22;
  long lVar23;
  long lVar24;
  ulong uVar25;
  long lVar26;
  long lVar27;
  long *plVar28;
  ulong uVar29;
  long lVar30;
  uint uVar31;
  long *plVar32;
  long *plVar33;
  undefined8 uVar34;
  long *plVar35;
  long *plVar36;
  int iVar37;
  int iVar38;
  ulong uVar39;
  ulong *puVar40;
  uint *puVar41;
  undefined8 uVar42;
  undefined8 uVar43;
  undefined8 uVar44;
  undefined8 uVar45;
  undefined8 uVar46;
  
  if (param_1 == (int *)0x0) {
    return 0;
  }
  if (param_3 == (int *)0x0) {
    return 0;
  }
  if (*param_1 != 0x4e444331) {
    return 0;
  }
  if ((*param_3 != 1) || (param_3[1] != 0x38)) {
    return 0;
  }
  plVar1 = (long *)(param_1 + 0x57da);
  pbVar2 = (byte *)(param_1 + 1);
  do {
    bVar3 = *pbVar2;
    cVar10 = '\x01';
    bVar11 = (bool)ExclusiveMonitorPass(pbVar2,0x10);
    if (bVar11) {
      *pbVar2 = 1;
      cVar10 = ExclusiveMonitorsStatus();
    }
  } while (cVar10 != '\0');
  if ((bVar3 & 1) != 0) {
    *(undefined8 *)(param_3 + 2) = DAT_0010e560;
    return 0;
  }
  if ((((((param_2 == (int *)0x0) || (*param_2 != 1)) || (param_2[1] != 0x88)) ||
       ((piVar21 = *(int **)(param_2 + 6), piVar21 == (int *)0x0 || (*piVar21 != 1)))) ||
      (((piVar21[1] != 0x38 ||
        ((piVar21[2] != 1 || (uVar17 = *(ulong *)(piVar21 + 8), uVar17 == 0)))) ||
       (iVar12 = piVar21[3], iVar12 == 0)))) ||
     ((((((piVar21[4] == 0 ||
          (uVar29 = *(ulong *)(piVar21 + 10), uVar29 != *(ulong *)(param_2 + 0xc))) ||
         (uVar25 = *(ulong *)(piVar21 + 0xc), uVar25 != *(ulong *)(param_2 + 0x10))) ||
        (((lVar22 = *(long *)(param_2 + 8), lVar22 == 0 ||
          (lVar23 = *(long *)(param_2 + 10), lVar23 == 0)) ||
         ((1 < (uint)param_2[2] || ((param_2[5] != 1 || (uVar29 + 0x2000 < 0x12000)))))))) ||
       ((((uVar29 & 7) != 0 ||
         (((((*(ulong *)(param_2 + 0xe) + 0x2000 < 0x12000 || ((*(ulong *)(param_2 + 0xe) & 7) != 0)
             ) || (uVar25 + 0x2000 < 0x12000)) ||
           (((uVar25 & 7) != 0 || (uVar29 = *(ulong *)(param_2 + 0x12), uVar29 + 0x2000 < 0x12000)))
           ) || (((uVar29 & 7) != 0 ||
                 ((999999 < param_2[0x14] - 1000000U || (1999999 < param_2[0x15] - 16000000U))))))))
        || (param_2[0x18] - 60000U < 0xfffe2b41)))) ||
      ((param_2[0x19] - 60000U < 0xfffe2b41 ||
       ((((ABS((float)param_2[0x18] / 300.0) < 0.5 && (ABS((float)param_2[0x19] / 300.0) < 0.5)) ||
         (param_2[0x16] == -1)) || (param_2[0x17] == -1)))))))) {
    lVar22 = 0;
    *(undefined8 *)(param_1 + 0x3bbe) = DAT_0010e7f0;
    do {
      if (*(long *)((long)param_1 + lVar22 + 0x10) != 0) {
        *(undefined4 *)((long)param_1 + lVar22 + 0x7c) = 1;
      }
      lVar22 = lVar22 + 0x70;
    } while (lVar22 != 0x7000);
    uVar16 = 0;
    goto LAB_001a5994;
  }
  if (uVar17 <= *(ulong *)(param_1 + 2)) {
    param_3[0xc] = 0;
    uVar18 = _UNK_00112c18;
    uVar46 = _DAT_00112c10;
    param_3[0xd] = 0;
    param_3[6] = 0;
    param_3[7] = 0;
    param_3[4] = 0;
    param_3[5] = 0;
    param_3[10] = 0;
    param_3[0xb] = 0;
    param_3[8] = 0;
    param_3[9] = 0;
    *(undefined8 *)(param_3 + 2) = uVar18;
    *(undefined8 *)param_3 = uVar46;
    pbVar2[0] = 0;
    pbVar2[1] = 0;
    pbVar2[2] = 0;
    pbVar2[3] = 0;
    return 0;
  }
  *(ulong *)(param_1 + 2) = uVar17;
  if ((param_2[3] == 1) && (param_2[4] == 1)) {
    uVar31 = param_2[0x1c];
    if (uVar31 < 0x101) {
      uVar4 = param_2[0x20];
      if (((uVar4 < 0x31) && ((uVar31 == 0 || (*(long *)(param_2 + 0x1a) != 0)))) &&
         ((uVar4 == 0 || (*(long *)(param_2 + 0x1e) != 0)))) {
        if (uVar31 != 0) {
          uVar25 = 0;
          lVar26 = *(long *)(param_2 + 0x1a);
          do {
            plVar13 = (long *)(lVar26 + uVar25 * 0x58);
            uVar5 = *(uint *)(plVar13 + 3);
            if (((0x1ff < uVar5) ||
                (((uVar25 != 0 && (uVar5 <= *(uint *)(lVar26 + uVar25 * 0x58 + -0x40))) ||
                 (0x3f < *(uint *)(lVar26 + uVar25 * 0x58 + 0x14))))) ||
               (*(int *)(lVar26 + uVar25 * 0x58 + 0x50) - 2U < 0xfffffffd)) goto LAB_001a58ec;
            if (uVar25 != 0) {
              piVar21 = (int *)(lVar26 + 0x10);
              uVar15 = uVar25;
              do {
                if ((*plVar13 == *(long *)(piVar21 + -4)) ||
                   ((iVar37 = *(int *)(lVar26 + uVar25 * 0x58 + 0x10), iVar37 != 0 &&
                    (iVar37 == *piVar21)))) goto LAB_001a58ec;
                uVar15 = uVar15 - 1;
                piVar21 = piVar21 + 0x16;
              } while (uVar15 != 0);
            }
            uVar25 = uVar25 + 1;
          } while (uVar25 != uVar31);
        }
        if (uVar4 != 0) {
          lVar26 = *(long *)(param_2 + 0x1e);
          uVar25 = 0;
          bVar11 = false;
          do {
            puVar14 = (ulong *)(lVar26 + uVar25 * 0x28);
            uVar5 = (uint)puVar14[2];
            if ((((0x1ff < uVar5) ||
                 ((uVar25 != 0 && (uVar5 <= *(uint *)(lVar26 + uVar25 * 0x28 + -0x18))))) ||
                ((*(int *)(lVar26 + uVar25 * 0x28 + 0xc) != 1 ||
                 (((uVar6 = *(uint *)(lVar26 + uVar25 * 0x28 + 0x14), 1 < uVar6 ||
                   (uVar15 = *puVar14, uVar15 + 0x2000 < 0x12000)) || ((uVar15 & 7) != 0)))))) ||
               ((uVar15 == uVar29 ||
                (iVar37 = *(int *)(lVar26 + uVar25 * 0x28 + 8), iVar37 - 2000000U < 0xfff0bdc0))))
            goto LAB_001a5ca4;
            piVar21 = (int *)(lVar26 + 8);
            uVar39 = uVar25;
            if (uVar6 != 0) {
              uVar46 = *(undefined8 *)(lVar26 + uVar25 * 0x28 + 0x20);
              uVar18 = NEON_cmhi(0xfffe2b41fffe2b41,
                                 CONCAT44((int)((ulong)uVar46 >> 0x20) + -60000,(int)uVar46 + -60000
                                         ),4);
              if ((((byte)uVar18 | (byte)((ulong)uVar18 >> 0x20)) & 1) != 0) goto LAB_001a5ca4;
              uVar46 = NEON_scvtf(uVar46,4);
              uVar46 = NEON_fcmgt(0x3f0000003f000000,
                                  CONCAT44(ABS((float)((ulong)uVar46 >> 0x20) / 300.0),
                                           ABS((float)uVar46 / 300.0)),4);
              if (((((~(byte)((ulong)uVar46 >> 0x20) | ~(byte)uVar46) & 1) == 0) ||
                  (*(int *)(lVar26 + uVar25 * 0x28 + 0x18) == -1)) ||
                 (*(int *)(lVar26 + uVar25 * 0x28 + 0x1c) == -1)) goto LAB_001a5ca4;
            }
            for (; uVar39 != 0; uVar39 = uVar39 - 1) {
              if ((uVar15 == *(ulong *)(piVar21 + -2)) || (iVar37 == *piVar21)) goto LAB_001a5ca4;
              piVar21 = piVar21 + 10;
            }
            if (uVar31 != 0) {
              piVar21 = (int *)(*(long *)(param_2 + 0x1a) + 0x10);
              uVar39 = (ulong)uVar31;
              do {
                if (((uVar5 == piVar21[2]) || (uVar15 == *(ulong *)(piVar21 + -4))) ||
                   (iVar37 == *piVar21)) goto LAB_001a5ca4;
                uVar39 = uVar39 - 1;
                piVar21 = piVar21 + 0x16;
              } while (uVar39 != 0);
            }
            uVar25 = uVar25 + 1;
            bVar11 = uVar4 <= uVar25;
            if (uVar25 == uVar4) goto LAB_001a5ca4;
          } while( true );
        }
        goto LAB_001a5ca8;
      }
    }
  }
LAB_001a58ec:
  lVar26 = 0;
  param_1[0x3bc0] = 0;
  param_1[0x3bc1] = 0;
  *(ulong *)(param_1 + 0x3bc2) = uVar17;
  *(long *)(param_1 + 0x3bc4) = lVar22;
  *(long *)(param_1 + 0x3bc6) = lVar23;
  uVar18 = _UNK_001128c8;
  uVar46 = _DAT_001128c0;
  param_1[0x3bc8] = iVar12;
  param_1[0x3bc9] = 0;
  *(undefined8 *)(param_1 + 0x3bbe) = uVar18;
  *(undefined8 *)(param_1 + 0x3bbc) = uVar46;
  do {
    if (*(long *)((long)param_1 + lVar26 + 0x10) != 0) {
      *(undefined4 *)((long)param_1 + lVar26 + 0x7c) = 1;
    }
    uVar16 = 0;
    lVar26 = lVar26 + 0x70;
  } while (lVar26 != 0x7000);
LAB_001a5994:
  uVar46 = *(undefined8 *)(param_1 + 0x3bc0);
  uVar44 = *(undefined8 *)(param_1 + 0x3bc6);
  uVar34 = *(undefined8 *)(param_1 + 0x3bc4);
  uVar43 = *(undefined8 *)(param_1 + 0x3bbe);
  uVar42 = *(undefined8 *)(param_1 + 0x3bbc);
  uVar18 = *(undefined8 *)(param_1 + 0x3bc8);
  *(undefined8 *)(param_3 + 6) = *(undefined8 *)(param_1 + 0x3bc2);
  *(undefined8 *)(param_3 + 4) = uVar46;
  *(undefined8 *)(param_3 + 10) = uVar44;
  *(undefined8 *)(param_3 + 8) = uVar34;
  uVar46 = DAT_0010e7a8;
  *(undefined8 *)(param_3 + 2) = uVar43;
  *(undefined8 *)param_3 = uVar42;
  *(undefined8 *)(param_3 + 0xc) = uVar18;
  *(undefined8 *)param_3 = uVar46;
  param_1[1] = 0;
  return uVar16;
LAB_001a5ca4:
  if (bVar11) {
LAB_001a5ca8:
    if ((param_4 != 0) && (param_4 < *(ulong *)(param_1 + 0x3bd8))) {
      lVar22 = 0;
      *(undefined8 *)(param_1 + 0x3bbe) = DAT_0010e630;
      do {
        if (*(long *)((long)param_1 + lVar22 + 0x10) != 0) {
          *(undefined4 *)((long)param_1 + lVar22 + 0x7c) = 1;
        }
        uVar16 = 0;
        lVar22 = lVar22 + 0x70;
      } while (lVar22 != 0x7000);
      goto LAB_001a5994;
    }
    puVar14 = (ulong *)(param_1 + 0x3bda);
    memcpy(puVar14,param_1 + 4,0xef58);
    lVar22 = *(long *)(param_1 + 0x77a8);
    if ((((lVar22 == *(long *)(param_2 + 8)) &&
         ((lVar23 = *(long *)(param_1 + 0x77a0), lVar23 == *(long *)(param_2 + 0xc) &&
          (lVar26 = *(long *)(param_1 + 0x77a2), lVar26 == *(long *)(param_2 + 0xe))))) &&
        (lVar24 = *(long *)(param_1 + 0x77a4), lVar24 == *(long *)(param_2 + 0x10))) &&
       (((lVar27 = *(long *)(param_1 + 0x77a6), lVar27 == *(long *)(param_2 + 0x12) &&
         (iVar12 = param_1[0x77aa], iVar12 == param_2[0x14])) &&
        (iVar37 = param_1[0x77ab], iVar37 == param_2[0x15])))) {
      iVar38 = param_1[0x77ac];
      lVar30 = *(long *)(param_2 + 6);
      if ((iVar38 != *(int *)(lVar30 + 0x10)) ||
         (uVar31 = *(uint *)(lVar30 + 0xc), uVar31 < (uint)param_1[0x77ad])) goto LAB_001a5de0;
    }
    else {
LAB_001a5de0:
      memset(puVar14,0,0xef58);
      lVar30 = *(long *)(param_2 + 6);
      lVar22 = *(long *)(param_2 + 8);
      lVar23 = *(long *)(param_2 + 0xc);
      lVar26 = *(long *)(param_2 + 0xe);
      lVar24 = *(long *)(param_2 + 0x10);
      lVar27 = *(long *)(param_2 + 0x12);
      iVar12 = param_2[0x14];
      iVar37 = param_2[0x15];
      uVar31 = *(uint *)(lVar30 + 0xc);
      iVar38 = *(int *)(lVar30 + 0x10);
    }
    *(long *)(param_1 + 0x77a0) = lVar23;
    *(long *)(param_1 + 0x77a2) = lVar26;
    *(long *)(param_1 + 0x77a4) = lVar24;
    *(long *)(param_1 + 0x77a6) = lVar27;
    param_1[0x77aa] = iVar12;
    param_1[0x77ab] = iVar37;
    *(long *)(param_1 + 0x77a8) = lVar22;
    param_1[0x77ac] = iVar38;
    param_1[0x77ad] = uVar31;
    if (param_4 != 0) {
      *(ulong *)(param_1 + 0x77ae) = param_4;
    }
    uVar46 = DAT_0010e7a8;
    uVar18 = *(undefined8 *)(lVar30 + 0x20);
    uVar34 = *(undefined8 *)(param_2 + 10);
    iVar12 = 0x13;
    if (param_2[2] != 0) {
      iVar12 = 0;
    }
    param_1[0x7796] = param_2[0x1c];
    param_1[0x7797] = 0;
    *(undefined8 *)(param_1 + 0x7792) = uVar46;
    param_1[0x7794] = 1;
    param_1[0x7795] = iVar12;
    *(undefined8 *)(param_1 + 0x7798) = uVar18;
    *(long *)(param_1 + 0x779a) = lVar22;
    *(undefined8 *)(param_1 + 0x779c) = uVar34;
    param_1[0x779e] = uVar31;
    param_1[0x779f] = 0;
    uVar18 = DAT_0010e6e0;
    uVar46 = DAT_0010e588;
    if (param_2[0x1c] == 0) {
      uVar17 = 0;
    }
    else {
      uVar29 = 0;
      uVar17 = 0;
      do {
        uVar25 = (ulong)(uint)param_2[0x20];
        lVar22 = *(long *)(param_2 + 0x1a);
        if (uVar17 < uVar25) {
          do {
            lVar23 = *(long *)(param_2 + 0x1e);
            if (*(uint *)(lVar22 + uVar29 * 0x58 + 0x18) <= *(uint *)(lVar23 + uVar17 * 0x28 + 0x10)
               ) break;
            if (*(int *)(lVar23 + uVar17 * 0x28 + 0x14) != 0) {
              plVar13 = (long *)(lVar23 + uVar17 * 0x28);
              lVar26 = 0;
              lVar24 = 0x30;
              plVar28 = plVar13 + 1;
              lVar27 = *plVar13;
              iVar12 = *(int *)(*(long *)(param_2 + 6) + 0xc);
              plVar33 = (long *)0x0;
              plVar36 = (long *)0x0;
              plVar13 = plVar1;
              do {
                if (*plVar13 == lVar27) {
                  plVar13 = plVar1 + lVar26 * 4;
                  goto LAB_001a5fd8;
                }
                if ((int)plVar13[1] == (int)*plVar28) goto LAB_001a5fd8;
                if (((*plVar13 != 0) ||
                    (plVar32 = plVar33, plVar35 = plVar13, plVar36 != (long *)0x0)) &&
                   ((plVar32 = plVar13, plVar35 = plVar36, plVar33 != (long *)0x0 &&
                    (*(uint *)((long)plVar33 + 0x14) <= *(uint *)((long)plVar13 + 0x14))))) {
                  plVar32 = plVar33;
                }
                lVar26 = lVar26 + 1;
                plVar13 = plVar13 + 4;
                lVar24 = lVar24 + -1;
                plVar33 = plVar32;
                plVar36 = plVar35;
              } while (lVar24 != 0);
              plVar13 = (long *)0x0;
LAB_001a5fd8:
              if (plVar36 != (long *)0x0) {
                plVar33 = plVar36;
              }
              if (plVar13 != (long *)0x0) {
                plVar33 = plVar13;
              }
              if (plVar33 != (long *)0x0) {
                lVar23 = lVar23 + uVar17 * 0x28;
                uVar44 = *(undefined8 *)(lVar23 + 0x18);
                uVar34 = *(undefined8 *)(lVar23 + 0x20);
                lVar23 = *plVar28;
                *plVar33 = lVar27;
                *(int *)((long)plVar33 + 0x14) = iVar12;
                *(undefined8 *)((long)plVar33 + 0xc) = uVar44;
                uVar34 = NEON_scvtf(uVar34,4);
                *(int *)(plVar33 + 1) = (int)lVar23;
                plVar33[3] = CONCAT44((float)((ulong)uVar34 >> 0x20) / 300.0,(float)uVar34 / 300.0);
                uVar25 = (ulong)(uint)param_2[0x20];
              }
            }
            uVar17 = uVar17 + 1;
          } while (uVar17 < uVar25);
          uVar17 = uVar17 & 0xffffffff;
        }
        puVar19 = (ulong *)(lVar22 + uVar29 * 0x58);
        puVar41 = (uint *)(lVar22 + uVar29 * 0x58 + 0x14);
        uVar25 = *puVar19;
        if (((((((*puVar41 ^ 0xffffffff) & 7) == 0 && 0x11fff < uVar25 + 0x2000) &&
               (uVar25 & 7) == 0) &&
             (uVar15 = *(ulong *)(lVar22 + uVar29 * 0x58 + 8), 0x11fff < uVar15 + 0x2000)) &&
            ((uVar15 & 7) == 0)) && (iVar12 = *(int *)(lVar22 + uVar29 * 0x58 + 0x10), 0 < iVar12))
        {
          lVar23 = lVar22 + uVar29 * 0x58;
          piVar21 = (int *)(lVar23 + 0x28);
          if ((*piVar21 - 60000U < 0xfffe2b41) ||
             (piVar20 = (int *)(lVar23 + 0x2c), *piVar20 - 60000U < 0xfffe2b41)) goto LAB_001a6060;
          puVar40 = (ulong *)FUN_001a6718(puVar14,uVar25,iVar12,
                                          *(undefined4 *)(*(long *)(param_2 + 6) + 0xc));
          if (puVar40 == (ulong *)0x0) goto LAB_001a60ac;
          FUN_001a6810((float)*piVar21 / 300.0,(float)*piVar20 / 300.0,(float)param_2[0x18] / 300.0,
                       (float)param_2[0x19] / 300.0,puVar40,
                       *(undefined4 *)(*(long *)(param_2 + 6) + 0xc));
          if (param_4 == 0) {
            uVar34 = 0;
          }
          else {
            FUN_001a692c((float)*piVar21 / 300.0,(float)*piVar20 / 300.0,puVar40,param_4);
            uVar34 = *(undefined8 *)((long)puVar40 + 0x24);
            *(undefined8 *)((long)puVar40 + 0x24) = *(undefined8 *)((long)puVar40 + 0x4c);
          }
          bVar11 = param_4 != 0;
          if ((((*puVar41 == 0x3f) && (*(int *)(lVar22 + uVar29 * 0x58 + 0x20) != -1)) &&
              (*(int *)(lVar22 + uVar29 * 0x58 + 0x24) != -1)) &&
             (*(uint *)(lVar22 + uVar29 * 0x58 + 0x3c) < 2)) {
            iVar12 = FUN_001a6a38(puVar14,param_2,puVar19,puVar40);
          }
          else {
            iVar12 = 1;
            *(int *)((long)puVar40 + 0x6c) = 1;
          }
        }
        else {
LAB_001a6060:
          lVar23 = 0x100;
          puVar40 = puVar14;
          do {
            if (*puVar40 == uVar25) {
              *(int *)((long)puVar40 + 0x6c) = 1;
              goto LAB_001a6090;
            }
            puVar40 = puVar40 + 0xe;
            lVar23 = lVar23 + -1;
          } while (lVar23 != 0);
          puVar40 = (ulong *)0x0;
LAB_001a6090:
          if ((uVar25 + 0x2000 < 0x12000) || ((uVar25 & 7) != 0)) {
            lVar23 = 0x100;
            piVar21 = param_1 + 0x3bf5;
            do {
              if (*(long *)(piVar21 + -0x1b) != 0) {
                *piVar21 = 1;
              }
              lVar23 = lVar23 + -1;
              piVar21 = piVar21 + 0x1c;
            } while (lVar23 != 0);
          }
LAB_001a60ac:
          bVar11 = false;
          uVar34 = 0;
          iVar12 = 1;
        }
        lVar22 = lVar22 + uVar29 * 0x58;
        uVar25 = *puVar19;
        iVar37 = *(int *)(lVar22 + 0x1c);
        iVar38 = *(int *)(lVar22 + 0x3c);
        piVar21 = param_1 + uVar29 * 0x1e + 0x5992;
        iVar7 = *(int *)(lVar22 + 0x10);
        iVar8 = *(int *)(*(long *)(param_2 + 6) + 0xc);
        iVar9 = *(int *)(lVar22 + 0x50);
        uVar44 = *(undefined8 *)(*(long *)(param_2 + 6) + 0x20);
        uVar43 = *(undefined8 *)(param_2 + 10);
        uVar42 = *(undefined8 *)(param_2 + 8);
        *(undefined8 *)piVar21 = uVar18;
        piVar21[2] = (uint)(iVar12 != 1 && iVar12 != 0x16);
        piVar21[3] = (uint)(iVar12 == 0);
        piVar21[4] = iVar12;
        piVar21[5] = iVar37;
        piVar21[6] = 0x5c;
        piVar21[7] = iVar38;
        *(ulong *)(piVar21 + 8) = uVar25;
        piVar21[10] = iVar7;
        piVar21[0xb] = iVar8;
        piVar21[0xe] = 0;
        piVar21[0xf] = 0;
        piVar21[0x10] = 0;
        piVar21[0x11] = 0;
        piVar21[0xc] = 0;
        piVar21[0xd] = 0;
        piVar21[0x12] = 0;
        piVar21[0x13] = iVar9;
        *(undefined8 *)(piVar21 + 0x14) = uVar44;
        *(undefined8 *)(piVar21 + 0x18) = uVar43;
        *(undefined8 *)(piVar21 + 0x16) = uVar42;
        piVar21[0x1a] = 0;
        piVar21[0x1b] = 0;
        piVar21[0x1c] = 0;
        piVar21[0x1d] = 0;
        if (puVar40 != (ulong *)0x0) {
          uVar45 = *(undefined8 *)((long)puVar40 + 0x24);
          uVar43 = *(undefined8 *)((long)puVar40 + 0x1c);
          piVar21[0xc] = (int)puVar40[2];
          uVar42 = *(undefined8 *)((long)puVar40 + 0x5c);
          uVar44 = *(undefined8 *)((long)puVar40 + 0x54);
          *(undefined8 *)(piVar21 + 0x1c) = uVar45;
          *(undefined8 *)(piVar21 + 0x1a) = uVar43;
          *(undefined8 *)(piVar21 + 0xf) = uVar42;
          *(undefined8 *)(piVar21 + 0xd) = uVar44;
          *(undefined8 *)(piVar21 + 0x11) = *(undefined8 *)((long)puVar40 + 100);
        }
        if (bVar11) {
          *(undefined8 *)((long)puVar40 + 0x24) = uVar34;
        }
        if ((iVar12 == 0x16) || (iVar12 == 1)) {
          *(undefined8 *)(param_1 + 0x7794) = uVar46;
          param_1[0x7797] = param_1[0x7797] + 1;
        }
        uVar29 = uVar29 + 1;
      } while (uVar29 < (uint)param_2[0x1c]);
    }
    uVar29 = (ulong)(uint)param_2[0x20];
    if (uVar17 < uVar29) {
      do {
        lVar22 = *(long *)(param_2 + 0x1e);
        if (*(int *)(lVar22 + uVar17 * 0x28 + 0x14) != 0) {
          plVar13 = (long *)(lVar22 + uVar17 * 0x28);
          lVar23 = 0;
          lVar26 = 0x30;
          plVar28 = plVar13 + 1;
          lVar24 = *plVar13;
          iVar12 = *(int *)(*(long *)(param_2 + 6) + 0xc);
          plVar33 = (long *)0x0;
          plVar36 = (long *)0x0;
          plVar13 = plVar1;
          do {
            if (*plVar13 == lVar24) {
              plVar13 = plVar1 + lVar23 * 4;
              goto LAB_001a6454;
            }
            if ((int)plVar13[1] == (int)*plVar28) goto LAB_001a6454;
            if (((*plVar13 != 0) || (plVar32 = plVar33, plVar35 = plVar13, plVar36 != (long *)0x0))
               && ((plVar32 = plVar13, plVar35 = plVar36, plVar33 != (long *)0x0 &&
                   (*(uint *)((long)plVar33 + 0x14) <= *(uint *)((long)plVar13 + 0x14))))) {
              plVar32 = plVar33;
            }
            lVar23 = lVar23 + 1;
            plVar13 = plVar13 + 4;
            lVar26 = lVar26 + -1;
            plVar33 = plVar32;
            plVar36 = plVar35;
          } while (lVar26 != 0);
          plVar13 = (long *)0x0;
LAB_001a6454:
          if (plVar36 != (long *)0x0) {
            plVar33 = plVar36;
          }
          if (plVar13 != (long *)0x0) {
            plVar33 = plVar13;
          }
          if (plVar33 != (long *)0x0) {
            lVar22 = lVar22 + uVar17 * 0x28;
            uVar18 = *(undefined8 *)(lVar22 + 0x18);
            uVar46 = *(undefined8 *)(lVar22 + 0x20);
            lVar22 = *plVar28;
            *plVar33 = lVar24;
            *(int *)((long)plVar33 + 0x14) = iVar12;
            *(undefined8 *)((long)plVar33 + 0xc) = uVar18;
            uVar46 = NEON_scvtf(uVar46,4);
            *(int *)(plVar33 + 1) = (int)lVar22;
            plVar33[3] = CONCAT44((float)((ulong)uVar46 >> 0x20) / 300.0,(float)uVar46 / 300.0);
            uVar29 = (ulong)(uint)param_2[0x20];
          }
        }
        uVar17 = uVar17 + 1;
      } while (uVar17 < uVar29);
    }
    memcpy(param_1 + 4,puVar14,0xef58);
    uVar16 = 1;
    goto LAB_001a5994;
  }
  goto LAB_001a58ec;
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

/* ===== FUN_001a88a4 @ 001a88a4 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_001a88a4(undefined8 *param_1)

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

/* ===== FUN_001a8be8 @ 001a8be8 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_001a8be8(long param_1,long param_2)

{
  uint uVar1;
  uint uVar2;
  ulong uVar3;
  long lVar4;
  long lVar5;
  undefined1 auVar6 [16];
  undefined1 auVar7 [16];
  undefined1 auVar8 [16];
  undefined8 local_a0;
  undefined8 uStack_98;
  undefined8 uStack_90;
  undefined8 uStack_88;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined8 uStack_70;
  undefined8 uStack_68;
  undefined8 local_60;
  undefined8 uStack_58;
  undefined8 uStack_50;
  undefined8 uStack_48;
  undefined8 local_40;
  undefined8 uStack_38;
  undefined8 uStack_30;
  undefined8 uStack_28;
  
  uVar3 = *(ulong *)(param_1 + 0x20);
  lVar5 = uVar3 << 3;
  lVar4 = 0x38;
  if (0x37 < *(ulong *)(param_1 + 0x28)) {
    lVar4 = 0x78;
  }
  auVar7._8_8_ = lVar5;
  auVar7._0_8_ = lVar5;
  lVar4 = lVar4 - *(ulong *)(param_1 + 0x28);
  auVar8._8_8_ = _UNK_00112bc8;
  auVar8._0_8_ = _DAT_00112bc0;
  auVar6 = NEON_ushl(auVar7,_DAT_00112990,8);
  uStack_98 = 0;
  uStack_88 = 0;
  uStack_90 = 0;
  uStack_78 = 0;
  local_80 = 0;
  uStack_68 = 0;
  uStack_70 = 0;
  auVar8 = NEON_ushl(auVar7,auVar8,8);
  uStack_58 = 0;
  local_60 = 0;
  uStack_48 = 0;
  uStack_50 = 0;
  uStack_38 = 0;
  local_40 = 0;
  uStack_28 = 0;
  uStack_30 = 0;
  local_a0 = 0x80;
  *(char *)((long)&local_a0 + lVar4 + 4) = (char)(uVar3 >> 0x15);
  *(char *)((long)&local_a0 + lVar4 + 7) = (char)lVar5;
  *(char *)((long)&local_a0 + lVar4 + 5) = (char)(uVar3 >> 0xd);
  *(char *)((long)&local_a0 + lVar4 + 6) = (char)(uVar3 >> 5);
  *(uint *)((long)&local_a0 + lVar4) =
       CONCAT13(auVar8[8],CONCAT12(auVar8[0],CONCAT11(auVar6[8],auVar6[0])));
  FUN_001a88c8(param_1,&local_a0,lVar4 + 8);
  uVar2 = 0;
  uVar3 = 0;
  do {
    uVar1 = uVar2 ^ 0xffffffff;
    uVar2 = uVar2 + 8;
    *(char *)(param_2 + uVar3) =
         (char)(*(uint *)(param_1 + (uVar3 & 0xfffffffc)) >> (ulong)(uVar1 & 0x18));
    uVar3 = uVar3 + 1;
  } while (uVar3 != 0x20);
  return;
}

/* ===== FUN_001a8f38 @ 001a8f38 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
FUN_001a8f38(long param_1,long param_2,uint param_3,long param_4,int param_5,uint param_6,
            int param_7)

{
  long *plVar1;
  int iVar2;
  undefined1 auVar3 [12];
  undefined1 auVar4 [16];
  long lVar5;
  undefined4 uVar6;
  ulong uVar7;
  ulong uVar8;
  int iVar9;
  uint uVar10;
  long *plVar11;
  long lVar12;
  float fVar13;
  undefined8 uVar14;
  float fVar15;
  undefined8 uVar16;
  undefined1 auVar17 [16];
  undefined1 auVar18 [16];
  
  uVar6 = 0;
  if (param_1 != 0) {
    uVar8 = (ulong)param_3;
    *(undefined4 *)(param_1 + 0x10) = 0;
    *(undefined4 *)(param_1 + 0x20) = 0;
    auVar4 = _DAT_00112810;
    if (((uVar8 < 0x101) && (uVar6 = 0, uVar8 == 0 || param_2 != 0)) && (param_4 != 0)) {
      if (uVar8 != 0) {
        uVar7 = 0;
        do {
          plVar11 = (long *)(param_2 + uVar7 * 0x30);
          lVar5 = *plVar11;
          if (lVar5 == 0) {
            return 0;
          }
          if (lVar5 != param_4) {
            uVar14 = NEON_scvtf(*(undefined8 *)(param_2 + uVar7 * 0x30 + 0x28),4);
            fVar13 = (float)uVar14 / 300.0;
            fVar15 = (float)((ulong)uVar14 >> 0x20) / 300.0;
            uVar14 = CONCAT44(fVar15,fVar13);
            auVar17._8_8_ = uVar14;
            auVar17._0_8_ = uVar14;
            auVar18 = NEON_fcmgt(auVar4,auVar17,4);
            auVar17 = NEON_fcmgt(auVar17,auVar4,4);
            auVar3._2_10_ = auVar17._6_10_;
            auVar3._0_2_ = auVar17._8_2_;
            if (((((auVar18._0_2_ & 1 | (auVar18._4_2_ & 1) << 1 |
                    ((uint)(CONCAT26(auVar17._12_2_,auVar3._0_6_ << 0x20) >> 0x20) & 1) << 2 |
                   (uint)auVar17._12_2_ << 3) ^ 0xffffffff) & 0xf) == 0) &&
               (uVar16 = NEON_fcmgt(0x3f0000003f000000,CONCAT44(ABS(fVar15),ABS(fVar13)),4),
               ((uint)uVar16 & (uint)((ulong)uVar16 >> 0x20) & 1) == 0)) {
              iVar9 = *(int *)(param_2 + uVar7 * 0x30 + 0x18);
              if (iVar9 - 1000000U < 1000000) {
                lVar5 = 0x78;
                do {
                  plVar1 = (long *)(param_1 + lVar5);
                  if (*plVar1 == *plVar11) {
                    plVar1[1] = 0;
                    *plVar1 = 0;
                    plVar1[3] = 0;
                    plVar1[2] = 0;
                  }
                  lVar5 = lVar5 + 0x20;
                } while (lVar5 != 0x378);
              }
              else if (*(int *)(param_2 + uVar7 * 0x30 + 0x1c) + 0xfefc99c0U < 1000000) {
                if ((param_6 != 0xffffffff) && (param_5 != -1)) {
                  lVar12 = param_2 + uVar7 * 0x30;
                  iVar2 = *(int *)(lVar12 + 0x20);
                  if (((iVar2 != -1) && (uVar10 = *(uint *)(lVar12 + 0x24), uVar10 != 0xffffffff))
                     && ((((uVar10 == param_6 && (param_6 < 9)) && (uVar10 < 9)) ||
                         ((((param_6 != 0 || param_5 != 0 && (uVar10 != 0 || iVar2 != 0)) &&
                           (iVar2 == param_5)) && (uVar10 == param_6)))))) goto LAB_001a8fec;
                }
                if (*(long *)(param_1 + 0x78) == lVar5) {
                  uVar10 = 0;
                }
                else if (*(long *)(param_1 + 0x98) == lVar5) {
                  uVar10 = 1;
                }
                else if (*(long *)(param_1 + 0xb8) == lVar5) {
                  uVar10 = 2;
                }
                else if (*(long *)(param_1 + 0xd8) == lVar5) {
                  uVar10 = 3;
                }
                else if (*(long *)(param_1 + 0xf8) == lVar5) {
                  uVar10 = 4;
                }
                else if (*(long *)(param_1 + 0x118) == lVar5) {
                  uVar10 = 5;
                }
                else if (*(long *)(param_1 + 0x138) == lVar5) {
                  uVar10 = 6;
                }
                else if (*(long *)(param_1 + 0x158) == lVar5) {
                  uVar10 = 7;
                }
                else if (*(long *)(param_1 + 0x178) == lVar5) {
                  uVar10 = 8;
                }
                else if (*(long *)(param_1 + 0x198) == lVar5) {
                  uVar10 = 9;
                }
                else if (*(long *)(param_1 + 0x1b8) == lVar5) {
                  uVar10 = 10;
                }
                else if (*(long *)(param_1 + 0x1d8) == lVar5) {
                  uVar10 = 0xb;
                }
                else if (*(long *)(param_1 + 0x1f8) == lVar5) {
                  uVar10 = 0xc;
                }
                else if (*(long *)(param_1 + 0x218) == lVar5) {
                  uVar10 = 0xd;
                }
                else if (*(long *)(param_1 + 0x238) == lVar5) {
                  uVar10 = 0xe;
                }
                else if (*(long *)(param_1 + 600) == lVar5) {
                  uVar10 = 0xf;
                }
                else if (*(long *)(param_1 + 0x278) == lVar5) {
                  uVar10 = 0x10;
                }
                else if (*(long *)(param_1 + 0x298) == lVar5) {
                  uVar10 = 0x11;
                }
                else if (*(long *)(param_1 + 0x2b8) == lVar5) {
                  uVar10 = 0x12;
                }
                else if (*(long *)(param_1 + 0x2d8) == lVar5) {
                  uVar10 = 0x13;
                }
                else if (*(long *)(param_1 + 0x2f8) == lVar5) {
                  uVar10 = 0x14;
                }
                else if (*(long *)(param_1 + 0x318) == lVar5) {
                  uVar10 = 0x15;
                }
                else if (*(long *)(param_1 + 0x338) == lVar5) {
                  uVar10 = 0x16;
                }
                else if (*(long *)(param_1 + 0x358) == lVar5) {
                  uVar10 = 0x17;
                }
                else {
                  uVar10 = *(uint *)(param_1 + 0x378);
                  *(uint *)(param_1 + 0x378) = uVar10 + 1;
                  uVar10 = uVar10 % 0x18;
                }
                lVar12 = param_1 + (ulong)uVar10 * 0x20;
                *(long *)(lVar12 + 0x78) = lVar5;
                *(int *)(lVar12 + 0x80) = iVar9;
                *(int *)(lVar12 + 0x84) = param_7;
                *(undefined8 *)(lVar12 + 0x88) = uVar14;
                *(undefined4 *)(lVar12 + 0x90) = 0x4029999a;
              }
            }
          }
LAB_001a8fec:
          uVar7 = uVar7 + 1;
        } while (uVar7 != uVar8);
      }
      lVar5 = 0;
      iVar9 = 0;
      do {
        if ((*(long *)(param_1 + lVar5 + 0x78) != 0) &&
           ((uint)(param_7 - *(int *)(param_1 + lVar5 + 0x84)) < 10)) {
          iVar9 = iVar9 + 1;
        }
        lVar5 = lVar5 + 0x20;
      } while (lVar5 != 0x300);
      uVar6 = 1;
      *(int *)(param_1 + 0x18) = iVar9;
      *(undefined4 *)(param_1 + 0x10) = 1;
      iVar2 = 0;
      if (0x17 < *(uint *)(param_1 + 0x378)) {
        iVar2 = *(uint *)(param_1 + 0x378) - 0x18;
      }
      *(uint *)(param_1 + 0x28) = param_3;
      *(int *)(param_1 + 0x2c) = iVar2;
      *(uint *)(param_1 + 0x20) = (uint)(iVar9 == 0);
      *(int *)(param_1 + 0x24) = param_7;
    }
  }
  return uVar6;
}

/* ===== FUN_001a95b0 @ 001a95b0 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Restarted to delay deadcode elimination for space: stack */

undefined8 FUN_001a95b0(int *param_1,int *param_2,undefined8 *param_3)

{
  byte *pbVar1;
  int *piVar2;
  byte bVar3;
  uint uVar4;
  char cVar5;
  bool bVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  bool bVar9;
  int iVar10;
  int iVar11;
  long lVar12;
  int *piVar13;
  long lVar14;
  long lVar15;
  long lVar16;
  long *plVar17;
  uint uVar18;
  int *piVar19;
  ulong uVar20;
  int *piVar21;
  ulong uVar22;
  int *piVar23;
  int *piVar24;
  undefined8 uVar25;
  code *pcVar26;
  int *piVar27;
  uint *puVar28;
  int *piVar29;
  float fVar30;
  float fVar31;
  float fVar32;
  undefined8 uVar33;
  float fVar35;
  float fVar36;
  undefined1 auVar34 [16];
  float fVar37;
  float fVar38;
  float fVar39;
  undefined8 uVar40;
  float fVar41;
  undefined1 auStack_5490 [80];
  undefined8 local_5440;
  undefined8 uStack_5438;
  undefined8 uStack_5428;
  undefined4 local_5420;
  uint uStack_541c;
  int local_5418;
  undefined8 local_5408;
  int local_5400;
  uint uStack_53fc;
  int iStack_53f8;
  int iStack_53f4;
  int local_53f0;
  uint uStack_53ec;
  long lStack_53e8;
  int local_53e0;
  int local_53dc;
  undefined8 local_53d8;
  undefined4 local_53d0;
  undefined8 uStack_53c8;
  int local_53bc;
  long local_53b8;
  long local_53b0;
  long lStack_53a8;
  float local_53a0;
  float local_539c;
  float local_5398;
  float local_5394;
  undefined1 auStack_5390 [8];
  int local_5388;
  uint local_537c;
  undefined4 local_5378;
  ulong local_5370;
  uint local_5340;
  int local_533c;
  uint local_5338;
  int local_5334;
  undefined8 local_5324;
  int local_5318;
  int local_5314;
  int local_5310;
  float local_530c;
  int local_52d4;
  undefined4 local_5228;
  uint local_5224;
  undefined4 local_5218;
  undefined8 local_51ec;
  float local_51e4;
  float local_51e0;
  undefined8 local_1a8;
  undefined8 uStack_1a0;
  undefined8 local_198;
  undefined8 uStack_190;
  undefined8 uStack_188;
  long local_180;
  undefined8 local_178;
  undefined8 uStack_170;
  undefined8 uStack_168;
  undefined8 uStack_160;
  undefined8 local_158;
  undefined8 uStack_150;
  undefined8 uStack_148;
  undefined8 uStack_140;
  undefined8 local_138;
  undefined8 uStack_130;
  undefined8 local_128;
  undefined8 uStack_120;
  undefined8 uStack_118;
  undefined8 uStack_110;
  undefined8 local_108;
  undefined8 local_100;
  int local_f8;
  uint local_f4;
  undefined8 local_f0;
  ulong local_e8;
  undefined8 local_e0;
  undefined8 local_d8;
  undefined8 local_d0;
  ulong local_c8;
  ulong local_c0;
  ulong uStack_b8;
  undefined4 local_b0;
  undefined8 local_ac;
  
  if (((ulong)param_1 & 7) != 0) {
    return 0;
  }
  if (param_1 == (int *)0x0) {
    return 0;
  }
  if (param_3 == (undefined8 *)0x0) {
    return 0;
  }
  if (*param_1 != 0x4e444132) {
    return 0;
  }
  param_3[0xb] = 0;
  param_3[2] = 0;
  param_3[1] = 0;
  param_3[4] = 0;
  param_3[3] = 0;
  param_3[10] = 0;
  param_3[9] = 0;
  param_3[8] = 0;
  param_3[7] = 0;
  param_3[6] = 0;
  param_3[5] = 0;
  uVar7 = DAT_0010e720;
  pbVar1 = (byte *)(param_1 + 1);
  *(undefined4 *)(param_3 + 1) = 1;
  *param_3 = uVar7;
  *(undefined4 *)(param_3 + 3) = 1;
  do {
    bVar3 = *pbVar1;
    cVar5 = '\x01';
    bVar6 = (bool)ExclusiveMonitorPass(pbVar1,0x10);
    if (bVar6) {
      *pbVar1 = 1;
      cVar5 = ExclusiveMonitorsStatus();
    }
  } while (cVar5 != '\0');
  if ((bVar3 & 1) != 0) {
    *(undefined4 *)((long)param_3 + 0xc) = 6;
    return 0;
  }
  if ((((((param_2 == (int *)0x0) || (*param_2 != 2)) || (param_2[1] != 0x60)) ||
       ((piVar21 = *(int **)(param_2 + 2), piVar21 == (int *)0x0 ||
        (piVar19 = *(int **)(param_2 + 6), piVar19 == (int *)0x0)))) ||
      ((piVar29 = *(int **)(param_2 + 4), piVar29 == (int *)0x0 ||
       ((piVar24 = *(int **)(param_2 + 0xc), piVar24 == (int *)0x0 ||
        (piVar27 = *(int **)(param_2 + 0xe), piVar27 == (int *)0x0)))))) ||
     ((*(long *)(param_2 + 10) == 0 ||
      ((((pcVar26 = *(code **)(param_2 + 0x16), pcVar26 == (code *)0x0 ||
         (*(long *)(param_2 + 0x12) == 0)) || (*piVar21 != 1)) ||
       (((piVar21[1] != 0xa8 || (*piVar19 != 1)) ||
        ((piVar19[1] != 0x48 || ((*piVar29 != 1 || (piVar29[1] != 0x38))))))))))))
  goto LAB_001a9c28;
  lVar14 = *(long *)(piVar21 + 8);
  param_3[4] = lVar14;
  uVar20 = *(ulong *)(piVar21 + 6);
  param_3[5] = uVar20;
  lVar15 = *(long *)(piVar21 + 10);
  param_3[6] = lVar15;
  iVar11 = piVar21[3];
  *(int *)((long)param_3 + 0x1c) = iVar11;
  if ((uVar20 == 0) || (uVar20 <= *(ulong *)(param_1 + 0x14aa))) goto LAB_001aa7f0;
  *(ulong *)(param_1 + 0x14aa) = uVar20;
  if (piVar19[2] == 0) {
    iVar10 = 1;
    goto LAB_001a9c2c;
  }
  if (piVar19[2] == 1) {
    piVar2 = piVar19 + 8;
    iVar10 = FUN_001aa868();
    if (iVar10 == 0) goto LAB_001a9c28;
    if (piVar19[3] - 6U < 0xfffffffb) {
      iVar10 = 9;
    }
    else if (((piVar29[2] == 1) && (piVar21[2] == 1)) && (piVar29[3] != 0)) {
      iVar10 = 3;
      if (((*(ulong *)(piVar29 + 8) == uVar20) && (piVar29[3] == iVar11)) &&
         ((lVar16 = *(long *)(piVar29 + 10), lVar16 == *(long *)(piVar21 + 0xe) &&
          ((lVar12 = *(long *)(piVar29 + 0xc), lVar12 == *(long *)(piVar21 + 0x12) &&
           (piVar29[4] != 0)))))) {
        if ((piVar21[0x20] == 1) &&
           (((((piVar21[0x1b] == 1 && (piVar21[0x1c] == 1)) && (piVar21[0x1d] == 0)) &&
             ((piVar21[0x1e] == 1 && (piVar21[0x1f] == 1)))) && (piVar21[0x21] == 0)))) {
          uVar4 = piVar19[4];
          if ((1 < uVar4) || (uVar4 != piVar21[0x26])) goto LAB_001a9c28;
          if (uVar4 == 0) {
LAB_001a9928:
            if (lVar14 == 0) goto LAB_001a9c28;
            iVar10 = 2;
            if ((((*(long *)(piVar19 + 6) != 0) && (*(long *)(piVar19 + 6) == lVar15)) &&
                ((*(long *)(piVar21 + 0xc) != 0 &&
                 (((lVar16 != 0 && (*(long *)(piVar21 + 0x10) != 0)) && (lVar12 != 0)))))) &&
               (((*(long *)(piVar21 + 0x14) != 0 && (*(long *)(piVar21 + 0x16) != 0)) &&
                ((0 < piVar21[0x18] &&
                 (((uVar4 = piVar21[0x19], -1 < (int)uVar4 && ((uint)piVar21[0x1a] >> 5 < 0x753)) &&
                  (uVar4 >> 5 < 0x753)))))))) {
              if (((((((*piVar24 == 1) && (piVar24[1] == 0x88)) && (piVar24[2] == 1)) &&
                    ((piVar24[3] == 1 && (piVar24[4] == 1)))) &&
                   ((((piVar24[5] == 1 &&
                      ((piVar23 = *(int **)(piVar24 + 6), piVar23 != (int *)0x0 && (*piVar23 == 1)))
                      ) && (piVar23[1] == 0x38)) &&
                    (((((piVar23[2] == 1 && (*(ulong *)(piVar23 + 8) == uVar20)) &&
                       (piVar23[3] == iVar11)) &&
                      ((piVar23[4] == piVar29[4] && (*(long *)(piVar23 + 10) == lVar16)))) &&
                     ((*(long *)(piVar23 + 0xc) == lVar12 &&
                      ((*(long *)(piVar24 + 8) == lVar14 && (*(long *)(piVar24 + 10) == lVar15))))))
                    )))) && ((*(long *)(piVar24 + 0xc) == lVar16 &&
                             ((((*(long *)(piVar24 + 0xe) == *(long *)(piVar21 + 0x10) &&
                                (*(long *)(piVar24 + 0x10) == lVar12)) &&
                               (*(long *)(piVar24 + 0x12) == *(long *)(piVar21 + 0x14))) &&
                              ((piVar24[0x14] == piVar21[0x18] && (piVar24[0x18] == uVar4)))))))) &&
                 (((piVar24[0x19] == piVar21[0x1a] && (uVar4 = piVar24[0x1c], uVar4 < 0x101)) &&
                  ((((((*(long *)(piVar24 + 0x1a) != 0 || (uVar4 == 0)) &&
                      ((uint)piVar24[0x20] < 0x31)) &&
                     (((((*(long *)(piVar24 + 0x1e) != 0 || (piVar24[0x20] == 0)) && (*piVar27 == 1)
                        ) && ((piVar27[1] == 0x38 && (piVar27[2] == 1)))) &&
                      ((piVar27[3] == 0 && ((piVar27[5] == 0 && (piVar27[4] == uVar4)))))))) &&
                    (*(ulong *)(piVar27 + 6) == uVar20)) &&
                   (((piVar27[0xc] == iVar11 && (*(long *)(piVar27 + 8) == lVar14)) &&
                    (*(long *)(piVar27 + 10) == lVar15)))))))) {
                piVar27 = *(int **)(param_2 + 8);
                if ((((piVar27 == (int *)0x0) || (*piVar27 != 1)) ||
                    (((piVar27[1] != 0x58 ||
                      ((piVar27[2] != 1 || (*(ulong *)(piVar27 + 6) != uVar20)))) ||
                     (*(long *)(piVar27 + 8) != lVar14)))) ||
                   ((*(long *)(piVar27 + 10) != lVar15 || (0xf < (uint)piVar27[3]))))
                goto LAB_001a9c20;
                iVar10 = 3;
                if ((((ABS((float)piVar27[0xe]) != INFINITY) && (!NAN(ABS((float)piVar27[0xe])))) &&
                    (ABS((float)piVar27[0xf]) != INFINITY)) && (!NAN(ABS((float)piVar27[0xf])))) {
                  iVar11 = (*pcVar26)(*(undefined8 *)(param_2 + 0x14),piVar21,piVar19);
                  if (iVar11 == 1) {
                    memcpy(auStack_5390,param_1 + 2,0x52f0);
                    iVar11 = FUN_001aa8d8(&local_1a8,piVar21);
                    if (((iVar11 == 0) || ((int)local_100 != piVar29[4])) ||
                       ((local_180 != *(long *)(piVar21 + 10) || (local_5224 != piVar19[3])))) {
                      memset(auStack_5390,0,0x52f0);
                      local_d0 = DAT_0010e6f0;
                      FUN_001aaaa4(auStack_5390);
                    }
                    uVar8 = _UNK_00112a18;
                    uVar25 = _DAT_00112a10;
                    uVar7 = DAT_0010e6e0;
                    if ((local_f8 == 0) ||
                       ((local_100._4_4_ <= (uint)piVar29[3] &&
                        (local_e8 <= *(ulong *)(piVar21 + 0xc))))) {
                      uVar33 = NEON_scvtf(*(undefined8 *)(piVar21 + 0x19),4);
                      fVar30 = (float)uVar33 / 300.0;
                      fVar35 = (float)((ulong)uVar33 >> 0x20) / 300.0;
                      fVar32 = (float)local_e0;
                      fVar37 = (float)((ulong)local_e0 >> 0x20);
                      if (local_f8 == 0) {
                        bVar6 = false;
                        bVar9 = false;
                        if (piVar19[3] == 5) goto LAB_001a9e74;
                      }
                      else {
                        bVar9 = piVar29[3] != local_100._4_4_;
                        if (piVar19[3] == 5) {
LAB_001a9e74:
                          bVar6 = bVar9;
                          uVar20 = *(ulong *)(piVar21 + 0xc);
                          if ((local_c8 - 1 < uVar20) && (uVar20 - local_c8 < 0xfb)) {
                            if (local_c8 < uVar20) {
                              fVar31 = fVar30 - (float)local_ac;
                              fVar36 = fVar35 - (float)((ulong)local_ac >> 0x20);
                              fVar38 = fVar36 * fVar36 + fVar31 * fVar31;
                              if (fVar38 == 1e-07 || fVar38 < 1e-07 != NAN(fVar38)) {
                                if ((local_c0 == 0) || (0x43 < uVar20 - local_c0)) {
                                  local_d8 = 0;
                                  local_ac = CONCAT44(fVar35,fVar30);
                                  local_b0 = 1;
                                  local_c8 = uVar20;
                                  uStack_b8 = uVar20;
                                }
                              }
                              else {
                                fVar38 = 1000.0 / (float)(uVar20 - local_c8);
                                fVar31 = fVar31 * fVar38;
                                fVar36 = fVar36 * fVar38;
                                local_d8 = CONCAT44(fVar36,fVar31);
                                uVar40 = NEON_fcmgt(CONCAT44(ABS(fVar36),ABS(fVar31)),
                                                    0x7f8000007f800000,4);
                                uVar33 = NEON_fcmgt(0x7f8000007f800000,
                                                    CONCAT44(ABS(fVar36),ABS(fVar31)),4);
                                if ((((~((byte)uVar33 | (byte)uVar40) |
                                      ~((byte)((ulong)uVar33 >> 0x20) |
                                       (byte)((ulong)uVar40 >> 0x20))) & 1) != 0) ||
                                   (196.0 < fVar36 * fVar36 + fVar31 * fVar31)) {
                                  local_d8 = 0;
                                  local_b0 = 0;
                                  uStack_b8 = 0;
                                }
                                else {
                                  local_b0 = 1;
                                  uStack_b8 = uVar20;
                                }
                                local_ac = CONCAT44(fVar35,fVar30);
                                local_c8 = uVar20;
                                local_c0 = uVar20;
                              }
                            }
                          }
                          else {
                            local_ac = CONCAT44(fVar35,fVar30);
                            local_b0 = 0;
                            uStack_b8 = 0;
                            local_c0 = 0;
                            local_d8 = 0;
                            local_c8 = uVar20;
                          }
                          uVar22 = uVar20 - local_e8;
                          if (99 < uVar22) {
                            uVar22 = 100;
                          }
                          local_5388 = (int)uVar22 + 0x18;
                          if (uVar20 <= local_e8 - 1) {
                            local_5388 = 0x39;
                          }
                        }
                        else if (piVar29[3] == local_100._4_4_) {
                          bVar6 = false;
                        }
                        else {
                          uVar33 = NEON_fmov(0x41f00000,4);
                          fVar31 = (fVar30 - fVar32) * (float)uVar33;
                          fVar36 = (fVar35 - fVar37) * (float)((ulong)uVar33 >> 0x20);
                          if (fVar36 * fVar36 + fVar31 * fVar31 <= 81.0) {
                            bVar6 = true;
                            local_d8 = CONCAT44(fVar36,fVar31);
                          }
                          else {
                            bVar6 = true;
                          }
                        }
                      }
                      uStack_190 = *(undefined8 *)(piVar21 + 6);
                      local_198 = *(undefined8 *)(piVar21 + 4);
                      uStack_188 = *(undefined8 *)(piVar21 + 8);
                      local_180 = *(long *)(piVar21 + 10);
                      uStack_1a0 = *(undefined8 *)(piVar21 + 2);
                      local_1a8 = *(undefined8 *)piVar21;
                      uStack_150 = *(undefined8 *)(piVar21 + 0x16);
                      local_158 = *(undefined8 *)(piVar21 + 0x14);
                      uStack_148 = *(undefined8 *)(piVar21 + 0x18);
                      uStack_140 = *(undefined8 *)(piVar21 + 0x1a);
                      uStack_170 = *(undefined8 *)(piVar21 + 0xe);
                      local_178 = *(undefined8 *)(piVar21 + 0xc);
                      uStack_160 = *(undefined8 *)(piVar21 + 0x12);
                      uStack_168 = *(undefined8 *)(piVar21 + 0x10);
                      uStack_120 = *(undefined8 *)(piVar21 + 0x22);
                      local_128 = *(undefined8 *)(piVar21 + 0x20);
                      uStack_118 = *(undefined8 *)(piVar21 + 0x24);
                      uStack_110 = *(undefined8 *)(piVar21 + 0x26);
                      local_108 = *(undefined8 *)(piVar21 + 0x28);
                      uStack_130 = *(undefined8 *)(piVar21 + 0x1e);
                      local_138 = *(undefined8 *)(piVar21 + 0x1c);
                      local_100 = NEON_rev64(*(undefined8 *)(piVar29 + 3),4);
                      local_e0 = CONCAT44(fVar35,fVar30);
                      local_e8 = *(ulong *)(piVar21 + 0xc);
                      local_f8 = 1;
                      local_5228 = (undefined4)*(undefined8 *)(piVar29 + 3);
                      local_5310 = piVar27[3];
                      local_5318 = piVar27[0xc];
                      fVar31 = (float)local_d8;
                      fVar36 = local_d8._4_4_;
                      if (local_5310 == 0) {
                        local_5314 = piVar27[0xd];
                      }
                      else {
                        fVar38 = (float)local_5318 / 300.0;
                        local_5314 = piVar27[0xd];
                        if ((((fVar38 != -200.0 && fVar38 < -200.0 == NAN(fVar38)) &&
                             (fVar38 < 200.0)) &&
                            (fVar39 = (float)local_5314 / 300.0,
                            fVar39 != -200.0 && fVar39 < -200.0 == NAN(fVar39))) && (fVar39 < 200.0)
                           ) {
                          fVar38 = fVar38 - fVar30;
                          fVar39 = fVar39 - fVar35;
                          fVar41 = (float)NEON_fmadd(fVar38,fVar38,fVar39 * fVar39);
                          if ((fVar41 != 0.0004 && fVar41 < 0.0004 == NAN(fVar41)) &&
                             (fVar41 < 36.0)) {
                            fVar31 = fVar38 * (1.0 / SQRT(fVar41)) * local_d0._4_4_;
                            fVar36 = fVar39 * (1.0 / SQRT(fVar41)) * local_d0._4_4_;
                          }
                        }
                      }
                      uVar4 = piVar19[3];
                      local_52d4 = piVar19[5];
                      if (piVar27[4] != 1) {
                        local_5310 = 0;
                      }
                      local_5324 = *(undefined8 *)(piVar27 + 0xe);
                      if ((2 < uVar4) && (local_5218 = 0, uVar4 == 5)) {
                        local_51ec = CONCAT44(fVar35,fVar30);
                        local_5378 = local_b0;
                        local_5370 = uStack_b8;
                        local_51e4 = (float)local_d8;
                        local_51e0 = local_d8._4_4_;
                      }
                      local_5224 = uVar4;
                      if (piVar24[0x1c] != 0) {
                        uVar20 = 0;
                        do {
                          lVar14 = *(long *)(piVar24 + 0x1a);
                          local_5408 = uVar7;
                          plVar17 = (long *)(lVar14 + uVar20 * 0x58);
                          lStack_53e8 = 0;
                          uStack_53ec = 0;
                          local_53f0 = 0;
                          local_53d8 = 0;
                          local_53dc = 0;
                          local_53e0 = 0;
                          uStack_53c8 = 0;
                          local_53d0 = 0;
                          local_53b8 = 0;
                          local_53bc = 0;
                          lStack_53a8 = 0;
                          local_53b0 = 0;
                          local_5394 = 0.0;
                          local_5398 = 0.0;
                          local_539c = 0.0;
                          local_53a0 = 0.0;
                          iStack_53f4 = 0;
                          iStack_53f8 = 0;
                          uStack_53fc = 0;
                          local_5400 = 0;
                          if ((((*(int *)((long)plVar17 + 0x14) != 0x3f) ||
                               (puVar28 = (uint *)(lVar14 + uVar20 * 0x58 + 0x3c), 1 < *puVar28)) ||
                              (lVar15 = *plVar17, lVar15 == 0)) ||
                             (*(long *)(lVar14 + uVar20 * 0x58 + 8) == 0)) {
LAB_001aa63c:
                            iVar10 = 10;
                            goto LAB_001a9c2c;
                          }
                          piVar23 = (int *)(lVar14 + uVar20 * 0x58 + 0x10);
                          iVar11 = *piVar23;
                          if (iVar11 == 0) goto LAB_001aa63c;
                          if (uVar20 != 0) {
                            piVar13 = (int *)(*(long *)(piVar24 + 0x1a) + 0x10);
                            uVar22 = uVar20;
                            do {
                              if ((*(long *)(piVar13 + -4) == lVar15) || (*piVar13 == iVar11)) {
                                iVar10 = 2;
                                goto LAB_001a9c2c;
                              }
                              piVar13 = piVar13 + 0x16;
                              uVar22 = uVar22 - 1;
                            } while (uVar22 != 0);
                          }
                          iVar11 = (**(code **)(param_2 + 0x12))
                                             (*(undefined8 *)(param_2 + 0x10),
                                              *(undefined8 *)(piVar29 + 8),
                                              *(undefined8 *)(piVar21 + 8),
                                              *(undefined8 *)(piVar19 + 6),lVar15,iVar11,&local_5408
                                             );
                          iVar10 = 10;
                          if (((((iVar11 == 0) || ((int)local_5408 != 1)) ||
                               ((local_5408._4_4_ != 0x78 ||
                                (((local_5400 != 1 || (1 < uStack_53fc)) ||
                                 (uStack_53fc != (iStack_53f8 == 0))))))) ||
                              ((lStack_53e8 != *plVar17 || (local_53e0 != *piVar23)))) ||
                             ((((local_53b8 != *(long *)(piVar29 + 8) ||
                                (((local_53dc != piVar29[3] ||
                                  (local_53b0 != *(long *)(piVar21 + 8))) ||
                                 ((lStack_53a8 != *(long *)(piVar19 + 6) ||
                                  (((((iStack_53f4 != *(int *)(lVar14 + uVar20 * 0x58 + 0x1c) ||
                                      (uStack_53ec != *puVar28)) ||
                                     (local_53bc != *(int *)(lVar14 + uVar20 * 0x58 + 0x50))) ||
                                    ((local_53a0 !=
                                      (float)*(int *)(lVar14 + uVar20 * 0x58 + 0x28) / 300.0 ||
                                     (local_539c !=
                                      (float)*(int *)(lVar14 + uVar20 * 0x58 + 0x2c) / 300.0)))) ||
                                   (ABS(local_5398) == INFINITY)))))))) ||
                               (((NAN(ABS(local_5398)) || (ABS(local_5394) == INFINITY)) ||
                                (NAN(ABS(local_5394)))))) || (local_53f0 != 0x5c))))
                          goto LAB_001a9c2c;
                          if (uStack_53fc != 0) {
                            iVar11 = FUN_001aa954(local_53a0,local_539c,CONCAT44(fVar35,fVar30),
                                                  fVar35,*(undefined8 *)(param_2 + 10));
                            if (iVar11 < 0) goto LAB_001aa624;
                            if (iVar11 != 0) {
                              uStack_5428 = NEON_rev64(local_53d8,4);
                              local_5420 = local_53d0;
                              uStack_541c = uStack_53ec;
                              local_5418 = local_53bc;
                              uStack_5438 = uVar8;
                              local_5440 = uVar25;
                              if (((uint)piVar19[3] < 6) &&
                                 ((1 << (ulong)(piVar19[3] & 0x1f) & 0x34U) != 0)) {
                                uVar4 = *(uint *)(lVar14 + uVar20 * 0x58 + 0x40);
                                if (1 < uVar4) goto LAB_001aa7f8;
                                iVar11 = *(int *)(lVar14 + uVar20 * 0x58 + 0x44);
                                if (uVar4 == 0) {
                                  lVar14 = lVar14 + uVar20 * 0x58;
                                  uVar18 = *(uint *)(lVar14 + 0x48);
                                  iVar10 = *(int *)(lVar14 + 0x4c);
                                }
                                else if (((iVar11 - 0x4e21U < 0xffffb1e0) ||
                                         (uVar18 = *(uint *)(lVar14 + uVar20 * 0x58 + 0x48),
                                         2000 < uVar18)) ||
                                        (iVar10 = *(int *)(lVar14 + uVar20 * 0x58 + 0x4c),
                                        iVar10 - 0x7d1U < 0xfffff830)) goto LAB_001aa7f8;
                                local_5340 = uVar4;
                                local_533c = iVar11;
                                local_5338 = uVar18;
                                local_5334 = iVar10;
                                iVar11 = FUN_001abc58(local_53a0,local_539c,local_5398,local_5394,
                                                      CONCAT44(fVar35,fVar30),fVar35,auStack_5390,
                                                      &local_5440,lStack_53e8,piVar2,piVar29[3],
                                                      *(undefined8 *)(piVar21 + 0xc));
                              }
                              else {
                                iVar11 = FUN_001b7968(local_53a0,local_539c,local_5398,local_5394,
                                                      CONCAT44(fVar35,fVar30),fVar35,fVar31,fVar36,
                                                      &local_5440,lStack_53e8,piVar2,piVar29[3],
                                                      auStack_5490);
                                if (iVar11 < 0) goto LAB_001a9c2c;
                                if (iVar11 == 0) goto LAB_001aa5f0;
                                iVar11 = FUN_001aab24(auStack_5390,auStack_5490);
                              }
                              if (iVar11 == 0) goto LAB_001aa7f8;
                            }
                          }
LAB_001aa5f0:
                          uVar20 = uVar20 + 1;
                        } while (uVar20 < (uint)piVar24[0x1c]);
                        uVar4 = piVar19[3];
                      }
                      if (uVar4 == 5) {
                        uVar18 = piVar21[5];
                        if (uVar18 - 0x7d1 < 0xfffff893) {
LAB_001aa624:
                          iVar10 = 3;
                          goto LAB_001a9c2c;
                        }
                        local_530c = (float)uVar18 / 300.0;
                        local_537c = uVar18;
                      }
                      else if ((bVar6) && (piVar27[3] != 0)) {
                        fVar36 = (float)piVar27[0xe];
                        fVar31 = (float)piVar27[0xf];
                        fVar38 = (float)NEON_fmadd(fVar36,fVar36,fVar31 * fVar31);
                        if (fVar38 != 100.0 && fVar38 < 100.0 == NAN(fVar38)) {
                          fVar37 = (float)NEON_fmadd(fVar36 * (fVar30 - fVar32),1.0 / SQRT(fVar38),
                                                     fVar31 * (fVar35 - fVar37) *
                                                     (1.0 / SQRT(fVar38)));
                          fVar32 = (float)local_d0;
                          if ((fVar37 != 0.06 && fVar37 < 0.06 == NAN(fVar37)) &&
                             (fVar37 < (float)local_d0 * 0.32 ==
                              (NAN(fVar37) || NAN((float)local_d0 * 0.32)))) {
                            fVar32 = (float)NEON_fmadd(fVar37 - (float)local_d0,0x3da3d70a,
                                                       (float)local_d0);
                    /* WARNING: Ignoring partial resolution of indirect */
                            local_d0._0_4_ = fVar32;
                          }
                          fVar32 = fVar32 * 30.0;
                          fVar37 = 5.0;
                          if ((5.0 <= fVar32) &&
                             (fVar37 = fVar32, fVar32 != 14.0 && fVar32 < 14.0 == NAN(fVar32))) {
                            fVar37 = 14.0;
                          }
                          NEON_fmadd(fVar37 - local_d0._4_4_,0x3d4ccccd,local_d0._4_4_);
                        }
                      }
                      auVar34._4_4_ = fVar35;
                      auVar34._0_4_ = fVar30;
                      auVar34._8_8_ = 0;
                      iVar11 = FUN_001ad4b4(auVar34,fVar35,(float)local_d8,local_d8._4_4_,
                                            auStack_5390,uVar4,piVar29[3],
                                            *(undefined8 *)(piVar21 + 0xc),piVar2,piVar27[4] == 1,
                                            *(undefined8 *)(param_2 + 10),param_3);
                      if (iVar11 == 0) {
LAB_001aa7f8:
                        iVar10 = 2;
                      }
                      else {
                        iVar10 = *(int *)((long)param_3 + 0xc);
                        if (iVar10 == 0) {
                          iVar11 = (**(code **)(param_2 + 0x16))
                                             (*(undefined8 *)(param_2 + 0x14),piVar21,piVar19);
                          if (iVar11 == 1) {
                            if (*(int *)(param_3 + 1) == 1) {
                              local_f4 = 0;
                            }
                            else if (*(int *)(param_3 + 1) == 2) {
                              local_f4 = 1;
                            }
                            else {
                              local_f4 = (uint)(piVar27[3] != 0);
                            }
                            local_f0 = *(undefined8 *)(piVar21 + 6);
                            memcpy(param_1 + 2,auStack_5390,0x52f0);
                            auVar34 = NEON_ext(*(undefined1 (*) [16])(piVar21 + 6),
                                               *(undefined1 (*) [16])(piVar21 + 6),8,1);
                            param_3[5] = auVar34._8_8_;
                            param_3[4] = auVar34._0_8_;
                            param_3[6] = *(undefined8 *)(piVar21 + 10);
                            *(int *)((long)param_3 + 0x1c) = piVar21[3];
                            goto LAB_001a9c68;
                          }
                          goto LAB_001aa7f0;
                        }
                      }
                      goto LAB_001a9c2c;
                    }
                  }
LAB_001aa7f0:
                  iVar10 = 5;
                }
              }
              else {
                iVar10 = 10;
              }
            }
          }
          else if (((piVar21[0x22] == 1) && (piVar21[0x27] == 1)) &&
                  (uVar22 = *(ulong *)(piVar21 + 0x24), uVar22 != 0)) {
            iVar10 = 8;
            if ((uVar22 <= *(ulong *)(piVar21 + 0xc)) && (*(ulong *)(piVar21 + 0xc) - uVar22 < 0xdd)
               ) goto LAB_001a9928;
          }
          else {
            iVar10 = 8;
          }
        }
        else {
          iVar10 = 7;
        }
      }
    }
    else {
LAB_001a9c20:
      iVar10 = 3;
    }
  }
  else {
LAB_001a9c28:
    iVar10 = 2;
  }
LAB_001a9c2c:
  uVar25 = *(undefined8 *)(param_1 + 0x14aa);
  memset(param_1 + 2,0,0x52f0);
  uVar7 = DAT_0010e6f0;
  *(undefined8 *)(param_1 + 0x14aa) = uVar25;
  *(undefined8 *)(param_1 + 0x14b2) = uVar7;
  FUN_001aaaa4(param_1 + 2);
  *(undefined4 *)(param_3 + 1) = 1;
  *(int *)((long)param_3 + 0xc) = iVar10;
  *(undefined4 *)(param_3 + 3) = 1;
LAB_001a9c68:
  param_1[1] = 0;
  return 1;
}

/* ===== FUN_001afaf4 @ 001afaf4 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_001afaf4(ulong *param_1)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  uint *puVar5;
  long lVar7;
  long lVar8;
  int iVar9;
  uint uVar10;
  uint uVar11;
  ulong uVar12;
  ulong uVar13;
  long lVar14;
  ulong *puVar15;
  undefined8 *puVar16;
  ulong uVar17;
  float fVar18;
  ulong uVar19;
  ulong uVar20;
  float fVar21;
  ulong uVar22;
  float fVar23;
  float fVar24;
  ulong uVar25;
  int iVar27;
  undefined8 uVar26;
  ulong uVar28;
  int iVar30;
  undefined8 uVar29;
  float fVar31;
  ulong uVar32;
  ulong uVar33;
  float fVar34;
  float fVar35;
  float fVar36;
  float fVar37;
  float fVar38;
  float fVar39;
  float fVar40;
  float fVar41;
  undefined4 uVar42;
  float fVar43;
  undefined4 uVar44;
  float fVar45;
  uint local_60 [2];
  undefined8 auStack_58 [11];
  uint *puVar6;
  
  puVar5 = local_60;
  puVar6 = local_60;
  uVar1 = (uint)param_1[0x2d];
  *(uint *)((long)param_1 + 0x17c) = uVar1;
  uVar12 = param_1[0x2f];
  iVar9 = (int)*(undefined8 *)((long)param_1 + 0x16c);
  uVar10 = 0xf;
  if (iVar9 != 2) {
    uVar10 = 8;
  }
  if ((uint)uVar12 == 0) {
    uVar11 = 0;
  }
  else {
    uVar13 = 0;
    uVar11 = 0;
    puVar15 = param_1 + 0x44;
    do {
      if (iVar9 == 5) {
        uVar17 = *puVar15;
        if ((uVar17 != 0) && ((uVar17 <= *param_1 && (*param_1 - uVar17 < 0x10c)))) {
LAB_001afb84:
          uVar17 = (ulong)uVar11;
          if (uVar13 != uVar17) {
            uVar19 = puVar15[-9];
            param_1[uVar17 * 10 + 0x3c] = puVar15[-8];
            param_1[uVar17 * 10 + 0x3b] = uVar19;
            uVar22 = puVar15[-3];
            uVar20 = *puVar15;
            uVar19 = puVar15[-1];
            uVar33 = puVar15[-6];
            uVar32 = puVar15[-7];
            uVar28 = puVar15[-4];
            uVar25 = puVar15[-5];
            param_1[uVar17 * 10 + 0x42] = puVar15[-2];
            param_1[uVar17 * 10 + 0x41] = uVar22;
            param_1[uVar17 * 10 + 0x44] = uVar20;
            param_1[uVar17 * 10 + 0x43] = uVar19;
            param_1[uVar17 * 10 + 0x3e] = uVar33;
            param_1[uVar17 * 10 + 0x3d] = uVar32;
            param_1[uVar17 * 10 + 0x40] = uVar28;
            param_1[uVar17 * 10 + 0x3f] = uVar25;
          }
          uVar11 = uVar11 + 1;
        }
      }
      else if ((*(uint *)((long)puVar15 + -4) <= uVar1) &&
              (uVar1 - *(uint *)((long)puVar15 + -4) <= uVar10)) goto LAB_001afb84;
      uVar13 = uVar13 + 1;
      puVar15 = puVar15 + 10;
    } while ((uint)uVar12 != uVar13);
  }
  uVar10 = (uint)param_1[0x19];
  *(uint *)(param_1 + 0x2f) = uVar11;
  uVar1 = (uint)param_1[0x2d];
  if (uVar10 - 1 < uVar1) {
    uVar10 = 0;
    *(undefined4 *)(param_1 + 0x19) = 0;
    fVar18 = *(float *)(param_1 + 0x1a) + -0.5;
    if (fVar18 <= 0.0) {
      fVar18 = 0.0;
    }
    *(float *)(param_1 + 0x1a) = fVar18;
    if (2 < *(uint *)((long)param_1 + 0xcc)) {
      uVar10 = 0;
      *(undefined4 *)((long)param_1 + 0xcc) = 0;
    }
  }
  if ((uVar11 == 0) || ((int)param_1[0x2e] == 0)) {
    if ((*(int *)((long)param_1 + 0x174) == 1) && (uVar10 <= uVar1)) {
      *(undefined4 *)(param_1[0xa3c] + 8) = 1;
      return;
    }
    return;
  }
  if (*(uint *)((long)param_1 + 0xd4) <= uVar1) {
    iVar2 = *(int *)((long)param_1 + 0x1c4);
    iVar9 = *(int *)((long)param_1 + 0x1cc);
    if ((*(uint *)((long)param_1 + 0xdc) <= uVar1) || ((int)param_1[0x10] == 0)) {
      uVar12 = (ulong)uVar11;
      if (7 < uVar11) {
        uVar13 = uVar12 & 0xfffffff8;
        puVar16 = auStack_58 + 1;
        uVar17 = uVar13;
        uVar26 = _DAT_00112af0;
        uVar29 = _UNK_00112af8;
        do {
          iVar27 = (int)((ulong)uVar26 >> 0x20);
          iVar30 = (int)((ulong)uVar29 >> 0x20);
          uVar17 = uVar17 - 8;
          puVar16[-1] = uVar29;
          puVar16[-2] = uVar26;
          puVar16[1] = CONCAT44(iVar30 + 4,(int)uVar29 + 4);
          *puVar16 = CONCAT44(iVar27 + 4,(int)uVar26 + 4);
          puVar16 = puVar16 + 4;
          uVar26 = CONCAT44(iVar27 + 8,(int)uVar26 + 8);
          uVar29 = CONCAT44(iVar30 + 8,(int)uVar29 + 8);
          if (uVar17 == 0) goto joined_r0x001afcc0;
        } while( true );
      }
      uVar13 = 0;
      do {
        local_60[uVar13] = (uint)uVar13;
        uVar13 = uVar13 + 1;
joined_r0x001afcc0:
      } while (uVar13 != uVar12);
      fVar18 = *(float *)((long)param_1 + 0x1a4);
      fVar21 = *(float *)(param_1 + 0x35);
      if (1 < uVar11) {
        lVar14 = 0;
        uVar13 = 1;
        do {
          uVar10 = local_60[uVar13];
          fVar23 = *(float *)(param_1 + (ulong)uVar10 * 10 + 0x43);
          lVar7 = lVar14;
          do {
            if (fVar23 <= *(float *)(param_1 + (ulong)local_60[lVar7] * 10 + 0x43)) {
              uVar17 = lVar7 + 1;
              goto LAB_001afd00;
            }
            lVar8 = lVar7 + -1;
            local_60[lVar7 + 1] = local_60[lVar7];
            lVar7 = lVar8;
          } while (lVar8 != -1);
          uVar17 = 0;
LAB_001afd00:
          uVar13 = uVar13 + 1;
          lVar14 = lVar14 + 1;
          local_60[uVar17 & 0xffffffff] = uVar10;
        } while (uVar13 != uVar12);
      }
      uVar10 = *(uint *)((long)param_1 + (ulong)local_60[0] * 0x50 + 0x21c);
      fVar23 = 0.0;
      if (uVar10 <= uVar1) {
        fVar23 = (float)(uVar1 - uVar10) / 30.0;
      }
      fVar34 = (float)NEON_fmadd((int)param_1[(ulong)local_60[0] * 10 + 0x3e],fVar23,
                                 (int)param_1[(ulong)local_60[0] * 10 + 0x3d]);
      fVar35 = (float)NEON_fmadd(*(undefined4 *)((long)param_1 + (ulong)local_60[0] * 0x50 + 0x1ec),
                                 fVar23,*(undefined4 *)
                                         ((long)param_1 + (ulong)local_60[0] * 0x50 + 0x1e4));
      fVar31 = (float)NEON_fmadd(fVar35 - fVar18,fVar35 - fVar18,
                                 (fVar34 - fVar21) * (fVar34 - fVar21));
      fVar23 = *(float *)(param_1 + (ulong)local_60[0] * 10 + 0x41) - fVar23;
      if (fVar23 <= 0.0) {
        fVar23 = 0.0;
      }
      fVar38 = ((float)(int)param_1[0x39] / 100.0) * 1.3333334;
      fVar24 = SQRT(fVar31) * 0.6;
      if (fVar38 <= SQRT(fVar31) * 0.6) {
        fVar24 = fVar38;
      }
      fVar38 = *(float *)(param_1 + (ulong)local_60[0] * 10 + 0x40);
      fVar34 = (float)NEON_fmadd(*(float *)(param_1 + (ulong)local_60[0] * 10 + 0x42) *
                                 *(float *)(param_1 + (ulong)local_60[0] * 10 + 0x3f),fVar23 * 1.3,
                                 fVar34);
      fVar31 = *(float *)((long)param_1 + (ulong)local_60[0] * 0x50 + 0x1fc);
      fVar40 = (float)NEON_fmadd(fVar38,fVar24,fVar21);
      fVar23 = (float)NEON_fmadd(*(float *)((long)param_1 + (ulong)local_60[0] * 0x50 + 500) *
                                 *(float *)(param_1 + (ulong)local_60[0] * 10 + 0x42),fVar23 * 1.3,
                                 fVar35);
      fVar35 = (float)NEON_fmadd(fVar31,fVar24,fVar18);
      if (2 < uVar11) {
        uVar11 = 3;
      }
      fVar36 = (float)NEON_fmadd(fVar35 - fVar23,fVar35 - fVar23,
                                 (fVar40 - fVar34) * (fVar40 - fVar34));
      fVar39 = (float)NEON_fmsub(fVar38,fVar24,fVar21);
      fVar37 = (float)NEON_fmsub(fVar31,fVar24,fVar18);
      fVar36 = SQRT(fVar36) * 0.4;
      if (1 < uVar11) {
        lVar14 = (ulong)uVar11 - 1;
        do {
          puVar5 = puVar5 + 1;
          uVar4 = *puVar5;
          uVar3 = *(uint *)((long)param_1 + (ulong)uVar4 * 0x50 + 0x21c);
          uVar10 = 0;
          if (uVar3 <= uVar1) {
            uVar10 = uVar1 - uVar3;
          }
          lVar14 = lVar14 + -1;
          fVar41 = (float)uVar10 / 30.0;
          fVar43 = *(float *)(param_1 + (ulong)uVar4 * 10 + 0x41) - fVar41;
          uVar44 = NEON_fmadd((int)param_1[(ulong)uVar4 * 10 + 0x3e],fVar41,
                              (int)param_1[(ulong)uVar4 * 10 + 0x3d]);
          uVar42 = NEON_fmadd(*(undefined4 *)((long)param_1 + (ulong)uVar4 * 0x50 + 0x1ec),fVar41,
                              *(undefined4 *)((long)param_1 + (ulong)uVar4 * 0x50 + 0x1e4));
          if (fVar43 <= 0.0) {
            fVar43 = 0.0;
          }
          fVar45 = (float)NEON_fmadd(*(float *)(param_1 + (ulong)uVar4 * 10 + 0x42) *
                                     *(float *)(param_1 + (ulong)uVar4 * 10 + 0x3f),fVar43,uVar44);
          fVar41 = (float)NEON_fmadd(*(float *)((long)param_1 + (ulong)uVar4 * 0x50 + 500) *
                                     *(float *)(param_1 + (ulong)uVar4 * 10 + 0x42),fVar43,uVar42);
          fVar41 = (float)NEON_fmadd(fVar35 - fVar41,fVar35 - fVar41,
                                     (fVar40 - fVar45) * (fVar40 - fVar45));
          fVar36 = (float)NEON_fmadd(SQRT(fVar41),0x3e4ccccd,fVar36);
        } while (lVar14 != 0);
      }
      fVar35 = (float)NEON_fmadd(fVar35 - fVar18,fVar35 - fVar18,
                                 (fVar40 - fVar21) * (fVar40 - fVar21));
      fVar23 = (float)NEON_fmadd(fVar37 - fVar23,fVar37 - fVar23,
                                 (fVar39 - fVar34) * (fVar39 - fVar34));
      fVar34 = (float)NEON_fmadd(SQRT(fVar35),0xbdcccccd,fVar36);
      fVar23 = SQRT(fVar23) * 0.4;
      if (1 < uVar11) {
        lVar14 = (ulong)uVar11 - 1;
        do {
          puVar6 = puVar6 + 1;
          uVar3 = *puVar6;
          uVar11 = *(uint *)((long)param_1 + (ulong)uVar3 * 0x50 + 0x21c);
          uVar10 = 0;
          if (uVar11 <= uVar1) {
            uVar10 = uVar1 - uVar11;
          }
          lVar14 = lVar14 + -1;
          fVar40 = (float)uVar10 / 30.0;
          uVar44 = NEON_fmadd((int)param_1[(ulong)uVar3 * 10 + 0x3e],fVar40,
                              (int)param_1[(ulong)uVar3 * 10 + 0x3d]);
          uVar42 = NEON_fmadd(*(undefined4 *)((long)param_1 + (ulong)uVar3 * 0x50 + 0x1ec),fVar40,
                              *(undefined4 *)((long)param_1 + (ulong)uVar3 * 0x50 + 0x1e4));
          fVar35 = *(float *)(param_1 + (ulong)uVar3 * 10 + 0x41) - fVar40;
          if (*(float *)(param_1 + (ulong)uVar3 * 10 + 0x41) - fVar40 <= 0.0) {
            fVar35 = 0.0;
          }
          fVar40 = (float)NEON_fmadd(*(float *)(param_1 + (ulong)uVar3 * 10 + 0x42) *
                                     *(float *)(param_1 + (ulong)uVar3 * 10 + 0x3f),fVar35,uVar44);
          fVar35 = (float)NEON_fmadd(*(float *)((long)param_1 + (ulong)uVar3 * 0x50 + 500) *
                                     *(float *)(param_1 + (ulong)uVar3 * 10 + 0x42),fVar35,uVar42);
          fVar35 = (float)NEON_fmadd(fVar37 - fVar35,fVar37 - fVar35,
                                     (fVar39 - fVar40) * (fVar39 - fVar40));
          fVar23 = (float)NEON_fmadd(SQRT(fVar35),0x3e4ccccd,fVar23);
        } while (lVar14 != 0);
      }
      uVar10 = (int)param_1[0x3a] * 3 + 0x212;
      fVar18 = (float)NEON_fmadd(fVar37 - fVar18,fVar37 - fVar18,
                                 (fVar39 - fVar21) * (fVar39 - fVar21));
      fVar18 = (float)NEON_fmadd(SQRT(fVar18),0xbdcccccd,fVar23);
      if (fVar34 == fVar18 || fVar34 < fVar18 != (NAN(fVar34) || NAN(fVar18))) {
        fVar38 = -fVar38;
        fVar31 = -fVar31;
      }
      uVar11 = uVar10 / 0x424;
      if (0xb < uVar11) {
        uVar11 = 0xc;
      }
      if (uVar10 < 0x424) {
        uVar11 = 1;
      }
      if (0xb3 < iVar9) {
        iVar9 = 0xb4;
      }
      if (iVar9 < 0x33) {
        iVar9 = 0x32;
      }
      uVar12 = param_1[0xa3c];
      *(float *)(uVar12 + 0x38) = fVar31;
      *(float *)(uVar12 + 0x3c) = fVar38;
      uVar10 = (uint)((iVar9 * 4 + 0x32) * 0x51f) >> 0x11;
      *(undefined4 *)(uVar12 + 8) = 2;
      *(undefined4 *)(uVar12 + 0x10) = 1;
      *(uint *)(uVar12 + 0x14) = uVar10;
      *(float *)(uVar12 + 0x40) = ((float)iVar2 / 100.0) * 180.0;
      *(float *)(uVar12 + 0x44) = fVar24;
      *(uint *)((long)param_1 + 0xd4) = uVar11 + uVar1;
      *(uint *)((long)param_1 + 0xdc) = uVar1 + 2;
      *(uint *)((long)param_1 + 0x184) = uVar1;
      *(undefined4 *)(param_1 + 0x33) = 0;
      *(undefined4 *)(param_1 + 0x30) = 0;
      uVar42 = NEON_fmin(*(float *)(param_1 + 0x1a) + 1.0,0x40a00000);
      *(uint *)(param_1 + 0x19) = uVar1 + uVar10;
      *(int *)((long)param_1 + 0xcc) = *(int *)((long)param_1 + 0xcc) + 1;
      *(int *)(param_1 + 0x1d) = (int)param_1[0x1d] + 1;
      *(undefined4 *)(param_1 + 0x1a) = uVar42;
      return;
    }
  }
  *(uint *)((long)param_1 + 0x184) = uVar1;
  return;
}

/* ===== FUN_001b4670 @ 001b4670 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Type propagation algorithm not settling */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

bool FUN_001b4670(undefined1 param_1 [16],undefined1 param_2 [16],float param_3,long param_4,
                 float *param_5,long param_6,uint param_7,uint param_8,uint param_9,int param_10,
                 undefined1 (*param_11) [16])

{
  uint uVar1;
  undefined1 auVar2 [16];
  undefined1 auVar3 [16];
  undefined1 auVar4 [16];
  undefined1 auVar5 [16];
  undefined1 auVar6 [16];
  undefined1 auVar7 [16];
  undefined1 auVar8 [16];
  undefined1 auVar9 [16];
  undefined1 auVar10 [16];
  long lVar11;
  undefined1 auVar12 [16];
  undefined1 auVar13 [16];
  undefined1 auVar14 [16];
  undefined1 auVar15 [16];
  undefined1 auVar16 [16];
  undefined1 auVar17 [16];
  undefined1 auVar18 [16];
  byte bVar19;
  byte bVar20;
  byte bVar21;
  byte bVar22;
  undefined1 uVar23;
  undefined1 uVar24;
  undefined1 uVar25;
  undefined1 uVar26;
  undefined8 uVar27;
  undefined8 uVar28;
  undefined8 uVar29;
  undefined8 uVar30;
  undefined1 auVar31 [16];
  int iVar32;
  int iVar33;
  uint uVar34;
  ulong uVar35;
  long lVar36;
  ulong uVar37;
  undefined8 *puVar38;
  ulong uVar39;
  ulong uVar40;
  ulong uVar41;
  uint uVar42;
  undefined1 (*pauVar43) [16];
  uint uVar44;
  float *pfVar45;
  ulong uVar46;
  byte bVar47;
  float fVar48;
  uint uVar50;
  byte bVar57;
  byte bVar58;
  float fVar49;
  undefined1 auVar51 [16];
  undefined1 auVar52 [16];
  undefined1 auVar53 [16];
  undefined1 auVar54 [16];
  byte bVar59;
  undefined1 auVar55 [16];
  undefined1 auVar56 [16];
  float fVar60;
  undefined1 auVar61 [16];
  undefined1 auVar62 [16];
  undefined1 auVar63 [16];
  undefined1 auVar64 [16];
  undefined1 auVar65 [16];
  undefined1 auVar66 [16];
  undefined8 uVar67;
  undefined1 auVar68 [16];
  undefined1 auVar69 [16];
  undefined1 auVar70 [16];
  undefined1 auVar71 [16];
  undefined1 auVar72 [16];
  undefined1 auVar73 [16];
  undefined1 auVar74 [16];
  undefined1 auVar75 [16];
  undefined1 auVar76 [16];
  undefined1 auVar77 [16];
  undefined1 auVar78 [16];
  undefined1 auVar79 [16];
  undefined1 auVar80 [16];
  undefined1 auVar81 [16];
  undefined1 auVar82 [16];
  undefined1 auVar83 [16];
  undefined1 auVar84 [16];
  undefined1 auVar85 [16];
  undefined1 auVar86 [16];
  undefined1 auVar87 [16];
  undefined1 auVar88 [16];
  undefined1 auVar89 [16];
  undefined1 auVar90 [16];
  undefined1 auVar91 [16];
  undefined1 uVar93;
  undefined1 uVar94;
  undefined1 uVar95;
  undefined1 uVar96;
  undefined1 uVar97;
  undefined1 uVar98;
  undefined1 uVar99;
  undefined1 auVar92 [16];
  undefined1 uVar100;
  float fVar101;
  undefined1 auVar103 [16];
  int iVar104;
  int iVar105;
  int iVar106;
  int iVar108;
  undefined8 uVar107;
  int iVar109;
  int iVar111;
  undefined8 uVar110;
  int iVar112;
  int iVar114;
  undefined8 uVar113;
  undefined1 auVar115 [16];
  undefined1 auVar116 [16];
  undefined1 auVar117 [16];
  undefined1 auVar118 [16];
  undefined1 auVar119 [16];
  int iVar120;
  int iVar122;
  int iVar123;
  int iVar124;
  undefined1 auVar121 [16];
  undefined1 uVar125;
  byte bVar126;
  undefined1 uVar127;
  byte bVar128;
  undefined1 uVar129;
  byte bVar130;
  undefined1 uVar131;
  byte bVar132;
  undefined1 uVar133;
  undefined1 uVar134;
  undefined1 uVar135;
  undefined1 uVar136;
  byte bVar137;
  byte bVar138;
  byte bVar139;
  byte bVar140;
  undefined1 uVar141;
  undefined1 uVar142;
  undefined1 uVar143;
  undefined1 uVar144;
  undefined1 uVar145;
  undefined1 uVar146;
  undefined1 uVar147;
  undefined1 uVar148;
  undefined1 uVar149;
  byte bVar150;
  undefined1 uVar151;
  byte bVar152;
  undefined1 uVar153;
  byte bVar154;
  undefined1 uVar155;
  byte bVar156;
  undefined1 uVar157;
  undefined1 uVar158;
  undefined1 uVar159;
  undefined1 uVar160;
  undefined1 uVar161;
  byte bVar162;
  undefined1 uVar163;
  byte bVar164;
  undefined1 uVar165;
  byte bVar166;
  undefined1 uVar167;
  byte bVar168;
  undefined1 uVar169;
  undefined1 uVar170;
  undefined1 uVar171;
  undefined1 uVar172;
  byte bVar173;
  byte bVar174;
  byte bVar175;
  byte bVar176;
  byte bVar177;
  byte bVar178;
  byte bVar179;
  byte bVar180;
  byte bVar181;
  byte bVar182;
  byte bVar183;
  byte bVar184;
  undefined1 uVar185;
  undefined1 uVar186;
  undefined1 uVar187;
  undefined1 uVar188;
  undefined1 uVar189;
  byte bVar190;
  undefined1 uVar191;
  byte bVar192;
  undefined1 uVar193;
  byte bVar194;
  undefined1 uVar195;
  byte bVar196;
  undefined1 uVar197;
  undefined1 uVar198;
  undefined1 uVar199;
  undefined1 uVar200;
  undefined1 uVar201;
  undefined1 uVar202;
  undefined1 uVar203;
  undefined1 uVar204;
  undefined1 uVar205;
  byte bVar206;
  undefined1 uVar207;
  byte bVar208;
  undefined1 uVar209;
  byte bVar210;
  undefined1 uVar211;
  byte bVar212;
  undefined1 uVar213;
  undefined1 uVar214;
  undefined1 uVar215;
  undefined1 uVar216;
  long lVar218;
  undefined1 auVar217 [16];
  undefined1 uVar219;
  byte bVar220;
  undefined1 uVar221;
  byte bVar222;
  undefined1 uVar223;
  byte bVar224;
  undefined1 uVar225;
  byte bVar226;
  undefined1 uVar227;
  byte bVar228;
  undefined1 uVar229;
  byte bVar230;
  undefined1 uVar231;
  byte bVar232;
  undefined1 uVar233;
  byte bVar234;
  undefined1 uVar235;
  byte bVar236;
  undefined1 uVar237;
  byte bVar238;
  undefined1 uVar239;
  byte bVar240;
  undefined1 uVar241;
  byte bVar242;
  undefined1 uVar243;
  byte bVar244;
  undefined1 uVar245;
  byte bVar246;
  undefined1 uVar247;
  byte bVar248;
  undefined1 uVar249;
  byte bVar250;
  undefined1 auVar251 [16];
  byte bVar252;
  byte bVar253;
  byte bVar254;
  byte bVar255;
  undefined1 uVar256;
  undefined1 uVar257;
  undefined1 uVar258;
  undefined1 uVar259;
  byte bVar260;
  byte bVar261;
  byte bVar262;
  byte bVar263;
  undefined1 uVar264;
  undefined1 uVar265;
  undefined1 uVar266;
  undefined1 uVar267;
  undefined1 auVar268 [16];
  float local_3bc;
  uint local_3ac;
  float local_390;
  undefined1 auStack_38c [12];
  byte bStack_378;
  byte bStack_377;
  byte bStack_376;
  byte bStack_375;
  undefined1 uStack_374;
  undefined1 uStack_373;
  undefined1 uStack_372;
  undefined1 uStack_371;
  float local_358;
  float fStack_354;
  char local_350 [128];
  float local_2d0 [130];
  undefined1 local_c8 [16];
  undefined8 local_b8;
  undefined1 auVar102 [16];
  
  if (param_7 <= param_8 - 1) {
    return false;
  }
  if (param_11 == (undefined1 (*) [16])0x0) {
    return false;
  }
  uVar39 = (ulong)param_7;
  *(undefined8 *)(*param_11 + 8) = 0;
  *(undefined8 *)*param_11 = 0;
  *(undefined8 *)(param_11[1] + 8) = 0;
  *(undefined8 *)param_11[1] = 0;
  *(undefined8 *)(param_11[2] + 8) = 0;
  *(undefined8 *)param_11[2] = 0;
  *(undefined8 *)(param_11[3] + 8) = 0;
  *(undefined8 *)param_11[3] = 0;
  *(undefined8 *)(param_11[4] + 8) = 0;
  *(undefined8 *)param_11[4] = 0;
  *(undefined8 *)(param_11[5] + 8) = 0;
  *(undefined8 *)param_11[5] = 0;
  *(undefined8 *)(param_11[6] + 8) = 0;
  *(undefined8 *)param_11[6] = 0;
  *(undefined8 *)(param_11[7] + 8) = 0;
  *(undefined8 *)param_11[7] = 0;
  if (param_8 == param_7) {
    memset(param_11,1,(ulong)param_8);
    return true;
  }
  fVar101 = (float)uVar39;
  auVar102 = ZEXT416((uint)fVar101);
  uVar35 = 0;
  local_2d0[2] = 0.0;
  local_2d0[3] = 0.0;
  local_2d0[0] = 0.0;
  local_2d0[1] = 0.0;
  local_2d0[6] = 0.0;
  local_2d0[7] = 0.0;
  local_2d0[4] = 0.0;
  local_2d0[5] = 0.0;
  local_2d0[10] = 0.0;
  local_2d0[0xb] = 0.0;
  local_2d0[8] = 0.0;
  local_2d0[9] = 0.0;
  local_2d0[0xe] = 0.0;
  local_2d0[0xf] = 0.0;
  local_2d0[0xc] = 0.0;
  local_2d0[0xd] = 0.0;
  local_2d0[0x12] = 0.0;
  local_2d0[0x13] = 0.0;
  local_2d0[0x10] = 0.0;
  local_2d0[0x11] = 0.0;
  local_2d0[0x16] = 0.0;
  local_2d0[0x17] = 0.0;
  local_2d0[0x14] = 0.0;
  local_2d0[0x15] = 0.0;
  local_2d0[0x1a] = 0.0;
  local_2d0[0x1b] = 0.0;
  local_2d0[0x18] = 0.0;
  local_2d0[0x19] = 0.0;
  local_2d0[0x1e] = 0.0;
  local_2d0[0x1f] = 0.0;
  local_2d0[0x1c] = 0.0;
  local_2d0[0x1d] = 0.0;
  local_2d0[0x22] = 0.0;
  local_2d0[0x23] = 0.0;
  local_2d0[0x20] = 0.0;
  local_2d0[0x21] = 0.0;
  local_2d0[0x26] = 0.0;
  local_2d0[0x27] = 0.0;
  local_2d0[0x24] = 0.0;
  local_2d0[0x25] = 0.0;
  local_2d0[0x2a] = 0.0;
  local_2d0[0x2b] = 0.0;
  local_2d0[0x28] = 0.0;
  local_2d0[0x29] = 0.0;
  local_2d0[0x2e] = 0.0;
  local_2d0[0x2f] = 0.0;
  local_2d0[0x2c] = 0.0;
  local_2d0[0x2d] = 0.0;
  local_2d0[0x32] = 0.0;
  local_2d0[0x33] = 0.0;
  local_2d0[0x30] = 0.0;
  local_2d0[0x31] = 0.0;
  local_2d0[0x36] = 0.0;
  local_2d0[0x37] = 0.0;
  local_2d0[0x34] = 0.0;
  local_2d0[0x35] = 0.0;
  local_2d0[0x3a] = 0.0;
  local_2d0[0x3b] = 0.0;
  local_2d0[0x38] = 0.0;
  local_2d0[0x39] = 0.0;
  local_2d0[0x3e] = 0.0;
  local_2d0[0x3f] = 0.0;
  local_2d0[0x3c] = 0.0;
  local_2d0[0x3d] = 0.0;
  local_2d0[0x42] = 0.0;
  local_2d0[0x43] = 0.0;
  local_2d0[0x40] = 0.0;
  local_2d0[0x41] = 0.0;
  local_2d0[0x46] = 0.0;
  local_2d0[0x47] = 0.0;
  local_2d0[0x44] = 0.0;
  local_2d0[0x45] = 0.0;
  local_2d0[0x4a] = 0.0;
  local_2d0[0x4b] = 0.0;
  local_2d0[0x48] = 0.0;
  local_2d0[0x49] = 0.0;
  local_2d0[0x4e] = 0.0;
  local_2d0[0x4f] = 0.0;
  local_2d0[0x4c] = 0.0;
  local_2d0[0x4d] = 0.0;
  local_2d0[0x52] = 0.0;
  local_2d0[0x53] = 0.0;
  local_2d0[0x50] = 0.0;
  local_2d0[0x51] = 0.0;
  local_2d0[0x56] = 0.0;
  local_2d0[0x57] = 0.0;
  local_2d0[0x54] = 0.0;
  local_2d0[0x55] = 0.0;
  local_2d0[0x5a] = 0.0;
  local_2d0[0x5b] = 0.0;
  local_2d0[0x58] = 0.0;
  local_2d0[0x59] = 0.0;
  local_2d0[0x5e] = 0.0;
  local_2d0[0x5f] = 0.0;
  local_2d0[0x5c] = 0.0;
  local_2d0[0x5d] = 0.0;
  local_2d0[0x62] = 0.0;
  local_2d0[99] = 0.0;
  local_2d0[0x60] = 0.0;
  local_2d0[0x61] = 0.0;
  local_2d0[0x66] = 0.0;
  local_2d0[0x67] = 0.0;
  local_2d0[100] = 0.0;
  local_2d0[0x65] = 0.0;
  local_2d0[0x6a] = 0.0;
  local_2d0[0x6b] = 0.0;
  local_2d0[0x68] = 0.0;
  local_2d0[0x69] = 0.0;
  local_2d0[0x6e] = 0.0;
  local_2d0[0x6f] = 0.0;
  local_2d0[0x6c] = 0.0;
  local_2d0[0x6d] = 0.0;
  local_2d0[0x72] = 0.0;
  local_2d0[0x73] = 0.0;
  local_2d0[0x70] = 0.0;
  local_2d0[0x71] = 0.0;
  local_2d0[0x76] = 0.0;
  local_2d0[0x77] = 0.0;
  local_2d0[0x74] = 0.0;
  local_2d0[0x75] = 0.0;
  local_2d0[0x7a] = 0.0;
  local_2d0[0x7b] = 0.0;
  local_2d0[0x78] = 0.0;
  local_2d0[0x79] = 0.0;
  local_2d0[0x7e] = 0.0;
  local_2d0[0x7f] = 0.0;
  local_2d0[0x7c] = 0.0;
  local_2d0[0x7d] = 0.0;
  local_3ac = 0xffffffff;
  local_3bc = -1e+30;
  local_350[8] = '\0';
  local_350[9] = '\0';
  local_350[10] = '\0';
  local_350[0xb] = '\0';
  local_350[0xc] = '\0';
  local_350[0xd] = '\0';
  local_350[0xe] = '\0';
  local_350[0xf] = '\0';
  local_350[0] = '\0';
  local_350[1] = '\0';
  local_350[2] = '\0';
  local_350[3] = '\0';
  local_350[4] = '\0';
  local_350[5] = '\0';
  local_350[6] = '\0';
  local_350[7] = '\0';
  local_350[0x18] = '\0';
  local_350[0x19] = '\0';
  local_350[0x1a] = '\0';
  local_350[0x1b] = '\0';
  local_350[0x1c] = '\0';
  local_350[0x1d] = '\0';
  local_350[0x1e] = '\0';
  local_350[0x1f] = '\0';
  local_350[0x10] = '\0';
  local_350[0x11] = '\0';
  local_350[0x12] = '\0';
  local_350[0x13] = '\0';
  local_350[0x14] = '\0';
  local_350[0x15] = '\0';
  local_350[0x16] = '\0';
  local_350[0x17] = '\0';
  local_350[0x28] = '\0';
  local_350[0x29] = '\0';
  local_350[0x2a] = '\0';
  local_350[0x2b] = '\0';
  local_350[0x2c] = '\0';
  local_350[0x2d] = '\0';
  local_350[0x2e] = '\0';
  local_350[0x2f] = '\0';
  local_350[0x20] = '\0';
  local_350[0x21] = '\0';
  local_350[0x22] = '\0';
  local_350[0x23] = '\0';
  local_350[0x24] = '\0';
  local_350[0x25] = '\0';
  local_350[0x26] = '\0';
  local_350[0x27] = '\0';
  local_350[0x38] = '\0';
  local_350[0x39] = '\0';
  local_350[0x3a] = '\0';
  local_350[0x3b] = '\0';
  local_350[0x3c] = '\0';
  local_350[0x3d] = '\0';
  local_350[0x3e] = '\0';
  local_350[0x3f] = '\0';
  local_350[0x30] = '\0';
  local_350[0x31] = '\0';
  local_350[0x32] = '\0';
  local_350[0x33] = '\0';
  local_350[0x34] = '\0';
  local_350[0x35] = '\0';
  local_350[0x36] = '\0';
  local_350[0x37] = '\0';
  local_350[0x48] = '\0';
  local_350[0x49] = '\0';
  local_350[0x4a] = '\0';
  local_350[0x4b] = '\0';
  local_350[0x4c] = '\0';
  local_350[0x4d] = '\0';
  local_350[0x4e] = '\0';
  local_350[0x4f] = '\0';
  local_350[0x40] = '\0';
  local_350[0x41] = '\0';
  local_350[0x42] = '\0';
  local_350[0x43] = '\0';
  local_350[0x44] = '\0';
  local_350[0x45] = '\0';
  local_350[0x46] = '\0';
  local_350[0x47] = '\0';
  local_350[0x58] = '\0';
  local_350[0x59] = '\0';
  local_350[0x5a] = '\0';
  local_350[0x5b] = '\0';
  local_350[0x5c] = '\0';
  local_350[0x5d] = '\0';
  local_350[0x5e] = '\0';
  local_350[0x5f] = '\0';
  local_350[0x50] = '\0';
  local_350[0x51] = '\0';
  local_350[0x52] = '\0';
  local_350[0x53] = '\0';
  local_350[0x54] = '\0';
  local_350[0x55] = '\0';
  local_350[0x56] = '\0';
  local_350[0x57] = '\0';
  local_350[0x68] = '\0';
  local_350[0x69] = '\0';
  local_350[0x6a] = '\0';
  local_350[0x6b] = '\0';
  local_350[0x6c] = '\0';
  local_350[0x6d] = '\0';
  local_350[0x6e] = '\0';
  local_350[0x6f] = '\0';
  local_350[0x60] = '\0';
  local_350[0x61] = '\0';
  local_350[0x62] = '\0';
  local_350[99] = '\0';
  local_350[100] = '\0';
  local_350[0x65] = '\0';
  local_350[0x66] = '\0';
  local_350[0x67] = '\0';
  local_350[0x78] = '\0';
  local_350[0x79] = '\0';
  local_350[0x7a] = '\0';
  local_350[0x7b] = '\0';
  local_350[0x7c] = '\0';
  local_350[0x7d] = '\0';
  local_350[0x7e] = '\0';
  local_350[0x7f] = '\0';
  local_350[0x70] = '\0';
  local_350[0x71] = '\0';
  local_350[0x72] = '\0';
  local_350[0x73] = '\0';
  local_350[0x74] = '\0';
  local_350[0x75] = '\0';
  local_350[0x76] = '\0';
  local_350[0x77] = '\0';
LAB_001b47a0:
  sincosf(((float)(uVar35 & 0xffffffff) * 6.2831855) / auVar102._0_4_,&fStack_354,&local_358);
  if ((param_5 != (float *)0x0 && param_6 != 0) && (fVar48 = param_5[8], fVar48 != 0.0)) {
    uVar40 = 0;
    uVar42 = 1;
    local_390 = 1e+30;
LAB_001b47f0:
    uVar46 = (ulong)(uint)fVar48;
    lVar36 = uVar40 << 4;
    pfVar45 = (float *)(param_6 + 0x87c + uVar40 * 0x3c);
    do {
      local_b8 = 0;
      if ((param_3 < 0.0) || (iVar32 = FUN_001b58b4(local_c8), iVar32 == 0)) {
LAB_001b49bc:
        auVar102 = ZEXT416((uint)fVar101);
        break;
      }
      fVar48 = (float)FUN_001b6b34((long)param_5 + lVar36,local_c8,0);
      fVar60 = 0.0;
      if ((param_5[9] != 0.0) && (1 < (uint)uVar46)) {
        fVar49 = hypotf(*param_5 - param_5[4],param_5[1] - param_5[5]);
        fVar60 = param_5[2];
        if (param_5[2] <= param_5[6]) {
          fVar60 = param_5[6];
        }
        fVar60 = (float)NEON_fminnm(fVar60,0);
        fVar60 = fVar49 * 0.5 * fVar60;
      }
      fVar60 = fVar48 + fVar60 + *pfVar45;
      fVar48 = ABS(fVar60);
      if (((fVar48 == INFINITY) || (NAN(fVar48))) || (fVar60 < 0.0)) {
        *(undefined4 *)(param_4 + 0x194) = 1;
        goto LAB_001b49bc;
      }
      iVar32 = FUN_001b6c88(param_4,(long)&local_b8 + 4,&local_b8);
      if (iVar32 != 0) goto LAB_001b4938;
      if (*(int *)(param_4 + 0x194) != 0) goto LAB_001b49bc;
      uVar46 = (ulong)(uint)param_5[8];
      uVar42 = 0;
      uVar40 = uVar40 + 1;
      lVar36 = lVar36 + 0x10;
      pfVar45 = pfVar45 + 0xf;
      if (uVar46 <= uVar40) goto LAB_001b49e8;
    } while( true );
  }
  if (*(int *)(param_4 + 0x194) != 0) {
    return false;
  }
  goto LAB_001b49d8;
LAB_001b4938:
  uVar40 = uVar40 + 1;
  fVar48 = param_5[8];
  uVar42 = (uint)local_b8 & uVar42;
  fVar60 = local_b8._4_4_;
  if (local_390 <= local_b8._4_4_) {
    fVar60 = local_390;
  }
  local_390 = fVar60;
  if ((uint)fVar48 <= uVar40) goto code_r0x001b4968;
  goto LAB_001b47f0;
code_r0x001b4968:
  if (uVar42 == 0) {
LAB_001b49e8:
    auVar102 = ZEXT416((uint)fVar101);
  }
  else {
    auVar102 = ZEXT416((uint)fVar101);
    local_350[uVar35] = '\x01';
    local_2d0[uVar35] = fVar60;
    if (fVar60 != local_3bc && fVar60 < local_3bc == (NAN(fVar60) || NAN(local_3bc))) {
      local_3ac = (uint)uVar35;
      local_3bc = fVar60;
    }
  }
LAB_001b49d8:
  uVar35 = uVar35 + 1;
  if (uVar35 == uVar39) {
    uVar35 = 0xffffffff;
    uVar42 = 0xffffffff;
    if ((-1 < (int)local_3ac) && (param_7 != 0)) {
      uVar40 = 0;
      fVar101 = -1e+30;
      uVar34 = local_3ac;
      do {
        if (local_350[uVar40] != '\0') {
          uVar42 = (int)uVar40 - local_3ac;
          if (uVar40 <= local_3ac) {
            uVar42 = uVar34;
          }
          if (param_7 - uVar42 <= uVar42) {
            uVar42 = param_7 - uVar42;
          }
          if ((1 < uVar42) &&
             (fVar48 = local_2d0[uVar40],
             fVar48 != fVar101 && fVar48 < fVar101 == (NAN(fVar48) || NAN(fVar101)))) {
            uVar35 = uVar40 & 0xffffffff;
            fVar101 = fVar48;
          }
        }
        uVar42 = (uint)uVar35;
        uVar40 = uVar40 + 1;
        uVar34 = uVar34 - 1;
      } while (uVar39 != uVar40);
    }
    uVar34 = 0;
    if ((((-1 < (int)param_9) && (param_9 < param_7)) && (uVar34 = 0, param_8 != 0)) &&
       ((*param_11)[param_9] == '\0')) {
      uVar34 = 1;
      (*param_11)[param_9] = 1;
    }
    if (((-1 < (int)local_3ac) && (local_3ac < param_7)) &&
       ((uVar34 < param_8 && ((*param_11)[local_3ac] == '\0')))) {
      uVar34 = uVar34 + 1;
      (*param_11)[local_3ac] = 1;
    }
    if (((3 < param_8) && (-1 < (int)uVar42)) &&
       ((uVar42 < param_7 && ((*param_11)[uVar42] == '\0')))) {
      uVar34 = uVar34 + 1;
      (*param_11)[uVar42] = 1;
    }
    bVar19 = DAT_00112830;
    bVar20 = UNK_00112831;
    bVar21 = UNK_00112832;
    bVar22 = UNK_00112833;
    uVar23 = UNK_00112834;
    uVar24 = UNK_00112835;
    uVar25 = UNK_00112836;
    uVar26 = UNK_00112837;
    bVar177 = UNK_00112838;
    bVar179 = UNK_00112839;
    bVar181 = UNK_0011283a;
    bVar183 = UNK_0011283b;
    uVar185 = UNK_0011283c;
    uVar186 = UNK_0011283d;
    uVar187 = UNK_0011283e;
    uVar188 = UNK_0011283f;
    uVar27 = _DAT_001128f0;
    uVar28 = _UNK_001128f8;
    auVar102 = _DAT_00112ad0;
    uVar29 = _DAT_00112af0;
    uVar30 = _UNK_00112af8;
    auVar31 = _DAT_00112b20;
    if ((-1 < (int)param_9) && (param_10 != 0)) {
      uVar42 = uVar34 + 1;
      uVar50 = uVar42;
      if (uVar42 < param_8) {
        iVar32 = 0;
        if (param_7 != 0) {
          iVar32 = (int)(param_9 + 1) / (int)param_7;
        }
        uVar44 = (param_9 + 1) - iVar32 * param_7;
        if (((uVar44 < param_7) && ((*param_11)[uVar44] == '\0')) && (uVar34 < param_8)) {
          (*param_11)[uVar44] = 1;
          uVar50 = uVar34 + 2;
          uVar34 = uVar42;
        }
      }
      bVar19 = DAT_00112830;
      bVar20 = UNK_00112831;
      bVar21 = UNK_00112832;
      bVar22 = UNK_00112833;
      uVar23 = UNK_00112834;
      uVar24 = UNK_00112835;
      uVar25 = UNK_00112836;
      uVar26 = UNK_00112837;
      bVar177 = UNK_00112838;
      bVar179 = UNK_00112839;
      bVar181 = UNK_0011283a;
      bVar183 = UNK_0011283b;
      uVar185 = UNK_0011283c;
      uVar186 = UNK_0011283d;
      uVar187 = UNK_0011283e;
      uVar188 = UNK_0011283f;
      uVar27 = _DAT_001128f0;
      uVar28 = _UNK_001128f8;
      auVar102 = _DAT_00112ad0;
      uVar29 = _DAT_00112af0;
      uVar30 = _UNK_00112af8;
      auVar31 = _DAT_00112b20;
      if (uVar50 < param_8) {
        iVar33 = param_7 + param_9 + -1;
        iVar32 = 0;
        if (param_7 != 0) {
          iVar32 = iVar33 / (int)param_7;
        }
        uVar42 = iVar33 - iVar32 * param_7;
        if (((-1 < (int)uVar42) && (uVar42 < param_7)) &&
           (((*param_11)[uVar42] == '\0' && (uVar34 < param_8)))) {
          (*param_11)[uVar42] = 1;
          uVar34 = uVar50;
          bVar19 = DAT_00112830;
          bVar20 = UNK_00112831;
          bVar21 = UNK_00112832;
          bVar22 = UNK_00112833;
          uVar23 = UNK_00112834;
          uVar24 = UNK_00112835;
          uVar25 = UNK_00112836;
          uVar26 = UNK_00112837;
          bVar177 = UNK_00112838;
          bVar179 = UNK_00112839;
          bVar181 = UNK_0011283a;
          bVar183 = UNK_0011283b;
          uVar185 = UNK_0011283c;
          uVar186 = UNK_0011283d;
          uVar187 = UNK_0011283e;
          uVar188 = UNK_0011283f;
          uVar27 = _DAT_001128f0;
          uVar28 = _UNK_001128f8;
          auVar102 = _DAT_00112ad0;
          uVar29 = _DAT_00112af0;
          uVar30 = _UNK_00112af8;
          auVar31 = _DAT_00112b20;
        }
      }
    }
    DAT_00112830 = bVar19;
    UNK_00112831 = bVar20;
    UNK_00112832 = bVar21;
    UNK_00112833 = bVar22;
    UNK_00112834 = uVar23;
    UNK_00112835 = uVar24;
    UNK_00112836 = uVar25;
    UNK_00112837 = uVar26;
    UNK_00112838 = bVar177;
    UNK_00112839 = bVar179;
    UNK_0011283a = bVar181;
    UNK_0011283b = bVar183;
    UNK_0011283c = uVar185;
    UNK_0011283d = uVar186;
    UNK_0011283e = uVar187;
    UNK_0011283f = uVar188;
    _DAT_001128f0 = uVar27;
    _UNK_001128f8 = uVar28;
    _DAT_00112ad0 = auVar102;
    _DAT_00112af0 = uVar29;
    _UNK_00112af8 = uVar30;
    _DAT_00112b20 = auVar31;
    if (uVar34 < param_8) {
      uVar40 = uVar39 & 0xfffffff8;
      uVar35 = uVar39 & 0xfffffff0;
      bVar260 = (byte)_UNK_00112c68;
      bVar261 = (byte)((ulong)_UNK_00112c68 >> 8);
      bVar262 = (byte)((ulong)_UNK_00112c68 >> 0x10);
      bVar263 = (byte)((ulong)_UNK_00112c68 >> 0x18);
      uVar264 = (undefined1)((ulong)_UNK_00112c68 >> 0x20);
      uVar265 = (undefined1)((ulong)_UNK_00112c68 >> 0x28);
      uVar266 = (undefined1)((ulong)_UNK_00112c68 >> 0x30);
      uVar267 = (undefined1)((ulong)_UNK_00112c68 >> 0x38);
      bVar252 = (byte)_DAT_00112c60;
      bVar253 = (byte)((ulong)_DAT_00112c60 >> 8);
      bVar254 = (byte)((ulong)_DAT_00112c60 >> 0x10);
      bVar255 = (byte)((ulong)_DAT_00112c60 >> 0x18);
      uVar256 = (undefined1)((ulong)_DAT_00112c60 >> 0x20);
      uVar257 = (undefined1)((ulong)_DAT_00112c60 >> 0x28);
      uVar258 = (undefined1)((ulong)_DAT_00112c60 >> 0x30);
      uVar259 = (undefined1)((ulong)_DAT_00112c60 >> 0x38);
      do {
        iVar32 = 0;
        fVar101 = -1e+30;
        uVar46 = 0;
        uVar42 = 0;
        uVar41 = 0xffffffff;
        do {
          if ((*param_11)[uVar46] == '\0') {
            iVar33 = (int)uVar46;
            uVar50 = param_7;
            if (param_7 < 8) {
              uVar37 = 0;
LAB_001b4f48:
              uVar44 = iVar33 - (int)uVar37;
              do {
                uVar1 = uVar50;
                if ((*param_11)[uVar37] != '\0') {
                  uVar1 = uVar44;
                  if (uVar46 <= uVar37) {
                    uVar1 = iVar32 + (int)uVar37;
                  }
                  if (param_7 - uVar1 <= uVar1) {
                    uVar1 = param_7 - uVar1;
                  }
                  if (uVar50 <= uVar1) {
                    uVar1 = uVar50;
                  }
                }
                uVar50 = uVar1;
                uVar37 = uVar37 + 1;
                uVar44 = uVar44 - 1;
              } while (uVar39 != uVar37);
            }
            else if (param_7 < 0x10) {
              uVar37 = 0;
LAB_001b4e64:
              auVar121._0_8_ = CONCAT44(uVar50,uVar50);
              auVar121._8_4_ = uVar50;
              auVar121._12_4_ = uVar50;
              lVar36 = uVar37 - uVar40;
              bVar47 = (byte)uVar37;
              bVar126 = bVar47 | bVar252;
              bVar57 = (byte)(uVar37 >> 8);
              bVar128 = bVar57 | bVar253;
              bVar58 = (byte)(uVar37 >> 0x10);
              bVar130 = bVar58 | bVar254;
              bVar59 = (byte)(uVar37 >> 0x18);
              bVar132 = bVar59 | bVar255;
              bVar137 = bVar47 | bVar260;
              bVar138 = bVar57 | bVar261;
              bVar139 = bVar58 | bVar262;
              bVar140 = bVar59 | bVar263;
              bVar150 = bVar47 | auVar31[0];
              bVar152 = bVar57 | auVar31[1];
              bVar154 = bVar58 | auVar31[2];
              bVar156 = bVar59 | auVar31[3];
              uVar141 = auVar31[4];
              uVar143 = auVar31[5];
              uVar145 = auVar31[6];
              uVar147 = auVar31[7];
              bVar162 = bVar47 | auVar31[8];
              bVar164 = bVar57 | auVar31[9];
              bVar166 = bVar58 | auVar31[10];
              bVar168 = bVar59 | auVar31[0xb];
              uVar149 = auVar31[0xc];
              uVar151 = auVar31[0xd];
              uVar153 = auVar31[0xe];
              uVar155 = auVar31[0xf];
              bVar173 = bVar47 | auVar102[0];
              bVar174 = bVar57 | auVar102[1];
              bVar175 = bVar58 | auVar102[2];
              bVar176 = bVar59 | auVar102[3];
              uVar93 = auVar102[4];
              uVar94 = auVar102[5];
              uVar95 = auVar102[6];
              uVar96 = auVar102[7];
              bVar178 = bVar47 | auVar102[8];
              bVar180 = bVar57 | auVar102[9];
              bVar182 = bVar58 | auVar102[10];
              bVar184 = bVar59 | auVar102[0xb];
              uVar97 = auVar102[0xc];
              uVar98 = auVar102[0xd];
              uVar99 = auVar102[0xe];
              uVar100 = auVar102[0xf];
              bVar190 = bVar47 | bVar19;
              bVar192 = bVar57 | bVar20;
              bVar194 = bVar58 | bVar21;
              bVar196 = bVar59 | bVar22;
              bVar206 = bVar47 | bVar177;
              bVar208 = bVar57 | bVar179;
              bVar210 = bVar58 | bVar181;
              bVar212 = bVar59 | bVar183;
              auVar217._8_8_ = auVar121._8_8_;
              auVar217._0_8_ = auVar121._0_8_;
              bVar220 = bVar47 | (byte)uVar27;
              bVar222 = bVar57 | (byte)((ulong)uVar27 >> 8);
              bVar224 = bVar58 | (byte)((ulong)uVar27 >> 0x10);
              bVar226 = bVar59 | (byte)((ulong)uVar27 >> 0x18);
              bVar228 = bVar47 | (byte)((ulong)uVar27 >> 0x20);
              bVar230 = bVar57 | (byte)((ulong)uVar27 >> 0x28);
              bVar232 = bVar58 | (byte)((ulong)uVar27 >> 0x30);
              bVar234 = bVar59 | (byte)((ulong)uVar27 >> 0x38);
              bVar236 = bVar47 | (byte)uVar28;
              bVar238 = bVar57 | (byte)((ulong)uVar28 >> 8);
              bVar240 = bVar58 | (byte)((ulong)uVar28 >> 0x10);
              bVar242 = bVar59 | (byte)((ulong)uVar28 >> 0x18);
              bVar244 = bVar47 | (byte)((ulong)uVar28 >> 0x20);
              bVar246 = bVar57 | (byte)((ulong)uVar28 >> 0x28);
              bVar248 = bVar58 | (byte)((ulong)uVar28 >> 0x30);
              bVar250 = bVar59 | (byte)((ulong)uVar28 >> 0x38);
              auVar251[0] = bVar47 | (byte)uVar29;
              auVar251[1] = bVar57 | (byte)((ulong)uVar29 >> 8);
              auVar251[2] = bVar58 | (byte)((ulong)uVar29 >> 0x10);
              auVar251[3] = bVar59 | (byte)((ulong)uVar29 >> 0x18);
              auVar251[4] = bVar47 | (byte)((ulong)uVar29 >> 0x20);
              auVar251[5] = bVar57 | (byte)((ulong)uVar29 >> 0x28);
              auVar251[6] = bVar58 | (byte)((ulong)uVar29 >> 0x30);
              auVar251[7] = bVar59 | (byte)((ulong)uVar29 >> 0x38);
              auVar251[8] = bVar47 | (byte)uVar30;
              auVar251[9] = bVar57 | (byte)((ulong)uVar30 >> 8);
              auVar251[10] = bVar58 | (byte)((ulong)uVar30 >> 0x10);
              auVar251[0xb] = bVar59 | (byte)((ulong)uVar30 >> 0x18);
              auVar251[0xc] = bVar47 | (byte)((ulong)uVar30 >> 0x20);
              auVar251[0xd] = bVar57 | (byte)((ulong)uVar30 >> 0x28);
              auVar251[0xe] = bVar58 | (byte)((ulong)uVar30 >> 0x30);
              auVar251[0xf] = bVar59 | (byte)((ulong)uVar30 >> 0x38);
              puVar38 = (undefined8 *)(*param_11 + uVar37);
              uVar133 = uVar256;
              uVar134 = uVar257;
              uVar135 = uVar258;
              uVar136 = uVar259;
              uVar142 = uVar264;
              uVar144 = uVar265;
              uVar146 = uVar266;
              uVar148 = uVar267;
              uVar198 = uVar23;
              uVar200 = uVar24;
              uVar202 = uVar25;
              uVar204 = uVar26;
              uVar125 = uVar185;
              uVar127 = uVar186;
              uVar129 = uVar187;
              uVar131 = uVar188;
              do {
                auVar2._8_8_ = uVar46;
                auVar2._0_8_ = uVar46;
                auVar9[1] = bVar152;
                auVar9[0] = bVar150;
                auVar9[2] = bVar154;
                auVar9[3] = bVar156;
                auVar9[4] = uVar141;
                auVar9[5] = uVar143;
                auVar9[6] = uVar145;
                auVar9[7] = uVar147;
                auVar9[8] = bVar162;
                auVar9[9] = bVar164;
                auVar9[10] = bVar166;
                auVar9[0xb] = bVar168;
                auVar9[0xc] = uVar149;
                auVar9[0xd] = uVar151;
                auVar9[0xe] = uVar153;
                auVar9[0xf] = uVar155;
                auVar55 = NEON_cmhi(auVar2,auVar9,8);
                lVar36 = lVar36 + 8;
                auVar3._8_8_ = uVar46;
                auVar3._0_8_ = uVar46;
                auVar8[1] = bVar128;
                auVar8[0] = bVar126;
                auVar8[2] = bVar130;
                auVar8[3] = bVar132;
                auVar8[4] = uVar133;
                auVar8[5] = uVar134;
                auVar8[6] = uVar135;
                auVar8[7] = uVar136;
                auVar8[8] = bVar137;
                auVar8[9] = bVar138;
                auVar8[10] = bVar139;
                auVar8[0xb] = bVar140;
                auVar8[0xc] = uVar142;
                auVar8[0xd] = uVar144;
                auVar8[0xe] = uVar146;
                auVar8[0xf] = uVar148;
                auVar65 = NEON_cmhi(auVar3,auVar8,8);
                auVar4._8_8_ = uVar46;
                auVar4._0_8_ = uVar46;
                auVar13[1] = bVar192;
                auVar13[0] = bVar190;
                auVar13[2] = bVar194;
                auVar13[3] = bVar196;
                auVar13[4] = uVar198;
                auVar13[5] = uVar200;
                auVar13[6] = uVar202;
                auVar13[7] = uVar204;
                auVar13[8] = bVar206;
                auVar13[9] = bVar208;
                auVar13[10] = bVar210;
                auVar13[0xb] = bVar212;
                auVar13[0xc] = uVar125;
                auVar13[0xd] = uVar127;
                auVar13[0xe] = uVar129;
                auVar13[0xf] = uVar131;
                auVar73 = NEON_cmhi(auVar4,auVar13,8);
                auVar5._8_8_ = uVar46;
                auVar5._0_8_ = uVar46;
                auVar12[1] = bVar174;
                auVar12[0] = bVar173;
                auVar12[2] = bVar175;
                auVar12[3] = bVar176;
                auVar12[4] = uVar93;
                auVar12[5] = uVar94;
                auVar12[6] = uVar95;
                auVar12[7] = uVar96;
                auVar12[8] = bVar178;
                auVar12[9] = bVar180;
                auVar12[10] = bVar182;
                auVar12[0xb] = bVar184;
                auVar12[0xc] = uVar97;
                auVar12[0xd] = uVar98;
                auVar12[0xe] = uVar99;
                auVar12[0xf] = uVar100;
                auVar82 = NEON_cmhi(auVar5,auVar12,8);
                auVar56._0_4_ = auVar55._0_4_;
                auVar56._4_4_ = auVar55._8_4_;
                auVar56._8_4_ = auVar65._0_4_;
                auVar56._12_4_ = auVar65._8_4_;
                auVar66._4_4_ = auVar73._8_4_;
                auVar66._0_4_ = auVar73._0_4_;
                auVar66._8_4_ = auVar82._0_4_;
                auVar66._12_4_ = auVar82._8_4_;
                auVar74._0_4_ =
                     iVar33 - CONCAT13(bVar226,CONCAT12(bVar224,CONCAT11(bVar222,bVar220)));
                auVar74._4_4_ =
                     iVar33 - CONCAT13(bVar234,CONCAT12(bVar232,CONCAT11(bVar230,bVar228)));
                auVar74._8_4_ =
                     iVar33 - CONCAT13(bVar242,CONCAT12(bVar240,CONCAT11(bVar238,bVar236)));
                auVar74._12_4_ =
                     iVar33 - CONCAT13(bVar250,CONCAT12(bVar248,CONCAT11(bVar246,bVar244)));
                iVar104 = auVar251._0_4_;
                auVar83._0_4_ = iVar33 - iVar104;
                iVar105 = auVar251._4_4_;
                auVar83._4_4_ = iVar33 - iVar105;
                iVar106 = auVar251._8_4_;
                auVar83._8_4_ = iVar33 - iVar106;
                iVar108 = auVar251._12_4_;
                auVar83._12_4_ = iVar33 - iVar108;
                auVar90._0_4_ = iVar104 - iVar33;
                auVar90._4_4_ = iVar105 - iVar33;
                auVar90._8_4_ = iVar106 - iVar33;
                auVar90._12_4_ = iVar108 - iVar33;
                auVar65 = NEON_bsl(auVar66,auVar83,auVar90,1);
                auVar16._4_4_ =
                     CONCAT13(bVar234,CONCAT12(bVar232,CONCAT11(bVar230,bVar228))) - iVar33;
                auVar16._0_4_ =
                     CONCAT13(bVar226,CONCAT12(bVar224,CONCAT11(bVar222,bVar220))) - iVar33;
                auVar16._8_4_ =
                     CONCAT13(bVar242,CONCAT12(bVar240,CONCAT11(bVar238,bVar236))) - iVar33;
                auVar16._12_4_ =
                     CONCAT13(bVar250,CONCAT12(bVar248,CONCAT11(bVar246,bVar244))) - iVar33;
                auVar55 = NEON_bsl(auVar56,auVar74,auVar16,1);
                uVar67 = *puVar38;
                auVar251._0_4_ = iVar104 + 8;
                auVar251._4_4_ = iVar105 + 8;
                auVar251._8_4_ = iVar106 + 8;
                auVar251._12_4_ = iVar108 + 8;
                auVar84._0_4_ = param_7 - auVar65._0_4_;
                auVar84._4_4_ = param_7 - auVar65._4_4_;
                auVar84._8_4_ = param_7 - auVar65._8_4_;
                auVar84._12_4_ = param_7 - auVar65._12_4_;
                auVar91._0_4_ = param_7 - auVar55._0_4_;
                auVar91._4_4_ = param_7 - auVar55._4_4_;
                auVar91._8_4_ = param_7 - auVar55._8_4_;
                auVar91._12_4_ = param_7 - auVar55._12_4_;
                auVar65 = NEON_umin(auVar65,auVar84,4);
                auVar55 = NEON_umin(auVar55,auVar91,4);
                auVar75[1] = 0;
                auVar75[0] = (byte)uVar67;
                auVar75[2] = (char)((ulong)uVar67 >> 8);
                auVar75[3] = 0;
                auVar75[4] = (char)((ulong)uVar67 >> 0x10);
                auVar75[5] = 0;
                auVar75[6] = (char)((ulong)uVar67 >> 0x18);
                auVar75[7] = 0;
                auVar75[8] = (char)((ulong)uVar67 >> 0x20);
                auVar75[9] = 0;
                auVar75[10] = (char)((ulong)uVar67 >> 0x28);
                auVar75[0xb] = 0;
                auVar75[0xc] = (char)((ulong)uVar67 >> 0x30);
                auVar75[0xd] = 0;
                auVar75[0xe] = (char)((ulong)uVar67 >> 0x38);
                auVar75[0xf] = 0;
                iVar104 = CONCAT13(bVar226,CONCAT12(bVar224,CONCAT11(bVar222,bVar220))) + 8;
                bVar220 = (byte)iVar104;
                bVar222 = (byte)((uint)iVar104 >> 8);
                bVar224 = (byte)((uint)iVar104 >> 0x10);
                bVar226 = (byte)((uint)iVar104 >> 0x18);
                iVar104 = CONCAT13(bVar234,CONCAT12(bVar232,CONCAT11(bVar230,bVar228))) + 8;
                bVar228 = (byte)iVar104;
                bVar230 = (byte)((uint)iVar104 >> 8);
                bVar232 = (byte)((uint)iVar104 >> 0x10);
                bVar234 = (byte)((uint)iVar104 >> 0x18);
                iVar104 = CONCAT13(bVar242,CONCAT12(bVar240,CONCAT11(bVar238,bVar236))) + 8;
                bVar236 = (byte)iVar104;
                bVar238 = (byte)((uint)iVar104 >> 8);
                bVar240 = (byte)((uint)iVar104 >> 0x10);
                bVar242 = (byte)((uint)iVar104 >> 0x18);
                iVar104 = CONCAT13(bVar250,CONCAT12(bVar248,CONCAT11(bVar246,bVar244))) + 8;
                bVar244 = (byte)iVar104;
                bVar246 = (byte)((uint)iVar104 >> 8);
                bVar248 = (byte)((uint)iVar104 >> 0x10);
                bVar250 = (byte)((uint)iVar104 >> 0x18);
                auVar73 = NEON_cmeq(auVar75,0,2);
                auVar65 = NEON_umin(auVar65,auVar121,4);
                auVar55 = NEON_umin(auVar55,auVar217,4);
                auVar85._0_4_ = (int)auVar73._0_2_;
                auVar85._4_4_ = (int)auVar73._2_2_;
                auVar85._8_4_ = (int)auVar73._4_2_;
                auVar85._12_4_ = (int)auVar73._6_2_;
                auVar76._0_4_ = (int)auVar73._8_2_;
                auVar76._4_4_ = (int)auVar73._10_2_;
                auVar76._8_4_ = (int)auVar73._12_2_;
                auVar76._12_4_ = (int)auVar73._14_2_;
                auVar121 = NEON_bif(auVar121,auVar65,auVar85,1);
                auVar217 = NEON_bif(auVar217,auVar55,auVar76,1);
                lVar11 = CONCAT17(uVar147,CONCAT16(uVar145,CONCAT15(uVar143,CONCAT14(uVar141,
                                                  CONCAT13(bVar156,CONCAT12(bVar154,CONCAT11(bVar152
                                                  ,bVar150))))))) + 8;
                bVar150 = (byte)lVar11;
                bVar152 = (byte)((ulong)lVar11 >> 8);
                bVar154 = (byte)((ulong)lVar11 >> 0x10);
                bVar156 = (byte)((ulong)lVar11 >> 0x18);
                uVar141 = (undefined1)((ulong)lVar11 >> 0x20);
                uVar143 = (undefined1)((ulong)lVar11 >> 0x28);
                uVar145 = (undefined1)((ulong)lVar11 >> 0x30);
                uVar147 = (undefined1)((ulong)lVar11 >> 0x38);
                lVar11 = CONCAT17(uVar155,CONCAT16(uVar153,CONCAT15(uVar151,CONCAT14(uVar149,
                                                  CONCAT13(bVar168,CONCAT12(bVar166,CONCAT11(bVar164
                                                  ,bVar162))))))) + 8;
                bVar162 = (byte)lVar11;
                bVar164 = (byte)((ulong)lVar11 >> 8);
                bVar166 = (byte)((ulong)lVar11 >> 0x10);
                bVar168 = (byte)((ulong)lVar11 >> 0x18);
                uVar149 = (undefined1)((ulong)lVar11 >> 0x20);
                uVar151 = (undefined1)((ulong)lVar11 >> 0x28);
                uVar153 = (undefined1)((ulong)lVar11 >> 0x30);
                uVar155 = (undefined1)((ulong)lVar11 >> 0x38);
                lVar11 = CONCAT17(uVar96,CONCAT16(uVar95,CONCAT15(uVar94,CONCAT14(uVar93,CONCAT13(
                                                  bVar176,CONCAT12(bVar175,CONCAT11(bVar174,bVar173)
                                                                  )))))) + 8;
                bVar173 = (byte)lVar11;
                bVar174 = (byte)((ulong)lVar11 >> 8);
                bVar175 = (byte)((ulong)lVar11 >> 0x10);
                bVar176 = (byte)((ulong)lVar11 >> 0x18);
                uVar93 = (undefined1)((ulong)lVar11 >> 0x20);
                uVar94 = (undefined1)((ulong)lVar11 >> 0x28);
                uVar95 = (undefined1)((ulong)lVar11 >> 0x30);
                uVar96 = (undefined1)((ulong)lVar11 >> 0x38);
                lVar11 = CONCAT17(uVar100,CONCAT16(uVar99,CONCAT15(uVar98,CONCAT14(uVar97,CONCAT13(
                                                  bVar184,CONCAT12(bVar182,CONCAT11(bVar180,bVar178)
                                                                  )))))) + 8;
                bVar178 = (byte)lVar11;
                bVar180 = (byte)((ulong)lVar11 >> 8);
                bVar182 = (byte)((ulong)lVar11 >> 0x10);
                bVar184 = (byte)((ulong)lVar11 >> 0x18);
                uVar97 = (undefined1)((ulong)lVar11 >> 0x20);
                uVar98 = (undefined1)((ulong)lVar11 >> 0x28);
                uVar99 = (undefined1)((ulong)lVar11 >> 0x30);
                uVar100 = (undefined1)((ulong)lVar11 >> 0x38);
                lVar11 = CONCAT17(uVar204,CONCAT16(uVar202,CONCAT15(uVar200,CONCAT14(uVar198,
                                                  CONCAT13(bVar196,CONCAT12(bVar194,CONCAT11(bVar192
                                                  ,bVar190))))))) + 8;
                bVar190 = (byte)lVar11;
                bVar192 = (byte)((ulong)lVar11 >> 8);
                bVar194 = (byte)((ulong)lVar11 >> 0x10);
                bVar196 = (byte)((ulong)lVar11 >> 0x18);
                uVar198 = (undefined1)((ulong)lVar11 >> 0x20);
                uVar200 = (undefined1)((ulong)lVar11 >> 0x28);
                uVar202 = (undefined1)((ulong)lVar11 >> 0x30);
                uVar204 = (undefined1)((ulong)lVar11 >> 0x38);
                lVar11 = CONCAT17(uVar131,CONCAT16(uVar129,CONCAT15(uVar127,CONCAT14(uVar125,
                                                  CONCAT13(bVar212,CONCAT12(bVar210,CONCAT11(bVar208
                                                  ,bVar206))))))) + 8;
                bVar206 = (byte)lVar11;
                bVar208 = (byte)((ulong)lVar11 >> 8);
                bVar210 = (byte)((ulong)lVar11 >> 0x10);
                bVar212 = (byte)((ulong)lVar11 >> 0x18);
                uVar125 = (undefined1)((ulong)lVar11 >> 0x20);
                uVar127 = (undefined1)((ulong)lVar11 >> 0x28);
                uVar129 = (undefined1)((ulong)lVar11 >> 0x30);
                uVar131 = (undefined1)((ulong)lVar11 >> 0x38);
                lVar11 = CONCAT17(uVar136,CONCAT16(uVar135,CONCAT15(uVar134,CONCAT14(uVar133,
                                                  CONCAT13(bVar132,CONCAT12(bVar130,CONCAT11(bVar128
                                                  ,bVar126))))))) + 8;
                bVar126 = (byte)lVar11;
                bVar128 = (byte)((ulong)lVar11 >> 8);
                bVar130 = (byte)((ulong)lVar11 >> 0x10);
                bVar132 = (byte)((ulong)lVar11 >> 0x18);
                uVar133 = (undefined1)((ulong)lVar11 >> 0x20);
                uVar134 = (undefined1)((ulong)lVar11 >> 0x28);
                uVar135 = (undefined1)((ulong)lVar11 >> 0x30);
                uVar136 = (undefined1)((ulong)lVar11 >> 0x38);
                lVar11 = CONCAT17(uVar148,CONCAT16(uVar146,CONCAT15(uVar144,CONCAT14(uVar142,
                                                  CONCAT13(bVar140,CONCAT12(bVar139,CONCAT11(bVar138
                                                  ,bVar137))))))) + 8;
                bVar137 = (byte)lVar11;
                bVar138 = (byte)((ulong)lVar11 >> 8);
                bVar139 = (byte)((ulong)lVar11 >> 0x10);
                bVar140 = (byte)((ulong)lVar11 >> 0x18);
                uVar142 = (undefined1)((ulong)lVar11 >> 0x20);
                uVar144 = (undefined1)((ulong)lVar11 >> 0x28);
                uVar146 = (undefined1)((ulong)lVar11 >> 0x30);
                uVar148 = (undefined1)((ulong)lVar11 >> 0x38);
                puVar38 = puVar38 + 1;
              } while (lVar36 != 0);
              auVar55 = NEON_umin(auVar121,auVar217,4);
              uVar50 = NEON_uminv(auVar55,4);
              uVar37 = uVar40;
              if (uVar40 != uVar39) goto LAB_001b4f48;
            }
            else {
              uVar97 = (undefined1)_UNK_001129a8;
              uVar98 = (undefined1)((ulong)_UNK_001129a8 >> 8);
              uVar99 = (undefined1)((ulong)_UNK_001129a8 >> 0x10);
              uVar100 = (undefined1)((ulong)_UNK_001129a8 >> 0x18);
              uVar141 = (undefined1)((ulong)_UNK_001129a8 >> 0x20);
              uVar143 = (undefined1)((ulong)_UNK_001129a8 >> 0x28);
              uVar145 = (undefined1)((ulong)_UNK_001129a8 >> 0x30);
              uVar147 = (undefined1)((ulong)_UNK_001129a8 >> 0x38);
              uVar125 = (undefined1)_DAT_001129a0;
              uVar127 = (undefined1)((ulong)_DAT_001129a0 >> 8);
              uVar129 = (undefined1)((ulong)_DAT_001129a0 >> 0x10);
              uVar131 = (undefined1)((ulong)_DAT_001129a0 >> 0x18);
              uVar93 = (undefined1)((ulong)_DAT_001129a0 >> 0x20);
              uVar94 = (undefined1)((ulong)_DAT_001129a0 >> 0x28);
              uVar95 = (undefined1)((ulong)_DAT_001129a0 >> 0x30);
              uVar96 = (undefined1)((ulong)_DAT_001129a0 >> 0x38);
              uVar161 = (undefined1)_UNK_00112968;
              uVar163 = (undefined1)((ulong)_UNK_00112968 >> 8);
              uVar165 = (undefined1)((ulong)_UNK_00112968 >> 0x10);
              uVar167 = (undefined1)((ulong)_UNK_00112968 >> 0x18);
              uVar169 = (undefined1)((ulong)_UNK_00112968 >> 0x20);
              uVar170 = (undefined1)((ulong)_UNK_00112968 >> 0x28);
              uVar171 = (undefined1)((ulong)_UNK_00112968 >> 0x30);
              uVar172 = (undefined1)((ulong)_UNK_00112968 >> 0x38);
              uVar149 = (undefined1)_DAT_00112960;
              uVar151 = (undefined1)((ulong)_DAT_00112960 >> 8);
              uVar153 = (undefined1)((ulong)_DAT_00112960 >> 0x10);
              uVar155 = (undefined1)((ulong)_DAT_00112960 >> 0x18);
              uVar157 = (undefined1)((ulong)_DAT_00112960 >> 0x20);
              uVar158 = (undefined1)((ulong)_DAT_00112960 >> 0x28);
              uVar159 = (undefined1)((ulong)_DAT_00112960 >> 0x30);
              uVar160 = (undefined1)((ulong)_DAT_00112960 >> 0x38);
              uVar205 = (undefined1)_UNK_00112b78;
              uVar207 = (undefined1)((ulong)_UNK_00112b78 >> 8);
              uVar209 = (undefined1)((ulong)_UNK_00112b78 >> 0x10);
              uVar211 = (undefined1)((ulong)_UNK_00112b78 >> 0x18);
              uVar213 = (undefined1)((ulong)_UNK_00112b78 >> 0x20);
              uVar214 = (undefined1)((ulong)_UNK_00112b78 >> 0x28);
              uVar215 = (undefined1)((ulong)_UNK_00112b78 >> 0x30);
              uVar216 = (undefined1)((ulong)_UNK_00112b78 >> 0x38);
              uVar189 = (undefined1)_DAT_00112b70;
              uVar191 = (undefined1)((ulong)_DAT_00112b70 >> 8);
              uVar193 = (undefined1)((ulong)_DAT_00112b70 >> 0x10);
              uVar195 = (undefined1)((ulong)_DAT_00112b70 >> 0x18);
              uVar197 = (undefined1)((ulong)_DAT_00112b70 >> 0x20);
              uVar199 = (undefined1)((ulong)_DAT_00112b70 >> 0x28);
              uVar201 = (undefined1)((ulong)_DAT_00112b70 >> 0x30);
              uVar203 = (undefined1)((ulong)_DAT_00112b70 >> 0x38);
              uVar235 = (undefined1)_UNK_001129c8;
              uVar237 = (undefined1)((ulong)_UNK_001129c8 >> 8);
              uVar239 = (undefined1)((ulong)_UNK_001129c8 >> 0x10);
              uVar241 = (undefined1)((ulong)_UNK_001129c8 >> 0x18);
              uVar243 = (undefined1)((ulong)_UNK_001129c8 >> 0x20);
              uVar245 = (undefined1)((ulong)_UNK_001129c8 >> 0x28);
              uVar247 = (undefined1)((ulong)_UNK_001129c8 >> 0x30);
              uVar249 = (undefined1)((ulong)_UNK_001129c8 >> 0x38);
              uVar219 = (undefined1)_DAT_001129c0;
              uVar221 = (undefined1)((ulong)_DAT_001129c0 >> 8);
              uVar223 = (undefined1)((ulong)_DAT_001129c0 >> 0x10);
              uVar225 = (undefined1)((ulong)_DAT_001129c0 >> 0x18);
              uVar227 = (undefined1)((ulong)_DAT_001129c0 >> 0x20);
              uVar229 = (undefined1)((ulong)_DAT_001129c0 >> 0x28);
              uVar231 = (undefined1)((ulong)_DAT_001129c0 >> 0x30);
              uVar233 = (undefined1)((ulong)_DAT_001129c0 >> 0x38);
              auVar268._12_4_ = param_7;
              auVar268._8_4_ = param_7;
              auVar268._4_4_ = param_7;
              auVar268._0_4_ = param_7;
              auVar55._4_4_ = param_7;
              auVar55._0_4_ = param_7;
              auVar55._8_4_ = param_7;
              auVar55._12_4_ = param_7;
              auVar92._12_4_ = param_7;
              auVar92._8_4_ = param_7;
              auVar92._4_4_ = param_7;
              auVar92._0_4_ = param_7;
              auVar103._12_4_ = param_7;
              auVar103._8_4_ = param_7;
              auVar103._4_4_ = param_7;
              auVar103._0_4_ = param_7;
              uVar37 = uVar35;
              pauVar43 = param_11;
              uVar67 = uVar29;
              uVar107 = uVar30;
              uVar110 = uVar27;
              uVar113 = uVar28;
              auVar65 = _DAT_00112ae0;
              bVar47 = bVar19;
              bVar57 = bVar20;
              bVar58 = bVar21;
              bVar59 = bVar22;
              uVar133 = uVar23;
              uVar134 = uVar24;
              uVar135 = uVar25;
              uVar136 = uVar26;
              auVar73 = auVar102;
              auVar82 = auVar31;
              bVar126 = bVar252;
              bVar128 = bVar253;
              bVar130 = bVar254;
              bVar132 = bVar255;
              uVar142 = uVar256;
              uVar144 = uVar257;
              uVar146 = uVar258;
              uVar148 = uVar259;
              bVar137 = bVar260;
              bVar138 = bVar261;
              bVar139 = bVar262;
              bVar140 = bVar263;
              uVar198 = uVar264;
              uVar200 = uVar265;
              uVar202 = uVar266;
              uVar204 = uVar267;
              lVar36 = _DAT_001129b0;
              lVar11 = _UNK_001129b8;
              do {
                auVar51._8_8_ = uVar46;
                auVar51._0_8_ = uVar46;
                auVar51 = NEON_cmhi(auVar51,auVar82,8);
                uVar37 = uVar37 - 0x10;
                auVar61._8_8_ = uVar46;
                auVar61._0_8_ = uVar46;
                auVar18[1] = bVar128;
                auVar18[0] = bVar126;
                auVar18[2] = bVar130;
                auVar18[3] = bVar132;
                auVar18[4] = uVar142;
                auVar18[5] = uVar144;
                auVar18[6] = uVar146;
                auVar18[7] = uVar148;
                auVar18[8] = bVar137;
                auVar18[9] = bVar138;
                auVar18[10] = bVar139;
                auVar18[0xb] = bVar140;
                auVar18[0xc] = uVar198;
                auVar18[0xd] = uVar200;
                auVar18[0xe] = uVar202;
                auVar18[0xf] = uVar204;
                auVar61 = NEON_cmhi(auVar61,auVar18,8);
                auVar68._8_8_ = uVar46;
                auVar68._0_8_ = uVar46;
                auVar118[1] = bVar57;
                auVar118[0] = bVar47;
                auVar118[2] = bVar58;
                auVar118[3] = bVar59;
                auVar118[4] = uVar133;
                auVar118[5] = uVar134;
                auVar118[6] = uVar135;
                auVar118[7] = uVar136;
                auVar118[8] = bVar177;
                auVar118[9] = bVar179;
                auVar118[10] = bVar181;
                auVar118[0xb] = bVar183;
                auVar118[0xc] = uVar185;
                auVar118[0xd] = uVar186;
                auVar118[0xe] = uVar187;
                auVar118[0xf] = uVar188;
                auVar68 = NEON_cmhi(auVar68,auVar118,8);
                auVar77._8_8_ = uVar46;
                auVar77._0_8_ = uVar46;
                auVar77 = NEON_cmhi(auVar77,auVar73,8);
                auVar52._0_4_ = auVar51._0_4_;
                auVar52._4_4_ = auVar51._8_4_;
                auVar52._8_4_ = auVar61._0_4_;
                auVar52._12_4_ = auVar61._8_4_;
                auVar62._4_4_ = auVar68._8_4_;
                auVar62._0_4_ = auVar68._0_4_;
                auVar62._8_4_ = auVar77._0_4_;
                auVar62._12_4_ = auVar77._8_4_;
                iVar104 = (int)uVar67;
                auVar69._0_4_ = iVar33 - iVar104;
                iVar105 = (int)((ulong)uVar67 >> 0x20);
                auVar69._4_4_ = iVar33 - iVar105;
                iVar106 = (int)uVar107;
                auVar69._8_4_ = iVar33 - iVar106;
                iVar108 = (int)((ulong)uVar107 >> 0x20);
                auVar69._12_4_ = iVar33 - iVar108;
                auVar78._0_4_ = iVar104 - iVar33;
                auVar78._4_4_ = iVar105 - iVar33;
                auVar78._8_4_ = iVar106 - iVar33;
                auVar78._12_4_ = iVar108 - iVar33;
                auVar61 = NEON_bsl(auVar62,auVar69,auVar78,1);
                iVar109 = (int)uVar110;
                auVar70._0_4_ = iVar33 - iVar109;
                iVar111 = (int)((ulong)uVar110 >> 0x20);
                auVar70._4_4_ = iVar33 - iVar111;
                iVar112 = (int)uVar113;
                auVar70._8_4_ = iVar33 - iVar112;
                iVar114 = (int)((ulong)uVar113 >> 0x20);
                auVar70._12_4_ = iVar33 - iVar114;
                auVar79._0_4_ = iVar109 - iVar33;
                auVar79._4_4_ = iVar111 - iVar33;
                auVar79._8_4_ = iVar112 - iVar33;
                auVar79._12_4_ = iVar114 - iVar33;
                auVar115._8_8_ = uVar46;
                auVar115._0_8_ = uVar46;
                auVar17._8_4_ = (int)lVar11;
                auVar17._0_8_ = lVar36;
                auVar17._12_4_ = (int)((ulong)lVar11 >> 0x20);
                auVar118 = NEON_cmhi(auVar115,auVar17,8);
                auVar51 = NEON_bsl(auVar52,auVar70,auVar79,1);
                auVar71._0_4_ = param_7 - auVar61._0_4_;
                auVar71._4_4_ = param_7 - auVar61._4_4_;
                auVar71._8_4_ = param_7 - auVar61._8_4_;
                auVar71._12_4_ = param_7 - auVar61._12_4_;
                auVar117._8_8_ = uVar46;
                auVar117._0_8_ = uVar46;
                auVar15[1] = uVar221;
                auVar15[0] = uVar219;
                auVar15[2] = uVar223;
                auVar15[3] = uVar225;
                auVar15[4] = uVar227;
                auVar15[5] = uVar229;
                auVar15[6] = uVar231;
                auVar15[7] = uVar233;
                auVar15[8] = uVar235;
                auVar15[9] = uVar237;
                auVar15[10] = uVar239;
                auVar15[0xb] = uVar241;
                auVar15[0xc] = uVar243;
                auVar15[0xd] = uVar245;
                auVar15[0xe] = uVar247;
                auVar15[0xf] = uVar249;
                auVar68 = NEON_cmhi(auVar117,auVar15,8);
                auVar61 = NEON_umin(auVar61,auVar71,4);
                auVar6._8_8_ = uVar46;
                auVar6._0_8_ = uVar46;
                auVar14[1] = uVar191;
                auVar14[0] = uVar189;
                auVar14[2] = uVar193;
                auVar14[3] = uVar195;
                auVar14[4] = uVar197;
                auVar14[5] = uVar199;
                auVar14[6] = uVar201;
                auVar14[7] = uVar203;
                auVar14[8] = uVar205;
                auVar14[9] = uVar207;
                auVar14[10] = uVar209;
                auVar14[0xb] = uVar211;
                auVar14[0xc] = uVar213;
                auVar14[0xd] = uVar214;
                auVar14[0xe] = uVar215;
                auVar14[0xf] = uVar216;
                auVar77 = NEON_cmhi(auVar6,auVar14,8);
                auVar7._8_8_ = uVar46;
                auVar7._0_8_ = uVar46;
                auVar10[1] = uVar151;
                auVar10[0] = uVar149;
                auVar10[2] = uVar153;
                auVar10[3] = uVar155;
                auVar10[4] = uVar157;
                auVar10[5] = uVar158;
                auVar10[6] = uVar159;
                auVar10[7] = uVar160;
                auVar10[8] = uVar161;
                auVar10[9] = uVar163;
                auVar10[10] = uVar165;
                auVar10[0xb] = uVar167;
                auVar10[0xc] = uVar169;
                auVar10[0xd] = uVar170;
                auVar10[0xe] = uVar171;
                auVar10[0xf] = uVar172;
                auVar117 = NEON_cmhi(auVar7,auVar10,8);
                auVar115 = NEON_cmeq(*pauVar43,0,1);
                auVar86._4_4_ = auVar118._8_4_;
                auVar86._0_4_ = auVar118._0_4_;
                auVar86._8_4_ = auVar68._0_4_;
                auVar86._12_4_ = auVar68._8_4_;
                auVar116._0_4_ = auVar77._0_4_;
                auVar116._4_4_ = auVar77._8_4_;
                auVar116._8_4_ = auVar117._0_4_;
                auVar116._12_4_ = auVar117._8_4_;
                auVar80._0_4_ =
                     iVar33 - CONCAT13(uVar131,CONCAT12(uVar129,CONCAT11(uVar127,uVar125)));
                auVar80._4_4_ = iVar33 - CONCAT13(uVar96,CONCAT12(uVar95,CONCAT11(uVar94,uVar93)));
                auVar80._8_4_ = iVar33 - CONCAT13(uVar100,CONCAT12(uVar99,CONCAT11(uVar98,uVar97)));
                auVar80._12_4_ =
                     iVar33 - CONCAT13(uVar147,CONCAT12(uVar145,CONCAT11(uVar143,uVar141)));
                auVar72._0_4_ =
                     CONCAT13(uVar131,CONCAT12(uVar129,CONCAT11(uVar127,uVar125))) - iVar33;
                auVar72._4_4_ = CONCAT13(uVar96,CONCAT12(uVar95,CONCAT11(uVar94,uVar93))) - iVar33;
                auVar72._8_4_ = CONCAT13(uVar100,CONCAT12(uVar99,CONCAT11(uVar98,uVar97))) - iVar33;
                auVar72._12_4_ =
                     CONCAT13(uVar147,CONCAT12(uVar145,CONCAT11(uVar143,uVar141))) - iVar33;
                auVar61 = NEON_umin(auVar61,auVar268,4);
                iVar120 = auVar65._0_4_;
                auVar119._0_4_ = iVar33 - iVar120;
                iVar122 = auVar65._4_4_;
                auVar119._4_4_ = iVar33 - iVar122;
                iVar123 = auVar65._8_4_;
                auVar119._8_4_ = iVar33 - iVar123;
                iVar124 = auVar65._12_4_;
                auVar119._12_4_ = iVar33 - iVar124;
                auVar68 = NEON_bit(auVar72,auVar80,auVar86,1);
                auVar81._0_4_ = iVar120 - iVar33;
                auVar81._4_4_ = iVar122 - iVar33;
                auVar81._8_4_ = iVar123 - iVar33;
                auVar81._12_4_ = iVar124 - iVar33;
                auVar87._0_4_ = (int)(short)auVar115[0];
                auVar87._4_4_ = (int)(short)auVar115[1];
                auVar87._8_4_ = (int)(short)auVar115[2];
                auVar87._12_4_ = (int)(short)auVar115[3];
                auVar77 = NEON_bit(auVar81,auVar119,auVar116,1);
                auVar268 = NEON_bif(auVar268,auVar61,auVar87,1);
                auVar63._0_4_ = param_7 - auVar51._0_4_;
                auVar63._4_4_ = param_7 - auVar51._4_4_;
                auVar63._8_4_ = param_7 - auVar51._8_4_;
                auVar63._12_4_ = param_7 - auVar51._12_4_;
                auVar88._0_4_ = param_7 - auVar68._0_4_;
                auVar88._4_4_ = param_7 - auVar68._4_4_;
                auVar88._8_4_ = param_7 - auVar68._8_4_;
                auVar88._12_4_ = param_7 - auVar68._12_4_;
                auVar65 = NEON_umin(auVar51,auVar63,4);
                auVar64._0_4_ = param_7 - auVar77._0_4_;
                auVar64._4_4_ = param_7 - auVar77._4_4_;
                auVar64._8_4_ = param_7 - auVar77._8_4_;
                auVar64._12_4_ = param_7 - auVar77._12_4_;
                auVar61 = NEON_umin(auVar68,auVar88,4);
                auVar51 = NEON_umin(auVar77,auVar64,4);
                auVar65 = NEON_umin(auVar65,auVar55,4);
                auVar89._0_4_ = (int)(short)auVar115[4];
                auVar89._4_4_ = (int)(short)auVar115[5];
                auVar89._8_4_ = (int)(short)auVar115[6];
                auVar89._12_4_ = (int)(short)auVar115[7];
                auVar61 = NEON_umin(auVar61,auVar92,4);
                auVar55 = NEON_bif(auVar55,auVar65,auVar89,1);
                auVar53._0_4_ = (int)(short)auVar115[8];
                auVar53._4_4_ = (int)(short)auVar115[9];
                auVar53._8_4_ = (int)(short)auVar115[10];
                auVar53._12_4_ = (int)(short)auVar115[0xb];
                auVar65 = NEON_umin(auVar51,auVar103,4);
                auVar92 = NEON_bif(auVar92,auVar61,auVar53,1);
                auVar54._0_4_ = (int)(short)auVar115[0xc];
                auVar54._4_4_ = (int)(short)auVar115[0xd];
                auVar54._8_4_ = (int)(short)auVar115[0xe];
                auVar54._12_4_ = (int)(short)auVar115[0xf];
                auVar103 = NEON_bif(auVar103,auVar65,auVar54,1);
                lVar218 = auVar82._8_8_;
                auVar82._0_8_ = auVar82._0_8_ + 0x10;
                auVar82._8_8_ = lVar218 + 0x10;
                lVar218 = auVar73._8_8_;
                auVar73._0_8_ = auVar73._0_8_ + 0x10;
                auVar73._8_8_ = lVar218 + 0x10;
                lVar218 = CONCAT17(uVar136,CONCAT16(uVar135,CONCAT15(uVar134,CONCAT14(uVar133,
                                                  CONCAT13(bVar59,CONCAT12(bVar58,CONCAT11(bVar57,
                                                  bVar47))))))) + 0x10;
                bVar47 = (byte)lVar218;
                bVar57 = (byte)((ulong)lVar218 >> 8);
                bVar58 = (byte)((ulong)lVar218 >> 0x10);
                bVar59 = (byte)((ulong)lVar218 >> 0x18);
                uVar133 = (undefined1)((ulong)lVar218 >> 0x20);
                uVar134 = (undefined1)((ulong)lVar218 >> 0x28);
                uVar135 = (undefined1)((ulong)lVar218 >> 0x30);
                uVar136 = (undefined1)((ulong)lVar218 >> 0x38);
                lVar218 = CONCAT17(uVar188,CONCAT16(uVar187,CONCAT15(uVar186,CONCAT14(uVar185,
                                                  CONCAT13(bVar183,CONCAT12(bVar181,CONCAT11(bVar179
                                                  ,bVar177))))))) + 0x10;
                bVar177 = (byte)lVar218;
                bVar179 = (byte)((ulong)lVar218 >> 8);
                bVar181 = (byte)((ulong)lVar218 >> 0x10);
                bVar183 = (byte)((ulong)lVar218 >> 0x18);
                uVar185 = (undefined1)((ulong)lVar218 >> 0x20);
                uVar186 = (undefined1)((ulong)lVar218 >> 0x28);
                uVar187 = (undefined1)((ulong)lVar218 >> 0x30);
                uVar188 = (undefined1)((ulong)lVar218 >> 0x38);
                lVar218 = CONCAT17(uVar148,CONCAT16(uVar146,CONCAT15(uVar144,CONCAT14(uVar142,
                                                  CONCAT13(bVar132,CONCAT12(bVar130,CONCAT11(bVar128
                                                  ,bVar126))))))) + 0x10;
                bVar126 = (byte)lVar218;
                bVar128 = (byte)((ulong)lVar218 >> 8);
                bVar130 = (byte)((ulong)lVar218 >> 0x10);
                bVar132 = (byte)((ulong)lVar218 >> 0x18);
                uVar142 = (undefined1)((ulong)lVar218 >> 0x20);
                uVar144 = (undefined1)((ulong)lVar218 >> 0x28);
                uVar146 = (undefined1)((ulong)lVar218 >> 0x30);
                uVar148 = (undefined1)((ulong)lVar218 >> 0x38);
                lVar218 = CONCAT17(uVar204,CONCAT16(uVar202,CONCAT15(uVar200,CONCAT14(uVar198,
                                                  CONCAT13(bVar140,CONCAT12(bVar139,CONCAT11(bVar138
                                                  ,bVar137))))))) + 0x10;
                bVar137 = (byte)lVar218;
                bVar138 = (byte)((ulong)lVar218 >> 8);
                bVar139 = (byte)((ulong)lVar218 >> 0x10);
                bVar140 = (byte)((ulong)lVar218 >> 0x18);
                uVar198 = (undefined1)((ulong)lVar218 >> 0x20);
                uVar200 = (undefined1)((ulong)lVar218 >> 0x28);
                uVar202 = (undefined1)((ulong)lVar218 >> 0x30);
                uVar204 = (undefined1)((ulong)lVar218 >> 0x38);
                lVar36 = lVar36 + 0x10;
                lVar11 = lVar11 + 0x10;
                lVar218 = CONCAT17(uVar233,CONCAT16(uVar231,CONCAT15(uVar229,CONCAT14(uVar227,
                                                  CONCAT13(uVar225,CONCAT12(uVar223,CONCAT11(uVar221
                                                  ,uVar219))))))) + 0x10;
                uVar219 = (undefined1)lVar218;
                uVar221 = (undefined1)((ulong)lVar218 >> 8);
                uVar223 = (undefined1)((ulong)lVar218 >> 0x10);
                uVar225 = (undefined1)((ulong)lVar218 >> 0x18);
                uVar227 = (undefined1)((ulong)lVar218 >> 0x20);
                uVar229 = (undefined1)((ulong)lVar218 >> 0x28);
                uVar231 = (undefined1)((ulong)lVar218 >> 0x30);
                uVar233 = (undefined1)((ulong)lVar218 >> 0x38);
                lVar218 = CONCAT17(uVar249,CONCAT16(uVar247,CONCAT15(uVar245,CONCAT14(uVar243,
                                                  CONCAT13(uVar241,CONCAT12(uVar239,CONCAT11(uVar237
                                                  ,uVar235))))))) + 0x10;
                uVar235 = (undefined1)lVar218;
                uVar237 = (undefined1)((ulong)lVar218 >> 8);
                uVar239 = (undefined1)((ulong)lVar218 >> 0x10);
                uVar241 = (undefined1)((ulong)lVar218 >> 0x18);
                uVar243 = (undefined1)((ulong)lVar218 >> 0x20);
                uVar245 = (undefined1)((ulong)lVar218 >> 0x28);
                uVar247 = (undefined1)((ulong)lVar218 >> 0x30);
                uVar249 = (undefined1)((ulong)lVar218 >> 0x38);
                lVar218 = CONCAT17(uVar203,CONCAT16(uVar201,CONCAT15(uVar199,CONCAT14(uVar197,
                                                  CONCAT13(uVar195,CONCAT12(uVar193,CONCAT11(uVar191
                                                  ,uVar189))))))) + 0x10;
                uVar189 = (undefined1)lVar218;
                uVar191 = (undefined1)((ulong)lVar218 >> 8);
                uVar193 = (undefined1)((ulong)lVar218 >> 0x10);
                uVar195 = (undefined1)((ulong)lVar218 >> 0x18);
                uVar197 = (undefined1)((ulong)lVar218 >> 0x20);
                uVar199 = (undefined1)((ulong)lVar218 >> 0x28);
                uVar201 = (undefined1)((ulong)lVar218 >> 0x30);
                uVar203 = (undefined1)((ulong)lVar218 >> 0x38);
                lVar218 = CONCAT17(uVar216,CONCAT16(uVar215,CONCAT15(uVar214,CONCAT14(uVar213,
                                                  CONCAT13(uVar211,CONCAT12(uVar209,CONCAT11(uVar207
                                                  ,uVar205))))))) + 0x10;
                uVar205 = (undefined1)lVar218;
                uVar207 = (undefined1)((ulong)lVar218 >> 8);
                uVar209 = (undefined1)((ulong)lVar218 >> 0x10);
                uVar211 = (undefined1)((ulong)lVar218 >> 0x18);
                uVar213 = (undefined1)((ulong)lVar218 >> 0x20);
                uVar214 = (undefined1)((ulong)lVar218 >> 0x28);
                uVar215 = (undefined1)((ulong)lVar218 >> 0x30);
                uVar216 = (undefined1)((ulong)lVar218 >> 0x38);
                lVar218 = CONCAT17(uVar160,CONCAT16(uVar159,CONCAT15(uVar158,CONCAT14(uVar157,
                                                  CONCAT13(uVar155,CONCAT12(uVar153,CONCAT11(uVar151
                                                  ,uVar149))))))) + 0x10;
                uVar149 = (undefined1)lVar218;
                uVar151 = (undefined1)((ulong)lVar218 >> 8);
                uVar153 = (undefined1)((ulong)lVar218 >> 0x10);
                uVar155 = (undefined1)((ulong)lVar218 >> 0x18);
                uVar157 = (undefined1)((ulong)lVar218 >> 0x20);
                uVar158 = (undefined1)((ulong)lVar218 >> 0x28);
                uVar159 = (undefined1)((ulong)lVar218 >> 0x30);
                uVar160 = (undefined1)((ulong)lVar218 >> 0x38);
                lVar218 = CONCAT17(uVar172,CONCAT16(uVar171,CONCAT15(uVar170,CONCAT14(uVar169,
                                                  CONCAT13(uVar167,CONCAT12(uVar165,CONCAT11(uVar163
                                                  ,uVar161))))))) + 0x10;
                uVar161 = (undefined1)lVar218;
                uVar163 = (undefined1)((ulong)lVar218 >> 8);
                uVar165 = (undefined1)((ulong)lVar218 >> 0x10);
                uVar167 = (undefined1)((ulong)lVar218 >> 0x18);
                uVar169 = (undefined1)((ulong)lVar218 >> 0x20);
                uVar170 = (undefined1)((ulong)lVar218 >> 0x28);
                uVar171 = (undefined1)((ulong)lVar218 >> 0x30);
                uVar172 = (undefined1)((ulong)lVar218 >> 0x38);
                uVar67 = CONCAT44(iVar105 + 0x10,iVar104 + 0x10);
                uVar107 = CONCAT44(iVar108 + 0x10,iVar106 + 0x10);
                uVar110 = CONCAT44(iVar111 + 0x10,iVar109 + 0x10);
                uVar113 = CONCAT44(iVar114 + 0x10,iVar112 + 0x10);
                iVar104 = CONCAT13(uVar131,CONCAT12(uVar129,CONCAT11(uVar127,uVar125))) + 0x10;
                uVar125 = (undefined1)iVar104;
                uVar127 = (undefined1)((uint)iVar104 >> 8);
                uVar129 = (undefined1)((uint)iVar104 >> 0x10);
                uVar131 = (undefined1)((uint)iVar104 >> 0x18);
                iVar104 = CONCAT13(uVar96,CONCAT12(uVar95,CONCAT11(uVar94,uVar93))) + 0x10;
                uVar93 = (undefined1)iVar104;
                uVar94 = (undefined1)((uint)iVar104 >> 8);
                uVar95 = (undefined1)((uint)iVar104 >> 0x10);
                uVar96 = (undefined1)((uint)iVar104 >> 0x18);
                iVar104 = CONCAT13(uVar100,CONCAT12(uVar99,CONCAT11(uVar98,uVar97))) + 0x10;
                uVar97 = (undefined1)iVar104;
                uVar98 = (undefined1)((uint)iVar104 >> 8);
                uVar99 = (undefined1)((uint)iVar104 >> 0x10);
                uVar100 = (undefined1)((uint)iVar104 >> 0x18);
                iVar104 = CONCAT13(uVar147,CONCAT12(uVar145,CONCAT11(uVar143,uVar141))) + 0x10;
                uVar141 = (undefined1)iVar104;
                uVar143 = (undefined1)((uint)iVar104 >> 8);
                uVar145 = (undefined1)((uint)iVar104 >> 0x10);
                uVar147 = (undefined1)((uint)iVar104 >> 0x18);
                auVar65._0_4_ = iVar120 + 0x10;
                auVar65._4_4_ = iVar122 + 0x10;
                auVar65._8_4_ = iVar123 + 0x10;
                auVar65._12_4_ = iVar124 + 0x10;
                pauVar43 = pauVar43 + 1;
              } while (uVar37 != 0);
              auVar65 = NEON_umin(auVar268,auVar92,4);
              auVar55 = NEON_umin(auVar55,auVar103,4);
              auVar55 = NEON_umin(auVar65,auVar55,4);
              uVar50 = NEON_uminv(auVar55,4);
              bVar177 = bStack_378;
              bVar179 = bStack_377;
              bVar181 = bStack_376;
              bVar183 = bStack_375;
              uVar185 = uStack_374;
              uVar186 = uStack_373;
              uVar187 = uStack_372;
              uVar188 = uStack_371;
              if (uVar35 != uVar39) {
                uVar37 = uVar35;
                if ((param_7 >> 3 & 1) == 0) goto LAB_001b4f48;
                goto LAB_001b4e64;
              }
            }
            if (local_350[uVar46] == '\0') {
              fVar48 = -1e+30;
            }
            else {
              fVar48 = local_2d0[uVar46];
            }
            if ((((int)uVar41 < 0) || (uVar42 < uVar50)) ||
               ((uVar50 == uVar42 &&
                (fVar48 != fVar101 && fVar48 < fVar101 == (NAN(fVar48) || NAN(fVar101)))))) {
              uVar41 = uVar46 & 0xffffffff;
              uVar42 = uVar50;
              fVar101 = fVar48;
            }
          }
          uVar46 = uVar46 + 1;
          iVar32 = iVar32 + -1;
        } while (uVar46 != uVar39);
        if ((int)(uint)uVar41 < 0) {
          return false;
        }
        if (((uint)uVar41 < param_7) && ((*param_11)[uVar41] == '\0')) {
          uVar34 = uVar34 + 1;
          (*param_11)[uVar41] = 1;
        }
      } while (uVar34 < param_8);
    }
    return uVar34 == param_8;
  }
  goto LAB_001b47a0;
}

/* ===== FUN_001b65d8 @ 001b65d8 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

bool FUN_001b65d8(undefined8 param_1,double param_2,undefined8 param_3,double param_4,double param_5
                 ,undefined8 param_6,double param_7,int *param_8,int *param_9)

{
  undefined1 auVar1 [16];
  int iVar2;
  bool bVar3;
  double dVar4;
  undefined8 uVar5;
  int iVar6;
  double dVar7;
  undefined1 auVar8 [16];
  double local_68;
  double local_58;
  
  local_58 = 0.0;
  local_68 = 0.0;
  iVar2 = FUN_001b29d8(&local_58,&local_68);
  if (iVar2 < 0) {
    bVar3 = false;
  }
  else {
    dVar7 = (double)NEON_fmadd(param_3,param_3,param_4 * param_4);
    dVar4 = 0.0;
    if (dVar7 != DAT_0010e730 && dVar7 < DAT_0010e730 == (NAN(dVar7) || NAN(DAT_0010e730))) {
      dVar4 = (double)NEON_fnmadd(param_1,param_3,-(param_2 * param_4));
      dVar4 = (double)NEON_fminnm(param_6,dVar4 / dVar7);
      if (dVar4 <= 0.0) {
        dVar4 = 0.0;
      }
    }
    dVar7 = (double)NEON_fmadd(param_4,dVar4,param_2);
    uVar5 = NEON_fmadd(param_3,dVar4,param_1);
    bVar3 = false;
    dVar4 = (double)NEON_fmadd(uVar5,uVar5,dVar7 * dVar7);
    dVar7 = ABS(SQRT(dVar4) - param_5);
    if ((dVar7 != INFINITY) && (!NAN(dVar7))) {
      dVar4 = SQRT(dVar4) - param_5;
      iVar6 = NEON_fminnm(param_8[8],(float)(dVar4 * 300.0));
      param_8[8] = iVar6;
      auVar8 = _DAT_00112930;
      if (iVar2 == 0) {
        dVar4 = *(double *)(param_8 + 6);
      }
      else {
        param_7 = local_58 + param_7;
        if (*param_9 == 0) {
          *param_9 = 1;
          auVar1._8_8_ = param_7;
          auVar1._0_8_ = param_7;
          auVar8 = NEON_fcmge(auVar8,auVar1,8);
          iVar2 = NEON_fminnm(param_8[9],(float)param_7);
          *param_8 = *param_8 + 1;
          *(ulong *)(param_8 + 1) =
               CONCAT44((int)((ulong)*(undefined8 *)(param_8 + 1) >> 0x20) - auVar8._8_4_,
                        (int)*(undefined8 *)(param_8 + 1) - auVar8._0_4_);
          param_8[9] = iVar2;
        }
        param_5 = -dVar4 / param_5;
        if (param_5 <= 0.0) {
          param_5 = 0.0;
        }
        dVar4 = (double)NEON_fminnm(param_5,0x3ff0000000000000);
        dVar4 = *(double *)(param_8 + 6) +
                ((dVar4 + 1.0) * (local_68 - local_58)) / (param_7 + DAT_0010e768);
        *(double *)(param_8 + 6) = dVar4;
      }
      dVar4 = ABS(dVar4);
      bVar3 = dVar4 != INFINITY && dVar4 < INFINITY == NAN(dVar4) || dVar4 < INFINITY;
    }
  }
  return bVar3;
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

/* ===== FUN_001ba550 @ 001ba550 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_001ba550(long param_1,long param_2)

{
  uint uVar1;
  uint uVar2;
  ulong uVar3;
  long lVar4;
  long lVar5;
  undefined1 auVar6 [16];
  undefined1 auVar7 [16];
  undefined1 auVar8 [16];
  undefined8 local_a0;
  undefined8 uStack_98;
  undefined8 uStack_90;
  undefined8 uStack_88;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined8 uStack_70;
  undefined8 uStack_68;
  undefined8 local_60;
  undefined8 uStack_58;
  undefined8 uStack_50;
  undefined8 uStack_48;
  undefined8 local_40;
  undefined8 uStack_38;
  undefined8 uStack_30;
  undefined8 uStack_28;
  
  uVar3 = *(ulong *)(param_1 + 0x20);
  lVar5 = uVar3 << 3;
  lVar4 = 0x38;
  if (0x37 < *(ulong *)(param_1 + 0x28)) {
    lVar4 = 0x78;
  }
  auVar7._8_8_ = lVar5;
  auVar7._0_8_ = lVar5;
  lVar4 = lVar4 - *(ulong *)(param_1 + 0x28);
  auVar8._8_8_ = _UNK_00112bc8;
  auVar8._0_8_ = _DAT_00112bc0;
  auVar6 = NEON_ushl(auVar7,_DAT_00112990,8);
  uStack_98 = 0;
  uStack_88 = 0;
  uStack_90 = 0;
  uStack_78 = 0;
  local_80 = 0;
  uStack_68 = 0;
  uStack_70 = 0;
  auVar8 = NEON_ushl(auVar7,auVar8,8);
  uStack_58 = 0;
  local_60 = 0;
  uStack_48 = 0;
  uStack_50 = 0;
  uStack_38 = 0;
  local_40 = 0;
  uStack_28 = 0;
  uStack_30 = 0;
  local_a0 = 0x80;
  *(char *)((long)&local_a0 + lVar4 + 4) = (char)(uVar3 >> 0x15);
  *(char *)((long)&local_a0 + lVar4 + 7) = (char)lVar5;
  *(char *)((long)&local_a0 + lVar4 + 5) = (char)(uVar3 >> 0xd);
  *(char *)((long)&local_a0 + lVar4 + 6) = (char)(uVar3 >> 5);
  *(uint *)((long)&local_a0 + lVar4) =
       CONCAT13(auVar8[8],CONCAT12(auVar8[0],CONCAT11(auVar6[8],auVar6[0])));
  FUN_001ba230(param_1,&local_a0,lVar4 + 8);
  uVar2 = 0;
  uVar3 = 0;
  do {
    uVar1 = uVar2 ^ 0xffffffff;
    uVar2 = uVar2 + 8;
    *(char *)(param_2 + uVar3) =
         (char)(*(uint *)(param_1 + (uVar3 & 0xfffffffc)) >> (ulong)(uVar1 & 0x18));
    uVar3 = uVar3 + 1;
  } while (uVar3 != 0x20);
  return;
}

/* ===== FUN_001bae8c @ 001bae8c [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_001bae8c(int *param_1,int *param_2,int *param_3)

{
  int *piVar1;
  long *plVar2;
  int *piVar3;
  int iVar4;
  char cVar5;
  bool bVar6;
  undefined1 auVar7 [16];
  undefined8 uVar8;
  int iVar9;
  ulong uVar10;
  int iVar11;
  long lVar12;
  int *piVar13;
  long lVar14;
  long lVar15;
  long *plVar16;
  long lVar17;
  long lVar18;
  int iVar19;
  ulong uVar20;
  int iVar21;
  ulong uVar22;
  int iVar23;
  uint uVar24;
  float fVar25;
  long lVar26;
  float fVar28;
  undefined1 auVar27 [16];
  long lVar29;
  undefined1 auVar30 [16];
  
  if (param_1 == (int *)0x0) {
    return 0;
  }
  if (param_3 == (int *)0x0) {
    return 0;
  }
  if (*param_1 != 0x4e444152) {
    return 0;
  }
  if ((*param_3 != 1) || (param_3[1] != 0xdc0)) {
    return 0;
  }
  memset(param_3 + 2,0,0xdb8);
  piVar1 = param_1 + 1;
  *(undefined8 *)param_3 = DAT_0010e590;
  do {
    if (*piVar1 != 0) {
      ClearExclusiveLocal();
      param_3[3] = 4;
      return 0;
    }
    cVar5 = '\x01';
    bVar6 = (bool)ExclusiveMonitorPass(piVar1,0x10);
    if (bVar6) {
      *piVar1 = 1;
      cVar5 = ExclusiveMonitorsStatus();
    }
  } while (cVar5 != '\0');
  if (((((((param_2 == (int *)0x0) || (*param_2 != 1)) || (param_2[1] != 0x58)) ||
        (((1 < (uint)param_2[2] || (piVar13 = *(int **)(param_2 + 4), piVar13 == (int *)0x0)) ||
         ((*piVar13 != 1 || ((piVar13[1] != 0x38 || (piVar13[2] != 1)))))))) ||
       (uVar10 = *(ulong *)(piVar13 + 8), uVar10 == 0)) ||
      (((piVar13[3] == 0 || (piVar13[4] == 0)) ||
       (uVar22 = *(ulong *)(piVar13 + 10), uVar22 != *(ulong *)(param_2 + 10))))) ||
     ((((uVar20 = *(ulong *)(piVar13 + 0xc), uVar20 != *(ulong *)(param_2 + 0xc) ||
        (lVar14 = *(long *)(param_2 + 6), lVar14 == 0)) ||
       ((lVar17 = *(long *)(param_2 + 8), lVar17 == 0 ||
        ((uVar22 + 0x1000 < 0x11000 || ((uVar22 & 7) != 0)))))) ||
      ((uVar20 + 0x1000 < 0x11000 ||
       (((((((uVar20 & 7) != 0 || (*(ulong *)(param_2 + 0xe) + 0x1000 < 0x11000)) ||
           ((*(ulong *)(param_2 + 0xe) & 7) != 0)) ||
          ((*(ulong *)(param_2 + 0x10) + 0x1000 < 0x11000 || ((*(ulong *)(param_2 + 0x10) & 7) != 0)
           ))) || (999999 < param_2[0x12] - 1000000U)) ||
        (((1999999 < param_2[0x13] - 16000000U || (0x31 < (uint)param_2[3])) ||
         ((param_2[3] != 0 && (*(long *)(param_2 + 0x14) == 0)))))))))))) {
    uVar8 = 0;
    param_3[3] = 1;
    goto LAB_001bb238;
  }
  uVar22 = *(ulong *)(param_1 + 2);
  param_3[0xe] = piVar13[3];
  *(ulong *)(param_3 + 8) = uVar10;
  *(long *)(param_3 + 10) = lVar14;
  *(long *)(param_3 + 0xc) = lVar17;
  if (uVar22 < uVar10) {
    *(ulong *)(param_1 + 2) = uVar10;
    uVar8 = FUN_001bb554(param_2);
    if ((int)uVar8 != 0) {
      piVar13 = param_1 + 0x1f2;
      memcpy(piVar13,param_1 + 4,0x7b8);
      lVar14 = *(long *)(param_1 + 0x1f2);
      if (((((lVar14 == *(long *)(param_2 + 6)) &&
            (lVar17 = *(long *)(param_1 + 500), lVar17 == *(long *)(param_2 + 10))) &&
           (lVar12 = *(long *)(param_1 + 0x1f6), lVar12 == *(long *)(param_2 + 0xc))) &&
          ((lVar15 = *(long *)(param_1 + 0x1f8), lVar15 == *(long *)(param_2 + 0xe) &&
           (lVar18 = *(long *)(param_1 + 0x1fa), lVar18 == *(long *)(param_2 + 0x10))))) &&
         ((iVar19 = param_1[0x1fc], iVar19 == param_2[0x12] &&
          (iVar23 = param_1[0x1fd], iVar23 == param_2[0x13])))) {
        iVar21 = param_1[0x1fe];
        if ((iVar21 != *(int *)(*(long *)(param_2 + 4) + 0x10)) ||
           (uVar24 = *(uint *)(*(long *)(param_2 + 4) + 0xc), uVar24 < (uint)param_1[0x1ff]))
        goto LAB_001bb158;
      }
      else {
LAB_001bb158:
        memset(piVar13,0,0x7b8);
        lVar14 = *(long *)(param_2 + 6);
        lVar17 = *(long *)(param_2 + 10);
        lVar12 = *(long *)(param_2 + 0xc);
        lVar15 = *(long *)(param_2 + 0xe);
        lVar18 = *(long *)(param_2 + 0x10);
        iVar19 = param_2[0x12];
        iVar23 = param_2[0x13];
        uVar24 = *(uint *)(*(long *)(param_2 + 4) + 0xc);
        iVar21 = *(int *)(*(long *)(param_2 + 4) + 0x10);
      }
      *(long *)(param_1 + 500) = lVar17;
      iVar9 = param_2[2];
      *(long *)(param_1 + 0x1f6) = lVar12;
      *(long *)(param_1 + 0x1f2) = lVar14;
      iVar4 = param_2[3];
      *(long *)(param_1 + 0x1f8) = lVar15;
      iVar11 = 3;
      if (iVar9 != 0) {
        iVar11 = 0;
      }
      *(long *)(param_1 + 0x1fa) = lVar18;
      param_1[0x1fc] = iVar19;
      param_1[0x1fd] = iVar23;
      param_1[0x1fe] = iVar21;
      param_1[0x1ff] = uVar24;
      param_3[2] = iVar9;
      param_3[3] = iVar11;
      auVar7 = _DAT_00112810;
      if (iVar4 != 0) {
        lVar14 = 0;
        uVar10 = 0;
        do {
          lVar12 = *(long *)(param_2 + 0x14);
          plVar2 = (long *)(lVar12 + lVar14);
          lVar17 = *plVar2;
          if (lVar17 == *(long *)(param_2 + 0x10)) {
            iVar19 = *(int *)((long)plVar2 + 0x1c);
            param_3[5] = param_3[5] + 1;
            if (((iVar19 == 0) || (*(int *)(lVar12 + lVar14 + 0x10) != param_2[0x12])) ||
               (*(int *)(lVar12 + lVar14 + 0x14) != param_2[0x13])) {
              iVar19 = param_3[7];
LAB_001bb250:
              param_3[2] = 0;
              param_3[7] = iVar19 + 1;
            }
          }
          else {
            uVar24 = param_3[4];
            uVar22 = (ulong)uVar24;
            lVar15 = plVar2[2];
            plVar16 = (long *)(param_3 + (ulong)uVar24 * 10 + 0x10);
            param_3[4] = uVar24 + 1;
            lVar18 = plVar2[3];
            lVar26 = *(long *)((long)plVar2 + 0x2c);
            lVar29 = plVar2[4];
            *plVar16 = lVar17;
            *(int *)(plVar16 + 2) = (int)lVar18;
            *(int *)((long)plVar16 + 0x14) = 0;
            *(int *)(plVar16 + 1) = (int)lVar15;
            *(int *)((long)plVar16 + 0xc) = 0;
            piVar3 = param_3 + uVar22 * 8 + 0x1f0;
            plVar16[4] = lVar29;
            plVar16[3] = lVar26;
            if ((((*(int *)((long)plVar2 + 0x1c) == 0) ||
                 (uVar20 = *(ulong *)(lVar12 + lVar14 + 8), uVar20 + 0x1000 < 0x11000)) ||
                ((uVar20 & 7) != 0)) ||
               (((int)plVar2[2] - 2000000U < 0xfff0bdc0 ||
                (*(int *)(lVar12 + lVar14 + 0x14) + 0xfeed5780U < 0xffe17b80)))) {
              iVar19 = param_3[7];
              *piVar3 = 5;
              goto LAB_001bb250;
            }
            param_3[uVar22 * 10 + 0x13] = 1;
            uVar8 = NEON_scvtf(plVar2[4],4);
            fVar25 = (float)uVar8 / 300.0;
            fVar28 = (float)((ulong)uVar8 >> 0x20) / 300.0;
            auVar27._0_8_ = CONCAT44(fVar28,fVar25);
            auVar27._8_8_ = auVar27._0_8_;
            auVar30 = NEON_fcmgt(auVar7,auVar27,4);
            auVar27 = NEON_fcmgt(auVar27,auVar7,4);
            if ((((auVar30._0_2_ & 1 | (auVar30._4_2_ & 1) << 1 | (auVar27._8_2_ & 1) << 2 |
                  (uint)auVar27._12_2_ << 3) ^ 0xffffffff) & 0xf) == 0) {
              lVar17 = FUN_001bb640(piVar13,plVar2,*(undefined4 *)(*(long *)(param_2 + 4) + 0xc),
                                    param_3 + uVar22 * 8 + 0x1f2);
              if ((lVar17 == 0) ||
                 (FUN_001bb730(lVar17,*(undefined4 *)(*(long *)(param_2 + 4) + 0xc),piVar3),
                 param_3[uVar22 * 8 + 0x1f1] == 0)) {
                iVar19 = param_3[7];
                param_3[2] = 0;
                *piVar3 = 5;
                param_3[7] = iVar19 + 1;
                param_3[uVar22 * 10 + 0x13] = 0;
              }
              else if (*(int *)(lVar12 + lVar14 + 0x34) < 1) {
                *piVar3 = 6;
              }
              else if ((0.5 <= ABS(fVar25)) || (0.5 <= ABS(fVar28))) {
                if ((*(int *)((long)plVar2 + 0x2c) == -1) || ((int)plVar2[6] == -1)) {
                  *piVar3 = 9;
                }
                else {
                  fVar25 = (float)param_3[uVar22 * 8 + 0x1f7];
                  if (fVar25 == 100.0 || fVar25 < 100.0 != NAN(fVar25)) {
                    iVar19 = param_3[6];
                    param_3[(ulong)uVar24 * 10 + 0x15] = 1;
                    param_3[6] = iVar19 + 1;
                  }
                  else {
                    *piVar3 = 10;
                  }
                }
              }
              else {
                *piVar3 = 8;
              }
            }
            else {
              *piVar3 = 7;
            }
          }
          uVar10 = uVar10 + 1;
          lVar14 = lVar14 + 0x40;
        } while (uVar10 < (uint)param_2[3]);
        iVar9 = param_3[2];
      }
      if (iVar9 != 0) {
        memcpy(param_1 + 4,piVar13,0x7b8);
        uVar8 = 1;
        goto LAB_001bb238;
      }
      iVar19 = 3;
      goto LAB_001bb518;
    }
    iVar19 = 1;
  }
  else {
    iVar19 = 2;
LAB_001bb518:
    uVar8 = 0;
  }
  param_3[3] = iVar19;
LAB_001bb238:
  *piVar1 = 0;
  return uVar8;
}

/* ===== strtoull @ 0031c120 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

ulonglong strtoull(char *__nptr,char **__endptr,int __base)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

