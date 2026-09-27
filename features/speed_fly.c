/*
 * Speed / Fly / FPS — Feature
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
 * Notes: Speed levels 1-4, fly toggle, FPS limit (144 = unlimited).
 */

/* ===== nexus_script_port_client_performance_apply @ 0013c7b0 [libNexusEvasionRuntime69252.so] ===== */

undefined8 nexus_script_port_client_performance_apply(uint param_1)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  undefined8 uVar3;
  
  puVar2 = (undefined4 *)__errno();
  uVar1 = *puVar2;
  if (param_1 < 0x91) {
    uVar3 = FUN_0013c82c();
    if ((int)uVar3 == 0) goto LAB_0013c818;
    if (((DAT_00209a78 & 1) != 0) && ((int)DAT_001dfff0 != 0)) {
      uVar3 = nexus_script_port_set("FPSLimit",param_1);
      goto LAB_0013c818;
    }
  }
  uVar3 = 0;
LAB_0013c818:
  *puVar2 = uVar1;
  return uVar3;
}

/* ===== FUN_00173740 @ 00173740 [libNexusEvasionRuntime69252.so] ===== */

undefined8 FUN_00173740(long param_1,undefined8 param_2,undefined8 param_3,long param_4)

{
  undefined4 uVar1;
  long lVar2;
  bool bVar3;
  int iVar4;
  int iVar5;
  undefined8 uVar6;
  undefined4 *puVar7;
  undefined8 uVar8;
  long lVar9;
  ulong uVar10;
  int local_b0;
  undefined4 uStack_ac;
  uint local_a4;
  long local_a0;
  int local_94;
  char *local_90;
  timespec local_88;
  long local_78;
  long lStack_70;
  long local_68;
  
  lVar2 = tpidr_el0;
  local_68 = *(long *)(lVar2 + 0x28);
  uVar6 = (*(code *)(DAT_001e0978 + 0xe7af70))();
  puVar7 = (undefined4 *)__errno();
  uVar1 = *puVar7;
  local_a0 = 0;
  iVar4 = clock_gettime(1,&local_88);
  uVar10 = 0;
  if (iVar4 == 0) {
    uVar10 = CONCAT44(local_88.tv_sec._4_4_,(undefined4)local_88.tv_sec) * 1000 +
             CONCAT44(local_88.tv_nsec._4_4_,(int)local_88.tv_nsec) / 1000000;
  }
  uVar8 = FUN_001550fc();
  bVar3 = false;
  lVar9 = 0;
  if ((((int)uVar8 != 0) && (DAT_0020f718 == param_1)) && (DAT_0020f710 == param_4)) {
    if (((DAT_00215d08 == '\x01') &&
        (uVar8 = FUN_001428fc(uVar8,DAT_00215d10,&local_90,4,0), (int)uVar8 != 0)) &&
       (((int)local_90 == DAT_00215d18 &&
        ((iVar4 = FUN_001428fc(uVar8,DAT_00215d20,&local_88,0x20), iVar4 != 0 &&
         (((CONCAT44(local_88.tv_sec._4_4_,(undefined4)local_88.tv_sec) == DAT_00215d28 &&
           CONCAT44(local_88.tv_nsec._4_4_,(int)local_88.tv_nsec) == DAT_00215d30) &&
          local_78 == DAT_00215d38) && lStack_70 == DAT_00215d40)))))) {
      local_b0 = -1;
      local_90 = "speedExploitEnabled";
      if (((int)DAT_00214938 == 1) &&
         (((((DAT_00214930 != (code *)0x0 &&
             (iVar4 = (*DAT_00214930)(&local_90,&local_88,1,&local_a0,&local_b0), iVar4 == 1)) &&
            (local_a0 != 0)) && ((local_b0 == 0 && ((int)local_88.tv_nsec == 2)))) &&
          (local_88.tv_sec._4_4_ == 1)))) {
        bVar3 = false;
        lVar9 = local_a0;
        if (((DAT_00228598 != DAT_0020f650) || (DAT_0020f668 == 0)) ||
           ((uVar10 < DAT_0020f668 ||
            (((bVar3 = false, 100 < uVar10 - DAT_0020f668 || (DAT_002285a8 == 0)) ||
             (DAT_002285ac == 0)))))) goto LAB_001738a8;
        if (DAT_002283a0 == DAT_0020f6f8) {
          bVar3 = false;
          if (((DAT_00228398 == DAT_00228598) && (DAT_00228488 == 4)) && (DAT_00228394 == 0x2e)) {
            bVar3 = DAT_002284a8 == local_a0;
          }
          goto LAB_001738a8;
        }
      }
      bVar3 = false;
      lVar9 = local_a0;
    }
    else {
      bVar3 = false;
      lVar9 = 0;
    }
  }
LAB_001738a8:
  iVar4 = FUN_0019fa1c(&DAT_0022b6c8,DAT_0020f6f8,lVar9,uVar10,bVar3);
  if (iVar4 != 0) {
    while (((iVar5 = FUN_001550fc(), iVar5 != 0 && (DAT_0020f718 == param_1)) &&
           (DAT_0020f710 == param_4))) {
      local_90 = "speedExploitEnabled";
      local_94 = -1;
      if (((((((int)DAT_00214938 != 1) || (DAT_00214930 == (code *)0x0)) ||
            (uVar8 = (*DAT_00214930)(&local_90,&local_88,1,&local_b0,&local_94), (int)uVar8 != 1))
           || ((CONCAT44(uStack_ac,local_b0) == 0 || (local_94 != 0)))) ||
          ((((int)local_88.tv_nsec != 2 ||
            ((local_88.tv_sec._4_4_ != 1 || (CONCAT44(uStack_ac,local_b0) != local_a0)))) ||
           (iVar5 = FUN_001428fc(uVar8,param_4 + 0xb8,&local_a4,4), iVar5 == 0)))) ||
         ((int)local_a4 < 0)) break;
      (*(code *)(DAT_001e0978 + 0xe7af70))
                (0,param_1,(ulong)local_a4 / 0x32,(local_a4 % 0x32) * 0x14,param_4);
      iVar4 = iVar4 + -1;
      if (iVar4 == 0) break;
    }
  }
  *puVar7 = uVar1;
  if (*(long *)(lVar2 + 0x28) == local_68) {
    return uVar6;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0019e3e8 @ 0019e3e8 [libNexusEvasionRuntime69252.so] ===== */

void FUN_0019e3e8(long *param_1,undefined8 *param_2,long param_3,long *param_4)

{
  char *__s2;
  long lVar1;
  int iVar2;
  undefined8 uVar3;
  long lVar4;
  long lVar5;
  char *__s2_00;
  long *plVar6;
  ulong local_b0;
  char acStack_a8 [64];
  long local_68;
  
  lVar1 = tpidr_el0;
  local_68 = *(long *)(lVar1 + 0x28);
  uVar3 = 0;
  if (((param_3 == 0) || (param_1 == (long *)0x0)) || (param_4 == (long *)0x0)) goto LAB_0019e534;
  lVar4 = param_4[0x13];
  if (lVar4 == *param_1) {
    lVar5 = 0;
    plVar6 = param_1;
LAB_0019e464:
    if (*param_4 == param_1[lVar5 * 3 + 2]) {
      uVar3 = 0;
      if (((param_2 == (undefined8 *)0x0) || (param_4[0x14] != *param_4)) ||
         ((uVar3 = 0, lVar4 + 8U < 0x10008 || ((code *)param_2[1] == (code *)0x0))))
      goto LAB_0019e534;
      iVar2 = (*(code *)param_2[1])(*param_2,lVar4,&local_b0,8);
      if (iVar2 == 1) {
        uVar3 = 0;
        if ((local_b0 + 0x1001 < 0x11001) || ((local_b0 & 7) != 0)) goto LAB_0019e534;
        if (local_b0 == param_1[lVar5 * 3 + 1]) {
          uVar3 = FUN_0019dbd4(param_2,*plVar6,acStack_a8);
          if ((int)uVar3 == 0) goto LAB_0019e534;
          __s2_00 = (&PTR_s_ControllerUltiProjectile_001cb280)[lVar5];
          iVar2 = strcmp(acStack_a8,__s2_00);
          if (iVar2 == 0) {
            uVar3 = FUN_0019dbd4(param_2,param_3,acStack_a8);
            if ((int)uVar3 == 0) goto LAB_0019e534;
            lVar4 = param_1[6];
            if ((param_1[lVar5 * 3 + 2] == param_3) || (lVar4 = param_3, param_1[6] == param_3)) {
              __s2 = "SpeedyProjectile";
              if (lVar4 != param_3) {
                __s2 = __s2_00;
              }
              iVar2 = strcmp(acStack_a8,__s2);
              if (iVar2 == 0) {
                uVar3 = 1;
                *param_4 = param_3;
                param_4[0x14] = param_3;
                goto LAB_0019e534;
              }
            }
          }
        }
      }
    }
  }
  else {
    plVar6 = param_1 + 3;
    if (lVar4 == *plVar6) {
      lVar5 = 1;
      goto LAB_0019e464;
    }
  }
  uVar3 = 0;
LAB_0019e534:
  if (*(long *)(lVar1 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar3);
}

