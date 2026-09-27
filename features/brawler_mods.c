/*
 * Brawler-specific mods (Nani / Colt / Kit / Dyna) — Feature
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
 * Notes: Brawler-specific behaviors: Nani ult auto-dodge + control mode, Colt mod, Kit/Nani toggles, Dyna jump.
 */

/* ===== FUN_0011b320 @ 0011b320 [libNexusEvasion69252.so] ===== */

char * FUN_0011b320(char *param_1)

{
  uint uVar1;
  int iVar2;
  char *pcVar3;
  
  pcVar3 = param_1;
  if (param_1 != (char *)0x0) {
    uVar1 = strcmp(param_1,"attackRangeIndicator");
    pcVar3 = (char *)(ulong)uVar1;
    if (uVar1 != 0) {
      uVar1 = strcmp(param_1,"hitboxRenderer");
      pcVar3 = (char *)(ulong)uVar1;
      if (uVar1 != 0) {
        uVar1 = strcmp(param_1,"enemyTracer");
        pcVar3 = (char *)(ulong)uVar1;
        if (uVar1 != 0) {
          uVar1 = strcmp(param_1,"trophiesAboveHead");
          pcVar3 = (char *)(ulong)uVar1;
          if (uVar1 != 0) {
            uVar1 = strcmp(param_1,"kitNaniModEnabled");
            pcVar3 = (char *)(ulong)uVar1;
            if (uVar1 != 0) {
              uVar1 = strcmp(param_1,"dynaJumpEnabled");
              pcVar3 = (char *)(ulong)uVar1;
              if (uVar1 != 0) {
                iVar2 = strcmp(param_1,"characterOutlineEnabled");
                if (((((iVar2 == 0) || (iVar2 = strcmp(param_1,"antiAfkEnabled"), iVar2 == 0)) ||
                     (iVar2 = strcmp(param_1,"autododgeEnabled"), iVar2 == 0)) ||
                    ((iVar2 = strcmp(param_1,"combatDodgeRequireHold"), iVar2 == 0 ||
                     (iVar2 = strncmp(param_1,"aop",3), iVar2 == 0)))) ||
                   ((iVar2 = strncmp(param_1,"killaura",8), iVar2 == 0 ||
                    ((iVar2 = strcmp(param_1,"coltModEnabled"), iVar2 == 0 ||
                     (iVar2 = strcmp(param_1,"koltModEnabled"), iVar2 == 0)))))) {
                  pcVar3 = (char *)0x1;
                }
                else {
                  iVar2 = strncmp(param_1,"holdToShoot",0xb);
                  pcVar3 = (char *)(ulong)(iVar2 == 0);
                }
              }
            }
          }
        }
      }
    }
  }
  return pcVar3;
}

/* ===== FUN_0015494c @ 0015494c [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0015494c(undefined8 param_1,undefined8 param_2)

{
  long lVar1;
  float fVar2;
  float fVar3;
  bool bVar4;
  int iVar5;
  ulong uVar6;
  long lVar7;
  long lVar8;
  float fVar9;
  undefined8 *local_158;
  code *pcStack_150;
  code *local_148;
  code *pcStack_140;
  undefined8 local_118;
  undefined8 local_110;
  ulong local_108;
  timespec local_100;
  undefined4 uStack_f0;
  undefined4 local_ec;
  undefined4 uStack_e8;
  undefined8 local_e0;
  undefined8 uStack_d8;
  undefined4 uStack_d0;
  float local_cc;
  float fStack_c8;
  undefined4 uStack_c4;
  undefined8 local_c0;
  undefined4 local_b8;
  undefined4 uStack_b4;
  ulong local_b0;
  undefined8 uStack_a8;
  char *local_a0 [2];
  int local_90;
  int local_74;
  float local_6c;
  float fStack_68;
  ulong local_60;
  undefined1 auStack_58 [8];
  undefined8 local_50;
  long local_48;
  
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  iVar5 = DAT_0020f694 + -1000000;
  if (999999 < DAT_0020f694 + 0xfefc99c0U) {
    iVar5 = DAT_0020f694;
  }
  if (iVar5 == 0xf42401) {
    local_158 = (undefined8 *)CONCAT44(local_158._4_4_,0xffffffff);
    local_a0[0] = "coltModEnabled";
    if (((((int)DAT_00214938 == 1) && (DAT_00214930 != (code *)0x0)) &&
        (iVar5 = (*DAT_00214930)(local_a0,&local_100,1,&local_60,&local_158), iVar5 == 1)) &&
       (((local_60 != 0 && ((int)local_158 == 0)) &&
        (((int)local_100.tv_nsec == 2 && (local_100.tv_sec._4_4_ == 1)))))) {
      uVar6 = FUN_001550fc();
      if ((int)uVar6 == 0) goto LAB_00154cd4;
      iVar5 = FUN_00150bf0(param_1,param_2);
      uVar6 = 0;
      if ((iVar5 == 0) || (DAT_0020f624 == 0)) goto LAB_00154cd4;
      if (DAT_0020f5e8 == local_60) {
        iVar5 = clock_gettime(1,&local_100);
        if (iVar5 == 0) {
          lVar8 = CONCAT44(local_100.tv_sec._4_4_,(undefined4)local_100.tv_sec) * 1000 +
                  CONCAT44(local_100.tv_nsec._4_4_,(int)local_100.tv_nsec) / 1000000;
        }
        else {
          lVar8 = 0;
        }
        lVar7 = FUN_00154d1c(local_a0);
        if (lVar7 == 0) {
          DAT_00214828 = 0;
          DAT_00214820 = 0;
          _DAT_00214838 = 0;
          DAT_00214830 = 0;
          DAT_00214848 = 0;
          _DAT_00214840 = 0;
          uRam0000000000214858 = 0;
          _DAT_00214850 = 0;
          DAT_00214860 = 0;
          DAT_00214868 = 0;
          local_100.tv_sec._0_4_ = 2;
          uStack_d8 = DAT_0020f678;
          uStack_d0 = (undefined4)param_2;
          local_cc = (float)((ulong)param_2 >> 0x20);
          fStack_c8 = (float)DAT_0020f6f8;
          uStack_c4 = (undefined4)((ulong)DAT_0020f6f8 >> 0x20);
          local_c0 = DAT_0020f650;
          local_100.tv_nsec._4_4_ = 0;
          uStack_f0 = 0;
          local_b8 = DAT_0020f690;
          uStack_b4 = DAT_0020f638._4_4_;
          local_100.tv_sec._4_4_ = 0;
          local_100.tv_nsec._0_4_ = 0;
          local_ec = DAT_002148a4;
          uStack_e8 = 0;
          local_b0 = local_60;
          uStack_a8 = 0;
          local_e0 = param_1;
          iVar5 = FUN_00185c68(&DAT_0020d168,&DAT_0020f628,&local_100,FUN_00150f7c,0,&local_118,
                               &local_158);
          if ((((iVar5 != 1) ||
               (iVar5 = FUN_00189c88(DAT_001e0978,local_118,FUN_001428fc,0,local_a0), iVar5 == 0))
              || (local_74 == 0)) || (local_90 != (int)local_110)) goto LAB_00154cd0;
          bVar4 = DAT_00214928 == -1;
          DAT_00214928 = DAT_00214928 + 1;
          if (bVar4) {
            DAT_00214928 = 1;
          }
          uVar6 = FUN_0019a40c(DAT_0020f6b4,DAT_0020f6b8,local_110._4_4_,local_108 & 0xffffffff,
                               &DAT_00214820,DAT_0020f6f8,DAT_00214928,lVar8,DAT_0020f690);
          if ((int)uVar6 == 0) goto LAB_00154cd4;
        }
        else {
          DAT_00214830 = lVar8 + 0x334;
        }
        iVar5 = FUN_00189c88(DAT_001e0978,param_2,FUN_001428fc,0,&local_100);
        fVar3 = fStack_c8;
        fVar2 = local_cc;
        uVar6 = 0;
        if ((iVar5 != 0) && (uStack_d8._4_4_ != 0)) {
          fVar9 = hypotf(local_6c - local_cc,fStack_68 - fStack_c8);
          if (fVar9 <= 5.0) {
            fVar9 = 5.0;
          }
          fVar9 = (float)NEON_fmin(fVar9,0x41900000);
          local_50 = CONCAT44((int)((fVar3 + (float)((ulong)DAT_00214848 >> 0x20) * fVar9) * 300.0),
                              (int)((fVar2 + (float)DAT_00214848 * fVar9) * 300.0));
          uVar6 = FUN_001428fc();
          if ((int)uVar6 != 0) {
            local_158 = &local_118;
            local_108 = local_60;
            pcStack_150 = FUN_001428fc;
            local_148 = FUN_00154ec4;
            pcStack_140 = FUN_00154f00;
            local_118 = param_1;
            local_110 = param_2;
            iVar5 = FUN_0018a084(&local_158,param_1,auStack_58,&local_50);
            if (iVar5 == -1) {
              FUN_001417c8("fatal","colt_xy_restore_unverified",0);
                    /* WARNING: Subroutine does not return */
              abort();
            }
            uVar6 = (ulong)(iVar5 == 1);
          }
        }
        goto LAB_00154cd4;
      }
    }
  }
LAB_00154cd0:
  uVar6 = 0;
LAB_00154cd4:
  if (*(long *)(lVar1 + 0x28) != local_48) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar6);
  }
  return;
}

/* ===== FUN_00154f00 @ 00154f00 [libNexusEvasionRuntime69252.so] ===== */

void FUN_00154f00(undefined8 *param_1)

{
  long lVar1;
  int iVar2;
  ulong uVar3;
  undefined1 auStack_80 [4];
  int local_7c;
  int local_78;
  long local_40;
  int local_34;
  char *local_30;
  long local_28;
  
  lVar1 = tpidr_el0;
  local_28 = *(long *)(lVar1 + 0x28);
  iVar2 = DAT_0020f694 + -1000000;
  if (999999 < DAT_0020f694 + 0xfefc99c0U) {
    iVar2 = DAT_0020f694;
  }
  if (iVar2 == 0xf42401) {
    local_34 = -1;
    local_30 = "coltModEnabled";
    if (((((int)DAT_00214938 == 1) && (DAT_00214930 != (code *)0x0)) &&
        (iVar2 = (*DAT_00214930)(&local_30,auStack_80,1,&local_40,&local_34), iVar2 == 1)) &&
       (((local_40 != 0 && (local_34 == 0)) && ((local_78 == 2 && (local_7c == 1)))))) {
      uVar3 = FUN_001550fc();
      if ((int)uVar3 == 0) goto LAB_00155010;
      if (local_40 == param_1[2]) {
        uVar3 = FUN_00154d1c(auStack_80);
        if (uVar3 != 0) {
          iVar2 = FUN_00150bf0(*param_1,param_1[1]);
          uVar3 = (ulong)(iVar2 != 0);
        }
        goto LAB_00155010;
      }
    }
  }
  uVar3 = 0;
LAB_00155010:
  if (*(long *)(lVar1 + 0x28) == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar3);
}

/* ===== FUN_00155ad0 @ 00155ad0 [libNexusEvasionRuntime69252.so] ===== */

void FUN_00155ad0(void)

{
  long lVar1;
  int iVar2;
  ulong uVar3;
  long local_50;
  int local_44;
  char local_40 [4];
  int local_3c;
  int local_38;
  char *local_30;
  long local_28;
  
  lVar1 = tpidr_el0;
  local_28 = *(long *)(lVar1 + 0x28);
  local_50 = 0;
  iVar2 = DAT_0020f694 + -1000000;
  if (999999 < DAT_0020f694 + 0xfefc99c0U) {
    iVar2 = DAT_0020f694;
  }
  if (iVar2 == 0xf42409) {
    local_44 = -1;
    local_30 = "dynaJumpEnabled";
    if ((((((int)DAT_00214938 == 1) && (DAT_00214930 != (code *)0x0)) &&
         (iVar2 = (*DAT_00214930)(&local_30,local_40,1,&local_50,&local_44), iVar2 == 1)) &&
        ((local_50 != 0 && (local_44 == 0)))) && ((local_38 == 2 && (local_3c == 1)))) {
      uVar3 = FUN_001550fc();
      if (((int)uVar3 != 0) &&
         (uVar3 = FUN_001bc828(DAT_0020f700,FUN_001428fc,0,local_40), (int)uVar3 != 0)) {
        uVar3 = (ulong)(local_40[0] == '\x01');
      }
      goto LAB_00155be0;
    }
  }
  uVar3 = 0;
LAB_00155be0:
  if (*(long *)(lVar1 + 0x28) == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar3);
}

/* ===== FUN_00158cd4 @ 00158cd4 [libNexusEvasionRuntime69252.so] ===== */

void FUN_00158cd4(long param_1,long param_2)

{
  long lVar1;
  int iVar2;
  long lVar3;
  bool bVar4;
  int iVar5;
  int iVar6;
  undefined8 uVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  bool bVar11;
  long local_b8;
  long local_b0;
  uint local_a8;
  undefined4 uStack_a4;
  char local_9c [4];
  int local_98;
  undefined4 uStack_94;
  int local_90 [4];
  long local_80;
  int local_78;
  int iStack_74;
  int local_70;
  undefined4 uStack_6c;
  long local_68;
  long lStack_60;
  long local_58;
  
  lVar3 = tpidr_el0;
  local_58 = *(long *)(lVar3 + 0x28);
  if (*(int *)(param_1 + 0x10) == 4) {
    if (*(int *)(param_1 + 0x14) == 0x2e) {
      if ((((DAT_00215d08 == '\x01') &&
           (uVar7 = FUN_001428fc(param_1,DAT_00215d10,local_90,4), (int)uVar7 != 0)) &&
          (local_90[0] == DAT_00215d18)) &&
         ((iVar5 = FUN_001428fc(uVar7,DAT_00215d20,&local_78,0x20), iVar5 != 0 &&
          (((CONCAT44(iStack_74,local_78) == DAT_00215d28 &&
            CONCAT44(uStack_6c,local_70) == DAT_00215d30) && local_68 == DAT_00215d38) &&
           lStack_60 == DAT_00215d40)))) {
        local_a8 = 0xffffffff;
        local_90[0] = 0x11ea3c;
        local_90[1] = 0;
        if ((((((int)DAT_00214938 == 1) &&
              ((DAT_00214930 != (code *)0x0 &&
               (uVar7 = (*DAT_00214930)(local_90,&local_78,1,&local_b8,&local_a8), (int)uVar7 == 1))
              )) && (local_b8 != 0)) && (((local_a8 == 0 && (local_70 == 2)) && (iStack_74 == 1))))
           && (local_b8 == *(long *)(param_1 + 0x48))) {
          if (*(int *)(param_2 + 0xb0) != 0) {
            iVar5 = *(int *)(param_1 + 0xc0);
            if (((iVar5 == 0) || (local_78 = 0, *(int *)(param_2 + 0xb0) != 1)) ||
               (((*(byte *)(param_2 + 0x29) >> 1 & 1) == 0 ||
                ((DAT_00215d08 == '\0' || (uVar7 = FUN_001550fc(), (int)uVar7 == 0))))))
            goto LAB_00159060;
            iVar6 = FUN_001428fc(uVar7,DAT_0020f718 + 0x24c,&local_78,4);
            uVar7 = 0;
            iVar2 = 0;
            if ((local_78 < 0x7d1 && 99 < local_78) && iVar6 != 0) {
              iVar2 = local_78;
            }
            if (iVar5 != iVar2) goto LAB_0015920c;
          }
LAB_00158da8:
          iVar5 = FUN_001428fc(uVar7,*(long *)(param_1 + 0x58) + 0xf9e,local_9c,1);
          uVar7 = 0;
          if ((iVar5 == 0) ||
             ((local_9c[0] != '\x01' ||
              (uVar7 = FUN_001428fc(0,*(long *)(param_1 + 0x58) + 0x918,&local_a8,8),
              (int)uVar7 == 0)))) goto LAB_0015920c;
          lVar9 = CONCAT44(uStack_a4,local_a8);
          uVar7 = 0;
          if ((lVar9 + 0x2000U < 0x12000) || ((local_a8 & 7) != 0)) goto LAB_0015920c;
          if (lVar9 == *(long *)(param_1 + 0x60)) {
            uVar7 = FUN_0013a78c(lVar9 + 0x28,&local_b0);
            if ((int)uVar7 == 0) goto LAB_0015920c;
            if (local_b0 == *(long *)(param_1 + 0x68)) {
              lVar10 = *(long *)(param_1 + 0x58);
              local_80 = *(long *)(param_1 + 0x18);
              lVar9 = lVar10 + 0xf74;
              local_90[3] = 0;
              local_90[1] = 0;
              local_90[2] = 0;
              local_90[0] = 0x3d4ccccd;
              uVar7 = FUN_001428fc(uVar7,lVar9,&local_78,4);
              if ((int)uVar7 == 0) goto LAB_0015920c;
              lVar1 = lVar10 + 4000;
              uVar7 = FUN_001428fc(uVar7,lVar1,&local_70,4);
              if ((int)uVar7 == 0) goto LAB_0015920c;
              lVar10 = lVar10 + 0xfcc;
              uVar7 = FUN_001428fc(uVar7,lVar10,&local_68,8);
              if ((int)uVar7 == 0) goto LAB_0015920c;
              lVar8 = FUN_0014bef8(DAT_001cfb4c,local_90,4,lVar9);
              if (((lVar8 == 4) && (iVar5 = FUN_001428fc(4,lVar9,&local_98,4), iVar5 != 0)) &&
                 (local_98 == local_90[0])) {
                lVar8 = FUN_0014bef8(DAT_001cfb4c,local_90 + 2,4,lVar1);
                if (((lVar8 == 4) && (iVar5 = FUN_001428fc(4,lVar1,&local_98,4), iVar5 != 0)) &&
                   (local_98 == local_90[2])) {
                  lVar8 = FUN_0014bef8(DAT_001cfb4c,&local_80,8,lVar10);
                  if (((lVar8 == 8) && (iVar5 = FUN_001428fc(8,lVar10,&local_98,8), iVar5 != 0)) &&
                     (CONCAT44(uStack_94,local_98) == local_80)) goto LAB_00159208;
                  bVar11 = false;
                  bVar4 = false;
                }
                else {
                  bVar11 = false;
                  bVar4 = true;
                }
              }
              else {
                bVar4 = false;
                bVar11 = true;
              }
              lVar8 = FUN_0014bef8(DAT_001cfb4c,&local_78,4,lVar9);
              if ((((lVar8 != 4) || (iVar5 = FUN_001428fc(4,lVar9,&local_98,4), iVar5 == 0)) ||
                  (local_98 != local_78)) ||
                 ((!bVar11 &&
                  ((((lVar9 = FUN_0014bef8(DAT_001cfb4c,&local_70,4,lVar1), lVar9 != 4 ||
                     (iVar5 = FUN_001428fc(4,lVar1,&local_98,4), iVar5 == 0)) ||
                    (local_98 != local_70)) ||
                   ((!bVar4 &&
                    (((lVar9 = FUN_0014bef8(DAT_001cfb4c,&local_68,8,lVar10), lVar9 != 8 ||
                      (iVar5 = FUN_001428fc(8,lVar10,&local_98,8), iVar5 == 0)) ||
                     (CONCAT44(uStack_94,local_98) != local_68)))))))))) {
                FUN_001417c8("fatal","movement_restore_unverified",0);
                    /* WARNING: Subroutine does not return */
                abort();
              }
            }
          }
        }
      }
    }
    else if (*(int *)(param_1 + 0x14) == 0x79) {
      local_a8 = 0xffffffff;
      local_90[0] = 0x116e97;
      local_90[1] = 0;
      if (((((int)DAT_00214938 == 1) && (DAT_00214930 != (code *)0x0)) &&
          (uVar7 = (*DAT_00214930)(local_90,&local_78,1,&local_b8,&local_a8), (int)uVar7 == 1)) &&
         (((local_b8 != 0 && (local_a8 == 0)) &&
          ((local_70 == 2 && ((iStack_74 == 1 && (local_b8 == *(long *)(param_1 + 0x48)))))))))
      goto LAB_00158da8;
    }
LAB_00159060:
    uVar7 = 0;
  }
  else {
LAB_00159208:
    uVar7 = 1;
  }
LAB_0015920c:
  if (*(long *)(lVar3 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar7);
}

/* ===== FUN_0015f500 @ 0015f500 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0015f500(long param_1)

{
  long lVar1;
  undefined4 uVar2;
  int iVar3;
  uint uVar4;
  ulong uVar5;
  undefined8 uVar6;
  undefined1 auVar7 [16];
  long local_268;
  undefined8 local_260;
  uint local_258;
  uint local_254;
  int iStack_250;
  undefined4 local_24c;
  undefined4 uStack_248;
  char *local_240;
  undefined8 uStack_238;
  ulong local_230;
  undefined8 local_228;
  long local_220;
  undefined4 local_210;
  uint uStack_20c;
  undefined4 local_208;
  undefined4 uStack_204;
  undefined4 local_200;
  undefined4 uStack_1fc;
  int local_1f0 [4];
  uint local_1e0;
  undefined4 local_1d4;
  undefined4 local_1c4;
  float local_1bc;
  float fStack_1b8;
  byte local_1b0 [16];
  long local_1a0;
  char acStack_198 [4];
  int local_194;
  int local_190;
  long local_68;
  
  lVar1 = tpidr_el0;
  local_68 = *(long *)(lVar1 + 0x28);
  if (DAT_00214cf0 != DAT_0020f650) {
    local_1a0 = 0;
    if (param_1 != 0) {
      local_1f0[0] = -1;
      local_240 = "dynaJumpEnabled";
      if ((((((((int)DAT_00214938 == 1) && (DAT_00214930 != (code *)0x0)) &&
             (iVar3 = (*DAT_00214930)(&local_240,acStack_198,1,&local_1a0,local_1f0), iVar3 == 1))
            && ((local_1a0 != 0 && (local_1f0[0] == 0)))) &&
           ((local_190 == 2 && ((local_194 == 1 && (iVar3 = FUN_001550fc(), iVar3 != 0)))))) &&
          (iVar3 = FUN_00189c88(DAT_001e0978,DAT_0020f718,FUN_001428fc,0,local_1f0), iVar3 != 0)) &&
         (iVar3 = FUN_001bc828(DAT_0020f700,FUN_001428fc,0,local_1b0), iVar3 != 0)) {
        local_230 = *(ulong *)(param_1 + 0x28);
        auVar7 = NEON_ext(_DAT_0020f6f0,_DAT_0020f6f0,8,1);
        local_228 = DAT_0020f700;
        local_220 = DAT_0020f718;
        uStack_238 = auVar7._8_8_;
        local_240 = auVar7._0_8_;
        uStack_20c = (uint)local_1b0[0];
        local_210 = 1;
        local_208 = local_1c4;
        uStack_204 = local_1d4;
        local_200 = (undefined4)(long)(local_1bc * 300.0);
        uStack_1fc = (undefined4)(long)(fStack_1b8 * 300.0);
        iVar3 = FUN_0019a13c(&DAT_00228bd0,&local_240,&local_258);
        if (iVar3 != 0) {
          if (((DAT_00214cf8 == 0) && (DAT_00214c10 == 0)) && (DAT_00214c14 == 0)) {
            local_268 = 0;
            uVar5 = FUN_001391f0(1);
            if ((((DAT_0020f718 == local_220) &&
                 (iVar3 = FUN_00165d8c(&DAT_0020f680,&local_260), iVar3 != 0)) &&
                ((iVar3 = FUN_00155ad0(), iVar3 != 0 &&
                 ((iVar3 = FUN_00155034("dynaJumpEnabled",&local_268), iVar3 != 0 &&
                  (local_268 == local_1a0)))))) &&
               ((local_230 <= uVar5 &&
                ((uVar5 - local_230 < 0x1f5 &&
                 (iVar3 = FUN_00168fe8(), uVar2 = DAT_0020d158, iVar3 != 0)))))) {
              DAT_00214c10 = 1;
              DAT_00214cf8 = 3;
              DAT_0020d158 = 3;
              uVar4 = (*(code *)(DAT_001e0978 + 0xb3d64c))
                                (local_228,local_220,local_260,local_258,local_254,local_24c,
                                 uStack_248);
              DAT_00214c10 = 0;
              DAT_00214cf8 = 0;
              DAT_00214cf0 = DAT_0020f650;
              DAT_0020d158 = uVar2;
              uVar6 = FUN_001391f0(1);
              FUN_0019a3cc(&DAT_00228bd0,uVar6,uVar4);
              if ((uVar4 & 1) != 0) {
                snprintf(acStack_198,300,",\"shot\":%u,\"raw_x\":%d,\"raw_y\":%d,\"own_gid\":%u",
                         (ulong)(iStack_250 + 1),(ulong)local_258,(ulong)local_254,(ulong)local_1e0)
                ;
                FUN_001417c8("dyna_accepted","engine_submit_skill",acStack_198);
              }
            }
            else {
              FUN_0019a3cc(&DAT_00228bd0,local_230,0xffffffff);
            }
          }
          else {
            FUN_0019a3cc(&DAT_00228bd0,local_230,0);
          }
        }
        goto LAB_0015f810;
      }
    }
    uRam0000000000228be8 = 0;
    _DAT_00228be0 = 0;
    uRam0000000000228bf8 = 0;
    _DAT_00228bf0 = 0;
    uRam0000000000228c08 = 0;
    _DAT_00228c00 = 0;
    uRam0000000000228c18 = 0;
    _DAT_00228c10 = 0;
    uRam0000000000228c28 = 0;
    _DAT_00228c20 = 0;
    uRam0000000000228c38 = 0;
    _DAT_00228c30 = 0;
    uRam0000000000228bd8 = 0;
    _DAT_00228bd0 = 0;
  }
LAB_0015f810:
  if (*(long *)(lVar1 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00194ddc @ 00194ddc [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00194ddc(long param_1,long param_2,long param_3,uint param_4)

{
  ulong uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  long lVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  size_t sVar9;
  long lVar10;
  int *piVar11;
  ulong uVar12;
  char *__s;
  int *piVar13;
  ulong uVar14;
  float fVar15;
  undefined8 uVar16;
  undefined8 uVar17;
  undefined8 uVar19;
  undefined1 auVar18 [16];
  float fVar20;
  float fVar21;
  undefined4 uVar22;
  float fVar23;
  float fVar24;
  float fVar25;
  float fVar26;
  float fVar27;
  float fVar28;
  undefined4 uVar29;
  float fVar30;
  float fVar31;
  int iVar32;
  undefined4 uVar33;
  float fVar34;
  float fVar35;
  float fVar36;
  float fVar37;
  float fVar38;
  float fVar39;
  float fVar40;
  float fVar41;
  undefined4 uVar42;
  undefined4 uVar43;
  undefined4 uVar44;
  undefined4 uVar45;
  float fVar46;
  undefined4 uVar47;
  float fVar48;
  float fVar49;
  float fVar50;
  float fVar51;
  float fVar52;
  float fVar53;
  int iVar54;
  float fVar55;
  float fVar56;
  float local_f0 [4];
  float local_e0 [4];
  float local_d0 [4];
  float local_c0 [4];
  long local_b0;
  
  lVar6 = tpidr_el0;
  local_b0 = *(long *)(lVar6 + 0x28);
  piVar13 = *(int **)(param_1 + 8);
  if ((param_4 & 1) != 0) {
    if (*(int *)(param_3 + 0x58) != 0) {
      FUN_00196680(*(undefined4 *)(param_2 + 0x870),*(undefined4 *)(param_2 + 0x874),
                   *(undefined4 *)(param_2 + 0x878),param_1,param_2,2);
    }
    uVar8 = DAT_0010e578;
    uVar7 = DAT_0010e538;
    if (*(int *)(param_2 + 0x11c) != 0) {
      uVar14 = 0;
      uVar19 = SUB168(_DAT_00112b00,8);
      uVar16 = SUB168(_DAT_00112b00,0);
      do {
        if (*(int *)(param_2 + uVar14 * 0x48 + 0x144) == 0) {
          if (((*(int *)(param_3 + 0x58) != 0) && (*(int *)(param_2 + uVar14 * 0x48 + 0x140) != 2))
             && (*(int *)(param_2 + uVar14 * 0x48 + 0x148) != 0)) {
            lVar10 = param_2 + uVar14 * 0x48;
            FUN_00196680(*(undefined4 *)(lVar10 + 0x158),*(undefined4 *)(lVar10 + 0x15c),
                         *(undefined4 *)(lVar10 + 0x164),param_1,param_2);
          }
          lVar10 = param_2 + uVar14 * 0x48;
          iVar54 = piVar13[2];
          iVar2 = *piVar13;
          iVar3 = piVar13[1];
          fVar56 = *(float *)(param_2 + 0xf8);
          fVar52 = *(float *)(param_2 + 0xfc);
          iVar4 = piVar13[3];
          fVar55 = *(float *)(lVar10 + 0x150);
          fVar53 = *(float *)(lVar10 + 0x154);
          if (*(int *)(param_3 + 0x50) != 0) {
            iVar5 = *(int *)(param_2 + uVar14 * 0x48 + 0x140);
            *(undefined4 *)(param_1 + 0x1c) = 0x3f800000;
            uVar22 = 0x3f800000;
            if (iVar5 != 1) {
              uVar22 = 0x3e4ccccd;
            }
            *(undefined4 *)(param_1 + 0x18) = uVar22;
            uVar17 = NEON_cmlt(CONCAT44((uint)(iVar5 == 0) << 0x1f,(uint)(iVar5 == 0) << 0x1f),0,4);
            uVar17 = NEON_bsl(uVar17,uVar7,uVar8,1);
            *(undefined8 *)(param_1 + 0x10) = uVar17;
            if (*(int *)(param_2 + 0x820) != 0) {
              lVar10 = param_2 + uVar14 * 0x48;
              fVar41 = *(float *)(param_2 + 0x844);
              uVar42 = *(undefined4 *)(param_2 + 0x834);
              uVar43 = *(undefined4 *)(param_2 + 0x854);
              fVar39 = *(float *)(lVar10 + 0x15c);
              fVar40 = *(float *)(lVar10 + 0x158);
              fVar38 = *(float *)(param_2 + 0x864);
              fVar25 = fVar39 * -300.0;
              fVar15 = fVar40 * 300.0;
              uVar22 = NEON_fmadd(uVar42,fVar15,fVar25 * fVar41);
              fVar23 = (float)NEON_fmadd(uVar43,0,uVar22);
              fVar23 = fVar38 + fVar23;
              if (((ABS(fVar23) != INFINITY) && (!NAN(ABS(fVar23)))) && (1e-06 < fVar23)) {
                fVar36 = *(float *)(param_2 + 0x838);
                uVar44 = *(undefined4 *)(param_2 + 0x828);
                uVar47 = *(undefined4 *)(param_2 + 0x848);
                fVar49 = *(float *)(param_2 + 0x858);
                fVar51 = *(float *)(param_2 + 0x868);
                uVar22 = NEON_fmadd(uVar44,fVar15,fVar25 * fVar36);
                fVar27 = (float)NEON_fmadd(uVar47,0,uVar22);
                fVar27 = (float)NEON_fmadd((fVar49 + fVar27) / fVar23,0x3f000000,0x3f000000);
                fVar27 = fVar51 * fVar27;
                fVar28 = ABS(fVar27);
                if (((fVar28 < 1e+06) && (fVar28 != INFINITY)) && (!NAN(fVar28))) {
                  fVar28 = *(float *)(param_2 + 0x83c);
                  uVar33 = *(undefined4 *)(param_2 + 0x84c);
                  uVar22 = *(undefined4 *)(param_2 + 0x82c);
                  uVar29 = NEON_fmadd(uVar22,fVar15,fVar25 * fVar28);
                  fVar30 = (float)NEON_fmadd(uVar33,0,uVar29);
                  fVar34 = *(float *)(param_2 + 0x85c);
                  fVar23 = (float)NEON_fmadd((fVar34 + fVar30) / fVar23,0x3f000000,0x3f000000);
                  fVar30 = *(float *)(param_2 + 0x86c);
                  fVar23 = fVar30 * (1.0 - fVar23);
                  if ((ABS(fVar23) != INFINITY) && (ABS(fVar23) < 1e+06)) {
                    fVar26 = (fVar40 + 1.0) * 300.0;
                    uVar29 = NEON_fmadd(uVar42,fVar26,fVar25 * fVar41);
                    fVar20 = (float)NEON_fmadd(uVar43,0,uVar29);
                    fVar20 = fVar38 + fVar20;
                    if ((ABS(fVar20) != INFINITY) && ((!NAN(ABS(fVar20)) && (1e-06 < fVar20)))) {
                      uVar29 = NEON_fmadd(uVar44,fVar26,fVar25 * fVar36);
                      fVar21 = (float)NEON_fmadd(uVar47,0,uVar29);
                      fVar21 = (float)NEON_fmadd((fVar49 + fVar21) / fVar20,0x3f000000,0x3f000000);
                      fVar24 = ABS(fVar51 * fVar21);
                      if ((fVar24 < 1e+06) && ((fVar24 != INFINITY && (!NAN(fVar24))))) {
                        uVar29 = NEON_fmadd(uVar22,fVar26,fVar25 * fVar28);
                        fVar25 = (float)NEON_fmadd(uVar33,0,uVar29);
                        fVar25 = (float)NEON_fmadd((fVar34 + fVar25) / fVar20,0x3f000000,0x3f000000)
                        ;
                        fVar25 = fVar30 * (1.0 - fVar25);
                        fVar20 = ABS(fVar25);
                        if ((fVar20 != INFINITY) && (fVar20 < 1e+06)) {
                          fVar20 = (fVar39 + 1.0) * -300.0;
                          uVar29 = NEON_fmadd(uVar42,fVar15,fVar20 * fVar41);
                          fVar26 = (float)NEON_fmadd(uVar43,0,uVar29);
                          fVar26 = fVar38 + fVar26;
                          if (((ABS(fVar26) != INFINITY) && (!NAN(ABS(fVar26)))) && (1e-06 < fVar26)
                             ) {
                            uVar29 = NEON_fmadd(uVar44,fVar15,fVar20 * fVar36);
                            fVar24 = (float)NEON_fmadd(uVar47,0,uVar29);
                            fVar24 = (float)NEON_fmadd((fVar49 + fVar24) / fVar26,0x3f000000,
                                                       0x3f000000);
                            fVar31 = ABS(fVar51 * fVar24);
                            if (((fVar31 < 1e+06) && (fVar31 != INFINITY)) && (!NAN(fVar31))) {
                              uVar29 = NEON_fmadd(uVar22,fVar15,fVar20 * fVar28);
                              fVar15 = (float)NEON_fmadd(uVar33,0,uVar29);
                              fVar15 = (float)NEON_fmadd((fVar34 + fVar15) / fVar26,0x3f000000,
                                                         0x3f000000);
                              fVar20 = fVar30 * (1.0 - fVar15);
                              fVar15 = ABS(fVar20);
                              if ((fVar15 != INFINITY) && (fVar15 < 1e+06)) {
                                fVar15 = hypotf(fVar51 * fVar21 - fVar27,fVar25 - fVar23);
                                fVar23 = hypotf(fVar51 * fVar24 - fVar27,fVar20 - fVar23);
                                fVar15 = (fVar15 + fVar23) * 0.5;
                                if ((ABS(fVar15) != INFINITY) &&
                                   ((!NAN(ABS(fVar15)) && (1.0 <= fVar15)))) {
                                  piVar11 = *(int **)(param_1 + 8);
                                  iVar5 = piVar11[3];
                                  fVar23 = (float)piVar11[2] / *(float *)(param_2 + 0xf8);
                                  fVar25 = (float)iVar5 / *(float *)(param_2 + 0xfc);
                                  fVar15 = (float)NEON_fmin(*(float *)(param_2 + uVar14 * 0x48 +
                                                                      0x160) / fVar15,0x3fc00000);
                                  fVar27 = -fVar15;
                                  fVar20 = fVar15 * 3.0 * 300.0;
                                  uVar12 = 0;
                                  do {
                                    fVar26 = fVar27;
                                    if (uVar12 != 3 && uVar12 != 0) {
                                      fVar26 = fVar15;
                                    }
                                    fVar21 = fVar27;
                                    if (1 < uVar12) {
                                      fVar21 = fVar15;
                                    }
                                    fVar21 = (fVar39 + fVar21) * -300.0;
                                    fVar26 = (fVar40 + fVar26) * 300.0;
                                    uVar29 = NEON_fmadd(uVar42,fVar26,fVar41 * fVar21);
                                    fVar24 = (float)NEON_fmadd(uVar43,0,uVar29);
                                    fVar24 = fVar38 + fVar24;
                                    if (((ABS(fVar24) == INFINITY) || (NAN(ABS(fVar24)))) ||
                                       (fVar24 <= 1e-06)) goto LAB_001957d0;
                                    uVar45 = NEON_fmadd(uVar44,fVar26,fVar36 * fVar21);
                                    uVar29 = NEON_fmadd(uVar22,fVar26,fVar28 * fVar21);
                                    fVar21 = (float)NEON_fmadd(uVar47,0,uVar45);
                                    fVar26 = (float)NEON_fmadd(uVar33,0,uVar29);
                                    fVar31 = (float)NEON_fmadd((fVar49 + fVar21) / fVar24,0x3f000000
                                                               ,0x3f000000);
                                    fVar31 = fVar51 * fVar31;
                                    local_d0[uVar12] = fVar31;
                                    fVar26 = (float)NEON_fmadd((fVar34 + fVar26) / fVar24,0x3f000000
                                                               ,0x3f000000);
                                    fVar21 = ABS(fVar31);
                                    fVar26 = fVar30 * (1.0 - fVar26);
                                    local_f0[uVar12] = fVar26;
                                    if (((1e+06 <= fVar21) || (fVar21 == INFINITY)) ||
                                       ((NAN(fVar21) ||
                                        ((ABS(fVar26) == INFINITY || (1e+06 <= ABS(fVar26)))))))
                                    goto LAB_001957d0;
                                    uVar1 = uVar12 + 1;
                                    iVar32 = *piVar11;
                                    fVar35 = (float)(piVar11[1] + iVar5);
                                    fVar21 = (float)NEON_fmadd(fVar31,fVar23,(float)iVar32);
                                    fVar26 = (float)NEON_fmsub(fVar26,fVar25,fVar35);
                                    local_d0[uVar12] = fVar21;
                                    fVar31 = local_d0[1];
                                    fVar24 = local_d0[0];
                                    local_f0[uVar12] = fVar26;
                                    fVar21 = local_f0[1];
                                    fVar26 = local_f0[0];
                                    uVar12 = uVar1;
                                  } while (uVar1 != 4);
                                  uVar12 = 0;
                                  do {
                                    fVar37 = fVar27;
                                    if (uVar12 != 3 && uVar12 != 0) {
                                      fVar37 = fVar15;
                                    }
                                    fVar46 = fVar27;
                                    if (1 < uVar12) {
                                      fVar46 = fVar15;
                                    }
                                    fVar46 = (fVar39 + fVar46) * -300.0;
                                    fVar37 = (fVar40 + fVar37) * 300.0;
                                    uVar29 = NEON_fmadd(uVar42,fVar37,fVar41 * fVar46);
                                    fVar48 = (float)NEON_fmadd(uVar43,fVar20,uVar29);
                                    fVar48 = fVar38 + fVar48;
                                    if (((ABS(fVar48) == INFINITY) || (NAN(ABS(fVar48)))) ||
                                       (fVar48 <= 1e-06)) goto LAB_001957d0;
                                    uVar29 = NEON_fmadd(uVar44,fVar37,fVar36 * fVar46);
                                    fVar50 = (float)NEON_fmadd(uVar47,fVar20,uVar29);
                                    uVar29 = NEON_fmadd(uVar22,fVar37,fVar28 * fVar46);
                                    fVar37 = (float)NEON_fmadd(uVar33,fVar20,uVar29);
                                    fVar46 = (float)NEON_fmadd((fVar49 + fVar50) / fVar48,0x3f000000
                                                               ,0x3f000000);
                                    fVar46 = fVar51 * fVar46;
                                    fVar37 = (float)NEON_fmadd((fVar34 + fVar37) / fVar48,0x3f000000
                                                               ,0x3f000000);
                                    fVar48 = ABS(fVar46);
                                    local_c0[uVar12] = fVar46;
                                    fVar37 = fVar30 * (1.0 - fVar37);
                                    local_e0[uVar12] = fVar37;
                                    if (((1e+06 <= fVar48) || (fVar48 == INFINITY)) ||
                                       ((NAN(fVar48) ||
                                        ((ABS(fVar37) == INFINITY || (1e+06 <= ABS(fVar37)))))))
                                    goto LAB_001957d0;
                                    fVar46 = (float)NEON_fmadd(fVar46,fVar23,(float)iVar32);
                                    fVar37 = (float)NEON_fmsub(fVar37,fVar25,fVar35);
                                    uVar1 = uVar12 + 1;
                                    local_c0[uVar12] = fVar46;
                                    local_e0[uVar12] = fVar37;
                                    uVar12 = uVar1;
                                  } while (uVar1 != 4);
                                  FUN_00193efc(local_d0[0],local_f0[0],local_d0[1],local_f0[1],
                                               param_1,2);
                                  fVar39 = local_c0[1];
                                  fVar27 = local_c0[0];
                                  fVar23 = local_e0[1];
                                  fVar25 = local_e0[0];
                                  FUN_00193efc(local_c0[0],local_e0[0],local_c0[1],local_e0[1],
                                               param_1,2);
                                  FUN_00193efc(fVar24,fVar26,fVar27,fVar25,param_1,2);
                                  fVar40 = local_d0[2];
                                  fVar15 = local_f0[2];
                                  FUN_00193efc(fVar31,fVar21,local_d0[2],local_f0[2],param_1,2);
                                  fVar28 = local_c0[2];
                                  fVar38 = local_e0[2];
                                  FUN_00193efc(fVar39,fVar23,local_c0[2],local_e0[2],param_1,2);
                                  FUN_00193efc(fVar31,fVar21,fVar39,fVar23,param_1,2);
                                  fVar41 = local_d0[3];
                                  fVar23 = local_f0[3];
                                  FUN_00193efc(fVar40,fVar15,local_d0[3],local_f0[3],param_1,2);
                                  fVar36 = local_c0[3];
                                  fVar39 = local_e0[3];
                                  FUN_00193efc(fVar28,fVar38,local_c0[3],local_e0[3],param_1,2);
                                  FUN_00193efc(fVar40,fVar15,fVar28,fVar38,param_1,2);
                                  FUN_00193efc(fVar41,fVar23,fVar24,fVar26,param_1,2);
                                  FUN_00193efc(fVar36,fVar39,fVar27,fVar25,param_1,2);
                                  FUN_00193efc(fVar41,fVar23,fVar36,fVar39,param_1,2);
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
LAB_001957d0:
          if (((*(int *)(param_3 + 0x54) != 0) && (*(int *)(param_2 + uVar14 * 0x48 + 0x140) == 0))
             && (*(int *)(param_2 + uVar14 * 0x48 + 0x148) != 0)) {
            *(undefined8 *)(param_1 + 0x18) = uVar19;
            *(undefined8 *)(param_1 + 0x10) = uVar16;
            FUN_00193efc((*(float *)(param_2 + 0x100) * (float)piVar13[2]) /
                         *(float *)(param_2 + 0xf8) + (float)*piVar13,
                         (float)(piVar13[3] + piVar13[1]) -
                         (*(float *)(param_2 + 0x104) * (float)piVar13[3]) /
                         *(float *)(param_2 + 0xfc),
                         (float)(iVar2 + (int)((fVar55 * (float)iVar54) / fVar56)),
                         (float)((iVar4 + iVar3) - (int)((fVar53 * (float)iVar4) / fVar52)),*piVar13
                         ,param_1,2);
          }
        }
        uVar14 = uVar14 + 1;
      } while (uVar14 < *(uint *)(param_2 + 0x11c));
    }
  }
  if ((((param_4 >> 1 & 1) == 0) || (*(int *)(param_3 + 0x10) == 0)) ||
     ((*(int *)(param_3 + 0x1c) == 0 || (*(int *)(param_2 + 0x7e8) == 0)))) {
switchD_00195910_caseD_f4244d:
    if (*(long *)(lVar6 + 0x28) == local_b0) {
      return;
    }
  }
  else {
    iVar3 = *(int *)(param_2 + 0x7ec);
    iVar2 = iVar3 + -1000000;
    if (999999 < iVar3 + 0xfefc99c0U) {
      iVar2 = iVar3;
    }
    __s = "SHELLY";
    switch(iVar2) {
    case 16000000:
    case 0xf42421:
    case 0xf42437:
    case 0xf424ac:
    case 0xf424ae:
    case 0xf424b6:
    case 0xf424cc:
      break;
    case 0xf42401:
      __s = "COLT";
      break;
    case 0xf42402:
      __s = "BULL";
      break;
    case 0xf42403:
      __s = "BROCK";
      break;
    case 0xf42404:
      __s = "RICO";
      break;
    case 0xf42405:
    case 0xf42524:
      __s = "SPIKE";
      break;
    case 0xf42406:
      __s = "BARLEY";
      break;
    case 0xf42407:
      __s = "JESSIE";
      break;
    case 0xf42408:
      __s = "NITA";
      break;
    case 0xf42409:
      __s = "DYNAMIKE";
      break;
    case 0xf4240a:
      __s = "EL PRIMO";
      break;
    case 0xf4240b:
      __s = "MORTIS";
      break;
    case 0xf4240c:
      __s = "CROW";
      break;
    case 0xf4240d:
      __s = "POCO";
      break;
    case 0xf4240e:
      __s = "BO";
      break;
    case 0xf4240f:
      __s = "PIPER";
      break;
    case 0xf42410:
      __s = "PAM";
      break;
    case 0xf42411:
      __s = "TARA";
      break;
    case 0xf42412:
      __s = "DARRYL";
      break;
    case 0xf42413:
      __s = "PENNY";
      break;
    case 0xf42414:
      __s = "FRANK";
      break;
    case 0xf42415:
      __s = "GENE";
      break;
    case 0xf42416:
      __s = "TICK";
      break;
    case 0xf42417:
    case 0xf424fe:
      __s = "LEON";
      break;
    case 0xf42418:
      __s = "ROSA";
      break;
    case 0xf42419:
      __s = "CARL";
      break;
    case 0xf4241a:
      __s = "BIBI";
      break;
    case 0xf4241b:
    case 0xf424e3:
      __s = "8-BIT";
      break;
    case 0xf4241c:
      __s = "SANDY";
      break;
    case 0xf4241d:
      __s = "BEA";
      break;
    case 0xf4241e:
      __s = "EMZ";
      break;
    case 0xf4241f:
      __s = "MR. P";
      break;
    case 0xf42420:
      __s = "MAX";
      break;
    case 0xf42422:
      __s = "JACKY";
      break;
    case 0xf42423:
      __s = "GALE";
      break;
    case 0xf42424:
      __s = "NANI";
      break;
    case 0xf42425:
      __s = "SPROUT";
      break;
    case 0xf42426:
      __s = "SURGE";
      break;
    case 0xf42427:
      __s = "COLETTE";
      break;
    case 0xf42428:
      __s = "AMBER";
      break;
    case 0xf42429:
      __s = "LOU";
      break;
    case 0xf4242a:
      __s = "BYRON";
      break;
    case 0xf4242b:
      __s = "EDGAR";
      break;
    case 0xf4242c:
      __s = "RUFFS";
      break;
    case 0xf4242d:
      __s = "STU";
      break;
    case 0xf4242e:
      __s = "BELLE";
      break;
    case 0xf4242f:
      __s = "SQUEAK";
      break;
    case 0xf42430:
      __s = "GROM";
      break;
    case 0xf42431:
      __s = "BUZZ";
      break;
    case 0xf42432:
      __s = "GRIFF";
      break;
    case 0xf42433:
      __s = "ASH";
      break;
    case 0xf42434:
    case 0xf424c9:
      __s = "MEG";
      break;
    case 0xf42435:
      __s = "LOLA";
      break;
    case 0xf42436:
      __s = "FANG";
      break;
    case 0xf42438:
      __s = "EVE";
      break;
    case 0xf42439:
      __s = "JANET";
      break;
    case 0xf4243a:
    case 0xf424d5:
      __s = "BONNIE";
      break;
    case 0xf4243b:
      __s = "OTIS";
      break;
    case 0xf4243c:
      __s = "SAM";
      break;
    case 0xf4243d:
      __s = "GUS";
      break;
    case 0xf4243e:
      __s = "BUSTER";
      break;
    case 0xf4243f:
      __s = "CHESTER";
      break;
    case 0xf42440:
      __s = "GRAY";
      break;
    case 0xf42441:
      __s = "MANDY";
      break;
    case 0xf42442:
    case 0xf424e4:
      __s = "R-T";
      break;
    case 0xf42443:
      __s = "WILLOW";
      break;
    case 0xf42444:
      __s = "MAISIE";
      break;
    case 0xf42445:
      __s = "HANK";
      break;
    case 0xf42446:
      __s = "CORDELIUS";
      break;
    case 0xf42447:
      __s = "DOUG";
      break;
    case 0xf42448:
      __s = "PEARL";
      break;
    case 0xf42449:
      __s = "CHUCK";
      break;
    case 0xf4244a:
      __s = "CHARLIE";
      break;
    case 0xf4244b:
      __s = "MICO";
      break;
    case 0xf4244c:
    case 0xf4257d:
      __s = "KIT";
      break;
    default:
      goto switchD_00195910_caseD_f4244d;
    case 0xf4244e:
      __s = "MELODIE";
      break;
    case 0xf4244f:
      __s = "ANGELO";
      break;
    case 0xf42450:
      __s = "DRACO";
      break;
    case 0xf42451:
      __s = "LILY";
      break;
    case 0xf42452:
      __s = "BERRY";
      break;
    case 0xf42453:
      __s = "CLANCY";
      break;
    case 0xf42454:
      __s = "MOE";
      break;
    case 0xf42455:
    case 0xf424fd:
      __s = "KENJI";
      break;
    case 0xf42456:
      __s = "SHADE";
      break;
    case 0xf42457:
      __s = "JUJU";
      break;
    case 0xf42458:
      __s = "BUZZ LIGHTYEAR";
      break;
    case 0xf42459:
      __s = "MEEPLE";
      break;
    case 0xf4245a:
      __s = "OLLIE";
      break;
    case 0xf4245b:
      __s = "LUMI";
      break;
    case 0xf4245c:
      __s = "FINX";
      break;
    case 0xf4245d:
      __s = "JAE-YONG";
      break;
    case 0xf4245e:
      __s = "KAZE";
      break;
    case 0xf4245f:
      __s = "ALLI";
      break;
    case 0xf42460:
      __s = "TRUNK";
      break;
    case 0xf42461:
    case 0xf425b7:
      __s = "MINA";
      break;
    case 0xf42462:
      __s = "ZIGGY";
      break;
    case 0xf42463:
      __s = "PIERCE";
      break;
    case 0xf42464:
      __s = "GIGI";
      break;
    case 0xf42465:
      __s = "GLOWY";
      break;
    case 0xf42466:
    case 0xf425b8:
      __s = "SIRIUS";
      break;
    case 0xf42467:
      __s = "NAJIA";
      break;
    case 0xf42468:
      __s = "DAMIAN";
      break;
    case 0xf42469:
      __s = "STARR NOVA";
      break;
    case 0xf4246a:
      __s = "BOLT";
      break;
    case 0xf4246b:
      __s = "NORI";
      break;
    case 0xf4246c:
      __s = "WENDY";
      break;
    case 0xf4246d:
      __s = "COSMO";
      break;
    case 0xf4246e:
      __s = "VINCE";
      break;
    case 0xf424f2:
      __s = "DRILLER";
    }
    iVar2 = piVar13[2];
    iVar3 = *piVar13;
    if (iVar2 < 0) {
      iVar2 = iVar2 + 1;
    }
    sVar9 = strlen(__s);
    uVar7 = _DAT_001127e0;
    iVar4 = piVar13[1];
    iVar54 = piVar13[3];
    iVar2 = iVar3 + (iVar2 >> 1) + (int)sVar9 * -6;
    *(undefined8 *)(param_1 + 0x18) = _UNK_001127e8;
    *(undefined8 *)(param_1 + 0x10) = uVar7;
    sVar9 = strlen(__s);
    FUN_00193d88(param_1,iVar2 + -8,iVar54 + iVar4 + -0x6e,(int)sVar9 * 0xc + 0x10,0x18,1);
    auVar18 = NEON_fmov(0x3f800000,4);
    *(long *)(param_1 + 0x18) = auVar18._8_8_;
    *(long *)(param_1 + 0x10) = auVar18._0_8_;
    if (*(long *)(lVar6 + 0x28) == local_b0) {
      FUN_00194be4(param_1,__s,iVar2,iVar54 + iVar4 + -0x69);
      return;
    }
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0016bae0 @ 0016bae0 [libNexusUI69252.so] ===== */

int FUN_0016bae0(char *param_1)

{
  int iVar1;
  int iVar2;
  
  iVar1 = strcmp(param_1,"KILL AURA");
  if (iVar1 != 0) {
    iVar1 = strcmp(param_1,"SMART AIM");
    if (iVar1 == 0) {
      iVar1 = 1;
    }
    else {
      iVar1 = strcmp(param_1,"AUTO DODGE");
      if (iVar1 == 0) {
        iVar1 = 2;
      }
      else {
        iVar1 = strcmp(param_1,"AUTO FARM");
        if (iVar1 == 0) {
          iVar1 = 3;
        }
        else {
          iVar1 = strcmp(param_1,"X-RAY");
          if (iVar1 == 0) {
            iVar1 = 4;
          }
          else {
            iVar1 = strcmp(param_1,"FOLLOW");
            if (iVar1 == 0) {
              iVar1 = 5;
            }
            else {
              iVar1 = strcmp(param_1,"SPIN");
              if (iVar1 == 0) {
                iVar1 = 6;
              }
              else {
                iVar1 = strcmp(param_1,"HOLD FIRE");
                if (iVar1 == 0) {
                  iVar1 = 7;
                }
                else {
                  iVar1 = strcmp(param_1,"BALL ASSIST");
                  if (iVar1 == 0) {
                    iVar1 = 8;
                  }
                  else {
                    iVar1 = strcmp(param_1,"NANI ULTI MOD");
                    if (iVar1 == 0) {
                      iVar1 = 9;
                    }
                    else {
                      iVar1 = strcmp(param_1,"ESP");
                      if (iVar1 == 0) {
                        iVar1 = 10;
                      }
                      else {
                        iVar1 = strcmp(param_1,"SPEED EXPLOIT");
                        if (iVar1 == 0) {
                          iVar1 = 0xb;
                        }
                        else {
                          iVar1 = strcmp(param_1,"AUTO PIN");
                          if (iVar1 == 0) {
                            iVar1 = 0xc;
                          }
                          else {
                            iVar1 = strcmp(param_1,"COLT MOD");
                            if (iVar1 == 0) {
                              iVar1 = 0xd;
                            }
                            else {
                              iVar2 = strcmp(param_1,"BOLT MOD");
                              iVar1 = 0xe;
                              if (iVar2 != 0) {
                                iVar1 = -1;
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
  }
  return iVar1;
}

