/*
 * Character Outline — Feature
 * Decompiled with Ghidra 11.3.2 (arm64 pseudocode) from: libNexusEvasionRuntime69252.so
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
 * Notes: Patches the game GLSL at runtime: injects uniform u_nexusOutline and replaces vec4(v_outlineColor, 1.0) emission sites.
 */

/* ===== FUN_0016b860 @ 0016b860 [libNexusEvasionRuntime69252.so] ===== */

uint FUN_0016b860(void)

{
  char *pcVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  
  iVar2 = FUN_001419f8();
  if (iVar2 == 0) {
    uVar4 = 0;
  }
  else {
    iVar2 = FUN_00173c58(0x7ff030,0x35d4,
                         "2829b8cd1249c61c0fb4974f8f97f5a62715fd5f4802de9e9c08737fcb03ab6d",0x800a1c
                         ,0x3d809d20,FUN_00173eb4);
    iVar3 = FUN_00173c58(0x7c7c44,0x13e0,
                         "7b1f29aa558338aecdc24cd1d6e9839216b180e1cd0828d552c435061062a92a",0x7c88b0
                         ,0xbd027922,FUN_001743cc);
    uVar4 = (uint)(iVar3 != 0 && iVar2 != 0);
    pcVar1 = "register_observers_ready";
    if (uVar4 == 0) {
      pcVar1 = "register_observers_unavailable";
    }
    DAT_0022b868 = uVar4;
    FUN_001417c8("outline_native",pcVar1,0);
  }
  return uVar4;
}

/* ===== FUN_00173eb4 @ 00173eb4 [libNexusEvasionRuntime69252.so] ===== */

void FUN_00173eb4(long param_1)

{
  long *plVar1;
  undefined4 uVar2;
  char cVar3;
  bool bVar4;
  long lVar5;
  int iVar6;
  undefined4 *puVar7;
  undefined8 uVar8;
  char *pcVar9;
  char *pcVar10;
  ulong uVar11;
  long lVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  float fVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  float fVar20;
  long local_f8;
  ulong local_f0;
  ulong local_e8;
  uint local_e0;
  byte local_dc;
  byte local_db;
  byte local_da;
  byte local_d9;
  undefined1 auStack_d8 [32];
  float local_b8;
  float fStack_b4;
  float local_b0;
  float fStack_ac;
  char acStack_a8 [64];
  long local_68;
  
  lVar5 = tpidr_el0;
  local_68 = *(long *)(lVar5 + 0x28);
  puVar7 = (undefined4 *)__errno();
  uVar2 = *puVar7;
  do {
    cVar3 = '\x01';
    bVar4 = (bool)ExclusiveMonitorPass(0x22b870,0x10);
    if (bVar4) {
      cVar3 = ExclusiveMonitorsStatus();
      DAT_0022b870 = DAT_0022b870 + 1;
    }
  } while (cVar3 != '\0');
  uVar11 = 7;
  local_f0 = 0;
  local_e8 = 0;
  local_f8 = 0;
  local_e0 = 7;
  if (param_1 == 0) {
LAB_00174338:
    plVar1 = (long *)(uVar11 * 8 + 0x22b890);
    do {
      lVar12 = *plVar1;
      cVar3 = '\x01';
      bVar4 = (bool)ExclusiveMonitorPass(plVar1,0x10);
      if (bVar4) {
        *plVar1 = lVar12 + 1;
        cVar3 = ExclusiveMonitorsStatus();
      }
    } while (cVar3 != '\0');
    if (lVar12 != 0) goto LAB_00174388;
    snprintf(acStack_a8,0x40,",\"site\":%u,\"reason\":%u",0);
    pcVar9 = "register_observer_refused";
    pcVar10 = acStack_a8;
  }
  else {
    uVar8 = FUN_00174824(auStack_d8,&local_e0);
    if ((int)uVar8 == 0) {
      uVar11 = (ulong)local_e0;
      if (local_e0 - 10 < 0xfffffff7) goto LAB_00174388;
      goto LAB_00174338;
    }
    if ((((*(long *)(param_1 + 0x98) == 0) || (*(long *)(param_1 + 0x48) == 0)) ||
        (*(long *)(param_1 + 0x40) == 0)) ||
       (uVar8 = FUN_001428fc(uVar8,*(long *)(param_1 + 0x98) + 0x10,&local_e8,8), (int)uVar8 == 0))
    {
LAB_00174320:
      uVar11 = 7;
      goto LAB_00174338;
    }
    uVar11 = 7;
    if (((local_e8 + 0x2000 < 0x12000) || ((local_e8 & 7) != 0)) ||
       (local_e8 != *(ulong *)(param_1 + 0x48))) goto LAB_00174338;
    iVar6 = FUN_001428fc(uVar8,*(long *)(param_1 + 0x98) + 0x18,&local_f0,8);
    if (iVar6 == 0) goto LAB_00174320;
    uVar11 = 7;
    if ((local_f0 + 0x2000 < 0x12000) || ((local_f0 & 7) != 0)) goto LAB_00174338;
    uVar8 = FUN_0013a78c(local_f0 + 0xae0,&local_f8);
    if (((int)uVar8 == 0) ||
       ((local_f8 != *(long *)(param_1 + 0x40) ||
        (iVar6 = FUN_001428fc(uVar8,local_f8 + 9,&local_dc,4), iVar6 == 0)))) goto LAB_00174320;
    fVar13 = *(float *)(param_1 + 0x120);
    uVar11 = 8;
    if (((ABS(fVar13) == INFINITY) || (NAN(ABS(fVar13)))) || (2e-06 < ABS(fVar13 + -0.003921569)))
    goto LAB_00174338;
    fVar14 = ABS(*(float *)(param_1 + 0x110));
    if ((fVar14 == INFINITY) || (NAN(fVar14))) goto LAB_00174338;
    fVar14 = (float)NEON_ucvtf((uint)local_dc);
    fVar15 = ABS(fVar13 * fVar14);
    if ((fVar15 == INFINITY) ||
       ((NAN(fVar15) || (2e-06 < ABS(*(float *)(param_1 + 0x110) - fVar13 * fVar14)))))
    goto LAB_00174338;
    fVar14 = *(float *)(param_1 + 0x124);
    if ((ABS(fVar14) == INFINITY) || ((NAN(ABS(fVar14)) || (2e-06 < ABS(fVar14 + -0.003921569)))))
    goto LAB_00174338;
    fVar15 = ABS(*(float *)(param_1 + 0x114));
    if ((fVar15 == INFINITY) || (NAN(fVar15))) goto LAB_00174338;
    fVar15 = (float)NEON_ucvtf((uint)local_db);
    fVar17 = ABS(fVar14 * fVar15);
    if (((fVar17 == INFINITY) || (NAN(fVar17))) ||
       (2e-06 < ABS(*(float *)(param_1 + 0x114) - fVar14 * fVar15))) goto LAB_00174338;
    fVar15 = *(float *)(param_1 + 0x128);
    if (((ABS(fVar15) == INFINITY) || (NAN(ABS(fVar15)))) || (2e-06 < ABS(fVar15 + -0.003921569)))
    goto LAB_00174338;
    fVar17 = ABS(*(float *)(param_1 + 0x118));
    if ((fVar17 == INFINITY) || (NAN(fVar17))) goto LAB_00174338;
    fVar17 = (float)NEON_ucvtf((uint)local_da);
    fVar19 = ABS(fVar15 * fVar17);
    if ((fVar19 == INFINITY) ||
       ((NAN(fVar19) || (2e-06 < ABS(*(float *)(param_1 + 0x118) - fVar15 * fVar17)))))
    goto LAB_00174338;
    fVar17 = *(float *)(param_1 + 300);
    if ((ABS(fVar17) == INFINITY) || ((NAN(ABS(fVar17)) || (2e-06 < ABS(fVar17 + -0.003921569)))))
    goto LAB_00174338;
    fVar19 = ABS(*(float *)(param_1 + 0x11c));
    if ((fVar19 == INFINITY) || (NAN(fVar19))) goto LAB_00174338;
    fVar19 = (float)NEON_ucvtf((uint)local_d9);
    fVar20 = ABS(fVar17 * fVar19);
    if (((fVar20 == INFINITY) || (NAN(fVar20))) ||
       (2e-06 < ABS(*(float *)(param_1 + 0x11c) - fVar17 * fVar19))) goto LAB_00174338;
    iVar6 = FUN_00174ad8(auStack_d8);
    if (iVar6 == 0) {
      uVar11 = 9;
      goto LAB_00174338;
    }
    fVar18 = (float)NEON_ucvtf((int)(local_b8 * 255.0));
    fVar19 = (float)NEON_ucvtf((int)(fStack_b4 * 255.0));
    fVar20 = (float)NEON_ucvtf((int)(local_b0 * 255.0));
    fVar16 = (float)NEON_ucvtf((int)(fStack_ac * 255.0));
    *(float *)(param_1 + 0x110) = fVar13 * fVar18;
    *(float *)(param_1 + 0x114) = fVar14 * fVar19;
    *(float *)(param_1 + 0x118) = fVar15 * fVar20;
    *(float *)(param_1 + 0x11c) = fVar17 * fVar16;
    do {
      lVar12 = DAT_0022b880;
      cVar3 = '\x01';
      bVar4 = (bool)ExclusiveMonitorPass(0x22b880,0x10);
      if (bVar4) {
        cVar3 = ExclusiveMonitorsStatus();
        DAT_0022b880 = DAT_0022b880 + 1;
      }
    } while (cVar3 != '\0');
    if (lVar12 != 0) goto LAB_00174388;
    pcVar9 = "environment_rgba_applied";
    pcVar10 = (char *)0x0;
  }
  FUN_001417c8("outline_native",pcVar9,pcVar10);
LAB_00174388:
  *puVar7 = uVar2;
  if (*(long *)(lVar5 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_001743cc @ 001743cc [libNexusEvasionRuntime69252.so] ===== */

void FUN_001743cc(long param_1)

{
  long *plVar1;
  undefined4 uVar2;
  char cVar3;
  bool bVar4;
  long lVar5;
  int iVar6;
  undefined4 *puVar7;
  undefined8 uVar8;
  char *pcVar9;
  char *pcVar10;
  ulong uVar11;
  long lVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  char local_100 [4];
  byte local_fc [4];
  long local_f8;
  ulong local_f0;
  ulong local_e8;
  uint local_dc;
  float local_d8;
  float local_d4;
  float local_d0;
  float local_cc;
  undefined1 auStack_c8 [32];
  undefined8 local_a8;
  undefined4 local_a0;
  float local_9c;
  char acStack_98 [64];
  long local_58;
  
  lVar5 = tpidr_el0;
  local_58 = *(long *)(lVar5 + 0x28);
  puVar7 = (undefined4 *)__errno();
  uVar2 = *puVar7;
  do {
    cVar3 = '\x01';
    bVar4 = (bool)ExclusiveMonitorPass(0x22b878,0x10);
    if (bVar4) {
      cVar3 = ExclusiveMonitorsStatus();
      DAT_0022b878 = DAT_0022b878 + 1;
    }
  } while (cVar3 != '\0');
  uVar11 = 7;
  local_f0 = 0;
  local_e8 = 0;
  local_f8 = 0;
  local_fc[0] = 0;
  local_dc = 7;
  local_100[0] = '\0';
  if (param_1 == 0) {
LAB_0017478c:
    plVar1 = (long *)(uVar11 * 8 + 0x22b8e0);
    do {
      lVar12 = *plVar1;
      cVar3 = '\x01';
      bVar4 = (bool)ExclusiveMonitorPass(plVar1,0x10);
      if (bVar4) {
        *plVar1 = lVar12 + 1;
        cVar3 = ExclusiveMonitorsStatus();
      }
    } while (cVar3 != '\0');
    if (lVar12 != 0) goto LAB_001747e0;
    snprintf(acStack_98,0x40,",\"site\":%u,\"reason\":%u",1);
    pcVar9 = "register_observer_refused";
    pcVar10 = acStack_98;
  }
  else {
    uVar8 = FUN_00174824(auStack_c8,&local_dc);
    if ((int)uVar8 == 0) {
      uVar11 = (ulong)local_dc;
      if (local_dc - 10 < 0xfffffff7) goto LAB_001747e0;
      goto LAB_0017478c;
    }
    if ((((*(long *)(param_1 + 0xc0) == 0) || (*(long *)(param_1 + 0x40) == 0)) ||
        (*(long *)(param_1 + 0x48) == 0)) ||
       (uVar8 = FUN_001428fc(uVar8,*(long *)(param_1 + 0xc0),&local_e8,8), (int)uVar8 == 0)) {
LAB_00174774:
      uVar11 = 7;
      goto LAB_0017478c;
    }
    uVar11 = 7;
    if (((local_e8 + 0x2000 < 0x12000) || ((local_e8 & 7) != 0)) ||
       (local_e8 != *(ulong *)(param_1 + 0x40))) goto LAB_0017478c;
    uVar8 = FUN_001428fc(uVar8,*(long *)(param_1 + 0xc0) + 8,&local_f0,8);
    if ((int)uVar8 == 0) goto LAB_00174774;
    uVar11 = 7;
    if (((local_f0 + 0x2000 < 0x12000) || ((local_f0 & 7) != 0)) ||
       ((local_f0 != *(ulong *)(param_1 + 0x48) || (*(long *)(param_1 + 0x50) != local_e8 + 0x1a4)))
       ) goto LAB_0017478c;
    uVar8 = FUN_001428fc(uVar8,*(long *)(param_1 + 0x50),&local_d8,0x10);
    if ((int)uVar8 == 0) goto LAB_00174774;
    iVar6 = FUN_001428fc(uVar8,local_e8 + 0xb,local_100,1);
    uVar11 = 7;
    if ((iVar6 == 0) || (local_100[0] == '\0')) goto LAB_0017478c;
    uVar8 = FUN_0013a78c(param_1 + 0x338,&local_f8);
    if (((int)uVar8 == 0) || (iVar6 = FUN_001428fc(uVar8,local_f8 + 3,local_fc,1), iVar6 == 0))
    goto LAB_00174774;
    if (*(int *)(param_1 + 200) != 0x3b808081) {
      uVar11 = 8;
      goto LAB_0017478c;
    }
    uVar11 = 8;
    fVar13 = ABS(*(float *)(param_1 + 0x120));
    if ((((fVar13 == INFINITY) || (NAN(fVar13))) || (ABS(local_d8) == INFINITY)) ||
       ((NAN(ABS(local_d8)) || (2e-06 < ABS(*(float *)(param_1 + 0x120) - local_d8)))))
    goto LAB_0017478c;
    fVar13 = ABS(*(float *)(param_1 + 0x124));
    if ((((fVar13 == INFINITY) || ((NAN(fVar13) || (ABS(local_d4) == INFINITY)))) ||
        (NAN(ABS(local_d4)))) || (2e-06 < ABS(*(float *)(param_1 + 0x124) - local_d4)))
    goto LAB_0017478c;
    fVar13 = ABS(*(float *)(param_1 + 0x130));
    if ((((fVar13 == INFINITY) || (NAN(fVar13))) || (ABS(local_d0) == INFINITY)) ||
       ((NAN(ABS(local_d0)) || (2e-06 < ABS(*(float *)(param_1 + 0x130) - local_d0)))))
    goto LAB_0017478c;
    fVar13 = ABS(*(float *)(param_1 + 0x110));
    if ((fVar13 == INFINITY) || (NAN(fVar13))) goto LAB_0017478c;
    fVar15 = (float)NEON_ucvtf((uint)local_fc[0]);
    fVar13 = local_cc * 0.003921569 * fVar15;
    fVar14 = ABS(fVar13);
    if ((((((fVar14 == INFINITY) || (NAN(fVar14))) ||
          (2e-06 < ABS(*(float *)(param_1 + 0x110) - fVar13))) ||
         (((((local_d8 < 0.0 || (local_d8 != 1.0 && local_d8 < 1.0 == NAN(local_d8))) ||
            (local_d4 < 0.0)) ||
           ((local_d4 != 1.0 && local_d4 < 1.0 == NAN(local_d4) || (local_d0 < 0.0)))) ||
          ((local_d0 != 1.0 && local_d0 < 1.0 == NAN(local_d0) ||
           ((ABS(local_cc) == INFINITY || (NAN(ABS(local_cc)))))))))) || (local_cc < 0.0)) ||
       (local_cc != 1.0 && local_cc < 1.0 == NAN(local_cc))) goto LAB_0017478c;
    iVar6 = FUN_00174ad8(auStack_c8);
    if (iVar6 == 0) {
      uVar11 = 9;
      goto LAB_0017478c;
    }
    *(undefined8 *)(param_1 + 0x120) = local_a8;
    *(undefined4 *)(param_1 + 0x130) = local_a0;
    *(float *)(param_1 + 0x110) = local_9c * 0.003921569 * fVar15;
    do {
      lVar12 = DAT_0022b888;
      cVar3 = '\x01';
      bVar4 = (bool)ExclusiveMonitorPass(0x22b888,0x10);
      if (bVar4) {
        cVar3 = ExclusiveMonitorsStatus();
        DAT_0022b888 = DAT_0022b888 + 1;
      }
    } while (cVar3 != '\0');
    if (lVar12 != 0) goto LAB_001747e0;
    pcVar9 = "scene_rgba_applied";
    pcVar10 = (char *)0x0;
  }
  FUN_001417c8("outline_native",pcVar9,pcVar10);
LAB_001747e0:
  *puVar7 = uVar2;
  if (*(long *)(lVar5 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00174824 @ 00174824 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00174824(ulong *param_1,undefined4 *param_2)

{
  long lVar1;
  int iVar2;
  ulong uVar3;
  ulong uVar4;
  int local_ac;
  ulong local_a8;
  undefined1 auStack_a0 [4];
  int local_9c;
  int local_98;
  uint local_94;
  uint uStack_90;
  int local_8c;
  uint local_88;
  uint uStack_84;
  int local_80;
  uint local_7c;
  uint uStack_78;
  int local_74;
  uint local_70;
  uint uStack_6c;
  int local_68;
  undefined *local_60;
  undefined *puStack_58;
  undefined *puStack_50;
  undefined *puStack_48;
  undefined *local_40;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  param_1[3] = 0;
  param_1[2] = 0;
  param_1[5] = 0;
  param_1[4] = 0;
  param_1[1] = 0;
  *param_1 = 0;
  *param_2 = 1;
  if ((((int)_DAT_0022b868 == 1) && ((int)DAT_001dfff0 != 0)) && ((int)DAT_00214938 == 1)) {
    *param_2 = 2;
    uVar3 = FUN_0013c82c();
    if ((int)uVar3 == 0) goto LAB_00174ab0;
    *param_2 = 3;
    uVar3 = FUN_001428fc(uVar3,DAT_001e0978 + 0x1307e20,param_1,8);
    if ((int)uVar3 == 0) goto LAB_00174ab0;
    uVar4 = *param_1;
    uVar3 = 0;
    if ((uVar4 + 0x2000 < 0x12000) || ((uVar4 & 7) != 0)) goto LAB_00174ab0;
    uVar3 = FUN_001428fc(0,uVar4 + 0x50,param_1 + 1,4);
    if ((int)uVar3 == 0) goto LAB_00174ab0;
    iVar2 = (int)param_1[1];
    if (iVar2 != 4) {
      if (iVar2 != 5) goto LAB_00174aac;
      *param_2 = 4;
      uVar3 = FUN_00139fcc();
      param_1[2] = uVar3;
      if (uVar3 == 0) goto LAB_00174ab0;
    }
    puStack_48 = PTR_s_outlineColorB_001c58f8;
    puStack_50 = PTR_s_outlineColorG_001c58f0;
    puStack_58 = PTR_s_outlineColorR_001c58e8;
    local_60 = PTR_s_characterOutlineEnabled_001c58e0;
    local_a8 = 0;
    *param_2 = 5;
    local_ac = -1;
    local_40 = PTR_s_outlineOpacity_001c5900;
    if (DAT_00214930 != (code *)0x0) {
      iVar2 = (*DAT_00214930)(&local_60,auStack_a0,5,&local_a8,&local_ac);
      uVar3 = 0;
      if (((iVar2 != 1) || (local_a8 == 0)) ||
         ((local_ac != 0 || ((local_9c != 1 || (local_98 != 2)))))) goto LAB_00174ab0;
      *param_2 = 6;
      if (local_8c == 1) {
        uVar3 = 0;
        if ((uStack_90 != local_94) || (1000 < local_94)) goto LAB_00174ab0;
        *(float *)(param_1 + 4) = (float)(int)uStack_90 / 1000.0;
        if (local_80 == 1) {
          uVar3 = 0;
          if ((uStack_84 != local_88) || (1000 < local_88)) goto LAB_00174ab0;
          *(float *)((long)param_1 + 0x24) = (float)(int)uStack_84 / 1000.0;
          if (local_74 == 1) {
            uVar3 = 0;
            if ((uStack_78 != local_7c) || (1000 < local_7c)) goto LAB_00174ab0;
            *(float *)(param_1 + 5) = (float)(int)uStack_78 / 1000.0;
            if (local_68 == 1) {
              uVar3 = 0;
              if ((uStack_6c == local_70) && (local_70 < 0x3e9)) {
                uVar3 = 1;
                param_1[3] = local_a8;
                *(float *)((long)param_1 + 0x2c) = (float)(int)uStack_6c / 1000.0;
              }
              goto LAB_00174ab0;
            }
          }
        }
      }
    }
  }
LAB_00174aac:
  uVar3 = 0;
LAB_00174ab0:
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar3);
}

/* ===== FUN_0019b428 @ 0019b428 [libNexusEvasionRuntime69252.so] ===== */

char * FUN_0019b428(char *param_1,size_t param_2)

{
  char cVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  int iVar7;
  void *pvVar8;
  char *pcVar9;
  char *pcVar10;
  char *pcVar11;
  ulong uVar12;
  char *pcVar13;
  char *pcVar14;
  
  if ((((param_1 != (char *)0x0) && (0xfffffffffffbffff < param_2 - 0x40001)) &&
      (pvVar8 = memchr(param_1,0,param_2), pvVar8 == (void *)0x0)) &&
     ((param_1[param_2] == '\0' &&
      (pcVar9 = strstr(param_1,"u_nexusOutline"), pcVar9 == (char *)0x0)))) {
    pcVar9 = strstr(param_1,"void main");
    pcVar10 = strstr(param_1,"vec4(v_outlineColor, 1.0)");
    if (pcVar10 != (char *)0x0) {
      uVar12 = 0;
      param_2 = param_2 + 0x24;
      do {
        uVar12 = uVar12 + 1;
        pcVar10 = strstr(pcVar10 + 0x19,"vec4(v_outlineColor, 1.0)");
        param_2 = param_2 + 0x2c;
      } while (pcVar10 != (char *)0x0);
      if (pcVar9 == (char *)0x0) {
        return (char *)0x0;
      }
      if (uVar12 == 0) {
        return (char *)0x0;
      }
      if (0x40 < uVar12) {
        return (char *)0x0;
      }
      pcVar10 = (char *)malloc(param_2);
      if (pcVar10 == (char *)0x0) {
        return (char *)0x0;
      }
      cVar1 = *param_1;
      pcVar14 = pcVar10;
      while (cVar1 != '\0') {
        while( true ) {
          uVar3 = s_uniform_highp_vec4_u_nexusOutlin_00118198._24_8_;
          uVar2 = s_uniform_highp_vec4_u_nexusOutlin_00118198._16_8_;
          uVar5 = s_uniform_highp_vec4_u_nexusOutlin_00118198._8_8_;
          uVar4 = s_uniform_highp_vec4_u_nexusOutlin_00118198._0_8_;
          pcVar13 = pcVar14;
          if (param_1 == pcVar9) {
            pcVar14[0x1f] = 'n';
            pcVar14[0x20] = 'e';
            pcVar14[0x21] = ';';
            pcVar14[0x22] = '\n';
            *(undefined8 *)(pcVar14 + 8) = uVar5;
            *(undefined8 *)pcVar14 = uVar4;
            *(undefined8 *)(pcVar14 + 0x18) = uVar3;
            *(undefined8 *)(pcVar14 + 0x10) = uVar2;
            pcVar13 = pcVar14 + 0x23;
          }
          iVar7 = strncmp(param_1,"vec4(v_outlineColor, 1.0)",0x19);
          uVar5 = s__u_nexusOutline_a_<_0_0___vec4(v_00119287._48_8_;
          uVar4 = s__u_nexusOutline_a_<_0_0___vec4(v_00119287._32_8_;
          if (iVar7 == 0) break;
          pcVar11 = param_1 + 1;
          pcVar14 = pcVar13 + 1;
          *pcVar13 = *param_1;
          param_1 = pcVar11;
          if (*pcVar11 == '\0') goto LAB_0019b5bc;
        }
        uVar2 = CONCAT35(s__u_nexusOutline_a_<_0_0___vec4(v_00119287._61_3_,
                         s__u_nexusOutline_a_<_0_0___vec4(v_00119287._56_5_);
        param_1 = param_1 + 0x19;
        uVar3 = CONCAT53(s__u_nexusOutline_a_<_0_0___vec4(v_00119287._64_5_,
                         s__u_nexusOutline_a_<_0_0___vec4(v_00119287._61_3_);
        *(undefined8 *)(pcVar13 + 0x28) = s__u_nexusOutline_a_<_0_0___vec4(v_00119287._40_8_;
        *(undefined8 *)(pcVar13 + 0x20) = uVar4;
        *(undefined8 *)(pcVar13 + 0x38) = uVar2;
        *(undefined8 *)(pcVar13 + 0x30) = uVar5;
        uVar6 = s__u_nexusOutline_a_<_0_0___vec4(v_00119287._24_8_;
        uVar2 = s__u_nexusOutline_a_<_0_0___vec4(v_00119287._16_8_;
        uVar5 = s__u_nexusOutline_a_<_0_0___vec4(v_00119287._8_8_;
        uVar4 = s__u_nexusOutline_a_<_0_0___vec4(v_00119287._0_8_;
        *(undefined8 *)(pcVar13 + 0x3d) = uVar3;
        *(undefined8 *)(pcVar13 + 8) = uVar5;
        *(undefined8 *)pcVar13 = uVar4;
        *(undefined8 *)(pcVar13 + 0x18) = uVar6;
        *(undefined8 *)(pcVar13 + 0x10) = uVar2;
        pcVar14 = pcVar13 + 0x45;
        cVar1 = *param_1;
      }
LAB_0019b5bc:
      *pcVar14 = '\0';
      return pcVar10;
    }
  }
  return (char *)0x0;
}

