/*
 * Kill Aura — Feature
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
 * Notes: Auto target selection + attack. Policy JSON marks aim/prediction/dodge as free_scope:2. Target routing via nexus_evasion_runtime_aura_route_v1 dispatcher.
 */

/* ===== FUN_0010f880 @ 0010f880 [libNexusEvasion69252.so] ===== */

char * FUN_0010f880(char *param_1)

{
  int iVar1;
  
  if (param_1 != (char *)0x0) {
    iVar1 = strcmp(param_1,"killauraNoWall");
    if ((((iVar1 == 0) || (iVar1 = strcmp(param_1,"killauraNoBall"), iVar1 == 0)) ||
        (iVar1 = strcmp(param_1,"espShowNames"), iVar1 == 0)) ||
       (((iVar1 = strcmp(param_1,"espShowDistance"), iVar1 == 0 ||
         (iVar1 = strcmp(param_1,"espShowHitbox"), iVar1 == 0)) ||
        (iVar1 = strcmp(param_1,"espShowCircle"), iVar1 == 0)))) {
      param_1 = (char *)0x1;
    }
    else {
      iVar1 = strcmp(param_1,"espShowAimLine");
      param_1 = (char *)(ulong)(iVar1 == 0);
    }
  }
  return param_1;
}

/* ===== FUN_001130c0 @ 001130c0 [libNexusEvasion69252.so] ===== */

undefined4 FUN_001130c0(char *param_1)

{
  undefined **ppuVar1;
  int iVar2;
  int iVar3;
  long lVar4;
  int *piVar5;
  ulong uVar6;
  undefined **ppuVar7;
  
  if (param_1 == (char *)0x0) {
    return 0xffffffff;
  }
  lVar4 = 0;
  while (param_1[lVar4] != '\0') {
    lVar4 = lVar4 + 1;
    if (lVar4 == 0x60) {
      return 0xffffffff;
    }
  }
  if (lVar4 == 0x60) {
    return 0xffffffff;
  }
  uVar6 = 0;
  piVar5 = &DAT_001216a0;
  while (iVar2 = strcmp(param_1,*(char **)(piVar5 + -2)), iVar2 != 0) {
    uVar6 = uVar6 + 1;
    piVar5 = piVar5 + 10;
    if (uVar6 == 0x8e) {
      return 0xffffffff;
    }
  }
  if (0x44 < uVar6) {
    return 0xffffffff;
  }
  iVar2 = *piVar5;
  ppuVar1 = &PTR_s_cameraEnabled_001216c0;
  do {
    ppuVar7 = ppuVar1;
    iVar3 = strcmp("killauraEnabled",*ppuVar7);
    ppuVar1 = ppuVar7 + 5;
  } while (iVar3 != 0);
  if (iVar2 == *(int *)(ppuVar7 + 1)) {
    return 0;
  }
  ppuVar1 = &PTR_s_cameraEnabled_001216c0;
  do {
    ppuVar7 = ppuVar1;
    iVar3 = strcmp("killauraMainAttack",*ppuVar7);
    ppuVar1 = ppuVar7 + 5;
  } while (iVar3 != 0);
  if (iVar2 == *(int *)(ppuVar7 + 1)) {
    return 1;
  }
  ppuVar1 = &PTR_s_cameraEnabled_001216c0;
  do {
    ppuVar7 = ppuVar1;
    iVar3 = strcmp("aopPredictEnabled",*ppuVar7);
    ppuVar1 = ppuVar7 + 5;
  } while (iVar3 != 0);
  if (iVar2 != *(int *)(ppuVar7 + 1)) {
    return 0xffffffff;
  }
  return 2;
}

/* ===== FUN_001133d4 @ 001133d4 [libNexusEvasion69252.so] ===== */

bool FUN_001133d4(uint param_1,uint param_2,undefined8 *param_3,undefined4 *param_4)

{
  bool bVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined **ppuVar6;
  bool bVar7;
  int iVar8;
  undefined4 uVar9;
  undefined **ppuVar10;
  
  ppuVar6 = &PTR_s_cameraEnabled_001216c0;
  do {
    ppuVar10 = ppuVar6;
    iVar8 = strcmp("killauraEnabled",*ppuVar10);
    ppuVar6 = ppuVar10 + 5;
  } while (iVar8 != 0);
  if ((&DAT_00129130)[*(int *)(ppuVar10 + 1)] != 0) {
    ppuVar6 = &PTR_s_cameraEnabled_001216c0;
    do {
      ppuVar10 = ppuVar6;
      iVar8 = strcmp("killauraMainAttack",*ppuVar10);
      ppuVar6 = ppuVar10 + 5;
    } while (iVar8 != 0);
    if ((&DAT_00129130)[*(int *)(ppuVar10 + 1)] != 0) {
      if (((uint)(DAT_001293e8 != 0) | (uint)(DAT_001293f0 != 0) << 1) == 3) {
        if (((param_1 ^ 0xffffffff) & 3) != 0) {
          bVar7 = false;
          uVar9 = 3;
          goto joined_r0x00113528;
        }
        ppuVar6 = &PTR_s_cameraEnabled_001216c0;
        do {
          ppuVar10 = ppuVar6;
          iVar8 = strcmp("aopPredictEnabled",*ppuVar10);
          ppuVar6 = ppuVar10 + 5;
        } while (iVar8 != 0);
        iVar8 = FUN_0011b604(param_1,(&DAT_00129130)[*(int *)(ppuVar10 + 1)]);
        if (iVar8 != 0) {
          bVar1 = ((param_2 ^ 0xffffffff) & 0x7f) == 0;
          bVar7 = bVar1 && DAT_001293c4 == 0;
          uVar9 = 0;
          if (!bVar1 || DAT_001293c4 != 0) {
            uVar9 = 5;
          }
          goto joined_r0x00113528;
        }
        uVar9 = 4;
      }
      else {
        uVar9 = 2;
      }
      bVar7 = false;
      goto joined_r0x00113528;
    }
  }
  bVar7 = false;
  uVar9 = 1;
joined_r0x00113528:
  if (param_4 != (undefined4 *)0x0) {
    *param_4 = uVar9;
  }
  uVar5 = DAT_00129900;
  uVar4 = DAT_001298e8;
  uVar3 = DAT_001293d0;
  uVar2 = DAT_00129348;
  if (bVar7) {
    *param_3 = DAT_001047a8;
    param_3[1] = uVar2;
    param_3[2] = uVar4;
    param_3[3] = uVar3;
    param_3[4] = uVar5;
  }
  return bVar7;
}

/* ===== FUN_00169084 @ 00169084 [libNexusEvasionRuntime69252.so] ===== */

void FUN_00169084(long param_1)

{
  uint uVar1;
  uint uVar2;
  long lVar3;
  bool bVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  float fVar8;
  int local_b4;
  long local_b0;
  undefined1 auStack_a8 [4];
  uint local_a4;
  int local_a0;
  uint local_98;
  int local_94;
  uint local_8c;
  int local_88;
  uint local_80;
  int local_7c;
  int iStack_78;
  int local_6c;
  undefined *local_60;
  undefined *puStack_58;
  undefined *puStack_50;
  undefined *puStack_48;
  undefined *local_40;
  undefined *puStack_38;
  long local_28;
  
  lVar3 = tpidr_el0;
  local_28 = *(long *)(lVar3 + 0x28);
  puStack_58 = PTR_s_killauraEnabled_001c5840;
  local_60 = PTR_s_autofarmEnabled_001c5838;
  puStack_48 = PTR_s_killauraGadget_001c5850;
  puStack_50 = PTR_s_killauraSuper_001c5848;
  puStack_38 = PTR_s_killauraDisableBelowHealthPercen_001c5860;
  local_40 = PTR_s_combatFireInterval_001c5858;
  if (DAT_00214930 != (code *)0x0) {
    iVar5 = (*DAT_00214930)(&local_60,auStack_a8,6,&local_b0,&local_b4);
    bVar4 = false;
    if (((iVar5 != 1) || (local_b0 == 0)) || (local_b4 != 0)) goto LAB_001691f8;
    if (((local_a4 < 2) && (local_98 < 2)) && ((local_8c < 2 && (local_80 < 2)))) {
      bVar4 = local_a4 == 1 && local_a0 == 2;
      uVar7 = (uint)(local_98 == 1 && local_94 == 2);
      uVar6 = (uint)bVar4;
      if (uVar6 == 0) {
        if ((DAT_0020f6a4 == 0) || (DAT_0020f6a4 < DAT_0020f6a0)) {
          fVar8 = 100.0;
        }
        else {
          fVar8 = ((float)DAT_0020f6a0 / (float)DAT_0020f6a4) * 100.0;
        }
        if (fVar8 < (float)local_6c) {
          uVar7 = 0;
        }
      }
      uVar2 = 0;
      if (local_8c == 1) {
        uVar2 = uVar7;
      }
      *(long *)(param_1 + 0x10) = local_b0;
      uVar1 = 0;
      if (local_88 == 2) {
        uVar1 = uVar2;
      }
      uVar2 = 0;
      if (local_80 == 1) {
        uVar2 = uVar7;
      }
      *(uint *)(param_1 + 0x38) = uVar6;
      *(uint *)(param_1 + 0x3c) = uVar1;
      uVar6 = 0;
      if (local_7c == 2) {
        uVar6 = uVar2;
      }
      if (iStack_78 < 0x79) {
        iStack_78 = 0x78;
      }
      *(uint *)(param_1 + 0x40) = uVar6;
      *(int *)(param_1 + 0x44) = iStack_78;
      if (((bVar4) || (uVar1 != 0)) || (uVar6 != 0)) {
        iVar5 = FUN_001550fc(0);
        bVar4 = iVar5 != 0;
        goto LAB_001691f8;
      }
    }
  }
  bVar4 = false;
LAB_001691f8:
  if (*(long *)(lVar3 + 0x28) == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(bVar4);
}

