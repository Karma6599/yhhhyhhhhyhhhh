/*
 * bolt_mod - Bolt autopilot (mixed: reconstructed attack engine + raw pathing remainder)
 * Decompiled with Ghidra 11.3.2 (arm64 pseudocode) from: libNexusEvasion69252.so, libNexusEvasionRuntime69252.so
 * Layout note: FUN_00112940 (adapter-family classifier), FUN_0015fe20 (auto-attack tick),
 * FUN_0016944c (armed check) and FUN_00169b3c (guarded-write verify) are reconstructed below.
 * Still raw at the end: FUN_00167544, the 0x700-line wall-avoidance pathing engine
 * (smooth movement with the wall grid at DAT_00215aac/ab8/ac0/b00/b04, aim vector
 * DAT_002284b0/b4, threat state 0x228728-0x2287a8 family).
 */

#define _GNU_SOURCE 1

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>

extern void log_event(const char *category, const char *event, const char *json);

extern uintptr_t g_game_base;
extern uintptr_t game_read(uintptr_t handle, uintptr_t addr, void *out, uint32_t len);
extern uintptr_t runtime_read_handle(void);
extern uintptr_t reader_open_ctx(uintptr_t world, uintptr_t ctrl);
extern int runtime_gate_b(void);
extern int game_read_scaled_position(uintptr_t game_base, uintptr_t ctrl,
                                     void *read_fn, int flags, void *out);
extern int bolt_wall_probe(uintptr_t world, void *read_fn, int flags, void *out_flags);

extern pthread_once_t g_snapshot_plus_once;
extern long (*snapshot_plus_active_fn)(void);
extern void snapshot_plus_once_init(void);
extern void *g_snapshot_keys_fn;
extern int   g_snapshot_keys_ready;

typedef int (*snapshot_keys_fn_t)(const char *const *keys, int32_t *triples,
                                  int count, int64_t *epoch, int32_t *status);
typedef uintptr_t (*game_read_fn_t)(uintptr_t handle, uintptr_t addr, void *out, uint32_t len);

#define SNAPSHOT_TRIPLE_EFFECTIVE 1
#define SNAPSHOT_TRIPLE_FLAGS     2
#define SNAPSHOT_FLAG_ACTIVE      2

extern uint64_t g_ctx_a;
extern uint64_t g_ctx_c;
extern uint64_t g_ctx_bolt_marker;
extern uint64_t g_ctx_own_x;
extern uint64_t g_ctx_own_y;
extern uint64_t g_ctx_own_radius;
extern uint32_t g_ctx_hp_cur;
extern uint32_t g_ctx_hp_max;
extern uint64_t g_ctx_2;
extern uint32_t g_ctx_enemy_count;
extern uintptr_t g_ctx_enemy_list;

extern uint64_t g_plan_mode;
extern uint64_t g_plan_base;
extern uint64_t g_plan_peer_check;
extern uint64_t g_plan_aux;
extern uint64_t g_plan_kind;
extern uint64_t g_bolt_record_epoch;
extern uint64_t g_bolt_record_stamp;
extern uint64_t g_bolt_target_id;
extern float    g_bolt_aim_x;
extern float    g_bolt_aim_y;
extern uint64_t g_bolt_state_word;
extern uint64_t g_bolt_sub_state;
extern uint64_t g_bolt_state_c14;
extern uint64_t g_bolt_last_fire_ms;
extern uint64_t g_bolt_busy;
extern uint64_t g_bolt_state_f0;

extern int bolt_target_position(uint32_t a, uint32_t b, uint32_t c, uint32_t d);
extern int bolt_write_apply(void *payload);

#define BOLT_ARMED_MARKER 17000010u

#define BOLT_GAME_ATTACK_OFF 0xb2e994u

int evasion_key_adapter_family(const char *name)
{
    if (name == NULL)
        return 0;

    if (strcmp(name, "isXrayEnabled") == 0 ||
        strcmp(name, "xRayEnabled") == 0 ||
        strcmp(name, "xrayEnabled") == 0 ||
        strcmp(name, "xrayShowTargetName") == 0 ||
        strcmp(name, "antiAfkEnabled") == 0 ||
        strcmp(name, "hitboxRenderer") == 0 ||
        strcmp(name, "enemyTracer") == 0 ||
        strcmp(name, "dynaJumpEnabled") == 0 ||
        strcmp(name, "attackRangeIndicator") == 0 ||
        strcmp(name, "ballAssistEnabled") == 0 ||
        strcmp(name, "coltModEnabled") == 0 ||
        strcmp(name, "koltModEnabled") == 0 ||
        strcmp(name, "autofarmEnabled") == 0 ||
        strcmp(name, "autofarmAttackEnemies") == 0 ||
        strcmp(name, "characterOutlineEnabled") == 0 ||
        strcmp(name, "boltModEnabled") == 0 ||
        strcmp(name, "boltWallAvoidEnabled") == 0 ||
        strcmp(name, "boltPredictionEnabled") == 0 ||
        strcmp(name, "boltAutoAttackEnabled") == 0 ||
        strcmp(name, "boltSafeExitEnabled") == 0 ||
        strcmp(name, "trophiesAboveHead") == 0 ||
        strcmp(name, "kitNaniModEnabled") == 0 ||
        strcmp(name, "killauraSuper") == 0 ||
        strcmp(name, "killauraGadget") == 0)
        return 1;

    return strcmp(name, "speedLocalMoveEnabled") == 0;
}

static uint64_t monotonic_ms(void)
{
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
        return 0;
    return (uint64_t)ts.tv_sec * 1000 + (uint64_t)ts.tv_nsec / 1000000;
}

static int snapshot_single(const char *key, int32_t *triple,
                           int64_t *epoch, int32_t *status)
{
    if (__atomic_load_n(&g_snapshot_keys_ready, __ATOMIC_ACQUIRE) != 1
        || g_snapshot_keys_fn == NULL)
        return 0;

    pthread_once(&g_snapshot_plus_once, snapshot_plus_once_init);
    if (snapshot_plus_active_fn == NULL || snapshot_plus_active_fn() != 1)
        return 0;

    snapshot_keys_fn_t query = (snapshot_keys_fn_t)g_snapshot_keys_fn;
    return query(&key, triple, 1, epoch, status) == 1;
}

static int bolt_marker_armed(void)
{
    uint32_t marker = (uint32_t)g_ctx_bolt_marker;
    uint32_t adjusted = marker - 1000000u;
    if (marker + 0xfefc99c0u > 999999u)
        adjusted = marker;
    return adjusted == 0xf4246au;
}

static int feature_active(const char *key, int64_t *epoch_out)
{
    int32_t triple[3];
    int32_t status = -1;
    int64_t epoch = 0;

    if (!snapshot_single(key, triple, &epoch, &status))
        return 0;
    if (epoch == 0 || status != 0
        || triple[SNAPSHOT_TRIPLE_FLAGS] != SNAPSHOT_FLAG_ACTIVE
        || triple[SNAPSHOT_TRIPLE_EFFECTIVE] != 1)
        return 0;

    if (epoch_out)
        *epoch_out = epoch;
    return 1;
}

int bolt_mod_armed(void)
{
    if (!bolt_marker_armed())
        return 0;

    int64_t epoch;
    if (!feature_active("boltModEnabled", &epoch))
        return 0;

    return feature_active("boltAutoAttackEnabled", NULL)
        && runtime_read_handle() != 0;
}

int bolt_write_verify(void *payload)
{
    uint64_t *rec = payload;

    if (runtime_read_handle() == 0)
        return 0;
    if (!bolt_marker_armed())
        return 0;

    int64_t epoch;
    if (!feature_active("boltModEnabled", &epoch))
        return 0;
    if ((uint64_t)epoch != rec[2])
        return 0;

    if (!feature_active("boltAutoAttackEnabled", &epoch))
        return 0;
    if ((uint64_t)epoch != rec[2])
        return 0;

    return reader_open_ctx((uintptr_t)rec[0], rec[1]) != 0;
}

typedef struct {
    uint64_t *payload;
    game_read_fn_t read;
    int (*apply)(void *payload);
    int (*verify)(void *payload);
    uint64_t payload_slots[8];
} bolt_guarded_record_t;

extern int remote_guarded_apply(void *record, uintptr_t target,
                                const void *in, void *out);

void bolt_auto_attack_tick(void *frame)
{
    uint64_t now_ms = monotonic_ms();

    if (frame == NULL)
        return;
    if (!bolt_marker_armed())
        return;

    int64_t epoch;
    if (!feature_active("boltModEnabled", &epoch))
        return;
    if (!feature_active("boltAutoAttackEnabled", &epoch))
        return;

    uintptr_t handle = runtime_read_handle();
    if (handle == 0)
        return;
    if ((uint64_t)epoch != g_bolt_record_epoch)
        return;
    if (g_bolt_record_stamp != g_plan_base || g_bolt_target_id == 0)
        return;
    if (g_plan_aux != 0x84)
        return;

    uint32_t lanes[4] = { (uint32_t)g_plan_mode, (uint32_t)g_bolt_state_word,
                          (uint32_t)g_bolt_sub_state, (uint32_t)g_bolt_state_c14 };
    uint32_t tag[4] = { 2, 0, 0, 0 };
    if (memcmp(lanes, tag, sizeof tag) != 0)
        return;
    if (g_plan_kind != 3 || g_plan_peer_check != g_bolt_record_stamp)
        return;
    if (g_bolt_state_f0 == g_bolt_record_stamp)
        return;
    if (now_ms <= g_bolt_last_fire_ms - 1 || now_ms - g_bolt_last_fire_ms > 0x351)
        return;
    if (runtime_gate_b() == 0)
        return;

    uint8_t wall_flags[2];
    if (bolt_wall_probe(g_ctx_a, (void *)game_read, 0, wall_flags) == 0)
        return;
    if (wall_flags[0] != 0 || wall_flags[1] != 0)
        return;

    if (g_ctx_enemy_count == 0)
        return;

    uint32_t *entry = (uint32_t *)(g_ctx_enemy_list + 0x10);
    for (uint32_t i = (uint32_t)g_ctx_enemy_count; i != 0; i--, entry += 0x10) {
        if (*entry != (uint32_t)g_bolt_target_id)
            continue;

        uint64_t entity = *(uint64_t *)((char *)entry - 0x10);
        if (entity == 0)
            break;

        uint32_t target_id = *entry;
        struct {
            uint64_t field_0;
            uint32_t gid;
            uint32_t field_c;
            float    x;
            float    y;
        } pos = { 0, 0, 0, 0.0f, 0.0f };

        if (game_read_scaled_position(g_game_base, entity,
                                      (void *)game_read, 0, &pos) == 0)
            break;
        if (pos.gid != target_id || pos.field_c == 0)
            break;

        uint32_t nav[2] = { 0, 0 };
        if (bolt_target_position((uint32_t)g_ctx_own_radius,
                                 target_id, nav[0], nav[1]) == 0)
            break;

        float dx = pos.x - (float)(int32_t)g_ctx_own_x;
        float dy = pos.y - (float)(int32_t)g_ctx_own_y;
        float dist = hypotf(dx, dy);

        float r_own = (float)(uint32_t)g_ctx_own_radius / 300.0f;
        float r_tgt = (float)pos.field_c / 300.0f;
        if (r_own <= 0.25f)
            r_own = 0.25f;
        if (r_tgt <= 0.25f)
            r_tgt = 0.25f;
        r_own = fminf(r_own, 1.2f);
        r_tgt = fminf(r_tgt, 1.2f);

        float aim = -1.0f;
        if (dist >= 0.0001f) {
            aim = (g_bolt_aim_x * dx + dy * g_bolt_aim_y) / dist;
        }

        float reach = r_own + r_tgt + 3.25f;
        int in_range = dist <= reach;
        if (!in_range && aim >= 0.15f)
            in_range = dist <= (r_own + r_tgt + 0.85f);
        if (!in_range)
            break;

        uintptr_t target_handle = (uintptr_t)bolt_target_position(
            (uint32_t)g_ctx_own_radius, target_id, nav[0], nav[1]);
        if (target_handle == 0)
            break;

        int32_t xy[2] = { (int32_t)(pos.x * 300.0f), (int32_t)(pos.y * 300.0f) };
        uint64_t pair_word = 0;
        if (game_read(target_handle, g_ctx_a + 0xfac, &pair_word, 8) == 0)
            break;

        uint64_t payload[3] = { g_ctx_a, g_ctx_c, (uint64_t)epoch };
        bolt_guarded_record_t record;
        record.payload = payload;
        record.read = game_read;
        record.apply = bolt_write_apply;
        record.verify = bolt_write_verify;

        int result = remote_guarded_apply(&record, g_ctx_a, &pair_word, xy);
        if (result == 1) {
            if (bolt_write_verify(payload) != 0) {
                uint64_t saved_busy = g_bolt_busy;
                g_bolt_state_word = 5;
                g_bolt_busy = 1;
                g_bolt_sub_state = 1;
                g_bolt_last_fire_ms = now_ms;

                uint64_t attack_result = ((uint64_t (*)(uint64_t, uint64_t))
                    (g_game_base + BOLT_GAME_ATTACK_OFF))(payload[0], payload[1]);

                g_bolt_state_word = 0;
                g_bolt_sub_state = 0;
                g_bolt_state_f0 = g_plan_base;
                g_bolt_busy = saved_busy;

                char json[0xb4];
                snprintf(json, sizeof json,
                         ",\"target_gid\":%u,\"result\":%d",
                         pos.gid, (uint32_t)attack_result);
                log_event("bolt_fire", "original_wrapper", json);
            }
        } else if (result == -1) {
            abort();
        }
        break;
    }
}
/* ===== FUN_00167544 @ 00167544 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00167544(undefined8 *param_1)

{
  int *piVar1;
  long *plVar2;
  uint uVar3;
  ulong uVar4;
  long lVar5;
  undefined8 uVar6;
  int iVar7;
  undefined *puVar8;
  int iVar9;
  bool bVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  uint uVar14;
  long lVar15;
  long lVar16;
  long lVar17;
  uint uVar18;
  long lVar19;
  long lVar20;
  ulong uVar21;
  long lVar22;
  uint uVar23;
  ulong uVar24;
  float fVar25;
  float fVar26;
  undefined4 uVar27;
  undefined4 uVar28;
  undefined8 uVar29;
  undefined1 auVar30 [16];
  float fVar31;
  undefined8 uVar32;
  float fVar33;
  float fVar34;
  float fVar35;
  float fVar36;
  float fVar37;
  undefined8 uVar38;
  float fVar39;
  float fVar40;
  undefined4 local_870;
  undefined4 local_86c;
  undefined4 uStack_868;
  undefined1 auStack_864 [4];
  float local_860;
  float fStack_85c;
  undefined4 local_858;
  undefined4 local_854;
  undefined8 local_850;
  undefined8 uStack_848;
  undefined4 local_840;
  undefined4 uStack_83c;
  undefined4 uStack_838;
  undefined8 uStack_834;
  long local_820;
  undefined8 local_818;
  undefined8 local_810;
  undefined4 local_808;
  undefined4 uStack_804;
  char *local_800;
  undefined *puStack_7f8;
  undefined *local_7f0;
  undefined8 local_7e8;
  uint local_7e0;
  float fStack_7dc;
  float fStack_7d8;
  float fStack_7d4;
  undefined8 local_7d0;
  undefined *local_7c8;
  uint *local_7c0;
  uint local_7b8;
  undefined4 local_7b4;
  uint local_7b0 [4];
  long local_7a0;
  undefined4 uStack_794;
  undefined8 uStack_790;
  float local_788 [11];
  int local_75c;
  long local_b0;
  
  lVar5 = tpidr_el0;
  local_b0 = *(long *)(lVar5 + 0x28);
  if (param_1 == (undefined8 *)0x0) goto LAB_00168358;
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
  param_1[0x13] = 0;
  param_1[0x12] = 0;
  param_1[0x15] = 0;
  param_1[0x14] = 0;
  param_1[0x17] = 0;
  param_1[0x16] = 0;
  param_1[0x19] = 0;
  param_1[0x18] = 0;
  param_1[0x1b] = 0;
  param_1[0x1a] = 0;
  param_1[0x1d] = 0;
  param_1[0x1c] = 0;
  param_1[0x1f] = 0;
  param_1[0x1e] = 0;
  param_1[0x21] = 0;
  param_1[0x20] = 0;
  param_1[1] = 0;
  *param_1 = 0;
  FUN_00168604(&DAT_00215aa0);
  FUN_0016881c(param_1,"kitNaniModEnabled",0x79,&DAT_00228600,&DAT_00228608,&DAT_00228610);
  if (*(int *)((long)param_1 + 0xc) != 0) goto LAB_00168358;
  local_850 = 0;
  puStack_7f8 = PTR_s_boltWallAvoidEnabled_001c57a0;
  local_800 = PTR_s_boltModEnabled_001c5798;
  local_7e8 = PTR_s_boltSafeExitEnabled_001c57b0;
  local_7f0 = PTR_s_boltPredictionEnabled_001c57a8;
  iVar11 = DAT_0020f694 + -1000000;
  if (999999 < DAT_0020f694 + 0xfefc99c0U) {
    iVar11 = DAT_0020f694;
  }
  fStack_7d8 = SUB164(_PTR_s_boltSmoothPercent_001c57b8,8);
  fStack_7d4 = SUB164(_PTR_s_boltSmoothPercent_001c57b8,0xc);
  local_7e0 = SUB164(_PTR_s_boltSmoothPercent_001c57b8,0);
  fStack_7dc = SUB164(_PTR_s_boltSmoothPercent_001c57b8,4);
  local_7c8 = PTR_s_boltTargetRange_001c57d0;
  local_7d0 = PTR_s_boltExitHoldMs_001c57c8;
  if ((((((iVar11 == 0xf4246a) && (DAT_00214930 != (code *)0x0)) &&
        (iVar11 = (*DAT_00214930)(&local_800,local_7b0,8,&local_850,&local_820), iVar11 == 1)) &&
       (((local_850 != 0 && ((int)local_820 == 0)) &&
        ((local_7b0[1] == 1 &&
         ((iVar12 = FUN_001550fc(), uVar27 = (undefined4)local_7a0, lVar17 = local_850,
          iVar9 = DAT_00215b00, puVar8 = DAT_00215ad0, lVar15 = DAT_00215ac0, uVar23 = DAT_00215aac,
          iVar7 = DAT_0020f7f4, iVar11 = DAT_0020f7f0, iVar12 != 0 && (local_75c != 0)))))))) &&
      (DAT_00215ab8 == DAT_0020f650)) &&
     ((((DAT_00215ac0 == DAT_0020f6f8 && (local_850 == DAT_00215ac8)) &&
       (DAT_00215b00 == DAT_0020f690)) && ((DAT_0020d15c & 1) != 0)))) {
    uVar32 = 0;
    uVar38 = 0;
    uVar29 = NEON_scvtf(CONCAT44(DAT_00215b04._4_4_,(int)DAT_00215b04),4);
    fVar34 = (float)uVar29 / 300.0;
    fVar36 = (float)((ulong)uVar29 >> 0x20) / 300.0;
    if ((DAT_0022872c == DAT_00215b00) && (uVar38 = 0, DAT_00228728 != 0)) {
      uVar38 = 0;
      uVar18 = DAT_00215aac - DAT_00228728;
      if ((DAT_00228728 <= DAT_00215aac && uVar18 != 0) && (uVar38 = 0, uVar18 < 9)) {
        fVar37 = (fVar34 - DAT_00228730) * (30.0 / (float)uVar18);
        fVar39 = (fVar36 - DAT_00228734) * (30.0 / (float)uVar18);
        uVar38 = CONCAT44(fVar39,fVar37);
        fVar37 = fVar39 * fVar39 + fVar37 * fVar37;
        if (fVar37 != 400.0 && fVar37 < 400.0 == NAN(fVar37)) {
          uVar38 = 0;
        }
      }
    }
    fVar37 = (float)NEON_ucvtf(DAT_0020f6bc);
    DAT_0022872c = DAT_00215b00;
    DAT_00228728 = DAT_00215aac;
    DAT_00228730 = fVar34;
    DAT_00228734 = fVar36;
    if (DAT_0020f6c8 == 0) {
      fVar26 = 0.0;
      fVar39 = 0.0;
      fVar35 = 0.0;
      iVar12 = 0;
    }
    else {
      uVar32 = 0;
      fVar35 = 0.0;
      fVar39 = 0.0;
      fVar40 = 1e+09;
      fVar26 = 0.0;
      uVar24 = 0;
      iVar12 = 0;
      fVar25 = ((float)local_75c / 100.0) * ((float)local_75c / 100.0);
      lVar22 = DAT_0020f6c0;
      do {
        plVar2 = (long *)(lVar22 + uVar24 * 0x40);
        lVar16 = lVar22;
        if (((*(int *)((long)plVar2 + 0x2c) != 0) && ((int)plVar2[5] != 0)) &&
           ((*(int *)(lVar22 + uVar24 * 0x40 + 0x30) != 0 && (*plVar2 != DAT_0020f680)))) {
          lVar19 = lVar22 + uVar24 * 0x40;
          iVar13 = FUN_0017fba8(DAT_0020f698,DAT_0020f69c,*(undefined4 *)(lVar19 + 0x18),
                                *(undefined4 *)(lVar19 + 0x1c));
          lVar16 = DAT_0020f6c0;
          if (iVar13 == 0) {
            fVar31 = *(float *)(lVar19 + 0x38) - fVar36;
            fVar33 = *(float *)(lVar19 + 0x34) - fVar34;
            fVar31 = (float)NEON_fmadd(fVar33,fVar33,fVar31 * fVar31);
            if ((0.04 <= fVar31) &&
               (fVar31 == fVar25 || fVar31 < fVar25 != (NAN(fVar31) || NAN(fVar25)))) {
              piVar1 = (int *)(lVar22 + uVar24 * 0x40 + 0x10);
              lVar20 = -0x1f00;
              do {
                if (((*plVar2 == *(long *)(lVar20 + 0x20f0c0)) &&
                    (iVar13 = *(int *)(lVar20 + 0x20f0c8), iVar13 == *piVar1)) &&
                   (*(int *)(lVar20 + 0x20f0e0) - 1U < uVar23)) {
                  uVar18 = uVar23 - *(int *)(lVar20 + 0x20f0e0);
                  if (0x12 < uVar18) goto LAB_001678d0;
                  uVar29 = *(undefined8 *)(lVar20 + 0x20f0d4);
                  goto LAB_00167a18;
                }
                lVar20 = lVar20 + 0xf8;
              } while (lVar20 != 0);
              iVar13 = *piVar1;
              uVar18 = 0;
              uVar29 = 0;
LAB_00167a18:
              fVar33 = (float)NEON_fmadd((float)uVar18,0x3f0ccccd,fVar31);
              fVar31 = fVar33 * 0.72;
              if (iVar13 != DAT_0022862c) {
                fVar31 = fVar33;
              }
              if (fVar31 < fVar40) {
                fVar35 = (float)NEON_ucvtf(*(undefined4 *)(lVar22 + uVar24 * 0x40 + 0x3c));
                fVar35 = fVar35 / 300.0;
                uVar32 = uVar29;
                fVar40 = fVar31;
                iVar12 = iVar13;
                fVar39 = *(float *)(lVar19 + 0x38);
                fVar26 = *(float *)(lVar19 + 0x34);
              }
            }
          }
        }
LAB_001678d0:
        uVar24 = uVar24 + 1;
        lVar22 = lVar16;
      } while (uVar24 < DAT_0020f6c8);
    }
    lVar22 = DAT_00215ab8;
    uVar6 = _UNK_00112958;
    uVar29 = _DAT_00112950;
    lVar16 = DAT_00215ac0;
    if ((((DAT_00228650 == 0) || (DAT_00228638 != DAT_00215ac0)) ||
        ((DAT_00228628 != iVar9 ||
         ((DAT_0022862c != iVar12 ||
          (lVar16 = DAT_00228638, lVar19 = DAT_00228738, DAT_00228738 != local_850)))))) &&
       (bVar10 = DAT_00228740 == -1, DAT_00228740 = DAT_00228740 + 1, lVar19 = local_850, bVar10)) {
      DAT_00228740 = 1;
    }
    lVar20 = DAT_00228740;
    DAT_00228738 = lVar19;
    param_1[6] = lVar19;
    param_1[7] = lVar20;
    *(uint *)(param_1 + 3) = (uint)(iVar12 == 0) << 1;
    *(undefined4 *)((long)param_1 + 0x1c) = 0;
    uVar18 = DAT_00215aac;
    param_1[1] = uVar6;
    *param_1 = uVar29;
    uVar6 = _UNK_00112848;
    uVar29 = _DAT_00112840;
    *(uint *)(param_1 + 0x17) = uVar23;
    *(int *)((long)param_1 + 0xbc) = iVar9;
    *(undefined4 *)(param_1 + 0xb) = 0;
    *(uint *)((long)param_1 + 0x5c) = uVar18;
    param_1[9] = uVar6;
    param_1[8] = uVar29;
    uVar29 = DAT_0010e680;
    param_1[0x13] = 0;
    param_1[0x14] = lVar15;
    *(undefined4 *)(param_1 + 0x19) = uStack_794;
    *(float *)((long)param_1 + 0xcc) = local_788[0];
    param_1[10] = uVar29;
    uVar6 = _UNK_00112c48;
    uVar29 = _DAT_00112c40;
    *(int *)(param_1 + 2) = iVar9;
    *(int *)((long)param_1 + 0x14) = iVar12;
    param_1[4] = lVar16;
    param_1[5] = lVar22;
    param_1[0xc] = lVar16;
    param_1[0xd] = lVar22;
    param_1[0xe] = lVar19;
    param_1[0x10] = uVar6;
    param_1[0xf] = uVar29;
    param_1[0x11] = 0;
    param_1[0x12] = 0;
    param_1[0x15] = lVar17;
    param_1[0x16] = puVar8;
    *(int *)(param_1 + 0x18) = iVar12;
    *(undefined4 *)((long)param_1 + 0xc4) = uVar27;
    *(float *)(param_1 + 0x1a) = local_788[2];
    *(float *)((long)param_1 + 0xd4) = local_788[5];
    *(float *)(param_1 + 0x1b) = local_788[8];
    *(ulong *)((long)param_1 + 0xdc) = CONCAT44(fVar36,fVar34);
    *(undefined8 *)((long)param_1 + 0xe4) = uVar38;
    *(float *)((long)param_1 + 0xec) = fVar37 / 300.0;
    *(float *)(param_1 + 0x1e) = (float)iVar11;
    *(float *)((long)param_1 + 0xf4) = (float)iVar7;
    *(float *)(param_1 + 0x1f) = fVar26;
    *(float *)((long)param_1 + 0xfc) = fVar39;
    param_1[0x20] = uVar32;
    *(float *)(param_1 + 0x21) = fVar35;
    *(undefined4 *)((long)param_1 + 0x10c) = 0;
    memcpy(&DAT_00228618,param_1,0x110);
    iVar11 = *(int *)((long)param_1 + 0xc);
  }
  else {
    uRam0000000000228630 = 0;
    _DAT_00228628 = 0;
    DAT_00228640 = 0;
    DAT_00228638 = 0;
    DAT_00228650 = 0;
    DAT_00228648 = 0;
    uRam0000000000228660 = 0;
    _DAT_00228658 = 0;
    uRam0000000000228670 = 0;
    _DAT_00228668 = 0;
    uRam0000000000228680 = 0;
    _DAT_00228678 = 0;
    uRam0000000000228690 = 0;
    _DAT_00228688 = 0;
    uRam00000000002286a0 = 0;
    _DAT_00228698 = 0;
    uRam00000000002286b0 = 0;
    _DAT_002286a8 = 0;
    uRam00000000002286c0 = 0;
    _DAT_002286b8 = 0;
    uRam00000000002286d0 = 0;
    _DAT_002286c8 = 0;
    uRam00000000002286e0 = 0;
    _DAT_002286d8 = 0;
    uRam00000000002286f0 = 0;
    _DAT_002286e8 = 0;
    uRam0000000000228700 = 0;
    _DAT_002286f8 = 0;
    uRam0000000000228710 = 0;
    _DAT_00228708 = 0;
    uRam0000000000228720 = 0;
    _DAT_00228718 = 0;
    uRam0000000000228620 = 0;
    _DAT_00228618 = 0;
    DAT_00228728 = 0;
    iVar11 = *(int *)((long)param_1 + 0xc);
  }
  if (iVar11 != 0) goto LAB_00168358;
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
  param_1[0x13] = 0;
  param_1[0x12] = 0;
  param_1[0x15] = 0;
  param_1[0x14] = 0;
  param_1[1] = 0;
  *param_1 = 0;
  iVar7 = DAT_0020f694;
  param_1[0x17] = 0;
  param_1[0x16] = 0;
  param_1[0x19] = 0;
  param_1[0x18] = 0;
  param_1[0x1b] = 0;
  param_1[0x1a] = 0;
  param_1[0x1d] = 0;
  param_1[0x1c] = 0;
  param_1[0x1f] = 0;
  param_1[0x1e] = 0;
  param_1[0x21] = 0;
  param_1[0x20] = 0;
  iVar11 = iVar7 + -1000000;
  if (999999 < iVar7 + 0xfefc99c0U) {
    iVar11 = iVar7;
  }
  if (iVar11 == 0xf42401) {
    local_850._4_4_ = (undefined4)((ulong)local_850 >> 0x20);
    local_850 = CONCAT44(local_850._4_4_,0xffffffff);
    local_800 = "coltModEnabled";
    if (((((((int)DAT_00214938 != 1) || (DAT_00214930 == (code *)0x0)) ||
          (iVar11 = (*DAT_00214930)(&local_800,local_7b0,1,&local_820,&local_850), iVar11 != 1)) ||
         ((local_820 == 0 || ((int)local_850 != 0)))) ||
        (((local_7b0[2] != 2 || ((local_7b0[1] != 1 || (iVar11 = FUN_001550fc(), iVar11 == 0)))) ||
         (DAT_00215ac8 != local_820)))) ||
       ((((DAT_00215ab8 != DAT_0020f650 || (DAT_00215b00 != DAT_0020f690)) ||
         (DAT_00215ac0 != DAT_0020f6f8)) || (lVar15 = FUN_00154d1c(local_7b0), lVar15 == 0))))
    goto LAB_00167f74;
    local_808 = *(undefined4 *)(lVar15 + 0x14);
    local_854 = *(undefined4 *)(lVar15 + 0x18);
    FUN_001887c4(&local_800,lVar15,&local_808,&local_854,&local_858);
    fVar36 = (float)(int)DAT_00215b04 / 300.0;
    fVar37 = (float)DAT_00215b04._4_4_ / 300.0;
    fVar34 = local_788[3] - fVar36;
    fVar39 = local_788[4] - fVar37;
    local_860 = fVar39;
    fStack_85c = fVar34;
    uVar27 = FUN_00187924(&DAT_0020f5c0,DAT_0020f694,DAT_002148b8,0);
    uVar27 = FUN_00189070(fVar34,fVar39,local_808,local_854,uVar27,0x3d0f5c29,0x3f800000,auStack_864
                         );
    uStack_868 = NEON_fmadd(local_808,uVar27,local_788[3]);
    local_86c = NEON_fmadd(local_854,uVar27,local_788[4]);
    fVar34 = hypotf(fVar34,fVar39);
    FUN_0018940c(local_858,fVar34,uVar27,&local_800,&uStack_868,&local_86c);
    FUN_001894e4(fVar36,fVar37,local_788[3],local_788[4],local_808,local_854,uVar27,&uStack_868,
                 &local_86c);
    local_870 = 0;
    iVar11 = FUN_0019a4f4(fVar36,fVar37,uStack_868,local_86c,&DAT_00214820,
                          (ulong)local_7d0 & 0xffffffff,fStack_7d4,&fStack_85c,&local_860,&local_870
                         );
    if (iVar11 < 0) goto LAB_00167f84;
    if (iVar11 == 0) {
      uVar27 = 1;
    }
    else {
      uStack_848 = _UNK_001c57e0;
      local_850 = _DAT_001c57d8;
      local_840 = SUB84(PTR_FUN_001c57e8,0);
      uStack_83c = (undefined4)((ulong)PTR_FUN_001c57e8 >> 0x20);
      FUN_001ba6c0(fVar36,fVar37,&DAT_00214860,&local_850,DAT_00215aac,&fStack_85c,&local_860);
      uVar27 = 2;
    }
    iVar7 = DAT_00215b00;
    lVar17 = DAT_00215ac0;
    *(undefined4 *)(param_1 + 9) = uVar27;
    lVar15 = DAT_00215ab8;
    uVar29 = _UNK_00112a28;
    uVar32 = _DAT_00112a20;
    *(int *)(param_1 + 2) = iVar7;
    *(undefined4 *)((long)param_1 + 0x14) = (undefined4)local_7a0;
    uVar23 = DAT_00215aac;
    uVar38 = DAT_00214828;
    param_1[1] = uVar29;
    *param_1 = uVar32;
    uVar32 = DAT_0010e6a8;
    param_1[3] = 0;
    param_1[4] = lVar17;
    uVar29 = DAT_0010e720;
    param_1[7] = uVar38;
    *(undefined8 *)((long)param_1 + 0x4c) = uVar32;
    param_1[8] = uVar29;
    param_1[5] = lVar15;
    param_1[6] = local_820;
    *(undefined4 *)((long)param_1 + 0x54) = 2;
    *(uint *)(param_1 + 0xb) = (uint)(iVar11 == 0);
    *(uint *)((long)param_1 + 0x5c) = uVar23;
    *(float *)(param_1 + 0xf) = fStack_85c;
    *(float *)((long)param_1 + 0x7c) = local_860;
    param_1[0xc] = lVar17;
    param_1[0xd] = lVar15;
    param_1[0xe] = local_820;
    *(undefined4 *)(param_1 + 0x10) = 0x43250000;
    *(undefined4 *)((long)param_1 + 0x84) = local_870;
    param_1[0x21] = 0;
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
    iVar11 = *(int *)((long)param_1 + 0xc);
  }
  else {
LAB_00167f74:
    DAT_00214828 = 0;
    DAT_00214820 = 0;
    _DAT_00214838 = 0;
    DAT_00214830 = 0;
    DAT_00214848 = 0;
    _DAT_00214840 = 0;
    uRam0000000000214858 = 0;
    _DAT_00214850 = 0;
LAB_00167f84:
    iVar11 = *(int *)((long)param_1 + 0xc);
  }
  if (iVar11 == 0) {
    local_850._4_4_ = (undefined4)((ulong)local_850 >> 0x20);
    local_850 = CONCAT44(local_850._4_4_,0xffffffff);
    local_800 = "autofarmEnabled";
    if ((((((int)DAT_00214938 == 1) && (DAT_00214930 != (code *)0x0)) &&
         ((iVar11 = (*DAT_00214930)(&local_800,local_7b0,1,&local_808,&local_850), iVar11 == 1 &&
          ((CONCAT44(uStack_804,local_808) != 0 && ((int)local_850 == 0)))))) &&
        ((local_7b0[2] == 2 &&
         (((local_7b0[1] == 1 && (iVar11 = FUN_001550fc(), iVar11 != 0)) &&
          (lVar15 = CONCAT44(uStack_804,local_808), lVar15 == DAT_00215ac8)))))) &&
       (((DAT_00215ab8 == DAT_0020f650 && (DAT_00215ac0 == DAT_0020f6f8)) &&
        ((DAT_00215b00 == DAT_0020f690 && (DAT_0020d15c != 0)))))) {
      puStack_7f8 = SUB168(_PTR_s_autofarmFollowTarget_001c57f0,8);
      local_800 = SUB168(_PTR_s_autofarmFollowTarget_001c57f0,0);
      if ((((DAT_00214930 != (code *)0x0) &&
           (iVar11 = (*DAT_00214930)(&local_800,local_7b0,2,&local_850,&local_820), iVar11 == 1)) &&
          (local_850 == lVar15)) &&
         ((((int)local_820 == 0 && (-1 < (int)local_7b0[0])) && ((int)local_7b0[0] < 2)))) {
        lVar15 = CONCAT44(uStack_804,local_808);
        if (DAT_002287a8 != lVar15) {
          uRam0000000000228750 = 0;
          _DAT_00228748 = 0;
          DAT_00228760 = 0;
          _DAT_00228758 = 0;
          uRam0000000000228770 = 0;
          _DAT_00228768 = 0;
          uRam0000000000228780 = 0;
          _DAT_00228778 = 0;
          uRam0000000000228790 = 0;
          _DAT_00228788 = 0;
          uRam00000000002287a0 = 0;
          _DAT_00228798 = 0;
          DAT_002287a8 = lVar15;
        }
        local_820 = 0;
        local_818 = 0;
        local_810 = 0;
        if (local_7b0[0] == 0) {
          FUN_00168bac(lVar15,&local_820);
        }
        uVar24 = (ulong)DAT_0020f6c8;
        if (DAT_0020f6c8 == 0) {
          uVar23 = 0;
        }
        else {
          uVar21 = 0;
          uVar23 = 0;
          lVar15 = DAT_0020f6c0;
          do {
            plVar2 = (long *)(lVar15 + uVar21 * 0x40);
            if ((((int)plVar2[5] != 0) && (*(int *)((long)plVar2 + 0x2c) != 0)) &&
               ((*(int *)(lVar15 + uVar21 * 0x40 + 0x30) != 0 && (*plVar2 != DAT_0020f680)))) {
              lVar15 = lVar15 + uVar21 * 0x40;
              uVar3 = *(uint *)(lVar15 + 0x10);
              uVar14 = FUN_0017fba8(DAT_0020f698,DAT_0020f69c,*(undefined4 *)(lVar15 + 0x18),
                                    *(undefined4 *)(lVar15 + 0x1c));
              iVar11 = FUN_00186cf0(DAT_0020f6b4,DAT_0020f6b8,*(undefined4 *)(lVar15 + 0x34),
                                    *(undefined4 *)(lVar15 + 0x38),DAT_0020f7f0,DAT_0020f7f4,
                                    FUN_00150fa0,0,0x40);
              uVar18 = *(uint *)(lVar15 + 0x24);
              fVar34 = 1.0;
              if ((uVar18 != 0) && (fVar34 = 1.0, *(uint *)(lVar15 + 0x20) <= uVar18)) {
                fVar34 = (float)*(uint *)(lVar15 + 0x20) / (float)uVar18;
              }
              lVar22 = *plVar2;
              uVar32 = *(undefined8 *)(lVar15 + 0x34);
              fVar36 = (float)FUN_00165c60(plVar2);
              lVar17 = -0x1f00;
              do {
                if ((((*plVar2 == *(long *)(lVar17 + 0x20f0c0)) &&
                     (*(uint *)(lVar17 + 0x20f0c8) == *(uint *)(lVar15 + 0x10))) &&
                    (*(int *)(lVar17 + 0x20f0e0) - 1U < DAT_00215aac)) &&
                   (uVar18 = DAT_00215aac - *(int *)(lVar17 + 0x20f0e0), uVar18 < 0x13)) {
                  fVar37 = (float)NEON_fmadd((float)uVar18,0xbda3d70a,0x3f800000);
                  if (fVar37 <= 0.15) {
                    fVar37 = 0.15;
                  }
                  uVar29 = *(undefined8 *)(lVar17 + 0x20f0d4);
                  fVar39 = 1.0;
                  if (4 < uVar18) {
                    fVar39 = fVar37;
                  }
                  goto LAB_001682bc;
                }
                lVar17 = lVar17 + 0xf8;
              } while (lVar17 != 0);
              uVar29 = 0;
              uVar18 = 0;
              fVar39 = 1.0;
LAB_001682bc:
              uVar4 = (ulong)uVar23;
              uVar23 = uVar23 + 1;
              uVar24 = (ulong)DAT_0020f6c8;
              local_7b0[uVar4 * 0xe] = uVar3;
              local_7b0[uVar4 * 0xe + 1] = uVar14;
              local_7b0[uVar4 * 0xe + 2] = uVar18;
              local_7b0[uVar4 * 0xe + 3] = (uint)(iVar11 == 1);
              (&local_7a0)[uVar4 * 7] = lVar22;
              *(undefined8 *)(&stack0xfffffffffffff868 + uVar4 * 0x38) = uVar32;
              (&uStack_790)[uVar4 * 7] = uVar29;
              local_788[uVar4 * 0xe] = fVar34;
              local_788[uVar4 * 0xe + 1] = fVar36;
              local_788[uVar4 * 0xe + 2] = fVar39;
              lVar15 = DAT_0020f6c0;
            }
            uVar21 = uVar21 + 1;
          } while ((uVar21 < uVar24) && (uVar23 < 0x20));
        }
        uVar27 = 1;
        uVar32 = NEON_scvtf(CONCAT44(DAT_00215b04._4_4_,(int)DAT_00215b04),4);
        local_7f0 = DAT_00215ad0;
        local_7e8 = (undefined *)CONCAT44(1,DAT_00215b00);
        fStack_7dc = (float)uVar32 / 300.0;
        fStack_7d8 = (float)((ulong)uVar32 >> 0x20) / 300.0;
        auVar30._8_8_ = DAT_00215ac0;
        auVar30._0_8_ = DAT_00215ab8;
        local_7e0 = local_7b0[0];
        auVar30 = NEON_ext(auVar30,auVar30,8,1);
        puStack_7f8 = auVar30._8_8_;
        local_800 = auVar30._0_8_;
        fStack_7d4 = 1.0;
        if ((DAT_0020f6a4 != 0) && (DAT_0020f6a0 <= DAT_0020f6a4)) {
          fStack_7d4 = (float)DAT_0020f6a0 / (float)DAT_0020f6a4;
        }
        uVar28 = FUN_00165c60(&DAT_0020f680);
        local_7c0 = local_7b0;
        local_7d0 = (undefined *)CONCAT44((float)DAT_0020f7f0,uVar28);
        local_7c8 = (undefined *)CONCAT44(local_7c8._4_4_,(float)DAT_0020f7f4);
        local_7b4 = (undefined4)local_818;
        local_7b8 = uVar23;
        iVar11 = FUN_0019a950(&DAT_00228748,&local_800,&local_850);
        uVar29 = _UNK_00112858;
        uVar32 = _DAT_00112850;
        param_1[0x21] = 0;
        uVar38 = DAT_00228760;
        lVar17 = DAT_00215ac0;
        lVar15 = DAT_00215ab8;
        *(int *)(param_1 + 2) = DAT_00215b00;
        uVar23 = DAT_00215aac;
        param_1[1] = uVar29;
        *param_1 = uVar32;
        *(undefined4 *)((long)param_1 + 0x1c) = local_850._4_4_;
        if (iVar11 != 0) {
          uVar27 = 2;
        }
        param_1[4] = lVar17;
        param_1[5] = lVar15;
        *(undefined8 *)((long)param_1 + 0x14) = uStack_848;
        uVar32 = DAT_0010e720;
        param_1[6] = CONCAT44(uStack_804,local_808);
        param_1[7] = uVar38;
        *(undefined4 *)(param_1 + 9) = uVar27;
        param_1[8] = uVar32;
        uVar32 = DAT_0010e6a8;
        *(uint *)((long)param_1 + 0x5c) = uVar23;
        param_1[0xc] = lVar17;
        param_1[0xd] = lVar15;
        param_1[0xe] = CONCAT44(uStack_804,local_808);
        *(undefined8 *)((long)param_1 + 0x4c) = uVar32;
        *(undefined4 *)((long)param_1 + 0x54) = local_840;
        *(uint *)(param_1 + 0xb) = (uint)(iVar11 == 0);
        param_1[0x10] = uStack_834;
        param_1[0xf] = CONCAT44(uStack_838,uStack_83c);
        param_1[0x12] = 0;
        param_1[0x11] = 0;
        param_1[0x14] = 0;
        param_1[0x13] = 0;
        param_1[0x16] = 0;
        param_1[0x15] = 0;
        param_1[0x18] = 0;
        param_1[0x17] = 0;
        param_1[0x1a] = 0;
        param_1[0x19] = 0;
        param_1[0x1c] = 0;
        param_1[0x1b] = 0;
        param_1[0x1e] = 0;
        param_1[0x1d] = 0;
        param_1[0x20] = 0;
        param_1[0x1f] = 0;
        goto LAB_00168358;
      }
    }
    uRam0000000000228750 = 0;
    _DAT_00228748 = 0;
    DAT_00228760 = 0;
    _DAT_00228758 = 0;
    uRam0000000000228770 = 0;
    _DAT_00228768 = 0;
    uRam0000000000228780 = 0;
    _DAT_00228778 = 0;
    uRam0000000000228790 = 0;
    _DAT_00228788 = 0;
    uRam00000000002287a0 = 0;
    _DAT_00228798 = 0;
    DAT_002287a8 = 0;
    if (*(int *)((long)param_1 + 0xc) == 0) {
      FUN_0016881c(param_1,"speedExploitEnabled",0x2e,&DAT_002287b0,&DAT_002287b8,&DAT_002287c0);
    }
  }
LAB_00168358:
  if (*(long *)(lVar5 + 0x28) == local_b0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

