/*
 * menu_engine — UI subsystem
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
 * Notes: Action-id dispatch (TRY_MENU_ACT_*), sections, snapshots, import/export, profile pump.
 */

/* ===== FUN_0014c510 @ 0014c510 ===== */

int FUN_0014c510(void)

{
  int iVar1;
  int iVar2;
  
  gettid();
  iVar1 = nexus_menu_server_thread();
  iVar2 = 0;
  if ((iVar1 != 0) && (DAT_0022d830 != 0x7fffffff)) {
    DAT_0022d830 = DAT_0022d830 + 1;
    iVar2 = DAT_0022d830;
  }
  return iVar2;
}

/* ===== FUN_0014c550 @ 0014c550 ===== */

bool FUN_0014c550(int param_1,char *param_2,char *param_3,uint param_4,int param_5)

{
  undefined4 uVar1;
  long lVar2;
  bool bVar3;
  long *plVar4;
  bool bVar5;
  char cVar6;
  int iVar7;
  size_t sVar8;
  size_t sVar9;
  undefined4 *puVar10;
  long lVar11;
  long lVar12;
  long *local_70;
  long local_68;
  
  lVar2 = tpidr_el0;
  bVar5 = false;
  local_68 = *(long *)(lVar2 + 0x28);
  if ((((2 < param_4) || (param_1 < 1)) || (param_2 == (char *)0x0)) ||
     ((param_3 == (char *)0x0 || (param_5 - 0x101U < 0xffffff00)))) goto LAB_0014c6fc;
  sVar8 = strnlen(param_2,0x181);
  sVar9 = strnlen(param_3,0x401);
  bVar5 = false;
  if ((sVar8 - 0x181 < 0xfffffffffffffe80) || (0x400 < sVar9)) goto LAB_0014c6fc;
  puVar10 = (undefined4 *)__errno();
  uVar1 = *puVar10;
  local_70 = (long *)0x0;
  if ((DAT_0022f028 == (long *)0x0) || (DAT_0022d838 == 0)) {
LAB_0014c6f0:
    bVar5 = false;
  }
  else {
    gettid();
    iVar7 = nexus_menu_server_thread();
    if (iVar7 == 0) goto LAB_0014c6f0;
    iVar7 = (**(code **)(*DAT_0022f028 + 0x30))(DAT_0022f028,&local_70,0x10006);
    if (iVar7 == 0) {
      bVar3 = true;
    }
    else {
      if ((iVar7 != -2) ||
         (iVar7 = (**(code **)(*DAT_0022f028 + 0x20))(DAT_0022f028,&local_70,0), iVar7 != 0))
      goto LAB_0014c6f0;
      bVar3 = false;
    }
    plVar4 = local_70;
    if (local_70 == (long *)0x0) goto LAB_0014c6f0;
    iVar7 = (**(code **)(*local_70 + 0x98))(local_70,4);
    if (iVar7 != 0) {
      cVar6 = (**(code **)(*plVar4 + 0x720))(plVar4);
      if (cVar6 != '\0') {
        (**(code **)(*plVar4 + 0x88))(plVar4);
      }
      if (!bVar3) {
        (**(code **)(*DAT_0022f028 + 0x28))();
      }
      goto LAB_0014c6f0;
    }
    lVar11 = (**(code **)(*plVar4 + 0x580))(plVar4,sVar8 & 0xffffffff);
    if (lVar11 == 0) {
LAB_0014c760:
      bVar5 = false;
    }
    else {
      cVar6 = (**(code **)(*plVar4 + 0x720))();
      if ((cVar6 != '\0') ||
         (lVar12 = (**(code **)(*plVar4 + 0x580))(plVar4,sVar9 & 0xffffffff), lVar12 == 0))
      goto LAB_0014c760;
      cVar6 = (**(code **)(*plVar4 + 0x720))();
      if (cVar6 != '\0') goto LAB_0014c760;
      (**(code **)(*plVar4 + 0x680))(plVar4,lVar11,0,sVar8 & 0xffffffff,param_2);
      if (sVar9 != 0) {
        (**(code **)(*plVar4 + 0x680))(plVar4,lVar12,0,sVar9 & 0xffffffff,param_3);
      }
      cVar6 = (**(code **)(*plVar4 + 0x720))();
      if (cVar6 != '\0') goto LAB_0014c760;
      cVar6 = (**(code **)(*plVar4 + 0x3a8))
                        (plVar4,DAT_0022d838,DAT_0022d840,param_1,lVar11,lVar12,param_4,param_5);
      bVar5 = cVar6 == '\x01';
    }
    cVar6 = (**(code **)(*plVar4 + 0x720))(plVar4);
    if (cVar6 != '\0') {
      bVar5 = false;
    }
    (**(code **)(*plVar4 + 0xa0))(plVar4,0);
    cVar6 = (**(code **)(*plVar4 + 0x720))(plVar4);
    if (cVar6 != '\0') {
      (**(code **)(*plVar4 + 0x88))(plVar4);
    }
    if (!bVar3) {
      (**(code **)(*DAT_0022f028 + 0x28))();
    }
  }
  *puVar10 = uVar1;
LAB_0014c6fc:
  if (*(long *)(lVar2 + 0x28) == local_68) {
    return bVar5;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0014c8ac @ 0014c8ac ===== */

ulong FUN_0014c8ac(int param_1,uint *param_2,undefined1 *param_3,ulong param_4)

{
  undefined4 uVar1;
  uint uVar2;
  long lVar3;
  bool bVar4;
  long *plVar5;
  char cVar6;
  int iVar7;
  undefined4 *puVar8;
  long lVar9;
  ulong __n;
  ulong uVar10;
  undefined8 local_480;
  uint local_478;
  uint local_474;
  undefined1 auStack_470 [1024];
  long local_70;
  
  lVar3 = tpidr_el0;
  uVar10 = 0;
  local_70 = *(long *)(lVar3 + 0x28);
  if ((((param_1 < 1) || (param_2 == (uint *)0x0)) || (param_3 == (undefined1 *)0x0)) ||
     (param_4 == 0)) goto LAB_0014c9fc;
  *param_2 = 3;
  *param_3 = 0;
  puVar8 = (undefined4 *)__errno();
  uVar1 = *puVar8;
  local_480 = (long *)0x0;
  if ((DAT_0022f028 == (long *)0x0) || (DAT_0022d838 == 0)) {
LAB_0014c9f4:
    uVar10 = 0;
  }
  else {
    gettid();
    iVar7 = nexus_menu_server_thread();
    if (iVar7 == 0) goto LAB_0014c9f4;
    iVar7 = (**(code **)(*DAT_0022f028 + 0x30))(DAT_0022f028,&local_480,0x10006);
    if (iVar7 == 0) {
      bVar4 = true;
    }
    else {
      if ((iVar7 != -2) ||
         (iVar7 = (**(code **)(*DAT_0022f028 + 0x20))(DAT_0022f028,&local_480,0), iVar7 != 0))
      goto LAB_0014c9f4;
      bVar4 = false;
    }
    plVar5 = local_480;
    if (local_480 == (long *)0x0) goto LAB_0014c9f4;
    iVar7 = (**(code **)(*local_480 + 0x98))(local_480,2);
    if (iVar7 != 0) {
      cVar6 = (**(code **)(*plVar5 + 0x720))(plVar5);
      if (cVar6 != '\0') {
        (**(code **)(*plVar5 + 0x88))(plVar5);
      }
      if (!bVar4) {
        (**(code **)(*DAT_0022f028 + 0x28))();
      }
      goto LAB_0014c9f4;
    }
    uVar10 = (**(code **)(*plVar5 + 0x390))(plVar5,DAT_0022d838,DAT_0022d848,param_1);
    memset(&local_480,0,0x410);
    if (uVar10 == 0) goto LAB_0014cac0;
    cVar6 = (**(code **)(*plVar5 + 0x720))(plVar5);
    if (cVar6 == '\0') {
      iVar7 = (**(code **)(*plVar5 + 0x558))(plVar5,uVar10);
      uVar2 = iVar7 - 0x10;
      __n = (ulong)uVar2;
      if (0x400 < uVar2) goto LAB_0014cabc;
      cVar6 = (**(code **)(*plVar5 + 0x720))(plVar5);
      if (cVar6 != '\0') goto LAB_0014cabc;
      (**(code **)(*plVar5 + 0x640))(plVar5,uVar10,0,iVar7,&local_480);
      cVar6 = (**(code **)(*plVar5 + 0x720))(plVar5);
      uVar10 = 0;
      if (cVar6 == '\0') {
        if ((int)local_480 != 1) goto LAB_0014cabc;
        uVar10 = 0;
        if (((local_480._4_4_ == param_1) && (local_478 < 4)) && (local_474 == uVar2)) {
          if ((param_4 <= __n) || ((local_478 != 1 && (uVar2 != 0)))) goto LAB_0014cabc;
          lVar9 = __memchr_chk(auStack_470,0,__n,0x400);
          uVar10 = 0;
          if (lVar9 == 0) {
            memcpy(param_3,auStack_470,__n);
            uVar10 = 1;
            param_3[__n] = 0;
            *param_2 = local_478;
          }
        }
      }
    }
    else {
LAB_0014cabc:
      uVar10 = 0;
    }
LAB_0014cac0:
    (**(code **)(*plVar5 + 0xa0))(plVar5,0);
    cVar6 = (**(code **)(*plVar5 + 0x720))(plVar5);
    if (cVar6 != '\0') {
      (**(code **)(*plVar5 + 0x88))(plVar5);
    }
    if (!bVar4) {
      (**(code **)(*DAT_0022f028 + 0x28))();
    }
  }
  *puVar8 = uVar1;
LAB_0014c9fc:
  if (*(long *)(lVar3 + 0x28) == local_70) {
    return uVar10 & 0xffffffff;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0014cc0c @ 0014cc0c ===== */

void FUN_0014cc0c(int param_1)

{
  undefined4 uVar1;
  long lVar2;
  bool bVar3;
  long *plVar4;
  char cVar5;
  int iVar6;
  undefined4 *puVar7;
  long *local_60;
  long local_58;
  
  lVar2 = tpidr_el0;
  local_58 = *(long *)(lVar2 + 0x28);
  if (param_1 < 1) goto LAB_0014cd2c;
  puVar7 = (undefined4 *)__errno();
  uVar1 = *puVar7;
  local_60 = (long *)0x0;
  if ((DAT_0022f028 != (long *)0x0) && (DAT_0022d838 != 0)) {
    gettid();
    iVar6 = nexus_menu_server_thread();
    if (iVar6 != 0) {
      iVar6 = (**(code **)(*DAT_0022f028 + 0x30))(DAT_0022f028,&local_60,0x10006);
      if (iVar6 == 0) {
        bVar3 = true;
      }
      else {
        if ((iVar6 != -2) ||
           (iVar6 = (**(code **)(*DAT_0022f028 + 0x20))(DAT_0022f028,&local_60,0), iVar6 != 0))
        goto LAB_0014cd28;
        bVar3 = false;
      }
      plVar4 = local_60;
      if (local_60 != (long *)0x0) {
        (**(code **)(*local_60 + 0x468))(local_60,DAT_0022d838,DAT_0022d850,param_1);
        cVar5 = (**(code **)(*plVar4 + 0x720))(plVar4);
        if (cVar5 != '\0') {
          (**(code **)(*plVar4 + 0x88))(plVar4);
        }
        if (!bVar3) {
          (**(code **)(*DAT_0022f028 + 0x28))();
        }
      }
    }
  }
LAB_0014cd28:
  *puVar7 = uVar1;
LAB_0014cd2c:
  if (*(long *)(lVar2 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0014ef90 @ 0014ef90 ===== */

void FUN_0014ef90(undefined8 *param_1,undefined8 param_2)

{
  bool bVar1;
  uint uVar2;
  undefined4 uVar3;
  long lVar4;
  int iVar5;
  int iVar6;
  pthread_t __target_thread;
  undefined8 uVar7;
  code *pcVar8;
  long lVar9;
  long lVar10;
  long lVar11;
  ulong uVar12;
  undefined4 uVar13;
  uint uVar14;
  uint local_a4;
  int local_a0;
  int local_9c;
  long local_98;
  char *pcStack_90;
  byte local_88 [4];
  undefined4 local_84;
  undefined4 uStack_80;
  int local_7c;
  undefined8 local_78;
  long local_70;
  long local_58;
  
  lVar9 = DAT_001a7c48;
  lVar4 = tpidr_el0;
  local_58 = *(long *)(lVar4 + 0x28);
  param_1[4] = 0;
  param_1[5] = 0;
  uVar12 = DAT_0022db60;
  uVar7 = DAT_0010f928;
  local_7c = 0;
  *param_1 = param_2;
  param_1[1] = lVar9;
  param_1[2] = uVar12;
  param_1[3] = 0;
  param_1[6] = uVar7;
  iVar6 = DAT_0022f038;
  local_84 = 2000;
  uStack_80 = 0xdac;
  local_88[0] = 1;
  if (DAT_0022d8d8 != '\x01') goto LAB_0014f040;
  local_78 = 0;
  local_70 = 0;
  if ((DAT_0022f038 < 1) || (iVar5 = gettid(), iVar5 != iVar6)) goto LAB_0014f040;
  __target_thread = pthread_self();
  iVar6 = pthread_getname_np(__target_thread,(char *)&local_78,0x10);
  if ((iVar6 != 0) ||
     (((local_78 != 0x706f6f6c6e69614d || (char)local_70 != '\0' || (DAT_0022f03c < 1)) ||
      (uVar7 = FUN_00169574(&DAT_0022f040), (int)uVar7 != 7)))) goto LAB_0014f040;
  local_78 = 0;
  uVar7 = FUN_0014e7c4(uVar7,DAT_0022d8e0 + 0x12eb9f0,&local_78,8);
  uVar12 = local_78;
  if (((local_78 & 7) != 0 || local_78 < 0x1000) || (int)uVar7 == 0) {
    uVar12 = 0;
  }
  if (uVar12 != DAT_0022f178) goto LAB_0014f040;
  local_78 = 0;
  iVar6 = FUN_0014e7c4(uVar7,uVar12 + 0x90,&local_78,8);
  uVar12 = local_78;
  if (((local_78 & 7) != 0 || local_78 < 0x1000) || iVar6 == 0) {
    uVar12 = 0;
  }
  if (((uVar12 != DAT_0022db60) || (uVar7 = FUN_00154d5c(DAT_0022f180), (int)uVar7 == 0)) ||
     (iVar6 = FUN_0014e7c4(uVar7,DAT_0022f178 + 0x19c,local_88,1), iVar6 == 0)) goto LAB_0014f040;
  *(uint *)((long)param_1 + 0x2c) = (uint)local_88[0];
  iVar6 = nexus_menu_setting_value(0x25,&local_7c);
  if ((iVar6 == 1) && (iVar6 = nexus_menu_setting_value(0x62,&uStack_80), iVar6 == 1)) {
    iVar6 = nexus_menu_setting_value(99,&local_84);
    uVar14 = (uint)(iVar6 == 1);
  }
  else {
    uVar14 = 0;
  }
  pcVar8 = DAT_0022f188;
  if ((DAT_0022f188 == (code *)0x0) &&
     (((pcVar8 = (code *)dlsym(DAT_0022f0c0,"nexus_evasion_snapshot_keys_v1"), pcVar8 == (code *)0x0
       || (iVar6 = dladdr(pcVar8,&local_78), iVar6 == 0)) || (local_70 != DAT_0022f0c8)))) {
    local_98 = 0;
    pcStack_90 = "autofarmEnabled";
    local_9c = -1;
    pcVar8 = DAT_0022f188;
    if (DAT_0022f188 != (code *)0x0) goto LAB_0014f228;
LAB_0014f2b8:
    uVar13 = 0;
    uVar14 = 0;
    *(undefined4 *)(param_1 + 4) = 0;
  }
  else {
LAB_0014f228:
    DAT_0022f188 = pcVar8;
    pcStack_90 = "autofarmEnabled";
    local_98 = 0;
    local_9c = -1;
    iVar6 = (*DAT_0022f188)(&pcStack_90,&local_78,1,&local_98,&local_9c);
    if ((iVar6 != 1) || (local_98 == 0)) goto LAB_0014f2b8;
    uVar2 = 0;
    if (local_9c == 0) {
      uVar2 = uVar14;
    }
    *(uint *)(param_1 + 4) = uVar2;
    if (uVar2 == 1) {
      iVar6 = pthread_once((pthread_once_t *)&DAT_0022f1d0,FUN_001552c0);
      if (DAT_0022f1d8 == (code *)0x0) {
        uVar13 = 1;
        uVar14 = 0;
      }
      else {
        iVar6 = (*DAT_0022f1d8)(iVar6);
        uVar14 = 0;
        uVar13 = 1;
        if ((iVar6 == 1) && ((int)local_70 == 2)) {
          uVar14 = (uint)(local_78._4_4_ == 1);
        }
      }
    }
    else {
      uVar13 = 0;
      uVar14 = 0;
    }
  }
  lVar9 = DAT_0022d8e0;
  uVar3 = 0;
  if (local_7c == 1) {
    uVar3 = uVar13;
  }
  *(undefined4 *)(param_1 + 6) = uStack_80;
  *(undefined4 *)((long)param_1 + 0x34) = local_84;
  *(uint *)((long)param_1 + 0x24) = uVar14;
  *(undefined4 *)(param_1 + 5) = uVar3;
  lVar9 = FUN_00154cf0(lVar9 + 0x1307e20);
  local_a4 = 0xffffffff;
  local_a0 = -1;
  if ((((lVar9 == 0) || (iVar6 = FUN_0014e7c4(lVar9,lVar9 + 0x50,&local_a0,4), iVar6 == 0)) ||
      (local_a0 < 0)) || (0x40 < local_a0)) {
LAB_0014f3d4:
    *(undefined4 *)(param_1 + 4) = 0;
  }
  else {
    if (local_a0 == 5) {
      lVar10 = FUN_00154cf0(lVar9 + 0x48);
      lVar11 = FUN_00154cf0(lVar10 + 0x28);
      if (((lVar11 == 0) || (iVar6 = FUN_0014e7c4(lVar11,lVar11 + 0xb8,&local_a4,4), iVar6 == 0)) ||
         (uVar12 = (ulong)local_a4, (int)local_a4 < 0)) goto LAB_0014f3d4;
      if (lVar10 == 0) goto LAB_0014f3dc;
      if (((lVar10 != DAT_0022f190) || (lVar11 != DAT_0022f198)) ||
         ((lVar9 != DAT_0022f1a0 ||
          (bVar1 = uVar12 < DAT_0022f1a8, DAT_0022f190 = lVar10, DAT_0022f198 = lVar11,
          DAT_0022f1a0 = lVar9, DAT_0022f1a8 = uVar12, bVar1)))) {
        if (DAT_001a7c48 == -1) {
          *(undefined4 *)(param_1 + 4) = 0;
          DAT_0022f190 = lVar10;
          DAT_0022f198 = lVar11;
          DAT_0022f1a0 = lVar9;
          DAT_0022f1a8 = uVar12;
        }
        else {
          DAT_001a7c48 = DAT_001a7c48 + 1;
          DAT_0022f190 = lVar10;
          DAT_0022f198 = lVar11;
          DAT_0022f1a0 = lVar9;
          DAT_0022f1a8 = uVar12;
        }
      }
    }
    else {
LAB_0014f3dc:
      DAT_0022f198 = 0;
      DAT_0022f190 = 0;
      DAT_0022f1a8 = 0;
    }
    lVar9 = DAT_001a7c48;
    param_1[3] = DAT_0022f190;
    param_1[1] = lVar9;
  }
LAB_0014f040:
  if (*(long *)(lVar4 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00150378 @ 00150378 ===== */

void FUN_00150378(undefined8 param_1)

{
  ulong uVar1;
  long lVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  pthread_t __target_thread;
  ulong uVar6;
  undefined8 uVar7;
  int local_68;
  char local_64 [4];
  undefined1 auStack_60 [5];
  char local_5b;
  ulong local_58;
  ulong local_50;
  long local_48;
  
  lVar2 = tpidr_el0;
  local_48 = *(long *)(lVar2 + 0x28);
  local_68 = -1;
  local_58 = 0;
  local_64[0] = '\x01';
  iVar3 = FUN_0014e7c4(param_1,DAT_0022d8e0 + 0x1307e20,&local_58,8);
  uVar1 = local_58;
  iVar5 = DAT_0022f038;
  if (DAT_0022d890 == 1) {
    if (-1 < DAT_0022f03c) {
LAB_00150400:
      local_58 = 0;
      local_50 = 0;
      if ((0 < DAT_0022f038) && (iVar4 = gettid(), iVar4 == iVar5)) {
        __target_thread = pthread_self();
        iVar4 = pthread_getname_np(__target_thread,(char *)&local_58,0x10);
        if ((iVar4 == 0) && (local_58 == 0x706f6f6c6e69614d && (local_50 & 0xff) == 0)) {
          iVar4 = FUN_0014fac4();
          uVar6 = 0;
          if ((((iVar4 == 0) || (iVar3 == 0)) || (uVar1 < 0x1000)) || ((uVar1 & 7) != 0))
          goto LAB_0015045c;
          iVar3 = FUN_0014e7c4(0,uVar1 + 0x50,&local_68,4);
          uVar6 = 0;
          if ((((iVar3 == 0) || (local_68 < 0)) || (0x40 < local_68)) || (local_68 == 5))
          goto LAB_0015045c;
          iVar5 = nexus_menu_ui_state(auStack_60,iVar5);
          uVar6 = 0;
          if ((iVar5 != 1) || (local_5b != '\0')) goto LAB_0015045c;
          local_58 = 0;
          uVar7 = FUN_0014e7c4(0,DAT_0022d8e0 + 0x12eb9f0,&local_58,8);
          uVar1 = local_58;
          if (((local_58 & 7) != 0 || local_58 < 0x1000) || (int)uVar7 == 0) {
            uVar1 = 0;
          }
          if (uVar1 == DAT_0022f178) {
            local_58 = 0;
            uVar6 = FUN_0014e7c4(uVar7,uVar1 + 0x90,&local_58,8);
            uVar1 = local_58;
            if (((local_58 & 7) != 0 || local_58 < 0x1000) || (int)uVar6 == 0) {
              uVar1 = 0;
            }
            if (uVar1 == DAT_0022db60) {
              if (((DAT_0022d890 & 1) != 0) || (uVar6 = FUN_00154d5c(DAT_0022f180), (int)uVar6 != 0)
                 ) {
                iVar5 = FUN_0014e7c4(uVar6,DAT_0022f178 + 0x19c,local_64,1);
                uVar6 = 0;
                if ((iVar5 != 0) && (local_64[0] == '\0')) {
                  iVar5 = FUN_001554b0(0);
                  uVar6 = (ulong)(iVar5 != 0);
                }
              }
              goto LAB_0015045c;
            }
          }
        }
      }
    }
  }
  else if (0 < DAT_0022f03c) goto LAB_00150400;
  uVar6 = 0;
LAB_0015045c:
  if (*(long *)(lVar2 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar6);
}

/* ===== FUN_00152b7c @ 00152b7c ===== */

/* WARNING: Removing unreachable block (ram,0x00152db8) */

void FUN_00152b7c(void)

{
  uint uVar1;
  long lVar2;
  bool bVar3;
  char cVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  uint *puVar8;
  DIR *__dirp;
  dirent *pdVar9;
  ssize_t sVar10;
  long lVar11;
  undefined8 uVar12;
  long lVar13;
  char *pcVar14;
  char *pcVar15;
  char *__s1;
  stat sStack_270;
  undefined8 uStack_1e0;
  undefined8 uStack_1d8;
  undefined8 local_1d0;
  undefined8 uStack_1c8;
  undefined8 uStack_1c0;
  undefined8 uStack_1b8;
  long *local_1b0 [40];
  long local_70;
  
  lVar2 = tpidr_el0;
  local_70 = *(long *)(lVar2 + 0x28);
  puVar8 = (uint *)__errno();
  uVar1 = *puVar8;
  __dirp = opendir("/proc/self/task");
  if (__dirp != (DIR *)0x0) {
    iVar5 = dirfd(__dirp);
    if (-1 < iVar5) {
      __s1 = "task_directory_fd";
LAB_00152bfc:
      *puVar8 = 0;
      pdVar9 = readdir(__dirp);
      if (pdVar9 != (dirent *)0x0) {
        pcVar14 = pdVar9->d_name;
        if (*pcVar14 == '.') {
          iVar7 = 5;
        }
        else {
          sStack_270.__unused[2] = 0;
          sStack_270.__unused[1] = 0;
          uStack_1d8 = 0;
          uStack_1e0 = 0;
          uStack_1c8 = 0;
          local_1d0 = 0;
          uStack_1b8 = 0;
          uStack_1c0 = 0;
          uVar6 = FUN_0015540c(local_1b0,0x140,0x140,"/proc/self/task/%s/comm",pcVar14);
          if (uVar6 < 0x140) {
            do {
              iVar7 = __open_2(local_1b0,0x80000);
              if (-1 < iVar7) goto LAB_00152cb4;
            } while (*puVar8 == 4);
            if ((*puVar8 & 0xfffffffe) == 2) {
              do {
                iVar7 = fstatat(iVar5,pcVar14,&sStack_270,0x100);
                if (-1 < iVar7) goto LAB_00152cec;
              } while (*puVar8 == 4);
              if (*puVar8 != 2) goto LAB_00152cec;
              iVar7 = 5;
            }
            else {
LAB_00152cec:
              iVar7 = 4;
            }
            __s1 = "task_comm_open";
          }
          else {
            iVar7 = 4;
            __s1 = "task_comm_path";
          }
        }
        goto LAB_00152bf4;
      }
      bVar3 = *puVar8 == 0;
      __s1 = "";
      if (!bVar3) {
        __s1 = "task_directory_read";
      }
      goto LAB_00152df4;
    }
    bVar3 = false;
    __s1 = "task_directory_fd";
    goto LAB_00152df4;
  }
  bVar3 = false;
  __s1 = "task_directory_open";
  *puVar8 = uVar1;
LAB_00152e04:
  if (__s1 == (char *)0x0) goto LAB_00152e84;
  iVar5 = strcmp(__s1,"mainloop_live");
  if ((((iVar5 == 0) && (local_1b0[0] = (long *)0x0, DAT_0022e010 != (long *)0x0)) &&
      (iVar5 = (**(code **)(*DAT_0022e010 + 0x30))(DAT_0022e010,local_1b0,0x10006), iVar5 == 0)) &&
     ((local_1b0[0] != (long *)0x0 &&
      (cVar4 = (**(code **)(*local_1b0[0] + 0x720))(), cVar4 == '\0')))) {
    iVar5 = (**(code **)(*local_1b0[0] + 0x98))(local_1b0[0],8);
    if (iVar5 == 0) {
      lVar11 = (**(code **)(*local_1b0[0] + 0x30))(local_1b0[0],"nexus/loader/NexusLoader");
      if ((((lVar11 == 0) || (cVar4 = (**(code **)(*local_1b0[0] + 0x720))(), cVar4 != '\0')) ||
          ((lVar13 = (**(code **)(*local_1b0[0] + 0x388))
                               (local_1b0[0],lVar11,"statusJson","()Ljava/lang/String;"),
           lVar13 == 0 ||
           (((cVar4 = (**(code **)(*local_1b0[0] + 0x720))(), cVar4 != '\0' ||
             (lVar11 = (**(code **)(*local_1b0[0] + 0x390))(local_1b0[0],lVar11,lVar13), lVar11 == 0
             )) || (cVar4 = (**(code **)(*local_1b0[0] + 0x720))(), cVar4 != '\0')))))) ||
         (pcVar14 = (char *)(**(code **)(*local_1b0[0] + 0x548))(local_1b0[0],lVar11,0),
         pcVar14 == (char *)0x0)) {
        bVar3 = false;
      }
      else {
        iVar5 = strncmp(pcVar14,
                        "{\"schema\":1,\"phase\":\"linking\",\"game_jni_boundary_observed\":true,\"synchronous_link_barrier_complete\":false,"
                        ,0x6a);
        if (iVar5 == 0) {
          pcVar15 = strstr(pcVar14,"\"process_restart_required\":false,\"error\":");
          bVar3 = pcVar15 != (char *)0x0;
        }
        else {
          bVar3 = false;
        }
        (**(code **)(*local_1b0[0] + 0x550))(local_1b0[0],lVar11,pcVar14);
      }
      cVar4 = (**(code **)(*local_1b0[0] + 0x720))();
      if (cVar4 != '\0') {
        (**(code **)(*local_1b0[0] + 0x88))();
      }
      (**(code **)(*local_1b0[0] + 0xa0))(local_1b0[0],0);
    }
    else {
      bVar3 = false;
    }
    cVar4 = (**(code **)(*local_1b0[0] + 0x720))();
    if (cVar4 != '\0') {
      (**(code **)(*local_1b0[0] + 0x88))();
    }
    if (!bVar3) goto LAB_00152f34;
    goto LAB_00152e88;
  }
  goto LAB_00152f34;
  while (uVar6 = *puVar8, uVar6 == 4) {
LAB_00152cb4:
    sVar10 = read(iVar7,sStack_270.__unused + 1,0x3f);
    if (-1 < sVar10) {
      close(iVar7);
      if (sVar10 == 0) goto LAB_00152d50;
      if (sStack_270.__unused[1] == 0x706f6f6c6e69614d && (short)sStack_270.__unused[2] == 10) {
        iVar7 = 4;
      }
      else {
        iVar7 = (uint)(sStack_270.__unused[1] == 0x706f6f6c6e69614d &&
                      (char)sStack_270.__unused[2] == '\0') << 2;
      }
      __s1 = "mainloop_live";
      goto LAB_00152bf4;
    }
  }
  close(iVar7);
  if ((uVar6 & 0xfffffffe) == 2) {
LAB_00152d50:
    do {
      iVar7 = fstatat(iVar5,pcVar14,&sStack_270,0x100);
      if (-1 < iVar7) goto LAB_00152d84;
    } while (*puVar8 == 4);
    if (*puVar8 == 2) {
      iVar7 = 5;
      goto LAB_00152d88;
    }
  }
LAB_00152d84:
  iVar7 = 4;
LAB_00152d88:
  __s1 = "task_comm_read";
LAB_00152bf4:
  if (iVar7 == 4) goto LAB_00152df0;
  goto LAB_00152bfc;
LAB_00152df0:
  bVar3 = false;
LAB_00152df4:
  closedir(__dirp);
  *puVar8 = uVar1;
  if (!bVar3) goto LAB_00152e04;
LAB_00152e84:
  if (bVar3) {
LAB_00152e88:
    uVar12 = 1;
    goto LAB_00152f40;
  }
LAB_00152f34:
  uVar12 = 0;
  PTR_s_ui_initialize_001a7c40 = __s1;
LAB_00152f40:
  if (*(long *)(lVar2 + 0x28) != local_70) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar12);
  }
  return;
}

/* ===== FUN_00155e08 @ 00155e08 ===== */

int FUN_00155e08(void)

{
  int iVar1;
  int iVar2;
  
  iVar2 = nexus_menu_status();
  iVar1 = iVar2 >> 0x1f;
  if (iVar2 == 3) {
    iVar1 = 1;
  }
  return iVar1;
}

/* ===== FUN_00155e28 @ 00155e28 ===== */

void FUN_00155e28(long *param_1)

{
  long lVar1;
  undefined1 auStack_838 [2048];
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  nexus_menu_diagnostics(auStack_838,0x800);
  (**(code **)(*param_1 + 0x538))(param_1,auStack_838);
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_001560d8 @ 001560d8 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_001560d8(int *param_1)

{
  byte *pbVar1;
  long lVar2;
  char *pcVar3;
  char *pcVar4;
  uint uVar5;
  long lVar6;
  bool bVar7;
  int iVar8;
  int iVar9;
  __uid_t _Var10;
  uint uVar11;
  __pid_t _Var12;
  undefined4 uVar13;
  long lVar14;
  int *piVar15;
  undefined4 *puVar16;
  ulong uVar17;
  ulong uVar18;
  byte bVar19;
  uint uVar20;
  byte bVar23;
  byte bVar24;
  byte bVar25;
  undefined1 auVar21 [16];
  undefined1 auVar22 [16];
  long local_ec;
  int local_e4;
  uint local_e0;
  uint local_dc;
  timespec local_d8;
  uint local_c8;
  int local_c4;
  __uid_t local_c0;
  long local_a8;
  long local_58;
  
  lVar6 = tpidr_el0;
  uVar17 = (ulong)param_1 & 0xffffffff;
  local_58 = *(long *)(lVar6 + 0x28);
  if (-1 < (int)param_1) {
    iVar8 = __openat_2(uVar17,"performance.bin",0x88000);
    if (iVar8 < 0) {
      param_1 = (int *)__errno();
      bVar7 = *param_1 == 2;
      uVar11 = 0;
      goto LAB_0015630c;
    }
    iVar9 = fstat(iVar8,(stat *)&local_d8);
    if ((((iVar9 == 0) && ((local_c8 & 0xf000) == 0x8000)) &&
        (_Var10 = getuid(), local_c0 == _Var10)) &&
       (((local_c4 == 1 && ((local_c8 & 0x3f) == 0)) && (local_a8 == 0x14)))) {
      uVar18 = 0;
      do {
        lVar14 = __read_chk(iVar8,(long)&local_ec + uVar18,0x14 - uVar18,0xffffffffffffffff);
        if ((lVar14 < 0) && (piVar15 = (int *)__errno(), *piVar15 == 4)) {
          iVar9 = 2;
        }
        else {
          lVar2 = 0;
          if (0 < lVar14) {
            lVar2 = lVar14;
          }
          uVar18 = lVar2 + uVar18;
          iVar9 = 3;
          if (0 < lVar14) {
            iVar9 = 0;
          }
        }
      } while ((iVar9 != 3) && (uVar18 < 0x14));
      uVar11 = close(iVar8);
      param_1 = (int *)(ulong)uVar11;
      if ((uVar18 == 0x14) &&
         (((local_ec == 0x323532393650584e && (local_e4 == 1)) && (local_e0 < 2)))) {
        lVar14 = 0;
        uVar11 = 0xffffffff;
        do {
          pbVar1 = (byte *)((long)&local_ec + lVar14);
          lVar14 = lVar14 + 1;
          uVar20 = uVar11 ^ *pbVar1;
          uVar5 = -(uVar20 & 1) & 0xedb88320 ^ uVar20 >> 1;
          bVar19 = (byte)uVar20;
          auVar21[0] = bVar19 & (byte)_DAT_0010f9c0;
          bVar23 = (byte)(uVar11 >> 8);
          auVar21[1] = bVar23 & (byte)((ulong)_DAT_0010f9c0 >> 8);
          bVar24 = (byte)(uVar11 >> 0x10);
          auVar21[2] = bVar24 & (byte)((ulong)_DAT_0010f9c0 >> 0x10);
          bVar25 = (byte)(uVar11 >> 0x18);
          auVar21[3] = bVar25 & (byte)((ulong)_DAT_0010f9c0 >> 0x18);
          auVar21[4] = bVar19 & (byte)((ulong)_DAT_0010f9c0 >> 0x20);
          auVar21[5] = bVar23 & (byte)((ulong)_DAT_0010f9c0 >> 0x28);
          auVar21[6] = bVar24 & (byte)((ulong)_DAT_0010f9c0 >> 0x30);
          auVar21[7] = bVar25 & (byte)((ulong)_DAT_0010f9c0 >> 0x38);
          auVar21[8] = bVar19 & (byte)_UNK_0010f9c8;
          auVar21[9] = bVar23 & (byte)((ulong)_UNK_0010f9c8 >> 8);
          auVar21[10] = bVar24 & (byte)((ulong)_UNK_0010f9c8 >> 0x10);
          auVar21[0xb] = bVar25 & (byte)((ulong)_UNK_0010f9c8 >> 0x18);
          auVar21[0xc] = bVar19 & (byte)((ulong)_UNK_0010f9c8 >> 0x20);
          auVar21[0xd] = bVar23 & (byte)((ulong)_UNK_0010f9c8 >> 0x28);
          auVar21[0xe] = bVar24 & (byte)((ulong)_UNK_0010f9c8 >> 0x30);
          auVar21[0xf] = bVar25 & (byte)((ulong)_UNK_0010f9c8 >> 0x38);
          uVar11 = (int)(uVar20 << 0x1e) >> 0x1f & 0xedb88320U ^ uVar5 >> 1;
          auVar21 = NEON_cmeq(auVar21,0,2);
          auVar22[0] = (byte)_DAT_0010fac0 & ~auVar21[0];
          auVar22[1] = (byte)((ulong)_DAT_0010fac0 >> 8) & ~auVar21[1];
          auVar22[2] = (byte)((ulong)_DAT_0010fac0 >> 0x10) & ~auVar21[2];
          auVar22[3] = (byte)((ulong)_DAT_0010fac0 >> 0x18) & ~auVar21[3];
          auVar22[4] = (byte)((ulong)_DAT_0010fac0 >> 0x20) & ~auVar21[4];
          auVar22[5] = (byte)((ulong)_DAT_0010fac0 >> 0x28) & ~auVar21[5];
          auVar22[6] = (byte)((ulong)_DAT_0010fac0 >> 0x30) & ~auVar21[6];
          auVar22[7] = (byte)((ulong)_DAT_0010fac0 >> 0x38) & ~auVar21[7];
          auVar22[8] = (byte)_UNK_0010fac8 & ~auVar21[8];
          auVar22[9] = (byte)((ulong)_UNK_0010fac8 >> 8) & ~auVar21[9];
          auVar22[10] = (byte)((ulong)_UNK_0010fac8 >> 0x10) & ~auVar21[10];
          auVar22[0xb] = (byte)((ulong)_UNK_0010fac8 >> 0x18) & ~auVar21[0xb];
          auVar22[0xc] = (byte)((ulong)_UNK_0010fac8 >> 0x20) & ~auVar21[0xc];
          auVar22[0xd] = (byte)((ulong)_UNK_0010fac8 >> 0x28) & ~auVar21[0xd];
          auVar22[0xe] = (byte)((ulong)_UNK_0010fac8 >> 0x30) & ~auVar21[0xe];
          auVar22[0xf] = (byte)((ulong)_UNK_0010fac8 >> 0x38) & ~auVar21[0xf];
          auVar21 = NEON_ext(auVar22,auVar22,8,1);
          uVar11 = (int)(uVar11 << 0x1a) >> 0x1f & 0xedb88320U ^ uVar11 >> 6;
          param_1 = (int *)(ulong)uVar11;
          uVar20 = CONCAT13(auVar22[3] ^ auVar21[3],
                            CONCAT12(auVar22[2] ^ auVar21[2],
                                     CONCAT11(auVar22[1] ^ auVar21[1],auVar22[0] ^ auVar21[0])));
          uVar11 = uVar20 ^ (int)(uVar5 << 0x1a) >> 0x1f & 0x76dc4190U ^ uVar11 ^
                   (uint)(CONCAT17(auVar22[7] ^ auVar21[7],
                                   CONCAT16(auVar22[6] ^ auVar21[6],
                                            CONCAT15(auVar22[5] ^ auVar21[5],
                                                     CONCAT14(auVar22[4] ^ auVar21[4],uVar20)))) >>
                         0x20);
        } while (lVar14 != 0x10);
        bVar7 = local_dc == ~uVar11;
        uVar11 = local_e0;
        if (!bVar7) {
          uVar11 = 0;
        }
        goto LAB_0015630c;
      }
    }
    else {
      uVar11 = close(iVar8);
      param_1 = (int *)(ulong)uVar11;
    }
  }
  bVar7 = false;
  uVar11 = 0;
LAB_0015630c:
  DAT_0022d880._0_4_ = 0;
  if (bVar7) {
    DAT_0022d880._0_4_ = uVar11;
  }
  DAT_0022f4b8 = (uint)DAT_0022d880;
  puVar16 = (undefined4 *)__errno(param_1);
  uVar13 = *puVar16;
  local_d8.tv_nsec._0_4_ = 0;
  local_d8.tv_sec = 0;
  DAT_0022dfb0 = FUN_00155a40(uVar17,&local_d8);
  _DAT_0022e000 = local_d8.tv_sec;
  DAT_0022e004._4_4_ = (undefined4)local_d8.tv_nsec;
  DAT_0022dfb4 = (uint)(DAT_0022dfb0 != 0 && (int)local_d8.tv_sec != 0);
  if (DAT_0022dfb0 != 0 && (int)local_d8.tv_sec != 0) {
    DAT_0022f4b8 = 1;
  }
  *puVar16 = uVar13;
  pcVar3 = "optimization_off";
  if (uVar11 != 0) {
    pcVar3 = "optimization_on";
  }
  iVar8 = DAT_0022f4bc + 1;
  pcVar4 = "invalid_or_unreadable_default_off";
  if (bVar7) {
    pcVar4 = pcVar3;
  }
  bVar7 = DAT_0022f4bc < 0x80;
  DAT_0022f4bc = iVar8;
  if (bVar7) {
    _Var12 = getpid();
    uVar13 = gettid(_Var12);
    iVar8 = clock_gettime(0,&local_d8);
    if (iVar8 == 0) {
      lVar14 = local_d8.tv_sec * 1000 +
               CONCAT44(local_d8.tv_nsec._4_4_,(undefined4)local_d8.tv_nsec) / 1000000;
    }
    else {
      lVar14 = 0;
    }
    __android_log_print(4,"NexusLab69252",
                        "{\"stage\":\"%s\",\"reason\":\"%s\",\"action\":%u,\"pid\":%d,\"tid\":%d,\"time_ms\":%llu}"
                        ,"performance_settings",pcVar4,0,_Var12,uVar13,lVar14);
  }
  if (*(long *)(lVar6 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00156480 @ 00156480 ===== */

void FUN_00156480(undefined8 param_1,long param_2,ulong param_3)

{
  long lVar1;
  bool bVar2;
  __pid_t _Var3;
  int iVar4;
  int iVar5;
  int *piVar6;
  long lVar7;
  ulong uVar8;
  timespec local_98 [5];
  long local_48;
  
  lVar1 = tpidr_el0;
  bVar2 = false;
  local_48 = *(long *)(lVar1 + 0x28);
  if (((param_3 < 0x1001) && (param_2 != 0)) && (-1 < DAT_001a7c50)) {
    _Var3 = getpid();
    iVar4 = clock_gettime(1,local_98);
    if (iVar4 == 0) {
      lVar7 = local_98[0].tv_sec * 1000 + (ulong)local_98[0].tv_nsec / 1000000;
    }
    else {
      lVar7 = 0;
    }
    FUN_0015540c(local_98,0x50,0x50,"settings-%d-%llu.tmp",_Var3,lVar7);
    iVar4 = openat(DAT_001a7c50,(char *)local_98,0x880c1,0x180);
    if (-1 < iVar4) {
      uVar8 = 0;
      if (param_3 != 0) {
        do {
          lVar7 = __write_chk(iVar4,param_2 + uVar8,param_3 - uVar8,0xffffffffffffffff);
          if (lVar7 < 0) {
            piVar6 = (int *)__errno();
            if (*piVar6 != 4) break;
          }
          else {
            uVar8 = lVar7 + uVar8;
            if (lVar7 == 0) break;
          }
        } while (uVar8 < param_3);
      }
      if (uVar8 == param_3) {
        iVar5 = fsync(iVar4);
        close(iVar4);
        if ((iVar5 == 0) &&
           (iVar4 = renameat(DAT_001a7c50,(char *)local_98,DAT_001a7c50,"settings.bin"), iVar4 == 0)
           ) {
          iVar4 = fsync(DAT_001a7c50);
          bVar2 = iVar4 == 0;
          goto LAB_001565e8;
        }
      }
      else {
        close(iVar4);
      }
    }
    bVar2 = false;
  }
LAB_001565e8:
  if (*(long *)(lVar1 + 0x28) != local_48) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(bVar2);
  }
  return;
}

/* ===== FUN_00156614 @ 00156614 ===== */

void FUN_00156614(undefined8 param_1,long param_2,ulong param_3,ulong *param_4)

{
  long lVar1;
  bool bVar2;
  int __fd;
  int iVar3;
  __uid_t _Var4;
  int *piVar5;
  long lVar6;
  ulong uVar7;
  uint local_b8;
  int local_b4;
  __uid_t local_b0;
  ulong local_98;
  long local_48;
  
  lVar1 = tpidr_el0;
  bVar2 = false;
  local_48 = *(long *)(lVar1 + 0x28);
  if ((((param_3 < 0x1001) && (param_4 != (ulong *)0x0)) && (param_2 != 0)) && (-1 < DAT_001a7c50))
  {
    __fd = __openat_2(DAT_001a7c50,"settings.bin",0x88000);
    if (-1 < __fd) {
      iVar3 = fstat(__fd,(stat *)&stack0xffffffffffffff38);
      if ((((iVar3 == 0) && ((local_b8 & 0xf000) == 0x8000)) &&
          ((_Var4 = getuid(), local_b0 == _Var4 && ((local_b4 == 1 && (-1 < (long)local_98)))))) &&
         ((long)local_98 <= (long)param_3)) {
        uVar7 = 0;
        if (local_98 != 0) {
          do {
            lVar6 = __read_chk(__fd,param_2 + uVar7,local_98 - uVar7,0xffffffffffffffff);
            if (lVar6 < 0) {
              piVar5 = (int *)__errno();
              if (*piVar5 != 4) break;
            }
            else {
              uVar7 = lVar6 + uVar7;
              if (lVar6 == 0) break;
            }
          } while (uVar7 < local_98);
        }
        close(__fd);
        bVar2 = uVar7 == local_98;
        *param_4 = uVar7;
        goto LAB_001566e4;
      }
      close(__fd);
    }
    bVar2 = false;
  }
LAB_001566e4:
  if (*(long *)(lVar1 + 0x28) != local_48) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(bVar2);
  }
  return;
}

/* ===== FUN_00156760 @ 00156760 ===== */

void FUN_00156760(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined4 param_4)

{
  bool bVar1;
  long lVar2;
  __pid_t _Var3;
  undefined4 uVar4;
  int iVar5;
  long lVar6;
  timespec local_58;
  long local_48;
  
  lVar2 = tpidr_el0;
  local_48 = *(long *)(lVar2 + 0x28);
  iVar5 = DAT_0022f4bc + 1;
  bVar1 = DAT_0022f4bc < 0x80;
  DAT_0022f4bc = iVar5;
  if (bVar1) {
    _Var3 = getpid();
    uVar4 = gettid(_Var3);
    iVar5 = clock_gettime(0,&local_58);
    if (iVar5 == 0) {
      lVar6 = local_58.tv_sec * 1000 + (ulong)local_58.tv_nsec / 1000000;
    }
    else {
      lVar6 = 0;
    }
    __android_log_print(4,"NexusLab69252",
                        "{\"stage\":\"%s\",\"reason\":\"%s\",\"action\":%u,\"pid\":%d,\"tid\":%d,\"time_ms\":%llu}"
                        ,param_2,param_3,param_4,_Var3,uVar4,lVar6);
  }
  if (*(long *)(lVar2 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00156850 @ 00156850 ===== */

void FUN_00156850(undefined8 param_1,undefined8 *param_2)

{
  undefined *puVar1;
  char *pcVar2;
  long lVar3;
  undefined8 uVar4;
  long lVar5;
  long lVar6;
  int iVar7;
  int iVar8;
  undefined8 uVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  ulong uVar13;
  char *pcVar14;
  undefined1 auStack_2360 [16];
  uint local_2350;
  long local_2348;
  char local_2340;
  undefined1 auStack_1340 [16];
  uint local_1330;
  long local_1328;
  char local_1320 [4073];
  undefined1 auStack_337 [23];
  undefined8 local_320;
  undefined8 uStack_318;
  undefined8 local_310;
  undefined8 uStack_308;
  undefined8 local_300;
  undefined8 uStack_2f8;
  undefined8 local_2f0;
  undefined8 uStack_2e8;
  undefined8 local_2e0;
  undefined8 uStack_2d8;
  undefined8 local_2d0;
  undefined8 uStack_2c8;
  undefined8 local_2c0;
  undefined8 uStack_2b8;
  undefined8 local_2b0;
  undefined8 uStack_2a8;
  undefined8 local_2a0;
  undefined8 uStack_298;
  undefined8 local_290;
  undefined8 uStack_288;
  undefined8 local_280;
  undefined8 uStack_278;
  undefined8 local_270;
  undefined8 uStack_268;
  undefined8 local_260;
  undefined8 uStack_258;
  undefined8 local_250;
  undefined8 uStack_248;
  undefined8 local_240;
  undefined8 uStack_238;
  undefined8 local_230;
  undefined8 uStack_228;
  undefined1 auStack_218 [54];
  char acStack_1e2 [10];
  undefined5 local_1d8;
  undefined3 uStack_1d3;
  undefined5 local_1d0;
  undefined3 auStack_1cb [76];
  char acStack_98 [64];
  long local_58;
  
  lVar3 = tpidr_el0;
  local_58 = *(long *)(lVar3 + 0x28);
  memcpy(auStack_1340,&PTR_s_libNexusEvasion69252_so_00198c78,0x1020);
  memcpy(auStack_2360,&PTR_s_libNexusEvasionRuntime69252_so_00199c98,0x1020);
  dl_iterate_phdr(FUN_00156d68,auStack_1340);
  dl_iterate_phdr(FUN_00156d68,auStack_2360);
  uVar9 = 0xffffffff;
  if (((((1 < local_1330) || (1 < local_2350)) || (uVar9 = 0, local_1330 == 0)) ||
      (((local_2350 == 0 || (uVar9 = 0xffffffff, local_1320[0] == '\0')) ||
       ((local_2340 == '\0' || ((local_1328 == 0 || (local_2348 == 0)))))))) ||
     ((uVar9 = FUN_00156f1c(auStack_1340,&DAT_00110010,0x1d8f0), (int)uVar9 == 0 ||
      (uVar9 = FUN_00156f1c(auStack_2360,&DAT_00110030,0xc46e0), (int)uVar9 == 0))))
  goto LAB_00156bcc;
  iVar7 = FUN_001574ec(local_1320,auStack_218);
  if ((iVar7 != 0) && (iVar7 = FUN_00157740(auStack_218,0,0), -1 < iVar7)) {
    uStack_228 = 0;
    local_230 = 0;
    uStack_238 = 0;
    local_240 = 0;
    uStack_248 = 0;
    local_250 = 0;
    uStack_258 = 0;
    local_260 = 0;
    uStack_268 = 0;
    local_270 = 0;
    uStack_278 = 0;
    local_280 = 0;
    uStack_288 = 0;
    local_290 = 0;
    uStack_298 = 0;
    local_2a0 = 0;
    uStack_2a8 = 0;
    local_2b0 = 0;
    uStack_2b8 = 0;
    local_2c0 = 0;
    uStack_2c8 = 0;
    local_2d0 = 0;
    uStack_2d8 = 0;
    local_2e0 = 0;
    uStack_2e8 = 0;
    local_2f0 = 0;
    uStack_2f8 = 0;
    local_300 = 0;
    uStack_308 = 0;
    local_310 = 0;
    uStack_318 = 0;
    local_320 = 0;
    lVar10 = dlopen(auStack_218,6);
    if (lVar10 != 0) {
LAB_001569f0:
      close(iVar7);
      lVar11 = dlsym(lVar10,"nexus_evasion_menu_backend_v1");
      lVar12 = dlsym(lVar10,"nexus_evasion_bind_image_v1");
      lVar5 = local_1328;
      if (((lVar11 == 0) ||
          (((iVar7 = dladdr(lVar11,&local_1d8), lVar6 = local_1328, iVar7 == 0 ||
            (CONCAT35(auStack_1cb[0],local_1d0) != lVar5)) || (lVar12 == 0)))) ||
         ((iVar7 = dladdr(lVar12,&local_1d8), iVar7 == 0 ||
          (CONCAT35(auStack_1cb[0],local_1d0) != lVar6)))) {
        dlclose(lVar10);
        uVar9 = 0xffffffff;
      }
      else {
        uVar9 = 1;
        param_2[3] = lVar11;
        param_2[4] = lVar12;
        uVar4 = DAT_0010f7d8;
        param_2[1] = lVar10;
        param_2[2] = local_1328;
        *param_2 = uVar4;
      }
      goto LAB_00156bcc;
    }
    iVar8 = FUN_001574ec(auStack_218,acStack_98);
    if (((((iVar8 != 0) &&
          (uVar13 = readlink(acStack_98,(char *)&local_1d8,0x13f),
          0xfffffffffffffec1 < uVar13 - 0x13f)) &&
         (*(undefined1 *)((long)&local_1d8 + uVar13) = 0, 0x17 < uVar13)) &&
        ((CONCAT35(uStack_1d3,local_1d8) == 0x6e3a64666d656d2f &&
          CONCAT53(local_1d0,uStack_1d3) == 0x2d737578656e3a64 &&
         (iVar8 = strcmp(acStack_1e2 + uVar13," (deleted)"), iVar8 == 0)))) &&
       (uVar13 - 0x17 < 0x100)) {
      __memcpy_chk(&local_320,auStack_1cb,uVar13 - 0x17,0x100);
      auStack_337[uVar13] = 0;
      uVar13 = __strlen_chk(&local_320,0x100);
      if ((0x41 < uVar13) && (lVar10 = dlopen((long)&local_2e0 + 1,6), lVar10 != 0))
      goto LAB_001569f0;
    }
    pcVar14 = (char *)dlerror();
    uVar13 = __strlen_chk(&local_320,0x100);
    puVar1 = (undefined *)((long)&local_2e0 + 1);
    if (uVar13 < 0x42) {
      puVar1 = &DAT_00134f22;
    }
    pcVar2 = "unknown";
    if (pcVar14 != (char *)0x0) {
      pcVar2 = pcVar14;
    }
    __android_log_print(6,"NexusMem","resident lookup failed descriptor=%s soname=%s error=%s",
                        auStack_218,puVar1,pcVar2);
    close(iVar7);
  }
  uVar9 = 0;
LAB_00156bcc:
  if (*(long *)(lVar3 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar9);
}

/* ===== FUN_00156c50 @ 00156c50 ===== */

void FUN_00156c50(undefined8 param_1,undefined4 param_2,undefined8 param_3)

{
  nexus_menu_register_backend(param_2,param_3);
  return;
}

/* ===== FUN_00156c5c @ 00156c5c ===== */

bool FUN_00156c5c(void)

{
  int iVar1;
  
  nexus_menu_start();
  iVar1 = nexus_menu_status();
  return iVar1 != 0;
}

/* ===== FUN_00156c7c @ 00156c7c ===== */

void FUN_00156c7c(undefined8 param_1,undefined4 param_2)

{
  bool bVar1;
  long lVar2;
  __pid_t _Var3;
  undefined4 uVar4;
  int iVar5;
  undefined8 uVar6;
  long lVar7;
  timespec local_48;
  long local_38;
  
  lVar2 = tpidr_el0;
  local_38 = *(long *)(lVar2 + 0x28);
  uVar6 = FUN_00169ab8(param_2);
  iVar5 = DAT_0022f4bc + 1;
  bVar1 = DAT_0022f4bc < 0x80;
  DAT_0022f4bc = iVar5;
  if (bVar1) {
    _Var3 = getpid();
    uVar4 = gettid(_Var3);
    iVar5 = clock_gettime(0,&local_48);
    if (iVar5 == 0) {
      lVar7 = local_48.tv_sec * 1000 + (ulong)local_48.tv_nsec / 1000000;
    }
    else {
      lVar7 = 0;
    }
    __android_log_print(4,"NexusLab69252",
                        "{\"stage\":\"%s\",\"reason\":\"%s\",\"action\":%u,\"pid\":%d,\"tid\":%d,\"time_ms\":%llu}"
                        ,"nexus_ui_backend_binding",uVar6,0,_Var3,uVar4,lVar7);
  }
  if (*(long *)(lVar2 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0015d224 @ 0015d224 ===== */

/* WARNING: Removing unreachable block (ram,0x0015dc94) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0015d224(long param_1,undefined8 param_2,ulong param_3)

{
  char *pcVar1;
  undefined4 uVar2;
  long lVar3;
  float fVar4;
  byte bVar5;
  long *plVar6;
  __time_t _Var7;
  __time_t _Var8;
  bool bVar9;
  bool bVar10;
  char cVar11;
  char cVar12;
  char cVar13;
  int iVar14;
  int iVar15;
  int iVar16;
  __pid_t _Var17;
  int iVar18;
  undefined4 *puVar19;
  ulong uVar20;
  pthread_t __target_thread;
  undefined8 uVar21;
  long lVar22;
  long lVar23;
  ulong uVar24;
  undefined *puVar25;
  long lVar26;
  long lVar27;
  char *pcVar28;
  char *pcVar29;
  ulong uVar30;
  uint uVar31;
  uint uVar32;
  undefined4 uVar33;
  ulong uVar34;
  undefined8 local_1790;
  int local_1788;
  char local_1784 [4];
  long local_1780;
  undefined8 local_1778;
  undefined8 local_1770;
  undefined1 local_1768;
  undefined1 uStack_1767;
  undefined2 uStack_1766;
  float fStack_1764;
  char local_1760;
  undefined7 uStack_175f;
  int local_1750;
  int local_174c;
  int local_1744;
  timespec local_10e0;
  code *local_10d0;
  code *pcStack_10c8;
  undefined8 local_10c0;
  code *pcStack_10b8;
  undefined8 local_10b0;
  undefined8 uStack_10a8;
  undefined8 local_10a0;
  undefined8 uStack_1098;
  undefined8 local_1090;
  undefined8 uStack_1088;
  undefined8 local_1080;
  long lStack_1078;
  long local_1070;
  ulong uStack_1068;
  undefined8 local_d8;
  undefined8 local_d0;
  long local_c8;
  long lStack_c0;
  long lStack_b8;
  long lStack_b0;
  long local_a8;
  int local_a0;
  char local_9c;
  undefined1 local_9b;
  undefined1 local_9a;
  undefined1 local_99;
  long local_78;
  
  lVar3 = tpidr_el0;
  local_78 = *(long *)(lVar3 + 0x28);
  puVar19 = (undefined4 *)__errno();
  uVar2 = *puVar19;
  if (((int)DAT_0022db78 == 0) || (uVar20 = FUN_00193f80(1,&DAT_0025ca14), (uVar20 & 1) != 0))
  goto LAB_0015e35c;
  local_1780 = 0;
  local_1778 = 0;
  __target_thread = pthread_self();
  iVar14 = pthread_getname_np(__target_thread,(char *)&local_1780,0x10);
  if (iVar14 == 0) {
    iVar14 = gettid();
    iVar15 = clock_gettime(1,&local_10e0);
    if (iVar15 == 0) {
      uVar20 = local_10e0.tv_sec * 1000 + (ulong)local_10e0.tv_nsec / 1000000;
    }
    else {
      uVar20 = 0;
    }
    uVar31 = (uint)param_3;
    if (((((int)DAT_0022f044 != 0) && (uVar21 = FUN_00169574(&DAT_0022f040), (int)uVar21 < 7)) &&
        (uVar31 == 0)) &&
       (((0 < iVar14 && (local_1780 == 0x706f6f6c6e69614d && (char)local_1778 == '\0')) &&
        ((DAT_0022f038 == 0 || (DAT_0022f038 == iVar14)))))) {
      local_10e0.tv_sec = 0;
      iVar15 = FUN_0014e7c4(uVar21,DAT_0022d8e0 + 0x12eb9f0,&local_10e0,8);
      bVar9 = (local_10e0.tv_sec & 7U) != 0;
      uVar34 = local_10e0.tv_sec;
      if ((iVar15 == 0 || (ulong)local_10e0.tv_sec < 0x1000) || bVar9) {
        uVar34 = 0;
      }
      if ((iVar15 != 0 && 0xfff < (ulong)local_10e0.tv_sec) && !bVar9) {
        lVar22 = FUN_00154cf0(uVar34 + 0x90);
        local_d8 = CONCAT71(local_d8._1_7_,1);
        if (((lVar22 != 0) &&
            (uVar21 = FUN_0014e7c4(lVar22,uVar34 + 0x3c,&local_1770,0x10), (int)uVar21 != 0)) &&
           ((iVar15 = FUN_0014e7c4(uVar21,uVar34 + 0x19c,&local_d8,1), iVar15 != 0 &&
            ((char)local_d8 == '\0')))) {
          if ((ABS((float)local_1770) != INFINITY) && (!NAN(ABS((float)local_1770)))) {
            if ((ABS(local_1770._4_4_) != INFINITY) && (!NAN(ABS(local_1770._4_4_)))) {
              fVar4 = (float)CONCAT22(uStack_1766,CONCAT11(uStack_1767,local_1768));
              if (((((ABS(fVar4) != INFINITY) && (!NAN(ABS(fVar4)))) &&
                   (ABS(fStack_1764) != INFINITY)) &&
                  (((!NAN(ABS(fStack_1764)) && ((float)local_1770 == 0.0)) &&
                   ((local_1770._4_4_ == 0.0 && ((960.0 <= fVar4 && (560.0 <= fStack_1764)))))))) &&
                 ((fVar4 == 8192.0 || fVar4 < 8192.0 != NAN(fVar4) &&
                  (((fStack_1764 == 8192.0 || fStack_1764 < 8192.0 != NAN(fStack_1764) &&
                    (lVar23 = FUN_00154cf0(param_1), lVar23 == DAT_0022d8e0 + 0x11abad8)) &&
                   (iVar15 = FUN_00155084(param_1,lVar22), iVar15 != 0)))))) {
                local_10e0.tv_nsec = DAT_0022d8e0;
                local_10d0 = (code *)0x13667e;
                local_10c0 = FUN_0014e7c4;
                pcStack_10c8 = (code *)0x135b30;
                local_10e0.tv_sec = DAT_0010f748;
                pcStack_10b8 = (code *)0x0;
                FUN_00169594(&DAT_0022f040,&local_10e0,uVar20,1);
              }
            }
          }
        }
      }
    }
    if (param_3 == 3) {
      if ((uVar20 < DAT_0025ca18) && (DAT_0025ca18 - uVar20 < 0x3e9)) {
        bVar9 = false;
      }
      else {
        DAT_0025ca18 = uVar20 + 0x10;
        bVar9 = true;
      }
LAB_0015d588:
      bVar10 = false;
      if (((DAT_0022f03c == 2) && ((DAT_0022d890 & 1) == 0)) &&
         (((DAT_0022d880._4_4_ & 1) == 0 && ((DAT_0022d88c & 1) == 0)))) {
        local_10e0.tv_nsec = 0;
        local_10e0.tv_sec = 0;
        if ((((DAT_0022f038 < 1) || (iVar14 != DAT_0022f038)) ||
            (iVar15 = pthread_getname_np(__target_thread,(char *)&local_10e0,0x10), iVar15 != 0)) ||
           (local_10e0.tv_sec != 0x706f6f6c6e69614d || (local_10e0.tv_nsec & 0xffU) != 0)) {
          bVar10 = false;
        }
        else {
          iVar15 = nexus_menu_ui_state(&local_10e0,iVar14);
          bVar10 = false;
          if (((iVar15 == 1) && (local_10e0.tv_sec._4_1_ != '\0')) &&
             (local_10e0.tv_sec._5_1_ == '\0')) {
            uVar32 = local_10e0.tv_sec._6_1_ - 0x43;
            if ((uVar32 < 0x32) && ((1L << ((ulong)uVar32 & 0x3f) & 0x22002000cd001U) != 0)) {
              bVar10 = true;
            }
            else {
              bVar10 = local_10e0.tv_sec._6_1_ == 0x65;
            }
          }
        }
      }
      iVar15 = nexus_rich_main_frame(uVar20,param_3 != 3,bVar10);
      if (iVar15 != 0) {
        DAT_0022d898 = 0;
        _DAT_0022d8a0 = 0;
      }
    }
    else {
      bVar9 = false;
      iVar15 = 0;
      if (param_3 == 1) goto LAB_0015d588;
    }
    uVar32 = 0;
    if (((!bVar9) && (param_3 != 3 || iVar15 == 0)) && (uVar32 = uVar31, uVar31 == 3)) {
      local_10e0.tv_nsec = 0;
      local_10e0.tv_sec = 0;
      if (((DAT_0022f038 < 1) || (iVar14 != DAT_0022f038)) ||
         ((iVar15 = pthread_getname_np(__target_thread,(char *)&local_10e0,0x10), iVar15 != 0 ||
          (local_10e0.tv_sec != 0x706f6f6c6e69614d || (char)local_10e0.tv_nsec != '\0')))) {
LAB_0015d6c4:
        uVar32 = 3;
      }
      else if (((DAT_0022f03c < 1) || (DAT_0022f178 == 0)) ||
              ((DAT_0022db60 == 0 || ((DAT_0022f180 == 0 || (DAT_0025ca38 == 0)))))) {
        uVar32 = 3;
        DAT_002815b0 = 0;
        DAT_002815b8 = 0;
      }
      else {
        uVar21 = nexus_menu_ui_state(&local_10e0,iVar14);
        if ((int)uVar21 != 1) goto LAB_0015d6c4;
        if ((local_10e0.tv_sec._4_1_ == '\0') || (DAT_0022f03c == 2)) {
          DAT_002815b0 = 0;
          DAT_002815b8 = 0;
        }
        else {
          if (uVar20 <= DAT_002815b0 - 1) {
            DAT_002815b0 = uVar20;
          }
          if (((DAT_002815b8 & 1) == 0) && (0x270 < uVar20 - DAT_002815b0 >> 4)) {
            DAT_002815b8 = 1;
            FUN_00156760(uVar21,"nexus_menu_open_wait","pending_open_deadline",0);
          }
        }
        uVar32 = 3;
        if (local_10e0.tv_sec._4_1_ != '\0') {
          uVar32 = 0;
        }
      }
    }
    if ((((((DAT_0022d890 & 1) == 0) && ((DAT_0022d880._4_4_ & 1) == 0)) && (-1 < DAT_0022f03c)) &&
        (((uVar21 = nexus_menu_status(), 0 < iVar14 && (uVar32 < 2)) &&
         (((int)uVar21 != 0 && (local_1780 == 0x706f6f6c6e69614d && (char)local_1778 == '\0'))))))
       && ((DAT_0022f038 == 0 || (DAT_0022f038 == iVar14)))) {
      local_10e0.tv_sec = 0;
      uVar21 = FUN_0014e7c4(uVar21,DAT_0022d8e0 + 0x12eb9f0,&local_10e0,8);
      bVar9 = (int)uVar21 != 0;
      bVar10 = (local_10e0.tv_sec & 7U) != 0;
      uVar34 = local_10e0.tv_sec;
      if ((!bVar9 || (ulong)local_10e0.tv_sec < 0x1000) || bVar10) {
        uVar34 = 0;
      }
      if ((bVar9 && 0xfff < (ulong)local_10e0.tv_sec) && !bVar10) {
        local_10e0.tv_sec = 0;
        uVar21 = FUN_0014e7c4(uVar21,uVar34 + 0x90,&local_10e0,8);
        _Var7 = local_10e0.tv_sec;
        if ((((int)uVar21 != 0) && (0xfff < (ulong)local_10e0.tv_sec)) &&
           ((local_10e0.tv_sec & 7U) == 0)) {
          local_1784[0] = '\x01';
          uVar21 = FUN_0014e7c4(uVar21,uVar34 + 0x3c,&local_d8,0x10);
          if ((((int)uVar21 != 0) &&
              (iVar15 = FUN_0014e7c4(uVar21,uVar34 + 0x19c,local_1784,1), iVar15 != 0)) &&
             (local_1784[0] == '\0')) {
            if ((ABS((float)local_d8) != INFINITY) && (!NAN(ABS((float)local_d8)))) {
              if ((ABS(local_d8._4_4_) != INFINITY) && (!NAN(ABS(local_d8._4_4_)))) {
                if ((ABS((float)local_d0) != INFINITY) && (!NAN(ABS((float)local_d0)))) {
                  if ((ABS(local_d0._4_4_) != INFINITY) &&
                     ((((((!NAN(ABS(local_d0._4_4_)) && ((float)local_d8 == 0.0)) &&
                         (local_d8._4_4_ == 0.0)) &&
                        ((960.0 <= (float)local_d0 && (560.0 <= local_d0._4_4_)))) &&
                       ((float)local_d0 == 8192.0 ||
                        (float)local_d0 < 8192.0 != NAN((float)local_d0))) &&
                      (local_d0._4_4_ == 8192.0 || local_d0._4_4_ < 8192.0 != NAN(local_d0._4_4_))))
                     ) {
                    if (DAT_0022f03c == 0) {
                      if (((uVar32 == 0) &&
                          (lVar22 = FUN_00154cf0(param_1), lVar22 == DAT_0022d8e0 + 0x11abad8)) &&
                         (iVar15 = FUN_00155084(param_1,_Var7), iVar15 != 0)) {
                        if (DAT_0025ca20 == 0) {
                          DAT_0025ca20 = uVar20;
                        }
                        if (uVar20 - DAT_0025ca20 < 0xea61) {
                          if (DAT_0025ca28 <= uVar20) {
                            DAT_0025ca28 = uVar20 + 200;
                            uVar21 = FUN_001554b0();
                            if ((int)uVar21 == 0) {
                              if (DAT_0025ca30 < 4) {
                                local_10e0.tv_sec = CONCAT71(local_10e0.tv_sec._1_7_,0xff);
                                local_1770 = (long *)CONCAT71(local_1770._1_7_,0xff);
                                uVar21 = FUN_0014e7c4(uVar21,DAT_0022d8e0 + 0x12eb0e8,&local_10e0,1)
                                ;
                                uVar21 = FUN_0014e7c4(uVar21,DAT_0022d8e0 + 0x12eb170,&local_1770,1)
                                ;
                                pcVar29 = "ui_sc_exports_pending";
                                if ((char)local_1770 != '\0') {
                                  pcVar29 = "ui_prefetch_active";
                                }
                                pcVar28 = "asset_probe_read_failed";
                                if ((char)local_10e0.tv_sec != -1) {
                                  pcVar28 = pcVar29;
                                }
                                FUN_00156760(uVar21,"nexus_shell_waiting_assets",pcVar28,0);
                                DAT_0025ca30 = DAT_0025ca30 + 1;
                              }
                            }
                            else {
                              iVar15 = FUN_001608e0(uVar34,_Var7,iVar14);
                              if (iVar15 == -1) goto LAB_0015d98c;
                              if (iVar15 != 0) goto LAB_0015d8c0;
                            }
                          }
                        }
                        else {
                          FUN_001607dc("asset_readiness_deadline");
                        }
                      }
                    }
                    else {
                      if (uVar32 != 1) {
LAB_0015d8c0:
                        if (DAT_0022f03c == 2) {
                          uVar31 = 0x28;
                          if ((int)DAT_0022d880 != 0) {
                            uVar31 = 100;
                          }
                          if ((((DAT_0022d8a4 != 0) && (DAT_0022d8a0 == uVar31)) &&
                              (DAT_0022d898 <= uVar20)) && (uVar20 - DAT_0022d898 < (ulong)uVar31))
                          goto LAB_0015e280;
                          _DAT_0022d8a0 = CONCAT44(1,uVar31);
                          DAT_0022d898 = uVar20;
                        }
                      }
                      if (((uVar34 == DAT_0022f178) && (_Var7 == DAT_0022db60)) &&
                         (iVar15 = FUN_00154d5c(DAT_0022f180,_Var7), iVar15 != 0)) {
                        uVar21 = FUN_0015922c();
                        uVar21 = FUN_00160c3c(uVar21,&local_d8);
                        if ((int)uVar21 != 0) {
                          if ((int)uVar21 == -1) {
LAB_0015d98c:
                            FUN_001607dc(PTR_s_launcher_initializing_001a7c58);
                          }
                          else {
                            if ((DAT_0025ca34 & 1) == 0) {
                              local_1770 = (long *)0x0;
                              DAT_0025ca34 = 1;
                              iVar15 = FUN_00156614(uVar21,&local_10e0,0x1000,&local_1770);
                              if (iVar15 != 0) {
                                uVar21 = nexus_menu_import(&local_10e0,local_1770,iVar14);
                                pcVar29 = "restored";
                                if (1 < (int)uVar21 - 1U) {
                                  pcVar29 = "rejected";
                                }
                                FUN_00156760(uVar21,"nexus_ui_settings_restore",pcVar29,0);
                              }
                            }
                            local_1788 = 0;
                            local_1790 = 0;
                            iVar15 = FUN_001612c8(&local_1788,(long)&local_1790 + 4,&local_1790);
                            _Var8 = local_10e0.tv_sec;
                            if ((uVar32 == 1) || (iVar15 != 0)) {
                              local_10e0.tv_sec = local_10e0.tv_sec & 0xffffffff00000000;
                              if (iVar15 == 0) {
                                if (DAT_0022f180 != param_1) {
                                  iVar16 = (**(code **)(DAT_0025ca38 + 0x18))(param_1,&local_10e0);
                                  iVar18 = (uint)local_10e0.tv_sec;
                                  goto LAB_0015dd00;
                                }
                                uVar24 = FUN_001613ac(iVar14);
                                pcVar29 = "queued";
                                if (1 < (int)uVar24 - 1U) {
                                  pcVar29 = "blocked";
                                }
                                pcVar28 = "nexus_android_overlay_open";
                                uVar30 = 1;
                              }
                              else {
                                iVar16 = 1;
                                local_10e0.tv_sec._4_4_ = SUB84(_Var8,4);
                                local_10e0.tv_sec = CONCAT44(local_10e0.tv_sec._4_4_,local_1788);
                                iVar18 = local_1788;
LAB_0015dd00:
                                if ((iVar16 == 0) || (iVar18 == 0)) goto LAB_0015e0c8;
                                if ((int)local_1790 == 0) {
LAB_0015dd30:
                                  uVar24 = FUN_0016169c(iVar18,&local_1770);
                                  if ((int)uVar24 != 0) goto LAB_0015e058;
                                  uVar24 = local_10e0.tv_sec & 0xffffffff;
                                  if ((uint)local_10e0.tv_sec == 0x97) {
                                    if (499 < uVar20 - DAT_0025ca40) goto LAB_0015de20;
                                    local_1770 = (long *)((ulong)local_1770 & 0xffffffff00000000);
                                    goto LAB_0015e058;
                                  }
                                  if (((uint)local_10e0.tv_sec & 0xfffffffc) == 0x94) {
LAB_0015de20:
                                    uVar24 = FUN_00161970();
LAB_0015e054:
                                    local_1770 = (long *)CONCAT44(local_1770._4_4_,(int)uVar24);
                                    goto LAB_0015e058;
                                  }
                                  if (((uint)local_10e0.tv_sec & 0xfffffffe) == 0x20000) {
                                    uVar33 = 1;
                                    if ((uint)local_10e0.tv_sec == 0x20000) {
                                      uVar33 = 0xffffffff;
                                    }
                                    uVar24 = nexus_menu_scroll_battle(uVar33,iVar14);
                                    goto LAB_0015e054;
                                  }
                                  if ((uint)local_10e0.tv_sec == 0x21028) {
                                    uVar24 = nexus_menu_server_open(iVar14);
                                    goto LAB_0015e054;
                                  }
                                  if ((uint)local_10e0.tv_sec - 0x24000 < 0x30) {
                                    uVar24 = nexus_menu_server_action(uVar24,iVar14);
                                    goto LAB_0015e054;
                                  }
                                  if ((uint)local_10e0.tv_sec == 0x2102f) {
                                    uVar24 = nexus_menu_theme_open(iVar14);
                                    goto LAB_0015e054;
                                  }
                                  if ((uint)local_10e0.tv_sec == 0x21036) {
                                    uVar24 = nexus_script_port_fonts_open();
LAB_0015decc:
                                    local_1770 = (long *)CONCAT44(local_1770._4_4_,
                                                                  (uint)((int)uVar24 != 0));
                                    goto LAB_0015e058;
                                  }
                                  if ((uint)local_10e0.tv_sec == 0x21031) {
                                    uVar24 = nexus_script_port_fps_open();
                                    goto LAB_0015decc;
                                  }
                                  if (((uint)local_10e0.tv_sec & 0xffffffc0) == 0x26000) {
                                    uVar24 = nexus_menu_theme_action(uVar24,iVar14);
                                    goto LAB_0015e054;
                                  }
                                  if ((uint)local_10e0.tv_sec == 0x21030) {
                                    uVar24 = nexus_menu_debug_open(iVar14);
                                    goto LAB_0015e054;
                                  }
                                  if ((uint)local_10e0.tv_sec - 0x27000 < 0x120) {
                                    uVar24 = nexus_menu_debug_action(uVar24,iVar14);
                                    goto LAB_0015e054;
                                  }
                                  if ((uint)local_10e0.tv_sec == 0x21033) {
                                    uVar24 = nexus_menu_profile_open(iVar14);
                                    goto LAB_0015e054;
                                  }
                                  if ((uint)local_10e0.tv_sec - 0x28000 < 6) {
                                    uVar24 = nexus_menu_profile_action(uVar24,iVar14);
                                    goto LAB_0015e054;
                                  }
                                  if ((uint)local_10e0.tv_sec == 0x21035) {
                                    uVar24 = nexus_menu_editor_open(iVar14);
                                    goto LAB_0015e054;
                                  }
                                  if (((uint)local_10e0.tv_sec & 0xfffffe00) == 0x29000) {
                                    uVar24 = nexus_menu_editor_action(uVar24,iVar14);
                                    goto LAB_0015e054;
                                  }
                                  if ((uint)local_10e0.tv_sec - 0x21000 < 0x37) {
                                    uVar24 = FUN_001924cc();
                                    goto LAB_0015e054;
                                  }
                                  if ((uint)local_10e0.tv_sec == 0x20002) {
                                    if (iVar15 == 0) {
                                      uVar24 = FUN_00161b84(uVar20,iVar14);
                                    }
                                    else {
                                      uVar24 = FUN_00161acc(uVar20,iVar14);
                                    }
                                    goto LAB_0015e054;
                                  }
                                  uVar24 = nexus_menu_dispatch(uVar24,0,0,iVar14);
                                  uVar30 = local_10e0.tv_sec & 0xffffffff;
                                  iVar15 = (int)uVar24;
                                  uVar31 = iVar15 - 1;
                                  local_1770 = (long *)CONCAT44(local_1770._4_4_,iVar15);
                                  if (((uint)local_10e0.tv_sec == 0x32) && (uVar31 < 2)) {
                                    if (((int)DAT_0022d880 == 0) &&
                                       ((DAT_0022dfbc == 0 && (DAT_0022d894 == 0))))
                                    goto LAB_0015e058;
                                    DAT_0022d88c = 0;
                                    uVar24 = FUN_001645e0(0,uVar20);
                                    goto LAB_0015e054;
                                  }
                                }
                                else {
                                  uVar24 = FUN_00161538(iVar18,local_1790._4_4_,iVar14);
                                  local_1770 = (long *)CONCAT44(local_1770._4_4_,(int)uVar24);
                                  if ((int)uVar24 == -0x80000000) {
                                    iVar18 = (uint)local_10e0.tv_sec;
                                    goto LAB_0015dd30;
                                  }
LAB_0015e058:
                                  uVar30 = local_10e0.tv_sec & 0xffffffff;
                                  uVar31 = (int)(float)local_1770 - 1;
                                  iVar15 = (int)(float)local_1770;
                                }
                                if (((int)uVar30 == 0x32) && (uVar31 < 2)) {
                                  uVar24 = FUN_00192228(0);
                                  uVar30 = local_10e0.tv_sec & 0xffffffff;
                                  if ((int)uVar24 != 1) {
                                    iVar15 = (int)uVar24;
                                  }
                                }
                                pcVar1 = "pending";
                                if (iVar15 != 2) {
                                  pcVar1 = "blocked";
                                }
                                pcVar28 = "nexus_menu_click";
                                pcVar29 = "acknowledged";
                                if (iVar15 != 1) {
                                  pcVar29 = pcVar1;
                                }
                              }
                              DAT_0022d898 = 0;
                              _DAT_0022d8a0 = 0;
                              FUN_00156760(uVar24,pcVar28,pcVar29,uVar30);
                            }
LAB_0015e0c8:
                            FUN_00161c58(iVar14);
                            iVar15 = nexus_menu_ui_state(&local_a0,iVar14);
                            if (iVar15 == 1) {
                              memset(&local_10e0,0,0x690);
                              lVar22 = local_10e0.tv_nsec;
                              local_10e0.tv_sec = CONCAT44(local_10e0.tv_sec._4_4_,local_a0);
                              local_10e0.tv_nsec._4_4_ = SUB84(lVar22,4);
                              local_10e0.tv_nsec._0_4_ =
                                   CONCAT13(local_99,CONCAT12(local_9b,CONCAT11(local_9c,local_9a)))
                              ;
                              if ((local_9c == '\0') ||
                                 (iVar15 = FUN_0016205c(&local_10e0,iVar14), iVar15 == 1)) {
                                FUN_00162118(&local_10e0);
                                FUN_00162260(&local_10e0);
                                uVar33 = 0x28;
                                if ((int)DAT_0022d880 != 0) {
                                  uVar33 = 100;
                                }
                                _DAT_0022d8a0 = CONCAT44(1,uVar33);
                                DAT_0022d898 = uVar20;
                                FUN_00162390((local_10e0.tv_nsec._1_1_ != '\0' &&
                                             (char)local_10e0.tv_nsec == 'Y') &&
                                             local_10e0.tv_nsec._2_1_ == '\0',uVar20);
                                FUN_0016291c(&local_10e0,iVar14);
                                memcpy(&local_1770,&local_10e0,0x690);
                                uStack_1767 = 0;
                                iVar15 = (**(code **)(DAT_0025ca38 + 0x10))
                                                   ((ulong)local_d0 & 0xffffffff,local_d0._4_4_,
                                                    &local_1770,uVar34,_Var7,uVar20);
                                pcVar29 = "android_overlay_services_waiting";
                                if ((1 < iVar15 - 2U) && (iVar15 != 0)) {
                                  if (iVar15 == -1) {
                                    FUN_001607dc("rich_services_stopped");
                                    goto LAB_0015e280;
                                  }
                                  if (iVar15 != 4) {
                                    pcVar29 = "android_overlay_bridge_ready";
                                  }
                                }
                                uVar21 = FUN_0015922c();
                                iVar15 = FUN_00160c3c(uVar21,&local_d8);
                                if (iVar15 != 0) {
                                  if (iVar15 == -1) goto LAB_0015d98c;
                                  if (DAT_0022f03c == 1) {
                                    DAT_0022f03c = 2;
                                    uVar21 = FUN_00183cdc(3,iVar14,pcVar29);
                                    FUN_00156760(uVar21,"nexus_menu_attached",pcVar29,0);
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                      else {
                        FUN_001607dc("stage_generation_or_launcher_membership");
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
LAB_0015e280:
    if (param_3 == 3) {
      if ((int)DAT_0022db78 == 0) {
LAB_0015e2ec:
        DAT_0022db70 = 0;
      }
      else {
        local_10e0.tv_nsec = 0;
        local_10e0.tv_sec = 0;
        if ((((DAT_0022f038 < 1) || (iVar14 != DAT_0022f038)) ||
            (iVar15 = pthread_getname_np(__target_thread,(char *)&local_10e0,0x10), iVar15 != 0)) ||
           (((local_10e0.tv_sec != 0x706f6f6c6e69614d || (local_10e0.tv_nsec & 0xffU) != 0 ||
             (iVar15 = FUN_0014fac4(), iVar15 == 0)) || ((DAT_0022d880._4_4_ & 1) != 0))))
        goto LAB_0015e2ec;
        if ((DAT_0022f4b8 != 0) && (DAT_0025cb30 == 0)) {
          DAT_0025cb30 = uVar20;
        }
        if ((((DAT_0022f4b8 != 0) && (DAT_0025cb30 != 0)) && (DAT_0025cb30 <= uVar20)) &&
           (0x752 < uVar20 - DAT_0025cb30 >> 5)) {
          DAT_0022f4b8 = 0;
          DAT_0022dfb8 = 1;
          DAT_0025cb30 = 0;
          uVar31 = DAT_0022f4bc + 1;
          DAT_0022d898 = 0;
          _DAT_0022d8a0 = 0;
          bVar9 = DAT_0022f4bc < 0x80;
          DAT_0022f4bc = uVar31;
          if (bVar9) {
            _Var17 = getpid();
            iVar15 = clock_gettime(0,&local_10e0);
            if (iVar15 == 0) {
              lVar22 = local_10e0.tv_sec * 1000 + (ulong)local_10e0.tv_nsec / 1000000;
            }
            else {
              lVar22 = 0;
            }
            __android_log_print(4,"NexusLab69252",
                                "{\"stage\":\"%s\",\"reason\":\"%s\",\"action\":%u,\"pid\":%d,\"tid\":%d,\"time_ms\":%llu}"
                                ,"performance_graphics","saved_mode_restore_unavailable",0x20002,
                                _Var17,iVar14,lVar22);
          }
        }
        uVar31 = DAT_0022f4bc;
        uVar21 = local_1790;
        bVar5 = DAT_0022d890;
        if (((DAT_0022f4b8 != 0) && ((DAT_0022d88c & 1) == 0)) &&
           (((DAT_0022d890 & 1) == 0 && (DAT_0022dfbc == 0)))) {
          iVar15 = FUN_00150378();
          if ((iVar15 != 0) &&
             (iVar15 = nexus_ui_graphics_resources(DAT_0022f178,DAT_0022db60,DAT_0022f180,0),
             iVar15 == 1)) {
            DAT_0022f4b8 = 0;
            DAT_0022d88c = 1;
            DAT_0025cb30 = 0;
            uVar31 = DAT_0022f4bc + 1;
            bVar9 = DAT_0022f4bc < 0x80;
            DAT_0022d8a8 = uVar20;
            DAT_0022f4bc = uVar31;
            if (bVar9) {
              _Var17 = getpid();
              iVar15 = clock_gettime(0,&local_10e0);
              if (iVar15 == 0) {
                lVar22 = local_10e0.tv_sec * 1000 + (ulong)local_10e0.tv_nsec / 1000000;
              }
              else {
                lVar22 = 0;
              }
              __android_log_print(4,"NexusLab69252",
                                  "{\"stage\":\"%s\",\"reason\":\"%s\",\"action\":%u,\"pid\":%d,\"tid\":%d,\"time_ms\":%llu}"
                                  ,"performance_graphics","restoring_saved_mode",0x20002,_Var17,
                                  iVar14,lVar22);
            }
          }
          uVar31 = DAT_0022f4bc;
          uVar21 = local_1790;
          bVar5 = DAT_0022d890 & 1;
        }
        DAT_0022f4bc = uVar31;
        local_1790 = uVar21;
        if (bVar5 != 0) {
          if (uVar20 != DAT_00281a20) {
            if ((uVar20 < DAT_00281a20) || (60000 < uVar20 - DAT_00281a20)) {
              DAT_0022d890 = 0;
              DAT_0022dfb8 = 1;
              DAT_0022d880._5_3_ = (undefined3)(DAT_0022d880._4_4_ >> 8);
              DAT_0022d880._4_4_ = CONCAT31(DAT_0022d880._5_3_,1);
              DAT_0022f4bc = uVar31 + 1;
              DAT_0022d898 = 0;
              _DAT_0022d8a0 = 0;
              if (uVar31 < 0x80) {
                _Var17 = getpid();
                iVar15 = clock_gettime(0,&local_10e0);
                if (iVar15 == 0) {
                  lVar22 = local_10e0.tv_sec * 1000 + (ulong)local_10e0.tv_nsec / 1000000;
                }
                else {
                  lVar22 = 0;
                }
                __android_log_print(4,"NexusLab69252",
                                    "{\"stage\":\"%s\",\"reason\":\"%s\",\"action\":%u,\"pid\":%d,\"tid\":%d,\"time_ms\":%llu}"
                                    ,"performance_graphics","texture_reload_unverified",0x20002,
                                    _Var17,iVar14,lVar22);
              }
            }
            else {
              local_10e0.tv_nsec = 0;
              local_10d0 = FUN_0014e7c4;
              pcStack_10c8 = FUN_001556ec;
              local_10c0 = FUN_00155848;
              uVar31 = (uint)local_1788 >> 8;
              local_1788 = CONCAT31((int3)uVar31,1);
              local_1790._5_3_ = (undefined3)((ulong)uVar21 >> 0x28);
              local_1790._0_4_ = (int)uVar21;
              local_1790._0_5_ = CONCAT14(1,(int)local_1790);
              local_10e0.tv_sec = DAT_0022d8e0;
              iVar15 = FUN_00150378();
              if ((((iVar15 != 0) &&
                   (uVar21 = FUN_00150710(&local_10e0,&local_1770), (int)uVar21 != 0)) &&
                  (local_1770 == DAT_0022dfd8)) &&
                 (((((CONCAT22(uStack_1766,CONCAT11(uStack_1767,local_1768)) == DAT_0022dfe0 &&
                     (fStack_1764 == DAT_0022dfe4)) &&
                    ((local_1760 == DAT_0022dfe8 &&
                     ((uVar21 = FUN_0014e7c4(uVar21,(long)local_1770 + 0x101,&local_1788,1),
                      (int)uVar21 != 0 && ((char)local_1788 == '\0')))))) &&
                   (iVar15 = FUN_0014e7c4(uVar21,(long)local_1770 + 0x1cd,(long)&local_1790 + 4,1),
                   iVar15 != 0)) &&
                  (((bVar9 = local_1790._4_1_ == '\0', bVar9 &&
                    (iVar15 = FUN_00167524(&local_a0,&local_d8), iVar15 != 0)) &&
                   (local_a0 != DAT_00281a28)))))) {
                iVar15 = 0x800;
                if (fStack_1764 != 0.0 &&
                    CONCAT22(uStack_1766,CONCAT11(uStack_1767,local_1768)) != 0) {
                  iVar15 = 0x1000;
                }
                if (((float)local_d8 == (float)iVar15) && (local_d8._4_4_ == (float)iVar15)) {
                  uVar21 = FUN_001608e0(DAT_0022f178,DAT_0022db60,iVar14);
                  if ((int)uVar21 != 0) {
                    if ((int)uVar21 != -1) {
                      DAT_0022d890 = 0;
                      DAT_0022dfb8 = 0;
                      DAT_0022d894 = 0;
                      DAT_0025cb38 = DAT_00281a2c;
                      pcVar29 = "stock_atlas_restored";
                      if (DAT_00281a2c != 0) {
                        pcVar29 = "lowres_atlas_verified";
                      }
                      DAT_0022d898 = 0;
                      _DAT_0022d8a0 = 0;
                      FUN_00156760(uVar21,"performance_graphics",pcVar29,0x20002);
                      goto LAB_0015e888;
                    }
                    DAT_0022dfb8 = 1;
                    DAT_0022d880._4_4_ = CONCAT31(DAT_0022d880._5_3_,1);
                  }
                }
              }
            }
          }
          goto LAB_0015e2ec;
        }
LAB_0015e888:
        bVar9 = false;
        if ((DAT_0022d88c == 1) && (DAT_0022d8a8 <= uVar20 && uVar20 - DAT_0022d8a8 != 0)) {
          if (uVar20 - DAT_0022d8a8 >> 5 < 0x271) {
            local_10e0.tv_nsec = 0;
            local_10e0.tv_sec = 0;
            local_d8 = CONCAT44(local_d8._4_4_,0xffffffff);
            local_a0 = CONCAT31(local_a0._1_3_,1);
            if (((((DAT_0022f038 < 1) || (iVar14 != DAT_0022f038)) ||
                 (iVar15 = pthread_getname_np(__target_thread,(char *)&local_10e0,0x10), iVar15 != 0
                 )) || ((local_10e0.tv_sec != 0x706f6f6c6e69614d ||
                         (local_10e0.tv_nsec & 0xffU) != 0 || (iVar15 = FUN_0014fac4(), iVar15 == 0)
                        ))) ||
               ((uVar21 = nexus_menu_ui_state(&local_1770,iVar14), (int)uVar21 != 1 ||
                (local_1770._5_1_ != '\0')))) {
LAB_0015e920:
              iVar15 = 0;
              goto LAB_0015e928;
            }
            local_10e0.tv_sec = 0;
            uVar21 = FUN_0014e7c4(uVar21,DAT_0022d8e0 + 0x12eb9f0,&local_10e0,8);
            uVar34 = local_10e0.tv_sec;
            if (((local_10e0.tv_sec & 7U) != 0 || (ulong)local_10e0.tv_sec < 0x1000) ||
                (int)uVar21 == 0) {
              uVar34 = 0;
            }
            if (uVar34 != DAT_0022f178) goto LAB_0015e920;
            local_10e0.tv_sec = 0;
            iVar15 = FUN_0014e7c4(uVar21,uVar34 + 0x90,&local_10e0,8);
            uVar34 = local_10e0.tv_sec;
            if (((local_10e0.tv_sec & 7U) != 0 || (ulong)local_10e0.tv_sec < 0x1000) || iVar15 == 0)
            {
              uVar34 = 0;
            }
            if (((uVar34 != DAT_0022db60) || (uVar21 = FUN_00154d5c(DAT_0022f180), (int)uVar21 == 0)
                ) || (iVar15 = FUN_0014e7c4(uVar21,DAT_0022f178 + 0x19c,&local_a0,1), iVar15 == 0))
            goto LAB_0015e920;
            if ((char)local_a0 == '\0') {
              lVar22 = FUN_00154cf0(DAT_0022d8e0 + 0x1307e20);
              if ((((lVar22 == 0) ||
                   (iVar15 = FUN_0014e7c4(lVar22,lVar22 + 0x50,&local_d8,4), iVar15 == 0)) ||
                  ((int)(float)local_d8 < 0)) ||
                 ((0x40 < (int)(float)local_d8 || ((float)local_d8 == 7.00649e-45))))
              goto LAB_0015e920;
              local_10e0.tv_sec = DAT_0022d8e0;
              local_10d0 = FUN_0014e7c4;
              local_10e0.tv_nsec = 0;
              pcStack_10c8 = FUN_00167750;
              local_10c0 = FUN_00167888;
              pcStack_10b8 = FUN_0016793c;
              iVar15 = FUN_00167e78(&local_10e0);
              if (iVar15 != 2) goto LAB_0015e928;
            }
          }
          else {
            iVar15 = 0;
LAB_0015e928:
            if (((iVar15 != 0) || ((DAT_0022dfb8 & 1) != 0)) ||
               ((uVar20 < DAT_0022d8a8 || (0x270 < uVar20 - DAT_0022d8a8 >> 5)))) {
              pcVar29 = "game_reload_unverified";
              if (iVar15 != -1) {
                pcVar29 = "game_reload_unavailable";
              }
              DAT_0022d88c = 0;
              DAT_0022d894 = (uint)(iVar15 != 1);
              uVar31 = DAT_0022f4bc + 1;
              pcVar28 = "game_reload_requested";
              if (iVar15 != 1) {
                pcVar28 = pcVar29;
              }
              DAT_0022d898 = 0;
              _DAT_0022d8a0 = 0;
              bVar9 = DAT_0022f4bc < 0x80;
              DAT_0022f4bc = uVar31;
              if (bVar9) {
                _Var17 = getpid();
                iVar18 = clock_gettime(0,&local_10e0);
                if (iVar18 == 0) {
                  lVar22 = local_10e0.tv_sec * 1000 + (ulong)local_10e0.tv_nsec / 1000000;
                }
                else {
                  lVar22 = 0;
                }
                __android_log_print(4,"NexusLab69252",
                                    "{\"stage\":\"%s\",\"reason\":\"%s\",\"action\":%u,\"pid\":%d,\"tid\":%d,\"time_ms\":%llu}"
                                    ,"performance_reload",pcVar28,0x20002,_Var17,iVar14,lVar22);
              }
              bVar9 = iVar15 == 1 || iVar15 == -1;
              goto LAB_0015ea40;
            }
          }
          bVar9 = false;
        }
LAB_0015ea40:
        if ((bVar9) || (DAT_0022f03c < 1)) goto LAB_0015e2ec;
        FUN_0015f334(3,0,0,uVar20);
        local_10e0.tv_nsec = 0;
        local_10e0.tv_sec = 0;
        if ((((0 < DAT_0022f038) && (iVar14 == DAT_0022f038)) &&
            (iVar15 = pthread_getname_np(__target_thread,(char *)&local_10e0,0x10), iVar15 == 0)) &&
           (local_10e0.tv_sec == 0x706f6f6c6e69614d && (char)local_10e0.tv_nsec == '\0')) {
          iVar15 = pthread_once((pthread_once_t *)&DAT_0022f1d0,FUN_001552c0);
          if ((DAT_0022f1d8 == (code *)0x0) || (iVar15 = (*DAT_0022f1d8)(iVar15), iVar15 != 1)) {
            DAT_0022db68 = 0;
            DAT_0022db6c = 0;
          }
          pthread_mutex_lock((pthread_mutex_t *)((long)&DAT_0025c928 + 4));
          uStack_10a8 = uRam000000000025c990;
          local_10b0 = _DAT_0025c988;
          pcStack_10b8 = pcRam000000000025c980;
          local_10c0 = _DAT_0025c978;
          pcStack_10c8 = pcRam000000000025c970;
          local_10d0 = _DAT_0025c968;
          local_10e0.tv_nsec = uRam000000000025c960;
          local_10e0.tv_sec = _DAT_0025c958;
          uStack_1098 = uRam000000000025c9a0;
          local_10a0 = _DAT_0025c998;
          uStack_1088 = uRam000000000025c9b0;
          local_1090 = _DAT_0025c9a8;
          lStack_1078 = DAT_0025c9c0;
          local_1080 = _DAT_0025c9b8;
          uStack_1068 = DAT_0025c9d0;
          local_1070 = DAT_0025c9c8;
          uRam000000000025c960 = 0;
          _DAT_0025c958 = 0;
          pcRam000000000025c970 = (code *)0x0;
          _DAT_0025c968 = (code *)0x0;
          uRam000000000025c9a0 = 0;
          _DAT_0025c998 = 0;
          uRam000000000025c9b0 = 0;
          _DAT_0025c9a8 = 0;
          pcRam000000000025c980 = (code *)0x0;
          _DAT_0025c978 = (code *)0x0;
          uRam000000000025c990 = 0;
          _DAT_0025c988 = 0;
          DAT_0025c9c0 = 0;
          _DAT_0025c9b8 = 0;
          DAT_0025c9d0 = 0;
          DAT_0025c9c8 = 0;
          pthread_mutex_unlock((pthread_mutex_t *)((long)&DAT_0025c928 + 4));
          iVar15 = (uint)local_10e0.tv_sec;
          if ((uint)local_10e0.tv_sec != 0) {
            FUN_0014ef90(&local_1770,uVar20);
            iVar18 = FUN_00159ac0();
            if (((iVar18 == 0) || (local_1750 == 0)) ||
               ((local_1744 != 0 ||
                ((((CONCAT71(uStack_175f,local_1760) != lStack_1078 ||
                   (CONCAT44(fStack_1764,CONCAT22(uStack_1766,CONCAT11(uStack_1767,local_1768))) !=
                    local_1070)) || (uVar20 < uStack_1068)) || (5000 < uVar20 - uStack_1068)))))) {
              puVar25 = &DAT_00135c8a;
LAB_0015ec24:
              FUN_0015b620(puVar25);
            }
            else {
              uVar34 = (ulong)&local_10e0 | 4;
              if (iVar15 == 0x7f) {
                uVar21 = __strlen_chk(uVar34,0x7c);
                iVar15 = FUN_00159e08(uVar34,uVar21,&local_d8);
                if (iVar15 != 0) {
                  puVar25 = &UNK_001349cd;
                  if ((float)local_d8 != 0.0) {
                    puVar25 = &DAT_0013b049;
                  }
                  DAT_0022db6c = (int)(float)local_d8;
                  goto LAB_0015ec24;
                }
              }
              else {
                uVar21 = __strlen_chk(uVar34,0x7c);
                iVar15 = FUN_00159f20(uVar34,uVar21,&local_a0);
                if (iVar15 != 0) {
                  local_d8 = DAT_0022d8e0;
                  local_d0 = FUN_0014e7c4;
                  local_a8 = DAT_0022d8e0 + 0x7acd48;
                  local_c8 = DAT_0022d8e0 + _DAT_0010fad0;
                  lStack_c0 = DAT_0022d8e0 + _UNK_0010fad8;
                  lStack_b8 = DAT_0022d8e0 + _DAT_0010f940;
                  lStack_b0 = DAT_0022d8e0 + _UNK_0010f948;
                  iVar15 = FUN_00168764(&local_d8,local_10e0.tv_sec & 0xffffffff,&local_a0,
                                        CONCAT44(DAT_0022db6c,DAT_0022db68));
                  puVar25 = &UNK_00135ce7;
                  if (iVar15 != 0) {
                    puVar25 = &DAT_0013a43d;
                  }
                  goto LAB_0015ec24;
                }
              }
            }
          }
        }
        if (DAT_0025c920 != 0) {
          local_10e0.tv_nsec = 0;
          local_10e0.tv_sec = 0;
          if (((0 < DAT_0022f038) && (iVar14 == DAT_0022f038)) &&
             (((((iVar15 = pthread_getname_np(__target_thread,(char *)&local_10e0,0x10), iVar15 == 0
                 && ((local_10e0.tv_sec == 0x706f6f6c6e69614d && (char)local_10e0.tv_nsec == '\0' &&
                     (iVar15 = pthread_once((pthread_once_t *)&DAT_0022f1d0,FUN_001552c0),
                     DAT_0022f1d8 != (code *)0x0)))) &&
                (iVar15 = (*DAT_0022f1d8)(iVar15), iVar15 == 1)) &&
               (((uVar20 <= DAT_00281a30 - 1 || (999 < uVar20 - DAT_00281a30)) &&
                (DAT_00281a30 = uVar20, FUN_0014ef90(&local_10e0,uVar20), uVar20 != 0)))) &&
              (((((int)local_10c0 != 0 && (local_10c0._4_4_ != 0)) &&
                ((uVar20 <= DAT_00281a38 - 1 || (0x270 < uVar20 - DAT_00281a38 >> 3)))) &&
               ((local_1770 = (long *)0x0, DAT_0022f4c8 != (long *)0x0 && (DAT_0022f4e8 != 0))))))))
          {
            iVar15 = (**(code **)(*DAT_0022f4c8 + 0x30))(DAT_0022f4c8,&local_1770,0x10006);
            if (iVar15 == 0) {
              bVar9 = true;
            }
            else {
              if ((iVar15 != -2) ||
                 (iVar15 = (**(code **)(*DAT_0022f4c8 + 0x20))(DAT_0022f4c8,&local_1770,0),
                 iVar15 != 0)) goto LAB_0015ec7c;
              bVar9 = false;
            }
            plVar6 = local_1770;
            if (local_1770 != (long *)0x0) {
              iVar15 = (**(code **)(*local_1770 + 0x98))(local_1770,0xc);
              if (-1 < iVar15) {
                lVar22 = (**(code **)(*plVar6 + 0x488))(plVar6,DAT_0025c920,DAT_0025c8e0);
                cVar11 = (**(code **)(*plVar6 + 0x720))(plVar6);
                if ((cVar11 == '\0') && (lVar22 != 0)) {
                  lVar23 = (**(code **)(*plVar6 + 0x110))(plVar6,lVar22,DAT_0025c8e8);
                  cVar11 = (**(code **)(*plVar6 + 0x720))(plVar6);
                  if ((cVar11 == '\0') && (lVar23 != 0)) {
                    lVar23 = (**(code **)(*plVar6 + 0x110))(plVar6,lVar23,DAT_0025c8f0,0x102001b);
                    cVar11 = (**(code **)(*plVar6 + 0x720))(plVar6);
                    if ((cVar11 == '\0') && (lVar23 != 0)) {
                      cVar11 = (**(code **)(*plVar6 + 0x128))(plVar6,lVar23,DAT_0025c910);
                      cVar12 = (**(code **)(*plVar6 + 0x128))(plVar6,lVar23,DAT_0025c918);
                      cVar13 = (**(code **)(*plVar6 + 0x720))(plVar6);
                      if ((cVar13 == '\0') && ((cVar11 != '\0' && (cVar12 != '\0')))) {
                        lVar26 = (**(code **)(*plVar6 + 0x110))(plVar6,lVar23,DAT_0025c8f8);
                        cVar11 = (**(code **)(*plVar6 + 0x720))(plVar6);
                        if ((cVar11 == '\0') && (lVar26 != 0)) {
                          lVar26 = (**(code **)(*plVar6 + 0x110))(plVar6,lVar26,DAT_0025c900);
                          cVar11 = (**(code **)(*plVar6 + 0x720))(plVar6);
                          if (((cVar11 == '\0') && (lVar26 != 0)) &&
                             (lVar27 = (**(code **)(*plVar6 + 0x548))(plVar6,lVar26,0), lVar27 != 0)
                             ) {
                            iVar18 = FUN_001689f8();
                            (**(code **)(*plVar6 + 0x550))(plVar6,lVar26,lVar27);
                            if (iVar18 != 0) {
                              lVar26 = (**(code **)(*plVar6 + 0x488))
                                                 (plVar6,DAT_0025c920,DAT_0025c8e0);
                              FUN_0014cfcc(1);
                              FUN_0014ef90(&local_1770);
                              cVar11 = (**(code **)(*plVar6 + 0x720))(plVar6);
                              if ((((cVar11 == '\0') && (lVar26 != 0)) &&
                                  ((cVar11 = (**(code **)(*plVar6 + 0xc0))(plVar6,lVar26,lVar22),
                                   cVar11 != '\0' && ((local_1750 != 0 && (local_174c != 0)))))) &&
                                 (((code *)CONCAT71(uStack_175f,local_1760) == local_10d0 &&
                                  (CONCAT44(fStack_1764,
                                            CONCAT22(uStack_1766,CONCAT11(uStack_1767,local_1768)))
                                   == local_10e0.tv_nsec)))) {
                                DAT_00281a38 = uVar20;
                                (**(code **)(*plVar6 + 0x128))(plVar6,lVar23,DAT_0025c908);
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
              cVar11 = (**(code **)(*plVar6 + 0x720))(plVar6);
              if (cVar11 != '\0') {
                (**(code **)(*plVar6 + 0x88))(plVar6);
              }
              if (-1 < iVar15) {
                (**(code **)(*plVar6 + 0xa0))(plVar6,0);
              }
              if (!bVar9) {
                (**(code **)(*DAT_0022f4c8 + 0x28))();
              }
            }
          }
        }
LAB_0015ec7c:
        iVar15 = nexus_menu_ui_state(&local_10e0,iVar14);
        DAT_0022db70 = 0;
        if (((iVar15 == 1) && (local_10e0.tv_sec._4_1_ == '\0')) &&
           (DAT_0022db70 = uVar20, (int)DAT_0025c9d8 != 0)) {
          DAT_0022db70 = 0;
        }
      }
      nexus_menu_theme_pump(iVar14,uVar20);
      nexus_menu_debug_pump(iVar14,uVar20);
      nexus_menu_profile_pump(iVar14,uVar20);
      nexus_menu_editor_pump(iVar14,uVar20);
    }
    else {
      FUN_0015f334(param_3 & 0xffffffff,param_1,param_2,uVar20);
    }
  }
  DAT_0025ca14 = 0;
LAB_0015e35c:
  *puVar19 = uVar2;
  if (*(long *)(lVar3 + 0x28) == local_78) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_001607dc @ 001607dc ===== */

void FUN_001607dc(undefined8 param_1)

{
  bool bVar1;
  long lVar2;
  __pid_t _Var3;
  undefined4 uVar4;
  int iVar5;
  long lVar6;
  timespec local_48;
  long local_38;
  
  lVar2 = tpidr_el0;
  local_38 = *(long *)(lVar2 + 0x28);
  DAT_0022f03c = 0xffffffff;
  FUN_00183cdc(0xffffffff,DAT_0022f038,param_1);
  iVar5 = DAT_0022f4bc + 1;
  bVar1 = DAT_0022f4bc < 0x80;
  DAT_0022f4bc = iVar5;
  if (bVar1) {
    _Var3 = getpid();
    uVar4 = gettid(_Var3);
    iVar5 = clock_gettime(0,&local_48);
    if (iVar5 == 0) {
      lVar6 = local_48.tv_sec * 1000 + (ulong)local_48.tv_nsec / 1000000;
    }
    else {
      lVar6 = 0;
    }
    __android_log_print(4,"NexusLab69252",
                        "{\"stage\":\"%s\",\"reason\":\"%s\",\"action\":%u,\"pid\":%d,\"tid\":%d,\"time_ms\":%llu}"
                        ,"nexus_shell_stopped",param_1,0,_Var3,uVar4,lVar6);
  }
  if (*(long *)(lVar2 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00160c3c @ 00160c3c ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00160c3c(int param_1,long param_2)

{
  long lVar1;
  undefined4 uVar2;
  int iVar3;
  undefined8 uVar4;
  long lVar5;
  long lVar6;
  long lVar7;
  short sVar8;
  uint uVar9;
  undefined *puVar10;
  char *pcVar11;
  ushort uVar12;
  undefined8 uVar13;
  undefined1 auVar14 [16];
  undefined2 uVar15;
  undefined1 auVar16 [16];
  undefined1 local_17c [4];
  undefined1 local_178 [4];
  short local_174 [2];
  undefined1 auStack_170 [192];
  long local_b0;
  float local_a8;
  undefined8 local_a0;
  undefined8 uStack_98;
  undefined8 local_90;
  undefined8 local_88;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined8 local_70;
  undefined8 local_68;
  long local_58;
  
  lVar1 = tpidr_el0;
  local_58 = *(long *)(lVar1 + 0x28);
  uStack_98 = 0;
  local_a0 = 0;
  local_88 = 0;
  local_90 = 0;
  uStack_78 = 0;
  local_80 = 0;
  local_68 = 0;
  local_70 = 0;
  uVar2 = nexus_menu_setting_value(0x6e,(ulong)&local_a0 | 0xc);
  local_a0 = CONCAT44(local_a0._4_4_,uVar2);
  iVar3 = nexus_menu_setting_value(0x70,&local_90);
  local_a0._4_4_ = iVar3;
  uVar4 = nexus_menu_setting_value(0x71,(long)&local_90 + 4);
  iVar3 = (int)uVar4;
  uStack_98._0_4_ = iVar3;
  if ((((int)local_a0 == 3) || (local_a0._4_4_ == 3)) || (iVar3 == 3)) {
    pcVar11 = "settings_busy";
    goto LAB_00160d10;
  }
  if ((((int)local_a0 == 1) && (local_a0._4_4_ == 1)) && (iVar3 == 1)) {
    if (uStack_98._4_4_ < 6) {
      puVar10 = (&PTR_s_map_editor_big_exit_button_0019ae20)[uStack_98._4_4_];
    }
    else {
      puVar10 = (undefined *)0x0;
    }
    auVar14._8_8_ = local_90;
    auVar14._0_8_ = local_90;
    auVar16 = NEON_cmgt(auVar14,_DAT_0010f9a0,4);
    auVar14 = NEON_cmgt(_DAT_0010f9a0,auVar14,4);
    uVar12 = NEON_umaxv(CONCAT26(auVar14._12_2_,
                                 CONCAT24(auVar14._8_2_,CONCAT22(auVar16._4_2_,auVar16._0_2_))),2);
    if (((uVar12 & 1) == 0) && (puVar10 != (undefined *)0x0)) {
      uVar4 = FUN_0014e7c4(uVar4,DAT_0022db60 + 0x10,&local_80,0x10);
      if ((int)uVar4 != 0) {
        local_68 = local_68 | 0x100000000;
      }
      uVar4 = FUN_0014e7c4(uVar4,DAT_0022f178 + 0x178,&local_70,4);
      if ((int)uVar4 != 0) {
        local_68 = local_68 | 0x200000000;
      }
      uVar4 = FUN_0014e7c4(uVar4,DAT_0022f178 + 0x4c,(long)&local_70 + 4,4);
      if ((int)uVar4 != 0) {
        local_68 = local_68 | 0x400000000;
      }
      uVar4 = FUN_0014e7c4(uVar4,DAT_0022f178 + 0x54,&local_68,4);
      if ((int)uVar4 != 0) {
        local_68 = local_68 | 0x800000000;
      }
      iVar3 = FUN_0014e7c4(uVar4,DAT_0022f178 + 0x284,&local_88,8);
      uVar9 = local_68._4_4_;
      if (iVar3 != 0) {
        uVar9 = local_68._4_4_ | 0x10;
        local_68 = local_68 | 0x1000000000;
      }
      if (uVar9 == 0x1f) {
        auVar16._8_8_ = uStack_78;
        auVar16._0_8_ = local_80;
        auVar14 = NEON_fcmeq(auVar16,_DAT_0010fbb0,4);
        uVar12 = NEON_umaxv(CONCAT26(CONCAT11(~auVar14[0xd],~auVar14[0xc]),
                                     CONCAT24(CONCAT11(~auVar14[9],~auVar14[8]),
                                              CONCAT22(CONCAT11(~auVar14[5],~auVar14[4]),
                                                       CONCAT11(~auVar14[1],~auVar14[0])))),2);
        if ((uVar12 & 1) == 0) {
          uVar4 = nexus_rich_launcher_geometry
                            (*(undefined4 *)(param_2 + 8),(short)*(undefined4 *)(param_2 + 0xc),
                             (float)local_70,local_70._4_4_,(undefined4)local_68,&local_b0);
          if ((int)uVar4 == 1) {
            if (((float)(int)(*(float *)(param_2 + 8) / (float)local_70) == (float)(int)local_88) &&
               ((float)(int)(*(float *)(param_2 + 0xc) / (float)local_70) == (float)local_88._4_4_))
            {
              uVar13 = NEON_scvtf(local_90,4);
              local_b0 = CONCAT44((float)((ulong)local_b0 >> 0x20) +
                                  (float)((ulong)uVar13 >> 0x20) * local_a8,
                                  (float)local_b0 + (float)uVar13 * local_a8);
              if (uStack_98._4_4_ != DAT_001a7c60) {
                if (uStack_98._4_4_ < 6) {
                  puVar10 = (&PTR_s_map_editor_big_exit_button_0019ae20)[uStack_98._4_4_];
                }
                else {
                  puVar10 = (undefined *)0x0;
                }
                uVar4 = FUN_00163eb8(uVar4,puVar10);
                if ((int)uVar4 == 0) {
                  pcVar11 = "launcher_asset_cache_pending";
                  goto LAB_00160d10;
                }
              }
              if (DAT_0022f180 != 0) {
                if (uStack_98._4_4_ != DAT_001a7c60) {
                  if (uStack_98._4_4_ < 6) {
                    puVar10 = (&PTR_s_map_editor_big_exit_button_0019ae20)[uStack_98._4_4_];
                  }
                  else {
                    puVar10 = (undefined *)0x0;
                  }
                  lVar5 = (*(code *)(DAT_0022d8e0 + 0x51ecec))("sc/ui.sc",puVar10);
                  local_174[0] = 0;
                  if (((lVar5 == 0) || (lVar6 = FUN_00154cf0(), lVar6 != DAT_0022d8e0 + 0x11ad208))
                     || (iVar3 = FUN_0014e7c4(lVar6,lVar5 + 0xbe,local_174,2), iVar3 == 0)) {
LAB_0016124c:
                    pcVar11 = "launcher_movie_contract";
                  }
                  else {
                    sVar8 = 1;
                    if (uStack_98._4_4_ == 5) {
                      sVar8 = 2;
                    }
                    if (sVar8 != local_174[0]) goto LAB_0016124c;
                    (*(code *)(DAT_0022d8e0 + 0x772a30))(DAT_0022f180,lVar5,1);
                    (*(code *)(DAT_0022d8e0 + 0x5d7c30))(lVar5,uStack_98._4_4_ == 5);
                    lVar6 = FUN_00154cf0(DAT_0022f180 + 0x88);
                    lVar7 = FUN_00154cf0(DAT_0022f180 + 0x80);
                    if (lVar7 == lVar5) {
                      lVar7 = FUN_00154cf0(DAT_0022f180 + 0xb0);
                      pcVar11 = "launcher_binding_contract";
                      if ((lVar7 == lVar5) && (lVar6 != 0)) {
                        lVar7 = FUN_00154cf0(lVar6);
                        if ((lVar7 == DAT_0022d8e0 + 0x11ad208) &&
                           (iVar3 = FUN_00154d5c(lVar6,DAT_0022f180), iVar3 != 0)) {
                          uVar4 = (*(code *)(DAT_0022d8e0 + 0x88836c))(DAT_0022f180,&DAT_0025ca50,1)
                          ;
                          local_178[0] = 0;
                          local_17c[0] = 0;
                          DAT_001a7c60 = uStack_98._4_4_;
                          uVar4 = FUN_0014e7c4(uVar4,DAT_0022f180 + 0xc,local_178,1);
                          if (((int)uVar4 == 0) ||
                             (iVar3 = FUN_0014e7c4(uVar4,lVar5 + 0xc,local_17c,1), iVar3 == 0)) {
                            pcVar11 = "launcher_alpha_read";
                            goto LAB_00160e70;
                          }
                          if (uStack_98._4_4_ < 6) {
                            puVar10 = (&PTR_s_map_editor_big_exit_button_0019ae20)[uStack_98._4_4_];
                          }
                          else {
                            puVar10 = (undefined *)0x0;
                          }
                          uVar4 = FUN_0015540c(auStack_170,0xc0,0xc0,"asset=%s frame=%d alpha=%u/%u"
                                               ,puVar10,uStack_98._4_4_ == 5,local_178[0],
                                               local_17c[0]);
                          uVar4 = FUN_00156760(uVar4,"nexus_shell_launcher_style",auStack_170,0);
                          goto LAB_00160f80;
                        }
                        goto LAB_00161264;
                      }
                    }
                    else {
LAB_00161264:
                      pcVar11 = "launcher_binding_contract";
                    }
                  }
                  uVar4 = 0xffffffff;
                  PTR_s_launcher_initializing_001a7c58 = pcVar11;
                  goto LAB_00160e78;
                }
LAB_00160f80:
                if ((local_b0 != DAT_0025ca60 || local_a8 != DAT_0025ca68) ||
                   (DAT_001a7c64 != param_1)) {
                  FUN_0016cf00(local_a8,SUB42(local_a8,0),DAT_0022f180);
                  uVar2 = (undefined4)local_b0;
                  if (param_1 != 0) {
                    uVar2 = 0xc61c3c00;
                  }
                  uVar15 = (undefined2)((ulong)local_b0 >> 0x20);
                  if (param_1 != 0) {
                    uVar15 = 0x3c00;
                  }
                  uVar4 = (*(code *)(DAT_0022d8e0 + 0x595314))(uVar2,uVar15,DAT_0022f180);
                  DAT_0025ca60 = local_b0;
                  DAT_0025ca68 = local_a8;
                  DAT_001a7c64 = param_1;
                }
                PTR_s_launcher_initializing_001a7c58 = s_launcher_ready_00136742;
                if (DAT_0025ca6c == '\x01') {
                  if (DAT_0025ca70 < 8) {
                    FUN_00156760(uVar4,"nexus_shell_launcher_recovered",
                                 "root_projection_settings_ready",0);
                    DAT_0025ca70 = DAT_0025ca70 + 1;
                  }
                  uVar4 = 1;
                  DAT_0025ca6c = '\0';
                  DAT_0025ca78 = 0;
                  goto LAB_00160e78;
                }
              }
              uVar4 = 1;
              goto LAB_00160e78;
            }
          }
          pcVar11 = "projection_not_coherent";
        }
        else {
          pcVar11 = "root_transform_not_identity";
        }
      }
      else {
        pcVar11 = "projection_read_unavailable";
      }
LAB_00160d10:
      FUN_001641ec(pcVar11,&local_a0,param_2);
      uVar4 = 0;
      goto LAB_00160e78;
    }
    pcVar11 = "launcher_settings_value_contract";
  }
  else {
    pcVar11 = "launcher_settings_query_contract";
  }
LAB_00160e70:
  uVar4 = 0xffffffff;
  PTR_s_launcher_initializing_001a7c58 = pcVar11;
LAB_00160e78:
  if (*(long *)(lVar1 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar4);
}

/* ===== FUN_001613ac @ 001613ac ===== */

int FUN_001613ac(undefined4 param_1)

{
  long lVar1;
  bool bVar2;
  long *plVar3;
  bool bVar4;
  char cVar5;
  int iVar6;
  int iVar7;
  long *local_50;
  long local_48;
  
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  iVar6 = nexus_menu_dispatch(1,0,0,param_1);
  if (1 < iVar6 - 1U) goto LAB_00161508;
  local_50 = (long *)0x0;
  if (((DAT_0022d8b0 == '\x01') && (DAT_0022d8d0 != (long *)0x0)) && (DAT_0022f030 != 0)) {
    iVar7 = (**(code **)(*DAT_0022d8d0 + 0x30))(DAT_0022d8d0,&local_50,0x10006);
    if (iVar7 == 0) {
      bVar2 = true;
      plVar3 = local_50;
    }
    else {
      if ((iVar7 != -2) ||
         (iVar7 = (**(code **)(*DAT_0022d8d0 + 0x20))(DAT_0022d8d0,&local_50,0), iVar7 != 0))
      goto LAB_00161504;
      bVar2 = false;
      plVar3 = local_50;
    }
    local_50 = plVar3;
    if (plVar3 != (long *)0x0) {
      cVar5 = (**(code **)(*plVar3 + 0x3a8))(plVar3,DAT_0022f030,DAT_0022f4f0);
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
      if (bVar4) goto LAB_00161508;
    }
  }
LAB_00161504:
  iVar6 = 0;
LAB_00161508:
  if (*(long *)(lVar1 + 0x28) == local_48) {
    return iVar6;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00161538 @ 00161538 ===== */

ulong FUN_00161538(uint param_1,uint param_2,undefined4 param_3)

{
  bool bVar1;
  ulong uVar2;
  uint uVar3;
  long lVar4;
  int iVar5;
  
  if (param_1 == 0) {
LAB_001615f0:
    uVar2 = nexus_menu_set_value(param_1,param_2,param_3);
    return uVar2;
  }
  uVar2 = FUN_00193f80(1,&DAT_0022f500);
  if ((uVar2 & 1) == 0) {
    if (DAT_00233ecc != 0) {
      lVar4 = 0;
      do {
        if (*(uint *)((long)&DAT_0022f6cc + lVar4) == param_1) {
          DAT_0022f500 = 0;
          if ((*(int *)((long)&DAT_0022f6d0 + lVar4) != 5) &&
             (*(int *)((long)&DAT_0022f6d0 + lVar4) != 2)) goto LAB_001615a8;
          if (param_2 < 2) {
            iVar5 = *(int *)((long)&DAT_0022f6e0 + lVar4);
            if ((*(int *)((long)&DAT_0022f6dc + lVar4) == 0) &&
               (iVar5 = *(int *)((long)&DAT_0022f6e4 + lVar4),
               *(int *)((long)&DAT_0022f6d4 + lVar4) != 0)) {
              bVar1 = true;
            }
            else {
              bVar1 = iVar5 != 0;
            }
            if (*(int *)((long)&DAT_0022f6d8 + lVar4) != 0) {
              uVar3 = 0x80000000;
              if ((bool)(param_2 == 0 ^ bVar1)) {
                uVar3 = 1;
              }
              DAT_0022f500 = 0;
              return (ulong)uVar3;
            }
            DAT_0022f500 = 0;
            return 0;
          }
          goto LAB_00161640;
        }
        lVar4 = lVar4 + 0x24;
      } while ((ulong)DAT_00233ecc * 0x24 - lVar4 != 0);
    }
    DAT_0022f500 = 0;
  }
LAB_001615a8:
  if (param_1 == 0x2e001) {
    uVar2 = 4;
    if ((0xffffff6e < param_2 - 0x92) && (DAT_0025ca80 == 1)) {
      uVar2 = 1;
      DAT_0025ca84 = param_2;
    }
  }
  else {
    if (param_1 < 0xa8) goto LAB_001615f0;
LAB_00161640:
    uVar2 = 4;
  }
  return uVar2;
}

/* ===== FUN_00161acc @ 00161acc ===== */

void FUN_00161acc(undefined8 param_1)

{
  long lVar1;
  int iVar2;
  undefined8 uVar3;
  uint uVar4;
  undefined1 auStack_30 [4];
  char local_2c;
  char local_2b;
  long local_28;
  
  lVar1 = tpidr_el0;
  local_28 = *(long *)(lVar1 + 0x28);
  iVar2 = nexus_menu_ui_state(auStack_30);
  uVar3 = 0;
  if (((iVar2 == 1) && (local_2c != '\0')) && (local_2b == '\0')) {
    uVar4 = (uint)DAT_0022d880;
    if ((DAT_0022dfbc == 0 && DAT_0022d894 == 0) && (DAT_0022dfb8 & 1) == 0) {
      uVar4 = (uint)(uVar4 == 0);
    }
    uVar3 = FUN_001645e0(uVar4,param_1);
  }
  if (*(long *)(lVar1 + 0x28) == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar3);
}

/* ===== FUN_00161b84 @ 00161b84 ===== */

void FUN_00161b84(undefined8 param_1)

{
  long lVar1;
  int iVar2;
  undefined8 uVar3;
  uint uVar4;
  undefined1 auStack_30 [4];
  char local_2c;
  char local_2b;
  char local_2a;
  long local_28;
  
  lVar1 = tpidr_el0;
  local_28 = *(long *)(lVar1 + 0x28);
  iVar2 = nexus_menu_ui_state(auStack_30);
  uVar3 = 0;
  if (((iVar2 == 1) && (local_2c != '\0')) && (local_2b == '\0')) {
    if ((local_2a == 'R') || (local_2a == 'O')) {
      uVar4 = (uint)DAT_0022d880;
      if ((DAT_0022dfbc == 0 && DAT_0022d894 == 0) && (DAT_0022dfb8 & 1) == 0) {
        uVar4 = (uint)(uVar4 == 0);
      }
      uVar3 = FUN_001645e0(uVar4,param_1);
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

/* ===== FUN_00161c58 @ 00161c58 ===== */

void FUN_00161c58(undefined4 param_1)

{
  long lVar1;
  bool bVar2;
  undefined8 uVar3;
  long *plVar4;
  bool bVar5;
  char cVar6;
  int iVar7;
  ulong uVar8;
  int iVar9;
  uint uVar10;
  long lVar11;
  char cVar12;
  undefined8 local_a0;
  undefined8 local_98;
  undefined8 uStack_90;
  long local_88;
  undefined8 uStack_80;
  undefined8 local_78;
  undefined8 local_70;
  long local_68;
  
  uVar3 = DAT_0010f748;
  lVar1 = tpidr_el0;
  local_68 = *(long *)(lVar1 + 0x28);
  local_78 = 0;
  local_98 = 0;
  uStack_80 = 0;
  local_88 = 0;
  local_a0 = DAT_0010f748;
  uStack_90 = 0x500000000;
  iVar7 = FUN_00191ea0(&local_a0);
  if (iVar7 == 0) {
    uStack_80 = 0;
    local_78 = 0;
    local_88 = 0;
    local_a0 = uVar3;
    local_98 = 0;
    uStack_90 = 0x500000000;
  }
  uVar8 = FUN_00193f80(1,&DAT_001a7c08);
  if ((uVar8 & 1) != 0) goto LAB_00161e94;
  lVar11 = 0;
  iVar7 = 0;
  iVar9 = 5;
  bVar2 = true;
  if ((int)local_a0 == 1) {
    uVar10 = 0;
    if (local_a0._4_4_ == 0x30) {
      lVar11 = local_88;
      iVar9 = uStack_90._4_4_;
      if ((int)uStack_90 == 0) {
        uVar10 = 0;
        iVar7 = 0;
      }
      else {
        bVar2 = false;
        iVar7 = 1;
        uVar10 = (uint)(local_98._4_4_ != 0);
        if ((local_98._4_4_ != 0) && (local_88 == 0)) {
          iVar7 = 0;
          uVar10 = 0;
          bVar2 = true;
          iVar9 = 4;
        }
      }
    }
  }
  else {
    uVar10 = 0;
  }
  if ((((DAT_001a7c10 == uVar10) && (DAT_001a7c14 == iVar7)) && (DAT_001a7c20 == lVar11)) &&
     (DAT_001a7c18 == iVar9)) {
    if (bVar2) goto LAB_00161e1c;
LAB_00161dc8:
    if (uVar10 == 0) {
      cVar12 = '\0';
      if ((DAT_001a7c2c == '\0' || DAT_001a7c2b == '\0') && (DAT_001a7c2a == '\0'))
      goto LAB_00161e1c;
    }
    else {
      cVar12 = '\x01';
      if (((DAT_001a7c2b != '\0') && ((DAT_001a7c2c != '\0' && (DAT_001a7c30 == lVar11)))) &&
         ((bool)DAT_001a7c2a == (DAT_001a7c28 != '\0'))) goto LAB_00161e1c;
    }
    bVar2 = false;
  }
  else {
    bVar5 = DAT_001a7c0c == -1;
    DAT_001a7c0c = DAT_001a7c0c + 1;
    if (bVar5) {
      DAT_001a7c0c = 1;
    }
    DAT_001a7c10 = uVar10;
    DAT_001a7c14 = iVar7;
    DAT_001a7c18 = iVar9;
    DAT_001a7c20 = lVar11;
    if (!bVar2) goto LAB_00161dc8;
LAB_00161e1c:
    cVar12 = '\0';
    lVar11 = 0;
    bVar2 = true;
  }
  DAT_001a7c08 = 0;
  if (((int)uStack_90 != 0) && (iVar7 = nexus_menu_ui_state(&local_70,param_1), iVar7 == 1)) {
    if ((bool)local_70._5_1_ != (local_98._4_4_ != 0)) {
      nexus_menu_set_battle(local_98._4_4_ != 0,param_1);
    }
  }
  if ((((!bVar2) && (DAT_0022d8b8 != 0)) && (DAT_0022f4f8 != 0)) &&
     ((((int)DAT_0022d8c8 != 0 && (local_70 = (long *)0x0, DAT_0022d8b0 == '\x01')) &&
      ((DAT_0022d8d0 != (long *)0x0 && (DAT_0022f030 != 0)))))) {
    iVar7 = (**(code **)(*DAT_0022d8d0 + 0x30))(DAT_0022d8d0,&local_70,0x10006);
    if (iVar7 == 0) {
      bVar2 = true;
    }
    else {
      if ((iVar7 != -2) ||
         (iVar7 = (**(code **)(*DAT_0022d8d0 + 0x20))(DAT_0022d8d0,&local_70,0), iVar7 != 0))
      goto LAB_00161e94;
      bVar2 = false;
    }
    plVar4 = local_70;
    if (local_70 != (long *)0x0) {
      cVar6 = (**(code **)(*local_70 + 0x3a8))(local_70,DAT_0022d8b8,DAT_0022f4f8,cVar12,lVar11);
      if (cVar6 == '\x01') {
        cVar6 = (**(code **)(*plVar4 + 0x720))(plVar4);
        bVar5 = cVar6 == '\0';
      }
      else {
        bVar5 = false;
      }
      cVar6 = (**(code **)(*plVar4 + 0x720))(plVar4);
      if (cVar6 != '\0') {
        (**(code **)(*plVar4 + 0x88))(plVar4);
      }
      if (!bVar2) {
        (**(code **)(*DAT_0022d8d0 + 0x28))();
      }
      if ((((bVar5) && (uVar8 = FUN_00193f80(1,&DAT_001a7c08), (uVar8 & 1) == 0)) &&
          (DAT_001a7c08 = 0, (bool)DAT_001a7c2a == (cVar12 != '\0' && DAT_001a7c28 != '\0'))) &&
         (uVar8 = FUN_00193f80(1,&DAT_001a7c08), (uVar8 & 1) == 0)) {
        DAT_001a7c2b = '\x01';
        DAT_001a7c08 = 0;
        DAT_001a7c2c = cVar12;
        DAT_001a7c30 = lVar11;
      }
    }
  }
LAB_00161e94:
  if (*(long *)(lVar1 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0016291c @ 0016291c ===== */

/* WARNING: Type propagation algorithm not settling */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0016291c(undefined1 *param_1,undefined4 param_2)

{
  long lVar1;
  void *__s;
  uint uVar2;
  int iVar3;
  char *pcVar4;
  byte bVar5;
  byte bVar6;
  byte bVar7;
  byte bVar8;
  long lVar9;
  undefined1 auVar10 [16];
  undefined8 uVar11;
  undefined8 uVar12;
  undefined8 uVar13;
  undefined8 uVar14;
  undefined *puVar15;
  char cVar16;
  char cVar17;
  undefined2 uVar18;
  bool bVar19;
  bool bVar20;
  bool bVar21;
  bool bVar22;
  bool bVar23;
  int iVar24;
  uint uVar25;
  uint uVar26;
  long lVar27;
  int *piVar28;
  undefined8 *puVar29;
  long lVar30;
  uint *puVar31;
  int iVar32;
  ulong uVar33;
  uint uVar34;
  int iVar35;
  uint uVar36;
  ulong uVar37;
  ulong uVar38;
  undefined1 *puVar39;
  byte *pbVar40;
  ulong uVar41;
  ushort uVar42;
  undefined1 auVar43 [16];
  undefined1 auVar44 [16];
  undefined1 auStack_8e70 [4];
  undefined4 local_8e6c;
  undefined1 local_8e68;
  undefined1 local_8e67;
  undefined4 local_8e60;
  undefined *local_8e58;
  char *local_8e50;
  undefined8 local_8e48;
  undefined4 local_8e40;
  undefined8 local_8e3c;
  undefined8 uStack_8e34;
  undefined1 local_8e2c;
  undefined2 local_8e2b;
  undefined1 local_8e29;
  undefined2 local_8e28;
  int local_8e20 [2];
  char *local_8e18 [3];
  uint local_8e00 [4];
  undefined8 local_8df0;
  undefined2 local_8de8;
  undefined4 local_8de0;
  undefined *local_8dd8;
  char *pcStack_8dd0;
  undefined8 local_8dc8;
  undefined4 local_8dc0;
  undefined8 local_8dbc;
  undefined8 local_8db4;
  undefined1 local_8dac;
  undefined4 local_8dab;
  undefined1 local_8da7;
  undefined4 local_8da0;
  char *local_8d98;
  char *pcStack_8d90;
  undefined8 local_8d88;
  undefined4 local_8d80;
  undefined8 local_8d7c;
  undefined8 local_8d74;
  undefined1 local_8d6c;
  undefined4 local_8d6b;
  undefined1 local_8d67;
  int aiStack_89ac [115];
  undefined8 local_87e0;
  ulong uStack_87d8;
  ulong uStack_87d0;
  undefined8 uStack_87c8;
  undefined8 uStack_87c0;
  undefined8 local_87b8;
  undefined8 uStack_87b0;
  undefined8 local_87a8;
  undefined8 uStack_87a0;
  undefined8 local_8798;
  undefined8 uStack_8790;
  undefined8 local_8788;
  uint local_3fe0;
  undefined4 uStack_3fdc;
  uint uStack_3fd8;
  uint uStack_3fd4;
  uint local_3fd0;
  byte bStack_3fcc;
  byte bStack_3fcb;
  undefined2 uStack_3fca;
  undefined1 auStack_3f73 [16123];
  long local_78;
  
  iVar24 = DAT_0025ca80;
  lVar9 = tpidr_el0;
  local_78 = *(long *)(lVar9 + 0x28);
  if (param_1 == (undefined1 *)0x0) goto LAB_00163e80;
  puVar39 = param_1;
  if (DAT_0025ca80 != 0) {
    memset(auStack_8e70,0,0x690);
    local_8e67 = 1;
    if (iVar24 == 1) {
      local_8788 = 0;
      uStack_8790 = 0;
      local_8798 = 0;
      uStack_87a0 = 0;
      local_87a8 = 0;
      uStack_87b0 = 0;
      local_87b8 = 0;
      uStack_87c0 = 0;
      uStack_87c8 = 0;
      uStack_87d0 = 0;
      uStack_87d8 = 0;
      local_87e0 = DAT_0010f850;
      iVar24 = FUN_00191c90(&local_87e0);
      if ((((iVar24 != 0) && ((uint)local_87e0 == 1)) && (local_87e0._4_4_ == 0x60)) &&
         ((uStack_87d8 & 1) != 0)) {
        uRam000000000025cab8 = local_87b8;
        _DAT_0025cab0 = uStack_87c0;
        uRam000000000025cac8 = local_87a8;
        _DAT_0025cac0 = uStack_87b0;
        uRam000000000025cad8 = local_8798;
        _DAT_0025cad0 = uStack_87a0;
        uRam000000000025cae8 = local_8788;
        _DAT_0025cae0 = uStack_8790;
        uRam000000000025ca98 = uStack_87d8;
        _DAT_0025ca90 = local_87e0;
        uRam000000000025caa8 = uStack_87c8;
        _DAT_0025caa0 = uStack_87d0;
      }
      local_8e68 = 0x4c;
      local_8e58 = &DAT_00137c9c;
      local_8e50 = "FPS LIMIT";
      local_8e18[0] = "FPS LIMIT";
      local_8e18[1] = "1-144 FPS; 145 MEANS UNLIMITED";
      local_8e6c = 4;
      local_8e20[0] = 0x2e001;
      puVar39 = auStack_8e70;
      local_8e60 = 0x2e000;
      local_8e00[0] = 0x18;
      local_8e00[1] = DAT_0025ca84;
      local_8dac = DAT_0025ca84 != DAT_0025ca88;
      local_8e00[2] = DAT_0025ca88;
      local_8e00[3] = DAT_0025ca84;
      local_8df0 = 0x10100000000;
      local_8de0 = 0x2e002;
      pcStack_8dd0 = "APPLY SELECTED FRAME LIMIT";
      if (!(bool)local_8dac) {
        pcStack_8dd0 = "CURRENT LIMIT IS SAVED";
      }
      local_8e48 = 0;
      local_8e40 = 0x18;
      local_8dd8 = &DAT_00132558;
      uStack_8e34 = 0;
      local_8e3c = 0;
      local_8e2c = 1;
      local_8da0 = 0x2e003;
      local_8e2b = 0;
      local_8e29 = 0;
      local_8e28 = 1;
      local_8d98 = "RESET";
      pcStack_8d90 = "USE THE DEVICE DEFAULT LIMIT";
      local_8e18[2] = (char *)0x0;
      local_8de8 = 3;
      local_8dc8 = 0;
      local_8dc0 = 0x18;
      local_8dbc = 0;
      local_8db4 = 0;
      local_8dab = 0;
      local_8da7 = 0;
      local_8d88 = 0;
      local_8d80 = 0x18;
      local_8d7c = 0;
      local_8d74 = 0;
      local_8d6c = 1;
      local_8d6b = 0;
      local_8d67 = 0;
    }
    else if (iVar24 == 2) {
      local_87b8 = 0;
      uStack_87c0 = 0;
      uStack_87c8 = 0;
      uStack_87d0 = 0;
      uStack_87d8 = 0;
      local_87e0 = DAT_0010f748;
      iVar24 = FUN_00191df8(&local_87e0);
      if (((iVar24 != 0) && ((uint)local_87e0 == 1)) &&
         ((local_87e0._4_4_ == 0x30 && (((int)uStack_87d8 != 0 && ((uint)uStack_87d0 < 6)))))) {
        _DAT_0025caf8 = uStack_87d8;
        _DAT_0025caf0 = local_87e0;
        _DAT_0025cb08 = uStack_87c8;
        _DAT_0025cb00 = uStack_87d0;
        uRam000000000025cb18 = local_87b8;
        _DAT_0025cb10 = uStack_87c0;
      }
      lVar30 = 0;
      local_8e60 = 0x2e010;
      local_8e68 = 0x4e;
      local_8e6c = 7;
      local_8e58 = &DAT_00137c9c;
      local_8e50 = "FONT";
      local_8e48 = 0;
      local_8e40 = 0x18;
      uStack_8e34 = 0;
      local_8e3c = 0;
      local_8e2c = 1;
      local_8e2b = 0;
      local_8e29 = 0;
      local_8e28 = 1;
      lVar27 = 0;
      do {
        uVar34 = (uint)lVar30;
        if (DAT_0025caf8 == 0 || DAT_0025cafc == 0) {
          uVar36 = 0;
        }
        else {
          uVar36 = DAT_0025cb08 >> (ulong)(uVar34 & 0x1f) & 1;
        }
        iVar24 = (&DAT_00131030)[lVar30];
        bVar19 = (_DAT_0025cb00 & 0xffffffff) * 0x40 - lVar27 == 0;
        pcVar4 = "CURRENT FONT";
        if (!bVar19) {
          pcVar4 = "AVAILABLE";
        }
        puVar39 = auStack_8e70;
        uVar26 = 0;
        if (DAT_0025cb04 == 0) {
          uVar26 = (uint)bVar19;
        }
        if (uVar36 == 0) {
          pcVar4 = "FONT ASSET IS NOT LOADED";
        }
        lVar1 = lVar27 + 0x40;
        *(uint *)((long)local_8e00 + lVar27 + 8) = uVar26;
        *(uint *)((long)local_8e00 + lVar27 + 0xc) = uVar34;
        lVar30 = lVar30 + 1;
        *(long *)((long)local_8e18 + lVar27) = (long)&DAT_00131030 + (long)iVar24;
        *(char **)((long)local_8e18 + lVar27 + 8) = pcVar4;
        *(uint *)((long)local_8e20 + lVar27) = uVar34 + 0x2e020;
        *(undefined8 *)((long)local_8e18 + lVar27 + 0x10) = 0;
        *(undefined4 *)((long)local_8e00 + lVar27) = 0x18;
        *(uint *)((long)local_8e00 + lVar27 + 4) = (uint)bVar19;
        *(uint *)((long)&local_8df0 + lVar27) = uVar36 ^ 1;
        *(char *)((long)&local_8df0 + lVar27 + 4) = (char)uVar36;
        *(undefined1 *)((long)&local_8df0 + lVar27 + 5) = 1;
        *(undefined2 *)((long)&local_8df0 + lVar27 + 6) = 0;
        *(undefined1 *)((long)&local_8de8 + lVar27) = 0;
        *(bool *)((long)&local_8de8 + lVar27 + 1) = bVar19;
        lVar27 = lVar1;
      } while (lVar1 != 0x180);
    }
    else {
      puVar39 = auStack_8e70;
    }
  }
  puVar15 = PTR_DAT_001a36f0;
  uVar12 = _UNK_00131020;
  uVar11 = _DAT_00131018;
  uVar41 = 0;
  uVar38 = 1;
  do {
    local_3fd0 = 0;
    bStack_3fcc = 0;
    bStack_3fcb = 0;
    uStack_3fca = 0;
    uStack_3fd8 = (uint)uVar12;
    uVar36 = uStack_3fd8;
    uStack_3fd4 = (uint)((ulong)uVar12 >> 0x20);
    uVar26 = uStack_3fd4;
    local_3fe0 = (uint)uVar11;
    uVar34 = local_3fe0;
    uStack_3fdc._0_1_ = (char)((ulong)uVar11 >> 0x20);
    cVar16 = (char)uStack_3fdc;
    uStack_3fdc._1_1_ = (char)((ulong)uVar11 >> 0x28);
    cVar17 = uStack_3fdc._1_1_;
    uStack_3fdc._2_2_ = (undefined2)((ulong)uVar11 >> 0x30);
    uVar18 = uStack_3fdc._2_2_;
    iVar24 = nexus_menu_action_status(uVar38 & 0xffffffff,&local_3fe0,param_2);
    if (iVar24 == 1) {
      bVar19 = ((uint)uVar38 & 0xfc) == 0x94;
      bVar5 = puVar15[uVar38 * 0x48 + 0x44];
      uVar25 = (uint)bStack_3fcc;
      if (bVar19) {
        uVar25 = 1;
      }
      uVar2 = 0;
      if (!bVar19) {
        uVar2 = local_3fd0;
      }
      if (uVar41 != 0) {
        puVar31 = (uint *)&local_87e0;
        uVar37 = uVar41;
        do {
          uVar33 = uVar41;
          if (uVar38 == *puVar31) goto LAB_00162d3c;
          uVar37 = uVar37 - 1;
          puVar31 = puVar31 + 9;
        } while (uVar37 != 0);
        if (0x1ff < uVar41) goto LAB_00162d50;
      }
      uVar33 = uVar41 + 1;
      puVar31 = (uint *)((long)&local_87e0 + uVar41 * 0x24);
LAB_00162d3c:
      *puVar31 = (uint)uVar38;
      puVar31[1] = (uint)bVar5;
      puVar31[2] = 0;
      puVar31[3] = uVar25;
      puVar31[4] = (uint)bStack_3fcb;
      *(ulong *)(puVar31 + 5) =
           CONCAT44(uStack_3fd8,
                    CONCAT22(uStack_3fdc._2_2_,CONCAT11(uStack_3fdc._1_1_,(char)uStack_3fdc)));
      puVar31[7] = uStack_3fd4;
      puVar31[8] = uVar2;
      uVar41 = uVar33;
    }
LAB_00162d50:
    uVar38 = uVar38 + 1;
  } while (uVar38 != 0xa8);
  if (uVar41 < 0x200) {
    uVar38 = 0;
    uVar37 = uVar41;
    do {
      local_3fd0 = 0;
      bStack_3fcc = 0;
      bStack_3fcb = 0;
      uStack_3fca = 0;
      local_3fe0 = uVar34;
      uStack_3fdc._0_1_ = cVar16;
      uStack_3fdc._1_1_ = cVar17;
      uStack_3fdc._2_2_ = uVar18;
      uStack_3fd8 = uVar36;
      uStack_3fd4 = uVar26;
      iVar24 = FUN_00192340((&PTR_s_DisablePinAnimation_0019ae50)[uVar38 * 3],&local_3fe0);
      bVar5 = bStack_3fcc;
      uVar41 = uVar37;
      if (iVar24 == 1) {
        uVar25 = ((int)(0x29811000000000 >> (uVar38 & 0x3f)) << 1 ^ 0xffffffffU) & 2;
        if ((0x42020ff0000000U >> (uVar38 & 0x3f) & 1) != 0) {
          uVar25 = 4;
        }
        if (uVar37 != 0) {
          puVar31 = (uint *)&local_87e0;
          uVar33 = uVar37;
          do {
            if (uVar38 + 0x21000 == (ulong)*puVar31) goto LAB_00162e8c;
            uVar33 = uVar33 - 1;
            puVar31 = puVar31 + 9;
          } while (uVar33 != 0);
        }
        uVar41 = uVar37 + 1;
        puVar31 = (uint *)((long)&local_87e0 + uVar37 * 0x24);
LAB_00162e8c:
        *puVar31 = (uint)(uVar38 + 0x21000);
        bVar6 = bStack_3fcb;
        puVar31[1] = uVar25;
        uVar11 = CONCAT44(uStack_3fd8,
                          CONCAT22(uStack_3fdc._2_2_,CONCAT11(uStack_3fdc._1_1_,(char)uStack_3fdc)))
        ;
        auVar44._8_4_ = uStack_3fd4;
        auVar44._0_8_ = uVar11;
        auVar44._12_4_ = local_3fd0;
        puVar31[2] = 0;
        puVar31[3] = (uint)bVar5;
        puVar31[4] = (uint)bVar6;
        *(long *)(puVar31 + 7) = auVar44._8_8_;
        *(undefined8 *)(puVar31 + 5) = uVar11;
      }
    } while ((uVar38 < 0x36) && (uVar38 = uVar38 + 1, uVar37 = uVar41, uVar41 < 0x200));
    uVar34 = *(uint *)(param_1 + 4);
    if ((uVar34 != 0) && (uVar41 < 0x200)) {
      uVar38 = 0;
      uVar37 = uVar41;
      do {
        iVar24 = *(int *)(param_1 + uVar38 * 0x40 + 0x10);
        uVar41 = uVar37;
        if (iVar24 != 0) {
          if (uVar37 != 0) {
            piVar28 = (int *)&local_87e0;
            uVar33 = uVar37;
            do {
              if (*piVar28 == iVar24) goto LAB_00162f24;
              uVar33 = uVar33 - 1;
              piVar28 = piVar28 + 9;
            } while (uVar33 != 0);
          }
          uVar41 = uVar37 + 1;
          piVar28 = (int *)((long)&local_87e0 + uVar37 * 0x24);
LAB_00162f24:
          bVar5 = param_1[uVar38 * 0x40 + 0x48];
          bVar6 = param_1[uVar38 * 0x40 + 0x49];
          bVar7 = param_1[uVar38 * 0x40 + 0x44];
          bVar8 = param_1[uVar38 * 0x40 + 0x45];
          auVar44 = *(undefined1 (*) [16])(param_1 + uVar38 * 0x40 + 0x34);
          *piVar28 = iVar24;
          piVar28[1] = (uint)bVar5;
          piVar28[2] = (uint)bVar6;
          piVar28[3] = (uint)bVar7;
          piVar28[4] = (uint)bVar8;
          *(long *)(piVar28 + 7) = auVar44._8_8_;
          *(long *)(piVar28 + 5) = auVar44._0_8_;
        }
        uVar38 = uVar38 + 1;
      } while ((uVar38 < uVar34) && (uVar37 = uVar41, uVar41 < 0x200));
    }
  }
  if ((puVar39 != param_1) && ((uVar34 = *(uint *)(puVar39 + 4), uVar34 != 0 && (uVar41 < 0x200))))
  {
    uVar38 = 0;
    uVar37 = uVar41;
    do {
      if (uVar37 != 0) {
        piVar28 = (int *)&local_87e0;
        uVar33 = uVar37;
        do {
          uVar41 = uVar37;
          if (*piVar28 == *(int *)(puVar39 + uVar38 * 0x40 + 0x10)) goto LAB_00162fc4;
          uVar33 = uVar33 - 1;
          piVar28 = piVar28 + 9;
        } while (uVar33 != 0);
      }
      uVar41 = uVar37 + 1;
      piVar28 = (int *)((long)&local_87e0 + uVar37 * 0x24);
LAB_00162fc4:
      bVar5 = puVar39[uVar38 * 0x40 + 0x48];
      bVar6 = puVar39[uVar38 * 0x40 + 0x49];
      bVar7 = puVar39[uVar38 * 0x40 + 0x44];
      bVar8 = puVar39[uVar38 * 0x40 + 0x45];
      auVar44 = *(undefined1 (*) [16])(puVar39 + uVar38 * 0x40 + 0x34);
      *piVar28 = *(int *)(puVar39 + uVar38 * 0x40 + 0x10);
      piVar28[1] = (uint)bVar5;
      piVar28[2] = (uint)bVar6;
      piVar28[3] = (uint)bVar7;
      piVar28[4] = (uint)bVar8;
      *(long *)(piVar28 + 7) = auVar44._8_8_;
      *(long *)(piVar28 + 5) = auVar44._0_8_;
    } while ((uVar38 + 1 < (ulong)uVar34) && (uVar38 = uVar38 + 1, uVar37 = uVar41, uVar41 < 0x200))
    ;
  }
  iVar24 = nexus_menu_ui_state(&local_3fe0,param_2);
  uVar38 = uVar41;
  if (iVar24 == 1) {
    uVar34 = 1;
    if (((DAT_0022d88c & 1) == 0) && ((DAT_0022d890 & 1) == 0)) {
      uVar34 = (uint)(DAT_0022f4b8 != 0);
    }
    bVar5 = DAT_0022d880._4_1_ | DAT_0022d888;
    bVar6 = bVar5 | (DAT_0022dfbc != 0 || DAT_0022d894 != 0) | DAT_0022dfb8;
    iVar24 = (int)DAT_0022d880;
    if (uVar34 == 0 && ((bVar6 ^ 0xff) & 1) == 0) {
      iVar24 = DAT_0025cb38;
    }
    iVar35 = DAT_0025cb38;
    if (uVar34 == 0 && (bVar6 & 1) == 0) {
      iVar35 = (int)DAT_0022d880;
    }
    iVar32 = 4;
    if ((bVar5 & 1) == 0) {
      iVar32 = 1;
    }
    uVar34 = (uint)((char)uStack_3fdc != '\0' && uStack_3fdc._1_1_ == '\0') & (bVar5 ^ 0xffffffff) &
             (uVar34 ^ 1);
    iVar3 = 0;
    if (uVar34 == 0) {
      iVar3 = iVar32;
    }
    if (uVar41 != 0) {
      piVar28 = (int *)&local_87e0;
      uVar37 = uVar41;
      do {
        if (*piVar28 == 0x20002) goto LAB_00163130;
        uVar37 = uVar37 - 1;
        piVar28 = piVar28 + 9;
      } while (uVar37 != 0);
      if (0x1ff < uVar41) goto LAB_00163150;
    }
    uVar38 = uVar41 + 1;
    piVar28 = (int *)((long)&local_87e0 + uVar41 * 0x24);
LAB_00163130:
    piVar28[2] = 0;
    piVar28[3] = uVar34;
    piVar28[6] = iVar35;
    piVar28[7] = (int)DAT_0022d880;
    piVar28[8] = iVar3;
    piVar28[4] = 1;
    piVar28[5] = iVar24;
    *(undefined8 *)piVar28 = DAT_0010f8f8;
  }
LAB_00163150:
  DAT_002815ac = nexus_menu_section_snapshot(0x25cb44,0x24a68,param_2);
  if (DAT_002815ac == 0) goto LAB_00163e80;
  uVar41 = FUN_00193f80(1,&DAT_00237e60);
  if ((uVar41 & 1) == 0) {
    iVar24 = memcmp(&DAT_00237e64,(void *)((long)&DAT_0025cb40 + 4),0x24a68);
    DAT_00237e60 = 0;
    if (iVar24 != 0) goto LAB_001631b4;
  }
  else {
LAB_001631b4:
    uVar41 = FUN_00193f80(1,&DAT_0022f500);
    if ((uVar41 & 1) != 0) {
      DAT_002815ac = 0;
      goto LAB_00163e80;
    }
    DAT_00233ee8 = 1;
    DAT_0022f500 = 0;
  }
  if (DAT_0025cb4c == 0) {
LAB_00163440:
    uVar41 = uVar38;
    if (uVar38 != 0) goto LAB_00163444;
  }
  else {
    if (DAT_0025cb74 != 0) {
      uVar41 = 0;
      do {
        if (uVar38 == 0) {
LAB_00163208:
          uVar33 = uVar38 + 1;
          piVar28 = (int *)((long)&local_87e0 + uVar38 * 0x24);
LAB_00163214:
          uVar36 = (uint)(&DAT_0025cdb4)[uVar41 * 0x92] >> 1 & 1;
          uVar34 = (&DAT_0025cdb4)[uVar41 * 0x92] & 1;
          *piVar28 = (&DAT_0025cdac)[uVar41 * 0x92];
          piVar28[1] = 0;
          piVar28[2] = uVar36;
          piVar28[3] = uVar34;
          piVar28[4] = 0;
          piVar28[5] = uVar36;
          piVar28[6] = uVar36;
          piVar28[7] = uVar36;
          piVar28[8] = uVar34 ^ 1;
          uVar38 = uVar33;
        }
        else {
          piVar28 = (int *)&local_87e0;
          uVar37 = uVar38;
          do {
            uVar33 = uVar38;
            if (*piVar28 == (&DAT_0025cdac)[uVar41 * 0x92]) goto LAB_00163214;
            uVar37 = uVar37 - 1;
            piVar28 = piVar28 + 9;
          } while (uVar37 != 0);
          if (uVar38 < 0x200) goto LAB_00163208;
        }
        uVar41 = uVar41 + 1;
      } while (uVar41 != DAT_0025cb74);
    }
    bVar19 = DAT_0025cb64 != 0 && DAT_0025cb68 == 0;
    if (DAT_0025cb4c == 1) {
      if (uVar38 == 0) {
LAB_0016334c:
        uVar37 = uVar38 + 1;
        piVar28 = (int *)((long)&local_87e0 + uVar38 * 0x24);
LAB_00163360:
        piVar28[4] = 0;
        piVar28[5] = 0;
        piVar28[6] = 0;
        piVar28[7] = 0;
        piVar28[8] = 0;
        *(undefined8 *)(piVar28 + 2) = _UNK_0010fb08;
        *(undefined8 *)piVar28 = _DAT_0010fb00;
      }
      else {
        piVar28 = (int *)&local_87e0;
        uVar41 = uVar38;
        do {
          uVar37 = uVar38;
          if (*piVar28 == 0x24001) goto LAB_00163360;
          uVar41 = uVar41 - 1;
          piVar28 = piVar28 + 9;
        } while (uVar41 != 0);
        if (uVar38 < 0x200) goto LAB_0016334c;
      }
      piVar28 = (int *)&local_87e0;
      uVar34 = DAT_0025cb7c >> 0x1f;
      uVar38 = uVar37;
      do {
        uVar41 = uVar37;
        if (*piVar28 == 0x24002) goto LAB_001633c0;
        uVar38 = uVar38 - 1;
        piVar28 = piVar28 + 9;
      } while (uVar38 != 0);
      if (uVar37 < 0x200) {
        uVar41 = uVar37 + 1;
        piVar28 = (int *)((long)&local_87e0 + uVar37 * 0x24);
LAB_001633c0:
        piVar28[2] = uVar34;
        piVar28[5] = uVar34;
        piVar28[6] = uVar34;
        piVar28[7] = uVar34;
        piVar28[8] = 0;
        *(undefined8 *)piVar28 = DAT_0010f760;
        *(undefined8 *)(piVar28 + 3) = DAT_0010f860;
      }
      uVar34 = (uint)bVar19;
      piVar28 = (int *)&local_87e0;
      uVar37 = uVar41;
      do {
        uVar38 = uVar41;
        if (*piVar28 == 0x24003) goto LAB_00163428;
        uVar37 = uVar37 - 1;
        piVar28 = piVar28 + 9;
      } while (uVar37 != 0);
      if (uVar41 < 0x200) {
        uVar38 = uVar41 + 1;
        piVar28 = (int *)((long)&local_87e0 + uVar41 * 0x24);
LAB_00163428:
        piVar28[2] = 0;
        piVar28[3] = uVar34;
        piVar28[4] = 0;
        piVar28[5] = 0;
        piVar28[6] = 0;
        piVar28[7] = 0;
        piVar28[8] = uVar34 ^ 1;
        *(undefined8 *)piVar28 = DAT_0010f8d8;
        goto LAB_00163440;
      }
    }
    else {
      if (DAT_0025cb4c == 2) {
        if (uVar38 == 0) {
LAB_001638e4:
          uVar37 = uVar38 + 1;
          piVar28 = (int *)((long)&local_87e0 + uVar38 * 0x24);
LAB_001638f8:
          piVar28[4] = 0;
          piVar28[5] = 0;
          piVar28[6] = 0;
          piVar28[7] = 0;
          piVar28[8] = 0;
          *(undefined8 *)(piVar28 + 2) = _UNK_0010fa48;
          *(undefined8 *)piVar28 = _DAT_0010fa40;
        }
        else {
          piVar28 = (int *)&local_87e0;
          uVar41 = uVar38;
          do {
            uVar37 = uVar38;
            if (*piVar28 == 0x26001) goto LAB_001638f8;
            uVar41 = uVar41 - 1;
            piVar28 = piVar28 + 9;
          } while (uVar41 != 0);
          if (uVar38 < 0x200) goto LAB_001638e4;
        }
        uVar36 = (uint)bVar19;
        uVar34 = 0;
        if (DAT_0025cb60 < 2) {
          uVar34 = uVar36;
        }
        piVar28 = (int *)&local_87e0;
        uVar26 = 0;
        if (DAT_0025cb70 != 0) {
          uVar26 = uVar34;
        }
        uVar38 = uVar37;
        do {
          uVar41 = uVar37;
          if (*piVar28 == 0x26002) goto LAB_0016396c;
          uVar38 = uVar38 - 1;
          piVar28 = piVar28 + 9;
        } while (uVar38 != 0);
        if (uVar37 < 0x200) {
          uVar41 = uVar37 + 1;
          piVar28 = (int *)((long)&local_87e0 + uVar37 * 0x24);
LAB_0016396c:
          piVar28[2] = 0;
          piVar28[3] = uVar26;
          piVar28[4] = 0;
          piVar28[5] = 0;
          piVar28[6] = 0;
          piVar28[7] = 0;
          piVar28[8] = uVar26 ^ 1;
          *(undefined8 *)piVar28 = DAT_0010f8c0;
        }
        if (uVar34 == 0) {
          uVar34 = 0;
        }
        else {
          uVar34 = (uint)(DAT_0025cb70 + DAT_0025cb74 < DAT_0025cb6c);
        }
        if (uVar41 == 0) {
LAB_00163a44:
          uVar37 = uVar41 + 1;
          piVar28 = (int *)((long)&local_87e0 + uVar41 * 0x24);
LAB_00163a58:
          piVar28[2] = 0;
          piVar28[3] = uVar34;
          piVar28[4] = 0;
          piVar28[5] = 0;
          piVar28[6] = 0;
          piVar28[7] = 0;
          piVar28[8] = uVar34 ^ 1;
          *(undefined8 *)piVar28 = DAT_0010f898;
        }
        else {
          piVar28 = (int *)&local_87e0;
          uVar38 = uVar41;
          do {
            uVar37 = uVar41;
            if (*piVar28 == 0x26003) goto LAB_00163a58;
            uVar38 = uVar38 - 1;
            piVar28 = piVar28 + 9;
          } while (uVar38 != 0);
          if (uVar41 < 0x200) goto LAB_00163a44;
        }
        uVar34 = 0;
        if (DAT_0025cb60 == 0) {
          uVar34 = uVar36;
        }
        piVar28 = (int *)&local_87e0;
        uVar38 = uVar37;
        do {
          uVar41 = uVar37;
          if (*piVar28 == 0x26004) goto LAB_00163ac4;
          uVar38 = uVar38 - 1;
          piVar28 = piVar28 + 9;
        } while (uVar38 != 0);
        if (uVar37 < 0x200) {
          uVar41 = uVar37 + 1;
          piVar28 = (int *)((long)&local_87e0 + uVar37 * 0x24);
LAB_00163ac4:
          piVar28[2] = 0;
          piVar28[3] = uVar34;
          piVar28[4] = 0;
          piVar28[5] = 0;
          piVar28[6] = 0;
          piVar28[7] = 0;
          piVar28[8] = uVar34 ^ 1;
          *(undefined8 *)piVar28 = DAT_0010f838;
        }
        uVar34 = 0;
        if (DAT_0025cb60 == 1) {
          uVar34 = uVar36;
        }
        uVar26 = 0;
        if (-1 < DAT_0025cb84) {
          uVar26 = uVar34;
        }
        if (uVar41 == 0) {
LAB_00163b28:
          uVar37 = uVar41 + 1;
          piVar28 = (int *)((long)&local_87e0 + uVar41 * 0x24);
LAB_00163b3c:
          piVar28[2] = 0;
          piVar28[3] = uVar26;
          piVar28[4] = 0;
          piVar28[5] = 0;
          piVar28[6] = 0;
          piVar28[7] = 0;
          piVar28[8] = uVar26 ^ 1;
          *(undefined8 *)piVar28 = DAT_0010f828;
        }
        else {
          piVar28 = (int *)&local_87e0;
          uVar38 = uVar41;
          do {
            uVar37 = uVar41;
            if (*piVar28 == 0x26006) goto LAB_00163b3c;
            uVar38 = uVar38 - 1;
            piVar28 = piVar28 + 9;
          } while (uVar38 != 0);
          if (uVar41 < 0x200) goto LAB_00163b28;
        }
        uVar26 = 0;
        uVar34 = 0;
        if (DAT_0025cb60 == 2) {
          uVar34 = uVar36;
        }
        if ((((uVar34 == 1) && (DAT_0025cb8c == 1)) && (uVar26 = 0, (DAT_0025cb78 & 1) != 0)) &&
           (-1 < DAT_0025cb84)) {
          uVar26 = (uint)(-1 < DAT_0025cb88 || DAT_0025cb88 == -2);
        }
        piVar28 = (int *)&local_87e0;
        uVar38 = uVar37;
        do {
          uVar41 = uVar37;
          if (*piVar28 == 0x26007) goto LAB_00163be4;
          uVar38 = uVar38 - 1;
          piVar28 = piVar28 + 9;
        } while (uVar38 != 0);
        if (uVar37 < 0x200) {
          uVar41 = uVar37 + 1;
          piVar28 = (int *)((long)&local_87e0 + uVar37 * 0x24);
LAB_00163be4:
          piVar28[2] = 0;
          piVar28[3] = uVar26;
          piVar28[4] = 0;
          piVar28[5] = 0;
          piVar28[6] = 0;
          piVar28[7] = 0;
          piVar28[8] = uVar26 ^ 1;
          *(undefined8 *)piVar28 = DAT_0010f7c8;
        }
        uVar34 = 0;
        if (DAT_0025cb60 == 3) {
          uVar34 = uVar36;
        }
        piVar28 = (int *)&local_87e0;
        uVar37 = uVar41;
        do {
          uVar38 = uVar41;
          if (*piVar28 == 0x26005) goto LAB_00163c50;
          uVar37 = uVar37 - 1;
          piVar28 = piVar28 + 9;
        } while (uVar37 != 0);
        if (uVar41 < 0x200) {
          uVar38 = uVar41 + 1;
          piVar28 = (int *)((long)&local_87e0 + uVar41 * 0x24);
LAB_00163c50:
          piVar28[2] = 0;
          piVar28[3] = uVar34 & DAT_0025cb78;
          piVar28[4] = 0;
          piVar28[5] = 0;
          piVar28[6] = 0;
          piVar28[7] = 0;
          piVar28[8] = uVar34 & DAT_0025cb78 ^ 1;
          *(undefined8 *)piVar28 = DAT_0010f830;
        }
        uVar36 = 0;
        do {
          if (uVar36 < 3) {
            uVar26 = 2;
joined_r0x00163d10:
            if (uVar36 < 6) {
                    /* WARNING: Could not recover jumptable at 0x00163d30. Too many branches */
                    /* WARNING: Treating indirect jump as call */
              (*(code *)(&UNK_00163d34 + (ulong)(byte)(&DAT_0010fbd0)[uVar36] * 4))();
              return;
            }
            iVar24 = DAT_0025cb98;
            if (uVar36 != 6) {
              iVar24 = 0;
            }
            if (uVar34 == 0) goto LAB_00163cfc;
LAB_00163d50:
            uVar26 = (uint)((uVar26 & DAT_0025cb78) != 0);
          }
          else {
            if (uVar36 != 5) {
              uVar26 = 8;
              if (uVar36 != 6) {
                uVar26 = 1;
              }
              goto joined_r0x00163d10;
            }
            uVar26 = 4;
            iVar24 = DAT_0025cb94;
            if (uVar34 != 0) goto LAB_00163d50;
LAB_00163cfc:
            uVar26 = 0;
          }
          iVar35 = 2;
          if (4 < uVar36 == 0) {
            iVar35 = 0;
          }
          if (uVar38 == 0) {
LAB_00163c9c:
            uVar37 = uVar38 + 1;
            piVar28 = (int *)((long)&local_87e0 + uVar38 * 0x24);
LAB_00163ca8:
            uVar25 = (uint)(iVar24 != 0);
            piVar28[2] = uVar25;
            piVar28[3] = uVar26;
            *piVar28 = uVar36 + 0x26008;
            piVar28[1] = iVar35;
            piVar28[4] = (uint)(4 < uVar36);
            piVar28[5] = uVar25;
            uVar25 = (uint)(iVar24 != 0);
            piVar28[6] = uVar25;
            piVar28[7] = uVar25;
            piVar28[8] = uVar26 ^ 1;
            uVar38 = uVar37;
          }
          else {
            piVar28 = (int *)&local_87e0;
            uVar41 = uVar38;
            do {
              uVar37 = uVar38;
              if (*piVar28 == uVar36 + 0x26008) goto LAB_00163ca8;
              uVar41 = uVar41 - 1;
              piVar28 = piVar28 + 9;
            } while (uVar41 != 0);
            if (uVar38 < 0x200) goto LAB_00163c9c;
          }
          uVar36 = uVar36 + 1;
        } while (uVar36 != 7);
        goto LAB_00163440;
      }
      if (DAT_0025cb4c != 3) goto LAB_00163440;
      if (uVar38 == 0) {
LAB_001632ec:
        piVar28 = (int *)((long)&local_87e0 + uVar38 * 0x24);
        uVar38 = uVar38 + 1;
LAB_00163300:
        piVar28[4] = 0;
        piVar28[5] = 0;
        piVar28[6] = 0;
        piVar28[7] = 0;
        piVar28[8] = 0;
        *(undefined8 *)(piVar28 + 2) = _UNK_0010fb18;
        *(undefined8 *)piVar28 = _DAT_0010fb10;
        goto LAB_00163440;
      }
      piVar28 = (int *)&local_87e0;
      uVar41 = uVar38;
      do {
        if (*piVar28 == 0x27000) goto LAB_00163300;
        uVar41 = uVar41 - 1;
        piVar28 = piVar28 + 9;
      } while (uVar41 != 0);
      uVar41 = uVar38;
      if (uVar38 < 0x200) goto LAB_001632ec;
    }
LAB_00163444:
    puVar29 = &uStack_87c8;
    uVar38 = uVar41;
    do {
      if (((*(int *)(puVar29 + -3) == 0x79) && (*(int *)((long)puVar29 + -0xc) != 0)) &&
         (*(int *)(puVar29 + -1) != 0)) {
        *(undefined4 *)puVar29 = *(undefined4 *)((long)puVar29 + -4);
      }
      uVar38 = uVar38 - 1;
      puVar29 = (undefined8 *)((long)puVar29 + 0x24);
    } while (uVar38 != 0);
  }
  uVar38 = FUN_00193f80(1,&DAT_00233eec);
  if ((uVar38 & 1) == 0) {
    memset(&local_3fe0,0,0x3f60);
    uVar14 = _UNK_0010fa68;
    uVar13 = _DAT_0010fa60;
    uVar12 = _UNK_0010f978;
    uVar11 = _DAT_0010f970;
    uVar34 = *(uint *)(puVar39 + 4);
    if (uVar34 == 0) {
      uVar38 = 0;
    }
    else {
      uVar38 = 0;
      pbVar40 = puVar39 + 0x49;
      uVar37 = 1;
      do {
        uVar36 = *(uint *)(pbVar40 + -0x39);
        if (uVar36 != 0) {
          auVar43._0_4_ = uVar36 + (int)uVar13;
          auVar43._4_4_ = uVar36 + (int)((ulong)uVar13 >> 0x20);
          auVar43._8_4_ = uVar36 + (int)uVar14;
          auVar43._12_4_ = uVar36 + (int)((ulong)uVar14 >> 0x20);
          auVar10._8_8_ = uVar12;
          auVar10._0_8_ = uVar11;
          auVar44 = NEON_cmhi(auVar10,auVar43,4);
          uVar42 = NEON_umaxv(CONCAT26(auVar44._12_2_,
                                       CONCAT24(auVar44._8_2_,CONCAT22(auVar44._4_2_,auVar44._0_2_))
                                      ),2);
          if ((((uVar42 & 1) != 0) || ((uVar36 & 0xffffffc0) == 0x26000)) ||
             (((uVar36 & 0xfffffe00) == 0x29000 || ((uVar36 & 0xfffffffc) == 0x2e000)))) {
            bVar6 = pbVar40[-1];
            bVar5 = *pbVar40;
            __s = (void *)((long)&uStack_3fd4 + uVar38 * 0x270);
            (&local_3fe0)[uVar38 * 0x9c] = uVar36;
            (&uStack_3fdc)[uVar38 * 0x9c] = (uint)bVar6;
            (&uStack_3fd8)[uVar38 * 0x9c] = (uint)bVar5;
            memset(__s,0,0x264);
            FUN_00158f70(__s,0x61,*(undefined8 *)(pbVar40 + -0x31));
            FUN_00158f70(auStack_3f73 + uVar38 * 0x270,0x201,*(undefined8 *)(pbVar40 + -0x29));
            uVar38 = uVar38 + 1;
          }
        }
        if (uVar34 <= uVar37) break;
        pbVar40 = pbVar40 + 0x40;
        uVar37 = uVar37 + 1;
      } while (uVar38 < 0x1a);
    }
    uVar34 = DAT_00233ef8;
    uVar36 = (uint)(byte)puVar39[8];
    if (DAT_00233ef4 == uVar36) {
      bVar5 = puVar39[0xb];
      bVar19 = true;
      if (DAT_00233ef8 != bVar5) goto LAB_00163648;
      uVar26 = (uint)bVar5;
      if (DAT_00233efc == uVar38) {
        iVar24 = memcmp(&DAT_00233f00,&local_3fe0,uVar38 * 0x270);
        bVar19 = iVar24 != 0;
        uVar26 = uVar34;
      }
    }
    else {
      bVar5 = puVar39[0xb];
LAB_00163648:
      uVar26 = (uint)bVar5;
      bVar19 = true;
    }
    DAT_00233efc = (uint)uVar38;
    DAT_00233ef4 = uVar36;
    DAT_00233ef8 = uVar26;
    if (uVar38 != 0) {
      __memcpy_chk(&DAT_00233f00,&local_3fe0,uVar38 * 0x270,0x3f60);
    }
    if (bVar19) {
      bVar19 = DAT_00233ef0 == -1;
      DAT_00233ef0 = DAT_00233ef0 + 1;
      if (bVar19) {
        DAT_00233ef0 = 1;
      }
      DAT_00233eec = 0;
      DAT_0025cb40._0_4_ = 1;
    }
    else {
      DAT_00233eec = 0;
    }
  }
  if (((int)DAT_0025cb40 != 0) && (uVar38 = FUN_00193f80(1,&DAT_0022f500), (uVar38 & 1) == 0)) {
    DAT_00233ee8 = 1;
    DAT_0022f500 = 0;
  }
  if (uVar41 < 0x201) {
    bVar5 = puVar39[9];
    bVar6 = puVar39[8];
    bVar7 = puVar39[10];
    bVar8 = puVar39[0xb];
    uVar38 = FUN_00193f80(1,&DAT_0022f500);
    iVar24 = DAT_00233ee4;
    if ((uVar38 & 1) == 0) {
      uVar38 = (ulong)DAT_00233ecc;
      DAT_00233ee4 = 1;
      bVar19 = DAT_00233ee8 != 0;
      uVar34 = (uint)bVar5;
      uVar36 = (uint)bVar6;
      bVar20 = DAT_00233ed4 != uVar34;
      bVar21 = DAT_00233ed8 != uVar36;
      uVar26 = (uint)bVar7;
      uVar25 = (uint)bVar8;
      bVar22 = DAT_00233edc != uVar26;
      bVar23 = DAT_00233ee0 != uVar25;
      DAT_00233ecc = (uint)uVar41;
      DAT_00233ed4 = uVar34;
      DAT_00233ed8 = uVar36;
      DAT_00233edc = uVar26;
      DAT_00233ee0 = uVar25;
      if ((uVar41 == 0) ||
         ((((((uVar41 != uVar38 || bVar19) || iVar24 == 0) || bVar20) || bVar21) || bVar22) ||
          bVar23)) {
        if (uVar41 != 0) {
          __memcpy_chk(&DAT_0022f6cc,&local_87e0,uVar41 * 0x24,0x4820);
        }
        if ((((((uVar41 != uVar38 || bVar19) || iVar24 == 0) || bVar20) || bVar21) || bVar22) ||
            bVar23) {
LAB_00163dcc:
          bVar19 = DAT_00233ed0 == -1;
          DAT_00233ed0 = DAT_00233ed0 + 1;
          if (bVar19) {
            DAT_00233ed0 = 1;
          }
        }
      }
      else {
        lVar30 = 0x1cc;
        uVar38 = uVar41;
        do {
          if (((((*(int *)((long)&DAT_0022f500 + lVar30) != *(int *)((long)aiStack_89ac + lVar30))
                || (*(int *)((long)&DAT_0022f504 + lVar30) !=
                    *(int *)((long)aiStack_89ac + lVar30 + 4))) ||
               (*(int *)(lVar30 + 0x22f508) != *(int *)((long)aiStack_89ac + lVar30 + 8))) ||
              ((*(int *)(lVar30 + 0x22f50c) != *(int *)((long)aiStack_89ac + lVar30 + 0xc) ||
               (*(int *)(lVar30 + 0x22f510) != *(int *)((long)aiStack_89ac + lVar30 + 0x10))))) ||
             (((*(int *)(lVar30 + 0x22f514) != *(int *)((long)aiStack_89ac + lVar30 + 0x14) ||
               ((*(int *)(lVar30 + 0x22f518) != *(int *)((long)aiStack_89ac + lVar30 + 0x18) ||
                (*(int *)(lVar30 + 0x22f51c) != *(int *)((long)aiStack_89ac + lVar30 + 0x1c))))) ||
              (*(int *)(lVar30 + 0x22f520) != *(int *)((long)aiStack_89ac + lVar30 + 0x20))))) {
            __memcpy_chk(&DAT_0022f6cc,&local_87e0,uVar41 * 0x24,0x4820);
            goto LAB_00163dcc;
          }
          uVar38 = uVar38 - 1;
          lVar30 = lVar30 + 0x24;
        } while (uVar38 != 0);
        __memcpy_chk(&DAT_0022f6cc,&local_87e0,uVar41 * 0x24,0x4820);
      }
      DAT_00233ee8 = 0;
      DAT_0022f500 = 0;
      if ((DAT_002815ac != 0) &&
         (((DAT_0025cb4c == 0 ||
           (iVar24 = nexus_menu_section_present(0x25cb44,0x24a68,param_2), iVar24 != 0)) &&
          (uVar38 = FUN_00193f80(1,&DAT_0022f500), iVar24 = DAT_00233ed0, (uVar38 & 1) == 0)))) {
        DAT_0022f500 = 0;
        uVar38 = FUN_00193f80(1,&DAT_00237e60);
        if ((uVar38 & 1) == 0) {
          memcpy(&DAT_00237e64,(void *)((long)&DAT_0025cb40 + 4),0x24a68);
          DAT_0025c8cc = iVar24;
          DAT_00237e60 = 0;
        }
      }
      DAT_0025cb40._0_4_ = 0;
    }
  }
LAB_00163e80:
  if (*(long *)(lVar9 + 0x28) == local_78) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_001641ec @ 001641ec ===== */

void FUN_001641ec(undefined *param_1,undefined4 *param_2,long param_3)

{
  bool bVar1;
  uint uVar2;
  long lVar3;
  __pid_t _Var4;
  undefined4 uVar5;
  int iVar6;
  long lVar7;
  timespec local_1e8;
  undefined1 auStack_1d8 [384];
  long local_58;
  
  lVar3 = tpidr_el0;
  local_58 = *(long *)(lVar3 + 0x28);
  DAT_0025ca6c = 1;
  PTR_s_launcher_initializing_001a7c58 = param_1;
  if ((DAT_0025ca78 != param_1) && (DAT_0025ca70 < 8)) {
    FUN_0015540c((double)(float)param_2[8],(double)(float)param_2[9],(double)(float)param_2[10],
                 (double)(float)param_2[0xb],(double)*(float *)(param_3 + 8),
                 (double)*(float *)(param_3 + 0xc),(double)(float)param_2[0xc],
                 (double)(float)param_2[0xd],auStack_1d8,0x180,0x180,
                 "%s q=%d,%d,%d reads=%x root=%.9g,%.9g,%.9g,%.9g viewport=%.9gx%.9g scale=%.9g offset=%.9g,%.9g dims=%d,%d"
                 ,param_1,*param_2,param_2[1],param_2[2],param_2[0xf],(double)(float)param_2[0xe],
                 param_2[6],param_2[7]);
    uVar2 = DAT_0022f4bc + 1;
    bVar1 = DAT_0022f4bc < 0x80;
    DAT_0022f4bc = uVar2;
    if (bVar1) {
      _Var4 = getpid();
      uVar5 = gettid(_Var4);
      iVar6 = clock_gettime(0,&local_1e8);
      if (iVar6 == 0) {
        lVar7 = local_1e8.tv_sec * 1000 + (ulong)local_1e8.tv_nsec / 1000000;
      }
      else {
        lVar7 = 0;
      }
      __android_log_print(4,"NexusLab69252",
                          "{\"stage\":\"%s\",\"reason\":\"%s\",\"action\":%u,\"pid\":%d,\"tid\":%d,\"time_ms\":%llu}"
                          ,"nexus_shell_launcher_waiting",auStack_1d8,0,_Var4,uVar5,lVar7);
    }
    DAT_0025ca70 = DAT_0025ca70 + 1;
  }
  DAT_0025ca78 = param_1;
  if (*(long *)(lVar3 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00176418 @ 00176418 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_00176418(long param_1,int param_2)

{
  int *piVar1;
  ulong uVar2;
  uint uVar3;
  long lVar4;
  long lVar5;
  bool bVar6;
  int iVar7;
  undefined8 uVar8;
  long *plVar9;
  ulong uVar10;
  ulong uVar11;
  ulong uVar12;
  double dVar13;
  
  uVar8 = FUN_00179448();
  if ((int)uVar8 == 0) {
    return uVar8;
  }
  if (DAT_0022d798 == 0) {
    return 1;
  }
  iVar7 = nexus_menu_main_count();
  lVar5 = DAT_0022d7b8;
  lVar4 = DAT_0022d7b0;
  uVar3 = DAT_0022d790;
  plVar9 = &DAT_0022d7d8;
  if (DAT_0022d794 == 0) {
    uVar8 = 0;
    goto LAB_001767ac;
  }
  uVar11 = (ulong)(iVar7 + 4) / 5;
  uVar12 = (ulong)DAT_0022d790;
  uVar8 = FUN_001830b0(DAT_0022d7b8,1,DAT_0022d7a8,uVar11);
  if ((int)uVar8 == 0) {
    plVar9 = &DAT_0022d7d8;
    goto LAB_001767ac;
  }
  if (uVar3 < 0xb) {
    if (uVar3 == 0) {
      bVar6 = true;
    }
    else {
      uVar10 = 0;
      bVar6 = false;
      do {
        if ((((((&DAT_0022d600)[uVar10 * 10] < 0) || (ABS((&DAT_0022d604)[uVar10 * 10]) == INFINITY)
              ) || (NAN(ABS((&DAT_0022d604)[uVar10 * 10])))) ||
            ((ABS((&DAT_0022d608)[uVar10 * 10]) == INFINITY ||
             (NAN(ABS((&DAT_0022d608)[uVar10 * 10])))))) ||
           ((1 < (uint)(&DAT_0022d60c)[uVar10 * 10] ||
            ((1 < (uint)(&DAT_0022d610)[uVar10 * 10] ||
             (piVar1 = &DAT_0022d600, uVar2 = uVar10,
             (&DAT_0022d60c)[uVar10 * 10] == (&DAT_0022d610)[uVar10 * 10])))))) break;
        for (; uVar2 != 0; uVar2 = uVar2 - 1) {
          if (*piVar1 == (&DAT_0022d600)[uVar10 * 10]) goto LAB_0017658c;
          piVar1 = piVar1 + 10;
        }
        uVar10 = uVar10 + 1;
        bVar6 = uVar12 <= uVar10;
      } while (uVar10 != uVar12);
    }
LAB_0017658c:
    if ((uVar3 < 2) && (bVar6)) {
      if (DAT_0022d574 != 0) {
        uVar8 = 0;
        plVar9 = &DAT_0022d7d8;
        goto LAB_001767ac;
      }
      if (DAT_0022d564 != 0) {
        if (uVar3 != 1) {
          uVar8 = 0;
          plVar9 = &DAT_0022d7d8;
          goto LAB_001767ac;
        }
        uVar8 = 0;
        plVar9 = &DAT_0022d7d8;
        if ((DAT_0022d600 != DAT_0022d5b0) || (DAT_0022d610 == 0)) goto LAB_001767ac;
        FUN_00183658(lVar5,&DAT_0022d600,uVar11,1);
      }
      uVar8 = 0;
      plVar9 = &DAT_0022d7d8;
      if ((DAT_0022d578 == 0) || (DAT_0022d57c != 0)) goto LAB_001767ac;
      if (500 < (ulong)(lVar5 - DAT_0022d5a0)) {
        uVar8 = 0;
        plVar9 = &DAT_0022d7d8;
        goto LAB_001767ac;
      }
      if (uVar3 != 0) {
        if (DAT_0022d60c != 0) {
          uVar8 = 0;
          plVar9 = &DAT_0022d7d8;
          goto LAB_001767ac;
        }
        if (DAT_0022d600 != DAT_0022d5b0) {
          uVar8 = 0;
          plVar9 = &DAT_0022d7d8;
          goto LAB_001767ac;
        }
        dVar13 = (double)NEON_fmadd((double)DAT_0022d604 - (double)DAT_0022d5b4,
                                    (double)DAT_0022d604 - (double)DAT_0022d5b4,
                                    ((double)DAT_0022d608 - (double)DAT_0022d5b8) *
                                    ((double)DAT_0022d608 - (double)DAT_0022d5b8));
        if ((((dVar13 < 64.0 == NAN(dVar13)) || (DAT_0022d5c4 == 0)) || (DAT_0022d5c8 == 0)) ||
           (((DAT_0022d5d0 == 0 || (DAT_0022d5c4 != DAT_0022d614)) ||
            ((DAT_0022d5c8 != DAT_0022d618 || (DAT_0022d5d0 != DAT_0022d620)))))) goto LAB_00176768;
      }
      uVar8 = 0;
      plVar9 = &DAT_0022d7d8;
      if (((DAT_0022d5c8 == param_1) && (DAT_0022d5c4 == param_2)) && (DAT_0022d5d0 == lVar4)) {
        plVar9 = &DAT_0022d7e0;
        uVar8 = 1;
        _DAT_0022d578 = 0;
      }
      goto LAB_001767ac;
    }
  }
LAB_00176768:
  DAT_0022d564 = 0;
  _DAT_0022d578 = _UNK_0010fa98;
  _DAT_0022d570 = _DAT_0010fa90;
  FUN_00183230(0,0,&DAT_0022d518,lVar5,0,4,0xffffffff,uVar11);
  uVar8 = 0;
  plVar9 = &DAT_0022d7d8;
LAB_001767ac:
  *plVar9 = *plVar9 + 1;
  return uVar8;
}

/* ===== nexus_rich_main_row @ 00178b1c ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint nexus_rich_main_row(void)

{
  uint uVar1;
  float fVar2;
  int iVar3;
  uint uVar4;
  
  iVar3 = nexus_menu_editor_scroll_revision();
  if ((iVar3 != DAT_0028467c) && (DAT_0028467c = iVar3, iVar3 != 0)) {
    DAT_0022d5f8 = 0;
    DAT_0022d794 = 0;
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
    _DAT_0022d560 = 0;
    DAT_0022d558 = 0;
    _DAT_0022d570 = 0;
    _DAT_0022d568 = 0;
    _DAT_0022d580 = 0;
    _DAT_0022d578 = 0;
    DAT_0022d590 = 0;
    DAT_0022d588 = 0;
    DAT_0022d5a0 = 0;
    DAT_0022d598 = 0;
    _DAT_0022d5b0 = 0;
    DAT_0022d5a8 = 0;
    _DAT_0022d5c0 = 0;
    _DAT_0022d5b8 = 0;
    DAT_0022d5d0 = 0;
    DAT_0022d5c8 = 0;
    _DAT_0022d5e0 = 0;
    _DAT_0022d5d8 = 0;
    _DAT_0022d5f0 = 0;
    _DAT_0022d5e8 = 0;
    DAT_0022d79c = 0xffffffff;
  }
  iVar3 = nexus_menu_theme_scroll_revision();
  if ((iVar3 != DAT_00284680) && (DAT_00284680 = iVar3, iVar3 != 0)) {
    DAT_0022d5f8 = 0;
    DAT_0022d794 = 0;
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
    _DAT_0022d560 = 0;
    DAT_0022d558 = 0;
    _DAT_0022d570 = 0;
    _DAT_0022d568 = 0;
    _DAT_0022d580 = 0;
    _DAT_0022d578 = 0;
    DAT_0022d590 = 0;
    DAT_0022d588 = 0;
    DAT_0022d5a0 = 0;
    DAT_0022d598 = 0;
    _DAT_0022d5b0 = 0;
    DAT_0022d5a8 = 0;
    _DAT_0022d5c0 = 0;
    _DAT_0022d5b8 = 0;
    DAT_0022d5d0 = 0;
    DAT_0022d5c8 = 0;
    _DAT_0022d5e0 = 0;
    _DAT_0022d5d8 = 0;
    _DAT_0022d5f0 = 0;
    _DAT_0022d5e8 = 0;
    DAT_0022d79c = 0xffffffff;
  }
  fVar2 = DAT_0022d518;
  iVar3 = nexus_menu_main_count();
  uVar4 = 0;
  if ((((ABS(fVar2) != INFINITY) && (!NAN(ABS(fVar2)))) && (0.0 <= fVar2)) &&
     (uVar1 = iVar3 + 4, 4 < uVar1)) {
    uVar4 = (uint)((fVar2 + 32.0) / 110.0);
    if (uVar1 / 5 < uVar4 || uVar1 / 5 == uVar4) {
      uVar4 = uVar1 / 5 - 1;
    }
  }
  return uVar4;
}

/* ===== FUN_0017fbc8 @ 0017fbc8 ===== */

void FUN_0017fbc8(undefined8 param_1,undefined8 param_2)

{
  bool bVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  
  puVar4 = (undefined4 *)__errno();
  uVar3 = *puVar4;
  iVar2 = DAT_001a7d20 + 1;
  bVar1 = DAT_001a7d20 < 0x40;
  DAT_001a7d20 = iVar2;
  if ((bVar1) && (DAT_001a7d00 != (code *)0x0)) {
    (*DAT_001a7d00)(DAT_001a7cc8,"nexus_menu_probe",param_2,0);
  }
  *puVar4 = uVar3;
  return;
}

/* ===== FUN_00180034 @ 00180034 ===== */

void FUN_00180034(undefined8 param_1,ulong param_2,long param_3)

{
  bool bVar1;
  bool bVar2;
  uint uVar3;
  ulong uVar4;
  long lVar5;
  int iVar6;
  FILE *__stream;
  char *pcVar7;
  undefined8 uVar8;
  undefined4 *puVar9;
  undefined4 uVar10;
  ulong local_270;
  ulong uStack_268;
  char local_260;
  char local_25f;
  char local_25e;
  char local_25d;
  char acStack_258 [512];
  long local_58;
  
  lVar5 = tpidr_el0;
  param_2 = param_2 & 0xffffffffffffff;
  local_58 = *(long *)(lVar5 + 0x28);
  if ((param_2 < 0x1000) || ((param_2 ^ 0xffffffffffffff) <= param_3 - 1U)) {
    puVar9 = (undefined4 *)__errno();
    uVar10 = *puVar9;
    uVar3 = DAT_001a7d20 + 1;
    bVar2 = DAT_001a7d20 < 0x40;
    DAT_001a7d20 = uVar3;
    if ((bVar2) && (DAT_001a7d00 != (code *)0x0)) {
      pcVar7 = "clip_writable_invalid";
LAB_00180270:
      (*DAT_001a7d00)(DAT_001a7cc8,"nexus_menu_probe",pcVar7,0);
    }
  }
  else {
    __stream = fopen("/proc/self/maps","r");
    if (__stream != (FILE *)0x0) {
      pcVar7 = fgets(acStack_258,0x200,__stream);
      if (pcVar7 != (char *)0x0) {
        uVar4 = param_2 + param_3;
        do {
          iVar6 = sscanf(acStack_258,"%lx-%lx %4s",&uStack_268,&local_270,&local_260);
          if ((((iVar6 == 3) && (local_270 >> 0x38 == 0)) &&
              (uStack_268 < local_270 && (param_2 < uVar4 && uVar4 >> 0x38 == 0))) &&
             ((uStack_268 <= param_2 && (uVar4 <= local_270)))) {
            bVar2 = false;
            if ((local_260 != 'r') || ((local_25f != 'w' || (local_25e != '-')))) goto LAB_001801b8;
            iVar6 = fclose(__stream);
            if (local_25d == 'p') {
              uVar8 = 1;
              goto LAB_00180284;
            }
            bVar2 = false;
            goto LAB_001801c0;
          }
          pcVar7 = fgets(acStack_258,0x200,__stream);
        } while (pcVar7 != (char *)0x0);
      }
      bVar2 = true;
LAB_001801b8:
      iVar6 = fclose(__stream);
LAB_001801c0:
      puVar9 = (undefined4 *)__errno(iVar6);
      uVar10 = *puVar9;
      uVar3 = DAT_001a7d20 + 1;
      bVar1 = DAT_001a7d20 < 0x40;
      DAT_001a7d20 = uVar3;
      if ((bVar1) && (DAT_001a7d00 != (code *)0x0)) {
        pcVar7 = "clip_writable_mapping_missing";
        if (!bVar2) {
          pcVar7 = "clip_writable_permissions";
        }
        (*DAT_001a7d00)(DAT_001a7cc8,"nexus_menu_probe",pcVar7,0);
      }
      uVar8 = 0;
      *puVar9 = uVar10;
      goto LAB_00180284;
    }
    puVar9 = (undefined4 *)__errno();
    uVar10 = *puVar9;
    uVar3 = DAT_001a7d20 + 1;
    bVar2 = DAT_001a7d20 < 0x40;
    DAT_001a7d20 = uVar3;
    if ((bVar2) && (DAT_001a7d00 != (code *)0x0)) {
      pcVar7 = "clip_writable_maps_unavailable";
      goto LAB_00180270;
    }
  }
  uVar8 = 0;
  *puVar9 = uVar10;
LAB_00180284:
  if (*(long *)(lVar5 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar8);
}

/* ===== nexus_menu_init @ 00183a08 ===== */

undefined8 nexus_menu_init(void)

{
  undefined4 *puVar1;
  undefined *puVar2;
  ulong uVar3;
  undefined8 uVar4;
  undefined4 *puVar5;
  long lVar6;
  undefined4 uVar7;
  
  uVar3 = FUN_00193f80(1,&DAT_00284688);
  puVar2 = PTR_PTR_s_nexus_sx_spin_001a36f8;
  if ((uVar3 & 1) == 0) {
    if (DAT_0028468c == 0) {
      lVar6 = 0x28;
      puVar5 = (undefined4 *)(PTR_PTR_s_nexus_sx_spin_001a36f8 + 0x28);
      do {
        puVar1 = puVar5 + -6;
        uVar7 = *puVar5;
        puVar5 = puVar5 + 0xc;
        *(ulong *)((long)&DAT_00284688 + lVar6) = CONCAT44(uVar7,*puVar1);
        lVar6 = lVar6 + 8;
      } while (lVar6 != 0x1f8);
      DAT_00284880 = *(undefined4 *)(puVar2 + 0xaf0);
      DAT_00284884 = *(undefined4 *)(puVar2 + 0xb08);
      DAT_0028469d = 0x52;
      DAT_0028468c = 1;
      DAT_002846a0 = 1;
      FUN_00183ac8(&DAT_00284aa8,0x40,0x40,"host_not_registered");
    }
    uVar4 = 1;
    DAT_00284688 = 0;
  }
  else {
    uVar4 = 3;
  }
  return uVar4;
}

/* ===== nexus_menu_start @ 00183b6c ===== */

void nexus_menu_start(void)

{
  undefined4 *puVar1;
  undefined *puVar2;
  ulong uVar3;
  undefined4 *puVar4;
  long lVar5;
  undefined4 uVar6;
  
  uVar3 = FUN_00193f80(1,&DAT_00284688);
  puVar2 = PTR_PTR_s_nexus_sx_spin_001a36f8;
  if ((uVar3 & 1) == 0) {
    if (DAT_0028468c == 0) {
      lVar5 = 0x28;
      puVar4 = (undefined4 *)(PTR_PTR_s_nexus_sx_spin_001a36f8 + 0x28);
      do {
        puVar1 = puVar4 + -6;
        uVar6 = *puVar4;
        puVar4 = puVar4 + 0xc;
        *(ulong *)((long)&DAT_00284688 + lVar5) = CONCAT44(uVar6,*puVar1);
        lVar5 = lVar5 + 8;
      } while (lVar5 != 0x1f8);
      DAT_00284880 = *(undefined4 *)(puVar2 + 0xaf0);
      DAT_00284884 = *(undefined4 *)(puVar2 + 0xb08);
      DAT_0028469d = 0x52;
      DAT_0028468c = 1;
      DAT_002846a0 = 1;
      FUN_00183ac8(&DAT_00284aa8,0x40,0x40,"host_not_registered");
    }
    DAT_00284688 = 0;
    DAT_00284ae8 = 1;
    FUN_001939e0(0,1,&DAT_00284aec);
    uVar3 = FUN_00193f80(1,&DAT_00284688);
    if ((uVar3 & 1) == 0) {
      DAT_00284690 = 1;
      if (DAT_00284698 == 0) {
        DAT_00284698 = 1;
      }
      DAT_00284688 = 0;
    }
  }
  return;
}

/* ===== nexus_menu_status @ 00183d70 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 nexus_menu_status(void)

{
  if (DAT_00284ae8 != 0) {
    return CONCAT44(DAT_00284af0,_DAT_00284aec);
  }
  return 0;
}

/* ===== nexus_menu_register_backend @ 00183e8c ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 nexus_menu_register_backend(uint param_1,int *param_2)

{
  ulong uVar1;
  undefined8 uVar2;
  
  if (7 < param_1) {
    return 4;
  }
  if (param_2 == (int *)0x0) {
    return 4;
  }
  if (*param_2 != 1) {
    return 4;
  }
  if (param_2[1] != 0x40) {
    return 4;
  }
  if (*(long *)(param_2 + 10) == 0) {
    return 4;
  }
  if (*(long *)(param_2 + 0xc) == 0) {
    return 4;
  }
  if (*(ulong *)(param_2 + 8) >> 0x28 != 0) {
    return 4;
  }
  uVar1 = FUN_00193f80(1,&DAT_00284688);
  if ((uVar1 & 1) != 0) {
    return 3;
  }
  if (param_1 == 0) {
LAB_00183f48:
    if ((*(ulong *)(param_2 + 4) & DAT_002848d8) != 0) {
      DAT_00284688 = 0;
      return 4;
    }
    if ((*(ulong *)(param_2 + 6) & DAT_002848e0) != 0) {
      DAT_00284688 = 0;
      return 4;
    }
    if ((*(ulong *)(param_2 + 8) & DAT_002848e8) != 0) {
      DAT_00284688 = 0;
      return 4;
    }
    if (param_1 != 2) goto LAB_0018405c;
LAB_00183f8c:
    if ((*(ulong *)(param_2 + 4) & DAT_00284958) != 0) {
      DAT_00284688 = 0;
      return 4;
    }
    if ((*(ulong *)(param_2 + 6) & DAT_00284960) != 0) {
      DAT_00284688 = 0;
      return 4;
    }
    if ((*(ulong *)(param_2 + 8) & DAT_00284968) != 0) {
      DAT_00284688 = 0;
      return 4;
    }
    if (param_1 == 4) goto LAB_00183fd0;
  }
  else {
    if ((*(ulong *)(param_2 + 4) & DAT_00284898) != 0) {
      DAT_00284688 = 0;
      return 4;
    }
    if ((*(ulong *)(param_2 + 6) & DAT_002848a0) != 0) {
      DAT_00284688 = 0;
      return 4;
    }
    if ((*(ulong *)(param_2 + 8) & DAT_002848a8) != 0) {
      DAT_00284688 = 0;
      return 4;
    }
    if (param_1 != 1) goto LAB_00183f48;
LAB_0018405c:
    if ((*(ulong *)(param_2 + 4) & DAT_00284918) != 0) {
      DAT_00284688 = 0;
      return 4;
    }
    if ((*(ulong *)(param_2 + 6) & DAT_00284920) != 0) {
      DAT_00284688 = 0;
      return 4;
    }
    if ((*(ulong *)(param_2 + 8) & DAT_00284928) != 0) {
      DAT_00284688 = 0;
      return 4;
    }
    if (param_1 != 3) goto LAB_00183f8c;
  }
  if ((*(ulong *)(param_2 + 4) & DAT_00284998) != 0) {
    DAT_00284688 = 0;
    return 4;
  }
  if ((*(ulong *)(param_2 + 6) & DAT_002849a0) != 0) {
    DAT_00284688 = 0;
    return 4;
  }
  if ((*(ulong *)(param_2 + 8) & DAT_002849a8) != 0) {
    DAT_00284688 = 0;
    return 4;
  }
  if (param_1 == 5) {
    if ((*(ulong *)(param_2 + 4) & DAT_00284a18) != 0) {
      DAT_00284688 = 0;
      return 4;
    }
    if ((*(ulong *)(param_2 + 6) & DAT_00284a20) != 0) {
      DAT_00284688 = 0;
      return 4;
    }
    if ((*(ulong *)(param_2 + 8) & DAT_00284a28) != 0) {
      DAT_00284688 = 0;
      return 4;
    }
    if ((*(ulong *)(param_2 + 4) & DAT_00284a58) != 0) {
      DAT_00284688 = 0;
      return 4;
    }
    if ((*(ulong *)(param_2 + 6) & DAT_00284a60) != 0) {
      DAT_00284688 = 0;
      return 4;
    }
    if ((*(ulong *)(param_2 + 8) & DAT_00284a68) != 0) {
      DAT_00284688 = 0;
      return 4;
    }
    DAT_00284688 = 0;
    DAT_002846a0 = DAT_002846a0 + 1;
    DAT_002849d8 = *(undefined8 *)(param_2 + 4);
    DAT_002849e0 = *(undefined8 *)(param_2 + 6);
    _DAT_002849c8 = *(undefined8 *)param_2;
    DAT_002849d0 = *(undefined8 *)(param_2 + 2);
    uRam00000000002849e8 = *(undefined8 *)(param_2 + 8);
    DAT_002849f0 = *(undefined8 *)(param_2 + 10);
    uRam00000000002849f8 = *(undefined8 *)(param_2 + 0xc);
    DAT_00284a00 = *(undefined8 *)(param_2 + 0xe);
    return 1;
  }
LAB_00183fd0:
  if (((*(ulong *)(param_2 + 4) & DAT_002849d8) == 0) &&
     ((*(ulong *)(param_2 + 6) & DAT_002849e0) == 0)) {
    uVar2 = FUN_00194040(0x284000);
    return uVar2;
  }
  DAT_00284688 = 0;
  return 4;
}

/* ===== nexus_menu_register_storage @ 0018416c ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 nexus_menu_register_storage(int *param_1)

{
  ulong uVar1;
  undefined8 uVar2;
  
  if ((((param_1 == (int *)0x0) || (*param_1 != 1)) || (param_1[1] != 0x20)) ||
     ((*(long *)(param_1 + 4) == 0 || (*(long *)(param_1 + 6) == 0)))) {
    uVar2 = 4;
  }
  else {
    uVar1 = FUN_00193f80(1,&DAT_00284688);
    if ((uVar1 & 1) == 0) {
      DAT_00284a90 = *(undefined8 *)(param_1 + 2);
      _DAT_00284a88 = *(undefined8 *)param_1;
      DAT_00284aa0 = *(undefined8 *)(param_1 + 6);
      DAT_00284a98 = *(undefined8 *)(param_1 + 4);
      uVar2 = 1;
      DAT_00284688 = 0;
    }
    else {
      uVar2 = 3;
    }
  }
  return uVar2;
}

/* ===== nexus_menu_setting_value @ 001841f0 ===== */

undefined8 nexus_menu_setting_value(uint param_1,undefined4 *param_2)

{
  undefined8 uVar1;
  ulong uVar2;
  
  uVar1 = 4;
  if ((param_1 < 0x76) && (param_2 != (undefined4 *)0x0)) {
    uVar2 = FUN_00193f80(1,&DAT_00284688);
    if ((uVar2 & 1) == 0) {
      uVar1 = 1;
      *param_2 = *(undefined4 *)((long)&DAT_002846b0 + (ulong)param_1 * 4);
      DAT_00284688 = 0;
    }
    else {
      uVar1 = 3;
    }
  }
  return uVar1;
}

/* ===== nexus_menu_export @ 0018425c ===== */

undefined8 nexus_menu_export(long param_1,ulong param_2,undefined8 *param_3)

{
  undefined8 uVar1;
  ulong uVar2;
  
  uVar1 = 4;
  if (((param_1 != 0) && (0x3c3 < param_2)) && (param_3 != (undefined8 *)0x0)) {
    uVar2 = FUN_00193f80(1,&DAT_00284688);
    if ((uVar2 & 1) == 0) {
      FUN_001842d0(param_1);
      uVar1 = 1;
      *param_3 = 0x3c4;
      DAT_00284688 = 0;
    }
    else {
      uVar1 = 3;
    }
  }
  return uVar1;
}

/* ===== nexus_menu_import @ 001843dc ===== */

void nexus_menu_import(undefined8 param_1,undefined8 param_2,int param_3)

{
  long lVar1;
  undefined8 uVar2;
  ulong uVar3;
  undefined1 auStack_210 [472];
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  uVar2 = FUN_0018447c(param_1,param_2,auStack_210);
  if ((int)uVar2 == 1) {
    uVar3 = FUN_00193f80(uVar2,&DAT_00284688);
    if ((uVar3 & 1) == 0) {
      uVar2 = 4;
      if ((0 < param_3) && (DAT_00284694 == param_3)) {
        uVar2 = FUN_001846b8(auStack_210);
      }
      DAT_00284688 = 0;
    }
    else {
      uVar2 = 3;
    }
  }
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar2);
}

/* ===== FUN_0018447c @ 0018447c ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_0018447c(long *param_1,long param_2,long param_3)

{
  byte *pbVar1;
  uint uVar2;
  long lVar3;
  int iVar4;
  long lVar5;
  uint uVar6;
  int iVar7;
  int *piVar8;
  char *__s1;
  ulong uVar9;
  byte bVar10;
  uint uVar11;
  byte bVar14;
  byte bVar15;
  byte bVar16;
  undefined1 auVar12 [16];
  undefined1 auVar13 [16];
  
  if ((((param_1 != (long *)0x0) && (param_2 == 0x3c4)) && (*param_1 == 0x32353239364d4e)) &&
     ((lVar3 = param_1[1], (int)lVar3 - 1U < 2 && (*(int *)((long)param_1 + 0xc) == 0x76)))) {
    lVar5 = 0;
    uVar6 = 0xffffffff;
    do {
      pbVar1 = (byte *)((long)param_1 + lVar5);
      lVar5 = lVar5 + 1;
      uVar11 = uVar6 ^ *pbVar1;
      uVar2 = -(uVar11 & 1) & 0xedb88320 ^ uVar11 >> 1;
      bVar10 = (byte)uVar11;
      auVar12[0] = bVar10 & (byte)_DAT_0010f9c0;
      bVar14 = (byte)(uVar6 >> 8);
      auVar12[1] = bVar14 & (byte)((ulong)_DAT_0010f9c0 >> 8);
      bVar15 = (byte)(uVar6 >> 0x10);
      auVar12[2] = bVar15 & (byte)((ulong)_DAT_0010f9c0 >> 0x10);
      bVar16 = (byte)(uVar6 >> 0x18);
      auVar12[3] = bVar16 & (byte)((ulong)_DAT_0010f9c0 >> 0x18);
      auVar12[4] = bVar10 & (byte)((ulong)_DAT_0010f9c0 >> 0x20);
      auVar12[5] = bVar14 & (byte)((ulong)_DAT_0010f9c0 >> 0x28);
      auVar12[6] = bVar15 & (byte)((ulong)_DAT_0010f9c0 >> 0x30);
      auVar12[7] = bVar16 & (byte)((ulong)_DAT_0010f9c0 >> 0x38);
      auVar12[8] = bVar10 & (byte)_UNK_0010f9c8;
      auVar12[9] = bVar14 & (byte)((ulong)_UNK_0010f9c8 >> 8);
      auVar12[10] = bVar15 & (byte)((ulong)_UNK_0010f9c8 >> 0x10);
      auVar12[0xb] = bVar16 & (byte)((ulong)_UNK_0010f9c8 >> 0x18);
      auVar12[0xc] = bVar10 & (byte)((ulong)_UNK_0010f9c8 >> 0x20);
      auVar12[0xd] = bVar14 & (byte)((ulong)_UNK_0010f9c8 >> 0x28);
      auVar12[0xe] = bVar15 & (byte)((ulong)_UNK_0010f9c8 >> 0x30);
      auVar12[0xf] = bVar16 & (byte)((ulong)_UNK_0010f9c8 >> 0x38);
      uVar6 = (int)(uVar11 << 0x1e) >> 0x1f & 0xedb88320U ^ uVar2 >> 1;
      auVar12 = NEON_cmeq(auVar12,0,2);
      auVar13[0] = (byte)_DAT_0010fac0 & ~auVar12[0];
      auVar13[1] = (byte)((ulong)_DAT_0010fac0 >> 8) & ~auVar12[1];
      auVar13[2] = (byte)((ulong)_DAT_0010fac0 >> 0x10) & ~auVar12[2];
      auVar13[3] = (byte)((ulong)_DAT_0010fac0 >> 0x18) & ~auVar12[3];
      auVar13[4] = (byte)((ulong)_DAT_0010fac0 >> 0x20) & ~auVar12[4];
      auVar13[5] = (byte)((ulong)_DAT_0010fac0 >> 0x28) & ~auVar12[5];
      auVar13[6] = (byte)((ulong)_DAT_0010fac0 >> 0x30) & ~auVar12[6];
      auVar13[7] = (byte)((ulong)_DAT_0010fac0 >> 0x38) & ~auVar12[7];
      auVar13[8] = (byte)_UNK_0010fac8 & ~auVar12[8];
      auVar13[9] = (byte)((ulong)_UNK_0010fac8 >> 8) & ~auVar12[9];
      auVar13[10] = (byte)((ulong)_UNK_0010fac8 >> 0x10) & ~auVar12[10];
      auVar13[0xb] = (byte)((ulong)_UNK_0010fac8 >> 0x18) & ~auVar12[0xb];
      auVar13[0xc] = (byte)((ulong)_UNK_0010fac8 >> 0x20) & ~auVar12[0xc];
      auVar13[0xd] = (byte)((ulong)_UNK_0010fac8 >> 0x28) & ~auVar12[0xd];
      auVar13[0xe] = (byte)((ulong)_UNK_0010fac8 >> 0x30) & ~auVar12[0xe];
      auVar13[0xf] = (byte)((ulong)_UNK_0010fac8 >> 0x38) & ~auVar12[0xf];
      auVar12 = NEON_ext(auVar13,auVar13,8,1);
      uVar11 = CONCAT13(auVar13[3] ^ auVar12[3],
                        CONCAT12(auVar13[2] ^ auVar12[2],
                                 CONCAT11(auVar13[1] ^ auVar12[1],auVar13[0] ^ auVar12[0])));
      uVar6 = uVar11 ^ (int)(uVar2 << 0x1a) >> 0x1f & 0x76dc4190U ^
              (int)(uVar6 << 0x1a) >> 0x1f & 0xedb88320U ^ uVar6 >> 6 ^
              (uint)(CONCAT17(auVar13[7] ^ auVar12[7],
                              CONCAT16(auVar13[6] ^ auVar12[6],
                                       CONCAT15(auVar13[5] ^ auVar12[5],
                                                CONCAT14(auVar13[4] ^ auVar12[4],uVar11)))) >> 0x20)
      ;
    } while (lVar5 != 0x3c0);
    if (((uint)*(ushort *)(param_1 + 0x78) |
        (uint)*(byte *)((long)param_1 + 0x3c2) << 0x10 |
        (uint)*(byte *)((long)param_1 + 0x3c3) << 0x18) == ~uVar6) {
      uVar9 = 0;
      lVar5 = (long)param_1 + 0x17;
      piVar8 = (int *)(PTR_PTR_s_nexus_sx_spin_001a36f8 + 0xc);
      while (((uVar9 == *(uint *)(lVar5 + -7) && (iVar7 = *(int *)(lVar5 + -3), piVar8[-1] <= iVar7)
              ) && (iVar7 <= *piVar8))) {
        if (((int)lVar3 == 1) && (iVar7 == 100)) {
          __s1 = *(char **)(piVar8 + -3);
          iVar4 = strcmp(__s1,"nexus_dodge_reaction_pct");
          if (((iVar4 == 0) ||
              (((iVar4 = strcmp(__s1,"nexus_v2_dodge_reaction_pct"), iVar4 == 0 ||
                (iVar4 = strcmp(__s1,"nexus_v3_dodge_reaction_pct"), iVar4 == 0)) ||
               (iVar4 = strcmp(__s1,"nexus_v4_dodge_reaction_pct"), iVar4 == 0)))) ||
             (iVar4 = strcmp(__s1,"nexus_v5_dodge_reaction_pct"), iVar4 == 0)) {
            iVar7 = 0xb4;
          }
        }
        *(int *)(param_3 + uVar9 * 4) = iVar7;
        uVar9 = uVar9 + 1;
        piVar8 = piVar8 + 6;
        lVar5 = lVar5 + 8;
        if (uVar9 == 0x76) {
          return 1;
        }
      }
    }
  }
  return 4;
}

/* ===== nexus_menu_set_battle @ 00184858 ===== */

undefined8 nexus_menu_set_battle(int param_1,int param_2)

{
  ulong uVar1;
  undefined8 uVar2;
  
  uVar1 = FUN_00193f80(1,&DAT_00284688);
  if ((uVar1 & 1) == 0) {
    uVar2 = 4;
    if ((0 < param_2) && (DAT_00284694 == param_2)) {
      if ((bool)DAT_0028469e == (param_1 != 0)) {
        uVar2 = 1;
      }
      else {
        uVar2 = 1;
        DAT_0028469f = 0;
        DAT_002846a0 = DAT_002846a0 + 1;
        DAT_0028469e = param_1 != 0;
      }
    }
    DAT_00284688 = 0;
  }
  else {
    uVar2 = 3;
  }
  return uVar2;
}

/* ===== nexus_menu_scroll_battle @ 00184900 ===== */

undefined8 nexus_menu_scroll_battle(int param_1,int param_2)

{
  uint uVar1;
  ulong uVar2;
  undefined8 uVar3;
  int iVar4;
  
  uVar2 = FUN_00193f80(1,&DAT_00284688);
  if ((uVar2 & 1) == 0) {
    uVar3 = 4;
    if (((0 < param_2) && (DAT_00284694 == param_2)) && (DAT_0028469e != '\0')) {
      uVar3 = 1;
      uVar1 = (uint)DAT_0028469f + param_1;
      iVar4 = (int)(*(long *)PTR_DAT_001a3700 + 1U >> 1);
      if (iVar4 < 5) {
        iVar4 = 4;
      }
      if ((int)uVar1 < 1) {
        uVar1 = 0;
      }
      if (iVar4 - 4U <= uVar1) {
        uVar1 = iVar4 - 4U;
      }
      DAT_002846a0 = DAT_002846a0 + 1;
      DAT_0028469f = (byte)uVar1;
    }
    DAT_00284688 = 0;
  }
  else {
    uVar3 = 3;
  }
  return uVar3;
}

/* ===== nexus_menu_action_status @ 00184d4c ===== */

void nexus_menu_action_status(uint param_1,undefined8 *param_2,int param_3)

{
  long lVar1;
  undefined8 uVar2;
  ulong uVar3;
  undefined8 local_60;
  undefined8 uStack_58;
  undefined8 local_50;
  long local_48;
  
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  uVar2 = 4;
  if ((param_1 < 0xa8) && (param_2 != (undefined8 *)0x0)) {
    uVar3 = FUN_00193f80(1,&DAT_00284688);
    if ((uVar3 & 1) == 0) {
      uVar2 = 4;
      if ((0 < param_3) && (DAT_00284694 == param_3)) {
        FUN_00184e28(&local_60,PTR_DAT_001a36f0 + (ulong)param_1 * 0x48);
        uVar2 = 1;
        param_2[1] = uStack_58;
        *param_2 = local_60;
        param_2[2] = local_50;
      }
      DAT_00284688 = 0;
    }
    else {
      uVar2 = 3;
    }
  }
  if (*(long *)(lVar1 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar2);
}

/* ===== nexus_menu_ui_state @ 00185650 ===== */

undefined8 nexus_menu_ui_state(undefined4 *param_1,int param_2)

{
  undefined1 uVar1;
  undefined1 uVar2;
  undefined1 uVar3;
  undefined1 uVar4;
  ulong uVar5;
  undefined8 uVar6;
  
  if (param_1 == (undefined4 *)0x0) {
    uVar6 = 4;
  }
  else {
    uVar5 = FUN_00193f80(1,&DAT_00284688);
    uVar4 = DAT_0028469f;
    uVar3 = DAT_0028469e;
    uVar2 = DAT_0028469d;
    uVar1 = DAT_0028469c;
    if ((uVar5 & 1) == 0) {
      uVar6 = 4;
      if ((0 < param_2) && (DAT_00284694 == param_2)) {
        uVar6 = 1;
        *param_1 = DAT_002846a0;
        *(undefined1 *)(param_1 + 1) = uVar1;
        *(undefined1 *)((long)param_1 + 5) = uVar3;
        *(undefined1 *)((long)param_1 + 6) = uVar2;
        *(undefined1 *)((long)param_1 + 7) = uVar4;
      }
      DAT_00284688 = 0;
    }
    else {
      uVar6 = 3;
    }
  }
  return uVar6;
}

/* ===== nexus_menu_view @ 001856f8 ===== */

/* WARNING: Removing unreachable block (ram,0x00185a54) */

void nexus_menu_view(undefined4 *param_1,int param_2)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  ulong uVar3;
  uint uVar4;
  uint uVar5;
  byte bVar6;
  ushort uVar7;
  long lVar8;
  bool bVar9;
  int iVar10;
  int iVar11;
  ulong uVar12;
  undefined8 uVar13;
  uint uVar14;
  char cVar15;
  uint uVar16;
  undefined *puVar17;
  long lVar18;
  ulong uVar19;
  undefined8 *puVar20;
  long lVar21;
  ushort *puVar22;
  char *pcVar23;
  undefined4 uVar24;
  undefined4 uVar25;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined8 local_70;
  long local_68;
  
  lVar8 = tpidr_el0;
  local_68 = *(long *)(lVar8 + 0x28);
  if (param_1 == (undefined4 *)0x0) {
    uVar13 = 4;
  }
  else {
    uVar12 = FUN_00193f80(1,&DAT_00284688);
    if ((uVar12 & 1) == 0) {
      uVar13 = 4;
      if ((0 < param_2) && (DAT_00284694 == param_2)) {
        FUN_00185d10(4);
        memset(param_1,0,0x690);
        *param_1 = DAT_002846a0;
        *(undefined1 *)((long)param_1 + 9) = DAT_0028469c;
        *(char *)(param_1 + 2) = DAT_0028469d;
        cVar15 = DAT_0028469e;
        *(char *)((long)param_1 + 10) = DAT_0028469e;
        uVar12 = (ulong)DAT_0028469f;
        *(byte *)((long)param_1 + 0xb) = DAT_0028469f;
        if (cVar15 != '\0') {
          uVar19 = 0;
          puVar20 = (undefined8 *)(PTR_DAT_001a3708 + uVar12 * 0x20 + 8);
          uVar3 = 0;
          if (uVar12 * 2 <= *(ulong *)PTR_DAT_001a3700) {
            uVar3 = *(ulong *)PTR_DAT_001a3700 + uVar12 * -2;
          }
          do {
            if (uVar3 == uVar19) break;
            uVar4 = param_1[1];
            uVar7 = *(ushort *)((long)puVar20 + -6);
            uVar24 = 0xc3dc0000;
            if ((uVar19 & 1) != 0) {
              uVar24 = 0xc2b40000;
            }
            param_1[1] = uVar4 + 1;
            puVar17 = PTR_DAT_001a36f0;
            uVar16 = (uint)uVar7;
            param_1[(ulong)uVar4 * 0x10 + 4] = uVar16;
            uVar25 = NEON_fmadd((float)(uVar19 >> 1 & 0x7fffffff),0x42900000,0x43320000);
            *(undefined8 *)(param_1 + (ulong)uVar4 * 0x10 + 6) = *puVar20;
            uVar13 = *(undefined8 *)(puVar17 + (ulong)uVar16 * 0x48 + 0x18);
            param_1[(ulong)uVar4 * 0x10 + 10] = uVar24;
            param_1[(ulong)uVar4 * 0x10 + 0xb] = uVar25;
            *(undefined8 *)(param_1 + (ulong)uVar4 * 0x10 + 8) = uVar13;
            *(undefined *)(param_1 + (ulong)uVar4 * 0x10 + 0x12) =
                 puVar17[(ulong)uVar16 * 0x48 + 0x44];
            FUN_00184e28(&local_80);
            uVar19 = uVar19 + 1;
            *(undefined8 *)(param_1 + (ulong)uVar4 * 0x10 + 0xe) = uStack_78;
            *(undefined8 *)(param_1 + (ulong)uVar4 * 0x10 + 0xc) = local_80;
            *(undefined8 *)(param_1 + (ulong)uVar4 * 0x10 + 0x10) = local_70;
            puVar20 = puVar20 + 2;
          } while (uVar19 != 8);
          uVar13 = DAT_0010f8b8;
          uVar4 = param_1[1];
          puVar1 = param_1 + (ulong)uVar4 * 0x10 + 4;
          *puVar1 = 0x20000;
          *(undefined **)(puVar1 + 2) = &DAT_00133357;
          *(undefined **)(puVar1 + 4) = &DAT_00134f22;
          *(undefined8 *)(puVar1 + 6) = uVar13;
          puVar2 = param_1 + (ulong)(uVar4 + 1) * 0x10 + 4;
          *(undefined1 *)(puVar1 + 0xd) = 1;
          uVar13 = DAT_0010f7b8;
          *puVar2 = 0x20001;
          *(undefined **)(puVar2 + 2) = &DAT_0013c999;
          *(undefined **)(puVar2 + 4) = &DAT_00134f22;
          *(undefined8 *)(puVar2 + 6) = uVar13;
          *(undefined1 *)(puVar2 + 0xd) = 1;
          param_1[1] = uVar4 + 2;
        }
        lVar18 = 0;
        puVar20 = (undefined8 *)PTR_PTR_s_nexus_sx_spin_001a36f8;
        do {
          iVar10 = strcmp((char *)*puVar20,"nexus_quick_menu_preset");
          if (iVar10 == 0) goto LAB_00185934;
          lVar18 = lVar18 + 1;
          puVar20 = puVar20 + 3;
        } while (lVar18 != 0x76);
        lVar18 = -1;
LAB_00185934:
        lVar21 = 0;
        iVar10 = *(int *)((long)&DAT_002846b0 + lVar18 * 4);
        puVar20 = (undefined8 *)PTR_PTR_s_nexus_sx_spin_001a36f8;
        do {
          iVar11 = strcmp((char *)*puVar20,"nexus_quick_menu_disabled");
          if (iVar11 == 0) goto LAB_00185974;
          lVar21 = lVar21 + 1;
          puVar20 = puVar20 + 3;
        } while (lVar21 != 0x76);
        lVar21 = -1;
LAB_00185974:
        puVar17 = PTR_DAT_001a36f0;
        if ((DAT_0028469e == '\0') && (uVar12 = *(ulong *)PTR_DAT_001a3710, uVar12 != 0)) {
          uVar4 = iVar10 - 1;
          lVar18 = (ulong)uVar4 * 0x20;
          iVar10 = *(int *)((long)&DAT_002846b0 + lVar21 * 4);
          uVar19 = 1;
          pcVar23 = PTR_DAT_001a3718;
          cVar15 = DAT_0028469d;
          do {
            if (*pcVar23 == cVar15) {
              bVar6 = pcVar23[1];
              uVar16 = (uint)bVar6;
              if (bVar6 < 0xf) {
                uVar5 = param_1[1];
                if (uVar5 < 0xf) {
                  uVar7 = *(ushort *)(pcVar23 + 2);
                  if (cVar15 == 'R') {
                    if (iVar10 != 0) {
LAB_00185a48:
                      cVar15 = 'R';
                      goto LAB_00185a00;
                    }
                    if (uVar4 < 3) {
                      uVar14 = (uint)bVar6;
                      if (*(uint *)(&DAT_001406b8 + lVar18) == uVar14) {
                        uVar16 = 0;
                      }
                      else if (*(uint *)(&DAT_001406bc + lVar18) == uVar14) {
                        uVar16 = 1;
                      }
                      else if (*(uint *)(&DAT_001406c0 + lVar18) == uVar14) {
                        uVar16 = 2;
                      }
                      else if (*(uint *)(&DAT_001406c4 + lVar18) == uVar14) {
                        uVar16 = 3;
                      }
                      else if (*(uint *)(&DAT_001406c8 + lVar18) == uVar16) {
                        uVar16 = 4;
                      }
                      else if (*(uint *)(&DAT_001406cc + lVar18) == uVar16) {
                        uVar16 = 5;
                      }
                      else if (*(uint *)(&DAT_001406d0 + lVar18) == uVar16) {
                        uVar16 = 6;
                      }
                      else {
                        if (*(uint *)(&DAT_001406d4 + lVar18) != uVar16) goto LAB_00185a48;
                        uVar16 = 7;
                      }
                    }
                  }
                  param_1[1] = uVar5 + 1;
                  uVar14 = (uint)uVar7;
                  param_1[(ulong)uVar5 * 0x10 + 4] = uVar14;
                  *(undefined8 *)(param_1 + (ulong)uVar5 * 0x10 + 6) = *(undefined8 *)(pcVar23 + 8);
                  *(undefined8 *)(param_1 + (ulong)uVar5 * 0x10 + 8) =
                       *(undefined8 *)(puVar17 + (ulong)uVar14 * 0x48 + 0x18);
                  param_1[(ulong)uVar5 * 0x10 + 10] = (&DAT_00140698)[uVar16 % 5];
                  param_1[(ulong)uVar5 * 0x10 + 0xb] = (&DAT_001406ac)[uVar16 / 5];
                  *(undefined *)(param_1 + (ulong)uVar5 * 0x10 + 0x12) =
                       puVar17[(ulong)uVar14 * 0x48 + 0x44];
                  FUN_00184e28(&local_80,puVar17 + (ulong)uVar7 * 0x48);
                  puVar17 = PTR_DAT_001a36f0;
                  *(undefined8 *)(param_1 + (ulong)uVar5 * 0x10 + 0x10) = local_70;
                  *(undefined8 *)(param_1 + (ulong)uVar5 * 0x10 + 0xe) = uStack_78;
                  *(undefined8 *)(param_1 + (ulong)uVar5 * 0x10 + 0xc) = local_80;
                  cVar15 = DAT_0028469d;
                  if (DAT_0028469e != '\0') break;
                }
              }
            }
LAB_00185a00:
            bVar9 = uVar19 < uVar12;
            pcVar23 = pcVar23 + 0x10;
            uVar19 = uVar19 + 1;
          } while (bVar9);
        }
        uVar12 = 0;
        puVar22 = (ushort *)PTR_DAT_001a3720;
        do {
          uVar4 = param_1[1];
          param_1[1] = uVar4 + 1;
          uVar24 = NEON_fmadd((float)(uVar12 & 0xffffffff),0x43120000,0xc3e40000);
          uVar7 = *puVar22;
          param_1[(ulong)uVar4 * 0x10 + 4] = (uint)uVar7;
          uVar13 = *(undefined8 *)(puVar22 + 4);
          param_1[(ulong)uVar4 * 0x10 + 10] = uVar24;
          param_1[(ulong)uVar4 * 0x10 + 0xb] = 0x44010000;
          *(undefined1 *)(param_1 + (ulong)uVar4 * 0x10 + 0x12) = 1;
          *(undefined8 *)(param_1 + (ulong)uVar4 * 0x10 + 6) = uVar13;
          *(undefined **)(param_1 + (ulong)uVar4 * 0x10 + 8) = &DAT_00134f22;
          *(bool *)((long)param_1 + (ulong)uVar4 * 0x40 + 0x49) = DAT_0028469d == (char)puVar22[1];
          FUN_00184e28(&local_80,puVar17 + (ulong)(uint)uVar7 * 0x48);
          puVar17 = PTR_DAT_001a36f0;
          uVar12 = uVar12 + 1;
          puVar22 = puVar22 + 8;
          *(undefined8 *)(param_1 + (ulong)uVar4 * 0x10 + 0xe) = uStack_78;
          *(undefined8 *)(param_1 + (ulong)uVar4 * 0x10 + 0xc) = local_80;
          *(undefined8 *)(param_1 + (ulong)uVar4 * 0x10 + 0x10) = local_70;
          uVar13 = DAT_0010f788;
        } while (uVar12 != 6);
        uVar4 = param_1[1];
        param_1[1] = uVar4 + 1;
        *(undefined **)(param_1 + (ulong)uVar4 * 0x10 + 6) = &DAT_00136344;
        *(undefined **)(param_1 + (ulong)uVar4 * 0x10 + 8) = &DAT_00134f22;
        param_1[(ulong)uVar4 * 0x10 + 4] = 0x82;
        *(undefined8 *)(param_1 + (ulong)uVar4 * 0x10 + 10) = uVar13;
        FUN_00184e28(&local_80,puVar17 + 0x2490);
        uVar13 = 1;
        *(undefined8 *)(param_1 + (ulong)uVar4 * 0x10 + 0xe) = uStack_78;
        *(undefined8 *)(param_1 + (ulong)uVar4 * 0x10 + 0xc) = local_80;
        *(undefined8 *)(param_1 + (ulong)uVar4 * 0x10 + 0x10) = local_70;
      }
      DAT_00284688 = 0;
    }
    else {
      uVar13 = 3;
    }
  }
  if (*(long *)(lVar8 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar13);
}

/* ===== nexus_menu_server_thread @ 00185e94 ===== */

bool nexus_menu_server_thread(int param_1)

{
  return 0 < param_1 && DAT_00284694 == param_1;
}

/* ===== nexus_menu_server_open @ 00185eac ===== */

undefined8 nexus_menu_server_open(int param_1)

{
  ulong uVar1;
  
  uVar1 = FUN_00193f80(1,&DAT_00284688);
  if ((uVar1 & 1) != 0) {
    return 3;
  }
  if ((((0 < param_1) && (DAT_00284694 == param_1)) && (DAT_0028469e == '\0')) &&
     (DAT_0028469c != '\0')) {
    if (DAT_0028469d != 0x51) {
      DAT_0022d820 = (uint)DAT_0028469d;
    }
    DAT_0028469f = 0;
    DAT_002846a0 = DAT_002846a0 + 1;
    DAT_0028469d = 0x51;
    DAT_00284688 = 0;
    FUN_00191920(&DAT_00284af0,0x918);
    if (DAT_00284af4 < 0x21) {
      if (DAT_00284af4 != 0) {
        return 1;
      }
    }
    else {
      DAT_00284af4 = 0;
    }
    FUN_00191bb0(0xfffffffe);
    return 1;
  }
  DAT_00284688 = 0;
  return 4;
}

/* ===== nexus_menu_server_action @ 00185f90 ===== */

undefined4 nexus_menu_server_action(uint param_1,int param_2)

{
  undefined4 uVar1;
  ulong uVar2;
  
  if (param_1 - 0x24030 < 0xffffffd0) {
    return 4;
  }
  uVar2 = FUN_00193f80(1,&DAT_00284688);
  if ((uVar2 & 1) != 0) {
    return 3;
  }
  if (param_2 < 1) {
    DAT_00284688 = 0;
    return 4;
  }
  if (DAT_00284694 != param_2) {
    DAT_00284688 = 0;
    return 4;
  }
  if (DAT_0028469d != 'Q') {
    DAT_00284688 = 0;
    return 4;
  }
  if (DAT_0028469e != '\0') {
    DAT_00284688 = 0;
    return 4;
  }
  if (DAT_0028469c == '\0') {
    DAT_00284688 = 0;
    return 4;
  }
  if (param_1 == 0x24003) {
    uVar1 = 0xfffffffe;
  }
  else if (param_1 == 0x24002) {
    uVar1 = 0xffffffff;
  }
  else {
    if (param_1 == 0x24001) {
      uVar1 = 1;
      DAT_0028469f = 0;
      DAT_0028469d = (char)DAT_0022d820;
      goto LAB_001860dc;
    }
    if (param_1 < 0x24010) {
      DAT_00284688 = 0;
      return 4;
    }
    if (DAT_00284af4 <= param_1 - 0x24010) {
      DAT_00284688 = 0;
      return 4;
    }
    uVar1 = (&DAT_00284b08)[(ulong)(param_1 - 0x24010) * 0x12];
  }
  DAT_00284688 = 0;
  uVar1 = FUN_00191bb0(uVar1);
  uVar2 = FUN_00193f80(1,&DAT_00284688);
  if ((uVar2 & 1) != 0) {
    return uVar1;
  }
LAB_001860dc:
  DAT_002846a0 = DAT_002846a0 + 1;
  DAT_00284688 = 0;
  return uVar1;
}

/* ===== nexus_menu_theme_action @ 00186548 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 nexus_menu_theme_action(uint param_1,int param_2)

{
  undefined4 uVar1;
  int iVar2;
  ulong uVar3;
  undefined8 uVar4;
  uint uVar5;
  
  if (param_1 - 0x26040 < 0xffffffc0) {
    return 4;
  }
  uVar3 = FUN_00193f80(1,&DAT_00284688);
  if ((uVar3 & 1) != 0) {
    return 3;
  }
  if (param_2 < 1) {
    DAT_00284688 = 0;
    return 4;
  }
  if (DAT_00284694 != param_2) {
    DAT_00284688 = 0;
    return 4;
  }
  if (DAT_0028469c == '\0') {
    DAT_00284688 = 0;
    return 4;
  }
  if (DAT_0028469e != '\0') {
    DAT_00284688 = 0;
    return 4;
  }
  if (DAT_0028469d != 't') {
    DAT_00284688 = 0;
    return 4;
  }
  if (param_1 != 0x26001) {
    if (DAT_00287ba4 != 0) {
      DAT_00284688 = 0;
      return 2;
    }
    if (DAT_00287b98 == 0) {
      DAT_00284688 = 0;
      return 4;
    }
    if (DAT_00285410 == 0) {
      DAT_00284688 = 0;
      return 4;
    }
    if ((param_1 == 0x26004) && (DAT_00287b90 == 0)) {
      DAT_00287b90 = 3;
      FUN_00194010(1,&DAT_0022d824);
      DAT_00284688 = 0;
      DAT_0028469f = 0;
      DAT_002846a0 = DAT_002846a0 + 1;
      DAT_00287bac = DAT_00287bac + 1;
      DAT_00287bb4 = 0;
      return 1;
    }
    if (((param_1 & 0xfffffffe) == 0x26002) && (DAT_00287b90 < 2)) {
      if (param_1 == 0x26002) {
        if (DAT_00287b94 == 0) {
          DAT_00284688 = 0;
          return 4;
        }
        uVar5 = 0;
        if (7 < DAT_00287b94) {
          uVar5 = DAT_00287b94 - 8;
        }
      }
      else {
        uVar5 = DAT_00287b94 + 8;
        if (DAT_00285418 <= DAT_00287b94 + 8) {
          DAT_00284688 = 0;
          return 4;
        }
      }
      DAT_00287b98 = 0;
      DAT_00287ba4 = 1;
      DAT_00287c50 = DAT_00287c48;
      DAT_00287b94 = uVar5;
      FUN_00194010(1,&DAT_0022d824);
      DAT_00284688 = 0;
      DAT_0028469f = 0;
      DAT_002846a0 = DAT_002846a0 + 1;
      DAT_00287bac = DAT_00287bac + 1;
      DAT_00287bb4 = 0;
      return 1;
    }
    if ((param_1 < 0x26020) || (1 < DAT_00287b90)) {
      if ((param_1 != 0x26006) || ((DAT_00287b90 != 1 || (DAT_00287b9c == 0)))) {
        if ((param_1 == 0x26007) &&
           ((((DAT_00287b90 == 2 && (DAT_00287c68 == 1)) && (DAT_00287b9c != 0)) &&
            ((DAT_00287ba0 != 0 && ((DAT_00285424 & 1) != 0)))))) {
          uVar4 = 1;
          uVar1 = DAT_00287a64;
          uVar5 = DAT_0028793c;
        }
        else {
          if ((param_1 == 0x26005) && ((DAT_00285424 & 1) != 0)) {
            FUN_00186c90(2,0,0);
            DAT_00284688 = 0;
            return 2;
          }
          if (DAT_00287b90 != 3) {
            DAT_00284688 = 0;
            return 4;
          }
          if (param_1 == 0x26008) {
            uVar5 = 1;
          }
          else if (param_1 == 0x26009) {
            uVar5 = 4;
          }
          else {
            uVar5 = (uint)(param_1 == 0x2600a) << 1;
          }
          if ((uVar5 == 0) || ((DAT_00285424 >> 1 & 1) == 0)) {
            if ((param_1 - 0x2600d < 0xfffffffe) || ((DAT_00285424 & 1) == 0)) {
              if ((param_1 == 0x2600d) && ((DAT_00285424 >> 2 & 1) != 0)) {
                uVar4 = 4;
                iVar2 = DAT_00285434;
              }
              else {
                if (param_1 != 0x2600e) {
                  DAT_00284688 = 0;
                  return 4;
                }
                if ((DAT_00285424 >> 3 & 1) == 0) {
                  DAT_00284688 = 0;
                  return 4;
                }
                uVar4 = 5;
                iVar2 = DAT_00285438;
              }
              uVar5 = (uint)(iVar2 == 0);
            }
            else {
              uVar4 = 6;
              uVar5 = 1;
              if (param_1 == 0x2600b) {
                uVar5 = 0xffffffff;
              }
            }
          }
          else {
            uVar4 = 3;
            uVar5 = DAT_00285430 ^ uVar5;
          }
          uVar1 = 0;
        }
        FUN_00186c90(uVar4,uVar5,uVar1);
        DAT_00284688 = 0;
        return 2;
      }
      if (DAT_00285414 != DAT_00287c40) goto LAB_001869e8;
      DAT_00287b88 = 0;
      uRam0000000000287b80 = 0;
      _DAT_00287b78 = 0;
      uRam0000000000287b60 = 0;
      _DAT_00287b58 = 0;
      uRam0000000000287b50 = 0;
      _DAT_00287b48 = 0;
      uRam0000000000287b70 = 0;
      _DAT_00287b68 = 0;
      uRam0000000000287b40 = 0;
      _DAT_00287b38 = 0;
      uRam0000000000287b30 = 0;
      _DAT_00287b28 = 0;
      uRam0000000000287b20 = 0;
      _DAT_00287b18 = 0;
      uRam0000000000287b10 = 0;
      _DAT_00287b08 = 0;
      uRam0000000000287b00 = 0;
      _DAT_00287af8 = 0;
      uRam0000000000287af0 = 0;
      _DAT_00287ae8 = 0;
      uRam0000000000287ae0 = 0;
      _DAT_00287ad8 = 0;
      uRam0000000000287ad0 = 0;
      _DAT_00287ac8 = 0;
      uRam0000000000287ac0 = 0;
      _DAT_00287ab8 = 0;
      uRam0000000000287ab0 = 0;
      _DAT_00287aa8 = 0;
      uRam0000000000287aa0 = 0;
      _DAT_00287a98 = 0;
      uRam0000000000287a90 = 0;
      _DAT_00287a88 = 0;
      uRam0000000000287a80 = 0;
      _DAT_00287a78 = 0;
      uRam0000000000287a70 = 0;
      _DAT_00287a68 = 0;
      DAT_00287a64 = -2;
      FUN_00183ac8(&DAT_00287a6c,0x60,0x60,"NO MUSIC");
    }
    else {
      param_1 = param_1 - 0x26020;
      if (DAT_00285420 <= param_1) {
        DAT_00284688 = 0;
        return 4;
      }
      if (DAT_00287bb0 != DAT_00287bac) {
        DAT_00284688 = 0;
        return 4;
      }
      if ((DAT_00287bb4 >> (ulong)(param_1 & 0x1f) & 1) == 0) {
        DAT_00284688 = 0;
        return 4;
      }
      uVar3 = (ulong)param_1;
      iVar2 = (&DAT_00287bc0)[param_1];
      if (iVar2 != (&DAT_0028543c)[uVar3 * 0x4a]) {
        DAT_00284688 = 0;
        return 4;
      }
      uVar5 = (&DAT_00285440)[uVar3 * 0x4a];
      if (DAT_00287b90 == 0) {
        if ((uVar5 >> 1 & 1) != 0) {
          DAT_0028793c = iVar2;
          DAT_00287940 = uVar5;
          memmove(&DAT_00287944,&DAT_00285444 + uVar3 * 0x128,0x120);
          DAT_00287c40 = DAT_00285414;
          DAT_00287b9c = (int)DAT_0010f860;
          DAT_00287ba0 = (int)((ulong)DAT_0010f860 >> 0x20);
          DAT_00287b90 = 1;
          FUN_00183ac8(&DAT_00287c6c,0x60,0x60,&DAT_001343dc,"CHOOSE MUSIC FOR THIS BACKGROUND");
          DAT_00287ba4 = 1;
          DAT_00287b94 = 0;
          DAT_00287b98 = 0;
          DAT_00287c50 = DAT_00287c48;
          FUN_00194010(1,&DAT_0022d824);
          DAT_00284688 = 0;
          DAT_0028469f = 0;
          DAT_002846a0 = DAT_002846a0 + 1;
          DAT_00287bac = DAT_00287bac + 1;
          DAT_00287bb4 = 0;
          return 1;
        }
        DAT_00284688 = 0;
        return 4;
      }
      if ((DAT_00287b9c == 0) || (DAT_00285414 != DAT_00287c40)) {
LAB_001869e8:
        FUN_00183ac8(&DAT_00287c6c,0x60,0x60,&DAT_001343dc,
                     "SETTINGS CHANGED - SELECT BACKGROUND AGAIN");
        DAT_00284688 = 0;
        DAT_0028469f = 0;
        DAT_002846a0 = DAT_002846a0 + 1;
        DAT_00287bac = DAT_00287bac + 1;
        DAT_00287bb4 = 0;
        return 4;
      }
      _DAT_00287a68 = CONCAT44(_DAT_00287a6c,uVar5);
      DAT_00287a64 = iVar2;
      memmove(&DAT_00287a6c,&DAT_00285444 + uVar3 * 0x128,0x120);
    }
    DAT_00287ba0 = 1;
    FUN_00186bf4();
    DAT_00284688 = 0;
    return 1;
  }
  DAT_00287ba4 = 0;
  DAT_00287c6c = 0;
  DAT_00287c48 = DAT_00287c48 + 1;
  if (DAT_00287b90 == 2) {
    DAT_00287ba4 = 1;
    DAT_00287b90 = (uint)DAT_0010f860;
    DAT_00287b94 = (uint)((ulong)DAT_0010f860 >> 0x20);
  }
  else {
    if (DAT_00287b90 == 0) {
      DAT_0028469d = (char)DAT_00287b8c;
      goto LAB_0018670c;
    }
    DAT_00287b90 = 0;
    DAT_00287b94 = 0;
    DAT_00287b9c = 0;
    DAT_00287ba0 = (int)DAT_0010f740;
    DAT_00287ba4 = (int)((ulong)DAT_0010f740 >> 0x20);
  }
  DAT_00287b98 = 0;
  DAT_00287c50 = DAT_00287c48;
  FUN_00194010(1,&DAT_0022d824);
LAB_0018670c:
  DAT_00287bb4 = 0;
  DAT_00287bac = DAT_00287bac + 1;
  DAT_002846a0 = DAT_002846a0 + 1;
  DAT_0028469f = 0;
  DAT_00284688 = 0;
  return 1;
}

/* ===== nexus_menu_profile_open @ 00187e48 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 nexus_menu_profile_open(int param_1)

{
  ulong uVar1;
  undefined8 uVar2;
  
  uVar1 = FUN_00193f80(1,&DAT_00284688);
  if ((uVar1 & 1) == 0) {
    uVar2 = 4;
    if ((((0 < param_1) && (DAT_00284694 == param_1)) && (DAT_0028469c != '\0')) &&
       (DAT_0028469e == '\0')) {
      if (DAT_0028469d != 0x70) {
        DAT_0028c288 = (uint)DAT_0028469d;
      }
      uVar2 = 1;
      _DAT_0028c28c = 0;
      DAT_0028c2a0 = 0;
      DAT_0028c2c8 = 0;
      DAT_0028c298 = DAT_0028c298 + 1;
      DAT_0028469f = 0;
      DAT_002846a0 = DAT_002846a0 + 1;
      DAT_0028469d = 0x70;
    }
    DAT_00284688 = 0;
  }
  else {
    uVar2 = 3;
  }
  return uVar2;
}

/* ===== nexus_menu_profile_action @ 00187f14 ===== */

undefined8 nexus_menu_profile_action(int param_1,int param_2)

{
  char *pcVar1;
  bool bVar2;
  ulong uVar3;
  undefined8 uVar4;
  uint uVar5;
  
  if (param_1 - 0x28006U < 0xfffffffa) {
    return 4;
  }
  uVar3 = FUN_00193f80(1,&DAT_00284688);
  if ((uVar3 & 1) != 0) {
    return 3;
  }
  if (param_2 < 1) {
    DAT_00284688 = 0;
    return 4;
  }
  if (DAT_00284694 != param_2) {
    DAT_00284688 = 0;
    return 4;
  }
  if (DAT_0028469c == '\0') {
    DAT_00284688 = 0;
    return 4;
  }
  if (DAT_0028469e != '\0') {
    DAT_00284688 = 0;
    return 4;
  }
  if (DAT_0028469d != 'p') {
    DAT_00284688 = 0;
    return 4;
  }
  if (param_1 == 0x28000) {
    uVar4 = 1;
    DAT_0028c290 = 0;
    DAT_0028469f = 0;
    DAT_0028c298 = DAT_0028c298 + 1;
    DAT_0028469d = (char)DAT_0028c288;
  }
  else {
    if (DAT_0028c290 != 0) {
      DAT_00284688 = 0;
      return 2;
    }
    if (DAT_0028c28c == 0) {
      DAT_00284688 = 0;
      return 0;
    }
    if (DAT_0028a4c0 == 0) {
      DAT_00284688 = 0;
      return 0;
    }
    uVar5 = 8;
    if (param_1 != 0x28004) {
      uVar5 = 4;
    }
    if (param_1 == 0x28001) {
      uVar5 = 1;
    }
    if (((DAT_0028a4c4 & uVar5) == 0) || ((param_1 == 0x28005 && (DAT_0028a4d4 == 0)))) {
      DAT_00284688 = 0;
      return 0;
    }
    bVar2 = param_1 - 0x28001U < 2;
    DAT_0028c290 = 3;
    if (bVar2) {
      DAT_0028c290 = 1;
    }
    pcVar1 = "ENTER A VALUE";
    if (!bVar2) {
      pcVar1 = "APPLYING...";
    }
    DAT_0028c294 = param_1;
    FUN_00183ac8(&DAT_0028c2c8,0x60,0x60,&DAT_001343dc,pcVar1);
    uVar4 = 2;
  }
  DAT_002846a0 = DAT_002846a0 + 1;
  DAT_00284688 = 0;
  return uVar4;
}

/* ===== nexus_menu_profile_pump @ 001880e0 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void nexus_menu_profile_pump(int param_1,ulong param_2)

{
  undefined *puVar1;
  char *pcVar2;
  long lVar3;
  uint uVar4;
  long lVar5;
  bool bVar6;
  undefined1 uVar7;
  int iVar8;
  uint uVar9;
  uint uVar10;
  int iVar11;
  uint uVar12;
  ulong uVar13;
  void *pvVar14;
  undefined8 uVar15;
  char *pcVar16;
  uint uVar17;
  undefined4 uVar18;
  uint uVar19;
  long lVar20;
  uint uVar21;
  undefined **ppuVar22;
  int local_2274;
  undefined8 local_2270;
  undefined8 uStack_2268;
  undefined8 local_2260;
  undefined1 auStack_224c [1028];
  undefined8 local_1e48;
  uint local_1e40;
  uint local_1e3c;
  uint local_1e2c;
  int local_1e28;
  uint local_1e24;
  uint local_1e20;
  undefined1 auStack_1e00 [16];
  undefined1 auStack_1df0 [16];
  undefined1 auStack_1de0 [593];
  undefined auStack_1b8f [6935];
  long local_78;
  
  lVar3 = tpidr_el0;
  local_78 = *(long *)(lVar3 + 0x28);
  uVar13 = FUN_00193f80(1,&DAT_00284688);
  uVar12 = DAT_0028c2a8;
  lVar5 = DAT_0028c298;
  uVar4 = DAT_0028c294;
  if ((uVar13 & 1) != 0) {
LAB_00188608:
    uVar7 = 3;
    goto LAB_0018860c;
  }
  if ((param_1 < 1) || (DAT_00284694 != param_1)) {
    uVar7 = 4;
    DAT_00284688 = 0;
    goto LAB_0018860c;
  }
  if (((DAT_0028469c == '\0') || (DAT_0028469e != '\0')) || (DAT_0028469d != 'p')) {
    DAT_0028c2a8 = 0;
    _DAT_0028c28c = 0;
    DAT_0028c298 = DAT_0028c298 + 1;
    DAT_00284688 = 0;
    if (uVar12 == 0) {
      uVar7 = 1;
      goto LAB_0018860c;
    }
  }
  else {
    iVar11 = DAT_0028c290;
    if ((DAT_0028c2a8 == 0) || (DAT_0028c290 != 0)) {
      if (DAT_0028c290 == 4) {
        uVar7 = 3;
        DAT_00284688 = 0;
        goto LAB_0018860c;
      }
      if (((DAT_0028c290 == 0) && (DAT_0028c2a0 != 0)) &&
         ((DAT_0028c2a0 <= param_2 && (param_2 - DAT_0028c2a0 < 500)))) {
        uVar7 = 1;
        DAT_00284688 = 0;
        goto LAB_0018860c;
      }
      _DAT_0028c28c = CONCAT44(4,DAT_0028c28c);
      DAT_00284688 = 0;
      local_1e48 = DAT_0010f800;
      memset(&local_1e40,0,0x1dc8);
      iVar8 = FUN_00191f04(&local_1e48,0x1dd0,0);
      uVar21 = 0;
      if ((((iVar8 != 0) && ((int)local_1e48 == 1)) && (local_1e48._4_4_ == 0x1dd0)) &&
         (local_1e28 == 0)) {
        if (local_1e24 < 0x21) {
          uVar17 = 0;
          uVar21 = 0;
          if (((local_1e24 <= local_1e20) && (uVar21 = uVar17, local_1e20 < 0x201)) &&
             ((local_1e40 < 2 && (local_1e2c < 2)))) {
            pvVar14 = memchr(auStack_1b8f,0,0x191);
            if (((pvVar14 == (void *)0x0) ||
                (pvVar14 = memchr(auStack_1df0,0,0x10), pvVar14 == (void *)0x0)) ||
               (pvVar14 = memchr(auStack_1e00,0,0x10), pvVar14 == (void *)0x0)) goto LAB_001882b0;
            pvVar14 = memchr(auStack_1de0,0,0x191);
            uVar21 = (uint)(pvVar14 != (void *)0x0);
          }
        }
        else {
LAB_001882b0:
          uVar21 = 0;
        }
      }
      memset(auStack_224c,0,0x401);
      local_2260 = 0;
      uStack_2268 = _UNK_00140720;
      local_2270 = _DAT_00140718;
      FUN_00192340("ShowSkinNamesInProfile",&local_2270);
      if (iVar11 == 0) {
LAB_001883bc:
        uVar17 = 0;
        uVar9 = 0;
        uVar10 = 1;
        uVar19 = 0;
      }
      else {
        uVar10 = 0;
        uVar17 = 1;
        if (uVar21 == 0) {
          uVar9 = 0;
          uVar19 = uVar10;
        }
        else {
          uVar9 = 0;
          uVar19 = 0;
          if (local_1e40 != 0) {
            uVar17 = 8;
            if (uVar4 != 0x28004) {
              uVar17 = 4;
            }
            if (uVar4 == 0x28001) {
              uVar17 = 1;
            }
            if ((local_1e3c & uVar17) == 0) {
              uVar10 = 0;
              uVar9 = 0;
              uVar17 = 1;
              uVar19 = 0;
            }
            else if (iVar11 == 3) {
              uVar9 = 0;
              uVar10 = 1;
              uVar17 = 1;
              uVar19 = 1;
            }
            else if (iVar11 == 2) {
              local_2274 = 3;
              uVar10 = FUN_0014c8ac(uVar12,&local_2274,auStack_224c,0x401);
              uVar19 = 0;
              uVar17 = 1;
              if ((uVar10 != 0) && (local_2274 != 3)) {
                if (local_2274 == 2) {
                  uVar19 = 0;
                  uVar10 = 2;
                }
                else if (local_2274 == 1) {
                  uVar19 = 1;
                }
                else {
                  uVar17 = 0;
                  uVar19 = 0;
                }
              }
              uVar9 = 0;
            }
            else {
              if (iVar11 != 1) goto LAB_001883bc;
              uVar9 = FUN_0014c510();
              uVar10 = uVar9;
              if (uVar9 != 0) {
                bVar6 = uVar4 == 0x28001;
                puVar1 = &DAT_00134f22;
                pcVar16 = "PLAYER TAG";
                if (!bVar6) {
                  puVar1 = auStack_1b8f;
                  pcVar16 = "VISUAL NAME";
                }
                uVar18 = 0x10;
                if (!bVar6) {
                  uVar18 = 100;
                }
                iVar8 = FUN_0014c550(uVar9,pcVar16,puVar1,(ulong)bVar6 << 1,uVar18);
                uVar10 = (uint)(iVar8 != 0);
              }
              uVar17 = uVar10 ^ 1;
              uVar19 = 0;
            }
          }
        }
      }
      uVar13 = FUN_00193f80(1,&DAT_00284688);
      if ((uVar13 & 1) != 0) {
        if (uVar9 != 0) {
          FUN_0014cc0c(uVar9);
          uVar7 = 3;
          goto LAB_0018860c;
        }
        goto LAB_00188608;
      }
      if ((((DAT_0028469c == '\0') || (DAT_0028469e != '\0')) || (DAT_0028469d != 'p')) ||
         ((DAT_0028c298 != lVar5 || (DAT_0028c290 != 4)))) {
        DAT_00284688 = 0;
        if (uVar9 == 0) {
          uVar7 = 4;
        }
        else {
          FUN_0014cc0c(uVar9);
          uVar7 = 4;
        }
        goto LAB_0018860c;
      }
      DAT_0028c2b8 = uStack_2268;
      _DAT_0028c2b0 = local_2270;
      _DAT_0028c28c = CONCAT44(4,uVar21);
      DAT_0028c2c0 = local_2260;
      DAT_0028c2a0 = param_2;
      if (uVar21 != 0) {
        memcpy(&DAT_0028a4b8,&local_1e48,0x1dd0);
      }
      if (iVar11 == 0) {
        _DAT_0028c28c = _DAT_0028c28c & 0xffffffff;
      }
      else if (uVar17 == 0) {
        _DAT_0028c28c = CONCAT44(2,DAT_0028c28c);
        if (uVar9 != 0) {
          DAT_0028c2a8 = uVar9;
        }
      }
      else {
        pcVar16 = "UNAVAILABLE IN THE CURRENT SCREEN";
        if (uVar10 != 0) {
          pcVar16 = "APPLYING...";
        }
        pcVar2 = "CANCELLED";
        if (uVar10 != 2) {
          pcVar2 = pcVar16;
        }
        _DAT_0028c28c = _DAT_0028c28c & 0xffffffff;
        DAT_0028c2a8 = 0;
        FUN_00183ac8(&DAT_0028c2c8,0x60,0x60,&DAT_001343dc,pcVar2);
      }
      if ((uVar19 != 0) && (uVar4 != 0x28004)) {
        DAT_0028469c = '\0';
      }
      DAT_002846a0 = DAT_002846a0 + 1;
      DAT_00284688 = 0;
      if ((uVar17 != 0) && (uVar12 != 0)) {
        FUN_0014cc0c(uVar12);
      }
      if ((uVar17 != 0) && (uVar9 != 0)) {
        FUN_0014cc0c(uVar9);
      }
      if (uVar19 != 0) {
        if (uVar4 == 0x28001) {
          uVar15 = __strlen_chk(auStack_224c,0x401);
          uVar10 = FUN_00191f60(auStack_224c,uVar15);
LAB_00188714:
          uVar12 = 0;
        }
        else if ((uVar4 & 0xfffffffe) == 0x28002) {
          puVar1 = auStack_224c;
          if (uVar4 == 0x28003) {
            uVar15 = 0;
            puVar1 = &DAT_00134f22;
          }
          else {
            uVar15 = __strlen_chk(auStack_224c,0x401);
          }
          uVar10 = FUN_00191fa8(puVar1,uVar15);
          if (uVar10 == 0) goto LAB_00188714;
          uVar12 = nexus_script_port_ui_reload_request();
        }
        else {
          if (uVar4 != 0x28005) {
            if (uVar4 == 0x28004) {
              lVar20 = 0;
              ppuVar22 = &PTR_s_DisablePinAnimation_0019d588;
              do {
                iVar11 = strcmp(*ppuVar22,"ShowSkinNamesInProfile");
                if (iVar11 == 0) {
                  iVar11 = FUN_001924cc(0x21000 - (int)lVar20);
                  uVar10 = (uint)(iVar11 == 1);
                  uVar12 = 0;
                  goto code_r0x0018871c;
                }
                lVar20 = lVar20 + -1;
                ppuVar22 = ppuVar22 + 3;
              } while (lVar20 != -0x37);
              uVar10 = 0;
            }
            goto LAB_00188714;
          }
          uVar10 = nexus_script_port_ui_reload_request();
          uVar12 = uVar10;
        }
code_r0x0018871c:
        uVar13 = FUN_00193f80(1,&DAT_00284688);
        if ((uVar13 & 1) == 0) {
          if (((DAT_0028c298 == lVar5) && (DAT_0028469e == '\0')) && (DAT_0028469d == 'p')) {
            DAT_0028c2a0 = 0;
            if (uVar10 == 0) {
              pcVar16 = "ACTION COULD NOT BE APPLIED";
LAB_00188850:
              DAT_0028469c = '\x01';
LAB_0018885c:
              FUN_00183ac8(&DAT_0028c2c8,0x60,0x60,pcVar16);
            }
            else {
              if (((uVar4 & 0xfffffffe) == 0x28002) && (uVar12 == 0)) {
                pcVar16 = "NAME SAVED - RELOAD TO APPLY";
                goto LAB_00188850;
              }
              if (uVar4 == 0x28004) {
                pcVar16 = "APPLIED TO THE NEXT PROFILE";
                goto LAB_0018885c;
              }
            }
            DAT_002846a0 = DAT_002846a0 + 1;
          }
          DAT_00284688 = 0;
        }
      }
      uVar7 = uVar10 != 0;
      goto LAB_0018860c;
    }
    _DAT_0028c28c = _DAT_0028c28c & 0xffffffff;
  }
  DAT_0028c2a8 = 0;
  DAT_00284688 = 0;
  FUN_0014cc0c(uVar12);
  uVar7 = 1;
LAB_0018860c:
  if (*(long *)(lVar3 + 0x28) != local_78) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar7);
  }
  return;
}

/* ===== nexus_menu_main_count @ 0018975c ===== */

int nexus_menu_main_count(void)

{
  int iVar1;
  long lVar2;
  uint uVar3;
  
  iVar1 = 6;
  switch(DAT_0028469d) {
  case 0x51:
    FUN_00191920(&DAT_00284af0,0x918);
    if (0x20 < DAT_00284af4) {
      DAT_00284af4 = 0;
    }
    iVar1 = DAT_00284af4 + 3;
    break;
  default:
    iVar1 = 0;
    lVar2 = 0x18;
    do {
      if (DAT_0028469d == 0x52) {
        uVar3 = 1;
      }
      else {
        uVar3 = (uint)(*(uint *)((long)&DAT_0019ddd0 + lVar2) == (uint)DAT_0028469d);
      }
      iVar1 = uVar3 + iVar1;
      lVar2 = lVar2 + 0x28;
    } while (lVar2 != 0xba8);
    break;
  case 100:
    iVar1 = 0x29;
    break;
  case 0x65:
    iVar1 = 2;
    if (DAT_0028cc30 != 4) {
      iVar1 = 0x15;
    }
    break;
  case 0x70:
    break;
  case 0x74:
    if (DAT_00287b90 == 2) {
      iVar1 = 4;
    }
    else if (DAT_00287b90 == 3) {
      iVar1 = 10;
    }
    else {
      iVar1 = 6;
      if (DAT_00287b98 != 0) {
        iVar1 = DAT_00285420 + 6;
      }
    }
  }
  return iVar1;
}

/* ===== nexus_menu_section_snapshot @ 0018c44c ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 nexus_menu_section_snapshot(undefined8 *param_1,long param_2,int param_3)

{
  int *piVar1;
  undefined4 uVar2;
  int iVar3;
  undefined8 uVar4;
  char cVar5;
  byte bVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  bool bVar10;
  int iVar11;
  ulong uVar12;
  undefined8 uVar13;
  void *pvVar14;
  char *pcVar15;
  uint uVar16;
  uint *puVar17;
  uint uVar18;
  ulong uVar19;
  ulong uVar20;
  undefined *puVar21;
  undefined8 *puVar22;
  int *piVar23;
  undefined **ppuVar24;
  char *__s1;
  
  if (param_1 == (undefined8 *)0x0) {
    return 0;
  }
  if (param_2 != 0x24a68) {
    return 0;
  }
  uVar12 = FUN_00193f80(1,&DAT_00284688);
  if ((uVar12 & 1) != 0) {
    return 0;
  }
  uVar13 = 0;
  if ((0 < param_3) && (DAT_00284694 == param_3)) {
    memset(param_1,0,0x24a68);
    uVar7 = DAT_0028dcf0;
    uVar4 = DAT_0010f820;
    param_1[7] = 0xffffffffffffffff;
    param_1[8] = 0xffffffffffffffff;
    uVar8 = DAT_0028dcf4;
    uVar13 = 1;
    *param_1 = uVar4;
    uVar9 = DAT_0028dcf8;
    uVar2 = DAT_002846a0;
    bVar6 = DAT_0028469d;
    *(undefined4 *)((long)param_1 + 0x5c) = uVar7;
    *(undefined4 *)(param_1 + 0xc) = uVar8;
    cVar5 = DAT_0028469c;
    *(uint *)((long)param_1 + 0x14) = (uint)bVar6;
    *(undefined4 *)((long)param_1 + 100) = uVar9;
    *(undefined4 *)((long)param_1 + 0xc) = uVar2;
    iVar3 = DAT_0028a42c;
    uVar8 = DAT_0028a428;
    uVar7 = DAT_0028a3dc;
    uVar2 = DAT_0028a3d8;
    iVar11 = DAT_00287b98;
    if ((cVar5 != '\0') && (DAT_0028469e == '\0')) {
      if (bVar6 == 0x74) {
        *(undefined4 *)(param_1 + 1) = 2;
        iVar3 = DAT_00287ba4;
        uVar18 = DAT_00287b90;
        bVar10 = DAT_00285410 != 0;
        *(undefined4 *)(param_1 + 2) = DAT_00285414;
        *(uint *)((long)param_1 + 0x1c) = uVar18;
        uVar2 = DAT_0028542c;
        *(uint *)(param_1 + 4) = (uint)(iVar11 != 0 && bVar10);
        uVar7 = DAT_00285438;
        uVar13 = _DAT_00285424;
        *(undefined4 *)((long)param_1 + 0x3c) = uVar2;
        *(uint *)((long)param_1 + 0x24) = (uint)(iVar3 != 0);
        *(undefined8 *)((long)param_1 + 0x34) = uVar13;
        iVar11 = DAT_00287ba0;
        uVar13 = _DAT_00285430;
        uVar2 = DAT_0028793c;
        if (DAT_00287b9c == 0) {
          uVar2 = 0xffffffff;
        }
        *(undefined4 *)(param_1 + 8) = uVar2;
        uVar2 = DAT_00287a64;
        *(undefined4 *)((long)param_1 + 0x54) = uVar7;
        uVar7 = DAT_00287c68;
        *(undefined8 *)((long)param_1 + 0x4c) = uVar13;
        if (iVar11 == 0) {
          uVar2 = 0xffffffff;
        }
        *(undefined4 *)((long)param_1 + 0x44) = uVar2;
        *(undefined4 *)(param_1 + 9) = uVar7;
        FUN_00183ac8(param_1 + 0xd,0xffffffffffffffff,0x80,&DAT_001343dc,&DAT_00287c6c);
        if (DAT_00287b9c != 0) {
          FUN_00183ac8(param_1 + 0x1d,0xffffffffffffffff,0x60,&DAT_001343dc,&DAT_00287944);
          FUN_00183ac8(param_1 + 0x35,0xffffffffffffffff,0x60,&DAT_001343dc,&DAT_002879a4);
          FUN_00183ac8(param_1 + 0x41,0xffffffffffffffff,0x60,&DAT_001343dc,&DAT_00287a04);
        }
        if (DAT_00287ba0 != 0) {
          FUN_00183ac8(param_1 + 0x29,0xffffffffffffffff,0x60,&DAT_001343dc,&DAT_00287a6c);
        }
        if ((((DAT_00287b98 != 0) &&
             (iVar11 = FUN_0018cb98(&DAT_00285408,DAT_00287b94), iVar11 != 0)) &&
            (param_1[5] = _DAT_00285418, uVar18 = DAT_00285420, DAT_00287b90 < 2)) &&
           (*(uint *)(param_1 + 6) = DAT_00285420, uVar18 != 0)) {
          uVar12 = 0;
          piVar23 = (int *)(param_1 + 0x4d);
          puVar21 = &DAT_002854a4;
          do {
            uVar18 = DAT_00287b90;
            iVar3 = *(int *)(puVar21 + -0x68);
            iVar11 = *(int *)(param_1 + 4);
            piVar1 = (int *)0x285428;
            if (DAT_00287b90 != 0) {
              piVar1 = &DAT_0028542c;
            }
            *piVar23 = (int)uVar12 + 0x26020;
            piVar23[1] = (int)uVar12;
            piVar23[4] = iVar3;
            if ((iVar11 == 0) || (*(int *)((long)param_1 + 0x24) != 0)) {
              uVar18 = 0;
            }
            else if (uVar18 != 1) {
              uVar18 = (byte)puVar21[-100] >> 1 & 1;
            }
            piVar23[2] = uVar18 & 0xffffffe0 |
                         uVar18 & 7 | (uint)(iVar3 == *piVar1) << 1 |
                         (*(uint *)(puVar21 + -100) >> 1 & 3) << 3;
            FUN_00183ac8(piVar23 + 0x16,0xffffffffffffffff,0x60,&DAT_001343dc,puVar21 + -0x60);
            FUN_00183ac8(piVar23 + 0x62,0xffffffffffffffff,0x60,&DAT_001343dc,puVar21);
            FUN_00183ac8(piVar23 + 0x7a,0xffffffffffffffff,0x60,&DAT_001343dc,puVar21 + 0x60);
            uVar12 = uVar12 + 1;
            puVar21 = puVar21 + 0x128;
            piVar23 = piVar23 + 0x92;
          } while (uVar12 < DAT_00285420);
          DAT_00284688 = 0;
          return 1;
        }
      }
      else if (bVar6 == 100) {
        *(undefined4 *)(param_1 + 1) = 3;
        *(undefined4 *)(param_1 + 3) = uVar7;
        *(int *)((long)param_1 + 0x1c) = iVar3;
        *(undefined4 *)(param_1 + 2) = uVar2;
        *(undefined4 *)(param_1 + 4) = uVar8;
        *(uint *)((long)param_1 + 0x24) = (uint)(iVar3 != 0);
        *(undefined4 *)(param_1 + 5) = 0x28;
        *(undefined4 *)(param_1 + 6) = 0x28;
        FUN_00183ac8(param_1 + 0xd,0xffffffffffffffff,0x80,&DAT_001343dc,&DAT_0028a454);
        if (0x100 < *(uint *)(param_1 + 6)) {
          DAT_00284688 = 0;
          return 0;
        }
        if (*(uint *)(param_1 + 6) != 0) {
          uVar12 = 0;
          piVar23 = (int *)(param_1 + 0x4d);
          ppuVar24 = &PTR_s_ABOUT_SCREEN_0019cf58;
          do {
            iVar11 = *(int *)(ppuVar24 + 2);
            uVar18 = *(uint *)(ppuVar24 + -2);
            iVar3 = *(int *)(param_1 + 4);
            *piVar23 = (int)uVar12 + 0x27020;
            piVar23[1] = (int)uVar12;
            piVar23[3] = iVar11;
            piVar23[4] = uVar18;
            if ((iVar3 == 0) || (*(int *)((long)param_1 + 0x24) != 0)) {
              uVar18 = 0;
            }
            else {
              uVar16 = 0;
              if ((uVar18 < 0x100) && (uVar18 < DAT_0028a3e0)) {
                uVar16 = *(uint *)(&DAT_0028a3e4 + ((ulong)(uVar18 >> 3) & 0x1ffffffc)) >>
                         (ulong)(uVar18 & 0x1f) & 1;
              }
              uVar18 = (uint)(uVar16 != 0);
            }
            __s1 = ppuVar24[-1];
            piVar23[2] = uVar18;
            FUN_00183ac8(piVar23 + 6,0xffffffffffffffff,0x40,&DAT_001343dc,__s1);
            FUN_00183ac8(piVar23 + 0x16,0xffffffffffffffff,0x60,&DAT_001343dc,*ppuVar24);
            piVar1 = piVar23 + 0x2e;
            FUN_00183ac8(piVar1,0xffffffffffffffff,0xd0,&DAT_001343dc,ppuVar24[1]);
            uVar18 = piVar23[2];
            if ((uVar18 & 1) == 0) {
              if (*(int *)(param_1 + 4) == 0) {
                pcVar15 = "DEBUG PROVIDER UNAVAILABLE";
              }
              else {
                pcVar15 = "UNAVAILABLE IN THE CURRENT CONTEXT";
                if (*(int *)((long)param_1 + 0x24) != 0) {
                  pcVar15 = "WAITING FOR INPUT OR ACTION";
                }
              }
              FUN_00183ac8(piVar1,0xffffffffffffffff,0xd0,&DAT_001343dc,pcVar15);
              uVar18 = piVar23[2];
            }
            if ((((uVar18 & 1) == 0) && (DAT_0028a438 != 0)) &&
               ((iVar11 = strcmp(__s1,"GFX_QUALITY_CYCLE"), iVar11 == 0 ||
                (iVar11 = strcmp(__s1,"MEM_QUALITY_CYCLE"), iVar11 == 0)))) {
              FUN_00183ac8(piVar1,0xffffffffffffffff,0xd0,&DAT_001343dc,
                           "DISABLE MAX OPTIMIZATION FIRST");
            }
            uVar12 = uVar12 + 1;
            piVar23 = piVar23 + 0x92;
            ppuVar24 = ppuVar24 + 5;
          } while (uVar12 < *(uint *)(param_1 + 6));
          DAT_00284688 = 0;
          return 1;
        }
      }
      else {
        if (bVar6 != 0x51) {
          DAT_00284688 = 0;
          return 1;
        }
        *(undefined4 *)(param_1 + 1) = 1;
        uVar18 = DAT_00284af4;
        if (DAT_00284af0 != 0x918) {
          DAT_00284688 = 0;
          return 1;
        }
        if (0x20 < DAT_00284af4) {
          DAT_00284688 = 0;
          return 1;
        }
        if (DAT_00284b00 + 1 < 0 != SCARRY4(DAT_00284b00,1)) {
          DAT_00284688 = 0;
          return 1;
        }
        if (DAT_00284af4 == 0) {
          uVar18 = 0;
        }
        else {
          uVar12 = 0;
          uVar20 = (ulong)DAT_00284af4;
          do {
            if ((999999 < (uint)(&DAT_00284b08)[uVar12 * 0x12]) ||
               (pvVar14 = memchr(&DAT_00284b10 + uVar12 * 0x48,0,0x40), pvVar14 == (void *)0x0))
            goto LAB_0018cb68;
            if (uVar12 != 0) {
              puVar17 = &DAT_00284b08;
              uVar19 = uVar12;
              do {
                if ((&DAT_00284b08)[uVar12 * 0x12] == *puVar17) goto LAB_0018cb68;
                uVar19 = uVar19 - 1;
                puVar17 = puVar17 + 0x12;
              } while (uVar19 != 0);
            }
            uVar12 = uVar12 + 1;
          } while (uVar12 < uVar20);
        }
        *(int *)(param_1 + 7) = DAT_00284b00;
        *(uint *)(param_1 + 5) = uVar18;
        uVar7 = DAT_00284b04;
        iVar11 = _DAT_00284afc;
        uVar2 = DAT_00284af8;
        *(uint *)(param_1 + 6) = uVar18;
        *(undefined4 *)(param_1 + 2) = uVar2;
        *(undefined4 *)(param_1 + 0xb) = uVar7;
        *(uint *)(param_1 + 4) = (uint)(iVar11 != 0);
        if (uVar18 != 0) {
          uVar12 = 0;
          puVar22 = param_1 + 0x58;
          puVar21 = &DAT_00284b10;
          do {
            *(int *)(puVar22 + -0xb) = (int)uVar12 + 0x24010;
            *(int *)((long)puVar22 + -0x54) = (int)uVar12;
            uVar13 = *(undefined8 *)(puVar21 + -8);
            puVar22[-9] = uVar13;
            *(uint *)(puVar22 + -10) =
                 (uint)(*(int *)(param_1 + 4) != 0) | (uint)((int)uVar13 == DAT_00284b00) << 1;
            FUN_00183ac8(puVar22,0xffffffffffffffff,0x60,&DAT_001343dc,puVar21);
            uVar12 = uVar12 + 1;
            puVar21 = puVar21 + 0x48;
            puVar22 = puVar22 + 0x49;
          } while (uVar12 < DAT_00284af4);
          DAT_00284688 = 0;
          return 1;
        }
      }
LAB_0018cb68:
      uVar13 = 1;
    }
  }
  DAT_00284688 = 0;
  return uVar13;
}

/* ===== nexus_menu_section_present @ 0018cd34 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 nexus_menu_section_present(int *param_1,long param_2,int param_3)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  ulong uVar6;
  uint uVar7;
  int *piVar8;
  int *piVar9;
  int iVar10;
  
  if (param_1 == (int *)0x0) {
    return 0;
  }
  if (param_2 != 0x24a68) {
    return 0;
  }
  if ((((*param_1 != 1) || (iVar10 = 0x2400f, param_1[1] != 0x24a68)) ||
      (0x100 < (uint)param_1[0xc])) || (uVar6 = FUN_00193f80(1,&DAT_00284688), (uVar6 & 1) != 0)) {
    return 0;
  }
  if (param_3 < 1) {
    DAT_00284688 = 0;
    return 0;
  }
  if (DAT_00284694 != param_3) {
    DAT_00284688 = 0;
    return 0;
  }
  if (DAT_0028469c == '\0') {
    DAT_00284688 = 0;
    return 0;
  }
  if (DAT_0028469e != '\0') {
    DAT_00284688 = 0;
    return 0;
  }
  if ((param_1[5] == (uint)DAT_0028469d) && (DAT_002846a0 == param_1[3])) {
    iVar2 = param_1[2];
    if (iVar2 == 1) {
      if (((DAT_0028469d == 0x51) && (param_1[4] == DAT_00284af8)) && (param_1[0xc] == DAT_00284af4)
         ) {
        uVar6 = (ulong)(uint)param_1[0xc];
        param_1 = param_1 + 0x9e;
        piVar8 = &DAT_00284b08;
        while( true ) {
          if (uVar6 == 0) {
            DAT_00284688 = 0;
            return 1;
          }
          iVar10 = iVar10 + 1;
          if (iVar10 != param_1[-4]) break;
          iVar2 = *param_1;
          iVar3 = *piVar8;
          param_1 = param_1 + 0x92;
          uVar6 = uVar6 - 1;
          piVar8 = piVar8 + 0x12;
          if (iVar2 != iVar3) {
            DAT_00284688 = 0;
            return 0;
          }
        }
        DAT_00284688 = 0;
        return 0;
      }
    }
    else if (iVar2 == 3) {
      if (((DAT_0028469d == 100) && (DAT_0028a3d8 == param_1[4])) && (param_1[0xc] == 0x28)) {
        uVar6 = 0;
        piVar8 = param_1 + 0x9a;
        piVar9 = &DAT_0019cf48;
        while ((uint)param_1[0xc] != uVar6) {
          if (uVar6 != (uint)piVar8[1]) {
            DAT_00284688 = 0;
            return 0;
          }
          uVar6 = uVar6 + 1;
          if ((int)uVar6 + 0x2701f != *piVar8) {
            DAT_00284688 = 0;
            return 0;
          }
          piVar1 = piVar8 + 4;
          iVar10 = *piVar9;
          piVar8 = piVar8 + 0x92;
          piVar9 = piVar9 + 10;
          if (*piVar1 != iVar10) {
            DAT_00284688 = 0;
            return 0;
          }
        }
        if (DAT_0028a428 == 0) {
          DAT_00284688 = 0;
          return 1;
        }
        uRam000000000028a40c = 0;
        _DAT_0028a404 = 0;
        uRam000000000028a41c = 0;
        _DAT_0028a414 = 0;
        if (param_1[0xc] == 0) {
          DAT_00284688 = 0;
          _DAT_0028a404 = 0;
          uRam000000000028a40c = 0;
          _DAT_0028a414 = 0;
          uRam000000000028a41c = 0;
          return 1;
        }
        uVar7 = 0;
        do {
          uVar5 = uVar7 >> 5;
          uVar4 = uVar7 & 0x1f;
          uVar7 = uVar7 + 1;
          (&DAT_0028a404)[uVar5] = (&DAT_0028a404)[uVar5] | 1 << (ulong)uVar4;
        } while (uVar7 < (uint)param_1[0xc]);
        DAT_00284688 = 0;
        return 1;
      }
    }
    else if (((iVar2 == 2) && (DAT_0028469d == 0x74)) &&
            ((DAT_00285414 == param_1[4] && (DAT_00287b90 == param_1[7])))) {
      iVar10 = DAT_00285420;
      if (1 < DAT_00287b90 || DAT_00287b98 == 0) {
        iVar10 = 0;
      }
      if (param_1[0xc] != iVar10) {
        DAT_00284688 = 0;
        return 0;
      }
      uVar6 = 0;
      piVar8 = param_1 + 0x9a;
      piVar9 = &DAT_0028543c;
      while( true ) {
        if ((uint)param_1[0xc] == uVar6) {
          if (DAT_00287b98 == 0) {
            DAT_00284688 = 0;
            return 1;
          }
          if (DAT_00287b90 < 2) {
            DAT_00287bb0 = DAT_00287bac;
            if (param_1[0xc] != 0) {
              uVar6 = 0;
              DAT_00287bb4 = 0;
              piVar8 = param_1 + 0x9e;
              do {
                iVar10 = *piVar8;
                piVar8 = piVar8 + 0x92;
                (&DAT_00287bc0)[uVar6] = iVar10;
                DAT_00287bb4 = DAT_00287bb4 | 1 << (ulong)((uint)uVar6 & 0x1f);
                uVar6 = uVar6 + 1;
              } while (uVar6 < (uint)param_1[0xc]);
              DAT_00284688 = 0;
              return 1;
            }
            DAT_00284688 = 0;
            DAT_00287bb0 = DAT_00287bac;
            DAT_00287bb4 = 0;
            return 1;
          }
          DAT_00284688 = 0;
          return 1;
        }
        if ((uVar6 != (uint)piVar8[1]) || (uVar6 = uVar6 + 1, (int)uVar6 + 0x2601f != *piVar8))
        break;
        piVar1 = piVar8 + 4;
        iVar10 = *piVar9;
        piVar8 = piVar8 + 0x92;
        piVar9 = piVar9 + 0x4a;
        if (*piVar1 != iVar10) {
          DAT_00284688 = 0;
          return 0;
        }
      }
    }
  }
  DAT_00284688 = 0;
  return 0;
}

/* ===== FUN_0018d64c @ 0018d64c ===== */

void FUN_0018d64c(int param_1)

{
  uint uVar1;
  long lVar2;
  undefined *puVar3;
  int iVar4;
  int iVar5;
  undefined8 uVar6;
  undefined8 *puVar7;
  long lVar8;
  int *piVar9;
  uint uVar10;
  long lVar11;
  long lVar12;
  char acStack_2a8 [96];
  uint local_248 [118];
  long local_70;
  
  lVar2 = tpidr_el0;
  local_70 = *(long *)(lVar2 + 0x28);
  memcpy(local_248,&DAT_002846b0,0x1d8);
  puVar3 = PTR_PTR_s_nexus_sx_spin_001a36f8;
  uVar6 = 0;
  if (param_1 < 0x5c) {
    if (param_1 == 0x40) {
      lVar11 = 0;
      piVar9 = (int *)(PTR_PTR_s_nexus_sx_spin_001a36f8 + 0xc);
      do {
        iVar4 = strcmp(*(char **)(piVar9 + -3),"nexus_sx_aop_target_mode");
        if (iVar4 == 0) {
          if (((-1 < (int)lVar11) && (piVar9[-1] < 1)) && (-1 < *piVar9)) {
            lVar8 = 0;
            piVar9 = (int *)(puVar3 + 0xc);
            local_248[lVar11] = 0;
            goto LAB_0018da64;
          }
          break;
        }
        lVar11 = lVar11 + 1;
        piVar9 = piVar9 + 6;
      } while (lVar11 != 0x76);
    }
    else {
      if (param_1 != 0x4a) goto LAB_0018ddf0;
      lVar8 = 0;
      puVar7 = (undefined8 *)PTR_PTR_s_nexus_sx_spin_001a36f8;
      do {
        iVar4 = strcmp((char *)*puVar7,"nexus_autofarm_follow_target");
        if (iVar4 == 0) {
          if (-1 < (int)lVar8) {
            lVar11 = 0;
            piVar9 = (int *)(puVar3 + 0xc);
            uVar10 = (uint)(local_248[lVar8] == 0);
            goto LAB_0018da0c;
          }
          break;
        }
        lVar8 = lVar8 + 1;
        puVar7 = puVar7 + 3;
      } while (lVar8 != 0x76);
    }
  }
  else {
    if (param_1 == 0x5c) {
      lVar11 = 0;
      puVar7 = (undefined8 *)PTR_PTR_s_nexus_sx_spin_001a36f8;
      do {
        iVar4 = strcmp((char *)*puVar7,"nexus_autododge_version");
        if (iVar4 == 0) {
          if (-1 < (int)lVar11) {
            iVar4 = *(int *)((long)&DAT_002846b0 + lVar11 * 4);
            if (0xfffffffa < iVar4 - 6U) goto LAB_0018d818;
            goto LAB_0018ddec;
          }
          break;
        }
        lVar11 = lVar11 + 1;
        puVar7 = puVar7 + 3;
      } while (lVar11 != 0x76);
      iVar4 = 3;
LAB_0018d818:
      lVar11 = 0;
      do {
        if (iVar4 == 1) {
          FUN_00183ac8(acStack_2a8,0x60,0x60,"nexus_dodge_%s",(&PTR_s_reaction_pct_0019e960)[lVar11]
                      );
        }
        else {
          FUN_00183ac8(acStack_2a8,0x60,0x60,"nexus_v%d_dodge_%s",iVar4);
        }
        lVar8 = 0;
        uVar10 = 0x424;
        if (lVar11 != 5) {
          uVar10 = 100;
        }
        uVar1 = 0xb4;
        if (lVar11 != 0) {
          uVar1 = uVar10;
        }
        piVar9 = (int *)(puVar3 + 0xc);
        while (iVar5 = strcmp(*(char **)(piVar9 + -3),acStack_2a8), iVar5 != 0) {
          lVar8 = lVar8 + 1;
          piVar9 = piVar9 + 6;
          if (lVar8 == 0x76) goto LAB_0018ddec;
        }
        if ((((int)lVar8 < 0) || ((int)uVar1 < piVar9[-1])) || (*piVar9 < (int)uVar1))
        goto LAB_0018ddec;
        lVar11 = lVar11 + 1;
        local_248[lVar8] = uVar1;
      } while (lVar11 != 6);
      lVar8 = 0;
      piVar9 = (int *)(puVar3 + 0xc);
      do {
        iVar4 = strcmp(*(char **)(piVar9 + -3),"nexus_autododge_require_hold");
        if (iVar4 == 0) {
          if ((((int)lVar8 < 0) || (0 < piVar9[-1])) || (*piVar9 < 0)) goto LAB_0018ddec;
          lVar11 = 0;
          piVar9 = (int *)(puVar3 + 0xc);
          local_248[lVar8] = 0;
          goto LAB_0018dbf4;
        }
        lVar8 = lVar8 + 1;
        piVar9 = piVar9 + 6;
        uVar6 = 4;
      } while (lVar8 != 0x76);
      goto LAB_0018ddf0;
    }
    if (param_1 == 0x70) {
      lVar11 = 0;
      puVar7 = (undefined8 *)PTR_PTR_s_nexus_sx_spin_001a36f8;
      do {
        iVar4 = strcmp((char *)*puVar7,"nexus_sx_outline_color_preset");
        if (iVar4 == 0) {
          if (-1 < (int)lVar11) {
            lVar8 = 0;
            piVar9 = (int *)(puVar3 + 0xc);
            uVar10 = (int)(local_248[lVar11] + 1) % 7;
            goto LAB_0018d960;
          }
          break;
        }
        lVar11 = lVar11 + 1;
        puVar7 = puVar7 + 3;
      } while (lVar11 != 0x76);
    }
    else {
      if (param_1 != 0x91) goto LAB_0018ddf0;
      lVar11 = 0;
      piVar9 = (int *)(PTR_PTR_s_nexus_sx_spin_001a36f8 + 0xc);
      do {
        iVar4 = strcmp(*(char **)(piVar9 + -3),"nexus_sx_bolt_smooth");
        if (iVar4 == 0) {
          if (((-1 < (int)lVar11) && (piVar9[-1] < 0x56)) && (0x54 < *piVar9)) {
            lVar8 = 0;
            piVar9 = (int *)(puVar3 + 0xc);
            local_248[lVar11] = 0x55;
            goto LAB_0018d9c0;
          }
          break;
        }
        lVar11 = lVar11 + 1;
        piVar9 = piVar9 + 6;
      } while (lVar11 != 0x76);
    }
  }
  goto LAB_0018ddec;
  while( true ) {
    lVar8 = lVar8 + 1;
    piVar9 = piVar9 + 6;
    uVar6 = 4;
    if (lVar8 == 0x76) break;
LAB_0018da64:
    iVar4 = strcmp(*(char **)(piVar9 + -3),"nexus_sx_combat_aura_range");
    if (iVar4 == 0) {
      if ((((int)lVar8 < 0) || (0x1068 < piVar9[-1])) || (*piVar9 < 0x1068)) goto LAB_0018ddec;
      lVar11 = 0;
      piVar9 = (int *)(puVar3 + 0xc);
      local_248[lVar8] = 0x1068;
      goto LAB_0018db9c;
    }
  }
  goto LAB_0018ddf0;
  while( true ) {
    lVar11 = lVar11 + 1;
    piVar9 = piVar9 + 6;
    uVar6 = 4;
    if (lVar11 == 0x76) break;
LAB_0018db9c:
    iVar4 = strcmp(*(char **)(piVar9 + -3),"nexus_sx_combat_fire_interval");
    if (iVar4 == 0) {
      if ((((int)lVar11 < 0) || (1000 < piVar9[-1])) || (*piVar9 < 1000)) goto LAB_0018ddec;
      lVar8 = 0;
      piVar9 = (int *)(puVar3 + 0xc);
      local_248[lVar11] = 1000;
      goto LAB_0018dd1c;
    }
  }
  goto LAB_0018ddf0;
  while( true ) {
    lVar8 = lVar8 + 1;
    piVar9 = piVar9 + 6;
    uVar6 = 4;
    if (lVar8 == 0x76) break;
LAB_0018dd1c:
    iVar4 = strcmp(*(char **)(piVar9 + -3),"nexus_sx_aop_reaction_ms");
    if (iVar4 == 0) {
      if ((((int)lVar8 < 0) || (0 < piVar9[-1])) || (*piVar9 < 0)) goto LAB_0018ddec;
      lVar11 = 0;
      piVar9 = (int *)(puVar3 + 0xc);
      local_248[lVar8] = 0;
      goto LAB_0018de84;
    }
  }
  goto LAB_0018ddf0;
  while( true ) {
    lVar11 = lVar11 + 1;
    piVar9 = piVar9 + 6;
    uVar6 = 4;
    if (lVar11 == 0x76) break;
LAB_0018de84:
    iVar4 = strcmp(*(char **)(piVar9 + -3),"nexus_sx_aop_lead_scale");
    if (iVar4 == 0) {
      if ((((int)lVar11 < 0) || (100 < piVar9[-1])) || (*piVar9 < 100)) goto LAB_0018ddec;
      uVar10 = 100;
      goto LAB_0018de40;
    }
  }
  goto LAB_0018ddf0;
  while( true ) {
    lVar11 = lVar11 + 1;
    piVar9 = piVar9 + 6;
    uVar6 = 4;
    if (lVar11 == 0x76) break;
LAB_0018da0c:
    iVar4 = strcmp(*(char **)(piVar9 + -3),"nexus_autofarm_follow_target");
    if (iVar4 == 0) goto LAB_0018ddd0;
  }
  goto LAB_0018ddf0;
  while( true ) {
    lVar11 = lVar11 + 1;
    piVar9 = piVar9 + 6;
    uVar6 = 4;
    if (lVar11 == 0x76) break;
LAB_0018dbf4:
    iVar4 = strcmp(*(char **)(piVar9 + -3),"nexus_dodge_blacklist_mask");
    if (iVar4 == 0) {
      if ((((int)lVar11 < 0) || (0x3ff < piVar9[-1])) || (*piVar9 < 0x3ff)) goto LAB_0018ddec;
      uVar10 = 0x3ff;
      goto LAB_0018de40;
    }
  }
  goto LAB_0018ddf0;
  while( true ) {
    lVar8 = lVar8 + 1;
    piVar9 = piVar9 + 6;
    uVar6 = 4;
    if (lVar8 == 0x76) break;
LAB_0018d960:
    iVar4 = strcmp(*(char **)(piVar9 + -3),"nexus_sx_outline_color_preset");
    if (iVar4 == 0) {
      if ((((int)lVar8 < 0) || ((int)uVar10 < piVar9[-1])) || (*piVar9 < (int)uVar10))
      goto LAB_0018ddec;
      lVar11 = 0;
      piVar9 = (int *)(puVar3 + 0xc);
      uVar1 = *(uint *)(&DAT_0014073c + (long)(int)uVar10 * 0xc);
      local_248[lVar8] = uVar10;
      goto LAB_0018dad8;
    }
  }
  goto LAB_0018ddf0;
  while( true ) {
    lVar11 = lVar11 + 1;
    piVar9 = piVar9 + 6;
    uVar6 = 4;
    if (lVar11 == 0x76) break;
LAB_0018dad8:
    iVar4 = strcmp(*(char **)(piVar9 + -3),"nexus_sx_outline_r");
    if (iVar4 == 0) {
      if ((((int)lVar11 < 0) || ((int)uVar1 < piVar9[-1])) || (*piVar9 < (int)uVar1))
      goto LAB_0018ddec;
      lVar8 = 0;
      piVar9 = (int *)(puVar3 + 0xc);
      local_248[lVar11] = uVar1;
      uVar1 = *(uint *)(&DAT_00140740 + (long)(int)uVar10 * 0xc);
      goto LAB_0018dc5c;
    }
  }
  goto LAB_0018ddf0;
  while( true ) {
    lVar8 = lVar8 + 1;
    piVar9 = piVar9 + 6;
    uVar6 = 4;
    if (lVar8 == 0x76) break;
LAB_0018dc5c:
    iVar4 = strcmp(*(char **)(piVar9 + -3),"nexus_sx_outline_g");
    if (iVar4 == 0) {
      if ((((int)lVar8 < 0) || ((int)uVar1 < piVar9[-1])) || (*piVar9 < (int)uVar1))
      goto LAB_0018ddec;
      lVar11 = 0;
      piVar9 = (int *)(puVar3 + 0xc);
      local_248[lVar8] = uVar1;
      uVar10 = *(uint *)(&DAT_00140744 + (long)(int)uVar10 * 0xc);
      goto LAB_0018dda8;
    }
  }
  goto LAB_0018ddf0;
  while( true ) {
    lVar11 = lVar11 + 1;
    piVar9 = piVar9 + 6;
    uVar6 = 4;
    if (lVar11 == 0x76) break;
LAB_0018dda8:
    iVar4 = strcmp(*(char **)(piVar9 + -3),"nexus_sx_outline_b");
    if (iVar4 == 0) goto LAB_0018ddd0;
  }
  goto LAB_0018ddf0;
LAB_0018ddd0:
  if (((-1 < (int)lVar11) && (piVar9[-1] <= (int)uVar10)) && ((int)uVar10 <= *piVar9)) {
LAB_0018de40:
    local_248[lVar11] = uVar10;
    uVar6 = FUN_001846b8(local_248);
    goto LAB_0018ddf0;
  }
LAB_0018ddec:
  uVar6 = 4;
  goto LAB_0018ddf0;
  while( true ) {
    lVar8 = lVar8 + 1;
    piVar9 = piVar9 + 6;
    uVar6 = 4;
    if (lVar8 == 0x76) break;
LAB_0018d9c0:
    iVar4 = strcmp(*(char **)(piVar9 + -3),"nexus_sx_bolt_wall_lookahead");
    if (iVar4 == 0) {
      if ((((int)lVar8 < 0) || (500 < piVar9[-1])) || (*piVar9 < 500)) goto LAB_0018ddec;
      lVar12 = 0;
      piVar9 = (int *)(puVar3 + 0xc);
      local_248[lVar8] = 500;
      goto LAB_0018db38;
    }
  }
  goto LAB_0018ddf0;
  while( true ) {
    lVar12 = lVar12 + 1;
    piVar9 = piVar9 + 6;
    uVar6 = 4;
    if (lVar12 == 0x76) break;
LAB_0018db38:
    iVar4 = strcmp(*(char **)(piVar9 + -3),"nexus_sx_bolt_target_range");
    if (iVar4 == 0) {
      if ((((int)lVar12 < 0) || (0x578 < piVar9[-1])) || (*piVar9 < 0x578)) goto LAB_0018ddec;
      lVar11 = 0;
      piVar9 = (int *)(puVar3 + 0xc);
      local_248[lVar12] = 0x578;
      goto LAB_0018dcbc;
    }
  }
  goto LAB_0018ddf0;
  while( true ) {
    lVar11 = lVar11 + 1;
    piVar9 = piVar9 + 6;
    uVar6 = 4;
    if (lVar11 == 0x76) break;
LAB_0018dcbc:
    iVar4 = strcmp(*(char **)(piVar9 + -3),"nexus_sx_bolt_exit_hold");
    if (iVar4 == 0) {
      if ((((int)lVar11 < 0) || (0x28a < piVar9[-1])) || (*piVar9 < 0x28a)) goto LAB_0018ddec;
      uVar10 = 0x28a;
      goto LAB_0018de40;
    }
  }
LAB_0018ddf0:
  if (*(long *)(lVar2 + 0x28) != local_70) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar6);
  }
  return;
}

/* ===== nexus_menu_engine_register @ 0018e174 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 nexus_menu_engine_register(int *param_1)

{
  int iVar1;
  ulong uVar2;
  undefined8 uVar3;
  
  if (((((((param_1 == (int *)0x0) || (*param_1 != 1)) || (param_1[1] != 0x80)) ||
        ((*(char **)(param_1 + 6) == (char *)0x0 ||
         (iVar1 = strcmp(*(char **)(param_1 + 6),
                         "a10aeb6b4085fb2a15d969a130cd9608231998b5269a40aed41129d99624ede3"),
         iVar1 != 0)))) ||
       ((*(ulong *)(param_1 + 4) < 0x1000 ||
        ((*(long *)(param_1 + 8) == 0 || (*(long *)(param_1 + 10) == 0)))))) ||
      (*(long *)(param_1 + 0xc) == 0)) ||
     ((((*(long *)(param_1 + 0xe) == 0 || (*(long *)(param_1 + 0x10) == 0)) ||
       (*(long *)(param_1 + 0x12) == 0)) ||
      (((*(long *)(param_1 + 0x14) == 0 || (*(long *)(param_1 + 0x16) == 0)) ||
       ((*(long *)(param_1 + 0x18) == 0 ||
        ((*(long *)(param_1 + 0x1a) == 0 || (*(long *)(param_1 + 0x1c) == 0)))))))))) {
    uVar3 = 4;
  }
  else {
    uVar2 = FUN_00193f80(1,&DAT_0028fb40);
    if ((uVar2 & 1) == 0) {
      if (DAT_0028fb44 == 0) {
        DAT_0028fae8 = *(undefined8 *)(param_1 + 10);
        DAT_0028fae0 = *(undefined8 *)(param_1 + 8);
        DAT_0028faf8 = *(undefined8 *)(param_1 + 0xe);
        DAT_0028faf0 = *(undefined8 *)(param_1 + 0xc);
        DAT_0028fac8 = *(undefined8 *)(param_1 + 2);
        _DAT_0028fac0 = *(undefined8 *)param_1;
        uRam000000000028fad8 = *(undefined8 *)(param_1 + 6);
        DAT_0028fad0 = *(undefined8 *)(param_1 + 4);
        DAT_0028fb28 = *(undefined8 *)(param_1 + 0x1a);
        DAT_0028fb20 = *(undefined8 *)(param_1 + 0x18);
        DAT_0028fb38 = *(undefined8 *)(param_1 + 0x1e);
        DAT_0028fb30 = *(undefined8 *)(param_1 + 0x1c);
        DAT_0028fb08 = *(undefined8 *)(param_1 + 0x12);
        DAT_0028fb00 = *(undefined8 *)(param_1 + 0x10);
        DAT_0028fb18 = *(undefined8 *)(param_1 + 0x16);
        DAT_0028fb10 = *(undefined8 *)(param_1 + 0x14);
        uVar3 = 1;
        DAT_0028fb44 = 1;
        FUN_00183cdc(2,0,"waiting_rooted_mainloop_text");
      }
      else {
        uVar3 = 4;
      }
      DAT_0028fb40 = 0;
    }
    else {
      uVar3 = 3;
    }
  }
  return uVar3;
}

/* ===== nexus_menu_engine_observe @ 0018e2b4 ===== */

ulong nexus_menu_engine_observe
                (float param_1,float param_2,uint param_3,long param_4,int param_5,char *param_6,
                ulong param_7)

{
  bool bVar1;
  undefined4 uVar2;
  char *pcVar3;
  uint uVar4;
  int iVar5;
  ulong uVar6;
  long lVar7;
  long lVar8;
  char *pcVar9;
  long lVar10;
  
  if (param_6 == (char *)0x0) {
    return 0;
  }
  if (param_5 < 1) {
    return 0;
  }
  if (1 < param_3) {
    return 0;
  }
  if (DAT_0028fb44 == 0) {
    return 0;
  }
  if (DAT_0028fb48 < 0) {
    return 0;
  }
  uVar4 = strcmp(param_6,"Mainloop");
  uVar6 = (ulong)uVar4;
  if (uVar4 != 0) {
    return 0;
  }
  if (ABS(param_1) == INFINITY) {
    return uVar6;
  }
  if (NAN(ABS(param_1))) {
    return uVar6;
  }
  if (DAT_0028fb4c != 0 && DAT_0028fb4c != param_5) {
    return uVar6;
  }
  if (param_2 != 8192.0 && param_2 < 8192.0 == NAN(param_2)) {
    return 0;
  }
  if (param_1 != 8192.0 && param_1 < 8192.0 == NAN(param_1)) {
    return 0;
  }
  if (param_2 < 560.0) {
    return 0;
  }
  if (param_1 < 960.0) {
    return 0;
  }
  if (ABS(param_2) == INFINITY) {
    return 0;
  }
  if (NAN(ABS(param_2))) {
    return 0;
  }
  uVar6 = nexus_menu_status(0);
  if ((int)uVar6 == 0) {
    return uVar6;
  }
  uVar6 = FUN_00193f80(1,&DAT_0028fb40);
  if ((uVar6 & 1) != 0) {
    return 3;
  }
  if (DAT_0028fb48 == 2) {
    if ((param_3 == 0) && (param_7 < DAT_0028fb68)) {
      DAT_0028fb40 = 0;
      return 1;
    }
    DAT_0028fb68 = param_7 + 100;
    iVar5 = FUN_0018e668();
    if (iVar5 != 0) {
      if (param_3 == 1) {
        lVar10 = 0;
        do {
          if (((&DAT_0028fca0)[lVar10] != '\0') && ((&DAT_0028fb80)[lVar10] == param_4)) {
            uVar4 = (&DAT_0028fc40)[lVar10];
            if ((uVar4 & 0xfffffffe) == 0x20000) {
              uVar2 = 0xffffffff;
              if (uVar4 == 0x20001) {
                uVar2 = 1;
              }
              iVar5 = nexus_menu_scroll_battle(uVar2,param_5);
            }
            else {
              iVar5 = nexus_menu_dispatch(uVar4,0,0,param_5);
            }
            uVar4 = DAT_0028fb54 + 1;
            bVar1 = DAT_0028fb54 < 0x80;
            DAT_0028fb54 = uVar4;
            if ((bVar1) && (DAT_0028fb38 != (code *)0x0)) {
              pcVar9 = "pending";
              if (iVar5 != 2) {
                pcVar9 = "blocked";
              }
              pcVar3 = "acknowledged";
              if (iVar5 != 1) {
                pcVar3 = pcVar9;
              }
              (*DAT_0028fb38)(DAT_0028fac8,"nexus_menu_click",pcVar3,(&DAT_0028fc40)[lVar10]);
            }
            break;
          }
          lVar10 = lVar10 + 1;
        } while (lVar10 != 0x18);
      }
LAB_0018e63c:
      uVar6 = FUN_0018e880(param_1,param_2);
      DAT_0028fb40 = 0;
      return uVar6;
    }
    pcVar9 = "stage_generation_or_membership";
LAB_0018e5c0:
    FUN_0018e804(pcVar9);
    uVar6 = 0xffffffff;
  }
  else {
    if ((((param_3 == 0) && (lVar10 = FUN_0018edcc(DAT_0028fad0 + 0x12eb9f0), lVar10 != 0)) &&
        (lVar7 = FUN_0018edcc(lVar10 + 0x90), lVar7 != 0)) &&
       ((lVar8 = FUN_0018edcc(param_4), lVar8 == DAT_0028fad0 + 0x11abad8 &&
        (iVar5 = FUN_0018ee70(param_4,lVar7), iVar5 != 0)))) {
      if (DAT_0028fb60 == 0) {
        DAT_0028fb60 = param_7;
      }
      if (DAT_0028fb68 <= param_7) {
        if ((0x1f < DAT_0028fb50) || (0x752 < param_7 - DAT_0028fb60 >> 5)) {
          pcVar9 = "asset_readiness_budget";
          goto LAB_0018e5c0;
        }
        DAT_0028fb50 = DAT_0028fb50 + 1;
        DAT_0028fb68 = param_7 + 500;
        iVar5 = (*DAT_0028fae8)(DAT_0028fac8,"popover_button_blue");
        if (iVar5 != 0) {
          DAT_0028fb4c = param_5;
          iVar5 = FUN_00183c78(param_5);
          if (iVar5 == 0) {
            DAT_0028fb40 = 0;
            DAT_0028fb4c = 0;
            return 3;
          }
          DAT_0028fb48 = 1;
          DAT_0028fb70 = lVar10;
          DAT_0028fb78 = lVar7;
          uVar6 = FUN_0018ef84();
          if ((int)uVar6 != 1) {
            DAT_0028fb40 = 0;
            return uVar6;
          }
          goto LAB_0018e63c;
        }
      }
    }
    uVar6 = 2;
  }
  DAT_0028fb40 = 0;
  return uVar6;
}

/* ===== FUN_0018e804 @ 0018e804 ===== */

void FUN_0018e804(undefined8 param_1)

{
  uint uVar1;
  
  DAT_0028fb48 = 0xffffffff;
  FUN_00183cdc(0xffffffff,DAT_0028fb4c,param_1);
  uVar1 = DAT_0028fb54;
  DAT_0028fb54 = DAT_0028fb54 + 1;
  if ((uVar1 < 0x80) && (DAT_0028fb38 != (code *)0x0)) {
                    /* WARNING: Could not recover jumptable at 0x0018e870. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*DAT_0028fb38)(DAT_0028fac8,"nexus_menu_stopped",param_1,0);
    return;
  }
  return;
}

/* ===== FUN_0018e880 @ 0018e880 ===== */

undefined8 FUN_0018e880(undefined4 param_1,float param_2)

{
  bool bVar1;
  uint uVar2;
  char cVar3;
  long lVar4;
  int iVar5;
  undefined8 *puVar6;
  undefined8 uVar7;
  ulong uVar8;
  long lVar9;
  char *__s1;
  long lVar10;
  long lVar11;
  long lVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  float fVar16;
  ulong local_7b0;
  undefined8 local_7a8;
  undefined1 auStack_7a0 [128];
  undefined4 local_720;
  uint local_71c;
  char local_717;
  undefined4 local_710 [6];
  float local_6f8 [8];
  byte local_6d8 [1608];
  long local_90;
  
  lVar4 = tpidr_el0;
  local_90 = *(long *)(lVar4 + 0x28);
  iVar5 = nexus_menu_view(&local_720,DAT_0028fb4c);
  if (iVar5 == 1) {
    if (local_71c < 0x17) {
      local_7a8 = 0;
      puVar6 = (undefined8 *)nexus_menu_settings(&local_7b0);
      if (local_7b0 != 0) {
        uVar8 = 0;
        do {
          __s1 = (char *)*puVar6;
          iVar5 = strcmp(__s1,"nexus_quick_menu_offset_x");
          if (iVar5 == 0) {
            nexus_menu_setting_value(uVar8 & 0xffffffff,(long)&local_7a8 + 4);
            __s1 = (char *)*puVar6;
          }
          iVar5 = strcmp(__s1,"nexus_quick_menu_offset_y");
          if (iVar5 == 0) {
            nexus_menu_setting_value(uVar8 & 0xffffffff,&local_7a8);
          }
          uVar8 = uVar8 + 1;
          puVar6 = puVar6 + 3;
        } while (uVar8 < local_7b0);
      }
      DAT_0028fc40 = 0x82;
      if (local_717 == '\0') {
        DAT_0028fc40 = 1;
      }
      DAT_0028fca0 = 1;
      iVar5 = FUN_0018f988(0,"NEXUS");
      if (iVar5 == 1) {
        fVar13 = (float)local_7a8._4_4_ + 90.0;
        fVar15 = (float)(int)local_7a8 + 40.0;
        if (((DAT_0028fe40 == '\0') || (DAT_0028fd80 != fVar13)) || (DAT_0028fde0 != fVar15)) {
          (*DAT_0028fb28)(fVar13,fVar15,DAT_0028fac8,DAT_0028fb80);
          DAT_0028fe40 = '\x01';
          DAT_0028fd80 = fVar13;
          DAT_0028fde0 = fVar15;
        }
        DAT_0028fc44 = 0;
        DAT_0028fca1 = 0;
        iVar5 = FUN_0018f988(1,"NEXUS MENU");
        if (iVar5 == 1) {
          fVar14 = (float)NEON_fmadd(param_1,0x3f000000,0x42b40000);
          fVar16 = (param_2 + -600.0) * 0.5;
          fVar15 = -9999.0;
          fVar13 = -9999.0;
          if (local_717 != '\0') {
            fVar15 = fVar16 + 100.0;
            fVar13 = fVar14 + -370.0;
          }
          if (((DAT_0028fe41 == '\0') || (DAT_0028fd84 != fVar13)) || (DAT_0028fde4 != fVar15)) {
            (*DAT_0028fb28)(fVar13,fVar15,DAT_0028fac8,DAT_0028fb88);
            DAT_0028fe41 = '\x01';
            DAT_0028fd84 = fVar13;
            DAT_0028fde4 = fVar15;
          }
          lVar9 = 0;
          lVar10 = 0x1e2;
          lVar11 = 0x188;
          lVar12 = 0xd0;
          do {
            (&DAT_0028fac0)[lVar10] = 0;
            *(undefined4 *)(&DAT_0028fac0 + lVar11) = 0;
            if ((local_717 == '\0') || ((ulong)local_71c <= lVar10 - 0x1e2U)) {
              if ((*(char *)(lVar10 + 0x28fc60) == '\0') ||
                 ((*(float *)(lVar11 + 0x28fc00) != -9999.0 ||
                  (*(float *)(lVar11 + 0x28fc60) != -9999.0)))) {
                (*DAT_0028fb28)(0xc61c3c00,0xc61c3c00,DAT_0028fac8,
                                *(undefined8 *)(&DAT_0028fac0 + lVar12));
                *(undefined4 *)(lVar11 + 0x28fc00) = 0xc61c3c00;
                *(undefined4 *)(lVar11 + 0x28fc60) = 0xc61c3c00;
                goto LAB_0018ed58;
              }
            }
            else {
              if (local_6d8[lVar9] < 8) {
                uVar2 = 1 << (ulong)(local_6d8[lVar9] & 0x1f);
                if ((uVar2 & 0xd8) == 0) {
                  if ((uVar2 & 0x24) == 0) goto LAB_0018ecc4;
                  FUN_0018fb04(auStack_7a0);
                }
                else {
                  FUN_0018fb04(auStack_7a0);
                }
              }
              else {
LAB_0018ecc4:
                FUN_0018fb04(auStack_7a0);
              }
              iVar5 = FUN_0018f988((int)lVar10 + -0x1e0,auStack_7a0);
              if (iVar5 != 1) goto LAB_0018ed88;
              *(undefined4 *)(&DAT_0028fac0 + lVar11) = *(undefined4 *)((long)local_710 + lVar9);
              fVar13 = *(float *)((long)local_6f8 + lVar9);
              fVar15 = *(float *)((long)local_6f8 + lVar9 + 4);
              cVar3 = *(char *)(lVar10 + 0x28fc60);
              (&DAT_0028fac0)[lVar10] = 1;
              fVar13 = fVar14 + fVar13;
              fVar15 = fVar16 + fVar15;
              if (((cVar3 == '\0') || (*(float *)(lVar11 + 0x28fc00) != fVar13)) ||
                 (*(float *)(lVar11 + 0x28fc60) != fVar15)) {
                (*DAT_0028fb28)(fVar13,fVar15,DAT_0028fac8,*(undefined8 *)(&DAT_0028fac0 + lVar12));
                *(float *)(lVar11 + 0x28fc00) = fVar13;
                *(float *)(lVar11 + 0x28fc60) = fVar15;
LAB_0018ed58:
                *(undefined1 *)(lVar10 + 0x28fc60) = 1;
              }
            }
            lVar10 = lVar10 + 1;
            lVar11 = lVar11 + 4;
            lVar12 = lVar12 + 8;
            lVar9 = lVar9 + 0x40;
          } while (lVar9 != 0x580);
          uVar7 = 1;
          DAT_0028fcb8 = local_720;
          goto LAB_0018ed8c;
        }
      }
LAB_0018ed88:
      uVar7 = 0xffffffff;
    }
    else {
      uVar7 = 0xffffffff;
      DAT_0028fb48 = 0xffffffff;
      FUN_00183cdc(0xffffffff,DAT_0028fb4c,"view_capacity");
      uVar2 = DAT_0028fb54 + 1;
      bVar1 = DAT_0028fb54 < 0x80;
      DAT_0028fb54 = uVar2;
      if ((bVar1) && (DAT_0028fb38 != (code *)0x0)) {
        (*DAT_0028fb38)(DAT_0028fac8,"nexus_menu_stopped","view_capacity",0);
      }
    }
  }
  else {
    uVar7 = 3;
  }
LAB_0018ed8c:
  if (*(long *)(lVar4 + 0x28) == local_90) {
    return uVar7;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0018ef84 @ 0018ef84 ===== */

undefined8 FUN_0018ef84(void)

{
  uint uVar1;
  ulong uVar2;
  ulong uVar3;
  ulong uVar4;
  ulong uVar5;
  long lVar6;
  bool bVar7;
  bool bVar8;
  int iVar9;
  long lVar10;
  char *pcVar11;
  char *pcVar12;
  ulong uVar13;
  ulong uVar14;
  undefined8 uVar15;
  ulong uVar16;
  ulong uVar17;
  long lVar18;
  short local_7c [2];
  ulong local_78 [2];
  long local_68;
  
  lVar6 = tpidr_el0;
  bVar8 = false;
  local_68 = *(long *)(lVar6 + 0x28);
  uVar14 = 0;
  do {
    uVar16 = 0;
    uVar3 = (&DAT_0019e9e0)[uVar14 * 3];
    uVar4 = (&DAT_0019e9e8)[uVar14 * 3];
    uVar17 = ~uVar3;
    uVar13 = uVar4;
    do {
      uVar5 = uVar13 - 0x10;
      if (0xf < uVar13) {
        uVar13 = 0x10;
      }
      uVar2 = uVar3 + uVar16 + DAT_0028fad0;
      if (((uVar2 < 0x1000 || uVar17 - DAT_0028fad0 < uVar13) ||
          (iVar9 = (*DAT_0028fae0)(DAT_0028fac8,uVar2,local_78,uVar13), iVar9 != 1)) ||
         (iVar9 = memcmp(local_78,(&PTR_DAT_0019e9f0)[uVar14 * 3] + uVar16,uVar13), iVar9 != 0))
      goto joined_r0x0018f090;
      uVar16 = uVar16 + 0x10;
      uVar17 = uVar17 - 0x10;
      uVar13 = uVar5;
    } while (uVar16 < uVar4);
    uVar16 = uVar14 + 1;
    bVar8 = 7 < uVar14;
    uVar14 = uVar16;
  } while (uVar16 != 9);
joined_r0x0018f090:
  if (bVar8) {
    uVar14 = 0;
    lVar18 = 0x2c0;
    do {
      lVar10 = (*DAT_0028faf0)(DAT_0028fac8,0x260);
      (&DAT_0028fb80)[uVar14] = lVar10;
      if (lVar10 == 0) {
        uVar15 = 0xffffffff;
        DAT_0028fb48 = 0xffffffff;
        FUN_00183cdc(0xffffffff,DAT_0028fb4c,"button_allocation");
        uVar1 = DAT_0028fb54 + 1;
        bVar8 = 0x7f < DAT_0028fb54;
        DAT_0028fb54 = uVar1;
        if ((bVar8) || (DAT_0028fb38 == (code *)0x0)) goto LAB_0018f718;
        pcVar11 = "nexus_menu_stopped";
        pcVar12 = "button_allocation";
        goto LAB_0018f0ec;
      }
      (*DAT_0028faf8)(DAT_0028fac8,lVar10);
      iVar9 = FUN_0018fba0(lVar10);
      if (iVar9 == 0) {
        uVar15 = 0xffffffff;
        DAT_0028fb48 = 0xffffffff;
        FUN_00183cdc(0xffffffff,DAT_0028fb4c,"button_constructor_contract");
        uVar1 = DAT_0028fb54 + 1;
        bVar8 = 0x7f < DAT_0028fb54;
        DAT_0028fb54 = uVar1;
        if ((bVar8) || (DAT_0028fb38 == (code *)0x0)) goto LAB_0018f718;
        pcVar11 = "nexus_menu_stopped";
        pcVar12 = "button_constructor_contract";
        goto LAB_0018f0ec;
      }
      uVar16 = (*DAT_0028fb00)(DAT_0028fac8,"popover_button_blue");
      local_7c[0] = 0;
      if (uVar16 == 0) {
LAB_0018f3ac:
        DAT_0028fb48 = 0xffffffff;
        FUN_00183cdc(0xffffffff,DAT_0028fb4c,"movie_type_or_frame_count");
        uVar1 = DAT_0028fb54 + 1;
        bVar8 = DAT_0028fb54 < 0x80;
        DAT_0028fb54 = uVar1;
        if ((bVar8) && (DAT_0028fb38 != (code *)0x0)) {
          pcVar11 = "movie_type_or_frame_count";
LAB_0018f458:
          (*DAT_0028fb38)(DAT_0028fac8,"nexus_menu_stopped",pcVar11,0);
        }
LAB_0018f464:
        uVar15 = 0xffffffff;
        goto LAB_0018f718;
      }
      local_78[0] = 0;
      if (uVar16 + 8 >> 3 < 0x201) {
        bVar8 = false;
      }
      else {
        iVar9 = (*DAT_0028fae0)(DAT_0028fac8,uVar16,local_78,8);
        bVar8 = iVar9 == 1;
      }
      bVar7 = false;
      if (0xfff < local_78[0]) {
        bVar7 = bVar8;
      }
      uVar13 = local_78[0];
      if (!(bool)(bVar7 & (local_78[0] & 7) == 0)) {
        uVar13 = 0;
      }
      if ((((uVar13 != DAT_0028fad0 + 0x11ad208U) || (uVar16 + 0xbe < 0x1000)) ||
          ((uVar16 & 0xfffffffffffffffe) == 0xffffffffffffff40)) ||
         ((iVar9 = (*DAT_0028fae0)(DAT_0028fac8,uVar16 + 0xbe,local_7c,2), iVar9 != 1 ||
          (local_7c[0] < 1)))) goto LAB_0018f3ac;
      (*DAT_0028fb08)(DAT_0028fac8,lVar10,uVar16);
      local_78[0] = 0;
      if (((lVar10 + 0x88U >> 3 < 0x201) ||
          (iVar9 = (*DAT_0028fae0)(DAT_0028fac8,lVar10 + 0x80,local_78,8), iVar9 != 1)) ||
         ((local_78[0] < 0x1000 ||
          (((local_78[0] & 7) != 0 || (iVar9 = FUN_0018fba0(lVar10), iVar9 == 0)))))) {
        DAT_0028fb48 = 0xffffffff;
        FUN_00183cdc(0xffffffff,DAT_0028fb4c,"movie_binding_contract");
        uVar1 = DAT_0028fb54 + 1;
        bVar8 = 0x7f < DAT_0028fb54;
        DAT_0028fb54 = uVar1;
        if ((bVar8) || (DAT_0028fb38 == (code *)0x0)) goto LAB_0018f464;
        pcVar11 = "movie_binding_contract";
        goto LAB_0018f458;
      }
      (*DAT_0028fb10)(DAT_0028fac8,uVar16,0);
      if ((((&DAT_0028fe40)[uVar14] == '\0') || (*(float *)(&DAT_0028fac0 + lVar18) != -9999.0)) ||
         (*(float *)((long)&DAT_0028fb20 + lVar18) != -9999.0)) {
        (*DAT_0028fb28)(0xc61c3c00,0xc61c3c00,DAT_0028fac8,(&DAT_0028fb80)[uVar14]);
        *(undefined4 *)(&DAT_0028fac0 + lVar18) = 0xc61c3c00;
        *(undefined4 *)((long)&DAT_0028fb20 + lVar18) = 0xc61c3c00;
        (&DAT_0028fe40)[uVar14] = 1;
      }
      iVar9 = FUN_0018f988(uVar14 & 0xffffffff,&DAT_00134f22);
      if (iVar9 != 1) goto LAB_0018f464;
      uVar14 = uVar14 + 1;
      lVar18 = lVar18 + 4;
    } while (uVar14 != 0x18);
    uVar1 = DAT_0028fb54 + 1;
    bVar8 = DAT_0028fb54 < 0x80;
    DAT_0028fb54 = uVar1;
    if ((bVar8) && (DAT_0028fb38 != (code *)0x0)) {
      (*DAT_0028fb38)(DAT_0028fac8,"nexus_menu_constructed","24_0x260_frame0_buttons",0);
    }
    local_78[0] = 0;
    if (DAT_0028fad0 + 0x12eb9f8U >> 3 < 0x201) {
      bVar8 = false;
    }
    else {
      iVar9 = (*DAT_0028fae0)(DAT_0028fac8,DAT_0028fad0 + 0x12eb9f0,local_78,8);
      bVar8 = iVar9 == 1;
    }
    bVar7 = false;
    if (0xfff < local_78[0]) {
      bVar7 = bVar8;
    }
    uVar14 = local_78[0];
    if (!(bool)(bVar7 & (local_78[0] & 7) == 0)) {
      uVar14 = 0;
    }
    if (uVar14 == DAT_0028fb70) {
      local_78[0] = 0;
      if (uVar14 + 0x98 >> 3 < 0x201) {
        bVar8 = false;
      }
      else {
        iVar9 = (*DAT_0028fae0)(DAT_0028fac8,uVar14 + 0x90,local_78,8);
        bVar8 = iVar9 == 1;
      }
      bVar7 = false;
      if (0xfff < local_78[0]) {
        bVar7 = bVar8;
      }
      uVar14 = local_78[0];
      if (!(bool)(bVar7 & (local_78[0] & 7) == 0)) {
        uVar14 = 0;
      }
      if (uVar14 == DAT_0028fb78) {
        lVar18 = 0;
        do {
          (*DAT_0028fb30)(DAT_0028fac8,DAT_0028fb70,*(undefined8 *)((long)&DAT_0028fb80 + lVar18));
          iVar9 = FUN_0018f750(*(undefined8 *)((long)&DAT_0028fb80 + lVar18),DAT_0028fb78);
          if (iVar9 == 0) {
            uVar15 = 0xffffffff;
            DAT_0028fb48 = 0xffffffff;
            FUN_00183cdc(0xffffffff,DAT_0028fb4c,"stage_root_membership");
            uVar1 = DAT_0028fb54 + 1;
            bVar8 = 0x7f < DAT_0028fb54;
            DAT_0028fb54 = uVar1;
            if ((bVar8) || (DAT_0028fb38 == (code *)0x0)) goto LAB_0018f718;
            pcVar11 = "nexus_menu_stopped";
            pcVar12 = "stage_root_membership";
            goto LAB_0018f0ec;
          }
          lVar18 = lVar18 + 8;
        } while (lVar18 != 0xc0);
        DAT_0028fb48 = 2;
        FUN_00183cdc(3,DAT_0028fb4c,"attached");
        uVar15 = 1;
        uVar1 = DAT_0028fb54 + 1;
        bVar8 = 0x7f < DAT_0028fb54;
        DAT_0028fb54 = uVar1;
        if ((bVar8) || (DAT_0028fb38 == (code *)0x0)) goto LAB_0018f718;
        pcVar11 = "nexus_menu_attached";
        pcVar12 = "all_root_membership_verified";
        goto LAB_0018f0ec;
      }
    }
    uVar15 = 0xffffffff;
    DAT_0028fb48 = 0xffffffff;
    FUN_00183cdc(0xffffffff,DAT_0028fb4c,"stage_changed_during_build");
    uVar1 = DAT_0028fb54 + 1;
    bVar8 = 0x7f < DAT_0028fb54;
    DAT_0028fb54 = uVar1;
    if ((bVar8) || (DAT_0028fb38 == (code *)0x0)) goto LAB_0018f718;
    pcVar11 = "nexus_menu_stopped";
    pcVar12 = "stage_changed_during_build";
  }
  else {
    uVar15 = 0xffffffff;
    DAT_0028fb48 = 0xffffffff;
    FUN_00183cdc(0xffffffff,DAT_0028fb4c,"a10_body_guard");
    uVar1 = DAT_0028fb54 + 1;
    bVar8 = 0x7f < DAT_0028fb54;
    DAT_0028fb54 = uVar1;
    if ((bVar8) || (DAT_0028fb38 == (code *)0x0)) goto LAB_0018f718;
    pcVar11 = "nexus_menu_stopped";
    pcVar12 = "a10_body_guard";
  }
LAB_0018f0ec:
  (*DAT_0028fb38)(DAT_0028fac8,pcVar11,pcVar12,0);
LAB_0018f718:
  if (*(long *)(lVar6 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return uVar15;
}

/* ===== nexus_menu_actions @ 0018fde4 ===== */

undefined * nexus_menu_actions(undefined8 *param_1)

{
  if (param_1 != (undefined8 *)0x0) {
    *param_1 = 0xa8;
  }
  return &DAT_0019eab8;
}

/* ===== nexus_menu_settings @ 0018fdfc ===== */

undefined ** nexus_menu_settings(undefined8 *param_1)

{
  if (param_1 != (undefined8 *)0x0) {
    *param_1 = 0x76;
  }
  return &PTR_s_nexus_sx_spin_001a19f8;
}

/* ===== nexus_menu_cells @ 0018fe14 ===== */

undefined1 * nexus_menu_cells(undefined8 *param_1)

{
  if (param_1 != (undefined8 *)0x0) {
    *param_1 = 0x9a;
  }
  return &DAT_001a2508;
}

/* ===== nexus_menu_tabs @ 0018fe2c ===== */

undefined2 * nexus_menu_tabs(undefined8 *param_1)

{
  if (param_1 != (undefined8 *)0x0) {
    *param_1 = 6;
  }
  return &DAT_001a2ea8;
}

/* ===== nexus_script_port_ui_server_thread @ 00191908 ===== */

void nexus_script_port_ui_server_thread(void)

{
  syscall(0xb2);
  nexus_menu_server_thread();
  return;
}

/* ===== FUN_0019265c @ 0019265c ===== */

undefined8 FUN_0019265c(long *param_1,undefined8 param_2,int *param_3)

{
  undefined *puVar1;
  char *pcVar2;
  long lVar3;
  bool bVar4;
  int iVar5;
  int iVar6;
  uint uVar7;
  ulong uVar8;
  int *piVar9;
  long lVar10;
  long lVar11;
  long lVar12;
  undefined8 uVar13;
  char *pcVar14;
  ulong uVar15;
  long *plVar16;
  ulong uVar17;
  undefined1 auStack_4540 [8];
  long local_4538;
  undefined1 auStack_4520 [8];
  long local_4518;
  undefined1 auStack_4500 [8];
  long local_44f8;
  undefined1 auStack_44e0 [8];
  long local_44d8;
  undefined1 auStack_44c0 [8];
  long local_44b8;
  undefined1 auStack_44a0 [8];
  long local_4498;
  undefined1 auStack_4480 [8];
  long local_4478;
  undefined1 auStack_4460 [8];
  long local_4458;
  stat sStack_4440;
  long local_43b0;
  long lStack_43a8;
  undefined1 auStack_43a0 [16384];
  undefined1 auStack_3a0 [89];
  undefined1 auStack_347 [23];
  undefined8 local_330;
  long lStack_328;
  undefined8 local_320;
  undefined8 uStack_318;
  undefined8 local_310;
  undefined8 uStack_308;
  undefined8 local_300;
  undefined8 uStack_2f8;
  undefined8 local_2f0;
  undefined8 uStack_2e8;
  undefined8 local_2e0;
  undefined8 uStack_2d8;
  undefined8 local_2d0;
  undefined8 uStack_2c8;
  undefined8 local_2c0;
  undefined8 uStack_2b8;
  undefined8 local_2b0;
  undefined8 uStack_2a8;
  undefined8 local_2a0;
  undefined8 uStack_298;
  undefined8 local_290;
  undefined8 uStack_288;
  undefined8 local_280;
  undefined8 uStack_278;
  undefined8 local_270;
  undefined8 uStack_268;
  undefined8 local_260;
  undefined8 uStack_258;
  undefined8 local_250;
  undefined8 uStack_248;
  undefined8 local_240;
  undefined8 uStack_238;
  undefined1 auStack_228 [8];
  long local_220;
  char acStack_1f2 [10];
  undefined5 local_1e8;
  undefined3 uStack_1e3;
  undefined5 local_1e0;
  undefined3 auStack_1db [76];
  char acStack_a8 [8];
  long local_a0;
  long local_68;
  
  lVar3 = tpidr_el0;
  local_68 = *(long *)(lVar3 + 0x28);
  uVar8 = param_1[1];
  if (uVar8 == 0) goto LAB_001927f8;
  uVar8 = FUN_00192f58(uVar8,"libNexusEvasionRuntime69252.so",
                       "571dcf2fac82e67c84db6e11f79bbe0031dfb9508fd3974db3ce989e4a5dadf2");
  iVar6 = (int)uVar8;
  if (iVar6 < 0) goto LAB_001927f8;
  *param_3 = *param_3 + 1;
  iVar5 = fstat(iVar6,&sStack_4440);
  FUN_00191314(auStack_3a0);
  uVar17 = 0;
  bVar4 = true;
  if ((iVar5 == 0) && (sStack_4440.st_size == 0xc46e0)) {
    uVar17 = 0;
    do {
      while( true ) {
        uVar15 = 0xc46e0 - uVar17;
        if (0x3fff < uVar15) {
          uVar15 = 0x4000;
        }
        lVar10 = __pread_chk(uVar8 & 0xffffffff,auStack_43a0,uVar15,uVar17,0x4000);
        if (lVar10 < 0) break;
        if (lVar10 == 0) {
LAB_00192774:
          bVar4 = true;
          goto LAB_00192778;
        }
        FUN_00191338(auStack_3a0,auStack_43a0,lVar10);
        uVar17 = lVar10 + uVar17;
        if (0xc46df < uVar17) goto LAB_0019276c;
      }
      piVar9 = (int *)__errno();
      if (*piVar9 != 4) goto LAB_00192774;
    } while (uVar17 < 0xc46e0);
LAB_0019276c:
    bVar4 = false;
  }
LAB_00192778:
  close(iVar6);
  uVar8 = FUN_00191678(auStack_3a0,sStack_4440.__unused + 1);
  if ((((bVar4) || (uVar17 != 0xc46e0)) ||
      (((sStack_4440.__unused[1] != 0x7ce682ac2fcf1d57 ||
        sStack_4440.__unused[2] != 0xbe9bf7116edb84) || local_43b0 != 0x4d97d38f50b9df31) ||
       lStack_43a8 != -0xd52a2b56167314d)) ||
     (uVar8 = FUN_0019339c(param_1[1],auStack_228), (int)uVar8 == 0)) goto LAB_001927f8;
  uVar8 = FUN_00192f58(auStack_228,0,0);
  iVar6 = (int)uVar8;
  if (iVar6 < 0) goto LAB_001927f8;
  uStack_238 = 0;
  local_240 = 0;
  uStack_248 = 0;
  local_250 = 0;
  uStack_258 = 0;
  local_260 = 0;
  uStack_268 = 0;
  local_270 = 0;
  uStack_278 = 0;
  local_280 = 0;
  uStack_288 = 0;
  local_290 = 0;
  uStack_298 = 0;
  local_2a0 = 0;
  uStack_2a8 = 0;
  local_2b0 = 0;
  uStack_2b8 = 0;
  local_2c0 = 0;
  uStack_2c8 = 0;
  local_2d0 = 0;
  uStack_2d8 = 0;
  local_2e0 = 0;
  uStack_2e8 = 0;
  local_2f0 = 0;
  uStack_2f8 = 0;
  local_300 = 0;
  uStack_308 = 0;
  local_310 = 0;
  uStack_318 = 0;
  local_320 = 0;
  lStack_328 = 0;
  local_330 = 0;
  lVar10 = dlopen(auStack_228,6);
  if (lVar10 == 0) {
    iVar5 = FUN_0019339c(auStack_228,acStack_a8);
    if ((((iVar5 != 0) &&
         (uVar8 = readlink(acStack_a8,(char *)&local_1e8,0x13f), 0xfffffffffffffec1 < uVar8 - 0x13f)
         ) && ((*(undefined1 *)((long)&local_1e8 + uVar8) = 0, 0x17 < uVar8 &&
               ((CONCAT35(uStack_1e3,local_1e8) == 0x6e3a64666d656d2f &&
                 CONCAT53(local_1e0,uStack_1e3) == 0x2d737578656e3a64 &&
                (iVar5 = strcmp(acStack_1f2 + uVar8," (deleted)"), iVar5 == 0)))))) &&
       (uVar8 - 0x17 < 0x100)) {
      __memcpy_chk(&local_330,auStack_1db,uVar8 - 0x17,0x100);
      auStack_347[uVar8] = 0;
      uVar8 = __strlen_chk(&local_330,0x100);
      if ((0x41 < uVar8) && (lVar10 = dlopen((long)&local_2f0 + 1,6), lVar10 != 0))
      goto LAB_001928bc;
    }
    pcVar14 = (char *)dlerror();
    uVar8 = __strlen_chk(&local_330,0x100);
    puVar1 = (undefined *)((long)&local_2f0 + 1);
    if (uVar8 < 0x42) {
      puVar1 = &DAT_00134f22;
    }
    pcVar2 = "unknown";
    if (pcVar14 != (char *)0x0) {
      pcVar2 = pcVar14;
    }
    __android_log_print(6,"NexusMem","resident lookup failed descriptor=%s soname=%s error=%s",
                        auStack_228,puVar1,pcVar2);
    uVar7 = close(iVar6);
    uVar8 = (ulong)uVar7;
  }
  else {
LAB_001928bc:
    close(iVar6);
    lVar11 = dlsym(lVar10,"nexus_script_port_query");
    lVar12 = dlsym(lVar10,"nexus_script_port_set");
    if ((((lVar11 != 0) && (lVar12 != 0)) && (iVar6 = dladdr(lVar11,&local_1e8), iVar6 != 0)) &&
       (((iVar6 = dladdr(lVar12,&local_330), iVar6 != 0 &&
         (CONCAT35(auStack_1db[0],local_1e0) == *param_1)) &&
        (lStack_328 == CONCAT35(auStack_1db[0],local_1e0))))) {
      *(long *)(param_3 + 2) = lVar11;
      *(long *)(param_3 + 4) = lVar12;
      lVar11 = dlsym(lVar10,"nexus_script_port_chat_snapshot");
      if (((lVar11 != 0) && (iVar6 = dladdr(lVar11,acStack_a8), iVar6 != 0)) &&
         (local_a0 == CONCAT35(auStack_1db[0],local_1e0))) {
        *(long *)(param_3 + 6) = lVar11;
      }
      lVar11 = dlsym(lVar10,"nexus_script_port_reset");
      if (((lVar11 != 0) && (iVar6 = dladdr(lVar11,auStack_228), iVar6 != 0)) &&
         (local_220 == CONCAT35(auStack_1db[0],local_1e0))) {
        *(long *)(param_3 + 10) = lVar11;
      }
      lVar11 = dlsym(lVar10,"nexus_script_port_hud_snapshot");
      if (((lVar11 != 0) && (iVar6 = dladdr(lVar11,auStack_4460), iVar6 != 0)) &&
         (local_4458 == CONCAT35(auStack_1db[0],local_1e0))) {
        *(long *)(param_3 + 8) = lVar11;
      }
      uVar13 = FUN_00193324(lVar10,"nexus_script_port_battle_snapshot",
                            CONCAT35(auStack_1db[0],local_1e0));
      *(undefined8 *)(param_3 + 0xe) = uVar13;
      lVar11 = dlsym(lVar10,"nexus_script_port_chat_action");
      if (((lVar11 != 0) && (iVar6 = dladdr(lVar11,auStack_4480), iVar6 != 0)) &&
         (local_4478 == CONCAT35(auStack_1db[0],local_1e0))) {
        *(long *)(param_3 + 0xc) = lVar11;
      }
      lVar11 = dlsym(lVar10,"nexus_script_port_server_snapshot");
      if (((lVar11 != 0) && (iVar6 = dladdr(lVar11,auStack_44a0), iVar6 != 0)) &&
         (local_4498 == CONCAT35(auStack_1db[0],local_1e0))) {
        *(long *)(param_3 + 0x10) = lVar11;
      }
      lVar11 = dlsym(lVar10,"nexus_script_port_server_select");
      if (((lVar11 != 0) && (iVar6 = dladdr(lVar11,auStack_44c0), iVar6 != 0)) &&
         (local_44b8 == CONCAT35(auStack_1db[0],local_1e0))) {
        *(long *)(param_3 + 0x12) = lVar11;
      }
      lVar11 = dlsym(lVar10,"nexus_script_port_camera_snapshot");
      if (((lVar11 != 0) && (iVar6 = dladdr(lVar11,auStack_44e0), iVar6 != 0)) &&
         (local_44d8 == CONCAT35(auStack_1db[0],local_1e0))) {
        *(long *)(param_3 + 0x14) = lVar11;
      }
      lVar11 = dlsym(lVar10,"nexus_script_port_camera_control");
      if (((lVar11 != 0) && (iVar6 = dladdr(lVar11,auStack_4500), iVar6 != 0)) &&
         (local_44f8 == CONCAT35(auStack_1db[0],local_1e0))) {
        *(long *)(param_3 + 0x16) = lVar11;
      }
      lVar11 = dlsym(lVar10,"nexus_script_port_fast_replay_snapshot");
      if (((lVar11 != 0) && (iVar6 = dladdr(lVar11,auStack_4520), iVar6 != 0)) &&
         (local_4518 == CONCAT35(auStack_1db[0],local_1e0))) {
        *(long *)(param_3 + 0x18) = lVar11;
      }
      lVar11 = dlsym(lVar10,"nexus_script_port_fast_replay_claim");
      if (((lVar11 != 0) && (iVar6 = dladdr(lVar11,auStack_4540), iVar6 != 0)) &&
         (local_4538 == CONCAT35(auStack_1db[0],local_1e0))) {
        *(long *)(param_3 + 0x1a) = lVar11;
      }
      if ((*(long *)(param_3 + 0x18) == 0) || (*(long *)(param_3 + 0x1a) == 0)) {
        *(long *)(param_3 + 0x18) = 0;
        param_3[0x1a] = 0;
        param_3[0x1b] = 0;
      }
      if ((*(long *)(param_3 + 0x14) == 0) || (*(long *)(param_3 + 0x16) == 0)) {
        *(long *)(param_3 + 0x14) = 0;
        param_3[0x16] = 0;
        param_3[0x17] = 0;
      }
      if ((*(long *)(param_3 + 0x10) == 0) || (*(long *)(param_3 + 0x12) == 0)) {
        *(long *)(param_3 + 0x10) = 0;
        param_3[0x12] = 0;
        param_3[0x13] = 0;
      }
      lVar11 = FUN_00193324(lVar10,"nexus_script_port_theme_snapshot",
                            CONCAT35(auStack_1db[0],local_1e0));
      plVar16 = (long *)(param_3 + 0x1c);
      *plVar16 = lVar11;
      lVar11 = FUN_00193324(lVar10,"nexus_script_port_theme_command",
                            CONCAT35(auStack_1db[0],local_1e0));
      *(long *)(param_3 + 0x1e) = lVar11;
      if ((*plVar16 == 0) || (lVar11 == 0)) {
        *plVar16 = 0;
        param_3[0x1e] = 0;
        param_3[0x1f] = 0;
      }
      lVar11 = FUN_00193324(lVar10,"nexus_script_port_client_debug_snapshot",
                            CONCAT35(auStack_1db[0],local_1e0));
      plVar16 = (long *)(param_3 + 0x20);
      *plVar16 = lVar11;
      lVar11 = FUN_00193324(lVar10,"nexus_script_port_client_debug_apply",
                            CONCAT35(auStack_1db[0],local_1e0));
      *(long *)(param_3 + 0x22) = lVar11;
      if ((*plVar16 == 0) || (lVar11 == 0)) {
        *plVar16 = 0;
        param_3[0x22] = 0;
        param_3[0x23] = 0;
      }
      lVar11 = FUN_00193324(lVar10,"nexus_script_port_client_performance_query",
                            CONCAT35(auStack_1db[0],local_1e0));
      plVar16 = (long *)(param_3 + 0x24);
      *plVar16 = lVar11;
      lVar11 = FUN_00193324(lVar10,"nexus_script_port_client_performance_apply",
                            CONCAT35(auStack_1db[0],local_1e0));
      *(long *)(param_3 + 0x26) = lVar11;
      if ((*plVar16 == 0) || (lVar11 == 0)) {
        *plVar16 = 0;
        param_3[0x26] = 0;
        param_3[0x27] = 0;
      }
      lVar11 = FUN_00193324(lVar10,"nexus_script_port_profile_snapshot",
                            CONCAT35(auStack_1db[0],local_1e0));
      plVar16 = (long *)(param_3 + 0x28);
      *plVar16 = lVar11;
      uVar13 = FUN_00193324(lVar10,"nexus_script_port_profile_open",
                            CONCAT35(auStack_1db[0],local_1e0));
      *(undefined8 *)(param_3 + 0x2a) = uVar13;
      lVar11 = FUN_00193324(lVar10,"nexus_script_port_profile_set_name",
                            CONCAT35(auStack_1db[0],local_1e0));
      *(long *)(param_3 + 0x2c) = lVar11;
      if (((*plVar16 == 0) || (*(long *)(param_3 + 0x2a) == 0)) || (lVar11 == 0)) {
        *plVar16 = 0;
        param_3[0x2a] = 0;
        param_3[0x2b] = 0;
        param_3[0x2c] = 0;
        param_3[0x2d] = 0;
      }
      lVar11 = FUN_00193324(lVar10,"nexus_script_port_client_editor_snapshot",
                            CONCAT35(auStack_1db[0],local_1e0));
      plVar16 = (long *)(param_3 + 0x2e);
      *plVar16 = lVar11;
      uVar13 = FUN_00193324(lVar10,"nexus_script_port_client_editor_apply",
                            CONCAT35(auStack_1db[0],local_1e0));
      *(undefined8 *)(param_3 + 0x30) = uVar13;
      lVar11 = FUN_00193324(lVar10,"nexus_script_port_client_editor_tick",
                            CONCAT35(auStack_1db[0],local_1e0));
      *(long *)(param_3 + 0x32) = lVar11;
      if (((*plVar16 == 0) || (*(long *)(param_3 + 0x30) == 0)) || (lVar11 == 0)) {
        *plVar16 = 0;
        param_3[0x30] = 0;
        param_3[0x31] = 0;
        param_3[0x32] = 0;
        param_3[0x33] = 0;
      }
      lVar11 = FUN_00193324(lVar10,"nexus_script_port_font_query",CONCAT35(auStack_1db[0],local_1e0)
                           );
      plVar16 = (long *)(param_3 + 0x34);
      *plVar16 = lVar11;
      uVar13 = FUN_00193324(lVar10,"nexus_script_port_font_apply",CONCAT35(auStack_1db[0],local_1e0)
                           );
      *(undefined8 *)(param_3 + 0x36) = uVar13;
      lVar11 = FUN_00193324(lVar10,"nexus_script_port_font_body_current",
                            CONCAT35(auStack_1db[0],local_1e0));
      *(long *)(param_3 + 0x38) = lVar11;
      if (((*plVar16 == 0) || (*(long *)(param_3 + 0x36) == 0)) || (lVar11 == 0)) {
        *plVar16 = 0;
        param_3[0x36] = 0;
        param_3[0x37] = 0;
        param_3[0x38] = 0;
        param_3[0x39] = 0;
      }
    }
    uVar8 = dlclose(lVar10);
  }
LAB_001927f8:
  if (*(long *)(lVar3 + 0x28) == local_68) {
    return 0;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar8);
}

/* ===== FUN_001939e0 @ 001939e0 ===== */

int FUN_001939e0(int param_1,int param_2,int *param_3)

{
  char cVar1;
  bool bVar2;
  int iVar3;
  
  if (DAT_002a1f88 != '\0') {
    iVar3 = *param_3;
    if (iVar3 == param_1) {
      *param_3 = param_2;
    }
    return iVar3;
  }
  do {
    iVar3 = *param_3;
    if (*param_3 != param_1) {
      return iVar3;
    }
    cVar1 = '\x01';
    bVar2 = (bool)ExclusiveMonitorPass(param_3,0x10);
    if (bVar2) {
      *param_3 = param_2;
      cVar1 = ExclusiveMonitorsStatus();
    }
  } while (cVar1 != '\0');
  return iVar3;
}

/* ===== FUN_00193f80 @ 00193f80 ===== */

undefined1 FUN_00193f80(undefined1 param_1,undefined1 *param_2)

{
  undefined1 uVar1;
  char cVar2;
  bool bVar3;
  
  if (DAT_002a1f88 == '\0') {
    do {
      uVar1 = *param_2;
      cVar2 = '\x01';
      bVar3 = (bool)ExclusiveMonitorPass(param_2,0x10);
      if (bVar3) {
        *param_2 = param_1;
        cVar2 = ExclusiveMonitorsStatus();
      }
    } while (cVar2 != '\0');
    return uVar1;
  }
  LOAcquire();
  uVar1 = *param_2;
  *param_2 = param_1;
  return uVar1;
}

/* ===== FUN_00193fb0 @ 00193fb0 ===== */

undefined4 FUN_00193fb0(undefined4 param_1,undefined4 *param_2)

{
  char cVar1;
  bool bVar2;
  undefined4 uVar3;
  
  if (DAT_002a1f88 == '\0') {
    do {
      uVar3 = *param_2;
      cVar1 = '\x01';
      bVar2 = (bool)ExclusiveMonitorPass(param_2,0x10);
      if (bVar2) {
        *param_2 = param_1;
        cVar1 = ExclusiveMonitorsStatus();
      }
    } while (cVar1 != '\0');
    return uVar3;
  }
  uVar3 = *param_2;
  *param_2 = param_1;
  LORelease();
  return uVar3;
}

/* ===== FUN_00193fe0 @ 00193fe0 ===== */

undefined4 FUN_00193fe0(undefined4 param_1,undefined4 *param_2)

{
  char cVar1;
  bool bVar2;
  undefined4 uVar3;
  
  if (DAT_002a1f88 == '\0') {
    do {
      uVar3 = *param_2;
      cVar1 = '\x01';
      bVar2 = (bool)ExclusiveMonitorPass(param_2,0x10);
      if (bVar2) {
        *param_2 = param_1;
        cVar1 = ExclusiveMonitorsStatus();
      }
    } while (cVar1 != '\0');
    return uVar3;
  }
  LOAcquire();
  uVar3 = *param_2;
  *param_2 = param_1;
  LORelease();
  return uVar3;
}

/* ===== FUN_00194010 @ 00194010 ===== */

int FUN_00194010(int param_1,int *param_2)

{
  char cVar1;
  bool bVar2;
  int iVar3;
  
  if (DAT_002a1f88 == '\0') {
    do {
      iVar3 = *param_2;
      cVar1 = '\x01';
      bVar2 = (bool)ExclusiveMonitorPass(param_2,0x10);
      if (bVar2) {
        *param_2 = iVar3 + param_1;
        cVar1 = ExclusiveMonitorsStatus();
      }
    } while (cVar1 != '\0');
    return iVar3;
  }
  iVar3 = *param_2;
  *param_2 = iVar3 + param_1;
  LORelease();
  return iVar3;
}

/* ===== nexus_menu_server_thread @ 001940b0 ===== */

void nexus_menu_server_thread(void)

{
  (*(code *)PTR_nexus_menu_server_thread_001a3760)();
  return;
}

/* ===== nexus_menu_ui_state @ 00194160 ===== */

void nexus_menu_ui_state(void)

{
  (*(code *)PTR_nexus_menu_ui_state_001a37b8)();
  return;
}

/* ===== nexus_menu_setting_value @ 001941e0 ===== */

void nexus_menu_setting_value(void)

{
  (*(code *)PTR_nexus_menu_setting_value_001a37f8)();
  return;
}

/* ===== nexus_menu_init @ 001942e0 ===== */

void nexus_menu_init(void)

{
  (*(code *)PTR_nexus_menu_init_001a3878)();
  return;
}

/* ===== nexus_menu_register_storage @ 00194320 ===== */

void nexus_menu_register_storage(void)

{
  (*(code *)PTR_nexus_menu_register_storage_001a3898)();
  return;
}

/* ===== nexus_menu_register_backend @ 00194330 ===== */

void nexus_menu_register_backend(void)

{
  (*(code *)PTR_nexus_menu_register_backend_001a38a0)();
  return;
}

/* ===== nexus_menu_status @ 001944a0 ===== */

void nexus_menu_status(void)

{
  (*(code *)PTR_nexus_menu_status_001a3958)();
  return;
}

/* ===== nexus_menu_start @ 00194520 ===== */

void nexus_menu_start(void)

{
  (*(code *)PTR_nexus_menu_start_001a3998)();
  return;
}

/* ===== nexus_menu_import @ 00194650 ===== */

void nexus_menu_import(void)

{
  (*(code *)PTR_nexus_menu_import_001a3a30)();
  return;
}

/* ===== nexus_menu_scroll_battle @ 00194660 ===== */

void nexus_menu_scroll_battle(void)

{
  (*(code *)PTR_nexus_menu_scroll_battle_001a3a38)();
  return;
}

/* ===== nexus_menu_server_open @ 00194670 ===== */

void nexus_menu_server_open(void)

{
  (*(code *)PTR_nexus_menu_server_open_001a3a40)();
  return;
}

/* ===== nexus_menu_server_action @ 00194680 ===== */

void nexus_menu_server_action(void)

{
  (*(code *)PTR_nexus_menu_server_action_001a3a48)();
  return;
}

/* ===== nexus_menu_theme_action @ 001946c0 ===== */

void nexus_menu_theme_action(void)

{
  (*(code *)PTR_nexus_menu_theme_action_001a3a68)();
  return;
}

/* ===== nexus_menu_profile_open @ 001946f0 ===== */

void nexus_menu_profile_open(void)

{
  (*(code *)PTR_nexus_menu_profile_open_001a3a80)();
  return;
}

/* ===== nexus_menu_profile_action @ 00194700 ===== */

void nexus_menu_profile_action(void)

{
  (*(code *)PTR_nexus_menu_profile_action_001a3a88)();
  return;
}

/* ===== nexus_menu_dispatch @ 00194730 ===== */

void nexus_menu_dispatch(void)

{
  (*(code *)PTR_nexus_menu_dispatch_001a3aa0)();
  return;
}

/* ===== nexus_menu_theme_pump @ 00194740 ===== */

void nexus_menu_theme_pump(void)

{
  (*(code *)PTR_nexus_menu_theme_pump_001a3aa8)();
  return;
}

/* ===== nexus_menu_profile_pump @ 00194760 ===== */

void nexus_menu_profile_pump(void)

{
  (*(code *)PTR_nexus_menu_profile_pump_001a3ab8)();
  return;
}

/* ===== nexus_menu_set_value @ 001947c0 ===== */

void nexus_menu_set_value(void)

{
  (*(code *)PTR_nexus_menu_set_value_001a3ae8)();
  return;
}

/* ===== nexus_menu_set_battle @ 001947d0 ===== */

void nexus_menu_set_battle(void)

{
  (*(code *)PTR_nexus_menu_set_battle_001a3af0)();
  return;
}

/* ===== nexus_menu_view @ 001947e0 ===== */

void nexus_menu_view(void)

{
  (*(code *)PTR_nexus_menu_view_001a3af8)();
  return;
}

/* ===== nexus_menu_action_status @ 00194830 ===== */

void nexus_menu_action_status(void)

{
  (*(code *)PTR_nexus_menu_action_status_001a3b20)();
  return;
}

/* ===== nexus_menu_section_snapshot @ 00194840 ===== */

void nexus_menu_section_snapshot(void)

{
  (*(code *)PTR_nexus_menu_section_snapshot_001a3b28)();
  return;
}

/* ===== nexus_menu_section_present @ 00194850 ===== */

void nexus_menu_section_present(void)

{
  (*(code *)PTR_nexus_menu_section_present_001a3b30)();
  return;
}

/* ===== nexus_menu_main_count @ 001948f0 ===== */

void nexus_menu_main_count(void)

{
  (*(code *)PTR_nexus_menu_main_count_001a3b80)();
  return;
}

/* ===== nexus_menu_settings @ 00194960 ===== */

void nexus_menu_settings(void)

{
  (*(code *)PTR_nexus_menu_settings_001a3bb8)();
  return;
}

