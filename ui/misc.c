/*
 * misc — UI subsystem
 * Decompiled with Ghidra 11.3.2 (arm64 pseudocode) from: libNexusUI69252.so
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
 * Notes: Unclassified remainder.
 */

/* ===== FUN_0014c4c0 @ 0014c4c0 ===== */

void FUN_0014c4c0(void)

{
  return;
}

/* ===== FUN_0014c4d0 @ 0014c4d0 ===== */

void FUN_0014c4d0(code *UNRECOVERED_JUMPTABLE)

{
  if (UNRECOVERED_JUMPTABLE != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x0014c4dc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*UNRECOVERED_JUMPTABLE)();
    return;
  }
  return;
}

/* ===== FUN_0014c4e4 @ 0014c4e4 ===== */

void FUN_0014c4e4(undefined8 param_1)

{
  __cxa_atexit(FUN_0014c4d0,param_1,&PTR_LOOP_001989f0);
  return;
}

/* ===== FUN_0014c500 @ 0014c500 ===== */

void FUN_0014c500(void)

{
  __register_atfork();
  return;
}

/* ===== FUN_0014cd5c @ 0014cd5c ===== */

int FUN_0014cd5c(void)

{
  undefined4 uVar1;
  long lVar2;
  bool bVar3;
  char cVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  undefined4 *puVar8;
  ulong uVar9;
  long *local_80;
  timespec local_78;
  long local_68;
  
  lVar2 = tpidr_el0;
  local_68 = *(long *)(lVar2 + 0x28);
  puVar8 = (undefined4 *)__errno();
  uVar1 = *puVar8;
  iVar7 = -1;
  local_80 = (long *)0x0;
  iVar5 = iVar7;
  if (((DAT_0022d860 != (long *)0x0) && (DAT_0022d868 != 0)) && (DAT_0022d870 != 0)) {
    iVar5 = clock_gettime(1,&local_78);
    if (iVar5 == 0) {
      uVar9 = local_78.tv_sec * 1000 + (ulong)local_78.tv_nsec / 1000000;
    }
    else {
      uVar9 = 0;
    }
    if ((DAT_0022d878 <= uVar9) || (iVar5 = DAT_001a7c00, 1000 < DAT_0022d878 - uVar9)) {
      DAT_0022d878 = uVar9 + 1000;
      if (0xfffffffffffffc17 < uVar9) {
        DAT_0022d878 = 0xffffffffffffffff;
      }
      DAT_001a7c00 = -1;
      iVar6 = (**(code **)(*DAT_0022d860 + 0x30))(DAT_0022d860,&local_80,0x10006);
      if (iVar6 == 0) {
        bVar3 = true;
      }
      else {
        iVar5 = iVar7;
        if ((iVar6 != -2) ||
           (iVar6 = (**(code **)(*DAT_0022d860 + 0x20))(DAT_0022d860,&local_80,0), iVar6 != 0))
        goto LAB_0014ceec;
        bVar3 = false;
      }
      iVar5 = (**(code **)(*local_80 + 0x408))(local_80,DAT_0022d868,DAT_0022d870);
      cVar4 = (**(code **)(*local_80 + 0x720))();
      if (cVar4 != '\0') {
        (**(code **)(*local_80 + 0x88))();
        iVar5 = -1;
      }
      if (iVar5 < 0) {
        iVar5 = -1;
      }
      DAT_001a7c00 = iVar5;
      if (!bVar3) {
        (**(code **)(*DAT_0022d860 + 0x28))();
      }
    }
  }
LAB_0014ceec:
  iVar7 = clock_gettime(1,&local_78);
  if (iVar7 == 0) {
    uVar9 = local_78.tv_sec * 1000 + (ulong)local_78.tv_nsec / 1000000;
    if ((DAT_0022d858 - 1 < uVar9) && (uVar9 - DAT_0022d858 >> 3 < 0x271)) goto LAB_0014cf48;
  }
  else {
    uVar9 = 0;
  }
  DAT_0022d858 = uVar9;
  __android_log_print(4,"NexusOnlineProbe","vm=%d class=%d method=%d count=%d",
                      DAT_0022d860 != (long *)0x0,DAT_0022d868 != 0,DAT_0022d870 != 0,iVar5);
LAB_0014cf48:
  *puVar8 = uVar1;
  if (*(long *)(lVar2 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return iVar5;
}

/* ===== FUN_0014cfcc @ 0014cfcc ===== */

void FUN_0014cfcc(clockid_t param_1)

{
  long lVar1;
  int iVar2;
  long lVar3;
  timespec local_38;
  long local_28;
  
  lVar1 = tpidr_el0;
  local_28 = *(long *)(lVar1 + 0x28);
  iVar2 = clock_gettime(param_1,&local_38);
  if (iVar2 == 0) {
    lVar3 = local_38.tv_sec * 1000 + (ulong)local_38.tv_nsec / 1000000;
  }
  else {
    lVar3 = 0;
  }
  if (*(long *)(lVar1 + 0x28) == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(lVar3);
}

/* ===== nexus_ui_performance_mode @ 0014d044 ===== */

undefined8 nexus_ui_performance_mode(void)

{
  return DAT_0022d880;
}

/* ===== thunk_FUN_0014d168 @ 0014d164 ===== */

void thunk_FUN_0014d168(long param_1,ulong param_2,char *param_3)

{
  long lVar1;
  int iVar2;
  undefined8 uVar3;
  ulong uVar4;
  ulong uVar5;
  undefined1 *puVar6;
  long lVar7;
  ulong uVar8;
  char acStack_23c [68];
  undefined1 auStack_1f8 [32];
  undefined1 auStack_1d8 [256];
  undefined1 auStack_d8 [112];
  long lStack_68;
  
  lVar1 = tpidr_el0;
  lStack_68 = *(long *)(lVar1 + 0x28);
  uVar3 = FUN_00191314(auStack_d8);
  if (param_2 != 0) {
    uVar8 = 0;
    do {
      uVar5 = param_2 - uVar8;
      if (0xff < uVar5) {
        uVar5 = 0x100;
      }
      uVar4 = FUN_0014e7c4(uVar3,uVar8 + param_1 + DAT_0022d8e0,auStack_1d8,uVar5);
      if ((int)uVar4 == 0) goto LAB_0014d254;
      uVar3 = FUN_00191338(auStack_d8,auStack_1d8,uVar5);
      uVar8 = uVar5 + uVar8;
    } while (uVar8 < param_2);
  }
  FUN_00191678(auStack_d8,auStack_1f8);
  lVar7 = 0;
  puVar6 = auStack_1f8;
  do {
    FUN_0015540c(acStack_23c + lVar7,0xffffffffffffffff,3,&DAT_00139d9f,*puVar6);
    lVar7 = lVar7 + 2;
    puVar6 = puVar6 + 1;
  } while (lVar7 != 0x40);
  iVar2 = strcmp(acStack_23c,param_3);
  uVar4 = (ulong)(iVar2 == 0);
LAB_0014d254:
  if (*(long *)(lVar1 + 0x28) == lStack_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar4);
}

/* ===== FUN_0014d168 @ 0014d168 ===== */

void FUN_0014d168(long param_1,ulong param_2,char *param_3)

{
  long lVar1;
  int iVar2;
  undefined8 uVar3;
  ulong uVar4;
  ulong uVar5;
  undefined1 *puVar6;
  long lVar7;
  ulong uVar8;
  char acStack_23c [68];
  undefined1 auStack_1f8 [32];
  undefined1 auStack_1d8 [256];
  undefined1 auStack_d8 [112];
  long local_68;
  
  lVar1 = tpidr_el0;
  local_68 = *(long *)(lVar1 + 0x28);
  uVar3 = FUN_00191314(auStack_d8);
  if (param_2 != 0) {
    uVar8 = 0;
    do {
      uVar5 = param_2 - uVar8;
      if (0xff < uVar5) {
        uVar5 = 0x100;
      }
      uVar4 = FUN_0014e7c4(uVar3,uVar8 + param_1 + DAT_0022d8e0,auStack_1d8,uVar5);
      if ((int)uVar4 == 0) goto LAB_0014d254;
      uVar3 = FUN_00191338(auStack_d8,auStack_1d8,uVar5);
      uVar8 = uVar5 + uVar8;
    } while (uVar8 < param_2);
  }
  FUN_00191678(auStack_d8,auStack_1f8);
  lVar7 = 0;
  puVar6 = auStack_1f8;
  do {
    FUN_0015540c(acStack_23c + lVar7,0xffffffffffffffff,3,&DAT_00139d9f,*puVar6);
    lVar7 = lVar7 + 2;
    puVar6 = puVar6 + 1;
  } while (lVar7 != 0x40);
  iVar2 = strcmp(acStack_23c,param_3);
  uVar4 = (ulong)(iVar2 == 0);
LAB_0014d254:
  if (*(long *)(lVar1 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar4);
}

/* ===== FUN_0014d288 @ 0014d288 ===== */

undefined8 FUN_0014d288(long *param_1)

{
  undefined8 uVar1;
  ulong uVar2;
  
  if (param_1 != (long *)0x0) {
    *param_1 = 0;
  }
  uVar1 = 0;
  if ((((DAT_0022d8b0 == '\x01' && DAT_0022d8b8 != 0) && DAT_0022d8c0 != 0) &&
      (param_1 != (long *)0x0)) && ((int)DAT_0022d8c8 != 0)) {
    *param_1 = 0;
    uVar2 = FUN_00193f80(1,&DAT_001a7c08);
    if ((uVar2 & 1) == 0) {
      uVar1 = 0;
      if (((DAT_001a7c14 != 0) && (DAT_001a7c10 != 0)) &&
         ((DAT_001a7c28 != '\0' && ((DAT_001a7c2a != '\0' && (DAT_001a7c20 != 0)))))) {
        uVar1 = 1;
        *param_1 = DAT_001a7c20;
      }
      DAT_001a7c08 = 0;
    }
    else {
      uVar1 = 0;
    }
  }
  return uVar1;
}

/* ===== FUN_0014d350 @ 0014d350 ===== */

bool FUN_0014d350(long param_1)

{
  long lVar1;
  bool bVar2;
  long *plVar3;
  bool bVar4;
  char cVar5;
  int iVar6;
  ulong uVar7;
  long *local_60;
  long local_58;
  
  lVar1 = tpidr_el0;
  local_58 = *(long *)(lVar1 + 0x28);
  if (param_1 != 0) {
    bVar4 = false;
    if (((DAT_0022d8b0 != '\x01') || (DAT_0022d8b8 == 0)) || (DAT_0022d8c0 == 0)) goto LAB_0014d4cc;
    if ((int)DAT_0022d8c8 != 0) {
      uVar7 = FUN_00193f80(1,&DAT_001a7c08);
      bVar4 = false;
      if ((uVar7 & 1) != 0) goto LAB_0014d4cc;
      DAT_001a7c08 = 0;
      if ((((DAT_001a7c14 == 0) || (DAT_001a7c10 == 0)) ||
          ((DAT_001a7c28 == '\0' || ((DAT_001a7c2a == '\0' || (DAT_001a7c20 == 0)))))) ||
         (DAT_001a7c20 != param_1)) goto LAB_0014d4cc;
      local_60 = (long *)0x0;
      if (((DAT_0022d8b0 == '\x01') && (DAT_0022d8d0 != (long *)0x0)) && (DAT_0022f030 != 0)) {
        iVar6 = (**(code **)(*DAT_0022d8d0 + 0x30))(DAT_0022d8d0,&local_60,0x10006);
        if (iVar6 == 0) {
          bVar2 = true;
        }
        else {
          if ((iVar6 != -2) ||
             (iVar6 = (**(code **)(*DAT_0022d8d0 + 0x20))(DAT_0022d8d0,&local_60,0), iVar6 != 0))
          goto LAB_0014d4c8;
          bVar2 = false;
        }
        plVar3 = local_60;
        if (local_60 != (long *)0x0) {
          cVar5 = (**(code **)(*local_60 + 0x3a8))(local_60,DAT_0022d8b8,DAT_0022d8c0,param_1);
          if (cVar5 == '\x01') {
            cVar5 = (**(code **)(*plVar3 + 0x720))(plVar3);
            bVar4 = cVar5 == '\0';
          }
          else {
            bVar4 = false;
          }
          cVar5 = (**(code **)(*plVar3 + 0x720))(plVar3);
          if (cVar5 != '\0') {
            (**(code **)(*plVar3 + 0x88))(plVar3);
          }
          if (!bVar2) {
            (**(code **)(*DAT_0022d8d0 + 0x28))();
          }
          goto LAB_0014d4cc;
        }
      }
    }
  }
LAB_0014d4c8:
  bVar4 = false;
LAB_0014d4cc:
  if (*(long *)(lVar1 + 0x28) == local_58) {
    return bVar4;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0014d544 @ 0014d544 ===== */

undefined8 FUN_0014d544(char *param_1,int *param_2)

{
  byte bVar1;
  byte bVar2;
  int iVar3;
  size_t sVar4;
  undefined8 uVar5;
  byte *pbVar6;
  int iVar7;
  
  if (param_1 == (char *)0x0) {
    return 0;
  }
  if (param_2 == (int *)0x0) {
    return 0;
  }
  sVar4 = strlen(param_1);
  if (0x9f < sVar4) {
    return 0;
  }
  iVar3 = FUN_0014d7d8(param_1,"PLAY AGAIN");
  if ((((iVar3 != 0) || (iVar3 = FUN_0014d7d8(param_1,&DAT_001347da), iVar3 != 0)) ||
      (iVar3 = FUN_0014d7d8(param_1,&DAT_0013a2b0), iVar3 != 0)) ||
     ((iVar3 = FUN_0014d7d8(param_1,&DAT_00138299), iVar3 != 0 ||
      (iVar3 = FUN_0014d7d8(param_1,&DAT_0013937f), iVar3 != 0)))) {
    return 1;
  }
  iVar3 = FUN_0014d7d8(param_1,&DAT_0013b993);
  if ((iVar3 != 0) || (iVar3 = FUN_0014d7d8(param_1,&DAT_001376bf), iVar3 != 0)) {
    return 2;
  }
  iVar3 = FUN_0014d7d8(param_1,"VICTORY");
  if (((iVar3 != 0) || (iVar3 = FUN_0014d7d8(param_1,"YOU WIN"), iVar3 != 0)) ||
     (iVar3 = FUN_0014d7d8(param_1,&DAT_00137b79), iVar3 != 0)) {
    return 3;
  }
  iVar3 = FUN_0014d7d8(param_1,"DEFEAT");
  if (((iVar3 != 0) || (iVar3 = FUN_0014d7d8(param_1,"YOU LOSE"), iVar3 != 0)) ||
     (iVar3 = FUN_0014d7d8(param_1,&DAT_0013a8fa), iVar3 != 0)) {
    return 4;
  }
  iVar3 = FUN_0014d7d8(param_1,&DAT_0013bf80);
  if ((iVar3 != 0) || (iVar3 = FUN_0014d7d8(param_1,&DAT_00138dba), iVar3 != 0)) {
    return 5;
  }
  iVar3 = FUN_0014d990(param_1);
  if (iVar3 == 0) {
    pbVar6 = (byte *)(param_1 + 2);
    while( true ) {
      bVar1 = pbVar6[-2];
      if (0x2d < bVar1) {
        return 0;
      }
      if ((1L << ((ulong)bVar1 & 0x3f) & 0x100002600U) == 0) break;
      pbVar6 = pbVar6 + 1;
    }
    if ((1L << ((ulong)bVar1 & 0x3f) & 0x280000000000U) == 0) {
      return 0;
    }
    for (; bVar2 = pbVar6[-1], bVar2 == 0x20 || bVar2 == 9; pbVar6 = pbVar6 + 1) {
    }
    if (0xfffffff5 < bVar2 - 0x3a) {
      iVar7 = 0;
      while( true ) {
        bVar2 = pbVar6[-1];
        if (9 < bVar2 - 0x30) break;
        iVar3 = (uint)bVar2 + iVar7 * 10;
        pbVar6 = pbVar6 + 1;
        iVar7 = iVar3 + -0x30;
        if (0x4e < iVar3) {
          return 0;
        }
      }
      if (bVar2 < 0x21) {
        do {
          if ((1L << ((ulong)bVar2 & 0x3f) & 0x100002600U) == 0) {
            if ((ulong)bVar2 != 0) {
              return 0;
            }
            uVar5 = 6;
            iVar3 = -iVar7;
            if (bVar1 != 0x2d) {
              iVar3 = iVar7;
            }
            goto LAB_0014d6ec;
          }
          bVar2 = *pbVar6;
          pbVar6 = pbVar6 + 1;
        } while (bVar2 < 0x21);
      }
    }
    return 0;
  }
  uVar5 = 7;
LAB_0014d6ec:
  *param_2 = iVar3;
  return uVar5;
}

/* ===== FUN_0014d7d8 @ 0014d7d8 ===== */

byte * FUN_0014d7d8(byte *param_1,byte *param_2)

{
  byte bVar1;
  byte bVar2;
  size_t sVar3;
  size_t sVar4;
  uint uVar5;
  byte *pbVar6;
  byte *pbVar7;
  uint uVar8;
  byte *pbVar9;
  
  if (param_1 != (byte *)0x0) {
    for (; *param_1 < 0x21 && (1L << ((ulong)*param_1 & 0x3f) & 0x100002600U) != 0;
        param_1 = param_1 + 1) {
    }
    sVar3 = strlen((char *)param_1);
    while ((sVar3 != 0 &&
           (param_1[sVar3 - 1] < 0x22 &&
            (1L << ((ulong)param_1[sVar3 - 1] & 0x3f) & 0x300002600U) != 0))) {
      sVar3 = sVar3 - 1;
    }
    sVar4 = strlen((char *)param_2);
    if (sVar3 == sVar4) {
      pbVar9 = param_1;
      do {
        if (param_1 + sVar3 <= pbVar9) {
          return (byte *)(ulong)(*param_2 == 0);
        }
        pbVar6 = pbVar9 + 1;
        bVar1 = *pbVar9;
        uVar5 = (uint)bVar1;
        if (bVar1 - 0x61 < 0x1a) {
          uVar5 = bVar1 - 0x20;
        }
        else if (((bVar1 & 0xfe) == 0xd0) && (bVar2 = *pbVar6, (bVar2 & 0xc0) == 0x80)) {
          pbVar6 = pbVar9 + 2;
          uVar5 = bVar2 & 0x3f | (bVar1 & 0x1f) << 6;
          if (uVar5 - 0x430 < 0x20) {
            uVar5 = uVar5 - 0x20;
          }
          else if (uVar5 == 0x451) {
            uVar5 = 0x401;
          }
        }
        pbVar7 = param_2 + 1;
        uVar8 = (uint)*param_2;
        if (uVar8 - 0x61 < 0x1a) {
          uVar8 = uVar8 - 0x20;
        }
        else if (((uVar8 & 0xfe) == 0xd0) && (bVar1 = *pbVar7, (bVar1 & 0xc0) == 0x80)) {
          pbVar7 = param_2 + 2;
          uVar8 = bVar1 & 0x3f | (uVar8 & 0x1f) << 6;
          if (uVar8 - 0x430 < 0x20) {
            uVar8 = uVar8 - 0x20;
          }
          else if (uVar8 == 0x451) {
            uVar8 = 0x401;
          }
        }
        pbVar9 = pbVar6;
        param_2 = pbVar7;
      } while (uVar5 == uVar8);
    }
    param_1 = (byte *)0x0;
  }
  return param_1;
}

/* ===== FUN_0014d990 @ 0014d990 ===== */

void FUN_0014d990(ulong *param_1)

{
  int iVar1;
  long lVar2;
  int iVar3;
  size_t sVar4;
  byte *pbVar5;
  byte bVar6;
  long lVar7;
  ulong local_38;
  undefined2 local_30;
  undefined1 local_2e;
  long local_28;
  
  lVar2 = tpidr_el0;
  local_28 = *(long *)(lVar2 + 0x28);
  for (; (byte)*param_1 < 0x21 && (1L << ((ulong)(byte)*param_1 & 0x3f) & 0x100002600U) != 0;
      param_1 = (ulong *)((long)param_1 + 1)) {
  }
  sVar4 = strlen((char *)param_1);
  if (sVar4 < 4) {
LAB_0014da40:
    sVar4 = strlen((char *)param_1);
    if (9 < sVar4) {
      local_38 = *param_1;
      local_30 = (undefined2)param_1[1];
      local_2e = 0;
      iVar3 = FUN_0014d7d8(&local_38,&DAT_00131dae);
      if (((iVar3 != 0) && (*(byte *)((long)param_1 + 10) < 0x3b)) &&
         ((1L << ((ulong)*(byte *)((long)param_1 + 10) & 0x3f) & 0x400000100000200U) != 0)) {
        pbVar5 = (byte *)((long)param_1 + 10);
        goto LAB_0014dac4;
      }
    }
    if ((byte)*param_1 == 0x23) {
      pbVar5 = (byte *)((long)param_1 + 1);
      goto LAB_0014dac4;
    }
  }
  else {
    local_38 = CONCAT44((int)(local_38 >> 0x20),(int)*param_1) & 0xffffff00ffffffff;
    iVar3 = FUN_0014d7d8(&local_38,&DAT_0013bfa4);
    if (((iVar3 == 0) || (0x3a < *(byte *)((long)param_1 + 4))) ||
       ((1L << ((ulong)*(byte *)((long)param_1 + 4) & 0x3f) & 0x400000100000200U) == 0))
    goto LAB_0014da40;
    pbVar5 = (byte *)((long)param_1 + 4);
LAB_0014dac4:
    for (; (bVar6 = *pbVar5, bVar6 == 9 || (bVar6 == 0x20)); pbVar5 = pbVar5 + 1) {
    }
    if (bVar6 == 0x3a) {
      pbVar5 = pbVar5 + 1;
    }
    for (; bVar6 = *pbVar5, bVar6 == 0x20 || bVar6 == 9; pbVar5 = pbVar5 + 1) {
    }
    if (0xfffffff6 < bVar6 - 0x3a) {
      iVar3 = 0;
      do {
        bVar6 = *pbVar5;
        if (9 < bVar6 - 0x30) {
          lVar7 = 1;
          if (bVar6 < 0x22) goto LAB_0014db6c;
          goto LAB_0014db8c;
        }
        iVar1 = (uint)bVar6 + iVar3 * 10;
        pbVar5 = pbVar5 + 1;
        iVar3 = iVar1 + -0x30;
      } while (iVar1 < 0x3b);
    }
  }
  iVar3 = 0;
LAB_0014db34:
  if (*(long *)(lVar2 + 0x28) == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar3);
  while( true ) {
    bVar6 = pbVar5[lVar7];
    lVar7 = lVar7 + 1;
    if (0x21 < bVar6) break;
LAB_0014db6c:
    if ((1L << ((ulong)bVar6 & 0x3f) & 0x300002600U) == 0) break;
  }
LAB_0014db8c:
  if (bVar6 != 0) {
    iVar3 = 0;
  }
  goto LAB_0014db34;
}

/* ===== FUN_0014db9c @ 0014db9c ===== */

void FUN_0014db9c(ulong *param_1,ulong *param_2)

{
  int iVar1;
  ulong uVar2;
  ulong uVar3;
  ulong uVar4;
  ulong uVar5;
  ulong uVar6;
  ulong uVar7;
  
  if (((param_1 == (ulong *)0x0) || (param_2 == (ulong *)0x0)) || ((int)param_1[0x32] != 0)) {
    return;
  }
  iVar1 = (int)param_2[4];
  if ((iVar1 == 0) ||
     ((*(int *)((long)param_2 + 0x24) == 0 && (iVar1 = (int)param_2[5], iVar1 == 0)))) {
LAB_0014dc10:
    param_1[0x1e] = 0;
    param_1[0x1d] = 0;
    param_1[0x34] = 0;
    param_1[0x33] = 0;
    *(undefined4 *)(param_1 + 0x2d) = 0;
    param_1[0x2c] = 0;
    param_1[0x36] = 0;
    param_1[0x35] = 0;
    param_1[0x38] = 0;
    param_1[0x37] = 0;
    param_1[0x3a] = 0;
    param_1[0x39] = 0;
    param_1[0x3c] = 0;
    param_1[0x3b] = 0;
    *(undefined4 *)((long)param_1 + 0x17c) = 0;
    *(undefined4 *)((long)param_1 + 0x174) = 0;
    param_1[0x20] = 0;
    param_1[0x1f] = 0;
    param_1[0x22] = 0;
    param_1[0x21] = 0;
    param_1[0x24] = 0;
    param_1[0x23] = 0;
    param_1[0x25] = 0;
    *(undefined8 *)((long)param_1 + 0x18c) = 0;
    *(undefined8 *)((long)param_1 + 0x184) = 0;
    param_1[0x1b] = 0;
    param_1[8] = 0;
    param_1[7] = 0;
    param_1[10] = 0;
    param_1[9] = 0;
    param_1[0xc] = 0;
    param_1[0xb] = 0;
    param_1[0xe] = 0;
    param_1[0xd] = 0;
    param_1[0x10] = 0;
    param_1[0xf] = 0;
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
    *(int *)((long)param_1 + 0x194) = iVar1;
    if (iVar1 == 0) goto LAB_0014dc88;
  }
  else {
    if ((*(int *)((long)param_1 + 0x194) == 0) ||
       ((*param_2 < *param_1 || (param_2[2] != param_1[2])))) {
      iVar1 = 1;
      goto LAB_0014dc10;
    }
    iVar1 = 1;
    if (param_2[1] != param_1[1]) goto LAB_0014dc10;
    *(undefined4 *)((long)param_1 + 0x194) = 1;
  }
  if ((param_2[3] != 0) && (param_2[1] != 0)) {
    uVar2 = *param_2;
    param_1[0x1b] = param_2[1];
    param_1[0x1c] = uVar2;
  }
LAB_0014dc88:
  uVar5 = param_2[2];
  uVar4 = param_2[5];
  uVar3 = param_2[4];
  uVar2 = param_2[6];
  uVar7 = param_2[1];
  uVar6 = *param_2;
  param_1[3] = param_2[3];
  param_1[2] = uVar5;
  param_1[5] = uVar4;
  param_1[4] = uVar3;
  param_1[6] = uVar2;
  param_1[1] = uVar7;
  *param_1 = uVar6;
  return;
}

/* ===== FUN_0014dca4 @ 0014dca4 ===== */

void FUN_0014dca4(long param_1,undefined1 (*param_2) [16],undefined8 param_3,long *param_4)

{
  long lVar1;
  long lVar2;
  long lVar3;
  int iVar4;
  int iVar5;
  undefined8 uVar6;
  ulong uVar7;
  undefined1 auVar8 [16];
  long lVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  long lVar14;
  int local_3c;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  if ((((param_1 == 0) || (param_2 == (undefined1 (*) [16])0x0)) || (*(int *)(param_1 + 0x194) == 0)
      ) || ((*(int *)(param_1 + 400) != 0 || (*(long *)(*param_2 + 8) != *(long *)(param_1 + 8)))))
  goto LAB_0014dee8;
  local_3c = 0;
  iVar4 = FUN_0014d544(param_3,&local_3c);
  if (((param_4 != (long *)0x0) && ((iVar4 == 2 && (*(int *)(param_2[2] + 4) != 0)))) &&
     (*(long *)(param_2[1] + 8) == 0)) {
    if ((*(int *)(param_1 + 0x188) != 0) && (*(int *)(param_1 + 0x18c) == 1)) {
      if ((*(long *)(param_1 + 0x100) - 1U < *(ulong *)*param_2) &&
         (0x270 < *(ulong *)*param_2 - *(long *)(param_1 + 0x100) >> 3)) {
        *(undefined4 *)(param_1 + 0x188) = 0;
        *(undefined8 *)(param_1 + 0xe8) = 0;
        *(undefined8 *)(param_1 + 0x40) = 0;
        *(undefined8 *)(param_1 + 0x38) = 0;
        *(undefined8 *)(param_1 + 0x50) = 0;
        *(undefined8 *)(param_1 + 0x48) = 0;
        *(undefined8 *)(param_1 + 0x60) = 0;
        *(undefined8 *)(param_1 + 0x58) = 0;
        *(undefined8 *)(param_1 + 0x70) = 0;
        *(undefined8 *)(param_1 + 0x68) = 0;
        *(undefined8 *)(param_1 + 0x80) = 0;
        *(undefined8 *)(param_1 + 0x78) = 0;
      }
    }
    if (((*(long *)(param_1 + 0x110) == 0) || (*(long *)(param_1 + 0x88) != *param_4)) ||
       (*(long *)(param_1 + 0xb0) != param_4[5])) {
      *(undefined8 *)(param_1 + 0x110) = *(undefined8 *)*param_2;
    }
    lVar10 = param_4[7];
    lVar9 = param_4[6];
    lVar2 = param_4[8];
    lVar12 = param_4[3];
    lVar11 = param_4[2];
    lVar14 = param_4[5];
    lVar13 = param_4[4];
    *(long *)(param_1 + 0xd0) = param_4[9];
    *(long *)(param_1 + 200) = lVar2;
    lVar2 = *param_4;
    lVar3 = param_4[1];
    *(long *)(param_1 + 0xc0) = lVar10;
    *(long *)(param_1 + 0xb8) = lVar9;
    *(long *)(param_1 + 0xb0) = lVar14;
    *(long *)(param_1 + 0xa8) = lVar13;
    *(long *)(param_1 + 0xa0) = lVar12;
    *(long *)(param_1 + 0x98) = lVar11;
    *(long *)(param_1 + 0x90) = lVar3;
    *(long *)(param_1 + 0x88) = lVar2;
    auVar8 = *param_2;
    *(undefined4 *)(param_1 + 200) = 2;
    auVar8 = NEON_ext(auVar8,auVar8,8,1);
    *(long *)(param_1 + 0xc0) = auVar8._8_8_;
    *(long *)(param_1 + 0xb8) = auVar8._0_8_;
    goto LAB_0014dee8;
  }
  if ((*(long *)(param_1 + 0xd8) == 0) || (*(long *)(param_1 + 0xd8) != *(long *)(*param_2 + 8)))
  goto LAB_0014dee8;
  if ((param_4 != (long *)0x0) && (iVar4 == 1)) {
    uVar6 = *(undefined8 *)*param_2;
    if (*(long *)(param_1 + 0xe8) == 0) {
      *(undefined8 *)(param_1 + 0xe8) = uVar6;
    }
    *(undefined8 *)(param_1 + 0x118) = uVar6;
    if (((*(long *)(param_1 + 0x108) == 0) || (*(long *)(param_1 + 0x38) != *param_4)) ||
       (*(long *)(param_1 + 0x60) != param_4[5])) {
      *(undefined8 *)(param_1 + 0x108) = uVar6;
    }
    lVar10 = param_4[7];
    lVar9 = param_4[6];
    lVar2 = param_4[8];
    lVar12 = param_4[3];
    lVar11 = param_4[2];
    lVar14 = param_4[5];
    lVar13 = param_4[4];
    *(long *)(param_1 + 0x80) = param_4[9];
    *(long *)(param_1 + 0x78) = lVar2;
    lVar2 = *param_4;
    lVar3 = param_4[1];
    *(long *)(param_1 + 0x70) = lVar10;
    *(long *)(param_1 + 0x68) = lVar9;
    *(long *)(param_1 + 0x60) = lVar14;
    *(long *)(param_1 + 0x58) = lVar13;
    *(long *)(param_1 + 0x50) = lVar12;
    *(long *)(param_1 + 0x48) = lVar11;
    *(long *)(param_1 + 0x40) = lVar3;
    *(long *)(param_1 + 0x38) = lVar2;
    auVar8 = *param_2;
    *(undefined4 *)(param_1 + 0x78) = 1;
    auVar8 = NEON_ext(auVar8,auVar8,8,1);
    *(long *)(param_1 + 0x70) = auVar8._8_8_;
    *(long *)(param_1 + 0x68) = auVar8._0_8_;
    goto LAB_0014dee8;
  }
  if (2 < iVar4 - 3U) {
    if (iVar4 == 6) {
      if (*(int *)(param_1 + 0x17c) == 0) {
        if ((*(int *)(param_1 + 0x174) == 0) || (*(int *)(param_1 + 0x164) != local_3c)) {
          uVar6 = *(undefined8 *)*param_2;
          *(int *)(param_1 + 0x164) = local_3c;
          *(undefined8 *)(param_1 + 0xf8) = uVar6;
        }
        else {
          uVar6 = *(undefined8 *)*param_2;
        }
        *(undefined8 *)(param_1 + 0x128) = uVar6;
        *(undefined4 *)(param_1 + 0x174) = 1;
      }
      goto LAB_0014dee8;
    }
    if (iVar4 != 7) goto LAB_0014dee8;
    uVar7 = *(ulong *)*param_2;
    if (*(long *)(param_1 + 0xe8) == 0) {
      if ((*(long *)(param_1 + 0xe0) - 1U < uVar7) && (uVar7 - *(long *)(param_1 + 0xe0) < 0x7531))
      {
        *(ulong *)(param_1 + 0xe8) = uVar7;
        goto LAB_0014df4c;
      }
    }
    else {
LAB_0014df4c:
      *(ulong *)(param_1 + 0x118) = uVar7;
    }
    *(int *)(param_1 + 0x168) = local_3c;
    goto LAB_0014dee8;
  }
  iVar5 = 2;
  if (iVar4 == 4) {
    iVar5 = -1;
  }
  if (iVar4 == 3) {
    iVar5 = 1;
  }
  uVar7 = *(ulong *)*param_2;
  if (*(long *)(param_1 + 0xe8) == 0) {
    if ((*(long *)(param_1 + 0xe0) - 1U < uVar7) && (uVar7 - *(long *)(param_1 + 0xe0) < 0x7531)) {
      *(ulong *)(param_1 + 0xe8) = uVar7;
      goto LAB_0014de0c;
    }
  }
  else {
LAB_0014de0c:
    *(ulong *)(param_1 + 0x118) = uVar7;
  }
  *(ulong *)(param_1 + 0x120) = uVar7;
  if (*(int *)(param_1 + 0x160) != iVar5) {
    *(int *)(param_1 + 0x160) = iVar5;
    *(ulong *)(param_1 + 0xf0) = uVar7;
  }
LAB_0014dee8:
  if (*(long *)(lVar1 + 0x28) != local_38) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

/* ===== FUN_0014dfa8 @ 0014dfa8 ===== */

void FUN_0014dfa8(long param_1,undefined8 *param_2,int param_3)

{
  undefined8 uVar1;
  
  if (((((param_1 != 0) && (param_2 != (undefined8 *)0x0)) && (*(int *)(param_1 + 0x194) != 0)) &&
      ((*(int *)(param_1 + 400) == 0 && (param_2[1] == *(long *)(param_1 + 8))))) &&
     ((*(long *)(param_1 + 0xd8) != 0 && (*(long *)(param_1 + 0xd8) == param_2[1])))) {
    uVar1 = *param_2;
    if (*(long *)(param_1 + 0xe8) == 0) {
      *(undefined8 *)(param_1 + 0xe8) = uVar1;
    }
    *(undefined8 *)(param_1 + 0x118) = uVar1;
    if (((*(int *)(param_1 + 0x17c) == 0) || (*(int *)(param_1 + 0x174) == 0)) ||
       (*(int *)(param_1 + 0x164) != param_3)) {
      *(undefined8 *)(param_1 + 0xf8) = uVar1;
    }
    *(int *)(param_1 + 0x164) = param_3;
    *(undefined8 *)(param_1 + 0x128) = uVar1;
    *(undefined4 *)(param_1 + 0x17c) = 1;
    *(undefined4 *)(param_1 + 0x174) = 1;
    return;
  }
  return;
}

/* ===== FUN_0014e030 @ 0014e030 ===== */

void FUN_0014e030(ulong *param_1,ulong *param_2,ulong *param_3)

{
  uint uVar1;
  long lVar2;
  undefined8 uVar3;
  long lVar4;
  ulong uVar5;
  ulong *puVar6;
  ulong uVar7;
  ulong uVar8;
  ulong *puVar9;
  ulong uVar10;
  ulong uVar11;
  ulong uVar12;
  ulong uVar13;
  ulong uVar14;
  ulong uVar15;
  ulong uVar16;
  
  lVar2 = tpidr_el0;
  lVar4 = *(long *)(lVar2 + 0x28);
  uVar3 = 0;
  if (((param_1 == (ulong *)0x0) || (param_2 == (ulong *)0x0)) || (param_3 == (ulong *)0x0))
  goto LAB_0014e080;
  if (((((*(int *)((long)param_1 + 0x194) != 0) && ((int)param_1[0x32] == 0)) &&
       (((int)param_2[4] != 0 && (*(int *)((long)param_2 + 0x2c) == 0)))) &&
      ((*(int *)((long)param_2 + 0x24) != 0 || ((int)param_2[5] != 0)))) &&
     ((param_2[2] == param_1[2] &&
      (((param_2[1] == param_1[1] && (*param_1 <= *param_2)) &&
       (FUN_0014e330(param_1,param_2), (int)param_1[0x31] == 0)))))) {
    uVar5 = param_1[0x20];
    if (uVar5 != 0) {
      uVar1 = *(uint *)((long)param_2 + 0x34);
      uVar3 = 0;
      if (4999 < uVar1) {
        uVar1 = 5000;
      }
      if (uVar1 < 0x1f5) {
        uVar1 = 500;
      }
      if ((*param_2 <= uVar5 - 1) || (*param_2 - uVar5 < (ulong)uVar1)) goto LAB_0014e080;
    }
    uVar1 = (uint)param_2[6];
    if (9999 < uVar1) {
      uVar1 = 10000;
    }
    uVar5 = param_1[7];
    if (uVar1 < 0x3e9) {
      uVar1 = 1000;
    }
    if ((uVar5 == 0) || (param_1[0x1b] != param_2[1])) {
LAB_0014e1c4:
      if (((*(int *)((long)param_2 + 0x24) == 0) || (param_2[3] != 0)) ||
         (uVar5 = param_1[0x11], uVar5 == 0)) goto LAB_0014e07c;
      uVar8 = *param_2;
      uVar3 = 0;
      puVar6 = param_1 + 0x18;
      if ((uVar8 <= *puVar6 - 1) || (15000 < uVar8 - *puVar6)) goto LAB_0014e080;
      uVar3 = 0;
      if ((uVar8 <= param_1[0x22] - 1) || (uVar8 - param_1[0x22] < 1000)) goto LAB_0014e080;
      uVar13 = param_1[0x14];
      uVar10 = param_1[0x13];
      puVar9 = param_1 + 0x17;
      uVar16 = param_1[0x16];
      uVar15 = param_1[0x15];
      uVar8 = param_1[0x12];
    }
    else {
      uVar8 = *param_2;
      puVar6 = param_1 + 0xe;
      if ((uVar8 <= *puVar6 - 1) ||
         (((15000 < uVar8 - *puVar6 || (uVar8 <= param_1[0x21] - 1)) ||
          (uVar10 = uVar8 - param_1[0x21], uVar10 < uVar1)))) goto LAB_0014e1c4;
      if (uVar10 >> 4 < 0x271) {
        if ((int)param_1[0x2c] == 0) {
          if (*(int *)((long)param_1 + 0x174) == 0) goto LAB_0014e07c;
        }
        else if (*(int *)((long)param_1 + 0x174) == 0) goto LAB_0014e294;
        uVar3 = 0;
        if ((uVar8 <= param_1[0x25] - 1) || (15000 < uVar8 - param_1[0x25])) goto LAB_0014e080;
        if (uVar8 - param_1[0x1f] < 0x2ee) goto LAB_0014e07c;
      }
LAB_0014e294:
      uVar13 = param_1[10];
      uVar10 = param_1[9];
      puVar9 = param_1 + 0xd;
      uVar16 = param_1[0xc];
      uVar15 = param_1[0xb];
      uVar8 = param_1[8];
    }
    uVar14 = puVar6[1];
    uVar12 = *puVar6;
    uVar11 = puVar6[2];
    uVar7 = *puVar9;
    if ((uVar8 == param_2[2]) && (uVar7 == param_2[1])) {
      uVar3 = 1;
      param_1[0x33] = uVar5;
      param_1[0x34] = uVar8;
      param_1[0x39] = uVar7;
      *(undefined4 *)(param_1 + 0x32) = 1;
      param_1[0x36] = uVar13;
      param_1[0x35] = uVar10;
      param_1[0x38] = uVar16;
      param_1[0x37] = uVar15;
      param_1[0x3b] = uVar14;
      param_1[0x3a] = uVar12;
      param_1[0x3c] = uVar11;
      param_1[0x20] = *param_2;
      param_3[3] = uVar13;
      param_3[2] = uVar10;
      param_3[5] = uVar16;
      param_3[4] = uVar15;
      *param_3 = uVar5;
      param_3[1] = uVar8;
      param_3[6] = uVar7;
      param_3[9] = uVar11;
      param_3[8] = uVar14;
      param_3[7] = uVar12;
      goto LAB_0014e080;
    }
  }
LAB_0014e07c:
  uVar3 = 0;
LAB_0014e080:
  if (*(long *)(lVar2 + 0x28) != lVar4) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar3);
  }
  return;
}

/* ===== FUN_0014e330 @ 0014e330 ===== */

void FUN_0014e330(long param_1,ulong *param_2)

{
  int iVar1;
  ulong uVar2;
  ulong uVar3;
  long lVar4;
  int iVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  
  uVar2 = param_2[1];
  if (uVar2 == 0) {
    return;
  }
  if (uVar2 < *(ulong *)(param_1 + 0x150)) {
    return;
  }
  uVar3 = *param_2;
  if (uVar3 <= *(long *)(param_1 + 0x118) - 1U) {
    return;
  }
  if (15000 < uVar3 - *(long *)(param_1 + 0x118)) {
    return;
  }
  if (*(long *)(param_1 + 0xe8) == 0) {
    return;
  }
  if (uVar3 - *(long *)(param_1 + 0xe8) < 0x2ee) {
    return;
  }
  if (*(ulong *)(param_1 + 0x150) != uVar2) {
    *(ulong *)(param_1 + 0x150) = uVar2;
    *(undefined4 *)(param_1 + 0x16c) = 0;
    *(undefined4 *)(param_1 + 0x170) = 0;
    *(undefined4 *)(param_1 + 0x180) = 0;
    *(undefined4 *)(param_1 + 0x178) = 0;
    *(long *)(param_1 + 0x130) = *(long *)(param_1 + 0x130) + 1;
  }
  *(undefined4 *)(param_1 + 0x184) = 1;
  uVar7 = DAT_0010f7f8;
  uVar2 = 0;
  if ((((*(int *)(param_1 + 0x174) != 0) && (*(long *)(param_1 + 0x128) - 1U < uVar3)) &&
      (uVar3 - *(long *)(param_1 + 0x128) < 0x3a99)) &&
     (uVar2 = *(ulong *)(param_1 + 0xf8), uVar2 != 0)) {
    uVar2 = (ulong)(0x2ed < uVar3 - uVar2);
  }
  iVar5 = *(int *)(param_1 + 0x160);
  if (iVar5 == 0) {
    if (*(int *)(param_1 + 0x180) == 0) {
      iVar5 = 0;
    }
    else {
      iVar5 = *(int *)(param_1 + 0x16c);
    }
  }
  else if (((*(long *)(param_1 + 0x120) - 1U < uVar3) &&
           (uVar3 - *(long *)(param_1 + 0x120) < 0x3a99)) &&
          ((*(long *)(param_1 + 0xf0) != 0 && (0x2ed < uVar3 - *(long *)(param_1 + 0xf0))))) {
    iVar1 = *(int *)(param_1 + 0x16c);
    if (iVar5 != iVar1) {
      lVar4 = *(long *)(param_1 + 0x148) - (ulong)(iVar1 == 2);
      *(int *)(param_1 + 0x16c) = iVar5;
      uVar6 = NEON_cmeq(CONCAT44(iVar1,iVar1),uVar7,4);
      uVar7 = NEON_cmeq(CONCAT44(iVar5,iVar5),uVar7,4);
      if (iVar5 == 2) {
        lVar4 = lVar4 + 1;
      }
      *(long *)(param_1 + 0x148) = lVar4;
      *(ulong *)(param_1 + 0x140) =
           *(long *)(param_1 + 0x140) + (long)(int)((ulong)uVar6 >> 0x20) +
           (ulong)((byte)((ulong)uVar7 >> 0x20) & 1);
      *(long *)(param_1 + 0x138) =
           *(long *)(param_1 + 0x138) + (long)(int)uVar6 + (ulong)((byte)uVar7 & 1);
    }
    *(undefined4 *)(param_1 + 0x180) = 1;
  }
  if (*(int *)(param_1 + 0x17c) == 0) {
    if (iVar5 == -1) {
      if (0 < *(int *)(param_1 + 0x164)) goto LAB_0014e524;
    }
    else if ((iVar5 == 1) && (*(int *)(param_1 + 0x164) < 0)) {
LAB_0014e524:
      if (*(int *)(param_1 + 0x178) == 0) {
        return;
      }
      iVar5 = *(int *)(param_1 + 0x170);
      *(undefined4 *)(param_1 + 0x178) = 0;
      *(undefined4 *)(param_1 + 0x170) = 0;
      *(long *)(param_1 + 0x158) = *(long *)(param_1 + 0x158) - (long)iVar5;
      return;
    }
  }
  if ((int)uVar2 != 0) {
    iVar5 = *(int *)(param_1 + 0x164);
    if (*(int *)(param_1 + 0x178) == 0) {
      lVar4 = 0;
    }
    else {
      lVar4 = (long)*(int *)(param_1 + 0x170);
      if (iVar5 == *(int *)(param_1 + 0x170)) {
        return;
      }
    }
    *(int *)(param_1 + 0x170) = iVar5;
    *(long *)(param_1 + 0x158) = (iVar5 - lVar4) + *(long *)(param_1 + 0x158);
    *(undefined4 *)(param_1 + 0x178) = 1;
  }
  return;
}

/* ===== FUN_0014e548 @ 0014e548 ===== */

void FUN_0014e548(long param_1,int param_2)

{
  if ((param_1 != 0) && (*(int *)(param_1 + 400) != 0)) {
    *(undefined4 *)(param_1 + 400) = 0;
    if (param_2 != 0) {
      *(undefined4 *)(param_1 + 0x188) = 1;
      *(undefined4 *)(param_1 + 0x18c) = *(undefined4 *)(param_1 + 0x1d8);
    }
    *(undefined8 *)(param_1 + 0x1b0) = 0;
    *(undefined8 *)(param_1 + 0x1a8) = 0;
    *(undefined8 *)(param_1 + 0x1c0) = 0;
    *(undefined8 *)(param_1 + 0x1b8) = 0;
    *(undefined8 *)(param_1 + 0x1d0) = 0;
    *(undefined8 *)(param_1 + 0x1c8) = 0;
    *(undefined8 *)(param_1 + 0x1e0) = 0;
    *(undefined8 *)(param_1 + 0x1d8) = 0;
    *(undefined8 *)(param_1 + 0x1a0) = 0;
    *(undefined8 *)(param_1 + 0x198) = 0;
  }
  return;
}

/* ===== FUN_0014e584 @ 0014e584 ===== */

void FUN_0014e584(long param_1,long param_2)

{
  if ((param_1 != 0) &&
     ((*(long *)(param_1 + 0x38) == param_2 || (*(long *)(param_1 + 0x88) == param_2)))) {
    *(undefined4 *)(param_1 + 0x188) = 1;
  }
  return;
}

/* ===== FUN_0014e5ac @ 0014e5ac ===== */

/* WARNING: Type propagation algorithm not settling */

void FUN_0014e5ac(long param_1)

{
  long lVar1;
  long lVar2;
  int iVar3;
  int iVar4;
  pthread_t __target_thread;
  ulong uVar5;
  long local_70 [4];
  undefined8 local_50;
  code *local_48;
  code *local_40;
  long local_38;
  
  iVar4 = DAT_0022f038;
  lVar2 = tpidr_el0;
  local_38 = *(long *)(lVar2 + 0x28);
  if (DAT_0022d8d8 == '\x01') {
    local_70[3] = 0;
    local_50 = 0;
    if ((0 < DAT_0022f038) && (iVar3 = gettid(), iVar3 == iVar4)) {
      __target_thread = pthread_self();
      iVar4 = pthread_getname_np(__target_thread,(char *)(local_70 + 3),0x10);
      if ((iVar4 == 0) && (local_70[3] == 0x706f6f6c6e69614d && (char)local_50 == '\0')) {
        local_70[1] = 0;
        local_70[2] = 0;
        local_48 = FUN_0014e7c4;
        local_40 = FUN_0014ebe8;
        local_70[3] = DAT_0022d8e0;
        local_50 = 0;
        local_70[0] = 1;
        uVar5 = FUN_00154b50(local_70 + 3,param_1);
        if ((int)uVar5 != 0) {
          lVar1 = param_1 + 0x218;
          uVar5 = (*local_48)(local_50,lVar1,local_70 + 2,8);
          if (((int)uVar5 != 0) &&
             ((local_70[2] == 0 ||
              (uVar5 = (*local_40)(local_50,lVar1,local_70 + 1,8), (int)uVar5 != 0)))) {
            iVar4 = (*local_48)(local_50,lVar1,local_70,8);
            uVar5 = 0;
            if ((iVar4 != 0) && (local_70[0] == 0)) {
              iVar4 = FUN_00154b50(local_70 + 3,param_1);
              uVar5 = (ulong)(iVar4 != 0);
            }
          }
        }
        goto LAB_0014e63c;
      }
    }
  }
  uVar5 = 0;
LAB_0014e63c:
  if (*(long *)(lVar2 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar5);
}

/* ===== FUN_0014e720 @ 0014e720 ===== */

void FUN_0014e720(void)

{
  long lVar1;
  bool bVar2;
  int iVar3;
  int iVar4;
  pthread_t __target_thread;
  long local_38;
  undefined8 local_30;
  long local_28;
  
  iVar4 = DAT_0022f038;
  lVar1 = tpidr_el0;
  local_28 = *(long *)(lVar1 + 0x28);
  local_38 = 0;
  local_30 = 0;
  if ((0 < DAT_0022f038) && (iVar3 = gettid(), iVar3 == iVar4)) {
    __target_thread = pthread_self();
    iVar4 = pthread_getname_np(__target_thread,(char *)&local_38,0x10);
    if (iVar4 == 0) {
      bVar2 = local_38 == 0x706f6f6c6e69614d && (char)local_30 == '\0';
      goto LAB_0014e774;
    }
  }
  bVar2 = false;
LAB_0014e774:
  if (*(long *)(lVar1 + 0x28) == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(bVar2);
}

/* ===== FUN_0014e7c4 @ 0014e7c4 ===== */

void FUN_0014e7c4(undefined8 param_1,ulong param_2,undefined8 param_3,ulong param_4)

{
  undefined *puVar1;
  int iVar2;
  long lVar3;
  bool bVar4;
  uint uVar5;
  uint uVar6;
  int *piVar7;
  undefined8 *puVar8;
  ulong uVar9;
  int iVar10;
  ulong *puVar11;
  ulong local_88;
  ulong uStack_80;
  undefined8 local_78;
  ulong uStack_70;
  long local_68;
  
  uVar6 = DAT_001a7c38;
  lVar3 = tpidr_el0;
  bVar4 = false;
  local_68 = *(long *)(lVar3 + 0x28);
  if ((((param_2 == 0) || ((int)DAT_001a7c38 < 0)) || (bVar4 = false, CARRY8(param_2,param_4))) ||
     (0x1000 < param_4)) goto LAB_0014ebb4;
  piVar7 = (int *)__errno(0);
  iVar2 = *piVar7;
  puVar8 = (undefined8 *)__emutls_get_address(&DAT_001a7c68);
  puVar1 = &DAT_00133b55;
  if (uVar6 != 0x7fff0001) {
    puVar1 = &DAT_00134ec4;
  }
  puVar8[5] = 0;
  puVar8[6] = 0;
  *puVar8 = puVar1;
  puVar8[1] = &DAT_00133b50;
  puVar8[2] = &DAT_00137150;
  puVar8[3] = param_4;
  puVar11 = puVar8 + 4;
  *puVar11 = 0;
  local_88 = param_2;
  uStack_80 = param_4;
  local_78 = param_3;
  uStack_70 = param_4;
  if (uVar6 != 0x7fff0001) {
    uVar9 = syscall(0x43,(ulong)uVar6,param_3,param_4,param_2);
    if ((long)uVar9 < 0) goto LAB_0014e8d8;
LAB_0014e938:
    *puVar11 = uVar9;
joined_r0x0014eba0:
    if ((long)uVar9 < 0) {
      *(int *)(puVar8 + 6) = *piVar7;
    }
    else {
      *(undefined4 *)(puVar8 + 6) = 0;
      *piVar7 = iVar2;
    }
  }
  else {
    uVar5 = getpid();
    uVar9 = syscall(0x10e,(ulong)uVar5,&local_78,1,&local_88,1,0);
    if (-1 < (long)uVar9) goto LAB_0014e938;
LAB_0014e8d8:
    iVar10 = *piVar7;
    if (iVar10 == 4) {
      if (uVar6 == 0x7fff0001) {
        uVar5 = getpid();
        uVar9 = syscall(0x10e,(ulong)uVar5,&local_78,1,&local_88,1,0);
      }
      else {
        uVar9 = syscall(0x43,(ulong)uVar6,param_3,param_4,param_2);
      }
      if (-1 < (long)uVar9) goto LAB_0014e938;
      iVar10 = *piVar7;
      if (iVar10 != 4) goto LAB_0014eb74;
      if (uVar6 == 0x7fff0001) {
        uVar5 = getpid();
        uVar9 = syscall(0x10e,(ulong)uVar5,&local_78,1,&local_88,1,0);
      }
      else {
        uVar9 = syscall(0x43,(ulong)uVar6,param_3,param_4,param_2);
      }
      if (-1 < (long)uVar9) goto LAB_0014e938;
      iVar10 = *piVar7;
      if (iVar10 != 4) goto LAB_0014eb74;
      if (uVar6 == 0x7fff0001) {
        uVar5 = getpid();
        uVar9 = syscall(0x10e,(ulong)uVar5,&local_78,1,&local_88,1,0);
      }
      else {
        uVar9 = syscall(0x43,(ulong)uVar6,param_3,param_4,param_2);
      }
      if (-1 < (long)uVar9) goto LAB_0014e938;
      iVar10 = *piVar7;
      if (iVar10 != 4) goto LAB_0014eb74;
      if (uVar6 == 0x7fff0001) {
        uVar5 = getpid();
        uVar9 = syscall(0x10e,(ulong)uVar5,&local_78,1,&local_88,1,0);
      }
      else {
        uVar9 = syscall(0x43,(ulong)uVar6,param_3,param_4,param_2);
      }
      if (-1 < (long)uVar9) goto LAB_0014e938;
      iVar10 = *piVar7;
      if (iVar10 != 4) goto LAB_0014eb74;
      if (uVar6 == 0x7fff0001) {
        uVar5 = getpid();
        uVar9 = syscall(0x10e,(ulong)uVar5,&local_78,1,&local_88,1,0);
      }
      else {
        uVar9 = syscall(0x43,(ulong)uVar6,param_3,param_4,param_2);
      }
      if (-1 < (long)uVar9) goto LAB_0014e938;
      iVar10 = *piVar7;
      if (iVar10 != 4) goto LAB_0014eb74;
      if (uVar6 == 0x7fff0001) {
        uVar5 = getpid();
        uVar9 = syscall(0x10e,(ulong)uVar5,&local_78,1,&local_88,1,0);
      }
      else {
        uVar9 = syscall(0x43,(ulong)uVar6,param_3,param_4,param_2);
      }
      if (-1 < (long)uVar9) goto LAB_0014e938;
      iVar10 = *piVar7;
      if (iVar10 != 4) goto LAB_0014eb74;
      if (uVar6 == 0x7fff0001) {
        uVar6 = getpid();
        uVar9 = syscall(0x10e,(ulong)uVar6,&local_78,1,&local_88,1,0);
      }
      else {
        uVar9 = syscall(0x43,(ulong)uVar6,param_3,param_4,param_2);
      }
      *puVar11 = uVar9;
      goto joined_r0x0014eba0;
    }
LAB_0014eb74:
    *puVar11 = uVar9;
    *(int *)(puVar8 + 6) = iVar10;
  }
  bVar4 = uVar9 == param_4;
LAB_0014ebb4:
  if (*(long *)(lVar3 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(bVar4);
}

/* ===== FUN_0014ebe8 @ 0014ebe8 ===== */

bool FUN_0014ebe8(undefined8 param_1,undefined8 param_2,undefined8 param_3,long param_4)

{
  bool bVar1;
  long lVar2;
  
  bVar1 = false;
  if (param_4 == 8) {
    if (-1 < DAT_001a7c38) {
      lVar2 = FUN_00153cbc(DAT_001a7c38,param_3,8,param_2);
      bVar1 = lVar2 == 8;
    }
  }
  return bVar1;
}

/* ===== FUN_0014ec30 @ 0014ec30 ===== */

void FUN_0014ec30(void)

{
                    /* WARNING: Could not recover jumptable at 0x0014ec40. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_0022d8e0 + 0x66ad48))();
  return;
}

/* ===== FUN_0014f434 @ 0014f434 ===== */

ulong FUN_0014f434(long param_1,undefined8 *param_2)

{
  undefined8 uVar1;
  ulong uVar2;
  int *piVar3;
  
  param_2[2] = 0;
  param_2[3] = 0;
  uVar1 = DAT_0010f720;
  piVar3 = (int *)(param_2 + 1);
  piVar3[0] = 0;
  piVar3[1] = 0;
  *param_2 = uVar1;
  if (((*(int *)(param_1 + 0x20) != 0) && (*(long *)(param_1 + 0x18) != 0)) &&
     ((DAT_0022f1c0 == 0 ||
      ((DAT_0022f1b0 != *(long *)(param_1 + 0x10) || (DAT_0022f1b8 != *(long *)(param_1 + 8))))))) {
    uVar2 = FUN_00192138(param_2);
    if ((int)uVar2 == 0) {
      return uVar2;
    }
    if ((param_2[2] != 0) && (param_2[3] != 0)) {
      return (ulong)(*piVar3 != 0);
    }
  }
  return 0;
}

/* ===== FUN_0014f4dc @ 0014f4dc ===== */

void FUN_0014f4dc(long param_1)

{
  long lVar1;
  int iVar2;
  ulong uVar3;
  ulong local_30;
  long local_28;
  
  lVar1 = tpidr_el0;
  local_28 = *(long *)(lVar1 + 0x28);
  uVar3 = 0;
  if ((*(int *)(param_1 + 0x20) != 0) && (DAT_0022db18 != 0)) {
    if ((*(long *)(param_1 + 0x10) == DAT_0022db20) && (*(long *)(param_1 + 8) == DAT_0022db30)) {
      uVar3 = FUN_00154eb4(DAT_0022db18);
      if ((int)uVar3 != 0) {
        local_30 = 0;
        iVar2 = FUN_0014e7c4(uVar3,DAT_0022db18 + 0x248,&local_30,8);
        uVar3 = local_30;
        if (((local_30 & 7) != 0 || local_30 < 0x1000) || iVar2 == 0) {
          uVar3 = 0;
        }
        uVar3 = (ulong)(uVar3 == DAT_0022db28);
      }
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

/* ===== FUN_0014f830 @ 0014f830 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0014f830(uint param_1)

{
  ulong uVar1;
  long lVar2;
  bool bVar3;
  ulong uVar4;
  ulong uVar5;
  bool bVar6;
  int iVar7;
  int iVar8;
  pthread_t __target_thread;
  undefined8 uVar9;
  uint uVar10;
  ulong local_68;
  ulong local_60;
  long local_58;
  
  iVar8 = DAT_0022f038;
  lVar2 = tpidr_el0;
  local_58 = *(long *)(lVar2 + 0x28);
  if (param_1 < 2) {
    local_68 = 0;
    local_60 = 0;
    if ((0 < DAT_0022f038) && (iVar7 = gettid(), iVar7 == iVar8)) {
      __target_thread = pthread_self();
      iVar8 = pthread_getname_np(__target_thread,(char *)&local_68,0x10);
      if ((iVar8 == 0) && (local_68 == 0x706f6f6c6e69614d && (local_60 & 0xff) == 0)) {
        uVar9 = FUN_0014fac4();
        uVar5 = DAT_0022db50;
        uVar4 = DAT_0022db48;
        if ((int)uVar9 == 0) goto LAB_0014f8c0;
        if (DAT_0022db48 == 0 && DAT_0022db50 == 0) {
          bVar3 = false;
          bVar6 = true;
          goto LAB_0014fa1c;
        }
        if (DAT_0022db40 == DAT_0022db60) {
          bVar6 = DAT_0022db48 == 0;
          if (DAT_0022db48 == 0) {
LAB_0014f998:
            if (uVar5 == 0) {
              bVar3 = false;
LAB_0014fa1c:
              local_68 = uVar4;
              local_60 = uVar5;
              uVar9 = FUN_00176ad4(DAT_0022db60,&local_68,param_1);
              if (param_1 != 0) {
                uVar10 = DAT_0022db5c;
                if (((!bVar6) && (local_68 == 0)) && (DAT_0022db48 == uVar4)) {
                  uVar10 = DAT_0022db5c & 0xfffffffe;
                  DAT_0022db48 = 0;
                  _DAT_0022db58 = _DAT_0022db58 & 0xfffffffeffffffff;
                }
                bVar6 = false;
                if (local_60 == 0) {
                  bVar6 = bVar3;
                }
                if ((bVar6) && (DAT_0022db50 == uVar5)) {
                  DAT_0022db50 = 0;
                  _DAT_0022db58 = CONCAT44(uVar10,DAT_0022db58) & 0xfffffffdffffffff;
                }
                if ((int)uVar9 == 1) {
                  DAT_0022db48 = 0;
                  DAT_0022db40 = 0;
                  _DAT_0022db58 = 0;
                  DAT_0022db50 = 0;
                  DAT_0022dad8 = 0;
                }
              }
              goto LAB_0014f8c0;
            }
            if ((DAT_0022db40 == DAT_0022db60) && (DAT_0022db50 != 0)) {
              local_68 = 0;
              iVar8 = FUN_0014e7c4(uVar9,DAT_0022db50,&local_68,8);
              uVar1 = local_68;
              if (((local_68 & 7) != 0 || local_68 < 0x1000) || iVar8 == 0) {
                uVar1 = 0;
              }
              if (uVar1 == DAT_0022d8e0 + 0x11c0a48U) {
                uVar9 = FUN_00154d5c(DAT_0022db50,DAT_0022db60);
                if ((int)uVar9 == 0) goto LAB_0014f8c0;
                bVar3 = true;
                goto LAB_0014fa1c;
              }
            }
          }
          else if (DAT_0022db40 == DAT_0022db60) {
            local_68 = 0;
            iVar8 = FUN_0014e7c4(uVar9,DAT_0022db48,&local_68,8);
            uVar1 = local_68;
            if (((local_68 & 7) != 0 || local_68 < 0x1000) || iVar8 == 0) {
              uVar1 = 0;
            }
            if (uVar1 == DAT_0022d8e0 + 0x11c0a48U) {
              uVar9 = FUN_00154d5c(DAT_0022db48,DAT_0022db60);
              if ((int)uVar9 == 0) goto LAB_0014f8c0;
              goto LAB_0014f998;
            }
          }
        }
      }
    }
  }
  uVar9 = 0;
LAB_0014f8c0:
  if (*(long *)(lVar2 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar9);
}

/* ===== FUN_0014fac4 @ 0014fac4 ===== */

void FUN_0014fac4(void)

{
  size_t sVar1;
  byte bVar2;
  size_t __n;
  long lVar3;
  int iVar4;
  ulong uVar5;
  int local_24c;
  undefined1 auStack_248 [512];
  long local_48;
  
  lVar3 = tpidr_el0;
  local_48 = *(long *)(lVar3 + 0x28);
  uVar5 = 0;
  bVar2 = DAT_0022dda8 ^ 1;
  if ((ulong)((long)PTR_nexus_script_port_ui_server_thread_001a36c8 - (long)PTR_DAT_001a36c0) <
      0x201) {
    bVar2 = 1;
  }
  if ((((DAT_0022d8d8 == '\x01') && (DAT_0022f1e0 != 0)) && (DAT_0022dda8 != 0)) &&
     (((__n = (long)PTR_nexus_script_port_ui_server_thread_001a36c8 - (long)PTR_DAT_001a36c0,
       __n != 0 && (bVar2 != 0)) &&
      (uVar5 = FUN_0014e7c4(0,DAT_0022d8e0 + 0x678fe4,&local_24c,4), (int)uVar5 != 0)))) {
    if (local_24c == DAT_0022f1e8) {
      sVar1 = __n;
      if (DAT_0022dda8 == 0) {
        sVar1 = 0;
      }
      uVar5 = FUN_0014e7c4(uVar5,DAT_0022f1e0,auStack_248,sVar1);
      if ((int)uVar5 != 0) {
        if (DAT_0022dda8 == 0) {
          __n = 0;
        }
        iVar4 = memcmp(auStack_248,&DAT_0022f1ec,__n);
        uVar5 = (ulong)(iVar4 == 0);
      }
    }
    else {
      uVar5 = 0;
    }
  }
  if (*(long *)(lVar3 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar5);
}

/* ===== FUN_0014fbe8 @ 0014fbe8 ===== */

void FUN_0014fbe8(int param_1)

{
  int iVar1;
  code *pcVar2;
  
  if (param_1 == 1) {
    iVar1 = pthread_once((pthread_once_t *)&DAT_0022f1d0,FUN_001552c0);
    pcVar2 = DAT_0022f1d8;
    if (DAT_0022f1d8 != (code *)0x0) {
      iVar1 = (*DAT_0022f1d8)(iVar1);
      pcVar2 = (code *)(ulong)(iVar1 == 1);
    }
  }
  else {
    pcVar2 = (code *)0x0;
  }
  DAT_0022db68 = SUB84(pcVar2,0);
  iVar1 = pthread_once((pthread_once_t *)&DAT_0022f1d0,FUN_001552c0);
  if ((DAT_0022f1d8 == (code *)0x0) || (iVar1 = (*DAT_0022f1d8)(iVar1), iVar1 != 1)) {
    DAT_0022db6c = 0;
  }
  return;
}

/* ===== FUN_0014fe14 @ 0014fe14 ===== */

void FUN_0014fe14(long param_1,ulong param_2,char *param_3)

{
  long lVar1;
  int iVar2;
  undefined8 uVar3;
  ulong uVar4;
  undefined1 *puVar5;
  undefined *puVar6;
  long lVar7;
  ulong uVar8;
  char acStack_23c [68];
  undefined1 auStack_1f8 [32];
  undefined1 auStack_1d8 [256];
  undefined1 auStack_d8 [112];
  long local_68;
  
  lVar1 = tpidr_el0;
  local_68 = *(long *)(lVar1 + 0x28);
  uVar3 = FUN_00191314(auStack_d8);
  if (param_2 != 0) {
    uVar8 = 0;
    puVar6 = &DAT_00133b50;
    do {
      uVar4 = param_2 - uVar8;
      if (0xff < uVar4) {
        uVar4 = 0x100;
      }
      iVar2 = FUN_0014e7c4(uVar3,uVar8 + param_1 + DAT_0022d8e0,auStack_1d8,uVar4);
      if (iVar2 == 0) goto LAB_0014ff0c;
      uVar3 = FUN_00191338(auStack_d8,auStack_1d8,uVar4);
      uVar8 = uVar4 + uVar8;
    } while (uVar8 < param_2);
  }
  FUN_00191678(auStack_d8,auStack_1f8);
  lVar7 = 0;
  puVar5 = auStack_1f8;
  do {
    FUN_0015540c(acStack_23c + lVar7,0xffffffffffffffff,3,&DAT_00139d9f,*puVar5);
    lVar7 = lVar7 + 2;
    puVar5 = puVar5 + 1;
  } while (lVar7 != 0x40);
  iVar2 = strcmp(acStack_23c,param_3);
  if (iVar2 == 0) {
    uVar3 = 1;
  }
  else {
    puVar6 = &DAT_00133616;
LAB_0014ff0c:
    FUN_0015540c(&DAT_0022f3ec,0x30,0x30,"body_%lx_%s",param_1,puVar6);
    uVar3 = 0;
    PTR_s_ui_initialize_001a7c40 = &DAT_0022f3ec;
  }
  if (*(long *)(lVar1 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar3);
}

/* ===== FUN_0014ff7c @ 0014ff7c ===== */

void FUN_0014ff7c(long param_1,long param_2,undefined4 *param_3,ulong param_4)

{
  byte bVar1;
  ulong uVar2;
  long lVar3;
  int iVar4;
  undefined8 uVar5;
  size_t __n;
  int local_24c;
  undefined1 auStack_248 [512];
  long local_48;
  
  lVar3 = tpidr_el0;
  local_48 = *(long *)(lVar3 + 0x28);
  uVar5 = 0;
  __n = (long)PTR_nexus_script_port_ui_server_thread_001a36c8 - (long)PTR_DAT_001a36c0;
  bVar1 = 0;
  if (0x200 < __n) {
    bVar1 = DAT_0022dda8;
  }
  if ((((param_4 < 4) || (param_2 != 0x88836c)) || (DAT_0022d8e0 != param_1)) ||
     (((DAT_0022d8d8 == '\0' || (DAT_0022dda0 == 0)) ||
      (((bVar1 & 1) != 0 ||
       (uVar5 = FUN_0014e7c4(0,param_1 + 0x88836c,&local_24c,4), (int)uVar5 == 0))))))
  goto LAB_0015007c;
  if (local_24c == DAT_0022ddac) {
    uVar2 = __n;
    if (DAT_0022dda8 == 0) {
      uVar2 = 0;
    }
    uVar5 = FUN_0014e7c4(uVar5,DAT_0022dda0,auStack_248,uVar2);
    if ((int)uVar5 == 0) goto LAB_0015007c;
    if (DAT_0022dda8 == 0) {
      __n = 0;
    }
    iVar4 = memcmp(auStack_248,&DAT_0022ddb0,__n);
    if (iVar4 == 0) {
      uVar5 = 1;
      local_24c = -0x56428403;
      *param_3 = 0xa9bd7bfd;
      goto LAB_0015007c;
    }
  }
  uVar5 = 0;
LAB_0015007c:
  if (*(long *)(lVar3 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar5);
}

/* ===== FUN_001505d8 @ 001505d8 ===== */

ulong FUN_001505d8(void)

{
  int iVar1;
  ulong uVar2;
  undefined **ppuVar3;
  
  uVar2 = FUN_0014d168(0x59567c,0x20,
                       "9e18529b88a6285f2675ad92537b12875a9735c6fc16cbdc947b58b5e235b5f9");
  if ((int)uVar2 != 0) {
    uVar2 = 0xffffffffffffffff;
    ppuVar3 = &PTR_s_a17505cdb72a066e8360be97ba3fcaee_00198a20;
    do {
      if (uVar2 == 0x10) goto LAB_00150630;
      iVar1 = FUN_0014d168(ppuVar3[-2],ppuVar3[-1],*ppuVar3);
      uVar2 = uVar2 + 1;
      ppuVar3 = ppuVar3 + 3;
    } while (iVar1 != 0);
    if (uVar2 < 0x11) {
      uVar2 = 0;
    }
    else {
LAB_00150630:
      uVar2 = FUN_0014d168(0x75724c,0x14,
                           "b1a8612e3b1b18d5a4fa341f79cdea62de5653b32cf5cc80e8525baefd0f85fa");
      if (((((int)uVar2 != 0) &&
           (uVar2 = FUN_0014d168(0x756bc0,0x68c,
                                 "f30e5b794304199c9860940b334237b8d3040aa8647118c729d9a73ac7df8faa")
           , (int)uVar2 != 0)) &&
          (uVar2 = FUN_0014d168(0x75cddc,0x18,
                                "0f2cf08558111cad0010ddf2c88c3f6312783209bcb27195a3a1879a44a058bb"),
          (int)uVar2 != 0)) &&
         (((uVar2 = FUN_0014d168(0x5db534,0xc60,
                                 "af6384d59fa8cd2716627388fb076db6f786e7d3884ea1f77b3bf85fddd9a9f3")
           , (int)uVar2 != 0 &&
           (uVar2 = FUN_0014d168(0xd06d98,0x5ac,
                                 "68feb4f4a33b0568cf9836c921863f42f30a2e87ddf099f79723d8fa6231776e")
           , (int)uVar2 != 0)) &&
          (uVar2 = FUN_0014d168(0x56f2fc,0x1d0,
                                "c56bb1024e8c5b5cf3b91a235440cd0cba889e4b3fe5838ae6b0c8e059710669"),
          (int)uVar2 != 0)))) {
        iVar1 = FUN_0014d168(0xff21e0,0xd8,
                             "1071ba9e2114fe5b27b428d41d48e90ceade20037a38deae7c6da369c9d078c9");
        uVar2 = (ulong)(iVar1 != 0);
      }
    }
  }
  return uVar2;
}

/* ===== FUN_00150710 @ 00150710 ===== */

void FUN_00150710(long *param_1,ulong *param_2)

{
  long lVar1;
  int iVar2;
  ulong uVar3;
  ulong uVar4;
  long local_58;
  ulong local_50;
  long local_48;
  
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  uVar3 = 0;
  if ((param_1 == (long *)0x0) || (param_2 == (ulong *)0x0)) goto LAB_00150774;
  if ((param_1[2] != 0) && (0x12ff040 < *param_1 + 0x12ff040U)) {
    param_2[1] = 0;
    param_2[2] = 0;
    *param_2 = 0;
    uVar3 = (*(code *)param_1[2])(param_1[1],*param_1 + 0x12ff038,param_2,8);
    if ((int)uVar3 == 0) goto LAB_00150774;
    uVar4 = *param_2;
    uVar3 = 0;
    if ((uVar4 + 0x238 >> 3 < 0x247) || ((uVar4 & 7) != 0)) goto LAB_00150774;
    local_58 = 0;
    local_50 = 0;
    iVar2 = (*(code *)param_1[2])(param_1[1],*param_1 + 0x12ff038,&local_50,8);
    if ((iVar2 != 0) &&
       (((local_50 == uVar4 &&
         (iVar2 = (*(code *)param_1[2])(param_1[1],uVar4,&local_58,8), iVar2 != 0)) &&
        (local_58 == *param_1 + 0x11baa20)))) {
      uVar3 = (*(code *)param_1[2])(param_1[1],*param_2 + 0x88,param_2 + 1,8);
      if ((int)uVar3 == 0) goto LAB_00150774;
      uVar3 = (*(code *)param_1[2])(param_1[1],*param_1 + 0x12f03e4,param_2 + 2,1);
      if ((int)uVar3 == 0) goto LAB_00150774;
      if ((((byte)param_2[2] < 2) && ((uint)param_2[1] < 4)) && (*(uint *)((long)param_2 + 0xc) < 4)
         ) {
        iVar2 = FUN_00155980(param_1,*param_2);
        uVar3 = (ulong)(iVar2 != 0);
        goto LAB_00150774;
      }
    }
  }
  uVar3 = 0;
LAB_00150774:
  if (*(long *)(lVar1 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar3);
}

/* ===== FUN_001508c8 @ 001508c8 ===== */

ulong FUN_001508c8(long *param_1,long param_2)

{
  int iVar1;
  ulong uVar2;
  
  uVar2 = (*(code *)param_1[4])(param_1[1],param_2 + 0x88,8);
  if ((int)uVar2 != 0) {
    iVar1 = (*(code *)param_1[4])(param_1[1],*param_1 + 0x12f03e4,1);
    uVar2 = (ulong)(iVar1 != 0);
  }
  return uVar2;
}

/* ===== FUN_00150920 @ 00150920 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00150920(uint *param_1)

{
  byte *pbVar1;
  uint uVar2;
  uint uVar3;
  undefined4 uVar4;
  long lVar5;
  __pid_t _Var6;
  int iVar7;
  int iVar8;
  int iVar9;
  int *piVar10;
  long lVar11;
  undefined4 *puVar12;
  long lVar13;
  ulong uVar14;
  byte bVar15;
  uint uVar16;
  byte bVar19;
  byte bVar20;
  byte bVar21;
  undefined1 auVar17 [16];
  undefined1 auVar18 [16];
  uint local_d0;
  uint local_cc;
  uint local_c8;
  byte local_70 [20];
  uint local_5c;
  long local_58;
  
  iVar9 = DAT_001a7c50;
  lVar5 = tpidr_el0;
  local_58 = *(long *)(lVar5 + 0x28);
  if (DAT_0022f420 != -1) {
    lVar11 = DAT_0022f420 + 1;
    DAT_0022f420 = lVar11;
    if ((((param_1 != (uint *)0x0) && (-1 < DAT_001a7c50)) && (uVar2 = *param_1, uVar2 < 2)) &&
       ((uVar3 = param_1[1], uVar3 < 4 && (uVar16 = param_1[2], uVar16 < 4)))) {
      local_70[8] = (char)uVar2;
      local_70[0xc] = (char)uVar3;
      local_70[9] = (char)(uVar2 >> 8);
      lVar13 = 0;
      local_70[0x10] = (char)uVar16;
      local_70[0] = 0x42;
      local_70[1] = 0x53;
      local_70[2] = 0x44;
      local_70[3] = 0x51;
      local_70[4] = 1;
      local_70[5] = 0;
      local_70[6] = 0;
      local_70[7] = 0;
      local_70[10] = (char)(uVar2 >> 0x10);
      local_70[0xb] = (char)(uVar2 >> 0x18);
      local_70[0x13] = (char)(uVar16 >> 0x18);
      local_70[0xd] = (char)(uVar3 >> 8);
      local_70[0xe] = (char)(uVar3 >> 0x10);
      local_70[0xf] = (char)(uVar3 >> 0x18);
      local_70[0x12] = (char)(uVar16 >> 0x10);
      local_70[0x11] = (char)(uVar16 >> 8);
      local_5c = 0xffffffff;
      do {
        pbVar1 = local_70 + lVar13;
        lVar13 = lVar13 + 1;
        uVar2 = local_5c ^ *pbVar1;
        uVar3 = -(uVar2 & 1) & 0xedb88320 ^ uVar2 >> 1;
        bVar15 = (byte)uVar2;
        auVar17[0] = bVar15 & (byte)_DAT_0010f9c0;
        bVar19 = (byte)(local_5c >> 8);
        auVar17[1] = bVar19 & (byte)((ulong)_DAT_0010f9c0 >> 8);
        bVar20 = (byte)(local_5c >> 0x10);
        auVar17[2] = bVar20 & (byte)((ulong)_DAT_0010f9c0 >> 0x10);
        bVar21 = (byte)(local_5c >> 0x18);
        auVar17[3] = bVar21 & (byte)((ulong)_DAT_0010f9c0 >> 0x18);
        auVar17[4] = bVar15 & (byte)((ulong)_DAT_0010f9c0 >> 0x20);
        auVar17[5] = bVar19 & (byte)((ulong)_DAT_0010f9c0 >> 0x28);
        auVar17[6] = bVar20 & (byte)((ulong)_DAT_0010f9c0 >> 0x30);
        auVar17[7] = bVar21 & (byte)((ulong)_DAT_0010f9c0 >> 0x38);
        auVar17[8] = bVar15 & (byte)_UNK_0010f9c8;
        auVar17[9] = bVar19 & (byte)((ulong)_UNK_0010f9c8 >> 8);
        auVar17[10] = bVar20 & (byte)((ulong)_UNK_0010f9c8 >> 0x10);
        auVar17[0xb] = bVar21 & (byte)((ulong)_UNK_0010f9c8 >> 0x18);
        auVar17[0xc] = bVar15 & (byte)((ulong)_UNK_0010f9c8 >> 0x20);
        auVar17[0xd] = bVar19 & (byte)((ulong)_UNK_0010f9c8 >> 0x28);
        auVar17[0xe] = bVar20 & (byte)((ulong)_UNK_0010f9c8 >> 0x30);
        auVar17[0xf] = bVar21 & (byte)((ulong)_UNK_0010f9c8 >> 0x38);
        uVar2 = (int)(uVar2 << 0x1e) >> 0x1f & 0xedb88320U ^ uVar3 >> 1;
        auVar17 = NEON_cmeq(auVar17,0,2);
        auVar18[0] = (byte)_DAT_0010fac0 & ~auVar17[0];
        auVar18[1] = (byte)((ulong)_DAT_0010fac0 >> 8) & ~auVar17[1];
        auVar18[2] = (byte)((ulong)_DAT_0010fac0 >> 0x10) & ~auVar17[2];
        auVar18[3] = (byte)((ulong)_DAT_0010fac0 >> 0x18) & ~auVar17[3];
        auVar18[4] = (byte)((ulong)_DAT_0010fac0 >> 0x20) & ~auVar17[4];
        auVar18[5] = (byte)((ulong)_DAT_0010fac0 >> 0x28) & ~auVar17[5];
        auVar18[6] = (byte)((ulong)_DAT_0010fac0 >> 0x30) & ~auVar17[6];
        auVar18[7] = (byte)((ulong)_DAT_0010fac0 >> 0x38) & ~auVar17[7];
        auVar18[8] = (byte)_UNK_0010fac8 & ~auVar17[8];
        auVar18[9] = (byte)((ulong)_UNK_0010fac8 >> 8) & ~auVar17[9];
        auVar18[10] = (byte)((ulong)_UNK_0010fac8 >> 0x10) & ~auVar17[10];
        auVar18[0xb] = (byte)((ulong)_UNK_0010fac8 >> 0x18) & ~auVar17[0xb];
        auVar18[0xc] = (byte)((ulong)_UNK_0010fac8 >> 0x20) & ~auVar17[0xc];
        auVar18[0xd] = (byte)((ulong)_UNK_0010fac8 >> 0x28) & ~auVar17[0xd];
        auVar18[0xe] = (byte)((ulong)_UNK_0010fac8 >> 0x30) & ~auVar17[0xe];
        auVar18[0xf] = (byte)((ulong)_UNK_0010fac8 >> 0x38) & ~auVar17[0xf];
        auVar17 = NEON_ext(auVar18,auVar18,8,1);
        uVar16 = CONCAT13(auVar18[3] ^ auVar17[3],
                          CONCAT12(auVar18[2] ^ auVar17[2],
                                   CONCAT11(auVar18[1] ^ auVar17[1],auVar18[0] ^ auVar17[0])));
        local_5c = uVar16 ^ (int)(uVar3 << 0x1a) >> 0x1f & 0x76dc4190U ^
                   (int)(uVar2 << 0x1a) >> 0x1f & 0xedb88320U ^ uVar2 >> 6 ^
                   (uint)(CONCAT17(auVar18[7] ^ auVar17[7],
                                   CONCAT16(auVar18[6] ^ auVar17[6],
                                            CONCAT15(auVar18[5] ^ auVar17[5],
                                                     CONCAT14(auVar18[4] ^ auVar17[4],uVar16)))) >>
                         0x20);
      } while (lVar13 != 0x14);
      local_5c = ~local_5c;
      _Var6 = getpid();
      FUN_0015540c(&local_d0,0x60,0x60,"bsd-quality-%d-%llu.tmp",_Var6,lVar11);
      iVar7 = openat(iVar9,(char *)&local_d0,0x880c1,0x180);
      if (-1 < iVar7) {
        uVar14 = 0;
        do {
          lVar11 = __write_chk(iVar7,local_70 + uVar14,0x18 - uVar14,0xffffffffffffffff);
          if (lVar11 < 0) {
            piVar10 = (int *)__errno();
            if (*piVar10 != 4) goto LAB_00150bbc;
          }
          else {
            uVar14 = lVar11 + uVar14;
            if (lVar11 == 0) break;
          }
        } while (uVar14 < 0x18);
        if (uVar14 == 0x18) {
          iVar8 = fsync(iVar7);
          iVar7 = close(iVar7);
          if (iVar7 == 0 && iVar8 == 0) {
            iVar7 = renameat(iVar9,(char *)&local_d0,iVar9,"bsd-debug-quality.bin");
            if (iVar7 == 0) {
              iVar9 = fsync(iVar9);
              if (iVar9 == 0) {
                uVar14 = FUN_00155a40(DAT_001a7c50,&local_d0);
                if ((int)uVar14 == 0) goto LAB_00150be8;
                if ((*param_1 == local_d0) && (param_1[1] == local_cc)) {
                  uVar14 = (ulong)(param_1[2] == local_c8);
                  goto LAB_00150be8;
                }
              }
              goto LAB_00150be4;
            }
          }
        }
        else {
LAB_00150bbc:
          iVar7 = close(iVar7);
        }
        puVar12 = (undefined4 *)__errno(iVar7);
        uVar4 = *puVar12;
        unlinkat(iVar9,(char *)&local_d0,0);
        *puVar12 = uVar4;
      }
    }
  }
LAB_00150be4:
  uVar14 = 0;
LAB_00150be8:
  if (*(long *)(lVar5 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar14);
}

/* ===== FUN_001525f4 @ 001525f4 ===== */

void FUN_001525f4(long *param_1,long param_2,long param_3)

{
  undefined4 uVar1;
  byte bVar2;
  long lVar3;
  char cVar4;
  int iVar5;
  uint uVar6;
  undefined4 *puVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  long lVar13;
  long lVar14;
  ulong uVar15;
  uint uVar16;
  long *local_b8;
  byte local_ac [68];
  long local_68;
  
  lVar3 = tpidr_el0;
  local_68 = *(long *)(lVar3 + 0x28);
  puVar7 = (undefined4 *)__errno();
  uVar1 = *puVar7;
  local_b8 = (long *)0x0;
  if ((((param_1 != (long *)0x0) && (param_2 != 0)) && (param_3 != 0)) &&
     ((iVar5 = (**(code **)(*param_1 + 0x30))(param_1,&local_b8,0x10006), iVar5 == 0 &&
      (local_b8 != (long *)0x0)))) {
    lVar8 = (**(code **)(*local_b8 + 0x78))();
    if (lVar8 != 0) {
      (**(code **)(*local_b8 + 0x88))();
    }
    iVar5 = (**(code **)(*local_b8 + 0x98))(local_b8,0xc);
    if (iVar5 == 0) {
      uVar6 = FUN_0015540c(local_ac,0x41,0x41,"native_%s_%s",&DAT_00134ec1,param_2);
      if (0xffffffbf < uVar6 - 0x41) {
        uVar15 = 0;
        do {
          bVar2 = local_ac[uVar15];
          uVar16 = (uint)bVar2;
          if (bVar2 - 0x41 < 0x1a) {
            uVar16 = bVar2 + 0x20;
            local_ac[uVar15] = (byte)uVar16;
          }
          if (((0x19 < uVar16 - 0x61) && (uVar16 != 0x5f)) && (9 < uVar16 - 0x30))
          goto LAB_0015276c;
          uVar15 = uVar15 + 1;
        } while (uVar6 != uVar15);
        lVar9 = (**(code **)(*local_b8 + 0x30))(local_b8,"nexus/loader/StartupDiagnostics");
        if ((((((lVar9 != 0) && (cVar4 = (**(code **)(*local_b8 + 0x720))(), cVar4 == '\0')) &&
              (lVar10 = (**(code **)(*local_b8 + 0x388))
                                  (local_b8,lVar9,"failure",
                                   "(Ljava/lang/String;Ljava/lang/Throwable;)V"), lVar10 != 0)) &&
             ((cVar4 = (**(code **)(*local_b8 + 0x720))(), cVar4 == '\0' &&
              (lVar11 = (**(code **)(*local_b8 + 0x30))(local_b8,"java/lang/LinkageError"),
              lVar11 != 0)))) &&
            ((cVar4 = (**(code **)(*local_b8 + 0x720))(), cVar4 == '\0' &&
             ((lVar12 = (**(code **)(*local_b8 + 0x108))
                                  (local_b8,lVar11,"<init>","(Ljava/lang/String;)V"), lVar12 != 0 &&
              (cVar4 = (**(code **)(*local_b8 + 0x720))(), cVar4 == '\0')))))) &&
           ((lVar13 = (**(code **)(*local_b8 + 0x538))(local_b8,local_ac), lVar13 != 0 &&
            ((((cVar4 = (**(code **)(*local_b8 + 0x720))(), cVar4 == '\0' &&
               (lVar14 = (**(code **)(*local_b8 + 0x538))(local_b8,param_3), lVar14 != 0)) &&
              (cVar4 = (**(code **)(*local_b8 + 0x720))(), cVar4 == '\0')) &&
             ((lVar11 = (**(code **)(*local_b8 + 0xe0))(local_b8,lVar11,lVar12,lVar14), lVar11 != 0
              && (cVar4 = (**(code **)(*local_b8 + 0x720))(), cVar4 == '\0')))))))) {
          (**(code **)(*local_b8 + 0x468))(local_b8,lVar9,lVar10,lVar13,lVar11);
        }
      }
LAB_0015276c:
      cVar4 = (**(code **)(*local_b8 + 0x720))();
      if (cVar4 != '\0') {
        (**(code **)(*local_b8 + 0x88))();
      }
      (**(code **)(*local_b8 + 0xa0))(local_b8,0);
    }
    cVar4 = (**(code **)(*local_b8 + 0x720))();
    if (cVar4 != '\0') {
      (**(code **)(*local_b8 + 0x88))();
    }
    if (lVar8 != 0) {
      (**(code **)(*local_b8 + 0x68))(local_b8,lVar8);
      (**(code **)(*local_b8 + 0xb8))(local_b8,lVar8);
    }
  }
  *puVar7 = uVar1;
  if (*(long *)(lVar3 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

/* ===== FUN_001529bc @ 001529bc ===== */

undefined8 FUN_001529bc(ulong *param_1)

{
  ulong uVar1;
  ulong uVar2;
  int iVar3;
  char *pcVar4;
  size_t sVar5;
  ulong uVar6;
  int *piVar7;
  ulong uVar8;
  ulong uVar9;
  ulong uVar10;
  char *__s;
  
  __s = (char *)param_1[1];
  if (((__s != (char *)0x0) && (pcVar4 = strrchr(__s,0x2f), pcVar4 != (char *)0x0)) &&
     (iVar3 = strcmp(pcVar4 + 1,"libg.so"), uVar2 = DAT_0010f740, iVar3 == 0)) {
    DAT_0022e018 = DAT_0022e018 + 1;
    uVar6 = (ulong)(ushort)param_1[3];
    if (uVar6 != 0) {
      piVar7 = (int *)param_1[2];
      uVar9 = DAT_0022f4b0 & 0xffffffff;
      iVar3 = DAT_0022f4b0._4_4_;
      do {
        uVar8 = uVar9;
        if (((*piVar7 == 1) && ((*(byte *)(piVar7 + 1) & 1) != 0)) && (iVar3 == 0)) {
          uVar10 = *(ulong *)(piVar7 + 10);
          if (uVar10 != 0) {
            if (((!CARRY8(*param_1,*(ulong *)(piVar7 + 4))) && (uVar9 < 8)) &&
               (uVar1 = *(ulong *)(piVar7 + 4) + *param_1, !CARRY8(uVar1,uVar10))) {
              uVar8 = uVar9 + 1;
              iVar3 = 0;
              DAT_0022f4b0 = CONCAT44(DAT_0022f4b0._4_4_,(int)uVar8);
              (&DAT_0022f430)[uVar9] = uVar1;
              (&DAT_0022f470)[uVar9] = uVar1 + uVar10;
              goto LAB_00152a44;
            }
          }
          uVar8 = 0;
          iVar3 = 1;
          DAT_0022f4b0 = uVar2;
        }
LAB_00152a44:
        piVar7 = piVar7 + 0xe;
        uVar6 = uVar6 - 1;
        uVar9 = uVar8;
      } while (uVar6 != 0);
    }
    sVar5 = strlen(__s);
    if (((sVar5 < 0x1000) && (*__s == '/')) && (pcVar4 = strchr(__s,0x21), pcVar4 == (char *)0x0)) {
      DAT_0022d8e0 = *param_1;
      sVar5 = strlen(__s);
      __memcpy_chk(&DAT_0022e01c,__s,sVar5 + 1,0x1000);
    }
  }
  return 0;
}

/* ===== FUN_00152b20 @ 00152b20 ===== */

ulong FUN_00152b20(void)

{
  int iVar1;
  ulong uVar2;
  
  uVar2 = FUN_00191058(DAT_0022d8e0,FUN_0014e7c4,0,&DAT_0010fbf8,0x13);
  if ((int)uVar2 != 0) {
    iVar1 = FUN_00191170(DAT_0022d8e0,FUN_0014e7c4,0);
    uVar2 = (ulong)(iVar1 != 0);
  }
  return uVar2;
}

/* ===== FUN_00153060 @ 00153060 ===== */

void FUN_00153060(void)

{
  long lVar1;
  int iVar2;
  uint uVar3;
  ulong uVar4;
  undefined8 *puVar5;
  long lVar6;
  undefined8 uVar7;
  long lVar8;
  char *pcVar9;
  ulong uVar10;
  long lVar11;
  long lVar12;
  undefined8 uVar13;
  undefined1 auStack_7c8 [184];
  undefined1 auStack_710 [512];
  undefined1 auStack_510 [512];
  undefined1 auStack_310 [512];
  long local_110 [12];
  long local_b0;
  undefined8 local_a8;
  long local_a0;
  undefined8 local_98;
  long local_90;
  undefined8 local_88;
  long local_80;
  undefined8 local_78;
  long local_70;
  
  lVar1 = tpidr_el0;
  local_70 = *(long *)(lVar1 + 0x28);
  PTR_s_ui_initialize_001a7c40 = s_postbattle_code_guards_00136be3;
  uVar4 = FUN_00152b7c();
  if (((((((int)uVar4 == 0) ||
         (uVar4 = FUN_00191058(DAT_0022d8e0,FUN_0014e7c4,0,&DAT_0010fbf8,0x13), (int)uVar4 == 0)) ||
        (uVar4 = FUN_00191170(DAT_0022d8e0,FUN_0014e7c4,0), (int)uVar4 == 0)) ||
       (((uVar4 = FUN_0014fe14(0x66ad48,0x1c,
                               "9edb63b79febb9496a60828270f995d5b618902d80ee1d7caca7a98df8ad3863"),
         (int)uVar4 == 0 ||
         (uVar4 = FUN_0014fe14(0x5921a0,0xb0,
                               "6b5853b7737f55c2722ea9a461730d5a0274ec3f756ad12c93cede1764df931a"),
         (int)uVar4 == 0)) ||
        ((uVar4 = FUN_0014fe14(0x887c14,0xe0,
                               "c83656694e0a00083bf7f59348635e4e0b9b5a8edc3bd225809dc8ef8c760356"),
         (int)uVar4 == 0 ||
         ((uVar4 = FUN_0014fe14(0x888218,0xa4,
                                "d8909f0e5d5ba0ce41dbe44eb6293f6a8242f1a45d597bfb8a1977a4ceecf2e7"),
          (int)uVar4 == 0 ||
          (uVar4 = FUN_0014fe14(0xe1f0a0,0xc,
                                "c4c1804266b14a1b5639f8ac2e088ecbd01d17033d0f7d80ad124f703c2d60aa"),
          (int)uVar4 == 0)))))))) ||
      (uVar4 = FUN_0014fe14(0x905bf8,0x2da8,
                            "20907f70cfbab3552cbf91ddb3508be4b8ed39570a1549ec348600cbc8b60ecd"),
      (int)uVar4 == 0)) ||
     ((((((uVar4 = FUN_0014fe14(0xaca9d8,100,
                                "18fca600c222900c6e286a8d734adf700bfa3a4eac4448fc398fa04375397bfc"),
          (int)uVar4 == 0 ||
          (uVar4 = FUN_0014fe14(0xf3e268,0x86c,
                                "c8628934ac257e1c3af88117aaa68471371e7fda0abea574ec4b36213b6493c4"),
          (int)uVar4 == 0)) ||
         (uVar4 = FUN_0014fe14(0xf3ead4,8,
                               "2a1fd00c285c56f51aac5cf74a3ac14ad7a1c31b4d6834053a65a90742992ca8"),
         (int)uVar4 == 0)) ||
        ((uVar4 = FUN_0014fe14(0x90aff8,0x2eec,
                               "2e46ec3e6df92b27880099567660412665865a4ddc3b4cef199c3bb877ff339e"),
         (int)uVar4 == 0 ||
         (uVar4 = FUN_0014fe14(0x91addc,0xcac,
                               "686c65d371b836433b18b0737fb3b08640d9b9ef7a1763aec9e87e92bf757c67"),
         (int)uVar4 == 0)))) ||
       ((uVar4 = FUN_0014fe14(0x91bb7c,8,
                              "e9afda7fdce9e2e1044ebf5a1a0325a13eecdc10f4f89decc9769e92ed1b8e34"),
        (int)uVar4 == 0 ||
        ((uVar4 = FUN_0014fe14(0xf41ba0,0x48,
                               "fa720cb094c0222bd9154df9dd75c13e6c700a31682aa270aa8cfc8b08447581"),
         (int)uVar4 == 0 ||
         (uVar4 = FUN_0014fe14(0xac319c,0xe0,
                               "ecc8aa9a360b1061baa9a987f682adb97f925bc7e1792933fb79367621924911"),
         (int)uVar4 == 0)))))) ||
      ((uVar4 = FUN_0014fe14(0x88836c,0xe8,
                             "44f795c76de8bc62ebc00eca9d0f192c685f30a2ca3ecdd4b0d016ad659b9ac6"),
       (int)uVar4 == 0 ||
       (uVar4 = FUN_0014fe14(0x678964,0x788,
                             "8ce589cc61489c895ba2116daf1f2ab2d73a0ac156e3b2006e1eae2b8f520057"),
       (int)uVar4 == 0)))))) goto LAB_001534cc;
  lVar8 = DAT_0022d8e0 + 0x772870;
  local_a0 = DAT_0022d8e0 + 0x5921a0;
  local_a8 = DAT_0010f888;
  local_98 = DAT_0010f798;
  local_90 = DAT_0022d8e0 + 0x88836c;
  local_80 = DAT_0022d8e0 + 0x678fe4;
  local_88 = DAT_0010f768;
  local_78 = DAT_0010f908;
  PTR_s_ui_initialize_001a7c40 = s_postbattle_island_allocate_0013afa9;
  local_b0 = lVar8;
  puVar5 = (undefined8 *)__emutls_get_address(&DAT_001a7c88);
  lVar11 = 0;
  lVar12 = 0;
  do {
    lVar6 = FUN_0015b8f4(*(undefined8 *)((long)&local_b0 + lVar11),(&DAT_0010fb80)[lVar12],
                         *(undefined4 *)((long)&local_a8 + lVar11));
    uVar13 = puVar5[1];
    uVar7 = *puVar5;
    local_110[lVar12 + 8] = lVar6;
    lVar12 = lVar12 + 1;
    *(undefined8 *)((long)local_110 + lVar11 + 8) = uVar13;
    *(undefined8 *)((long)local_110 + lVar11) = uVar7;
    lVar11 = lVar11 + 0x10;
  } while (lVar11 != 0x40);
  if (local_110[8] == 0) {
    lVar8 = 0;
LAB_0015346c:
    lVar11 = (&local_b0)[lVar8 * 2];
    pcVar9 = (char *)local_110[lVar8 * 2];
    lVar12 = local_110[lVar8 * 2 + 1];
  }
  else {
    iVar2 = FUN_00190d68(lVar8,local_110[8],(long)&local_a8 + 4);
    if (iVar2 == 0) {
      lVar8 = 0;
    }
    else {
      if (local_110[9] == 0) {
        lVar8 = 1;
        goto LAB_0015346c;
      }
      iVar2 = FUN_00190d68(local_a0,local_110[9],(long)&local_98 + 4);
      if (iVar2 == 0) {
        lVar8 = 1;
      }
      else {
        if (local_110[10] == 0) {
          lVar8 = 2;
          goto LAB_0015346c;
        }
        iVar2 = FUN_00190d68(local_90,local_110[10],(long)&local_88 + 4);
        if (iVar2 == 0) {
          lVar8 = 2;
        }
        else {
          if (local_110[0xb] == 0) {
            lVar8 = 3;
            goto LAB_0015346c;
          }
          uVar7 = FUN_00190d68(local_80,local_110[0xb],(long)&local_78 + 4);
          if ((int)uVar7 != 0) {
            uVar10 = (long)PTR_nexus_script_port_ui_server_thread_001a36c8 - (long)PTR_DAT_001a36c0;
            PTR_s_ui_initialize_001a7c40 = s_postbattle_island_size_00135b91;
            if (uVar10 < 0x201) {
              PTR_s_ui_initialize_001a7c40 = s_postbattle_island_readback_00131841;
              uVar7 = FUN_0014e7c4(uVar7,local_110[10],auStack_310,uVar10);
              if (((((int)uVar7 == 0) ||
                   (uVar7 = FUN_0014e7c4(uVar7,local_110[0xb],auStack_510,uVar10), (int)uVar7 == 0))
                  || (iVar2 = FUN_0014e7c4(uVar7,local_110[9],auStack_710,uVar10), iVar2 == 0)) ||
                 (((iVar2 = FUN_00152b7c(), iVar2 == 0 || (iVar2 = FUN_00152b20(), iVar2 == 0)) ||
                  ((iVar2 = FUN_0014fe14(0x88836c,0xe8,
                                         "44f795c76de8bc62ebc00eca9d0f192c685f30a2ca3ecdd4b0d016ad659b9ac6"
                                        ), iVar2 == 0 ||
                   (iVar2 = FUN_0014fe14(0x678964,0x788,
                                         "8ce589cc61489c895ba2116daf1f2ab2d73a0ac156e3b2006e1eae2b8f520057"
                                        ), iVar2 == 0)))))) {
                uVar4 = 0;
              }
              else {
                PTR_s_ui_initialize_001a7c40 = s_postbattle_write_or_readback_00137c48;
                uVar3 = FUN_0015c9e0(&local_b0,&DAT_0019ae00,auStack_7c8);
                uVar4 = (ulong)uVar3;
                if (uVar3 == 1) {
                  DAT_0022dda0 = local_110[10];
                  DAT_0022ddac = local_88._4_4_;
                  DAT_0022dda8 = 1;
                  __memcpy_chk(&DAT_0022ddb0,auStack_310,uVar10,0x200);
                  DAT_0022f1e0 = local_110[0xb];
                  DAT_0022f1e8 = local_78._4_4_;
                  __memcpy_chk(&DAT_0022f1ec,auStack_510,uVar10,0x200);
                  FUN_0015d078(local_110[9],local_98._4_4_,auStack_710);
                  DAT_0022d8d8 = 1;
                }
                else {
                  FUN_0015cd20(DAT_0022d8e0,auStack_7c8);
                  PTR_s_ui_initialize_001a7c40 = (undefined *)((long)&DAT_0025c9d8 + 4);
                }
              }
            }
            else {
              uVar4 = 0;
            }
            goto LAB_001534cc;
          }
          lVar8 = 3;
        }
      }
    }
    pcVar9 = "publish";
    lVar12 = 0;
    lVar11 = (&local_b0)[lVar8 * 2];
  }
  FUN_0015c6f8(DAT_0022d8e0,lVar11,lVar8,pcVar9,lVar12);
  uVar4 = 0;
  PTR_s_ui_initialize_001a7c40 = (undefined *)((long)&DAT_0025c9d8 + 4);
LAB_001534cc:
  if (*(long *)(lVar1 + 0x28) != local_70) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar4);
  }
  return;
}

/* ===== FUN_00153670 @ 00153670 ===== */

void FUN_00153670(void)

{
  long lVar1;
  undefined *puVar2;
  undefined *puVar3;
  undefined *puVar4;
  long lVar5;
  int iVar6;
  uint uVar7;
  undefined8 uVar8;
  void *__dest;
  undefined4 *__addr;
  ulong __n;
  long lVar9;
  int local_80;
  undefined4 local_7c;
  long local_78;
  undefined8 local_70;
  long local_68;
  undefined8 local_60;
  long local_58;
  
  lVar1 = tpidr_el0;
  local_58 = *(long *)(lVar1 + 0x28);
  if (DAT_0025c8d8 != 0 && DAT_0025c8d0 != 0) {
    uVar8 = FUN_00152b7c();
    if (((int)uVar8 == 0) || (uVar8 = FUN_00168e48(), lVar5 = DAT_0022d8e0, (int)uVar8 == 0))
    goto LAB_00153938;
    local_78 = DAT_0022d8e0 + 0x85cce8;
    lVar9 = DAT_0022d8e0 + 0x89902c;
    local_70 = DAT_0010f890;
    local_60 = DAT_0010f768;
    local_68 = lVar9;
    __dest = (void *)FUN_00168f3c();
    __addr = (undefined4 *)FUN_00168f3c(lVar9);
    puVar2 = PTR_DAT_001a36c0;
    uVar8 = 0;
    if ((__dest == (void *)0x0) ||
       ((__addr == (undefined4 *)0x0 ||
        (__n = (long)PTR_nexus_script_port_ui_server_thread_001a36c8 - (long)PTR_DAT_001a36c0,
        0x200 < __n)))) goto LAB_00153938;
    memcpy(__dest,PTR_DAT_001a36c0,__n);
    lVar9 = (long)PTR_DAT_001a36d0 - (long)puVar2;
    uVar8 = FUN_00190d68((long)__dest + lVar9,lVar5 + 0x85ccec,&local_7c);
    puVar4 = PTR_DAT_001a36e8;
    puVar3 = PTR_DAT_001a36e0;
    if ((int)uVar8 == 0) goto LAB_00153938;
    *(undefined4 *)((long)__dest + ((long)PTR_DAT_001a36d8 - (long)puVar2)) = 0x910003e0;
    *(int *)((long)__dest + ((long)puVar3 - (long)puVar2)) = (int)local_70;
    *(undefined4 *)(lVar9 + (long)__dest) = local_7c;
    *(code **)((long)__dest + ((long)puVar4 - (long)puVar2)) = FUN_001690ac;
    uVar8 = FUN_00190d68(__addr + 1,local_68 + 4,&local_7c);
    if ((int)uVar8 == 0) goto LAB_00153938;
    *__addr = (int)local_60;
    __addr[1] = local_7c;
    *(undefined8 *)(__addr + 4) = 0xd61f020058000050;
    *(code **)(__addr + 6) = FUN_00169170;
    FUN_00193944(__dest,(long)__dest + __n);
    FUN_00193944(__addr,__addr + 8);
    iVar6 = mprotect(__dest,DAT_0022f020,5);
    if ((iVar6 == 0) && (iVar6 = mprotect(__addr,DAT_0022f020,5), iVar6 == 0)) {
      uVar8 = FUN_00190d68(local_78,__dest,(long)&local_70 + 4);
      if (((int)uVar8 == 0) ||
         (((uVar8 = FUN_00190d68(local_68,__addr + 4,(long)&local_60 + 4), (int)uVar8 == 0 ||
           (uVar8 = FUN_00152b7c(), (int)uVar8 == 0)) || (uVar8 = FUN_00168e48(), (int)uVar8 == 0)))
         ) goto LAB_00153938;
      DAT_00281a40 = __addr;
      iVar6 = FUN_0015c8e4(uVar8,local_78,local_70._4_4_);
      DataMemoryBarrier(2,3);
      uVar8 = FUN_00193944(local_78,local_78 + 4);
      if (((iVar6 == 0) || (uVar8 = FUN_0014e7c4(uVar8,local_78,&local_80,4), (int)uVar8 == 0)) ||
         (local_80 != local_70._4_4_)) {
        uVar7 = 1;
      }
      else {
        uVar7 = FUN_0015c8e4(uVar8,local_68,local_60._4_4_);
        DataMemoryBarrier(2,3);
        uVar8 = FUN_00193944(local_68,local_68 + 4);
        if (uVar7 != 0) {
          uVar8 = FUN_0014e7c4(uVar8,local_68,&local_80,4);
          if (((int)uVar8 != 0) && (local_80 == local_60._4_4_)) {
            uVar8 = 1;
            DAT_0025c928._0_4_ = 1;
            goto LAB_00153938;
          }
          uVar7 = 0;
        }
      }
      FUN_0015c8e4(uVar8,local_78,local_70 & 0xffffffff);
      DataMemoryBarrier(2,3);
      uVar8 = FUN_00193944(local_78,local_78 + 4);
      uVar8 = FUN_0014e7c4(uVar8,local_78,&local_80,4);
      if (((int)uVar8 == 0) || (local_80 != (int)local_70)) {
LAB_001539ec:
        FUN_001524e8("fatal","social_restore_unverified");
                    /* WARNING: Subroutine does not return */
        abort();
      }
      if ((uVar7 & 1) == 0) {
        FUN_0015c8e4(uVar8,local_68,local_60 & 0xffffffff);
        DataMemoryBarrier(2,3);
        uVar8 = FUN_00193944(local_68,local_68 + 4);
        iVar6 = FUN_0014e7c4(uVar8,local_68,&local_80,4);
        if ((iVar6 == 0) || (local_80 != (int)local_60)) goto LAB_001539ec;
      }
    }
  }
  uVar8 = 0;
LAB_00153938:
  if (*(long *)(lVar1 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar8);
}

/* ===== FUN_00153a10 @ 00153a10 ===== */

void FUN_00153a10(long *param_1)

{
  long lVar1;
  char cVar2;
  char cVar3;
  int iVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  char *pcVar8;
  long *local_50;
  long local_48;
  
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  local_50 = (long *)0x0;
  if (((param_1 == (long *)0x0) ||
      (iVar4 = (**(code **)(*param_1 + 0x30))(param_1,&local_50,0x10006), iVar4 != 0)) ||
     (local_50 == (long *)0x0)) {
    FUN_001524e8("callback_unavailable","GetEnv_failed");
    pcVar8 = "callback_environment";
  }
  else {
    lVar5 = (**(code **)(*local_50 + 0x30))(local_50,"nexus/loader/NexusLoader");
    if ((lVar5 == 0) || (cVar2 = (**(code **)(*local_50 + 0x720))(), cVar2 != '\0')) {
      lVar7 = 0;
      pcVar8 = "callback_loader_class";
    }
    else {
      lVar6 = (**(code **)(*local_50 + 0x388))
                        (local_50,lVar5,"NativeLoaded","(Ljava/lang/String;)Z");
      if ((lVar6 == 0) || (cVar2 = (**(code **)(*local_50 + 0x720))(), cVar2 != '\0')) {
        lVar7 = 0;
        pcVar8 = "callback_method";
      }
      else {
        lVar7 = (**(code **)(*local_50 + 0x538))(local_50,"NexusUI");
        if ((lVar7 == 0) || (cVar2 = (**(code **)(*local_50 + 0x720))(), cVar2 != '\0')) {
          pcVar8 = "callback_argument";
        }
        else {
          cVar2 = (**(code **)(*local_50 + 0x3a8))(local_50,lVar5,lVar6,lVar7);
          cVar3 = (**(code **)(*local_50 + 0x720))();
          if (cVar3 == '\0') {
            (**(code **)(*local_50 + 0xb8))(local_50,lVar7);
            (**(code **)(*local_50 + 0xb8))(local_50,lVar5);
            if (cVar2 == '\0') {
              FUN_001525f4(param_1,"callback_rejected","required_synchronous_callback_absent");
            }
            FUN_001524e8("callback_return","NexusUI");
            goto LAB_00153b54;
          }
          pcVar8 = "callback_invocation";
        }
      }
    }
    cVar2 = (**(code **)(*local_50 + 0x720))();
    if (cVar2 != '\0') {
      (**(code **)(*local_50 + 0x88))();
    }
    if (lVar7 != 0) {
      (**(code **)(*local_50 + 0xb8))(local_50,lVar7);
    }
    if (lVar5 != 0) {
      (**(code **)(*local_50 + 0xb8))(local_50,lVar5);
    }
    FUN_001524e8("callback_unavailable","ModuleHost_lookup_or_invocation_failed");
  }
  FUN_001525f4(param_1,pcVar8,"required_synchronous_callback_absent");
LAB_00153b54:
  if (*(long *)(lVar1 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00153cbc @ 00153cbc ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

FILE * FUN_00153cbc(uint param_1,undefined4 *param_2,long param_3,ulong param_4)

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
  puVar10 = (undefined8 *)__emutls_get_address(&DAT_001a7c68);
  puVar2 = &DAT_00133b55;
  if (param_1 != 0x7fff0001) {
    puVar2 = &DAT_00134ec4;
  }
  puVar10[5] = 0;
  puVar10[6] = 0;
  *puVar10 = puVar2;
  puVar10[1] = "write";
  puVar10[2] = &DAT_00137150;
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
    if ((long)pFVar11 < 0) goto LAB_00153db8;
LAB_00153e1c:
    *puVar18 = (ulong)pFVar11;
    if ((long)pFVar11 < 0) {
LAB_00153e28:
      iVar19 = *piVar9;
      goto LAB_0015404c;
    }
LAB_00154b0c:
    *(undefined4 *)(puVar10 + 6) = 0;
    pFVar17 = pFVar11;
  }
  else {
    pFVar11 = (FILE *)syscall(0x44,(ulong)param_1,param_2,param_3,param_4);
    if (-1 < (long)pFVar11) goto LAB_00153e1c;
LAB_00153db8:
    iVar19 = *piVar9;
    if (iVar19 == 4) {
      if (param_1 == 0x7fff0001) {
        uVar8 = getpid();
        pFVar11 = (FILE *)syscall(0x10f,(ulong)uVar8,&local_288,1,&local_298,1,0);
      }
      else {
        pFVar11 = (FILE *)syscall(0x44,(ulong)param_1,param_2,param_3,param_4);
      }
      if (-1 < (long)pFVar11) goto LAB_00153e1c;
      iVar19 = *piVar9;
      if (iVar19 != 4) goto LAB_00154048;
      if (param_1 == 0x7fff0001) {
        uVar8 = getpid();
        pFVar11 = (FILE *)syscall(0x10f,(ulong)uVar8,&local_288,1,&local_298,1,0);
      }
      else {
        pFVar11 = (FILE *)syscall(0x44,(ulong)param_1,param_2,param_3,param_4);
      }
      if (-1 < (long)pFVar11) goto LAB_00153e1c;
      iVar19 = *piVar9;
      if (iVar19 != 4) goto LAB_00154048;
      if (param_1 == 0x7fff0001) {
        uVar8 = getpid();
        pFVar11 = (FILE *)syscall(0x10f,(ulong)uVar8,&local_288,1,&local_298,1,0);
      }
      else {
        pFVar11 = (FILE *)syscall(0x44,(ulong)param_1,param_2,param_3,param_4);
      }
      if (-1 < (long)pFVar11) goto LAB_00153e1c;
      iVar19 = *piVar9;
      if (iVar19 != 4) goto LAB_00154048;
      if (param_1 == 0x7fff0001) {
        uVar8 = getpid();
        pFVar11 = (FILE *)syscall(0x10f,(ulong)uVar8,&local_288,1,&local_298,1,0);
      }
      else {
        pFVar11 = (FILE *)syscall(0x44,(ulong)param_1,param_2,param_3,param_4);
      }
      if (-1 < (long)pFVar11) goto LAB_00153e1c;
      iVar19 = *piVar9;
      if (iVar19 != 4) goto LAB_00154048;
      if (param_1 == 0x7fff0001) {
        uVar8 = getpid();
        pFVar11 = (FILE *)syscall(0x10f,(ulong)uVar8,&local_288,1,&local_298,1,0);
      }
      else {
        pFVar11 = (FILE *)syscall(0x44,(ulong)param_1,param_2,param_3,param_4);
      }
      if (-1 < (long)pFVar11) goto LAB_00153e1c;
      iVar19 = *piVar9;
      if (iVar19 != 4) goto LAB_00154048;
      if (param_1 == 0x7fff0001) {
        uVar8 = getpid();
        pFVar11 = (FILE *)syscall(0x10f,(ulong)uVar8,&local_288,1,&local_298,1,0);
      }
      else {
        pFVar11 = (FILE *)syscall(0x44,(ulong)param_1,param_2,param_3,param_4);
      }
      if ((long)pFVar11 < 0) {
        iVar19 = *piVar9;
        if (iVar19 != 4) goto LAB_00154048;
        if (param_1 == 0x7fff0001) {
          uVar8 = getpid();
          pFVar11 = (FILE *)syscall(0x10f,(ulong)uVar8,&local_288,1,&local_298,1,0);
        }
        else {
          pFVar11 = (FILE *)syscall(0x44,(ulong)param_1,param_2,param_3,param_4);
        }
      }
      *puVar18 = (ulong)pFVar11;
      if (-1 < (long)pFVar11) goto LAB_00154b0c;
      goto LAB_00153e28;
    }
LAB_00154048:
    *puVar18 = (ulong)pFVar11;
LAB_0015404c:
    *(int *)(puVar10 + 6) = iVar19;
    pFVar17 = pFVar11;
    if ((param_1 != 0x7fff0001) || (iVar19 == 4)) goto LAB_00154b18;
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
        if (iVar6 != 4) goto LAB_001542f8;
        pFVar12 = fopen("/proc/self/maps","r");
        bVar5 = pFVar12 != (FILE *)0x0;
        if (pFVar12 != (FILE *)0x0) goto LAB_001540d8;
        iVar6 = *piVar9;
        if (iVar6 != 4) goto LAB_001542f8;
        pFVar12 = fopen("/proc/self/maps","r");
        bVar5 = pFVar12 != (FILE *)0x0;
        if (pFVar12 != (FILE *)0x0) goto LAB_001540d8;
        iVar6 = *piVar9;
        if (iVar6 != 4) goto LAB_001542f8;
        pFVar12 = fopen("/proc/self/maps","r");
        bVar5 = pFVar12 != (FILE *)0x0;
        if (pFVar12 != (FILE *)0x0) goto LAB_001540d8;
        iVar6 = *piVar9;
        if (iVar6 != 4) goto LAB_001542f8;
        pFVar12 = fopen("/proc/self/maps","r");
        bVar5 = pFVar12 != (FILE *)0x0;
        if (pFVar12 != (FILE *)0x0) goto LAB_001540d8;
        iVar6 = *piVar9;
        if (iVar6 != 4) goto LAB_001542f8;
        pFVar12 = fopen("/proc/self/maps","r");
        bVar5 = pFVar12 != (FILE *)0x0;
        if (pFVar12 != (FILE *)0x0) goto LAB_001540d8;
        iVar6 = *piVar9;
        if (iVar6 != 4) goto LAB_001542f8;
        pFVar12 = fopen("/proc/self/maps","r");
        bVar5 = pFVar12 != (FILE *)0x0;
        if (pFVar12 != (FILE *)0x0) goto LAB_001540d8;
        iVar6 = *piVar9;
        if (iVar6 != 4) goto LAB_001542f8;
        pFVar12 = fopen("/proc/self/maps","r");
        if (pFVar12 == (FILE *)0x0) goto LAB_001542f4;
LAB_001540e0:
        uVar8 = 0;
LAB_001540fc:
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
                goto LAB_001540fc;
              }
              iVar6 = 4;
            }
            iVar1 = 0;
            if (iVar7 != 0) {
              iVar1 = iVar6;
            }
            puVar10[5] = 0xffffffffffffffff;
            *(int *)((long)puVar10 + 0x34) = iVar1;
            puVar10[2] = &DAT_00132f2d;
            uVar8 = fclose(pFVar12);
            pFVar11 = (FILE *)(ulong)uVar8;
            goto LAB_00154340;
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
LAB_0015440c:
              uVar8 = (uint)((ulong)pFVar12 >> 0x3f);
              goto LAB_0015438c;
            }
            iVar6 = *piVar9;
            if (iVar6 == 4) {
              pFVar12 = (FILE *)syscall(0xde,0,pFVar11,3,0x22,0xffffffff,0);
              if (-1 < (long)pFVar12) goto LAB_0015440c;
              iVar6 = *piVar9;
              if (iVar6 == 4) {
                pFVar12 = (FILE *)syscall(0xde,0,pFVar11,3,0x22,0xffffffff,0);
                if (-1 < (long)pFVar12) {
LAB_001544dc:
                  uVar8 = (uint)((ulong)pFVar12 >> 0x3f);
                  goto LAB_0015438c;
                }
                iVar6 = *piVar9;
                if (iVar6 == 4) {
                  pFVar12 = (FILE *)syscall(0xde,0,pFVar11,3,0x22,0xffffffff,0);
                  if (-1 < (long)pFVar12) goto LAB_001544dc;
                  iVar6 = *piVar9;
                  if (iVar6 == 4) {
                    pFVar12 = (FILE *)syscall(0xde,0,pFVar11,3,0x22,0xffffffff,0);
                    if (-1 < (long)pFVar12) goto LAB_001544dc;
                    iVar6 = *piVar9;
                    if (iVar6 == 4) {
                      pFVar12 = (FILE *)syscall(0xde,0,pFVar11,3,0x22,0xffffffff,0);
                      if (-1 < (long)pFVar12) goto LAB_001544dc;
                      iVar6 = *piVar9;
                      if (iVar6 == 4) {
                        pFVar12 = (FILE *)syscall(0xde,0,pFVar11,3,0x22,0xffffffff,0);
                        goto LAB_0015440c;
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
LAB_0015438c:
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
                  if (*piVar9 != 4) goto LAB_0015455c;
                  uVar8 = getpid();
                  pFVar14 = (FILE *)syscall(0x10e,(ulong)uVar8,&local_2b8,1,&local_2c8,1,0);
                  if (-1 < (long)pFVar14) goto LAB_00154464;
                  if (*piVar9 == 4) {
                    uVar8 = getpid();
                    pFVar14 = (FILE *)syscall(0x10e,(ulong)uVar8,&local_2b8,1,&local_2c8,1,0);
                    if ((long)pFVar14 < 0) {
                      if (*piVar9 != 4) goto LAB_0015493c;
                      uVar8 = getpid();
                      pFVar14 = (FILE *)syscall(0x10e,(ulong)uVar8,&local_2b8,1,&local_2c8,1,0);
                      if ((long)pFVar14 < 0) {
                        if (*piVar9 != 4) goto LAB_0015493c;
                        uVar8 = getpid();
                        pFVar14 = (FILE *)syscall(0x10e,(ulong)uVar8,&local_2b8,1,&local_2c8,1,0);
                        if ((long)pFVar14 < 0) {
                          if (*piVar9 != 4) goto LAB_0015493c;
                          uVar8 = getpid();
                          pFVar14 = (FILE *)syscall(0x10e,(ulong)uVar8,&local_2b8,1,&local_2c8,1,0);
                          if ((long)pFVar14 < 0) {
                            if (*piVar9 != 4) goto LAB_0015493c;
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
LAB_0015493c:
                    uVar8 = 1;
                  }
                }
                else {
LAB_00154464:
                  uVar8 = (uint)((ulong)pFVar14 >> 0x3f);
                }
              }
              else {
LAB_0015455c:
                uVar8 = 1;
              }
              if (pFVar14 == pFVar11) goto LAB_00154568;
LAB_00154950:
              iVar6 = *piVar9;
              pcVar13 = "copy";
LAB_0015495c:
              puVar10[2] = pcVar13;
              if (uVar8 == 0) {
                iVar6 = 0;
              }
            }
            else {
              uVar8 = (uint)((ulong)pFVar14 >> 0x3f);
              if (pFVar14 != pFVar11) goto LAB_00154950;
LAB_00154568:
              *(undefined4 *)((long)pFVar12 + (param_4 - (long)pFVar16)) = *param_2;
              FUN_00193944(pFVar12,(long)&pFVar11->_flags + (long)&pFVar12->_flags);
              pFVar14 = (FILE *)syscall(0xe2,pFVar12,pFVar11,5);
              if (-1 < (long)pFVar14) {
                uVar8 = (uint)((ulong)pFVar14 >> 0x3f);
LAB_0015459c:
                if (pFVar14 != (FILE *)0x0) {
                  iVar6 = *piVar9;
                  if (uVar8 == 0) {
                    iVar6 = 0;
                  }
                  goto LAB_00154780;
                }
                pFVar14 = (FILE *)syscall(0xd8,pFVar12,pFVar11,pFVar11,3,pFVar16,0);
                if ((long)pFVar14 < 0) {
                  if (*piVar9 == 4) {
                    pFVar14 = (FILE *)syscall(0xd8,pFVar12,pFVar11,pFVar11,3,pFVar16,0);
                    if ((long)pFVar14 < 0) {
                      if (*piVar9 == 4) {
                        pFVar14 = (FILE *)syscall(0xd8,pFVar12,pFVar11,pFVar11,3,pFVar16,0);
                        if (-1 < (long)pFVar14) {
LAB_001546ac:
                          uVar8 = (uint)((ulong)pFVar14 >> 0x3f);
                          goto LAB_00154a88;
                        }
                        if (*piVar9 == 4) {
                          pFVar14 = (FILE *)syscall(0xd8,pFVar12,pFVar11,pFVar11,3,pFVar16,0);
                          if (-1 < (long)pFVar14) goto LAB_001546ac;
                          if (*piVar9 == 4) {
                            pFVar14 = (FILE *)syscall(0xd8,pFVar12,pFVar11,pFVar11,3,pFVar16,0);
                            if (-1 < (long)pFVar14) goto LAB_001546ac;
                            if (*piVar9 == 4) {
                              pFVar14 = (FILE *)syscall(0xd8,pFVar12,pFVar11,pFVar11,3,pFVar16,0);
                              if (-1 < (long)pFVar14) goto LAB_001546ac;
                              if (*piVar9 == 4) {
                                pFVar14 = (FILE *)syscall(0xd8,pFVar12,pFVar11,pFVar11,3,pFVar16,0);
                                if (-1 < (long)pFVar14) goto LAB_001546ac;
                                if (*piVar9 == 4) {
                                  pFVar14 = (FILE *)syscall(0xd8,pFVar12,pFVar11,pFVar11,3,pFVar16,0
                                                           );
                                  goto LAB_00154638;
                                }
                              }
                            }
                          }
                        }
                      }
                      uVar8 = 1;
                    }
                    else {
LAB_00154638:
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
LAB_00154a88:
                if (pFVar14 == pFVar16) {
                  pFVar11 = (FILE *)FUN_00193944(pFVar16,(long)&pFVar11->_flags +
                                                         (long)&pFVar16->_flags);
                  puVar10[6] = 0;
                  uVar4 = _DAT_0010f9d0;
                  puVar10[5] = _UNK_0010f9d8;
                  puVar10[4] = uVar4;
                  puVar10[2] = &DAT_00137150;
                  pFVar17 = (FILE *)0x4;
                  goto LAB_00154b14;
                }
                iVar6 = *piVar9;
                pcVar13 = "remap";
                goto LAB_0015495c;
              }
              iVar6 = *piVar9;
              if (iVar6 == 4) {
                pFVar14 = (FILE *)syscall(0xe2,pFVar12,pFVar11,5);
                if (-1 < (long)pFVar14) {
LAB_001545d4:
                  uVar8 = (uint)((ulong)pFVar14 >> 0x3f);
                  goto LAB_0015459c;
                }
                iVar6 = *piVar9;
                if (iVar6 == 4) {
                  pFVar14 = (FILE *)syscall(0xe2,pFVar12,pFVar11,5);
                  if (-1 < (long)pFVar14) {
LAB_00154664:
                    uVar8 = (uint)((ulong)pFVar14 >> 0x3f);
                    goto LAB_0015459c;
                  }
                  iVar6 = *piVar9;
                  if (iVar6 == 4) {
                    pFVar14 = (FILE *)syscall(0xe2,pFVar12,pFVar11,5);
                    if (-1 < (long)pFVar14) goto LAB_00154664;
                    iVar6 = *piVar9;
                    if (iVar6 == 4) {
                      pFVar14 = (FILE *)syscall(0xe2,pFVar12,pFVar11,5);
                      if (-1 < (long)pFVar14) goto LAB_00154664;
                      iVar6 = *piVar9;
                      if (iVar6 == 4) {
                        pFVar14 = (FILE *)syscall(0xe2,pFVar12,pFVar11,5);
                        if (-1 < (long)pFVar14) goto LAB_00154664;
                        iVar6 = *piVar9;
                        if (iVar6 == 4) {
                          pFVar14 = (FILE *)syscall(0xe2,pFVar12,pFVar11,5);
                          if (-1 < (long)pFVar14) goto LAB_00154664;
                          iVar6 = *piVar9;
                          if (iVar6 == 4) {
                            pFVar14 = (FILE *)syscall(0xe2,pFVar12,pFVar11,5);
                            goto LAB_001545d4;
                          }
                        }
                      }
                    }
                  }
                }
              }
LAB_00154780:
              puVar10[2] = &DAT_0013715b;
            }
            puVar10[5] = pFVar14;
            *(int *)((long)puVar10 + 0x34) = iVar6;
            pFVar11 = (FILE *)syscall(0xd7,pFVar12,pFVar11);
            goto LAB_00154340;
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
LAB_001540d8:
        if (bVar5) goto LAB_001540e0;
LAB_001542f4:
        iVar6 = *piVar9;
LAB_001542f8:
        puVar10[5] = 0xffffffffffffffff;
        puVar10[2] = &DAT_00132f2d;
        pFVar11 = pFVar12;
      }
      *(int *)((long)puVar10 + 0x34) = iVar6;
    }
LAB_00154340:
    if (iVar19 != 0) {
      iVar15 = iVar19;
    }
  }
LAB_00154b14:
  *piVar9 = iVar15;
LAB_00154b18:
  if (*(long *)(lVar3 + 0x28) == local_70) {
    return pFVar17;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(pFVar11);
}

/* ===== FUN_00154b50 @ 00154b50 ===== */

void FUN_00154b50(ulong *param_1,long param_2)

{
  long lVar1;
  int iVar2;
  ulong *puVar3;
  char local_60 [4];
  int local_5c;
  long local_58;
  long local_50;
  long local_48;
  long local_40;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  local_40 = 0;
  local_50 = 1;
  local_48 = 1;
  local_58 = 1;
  local_5c = 0;
  local_60[0] = '\x01';
  puVar3 = param_1;
  if (param_1 == (ulong *)0x0) goto LAB_00154cc8;
  if (((code *)param_1[2] != (code *)0x0) && (param_1[3] != 0)) {
    puVar3 = (ulong *)0x0;
    if ((0xffffffffffffed9f < param_2 - 0x1000U) ||
       ((*param_1 < 0x1000 ||
        (puVar3 = (ulong *)(*(code *)param_1[2])(param_1[1],param_2,&local_40,8), (int)puVar3 == 0))
       )) goto LAB_00154cc8;
    if (local_40 == *param_1 + 0x11c0a48) {
      iVar2 = (*(code *)param_1[2])(param_1[1],param_2 + 0x38,&local_48,8);
      puVar3 = (ulong *)0x0;
      if ((iVar2 != 0) && (local_48 == 0)) {
        iVar2 = (*(code *)param_1[2])(param_1[1],param_2 + 0x40,&local_5c,4);
        puVar3 = (ulong *)0x0;
        if ((iVar2 != 0) && (local_5c == -1)) {
          iVar2 = (*(code *)param_1[2])(param_1[1],param_2 + 0xa8,&local_50,8);
          puVar3 = (ulong *)0x0;
          if ((iVar2 != 0) && (local_50 == 0)) {
            iVar2 = (*(code *)param_1[2])(param_1[1],param_2 + 0x120,&local_58,8);
            puVar3 = (ulong *)0x0;
            if ((iVar2 != 0) &&
               ((local_58 == 0 &&
                (puVar3 = (ulong *)(*(code *)param_1[2])(param_1[1],param_2 + 0x1f0,local_60,1),
                (int)puVar3 != 0)))) {
              puVar3 = (ulong *)(ulong)(local_60[0] == '\0');
            }
          }
        }
      }
      goto LAB_00154cc8;
    }
  }
  puVar3 = (ulong *)0x0;
LAB_00154cc8:
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(puVar3);
}

/* ===== FUN_00154cf0 @ 00154cf0 ===== */

void FUN_00154cf0(undefined8 param_1)

{
  ulong uVar1;
  long lVar2;
  int iVar3;
  ulong local_30;
  long local_28;
  
  lVar2 = tpidr_el0;
  local_28 = *(long *)(lVar2 + 0x28);
  local_30 = 0;
  iVar3 = FUN_0014e7c4(param_1,param_1,&local_30,8);
  uVar1 = local_30;
  if (((local_30 & 7) != 0 || local_30 < 0x1000) || iVar3 == 0) {
    uVar1 = 0;
  }
  if (*(long *)(lVar2 + 0x28) == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar1);
}

/* ===== FUN_00154d5c @ 00154d5c ===== */

void FUN_00154d5c(ulong param_1,ulong param_2)

{
  long lVar1;
  ulong uVar2;
  int iVar3;
  ulong uVar4;
  undefined8 uVar5;
  ushort local_48 [2];
  int local_44;
  ulong local_40;
  long local_38;
  
  lVar1 = tpidr_el0;
  uVar4 = 0;
  local_38 = *(long *)(lVar1 + 0x28);
  local_44 = -1;
  local_48[0] = 0;
  if ((param_1 == 0) || (param_2 == 0)) goto LAB_00154e8c;
  local_40 = 0;
  uVar5 = FUN_0014e7c4(0,param_1 + 0x38,&local_40,8);
  uVar4 = local_40;
  if (((local_40 & 7) != 0 || local_40 < 0x1000) || (int)uVar5 == 0) {
    uVar4 = 0;
  }
  if (uVar4 == param_2) {
    iVar3 = FUN_0014e7c4(uVar5,param_1 + 0x40,&local_44,4);
    uVar4 = 0;
    if (((iVar3 == 0) || (local_44 < 0)) ||
       (uVar4 = FUN_0014e7c4(0,param_2 + 0x4e,local_48,2), (int)uVar4 == 0)) goto LAB_00154e8c;
    if (local_44 < (int)(uint)local_48[0]) {
      local_40 = 0;
      iVar3 = FUN_0014e7c4(uVar4,param_2 + 0x50,&local_40,8);
      uVar2 = local_40;
      uVar4 = 0;
      if (((iVar3 != 0) && (0xfff < local_40)) && ((local_40 & 7) == 0)) {
        local_40 = 0;
        iVar3 = FUN_0014e7c4(0,uVar2 + (long)local_44 * 8,&local_40,8);
        uVar4 = local_40;
        if (((local_40 & 7) != 0 || local_40 < 0x1000) || iVar3 == 0) {
          uVar4 = 0;
        }
        uVar4 = (ulong)(uVar4 == param_1);
      }
      goto LAB_00154e8c;
    }
  }
  uVar4 = 0;
LAB_00154e8c:
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar4);
}

/* ===== FUN_00154eb4 @ 00154eb4 ===== */

void FUN_00154eb4(ulong param_1,ulong param_2)

{
  ulong uVar1;
  long lVar2;
  ulong uVar3;
  bool bVar4;
  int iVar5;
  undefined8 uVar6;
  ulong uVar7;
  uint uVar8;
  long local_78;
  ulong local_70;
  long local_68;
  
  lVar2 = tpidr_el0;
  local_68 = *(long *)(lVar2 + 0x28);
  local_78 = 0;
  uVar7 = param_1;
  if (param_1 == 0) goto LAB_00154fd0;
  local_70 = 0;
  uVar6 = FUN_0014e7c4(param_1,param_1,&local_70,8);
  uVar7 = local_70;
  if (((local_70 & 7) != 0 || local_70 < 0x1000) || (int)uVar6 == 0) {
    uVar7 = 0;
  }
  if (uVar7 == DAT_0022d8e0 + 0x11c7b88U) {
    uVar7 = param_1;
    if (param_1 != param_2) {
      uVar8 = 0;
      do {
        local_70 = 0;
        iVar5 = FUN_0014e7c4(uVar6,uVar7 + 0x38,&local_70,8);
        uVar3 = local_70;
        bVar4 = (local_70 & 7) != 0;
        uVar1 = local_70;
        if ((iVar5 == 0 || local_70 < 0x1000) || bVar4) {
          uVar1 = 0;
        }
        uVar6 = FUN_00154d5c(uVar7,uVar1);
      } while (((((int)uVar6 != 0) && (uVar7 = uVar1, uVar8 < 0x1f)) &&
               ((iVar5 != 0 && 0xfff < uVar3) && !bVar4)) && (uVar8 = uVar8 + 1, uVar1 != param_2));
    }
    if (uVar7 == param_2) {
      uVar7 = FUN_0015516c(param_1,param_2);
      if ((int)uVar7 == 0) goto LAB_00154fd0;
      local_70 = 0;
      uVar6 = FUN_0014e7c4(uVar7,param_1 + 0x80,&local_70,8);
      uVar7 = local_70;
      if (((local_70 & 7) != 0 || local_70 < 0x1000) || (int)uVar6 == 0) {
        uVar7 = 0;
      }
      if (uVar7 == DAT_0022d8e0 + 0x11c7e60U) {
        uVar7 = FUN_0014e7c4(uVar6,DAT_0022d8e0 + 0x11c7e68,&local_78,8);
        if ((int)uVar7 != 0) {
          uVar7 = (ulong)(local_78 == DAT_0022d8e0 + 0x91bb7c);
        }
        goto LAB_00154fd0;
      }
    }
  }
  uVar7 = 0;
LAB_00154fd0:
  if (*(long *)(lVar2 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar7);
}

/* ===== FUN_00155084 @ 00155084 ===== */

void FUN_00155084(ulong param_1,ulong param_2)

{
  ulong uVar1;
  long lVar2;
  ulong uVar3;
  bool bVar4;
  int iVar5;
  ulong uVar6;
  uint uVar7;
  ulong local_60;
  long local_58;
  
  lVar2 = tpidr_el0;
  local_58 = *(long *)(lVar2 + 0x28);
  uVar6 = param_1;
  if ((param_1 != param_2) && (param_1 != 0)) {
    uVar7 = 0;
    do {
      local_60 = 0;
      iVar5 = FUN_0014e7c4(param_1,uVar6 + 0x38,&local_60,8);
      uVar3 = local_60;
      bVar4 = (local_60 & 7) != 0;
      uVar1 = local_60;
      if ((iVar5 == 0 || local_60 < 0x1000) || bVar4) {
        uVar1 = 0;
      }
      param_1 = FUN_00154d5c(uVar6,uVar1);
    } while ((((int)param_1 != 0) && (uVar6 = uVar1, uVar1 != param_2 && uVar7 < 0x1f)) &&
            (uVar7 = uVar7 + 1, (iVar5 != 0 && 0xfff < uVar3) && !bVar4));
  }
  if (*(long *)(lVar2 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar6 == param_2);
}

/* ===== FUN_0015516c @ 0015516c ===== */

void FUN_0015516c(ulong param_1,ulong param_2)

{
  long lVar1;
  bool bVar2;
  int iVar3;
  ulong uVar4;
  uint uVar5;
  float fVar6;
  float fVar7;
  char local_6c [4];
  ulong local_68;
  float local_60;
  float local_5c;
  long local_58;
  
  lVar1 = tpidr_el0;
  local_58 = *(long *)(lVar1 + 0x28);
  if (param_1 != 0) {
    uVar5 = 0;
    uVar4 = param_1;
    while( true ) {
      local_6c[0] = '\0';
      iVar3 = FUN_0014e7c4(param_1,uVar4 + 0xc,local_6c,1);
      param_1 = 0;
      if (((iVar3 == 0) || (local_6c[0] == '\0')) ||
         (param_1 = FUN_0014e7c4(0,uVar4 + 0x20,&local_60,8), (int)param_1 == 0)) goto LAB_00155290;
      param_1 = 0;
      fVar6 = ABS(local_60);
      if ((fVar6 == INFINITY) || (NAN(fVar6))) goto LAB_00155290;
      param_1 = 0;
      fVar7 = ABS(local_5c);
      if (((fVar7 == INFINITY) || ((NAN(fVar7) || (fVar6 < 8192.0 == NAN(fVar6))))) ||
         (fVar7 < 8192.0 == NAN(fVar7))) goto LAB_00155290;
      if (uVar4 == param_2) break;
      local_68 = 0;
      iVar3 = FUN_0014e7c4(0,uVar4 + 0x38,&local_68,8);
      param_1 = 0;
      bVar2 = (local_68 & 7) != 0;
      uVar4 = local_68;
      if ((iVar3 == 0 || local_68 < 0x1000) || bVar2) {
        uVar4 = 0;
      }
      if ((0x1e < uVar5) || (uVar5 = uVar5 + 1, (iVar3 == 0 || local_68 < 0x1000) || bVar2))
      goto LAB_00155290;
    }
    param_1 = 1;
  }
LAB_00155290:
  if (*(long *)(lVar1 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(param_1);
}

/* ===== FUN_001552c0 @ 001552c0 ===== */

void FUN_001552c0(void)

{
  long lVar1;
  undefined8 local_38;
  undefined8 local_30;
  long local_28;
  
  lVar1 = tpidr_el0;
  local_28 = *(long *)(lVar1 + 0x28);
  local_38 = 0;
  local_30 = 0;
  dl_iterate_phdr(FUN_0015532c,&local_38);
  if ((int)local_38 == 1) {
    DAT_0022f1d8 = local_30;
  }
  if (*(long *)(lVar1 + 0x28) == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0015532c @ 0015532c ===== */

undefined8 FUN_0015532c(long *param_1,undefined8 param_2,int *param_3)

{
  long lVar1;
  int iVar2;
  char *pcVar3;
  long lVar4;
  long lVar5;
  undefined1 auStack_68 [8];
  long local_60;
  long local_48;
  
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  if (((char *)param_1[1] != (char *)0x0) &&
     (pcVar3 = strrchr((char *)param_1[1],0x2f), pcVar3 != (char *)0x0)) {
    iVar2 = strcmp(pcVar3 + 1,"libNexusDelivery.so");
    if (iVar2 == 0) {
      *param_3 = *param_3 + 1;
      lVar4 = dlopen(param_1[1],6);
      if (lVar4 != 0) {
        lVar5 = dlsym(lVar4,"nexus_protected_plus_active");
        if (((lVar5 != 0) && (iVar2 = dladdr(lVar5,auStack_68), iVar2 != 0)) &&
           (local_60 == *param_1)) {
          *(long *)(param_3 + 2) = lVar5;
        }
        dlclose(lVar4);
      }
    }
  }
  if (*(long *)(lVar1 + 0x28) == local_48) {
    return 0;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0015540c @ 0015540c ===== */

void FUN_0015540c(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
                 undefined8 param_5,undefined8 param_6,undefined8 param_7,undefined8 param_8,
                 undefined8 param_9)

{
  long lVar1;
  long lVar2;
  undefined8 local_90;
  undefined8 uStack_88;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined1 *local_70;
  undefined1 **ppuStack_68;
  undefined8 *puStack_60;
  undefined8 uStack_58;
  
  ppuStack_68 = &local_70;
  lVar1 = tpidr_el0;
  lVar2 = *(long *)(lVar1 + 0x28);
  puStack_60 = &local_90;
  uStack_58 = 0xffffff80ffffffe0;
  local_90 = param_6;
  uStack_88 = param_7;
  local_80 = param_8;
  uStack_78 = param_9;
  local_70 = (undefined1 *)register0x00000008;
  __vsnprintf_chk(param_2,param_4,0,param_3,param_5,&local_70,param_8,param_9,param_1);
  if (*(long *)(lVar1 + 0x28) == lVar2) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_001556ec @ 001556ec ===== */

void FUN_001556ec(undefined8 param_1,long param_2,undefined8 param_3,long param_4)

{
  ulong uVar1;
  ulong uVar2;
  long lVar3;
  bool bVar4;
  bool bVar5;
  undefined8 uVar6;
  ulong uVar7;
  long lVar8;
  ulong local_50;
  long local_48;
  
  lVar3 = tpidr_el0;
  local_48 = *(long *)(lVar3 + 0x28);
  local_50 = 0;
  uVar6 = FUN_0014e7c4(param_1,DAT_0022d8e0 + 0x12ff038,&local_50,8);
  bVar4 = (int)uVar6 != 0;
  bVar5 = (local_50 & 7) != 0;
  uVar1 = local_50;
  if ((!bVar4 || local_50 < 0x1000) || bVar5) {
    uVar1 = 0;
  }
  if ((bVar4 && 0xfff < local_50) && !bVar5) {
    local_50 = 0;
    uVar7 = FUN_0014e7c4(uVar6,uVar1,&local_50,8);
    uVar2 = local_50;
    if (((local_50 & 7) != 0 || local_50 < 0x1000) || (int)uVar7 == 0) {
      uVar2 = 0;
    }
    if (uVar2 == DAT_0022d8e0 + 0x11baa20U) {
      if ((((param_4 == 8) && (uVar1 + 0x88 == param_2)) ||
          ((uVar7 = 0, param_4 == 1 && (DAT_0022d8e0 + 0x12f03e4 == param_2)))) &&
         (uVar7 = FUN_00155848(uVar7,param_2,param_4), (int)uVar7 != 0)) {
        lVar8 = FUN_00153cbc(DAT_001a7c38,param_3,param_4,param_2);
        uVar7 = (ulong)(lVar8 == param_4);
      }
      goto LAB_0015581c;
    }
  }
  uVar7 = 0;
LAB_0015581c:
  if (*(long *)(lVar3 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar7);
}

/* ===== FUN_00155848 @ 00155848 ===== */

bool FUN_00155848(ulong param_1,ulong param_2,ulong param_3)

{
  long lVar1;
  bool bVar2;
  int iVar3;
  uint uVar4;
  FILE *__stream;
  char *pcVar5;
  ulong local_260;
  ulong local_258;
  int local_250 [2];
  char acStack_248 [512];
  long local_48;
  
  lVar1 = tpidr_el0;
  bVar2 = false;
  local_48 = *(long *)(lVar1 + 0x28);
  if ((param_2 - 1 < ~param_3) && (param_3 != 0)) {
    __stream = fopen("/proc/self/maps","r");
    if (__stream == (FILE *)0x0) {
      bVar2 = false;
      param_1 = 0;
    }
    else {
      pcVar5 = fgets(acStack_248,0x200,__stream);
      if (pcVar5 != (char *)0x0) {
        do {
          iVar3 = sscanf(acStack_248,"%lx-%lx %4s",&local_258,&local_260,local_250);
          if (((iVar3 == 3) && (local_258 <= param_2)) && (param_3 + param_2 <= local_260)) {
            bVar2 = local_250[0] == 0x702d7772;
            goto LAB_00155940;
          }
          pcVar5 = fgets(acStack_248,0x200,__stream);
        } while (pcVar5 != (char *)0x0);
      }
      bVar2 = false;
LAB_00155940:
      uVar4 = fclose(__stream);
      param_1 = (ulong)uVar4;
    }
  }
  if (*(long *)(lVar1 + 0x28) == local_48) {
    return bVar2;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(param_1);
}

/* ===== FUN_00155980 @ 00155980 ===== */

void FUN_00155980(long *param_1,long param_2)

{
  long lVar1;
  int iVar2;
  ulong uVar3;
  long local_48;
  long local_40;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  local_48 = 0;
  local_40 = 0;
  iVar2 = (*(code *)param_1[2])(param_1[1],*param_1 + 0x12ff038,&local_40,8);
  uVar3 = 0;
  if (((iVar2 != 0) && (local_40 == param_2)) &&
     (uVar3 = (*(code *)param_1[2])(param_1[1],param_2,&local_48,8), (int)uVar3 != 0)) {
    uVar3 = (ulong)(local_48 == *param_1 + 0x11baa20);
  }
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar3);
}

/* ===== FUN_00155a40 @ 00155a40 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00155a40(int param_1,uint *param_2)

{
  long lVar1;
  uint uVar2;
  long lVar3;
  bool bVar4;
  int iVar5;
  int iVar6;
  __uid_t _Var7;
  undefined8 uVar8;
  long lVar9;
  int *piVar10;
  uint uVar11;
  ulong uVar12;
  byte bVar13;
  uint uVar14;
  byte bVar17;
  byte bVar18;
  byte bVar19;
  undefined1 auVar15 [16];
  undefined1 auVar16 [16];
  stat sStack_f0;
  uint local_60;
  uint local_5c;
  long local_58;
  
  lVar3 = tpidr_el0;
  local_58 = *(long *)(lVar3 + 0x28);
  uVar8 = 0;
  if ((-1 < param_1) && (param_2 != (uint *)0x0)) {
    iVar5 = __openat_2(param_1,"bsd-debug-quality.bin",0x88000);
    if (iVar5 < 0) {
      piVar10 = (int *)__errno();
      if (*piVar10 == 2) {
        uVar8 = 1;
        param_2[0] = 0;
        param_2[1] = 0;
        param_2[2] = 0;
        goto LAB_00155cc0;
      }
    }
    else {
      iVar6 = fstat(iVar5,&sStack_f0);
      if ((iVar6 == 0) && (((uint)sStack_f0.st_nlink & 0xf000) == 0x8000)) {
        _Var7 = getuid();
        uVar12 = 0;
        bVar4 = false;
        if ((((sStack_f0.st_mode == _Var7) && (sStack_f0.st_nlink._4_4_ == 1)) &&
            (((uint)sStack_f0.st_nlink & 0x3f) == 0)) && (sStack_f0.st_size == 0x18)) {
          uVar12 = 0;
          do {
            lVar9 = __read_chk(iVar5,(long)sStack_f0.__unused + uVar12 + 8,0x18 - uVar12,
                               0xffffffffffffffff);
            if ((lVar9 < 0) && (piVar10 = (int *)__errno(), *piVar10 == 4)) {
              iVar6 = 2;
            }
            else {
              lVar1 = 0;
              if (0 < lVar9) {
                lVar1 = lVar9;
              }
              iVar6 = 3;
              if (0 < lVar9) {
                iVar6 = 0;
              }
              uVar12 = lVar1 + uVar12;
            }
            bVar4 = true;
          } while ((iVar6 != 3) && (uVar12 < 0x18));
        }
      }
      else {
        bVar4 = false;
        uVar12 = 0;
      }
      iVar5 = close(iVar5);
      uVar8 = 0;
      if ((!bVar4) || ((iVar5 != 0 || (uVar12 != 0x18)))) goto LAB_00155cc0;
      if (((int)sStack_f0.__unused[1] == 0x51445342) && (sStack_f0.__unused[1]._4_4_ == 1)) {
        lVar9 = 0;
        uVar11 = 0xffffffff;
        do {
          lVar1 = lVar9 + 8;
          lVar9 = lVar9 + 1;
          uVar14 = uVar11 ^ *(byte *)((long)sStack_f0.__unused + lVar1);
          uVar2 = -(uVar14 & 1) & 0xedb88320 ^ uVar14 >> 1;
          bVar13 = (byte)uVar14;
          auVar15[0] = bVar13 & (byte)_DAT_0010f9c0;
          bVar17 = (byte)(uVar11 >> 8);
          auVar15[1] = bVar17 & (byte)((ulong)_DAT_0010f9c0 >> 8);
          bVar18 = (byte)(uVar11 >> 0x10);
          auVar15[2] = bVar18 & (byte)((ulong)_DAT_0010f9c0 >> 0x10);
          bVar19 = (byte)(uVar11 >> 0x18);
          auVar15[3] = bVar19 & (byte)((ulong)_DAT_0010f9c0 >> 0x18);
          auVar15[4] = bVar13 & (byte)((ulong)_DAT_0010f9c0 >> 0x20);
          auVar15[5] = bVar17 & (byte)((ulong)_DAT_0010f9c0 >> 0x28);
          auVar15[6] = bVar18 & (byte)((ulong)_DAT_0010f9c0 >> 0x30);
          auVar15[7] = bVar19 & (byte)((ulong)_DAT_0010f9c0 >> 0x38);
          auVar15[8] = bVar13 & (byte)_UNK_0010f9c8;
          auVar15[9] = bVar17 & (byte)((ulong)_UNK_0010f9c8 >> 8);
          auVar15[10] = bVar18 & (byte)((ulong)_UNK_0010f9c8 >> 0x10);
          auVar15[0xb] = bVar19 & (byte)((ulong)_UNK_0010f9c8 >> 0x18);
          auVar15[0xc] = bVar13 & (byte)((ulong)_UNK_0010f9c8 >> 0x20);
          auVar15[0xd] = bVar17 & (byte)((ulong)_UNK_0010f9c8 >> 0x28);
          auVar15[0xe] = bVar18 & (byte)((ulong)_UNK_0010f9c8 >> 0x30);
          auVar15[0xf] = bVar19 & (byte)((ulong)_UNK_0010f9c8 >> 0x38);
          uVar11 = (int)(uVar14 << 0x1e) >> 0x1f & 0xedb88320U ^ uVar2 >> 1;
          auVar15 = NEON_cmeq(auVar15,0,2);
          auVar16[0] = (byte)_DAT_0010fac0 & ~auVar15[0];
          auVar16[1] = (byte)((ulong)_DAT_0010fac0 >> 8) & ~auVar15[1];
          auVar16[2] = (byte)((ulong)_DAT_0010fac0 >> 0x10) & ~auVar15[2];
          auVar16[3] = (byte)((ulong)_DAT_0010fac0 >> 0x18) & ~auVar15[3];
          auVar16[4] = (byte)((ulong)_DAT_0010fac0 >> 0x20) & ~auVar15[4];
          auVar16[5] = (byte)((ulong)_DAT_0010fac0 >> 0x28) & ~auVar15[5];
          auVar16[6] = (byte)((ulong)_DAT_0010fac0 >> 0x30) & ~auVar15[6];
          auVar16[7] = (byte)((ulong)_DAT_0010fac0 >> 0x38) & ~auVar15[7];
          auVar16[8] = (byte)_UNK_0010fac8 & ~auVar15[8];
          auVar16[9] = (byte)((ulong)_UNK_0010fac8 >> 8) & ~auVar15[9];
          auVar16[10] = (byte)((ulong)_UNK_0010fac8 >> 0x10) & ~auVar15[10];
          auVar16[0xb] = (byte)((ulong)_UNK_0010fac8 >> 0x18) & ~auVar15[0xb];
          auVar16[0xc] = (byte)((ulong)_UNK_0010fac8 >> 0x20) & ~auVar15[0xc];
          auVar16[0xd] = (byte)((ulong)_UNK_0010fac8 >> 0x28) & ~auVar15[0xd];
          auVar16[0xe] = (byte)((ulong)_UNK_0010fac8 >> 0x30) & ~auVar15[0xe];
          auVar16[0xf] = (byte)((ulong)_UNK_0010fac8 >> 0x38) & ~auVar15[0xf];
          auVar15 = NEON_ext(auVar16,auVar16,8,1);
          uVar14 = CONCAT13(auVar16[3] ^ auVar15[3],
                            CONCAT12(auVar16[2] ^ auVar15[2],
                                     CONCAT11(auVar16[1] ^ auVar15[1],auVar16[0] ^ auVar15[0])));
          uVar11 = uVar14 ^ (int)(uVar2 << 0x1a) >> 0x1f & 0x76dc4190U ^
                   (int)(uVar11 << 0x1a) >> 0x1f & 0xedb88320U ^ uVar11 >> 6 ^
                   (uint)(CONCAT17(auVar16[7] ^ auVar15[7],
                                   CONCAT16(auVar16[6] ^ auVar15[6],
                                            CONCAT15(auVar16[5] ^ auVar15[5],
                                                     CONCAT14(auVar16[4] ^ auVar15[4],uVar14)))) >>
                         0x20);
        } while (lVar9 != 0x14);
        if (local_5c == ~uVar11) {
          uVar8 = 0;
          if ((((uint)sStack_f0.__unused[2] < 2) && (sStack_f0.__unused[2]._4_4_ < 4)) &&
             (local_60 < 4)) {
            uVar8 = 1;
            *param_2 = (uint)sStack_f0.__unused[2];
            param_2[1] = sStack_f0.__unused[2]._4_4_;
            param_2[2] = local_60;
          }
          goto LAB_00155cc0;
        }
      }
    }
    uVar8 = 0;
  }
LAB_00155cc0:
  if (*(long *)(lVar3 + 0x28) != local_58) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar8);
  }
  return;
}

/* ===== FUN_00155cf0 @ 00155cf0 ===== */

void FUN_00155cf0(long *param_1,undefined8 param_2,undefined4 *param_3)

{
  long lVar1;
  int iVar2;
  undefined4 uVar3;
  char *__s;
  code *pcVar4;
  undefined1 auStack_68 [8];
  long local_60;
  long local_48;
  
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  __s = (char *)param_1[1];
  if ((__s == (char *)0x0) || (__s = strrchr(__s,0x2f), __s == (char *)0x0)) goto LAB_00155d74;
  iVar2 = strcmp(__s + 1,"libNexusDelivery.so");
  if (iVar2 == 0) {
    iVar2 = param_3[2];
    param_3[2] = iVar2 + 1;
    if (iVar2 != 0) {
      if (-1 < (int)param_3[1]) {
        close(param_3[1]);
      }
      __s = (char *)0x1;
      param_3[1] = 0xffffffff;
      goto LAB_00155d74;
    }
    __s = (char *)dlopen(param_1[1],6);
    if (__s == (char *)0x0) goto LAB_00155d74;
    pcVar4 = (code *)dlsym(__s,"nexus_protected_process_memory");
    if (((pcVar4 != (code *)0x0) && (iVar2 = dladdr(pcVar4,auStack_68), iVar2 != 0)) &&
       (local_60 == *param_1)) {
      uVar3 = (*pcVar4)(*param_3);
      param_3[1] = uVar3;
    }
    dlclose(__s);
  }
  __s = (char *)0x0;
LAB_00155d74:
  if (*(long *)(lVar1 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(__s);
}

/* ===== FUN_00155dfc @ 00155dfc ===== */

void FUN_00155dfc(void)

{
  FUN_00169558(&DAT_0022f040);
  return;
}

/* ===== FUN_00155e94 @ 00155e94 ===== */

int FUN_00155e94(char *param_1)

{
  long lVar1;
  long lVar2;
  uint uVar3;
  int __fd;
  __uid_t _Var4;
  int iVar5;
  int __fd_00;
  char *pcVar6;
  size_t sVar7;
  long lVar8;
  char *__s1;
  stat asStack_20c8 [29];
  char acStack_1048 [4096];
  long local_48;
  
  lVar2 = tpidr_el0;
  local_48 = *(long *)(lVar2 + 0x28);
  pcVar6 = (char *)__strchr_chk("bsd.suitcase.nexusv2",0x2f,0x15);
  if (pcVar6 == (char *)0x0) {
    uVar3 = strncmp(param_1,"/data/user/",0xb);
    pcVar6 = (char *)(ulong)uVar3;
    if (uVar3 == 0) {
      lVar1 = 0;
      do {
        lVar8 = lVar1;
        lVar1 = lVar8 + 1;
      } while ((byte)param_1[lVar8 + 0xb] - 0x30 < 10);
      __fd_00 = -1;
      if (((lVar1 == 1) || ((byte)param_1[lVar8 + 0xb] != 0x2f)) ||
         ((0xc < lVar8 + 0xbU && (param_1[0xb] == '0')))) goto LAB_00155edc;
      __s1 = param_1 + 0xb + lVar1;
    }
    else {
      uVar3 = strncmp(param_1,"/data/data/",0xb);
      pcVar6 = (char *)(ulong)uVar3;
      if (uVar3 != 0) goto LAB_00155ed8;
      __s1 = param_1 + 0xb;
    }
    sVar7 = strlen(param_1);
    uVar3 = strncmp(__s1,"bsd.suitcase.nexusv2",0x14);
    pcVar6 = (char *)(ulong)uVar3;
    if (uVar3 == 0) {
      uVar3 = strcmp(__s1 + 0x14,"/files");
      pcVar6 = (char *)(ulong)uVar3;
      __fd_00 = -1;
      if (((uVar3 != 0) || (0xfff < sVar7)) ||
         (pcVar6 = realpath(param_1,acStack_1048), pcVar6 == (char *)0x0)) goto LAB_00155edc;
      uVar3 = strcmp(param_1,acStack_1048);
      pcVar6 = (char *)(ulong)uVar3;
      if (uVar3 != 0) goto LAB_00155edc;
      __memcpy_chk(asStack_20c8[0].__unused + 1,param_1,sVar7 - 6,0x1000);
      *(undefined1 *)((long)asStack_20c8[0].__unused + sVar7 + 2) = 0;
      pcVar6 = (char *)__open_2(asStack_20c8[0].__unused + 1,0x8c000);
      __fd = (int)pcVar6;
      if (-1 < __fd) {
        _Var4 = geteuid();
        iVar5 = fstat(__fd,asStack_20c8);
        __fd_00 = __fd;
        if (((iVar5 == 0) && (((uint)asStack_20c8[0].st_nlink & 0xf000) == 0x4000)) &&
           (asStack_20c8[0].st_mode == _Var4)) {
          __fd_00 = __openat_2((ulong)pcVar6 & 0xffffffff,"files",0x8c000);
          uVar3 = close(__fd);
          pcVar6 = (char *)(ulong)uVar3;
          if (__fd_00 < 0) goto LAB_00155ed8;
          uVar3 = fstat(__fd_00,asStack_20c8);
          pcVar6 = (char *)(ulong)uVar3;
          if (((uVar3 == 0) && (((uint)asStack_20c8[0].st_nlink & 0xf000) == 0x4000)) &&
             (asStack_20c8[0].st_mode == _Var4)) goto LAB_00155edc;
        }
        uVar3 = close(__fd_00);
        pcVar6 = (char *)(ulong)uVar3;
      }
    }
  }
LAB_00155ed8:
  __fd_00 = -1;
LAB_00155edc:
  if (*(long *)(lVar2 + 0x28) != local_48) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(pcVar6);
  }
  return __fd_00;
}

/* ===== FUN_00156c48 @ 00156c48 ===== */

void FUN_00156c48(undefined8 param_1,undefined8 param_2)

{
  dlclose(param_2);
  return;
}

/* ===== FUN_00156d68 @ 00156d68 ===== */

undefined8 FUN_00156d68(undefined8 *param_1,undefined8 param_2,undefined8 *param_3)

{
  long lVar1;
  int iVar2;
  ulong uVar3;
  size_t sVar4;
  char *__s;
  char *__s2;
  undefined1 auStack_2ef [23];
  undefined1 auStack_2d8 [65];
  char acStack_297 [181];
  char acStack_1e2 [10];
  undefined5 local_1d8;
  undefined3 uStack_1d3;
  undefined5 uStack_1d0;
  undefined1 auStack_1cb [307];
  char acStack_98 [64];
  long local_58;
  
  lVar1 = tpidr_el0;
  local_58 = *(long *)(lVar1 + 0x28);
  __s = (char *)param_1[1];
  iVar2 = FUN_001574ec(__s,acStack_98);
  if (((((iVar2 != 0) &&
        (uVar3 = readlink(acStack_98,(char *)&local_1d8,0x13f), 0xfffffffffffffec1 < uVar3 - 0x13f))
       && (*(undefined1 *)((long)&local_1d8 + uVar3) = 0, 0x17 < uVar3)) &&
      ((CONCAT35(uStack_1d3,local_1d8) == 0x6e3a64666d656d2f &&
        CONCAT53(uStack_1d0,uStack_1d3) == 0x2d737578656e3a64 &&
       (iVar2 = strcmp(acStack_1e2 + uVar3," (deleted)"), iVar2 == 0)))) && (uVar3 - 0x17 < 0x100))
  {
    __memcpy_chk(auStack_2d8,auStack_1cb,uVar3 - 0x17,0x100);
    auStack_2ef[uVar3] = 0;
    uVar3 = __strlen_chk(auStack_2d8,0x100);
    if (0x41 < uVar3) {
      __s2 = (char *)*param_3;
      iVar2 = strcmp(acStack_297,__s2);
      if (iVar2 == 0) {
        if (*(uint *)(param_3 + 2) < 2) {
          *(uint *)(param_3 + 2) = *(uint *)(param_3 + 2) + 1;
        }
        iVar2 = FUN_00169ae0(__s,"bsd.suitcase.nexusv2",__s2,param_3[1]);
        if ((iVar2 != 0) && (sVar4 = strlen(__s), sVar4 < 0x1000)) {
          sVar4 = strlen(__s);
          memcpy(param_3 + 4,__s,sVar4 + 1);
          param_3[3] = *param_1;
        }
      }
    }
  }
  if (*(long *)(lVar1 + 0x28) == local_58) {
    return 0;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00156f1c @ 00156f1c ===== */

void FUN_00156f1c(undefined8 *param_1,ulong *param_2,ulong param_3)

{
  long *plVar1;
  undefined *puVar2;
  long lVar3;
  bool bVar4;
  uint __fd;
  int iVar5;
  __uid_t _Var6;
  uint uVar7;
  int *piVar8;
  undefined8 *puVar9;
  long lVar10;
  int iVar11;
  ulong uVar12;
  ulong uVar13;
  undefined1 local_4200 [136];
  ulong local_4178;
  uint local_4170;
  int local_416c;
  __uid_t local_4168;
  ulong local_4150;
  long local_4128;
  long local_4120;
  undefined1 auStack_4100 [112];
  ulong local_4090;
  ulong uStack_4088;
  ulong local_4080;
  ulong uStack_4078;
  undefined1 auStack_4070 [16384];
  long local_70;
  
  lVar3 = tpidr_el0;
  local_70 = *(long *)(lVar3 + 0x28);
  __fd = FUN_00157740(param_1 + 4,*param_1,param_1[1]);
  if (-1 < (int)__fd) {
    iVar5 = fstat(__fd,(stat *)(local_4200 + 0x80));
    if ((((iVar5 == 0) && ((local_4170 & 0xf000) == 0x8000)) &&
        (_Var6 = getuid(), local_4168 == _Var6)) && ((local_416c == 0 && (local_4150 == param_3))))
    {
      FUN_00191314(auStack_4100);
      if (param_3 == 0) {
        uVar13 = 0;
      }
      else {
        piVar8 = (int *)__errno();
        puVar9 = (undefined8 *)__emutls_get_address(&DAT_001a7c68);
        uVar13 = 0;
        puVar2 = &DAT_00133b55;
        if (__fd != 0x7fff0001) {
          puVar2 = &DAT_00134ec4;
        }
        plVar1 = puVar9 + 4;
        do {
          while( true ) {
            uVar12 = param_3 - uVar13;
            iVar5 = *piVar8;
            puVar9[5] = 0;
            puVar9[6] = 0;
            *puVar9 = puVar2;
            puVar9[1] = &DAT_00133b50;
            if (0x3fff < uVar12) {
              uVar12 = 0x4000;
            }
            *plVar1 = 0;
            puVar9[2] = &DAT_00137150;
            puVar9[3] = uVar12;
            local_4200._0_8_ = auStack_4070;
            local_4200._8_8_ = uVar12;
            local_4090 = uVar13;
            uStack_4088 = uVar12;
            if (__fd != 0x7fff0001) break;
            uVar7 = getpid();
            lVar10 = syscall(0x10e,(ulong)uVar7,local_4200,1,&local_4090,1,0);
            if (-1 < lVar10) goto LAB_00157104;
LAB_00157138:
            iVar11 = *piVar8;
            if (iVar11 != 4) goto LAB_001573dc;
            if (__fd != 0x7fff0001) {
              lVar10 = syscall(0x43,(ulong)__fd,auStack_4070,uVar12,uVar13);
              if (lVar10 < 0) goto LAB_001571a4;
              goto LAB_00157104;
            }
            uVar7 = getpid();
            lVar10 = syscall(0x10e,(ulong)uVar7,local_4200,1,&local_4090,1,0);
            if (-1 < lVar10) goto LAB_00157104;
LAB_001571a4:
            iVar11 = *piVar8;
            if (iVar11 != 4) {
LAB_001573dc:
              *plVar1 = lVar10;
              goto LAB_00157048;
            }
            if (__fd == 0x7fff0001) {
              uVar7 = getpid();
              lVar10 = syscall(0x10e,(ulong)uVar7,local_4200,1,&local_4090,1,0);
            }
            else {
              lVar10 = syscall(0x43,(ulong)__fd,auStack_4070,uVar12,uVar13);
            }
            if (-1 < lVar10) goto LAB_00157104;
            iVar11 = *piVar8;
            if (iVar11 != 4) goto LAB_001573dc;
            if (__fd == 0x7fff0001) {
              uVar7 = getpid();
              lVar10 = syscall(0x10e,(ulong)uVar7,local_4200,1,&local_4090,1,0);
            }
            else {
              lVar10 = syscall(0x43,(ulong)__fd,auStack_4070,uVar12,uVar13);
            }
            if (-1 < lVar10) goto LAB_00157104;
            iVar11 = *piVar8;
            if (iVar11 != 4) goto LAB_001573dc;
            if (__fd == 0x7fff0001) {
              uVar7 = getpid();
              lVar10 = syscall(0x10e,(ulong)uVar7,local_4200,1,&local_4090,1,0);
            }
            else {
              lVar10 = syscall(0x43,(ulong)__fd,auStack_4070,uVar12,uVar13);
            }
            if (-1 < lVar10) goto LAB_00157104;
            iVar11 = *piVar8;
            if (iVar11 != 4) goto LAB_001573dc;
            if (__fd == 0x7fff0001) {
              uVar7 = getpid();
              lVar10 = syscall(0x10e,(ulong)uVar7,local_4200,1,&local_4090,1,0);
            }
            else {
              lVar10 = syscall(0x43,(ulong)__fd,auStack_4070,uVar12,uVar13);
            }
            if (-1 < lVar10) goto LAB_00157104;
            iVar11 = *piVar8;
            if (iVar11 != 4) goto LAB_001573dc;
            if (__fd == 0x7fff0001) {
              uVar7 = getpid();
              lVar10 = syscall(0x10e,(ulong)uVar7,local_4200,1,&local_4090,1,0);
            }
            else {
              lVar10 = syscall(0x43,(ulong)__fd,auStack_4070,uVar12,uVar13);
            }
            if (-1 < lVar10) goto LAB_00157104;
            iVar11 = *piVar8;
            if (iVar11 != 4) goto LAB_001573dc;
            if (__fd == 0x7fff0001) {
              uVar7 = getpid();
              lVar10 = syscall(0x10e,(ulong)uVar7,local_4200,1,&local_4090,1,0);
            }
            else {
              lVar10 = syscall(0x43,(ulong)__fd,auStack_4070,uVar12,uVar13);
            }
            *plVar1 = lVar10;
            if (lVar10 < 0) goto LAB_00157044;
LAB_00157110:
            *(undefined4 *)(puVar9 + 6) = 0;
            *piVar8 = iVar5;
            if (lVar10 == 0) goto LAB_001574d0;
            uVar13 = lVar10 + uVar13;
            FUN_00191338(auStack_4100,auStack_4070);
            if (param_3 <= uVar13) goto LAB_00157414;
          }
          lVar10 = syscall(0x43,(ulong)__fd,auStack_4070,uVar12,uVar13);
          if (lVar10 < 0) goto LAB_00157138;
LAB_00157104:
          *plVar1 = lVar10;
          if (-1 < lVar10) goto LAB_00157110;
LAB_00157044:
          iVar11 = *piVar8;
LAB_00157048:
          *(int *)(puVar9 + 6) = iVar11;
          if (*piVar8 != 4) goto LAB_001574d0;
        } while (uVar13 < param_3);
      }
LAB_00157414:
      iVar5 = fstat(__fd,(stat *)local_4200);
      if (((iVar5 == 0) && (local_4200._128_8_ == local_4200._0_8_)) &&
         ((local_4178 == local_4200._8_8_ &&
          ((((local_4200._48_8_ == param_3 && (local_4128 == local_4200._88_8_)) &&
            (local_4120 == local_4200._96_8_)) && (local_4170 == local_4200._16_4_)))))) {
        close(__fd);
        FUN_00191678(auStack_4100,&local_4090);
        bVar4 = false;
        if ((local_4200._20_4_ == 0) && (uVar13 == param_3)) {
          bVar4 = ((local_4090 == *param_2 && uStack_4088 == param_2[1]) && local_4080 == param_2[2]
                  ) && uStack_4078 == param_2[3];
        }
        goto LAB_00157010;
      }
LAB_001574d0:
      close(__fd);
      FUN_00191678(auStack_4100,&local_4090);
    }
    else {
      close(__fd);
    }
  }
  bVar4 = false;
LAB_00157010:
  if (*(long *)(lVar3 + 0x28) == local_70) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(bVar4);
}

/* ===== FUN_001574ec @ 001574ec ===== */

/* WARNING: Removing unreachable block (ram,0x00157624) */

void FUN_001574ec(DIR *param_1,char *param_2)

{
  long lVar1;
  bool bVar2;
  bool bVar3;
  bool bVar4;
  int iVar5;
  size_t sVar6;
  DIR *__dirp;
  dirent *pdVar7;
  ssize_t sVar8;
  uint uVar9;
  long lVar10;
  char *__s;
  __ino_t _Var11;
  __ino_t _Var12;
  __dev_t _Var13;
  __dev_t _Var14;
  stat local_270 [3];
  char acStack_b0 [64];
  long local_70;
  
  lVar1 = tpidr_el0;
  local_70 = *(long *)(lVar1 + 0x28);
  __dirp = param_1;
  if (param_1 != (DIR *)0x0) {
    iVar5 = strncmp((char *)param_1,"/proc/self/fd/",0xe);
    if ((iVar5 == 0) && (uVar9 = (uint)(byte)param_1[0xe], param_1[0xe] != (DIR)0x0)) {
      lVar10 = 0xf;
      do {
        if (uVar9 - 0x3a < 0xfffffff6) goto LAB_001576dc;
        uVar9 = (uint)(byte)param_1[lVar10];
        lVar10 = lVar10 + 1;
      } while (uVar9 != 0);
      sVar6 = strlen((char *)param_1);
      if (sVar6 < 0x40) {
        strcpy(param_2,(char *)param_1);
        __dirp = (DIR *)0x1;
        goto LAB_001576e0;
      }
    }
    else {
      iVar5 = strncmp((char *)param_1,"/memfd:nexus-",0xd);
      if ((iVar5 == 0) && (sVar6 = strlen((char *)param_1), sVar6 < 0x140)) {
        __dirp = opendir("/proc/self/fd");
        if (__dirp != (DIR *)0x0) {
          _Var13 = 0;
          _Var11 = 0;
          bVar2 = false;
          bVar3 = false;
          do {
            do {
              do {
                pdVar7 = readdir(__dirp);
                while( true ) {
                  if (pdVar7 == (dirent *)0x0) goto LAB_00157710;
                  __s = pdVar7->d_name;
                  if ((*__s != '.') && (sVar6 = strlen(__s), sVar6 < 0x15)) break;
                  pdVar7 = readdir(__dirp);
                }
                FUN_0015540c(acStack_b0,0x40,0x40,"/proc/self/fd/%s",__s);
                sVar8 = readlink(acStack_b0,(char *)(local_270[0].__unused + 1),0x13f);
              } while (sVar8 - 0x13fU < 0xfffffffffffffec2);
              *(undefined1 *)((long)local_270[0].__unused + sVar8 + 8) = 0;
              iVar5 = strcmp((char *)(local_270[0].__unused + 1),(char *)param_1);
            } while (iVar5 != 0);
            iVar5 = stat(acStack_b0,local_270);
            if (iVar5 == 0) {
              _Var12 = local_270[0].st_ino;
              _Var14 = local_270[0].st_dev;
              if (bVar2) {
                bVar2 = true;
                iVar5 = 6;
                if (local_270[0].st_dev == _Var13) {
                  _Var12 = _Var11;
                  _Var14 = _Var13;
                  bVar4 = true;
                  if (local_270[0].st_ino == _Var11) goto LAB_00157678;
                }
                else {
                  bVar4 = true;
                }
              }
              else {
LAB_00157678:
                strcpy(param_2,acStack_b0);
                iVar5 = 0;
                bVar2 = true;
                _Var11 = _Var12;
                _Var13 = _Var14;
                bVar4 = bVar3;
              }
            }
            else {
              iVar5 = 6;
              bVar4 = true;
            }
            bVar3 = bVar4;
          } while (iVar5 != 6);
LAB_00157710:
          closedir(__dirp);
          __dirp = (DIR *)(ulong)(bVar2 && !bVar3);
        }
        goto LAB_001576e0;
      }
    }
LAB_001576dc:
    __dirp = (DIR *)0x0;
  }
LAB_001576e0:
  if (*(long *)(lVar1 + 0x28) != local_70) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(__dirp);
  }
  return;
}

/* ===== FUN_00157740 @ 00157740 ===== */

void FUN_00157740(undefined8 param_1,char *param_2,byte *param_3)

{
  byte bVar1;
  long lVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  __uid_t _Var6;
  ulong uVar7;
  size_t sVar8;
  int *piVar9;
  long lVar10;
  byte *pbVar11;
  undefined1 auStack_2ef [15];
  char *local_2e0;
  undefined4 local_2d8;
  undefined4 uStack_2d4;
  long lStack_2d0;
  long local_2c8;
  long lStack_2c0;
  long local_2b8;
  long lStack_2b0;
  long local_2a8;
  long lStack_2a0;
  char local_298;
  char acStack_297 [181];
  char acStack_1e2 [10];
  stat local_1d8 [2];
  char acStack_98 [14];
  char acStack_8a [50];
  long local_58;
  
  lVar2 = tpidr_el0;
  local_58 = *(long *)(lVar2 + 0x28);
  iVar3 = FUN_001574ec(param_1,acStack_98);
  if ((((iVar3 != 0) &&
       (uVar7 = readlink(acStack_98,(char *)local_1d8,0x13f), 0xfffffffffffffec1 < uVar7 - 0x13f))
      && (*(undefined1 *)((long)local_1d8[0].__unused + (uVar7 - 0x78)) = 0, 0x17 < uVar7)) &&
     (((CONCAT35(local_1d8[0].st_dev._5_3_,(undefined5)local_1d8[0].st_dev) == 0x6e3a64666d656d2f &&
        CONCAT53((undefined5)local_1d8[0].st_ino,local_1d8[0].st_dev._5_3_) == 0x2d737578656e3a64 &&
       (iVar3 = strcmp(acStack_1e2 + uVar7," (deleted)"), iVar3 == 0)) && (uVar7 - 0x17 < 0x100))))
  {
    __memcpy_chk(&local_2d8,(undefined1 *)((long)&local_1d8[0].st_ino + 5),uVar7 - 0x17,0x100);
    auStack_2ef[uVar7] = 0;
    if (local_2d8 == 0x69726373 && CONCAT31((undefined3)uStack_2d4,local_2d8._3_1_) == 0x2d747069) {
      iVar3 = -1;
      if ((param_2 == (char *)0x0) || (param_3 == (byte *)0x0)) goto LAB_00157810;
      sVar8 = strlen((char *)param_3);
      if (((sVar8 == 0x40) && (sVar8 = strlen(param_2), 2 < sVar8)) &&
         (((iVar3 = strcmp(param_2 + (sVar8 - 3),".js"), iVar3 == 0 ||
           ((3 < sVar8 && (iVar3 = strcmp(param_2 + (sVar8 - 4),".ngs"), iVar3 == 0)))) &&
          (lVar10 = __strlen_chk(&local_2d8,0x100), lVar10 == 0x27)))) {
        lVar10 = 0;
        do {
          uVar4 = (uint)*(byte *)((long)&uStack_2d4 + lVar10 + 3);
          if (9 < uVar4 - 0x30 && 5 < uVar4 - 0x61) goto LAB_0015780c;
          lVar10 = lVar10 + 1;
        } while (lVar10 != 0x20);
        bVar1 = *param_3;
        if (bVar1 != 0) {
          pbVar11 = param_3 + 1;
          do {
            if ((9 < bVar1 - 0x30) && (5 < bVar1 - 0x61)) goto LAB_0015780c;
            bVar1 = *pbVar11;
            pbVar11 = pbVar11 + 1;
          } while (bVar1 != 0);
        }
        goto LAB_00157944;
      }
    }
    else {
      uVar7 = __strlen_chk(&local_2d8,0x100);
      iVar3 = -1;
      if ((uVar7 < 0x42) || (local_298 != '-')) goto LAB_00157810;
      lVar10 = 0;
      do {
        if (9 < *(byte *)((long)&local_2d8 + lVar10) - 0x30 &&
            5 < *(byte *)((long)&local_2d8 + lVar10) - 0x61) goto LAB_0015780c;
        lVar10 = lVar10 + 1;
      } while (lVar10 != 0x40);
      if (((param_2 == (char *)0x0) || (iVar3 = strcmp(param_2,acStack_297), iVar3 == 0)) &&
         ((param_3 == (byte *)0x0 ||
          ((sVar8 = strlen((char *)param_3), sVar8 == 0x40 &&
           (((((((*(long *)param_3 == CONCAT44(uStack_2d4,local_2d8) &&
                 *(long *)(param_3 + 8) == lStack_2d0) && *(long *)(param_3 + 0x10) == local_2c8) &&
               *(long *)(param_3 + 0x18) == lStack_2c0) && *(long *)(param_3 + 0x20) == local_2b8)
             && *(long *)(param_3 + 0x28) == lStack_2b0) && *(long *)(param_3 + 0x30) == local_2a8)
            && *(long *)(param_3 + 0x38) == lStack_2a0)))))) {
LAB_00157944:
        iVar3 = FUN_001574ec(param_1,acStack_98);
        if (iVar3 != 0) {
          local_2e0 = (char *)0x0;
          piVar9 = (int *)__errno();
          *piVar9 = 0;
          lVar10 = strtol(acStack_8a,&local_2e0,10);
          iVar3 = -1;
          if ((((*piVar9 != 0) || (local_2e0 == (char *)0x0)) || (*local_2e0 != '\0')) ||
             ((lVar10 < 0 || (0x7fffffff < lVar10)))) goto LAB_00157810;
          iVar3 = fcntl((int)lVar10,0x406,0);
          if (-1 < iVar3) {
            uVar4 = fcntl(iVar3,0x40a);
            iVar5 = fstat(iVar3,local_1d8);
            if (((iVar5 == 0) && (((uint)local_1d8[0].st_nlink & 0xf000) == 0x8000)) &&
               ((_Var6 = getuid(), local_1d8[0].st_mode == _Var6 &&
                (((local_1d8[0].st_nlink._4_4_ == 0 && (-1 < (int)uVar4)) && ((uVar4 & 0xf) == 0xf))
                )))) goto LAB_00157810;
            close(iVar3);
          }
        }
      }
    }
  }
LAB_0015780c:
  iVar3 = -1;
LAB_00157810:
  if (*(long *)(lVar2 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar3);
}

/* ===== FUN_00157b0c @ 00157b0c ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

long FUN_00157b0c(long *param_1)

{
  int iVar1;
  undefined4 uVar2;
  long lVar3;
  ulong uVar4;
  char cVar5;
  ulong uVar6;
  long lVar7;
  undefined4 *puVar8;
  undefined4 *puVar9;
  undefined8 uVar10;
  undefined8 uVar11;
  undefined8 uVar12;
  undefined4 local_4868;
  undefined8 local_4864;
  undefined8 uStack_485c;
  undefined4 local_4854;
  uint uStack_4850;
  undefined4 local_484c;
  undefined4 auStack_4828 [4600];
  long local_48;
  
  lVar3 = tpidr_el0;
  local_48 = *(long *)(lVar3 + 0x28);
  uVar6 = FUN_00193f80(1,&DAT_0022f500);
  if ((uVar6 & 1) == 0) {
    if (DAT_00233ee4 != 0) {
      uVar6 = (ulong)DAT_00233ecc;
      uVar4 = uVar6 * 9;
      if (uVar4 < 0x1201) {
        local_4868 = 1;
        local_484c = 1;
        uStack_485c = _DAT_00233ed8;
        local_4864 = _DAT_00233ed0;
        local_4854 = DAT_00233ee0;
        uStack_4850 = DAT_00233ecc;
        if (DAT_00233ecc != 0) {
          puVar8 = auStack_4828;
          puVar9 = &DAT_0022f6ec;
          do {
            uVar10 = *(undefined8 *)(puVar9 + -8);
            uVar12 = *(undefined8 *)(puVar9 + -2);
            uVar11 = *(undefined8 *)(puVar9 + -4);
            uVar6 = uVar6 - 1;
            uVar2 = *puVar9;
            *(undefined8 *)(puVar8 + -6) = *(undefined8 *)(puVar9 + -6);
            *(undefined8 *)(puVar8 + -8) = uVar10;
            *(undefined8 *)(puVar8 + -2) = uVar12;
            *(undefined8 *)(puVar8 + -4) = uVar11;
            *puVar8 = uVar2;
            puVar8 = puVar8 + 9;
            puVar9 = puVar9 + 9;
          } while (uVar6 != 0);
        }
        DAT_0022f500 = 0;
        iVar1 = (int)uVar4 + 8;
        lVar7 = (**(code **)(*param_1 + 0x598))(param_1,iVar1);
        if (lVar7 == 0) goto LAB_00157c54;
        (**(code **)(*param_1 + 0x698))(param_1,lVar7,0,iVar1,&local_4868);
        cVar5 = (**(code **)(*param_1 + 0x720))(param_1);
        if (cVar5 == '\0') goto LAB_00157c54;
        (**(code **)(*param_1 + 0x88))(param_1);
        (**(code **)(*param_1 + 0xb8))(param_1,lVar7);
        goto LAB_00157c44;
      }
    }
    lVar7 = 0;
    DAT_0022f500 = 0;
  }
  else {
LAB_00157c44:
    lVar7 = 0;
  }
LAB_00157c54:
  if (*(long *)(lVar3 + 0x28) == local_48) {
    return lVar7;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00157c88 @ 00157c88 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_00157c88(undefined8 param_1,undefined8 param_2,uint param_3,int param_4)

{
  uint uVar1;
  ulong uVar2;
  ushort uVar3;
  undefined1 auVar4 [16];
  undefined1 auVar5 [16];
  
  if ((int)param_3 < 1) {
    return 4;
  }
  auVar5._8_8_ = _UNK_0010fb68;
  auVar5._0_8_ = _DAT_0010fb60;
  auVar4._0_4_ = param_3 + _DAT_0010fa70;
  auVar4._4_4_ = param_3 + _UNK_0010fa74;
  auVar4._8_4_ = param_3 + _UNK_0010fa78;
  auVar4._12_4_ = param_3 + _UNK_0010fa7c;
  auVar5 = NEON_cmhi(auVar5,auVar4,4);
  uVar3 = NEON_umaxv(CONCAT26(auVar5._12_2_,
                              CONCAT24(auVar5._8_2_,CONCAT22(auVar5._4_2_,auVar5._0_2_))),2);
  if ((((((uVar3 & 1) == 0) && (2 < param_3 - 0x20000)) && (0xa7 < param_3)) &&
      (((param_3 & 0xffffffc0) != 0x26000 && ((param_3 & 0xfffffe00) != 0x29000)))) &&
     ((0x90 < param_3 - 0x2e101 &&
      (((param_3 & 0xfffffffc) != 0x2e000 && (0x15 < param_3 - 0x2e010)))))) {
    return 4;
  }
  uVar2 = FUN_00193f80(1,&DAT_0022f500);
  if ((uVar2 & 1) != 0) {
    return 3;
  }
  if (param_3 == 0x82) {
    DAT_0022f6c8 = 0;
    DAT_0022f6c4 = 0;
  }
  else {
    if (DAT_00233ee4 == 0) {
      DAT_0022f500 = 0;
      return 3;
    }
    if (DAT_00233ed0 != param_4) {
      DAT_0022f500 = 0;
      return 3;
    }
    if (0x1f < DAT_0022f6c8) {
      DAT_0022f500 = 0;
      return 3;
    }
  }
  uVar1 = DAT_0022f6c8 + DAT_0022f6c4 & 0x1f;
  (&DAT_0022f6a4)[uVar1] = 0;
  (&DAT_0022f684)[uVar1] = param_3 == 0x82;
  (&DAT_0022f504)[uVar1] = param_3;
  (&DAT_0022f584)[uVar1] = param_4;
  (&DAT_0022f604)[uVar1] = 0;
  DAT_0022f500 = 0;
  DAT_0022f6c8 = DAT_0022f6c8 + 1;
  return 2;
}

/* ===== FUN_00157e00 @ 00157e00 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8
FUN_00157e00(undefined8 param_1,undefined8 param_2,uint param_3,undefined4 param_4,int param_5)

{
  uint uVar1;
  ulong uVar2;
  undefined8 uVar3;
  ushort uVar4;
  undefined1 auVar5 [16];
  undefined1 auVar6 [16];
  
  if (0 < (int)param_3) {
    auVar6._8_8_ = _UNK_0010fb68;
    auVar6._0_8_ = _DAT_0010fb60;
    auVar5._0_4_ = param_3 + _DAT_0010fa70;
    auVar5._4_4_ = param_3 + _UNK_0010fa74;
    auVar5._8_4_ = param_3 + _UNK_0010fa78;
    auVar5._12_4_ = param_3 + _UNK_0010fa7c;
    auVar6 = NEON_cmhi(auVar6,auVar5,4);
    uVar4 = NEON_umaxv(CONCAT26(auVar6._12_2_,
                                CONCAT24(auVar6._8_2_,CONCAT22(auVar6._4_2_,auVar6._0_2_))),2);
    if (((((uVar4 & 1) != 0) || (param_3 - 0x20000 < 3)) || (param_3 < 0xa8)) ||
       ((((param_3 & 0xffffffc0) == 0x26000 || ((param_3 & 0xfffffe00) == 0x29000)) ||
        ((param_3 - 0x2e101 < 0x91 ||
         (((param_3 & 0xfffffffc) == 0x2e000 || (param_3 - 0x2e010 < 0x16)))))))) {
      uVar2 = FUN_00193f80(1,&DAT_0022f500);
      if ((uVar2 & 1) == 0) {
        uVar3 = 3;
        if (((DAT_00233ee4 != 0) && (DAT_00233ed0 == param_5)) && (DAT_0022f6c8 < 0x20)) {
          uVar3 = 2;
          uVar1 = DAT_0022f6c4 + DAT_0022f6c8;
          DAT_0022f6c8 = DAT_0022f6c8 + 1;
          uVar1 = uVar1 & 0x1f;
          (&DAT_0022f6a4)[uVar1] = 1;
          (&DAT_0022f684)[uVar1] = 0;
          (&DAT_0022f504)[uVar1] = param_3;
          (&DAT_0022f584)[uVar1] = param_5;
          (&DAT_0022f604)[uVar1] = param_4;
        }
        DAT_0022f500 = 0;
        return uVar3;
      }
      return 3;
    }
  }
  return 4;
}

/* ===== FUN_00157f58 @ 00157f58 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

long FUN_00157f58(long *param_1)

{
  long lVar1;
  char cVar2;
  int iVar3;
  ulong uVar4;
  long lVar5;
  long lVar6;
  undefined1 *puVar7;
  uint uVar8;
  undefined1 auStack_4060 [16];
  undefined1 auStack_4050 [16];
  undefined1 auStack_4040 [16];
  undefined1 auStack_4030 [16];
  undefined1 auStack_4020 [16];
  undefined1 auStack_4010 [16];
  undefined1 auStack_4000 [32];
  undefined1 auStack_3fe0 [4];
  undefined8 local_3fdc;
  undefined8 uStack_3fd4;
  undefined1 local_3fcc [109];
  undefined1 auStack_3f5f [16119];
  long local_68;
  
  lVar1 = tpidr_el0;
  local_68 = *(long *)(lVar1 + 0x28);
  memset(auStack_3fe0,0,0x3f74);
  uVar4 = FUN_00193f80(1,&DAT_00233eec);
  if ((uVar4 & 1) == 0) {
    uVar8 = (uint)(_DAT_00233ef8 >> 0x20);
    uStack_3fd4 = _DAT_00233ef8;
    local_3fdc = _DAT_00233ef0;
    if (uVar8 != 0) {
      __memcpy_chk(local_3fcc,&DAT_00233f00,(ulong)uVar8 * 0x270,0x3f60);
    }
    DAT_00233eec = 0;
    lVar5 = (**(code **)(*param_1 + 0x30))(param_1,"java/lang/String");
    if (lVar5 != 0) {
      lVar6 = (**(code **)(*param_1 + 0x560))(param_1,uStack_3fd4._4_4_ * 5 + 5,lVar5,0);
      (**(code **)(*param_1 + 0xb8))(param_1,lVar5);
      if (lVar6 == 0) goto LAB_001584c8;
      FUN_0015540c(auStack_4000,0x20,0x20,&DAT_00133bc0,local_3fdc & 0xffffffff);
      FUN_0015540c(auStack_4010,0x10,0x10,&DAT_00133bc0,local_3fdc._4_4_);
      FUN_0015540c(auStack_4020,0x10,0x10,&DAT_00133bc0,uStack_3fd4 & 0xffffffff);
      FUN_0015540c(auStack_4030,0x10,0x10,&DAT_00133bc0,uStack_3fd4._4_4_);
      iVar3 = (**(code **)(*param_1 + 0x98))(param_1,0x10);
      if (iVar3 == 0) {
        lVar5 = (**(code **)(*param_1 + 0x538))(param_1,"NXOD1");
        if (lVar5 != 0) {
          (**(code **)(*param_1 + 0x570))(param_1,lVar6,0,lVar5);
          (**(code **)(*param_1 + 0xb8))(param_1,lVar5);
          lVar5 = (**(code **)(*param_1 + 0x538))(param_1,auStack_4000);
          if (lVar5 != 0) {
            (**(code **)(*param_1 + 0x570))(param_1,lVar6,1,lVar5);
            (**(code **)(*param_1 + 0xb8))(param_1,lVar5);
            lVar5 = (**(code **)(*param_1 + 0x538))(param_1,auStack_4010);
            if (lVar5 != 0) {
              (**(code **)(*param_1 + 0x570))(param_1,lVar6,2,lVar5);
              (**(code **)(*param_1 + 0xb8))(param_1,lVar5);
              lVar5 = (**(code **)(*param_1 + 0x538))(param_1,auStack_4020);
              if (lVar5 != 0) {
                (**(code **)(*param_1 + 0x570))(param_1,lVar6,3,lVar5);
                (**(code **)(*param_1 + 0xb8))(param_1,lVar5);
                lVar5 = (**(code **)(*param_1 + 0x538))(param_1,auStack_4030);
                if (lVar5 != 0) {
                  (**(code **)(*param_1 + 0x570))(param_1,lVar6,4,lVar5);
                  (**(code **)(*param_1 + 0xb8))(param_1,lVar5);
                  if (uStack_3fd4._4_4_ != 0) {
                    uVar4 = 0;
                    puVar7 = auStack_3f5f;
                    iVar3 = 9;
                    do {
                      FUN_0015540c(auStack_4040,0x10,0x10,&DAT_00133bc0,
                                   *(undefined4 *)(puVar7 + -0x6d));
                      FUN_0015540c(auStack_4050,0x10,0x10,&DAT_00133bc0,
                                   *(undefined4 *)(puVar7 + -0x69));
                      FUN_0015540c(auStack_4060,0x10,0x10,&DAT_00133bc0,
                                   *(undefined4 *)(puVar7 + -0x65));
                      lVar5 = (**(code **)(*param_1 + 0x538))(param_1,auStack_4040);
                      if (lVar5 == 0) goto LAB_00158474;
                      (**(code **)(*param_1 + 0x570))(param_1,lVar6,iVar3 + -4,lVar5);
                      (**(code **)(*param_1 + 0xb8))(param_1,lVar5);
                      lVar5 = (**(code **)(*param_1 + 0x538))(param_1,auStack_4050);
                      if (lVar5 == 0) goto LAB_00158474;
                      (**(code **)(*param_1 + 0x570))(param_1,lVar6,iVar3 + -3,lVar5);
                      (**(code **)(*param_1 + 0xb8))(param_1,lVar5);
                      lVar5 = (**(code **)(*param_1 + 0x538))(param_1,puVar7 + -0x61);
                      if (lVar5 == 0) goto LAB_00158474;
                      (**(code **)(*param_1 + 0x570))(param_1,lVar6,iVar3 + -2,lVar5);
                      (**(code **)(*param_1 + 0xb8))(param_1,lVar5);
                      lVar5 = (**(code **)(*param_1 + 0x538))(param_1,puVar7);
                      if (lVar5 == 0) goto LAB_00158474;
                      (**(code **)(*param_1 + 0x570))(param_1,lVar6,iVar3 + -1,lVar5);
                      (**(code **)(*param_1 + 0xb8))(param_1,lVar5);
                      lVar5 = (**(code **)(*param_1 + 0x538))(param_1,auStack_4060);
                      if (lVar5 == 0) goto LAB_00158474;
                      (**(code **)(*param_1 + 0x570))(param_1,lVar6,iVar3,lVar5);
                      (**(code **)(*param_1 + 0xb8))(param_1,lVar5);
                      uVar4 = uVar4 + 1;
                      iVar3 = iVar3 + 5;
                      puVar7 = puVar7 + 0x270;
                    } while (uVar4 < uStack_3fd4 >> 0x20);
                  }
                  (**(code **)(*param_1 + 0xa0))(param_1,0);
                  goto LAB_001584c8;
                }
              }
            }
          }
        }
LAB_00158474:
        cVar2 = (**(code **)(*param_1 + 0x720))(param_1);
        if (cVar2 != '\0') {
          (**(code **)(*param_1 + 0x88))(param_1);
        }
        (**(code **)(*param_1 + 0xa0))(param_1,0);
      }
      else {
        (**(code **)(*param_1 + 0x88))(param_1);
      }
      (**(code **)(*param_1 + 0xb8))(param_1,lVar6);
    }
  }
  lVar6 = 0;
LAB_001584c8:
  if (*(long *)(lVar1 + 0x28) == local_68) {
    return lVar6;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00158500 @ 00158500 ===== */

long FUN_00158500(long *param_1)

{
  undefined1 (*pauVar1) [12];
  int *piVar2;
  int *piVar3;
  int *piVar4;
  int *piVar5;
  int iVar6;
  undefined4 uVar7;
  long lVar8;
  undefined1 auVar9 [16];
  int *piVar10;
  undefined4 uVar11;
  char cVar12;
  int *__dest;
  ulong uVar13;
  long lVar14;
  long lVar15;
  undefined *puVar16;
  uint uVar17;
  ulong uVar18;
  undefined1 *puVar19;
  ulong uVar20;
  ulong uVar21;
  char *pcVar22;
  undefined1 auVar23 [16];
  undefined1 auStack_614 [1124];
  undefined8 local_1b0 [16];
  undefined1 auStack_12c [20];
  undefined4 local_118 [6];
  char acStack_100 [32];
  int *local_e0;
  int *piStack_d8;
  int *local_d0;
  int *local_c8;
  int *local_c0;
  undefined1 local_b4 [4];
  undefined8 local_b0;
  undefined8 uStack_a8;
  undefined8 uStack_a0;
  undefined8 uStack_98;
  undefined8 uStack_90;
  undefined4 uStack_88;
  uint local_84;
  int local_80;
  long local_78;
  
  lVar8 = tpidr_el0;
  local_78 = *(long *)(lVar8 + 0x28);
  __dest = (int *)malloc(0x24a68);
  if (__dest != (int *)0x0) {
    uVar13 = FUN_00193f80(1,&DAT_00237e60);
    if ((uVar13 & 1) == 0) {
      memcpy(__dest,&DAT_00237e64,0x24a68);
      uVar11 = DAT_0025c8cc;
      DAT_00237e60 = 0;
      if ((((*__dest == 1) && (__dest[1] == 0x24a68)) && (uVar17 = __dest[0xc], uVar17 < 0x101)) &&
         (lVar14 = (**(code **)(*param_1 + 0x30))(param_1,"java/lang/String"), lVar14 != 0)) {
        uVar13 = (ulong)uVar17 * 0xb + 0x1f;
        lVar15 = (**(code **)(*param_1 + 0x560))(param_1,uVar13 & 0xffffffff,lVar14,0);
        (**(code **)(*param_1 + 0xb8))(param_1,lVar14);
        if (lVar15 != 0) {
          uVar21 = 0;
          do {
            if (((*__dest != 1) || (__dest[1] != 0x24a68)) ||
               ((uVar17 = __dest[0xc], 0x100 < uVar17 || ((ulong)uVar17 * 0xb + 0x1f <= uVar21)))) {
LAB_00158918:
              free(__dest);
              cVar12 = (**(code **)(*param_1 + 0x720))(param_1);
              if (cVar12 != '\0') {
                (**(code **)(*param_1 + 0x88))(param_1);
              }
              (**(code **)(*param_1 + 0xb8))(param_1,lVar15);
              goto LAB_00158968;
            }
            if (uVar21 == 0) {
              pcVar22 = "NXOS1";
            }
            else {
              if (uVar21 < 0x1f) {
                pauVar1 = (undefined1 (*) [12])(__dest + 2);
                uStack_a0._0_4_ = (undefined4)((ulong)*(undefined8 *)(__dest + 4) >> 0x20);
                local_b0._0_4_ = uVar11;
                auVar23._12_4_ = (undefined4)uStack_a0;
                auVar23._0_12_ = *pauVar1;
                auVar9._12_4_ = (undefined4)uStack_a0;
                auVar9._0_12_ = *pauVar1;
                auVar23 = NEON_ext(auVar23,auVar9,0xc,1);
                local_80 = __dest[0xd];
                uStack_98._4_4_ = (undefined4)*(undefined8 *)(__dest + 8);
                uStack_90._0_4_ = (undefined4)((ulong)*(undefined8 *)(__dest + 8) >> 0x20);
                uStack_a0._4_4_ = (uint)*(undefined8 *)(__dest + 6);
                uStack_98._0_4_ = (undefined4)((ulong)*(undefined8 *)(__dest + 6) >> 0x20);
                uStack_90._4_4_ = (undefined4)*(undefined8 *)(__dest + 10);
                uStack_88 = (undefined4)((ulong)*(undefined8 *)(__dest + 10) >> 0x20);
                local_b0._4_4_ = SUB124(*pauVar1,4);
                uStack_a8._4_4_ = auVar23._4_4_;
                uStack_a8._0_4_ = SUB124(*pauVar1,8);
                puVar19 = local_b4;
                local_84 = uVar17;
                if (uVar21 < 0xe) {
LAB_001587f4:
                  uVar7 = *(undefined4 *)(puVar19 + uVar21 * 4);
                  puVar16 = &DAT_00133bc0;
                }
                else {
                  if (0x12 < uVar21) {
                    if (0x19 < uVar21) {
                      pcVar22 = (char *)local_1b0[uVar21];
                      local_e0 = __dest + 0x1a;
                      piStack_d8 = __dest + 0x3a;
                      local_c0 = __dest + 0x82;
                      local_d0 = __dest + 0x52;
                      local_c8 = __dest + 0x6a;
                      local_b0 = (int *)CONCAT44(local_b0._4_4_,uVar11);
                      uStack_a8 = (int *)CONCAT44(uStack_a8._4_4_,(undefined4)uStack_a8);
                      uStack_a0 = (int *)CONCAT44(uStack_a0._4_4_,(undefined4)uStack_a0);
                      uStack_98 = (int *)CONCAT44(uStack_98._4_4_,(undefined4)uStack_98);
                      uStack_90 = (int *)CONCAT44(uStack_90._4_4_,(undefined4)uStack_90);
                      goto joined_r0x00158914;
                    }
                    piStack_d8 = *(int **)(__dest + 0x15);
                    local_e0 = *(int **)(__dest + 0x13);
                    local_d0 = *(int **)(__dest + 0x17);
                    local_c8 = (int *)CONCAT44(local_c8._4_4_,__dest[0x19]);
                    puVar19 = auStack_12c;
                    goto LAB_001587f4;
                  }
                  piStack_d8 = *(int **)(__dest + 0x10);
                  local_e0 = *(int **)(__dest + 0xe);
                  puVar16 = &DAT_0013c694;
                  local_d0 = (int *)CONCAT44(local_d0._4_4_,__dest[0x12]);
                  uVar7 = local_118[uVar21];
                }
                pcVar22 = acStack_100;
                FUN_0015540c(acStack_100,0x20,0x20,puVar16,uVar7);
              }
              else {
                uVar20 = (uVar21 - 0x1f) / 0xb;
                uVar18 = (uVar21 - 0x1f) % 0xb;
                uVar17 = (uint)uVar18;
                if (uVar17 < 6) {
                  if ((uVar17 == 4) || (uVar17 == 2)) {
                    puVar16 = &DAT_0013c694;
                    lVar14 = uVar20 * 0x92 + 0x9e;
                    if (uVar17 != 2) {
                      lVar14 = uVar20 * 0x92 + 0x9f;
                    }
                    iVar6 = __dest[lVar14];
                    piVar10 = local_b0;
                  }
                  else {
                    puVar16 = &DAT_00133bc0;
                    piVar10 = *(int **)(__dest + uVar20 * 0x92 + 0x9a);
                    uStack_a8._0_4_ = 0;
                    uStack_a8._4_4_ = __dest[uVar20 * 0x92 + 0x9c];
                    uStack_a8 = (int *)((ulong)(uint)__dest[uVar20 * 0x92 + 0x9c] << 0x20);
                    uStack_a0._0_4_ = 0;
                    uStack_a0._4_4_ = __dest[uVar20 * 0x92 + 0x9d];
                    uStack_a0 = (int *)((ulong)(uint)__dest[uVar20 * 0x92 + 0x9d] << 0x20);
                    local_b0._0_4_ = SUB84(piVar10,0);
                    local_b0._4_4_ = (undefined4)((ulong)piVar10 >> 0x20);
                    iVar6 = *(int *)((long)&local_b0 + uVar18 * 4);
                  }
                  pcVar22 = acStack_100;
                  local_b0 = piVar10;
                  FUN_0015540c(acStack_100,0x20,0x20,puVar16,iVar6);
                  goto LAB_00158868;
                }
                piVar10 = __dest + uVar20 * 0x92 + 0xa0;
                piVar2 = __dest + uVar20 * 0x92 + 0xb0;
                piVar3 = __dest + uVar20 * 0x92 + 200;
                piVar4 = __dest + uVar20 * 0x92 + 0xfc;
                piVar5 = __dest + uVar20 * 0x92 + 0x114;
                local_b0._0_4_ = SUB84(piVar10,0);
                local_b0._4_4_ = (undefined4)((ulong)piVar10 >> 0x20);
                uStack_a8._0_4_ = SUB84(piVar2,0);
                uStack_a8._4_4_ = (uint)((ulong)piVar2 >> 0x20);
                uStack_a0._0_4_ = SUB84(piVar3,0);
                uStack_a0._4_4_ = (uint)((ulong)piVar3 >> 0x20);
                uStack_98._0_4_ = SUB84(piVar4,0);
                uStack_98._4_4_ = (undefined4)((ulong)piVar4 >> 0x20);
                uStack_90._0_4_ = SUB84(piVar5,0);
                uStack_90._4_4_ = (undefined4)((ulong)piVar5 >> 0x20);
                pcVar22 = (char *)(&local_b0)[uVar17 - 6];
                local_b0 = piVar10;
                uStack_a8 = piVar2;
                uStack_a0 = piVar3;
                uStack_98 = piVar4;
                uStack_90 = piVar5;
              }
joined_r0x00158914:
              if (pcVar22 == (char *)0x0) goto LAB_00158918;
            }
LAB_00158868:
            FUN_00158f70(auStack_614,0x514,pcVar22);
            lVar14 = (**(code **)(*param_1 + 0x538))(param_1,auStack_614);
            if (lVar14 == 0) goto LAB_00158918;
            (**(code **)(*param_1 + 0x570))(param_1,lVar15,uVar21 & 0xffffffff,lVar14);
            (**(code **)(*param_1 + 0xb8))(param_1,lVar14);
            cVar12 = (**(code **)(*param_1 + 0x720))(param_1);
            if (cVar12 != '\0') goto LAB_00158918;
            uVar21 = uVar21 + 1;
          } while (uVar13 != uVar21);
        }
        free(__dest);
        goto LAB_0015896c;
      }
    }
    free(__dest);
  }
LAB_00158968:
  lVar15 = 0;
LAB_0015896c:
  if (*(long *)(lVar8 + 0x28) == local_78) {
    return lVar15;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_001589b0 @ 001589b0 ===== */

long FUN_001589b0(long *param_1)

{
  long lVar1;
  char cVar2;
  ulong uVar3;
  long lVar4;
  uint uVar5;
  undefined4 local_60;
  undefined4 uStack_5c;
  int local_58;
  int iStack_54;
  undefined4 local_50;
  undefined4 uStack_4c;
  uint local_48;
  uint uStack_44;
  uint local_40;
  undefined4 uStack_3c;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  uVar5 = 1;
  uVar3 = FUN_00193f80(1,&DAT_001a7c08);
  if ((uVar3 & 1) == 0) {
    local_58 = DAT_001a7c10;
    iStack_54 = DAT_001a7c14;
    local_48 = (uint)DAT_001a7c28;
    uStack_44 = (uint)DAT_001a7c29;
    local_60 = 1;
    uStack_5c = DAT_001a7c0c;
    local_50 = (undefined4)DAT_001a7c20;
    uStack_4c = (undefined4)((ulong)DAT_001a7c20 >> 0x20);
    local_40 = (uint)((((DAT_001a7c14 == 0 || DAT_001a7c10 == 0) || local_48 == 0) ||
                      DAT_001a7c2a == '\0') || uStack_44 != 0);
    uStack_3c = DAT_001a7c18;
    DAT_001a7c08 = 0;
    if ((((DAT_0022d8b0 == '\x01') && (DAT_0022d8b8 != 0)) && ((int)DAT_0022d8c8 != 0)) &&
       ((DAT_0022d8c0 != 0 && (uVar3 = FUN_00193f80(1,&DAT_001a7c08), (uVar3 & 1) == 0)))) {
      DAT_001a7c08 = 0;
      uVar5 = (uint)((((DAT_001a7c14 == 0 || DAT_001a7c10 == 0) || DAT_001a7c28 == 0) ||
                     DAT_001a7c2a == '\0') || DAT_001a7c29 != 0);
    }
    local_40 = uVar5;
    lVar4 = (**(code **)(*param_1 + 0x598))(param_1,10);
    if (lVar4 == 0) goto LAB_00158b34;
    (**(code **)(*param_1 + 0x698))(param_1,lVar4,0,10,&local_60);
    cVar2 = (**(code **)(*param_1 + 0x720))(param_1);
    if (cVar2 == '\0') goto LAB_00158b34;
    (**(code **)(*param_1 + 0x88))(param_1);
    (**(code **)(*param_1 + 0xb8))(param_1,lVar4);
  }
  lVar4 = 0;
LAB_00158b34:
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return lVar4;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00158b60 @ 00158b60 ===== */

undefined8 FUN_00158b60(undefined8 param_1,undefined8 param_2,byte param_3,byte param_4)

{
  bool bVar1;
  bool bVar2;
  bool bVar3;
  ulong uVar4;
  undefined8 uVar5;
  
  if ((param_4 | param_3) < 2) {
    do {
      uVar4 = FUN_00193f80(1,&DAT_001a7c08);
    } while ((uVar4 & 1) != 0);
    bVar2 = param_3 == 1;
    bVar3 = param_4 == 1;
    if (((bool)DAT_001a7c28 != bVar2) || ((bool)DAT_001a7c29 != bVar3)) {
      if ((bool)DAT_001a7c28 != bVar2) {
        DAT_001a7c2b = 0;
      }
      bVar1 = DAT_001a7c0c == -1;
      DAT_001a7c0c = DAT_001a7c0c + 1;
      DAT_001a7c28 = bVar2;
      DAT_001a7c29 = bVar3;
      if (bVar1) {
        DAT_001a7c0c = 1;
      }
    }
    uVar5 = 1;
    DAT_001a7c08 = 0;
  }
  else {
    uVar5 = 4;
  }
  return uVar5;
}

/* ===== FUN_00158c1c @ 00158c1c ===== */

undefined8 FUN_00158c1c(undefined8 param_1,undefined8 param_2,byte param_3)

{
  bool bVar1;
  bool bVar2;
  ulong uVar3;
  
  if (param_3 < 2) {
    do {
      uVar3 = FUN_00193f80(1,&DAT_001a7c08);
    } while ((uVar3 & 1) != 0);
    bVar2 = param_3 == 1;
    if (((bool)DAT_001a7c2a != bVar2) &&
       (bVar1 = DAT_001a7c0c == -1, DAT_001a7c0c = DAT_001a7c0c + 1, DAT_001a7c2a = bVar2, bVar1)) {
      DAT_001a7c0c = 1;
    }
    DAT_001a7c08 = 0;
    return 1;
  }
  return 4;
}

/* ===== FUN_00158ca4 @ 00158ca4 ===== */

void FUN_00158ca4(undefined8 param_1,undefined8 param_2,uint param_3)

{
  long lVar1;
  int iVar2;
  uint uVar3;
  undefined1 auStack_58 [12];
  uint auStack_4c [5];
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  if ((param_3 < 0x6c) && (iVar2 = FUN_001592cc(auStack_58), iVar2 != 0)) {
    uVar3 = auStack_4c[param_3 >> 5] >> (ulong)(param_3 & 0x1f) & 1;
  }
  else {
    uVar3 = 0xffffffff;
  }
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar3);
}

/* ===== FUN_00158f70 @ 00158f70 ===== */

void FUN_00158f70(undefined1 *param_1,ulong param_2,byte *param_3)

{
  ulong uVar1;
  uint uVar2;
  byte bVar3;
  long lVar4;
  size_t __n;
  ulong uVar5;
  byte *pbVar6;
  uint uVar7;
  long lVar8;
  long lVar9;
  byte local_70;
  byte local_6f;
  byte local_6e;
  undefined1 local_6d;
  byte local_6c;
  byte local_6b;
  long local_68;
  
  lVar4 = tpidr_el0;
  local_68 = *(long *)(lVar4 + 0x28);
  if (param_3 != (byte *)0x0) {
    uVar7 = (uint)*param_3;
    if (*param_3 == 0) {
      uVar5 = 0;
    }
    else {
      lVar8 = 0;
      uVar5 = 0;
      pbVar6 = param_3;
      do {
        if (uVar7 >> 7 == 0) {
LAB_001590cc:
          lVar9 = 1;
        }
        else if ((uVar7 + 0x3e & 0xff) < 0x1e) {
          if ((pbVar6[1] & 0xc0) != 0x80) goto LAB_001590c8;
          lVar9 = 2;
          uVar7 = pbVar6[1] & 0x3f | (uVar7 & 0x1f) << 6;
        }
        else if ((uVar7 & 0xf0) == 0xe0) {
          bVar3 = pbVar6[1];
          if (((((bVar3 & 0xc0) != 0x80) || ((pbVar6[2] & 0xc0) != 0x80)) ||
              ((uVar7 == 0xe0 && (bVar3 < 0xa0)))) || ((uVar7 == 0xed && (0x9f < bVar3)))) {
LAB_001590c8:
            uVar7 = 0xfffd;
            goto LAB_001590cc;
          }
          lVar9 = 3;
          uVar7 = (uVar7 & 0xf) << 0xc | (bVar3 & 0x3f) << 6 | pbVar6[2] & 0x3f;
        }
        else {
          if (((((4 < (uVar7 + 0x10 & 0xff)) || (bVar3 = pbVar6[1], (bVar3 & 0xc0) != 0x80)) ||
               ((pbVar6[2] & 0xc0) != 0x80)) || ((pbVar6[3] & 0xc0) != 0x80)) ||
             (((uVar7 == 0xf0 && (bVar3 < 0x90)) || ((uVar7 == 0xf4 && (0x8f < bVar3))))))
          goto LAB_001590c8;
          lVar9 = 4;
          uVar7 = (uVar7 & 7) << 0x12 | (bVar3 & 0x3f) << 0xc | (pbVar6[2] & 0x3f) << 6 |
                  pbVar6[3] & 0x3f;
        }
        uVar2 = 0x20;
        if (0xfffffffd < uVar7 - 0xb || 0x1f < uVar7) {
          uVar2 = uVar7;
        }
        bVar3 = (byte)uVar2;
        if (uVar2 < 0x80) {
          __n = 1;
          local_70 = bVar3;
        }
        else if (uVar2 < 0x800) {
          local_70 = (byte)(uVar2 >> 6) | 0xc0;
          local_6f = bVar3 & 0x3f | 0x80;
          __n = 2;
        }
        else if (uVar2 >> 0x10 == 0) {
          local_70 = (byte)(uVar2 >> 0xc) | 0xe0;
          local_6f = (byte)(uVar2 >> 6) & 0x3f | 0x80;
          local_6e = bVar3 & 0x3f | 0x80;
          __n = 3;
        }
        else {
          __n = 6;
          local_70 = 0xed;
          local_6e = (byte)(uVar2 - 0x10000 >> 10) & 0x3f | 0x80;
          local_6f = (byte)((uVar2 - 0x10000 >> 10) + 0x800 >> 6) | 0x80;
          local_6c = (byte)(uVar2 >> 6) & 0xf | 0xb0;
          local_6b = bVar3 & 0x3f | 0x80;
          local_6d = 0xed;
        }
        uVar1 = __n + uVar5;
        if (param_2 <= uVar1) break;
        memcpy(param_1 + uVar5,&local_70,__n);
        lVar8 = lVar9 + lVar8;
        pbVar6 = param_3 + lVar8;
        uVar7 = (uint)*pbVar6;
        uVar5 = uVar1;
      } while (uVar7 != 0);
    }
    param_1 = param_1 + uVar5;
  }
  *param_1 = 0;
  if (*(long *)(lVar4 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0015922c @ 0015922c ===== */

bool FUN_0015922c(void)

{
  ulong uVar1;
  
  if (DAT_0022d8b0 == '\x01' && DAT_0022d8b8 != 0) {
    if ((int)DAT_0022d8c8 == 0) {
      return false;
    }
    if (DAT_0022d8c0 == 0) {
      return false;
    }
    uVar1 = FUN_00193f80(1,&DAT_001a7c08);
    if ((uVar1 & 1) == 0) {
      DAT_001a7c08 = 0;
      return (((DAT_001a7c14 != 0 && DAT_001a7c10 != 0) && DAT_001a7c28 != '\0') &&
             DAT_001a7c2a != '\0') && DAT_001a7c29 == '\0';
    }
  }
  return false;
}

/* ===== FUN_001597b8 @ 001597b8 ===== */

bool FUN_001597b8(int *param_1)

{
  uint uVar1;
  
  if ((((*param_1 == 0x3149524e) && (param_1[1] == 1)) && (param_1[2] == 0x6c)) &&
     (uVar1 = param_1[4] ^ ((param_1[3] ^ 0x3b7d4c22U) >> 0x1b | (param_1[3] ^ 0x3b7d4c22U) << 5),
     uVar1 = param_1[5] ^ (uVar1 >> 0x1b | uVar1 << 5),
     param_1[7] == (param_1[6] ^ (uVar1 >> 0x1b | uVar1 << 5)))) {
    return (uint)param_1[6] < 0x1000;
  }
  return false;
}

/* ===== FUN_00159824 @ 00159824 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_00159824(undefined8 param_1,int param_2,int *param_3)

{
  undefined8 uVar1;
  undefined8 uVar2;
  int iVar3;
  undefined8 uVar4;
  int iVar5;
  code *pcVar6;
  
  uVar4 = DAT_0025c928;
  if ((param_3 == (int *)0x0) || (*param_3 != 0x18)) {
    uVar4 = 4;
  }
  else if ((param_2 - 0x7dU < 4) || (param_2 == 0x10010)) {
    iVar3 = pthread_once((pthread_once_t *)&DAT_0022f1d0,FUN_001552c0);
    pcVar6 = DAT_0022f1d8;
    if ((DAT_0022f1d8 != (code *)0x0) &&
       (iVar3 = (*DAT_0022f1d8)(iVar3), pcVar6 = _DAT_0022db68, iVar3 != 1)) {
      pcVar6 = (code *)0x0;
    }
    iVar3 = (int)uVar4;
    uVar4 = 1;
    *(undefined1 *)((long)param_3 + 0x15) = 0;
    uVar2 = _UNK_0010f968;
    uVar1 = _DAT_0010f960;
    param_3[4] = (uint)(iVar3 == 0);
    *(bool *)(param_3 + 5) = iVar3 != 0;
    *(undefined8 *)(param_3 + 2) = uVar2;
    *(undefined8 *)param_3 = uVar1;
    *(undefined2 *)((long)param_3 + 0x16) = 0;
    if ((param_2 == 0x10010) || (param_2 == 0x7e)) {
      *(undefined1 *)((long)param_3 + 0x15) = 1;
      iVar5 = (int)pcVar6;
      param_3[1] = iVar5;
      param_3[2] = (uint)(iVar3 != 0 && iVar5 != 0);
      param_3[3] = iVar5;
    }
  }
  else {
    uVar4 = 0;
  }
  return uVar4;
}

/* ===== FUN_00159930 @ 00159930 ===== */

void FUN_00159930(undefined8 param_1,int *param_2)

{
  long lVar1;
  int iVar2;
  int iVar3;
  undefined8 uVar4;
  int *__arg;
  pthread_t local_40;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  if (((param_2 == (int *)0x0) || (*param_2 != 0x40)) || (param_2[1] - 0x81U < 0xfffffffc)) {
LAB_00159974:
    uVar4 = 4;
  }
  else {
    uVar4 = FUN_00159ac0();
    if ((int)uVar4 != 0) {
      iVar3 = param_2[1];
      if (iVar3 == 0x7e) {
        if (1 < (uint)param_2[3]) goto LAB_00159974;
        uVar4 = 1;
        DAT_0022db68 = param_2[3];
      }
      else {
        if (*(long *)(param_2 + 0xc) != 0) {
          if (*(long *)(lVar1 + 0x28) == local_38) {
            FUN_00159bbc(iVar3,*(undefined8 *)(param_2 + 10),*(long *)(param_2 + 0xc),DAT_0022db60,
                         DAT_001a7c48);
            return;
          }
          goto LAB_00159abc;
        }
        uVar4 = 0;
        if ((DAT_0025c8d8 != 0) && (DAT_0025c8d0 != 0)) {
          iVar2 = FUN_00193fe0(1,&DAT_0025c9d8);
          if (iVar2 == 0) {
            __arg = (int *)calloc(1,0x38);
            if (__arg != (int *)0x0) {
              *__arg = iVar3;
              uVar4 = DAT_001a7c48;
              *(undefined8 *)(__arg + 2) = DAT_0022db60;
              *(undefined8 *)(__arg + 4) = uVar4;
              iVar3 = pthread_create(&local_40,(pthread_attr_t *)0x0,FUN_0015a108,__arg);
              if (iVar3 == 0) {
                pthread_detach(local_40);
                uVar4 = 2;
                goto LAB_00159978;
              }
              free(__arg);
            }
            uVar4 = 0xffffffff;
            DAT_0025c9d8._0_4_ = 0;
          }
          else {
            uVar4 = 3;
          }
        }
      }
    }
  }
LAB_00159978:
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
LAB_00159abc:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar4);
}

/* ===== FUN_00159ac0 @ 00159ac0 ===== */

void FUN_00159ac0(void)

{
  long lVar1;
  bool bVar2;
  int iVar3;
  int iVar4;
  pthread_t __target_thread;
  long local_38;
  undefined8 local_30;
  long local_28;
  
  iVar4 = DAT_0022f038;
  lVar1 = tpidr_el0;
  local_28 = *(long *)(lVar1 + 0x28);
  if (((int)DAT_0025c928 != 0) && ((int)DAT_0022db78 != 0)) {
    local_38 = 0;
    local_30 = 0;
    if ((0 < DAT_0022f038) && (iVar3 = gettid(), iVar3 == iVar4)) {
      __target_thread = pthread_self();
      iVar4 = pthread_getname_np(__target_thread,(char *)&local_38,0x10);
      if (iVar4 == 0) {
        bVar2 = false;
        if ((local_38 != 0x706f6f6c6e69614d || (char)local_30 != '\0') || (DAT_0022f03c < 1))
        goto LAB_00159b34;
        iVar4 = pthread_once((pthread_once_t *)&DAT_0022f1d0,FUN_001552c0);
        if (DAT_0022f1d8 != (code *)0x0) {
          iVar4 = (*DAT_0022f1d8)(iVar4);
          bVar2 = iVar4 == 1;
          goto LAB_00159b34;
        }
      }
    }
  }
  bVar2 = false;
LAB_00159b34:
  if (*(long *)(lVar1 + 0x28) == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(bVar2);
}

/* ===== FUN_00159bbc @ 00159bbc ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8
FUN_00159bbc(void *param_1,void *param_2,ulong param_3,undefined8 param_4,undefined8 param_5)

{
  byte *pbVar1;
  long lVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  ulong uVar6;
  ulong uVar7;
  void *pvVar8;
  undefined8 uVar9;
  timespec local_90;
  undefined1 auStack_80 [40];
  long local_58;
  
  lVar2 = tpidr_el0;
  local_58 = *(long *)(lVar2 + 0x28);
  iVar3 = (int)param_1;
  if (iVar3 == 0x7f) {
    uVar9 = 4;
    if (((param_2 == (void *)0x0) || (0x5f < param_3 - 1)) ||
       (param_1 = memchr(param_2,0,param_3), param_1 != (void *)0x0)) goto LAB_00159d8c;
    uVar6 = 0;
    do {
      if ((4 < *(byte *)((long)param_2 + uVar6) - 9) && (*(byte *)((long)param_2 + uVar6) != 0x20))
      {
        uVar7 = param_3;
        if (param_3 <= uVar6) goto LAB_00159ce0;
        uVar9 = 4;
        goto LAB_00159cc8;
      }
      uVar6 = uVar6 + 1;
    } while (uVar6 != param_3);
  }
  else {
    param_1 = (void *)FUN_00159f20(param_2,param_3,auStack_80);
    if ((int)param_1 != 0) goto LAB_00159c84;
  }
  goto LAB_00159ce8;
  while (uVar7 = uVar7 - 1, uVar6 < uVar7) {
LAB_00159cc8:
    uVar5 = (uint)*(byte *)((long)param_2 + (uVar7 - 1));
    if ((4 < uVar5 - 9) && (uVar5 != 0x20)) goto LAB_00159ce0;
  }
  goto LAB_00159d8c;
LAB_00159ce0:
  if (uVar6 != uVar7) {
    if (uVar6 < uVar7) {
      uVar5 = 0;
      uVar9 = 4;
      pvVar8 = param_2;
      do {
        pbVar1 = (byte *)((long)pvVar8 + uVar6);
        if ((*pbVar1 - 0x3a < 0xfffffff6) || (999 < uVar5)) goto LAB_00159d8c;
        uVar7 = uVar7 - 1;
        pvVar8 = (void *)((long)pvVar8 + 1);
        uVar5 = ((uint)*pbVar1 + uVar5 * 10) - 0x30;
      } while (uVar6 != uVar7);
    }
LAB_00159c84:
    uVar9 = 4;
    if ((iVar3 - 0x7dU < 4) && (iVar3 - 0x7dU != 1)) {
      pthread_mutex_lock((pthread_mutex_t *)((long)&DAT_0025c928 + 4));
      if (DAT_0025c958 == 0) {
        iVar4 = clock_gettime(1,&local_90);
        if (iVar4 == 0) {
          DAT_0025c9d0 = local_90.tv_sec * 1000 + (ulong)local_90.tv_nsec / 1000000;
        }
        else {
          DAT_0025c9d0 = 0;
        }
        DAT_0025c9bc = 0;
        uRam000000000025c974 = 0;
        _DAT_0025c96c = 0;
        uRam000000000025c964 = 0;
        _DAT_0025c95c = 0;
        uRam000000000025c984 = 0;
        _DAT_0025c97c = 0;
        uRam000000000025c994 = 0;
        _DAT_0025c98c = 0;
        uRam000000000025c9a4 = 0;
        _DAT_0025c99c = 0;
        uRam000000000025c9b4 = 0;
        _DAT_0025c9ac = 0;
        DAT_0025c958 = iVar3;
        DAT_0025c9c0 = param_4;
        DAT_0025c9c8 = param_5;
        __memcpy_chk(&DAT_0025c95c,param_2,param_3,0x7c);
        uVar9 = 2;
        (&DAT_0025c95c)[param_3] = 0;
      }
      else {
        uVar9 = 3;
      }
      uVar5 = pthread_mutex_unlock((pthread_mutex_t *)((long)&DAT_0025c928 + 4));
      param_1 = (void *)(ulong)uVar5;
    }
    goto LAB_00159d8c;
  }
LAB_00159ce8:
  uVar9 = 4;
LAB_00159d8c:
  if (*(long *)(lVar2 + 0x28) != local_58) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(param_1);
  }
  return uVar9;
}

/* ===== FUN_00159e08 @ 00159e08 ===== */

undefined8 FUN_00159e08(void *param_1,ulong param_2,uint *param_3)

{
  byte *pbVar1;
  void *pvVar2;
  ulong uVar3;
  uint uVar4;
  
  if (0x5f < param_2 - 1) {
    return 0;
  }
  if (param_1 == (void *)0x0) {
    return 0;
  }
  if (param_3 == (uint *)0x0) {
    return 0;
  }
  pvVar2 = memchr(param_1,0,param_2);
  if (pvVar2 == (void *)0x0) {
    uVar3 = 0;
    while ((*(byte *)((long)param_1 + uVar3) - 9 < 5 || (*(byte *)((long)param_1 + uVar3) == 0x20)))
    {
      uVar3 = uVar3 + 1;
      if (uVar3 == param_2) {
        return 0;
      }
    }
    if (uVar3 < param_2) {
      while ((uVar4 = (uint)*(byte *)((long)param_1 + (param_2 - 1)), uVar4 - 9 < 5 ||
             (uVar4 == 0x20))) {
        param_2 = param_2 - 1;
        if (param_2 <= uVar3) {
          return 0;
        }
      }
    }
    if (uVar3 == param_2) {
      return 0;
    }
    uVar4 = 0;
    if (uVar3 < param_2) {
      do {
        pbVar1 = (byte *)((long)param_1 + uVar3);
        if (*pbVar1 - 0x3a < 0xfffffff6) {
          return 0;
        }
        if (999 < uVar4) {
          return 0;
        }
        param_2 = param_2 - 1;
        param_1 = (void *)((long)param_1 + 1);
        uVar4 = ((uint)*pbVar1 + uVar4 * 10) - 0x30;
      } while (uVar3 != param_2);
    }
    *param_3 = uVar4;
    return 1;
  }
  return 0;
}

/* ===== FUN_00159f20 @ 00159f20 ===== */

void FUN_00159f20(void *param_1,ulong param_2,ulong *param_3)

{
  byte bVar1;
  long lVar2;
  long lVar3;
  void *pvVar4;
  uint uVar5;
  ulong uVar6;
  ulong uVar7;
  ulong uVar8;
  byte *pbVar9;
  undefined1 *puVar10;
  undefined8 uStack_88;
  ulong uStack_80;
  ulong local_78;
  ulong uStack_70;
  long local_68;
  
  lVar2 = tpidr_el0;
  lVar3 = 0;
  local_68 = *(long *)(lVar2 + 0x28);
  if (((param_2 - 1 < 0x60) && (param_1 != (void *)0x0)) && (param_3 != (ulong *)0x0)) {
    pvVar4 = memchr(param_1,0,param_2);
    lVar3 = 0;
    if (pvVar4 == (void *)0x0) {
      uVar6 = 0;
      do {
        if ((4 < *(byte *)((long)param_1 + uVar6) - 9) && (*(byte *)((long)param_1 + uVar6) != 0x20)
           ) {
          if (uVar6 < param_2) goto LAB_00159fd4;
          goto LAB_00159ff8;
        }
        uVar6 = uVar6 + 1;
      } while (param_2 != uVar6);
LAB_0015a0d0:
      lVar3 = 0;
    }
  }
LAB_0015a0d4:
  if (*(long *)(lVar2 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(lVar3);
LAB_00159fd4:
  uVar5 = (uint)*(byte *)((long)param_1 + (param_2 - 1));
  if ((uVar5 - 9 < 5) || (uVar5 == 0x20)) goto LAB_00159fc4;
  if (*(char *)((long)param_1 + uVar6) == '#') {
    uVar6 = uVar6 + 1;
  }
LAB_00159ff8:
  lVar3 = 0;
  uVar8 = param_2 - uVar6;
  if ((uVar8 == 0) || (0x1e < uVar8)) goto LAB_0015a0d4;
  uVar7 = 0;
  uStack_80 = 0;
  uStack_70 = 0;
  local_78 = 0;
  uStack_88 = 0x23;
  if (param_2 <= uVar6) {
LAB_0015a0ac:
    lVar3 = 1;
    param_3[4] = uStack_70;
    param_3[1] = uStack_88;
    *param_3 = CONCAT44((int)(uVar7 >> 8),(int)uVar7) & 0xffffffff000000ff;
    param_3[3] = local_78;
    param_3[2] = uStack_80;
    goto LAB_0015a0d4;
  }
  pbVar9 = (byte *)((long)param_1 + uVar6);
  puVar10 = (undefined1 *)((long)&uStack_88 + 1);
  while( true ) {
    bVar1 = *pbVar9;
    uVar5 = bVar1 - 0x20;
    if (0x19 < bVar1 - 0x61) {
      uVar5 = (uint)bVar1;
    }
    lVar3 = __strchr_chk("0289PYLQGRJCUV",uVar5 & 0xff,0xf);
    if (lVar3 == 0) goto LAB_0015a0d4;
    if ((0xffffffffffU - (lVar3 + -0x131006)) / 0xe < uVar7) break;
    uVar8 = uVar8 - 1;
    pbVar9 = pbVar9 + 1;
    uVar7 = lVar3 + -0x131006 + uVar7 * 0xe;
    *puVar10 = (char)uVar5;
    puVar10 = puVar10 + 1;
    if (uVar8 == 0) goto LAB_0015a0ac;
  }
  goto LAB_0015a0d0;
LAB_00159fc4:
  lVar3 = 0;
  param_2 = param_2 - 1;
  if (param_2 <= uVar6) goto LAB_0015a0d4;
  goto LAB_00159fd4;
}

/* ===== FUN_0015b33c @ 0015b33c ===== */

undefined8 FUN_0015b33c(long param_1)

{
  long lVar1;
  bool bVar2;
  long *plVar3;
  char cVar4;
  char cVar5;
  int iVar6;
  int iVar7;
  long lVar8;
  long lVar9;
  long *local_70;
  long local_68;
  
  lVar1 = tpidr_el0;
  local_68 = *(long *)(lVar1 + 0x28);
  local_70 = (long *)0x0;
  if (DAT_0022f4c8 != (long *)0x0 && DAT_0022f4e8 != 0) {
    iVar6 = (**(code **)(*DAT_0022f4c8 + 0x30))(DAT_0022f4c8,&local_70,0x10006);
    if (iVar6 == 0) {
      bVar2 = true;
      plVar3 = local_70;
    }
    else {
      if ((iVar6 != -2) ||
         (iVar6 = (**(code **)(*DAT_0022f4c8 + 0x20))(DAT_0022f4c8,&local_70,0), iVar6 != 0))
      goto LAB_0015b4e0;
      bVar2 = false;
      plVar3 = local_70;
    }
    local_70 = plVar3;
    if (plVar3 != (long *)0x0) {
      iVar6 = (**(code **)(*plVar3 + 0x98))(plVar3,8);
      if ((iVar6 == 0) &&
         (lVar9 = (**(code **)(*plVar3 + 0x30))(plVar3,"android/app/Dialog"), lVar9 != 0)) {
        lVar9 = (**(code **)(*plVar3 + 0x108))(plVar3,lVar9,"isShowing",&DAT_001361b3);
        if ((lVar9 == 0) ||
           (lVar8 = (**(code **)(*plVar3 + 0x30))(plVar3,"android/os/Looper"), lVar8 == 0)) {
          lVar8 = 0;
        }
        else {
          lVar8 = (**(code **)(*plVar3 + 0x108))(plVar3,lVar8,&DAT_00135b8c,&DAT_0013ba46);
        }
      }
      else {
        lVar8 = 0;
        lVar9 = 0;
      }
      cVar4 = (**(code **)(*plVar3 + 0x720))(plVar3);
      if (((cVar4 == '\0') && (lVar9 != 0)) && (lVar8 != 0)) {
        *(undefined4 *)(param_1 + 0x28) = 1;
        iVar7 = (int)*(undefined8 *)(param_1 + 0x34);
        while (iVar7 == 0) {
          cVar4 = (**(code **)(*plVar3 + 0x128))(plVar3,*(undefined8 *)(param_1 + 0x18),lVar9);
          cVar5 = (**(code **)(*plVar3 + 0x720))(plVar3);
          if (cVar5 != '\0') break;
          if (cVar4 == '\0') {
            *(undefined4 *)(param_1 + 0x30) = 1;
            break;
          }
          usleep(40000);
          iVar7 = (int)*(undefined8 *)(param_1 + 0x34);
        }
      }
      cVar4 = (**(code **)(*plVar3 + 0x720))(plVar3);
      if (cVar4 != '\0') {
        (**(code **)(*plVar3 + 0x88))(plVar3);
      }
      if (lVar8 != 0) {
        (**(code **)(*plVar3 + 0x1e8))(plVar3,*(undefined8 *)(param_1 + 0x20),lVar8);
        cVar4 = (**(code **)(*plVar3 + 0x720))(plVar3);
        if (cVar4 != '\0') {
          (**(code **)(*plVar3 + 0x88))(plVar3);
        }
      }
      if (iVar6 == 0) {
        (**(code **)(*plVar3 + 0xa0))(plVar3,0);
      }
      if (!bVar2) {
        (**(code **)(*DAT_0022f4c8 + 0x28))();
      }
    }
  }
LAB_0015b4e0:
  *(undefined4 *)(param_1 + 0x2c) = 1;
  if (*(long *)(lVar1 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return 0;
}

/* ===== FUN_0015c6f8 @ 0015c6f8 ===== */

void FUN_0015c6f8(ulong param_1,long param_2,undefined4 param_3,char *param_4,int param_5)

{
  int iVar1;
  undefined8 uVar2;
  char *__s2;
  long lVar3;
  
  if (param_4 == (char *)0x0) {
    __s2 = "unknown";
  }
  else {
    lVar3 = 0;
    do {
      __s2 = *(char **)((long)&PTR_DAT_0019b378 + lVar3);
      iVar1 = strcmp(param_4,__s2);
      if (iVar1 == 0) break;
      lVar3 = lVar3 + 8;
      __s2 = "unknown";
    } while (lVar3 != 0xa0);
  }
  iVar1 = strcmp(__s2,"mmap");
  if ((((iVar1 == 0) || (iVar1 = strcmp(__s2,"seal"), iVar1 == 0)) ||
      (iVar1 = strcmp(__s2,"gap_open"), iVar1 == 0)) ||
     (((iVar1 = strcmp(__s2,"gap_read"), iVar1 == 0 ||
       (iVar1 = strcmp(__s2,"gap_close"), iVar1 == 0)) ||
      ((iVar1 = strcmp(__s2,"gap_mmap"), iVar1 == 0 ||
       (iVar1 = strcmp(__s2,"gap_unmap"), iVar1 == 0)))))) {
    if (0xffe < param_5 - 1U) {
      param_5 = 0;
    }
  }
  else {
    param_5 = 0;
  }
  if (((0xffffffffff88d78f < param_1) || (uVar2 = 0x772870, param_1 + 0x772870 != param_2)) &&
     (((0xffffffffffa6de5f < param_1 || (uVar2 = 0x5921a0, param_1 + 0x5921a0 != param_2)) &&
      (((0xffffffffff777c93 < param_1 || (uVar2 = 0x88836c, param_1 + 0x88836c != param_2)) &&
       (uVar2 = 0x678fe4, param_1 + 0x678fe4 != param_2 || 0xffffffffff98701b < param_1)))))) {
    uVar2 = 0;
  }
  FUN_0015540c(0x25c9dc,0x37,0x37,"pb_island_i%u_%lx_last_%s_e%u",param_3,uVar2,__s2,param_5);
  return;
}

/* ===== FUN_0015c8dc @ 0015c8dc ===== */

void FUN_0015c8dc(void)

{
  FUN_0014e7c4();
  return;
}

/* ===== FUN_0015c8e4 @ 0015c8e4 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0015c8e4(undefined8 param_1,long param_2,undefined4 param_3)

{
  long lVar1;
  bool bVar2;
  long lVar3;
  undefined1 auVar4 [16];
  undefined1 auVar5 [16];
  undefined1 auVar6 [16];
  undefined1 auVar7 [16];
  undefined4 local_2c;
  long local_28;
  
  lVar1 = tpidr_el0;
  local_28 = *(long *)(lVar1 + 0x28);
  local_2c = param_3;
  if (DAT_0022d8e0 + 0x89902c != param_2) {
    auVar4._0_8_ = DAT_0022d8e0 + _DAT_0010fa00;
    auVar4._8_8_ = DAT_0022d8e0 + _UNK_0010fa08;
    auVar6._0_8_ = DAT_0022d8e0 + _DAT_0010fa20;
    auVar6._8_8_ = DAT_0022d8e0 + _UNK_0010fa28;
    auVar5._8_8_ = param_2;
    auVar5._0_8_ = param_2;
    auVar5 = NEON_cmeq(auVar4,auVar5,8);
    auVar7._8_8_ = param_2;
    auVar7._0_8_ = param_2;
    auVar7 = NEON_cmeq(auVar6,auVar7,8);
    if ((((auVar7 & (undefined1  [16])0x1) == (undefined1  [16])0x0 &&
         (auVar5 & (undefined1  [16])0x1) == (undefined1  [16])0x0) &&
        (auVar5 & (undefined1  [16])0x1) == (undefined1  [16])0x0) &&
        DAT_0022d8e0 + 0x85cce8 != param_2) {
      bVar2 = false;
      goto LAB_0015c9ac;
    }
  }
  lVar3 = FUN_00153cbc(DAT_001a7c38,&local_2c,4);
  bVar2 = lVar3 == 4;
LAB_0015c9ac:
  if (*(long *)(lVar1 + 0x28) == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(bVar2);
}

/* ===== FUN_0015c9d0 @ 0015c9d0 ===== */

void FUN_0015c9d0(undefined8 param_1,long param_2)

{
  DataMemoryBarrier(2,3);
  FUN_00193944(param_2,param_2 + 4);
  return;
}

/* ===== FUN_0015c9e0 @ 0015c9e0 ===== */

void FUN_0015c9e0(undefined8 *param_1,undefined8 *param_2,uint *param_3)

{
  ulong uVar1;
  long lVar2;
  int iVar3;
  undefined8 *puVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  uint uVar7;
  uint uVar8;
  long lVar9;
  char *pcVar10;
  undefined8 *puVar11;
  ulong uVar12;
  undefined8 uVar13;
  undefined8 uVar14;
  uint local_164;
  undefined4 uStack_160;
  undefined4 local_15c;
  undefined4 uStack_158;
  undefined4 uStack_154;
  undefined4 uStack_150;
  undefined4 local_14c;
  undefined4 uStack_148;
  undefined4 uStack_144;
  undefined4 uStack_140;
  undefined4 local_13c;
  undefined4 uStack_138;
  undefined4 uStack_134;
  undefined4 uStack_130;
  undefined8 local_12c;
  undefined4 uStack_124;
  undefined4 local_120;
  undefined4 uStack_11c;
  undefined4 uStack_118;
  undefined4 uStack_114;
  undefined4 uStack_110;
  undefined4 uStack_10c;
  undefined4 uStack_108;
  undefined4 uStack_104;
  undefined4 local_100;
  undefined4 uStack_fc;
  undefined4 uStack_f8;
  undefined4 uStack_f4;
  undefined8 local_f0;
  undefined4 uStack_e8;
  undefined4 local_e4;
  undefined4 uStack_e0;
  undefined4 uStack_dc;
  undefined4 uStack_d8;
  undefined4 local_d4;
  undefined4 uStack_d0;
  undefined4 uStack_cc;
  undefined4 uStack_c8;
  undefined4 local_c4;
  undefined4 uStack_c0;
  undefined4 uStack_bc;
  undefined4 uStack_b8;
  undefined8 local_b4;
  undefined4 uStack_ac;
  undefined4 local_a8;
  undefined4 uStack_a4;
  undefined4 uStack_a0;
  undefined4 uStack_9c;
  undefined4 uStack_98;
  undefined4 uStack_94;
  undefined4 uStack_90;
  undefined4 uStack_8c;
  undefined4 local_88;
  undefined4 uStack_84;
  undefined4 uStack_80;
  undefined4 uStack_7c;
  undefined8 local_78;
  long local_70;
  
  lVar2 = tpidr_el0;
  lVar9 = 0;
  local_70 = *(long *)(lVar2 + 0x28);
  param_3[0x2c] = 0;
  param_3[0x2d] = 0;
  param_3[6] = 0;
  param_3[7] = 0;
  param_3[4] = 0;
  param_3[5] = 0;
  param_3[10] = 0;
  param_3[0xb] = 0;
  param_3[8] = 0;
  param_3[9] = 0;
  param_3[0xe] = 0;
  param_3[0xf] = 0;
  param_3[0xc] = 0;
  param_3[0xd] = 0;
  param_3[0x12] = 0;
  param_3[0x13] = 0;
  param_3[0x10] = 0;
  param_3[0x11] = 0;
  param_3[0x16] = 0;
  param_3[0x17] = 0;
  param_3[0x14] = 0;
  param_3[0x15] = 0;
  param_3[0x1a] = 0;
  param_3[0x1b] = 0;
  param_3[0x18] = 0;
  param_3[0x19] = 0;
  param_3[0x1e] = 0;
  param_3[0x1f] = 0;
  param_3[0x1c] = 0;
  param_3[0x1d] = 0;
  param_3[0x22] = 0;
  param_3[0x23] = 0;
  param_3[0x20] = 0;
  param_3[0x21] = 0;
  param_3[0x26] = 0;
  param_3[0x27] = 0;
  param_3[0x24] = 0;
  param_3[0x25] = 0;
  param_3[0x2a] = 0;
  param_3[0x2b] = 0;
  param_3[0x28] = 0;
  param_3[0x29] = 0;
  param_3[2] = 0;
  param_3[3] = 0;
  param_3[0] = 0;
  param_3[1] = 0;
  local_164 = 0;
  puVar4 = param_1;
  do {
    puVar11 = puVar4;
    if (lVar9 == -4) {
      iVar3 = 1;
      goto LAB_0015ccec;
    }
    iVar3 = (*(code *)param_2[2])(*param_2,*puVar11,*(undefined4 *)((long)puVar11 + 0xc));
    if (iVar3 == 0) {
      uVar5 = *puVar11;
      uVar8 = -(int)lVar9;
      puVar4 = (undefined8 *)__emutls_get_address(&DAT_001a7c68);
      uVar13 = puVar4[5];
      uVar6 = puVar4[4];
      uStack_a0 = (undefined4)puVar4[1];
      uStack_9c = (undefined4)((ulong)puVar4[1] >> 0x20);
      local_a8 = (undefined4)*puVar4;
      uStack_a4 = (undefined4)((ulong)*puVar4 >> 0x20);
      uStack_90 = (undefined4)puVar4[3];
      uStack_8c = (undefined4)((ulong)puVar4[3] >> 0x20);
      uStack_98 = (undefined4)puVar4[2];
      uStack_94 = (undefined4)((ulong)puVar4[2] >> 0x20);
      local_78 = puVar4[6];
      *param_3 = uVar8;
      uStack_80 = (undefined4)uVar13;
      uStack_7c = (undefined4)((ulong)uVar13 >> 0x20);
      local_88 = (undefined4)uVar6;
      uStack_84 = (undefined4)((ulong)uVar6 >> 0x20);
      *(ulong *)(param_3 + 9) = CONCAT44(uStack_a0,uStack_a4);
      *(ulong *)(param_3 + 7) = CONCAT44(local_a8,uStack_ac);
      *(ulong *)(param_3 + 0xd) = CONCAT44(uStack_90,uStack_94);
      *(ulong *)(param_3 + 0xb) = CONCAT44(uStack_98,uStack_9c);
      *(ulong *)(param_3 + 0x11) = CONCAT44(uStack_80,uStack_84);
      *(ulong *)(param_3 + 0xf) = CONCAT44(local_88,uStack_8c);
      *(undefined8 *)(param_3 + 2) = uVar5;
      *(char **)(param_3 + 4) = "write";
      param_3[6] = 0;
      *(undefined8 *)(param_3 + 0x14) = local_78;
      *(undefined8 *)(param_3 + 0x12) = uVar13;
      (*(code *)param_2[3])(*param_2,*puVar11);
      goto LAB_0015cbec;
    }
    (*(code *)param_2[3])(*param_2,*puVar11);
    iVar3 = (*(code *)param_2[1])(*param_2,*puVar11,&local_164);
    uVar7 = local_164;
    if (iVar3 == 0) {
      uVar6 = *puVar11;
      uVar8 = -(int)lVar9;
      puVar4 = (undefined8 *)__emutls_get_address(&DAT_001a7c68);
      uStack_dc = (undefined4)puVar4[1];
      uStack_d8 = (undefined4)((ulong)puVar4[1] >> 0x20);
      local_e4 = (undefined4)*puVar4;
      uStack_e0 = (undefined4)((ulong)*puVar4 >> 0x20);
      uVar5 = puVar4[6];
      uStack_cc = (undefined4)puVar4[3];
      uStack_c8 = (undefined4)((ulong)puVar4[3] >> 0x20);
      local_d4 = (undefined4)puVar4[2];
      uStack_d0 = (undefined4)((ulong)puVar4[2] >> 0x20);
      uVar14 = puVar4[5];
      uVar13 = puVar4[4];
      *param_3 = uVar8;
      *(undefined8 *)(param_3 + 2) = uVar6;
      *(undefined **)(param_3 + 4) = &DAT_00133b50;
      uStack_bc = (undefined4)uVar14;
      uStack_b8 = (undefined4)((ulong)uVar14 >> 0x20);
      local_c4 = (undefined4)uVar13;
      uStack_c0 = (undefined4)((ulong)uVar13 >> 0x20);
      param_3[6] = 0;
      *(ulong *)(param_3 + 9) = CONCAT44(uStack_dc,uStack_e0);
      *(ulong *)(param_3 + 7) = CONCAT44(local_e4,uStack_e8);
      *(ulong *)(param_3 + 0xd) = CONCAT44(uStack_cc,uStack_d0);
      *(ulong *)(param_3 + 0xb) = CONCAT44(local_d4,uStack_d8);
      *(ulong *)(param_3 + 0x11) = CONCAT44(uStack_bc,uStack_c0);
      *(ulong *)(param_3 + 0xf) = CONCAT44(local_c4,uStack_c8);
      local_b4 = uVar5;
      goto LAB_0015cbe8;
    }
    lVar9 = lVar9 + -1;
    puVar4 = puVar11 + 2;
  } while (local_164 == *(uint *)((long)puVar11 + 0xc));
  uVar6 = *puVar11;
  uVar8 = ~(uint)lVar9;
  puVar4 = (undefined8 *)__emutls_get_address(&DAT_001a7c68);
  uVar14 = puVar4[5];
  uVar13 = puVar4[4];
  uStack_118 = (undefined4)puVar4[1];
  uStack_114 = (undefined4)((ulong)puVar4[1] >> 0x20);
  local_120 = (undefined4)*puVar4;
  uStack_11c = (undefined4)((ulong)*puVar4 >> 0x20);
  uStack_108 = (undefined4)puVar4[3];
  uStack_104 = (undefined4)((ulong)puVar4[3] >> 0x20);
  uStack_110 = (undefined4)puVar4[2];
  uStack_10c = (undefined4)((ulong)puVar4[2] >> 0x20);
  uVar5 = puVar4[6];
  *param_3 = uVar8;
  uStack_f8 = (undefined4)uVar14;
  uStack_f4 = (undefined4)((ulong)uVar14 >> 0x20);
  local_100 = (undefined4)uVar13;
  uStack_fc = (undefined4)((ulong)uVar13 >> 0x20);
  *(ulong *)(param_3 + 9) = CONCAT44(uStack_118,uStack_11c);
  *(ulong *)(param_3 + 7) = CONCAT44(local_120,uStack_124);
  *(ulong *)(param_3 + 0xd) = CONCAT44(uStack_108,uStack_10c);
  *(ulong *)(param_3 + 0xb) = CONCAT44(uStack_110,uStack_114);
  *(ulong *)(param_3 + 0x11) = CONCAT44(uStack_f8,uStack_fc);
  *(ulong *)(param_3 + 0xf) = CONCAT44(local_100,uStack_104);
  *(undefined8 *)(param_3 + 2) = uVar6;
  *(char **)(param_3 + 4) = "mismatch";
  param_3[6] = uVar7;
  local_f0 = uVar5;
LAB_0015cbe8:
  *(undefined8 *)(param_3 + 0x14) = uVar5;
  *(undefined8 *)(param_3 + 0x12) = uVar14;
LAB_0015cbec:
  uVar7 = 1;
  pcVar10 = "read";
  puVar4 = param_1;
  uVar12 = 0;
  do {
    (*(code *)param_2[2])(*param_2,*puVar4,*(undefined4 *)(puVar4 + 1));
    (*(code *)param_2[3])(*param_2,*puVar4);
    iVar3 = (*(code *)param_2[1])(*param_2,*puVar4,&local_164);
    if (iVar3 == 0) {
      uVar8 = 0;
LAB_0015cc80:
      uVar5 = param_1[(uVar12 & 0xffffffff) * 2];
      puVar4 = (undefined8 *)__emutls_get_address(&DAT_001a7c68);
      uStack_154 = (undefined4)puVar4[1];
      uStack_150 = (undefined4)((ulong)puVar4[1] >> 0x20);
      local_15c = (undefined4)*puVar4;
      uStack_158 = (undefined4)((ulong)*puVar4 >> 0x20);
      uStack_144 = (undefined4)puVar4[3];
      uStack_140 = (undefined4)((ulong)puVar4[3] >> 0x20);
      local_14c = (undefined4)puVar4[2];
      uStack_148 = (undefined4)((ulong)puVar4[2] >> 0x20);
      uVar6 = puVar4[5];
      uStack_134 = (undefined4)uVar6;
      uStack_130 = (undefined4)((ulong)uVar6 >> 0x20);
      local_13c = (undefined4)puVar4[4];
      uStack_138 = (undefined4)((ulong)puVar4[4] >> 0x20);
      local_12c = puVar4[6];
      *(ulong *)(param_3 + 0x1f) = CONCAT44(uStack_154,uStack_158);
      *(ulong *)(param_3 + 0x1d) = CONCAT44(local_15c,uStack_160);
      *(ulong *)(param_3 + 0x23) = CONCAT44(uStack_144,uStack_148);
      *(ulong *)(param_3 + 0x21) = CONCAT44(local_14c,uStack_150);
      param_3[0x16] = (uint)uVar12;
      *(ulong *)(param_3 + 0x27) = CONCAT44(uStack_134,uStack_138);
      *(ulong *)(param_3 + 0x25) = CONCAT44(local_13c,uStack_140);
      *(undefined8 *)(param_3 + 0x18) = uVar5;
      *(char **)(param_3 + 0x1a) = pcVar10;
      param_3[0x1c] = uVar8;
      *(undefined8 *)(param_3 + 0x2a) = local_12c;
      *(undefined8 *)(param_3 + 0x28) = uVar6;
      param_3[0x2c] = 1;
      break;
    }
    if (local_164 != *(uint *)(puVar4 + 1)) {
      pcVar10 = "mismatch";
      uVar8 = local_164;
      goto LAB_0015cc80;
    }
    uVar1 = uVar12 + 1;
    uVar7 = (uint)(uVar12 < uVar8);
    puVar4 = puVar4 + 2;
    uVar12 = uVar1;
  } while (uVar8 + 1 != uVar1);
  iVar3 = -uVar7;
LAB_0015ccec:
  if (*(long *)(lVar2 + 0x28) != local_70) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar3);
  }
  return;
}

/* ===== FUN_0015d078 @ 0015d078 ===== */

void FUN_0015d078(long param_1,int param_2,void *param_3)

{
  long lVar1;
  undefined *puVar2;
  undefined *puVar3;
  int iVar4;
  ulong __n;
  undefined8 local_270;
  undefined8 auStack_268 [64];
  long local_68;
  
  puVar2 = PTR_DAT_001a36c0;
  lVar1 = tpidr_el0;
  local_68 = *(long *)(lVar1 + 0x28);
  if ((((param_1 != 0) && (param_2 != 0)) &&
      (__n = (long)PTR_nexus_script_port_ui_server_thread_001a36c8 - (long)PTR_DAT_001a36c0,
      __n < 0x201 && __n != 0)) && ((param_3 != (void *)0x0 && (DAT_0022dd98 == 0)))) {
    local_270 = 0;
    iVar4 = FUN_00190d68(DAT_0022d8e0 + 0x5921a0,param_1,(long)&local_270 + 4);
    puVar3 = PTR_DAT_001a36d0;
    if ((iVar4 != 0) &&
       ((local_270._4_4_ == param_2 &&
        (iVar4 = FUN_00190d68(PTR_DAT_001a36d0 + (param_1 - (long)puVar2),DAT_0022d8e0 + 0x5921a4,
                              &local_270), iVar4 != 0)))) {
      __memcpy_chk(auStack_268,puVar2,__n,0x200);
      *(undefined4 *)(PTR_DAT_001a36d8 + ((long)auStack_268 - (long)puVar2)) = 0x52800002;
      *(undefined4 *)(PTR_DAT_001a36e0 + ((long)auStack_268 - (long)puVar2)) = 0xd100c3ff;
      *(undefined4 *)(puVar3 + ((long)auStack_268 - (long)puVar2)) = (undefined4)local_270;
      *(code **)(PTR_DAT_001a36e8 + ((long)auStack_268 - (long)puVar2)) = FUN_0015d224;
      iVar4 = memcmp(param_3,auStack_268,__n);
      if (iVar4 == 0) {
        DAT_0022db80 = param_1;
        DAT_0022db88 = param_2;
        DAT_0022db90 = __n;
        __memcpy_chk(&DAT_0022db98,param_3,__n,0x208);
        DAT_0022dd98 = 1;
      }
    }
  }
  if (*(long *)(lVar1 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_001612c8 @ 001612c8 ===== */

undefined8 FUN_001612c8(undefined4 *param_1,undefined4 *param_2,uint *param_3)

{
  ulong uVar1;
  undefined4 uVar2;
  char cVar3;
  undefined8 uVar4;
  ulong uVar5;
  int iVar6;
  
  uVar4 = 0;
  if (((param_1 != (undefined4 *)0x0) && (param_2 != (undefined4 *)0x0)) && (param_3 != (uint *)0x0)
     ) {
    uVar5 = FUN_00193f80(1,&DAT_0022f500);
    if ((uVar5 & 1) == 0) {
      do {
        iVar6 = DAT_0022f6c8 + -1;
        if (iVar6 == -1) {
          uVar4 = 0;
          goto LAB_00161398;
        }
        uVar5 = (ulong)DAT_0022f6c4;
        uVar1 = (ulong)DAT_0022f6c4;
        DAT_0022f6c4 = DAT_0022f6c4 + 1 & 0x1f;
        cVar3 = (&DAT_0022f6a4)[uVar1];
        uVar2 = (&DAT_0022f604)[uVar5];
        DAT_0022f6c8 = iVar6;
      } while (((&DAT_0022f684)[uVar1] == '\0') && ((&DAT_0022f584)[uVar5] != DAT_00233ed0));
      uVar4 = 1;
      *param_1 = (&DAT_0022f504)[uVar5];
      *param_2 = uVar2;
      *param_3 = (uint)(cVar3 != '\0');
      DAT_00233ee8 = 1;
LAB_00161398:
      DAT_0022f500 = 0;
    }
    else {
      uVar4 = 0;
    }
  }
  return uVar4;
}

/* ===== FUN_0016169c @ 0016169c ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0016169c(uint param_1,uint *param_2)

{
  int iVar1;
  long lVar2;
  undefined8 uVar3;
  int iVar4;
  undefined8 uVar5;
  undefined8 local_98;
  ulong uStack_90;
  undefined8 uStack_88;
  undefined8 uStack_80;
  undefined8 uStack_78;
  undefined8 local_70;
  undefined8 uStack_68;
  undefined8 local_60;
  undefined8 uStack_58;
  undefined8 uStack_50;
  undefined8 local_48;
  undefined8 uStack_40;
  long local_38;
  
  lVar2 = tpidr_el0;
  local_38 = *(long *)(lVar2 + 0x28);
  if (param_1 == 0x21036) {
    local_70 = 0;
    uStack_88 = 0;
    uStack_90 = 0;
    uStack_78 = 0;
    uStack_80 = 0;
    local_98 = DAT_0010f748;
    iVar4 = FUN_00191df8(&local_98);
    if ((((iVar4 != 0) && ((int)local_98 == 1)) && (local_98._4_4_ == 0x30)) &&
       (((int)uStack_90 != 0 && ((uint)uStack_88 < 6)))) {
      uVar5 = 1;
      DAT_0025ca80 = 2;
      _DAT_0025caf8 = uStack_90;
      _DAT_0025caf0 = local_98;
      _DAT_0025cb08 = uStack_80;
      _DAT_0025cb00 = uStack_88;
      uRam000000000025cb18 = local_70;
      _DAT_0025cb10 = uStack_78;
      *param_2 = 1;
      goto LAB_00161948;
    }
  }
  else {
    if (param_1 != 0x21031) {
      if (DAT_0025ca80 == 0) {
        uVar5 = 0;
        goto LAB_00161948;
      }
      if (param_1 != 0x82) {
        if (DAT_0025ca80 == 2) {
          if (param_1 == 0x2e010) {
LAB_001618a0:
            uVar5 = 1;
            DAT_0025ca80 = 0;
            *param_2 = 1;
            goto LAB_00161948;
          }
          if (param_1 - 0x2e020 < 6) {
            iVar4 = FUN_00164450();
            if (iVar4 != 0) {
              DAT_0025ca80 = 0;
            }
LAB_00161930:
            uVar5 = 1;
            *param_2 = (uint)(iVar4 != 0);
            goto LAB_00161948;
          }
        }
        else if (DAT_0025ca80 == 1) {
          if (param_1 == 0x2e000) goto LAB_001618a0;
          if (param_1 - 0x2e101 < 0x91) {
            iVar4 = param_1 - 0x2e100;
            goto LAB_00161880;
          }
          if ((param_1 & 0xfffffffe) == 0x2e002) {
            iVar1 = 0x91;
            if (param_1 != 0x2e003) {
              iVar1 = DAT_0025ca84;
            }
            iVar4 = 0;
            if (iVar1 != 0x91) {
              iVar4 = iVar1;
            }
            iVar4 = FUN_00191cd0(iVar4);
            if (iVar4 != 0) {
              DAT_0025ca84 = iVar1;
              DAT_0025ca88 = iVar1;
              FUN_00164398();
            }
            goto LAB_00161930;
          }
        }
      }
      uVar5 = 0;
      DAT_0025ca80 = 0;
      goto LAB_00161948;
    }
    uStack_40 = 0;
    uStack_78 = 0;
    uStack_80 = 0;
    uStack_68 = 0;
    local_70 = 0;
    uStack_58 = 0;
    local_60 = 0;
    local_48 = 0;
    uStack_50 = 0;
    uStack_88 = 0;
    uStack_90 = 0;
    local_98 = DAT_0010f850;
    iVar4 = FUN_00191c90(&local_98);
    if (((iVar4 != 0) && ((int)local_98 == 1)) &&
       ((local_98._4_4_ == 0x60 && ((uStack_90 & 1) != 0)))) {
      uVar5 = 1;
      uRam000000000025caa8 = uStack_80;
      _DAT_0025caa0 = uStack_88;
      uVar3 = _DAT_0025caa0;
      uRam000000000025cab8 = local_70;
      _DAT_0025cab0 = uStack_78;
      DAT_0025caa0 = (int)uStack_88;
      DAT_0025ca80 = 1;
      uRam000000000025ca98 = uStack_90;
      _DAT_0025ca90 = local_98;
      uRam000000000025cac8 = local_60;
      _DAT_0025cac0 = uStack_68;
      uRam000000000025cad8 = uStack_50;
      _DAT_0025cad0 = uStack_58;
      iVar4 = 0x91;
      if (DAT_0025caa0 != 0) {
        iVar4 = DAT_0025caa0;
      }
      uRam000000000025cae8 = uStack_40;
      _DAT_0025cae0 = local_48;
      DAT_0025ca88 = iVar4;
      _DAT_0025caa0 = uVar3;
      if (iVar4 - 0x92U < 0xffffff6f) {
        *param_2 = 4;
        DAT_0025ca80 = 0;
        goto LAB_00161948;
      }
LAB_00161880:
      uVar5 = 1;
      DAT_0025ca84 = iVar4;
      *param_2 = 1;
      goto LAB_00161948;
    }
  }
  uVar5 = 1;
  *param_2 = 0;
LAB_00161948:
  if (*(long *)(lVar2 + 0x28) != local_38) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar5);
  }
  return;
}

/* ===== FUN_00161970 @ 00161970 ===== */

int FUN_00161970(undefined4 param_1)

{
  long lVar1;
  bool bVar2;
  long *plVar3;
  char cVar4;
  char cVar5;
  int iVar6;
  long *local_50;
  long local_48;
  
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  local_50 = (long *)0x0;
  if (DAT_0022f4c8 != (long *)0x0 && DAT_0022f4e8 != 0) {
    iVar6 = (**(code **)(*DAT_0022f4c8 + 0x30))(DAT_0022f4c8,&local_50,0x10006);
    if (iVar6 == 0) {
      bVar2 = true;
      plVar3 = local_50;
    }
    else {
      if ((iVar6 != -2) ||
         (iVar6 = (**(code **)(*DAT_0022f4c8 + 0x20))(DAT_0022f4c8,&local_50,0), iVar6 != 0))
      goto LAB_00161a08;
      bVar2 = false;
      plVar3 = local_50;
    }
    local_50 = plVar3;
    if (plVar3 != (long *)0x0) {
      cVar4 = (**(code **)(*plVar3 + 0x3a8))(plVar3,DAT_0022f4e8,DAT_0022f4d8,param_1);
      cVar5 = (**(code **)(*plVar3 + 0x720))(plVar3);
      if (cVar5 == '\0') {
        iVar6 = (uint)(cVar4 == '\x01') << 1;
      }
      else {
        (**(code **)(*plVar3 + 0x88))(plVar3);
        iVar6 = -1;
      }
      if (!bVar2) {
        (**(code **)(*DAT_0022f4c8 + 0x28))();
      }
      DAT_0025cb20 = 0;
      goto LAB_00161a0c;
    }
  }
LAB_00161a08:
  iVar6 = 0;
LAB_00161a0c:
  if (*(long *)(lVar1 + 0x28) != local_48) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return iVar6;
}

/* ===== FUN_00162260 @ 00162260 ===== */

void FUN_00162260(long param_1)

{
  uint uVar1;
  long lVar2;
  ulong uVar3;
  ulong uVar4;
  undefined8 *puVar5;
  undefined1 auStack_6f8 [4];
  int local_6f4;
  undefined2 local_6f0;
  undefined8 local_6e8;
  undefined8 uStack_6e0;
  undefined8 uStack_6d8;
  undefined8 local_6d0;
  undefined8 local_6c8;
  undefined8 uStack_6c0;
  undefined8 uStack_6b8;
  undefined8 uStack_6b0;
  long local_68;
  
  lVar2 = tpidr_el0;
  local_68 = *(long *)(lVar2 + 0x28);
  if ((((*(char *)(param_1 + 9) != '\0') && (*(char *)(param_1 + 10) == '\0')) &&
      (uVar1 = *(byte *)(param_1 + 8) - 0x43, uVar1 < 0x32)) &&
     (((1L << ((ulong)uVar1 & 0x3f) & 0x22006000cd001U) != 0 &&
      (uVar3 = (ulong)*(uint *)(param_1 + 4), *(uint *)(param_1 + 4) != 0)))) {
    uVar4 = 0;
    puVar5 = (undefined8 *)(param_1 + 0x28);
    do {
      if (*(int *)(puVar5 + -3) == 0x20002) {
        memset(auStack_6f8,0,0x690);
        local_6f0 = 0x14f;
        FUN_00162118(auStack_6f8);
        if (local_6f4 == 1) {
          local_6d0 = *puVar5;
          puVar5[-2] = uStack_6e0;
          puVar5[-3] = local_6e8;
          *puVar5 = local_6d0;
          puVar5[-1] = uStack_6d8;
          puVar5[2] = uStack_6c0;
          puVar5[1] = local_6c8;
          puVar5[4] = uStack_6b0;
          puVar5[3] = uStack_6b8;
        }
        uVar3 = (ulong)*(uint *)(param_1 + 4);
      }
      uVar4 = uVar4 + 1;
      puVar5 = puVar5 + 8;
    } while (uVar4 < uVar3);
  }
  if (*(long *)(lVar2 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00163eb8 @ 00163eb8 ===== */

void FUN_00163eb8(undefined8 param_1,char *param_2)

{
  long lVar1;
  int iVar2;
  size_t sVar3;
  ulong uVar4;
  ulong uVar5;
  long lVar6;
  ulong local_80;
  ulong local_78;
  byte local_6c [4];
  char local_68 [4];
  char local_64 [4];
  undefined1 auStack_60 [24];
  long local_48;
  
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  if ((param_2 != (char *)0x0) && (sVar3 = strlen(param_2), sVar3 < 0x81)) {
    local_6c[0] = 0;
    local_64[0] = '\x01';
    local_68[0] = '\x01';
    iVar2 = FUN_0014e7c4(sVar3,DAT_0022d8e0 + 0x12eb0e8,local_64,1);
    uVar5 = 0;
    if ((iVar2 == 0) || (local_64[0] != '\0')) goto LAB_00163ef8;
    iVar2 = FUN_0014e7c4(0,DAT_0022d8e0 + 0x12eb170,local_68,1);
    uVar5 = 0;
    if ((((iVar2 == 0) || (local_68[0] != '\0')) ||
        (uVar4 = (*(code *)(DAT_0022d8e0 + 0x51eacc))("sc/ui.sc",0), uVar5 = uVar4, uVar4 == 0)) ||
       (uVar5 = FUN_0014e7c4(uVar4,uVar4 + 0x80,local_6c,1), (int)uVar5 == 0)) goto LAB_00163ef8;
    if ((((local_6c[0] & 1) != 0) && (lVar6 = FUN_00154cf0(uVar4 + 0x88), lVar6 != 0)) &&
       (lVar6 = FUN_00154cf0(lVar6 + 8), lVar6 != 0)) {
      uVar5 = FUN_00154cf0(lVar6 + 0xf8);
      local_80 = 0;
      local_78 = 0;
      if (uVar5 != 0) {
        iVar2 = FUN_0014e7c4(uVar5,lVar6 + 0x100,&local_78,8);
        uVar5 = 0;
        if (((iVar2 != 0) && (local_78 != 0)) && (local_78 < 0x10001)) {
          iVar2 = FUN_0014e7c4(0,lVar6 + 0x110,&local_80,8);
          uVar5 = 0;
          if ((((iVar2 != 0) && (local_80 != 0)) && (local_80 < 0x10001)) &&
             (uVar5 = FUN_00190fd0(param_2,auStack_60), (int)uVar5 != 0)) {
            lVar6 = (*(code *)(DAT_0022d8e0 + 0x5dfbf8))(lVar6 + 0xf8,auStack_60);
            uVar5 = (ulong)(lVar6 != 0);
          }
        }
      }
      goto LAB_00163ef8;
    }
  }
  uVar5 = 0;
LAB_00163ef8:
  if (*(long *)(lVar1 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar5);
}

/* ===== FUN_00164090 @ 00164090 ===== */

void FUN_00164090(long param_1)

{
  ulong uVar1;
  long lVar2;
  int iVar3;
  ulong uVar4;
  char local_50 [4];
  int local_4c;
  long local_48;
  long local_40;
  long local_38;
  ulong local_30;
  long local_28;
  
  lVar2 = tpidr_el0;
  local_28 = *(long *)(lVar2 + 0x28);
  local_40 = 1;
  local_38 = 1;
  local_48 = 1;
  local_4c = 0;
  local_50[0] = '\x01';
  local_30 = 0;
  iVar3 = FUN_0014e7c4(param_1,param_1,&local_30,8);
  uVar4 = 0;
  uVar1 = local_30;
  if (((local_30 & 7) != 0 || local_30 < 0x1000) || iVar3 == 0) {
    uVar1 = 0;
  }
  if (uVar1 == DAT_0022d8e0 + 0x11c0a48U) {
    iVar3 = FUN_0014e7c4(0,param_1 + 0x38,&local_38,8);
    uVar4 = 0;
    if ((iVar3 != 0) && (local_38 == 0)) {
      iVar3 = FUN_0014e7c4(0,param_1 + 0x40,&local_4c,4);
      uVar4 = 0;
      if ((iVar3 != 0) && (local_4c == -1)) {
        iVar3 = FUN_0014e7c4(0,param_1 + 0xa8,&local_40,8);
        uVar4 = 0;
        if ((iVar3 != 0) && (local_40 == 0)) {
          iVar3 = FUN_0014e7c4(0,param_1 + 0x120,&local_48,8);
          uVar4 = 0;
          if ((iVar3 != 0) &&
             ((local_48 == 0 &&
              (uVar4 = FUN_0014e7c4(0,param_1 + 0x1f0,local_50,1), (int)uVar4 != 0)))) {
            uVar4 = (ulong)(local_50[0] == '\0');
          }
        }
      }
    }
  }
  if (*(long *)(lVar2 + 0x28) == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar4);
}

/* ===== FUN_00164398 @ 00164398 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00164398(void)

{
  long lVar1;
  int iVar2;
  undefined8 local_88;
  ulong uStack_80;
  undefined8 uStack_78;
  undefined8 uStack_70;
  undefined8 uStack_68;
  undefined8 uStack_60;
  undefined8 uStack_58;
  undefined8 local_50;
  undefined8 uStack_48;
  undefined8 uStack_40;
  undefined8 local_38;
  undefined8 uStack_30;
  long local_28;
  
  lVar1 = tpidr_el0;
  local_28 = *(long *)(lVar1 + 0x28);
  uStack_30 = 0;
  uStack_68 = 0;
  uStack_70 = 0;
  uStack_58 = 0;
  uStack_60 = 0;
  uStack_48 = 0;
  local_50 = 0;
  local_38 = 0;
  uStack_40 = 0;
  uStack_78 = 0;
  uStack_80 = 0;
  local_88 = DAT_0010f850;
  iVar2 = FUN_00191c90(&local_88);
  if ((((iVar2 != 0) && ((int)local_88 == 1)) && (local_88._4_4_ == 0x60)) && ((uStack_80 & 1) != 0)
     ) {
    uRam000000000025cab8 = uStack_60;
    _DAT_0025cab0 = uStack_68;
    uRam000000000025cac8 = local_50;
    _DAT_0025cac0 = uStack_58;
    uRam000000000025cad8 = uStack_40;
    _DAT_0025cad0 = uStack_48;
    uRam000000000025cae8 = uStack_30;
    _DAT_0025cae0 = local_38;
    uRam000000000025ca98 = uStack_80;
    _DAT_0025ca90 = local_88;
    uRam000000000025caa8 = uStack_70;
    _DAT_0025caa0 = uStack_78;
  }
  if (*(long *)(lVar1 + 0x28) == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_001645e0 @ 001645e0 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_001645e0(uint param_1,undefined8 param_2)

{
  byte *pbVar1;
  char *pcVar2;
  uint uVar3;
  uint uVar4;
  undefined4 uVar5;
  long lVar6;
  __pid_t _Var7;
  int iVar8;
  int iVar9;
  int iVar10;
  undefined8 uVar11;
  int *piVar12;
  long lVar13;
  undefined4 *puVar14;
  long lVar15;
  ulong uVar16;
  byte bVar17;
  uint uVar18;
  byte bVar21;
  byte bVar22;
  byte bVar23;
  undefined1 auVar19 [16];
  undefined1 auVar20 [16];
  char acStack_d0 [80];
  byte local_80 [16];
  uint local_70;
  long local_68;
  
  iVar10 = DAT_001a7c50;
  lVar6 = tpidr_el0;
  local_68 = *(long *)(lVar6 + 0x28);
  if (1 < param_1) {
    uVar11 = 4;
    goto LAB_00164618;
  }
  uVar11 = 0;
  if (((((DAT_0022d880._4_4_ & 1) != 0) || ((DAT_0022d888 & 1) != 0)) ||
      (uVar11 = 2, (DAT_0022d88c & 1) != 0)) || ((DAT_0022d890 & 1) != 0)) goto LAB_00164618;
  if ((((uint)DAT_0022d880 == param_1) && (DAT_0022dfbc == 0)) &&
     ((DAT_0022d894 == 0 && (((DAT_0022dfb8 & 1) == 0 && (DAT_0022f4b8 == 0)))))) {
    uVar11 = 1;
    goto LAB_00164618;
  }
  if (DAT_0025cb28 != -1) {
    lVar13 = DAT_0025cb28 + 1;
    DAT_0025cb28 = lVar13;
    if (-1 < DAT_001a7c50) {
      local_80[0xd] = (char)(param_1 >> 8);
      local_80[8] = 1;
      local_80[9] = 0;
      local_80[10] = 0;
      local_80[0xb] = 0;
      lVar15 = 0;
      local_80[0] = 0x4e;
      local_80[1] = 0x58;
      local_80[2] = 0x50;
      local_80[3] = 0x36;
      local_80[4] = 0x39;
      local_80[5] = 0x32;
      local_80[6] = 0x35;
      local_80[7] = 0x32;
      local_80[0xe] = (char)(param_1 >> 0x10);
      local_70 = 0xffffffff;
      local_80[0xc] = (char)param_1;
      local_80[0xf] = (char)(param_1 >> 0x18);
      do {
        pbVar1 = local_80 + lVar15;
        lVar15 = lVar15 + 1;
        uVar3 = local_70 ^ *pbVar1;
        uVar4 = -(uVar3 & 1) & 0xedb88320 ^ uVar3 >> 1;
        bVar17 = (byte)uVar3;
        auVar19[0] = bVar17 & (byte)_DAT_0010f9c0;
        bVar21 = (byte)(local_70 >> 8);
        auVar19[1] = bVar21 & (byte)((ulong)_DAT_0010f9c0 >> 8);
        bVar22 = (byte)(local_70 >> 0x10);
        auVar19[2] = bVar22 & (byte)((ulong)_DAT_0010f9c0 >> 0x10);
        bVar23 = (byte)(local_70 >> 0x18);
        auVar19[3] = bVar23 & (byte)((ulong)_DAT_0010f9c0 >> 0x18);
        auVar19[4] = bVar17 & (byte)((ulong)_DAT_0010f9c0 >> 0x20);
        auVar19[5] = bVar21 & (byte)((ulong)_DAT_0010f9c0 >> 0x28);
        auVar19[6] = bVar22 & (byte)((ulong)_DAT_0010f9c0 >> 0x30);
        auVar19[7] = bVar23 & (byte)((ulong)_DAT_0010f9c0 >> 0x38);
        auVar19[8] = bVar17 & (byte)_UNK_0010f9c8;
        auVar19[9] = bVar21 & (byte)((ulong)_UNK_0010f9c8 >> 8);
        auVar19[10] = bVar22 & (byte)((ulong)_UNK_0010f9c8 >> 0x10);
        auVar19[0xb] = bVar23 & (byte)((ulong)_UNK_0010f9c8 >> 0x18);
        auVar19[0xc] = bVar17 & (byte)((ulong)_UNK_0010f9c8 >> 0x20);
        auVar19[0xd] = bVar21 & (byte)((ulong)_UNK_0010f9c8 >> 0x28);
        auVar19[0xe] = bVar22 & (byte)((ulong)_UNK_0010f9c8 >> 0x30);
        auVar19[0xf] = bVar23 & (byte)((ulong)_UNK_0010f9c8 >> 0x38);
        uVar3 = (int)(uVar3 << 0x1e) >> 0x1f & 0xedb88320U ^ uVar4 >> 1;
        auVar19 = NEON_cmeq(auVar19,0,2);
        auVar20[0] = (byte)_DAT_0010fac0 & ~auVar19[0];
        auVar20[1] = (byte)((ulong)_DAT_0010fac0 >> 8) & ~auVar19[1];
        auVar20[2] = (byte)((ulong)_DAT_0010fac0 >> 0x10) & ~auVar19[2];
        auVar20[3] = (byte)((ulong)_DAT_0010fac0 >> 0x18) & ~auVar19[3];
        auVar20[4] = (byte)((ulong)_DAT_0010fac0 >> 0x20) & ~auVar19[4];
        auVar20[5] = (byte)((ulong)_DAT_0010fac0 >> 0x28) & ~auVar19[5];
        auVar20[6] = (byte)((ulong)_DAT_0010fac0 >> 0x30) & ~auVar19[6];
        auVar20[7] = (byte)((ulong)_DAT_0010fac0 >> 0x38) & ~auVar19[7];
        auVar20[8] = (byte)_UNK_0010fac8 & ~auVar19[8];
        auVar20[9] = (byte)((ulong)_UNK_0010fac8 >> 8) & ~auVar19[9];
        auVar20[10] = (byte)((ulong)_UNK_0010fac8 >> 0x10) & ~auVar19[10];
        auVar20[0xb] = (byte)((ulong)_UNK_0010fac8 >> 0x18) & ~auVar19[0xb];
        auVar20[0xc] = (byte)((ulong)_UNK_0010fac8 >> 0x20) & ~auVar19[0xc];
        auVar20[0xd] = (byte)((ulong)_UNK_0010fac8 >> 0x28) & ~auVar19[0xd];
        auVar20[0xe] = (byte)((ulong)_UNK_0010fac8 >> 0x30) & ~auVar19[0xe];
        auVar20[0xf] = (byte)((ulong)_UNK_0010fac8 >> 0x38) & ~auVar19[0xf];
        auVar19 = NEON_ext(auVar20,auVar20,8,1);
        uVar18 = CONCAT13(auVar20[3] ^ auVar19[3],
                          CONCAT12(auVar20[2] ^ auVar19[2],
                                   CONCAT11(auVar20[1] ^ auVar19[1],auVar20[0] ^ auVar19[0])));
        local_70 = uVar18 ^ (int)(uVar4 << 0x1a) >> 0x1f & 0x76dc4190U ^
                   (int)(uVar3 << 0x1a) >> 0x1f & 0xedb88320U ^ uVar3 >> 6 ^
                   (uint)(CONCAT17(auVar20[7] ^ auVar19[7],
                                   CONCAT16(auVar20[6] ^ auVar19[6],
                                            CONCAT15(auVar20[5] ^ auVar19[5],
                                                     CONCAT14(auVar20[4] ^ auVar19[4],uVar18)))) >>
                         0x20);
      } while (lVar15 != 0x10);
      local_70 = ~local_70;
      _Var7 = getpid();
      FUN_0015540c(acStack_d0,0x50,0x50,"performance-%d-%llu.tmp",_Var7,lVar13);
      iVar8 = openat(iVar10,acStack_d0,0x880c1,0x180);
      if (-1 < iVar8) {
        uVar16 = 0;
        do {
          lVar13 = __write_chk(iVar8,local_80 + uVar16,0x14 - uVar16,0xffffffffffffffff);
          if (lVar13 < 0) {
            piVar12 = (int *)__errno();
            if (*piVar12 != 4) goto LAB_001648e4;
          }
          else {
            uVar16 = lVar13 + uVar16;
            if (lVar13 == 0) break;
          }
        } while (uVar16 < 0x14);
        if (uVar16 == 0x14) {
          iVar9 = fsync(iVar8);
          iVar8 = close(iVar8);
          if (iVar8 == 0 && iVar9 == 0) {
            iVar8 = renameat(iVar10,acStack_d0,iVar10,"performance.bin");
            if (iVar8 == 0) {
              iVar10 = fsync(iVar10);
              DAT_0022d898 = 0;
              _DAT_0022d8a0 = 0;
              DAT_0022dfbc = (uint)(iVar10 != 0);
              DAT_0022d880._0_4_ = param_1;
              if (iVar10 == 0) {
                DAT_0022f4b8 = 0;
                DAT_0022d894 = 0;
                DAT_0022dfb8 = 0;
                pcVar2 = "saved_optimization_off";
                if (param_1 != 0) {
                  pcVar2 = "saved_optimization_on";
                }
                DAT_0025cb30 = 0;
                DAT_0022d88c = 1;
                DAT_0022d8a8 = param_2;
                FUN_00156760(0,"performance_mode",pcVar2,0x20002);
                uVar11 = 2;
                goto LAB_00164618;
              }
              FUN_00156760(iVar10,"performance_mode","visible_save_not_confirmed",0x20002);
              goto LAB_0016490c;
            }
          }
        }
        else {
LAB_001648e4:
          iVar8 = close(iVar8);
        }
        puVar14 = (undefined4 *)__errno(iVar8);
        uVar5 = *puVar14;
        unlinkat(iVar10,acStack_d0,0);
        *puVar14 = uVar5;
      }
    }
  }
LAB_0016490c:
  uVar11 = 0xffffffff;
LAB_00164618:
  if (*(long *)(lVar6 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar11);
}

/* ===== FUN_00164978 @ 00164978 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00164978(ulong *param_1)

{
  long lVar1;
  uint uVar2;
  long lVar3;
  __time_t _Var4;
  bool bVar5;
  bool bVar6;
  int iVar7;
  undefined8 uVar8;
  long lVar9;
  ulong uVar10;
  ulong uVar11;
  ulong *puVar12;
  ulong uVar13;
  ulong uVar14;
  uint *puVar15;
  uint uVar16;
  uint uVar17;
  ulong uVar18;
  long local_138;
  ushort local_124 [2];
  timespec local_120 [10];
  undefined8 local_80;
  long local_78;
  long local_70;
  
  lVar3 = tpidr_el0;
  local_70 = *(long *)(lVar3 + 0x28);
  if ((DAT_002815d8 != param_1[2]) || (DAT_00281600 != param_1[1])) {
    memset(&DAT_002815d8,0,0x338);
    DAT_00281600 = param_1[1];
    DAT_002815d8 = param_1[2];
    DAT_00281a18 = 0;
  }
  if ((((int)param_1[4] != 0) && (*(int *)((long)param_1 + 0x2c) == 0)) &&
     (DAT_00281a18 <= *param_1)) {
    DAT_00281a18 = *param_1 + 0x32;
    uVar8 = FUN_001668ec(param_1);
    uVar13 = DAT_002815e8;
    if (DAT_002815e8 == 0) {
      uVar13 = param_1[2];
    }
    if ((DAT_002815e0 != uVar13) || (DAT_00281608 == 0)) {
      DAT_00281608 = 1;
      local_120[0].tv_sec = 0;
      DAT_002815e0 = uVar13;
      iVar7 = FUN_0014e7c4(uVar8,uVar13 + 0x38,local_120,8);
      _DAT_00281620 = 0;
      DAT_00281610 = uVar13;
      DAT_00281618 = local_120[0].tv_sec;
      if (((local_120[0].tv_sec & 7U) != 0 || (ulong)local_120[0].tv_sec < 0x1000) || iVar7 == 0) {
        DAT_00281618 = 0;
      }
    }
    iVar7 = clock_gettime(1,local_120);
    if (iVar7 == 0) {
      local_138 = (ulong)local_120[0].tv_nsec / 1000000 + local_120[0].tv_sec * 1000;
    }
    else {
      local_138 = 0;
    }
    if (DAT_00281608 != 0) {
      uVar16 = 0;
      do {
        if ((uVar16 != 0) && ((uVar16 & 7) == 0)) {
          iVar7 = clock_gettime(1,local_120);
          if (iVar7 == 0) {
            uVar10 = (ulong)local_120[0].tv_nsec / 1000000 + local_120[0].tv_sec * 1000;
          }
          else {
            uVar10 = 0;
          }
          if ((local_138 - 1U < uVar10) && (1 < uVar10 - local_138)) break;
        }
        uVar2 = DAT_00281608 - 1;
        uVar10 = (&DAT_00281610)[(ulong)uVar2 * 3];
        if (uVar10 == param_1[2]) {
LAB_00164c74:
          uVar8 = FUN_0015516c(uVar10,param_1[2]);
          if ((int)uVar8 == 0) goto LAB_00164b0c;
          local_124[0] = 0;
          if (*(int *)(&DAT_00281624 + (ulong)uVar2 * 0x18) == 0) {
            bVar5 = DAT_0022d8e8 == '\x01';
            *(int *)(&DAT_00281624 + (ulong)uVar2 * 0x18) = 1;
            if (((bVar5) && ((int)param_1[4] != 0)) &&
               (uVar8 = FUN_00154eb4(uVar10,param_1[2]), (int)uVar8 != 0)) {
              local_80 = 0;
              uVar8 = FUN_0014e7c4(uVar8,uVar10 + 0x248,&local_80,8);
              bVar5 = (int)uVar8 != 0;
              bVar6 = (local_80 & 7) != 0;
              uVar14 = local_80;
              if ((!bVar5 || local_80 < 0x1000) || bVar6) {
                uVar14 = 0;
              }
              if ((bVar5 && 0xfff < local_80) && !bVar6) {
                if (DAT_0022db18 == uVar10) {
                  uVar18 = param_1[2];
                  if ((DAT_0022db20 == uVar18) && (DAT_0022db28 == uVar14)) goto LAB_00164d78;
                }
                else {
                  uVar18 = param_1[2];
                }
                DAT_0022db30 = param_1[1];
                DAT_0022db38._0_4_ =
                     (uint)((DAT_0022f1c0 != 0 && DAT_0022f1b0 == uVar18) &&
                           DAT_0022f1b8 == DAT_0022db30);
                DAT_0022db18 = uVar10;
                DAT_0022db20 = uVar18;
                DAT_0022db28 = uVar14;
              }
            }
LAB_00164d78:
            local_80 = 0;
            uVar8 = FUN_0014e7c4(uVar8,uVar10,&local_80,8);
            uVar14 = local_80;
            if (((local_80 & 7) != 0 || local_80 < 0x1000) || (int)uVar8 == 0) {
              uVar14 = 0;
            }
            if (((uVar14 == DAT_0022d8e0 + 0x11c7b88U) &&
                (uVar8 = FUN_00154eb4(uVar10,param_1[2]), (int)uVar8 != 0)) &&
               (uVar8 = FUN_00166d64(param_1,uVar10), (int)uVar8 != 0)) {
              if (DAT_002815e8 != uVar10) {
                DAT_002815f0 = 0;
                DAT_002815f8 = 0;
                DAT_002815e8 = uVar10;
              }
              DAT_00281600 = param_1[1];
              DAT_002815d8 = param_1[2];
              uVar8 = FUN_001668ec(param_1);
              if (uVar13 != uVar10) {
                DAT_00281608 = 0;
                break;
              }
            }
            local_80 = 0;
            lVar9 = FUN_0014e7c4(uVar8,uVar10,&local_80,8);
            uVar14 = local_80;
            if (((local_80 & 7) != 0 || local_80 < 0x1000) || (int)lVar9 == 0) {
              uVar14 = 0;
            }
            if (((uVar14 == DAT_0022d8e0 + 0x11c0a48U) && (uVar10 + 0x208 != 0)) &&
               (lVar9 = FUN_0014e7c4(lVar9,uVar10 + 0x208,&local_80,0x10), (int)lVar9 != 0)) {
              uVar14 = (ulong)local_80._4_4_;
              if (0xffffff60 < local_80._4_4_ - 0xa0) {
                lVar1 = uVar10 + 0x210;
                if (7 < local_80._4_4_) {
                  lVar1 = local_78;
                }
                if (((lVar1 != 0) &&
                    (lVar9 = FUN_0014e7c4(lVar9,lVar1,local_120,uVar14), (int)lVar9 != 0)) &&
                   (lVar9 = __memchr_chk(local_120,0,uVar14,0xa0), lVar9 == 0)) {
                  *(undefined1 *)((long)&local_120[0].tv_sec + uVar14) = 0;
                  lVar9 = FUN_00165224(param_1,uVar10,local_120);
                }
              }
            }
            local_80 = 0;
            lVar9 = FUN_0014e7c4(lVar9,uVar10,&local_80,8);
            uVar14 = local_80;
            if (((local_80 & 7) != 0 || local_80 < 0x1000) || (int)lVar9 == 0) {
              uVar14 = 0;
            }
            if (((uVar14 == DAT_0022d8e0 + 0x11abad8U) && (uVar10 + 0xe0 != 0)) &&
               (lVar9 = FUN_0014e7c4(lVar9,uVar10 + 0xe0,&local_80,0x10), (int)lVar9 != 0)) {
              uVar14 = (ulong)local_80._4_4_;
              if (0xffffff60 < local_80._4_4_ - 0xa0) {
                lVar1 = uVar10 + 0xe8;
                if (7 < local_80._4_4_) {
                  lVar1 = local_78;
                }
                if (((lVar1 != 0) &&
                    (lVar9 = FUN_0014e7c4(lVar9,lVar1,local_120,uVar14), (int)lVar9 != 0)) &&
                   (lVar9 = __memchr_chk(local_120,0,uVar14,0xa0), lVar9 == 0)) {
                  *(undefined1 *)((long)&local_120[0].tv_sec + uVar14) = 0;
                  lVar9 = FUN_00165360(param_1,uVar10,local_120);
                }
              }
            }
            uVar8 = FUN_0014e7c4(lVar9,uVar10 + 0x4e,local_124,2);
            if (((int)uVar8 != 0) && (local_124[0] < 0x201)) {
              (&DAT_00281620)[(ulong)uVar2 * 6] = (uint)local_124[0];
            }
          }
          puVar15 = &DAT_00281620 + (ulong)uVar2 * 6;
          if ((((*puVar15 == 0) || (0x1f < DAT_00281608)) ||
              (uVar8 = FUN_0014e7c4(uVar8,uVar10 + 0x4e,local_124,2), (int)uVar8 == 0)) ||
             ((local_124[0] == 0 || (0x200 < local_124[0])))) {
LAB_001650f0:
            DAT_00281608 = DAT_00281608 - 1;
          }
          else {
            if ((uint)local_124[0] < *puVar15) {
              *puVar15 = (uint)local_124[0];
            }
            local_120[0].tv_sec = 0;
            uVar8 = FUN_0014e7c4(uVar8,uVar10 + 0x50,local_120,8);
            _Var4 = local_120[0].tv_sec;
            if ((((int)uVar8 == 0) || ((ulong)local_120[0].tv_sec < 0x1000)) ||
               ((local_120[0].tv_sec & 7U) != 0)) goto LAB_001650f0;
            uVar2 = *puVar15;
            local_120[0].tv_sec = 0;
            *puVar15 = uVar2 - 1;
            iVar7 = FUN_0014e7c4(uVar8,_Var4 + (ulong)(uVar2 - 1) * 8,local_120,8);
            bVar5 = (local_120[0].tv_sec & 7U) != 0;
            uVar14 = local_120[0].tv_sec;
            if ((iVar7 == 0 || (ulong)local_120[0].tv_sec < 0x1000) || bVar5) {
              uVar14 = 0;
            }
            if (((iVar7 != 0 && 0xfff < (ulong)local_120[0].tv_sec) && !bVar5) &&
               (iVar7 = FUN_00154d5c(uVar14,uVar10), iVar7 != 0)) {
              uVar18 = (ulong)DAT_00281608;
              if (DAT_00281608 == 0) {
                uVar18 = 0;
              }
              else {
                puVar12 = &DAT_00281610;
                uVar11 = uVar18;
                do {
                  if (*puVar12 == uVar14) goto joined_r0x00165100;
                  puVar12 = puVar12 + 3;
                  uVar11 = uVar11 - 1;
                } while (uVar11 != 0);
              }
              DAT_00281608 = DAT_00281608 + 1;
              (&DAT_00281610)[uVar18 * 3] = uVar14;
              (&DAT_00281618)[uVar18 * 3] = uVar10;
              *(undefined8 *)(&DAT_00281620 + uVar18 * 6) = 0;
            }
          }
        }
        else {
          uVar8 = FUN_00154d5c(uVar10,(&DAT_00281618)[(ulong)uVar2 * 3]);
          if ((int)uVar8 != 0) {
            uVar18 = param_1[2];
            uVar14 = uVar10;
            if ((uVar10 != 0) && (uVar10 != uVar18)) {
              uVar17 = 0;
              do {
                local_120[0].tv_sec = 0;
                iVar7 = FUN_0014e7c4(uVar8,uVar14 + 0x38,local_120,8);
                _Var4 = local_120[0].tv_sec;
                bVar5 = (local_120[0].tv_sec & 7U) != 0;
                uVar11 = local_120[0].tv_sec;
                if ((iVar7 == 0 || (ulong)local_120[0].tv_sec < 0x1000) || bVar5) {
                  uVar11 = 0;
                }
                uVar8 = FUN_00154d5c(uVar14,uVar11);
              } while (((((int)uVar8 != 0) && (uVar14 = uVar11, uVar17 < 0x1f)) &&
                       ((iVar7 != 0 && 0xfff < (ulong)_Var4) && !bVar5)) &&
                      (uVar17 = uVar17 + 1, uVar11 != uVar18));
            }
            if (uVar14 == uVar18) goto LAB_00164c74;
          }
LAB_00164b0c:
          DAT_00281608 = DAT_00281608 - 1;
        }
joined_r0x00165100:
        if ((0x7e < uVar16) || (uVar16 = uVar16 + 1, DAT_00281608 == 0)) break;
      } while( true );
    }
  }
  if (*(long *)(lVar3 + 0x28) == local_70) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0016515c @ 0016515c ===== */

void FUN_0016515c(long param_1,void *param_2)

{
  long lVar1;
  long lVar2;
  long lVar3;
  void *pvVar4;
  ulong __n;
  undefined1 auStack_48 [4];
  uint local_44;
  long local_40;
  long local_38;
  
  lVar2 = tpidr_el0;
  local_38 = *(long *)(lVar2 + 0x28);
  lVar3 = param_1;
  if ((param_1 != 0) && (lVar3 = FUN_0014e7c4(param_1,param_1,auStack_48,0x10), (int)lVar3 != 0)) {
    __n = (ulong)local_44;
    if (0xffffff60 < local_44 - 0xa0) {
      lVar1 = param_1 + 8;
      if (7 < local_44) {
        lVar1 = local_40;
      }
      if (lVar1 != 0) {
        lVar3 = FUN_0014e7c4(lVar3,lVar1,param_2,__n);
        if ((int)lVar3 != 0) {
          pvVar4 = memchr(param_2,0,__n);
          lVar3 = 0;
          if (pvVar4 == (void *)0x0) {
            lVar3 = 1;
            *(undefined1 *)((long)param_2 + __n) = 0;
          }
        }
        goto LAB_001651fc;
      }
    }
    lVar3 = 0;
  }
LAB_001651fc:
  if (*(long *)(lVar2 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(lVar3);
}

/* ===== FUN_00165224 @ 00165224 ===== */

void FUN_00165224(long param_1,long param_2,undefined8 param_3)

{
  long lVar1;
  int iVar2;
  long lVar3;
  undefined1 auStack_9c [4];
  long local_98 [10];
  long local_48;
  
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  iVar2 = FUN_0014d544(param_3,auStack_9c);
  if (iVar2 == 2) {
    if ((((*(int *)(param_1 + 0x24) == 0) || (*(long *)(param_1 + 0x18) != 0)) ||
        (lVar3 = FUN_00167218(param_2,*(undefined8 *)(param_1 + 0x10)), lVar3 != 0)) ||
       (iVar2 = FUN_00166430(param_2,*(undefined8 *)(param_1 + 0x10),local_98), iVar2 == 0))
    goto LAB_0016531c;
  }
  else {
    if (iVar2 != 1) goto LAB_0016531c;
    lVar3 = FUN_00167218(param_2,*(undefined8 *)(param_1 + 0x10));
    iVar2 = FUN_00165944(lVar3,*(undefined8 *)(param_1 + 0x10),local_98);
    if (((iVar2 == 0) || (local_98[0] != param_2)) ||
       (iVar2 = FUN_00166d64(param_1,lVar3), iVar2 == 0)) goto LAB_0016531c;
    if (DAT_002815e8 != lVar3) {
      DAT_002815f0 = 0;
      DAT_002815f8 = 0;
      DAT_002815e8 = lVar3;
    }
    DAT_00281600 = *(undefined8 *)(param_1 + 8);
    DAT_002815d8 = *(undefined8 *)(param_1 + 0x10);
  }
  FUN_0014dca4(&DAT_0022d8f0,param_1,param_3,local_98);
LAB_0016531c:
  if (*(long *)(lVar1 + 0x28) != local_48) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

/* ===== FUN_00165360 @ 00165360 ===== */

void FUN_00165360(long param_1,ulong param_2,undefined8 param_3)

{
  ulong uVar1;
  long lVar2;
  ulong uVar3;
  bool bVar4;
  int iVar5;
  int iVar6;
  undefined8 uVar7;
  long lVar8;
  ulong *puVar9;
  ulong *puVar10;
  ulong uVar11;
  ulong uVar12;
  uint uVar13;
  ulong local_c8 [9];
  ulong local_80;
  undefined1 auStack_74 [4];
  ulong local_70;
  long local_68;
  
  lVar2 = tpidr_el0;
  local_68 = *(long *)(lVar2 + 0x28);
  local_c8[0] = 0;
  uVar7 = FUN_0014e7c4(param_1,param_2,local_c8,8);
  uVar11 = local_c8[0];
  if (((local_c8[0] & 7) != 0 || local_c8[0] < 0x1000) || (int)uVar7 == 0) {
    uVar11 = 0;
  }
  if (uVar11 != DAT_0022d8e0 + 0x11abad8U) goto LAB_001655e0;
  uVar12 = *(ulong *)(param_1 + 0x10);
  uVar11 = param_2;
  if ((param_2 != 0) && (uVar12 != param_2)) {
    uVar13 = 0;
    do {
      local_c8[0] = 0;
      iVar5 = FUN_0014e7c4(uVar7,uVar11 + 0x38,local_c8,8);
      uVar3 = local_c8[0];
      bVar4 = (local_c8[0] & 7) != 0;
      uVar1 = local_c8[0];
      if ((iVar5 == 0 || local_c8[0] < 0x1000) || bVar4) {
        uVar1 = 0;
      }
      uVar7 = FUN_00154d5c(uVar11,uVar1);
    } while (((((int)uVar7 != 0) && (uVar11 = uVar1, uVar13 < 0x1f)) &&
             ((iVar5 != 0 && 0xfff < uVar3) && !bVar4)) && (uVar13 = uVar13 + 1, uVar1 != uVar12));
  }
  if (((uVar11 != uVar12) ||
      (iVar5 = FUN_0015516c(param_2,*(undefined8 *)(param_1 + 0x10)), iVar5 == 0)) ||
     (lVar8 = FUN_00167218(param_2,*(undefined8 *)(param_1 + 0x10)), lVar8 == 0)) goto LAB_001655e0;
  iVar5 = FUN_0014d544(param_3,auStack_74);
  iVar6 = FUN_00166d64(param_1,lVar8);
  if (iVar6 == 0) goto LAB_001655e0;
  if (DAT_002815e8 != lVar8) {
    DAT_002815f0 = 0;
    DAT_002815f8 = 0;
    DAT_002815e8 = lVar8;
  }
  DAT_00281600 = *(undefined8 *)(param_1 + 8);
  DAT_002815d8 = *(undefined8 *)(param_1 + 0x10);
  if (iVar5 - 3U < 3) {
    puVar10 = &DAT_002815f0;
LAB_001655c4:
    puVar9 = (ulong *)0x0;
    *puVar10 = param_2;
  }
  else {
    if (iVar5 == 7) {
      puVar10 = &DAT_002815f8;
      goto LAB_001655c4;
    }
    if ((iVar5 != 1) ||
       (uVar7 = FUN_00165944(lVar8,DAT_002815d8,local_c8), uVar11 = local_c8[0], (int)uVar7 == 0))
    goto LAB_001655e0;
    uVar12 = param_2;
    if ((param_2 != 0) && (local_c8[0] != param_2)) {
      uVar13 = 0;
      do {
        local_70 = 0;
        iVar5 = FUN_0014e7c4(uVar7,uVar12 + 0x38,&local_70,8);
        uVar3 = local_70;
        bVar4 = (local_70 & 7) != 0;
        uVar1 = local_70;
        if ((iVar5 == 0 || local_70 < 0x1000) || bVar4) {
          uVar1 = 0;
        }
        uVar7 = FUN_00154d5c(uVar12,uVar1);
      } while (((((int)uVar7 != 0) && (uVar12 = uVar1, uVar13 < 0x1f)) &&
               ((iVar5 != 0 && 0xfff < uVar3) && !bVar4)) && (uVar13 = uVar13 + 1, uVar1 != uVar11))
      ;
    }
    if (uVar12 != uVar11) goto LAB_001655e0;
    puVar9 = local_c8;
    local_80 = param_2;
  }
  FUN_0014dca4(&DAT_0022d8f0,param_1,param_3,puVar9);
LAB_001655e0:
  if (*(long *)(lVar2 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0016563c @ 0016563c ===== */

void FUN_0016563c(long *param_1)

{
  long lVar1;
  bool bVar2;
  int iVar3;
  long *plVar4;
  undefined8 uVar5;
  long local_180 [10];
  undefined1 auStack_12c [4];
  undefined1 auStack_128 [16];
  long local_118;
  long local_110;
  long local_108;
  long local_100;
  undefined1 auStack_d8 [160];
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  plVar4 = param_1;
  if ((param_1 == (long *)0x0) ||
     (plVar4 = (long *)FUN_00166430(*param_1,param_1[1],auStack_128), (int)plVar4 == 0))
  goto LAB_00165730;
  if ((local_118 == param_1[2]) &&
     (((local_110 == param_1[3] && (local_108 == param_1[4])) && (local_100 == param_1[5])))) {
    plVar4 = (long *)FUN_00167000(param_1,auStack_d8);
    if ((int)plVar4 == 0) goto LAB_00165730;
    iVar3 = FUN_0014d544(auStack_d8,auStack_12c);
    if (iVar3 == (int)param_1[8]) {
      if (iVar3 == 2) {
        bVar2 = param_1[9] == 0;
      }
      else {
        if (iVar3 != 1) goto LAB_0016572c;
        uVar5 = FUN_00167218(*param_1,param_1[1]);
        plVar4 = (long *)FUN_0016733c(uVar5,param_1[6]);
        if (((int)plVar4 == 0) ||
           (plVar4 = (long *)FUN_00165944(uVar5,param_1[1],local_180), (int)plVar4 == 0))
        goto LAB_00165730;
        bVar2 = local_180[0] == *param_1;
      }
      plVar4 = (long *)(ulong)bVar2;
      goto LAB_00165730;
    }
  }
LAB_0016572c:
  plVar4 = (long *)0x0;
LAB_00165730:
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(plVar4);
}

/* ===== FUN_00165768 @ 00165768 ===== */

undefined4 FUN_00165768(undefined8 *param_1)

{
  long lVar1;
  int iVar2;
  long lVar3;
  undefined4 uVar4;
  long local_50;
  long local_48;
  ulong local_40;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  local_48 = 0;
  local_40 = 0;
  local_50 = 0;
  if ((((param_1 != (undefined8 *)0x0) && ((code *)param_1[2] != (code *)0x0)) && (param_1[3] != 0))
     && ((param_1[4] != 0 && (param_1[5] != 0)))) {
    iVar2 = (*(code *)param_1[2])(*param_1,param_1[1] + 0x13053c0,&local_40,8);
    uVar4 = 0;
    if ((iVar2 == 0) || ((local_40 < 0x1000 || ((local_40 & 7) != 0)))) goto LAB_00165898;
    iVar2 = (*(code *)param_1[2])(*param_1,local_40,&local_48,8);
    if (((iVar2 != 0) &&
        (((local_48 == param_1[1] + 0x11ebcd0 &&
          (iVar2 = (*(code *)param_1[2])(*param_1,local_48 + 0x18,&local_50,8), iVar2 != 0)) &&
         (local_50 == param_1[1] + 0xac319c)))) && (lVar3 = (*(code *)param_1[3])(0x98), lVar3 != 0)
       ) {
      uVar4 = 1;
      (*(code *)param_1[4])(lVar3,1,0);
      (*(code *)param_1[5])(local_40,lVar3);
      goto LAB_00165898;
    }
  }
  uVar4 = 0;
LAB_00165898:
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return uVar4;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_001658c4 @ 001658c4 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_001658c4(void)

{
  long lVar1;
  undefined8 local_58;
  long local_50;
  code *pcStack_48;
  long local_40;
  long lStack_38;
  long local_30;
  long local_28;
  
  lVar1 = tpidr_el0;
  local_28 = *(long *)(lVar1 + 0x28);
  local_58 = 0;
  local_50 = DAT_0022d8e0;
  pcStack_48 = FUN_0014e7c4;
  local_30 = DAT_0022d8e0 + 0xac319c;
  local_40 = DAT_0022d8e0 + _DAT_0010fa30;
  lStack_38 = DAT_0022d8e0 + _UNK_0010fa38;
  FUN_00165768(&local_58);
  if (*(long *)(lVar1 + 0x28) == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00165944 @ 00165944 ===== */

void FUN_00165944(ulong param_1,undefined8 param_2,ulong *param_3)

{
  ulong uVar1;
  long lVar2;
  ulong uVar3;
  bool bVar4;
  int iVar5;
  ulong uVar6;
  ulong uVar7;
  uint uVar8;
  ulong local_60;
  long local_58;
  
  lVar2 = tpidr_el0;
  local_58 = *(long *)(lVar2 + 0x28);
  uVar6 = FUN_00154eb4();
  if ((int)uVar6 != 0) {
    local_60 = 0;
    iVar5 = FUN_0014e7c4(uVar6,param_1 + 0x1b0,&local_60,8);
    uVar6 = local_60;
    if (((local_60 & 7) != 0 || local_60 < 0x1000) || iVar5 == 0) {
      uVar6 = 0;
    }
    uVar6 = FUN_00166430(uVar6,param_2,param_3);
    if ((int)uVar6 != 0) {
      uVar7 = *param_3;
      if ((uVar7 != param_1) && (uVar7 != 0)) {
        uVar8 = 0;
        do {
          local_60 = 0;
          iVar5 = FUN_0014e7c4(uVar6,uVar7 + 0x38,&local_60,8);
          uVar3 = local_60;
          bVar4 = (local_60 & 7) != 0;
          uVar1 = local_60;
          if ((iVar5 == 0 || local_60 < 0x1000) || bVar4) {
            uVar1 = 0;
          }
          uVar6 = FUN_00154d5c(uVar7,uVar1);
        } while (((((int)uVar6 != 0) && (uVar7 = uVar1, uVar8 < 0x1f)) &&
                 ((iVar5 != 0 && 0xfff < uVar3) && !bVar4)) && (uVar8 = uVar8 + 1, uVar1 != param_1)
                );
      }
      if ((uVar7 == param_1) && (param_3[4] == param_1 + 0x80)) {
        uVar6 = (ulong)(param_3[5] == DAT_0022d8e0 + 0x91bb7cU);
      }
      else {
        uVar6 = 0;
      }
    }
  }
  if (*(long *)(lVar2 + 0x28) != local_58) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar6);
  }
  return;
}

/* ===== FUN_00166430 @ 00166430 ===== */

void FUN_00166430(ulong param_1,ulong param_2,ulong *param_3)

{
  long lVar1;
  ulong uVar2;
  ulong uVar3;
  ulong uVar4;
  ulong uVar5;
  long lVar6;
  bool bVar7;
  ulong uVar8;
  bool bVar9;
  bool bVar10;
  bool bVar11;
  int iVar12;
  undefined8 uVar13;
  ulong uVar14;
  uint uVar15;
  char local_84 [4];
  ulong local_80;
  char local_74 [4];
  ulong local_70;
  long local_68;
  
  lVar6 = tpidr_el0;
  local_68 = *(long *)(lVar6 + 0x28);
  local_70 = 0;
  uVar13 = FUN_0014e7c4(param_1,param_1,&local_70,8);
  uVar2 = local_70;
  local_70 = 0;
  if (((uVar2 & 7) != 0 || uVar2 < 0x1000) || (int)uVar13 == 0) {
    uVar2 = 0;
  }
  uVar13 = FUN_0014e7c4(uVar13,param_1 + 0x38,&local_70,8);
  uVar14 = local_70;
  bVar9 = (int)uVar13 != 0;
  local_70 = 0;
  bVar10 = (uVar14 & 7) != 0;
  uVar3 = uVar14;
  if ((!bVar9 || uVar14 < 0x1000) || bVar10) {
    uVar3 = 0;
  }
  iVar12 = FUN_0014e7c4(uVar13,param_1 + 0xa8,&local_70,8);
  uVar13 = 0;
  local_80 = 0;
  bVar7 = iVar12 != 0;
  local_84[0] = '\0';
  bVar11 = (local_70 & 7) == 0;
  uVar4 = local_70;
  if ((!bVar7 || local_70 < 0x1000) || !bVar11) {
    uVar4 = 0;
  }
  if (((uVar2 == DAT_0022d8e0 + 0x11c0a48U) && ((bVar9 && 0xfff < uVar14) && !bVar10)) &&
     (((bVar7 && 0xffe < local_70) && (iVar12 == 0 || local_70 != 0xfff)) && bVar11)) {
    uVar14 = param_1;
    if ((param_1 != 0) && (param_1 != param_2)) {
      uVar15 = 0;
      do {
        local_70 = 0;
        iVar12 = FUN_0014e7c4(uVar13,uVar14 + 0x38,&local_70,8);
        uVar8 = local_70;
        bVar9 = (local_70 & 7) != 0;
        uVar5 = local_70;
        if ((iVar12 == 0 || local_70 < 0x1000) || bVar9) {
          uVar5 = 0;
        }
        uVar13 = FUN_00154d5c(uVar14,uVar5);
      } while ((((int)uVar13 != 0) && (uVar14 = uVar5, uVar15 < 0x1f)) &&
              (((iVar12 != 0 && 0xfff < uVar8) && !bVar9 && (uVar15 = uVar15 + 1, uVar5 != param_2))
              ));
    }
    if (uVar14 == param_2) {
      iVar12 = FUN_0015516c(param_1,param_2);
      uVar13 = 0;
      if ((param_1 != 0) && (iVar12 != 0)) {
        uVar15 = 0;
        uVar14 = param_1;
        while( true ) {
          local_74[0] = '\0';
          uVar13 = FUN_0014e7c4(0,uVar14 + 0x48,local_74,1);
          if (((int)uVar13 == 0) || (local_74[0] != '\x01')) goto LAB_0016685c;
          if (uVar14 == param_2) break;
          local_70 = 0;
          iVar12 = FUN_0014e7c4(uVar13,uVar14 + 0x38,&local_70,8);
          uVar13 = 0;
          bVar9 = (local_70 & 7) != 0;
          uVar14 = local_70;
          if ((iVar12 == 0 || local_70 < 0x1000) || bVar9) {
            uVar14 = 0;
          }
          if ((0x1e < uVar15) || (uVar15 = uVar15 + 1, (iVar12 == 0 || local_70 < 0x1000) || bVar9))
          goto LAB_00166860;
        }
        iVar12 = FUN_0014e7c4(uVar13,param_1 + 0x48,local_84,1);
        uVar13 = 0;
        if ((iVar12 != 0) && (local_84[0] == '\x01')) {
          local_70 = 0;
          uVar13 = FUN_0014e7c4(0,uVar4,&local_70,8);
          lVar1 = local_70 + 8;
          if (((local_70 & 7) != 0 || local_70 < 0x1000) || (int)uVar13 == 0) {
            lVar1 = 8;
          }
          uVar13 = FUN_0014e7c4(uVar13,lVar1,&local_80,8);
          if (((((int)uVar13 != 0) && (uVar13 = 0, (local_80 & 3) == 0)) &&
              (DAT_0022f4b0._4_4_ == 0)) && (((uint)DAT_0022f4b0 < 9 && ((uint)DAT_0022f4b0 != 0))))
          {
            if ((local_80 < DAT_0022f430) || (DAT_0022f470 <= local_80)) {
              bVar9 = 1 < (uint)DAT_0022f4b0;
              if ((((uint)DAT_0022f4b0 != 1) &&
                  ((((local_80 < DAT_0022f438 || (DAT_0022f478 <= local_80)) &&
                    ((bVar9 = 2 < (uint)DAT_0022f4b0, (uint)DAT_0022f4b0 != 2 &&
                     (((local_80 < DAT_0022f440 || (DAT_0022f480 <= local_80)) &&
                      (bVar9 = 3 < (uint)DAT_0022f4b0, (uint)DAT_0022f4b0 != 3)))))) &&
                   ((local_80 < DAT_0022f448 || (DAT_0022f488 <= local_80)))))) &&
                 ((bVar9 = 4 < (uint)DAT_0022f4b0, (uint)DAT_0022f4b0 != 4 &&
                  ((((local_80 < DAT_0022f450 || (DAT_0022f490 <= local_80)) &&
                    ((bVar9 = 5 < (uint)DAT_0022f4b0, (uint)DAT_0022f4b0 != 5 &&
                     (((local_80 < DAT_0022f458 || (DAT_0022f498 <= local_80)) &&
                      (bVar9 = 6 < (uint)DAT_0022f4b0, (uint)DAT_0022f4b0 != 6)))))) &&
                   (((local_80 < DAT_0022f460 || (DAT_0022f4a0 <= local_80)) &&
                    ((bVar9 = 7 < (uint)DAT_0022f4b0, (uint)DAT_0022f4b0 != 7 &&
                     (DAT_0022f4a8 <= local_80 || local_80 < DAT_0022f468)))))))))) {
                bVar9 = 8 < (uint)DAT_0022f4b0;
              }
              if (!bVar9) goto LAB_0016685c;
            }
            uVar13 = 1;
            *param_3 = param_1;
            param_3[1] = param_2;
            param_3[2] = uVar3;
            param_3[3] = uVar2;
            param_3[4] = uVar4;
            param_3[5] = local_80;
            param_3[9] = 0;
            param_3[6] = 0;
            param_3[7] = 0;
            *(undefined4 *)(param_3 + 8) = 0;
          }
        }
      }
    }
    else {
LAB_0016685c:
      uVar13 = 0;
    }
  }
LAB_00166860:
  if (*(long *)(lVar6 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar13);
  }
  return;
}

/* ===== FUN_001668ec @ 001668ec ===== */

/* WARNING: Type propagation algorithm not settling */

void FUN_001668ec(long *param_1)

{
  ulong uVar1;
  long lVar2;
  long lVar3;
  ulong uVar4;
  ulong uVar5;
  bool bVar6;
  int iVar7;
  undefined8 uVar8;
  long lVar9;
  ulong uVar10;
  ulong uVar11;
  long lVar12;
  uint uVar13;
  long local_190 [9];
  undefined8 local_148;
  int local_13c;
  long local_138;
  ulong local_130 [2];
  ulong local_120 [20];
  undefined8 local_80;
  long local_78;
  long local_70;
  
  uVar4 = DAT_002815e8;
  lVar3 = tpidr_el0;
  local_70 = *(long *)(lVar3 + 0x28);
  iVar7 = FUN_00154eb4(DAT_002815e8,param_1[2]);
  if ((iVar7 == 0) || (uVar8 = FUN_00166d64(param_1,uVar4), (int)uVar8 == 0)) {
    if (*(long *)(lVar3 + 0x28) == local_70) {
      DAT_002815e8 = 0;
      DAT_002815f0 = 0;
      DAT_002815f8 = 0;
      return;
    }
    goto LAB_00166d60;
  }
  local_120[0] = 0;
  uVar8 = FUN_0014e7c4(uVar8,uVar4 + 0x248,local_120,8);
  uVar10 = local_120[0];
  local_138 = 0;
  uVar11 = local_120[0] & 7;
  local_120[0] = 0;
  if ((uVar11 != 0 || uVar10 < 0x1000) || (int)uVar8 == 0) {
    uVar10 = 0;
  }
  uVar8 = FUN_0014e7c4(uVar8,uVar10,local_120,8);
  uVar11 = local_120[0];
  if (((local_120[0] & 7) != 0 || local_120[0] < 0x1000) || (int)uVar8 == 0) {
    uVar11 = 0;
  }
  if (((((uVar11 == DAT_0022d8e0 + 0x1226df0U) &&
        (uVar8 = FUN_0014e7c4(uVar8,DAT_0022d8e0 + 0x1226e18,&local_138,8), (int)uVar8 != 0)) &&
       (local_138 == DAT_0022d8e0 + 0xf3ead4)) &&
      (((iVar7 = FUN_0014e7c4(uVar8,uVar10 + 0xb0,&local_13c,4), iVar7 != 0 && (DAT_0022da84 != 0))
       && ((DAT_0022da7c._4_4_ == 0 && ((param_1[1] == DAT_0022d8f8 && (DAT_0022d9c8 != 0)))))))) &&
     (DAT_0022d9c8 == param_1[1])) {
    DAT_0022da08 = *param_1;
    if (DAT_0022d9d8 == 0) {
      DAT_0022d9d8 = DAT_0022da08;
    }
    if (((DAT_0022da6c == 0) || (DAT_0022da64 == 0)) || (DAT_0022da50._4_4_ != local_13c)) {
      DAT_0022d9e8 = DAT_0022da08;
    }
    DAT_0022da50._4_4_ = local_13c;
    DAT_0022da6c = 1;
    DAT_0022da64 = 1;
    DAT_0022da18 = DAT_0022da08;
  }
  lVar9 = FUN_00165944(uVar4,param_1[2],local_190);
  if ((int)lVar9 != 0) {
    if (((DAT_0022d928 == local_190[0]) && (DAT_0022d930 == param_1[2])) &&
       ((DAT_0022d958 == param_1[1] && (lVar9 = FUN_0016563c(&DAT_0022d928), (int)lVar9 != 0)))) {
      local_148 = DAT_0022d970;
      lVar9 = FUN_00167000(local_190,local_120);
      if ((int)lVar9 != 0) {
LAB_00166bc8:
        lVar9 = FUN_0014dca4(&DAT_0022d8f0,param_1,local_120,local_190);
      }
    }
    else if ((local_190[0] + 0x208 != 0) &&
            (lVar9 = FUN_0014e7c4(lVar9,local_190[0] + 0x208,&local_80,0x10), (int)lVar9 != 0)) {
      uVar10 = (ulong)local_80._4_4_;
      if (0xffffff60 < local_80._4_4_ - 0xa0) {
        lVar12 = local_190[0] + 0x210;
        if (7 < local_80._4_4_) {
          lVar12 = local_78;
        }
        if (((lVar12 != 0) && (lVar9 = FUN_0014e7c4(lVar9,lVar12,local_120,uVar10), (int)lVar9 != 0)
            ) && (lVar9 = __memchr_chk(local_120,0,uVar10,0xa0), lVar9 == 0)) {
          *(undefined1 *)((long)local_120 + uVar10) = 0;
          lVar9 = FUN_0014d544(local_120,&local_80);
          if ((int)lVar9 == 1) goto LAB_00166bc8;
        }
      }
    }
  }
  lVar12 = 0;
  local_130[1] = DAT_002815f8;
  local_130[0] = DAT_002815f0;
  do {
    uVar10 = local_130[lVar12];
    if (uVar10 != 0) {
      if (uVar10 != uVar4) {
        uVar13 = 0;
        uVar11 = uVar10;
        do {
          local_80 = 0;
          iVar7 = FUN_0014e7c4(lVar9,uVar11 + 0x38,&local_80,8);
          uVar5 = local_80;
          bVar6 = (local_80 & 7) != 0;
          uVar1 = local_80;
          if ((iVar7 == 0 || local_80 < 0x1000) || bVar6) {
            uVar1 = 0;
          }
          lVar9 = FUN_00154d5c(uVar11,uVar1);
        } while ((((int)lVar9 != 0) && (uVar11 = uVar1, uVar13 < 0x1f)) &&
                (((iVar7 != 0 && 0xfff < uVar5) && !bVar6 && (uVar13 = uVar13 + 1, uVar1 != uVar4)))
                );
        if (uVar11 != uVar4) goto LAB_00166c00;
      }
      if ((uVar10 + 0xe0 != 0) &&
         (lVar9 = FUN_0014e7c4(lVar9,uVar10 + 0xe0,&local_80,0x10), (int)lVar9 != 0)) {
        uVar11 = (ulong)local_80._4_4_;
        if (0xffffff60 < local_80._4_4_ - 0xa0) {
          lVar2 = uVar10 + 0xe8;
          if (7 < local_80._4_4_) {
            lVar2 = local_78;
          }
          if (((lVar2 != 0) && (lVar9 = FUN_0014e7c4(lVar9,lVar2,local_120,uVar11), (int)lVar9 != 0)
              ) && (lVar9 = __memchr_chk(local_120,0,uVar11,0xa0), lVar9 == 0)) {
            *(undefined1 *)((long)local_120 + uVar11) = 0;
            lVar9 = FUN_00165360(param_1,uVar10,local_120);
          }
        }
      }
    }
LAB_00166c00:
    lVar12 = lVar12 + 1;
  } while (lVar12 != 2);
  if (*(long *)(lVar3 + 0x28) == local_70) {
    return;
  }
LAB_00166d60:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00166d64 @ 00166d64 ===== */

void FUN_00166d64(long param_1,long param_2)

{
  long *plVar1;
  ulong uVar2;
  long *plVar3;
  long lVar4;
  bool bVar5;
  bool bVar6;
  int iVar7;
  long lVar8;
  long lVar9;
  ulong local_40;
  long local_38;
  
  lVar4 = tpidr_el0;
  local_38 = *(long *)(lVar4 + 0x28);
  local_40 = 0;
  iVar7 = FUN_0014e7c4(param_1,param_2 + 0x248,&local_40,8);
  lVar8 = DAT_0022d9c8;
  bVar6 = false;
  bVar5 = (local_40 & 7) != 0;
  uVar2 = local_40;
  if ((iVar7 == 0 || local_40 < 0x1000) || bVar5) {
    uVar2 = 0;
  }
  if (((iVar7 == 0 || local_40 < 0x1000) || bVar5) || (DAT_0022da84 == 0)) goto LAB_00166fd8;
  if (DAT_0022d9c8 == *(long *)(param_1 + 8)) {
    if ((DAT_00281918 == param_2) && (DAT_00281920 == uVar2)) {
      lVar8 = 0;
    }
    else if ((DAT_00281938 == param_2) && (DAT_00281940 == uVar2)) {
      lVar8 = 1;
    }
    else if ((DAT_00281958 == param_2) && (DAT_00281960 == uVar2)) {
      lVar8 = 2;
    }
    else if ((DAT_00281978 == param_2) && (DAT_00281980 == uVar2)) {
      lVar8 = 3;
    }
    else if ((DAT_00281998 == param_2) && (DAT_002819a0 == uVar2)) {
      lVar8 = 4;
    }
    else if ((DAT_002819b8 == param_2) && (DAT_002819c0 == uVar2)) {
      lVar8 = 5;
    }
    else if ((DAT_002819d8 == param_2) && (DAT_002819e0 == uVar2)) {
      lVar8 = 6;
    }
    else {
      if ((DAT_002819f8 != param_2) || (DAT_00281a00 != uVar2)) {
        plVar1 = &DAT_00281938;
        if (DAT_00281938 != 0) {
          plVar1 = (long *)0x0;
        }
        plVar3 = &DAT_00281918;
        if (DAT_00281918 != 0) {
          plVar3 = plVar1;
        }
        if ((DAT_00281938 != 0 && DAT_00281918 != 0) && DAT_00281958 == 0) {
          plVar3 = &DAT_00281958;
        }
        if (DAT_00281978 == 0 && plVar3 == (long *)0x0) {
          plVar3 = &DAT_00281978;
        }
        if (DAT_00281998 == 0 && plVar3 == (long *)0x0) {
          plVar3 = &DAT_00281998;
        }
        if (DAT_002819b8 == 0 && plVar3 == (long *)0x0) {
          plVar3 = &DAT_002819b8;
        }
        if (DAT_002819d8 == 0 && plVar3 == (long *)0x0) {
          plVar3 = &DAT_002819d8;
        }
        if (DAT_002819f8 == 0 && plVar3 == (long *)0x0) {
          plVar3 = &DAT_002819f8;
        }
        if (plVar3 != (long *)0x0) {
          lVar9 = *(long *)(param_1 + 0x10);
          bVar6 = true;
          *plVar3 = param_2;
          plVar3[1] = uVar2;
          plVar3[2] = lVar9;
          plVar3[3] = lVar8;
          goto LAB_00166fd8;
        }
        goto LAB_00166fd4;
      }
      lVar8 = 7;
    }
    bVar6 = (&DAT_00281930)[lVar8 * 4] == DAT_0022d9c8;
  }
  else {
LAB_00166fd4:
    bVar6 = false;
  }
LAB_00166fd8:
  if (*(long *)(lVar4 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(bVar6);
}

/* ===== FUN_00167000 @ 00167000 ===== */

void FUN_00167000(ulong *param_1,void *param_2)

{
  ulong uVar1;
  long lVar2;
  ulong uVar3;
  bool bVar4;
  int iVar5;
  undefined8 uVar6;
  void *pvVar7;
  long lVar8;
  ulong uVar9;
  ulong uVar10;
  uint uVar11;
  undefined8 local_78;
  long local_70;
  long local_68;
  
  lVar2 = tpidr_el0;
  local_68 = *(long *)(lVar2 + 0x28);
  if (param_1[9] == 0) {
    uVar9 = *param_1;
    lVar8 = uVar9 + 0x208;
    if (lVar8 != 0) {
      uVar6 = FUN_0014e7c4(param_1,lVar8,&local_78,0x10);
      if ((int)uVar6 == 0) goto LAB_001671e4;
      if (0xffffff60 < local_78._4_4_ - 0xa0) {
        lVar8 = uVar9 + 0x210;
LAB_0016719c:
        uVar9 = (ulong)local_78._4_4_;
        if (7 < local_78._4_4_) {
          lVar8 = local_70;
        }
        if (lVar8 != 0) {
          uVar6 = FUN_0014e7c4(uVar6,lVar8,param_2,uVar9);
          if ((int)uVar6 != 0) {
            pvVar7 = memchr(param_2,0,uVar9);
            uVar6 = 0;
            if (pvVar7 == (void *)0x0) {
              uVar6 = 1;
              *(undefined1 *)((long)param_2 + uVar9) = 0;
            }
          }
          goto LAB_001671e4;
        }
      }
    }
  }
  else {
    local_78 = 0;
    uVar6 = FUN_0014e7c4(param_1,param_1[9],&local_78,8);
    uVar9 = local_78;
    if (((local_78 & 7) != 0 || local_78 < 0x1000) || (int)uVar6 == 0) {
      uVar9 = 0;
    }
    if (uVar9 == DAT_0022d8e0 + 0x11abad8U) {
      uVar9 = param_1[9];
      uVar10 = *param_1;
      if ((uVar9 != 0) && (uVar9 != uVar10)) {
        uVar11 = 0;
        do {
          local_78 = 0;
          iVar5 = FUN_0014e7c4(uVar6,uVar9 + 0x38,&local_78,8);
          uVar3 = local_78;
          bVar4 = (local_78 & 7) != 0;
          uVar1 = local_78;
          if ((iVar5 == 0 || local_78 < 0x1000) || bVar4) {
            uVar1 = 0;
          }
          uVar6 = FUN_00154d5c(uVar9,uVar1);
        } while (((((int)uVar6 != 0) && (uVar9 = uVar1, uVar11 < 0x1f)) &&
                 ((iVar5 != 0 && 0xfff < uVar3) && !bVar4)) &&
                (uVar11 = uVar11 + 1, uVar1 != uVar10));
      }
      if (uVar9 == uVar10) {
        uVar6 = FUN_0015516c(param_1[9],param_1[1]);
        if ((int)uVar6 == 0) goto LAB_001671e4;
        uVar9 = param_1[9];
        lVar8 = uVar9 + 0xe0;
        if (lVar8 != 0) {
          uVar6 = FUN_0014e7c4(uVar6,lVar8,&local_78,0x10);
          if ((int)uVar6 == 0) goto LAB_001671e4;
          if (0xffffff60 < local_78._4_4_ - 0xa0) {
            lVar8 = uVar9 + 0xe8;
            goto LAB_0016719c;
          }
        }
      }
    }
  }
  uVar6 = 0;
LAB_001671e4:
  if (*(long *)(lVar2 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar6);
  }
  return;
}

/* ===== FUN_00167218 @ 00167218 ===== */

void FUN_00167218(ulong param_1,ulong param_2)

{
  ulong uVar1;
  long lVar2;
  bool bVar3;
  int iVar4;
  undefined8 uVar5;
  ulong uVar6;
  uint uVar7;
  ulong local_60;
  long local_58;
  
  lVar2 = tpidr_el0;
  local_58 = *(long *)(lVar2 + 0x28);
  if (param_1 != 0) {
    uVar7 = 0;
    uVar6 = param_1;
    while( true ) {
      local_60 = 0;
      uVar5 = FUN_0014e7c4(param_1,uVar6,&local_60,8);
      uVar1 = local_60;
      if (((local_60 & 7) != 0 || local_60 < 0x1000) || (int)uVar5 == 0) {
        uVar1 = 0;
      }
      if (uVar1 == DAT_0022d8e0 + 0x11c7b88U) break;
      if (uVar6 == param_2) {
        param_1 = 0;
        goto LAB_0016730c;
      }
      local_60 = 0;
      iVar4 = FUN_0014e7c4(uVar5,uVar6 + 0x38,&local_60,8);
      param_1 = 0;
      bVar3 = (local_60 & 7) != 0;
      uVar6 = local_60;
      if ((iVar4 == 0 || local_60 < 0x1000) || bVar3) {
        uVar6 = 0;
      }
      if ((0x1e < uVar7) || (uVar7 = uVar7 + 1, (iVar4 == 0 || local_60 < 0x1000) || bVar3))
      goto LAB_0016730c;
    }
    iVar4 = FUN_00154eb4(uVar6,param_2);
    param_1 = 0;
    if (iVar4 != 0) {
      param_1 = uVar6;
    }
  }
LAB_0016730c:
  if (*(long *)(lVar2 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(param_1);
}

/* ===== FUN_0016733c @ 0016733c ===== */

void FUN_0016733c(long param_1,long param_2)

{
  ulong uVar1;
  long lVar2;
  bool bVar3;
  bool bVar4;
  int iVar5;
  long lVar6;
  ulong local_40;
  long local_38;
  
  lVar2 = tpidr_el0;
  local_38 = *(long *)(lVar2 + 0x28);
  local_40 = 0;
  iVar5 = FUN_0014e7c4(param_1,param_1 + 0x248,&local_40,8);
  bVar4 = false;
  bVar3 = (local_40 & 7) != 0;
  uVar1 = local_40;
  if ((iVar5 == 0 || local_40 < 0x1000) || bVar3) {
    uVar1 = 0;
  }
  if ((param_2 != 0) && ((iVar5 != 0 && 0xfff < local_40) && !bVar3)) {
    if ((DAT_00281918 == param_1) && (DAT_00281920 == uVar1)) {
      lVar6 = 0;
    }
    else if ((DAT_00281938 == param_1) && (DAT_00281940 == uVar1)) {
      lVar6 = 1;
    }
    else if ((DAT_00281958 == param_1) && (DAT_00281960 == uVar1)) {
      lVar6 = 2;
    }
    else if ((DAT_00281978 == param_1) && (DAT_00281980 == uVar1)) {
      lVar6 = 3;
    }
    else if ((DAT_00281998 == param_1) && (DAT_002819a0 == uVar1)) {
      lVar6 = 4;
    }
    else if ((DAT_002819b8 == param_1) && (DAT_002819c0 == uVar1)) {
      lVar6 = 5;
    }
    else if ((DAT_002819d8 == param_1) && (DAT_002819e0 == uVar1)) {
      lVar6 = 6;
    }
    else {
      bVar4 = false;
      if ((DAT_002819f8 != param_1) || (DAT_00281a00 != uVar1)) goto LAB_001674fc;
      lVar6 = 7;
    }
    bVar4 = (&DAT_00281930)[lVar6 * 4] == param_2;
  }
LAB_001674fc:
  if (*(long *)(lVar2 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(bVar4);
}

/* ===== FUN_00167524 @ 00167524 ===== */

void FUN_00167524(int *param_1,int *param_2)

{
  ulong uVar1;
  ulong uVar2;
  ulong uVar3;
  long lVar4;
  bool bVar5;
  bool bVar6;
  int iVar7;
  undefined8 uVar8;
  ulong uVar9;
  undefined8 local_78;
  ulong local_70;
  long local_68;
  
  lVar4 = tpidr_el0;
  local_68 = *(long *)(lVar4 + 0x28);
  local_78 = 0;
  uVar8 = FUN_0014e7c4(param_1,DAT_0022d8e0 + 0x12429c0,(long)&local_78 + 4,4);
  if ((int)uVar8 == 0) goto LAB_0016771c;
  local_70 = 0;
  uVar8 = FUN_0014e7c4(uVar8,DAT_0022d8e0 + 0x12eb7f8,&local_70,8);
  bVar5 = (int)uVar8 != 0;
  bVar6 = (local_70 & 7) != 0;
  uVar9 = local_70;
  if ((!bVar5 || local_70 < 0x1000) || bVar6) {
    uVar9 = 0;
  }
  if ((bVar5 && 0xfff < local_70) && !bVar6) {
    local_70 = 0;
    uVar8 = FUN_0014e7c4(uVar8,uVar9 + 0x60,&local_70,8);
    bVar5 = (int)uVar8 != 0;
    bVar6 = (local_70 & 7) != 0;
    uVar1 = local_70;
    if ((!bVar5 || local_70 < 0x1000) || bVar6) {
      uVar1 = 0;
    }
    if ((bVar5 && 0xfff < local_70) && !bVar6) {
      local_70 = 0;
      uVar8 = FUN_0014e7c4(uVar8,uVar1 + 0x50,&local_70,8);
      bVar5 = (int)uVar8 != 0;
      bVar6 = (local_70 & 7) != 0;
      uVar2 = local_70;
      if ((!bVar5 || local_70 < 0x1000) || bVar6) {
        uVar2 = 0;
      }
      if ((bVar5 && 0xfff < local_70) && !bVar6) {
        uVar8 = FUN_0014e7c4(uVar8,uVar2,param_2,8);
        if ((int)uVar8 == 0) goto LAB_0016771c;
        if ((0xffffdfff < *param_2 - 0x2001U) && (0xffffdfff < param_2[1] - 0x2001U)) {
          local_70 = 0;
          iVar7 = FUN_0014e7c4(uVar8,DAT_0022d8e0 + 0x12eb7f8,&local_70,8);
          uVar3 = local_70;
          if (((local_70 & 7) != 0 || local_70 < 0x1000) || iVar7 == 0) {
            uVar3 = 0;
          }
          if (((uVar3 == uVar9) && (uVar9 = FUN_00154cf0(uVar9 + 0x60), uVar9 == uVar1)) &&
             (uVar9 = FUN_00154cf0(uVar1 + 0x50), uVar9 == uVar2)) {
            uVar8 = FUN_0014e7c4(uVar9,DAT_0022d8e0 + 0x12429c0,&local_78,4);
            if ((int)uVar8 == 0) goto LAB_0016771c;
            if ((int)local_78 == local_78._4_4_) {
              uVar8 = 1;
              *param_1 = (int)local_78;
              goto LAB_0016771c;
            }
          }
        }
      }
    }
  }
  uVar8 = 0;
LAB_0016771c:
  if (*(long *)(lVar4 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar8);
}

/* ===== FUN_00167750 @ 00167750 ===== */

void FUN_00167750(void)

{
  long lVar1;
  int iVar2;
  ulong uVar3;
  long local_40;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  uVar3 = FUN_0014d168(0x75bc3c,0x14,
                       "2ac86ecf2f5fb5847b475b3bfb248d09dce96f195be09baf2a92361f584dfdcd");
  if (((((int)uVar3 != 0) &&
       (uVar3 = FUN_0014d168(0x4f8058,0x2c,
                             "b1ea48a31bf8a6d35cd2fa451b1b8828c4be25f6bf6155b3a4c368096a3e783c"),
       (int)uVar3 != 0)) &&
      (uVar3 = FUN_0014d168(0x4f852c,0xc,
                            "db1b20fa8a89ea7479d2f2f151e289071cc1a4ce360f1fdd89e17d4ea5369cb5"),
      (int)uVar3 != 0)) &&
     (((uVar3 = FUN_0014d168(0x4f8c30,0x58,
                             "b1d1fbf008ffbf58da7338a11324129f384c5d69162665812119e8d59c87f649"),
       (int)uVar3 != 0 &&
       (uVar3 = FUN_0014d168(0x75b2a4,0x414,
                             "2c953180e20e98212607df431d583d48d31444e0a72d9d60bf5e2d018b1354a0"),
       (int)uVar3 != 0)) &&
      ((uVar3 = FUN_0014d168(0x75b6c0,0x57c,
                             "209b658c77125de592aadd9ddfa5daf55f83dde458d259176c867f5258dd5756"),
       (int)uVar3 != 0 &&
       (uVar3 = FUN_0014e7c4(uVar3,DAT_0022d8e0 + 0x2063d8,&local_40,8), (int)uVar3 != 0)))))) {
    if (local_40 == -0x700000010) {
      iVar2 = FUN_001505d8();
      uVar3 = (ulong)(iVar2 != 0);
    }
    else {
      uVar3 = 0;
    }
  }
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar3);
}

/* ===== FUN_00167888 @ 00167888 ===== */

void FUN_00167888(undefined8 param_1,undefined8 param_2)

{
  long lVar1;
  int iVar2;
  timespec local_48;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  iVar2 = clock_gettime(1,&local_48);
  if (iVar2 == 0) {
    DAT_00281a20 = local_48.tv_sec * 1000 + (ulong)local_48.tv_nsec / 1000000;
  }
  else {
    DAT_00281a20 = 0;
  }
  DAT_0022d890 = 1;
  (*(code *)(DAT_0022d8e0 + 0x75cddc))(param_2);
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00167e78 @ 00167e78 ===== */

void FUN_00167e78(long *param_1)

{
  long lVar1;
  uint uVar2;
  long lVar3;
  long lVar4;
  int iVar5;
  long *plVar6;
  byte local_5c [4];
  long local_58;
  long local_50;
  long local_48;
  long local_40;
  long local_38;
  
  lVar3 = tpidr_el0;
  local_38 = *(long *)(lVar3 + 0x28);
  plVar6 = param_1;
  if (param_1 == (long *)0x0) goto LAB_00168000;
  if ((((0x12ff040 < *param_1 + 0x12ff040U) && (param_1[2] != 0)) && (param_1[3] != 0)) &&
     (param_1[4] != 0)) {
    local_40 = 0;
    local_50 = 0;
    local_48 = 0;
    local_58 = 0;
    local_5c[0] = 0;
    plVar6 = (long *)FUN_00168580(param_1,&local_40,&local_48);
    lVar4 = local_40;
    if ((int)plVar6 == 0) goto LAB_00168000;
    lVar1 = local_40 + 0x101;
    iVar5 = (*(code *)param_1[2])(param_1[1],lVar1,local_5c,1);
    plVar6 = (long *)0x0;
    if ((iVar5 == 0) || (1 < local_5c[0])) goto LAB_00168000;
    if (local_5c[0] != 0) {
      plVar6 = (long *)0x2;
      goto LAB_00168000;
    }
    plVar6 = (long *)(*(code *)param_1[3])(param_1[1]);
    if ((int)plVar6 == 0) goto LAB_00168000;
    iVar5 = FUN_00168580(param_1,&local_50,&local_58);
    plVar6 = (long *)0x0;
    if ((iVar5 == 0) || (lVar4 != local_50)) goto LAB_00168000;
    if (local_48 == local_58) {
      iVar5 = (*(code *)param_1[2])(param_1[1],lVar1,local_5c,1);
      plVar6 = (long *)0x0;
      if (((iVar5 != 0) && (local_5c[0] == 0)) &&
         (((code *)param_1[5] == (code *)0x0 ||
          (plVar6 = (long *)(*(code *)param_1[5])(param_1[1],lVar4), (int)plVar6 != 0)))) {
        (*(code *)param_1[4])(param_1[1],lVar4);
        iVar5 = (*(code *)param_1[2])(param_1[1],lVar1,local_5c,1);
        uVar2 = 0xffffffff;
        if (local_5c[0] == 1 && iVar5 != 0) {
          uVar2 = 1;
        }
        plVar6 = (long *)(ulong)uVar2;
      }
      goto LAB_00168000;
    }
  }
  plVar6 = (long *)0x0;
LAB_00168000:
  if (*(long *)(lVar3 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(plVar6);
}

/* ===== FUN_00168028 @ 00168028 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00168028(long *param_1,long param_2,uint param_3)

{
  long lVar1;
  long lVar2;
  byte bVar3;
  byte bVar4;
  int iVar5;
  undefined8 uVar6;
  long local_b8;
  int local_b0;
  int local_ac;
  byte local_a8;
  long local_a0;
  undefined8 uStack_98;
  undefined8 local_90;
  long local_88;
  int local_80;
  int local_7c;
  byte local_78;
  long local_70;
  int iStack_68;
  int iStack_64;
  byte local_60;
  undefined7 uStack_5f;
  long local_58;
  
  lVar2 = tpidr_el0;
  local_58 = *(long *)(lVar2 + 0x28);
  uVar6 = 0;
  if (((param_1 == (long *)0x0) || (1 < param_3)) || (DAT_0022dff8 != 0)) goto LAB_001683d0;
  if ((param_1[3] == 0) || (param_1[4] == 0)) goto LAB_001683cc;
  iVar5 = FUN_00150710(param_1,&local_70);
  uVar6 = 0;
  if ((iVar5 == 0) || (local_70 != param_2)) goto LAB_001683d0;
  lVar1 = param_2 + 0x88;
  uVar6 = (*(code *)param_1[4])(param_1[1],lVar1,8);
  if (((int)uVar6 == 0) ||
     (uVar6 = (*(code *)param_1[4])(param_1[1],*param_1 + 0x12f03e4,1), (int)uVar6 == 0))
  goto LAB_001683d0;
  if ((DAT_0022dff0 != 0) && (DAT_0022dfc0 != param_2)) {
    if (DAT_0022dff4 == 0) {
      DAT_0022dff0 = 0;
LAB_00168180:
      if (param_3 == 0) {
        _DAT_0022dfe0 = CONCAT44(iStack_64,iStack_68);
        uVar6 = 1;
        _DAT_0022dfe8 = CONCAT71(uStack_5f,local_60);
        DAT_0022dfd8 = local_70;
        goto LAB_001683d0;
      }
      lRam000000000022dfc8 = CONCAT44(iStack_64,iStack_68);
      DAT_0022dfd0 = CONCAT71(uStack_5f,local_60);
      DAT_0022dfc0 = local_70;
      DAT_0022dff0 = 1;
LAB_0016819c:
      uStack_98 = 0;
      local_90 = CONCAT71(local_90._1_7_,1);
      local_a0 = param_2;
      goto LAB_001681a8;
    }
    goto LAB_001683cc;
  }
  if (DAT_0022dff4 == 0) goto LAB_00168180;
  if ((((local_70 != DAT_0022dfd8) || (iStack_68 != DAT_0022dfe0)) || (iStack_64 != DAT_0022dfe4))
     || (local_60 != DAT_0022dfe8)) goto LAB_001683cc;
  if (param_3 != 0) goto LAB_0016819c;
  uStack_98 = lRam000000000022dfc8;
  local_a0 = DAT_0022dfc0;
  local_90 = DAT_0022dfd0;
LAB_001681a8:
  uVar6 = FUN_00150710(param_1,&local_88);
  bVar4 = local_60;
  if ((int)uVar6 == 0) goto LAB_001683d0;
  if (((local_70 == local_88) && (iStack_68 == local_80)) && (iStack_64 == local_7c)) {
    if (local_60 == local_78) {
      bVar3 = (byte)local_90;
      if (((local_70 == local_a0) && (iStack_68 == (int)uStack_98)) &&
         ((iStack_64 == uStack_98._4_4_ && (local_60 == (byte)local_90)))) {
LAB_00168228:
        uVar6 = 1;
        _DAT_0022dfe0 = uStack_98;
        DAT_0022dfd8 = local_a0;
        _DAT_0022dfe8 = local_90;
        DAT_0022dff4 = param_3;
        goto LAB_001683d0;
      }
      if (CONCAT44(iStack_64,iStack_68) == uStack_98) {
LAB_001682ac:
        if (((((bVar4 == bVar3) ||
              (iVar5 = (*(code *)param_1[3])(param_1[1],*param_1 + 0x12f03e4,&local_90,1),
              iVar5 != 0)) && (iVar5 = FUN_00150710(param_1,&local_b8), iVar5 != 0)) &&
            ((local_b8 == local_a0 && (local_b0 == (int)uStack_98)))) &&
           ((local_ac == uStack_98._4_4_ && (local_a8 == (byte)local_90)))) goto LAB_00168228;
      }
      else {
        iVar5 = (*(code *)param_1[3])(param_1[1],lVar1,(ulong)&local_a0 | 8,8);
        if (iVar5 != 0) goto LAB_001682ac;
      }
      iVar5 = FUN_00155980(param_1,param_2);
      if ((iVar5 != 0) && (iVar5 = FUN_001508c8(param_1,param_2), iVar5 != 0)) {
        (*(code *)param_1[3])(param_1[1],lVar1,&iStack_68,8);
        (*(code *)param_1[3])(param_1[1],*param_1 + 0x12f03e4,&local_60,1);
        iVar5 = FUN_00150710(param_1,&local_b8);
        if (((iVar5 != 0) &&
            (((local_b8 == local_70 && (local_b0 == iStack_68)) && (local_ac == iStack_64)))) &&
           (local_a8 == local_60)) goto LAB_001683cc;
      }
      uVar6 = 0xffffffff;
      DAT_0022dff8 = 1;
      goto LAB_001683d0;
    }
  }
LAB_001683cc:
  uVar6 = 0;
LAB_001683d0:
  if (*(long *)(lVar2 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar6);
}

/* ===== FUN_00168410 @ 00168410 ===== */

void FUN_00168410(long *param_1,long *param_2)

{
  long lVar1;
  int iVar2;
  ulong uVar3;
  long lVar4;
  long local_58;
  long local_50;
  int local_48;
  int local_44;
  char local_40;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  local_58 = 0;
  local_50 = 0;
  lVar4 = *param_2;
  iVar2 = (*(code *)param_1[2])(param_1[1],*param_1 + 0x12ff038,&local_50,8);
  if ((((iVar2 != 0) && (local_50 == lVar4)) &&
      (iVar2 = (*(code *)param_1[2])(param_1[1],lVar4,&local_58,8), iVar2 != 0)) &&
     (local_58 == *param_1 + 0x11baa20)) {
    uVar3 = (*(code *)param_1[4])(param_1[1],*param_2 + 0x88,8);
    if (((int)uVar3 == 0) ||
       (uVar3 = (*(code *)param_1[4])(param_1[1],*param_1 + 0x12f03e4,1), (int)uVar3 == 0))
    goto LAB_00168558;
    (*(code *)param_1[3])(param_1[1],*param_2 + 0x88,param_2 + 1,8);
    uVar3 = FUN_00150710(param_1,&local_50);
    if ((int)uVar3 == 0) goto LAB_00168558;
    if (((local_50 == *param_2) && (local_48 == (int)param_2[1])) &&
       (local_44 == *(int *)((long)param_2 + 0xc))) {
      uVar3 = (ulong)(local_40 == (char)param_2[2]);
      goto LAB_00168558;
    }
  }
  uVar3 = 0;
LAB_00168558:
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar3);
}

/* ===== FUN_00168580 @ 00168580 ===== */

void FUN_00168580(long *param_1,ulong *param_2,ulong *param_3)

{
  ulong uVar1;
  long lVar2;
  ulong uVar3;
  int iVar4;
  int iVar5;
  undefined8 uVar6;
  long lVar7;
  long local_318;
  ulong local_310 [14];
  ulong local_2a0 [29];
  char local_1b8;
  int local_190;
  long local_68;
  
  lVar2 = tpidr_el0;
  local_68 = *(long *)(lVar2 + 0x28);
  local_2a0[0] = 0;
  iVar4 = (*(code *)param_1[2])(param_1[1],*param_1 + 0x12ff038,local_2a0,8);
  uVar3 = local_2a0[0];
  local_2a0[0] = 0;
  iVar5 = (*(code *)param_1[2])(param_1[1],*param_1 + 0x12eafa0,local_2a0,8);
  uVar6 = 0;
  uVar1 = 0;
  if (iVar5 != 0) {
    uVar1 = local_2a0[0];
  }
  if (((((iVar4 != 0) && (uVar3 != 0)) && (uVar6 = 0, uVar3 < 0xfffffffffffffdc8)) &&
      ((((uVar3 & 7) == 0 && (uVar1 != 0)) &&
       ((uVar6 = 0, uVar1 < 0xffffffffffffff90 &&
        (((uVar1 & 7) == 0 &&
         (uVar6 = (*(code *)param_1[2])(param_1[1],uVar3,local_2a0,0x238), (int)uVar6 != 0))))))))
     && (uVar6 = (*(code *)param_1[2])(param_1[1],uVar1,local_310,0x70), (int)uVar6 != 0)) {
    lVar7 = *param_1;
    uVar6 = 0;
    if (((local_2a0[0] == lVar7 + 0x11baa20U) && (local_1b8 == '\0')) && (local_190 == 0)) {
      uVar6 = 0;
      if (((lVar7 + 0x11a8410U <= local_310[0]) && (local_310[0] <= lVar7 + 0x123efa0U)) &&
         ((local_310[0] & 7) == 0)) {
        local_318 = 0;
        iVar4 = (*(code *)param_1[2])(param_1[1],local_310[0] + 0x58,&local_318,8);
        lVar7 = 0;
        if (iVar4 != 0) {
          lVar7 = local_318;
        }
        if (lVar7 == *param_1 + 0x4f8c30) {
          uVar6 = 1;
          *param_2 = uVar3;
          *param_3 = uVar1;
        }
        else {
          uVar6 = 0;
        }
      }
    }
  }
  if (*(long *)(lVar2 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar6);
}

/* ===== FUN_00168764 @ 00168764 ===== */

void FUN_00168764(long param_1,int param_2,uint *param_3,int param_4)

{
  uint uVar1;
  int iVar2;
  long lVar3;
  int iVar4;
  int iVar5;
  long lVar6;
  ulong uVar7;
  long lVar8;
  int iVar9;
  undefined1 local_74 [4];
  undefined8 local_70;
  long local_68;
  
  lVar3 = tpidr_el0;
  local_68 = *(long *)(lVar3 + 0x28);
  lVar6 = FUN_001688f0();
  uVar7 = 0;
  if ((param_3 == (uint *)0x0) || (lVar6 == 0)) goto LAB_001688bc;
  if (*param_3 < 0x100) {
    if ((param_2 == 0x7d) && (param_4 != 0)) {
      local_74[0] = 0;
      local_70 = CONCAT44(param_3[1],*param_3);
      iVar4 = (**(code **)(param_1 + 0x30))(local_74,&local_70);
      uVar7 = (ulong)(iVar4 != 0);
      goto LAB_001688bc;
    }
    if ((param_2 == 0x80) || (param_2 == 0x7d)) {
      iVar4 = 1;
      if (param_2 == 0x80) {
        iVar4 = 2;
      }
      iVar9 = 0;
      iVar2 = iVar4;
      do {
        lVar8 = (**(code **)(param_1 + 0x10))(0xa0);
        if (lVar8 == 0) {
          uVar7 = (ulong)(iVar9 != 0) << 1;
          goto LAB_001688bc;
        }
        if (param_2 == 0x7d) {
          local_70 = *(undefined8 *)param_3;
          (**(code **)(param_1 + 0x18))(lVar8,&local_70,0);
        }
        else {
          (**(code **)(param_1 + 0x20))(lVar8,*param_3,param_3[1],0,0);
        }
        iVar5 = (**(code **)(param_1 + 0x28))(lVar6,lVar8);
        if (iVar5 != 0) {
          iVar9 = iVar9 + 1;
        }
        iVar2 = iVar2 + -1;
      } while (iVar2 != 0);
      uVar1 = (uint)(iVar9 != 0) << 1;
      if (iVar9 == iVar4) {
        uVar1 = 1;
      }
      uVar7 = (ulong)uVar1;
      goto LAB_001688bc;
    }
  }
  uVar7 = 0;
LAB_001688bc:
  if (*(long *)(lVar3 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar7);
}

/* ===== FUN_001688f0 @ 001688f0 ===== */

void FUN_001688f0(long *param_1)

{
  long lVar1;
  int iVar2;
  long *plVar3;
  long local_40;
  long local_38;
  long *local_30;
  long local_28;
  
  lVar1 = tpidr_el0;
  local_28 = *(long *)(lVar1 + 0x28);
  local_38 = 0;
  local_30 = (long *)0x0;
  local_40 = 0;
  plVar3 = param_1;
  if (param_1 == (long *)0x0) goto LAB_001689d4;
  if ((code *)param_1[1] != (code *)0x0) {
    iVar2 = (*(code *)param_1[1])(0,*param_1 + 0x13053c0,&local_30,8);
    plVar3 = (long *)0x0;
    if (((iVar2 == 0) || (local_30 == (long *)0x0)) || (((ulong)local_30 & 7) != 0))
    goto LAB_001689d4;
    iVar2 = (*(code *)param_1[1])(0,local_30,&local_38,8);
    if (((iVar2 != 0) && (local_38 == *param_1 + 0x11ebcd0)) &&
       (iVar2 = (*(code *)param_1[1])(0,local_38 + 0x18,&local_40,8), iVar2 != 0)) {
      plVar3 = local_30;
      if (local_40 != *param_1 + 0xac319c) {
        plVar3 = (long *)0x0;
      }
      goto LAB_001689d4;
    }
  }
  plVar3 = (long *)0x0;
LAB_001689d4:
  if (*(long *)(lVar1 + 0x28) != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(plVar3);
  }
  return;
}

/* ===== FUN_001689f8 @ 001689f8 ===== */

void FUN_001689f8(char *param_1)

{
  char cVar1;
  char cVar2;
  long lVar3;
  bool bVar4;
  size_t sVar5;
  char *pcVar6;
  ulong uVar7;
  uint uVar8;
  char local_cc [164];
  long local_28;
  
  lVar3 = tpidr_el0;
  local_28 = *(long *)(lVar3 + 0x28);
  if ((*param_1 == '\0') || (sVar5 = strlen(param_1), 0xa0 < sVar5)) {
    bVar4 = false;
  }
  else {
    cVar2 = *param_1;
    if (cVar2 == '\0') {
      uVar7 = 0;
    }
    else {
      uVar7 = 0;
      uVar8 = 0;
      do {
        cVar1 = cVar2 + -0x20;
        if (0x19 < (byte)(cVar2 + 0x9fU)) {
          cVar1 = cVar2;
        }
        local_cc[uVar7] = cVar1;
        uVar8 = uVar8 + 1;
        uVar7 = (ulong)uVar8;
        cVar2 = param_1[uVar7];
      } while (cVar2 != '\0');
    }
    local_cc[uVar7] = '\0';
    pcVar6 = strstr(local_cc,"RELOAD");
    if (((pcVar6 == (char *)0x0) && (pcVar6 = strstr(param_1,&DAT_0013baaa), pcVar6 == (char *)0x0))
       && (pcVar6 = strstr(param_1,&DAT_001349fc), pcVar6 == (char *)0x0)) {
      pcVar6 = strstr(param_1,&DAT_001325df);
      bVar4 = pcVar6 != (char *)0x0;
    }
    else {
      bVar4 = true;
    }
  }
  if (*(long *)(lVar3 + 0x28) != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(bVar4);
  }
  return;
}

/* ===== FUN_00168b00 @ 00168b00 ===== */

void FUN_00168b00(ulong *param_1,ulong param_2,ulong param_3,ulong param_4)

{
  long lVar1;
  int iVar2;
  uint uVar3;
  ulong uVar4;
  ulong uVar5;
  undefined1 auStack_60 [4];
  undefined1 auStack_5c [4];
  long local_58;
  
  lVar1 = tpidr_el0;
  local_58 = *(long *)(lVar1 + 0x28);
  if (((param_3 < param_4) && ((uint)param_1[0x42] < 0x40)) && (param_1[0x43] <= param_4 - param_3))
  {
    uVar5 = *param_1;
    if (*param_1 <= param_3) {
      uVar5 = param_3;
    }
    uVar4 = -DAT_0022f020;
    if (uVar5 <= uVar4) {
      param_4 = param_4 - param_1[0x43];
      uVar5 = (DAT_0022f020 + uVar5) - 1 & uVar4;
      if (param_1[1] <= param_4) {
        param_4 = param_1[1];
      }
      param_4 = param_4 & uVar4;
      if (uVar5 <= param_4) {
        if ((((uVar5 < 0x10000) || (0xfffffffffffffffb < param_2)) ||
            ((~DAT_0022f020 < uVar5 ||
             ((uVar5 < DAT_0022d8e0 + 0x1400000 && (DAT_0022d8e0 < DAT_0022f020 + uVar5)))))) ||
           ((iVar2 = FUN_00190d68(param_2,uVar5,auStack_5c), iVar2 == 0 ||
            (iVar2 = FUN_00190d68(PTR_DAT_001a36d0 + (uVar5 - (long)PTR_DAT_001a36c0),param_2 + 4,
                                  auStack_60), iVar2 == 0)))) {
          uVar3 = (uint)param_1[0x42];
        }
        else {
          uVar4 = param_1[0x42];
          uVar3 = (uint)uVar4 + 1;
          *(uint *)(param_1 + 0x42) = uVar3;
          param_1[(ulong)(uint)uVar4 + 2] = uVar5;
        }
        if (((((((uVar3 < 0x40) && (param_4 != uVar5)) && (0xffff < param_4)) &&
              (((DAT_0022f020 - 1 & param_4) == 0 && (param_2 < 0xfffffffffffffffc)))) &&
             ((param_4 <= ~DAT_0022f020 &&
              ((DAT_0022d8e0 + 0x1400000 <= param_4 || (DAT_0022f020 + param_4 <= DAT_0022d8e0))))))
            && (iVar2 = FUN_00190d68(param_2,param_4,auStack_5c), iVar2 != 0)) &&
           (iVar2 = FUN_00190d68(PTR_DAT_001a36d0 + (param_4 - (long)PTR_DAT_001a36c0),param_2 + 4,
                                 auStack_60), iVar2 != 0)) {
          uVar5 = param_1[0x42];
          *(uint *)(param_1 + 0x42) = (uint)uVar5 + 1;
          param_1[(ulong)(uint)uVar5 + 2] = param_4;
        }
      }
    }
  }
  if (*(long *)(lVar1 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00168d18 @ 00168d18 ===== */

char * FUN_00168d18(char *param_1)

{
  int iVar1;
  char *pcVar2;
  
  pcVar2 = "unknown";
  if (param_1 != (char *)0x0) {
    iVar1 = strcmp(param_1,"none");
    pcVar2 = "none";
    if (iVar1 != 0) {
      iVar1 = strcmp(param_1,"range");
      pcVar2 = "range";
      if (iVar1 != 0) {
        iVar1 = strcmp(param_1,"maps");
        pcVar2 = "maps";
        if (iVar1 != 0) {
          iVar1 = strcmp(param_1,"alloc");
          pcVar2 = "alloc";
          if (iVar1 != 0) {
            iVar1 = strcmp(param_1,"copy");
            pcVar2 = "copy";
            if (iVar1 != 0) {
              iVar1 = strcmp(param_1,"seal");
              pcVar2 = "seal";
              if (iVar1 != 0) {
                iVar1 = strcmp(param_1,"remap");
                pcVar2 = "remap";
                if (iVar1 != 0) {
                  pcVar2 = "unknown";
                }
              }
            }
          }
        }
      }
    }
  }
  return pcVar2;
}

/* ===== FUN_00168dfc @ 00168dfc ===== */

void FUN_00168dfc(undefined8 param_1,ulong param_2)

{
  if (param_2 == 0xffffffffffffffff) {
    FUN_0015540c(param_1,0xffffffffffffffff,8,&DAT_0013c160);
    return;
  }
  if (param_2 < 0x4001) {
    FUN_0015540c(param_1,0xffffffffffffffff,8,&DAT_001362d1,param_2);
    return;
  }
  FUN_0015540c(param_1,0xffffffffffffffff,8,"other",param_2);
  return;
}

/* ===== FUN_00168e48 @ 00168e48 ===== */

ulong FUN_00168e48(void)

{
  int iVar1;
  ulong uVar2;
  
  uVar2 = FUN_0014fe14(0x85c388,0xf30,
                       "7ac2433a05745125eeb98a2e9fde3e961999a851ec4835c821514fc1394d5f5f");
  if (((((int)uVar2 != 0) &&
       (uVar2 = FUN_0014fe14(0x89902c,200,
                             "8a4832b05aaca5ba119e36d99caf4cc40fb6a3bba9a4966a8a0f029622810e57"),
       (int)uVar2 != 0)) &&
      (uVar2 = FUN_0014fe14(0xf6b008,0x58,
                            "3233ddb0149f97d8f9852ba100a5f932d2a1b48ac5ec10395788f545bc360c1a"),
      (int)uVar2 != 0)) &&
     (((uVar2 = FUN_0014fe14(0x7acd48,0xdc,
                             "7cda3202d7931909d5191380425c5b27717be7fd502a8bad7e27773ce9be9d9f"),
       (int)uVar2 != 0 &&
       (uVar2 = FUN_0014fe14(0xf34914,0x4c,
                             "5beb7a3333c7a671d712d7b1d8554e716dd11d9049a1fda1d9ad6a1ff603dde7"),
       (int)uVar2 != 0)) &&
      ((uVar2 = FUN_0014fe14(0x5418f0,0x2c,
                             "ebfe0ca4dc3231e8a3387c82b01cec33a1546c5870cb87e0cf089b1dd933c647"),
       (int)uVar2 != 0 &&
       (uVar2 = FUN_0014fe14(0x666228,0x38,
                             "6c48afdef5963fe3d415619463af7ec3cc603e2fc2742f57d96d141362fb7f08"),
       (int)uVar2 != 0)))))) {
    iVar1 = FUN_0014fe14(0xac319c,0xe0,
                         "ecc8aa9a360b1061baa9a987f682adb97f925bc7e1792933fb79367621924911");
    uVar2 = (ulong)(iVar1 != 0);
  }
  return uVar2;
}

/* ===== FUN_00168f3c @ 00168f3c ===== */

void * FUN_00168f3c(ulong param_1)

{
  void *__addr;
  long lVar1;
  bool bVar2;
  int iVar3;
  uint uVar4;
  void *__addr_00;
  void *pvVar5;
  ulong uVar6;
  ulong uVar7;
  undefined1 auStack_6c [4];
  long local_68;
  
  lVar1 = tpidr_el0;
  local_68 = *(long *)(lVar1 + 0x28);
  uVar6 = -DAT_0022f020 & param_1;
  uVar7 = 0x100000;
  do {
    __addr_00 = (void *)(uVar6 + uVar7);
    __addr = (void *)0x0;
    if (uVar7 <= uVar6) {
      __addr = (void *)(uVar6 - uVar7);
    }
    if ((__addr_00 != (void *)0x0) &&
       (__addr_00 = mmap(__addr_00,DAT_0022f020,3,0x22,-1,0),
       __addr_00 != (void *)0xffffffffffffffff)) {
      iVar3 = FUN_00190d68(param_1,__addr_00,auStack_6c);
      if ((iVar3 != 0) &&
         (pvVar5 = (void *)FUN_00190d68((long)__addr_00 + 0x200,param_1 + 4,auStack_6c),
         (int)pvVar5 != 0)) goto LAB_00169074;
      uVar4 = munmap(__addr_00,DAT_0022f020);
      __addr_00 = (void *)(ulong)uVar4;
    }
    pvVar5 = __addr_00;
    if ((uVar7 < uVar6) &&
       (__addr_00 = mmap(__addr,DAT_0022f020,3,0x22,-1,0), pvVar5 = __addr_00,
       __addr_00 != (void *)0xffffffffffffffff)) {
      iVar3 = FUN_00190d68(param_1,__addr_00,auStack_6c);
      if ((iVar3 != 0) &&
         (pvVar5 = (void *)FUN_00190d68((long)__addr_00 + 0x200,param_1 + 4,auStack_6c),
         (int)pvVar5 != 0)) goto LAB_00169074;
      uVar4 = munmap(__addr_00,DAT_0022f020);
      pvVar5 = (void *)(ulong)uVar4;
    }
    bVar2 = uVar7 < 0x6f00001;
    uVar7 = uVar7 + 0x100000;
  } while (bVar2);
  __addr_00 = (void *)0x0;
LAB_00169074:
  if (*(long *)(lVar1 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(pvVar5);
  }
  return __addr_00;
}

/* ===== FUN_001690ac @ 001690ac ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_001690ac(long param_1)

{
  long lVar1;
  undefined8 uVar2;
  int iVar3;
  long lVar4;
  timespec local_70 [2];
  int local_50;
  long local_38;
  
  uVar2 = _DAT_0022db6c;
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  if ((0xffffd8f0 < (int)_DAT_0022db6c - 10000U) && (iVar3 = FUN_00159ac0(), iVar3 != 0)) {
    iVar3 = clock_gettime(1,local_70);
    if (iVar3 == 0) {
      lVar4 = local_70[0].tv_sec * 1000 + (ulong)local_70[0].tv_nsec / 1000000;
    }
    else {
      lVar4 = 0;
    }
    FUN_0014ef90(local_70,lVar4);
    if (local_50 != 0) {
      *(undefined8 *)(param_1 + 0xa8) = uVar2;
    }
  }
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00169170 @ 00169170 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_00169170(long param_1,undefined8 param_2)

{
  long lVar1;
  long lVar2;
  long lVar3;
  ulong uVar4;
  int iVar5;
  undefined4 uVar6;
  undefined8 uVar7;
  long lVar8;
  undefined1 auStack_d0 [32];
  int local_b0;
  char local_94 [4];
  long local_90;
  ulong local_88;
  uint local_80;
  undefined4 uStack_7c;
  timespec local_78;
  long local_68;
  
  lVar3 = tpidr_el0;
  local_68 = *(long *)(lVar3 + 0x28);
  local_94[0] = '\x01';
  if (((int)_DAT_0022db68 != 0) && (uVar7 = FUN_00159ac0(), (int)uVar7 != 0)) {
    uVar7 = FUN_0014e7c4(uVar7,param_1 + 0x9d,local_94,1);
    if (((int)uVar7 != 0) &&
       (((local_94[0] == '\0' && (iVar5 = FUN_0014e7c4(uVar7,param_2,&local_80,8), iVar5 != 0)) &&
        (local_80 < 0x100)))) {
      iVar5 = clock_gettime(1,&local_78);
      if (iVar5 == 0) {
        lVar8 = local_78.tv_sec * 1000 + (ulong)local_78.tv_nsec / 1000000;
      }
      else {
        lVar8 = 0;
      }
      uVar7 = FUN_0014ef90(auStack_d0,lVar8);
      lVar8 = DAT_0022d8e0;
      if (local_b0 != 0) {
        local_78.tv_sec = 0;
        local_90 = 0;
        local_88 = 0;
        lVar2 = DAT_0022d8e0 + 0x13053c0;
        uVar7 = FUN_0014e7c4(uVar7,lVar2,&local_78,8);
        if ((((int)uVar7 != 0) && (local_78.tv_sec != 0)) &&
           (((local_78.tv_sec & 7U) == 0 &&
            (uVar7 = FUN_0014e7c4(uVar7,local_78.tv_sec,&local_88,8), uVar4 = local_88,
            (int)uVar7 != 0)))) {
          if (local_88 == lVar8 + 0x11ebcd0U) {
            lVar1 = local_88 + 0x18;
            uVar7 = FUN_0014e7c4(uVar7,lVar1,&local_90,8);
            if ((((int)uVar7 != 0) && (local_90 == lVar8 + 0xac319c)) && (local_78.tv_sec != 0)) {
              local_78.tv_sec = 0;
              local_90 = 0;
              local_88 = 0;
              uVar7 = FUN_0014e7c4(uVar7,lVar2,&local_78,8);
              if (((((((int)uVar7 != 0) && (local_78.tv_sec != 0)) && ((local_78.tv_sec & 7U) == 0))
                   && ((uVar7 = FUN_0014e7c4(uVar7,local_78.tv_sec,&local_88,8), (int)uVar7 != 0 &&
                       (local_88 == uVar4)))) &&
                  ((iVar5 = FUN_0014e7c4(uVar7,lVar1,&local_90,8), iVar5 != 0 &&
                   ((local_90 == lVar8 + 0xac319c && (local_78.tv_sec != 0)))))) &&
                 (local_80 < 0x100)) {
                local_88 = local_88 & 0xffffffffffffff00;
                local_78.tv_sec = CONCAT44(uStack_7c,local_80);
                iVar5 = (*(code *)(lVar8 + 0x7acd48))(&local_88,&local_78);
                if (iVar5 != 0) {
                  uVar6 = 1;
                  local_78.tv_sec = CONCAT71(local_78.tv_sec._1_7_,1);
                  FUN_00153cbc(DAT_001a7c38,&local_78,1,param_1 + 0x9d);
                  (*(code *)(DAT_0022d8e0 + 0x11a2830))(param_2);
                  goto LAB_001693e8;
                }
              }
            }
          }
        }
      }
    }
  }
  uVar6 = (*DAT_00281a40)(param_1,param_2);
LAB_001693e8:
  if (*(long *)(lVar3 + 0x28) == local_68) {
    return uVar6;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00169420 @ 00169420 ===== */

undefined8 FUN_00169420(undefined8 *param_1,int *param_2,int *param_3)

{
  int iVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  
  uVar2 = 0xffffffff;
  if ((((((param_1 != (undefined8 *)0x0) && (param_2 != (int *)0x0)) && (param_3 != (int *)0x0)) &&
       (((*param_2 == 1 && (param_2[1] == 0x38)) &&
        ((*param_3 == 1 && ((param_3[1] == 0x30 && (*(long *)(param_2 + 4) != 0)))))))) &&
      (*(long *)(param_2 + 6) != 0)) &&
     ((*(long *)(param_2 + 8) != 0 && (*(long *)(param_2 + 10) != 0)))) {
    if (((*(char **)(param_3 + 2) == (char *)0x0) ||
        (((iVar1 = strcmp(*(char **)(param_3 + 2),
                          "a10aeb6b4085fb2a15d969a130cd9608231998b5269a40aed41129d99624ede3"),
          iVar1 != 0 || (*(char **)(param_3 + 4) == (char *)0x0)) ||
         (iVar1 = strcmp(*(char **)(param_3 + 4),"fdf834103d333f9f8a1947b3a405b32da6ebb651"),
         iVar1 != 0)))) || (*(ulong *)(param_3 + 10) >> 0x28 != 0)) {
      uVar2 = 0xffffffff;
    }
    else {
      param_1[0x26] = 0;
      uVar2 = 1;
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
      param_1[0x23] = 0;
      param_1[0x22] = 0;
      param_1[0x25] = 0;
      param_1[0x24] = 0;
      param_1[1] = 0;
      *param_1 = 0;
      *(undefined4 *)param_1 = 0;
      *(undefined4 *)((long)param_1 + 4) = 0;
      *(undefined4 *)(param_1 + 1) = 0;
      uVar6 = *(undefined8 *)(param_2 + 4);
      uVar5 = *(undefined8 *)(param_2 + 10);
      uVar4 = *(undefined8 *)(param_2 + 8);
      uVar3 = *(undefined8 *)(param_2 + 0xc);
      uVar8 = *(undefined8 *)(param_2 + 2);
      uVar7 = *(undefined8 *)param_2;
      param_1[0xb] = *(undefined8 *)(param_2 + 6);
      param_1[10] = uVar6;
      param_1[0xd] = uVar5;
      param_1[0xc] = uVar4;
      param_1[0xe] = uVar3;
      param_1[9] = uVar8;
      param_1[8] = uVar7;
      uVar5 = *(undefined8 *)(param_3 + 4);
      uVar4 = *(undefined8 *)(param_3 + 10);
      uVar3 = *(undefined8 *)(param_3 + 8);
      uVar7 = *(undefined8 *)(param_3 + 2);
      uVar6 = *(undefined8 *)param_3;
      param_1[5] = *(undefined8 *)(param_3 + 6);
      param_1[4] = uVar5;
      param_1[7] = uVar4;
      param_1[6] = uVar3;
      *(undefined4 *)((long)param_1 + 0x124) = 1;
      param_1[3] = uVar7;
      param_1[2] = uVar6;
    }
  }
  return uVar2;
}

/* ===== FUN_00169558 @ 00169558 ===== */

void FUN_00169558(long param_1)

{
  if ((param_1 != 0) && (*(int *)(param_1 + 0x124) != 0)) {
    *(undefined4 *)(param_1 + 4) = 1;
  }
  return;
}

/* ===== FUN_00169574 @ 00169574 ===== */

undefined8 FUN_00169574(long param_1)

{
  if ((param_1 != 0) && (*(int *)(param_1 + 0x124) != 0)) {
    return *(undefined8 *)(param_1 + 8);
  }
  return 8;
}

/* ===== FUN_00169594 @ 00169594 ===== */

void FUN_00169594(undefined4 *param_1,int *param_2,ulong param_3,int param_4)

{
  long lVar1;
  int iVar2;
  uint uVar3;
  ulong uVar4;
  undefined8 uVar5;
  char *__s1;
  long lVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  undefined8 uVar10;
  undefined8 local_c0;
  long lStack_b8;
  long lStack_b0;
  long local_a8;
  long lStack_a0;
  undefined8 local_98;
  undefined8 uStack_90;
  long lStack_88;
  long lStack_80;
  long lStack_78;
  long local_70;
  long local_68;
  long lStack_60;
  long local_58;
  
  lVar1 = tpidr_el0;
  local_58 = *(long *)(lVar1 + 0x28);
  if ((param_1 == (undefined4 *)0x0) || (param_1[0x49] == 0)) {
    uVar5 = 0xffffffff;
    goto LAB_001696a8;
  }
  if ((param_4 == 0) || ((int)*(undefined8 *)(param_1 + 1) == 0)) {
    uVar5 = 0;
    goto LAB_001696a8;
  }
  uVar4 = FUN_00193f80(1,param_1);
  if ((uVar4 & 1) != 0) {
    uVar5 = 2;
    goto LAB_001696a8;
  }
  if ((param_1[0x49] == 0) || (7 < (int)*(undefined8 *)(param_1 + 2))) {
LAB_00169688:
    uVar5 = 0xffffffff;
  }
  else {
    if ((((param_2 == (int *)0x0) ||
         (((*param_2 != 1 || (param_2[1] != 0x30)) ||
          (uVar4 = *(ulong *)(param_2 + 2), 0x7ffffffffebeffff < uVar4 - 0x10000)))) ||
        ((((uVar4 & 0xfff) != 0 || (lVar6 = *(long *)(param_2 + 8), lVar6 == 0)) ||
         (*(char **)(param_2 + 4) == (char *)0x0)))) ||
       (((__s1 = *(char **)(param_2 + 6), __s1 == (char *)0x0 ||
         (iVar2 = strcmp(*(char **)(param_2 + 4),*(char **)(param_1 + 6)), iVar2 != 0)) ||
        ((iVar2 = strcmp(__s1,*(char **)(param_1 + 8)), iVar2 != 0 ||
         ((param_1[0x4b] != 0 &&
          (((*(ulong *)(param_1 + 0x3a) != uVar4 || (*(long *)(param_1 + 0x40) != lVar6)) ||
           (*(long *)(param_1 + 0x42) != *(long *)(param_2 + 10))))))))))) {
      uVar5 = 8;
LAB_00169684:
      FUN_001699bc(param_1,uVar5);
      goto LAB_00169688;
    }
    if ((param_1[0x4c] == 0) || (param_1[0x4d] == 0)) {
      if (param_1[0x4a] == 0) {
LAB_00169734:
        param_1[0x4a] = 1;
        if (*(long *)(param_1 + 0x44) != -1) {
          *(long *)(param_1 + 0x44) = *(long *)(param_1 + 0x44) + 1;
        }
        *(ulong *)(param_1 + 0x46) = param_3;
        if (param_1[0x4c] == 0) {
          if (*(long *)(param_1 + 0x20) == 0) {
            lStack_a0 = 0;
            local_a8 = 0;
            lStack_b0 = 0;
            lStack_b8 = 0;
            local_c0 = DAT_0010f7d8;
            iVar2 = (**(code **)(param_1 + 0x14))(*(undefined8 *)(param_1 + 0x12),&local_c0);
            if (iVar2 == 1) {
              if ((((((int)local_c0 == 1) && (local_c0._4_4_ == 0x28)) && (lStack_b8 != 0)) &&
                  ((lStack_b0 != 0 && (local_a8 != 0)))) && (lStack_a0 != 0)) {
                *(long *)(param_1 + 0x20) = lStack_b8;
                *(undefined8 *)(param_1 + 0x1e) = local_c0;
                *(long *)(param_1 + 0x24) = local_a8;
                *(long *)(param_1 + 0x22) = lStack_b0;
                *(long *)(param_1 + 0x26) = lStack_a0;
                uVar8 = *(undefined8 *)(param_2 + 4);
                uVar7 = *(undefined8 *)(param_2 + 10);
                uVar5 = *(undefined8 *)(param_2 + 8);
                uVar10 = *(undefined8 *)(param_2 + 2);
                uVar9 = *(undefined8 *)param_2;
                *(undefined8 *)(param_1 + 0x3e) = *(undefined8 *)(param_2 + 6);
                *(undefined8 *)(param_1 + 0x3c) = uVar8;
                *(undefined8 *)(param_1 + 0x42) = uVar7;
                *(undefined8 *)(param_1 + 0x40) = uVar5;
                param_1[0x4b] = 1;
                *(undefined8 *)(param_1 + 0x3a) = uVar10;
                *(undefined8 *)(param_1 + 0x38) = uVar9;
                goto LAB_00169790;
              }
              if (lStack_b8 != 0) {
                (**(code **)(param_1 + 0x16))(*(undefined8 *)(param_1 + 0x12));
              }
            }
            else {
              if (lStack_b8 != 0) {
                (**(code **)(param_1 + 0x16))(*(undefined8 *)(param_1 + 0x12));
              }
              if (-1 < iVar2) {
                uVar5 = 1;
                goto LAB_001697bc;
              }
            }
LAB_001698d8:
            uVar5 = 9;
            goto LAB_00169684;
          }
LAB_00169790:
          iVar2 = (**(code **)(param_1 + 0x26))(param_2);
          if (iVar2 == 1) {
            lStack_60 = 0;
            lStack_78 = 0;
            lStack_80 = 0;
            local_68 = 0;
            local_70 = 0;
            lStack_88 = 0;
            uStack_90 = 0;
            local_98 = DAT_0010f930;
            uVar3 = (**(code **)(param_1 + 0x24))(&local_98);
            if ((uVar3 < 4) && (uVar3 != 1)) {
              uVar5 = 3;
            }
            else {
              if (((uVar3 != 1) ||
                  ((((int)local_98 != 1 || (local_98._4_4_ != 0x40)) || (local_70 == 0)))) ||
                 (((local_68 == 0 || (lStack_60 == 0)) ||
                  ((lStack_88 != *(long *)(param_1 + 10) || lStack_80 != *(long *)(param_1 + 0xc))
                   || lStack_78 != *(long *)(param_1 + 0xe))))) {
                uVar5 = 10;
                goto LAB_00169684;
              }
              uVar3 = (**(code **)(param_1 + 0x18))(*(undefined8 *)(param_1 + 0x12),0,&local_98);
              if ((uVar3 & 0xfffffffe) == 2) {
                uVar5 = 4;
              }
              else {
                if (uVar3 == 1) {
                  param_1[0x4c] = 1;
                  *(undefined8 *)(param_1 + 0x2a) = uStack_90;
                  *(undefined8 *)(param_1 + 0x28) = local_98;
                  *(long *)(param_1 + 0x2e) = lStack_80;
                  *(long *)(param_1 + 0x2c) = lStack_88;
                  *(long *)(param_1 + 0x32) = local_70;
                  *(long *)(param_1 + 0x30) = lStack_78;
                  *(long *)(param_1 + 0x36) = lStack_60;
                  *(long *)(param_1 + 0x34) = local_68;
                  FUN_00169a50(param_1,7);
                  goto LAB_00169758;
                }
                uVar5 = 5;
              }
            }
          }
          else {
            if (iVar2 != 0) goto LAB_001698d8;
            uVar5 = 2;
          }
        }
        else {
LAB_00169758:
          iVar2 = (**(code **)(param_1 + 0x1a))(*(undefined8 *)(param_1 + 0x12));
          if (iVar2 == 1) {
            param_1[0x4d] = 1;
            FUN_00169a50(param_1,7);
            goto LAB_00169780;
          }
          uVar5 = 6;
        }
LAB_001697bc:
        FUN_00169a50(param_1,uVar5);
        uVar5 = 0;
      }
      else {
        uVar5 = 0;
        if ((*(ulong *)(param_1 + 0x46) <= param_3) && (0xf9 < param_3 - *(ulong *)(param_1 + 0x46))
           ) goto LAB_00169734;
      }
    }
    else {
LAB_00169780:
      uVar5 = 1;
    }
  }
  *param_1 = 0;
LAB_001696a8:
  if (*(long *)(lVar1 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar5);
}

/* ===== FUN_001699bc @ 001699bc ===== */

void FUN_001699bc(long param_1,int param_2)

{
  int iVar1;
  
  if ((*(int *)(param_1 + 0x130) == 0) && (*(long *)(param_1 + 0x80) != 0)) {
    (**(code **)(param_1 + 0x58))(*(undefined8 *)(param_1 + 0x48));
    *(undefined8 *)(param_1 + 0x98) = 0;
    *(undefined8 *)(param_1 + 0x90) = 0;
    *(undefined8 *)(param_1 + 0x88) = 0;
    *(undefined8 *)(param_1 + 0x80) = 0;
    *(undefined8 *)(param_1 + 0x78) = 0;
  }
  iVar1 = FUN_00193fb0(param_2,param_1 + 8);
  if (((iVar1 != param_2) && (*(code **)(param_1 + 0x70) != (code *)0x0)) &&
     (*(uint *)(param_1 + 0x120) < 0xc)) {
    *(uint *)(param_1 + 0x120) = *(uint *)(param_1 + 0x120) + 1;
                    /* WARNING: Could not recover jumptable at 0x00169a40. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(param_1 + 0x70))
              (*(undefined8 *)(param_1 + 0x48),param_2,*(undefined8 *)(param_1 + 0x110));
    return;
  }
  return;
}

/* ===== FUN_00169a50 @ 00169a50 ===== */

void FUN_00169a50(long param_1,int param_2)

{
  int iVar1;
  
  iVar1 = FUN_00193fb0(param_2,param_1 + 8);
  if (((iVar1 != param_2) && (*(code **)(param_1 + 0x70) != (code *)0x0)) &&
     (*(uint *)(param_1 + 0x120) < 0xc)) {
    *(uint *)(param_1 + 0x120) = *(uint *)(param_1 + 0x120) + 1;
                    /* WARNING: Could not recover jumptable at 0x00169aa8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (**(code **)(param_1 + 0x70))
              (*(undefined8 *)(param_1 + 0x48),param_2,*(undefined8 *)(param_1 + 0x110));
    return;
  }
  return;
}

/* ===== FUN_00169ab8 @ 00169ab8 ===== */

char * FUN_00169ab8(uint param_1)

{
  if (param_1 < 0xb) {
    return &DAT_0013cc84 + *(int *)(&DAT_0013cc84 + (ulong)param_1 * 4);
  }
  return "binding_state_invalid";
}

/* ===== FUN_00169ae0 @ 00169ae0 ===== */

void FUN_00169ae0(long param_1,char *param_2,char *param_3,byte *param_4)

{
  byte bVar1;
  long lVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  __uid_t _Var6;
  undefined8 uVar7;
  ulong uVar8;
  size_t sVar9;
  int *piVar10;
  long lVar11;
  byte *pbVar12;
  undefined1 auStack_2ef [15];
  char *local_2e0;
  undefined4 local_2d8;
  undefined4 uStack_2d4;
  long lStack_2d0;
  long local_2c8;
  long lStack_2c0;
  long local_2b8;
  long lStack_2b0;
  long local_2a8;
  long lStack_2a0;
  char local_298;
  char acStack_297 [181];
  char acStack_1e2 [10];
  stat local_1d8 [2];
  char acStack_98 [14];
  char acStack_8a [50];
  long local_58;
  
  lVar2 = tpidr_el0;
  local_58 = *(long *)(lVar2 + 0x28);
  uVar7 = 0;
  if ((param_1 == 0) || (param_2 == (char *)0x0)) goto LAB_00169c0c;
  iVar3 = strcmp(param_2,"bsd.suitcase.nexusv2");
  uVar7 = 0;
  if ((param_3 == (char *)0x0) || (iVar3 != 0)) goto LAB_00169c0c;
  iVar3 = strcmp(param_3,"libNexusEvasion69252.so");
  if ((((iVar3 == 0) || (iVar3 = strcmp(param_3,"libNexusEvasionRuntime69252.so"), iVar3 == 0)) &&
      (iVar3 = FUN_00169f00(param_1,acStack_98), iVar3 != 0)) &&
     ((((uVar8 = readlink(acStack_98,(char *)local_1d8,0x13f), 0xfffffffffffffec1 < uVar8 - 0x13f &&
        (*(undefined1 *)((long)local_1d8[0].__unused + (uVar8 - 0x78)) = 0, 0x17 < uVar8)) &&
       (CONCAT35(local_1d8[0].st_dev._5_3_,(undefined5)local_1d8[0].st_dev) == 0x6e3a64666d656d2f &&
        CONCAT53((undefined5)local_1d8[0].st_ino,local_1d8[0].st_dev._5_3_) == 0x2d737578656e3a64))
      && ((iVar3 = strcmp(acStack_1e2 + uVar8," (deleted)"), iVar3 == 0 && (uVar8 - 0x17 < 0x100))))
     )) {
    __memcpy_chk(&local_2d8,(undefined1 *)((long)&local_1d8[0].st_ino + 5),uVar8 - 0x17,0x100);
    auStack_2ef[uVar8] = 0;
    if (local_2d8 == 0x69726373 && CONCAT31((undefined3)uStack_2d4,local_2d8._3_1_) == 0x2d747069) {
      if (((((param_4 != (byte *)0x0) && (sVar9 = strlen((char *)param_4), sVar9 == 0x40)) &&
           (sVar9 = strlen(param_3), 2 < sVar9)) &&
          ((iVar3 = strcmp(param_3 + (sVar9 - 3),".js"), iVar3 == 0 ||
           ((3 < sVar9 && (iVar3 = strcmp(param_3 + (sVar9 - 4),".ngs"), iVar3 == 0)))))) &&
         (lVar11 = __strlen_chk(&local_2d8,0x100), lVar11 == 0x27)) {
        lVar11 = 0;
        do {
          uVar4 = (uint)*(byte *)((long)&uStack_2d4 + lVar11 + 3);
          if (9 < uVar4 - 0x30 && 5 < uVar4 - 0x61) goto LAB_00169c08;
          lVar11 = lVar11 + 1;
        } while (lVar11 != 0x20);
        bVar1 = *param_4;
        if (bVar1 != 0) {
          pbVar12 = param_4 + 1;
          do {
            if ((9 < bVar1 - 0x30) && (5 < bVar1 - 0x61)) goto LAB_00169c08;
            bVar1 = *pbVar12;
            pbVar12 = pbVar12 + 1;
          } while (bVar1 != 0);
        }
LAB_00169e0c:
        uVar7 = FUN_00169f00(param_1,acStack_98);
        if ((int)uVar7 == 0) goto LAB_00169c0c;
        local_2e0 = (char *)0x0;
        piVar10 = (int *)__errno();
        *piVar10 = 0;
        lVar11 = strtol(acStack_8a,&local_2e0,10);
        if (((((*piVar10 == 0) && (local_2e0 != (char *)0x0)) && (*local_2e0 == '\0')) &&
            ((-1 < lVar11 && (lVar11 < 0x80000000)))) &&
           (iVar3 = fcntl((int)lVar11,0x406,0), -1 < iVar3)) {
          uVar4 = fcntl(iVar3,0x40a);
          iVar5 = fstat(iVar3,local_1d8);
          if (((iVar5 == 0) && (((uint)local_1d8[0].st_nlink & 0xf000) == 0x8000)) &&
             ((_Var6 = getuid(), local_1d8[0].st_mode == _Var6 &&
              (((local_1d8[0].st_nlink._4_4_ == 0 && (-1 < (int)uVar4)) && ((uVar4 & 0xf) == 0xf))))
             )) {
            close(iVar3);
            uVar7 = 1;
            goto LAB_00169c0c;
          }
          close(iVar3);
        }
      }
    }
    else {
      uVar8 = __strlen_chk(&local_2d8,0x100);
      if ((0x41 < uVar8) && (local_298 == '-')) {
        lVar11 = 0;
        do {
          if (9 < *(byte *)((long)&local_2d8 + lVar11) - 0x30 &&
              5 < *(byte *)((long)&local_2d8 + lVar11) - 0x61) goto LAB_00169c08;
          lVar11 = lVar11 + 1;
        } while (lVar11 != 0x40);
        iVar3 = strcmp(param_3,acStack_297);
        if ((iVar3 == 0) &&
           ((param_4 == (byte *)0x0 ||
            ((sVar9 = strlen((char *)param_4), sVar9 == 0x40 &&
             (((((((*(long *)param_4 == CONCAT44(uStack_2d4,local_2d8) &&
                   *(long *)(param_4 + 8) == lStack_2d0) && *(long *)(param_4 + 0x10) == local_2c8)
                 && *(long *)(param_4 + 0x18) == lStack_2c0) &&
                *(long *)(param_4 + 0x20) == local_2b8) && *(long *)(param_4 + 0x28) == lStack_2b0)
              && *(long *)(param_4 + 0x30) == local_2a8) && *(long *)(param_4 + 0x38) == lStack_2a0)
             ))))) goto LAB_00169e0c;
      }
    }
  }
LAB_00169c08:
  uVar7 = 0;
LAB_00169c0c:
  if (*(long *)(lVar2 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar7);
}

/* ===== FUN_00169ef0 @ 00169ef0 ===== */

void FUN_00169ef0(undefined8 param_1,undefined8 param_2)

{
  FUN_00169ae0(param_1,param_2,"libNexusEvasion69252.so",0);
  return;
}

/* ===== FUN_00169f00 @ 00169f00 ===== */

/* WARNING: Removing unreachable block (ram,0x0016a020) */

void FUN_00169f00(char *param_1,char *param_2)

{
  long lVar1;
  bool bVar2;
  bool bVar3;
  bool bVar4;
  int iVar5;
  size_t sVar6;
  DIR *__dirp;
  dirent *pdVar7;
  ssize_t sVar8;
  uint uVar9;
  long lVar10;
  __ino_t _Var11;
  __ino_t _Var12;
  __dev_t _Var13;
  __dev_t _Var14;
  stat local_270 [3];
  char acStack_b0 [64];
  long local_70;
  
  lVar1 = tpidr_el0;
  local_70 = *(long *)(lVar1 + 0x28);
  iVar5 = strncmp(param_1,"/proc/self/fd/",0xe);
  if ((iVar5 == 0) && (uVar9 = (uint)(byte)param_1[0xe], param_1[0xe] != 0)) {
    lVar10 = 0xf;
    do {
      if (uVar9 - 0x3a < 0xfffffff6) goto LAB_0016a0d8;
      uVar9 = (uint)(byte)param_1[lVar10];
      lVar10 = lVar10 + 1;
    } while (uVar9 != 0);
    sVar6 = strlen(param_1);
    if (sVar6 < 0x40) {
      strcpy(param_2,param_1);
      __dirp = (DIR *)0x1;
      goto LAB_0016a0dc;
    }
  }
  else {
    iVar5 = strncmp(param_1,"/memfd:nexus-",0xd);
    if ((iVar5 == 0) && (sVar6 = strlen(param_1), sVar6 < 0x140)) {
      __dirp = opendir("/proc/self/fd");
      if (__dirp != (DIR *)0x0) {
        _Var13 = 0;
        _Var11 = 0;
        bVar2 = false;
        bVar3 = false;
        do {
          do {
            do {
              pdVar7 = readdir(__dirp);
              while( true ) {
                if (pdVar7 == (dirent *)0x0) goto LAB_0016a10c;
                if ((pdVar7->d_name[0] != '.') && (sVar6 = strlen(pdVar7->d_name), sVar6 < 0x15))
                break;
                pdVar7 = readdir(__dirp);
              }
              FUN_0016a13c(acStack_b0);
              sVar8 = readlink(acStack_b0,(char *)(local_270[0].__unused + 1),0x13f);
            } while (sVar8 - 0x13fU < 0xfffffffffffffec2);
            *(undefined1 *)((long)local_270[0].__unused + sVar8 + 8) = 0;
            iVar5 = strcmp((char *)(local_270[0].__unused + 1),param_1);
          } while (iVar5 != 0);
          iVar5 = stat(acStack_b0,local_270);
          if (iVar5 == 0) {
            _Var12 = local_270[0].st_ino;
            _Var14 = local_270[0].st_dev;
            if (bVar2) {
              bVar2 = true;
              iVar5 = 6;
              if (local_270[0].st_dev == _Var13) {
                _Var12 = _Var11;
                _Var14 = _Var13;
                bVar4 = true;
                if (local_270[0].st_ino == _Var11) goto LAB_0016a074;
              }
              else {
                bVar4 = true;
              }
            }
            else {
LAB_0016a074:
              strcpy(param_2,acStack_b0);
              iVar5 = 0;
              bVar2 = true;
              _Var11 = _Var12;
              _Var13 = _Var14;
              bVar4 = bVar3;
            }
          }
          else {
            iVar5 = 6;
            bVar4 = true;
          }
          bVar3 = bVar4;
        } while (iVar5 != 6);
LAB_0016a10c:
        closedir(__dirp);
        __dirp = (DIR *)(ulong)(bVar2 && !bVar3);
      }
      goto LAB_0016a0dc;
    }
  }
LAB_0016a0d8:
  __dirp = (DIR *)0x0;
LAB_0016a0dc:
  if (*(long *)(lVar1 + 0x28) != local_70) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(__dirp);
  }
  return;
}

/* ===== FUN_0016a13c @ 0016a13c ===== */

void FUN_0016a13c(char *param_1)

{
  long lVar1;
  int iVar2;
  undefined8 in_x4;
  undefined8 in_x5;
  undefined8 in_x6;
  undefined8 in_x7;
  long lVar3;
  undefined8 local_90;
  undefined8 local_88;
  undefined8 uStack_80;
  undefined8 local_78;
  undefined1 *local_70;
  undefined1 **ppuStack_68;
  undefined8 *puStack_60;
  undefined8 uStack_58;
  
  ppuStack_68 = &local_70;
  puStack_60 = &local_90;
  lVar1 = tpidr_el0;
  lVar3 = *(long *)(lVar1 + 0x28);
  uStack_58 = 0xffffff80ffffffe0;
  local_90 = in_x4;
  local_88 = in_x5;
  uStack_80 = in_x6;
  local_78 = in_x7;
  local_70 = (undefined1 *)register0x00000008;
  iVar2 = vsnprintf(param_1,0x40,"/proc/self/fd/%s",&local_70);
  if (*(long *)(lVar1 + 0x28) == lVar3) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar2);
}

/* ===== FUN_0016ba38 @ 0016ba38 ===== */

void FUN_0016ba38(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
                 undefined8 param_5,undefined8 param_6,undefined8 param_7,undefined8 param_8,
                 undefined8 param_9)

{
  long lVar1;
  long lVar2;
  undefined8 local_90;
  undefined8 local_88;
  undefined8 uStack_80;
  undefined8 local_78;
  undefined1 *local_70;
  undefined1 **ppuStack_68;
  undefined8 *puStack_60;
  undefined8 uStack_58;
  
  ppuStack_68 = &local_70;
  puStack_60 = &local_90;
  lVar1 = tpidr_el0;
  lVar2 = *(long *)(lVar1 + 0x28);
  uStack_58 = 0xffffff80ffffffe0;
  local_90 = param_6;
  local_88 = param_7;
  uStack_80 = param_8;
  local_78 = param_9;
  local_70 = (undefined1 *)register0x00000008;
  __vsnprintf_chk(param_2,0x80,0,param_3,param_5,&local_70,param_8,param_9,param_1);
  if (*(long *)(lVar1 + 0x28) == lVar2) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0016cca0 @ 0016cca0 ===== */

float FUN_0016cca0(float param_1,undefined1 param_2)

{
  long lVar1;
  float fVar2;
  
  lVar1 = __strchr_chk(&DAT_0013a4b5,param_2,5);
  if (lVar1 == 0) {
    lVar1 = __strchr_chk(&DAT_001384ef,param_2,5);
    if (lVar1 == 0) {
      lVar1 = __strchr_chk(&DAT_00135d51,param_2,3);
      fVar2 = 5.0;
      if (lVar1 == 0) {
        lVar1 = __strchr_chk("Jjrft",param_2,6);
        if (lVar1 == 0) {
          lVar1 = __strchr_chk(&DAT_0013c18d,param_2,5);
          if (lVar1 == 0) {
            lVar1 = __strchr_chk(&DAT_00131fb4,param_2,5);
            if (lVar1 == 0) {
              lVar1 = __strchr_chk("ABDGHKNOPQRUVXY",param_2,0x10);
              fVar2 = 7.2;
              if (lVar1 != 0) {
                fVar2 = 8.2;
              }
            }
            else {
              fVar2 = 10.8;
            }
          }
          else {
            fVar2 = 6.2;
          }
        }
        else {
          fVar2 = 4.8;
        }
      }
    }
    else {
      fVar2 = 2.6;
    }
  }
  else {
    fVar2 = 3.2;
  }
  return fVar2 * param_1;
}

/* ===== FUN_0016cde4 @ 0016cde4 ===== */

void FUN_0016cde4(undefined8 param_1)

{
  DAT_00281a48 = param_1;
  return;
}

/* ===== FUN_0016cdf0 @ 0016cdf0 ===== */

void FUN_0016cdf0(void)

{
                    /* WARNING: Could not recover jumptable at 0x0016ce04. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_00281a48 + 0x11a2840))();
  return;
}

/* ===== FUN_0016ce08 @ 0016ce08 ===== */

void FUN_0016ce08(undefined8 param_1)

{
                    /* WARNING: Could not recover jumptable at 0x0016ce1c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_00281a48 + 0x593f14))(param_1,0);
  return;
}

/* ===== FUN_0016ce20 @ 0016ce20 ===== */

void FUN_0016ce20(void)

{
                    /* WARNING: Could not recover jumptable at 0x0016ce30. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_00281a48 + 0x594e80))();
  return;
}

/* ===== FUN_0016ce34 @ 0016ce34 ===== */

void FUN_0016ce34(void)

{
                    /* WARNING: Could not recover jumptable at 0x0016ce44. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_00281a48 + 0x887c14))();
  return;
}

/* ===== FUN_0016ce48 @ 0016ce48 ===== */

void FUN_0016ce48(undefined8 param_1)

{
                    /* WARNING: Could not recover jumptable at 0x0016ce68. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_00281a48 + 0x51ecec))("sc/ui.sc",param_1,1);
  return;
}

/* ===== FUN_0016ce6c @ 0016ce6c ===== */

void FUN_0016ce6c(undefined8 param_1,undefined8 param_2)

{
                    /* WARNING: Could not recover jumptable at 0x0016ce80. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_00281a48 + 0x772a30))(param_1,param_2,1);
  return;
}

/* ===== FUN_0016ce84 @ 0016ce84 ===== */

void FUN_0016ce84(void)

{
                    /* WARNING: Could not recover jumptable at 0x0016ce94. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_00281a48 + 0x5d7c30))();
  return;
}

/* ===== FUN_0016ce98 @ 0016ce98 ===== */

void FUN_0016ce98(void)

{
                    /* WARNING: Could not recover jumptable at 0x0016cea8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_00281a48 + 0x5d8ad4))();
  return;
}

/* ===== FUN_0016ceac @ 0016ceac ===== */

void FUN_0016ceac(void)

{
                    /* WARNING: Could not recover jumptable at 0x0016cebc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_00281a48 + 0x5921a0))();
  return;
}

/* ===== FUN_0016cec0 @ 0016cec0 ===== */

void FUN_0016cec0(undefined8 param_1,undefined8 param_2)

{
                    /* WARNING: Could not recover jumptable at 0x0016ced4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_00281a48 + 0x88836c))(param_1,param_2,1);
  return;
}

/* ===== FUN_0016ced8 @ 0016ced8 ===== */

void FUN_0016ced8(void)

{
                    /* WARNING: Could not recover jumptable at 0x0016cee8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_00281a48 + 0x66ae58))();
  return;
}

/* ===== FUN_0016ceec @ 0016ceec ===== */

void FUN_0016ceec(void)

{
                    /* WARNING: Could not recover jumptable at 0x0016cefc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_00281a48 + 0x592250))();
  return;
}

/* ===== FUN_0016cf00 @ 0016cf00 ===== */

void FUN_0016cf00(undefined1 param_1 [16],undefined4 param_2,undefined8 param_3)

{
  (*(code *)(DAT_00281a48 + 0x59533c))();
                    /* WARNING: Could not recover jumptable at 0x0016cf58. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_00281a48 + 0x595344))(param_2,param_3);
  return;
}

/* ===== FUN_0016cf5c @ 0016cf5c ===== */

void FUN_0016cf5c(void)

{
                    /* WARNING: Could not recover jumptable at 0x0016cf6c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_00281a48 + 0x595314))();
  return;
}

/* ===== FUN_0016cf70 @ 0016cf70 ===== */

void FUN_0016cf70(void)

{
                    /* WARNING: Could not recover jumptable at 0x0016cf80. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_00281a48 + 0x5958e8))();
  return;
}

/* ===== FUN_0016cf84 @ 0016cf84 ===== */

void FUN_0016cf84(undefined8 param_1,undefined8 param_2)

{
                    /* WARNING: Could not recover jumptable at 0x0016cfa0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_00281a48 + 0x595378))(param_1,0,param_2,0);
  return;
}

/* ===== FUN_0016cfa4 @ 0016cfa4 ===== */

void FUN_0016cfa4(void)

{
                    /* WARNING: Could not recover jumptable at 0x0016cfb4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_00281a48 + 0x594130))();
  return;
}

/* ===== FUN_0016cfb8 @ 0016cfb8 ===== */

void FUN_0016cfb8(void)

{
                    /* WARNING: Could not recover jumptable at 0x0016cfc8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_00281a48 + 0x5988ec))();
  return;
}

/* ===== FUN_0016cfcc @ 0016cfcc ===== */

void FUN_0016cfcc(undefined8 param_1,undefined8 param_2)

{
  (*(code *)(DAT_00281a48 + 0xd46898))(param_1,param_2,0,0);
                    /* WARNING: Could not recover jumptable at 0x0016d01c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_00281a48 + 0xd464f8))(param_1);
  return;
}

/* ===== FUN_0016d5cc @ 0016d5cc ===== */

void FUN_0016d5cc(ulong param_1,ulong param_2)

{
  ulong uVar1;
  long lVar2;
  ulong uVar3;
  long lVar4;
  bool bVar5;
  bool bVar6;
  int iVar7;
  ushort local_58 [2];
  int local_54;
  ulong local_50;
  long local_48;
  
  lVar4 = tpidr_el0;
  bVar6 = false;
  local_48 = *(long *)(lVar4 + 0x28);
  local_54 = -1;
  local_58[0] = 0;
  if ((param_1 == 0) || (param_2 == 0)) goto LAB_0016d72c;
  uVar1 = param_1 + 0x40;
  local_50 = 0;
  if (uVar1 >> 3 < 0x201) {
    bVar6 = false;
  }
  else {
    iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1 + 0x38,&local_50,8);
    bVar6 = iVar7 == 1;
  }
  bVar5 = false;
  if (0xfff < local_50) {
    bVar5 = bVar6;
  }
  uVar3 = local_50;
  if (!(bool)(bVar5 & (local_50 & 7) == 0)) {
    uVar3 = 0;
  }
  if (uVar3 == param_2) {
    bVar6 = false;
    if ((uVar1 < 0x1000) || ((param_1 & 0xfffffffffffffffc) == 0xffffffffffffffbc))
    goto LAB_0016d72c;
    iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar1,&local_54,4);
    bVar6 = false;
    if ((iVar7 != 1) || (local_54 < 0)) goto LAB_0016d72c;
    bVar6 = false;
    if ((param_2 + 0x4e < 0x1000) || ((param_2 & 0xfffffffffffffffe) == 0xffffffffffffffb0))
    goto LAB_0016d72c;
    iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,param_2 + 0x4e,local_58,2);
    if ((iVar7 == 1) &&
       ((local_54 < (int)(uint)local_58[0] && (local_50 = 0, 0x200 < param_2 + 0x58 >> 3)))) {
      iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,param_2 + 0x50,&local_50,8);
      uVar1 = local_50;
      bVar6 = false;
      if ((iVar7 == 1) && ((0xfff < local_50 && ((local_50 & 7) == 0)))) {
        local_50 = 0;
        lVar2 = uVar1 + (long)local_54 * 8;
        if (lVar2 + 8U >> 3 < 0x201) {
          bVar6 = false;
        }
        else {
          iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,lVar2,&local_50,8);
          bVar6 = iVar7 == 1;
        }
        bVar5 = false;
        if (0xfff < local_50) {
          bVar5 = bVar6;
        }
        uVar1 = local_50;
        if (!(bool)(bVar5 & (local_50 & 7) == 0)) {
          uVar1 = 0;
        }
        bVar6 = uVar1 == param_1;
      }
      goto LAB_0016d72c;
    }
  }
  bVar6 = false;
LAB_0016d72c:
  if (*(long *)(lVar4 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(bVar6);
}

/* ===== FUN_0016d800 @ 0016d800 ===== */

void FUN_0016d800(ulong param_1,ulong param_2)

{
  undefined4 uVar1;
  ulong uVar2;
  long lVar3;
  bool bVar4;
  ulong uVar5;
  bool bVar6;
  int iVar7;
  undefined4 uVar8;
  uint uVar9;
  ulong local_60;
  long local_58;
  
  lVar3 = tpidr_el0;
  local_58 = *(long *)(lVar3 + 0x28);
  if (param_1 != 0) {
    uVar9 = 0;
    do {
      uVar8 = 1;
      if ((param_1 == param_2) || (0xf < uVar9)) goto LAB_0016d8d8;
      local_60 = 0;
      if (param_1 + 0x40 >> 3 < 0x201) {
        bVar6 = false;
      }
      else {
        iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1 + 0x38,&local_60,8);
        bVar6 = iVar7 == 1;
      }
      uVar5 = local_60;
      bVar4 = false;
      if (0xfff < local_60) {
        bVar4 = bVar6;
      }
      bVar4 = (bool)(bVar4 & (local_60 & 7) == 0);
      uVar2 = local_60;
      if (!bVar4) {
        uVar2 = 0;
      }
      iVar7 = FUN_0016d5cc(param_1,uVar2);
      if (iVar7 == 0) {
        uVar8 = 0;
        goto LAB_0016d8d8;
      }
      uVar9 = uVar9 + 1;
      param_1 = uVar5;
    } while (bVar4);
    param_1 = 0;
  }
  uVar8 = 1;
LAB_0016d8d8:
  uVar1 = 0;
  if (param_1 == param_2) {
    uVar1 = uVar8;
  }
  if (*(long *)(lVar3 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar1);
}

/* ===== FUN_0016d910 @ 0016d910 ===== */

void FUN_0016d910(void)

{
  ulong uVar1;
  long lVar2;
  bool bVar3;
  bool bVar4;
  int iVar5;
  char local_4c [4];
  undefined8 local_48;
  float local_40;
  float local_3c;
  long local_38;
  
  lVar2 = tpidr_el0;
  local_38 = *(long *)(lVar2 + 0x28);
  local_4c[0] = '\x01';
  local_48 = 0;
  if (DAT_001a7cd0 + 0x12eb9f8U >> 3 < 0x201) {
    bVar4 = false;
  }
  else {
    iVar5 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_001a7cd0 + 0x12eb9f0,&local_48,8);
    bVar4 = iVar5 == 1;
  }
  bVar3 = false;
  if (0xfff < local_48) {
    bVar3 = bVar4;
  }
  uVar1 = local_48;
  if (!(bool)(bVar3 & (local_48 & 7) == 0)) {
    uVar1 = 0;
  }
  if (uVar1 == DAT_001a7d28) {
    local_48 = 0;
    if (uVar1 + 0x98 >> 3 < 0x201) {
      bVar4 = false;
    }
    else {
      iVar5 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar1 + 0x90,&local_48,8);
      bVar4 = iVar5 == 1;
    }
    bVar3 = false;
    if (0xfff < local_48) {
      bVar3 = bVar4;
    }
    uVar1 = local_48;
    if (!(bool)(bVar3 & (local_48 & 7) == 0)) {
      uVar1 = 0;
    }
    if ((uVar1 == DAT_001a7d30) && (0x1000 < DAT_001a7d28 + 0x19d)) {
      iVar5 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_001a7d28 + 0x19c,local_4c,1);
      bVar4 = false;
      if ((iVar5 != 1) || (local_4c[0] != '\0')) goto LAB_0016da44;
      if (((0xfff < DAT_001a7d30 + 0x10) &&
          ((DAT_001a7d30 & 0xfffffffffffffff0) != 0xffffffffffffffe0)) &&
         (iVar5 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_001a7d30 + 0x10,&local_48,0x10), iVar5 == 1)) {
        if ((ABS((float)local_48) != INFINITY) && (!NAN(ABS((float)local_48)))) {
          if (((ABS(local_48._4_4_) != INFINITY) &&
              ((((!NAN(ABS(local_48._4_4_)) && (ABS(local_40) != INFINITY)) &&
                ((!NAN(ABS(local_40)) &&
                 ((((ABS(local_3c) != INFINITY && (!NAN(ABS(local_3c)))) && (local_48._4_4_ == 0.0))
                  && ((local_40 == 0.0 && (0.0 < (float)local_48)))))))) && (0.0 < local_3c)))) &&
             ((((float)local_48 == 128.0 || (float)local_48 < 128.0 != NAN((float)local_48) &&
               (local_3c == 128.0 || local_3c < 128.0 != NAN(local_3c))) && ((float)local_48 == 1.0)
              ))) {
            bVar4 = local_3c == 1.0;
            goto LAB_0016da44;
          }
        }
      }
    }
  }
  bVar4 = false;
LAB_0016da44:
  if (*(long *)(lVar2 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(bVar4);
}

/* ===== FUN_0016db8c @ 0016db8c ===== */

void FUN_0016db8c(void)

{
  long lVar1;
  bool bVar2;
  int iVar3;
  char local_2c [4];
  long local_28;
  
  lVar1 = tpidr_el0;
  local_28 = *(long *)(lVar1 + 0x28);
  local_2c[0] = '\x01';
  if (DAT_001a7d28 + 0x19dU < 0x1001) {
    bVar2 = false;
  }
  else {
    iVar3 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_001a7d28 + 0x19c,local_2c,1);
    bVar2 = iVar3 == 1 && local_2c[0] == '\0';
  }
  if (*(long *)(lVar1 + 0x28) == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(bVar2);
}

/* ===== FUN_0016dc20 @ 0016dc20 ===== */

void FUN_0016dc20(ulong param_1,ulong param_2,int param_3)

{
  long lVar1;
  bool bVar2;
  bool bVar3;
  int iVar4;
  ulong uVar5;
  short local_54 [2];
  ulong local_50;
  long local_48;
  
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  local_54[0] = 0;
  uVar5 = param_1;
  if (param_1 == 0) goto LAB_0016dd9c;
  local_50 = 0;
  if (param_1 + 8 >> 3 < 0x201) {
    bVar3 = false;
  }
  else {
    iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1,&local_50,8);
    bVar3 = iVar4 == 1;
  }
  bVar2 = false;
  if (0xfff < local_50) {
    bVar2 = bVar3;
  }
  uVar5 = local_50;
  if (!(bool)(bVar2 & (local_50 & 7) == 0)) {
    uVar5 = 0;
  }
  if (uVar5 == DAT_001a7cd0 + 0x11ad208U) {
    local_50 = 0;
    if ((((0xfff < param_1 + 0x30) && ((param_1 & 0xfffffffffffffff8) != 0xffffffffffffffc8)) &&
        (iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1 + 0x30,&local_50,8), iVar4 == 1)) &&
       ((local_50 == 0 || (local_50 == DAT_001a7d28)))) {
      uVar5 = 0;
      if ((param_1 + 0xbe < 0x1000) || ((param_1 & 0xfffffffffffffffe) == 0xffffffffffffff40))
      goto LAB_0016dd9c;
      iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1 + 0xbe,local_54,2);
      if (iVar4 == 1) {
        uVar5 = 0;
        if ((-1 < param_3) &&
           (local_54[0] == *(short *)(&DAT_0013ce3c + (param_2 & 0xffffffff) * 2))) {
          uVar5 = (ulong)(param_3 < local_54[0]);
        }
        goto LAB_0016dd9c;
      }
    }
  }
  uVar5 = 0;
LAB_0016dd9c:
  if (*(long *)(lVar1 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar5);
}

/* ===== FUN_0016ddc8 @ 0016ddc8 ===== */

void FUN_0016ddc8(ulong param_1)

{
  long lVar1;
  bool bVar2;
  int iVar3;
  int local_44;
  long local_40;
  long local_38;
  
  lVar1 = tpidr_el0;
  bVar2 = false;
  local_38 = *(long *)(lVar1 + 0x28);
  local_40 = 1;
  local_44 = 0;
  if ((0xfff < param_1 + 0x38) && ((param_1 & 0xfffffffffffffff8) != 0xffffffffffffffc0)) {
    iVar3 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1 + 0x38,&local_40,8);
    bVar2 = false;
    if ((iVar3 == 1) && (local_40 == 0)) {
      bVar2 = false;
      if ((0xfff < param_1 + 0x40) && ((param_1 & 0xfffffffffffffffc) != 0xffffffffffffffbc)) {
        iVar3 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1 + 0x40,&local_44,4);
        if (iVar3 == 1) {
          bVar2 = local_44 == -1;
        }
        else {
          bVar2 = false;
        }
      }
    }
  }
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(bVar2);
}

/* ===== FUN_0016debc @ 0016debc ===== */

void FUN_0016debc(long param_1,long param_2)

{
  ulong uVar1;
  long lVar2;
  bool bVar3;
  bool bVar4;
  int iVar5;
  ulong local_30;
  long local_28;
  
  lVar2 = tpidr_el0;
  local_28 = *(long *)(lVar2 + 0x28);
  local_30 = 0;
  if (param_1 + 8U >> 3 < 0x201) {
    bVar4 = false;
  }
  else {
    iVar5 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1,&local_30,8);
    bVar4 = iVar5 == 1;
  }
  bVar3 = false;
  if (0xfff < local_30) {
    bVar3 = bVar4;
  }
  uVar1 = local_30;
  if (!(bool)(bVar3 & (local_30 & 7) == 0)) {
    uVar1 = 0;
  }
  if (*(long *)(lVar2 + 0x28) == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar1 == DAT_001a7cd0 + param_2);
}

/* ===== FUN_0016df78 @ 0016df78 ===== */

void FUN_0016df78(ulong param_1)

{
  long lVar1;
  bool bVar2;
  int iVar3;
  long local_30;
  long local_28;
  
  lVar1 = tpidr_el0;
  bVar2 = false;
  local_28 = *(long *)(lVar1 + 0x28);
  local_30 = 0;
  if ((0xfff < param_1 + 0x30) && ((param_1 & 0xfffffffffffffff8) != 0xffffffffffffffc8)) {
    iVar3 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1 + 0x30,&local_30,8);
    if (iVar3 == 1) {
      bVar2 = local_30 == 0 || local_30 == DAT_001a7d28;
    }
    else {
      bVar2 = false;
    }
  }
  if (*(long *)(lVar1 + 0x28) == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(bVar2);
}

/* ===== FUN_0016e020 @ 0016e020 ===== */

void FUN_0016e020(ulong param_1,ulong param_2)

{
  ulong uVar1;
  uint uVar2;
  long lVar3;
  bool bVar4;
  bool bVar5;
  int iVar6;
  int iVar7;
  undefined8 uVar8;
  ulong uVar9;
  short sVar10;
  ulong uVar11;
  ulong uVar12;
  long lVar13;
  ulong uVar14;
  short local_d4 [2];
  ulong local_d0;
  ulong local_c8 [12];
  long local_68;
  
  lVar3 = tpidr_el0;
  local_68 = *(long *)(lVar3 + 0x28);
  if (param_2 != 0) {
    uVar14 = 0;
    while (uVar12 = uVar14, local_c8[uVar12] = param_2, param_2 != param_1) {
      local_d0 = 0;
      if (param_2 + 0x40 >> 3 < 0x201) {
        bVar5 = false;
      }
      else {
        iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,param_2 + 0x38,&local_d0,8);
        bVar5 = iVar6 == 1;
      }
      bVar4 = false;
      if (0xfff < local_d0) {
        bVar4 = bVar5;
      }
      if ((!(bool)(bVar4 & (local_d0 & 7) == 0)) ||
         (param_2 = local_d0, uVar14 = uVar12 + 1, 10 < uVar12)) break;
    }
    if ((1 < (int)uVar12 + 1U) && (local_c8[uVar12] == param_1)) {
      uVar14 = uVar12 & 0xffffffff;
      do {
        uVar2 = (int)uVar14 - 1;
        uVar14 = (ulong)uVar2;
        local_d4[0] = 0;
        local_d0 = 0;
        uVar11 = local_c8[uVar14];
        if (param_1 + 8 >> 3 < 0x201) {
          bVar5 = false;
        }
        else {
          iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1,&local_d0,8);
          bVar5 = iVar6 == 1;
        }
        bVar4 = false;
        if (0xfff < local_d0) {
          bVar4 = bVar5;
        }
        uVar9 = local_d0;
        if (!(bool)(bVar4 & (local_d0 & 7) == 0)) {
          uVar9 = 0;
        }
        if ((((uVar9 != DAT_001a7cd0 + 0x11ad208U) ||
             (iVar6 = FUN_0016d5cc(uVar11,param_1), iVar6 == 0)) || (param_1 + 0xc0 < 0x1000)) ||
           ((((param_1 & 0xfffffffffffffffe) == 0xffffffffffffff3e ||
             (iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1 + 0xc0,local_d4,2), iVar6 != 1)) ||
            ((local_d4[0] < 1 || ((0x200 < local_d4[0] || (local_d0 = 0, param_1 < 0xf70))))))))
        goto LAB_0016e2d8;
        iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1 + 0x90,&local_d0,8);
        bVar5 = (local_d0 & 7) != 0;
        uVar9 = local_d0;
        if ((iVar6 != 1 || local_d0 < 0x1000) || bVar5) {
          uVar9 = 0;
        }
        if (((iVar6 != 1 || local_d0 < 0x1000) || bVar5) || (local_d4[0] < 1)) goto LAB_0016e2d8;
        lVar13 = 0;
        iVar6 = 0;
        sVar10 = local_d4[0];
        do {
          local_d0 = 0;
          if (uVar9 + 8 >> 3 < 0x201) {
            bVar5 = false;
          }
          else {
            iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar9,&local_d0,8);
            bVar5 = iVar7 == 1;
            sVar10 = local_d4[0];
          }
          lVar13 = lVar13 + 1;
          bVar4 = false;
          if (0xfff < local_d0) {
            bVar4 = bVar5;
          }
          uVar1 = local_d0;
          if (!(bool)(bVar4 & (local_d0 & 7) == 0)) {
            uVar1 = 0;
          }
          if (uVar1 == uVar11) {
            iVar6 = iVar6 + 1;
          }
          uVar9 = uVar9 + 8;
        } while (lVar13 < sVar10);
        if (iVar6 != 1) goto LAB_0016e2d8;
        param_1 = uVar11;
      } while (uVar2 != 0);
      do {
        uVar14 = local_c8[uVar12];
        local_d4[0] = 0;
        if ((0xfff < uVar14 + 0xc0) && ((uVar14 & 0xfffffffffffffffe) != 0xffffffffffffff3e)) {
          (*DAT_001a7ce8)(DAT_001a7cc8,uVar14 + 0xc0,local_d4,2);
        }
        local_d0 = 0;
        if (uVar14 + 0x98 >> 3 < 0x201) {
          bVar5 = false;
        }
        else {
          iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar14 + 0x90,&local_d0,8);
          bVar5 = iVar6 == 1;
        }
        bVar4 = false;
        if (0xfff < local_d0) {
          bVar4 = bVar5;
        }
        uVar11 = local_d0;
        if (!(bool)(bVar4 & (local_d0 & 7) == 0)) {
          uVar11 = 0;
        }
        if (0 < local_d4[0]) {
          lVar13 = 0;
          do {
            local_d0 = 0;
            if (0x200 < uVar11 + 8 >> 3) {
              iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar11,&local_d0,8);
              bVar5 = (local_d0 & 7) != 0;
              uVar9 = local_d0;
              if ((iVar6 != 1 || local_d0 < 0x1000) || bVar5) {
                uVar9 = 0;
              }
              if ((((iVar6 == 1 && 0xfff < local_d0) && !bVar5) &&
                  (uVar9 != local_c8[(int)uVar12 - 1])) &&
                 (iVar6 = FUN_0016d5cc(uVar9,uVar14), iVar6 != 0)) {
                FUN_0016cf5c(0xc61c3c00,0xc61c3c00,uVar9);
              }
            }
            lVar13 = lVar13 + 1;
            uVar11 = uVar11 + 8;
          } while (lVar13 < local_d4[0]);
        }
        uVar12 = uVar12 - 1;
        uVar8 = 1;
      } while ((int)uVar12 != 0);
      goto LAB_0016e2dc;
    }
  }
LAB_0016e2d8:
  uVar8 = 0;
LAB_0016e2dc:
  if (*(long *)(lVar3 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar8);
}

/* ===== FUN_0016e464 @ 0016e464 ===== */

void FUN_0016e464(long param_1,char param_2)

{
  bool bVar1;
  long lVar2;
  bool bVar3;
  int iVar4;
  char local_2c [4];
  long local_28;
  
  lVar2 = tpidr_el0;
  local_28 = *(long *)(lVar2 + 0x28);
  local_2c[0] = -1;
  if (param_1 + 0x49U < 0x1001) {
    bVar3 = false;
  }
  else {
    iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1 + 0x48,local_2c,1);
    bVar3 = iVar4 == 1;
  }
  bVar1 = false;
  if (local_2c[0] == param_2) {
    bVar1 = bVar3;
  }
  if (*(long *)(lVar2 + 0x28) == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(bVar1);
}

/* ===== FUN_0016e7c0 @ 0016e7c0 ===== */

void FUN_0016e7c0(void)

{
  long lVar1;
  bool bVar2;
  bool bVar3;
  int iVar4;
  ulong uVar5;
  ulong local_48;
  int local_40;
  int local_3c;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  local_48 = 0;
  if (DAT_00282168 + 8U >> 3 < 0x201) {
    bVar3 = false;
  }
  else {
    iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_00282168,&local_48,8);
    bVar3 = iVar4 == 1;
  }
  bVar2 = false;
  if (0xfff < local_48) {
    bVar2 = bVar3;
  }
  uVar5 = local_48;
  if (!(bool)(bVar2 & (local_48 & 7) == 0)) {
    uVar5 = 0;
  }
  if (uVar5 == DAT_001a7cd0 + 0x11c0c58U) {
    uVar5 = FUN_00179dfc(DAT_00282168);
    if ((int)uVar5 == 0) goto LAB_0016e888;
    if (0x200 < DAT_00282168 + 0xf4U >> 3) {
      iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_00282168 + 0xec,&local_40,8);
      uVar5 = 0;
      if (((iVar4 == 1) && (local_40 == 1)) && (local_3c == 0x91)) {
        local_48 = 0;
        if (DAT_00282168 + 0x130U >> 3 < 0x201) {
          bVar3 = false;
        }
        else {
          iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_00282168 + 0x128,&local_48,8);
          bVar3 = iVar4 == 1;
        }
        bVar2 = false;
        if (0xfff < local_48) {
          bVar2 = bVar3;
        }
        uVar5 = local_48;
        if (!(bool)(bVar2 & (local_48 & 7) == 0)) {
          uVar5 = 0;
        }
        iVar4 = FUN_00179dfc(uVar5);
        uVar5 = (ulong)(iVar4 != 0);
      }
      goto LAB_0016e888;
    }
  }
  uVar5 = 0;
LAB_0016e888:
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar5);
}

/* ===== FUN_0016f79c @ 0016f79c ===== */

void FUN_0016f79c(long param_1)

{
  ulong uVar1;
  long lVar2;
  bool bVar3;
  bool bVar4;
  int iVar5;
  ulong local_30;
  long local_28;
  
  lVar2 = tpidr_el0;
  local_28 = *(long *)(lVar2 + 0x28);
  local_30 = 0;
  if (param_1 + 8U >> 3 < 0x201) {
    bVar4 = false;
  }
  else {
    iVar5 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1,&local_30,8);
    bVar4 = iVar5 == 1;
  }
  bVar3 = false;
  if (0xfff < local_30) {
    bVar3 = bVar4;
  }
  uVar1 = local_30;
  if (!(bool)(bVar3 & (local_30 & 7) == 0)) {
    uVar1 = 0;
  }
  if (*(long *)(lVar2 + 0x28) == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar1);
}

/* ===== FUN_0016fb10 @ 0016fb10 ===== */

void FUN_0016fb10(void)

{
  undefined1 *puVar1;
  long lVar2;
  int iVar3;
  int iVar4;
  float fVar5;
  undefined1 auStack_258 [12];
  float local_24c;
  undefined1 auStack_248 [512];
  long local_48;
  
  lVar2 = tpidr_el0;
  local_48 = *(long *)(lVar2 + 0x28);
  iVar3 = FUN_001922ac(auStack_248,0x200);
  DAT_0022d818 = 1.0;
  if ((((0x100 < DAT_001a7d28 + 0x4cU >> 4) &&
       (iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_001a7d28 + 0x3c,auStack_258,0x10), iVar4 == 1)) &&
      (ABS(local_24c) != INFINITY)) &&
     ((!NAN(ABS(local_24c)) && (local_24c != 720.0 && local_24c < 720.0 == NAN(local_24c))))) {
    DAT_0022d818 = (float)NEON_fminnm(local_24c / 720.0,0x3fc00000);
  }
  fVar5 = DAT_0022d818 * 8.0;
  puVar1 = (undefined1 *)0x0;
  if (iVar3 != 0) {
    puVar1 = auStack_248;
  }
  FUN_00179f68(DAT_0022d818 * 20.0,fVar5,DAT_0022d818 * 16.0,&DAT_00282b10,puVar1);
  DAT_00283330 = fVar5 + DAT_0028332c;
  if ((DAT_0028332c == 0.0 || DAT_0028332c < 0.0 != NAN(DAT_0028332c)) || iVar3 != 2) {
    DAT_00283330 = 0.0;
  }
  if (*(long *)(lVar2 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00172da8 @ 00172da8 ===== */

void FUN_00172da8(void)

{
  ulong uVar1;
  ulong uVar2;
  ulong uVar3;
  ulong uVar4;
  long lVar5;
  int iVar6;
  long lVar7;
  ulong __n;
  bool bVar8;
  ulong uVar9;
  long *plVar10;
  ulong uVar11;
  ulong uVar12;
  long local_90;
  undefined1 auStack_88 [32];
  long local_68;
  
  lVar5 = tpidr_el0;
  local_68 = *(long *)(lVar5 + 0x28);
  bVar8 = false;
  uVar9 = 0;
  do {
    uVar11 = 0;
    uVar2 = (&DAT_0019c100)[uVar9 * 3];
    uVar3 = (&DAT_0019c108)[uVar9 * 3];
    uVar12 = ~uVar2;
    __n = uVar3;
    do {
      uVar4 = __n - 0x20;
      if (0x1f < __n) {
        __n = 0x20;
      }
      uVar1 = uVar2 + uVar11 + DAT_001a7cd0;
      if ((((uVar1 < 0x1000 || uVar12 - DAT_001a7cd0 < __n) ||
           (iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar1,auStack_88,__n), iVar6 != 1)) ||
          ((0x88836c - uVar2 == uVar11 &&
           (iVar6 = FUN_0014ff7c(DAT_001a7cd0,0x88836c,auStack_88,__n), iVar6 == 0)))) ||
         (iVar6 = memcmp(auStack_88,(&PTR_DAT_0019c110)[uVar9 * 3] + uVar11,__n), iVar6 != 0)) {
        if (bVar8) goto LAB_00172ef4;
        bVar8 = false;
        goto LAB_00172f28;
      }
      uVar11 = uVar11 + 0x20;
      uVar12 = uVar12 - 0x20;
      __n = uVar4;
    } while (uVar11 < uVar3);
    uVar11 = uVar9 + 1;
    bVar8 = 0x37 < uVar9;
    uVar9 = uVar11;
  } while (uVar11 != 0x39);
LAB_00172ef4:
  bVar8 = false;
  local_90 = 0;
  lVar7 = DAT_001a7cd0 + 0x11c0a78;
  if (0x200 < DAT_001a7cd0 + 0x11c0a80U >> 3) {
    uVar9 = 0;
    plVar10 = &DAT_0013cea8;
    do {
      local_90 = 0;
      iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,lVar7,&local_90,8);
      if (((iVar6 != 1) || (local_90 != plVar10[-1] + DAT_001a7cd0)) ||
         (bVar8 = 0x23 < uVar9, uVar9 == 0x24)) break;
      uVar9 = uVar9 + 1;
      local_90 = 0;
      lVar7 = *plVar10 + DAT_001a7cd0;
      plVar10 = plVar10 + 2;
    } while (0x200 < lVar7 + 8U >> 3);
  }
LAB_00172f28:
  if (*(long *)(lVar5 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(bVar8);
}

/* ===== FUN_001753b0 @ 001753b0 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_001753b0(long param_1)

{
  long lVar1;
  int iVar2;
  undefined8 uVar3;
  uint uVar4;
  float fVar5;
  undefined8 local_e8;
  undefined8 uStack_e0;
  code *local_d8;
  code *pcStack_d0;
  code *local_c8;
  code *pcStack_c0;
  code *local_b8;
  code *pcStack_b0;
  code *local_a8;
  code *pcStack_a0;
  code *local_98;
  code *pcStack_90;
  code *local_88;
  code *pcStack_80;
  code *local_78;
  code *pcStack_70;
  long local_68;
  undefined4 local_60;
  undefined4 uStack_5c;
  undefined4 uStack_58;
  undefined4 uStack_54;
  undefined4 uStack_50;
  undefined4 uStack_4c;
  long local_48;
  
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  uStack_4c = DAT_001a7d60._4_4_;
  uStack_54 = (undefined4)_DAT_001a7d5c;
  uStack_50 = (undefined4)((ulong)_DAT_001a7d5c >> 0x20);
  uStack_5c = (undefined4)_DAT_001e5400;
  uStack_58 = (undefined4)((ulong)_DAT_001e5400 >> 0x20);
  local_60 = DAT_001e53fc;
  if ((DAT_001a7d28 + 0x28cU >> 3 < 0x201) ||
     (iVar2 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_001a7d28 + 0x284,&local_68,8), iVar2 != 1)) {
    uVar3 = 0;
  }
  else {
    if (((DAT_0022d810 == 0) ||
        ((DAT_0022d7f0 != CONCAT44(uStack_5c,local_60) ||
         DAT_0022d7f8 != CONCAT44(uStack_54,uStack_58)) ||
         DAT_0022d800 != CONCAT44(uStack_4c,uStack_50))) || (DAT_0022d808 != local_68)) {
      DAT_0022d7f8 = CONCAT44(uStack_54,uStack_58);
      DAT_0022d7f0 = CONCAT44(uStack_5c,local_60);
      DAT_0022d800 = CONCAT44(uStack_4c,uStack_50);
      DAT_0022d794 = 0;
      DAT_0022d7a8 = DAT_0022d7a8 + 1;
      DAT_0022d7b0 = DAT_0022d7b0 + 1;
      DAT_0022d808 = local_68;
      DAT_0022d810 = 1;
    }
    if (DAT_0022d7a0 != *(byte *)(param_1 + 8)) {
      DAT_0022d558 = 0;
      DAT_0022d7a8 = DAT_0022d7a8 + 1;
      DAT_0022d7b0 = DAT_0022d7b0 + 1;
      ram0x0022d520 = 0;
      _DAT_0022d518 = 0;
      _DAT_0022d530 = 0;
      _DAT_0022d528 = 0;
      uRam000000000022d540 = 0;
      _DAT_0022d538 = 0;
      uRam000000000022d550 = 0;
      _DAT_0022d548 = 0;
      DAT_0022d79c = 0xffffffff;
      DAT_0022d7a0 = (uint)*(byte *)(param_1 + 8);
    }
    if (((*(char *)(param_1 + 9) == '\0') || (*(char *)(param_1 + 10) != '\0')) ||
       (uVar4 = *(byte *)(param_1 + 8) - 0x43, 0x31 < uVar4)) {
      uVar4 = 0;
    }
    else {
      uVar4 = (uint)(0x22006000cd001 >> ((ulong)uVar4 & 0x3f)) & 1;
    }
    if (DAT_0022d798 != uVar4) {
      DAT_0022d7a8 = DAT_0022d7a8 + 1;
      DAT_0022d7b0 = DAT_0022d7b0 + 1;
      DAT_0022d798 = uVar4;
    }
    if ((uVar4 != 0) && (DAT_0022d79c != *(byte *)(param_1 + 0xb))) {
      DAT_0022d7b0 = DAT_0022d7b0 + 1;
      DAT_0022d7d0 = DAT_0022d7d0 + 1;
      DAT_0022d79c = (uint)*(byte *)(param_1 + 0xb);
    }
    uVar3 = FUN_00179448();
    if ((int)uVar3 != 0) {
      fVar5 = -132.0 - DAT_0022d518;
      if (uVar4 == 0) {
        fVar5 = -132.0;
      }
      fVar5 = -132.0 - (float)(int)fVar5;
      if (fVar5 == DAT_0022d7e8) {
        uVar3 = 1;
      }
      else {
        local_e8 = DAT_001a7cd0;
        uStack_e0 = 0;
        local_88 = FUN_00180724;
        pcStack_80 = FUN_0018073c;
        local_d8 = FUN_0017fe9c;
        pcStack_d0 = FUN_0017fee0;
        local_c8 = FUN_00180034;
        pcStack_c0 = FUN_001802bc;
        local_b8 = FUN_00180540;
        pcStack_b0 = FUN_00180548;
        local_a8 = FUN_00180564;
        pcStack_a0 = FUN_00180580;
        local_98 = FUN_0018058c;
        pcStack_90 = FUN_001805a4;
        iVar2 = 0;
        if (0x2a < DAT_001a7d18) {
          iVar2 = DAT_001a7d18 - 0x2b;
        }
        local_78 = FUN_00180758;
        pcStack_70 = FUN_0017fbc8;
        iVar2 = FUN_0017fc3c(0x44048000,-132.0 - fVar5,&local_e8,DAT_001a7d28,iVar2,1);
        uVar3 = 0;
        if (iVar2 == 1) {
          uVar3 = 1;
          DAT_0022d7c8 = DAT_0022d7c8 + 1;
          DAT_0022d7e8 = fVar5;
        }
      }
    }
  }
  if (*(long *)(lVar1 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar3);
}

/* ===== FUN_00176a24 @ 00176a24 ===== */

void FUN_00176a24(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
                 undefined8 param_5,undefined8 param_6,undefined8 param_7,undefined8 param_8,
                 undefined8 param_9)

{
  long lVar1;
  long lVar2;
  undefined8 local_90;
  undefined8 uStack_88;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined1 *local_70;
  undefined1 **ppuStack_68;
  undefined8 *puStack_60;
  undefined8 uStack_58;
  
  ppuStack_68 = &local_70;
  lVar1 = tpidr_el0;
  lVar2 = *(long *)(lVar1 + 0x28);
  puStack_60 = &local_90;
  uStack_58 = 0xffffff80ffffffe0;
  local_90 = param_6;
  uStack_88 = param_7;
  local_80 = param_8;
  uStack_78 = param_9;
  local_70 = (undefined1 *)register0x00000008;
  __vsnprintf_chk(param_2,param_4,0,param_3,param_5,&local_70,param_8,param_9,param_1);
  if (*(long *)(lVar1 + 0x28) == lVar2) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00176ad4 @ 00176ad4 ===== */

void FUN_00176ad4(long param_1,long *param_2,uint param_3)

{
  long lVar1;
  undefined1 auVar2 [16];
  bool bVar3;
  bool bVar4;
  int iVar5;
  ulong uVar6;
  long lVar7;
  long *plVar8;
  undefined8 *puVar9;
  ulong uVar10;
  long lVar11;
  long lVar12;
  ushort uVar13;
  undefined1 auVar14 [16];
  undefined1 auVar15 [16];
  undefined1 auVar16 [16];
  int local_cd8 [20];
  ulong local_c88;
  undefined8 local_c80 [384];
  uint local_80;
  long local_70;
  
  lVar1 = tpidr_el0;
  local_70 = *(long *)(lVar1 + 0x28);
  uVar6 = 0;
  if (((((param_2 == (long *)0x0) || (1 < param_3)) || (uVar6 = 0, DAT_001a7d30 != param_1)) ||
      ((DAT_001a7d0c == 0 || (uVar6 = (*DAT_001a7cf0)(DAT_001a7cc8), (int)uVar6 == 0)))) ||
     (uVar6 = FUN_00176d98(local_c80,0), (int)uVar6 == 0)) goto LAB_00176d04;
  lVar12 = 0;
  do {
    lVar7 = param_2[lVar12];
    if (lVar7 != 0) {
      if (lVar7 != param_2[1 - lVar12]) {
        auVar16._8_8_ = DAT_001a7d30;
        auVar16._0_8_ = DAT_001a7d28;
        auVar14._8_8_ = DAT_00281a50;
        auVar14._0_8_ = DAT_001a7d38;
        auVar15._8_8_ = lVar7;
        auVar15._0_8_ = lVar7;
        auVar16 = NEON_cmeq(auVar16,auVar15,8);
        auVar2._8_8_ = lVar7;
        auVar2._0_8_ = lVar7;
        auVar15 = NEON_cmeq(auVar14,auVar2,8);
        uVar13 = NEON_umaxv(CONCAT26(auVar15._8_2_,
                                     CONCAT24(auVar15._0_2_,CONCAT22(auVar16._8_2_,auVar16._0_2_))),
                            2);
        if ((((uVar13 & 1) == 0) && (DAT_0022d4e0 != lVar7)) &&
           ((DAT_0022d4e8 != lVar7 && (DAT_0022d4f0 != lVar7)))) {
          if (DAT_001a7d18 == 0) {
LAB_00176c48:
            local_c88 = 0;
            if (lVar7 + 8U >> 3 < 0x201) {
              bVar4 = false;
            }
            else {
              iVar5 = (*DAT_001a7ce8)(DAT_001a7cc8,lVar7,&local_c88,8);
              bVar4 = iVar5 == 1;
            }
            bVar3 = false;
            if (0xfff < local_c88) {
              bVar3 = bVar4;
            }
            uVar6 = local_c88;
            if (!(bool)(bVar3 & (local_c88 & 7) == 0)) {
              uVar6 = 0;
            }
            if ((uVar6 == DAT_001a7cd0 + 0x11c0a48U) &&
               (iVar5 = FUN_00177620(param_2[lVar12],param_1,local_cd8), iVar5 != 0)) {
              uVar6 = (ulong)local_80;
              if (local_80 != 0) {
                puVar9 = local_c80;
                do {
                  if (param_2[lVar12] == *(long *)*puVar9) goto LAB_00176d00;
                  puVar9 = puVar9 + 4;
                  uVar6 = uVar6 - 1;
                } while (uVar6 != 0);
              }
              goto LAB_00176b88;
            }
          }
          else if (DAT_001c3568 != lVar7) {
            plVar8 = &DAT_001c3640;
            uVar6 = 1;
            do {
              uVar10 = uVar6;
              if (DAT_001a7d18 == uVar10) break;
              lVar11 = *plVar8;
              plVar8 = plVar8 + 0x1b;
              uVar6 = uVar10 + 1;
            } while (lVar11 != lVar7);
            if (DAT_001a7d18 <= uVar10) goto LAB_00176c48;
          }
        }
      }
LAB_00176d00:
      uVar6 = 0;
      goto LAB_00176d04;
    }
LAB_00176b88:
    lVar12 = lVar12 + 1;
  } while (lVar12 != 2);
  if (param_3 == 0) {
LAB_00176d7c:
    uVar6 = 1;
  }
  else {
    local_cd8[0] = 0;
    if (*param_2 == 0) {
LAB_00176d58:
      if (param_2[1] == 0) goto LAB_00176d7c;
      iVar5 = FUN_00177834(param_2[1],1,local_cd8);
      if (iVar5 != 0) {
        uVar6 = 1;
        param_2[1] = 0;
        goto LAB_00176d04;
      }
    }
    else {
      iVar5 = FUN_00177834(*param_2,1,local_cd8);
      if (iVar5 != 0) {
        *param_2 = 0;
        goto LAB_00176d58;
      }
    }
    uVar6 = (ulong)-(uint)(local_cd8[0] != 0);
  }
LAB_00176d04:
  if (*(long *)(lVar1 + 0x28) != local_70) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar6);
  }
  return;
}

/* ===== FUN_00176d98 @ 00176d98 ===== */

ulong FUN_00176d98(void *param_1,undefined8 param_2)

{
  long lVar1;
  int iVar2;
  ulong uVar3;
  long lVar4;
  
  memset(param_1,0,0xc10);
  *(undefined8 *)((long)param_1 + 0xc08) = param_2;
  uVar3 = thunk_FUN_0014d168(0x8897e8,0x134,
                             "eaaf4013a20aee66e9a61ff61032f00d36d4843de800a1fb3f8865d9e3c32edb");
  if ((int)uVar3 == 0) {
    return uVar3;
  }
  uVar3 = thunk_FUN_0014d168(0x88991c,0x24,
                             "012d8d1313927592cb3f04e0c9f413139c8eab45f712823ba6788d3cb650fa3e");
  if ((int)uVar3 == 0) {
    return uVar3;
  }
  uVar3 = thunk_FUN_0014d168(0x5940d0,0x60,
                             "13a9d8594868ac625edc10fae30f2e9a166b7563a470769d50287b70be1501c7");
  if ((int)uVar3 == 0) {
    return uVar3;
  }
  if (DAT_00283340 == 0) {
    if (DAT_00283980 != 0) {
      return 0;
    }
  }
  else {
    uVar3 = FUN_00181264(DAT_00283338,DAT_00283340,DAT_00283980,0x12);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_002833a0,DAT_00283340,1,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00283418,DAT_00283340,1,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00283490,DAT_00283340,1,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00283508,DAT_00283340,1,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00283580,DAT_00283340,1,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_002835f8,DAT_00283340,1,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00283350,DAT_00283340,2,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00283358,DAT_00283340,2,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00283360,DAT_00283340,2,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00283368,DAT_00283340,2,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00283370,DAT_00283340,2,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00283678,DAT_00283340,0,DAT_00283680);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_002836f0,DAT_00283340,0,DAT_002836f8);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00283768,DAT_00283340,0,DAT_00283770);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_002837e0,DAT_00283340,0,DAT_002837e8);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00283858,DAT_00283340,0,DAT_00283860);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_002838d0,DAT_00283340,0,DAT_002838d8);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00283348,DAT_00283340,0,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00283340,DAT_001a7d30,3,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00283378,0,4,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00283380,0,4,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00283388,0,4,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00283390,0,4,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00283398,0,4,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
  }
  if (DAT_00282b10 == 0) {
    if (DAT_00282b18 != 0) {
      return 0;
    }
  }
  else {
    if (DAT_00282b20 != DAT_001a7d30) {
      return 0;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00282b10,DAT_00282b20,0,DAT_00282b18);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
  }
  if (DAT_00283a00 == 0) {
    if (DAT_00283a08 != 0) {
      return 0;
    }
  }
  else {
    if (DAT_00283a10 != DAT_001a7d30) {
      return 0;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00283a00,DAT_00283a10,0,DAT_00283a08);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
  }
  if (DAT_00284220 != 0) {
    if (DAT_002843d0 != DAT_001a7d30) {
      return 0;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00284220,DAT_002843d0,1,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
  }
  if (DAT_002842f8 != 0) {
    if (DAT_002843d0 != DAT_001a7d30) {
      return 0;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_002842f8,DAT_002843d0,1,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
  }
  if (DAT_002843e0 == 0) {
    if (DAT_002844c8 != 0) {
      return 0;
    }
  }
  else {
    if (DAT_002844c8 == 0) {
      return 0;
    }
    if (DAT_002844b8 != DAT_001a7d30) {
      return 0;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_002843e0,DAT_002844b8,1,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
  }
  if (DAT_00282158 == 0) {
    if (DAT_00282ae0 != 0) {
      return 0;
    }
  }
  else {
    uVar3 = FUN_00181264(DAT_00282150,DAT_00282158,DAT_00282ae0,9);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00282178,DAT_00282158,1,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_002822d0,DAT_00282158,1,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00282428,DAT_00282158,1,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00282580,DAT_00282158,1,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00282168,DAT_00282158,2,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_002826e0,DAT_00282158,0,DAT_002826e8);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00282838,DAT_00282158,0,DAT_00282840);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00282990,DAT_00282158,0,DAT_00282998);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00282160,DAT_00282158,0,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00282158,DAT_001a7d30,3,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00282170,0,4,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
  }
  lVar1 = DAT_002844f0;
  if (DAT_002844f8 != 0) {
    if (DAT_002844e8 != DAT_001a7d30) {
      return 0;
    }
    lVar4 = FUN_0017ec28();
    if (lVar1 != lVar4) {
      return 0;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_002844f8,DAT_002844f0,0,DAT_00284500);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
  }
  if (DAT_00281b10 == 0) {
    if (DAT_00282110 != 0) {
      return 0;
    }
  }
  else {
    uVar3 = FUN_00181264(DAT_00281b08,DAT_00281b10,DAT_00282110,0xb);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00281b20,DAT_00281b10,1,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00281bb8,DAT_00281b10,1,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00281c50,DAT_00281b10,1,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00281ce8,DAT_00281b10,1,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00281d80,DAT_00281b10,1,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00281e18,DAT_00281b10,1,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00281eb0,DAT_00281b10,1,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00281f48,DAT_00281b10,1,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00281fe8,DAT_00281b10,0,DAT_00281ff0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00282080,DAT_00281b10,0,DAT_00282088);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00281b18,DAT_00281b10,0,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
    uVar3 = FUN_001813b8(param_1,&DAT_00281b10,DAT_001a7d30,3,0);
    if ((int)uVar3 == 0) {
      return uVar3;
    }
  }
  iVar2 = FUN_00181754();
  return (ulong)(iVar2 != 0);
}

/* ===== FUN_00177620 @ 00177620 ===== */

void FUN_00177620(ulong param_1,undefined8 param_2,undefined8 param_3)

{
  long lVar1;
  bool bVar2;
  bool bVar3;
  int iVar4;
  ulong uVar5;
  ulong local_128;
  ulong uStack_120;
  ulong local_118;
  undefined8 local_110;
  ulong uStack_108;
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
  uStack_108 = 0;
  local_110 = 0;
  local_128 = 0;
  uStack_120 = param_1;
  if (param_1 + 0xb8 >> 3 < 0x201) {
    bVar3 = false;
    local_118 = 0;
  }
  else {
    iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1 + 0xb0,&local_128,8);
    bVar3 = iVar4 == 1;
    local_118 = local_128;
  }
  local_128 = 0;
  bVar2 = false;
  if (0xfff < local_118) {
    bVar2 = bVar3;
  }
  if (!(bool)(bVar2 & (local_118 & 7) == 0)) {
    local_118 = 0;
  }
  if (param_1 + 0x90 >> 3 < 0x201) {
    bVar3 = false;
  }
  else {
    iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1 + 0x88,&local_128,8);
    bVar3 = iVar4 == 1;
  }
  bVar2 = false;
  if (0xfff < local_128) {
    bVar2 = bVar3;
  }
  uStack_108 = local_128;
  if (!(bool)(bVar2 & (local_128 & 7) == 0)) {
    uStack_108 = 0;
  }
  uVar5 = FUN_0016d5cc(param_1,param_2);
  if ((int)uVar5 != 0) {
    local_128 = 0;
    if (((((param_1 + 0x30 < 0x1000) || ((param_1 & 0xfffffffffffffff8) == 0xffffffffffffffc8)) ||
         (iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1 + 0x30,&local_128,8), iVar4 != 1)) ||
        ((local_128 != 0 && (local_128 != DAT_001a7d28)))) ||
       (local_128 = CONCAT71(local_128._1_7_,0xff), param_1 + 0x49 < 0x1001)) {
      uVar5 = 0;
    }
    else {
      iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1 + 0x48,&local_128,1);
      uVar5 = 0;
      if ((iVar4 == 1) && ((char)local_128 == '\x01')) {
        iVar4 = FUN_0017c704(&uStack_120,1,param_3);
        uVar5 = (ulong)(iVar4 != 0);
      }
    }
  }
  if (*(long *)(lVar1 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar5);
}

/* ===== FUN_00177834 @ 00177834 ===== */

void FUN_00177834(ulong param_1,int param_2,undefined4 *param_3)

{
  uint uVar1;
  long lVar2;
  bool bVar3;
  bool bVar4;
  int iVar5;
  undefined8 uVar6;
  ulong local_188;
  ulong local_180;
  ulong local_170;
  int local_13c;
  int local_134;
  ulong local_130;
  ulong local_128;
  undefined8 local_120;
  ulong uStack_118;
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
  undefined8 local_60;
  long local_58;
  
  lVar2 = tpidr_el0;
  local_58 = *(long *)(lVar2 + 0x28);
  if (param_2 != 0) {
    local_60 = 0;
    local_188 = 0;
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
    uStack_118 = 0;
    local_120 = 0;
    uStack_108 = 0;
    uStack_110 = 0;
    if (param_1 + 0xb8 >> 3 < 0x201) {
      bVar4 = false;
      local_128 = 0;
      local_130 = param_1;
    }
    else {
      local_130 = param_1;
      iVar5 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1 + 0xb0,&local_188,8);
      bVar4 = iVar5 == 1;
      local_128 = local_188;
    }
    local_188 = 0;
    bVar3 = false;
    if (0xfff < local_128) {
      bVar3 = bVar4;
    }
    if (!(bool)(bVar3 & (local_128 & 7) == 0)) {
      local_128 = 0;
    }
    if (param_1 + 0x90 >> 3 < 0x201) {
      bVar4 = false;
    }
    else {
      iVar5 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1 + 0x88,&local_188,8);
      bVar4 = iVar5 == 1;
    }
    bVar3 = false;
    if (0xfff < local_188) {
      bVar3 = bVar4;
    }
    uStack_118 = local_188;
    if (!(bool)(bVar3 & (local_188 & 7) == 0)) {
      uStack_118 = 0;
    }
    uVar6 = FUN_0017c704(&local_130,1,&local_188);
    if ((int)uVar6 == 0) goto LAB_00177a28;
  }
  *param_3 = 1;
  (*(code *)(DAT_001a7cd0 + 0x59567c))(param_1);
  local_130 = 1;
  local_134 = 0;
  if (((((param_1 + 0x38 < 0x1000) || ((param_1 & 0xfffffffffffffff8) == 0xffffffffffffffc0)) ||
       (iVar5 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1 + 0x38,&local_130,8), iVar5 != 1)) ||
      ((local_130 != 0 || (param_1 + 0x40 < 0x1000)))) ||
     (((param_1 & 0xfffffffffffffffc) == 0xffffffffffffffbc ||
      ((iVar5 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1 + 0x40,&local_134,4), iVar5 != 1 ||
       (local_134 != -1)))))) {
    uVar6 = 0;
  }
  else {
    if ((param_2 != 0) && (local_13c == 3)) {
      FUN_0016cfa4(local_170,local_180);
      local_128 = local_180;
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
      local_120 = 0;
      uStack_108 = 0;
      uStack_110 = 0;
      local_60 = 0;
      uStack_118 = local_170;
      local_130 = param_1;
      iVar5 = FUN_0017c704(&local_130,1,&local_188);
      uVar6 = 0;
      if ((iVar5 == 0) || (local_13c != 2)) goto LAB_00177a28;
    }
    uVar1 = 0x887e90;
    if (param_2 == 0) {
      uVar1 = 0x5d48e8;
    }
    (*(code *)(DAT_001a7cd0 + (ulong)uVar1))(param_1);
    uVar6 = 1;
  }
LAB_00177a28:
  if (*(long *)(lVar2 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar6);
}

/* ===== FUN_00178714 @ 00178714 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00178714(void)

{
  long lVar1;
  ulong uVar2;
  int iVar3;
  ulong uVar4;
  undefined8 local_c8;
  undefined8 local_c0;
  code *local_b8;
  code *pcStack_b0;
  code *local_a8;
  code *local_a0;
  code *local_98;
  code *pcStack_90;
  code *local_88;
  code *pcStack_80;
  code *local_78;
  code *local_70;
  code *local_68;
  code *pcStack_60;
  code *local_58;
  code *pcStack_50;
  int local_44;
  ulong local_40;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  if (DAT_0022d4e0 == 0) {
    uVar4 = (ulong)((DAT_0022d4e8 == 0 && DAT_0022d4f0 == 0) && _DAT_0022d514 == 0);
    goto LAB_001788b0;
  }
  local_b8 = FUN_0017fe9c;
  pcStack_b0 = FUN_0017fee0;
  local_a8 = FUN_00180034;
  local_a0 = FUN_001802bc;
  local_98 = FUN_00180540;
  pcStack_90 = FUN_00180548;
  local_88 = FUN_00180564;
  pcStack_80 = FUN_00180580;
  local_78 = FUN_0018058c;
  local_70 = FUN_001805a4;
  local_68 = FUN_00180724;
  pcStack_60 = FUN_0018073c;
  local_c8 = DAT_001a7cd0;
  local_c0 = 0;
  local_58 = FUN_00180758;
  pcStack_50 = FUN_0017fbc8;
  uVar4 = FUN_00180d80(&local_c8,&DAT_0022d4e0,DAT_001a7d28,0);
  if (((int)uVar4 == 0) || (uVar4 = (*local_a0)(local_c0), (int)uVar4 == 0)) goto LAB_001788b0;
  if (DAT_0022d4f8 == 0) {
LAB_00178844:
    (*local_78)(local_c0,DAT_0022d4f0);
    uVar2 = DAT_0022d4f0;
    local_44 = 0;
    uVar4 = DAT_0022d4f0 + 0x40;
    local_40 = 1;
    if ((((((0x200 < uVar4 >> 3) &&
           (iVar3 = (*local_b8)(local_c0,DAT_0022d4f0 + 0x38,&local_40,8),
           uVar2 < 0xffffffffffffffbc)) && (iVar3 == 1)) &&
         ((local_40 == 0 && (iVar3 = (*local_b8)(local_c0,uVar4,&local_44,4), iVar3 == 1)))) &&
        ((local_44 == -1 &&
         ((local_40 = local_40 & 0xffffffffffff0000, 0x800 < DAT_0022d4e8 + 0x50U >> 1 &&
          (iVar3 = (*local_b8)(local_c0,DAT_0022d4e8 + 0x4e,&local_40,2), iVar3 == 1)))))) &&
       ((short)local_40 == 0)) {
      (*local_70)(local_c0,DAT_0022d4f0);
      DAT_0022d4f0 = 0;
      (*local_70)(local_c0,DAT_0022d4e0);
      _DAT_0022d510 = 0;
      uVar4 = 1;
      DAT_0022d4f8 = 0;
      DAT_0022d4f0 = 0;
      DAT_0022d508 = 0;
      DAT_0022d500 = 0;
      DAT_0022d4e8 = 0;
      DAT_0022d4e0 = 0;
      goto LAB_001788b0;
    }
  }
  else {
    (*local_78)(local_c0,DAT_0022d4e0);
    DAT_0022d4f8 = 0;
    iVar3 = FUN_00180d80(&local_c8,&DAT_0022d4e0,0,0);
    if (iVar3 != 0) goto LAB_00178844;
  }
  uVar4 = 0;
  _DAT_0022d510 = DAT_0010f740;
LAB_001788b0:
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar4);
}

/* ===== FUN_001789a8 @ 001789a8 ===== */

void FUN_001789a8(ulong param_1)

{
  long lVar1;
  bool bVar2;
  int iVar3;
  int local_44;
  long local_40;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  (*(code *)(DAT_001a7cd0 + 0x59567c))();
  bVar2 = false;
  local_44 = 0;
  local_40 = 1;
  if ((0xfff < param_1 + 0x38) && ((param_1 & 0xfffffffffffffff8) != 0xffffffffffffffc0)) {
    iVar3 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1 + 0x38,&local_40,8);
    bVar2 = false;
    if ((iVar3 == 1) && (local_40 == 0)) {
      bVar2 = false;
      if ((0xfff < param_1 + 0x40) && ((param_1 & 0xfffffffffffffffc) != 0xffffffffffffffbc)) {
        iVar3 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1 + 0x40,&local_44,4);
        if (iVar3 == 1) {
          bVar2 = local_44 == -1;
        }
        else {
          bVar2 = false;
        }
      }
    }
  }
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(bVar2);
}

/* ===== FUN_00178ab0 @ 00178ab0 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00178ab0(void)

{
  DAT_0022d79c = 0xffffffff;
  DAT_0022d810 = 0;
  DAT_0022d7e8 = 0;
  _DAT_0022d794 = 0;
  _DAT_0022d568 = 0;
  _DAT_0022d560 = 0;
  _DAT_0022d578 = 0;
  _DAT_0022d570 = 0;
  DAT_0022d588 = 0;
  _DAT_0022d580 = 0;
  DAT_0022d598 = 0;
  DAT_0022d590 = 0;
  DAT_0022d7b0 = DAT_0022d7b0 + 1;
  DAT_0022d7a8 = DAT_0022d7a8 + 1;
  DAT_0022d5a8 = 0;
  DAT_0022d5a0 = 0;
  _DAT_0022d5b8 = 0;
  _DAT_0022d5b0 = 0;
  DAT_0022d5c8 = 0;
  _DAT_0022d5c0 = 0;
  _DAT_0022d5d8 = 0;
  DAT_0022d5d0 = 0;
  _DAT_0022d5e8 = 0;
  _DAT_0022d5e0 = 0;
  DAT_0022d5f8 = 0;
  _DAT_0022d5f0 = 0;
  DAT_0022d530 = 0;
  DAT_0022d51c = 0;
  return;
}

/* ===== FUN_00179000 @ 00179000 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00179000(undefined8 param_1,undefined8 param_2,undefined8 param_3,int *param_4,uint param_5
                 ,undefined4 param_6)

{
  uint uVar1;
  int iVar2;
  long lVar3;
  ulong uVar4;
  int *piVar5;
  bool bVar6;
  int iVar7;
  undefined8 uVar8;
  ulong uVar9;
  uint uVar10;
  ulong uVar11;
  float fVar12;
  float fVar13;
  undefined8 local_70;
  undefined4 uStack_68;
  undefined4 uStack_64;
  undefined4 uStack_60;
  undefined4 uStack_5c;
  undefined8 uStack_58;
  undefined8 local_50;
  long local_48;
  
  lVar3 = tpidr_el0;
  local_48 = *(long *)(lVar3 + 0x28);
  iVar7 = FUN_001830b0();
  if (iVar7 == 0) {
LAB_001793d8:
    if (*(long *)(lVar3 + 0x28) == local_48) {
      return;
    }
    goto LAB_00179444;
  }
  if ((param_5 < 0xb) && ((param_4 != (int *)0x0 || (param_5 == 0)))) {
    if (param_5 == 0) {
      if (DAT_0022d574 != 0) goto LAB_00179284;
      if (DAT_0022d564 != 0) {
LAB_00179168:
        uStack_68 = (undefined4)_DAT_0022d5e0;
        local_70 = _DAT_0022d5d8;
        uStack_58 = _DAT_0022d5f0;
        uStack_5c = (undefined4)((ulong)_DAT_0022d5e8 >> 0x20);
        local_50 = DAT_0022d5f8;
        uStack_64 = (undefined4)DAT_0010f740;
        uStack_60 = (undefined4)((ulong)DAT_0010f740 >> 0x20);
        FUN_00183658(param_1,&local_70,param_6,1);
LAB_001791a0:
        if (param_5 != 0) {
          if (DAT_0022d564 != 0) goto LAB_001791b0;
          if (DAT_0022d584 < *param_4) {
            DAT_0022d564 = 1;
            DAT_0022d5a8 = DAT_0022d5a8 + 1;
            DAT_0022d56c = (undefined4)DAT_0010f740;
            DAT_0022d570 = (undefined4)((ulong)DAT_0010f740 >> 0x20);
            fVar12 = (float)param_4[1];
            fVar13 = (float)param_4[2];
            _DAT_0022d578 = 0;
            uVar10 = 0;
            if (fVar12 <= 350.0) {
              uVar10 = (uint)(fVar12 < -530.0 == NAN(fVar12));
            }
            uVar1 = 0;
            if (fVar13 < 132.0 == NAN(fVar13)) {
              uVar1 = uVar10;
            }
            DAT_0022d568 = 0;
            if (fVar13 <= 472.0) {
              DAT_0022d568 = uVar1;
            }
            _DAT_0022d5e0 = *(undefined8 *)(param_4 + 2);
            _DAT_0022d5d8 = *(undefined8 *)param_4;
            _DAT_0022d5f0 = *(undefined8 *)(param_4 + 6);
            _DAT_0022d5e8 = *(undefined8 *)(param_4 + 4);
            DAT_0022d5f8 = *(undefined8 *)(param_4 + 8);
            _DAT_0022d5b8 = *(undefined8 *)(param_4 + 2);
            _DAT_0022d5b0 = *(undefined8 *)param_4;
            DAT_0022d5c8 = *(undefined8 *)(param_4 + 6);
            _DAT_0022d5c0 = *(undefined8 *)(param_4 + 4);
            DAT_0022d5d0 = *(undefined8 *)(param_4 + 8);
            DAT_0022d584 = *param_4;
            DAT_0022d598 = param_1;
            FUN_00183230(param_4[1],param_4[2],&DAT_0022d518,param_1,1,1,*param_4,param_6);
            if ((DAT_0022d568 != 0) && (DAT_0022d530 == 0)) {
              DAT_0022d570 = 0;
            }
            if (param_4[4] != 0) {
              if (*(long *)(lVar3 + 0x28) != local_48) goto LAB_00179444;
              iVar7 = 1;
              goto LAB_001791d0;
            }
            goto LAB_001793d8;
          }
          if (param_4[3] != 0) {
            if (*(long *)(lVar3 + 0x28) == local_48) {
              FUN_00179dc0(&DAT_0022d560,&DAT_0022d518,param_1,param_6);
              return;
            }
            goto LAB_00179444;
          }
        }
      }
    }
    else {
      uVar11 = 0;
      bVar6 = false;
      uVar9 = (ulong)param_5;
      do {
        if (((((param_4[uVar11 * 10] < 0) || (ABS((float)param_4[uVar11 * 10 + 1]) == INFINITY)) ||
             (NAN(ABS((float)param_4[uVar11 * 10 + 1])))) ||
            ((ABS((float)param_4[uVar11 * 10 + 2]) == INFINITY ||
             (NAN(ABS((float)param_4[uVar11 * 10 + 2])))))) ||
           ((1 < (uint)param_4[uVar11 * 10 + 3] ||
            ((1 < (uint)param_4[uVar11 * 10 + 4] ||
             (uVar4 = uVar11, piVar5 = param_4, param_4[uVar11 * 10 + 3] == param_4[uVar11 * 10 + 4]
             )))))) break;
        for (; uVar4 != 0; uVar4 = uVar4 - 1) {
          if (*piVar5 == param_4[uVar11 * 10]) goto LAB_00179110;
          piVar5 = piVar5 + 10;
        }
        uVar11 = uVar11 + 1;
        bVar6 = uVar9 <= uVar11;
      } while (uVar11 != uVar9);
LAB_00179110:
      if (!bVar6) goto LAB_00179220;
      if ((param_5 < 2) && (DAT_0022d574 == 0)) {
        if (DAT_0022d564 == 0) goto LAB_001791a0;
        if ((param_5 == 0) || (*param_4 != DAT_0022d5b0)) goto LAB_00179168;
LAB_001791b0:
        iVar7 = param_4[4];
        if (*(long *)(lVar3 + 0x28) == local_48) {
LAB_001791d0:
          FUN_00183658(param_1,param_4,param_6,iVar7);
          return;
        }
        goto LAB_00179444;
      }
      iVar7 = DAT_0022d584;
      if (param_5 != 0) {
        do {
          iVar2 = *param_4;
          if (iVar7 < iVar2) {
            iVar7 = iVar2;
            DAT_0022d584 = iVar2;
          }
          param_4 = param_4 + 10;
          uVar9 = uVar9 - 1;
        } while (uVar9 != 0);
        if (param_5 != 0) goto LAB_00179220;
      }
LAB_00179284:
      DAT_0022d574 = 0;
    }
    if (*(long *)(lVar3 + 0x28) == local_48) {
      uVar8 = 0;
LAB_00179264:
      FUN_00183230(0,0,&DAT_0022d518,param_1,0,uVar8,0xffffffff,param_6);
      return;
    }
  }
  else {
LAB_00179220:
    DAT_0022d564 = 0;
    _DAT_0022d578 = _UNK_0010fa98;
    DAT_0022d570 = (undefined4)_DAT_0010fa90;
    DAT_0022d574 = (int)((ulong)_DAT_0010fa90 >> 0x20);
    if (*(long *)(lVar3 + 0x28) == local_48) {
      uVar8 = 4;
      goto LAB_00179264;
    }
  }
LAB_00179444:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00179448 @ 00179448 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00179448(void)

{
  long lVar1;
  bool bVar2;
  int iVar3;
  undefined8 local_a8;
  undefined8 uStack_a0;
  code *local_98;
  code *pcStack_90;
  code *local_88;
  code *pcStack_80;
  code *local_78;
  code *pcStack_70;
  code *local_68;
  code *pcStack_60;
  code *local_58;
  code *pcStack_50;
  code *local_48;
  code *pcStack_40;
  code *local_38;
  code *pcStack_30;
  long local_28;
  
  lVar1 = tpidr_el0;
  local_28 = *(long *)(lVar1 + 0x28);
  if (DAT_0022d4e0 == 0) {
    bVar2 = ((DAT_001a7d18 < 0x2c && DAT_0022d4e8 == 0) && DAT_0022d4f0 == 0) && _DAT_0022d514 == 0;
  }
  else {
    local_a8 = DAT_001a7cd0;
    uStack_a0 = 0;
    local_98 = FUN_0017fe9c;
    pcStack_90 = FUN_0017fee0;
    local_88 = FUN_00180034;
    pcStack_80 = FUN_001802bc;
    local_78 = FUN_00180540;
    pcStack_70 = FUN_00180548;
    local_68 = FUN_00180564;
    pcStack_60 = FUN_00180580;
    local_58 = FUN_0018058c;
    pcStack_50 = FUN_001805a4;
    local_48 = FUN_00180724;
    pcStack_40 = FUN_0018073c;
    local_38 = FUN_00180758;
    pcStack_30 = FUN_0017fbc8;
    if (DAT_0022d4f8 == DAT_001a7d38) {
      iVar3 = 0;
      if (0x2a < DAT_001a7d18) {
        iVar3 = DAT_001a7d18 - 0x2b;
      }
      iVar3 = FUN_00180d80(&local_a8,&DAT_0022d4e0,DAT_001a7d28,iVar3);
      bVar2 = iVar3 != 0;
    }
    else {
      bVar2 = false;
    }
  }
  if (*(long *)(lVar1 + 0x28) == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(bVar2);
}

/* ===== FUN_001795ac @ 001795ac ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_001795ac(void)

{
  long lVar1;
  bool bVar2;
  int iVar3;
  long local_58;
  long local_50;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 uStack_40;
  undefined4 local_3c;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  bVar2 = false;
  local_50 = 0;
  local_48 = 0;
  local_44 = (undefined4)_DAT_001a7d5c;
  uStack_40 = (undefined4)((ulong)_DAT_001a7d5c >> 0x20);
  local_3c = DAT_001a7d60._4_4_;
  if (((DAT_0022d810 == 0) || (DAT_001a7d28 + 0x178 < 0x1000)) ||
     ((DAT_001a7d28 & 0xfffffffffffffffc) == 0xfffffffffffffe84)) goto LAB_0017971c;
  iVar3 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_001a7d28 + 0x178,&local_50,4);
  if (iVar3 == 1) {
    bVar2 = false;
    if ((DAT_001a7d28 + 0x4c < 0x1000) ||
       ((DAT_001a7d28 & 0xfffffffffffffffc) == 0xffffffffffffffb0)) goto LAB_0017971c;
    iVar3 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_001a7d28 + 0x4c,(ulong)&local_50 | 4,4);
    if (iVar3 == 1) {
      bVar2 = false;
      if ((DAT_001a7d28 + 0x54 < 0x1000) ||
         ((DAT_001a7d28 & 0xfffffffffffffffc) == 0xffffffffffffffa8)) goto LAB_0017971c;
      iVar3 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_001a7d28 + 0x54,&local_48,4);
      if ((iVar3 == 1) &&
         (((0x200 < DAT_001a7d28 + 0x28c >> 3 &&
           (iVar3 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_001a7d28 + 0x284,&local_58,8), iVar3 == 1)) &&
          ((local_50 == DAT_0022d7f0 && CONCAT44(local_44,local_48) == DAT_0022d7f8) &&
           CONCAT44(local_3c,uStack_40) == DAT_0022d800)))) {
        bVar2 = local_58 == DAT_0022d808;
        goto LAB_0017971c;
      }
    }
  }
  bVar2 = false;
LAB_0017971c:
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(bVar2);
}

/* ===== FUN_0017975c @ 0017975c ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0017975c(int param_1,int *param_2,uint *param_3)

{
  pthread_mutex_t *__mutex;
  long lVar1;
  byte bVar2;
  byte bVar3;
  long lVar4;
  bool bVar5;
  bool bVar6;
  int iVar7;
  int *piVar8;
  undefined8 uVar9;
  ulong uVar10;
  uint uVar11;
  ulong uVar12;
  undefined8 *puVar13;
  ulong uVar14;
  float fVar15;
  float fVar16;
  float fVar17;
  undefined8 uVar18;
  float fVar19;
  float fVar20;
  uint local_13c;
  undefined8 local_138;
  int local_12c;
  byte local_128 [184];
  long local_70;
  
  lVar4 = tpidr_el0;
  local_70 = *(long *)(lVar4 + 0x28);
  if (DAT_0022d7ec == 1) {
    __mutex = (pthread_mutex_t *)(DAT_001a7cd0 + 0x12fd3d0);
    iVar7 = pthread_mutex_trylock(__mutex);
    uVar11 = 0xffffffff;
    if (iVar7 == 0) {
      lVar1 = 0x12fd3fc;
      if (param_1 != 0) {
        lVar1 = 0x12fd3f8;
      }
      local_13c = 0xffffffff;
      if ((((0x400 < DAT_001a7cd0 + lVar1 + 4U >> 2) &&
           (iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_001a7cd0 + lVar1,&local_13c,4), iVar7 == 1)) &&
          (-1 < (int)local_13c)) && ((int)local_13c < 0xb)) {
        lVar1 = 0x12fd4c8;
        if (param_1 != 0) {
          lVar1 = 0x12fd400;
        }
        if (0x218 < DAT_001a7cd0 + lVar1 + 200U >> 3) {
          iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_001a7cd0 + lVar1,&local_138,200);
          pthread_mutex_unlock(__mutex);
          if (iVar7 == 1) {
            uVar10 = (ulong)local_13c;
            if (0 < (int)local_13c) {
              uVar12 = 0;
              do {
                uVar9 = DAT_0022d7b0;
                if (DAT_001a7d5c <= 0.0) goto LAB_0017980c;
                uVar11 = 0;
                if (((ABS(DAT_001a7d5c) == INFINITY) || (NAN(ABS(DAT_001a7d5c)))) ||
                   ((DAT_001e53fc <= 0.0 ||
                    ((ABS(DAT_001e53fc) == INFINITY || (NAN(ABS(DAT_001e53fc))))))))
                goto LAB_00179810;
                iVar7 = (&local_12c)[uVar12 * 5];
                if ((iVar7 < 0) ||
                   ((bVar2 = local_128[uVar12 * 0x14 + 2], 1 < bVar2 ||
                    (bVar3 = local_128[uVar12 * 0x14 + 1], 1 < bVar3)))) goto LAB_0017980c;
                uVar11 = 0;
                if (1 < local_128[uVar12 * 0x14]) goto LAB_00179810;
                if ((uint)bVar2 == (uint)bVar3) goto LAB_00179810;
                uVar11 = 0;
                uVar18 = NEON_scvtf(*(undefined8 *)((long)&local_138 + uVar12 * 0x14),4);
                fVar15 = (((float)uVar18 - (float)_DAT_001e5400) / DAT_001e53fc -
                         (float)DAT_001a7d60) / DAT_001a7d5c;
                fVar16 = (((float)((ulong)uVar18 >> 0x20) - (float)((ulong)_DAT_001e5400 >> 0x20)) /
                          DAT_001e53fc - (float)((ulong)DAT_001a7d60 >> 0x20)) / DAT_001a7d5c;
                fVar17 = ABS(fVar15);
                if ((((fVar17 != 100000.0 && fVar17 < 100000.0 == NAN(fVar17)) ||
                     (fVar17 == INFINITY)) || (NAN(fVar17))) ||
                   ((fVar17 = ABS(fVar16), fVar17 == INFINITY ||
                    (fVar17 != 100000.0 && fVar17 < 100000.0 == NAN(fVar17))))) goto LAB_00179810;
                piVar8 = param_2 + uVar12 * 10;
                piVar8[3] = (uint)bVar2;
                piVar8[4] = (uint)bVar3;
                *piVar8 = iVar7;
                *(ulong *)(piVar8 + 1) = CONCAT44(fVar16,fVar15);
                piVar8[5] = 0;
                piVar8[6] = 0;
                piVar8[7] = 0;
                *(undefined8 *)(piVar8 + 8) = uVar9;
                if (DAT_001a7d18 != 0) {
                  puVar13 = &DAT_001c3568 + (ulong)(DAT_001a7d18 - 1) * 0x1b;
                  uVar14 = (ulong)DAT_001a7d18;
                  do {
                    uVar11 = (uint)(uVar14 - 1);
                    if (((*(int *)((long)puVar13 + 0xd4) != 0) && (*(int *)(puVar13 + 4) == 2)) &&
                       (*(char *)((long)puVar13 + 0x4c) != '\0')) {
                      if ((*(int *)(puVar13 + 5) != 0) &&
                         (uVar14 < 0x2c ||
                          fVar16 <= 472.0 &&
                          (fVar16 < 132.0 == NAN(fVar16) &&
                          (fVar15 <= 350.0 && fVar15 < -530.0 == NAN(fVar15))))) {
                        fVar17 = *(float *)((long)puVar13 + 0x44);
                        if (fVar17 == 0.0 || fVar17 < 0.0 != NAN(fVar17)) {
                          fVar17 = 168.0;
                        }
                        fVar19 = (float)NEON_fmsub(fVar17,0x3f000000,
                                                   *(undefined4 *)((long)puVar13 + 0x34));
                        fVar17 = (float)NEON_fmadd(fVar17,0x3f000000,
                                                   *(undefined4 *)((long)puVar13 + 0x34));
                        bVar5 = false;
                        bVar6 = true;
                        if (fVar15 < fVar19 == (NAN(fVar15) || NAN(fVar19))) {
                          bVar5 = false;
                          bVar6 = true;
                          if (!NAN(fVar15) && !NAN(fVar17)) {
                            bVar5 = fVar15 == fVar17;
                            bVar6 = fVar17 <= fVar15;
                          }
                        }
                        if (!bVar6 || bVar5) {
                          fVar17 = *(float *)(puVar13 + 9);
                          if (fVar17 == 0.0 || fVar17 < 0.0 != NAN(fVar17)) {
                            fVar17 = 64.0;
                          }
                          fVar19 = DAT_0022d7e8;
                          if (uVar11 < 0x2b) {
                            fVar19 = 0.0;
                          }
                          fVar20 = (float)NEON_fmsub(fVar17,0x3f000000,*(undefined4 *)(puVar13 + 7))
                          ;
                          fVar19 = fVar16 + fVar19;
                          if ((fVar19 < fVar20 == (NAN(fVar19) || NAN(fVar20))) &&
                             (fVar17 = (float)NEON_fmadd(fVar17,0x3f000000,
                                                         *(undefined4 *)(puVar13 + 7)),
                             fVar19 <= fVar17)) {
                            uVar9 = *puVar13;
                            piVar8[5] = *(int *)(puVar13 + 5);
                            *(undefined8 *)(piVar8 + 6) = uVar9;
                            break;
                          }
                        }
                      }
                    }
                    puVar13 = puVar13 + -0x1b;
                    uVar14 = uVar14 - 1;
                  } while (uVar11 != 0);
                }
                uVar12 = uVar12 + 1;
              } while (uVar12 != uVar10);
            }
            *param_3 = local_13c;
            if ((local_13c < 0xb) && ((param_2 != (int *)0x0 || (local_13c == 0)))) {
              if (local_13c == 0) {
                uVar11 = 1;
              }
              else {
                uVar12 = 0;
                uVar11 = 0;
                do {
                  if ((((param_2[uVar12 * 10] < 0) ||
                       (ABS((float)param_2[uVar12 * 10 + 1]) == INFINITY)) ||
                      (NAN(ABS((float)param_2[uVar12 * 10 + 1])))) ||
                     (((ABS((float)param_2[uVar12 * 10 + 2]) == INFINITY ||
                       (NAN(ABS((float)param_2[uVar12 * 10 + 2])))) ||
                      ((1 < (uint)param_2[uVar12 * 10 + 3] ||
                       ((1 < (uint)param_2[uVar12 * 10 + 4] ||
                        (uVar14 = uVar12, piVar8 = param_2,
                        param_2[uVar12 * 10 + 3] == param_2[uVar12 * 10 + 4])))))))) break;
                  for (; uVar14 != 0; uVar14 = uVar14 - 1) {
                    if (*piVar8 == param_2[uVar12 * 10]) goto LAB_00179810;
                    piVar8 = piVar8 + 10;
                  }
                  uVar12 = uVar12 + 1;
                  uVar11 = (uint)(uVar10 <= uVar12);
                } while (uVar12 != uVar10);
              }
              goto LAB_00179810;
            }
          }
          goto LAB_0017980c;
        }
      }
      pthread_mutex_unlock(__mutex);
    }
    else if (iVar7 == 0x10) goto LAB_00179810;
  }
LAB_0017980c:
  uVar11 = 0;
LAB_00179810:
  if (*(long *)(lVar4 + 0x28) == local_70) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar11);
}

/* ===== FUN_00179c50 @ 00179c50 ===== */

void FUN_00179c50(float param_1)

{
  long lVar1;
  int iVar2;
  undefined8 uVar3;
  float fVar4;
  undefined8 local_a8;
  undefined8 uStack_a0;
  code *local_98;
  code *pcStack_90;
  code *local_88;
  code *pcStack_80;
  code *local_78;
  code *pcStack_70;
  code *local_68;
  code *pcStack_60;
  code *local_58;
  code *pcStack_50;
  code *local_48;
  code *pcStack_40;
  code *local_38;
  code *pcStack_30;
  long local_28;
  
  lVar1 = tpidr_el0;
  local_28 = *(long *)(lVar1 + 0x28);
  fVar4 = -132.0 - (float)(int)(-132.0 - param_1);
  if (fVar4 == DAT_0022d7e8) {
    uVar3 = 1;
  }
  else {
    local_a8 = DAT_001a7cd0;
    uStack_a0 = 0;
    local_48 = FUN_00180724;
    pcStack_40 = FUN_0018073c;
    local_98 = FUN_0017fe9c;
    pcStack_90 = FUN_0017fee0;
    local_88 = FUN_00180034;
    pcStack_80 = FUN_001802bc;
    local_78 = FUN_00180540;
    pcStack_70 = FUN_00180548;
    local_68 = FUN_00180564;
    pcStack_60 = FUN_00180580;
    local_58 = FUN_0018058c;
    pcStack_50 = FUN_001805a4;
    iVar2 = 0;
    if (0x2a < DAT_001a7d18) {
      iVar2 = DAT_001a7d18 - 0x2b;
    }
    local_38 = FUN_00180758;
    pcStack_30 = FUN_0017fbc8;
    iVar2 = FUN_0017fc3c(0x44048000,-132.0 - fVar4,&local_a8,DAT_001a7d28,iVar2,1);
    uVar3 = 0;
    if (iVar2 == 1) {
      uVar3 = 1;
      DAT_0022d7c8 = DAT_0022d7c8 + 1;
      DAT_0022d7e8 = fVar4;
    }
  }
  if (*(long *)(lVar1 + 0x28) == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar3);
}

/* ===== FUN_00179dc0 @ 00179dc0 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00179dc0(long param_1,undefined8 param_2,undefined8 param_3,undefined4 param_4)

{
  undefined8 uVar1;
  
  uVar1 = _DAT_0010fa90;
  *(undefined8 *)(param_1 + 0x18) = _UNK_0010fa98;
  *(undefined8 *)(param_1 + 0x10) = uVar1;
  *(undefined4 *)(param_1 + 4) = 0;
  FUN_00183230(0,0,param_2,param_3,0,4,0xffffffff,param_4);
  return;
}

/* ===== FUN_00179dfc @ 00179dfc ===== */

void FUN_00179dfc(ulong param_1)

{
  ulong uVar1;
  long lVar2;
  bool bVar3;
  ulong uVar4;
  ulong uVar5;
  bool bVar6;
  int iVar7;
  uint uVar8;
  uint uVar9;
  ulong local_60;
  long local_58;
  
  lVar2 = tpidr_el0;
  local_58 = *(long *)(lVar2 + 0x28);
  if (param_1 != 0) {
    local_60 = 0;
    if ((((param_1 + 0x30 < 0x1000) || ((param_1 & 0xfffffffffffffff8) == 0xffffffffffffffc8)) ||
        (iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1 + 0x30,&local_60,8), uVar4 = DAT_00282158,
        iVar7 != 1)) || ((local_60 != 0 && (local_60 != DAT_001a7d28)))) {
      param_1 = 0;
    }
    else {
      uVar9 = 0;
      do {
        uVar8 = 1;
        if ((param_1 == uVar4) || (0xf < uVar9)) goto LAB_00179f58;
        local_60 = 0;
        if (param_1 + 0x40 >> 3 < 0x201) {
          bVar6 = false;
        }
        else {
          iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1 + 0x38,&local_60,8);
          bVar6 = iVar7 == 1;
        }
        uVar5 = local_60;
        bVar3 = false;
        if (0xfff < local_60) {
          bVar3 = bVar6;
        }
        bVar3 = (bool)(bVar3 & (local_60 & 7) == 0);
        uVar1 = local_60;
        if (!bVar3) {
          uVar1 = 0;
        }
        iVar7 = FUN_0016d5cc(param_1,uVar1);
        if (iVar7 == 0) {
          uVar8 = 0;
          goto LAB_00179f58;
        }
        uVar9 = uVar9 + 1;
        param_1 = uVar5;
      } while (bVar3);
      param_1 = 0;
      uVar8 = 1;
LAB_00179f58:
      uVar9 = 0;
      if (param_1 == uVar4) {
        uVar9 = uVar8;
      }
      param_1 = (ulong)uVar9;
    }
  }
  if (*(long *)(lVar2 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(param_1);
}

/* ===== FUN_0017b3f0 @ 0017b3f0 ===== */

void FUN_0017b3f0(void)

{
  FUN_0016cf5c(0xc61c3c00,0xc61c3c00,DAT_00283418);
  FUN_0016cf5c(0xc61c3c00,0xc61c3c00,DAT_00283490);
  FUN_0016cf5c(0xc61c3c00,0xc61c3c00,DAT_00283508);
  FUN_0016cf5c(0xc61c3c00,0xc61c3c00,DAT_00283580);
  FUN_0016cf5c(0xc61c3c00,0xc61c3c00,DAT_002835f8);
  FUN_0016cf5c(0xc61c3c00,0xc61c3c00,DAT_00283348);
  FUN_0016cf5c(0xc61c3c00,0xc61c3c00,DAT_00283350);
  FUN_0016cf5c(0xc61c3c00,0xc61c3c00,DAT_00283358);
  FUN_0016cf5c(0xc61c3c00,0xc61c3c00,DAT_00283360);
  FUN_0016cf5c(0xc61c3c00,0xc61c3c00,DAT_00283368);
  FUN_0016cf5c(0xc61c3c00,0xc61c3c00,DAT_00283370);
  FUN_0016cf5c(0xc61c3c00,0xc61c3c00,DAT_00283678);
  FUN_0016cf5c(0xc61c3c00,0xc61c3c00,DAT_002836f0);
  FUN_0016cf5c(0xc61c3c00,0xc61c3c00,DAT_00283768);
  FUN_0016cf5c(0xc61c3c00,0xc61c3c00,DAT_002837e0);
  FUN_0016cf5c(0xc61c3c00,0xc61c3c00,DAT_00283858);
  FUN_0016cf5c(0xc61c3c00,0xc61c3c00,DAT_002838d0);
  return;
}

/* ===== FUN_0017b530 @ 0017b530 ===== */

undefined4 FUN_0017b530(float param_1,float param_2,float param_3,float param_4,undefined8 *param_5)

{
  undefined4 uVar1;
  float fVar2;
  float fVar3;
  
  uVar1 = 0;
  if (((((ABS(param_1) != INFINITY) && (!NAN(ABS(param_1)))) && (param_5 != (undefined8 *)0x0)) &&
      ((uVar1 = 0, 0.0 < param_2 && (0.0 < param_1)))) &&
     ((ABS(param_2) != INFINITY && (!NAN(ABS(param_2)))))) {
    param_3 = param_3 * param_4;
    uVar1 = 0;
    if (((0.0 < param_3) && (ABS(param_3) != INFINITY)) && (!NAN(ABS(param_3)))) {
      uVar1 = 1;
      fVar2 = (float)DAT_0010f818 * DAT_0022d818;
      fVar3 = (float)((ulong)DAT_0010f818 >> 0x20) * DAT_0022d818;
      *param_5 = CONCAT44((param_2 * -0.5 + (float)((ulong)DAT_0010f738 >> 0x20) * DAT_0022d818) /
                          param_3,(param_1 * -0.5 + (float)DAT_0010f738 * DAT_0022d818) / param_3);
      param_5[1] = CONCAT44(fVar3 / param_3,fVar2 / param_3);
    }
  }
  return uVar1;
}

/* ===== FUN_0017b5e8 @ 0017b5e8 ===== */

void FUN_0017b5e8(undefined4 param_1,undefined4 param_2,float param_3,float param_4,ulong param_5)

{
  long lVar1;
  float fVar2;
  int iVar3;
  undefined8 uVar4;
  float fVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  float local_78;
  float fStack_74;
  float local_70;
  float fStack_6c;
  float local_68;
  float local_64;
  float local_60;
  float local_5c;
  long local_58;
  
  lVar1 = tpidr_el0;
  local_58 = *(long *)(lVar1 + 0x28);
  if (((((((param_5 + 0x10 < 0x1000) || ((param_5 & 0xfffffffffffffff0) == 0xffffffffffffffe0)) ||
         (iVar3 = (*DAT_001a7ce8)(DAT_001a7cc8,param_5 + 0x10,&local_68,0x10), fVar2 = local_5c,
         fVar5 = local_68, iVar3 != 1)) || ((ABS(local_68) == INFINITY || (NAN(ABS(local_68)))))) ||
       (ABS(local_64) == INFINITY)) ||
      (((NAN(ABS(local_64)) || (ABS(local_60) == INFINITY)) ||
       ((NAN(ABS(local_60)) ||
        (((ABS(local_5c) == INFINITY || (NAN(ABS(local_5c)))) || (local_64 != 0.0)))))))) ||
     (((local_60 != 0.0 || (local_68 <= 0.0)) ||
      ((local_5c <= 0.0 ||
       ((local_68 != 128.0 && local_68 < 128.0 == NAN(local_68) ||
        (local_5c != 128.0 && local_5c < 128.0 == NAN(local_5c))))))))) {
    uVar4 = 0;
  }
  else {
    FUN_0016cf5c(0,0,param_5);
    FUN_0016cf84(param_5,&local_68);
    uVar4 = 0;
    if ((ABS(local_60 - local_68) != INFINITY) && (!NAN(ABS(local_60 - local_68)))) {
      uVar4 = 0;
      local_5c = local_5c - local_64;
      if ((((ABS(local_5c) != INFINITY) && (!NAN(ABS(local_5c)))) && (0.0 < local_60 - local_68)) &&
         (0.0 < local_5c)) {
        uVar4 = 0;
        fVar5 = (fVar5 * param_3) / (local_60 - local_68);
        if ((ABS(fVar5) != INFINITY) && (!NAN(ABS(fVar5)))) {
          uVar4 = 0;
          local_5c = (fVar2 * param_4) / local_5c;
          if ((((ABS(local_5c) != INFINITY) && ((!NAN(ABS(local_5c)) && (0.0001 <= fVar5)))) &&
              (0.0001 <= local_5c)) &&
             ((fVar5 == 128.0 || fVar5 < 128.0 != NAN(fVar5) &&
              (local_5c == 128.0 || local_5c < 128.0 != NAN(local_5c))))) {
            FUN_0016cf00(param_5);
            FUN_0016cf84(param_5,&local_78);
            uVar6 = NEON_fmsub(local_78 + local_70,0x3f000000,param_1);
            uVar7 = NEON_fmsub(fStack_74 + fStack_6c,0x3f000000,param_2);
            FUN_0016cf5c(uVar6,uVar7,param_5);
            uVar4 = 1;
          }
        }
      }
    }
  }
  if (*(long *)(lVar1 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar4);
}

/* ===== FUN_0017b858 @ 0017b858 ===== */

undefined * FUN_0017b858(char *param_1)

{
  long lVar1;
  uint uVar2;
  int iVar3;
  size_t sVar4;
  undefined *puVar5;
  ulong uVar6;
  ulong uVar7;
  
  sVar4 = strlen(param_1);
  uVar2 = DAT_001a7d1c;
  if (sVar4 < 0x80) {
    uVar6 = (ulong)DAT_001a7d1c;
    if (DAT_001a7d1c != 0) {
      puVar5 = &DAT_001e5548;
      uVar7 = uVar6;
      do {
        iVar3 = strcmp(puVar5 + -0x80,param_1);
        if (iVar3 == 0) {
          return puVar5;
        }
        puVar5 = puVar5 + 0x90;
        uVar7 = uVar7 - 1;
      } while (uVar7 != 0);
      if (uVar2 == 0x800) goto LAB_0017b87c;
    }
    lVar1 = uVar6 * 0x90;
    DAT_001a7d1c = uVar2 + 1;
    memcpy(&DAT_001e54c8 + lVar1,param_1,sVar4 + 1);
    puVar5 = &DAT_001e5548 + lVar1;
    FUN_0016ced8(puVar5,&DAT_001e54c8 + lVar1);
  }
  else {
LAB_0017b87c:
    puVar5 = (undefined *)0x0;
  }
  return puVar5;
}

/* ===== FUN_0017b920 @ 0017b920 ===== */

void FUN_0017b920(float param_1,float param_2,float param_3,long param_4,char *param_5)

{
  ulong uVar1;
  ulong uVar2;
  long lVar3;
  bool bVar4;
  ulong uVar5;
  bool bVar6;
  int iVar7;
  ulong uVar8;
  uint uVar9;
  short local_8c [2];
  undefined8 local_88;
  long local_78;
  
  lVar3 = tpidr_el0;
  local_78 = *(long *)(lVar3 + 0x28);
  iVar7 = FUN_0017c598(*(undefined8 *)(param_4 + 8));
  if (iVar7 != 0) {
    uVar2 = *(ulong *)(param_4 + 8);
    uVar8 = *(ulong *)(param_4 + 0x10);
    if (uVar8 != 0) {
      uVar9 = 0;
      do {
        if ((uVar8 == uVar2) || (0xf < uVar9)) goto LAB_0017ba10;
        local_88 = 0;
        if (uVar8 + 0x40 >> 3 < 0x201) {
          bVar6 = false;
        }
        else {
          iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar8 + 0x38,&local_88,8);
          bVar6 = iVar7 == 1;
        }
        uVar5 = local_88;
        bVar4 = false;
        if (0xfff < local_88) {
          bVar4 = bVar6;
        }
        bVar4 = (bool)(bVar4 & (local_88 & 7) == 0);
        uVar1 = local_88;
        if (!bVar4) {
          uVar1 = 0;
        }
        iVar7 = FUN_0016d5cc(uVar8,uVar1);
        if (iVar7 == 0) goto LAB_0017bb60;
        uVar9 = uVar9 + 1;
        uVar8 = uVar5;
      } while (bVar4);
      uVar8 = 0;
    }
LAB_0017ba10:
    if (uVar8 == uVar2) {
      iVar7 = strcmp(param_5,(char *)(param_4 + 0x18));
      if (iVar7 != 0) {
        FUN_0016ced8(&local_88,param_5);
        FUN_0016ceac(*(undefined8 *)(param_4 + 0x10),&local_88);
        (*(code *)(DAT_001a7cd0 + 0x66ad48))(&local_88);
        FUN_00176a24((char *)(param_4 + 0x18),0xffffffffffffffff,0x60,&DAT_001343dc,param_5);
        FUN_0016ceec(*(undefined8 *)(param_4 + 0x10),0xffffffff);
      }
      local_8c[0] = 0;
      uVar2 = *(ulong *)(param_4 + 0x10) + 0xb0;
      if ((((0xfff < uVar2) &&
           ((*(ulong *)(param_4 + 0x10) & 0xfffffffffffffffe) != 0xffffffffffffff4e)) &&
          (iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar2,local_8c,2), iVar7 == 1)) &&
         ((0 < local_8c[0] && (local_8c[0] < 0x101)))) {
        param_3 = param_3 / (float)(int)local_8c[0];
        FUN_0016cf00(param_3,param_3,*(undefined8 *)(param_4 + 0x10));
        FUN_0016cf5c(0,0,*(undefined8 *)(param_4 + 0x10));
        FUN_0016cf84(*(undefined8 *)(param_4 + 0x10),&local_88);
        if ((ABS((float)local_88) != INFINITY) && (!NAN(ABS((float)local_88)))) {
          if ((ABS(local_88._4_4_) != INFINITY) && (!NAN(ABS(local_88._4_4_)))) {
            FUN_0016cf5c(param_1 - (float)local_88,param_2 - local_88._4_4_,
                         *(undefined8 *)(param_4 + 0x10));
            FUN_0016cf5c(0,0,*(undefined8 *)(param_4 + 8));
          }
        }
      }
    }
  }
LAB_0017bb60:
  if (*(long *)(lVar3 + 0x28) == local_78) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0017bb9c @ 0017bb9c ===== */

void FUN_0017bb9c(uint param_1)

{
  ulong uVar1;
  long lVar2;
  bool bVar3;
  bool bVar4;
  int iVar5;
  long lVar6;
  ulong local_48;
  int local_40;
  int local_3c;
  long local_38;
  
  lVar2 = tpidr_el0;
  local_38 = *(long *)(lVar2 + 0x28);
  lVar6 = (&DAT_00283350)[param_1];
  local_48 = 0;
  if (lVar6 + 8U >> 3 < 0x201) {
    bVar4 = false;
  }
  else {
    iVar5 = (*DAT_001a7ce8)(DAT_001a7cc8,lVar6,&local_48,8);
    bVar4 = iVar5 == 1;
  }
  bVar3 = false;
  if (0xfff < local_48) {
    bVar3 = bVar4;
  }
  uVar1 = local_48;
  if (!(bool)(bVar3 & (local_48 & 7) == 0)) {
    uVar1 = 0;
  }
  if (uVar1 == DAT_001a7cd0 + 0x11c0c58U) {
    iVar5 = FUN_0017c598(lVar6);
    bVar4 = false;
    if ((iVar5 == 0) || (lVar6 + 0xf4U >> 3 < 0x201)) goto LAB_0017bcf4;
    iVar5 = (*DAT_001a7ce8)(DAT_001a7cc8,lVar6 + 0xec,&local_40,8);
    if ((iVar5 == 1) &&
       ((local_40 == *(int *)(&DAT_0013ce70 + (ulong)param_1 * 4) &&
        (local_3c == *(int *)(&DAT_0013ce84 + (ulong)param_1 * 4))))) {
      local_48 = 0;
      if (lVar6 + 0x130U >> 3 < 0x201) {
        bVar4 = false;
      }
      else {
        iVar5 = (*DAT_001a7ce8)(DAT_001a7cc8,lVar6 + 0x128,&local_48,8);
        bVar4 = iVar5 == 1;
      }
      bVar3 = false;
      if (0xfff < local_48) {
        bVar3 = bVar4;
      }
      uVar1 = local_48;
      if (!(bool)(bVar3 & (local_48 & 7) == 0)) {
        uVar1 = 0;
      }
      iVar5 = FUN_0017c598(uVar1);
      bVar4 = iVar5 != 0;
      goto LAB_0017bcf4;
    }
  }
  bVar4 = false;
LAB_0017bcf4:
  if (*(long *)(lVar2 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(bVar4);
}

/* ===== FUN_0017bd68 @ 0017bd68 ===== */

void FUN_0017bd68(void)

{
  bool bVar1;
  uint uVar2;
  byte bVar3;
  char *pcVar4;
  long lVar5;
  int iVar6;
  int iVar7;
  byte bVar8;
  long local_50;
  long local_48;
  
  lVar5 = tpidr_el0;
  local_48 = *(long *)(lVar5 + 0x28);
  if (DAT_002839f8 == 0) {
    local_50 = 0;
    iVar6 = FUN_00172da8();
    iVar7 = thunk_FUN_0014d168(0x889288,0x2a4,
                               "44f217dd7b9d710fc5d55e552c247906a1dce667d803e2ce688316a5e39d0572");
    bVar3 = iVar6 != 0;
    if (iVar7 != 0) {
      bVar3 = iVar6 != 0 | 2;
    }
    iVar6 = thunk_FUN_0014d168(0x889940,0x13c,
                               "ddcaaa86a0d7437ea77f4cc53f14de1d77459f0348559e71c5edeea917a327e7");
    if (iVar6 != 0) {
      bVar3 = bVar3 | 4;
    }
    iVar6 = thunk_FUN_0014d168(0x5d8874,0xb0,
                               "77399cc53dfa8af7bca45327002bfaaab847dbaea6a1eeba62dec6aaeecc46a4");
    if (iVar6 != 0) {
      bVar3 = bVar3 | 8;
    }
    iVar6 = thunk_FUN_0014d168(0x5d76d4,0x94,
                               "4fe60124ce3d6bb318f6b2293544f34158857b6337d2d5dfa8b05e2c3483a92a");
    if (iVar6 != 0) {
      bVar3 = bVar3 | 0x10;
    }
    bVar8 = bVar3;
    if ((((0xfff < DAT_001a7cd0 + 0x11c0e00) &&
         ((DAT_001a7cd0 & 0xfffffffffffffff8) != 0xfffffffffee3f1f8)) &&
        (iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_001a7cd0 + 0x11c0e00,&local_50,8), iVar6 == 1)) &&
       (bVar8 = bVar3 | 0x20, local_50 != DAT_001a7cd0 + 0x889940)) {
      bVar8 = bVar3;
    }
    DAT_002839f8 = -1;
    if (bVar8 == 0x3f) {
      DAT_002839f8 = 1;
    }
    uVar2 = DAT_002839f4 + 1;
    bVar1 = DAT_002839f4 < 0x30;
    DAT_002839f4 = uVar2;
    if ((bVar1) && (DAT_001a7d00 != (code *)0x0)) {
      pcVar4 = "guards_verified";
      if (bVar8 != 0x3f) {
        pcVar4 = "guards_failed";
      }
      (*DAT_001a7d00)(DAT_001a7cc8,"script_port_camera",pcVar4,bVar8);
    }
  }
  if (*(long *)(lVar5 + 0x28) != local_48) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(DAT_002839f8 == 1);
  }
  return;
}

/* ===== FUN_0017c284 @ 0017c284 ===== */

void FUN_0017c284(ulong param_1)

{
  ulong uVar1;
  long lVar2;
  bool bVar3;
  bool bVar4;
  int iVar5;
  ulong uVar6;
  ulong uVar7;
  byte bVar8;
  ulong uVar9;
  uint uVar10;
  ulong local_60;
  long local_58;
  
  lVar2 = tpidr_el0;
  local_58 = *(long *)(lVar2 + 0x28);
  uVar6 = (*(code *)(DAT_001a7cd0 + 0x5d8874))();
  uVar7 = uVar6;
  if (uVar6 != 0) {
    local_60 = 0;
    if (uVar6 + 8 >> 3 < 0x201) {
      bVar4 = false;
    }
    else {
      iVar5 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar6,&local_60,8);
      bVar4 = iVar5 == 1;
    }
    bVar3 = false;
    if (0xfff < local_60) {
      bVar3 = bVar4;
    }
    uVar9 = local_60;
    if (!(bool)(bVar3 & (local_60 & 7) == 0)) {
      uVar9 = 0;
    }
    uVar7 = 0;
    if (uVar9 == DAT_001a7cd0 + 0x11ad208U) {
      uVar10 = 0;
      uVar9 = uVar6;
      do {
        bVar8 = 1;
        if ((uVar9 == param_1) || (0xf < uVar10)) goto LAB_0017c3ec;
        local_60 = 0;
        if (uVar9 + 0x40 >> 3 < 0x201) {
          bVar4 = false;
        }
        else {
          iVar5 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar9 + 0x38,&local_60,8);
          bVar4 = iVar5 == 1;
        }
        uVar7 = local_60;
        bVar3 = false;
        if (0xfff < local_60) {
          bVar3 = bVar4;
        }
        bVar3 = (bool)(bVar3 & (local_60 & 7) == 0);
        uVar1 = local_60;
        if (!bVar3) {
          uVar1 = 0;
        }
        iVar5 = FUN_0016d5cc(uVar9,uVar1);
        if (iVar5 == 0) {
          bVar8 = 0;
          goto LAB_0017c3ec;
        }
        uVar10 = uVar10 + 1;
        uVar9 = uVar7;
      } while (bVar3);
      uVar9 = 0;
      bVar8 = 1;
LAB_0017c3ec:
      uVar7 = uVar6;
      if (!(bool)(bVar8 & uVar9 == param_1)) {
        uVar7 = 0;
      }
    }
  }
  if (*(long *)(lVar2 + 0x28) != local_58) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar7);
  }
  return;
}

/* ===== FUN_0017c598 @ 0017c598 ===== */

void FUN_0017c598(ulong param_1)

{
  ulong uVar1;
  long lVar2;
  bool bVar3;
  ulong uVar4;
  ulong uVar5;
  bool bVar6;
  int iVar7;
  uint uVar8;
  uint uVar9;
  ulong local_60;
  long local_58;
  
  lVar2 = tpidr_el0;
  local_58 = *(long *)(lVar2 + 0x28);
  if (param_1 != 0) {
    local_60 = 0;
    if ((((param_1 + 0x30 < 0x1000) || ((param_1 & 0xfffffffffffffff8) == 0xffffffffffffffc8)) ||
        (iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1 + 0x30,&local_60,8), uVar4 = DAT_00283340,
        iVar7 != 1)) || ((local_60 != 0 && (local_60 != DAT_001a7d28)))) {
      param_1 = 0;
    }
    else {
      uVar9 = 0;
      do {
        uVar8 = 1;
        if ((param_1 == uVar4) || (0xf < uVar9)) goto LAB_0017c6f4;
        local_60 = 0;
        if (param_1 + 0x40 >> 3 < 0x201) {
          bVar6 = false;
        }
        else {
          iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1 + 0x38,&local_60,8);
          bVar6 = iVar7 == 1;
        }
        uVar5 = local_60;
        bVar3 = false;
        if (0xfff < local_60) {
          bVar3 = bVar6;
        }
        bVar3 = (bool)(bVar3 & (local_60 & 7) == 0);
        uVar1 = local_60;
        if (!bVar3) {
          uVar1 = 0;
        }
        iVar7 = FUN_0016d5cc(param_1,uVar1);
        if (iVar7 == 0) {
          uVar8 = 0;
          goto LAB_0017c6f4;
        }
        uVar9 = uVar9 + 1;
        param_1 = uVar5;
      } while (bVar3);
      param_1 = 0;
      uVar8 = 1;
LAB_0017c6f4:
      uVar9 = 0;
      if (param_1 == uVar4) {
        uVar9 = uVar8;
      }
      param_1 = (ulong)uVar9;
    }
  }
  if (*(long *)(lVar2 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(param_1);
}

/* ===== FUN_0017c704 @ 0017c704 ===== */

void FUN_0017c704(ulong *param_1,int param_2,ulong *param_3)

{
  ulong *puVar1;
  ushort *puVar2;
  ulong uVar3;
  long lVar4;
  bool bVar5;
  bool bVar6;
  int iVar7;
  long lVar8;
  undefined4 uVar9;
  ulong uVar10;
  ulong *puVar11;
  long lVar12;
  ulong *puVar13;
  short local_74 [2];
  ulong local_70;
  long local_68;
  
  lVar4 = tpidr_el0;
  local_68 = *(long *)(lVar4 + 0x28);
  param_3[7] = 0;
  param_3[6] = 0;
  param_3[9] = 0;
  param_3[8] = 0;
  param_3[5] = 0;
  param_3[4] = 0;
  param_3[1] = 0;
  *param_3 = 0;
  puVar13 = param_3 + 2;
  param_3[3] = 0;
  *puVar13 = 0;
  uVar10 = *param_1;
  param_3[1] = param_1[1];
  *param_3 = uVar10;
  puVar11 = param_3 + 7;
  *puVar11 = 0xfffffffefffffffe;
  if (((0xfff < uVar10 + 0x80) && ((uVar10 & 0xfffffffffffffff8) != 0xffffffffffffff78)) &&
     (iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar10 + 0x80,puVar13,8), iVar7 == 1)) {
    *(uint *)((long)param_3 + 0x44) = *(uint *)((long)param_3 + 0x44) | 1;
  }
  puVar1 = param_3 + 3;
  uVar10 = *param_1 + 0x88;
  if (((0xfff < uVar10) && ((*param_1 & 0xfffffffffffffff8) != 0xffffffffffffff70)) &&
     (iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar10,puVar1,8), iVar7 == 1)) {
    *(uint *)((long)param_3 + 0x44) = *(uint *)((long)param_3 + 0x44) | 2;
  }
  uVar10 = *param_1 + 0xb0;
  if (((0xfff < uVar10) && ((*param_1 & 0xfffffffffffffff8) != 0xffffffffffffff48)) &&
     (iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar10,param_3 + 4,8), iVar7 == 1)) {
    *(uint *)((long)param_3 + 0x44) = *(uint *)((long)param_3 + 0x44) | 4;
  }
  uVar10 = param_1[1] + 0x38;
  if (((0xfff < uVar10) && ((param_1[1] & 0xfffffffffffffff8) != 0xffffffffffffffc0)) &&
     (iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar10,param_3 + 5,8), iVar7 == 1)) {
    *(uint *)((long)param_3 + 0x44) = *(uint *)((long)param_3 + 0x44) | 8;
  }
  uVar10 = param_1[1] + 0x40;
  if (((0xfff < uVar10) && ((param_1[1] & 0xfffffffffffffffc) != 0xffffffffffffffbc)) &&
     (iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar10,puVar11,4), iVar7 == 1)) {
    *(uint *)((long)param_3 + 0x44) = *(uint *)((long)param_3 + 0x44) | 0x10;
  }
  uVar10 = *puVar1 + 0x38;
  if (((0xfff < uVar10) && ((*puVar1 & 0xfffffffffffffff8) != 0xffffffffffffffc0)) &&
     (iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar10,param_3 + 6,8), iVar7 == 1)) {
    *(uint *)((long)param_3 + 0x44) = *(uint *)((long)param_3 + 0x44) | 0x20;
  }
  uVar10 = *puVar1 + 0x40;
  if (((0xfff < uVar10) && ((*puVar1 & 0xfffffffffffffffc) != 0xffffffffffffffbc)) &&
     (iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar10,(long)param_3 + 0x3c,4), iVar7 == 1)) {
    *(uint *)((long)param_3 + 0x44) = *(uint *)((long)param_3 + 0x44) | 0x40;
  }
  uVar10 = param_3[3] + 0xc0;
  if (((0xfff < uVar10) && ((param_3[3] & 0xfffffffffffffffe) != 0xffffffffffffff3e)) &&
     (iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar10,param_3 + 8,2), iVar7 == 1)) {
    *(uint *)((long)param_3 + 0x44) = *(uint *)((long)param_3 + 0x44) | 0x80;
  }
  puVar2 = (ushort *)((long)param_3 + 0x42);
  uVar10 = param_3[3] + 0x4e;
  if (((0xfff < uVar10) && ((param_3[3] & 0xfffffffffffffffe) != 0xffffffffffffffb0)) &&
     (iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar10,puVar2,2), iVar7 == 1)) {
    *(uint *)((long)param_3 + 0x44) = *(uint *)((long)param_3 + 0x44) | 0x100;
  }
  if (((*(uint *)((long)param_3 + 0x44) ^ 0xffffffff) & 0x7f) == 0) {
    local_70 = 0;
    if (*param_1 + 8 >> 3 < 0x201) {
      bVar6 = false;
    }
    else {
      iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,*param_1,&local_70,8);
      bVar6 = iVar7 == 1;
    }
    bVar5 = false;
    if (0xfff < local_70) {
      bVar5 = bVar6;
    }
    uVar10 = local_70;
    if (!(bool)(bVar5 & (local_70 & 7) == 0)) {
      uVar10 = 0;
    }
    if (uVar10 == DAT_001a7cd0 + 0x11c0a48U) {
      local_70 = 0;
      if (param_1[1] + 8 >> 3 < 0x201) {
        bVar6 = false;
      }
      else {
        iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1[1],&local_70,8);
        bVar6 = iVar7 == 1;
      }
      bVar5 = false;
      if (0xfff < local_70) {
        bVar5 = bVar6;
      }
      uVar10 = DAT_001a7cd0 + 0x11ad208;
      uVar3 = local_70;
      if (!(bool)(bVar5 & (local_70 & 7) == 0)) {
        uVar3 = 0;
      }
      if (((uVar3 == uVar10) && (*puVar13 == param_1[1])) && (param_3[4] == *puVar13)) {
        local_70 = 0;
        if (*puVar1 + 8 >> 3 < 0x201) {
          bVar6 = false;
        }
        else {
          iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,*puVar1,&local_70,8);
          bVar6 = iVar7 == 1;
          uVar10 = DAT_001a7cd0 + 0x11ad208;
        }
        bVar5 = false;
        if (0xfff < local_70) {
          bVar5 = bVar6;
        }
        uVar3 = local_70;
        if (!(bool)(bVar5 & (local_70 & 7) == 0)) {
          uVar3 = 0;
        }
        if (uVar3 == uVar10) {
          local_70 = 0;
          uVar10 = param_1[1] + 0x30;
          if (((0xfff < uVar10) && ((param_1[1] & 0xfffffffffffffff8) != 0xffffffffffffffc8)) &&
             ((iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar10,&local_70,8), iVar7 == 1 &&
              ((local_70 == 0 || (local_70 == DAT_001a7d28)))))) {
            lVar8 = FUN_0016df78(*puVar1);
            if (((int)lVar8 == 0) || (lVar8 = FUN_0016d5cc(*puVar1,*param_1), (int)lVar8 == 0))
            goto LAB_0017cc28;
            uVar10 = *puVar1;
            if ((param_2 == 0) || (uVar10 == param_1[3])) {
              if (uVar10 == param_1[1]) {
                lVar8 = 1;
                *(undefined4 *)((long)param_3 + 0x4c) = 1;
                goto LAB_0017cc28;
              }
              lVar8 = 0;
              local_74[0] = 0;
              if (((*(uint *)((long)param_3 + 0x44) ^ 0xffffffff) & 0x1ff) != 0) goto LAB_0017cc28;
              if (((ushort)param_3[8] - 0x201 < 0xfffffe00) || (0x200 < *puVar2)) goto LAB_0017cc24;
              lVar8 = 0;
              if ((uVar10 + 0xbe < 0x1000) || ((uVar10 & 0xfffffffffffffffe) == 0xffffffffffffff40))
              goto LAB_0017cc28;
              iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar10 + 0xbe,local_74,2);
              lVar8 = 0;
              if (((iVar7 != 1) || (local_74[0] < 1)) ||
                 (lVar8 = FUN_0016f79c(*puVar1 + 0x90), lVar8 == 0)) goto LAB_0017cc28;
              if ((short)param_3[8] < 1) {
                iVar7 = (int)param_3[9];
              }
              else {
                lVar12 = 0;
                do {
                  local_70 = 0;
                  if ((lVar8 + 8U >> 3 < 0x201) ||
                     (iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,lVar8,&local_70,8), iVar7 != 1))
                  goto LAB_0017cc24;
                  lVar12 = lVar12 + 1;
                  lVar8 = lVar8 + 8;
                  iVar7 = (int)param_3[9];
                  if (local_70 == param_1[1]) {
                    iVar7 = iVar7 + 1;
                  }
                  *(int *)(param_3 + 9) = iVar7;
                } while (lVar12 < (short)param_3[8]);
              }
              if (iVar7 == 1) {
                iVar7 = FUN_0016d5cc(param_1[1],*puVar1);
                if (iVar7 != 0) {
                  uVar9 = 2;
LAB_0017cdcc:
                  lVar8 = 1;
                  *(undefined4 *)((long)param_3 + 0x4c) = uVar9;
                  goto LAB_0017cc28;
                }
                if ((param_3[5] == 0) && ((int)*puVar11 == -1)) {
                  lVar8 = FUN_0016f79c(*puVar1 + 0x50);
                  if ((*puVar2 == 0) || (lVar8 != 0)) {
                    if (*puVar2 != 0) {
                      uVar10 = 0;
                      do {
                        local_70 = 0;
                        if (((lVar8 + 8U >> 3 < 0x201) ||
                            (iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,lVar8,&local_70,8), iVar7 != 1))
                           || (local_70 == param_1[1])) goto LAB_0017cc24;
                        uVar10 = uVar10 + 1;
                        lVar8 = lVar8 + 8;
                      } while (uVar10 < *puVar2);
                    }
                    uVar9 = 3;
                    goto LAB_0017cdcc;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
LAB_0017cc24:
  lVar8 = 0;
LAB_0017cc28:
  if (*(long *)(lVar4 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(lVar8);
  }
  return;
}

/* ===== FUN_0017ce78 @ 0017ce78 ===== */

void FUN_0017ce78(void)

{
  long lVar1;
  int iVar2;
  ulong uVar3;
  char local_3c [4];
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  if ((DAT_002839fc == '\x01' && DAT_002843d8 != '\0') && (DAT_002843d0 == DAT_001a7d30)) {
    uVar3 = DAT_00284220;
    if ((DAT_00284220 == 0) || (uVar3 = FUN_0016d5cc(), (int)uVar3 == 0)) goto LAB_0017cefc;
    local_3c[0] = -1;
    if (0x1000 < DAT_00284220 + 0x49) {
      iVar2 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_00284220 + 0x48,local_3c,1);
      uVar3 = 0;
      if ((((iVar2 != 1) || (local_3c[0] != '\x01')) || (uVar3 = DAT_002842f8, DAT_002842f8 == 0))
         || (uVar3 = FUN_0016d5cc(DAT_002842f8,DAT_001a7d30), (int)uVar3 == 0)) goto LAB_0017cefc;
      local_3c[0] = -1;
      if (0x1000 < DAT_002842f8 + 0x49) {
        iVar2 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_002842f8 + 0x48,local_3c,1);
        uVar3 = (ulong)(iVar2 == 1 && local_3c[0] == '\x01');
        goto LAB_0017cefc;
      }
    }
  }
  uVar3 = 0;
LAB_0017cefc:
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar3);
}

/* ===== FUN_0017cfbc @ 0017cfbc ===== */

void FUN_0017cfbc(void)

{
  int iVar1;
  
  DAT_002844d0 = 0;
  DAT_002844c0 = 0;
  if (((DAT_002844c8 != 0) && (DAT_002843e0 != 0)) &&
     (iVar1 = FUN_0016d5cc(DAT_002843e0,DAT_001a7d30), iVar1 != 0)) {
    FUN_0016cf5c(0xc61c3c00,0xc61c3c00,DAT_002843e0);
    return;
  }
  return;
}

/* ===== FUN_0017d6b0 @ 0017d6b0 ===== */

void FUN_0017d6b0(float param_1,float param_2,long param_3,char *param_4)

{
  ulong uVar1;
  ulong uVar2;
  long lVar3;
  bool bVar4;
  ulong uVar5;
  bool bVar6;
  int iVar7;
  size_t sVar8;
  ulong uVar9;
  uint uVar10;
  undefined8 local_88;
  long local_78;
  
  lVar3 = tpidr_el0;
  local_78 = *(long *)(lVar3 + 0x28);
  iVar7 = FUN_0017df44(*(undefined8 *)(param_3 + 8));
  if (iVar7 != 0) {
    uVar2 = *(ulong *)(param_3 + 8);
    uVar9 = *(ulong *)(param_3 + 0x10);
    if (uVar9 != 0) {
      uVar10 = 0;
      do {
        if ((uVar9 == uVar2) || (0xf < uVar10)) goto LAB_0017d798;
        local_88 = 0;
        if (uVar9 + 0x40 >> 3 < 0x201) {
          bVar6 = false;
        }
        else {
          iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar9 + 0x38,&local_88,8);
          bVar6 = iVar7 == 1;
        }
        uVar5 = local_88;
        bVar4 = false;
        if (0xfff < local_88) {
          bVar4 = bVar6;
        }
        bVar4 = (bool)(bVar4 & (local_88 & 7) == 0);
        uVar1 = local_88;
        if (!bVar4) {
          uVar1 = 0;
        }
        iVar7 = FUN_0016d5cc(uVar9,uVar1);
        if (iVar7 == 0) goto LAB_0017d88c;
        uVar10 = uVar10 + 1;
        uVar9 = uVar5;
      } while (bVar4);
      uVar9 = 0;
    }
LAB_0017d798:
    if ((uVar9 == uVar2) && (sVar8 = strlen(param_4), sVar8 < 0x80)) {
      iVar7 = strcmp((char *)(param_3 + 0x18),param_4);
      if (iVar7 != 0) {
        FUN_0016ced8(&local_88,param_4);
        FUN_0016ceac(*(undefined8 *)(param_3 + 0x10),&local_88);
        (*(code *)(DAT_001a7cd0 + 0x66ad48))(&local_88);
        FUN_00176a24((char *)(param_3 + 0x18),0xffffffffffffffff,0x80,&DAT_001343dc,param_4);
        FUN_0016ceec(*(undefined8 *)(param_3 + 0x10),0xffffffff);
      }
      FUN_0016cf5c(0,0,*(undefined8 *)(param_3 + 0x10));
      FUN_0016cf5c(0,0,*(undefined8 *)(param_3 + 8));
      FUN_0016cf84(*(undefined8 *)(param_3 + 0x10),&local_88);
      if ((ABS((float)local_88) != INFINITY) && (!NAN(ABS((float)local_88)))) {
        if ((ABS(local_88._4_4_) != INFINITY) && (!NAN(ABS(local_88._4_4_)))) {
          FUN_0016cf5c(param_1 - (float)local_88,param_2 - local_88._4_4_,
                       *(undefined8 *)(param_3 + 0x10));
        }
      }
    }
  }
LAB_0017d88c:
  if (*(long *)(lVar3 + 0x28) == local_78) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0017df44 @ 0017df44 ===== */

void FUN_0017df44(ulong param_1)

{
  ulong uVar1;
  long lVar2;
  bool bVar3;
  ulong uVar4;
  ulong uVar5;
  bool bVar6;
  int iVar7;
  uint uVar8;
  uint uVar9;
  ulong local_60;
  long local_58;
  
  lVar2 = tpidr_el0;
  local_58 = *(long *)(lVar2 + 0x28);
  if (param_1 != 0) {
    local_60 = 0;
    if ((((param_1 + 0x30 < 0x1000) || ((param_1 & 0xfffffffffffffff8) == 0xffffffffffffffc8)) ||
        (iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1 + 0x30,&local_60,8), uVar4 = DAT_00281b10,
        iVar7 != 1)) || ((local_60 != 0 && (local_60 != DAT_001a7d28)))) {
      param_1 = 0;
    }
    else {
      uVar9 = 0;
      do {
        uVar8 = 1;
        if ((param_1 == uVar4) || (0xf < uVar9)) goto LAB_0017e0a0;
        local_60 = 0;
        if (param_1 + 0x40 >> 3 < 0x201) {
          bVar6 = false;
        }
        else {
          iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1 + 0x38,&local_60,8);
          bVar6 = iVar7 == 1;
        }
        uVar5 = local_60;
        bVar3 = false;
        if (0xfff < local_60) {
          bVar3 = bVar6;
        }
        bVar3 = (bool)(bVar3 & (local_60 & 7) == 0);
        uVar1 = local_60;
        if (!bVar3) {
          uVar1 = 0;
        }
        iVar7 = FUN_0016d5cc(param_1,uVar1);
        if (iVar7 == 0) {
          uVar8 = 0;
          goto LAB_0017e0a0;
        }
        uVar9 = uVar9 + 1;
        param_1 = uVar5;
      } while (bVar3);
      param_1 = 0;
      uVar8 = 1;
LAB_0017e0a0:
      uVar9 = 0;
      if (param_1 == uVar4) {
        uVar9 = uVar8;
      }
      param_1 = (ulong)uVar9;
    }
  }
  if (*(long *)(lVar2 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(param_1);
}

/* ===== FUN_0017e998 @ 0017e998 ===== */

void FUN_0017e998(float param_1,float param_2,float param_3,long param_4,char *param_5)

{
  ulong uVar1;
  ulong uVar2;
  long lVar3;
  bool bVar4;
  ulong uVar5;
  bool bVar6;
  int iVar7;
  size_t sVar8;
  ulong uVar9;
  uint uVar10;
  short local_8c [2];
  undefined8 local_88;
  long local_78;
  
  lVar3 = tpidr_el0;
  local_78 = *(long *)(lVar3 + 0x28);
  iVar7 = FUN_00179dfc(*(undefined8 *)(param_4 + 8));
  if (iVar7 != 0) {
    uVar2 = *(ulong *)(param_4 + 8);
    uVar9 = *(ulong *)(param_4 + 0x10);
    if (uVar9 != 0) {
      uVar10 = 0;
      do {
        if ((uVar9 == uVar2) || (0xf < uVar10)) goto LAB_0017ea88;
        local_88 = 0;
        if (uVar9 + 0x40 >> 3 < 0x201) {
          bVar6 = false;
        }
        else {
          iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar9 + 0x38,&local_88,8);
          bVar6 = iVar7 == 1;
        }
        uVar5 = local_88;
        bVar4 = false;
        if (0xfff < local_88) {
          bVar4 = bVar6;
        }
        bVar4 = (bool)(bVar4 & (local_88 & 7) == 0);
        uVar1 = local_88;
        if (!bVar4) {
          uVar1 = 0;
        }
        iVar7 = FUN_0016d5cc(uVar9,uVar1);
        if (iVar7 == 0) goto LAB_0017eaa0;
        uVar10 = uVar10 + 1;
        uVar9 = uVar5;
      } while (bVar4);
      uVar9 = 0;
    }
LAB_0017ea88:
    if ((uVar9 == uVar2) && (sVar8 = strlen(param_5), sVar8 < 0x140)) {
      iVar7 = strcmp(param_5,(char *)(param_4 + 0x18));
      if (iVar7 != 0) {
        FUN_0016ced8(&local_88,param_5);
        FUN_0016ceac(*(undefined8 *)(param_4 + 0x10),&local_88);
        (*(code *)(DAT_001a7cd0 + 0x66ad48))(&local_88);
        FUN_00176a24((char *)(param_4 + 0x18),0xffffffffffffffff,0x140,&DAT_001343dc,param_5);
        FUN_0016ceec(*(undefined8 *)(param_4 + 0x10),0xffffffff);
      }
      local_8c[0] = 0;
      uVar2 = *(ulong *)(param_4 + 0x10) + 0xb0;
      if ((((0xfff < uVar2) &&
           ((*(ulong *)(param_4 + 0x10) & 0xfffffffffffffffe) != 0xffffffffffffff4e)) &&
          (iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar2,local_8c,2), iVar7 == 1)) &&
         ((0 < local_8c[0] && (local_8c[0] < 0x101)))) {
        param_3 = param_3 / (float)(int)local_8c[0];
        FUN_0016cf00(param_3,param_3,*(undefined8 *)(param_4 + 0x10));
        FUN_0016cf5c(0,0,*(undefined8 *)(param_4 + 0x10));
        FUN_0016cf84(*(undefined8 *)(param_4 + 0x10),&local_88);
        if ((ABS((float)local_88) != INFINITY) && (!NAN(ABS((float)local_88)))) {
          if ((ABS(local_88._4_4_) != INFINITY) && (!NAN(ABS(local_88._4_4_)))) {
            FUN_0016cf5c(param_1 - (float)local_88,param_2 - local_88._4_4_,
                         *(undefined8 *)(param_4 + 0x10));
            FUN_0016cf5c(0,0,*(undefined8 *)(param_4 + 8));
          }
        }
      }
    }
  }
LAB_0017eaa0:
  if (*(long *)(lVar3 + 0x28) == local_78) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0017ec28 @ 0017ec28 ===== */

void FUN_0017ec28(void)

{
  ulong uVar1;
  ulong uVar2;
  ulong uVar3;
  long lVar4;
  bool bVar5;
  bool bVar6;
  bool bVar7;
  int iVar8;
  ulong uVar9;
  byte bVar10;
  ulong uVar11;
  uint uVar12;
  int local_74;
  ulong local_70;
  long local_68;
  
  lVar4 = tpidr_el0;
  local_68 = *(long *)(lVar4 + 0x28);
  local_70 = 0;
  if (DAT_001a7cd0 + 0x1307e28U >> 3 < 0x201) {
    bVar7 = false;
  }
  else {
    iVar8 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_001a7cd0 + 0x1307e20,&local_70,8);
    bVar7 = iVar8 == 1;
  }
  bVar5 = false;
  if (0xfff < local_70) {
    bVar5 = bVar7;
  }
  bVar5 = (bool)(bVar5 & (local_70 & 7) == 0);
  uVar11 = local_70;
  if (!bVar5) {
    uVar11 = 0;
  }
  local_74 = -1;
  uVar9 = 0;
  if (bVar5) {
    uVar9 = 0;
    if ((0xfff < uVar11 + 0x50) && ((uVar11 & 0xfffffffffffffffc) != 0xffffffffffffffac)) {
      iVar8 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar11 + 0x50,&local_74,4);
      uVar9 = 0;
      if ((iVar8 == 1) && (local_74 == 4)) {
        local_70 = 0;
        iVar8 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar11 + 0x48,&local_70,8);
        uVar11 = local_70;
        local_70 = 0;
        if (((uVar11 & 7) != 0 || uVar11 < 0x1000) || iVar8 != 1) {
          uVar11 = 0;
        }
        if (uVar11 + 8 >> 3 < 0x201) {
          bVar7 = false;
        }
        else {
          iVar8 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar11,&local_70,8);
          bVar7 = iVar8 == 1;
        }
        bVar5 = false;
        if (0xfff < local_70) {
          bVar5 = bVar7;
        }
        uVar1 = local_70;
        if (!(bool)(bVar5 & (local_70 & 7) == 0)) {
          uVar1 = 0;
        }
        uVar9 = 0;
        if (uVar1 == DAT_001a7cd0 + 0x1214018U) {
          local_70 = 0;
          if (uVar11 + 0x30 >> 3 < 0x201) {
            bVar7 = false;
            uVar11 = 0;
          }
          else {
            iVar8 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar11 + 0x28,&local_70,8);
            bVar7 = iVar8 == 1;
            uVar11 = local_70;
          }
          local_70 = 0;
          bVar5 = false;
          if (0xfff < uVar11) {
            bVar5 = bVar7;
          }
          if (!(bool)(bVar5 & (uVar11 & 7) == 0)) {
            uVar11 = 0;
          }
          if (uVar11 + 8 >> 3 < 0x201) {
            bVar7 = false;
          }
          else {
            iVar8 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar11,&local_70,8);
            bVar7 = iVar8 == 1;
          }
          bVar5 = false;
          if (0xfff < local_70) {
            bVar5 = bVar7;
          }
          uVar1 = local_70;
          if (!(bool)(bVar5 & (local_70 & 7) == 0)) {
            uVar1 = 0;
          }
          uVar9 = 0;
          if (uVar1 == DAT_001a7cd0 + 0x11f1a20U) {
            local_70 = 0;
            if (uVar11 + 0x950 >> 3 < 0x201) {
              bVar7 = false;
              uVar11 = 0;
            }
            else {
              iVar8 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar11 + 0x948,&local_70,8);
              bVar7 = iVar8 == 1;
              uVar11 = local_70;
            }
            local_70 = 0;
            bVar5 = false;
            if (0xfff < uVar11) {
              bVar5 = bVar7;
            }
            bVar5 = (bool)(bVar5 & (uVar11 & 7) == 0);
            uVar1 = uVar11;
            if (!bVar5) {
              uVar1 = 0;
            }
            if (uVar1 + 8 >> 3 < 0x201) {
              bVar7 = false;
            }
            else {
              iVar8 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar1,&local_70,8);
              bVar7 = iVar8 == 1;
            }
            uVar3 = DAT_001a7d30;
            bVar6 = false;
            if (0xfff < local_70) {
              bVar6 = bVar7;
            }
            uVar2 = local_70;
            if (!(bool)(bVar6 & (local_70 & 7) == 0)) {
              uVar2 = 0;
            }
            uVar9 = 0;
            if (uVar2 == DAT_001a7cd0 + 0x120abe0U) {
              if (bVar5) {
                uVar12 = 0;
                do {
                  if ((uVar11 == uVar3) || (0xf < uVar12)) goto LAB_0017f01c;
                  local_70 = 0;
                  if (uVar11 + 0x40 >> 3 < 0x201) {
                    bVar7 = false;
                  }
                  else {
                    iVar8 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar11 + 0x38,&local_70,8);
                    bVar7 = iVar8 == 1;
                  }
                  uVar9 = local_70;
                  bVar5 = false;
                  if (0xfff < local_70) {
                    bVar5 = bVar7;
                  }
                  bVar5 = (bool)(bVar5 & (local_70 & 7) == 0);
                  uVar2 = local_70;
                  if (!bVar5) {
                    uVar2 = 0;
                  }
                  iVar8 = FUN_0016d5cc(uVar11,uVar2);
                  if (iVar8 == 0) goto LAB_0017f044;
                  uVar12 = uVar12 + 1;
                  uVar11 = uVar9;
                } while (bVar5);
              }
              uVar11 = 0;
LAB_0017f01c:
              if (uVar11 == uVar3) {
                local_70 = 0;
                if (uVar1 + 0x98 >> 3 < 0x201) {
                  bVar7 = false;
                  uVar11 = 0;
                }
                else {
                  iVar8 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar1 + 0x90,&local_70,8);
                  bVar7 = iVar8 == 1;
                  uVar11 = local_70;
                }
                local_70 = 0;
                bVar5 = false;
                if (0xfff < uVar11) {
                  bVar5 = bVar7;
                }
                bVar5 = (bool)(bVar5 & (uVar11 & 7) == 0);
                uVar3 = uVar11;
                if (!bVar5) {
                  uVar3 = 0;
                }
                if (uVar3 + 8 >> 3 < 0x201) {
                  bVar7 = false;
                }
                else {
                  iVar8 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar3,&local_70,8);
                  bVar7 = iVar8 == 1;
                }
                bVar6 = false;
                if (0xfff < local_70) {
                  bVar6 = bVar7;
                }
                uVar2 = local_70;
                if (!(bool)(bVar6 & (local_70 & 7) == 0)) {
                  uVar2 = 0;
                }
                uVar9 = 0;
                if (uVar2 == DAT_001a7cd0 + 0x11ad208U) {
                  if (bVar5) {
                    uVar12 = 0;
                    do {
                      bVar10 = 1;
                      if ((uVar11 == uVar1) || (0xf < uVar12)) goto LAB_0017f1a0;
                      local_70 = 0;
                      if (uVar11 + 0x40 >> 3 < 0x201) {
                        bVar7 = false;
                      }
                      else {
                        iVar8 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar11 + 0x38,&local_70,8);
                        bVar7 = iVar8 == 1;
                      }
                      uVar9 = local_70;
                      bVar5 = false;
                      if (0xfff < local_70) {
                        bVar5 = bVar7;
                      }
                      bVar5 = (bool)(bVar5 & (local_70 & 7) == 0);
                      uVar2 = local_70;
                      if (!bVar5) {
                        uVar2 = 0;
                      }
                      iVar8 = FUN_0016d5cc(uVar11,uVar2);
                      if (iVar8 == 0) {
                        bVar10 = 0;
                        goto LAB_0017f1a0;
                      }
                      uVar12 = uVar12 + 1;
                      uVar11 = uVar9;
                    } while (bVar5);
                  }
                  uVar11 = 0;
                  bVar10 = 1;
LAB_0017f1a0:
                  uVar9 = uVar3;
                  if (!(bool)(bVar10 & uVar11 == uVar1)) {
                    uVar9 = 0;
                  }
                }
              }
              else {
LAB_0017f044:
                uVar9 = 0;
              }
            }
          }
        }
      }
    }
  }
  if (*(long *)(lVar4 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar9);
}

/* ===== FUN_0017f1e4 @ 0017f1e4 ===== */

void FUN_0017f1e4(undefined8 param_1,int param_2,char *param_3,int param_4)

{
  long lVar1;
  char *pcVar2;
  char *pcVar3;
  undefined1 auStack_cc [48];
  undefined1 auStack_9c [100];
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  if (param_2 < 0) {
    FUN_00176a24(auStack_9c,100,100,"not measured");
  }
  else {
    pcVar2 = "";
    pcVar3 = pcVar2;
    if ((param_3 != (char *)0x0) && (pcVar3 = param_3, *param_3 != '\0')) {
      pcVar2 = " / ";
    }
    FUN_00176a24(auStack_9c,100,100,"%d ms%s%.63s",param_2,pcVar2,pcVar3);
  }
  if (param_4 < 0) {
    FUN_00176a24(auStack_cc,0x30,0x30,&DAT_00132b21);
  }
  else {
    FUN_00176a24(auStack_cc,0x30,0x30,&DAT_0013c694,param_4);
  }
  FUN_00176a24(param_1,0xffffffffffffffff,0x140,&DAT_0013aa7b,auStack_9c,auStack_cc);
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0017fc3c @ 0017fc3c ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0017fc3c(float param_1,float param_2,long param_3,undefined8 param_4,undefined4 param_5,
                 uint param_6)

{
  long *plVar1;
  long lVar2;
  int iVar3;
  undefined4 *puVar4;
  undefined8 uVar5;
  code *pcVar6;
  undefined4 uVar7;
  long lVar8;
  char *pcVar9;
  float fVar10;
  undefined8 local_60;
  long local_58;
  
  lVar2 = tpidr_el0;
  local_58 = *(long *)(lVar2 + 0x28);
  local_60 = 0;
  if (((((1 < param_6) || (ABS(param_1) == INFINITY)) || (NAN(ABS(param_1)))) ||
      ((param_2 != 1e+06 && param_2 < 1e+06 == NAN(param_2) || (param_2 < -1e+06)))) ||
     ((param_1 != 1e+06 && param_1 < 1e+06 == NAN(param_1) || (param_1 < -1e+06)))) {
LAB_0017fcfc:
    puVar4 = (undefined4 *)__errno();
    uVar7 = *puVar4;
    if ((param_3 != 0) && (pcVar6 = *(code **)(param_3 + 0x78), pcVar6 != (code *)0x0)) {
      uVar5 = *(undefined8 *)(param_3 + 8);
      pcVar9 = "clip_move_arguments";
LAB_0017fd20:
      (*pcVar6)(uVar5,pcVar9);
    }
LAB_0017fd24:
    uVar5 = 0;
  }
  else {
    fVar10 = ABS(param_2);
    if ((fVar10 >= INFINITY) && (fVar10 == INFINITY || fVar10 < INFINITY != NAN(fVar10)))
    goto LAB_0017fcfc;
    iVar3 = FUN_00180d80(param_3,&DAT_0022d4e0,param_4,param_5);
    if (iVar3 == 0) {
      puVar4 = (undefined4 *)__errno();
      uVar7 = *puVar4;
      if ((param_3 != 0) && (pcVar6 = *(code **)(param_3 + 0x78), pcVar6 != (code *)0x0)) {
        uVar5 = *(undefined8 *)(param_3 + 8);
        pcVar9 = "clip_move_precondition";
        goto LAB_0017fd20;
      }
      goto LAB_0017fd24;
    }
    plVar1 = &DAT_0022d4e0;
    if (param_6 != 0) {
      plVar1 = &DAT_0022d4f0;
    }
    lVar8 = *plVar1;
    (**(code **)(param_3 + 0x60))((int)param_1,(int)param_2,*(undefined8 *)(param_3 + 8),lVar8);
    if ((lVar8 + 0x28U >> 3 < 0x201) ||
       (iVar3 = (**(code **)(param_3 + 0x10))(*(undefined8 *)(param_3 + 8),lVar8 + 0x20,&local_60,8)
       , iVar3 != 1)) {
      pcVar9 = "clip_move_readback";
    }
    else {
      pcVar9 = "clip_move_position";
      if (((float)local_60 == (float)(int)param_1) && (local_60._4_4_ == (float)(int)param_2)) {
        iVar3 = FUN_00180d80(param_3,&DAT_0022d4e0,param_4,param_5);
        if (iVar3 != 0) {
          uVar5 = 1;
          goto LAB_0017fd2c;
        }
        pcVar9 = "clip_move_postcondition";
      }
    }
    _DAT_0022d510 = DAT_0010f740;
    puVar4 = (undefined4 *)__errno();
    uVar7 = *puVar4;
    if (*(code **)(param_3 + 0x78) != (code *)0x0) {
      (**(code **)(param_3 + 0x78))(*(undefined8 *)(param_3 + 8),pcVar9);
    }
    uVar5 = 0xffffffff;
  }
  *puVar4 = uVar7;
LAB_0017fd2c:
  if (*(long *)(lVar2 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar5);
}

/* ===== FUN_0017fe9c @ 0017fe9c ===== */

bool FUN_0017fe9c(undefined8 param_1,ulong param_2,undefined8 param_3,ulong param_4)

{
  bool bVar1;
  int iVar2;
  
  bVar1 = false;
  if ((0xfff < param_2) && (!CARRY8(param_2,param_4))) {
    iVar2 = (*DAT_001a7ce8)(DAT_001a7cc8);
    bVar1 = iVar2 == 1;
  }
  return bVar1;
}

/* ===== FUN_0017fee0 @ 0017fee0 ===== */

void FUN_0017fee0(undefined8 param_1,void *param_2,void *param_3,size_t param_4)

{
  ulong uVar1;
  long lVar2;
  bool bVar3;
  long lVar4;
  bool bVar5;
  ulong local_50;
  long local_48;
  
  lVar4 = DAT_0022d4e0;
  lVar2 = tpidr_el0;
  local_48 = *(long *)(lVar2 + 0x28);
  if (DAT_0022d4e0 != 0) {
    local_50 = 0;
    if (DAT_0022d4e0 + 8U >> 3 < 0x201) {
      bVar5 = false;
    }
    else {
      param_1 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_0022d4e0,&local_50,8);
      bVar5 = (int)param_1 == 1;
    }
    bVar3 = false;
    if (0xfff < local_50) {
      bVar3 = bVar5;
    }
    uVar1 = local_50;
    if (!(bool)(bVar3 & (local_50 & 7) == 0)) {
      uVar1 = 0;
    }
    if (uVar1 == DAT_001a7cd0 + 0x11c11e8U) {
      if (((((param_4 == 1) && ((void *)(lVar4 + 0xe0) == param_2)) ||
           ((param_4 == 2 && ((void *)(lVar4 + 0xe7) == param_2)))) ||
          ((param_1 = 0, param_4 == 2 && ((void *)(lVar4 + 0x280) == param_2)))) &&
         (param_1 = FUN_00180034(param_1,param_2,param_4), (int)param_1 != 0)) {
        memcpy(param_2,param_3,param_4);
        param_1 = 1;
      }
      goto LAB_00180008;
    }
  }
  param_1 = 0;
LAB_00180008:
  if (*(long *)(lVar2 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(param_1);
}

/* ===== FUN_001802bc @ 001802bc ===== */

void FUN_001802bc(void)

{
  bool bVar1;
  long lVar2;
  long lVar3;
  int iVar4;
  ulong uVar5;
  long lVar6;
  long *plVar7;
  long local_60;
  long local_58;
  
  lVar3 = DAT_001a7cd0;
  lVar2 = tpidr_el0;
  local_58 = *(long *)(lVar2 + 0x28);
  if (DAT_0022d7ec != 0) {
    uVar5 = (ulong)(DAT_0022d7ec == 1);
    goto LAB_00180510;
  }
  if (DAT_001a7cd0 - 1U < 0xfffffffffecfffff) {
    lVar6 = 0;
    do {
      uVar5 = thunk_FUN_0014d168(*(undefined8 *)((long)&DAT_0019c658 + lVar6),
                                 *(undefined8 *)((long)&DAT_0019c660 + lVar6),
                                 *(undefined8 *)
                                  ((long)&PTR_s_34f70e1aa68de7bc005db64b015f2aba_0019c668 + lVar6));
      if ((int)uVar5 == 0) goto LAB_00180508;
      lVar6 = lVar6 + 0x18;
    } while (lVar6 != 0x540);
    local_60 = 0;
    if ((lVar3 + 0x11c11f0U >> 3 < 0x201) ||
       (iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,lVar3 + 0x11c11e8,&local_60,8), iVar4 != 1))
    goto LAB_00180504;
    uVar5 = 0;
    bVar1 = false;
    plVar7 = &DAT_0013fb90;
    do {
      if (local_60 != plVar7[-2] + lVar3) break;
      bVar1 = 0x47 < uVar5;
      if (uVar5 == 0x48) goto LAB_00180404;
      local_60 = 0;
      lVar6 = plVar7[-1] + lVar3 + *plVar7;
      if (lVar6 + 8U >> 3 < 0x201) break;
      iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,lVar6,&local_60,8);
      plVar7 = plVar7 + 3;
      uVar5 = uVar5 + 1;
    } while (iVar4 == 1);
    if (!bVar1) goto LAB_00180504;
LAB_00180404:
    uVar5 = thunk_FUN_0014d168(0x7050a4,0xbc,
                               "34a5074b72bb308ac25ec9ca60db279eb5472b00f936c54d506c99f27879ad7d");
    if ((((((int)uVar5 != 0) &&
          (uVar5 = thunk_FUN_0014d168(0x705160,0x7c,
                                      "3fec2737dd5569528dcbea561b5f857c8419aa34de056e3233869b1b32ee5f0b"
                                     ), (int)uVar5 != 0)) &&
         (uVar5 = thunk_FUN_0014d168(0x7051dc,0x70,
                                     "f9da06b9ce7403e941cdf05394a692d2bf59b300270f04e21f83e851110ea045"
                                    ), (int)uVar5 != 0)) &&
        ((uVar5 = thunk_FUN_0014d168(0x705418,0x90,
                                     "ba39ba731804b47bf09a9369d066567e3dd70764d5c5b47c985ce0c219e3ea75"
                                    ), (int)uVar5 != 0 &&
         (uVar5 = thunk_FUN_0014d168(0x705354,0xc4,
                                     "577309d6feab342db3a5ea28dcd15307b2d2201786e90ac420c9bcba1e70f719"
                                    ), (int)uVar5 != 0)))) &&
       ((uVar5 = thunk_FUN_0014d168(0x705280,0xd4,
                                    "82083116eab2c15c96c8b38209d190631012ad7b2c69185f18a6f0b215835016"
                                   ), (int)uVar5 != 0 &&
        ((uVar5 = thunk_FUN_0014d168(0x705024,0x80,
                                     "bb6ad764d8431c880757955fb510561f51565976e013bd7a7539453a65af4b5b"
                                    ), (int)uVar5 != 0 &&
         (uVar5 = thunk_FUN_0014d168(0x703f54,0x5dc,
                                     "8116689446e3a51512bec03280d3ce90ccd96999212446521c0aeaa1a90084b8"
                                    ), (int)uVar5 != 0)))))) {
      iVar4 = thunk_FUN_0014d168(0x70524c,0x34,
                                 "f8b2086478f23df04d7aff356408627f61f75f068fc0fe0a5b38563cd1ec2dda")
      ;
      uVar5 = (ulong)(iVar4 != 0);
      DAT_0022d7ec = -1;
      if (iVar4 != 0) {
        DAT_0022d7ec = 1;
      }
      goto LAB_00180510;
    }
  }
  else {
LAB_00180504:
    uVar5 = 0;
  }
LAB_00180508:
  DAT_0022d7ec = -1;
LAB_00180510:
  if (*(long *)(lVar2 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar5);
}

/* ===== FUN_00180540 @ 00180540 ===== */

void FUN_00180540(undefined8 param_1,undefined8 param_2)

{
  FUN_0016cdf0(param_2);
  return;
}

/* ===== FUN_00180548 @ 00180548 ===== */

void FUN_00180548(undefined8 param_1,undefined8 param_2,undefined4 param_3)

{
                    /* WARNING: Could not recover jumptable at 0x00180560. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001a7cd0 + 0x88b344))(param_2,param_3);
  return;
}

/* ===== FUN_00180564 @ 00180564 ===== */

void FUN_00180564(undefined8 param_1,undefined8 param_2,undefined4 param_3)

{
                    /* WARNING: Could not recover jumptable at 0x0018057c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001a7cd0 + 0x593f14))(param_2,param_3);
  return;
}

/* ===== FUN_00180580 @ 00180580 ===== */

void FUN_00180580(undefined8 param_1,undefined8 param_2,undefined8 param_3)

{
  FUN_0016cfa4(param_2,param_3);
  return;
}

/* ===== FUN_0018058c @ 0018058c ===== */

void FUN_0018058c(undefined8 param_1,undefined8 param_2)

{
                    /* WARNING: Could not recover jumptable at 0x001805a0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001a7cd0 + 0x59567c))(param_2);
  return;
}

/* ===== FUN_001805a4 @ 001805a4 ===== */

void FUN_001805a4(undefined8 param_1,long param_2)

{
  ulong uVar1;
  long lVar2;
  bool bVar3;
  bool bVar4;
  int iVar5;
  code *pcVar6;
  ulong local_30;
  long local_28;
  
  lVar2 = tpidr_el0;
  local_28 = *(long *)(lVar2 + 0x28);
  if (DAT_0022d4f0 == param_2) {
    local_30 = 0;
    if (param_2 + 8U >> 3 < 0x201) {
      bVar4 = false;
    }
    else {
      iVar5 = (*DAT_001a7ce8)(DAT_001a7cc8,param_2,&local_30,8);
      bVar4 = iVar5 == 1;
    }
    bVar3 = false;
    if (0xfff < local_30) {
      bVar3 = bVar4;
    }
    uVar1 = local_30;
    if (!(bool)(bVar3 & (local_30 & 7) == 0)) {
      uVar1 = 0;
    }
    if (uVar1 != DAT_001a7cd0 + 0x11abcf0U) goto LAB_00180660;
    pcVar6 = (code *)(DAT_001a7cd0 + 0x5940d0);
  }
  else {
LAB_00180660:
    if (DAT_0022d4e0 != param_2) goto LAB_00180700;
    local_30 = 0;
    if (param_2 + 8U >> 3 < 0x201) {
      bVar4 = false;
    }
    else {
      iVar5 = (*DAT_001a7ce8)(DAT_001a7cc8,param_2,&local_30,8);
      bVar4 = iVar5 == 1;
    }
    bVar3 = false;
    if (0xfff < local_30) {
      bVar3 = bVar4;
    }
    uVar1 = local_30;
    if (!(bool)(bVar3 & (local_30 & 7) == 0)) {
      uVar1 = 0;
    }
    if (uVar1 != DAT_001a7cd0 + 0x11c11e8U) goto LAB_00180700;
    pcVar6 = (code *)(DAT_001a7cd0 + 0x88b83c);
  }
  (*pcVar6)(param_2);
LAB_00180700:
  if (*(long *)(lVar2 + 0x28) == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00180724 @ 00180724 ===== */

void FUN_00180724(undefined8 param_1,undefined8 param_2)

{
                    /* WARNING: Could not recover jumptable at 0x00180738. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001a7cd0 + 0x59531c))(param_2);
  return;
}

/* ===== FUN_0018073c @ 0018073c ===== */

void FUN_0018073c(undefined8 param_1,undefined8 param_2,undefined4 param_3)

{
                    /* WARNING: Could not recover jumptable at 0x00180754. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(DAT_001a7cd0 + 0x88d290))(param_2,param_3);
  return;
}

/* ===== FUN_00180758 @ 00180758 ===== */

void FUN_00180758(undefined8 param_1,undefined8 param_2,undefined4 param_3)

{
  FUN_0016ce20(param_2,param_3);
  return;
}

/* ===== FUN_00180764 @ 00180764 ===== */

void FUN_00180764(long *param_1,ulong param_2,long param_3,ulong param_4)

{
  long lVar1;
  bool bVar2;
  int iVar3;
  long local_40;
  long local_38;
  
  lVar1 = tpidr_el0;
  bVar2 = false;
  local_38 = *(long *)(lVar1 + 0x28);
  local_40 = 0;
  if ((((param_2 < 0xfffffffffffffff8) && (param_2 <= ~param_4)) && (0xfff < param_2)) &&
     ((param_2 & 7) == 0)) {
    iVar3 = (*(code *)param_1[2])(param_1[1],param_2,&local_40,8);
    if (iVar3 == 1) {
      bVar2 = local_40 == *param_1 + param_3;
    }
    else {
      bVar2 = false;
    }
  }
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(bVar2);
}

/* ===== FUN_00180814 @ 00180814 ===== */

void FUN_00180814(long param_1,ulong param_2)

{
  long lVar1;
  bool bVar2;
  int iVar3;
  int local_44;
  long local_40;
  long local_38;
  
  lVar1 = tpidr_el0;
  bVar2 = false;
  local_38 = *(long *)(lVar1 + 0x28);
  local_40 = 1;
  local_44 = 0;
  if (0x200 < param_2 + 0x40 >> 3) {
    iVar3 = (**(code **)(param_1 + 0x10))(*(undefined8 *)(param_1 + 8),param_2 + 0x38,&local_40,8);
    bVar2 = false;
    if (((param_2 < 0xffffffffffffffbc) && (iVar3 == 1)) && (local_40 == 0)) {
      iVar3 = (**(code **)(param_1 + 0x10))(*(undefined8 *)(param_1 + 8),param_2 + 0x40,&local_44,4)
      ;
      if (iVar3 == 1) {
        bVar2 = local_44 == -1;
      }
      else {
        bVar2 = false;
      }
    }
  }
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(bVar2);
}

/* ===== FUN_001808ec @ 001808ec ===== */

void FUN_001808ec(long param_1,ulong param_2,ulong param_3)

{
  long lVar1;
  long lVar2;
  bool bVar3;
  int iVar4;
  ushort local_68 [2];
  int local_64;
  ulong local_60;
  ulong local_58;
  ulong local_50;
  long local_48;
  
  lVar2 = tpidr_el0;
  local_48 = *(long *)(lVar2 + 0x28);
  bVar3 = false;
  local_50 = 0;
  local_60 = 0;
  local_58 = 0;
  local_64 = -1;
  local_68[0] = 0;
  if (((0x200 < param_2 + 0x40 >> 3) && (0x20 < param_3 + 0x80 >> 7)) && ((param_3 & 7) == 0)) {
    iVar4 = (**(code **)(param_1 + 0x10))(*(undefined8 *)(param_1 + 8),param_2 + 0x38,&local_50,8);
    bVar3 = false;
    if (((param_2 < 0xffffffffffffffbc) && (iVar4 == 1)) && (local_50 == param_3)) {
      iVar4 = (**(code **)(param_1 + 0x10))(*(undefined8 *)(param_1 + 8),param_2 + 0x40,&local_64,4)
      ;
      bVar3 = false;
      if ((iVar4 == 1) && (-1 < local_64)) {
        iVar4 = (**(code **)(param_1 + 0x10))
                          (*(undefined8 *)(param_1 + 8),param_3 + 0x4e,local_68,2);
        if (iVar4 == 1) {
          bVar3 = false;
          if ((local_68[0] == 0xffff) || ((int)(uint)local_68[0] <= local_64)) goto LAB_00180a8c;
          iVar4 = (**(code **)(param_1 + 0x10))
                            (*(undefined8 *)(param_1 + 8),param_3 + 0x50,&local_58,8);
          if (iVar4 == 1) {
            bVar3 = false;
            if (((local_58 < 0x1000) || ((local_58 & 7) != 0)) ||
               (((ulong)local_68[0] << 3 ^ 0xffffffffffffffff) < local_58)) goto LAB_00180a8c;
            lVar1 = local_58 + (uint)(local_64 << 3);
            if ((0x200 < lVar1 + 8U >> 3) &&
               (iVar4 = (**(code **)(param_1 + 0x10))
                                  (*(undefined8 *)(param_1 + 8),lVar1,&local_60,8), iVar4 == 1)) {
              bVar3 = local_60 == param_2;
              goto LAB_00180a8c;
            }
          }
        }
        bVar3 = false;
      }
    }
  }
LAB_00180a8c:
  if (*(long *)(lVar2 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(bVar3);
}

/* ===== FUN_00180ab8 @ 00180ab8 ===== */

void FUN_00180ab8(long param_1,long param_2,uint param_3)

{
  bool bVar1;
  long lVar2;
  bool bVar3;
  int iVar4;
  uint uVar5;
  ushort local_2c [2];
  long local_28;
  
  lVar2 = tpidr_el0;
  local_28 = *(long *)(lVar2 + 0x28);
  local_2c[0] = 0;
  if (param_2 + 0x50U >> 1 < 0x801) {
    uVar5 = 0;
    bVar3 = false;
  }
  else {
    iVar4 = (**(code **)(param_1 + 0x10))(*(undefined8 *)(param_1 + 8),param_2 + 0x4e,local_2c,2);
    bVar3 = iVar4 == 1;
    uVar5 = (uint)local_2c[0];
  }
  bVar1 = false;
  if (uVar5 == param_3) {
    bVar1 = bVar3;
  }
  if (*(long *)(lVar2 + 0x28) == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(bVar1);
}

/* ===== FUN_00180b44 @ 00180b44 ===== */

void FUN_00180b44(ulong *param_1,ulong *param_2)

{
  ulong uVar1;
  long lVar2;
  int iVar3;
  ulong *puVar4;
  ulong local_28;
  long local_20;
  long local_18;
  
  lVar2 = tpidr_el0;
  local_18 = *(long *)(lVar2 + 0x28);
  local_28 = 0;
  puVar4 = param_1;
  if (param_1 == (ulong *)0x0) goto LAB_00180c08;
  if ((((((*param_1 - 0x1000 < 0xfffffffffecff000) && ((code *)param_1[2] != (code *)0x0)) &&
        (param_1[3] != 0)) &&
       (((param_1[4] != 0 && (param_1[5] != 0)) &&
        ((param_1[6] != 0 && ((param_1[7] != 0 && (param_1[8] != 0)))))))) && (param_1[9] != 0)) &&
     ((((param_1[10] != 0 && (param_1[0xb] != 0)) && (param_1[0xc] != 0)) &&
      (((param_1[0xd] != 0 && (param_1[0xe] != 0)) &&
       ((*(int *)((long)param_2 + 0x34) == 0 && (param_2[4] == *param_1)))))))) {
    uVar1 = *param_2;
    if (uVar1 != param_2[1]) {
      puVar4 = (ulong *)0x0;
      if ((uVar1 == param_2[2]) || (param_2[1] == param_2[2])) goto LAB_00180c08;
      local_20 = 0;
      if ((uVar1 - 0x1000 < 0xffffffffffffed38) &&
         ((((uVar1 & 7) == 0 &&
           (iVar3 = (*(code *)param_1[2])(param_1[1],uVar1,&local_20,8), iVar3 == 1)) &&
          (local_20 == *param_1 + 0x11c11e8)))) {
        puVar4 = (ulong *)FUN_00180764(param_1,param_2[1],0x11c1360,0x80);
        if (((int)puVar4 == 0) ||
           (puVar4 = (ulong *)FUN_00180764(param_1,param_2[2],0x11abcf0,0x80), (int)puVar4 == 0))
        goto LAB_00180c08;
        if ((0x200 < *param_2 + 0xd8 >> 3) &&
           ((iVar3 = (*(code *)param_1[2])(param_1[1],*param_2 + 0xd0,&local_28,8), iVar3 == 1 &&
            (local_28 == param_2[1])))) {
          puVar4 = (ulong *)FUN_00180ab8(param_1,*param_2,1);
          if ((((int)puVar4 != 0) &&
              (puVar4 = (ulong *)FUN_00180ab8(param_1,param_2[1],1), (int)puVar4 != 0)) &&
             (puVar4 = (ulong *)FUN_001808ec(param_1,param_2[1],*param_2), (int)puVar4 != 0)) {
            iVar3 = FUN_001808ec(param_1,param_2[2],param_2[1]);
            puVar4 = (ulong *)(ulong)(iVar3 != 0);
          }
          goto LAB_00180c08;
        }
      }
      puVar4 = (ulong *)0x0;
      goto LAB_00180c08;
    }
  }
  puVar4 = (ulong *)0x0;
LAB_00180c08:
  if (*(long *)(lVar2 + 0x28) != local_18) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(puVar4);
  }
  return;
}

/* ===== FUN_00180d80 @ 00180d80 ===== */

void FUN_00180d80(long param_1,ulong *param_2,ulong param_3,uint param_4)

{
  uint uVar1;
  long lVar2;
  int iVar3;
  ulong uVar4;
  uint uVar5;
  float local_80;
  short local_7c [2];
  short local_78 [2];
  char local_74 [4];
  char local_70 [4];
  undefined2 local_6c;
  char local_6a;
  ulong local_68;
  long lStack_60;
  long local_58;
  undefined8 local_50;
  long local_48;
  
  lVar2 = tpidr_el0;
  local_48 = *(long *)(lVar2 + 0x28);
  if ((param_2 != (ulong *)0x0) && ((int)param_2[6] != 0)) {
    uVar4 = FUN_00180b44(param_1,param_2);
    if ((int)uVar4 == 0) goto LAB_00180e0c;
    local_70[0] = '\0';
    local_6a = '\0';
    local_6c = 0;
    local_74[0] = '\x01';
    local_78[0] = 1;
    local_7c[0] = 1;
    local_50 = 0;
    local_80 = 0.0;
    local_68 = 0;
    lStack_60 = 0;
    local_58 = 0;
    if (((((((0x1000 < *param_2 + 0x49) &&
            (iVar3 = (**(code **)(param_1 + 0x10))
                               (*(undefined8 *)(param_1 + 8),*param_2 + 0x48,&local_6c,1),
            iVar3 == 1)) && ((char)local_6c == '\x01')) &&
          ((0x1000 < param_2[1] + 0x49 &&
           (iVar3 = (**(code **)(param_1 + 0x10))
                              (*(undefined8 *)(param_1 + 8),param_2[1] + 0x48,(ulong)&local_6c | 1,1
                              ), iVar3 == 1)))) && (local_6c._1_1_ == '\x01')) &&
        (((0x1000 < param_2[2] + 0x49 &&
          (iVar3 = (**(code **)(param_1 + 0x10))
                             (*(undefined8 *)(param_1 + 8),param_2[2] + 0x48,(ulong)&local_6c | 2,1)
          , iVar3 == 1)) &&
         ((local_6a == '\x01' &&
          (((0x1000 < *param_2 + 0xe1 &&
            (iVar3 = (**(code **)(param_1 + 0x10))
                               (*(undefined8 *)(param_1 + 8),*param_2 + 0xe0,local_70,1), iVar3 == 1
            )) && (local_70[0] == '\x01')))))))) &&
       ((((0x800 < *param_2 + 0xe9 >> 1 &&
          (iVar3 = (**(code **)(param_1 + 0x10))
                             (*(undefined8 *)(param_1 + 8),*param_2 + 0xe7,local_78,2), iVar3 == 1))
         && ((local_78[0] == 0 &&
             (((((0x800 < *param_2 + 0x282 >> 1 &&
                 (iVar3 = (**(code **)(param_1 + 0x10))
                                    (*(undefined8 *)(param_1 + 8),*param_2 + 0x280,local_7c,2),
                 iVar3 == 1)) &&
                ((local_7c[0] == 0 &&
                 (((0x1000 < *param_2 + 0x27f &&
                   (iVar3 = (**(code **)(param_1 + 0x10))
                                      (*(undefined8 *)(param_1 + 8),*param_2 + 0x27e,local_74,1),
                   iVar3 == 1)) && (local_74[0] == '\0')))))) &&
               ((0x400 < *param_2 + 0x23c >> 2 &&
                (iVar3 = (**(code **)(param_1 + 0x10))
                                   (*(undefined8 *)(param_1 + 8),*param_2 + 0x238,&local_80,4),
                iVar3 == 1)))) && (local_80 == 1.0)))))) &&
        (((0x200 < *param_2 + 200 >> 3 &&
          (iVar3 = (**(code **)(param_1 + 0x10))
                             (*(undefined8 *)(param_1 + 8),*param_2 + 0xc0,&local_50,8), iVar3 == 1)
          ) && (((float)local_50 == *(float *)(param_2 + 5) &&
                (((local_50._4_4_ == *(float *)((long)param_2 + 0x2c) &&
                  (0x202 < param_2[1] + 0x28 >> 3)) &&
                 (iVar3 = (**(code **)(param_1 + 0x10))
                                    (*(undefined8 *)(param_1 + 8),param_2[1] + 0x10,&local_68,0x18),
                 iVar3 == 1)))))))))) {
      if ((local_68 == 0x3f800000 && lStack_60 == 0x3f80000000000000) && local_58 == 0) {
        uVar4 = *param_2;
        if (param_2[3] == 0) {
          local_50 = local_50 & 0xffffffff00000000;
          local_68 = 1;
          if (((uVar4 + 0x40 >> 3 < 0x201) ||
              (iVar3 = (**(code **)(param_1 + 0x10))
                                 (*(undefined8 *)(param_1 + 8),uVar4 + 0x38,&local_68,8),
              0xffffffffffffffbb < uVar4)) ||
             (((iVar3 != 1 ||
               ((local_68 != 0 ||
                (iVar3 = (**(code **)(param_1 + 0x10))
                                   (*(undefined8 *)(param_1 + 8),uVar4 + 0x40,&local_50,4),
                iVar3 != 1)))) || ((float)local_50 != -NAN)))) goto LAB_00181090;
        }
        else {
          uVar4 = FUN_001808ec(param_1,uVar4);
          if ((int)uVar4 == 0) goto LAB_00180e0c;
        }
        local_68 = local_68 & 0xffffffffffff0000;
        if (0x800 < param_2[2] + 0x50 >> 1) {
          iVar3 = (**(code **)(param_1 + 0x10))
                            (*(undefined8 *)(param_1 + 8),param_2[2] + 0x4e,&local_68,2);
          uVar4 = 0;
          if ((iVar3 != 1) || ((ushort)local_68 != param_4)) goto LAB_00180e0c;
          local_68 = 0;
          if (0x200 < *param_2 + 0x38 >> 3) {
            iVar3 = (**(code **)(param_1 + 0x10))
                              (*(undefined8 *)(param_1 + 8),*param_2 + 0x30,&local_68,8);
            uVar4 = 0;
            if ((iVar3 != 1) || (local_68 != param_3)) goto LAB_00180e0c;
            local_68 = 0;
            if (0x200 < param_2[1] + 0x38 >> 3) {
              iVar3 = (**(code **)(param_1 + 0x10))
                                (*(undefined8 *)(param_1 + 8),param_2[1] + 0x30,&local_68,8);
              uVar4 = 0;
              if ((iVar3 == 1) && (local_68 == param_3)) {
                local_68 = 0;
                if (param_2[2] + 0x38 >> 3 < 0x201) {
                  uVar5 = 0;
                }
                else {
                  iVar3 = (**(code **)(param_1 + 0x10))
                                    (*(undefined8 *)(param_1 + 8),param_2[2] + 0x30,&local_68,8);
                  uVar5 = (uint)(iVar3 == 1);
                }
                uVar1 = 0;
                if (local_68 == param_3) {
                  uVar1 = uVar5;
                }
                uVar4 = (ulong)uVar1;
              }
              goto LAB_00180e0c;
            }
          }
        }
      }
LAB_00181090:
      uVar4 = 0;
      goto LAB_00180e0c;
    }
  }
  uVar4 = 0;
LAB_00180e0c:
  if (*(long *)(lVar2 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar4);
}

/* ===== FUN_00181264 @ 00181264 ===== */

void FUN_00181264(long param_1,ulong param_2,int param_3,uint param_4)

{
  long lVar1;
  bool bVar2;
  bool bVar3;
  int iVar4;
  ulong uVar5;
  ushort local_44 [2];
  ulong local_40;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  uVar5 = 0;
  local_44[0] = 0;
  if ((param_3 == 0) || (DAT_001a7d30 != param_1)) goto LAB_00181390;
  local_40 = 0;
  if (param_2 + 8 >> 3 < 0x201) {
    bVar3 = false;
  }
  else {
    iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,param_2,&local_40,8);
    bVar3 = iVar4 == 1;
  }
  bVar2 = false;
  if (0xfff < local_40) {
    bVar2 = bVar3;
  }
  uVar5 = local_40;
  if (!(bool)(bVar2 & (local_40 & 7) == 0)) {
    uVar5 = 0;
  }
  if (uVar5 == DAT_001a7cd0 + 0x11abcf0U) {
    uVar5 = FUN_0016d5cc(param_2,DAT_001a7d30);
    if ((int)uVar5 == 0) goto LAB_00181390;
    uVar5 = 0;
    if ((param_2 + 0x4e < 0x1000) || ((param_2 & 0xfffffffffffffffe) == 0xffffffffffffffb0))
    goto LAB_00181390;
    iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,param_2 + 0x4e,local_44,2);
    if (iVar4 == 1) {
      uVar5 = (ulong)(local_44[0] == param_4);
      goto LAB_00181390;
    }
  }
  uVar5 = 0;
LAB_00181390:
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar5);
}

/* ===== FUN_001813b8 @ 001813b8 ===== */

void FUN_001813b8(undefined8 *param_1,ulong *param_2,long param_3,int param_4,long param_5)

{
  long *plVar1;
  uint uVar2;
  long lVar3;
  bool bVar4;
  bool bVar5;
  int iVar6;
  long lVar7;
  undefined8 uVar8;
  ulong uVar9;
  ulong uVar10;
  undefined8 *puVar11;
  ulong *puVar12;
  ulong uVar13;
  ulong local_b8 [10];
  long local_68;
  
  lVar3 = tpidr_el0;
  local_68 = *(long *)(lVar3 + 0x28);
  if ((param_2 != (ulong *)0x0) && (uVar13 = *param_2, uVar13 != 0)) {
    uVar2 = *(uint *)(param_1 + 0x180);
    uVar9 = (ulong)uVar2;
    lVar7 = 0;
    if ((0x5f < uVar2) ||
       ((((uVar13 == DAT_001a7d28 || (uVar13 == DAT_001a7d30)) ||
         (lVar7 = 0, uVar13 == param_1[0x181])) ||
        ((uVar13 == DAT_001a7d38 || (uVar13 == DAT_00281a50)))))) goto LAB_001815dc;
    uVar10 = (ulong)DAT_001a7d18;
    if (DAT_001a7d18 != 0) {
      puVar12 = &DAT_001c3568;
      do {
        if (((uVar13 == *puVar12) || (uVar13 == puVar12[1])) || (uVar13 == puVar12[3]))
        goto LAB_001815d8;
        puVar12 = puVar12 + 0x1b;
        uVar10 = uVar10 - 1;
      } while (uVar10 != 0);
    }
    puVar11 = param_1;
    if (uVar2 != 0) {
      do {
        if (uVar13 == *(ulong *)*puVar11) goto LAB_001815d8;
        uVar9 = uVar9 - 1;
        puVar11 = puVar11 + 4;
      } while (uVar9 != 0);
    }
    lVar7 = 0x11c0a48;
    if (param_4 != 1) {
      if (param_4 == 2) {
        lVar7 = 0x11c0c58;
      }
      else {
        lVar7 = 0x11abcf0;
        if (param_4 != 3) {
          lVar7 = 0x11ad208;
        }
      }
    }
    local_b8[0] = 0;
    if (uVar13 + 8 >> 3 < 0x201) {
      bVar5 = false;
    }
    else {
      iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar13,local_b8,8);
      bVar5 = iVar6 == 1;
    }
    bVar4 = false;
    if (0xfff < local_b8[0]) {
      bVar4 = bVar5;
    }
    uVar9 = local_b8[0];
    if (!(bool)(bVar4 & (local_b8[0] & 7) == 0)) {
      uVar9 = 0;
    }
    if (uVar9 == DAT_001a7cd0 + lVar7) {
      local_b8[0] = 0;
      if ((((0xfff < uVar13 + 0x30) && ((uVar13 & 0xfffffffffffffff8) != 0xffffffffffffffc8)) &&
          (iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar13 + 0x30,local_b8,8), iVar6 == 1)) &&
         ((local_b8[0] == 0 || (local_b8[0] == DAT_001a7d28)))) {
        if (param_4 == 4) {
          lVar7 = FUN_0016ddc8();
          iVar6 = (int)lVar7;
        }
        else {
          lVar7 = FUN_0016d5cc(uVar13,param_3);
          iVar6 = (int)lVar7;
        }
        if ((iVar6 == 0) ||
           ((param_5 != 0 &&
            ((lVar7 = FUN_0016debc(param_5,0x11abad8), (int)lVar7 == 0 ||
             (lVar7 = FUN_0016d800(param_5,uVar13), (int)lVar7 == 0)))))) goto LAB_001815dc;
        if (param_4 == 1) {
          lVar7 = FUN_00177620(uVar13,param_3,local_b8);
          iVar6 = (int)lVar7;
joined_r0x00181728:
          if (iVar6 == 0) goto LAB_001815dc;
        }
        else if (param_4 == 2) {
          uVar9 = FUN_0016f79c(uVar13 + 0x160);
          uVar10 = FUN_0016f79c(uVar13 + 0x168);
          lVar7 = FUN_0016f79c(uVar13 + 0x128);
          if (lVar7 == 0) goto LAB_001815dc;
          iVar6 = FUN_0016d800(lVar7,uVar13);
          lVar7 = 0;
          if ((((iVar6 == 0) || (uVar9 == 0)) || (uVar10 < uVar9)) ||
             ((lVar7 = 0, 0x20 < uVar10 - uVar9 || ((uVar10 - uVar9 & 7) != 0)))) goto LAB_001815dc;
          if (uVar9 < uVar10) {
            do {
              uVar8 = FUN_0016f79c(uVar9);
              lVar7 = FUN_0016debc(uVar8,0x11ad208);
              if (((int)lVar7 == 0) || (lVar7 = FUN_0016d800(uVar8,uVar13), (int)lVar7 == 0))
              goto LAB_001815dc;
              uVar9 = uVar9 + 8;
            } while (uVar9 < uVar10);
            goto LAB_00181718;
          }
        }
        else {
LAB_00181718:
          if (param_4 == 4) {
            lVar7 = FUN_00181790(uVar13);
            iVar6 = (int)lVar7;
            goto joined_r0x00181728;
          }
        }
        lVar7 = 1;
        plVar1 = param_1 + (ulong)*(uint *)(param_1 + 0x180) * 4;
        *(uint *)(param_1 + 0x180) = *(uint *)(param_1 + 0x180) + 1;
        *plVar1 = (long)param_2;
        plVar1[1] = param_3;
        *(int *)(plVar1 + 2) = param_4;
        plVar1[3] = param_5;
        goto LAB_001815dc;
      }
    }
  }
LAB_001815d8:
  lVar7 = 0;
LAB_001815dc:
  if (*(long *)(lVar3 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(lVar7);
}

/* ===== FUN_00181754 @ 00181754 ===== */

bool FUN_00181754(void)

{
  int iVar1;
  
  if (DAT_00284650 != 0) {
    iVar1 = FUN_00181b78(DAT_00284650,DAT_00284658);
    return iVar1 != 0;
  }
  return true;
}

/* ===== FUN_00181790 @ 00181790 ===== */

void FUN_00181790(ulong param_1)

{
  long lVar1;
  ulong uVar2;
  long lVar3;
  bool bVar4;
  ulong uVar5;
  bool bVar6;
  int iVar7;
  undefined8 uVar8;
  ulong *puVar9;
  ulong uVar10;
  ulong uVar11;
  uint uVar12;
  ulong uVar13;
  long lVar14;
  ulong uVar15;
  uint uVar16;
  ulong uVar17;
  ulong local_1088;
  short local_107c [2];
  ulong local_1078;
  ulong local_1070 [512];
  long local_70;
  
  uVar13 = 0;
  lVar3 = tpidr_el0;
  local_70 = *(long *)(lVar3 + 0x28);
  uVar15 = 1;
  local_1070[0] = param_1;
  do {
    uVar11 = DAT_001a7d30;
    uVar17 = local_1070[uVar13];
    if (uVar17 == 0) {
LAB_00181b38:
      uVar8 = 0;
      goto LAB_00181b3c;
    }
    uVar16 = 0;
    uVar10 = uVar17;
    do {
      if ((uVar10 == uVar11) || (0xf < uVar16)) goto LAB_00181878;
      local_1078 = 0;
      if (uVar10 + 0x40 >> 3 < 0x201) {
        bVar6 = false;
      }
      else {
        iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar10 + 0x38,&local_1078,8);
        bVar6 = iVar7 == 1;
      }
      uVar5 = local_1078;
      bVar4 = false;
      if (0xfff < local_1078) {
        bVar4 = bVar6;
      }
      bVar4 = (bool)(bVar4 & (local_1078 & 7) == 0);
      uVar2 = local_1078;
      if (!bVar4) {
        uVar2 = 0;
      }
      iVar7 = FUN_0016d5cc(uVar10,uVar2);
      if (iVar7 == 0) goto LAB_00181880;
      uVar16 = uVar16 + 1;
      uVar10 = uVar5;
    } while (bVar4);
    uVar10 = 0;
LAB_00181878:
    if (uVar10 == uVar11) goto LAB_00181b38;
LAB_00181880:
    local_1078 = 0;
    uVar11 = uVar17 + 8 >> 3;
    if (uVar11 < 0x201) {
      local_1078 = 0;
      bVar6 = false;
    }
    else {
      iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar17,&local_1078,8);
      bVar6 = iVar7 == 1;
    }
    bVar4 = false;
    if (0xfff < local_1078) {
      bVar4 = bVar6;
    }
    if (!(bool)(bVar4 & (local_1078 & 7) == 0)) {
      local_1078 = 0;
    }
    if (local_1078 == DAT_001a7cd0 + 0x11ad208U) {
LAB_00181968:
      uVar16 = 0;
      while( true ) {
        local_1078 = 0;
        if (uVar11 < 0x201) {
          bVar6 = false;
        }
        else {
          iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar17,&local_1078,8);
          bVar6 = iVar7 == 1;
        }
        bVar4 = false;
        if (0xfff < local_1078) {
          bVar4 = bVar6;
        }
        uVar10 = local_1078;
        if (!(bool)(bVar4 & (local_1078 & 7) == 0)) {
          uVar10 = 0;
        }
        uVar12 = 1;
        if (uVar10 == DAT_001a7cd0 + 0x11ad208U) {
          uVar12 = 2;
        }
        if (uVar12 <= uVar16) break;
        local_1078 = 0;
        lVar14 = 0x4e;
        if (uVar16 != 0) {
          lVar14 = 0xc0;
        }
        local_107c[0] = 0;
        if ((((lVar14 + uVar17 + 2 >> 1 < 0x801) ||
             (iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,lVar14 + uVar17,local_107c,2), iVar7 != 1)) ||
            (local_107c[0] < 0)) || (0x200 < local_107c[0])) goto LAB_00181b38;
        if (local_107c[0] != 0) {
          lVar14 = 0x50;
          if (uVar16 != 0) {
            lVar14 = 0x90;
          }
          if ((lVar14 + uVar17 + 8 >> 3 < 0x201) ||
             (iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,lVar14 + uVar17,&local_1078,8), iVar7 != 1))
          goto LAB_00181b38;
          if (0 < local_107c[0]) {
            lVar14 = 0;
            do {
              local_1088 = 0;
              lVar1 = local_1078 + lVar14 * 8;
              if ((lVar1 + 8U >> 3 < 0x201) ||
                 (iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,lVar1,&local_1088,8), iVar7 != 1))
              goto LAB_00181b38;
              if (local_1088 != 0) {
                iVar7 = (int)uVar15;
                if (iVar7 != 0) {
                  puVar9 = local_1070;
                  uVar10 = uVar15;
                  do {
                    if (*puVar9 == local_1088) goto LAB_00181aa0;
                    puVar9 = puVar9 + 1;
                    uVar10 = uVar10 - 1;
                  } while (uVar10 != 0);
                  if (iVar7 == 0x200) goto LAB_00181b38;
                }
                local_1070[uVar15] = local_1088;
                uVar15 = (ulong)(iVar7 + 1);
              }
LAB_00181aa0:
              lVar14 = lVar14 + 1;
            } while (lVar14 < local_107c[0]);
          }
        }
        uVar16 = uVar16 + 1;
      }
    }
    else {
      local_1078 = 0;
      if (uVar11 < 0x201) {
        bVar6 = false;
      }
      else {
        iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar17,&local_1078,8);
        bVar6 = iVar7 == 1;
      }
      bVar4 = false;
      if (0xfff < local_1078) {
        bVar4 = bVar6;
      }
      uVar10 = local_1078;
      if (!(bool)(bVar4 & (local_1078 & 7) == 0)) {
        uVar10 = 0;
      }
      if (uVar10 == DAT_001a7cd0 + 0x11abcf0U) goto LAB_00181968;
    }
    uVar13 = uVar13 + 1;
  } while (uVar13 < uVar15);
  uVar8 = 1;
LAB_00181b3c:
  if (*(long *)(lVar3 + 0x28) != local_70) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar8);
  }
  return;
}

/* ===== FUN_00181b78 @ 00181b78 ===== */

void FUN_00181b78(ulong param_1,long param_2)

{
  long lVar1;
  bool bVar2;
  bool bVar3;
  int iVar4;
  ulong uVar5;
  ulong local_40;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  if (DAT_001a7d30 == param_2) {
    local_40 = 0;
    if (param_1 + 8 >> 3 < 0x201) {
      bVar3 = false;
    }
    else {
      iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1,&local_40,8);
      bVar3 = iVar4 == 1;
    }
    bVar2 = false;
    if (0xfff < local_40) {
      bVar2 = bVar3;
    }
    uVar5 = local_40;
    if (!(bool)(bVar2 & (local_40 & 7) == 0)) {
      uVar5 = 0;
    }
    if (uVar5 == DAT_001a7cd0 + 0x11ad208U) {
      local_40 = 0;
      if ((((0xfff < param_1 + 0x30) && ((param_1 & 0xfffffffffffffff8) != 0xffffffffffffffc8)) &&
          (iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1 + 0x30,&local_40,8), iVar4 == 1)) &&
         ((local_40 == 0 || (local_40 == DAT_001a7d28)))) {
        uVar5 = FUN_0016d5cc(param_1,param_2);
        if ((int)uVar5 == 0) goto LAB_00181cb0;
        local_40 = CONCAT71(local_40._1_7_,0xff);
        if (0x1000 < param_1 + 0x49) {
          iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1 + 0x48,&local_40,1);
          uVar5 = (ulong)(iVar4 == 1 && (char)local_40 == '\0');
          goto LAB_00181cb0;
        }
      }
    }
  }
  uVar5 = 0;
LAB_00181cb0:
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar5);
}

/* ===== FUN_00181d04 @ 00181d04 ===== */

void FUN_00181d04(uint *param_1)

{
  uint *__s1;
  uint uVar1;
  long lVar2;
  uint *puVar3;
  int iVar4;
  void *pvVar5;
  char *pcVar6;
  ulong uVar7;
  size_t sVar8;
  int local_58;
  int iStack_54;
  uint *local_50;
  long local_48;
  
  lVar2 = tpidr_el0;
  local_48 = *(long *)(lVar2 + 0x28);
  iVar4 = FUN_00182de4();
  if (((iVar4 != 0) && (((param_1[1] ^ 0xffffffff) & 3) == 0)) && (*param_1 < 1000000)) {
    __s1 = param_1 + 0x1a;
    pvVar5 = memchr(__s1,0,0x60);
    if (pvVar5 != (void *)0x0) {
      param_1 = param_1 + 0x32;
      pvVar5 = memchr(param_1,0,0x60);
      if (((pvVar5 != (void *)0x0) && (iVar4 = strncmp((char *)__s1,"sc/",3), iVar4 == 0)) &&
         ((pcVar6 = strstr((char *)__s1,".."), pcVar6 == (char *)0x0 && ((char)*param_1 != '\0'))))
      {
        local_50 = (uint *)0x0;
        sVar8 = strlen((char *)__s1);
        local_58 = (int)sVar8;
        iStack_54 = local_58;
        puVar3 = __s1;
        if (local_58 < 8) {
          __memcpy_chk(&local_50,__s1,(long)local_58,8);
          puVar3 = local_50;
        }
        local_50 = puVar3;
        (*(code *)(DAT_001a7cd0 + 0x51f074))(&local_58,0);
        (*(code *)(DAT_001a7cd0 + 0x75da50))(&local_58,0);
        uVar7 = (*(code *)(DAT_001a7cd0 + 0x51e62c))(__s1,0);
        if (uVar7 != 0) {
          iVar4 = (*(code *)(DAT_001a7cd0 + 0x5dd3d0))(uVar7,param_1);
          uVar1 = 1;
          if (iVar4 == 0) {
            uVar1 = 0xffffffff;
          }
          uVar7 = (ulong)uVar1;
        }
        goto LAB_00181db8;
      }
    }
  }
  uVar7 = 0xffffffff;
LAB_00181db8:
  if (*(long *)(lVar2 + 0x28) != local_48) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar7);
  }
  return;
}

/* ===== FUN_00181ea0 @ 00181ea0 ===== */

void FUN_00181ea0(long param_1,int param_2)

{
  long lVar1;
  long lVar2;
  bool bVar3;
  ulong uVar4;
  bool bVar5;
  int iVar6;
  ulong uVar7;
  ulong *puVar8;
  ulong *puVar9;
  ulong uVar10;
  long lVar11;
  uint uVar12;
  ulong uVar13;
  ulong uVar14;
  ulong local_9198;
  byte local_9190 [4];
  short local_918c [2];
  ulong local_9188;
  long local_9180;
  ulong local_9178;
  ushort local_916c [2];
  long local_9168;
  ulong local_9160;
  long local_9158;
  char local_9150;
  undefined2 uStack_914f;
  int iStack_914d;
  undefined1 uStack_9149;
  short local_9148;
  ulong local_90d0 [512];
  ulong local_80d0 [4096];
  char local_d0;
  undefined7 uStack_cf;
  undefined3 uStack_c8;
  undefined5 local_c5;
  undefined3 uStack_c0;
  long local_70;
  
  lVar2 = tpidr_el0;
  local_70 = *(long *)(lVar2 + 0x28);
  uVar7 = (*(code *)(DAT_001a7cd0 + 0xd1763c))(param_1 + 0x68,param_1 + 200,0);
  uVar10 = uVar7;
  if (uVar7 != 0) {
    local_80d0[0] = 0;
    if (uVar7 + 8 >> 3 < 0x201) {
      bVar5 = false;
    }
    else {
      iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar7,local_80d0,8);
      bVar5 = iVar6 == 1;
    }
    bVar3 = false;
    if (0xfff < local_80d0[0]) {
      bVar3 = bVar5;
    }
    uVar14 = local_80d0[0];
    if (!(bool)(bVar3 & (local_80d0[0] & 7) == 0)) {
      uVar14 = 0;
    }
    uVar10 = 0;
    if (uVar14 == DAT_001a7cd0 + 0x11ad208U) {
      local_90d0[0] = local_90d0[0] & 0xffffffff00000000;
      local_80d0[0] = 1;
      if ((((((0xfff < uVar7 + 0x38) &&
             (uVar14 = uVar7 & 0xfffffffffffffff8, uVar14 != 0xffffffffffffffc0)) &&
            (iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar7 + 0x38,local_80d0,8), iVar6 == 1)) &&
           ((local_80d0[0] == 0 && (0xfff < uVar7 + 0x40)))) &&
          (((uVar7 & 0xfffffffffffffffc) != 0xffffffffffffffbc &&
           ((iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar7 + 0x40,local_90d0,4), iVar6 == 1 &&
            ((int)local_90d0[0] == -1)))))) &&
         ((local_80d0[0] = 0, 0xfcf < uVar7 &&
          (((uVar14 != 0xffffffffffffffc8 &&
            (iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar7 + 0x30,local_80d0,8), iVar6 == 1)) &&
           ((local_80d0[0] == 0 || (local_80d0[0] == DAT_001a7d28)))))))) {
        FUN_0016ce84(uVar7,0);
        FUN_0016ce20(uVar7,0);
        FUN_0016cf5c(0xc61c3c00,0xc61c3c00,uVar7);
        lVar1 = DAT_001a7cd0;
        uVar10 = uVar7;
        if (param_2 == 0) goto LAB_0018201c;
        local_9160 = 0;
        local_9158 = 0;
        local_9168 = 0;
        local_916c[0] = 0;
        local_80d0[0] = 0;
        if (((((0xfff < uVar7) &&
              (iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar7,local_80d0,8), uVar4 = local_80d0[0],
              iVar6 == 1)) && (0xfff < local_80d0[0])) &&
            (((local_80d0[0] & 7) == 0 && (local_80d0[0] == lVar1 + 0x11ad208U)))) &&
           (((0xfff < uVar7 + 0x80 &&
             ((uVar14 != 0xffffffffffffff78 &&
              (iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar7 + 0x80,&local_9158,8), lVar1 = local_9158,
              iVar6 == 1)))) && (local_9158 != 0)))) {
          lVar11 = 0;
          while (0x1000 < lVar1 + lVar11 + 1U) {
            iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,lVar1 + lVar11,&local_d0 + lVar11,1);
            if (iVar6 != 1) break;
            if ((&local_d0)[lVar11] == '\0') {
              if (local_d0 == '\0') break;
              uVar13 = uVar7;
              if ((CONCAT71(uStack_cf,local_d0) == 0x617274735f726762 &&
                  CONCAT53(local_c5,uStack_c8) == 0x6e6968747265676e) &&
                  CONCAT35(uStack_c0,local_c5) == 0x73676e69687472) {
                FUN_0016ce84(uVar7,0);
                local_90d0[0] = 0;
                if ((((uVar7 + 0x90 < 0x1000) || (uVar14 == 0xffffffffffffff68)) ||
                    (iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar7 + 0x90,&local_9160,8), iVar6 != 1))
                   || (((((local_9160 + 8 >> 3 < 0x201 || ((local_9160 & 7) != 0)) ||
                         ((iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,local_9160,local_90d0,8), iVar6 != 1
                          || ((local_90d0[0] < 0x1000 || ((local_90d0[0] & 7) != 0)))))) ||
                        (local_80d0[0] = 0, 0xfffffffffffffff7 < local_90d0[0])) ||
                       ((((iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,local_90d0[0],local_80d0,8),
                          iVar6 != 1 || (local_80d0[0] < 0x1000)) || ((local_80d0[0] & 7) != 0)) ||
                        (uVar13 = local_90d0[0], local_80d0[0] != uVar4)))))) break;
              }
              if ((((0xfff < uVar13 + 0xc0) && ((uVar13 & 0xfffffffffffffffe) != 0xffffffffffffff3e)
                   ) && ((iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar13 + 0xc0,local_916c,2),
                         iVar6 == 1 &&
                         ((-1 < (short)local_916c[0] && ((short)local_916c[0] < 0x1001)))))) &&
                 ((0xf67 < uVar13 &&
                  (iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar13 + 0x98,&local_9168,8), iVar6 == 1))))
              {
                if (local_916c[0] == 0) goto LAB_0018201c;
                if ((((0xf6f < uVar13) &&
                     (iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar13 + 0x90,&local_9160,8), iVar6 == 1)
                     ) && (0xfff < local_9160)) && ((local_9160 & 7) == 0)) {
                  if (local_916c[0] < 2) goto LAB_0018201c;
                  uVar12 = 0;
                  uVar10 = 1;
                  goto LAB_00182410;
                }
              }
              break;
            }
            lVar11 = lVar11 + 1;
            if (lVar11 == 0x60) break;
          }
        }
LAB_0018269c:
        (*(code *)(DAT_001a7cd0 + 0x5d48e8))(uVar7);
      }
      uVar10 = 0;
    }
  }
  goto LAB_0018201c;
LAB_00182410:
  do {
    local_9180 = 0;
    local_9178 = 0;
    local_9188 = 0;
    lVar1 = local_9160 + uVar10 * 8;
    local_918c[0] = 0;
    local_9190[0] = 0;
    if ((lVar1 + 8U >> 3 < 0x201) ||
       (iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,lVar1,&local_9178,8), iVar6 != 1)) goto LAB_0018269c;
    if (local_9178 != 0) {
      local_9198 = 0;
      if (((local_9178 + 8 >> 3 < 0x201) ||
          (iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,local_9178,&local_9198,8), iVar6 != 1)) ||
         ((local_9198 < 0x1000 || ((local_9198 & 7) != 0)))) goto LAB_0018269c;
      if (local_9198 == uVar4) {
        if ((((((local_9178 + 0x38 < 0x1000) ||
               ((local_9178 & 0xfffffffffffffff8) == 0xffffffffffffffc0)) ||
              (iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,local_9178 + 0x38,&local_9188,8), iVar6 != 1))
             || ((local_9188 != 0 && (local_9188 != uVar13)))) ||
            ((local_9178 + 0xc0 < 0x1000 ||
             (((local_9178 & 0xfffffffffffffffe) == 0xffffffffffffff3e ||
              (iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,local_9178 + 0xc0,local_918c,2), iVar6 != 1)))))
            ) || ((local_918c[0] < 0 ||
                  (((((0x1000 < local_918c[0] || (local_9178 + 9 < 0x1001)) ||
                     (iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,local_9178 + 8,local_9190,1), iVar6 != 1)
                     ) || (1 < local_9190[0])) ||
                   ((local_9168 != 0 &&
                    ((lVar1 = local_9168 + uVar10 * 8, lVar1 + 8U >> 3 < 0x201 ||
                     (iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,lVar1,&local_9180,8), iVar6 != 1)))))))))
           ) goto LAB_0018269c;
        lVar1 = local_9180;
        if (local_9180 == 0) {
          local_9150 = '\0';
        }
        else {
          lVar11 = 0;
          while( true ) {
            if (lVar1 + lVar11 + 1U < 0x1001) goto LAB_0018269c;
            iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,lVar1 + lVar11,&local_9150 + lVar11,1);
            if (iVar6 != 1) goto LAB_0018269c;
            if ((&local_9150)[lVar11] == '\0') break;
            lVar11 = lVar11 + 1;
            if (lVar11 == 0x80) goto LAB_0018269c;
          }
        }
        if ((CONCAT13((undefined1)iStack_914d,CONCAT21(uStack_914f,local_9150)) != 0x635f6762 ||
             iStack_914d != 0x6f6c6f63) &&
           (CONCAT17(uStack_9149,CONCAT43(iStack_914d,CONCAT21(uStack_914f,local_9150))) !=
            0x65747461705f6762 || local_9148 != 0x6e72)) {
          uVar14 = (ulong)uVar12;
          local_80d0[uVar12] = local_9178;
          iVar6 = FUN_00182fd8(&local_d0,uVar10 & 0xffffffff,(long)local_918c[0]);
          uVar12 = uVar12 + 1;
          *(bool *)((long)local_90d0 + uVar14) = iVar6 == 0;
        }
      }
    }
    uVar10 = uVar10 + 1;
  } while (uVar10 < ((long)(short)local_916c[0] & 0xffffffffU));
  uVar10 = uVar7;
  if (uVar12 != 0) {
    uVar14 = (ulong)uVar12;
    puVar8 = local_90d0;
    puVar9 = local_80d0;
    do {
      uVar14 = uVar14 - 1;
      *(char *)(*puVar9 + 8) = (char)*puVar8;
      puVar8 = (ulong *)((long)puVar8 + 1);
      puVar9 = puVar9 + 1;
    } while (uVar14 != 0);
  }
LAB_0018201c:
  if (*(long *)(lVar2 + 0x28) == local_70) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar10);
}

/* ===== FUN_001826ec @ 001826ec ===== */

void FUN_001826ec(long param_1,ulong param_2)

{
  long lVar1;
  int iVar2;
  ulong uVar3;
  int local_44;
  long local_40;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  if (DAT_001a7d30 == param_1) {
    uVar3 = FUN_0016d910();
    if ((int)uVar3 == 0) goto LAB_001827c0;
    local_44 = 0;
    local_40 = 1;
    if (((((0xfff < param_2 + 0x38) && ((param_2 & 0xfffffffffffffff8) != 0xffffffffffffffc0)) &&
         (iVar2 = (*DAT_001a7ce8)(DAT_001a7cc8,param_2 + 0x38,&local_40,8), iVar2 == 1)) &&
        ((local_40 == 0 && (0xfff < param_2 + 0x40)))) &&
       (((param_2 & 0xfffffffffffffffc) != 0xffffffffffffffbc &&
        ((iVar2 = (*DAT_001a7ce8)(DAT_001a7cc8,param_2 + 0x40,&local_44,4), iVar2 == 1 &&
         (local_44 == -1)))))) {
      FUN_0016cfb8(DAT_001a7d28,param_2);
      uVar3 = FUN_0016d5cc(param_2,param_1);
      if ((int)uVar3 == 0) goto LAB_001827c0;
      local_40 = CONCAT71(local_40._1_7_,0xff);
      if (0x1000 < param_2 + 0x49) {
        iVar2 = (*DAT_001a7ce8)(DAT_001a7cc8,param_2 + 0x48,&local_40,1);
        uVar3 = (ulong)(iVar2 == 1 && (char)local_40 == '\0');
        goto LAB_001827c0;
      }
    }
  }
  uVar3 = 0;
LAB_001827c0:
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar3);
}

/* ===== FUN_00182848 @ 00182848 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00182848(undefined8 param_1)

{
  long lVar1;
  bool bVar2;
  int iVar3;
  ushort uVar4;
  undefined1 auVar5 [16];
  float fVar6;
  undefined1 auVar7 [16];
  float fVar8;
  float fVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  float fVar12;
  float fVar13;
  float local_78;
  float fStack_74;
  float local_70;
  float local_6c;
  float local_68;
  float local_64;
  float local_60;
  float local_5c;
  long local_58;
  
  lVar1 = tpidr_el0;
  local_58 = *(long *)(lVar1 + 0x28);
  FUN_0016cf00(ZEXT816(0x3f800000),0);
  FUN_0016cf84(param_1,&local_78);
  bVar2 = false;
  fVar8 = local_70 - local_78;
  fVar9 = local_6c - fStack_74;
  if ((ABS(fVar8) != INFINITY) && (!NAN(ABS(fVar8)))) {
    bVar2 = false;
    auVar5._4_4_ = fVar9;
    auVar5._0_4_ = fVar8;
    auVar5._12_4_ = fVar9;
    auVar5._8_4_ = fVar8;
    auVar7 = NEON_fcmgt(auVar5,_DAT_0010f9f0,4);
    auVar5 = NEON_fcmge(_DAT_0010f9f0,auVar5,4);
    uVar4 = NEON_umaxv(CONCAT26(auVar5._12_2_,
                                CONCAT24(auVar5._8_2_,CONCAT22(auVar7._4_2_,auVar7._0_2_))),2);
    if (((uVar4 & 1) == 0) && ((ABS(fVar9) != INFINITY && (!NAN(ABS(fVar9)))))) {
      if (DAT_001a7d28 + 0x4cU >> 4 < 0x101) {
        bVar2 = false;
      }
      else {
        iVar3 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_001a7d28 + 0x3c,&local_68,0x10);
        bVar2 = false;
        if ((((iVar3 == 1) && (local_68 == 0.0)) && (local_64 == 0.0)) &&
           ((0.0 < local_60 && (0.0 < local_5c)))) {
          bVar2 = false;
          if ((ABS(DAT_001e53fc) != INFINITY) && ((!NAN(ABS(DAT_001e53fc)) && (0.0 < DAT_001e53fc)))
             ) {
            bVar2 = false;
            if ((ABS(DAT_001e5400) != INFINITY) && (!NAN(ABS(DAT_001e5400)))) {
              bVar2 = false;
              if ((ABS(DAT_001e5404) != INFINITY) && (!NAN(ABS(DAT_001e5404)))) {
                fVar13 = local_60 / DAT_001e53fc;
                bVar2 = false;
                fVar12 = local_5c / DAT_001e53fc;
                fVar8 = (float)NEON_fminnm(fVar13 / fVar8,fVar12 / fVar9);
                if ((ABS(fVar8) != INFINITY) &&
                   (((!NAN(ABS(fVar8)) && (0.0001 <= fVar8)) &&
                    (fVar8 == 128.0 || fVar8 < 128.0 != NAN(fVar8))))) {
                  fVar9 = (float)NEON_fnmsub(local_5c,0x3f000000,DAT_001e5404);
                  fVar6 = (float)NEON_fnmsub(local_60,0x3f000000,DAT_001e5400);
                  uVar11 = NEON_fmadd((fStack_74 + local_6c) * -0.5,fVar8,fVar9 / DAT_001e53fc);
                  uVar10 = NEON_fmadd((local_78 + local_70) * -0.5,fVar8,fVar6 / DAT_001e53fc);
                  FUN_0016cf00(fVar8,SUB42(fVar8,0),param_1);
                  FUN_0016cf5c(uVar10,(short)uVar11,param_1);
                  FUN_0016cf84(param_1,&local_78);
                  bVar2 = false;
                  if ((ABS(local_70 - local_78) != INFINITY) && (!NAN(ABS(local_70 - local_78)))) {
                    bVar2 = false;
                    if ((ABS(local_6c - fStack_74) != INFINITY) &&
                       (((!NAN(ABS(local_6c - fStack_74)) &&
                         (local_70 = local_70 - local_78,
                         local_70 != 0.0 && local_70 < 0.0 == NAN(local_70))) &&
                        ((local_6c = local_6c - fStack_74,
                         local_6c != 0.0 && local_6c < 0.0 == NAN(local_6c) &&
                         ((local_70 <= fVar13 + 0.5 && (local_6c <= fVar12 + 0.5)))))))) {
                      iVar3 = FUN_0016d910(0);
                      bVar2 = iVar3 != 0;
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
  if (*(long *)(lVar1 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(bVar2);
}

/* ===== FUN_00182b58 @ 00182b58 ===== */

void FUN_00182b58(ulong param_1,undefined8 param_2)

{
  ulong uVar1;
  long lVar2;
  bool bVar3;
  bool bVar4;
  int iVar5;
  undefined8 uVar6;
  ulong uVar7;
  int local_64;
  ulong local_60;
  long local_58;
  
  lVar2 = tpidr_el0;
  local_58 = *(long *)(lVar2 + 0x28);
  uVar6 = FUN_00182de4();
  if ((int)uVar6 == 0) goto LAB_00182cc4;
  local_60 = 0;
  if (param_1 + 8 >> 3 < 0x201) {
    bVar4 = false;
  }
  else {
    iVar5 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1,&local_60,8);
    bVar4 = iVar5 == 1;
  }
  bVar3 = false;
  if (0xfff < local_60) {
    bVar3 = bVar4;
  }
  uVar7 = local_60;
  if (!(bool)(bVar3 & (local_60 & 7) == 0)) {
    uVar7 = 0;
  }
  if (uVar7 == DAT_001a7cd0 + 0x11ad208U) {
    local_60 = 0;
    if ((((0xfff < param_1 + 0x30) &&
         (uVar7 = param_1 & 0xfffffffffffffff8, uVar7 != 0xffffffffffffffc8)) &&
        (iVar5 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1 + 0x30,&local_60,8), iVar5 == 1)) &&
       ((local_60 == 0 || (local_60 == DAT_001a7d28)))) {
      uVar1 = param_1 + 0x38;
      local_64 = 0;
      local_60 = 1;
      if ((((uVar1 < 0x1000 || uVar7 == 0xffffffffffffffc0) ||
           ((iVar5 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar1,&local_60,8), iVar5 != 1 || (local_60 != 0))
           )) || (param_1 + 0x40 < 0x1000)) ||
         ((((param_1 & 0xfffffffffffffffc) == 0xffffffffffffffbc ||
           (iVar5 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1 + 0x40,&local_64,4), iVar5 != 1)) ||
          (local_64 != -1)))) {
        uVar6 = FUN_00181b78(param_1,param_2);
        if ((int)uVar6 == 0) goto LAB_00182cc4;
        (*(code *)(DAT_001a7cd0 + 0x59567c))(param_1);
        local_64 = 0;
        local_60 = 1;
        if ((((uVar1 < 0x1000 || uVar7 == 0xffffffffffffffc0) ||
             (iVar5 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar1,&local_60,8), iVar5 != 1)) ||
            ((local_60 != 0 ||
             ((param_1 + 0x40 < 0x1000 || ((param_1 & 0xfffffffffffffffc) == 0xffffffffffffffbc)))))
            ) || ((iVar5 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1 + 0x40,&local_64,4), iVar5 != 1 ||
                  (local_64 != -1)))) goto LAB_00182cc0;
      }
      (*(code *)(DAT_001a7cd0 + 0x5d48e8))(param_1);
      uVar6 = 1;
      goto LAB_00182cc4;
    }
  }
LAB_00182cc0:
  uVar6 = 0;
LAB_00182cc4:
  if (*(long *)(lVar2 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar6);
}

/* ===== FUN_00182de4 @ 00182de4 ===== */

ulong FUN_00182de4(void)

{
  int iVar1;
  ulong uVar2;
  
  if (DAT_00284678 == 0) {
    uVar2 = thunk_FUN_0014d168(0x51f074,0x13c,
                               "9967d384457781c5ed13ed01ee77994e84ac16d941bf84600b1dba2f608c2fc0");
    if ((((((int)uVar2 == 0) ||
          (uVar2 = thunk_FUN_0014d168(0x75da50,0x7c,
                                      "d7097026eaaede752ff10f7f2503fc76a6c5ee064617b30359964bb70aeda490"
                                     ), (int)uVar2 == 0)) ||
         (uVar2 = thunk_FUN_0014d168(0x51e62c,0x1b0,
                                     "a7505857cc0b9c3a6f072e8a489eefcf2ce6640801405461d43ec8aea1f1d78f"
                                    ), (int)uVar2 == 0)) ||
        ((((uVar2 = thunk_FUN_0014d168(0x5dd3d0,0x134,
                                       "aecbacc988140fd316ebf71b15026686841694b5669ed1e87c10c6509d25e700"
                                      ), (int)uVar2 == 0 ||
           (uVar2 = thunk_FUN_0014d168(0xd1763c,0x6c,
                                       "27f8452a4e51f9f0c5ac16e43ba77488dd95391d3e006f1cd31a0bbc9084b878"
                                      ), (int)uVar2 == 0)) ||
          ((uVar2 = thunk_FUN_0014d168(0x594138,0x234,
                                       "e9799904da24c81159b38ac4f53d167b65aeb46496dae806e7fe6e340c63e87f"
                                      ), (int)uVar2 == 0 ||
           ((uVar2 = thunk_FUN_0014d168(0x5988ec,8,
                                        "f04a1fc14120f8ef9e56a6bfaffa9a957006937226bd5ad4ccf43b6e6bbcac66"
                                       ), (int)uVar2 == 0 ||
            (uVar2 = thunk_FUN_0014d168(0x59567c,0x20,
                                        "9e18529b88a6285f2675ad92537b12875a9735c6fc16cbdc947b58b5e235b5f9"
                                       ), (int)uVar2 == 0)))))) ||
         (uVar2 = thunk_FUN_0014d168(0x5d48e8,0x8c,
                                     "20951e09482741d88a2c79792b49f48689dea51fde868582e629a3a73e99a4ac"
                                    ), (int)uVar2 == 0)))) ||
       ((((uVar2 = thunk_FUN_0014d168(0x5d7c30,0x70,
                                      "289189be747ebdef62c061b87e11ca33589e787ed4648e7ef3cd72a4008682ce"
                                     ), (int)uVar2 == 0 ||
          (uVar2 = thunk_FUN_0014d168(0x595378,0x78,
                                      "26dd902aec11222432647f202e43e8c10add7f8359f6b5d9b3b260378524f74b"
                                     ), (int)uVar2 == 0)) ||
         (uVar2 = thunk_FUN_0014d168(0x59533c,8,
                                     "47c05c261ebe1823420dd2d8496ecd0f068d47a15b4d069011c964f33adc75e7"
                                    ), (int)uVar2 == 0)) ||
        ((uVar2 = thunk_FUN_0014d168(0x595344,8,
                                     "0e7d6896adc47cf1b6cf69345698abcd462544086684a596d8f6a85ea9374adb"
                                    ), (int)uVar2 == 0 ||
         (uVar2 = thunk_FUN_0014d168(0x595314,8,
                                     "e422ec3b09a5625f93137b982a59bf96b5426bcbc1090633ddb89812444c9691"
                                    ), (int)uVar2 == 0)))))) {
      DAT_00284678 = -1;
    }
    else {
      iVar1 = thunk_FUN_0014d168(0x594e80,0x60,
                                 "fdd9a09d4b77013ddd856b9e42ff25272aaacef9d8f0d5d200862083230ab3a0")
      ;
      uVar2 = (ulong)(iVar1 != 0);
      DAT_00284678 = -1;
      if (iVar1 != 0) {
        DAT_00284678 = 1;
      }
    }
  }
  else {
    uVar2 = (ulong)(DAT_00284678 == 1);
  }
  return uVar2;
}

/* ===== FUN_00182fd8 @ 00182fd8 ===== */

bool FUN_00182fd8(char *param_1,uint param_2,int param_3)

{
  uint uVar1;
  int iVar2;
  uint uVar3;
  ulong uVar4;
  uint *puVar5;
  long lVar6;
  
  lVar6 = 0x38;
  puVar5 = &DAT_0019cba4;
  do {
    iVar2 = strcmp(param_1,*(char **)(puVar5 + -3));
    if (iVar2 == 0) {
      if (2 < (ulong)-lVar6) {
        uVar3 = puVar5[-1];
        uVar1 = *puVar5;
        if (uVar1 < 2) {
          uVar1 = 1;
        }
        uVar4 = (ulong)uVar1;
        do {
          if ((*(uint *)(&DAT_00140260 + (ulong)uVar3 * 8) <= param_2) &&
             (param_2 <= *(uint *)(&DAT_00140264 + (ulong)uVar3 * 8))) {
            return true;
          }
          uVar3 = uVar3 + 1;
          uVar4 = uVar4 - 1;
        } while (uVar4 != 0);
      }
      return false;
    }
    puVar5 = puVar5 + 4;
    lVar6 = lVar6 + -1;
  } while (lVar6 != -3);
  if (param_3 == 1) {
    return false;
  }
  if (param_3 == 0x1b0) {
    return false;
  }
  return param_3 != 0x6c;
}

/* ===== FUN_001830b0 @ 001830b0 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_001830b0(ulong param_1,int param_2,long param_3,int param_4)

{
  int iVar1;
  ulong uVar2;
  
  if (DAT_0022d560 == 0) {
    _DAT_0022d568 = 0;
    _DAT_0022d578 = 0;
    _DAT_0022d570 = 0;
    _DAT_0022d560 = 1;
    DAT_0022d598 = 0;
    DAT_0022d5a8 = 0;
    DAT_0022d5a0 = 0;
    _DAT_0022d5b8 = 0;
    _DAT_0022d5b0 = 0;
    DAT_0022d5c8 = 0;
    _DAT_0022d5c0 = 0;
    _DAT_0022d5d8 = 0;
    DAT_0022d5d0 = 0;
    _DAT_0022d5e8 = 0;
    _DAT_0022d5e0 = 0;
    DAT_0022d5f8 = 0;
    _DAT_0022d5f0 = 0;
    _DAT_0022d580 = CONCAT44(0xffffffff,param_4);
    DAT_0022d590 = param_1;
    DAT_0022d588 = param_3;
    iVar1 = param_4;
    uVar2 = _DAT_0022d560;
  }
  else {
    iVar1 = DAT_0022d580;
    uVar2 = _DAT_0022d560;
  }
  if ((((param_2 == 0) || (param_3 == 0)) || (0x3ff < param_4 - 1U)) ||
     ((DAT_0022d588 != param_3 || (iVar1 != param_4)))) {
    DAT_0022d584 = (undefined4)((ulong)_DAT_0022d580 >> 0x20);
    if (DAT_0022d588 != param_3) {
      DAT_0022d584 = 0xffffffff;
      DAT_0022d588 = param_3;
    }
    _DAT_0022d580 = CONCAT44(DAT_0022d584,param_4);
  }
  else if ((DAT_0022d590 <= param_1) &&
          ((DAT_0022d564 = (int)(uVar2 >> 0x20), DAT_0022d564 == 0 ||
           (param_1 - DAT_0022d590 < 0xfb)))) {
    if ((DAT_0022d578 != 0) && (500 < param_1 - DAT_0022d5a0)) {
      _DAT_0022d578 = _DAT_0022d578 & 0xffffffff00000000;
      _DAT_0022d560 = uVar2;
      DAT_0022d590 = param_1;
      return 1;
    }
    _DAT_0022d560 = uVar2;
    DAT_0022d590 = param_1;
    return 1;
  }
  _DAT_0022d578 = _UNK_0010fa98;
  _DAT_0022d570 = _DAT_0010fa90;
  _DAT_0022d560 = uVar2 & 0xffffffff;
  FUN_00183230(0,0,&DAT_0022d518,param_1,0,4,0xffffffff);
  DAT_0022d590 = param_1;
  return 0;
}

/* ===== FUN_00183230 @ 00183230 ===== */

void FUN_00183230(float param_1,float param_2,float *param_3,ulong param_4,int param_5,uint param_6,
                 float param_7,float param_8)

{
  undefined8 uVar1;
  bool bVar2;
  ulong uVar3;
  double dVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  double dVar8;
  double dVar9;
  float fVar10;
  double dVar11;
  
  if (param_3 == (float *)0x0) {
    return;
  }
  if (param_3[7] == 0.0) {
    param_3[0x10] = 0.0;
    param_3[0x11] = 0.0;
    param_3[2] = 0.0;
    param_3[3] = 0.0;
    param_3[0] = 0.0;
    param_3[1] = 0.0;
    param_3[6] = 0.0;
    param_3[7] = 0.0;
    param_3[4] = 0.0;
    param_3[5] = 0.0;
    param_3[10] = 0.0;
    param_3[0xb] = 0.0;
    param_3[8] = 0.0;
    param_3[9] = 0.0;
    param_3[0xe] = 0.0;
    param_3[0xf] = 0.0;
    param_3[0xc] = 0.0;
    param_3[0xd] = 0.0;
    param_3[7] = 1.4013e-45;
    param_3[8] = param_8;
    *(ulong *)(param_3 + 10) = param_4;
    param_3[9] = -NAN;
  }
  fVar7 = *param_3;
  if ((ABS(fVar7) != INFINITY) && (!NAN(ABS(fVar7)))) {
    fVar10 = param_3[1];
    fVar5 = ABS(fVar10);
    if (((((fVar5 != INFINITY) &&
          (((!NAN(fVar5) && (ABS(param_3[0xe]) != INFINITY)) && (!NAN(ABS(param_3[0xe])))))) &&
         (((ABS(param_3[0xf]) != INFINITY && (!NAN(ABS(param_3[0xf])))) &&
          (ABS(param_3[0x10]) != INFINITY)))) && ((!NAN(ABS(param_3[0x10])) && (0.0 <= fVar7)))) &&
       (fVar5 == 2400.0 || fVar5 < 2400.0 != NAN(fVar5))) {
      if ((int)param_8 - 0x401U < 0xfffffc00) {
        fVar6 = 0.0;
        dVar8 = 0.0;
        fVar5 = (float)NEON_fmin(fVar7,0);
        *param_3 = fVar5;
        if (0x400 < (uint)param_8) goto LAB_0018346c;
      }
      else {
        dVar8 = (double)NEON_fmadd((double)((int)param_8 - 1),0x405b800000000000,0x4066400000000000)
        ;
        dVar8 = dVar8 + 32.0 + -472.0;
        if (dVar8 <= 0.0) {
          dVar8 = 0.0;
        }
        fVar6 = (float)dVar8;
        dVar8 = (double)fVar6;
        fVar5 = fVar6;
        if (fVar7 == fVar6 || fVar7 < fVar6 != (NAN(fVar7) || NAN(fVar6))) {
          fVar5 = fVar7;
        }
        *param_3 = fVar5;
      }
      if (((param_4 < *(ulong *)(param_3 + 10)) ||
          (uVar3 = param_4 - *(ulong *)(param_3 + 10), 0xfa < uVar3)) || (param_3[8] != param_8)) {
LAB_0018346c:
        param_3[6] = 0.0;
        param_3[1] = 0.0;
        *(ulong *)(param_3 + 4) = param_4;
        uVar1 = DAT_0010f740;
        *(ulong *)(param_3 + 10) = param_4;
        *(undefined8 *)(param_3 + 2) = uVar1;
        param_3[8] = param_8;
        param_3[9] = -NAN;
        return;
      }
      if (((ABS(param_1) != INFINITY) && (!NAN(ABS(param_1)))) &&
         ((param_8 != 0.0 && (param_6 != 4)))) {
        fVar7 = ABS(param_2);
        if ((fVar7 < INFINITY) || (fVar7 != INFINITY && fVar7 < INFINITY == NAN(fVar7))) {
          if (param_6 == 1) {
            if (((-1 < (int)param_7) && (param_5 == 1)) && (param_3[6] == 0.0)) {
              param_3[1] = 0.0;
              if (((param_2 <= 472.0) && (param_1 < -530.0 == NAN(param_1))) &&
                 ((param_1 <= 350.0 && (param_2 < 132.0 == NAN(param_2))))) {
                param_3[2] = 0.0;
                param_3[9] = param_7;
                param_3[0xf] = param_2;
                param_3[0x10] = param_2;
                param_3[6] = 1.4013e-45;
                param_3[0xe] = param_1;
                *(ulong *)(param_3 + 0xc) = param_4;
              }
              goto LAB_0018336c;
            }
          }
          else if ((param_6 & 0xfffffffe) == 2) {
            if ((((param_3[6] != 0.0) && (param_6 != 2 || param_5 == 1)) && (-1 < (int)param_7)) &&
               (param_3[9] == param_7)) {
              FUN_00183854(param_3,param_4);
              if (param_6 == 3) {
                fVar7 = param_3[2];
                if (fVar7 == 0.0) {
                  param_3[1] = 0.0;
                }
                else {
                  *(ulong *)(param_3 + 4) = param_4;
                }
                param_3[2] = 0.0;
                param_3[3] = (float)(uint)(fVar7 != 0.0);
                param_3[6] = 0.0;
                param_3[9] = -NAN;
              }
              goto LAB_0018336c;
            }
          }
          else if (param_3[6] == 0.0 && param_5 == 0) {
            if (0x3f < uVar3) {
              uVar3 = 0x40;
            }
            dVar9 = ((double)uVar3 / 1000.0) * -8.0;
            dVar4 = exp(dVar9);
            dVar11 = (double)fVar10;
            dVar9 = expm1(dVar9);
            dVar9 = (double)fVar5 + dVar11 * dVar9 * -0.125;
            fVar7 = fVar6;
            if (dVar9 == dVar8 || dVar9 < dVar8 != (NAN(dVar9) || NAN(dVar8))) {
              fVar7 = (float)dVar9;
            }
            fVar5 = 0.0;
            if (0.0 <= dVar9) {
              fVar5 = fVar7;
            }
            fVar7 = (float)(dVar4 * dVar11);
            *param_3 = fVar5;
            param_3[1] = fVar7;
            if ((fVar5 != 0.0) || (0.0 <= fVar7)) {
              bVar2 = false;
              if ((fVar7 != 0.0 && fVar7 < 0.0 == NAN(fVar7)) &&
                 (bVar2 = false, !NAN(fVar5) && !NAN(fVar6))) {
                bVar2 = fVar5 == fVar6;
              }
              if ((!bVar2) && (1.0 <= ABS(fVar7))) goto LAB_0018336c;
            }
            param_3[1] = 0.0;
            goto LAB_0018336c;
          }
        }
      }
      param_3[6] = 0.0;
      param_3[1] = 0.0;
      uVar1 = DAT_0010f740;
      *(ulong *)(param_3 + 4) = param_4;
      param_3[9] = -NAN;
      *(undefined8 *)(param_3 + 2) = uVar1;
      goto LAB_0018336c;
    }
  }
  param_3[0x10] = 0.0;
  param_3[6] = 0.0;
  uVar1 = DAT_0010f740;
  *(ulong *)(param_3 + 4) = param_4;
  param_3[9] = -NAN;
  param_3[0xe] = 0.0;
  param_3[0xf] = 0.0;
  param_3[0] = 0.0;
  param_3[1] = 0.0;
  *(undefined8 *)(param_3 + 2) = uVar1;
LAB_0018336c:
  *(ulong *)(param_3 + 10) = param_4;
  return;
}

/* ===== FUN_00183658 @ 00183658 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00183658(undefined8 param_1,undefined8 *param_2,undefined4 param_3,int param_4)

{
  undefined4 uVar1;
  ulong uVar2;
  double dVar3;
  
  dVar3 = (double)*(float *)((long)param_2 + 4) - (double)DAT_0022d5b4;
  dVar3 = (double)NEON_fmadd(dVar3,dVar3,
                             ((double)*(float *)(param_2 + 1) - (double)DAT_0022d5b8) *
                             ((double)*(float *)(param_2 + 1) - (double)DAT_0022d5b8));
  if (dVar3 < 64.0 == NAN(dVar3)) {
    DAT_0022d56c = 1;
  }
  if (param_2[4] != DAT_0022d5d0) {
    DAT_0022d570 = 0;
  }
  if (DAT_0022d568 == 0) {
    FUN_00183230(0,0,&DAT_0022d518,param_1,0,0,0xffffffff,param_3);
  }
  else {
    if (DAT_0022d530 == 0) {
      DAT_0022d570 = 0;
    }
    uVar1 = 2;
    if (param_4 != 0) {
      uVar1 = 3;
    }
    FUN_00183230(&DAT_0022d518,param_1,param_4 == 0,uVar1,*(undefined4 *)param_2,param_3);
    if ((DAT_0022d51c._4_4_ != 0) || ((param_4 != 0 && (DAT_0022d524 != 0)))) {
      DAT_0022d56c = 1;
    }
  }
  DAT_0022d5f8 = param_2[4];
  _DAT_0022d5e0 = param_2[1];
  _DAT_0022d5d8 = *param_2;
  _DAT_0022d5f0 = param_2[3];
  _DAT_0022d5e8 = param_2[2];
  if ((DAT_0022d56c != 0) && (DAT_0022d568 != 0)) {
    DAT_0022d57c = 1;
  }
  if (param_4 == 0) {
    return;
  }
  DAT_0022d564 = 0;
  if ((DAT_0022d570 == 0) || (DAT_0022d56c != 0)) {
    DAT_0022d578 = 0;
    goto joined_r0x00183818;
  }
  if ((DAT_0022d5c4 == 0) || (DAT_0022d5c8 == 0)) {
LAB_00183820:
    uVar2 = 0;
  }
  else {
    uVar2 = DAT_0022d5d0;
    if (DAT_0022d5d0 != 0) {
      if ((DAT_0022d5c4 != *(int *)((long)param_2 + 0x14)) || (DAT_0022d5c8 != param_2[3]))
      goto LAB_00183820;
      uVar2 = (ulong)(DAT_0022d5d0 == param_2[4]);
    }
  }
  DAT_0022d578 = (undefined4)uVar2;
  if ((uVar2 & 1) != 0) {
    DAT_0022d564 = 0;
    DAT_0022d5a0 = param_1;
    return;
  }
joined_r0x00183818:
  if (DAT_0022d568 != 0) {
    DAT_0022d57c = 1;
  }
  DAT_0022d5a0 = param_1;
  return;
}

/* ===== FUN_00183854 @ 00183854 ===== */

void FUN_00183854(float param_1,float param_2,float param_3,float *param_4,long param_5)

{
  long lVar1;
  float fVar2;
  long lVar3;
  float fVar4;
  double dVar5;
  double dVar6;
  double dVar7;
  double dVar8;
  float fVar9;
  float fVar10;
  
  fVar2 = param_4[2];
  if (fVar2 == 0.0) {
    dVar6 = (double)param_2 - (double)param_4[0xf];
    dVar6 = (double)NEON_fmadd((double)param_1 - (double)param_4[0xe],
                               (double)param_1 - (double)param_4[0xe],dVar6 * dVar6);
    if (dVar6 < 64.0 != NAN(dVar6)) goto LAB_001839ec;
    param_4[2] = 1.4013e-45;
  }
  lVar1 = 0x3c;
  if (fVar2 != 0.0) {
    lVar1 = 0x40;
  }
  fVar10 = 0.0;
  lVar3 = 0x30;
  if (fVar2 != 0.0) {
    lVar3 = 0x28;
  }
  lVar3 = *(long *)((long)param_4 + lVar3);
  dVar8 = (double)*(float *)((long)param_4 + lVar1) - (double)param_2;
  dVar5 = (double)param_3;
  dVar6 = dVar8 + (double)*param_4;
  fVar4 = param_3;
  if (dVar6 == dVar5 || dVar6 < dVar5 != (NAN(dVar6) || NAN(dVar5))) {
    fVar4 = (float)dVar6;
  }
  fVar9 = 0.0;
  if (0.0 <= dVar6) {
    fVar9 = fVar4;
  }
  *param_4 = fVar9;
  if (lVar3 == param_5) {
    if (dVar8 != 0.0) goto LAB_001839ac;
    if (fVar9 == 0.0) goto LAB_001839b8;
LAB_001839c0:
    if ((param_3 == 0.0) || ((dVar8 != 0.0 && dVar8 < 0.0 == NAN(dVar8)) && fVar9 == param_3))
    goto LAB_001839dc;
  }
  else {
    dVar6 = (double)(ulong)(param_5 - lVar3);
    dVar5 = (dVar8 * 1000.0) / dVar6;
    dVar7 = (double)NEON_fmin(dVar5,0x40a2c00000000000);
    fVar10 = -2400.0;
    if (-2400.0 <= dVar5) {
      fVar10 = (float)dVar7;
    }
    if (fVar2 != 0.0) {
      dVar6 = expm1(dVar6 / -50.0);
      dVar6 = (double)NEON_fmsub((double)fVar10 - (double)param_4[1],dVar6,(double)param_4[1]);
      fVar10 = (float)dVar6;
    }
LAB_001839ac:
    param_4[1] = fVar10;
    if (fVar9 != 0.0) goto LAB_001839c0;
LAB_001839b8:
    if (0.0 <= dVar8) goto LAB_001839c0;
LAB_001839dc:
    param_4[1] = 0.0;
  }
  *(long *)(param_4 + 4) = param_5;
  param_4[3] = 1.4013e-45;
LAB_001839ec:
  param_4[0x10] = param_2;
  return;
}

/* ===== FUN_00183ac8 @ 00183ac8 ===== */

void FUN_00183ac8(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
                 undefined8 param_5,undefined8 param_6,undefined8 param_7,undefined8 param_8,
                 undefined8 param_9)

{
  long lVar1;
  long lVar2;
  undefined8 local_90;
  undefined8 uStack_88;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined1 *local_70;
  undefined1 **ppuStack_68;
  undefined8 *puStack_60;
  undefined8 uStack_58;
  
  ppuStack_68 = &local_70;
  lVar1 = tpidr_el0;
  lVar2 = *(long *)(lVar1 + 0x28);
  puStack_60 = &local_90;
  uStack_58 = 0xffffff80ffffffe0;
  local_90 = param_6;
  uStack_88 = param_7;
  local_80 = param_8;
  uStack_78 = param_9;
  local_70 = (undefined1 *)register0x00000008;
  __vsnprintf_chk(param_2,param_4,0,param_3,param_5,&local_70,param_8,param_9,param_1);
  if (*(long *)(lVar1 + 0x28) == lVar2) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00183c78 @ 00183c78 ===== */

bool FUN_00183c78(int param_1)

{
  bool bVar1;
  ulong uVar2;
  
  if ((param_1 < 1) || (uVar2 = FUN_00193f80(1,&DAT_00284688), (uVar2 & 1) != 0)) {
    bVar1 = false;
  }
  else {
    bVar1 = DAT_00284694 == 0 || DAT_00284694 == param_1;
    if (DAT_00284694 == 0 || DAT_00284694 == param_1) {
      DAT_00284694 = param_1;
    }
    DAT_00284688 = 0;
  }
  return bVar1;
}

/* ===== FUN_00183cdc @ 00183cdc ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00183cdc(undefined4 param_1,int param_2,long param_3)

{
  ulong uVar1;
  
  _DAT_00284aec = param_1;
  uVar1 = FUN_00193f80(1,&DAT_00284688);
  if ((uVar1 & 1) == 0) {
    if (((DAT_00284694 == 0) || (DAT_00284694 == param_2)) && (DAT_00284698 = param_1, param_3 != 0)
       ) {
      FUN_00183ac8(&DAT_00284aa8,0x40,0x40,&DAT_001343dc,param_3);
    }
    DAT_00284688 = 0;
  }
  return;
}

/* ===== FUN_00184004 @ 00184004 ===== */

undefined8 FUN_00184004(ulong param_1)

{
  int iVar1;
  undefined8 uVar2;
  ulong in_x9;
  undefined4 *unaff_x19;
  undefined8 *unaff_x20;
  uint unaff_w21;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  
  if ((in_x9 & param_1) == 0) {
    if (unaff_w21 == 6) {
LAB_00184014:
      if ((((unaff_x20[2] & DAT_00284a58) != 0) || ((unaff_x20[3] & DAT_00284a60) != 0)) ||
         ((unaff_x20[4] & DAT_00284a68) != 0)) goto LAB_001840dc;
    }
    else {
      if ((((unaff_x20[2] & DAT_00284a18) != 0) || ((unaff_x20[3] & DAT_00284a20) != 0)) ||
         ((unaff_x20[4] & DAT_00284a28) != 0)) goto LAB_001840dc;
      if (unaff_w21 != 7) goto LAB_00184014;
    }
    uVar3 = unaff_x20[4];
    uVar5 = unaff_x20[7];
    uVar4 = unaff_x20[6];
    uVar2 = 1;
    iVar1 = unaff_x19[6];
    uVar9 = unaff_x20[1];
    uVar8 = *unaff_x20;
    uVar7 = unaff_x20[3];
    uVar6 = unaff_x20[2];
    *(undefined8 *)(unaff_x19 + (ulong)unaff_w21 * 0x10 + 0x8a) = unaff_x20[5];
    *(undefined8 *)(unaff_x19 + (ulong)unaff_w21 * 0x10 + 0x88) = uVar3;
    *(undefined8 *)(unaff_x19 + (ulong)unaff_w21 * 0x10 + 0x8e) = uVar5;
    *(undefined8 *)(unaff_x19 + (ulong)unaff_w21 * 0x10 + 0x8c) = uVar4;
    unaff_x19[6] = iVar1 + 1;
    *(undefined8 *)(unaff_x19 + (ulong)unaff_w21 * 0x10 + 0x82) = uVar9;
    *(undefined8 *)(unaff_x19 + (ulong)unaff_w21 * 0x10 + 0x80) = uVar8;
    *(undefined8 *)(unaff_x19 + (ulong)unaff_w21 * 0x10 + 0x86) = uVar7;
    *(undefined8 *)(unaff_x19 + (ulong)unaff_w21 * 0x10 + 0x84) = uVar6;
  }
  else {
LAB_001840dc:
    uVar2 = 4;
  }
  *unaff_x19 = 0;
  return uVar2;
}

/* ===== FUN_001842d0 @ 001842d0 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_001842d0(undefined8 *param_1)

{
  long lVar1;
  byte *pbVar2;
  uint uVar3;
  undefined8 uVar4;
  long lVar5;
  long lVar6;
  uint uVar7;
  byte bVar8;
  uint uVar9;
  byte bVar12;
  byte bVar13;
  byte bVar14;
  undefined1 auVar10 [16];
  undefined1 auVar11 [16];
  
  FUN_00185d10();
  uVar4 = DAT_0010f868;
  lVar5 = 0;
  lVar6 = (long)param_1 + 0x17;
  *param_1 = 0x32353239364d4e;
  param_1[1] = uVar4;
  do {
    lVar1 = lVar5 * 4;
    *(int *)(lVar6 + -7) = (int)lVar5;
    lVar5 = lVar5 + 1;
    *(undefined4 *)(lVar6 + -3) = *(undefined4 *)((long)&DAT_002846b0 + lVar1);
    lVar6 = lVar6 + 8;
  } while (lVar5 != 0x76);
  lVar6 = 0;
  uVar7 = 0xffffffff;
  do {
    pbVar2 = (byte *)((long)param_1 + lVar6);
    lVar6 = lVar6 + 1;
    uVar9 = uVar7 ^ *pbVar2;
    uVar3 = -(uVar9 & 1) & 0xedb88320 ^ uVar9 >> 1;
    bVar8 = (byte)uVar9;
    auVar10[0] = bVar8 & (byte)_DAT_0010f9c0;
    bVar12 = (byte)(uVar7 >> 8);
    auVar10[1] = bVar12 & (byte)((ulong)_DAT_0010f9c0 >> 8);
    bVar13 = (byte)(uVar7 >> 0x10);
    auVar10[2] = bVar13 & (byte)((ulong)_DAT_0010f9c0 >> 0x10);
    bVar14 = (byte)(uVar7 >> 0x18);
    auVar10[3] = bVar14 & (byte)((ulong)_DAT_0010f9c0 >> 0x18);
    auVar10[4] = bVar8 & (byte)((ulong)_DAT_0010f9c0 >> 0x20);
    auVar10[5] = bVar12 & (byte)((ulong)_DAT_0010f9c0 >> 0x28);
    auVar10[6] = bVar13 & (byte)((ulong)_DAT_0010f9c0 >> 0x30);
    auVar10[7] = bVar14 & (byte)((ulong)_DAT_0010f9c0 >> 0x38);
    auVar10[8] = bVar8 & (byte)_UNK_0010f9c8;
    auVar10[9] = bVar12 & (byte)((ulong)_UNK_0010f9c8 >> 8);
    auVar10[10] = bVar13 & (byte)((ulong)_UNK_0010f9c8 >> 0x10);
    auVar10[0xb] = bVar14 & (byte)((ulong)_UNK_0010f9c8 >> 0x18);
    auVar10[0xc] = bVar8 & (byte)((ulong)_UNK_0010f9c8 >> 0x20);
    auVar10[0xd] = bVar12 & (byte)((ulong)_UNK_0010f9c8 >> 0x28);
    auVar10[0xe] = bVar13 & (byte)((ulong)_UNK_0010f9c8 >> 0x30);
    auVar10[0xf] = bVar14 & (byte)((ulong)_UNK_0010f9c8 >> 0x38);
    uVar7 = (int)(uVar9 << 0x1e) >> 0x1f & 0xedb88320U ^ uVar3 >> 1;
    auVar10 = NEON_cmeq(auVar10,0,2);
    auVar11[0] = (byte)_DAT_0010fac0 & ~auVar10[0];
    auVar11[1] = (byte)((ulong)_DAT_0010fac0 >> 8) & ~auVar10[1];
    auVar11[2] = (byte)((ulong)_DAT_0010fac0 >> 0x10) & ~auVar10[2];
    auVar11[3] = (byte)((ulong)_DAT_0010fac0 >> 0x18) & ~auVar10[3];
    auVar11[4] = (byte)((ulong)_DAT_0010fac0 >> 0x20) & ~auVar10[4];
    auVar11[5] = (byte)((ulong)_DAT_0010fac0 >> 0x28) & ~auVar10[5];
    auVar11[6] = (byte)((ulong)_DAT_0010fac0 >> 0x30) & ~auVar10[6];
    auVar11[7] = (byte)((ulong)_DAT_0010fac0 >> 0x38) & ~auVar10[7];
    auVar11[8] = (byte)_UNK_0010fac8 & ~auVar10[8];
    auVar11[9] = (byte)((ulong)_UNK_0010fac8 >> 8) & ~auVar10[9];
    auVar11[10] = (byte)((ulong)_UNK_0010fac8 >> 0x10) & ~auVar10[10];
    auVar11[0xb] = (byte)((ulong)_UNK_0010fac8 >> 0x18) & ~auVar10[0xb];
    auVar11[0xc] = (byte)((ulong)_UNK_0010fac8 >> 0x20) & ~auVar10[0xc];
    auVar11[0xd] = (byte)((ulong)_UNK_0010fac8 >> 0x28) & ~auVar10[0xd];
    auVar11[0xe] = (byte)((ulong)_UNK_0010fac8 >> 0x30) & ~auVar10[0xe];
    auVar11[0xf] = (byte)((ulong)_UNK_0010fac8 >> 0x38) & ~auVar10[0xf];
    auVar10 = NEON_ext(auVar11,auVar11,8,1);
    uVar9 = CONCAT13(auVar11[3] ^ auVar10[3],
                     CONCAT12(auVar11[2] ^ auVar10[2],
                              CONCAT11(auVar11[1] ^ auVar10[1],auVar11[0] ^ auVar10[0])));
    uVar7 = uVar9 ^ (int)(uVar3 << 0x1a) >> 0x1f & 0x76dc4190U ^
            (int)(uVar7 << 0x1a) >> 0x1f & 0xedb88320U ^ uVar7 >> 6 ^
            (uint)(CONCAT17(auVar11[7] ^ auVar10[7],
                            CONCAT16(auVar11[6] ^ auVar10[6],
                                     CONCAT15(auVar11[5] ^ auVar10[5],
                                              CONCAT14(auVar11[4] ^ auVar10[4],uVar9)))) >> 0x20);
  } while (lVar6 != 0x3c0);
  *(uint *)(param_1 + 0x78) = ~uVar7;
  return;
}

/* ===== FUN_001846b8 @ 001846b8 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_001846b8(void *param_1)

{
  long lVar1;
  int iVar2;
  long lVar3;
  code *pcVar4;
  int *piVar5;
  undefined8 uVar6;
  int iVar8;
  undefined8 uVar7;
  undefined8 uVar9;
  undefined8 uVar10;
  int local_3f8 [232];
  undefined4 local_58;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_4c;
  long local_48;
  
  lVar1 = tpidr_el0;
  lVar3 = 0;
  local_48 = *(long *)(lVar1 + 0x28);
  piVar5 = local_3f8;
  uVar6 = _DAT_0010fb50;
  uVar7 = _UNK_0010fb58;
  do {
    uVar10 = ((undefined8 *)((long)param_1 + lVar3))[1];
    uVar9 = *(undefined8 *)((long)param_1 + lVar3);
    lVar3 = lVar3 + 0x10;
    *piVar5 = (int)uVar6;
    piVar5[1] = (int)uVar9;
    iVar2 = (int)((ulong)uVar6 >> 0x20);
    piVar5[2] = iVar2;
    piVar5[3] = (int)((ulong)uVar9 >> 0x20);
    piVar5[4] = (int)uVar7;
    piVar5[5] = (int)uVar10;
    iVar8 = (int)((ulong)uVar7 >> 0x20);
    piVar5[6] = iVar8;
    piVar5[7] = (int)((ulong)uVar10 >> 0x20);
    piVar5 = piVar5 + 8;
    uVar6 = CONCAT44(iVar2 + 4,(int)uVar6 + 4);
    uVar7 = CONCAT44(iVar8 + 4,(int)uVar7 + 4);
  } while (lVar3 != 0x1d0);
  local_54 = *(undefined4 *)((long)param_1 + 0x1d0);
  local_4c = *(undefined4 *)((long)param_1 + 0x1d4);
  local_58 = 0x74;
  local_50 = 0x75;
  if (DAT_002848c0 == (code *)0x0) {
    if (DAT_00284900 == (code *)0x0) {
      if (DAT_00284940 == (code *)0x0) {
        if (DAT_00284980 == (code *)0x0) {
          if (DAT_002849c0 == (code *)0x0) {
            if (DAT_00284a00 == (code *)0x0) {
              if (DAT_00284a40 == (code *)0x0) {
                if (DAT_00284a80 == (code *)0x0) {
                  iVar2 = 0;
                  goto LAB_00184820;
                }
                lVar3 = 7;
                pcVar4 = DAT_00284a80;
              }
              else {
                lVar3 = 6;
                pcVar4 = DAT_00284a40;
              }
            }
            else {
              lVar3 = 5;
              pcVar4 = DAT_00284a00;
            }
          }
          else {
            lVar3 = 4;
            pcVar4 = DAT_002849c0;
          }
        }
        else {
          lVar3 = 3;
          pcVar4 = DAT_00284980;
        }
      }
      else {
        lVar3 = 2;
        pcVar4 = DAT_00284940;
      }
    }
    else {
      lVar3 = 1;
      pcVar4 = DAT_00284900;
    }
  }
  else {
    lVar3 = 0;
    pcVar4 = DAT_002848c0;
  }
  iVar2 = (*pcVar4)((&DAT_00284890)[lVar3 * 8],local_3f8,0x76);
  if (iVar2 - 1U < 2) {
    FUN_0014fbe8(*(undefined4 *)((long)param_1 + 0x40));
    memcpy(&DAT_002846b0,param_1,0x1d8);
    DAT_002846a0 = DAT_002846a0 + 1;
  }
LAB_00184820:
  if (*(long *)(lVar1 + 0x28) == local_48) {
    return iVar2;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00186bf4 @ 00186bf4 ===== */

void FUN_00186bf4(void)

{
  DAT_00287c60 = DAT_0028dd08 + 1;
  DAT_00287c68 = 0;
  if (DAT_0028dd08 == -1) {
    DAT_00287c60 = 1;
  }
  DAT_00287b90 = 2;
  DAT_0028dd08 = DAT_00287c60;
  FUN_00183ac8(&DAT_00287c6c,0x60,0x60,&DAT_001343dc,"LOADING PREVIEW");
  FUN_00194010(1,&DAT_0022d824);
  DAT_00287bb4 = 0;
  DAT_0028469f = 0;
  DAT_00287bac = DAT_00287bac + 1;
  DAT_002846a0 = DAT_002846a0 + 1;
  return;
}

/* ===== FUN_001895e0 @ 001895e0 ===== */

bool FUN_001895e0(int param_1,byte *param_2,uint *param_3)

{
  bool bVar1;
  ulong uVar2;
  byte bVar3;
  uint uVar4;
  size_t sVar5;
  bool bVar6;
  ulong uVar7;
  int iVar8;
  uint uVar9;
  long lVar10;
  
  param_3[0] = 0;
  param_3[1] = 0;
  param_3[2] = 0;
  param_3[3] = 0;
  if (param_1 == 9) {
    if (((param_2 != (byte *)0x0) && (*param_2 != 0)) &&
       (sVar5 = strlen((char *)param_2), sVar5 < 0x61)) {
      bVar6 = false;
      uVar7 = 0;
      do {
        for (; bVar3 = *param_2, bVar3 == 0x20 || bVar3 == 9; param_2 = param_2 + 1) {
        }
        if (bVar3 == 0x2d || bVar3 == 0x2b) {
          param_2 = param_2 + 1;
        }
        if (*param_2 - 0x3a < 0xfffffff6) {
          return bVar6;
        }
        uVar9 = 0;
        iVar8 = -0x80000000;
        if (bVar3 != 0x2d) {
          iVar8 = 0x7fffffff;
        }
        while (uVar4 = *param_2 - 0x30, uVar4 < 10) {
          param_2 = param_2 + 1;
          bVar1 = (iVar8 - uVar4) / 10 < uVar9;
          uVar9 = uVar4 + uVar9 * 10;
          if (bVar1) {
            return bVar6;
          }
        }
        lVar10 = 0;
        uVar4 = -uVar9;
        if (bVar3 != 0x2d) {
          uVar4 = uVar9;
        }
        param_3[uVar7] = uVar4;
        for (; bVar3 = param_2[lVar10], bVar3 == 0x20 || bVar3 == 9; lVar10 = lVar10 + 1) {
        }
        param_2 = param_2 + lVar10;
        if (uVar7 < 3) {
          if (bVar3 != 0x2c) {
            return bVar6;
          }
          param_2 = param_2 + 1;
        }
        else if (bVar3 != 0) {
          return bVar6;
        }
        uVar2 = uVar7 + 1;
        bVar6 = 2 < uVar7;
        uVar7 = uVar2;
      } while (uVar2 != 4);
      return bVar6;
    }
  }
  else {
    if (param_1 != 4) {
      return false;
    }
    if (param_2 == (byte *)0x0) {
      return false;
    }
    if ((*param_2 - 0x30 < 5) && (param_2[1] == 0)) {
      *param_3 = *param_2 - 0x30;
      return true;
    }
  }
  return false;
}

/* ===== FUN_0018cb98 @ 0018cb98 ===== */

undefined8 FUN_0018cb98(int *param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  void *pvVar4;
  ulong uVar5;
  uint *puVar6;
  ulong uVar7;
  
  if ((((*param_1 == 1) && (param_1[1] == 0x2534)) && (param_1[5] == param_2)) &&
     (uVar2 = param_1[6], uVar2 < 0x21)) {
    uVar3 = param_1[4];
    uVar1 = 0;
    if (param_2 <= uVar3) {
      uVar1 = uVar3 - param_2;
    }
    if (0x1000 < uVar3) {
      return 0;
    }
    if (uVar1 < uVar2) {
      return 0;
    }
    if ((((uint)param_1[10] < 8) && ((uint)param_1[0xb] < 2)) && ((uint)param_1[0xc] < 2)) {
      if (uVar2 == 0) {
        return 1;
      }
      uVar7 = 0;
      while (((uint)param_1[uVar7 * 0x4a + 0xd] < 1000000 &&
             ((*(byte *)(param_1 + uVar7 * 0x4a + 0xe) & 1) != 0))) {
        pvVar4 = memchr(param_1 + uVar7 * 0x4a + 0xf,0,0x60);
        if (pvVar4 == (void *)0x0) {
          return 0;
        }
        if ((char)param_1[uVar7 * 0x4a + 0xf] == '\0') {
          return 0;
        }
        pvVar4 = memchr(param_1 + uVar7 * 0x4a + 0x27,0,0x60);
        if (pvVar4 == (void *)0x0) {
          return 0;
        }
        if ((char)param_1[uVar7 * 0x4a + 0x27] == '\0') {
          return 0;
        }
        pvVar4 = memchr(param_1 + uVar7 * 0x4a + 0x3f,0,0x60);
        if (pvVar4 == (void *)0x0) {
          return 0;
        }
        if ((char)param_1[uVar7 * 0x4a + 0x3f] == '\0') {
          return 0;
        }
        if (uVar7 != 0) {
          uVar5 = uVar7;
          puVar6 = (uint *)(param_1 + 0xd);
          do {
            if (*puVar6 == param_1[uVar7 * 0x4a + 0xd]) {
              return 0;
            }
            puVar6 = puVar6 + 0x4a;
            uVar5 = uVar5 - 1;
          } while (uVar5 != 0);
        }
        uVar7 = uVar7 + 1;
        if ((uint)param_1[6] <= uVar7) {
          return 1;
        }
      }
    }
  }
  return 0;
}

/* ===== FUN_0018d334 @ 0018d334 ===== */

void FUN_0018d334(void)

{
  long lVar1;
  int iVar2;
  undefined4 uVar3;
  undefined1 auStack_3ec [964];
  long local_28;
  
  lVar1 = tpidr_el0;
  local_28 = *(long *)(lVar1 + 0x28);
  if (DAT_00284a98 == (code *)0x0) {
    uVar3 = 1;
  }
  else {
    FUN_001842d0(auStack_3ec);
    iVar2 = (*DAT_00284a98)(DAT_00284a90,auStack_3ec,0x3c4);
    uVar3 = 1;
    if (iVar2 == 0) {
      uVar3 = 0xffffffff;
    }
  }
  if (*(long *)(lVar1 + 0x28) != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar3);
  }
  return;
}

/* ===== FUN_0018d3b4 @ 0018d3b4 ===== */

undefined * FUN_0018d3b4(ulong param_1)

{
  ulong uVar1;
  long lVar2;
  ulong uVar3;
  
  uVar1 = param_1 >> 6 & 0x3ffffff;
  uVar3 = 1L << (param_1 & 0x3f);
  if (((&DAT_00284898)[uVar1] & uVar3) == 0) {
    if (((&DAT_002848d8)[uVar1] & uVar3) == 0) {
      if (((&DAT_00284918)[uVar1] & uVar3) == 0) {
        if (((&DAT_00284958)[uVar1] & uVar3) == 0) {
          if (((&DAT_00284998)[uVar1] & uVar3) == 0) {
            if (((&DAT_002849d8)[uVar1] & uVar3) == 0) {
              if (((&DAT_00284a18)[uVar1] & uVar3) == 0) {
                if (((&DAT_00284a58)[uVar1] & uVar3) == 0) {
                  return (undefined *)0x0;
                }
                lVar2 = 7;
              }
              else {
                lVar2 = 6;
              }
            }
            else {
              lVar2 = 5;
            }
          }
          else {
            lVar2 = 4;
          }
        }
        else {
          lVar2 = 3;
        }
      }
      else {
        lVar2 = 2;
      }
    }
    else {
      lVar2 = 1;
    }
  }
  else {
    lVar2 = 0;
  }
  return &DAT_00284888 + lVar2 * 0x40;
}

/* ===== FUN_0018d500 @ 0018d500 ===== */

void FUN_0018d500(void)

{
  long lVar1;
  undefined8 local_38;
  undefined8 local_30;
  long local_28;
  
  lVar1 = tpidr_el0;
  local_28 = *(long *)(lVar1 + 0x28);
  local_38 = 0;
  local_30 = 0;
  dl_iterate_phdr(FUN_0018d56c,&local_38);
  if ((int)local_38 == 1) {
    DAT_0028dd00 = local_30;
  }
  if (*(long *)(lVar1 + 0x28) == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0018d56c @ 0018d56c ===== */

undefined8 FUN_0018d56c(long *param_1,undefined8 param_2,int *param_3)

{
  long lVar1;
  int iVar2;
  char *pcVar3;
  long lVar4;
  long lVar5;
  undefined1 auStack_68 [8];
  long local_60;
  long local_48;
  
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  if (((char *)param_1[1] != (char *)0x0) &&
     (pcVar3 = strrchr((char *)param_1[1],0x2f), pcVar3 != (char *)0x0)) {
    iVar2 = strcmp(pcVar3 + 1,"libNexusDelivery.so");
    if (iVar2 == 0) {
      *param_3 = *param_3 + 1;
      lVar4 = dlopen(param_1[1],6);
      if (lVar4 != 0) {
        lVar5 = dlsym(lVar4,"nexus_protected_plus_active");
        if (((lVar5 != 0) && (iVar2 = dladdr(lVar5,auStack_68), iVar2 != 0)) &&
           (local_60 == *param_1)) {
          *(long *)(param_3 + 2) = lVar5;
        }
        dlclose(lVar4);
      }
    }
  }
  if (*(long *)(lVar1 + 0x28) == local_48) {
    return 0;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0018df74 @ 0018df74 ===== */

void FUN_0018df74(uint *param_1)

{
  long lVar1;
  uint uVar2;
  undefined *puVar3;
  int iVar4;
  uint uVar5;
  ulong uVar6;
  int *piVar7;
  long lVar8;
  code *pcVar9;
  int local_70 [5];
  char cStack_5b;
  undefined4 local_5c;
  long local_58;
  
  puVar3 = PTR_PTR_s_nexus_sx_spin_001a36f8;
  lVar1 = tpidr_el0;
  local_58 = *(long *)(lVar1 + 0x28);
  uVar5 = *param_1;
  uVar2 = uVar5 >> 6;
  uVar6 = 1L << ((ulong)uVar5 & 0x3f);
  if ((uVar6 & (&DAT_00284898)[uVar2]) == 0) {
    if (((&DAT_002848d8)[uVar2] & uVar6) == 0) {
      if (((&DAT_00284918)[uVar2] & uVar6) == 0) {
        if (((&DAT_00284958)[uVar2] & uVar6) == 0) {
          if (((&DAT_00284998)[uVar2] & uVar6) == 0) {
            if (((&DAT_002849d8)[uVar2] & uVar6) == 0) {
              if (((&DAT_00284a18)[uVar2] & uVar6) == 0) {
                if (((&DAT_00284a58)[uVar2] & uVar6) == 0) goto LAB_0018e0a0;
                lVar8 = 7;
              }
              else {
                lVar8 = 6;
              }
            }
            else {
              lVar8 = 5;
            }
          }
          else {
            lVar8 = 4;
          }
        }
        else {
          lVar8 = 3;
        }
      }
      else {
        lVar8 = 2;
      }
    }
    else {
      lVar8 = 1;
    }
  }
  else {
    lVar8 = 0;
  }
  pcVar9 = (code *)(&DAT_002848b0)[lVar8 * 8];
  if (((pcVar9 != (code *)0x0) && (-1 < (int)param_1[0xe])) && ((byte)param_1[0x11] < 5)) {
    iVar4 = FUN_0018ded4(*(undefined8 *)
                          (PTR_PTR_s_nexus_sx_spin_001a36f8 + (ulong)param_1[0xe] * 0x18));
    if (iVar4 == 0) {
      local_70[3] = 0;
      local_70[4] = 0;
      local_70[1] = 0;
      local_70[2] = 0;
      local_5c = 0;
      local_70[0] = 0x18;
      iVar4 = (*pcVar9)((&DAT_00284890)[lVar8 * 8],(ulong)uVar5,local_70);
      if (((((iVar4 == 1) && (local_70[0] == 0x18)) &&
           ((cStack_5b != '\0' &&
            ((uVar5 = FUN_0018d15c(param_1), -1 < (int)uVar5 &&
             (*(int *)(puVar3 + (ulong)uVar5 * 0x18 + 8) <= local_70[3])))))) &&
          (local_70[3] <= *(int *)(puVar3 + (ulong)uVar5 * 0x18 + 0xc))) &&
         (piVar7 = (int *)((long)&DAT_002846b0 + (ulong)uVar5 * 4), *piVar7 != local_70[3])) {
        *piVar7 = local_70[3];
        DAT_002846a0 = DAT_002846a0 + 1;
      }
    }
  }
LAB_0018e0a0:
  if (*(long *)(lVar1 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0018e668 @ 0018e668 ===== */

void FUN_0018e668(void)

{
  long lVar1;
  bool bVar2;
  bool bVar3;
  int iVar4;
  ulong uVar5;
  ulong uVar6;
  ulong uVar7;
  ulong uVar8;
  ulong local_40;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  local_40 = 0;
  if (DAT_0028fad0 + 0x12eb9f8U >> 3 < 0x201) {
    uVar6 = 0;
    uVar8 = 0;
  }
  else {
    iVar4 = (*DAT_0028fae0)(DAT_0028fac8,DAT_0028fad0 + 0x12eb9f0,&local_40,8);
    uVar7 = local_40;
    bVar3 = (local_40 & 7) != 0;
    uVar5 = local_40;
    if ((iVar4 != 1 || local_40 < 0x1000) || bVar3) {
      uVar5 = 0;
    }
    uVar6 = 0;
    uVar8 = 0;
    if ((iVar4 == 1 && 0xfff < local_40) && !bVar3) {
      local_40 = 0;
      if (uVar5 + 0x98 >> 3 < 0x201) {
        bVar3 = false;
      }
      else {
        iVar4 = (*DAT_0028fae0)(DAT_0028fac8,uVar5 + 0x90,&local_40,8);
        bVar3 = iVar4 == 1;
      }
      bVar2 = false;
      if (0xfff < local_40) {
        bVar2 = bVar3;
      }
      uVar6 = uVar7;
      uVar8 = local_40;
      if (!(bool)(bVar2 & (local_40 & 7) == 0)) {
        uVar8 = 0;
      }
    }
  }
  uVar5 = 0;
  if (((uVar6 == DAT_0028fb70) && (uVar8 == DAT_0028fb78)) &&
     (uVar5 = FUN_0018f750(DAT_0028fb80,uVar8), (int)uVar5 != 0)) {
    uVar5 = 0;
    do {
      uVar7 = uVar5;
      if (uVar7 == 0x17) break;
      iVar4 = FUN_0018f750((&DAT_0028fb88)[uVar7],uVar8);
      uVar5 = uVar7 + 1;
    } while (iVar4 != 0);
    uVar5 = (ulong)(0x16 < uVar7);
  }
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar5);
}

/* ===== FUN_0018edcc @ 0018edcc ===== */

void FUN_0018edcc(long param_1)

{
  ulong uVar1;
  long lVar2;
  bool bVar3;
  bool bVar4;
  int iVar5;
  ulong local_30;
  long local_28;
  
  lVar2 = tpidr_el0;
  local_28 = *(long *)(lVar2 + 0x28);
  local_30 = 0;
  if (param_1 + 8U >> 3 < 0x201) {
    bVar4 = false;
  }
  else {
    iVar5 = (*DAT_0028fae0)(DAT_0028fac8,param_1,&local_30,8);
    bVar4 = iVar5 == 1;
  }
  bVar3 = false;
  if (0xfff < local_30) {
    bVar3 = bVar4;
  }
  uVar1 = local_30;
  if (!(bool)(bVar3 & (local_30 & 7) == 0)) {
    uVar1 = 0;
  }
  if (*(long *)(lVar2 + 0x28) == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar1);
}

/* ===== FUN_0018ee70 @ 0018ee70 ===== */

void FUN_0018ee70(ulong param_1,ulong param_2)

{
  ulong uVar1;
  long lVar2;
  bool bVar3;
  bool bVar4;
  int iVar5;
  uint uVar6;
  ulong local_60;
  long local_58;
  
  lVar2 = tpidr_el0;
  local_58 = *(long *)(lVar2 + 0x28);
  if ((param_1 != param_2) && (param_1 != 0)) {
    uVar6 = 0;
    do {
      local_60 = 0;
      if (param_1 + 0x40 >> 3 < 0x201) {
        bVar4 = false;
      }
      else {
        iVar5 = (*DAT_0028fae0)(DAT_0028fac8,param_1 + 0x38,&local_60,8);
        bVar4 = iVar5 == 1;
      }
      bVar3 = false;
      if (0xfff < local_60) {
        bVar3 = bVar4;
      }
      bVar3 = (bool)(bVar3 & (local_60 & 7) == 0);
      uVar1 = local_60;
      if (!bVar3) {
        uVar1 = 0;
      }
      iVar5 = FUN_0018f750(param_1,uVar1);
    } while ((((iVar5 != 0) && (param_1 = uVar1, uVar1 != param_2)) && (uVar6 < 0x1f)) &&
            (uVar6 = uVar6 + 1, !(bool)(bVar3 ^ 1)));
  }
  if (*(long *)(lVar2 + 0x28) != local_58) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(param_1 == param_2);
  }
  return;
}

/* ===== FUN_0018f750 @ 0018f750 ===== */

void FUN_0018f750(ulong param_1,ulong param_2)

{
  ulong uVar1;
  long lVar2;
  ulong uVar3;
  long lVar4;
  bool bVar5;
  bool bVar6;
  int iVar7;
  ushort local_58 [2];
  int local_54;
  ulong local_50;
  long local_48;
  
  lVar4 = tpidr_el0;
  bVar6 = false;
  local_48 = *(long *)(lVar4 + 0x28);
  local_54 = -1;
  local_58[0] = 0;
  if ((param_1 == 0) || (param_2 == 0)) goto LAB_0018f8b0;
  uVar1 = param_1 + 0x40;
  local_50 = 0;
  if (uVar1 >> 3 < 0x201) {
    bVar6 = false;
  }
  else {
    iVar7 = (*DAT_0028fae0)(DAT_0028fac8,param_1 + 0x38,&local_50,8);
    bVar6 = iVar7 == 1;
  }
  bVar5 = false;
  if (0xfff < local_50) {
    bVar5 = bVar6;
  }
  uVar3 = local_50;
  if (!(bool)(bVar5 & (local_50 & 7) == 0)) {
    uVar3 = 0;
  }
  if (uVar3 == param_2) {
    bVar6 = false;
    if ((uVar1 < 0x1000) || ((param_1 & 0xfffffffffffffffc) == 0xffffffffffffffbc))
    goto LAB_0018f8b0;
    iVar7 = (*DAT_0028fae0)(DAT_0028fac8,uVar1,&local_54,4);
    bVar6 = false;
    if ((iVar7 != 1) || (local_54 < 0)) goto LAB_0018f8b0;
    bVar6 = false;
    if ((param_2 + 0x4e < 0x1000) || ((param_2 & 0xfffffffffffffffe) == 0xffffffffffffffb0))
    goto LAB_0018f8b0;
    iVar7 = (*DAT_0028fae0)(DAT_0028fac8,param_2 + 0x4e,local_58,2);
    if ((iVar7 == 1) &&
       ((local_54 < (int)(uint)local_58[0] && (local_50 = 0, 0x200 < param_2 + 0x58 >> 3)))) {
      iVar7 = (*DAT_0028fae0)(DAT_0028fac8,param_2 + 0x50,&local_50,8);
      uVar1 = local_50;
      bVar6 = false;
      if ((iVar7 == 1) && ((0xfff < local_50 && ((local_50 & 7) == 0)))) {
        local_50 = 0;
        lVar2 = uVar1 + (uint)(local_54 << 3);
        if (lVar2 + 8U >> 3 < 0x201) {
          bVar6 = false;
        }
        else {
          iVar7 = (*DAT_0028fae0)(DAT_0028fac8,lVar2,&local_50,8);
          bVar6 = iVar7 == 1;
        }
        bVar5 = false;
        if (0xfff < local_50) {
          bVar5 = bVar6;
        }
        uVar1 = local_50;
        if (!(bool)(bVar5 & (local_50 & 7) == 0)) {
          uVar1 = 0;
        }
        bVar6 = uVar1 == param_1;
      }
      goto LAB_0018f8b0;
    }
  }
  bVar6 = false;
LAB_0018f8b0:
  if (*(long *)(lVar4 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(bVar6);
}

/* ===== FUN_0018fb04 @ 0018fb04 ===== */

void FUN_0018fb04(char *param_1,undefined8 param_2,undefined8 param_3,char *param_4,
                 undefined8 param_5,undefined8 param_6,undefined8 param_7,undefined8 param_8)

{
  long lVar1;
  int iVar2;
  long lVar3;
  undefined8 local_90;
  undefined8 local_88;
  undefined8 uStack_80;
  undefined8 local_78;
  undefined1 *local_70;
  undefined1 **ppuStack_68;
  undefined8 *puStack_60;
  undefined8 uStack_58;
  
  ppuStack_68 = &local_70;
  puStack_60 = &local_90;
  lVar1 = tpidr_el0;
  lVar3 = *(long *)(lVar1 + 0x28);
  uStack_58 = 0xffffff80ffffffe0;
  local_90 = param_5;
  local_88 = param_6;
  uStack_80 = param_7;
  local_78 = param_8;
  local_70 = (undefined1 *)register0x00000008;
  iVar2 = vsnprintf(param_1,0x80,param_4,&local_70);
  if (*(long *)(lVar1 + 0x28) == lVar3) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar2);
}

/* ===== FUN_0018fba0 @ 0018fba0 ===== */

void FUN_0018fba0(ulong param_1)

{
  long lVar1;
  bool bVar2;
  bool bVar3;
  int iVar4;
  ulong uVar5;
  char local_60 [4];
  int local_5c;
  long local_58;
  long local_50;
  long local_48;
  ulong local_40;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  local_50 = 1;
  local_48 = 1;
  local_58 = 1;
  local_5c = 0;
  local_60[0] = '\x01';
  local_40 = 0;
  if (param_1 + 8 >> 3 < 0x201) {
    bVar3 = false;
  }
  else {
    iVar4 = (*DAT_0028fae0)(DAT_0028fac8,param_1,&local_40,8);
    bVar3 = iVar4 == 1;
  }
  bVar2 = false;
  if (0xfff < local_40) {
    bVar2 = bVar3;
  }
  uVar5 = local_40;
  if (!(bool)(bVar2 & (local_40 & 7) == 0)) {
    uVar5 = 0;
  }
  if (uVar5 == DAT_0028fad0 + 0x11c0a48U) {
    bVar3 = false;
    if ((param_1 + 0x38 < 0x1000) ||
       (uVar5 = param_1 & 0xfffffffffffffff8, uVar5 == 0xffffffffffffffc0)) goto LAB_0018fdbc;
    iVar4 = (*DAT_0028fae0)(DAT_0028fac8,param_1 + 0x38,&local_48,8);
    bVar3 = false;
    if ((iVar4 != 1) || (local_48 != 0)) goto LAB_0018fdbc;
    bVar3 = false;
    if ((param_1 + 0x40 < 0x1000) || ((param_1 & 0xfffffffffffffffc) == 0xffffffffffffffbc))
    goto LAB_0018fdbc;
    iVar4 = (*DAT_0028fae0)(DAT_0028fac8,param_1 + 0x40,&local_5c,4);
    bVar3 = false;
    if ((iVar4 != 1) || (local_5c != -1)) goto LAB_0018fdbc;
    bVar3 = false;
    if ((param_1 + 0xa8 < 0x1000) || (uVar5 == 0xffffffffffffff50)) goto LAB_0018fdbc;
    iVar4 = (*DAT_0028fae0)(DAT_0028fac8,param_1 + 0xa8,&local_50,8);
    bVar3 = false;
    if ((iVar4 != 1) || (local_50 != 0)) goto LAB_0018fdbc;
    bVar3 = false;
    if ((param_1 + 0x120 < 0x1000) || (uVar5 == 0xfffffffffffffed8)) goto LAB_0018fdbc;
    iVar4 = (*DAT_0028fae0)(DAT_0028fac8,param_1 + 0x120,&local_58,8);
    bVar3 = false;
    if ((param_1 + 0x1f1 < 0x1001) || ((iVar4 != 1 || (local_58 != 0)))) goto LAB_0018fdbc;
    iVar4 = (*DAT_0028fae0)(DAT_0028fac8,param_1 + 0x1f0,local_60,1);
    if (iVar4 == 1) {
      bVar3 = local_60[0] == '\0';
      goto LAB_0018fdbc;
    }
  }
  bVar3 = false;
LAB_0018fdbc:
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(bVar3);
}

/* ===== FUN_00190510 @ 00190510 ===== */

void FUN_00190510(undefined8 *param_1,long param_2)

{
  ulong uVar1;
  long lVar2;
  bool bVar3;
  bool bVar4;
  int iVar5;
  ulong local_30;
  long local_28;
  
  lVar2 = tpidr_el0;
  local_28 = *(long *)(lVar2 + 0x28);
  local_30 = 0;
  if (param_2 + 8U >> 3 < 0x201) {
    bVar4 = false;
  }
  else {
    iVar5 = (*(code *)param_1[1])(*param_1,param_2,&local_30,8);
    bVar4 = iVar5 != 0;
  }
  bVar3 = false;
  if (0xfff < local_30) {
    bVar3 = bVar4;
  }
  uVar1 = local_30;
  if (!(bool)(bVar3 & (local_30 & 7) == 0)) {
    uVar1 = 0;
  }
  if (*(long *)(lVar2 + 0x28) == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar1);
}

/* ===== FUN_001905a4 @ 001905a4 ===== */

void FUN_001905a4(undefined8 *param_1,ulong param_2,ulong param_3)

{
  ulong uVar1;
  long lVar2;
  long lVar3;
  bool bVar4;
  bool bVar5;
  int iVar6;
  ulong uVar7;
  ushort local_58 [2];
  int local_54;
  ulong local_50;
  long local_48;
  
  lVar3 = tpidr_el0;
  uVar7 = 0;
  local_48 = *(long *)(lVar3 + 0x28);
  local_54 = -1;
  local_58[0] = 0;
  if ((param_2 == 0) || (param_3 == 0)) goto LAB_001906e4;
  uVar1 = param_2 + 0x40;
  local_50 = 0;
  if (uVar1 >> 3 < 0x201) {
    bVar5 = false;
  }
  else {
    iVar6 = (*(code *)param_1[1])(*param_1,param_2 + 0x38,&local_50,8);
    bVar5 = iVar6 != 0;
  }
  bVar4 = false;
  if (0xfff < local_50) {
    bVar4 = bVar5;
  }
  uVar7 = local_50;
  if (!(bool)(bVar4 & (local_50 & 7) == 0)) {
    uVar7 = 0;
  }
  if (uVar7 == param_3) {
    uVar7 = 0;
    if ((uVar1 < 0x1000) || ((param_2 & 0xfffffffffffffffc) == 0xffffffffffffffbc))
    goto LAB_001906e4;
    iVar6 = (*(code *)param_1[1])(*param_1,uVar1,&local_54,4);
    uVar7 = 0;
    if ((iVar6 == 0) || (local_54 < 0)) goto LAB_001906e4;
    uVar7 = 0;
    if (((param_3 + 0x4e < 0x1000) || ((param_3 & 0xfffffffffffffffe) == 0xffffffffffffffb0)) ||
       (uVar7 = (*(code *)param_1[1])(*param_1,param_3 + 0x4e,local_58,2), (int)uVar7 == 0))
    goto LAB_001906e4;
    if ((local_54 < (int)(uint)local_58[0]) && (local_50 = 0, 0x200 < param_3 + 0x58 >> 3)) {
      iVar6 = (*(code *)param_1[1])(*param_1,param_3 + 0x50,&local_50,8);
      uVar1 = local_50;
      uVar7 = 0;
      if ((iVar6 != 0) && ((0xfff < local_50 && ((local_50 & 7) == 0)))) {
        local_50 = 0;
        lVar2 = uVar1 + (uint)(local_54 << 3);
        if (lVar2 + 8U >> 3 < 0x201) {
          bVar5 = false;
        }
        else {
          iVar6 = (*(code *)param_1[1])(*param_1,lVar2,&local_50,8);
          bVar5 = iVar6 != 0;
        }
        bVar4 = false;
        if (0xfff < local_50) {
          bVar4 = bVar5;
        }
        uVar7 = local_50;
        if (!(bool)(bVar4 & (local_50 & 7) == 0)) {
          uVar7 = 0;
        }
        uVar7 = (ulong)(uVar7 == param_2);
      }
      goto LAB_001906e4;
    }
  }
  uVar7 = 0;
LAB_001906e4:
  if (*(long *)(lVar3 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar7);
}

/* ===== FUN_001907b0 @ 001907b0 ===== */

void FUN_001907b0(undefined8 *param_1,ulong param_2,ulong param_3)

{
  ulong uVar1;
  long lVar2;
  bool bVar3;
  bool bVar4;
  int iVar5;
  uint uVar6;
  ulong local_60;
  long local_58;
  
  lVar2 = tpidr_el0;
  local_58 = *(long *)(lVar2 + 0x28);
  if ((param_2 != param_3) && (param_2 != 0)) {
    uVar6 = 0;
    do {
      local_60 = 0;
      if (param_2 + 0x40 >> 3 < 0x201) {
        bVar4 = false;
      }
      else {
        iVar5 = (*(code *)param_1[1])(*param_1,param_2 + 0x38,&local_60,8);
        bVar4 = iVar5 != 0;
      }
      bVar3 = false;
      if (0xfff < local_60) {
        bVar3 = bVar4;
      }
      bVar3 = (bool)(bVar3 & (local_60 & 7) == 0);
      uVar1 = local_60;
      if (!bVar3) {
        uVar1 = 0;
      }
      iVar5 = FUN_001905a4(param_1,param_2,uVar1);
    } while ((((iVar5 != 0) && (param_2 = uVar1, uVar1 != param_3)) && (uVar6 < 0x1f)) &&
            (uVar6 = uVar6 + 1, !(bool)(bVar3 ^ 1)));
  }
  if (*(long *)(lVar2 + 0x28) != local_58) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(param_2 == param_3);
  }
  return;
}

/* ===== FUN_001908c0 @ 001908c0 ===== */

void FUN_001908c0(long *param_1,undefined8 *param_2,ulong param_3)

{
  long lVar1;
  bool bVar2;
  bool bVar3;
  int iVar4;
  ulong uVar5;
  ulong uVar6;
  char local_60 [4];
  int local_5c;
  long local_58;
  long local_50;
  long local_48;
  ulong local_40;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  local_50 = 1;
  local_48 = 1;
  local_58 = 1;
  local_5c = 0;
  local_60[0] = '\x01';
  if (param_3 != 0) {
    local_40 = 0;
    if (param_3 + 8 >> 3 < 0x201) {
      bVar3 = false;
    }
    else {
      iVar4 = (*(code *)param_2[1])(*param_2,param_3,&local_40,8);
      bVar3 = iVar4 != 0;
    }
    bVar2 = false;
    if (0xfff < local_40) {
      bVar2 = bVar3;
    }
    uVar5 = local_40;
    if (!(bool)(bVar2 & (local_40 & 7) == 0)) {
      uVar5 = 0;
    }
    if (uVar5 == *param_1 + 0x11c0a48U) {
      uVar5 = 0;
      if ((0xfff < param_3 + 0x38) &&
         (uVar6 = param_3 & 0xfffffffffffffff8, uVar6 != 0xffffffffffffffc0)) {
        iVar4 = (*(code *)param_2[1])(*param_2,param_3 + 0x38,&local_48,8);
        uVar5 = 0;
        if ((iVar4 != 0) && (local_48 == 0)) {
          uVar5 = 0;
          if ((0xfff < param_3 + 0x40) && ((param_3 & 0xfffffffffffffffc) != 0xffffffffffffffbc)) {
            iVar4 = (*(code *)param_2[1])(*param_2,param_3 + 0x40,&local_5c,4);
            uVar5 = 0;
            if ((iVar4 != 0) && (local_5c == -1)) {
              uVar5 = 0;
              if ((0xfff < param_3 + 0xa8) && (uVar6 != 0xffffffffffffff50)) {
                iVar4 = (*(code *)param_2[1])(*param_2,param_3 + 0xa8,&local_50,8);
                uVar5 = 0;
                if ((iVar4 != 0) && (local_50 == 0)) {
                  uVar5 = 0;
                  if ((0xfff < param_3 + 0x120) && (uVar6 != 0xfffffffffffffed8)) {
                    iVar4 = (*(code *)param_2[1])(*param_2,param_3 + 0x120,&local_58,8);
                    uVar5 = 0;
                    if ((0x1000 < param_3 + 0x1f1) &&
                       (((iVar4 != 0 && (local_58 == 0)) &&
                        (uVar5 = (*(code *)param_2[1])(*param_2,param_3 + 0x1f0,local_60,1),
                        (int)uVar5 != 0)))) {
                      uVar5 = (ulong)(local_60[0] == '\0');
                    }
                  }
                }
              }
            }
          }
        }
      }
      goto LAB_00190aa8;
    }
  }
  uVar5 = 0;
LAB_00190aa8:
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar5);
}

/* ===== FUN_00190d68 @ 00190d68 ===== */

undefined8 FUN_00190d68(ulong param_1,ulong param_2,uint *param_3)

{
  ulong uVar1;
  ulong uVar2;
  undefined8 uVar3;
  
  uVar3 = 0;
  if ((param_3 != (uint *)0x0) && ((((uint)param_2 | (uint)param_1) & 3) == 0)) {
    uVar2 = param_2 - param_1;
    if ((param_1 <= param_2) && (uVar2 >> 0x1b != 0)) {
      return 0;
    }
    if ((param_2 < param_1) && (0x8000000 < param_1 - param_2)) {
      return 0;
    }
    uVar1 = uVar2 + 3;
    if (-1 < (long)uVar2) {
      uVar1 = uVar2;
    }
    uVar3 = 1;
    *param_3 = (uint)uVar1 >> 2 & 0x3ffffff | 0x14000000;
  }
  return uVar3;
}

/* ===== FUN_00190dd4 @ 00190dd4 ===== */

void FUN_00190dd4(ulong *param_1,undefined8 *param_2)

{
  long lVar1;
  int iVar2;
  ulong uVar3;
  ulong uVar4;
  ulong *puVar5;
  int local_4c;
  long local_48;
  
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  uVar4 = *param_1;
  uVar3 = 0;
  puVar5 = param_1 + 2;
  local_4c = 0;
  if (uVar4 == *puVar5) goto LAB_00190f3c;
  if (((uVar4 & 3) == 0) && (*(uint *)((long)param_1 + 0xc) >> 0x1a == 5)) {
    uVar3 = (*(code *)param_2[1])(*param_2,uVar4,&local_4c);
    if ((int)uVar3 == 0) goto LAB_00190f3c;
    if (((local_4c == (int)param_1[1]) && ((*puVar5 & 3) == 0)) &&
       (*(uint *)((long)param_1 + 0x1c) >> 0x1a == 5)) {
      uVar3 = (*(code *)param_2[1])(*param_2,*puVar5,&local_4c);
      if ((int)uVar3 == 0) goto LAB_00190f3c;
      if (local_4c == (int)param_1[3]) {
        iVar2 = (*(code *)param_2[2])(*param_2,*param_1,*(undefined4 *)((long)param_1 + 0xc));
        (*(code *)param_2[3])(*param_2,*param_1);
        if (((iVar2 == 0) ||
            (iVar2 = (*(code *)param_2[1])(*param_2,*param_1,&local_4c), iVar2 == 0)) ||
           (local_4c != *(int *)((long)param_1 + 0xc))) {
          uVar3 = 0;
          puVar5 = param_1;
        }
        else {
          iVar2 = (*(code *)param_2[2])(*param_2,param_1[2],*(undefined4 *)((long)param_1 + 0x1c));
          (*(code *)param_2[3])(*param_2,param_1[2]);
          if ((iVar2 == 0) ||
             (iVar2 = (*(code *)param_2[1])(*param_2,*puVar5,&local_4c), iVar2 == 0)) {
            uVar3 = 1;
          }
          else {
            uVar3 = 1;
            if (local_4c == *(int *)((long)param_1 + 0x1c)) goto LAB_00190f3c;
          }
        }
        (*(code *)param_2[2])(*param_2,*puVar5,(int)param_1[uVar3 * 2 + 1]);
        (*(code *)param_2[3])(*param_2,*puVar5);
        iVar2 = (*(code *)param_2[1])(*param_2,*puVar5,&local_4c);
        if (iVar2 == 0) {
          uVar3 = 0xffffffff;
        }
        else {
          uVar3 = (ulong)-(uint)(local_4c != (int)param_1[uVar3 * 2 + 1]);
        }
        goto LAB_00190f3c;
      }
    }
  }
  uVar3 = 0;
LAB_00190f3c:
  if (*(long *)(lVar1 + 0x28) != local_48) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar3);
  }
  return;
}

/* ===== FUN_00190fd0 @ 00190fd0 ===== */

undefined8 FUN_00190fd0(char *param_1,long *param_2)

{
  undefined8 uVar1;
  size_t __n;
  
  uVar1 = 0;
  if ((param_1 != (char *)0x0) && (param_2 != (long *)0x0)) {
    __n = strlen(param_1);
    if (__n < 0x101) {
      *param_2 = 0;
      param_2[1] = 0;
      param_2[2] = 0;
      if (__n < 0x17) {
        *(char *)param_2 = (char)((int)__n << 1);
        memcpy((void *)((long)param_2 + 1),param_1,__n);
        uVar1 = 1;
      }
      else {
        uVar1 = 1;
        param_2[2] = (long)param_1;
        *param_2 = (__n | 0xf) + 2;
        param_2[1] = __n;
      }
    }
    else {
      uVar1 = 0;
    }
  }
  return uVar1;
}

/* ===== FUN_00191058 @ 00191058 ===== */

void FUN_00191058(ulong param_1,code *param_2,undefined8 param_3,long param_4,ulong param_5)

{
  long lVar1;
  bool bVar2;
  int iVar3;
  ulong uVar4;
  long *plVar5;
  undefined1 auStack_4f8 [112];
  long local_488;
  long lStack_480;
  long local_478;
  long lStack_470;
  undefined1 auStack_468 [1024];
  long local_68;
  
  lVar1 = tpidr_el0;
  local_68 = *(long *)(lVar1 + 0x28);
  bVar2 = false;
  if ((param_1 != 0) && (param_5 != 0)) {
    bVar2 = false;
    uVar4 = 0;
    plVar5 = (long *)(param_4 + 0xc);
    do {
      if ((((*(uint *)((long)plVar5 + -4) == 0) || (0x400 < *(uint *)((long)plVar5 + -4))) ||
          (CARRY8(*(ulong *)((long)plVar5 + -0xc),param_1))) ||
         (iVar3 = (*param_2)(param_3,*(ulong *)((long)plVar5 + -0xc) + param_1,auStack_468),
         iVar3 == 0)) break;
      FUN_00191314(auStack_4f8);
      FUN_00191338(auStack_4f8,auStack_468,*(undefined4 *)((long)plVar5 + -4));
      FUN_00191678(auStack_4f8,&local_488);
      if (((local_488 != *plVar5 || lStack_480 != plVar5[1]) || local_478 != plVar5[2]) ||
          lStack_470 != plVar5[3]) break;
      uVar4 = uVar4 + 1;
      plVar5 = plVar5 + 6;
      bVar2 = param_5 <= uVar4;
    } while (param_5 != uVar4);
  }
  if (*(long *)(lVar1 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(bVar2);
}

/* ===== FUN_00191170 @ 00191170 ===== */

void FUN_00191170(ulong param_1,code *param_2,undefined8 param_3)

{
  long lVar1;
  int iVar2;
  ulong uVar3;
  ulong local_50;
  long local_48;
  
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  local_50 = 0;
  if (param_1 < 0xfffffffffee3f448) {
    uVar3 = (*param_2)(param_3,param_1 + 0x11c0bb8,&local_50,8);
    if (((((int)uVar3 == 0) || (uVar3 = 0, local_50 != param_1 + 0x772ba0)) ||
        (0xfffffffffee3f407 < param_1)) ||
       (uVar3 = (*param_2)(param_3,param_1 + 0x11c0bf8,&local_50,8), (int)uVar3 == 0))
    goto LAB_001912e8;
    if (local_50 == param_1 + 0x88836c) {
      uVar3 = (*param_2)(param_3,param_1 + 0x11c0ba8,&local_50,8);
      if ((int)uVar3 == 0) goto LAB_001912e8;
      if (local_50 == param_1 + 0x888218) {
        uVar3 = (*param_2)(param_3,param_1 + 0x11abb60,&local_50,8);
        if ((((int)uVar3 != 0) && (uVar3 = 0, local_50 == param_1 + 0x592300)) &&
           (param_1 < 0xfffffffffedc28a8)) {
          iVar2 = (*param_2)(param_3,param_1 + 0x123d758,&local_50,8);
          uVar3 = 0;
          if ((iVar2 != 0) && (0xfff < local_50)) {
            uVar3 = (ulong)((local_50 & 3) == 0);
          }
        }
        goto LAB_001912e8;
      }
    }
  }
  uVar3 = 0;
LAB_001912e8:
  if (*(long *)(lVar1 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar3);
}

/* ===== FUN_00191314 @ 00191314 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00191314(undefined8 *param_1)

{
  undefined8 uVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  
  uVar4 = _UNK_0013ce10;
  uVar3 = _DAT_0013ce08;
  uVar2 = _UNK_0013ce00;
  uVar1 = _DAT_0013cdf8;
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

/* ===== FUN_00191338 @ 00191338 ===== */

void FUN_00191338(uint *param_1,void *param_2,ulong param_3)

{
  long lVar1;
  int iVar2;
  size_t __n;
  uint uVar3;
  uint uVar4;
  long lVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  uint uVar11;
  undefined8 uVar12;
  undefined8 uVar13;
  undefined8 uVar14;
  undefined8 uVar15;
  uint uVar16;
  uint uVar17;
  uint uVar18;
  uint uVar19;
  uint uVar20;
  long lVar21;
  uint uVar22;
  undefined8 uVar23;
  undefined8 uVar24;
  undefined8 uVar25;
  undefined8 uVar26;
  undefined8 local_190;
  undefined8 uStack_188;
  undefined8 uStack_180;
  undefined8 uStack_178;
  undefined8 local_170;
  undefined8 uStack_168;
  undefined8 uStack_160;
  undefined8 local_158;
  int local_150 [48];
  long local_90;
  
  lVar5 = tpidr_el0;
  local_90 = *(long *)(lVar5 + 0x28);
  *(ulong *)(param_1 + 8) = *(long *)(param_1 + 8) + param_3;
  uVar15 = DAT_0010f8f0;
  uVar14 = DAT_0010f870;
  uVar13 = DAT_0010f7d0;
  uVar12 = DAT_0010f750;
  if (param_3 != 0) {
    lVar21 = *(long *)(param_1 + 10);
    do {
      __n = 0x40U - lVar21;
      if (param_3 <= 0x40U - lVar21) {
        __n = param_3;
      }
      memcpy((void *)((long)(param_1 + 0xc) + lVar21),param_2,__n);
      param_3 = param_3 - __n;
      lVar21 = *(long *)(param_1 + 10) + __n;
      *(long *)(param_1 + 10) = lVar21;
      if (lVar21 == 0x40) {
        uVar20 = CONCAT13((char)param_1[0xc],
                          CONCAT12(*(undefined1 *)((long)param_1 + 0x31),
                                   CONCAT11(*(undefined1 *)((long)param_1 + 0x32),
                                            *(undefined1 *)((long)param_1 + 0x33))));
        uStack_188 = CONCAT17((char)param_1[0xf],
                              CONCAT16(*(undefined1 *)((long)param_1 + 0x3d),
                                       CONCAT15(*(undefined1 *)((long)param_1 + 0x3e),
                                                CONCAT14(*(undefined1 *)((long)param_1 + 0x3f),
                                                         CONCAT13((char)param_1[0xe],
                                                                  CONCAT12(*(undefined1 *)
                                                                            ((long)param_1 + 0x39),
                                                                           CONCAT11(*(undefined1 *)
                                                                                     ((long)param_1
                                                                                     + 0x3a),*(
                                                  undefined1 *)((long)param_1 + 0x3b))))))));
        local_190 = CONCAT17((char)param_1[0xd],
                             CONCAT16(*(undefined1 *)((long)param_1 + 0x35),
                                      CONCAT15(*(undefined1 *)((long)param_1 + 0x36),
                                               CONCAT14(*(undefined1 *)((long)param_1 + 0x37),uVar20
                                                       ))));
        uStack_178 = CONCAT17((char)param_1[0x13],
                              CONCAT16(*(undefined1 *)((long)param_1 + 0x4d),
                                       CONCAT15(*(undefined1 *)((long)param_1 + 0x4e),
                                                CONCAT14(*(undefined1 *)((long)param_1 + 0x4f),
                                                         CONCAT13((char)param_1[0x12],
                                                                  CONCAT12(*(undefined1 *)
                                                                            ((long)param_1 + 0x49),
                                                                           CONCAT11(*(undefined1 *)
                                                                                     ((long)param_1
                                                                                     + 0x4a),*(
                                                  undefined1 *)((long)param_1 + 0x4b))))))));
        uStack_180 = CONCAT17((char)param_1[0x11],
                              CONCAT16(*(undefined1 *)((long)param_1 + 0x45),
                                       CONCAT15(*(undefined1 *)((long)param_1 + 0x46),
                                                CONCAT14(*(undefined1 *)((long)param_1 + 0x47),
                                                         CONCAT13((char)param_1[0x10],
                                                                  CONCAT12(*(undefined1 *)
                                                                            ((long)param_1 + 0x41),
                                                                           CONCAT11(*(undefined1 *)
                                                                                     ((long)param_1
                                                                                     + 0x42),*(
                                                  undefined1 *)((long)param_1 + 0x43))))))));
        uStack_168 = CONCAT17((char)param_1[0x17],
                              CONCAT16(*(undefined1 *)((long)param_1 + 0x5d),
                                       CONCAT15(*(undefined1 *)((long)param_1 + 0x5e),
                                                CONCAT14(*(undefined1 *)((long)param_1 + 0x5f),
                                                         CONCAT13((char)param_1[0x16],
                                                                  CONCAT12(*(undefined1 *)
                                                                            ((long)param_1 + 0x59),
                                                                           CONCAT11(*(undefined1 *)
                                                                                     ((long)param_1
                                                                                     + 0x5a),*(
                                                  undefined1 *)((long)param_1 + 0x5b))))))));
        local_170 = CONCAT17((char)param_1[0x15],
                             CONCAT16(*(undefined1 *)((long)param_1 + 0x55),
                                      CONCAT15(*(undefined1 *)((long)param_1 + 0x56),
                                               CONCAT14(*(undefined1 *)((long)param_1 + 0x57),
                                                        CONCAT13((char)param_1[0x14],
                                                                 CONCAT12(*(undefined1 *)
                                                                           ((long)param_1 + 0x51),
                                                                          CONCAT11(*(undefined1 *)
                                                                                    ((long)param_1 +
                                                                                    0x52),*(
                                                  undefined1 *)((long)param_1 + 0x53))))))));
        local_158 = CONCAT17((char)param_1[0x1b],
                             CONCAT16(*(undefined1 *)((long)param_1 + 0x6d),
                                      CONCAT15(*(undefined1 *)((long)param_1 + 0x6e),
                                               CONCAT14(*(undefined1 *)((long)param_1 + 0x6f),
                                                        CONCAT13((char)param_1[0x1a],
                                                                 CONCAT12(*(undefined1 *)
                                                                           ((long)param_1 + 0x69),
                                                                          CONCAT11(*(undefined1 *)
                                                                                    ((long)param_1 +
                                                                                    0x6a),*(
                                                  undefined1 *)((long)param_1 + 0x6b))))))));
        uStack_160 = CONCAT17((char)param_1[0x19],
                              CONCAT16(*(undefined1 *)((long)param_1 + 0x65),
                                       CONCAT15(*(undefined1 *)((long)param_1 + 0x66),
                                                CONCAT14(*(undefined1 *)((long)param_1 + 0x67),
                                                         CONCAT13((char)param_1[0x18],
                                                                  CONCAT12(*(undefined1 *)
                                                                            ((long)param_1 + 0x61),
                                                                           CONCAT11(*(undefined1 *)
                                                                                     ((long)param_1
                                                                                     + 0x62),*(
                                                  undefined1 *)((long)param_1 + 99))))))));
        lVar21 = 0;
        do {
          lVar1 = lVar21 + 4;
          uVar3 = *(uint *)((long)&local_190 + lVar21 + 4);
          uVar4 = *(uint *)((long)local_150 + lVar21 + -8);
          uVar25 = NEON_ushl(CONCAT44(uVar3,uVar3),uVar15,4);
          uVar23 = NEON_ushl(CONCAT44(uVar3,uVar3),uVar14,4);
          uVar26 = NEON_ushl(CONCAT44(uVar4,uVar4),uVar12,4);
          uVar24 = NEON_ushl(CONCAT44(uVar4,uVar4),uVar13,4);
          *(uint *)((long)local_150 + lVar21) =
               *(int *)((long)&local_170 + lVar21 + 4) + uVar20 +
               (CONCAT13(((byte)((ulong)uVar23 >> 0x18) | (byte)((ulong)uVar25 >> 0x18)) ^
                         ((byte)((ulong)uVar23 >> 0x38) | (byte)((ulong)uVar25 >> 0x38)),
                         CONCAT12(((byte)((ulong)uVar23 >> 0x10) | (byte)((ulong)uVar25 >> 0x10)) ^
                                  ((byte)((ulong)uVar23 >> 0x30) | (byte)((ulong)uVar25 >> 0x30)),
                                  CONCAT11(((byte)((ulong)uVar23 >> 8) | (byte)((ulong)uVar25 >> 8))
                                           ^ ((byte)((ulong)uVar23 >> 0x28) |
                                             (byte)((ulong)uVar25 >> 0x28)),
                                           ((byte)uVar23 | (byte)uVar25) ^
                                           ((byte)((ulong)uVar23 >> 0x20) |
                                           (byte)((ulong)uVar25 >> 0x20))))) ^ uVar3 >> 3) +
               (CONCAT13(((byte)((ulong)uVar24 >> 0x18) | (byte)((ulong)uVar26 >> 0x18)) ^
                         ((byte)((ulong)uVar24 >> 0x38) | (byte)((ulong)uVar26 >> 0x38)),
                         CONCAT12(((byte)((ulong)uVar24 >> 0x10) | (byte)((ulong)uVar26 >> 0x10)) ^
                                  ((byte)((ulong)uVar24 >> 0x30) | (byte)((ulong)uVar26 >> 0x30)),
                                  CONCAT11(((byte)((ulong)uVar24 >> 8) | (byte)((ulong)uVar26 >> 8))
                                           ^ ((byte)((ulong)uVar24 >> 0x28) |
                                             (byte)((ulong)uVar26 >> 0x28)),
                                           ((byte)uVar24 | (byte)uVar26) ^
                                           ((byte)((ulong)uVar24 >> 0x20) |
                                           (byte)((ulong)uVar26 >> 0x20))))) ^ uVar4 >> 10);
          lVar21 = lVar1;
          uVar20 = uVar3;
        } while (lVar1 != 0xc0);
        lVar21 = 0;
        uVar16 = param_1[2];
        uVar17 = param_1[1];
        uVar4 = param_1[3];
        uVar8 = param_1[7];
        uVar18 = param_1[6];
        uVar19 = param_1[5];
        uVar3 = *param_1;
        uVar20 = param_1[4];
        do {
          uVar11 = uVar20;
          uVar22 = uVar3;
          uVar10 = uVar19;
          uVar9 = uVar18;
          uVar7 = uVar17;
          uVar6 = uVar16;
          iVar2 = (uVar11 & uVar10) + uVar8 + (uVar9 & (uVar11 ^ 0xffffffff)) +
                  *(int *)((long)&DAT_00140dc0 + lVar21) +
                  ((uVar11 >> 6 | uVar11 << 0x1a) ^ (uVar11 >> 0xb | uVar11 << 0x15) ^
                  (uVar11 >> 0x19 | uVar11 << 7)) + *(int *)((long)&local_190 + lVar21);
          uVar20 = iVar2 + uVar4;
          uVar3 = ((uVar22 >> 2 | uVar22 << 0x1e) ^ (uVar22 >> 0xd | uVar22 << 0x13) ^
                  (uVar22 >> 0x16 | uVar22 << 10)) + ((uVar7 ^ uVar6) & uVar22 ^ uVar7 & uVar6) +
                  iVar2;
          lVar21 = lVar21 + 4;
          uVar16 = uVar7;
          uVar17 = uVar22;
          uVar4 = uVar6;
          uVar8 = uVar9;
          uVar18 = uVar10;
          uVar19 = uVar11;
        } while (lVar21 != 0x100);
        lVar21 = 0;
        param_1[2] = uVar7 + param_1[2];
        param_1[3] = uVar6 + param_1[3];
        *param_1 = uVar3 + *param_1;
        param_1[1] = uVar22 + param_1[1];
        param_1[4] = uVar20 + param_1[4];
        param_1[5] = uVar11 + param_1[5];
        param_1[6] = uVar10 + param_1[6];
        param_1[7] = uVar9 + param_1[7];
        param_1[10] = 0;
        param_1[0xb] = 0;
      }
      param_2 = (void *)((long)param_2 + __n);
    } while (param_3 != 0);
  }
  if (*(long *)(lVar5 + 0x28) == local_90) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00191678 @ 00191678 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00191678(long param_1,long param_2)

{
  uint uVar1;
  long lVar2;
  uint uVar3;
  ulong uVar4;
  long lVar5;
  long lVar6;
  undefined1 auVar7 [16];
  undefined1 auVar8 [16];
  undefined1 auVar9 [16];
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
  undefined8 local_60;
  undefined8 uStack_58;
  undefined8 uStack_50;
  undefined8 uStack_48;
  long local_38;
  
  lVar2 = tpidr_el0;
  local_38 = *(long *)(lVar2 + 0x28);
  uVar4 = *(ulong *)(param_1 + 0x20);
  auVar9._8_8_ = _UNK_0010fba8;
  auVar9._0_8_ = _DAT_0010fba0;
  lVar6 = uVar4 << 3;
  uStack_b8 = 0;
  uStack_a8 = 0;
  uStack_b0 = 0;
  lVar5 = 0x38;
  if (0x37 < *(ulong *)(param_1 + 0x28)) {
    lVar5 = 0x78;
  }
  auVar8._8_8_ = lVar6;
  auVar8._0_8_ = lVar6;
  lVar5 = lVar5 - *(ulong *)(param_1 + 0x28);
  auVar7 = NEON_ushl(auVar8,_DAT_0010fa50,8);
  uStack_98 = 0;
  local_a0 = 0;
  uStack_88 = 0;
  uStack_90 = 0;
  auVar9 = NEON_ushl(auVar8,auVar9,8);
  uStack_78 = 0;
  local_80 = 0;
  uStack_68 = 0;
  uStack_70 = 0;
  uStack_58 = 0;
  local_60 = 0;
  uStack_48 = 0;
  uStack_50 = 0;
  local_c0 = 0x80;
  *(char *)((long)&local_c0 + lVar5 + 4) = (char)(uVar4 >> 0x15);
  *(char *)((long)&local_c0 + lVar5 + 7) = (char)lVar6;
  *(char *)((long)&local_c0 + lVar5 + 5) = (char)(uVar4 >> 0xd);
  *(char *)((long)&local_c0 + lVar5 + 6) = (char)(uVar4 >> 5);
  *(uint *)((long)&local_c0 + lVar5) =
       CONCAT13(auVar9[8],CONCAT12(auVar9[0],CONCAT11(auVar7[8],auVar7[0])));
  FUN_00191338(param_1,&local_c0,lVar5 + 8);
  uVar3 = 0;
  uVar4 = 0;
  do {
    uVar1 = uVar3 ^ 0xffffffff;
    uVar3 = uVar3 + 8;
    *(char *)(param_2 + uVar4) =
         (char)(*(uint *)(param_1 + (uVar4 & 0xfffffffc)) >> (ulong)(uVar1 & 0x18));
    uVar4 = uVar4 + 1;
  } while (uVar4 != 0x20);
  if (*(long *)(lVar2 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00191920 @ 00191920 ===== */

undefined8 FUN_00191920(undefined4 *param_1,long param_2)

{
  undefined8 uVar1;
  
  if ((param_1 != (undefined4 *)0x0) && (param_2 == 0x918)) {
    memset(param_1,0,0x918);
    *param_1 = 0x918;
    param_1[4] = 0xffffffff;
    FUN_0019198c();
    if (DAT_002a1e58 != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x00191978. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      uVar1 = (*DAT_002a1e58)(param_1,0x918);
      return uVar1;
    }
  }
  return 0;
}

/* ===== FUN_0019198c @ 0019198c ===== */

void FUN_0019198c(void)

{
  ulong uVar1;
  undefined4 uVar2;
  long lVar3;
  int iVar4;
  undefined4 *puVar5;
  undefined8 local_140;
  long lStack_138;
  long local_130;
  undefined8 local_128;
  undefined8 local_120;
  undefined8 uStack_118;
  undefined8 uStack_110;
  undefined8 local_108;
  undefined8 local_100;
  undefined8 uStack_f8;
  undefined8 uStack_f0;
  undefined8 local_e8;
  undefined8 local_e0;
  undefined8 uStack_d8;
  undefined8 uStack_d0;
  undefined8 local_c8;
  undefined8 local_c0;
  undefined8 uStack_b8;
  undefined8 uStack_b0;
  undefined8 local_a8;
  undefined8 local_a0;
  undefined8 uStack_98;
  undefined8 uStack_90;
  undefined8 local_88;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined8 uStack_70;
  undefined8 local_68;
  undefined8 local_60;
  timespec local_58;
  long local_48;
  
  lVar3 = tpidr_el0;
  local_48 = *(long *)(lVar3 + 0x28);
  if (DAT_002a1f28 == 0 || DAT_002a1f30 == 0) {
    puVar5 = (undefined4 *)__errno();
    uVar2 = *puVar5;
    iVar4 = clock_gettime(1,&local_58);
    if ((iVar4 == 0) &&
       (uVar1 = local_58.tv_sec * 1000 + (ulong)local_58.tv_nsec / 1000000, DAT_002a1f38 <= uVar1))
    {
      DAT_002a1f38 = uVar1 + 500;
      local_60 = 0;
      lStack_138 = 0;
      local_140 = 0;
      local_128 = 0;
      local_130 = 0;
      uStack_118 = 0;
      local_120 = 0;
      local_108 = 0;
      uStack_110 = 0;
      uStack_f8 = 0;
      local_100 = 0;
      local_e8 = 0;
      uStack_f0 = 0;
      uStack_d8 = 0;
      local_e0 = 0;
      local_c8 = 0;
      uStack_d0 = 0;
      uStack_b8 = 0;
      local_c0 = 0;
      local_a8 = 0;
      uStack_b0 = 0;
      uStack_98 = 0;
      local_a0 = 0;
      local_88 = 0;
      uStack_90 = 0;
      uStack_78 = 0;
      local_80 = 0;
      local_68 = 0;
      uStack_70 = 0;
      dl_iterate_phdr(FUN_0019265c,&local_140);
      if (((int)local_140 == 1) && ((lStack_138 != 0 && (local_130 != 0)))) {
        DAT_002a1f28 = lStack_138;
        DAT_002a1f30 = local_130;
        DAT_002a1f08 = local_128;
        DAT_002a1f18 = local_120;
        DAT_002a1f10 = uStack_118;
        DAT_002a1eb8 = local_108;
        DAT_002a1f20 = uStack_110;
        DAT_002a1e58 = local_100;
        DAT_002a1e60 = uStack_f8;
        DAT_002a1e68 = uStack_f0;
        DAT_002a1e70 = local_e8;
        DAT_002a1ef8 = local_e0;
        DAT_002a1f00 = uStack_d8;
        DAT_002a1ed8 = uStack_d0;
        DAT_002a1ee0 = local_c8;
        DAT_002a1ee8 = local_c0;
        DAT_002a1ef0 = uStack_b8;
        DAT_002a1e78 = uStack_b0;
        DAT_002a1e80 = local_a8;
        DAT_002a1ec0 = local_a0;
        DAT_002a1ec8 = uStack_98;
        DAT_002a1ed0 = uStack_90;
        DAT_002a1e88 = local_88;
        DAT_002a1e90 = local_80;
        DAT_002a1e98 = uStack_78;
        DAT_002a1ea0 = uStack_70;
        DAT_002a1ea8 = local_68;
        DAT_002a1eb0 = local_60;
      }
    }
    *puVar5 = uVar2;
  }
  if (*(long *)(lVar3 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00191bb0 @ 00191bb0 ===== */

bool FUN_00191bb0(undefined4 param_1)

{
  bool bVar1;
  int iVar2;
  
  FUN_0019198c();
  if (DAT_002a1e60 == (code *)0x0) {
    bVar1 = false;
  }
  else {
    iVar2 = (*DAT_002a1e60)(param_1);
    bVar1 = iVar2 != 0;
  }
  return bVar1;
}

/* ===== FUN_00191bf4 @ 00191bf4 ===== */

undefined8 FUN_00191bf4(undefined8 param_1)

{
  undefined8 uVar1;
  
  FUN_0019198c();
  if (DAT_002a1e68 != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x00191c20. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar1 = (*DAT_002a1e68)(param_1);
    return uVar1;
  }
  return 0;
}

/* ===== FUN_00191c34 @ 00191c34 ===== */

undefined8 FUN_00191c34(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined8 uVar1;
  
  FUN_0019198c();
  if (DAT_002a1e70 != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x00191c78. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar1 = (*DAT_002a1e70)(param_1,param_2,param_3);
    return uVar1;
  }
  return 0;
}

/* ===== FUN_00191c90 @ 00191c90 ===== */

undefined8 FUN_00191c90(undefined8 param_1)

{
  undefined8 uVar1;
  
  FUN_0019198c();
  if (DAT_002a1e78 != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x00191cbc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar1 = (*DAT_002a1e78)(param_1);
    return uVar1;
  }
  return 0;
}

/* ===== FUN_00191cd0 @ 00191cd0 ===== */

undefined8 FUN_00191cd0(undefined4 param_1)

{
  undefined8 uVar1;
  
  FUN_0019198c();
  if (DAT_002a1e80 != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x00191cfc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar1 = (*DAT_002a1e80)(param_1);
    return uVar1;
  }
  return 0;
}

/* ===== FUN_00191d10 @ 00191d10 ===== */

undefined8 FUN_00191d10(undefined8 param_1,undefined8 param_2)

{
  undefined8 uVar1;
  
  FUN_0019198c();
  if (DAT_002a1e88 != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x00191d44. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar1 = (*DAT_002a1e88)(param_1,param_2);
    return uVar1;
  }
  return 0;
}

/* ===== FUN_00191d58 @ 00191d58 ===== */

undefined8
FUN_00191d58(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
            undefined4 param_5)

{
  undefined8 uVar1;
  
  FUN_0019198c();
  if (DAT_002a1e90 != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x00191db4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar1 = (*DAT_002a1e90)(param_1,param_2,param_3,param_4,param_5);
    return uVar1;
  }
  return 0;
}

/* ===== FUN_00191dd0 @ 00191dd0 ===== */

void FUN_00191dd0(void)

{
  FUN_0019198c();
  if (DAT_002a1e98 != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x00191dec. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*DAT_002a1e98)();
    return;
  }
  return;
}

/* ===== FUN_00191df8 @ 00191df8 ===== */

undefined8 FUN_00191df8(undefined8 param_1)

{
  undefined8 uVar1;
  
  FUN_0019198c();
  if (DAT_002a1ea0 != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x00191e24. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar1 = (*DAT_002a1ea0)(param_1);
    return uVar1;
  }
  return 0;
}

/* ===== FUN_00191e38 @ 00191e38 ===== */

undefined8 FUN_00191e38(undefined4 param_1)

{
  undefined8 uVar1;
  
  FUN_0019198c();
  if (DAT_002a1ea8 != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x00191e64. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar1 = (*DAT_002a1ea8)(param_1);
    return uVar1;
  }
  return 0;
}

/* ===== FUN_00191e78 @ 00191e78 ===== */

void FUN_00191e78(void)

{
  FUN_0019198c();
  if (DAT_002a1eb0 != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x00191e94. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*DAT_002a1eb0)();
    return;
  }
  return;
}

/* ===== FUN_00191ea0 @ 00191ea0 ===== */

undefined8 FUN_00191ea0(undefined8 *param_1)

{
  undefined8 uVar1;
  
  if (param_1 != (undefined8 *)0x0) {
    param_1[4] = 0;
    param_1[5] = 0;
    param_1[3] = 0;
    uVar1 = DAT_0010f748;
    param_1[1] = 0;
    param_1[2] = 0x500000000;
    *param_1 = uVar1;
    FUN_0019198c();
    if (DAT_002a1eb8 != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x00191ef0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      uVar1 = (*DAT_002a1eb8)(param_1,0x30);
      return uVar1;
    }
  }
  return 0;
}

/* ===== FUN_00191f04 @ 00191f04 ===== */

undefined8 FUN_00191f04(undefined8 param_1,undefined8 param_2,undefined4 param_3)

{
  undefined8 uVar1;
  
  FUN_0019198c();
  if (DAT_002a1ec0 != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x00191f48. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar1 = (*DAT_002a1ec0)(param_1,param_2,param_3);
    return uVar1;
  }
  return 0;
}

/* ===== FUN_00191f60 @ 00191f60 ===== */

undefined8 FUN_00191f60(undefined8 param_1,undefined8 param_2)

{
  undefined8 uVar1;
  
  FUN_0019198c();
  if (DAT_002a1ec8 != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x00191f94. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar1 = (*DAT_002a1ec8)(param_1,param_2);
    return uVar1;
  }
  return 0;
}

/* ===== FUN_00191fa8 @ 00191fa8 ===== */

undefined8 FUN_00191fa8(undefined8 param_1,undefined8 param_2)

{
  undefined8 uVar1;
  
  FUN_0019198c();
  if (DAT_002a1ed0 != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x00191fdc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar1 = (*DAT_002a1ed0)(param_1,param_2);
    return uVar1;
  }
  return 0;
}

/* ===== FUN_00191ff0 @ 00191ff0 ===== */

undefined8 FUN_00191ff0(undefined8 param_1,undefined8 param_2,undefined4 param_3)

{
  undefined8 uVar1;
  
  FUN_0019198c();
  if (DAT_002a1ed8 != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x00192034. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar1 = (*DAT_002a1ed8)(param_1,param_2,param_3);
    return uVar1;
  }
  return 0;
}

/* ===== FUN_0019204c @ 0019204c ===== */

undefined8 FUN_0019204c(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined8 uVar1;
  
  FUN_0019198c();
  if (DAT_002a1ee0 != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x00192090. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar1 = (*DAT_002a1ee0)(param_1,param_2,param_3);
    return uVar1;
  }
  return 0;
}

/* ===== FUN_001920a8 @ 001920a8 ===== */

undefined8 FUN_001920a8(undefined8 param_1,undefined8 param_2)

{
  undefined8 uVar1;
  
  FUN_0019198c();
  if (DAT_002a1ee8 != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x001920dc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar1 = (*DAT_002a1ee8)(param_1,param_2);
    return uVar1;
  }
  return 0;
}

/* ===== FUN_001920f0 @ 001920f0 ===== */

undefined8 FUN_001920f0(undefined4 param_1,undefined4 param_2)

{
  undefined8 uVar1;
  
  FUN_0019198c();
  if (DAT_002a1ef0 != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x00192124. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar1 = (*DAT_002a1ef0)(param_1,param_2);
    return uVar1;
  }
  return 0;
}

/* ===== FUN_00192138 @ 00192138 ===== */

undefined8 FUN_00192138(undefined8 param_1)

{
  undefined8 uVar1;
  
  FUN_0019198c();
  if (DAT_002a1ef8 != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x00192164. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar1 = (*DAT_002a1ef8)(param_1);
    return uVar1;
  }
  return 0;
}

/* ===== FUN_00192178 @ 00192178 ===== */

undefined8 FUN_00192178(undefined8 param_1,undefined8 param_2,undefined4 param_3)

{
  undefined8 uVar1;
  
  FUN_0019198c();
  if (DAT_002a1f00 != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x001921bc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar1 = (*DAT_002a1f00)(param_1,param_2,param_3);
    return uVar1;
  }
  return 0;
}

/* ===== FUN_001921d4 @ 001921d4 ===== */

undefined8 FUN_001921d4(undefined1 *param_1,long param_2)

{
  undefined8 uVar1;
  
  if ((param_1 != (undefined1 *)0x0) && (param_2 != 0)) {
    *param_1 = 0;
    FUN_0019198c();
    if (DAT_002a1f08 != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x00192214. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      uVar1 = (*DAT_002a1f08)(param_1,param_2);
      return uVar1;
    }
  }
  return 0;
}

/* ===== FUN_00192228 @ 00192228 ===== */

undefined8 FUN_00192228(int param_1)

{
  undefined8 uVar1;
  
  FUN_0019198c();
  if (DAT_002a1f10 == (code *)0x0) {
    uVar1 = 0;
  }
  else {
    uVar1 = (*DAT_002a1f10)(param_1);
    if ((((int)uVar1 != 0) &&
        (((param_1 != 0 || (DAT_002a1e70 == (code *)0x0)) ||
         (uVar1 = (*DAT_002a1e70)(2,0,0), (int)uVar1 != 0)))) &&
       ((DAT_002a1e70 == (code *)0x0 || (uVar1 = (*DAT_002a1e70)(4,0,0), (int)uVar1 != 0)))) {
      uVar1 = 1;
    }
  }
  return uVar1;
}

/* ===== FUN_001922ac @ 001922ac ===== */

undefined8 FUN_001922ac(undefined1 *param_1,long param_2)

{
  undefined8 uVar1;
  
  if ((param_1 != (undefined1 *)0x0) && (param_2 != 0)) {
    *param_1 = 0;
    FUN_0019198c();
    if (DAT_002a1f18 != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x001922ec. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      uVar1 = (*DAT_002a1f18)(param_1,param_2);
      return uVar1;
    }
  }
  return 0;
}

/* ===== FUN_00192300 @ 00192300 ===== */

undefined8 FUN_00192300(undefined4 param_1)

{
  undefined8 uVar1;
  
  FUN_0019198c();
  if (DAT_002a1f20 != (code *)0x0) {
                    /* WARNING: Could not recover jumptable at 0x0019232c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar1 = (*DAT_002a1f20)(param_1);
    return uVar1;
  }
  return 0;
}

/* ===== FUN_00192f58 @ 00192f58 ===== */

void FUN_00192f58(undefined8 param_1,char *param_2,byte *param_3)

{
  byte bVar1;
  long lVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  __uid_t _Var6;
  ulong uVar7;
  size_t sVar8;
  int *piVar9;
  long lVar10;
  byte *pbVar11;
  undefined1 auStack_2ef [15];
  char *local_2e0;
  undefined4 local_2d8;
  undefined4 uStack_2d4;
  long lStack_2d0;
  long local_2c8;
  long lStack_2c0;
  long local_2b8;
  long lStack_2b0;
  long local_2a8;
  long lStack_2a0;
  char local_298;
  char acStack_297 [181];
  char acStack_1e2 [10];
  stat local_1d8 [2];
  char acStack_98 [14];
  char acStack_8a [50];
  long local_58;
  
  lVar2 = tpidr_el0;
  local_58 = *(long *)(lVar2 + 0x28);
  iVar3 = FUN_0019339c(param_1,acStack_98);
  if ((((iVar3 != 0) &&
       (uVar7 = readlink(acStack_98,(char *)local_1d8,0x13f), 0xfffffffffffffec1 < uVar7 - 0x13f))
      && (*(undefined1 *)((long)local_1d8[0].__unused + (uVar7 - 0x78)) = 0, 0x17 < uVar7)) &&
     (((CONCAT35(local_1d8[0].st_dev._5_3_,(undefined5)local_1d8[0].st_dev) == 0x6e3a64666d656d2f &&
        CONCAT53((undefined5)local_1d8[0].st_ino,local_1d8[0].st_dev._5_3_) == 0x2d737578656e3a64 &&
       (iVar3 = strcmp(acStack_1e2 + uVar7," (deleted)"), iVar3 == 0)) && (uVar7 - 0x17 < 0x100))))
  {
    __memcpy_chk(&local_2d8,(undefined1 *)((long)&local_1d8[0].st_ino + 5),uVar7 - 0x17,0x100);
    auStack_2ef[uVar7] = 0;
    if (local_2d8 == 0x69726373 && CONCAT31((undefined3)uStack_2d4,local_2d8._3_1_) == 0x2d747069) {
      iVar3 = -1;
      if ((param_2 == (char *)0x0) || (param_3 == (byte *)0x0)) goto LAB_00193028;
      sVar8 = strlen((char *)param_3);
      if (((sVar8 == 0x40) && (sVar8 = strlen(param_2), 2 < sVar8)) &&
         (((iVar3 = strcmp(param_2 + (sVar8 - 3),".js"), iVar3 == 0 ||
           ((3 < sVar8 && (iVar3 = strcmp(param_2 + (sVar8 - 4),".ngs"), iVar3 == 0)))) &&
          (lVar10 = __strlen_chk(&local_2d8,0x100), lVar10 == 0x27)))) {
        lVar10 = 0;
        do {
          uVar4 = (uint)*(byte *)((long)&uStack_2d4 + lVar10 + 3);
          if (9 < uVar4 - 0x30 && 5 < uVar4 - 0x61) goto LAB_00193024;
          lVar10 = lVar10 + 1;
        } while (lVar10 != 0x20);
        bVar1 = *param_3;
        if (bVar1 != 0) {
          pbVar11 = param_3 + 1;
          do {
            if ((9 < bVar1 - 0x30) && (5 < bVar1 - 0x61)) goto LAB_00193024;
            bVar1 = *pbVar11;
            pbVar11 = pbVar11 + 1;
          } while (bVar1 != 0);
        }
        goto LAB_0019315c;
      }
    }
    else {
      uVar7 = __strlen_chk(&local_2d8,0x100);
      iVar3 = -1;
      if ((uVar7 < 0x42) || (local_298 != '-')) goto LAB_00193028;
      lVar10 = 0;
      do {
        if (9 < *(byte *)((long)&local_2d8 + lVar10) - 0x30 &&
            5 < *(byte *)((long)&local_2d8 + lVar10) - 0x61) goto LAB_00193024;
        lVar10 = lVar10 + 1;
      } while (lVar10 != 0x40);
      if (((param_2 == (char *)0x0) || (iVar3 = strcmp(param_2,acStack_297), iVar3 == 0)) &&
         ((param_3 == (byte *)0x0 ||
          ((sVar8 = strlen((char *)param_3), sVar8 == 0x40 &&
           (((((((*(long *)param_3 == CONCAT44(uStack_2d4,local_2d8) &&
                 *(long *)(param_3 + 8) == lStack_2d0) && *(long *)(param_3 + 0x10) == local_2c8) &&
               *(long *)(param_3 + 0x18) == lStack_2c0) && *(long *)(param_3 + 0x20) == local_2b8)
             && *(long *)(param_3 + 0x28) == lStack_2b0) && *(long *)(param_3 + 0x30) == local_2a8)
            && *(long *)(param_3 + 0x38) == lStack_2a0)))))) {
LAB_0019315c:
        iVar3 = FUN_0019339c(param_1,acStack_98);
        if (iVar3 != 0) {
          local_2e0 = (char *)0x0;
          piVar9 = (int *)__errno();
          *piVar9 = 0;
          lVar10 = strtol(acStack_8a,&local_2e0,10);
          iVar3 = -1;
          if ((((*piVar9 != 0) || (local_2e0 == (char *)0x0)) || (*local_2e0 != '\0')) ||
             ((lVar10 < 0 || (0x7fffffff < lVar10)))) goto LAB_00193028;
          iVar3 = fcntl((int)lVar10,0x406,0);
          if (-1 < iVar3) {
            uVar4 = fcntl(iVar3,0x40a);
            iVar5 = fstat(iVar3,local_1d8);
            if (((iVar5 == 0) && (((uint)local_1d8[0].st_nlink & 0xf000) == 0x8000)) &&
               ((_Var6 = getuid(), local_1d8[0].st_mode == _Var6 &&
                (((local_1d8[0].st_nlink._4_4_ == 0 && (-1 < (int)uVar4)) && ((uVar4 & 0xf) == 0xf))
                )))) goto LAB_00193028;
            close(iVar3);
          }
        }
      }
    }
  }
LAB_00193024:
  iVar3 = -1;
LAB_00193028:
  if (*(long *)(lVar2 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar3);
}

/* ===== FUN_00193324 @ 00193324 ===== */

void FUN_00193324(undefined8 param_1,undefined8 param_2,long param_3)

{
  long lVar1;
  int iVar2;
  long lVar3;
  undefined1 auStack_58 [8];
  long local_50;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  lVar3 = dlsym();
  if (lVar3 != 0) {
    iVar2 = dladdr(lVar3,auStack_58);
    if (iVar2 == 0) {
      lVar3 = 0;
    }
    else if (local_50 != param_3) {
      lVar3 = 0;
    }
  }
  if (*(long *)(lVar1 + 0x28) != local_38) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(lVar3);
  }
  return;
}

/* ===== FUN_0019339c @ 0019339c ===== */

/* WARNING: Removing unreachable block (ram,0x001934c0) */

void FUN_0019339c(DIR *param_1,char *param_2)

{
  long lVar1;
  bool bVar2;
  bool bVar3;
  bool bVar4;
  int iVar5;
  size_t sVar6;
  DIR *__dirp;
  dirent *pdVar7;
  ssize_t sVar8;
  uint uVar9;
  long lVar10;
  __ino_t _Var11;
  __ino_t _Var12;
  __dev_t _Var13;
  __dev_t _Var14;
  stat local_270 [3];
  char acStack_b0 [64];
  long local_70;
  
  lVar1 = tpidr_el0;
  local_70 = *(long *)(lVar1 + 0x28);
  __dirp = param_1;
  if (param_1 != (DIR *)0x0) {
    iVar5 = strncmp((char *)param_1,"/proc/self/fd/",0xe);
    if ((iVar5 == 0) && (uVar9 = (uint)(byte)param_1[0xe], param_1[0xe] != (DIR)0x0)) {
      lVar10 = 0xf;
      do {
        if (uVar9 - 0x3a < 0xfffffff6) goto LAB_00193578;
        uVar9 = (uint)(byte)param_1[lVar10];
        lVar10 = lVar10 + 1;
      } while (uVar9 != 0);
      sVar6 = strlen((char *)param_1);
      if (sVar6 < 0x40) {
        strcpy(param_2,(char *)param_1);
        __dirp = (DIR *)0x1;
        goto LAB_0019357c;
      }
    }
    else {
      iVar5 = strncmp((char *)param_1,"/memfd:nexus-",0xd);
      if ((iVar5 == 0) && (sVar6 = strlen((char *)param_1), sVar6 < 0x140)) {
        __dirp = opendir("/proc/self/fd");
        if (__dirp != (DIR *)0x0) {
          _Var13 = 0;
          _Var11 = 0;
          bVar2 = false;
          bVar3 = false;
          do {
            do {
              do {
                pdVar7 = readdir(__dirp);
                while( true ) {
                  if (pdVar7 == (dirent *)0x0) goto LAB_001935ac;
                  if ((pdVar7->d_name[0] != '.') && (sVar6 = strlen(pdVar7->d_name), sVar6 < 0x15))
                  break;
                  pdVar7 = readdir(__dirp);
                }
                FUN_001935dc(acStack_b0);
                sVar8 = readlink(acStack_b0,(char *)(local_270[0].__unused + 1),0x13f);
              } while (sVar8 - 0x13fU < 0xfffffffffffffec2);
              *(undefined1 *)((long)local_270[0].__unused + sVar8 + 8) = 0;
              iVar5 = strcmp((char *)(local_270[0].__unused + 1),(char *)param_1);
            } while (iVar5 != 0);
            iVar5 = stat(acStack_b0,local_270);
            if (iVar5 == 0) {
              _Var12 = local_270[0].st_ino;
              _Var14 = local_270[0].st_dev;
              if (bVar2) {
                bVar2 = true;
                iVar5 = 6;
                if (local_270[0].st_dev == _Var13) {
                  _Var12 = _Var11;
                  _Var14 = _Var13;
                  bVar4 = true;
                  if (local_270[0].st_ino == _Var11) goto LAB_00193514;
                }
                else {
                  bVar4 = true;
                }
              }
              else {
LAB_00193514:
                strcpy(param_2,acStack_b0);
                iVar5 = 0;
                bVar2 = true;
                _Var11 = _Var12;
                _Var13 = _Var14;
                bVar4 = bVar3;
              }
            }
            else {
              iVar5 = 6;
              bVar4 = true;
            }
            bVar3 = bVar4;
          } while (iVar5 != 6);
LAB_001935ac:
          closedir(__dirp);
          __dirp = (DIR *)(ulong)(bVar2 && !bVar3);
        }
        goto LAB_0019357c;
      }
    }
LAB_00193578:
    __dirp = (DIR *)0x0;
  }
LAB_0019357c:
  if (*(long *)(lVar1 + 0x28) != local_70) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(__dirp);
  }
  return;
}

/* ===== FUN_001935dc @ 001935dc ===== */

void FUN_001935dc(char *param_1)

{
  long lVar1;
  int iVar2;
  undefined8 in_x4;
  undefined8 in_x5;
  undefined8 in_x6;
  undefined8 in_x7;
  long lVar3;
  undefined8 local_90;
  undefined8 local_88;
  undefined8 uStack_80;
  undefined8 local_78;
  undefined1 *local_70;
  undefined1 **ppuStack_68;
  undefined8 *puStack_60;
  undefined8 uStack_58;
  
  ppuStack_68 = &local_70;
  puStack_60 = &local_90;
  lVar1 = tpidr_el0;
  lVar3 = *(long *)(lVar1 + 0x28);
  uStack_58 = 0xffffff80ffffffe0;
  local_90 = in_x4;
  local_88 = in_x5;
  uStack_80 = in_x6;
  local_78 = in_x7;
  local_70 = (undefined1 *)register0x00000008;
  iVar2 = vsnprintf(param_1,0x40,"/proc/self/fd/%s",&local_70);
  if (*(long *)(lVar1 + 0x28) == lVar3) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(iVar2);
}

/* ===== __emutls_get_address @ 0019367c ===== */

void * __emutls_get_address(size_t *param_1)

{
  pthread_key_t __key;
  undefined8 *__ptr;
  void *pvVar1;
  void *__s;
  ulong uVar2;
  size_t __n;
  size_t sVar3;
  long lVar4;
  
  sVar3 = param_1[2];
  if (sVar3 == 0) {
    pthread_once((pthread_once_t *)&DAT_002a1f48,FUN_00193880);
    pthread_mutex_lock((pthread_mutex_t *)&DAT_002a1f58);
    sVar3 = param_1[2];
    if (sVar3 == 0) {
      sVar3 = DAT_002a1f50 + 1;
      DAT_002a1f50 = sVar3;
      param_1[2] = sVar3;
    }
    pthread_mutex_unlock((pthread_mutex_t *)&DAT_002a1f58);
    __ptr = (undefined8 *)pthread_getspecific(DAT_002a1f44);
    if (__ptr == (undefined8 *)0x0) goto LAB_0019375c;
LAB_001936b0:
    uVar2 = __ptr[1];
    if (sVar3 <= uVar2) goto LAB_001937a4;
    lVar4 = (sVar3 + 0x11 & 0xfffffffffffffff0) - 2;
    __ptr = (undefined8 *)realloc(__ptr,lVar4 * 8 + 0x10);
    if (__ptr == (undefined8 *)0x0) {
                    /* WARNING: Subroutine does not return */
      abort();
    }
    memset(__ptr + uVar2 + 2,0,(lVar4 - uVar2) * 8);
  }
  else {
    __ptr = (undefined8 *)pthread_getspecific(DAT_002a1f44);
    if (__ptr != (undefined8 *)0x0) goto LAB_001936b0;
LAB_0019375c:
    lVar4 = (sVar3 + 0x11 & 0xfffffffffffffff0) - 2;
    __ptr = (undefined8 *)malloc(lVar4 * 8 + 0x10);
    if (__ptr == (undefined8 *)0x0) {
                    /* WARNING: Subroutine does not return */
      abort();
    }
    memset(__ptr + 2,0,lVar4 * 8);
    *__ptr = 1;
  }
  __key = DAT_002a1f44;
  __ptr[1] = lVar4;
  pthread_setspecific(__key,__ptr);
LAB_001937a4:
  __s = (void *)__ptr[sVar3 + 1];
  if (__s == (void *)0x0) {
    uVar2 = param_1[1];
    if (uVar2 < 9) {
      uVar2 = 8;
    }
    if ((uVar2 & uVar2 - 1) != 0) {
                    /* WARNING: Subroutine does not return */
      abort();
    }
    __n = *param_1;
    pvVar1 = malloc(uVar2 + 7 + __n);
    if (pvVar1 == (void *)0x0) {
                    /* WARNING: Subroutine does not return */
      abort();
    }
    __s = (void *)((long)pvVar1 + uVar2 + 7 & -uVar2);
    *(void **)((long)__s + -8) = pvVar1;
    if ((void *)param_1[3] == (void *)0x0) {
      memset(__s,0,__n);
    }
    else {
      memcpy(__s,(void *)param_1[3],__n);
    }
    __ptr[sVar3 + 1] = __s;
  }
  return __s;
}

/* ===== FUN_00193848 @ 00193848 ===== */

ulong FUN_00193848(ulong param_1)

{
  uint uVar1;
  
  if (DAT_002a1f40 == '\x01') {
    uVar1 = pthread_key_delete(DAT_002a1f44);
    param_1 = (ulong)uVar1;
    DAT_002a1f40 = '\0';
  }
  return param_1;
}

/* ===== FUN_00193880 @ 00193880 ===== */

void FUN_00193880(void)

{
  int iVar1;
  
  iVar1 = pthread_key_create(&DAT_002a1f44,FUN_001938bc);
  if (iVar1 == 0) {
    DAT_002a1f40 = 1;
    return;
  }
                    /* WARNING: Subroutine does not return */
  abort();
}

/* ===== FUN_001938bc @ 001938bc ===== */

long * FUN_001938bc(long *param_1)

{
  uint uVar1;
  ulong uVar2;
  ulong uVar3;
  
  if (*param_1 != 0) {
    *param_1 = *param_1 + -1;
    uVar1 = pthread_setspecific(DAT_002a1f44,param_1);
    return (long *)(ulong)uVar1;
  }
  uVar2 = param_1[1];
  if (uVar2 != 0) {
    uVar3 = 0;
    do {
      if (param_1[uVar3 + 2] != 0) {
        free(*(void **)(param_1[uVar3 + 2] + -8));
        uVar2 = param_1[1];
      }
      uVar3 = uVar3 + 1;
    } while (uVar3 < uVar2);
  }
  free(param_1);
  return param_1;
}

/* ===== FUN_00193944 @ 00193944 ===== */

void FUN_00193944(ulong param_1,ulong param_2)

{
  ulong uVar1;
  uint uVar2;
  uint uVar3;
  ulong uVar4;
  
  if (DAT_002a1f80 == 0) {
    DAT_002a1f80 = ctr_el0;
    uVar2 = (uint)DAT_002a1f80;
  }
  else {
    uVar2 = (uint)DAT_002a1f80;
  }
  uVar3 = (uint)DAT_002a1f80;
  if ((uVar2 >> 0x1c & 1) == 0) {
    uVar4 = (ulong)(uint)(4 << (ulong)(uVar3 >> 0x10 & 0xf));
    for (uVar1 = -uVar4 & param_1; uVar1 < param_2; uVar1 = uVar1 + uVar4) {
      DC_CVAU(uVar1);
    }
  }
  UnkSytemRegWrite(0,3,3,0xb,4,0);
  if ((uVar3 >> 0x1d & 1) == 0) {
    uVar1 = (ulong)(uint)(4 << (ulong)(uVar3 & 0xf));
    for (param_1 = -uVar1 & param_1; param_1 < param_2; param_1 = param_1 + uVar1) {
      IC_IVAU(param_1);
    }
    UnkSytemRegWrite(0,3,3,0xb,4,0);
  }
  InstructionSynchronizationBarrier();
  return;
}

/* ===== FUN_00193aac @ 00193aac ===== */

void FUN_00193aac(ulong param_1,long param_2)

{
  uint uVar1;
  ulong uVar2;
  int iVar3;
  ulong uVar4;
  ulong uVar5;
  ulong uVar6;
  
  uVar1 = (uint)param_1;
  if (DAT_002a1f90 != 0) {
    return;
  }
  if (((uint)(param_1 >> 0x3e) & 1) == 0) {
    uVar4 = 0;
  }
  else {
    uVar4 = *(ulong *)(param_2 + 0x10);
  }
  uVar5 = (param_1 & 0x8000000) >> 0x1a;
  if ((uVar4 & 0x80) != 0) {
    uVar5 = 6;
  }
  uVar5 = uVar5 | (ulong)(uVar1 << 3) & 0x400 | ((param_1 & 0x10) >> 4) << 0xf;
  uVar6 = uVar5 | 0x20;
  if ((uVar1 & 0xc0000) != 0xc0000) {
    uVar6 = uVar5;
  }
  uVar6 = (param_1 & 0x800000) >> 0x14 | (param_1 & 0x100000) >> 0x10 | uVar6;
  if ((param_1 & 0x200) != 0) {
    uVar6 = uVar6 | 0x10100;
  }
  uVar6 = (param_1 & 0x1000) >> 6 | (param_1 & 0x1000000) >> 7 | uVar6;
  if ((param_1 & 0x4000000) != 0) {
    uVar6 = uVar6 | 0x800000;
  }
  uVar6 = (ulong)(uVar1 << 6) & 0x800 | (param_1 & 8) << 0xb | uVar6;
  if ((param_1 & 0x40) != 0) {
    uVar6 = uVar6 | 0x1000;
  }
  uVar6 = (param_1 & 0x6000) << 7 | uVar6;
  if ((param_1 & 0x20000000) != 0) {
    uVar6 = uVar6 | 0x400000000000;
  }
  uVar6 = (param_1 & 0x10000000) << 0x15 | uVar6;
  uVar2 = uVar4 << 0x23;
  uVar5 = uVar2 & 0x2000000000;
  if ((uVar4 & 0x40000) != 0) {
    uVar6 = uVar6 | 0x180000000000;
  }
  if ((uVar4 & 0x400000) != 0) {
    uVar6 = uVar6 | 0x380000000000;
  }
  if ((uVar4 & 8) != 0) {
    uVar5 = 0x6000000000;
  }
  uVar6 = uVar2 & 0x8000000000 | uVar5 | uVar6;
  iVar3 = (int)uVar4;
  if ((uVar4 & 0x20) != 0) {
    uVar6 = uVar6 | 0x10000000000;
  }
  uVar5 = (param_1 & 0x100) >> 1 | uVar2 & 0x20000000000 | uVar6 | (uVar4 & 1) << 0x13;
  if ((uVar4 & 0x10000) != 0) {
    uVar5 = uVar5 | 1;
  }
  uVar5 = (ulong)(uint)(iVar3 << 10) & 0x2000000 |
          uVar4 >> 1 & 0x100000000 |
          uVar4 >> 4 & 0x10000000 | (ulong)(uint)(iVar3 << 0xd) & 0x4000000 | uVar5;
  if ((uVar4 & 0x100) != 0) {
    uVar5 = uVar5 | 0x1000000;
  }
  uVar5 = (uVar4 & 0x2000000) << 0x1e |
          (uVar4 & 0x1000000) << 0x20 |
          (uVar4 & 0x800000) << 0x13 |
          (uVar4 & 0x80000000) << 0x17 |
          (ulong)(uint)(iVar3 << 8) & 0x20000000 |
          (uVar4 & 0x20000) << 0x21 | (uVar4 & 0xe00) << 0x18 | uVar5;
  if ((uVar1 >> 0xb & 1) == 0) {
    if ((param_1 & 0x201) != 0) {
      uVar5 = uVar5 | 0x300;
    }
    if ((uVar4 & 1) != 0 || (param_1 & 0x10000) != 0) {
      uVar5 = uVar5 | 0x40000;
    }
    if ((param_1 & 0x4008000) != 0) {
      uVar5 = uVar5 | 0x400000;
    }
    uVar6 = (param_1 & 0x20000) >> 4;
    if ((uVar4 & 0x100004000) != 0) {
      uVar5 = uVar5 | 0x8000000;
    }
    uVar5 = (ulong)(uint)(iVar3 << 0x13) & 0x80000000 | uVar5;
    if ((uVar4 & 2) != 0 && (param_1 & 0x400000) != 0) {
      uVar5 = uVar5 | 0x1000000000;
    }
  }
  else {
    uVar4 = id_aa64pfr1_el1;
    if ((uVar4 & 0xf00) != 0) {
      uVar5 = uVar5 | 0x80000000000;
    }
    uVar6 = uVar5 | 0x1000000000000;
    if ((uVar4 & 0xf0) != 0x10) {
      uVar6 = uVar5;
    }
    uVar2 = id_aa64pfr0_el1;
    uVar5 = uVar6 | 0x200000000000000;
    if ((uVar4 & 0xf000000) != 0x2000000) {
      uVar5 = uVar6;
    }
    if ((~(uint)uVar2 & 0xf0000) != 0) {
      uVar5 = uVar5 | 0x300;
    }
    if ((uVar2 & 0xf00000000) != 0) {
      uVar4 = UnkSytemRegRead(3,0,0,4,4);
      if ((uVar4 & 0xf) == 1) {
        uVar5 = uVar5 | 0x1000000000;
      }
      else if ((uVar4 & 0xf) == 0) {
        uVar5 = uVar5 | 0x40000000;
      }
      if ((uVar4 & 0xf00000) != 0) {
        uVar5 = uVar5 | 0x80000000;
      }
    }
    uVar4 = id_aa64isar0_el1;
    if ((uVar4 & 0xf00000000) != 0) {
      uVar5 = uVar5 | 0x2000;
    }
    uVar4 = id_aa64isar1_el1;
    if ((uVar4 & 0xf) != 0) {
      uVar5 = uVar5 | 0x40000;
    }
    if ((uVar4 & 0xf00000) != 0) {
      uVar5 = uVar5 | 0x400000;
    }
    uVar6 = uVar5 | 0x800000000000;
    if ((uVar4 & 0xf0000000000) != 0x20000000000) {
      uVar6 = uVar5;
    }
    if ((uVar4 & 0xf00000000000) != 0) {
      uVar6 = uVar6 | 0x8000000;
    }
    if (uVar4 >> 0x3c == 0) goto LAB_00193eb0;
    if (uVar4 >> 0x3d == 0) {
      DAT_002a1f90 = uVar6 | 0x408000000000000;
      return;
    }
    uVar5 = 0x38000000000000;
    if (uVar4 >> 0x3c < 3) {
      uVar5 = 0x18000000000000;
    }
  }
  uVar6 = uVar6 | uVar5;
LAB_00193eb0:
  DAT_002a1f90 = uVar6 | 0x400000000000000;
  return;
}

/* ===== FUN_00194040 @ 00194040 ===== */

void FUN_00194040(long param_1)

{
  FUN_00184004(*(undefined8 *)(param_1 + 0x9e8));
  return;
}

/* ===== FUN_00194050 @ 00194050 ===== */

void FUN_00194050(void)

{
  (*(code *)PTR_001a3738)();
  return;
}

/* ===== nexus_ui_performance_mode @ 00194140 ===== */

void nexus_ui_performance_mode(void)

{
  (*(code *)PTR_nexus_ui_performance_mode_001a37a8)();
  return;
}

/* ===== __cxa_finalize @ 002a2000 ===== */

/* WARNING: Control flow encountered bad instruction data */

void __cxa_finalize(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== __cxa_atexit @ 002a2008 ===== */

/* WARNING: Control flow encountered bad instruction data */

void __cxa_atexit(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== __register_atfork @ 002a2010 ===== */

/* WARNING: Control flow encountered bad instruction data */

void __register_atfork(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== gettid @ 002a2018 ===== */

/* WARNING: Control flow encountered bad instruction data */

void gettid(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== strnlen @ 002a2020 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

size_t strnlen(char *__string,size_t __maxlen)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== __errno @ 002a2028 ===== */

/* WARNING: Control flow encountered bad instruction data */

void __errno(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== __stack_chk_fail @ 002a2030 ===== */

/* WARNING: Control flow encountered bad instruction data */

void __stack_chk_fail(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== memset @ 002a2038 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * memset(void *__s,int __c,size_t __n)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== __memchr_chk @ 002a2040 ===== */

/* WARNING: Control flow encountered bad instruction data */

void __memchr_chk(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== memcpy @ 002a2048 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * memcpy(void *__dest,void *__src,size_t __n)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== clock_gettime @ 002a2050 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int clock_gettime(clockid_t __clock_id,timespec *__tp)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== __android_log_print @ 002a2058 ===== */

/* WARNING: Control flow encountered bad instruction data */

void __android_log_print(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== strcmp @ 002a2060 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int strcmp(char *__s1,char *__s2)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== strlen @ 002a2068 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

size_t strlen(char *__s)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== pthread_self @ 002a2070 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

pthread_t pthread_self(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== pthread_getname_np @ 002a2078 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_getname_np(pthread_t __target_thread,char *__buf,size_t __buflen)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== getpid @ 002a2080 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

__pid_t getpid(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== syscall @ 002a2088 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

long syscall(long __sysno,...)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== dlsym @ 002a2090 ===== */

/* WARNING: Control flow encountered bad instruction data */

void dlsym(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== dladdr @ 002a2098 ===== */

/* WARNING: Control flow encountered bad instruction data */

void dladdr(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== pthread_once @ 002a20a0 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_once(pthread_once_t *__once_control,__init_routine *__init_routine)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== memcmp @ 002a20a8 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int memcmp(void *__s1,void *__s2,size_t __n)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== openat @ 002a20b0 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int openat(int __fd,char *__file,int __oflag,...)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== __write_chk @ 002a20b8 ===== */

/* WARNING: Control flow encountered bad instruction data */

void __write_chk(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== fsync @ 002a20c0 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int fsync(int __fd)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== close @ 002a20c8 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int close(int __fd)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== renameat @ 002a20d0 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int renameat(int __oldfd,char *__old,int __newfd,char *__new)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== unlinkat @ 002a20d8 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int unlinkat(int __fd,char *__name,int __flag)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== __open_2 @ 002a20e0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void __open_2(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== read @ 002a20e8 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

ssize_t read(int __fd,void *__buf,size_t __nbytes)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== dl_iterate_phdr @ 002a20f0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void dl_iterate_phdr(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== sysconf @ 002a20f8 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

long sysconf(int __name)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== fstat @ 002a2100 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int fstat(int __fd,stat *__buf)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== mkdirat @ 002a2108 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int mkdirat(int __fd,char *__path,__mode_t __mode)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== __openat_2 @ 002a2110 ===== */

/* WARNING: Control flow encountered bad instruction data */

void __openat_2(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== getuid @ 002a2118 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

__uid_t getuid(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== abort @ 002a2120 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void abort(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== strrchr @ 002a2128 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * strrchr(char *__s,int __c)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== strchr @ 002a2130 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * strchr(char *__s,int __c)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== __memcpy_chk @ 002a2138 ===== */

/* WARNING: Control flow encountered bad instruction data */

void __memcpy_chk(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== opendir @ 002a2140 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

DIR * opendir(char *__name)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== dirfd @ 002a2148 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int dirfd(DIR *__dirp)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== readdir @ 002a2150 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

dirent * readdir(DIR *__dirp)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== fstatat @ 002a2158 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int fstatat(int __fd,char *__file,stat *__buf,int __flag)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== closedir @ 002a2160 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int closedir(DIR *__dirp)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== strncmp @ 002a2168 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int strncmp(char *__s1,char *__s2,size_t __n)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== strstr @ 002a2170 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * strstr(char *__haystack,char *__needle)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== mprotect @ 002a2178 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int mprotect(void *__addr,size_t __len,int __prot)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== fopen @ 002a2180 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

FILE * fopen(char *__filename,char *__modes)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== fgets @ 002a2188 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * fgets(char *__s,int __n,FILE *__stream)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== sscanf @ 002a2190 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int sscanf(char *__s,char *__format,...)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== ferror @ 002a2198 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int ferror(FILE *__stream)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== clearerr @ 002a21a0 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void clearerr(FILE *__stream)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== fclose @ 002a21a8 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int fclose(FILE *__stream)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== dlopen @ 002a21b0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void dlopen(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== dlclose @ 002a21b8 ===== */

/* WARNING: Control flow encountered bad instruction data */

void dlclose(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== __vsnprintf_chk @ 002a21c0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void __vsnprintf_chk(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== __read_chk @ 002a21c8 ===== */

/* WARNING: Control flow encountered bad instruction data */

void __read_chk(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== __strchr_chk @ 002a21d0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void __strchr_chk(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== realpath @ 002a21d8 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * realpath(char *__name,char *__resolved)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== geteuid @ 002a21e0 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

__uid_t geteuid(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== readlink @ 002a21e8 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

ssize_t readlink(char *__path,char *__buf,size_t __len)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== __strlen_chk @ 002a21f0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void __strlen_chk(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== dlerror @ 002a21f8 ===== */

/* WARNING: Control flow encountered bad instruction data */

void dlerror(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== stat @ 002a2200 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int stat(char *__file,stat *__buf)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== strcpy @ 002a2208 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

char * strcpy(char *__dest,char *__src)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== strtol @ 002a2210 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

long strtol(char *__nptr,char **__endptr,int __base)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== fcntl @ 002a2218 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int fcntl(int __fd,int __cmd,...)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== malloc @ 002a2220 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * malloc(size_t __size)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== free @ 002a2228 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void free(void *__ptr)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== calloc @ 002a2230 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * calloc(size_t __nmemb,size_t __size)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== pthread_create @ 002a2238 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_create(pthread_t *__newthread,pthread_attr_t *__attr,__start_routine *__start_routine,
                  void *__arg)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== pthread_detach @ 002a2240 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_detach(pthread_t __th)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== memchr @ 002a2248 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * memchr(void *__s,int __c,size_t __n)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== pthread_mutex_lock @ 002a2250 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_mutex_lock(pthread_mutex_t *__mutex)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== pthread_mutex_unlock @ 002a2258 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_mutex_unlock(pthread_mutex_t *__mutex)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== pthread_join @ 002a2260 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_join(pthread_t __th,void **__thread_return)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== usleep @ 002a2268 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int usleep(__useconds_t __useconds)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== mmap @ 002a2270 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * mmap(void *__addr,size_t __len,int __prot,int __flags,int __fd,__off_t __offset)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== munmap @ 002a2278 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int munmap(void *__addr,size_t __len)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== feof @ 002a2280 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int feof(FILE *__stream)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== strtoll @ 002a2288 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

longlong strtoll(char *__nptr,char **__endptr,int __base)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== vsnprintf @ 002a2290 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int vsnprintf(char *__s,size_t __maxlen,char *__format,__gnuc_va_list __arg)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== pthread_mutex_trylock @ 002a2298 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_mutex_trylock(pthread_mutex_t *__mutex)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== exp @ 002a22a0 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

double exp(double __x)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== expm1 @ 002a22a8 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

double expm1(double __x)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== memmove @ 002a22b0 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * memmove(void *__dest,void *__src,size_t __n)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== __pread_chk @ 002a22b8 ===== */

/* WARNING: Control flow encountered bad instruction data */

void __pread_chk(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== pthread_getspecific @ 002a22c0 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * pthread_getspecific(pthread_key_t __key)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== realloc @ 002a22c8 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

void * realloc(void *__ptr,size_t __size)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== pthread_setspecific @ 002a22d0 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_setspecific(pthread_key_t __key,void *__pointer)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== pthread_key_delete @ 002a22d8 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_key_delete(pthread_key_t __key)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== pthread_key_create @ 002a22e0 ===== */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Unknown calling convention -- yet parameter storage is locked */

int pthread_key_create(pthread_key_t *__key,__destr_function *__destr_function)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== getauxval @ 002a22e8 ===== */

/* WARNING: Control flow encountered bad instruction data */

void getauxval(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

/* ===== __system_property_get @ 002a22f0 ===== */

/* WARNING: Control flow encountered bad instruction data */

void __system_property_get(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}

