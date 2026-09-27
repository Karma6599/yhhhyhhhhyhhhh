/*
 * Autofarm / automation (pin, spray, play-again) — Feature
 * Decompiled with Ghidra 11.3.2 (arm64 pseudocode) from: libNexusEvasion69252.so, libNexusEvasionRuntime69252.so, libNexusUI69252.so
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
 * Notes: Auto play-again + autofarm click/attack automation with delay tuning.
 */

/* ===== nexus_evasion_handler_status_v1 @ 00111f3c [libNexusEvasion69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void nexus_evasion_handler_status_v1(char *param_1,int *param_2)

{
  char *pcVar1;
  uint uVar2;
  uint uVar3;
  char cVar4;
  long lVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  char cVar8;
  bool bVar9;
  int iVar10;
  int iVar11;
  uint uVar12;
  int iVar13;
  int iVar14;
  uint uVar15;
  ulong uVar16;
  ulong uVar17;
  long lVar18;
  code *pcVar19;
  undefined8 uVar20;
  uint uVar21;
  undefined8 uVar22;
  undefined **ppuVar23;
  uint uVar24;
  undefined8 local_a0;
  undefined8 uStack_98;
  undefined8 uStack_90;
  undefined8 uStack_88;
  undefined8 local_80;
  int local_6c;
  long local_68;
  
  lVar5 = tpidr_el0;
  local_68 = *(long *)(lVar5 + 0x28);
  iVar10 = FUN_00112940();
  if (iVar10 == 0) {
    if (param_1 != (char *)0x0) {
      iVar10 = strcmp(param_1,"pinEnabled");
      if ((iVar10 == 0) || (iVar10 = strcmp(param_1,"sprayEnabled"), iVar10 == 0)) {
        if ((param_2 == (int *)0x0) || ((*param_2 != 1 || (param_2[1] != 0x48)))) goto LAB_0011268c;
        do {
          cVar8 = DAT_00129ce8;
          cVar4 = '\x01';
          bVar9 = (bool)ExclusiveMonitorPass(0x129ce8,0x10);
          if (bVar9) {
            _DAT_00129ce8 = CONCAT31(DAT_00129ce8_1,1);
            cVar4 = ExclusiveMonitorsStatus();
          }
        } while (cVar4 != '\0' || cVar8 != '\0');
        FUN_0010f580();
        if (*param_1 != '\0') {
          lVar18 = 1;
          do {
            pcVar1 = param_1 + lVar18;
            lVar18 = lVar18 + 1;
          } while (*pcVar1 != '\0');
        }
        iVar10 = strcmp(param_1,"isSpinEnabled");
        lVar18 = 0;
        if (iVar10 != 0) {
          ppuVar23 = &PTR_s_cameraEnabled_001216c0;
          do {
            lVar18 = lVar18 + 1;
            iVar10 = strcmp(param_1,*ppuVar23);
            ppuVar23 = ppuVar23 + 5;
          } while (iVar10 != 0);
        }
        uVar24 = 0;
        iVar10 = (&DAT_00129130)[(int)(&DAT_001216a0)[lVar18 * 10]];
        if ((DAT_00129450 != 0) && (DAT_00129468 != (code *)0x0)) {
          iVar11 = (*DAT_00129468)();
          if ((iVar11 == 1) &&
             (((DAT_001293d0 == DAT_00129450 && (DAT_001293a8 == DAT_00129458)) &&
              (iVar11 = pthread_once((pthread_once_t *)&DAT_00129cd8,FUN_0011b480),
              DAT_00129ce0 != (code *)0x0)))) {
            iVar11 = (*DAT_00129ce0)(iVar11);
            uVar24 = (uint)(iVar11 == 1);
          }
          else {
            uVar24 = 0;
          }
        }
        if (DAT_00129418 == (code *)0x0) {
          uVar15 = 0;
          lVar18 = DAT_00129450;
        }
        else {
          uVar15 = (*DAT_00129418)(DAT_00129410);
          lVar18 = DAT_00129450;
        }
LAB_0011271c:
        uVar22 = DAT_001298e8;
        uVar20 = DAT_00129348;
        iVar11 = 5;
        if (iVar10 == 0) {
          iVar11 = 1;
        }
        uVar12 = 0;
        if (uVar24 == 0) {
          iVar11 = 3;
        }
        else {
          uVar12 = (uint)(iVar10 != 0);
        }
        uVar2 = 0;
        if (DAT_001293c4 == 0) {
          uVar2 = uVar12;
        }
        uVar21 = 0;
        if (((uVar15 ^ 0xffffffff) & 0x7f) == 0) {
          uVar21 = uVar2;
        }
        iVar14 = 2;
        if (lVar18 != 0) {
          iVar14 = iVar11;
        }
        *(undefined8 *)param_2 = DAT_001047a0;
        param_2[2] = iVar10;
        param_2[3] = (uint)(lVar18 != 0);
        param_2[4] = uVar24;
        goto LAB_00112854;
      }
      iVar10 = FUN_00112b5c(param_1);
      if (iVar10 != 0) goto LAB_0011228c;
      iVar10 = strcmp(param_1,"holdToShootEnabled");
      if (iVar10 == 0) {
LAB_00112588:
        bVar9 = true;
      }
      else {
        iVar10 = strcmp(param_1,"holdToShootAim");
        if ((iVar10 != 0) && (iVar10 = strcmp(param_1,"holdToShootRangeCheck"), iVar10 != 0)) {
          uVar24 = strcmp(param_1,"isSpinEnabled");
          uVar17 = (ulong)uVar24;
          if (uVar24 == 0) {
            if (*(long *)(lVar5 + 0x28) == local_68) {
              FUN_00112be8(param_2);
              return;
            }
            goto LAB_0011293c;
          }
          goto LAB_001123c0;
        }
        iVar10 = strcmp(param_1,"holdToShootAim");
        if (iVar10 == 0) goto LAB_00112588;
        iVar10 = strcmp(param_1,"holdToShootRangeCheck");
        bVar9 = iVar10 == 0;
      }
      uVar17 = 0;
      if ((param_2 == (int *)0x0) || (!bVar9)) goto LAB_00112690;
      if ((*param_2 != 1) || (param_2[1] != 0x48)) goto LAB_0011268c;
      do {
        cVar8 = DAT_00129ce8;
        cVar4 = '\x01';
        bVar9 = (bool)ExclusiveMonitorPass(0x129ce8,0x10);
        if (bVar9) {
          _DAT_00129ce8 = CONCAT31(DAT_00129ce8_1,1);
          cVar4 = ExclusiveMonitorsStatus();
        }
      } while (cVar4 != '\0' || cVar8 != '\0');
      FUN_0010f580(0);
      if (*param_1 != '\0') {
        lVar18 = 1;
        do {
          pcVar1 = param_1 + lVar18;
          lVar18 = lVar18 + 1;
        } while (*pcVar1 != '\0');
      }
      iVar10 = strcmp(param_1,"isSpinEnabled");
      lVar18 = 0;
      if (iVar10 != 0) {
        ppuVar23 = &PTR_s_cameraEnabled_001216c0;
        do {
          lVar18 = lVar18 + 1;
          iVar10 = strcmp(param_1,*ppuVar23);
          ppuVar23 = ppuVar23 + 5;
        } while (iVar10 != 0);
      }
      iVar10 = (&DAT_00129130)[(int)(&DAT_001216a0)[lVar18 * 10]];
      iVar11 = FUN_00116854();
      iVar14 = FUN_00117314(0);
      uVar20 = DAT_00129bb8;
      lVar18 = DAT_00129b48;
      bVar9 = DAT_00129b48 != 0;
      if (DAT_00129b48 == 0) {
        iVar13 = 2;
      }
      else if (iVar11 == 0) {
        iVar13 = 3;
      }
      else {
        iVar13 = 4;
        if (iVar14 != 0) {
          iVar13 = 5;
        }
        if (iVar10 == 0) {
          iVar13 = 1;
        }
      }
      param_2[2] = iVar10;
      param_2[3] = (uint)bVar9;
      uVar22 = DAT_001047a0;
      param_2[4] = iVar11;
      param_2[5] = (uint)(iVar10 != 0 && iVar14 != 0);
      uVar6 = DAT_00129348;
      param_2[6] = 0;
      param_2[7] = iVar13;
      uVar7 = DAT_00129c28;
      *(undefined8 *)(param_2 + 0x10) = uVar20;
      *(undefined8 *)param_2 = uVar22;
      param_2[8] = 0;
      param_2[9] = 0;
      *(undefined8 *)(param_2 + 10) = uVar6;
      *(undefined8 *)(param_2 + 0xc) = uVar7;
      *(long *)(param_2 + 0xe) = lVar18;
      goto LAB_0011286c;
    }
    iVar10 = FUN_00112b5c(0);
    if (iVar10 == 0) {
LAB_001123c0:
      uVar17 = FUN_00112d44(param_1);
      if (-1 < (int)uVar17) {
        if (*(long *)(lVar5 + 0x28) == local_68) {
          FUN_00112e84(param_1,param_2);
          return;
        }
        goto LAB_0011293c;
      }
      if (((param_2 == (int *)0x0) || (*param_2 != 1)) || (param_2[1] != 0x48)) goto LAB_0011268c;
      uVar17 = FUN_0010f504(param_1);
      if (uVar17 != 0) {
        if (*(int *)(uVar17 + 0xc) != 0) goto LAB_0011268c;
        do {
          cVar8 = DAT_00129ce8;
          cVar4 = '\x01';
          bVar9 = (bool)ExclusiveMonitorPass(0x129ce8,0x10);
          if (bVar9) {
            _DAT_00129ce8 = CONCAT31(DAT_00129ce8_1,1);
            cVar4 = ExclusiveMonitorsStatus();
          }
        } while (cVar4 != '\0' || cVar8 != '\0');
        FUN_0010f580();
        uVar15 = FUN_001130c0(param_1);
        uVar12 = FUN_00113200();
        uVar24 = (uint)(DAT_001293e8 != 0) | (uint)(DAT_001293f0 != 0) << 1 |
                 (uint)(DAT_001293f8 != 0) << 2;
        pcVar19 = DAT_00129418;
        if (DAT_00129418 != (code *)0x0) {
          uVar16 = (*DAT_00129418)(DAT_00129410);
          pcVar19 = (code *)(uVar16 & 0xffffffff);
        }
        local_6c = 0;
        local_80 = 0;
        uStack_98 = 0;
        local_a0 = 0;
        uStack_88 = 0;
        uStack_90 = 0;
        iVar11 = FUN_001133d4(uVar12,(ulong)pcVar19 & 0xffffffff,&local_a0,&local_6c);
        uVar3 = 1 << (ulong)(uVar15 & 0x1f);
        iVar10 = (&DAT_00129130)[*(int *)(uVar17 + 8)];
        uVar2 = uVar12 & uVar3;
        uVar21 = 0;
        if (uVar2 != 0) {
          uVar21 = (uint)(-1 < (int)uVar15);
        }
        iVar13 = FUN_0011359c(uVar12);
        uVar6 = DAT_001298e8;
        lVar18 = DAT_001293d0;
        uVar22 = DAT_00129348;
        uVar20 = DAT_001047a0;
        uVar15 = (uint)(-1 < (int)uVar15 && (uVar24 & uVar3) != 0);
        iVar14 = 2;
        if (uVar15 != 0) {
          iVar14 = 3;
        }
        if ((uVar15 == 1) && (uVar2 != 0)) {
          if (iVar10 == 0) {
            iVar14 = 1;
          }
          else {
            iVar14 = 4;
            if (iVar13 != 0) {
              iVar14 = local_6c;
            }
          }
        }
        uVar12 = 0;
        if (iVar10 != 0) {
          uVar12 = uVar21;
        }
        uVar2 = 0;
        if (iVar13 != 0) {
          uVar2 = uVar12;
        }
        uVar3 = 0;
        if (iVar11 != 0) {
          uVar3 = uVar12;
        }
        uVar17 = 1;
        param_2[8] = (int)pcVar19;
        param_2[9] = uVar24;
        uVar7 = DAT_00129900;
        *(undefined8 *)param_2 = uVar20;
        param_2[2] = iVar10;
        param_2[3] = uVar15;
        param_2[4] = uVar21;
        param_2[5] = uVar2;
        param_2[6] = uVar3;
        param_2[7] = iVar14;
        *(undefined8 *)(param_2 + 10) = uVar22;
        *(undefined8 *)(param_2 + 0xc) = uVar6;
        *(long *)(param_2 + 0xe) = lVar18;
        *(undefined8 *)(param_2 + 0x10) = uVar7;
        _DAT_00129ce8 = 0;
      }
    }
    else {
LAB_0011228c:
      if (((param_2 != (int *)0x0) && (*param_2 == 1)) && (param_2[1] == 0x48)) {
        do {
          cVar8 = DAT_00129ce8;
          cVar4 = '\x01';
          bVar9 = (bool)ExclusiveMonitorPass(0x129ce8,0x10);
          if (bVar9) {
            _DAT_00129ce8 = CONCAT31(DAT_00129ce8_1,1);
            cVar4 = ExclusiveMonitorsStatus();
          }
        } while (cVar4 != '\0' || cVar8 != '\0');
        FUN_0010f580();
        if (*param_1 != '\0') {
          lVar18 = 1;
          do {
            pcVar1 = param_1 + lVar18;
            lVar18 = lVar18 + 1;
          } while (*pcVar1 != '\0');
        }
        iVar10 = strcmp(param_1,"isSpinEnabled");
        lVar18 = 0;
        if (iVar10 != 0) {
          ppuVar23 = &PTR_s_cameraEnabled_001216c0;
          do {
            lVar18 = lVar18 + 1;
            iVar10 = strcmp(param_1,*ppuVar23);
            ppuVar23 = ppuVar23 + 5;
          } while (iVar10 != 0);
        }
        uVar24 = 0;
        iVar10 = (&DAT_00129130)[(int)(&DAT_001216a0)[lVar18 * 10]];
        if ((DAT_00129428 != 0) && (DAT_00129440 != (code *)0x0)) {
          iVar11 = (*DAT_00129440)();
          if (((iVar11 == 1) && ((DAT_001293d0 == DAT_00129428 && (DAT_001293a8 == DAT_00129430))))
             && (iVar11 = pthread_once((pthread_once_t *)&DAT_00129cd8,FUN_0011b480),
                DAT_00129ce0 != (code *)0x0)) {
            iVar11 = (*DAT_00129ce0)(iVar11);
            uVar24 = (uint)(iVar11 == 1);
          }
          else {
            uVar24 = 0;
          }
        }
        if (DAT_00129418 == (code *)0x0) {
          uVar15 = 0;
          lVar18 = DAT_00129428;
        }
        else {
          uVar15 = (*DAT_00129418)(DAT_00129410);
          lVar18 = DAT_00129428;
        }
        goto LAB_0011271c;
      }
LAB_0011268c:
      uVar17 = 0;
    }
  }
  else {
    if (((param_2 == (int *)0x0) || (*param_2 != 1)) || (param_2[1] != 0x48)) goto LAB_0011268c;
    do {
      cVar8 = DAT_00129ce8;
      cVar4 = '\x01';
      bVar9 = (bool)ExclusiveMonitorPass(0x129ce8,0x10);
      if (bVar9) {
        _DAT_00129ce8 = CONCAT31(DAT_00129ce8_1,1);
        cVar4 = ExclusiveMonitorsStatus();
      }
    } while (cVar4 != '\0' || cVar8 != '\0');
    FUN_0010f580();
    if (*param_1 != '\0') {
      lVar18 = 1;
      do {
        pcVar1 = param_1 + lVar18;
        lVar18 = lVar18 + 1;
      } while (*pcVar1 != '\0');
    }
    iVar10 = strcmp(param_1,"isSpinEnabled");
    lVar18 = 0;
    if (iVar10 != 0) {
      ppuVar23 = &PTR_s_cameraEnabled_001216c0;
      do {
        lVar18 = lVar18 + 1;
        iVar10 = strcmp(param_1,*ppuVar23);
        ppuVar23 = ppuVar23 + 5;
      } while (iVar10 != 0);
    }
    uVar24 = 0;
    iVar10 = (&DAT_00129130)[(int)(&DAT_001216a0)[lVar18 * 10]];
    if ((DAT_00129478 != 0) && (DAT_00129490 != (code *)0x0)) {
      iVar11 = (*DAT_00129490)();
      if ((iVar11 == 1) && ((DAT_001293d0 == DAT_00129478 && (DAT_001293a8 == DAT_00129480)))) {
        iVar11 = FUN_0011b320(param_1);
        if (iVar11 == 0) {
          iVar11 = pthread_once((pthread_once_t *)&DAT_00129cd8,FUN_0011b480);
          if (DAT_00129ce0 == (code *)0x0) goto LAB_001127bc;
          iVar11 = (*DAT_00129ce0)(iVar11);
          uVar24 = (uint)(iVar11 == 1);
        }
        else {
          uVar24 = 1;
        }
      }
      else {
LAB_001127bc:
        uVar24 = 0;
      }
    }
    if (DAT_00129418 == (code *)0x0) {
      uVar15 = 0;
    }
    else {
      uVar15 = (*DAT_00129418)(DAT_00129410);
    }
    uVar22 = DAT_001298e8;
    lVar18 = DAT_00129478;
    uVar20 = DAT_00129348;
    iVar11 = 5;
    if (iVar10 == 0) {
      iVar11 = 1;
    }
    uVar12 = 0;
    if (uVar24 == 0) {
      iVar11 = 3;
    }
    else {
      uVar12 = (uint)(iVar10 != 0);
    }
    uVar2 = 0;
    if (DAT_001293c4 == 0) {
      uVar2 = uVar12;
    }
    uVar21 = 0;
    if (((uVar15 ^ 0xffffffff) & 0x7f) == 0) {
      uVar21 = uVar2;
    }
    bVar9 = DAT_00129478 != 0;
    iVar14 = 2;
    if (bVar9) {
      iVar14 = iVar11;
    }
    *(undefined8 *)param_2 = DAT_001047a0;
    param_2[2] = iVar10;
    param_2[3] = (uint)bVar9;
    param_2[4] = uVar24;
LAB_00112854:
    param_2[5] = uVar12;
    param_2[6] = uVar21;
    param_2[7] = iVar14;
    param_2[8] = uVar15;
    param_2[9] = 0;
    *(undefined8 *)(param_2 + 10) = uVar20;
    *(undefined8 *)(param_2 + 0xc) = uVar22;
    *(long *)(param_2 + 0xe) = lVar18;
    param_2[0x10] = 0;
    param_2[0x11] = 0;
LAB_0011286c:
    uVar17 = 1;
    _DAT_00129ce8 = 0;
  }
LAB_00112690:
  if (*(long *)(lVar5 + 0x28) == local_68) {
    return;
  }
LAB_0011293c:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar17);
}

/* ===== nexus_evasion_snapshot_keys_v1 @ 0011ae20 [libNexusEvasion69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void nexus_evasion_snapshot_keys_v1
               (undefined8 *param_1,long param_2,long param_3,undefined8 *param_4,
               undefined4 *param_5)

{
  uint uVar1;
  uint uVar2;
  char cVar3;
  bool bVar4;
  long lVar5;
  char cVar6;
  int iVar7;
  uint uVar8;
  undefined4 uVar9;
  uint uVar10;
  uint uVar11;
  int iVar12;
  undefined8 uVar13;
  long lVar14;
  long *plVar15;
  long lVar16;
  undefined **ppuVar17;
  undefined8 *puVar18;
  undefined4 *puVar19;
  char *pcVar20;
  uint *puVar21;
  undefined **ppuVar22;
  long local_4e0 [142];
  long local_70;
  
  lVar5 = tpidr_el0;
  uVar13 = 0;
  local_70 = *(long *)(lVar5 + 0x28);
  if ((((param_3 - 1U < 0x8e) && (param_1 != (undefined8 *)0x0)) && (param_2 != 0)) &&
     ((param_4 != (undefined8 *)0x0 && (param_5 != (undefined4 *)0x0)))) {
    lVar16 = 0;
    do {
      pcVar20 = (char *)param_1[lVar16];
      if (pcVar20 == (char *)0x0) {
LAB_0011af8c:
        uVar13 = 0;
        goto LAB_0011af90;
      }
      lVar14 = 0;
      while (pcVar20[lVar14] != '\0') {
        lVar14 = lVar14 + 1;
        if (lVar14 == 0x60) goto LAB_0011af8c;
      }
      lVar14 = 0x8e;
      ppuVar22 = &PTR_s_isSpinEnabled_00121698;
      while (iVar7 = strcmp(pcVar20,*ppuVar22), iVar7 != 0) {
        lVar14 = lVar14 + -1;
        ppuVar22 = ppuVar22 + 5;
        if (lVar14 == 0) goto LAB_0011af8c;
      }
      local_4e0[lVar16] = (long)ppuVar22;
      lVar16 = lVar16 + 1;
    } while (lVar16 != param_3);
    do {
      cVar6 = DAT_00129ce8;
      cVar3 = '\x01';
      bVar4 = (bool)ExclusiveMonitorPass(0x129ce8,0x10);
      if (bVar4) {
        _DAT_00129ce8 = CONCAT31(DAT_00129ce8_1,1);
        cVar3 = ExclusiveMonitorsStatus();
      }
    } while (cVar3 != '\0' || cVar6 != '\0');
    FUN_0010f580();
    uVar8 = FUN_00113200();
    uVar9 = FUN_001187d8();
    ppuVar22 = &PTR_s_cameraEnabled_001216c0;
    do {
      ppuVar17 = ppuVar22;
      iVar7 = strcmp("aopPredictEnabled",*ppuVar17);
      ppuVar22 = ppuVar17 + 5;
    } while (iVar7 != 0);
    iVar7 = FUN_0011b604(uVar8,(&DAT_00129130)[*(int *)(ppuVar17 + 1)]);
    plVar15 = local_4e0;
    lVar16 = param_3;
    puVar18 = param_1;
    puVar21 = (uint *)(param_2 + 8);
    do {
      pcVar20 = (char *)*puVar18;
      lVar14 = *plVar15;
      uVar10 = FUN_001130c0(pcVar20);
      uVar11 = 0;
      if ((-1 < (int)uVar10) && (iVar7 != 0)) {
        uVar11 = uVar8 >> (ulong)(uVar10 & 0x1f) & 1;
      }
      uVar2 = (&DAT_00129130)[*(int *)(lVar14 + 8)];
      uVar13 = FUN_00112d44(pcVar20);
      if ((int)uVar13 < 0) {
        uVar10 = ~uVar10 >> 0x1f;
      }
      else {
        uVar11 = FUN_001189fc(uVar13,uVar9,0);
        pcVar20 = (char *)*puVar18;
        uVar10 = 1;
      }
      iVar12 = FUN_00112b5c(pcVar20);
      if (iVar12 != 0) {
        uVar11 = 0;
        if ((DAT_00129428 != 0) && (DAT_00129440 != (code *)0x0)) {
          iVar12 = (*DAT_00129440)();
          if ((iVar12 == 1) &&
             (((DAT_001293d0 == DAT_00129428 && (DAT_001293a8 == DAT_00129430)) &&
              (iVar12 = pthread_once((pthread_once_t *)&DAT_00129cd8,FUN_0011b480),
              DAT_00129ce0 != (code *)0x0)))) {
            iVar12 = (*DAT_00129ce0)(iVar12);
            uVar11 = (uint)(iVar12 == 1);
          }
          else {
            uVar11 = 0;
          }
        }
        pcVar20 = (char *)*puVar18;
        uVar10 = 1;
      }
      if ((pcVar20 != (char *)0x0) &&
         ((iVar12 = strcmp(pcVar20,"pinEnabled"), iVar12 == 0 ||
          (iVar12 = strcmp(pcVar20,"sprayEnabled"), iVar12 == 0)))) {
        uVar11 = 0;
        uVar10 = 1;
        if ((DAT_00129450 != 0) && (DAT_00129468 != (code *)0x0)) {
          iVar12 = (*DAT_00129468)();
          if (((iVar12 == 1) && (DAT_001293d0 == DAT_00129450)) &&
             ((DAT_001293a8 == DAT_00129458 &&
              (iVar12 = pthread_once((pthread_once_t *)&DAT_00129cd8,FUN_0011b480),
              DAT_00129ce0 != (code *)0x0)))) {
            iVar12 = (*DAT_00129ce0)(iVar12);
            uVar11 = (uint)(iVar12 == 1);
          }
          else {
            uVar11 = 0;
          }
        }
      }
      uVar13 = *puVar18;
      iVar12 = FUN_00112940(uVar13);
      if (iVar12 != 0) {
        uVar10 = 1;
        uVar11 = 0;
        if ((DAT_00129478 != 0) && (uVar11 = 0, DAT_00129490 != (code *)0x0)) {
          iVar12 = (*DAT_00129490)();
          if ((iVar12 == 1) && ((DAT_001293d0 == DAT_00129478 && (DAT_001293a8 == DAT_00129480)))) {
            iVar12 = FUN_0011b320(uVar13);
            if (iVar12 == 0) {
              iVar12 = pthread_once((pthread_once_t *)&DAT_00129cd8,FUN_0011b480);
              if (DAT_00129ce0 == (code *)0x0) goto LAB_0011afc0;
              iVar12 = (*DAT_00129ce0)(iVar12);
              uVar11 = (uint)(iVar12 == 1);
            }
            else {
              uVar11 = 1;
            }
          }
          else {
LAB_0011afc0:
            uVar11 = 0;
          }
        }
      }
      uVar1 = uVar2;
      if (uVar11 == 0 || uVar10 == 0) {
        uVar1 = 0;
      }
      if (*(int *)(lVar14 + 0xc) != 0) {
        uVar1 = uVar2;
      }
      if (uVar10 == 0) {
        if (*(int *)(lVar14 + 0x20) == 0) {
          uVar11 = (uint)(*(int *)(lVar14 + 0xc) != 0);
        }
        else {
          uVar11 = 3;
        }
      }
      else {
        uVar11 = (uint)(uVar11 != 0) << 1;
      }
      puVar18 = puVar18 + 1;
      puVar21[-2] = uVar2;
      puVar21[-1] = uVar1;
      *puVar21 = uVar11;
      lVar16 = lVar16 + -1;
      plVar15 = plVar15 + 1;
      puVar21 = puVar21 + 3;
    } while (lVar16 != 0);
    puVar19 = (undefined4 *)(param_2 + 8);
    do {
      iVar7 = FUN_0010f880(*param_1);
      if (iVar7 != 0) {
        puVar19[-1] = puVar19[-2];
        *puVar19 = 1;
      }
      uVar9 = DAT_001293c4;
      param_3 = param_3 + -1;
      puVar19 = puVar19 + 3;
      param_1 = param_1 + 1;
    } while (param_3 != 0);
    uVar13 = 1;
    *param_4 = DAT_00129348;
    *param_5 = uVar9;
    _DAT_00129ce8 = 0;
  }
LAB_0011af90:
  if (*(long *)(lVar5 + 0x28) == local_70) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar13);
}

/* ===== FUN_0015f874 @ 0015f874 [libNexusEvasionRuntime69252.so] ===== */

void FUN_0015f874(long param_1)

{
  uint uVar1;
  long lVar2;
  long *plVar3;
  int iVar4;
  undefined8 uVar5;
  ulong uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  float fVar9;
  float fVar10;
  float fVar11;
  long *local_248;
  code *pcStack_240;
  code *local_238;
  code *pcStack_230;
  long local_228;
  long lStack_220;
  long *local_218;
  uint local_210;
  int local_20c;
  undefined8 local_208;
  long local_200;
  undefined8 local_1f8;
  int local_1f0;
  undefined4 uStack_1ec;
  uint local_1e8;
  float local_1e4;
  float fStack_1e0;
  char local_1d8;
  char local_1d7;
  long *local_1c8;
  timespec local_1c0 [15];
  ulong local_d0;
  undefined1 auStack_c8 [8];
  char *local_c0;
  undefined *puStack_b8;
  uint local_b0;
  undefined4 local_a8;
  undefined4 uStack_a4;
  uint local_a0;
  uint local_9c;
  int local_94;
  float local_8c;
  float fStack_88;
  long local_78;
  
  lVar2 = tpidr_el0;
  local_78 = *(long *)(lVar2 + 0x28);
  if (param_1 != 0) {
    local_248 = (long *)CONCAT44(local_248._4_4_,0xffffffff);
    local_c0 = "autofarmEnabled";
    if (((((int)DAT_00214938 == 1) && (DAT_00214930 != (code *)0x0)) &&
        (iVar4 = (*DAT_00214930)(&local_c0,local_1c0,1,&local_1c8,&local_248), iVar4 == 1)) &&
       ((((local_1c8 != (long *)0x0 && ((int)local_248 == 0)) &&
         (((int)local_1c0[0].tv_nsec == 2 &&
          ((local_1c0[0].tv_sec._4_4_ == 1 && (iVar4 = FUN_0016944c(), iVar4 == 0)))))) &&
        (iVar4 = FUN_001695cc(), plVar3 = local_1c8, iVar4 != 0)))) {
      puStack_b8 = PTR_s_combatFireInterval_001c57f8;
      local_c0 = PTR_s_autofarmFollowTarget_001c57f0;
      if (((((DAT_00214930 != (code *)0x0) &&
            (iVar4 = (*DAT_00214930)(&local_c0,local_1c0,2,&local_248,&local_1f0), iVar4 == 1)) &&
           (local_248 == plVar3)) && ((local_1f0 == 0 && (-1 < (int)local_1c0[0].tv_sec)))) &&
         ((int)local_1c0[0].tv_sec < 2)) {
        uVar1 = local_1c0[0].tv_nsec._4_4_;
        if ((int)local_1c0[0].tv_nsec._4_4_ < 0x79) {
          uVar1 = 0x78;
        }
        if (0x1a3 < uVar1) {
          uVar1 = 0x1a4;
        }
        if (DAT_00228c40 != DAT_0020f690) {
          DAT_00228c48 = 0;
          DAT_00228c40 = (ulong)DAT_0020f690;
        }
        iVar4 = clock_gettime(1,local_1c0);
        if (iVar4 == 0) {
          uVar6 = CONCAT44(local_1c0[0].tv_sec._4_4_,(int)local_1c0[0].tv_sec) * 1000 +
                  CONCAT44(local_1c0[0].tv_nsec._4_4_,(int)local_1c0[0].tv_nsec) / 1000000;
        }
        else {
          uVar6 = 0;
        }
        if (((((uVar6 <= DAT_00228c48 - 1) || ((ulong)uVar1 <= uVar6 - DAT_00228c48)) &&
             ((DAT_00214cf8 == 0 &&
              (((DAT_00214c10 == 0 && DAT_00214c14 == 0 && (DAT_00214cf0 != DAT_0020f650)) &&
               (iVar4 = FUN_00168fe8(), iVar4 != 0)))))) &&
            ((((iVar4 = FUN_001bc828(DAT_0020f670,FUN_001428fc,0,&local_1d8), iVar4 != 0 &&
               (local_1d8 == '\0')) && (local_1d7 == '\0')) &&
             ((iVar4 = FUN_00155ad0(), iVar4 == 0 &&
              (iVar4 = FUN_00168bac(local_1c8,&local_1f0), iVar4 != 0)))))) &&
           ((iVar4 = FUN_00189c88(DAT_001e0978,CONCAT44(uStack_1ec,local_1f0),FUN_001428fc,0,
                                  &local_c0), iVar4 != 0 &&
            (((local_94 != 0 && (local_b0 == local_1e8)) &&
             (iVar4 = FUN_0017fba8(DAT_0020f698,DAT_0020f69c,local_a8,uStack_a4), iVar4 == 0)))))) {
          fVar11 = 1.0;
          if ((DAT_0020f6a4 != 0) && (fVar11 = 1.0, DAT_0020f6a0 <= DAT_0020f6a4)) {
            fVar11 = (float)DAT_0020f6a0 / (float)DAT_0020f6a4;
          }
          fVar10 = 1.0;
          if ((local_9c != 0) && (fVar10 = 1.0, local_a0 <= local_9c)) {
            fVar10 = (float)local_a0 / (float)local_9c;
          }
          uVar7 = FUN_00165c60(&DAT_0020f680);
          uVar8 = FUN_00165c60(&local_c0);
          fVar9 = hypotf(local_8c - DAT_0020f6b4,fStack_88 - DAT_0020f6b8);
          iVar4 = FUN_0019a848(fVar11,fVar10,uVar7,uVar8,fVar9,&DAT_00228748);
          if ((((((iVar4 != 0) && (DAT_0020f718 == DAT_0020f680)) &&
                (iVar4 = FUN_00165d8c(&DAT_0020f680,&local_1f8), iVar4 != 0)) &&
               ((uVar5 = FUN_0013a78c(DAT_0020f678,&local_200), (int)uVar5 != 0 &&
                (uVar5 = FUN_001428fc(uVar5,DAT_0020f678 + 0xc,&local_20c,4), (int)uVar5 != 0)))) &&
              (iVar4 = FUN_001428fc(uVar5,DAT_0020f678 + 0xe0,&local_210,4), iVar4 != 0)) &&
             ((0 < local_20c && (local_20c < 0x41)))) {
            if ((-1 < (int)local_210) &&
               ((((int)local_210 < local_20c &&
                 (iVar4 = FUN_0013a78c(local_200 + (ulong)local_210 * 8,&local_208), iVar4 != 0)) &&
                (iVar4 = (*(code *)(DAT_001e0978 + 0xecfb74))(local_1f8,local_208,DAT_0020f680),
                iVar4 != 0)))) {
              local_d0 = CONCAT44((int)(fStack_1e0 * 300.0),(int)(local_1e4 * 300.0));
              iVar4 = FUN_00169714(local_8c,fStack_88,1);
              if (((iVar4 != 0) && (uVar5 = FUN_00169714(local_1e4,fStack_1e0,1), (int)uVar5 != 0))
                 && (iVar4 = FUN_001428fc(uVar5,DAT_0020f670 + 0xfac,auStack_c8,8), iVar4 != 0)) {
                local_248 = &local_228;
                local_218 = local_1c8;
                pcStack_240 = FUN_001428fc;
                local_228 = DAT_0020f670;
                lStack_220 = DAT_0020f680;
                local_238 = FUN_00154ec4;
                pcStack_230 = FUN_00169a50;
                iVar4 = FUN_0018a084(&local_248,DAT_0020f670,auStack_c8,&local_d0);
                if (iVar4 == 1) {
                  iVar4 = FUN_00169a50(&local_228);
                  uVar7 = DAT_0020d158;
                  if (iVar4 != 0) {
                    DAT_00214cf8 = 4;
                    DAT_0020d158 = 1;
                    DAT_00214c10 = 1;
                    DAT_00228c48 = uVar6;
                    uVar6 = (*(code *)(DAT_001e0978 + 0xb2e994))(local_228,lStack_220);
                    DAT_00214cf8 = 0;
                    DAT_00214c10 = 0;
                    DAT_00214cf0 = DAT_0020f650;
                    DAT_0020d158 = uVar7;
                    snprintf((char *)local_1c0,0xf0,
                             ",\"target_gid\":%u,\"result\":%d,\"raw_x\":%d,\"raw_y\":%d",
                             (ulong)local_b0,uVar6 & 0xffffffff,local_d0 & 0xffffffff,
                             local_d0 >> 0x20);
                    FUN_001417c8("autofarm_fire","original_wrapper",local_1c0);
                  }
                }
                else if (iVar4 == -1) {
                  FUN_001417c8("fatal","farm_xy_restore_unverified",0);
                    /* WARNING: Subroutine does not return */
                  abort();
                }
              }
            }
          }
        }
      }
    }
  }
  if (*(long *)(lVar2 + 0x28) == local_78) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_001661a0 @ 001661a0 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_001661a0(long param_1)

{
  long lVar1;
  int iVar2;
  undefined8 uVar3;
  int local_64;
  long local_60;
  undefined1 auStack_58 [4];
  uint local_54;
  int local_50;
  uint local_48;
  int local_44;
  int local_3c;
  int local_30;
  long local_28;
  
  lVar1 = tpidr_el0;
  local_28 = *(long *)(lVar1 + 0x28);
  local_60 = 0;
  local_64 = -1;
  if ((int)_DAT_002208a0 == 1) {
    iVar2 = pthread_once((pthread_once_t *)&DAT_0021ca04,FUN_00164598);
    if (DAT_0021ca08 != (code *)0x0) {
      iVar2 = (*DAT_0021ca08)(iVar2);
      uVar3 = 0;
      if ((iVar2 != 1) || (DAT_002208a8 == (code *)0x0)) goto LAB_001662c0;
      iVar2 = (*DAT_002208a8)(&PTR_s_pinEnabled_001c4320,auStack_58,4,&local_60,&local_64);
      uVar3 = 0;
      if ((iVar2 != 1) || (((local_60 == 0 || (local_64 != 0)) || (local_50 != 2))))
      goto LAB_001662c0;
      if (local_54 < 2) {
        uVar3 = 0;
        if ((0x1324 < local_3c - 100U) || (local_44 != 2)) goto LAB_001662c0;
        if ((local_48 < 2) && (0xffffecda < local_30 - 0x1389U)) {
          *(long *)(param_1 + 0x10) = local_60;
          uVar3 = 1;
          *(uint *)(param_1 + 0x4c) = local_54;
          *(uint *)(param_1 + 0x50) = local_48;
          *(int *)(param_1 + 0x54) = local_3c;
          *(int *)(param_1 + 0x58) = local_30;
          goto LAB_001662c0;
        }
      }
    }
  }
  uVar3 = 0;
LAB_001662c0:
  if (*(long *)(lVar1 + 0x28) == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar3);
}

/* ===== FUN_001695cc @ 001695cc [libNexusEvasionRuntime69252.so] ===== */

void FUN_001695cc(void)

{
  long lVar1;
  bool bVar2;
  int iVar3;
  long local_60;
  int local_54;
  undefined1 auStack_50 [4];
  int local_4c;
  int local_48;
  char *local_40;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  local_40 = "autofarmEnabled";
  local_54 = -1;
  if (((((int)DAT_00214938 == 1) && (DAT_00214930 != (code *)0x0)) &&
      (iVar3 = (*DAT_00214930)(&local_40,auStack_50,1,&local_60,&local_54), iVar3 == 1)) &&
     (((local_60 != 0 && (local_54 == 0)) && ((local_48 == 2 && (local_4c == 1)))))) {
    local_54 = -1;
    local_40 = "autofarmAttackEnemies";
    if (((((int)DAT_00214938 == 1) && (DAT_00214930 != (code *)0x0)) &&
        ((iVar3 = (*DAT_00214930)(&local_40,auStack_50,1,&local_60,&local_54), iVar3 == 1 &&
         (((local_60 != 0 && (local_54 == 0)) && (local_48 == 2)))))) && (local_4c == 1)) {
      iVar3 = FUN_001550fc();
      bVar2 = iVar3 != 0;
      goto LAB_001696ec;
    }
  }
  bVar2 = false;
LAB_001696ec:
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(bVar2);
}

/* ===== FUN_00169a50 @ 00169a50 [libNexusEvasionRuntime69252.so] ===== */

void FUN_00169a50(undefined8 *param_1)

{
  long lVar1;
  int iVar2;
  ulong uVar3;
  long local_50;
  int local_44;
  undefined1 auStack_40 [4];
  int local_3c;
  int local_38;
  char *local_30;
  long local_28;
  
  lVar1 = tpidr_el0;
  local_28 = *(long *)(lVar1 + 0x28);
  uVar3 = FUN_001695cc();
  if ((int)uVar3 != 0) {
    local_44 = -1;
    local_30 = "autofarmEnabled";
    if ((((((int)DAT_00214938 == 1) && (DAT_00214930 != (code *)0x0)) &&
         (iVar2 = (*DAT_00214930)(&local_30,auStack_40,1,&local_50,&local_44), iVar2 == 1)) &&
        ((local_50 != 0 && (local_44 == 0)))) &&
       ((local_38 == 2 && ((local_3c == 1 && (local_50 == param_1[2])))))) {
      iVar2 = FUN_00150bf0(*param_1,param_1[1]);
      uVar3 = (ulong)(iVar2 != 0);
    }
    else {
      uVar3 = 0;
    }
  }
  if (*(long *)(lVar1 + 0x28) == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar3);
}

/* ===== FUN_00172f00 @ 00172f00 [libNexusEvasionRuntime69252.so] ===== */

undefined4 FUN_00172f00(uint param_1)

{
  code *pcVar1;
  code *pcVar2;
  code *pcVar3;
  long lVar4;
  int iVar5;
  undefined8 uVar6;
  long lVar7;
  char *pcVar8;
  undefined4 uVar9;
  char *pcVar10;
  code *local_b8;
  long local_b0;
  ulong local_a8 [12];
  long local_48;
  
  lVar4 = tpidr_el0;
  local_48 = *(long *)(lVar4 + 0x28);
  if ((2 < param_1 - 1) && (DAT_0020ad98 != (code *)0x0)) {
    uVar6 = (*DAT_0020ad98)("autofarmEnabled");
    lVar7 = DAT_001e0978;
    if ((int)uVar6 == 1) {
      local_b0 = 0;
      local_a8[0] = 0;
      local_b8 = (code *)0x0;
      pcVar3 = (code *)(DAT_001e0978 + 0x11a2840);
      if ((((((pcVar3 == (code *)0x0) ||
             (pcVar1 = (code *)(DAT_001e0978 + 0xf41ba0), pcVar1 == (code *)0x0)) ||
            (pcVar2 = (code *)(DAT_001e0978 + 0xac319c), pcVar2 == (code *)0x0)) ||
           ((uVar6 = FUN_001428fc(uVar6,DAT_001e0978 + 0x13053c0,local_a8,8), (int)uVar6 == 0 ||
            (local_a8[0] < 0x1000)))) ||
          (((local_a8[0] & 7) != 0 ||
           ((uVar6 = FUN_001428fc(uVar6,local_a8[0],&local_b0,8), (int)uVar6 == 0 ||
            (local_b0 != lVar7 + 0x11ebcd0)))))) ||
         ((iVar5 = FUN_001428fc(uVar6,local_b0 + 0x18,&local_b8,8), iVar5 == 0 ||
          ((local_b8 != pcVar2 || (lVar7 = (*pcVar3)(0x98), lVar7 == 0)))))) {
        uVar9 = 0;
        pcVar10 = "native_send_failed";
        pcVar8 = "false";
      }
      else {
        uVar9 = 1;
        (*pcVar1)(lVar7,1,0);
        (*pcVar2)(local_a8[0],lVar7);
        pcVar10 = "play_again_status";
        pcVar8 = "true";
      }
      snprintf((char *)local_a8,0x60,",\"status\":%d,\"sent\":%s",(ulong)param_1,pcVar8);
      FUN_001417c8("autofarm_play_again",pcVar10,local_a8);
      goto LAB_00173048;
    }
  }
  uVar9 = 0;
LAB_00173048:
  if (*(long *)(lVar4 + 0x28) == local_48) {
    return uVar9;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0018ded4 @ 0018ded4 [libNexusUI69252.so] ===== */

bool FUN_0018ded4(char *param_1)

{
  bool bVar1;
  int iVar2;
  
  iVar2 = strncmp(param_1,"nexus_quick_menu_",0x11);
  if ((((iVar2 == 0) || (iVar2 = strcmp(param_1,"nexus_autofarm_post_delay_ms"), iVar2 == 0)) ||
      (iVar2 = strcmp(param_1,"nexus_autofarm_click_gap_ms"), iVar2 == 0)) ||
     ((iVar2 = strcmp(param_1,"nexus_auto_play_again_enabled"), iVar2 == 0 ||
      (iVar2 = strcmp(param_1,"nexus_autofarm_show_stats"), iVar2 == 0)))) {
    bVar1 = true;
  }
  else {
    iVar2 = strcmp(param_1,"nexus_sx_outline_color_preset");
    bVar1 = iVar2 == 0;
  }
  return bVar1;
}

