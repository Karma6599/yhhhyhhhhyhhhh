/*
 * Spin — Feature
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
 * Notes: Server-visible spin movement incl. online-only-idle modes and movement mode selection.
 */

/* ===== FUN_0010f504 @ 0010f504 [libNexusEvasion69252.so] ===== */

undefined ** FUN_0010f504(char *param_1)

{
  int iVar1;
  long lVar2;
  undefined **ppuVar3;
  
  if (param_1 != (char *)0x0) {
    lVar2 = 0;
    do {
      if (param_1[lVar2] == '\0') {
        if (lVar2 == 0x60) {
          return (undefined **)0x0;
        }
        lVar2 = 0x8e;
        ppuVar3 = &PTR_s_isSpinEnabled_00121698;
        do {
          iVar1 = strcmp(param_1,*ppuVar3);
          if (iVar1 == 0) {
            return ppuVar3;
          }
          lVar2 = lVar2 + -1;
          ppuVar3 = ppuVar3 + 5;
        } while (lVar2 != 0);
        return (undefined **)0x0;
      }
      lVar2 = lVar2 + 1;
    } while (lVar2 != 0x60);
  }
  return (undefined **)0x0;
}

/* ===== nexus_evasion_get_port_state @ 0010fc18 [libNexusEvasion69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulong nexus_evasion_get_port_state(char *param_1)

{
  char cVar1;
  bool bVar2;
  char cVar3;
  int iVar4;
  long lVar5;
  ulong uVar6;
  int *piVar7;
  
  if (param_1 != (char *)0x0) {
    lVar5 = 0;
    do {
      if (param_1[lVar5] == '\0') {
        if (lVar5 == 0x60) {
          return 0xffffffff;
        }
        uVar6 = 0xffffffffffffffbb;
        piVar7 = &DAT_001216b8;
        do {
          iVar4 = strcmp(param_1,*(char **)(piVar7 + -8));
          if (iVar4 == 0) {
            iVar4 = FUN_0010f880(param_1);
            if (iVar4 != 0) {
              return 1;
            }
            if (uVar6 + 0x45 < 0x45) {
              if (*piVar7 == 0) {
                do {
                  cVar3 = DAT_00129ce8;
                  cVar1 = '\x01';
                  bVar2 = (bool)ExclusiveMonitorPass(0x129ce8,0x10);
                  if (bVar2) {
                    _DAT_00129ce8 = CONCAT31(DAT_00129ce8_1,1);
                    cVar1 = ExclusiveMonitorsStatus();
                  }
                } while (cVar1 != '\0' || cVar3 != '\0');
                FUN_0010f580();
                iVar4 = FUN_0010f934(param_1);
                _DAT_00129ce8 = 0;
                return (ulong)(iVar4 != 0) << 1;
              }
            }
            else if (*piVar7 == 0) {
              return (ulong)(uVar6 < 0x49);
            }
            return 3;
          }
          piVar7 = piVar7 + 10;
          uVar6 = uVar6 + 1;
        } while (uVar6 != 0x49);
        return 0xffffffff;
      }
      lVar5 = lVar5 + 1;
    } while (lVar5 != 0x60);
  }
  return 0xffffffff;
}

/* ===== FUN_00112be8 @ 00112be8 [libNexusEvasion69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int * FUN_00112be8(int *param_1)

{
  uint uVar1;
  int iVar2;
  char cVar3;
  int iVar4;
  undefined8 uVar5;
  long lVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  char cVar9;
  undefined **ppuVar10;
  bool bVar11;
  bool bVar12;
  int iVar13;
  int iVar14;
  int *piVar15;
  int iVar16;
  uint uVar17;
  undefined **ppuVar18;
  
  piVar15 = param_1;
  if (param_1 != (int *)0x0) {
    if ((*param_1 == 1) && (param_1[1] == 0x48)) {
      do {
        cVar9 = DAT_00129ce8;
        cVar3 = '\x01';
        bVar11 = (bool)ExclusiveMonitorPass(0x129ce8,0x10);
        if (bVar11) {
          _DAT_00129ce8 = CONCAT31(DAT_00129ce8_1,1);
          cVar3 = ExclusiveMonitorsStatus();
        }
      } while (cVar3 != '\0' || cVar9 != '\0');
      FUN_0010f580();
      iVar13 = FUN_001158a0();
      iVar4 = DAT_00129130;
      ppuVar10 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar18 = ppuVar10;
        iVar14 = strcmp("spinMovementMode",*ppuVar18);
        ppuVar10 = ppuVar18 + 5;
      } while (iVar14 != 0);
      iVar14 = FUN_0011c394((&DAT_00129130)[*(int *)(ppuVar18 + 1)]);
      uVar8 = DAT_00129b30;
      uVar7 = DAT_00129ac0;
      lVar6 = DAT_00129a60;
      uVar5 = DAT_00129348;
      uVar17 = 0;
      if (iVar13 != 0) {
        uVar17 = (uint)(iVar4 != 0);
      }
      iVar16 = 3;
      if (iVar13 != 0) {
        iVar16 = 1;
      }
      bVar11 = DAT_00129a60 != 0;
      iVar2 = 2;
      if (bVar11) {
        iVar2 = iVar16;
      }
      uVar1 = 0;
      if (iVar14 != 0) {
        uVar1 = uVar17;
      }
      iVar16 = 4;
      if (iVar14 != 0) {
        iVar16 = 5;
      }
      piVar15 = (int *)0x1;
      bVar12 = DAT_00129a60 != 0;
      *(undefined8 *)param_1 = DAT_001047a0;
      param_1[2] = iVar4;
      param_1[3] = (uint)bVar11;
      if ((iVar4 != 0 && iVar13 != 0) && bVar12) {
        iVar2 = iVar16;
      }
      param_1[4] = iVar13;
      param_1[5] = uVar1;
      param_1[8] = 0;
      param_1[9] = 0;
      *(undefined8 *)(param_1 + 10) = uVar5;
      *(undefined8 *)(param_1 + 0xc) = uVar8;
      *(long *)(param_1 + 0xe) = lVar6;
      param_1[6] = 0;
      param_1[7] = iVar2;
      *(undefined8 *)(param_1 + 0x10) = uVar7;
      _DAT_00129ce8 = 0;
    }
    else {
      piVar15 = (int *)0x0;
    }
  }
  return piVar15;
}

/* ===== FUN_00112e84 @ 00112e84 [libNexusEvasion69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_00112e84(char *param_1,int *param_2)

{
  char *pcVar1;
  uint uVar2;
  uint uVar3;
  char cVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  char cVar9;
  undefined8 uVar10;
  bool bVar11;
  bool bVar12;
  uint uVar13;
  uint uVar14;
  int iVar15;
  int iVar16;
  undefined8 uVar17;
  long lVar18;
  int iVar19;
  undefined **ppuVar20;
  
  if ((((param_2 == (int *)0x0) || (*param_2 != 1)) || (param_2[1] != 0x48)) ||
     (uVar13 = FUN_00112d44(), (int)uVar13 < 0)) {
    uVar17 = 0;
  }
  else {
    do {
      cVar9 = DAT_00129ce8;
      cVar4 = '\x01';
      bVar11 = (bool)ExclusiveMonitorPass(0x129ce8,0x10);
      if (bVar11) {
        _DAT_00129ce8 = CONCAT31(DAT_00129ce8_1,1);
        cVar4 = ExclusiveMonitorsStatus();
      }
    } while (cVar4 != '\0' || cVar9 != '\0');
    FUN_0010f580();
    uVar3 = (uint)(DAT_00129c50 != 0) | (uint)(DAT_00129c58 != 0) << 1 |
            (uint)(DAT_00129c60 != 0) << 2 | (uint)(DAT_00129c68 != 0) << 3 |
            (uint)(DAT_00129c70 != 0) << 4 | (uint)(DAT_00129c78 != 0) << 5 |
            (uint)(DAT_00129c80 != 0) << 6 | (uint)(DAT_00129c88 != 0) << 7 |
            (uint)(DAT_00129c90 != 0) << 8 | (uint)(DAT_00129c98 != 0) << 9;
    uVar14 = FUN_001187d8();
    iVar15 = FUN_001189fc(uVar13,uVar14,0);
    if (*param_1 != '\0') {
      lVar18 = 1;
      do {
        pcVar1 = param_1 + lVar18;
        lVar18 = lVar18 + 1;
      } while (*pcVar1 != '\0');
    }
    iVar16 = strcmp(param_1,"isSpinEnabled");
    lVar18 = 0;
    if (iVar16 != 0) {
      ppuVar20 = &PTR_s_cameraEnabled_001216c0;
      do {
        lVar18 = lVar18 + 1;
        iVar16 = strcmp(param_1,*ppuVar20);
        ppuVar20 = ppuVar20 + 5;
      } while (iVar16 != 0);
    }
    uVar10 = DAT_00129cf8;
    uVar6 = DAT_00129348;
    uVar5 = DAT_001047a0;
    uVar17 = 1;
    uVar2 = 1 << (ulong)(uVar13 & 0x1f);
    iVar16 = (&DAT_00129130)[(int)(&DAT_001216a0)[lVar18 * 10]];
    param_2[8] = 0;
    param_2[9] = uVar3;
    uVar8 = DAT_00129cc0;
    uVar7 = DAT_00129c38;
    *(undefined8 *)param_2 = uVar5;
    *(undefined8 *)(param_2 + 10) = uVar6;
    *(undefined8 *)(param_2 + 0xc) = uVar10;
    uVar13 = 0;
    if (iVar15 != 0) {
      uVar13 = (uint)(iVar16 != 0);
    }
    iVar19 = 4;
    if (iVar15 != 0) {
      iVar19 = 5;
    }
    *(undefined8 *)(param_2 + 0xe) = uVar7;
    *(undefined8 *)(param_2 + 0x10) = uVar8;
    if (iVar16 == 0) {
      iVar19 = 1;
    }
    bVar11 = (uVar14 & uVar2) != 0;
    if (!bVar11) {
      iVar19 = 3;
    }
    bVar12 = (uVar3 & uVar2) != 0;
    if (!bVar12) {
      iVar19 = 2;
    }
    param_2[4] = (uint)bVar11;
    param_2[5] = uVar13;
    param_2[2] = iVar16;
    param_2[3] = (uint)bVar12;
    param_2[6] = 0;
    param_2[7] = iVar19;
    _DAT_00129ce8 = 0;
  }
  return uVar17;
}

/* ===== nexus_evasion_spin_snapshot_v1 @ 00115bd4 [libNexusEvasion69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int * nexus_evasion_spin_snapshot_v1(int *param_1)

{
  char cVar1;
  undefined8 uVar2;
  int iVar3;
  int iVar4;
  long lVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  char cVar9;
  undefined **ppuVar10;
  bool bVar11;
  int iVar12;
  int iVar13;
  int iVar14;
  int iVar15;
  int iVar16;
  int *piVar17;
  undefined **ppuVar18;
  
  piVar17 = param_1;
  if (param_1 != (int *)0x0) {
    if ((*param_1 == 1) && (param_1[1] == 0x58)) {
      do {
        cVar9 = DAT_00129ce8;
        cVar1 = '\x01';
        bVar11 = (bool)ExclusiveMonitorPass(0x129ce8,0x10);
        if (bVar11) {
          _DAT_00129ce8 = CONCAT31(DAT_00129ce8_1,1);
          cVar1 = ExclusiveMonitorsStatus();
        }
      } while (cVar1 != '\0' || cVar9 != '\0');
      FUN_0010f580();
      iVar12 = FUN_001158a0();
      uVar8 = DAT_00129b30;
      uVar7 = DAT_00129ac0;
      uVar6 = DAT_00129a68;
      lVar5 = DAT_00129a60;
      uVar2 = DAT_00129348;
      iVar3 = DAT_00129130;
      bVar11 = DAT_00129a60 != 0;
      ppuVar10 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar18 = ppuVar10;
        iVar13 = strcmp("spinSpeed",*ppuVar18);
        iVar4 = DAT_001293c4;
        ppuVar10 = ppuVar18 + 5;
      } while (iVar13 != 0);
      iVar13 = (&DAT_00129130)[*(int *)(ppuVar18 + 1)];
      ppuVar10 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar18 = ppuVar10;
        iVar14 = strcmp("spinMovementMode",*ppuVar18);
        ppuVar10 = ppuVar18 + 5;
      } while (iVar14 != 0);
      iVar14 = (&DAT_00129130)[*(int *)(ppuVar18 + 1)];
      ppuVar10 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar18 = ppuVar10;
        iVar15 = strcmp("spinRadius",*ppuVar18);
        ppuVar10 = ppuVar18 + 5;
      } while (iVar15 != 0);
      iVar15 = (&DAT_00129130)[*(int *)(ppuVar18 + 1)];
      ppuVar10 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar18 = ppuVar10;
        iVar16 = strcmp("spinOnlineOnlyIdle",*ppuVar18);
        ppuVar10 = ppuVar18 + 5;
      } while (iVar16 != 0);
      piVar17 = (int *)0x1;
      iVar16 = (&DAT_00129130)[*(int *)(ppuVar18 + 1)];
      param_1[0x12] = iVar14;
      param_1[0x13] = iVar15;
      *(undefined8 *)(param_1 + 2) = uVar2;
      *(undefined8 *)(param_1 + 6) = uVar6;
      *(long *)(param_1 + 4) = lVar5;
      uVar2 = DAT_001047b0;
      param_1[0x14] = iVar16;
      *(undefined8 *)param_1 = uVar2;
      *(undefined8 *)(param_1 + 8) = uVar7;
      *(undefined8 *)(param_1 + 10) = uVar8;
      param_1[0xc] = (uint)bVar11;
      param_1[0xd] = iVar12;
      param_1[0x10] = 0x168;
      param_1[0x11] = iVar4;
      param_1[0xe] = iVar3;
      param_1[0xf] = iVar13;
      _DAT_00129ce8 = 0;
    }
    else {
      piVar17 = (int *)0x0;
    }
  }
  return piVar17;
}

/* ===== FUN_00115e3c @ 00115e3c [libNexusEvasion69252.so] ===== */

void FUN_00115e3c(int param_1,uint param_2,undefined8 *param_3)

{
  long lVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  undefined **ppuVar8;
  bool bVar9;
  int iVar10;
  int iVar11;
  undefined8 uVar12;
  uint uVar13;
  undefined **ppuVar14;
  undefined8 local_80;
  undefined8 uStack_78;
  long lStack_70;
  long local_68;
  undefined8 uStack_60;
  long local_58;
  
  lVar1 = tpidr_el0;
  local_58 = *(long *)(lVar1 + 0x28);
  FUN_0010f580();
  ppuVar8 = &PTR_s_cameraEnabled_001216c0;
  do {
    ppuVar14 = ppuVar8;
    iVar10 = strcmp("spinMovementMode",*ppuVar14);
    ppuVar8 = ppuVar14 + 5;
  } while (iVar10 != 0);
  iVar10 = (&DAT_00129130)[*(int *)(ppuVar14 + 1)];
  if ((param_2 == 1) && (iVar10 == 0)) {
    bVar9 = true;
  }
  else {
    if (iVar10 != 1) {
      uVar12 = 0;
      goto LAB_00116048;
    }
    bVar9 = (param_2 & 0xfffffffe) == 2;
  }
  uVar12 = 0;
  if ((param_1 == 0x19) && (bVar9)) {
    iVar11 = FUN_0011c394(iVar10);
    uVar12 = 0;
    if ((iVar11 != 0) && ((DAT_001293c4 == 0 && (DAT_00129130 != 0)))) {
      iVar11 = FUN_001158a0(0);
      uVar12 = 0;
      if ((iVar11 != 0) && (DAT_00129a98 != (code *)0x0)) {
        uStack_60 = 0;
        local_68 = 0;
        lStack_70 = 0;
        uStack_78 = 0;
        local_80 = DAT_001047a8;
        iVar11 = (*DAT_00129a98)(DAT_00129a80,0x19,&local_80);
        uVar6 = DAT_00129ac0;
        uVar5 = DAT_00129a68;
        uVar4 = DAT_00129a60;
        uVar3 = DAT_00129348;
        uVar2 = DAT_001047c0;
        uVar12 = 0;
        if (((iVar11 == 1) && ((int)local_80 == 1)) && (local_80._4_4_ == 0x28)) {
          uVar12 = 0;
          uVar13 = 0xff;
          if (iVar10 != 1) {
            uVar13 = 0x3f;
          }
          if ((((((uVar13 & ((uint)uStack_78 ^ 0xffffffff)) == 0) && (uStack_60._4_4_ == 0)) &&
               ((lStack_70 != 0 && ((local_68 != 0 && (uStack_78._4_4_ != 0)))))) &&
              (999999 < (uint)uStack_60)) && ((uint)uStack_60 < 2000000)) {
            uVar12 = 1;
            *(undefined4 *)(param_3 + 1) = 0x19;
            *(uint *)((long)param_3 + 0xc) = param_2;
            uVar7 = DAT_00129b30;
            *param_3 = uVar2;
            param_3[4] = uVar5;
            param_3[3] = uVar4;
            param_3[2] = uVar3;
            param_3[5] = uVar6;
            param_3[6] = uVar7;
            param_3[8] = uStack_78;
            param_3[7] = local_80;
            param_3[10] = local_68;
            param_3[9] = lStack_70;
            param_3[0xb] = uStack_60;
          }
        }
      }
    }
  }
LAB_00116048:
  if (*(long *)(lVar1 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar12);
}

/* ===== FUN_001189fc @ 001189fc [libNexusEvasion69252.so] ===== */

ulong FUN_001189fc(uint param_1,uint param_2,int param_3)

{
  uint uVar1;
  undefined **ppuVar2;
  int iVar3;
  ulong uVar4;
  undefined **ppuVar5;
  
  if (9 < param_1) {
    return 0;
  }
  if ((param_2 >> (ulong)(param_1 & 0x1f) & 1) == 0) {
    return 0;
  }
  if (param_1 < 2) {
    if ((param_2 & 1) == 0) {
      return 0;
    }
    ppuVar2 = &PTR_s_cameraEnabled_001216c0;
    do {
      ppuVar5 = ppuVar2;
      iVar3 = strcmp("holdToShootEnabled",*ppuVar5);
      ppuVar2 = ppuVar5 + 5;
    } while (iVar3 != 0);
    if ((&DAT_00129130)[*(int *)(ppuVar5 + 1)] != 0) {
      if (DAT_00129b78 == 0) {
        return 0;
      }
      uVar4 = FUN_00116854();
      if ((int)uVar4 == 0) {
        return uVar4;
      }
      ppuVar2 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar5 = ppuVar2;
        iVar3 = strcmp("holdToShootAim",*ppuVar5);
        ppuVar2 = ppuVar5 + 5;
      } while (iVar3 != 0);
      uVar1 = (&DAT_00129130)[*(int *)(ppuVar5 + 1)];
      ppuVar2 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar5 = ppuVar2;
        iVar3 = strcmp("holdToShootRangeCheck",*ppuVar5);
        ppuVar2 = ppuVar5 + 5;
      } while (iVar3 != 0);
      if (1 < ((&DAT_00129130)[*(int *)(ppuVar5 + 1)] | uVar1)) {
        return 0;
      }
      if ((DAT_00129b90 >> (ulong)((uVar1 | (&DAT_00129130)[*(int *)(ppuVar5 + 1)] << 1) & 0x1f) & 1
          ) == 0) {
        return 0;
      }
    }
    ppuVar2 = &PTR_s_cameraEnabled_001216c0;
    do {
      ppuVar5 = ppuVar2;
      iVar3 = strcmp("aopPredictEnabled",*ppuVar5);
      ppuVar2 = ppuVar5 + 5;
    } while (iVar3 != 0);
    if ((param_3 != 0 && param_1 == 1) || ((&DAT_00129130)[*(int *)(ppuVar5 + 1)] != 0)) {
      ppuVar2 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar5 = ppuVar2;
        iVar3 = strcmp("killauraEnabled",*ppuVar5);
        ppuVar2 = ppuVar5 + 5;
      } while (iVar3 != 0);
      if ((&DAT_00129130)[*(int *)(ppuVar5 + 1)] == 0) {
        if ((param_2 >> 1 & 1) == 0) {
          return 0;
        }
      }
      else {
        iVar3 = FUN_00118eec("aopPredictVersion");
        if ((param_2 >> 1 & 1) == 0) {
          return 0;
        }
        if (iVar3 - 3U < 0xfffffffe) {
          return 0;
        }
      }
      iVar3 = FUN_00118eec("aopPredictVersion");
      if (iVar3 - 3U < 0xfffffffe) {
        return 0;
      }
    }
    ppuVar2 = &PTR_s_cameraEnabled_001216c0;
    do {
      ppuVar5 = ppuVar2;
      iVar3 = strcmp("aopAimTargetMode",*ppuVar5);
      ppuVar2 = ppuVar5 + 5;
    } while (iVar3 != 0);
    if ((&DAT_00129130)[*(int *)(ppuVar5 + 1)] == 1) {
      return 1;
    }
    iVar3 = FUN_00118eec("aopAimTargetMode");
    return (ulong)(iVar3 == 2);
  }
  switch(param_1) {
  case 4:
    goto switchD_00118a74_caseD_4;
  case 5:
    if (DAT_00129478 == 0) {
      return 0;
    }
    if (DAT_00129490 == (code *)0x0) {
      return 0;
    }
    iVar3 = (*DAT_00129490)(0);
    if (iVar3 != 1) {
      return 0;
    }
    if (DAT_001293d0 != DAT_00129478) {
      return 0;
    }
    if (DAT_001293a8 != DAT_00129480) {
      return 0;
    }
    iVar3 = FUN_0011b320("coltModEnabled");
    break;
  case 6:
    if (DAT_00129478 == 0) {
      return 0;
    }
    if (DAT_00129490 == (code *)0x0) {
      return 0;
    }
    iVar3 = (*DAT_00129490)(0);
    if (iVar3 != 1) {
      return 0;
    }
    if (DAT_001293d0 != DAT_00129478) {
      return 0;
    }
    if (DAT_001293a8 != DAT_00129480) {
      return 0;
    }
    iVar3 = FUN_0011b320("autofarmEnabled");
    break;
  case 7:
    if (DAT_00129478 == 0) {
      return 0;
    }
    if (DAT_00129490 == (code *)0x0) {
      return 0;
    }
    iVar3 = (*DAT_00129490)(0);
    if (iVar3 != 1) {
      return 0;
    }
    if (DAT_001293d0 != DAT_00129478) {
      return 0;
    }
    if (DAT_001293a8 != DAT_00129480) {
      return 0;
    }
    iVar3 = FUN_0011b320("boltModEnabled");
    break;
  case 8:
    if (DAT_00129478 == 0) {
      return 0;
    }
    if (DAT_00129490 == (code *)0x0) {
      return 0;
    }
    iVar3 = (*DAT_00129490)(0);
    if (iVar3 != 1) {
      return 0;
    }
    if (DAT_001293d0 != DAT_00129478) {
      return 0;
    }
    if (DAT_001293a8 != DAT_00129480) {
      return 0;
    }
    iVar3 = FUN_0011b320("kitNaniModEnabled");
    break;
  case 9:
    if (DAT_00129478 == 0) {
      return 0;
    }
    if (DAT_00129490 == (code *)0x0) {
      return 0;
    }
    iVar3 = (*DAT_00129490)(0);
    if (iVar3 != 1) {
      return 0;
    }
    if (DAT_001293d0 != DAT_00129478) {
      return 0;
    }
    if (DAT_001293a8 != DAT_00129480) {
      return 0;
    }
    goto switchD_00118a74_caseD_4;
  default:
    if ((param_2 >> 2 & 1) == 0) {
      return 0;
    }
    iVar3 = FUN_00118eec("dodgeVersion");
    if (4 < iVar3 - 1U) {
      return 0;
    }
    iVar3 = FUN_00118eec("isSpinEnabled");
    if (iVar3 == 0) {
      return 1;
    }
    iVar3 = FUN_00118eec("spinMovementMode");
    if (iVar3 == 0) {
      return 1;
    }
    iVar3 = FUN_00118eec("spinOnlineOnlyIdle");
    return (ulong)(iVar3 != 0);
  }
  if (iVar3 != 0) {
    return 1;
  }
switchD_00118a74_caseD_4:
  iVar3 = pthread_once((pthread_once_t *)&DAT_00129cd8,FUN_0011b480);
  if (DAT_00129ce0 == (code *)0x0) {
    return 0;
  }
  iVar3 = (*DAT_00129ce0)(iVar3);
  return (ulong)(iVar3 == 1);
}

/* ===== FUN_00118eec @ 00118eec [libNexusEvasion69252.so] ===== */

undefined4 FUN_00118eec(char *param_1)

{
  char *pcVar1;
  int iVar2;
  long lVar3;
  undefined **ppuVar4;
  
  if (*param_1 != '\0') {
    lVar3 = 1;
    do {
      pcVar1 = param_1 + lVar3;
      lVar3 = lVar3 + 1;
    } while (*pcVar1 != '\0');
  }
  iVar2 = strcmp(param_1,"isSpinEnabled");
  lVar3 = 0;
  if (iVar2 != 0) {
    ppuVar4 = &PTR_s_cameraEnabled_001216c0;
    do {
      lVar3 = lVar3 + 1;
      iVar2 = strcmp(param_1,*ppuVar4);
      ppuVar4 = ppuVar4 + 5;
    } while (iVar2 != 0);
  }
  return (&DAT_00129130)[(int)(&DAT_001216a0)[lVar3 * 10]];
}

/* ===== FUN_0011c828 @ 0011c828 [libNexusEvasion69252.so] ===== */

void FUN_0011c828(undefined8 param_1,long param_2,long param_3)

{
  long lVar1;
  int iVar2;
  long lVar3;
  int iVar4;
  ulong uVar5;
  int *piVar6;
  int *piVar7;
  int local_5b0 [118];
  long local_3d8 [118];
  long local_28;
  
  lVar1 = tpidr_el0;
  iVar4 = 4;
  local_28 = *(long *)(lVar1 + 0x28);
  if ((param_2 != 0) && (param_3 == 0x76)) {
    uVar5 = 0;
    lVar3 = 0;
    piVar6 = (int *)(param_2 + 4);
    piVar7 = &DAT_00124248;
    do {
      if (((uVar5 != (uint)piVar6[-1]) || (iVar4 = *piVar6, iVar4 < piVar7[-1])) ||
         (*piVar7 < iVar4)) {
        iVar4 = 4;
        goto LAB_0011c8f0;
      }
      if (*(long *)(piVar7 + -4) != 0) {
        local_3d8[lVar3] = *(long *)(piVar7 + -4);
        local_5b0[lVar3] = iVar4;
        lVar3 = lVar3 + 1;
      }
      uVar5 = uVar5 + 1;
      piVar7 = piVar7 + 6;
      piVar6 = piVar6 + 2;
    } while (uVar5 != 0x76);
    iVar2 = nexus_evasion_restore_v1(local_3d8,local_5b0);
    iVar4 = iVar2;
    if (iVar2 != 2) {
      iVar4 = 4;
    }
    if (iVar2 == 1) {
      iVar4 = 1;
    }
  }
LAB_0011c8f0:
  if (*(long *)(lVar1 + 0x28) != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar4);
  }
  return;
}

/* ===== nexus_evasion_get_requested @ 0011d520 [libNexusEvasion69252.so] ===== */

void nexus_evasion_get_requested(void)

{
  (*(code *)PTR_nexus_evasion_get_requested_00124f50)();
  return;
}

/* ===== nexus_evasion_get_effective @ 0011d530 [libNexusEvasion69252.so] ===== */

void nexus_evasion_get_effective(void)

{
  (*(code *)PTR_nexus_evasion_get_effective_00124f58)();
  return;
}

/* ===== nexus_evasion_get_port_state @ 0011d540 [libNexusEvasion69252.so] ===== */

void nexus_evasion_get_port_state(void)

{
  (*(code *)PTR_nexus_evasion_get_port_state_00124f60)();
  return;
}

/* ===== FUN_0014382c @ 0014382c [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0014382c(void)

{
  uint uVar1;
  long lVar2;
  undefined8 uVar3;
  int iVar4;
  char *pcVar5;
  long lVar6;
  void *pvVar7;
  char *pcVar8;
  char *pcVar9;
  uint uVar10;
  undefined8 local_390;
  long local_388;
  undefined8 uStack_380;
  code *local_378;
  code *pcStack_370;
  code *local_368;
  code *pcStack_360;
  code *local_358;
  char *local_350;
  long local_348;
  undefined1 auStack_330 [344];
  undefined1 auStack_1d8 [344];
  long local_80;
  
  lVar2 = tpidr_el0;
  local_80 = *(long *)(lVar2 + 0x28);
  DAT_00215d78 = dlsym(DAT_0020d0f0,"nexus_evasion_spin_register_v1");
  DAT_00215d80 = dlsym(DAT_0020d0f0,"nexus_evasion_spin_snapshot_v1");
  DAT_00215d88 = dlsym(DAT_0020d0f0,"nexus_evasion_spin_lease_v1");
  DAT_00215d90 = dlsym(DAT_0020d0f0,"nexus_evasion_spin_recheck_v1");
  DAT_00215d98 = dlsym(DAT_0020d0f0,"nexus_evasion_publish_spin_post_v1");
  DAT_00215da0 = dlsym(DAT_0020d0f0,"nexus_evasion_spin_validate_post_v1");
  DAT_00215da8 = dlsym(DAT_0020d0f0,"nexus_evasion_publish_spin_movement_post_v1");
  DAT_00215db0 = dlsym(DAT_0020d0f0,"nexus_evasion_spin_validate_movement_post_v1");
  if (((((((DAT_00215d78 == 0) || (iVar4 = dladdr(DAT_00215d78,&local_350), iVar4 == 0)) ||
         (local_348 != DAT_0020c0e8)) ||
        ((local_350 == (char *)0x0 || (iVar4 = strcmp(local_350,&DAT_0020c0f0), iVar4 != 0)))) ||
       ((DAT_00215d80 == 0 ||
        ((iVar4 = dladdr(DAT_00215d80,&local_350), iVar4 == 0 || (local_348 != DAT_0020c0e8)))))) ||
      ((local_350 == (char *)0x0 ||
       ((((((iVar4 = strcmp(local_350,&DAT_0020c0f0), iVar4 != 0 || (DAT_00215d88 == 0)) ||
           (iVar4 = dladdr(DAT_00215d88,&local_350), iVar4 == 0)) ||
          (((local_348 != DAT_0020c0e8 || (local_350 == (char *)0x0)) ||
           ((iVar4 = strcmp(local_350,&DAT_0020c0f0), iVar4 != 0 ||
            ((DAT_00215d90 == 0 || (iVar4 = dladdr(DAT_00215d90,&local_350), iVar4 == 0)))))))) ||
         (local_348 != DAT_0020c0e8)) ||
        (((local_350 == (char *)0x0 || (iVar4 = strcmp(local_350,&DAT_0020c0f0), iVar4 != 0)) ||
         (DAT_00215d98 == 0)))))))) ||
     ((((iVar4 = dladdr(DAT_00215d98,&local_350), iVar4 == 0 || (local_348 != DAT_0020c0e8)) ||
       (((local_350 == (char *)0x0 ||
         ((iVar4 = strcmp(local_350,&DAT_0020c0f0), iVar4 != 0 || (DAT_00215da0 == 0)))) ||
        (iVar4 = dladdr(DAT_00215da0,&local_350), iVar4 == 0)))) ||
      (((((((local_348 != DAT_0020c0e8 || (local_350 == (char *)0x0)) ||
           (iVar4 = strcmp(local_350,&DAT_0020c0f0), iVar4 != 0)) ||
          ((DAT_00215da8 == 0 || (iVar4 = dladdr(DAT_00215da8,&local_350), iVar4 == 0)))) ||
         (local_348 != DAT_0020c0e8)) ||
        (((local_350 == (char *)0x0 || (iVar4 = strcmp(local_350,&DAT_0020c0f0), iVar4 != 0)) ||
         ((DAT_00215db0 == 0 ||
          (((iVar4 = dladdr(DAT_00215db0,&local_350), iVar4 == 0 || (local_348 != DAT_0020c0e8)) ||
           (local_350 == (char *)0x0)))))))) ||
       (iVar4 = strcmp(local_350,&DAT_0020c0f0), iVar4 != 0)))))) {
    pcVar8 = "exports";
  }
  else {
    DAT_00215db8 = 1;
    iVar4 = FUN_0018d938(&DAT_00215dc0,DAT_001e0978,&PTR_DAT_001c3220,FUN_001428fc,0);
    if (iVar4 == 0) {
      pcVar8 = "guest_game_identity";
    }
    else {
      lVar6 = FUN_001bbbac();
      if (lVar6 - 0x1001U < 0xfffffffffffff000) {
        pcVar8 = "provider_size";
      }
      else {
        local_388 = DAT_001e0978;
        uStack_380 = 0;
        local_378 = FUN_001428fc;
        pcStack_370 = FUN_001592e8;
        local_368 = FUN_00159434;
        pcStack_360 = FUN_001596ac;
        local_390 = DAT_0010e7f8;
        DAT_00215dd8 = 1;
        local_358 = FUN_00159730;
        iVar4 = FUN_001bbbb4(&DAT_00215de0,0x1000,&local_390);
        DAT_00215dd8 = 0;
        if (iVar4 == 1) {
          DAT_00216de0 = 1;
          DAT_00216de8 = DAT_001e0978;
          uRam0000000000216df8 = _UNK_001c3278;
          _DAT_00216df0 = _DAT_001c3270;
          puRam0000000000216e08 = PTR_FUN_001c3288;
          _DAT_00216e00 = PTR_FUN_001c3280;
          DAT_00216e10 = PTR_FUN_001c3290;
          iVar4 = FUN_00158080();
          if ((iVar4 == 0) ||
             (iVar4 = FUN_001a16a0(&DAT_00216de8,&DAT_00216e18), uVar3 = DAT_0010e540, iVar4 != 1))
          {
            pcVar8 = "online_movement_native_guards";
          }
          else {
            DAT_00216e60 = 1;
            DAT_00216e90 = DAT_001e0978 + 0xb30690;
            DAT_00216e98 = (void *)0x0;
            DAT_00216e80 = FUN_001455a4;
            DAT_00216e88 = FUN_00159ba0;
            DAT_00216ea0 = DAT_001e0978 + 0xb30694;
            DAT_00216e70 = DAT_00209d00;
            DAT_00216e78 = DAT_001e0978;
            DAT_00216e68 = DAT_0010e540;
            _DAT_00216ea8 = DAT_0010e5b8;
            pvVar7 = (void *)FUN_00152af0();
            DAT_00216e98 = pvVar7;
            if (((pvVar7 != (void *)0x0) && ((((uint)DAT_00216e90 | (uint)pvVar7) & 3) == 0)) &&
               (0xfffffffff0000002 < ((long)pvVar7 - DAT_00216e90) - 0x7fffffdU)) {
              uVar10 = (uint)((long)pvVar7 - DAT_00216e90);
              uVar1 = uVar10 + 3;
              if (-1 < (int)uVar10) {
                uVar1 = uVar10;
              }
              _DAT_00216ea8 =
                   CONCAT44(uVar1 >> 2,DAT_00216ea8) & 0x3ffffffffffffff | 0x1400000000000000;
              iVar4 = FUN_0015a024(auStack_1d8);
              if (iVar4 != 0) {
                memcpy(pvVar7,auStack_1d8,0x158);
                FUN_001bdaf0(pvVar7,(long)pvVar7 + 0x158);
                iVar4 = mprotect(DAT_00216e98,DAT_00209d88,5);
                if (iVar4 == 0) {
                  iVar4 = FUN_001428fc(0,DAT_00216e98,auStack_330,0x158);
                  if (iVar4 != 0) {
                    iVar4 = memcmp(auStack_1d8,auStack_330,0x158);
                    if ((iVar4 == 0) && (iVar4 = FUN_0015a11c(), iVar4 != 0)) {
                      DAT_00216eb8 = DAT_00209d00;
                      DAT_00216ec0 = DAT_001e0978;
                      DAT_00216ed8 = DAT_001e0978 + 0xe7b768;
                      DAT_00216eb0 = uVar3;
                      DAT_00216ee8 = DAT_001e0978 + 0xe7b76c;
                      DAT_00216ec8 = FUN_001455a4;
                      DAT_00216ed0 = FUN_0015a3f0;
                      DAT_00216ee0 = (void *)0x0;
                      _DAT_00216ef0 = DAT_0010e6b8;
                      pvVar7 = (void *)FUN_00152af0();
                      DAT_00216ee0 = pvVar7;
                      if ((pvVar7 != (void *)0x0) &&
                         (((((uint)DAT_00216ed8 | (uint)pvVar7) & 3) == 0 &&
                          (0xfffffffff0000002 < ((long)pvVar7 - DAT_00216ed8) - 0x7fffffdU)))) {
                        uVar10 = (uint)((long)pvVar7 - DAT_00216ed8);
                        uVar1 = uVar10 + 3;
                        if (-1 < (int)uVar10) {
                          uVar1 = uVar10;
                        }
                        _DAT_00216ef0 =
                             CONCAT44(uVar1 >> 2,DAT_00216ef0) & 0x3ffffffffffffff |
                             0x1400000000000000;
                        iVar4 = FUN_0015a744(auStack_1d8);
                        if (iVar4 != 0) {
                          memcpy(pvVar7,auStack_1d8,0x158);
                          FUN_001bdaf0(pvVar7,(long)pvVar7 + 0x158);
                          iVar4 = mprotect(DAT_00216ee0,DAT_00209d88,5);
                          if (iVar4 == 0) {
                            iVar4 = FUN_001428fc(0,DAT_00216ee0,auStack_330,0x158);
                            if (iVar4 != 0) {
                              iVar4 = memcmp(auStack_1d8,auStack_330,0x158);
                              if ((iVar4 == 0) && (iVar4 = FUN_0015a83c(), iVar4 != 0)) {
                                iVar4 = FUN_0015aa90(DAT_00216e90,_DAT_00216ea8 & 0xffffffff,
                                                     DAT_00216eac);
                                if (iVar4 < 0) {
                                  FUN_001417c8("fatal","spin_post_publication_restore_unverified",0)
                                  ;
                    /* WARNING: Subroutine does not return */
                                  abort();
                                }
                                if (iVar4 == 1) {
                                  DAT_00216ef8 = 1;
                                  iVar4 = FUN_0015aa90(DAT_00216ed8,_DAT_00216ef0 & 0xffffffff,
                                                       DAT_00216ef4);
                                  if (iVar4 < 0) {
                                    FUN_001417c8("fatal",
                                                 "spin_movement_post_publication_restore_unverified"
                                                 ,0);
                    /* WARNING: Subroutine does not return */
                                    abort();
                                  }
                                  if (iVar4 == 1) {
                                    DAT_00216efc = 1;
                                    iVar4 = FUN_0015a11c();
                                    if ((iVar4 != 0) && (iVar4 = FUN_0015a83c(1), iVar4 != 0)) {
                                      pcVar5 = "spin_post_installed";
                                      pcVar8 = 
                                      "owned_final_and_movement_epilogues_no_capability_yet";
                                      pcVar9 = 
                                      ",\"action\":25,\"frame_entry_rva\":\"0xb30690\",\"movement_entry_rva\":\"0xe7b768\",\"movement_function_rva\":\"0xe7af70\",\"original_call_added\":false,\"online_movement_bound\":true"
                                      ;
                                      goto LAB_001439a0;
                                    }
                                  }
                                  FUN_0015abf4();
                                  pcVar8 = "movement_post_publication";
                                }
                                else {
                                  pcVar8 = "post_publication";
                                }
                                goto LAB_0014398c;
                              }
                            }
                            pcVar8 = "movement_post_prepublication_readback";
                            goto LAB_0014398c;
                          }
                        }
                      }
                      pcVar8 = "movement_post_island";
                      goto LAB_0014398c;
                    }
                  }
                  pcVar8 = "post_prepublication_readback";
                  goto LAB_0014398c;
                }
              }
            }
            pcVar8 = "post_island";
          }
        }
        else {
          pcVar8 = "provider_native_guards";
        }
      }
    }
  }
LAB_0014398c:
  DAT_00215dd8 = 0;
  pcVar5 = "spin_unavailable";
  pcVar9 = ",\"action\":25,\"other_features_unchanged\":true";
LAB_001439a0:
  FUN_001417c8(pcVar5,pcVar8,pcVar9);
  if (*(long *)(lVar2 + 0x28) == local_80) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00146764 @ 00146764 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00146764(long *param_1,undefined8 param_2,int param_3)

{
  int iVar1;
  long lVar2;
  bool bVar3;
  char cVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  void *pvVar13;
  undefined8 uVar14;
  long lVar15;
  long lVar16;
  long lVar17;
  undefined8 uVar18;
  char *pcVar19;
  char *pcVar20;
  char *pcVar21;
  int local_12d0;
  long local_1298;
  long local_1290;
  long local_1288;
  long local_1270;
  long local_1268;
  long local_1260;
  int local_1258;
  undefined1 auStack_1120 [4096];
  undefined1 auStack_120 [32];
  undefined8 local_100;
  undefined8 uStack_f8;
  undefined8 uStack_f0;
  undefined8 uStack_e8;
  undefined8 local_e0;
  undefined8 uStack_d8;
  undefined8 uStack_d0;
  undefined8 uStack_c8;
  undefined1 local_c0;
  undefined8 local_bc;
  undefined8 uStack_b4;
  undefined8 local_ac;
  undefined8 uStack_a4;
  undefined8 local_9c;
  undefined8 uStack_94;
  undefined8 local_8c;
  undefined8 uStack_84;
  long local_78;
  
  lVar2 = tpidr_el0;
  local_78 = *(long *)(lVar2 + 0x28);
  DAT_00216f08 = 0;
  DAT_00214c18 = 0;
  uRam0000000000228de8 = 0;
  _DAT_00228de0 = 0;
  uRam0000000000228df8 = 0;
  _DAT_00228df0 = 0;
  uRam0000000000228e08 = 0;
  _DAT_00228e00 = 0;
  uRam0000000000228e18 = 0;
  _DAT_00228e10 = 0;
  if ((((param_1 != (long *)0x0) && (param_3 != 0)) && (DAT_00209cd4 != 0)) &&
     ((-1 < DAT_001cfb50 && (DAT_00209d00 != 0)))) {
    iVar5 = (**(code **)(*param_1 + 0x98))(param_1,0x80);
    if (iVar5 < 0) {
      if (*(long *)(lVar2 + 0x28) == local_78) {
                    /* WARNING: Could not recover jumptable at 0x001468fc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
        (**(code **)(*param_1 + 0x88))(param_1);
        return;
      }
      goto LAB_00146e38;
    }
    lVar8 = (**(code **)(*param_1 + 0x480))(param_1,param_2,&DAT_00117e9e,"Lnexus/loader/d;");
    pcVar20 = "core";
    if (((lVar8 == 0) ||
        (lVar9 = (**(code **)(*param_1 + 0x488))(param_1,param_2,lVar8), lVar9 == 0)) ||
       ((cVar4 = (**(code **)(*param_1 + 0x720))(param_1), pcVar20 = "core", cVar4 != '\0' ||
        (lVar10 = (**(code **)(*param_1 + 0xf8))(param_1,lVar9), lVar10 == 0)))) {
LAB_00146868:
      bVar3 = true;
    }
    else {
      lVar11 = (**(code **)(*param_1 + 0x2f0))(param_1,lVar10,&DAT_0011fd4c,"Lnexus/loader/e;");
      if (lVar11 == 0) {
        lVar11 = 0;
      }
      else {
        lVar11 = (**(code **)(*param_1 + 0x2f8))(param_1,lVar9,lVar11);
      }
      (**(code **)(*param_1 + 0xb8))(param_1,lVar10);
      if ((lVar11 == 0) || (cVar4 = (**(code **)(*param_1 + 0x720))(param_1), cVar4 != '\0')) {
        bVar3 = true;
        pcVar20 = "core";
      }
      else {
        cVar4 = (**(code **)(*param_1 + 0x720))(param_1);
        if ((cVar4 == '\0') && (lVar10 = (**(code **)(*param_1 + 0xf8))(param_1,lVar9), lVar10 != 0)
           ) {
          lVar12 = (**(code **)(*param_1 + 0x2f0))
                             (param_1,lVar10,&DAT_0011d6aa,"Ljava/lang/String;");
          if (lVar12 == 0) {
            lVar12 = 0;
          }
          else {
            lVar12 = (**(code **)(*param_1 + 0x2f8))(param_1,lVar9,lVar12);
          }
          (**(code **)(*param_1 + 0xb8))(param_1,lVar10);
          if ((((lVar12 != 0) && (cVar4 = (**(code **)(*param_1 + 0x720))(param_1), cVar4 == '\0'))
              && (uVar6 = (**(code **)(*param_1 + 0x540))(param_1,lVar12), -1 < (int)uVar6)) &&
             (((uVar6 < 0xa0 && (cVar4 = (**(code **)(*param_1 + 0x720))(param_1), cVar4 == '\0'))
              && (pvVar13 = (void *)(**(code **)(*param_1 + 0x548))(param_1,lVar12,0),
                 pvVar13 != (void *)0x0)))) {
            memcpy(&local_1260,pvVar13,(ulong)uVar6);
            lVar10 = *param_1;
            *(undefined1 *)((long)&local_1260 + (ulong)uVar6) = 0;
            (**(code **)(lVar10 + 0x550))(param_1,lVar12,pvVar13);
            if (((local_1260 == 0x676e696b6e696c) &&
                (cVar4 = (**(code **)(*param_1 + 0x720))(param_1), cVar4 == '\0')) &&
               (lVar10 = (**(code **)(*param_1 + 0xf8))(param_1,lVar9), lVar10 != 0)) {
              lVar12 = (**(code **)(*param_1 + 0x2f0))
                                 (param_1,lVar10,&DAT_001147f8,"Ljava/lang/String;");
              if (lVar12 == 0) {
                lVar12 = 0;
              }
              else {
                lVar12 = (**(code **)(*param_1 + 0x2f8))(param_1,lVar9,lVar12);
              }
              (**(code **)(*param_1 + 0xb8))(param_1,lVar10);
              if (((((lVar12 != 0) &&
                    (cVar4 = (**(code **)(*param_1 + 0x720))(param_1), cVar4 == '\0')) &&
                   (uVar6 = (**(code **)(*param_1 + 0x540))(param_1,lVar12), -1 < (int)uVar6)) &&
                  ((uVar6 < 0xa0 &&
                   (cVar4 = (**(code **)(*param_1 + 0x720))(param_1), cVar4 == '\0')))) &&
                 (pvVar13 = (void *)(**(code **)(*param_1 + 0x548))(param_1,lVar12,0),
                 pvVar13 != (void *)0x0)) {
                memcpy(&local_1260,pvVar13,(ulong)uVar6);
                lVar10 = *param_1;
                *(undefined1 *)((long)&local_1260 + (ulong)uVar6) = 0;
                (**(code **)(lVar10 + 0x550))(param_1,lVar12,pvVar13);
                if (((local_1260 == 0x476e6f6973617645 && local_1258 == 0x656d61) &&
                    (cVar4 = (**(code **)(*param_1 + 0x720))(param_1), cVar4 == '\0')) &&
                   (lVar10 = (**(code **)(*param_1 + 0xf8))(param_1,lVar9), lVar10 != 0)) {
                  lVar12 = (**(code **)(*param_1 + 0x2f0))
                                     (param_1,lVar10,&DAT_00119e58,&DAT_0011896b);
                  if (lVar12 == 0) {
                    (**(code **)(*param_1 + 0xb8))(param_1,lVar10);
                  }
                  else {
                    cVar4 = (**(code **)(*param_1 + 0x300))(param_1,lVar9,lVar12);
                    (**(code **)(*param_1 + 0xb8))(param_1,lVar10);
                    if (((cVar4 == '\x01') &&
                        (cVar4 = (**(code **)(*param_1 + 0x720))(param_1), cVar4 == '\0')) &&
                       (iVar5 = FUN_0016a4cc(param_1,lVar9,&DAT_00120926), iVar5 != 0)) {
                      local_1268 = 0;
                      uVar14 = FUN_0016a2d4(param_1,lVar11,"profileSha","Ljava/lang/String;");
                      iVar5 = FUN_0016a5b8(param_1,uVar14,&local_bc,0x41);
                      if ((iVar5 == 0) ||
                         (iVar5 = memcmp(&local_bc,
                                         "114ba105835bd4157cfd80aad9498734d51108ee193601410abeb5e0bab3a71c"
                                         ,0x41), iVar5 != 0)) {
                        bVar3 = true;
                        pcVar20 = "selected_profile";
                      }
                      else {
                        iVar5 = FUN_0016a6a8(param_1,lVar11,"revision",&local_1268);
                        lVar10 = local_1268;
                        bVar3 = true;
                        pcVar20 = "selected_profile";
                        if (((iVar5 != 0) && (0 < local_1268)) &&
                           (iVar5 = FUN_0016a4cc(param_1,lVar11,"localAllowed"), iVar5 != 0)) {
                          uVar14 = FUN_0016a2d4(param_1,lVar11,"transport","Ljava/lang/String;");
                          iVar5 = FUN_0016a3a4(param_1,uVar14,"https_release");
                          if (iVar5 != 0) {
                            uVar14 = FUN_0016a2d4(param_1,lVar11,"gameSha","Ljava/lang/String;");
                            iVar5 = FUN_0016a3a4(param_1,uVar14,
                                                 "a10aeb6b4085fb2a15d969a130cd9608231998b5269a40aed41129d99624ede3"
                                                );
                            if (iVar5 != 0) {
                              uVar14 = FUN_0016a2d4(param_1,lVar9,&DAT_00120939,"Ljava/util/Map;");
                              lVar12 = FUN_0016a79c(param_1,uVar14);
                              if ((((((lVar12 == 0) ||
                                     (cVar4 = (**(code **)(*param_1 + 0x720))(param_1),
                                     cVar4 != '\0')) ||
                                    (lVar15 = (**(code **)(*param_1 + 0xf8))(param_1,lVar12),
                                    lVar15 == 0)) ||
                                   ((lVar15 = (**(code **)(*param_1 + 0x108))
                                                        (param_1,lVar15,"booleanValue",&DAT_001198f1
                                                        ), lVar15 == 0 ||
                                    (cVar4 = (**(code **)(*param_1 + 0x128))(param_1,lVar12,lVar15),
                                    cVar4 != '\x01')))) ||
                                  (cVar4 = (**(code **)(*param_1 + 0x720))(param_1), cVar4 != '\0'))
                                 || ((lVar12 = FUN_0016a2d4(param_1,lVar11,"modules",
                                                            "Ljava/util/List;"), lVar12 == 0 ||
                                     (lVar15 = (**(code **)(*param_1 + 0xf8))(param_1,lVar12),
                                     lVar15 == 0)))) {
                                pcVar20 = "callback_record";
                              }
                              else {
                                lVar16 = (**(code **)(*param_1 + 0x108))
                                                   (param_1,lVar15,&DAT_00114e24,&DAT_00119177);
                                lVar15 = (**(code **)(*param_1 + 0x108))
                                                   (param_1,lVar15,&DAT_001141a6,
                                                    "(I)Ljava/lang/Object;");
                                pcVar20 = "callback_record";
                                if (((lVar16 != 0) && (lVar15 != 0)) &&
                                   (cVar4 = (**(code **)(*param_1 + 0x720))(param_1), cVar4 == '\0')
                                   ) {
                                  iVar5 = (**(code **)(*param_1 + 0x188))(param_1,lVar12,lVar16);
                                  if ((iVar5 - 0x11U < 0xfffffff0) ||
                                     (cVar4 = (**(code **)(*param_1 + 0x720))(param_1),
                                     cVar4 != '\0')) {
                                    pcVar20 = "selected_module";
                                  }
                                  else {
                                    pcVar21 = "selected_module";
                                    local_12d0 = 0;
                                    iVar1 = 0;
                                    local_c0 = 0;
                                    uStack_f8 = 0;
                                    local_100 = 0;
                                    uStack_e8 = 0;
                                    uStack_f0 = 0;
                                    uStack_d8 = 0;
                                    local_e0 = 0;
                                    uStack_c8 = 0;
                                    uStack_d0 = 0;
                                    local_1270 = 0;
                                    do {
                                      lVar16 = (**(code **)(*param_1 + 0x110))
                                                         (param_1,lVar12,lVar15,iVar1);
                                      pcVar20 = "selected_module";
                                      if ((lVar16 == 0) ||
                                         (cVar4 = (**(code **)(*param_1 + 0x720))(param_1),
                                         pcVar20 = pcVar21, cVar4 != '\0')) goto LAB_00146868;
                                      lVar17 = FUN_0016a2d4(param_1,lVar16,&DAT_001141aa,
                                                            "Ljava/lang/String;");
                                      iVar7 = FUN_0016a3a4(param_1,lVar17,"EvasionGame");
                                      if (iVar7 != 0) {
                                        iVar7 = FUN_0016a4cc(param_1,lVar16,"callback");
                                        if (iVar7 == 0) goto LAB_00146868;
                                        uVar14 = FUN_0016a2d4(param_1,lVar16,&DAT_0011aae0,
                                                              "Ljava/lang/String;");
                                        iVar7 = FUN_0016a3a4(param_1,uVar14,"jni-onload-v1");
                                        if (iVar7 == 0) goto LAB_00146868;
                                        uVar14 = FUN_0016a2d4(param_1,lVar16,&DAT_00119e5a,
                                                              "Ljava/lang/String;");
                                        iVar7 = FUN_0016a5b8(param_1,uVar14,&local_100,0x41);
                                        if ((iVar7 == 0) ||
                                           (iVar7 = FUN_0016a6a8(param_1,lVar16,"bytes",&local_1270)
                                           , iVar7 == 0)) goto LAB_00146868;
                                        local_12d0 = local_12d0 + 1;
                                      }
                                      if (lVar17 != 0) {
                                        (**(code **)(*param_1 + 0xb8))(param_1,lVar17);
                                      }
                                      (**(code **)(*param_1 + 0xb8))(param_1,lVar16);
                                      lVar16 = local_1270;
                                      iVar1 = iVar1 + 1;
                                    } while (iVar5 != iVar1);
                                    bVar3 = true;
                                    pcVar20 = "selected_module";
                                    if ((((local_12d0 == 1) && (0x3f < local_1270)) &&
                                        (local_1270 < 0x100001)) &&
                                       (cVar4 = (**(code **)(*param_1 + 0x720))(param_1),
                                       cVar4 == '\0')) {
                                      uVar14 = FUN_0016a2d4(param_1,lVar9,&DAT_0011df95,
                                                            "Ljava/util/Map;");
                                      uVar14 = FUN_0016a79c(param_1,uVar14);
                                      iVar5 = FUN_0016a5b8(param_1,uVar14,auStack_1120,0x1000);
                                      if ((iVar5 == 0) ||
                                         (iVar5 = FUN_0016a8c0(&local_100,auStack_120), iVar5 == 0))
                                      {
                                        pcVar20 = "selected_self_path_sha";
                                      }
                                      else {
                                        iVar5 = dladdr(FUN_00159ba0,&local_1290);
                                        pcVar20 = "selected_self_path_sha";
                                        if (((iVar5 != 0) && (local_1288 != 0)) &&
                                           ((local_1290 != 0 &&
                                            ((iVar5 = FUN_0016a97c(auStack_1120), iVar5 != 0 &&
                                             (iVar5 = FUN_00142158(auStack_1120,lVar16,auStack_120),
                                             iVar5 != 0)))))) {
                                          local_1298 = 0;
                                          uVar14 = FUN_0016a2d4(param_1,lVar9,&DAT_0011fd4c,
                                                                "Lnexus/loader/e;");
                                          uVar18 = (**(code **)(*param_1 + 0x488))
                                                             (param_1,param_2,lVar8);
                                          cVar4 = (**(code **)(*param_1 + 0xc0))
                                                            (param_1,lVar9,uVar18);
                                          if ((cVar4 == '\0') ||
                                             (cVar4 = (**(code **)(*param_1 + 0xc0))
                                                                (param_1,lVar11,uVar14),
                                             cVar4 == '\0')) {
                                            pcVar20 = "selected_plan_changed";
                                          }
                                          else {
                                            iVar5 = FUN_0016a6a8(param_1,uVar14,"revision",
                                                                 &local_1298);
                                            pcVar20 = "selected_plan_changed";
                                            if ((iVar5 != 0) && (local_1298 == lVar10)) {
                                              uVar14 = FUN_0016a2d4(param_1,lVar9,&DAT_0011d6aa,
                                                                    "Ljava/lang/String;");
                                              iVar5 = FUN_0016a3a4(param_1,uVar14,"linking");
                                              if (iVar5 != 0) {
                                                uVar14 = FUN_0016a2d4(param_1,lVar9,&DAT_001147f8,
                                                                      "Ljava/lang/String;");
                                                iVar5 = FUN_0016a3a4(param_1,uVar14,"EvasionGame");
                                                if ((iVar5 != 0) &&
                                                   (cVar4 = (**(code **)(*param_1 + 0x720))(param_1)
                                                   , cVar4 == '\0')) {
                                                  DAT_00214c18 = lVar10;
                                                  DAT_00216f08 = 1;
                                                  bVar3 = false;
                                                  uRam0000000000228de8 = uStack_b4;
                                                  _DAT_00228de0 = local_bc;
                                                  uRam0000000000228df8 = uStack_a4;
                                                  _DAT_00228df0 = local_ac;
                                                  uRam0000000000228e08 = uStack_94;
                                                  _DAT_00228e00 = local_9c;
                                                  uRam0000000000228e18 = uStack_84;
                                                  _DAT_00228e10 = local_8c;
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
                            }
                          }
                        }
                      }
                      goto LAB_0014686c;
                    }
                  }
                }
              }
            }
          }
        }
        bVar3 = true;
        pcVar20 = "link_phase";
      }
    }
LAB_0014686c:
    cVar4 = (**(code **)(*param_1 + 0x720))(param_1);
    if (cVar4 != '\0') {
      (**(code **)(*param_1 + 0x88))(param_1);
    }
    (**(code **)(*param_1 + 0xa0))(param_1,0);
    if (bVar3) {
      pcVar21 = "spin_loader_refused";
      pcVar19 = ",\"action\":25,\"capability_issued\":false";
    }
    else {
      snprintf((char *)&local_1260,0x140,
               ",\"action\":25,\"loader_generation\":%llu,\"profile_sha256\":\"%s\",\"callback_verified\":true,\"self_file_verified\":true,\"paid_state_changed\":false"
               ,DAT_00214c18,"114ba105835bd4157cfd80aad9498734d51108ee193601410abeb5e0bab3a71c");
      pcVar21 = "spin_loader_verified";
      pcVar20 = "actual_signed_release_selection";
      pcVar19 = (char *)&local_1260;
    }
    FUN_001417c8(pcVar21,pcVar20,pcVar19);
  }
  if (*(long *)(lVar2 + 0x28) == local_78) {
    return;
  }
LAB_00146e38:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_001474c0 @ 001474c0 [libNexusEvasionRuntime69252.so] ===== */

void FUN_001474c0(int param_1)

{
  long lVar1;
  int iVar2;
  char *pcVar3;
  char *pcVar4;
  char *pcVar5;
  undefined8 local_80;
  undefined8 local_78;
  long lStack_70;
  code *local_68;
  code *pcStack_60;
  undefined8 local_58;
  undefined *puStack_50;
  code *local_48;
  code *pcStack_40;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  if ((DAT_00216ef8 == '\x01' && DAT_00216efc != '\0') && DAT_00216de0 != '\0') {
    if (((param_1 == 0) || (DAT_00216f08 == '\0')) || (DAT_00214c18 == 0)) {
      FUN_0015abf4();
      if (*(long *)(lVar1 + 0x28) == local_38) {
        pcVar3 = "verified_lab_loader_identity_missing";
LAB_0014769c:
        FUN_001417c8("spin_unavailable",pcVar3,0);
        return;
      }
      goto LAB_001476d8;
    }
    iVar2 = (*DAT_00215d98)(&DAT_00216e68);
    if (iVar2 != 1) {
      FUN_0015abf4();
      if (*(long *)(lVar1 + 0x28) == local_38) {
        pcVar3 = "post_proof_rejected";
        goto LAB_0014769c;
      }
      goto LAB_001476d8;
    }
    iVar2 = (*DAT_00215da8)(&DAT_00216eb0);
    if (iVar2 != 1) {
      FUN_0015abf4();
      if (*(long *)(lVar1 + 0x28) == local_38) {
        pcVar3 = "movement_post_proof_rejected";
        goto LAB_0014769c;
      }
      goto LAB_001476d8;
    }
    DAT_00216f00 = 1;
    DAT_00228e20 = 0x100000001;
    local_78 = DAT_00209d00;
    lStack_70 = DAT_00214c18;
    local_68 = FUN_001455a4;
    pcStack_60 = FUN_00159ba0;
    local_58 = 0;
    puStack_50 = PTR_FUN_001cb6f0;
    local_80 = DAT_0010e540;
    local_48 = FUN_0016aa6c;
    pcStack_40 = FUN_0016ab00;
    iVar2 = (*DAT_00215d78)(&local_80);
    if (iVar2 == 1) {
      pcVar3 = "spin_registered";
      pcVar4 = "verified_action25_lab_trial";
      pcVar5 = 
      ",\"action\":25,\"scope\":\"LAB_TRIAL\",\"modes\":[\"offline_facing_priority\",\"online_movement\"],\"rotation\":360,\"movement_reassert_rva\":\"0xe7b768\",\"paid_state_changed\":false,\"free_grant\":false"
      ;
      DAT_00216f04 = 1;
    }
    else {
      pcVar3 = "spin_unavailable";
      pcVar4 = "lab_trial_registration_rejected";
      pcVar5 = (char *)0x0;
      DAT_00228e20 = 0x200000000;
    }
    FUN_001417c8(pcVar3,pcVar4,pcVar5);
  }
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
LAB_001476d8:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00159ba0 @ 00159ba0 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00159ba0(long param_1)

{
  uint uVar1;
  uint uVar2;
  undefined4 uVar3;
  long lVar4;
  undefined8 uVar5;
  ulong uVar6;
  undefined4 uVar7;
  undefined8 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined8 uVar14;
  long lVar15;
  undefined8 uVar16;
  undefined8 uVar17;
  ulong uVar18;
  int iVar19;
  int iVar20;
  undefined4 *puVar21;
  long lVar22;
  ulong uVar23;
  long lVar24;
  undefined8 local_800;
  undefined8 local_7f8;
  undefined8 uStack_7f0;
  undefined8 local_7e8;
  undefined8 uStack_7e0;
  undefined8 local_7d8;
  undefined8 uStack_7d0;
  undefined8 local_7c8;
  undefined8 *local_7c0;
  undefined8 *puStack_7b8;
  undefined8 *local_7b0;
  undefined8 *puStack_7a8;
  undefined8 local_7a0;
  undefined8 *local_798;
  undefined8 *puStack_790;
  undefined8 local_788;
  undefined8 local_780;
  ulong local_778;
  undefined8 local_770;
  int local_768;
  int local_764;
  undefined8 local_760;
  undefined8 uStack_758;
  undefined8 local_750;
  ulong local_748;
  undefined8 local_740;
  uint local_738;
  undefined4 local_734;
  undefined8 local_730;
  undefined8 local_728;
  undefined8 local_720;
  undefined8 uStack_718;
  undefined8 uStack_710;
  undefined8 uStack_708;
  long local_700;
  undefined8 uStack_6f8;
  undefined8 local_6f0;
  undefined8 local_6e0;
  undefined8 uStack_6d8;
  ulong local_6d0;
  ulong local_6c8;
  undefined8 local_6c0;
  char acStack_6b8 [1400];
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
  ulong local_d0;
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
  long local_70;
  
  lVar4 = tpidr_el0;
  local_70 = *(long *)(lVar4 + 0x28);
  puVar21 = (undefined4 *)__errno();
  iVar20 = DAT_00209cd8;
  uVar3 = *puVar21;
  if (((((DAT_00216f04 == '\x01') && ((int)DAT_001dfff0 != 0)) && (DAT_00209cd8 != 0)) &&
      ((iVar19 = gettid(), iVar20 == iVar19 && (param_1 != 0)))) &&
     (((DAT_002170e4 & 1) == 0 && ((DAT_00216f10 != 0 && (DAT_00216f14 == 0)))))) {
    DAT_00216f14 = 1;
    DAT_002171a8 = DAT_002171a8 + 1;
    if ((DAT_00217090 != 1) &&
       (((((DAT_00216f28 ^ *(ulong *)(param_1 + 0x98)) & 0xffffffffffffff) == 0 &&
         (((DAT_00216f30 ^ *(ulong *)(param_1 + 0xc0)) & 0xffffffffffffff) == 0)) &&
        (DAT_00217030 == DAT_0020f650)))) {
      lVar22 = FUN_0015b4a0();
      uVar23 = FUN_001391f0(1);
      if ((uVar23 != 0) && (DAT_002170b0 <= uVar23)) {
        local_734 = 0;
        local_764 = iVar20;
        local_788 = 0;
        uStack_6d8 = DAT_00216f20;
        local_6e0 = _DAT_00216f18;
        local_6c8 = DAT_00216f30;
        local_6d0 = DAT_00216f28;
        uStack_98 = uRam0000000000216fe8;
        local_a0 = _DAT_00216fe0;
        uStack_88 = uRam0000000000216ff8;
        uStack_90 = _DAT_00216ff0;
        uStack_78 = uRam0000000000217008;
        local_80 = _DAT_00217000;
        uStack_d8 = uRam0000000000216fa8;
        local_e0 = _DAT_00216fa0;
        uStack_c8 = uRam0000000000216fb8;
        local_d0 = _DAT_00216fb0;
        local_6c0 = DAT_00216f38;
        uStack_b8 = uRam0000000000216fc8;
        local_c0 = _DAT_00216fc0;
        uStack_a8 = uRam0000000000216fd8;
        uStack_b0 = _DAT_00216fd0;
        uStack_118 = uRam0000000000216f68;
        local_120 = DAT_00216f60;
        uStack_108 = DAT_00216f78;
        uStack_110 = _DAT_00216f70;
        uStack_f8 = uRam0000000000216f88;
        local_100 = _DAT_00216f80;
        uStack_e8 = uRam0000000000216f98;
        uStack_f0 = _DAT_00216f90;
        uStack_138 = uRam0000000000216f48;
        local_140 = _DAT_00216f40;
        uStack_128 = uRam0000000000216f58;
        uStack_130 = _DAT_00216f50;
        uStack_718 = uRam0000000000217018;
        local_720 = _DAT_00217010;
        uStack_708 = uRam0000000000217028;
        uStack_710 = _DAT_00217020;
        uStack_6f8 = uRam0000000000217038;
        local_700 = DAT_00217030;
        local_6f0 = DAT_00217040;
        local_740 = DAT_0010e520;
        local_738 = DAT_00217080;
        local_730 = DAT_00217050;
        local_728 = DAT_00217084;
        local_768 = DAT_00209cd8;
        local_770 = DAT_0010e580;
        uStack_758 = _UNK_00112a98;
        local_760 = _DAT_00112a90;
        local_798 = &local_6e0;
        local_748 = DAT_00216f28;
        puStack_790 = &local_140;
        local_7a0 = DAT_0010e580;
        local_750 = DAT_0010e740;
        local_7c0 = &local_7a0;
        puStack_7b8 = &local_720;
        local_780 = DAT_0010e588;
        local_7c8 = DAT_0010e668;
        local_7b0 = &local_740;
        puStack_7a8 = &local_770;
        DAT_002170e4 = 1;
        uStack_7d0 = 0;
        local_7d8 = 0;
        uStack_7e0 = 0;
        local_7e8 = 0;
        uStack_7f0 = 0;
        local_7f8 = 0;
        local_800 = DAT_0010e7a8;
        local_778 = uVar23;
        iVar20 = FUN_0015acc4(&local_7c8,&local_800);
        uVar1 = 0;
        if (local_7f8._4_4_ != 0) {
          uVar1 = (uint)(iVar20 != 0);
        }
        uVar2 = 0;
        if ((int)uStack_7d0 != 0) {
          uVar2 = (uint)(iVar20 != 0);
        }
        local_760 = CONCAT44(uVar1,iVar20);
        uStack_758 = CONCAT44(uStack_758._4_4_,uVar2);
        FUN_0015b524(local_6c8);
        FUN_001bbda8(&DAT_00215de0,&local_7c8,&DAT_002171b0);
        uVar18 = local_d0;
        uVar17 = local_120;
        uVar16 = uStack_6d8;
        lVar15 = local_700;
        uVar14 = local_730;
        uVar1 = local_738;
        uVar10 = DAT_002171e4;
        uVar9 = DAT_002171dc;
        uVar8 = DAT_002171d0;
        uVar7 = DAT_002171bc;
        iVar20 = DAT_002171b8;
        uVar6 = DAT_002171a8;
        uVar5 = DAT_00214c18;
        DAT_002170e0 = 0;
        DAT_002170e4 = 0;
        DAT_002170e8 = 0;
        DAT_002170f0 = 0;
        if (((DAT_002171a8 < 9) || (DAT_002171b8 != DAT_001cfbc0)) ||
           ((DAT_002171f8 == 0 || (999 < uVar23 - DAT_002171f8)))) {
          uVar11 = (undefined4)local_760;
          uVar12 = local_760._4_4_;
          uVar13 = (undefined4)uStack_758;
          lVar24 = FUN_0015b4a0();
          snprintf(acStack_6b8,0x578,
                   ",\"action\":25,\"scope\":\"LAB_TRIAL\",\"mode\":\"offline\",\"publication\":%llu,\"sequence\":%llu,\"epoch\":%llu,\"own_gid\":%d,\"requested\":%u,\"generation\":%llu,\"loader_generation\":%llu,\"post_entry_rva\":\"0xb30690\",\"post_calls\":%llu,\"active_known\":%u,\"active\":%u,\"ended\":%u,\"provider_reason\":%u,\"angle\":%d,\"writes_completed\":%u,\"readback_verified\":%u,\"field_apply_calls\":%llu,\"duration_us\":%llu,\"native_calls\":0,\"client_inputs\":0,\"paid_state_changed\":false"
                   ,lVar15,uVar16,uVar17,uVar18 & 0xffffffff,(ulong)uVar1,uVar14,uVar5,uVar6,uVar11,
                   uVar12,uVar13,iVar20,uVar9,uVar7,uVar10,uVar8,lVar24 - lVar22);
          FUN_001417c8("spin_post","post_natural_local_facing",acStack_6b8);
          DAT_001cfbc0 = DAT_002171b8;
          DAT_002171f8 = uVar23;
        }
      }
    }
  }
  *puVar21 = uVar3;
  if (*(long *)(lVar4 + 0x28) == local_70) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0015abf4 @ 0015abf4 [libNexusEvasionRuntime69252.so] ===== */

void FUN_0015abf4(void)

{
  int iVar1;
  
  if (DAT_00216efc == '\x01') {
    iVar1 = FUN_0015a83c(1);
    if (iVar1 == 0) {
      return;
    }
    iVar1 = FUN_0015aa90(DAT_00216ed8,DAT_00216ef4,DAT_00216ef0);
    if (iVar1 < 0) {
      FUN_001417c8("fatal","spin_movement_post_restore_unverified",0);
                    /* WARNING: Subroutine does not return */
      abort();
    }
    if (iVar1 == 1) {
      DAT_00216efc = '\0';
    }
  }
  if ((DAT_00216ef8 == '\x01') && (iVar1 = FUN_0015a11c(1), iVar1 != 0)) {
    iVar1 = FUN_0015aa90(DAT_00216e90,DAT_00216eac,DAT_00216ea8);
    if (iVar1 < 0) {
      FUN_001417c8("fatal","spin_post_restore_unverified",0);
                    /* WARNING: Subroutine does not return */
      abort();
    }
    if (iVar1 == 1) {
      DAT_00216ef8 = '\0';
    }
  }
  return;
}

/* ===== FUN_001603a8 @ 001603a8 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_001603a8(long param_1)

{
  int iVar1;
  ulong uVar2;
  long lVar3;
  int iVar4;
  int iVar5;
  undefined8 uVar6;
  ulong uVar7;
  long lVar8;
  float local_458;
  float fStack_454;
  undefined8 local_450;
  undefined8 local_448;
  undefined8 uStack_440;
  undefined8 local_438;
  undefined8 uStack_430;
  undefined8 local_428;
  undefined8 uStack_420;
  undefined8 local_418;
  undefined8 local_410;
  undefined8 local_408;
  undefined8 uStack_400;
  undefined8 local_3f8;
  undefined8 uStack_3f0;
  undefined8 local_3e8;
  undefined4 local_3e0;
  undefined1 auStack_3d8 [44];
  int local_3ac;
  ulong local_398;
  char acStack_390 [800];
  long local_70;
  
  iVar4 = DAT_00209cd8;
  lVar3 = tpidr_el0;
  local_70 = *(long *)(lVar3 + 0x28);
  if ((((((param_1 != 0) && (DAT_00216e60 != '\0')) && (DAT_00216f10 != 0)) &&
       (((*(long *)(param_1 + 8) != 0 && (lVar8 = *(long *)(param_1 + 0x10), lVar8 != 0)) &&
        ((*(int *)(param_1 + 0x24) != 0 && ((DAT_00217080 != 0 && (DAT_00217090 == 1)))))))) &&
      (0 < (int)DAT_00217094)) &&
     (((DAT_00209cd8 != 0 && (uVar6 = gettid(), iVar4 == (int)uVar6)) && ((int)DAT_0020f638 != 0))))
  {
    local_398 = 0;
    iVar4 = FUN_001428fc(uVar6,*(long *)(lVar8 + 0x30) + 0x58,&local_398,8);
    if (((iVar4 != 0) && (0x11fff < local_398 + 0x2000)) &&
       (((local_398 & 7) == 0 &&
        ((iVar4 = FUN_00189c88(DAT_001e0978,*(undefined8 *)(lVar8 + 0x40),FUN_001428fc,0,auStack_3d8
                              ), iVar4 != 0 && (local_3ac != 0)))))) {
      uStack_400 = *(undefined8 *)(lVar8 + 0x28);
      local_408 = *(undefined8 *)(lVar8 + 0x20);
      uStack_3f0 = *(undefined8 *)(lVar8 + 0x38);
      local_3f8 = *(undefined8 *)(lVar8 + 0x30);
      local_410 = *(undefined8 *)(*(long *)(param_1 + 8) + 8);
      local_3e8 = *(undefined8 *)(lVar8 + 0x40);
      local_3e0 = *(undefined4 *)(lVar8 + 0x70);
      local_418 = DAT_0010e7f8;
      uStack_420 = 0;
      local_428 = 0;
      uStack_430 = 0;
      local_438 = 0;
      uStack_440 = 0;
      local_448 = 0;
      local_450 = DAT_0010e7a8;
      if ((DAT_0020adec == '\x01') &&
         ((((iVar4 = FUN_0018a8cc(&DAT_0020adf0,&local_418,&local_450), iVar4 == 1 &&
            ((int)local_448 != 0)) && (local_448._4_4_ != 0)) && ((int)uStack_420 == 0)))) {
        DAT_00217120 = *(undefined8 *)(lVar8 + 0x20);
        DAT_00217130 = *(undefined8 *)(param_1 + 0x28);
        DAT_00217100 = DAT_0010e670;
        DAT_00217140 = *(undefined8 *)(lVar8 + 0x30);
        DAT_00217138 = *(undefined8 *)(lVar8 + 0x28);
        DAT_00217128 = DAT_00217050;
        DAT_00217150 = *(undefined8 *)(lVar8 + 0x40);
        DAT_00217148 = *(undefined8 *)(lVar8 + 0x38);
        _DAT_00217160 = *(ulong *)(lVar8 + 0x70);
        DAT_00217108 = 1;
        DAT_0021710c = DAT_0020f638._4_4_;
        DAT_00217168 = *(undefined4 *)(lVar8 + 0x78);
        DAT_00217190 = 0;
        DAT_00217198 = 0;
        DAT_00217110 = 0;
        DAT_00217118 = DAT_0020f650;
        DAT_00217158 = local_398;
        uRam0000000000217174 = (undefined4)_UNK_00112a48;
        uRam0000000000217178 = (undefined4)((ulong)_UNK_00112a48 >> 0x20);
        _DAT_0021716c = (undefined4)_DAT_00112a40;
        _DAT_00217170 = (undefined4)((ulong)_DAT_00112a40 >> 0x20);
        uRam0000000000217184 = (undefined4)_UNK_00112c38;
        uRam0000000000217188 = (undefined4)((ulong)_UNK_00112c38 >> 0x20);
        _DAT_0021717c = (undefined4)_DAT_00112c30;
        _DAT_00217180 = (undefined4)((ulong)_DAT_00112c30 >> 0x20);
        DAT_002171a0._0_4_ = 0;
        if (((int)DAT_00217098 != 0) &&
           ((DAT_00228c58 == 0 || (iVar4 = FUN_0016b778(&DAT_00217100), iVar4 == 0)))) {
          DAT_00228c60 = DAT_00228c60 + 1;
          if (DAT_00228cf8 == 1) {
            DAT_00228cb8 = DAT_00228cb8 + 1;
          }
          DAT_00228cf8 = 0;
          DAT_00228cc0 = 0;
          DAT_00228cd0 = 0;
          DAT_00228ce8 = 0;
          DAT_00228cc8 = 0;
          DAT_00228c88 = 0;
          uRam0000000000228ca8 = 0;
          DAT_00228c90 = 0;
          _DAT_00228c78 = 0;
          _DAT_00228c70 = 0;
          DAT_00228c68 = DAT_0010e6c0;
          DAT_00228cd8 = 0.0;
          DAT_00228cb0 = 0;
          DAT_00228c80 = DAT_00217118;
          _DAT_00228ca0 = (ulong)DAT_0021710c;
          DAT_00228c98 = DAT_00228cb8;
          goto LAB_001607f0;
        }
        uRam0000000000228ca8 = 0;
        DAT_00228cb0 = 0;
        DAT_00228c88 = 0;
        DAT_00228c98 = 0;
        DAT_00228c90 = 0;
        _DAT_00228c78 = 0;
        DAT_00228c68 = DAT_0010e6c0;
        _DAT_00228ca0 = (ulong)DAT_0021710c;
        if (((DAT_00228cc0 == *(long *)(lVar8 + 0x40)) && (DAT_00228cc8 == *(long *)(lVar8 + 0x20)))
           && (DAT_00228cd0 != 0)) {
          uVar7 = *(ulong *)(param_1 + 0x28);
          uVar2 = uVar7 - DAT_00228cd0;
          if ((uVar7 < DAT_00228cd0) || (0xfa < uVar2)) goto LAB_00160690;
          if (99 < uVar2) {
            uVar2 = 100;
          }
          if (0xf < uVar2) {
            DAT_00228cd8 = (float)NEON_fmadd((float)uVar2 * (float)(int)DAT_00217084,0x358637bd,
                                             DAT_00228cd8);
            DAT_00228cd0 = uVar7;
          }
          if (DAT_00228cd8 < 6.2831855 == NAN(DAT_00228cd8)) {
            do {
              DAT_00228cd8 = DAT_00228cd8 + -6.2831855;
            } while (DAT_00228cd8 < 6.2831855 == NAN(DAT_00228cd8));
          }
        }
        else {
LAB_00160690:
          if (DAT_00228cf8 == 1) {
            DAT_00228cb8 = DAT_00228cb8 + 1;
          }
          DAT_00228cf8 = 0;
          DAT_00228cc8 = *(long *)(lVar8 + 0x20);
          DAT_00228cd0 = *(ulong *)(param_1 + 0x28);
          DAT_00228ce8 = 0;
          DAT_00228cd8 = 0.0;
          DAT_00228c98 = DAT_00228cb8;
          DAT_00228cc0 = *(long *)(lVar8 + 0x40);
        }
        _DAT_00228c70 = 0;
        DAT_002170f8 = 1;
        DAT_00228ce0 = DAT_00228ce0 + 1;
        DAT_00228c80 = DAT_00217118;
        FUN_00169ce4();
        iVar4 = *(int *)(lVar8 + 0x74);
        sincosf(DAT_00228cd8,&fStack_454,&local_458);
        iVar4 = iVar4 + (int)(long)local_458;
        iVar1 = *(int *)(lVar8 + 0x78) + (int)(long)fStack_454;
        if ((int)(long)local_458 == 0 && (int)(long)fStack_454 == 0) {
          iVar4 = iVar4 + 1;
        }
        DAT_00228cb0 = CONCAT44(iVar1,iVar4);
        if (((DAT_00228ce8 == 0) || (*(ulong *)(param_1 + 0x28) < DAT_00228ce8)) ||
           (0x17 < *(ulong *)(param_1 + 0x28) - DAT_00228ce8)) {
          iVar5 = (*DAT_00216e28)(DAT_00216e20,&DAT_00217100,1,iVar4,iVar1);
          if (iVar5 != 1) goto LAB_001609ac;
          (*DAT_00216e50)(DAT_00216e20,*(undefined8 *)(lVar8 + 0x38),iVar4,iVar1,1);
          _DAT_00228c78 = CONCAT44(DAT_00228c7c,1);
          DAT_00228d48 = DAT_00217148;
          DAT_00228d40 = DAT_00217140;
          DAT_00228d58 = DAT_00217158;
          DAT_00228d50 = DAT_00217150;
          uRam0000000000228d88 = CONCAT44(uRam000000000021718c,uRam0000000000217188);
          _DAT_00228d80 = CONCAT44(uRam0000000000217184,_DAT_00217180);
          DAT_00228c70 = 1;
          DAT_00228cf0 = DAT_00228cf0 + 1;
          DAT_00228cf8 = 1;
          uRam0000000000228d98 = DAT_00217198;
          _DAT_00228d90 = DAT_00217190;
          DAT_00228da0 = CONCAT44(DAT_002171a0._4_4_,(undefined4)DAT_002171a0);
          uRam0000000000228d68 = CONCAT44(_DAT_0021716c,DAT_00217168);
          uRam0000000000228d78 = CONCAT44(_DAT_0021717c,uRam0000000000217178);
          _DAT_00228d70 = CONCAT44(uRam0000000000217174,_DAT_00217170);
          _DAT_00228d60 = _DAT_00217160;
          DAT_00228da8 = DAT_00217118;
          DAT_00228d28 = DAT_00217128;
          DAT_00228d20 = DAT_00217120;
          DAT_00228d38 = DAT_00217138;
          _DAT_00228d30 = DAT_00217130;
          uRam0000000000228d08 = CONCAT44(DAT_0021710c,DAT_00217108);
          DAT_00228db0 = DAT_0021710c;
          DAT_00228ce8 = *(ulong *)(param_1 + 0x28);
          _DAT_00228d00 = DAT_00217100;
          uRam0000000000228d18 = DAT_00217118;
          _DAT_00228d10 = DAT_00217110;
          DAT_00228db4 = iVar4;
          DAT_00228db8 = iVar1;
        }
        else if (DAT_00228c7c == 0) {
          DAT_00228c70 = 7;
        }
        else {
LAB_001609ac:
          if (DAT_00228c70 == 0) {
            DAT_00228c70 = 5;
          }
        }
        iVar4 = DAT_00228c70;
        DAT_00228c90 = DAT_00228dc0;
        DAT_00228c98 = DAT_00228cb8;
        DAT_00228c88 = DAT_00228cf0;
        DAT_002170f8 = 0;
        lVar8 = *(long *)(param_1 + 0x28);
        _DAT_00228c70 = (ulong)CONCAT14(DAT_00228cf8,DAT_00228c70);
        if (((DAT_00228ce0 < 9) || (DAT_00228dc8 == 0)) || (999 < (ulong)(lVar8 - DAT_00228dc8))) {
          snprintf(acStack_390,800,
                   ",\"action\":25,\"mode\":\"online\",\"publication\":%llu,\"source_tick\":%u,\"epoch\":%llu,\"own_gid\":%d,\"configured_radius\":%d,\"effective_radius_raw\":1,\"only_standing\":%d,\"priority_yields\":%llu,\"phase\":%.6f,\"apply_reason\":%u,\"pending\":%u,\"local_calls\":%llu,\"queue_calls\":%llu,\"target_x\":%d,\"target_y\":%d,\"native_acceptance_proven\":false"
                   ,(double)DAT_00228cd8,DAT_00217118,(ulong)DAT_0021710c,DAT_00217120,
                   _DAT_00217160 & 0xffffffff,(ulong)DAT_00217094,(int)DAT_00217098,DAT_00228c60,
                   iVar4,(uint)DAT_00228cf8,DAT_00228cf0,DAT_00228dc0,(undefined4)DAT_00228cb0,
                   DAT_00228cb0._4_4_);
          FUN_001417c8("spin_move","tale_style_real_client_input",acStack_390);
          DAT_00228dc8 = lVar8;
        }
        goto LAB_001607f0;
      }
    }
  }
  if (DAT_00228cf8 == 1) {
    DAT_00228cb8 = DAT_00228cb8 + 1;
  }
  DAT_00228cf8 = 0;
  _DAT_00228c70 = _DAT_00228c70 & 0xffffffff;
  DAT_00228cc0 = 0;
  DAT_00228ce8 = 0;
  DAT_00228cd0 = 0;
  DAT_00228cc8 = 0;
  DAT_00228cd8 = 0.0;
  DAT_00228c98 = DAT_00228cb8;
LAB_001607f0:
  if (*(long *)(lVar3 + 0x28) == local_70) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

