/*
 * Aura (attack-range engine) — Feature
 * Decompiled with Ghidra 11.3.2 (arm64 pseudocode) from: libNexusEvasion69252.so, libNexusEvasionRuntime69252.so
 * Related menu entries (from embedded nexus-overlay-wire/v1):
 *   - menu.killaura "Kill aura" [free]
 *   - menu.autododge "Auto dodge" [free]
 *   - menu.follow "Follow" [Nexus+ PAID]
 *   - menu.aim "Smart aim" [free]
 *   - menu.xray "X-Ray" [Nexus+ PAID]
 *   - menu.hold "Hold fire" [free]
 *   - menu.spin "Spin" [Nexus+ PAID]
 *   - killauraEnabled "Kill aura" [free]
 *   - aopPredictEnabled "Prediction" [free]
 *   - killauraMainAttack "Main attack" [free]
 *   - killauraNoWall "Wall check" [free]
 *   - killauraNoBall "Ignore ball" [free]
 *   - autododgeEnabled "Auto dodge" [free]
 *   - aopAimEnabled "Smart aim" [free]
 *   - isSpinEnabled "Spin" [Nexus+ PAID]
 *   - followEnabled "Follow" [Nexus+ PAID]
 *   - followClosestAllyEnabled "Closest ally" [Nexus+ PAID]
 *   - ballAssistEnabled "Ball assist" [Nexus+ PAID]
 *   - holdToShootEnabled "Hold fire" [free]
 *   - isXrayEnabled "X-Ray" [Nexus+ PAID]
 *   - espEnabled "ESP" [Nexus+ PAID]
 *   - characterOutlineEnabled "Character outline" [free]
 *   - attackRangeIndicator "Attack range" [Nexus+ PAID]
 *   - hitboxRenderer "Hitboxes" [Nexus+ PAID]
 *   - enemyTracer "Enemy tracer" [Nexus+ PAID]
 *   - trophiesAboveHead "Trophies" [Nexus+ PAID]
 *   - pinEnabled "Auto pin" [Nexus+ PAID]
 *   - sprayEnabled "Auto spray" [Nexus+ PAID]
 *   - ... +205 more (see docs/feature_list.json)
 * Notes: Weapon-aware attack engine: candidates, weapon_radius, wall/ball checks, fire interval policy ("bounded_primary_policy").
 */

/* ===== nexus_evasion_prediction_snapshot_v1 @ 00113804 [libNexusEvasion69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int * nexus_evasion_prediction_snapshot_v1(int *param_1)

{
  char cVar1;
  bool bVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  int iVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  undefined8 uVar10;
  char cVar11;
  undefined **ppuVar12;
  int iVar13;
  int iVar14;
  int iVar15;
  int iVar16;
  int iVar17;
  int iVar18;
  int iVar19;
  int iVar20;
  int iVar21;
  int iVar22;
  int iVar23;
  int iVar24;
  int iVar25;
  int iVar26;
  int iVar27;
  int iVar28;
  int iVar29;
  int iVar30;
  int *piVar31;
  undefined **ppuVar32;
  int local_a8;
  
  piVar31 = param_1;
  if (param_1 != (int *)0x0) {
    if ((*param_1 == 1) && (param_1[1] == 0x88)) {
      do {
        cVar11 = DAT_00129ce8;
        cVar1 = '\x01';
        bVar2 = (bool)ExclusiveMonitorPass(0x129ce8,0x10);
        if (bVar2) {
          _DAT_00129ce8 = CONCAT31(DAT_00129ce8_1,1);
          cVar1 = ExclusiveMonitorsStatus();
        }
      } while (cVar1 != '\0' || cVar11 != '\0');
      FUN_0010f580();
      iVar13 = FUN_00113200();
      uVar5 = DAT_00129348;
      if (DAT_00129418 == (code *)0x0) {
        local_a8 = 0;
      }
      else {
        local_a8 = (*DAT_00129418)(DAT_00129410);
      }
      iVar6 = DAT_001293c4;
      ppuVar12 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar32 = ppuVar12;
        iVar14 = strcmp("killauraEnabled",*ppuVar32);
        ppuVar12 = ppuVar32 + 5;
      } while (iVar14 != 0);
      iVar14 = (&DAT_00129130)[*(int *)(ppuVar32 + 1)];
      ppuVar12 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar32 = ppuVar12;
        iVar15 = strcmp("killauraMainAttack",*ppuVar32);
        ppuVar12 = ppuVar32 + 5;
      } while (iVar15 != 0);
      iVar15 = (&DAT_00129130)[*(int *)(ppuVar32 + 1)];
      ppuVar12 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar32 = ppuVar12;
        iVar16 = strcmp("aopPredictEnabled",*ppuVar32);
        ppuVar12 = ppuVar32 + 5;
      } while (iVar16 != 0);
      iVar16 = (&DAT_00129130)[*(int *)(ppuVar32 + 1)];
      ppuVar12 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar32 = ppuVar12;
        iVar17 = strcmp("aopTargetMode",*ppuVar32);
        ppuVar12 = ppuVar32 + 5;
      } while (iVar17 != 0);
      iVar17 = (&DAT_00129130)[*(int *)(ppuVar32 + 1)];
      ppuVar12 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar32 = ppuVar12;
        iVar18 = strcmp("combatAuraRange",*ppuVar32);
        ppuVar12 = ppuVar32 + 5;
      } while (iVar18 != 0);
      iVar18 = (&DAT_00129130)[*(int *)(ppuVar32 + 1)];
      ppuVar12 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar32 = ppuVar12;
        iVar19 = strcmp("combatFireInterval",*ppuVar32);
        ppuVar12 = ppuVar32 + 5;
      } while (iVar19 != 0);
      iVar19 = (&DAT_00129130)[*(int *)(ppuVar32 + 1)];
      ppuVar12 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar32 = ppuVar12;
        iVar20 = strcmp("killauraDisableBelowHealthPercent",*ppuVar32);
        ppuVar12 = ppuVar32 + 5;
      } while (iVar20 != 0);
      iVar20 = (&DAT_00129130)[*(int *)(ppuVar32 + 1)];
      ppuVar12 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar32 = ppuVar12;
        iVar21 = strcmp("holdToShootEnabled",*ppuVar32);
        ppuVar12 = ppuVar32 + 5;
      } while (iVar21 != 0);
      iVar21 = (&DAT_00129130)[*(int *)(ppuVar32 + 1)];
      ppuVar12 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar32 = ppuVar12;
        iVar22 = strcmp("autofarmEnabled",*ppuVar32);
        ppuVar12 = ppuVar32 + 5;
      } while (iVar22 != 0);
      iVar22 = (&DAT_00129130)[*(int *)(ppuVar32 + 1)];
      ppuVar12 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar32 = ppuVar12;
        iVar23 = strcmp("xrayEnabled",*ppuVar32);
        ppuVar12 = ppuVar32 + 5;
      } while (iVar23 != 0);
      iVar23 = (&DAT_00129130)[*(int *)(ppuVar32 + 1)];
      ppuVar12 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar32 = ppuVar12;
        iVar24 = strcmp("coltModEnabled",*ppuVar32);
        ppuVar12 = ppuVar32 + 5;
      } while (iVar24 != 0);
      iVar24 = (&DAT_00129130)[*(int *)(ppuVar32 + 1)];
      ppuVar12 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar32 = ppuVar12;
        iVar25 = strcmp("aopPredictVersion",*ppuVar32);
        ppuVar12 = ppuVar32 + 5;
      } while (iVar25 != 0);
      iVar25 = (&DAT_00129130)[*(int *)(ppuVar32 + 1)];
      ppuVar12 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar32 = ppuVar12;
        iVar26 = strcmp("aopProjectileSpeed",*ppuVar32);
        ppuVar12 = ppuVar32 + 5;
      } while (iVar26 != 0);
      iVar26 = (&DAT_00129130)[*(int *)(ppuVar32 + 1)];
      ppuVar12 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar32 = ppuVar12;
        iVar27 = strcmp("aopLeadScale",*ppuVar32);
        ppuVar12 = ppuVar32 + 5;
      } while (iVar27 != 0);
      iVar27 = (&DAT_00129130)[*(int *)(ppuVar32 + 1)];
      ppuVar12 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar32 = ppuVar12;
        iVar28 = strcmp("aopReactionMs",*ppuVar32);
        ppuVar12 = ppuVar32 + 5;
      } while (iVar28 != 0);
      iVar28 = (&DAT_00129130)[*(int *)(ppuVar32 + 1)];
      ppuVar12 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar32 = ppuVar12;
        iVar29 = strcmp("aopAimEnabled",*ppuVar32);
        ppuVar12 = ppuVar32 + 5;
      } while (iVar29 != 0);
      iVar29 = (&DAT_00129130)[*(int *)(ppuVar32 + 1)];
      ppuVar12 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar32 = ppuVar12;
        iVar30 = strcmp("autododgeEnabled",*ppuVar32);
        lVar8 = DAT_001293f0;
        lVar7 = DAT_001293e8;
        ppuVar12 = ppuVar32 + 5;
      } while (iVar30 != 0);
      iVar30 = (&DAT_00129130)[*(int *)(ppuVar32 + 1)];
      piVar31 = (int *)0x1;
      param_1[0x16] = iVar27;
      param_1[0x17] = iVar28;
      lVar9 = DAT_001293f8;
      uVar4 = _UNK_001047f8;
      uVar3 = _DAT_001047f0;
      param_1[0x18] = iVar29;
      param_1[0x19] = iVar30;
      uVar10 = DAT_00129900;
      *(undefined8 *)(param_1 + 4) = uVar5;
      *(undefined8 *)(param_1 + 0x20) = uVar10;
      uVar5 = DAT_001293d0;
      *(undefined8 *)(param_1 + 2) = uVar4;
      *(undefined8 *)param_1 = uVar3;
      param_1[6] = local_a8;
      param_1[7] = iVar6;
      param_1[8] = iVar14;
      param_1[9] = iVar15;
      param_1[10] = iVar16;
      param_1[0xb] = iVar17;
      param_1[0xc] = iVar18;
      param_1[0xd] = iVar19;
      param_1[0xe] = iVar20;
      param_1[0xf] = iVar21;
      param_1[0x14] = iVar25;
      param_1[0x15] = iVar26;
      param_1[0x12] = iVar24;
      param_1[0x10] = iVar22;
      param_1[0x11] = iVar23;
      uVar3 = DAT_001298e8;
      param_1[0x1a] = (uint)(lVar7 != 0) | (uint)(lVar8 != 0) << 1 | (uint)(lVar9 != 0) << 2;
      param_1[0x1b] = iVar13;
      *(undefined8 *)(param_1 + 0x1c) = uVar3;
      *(undefined8 *)(param_1 + 0x1e) = uVar5;
      _DAT_00129ce8 = 0;
    }
    else {
      piVar31 = (int *)0x0;
    }
  }
  return piVar31;
}

/* ===== nexus_evasion_hold_snapshot_v1 @ 00116b5c [libNexusEvasion69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int * nexus_evasion_hold_snapshot_v1(int *param_1)

{
  char cVar1;
  bool bVar2;
  undefined8 uVar3;
  int iVar4;
  int iVar5;
  long lVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  char cVar10;
  undefined **ppuVar11;
  int iVar12;
  int iVar13;
  int iVar14;
  int iVar15;
  int iVar16;
  int iVar17;
  int iVar18;
  int iVar19;
  int iVar20;
  int iVar21;
  int iVar22;
  int iVar23;
  int iVar24;
  int iVar25;
  int iVar26;
  int iVar27;
  int iVar28;
  int iVar29;
  int iVar30;
  int iVar31;
  int iVar32;
  int iVar33;
  int iVar34;
  int iVar35;
  int iVar36;
  int iVar37;
  int iVar38;
  int iVar39;
  int iVar40;
  int iVar41;
  int iVar42;
  int *piVar43;
  undefined **ppuVar44;
  
  piVar43 = param_1;
  if (param_1 != (int *)0x0) {
    if ((*param_1 == 1) && (param_1[1] == 0xc0)) {
      do {
        cVar10 = DAT_00129ce8;
        cVar1 = '\x01';
        bVar2 = (bool)ExclusiveMonitorPass(0x129ce8,0x10);
        if (bVar2) {
          _DAT_00129ce8 = CONCAT31(DAT_00129ce8_1,1);
          cVar1 = ExclusiveMonitorsStatus();
        }
      } while (cVar1 != '\0' || cVar10 != '\0');
      FUN_0010f580();
      iVar12 = FUN_00116854();
      iVar13 = FUN_00117314(0);
      uVar9 = DAT_00129c28;
      uVar8 = DAT_00129bb8;
      iVar4 = DAT_00129b90;
      uVar7 = DAT_00129b50;
      lVar6 = DAT_00129b48;
      uVar3 = DAT_00129348;
      ppuVar11 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar44 = ppuVar11;
        iVar14 = strcmp("holdToShootEnabled",*ppuVar44);
        ppuVar11 = ppuVar44 + 5;
      } while (iVar14 != 0);
      iVar14 = (&DAT_00129130)[*(int *)(ppuVar44 + 1)];
      ppuVar11 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar44 = ppuVar11;
        iVar15 = strcmp("holdToShootAim",*ppuVar44);
        ppuVar11 = ppuVar44 + 5;
      } while (iVar15 != 0);
      iVar15 = (&DAT_00129130)[*(int *)(ppuVar44 + 1)];
      ppuVar11 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar44 = ppuVar11;
        iVar16 = strcmp("holdToShootRangeCheck",*ppuVar44);
        ppuVar11 = ppuVar44 + 5;
      } while (iVar16 != 0);
      iVar16 = (&DAT_00129130)[*(int *)(ppuVar44 + 1)];
      ppuVar11 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar44 = ppuVar11;
        iVar17 = strcmp("holdToShootDelayMs",*ppuVar44);
        iVar5 = DAT_001293c4;
        ppuVar11 = ppuVar44 + 5;
      } while (iVar17 != 0);
      iVar17 = (&DAT_00129130)[*(int *)(ppuVar44 + 1)];
      ppuVar11 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar44 = ppuVar11;
        iVar18 = strcmp("aopAimEnabled",*ppuVar44);
        ppuVar11 = ppuVar44 + 5;
      } while (iVar18 != 0);
      iVar18 = (&DAT_00129130)[*(int *)(ppuVar44 + 1)];
      ppuVar11 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar44 = ppuVar11;
        iVar19 = strcmp("aopPredictEnabled",*ppuVar44);
        ppuVar11 = ppuVar44 + 5;
      } while (iVar19 != 0);
      iVar19 = (&DAT_00129130)[*(int *)(ppuVar44 + 1)];
      ppuVar11 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar44 = ppuVar11;
        iVar20 = strcmp("aopPredictVersion",*ppuVar44);
        ppuVar11 = ppuVar44 + 5;
      } while (iVar20 != 0);
      iVar20 = (&DAT_00129130)[*(int *)(ppuVar44 + 1)];
      ppuVar11 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar44 = ppuVar11;
        iVar21 = strcmp("aopAimTargetMode",*ppuVar44);
        ppuVar11 = ppuVar44 + 5;
      } while (iVar21 != 0);
      iVar21 = (&DAT_00129130)[*(int *)(ppuVar44 + 1)];
      ppuVar11 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar44 = ppuVar11;
        iVar22 = strcmp("aopTargetMode",*ppuVar44);
        ppuVar11 = ppuVar44 + 5;
      } while (iVar22 != 0);
      iVar22 = (&DAT_00129130)[*(int *)(ppuVar44 + 1)];
      ppuVar11 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar44 = ppuVar11;
        iVar23 = strcmp("aopMaxRange",*ppuVar44);
        ppuVar11 = ppuVar44 + 5;
      } while (iVar23 != 0);
      iVar23 = (&DAT_00129130)[*(int *)(ppuVar44 + 1)];
      ppuVar11 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar44 = ppuVar11;
        iVar24 = strcmp("aopProjectileSpeed",*ppuVar44);
        ppuVar11 = ppuVar44 + 5;
      } while (iVar24 != 0);
      iVar24 = (&DAT_00129130)[*(int *)(ppuVar44 + 1)];
      ppuVar11 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar44 = ppuVar11;
        iVar25 = strcmp("aopLeadScale",*ppuVar44);
        ppuVar11 = ppuVar44 + 5;
      } while (iVar25 != 0);
      iVar25 = (&DAT_00129130)[*(int *)(ppuVar44 + 1)];
      ppuVar11 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar44 = ppuVar11;
        iVar26 = strcmp("aopReactionMs",*ppuVar44);
        ppuVar11 = ppuVar44 + 5;
      } while (iVar26 != 0);
      iVar26 = (&DAT_00129130)[*(int *)(ppuVar44 + 1)];
      ppuVar11 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar44 = ppuVar11;
        iVar27 = strcmp("killauraEnabled",*ppuVar44);
        ppuVar11 = ppuVar44 + 5;
      } while (iVar27 != 0);
      iVar27 = (&DAT_00129130)[*(int *)(ppuVar44 + 1)];
      ppuVar11 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar44 = ppuVar11;
        iVar28 = strcmp("killauraMainAttack",*ppuVar44);
        ppuVar11 = ppuVar44 + 5;
      } while (iVar28 != 0);
      iVar28 = (&DAT_00129130)[*(int *)(ppuVar44 + 1)];
      ppuVar11 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar44 = ppuVar11;
        iVar29 = strcmp("killauraSuper",*ppuVar44);
        ppuVar11 = ppuVar44 + 5;
      } while (iVar29 != 0);
      iVar29 = (&DAT_00129130)[*(int *)(ppuVar44 + 1)];
      ppuVar11 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar44 = ppuVar11;
        iVar30 = strcmp("killauraGadget",*ppuVar44);
        ppuVar11 = ppuVar44 + 5;
      } while (iVar30 != 0);
      iVar30 = (&DAT_00129130)[*(int *)(ppuVar44 + 1)];
      ppuVar11 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar44 = ppuVar11;
        iVar31 = strcmp("killauraNoWall",*ppuVar44);
        ppuVar11 = ppuVar44 + 5;
      } while (iVar31 != 0);
      iVar31 = (&DAT_00129130)[*(int *)(ppuVar44 + 1)];
      ppuVar11 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar44 = ppuVar11;
        iVar32 = strcmp("killauraNoBall",*ppuVar44);
        ppuVar11 = ppuVar44 + 5;
      } while (iVar32 != 0);
      iVar32 = (&DAT_00129130)[*(int *)(ppuVar44 + 1)];
      ppuVar11 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar44 = ppuVar11;
        iVar33 = strcmp("combatAuraRange",*ppuVar44);
        ppuVar11 = ppuVar44 + 5;
      } while (iVar33 != 0);
      iVar33 = (&DAT_00129130)[*(int *)(ppuVar44 + 1)];
      ppuVar11 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar44 = ppuVar11;
        iVar34 = strcmp("combatFireInterval",*ppuVar44);
        ppuVar11 = ppuVar44 + 5;
      } while (iVar34 != 0);
      iVar34 = (&DAT_00129130)[*(int *)(ppuVar44 + 1)];
      ppuVar11 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar44 = ppuVar11;
        iVar35 = strcmp("killauraDisableBelowHealthPercent",*ppuVar44);
        ppuVar11 = ppuVar44 + 5;
      } while (iVar35 != 0);
      iVar35 = (&DAT_00129130)[*(int *)(ppuVar44 + 1)];
      ppuVar11 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar44 = ppuVar11;
        iVar36 = strcmp("autofarmEnabled",*ppuVar44);
        ppuVar11 = ppuVar44 + 5;
      } while (iVar36 != 0);
      iVar36 = (&DAT_00129130)[*(int *)(ppuVar44 + 1)];
      ppuVar11 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar44 = ppuVar11;
        iVar37 = strcmp("xrayEnabled",*ppuVar44);
        ppuVar11 = ppuVar44 + 5;
      } while (iVar37 != 0);
      iVar37 = (&DAT_00129130)[*(int *)(ppuVar44 + 1)];
      ppuVar11 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar44 = ppuVar11;
        iVar38 = strcmp("coltModEnabled",*ppuVar44);
        ppuVar11 = ppuVar44 + 5;
      } while (iVar38 != 0);
      iVar38 = (&DAT_00129130)[*(int *)(ppuVar44 + 1)];
      ppuVar11 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar44 = ppuVar11;
        iVar39 = strcmp("dynaJumpEnabled",*ppuVar44);
        ppuVar11 = ppuVar44 + 5;
      } while (iVar39 != 0);
      iVar39 = (&DAT_00129130)[*(int *)(ppuVar44 + 1)];
      ppuVar11 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar44 = ppuVar11;
        iVar40 = strcmp("boltModEnabled",*ppuVar44);
        ppuVar11 = ppuVar44 + 5;
      } while (iVar40 != 0);
      iVar40 = (&DAT_00129130)[*(int *)(ppuVar44 + 1)];
      ppuVar11 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar44 = ppuVar11;
        iVar41 = strcmp("ballAssistEnabled",*ppuVar44);
        ppuVar11 = ppuVar44 + 5;
      } while (iVar41 != 0);
      iVar41 = (&DAT_00129130)[*(int *)(ppuVar44 + 1)];
      ppuVar11 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar44 = ppuVar11;
        iVar42 = strcmp("followEnabled",*ppuVar44);
        ppuVar11 = ppuVar44 + 5;
      } while (iVar42 != 0);
      piVar43 = (int *)0x1;
      iVar42 = (&DAT_00129130)[*(int *)(ppuVar44 + 1)];
      *(undefined8 *)(param_1 + 2) = uVar3;
      param_1[0xc] = (uint)(lVar6 != 0);
      param_1[0x29] = iVar38;
      param_1[0x2a] = iVar39;
      param_1[0x2b] = iVar40;
      param_1[0x2c] = iVar41;
      *(undefined8 *)(param_1 + 8) = uVar8;
      *(undefined8 *)(param_1 + 10) = uVar9;
      param_1[0x2f] = 0;
      uVar3 = DAT_001047d8;
      param_1[0xd] = iVar12;
      param_1[0xe] = iVar4;
      param_1[0xf] = iVar13;
      param_1[0x10] = iVar14;
      param_1[0x11] = iVar15;
      param_1[0x12] = iVar16;
      iVar4 = DAT_00129130;
      *(undefined8 *)param_1 = uVar3;
      param_1[0x13] = iVar17;
      param_1[0x14] = iVar5;
      param_1[0x2d] = iVar42;
      param_1[0x2e] = iVar4;
      *(undefined8 *)(param_1 + 6) = uVar7;
      *(long *)(param_1 + 4) = lVar6;
      param_1[0x15] = iVar18;
      param_1[0x16] = iVar19;
      param_1[0x17] = iVar20;
      param_1[0x18] = iVar21;
      param_1[0x19] = iVar22;
      param_1[0x1a] = iVar23;
      param_1[0x1b] = iVar24;
      param_1[0x1c] = iVar25;
      param_1[0x1d] = iVar26;
      param_1[0x1e] = iVar27;
      param_1[0x1f] = iVar28;
      param_1[0x20] = iVar29;
      param_1[0x21] = iVar30;
      param_1[0x22] = iVar31;
      param_1[0x23] = iVar32;
      param_1[0x24] = iVar33;
      param_1[0x25] = iVar34;
      param_1[0x26] = iVar35;
      param_1[0x27] = iVar36;
      param_1[0x28] = iVar37;
      _DAT_00129ce8 = 0;
    }
    else {
      piVar43 = (int *)0x0;
    }
  }
  return piVar43;
}

/* ===== nexus_evasion_aura_snapshot_v1 @ 0011aad0 [libNexusEvasion69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int * nexus_evasion_aura_snapshot_v1(int *param_1)

{
  char cVar1;
  bool bVar2;
  undefined8 uVar3;
  int iVar4;
  char cVar5;
  undefined **ppuVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  int iVar14;
  int iVar15;
  int iVar16;
  int iVar17;
  int *piVar18;
  undefined **ppuVar19;
  int local_88;
  
  piVar18 = param_1;
  if (param_1 != (int *)0x0) {
    if ((*param_1 == 1) && (param_1[1] == 0x48)) {
      do {
        cVar5 = DAT_00129ce8;
        cVar1 = '\x01';
        bVar2 = (bool)ExclusiveMonitorPass(0x129ce8,0x10);
        if (bVar2) {
          _DAT_00129ce8 = CONCAT31(DAT_00129ce8_1,1);
          cVar1 = ExclusiveMonitorsStatus();
        }
      } while (cVar1 != '\0' || cVar5 != '\0');
      FUN_0010f580();
      uVar3 = DAT_00129348;
      if (DAT_00129418 == (code *)0x0) {
        local_88 = 0;
      }
      else {
        local_88 = (*DAT_00129418)(DAT_00129410);
      }
      iVar4 = DAT_001293c4;
      ppuVar6 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar19 = ppuVar6;
        iVar7 = strcmp("killauraEnabled",*ppuVar19);
        ppuVar6 = ppuVar19 + 5;
      } while (iVar7 != 0);
      iVar7 = (&DAT_00129130)[*(int *)(ppuVar19 + 1)];
      ppuVar6 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar19 = ppuVar6;
        iVar8 = strcmp("killauraMainAttack",*ppuVar19);
        ppuVar6 = ppuVar19 + 5;
      } while (iVar8 != 0);
      iVar8 = (&DAT_00129130)[*(int *)(ppuVar19 + 1)];
      ppuVar6 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar19 = ppuVar6;
        iVar9 = strcmp("aopPredictEnabled",*ppuVar19);
        ppuVar6 = ppuVar19 + 5;
      } while (iVar9 != 0);
      iVar9 = (&DAT_00129130)[*(int *)(ppuVar19 + 1)];
      ppuVar6 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar19 = ppuVar6;
        iVar10 = strcmp("aopTargetMode",*ppuVar19);
        ppuVar6 = ppuVar19 + 5;
      } while (iVar10 != 0);
      iVar10 = (&DAT_00129130)[*(int *)(ppuVar19 + 1)];
      ppuVar6 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar19 = ppuVar6;
        iVar11 = strcmp("combatAuraRange",*ppuVar19);
        ppuVar6 = ppuVar19 + 5;
      } while (iVar11 != 0);
      iVar11 = (&DAT_00129130)[*(int *)(ppuVar19 + 1)];
      ppuVar6 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar19 = ppuVar6;
        iVar12 = strcmp("combatFireInterval",*ppuVar19);
        ppuVar6 = ppuVar19 + 5;
      } while (iVar12 != 0);
      iVar12 = (&DAT_00129130)[*(int *)(ppuVar19 + 1)];
      ppuVar6 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar19 = ppuVar6;
        iVar13 = strcmp("killauraDisableBelowHealthPercent",*ppuVar19);
        ppuVar6 = ppuVar19 + 5;
      } while (iVar13 != 0);
      iVar13 = (&DAT_00129130)[*(int *)(ppuVar19 + 1)];
      ppuVar6 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar19 = ppuVar6;
        iVar14 = strcmp("holdToShootEnabled",*ppuVar19);
        ppuVar6 = ppuVar19 + 5;
      } while (iVar14 != 0);
      iVar14 = (&DAT_00129130)[*(int *)(ppuVar19 + 1)];
      ppuVar6 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar19 = ppuVar6;
        iVar15 = strcmp("autofarmEnabled",*ppuVar19);
        ppuVar6 = ppuVar19 + 5;
      } while (iVar15 != 0);
      iVar15 = (&DAT_00129130)[*(int *)(ppuVar19 + 1)];
      ppuVar6 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar19 = ppuVar6;
        iVar16 = strcmp("xrayEnabled",*ppuVar19);
        ppuVar6 = ppuVar19 + 5;
      } while (iVar16 != 0);
      iVar16 = (&DAT_00129130)[*(int *)(ppuVar19 + 1)];
      ppuVar6 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar19 = ppuVar6;
        iVar17 = strcmp("coltModEnabled",*ppuVar19);
        ppuVar6 = ppuVar19 + 5;
      } while (iVar17 != 0);
      piVar18 = (int *)0x1;
      iVar17 = (&DAT_00129130)[*(int *)(ppuVar19 + 1)];
      param_1[0xc] = iVar13;
      param_1[0xd] = iVar14;
      *(undefined8 *)(param_1 + 2) = uVar3;
      uVar3 = DAT_001047a0;
      param_1[0xe] = iVar15;
      param_1[0xf] = iVar16;
      param_1[0x10] = iVar17;
      *(undefined8 *)param_1 = uVar3;
      param_1[4] = local_88;
      param_1[5] = iVar4;
      param_1[6] = iVar7;
      param_1[7] = iVar8;
      param_1[8] = iVar9;
      param_1[9] = iVar10;
      param_1[10] = iVar11;
      param_1[0xb] = iVar12;
      _DAT_00129ce8 = 0;
    }
    else {
      piVar18 = (int *)0x0;
    }
  }
  return piVar18;
}

/* ===== nexus_evasion_runtime_aura_route_v1 @ 00137970 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 nexus_evasion_runtime_aura_route_v1(int *param_1)

{
  char cVar1;
  bool bVar2;
  char cVar3;
  undefined8 uVar4;
  
  if (((param_1 == (int *)0x0) ||
      (((*param_1 == 1 && (param_1[1] == 0x10)) && (*(long *)(param_1 + 2) != 0)))) &&
     (DAT_001cfe90 == '\x01')) {
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
      if (param_1 == (int *)0x0) {
        DAT_001cfe98 = 0;
      }
      else {
        DAT_001cfe98 = *(undefined8 *)(param_1 + 2);
      }
      uVar4 = FUN_001813ac(&DAT_001cfea0,FUN_00137a10);
      _DAT_001cfe94 = 0;
      return uVar4;
    }
  }
  return 0;
}

/* ===== FUN_00137a10 @ 00137a10 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00137a10(undefined8 param_1,long param_2)

{
  long lVar1;
  int iVar2;
  undefined8 uVar3;
  int iVar4;
  ulong uVar5;
  undefined8 local_360;
  undefined8 uStack_358;
  undefined8 uStack_350;
  undefined8 local_348;
  undefined8 uStack_340;
  undefined8 local_338;
  undefined8 uStack_330;
  timespec local_328 [44];
  long local_68;
  
  lVar1 = tpidr_el0;
  local_68 = *(long *)(lVar1 + 0x28);
  uStack_330 = 0;
  local_338 = 0;
  uStack_340 = 0;
  local_348 = 0;
  uStack_350 = 0;
  uStack_358 = 0;
  local_360 = DAT_0010e7a8;
  _DAT_0020adbc = 0;
  _DAT_0020adb4 = 0;
  uRam000000000020adcc = 0;
  _DAT_0020adc4 = 0;
  uRam000000000020addc = 0;
  _DAT_0020add4 = 0;
  _DAT_0020ade4 = 0;
  if (DAT_0020adec == '\x01') {
    iVar2 = FUN_0018a8cc(&DAT_0020adf0,param_2,&local_360);
    iVar4 = 0;
    if ((iVar2 != 0) && ((int)uStack_358 != 0)) {
      iVar4 = 1;
      _DAT_0020adbc = uStack_358;
      _DAT_0020adb4 = local_360;
      uRam000000000020adcc = local_348;
      _DAT_0020adc4 = uStack_350;
      uRam000000000020addc = local_338;
      _DAT_0020add4 = uStack_340;
      _DAT_0020ade4 = uStack_330;
    }
  }
  else {
    iVar4 = 0;
  }
  iVar2 = clock_gettime(1,local_328);
  if (iVar2 == 0) {
    uVar5 = local_328[0].tv_sec * 1000 + (ulong)local_328[0].tv_nsec / 1000000;
  }
  else {
    uVar5 = 0;
  }
  iVar2 = DAT_0020adc0;
  if ((((DAT_0020aef0 == *(long *)(param_2 + 0x10)) && (DAT_0020aef8 == iVar4)) &&
      (DAT_0020aefc == DAT_0020adc0)) && (iVar2 = DAT_0020aefc, DAT_0020af00 == DAT_0020ade4)) {
    iVar2 = 0;
    if ((int)uStack_330 == 1) {
      iVar2 = iVar4;
    }
    if (((iVar2 != 1) || (uVar5 < DAT_0020af08)) ||
       (iVar2 = DAT_0020aefc, uVar5 - DAT_0020af08 < 2000)) goto LAB_00137c2c;
  }
  snprintf((char *)local_328,700,
           ",\"epoch\":%llu,\"sequence\":%llu,\"screen\":\"0x%llx\",\"client\":\"0x%llx\",\"own_gid\":%d,\"active_known\":%d,\"active_eligible\":%u,\"active_ended\":%u,\"native_wrapper_calls\":%llu,\"calls_while_ended\":%llu,\"last_call_mono_ms\":%llu"
           ,*(long *)(param_2 + 0x10),*(undefined8 *)(param_2 + 8),*(undefined8 *)(param_2 + 0x18),
           *(undefined8 *)(param_2 + 0x28),(ulong)*(uint *)(param_2 + 0x38),iVar4,iVar2,DAT_0020ade4
           ,DAT_0020af10,DAT_0020af18,DAT_0020af20);
  FUN_001417c8("aura_phase","typed_current_phase_and_call_counters",local_328);
  DAT_0020aef0 = *(long *)(param_2 + 0x10);
  DAT_0020aefc = DAT_0020adc0;
  DAT_0020af00 = DAT_0020ade4;
  DAT_0020aef8 = iVar4;
  DAT_0020af08 = uVar5;
LAB_00137c2c:
  if (iVar4 == 0) {
    uVar3 = 0xffffffff;
  }
  else if (uStack_358._4_4_ == 0) {
    uVar3 = 0;
  }
  else if (DAT_001cfe98 == (code *)0x0) {
    uVar3 = 1;
  }
  else {
    uVar3 = (*DAT_001cfe98)(0,param_2);
  }
  if (*(long *)(lVar1 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar3);
  }
  return;
}

/* ===== JNI_OnLoad @ 0014070c [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 JNI_OnLoad(long *param_1)

{
  bool bVar1;
  long lVar2;
  long *plVar3;
  __mode_t _Var4;
  char cVar5;
  int iVar6;
  int iVar7;
  __uid_t _Var8;
  uint uVar9;
  ssize_t sVar10;
  char *pcVar11;
  long *__s1;
  long lVar12;
  long *plVar13;
  long *plVar14;
  long *plVar15;
  undefined4 *puVar16;
  long lVar17;
  long lVar18;
  int *piVar19;
  long *plVar20;
  undefined8 local_9b0;
  undefined8 uStack_9a8;
  undefined *puStack_9a0;
  undefined *puStack_998;
  undefined8 local_988;
  long local_980;
  char *pcStack_978;
  char *local_970;
  code *pcStack_968;
  undefined8 local_960;
  undefined8 uStack_958;
  undefined8 local_950;
  int local_944;
  long *local_940;
  char acStack_938 [200];
  stat local_870 [14];
  long local_70;
  
  lVar2 = tpidr_el0;
  local_70 = *(long *)(lVar2 + 0x28);
  DAT_00209cf8 = param_1;
  iVar6 = open("/proc/self/stat",0x80000);
  if (iVar6 < 0) {
LAB_001409a0:
    pcVar11 = (char *)0x0;
  }
  else {
    memset(local_870,0,0x800);
    sVar10 = read(iVar6,local_870,0x7ff);
    close(iVar6);
    if (sVar10 < 1) goto LAB_001409a0;
    pcVar11 = strrchr((char *)local_870,0x29);
    if (pcVar11 != (char *)0x0) {
      if (pcVar11[1] != ' ') goto LAB_001409a0;
      local_940 = (long *)0x0;
      pcVar11 = strtok_r(pcVar11 + 2," ",(char **)&local_940);
      if (((((((pcVar11 != (char *)0x0) &&
              (pcVar11 = strtok_r((char *)0x0," ",(char **)&local_940), pcVar11 != (char *)0x0)) &&
             (pcVar11 = strtok_r((char *)0x0," ",(char **)&local_940), pcVar11 != (char *)0x0)) &&
            ((pcVar11 = strtok_r((char *)0x0," ",(char **)&local_940), pcVar11 != (char *)0x0 &&
             (pcVar11 = strtok_r((char *)0x0," ",(char **)&local_940), pcVar11 != (char *)0x0)))) &&
           (pcVar11 = strtok_r((char *)0x0," ",(char **)&local_940), pcVar11 != (char *)0x0)) &&
          ((((pcVar11 = strtok_r((char *)0x0," ",(char **)&local_940), pcVar11 != (char *)0x0 &&
             (pcVar11 = strtok_r((char *)0x0," ",(char **)&local_940), pcVar11 != (char *)0x0)) &&
            ((pcVar11 = strtok_r((char *)0x0," ",(char **)&local_940), pcVar11 != (char *)0x0 &&
             (((pcVar11 = strtok_r((char *)0x0," ",(char **)&local_940), pcVar11 != (char *)0x0 &&
               (pcVar11 = strtok_r((char *)0x0," ",(char **)&local_940), pcVar11 != (char *)0x0)) &&
              (pcVar11 = strtok_r((char *)0x0," ",(char **)&local_940), pcVar11 != (char *)0x0))))))
           && ((pcVar11 = strtok_r((char *)0x0," ",(char **)&local_940), pcVar11 != (char *)0x0 &&
               (pcVar11 = strtok_r((char *)0x0," ",(char **)&local_940), pcVar11 != (char *)0x0)))))
          ) && ((pcVar11 = strtok_r((char *)0x0," ",(char **)&local_940), pcVar11 != (char *)0x0 &&
                (((pcVar11 = strtok_r((char *)0x0," ",(char **)&local_940), pcVar11 != (char *)0x0
                  && (pcVar11 = strtok_r((char *)0x0," ",(char **)&local_940),
                     pcVar11 != (char *)0x0)) &&
                 ((pcVar11 = strtok_r((char *)0x0," ",(char **)&local_940), pcVar11 != (char *)0x0
                  && ((pcVar11 = strtok_r((char *)0x0," ",(char **)&local_940),
                      pcVar11 != (char *)0x0 &&
                      (pcVar11 = strtok_r((char *)0x0," ",(char **)&local_940),
                      pcVar11 != (char *)0x0)))))))))) {
        pcVar11 = (char *)strtoull(pcVar11,(char **)0x0,10);
      }
    }
  }
  DAT_00209d00 = pcVar11;
  __s1 = (long *)FUN_001417c8("initialize_enter","readonly_observer",0);
  local_940 = (long *)0x0;
  if (param_1 == (long *)0x0) {
    plVar20 = (long *)"jni_environment";
  }
  else {
    __s1 = (long *)(**(code **)(*param_1 + 0x30))(param_1,&local_940,0x10006);
    plVar20 = (long *)"jni_environment";
    if (((int)__s1 == 0) && (__s1 = local_940, local_940 != (long *)0x0)) {
      lVar12 = (**(code **)(*local_940 + 0x30))(local_940,"nexus/loader/NexusLoader");
      if (lVar12 == 0) {
        __s1 = (long *)0x0;
        plVar20 = (long *)"verified_loader_context";
      }
      else {
        __s1 = (long *)(**(code **)(*local_940 + 0x720))();
        plVar3 = local_940;
        plVar20 = (long *)"verified_loader_context";
        if (((ulong)__s1 & 0xff) == 0) {
          PTR_s_runtime_initialize_001cfb40 = s_verified_loader_context_0011e959;
          plVar13 = (long *)(**(code **)(*local_940 + 0x388))
                                      (local_940,lVar12,"applicationContext",
                                       "()Landroid/content/Context;");
          __s1 = plVar13;
          plVar20 = (long *)PTR_s_runtime_initialize_001cfb40;
          if (((((plVar13 != (long *)0x0) &&
                (__s1 = (long *)(**(code **)(*plVar3 + 0x720))(plVar3),
                plVar20 = (long *)PTR_s_runtime_initialize_001cfb40, ((ulong)__s1 & 0xff) == 0)) &&
               (plVar13 = (long *)(**(code **)(*plVar3 + 0x390))(plVar3,lVar12,plVar13),
               __s1 = plVar13, plVar20 = (long *)PTR_s_runtime_initialize_001cfb40,
               plVar13 != (long *)0x0)) &&
              (((__s1 = (long *)(**(code **)(*plVar3 + 0x720))(plVar3),
                plVar20 = (long *)PTR_s_runtime_initialize_001cfb40, ((ulong)__s1 & 0xff) == 0 &&
                (plVar14 = (long *)(**(code **)(*plVar3 + 0xf8))(plVar3,plVar13), __s1 = plVar14,
                plVar20 = (long *)PTR_s_runtime_initialize_001cfb40, plVar14 != (long *)0x0)) &&
               ((__s1 = (long *)(**(code **)(*plVar3 + 0x108))
                                          (plVar3,plVar14,"getPackageName","()Ljava/lang/String;"),
                plVar20 = (long *)PTR_s_runtime_initialize_001cfb40, __s1 != (long *)0x0 &&
                ((plVar15 = (long *)(**(code **)(*plVar3 + 0x110))(plVar3,plVar13,__s1),
                 __s1 = plVar15, plVar20 = (long *)PTR_s_runtime_initialize_001cfb40,
                 plVar15 != (long *)0x0 &&
                 (__s1 = (long *)(**(code **)(*plVar3 + 0x720))(plVar3),
                 plVar20 = (long *)PTR_s_runtime_initialize_001cfb40, ((ulong)__s1 & 0xff) == 0)))))
               ))) && (__s1 = (long *)(**(code **)(*plVar3 + 0x548))(plVar3,plVar15,0),
                      plVar20 = (long *)PTR_s_runtime_initialize_001cfb40, __s1 != (long *)0x0)) {
            iVar6 = strcmp((char *)__s1,"bsd.suitcase.nexusv2");
            __s1 = (long *)(**(code **)(*plVar3 + 0x550))(plVar3,plVar15,__s1);
            plVar20 = (long *)PTR_s_runtime_initialize_001cfb40;
            if (((((iVar6 == 0) &&
                  (__s1 = (long *)(**(code **)(*plVar3 + 0x108))
                                            (plVar3,plVar14,"getFilesDir","()Ljava/io/File;"),
                  plVar20 = (long *)PTR_s_runtime_initialize_001cfb40, __s1 != (long *)0x0)) &&
                 (plVar13 = (long *)(**(code **)(*plVar3 + 0x110))(plVar3,plVar13,__s1),
                 __s1 = plVar13, plVar20 = (long *)PTR_s_runtime_initialize_001cfb40,
                 plVar13 != (long *)0x0)) &&
                ((__s1 = (long *)(**(code **)(*plVar3 + 0xf8))(plVar3,plVar13),
                 plVar20 = (long *)PTR_s_runtime_initialize_001cfb40, __s1 != (long *)0x0 &&
                 (__s1 = (long *)(**(code **)(*plVar3 + 0x108))
                                           (plVar3,__s1,"getCanonicalPath","()Ljava/lang/String;"),
                 plVar20 = (long *)PTR_s_runtime_initialize_001cfb40, __s1 != (long *)0x0)))) &&
               ((plVar13 = (long *)(**(code **)(*plVar3 + 0x110))(plVar3,plVar13,__s1),
                __s1 = plVar13, plVar20 = (long *)PTR_s_runtime_initialize_001cfb40,
                plVar13 != (long *)0x0 &&
                ((__s1 = (long *)(**(code **)(*plVar3 + 0x720))(plVar3),
                 plVar20 = (long *)PTR_s_runtime_initialize_001cfb40, ((ulong)__s1 & 0xff) == 0 &&
                 (__s1 = (long *)(**(code **)(*plVar3 + 0x548))(plVar3,plVar13,0),
                 plVar20 = (long *)PTR_s_runtime_initialize_001cfb40, __s1 != (long *)0x0)))))) {
              PTR_s_runtime_initialize_001cfb40 = s_storage_private_files_00115442;
              iVar6 = FUN_00150078();
              __s1 = (long *)(**(code **)(*plVar3 + 0x550))(plVar3,plVar13,__s1);
              plVar20 = (long *)PTR_s_runtime_initialize_001cfb40;
              if (-1 < iVar6) {
                iVar7 = fstat(iVar6,local_870);
                _Var4 = local_870[0].st_mode;
                if (((iVar7 == 0) && (((uint)local_870[0].st_nlink & 0xf000) == 0x4000)) &&
                   (_Var8 = getuid(), _Var4 == _Var8)) {
                  uVar9 = getpid();
                  snprintf(&DAT_00209d08,0x80,"nexus-evasion-game-%d-%llu.jsonl",(ulong)uVar9,
                           DAT_00209d00);
                  DAT_001cfb48 = openat(iVar6,&DAT_00209d08,0x880c1,0x180);
                  if (DAT_001cfb48 < 0) {
LAB_0014128c:
                    pcVar11 = "false";
                  }
                  else {
                    iVar7 = fstat(DAT_001cfb48,local_870);
                    if (((iVar7 != 0) || (((uint)local_870[0].st_nlink & 0xf000) != 0x8000)) ||
                       ((_Var8 = getuid(), local_870[0].st_mode != _Var8 ||
                        (local_870[0].st_nlink._4_4_ != 1)))) {
                      close(DAT_001cfb48);
                      DAT_001cfb48 = -1;
                      goto LAB_0014128c;
                    }
                    pcVar11 = "true";
                    if (DAT_001cfb48 < 0) {
                      pcVar11 = "false";
                    }
                  }
                  DAT_001cfb50 = iVar6;
                  snprintf(acStack_938,200,",\"journal\":\"%s\",\"journal_open\":%s",&DAT_00209d08,
                           pcVar11);
                  FUN_001417c8("context_ready","actual_lab_context",acStack_938);
                  __s1 = (long *)FUN_001419f8();
                  plVar20 = (long *)PTR_s_runtime_initialize_001cfb40;
                  if ((int)__s1 != 0) {
                    local_870[0].st_ino._0_4_ = 0;
                    local_870[0].st_dev = DAT_0010e600;
                    dl_iterate_phdr(FUN_0015029c,local_870);
                    DAT_001cfb4c = local_870[0].st_dev._4_4_;
                    if ((int)local_870[0].st_ino != 1) {
                      DAT_001cfb4c = -1;
                    }
                    __s1 = (long *)sysconf(0x27);
                    if (DAT_001cfb4c < 0) {
                      plVar20 = (long *)"process_memory";
                    }
                    else if ((__s1 == (long *)0x4000) || (__s1 == (long *)0x1000)) {
                      DAT_00209d88 = __s1;
                      __s1 = (long *)dl_iterate_phdr(FUN_00141ed8,0);
                      plVar20 = (long *)"mapped_game_count";
                      if ((DAT_00209d90 == 1) &&
                         (plVar20 = (long *)"mapped_game_count", DAT_001e0978 != 0)) {
                        if (DAT_00209d94 == 1) {
                          local_944 = 0;
                          __s1 = (long *)dl_iterate_phdr(FUN_00142044,&local_944);
                          if (local_944 == 0) {
                            plVar20 = (long *)"mapped_game_BuildID";
                          }
                          else {
                            __s1 = (long *)FUN_00142158(&DAT_00209d98,0x1340c90,&DAT_0010ef58);
                            if ((int)__s1 == 0) {
                              plVar20 = (long *)"mapped_game_file_SHA";
                            }
                            else {
                              __s1 = (long *)FUN_00142748();
                              if ((int)__s1 == 0) {
                                plVar20 = (long *)"paired_backend_NOLOAD_exports";
                              }
                              else {
                                local_980 = DAT_001e0978;
                                pcStack_978 = 
                                "a10aeb6b4085fb2a15d969a130cd9608231998b5269a40aed41129d99624ede3";
                                local_970 = "fdf834103d333f9f8a1947b3a405b32da6ebb651";
                                pcStack_968 = FUN_001428fc;
                                local_960 = 0;
                                uStack_958 = DAT_0020ad98;
                                local_988 = DAT_0010e7f8;
                                local_950 = DAT_0020ada0;
                                __s1 = (long *)ng_bind_v1(&local_988);
                                if ((int)__s1 == 0) {
                                  plVar20 = (long *)"a10_passive_code_or_vtable_guards";
                                }
                                else {
                                  __s1 = (long *)FUN_00142d54();
                                  if ((int)__s1 == 0) {
                                    plVar20 = (long *)"aura_adapter_guards_or_activation_exports";
                                  }
                                  else {
                                    iVar6 = FUN_00143180();
                                    puVar16 = (undefined4 *)FUN_001bd828(&DAT_001cfbf8);
                                    *puVar16 = 0;
                                    if (iVar6 == 0) {
                                      piVar19 = (int *)FUN_001bd828(&DAT_001cfc18);
                                      iVar6 = *piVar19;
                                      __s1 = (long *)FUN_001bd828(&DAT_001cfc38);
                                      plVar20 = (long *)"function_routes_preparation";
                                      if (iVar6 != 0) {
                                        plVar20 = __s1;
                                      }
                                    }
                                    else {
                                      FUN_0014382c();
                                      FUN_001417c8("backend_bound",
                                                   "a10_and_paired_ack_backend_verified",
                                                                                                      
                                                  ",\"game_sha256\":\"a10aeb6b4085fb2a15d969a130cd9608231998b5269a40aed41129d99624ede3\",\"backend_sha256\":\"70bbfc332f9e8682046bf278782228a7478ae83cbe97d3e3ed6acab4bc2387c4\""
                                                  );
                                      FUN_00144010();
                                      DAT_0020ada8 = FUN_00144160(DAT_001e0978 + 0xb337c4);
                                      if (DAT_0020ada8 == 0) {
                                        __s1 = (long *)FUN_00144f20();
                                        plVar20 = __s1;
                                      }
                                      else {
                                        __s1 = (long *)FUN_001419f8();
                                        plVar20 = (long *)PTR_s_runtime_initialize_001cfb40;
                                        if ((int)__s1 != 0) {
                                          uStack_9a8 = _UNK_001c2d68;
                                          local_9b0 = _DAT_001c2d60;
                                          puStack_998 = PTR_FUN_001c2d78;
                                          puStack_9a0 = PTR_FUN_001c2d70;
                                          __s1 = (long *)FUN_0018b668(DAT_001e0978 + 0xb337c4,
                                                                      DAT_0020adb0,&local_9b0);
                                          DAT_00209cd0 = (int)__s1;
                                          if (DAT_00209cd0 < 0) {
                                            FUN_001417c8("fatal",
                                                  "entry_write_and_restore_unverified",0);
                                            FUN_00145114(param_1,"entry_restore_unverified",
                                                         "native_link_failed");
                    /* WARNING: Subroutine does not return */
                                            abort();
                                          }
                                          if (DAT_00209cd0 == 1) {
                                            __s1 = (long *)FUN_001454d8();
                                            if ((int)__s1 == 0) {
                                              plVar20 = (long *)
                                                  "owned_observer_postpublication_proof";
                                            }
                                            else {
                                              FUN_001417c8("observer_installed",
                                                           "one_preserving_preobserver",0);
                                              lVar17 = (**(code **)(*local_940 + 0x388))
                                                                 (local_940,lVar12,"NativeLoaded",
                                                                  "(Ljava/lang/String;)Z");
                                              if (((lVar17 == 0) ||
                                                  (lVar18 = (**(code **)(*local_940 + 0x538))
                                                                      (local_940,"EvasionGame"),
                                                  lVar18 == 0)) ||
                                                 (cVar5 = (**(code **)(*local_940 + 0x720))(),
                                                 cVar5 != '\0')) {
                                                pcVar11 = "callback_lookup_or_argument";
                                              }
                                              else {
                                                cVar5 = (**(code **)(*local_940 + 0x3a8))
                                                                  (local_940,lVar12,lVar17,lVar18);
                                                pcVar11 = "callback_invocation_or_rejected";
                                                DAT_00209cd4 = (uint)(cVar5 == '\x01');
                                              }
                                              cVar5 = (**(code **)(*local_940 + 0x720))();
                                              if (cVar5 == '\0') {
                                                if (DAT_00209cd4 == 0) goto LAB_0014157c;
                                              }
                                              else {
                                                (**(code **)(*local_940 + 0x88))();
                                                DAT_00209cd4 = 0;
LAB_0014157c:
                                                FUN_00145114(param_1,pcVar11,
                                                             "required_synchronous_callback_absent")
                                                ;
                                              }
                                              FUN_001813dc(&DAT_001cfea0,DAT_00209cd4 != 0);
                                              if (DAT_00209cd4 == 0) {
                                                uVar9 = 0;
                                              }
                                              else {
                                                FUN_001417c8("aura_policy",
                                                             "signed_local_free_feature_test",
                                                                                                                          
                                                  ",\"plus_entitlement_used\":false,\"paid_state_changed\":false"
                                                  );
                                                uVar9 = DAT_00209cd4;
                                              }
                                              __s1 = (long *)FUN_00146468(uVar9);
                                              if ((int)__s1 == 0) {
                                                plVar20 = (long *)"persistent_handler_registration";
                                              }
                                              else {
                                                FUN_001417c8("handler_registered",
                                                                                                                          
                                                  "implemented_main_free_scope_after_actual_loader_callback"
                                                  ,
                                                  ",\"implemented_handlers\":7,\"prediction_handler_registered\":true,\"lobby_ack_allowed\":true"
                                                  );
                                                __s1 = (long *)FUN_0014653c(DAT_00209cd4);
                                                if ((int)__s1 != 0) {
                                                  FUN_00146764(local_940,lVar12,DAT_00209cd4);
                                                  FUN_001474c0(DAT_00209cd4);
                                                  FUN_001476dc(DAT_00209cd4);
                                                  FUN_00147aec(DAT_00209cd4);
                                                  FUN_00137f5c();
                                                  FUN_00147cec(DAT_00209cd4);
                                                  FUN_00148054(DAT_00209cd4);
                                                  pcVar11 = "EvasionGame_rejected";
                                                  if (DAT_00209cd4 != 0) {
                                                    pcVar11 = "EvasionGame_acknowledged";
                                                  }
                                                  DAT_001dfff0._0_4_ = DAT_00209cd4;
                                                  FUN_001417c8("callback_return",pcVar11,0);
                                                  goto LAB_00140f2c;
                                                }
                                                plVar20 = (long *)
                                                  "functions_owned_proof_and_handlers";
                                              }
                                            }
                                          }
                                          else {
                                            plVar20 = (long *)
                                                  "entry_publication_failed_original_restored";
                                          }
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                        else {
                          plVar20 = (long *)"mapped_paired_backend_count";
                        }
                      }
                    }
                    else {
                      plVar20 = (long *)"page_size";
                    }
                  }
                }
                else {
                  uVar9 = close(iVar6);
                  __s1 = (long *)(ulong)uVar9;
                  plVar20 = (long *)PTR_s_runtime_initialize_001cfb40;
                }
              }
            }
          }
        }
      }
    }
  }
  iVar6 = DAT_00209cd8;
  if ((DAT_00209cd8 != 0) && (iVar7 = gettid(__s1), iVar6 == iVar7)) {
    local_870[0].st_dev = CONCAT44(local_870[0].st_dev._4_4_,0xffffffff);
    iVar6 = FUN_0014a4ac(&DAT_001e00e0,local_870);
    if (iVar6 != 0) {
      if (DAT_001e01b8 <= (int)local_870[0].st_dev) goto LAB_00140b20;
    }
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
LAB_00140b20:
  do {
    uVar9 = _DAT_001dff88;
    cVar5 = '\x01';
    bVar1 = (bool)ExclusiveMonitorPass(0x1dff88,0x10);
    if (bVar1) {
      _DAT_001dff88 = CONCAT31(DAT_001dff88_1,1);
      cVar5 = ExclusiveMonitorsStatus();
    }
  } while (cVar5 != '\0');
  if ((uVar9 & 1) == 0) {
    FUN_00138d54();
    if (DAT_001dff90 != 0) {
      DAT_001dffa0 = 0;
      DAT_001cfad8 = DAT_001cfad8 + 1;
      FUN_001393f4();
    }
    DAT_001dff90 = 0;
    DAT_001dff98 = 0;
    _DAT_001dff88 = 0;
  }
  DAT_001e01c8 = 0;
  DAT_001e01d0 = 0;
  do {
    uVar9 = _DAT_001e01d8;
    cVar5 = '\x01';
    bVar1 = (bool)ExclusiveMonitorPass(0x1e01d8,0x10);
    if (bVar1) {
      _DAT_001e01d8 = CONCAT31(DAT_001e01d8_1,1);
      cVar5 = ExclusiveMonitorsStatus();
    }
  } while (cVar5 != '\0');
  if ((uVar9 & 1) == 0) {
    memset(&DAT_001e01e8,0,0x784);
    DAT_001e01e0 = 0;
    _DAT_001e01d8 = 0;
  }
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
  DAT_001cfbf0 = 0xffffffff;
  FUN_0017f7c4(0,0,0xffffffff);
  FUN_0019c6e0(0,0);
  DAT_00228c58 = 0;
  DAT_0022b868 = 0;
  DAT_00214938._0_4_ = 0;
  if (DAT_00228ef0 != (code *)0x0) {
    (*DAT_00228ef0)(0);
  }
  DAT_002208a0 = 0;
  if (DAT_00228e40 != (code *)0x0) {
    (*DAT_00228e40)(0);
  }
  DAT_002208f8 = 0;
  DAT_00220784 = 0;
  nexus_visual_gl_disable_v1();
  if (DAT_00228e78 != (code *)0x0) {
    (*DAT_00228e78)(0);
  }
  DAT_00214940 = 0;
  DAT_00228e38 = (DAT_00228e38 & 0xffffffff00000000) + 0x100000000;
  if (DAT_00228e30 == '\x01') {
    (*DAT_00228e28)(0);
  }
  DAT_00216f04 = 0;
  _DAT_00214de0 = 0;
  DAT_00214dc8 = 0;
  DAT_00214dc0 = 0;
  DAT_00214dd8 = 0;
  DAT_00214dd0 = 0;
  DAT_00214db8 = 0;
  DAT_00214db0 = 0;
  DAT_00228e20 = (DAT_00228e20 & 0xffffffff00000000) + 0x100000000;
  if (DAT_00215db8 == '\x01') {
    (*DAT_00215d78)(0);
  }
  if (DAT_00228cf8 == '\x01') {
    DAT_00228cb8 = DAT_00228cb8 + 1;
  }
  DAT_00228cf8 = '\0';
  DAT_00228cc0 = 0;
  DAT_00228c74 = 0;
  DAT_00228ce8 = 0;
  DAT_00228cc8 = 0;
  DAT_00228cd0 = 0;
  DAT_00228cd8 = 0;
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
  DAT_002170b0 = 0;
  DAT_00228c98 = DAT_00228cb8;
  if ((((DAT_00216ef8 & 1) != 0) || (DAT_00216efc != '\0')) && (iVar6 = FUN_001419f8(), iVar6 != 0))
  {
    FUN_0015abf4();
  }
  DAT_002147dc = 0;
  DAT_00228dd8 = (DAT_00228dd8 & 0xffffffff00000000) + 0x100000000;
  if (DAT_00213058 == '\x01') {
    (*DAT_00213038)(0);
  }
  if ((DAT_002147d8 == '\x01') && (iVar6 = FUN_001419f8(), iVar6 != 0)) {
    iVar6 = FUN_001841d0(&DAT_00213130);
    if (iVar6 == 0) {
      DAT_002147d8 = '\0';
    }
    else {
      if (iVar6 == -1) {
        FUN_001417c8("fatal","event_route_restore_unverified",0);
                    /* WARNING: Subroutine does not return */
        abort();
      }
      FUN_001417c8("event_route_restore_pending","resident_disabled_owner",0);
    }
  }
  DAT_00228dd0 = (DAT_00228dd0 & 0xffffffff00000000) + 0x100000000;
  if (DAT_0020d130 != (code *)0x0) {
    (*DAT_0020d130)(0);
  }
  if ((local_940 != (long *)0x0) && (cVar5 = (**(code **)(*local_940 + 0x720))(), cVar5 != '\0')) {
    (**(code **)(*local_940 + 0x88))();
  }
  FUN_001417c8("disabled",plVar20,0);
  pcVar11 = "required_synchronous_callback_absent";
  if (DAT_00209cd4 != 0) {
    pcVar11 = "startup_failure";
  }
  FUN_00145114(param_1,plVar20,pcVar11);
LAB_00140f2c:
  if (*(long *)(lVar2 + 0x28) != local_70) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return 0x10006;
}

/* ===== FUN_00142d54 @ 00142d54 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00142d54(void)

{
  long lVar1;
  undefined8 uVar2;
  int iVar3;
  undefined8 uVar4;
  ulong uVar5;
  undefined8 local_150;
  undefined8 uStack_148;
  undefined *local_140;
  char *local_138;
  long local_130;
  char *pcStack_128;
  char *local_120;
  undefined8 uStack_118;
  code *local_110;
  code *pcStack_108;
  long local_100;
  code *pcStack_f8;
  code *local_f0;
  code *pcStack_e8;
  code *local_e0;
  code *pcStack_d8;
  code *local_d0;
  undefined8 local_c8;
  long local_c0;
  char *pcStack_b8;
  char *local_b0;
  undefined8 uStack_a8;
  code *local_a0;
  undefined8 local_98;
  long local_90;
  char *pcStack_88;
  char *local_80;
  code *pcStack_78;
  undefined8 local_70;
  long local_68;
  
  lVar1 = tpidr_el0;
  local_68 = *(long *)(lVar1 + 0x28);
  DAT_0020d130 = dlsym(DAT_0020d0f0,"nexus_evasion_register_handlers_v1");
  DAT_0020d138 = dlsym(DAT_0020d0f0,"nexus_evasion_attempt_stamp_v1");
  DAT_0020d140 = dlsym(DAT_0020d0f0,"nexus_evasion_attempt_recheck_v1");
  if (((((DAT_0020d130 != 0) && (iVar3 = dladdr(DAT_0020d130,&local_138), iVar3 != 0)) &&
       (local_130 == DAT_0020c0e8)) &&
      ((local_138 != (char *)0x0 && (iVar3 = strcmp(local_138,&DAT_0020c0f0), iVar3 == 0)))) &&
     ((((DAT_0020d138 != 0 &&
        ((iVar3 = dladdr(DAT_0020d138,&local_138), iVar3 != 0 && (local_130 == DAT_0020c0e8)))) &&
       (local_138 != (char *)0x0)) &&
      ((((iVar3 = strcmp(local_138,&DAT_0020c0f0), iVar3 == 0 && (DAT_0020d140 != 0)) &&
        (iVar3 = dladdr(DAT_0020d140,&local_138), iVar3 != 0)) &&
       (((local_130 == DAT_0020c0e8 && (local_138 != (char *)0x0)) &&
        (iVar3 = strcmp(local_138,&DAT_0020c0f0), iVar3 == 0)))))))) {
    DAT_0020d110 = dlsym(DAT_0020d0f0,"nexus_evasion_aura_snapshot_v1");
    DAT_0020d118 = (code *)dlsym(DAT_0020d0f0,"nexus_evasion_register_frame_family_v1");
    DAT_0020d120 = (code *)dlsym(DAT_0020d0f0,"nexus_evasion_bind_image_v1");
    DAT_0020d128 = dlsym(DAT_0020d0f0,"nexus_evasion_publish_observer_v1");
    if (((((DAT_0020d110 != 0) && (iVar3 = dladdr(DAT_0020d110,&local_138), iVar3 != 0)) &&
         ((local_130 == DAT_0020c0e8 &&
          (((local_138 != (char *)0x0 && (iVar3 = strcmp(local_138,&DAT_0020c0f0), iVar3 == 0)) &&
           (DAT_0020d118 != (code *)0x0)))))) &&
        ((iVar3 = dladdr(DAT_0020d118,&local_138), iVar3 != 0 && (local_130 == DAT_0020c0e8)))) &&
       (((local_138 != (char *)0x0 &&
         (((iVar3 = strcmp(local_138,&DAT_0020c0f0), iVar3 == 0 && (DAT_0020d120 != (code *)0x0)) &&
          ((iVar3 = dladdr(DAT_0020d120,&local_138), iVar3 != 0 &&
           ((((local_130 == DAT_0020c0e8 && (local_138 != (char *)0x0)) &&
             (iVar3 = strcmp(local_138,&DAT_0020c0f0), iVar3 == 0)) &&
            ((DAT_0020d128 != 0 && (iVar3 = dladdr(DAT_0020d128,&local_138), iVar3 != 0)))))))))) &&
        ((local_130 == DAT_0020c0e8 && (local_138 != (char *)0x0)))))) {
      iVar3 = strcmp(local_138,&DAT_0020c0f0);
      if ((iVar3 == 0) && (uVar5 = FUN_001808a0(), uVar2 = DAT_0010e580, uVar5 < 0x10001)) {
        local_70 = 0;
        local_98 = DAT_0010e580;
        local_90 = DAT_001e0978;
        pcStack_88 = "a10aeb6b4085fb2a15d969a130cd9608231998b5269a40aed41129d99624ede3";
        local_80 = "fdf834103d333f9f8a1947b3a405b32da6ebb651";
        pcStack_78 = FUN_001428fc;
        uVar4 = (*DAT_0020d120)(&local_98);
        if ((int)uVar4 == 0) goto LAB_00142e20;
        uVar5 = FUN_0018a664();
        if (uVar5 < 0x101) {
          local_c8 = uVar2;
          local_c0 = DAT_001e0978;
          pcStack_b8 = "a10aeb6b4085fb2a15d969a130cd9608231998b5269a40aed41129d99624ede3";
          local_b0 = "fdf834103d333f9f8a1947b3a405b32da6ebb651";
          uStack_a8 = 0;
          local_a0 = FUN_001428fc;
          uVar4 = FUN_0018a66c(&DAT_0020adf0,0x100,&local_c8);
          if ((int)uVar4 != 0) {
            DAT_0020adec = 1;
            local_120 = "fdf834103d333f9f8a1947b3a405b32da6ebb651";
            uStack_118 = 0;
            local_110 = FUN_001428fc;
            pcStack_108 = FUN_00150434;
            local_130 = DAT_001e0978;
            pcStack_128 = "a10aeb6b4085fb2a15d969a130cd9608231998b5269a40aed41129d99624ede3";
            local_138 = DAT_0010e800;
            local_100 = DAT_0020d110;
            pcStack_f8 = FUN_00137a10;
            local_f0 = FUN_00150584;
            pcStack_e8 = FUN_00150774;
            local_e0 = FUN_001507f0;
            pcStack_d8 = FUN_00150828;
            local_d0 = FUN_00150860;
            uVar4 = FUN_001808a8(&DAT_001cfea0,0x10000,&local_138);
            if ((int)uVar4 != 0) {
              uStack_148 = _UNK_001c31c8;
              local_150 = _DAT_001c31c0;
              local_140 = PTR_FUN_001c31d0;
              uVar4 = (*DAT_0020d118)(&local_150);
              if ((int)uVar4 != 0) {
                uVar4 = 1;
                DAT_001cfe90 = 1;
              }
            }
          }
          goto LAB_00142e20;
        }
      }
    }
  }
  uVar4 = 0;
LAB_00142e20:
  if (*(long *)(lVar1 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar4);
}

/* ===== FUN_00150584 @ 00150584 [libNexusEvasionRuntime69252.so] ===== */

void FUN_00150584(undefined8 param_1,uint *param_2,undefined4 param_3,uint param_4)

{
  char *pcVar1;
  long lVar2;
  int iVar3;
  ulong uVar4;
  timespec local_570 [75];
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
  
  lVar2 = tpidr_el0;
  local_58 = *(long *)(lVar2 + 0x28);
  iVar3 = clock_gettime(1,local_570);
  if (iVar3 == 0) {
    uVar4 = local_570[0].tv_sec * 1000 + (ulong)local_570[0].tv_nsec / 1000000;
  }
  else {
    uVar4 = 0;
  }
  if (param_2[1] != 0) {
    DAT_0020af10 = DAT_0020af10 + 1;
    DAT_0020af20 = uVar4;
    if ((DAT_0020adbc != 0) && (DAT_0020ade4 != 0)) {
      DAT_0020af18 = DAT_0020af18 + 1;
    }
  }
  if ((((*param_2 != DAT_0020d148) || (DAT_0020d150 == 0)) || (uVar4 < DAT_0020d150)) ||
     (999 < uVar4 - DAT_0020d150)) {
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
    DAT_0020d148 = *param_2;
    DAT_0020d150 = uVar4;
    FUN_00181410(&DAT_001cfea0,&local_c0);
    snprintf((char *)local_570,0x4b0,
             ",\"aura_reason\":%u,\"ready_mask\":%u,\"settings_generation\":%llu,\"frame_execution_evidence_ready\":%u,\"wrapper_called\":%u,\"wrapper_raw_result\":%d,\"candidates\":%u,\"worker_target\":\"0x%llx\",\"final_target\":\"0x%llx\",\"raw_x\":%d,\"raw_y\":%d,\"damage_proven\":false,\"weapon_known\":%u,\"weapon_name\":\"%s\",\"weapon_radius_raw\":%u,\"weapon_bypass\":%u,\"weapon_variants\":%u,\"weapon_scope\":\"raw_context\",\"active_known\":%u,\"active_eligible\":%u,\"active_route\":%d,\"active_ended\":%u"
             ,(ulong)*param_2,(ulong)param_4,*(undefined8 *)(param_2 + 10),(ulong)param_2[2],
             (ulong)param_2[1],param_2[3],param_3,*(undefined8 *)(param_2 + 6),
             *(undefined8 *)(param_2 + 8),param_2[4],param_2[5],(undefined4)local_c0,
             (long)&uStack_b0 + 4,local_c0._4_4_,(undefined4)uStack_b8,uStack_b8._4_4_,DAT_0020adbc,
             DAT_0020adc0,DAT_0020add0,DAT_0020ade4);
    pcVar1 = "bounded_primary_policy";
    if (param_2[1] != 0) {
      pcVar1 = "native_wrapper_invoked";
    }
    FUN_001417c8("aura_state",pcVar1,local_570);
  }
  if (*(long *)(lVar2 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

