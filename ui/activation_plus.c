/*
 * activation_plus — UI subsystem
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
 * Notes: Nexus+ activation UI: key input/paste/bot, plus layout, entitlement header.
 */

/* ===== FUN_00162390 @ 00162390 ===== */

void FUN_00162390(int param_1,ulong param_2)

{
  long lVar1;
  bool bVar2;
  bool bVar3;
  long *plVar4;
  char cVar5;
  int iVar6;
  long lVar7;
  long lVar8;
  char *__s;
  size_t sVar9;
  int *piVar10;
  undefined4 uVar11;
  ulong uVar12;
  undefined8 *__dest;
  char *local_378;
  undefined4 local_370;
  undefined4 local_36c;
  undefined8 local_368;
  longlong lStack_360;
  undefined8 local_358;
  undefined8 uStack_350;
  undefined8 local_348;
  undefined8 uStack_340;
  undefined8 local_338;
  undefined8 uStack_330;
  undefined8 local_328;
  undefined8 uStack_320;
  undefined8 local_318;
  undefined8 uStack_310;
  undefined8 local_308;
  undefined8 uStack_300;
  undefined8 local_2f8;
  undefined8 local_2f0;
  undefined1 auStack_26f [129];
  char local_1ee [129];
  int local_16d;
  undefined4 uStack_169;
  char local_165;
  undefined8 local_e8;
  undefined8 local_e0;
  undefined8 uStack_d8;
  undefined8 local_d0;
  undefined8 uStack_c8;
  undefined8 local_c0;
  undefined8 uStack_b8;
  undefined8 local_b0;
  undefined8 uStack_a8;
  undefined8 local_a0;
  undefined8 uStack_98;
  undefined8 local_90;
  undefined8 uStack_88;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined8 local_70;
  long local_68;
  
  lVar1 = tpidr_el0;
  local_68 = *(long *)(lVar1 + 0x28);
  if ((param_1 == 0) || (DAT_0025cb3c != 0)) {
    if ((param_1 == 0) &&
       (((DAT_0025cb3c != 0 && (local_2f0 = (long *)0x0, DAT_0022f4c8 != (long *)0x0)) &&
        (DAT_0022f4e8 != 0)))) {
      iVar6 = (**(code **)(*DAT_0022f4c8 + 0x30))(DAT_0022f4c8,&local_2f0,0x10006);
      if (iVar6 == 0) {
        bVar2 = true;
      }
      else {
        if ((iVar6 != -2) ||
           (iVar6 = (**(code **)(*DAT_0022f4c8 + 0x20))(DAT_0022f4c8,&local_2f0,0), iVar6 != 0))
        goto LAB_001623e4;
        bVar2 = false;
      }
      plVar4 = local_2f0;
      if ((local_2f0 != (long *)0x0) && (DAT_0022f4e0 != 0)) {
        (**(code **)(*local_2f0 + 0x468))(local_2f0,DAT_0022f4e8);
        cVar5 = (**(code **)(*plVar4 + 0x720))(plVar4);
        if (cVar5 != '\0') {
          (**(code **)(*plVar4 + 0x88))(plVar4);
        }
      }
      if (!bVar2) {
        (**(code **)(*DAT_0022f4c8 + 0x28))();
      }
    }
  }
  else {
    DAT_0025cb20 = 0;
    DAT_0025ca40 = param_2;
  }
LAB_001623e4:
  DAT_0025cb3c = param_1;
  if (param_2 < DAT_0025cb20) goto LAB_001624a8;
  DAT_0025cb20 = param_2 + 0xfa;
  local_70 = 0;
  local_2f0 = (long *)0x0;
  local_e0 = 0;
  uStack_78 = 0;
  local_80 = 0;
  uStack_88 = 0;
  local_90 = 0;
  uStack_98 = 0;
  local_a0 = 0;
  uStack_a8 = 0;
  local_b0 = 0;
  uStack_b8 = 0;
  local_c0 = 0;
  uStack_c8 = 0;
  local_d0 = 0;
  local_e8 = DAT_0010f8a0;
  uStack_d8 = 0xffffffffffffffff;
  if ((DAT_0022f4c8 != (long *)0x0) && (DAT_0022f4e8 != 0)) {
    iVar6 = (**(code **)(*DAT_0022f4c8 + 0x30))(DAT_0022f4c8,&local_2f0,0x10006);
    if (iVar6 == 0) {
      bVar2 = true;
      plVar4 = local_2f0;
    }
    else {
      if ((iVar6 != -2) ||
         (iVar6 = (**(code **)(*DAT_0022f4c8 + 0x20))(DAT_0022f4c8,&local_2f0,0), iVar6 != 0))
      goto LAB_001624a0;
      bVar2 = false;
      plVar4 = local_2f0;
    }
    local_2f0 = plVar4;
    if (plVar4 != (long *)0x0) {
      iVar6 = (**(code **)(*plVar4 + 0x98))(plVar4,8);
      if (iVar6 == 0) {
        lVar7 = (**(code **)(*plVar4 + 0x390))(plVar4,DAT_0022f4e8,DAT_0022f4d0);
        memset(&local_2f0,0,0x204);
        cVar5 = (**(code **)(*plVar4 + 0x720))(plVar4);
        bVar3 = true;
        if (((cVar5 == '\0') && (lVar7 != 0)) &&
           (iVar6 = (**(code **)(*plVar4 + 0x558))(plVar4,lVar7), iVar6 == 4)) {
          uVar12 = 0;
          __dest = &local_2f0;
          do {
            lVar8 = (**(code **)(*plVar4 + 0x568))(plVar4,lVar7,uVar12 & 0xffffffff);
            if (((lVar8 == 0) || (cVar5 = (**(code **)(*plVar4 + 0x720))(plVar4), cVar5 != '\0')) ||
               ((iVar6 = (**(code **)(*plVar4 + 0x540))(plVar4,lVar8), 0x80 < iVar6 ||
                (__s = (char *)(**(code **)(*plVar4 + 0x548))(plVar4,lVar8,0), __s == (char *)0x0)))
               ) {
              bVar3 = true;
              goto LAB_00162720;
            }
            sVar9 = strlen(__s);
            memcpy(__dest,__s,sVar9 + 1);
            (**(code **)(*plVar4 + 0x550))(plVar4,lVar8,__s);
            uVar12 = uVar12 + 1;
            __dest = (undefined8 *)((long)__dest + 0x81);
          } while (uVar12 != 4);
          bVar3 = false;
        }
LAB_00162720:
        cVar5 = (**(code **)(*plVar4 + 0x720))(plVar4);
        if (cVar5 == '\0') {
          if (bVar3) goto LAB_001627f8;
          local_2f8 = 0;
          local_378 = (char *)0x0;
          uStack_300 = 0;
          local_308 = 0;
          uStack_310 = 0;
          local_318 = 0;
          uStack_320 = 0;
          local_328 = 0;
          uStack_330 = 0;
          local_338 = 0;
          uStack_340 = 0;
          local_348 = 0;
          uStack_350 = 0;
          local_358 = 0;
          lStack_360 = 0;
          local_368 = 0;
          local_370 = 0x80;
          if ((short)local_2f0 == 0x31) {
            local_36c = 1;
          }
          else if ((short)local_2f0 == 0x30) {
            local_36c = 0;
          }
          else {
            local_36c = 0xfffffffe;
            if ((short)local_2f0 == 0x312d && local_2f0._2_1_ == '\0') {
              local_36c = 0xffffffff;
            }
          }
          piVar10 = (int *)__errno();
          *piVar10 = 0;
          lStack_360 = strtoll(local_1ee,&local_378,10);
          if (((*piVar10 != 0) || (local_1ee[0] == '\0')) ||
             ((local_378 == (char *)0x0 || (*local_378 != '\0')))) goto LAB_001627f8;
          uVar12 = __strlen_chk(auStack_26f,0x183);
          if (0x60 < uVar12) goto LAB_001627f8;
          lVar7 = __strlen_chk(auStack_26f,0x183);
          __memcpy_chk(&local_358,auStack_26f,lVar7 + 1,0x68);
          if (local_16d == 0x79737562 && (char)uStack_169 == '\0') {
            uVar11 = 1;
          }
          else if (CONCAT44(uStack_169,local_16d) == 0x676e6974696465) {
            uVar11 = 2;
          }
          else {
            uVar11 = 0;
            if (CONCAT44(uStack_169,local_16d) == 0x6465747065636361 && local_165 == '\0') {
              uVar11 = 3;
            }
          }
          local_368 = CONCAT44(local_368._4_4_,uVar11);
          iVar6 = nexus_rich_set_plus_state(&local_370);
          if (iVar6 != 1) goto LAB_001627f8;
        }
        else {
          (**(code **)(*plVar4 + 0x88))(plVar4);
LAB_001627f8:
          nexus_rich_set_plus_state(&local_e8);
        }
        (**(code **)(*plVar4 + 0xa0))(plVar4,0);
      }
      else {
        (**(code **)(*plVar4 + 0x88))(plVar4);
        nexus_rich_set_plus_state(&local_e8);
      }
      if (!bVar2) {
        (**(code **)(*DAT_0022f4c8 + 0x28))();
      }
      goto LAB_001624a8;
    }
  }
LAB_001624a0:
  nexus_rich_set_plus_state(&local_e8);
LAB_001624a8:
  if (*(long *)(lVar1 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

/* ===== nexus_rich_plus_layout @ 0016bc94 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 nexus_rich_plus_layout(uint *param_1,int *param_2,undefined8 param_3)

{
  undefined *puVar1;
  undefined4 uVar2;
  undefined *puVar3;
  byte bVar4;
  long lVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  int iVar9;
  void *pvVar10;
  uint uVar11;
  long lVar12;
  long lVar13;
  ulong uVar14;
  long lVar15;
  ulong uVar16;
  ulong uVar17;
  undefined4 uVar18;
  undefined4 local_15c;
  undefined1 auStack_158 [128];
  undefined1 auStack_d8 [128];
  long local_58;
  
  lVar5 = tpidr_el0;
  local_58 = *(long *)(lVar5 + 0x28);
  if (param_1 != (uint *)0x0) {
    uVar18 = 4;
    if ((((param_2 == (int *)0x0) || (*param_1 != 0x25b)) || (*param_2 != 0x80)) ||
       (((param_2[1] - 2U < 0xfffffffd || (3 < (uint)param_2[2])) ||
        (*(long *)(param_2 + 4) - 0x591c9U < 0xfffffffffffa6e36)))) goto LAB_0016c2c4;
    pvVar10 = memchr(param_2 + 6,0,0x61);
    if (pvVar10 != (void *)0x0) {
      bVar4 = *(byte *)(param_2 + 6);
      uVar11 = (uint)bVar4;
      if (bVar4 != 0) {
        uVar14 = 1;
        do {
          if (uVar11 - 0x7f < 0xffffffa1) goto LAB_0016c2c0;
          uVar11 = (uint)*(byte *)((long)param_2 + uVar14 + 0x18);
          uVar14 = (ulong)((int)uVar14 + 1);
        } while (uVar11 != 0);
      }
      uVar11 = *param_1;
      if (0x2b < uVar11) {
        uVar14 = (ulong)uVar11 - 0x2b;
        if (uVar14 < 2) {
          lVar13 = 0x2b;
        }
        else {
          uVar16 = uVar14 & 0xfffffffffffffffe;
          lVar15 = (long)param_1 + 0x1e7d;
          lVar13 = uVar16 + 0x2b;
          uVar17 = uVar16;
          do {
            *(undefined2 *)(lVar15 + -0xb1) = 0;
            uVar17 = uVar17 - 2;
            *(undefined2 *)(lVar15 + -1) = 0;
            lVar15 = lVar15 + 0x160;
          } while (uVar17 != 0);
          if (uVar14 == uVar16) goto LAB_0016bdf0;
        }
        lVar12 = (ulong)uVar11 - lVar13;
        lVar15 = (long)param_1 + lVar13 * 0xb0 + 0x3d;
        do {
          *(undefined2 *)(lVar15 + -1) = 0;
          lVar15 = lVar15 + 0xb0;
          lVar12 = lVar12 + -1;
        } while (lVar12 != 0);
      }
LAB_0016bdf0:
      param_1[0x110c] = 0;
      param_1[0x15dc] = 0;
      uVar8 = _UNK_0010fb38;
      uVar7 = _DAT_0010fb30;
      uVar6 = DAT_0010f8a8;
      param_1[0x771] = 0;
      param_1[0x772] = 0;
      *(undefined8 *)(param_1 + 0x769) = uVar6;
      uVar6 = DAT_0010f808;
      *(undefined8 *)(param_1 + 0x76f) = uVar8;
      *(undefined8 *)(param_1 + 0x76d) = uVar7;
      *(undefined1 *)(param_1 + 0x773) = 1;
      *(undefined8 *)(param_1 + 0xc39) = uVar6;
      uVar7 = _UNK_0010fbc8;
      uVar6 = _DAT_0010fbc0;
      param_1[0xc41] = 0;
      param_1[0xc42] = 0;
      param_1[0x76c] = 1;
      *(undefined8 *)(param_1 + 0xc3f) = uVar7;
      *(undefined8 *)(param_1 + 0xc3d) = uVar6;
      uVar6 = DAT_0010f758;
      param_1[0xc3c] = 1;
      *(undefined8 *)(param_1 + 0x1109) = uVar6;
      uVar7 = _UNK_0010fb48;
      uVar6 = _DAT_0010fb40;
      *(undefined1 *)(param_1 + 0xc43) = 1;
      *(undefined8 *)(param_1 + 0x110f) = uVar7;
      *(undefined8 *)(param_1 + 0x110d) = uVar6;
      uVar6 = DAT_0010f780;
      param_1[0x1111] = 0;
      param_1[0x1112] = 0;
      *(undefined1 *)(param_1 + 0x1113) = 1;
      *(undefined8 *)(param_1 + 0x15d9) = uVar6;
      uVar7 = _UNK_0010f9e8;
      uVar6 = _DAT_0010f9e0;
      param_1[0x15e1] = 0;
      param_1[0x15e2] = 0;
      puVar1 = &UNK_00139ee0;
      *(undefined8 *)(param_1 + 0x15df) = uVar7;
      *(undefined8 *)(param_1 + 0x15dd) = uVar6;
      *(undefined1 *)(param_1 + 0x15e3) = 1;
      puVar3 = &UNK_001350a0;
      if (param_2[1] != 0) {
        puVar3 = &DAT_00139efe;
      }
      if (((param_2[1] == 1) && (puVar3 = puVar1, -1 < *(long *)(param_2 + 4))) &&
         (*(long *)(param_2 + 4) != 0)) {
        FUN_0016ba38(auStack_d8,0x80);
      }
      else {
        puVar1 = puVar3;
        FUN_0016ba38(auStack_d8,0x80);
      }
      FUN_0016ba38(auStack_158,0x80);
      iVar9 = param_2[1];
      uVar18 = 0x3fb5c28f;
      if (-1 < iVar9) {
        uVar18 = 0x3fdc28f6;
      }
      uVar2 = 0xffffd45a;
      if (-1 < iVar9) {
        uVar2 = 0xffd8dbe2;
      }
      local_15c = 0x2c;
      iVar9 = FUN_0016c388(0xc25c0000,0x434d0000,uVar18,0x414711dc,0x44200000,param_1,&local_15c,
                           puVar1,uVar2,0x30,iVar9 == 1,param_3);
      if (((((iVar9 != 0) &&
            (iVar9 = FUN_0016c388(0xc2940000,0x436b0000,0x3f800000,0x4169999a,0x442f0000,param_1,
                                  &local_15c,auStack_d8,0xffd8dbe2,0x30,0,param_3), iVar9 != 0)) &&
           (iVar9 = FUN_0016c388(0xc2940000,0x43818000,0x3f6147ae,0x415dd174,0x442f0000,param_1,
                                 &local_15c,&DAT_00138a84,0xffd8dbe2,0x40,0,param_3), iVar9 != 0))
          && ((iVar9 = FUN_0016c388(0xc2940000,0x438d8000,0x3f733333,0x41600000,0x440c0000,param_1,
                                    &local_15c,&DAT_0013bade,0xffd8dbe2,0x30,0,param_3), iVar9 != 0
              && (iVar9 = FUN_0016c388(0xc3250000,0x439f0000,0x3f6147ae,0x415dd174,0x439c0000,
                                       param_1,&local_15c,auStack_158,0xffffffff,0x24,0,param_3),
                 iVar9 != 0)))) &&
         ((iVar9 = FUN_0016c388(0x42ac0000,0x439f0000,0x3f51eb85,0x4156a258,0x42d00000,param_1,
                                &local_15c,&DAT_0013a9e5,0xffffffff,0x10,0,param_3), iVar9 != 0 &&
          (iVar9 = FUN_0016c388(0xc37a0000,0x43c93333,0x3f800000,0x41533333,0x43300000,param_1,
                                &local_15c,&DAT_0013a49c,0xffffffff,0x14,0,param_3), iVar9 != 0))))
      {
        iVar9 = FUN_0016c388(0x425c0000,0x43c53333,0x3f51eb85,0x4156a258,0x43300000,param_1,
                             &local_15c,&DAT_00138abd,0xffffffff,0x14,0,param_3);
        uVar18 = 4;
        if (iVar9 != 0) {
          iVar9 = FUN_0016c388(0x425c0000,0x43cd3333,0x3f75c28f,0x415f5555,0x43300000,param_1,
                               &local_15c,&DAT_00134a70,0xffffffff,0xc,0,param_3);
          uVar18 = 4;
          if (iVar9 != 0) {
            uVar18 = 1;
          }
        }
        goto LAB_0016c2c4;
      }
    }
  }
LAB_0016c2c0:
  uVar18 = 4;
LAB_0016c2c4:
  if (*(long *)(lVar5 + 0x28) == local_58) {
    return uVar18;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== nexus_rich_header_entitlement @ 0016c2f8 ===== */

undefined4 nexus_rich_header_entitlement(int *param_1,int param_2)

{
  bool bVar1;
  int iVar2;
  undefined4 uVar3;
  
  if ((((param_1 != (int *)0x0) && (*param_1 == 0x25b)) && (param_1[0x450] == 3)) &&
     (iVar2 = strcmp((char *)((long)param_1 + 0x116e),"+"), iVar2 == 0)) {
    if (param_2 == 1) {
      bVar1 = (char)param_1[0xf] != '\0';
    }
    else {
      bVar1 = false;
    }
    uVar3 = 4;
    if (param_2 + 1U < 3) {
      uVar3 = 1;
    }
    *(bool *)(param_1 + 0x45b) = bVar1;
    return uVar3;
  }
  return 4;
}

/* ===== nexus_rich_set_plus_state @ 001767dc ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 nexus_rich_set_plus_state(int *param_1)

{
  byte bVar1;
  void *pvVar2;
  undefined8 uVar3;
  ulong uVar4;
  uint uVar5;
  
  if ((((param_1 != (int *)0x0) && (*param_1 == 0x80)) && (0xfffffffc < param_1[1] - 2U)) &&
     (((uint)param_1[2] < 4 && (0xfffffffffffa6e35 < *(long *)(param_1 + 4) - 0x591c9U)))) {
    pvVar2 = memchr(param_1 + 6,0,0x61);
    if (pvVar2 != (void *)0x0) {
      bVar1 = *(byte *)(param_1 + 6);
      uVar5 = (uint)bVar1;
      if (bVar1 != 0) {
        uVar4 = 1;
        do {
          if (uVar5 - 0x7f < 0xffffffa1) {
            return 4;
          }
          uVar5 = (uint)*(byte *)((long)param_1 + uVar4 + 0x18);
          uVar4 = (ulong)((int)uVar4 + 1);
        } while (uVar5 != 0);
      }
      if (DAT_001a7d0c == 0) {
        return 0;
      }
      uVar3 = (*DAT_001a7cf0)(DAT_001a7cc8);
      if ((int)uVar3 == 0) {
        return uVar3;
      }
      uVar4 = FUN_00193f80(1,&DAT_001a7d08);
      if ((uVar4 & 1) != 0) {
        return 3;
      }
      DAT_001a7d08 = 0;
      DAT_001e5168 = *(undefined8 *)param_1;
      _DAT_001e5170 = *(undefined8 *)(param_1 + 2);
      _DAT_001e5178 = *(undefined8 *)(param_1 + 4);
      _DAT_001e5180 = *(undefined8 *)(param_1 + 6);
      _DAT_001e5188 = *(undefined8 *)(param_1 + 8);
      _DAT_001e5190 = *(undefined8 *)(param_1 + 10);
      _DAT_001e5198 = *(undefined8 *)(param_1 + 0xc);
      _DAT_001e51a0 = *(undefined8 *)(param_1 + 0xe);
      _DAT_001e51a8 = *(undefined8 *)(param_1 + 0x10);
      _DAT_001e51b0 = *(undefined8 *)(param_1 + 0x12);
      _DAT_001e51b8 = *(undefined8 *)(param_1 + 0x14);
      _DAT_001e51c0 = *(undefined8 *)(param_1 + 0x16);
      _DAT_001e51c8 = *(undefined8 *)(param_1 + 0x18);
      _DAT_001e51d0 = *(undefined8 *)(param_1 + 0x1a);
      _DAT_001e51d8 = *(undefined8 *)(param_1 + 0x1c);
      DAT_001e51e0 = *(undefined8 *)(param_1 + 0x1e);
      return 1;
    }
  }
  return 4;
}

/* ===== nexus_rich_set_plus_state @ 00194810 ===== */

void nexus_rich_set_plus_state(void)

{
  (*(code *)PTR_nexus_rich_set_plus_state_001a3b10)();
  return;
}

/* ===== nexus_rich_plus_layout @ 001948a0 ===== */

void nexus_rich_plus_layout(void)

{
  (*(code *)PTR_nexus_rich_plus_layout_001a3b58)();
  return;
}

/* ===== nexus_rich_header_entitlement @ 001948b0 ===== */

void nexus_rich_header_entitlement(void)

{
  (*(code *)PTR_nexus_rich_header_entitlement_001a3b60)();
  return;
}

