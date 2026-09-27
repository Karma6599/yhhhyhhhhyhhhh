/*
 * Auto Dodge — Feature
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
 * Notes: Projectile evasion with tunable reaction/safety/power/distance/commit/burst params + per-attack blacklist mask (combatDodgeBlacklistMask).
 */

/* ===== FUN_0010ec90 @ 0010ec90 [libNexusEvasion69252.so] ===== */

/* WARNING: Type propagation algorithm not settling */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_0010ec90(char *param_1,uint param_2,uint param_3)

{
  char cVar1;
  bool bVar2;
  char cVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  undefined4 uVar7;
  long lVar8;
  uint *puVar9;
  ulong uVar10;
  uint uVar11;
  
  if (param_1 == (char *)0x0) {
    return 0xffffffff;
  }
  lVar8 = 0;
  while (param_1[lVar8] != '\0') {
    lVar8 = lVar8 + 1;
    if (lVar8 == 0x60) {
      return 0xffffffff;
    }
  }
  if (lVar8 == 0x60) {
    return 0xffffffff;
  }
  uVar10 = 0xffffffffffffffd7;
  puVar9 = &DAT_001216a8;
  while (iVar4 = strcmp(param_1,*(char **)(puVar9 + -4)), iVar4 != 0) {
    puVar9 = puVar9 + 10;
    uVar10 = uVar10 + 1;
    if (uVar10 == 0x65) {
      return 0xffffffff;
    }
  }
  if (puVar9[-1] != param_3) {
    return 0xffffffff;
  }
  iVar4 = strcmp(param_1,"combatDodgeRequireHold");
  uVar11 = 0;
  if (iVar4 != 0) {
    uVar11 = param_2;
  }
  if (uVar11 != 0 || param_3 != 0) {
    if (param_3 == 0) {
      iVar4 = FUN_0011b320(param_1);
      if (iVar4 == 0) goto LAB_0010ef98;
    }
    else {
      iVar4 = strcmp(param_1,"outlineOpacity");
      if (((((iVar4 != 0) && (iVar4 = strcmp(param_1,"outlineColorR"), iVar4 != 0)) &&
           (iVar4 = strcmp(param_1,"outlineColorG"), iVar4 != 0)) &&
          ((iVar4 = strcmp(param_1,"outlineColorB"), iVar4 != 0 &&
           (iVar4 = strncmp(param_1,"aop",3), iVar4 != 0)))) &&
         ((((iVar4 = strncmp(param_1,"killaura",8), iVar4 != 0 &&
            ((iVar4 = strncmp(param_1,"combatAura",10), iVar4 != 0 &&
             (iVar4 = strncmp(param_1,"combatFire",10), iVar4 != 0)))) &&
           (iVar4 = strncmp(param_1,"combatDodge",0xb), iVar4 != 0)) &&
          ((((iVar4 = strncmp(param_1,"dodge",5), iVar4 != 0 &&
             (iVar4 = strncmp(param_1,"v2Dodge",7), iVar4 != 0)) &&
            (iVar4 = strncmp(param_1,"v3Dodge",7), iVar4 != 0)) &&
           (((iVar4 = strncmp(param_1,"v4Dodge",7), iVar4 != 0 &&
             (iVar4 = strncmp(param_1,"v5Dodge",7), iVar4 != 0)) &&
            (iVar4 = strncmp(param_1,"holdToShoot",0xb), iVar4 != 0)))))))) {
LAB_0010ef98:
        iVar4 = pthread_once((pthread_once_t *)&DAT_00129cd8,FUN_0011b480);
        if ((DAT_00129ce0 == (code *)0x0) || (iVar4 = (*DAT_00129ce0)(iVar4), iVar4 != 1)) {
          return 2;
        }
      }
    }
  }
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
  if ((param_3 == 0) && (uVar11 != 0)) {
    iVar4 = FUN_0010f880(param_1);
    if (iVar4 == 0) {
      iVar4 = FUN_00112940(param_1);
      if (iVar4 == 0) {
        iVar4 = strcmp(param_1,"pinEnabled");
        if ((iVar4 == 0) || (iVar4 = strcmp(param_1,"sprayEnabled"), iVar4 == 0)) {
          if (DAT_00129450 == 0) {
            _DAT_00129ce8 = 0;
            return 2;
          }
          if (DAT_00129468 == (code *)0x0) {
            _DAT_00129ce8 = 0;
            return 2;
          }
          iVar4 = (*DAT_00129468)(2);
          if (iVar4 != 1) {
            _DAT_00129ce8 = 0;
            return 2;
          }
          if (DAT_001293d0 != DAT_00129450) {
            _DAT_00129ce8 = 0;
            return 2;
          }
          if (DAT_001293a8 != DAT_00129458) {
            _DAT_00129ce8 = 0;
            return 2;
          }
LAB_0010f0b4:
          iVar4 = pthread_once((pthread_once_t *)&DAT_00129cd8,FUN_0011b480);
          if (DAT_00129ce0 == (code *)0x0) {
            _DAT_00129ce8 = 0;
            return 2;
          }
          iVar4 = (*DAT_00129ce0)(iVar4);
          uVar5 = (uint)(iVar4 == 1);
        }
        else {
          iVar4 = FUN_00112b5c(param_1);
          if (iVar4 != 0) {
            if (DAT_00129428 == 0) {
              _DAT_00129ce8 = 0;
              return 2;
            }
            if (DAT_00129440 == (code *)0x0) {
              _DAT_00129ce8 = 0;
              return 2;
            }
            iVar4 = (*DAT_00129440)(2);
            if (iVar4 != 1) {
              _DAT_00129ce8 = 0;
              return 2;
            }
            if (DAT_001293d0 != DAT_00129428) {
              _DAT_00129ce8 = 0;
              return 2;
            }
            if (DAT_001293a8 != DAT_00129430) {
              _DAT_00129ce8 = 0;
              return 2;
            }
            goto LAB_0010f0b4;
          }
          iVar4 = strcmp(param_1,"speedExploitEnabled");
          if ((iVar4 == 0) || (iVar4 = strcmp(param_1,"speedLocalMoveEnabled"), iVar4 == 0))
          goto LAB_0010f1a4;
          iVar4 = strcmp(param_1,"holdToShootEnabled");
          if ((iVar4 == 0) ||
             ((iVar4 = strcmp(param_1,"holdToShootAim"), iVar4 == 0 ||
              (iVar4 = strcmp(param_1,"holdToShootRangeCheck"), iVar4 == 0)))) {
            iVar4 = FUN_00117314(param_1);
            uVar5 = (uint)(iVar4 != 0);
          }
          else {
            iVar4 = strcmp(param_1,"isSpinEnabled");
            if (iVar4 == 0) {
              uVar5 = FUN_001158a0();
            }
            else {
              iVar4 = FUN_00112d44(param_1);
              if (iVar4 < 0) {
                uVar5 = FUN_001130c0(param_1);
                uVar6 = FUN_00113200();
                lVar8 = FUN_0010f504("aopPredictEnabled");
                if ((int)uVar5 < 0) {
                  lVar8 = FUN_0010f504(param_1,(&DAT_00129130)[*(int *)(lVar8 + 8)] != 0 ||
                                               uVar5 == 2);
                  if (lVar8 == 0) {
                    _DAT_00129ce8 = 0;
                    return 2;
                  }
                  if (*(int *)(lVar8 + 0xc) != 0) {
                    _DAT_00129ce8 = 0;
                    return 2;
                  }
                  uVar5 = (uint)(*(int *)(lVar8 + 0x20) == 0);
                }
                else {
                  if ((uVar6 >> (ulong)(uVar5 & 0x1f) & 1) == 0) {
                    _DAT_00129ce8 = 0;
                    return 2;
                  }
                  iVar4 = FUN_0011b604(uVar6);
                  uVar5 = (uint)(iVar4 != 0);
                }
              }
              else {
                uVar5 = FUN_0011b5d0(param_1,1);
              }
            }
          }
        }
        if (uVar5 == 0) {
          _DAT_00129ce8 = 0;
          return 2;
        }
      }
      else {
        if (DAT_00129478 == 0) {
          _DAT_00129ce8 = 0;
          return 2;
        }
        if (DAT_00129490 == (code *)0x0) {
          _DAT_00129ce8 = 0;
          return 2;
        }
        iVar4 = (*DAT_00129490)(2);
        if (iVar4 != 1) {
          _DAT_00129ce8 = 0;
          return 2;
        }
        if (DAT_001293d0 != DAT_00129478) {
          _DAT_00129ce8 = 0;
          return 2;
        }
        if (DAT_001293a8 != DAT_00129480) {
          _DAT_00129ce8 = 0;
          return 2;
        }
        iVar4 = FUN_0011b320(param_1);
        if (iVar4 == 0) goto LAB_0010f0b4;
      }
    }
  }
  else if (param_3 != 0) {
    uVar5 = uVar11;
    if ((int)puVar9[1] <= (int)uVar11) {
      uVar5 = puVar9[1];
    }
    uVar6 = *puVar9;
    if ((int)*puVar9 <= (int)uVar11) {
      uVar6 = uVar5;
    }
    goto LAB_0010f1ac;
  }
LAB_0010f1a4:
  uVar6 = (uint)(uVar11 != 0);
LAB_0010f1ac:
  uVar5 = uVar11;
  if (4 < uVar11 - 1) {
    uVar5 = 3;
  }
  if (uVar10 != 0x5c) {
    uVar5 = uVar6;
  }
  uVar6 = 1;
  if (uVar11 == 2) {
    uVar6 = 2;
  }
  if (2 < uVar10 - 0x5e) {
    uVar6 = uVar5;
  }
  uVar11 = 4;
  if (uVar10 != 0x5d) {
    uVar11 = uVar6;
  }
  if (uVar10 < 5) {
    iVar4 = strcmp(param_1,"followModeEnemy");
    if ((iVar4 == 0) || (iVar4 = strcmp(param_1,"followModeTeam"), iVar4 == 0)) {
      uVar11 = 1;
    }
    else {
      lVar8 = -0x1630;
      do {
        iVar4 = strcmp("followModeTeam",*(char **)((long)&PTR_s_aopAimEnabled_00122cc8 + lVar8));
        if (iVar4 == 0) {
          if ((&DAT_00129130)[*(int *)((long)&PTR_s_aopPredictEnabled_00122cd0 + lVar8)] !=
              (uint)(uVar11 != 0)) {
            (&DAT_00129130)[*(int *)((long)&PTR_s_aopPredictEnabled_00122cd0 + lVar8)] =
                 (uint)(uVar11 != 0);
            DAT_00129348 = DAT_00129348 + 1;
          }
          break;
        }
        lVar8 = lVar8 + 0x28;
      } while (lVar8 != 0);
    }
  }
  iVar4 = strcmp(param_1,"speedExploitEnabled");
  if ((iVar4 == 0) && (uVar11 != 0)) {
    lVar8 = -0x1630;
    do {
      iVar4 = strcmp("speedLocalMoveEnabled",*(char **)((long)&PTR_s_aopAimEnabled_00122cc8 + lVar8)
                    );
      if (iVar4 == 0) {
        if ((&DAT_00129130)[*(int *)((long)&PTR_s_aopPredictEnabled_00122cd0 + lVar8)] != 0) {
          (&DAT_00129130)[*(int *)((long)&PTR_s_aopPredictEnabled_00122cd0 + lVar8)] = 0;
          DAT_00129348 = DAT_00129348 + 1;
        }
        break;
      }
      lVar8 = lVar8 + 0x28;
    } while (lVar8 != 0);
  }
  iVar4 = strcmp(param_1,"speedLocalMoveEnabled");
  if ((iVar4 == 0) && (uVar11 != 0)) {
    lVar8 = -0x1630;
    do {
      iVar4 = strcmp("speedExploitEnabled",*(char **)((long)&PTR_s_aopAimEnabled_00122cc8 + lVar8));
      if (iVar4 == 0) {
        if ((&DAT_00129130)[*(int *)((long)&PTR_s_aopPredictEnabled_00122cd0 + lVar8)] != 0) {
          (&DAT_00129130)[*(int *)((long)&PTR_s_aopPredictEnabled_00122cd0 + lVar8)] = 0;
          DAT_00129348 = DAT_00129348 + 1;
        }
        break;
      }
      lVar8 = lVar8 + 0x28;
    } while (lVar8 != 0);
  }
  if ((&DAT_00129130)[(int)puVar9[-2]] != uVar11) {
    (&DAT_00129130)[(int)puVar9[-2]] = uVar11;
    DAT_00129348 = DAT_00129348 + 1;
  }
  if ((param_3 == 0) || (puVar9[4] != 0)) {
    uVar7 = 1;
    if (uVar11 != 0) {
      uVar7 = 2;
    }
  }
  else {
    uVar7 = 1;
  }
  if (param_3 == 0) {
    uVar7 = 1;
  }
  _DAT_00129ce8 = 0;
  return uVar7;
}

/* ===== nexus_evasion_functions_snapshot_v1 @ 00117d08 [libNexusEvasion69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void nexus_evasion_functions_snapshot_v1(int *param_1)

{
  char *pcVar1;
  long lVar2;
  char cVar3;
  long lVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  char cVar9;
  bool bVar10;
  bool bVar11;
  bool bVar12;
  bool bVar13;
  bool bVar14;
  bool bVar15;
  bool bVar16;
  bool bVar17;
  bool bVar18;
  bool bVar19;
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
  int iVar43;
  int iVar44;
  int iVar45;
  int iVar46;
  int iVar47;
  int iVar48;
  int iVar49;
  int iVar50;
  int iVar51;
  int iVar52;
  int iVar53;
  int iVar54;
  int iVar55;
  int iVar56;
  uint uVar57;
  undefined **ppuVar58;
  undefined **ppuVar59;
  long lVar60;
  long lVar61;
  char local_b0;
  undefined5 uStack_af;
  undefined2 uStack_aa;
  undefined6 uStack_a8;
  long local_70;
  
  lVar4 = tpidr_el0;
  local_70 = *(long *)(lVar4 + 0x28);
  if (param_1 != (int *)0x0) {
    if ((*param_1 == 1) && (param_1[1] == 0xb8)) {
      do {
        cVar9 = DAT_00129ce8;
        cVar3 = '\x01';
        bVar10 = (bool)ExclusiveMonitorPass(0x129ce8,0x10);
        if (bVar10) {
          _DAT_00129ce8 = CONCAT31(DAT_00129ce8_1,1);
          cVar3 = ExclusiveMonitorsStatus();
        }
      } while (cVar3 != '\0' || cVar9 != '\0');
      FUN_0010f580();
      iVar20 = FUN_001187d8();
      iVar21 = FUN_001189fc(0,iVar20,0);
      iVar22 = FUN_001189fc(1,iVar20,0);
      iVar23 = FUN_001189fc(2,iVar20,0);
      iVar24 = FUN_001189fc(3,iVar20,0);
      iVar25 = FUN_001189fc(4,iVar20,0);
      iVar26 = FUN_001189fc(5,iVar20,0);
      iVar27 = FUN_001189fc(6,iVar20,0);
      iVar28 = FUN_001189fc(7,iVar20,0);
      iVar29 = FUN_001189fc(8,iVar20,0);
      iVar30 = FUN_001189fc(9,iVar20,0);
      uVar8 = DAT_00129cc0;
      uVar7 = DAT_00129c38;
      iVar56 = DAT_001293c4;
      uVar6 = DAT_00129348;
      bVar10 = DAT_00129c50 != 0;
      bVar11 = DAT_00129c58 != 0;
      bVar12 = DAT_00129c60 != 0;
      bVar13 = DAT_00129c68 != 0;
      bVar14 = DAT_00129c70 != 0;
      bVar15 = DAT_00129c78 != 0;
      bVar16 = DAT_00129c80 != 0;
      bVar17 = DAT_00129c88 != 0;
      bVar18 = DAT_00129c90 != 0;
      bVar19 = DAT_00129c98 != 0;
      ppuVar59 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar58 = ppuVar59;
        iVar31 = strcmp("aopAimEnabled",*ppuVar58);
        ppuVar59 = ppuVar58 + 5;
      } while (iVar31 != 0);
      iVar31 = (&DAT_00129130)[*(int *)(ppuVar58 + 1)];
      ppuVar59 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar58 = ppuVar59;
        iVar32 = strcmp("aopPredictEnabled",*ppuVar58);
        ppuVar59 = ppuVar58 + 5;
      } while (iVar32 != 0);
      iVar32 = (&DAT_00129130)[*(int *)(ppuVar58 + 1)];
      ppuVar59 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar58 = ppuVar59;
        iVar33 = strcmp("aopPredictVersion",*ppuVar58);
        ppuVar59 = ppuVar58 + 5;
      } while (iVar33 != 0);
      iVar33 = (&DAT_00129130)[*(int *)(ppuVar58 + 1)];
      ppuVar59 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar58 = ppuVar59;
        iVar34 = strcmp("aopAimTargetMode",*ppuVar58);
        ppuVar59 = ppuVar58 + 5;
      } while (iVar34 != 0);
      iVar34 = (&DAT_00129130)[*(int *)(ppuVar58 + 1)];
      ppuVar59 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar58 = ppuVar59;
        iVar35 = strcmp("aopTargetMode",*ppuVar58);
        ppuVar59 = ppuVar58 + 5;
      } while (iVar35 != 0);
      iVar35 = (&DAT_00129130)[*(int *)(ppuVar58 + 1)];
      ppuVar59 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar58 = ppuVar59;
        iVar36 = strcmp("aopMaxRange",*ppuVar58);
        ppuVar59 = ppuVar58 + 5;
      } while (iVar36 != 0);
      iVar36 = (&DAT_00129130)[*(int *)(ppuVar58 + 1)];
      ppuVar59 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar58 = ppuVar59;
        iVar37 = strcmp("aopProjectileSpeed",*ppuVar58);
        ppuVar59 = ppuVar58 + 5;
      } while (iVar37 != 0);
      iVar37 = (&DAT_00129130)[*(int *)(ppuVar58 + 1)];
      ppuVar59 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar58 = ppuVar59;
        iVar38 = strcmp("aopLeadScale",*ppuVar58);
        ppuVar59 = ppuVar58 + 5;
      } while (iVar38 != 0);
      iVar38 = (&DAT_00129130)[*(int *)(ppuVar58 + 1)];
      ppuVar59 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar58 = ppuVar59;
        iVar39 = strcmp("aopReactionMs",*ppuVar58);
        ppuVar59 = ppuVar58 + 5;
      } while (iVar39 != 0);
      iVar39 = (&DAT_00129130)[*(int *)(ppuVar58 + 1)];
      ppuVar59 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar58 = ppuVar59;
        iVar40 = strcmp("aopAimUltimate",*ppuVar58);
        ppuVar59 = ppuVar58 + 5;
      } while (iVar40 != 0);
      iVar40 = (&DAT_00129130)[*(int *)(ppuVar58 + 1)];
      ppuVar59 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar58 = ppuVar59;
        iVar41 = strcmp("aopAimGadget",*ppuVar58);
        ppuVar59 = ppuVar58 + 5;
      } while (iVar41 != 0);
      iVar41 = (&DAT_00129130)[*(int *)(ppuVar58 + 1)];
      ppuVar59 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar58 = ppuVar59;
        iVar42 = strcmp("killauraEnabled",*ppuVar58);
        ppuVar59 = ppuVar58 + 5;
      } while (iVar42 != 0);
      iVar42 = (&DAT_00129130)[*(int *)(ppuVar58 + 1)];
      ppuVar59 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar58 = ppuVar59;
        iVar43 = strcmp("killauraMainAttack",*ppuVar58);
        ppuVar59 = ppuVar58 + 5;
      } while (iVar43 != 0);
      iVar43 = (&DAT_00129130)[*(int *)(ppuVar58 + 1)];
      ppuVar59 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar58 = ppuVar59;
        iVar44 = strcmp("holdToShootEnabled",*ppuVar58);
        ppuVar59 = ppuVar58 + 5;
      } while (iVar44 != 0);
      iVar44 = (&DAT_00129130)[*(int *)(ppuVar58 + 1)];
      ppuVar59 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar58 = ppuVar59;
        iVar45 = strcmp("autofarmEnabled",*ppuVar58);
        ppuVar59 = ppuVar58 + 5;
      } while (iVar45 != 0);
      iVar45 = (&DAT_00129130)[*(int *)(ppuVar58 + 1)];
      ppuVar59 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar58 = ppuVar59;
        iVar46 = strcmp("xrayEnabled",*ppuVar58);
        ppuVar59 = ppuVar58 + 5;
      } while (iVar46 != 0);
      iVar46 = (&DAT_00129130)[*(int *)(ppuVar58 + 1)];
      ppuVar59 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar58 = ppuVar59;
        iVar47 = strcmp("coltModEnabled",*ppuVar58);
        ppuVar59 = ppuVar58 + 5;
      } while (iVar47 != 0);
      iVar47 = (&DAT_00129130)[*(int *)(ppuVar58 + 1)];
      ppuVar59 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar58 = ppuVar59;
        iVar48 = strcmp("ballAssistEnabled",*ppuVar58);
        ppuVar59 = ppuVar58 + 5;
      } while (iVar48 != 0);
      iVar48 = (&DAT_00129130)[*(int *)(ppuVar58 + 1)];
      ppuVar59 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar58 = ppuVar59;
        iVar49 = strcmp("boltModEnabled",*ppuVar58);
        ppuVar59 = ppuVar58 + 5;
      } while (iVar49 != 0);
      iVar49 = (&DAT_00129130)[*(int *)(ppuVar58 + 1)];
      ppuVar59 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar58 = ppuVar59;
        iVar50 = strcmp("autododgeEnabled",*ppuVar58);
        ppuVar59 = ppuVar58 + 5;
      } while (iVar50 != 0);
      if ((&DAT_00129130)[*(int *)(ppuVar58 + 1)] == 0) {
        ppuVar59 = &PTR_s_cameraEnabled_001216c0;
        do {
          ppuVar58 = ppuVar59;
          iVar50 = strcmp("kitNaniModEnabled",*ppuVar58);
          ppuVar59 = ppuVar58 + 5;
        } while (iVar50 != 0);
        uVar57 = (uint)((&DAT_00129130)[*(int *)(ppuVar58 + 1)] != 0);
      }
      else {
        uVar57 = 1;
      }
      ppuVar59 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar58 = ppuVar59;
        iVar50 = strcmp("dodgeVersion",*ppuVar58);
        ppuVar59 = ppuVar58 + 5;
      } while (iVar50 != 0);
      iVar50 = (&DAT_00129130)[*(int *)(ppuVar58 + 1)];
      ppuVar59 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar58 = ppuVar59;
        iVar51 = strcmp("dodgeBlacklistMask",*ppuVar58);
        ppuVar59 = ppuVar58 + 5;
      } while (iVar51 != 0);
      iVar51 = (&DAT_00129130)[*(int *)(ppuVar58 + 1)];
      ppuVar59 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar58 = ppuVar59;
        iVar52 = strcmp("followEnabled",*ppuVar58);
        ppuVar59 = ppuVar58 + 5;
      } while (iVar52 != 0);
      iVar52 = (&DAT_00129130)[*(int *)(ppuVar58 + 1)];
      ppuVar59 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar58 = ppuVar59;
        iVar53 = strcmp("followDistance",*ppuVar58);
        ppuVar59 = ppuVar58 + 5;
      } while (iVar53 != 0);
      iVar53 = (&DAT_00129130)[*(int *)(ppuVar58 + 1)];
      ppuVar59 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar58 = ppuVar59;
        iVar54 = strcmp("kitNaniModEnabled",*ppuVar58);
        ppuVar59 = ppuVar58 + 5;
      } while (iVar54 != 0);
      iVar54 = (&DAT_00129130)[*(int *)(ppuVar58 + 1)];
      ppuVar59 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar58 = ppuVar59;
        iVar55 = strcmp("speedExploitEnabled",*ppuVar58);
        ppuVar59 = ppuVar58 + 5;
      } while (iVar55 != 0);
      iVar55 = (&DAT_00129130)[*(int *)(ppuVar58 + 1)];
      param_1[0xc] = iVar31;
      param_1[0xd] = iVar32;
      uVar5 = DAT_001047d0;
      *(undefined8 *)(param_1 + 2) = uVar6;
      *(undefined8 *)(param_1 + 4) = uVar7;
      *(undefined8 *)(param_1 + 6) = uVar8;
      param_1[0xe] = iVar33;
      param_1[0xf] = iVar34;
      param_1[8] = (uint)bVar10 | (uint)bVar11 << 1 | (uint)bVar12 << 2 | (uint)bVar13 << 3 |
                   (uint)bVar14 << 4 | (uint)bVar15 << 5 | (uint)bVar16 << 6 | (uint)bVar17 << 7 |
                   (uint)bVar18 << 8 | (uint)bVar19 << 9;
      param_1[9] = iVar20;
      param_1[10] = (uint)(iVar30 != 0) << 9 |
                    (uint)(iVar21 != 0) | (uint)(iVar22 != 0) << 1 | (uint)(iVar23 != 0) << 2 |
                    (uint)(iVar24 != 0) << 3 | (uint)(iVar25 != 0) << 4 | (uint)(iVar26 != 0) << 5 |
                    (uint)(iVar27 != 0) << 6 | (uint)(iVar28 != 0) << 7 | (uint)(iVar29 != 0) << 8;
      param_1[0xb] = iVar56;
      *(undefined8 *)param_1 = uVar5;
      param_1[0x10] = iVar35;
      param_1[0x11] = iVar36;
      param_1[0x23] = 0;
      param_1[0x24] = 0;
      param_1[0x21] = 0;
      param_1[0x22] = 0;
      param_1[0x25] = 0;
      param_1[0x26] = 0;
      param_1[0x12] = iVar37;
      param_1[0x13] = iVar38;
      param_1[0x14] = iVar39;
      param_1[0x15] = iVar40;
      param_1[0x16] = iVar41;
      param_1[0x17] = iVar42;
      param_1[0x18] = iVar43;
      param_1[0x19] = iVar44;
      param_1[0x1a] = iVar45;
      param_1[0x1b] = iVar46;
      param_1[0x1c] = iVar47;
      param_1[0x1d] = iVar48;
      param_1[0x1e] = iVar49;
      param_1[0x1f] = uVar57;
      param_1[0x20] = iVar50;
      param_1[0x27] = 0;
      param_1[0x28] = iVar51;
      param_1[0x29] = iVar52;
      param_1[0x2a] = iVar53;
      param_1[0x2b] = iVar54;
      param_1[0x2c] = iVar55;
      lVar61 = 0;
      do {
        if (param_1[0x20] == 1) {
          snprintf(&local_b0,0x40,"dodge%s",(&PTR_s_ReactionPercent_00121668)[lVar61]);
        }
        else {
          snprintf(&local_b0,0x40,"v%dDodge%s");
        }
        if (local_b0 != '\0') {
          lVar60 = 1;
          do {
            pcVar1 = &local_b0 + lVar60;
            lVar60 = lVar60 + 1;
          } while (*pcVar1 != '\0');
        }
        lVar60 = 0;
        if (CONCAT26(uStack_aa,CONCAT51(uStack_af,local_b0)) != 0x6e456e6970537369 ||
            CONCAT62(uStack_a8,uStack_aa) != 0x64656c62616e45) {
          ppuVar59 = &PTR_s_cameraEnabled_001216c0;
          do {
            lVar60 = lVar60 + 1;
            iVar56 = strcmp(&local_b0,*ppuVar59);
            ppuVar59 = ppuVar59 + 5;
          } while (iVar56 != 0);
        }
        lVar2 = lVar61 + 1;
        param_1[lVar61 + 0x22] = (&DAT_00129130)[(int)(&DAT_001216a0)[lVar60 * 10]];
        lVar61 = lVar2;
      } while (lVar2 != 6);
      param_1 = (int *)0x1;
      _DAT_00129ce8 = 0;
    }
    else {
      param_1 = (int *)0x0;
    }
  }
  if (*(long *)(lVar4 + 0x28) == local_70) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(param_1);
}

/* ===== FUN_00118fe0 @ 00118fe0 [libNexusEvasion69252.so] ===== */

void FUN_00118fe0(int param_1,int param_2,undefined8 *param_3)

{
  bool bVar1;
  long lVar2;
  bool bVar3;
  bool bVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  undefined4 uVar10;
  int iVar11;
  undefined8 uVar12;
  long lVar13;
  uint uVar14;
  undefined **ppuVar15;
  char *pcVar16;
  char *pcVar17;
  undefined **ppuVar18;
  char *pcVar19;
  char *pcVar20;
  undefined8 local_90;
  undefined8 uStack_88;
  long lStack_80;
  long local_78;
  undefined8 uStack_70;
  long local_68;
  
  lVar2 = tpidr_el0;
  local_68 = *(long *)(lVar2 + 0x28);
  uVar12 = 0;
  if (param_1 < 0x79) {
    bVar4 = false;
    pcVar16 = "autododgeEnabled";
    uVar14 = 2;
    bVar3 = true;
    pcVar19 = "boltModEnabled";
    pcVar17 = "autododgeEnabled";
    pcVar20 = "boltModEnabled";
    switch(param_1) {
    case 0x16:
      break;
    case 0x17:
      bVar3 = false;
      bVar4 = true;
      uVar14 = 0;
      pcVar17 = pcVar16;
      pcVar20 = pcVar19;
      if ((param_2 != 1) && (uVar12 = 0, param_2 != 4)) goto switchD_00119060_caseD_18;
      break;
    default:
      goto switchD_00119060_caseD_18;
    case 0x1a:
      bVar4 = false;
      bVar3 = false;
      uVar14 = 4;
      pcVar17 = pcVar16;
      pcVar20 = "followEnabled";
      break;
    case 0x2e:
      bVar4 = false;
      bVar3 = false;
      uVar14 = 9;
      pcVar17 = "speedExploitEnabled";
      pcVar20 = pcVar19;
      break;
    case 0x44:
      bVar4 = false;
      bVar3 = false;
      uVar14 = 6;
      pcVar17 = pcVar16;
      pcVar20 = "autofarmEnabled";
    }
  }
  else {
    if (param_1 == 0x79) {
      uVar14 = 8;
      pcVar17 = "kitNaniModEnabled";
    }
    else {
      if (param_1 == 0x7a) {
        bVar4 = false;
        bVar3 = false;
        uVar14 = 5;
        pcVar17 = "autododgeEnabled";
        pcVar20 = "coltModEnabled";
        goto switchD_00119060_caseD_16;
      }
      if (param_1 != 0x84) goto switchD_00119060_caseD_18;
      uVar14 = 7;
      pcVar17 = "autododgeEnabled";
    }
    bVar3 = false;
    bVar4 = false;
    pcVar20 = "boltModEnabled";
  }
switchD_00119060_caseD_16:
  bVar1 = bVar3;
  if ((uVar14 & 0xc) == 4 || (uVar14 & 0xe) == 8) {
    bVar1 = true;
  }
  if ((0xfffffffd < param_2 - 4U) || (!bVar1)) {
    FUN_0010f580(0);
    uVar10 = FUN_001187d8();
    if (DAT_001293c4 == 0) {
      uVar12 = FUN_001189fc(uVar14,uVar10,0);
      if ((int)uVar12 == 0) goto switchD_00119060_caseD_18;
      if (bVar3) {
        ppuVar18 = &PTR_s_cameraEnabled_001216c0;
        do {
          ppuVar15 = ppuVar18;
          iVar11 = strcmp("autododgeEnabled",*ppuVar15);
          ppuVar18 = ppuVar15 + 5;
        } while (iVar11 != 0);
        if ((&DAT_00129130)[*(int *)(ppuVar15 + 1)] == 0) {
          ppuVar18 = &PTR_s_cameraEnabled_001216c0;
          do {
            ppuVar15 = ppuVar18;
            iVar11 = strcmp("kitNaniModEnabled",*ppuVar15);
            ppuVar18 = ppuVar15 + 5;
          } while (iVar11 != 0);
          iVar11 = (&DAT_00129130)[*(int *)(ppuVar15 + 1)];
          goto joined_r0x001192c4;
        }
      }
      else {
        pcVar16 = "aopAimEnabled";
        if (!bVar4) {
          pcVar16 = pcVar20;
        }
        if ((3 < uVar14 - 4) && (uVar14 != 0)) {
          pcVar16 = pcVar17;
        }
        if (*pcVar16 != '\0') {
          lVar13 = 1;
          do {
            pcVar17 = pcVar16 + lVar13;
            lVar13 = lVar13 + 1;
          } while (*pcVar17 != '\0');
        }
        iVar11 = strcmp(pcVar16,"isSpinEnabled");
        lVar13 = 0;
        if (iVar11 != 0) {
          ppuVar18 = &PTR_s_cameraEnabled_001216c0;
          do {
            lVar13 = lVar13 + 1;
            iVar11 = strcmp(pcVar16,*ppuVar18);
            ppuVar18 = ppuVar18 + 5;
          } while (iVar11 != 0);
        }
        iVar11 = (&DAT_00129130)[(int)(&DAT_001216a0)[lVar13 * 10]];
joined_r0x001192c4:
        if (iVar11 == 0) goto LAB_00119174;
      }
      if (DAT_00129ca8 != (code *)0x0) {
        uStack_70 = 0;
        local_78 = 0;
        lStack_80 = 0;
        uStack_88 = 0;
        local_90 = DAT_001047a8;
        iVar11 = (*DAT_00129ca8)(DAT_00129c48,param_1,&local_90);
        uVar8 = DAT_00129cc0;
        uVar7 = DAT_00129c38;
        uVar6 = DAT_00129348;
        uVar5 = DAT_001047b0;
        uVar12 = 0;
        if (((iVar11 == 1) && ((int)local_90 == 1)) && (local_90._4_4_ == 0x28)) {
          uVar12 = 0;
          uVar14 = 0x3f;
          if (!bVar4) {
            uVar14 = 0xff;
          }
          if ((((((uVar14 & ((uint)uStack_88 ^ 0xffffffff)) == 0) && (uStack_70._4_4_ == 0)) &&
               ((lStack_80 != 0 && ((local_78 != 0 && (uStack_88._4_4_ != 0)))))) &&
              (999999 < (uint)uStack_70)) && ((uint)uStack_70 < 2000000)) {
            uVar12 = 1;
            *(int *)(param_3 + 1) = param_1;
            *(int *)((long)param_3 + 0xc) = param_2;
            uVar9 = DAT_00129cf8;
            *param_3 = uVar5;
            param_3[2] = uVar6;
            param_3[3] = uVar7;
            param_3[4] = uVar8;
            param_3[5] = uVar9;
            param_3[7] = uStack_88;
            param_3[6] = local_90;
            param_3[9] = local_78;
            param_3[8] = lStack_80;
            param_3[10] = uStack_70;
          }
        }
        goto switchD_00119060_caseD_18;
      }
    }
  }
LAB_00119174:
  uVar12 = 0;
switchD_00119060_caseD_18:
  if (*(long *)(lVar2 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar12);
}

/* ===== nexus_evasion_dodge_profile @ 0011a5b4 [libNexusEvasion69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void nexus_evasion_dodge_profile(uint param_1,long param_2)

{
  int iVar1;
  int iVar2;
  char cVar3;
  bool bVar4;
  long lVar5;
  char cVar6;
  int iVar7;
  undefined8 uVar8;
  long lVar9;
  long lVar10;
  char local_98 [48];
  long local_68;
  
  lVar5 = tpidr_el0;
  local_68 = *(long *)(lVar5 + 0x28);
  uVar8 = 0;
  if ((param_2 == 0) || (param_1 - 6 < 0xfffffffb)) goto LAB_0011a6f8;
  do {
    cVar6 = DAT_00129ce8;
    cVar3 = '\x01';
    bVar4 = (bool)ExclusiveMonitorPass(0x129ce8,0x10);
    if (bVar4) {
      _DAT_00129ce8 = CONCAT31(DAT_00129ce8_1,1);
      cVar3 = ExclusiveMonitorsStatus();
    }
  } while (cVar3 != '\0' || cVar6 != '\0');
  FUN_0010f580(0);
  lVar10 = 0;
  do {
    if (param_1 == 1) {
      snprintf(local_98,0x30,"dodge%s",(&PTR_s_ReactionPercent_00121668)[lVar10]);
    }
    else {
      snprintf(local_98,0x30,"v%dDodge%s",(ulong)param_1);
    }
    lVar9 = 0;
    while (local_98[lVar9] != '\0') {
      lVar9 = lVar9 + 1;
      if (lVar9 == 0x60) goto LAB_0011a6f0;
    }
    lVar9 = -0x1630;
    while (iVar7 = strcmp(local_98,*(char **)((long)&PTR_s_aopAimEnabled_00122cc8 + lVar9)),
          iVar7 != 0) {
      lVar9 = lVar9 + 0x28;
      if (lVar9 == 0) goto LAB_0011a6f0;
    }
    iVar2 = (&DAT_00129130)[*(int *)((long)&PTR_s_aopPredictEnabled_00122cd0 + lVar9)];
    iVar1 = *(int *)((long)&PTR_s_autododgeEnabled_00122cd8 + lVar9 + 4);
    iVar7 = iVar2;
    if (iVar1 <= iVar2) {
      iVar7 = iVar1;
    }
    iVar1 = *(int *)((long)&PTR_s_autododgeEnabled_00122cd8 + lVar9);
    if (*(int *)((long)&PTR_s_autododgeEnabled_00122cd8 + lVar9) <= iVar2) {
      iVar1 = iVar7;
    }
    *(int *)(param_2 + lVar10 * 4) = iVar1;
    lVar10 = lVar10 + 1;
  } while (lVar10 != 6);
  uVar8 = 1;
LAB_0011a6f4:
  _DAT_00129ce8 = 0;
LAB_0011a6f8:
  if (*(long *)(lVar5 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar8);
LAB_0011a6f0:
  uVar8 = 0;
  goto LAB_0011a6f4;
}

/* ===== nexus_autododge_set_enabled @ 0011a76c [libNexusEvasion69252.so] ===== */

bool nexus_autododge_set_enabled(undefined4 param_1)

{
  int iVar1;
  
  iVar1 = FUN_0010ec90("autododgeEnabled",param_1,0);
  return iVar1 == 1;
}

/* ===== nexus_autododge_set_replay_block @ 0011a798 [libNexusEvasion69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

bool nexus_autododge_set_replay_block(int param_1)

{
  char cVar1;
  char cVar2;
  uint uVar3;
  bool bVar4;
  
  uVar3 = DAT_001293c4;
  cVar2 = DAT_00129ce8;
  do {
    cVar1 = '\x01';
    bVar4 = (bool)ExclusiveMonitorPass(0x129ce8,0x10);
    DAT_00129ce8 = cVar2;
    if (bVar4) {
      DAT_00129ce8 = '\x01';
      cVar1 = ExclusiveMonitorsStatus();
    }
    bVar4 = cVar2 != '\0';
    cVar2 = DAT_00129ce8;
  } while (cVar1 != '\0' || bVar4);
  DAT_001293c4 = (uint)(param_1 != 0);
  _DAT_00129ce8 = 0;
  return uVar3 != (param_1 != 0);
}

/* ===== nexus_evasion_resolve_slot @ 0011a7dc [libNexusEvasion69252.so] ===== */

undefined * nexus_evasion_resolve_slot(int param_1)

{
  if (param_1 - 5U < 4) {
    return (&PTR_nexus_autododge_set_enabled_00122d18)[(int)(param_1 - 5U)];
  }
  return (undefined *)0x0;
}

/* ===== FUN_0011c560 @ 0011c560 [libNexusEvasion69252.so] ===== */

void FUN_0011c560(undefined8 param_1,uint param_2,int *param_3)

{
  int iVar1;
  long lVar2;
  uint uVar3;
  int iVar4;
  undefined4 uVar5;
  uint uVar6;
  undefined *puVar7;
  ulong uVar8;
  undefined1 auStack_b8 [96];
  long local_58;
  
  lVar2 = tpidr_el0;
  local_58 = *(long *)(lVar2 + 0x28);
  if ((param_3 == (int *)0x0) || (*param_3 != 0x18)) {
LAB_0011c638:
    puVar7 = (undefined *)0x4;
  }
  else {
    uVar6 = param_2 - 0x10000;
    if (uVar6 < 0x76) {
      puVar7 = (&PTR_s_isSpinEnabled_00124238)[(ulong)uVar6 * 3];
      if (puVar7 != (undefined *)0x0) {
        uVar3 = nexus_evasion_get_requested(puVar7);
        iVar4 = nexus_evasion_get_port_state(puVar7);
        if (uVar3 != 0x80000000) {
          param_3[2] = 0;
          param_3[3] = 0;
          param_3[4] = 0;
          param_3[5] = 0;
          param_3[3] = uVar3;
          *(undefined1 *)((long)param_3 + 0x15) = 1;
          *param_3 = 0x18;
          param_3[1] = uVar3;
          iVar1 = *(int *)(&DAT_00124240 + (ulong)uVar6 * 0x18);
joined_r0x0011c720:
          if (iVar1 == 0) {
            uVar3 = nexus_evasion_get_effective(puVar7);
          }
          param_3[2] = uVar3;
          *(bool *)(param_3 + 5) = iVar4 != 3;
          if (iVar4 != 3) {
            uVar6 = (uint)((iVar1 == 0 && iVar4 != 2) && iVar4 != 1);
          }
          else {
            uVar6 = 3;
          }
          puVar7 = (undefined *)0x1;
          param_3[4] = uVar6;
          goto LAB_0011c63c;
        }
      }
    }
    else {
      if (0xa7 < param_2) goto LAB_0011c638;
      uVar8 = (ulong)param_2;
      uVar5 = nexus_evasion_get_requested("dodgeVersion");
      puVar7 = (undefined *)FUN_0011c914(&DAT_00122d38 + uVar8 * 4,uVar5,auStack_b8);
      if (puVar7 == (undefined *)0x0) goto LAB_0011c63c;
      uVar3 = nexus_evasion_get_requested();
      iVar4 = nexus_evasion_get_port_state(puVar7);
      if (uVar3 != 0x80000000) {
        param_3[2] = 0;
        param_3[3] = 0;
        param_3[4] = 0;
        param_3[5] = 0;
        *param_3 = 0x18;
        *(undefined1 *)((long)param_3 + 0x15) = 1;
        param_3[3] = uVar3;
        if (uVar8 - 0x5f < 10) {
          uVar3 = uVar3 >> (ulong)(*(uint *)(&DAT_00122d50 + uVar8 * 0x20) & 0x1f) & 1;
        }
        param_3[1] = uVar3;
        iVar1 = *(int *)(&DAT_00122d48 + uVar8 * 0x20);
        goto joined_r0x0011c720;
      }
    }
    puVar7 = (undefined *)0x0;
  }
LAB_0011c63c:
  if (*(long *)(lVar2 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(puVar7);
}

/* ===== FUN_0011c914 @ 0011c914 [libNexusEvasion69252.so] ===== */

char * FUN_0011c914(undefined8 *param_1,uint param_2,char *param_3)

{
  int iVar1;
  char *__s1;
  
  __s1 = (char *)*param_1;
  if ((__s1 != (char *)0x0) && (*(int *)(param_1 + 2) != 0)) {
    iVar1 = strncmp(__s1,"dodge",5);
    if ((iVar1 == 0) &&
       ((iVar1 = strcmp(__s1,"dodgeVersion"), iVar1 != 0 &&
        (iVar1 = strcmp(__s1,"dodgeBlacklistMask"), iVar1 != 0)))) {
      iVar1 = strcmp(__s1,"dodgePrecision");
      if ((param_2 - 2 < 4) && (iVar1 != 0)) {
        snprintf(param_3,0x60,"v%uDodge%s",(ulong)param_2,__s1 + 5);
        __s1 = param_3;
      }
    }
  }
  return __s1;
}

/* ===== FUN_0011d150 @ 0011d150 [libNexusEvasion69252.so] ===== */

void FUN_0011d150(undefined8 param_1,undefined8 param_2,undefined4 param_3)

{
  nexus_evasion_set_parameter("dodgeVersion",param_3);
  return;
}

/* ===== FUN_001417c8 @ 001417c8 [libNexusEvasionRuntime69252.so] ===== */

void FUN_001417c8(char *param_1,undefined8 param_2,undefined *param_3)

{
  undefined *puVar1;
  long lVar2;
  undefined8 uVar3;
  uint uVar4;
  __uid_t _Var5;
  int iVar6;
  uint uVar7;
  ulong uVar8;
  size_t sVar9;
  ssize_t sVar10;
  long lVar11;
  size_t __n;
  long lVar12;
  timespec local_1008 [250];
  long local_68;
  
  lVar2 = tpidr_el0;
  local_68 = *(long *)(lVar2 + 0x28);
  uVar4 = strcmp(param_1,"aura_phase");
  uVar7 = 0x140;
  if (uVar4 != 0) {
    uVar4 = strcmp(param_1,"dodge_phase");
    uVar7 = 0x140;
    if (uVar4 != 0) {
      uVar7 = 0x120;
    }
  }
  if (DAT_00209ce8 < uVar7) {
    DAT_00209ce8 = DAT_00209ce8 + 1;
    uVar4 = getpid();
    _Var5 = getuid();
    uVar8 = gettid((ulong)_Var5);
    uVar3 = DAT_00209d00;
    iVar6 = clock_gettime(0,local_1008);
    if (iVar6 == 0) {
      lVar12 = local_1008[0].tv_sec * 1000 + (ulong)local_1008[0].tv_nsec / 1000000;
    }
    else {
      lVar12 = 0;
    }
    iVar6 = clock_gettime(1,local_1008);
    if (iVar6 == 0) {
      lVar11 = local_1008[0].tv_sec * 1000 + (ulong)local_1008[0].tv_nsec / 1000000;
    }
    else {
      lVar11 = 0;
    }
    puVar1 = &DAT_00117c20;
    if (param_3 != (undefined *)0x0) {
      puVar1 = param_3;
    }
    uVar7 = snprintf((char *)local_1008,4000,
                     "{\"schema\":1,\"component\":\"nexus_evasion_runtime\",\"module\":\"EvasionGame\",\"stage\":\"evasion_game_%s\",\"reason\":\"%s\",\"pid\":%d,\"uid\":%d,\"tid\":%d,\"start_ticks\":%llu,\"time_ms\":%llu,\"mono_ms\":%llu,\"observer_rva\":\"0xb337c4\",\"capture_request_id\":%llu,\"gameplay_effective\":false%s}"
                     ,param_1,param_2,(ulong)uVar4,(ulong)_Var5,uVar8 & 0xffffffff,uVar3,lVar12,
                     lVar11,DAT_0020c0e0,puVar1);
    uVar4 = uVar7;
    if ((uVar7 < 4000) &&
       (__android_log_write(4,"NexusLab69252",local_1008), uVar4 = DAT_001cfb48,
       -1 < (int)DAT_001cfb48)) {
      __n = (size_t)(int)uVar7;
      if (__n + 1 + DAT_00209cf0 < 0x80001) {
        sVar9 = write(DAT_001cfb48,local_1008,__n);
        if ((sVar9 == __n) && (sVar10 = write(DAT_001cfb48,&DAT_0011e338,1), sVar10 == 1)) {
          DAT_00209cf0 = __n + 1 + DAT_00209cf0;
          uVar4 = 1;
        }
        else {
          uVar4 = close(DAT_001cfb48);
          DAT_001cfb48 = 0xffffffff;
        }
      }
    }
  }
  if (*(long *)(lVar2 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar4);
  }
  return;
}

/* ===== FUN_00142748 @ 00142748 [libNexusEvasionRuntime69252.so] ===== */

void FUN_00142748(void)

{
  long lVar1;
  bool bVar2;
  int iVar3;
  long lVar4;
  char *pcVar5;
  undefined8 uVar6;
  char *local_58;
  long local_50;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  DAT_0020d0f0 = FUN_0014d54c(&DAT_0020c0f0);
  if (DAT_0020d0f0 == 0) {
    lVar4 = dlerror();
    if (lVar4 == 0) {
      pcVar5 = "unknown";
    }
    else {
      pcVar5 = (char *)dlerror();
    }
    __android_log_print(6,"NexusMem","evasion backend resident handle missing path=%s error=%s",
                        &DAT_0020c0f0,pcVar5);
  }
  else {
    DAT_0020ad98 = (code *)dlsym(DAT_0020d0f0,"nexus_evasion_get_requested");
    DAT_0020ada0 = (code *)dlsym(DAT_0020d0f0,"nexus_evasion_dodge_profile");
    if ((((((DAT_0020ad98 == (code *)0x0) || (iVar3 = dladdr(DAT_0020ad98,&local_58), iVar3 == 0))
          || (local_50 != DAT_0020c0e8)) ||
         ((local_58 == (char *)0x0 || (iVar3 = strcmp(local_58,&DAT_0020c0f0), iVar3 != 0)))) ||
        ((DAT_0020ada0 == (code *)0x0 ||
         ((iVar3 = dladdr(DAT_0020ada0,&local_58), iVar3 == 0 || (local_50 != DAT_0020c0e8)))))) ||
       ((local_58 == (char *)0x0 || (iVar3 = strcmp(local_58,&DAT_0020c0f0), iVar3 != 0)))) {
      __android_log_print(6,"NexusMem",
                          "evasion backend export owner mismatch req=%p profile=%p base=%p path=%s",
                          DAT_0020ad98,DAT_0020ada0,DAT_0020c0e8,&DAT_0020c0f0);
    }
    else {
      uVar6 = (*DAT_0020ad98)("dodgeVersion");
      if ((int)uVar6 - 1U < 5) {
        iVar3 = (*DAT_0020ada0)(uVar6,&local_58);
        bVar2 = iVar3 == 1;
        goto LAB_00142864;
      }
    }
  }
  bVar2 = false;
LAB_00142864:
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(bVar2);
}

/* ===== FUN_001455a4 @ 001455a4 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_001455a4(ulong *param_1)

{
  undefined *puVar1;
  int iVar2;
  char cVar3;
  ulong uVar4;
  ulong uVar5;
  ulong uVar6;
  ulong uVar7;
  long lVar8;
  char cVar9;
  bool bVar10;
  uint uVar11;
  int iVar12;
  __uid_t _Var13;
  int iVar14;
  int iVar15;
  int *piVar16;
  size_t sVar17;
  int *piVar18;
  pthread_t __target_thread;
  long lVar19;
  char *pcVar20;
  undefined8 uVar21;
  undefined8 uVar22;
  char *pcVar23;
  timespec *ptVar24;
  ulong uVar25;
  uint uVar26;
  ulong uVar27;
  long lVar28;
  uint *puVar29;
  ulong uVar30;
  ulong uVar31;
  undefined1 auVar32 [16];
  undefined8 in_stack_ffffffffffffe3b0;
  ulong uVar33;
  undefined8 in_stack_ffffffffffffe3b8;
  ulong uVar34;
  ulong uVar35;
  undefined8 uVar36;
  undefined8 in_stack_ffffffffffffe3f0;
  undefined8 uVar37;
  undefined4 uVar38;
  undefined4 uVar39;
  undefined4 uVar40;
  undefined4 uVar41;
  undefined4 uVar42;
  undefined4 uVar43;
  undefined4 uVar44;
  undefined4 uVar45;
  undefined4 uVar46;
  undefined4 uVar47;
  undefined4 uVar48;
  undefined4 uVar49;
  undefined4 uVar50;
  undefined4 uVar51;
  undefined4 uVar52;
  undefined4 uVar53;
  undefined4 uVar54;
  undefined4 uVar55;
  ulong uVar56;
  undefined8 uVar57;
  undefined8 uVar58;
  long local_1b00;
  undefined8 local_1af8;
  long local_1af0;
  undefined8 local_1ae8;
  long local_1ae0;
  ulong local_1ad8;
  ulong uStack_1ad0;
  ulong local_1ac8;
  stat sStack_1ac0;
  undefined1 local_1a30 [136];
  __ino_t local_19a8;
  uint local_19a0;
  int local_199c;
  __uid_t local_1998;
  size_t local_1980;
  long local_1958;
  long local_1950;
  undefined1 auStack_1930 [3200];
  undefined1 auStack_cb0 [20];
  uint auStack_c9c [635];
  undefined8 local_2b0;
  undefined8 local_2a8;
  undefined8 uStack_2a0;
  undefined8 local_298;
  ulong uStack_290;
  undefined8 local_288;
  undefined8 uStack_280;
  undefined8 local_278;
  undefined8 uStack_270;
  undefined8 local_268;
  undefined8 uStack_260;
  undefined8 local_258;
  undefined8 uStack_250;
  undefined8 local_248;
  undefined8 uStack_240;
  undefined8 local_238;
  undefined8 uStack_230;
  undefined8 local_228;
  undefined8 uStack_220;
  undefined8 local_218;
  undefined8 uStack_210;
  undefined8 local_208;
  undefined8 uStack_200;
  undefined8 local_1f8;
  undefined8 uStack_1f0;
  undefined8 local_1e8;
  long local_1e0;
  undefined8 local_1d8;
  timespec local_1d0;
  undefined8 *local_1c0;
  undefined1 *local_1b8;
  uint local_1b0;
  int local_1ac;
  long local_1a8;
  undefined1 auStack_d0 [96];
  long local_70;
  
  uVar48 = (undefined4)((ulong)in_stack_ffffffffffffe3b0 >> 0x20);
  uVar49 = (undefined4)((ulong)in_stack_ffffffffffffe3b8 >> 0x20);
  uVar50 = (undefined4)((ulong)in_stack_ffffffffffffe3f0 >> 0x20);
  lVar8 = tpidr_el0;
  local_70 = *(long *)(lVar8 + 0x28);
  piVar16 = (int *)__errno();
  iVar2 = *piVar16;
  piVar18 = piVar16;
  if ((int)DAT_001dfff0 == 0) goto LAB_00145bc8;
  uVar11 = clock_gettime(1,&local_1d0);
  uVar25 = DAT_00217318;
  piVar18 = (int *)(ulong)uVar11;
  if (((((uVar11 == 0) &&
        (uVar30 = local_1d0.tv_sec * 1000 + (ulong)local_1d0.tv_nsec / 1000000, uVar30 != 0)) &&
       (DAT_00217318 < uVar30)) && ((DAT_00217318 == 0 || (999 < uVar30 - DAT_00217318)))) &&
     (-1 < DAT_001cfb50)) {
    do {
      if (DAT_00217318 != uVar25) {
        ClearExclusiveLocal();
        goto LAB_00145624;
      }
      cVar3 = '\x01';
      bVar10 = (bool)ExclusiveMonitorPass(0x217318,0x10);
      if (bVar10) {
        cVar3 = ExclusiveMonitorsStatus();
        DAT_00217318 = uVar30;
      }
    } while (cVar3 != '\0');
    do {
      cVar9 = DAT_00217320;
      cVar3 = '\x01';
      bVar10 = (bool)ExclusiveMonitorPass(0x217320,0x10);
      if (bVar10) {
        _DAT_00217320 = CONCAT31(DAT_00217320_1,1);
        cVar3 = ExclusiveMonitorsStatus();
      }
    } while (cVar3 != '\0');
    if (cVar9 == '\0') {
      uVar11 = openat(DAT_001cfb50,"nexus-evasion-game.capture",0x88800);
      piVar18 = (int *)(ulong)uVar11;
      if ((int)uVar11 < 0) {
        if (*piVar16 == 2) goto LAB_00145d7c;
        pcVar20 = "control_open_refused";
LAB_00145d74:
        piVar18 = (int *)FUN_00160f44(pcVar20,0);
      }
      else {
        iVar12 = fstat(uVar11,(stat *)(local_1a30 + 0x80));
        if ((((iVar12 != 0) || ((local_19a0 & 0xf000) != 0x8000)) ||
            ((_Var13 = getuid(), local_1998 != _Var13 ||
             (((local_199c != 1 || ((local_19a0 & 0x1ff) != 0x180)) || ((long)local_1980 < 1))))))
           || (0x60 < (long)local_1980)) {
          do {
            cVar9 = DAT_001cfe94;
            cVar3 = '\x01';
            bVar10 = (bool)ExclusiveMonitorPass(0x1cfe94,0x10);
            if (bVar10) {
              _DAT_001cfe94 = CONCAT31(DAT_001cfe94_1,1);
              cVar3 = ExclusiveMonitorsStatus();
            }
          } while (cVar3 != '\0');
          if (cVar9 == '\0') {
            uVar26 = DAT_00217324 + 1;
            bVar10 = DAT_00217324 < 8;
            DAT_00217324 = uVar26;
            if (bVar10) {
              FUN_001417c8("capture_request_rejected","control_owner_mode_type_link_or_size",0);
            }
            _DAT_001cfe94 = 0;
          }
          uVar11 = close(uVar11);
          piVar18 = (int *)(ulong)uVar11;
          goto LAB_00145d7c;
        }
        sVar17 = read(uVar11,auStack_d0,local_1980);
        if ((((sVar17 != local_1980) || (iVar12 = fstat(uVar11,(stat *)local_1a30), iVar12 != 0)) ||
            (((local_1a30._128_8_ != local_1a30._0_8_ ||
              ((local_19a8 != local_1a30._8_8_ || (local_1980 != local_1a30._48_8_)))) ||
             (local_1998 != local_1a30._24_4_)))) ||
           (((local_19a0 != local_1a30._16_4_ || (local_1a30._20_4_ != 1)) ||
            (local_1958 != local_1a30._88_8_)))) {
          close(uVar11);
LAB_00146414:
          pcVar20 = "control_changed_during_read";
          goto LAB_00145d74;
        }
        close(uVar11);
        if (local_1950 != local_1a30._96_8_) goto LAB_00146414;
        piVar18 = (int *)FUN_0018b35c(auStack_d0,local_1980,sStack_1ac0.__unused + 1);
        if ((int)piVar18 == 0) {
          pcVar20 = "control_format";
          goto LAB_00145d74;
        }
        if (DAT_0020c0e0 < (ulong)sStack_1ac0.__unused[1]) {
          do {
            cVar9 = DAT_001cfe94;
            cVar3 = '\x01';
            bVar10 = (bool)ExclusiveMonitorPass(0x1cfe94,0x10);
            if (bVar10) {
              _DAT_001cfe94 = CONCAT31(DAT_001cfe94_1,1);
              cVar3 = ExclusiveMonitorsStatus();
            }
          } while (cVar3 != '\0');
          if (cVar9 == '\0') {
            piVar18 = (int *)(ulong)DAT_001cfb48;
            if ((int)DAT_001cfb48 < 0) {
LAB_0014642c:
              uVar11 = DAT_00217324 + 1;
              bVar10 = DAT_00217324 < 8;
              DAT_00217324 = uVar11;
              if (bVar10) {
                pcVar20 = "capture_request_rejected";
                pcVar23 = "capture_journal_reset_failed";
                ptVar24 = (timespec *)0x0;
                goto LAB_00146458;
              }
            }
            else {
              uVar11 = fstat(DAT_001cfb48,&sStack_1ac0);
              piVar18 = (int *)(ulong)uVar11;
              if ((uVar11 != 0) || (((uint)sStack_1ac0.st_nlink & 0xf000) != 0x8000))
              goto LAB_0014642c;
              _Var13 = getuid();
              piVar18 = (int *)(ulong)_Var13;
              if ((sStack_1ac0.st_mode != _Var13) ||
                 ((sStack_1ac0.st_nlink._4_4_ != 1 ||
                  (piVar18 = (int *)FUN_0018b204(&DAT_001cfb48), (int)piVar18 == 0))))
              goto LAB_0014642c;
              DAT_00209cf0 = 0;
              DAT_00209ce8 = 0;
              DAT_0020c0e0 = sStack_1ac0.__unused[1];
              DAT_001cfb30 = (uint)sStack_1ac0.__unused[2];
              DAT_001cfb34 = sStack_1ac0.__unused[2]._4_4_;
              DAT_001cfb28 = 0;
              _DAT_001cfb38 = 0;
              DAT_002172f0 = 0;
              DAT_002172e8 = 0;
              DAT_00209ce0 = 0;
              DAT_00217258 = 0;
              snprintf((char *)&local_1d0,0x100,
                       ",\"samples\":%u,\"interval_ms\":%u,\"journal\":\"%s\",\"diagnostic_reset_only\":true"
                       ,(ulong)(uint)sStack_1ac0.__unused[2],(ulong)sStack_1ac0.__unused[2]._4_4_,
                       &DAT_00209d08);
              pcVar20 = "capture_request_accepted";
              pcVar23 = "validated_private_diagnostic_reset";
              ptVar24 = &local_1d0;
LAB_00146458:
              piVar18 = (int *)FUN_001417c8(pcVar20,pcVar23,ptVar24);
            }
            _DAT_001cfe94 = 0;
          }
        }
      }
LAB_00145d7c:
      _DAT_00217320 = 0;
    }
  }
LAB_00145624:
  do {
    cVar9 = DAT_001cfe94;
    cVar3 = '\x01';
    bVar10 = (bool)ExclusiveMonitorPass(0x1cfe94,0x10);
    if (bVar10) {
      _DAT_001cfe94 = CONCAT31(DAT_001cfe94_1,1);
      cVar3 = ExclusiveMonitorsStatus();
    }
  } while (cVar3 != '\0');
  if (cVar9 != '\0') goto LAB_00145bc8;
  iVar12 = clock_gettime(1,&local_1d0);
  if (iVar12 == 0) {
    lVar28 = local_1d0.tv_sec * 1000 + (ulong)local_1d0.tv_nsec / 1000000;
  }
  else {
    lVar28 = 0;
  }
  piVar18 = (int *)FUN_0018b7d4(&DAT_001cfb20,lVar28);
  iVar12 = (int)piVar18;
  if (((iVar12 != 0) || (DAT_00209cc8 != (code *)0x0)) || (DAT_001cfe90 != '\0')) {
    iVar14 = clock_gettime(1,&local_1d0);
    if (iVar14 == 0) {
      local_1b00 = local_1d0.tv_sec * 1000000 + (ulong)local_1d0.tv_nsec / 1000;
    }
    else {
      local_1b00 = 0;
    }
    iVar14 = gettid();
    local_1d8 = 0;
    local_1e0 = 0;
    __target_thread = pthread_self();
    uVar11 = pthread_getname_np(__target_thread,(char *)&local_1e0,0x10);
    piVar18 = (int *)(ulong)uVar11;
    if (((uVar11 == 0) && (local_1e0 == 0x706f6f6c6e69614d && (char)local_1d8 == '\0')) &&
       ((DAT_00209cd8 == 0 || (DAT_00209cd8 == iVar14)))) {
      DAT_0020d108 = 0;
      DAT_0020d100 = 0;
      DAT_0020d0f8 = 0;
      lVar19 = FUN_0015b4a0();
      uStack_1ad0 = param_1[1];
      local_1ad8 = *param_1;
      DAT_00217250 = DAT_00217250 + 1;
      local_1ae8 = DAT_0010e668;
      local_1ac8 = param_1[2];
      local_1ae0 = DAT_00217250;
      iVar15 = ng_feed_v1(&local_1ae8);
      local_1e8 = 0;
      uStack_2a0 = 0;
      local_2a8 = 0;
      uStack_290 = 0;
      local_298 = 0;
      uStack_280 = 0;
      local_288 = 0;
      uStack_270 = 0;
      local_278 = 0;
      uStack_260 = 0;
      local_268 = 0;
      uStack_250 = 0;
      local_258 = 0;
      uStack_240 = 0;
      local_248 = 0;
      uStack_230 = 0;
      local_238 = 0;
      uStack_220 = 0;
      local_228 = 0;
      uStack_210 = 0;
      local_218 = 0;
      uStack_200 = 0;
      local_208 = 0;
      uStack_1f0 = 0;
      local_1f8 = 0;
      local_2b0 = DAT_0010e808;
      ng_status_v1(&local_2b0);
      piVar18 = (int *)FUN_0015b4a0();
      DAT_00217258 = (long)piVar18 - lVar19;
      if (DAT_00209ce0 < DAT_00217258) {
        DAT_00209ce0 = DAT_00217258;
      }
      if (iVar12 != 0) {
        piVar18 = (int *)FUN_0018b82c(&DAT_001cfb20,iVar15);
      }
      if (((iVar15 == 0) || (DAT_00209cd8 != 0)) &&
         (local_1af0 = 0, iVar14 = DAT_00209cd8, iVar15 == 0)) {
        uVar11 = 0;
LAB_00145db4:
        if (local_1af0 != DAT_00217250) goto LAB_00145dc4;
      }
      else {
        DAT_00209cd8 = iVar14;
        local_1af0 = 0;
        piVar18 = (int *)ng_projectiles_v1(auStack_cb0,0x40,&local_1af0);
        uVar11 = (uint)piVar18;
        if (-1 < (int)uVar11) goto LAB_00145db4;
LAB_00145dc4:
        uVar11 = 0;
      }
      if ((DAT_00209cc8 != (code *)0x0) || (DAT_001cfe90 != '\0')) {
        local_1d0.tv_nsec = (long)&local_1ae8;
        local_1c0 = &local_2b0;
        local_1b8 = auStack_cb0;
        local_1d0.tv_sec = DAT_0010e580;
        local_1b0 = uVar11;
        local_1ac = iVar15;
        local_1a8 = lVar28;
        lVar28 = FUN_0015b4a0();
        FUN_0014a34c(&local_1d0);
        lVar19 = FUN_0015b4a0();
        uVar25 = lVar19 - lVar28;
        DAT_00217260 = DAT_00217260 + uVar25;
        if (DAT_002172a0 < uVar25) {
          DAT_002172a0 = uVar25;
        }
        uVar21 = FUN_00139fcc();
        FUN_0014a1c8(uVar21,local_1a8);
        FUN_00149fc4(&local_1d0);
        lVar28 = FUN_0015b4a0();
        FUN_0015b854(&local_1d0);
        lVar19 = FUN_0015b4a0();
        uVar25 = lVar19 - lVar28;
        DAT_00217268 = DAT_00217268 + uVar25;
        if (DAT_002172a8 < uVar25) {
          DAT_002172a8 = uVar25;
        }
        if ((local_1ac != 0) && (iVar14 = FUN_001550fc(), iVar14 != 0)) {
          DAT_002172e0 = 0;
        }
        lVar28 = FUN_0015b4a0();
        FUN_0015bfc4(&local_1d0);
        lVar19 = FUN_0015b4a0();
        uVar25 = lVar19 - lVar28;
        DAT_00217270 = DAT_00217270 + uVar25;
        if (DAT_002172b0 < uVar25) {
          DAT_002172b0 = uVar25;
        }
        lVar28 = FUN_0015b4a0();
        FUN_0015c6d4(&local_1d0);
        lVar19 = FUN_0015b4a0();
        uVar25 = lVar19 - lVar28;
        DAT_00217278 = DAT_00217278 + uVar25;
        if (DAT_002172b8 < uVar25) {
          DAT_002172b8 = uVar25;
        }
        FUN_0015cfd8(&local_1d0);
        FUN_0015d2d4();
        lVar28 = FUN_0015b4a0();
        FUN_0015d3c4(&local_1d0);
        lVar19 = FUN_0015b4a0();
        uVar25 = lVar19 - lVar28;
        DAT_00217290 = DAT_00217290 + uVar25;
        if (DAT_002172d0 < uVar25) {
          DAT_002172d0 = uVar25;
        }
        FUN_0014a254(&local_1d0);
        lVar28 = FUN_0015b4a0();
        FUN_0015d5d4(&local_1d0);
        lVar19 = FUN_0015b4a0();
        uVar25 = lVar19 - lVar28;
        DAT_00217280 = DAT_00217280 + uVar25;
        if (DAT_002172c0 < uVar25) {
          DAT_002172c0 = uVar25;
        }
        FUN_0015d7dc(&local_1d0);
        lVar28 = FUN_0015b4a0();
        FUN_0015d9f0(&local_1d0);
        lVar19 = FUN_0015b4a0();
        uVar25 = lVar19 - lVar28;
        DAT_00217288 = DAT_00217288 + uVar25;
        if (DAT_002172c8 < uVar25) {
          DAT_002172c8 = uVar25;
        }
        FUN_0015ec50(&local_1d0);
        FUN_0015efa8(&local_1d0);
        FUN_0015f500(&local_1d0);
        FUN_0015f874(&local_1d0);
        FUN_0015fe20(&local_1d0);
        FUN_001603a8(&local_1d0);
        piVar18 = (int *)FUN_00160b54(&local_1d0);
        if ((DAT_001cfe90 == '\x01') && (piVar18 = (int *)FUN_00160cec(), (int)piVar18 != 0)) {
          DAT_0020d158 = 1;
          piVar18 = (int *)FUN_00181454(&DAT_001cfea0,&local_1d0);
          DAT_0020d158 = 0;
          if (DAT_00214cf8 == 1) {
            DAT_00214cf8 = 0;
            DAT_00214c10 = 0;
          }
        }
        if (DAT_00209cc8 != (code *)0x0) {
          lVar28 = FUN_0015b4a0();
          (*DAT_00209cc8)(DAT_00209cc0,&local_1d0);
          piVar18 = (int *)FUN_0015b4a0();
          uVar25 = (long)piVar18 - lVar28;
          DAT_00217298 = DAT_00217298 + uVar25;
          if (DAT_002172d8 < uVar25) {
            DAT_002172d8 = uVar25;
          }
        }
      }
      if ((iVar12 != 0) && ((iVar15 != 0 || (DAT_001cfb3c < 0x21)))) {
        uVar25 = param_1[2];
        local_1af8 = 0;
        uVar31 = param_1[1];
        uVar30 = *param_1;
        bVar10 = iVar15 != 0;
        pcVar20 = "false";
        if (bVar10) {
          pcVar20 = "true";
        }
        uVar21 = 0;
        if (bVar10) {
          uVar21 = uStack_280;
        }
        uVar39 = 0;
        if (bVar10) {
          uVar39 = (undefined4)local_238;
        }
        uVar38 = 0;
        if (bVar10) {
          uVar38 = uStack_240._4_4_;
        }
        uVar51 = 0;
        if (bVar10) {
          uVar51 = (undefined4)uStack_240;
        }
        uVar36 = 0;
        if (bVar10) {
          uVar36 = local_278;
        }
        uVar45 = 0;
        if (bVar10) {
          uVar45 = (undefined4)uStack_220;
        }
        uVar46 = 0;
        if (bVar10) {
          uVar46 = uStack_220._4_4_;
        }
        uVar41 = 0;
        if (bVar10) {
          uVar41 = (undefined4)uStack_230;
        }
        uVar42 = 0;
        if (bVar10) {
          uVar42 = uStack_230._4_4_;
        }
        uVar40 = 0;
        if (bVar10) {
          uVar40 = local_238._4_4_;
        }
        uVar43 = 0;
        if (bVar10) {
          uVar43 = (undefined4)local_228;
        }
        uVar44 = 0;
        if (bVar10) {
          uVar44 = local_228._4_4_;
        }
        uVar47 = 0;
        if (bVar10) {
          uVar47 = (undefined4)local_218;
        }
        uVar37 = CONCAT44(uVar50,uVar51);
        uVar34 = CONCAT44(uVar49,(undefined4)local_248);
        uVar33 = CONCAT44(uVar48,uStack_250._4_4_);
        uVar35 = uStack_290;
        uVar48 = local_218._4_4_;
        uVar49 = (undefined4)uStack_210;
        uVar50 = uStack_210._4_4_;
        uVar51 = (undefined4)local_208;
        uVar52 = local_208._4_4_;
        uVar53 = (undefined4)uStack_200;
        uVar54 = uStack_200._4_4_;
        uVar55 = (undefined4)local_1f8;
        uVar56 = DAT_00217258;
        lVar28 = DAT_0020d0f8;
        uVar57 = DAT_0020d100;
        uVar58 = DAT_0020d108;
        auVar32 = FUN_00160e50(auStack_1930,uVar40,&local_1af8,
                               ",\"accepted\":%s,\"callbacks\":%llu,\"sequence\":%llu,\"frame_reason\":%u,\"stable_frames\":%u,\"passive_ready\":%u,\"epoch\":%llu,\"screen\":\"0x%llx\",\"own_arg\":\"0x%llx\",\"optional_arg\":\"0x%llx\",\"client\":\"0x%llx\",\"battle\":\"0x%llx\",\"own_gid\":%d,\"own_x\":%d,\"own_y\":%d,\"own_z\":%d,\"own_radius\":%d,\"own_team\":%d,\"object_count\":%d,\"projectile_count\":%u,\"friendly_projectiles\":%u,\"unknown_objects\":%u,\"feed_complete\":%u,\"requested_autododge\":%d,\"dodge_version\":%d,\"profile\":[%d,%d,%d,%d,%d,%d],\"duration_us\":%llu,\"read_calls\":%llu,\"read_bytes\":%llu,\"read_failures\":%llu,\"projectiles\":["
                               ,pcVar20,DAT_001cfb20,DAT_00217250,local_248._4_4_,uVar33,uVar34,
                               uStack_290,uVar30,uVar31,uVar25,uVar36,uVar21,uVar37,uVar38,uVar39,
                               uVar40,uVar41,uVar42,uVar43,uVar44,uVar45,uVar46,uVar47,
                               local_218._4_4_,(undefined4)uStack_210,uStack_210._4_4_,
                               (undefined4)local_208,local_208._4_4_,(undefined4)uStack_200,
                               uStack_200._4_4_,(undefined4)local_1f8,DAT_00217258,DAT_0020d0f8,
                               DAT_0020d100,DAT_0020d108);
        uVar22 = auVar32._8_8_;
        uVar26 = uVar11;
        if (7 < (int)uVar11) {
          uVar26 = 8;
        }
        bVar10 = auVar32._0_4_ != 0;
        if ((uVar11 != 0) && (auVar32._0_4_ != 0)) {
          uVar27 = 1;
          puVar29 = auStack_c9c;
          do {
            puVar1 = &DAT_00117c20;
            if (uVar27 != 1) {
              puVar1 = &DAT_00120242;
            }
            uVar35 = (ulong)puVar29[2];
            uVar34 = (ulong)puVar29[1];
            uVar31 = (ulong)puVar29[4];
            uVar30 = (ulong)puVar29[3];
            uVar33 = (ulong)*puVar29;
            auVar32 = FUN_00160e50(auStack_1930,uVar22,&local_1af8,
                                   "%s{\"gid\":%d,\"x\":%d,\"y\":%d,\"z\":%d,\"team\":%d,\"speed\":%d,\"radius\":%d,\"bouncing\":%u}"
                                   ,puVar1,puVar29[-3],puVar29[-2],puVar29[-1],uVar33,uVar34,uVar35,
                                   uVar30,uVar31,uVar25,uVar36,uVar21,uVar37,uVar38,uVar39,uVar40,
                                   uVar41,uVar42,uVar43,uVar44,uVar45,uVar46,uVar47,uVar48,uVar49,
                                   uVar50,uVar51,uVar52,uVar53,uVar54,uVar55,uVar56,lVar28,uVar57,
                                   uVar58);
            uVar22 = auVar32._8_8_;
            bVar10 = auVar32._0_4_ != 0;
            if (uVar26 <= uVar27) break;
            uVar27 = uVar27 + 1;
            puVar29 = puVar29 + 10;
          } while (auVar32._0_4_ != 0);
        }
        piVar18 = auVar32._0_8_;
        if ((bVar10) &&
           (piVar18 = (int *)FUN_00160e50(auStack_1930,auVar32._8_8_,&local_1af8,
                                          "],\"projectiles_logged\":%d,\"projectiles_omitted\":%d,\"previous_callback_us\":%llu,\"max_callback_us\":%llu"
                                          ,(ulong)uVar26,uVar11 - uVar26,DAT_002172e8,DAT_002172f0,
                                          uVar33,uVar34,uVar35,uVar30,uVar31,uVar25,uVar36,uVar21,
                                          uVar37,uVar38,uVar39,uVar40,uVar41,uVar42,uVar43,uVar44,
                                          uVar45,uVar46,uVar47,uVar48,uVar49,uVar50,uVar51,uVar52,
                                          uVar53,uVar54,uVar55,uVar56,lVar28,uVar57,uVar58),
           (int)piVar18 != 0)) {
          pcVar20 = "frame_contract_rejected";
          if (iVar15 != 0) {
            pcVar20 = "readonly_frame_observed";
          }
          piVar18 = (int *)FUN_001417c8("frame",pcVar20,auStack_1930);
        }
        if (DAT_001cfb30 == 0) {
          pcVar20 = "capture_limit";
          pcVar23 = "256_accepted_samples_complete";
          goto LAB_00145a44;
        }
      }
    }
    else {
      uVar11 = DAT_001cfb3c;
      _DAT_001cfb38 = CONCAT44(DAT_001cfb3c + 1,DAT_001cfb38);
      if (uVar11 < 8) {
        pcVar20 = "frame_thread_rejected";
        pcVar23 = "expected_Mainloop";
LAB_00145a44:
        piVar18 = (int *)FUN_001417c8(pcVar20,pcVar23,0);
      }
    }
    if (local_1b00 != 0) {
      uVar11 = clock_gettime(1,&local_1d0);
      piVar18 = (int *)(ulong)uVar11;
      if (uVar11 == 0) {
        lVar28 = local_1d0.tv_sec * 1000000 + (ulong)local_1d0.tv_nsec / 1000;
      }
      else {
        lVar28 = 0;
      }
      DAT_002172e8 = lVar28 - local_1b00;
      if (DAT_002172f0 < DAT_002172e8) {
        DAT_002172f0 = DAT_002172e8;
      }
      DAT_002172f8 = DAT_002172f8 + DAT_002172e8;
      DAT_00217300 = DAT_00217300 + DAT_00217258;
      DAT_00217308 = DAT_00217308 + DAT_0020d0f8;
      DAT_00217310 = DAT_00217310 + 1;
      uVar25 = (ulong)DAT_00217310;
      if (0x77 < DAT_00217310) {
        uVar30 = 0;
        if (uVar25 != 0) {
          uVar30 = DAT_002172f8 / uVar25;
        }
        uVar31 = 0;
        if (uVar25 != 0) {
          uVar31 = DAT_00217308 / uVar25;
        }
        uVar35 = 0;
        if (uVar25 != 0) {
          uVar35 = DAT_00217300 / uVar25;
        }
        uVar56 = 0;
        if (uVar25 != 0) {
          uVar56 = DAT_00217260 / uVar25;
        }
        uVar33 = 0;
        if (uVar25 != 0) {
          uVar33 = DAT_00217268 / uVar25;
        }
        uVar34 = 0;
        if (uVar25 != 0) {
          uVar34 = DAT_00217270 / uVar25;
        }
        uVar27 = 0;
        if (uVar25 != 0) {
          uVar27 = DAT_00217278 / uVar25;
        }
        uVar4 = 0;
        if (uVar25 != 0) {
          uVar4 = DAT_00217280 / uVar25;
        }
        uVar5 = 0;
        if (uVar25 != 0) {
          uVar5 = DAT_00217288 / uVar25;
        }
        uVar6 = 0;
        if (uVar25 != 0) {
          uVar6 = DAT_00217290 / uVar25;
        }
        uVar7 = 0;
        if (uVar25 != 0) {
          uVar7 = DAT_00217298 / uVar25;
        }
        piVar18 = (int *)__android_log_print(4,"NexusPerf",
                                             "frames=%u total_us=%llu feed_us=%llu reads=%llu hud=%llu aim=%llu trophy=%llu branding=%llu visual=%llu dodge=%llu periodic=%llu consumer=%llu aim_max=%llu branding_max=%llu"
                                             ,uVar25,uVar30,uVar35,uVar31,uVar56,uVar33,uVar34,
                                             uVar27,uVar4,uVar5,uVar6,uVar7,DAT_002172a8,
                                             DAT_002172b8);
        DAT_00217310 = 0;
        DAT_00217308 = 0;
        DAT_00217300 = 0;
        DAT_002172f8 = 0;
        DAT_00217268 = 0;
        DAT_00217260 = 0;
        DAT_00217278 = 0;
        DAT_00217270 = 0;
        DAT_00217288 = 0;
        DAT_00217280 = 0;
        DAT_00217298 = 0;
        DAT_00217290 = 0;
        DAT_002172a8 = 0;
        DAT_002172a0 = 0;
        DAT_002172b8 = 0;
        DAT_002172b0 = 0;
        DAT_002172c8 = 0;
        DAT_002172c0 = 0;
        DAT_002172d8 = 0;
        DAT_002172d0 = 0;
      }
    }
  }
  _DAT_001cfe94 = 0;
LAB_00145bc8:
  *piVar16 = iVar2;
  if (*(long *)(lVar8 + 0x28) != local_70) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(piVar18);
  }
  return;
}

/* ===== FUN_0014653c @ 0014653c [libNexusEvasionRuntime69252.so] ===== */

undefined4 FUN_0014653c(int param_1)

{
  char *pcVar1;
  long lVar2;
  int iVar3;
  undefined4 uVar4;
  undefined8 local_170;
  undefined8 local_168;
  code *pcStack_160;
  undefined8 local_158;
  code *pcStack_150;
  undefined *local_148;
  undefined *puStack_140;
  undefined8 local_138;
  undefined *puStack_130;
  undefined *local_128;
  undefined *puStack_120;
  undefined *local_118;
  undefined *puStack_110;
  undefined *local_108;
  code *pcStack_100;
  code *local_f8;
  undefined8 local_f0;
  undefined8 local_e8;
  undefined8 uStack_e0;
  code *local_d8;
  undefined *puStack_d0;
  undefined *local_c8;
  undefined4 local_c0;
  undefined8 local_bc;
  undefined4 local_b4;
  undefined8 local_b0;
  undefined8 uStack_a8;
  undefined8 local_a0;
  undefined4 local_98;
  undefined8 local_94;
  undefined4 local_8c;
  undefined8 local_88;
  undefined8 uStack_80;
  undefined8 local_78;
  undefined4 local_70;
  undefined8 local_6c;
  undefined4 local_64;
  undefined8 local_60;
  undefined8 uStack_58;
  undefined8 local_50;
  long local_48;
  
  lVar2 = tpidr_el0;
  uVar4 = 0;
  local_48 = *(long *)(lVar2 + 0x28);
  if ((param_1 != 0) && (DAT_002147d8 != '\0')) {
    local_f0 = DAT_0010e780;
    local_e8 = DAT_00209d00;
    uStack_e0 = DAT_001e0978;
    local_bc = DAT_00214160;
    local_b4 = 0;
    uStack_a8 = uRam0000000000214150;
    local_b0 = DAT_00214148;
    local_d8 = FUN_001455a4;
    puStack_d0 = &DAT_00213130;
    local_c8 = PTR_FUN_001cb6d8;
    local_94 = DAT_00214398;
    local_c0 = DAT_00214130;
    local_a0 = DAT_00214158;
    uStack_80 = uRam0000000000214388;
    local_88 = DAT_00214380;
    local_98 = DAT_00214368;
    local_78 = DAT_00214390;
    local_6c = DAT_002145d0;
    local_70 = DAT_002145a0;
    local_8c = 0;
    local_64 = 0;
    uStack_58 = uRam00000000002145c0;
    local_60 = DAT_002145b8;
    local_50 = DAT_002145c8;
    iVar3 = (*DAT_00213050)(&local_f0);
    if (iVar3 == 0) {
      uVar4 = 0;
    }
    else {
      DAT_00228dd8 = 0x1000003f7;
      puStack_140 = PTR_FUN_001cb6e0;
      if (DAT_00215a60 == '\0') {
        DAT_00228dd8 = 0x100000003;
        puStack_140 = (undefined *)0x0;
      }
      local_158 = 0;
      pcStack_150 = FUN_001512f8;
      local_138 = 0;
      local_148 = PTR_FUN_001cb6e8;
      local_168 = DAT_00209d00;
      pcStack_160 = FUN_001455a4;
      pcStack_100 = FUN_0016a114;
      local_170 = DAT_0010e628;
      local_f8 = FUN_0016a17c;
      puStack_130 = puStack_140;
      local_128 = puStack_140;
      puStack_120 = puStack_140;
      local_118 = puStack_140;
      puStack_110 = puStack_140;
      local_108 = puStack_140;
      iVar3 = (*DAT_00213038)(&local_170);
      if (iVar3 == 0) {
        uVar4 = 0;
        DAT_00228dd8 = 0x200000000;
      }
      else {
        uVar4 = 1;
        pcVar1 = "smartaim_dodge_v1_v5_follow";
        if (DAT_00215a60 == '\0') {
          pcVar1 = "natural_smartaim_only";
        }
        DAT_002147dc = 1;
        FUN_001417c8("functions_registered",pcVar1,
                     ",\"aim\":true,\"prediction_for_aim\":true,\"dodge_handler_separate_diagnostic\":true,\"require_move_hold\":false,\"free_scope\":2,\"paid_state_changed\":false"
                    );
      }
    }
  }
  if (*(long *)(lVar2 + 0x28) != local_48) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return uVar4;
}

/* ===== FUN_00153980 @ 00153980 [libNexusEvasionRuntime69252.so] ===== */

void FUN_00153980(void)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  long lVar4;
  int iVar5;
  undefined8 uVar6;
  size_t __len;
  ulong __len_00;
  char *pcVar7;
  char *pcVar8;
  char *pcVar9;
  uint uVar10;
  long lVar11;
  undefined8 local_258;
  long local_250;
  char *pcStack_248;
  char *local_240;
  undefined8 uStack_238;
  code *local_230;
  code *pcStack_228;
  code *local_220;
  code *pcStack_218;
  code *local_210;
  code *pcStack_208;
  undefined1 auStack_200 [112];
  long local_190;
  long lStack_188;
  long local_180;
  long lStack_178;
  undefined1 auStack_170 [256];
  long local_70;
  
  lVar4 = tpidr_el0;
  local_70 = *(long *)(lVar4 + 0x28);
  iVar5 = FUN_0018d938(&DAT_00215a10,DAT_001e0978,&PTR_DAT_001c3220,FUN_001428fc,0);
  if (iVar5 == 0) {
    pcVar8 = "guest_game_elf";
  }
  else {
    DAT_00215a28 = 0;
    uVar6 = dl_iterate_phdr(FUN_00157f30,0);
    pcVar8 = "packaged_cpp_owner";
    if ((DAT_00215a28 == 1) && (DAT_00215a40 != 0)) {
      DAT_00215a48 = 1;
      iVar5 = FUN_00158080(uVar6,"packaged_cpp_owner");
      if (iVar5 == 0) {
        pcVar8 = "allocator_got_owner";
      }
      else {
        lVar11 = 0;
        do {
          uVar6 = FUN_0018d348(auStack_200);
          uVar1 = (&DAT_0010f5bc)[lVar11 * 10];
          if (uVar1 != 0) {
            uVar10 = 0;
            uVar2 = (&DAT_0010f5b8)[lVar11 * 10];
            do {
              uVar3 = uVar1 - uVar10;
              if (0xff < uVar3) {
                uVar3 = 0x100;
              }
              iVar5 = FUN_001428fc(uVar6,DAT_001e0978 + (ulong)uVar10 + (ulong)uVar2,auStack_170,
                                   uVar3);
              if (iVar5 == 0) goto LAB_00153c30;
              uVar6 = FUN_0018d36c(auStack_200,auStack_170,uVar3);
              uVar10 = uVar3 + uVar10;
            } while (uVar10 < uVar1);
          }
          FUN_0018d6ac(auStack_200,&local_190);
          if (((local_190 != (&DAT_0010f5c0)[lVar11 * 5] ||
               lStack_188 != (&DAT_0010f5c8)[lVar11 * 5]) ||
              local_180 != (&DAT_0010f5d0)[lVar11 * 5]) || lStack_178 != (&DAT_0010f5d8)[lVar11 * 5]
             ) {
LAB_00153c30:
            pcVar8 = "actor_native_contract";
            goto LAB_00153c44;
          }
          lVar11 = lVar11 + 1;
        } while (lVar11 != 6);
        __len = FUN_001a23cc();
        __len_00 = FUN_001bae30();
        pcVar8 = "state_size";
        if (((0xffffffffffefffff < __len - 0x100001) && (__len_00 != 0)) && (__len_00 < 0x10001)) {
          DAT_00215a50 = mmap((void *)0x0,__len,3,0x22,-1,0);
          DAT_00215a58 = mmap((void *)0x0,__len_00,3,0x22,-1,0);
          pcVar8 = "resident_state_allocation";
          if ((DAT_00215a50 != (void *)0xffffffffffffffff) &&
             (DAT_00215a58 != (void *)0xffffffffffffffff)) {
            local_250 = DAT_001e0978;
            pcStack_248 = "a10aeb6b4085fb2a15d969a130cd9608231998b5269a40aed41129d99624ede3";
            local_240 = "fdf834103d333f9f8a1947b3a405b32da6ebb651";
            uStack_238 = 0;
            local_258 = DAT_0010e700;
            local_230 = FUN_001428fc;
            pcStack_228 = FUN_00158260;
            local_220 = FUN_00158310;
            pcStack_218 = FUN_0015851c;
            local_210 = FUN_00158a38;
            pcStack_208 = FUN_00158aec;
            iVar5 = FUN_001a24f8(DAT_00215a50,__len,&local_258);
            if ((iVar5 != 0) && (iVar5 = FUN_001bae38(DAT_00215a58,__len_00), iVar5 != 0)) {
              pcVar7 = "dodge_bound";
              pcVar8 = "single_source_consumer_and_guest_ops";
              DAT_00215a60 = 1;
              pcVar9 = 
              ",\"action\":22,\"version\":3,\"require_move_hold\":false,\"native_calls_during_bind\":0,\"new_hooks\":0"
              ;
              goto LAB_00153c5c;
            }
            pcVar8 = "consumer_native_binding";
          }
        }
      }
    }
  }
LAB_00153c44:
  pcVar7 = "dodge_disabled";
  pcVar9 = ",\"action\":22,\"handler_registered\":false";
  DAT_00215a60 = 0;
LAB_00153c5c:
  FUN_001417c8(pcVar7,pcVar8,pcVar9);
  if (*(long *)(lVar4 + 0x28) == local_70) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0015d9f0 @ 0015d9f0 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0015d9f0(long param_1)

{
  bool bVar1;
  long lVar2;
  undefined8 uVar3;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  int iVar10;
  undefined4 uVar11;
  undefined8 uVar12;
  size_t sVar13;
  long lVar14;
  int *piVar15;
  undefined8 uVar16;
  undefined8 uVar17;
  undefined8 uVar18;
  long lVar19;
  ulong uVar20;
  ulong uVar21;
  long lVar22;
  ulong uVar23;
  undefined8 *puVar24;
  undefined1 auVar25 [16];
  undefined8 uVar26;
  undefined8 uVar27;
  undefined8 uVar28;
  undefined8 uVar29;
  undefined8 uVar30;
  undefined8 in_stack_ffffffffffffeed0;
  undefined4 uVar31;
  undefined4 uVar32;
  undefined4 uVar33;
  undefined4 uVar34;
  undefined4 uVar35;
  undefined4 uVar36;
  undefined4 uVar37;
  undefined4 uVar38;
  undefined4 uVar39;
  undefined4 uVar40;
  undefined4 uVar41;
  undefined4 uVar42;
  undefined4 uVar43;
  undefined4 uVar44;
  undefined4 uVar45;
  undefined4 uVar46;
  undefined4 uVar47;
  undefined4 uVar48;
  undefined4 uVar49;
  undefined4 uVar50;
  undefined4 uVar51;
  undefined8 local_f88;
  undefined *local_f80;
  undefined8 *puStack_f78;
  undefined8 *local_f70;
  undefined8 *puStack_f68;
  undefined8 *local_f60;
  undefined8 *puStack_f58;
  undefined8 local_f50;
  undefined8 uStack_f48;
  undefined8 uStack_f40;
  undefined8 uStack_f38;
  undefined8 local_f30;
  undefined8 uStack_f28;
  undefined8 uStack_f20;
  undefined8 uStack_f18;
  undefined8 local_f10;
  undefined8 uStack_f08;
  undefined8 local_f00;
  undefined8 uStack_ef8;
  undefined8 uStack_ef0;
  undefined8 uStack_ee8;
  undefined8 local_ee0;
  undefined8 uStack_ed8;
  undefined8 uStack_ed0;
  undefined8 uStack_ec8;
  undefined8 local_ec0;
  undefined8 uStack_eb8;
  undefined8 uStack_eb0;
  undefined8 uStack_ea8;
  undefined8 local_ea0;
  undefined8 uStack_e98;
  undefined8 uStack_e90;
  undefined8 uStack_e88;
  undefined8 local_e80;
  undefined8 uStack_e78;
  undefined8 uStack_e70;
  undefined8 uStack_e68;
  undefined8 local_e60;
  undefined8 uStack_e58;
  undefined8 uStack_e50;
  undefined8 uStack_e48;
  undefined8 local_e40;
  int local_e38;
  uint local_e34;
  undefined *local_e30;
  undefined8 local_e28;
  undefined8 local_e20;
  undefined8 local_e18;
  undefined8 local_e10;
  undefined8 uStack_e08;
  undefined8 local_e00;
  undefined *local_df0;
  undefined8 local_de8;
  undefined8 local_de0;
  undefined8 uStack_dd8;
  undefined8 local_dd0;
  undefined8 uStack_dc8;
  undefined8 local_dc0;
  undefined8 uStack_db8;
  undefined8 local_db0;
  undefined8 local_da8;
  undefined8 local_da0;
  undefined8 uStack_d98;
  undefined8 local_d90;
  undefined8 uStack_d88;
  undefined8 local_d80;
  undefined4 local_d78;
  uint local_d6c;
  undefined8 local_d68;
  long local_d60;
  undefined8 local_d58;
  uint local_d4c;
  undefined1 auStack_d48 [16];
  int local_d38;
  undefined4 uStack_d34;
  undefined8 uStack_d30;
  undefined4 local_d1c;
  timespec local_d08 [200];
  long local_88;
  
  lVar2 = tpidr_el0;
  local_88 = *(long *)(lVar2 + 0x28);
  if (DAT_00215a60 != '\x01') goto LAB_0015e540;
  iVar4 = clock_gettime(1,local_d08);
  if (iVar4 == 0) {
    lVar22 = local_d08[0].tv_sec * 1000000 + (ulong)local_d08[0].tv_nsec / 1000;
  }
  else {
    lVar22 = 0;
  }
  _DAT_00215aa8 = 0;
  DAT_00215aa0 = 0;
  DAT_00215ab8 = 0;
  _DAT_00215ab0 = 0;
  DAT_00215ac8 = 0;
  DAT_00215ac0 = 0;
  DAT_00215ad8 = 0;
  DAT_00215ad0 = 0;
  DAT_00215ae8 = 0;
  DAT_00215ae0 = 0;
  DAT_00215af8 = 0;
  DAT_00215af0 = 0;
  DAT_00220920 = DAT_00220920 + 1;
  DAT_00215b04._4_4_ = 0;
  DAT_00215b0c = 0;
  _DAT_00215b00 = 0;
  uRam0000000000215b18 = 0;
  DAT_00215b1c = 0;
  DAT_00215b10 = 0;
  DAT_00215b14 = 0;
  _DAT_00215b28 = 0;
  _DAT_00215b20 = 0;
  _DAT_00215b38 = 0;
  DAT_00215b30 = 0;
  DAT_00215b40 = 0;
  DAT_00220928 = 1;
  DAT_0022092c = 0;
  _DAT_00215ca0 = 0;
  DAT_00215c98 = 0;
  _DAT_00215cb0 = 0;
  DAT_00215ca8 = 0;
  _DAT_00215cc0 = 0;
  _DAT_00215cb8 = 0;
  DAT_00215cd0 = 0;
  _DAT_00215cc8 = 0;
  DAT_00215ce0 = 0;
  DAT_00215cd8 = 0;
  _DAT_00215cf0 = 0;
  DAT_00215ce8 = 0;
  DAT_00215d00 = (undefined8 *)0x0;
  DAT_00215cf8 = 0;
  DAT_002209b0 = 0;
  _DAT_00220938 = 0;
  DAT_00220930 = 0;
  DAT_00220948 = (undefined *)0x0;
  _DAT_00220940 = 0;
  DAT_00220958 = 0;
  DAT_00220950 = 0;
  uRam0000000000220968 = 0;
  _DAT_00220960 = 0;
  uRam0000000000220978 = 0;
  _DAT_00220970 = 0;
  uRam0000000000220988 = 0;
  _DAT_00220980 = 0;
  DAT_00220998 = (undefined8 *)0x0;
  DAT_00220990 = 0;
  DAT_002209a8 = (undefined *)0x0;
  _DAT_002209a0 = 0;
  memset(&DAT_002209c0,0,0xdb8);
  iVar4 = DAT_00209cd8;
  uVar11 = (undefined4)((ulong)in_stack_ffffffffffffeed0 >> 0x20);
  _DAT_00215c40 = 0;
  DAT_002209b8 = DAT_0010e590;
  _DAT_00215d70 = 0;
  DAT_00215d74 = 0;
  _DAT_00215c08 = 0;
  DAT_00215c00 = 0;
  DAT_00215c18 = 0;
  _DAT_00215c10 = 0;
  uRam0000000000215c28 = 0;
  _DAT_00215c20 = 0;
  _DAT_00215c38 = 0;
  DAT_00215c30 = 0;
  DAT_00215d58 = (undefined *)0x0;
  DAT_00215d50 = 0;
  DAT_00215d68 = 0;
  DAT_00215d6c = 0;
  _DAT_00215d60 = 0;
  _DAT_00215a70 = DAT_0020f638;
  _DAT_00215a68 = _DAT_0020f630;
  uRam0000000000215a80 = _DAT_0020f648;
  _DAT_00215a78 = _DAT_0020f640;
  DAT_00215a90 = _DAT_0020f658;
  DAT_00215a88 = DAT_0020f650;
  DAT_00215a98 = DAT_0020f660;
  _DAT_00215bf8 = 0;
  DAT_00215bec._4_4_ = 0;
  DAT_00215bf4 = 0;
  DAT_00215be8 = 0;
  DAT_00215bec._0_4_ = 0;
  DAT_00215be0 = 0;
  uRam0000000000215bd8 = 0;
  _DAT_00215bd0 = 0;
  _DAT_00215bc8 = 0;
  _DAT_00215bc0 = 0;
  _DAT_00215bb8 = 0;
  _DAT_00215bb0 = 0;
  uRam0000000000215ba8 = 0;
  _DAT_00215ba0 = 0;
  uRam0000000000215b98 = 0;
  _DAT_00215b90 = 0;
  uRam0000000000215b88 = 0;
  _DAT_00215b80 = 0;
  uRam0000000000215b78 = 0;
  _DAT_00215b70 = 0;
  uRam0000000000215b68 = 0;
  _DAT_00215b60 = 0;
  uRam0000000000215b58 = 0;
  DAT_00215b50 = 0;
  DAT_00215b48 = DAT_0010e7d0;
  if ((((((param_1 == 0) || (*(long *)(param_1 + 8) == 0)) ||
        (lVar14 = *(long *)(param_1 + 0x10), lVar14 == 0)) ||
       ((*(int *)(param_1 + 0x24) == 0 || (*(int *)(lVar14 + 0x68) == 0)))) ||
      ((DAT_00215a70 = (int)DAT_0020f638, bVar1 = DAT_00215a70 == 0, bVar1 ||
       ((_DAT_0020f658 != *(long *)(lVar14 + 0x28) || (DAT_0020f660 != *(long *)(lVar14 + 0x38))))))
      ) || (DAT_00209cd8 == 0)) {
LAB_0015e0d8:
    FUN_001a278c(DAT_00215a50,0,&DAT_00228378);
    FUN_001bae8c(DAT_00215a58,0,&DAT_002209b8);
  }
  else {
    iVar5 = gettid();
    uVar11 = (undefined4)((ulong)in_stack_ffffffffffffeed0 >> 0x20);
    if (iVar4 != iVar5) goto LAB_0015e0d8;
    iVar4 = (*DAT_0020d160)(&DAT_00215b48);
    uVar11 = (undefined4)((ulong)in_stack_ffffffffffffeed0 >> 0x20);
    if (iVar4 != 1) goto LAB_0015e0d8;
    lVar14 = *(long *)(param_1 + 0x10);
    local_d4c = 0;
    DAT_00220928 = 2;
    uVar12 = FUN_00189c88(DAT_001e0978,*(undefined8 *)(lVar14 + 0x40),FUN_001428fc,0,auStack_d48);
    uVar11 = (undefined4)((ulong)in_stack_ffffffffffffeed0 >> 0x20);
    if (((int)uVar12 == 0) || (local_d38 != *(int *)(lVar14 + 0x70))) goto LAB_0015e0d8;
    if (DAT_00215bc8 == 5) {
      if (*(ulong *)(lVar14 + 0x40) < 0xfffffffffffffdb4) {
        iVar4 = FUN_001428fc(uVar12,*(ulong *)(lVar14 + 0x40) + 0x24c,&local_d4c,4);
        uVar11 = (undefined4)((ulong)in_stack_ffffffffffffeed0 >> 0x20);
        if (((iVar4 != 0) && (99 < local_d4c)) && (local_d4c < 0x7d1)) goto LAB_0015dcc4;
      }
      goto LAB_0015e0d8;
    }
LAB_0015dcc4:
    iVar4 = FUN_0013a78c(*(long *)(lVar14 + 0x30) + 0x58,&local_d58);
    uVar11 = (undefined4)((ulong)in_stack_ffffffffffffeed0 >> 0x20);
    if (iVar4 == 0) goto LAB_0015e0d8;
    iVar4 = FUN_0013a78c(*(long *)(lVar14 + 0x38) + 0x28,&local_d60);
    uVar11 = (undefined4)((ulong)in_stack_ffffffffffffeed0 >> 0x20);
    if (iVar4 == 0) goto LAB_0015e0d8;
    uVar12 = FUN_0013a78c(local_d60,&local_d68);
    uVar11 = (undefined4)((ulong)in_stack_ffffffffffffeed0 >> 0x20);
    if ((int)uVar12 == 0) goto LAB_0015e0d8;
    uVar12 = FUN_001428fc(uVar12,local_d60 + 0xc,&local_d6c,4);
    uVar11 = (undefined4)((ulong)in_stack_ffffffffffffeed0 >> 0x20);
    if ((((int)uVar12 == 0) || ((int)local_d6c < 1)) || (0x100 < (int)local_d6c)) goto LAB_0015e0d8;
    iVar4 = FUN_001428fc(uVar12,local_d68,&DAT_00221778,(ulong)local_d6c << 3);
    uVar9 = local_d4c;
    uVar16 = DAT_00215b50;
    uVar12 = DAT_00215a88;
    uVar11 = (undefined4)((ulong)in_stack_ffffffffffffeed0 >> 0x20);
    if (iVar4 == 0) goto LAB_0015e0d8;
    uVar33 = DAT_00215bc8;
    uVar8 = DAT_00215bcc;
    _DAT_00215c38 = CONCAT44(DAT_00215bec._4_4_,(int)DAT_00215bec);
    _DAT_00215c10 = CONCAT44(DAT_00215be8,DAT_00215bcc);
    uRam0000000000215c28 = uRam0000000000215bd8;
    _DAT_00215c20 = _DAT_00215bd0;
    uVar20 = uRam0000000000215b68 & 0x3f400000000;
    _DAT_00215c40 =
         CONCAT44(DAT_00215c40_4,
                  DAT_00215bb0 << 1 | DAT_00215bc0 << 2 | DAT_00215bf4 << 3 | DAT_00215bf8 << 4 |
                  DAT_00215bb8);
    uVar32 = DAT_00215a74;
    DAT_00215c30 = DAT_00215be0;
    uVar11 = *(undefined4 *)(lVar14 + 0x78);
    uVar17 = *(undefined8 *)(lVar14 + 0x20);
    DAT_00215c00 = DAT_0010e540;
    uVar29 = *(undefined8 *)(lVar14 + 0x30);
    uVar27 = *(undefined8 *)(lVar14 + 0x28);
    uVar18 = *(undefined8 *)(param_1 + 0x28);
    uVar26 = *(undefined8 *)(lVar14 + 0x40);
    uVar28 = *(undefined8 *)(lVar14 + 0x38);
    uVar30 = *(undefined8 *)(lVar14 + 0x70);
    _DAT_00215c08 = CONCAT44(DAT_00215bc8,DAT_00215bc4);
    DAT_00215c18 = DAT_00215b50;
    if ((((DAT_00215bc4 == 0) || (((byte)DAT_00215b70 >> 2 & 1) == 0)) &&
        ((((int)DAT_00215bec == 0 || (((byte)DAT_00215b70 >> 4 & 1) == 0)) &&
         ((DAT_00215bb8 == 0 || (((byte)DAT_00215b70 >> 5 & 1) == 0)))))) &&
       (((DAT_00215bb0 == 0 || (((byte)DAT_00215b70 >> 6 & 1) == 0)) &&
        (((DAT_00215bc0 == 0 || (-1 < (char)(byte)DAT_00215b70)) &&
         ((DAT_00215bf4 == 0 || ((DAT_00215b70 >> 8 & 1) == 0)))))))) {
      uVar7 = (uint)((_DAT_00215b70 & 0x200) == 0 || DAT_00215bf8 == 0);
    }
    else {
      uVar7 = 0;
    }
    uVar6 = FUN_0015923c(&DAT_00215b48);
    uVar3 = DAT_0010e7a8;
    _DAT_00215aa8 = CONCAT44(uVar32,1);
    _DAT_00215ab0 = CONCAT44(uVar9,uVar33);
    DAT_00215ab8 = uVar12;
    DAT_00215af8 = local_d58;
    DAT_00215ac8 = uVar16;
    _DAT_00215b28 = _DAT_00215b28 & 0xffffffff00000000;
    DAT_00215b30 = 0;
    _DAT_00215b38 = (ulong)uVar8;
    DAT_00215b40 = CONCAT44(DAT_00215b40._4_4_,uVar6);
    DAT_00215b14 = (uint)_UNK_00112a98;
    uRam0000000000215b18 = (undefined4)((ulong)_UNK_00112a98 >> 0x20);
    DAT_00215b0c = (int)_DAT_00112a90;
    DAT_00215b10 = (uint)((ulong)_DAT_00112a90 >> 0x20);
    local_de8 = DAT_0010e7a8;
    DAT_00215b1c = local_d1c;
    _DAT_00215b20 = CONCAT44(uVar7,(uint)(uVar20 != 0));
    DAT_00215aa0 = DAT_0010e670;
    local_d78 = *(undefined4 *)(lVar14 + 0x70);
    uStack_d98 = *(undefined8 *)(lVar14 + 0x28);
    local_da0 = *(undefined8 *)(lVar14 + 0x20);
    local_d90 = *(undefined8 *)(lVar14 + 0x30);
    uStack_d88 = *(undefined8 *)(lVar14 + 0x38);
    local_db0 = DAT_0010e7f8;
    local_da8 = *(undefined8 *)(*(long *)(param_1 + 8) + 8);
    local_d80 = *(undefined8 *)(lVar14 + 0x40);
    uStack_dd8 = 0;
    local_de0 = 0;
    uStack_dc8 = 0;
    local_dd0 = 0;
    uStack_db8 = 0;
    local_dc0 = 0;
    DAT_00215ac0 = uVar17;
    DAT_00215ad0 = uVar18;
    DAT_00215ad8 = uVar27;
    DAT_00215ae0 = uVar29;
    DAT_00215ae8 = uVar28;
    DAT_00215af0 = uVar26;
    _DAT_00215b00 = uVar30;
    DAT_00215b04._4_4_ = uVar11;
    if ((DAT_0020adec == '\x01') &&
       (iVar4 = FUN_0018a8cc(&DAT_0020adf0,&local_db0,&local_de8), iVar4 == 1)) {
      DAT_00215b0c = (int)local_de0;
      DAT_00215b10 = (uint)((ulong)local_de0 >> 0x20);
      DAT_00215b14 = (uint)uStack_db8;
    }
    _DAT_00215cb0 = *(undefined8 *)(lVar14 + 0x20);
    _DAT_00215cb8 = *(undefined8 *)(lVar14 + 0x28);
    _DAT_00215cc8 = *(undefined8 *)(lVar14 + 0x38);
    _DAT_00215cc0 = *(undefined8 *)(lVar14 + 0x30);
    DAT_00215cd0 = *(undefined8 *)(lVar14 + 0x40);
    _DAT_00215ca0 = CONCAT44(DAT_00215a74,1);
    DAT_00215ca8 = DAT_00215a88;
    DAT_00215cd8 = *(undefined8 *)(lVar14 + 0x50);
    DAT_00215c98 = DAT_0010e800;
    DAT_00215ce0 = local_d60;
    DAT_00215ce8 = local_d68;
    _DAT_00215cf0 = CONCAT44(local_d6c,*(undefined4 *)(lVar14 + 0x70));
    DAT_00215cf8 = DAT_0010e740;
    DAT_00215d00 = &DAT_00221778;
    if (((DAT_00215c08 != 0) || (DAT_00215c38 != 0)) || (DAT_00215c40 != 0)) {
      FUN_00161098(*(undefined8 *)(lVar14 + 0x38),*(undefined8 *)(lVar14 + 0x50),DAT_00215a74,1);
    }
    uVar11 = (undefined4)((ulong)in_stack_ffffffffffffeed0 >> 0x20);
    if ((DAT_0020d15c == '\x01') && (DAT_00215d48 == *(long *)(lVar14 + 0x50))) {
      DAT_00215d74 = *(undefined4 *)(lVar14 + 0x80);
      DAT_00215d6c = 1;
      _DAT_00215d70 = 1;
      _DAT_00215d60 = CONCAT44(DAT_0020f7f4,DAT_0020f7f0);
      DAT_00215d50 = DAT_0010e668;
      DAT_00215d58 = &DAT_0020f7f8;
      DAT_00215d68 = DAT_0020f7f4 * DAT_0020f7f0;
    }
    DAT_00220928 = 3;
    if ((int)local_d6c < 1) {
      local_e34 = 0;
      uVar9 = 0;
      iVar4 = 1;
    }
    else {
      uVar20 = 0;
      uVar8 = 0;
      uVar9 = 0;
      iVar5 = 1;
      do {
        uVar11 = (undefined4)uVar20;
        uVar21 = (&DAT_00221778)[uVar20];
        DAT_0022092c = uVar11;
        if ((uVar21 + 0x2000 < 0x12000 || (uVar21 & 7) != 0) ||
           (iVar4 = FUN_0013a78c(uVar21,local_d08), iVar4 == 0)) goto LAB_0015ea14;
        iVar4 = iVar5;
        if (uVar21 != *(ulong *)(lVar14 + 0x40)) {
          if (local_d08[0].tv_sec == DAT_001e0978 + 0x12206a8) {
            uVar23 = (ulong)uVar8;
            iVar10 = FUN_001a20b0(DAT_001e0978,uVar21,FUN_001428fc,0,
                                  &DAT_00221f78 + (ulong)uVar8 * 0xb);
            (&DAT_00221f90)[(ulong)uVar8 * 0x16] = uVar11;
            iVar4 = 0;
            if (iVar10 != 0) {
              iVar4 = iVar5;
            }
            if (*(int *)(param_1 + 0x24) != 0) {
              uVar7 = *(uint *)(param_1 + 0x20);
              uVar21 = (ulong)uVar7;
              if (((uVar7 < 0x101) && (*(long *)(param_1 + 0x18) != 0)) && (uVar7 != 0)) {
                piVar15 = (int *)(*(long *)(param_1 + 0x18) + 0x10);
                do {
                  if ((*(long *)(piVar15 + -4) == (&DAT_00221f78)[(ulong)uVar8 * 0xb]) &&
                     (piVar15[-2] == (&DAT_00221f88)[uVar23 * 0x16])) {
                    if (((piVar15[-1] == (&DAT_00221fa0)[uVar23 * 0x16]) &&
                        ((*piVar15 == (&DAT_00221fa4)[uVar23 * 0x16] &&
                         (piVar15[1] == (&DAT_00221f94)[uVar23 * 0x16])))) &&
                       ((iVar5 = piVar15[3], iVar5 - 1U >> 5 < 0x271 &&
                        ((uVar7 = piVar15[4], uVar7 < 0x7d1 &&
                         (iVar10 = *(int *)(lVar14 + 0x80), iVar10 - 1U < 2000)))))) {
                      (&DAT_00221fb8)[uVar23 * 0x16] = 1;
                      (&DAT_00221fbc)[uVar23 * 0x16] = iVar5;
                      (&DAT_00221fc0)[uVar23 * 0x16] = uVar7;
                      (&DAT_00221fc4)[uVar23 * 0x16] = iVar10;
                    }
                    break;
                  }
                  uVar21 = uVar21 - 1;
                  piVar15 = piVar15 + 10;
                } while (uVar21 != 0);
              }
            }
            if ((DAT_0020d15c == '\x01') && (DAT_00215d48 == *(long *)(lVar14 + 0x50))) {
              uVar11 = FUN_00186cf0((float)(int)(&DAT_00221fa0)[uVar23 * 0x16] / 300.0,
                                    (float)(int)(&DAT_00221fa4)[uVar23 * 0x16] / 300.0,
                                    (float)*(int *)(lVar14 + 0x74) / 300.0,
                                    (float)*(int *)(lVar14 + 0x78) / 300.0,DAT_0020f7f0,DAT_0020f7f4
                                    ,FUN_00150fa0,0,0x40);
            }
            else {
              uVar11 = 0xffffffff;
            }
            uVar8 = uVar8 + 1;
            (&DAT_00221fc8)[uVar23 * 0x16] = uVar11;
          }
          else if ((local_d08[0].tv_sec == DAT_001e0978 + 0x1220040) ||
                  (local_d08[0].tv_sec == DAT_001e0978 + 0x12200a0)) {
            if (uVar9 == 0x30) goto LAB_0015ea14;
            iVar10 = FUN_001bb808(DAT_001e0978,uVar21,uVar20 & 0xffffffff,FUN_001428fc,0,
                                  &DAT_00227778 + (ulong)uVar9 * 0x40);
            uVar9 = uVar9 + 1;
            iVar4 = 0;
            if (iVar10 != 0) {
              iVar4 = iVar5;
            }
          }
          else if ((local_d08[0].tv_sec != DAT_001e0978 + 0x121fff8) &&
                  (local_d08[0].tv_sec != DAT_001e0978 + 0x12205d0)) {
LAB_0015ea14:
            iVar4 = 0;
            break;
          }
        }
        uVar20 = uVar20 + 1;
        iVar5 = iVar4;
      } while ((long)uVar20 < (long)(int)local_d6c);
      uVar11 = (undefined4)((ulong)in_stack_ffffffffffffeed0 >> 0x20);
      local_e34 = uVar9;
      if (uVar8 == 0) {
        uVar9 = 0;
      }
      else {
        uVar20 = 0;
        uVar9 = 0;
        puVar24 = &DAT_00221f80;
        do {
          iVar5 = FUN_00166d24(*puVar24);
          uVar11 = (undefined4)((ulong)in_stack_ffffffffffffeed0 >> 0x20);
          if (iVar5 == 0) {
            uVar21 = (ulong)uVar9;
            if (uVar20 != uVar21) {
              uVar12 = puVar24[-1];
              uVar16 = puVar24[1];
              uVar28 = puVar24[2];
              (&DAT_00221f80)[uVar21 * 0xb] = *puVar24;
              (&DAT_00221f78)[uVar21 * 0xb] = uVar12;
              *(undefined8 *)(&DAT_00221f90 + uVar21 * 0x16) = uVar28;
              *(undefined8 *)(&DAT_00221f88 + uVar21 * 0x16) = uVar16;
              uVar12 = puVar24[5];
              auVar25 = *(undefined1 (*) [16])(puVar24 + 7);
              uVar16 = puVar24[9];
              uVar17 = puVar24[4];
              uVar28 = puVar24[3];
              *(undefined8 *)(uVar21 * 0x58 + 0x221fb0) = puVar24[6];
              *(undefined8 *)(&DAT_00221fa8 + uVar21 * 0x58) = uVar12;
              *(long *)(&DAT_00221fc0 + uVar21 * 0x16) = auVar25._8_8_;
              *(long *)(&DAT_00221fb8 + uVar21 * 0x16) = auVar25._0_8_;
              *(undefined8 *)(&DAT_00221fc8 + uVar21 * 0x16) = uVar16;
              *(undefined8 *)(&DAT_00221fa0 + uVar21 * 0x16) = uVar17;
              *(undefined8 *)(&DAT_00221f98 + uVar21 * 0x16) = uVar28;
            }
            uVar9 = uVar9 + 1;
          }
          uVar20 = uVar20 + 1;
          puVar24 = puVar24 + 0xb;
        } while (uVar8 != uVar20);
      }
    }
    local_e28 = *(undefined8 *)(lVar14 + 0x20);
    local_e18 = *(undefined8 *)(lVar14 + 0x28);
    local_e40 = DAT_0010e700;
    auVar25 = NEON_ext(*(undefined1 (*) [16])(lVar14 + 0x30),*(undefined1 (*) [16])(lVar14 + 0x30),8
                       ,1);
    local_e00 = *(undefined8 *)(lVar14 + 0x40);
    local_e30 = &DAT_00215a68;
    uStack_e08 = auVar25._8_8_;
    local_e10 = auVar25._0_8_;
    local_e20 = DAT_00215b50;
    local_df0 = &DAT_00227778;
    local_e38 = iVar4;
    FUN_001bae8c(DAT_00215a58,&local_e40,&DAT_002209b8);
    uRam0000000000220968 = *(undefined8 *)(lVar14 + 0x30);
    _DAT_00220960 = *(undefined8 *)(lVar14 + 0x28);
    _DAT_00220938 = CONCAT44(iVar4,(int)_DAT_00215c08);
    DAT_00220950 = *(undefined8 *)(lVar14 + 0x20);
    DAT_00220930 = DAT_0010e678;
    _DAT_00220970 = *(undefined8 *)(lVar14 + 0x38);
    uRam0000000000220978 = *(undefined8 *)(lVar14 + 0x40);
    DAT_00220948 = &DAT_00215a68;
    DAT_00220990 = *(undefined8 *)(lVar14 + 0x74);
    DAT_00220958 = DAT_00215b50;
    _DAT_00220980 = CONCAT44(uStack_d34,local_d38);
    DAT_00220998 = &DAT_00221f78;
    DAT_002209a8 = &DAT_002209f8;
    DAT_00220928 = 0;
    if (DAT_002209c0 == 0 || iVar4 == 0) {
      DAT_00220928 = 4;
    }
    _DAT_00220940 = CONCAT44(1,DAT_002209c0);
    uRam0000000000220988 = uStack_d30;
    _DAT_002209a0 = CONCAT44(DAT_002209a0_4,uVar9);
    DAT_002209b0 = CONCAT44(DAT_002209b0._4_4_,DAT_002209c8);
    DAT_00215a64 = 1;
    uStack_f48 = 0;
    local_f50 = 0;
    uStack_f38 = 0;
    uStack_f40 = 0;
    uStack_f28 = 0;
    local_f30 = 0;
    uStack_f18 = 0;
    uStack_f20 = 0;
    uStack_f08 = 0;
    local_f10 = 0;
    uStack_ef8 = 0;
    local_f00 = 0;
    uStack_ee8 = 0;
    uStack_ef0 = 0;
    uStack_ed8 = 0;
    local_ee0 = 0;
    uStack_ec8 = 0;
    uStack_ed0 = 0;
    uStack_eb8 = 0;
    local_ec0 = 0;
    uStack_ea8 = 0;
    uStack_eb0 = 0;
    uStack_e98 = 0;
    local_ea0 = 0;
    uStack_e88 = 0;
    uStack_e90 = 0;
    uStack_e78 = 0;
    local_e80 = 0;
    uStack_e68 = 0;
    uStack_e70 = 0;
    uStack_e58 = 0;
    local_e60 = 0;
    uStack_e48 = 0;
    uStack_e50 = 0;
    FUN_00167544(&local_f50);
    local_f80 = &DAT_00215a68;
    puStack_f78 = &DAT_00215aa0;
    local_f88 = uVar3;
    local_f60 = &DAT_00220930;
    local_f70 = &DAT_00215c00;
    puStack_f68 = &DAT_00215c98;
    DAT_0020d158 = 2;
    puStack_f58 = &local_f50;
    FUN_001a278c(DAT_00215a50,&local_f88,&DAT_00228378);
    DAT_0020d158 = 0;
    DAT_00215a64 = 0;
  }
  DAT_00215c90 = 0;
  iVar4 = clock_gettime(1,local_d08);
  if (iVar4 == 0) {
    lVar14 = local_d08[0].tv_sec * 1000000 + (ulong)local_d08[0].tv_nsec / 1000;
  }
  else {
    lVar14 = 0;
  }
  DAT_00228578 = lVar14 - lVar22;
  if (param_1 == 0) {
    iVar4 = clock_gettime(1,local_d08);
    if (iVar4 == 0) {
      lVar22 = local_d08[0].tv_sec * 1000 + (ulong)local_d08[0].tv_nsec / 1000000;
    }
    else {
      lVar22 = 0;
    }
  }
  else {
    lVar22 = *(long *)(param_1 + 0x28);
  }
  bVar1 = DAT_00228380 != DAT_002287c4 || DAT_00215b14 != DAT_001cfbe0;
  if ((((DAT_00220920 < 9) || (bVar1)) || (DAT_002287c8 == 0)) ||
     (999 < (ulong)(lVar22 - DAT_002287c8))) {
    uVar27 = CONCAT44(uVar11,DAT_00215c0c);
    uVar8 = DAT_00215b14;
    uVar11 = DAT_00215b00;
    uVar32 = (undefined4)DAT_00215b04;
    uVar33 = DAT_00215b04._4_4_;
    uVar6 = DAT_00220928;
    uVar34 = DAT_0022092c;
    uVar35 = DAT_00215cf4;
    uVar36 = DAT_002209a0;
    uVar37 = DAT_002209c8;
    iVar4 = DAT_002209c0;
    uVar38 = DAT_00228384;
    uVar39 = DAT_00228388;
    uVar40 = DAT_002283d4;
    uVar41 = DAT_002283d8;
    uVar42 = DAT_00228448;
    uVar43 = DAT_0022844c;
    iVar5 = DAT_00228380;
    uVar44 = DAT_00228480;
    uVar45 = DAT_00228484;
    uVar46 = DAT_002284c8;
    uVar47 = DAT_002284e0;
    uVar48 = DAT_00228530;
    uVar49 = DAT_00228534;
    uVar12 = DAT_002283b8;
    uVar16 = DAT_002283c0;
    uVar50 = DAT_00228570;
    uVar51 = DAT_00228574;
    uVar28 = DAT_00215c48;
    uVar17 = DAT_00215c50;
    uVar18 = DAT_00215c58;
    uVar26 = DAT_00215c60;
    lVar14 = DAT_00228578;
    snprintf((char *)local_d08,0xc80,
             ",\"action\":22,\"publication\":%llu,\"source_tick\":%u,\"epoch\":%llu,\"settings_generation\":%llu,\"requested\":%u,\"version\":%u,\"active_known\":%u,\"active\":%u,\"ended\":%u,\"own_gid\":%d,\"own_x\":%d,\"own_y\":%d,\"collect_stage\":%u,\"collect_index\":%u,\"object_count\":%u,\"projectile_count\":%u,\"actor_count\":%u,\"actors_complete\":%u,\"coverage_known\":%u,\"wall_known\":%u,\"poison_known\":%u,\"hazard_known\":%u,\"classifier_ready\":%u,\"classifier_reason\":%u,\"consumer_reason\":%u,\"proposal_kind\":%u,\"proposal_reason\":%u,\"live_threats\":%u,\"begin_reason\":%u,\"apply_reason\":%u,\"pending\":%u,\"local_calls\":%llu,\"queue_calls\":%llu,\"target_x\":%d,\"target_y\":%d,\"local_permission_calls\":%llu,\"queue_permission_calls\":%llu,\"local_permission_refusals\":%llu,\"queue_permission_refusals\":%llu,\"duration_us\":%llu,\"native_acceptance_proven\":false"
             ,DAT_00215a88,_DAT_00215a70 >> 0x20,DAT_00215ac0,DAT_00215b50,
             _DAT_00215c08 & 0xffffffff,uVar27,DAT_00215b0c,DAT_00215b10,DAT_00215b14,DAT_00215b00,
             (undefined4)DAT_00215b04,DAT_00215b04._4_4_,DAT_00220928,DAT_0022092c,DAT_00215cf4,
             DAT_002209a0,DAT_002209c8,DAT_002209c0,DAT_00228384,DAT_00228388,DAT_002283d4,
             DAT_002283d8,DAT_00228448,DAT_0022844c,DAT_00228380,DAT_00228480,DAT_00228484,
             DAT_002284c8,DAT_002284e0,DAT_00228530,DAT_00228534,DAT_002283b8,DAT_002283c0,
             DAT_00228570,DAT_00228574,DAT_00215c48,DAT_00215c50,DAT_00215c58,DAT_00215c60,
             DAT_00228578);
    uVar31 = (undefined4)((ulong)uVar27 >> 0x20);
    sVar13 = strlen((char *)local_d08);
    uVar9 = DAT_002209a0;
    if (3 < DAT_002209a0) {
      uVar9 = 4;
    }
    uVar7 = snprintf((char *)((long)&local_d08[0].tv_sec + sVar13),0xc80 - sVar13,
                     ",\"projectiles_omitted\":%u,\"raw_projectiles\":[",
                     (ulong)(DAT_002209a0 - uVar9));
    if (((int)uVar7 < 0) || (0xc80 - sVar13 <= (ulong)uVar7)) goto LAB_0015e540;
    lVar19 = sVar13 + uVar7;
    if (uVar9 != 0) {
      uVar27 = CONCAT44(uVar31,DAT_00221f9c);
      uVar7 = snprintf((char *)((long)&local_d08[0].tv_sec + lVar19),0xc80U - lVar19,
                       "%s{\"gid\":%u,\"known\":%u,\"z\":%u,\"team_a\":%u,\"team_b\":%u,\"bouncing\":%u,\"wall_los\":%d}"
                       ,&DAT_00117c20,(ulong)DAT_00221f88,(ulong)DAT_00221f8c,(ulong)DAT_00221f94,
                       (ulong)DAT_00221f98,uVar27,DAT_00221fb4,DAT_00221fc8,uVar8,uVar11,uVar32,
                       uVar33,uVar6,uVar34,uVar35,uVar36,uVar37,iVar4,uVar38,uVar39,uVar40,uVar41,
                       uVar42,uVar43,iVar5,uVar44,uVar45,uVar46,uVar47,uVar48,uVar49,uVar12,uVar16,
                       uVar50,uVar51,uVar28,uVar17,uVar18,uVar26,lVar14);
      if (((int)uVar7 < 0) || (0xc80U - lVar19 <= (ulong)uVar7)) goto LAB_0015e540;
      lVar19 = lVar19 + (ulong)uVar7;
      if (uVar9 != 1) {
        uVar27 = CONCAT44((int)((ulong)uVar27 >> 0x20),DAT_00221ff4);
        uVar8 = snprintf((char *)((long)&local_d08[0].tv_sec + lVar19),0xc80U - lVar19,
                         "%s{\"gid\":%u,\"known\":%u,\"z\":%u,\"team_a\":%u,\"team_b\":%u,\"bouncing\":%u,\"wall_los\":%d}"
                         ,&DAT_00120242,(ulong)DAT_00221fe0,(ulong)DAT_00221fe4,(ulong)DAT_00221fec,
                         (ulong)DAT_00221ff0,uVar27,DAT_0022200c,DAT_00222020,uVar8,uVar11,uVar32,
                         uVar33,uVar6,uVar34,uVar35,uVar36,uVar37,iVar4,uVar38,uVar39,uVar40,uVar41,
                         uVar42,uVar43,iVar5,uVar44,uVar45,uVar46,uVar47,uVar48,uVar49,uVar12,uVar16
                         ,uVar50,uVar51,uVar28,uVar17,uVar18,uVar26,lVar14);
        if (((int)uVar8 < 0) || (0xc80U - lVar19 <= (ulong)uVar8)) goto LAB_0015e540;
        lVar19 = lVar19 + (ulong)uVar8;
        if (uVar9 != 2) {
          uVar12 = CONCAT44((int)((ulong)uVar27 >> 0x20),DAT_0022204c);
          uVar8 = snprintf((char *)((long)&local_d08[0].tv_sec + lVar19),0xc80U - lVar19,
                           "%s{\"gid\":%u,\"known\":%u,\"z\":%u,\"team_a\":%u,\"team_b\":%u,\"bouncing\":%u,\"wall_los\":%d}"
                           ,&DAT_00120242,(ulong)DAT_00222038,(ulong)DAT_0022203c,
                           (ulong)DAT_00222044,(ulong)DAT_00222048,uVar12,DAT_00222064,DAT_00222078)
          ;
          if (((int)uVar8 < 0) || (0xc80U - lVar19 <= (ulong)uVar8)) goto LAB_0015e540;
          lVar19 = lVar19 + (ulong)uVar8;
          if (uVar9 != 3) {
            uVar9 = snprintf((char *)((long)&local_d08[0].tv_sec + lVar19),0xc80U - lVar19,
                             "%s{\"gid\":%u,\"known\":%u,\"z\":%u,\"team_a\":%u,\"team_b\":%u,\"bouncing\":%u,\"wall_los\":%d}"
                             ,&DAT_00120242,(ulong)DAT_00222090,(ulong)DAT_00222094,
                             (ulong)DAT_0022209c,(ulong)DAT_002220a0,
                             CONCAT44((int)((ulong)uVar12 >> 0x20),DAT_002220a4),DAT_002220bc,
                             DAT_002220d0);
            if (((int)uVar9 < 0) || (0xc80U - lVar19 <= (ulong)uVar9)) goto LAB_0015e540;
            lVar19 = lVar19 + (ulong)uVar9;
          }
        }
      }
    }
    if (lVar19 - 0xc7eU < 0xfffffffffffff380) goto LAB_0015e540;
    *(undefined2 *)((long)&local_d08[0].tv_sec + lVar19) = 0x5d;
    FUN_001417c8("dodge_frame","single_consumer_frame",local_d08);
    DAT_002287c8 = lVar22;
  }
  if ((DAT_00215b0c != 0) &&
     ((DAT_002287d0 == 0 || bVar1 || (0x270 < (ulong)(lVar22 - DAT_002287d0) >> 3)))) {
    snprintf((char *)local_d08,0x14a,
             ",\"action\":22,\"publication\":%llu,\"epoch\":%llu,\"active\":%u,\"ended\":%u,\"local_calls\":%llu,\"queue_calls\":%llu"
             ,DAT_00215a88,DAT_00215ac0,(ulong)DAT_00215b10,(ulong)DAT_00215b14,DAT_002283b8,
             DAT_002283c0);
    FUN_001417c8("dodge_phase","phase_and_cumulative_operations",local_d08);
    DAT_002287d0 = lVar22;
  }
  DAT_002287c4 = DAT_00228380;
  DAT_001cfbe0 = DAT_00215b14;
LAB_0015e540:
  if (*(long *)(lVar2 + 0x28) == local_88) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00166d24 @ 00166d24 [libNexusEvasionRuntime69252.so] ===== */

void FUN_00166d24(long param_1)

{
  ulong uVar1;
  undefined *puVar2;
  uint uVar3;
  byte bVar4;
  char cVar5;
  long lVar6;
  long lVar7;
  __mode_t _Var8;
  bool bVar9;
  int iVar10;
  uint uVar11;
  __uid_t _Var12;
  uint uVar13;
  undefined8 uVar14;
  int *piVar15;
  undefined8 *puVar16;
  long lVar17;
  int iVar18;
  ulong uVar19;
  ulong uVar20;
  long *plVar21;
  int local_14c;
  ulong local_148;
  ulong local_140;
  ulong local_138;
  ulong local_130;
  undefined8 uStack_128;
  int *local_120;
  undefined8 uStack_118;
  int local_110;
  uint local_10c;
  int local_108;
  uint uStack_104;
  uint local_100;
  uint local_fc;
  uint local_f8;
  uint local_f4;
  stat local_f0;
  
  lVar6 = tpidr_el0;
  local_f0.__unused[2] = *(long *)(lVar6 + 0x28);
  iVar10 = clock_gettime(1,(timespec *)&local_f0);
  if (iVar10 == 0) {
    lVar7 = CONCAT44(DAT_00228590._4_4_,(undefined4)DAT_00228590);
    lVar17 = local_f0.st_dev * 1000 + local_f0.st_ino / 1000000;
    if ((lVar17 != 0) && ((lVar7 == 0 || (0xf9 < (ulong)(lVar17 - lVar7))))) {
      do {
        if (DAT_00228590 != lVar7) {
          ClearExclusiveLocal();
          goto LAB_00166d78;
        }
        cVar5 = '\x01';
        bVar9 = (bool)ExclusiveMonitorPass(0x228590,0x10);
        if (bVar9) {
          cVar5 = ExclusiveMonitorsStatus();
          DAT_00228590 = lVar17;
        }
      } while (cVar5 != '\0');
      iVar10 = DAT_001cfb50;
      if (-1 < DAT_001cfb50) {
        iVar10 = openat(DAT_001cfb50,"nexus-menu",0x8c000);
        if (-1 < iVar10) {
          uVar11 = openat(iVar10,"autododge-ignore.bin",0x88000);
          iVar10 = close(iVar10);
          if (-1 < (int)uVar11) {
            iVar10 = fstat(uVar11,&local_f0);
            if ((iVar10 == 0) && (((uint)local_f0.st_nlink & 0xf000) == 0x8000)) {
              _Var8 = local_f0.st_mode;
              _Var12 = getuid();
              if ((_Var8 == _Var12) &&
                 ((local_f0.st_nlink._4_4_ == 1 && (local_f0.st_size == 0x20)))) {
                piVar15 = (int *)__errno();
                iVar10 = *piVar15;
                puVar16 = (undefined8 *)FUN_001bd828(&DAT_001cfcd8);
                puVar2 = &DAT_00116561;
                if (uVar11 != 0x7fff0001) {
                  puVar2 = &DAT_00117c1b;
                }
                puVar16[5] = 0;
                puVar16[6] = 0;
                *puVar16 = puVar2;
                puVar16[1] = &DAT_00116571;
                local_120 = &local_110;
                plVar21 = puVar16 + 4;
                *plVar21 = 0;
                puVar16[2] = &DAT_0011a7d3;
                puVar16[3] = 0x20;
                uStack_118 = 0x20;
                local_130 = 0;
                uStack_128 = 0x20;
                if (uVar11 != 0x7fff0001) {
                  lVar17 = syscall(0x43,(ulong)uVar11,&local_110,0x20,0);
                }
                else {
                  uVar13 = getpid();
                  lVar17 = syscall(0x10e,(ulong)uVar13,&local_120,1,&local_130,1,0);
                }
                if (lVar17 < 0) {
                  iVar18 = *piVar15;
                  if (iVar18 == 4) {
                    if (uVar11 == 0x7fff0001) {
                      uVar13 = getpid();
                      lVar17 = syscall(0x10e,(ulong)uVar13,&local_120,1,&local_130,1,0);
                    }
                    else {
                      lVar17 = syscall(0x43,(ulong)uVar11,&local_110,0x20,0);
                    }
                    if (-1 < lVar17) goto LAB_001670b4;
                    iVar18 = *piVar15;
                    if (iVar18 != 4) goto LAB_0016745c;
                    if (uVar11 == 0x7fff0001) {
                      uVar13 = getpid();
                      lVar17 = syscall(0x10e,(ulong)uVar13,&local_120,1,&local_130,1,0);
                    }
                    else {
                      lVar17 = syscall(0x43,(ulong)uVar11,&local_110,0x20,0);
                    }
                    if (-1 < lVar17) goto LAB_001670b4;
                    iVar18 = *piVar15;
                    if (iVar18 != 4) goto LAB_0016745c;
                    if (uVar11 == 0x7fff0001) {
                      uVar13 = getpid();
                      lVar17 = syscall(0x10e,(ulong)uVar13,&local_120,1,&local_130,1,0);
                    }
                    else {
                      lVar17 = syscall(0x43,(ulong)uVar11,&local_110,0x20,0);
                    }
                    if (-1 < lVar17) goto LAB_001670b4;
                    iVar18 = *piVar15;
                    if (iVar18 != 4) goto LAB_0016745c;
                    if (uVar11 == 0x7fff0001) {
                      uVar13 = getpid();
                      lVar17 = syscall(0x10e,(ulong)uVar13,&local_120,1,&local_130,1,0);
                    }
                    else {
                      lVar17 = syscall(0x43,(ulong)uVar11,&local_110,0x20,0);
                    }
                    if (-1 < lVar17) goto LAB_001670b4;
                    iVar18 = *piVar15;
                    if (iVar18 != 4) goto LAB_0016745c;
                    if (uVar11 == 0x7fff0001) {
                      uVar13 = getpid();
                      lVar17 = syscall(0x10e,(ulong)uVar13,&local_120,1,&local_130,1,0);
                    }
                    else {
                      lVar17 = syscall(0x43,(ulong)uVar11,&local_110,0x20,0);
                    }
                    if (-1 < lVar17) goto LAB_001670b4;
                    iVar18 = *piVar15;
                    if (iVar18 != 4) goto LAB_0016745c;
                    if (uVar11 == 0x7fff0001) {
                      uVar13 = getpid();
                      lVar17 = syscall(0x10e,(ulong)uVar13,&local_120,1,&local_130,1,0);
                    }
                    else {
                      lVar17 = syscall(0x43,(ulong)uVar11,&local_110,0x20,0);
                    }
                    if (-1 < lVar17) goto LAB_001670b4;
                    iVar18 = *piVar15;
                    if (iVar18 != 4) goto LAB_0016745c;
                    if (uVar11 == 0x7fff0001) {
                      uVar13 = getpid();
                      lVar17 = syscall(0x10e,(ulong)uVar13,&local_120,1,&local_130,1,0);
                    }
                    else {
                      lVar17 = syscall(0x43,(ulong)uVar11,&local_110,0x20,0);
                    }
                    *plVar21 = lVar17;
                    uVar13 = local_f8;
                    goto joined_r0x00167488;
                  }
LAB_0016745c:
                  *plVar21 = lVar17;
                }
                else {
LAB_001670b4:
                  *plVar21 = lVar17;
                  uVar13 = local_f8;
joined_r0x00167488:
                  if (-1 < lVar17) {
                    *(undefined4 *)(puVar16 + 6) = 0;
                    *piVar15 = iVar10;
                    if ((((lVar17 == 0x20) && (local_110 == 0x3149524e)) && (local_10c == 1)) &&
                       ((local_108 == 0x6c &&
                        (uVar3 = local_100 ^
                                 ((uStack_104 ^ 0x3b7d4c22) >> 0x1b | (uStack_104 ^ 0x3b7d4c22) << 5
                                 ), uVar3 = local_fc ^ (uVar3 >> 0x1b | uVar3 << 5),
                        local_f4 == (uVar13 ^ (uVar3 >> 0x1b | uVar3 << 5)))))) {
                      local_f8 = uVar13;
                      iVar10 = close(uVar11);
                      if (uVar13 < 0x1000) {
                        DAT_00228580._0_4_ = uStack_104;
                        DAT_00228580._4_4_ = local_100;
                        DAT_00228588._0_4_ = local_fc;
                        DAT_00228588._4_4_ = local_f8;
                      }
                      goto LAB_00166d78;
                    }
                    goto LAB_00167534;
                  }
                  iVar18 = *piVar15;
                }
                *(int *)(puVar16 + 6) = iVar18;
              }
            }
LAB_00167534:
            iVar10 = close(uVar11);
          }
        }
      }
    }
  }
LAB_00166d78:
  if ((DAT_00228588._4_4_ != 0 || (uint)DAT_00228588 != 0) ||
      (DAT_00228580._4_4_ != 0 || (uint)DAT_00228580 != 0)) {
    local_f0.st_ino = 0;
    local_f0.st_dev = 0;
    local_f0.st_mode = 0;
    local_f0.st_uid = 0;
    local_f0.st_nlink = 0;
    local_f0.st_rdev = 0;
    local_f0.st_gid = 0;
    local_f0.__pad0 = 0;
    local_f0.st_blksize = 0;
    local_f0.st_size = 0;
    uVar14 = FUN_001428fc(iVar10,param_1 + 8,&local_120,8);
    if ((((int)uVar14 != 0) && ((int *)0x11fff < local_120 + 0x800)) &&
       (((ulong)local_120 & 7) == 0)) {
      uVar14 = FUN_001428fc(uVar14,local_120 + 2,&local_14c,4);
      if ((((int)uVar14 != 0) && (-1 < local_14c)) && (local_14c < 0x2711)) {
        uVar14 = FUN_001428fc(uVar14,local_120,&local_130,8);
        if ((((int)uVar14 != 0) && (0x11fff < local_130 + 0x2000)) && ((local_130 & 7) == 0)) {
          uVar14 = FUN_001428fc(uVar14,local_130 + 0x38,&local_138,8);
          if ((((int)uVar14 != 0) && (0x11fff < local_138 + 0x2000)) && ((local_138 & 7) == 0)) {
            uVar14 = FUN_001428fc(uVar14,local_138,&local_140,8);
            if ((((int)uVar14 != 0) && (0x11fff < local_140 + 0x2000)) && ((local_140 & 7) == 0)) {
              uVar14 = FUN_001428fc(uVar14,local_140 + 8,&local_148,8);
              if ((((int)uVar14 != 0) && (0x11fff < local_148 + 0x2000)) && ((local_148 & 7) == 0))
              {
                uVar14 = FUN_001428fc(uVar14,local_148 + (long)local_14c * 0x10,&local_110,0x10);
                if (((int)uVar14 != 0) && (uVar20 = (ulong)local_10c, 0xffffffc0 < local_10c - 0x40)
                   ) {
                  if (local_10c < 8) {
                    memcpy(&local_f0,&local_108,uVar20 + 1);
                  }
                  else {
                    iVar10 = FUN_001428fc(uVar14,CONCAT44(uStack_104,local_108),&local_f0,uVar20 + 1
                                         );
                    if (iVar10 == 0) goto LAB_00167188;
                  }
                  if (*(char *)((long)local_f0.__unused + (uVar20 - 0x78)) == '\0') {
                    uVar19 = 0;
                    bVar9 = false;
                    do {
                      bVar4 = *(byte *)((long)local_f0.__unused + (uVar19 - 0x78));
                      if ((0x19 < (bVar4 & 0xffffffdf) - 0x41) &&
                         (bVar4 != 0x5f && 9 < bVar4 - 0x30)) break;
                      uVar19 = uVar19 + 1;
                      bVar9 = uVar20 <= uVar19;
                    } while (uVar20 != uVar19);
                    if (bVar9) {
                      uVar20 = 0;
                      uVar19 = 0x13e;
                      do {
                        uVar1 = uVar20 + (uVar19 - uVar20 >> 1);
                        iVar10 = strcmp((char *)&local_f0,
                                        (&PTR_s_AlternatorHealProjectile_001c43b8)[uVar1 * 2]);
                        lVar17 = DAT_00228590;
                        if (-1 < iVar10) {
                          if (iVar10 == 0) {
                            uVar11 = (uint)*(undefined8 *)
                                            ((long)&DAT_00228580 +
                                            ((ulong)(*(ushort *)(&UNK_001c43c0 + uVar1 * 0x10) >> 3)
                                            & 0x1ffc)) >>
                                     (ulong)(*(ushort *)(&UNK_001c43c0 + uVar1 * 0x10) & 0x1f) & 1;
                            break;
                          }
                          uVar20 = uVar1 + 1;
                          uVar1 = uVar19;
                        }
                        uVar19 = uVar1;
                        uVar11 = 0;
                      } while (uVar20 < uVar19);
                      goto LAB_0016718c;
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
LAB_00167188:
  uVar11 = 0;
  lVar17 = DAT_00228590;
LAB_0016718c:
  DAT_00228590._4_4_ = (undefined4)((ulong)lVar17 >> 0x20);
  DAT_00228590._0_4_ = (undefined4)lVar17;
  if (*(long *)(lVar6 + 0x28) != local_f0.__unused[2]) {
    DAT_00228590 = lVar17;
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar11);
  }
  return;
}

/* ===== ng_feed_v1 @ 0018bddc [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void ng_feed_v1(int *param_1)

{
  char cVar1;
  long lVar2;
  undefined1 auVar3 [16];
  undefined1 auVar4 [16];
  char cVar5;
  bool bVar6;
  int iVar7;
  undefined4 uVar8;
  int iVar9;
  undefined8 uVar10;
  ulong uVar11;
  ulong uVar12;
  long lVar13;
  ulong uVar14;
  undefined1 (*pauVar15) [16];
  ulong uVar16;
  ulong *puVar17;
  uint uVar18;
  uint uVar19;
  uint uVar20;
  int iVar23;
  int iVar24;
  undefined1 auVar21 [16];
  undefined1 auVar22 [16];
  undefined1 auVar25 [16];
  undefined1 auVar26 [16];
  undefined1 auVar27 [16];
  undefined1 auVar28 [16];
  undefined1 auVar29 [16];
  undefined8 local_1c18;
  int local_1c04;
  byte local_1bf4 [4];
  ulong local_1bf0;
  int iStack_1be8;
  undefined4 uStack_1be4;
  undefined8 uStack_1be0;
  int local_1bd8;
  undefined4 local_1bd4;
  undefined4 uStack_1bd0;
  uint local_1bcc;
  uint local_1bc4;
  uint local_1bc0;
  uint local_1bbc;
  uint local_1bb8;
  int local_1bb4;
  ulong local_1bb0;
  ulong local_1ba8;
  long local_1ba0;
  long local_1b98;
  long local_1b90;
  ulong local_1b88;
  ulong local_1b80;
  ulong local_1b78;
  ulong local_1b70;
  ulong local_1b68;
  ulong local_1b60;
  long local_1b58;
  ulong auStack_1b50 [320];
  ulong local_1150 [4];
  undefined1 auStack_1130 [4064];
  undefined8 local_150;
  long lStack_148;
  long lStack_140;
  ulong local_138;
  long local_130;
  long lStack_128;
  ulong uStack_120;
  ulong local_118;
  long local_110;
  undefined8 uStack_108;
  ulong uStack_100;
  ulong local_f8;
  undefined8 local_f0;
  undefined8 uStack_e8;
  undefined8 uStack_e0;
  undefined8 uStack_d8;
  undefined8 local_d0;
  undefined8 uStack_c8;
  undefined8 local_c0;
  undefined8 uStack_b8;
  undefined8 local_b0;
  undefined8 uStack_a8;
  undefined8 uStack_a0;
  undefined4 uStack_98;
  undefined4 local_94;
  undefined4 uStack_90;
  undefined4 uStack_8c;
  undefined8 uStack_88;
  long local_78;
  
  lVar2 = tpidr_el0;
  local_78 = *(long *)(lVar2 + 0x28);
  do {
    cVar5 = DAT_002fe0a0;
    cVar1 = '\x01';
    bVar6 = (bool)ExclusiveMonitorPass(0x2fe0a0,0x10);
    if (bVar6) {
      _DAT_002fe0a0 = CONCAT31(DAT_002fe0a0_1,1);
      cVar1 = ExclusiveMonitorsStatus();
    }
  } while (cVar1 != '\0');
  if (cVar5 != '\0') {
    uVar10 = 0;
    goto LAB_0018bf80;
  }
  if (DAT_002fd678 == (code *)0x0) {
    uVar20 = 1;
LAB_0018bf48:
    uVar10 = 0;
    _DAT_001cfdf8 = _DAT_001cfdf8 & 0xffffffff;
    _DAT_001cfe00 = (ulong)uVar20 << 0x20;
    _DAT_001cfe20 = _DAT_001cfe20 & 0xffffffff;
    DAT_001cfda8 = DAT_001cfda8 + 1;
    _DAT_001cfe30 = _DAT_001cfe30 & 0xffffffff00000000;
    DAT_001cfdb8 = DAT_001cfdb8 + 1;
  }
  else {
    if (((param_1 == (int *)0x0) || (*param_1 != 1)) || (param_1[1] != 0x28)) {
      uVar20 = 2;
      goto LAB_0018bf48;
    }
    uVar20 = 2;
    if ((*(ulong *)(param_1 + 4) + 0x2000 < 0x12000) || ((*(ulong *)(param_1 + 4) & 7) != 0))
    goto LAB_0018bf48;
    uVar20 = 2;
    if ((*(ulong *)(param_1 + 6) + 0x2000 < 0x12000) || ((*(ulong *)(param_1 + 6) & 7) != 0))
    goto LAB_0018bf48;
    uVar20 = 8;
    if ((*(ulong *)(param_1 + 2) == 0) || (*(ulong *)(param_1 + 2) <= DAT_001cfdb0))
    goto LAB_0018bf48;
    uStack_a8 = _DAT_001cfe40;
    local_b0 = _DAT_001cfe38;
    uStack_98 = (undefined4)_DAT_001cfe50;
    local_94 = (undefined4)((ulong)_DAT_001cfe50 >> 0x20);
    uStack_a0 = _DAT_001cfe48;
    uStack_88 = DAT_001cfe60;
    uStack_90 = (undefined4)_DAT_001cfe58;
    uStack_8c = (undefined4)((ulong)_DAT_001cfe58 >> 0x20);
    uStack_e8 = _DAT_001cfe00;
    local_f0 = _DAT_001cfdf8;
    uStack_d8 = _DAT_001cfe10;
    uStack_e0 = _DAT_001cfe08;
    uStack_c8 = _DAT_001cfe20;
    local_d0 = _DAT_001cfe18;
    uStack_b8 = _DAT_001cfe30;
    local_c0 = _DAT_001cfe28;
    lStack_128 = DAT_001cfdc0;
    local_130 = DAT_001cfdb8;
    local_118 = DAT_001cfdd0;
    uStack_120 = DAT_001cfdc8;
    uStack_108 = _DAT_001cfde0;
    local_110 = DAT_001cfdd8;
    local_f8 = DAT_001cfdf0;
    uStack_100 = DAT_001cfde8;
    lStack_148 = DAT_001cfda0;
    local_150 = DAT_001cfd98;
    local_138 = DAT_001cfdb0;
    lStack_140 = DAT_001cfda8;
    iVar7 = FUN_0018ca9c(DAT_002fd660 + 0x1307e20,&local_1b58);
    if ((iVar7 == 0) || (local_1b58 + 0x54U < 0x10004)) {
LAB_0018bf34:
      uVar20 = 3;
      goto LAB_0018bf48;
    }
    iVar7 = (*DAT_002fd678)(DAT_002fd680,local_1b58 + 0x50,&local_1bb4,4);
    uVar20 = 3;
    if ((iVar7 != 1) || (local_1bb4 != 5)) goto LAB_0018bf48;
    iVar7 = FUN_0018ca9c(local_1b58 + 0x48,&local_1b60);
    if (iVar7 == 0) goto LAB_0018bf34;
    iVar7 = FUN_0018ca9c(*(long *)(param_1 + 4) + 0x918,&local_1b68);
    if (((iVar7 == 0) || (local_1b68 != local_1b60)) ||
       (iVar7 = FUN_0018ca9c(local_1b68 + 0x28,&local_1b70), iVar7 == 0)) {
      uVar20 = 4;
      goto LAB_0018bf48;
    }
    iVar7 = FUN_0018ca9c(local_1b70 + 0x28,&local_1b78);
    if ((iVar7 == 0) || (local_1b70 + 0x104 < 0x10004)) {
      uVar20 = 4;
      goto LAB_0018bf48;
    }
    uVar20 = 4;
    iVar7 = (*DAT_002fd678)(DAT_002fd680,local_1b70 + 0x100,&local_1bbc,4);
    if ((iVar7 != 1) || (uVar20 = 4, (int)local_1bbc < 1)) goto LAB_0018bf48;
    if (local_1b78 + 0x10 < 0x10004) {
LAB_0018c0b8:
      uVar20 = 4;
      goto LAB_0018bf48;
    }
    iVar7 = (*DAT_002fd678)(DAT_002fd680,local_1b78 + 0xc,&local_1bb8,4);
    if ((iVar7 != 1) || ((int)local_1bb8 < 1)) {
LAB_0018c194:
      uVar20 = 4;
      goto LAB_0018bf48;
    }
    uVar20 = 4;
    if (0x200 < (int)local_1bb8) goto LAB_0018bf48;
    iVar7 = FUN_0018ca9c(local_1b78,&local_1b80);
    if (((iVar7 == 0) || (iVar7 = FUN_0018ca9c(local_1b70 + 0xf8,&local_1ba8), iVar7 == 0)) ||
       (iVar7 = FUN_0018ca9c(local_1b60 + 0x58,&local_1bb0), iVar7 == 0)) goto LAB_0018c0b8;
    if (local_1b80 < 0x10000) goto LAB_0018c194;
    uVar20 = 4;
    if (CARRY8(local_1b80,(long)(int)local_1bb8 << 3)) goto LAB_0018bf48;
    iVar7 = (*DAT_002fd678)(DAT_002fd680,local_1b80,local_1150);
    if (iVar7 != 1) goto LAB_0018c0b8;
    uVar12 = (ulong)local_1bb8;
    if ((int)local_1bb8 < 1) {
      uVar20 = 5;
      goto LAB_0018bf48;
    }
    uVar11 = *(ulong *)(param_1 + 6);
    if (local_1bb8 < 8) {
      uVar14 = 0;
      iVar7 = 0;
LAB_0018c208:
      lVar13 = uVar12 - uVar14;
      puVar17 = local_1150 + uVar14;
      do {
        if (*puVar17 == uVar11) {
          iVar7 = iVar7 + 1;
        }
        lVar13 = lVar13 + -1;
        puVar17 = puVar17 + 1;
      } while (lVar13 != 0);
    }
    else {
      uVar14 = uVar12 & 0xfffffff8;
      iVar7 = 0;
      iVar9 = 0;
      iVar23 = 0;
      iVar24 = 0;
      pauVar15 = (undefined1 (*) [16])auStack_1130;
      uVar16 = uVar14;
      auVar25 = ZEXT816(0);
      do {
        uVar16 = uVar16 - 8;
        auVar22._8_8_ = uVar11;
        auVar22._0_8_ = uVar11;
        auVar22 = NEON_cmeq(pauVar15[-2],auVar22,8);
        auVar27._8_8_ = uVar11;
        auVar27._0_8_ = uVar11;
        auVar27 = NEON_cmeq(pauVar15[-1],auVar27,8);
        auVar28._8_8_ = uVar11;
        auVar28._0_8_ = uVar11;
        auVar28 = NEON_cmeq(*pauVar15,auVar28,8);
        auVar29._8_8_ = uVar11;
        auVar29._0_8_ = uVar11;
        auVar29 = NEON_cmeq(pauVar15[1],auVar29,8);
        iVar7 = iVar7 - auVar22._0_4_;
        iVar9 = iVar9 - auVar22._8_4_;
        iVar23 = iVar23 - auVar27._0_4_;
        iVar24 = iVar24 - auVar27._8_4_;
        auVar26._0_4_ = auVar25._0_4_ - auVar28._0_4_;
        auVar26._4_4_ = auVar25._4_4_ - auVar28._8_4_;
        auVar26._8_4_ = auVar25._8_4_ - auVar29._0_4_;
        auVar26._12_4_ = auVar25._12_4_ - auVar29._8_4_;
        pauVar15 = pauVar15 + 4;
        auVar25 = auVar26;
      } while (uVar16 != 0);
      iVar7 = auVar26._0_4_ + iVar7 + auVar26._4_4_ + iVar9 +
              auVar26._8_4_ + iVar23 + auVar26._12_4_ + iVar24;
      if (uVar14 != uVar12) goto LAB_0018c208;
    }
    if (((iVar7 != 1) || (*(long *)(param_1 + 6) + 0xcU < 0x10004)) ||
       ((iVar7 = (*DAT_002fd678)(DAT_002fd680,*(long *)(param_1 + 6) + 8,&local_1bc0,4), iVar7 != 1
        || (local_1bc0 != local_1bbc)))) {
      uVar20 = 5;
      goto LAB_0018bf48;
    }
    iVar7 = FUN_0018ca9c(*(undefined8 *)(param_1 + 6),&local_1b90);
    if ((((iVar7 == 0) ||
         ((local_1b90 != DAT_002fd660 + 0x1220040 && (local_1b90 != DAT_002fd660 + 0x12200a0)))) ||
        (iVar7 = FUN_0018ca9c(*(long *)(param_1 + 6) + 0x10,&local_1b98), iVar7 == 0)) ||
       (((iVar7 = FUN_0018ca9c(local_1b98,&local_1ba0), iVar7 == 0 ||
         (local_1ba0 != DAT_002fd660 + 0x121c438)) || (local_1b98 + 0x2b0U < 0x10004)))) {
LAB_0018c2f0:
      uVar20 = 6;
      goto LAB_0018bf48;
    }
    iVar7 = (*DAT_002fd678)(DAT_002fd680,local_1b98 + 0x2ac,&local_d0,4);
    uVar20 = 6;
    if (((iVar7 != 1) || ((int)local_d0 < 1)) || (uVar20 = 6, 0x5dc < (int)local_d0))
    goto LAB_0018bf48;
    iVar7 = FUN_0018cb14(*(undefined8 *)(param_1 + 6),(long)&uStack_e0 + 4,&uStack_d8,
                         (long)&uStack_d8 + 4);
    if ((iVar7 == 0) ||
       (iVar7 = FUN_0018cc38(*(undefined8 *)(param_1 + 6),local_1b70,(long)&local_d0 + 4),
       iVar7 == 0)) goto LAB_0018c2f0;
    uVar8 = (*DAT_002fd688)("autododgeEnabled");
    uStack_b8 = CONCAT44(uVar8,(undefined4)uStack_b8);
    uVar10 = (*DAT_002fd688)("dodgeVersion");
    uVar20 = 9;
    iVar7 = (int)uVar10;
    local_b0 = CONCAT44(local_b0._4_4_,iVar7);
    if (((1 < uStack_b8._4_4_) || (iVar7 < 1)) || (uVar20 = 9, 5 < iVar7)) goto LAB_0018bf48;
    iVar7 = (*DAT_002fd690)(uVar10,(long)&local_b0 + 4);
    if (iVar7 != 1) {
      uVar20 = 9;
      goto LAB_0018bf48;
    }
    iVar7 = (int)local_b0;
    iVar9 = (*DAT_002fd688)("dodgeVersion");
    if (iVar7 != iVar9) {
LAB_0018c740:
      uVar20 = 9;
      goto LAB_0018bf48;
    }
    iVar7 = uStack_b8._4_4_;
    iVar9 = (*DAT_002fd688)("autododgeEnabled");
    auVar25 = _DAT_00112a00;
    if (iVar7 != iVar9) goto LAB_0018c740;
    uVar20 = ~local_d0._4_4_ >> 0x1f;
    if ((int)local_1bb8 < 1) {
      local_1c18 = 0;
      local_1c04 = 0;
    }
    else {
      lVar13 = 0;
      local_1c04 = 0;
      local_1c18 = 0;
      do {
        uVar12 = local_1150[lVar13];
        if (uVar12 != *(ulong *)(param_1 + 6)) {
          if ((uVar12 + 0x2000 < 0x12000 || (uVar12 & 7) != 0) ||
             (iVar7 = FUN_0018ca9c(uVar12,&local_1b90), iVar7 == 0)) {
            uVar20 = 0;
            local_1c04 = local_1c04 + 1;
          }
          else if (local_1b90 == DAT_002fd660 + 0x12206a8) {
            uStack_1be0 = 0;
            uStack_1be4 = 0;
            iStack_1be8 = 0;
            local_1bcc = 0;
            uStack_1bd0 = 0;
            local_1bd4 = 0;
            local_1bd8 = 0;
            local_1bf0 = uVar12;
            iVar7 = FUN_0018ca9c(uVar12 + 0x10,&local_1b98);
            if (((((iVar7 != 0) && (iVar7 = FUN_0018ca9c(local_1b98,&local_1ba0), iVar7 != 0)) &&
                 ((local_1ba0 == DAT_002fd660 + 0x121ea60 &&
                  ((((iVar7 = (*DAT_002fd678)(DAT_002fd680,uVar12 + 8,&iStack_1be8,4), iVar7 == 1 &&
                     (0 < iStack_1be8)) &&
                    (iVar7 = FUN_0018cb14(uVar12,&uStack_1be4,&uStack_1be0,(long)&uStack_1be0 + 4),
                    iVar7 != 0)) &&
                   ((iVar7 = FUN_0018cc38(uVar12,local_1b70,&local_1bd8), iVar7 != 0 &&
                    (0x10003 < local_1b98 + 0x128U)))))))) &&
                (iVar7 = (*DAT_002fd678)(DAT_002fd680,local_1b98 + 0x124,&local_1bd4,4), iVar7 == 1)
                ) && ((0x10003 < local_1b98 + 0x134U &&
                      (iVar7 = (*DAT_002fd678)(DAT_002fd680,local_1b98 + 0x130,&uStack_1bd0,4),
                      iVar7 == 1)))) {
              if (local_1b98 + 0x1dbU < 0x10001) {
                bVar6 = false;
              }
              else {
                iVar7 = (*DAT_002fd678)(DAT_002fd680,local_1b98 + 0x1da,local_1bf4,1);
                bVar6 = iVar7 == 1;
              }
              if (bVar6) {
                auVar21._8_8_ = CONCAT44(uStack_1bd0,local_1bd4);
                auVar21._0_8_ = CONCAT44(uStack_1bd0,local_1bd4);
                auVar27 = NEON_cmgt(auVar21,auVar25,4);
                auVar22 = NEON_cmgt(auVar25,auVar21,4);
                if (((((auVar27 & (undefined1  [16])0x1) == (undefined1  [16])0x0 &&
                      (auVar27 & (undefined1  [16])0x100000000) == (undefined1  [16])0x0) &&
                     (auVar22 & (undefined1  [16])0x1) == (undefined1  [16])0x0) &&
                     (auVar22 & (undefined1  [16])0x100000000) == (undefined1  [16])0x0) &&
                   (local_1bf4[0] < 2)) {
                  if (local_1bd8 == local_d0._4_4_) {
                    local_1c18 = CONCAT44(local_1c18._4_4_ + 1,(int)local_1c18);
                  }
                  else {
                    if ((local_1bd8 < 0) || (local_d0 < 0)) goto LAB_0018c700;
                    local_1bcc = (uint)local_1bf4[0];
                    if ((int)local_1c18 == 0x40) {
                      uVar20 = 0;
                      local_1c18 = CONCAT44(local_1c18._4_4_,0x40);
                    }
                    else {
                      auVar3._8_4_ = iStack_1be8;
                      auVar3._0_8_ = local_1bf0;
                      auVar3._12_4_ = uStack_1be4;
                      auVar4._8_4_ = local_1bd8;
                      auVar4._0_8_ = uStack_1be0;
                      auVar4._12_4_ = local_1bd4;
                      uVar12 = local_1c18 & 0xffffffff;
                      local_1c18 = CONCAT44(local_1c18._4_4_,(int)local_1c18 + 1);
                      auStack_1b50[uVar12 * 5 + 1] = auVar3._8_8_;
                      auStack_1b50[uVar12 * 5] = local_1bf0;
                      auStack_1b50[uVar12 * 5 + 3] = auVar4._8_8_;
                      auStack_1b50[uVar12 * 5 + 2] = uStack_1be0;
                      auStack_1b50[uVar12 * 5 + 4] = (ulong)CONCAT14(local_1bf4[0],uStack_1bd0);
                    }
                  }
                  goto LAB_0018c49c;
                }
              }
            }
LAB_0018c700:
            uVar20 = 0;
          }
          else if (local_1b90 != DAT_002fd660 + 0x1220040 && local_1b90 != DAT_002fd660 + 0x12200a0)
          {
            local_1c04 = local_1c04 + 1;
          }
        }
LAB_0018c49c:
        lVar13 = lVar13 + 1;
      } while (lVar13 < (int)local_1bb8);
    }
    iVar7 = FUN_0018ca9c(local_1b58 + 0x48,&local_1b88);
    if ((((((iVar7 == 0) || (local_1b88 != local_1b60)) ||
          (iVar7 = FUN_0018ca9c(*(long *)(param_1 + 4) + 0x918,&local_1b88), iVar7 == 0)) ||
         (((local_1b88 != local_1b60 ||
           (iVar7 = FUN_0018ca9c(local_1b88 + 0x28,&local_1b88), iVar7 == 0)) ||
          ((local_1b88 != local_1b70 ||
           ((iVar7 = FUN_0018ca9c(local_1b88 + 0x28,&local_1b88), iVar7 == 0 ||
            (local_1b88 != local_1b78)))))))) ||
        ((((iVar7 = FUN_0018ca9c(local_1b88,&local_1b88), iVar7 == 0 ||
           (((local_1b88 != local_1b80 || (local_1b78 + 0x10 < 0x10004)) ||
            (iVar7 = (*DAT_002fd678)(DAT_002fd680,local_1b78 + 0xc,&local_1bc4,4), iVar7 != 1)))) ||
          (((local_1bc4 != local_1bb8 || (local_1b70 + 0x104 < 0x10004)) ||
           ((iVar7 = (*DAT_002fd678)(DAT_002fd680,local_1b70 + 0x100,&local_1bc4,4), iVar7 != 1 ||
            ((local_1bc4 != local_1bbc || (*(long *)(param_1 + 6) + 0xcU < 0x10004)))))))) ||
         (iVar7 = (*DAT_002fd678)(DAT_002fd680,*(long *)(param_1 + 6) + 8,&local_1bc4,4), iVar7 != 1
         )))) || ((((local_1bc4 != local_1bbc ||
                    (iVar7 = FUN_0018ca9c(local_1b60 + 0x58,&local_1b88), iVar7 == 0)) ||
                   (local_1b88 != local_1bb0)) ||
                  ((iVar7 = FUN_0018ca9c(local_1b70 + 0xf8,&local_1b88), iVar7 == 0 ||
                   (local_1b88 != local_1ba8)))))) {
      uVar20 = 7;
      goto LAB_0018bf48;
    }
    lStack_128 = *(long *)(param_1 + 4);
    if ((((DAT_001cfdc0 == lStack_128) && (DAT_001cfdc8 == local_1b60)) &&
        ((local_1b60 = DAT_001cfdc8, DAT_001cfdd0 == local_1b70 &&
         (((DAT_001cfdd8 == *(long *)(param_1 + 6) && (DAT_001cfe08 == local_1bbc)) &&
          (DAT_001cfdf0 == local_1bb0)))))) && (DAT_001cfde8 == local_1b88)) {
      uVar19 = 4;
      if (DAT_001cfdfc < 4) {
        uVar19 = DAT_001cfdfc + 1;
      }
      uVar18 = (uint)(3 < uVar19);
      local_f0 = CONCAT44(uVar19,(undefined4)local_f0);
      local_118 = DAT_001cfdd0;
      local_110 = DAT_001cfdd8;
      local_f8 = DAT_001cfdf0;
      local_1bbc = DAT_001cfe08;
    }
    else {
      uVar18 = 0;
      local_110 = *(long *)(param_1 + 6);
      local_130 = local_130 + 1;
      local_f0 = CONCAT44(1,(undefined4)local_f0);
      local_118 = local_1b70;
      local_f8 = local_1bb0;
    }
    uStack_108 = *(undefined8 *)(param_1 + 8);
    uStack_100 = local_1b88;
    local_138 = *(ulong *)(param_1 + 2);
    lStack_148 = lStack_148 + 1;
    uVar8 = 0;
    if (uVar20 == 0) {
      uVar8 = 10;
    }
    uStack_e0 = CONCAT44(uStack_e0._4_4_,local_1bbc);
    local_94 = 0;
    uStack_90 = 0;
    uStack_c8 = CONCAT44((int)local_1c18,local_1bb8);
    uVar19 = 0;
    if (uVar20 != 0) {
      uVar19 = uVar18;
    }
    uStack_8c = 0;
    local_c0 = CONCAT44(local_1c04,local_1c18._4_4_);
    uStack_b8 = CONCAT44(uStack_b8._4_4_,uVar20);
    uStack_e8 = CONCAT44(uVar8,uVar19);
    uStack_120 = local_1b60;
    memcpy(&DAT_002fd698,auStack_1b50,(local_1c18 & 0xffffffff) * 0x28);
    _DAT_001cfe38 = local_b0;
    _DAT_001cfe40 = uStack_a8;
    _DAT_001cfe48 = uStack_a0;
    _DAT_001cfe50 = CONCAT44(local_94,uStack_98);
    uVar10 = 1;
    DAT_001cfe60 = uStack_88;
    _DAT_001cfe58 = CONCAT44(uStack_8c,uStack_90);
    _DAT_001cfdf8 = local_f0;
    _DAT_001cfe00 = uStack_e8;
    _DAT_001cfe08 = uStack_e0;
    _DAT_001cfe10 = uStack_d8;
    _DAT_001cfe20 = uStack_c8;
    _DAT_001cfe18 = local_d0;
    _DAT_001cfe28 = local_c0;
    _DAT_001cfe30 = uStack_b8;
    DAT_001cfdc0 = lStack_128;
    DAT_001cfdc8 = uStack_120;
    DAT_001cfdd0 = local_118;
    _DAT_001cfde0 = uStack_108;
    DAT_001cfdd8 = local_110;
    DAT_001cfde8 = uStack_100;
    DAT_001cfdf0 = local_f8;
    DAT_001cfdb8 = local_130;
    DAT_001cfd98 = local_150;
    DAT_001cfda0 = lStack_148;
    DAT_001cfdb0 = local_138;
    DAT_001cfda8 = lStack_140;
  }
  _DAT_002fe0a0 = 0;
LAB_0018bf80:
  if (*(long *)(lVar2 + 0x28) == local_78) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar10);
}

/* ===== ng_diagnostics @ 0018cf6c [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

long ng_diagnostics(char *param_1,size_t param_2)

{
  char cVar1;
  bool bVar2;
  char cVar3;
  uint uVar4;
  
  if ((param_1 != (char *)0x0) || (param_2 == 0)) {
    do {
      cVar3 = DAT_002fe0a0;
      cVar1 = '\x01';
      bVar2 = (bool)ExclusiveMonitorPass(0x2fe0a0,0x10);
      if (bVar2) {
        _DAT_002fe0a0 = CONCAT31(DAT_002fe0a0_1,1);
        cVar1 = ExclusiveMonitorsStatus();
      }
    } while (cVar1 != '\0');
    if (cVar3 == '\0') {
      _DAT_002fe0a0 = 0;
      uVar4 = snprintf(param_1,param_2,
                       "{\"schema\":\"nexus-evasion-game-passive/v1\",\"bound\":%u,\"accepted\":%llu,\"rejected\":%llu,\"sequence\":%llu,\"reason\":%u,\"stable_frames\":%u,\"passive_ready\":%u,\"projectiles\":%u,\"feed_complete\":%u,\"requested_autododge\":%d,\"requested_version\":%d,\"movement_enabled\":false,\"gameplay_effective\":false}"
                       ,(ulong)DAT_001cfdf8,DAT_001cfda0,DAT_001cfda8,DAT_001cfdb0,
                       (ulong)DAT_001cfe04,DAT_001cfdfc,DAT_001cfe00,DAT_001cfe24,DAT_001cfe30,
                       DAT_001cfe34,DAT_001cfe38);
      if ((int)uVar4 < 0) {
        return 0;
      }
      return (ulong)uVar4 + 1;
    }
  }
  return 0;
}

/* ===== FUN_00158d24 @ 00158d24 [libNexusUI69252.so] ===== */

void FUN_00158d24(undefined8 param_1,undefined8 param_2,uint param_3,byte param_4)

{
  long lVar1;
  __pid_t _Var2;
  int iVar3;
  int iVar4;
  undefined8 uVar5;
  int *piVar6;
  long lVar7;
  uint uVar8;
  uint *puVar9;
  ulong uVar10;
  uint local_b8 [7];
  uint local_9c;
  timespec local_98 [5];
  long local_48;
  
  lVar1 = tpidr_el0;
  uVar5 = 4;
  local_48 = *(long *)(lVar1 + 0x28);
  if (param_3 < 0x6c) {
    if (param_4 < 2) {
      uVar5 = FUN_001592cc(local_b8);
      if ((int)uVar5 != 0) {
        uVar8 = 1 << (ulong)(param_3 & 0x1f);
        if (param_4 == 1) {
          puVar9 = local_b8 + (param_3 >> 5);
          uVar8 = local_b8[(ulong)(param_3 >> 5) + 3] | uVar8;
        }
        else {
          puVar9 = local_b8 + (param_3 >> 5);
          uVar8 = local_b8[(ulong)(param_3 >> 5) + 3] & (uVar8 ^ 0xffffffff);
        }
        puVar9[3] = uVar8;
        if (-1 < DAT_001a7c50) {
          local_b8[1] = local_b8[1] ^
                        ((local_b8[0] ^ 0xba0d24b4) >> 0x1b | (local_b8[0] ^ 0xba0d24b4) << 5);
          local_b8[2] = local_b8[2] ^ (local_b8[1] >> 0x1b | local_b8[1] << 5);
          local_b8[3] = local_b8[3] ^ (local_b8[2] >> 0x1b | local_b8[2] << 5);
          local_b8[4] = local_b8[4] ^ (local_b8[3] >> 0x1b | local_b8[3] << 5);
          local_b8[5] = local_b8[5] ^ (local_b8[4] >> 0x1b | local_b8[4] << 5);
          local_9c = local_b8[6] ^ (local_b8[5] >> 0x1b | local_b8[5] << 5);
          _Var2 = getpid();
          iVar3 = clock_gettime(1,local_98);
          if (iVar3 == 0) {
            lVar7 = local_98[0].tv_sec * 1000 + (ulong)local_98[0].tv_nsec / 1000000;
          }
          else {
            lVar7 = 0;
          }
          FUN_0015540c(local_98,0x50,0x50,"autododge-ignore-%d-%llu.tmp",_Var2,lVar7);
          iVar3 = openat(DAT_001a7c50,(char *)local_98,0x880c1,0x180);
          if (-1 < iVar3) {
            uVar10 = 0;
            do {
              lVar7 = __write_chk(iVar3,(long)local_b8 + uVar10,0x20 - uVar10,0xffffffffffffffff);
              if (lVar7 < 0) {
                piVar6 = (int *)__errno();
                if (*piVar6 != 4) goto LAB_00158f14;
              }
              else {
                uVar10 = lVar7 + uVar10;
                if (lVar7 == 0) break;
              }
            } while (uVar10 < 0x20);
            if (uVar10 == 0x20) {
              iVar4 = fsync(iVar3);
              close(iVar3);
              if (((iVar4 == 0) &&
                  (iVar3 = renameat(DAT_001a7c50,(char *)local_98,DAT_001a7c50,
                                    "autododge-ignore.bin"), iVar3 == 0)) &&
                 (iVar3 = fsync(DAT_001a7c50), iVar3 == 0)) {
                uVar5 = 1;
                goto LAB_00158f30;
              }
            }
            else {
LAB_00158f14:
              close(iVar3);
            }
            unlinkat(DAT_001a7c50,(char *)local_98,0);
          }
        }
        uVar5 = 0;
      }
    }
  }
LAB_00158f30:
  if (*(long *)(lVar1 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar5);
}

/* ===== FUN_001592cc @ 001592cc [libNexusUI69252.so] ===== */

bool FUN_001592cc(int *param_1)

{
  undefined *puVar1;
  long lVar2;
  bool bVar3;
  uint uVar4;
  int iVar5;
  __uid_t _Var6;
  uint uVar7;
  int *piVar8;
  undefined8 *puVar9;
  long lVar10;
  int iVar11;
  long *plVar12;
  stat sStack_f8;
  int *local_68;
  undefined8 uStack_60;
  long local_58;
  
  lVar2 = tpidr_el0;
  bVar3 = false;
  local_58 = *(long *)(lVar2 + 0x28);
  piVar8 = param_1;
  if ((param_1 == (int *)0x0) || (piVar8 = (int *)(ulong)DAT_001a7c50, (int)DAT_001a7c50 < 0))
  goto LAB_00159474;
  param_1[2] = 0;
  param_1[3] = 0;
  param_1[0] = 0;
  param_1[1] = 0;
  param_1[6] = 0;
  param_1[7] = 0;
  param_1[4] = 0;
  param_1[5] = 0;
  uVar4 = __openat_2(piVar8,"autododge-ignore.bin",0x88000);
  if ((int)uVar4 < 0) {
    piVar8 = (int *)__errno();
    if (*piVar8 == 2) {
      bVar3 = true;
      uVar4 = param_1[4] ^ ((param_1[3] ^ 0x3b7d4c22U) >> 0x1b | (param_1[3] ^ 0x3b7d4c22U) << 5);
      *(undefined8 *)param_1 = DAT_0010f858;
      uVar4 = param_1[5] ^ (uVar4 >> 0x1b | uVar4 << 5);
      param_1[2] = 0x6c;
      param_1[7] = param_1[6] ^ (uVar4 >> 0x1b | uVar4 << 5);
    }
    else {
      bVar3 = false;
    }
    goto LAB_00159474;
  }
  iVar5 = fstat(uVar4,&sStack_f8);
  if ((iVar5 == 0) && (((uint)sStack_f8.st_nlink & 0xf000) == 0x8000)) {
    _Var6 = getuid();
    bVar3 = false;
    if (((sStack_f8.st_mode == _Var6) && (sStack_f8.st_nlink._4_4_ == 1)) &&
       (sStack_f8.st_size == 0x20)) {
      piVar8 = (int *)__errno();
      iVar5 = *piVar8;
      puVar9 = (undefined8 *)__emutls_get_address(&DAT_001a7c68);
      puVar1 = &DAT_00133b55;
      if (uVar4 != 0x7fff0001) {
        puVar1 = &DAT_00134ec4;
      }
      puVar9[5] = 0;
      puVar9[6] = 0;
      *puVar9 = puVar1;
      puVar9[1] = &DAT_00133b50;
      puVar9[2] = &DAT_00137150;
      puVar9[3] = 0x20;
      plVar12 = puVar9 + 4;
      *plVar12 = 0;
      uStack_60 = 0x20;
      sStack_f8.__unused[1] = 0;
      sStack_f8.__unused[2] = 0x20;
      local_68 = param_1;
      if (uVar4 != 0x7fff0001) {
        lVar10 = syscall(0x43,(ulong)uVar4,param_1,0x20,0);
      }
      else {
        uVar7 = getpid();
        lVar10 = syscall(0x10e,(ulong)uVar7,&local_68,1,sStack_f8.__unused + 1,1,0);
      }
      if (lVar10 < 0) {
        iVar11 = *piVar8;
        if (iVar11 == 4) {
          if (uVar4 == 0x7fff0001) {
            uVar7 = getpid();
            lVar10 = syscall(0x10e,(ulong)uVar7,&local_68,1,sStack_f8.__unused + 1,1,0);
          }
          else {
            lVar10 = syscall(0x43,(ulong)uVar4,param_1,0x20,0);
          }
          if (-1 < lVar10) goto LAB_001594c0;
          iVar11 = *piVar8;
          if (iVar11 != 4) goto LAB_00159758;
          if (uVar4 == 0x7fff0001) {
            uVar7 = getpid();
            lVar10 = syscall(0x10e,(ulong)uVar7,&local_68,1,sStack_f8.__unused + 1,1,0);
          }
          else {
            lVar10 = syscall(0x43,(ulong)uVar4,param_1,0x20,0);
          }
          if (-1 < lVar10) goto LAB_001594c0;
          iVar11 = *piVar8;
          if (iVar11 != 4) goto LAB_00159758;
          if (uVar4 == 0x7fff0001) {
            uVar7 = getpid();
            lVar10 = syscall(0x10e,(ulong)uVar7,&local_68,1,sStack_f8.__unused + 1,1,0);
          }
          else {
            lVar10 = syscall(0x43,(ulong)uVar4,param_1,0x20,0);
          }
          if (-1 < lVar10) goto LAB_001594c0;
          iVar11 = *piVar8;
          if (iVar11 != 4) goto LAB_00159758;
          if (uVar4 == 0x7fff0001) {
            uVar7 = getpid();
            lVar10 = syscall(0x10e,(ulong)uVar7,&local_68,1,sStack_f8.__unused + 1,1,0);
          }
          else {
            lVar10 = syscall(0x43,(ulong)uVar4,param_1,0x20,0);
          }
          if (-1 < lVar10) goto LAB_001594c0;
          iVar11 = *piVar8;
          if (iVar11 != 4) goto LAB_00159758;
          if (uVar4 == 0x7fff0001) {
            uVar7 = getpid();
            lVar10 = syscall(0x10e,(ulong)uVar7,&local_68,1,sStack_f8.__unused + 1,1,0);
          }
          else {
            lVar10 = syscall(0x43,(ulong)uVar4,param_1,0x20,0);
          }
          if (-1 < lVar10) goto LAB_001594c0;
          iVar11 = *piVar8;
          if (iVar11 != 4) goto LAB_00159758;
          if (uVar4 == 0x7fff0001) {
            uVar7 = getpid();
            lVar10 = syscall(0x10e,(ulong)uVar7,&local_68,1,sStack_f8.__unused + 1,1,0);
          }
          else {
            lVar10 = syscall(0x43,(ulong)uVar4,param_1,0x20,0);
          }
          if (-1 < lVar10) goto LAB_001594c0;
          iVar11 = *piVar8;
          if (iVar11 != 4) goto LAB_00159758;
          if (uVar4 == 0x7fff0001) {
            uVar7 = getpid();
            lVar10 = syscall(0x10e,(ulong)uVar7,&local_68,1,sStack_f8.__unused + 1,1,0);
          }
          else {
            lVar10 = syscall(0x43,(ulong)uVar4,param_1,0x20,0);
          }
          *plVar12 = lVar10;
          goto joined_r0x00159788;
        }
LAB_00159758:
        *plVar12 = lVar10;
      }
      else {
LAB_001594c0:
        *plVar12 = lVar10;
joined_r0x00159788:
        if (-1 < lVar10) {
          bVar3 = false;
          *(undefined4 *)(puVar9 + 6) = 0;
          *piVar8 = iVar5;
          if (lVar10 == 0x20) {
            iVar5 = FUN_001597b8(param_1);
            bVar3 = iVar5 != 0;
          }
          goto LAB_00159414;
        }
        iVar11 = *piVar8;
      }
      bVar3 = false;
      *(int *)(puVar9 + 6) = iVar11;
    }
  }
  else {
    bVar3 = false;
  }
LAB_00159414:
  uVar4 = close(uVar4);
  piVar8 = (int *)(ulong)uVar4;
LAB_00159474:
  if (*(long *)(lVar2 + 0x28) != local_58) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(piVar8);
  }
  return bVar3;
}

/* ===== FUN_0018d15c @ 0018d15c [libNexusUI69252.so] ===== */

ulong FUN_0018d15c(long param_1)

{
  long lVar1;
  int iVar2;
  char *pcVar3;
  char *__s1;
  ulong uVar4;
  undefined8 *puVar5;
  long lVar6;
  char acStack_a8 [96];
  long local_48;
  
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  __s1 = *(char **)(param_1 + 0x20);
  if (__s1 == (char *)0x0) {
    uVar4 = 0xffffffff;
  }
  else {
    iVar2 = strncmp(__s1,"nexus_dodge_",0xc);
    if ((iVar2 == 0) && (pcVar3 = strstr(__s1,"blacklist"), pcVar3 == (char *)0x0)) {
      lVar6 = 0;
      puVar5 = (undefined8 *)PTR_PTR_s_nexus_sx_spin_001a36f8;
      do {
        iVar2 = strcmp((char *)*puVar5,"nexus_autododge_version");
        if (iVar2 == 0) {
          if (-1 < (int)lVar6) {
            iVar2 = *(int *)((long)&DAT_002846b0 + lVar6 * 4);
            if (1 < iVar2) goto LAB_0018d21c;
            goto LAB_0018d26c;
          }
          break;
        }
        lVar6 = lVar6 + 1;
        puVar5 = puVar5 + 3;
      } while (lVar6 != 0x76);
      iVar2 = 3;
LAB_0018d21c:
      FUN_00183ac8(acStack_a8,0x60,0x60,"nexus_v%d_dodge_%s",iVar2,__s1 + 0xc);
      uVar4 = 0;
      puVar5 = (undefined8 *)PTR_PTR_s_nexus_sx_spin_001a36f8;
      do {
        iVar2 = strcmp((char *)*puVar5,acStack_a8);
        if (iVar2 == 0) {
          if (-1 < (int)uVar4) goto LAB_0018d270;
          break;
        }
        uVar4 = uVar4 + 1;
        puVar5 = puVar5 + 3;
      } while (uVar4 != 0x76);
    }
LAB_0018d26c:
    uVar4 = (ulong)*(uint *)(param_1 + 0x38);
  }
LAB_0018d270:
  if (*(long *)(lVar1 + 0x28) != local_48) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return uVar4 & 0xffffffff;
}

/* ===== FUN_0018d48c @ 0018d48c [libNexusUI69252.so] ===== */

undefined4 FUN_0018d48c(void)

{
  int iVar1;
  long lVar2;
  undefined8 *puVar3;
  
  lVar2 = 0;
  puVar3 = (undefined8 *)PTR_PTR_s_nexus_sx_spin_001a36f8;
  do {
    iVar1 = strcmp((char *)*puVar3,"nexus_autododge_version");
    if (iVar1 == 0) {
      if ((int)lVar2 < 0) {
        return 3;
      }
      return *(undefined4 *)((long)&DAT_002846b0 + lVar2 * 4);
    }
    lVar2 = lVar2 + 1;
    puVar3 = puVar3 + 3;
  } while (lVar2 != 0x76);
  return 3;
}

