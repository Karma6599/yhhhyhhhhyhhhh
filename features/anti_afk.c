/*
 * Anti-AFK — Feature
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
 * Notes: AFK evasion automation.
 */

/* ===== FUN_0015ec50 @ 0015ec50 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0015ec50(long param_1)

{
  long lVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  undefined8 uVar6;
  uint local_148;
  undefined4 uStack_144;
  long local_140;
  char acStack_138 [4];
  int local_134;
  int local_130;
  char *local_80;
  undefined8 uStack_78;
  undefined *puStack_70;
  undefined *puStack_68;
  undefined *local_60;
  undefined *puStack_58;
  undefined *local_50;
  long local_48;
  
  iVar5 = DAT_00209cd8;
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  if ((((param_1 != 0) && (*(int *)(param_1 + 0x24) != 0)) && (DAT_00209cd8 != 0)) &&
     ((iVar4 = gettid(), iVar5 == iVar4 && (DAT_0020f6cc != 0)))) {
    local_148 = 0xffffffff;
    local_80 = "antiAfkEnabled";
    if (((((int)DAT_00214938 == 1) &&
         ((iVar5 = (*DAT_00214930)(&local_80,acStack_138,1,&local_140,&local_148), iVar5 == 1 &&
          (local_140 != 0)))) && (local_148 == 0)) &&
       ((((local_130 == 2 && (local_134 == 1)) &&
         (uVar6 = FUN_00150bf0(DAT_0020f670,DAT_0020f680), (int)uVar6 != 0)) &&
        (((iVar5 = FUN_001428fc(uVar6,DAT_0020f708 + 0x58,&local_148,8), uVar3 = DAT_0020f750,
          uVar2 = DAT_0020f74c, iVar5 != 0 && (0x11fff < CONCAT44(uStack_144,local_148) + 0x2000U))
         && ((local_148 & 7) == 0)))))) {
      if ((DAT_002285e0 == DAT_0020f6f8) && (DAT_002285e8 == DAT_0020f690)) {
        DAT_002288d0 = DAT_002285e8;
        if (DAT_00228880 == '\x01') {
          DAT_002288f8 = (uint)(0x3f < (ulong)(((long)(int)DAT_0020f750 - (long)DAT_002285fc) *
                                               ((long)(int)DAT_0020f750 - (long)DAT_002285fc) +
                                              ((long)(int)DAT_0020f74c - (long)DAT_002285f8) *
                                              ((long)(int)DAT_0020f74c - (long)DAT_002285f8)));
        }
        else {
          DAT_002288f8 = 0;
        }
      }
      else {
        DAT_002288f8 = 0;
        DAT_002285f0 = 0;
        DAT_00228880 = '\0';
        DAT_002288d0 = DAT_0020f690;
      }
      DAT_002288a0 = *(undefined8 *)(param_1 + 0x28);
      DAT_002288f4 = 1;
      if (DAT_00215a60 == '\x01') {
        if (DAT_00228398 == DAT_0020f650) {
          DAT_002288f4 = 1;
          if (((DAT_002283a0 == DAT_0020f6f8) && (DAT_00228480 != 2)) && (DAT_00228534 == 0)) {
            DAT_002288f4 = (uint)(DAT_0022856c != 0);
          }
        }
        else {
          DAT_002288f4 = 1;
        }
      }
      uRam00000000002288c0 = DAT_0020f680;
      _DAT_002288b8 = DAT_0020f678;
      DAT_00228898 = local_140;
      DAT_002288a8 = DAT_0020f670;
      DAT_002288b0 = DAT_0020f708;
      DAT_00228888 = DAT_0020f6f8;
      DAT_00228890 = DAT_0020f6f0;
      DAT_002288dc = 0;
      DAT_002288d4 = 0;
      DAT_002288e8 = DAT_0020f74c;
      DAT_002288ec = DAT_0020f750;
      uStack_78 = _UNK_001c5808;
      local_80 = _DAT_001c5800;
      puStack_68 = PTR_FUN_001c5818;
      puStack_70 = PTR_FUN_001c5810;
      DAT_002288f0 = 1;
      DAT_00228900 = DAT_002285f0;
      puStack_58 = PTR_FUN_001c5828;
      local_60 = PTR_FUN_001c5820;
      local_50 = PTR_FUN_001c5830;
      DAT_002288c8 = CONCAT44(uStack_144,local_148);
      iVar5 = FUN_00199bbc(&DAT_002287d8,&DAT_00228888,&local_80);
      if (iVar5 != 0) {
        snprintf(acStack_138,0xb4,",\"queued\":%llu,\"x\":%d,\"y\":%d,\"local_moves\":0",
                 DAT_00228870,(ulong)uVar2,(ulong)uVar3);
        FUN_001417c8("anti_afk","idle_current_position",acStack_138);
      }
      goto LAB_0015ee04;
    }
  }
  FUN_00199bbc(&DAT_002287d8,0,0);
LAB_0015ee04:
  if (*(long *)(lVar1 + 0x28) != local_48) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

/* ===== FUN_00168d08 @ 00168d08 [libNexusEvasionRuntime69252.so] ===== */

void FUN_00168d08(undefined8 param_1,long *param_2,int param_3)

{
  long lVar1;
  int iVar2;
  int iVar3;
  ulong uVar4;
  ulong uVar5;
  float fVar6;
  timespec local_98;
  int local_88;
  float local_64;
  float local_60;
  int local_54;
  int local_50;
  undefined4 uStack_4c;
  long local_48;
  char *local_40;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  iVar2 = clock_gettime(1,&local_98);
  iVar3 = DAT_00209cd8;
  if (iVar2 == 0) {
    uVar5 = CONCAT44(local_98.tv_sec._4_4_,(undefined4)local_98.tv_sec) * 1000 +
            CONCAT44(local_98.tv_nsec._4_4_,(int)local_98.tv_nsec) / 1000000;
  }
  else {
    uVar5 = 0;
  }
  if ((((param_3 == 2) && (DAT_00209cd8 != 0)) && (iVar2 = gettid(), iVar3 == iVar2)) &&
     ((int)DAT_001dfff0 != 0)) {
    local_50 = -1;
    local_40 = "antiAfkEnabled";
    if ((((((int)DAT_00214938 == 1) &&
          (iVar3 = (*DAT_00214930)(&local_40,&local_98,1,&local_48,&local_50), iVar3 == 1)) &&
         ((local_48 != 0 && ((local_50 == 0 && ((int)local_98.tv_nsec == 2)))))) &&
        (local_98.tv_sec._4_4_ == 1)) && (local_48 == param_2[2])) {
      uVar4 = 0;
      if ((((uVar5 < (ulong)param_2[3]) || (500 < uVar5 - param_2[3])) ||
          (uVar4 = FUN_00158080(0), (int)uVar4 == 0)) || (uVar4 = FUN_00168fe8(), (int)uVar4 == 0))
      goto LAB_00168fc0;
      iVar3 = FUN_00168c90();
      if (((iVar3 == 0) && (DAT_0020f6f8 == *param_2)) && (DAT_0020f6f0 == param_2[1])) {
        uVar4 = FUN_00150bf0(param_2[4],param_2[7]);
        if (((int)uVar4 == 0) ||
           (uVar4 = FUN_00189c88(DAT_001e0978,param_2[7],FUN_001428fc,0,&local_98), (int)uVar4 == 0)
           ) goto LAB_00168fc0;
        if ((local_88 == (int)param_2[9]) &&
           ((fVar6 = ABS(local_64 + (float)DAT_002288e8 / -300.0),
            fVar6 == 1e-05 || fVar6 < 1e-05 != NAN(fVar6) &&
            (fVar6 = ABS(local_60 + (float)DAT_002288ec / -300.0),
            fVar6 == 1e-05 || fVar6 < 1e-05 != NAN(fVar6))))) {
          uVar4 = FUN_0013a78c(param_2[5] + 0x58,&local_40);
          if ((int)uVar4 == 0) goto LAB_00168fc0;
          if (local_40 == (char *)param_2[8]) {
            uVar4 = FUN_0013a78c(local_40 + 0x20,&local_50);
            if ((int)uVar4 != 0) {
              iVar3 = FUN_001428fc(uVar4,CONCAT44(uStack_4c,local_50) + 0xc,&local_54,4);
              uVar4 = 0;
              if (((iVar3 != 0) && (-1 < local_54)) && (local_54 < 0x1e)) {
                uVar4 = (ulong)(uVar5 <= DAT_002285f0 - 1U || 0x15e < uVar5 - DAT_002285f0);
              }
            }
            goto LAB_00168fc0;
          }
        }
      }
    }
  }
  uVar4 = 0;
LAB_00168fc0:
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar4);
}

/* ===== FUN_00173b54 @ 00173b54 [libNexusEvasionRuntime69252.so] ===== */

void FUN_00173b54(long param_1)

{
  long lVar1;
  int iVar2;
  undefined8 uVar3;
  long local_50;
  int local_44;
  int local_40;
  int local_3c;
  int local_38;
  char *local_30;
  long local_28;
  
  lVar1 = tpidr_el0;
  local_28 = *(long *)(lVar1 + 0x28);
  if (param_1 != 0) {
    local_44 = -1;
    local_30 = "antiAfkEnabled";
    if (((((((int)DAT_00214938 == 1) && (DAT_00214930 != (code *)0x0)) &&
          (iVar2 = (*DAT_00214930)(&local_30,&local_40,1,&local_50,&local_44), iVar2 == 1)) &&
         ((local_50 != 0 && (local_44 == 0)))) &&
        ((local_38 == 2 && ((local_3c == 1 && (uVar3 = FUN_001550fc(), (int)uVar3 != 0)))))) &&
       ((iVar2 = FUN_001428fc(uVar3,DAT_0022b6f8,&local_40,4), iVar2 != 0 &&
        (local_40 == DAT_0022b864)))) {
      *(ulong *)(param_1 + 0x40) = *(ulong *)(param_1 + 0x40) & 0xfffffffffffffffe;
    }
  }
  if (*(long *)(lVar1 + 0x28) == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

