/*
 * renderer — UI subsystem
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
 * Notes: nexus_rich_* immediate-mode renderer + color/diagnostics.
 */

/* ===== nexus_script_port_ui_reload_request @ 0014d054 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 nexus_script_port_ui_reload_request(void)

{
  long lVar1;
  undefined4 uVar2;
  int iVar3;
  undefined1 auStack_50 [5];
  char local_4b;
  timespec local_48;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  uVar2 = gettid();
  iVar3 = nexus_menu_server_thread();
  if (iVar3 == 0) {
    uVar2 = 0;
  }
  else {
    iVar3 = nexus_menu_ui_state(auStack_50,uVar2);
    uVar2 = 0;
    if ((((iVar3 == 1) && (local_4b == '\0')) && ((DAT_0022d880._4_1_ & 1) == 0)) &&
       ((((DAT_0022d888 & 1) == 0 && (uVar2 = 1, (DAT_0022d88c & 1) == 0)) &&
        ((DAT_0022d890 & 1) == 0)))) {
      DAT_0022d894 = 0;
      DAT_0022d898 = 0;
      _DAT_0022d8a0 = 0;
      iVar3 = clock_gettime(1,&local_48);
      DAT_0022d8a8 = 0;
      if (iVar3 == 0) {
        DAT_0022d8a8 = local_48.tv_sec * 1000 + (ulong)local_48.tv_nsec / 1000000;
      }
      DAT_0022d88c = 1;
    }
  }
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return uVar2;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== nexus_release_ui_frame_v1 @ 0014fc7c ===== */

bool nexus_release_ui_frame_v1(long param_1)

{
  bool bVar1;
  undefined4 *puVar2;
  
  puVar2 = (undefined4 *)__errno();
  if ((DAT_0022d8e0 + 0x7316b8 == param_1) && ((int)DAT_0022db78 != 0)) {
    bVar1 = DAT_0022db70 != 0;
  }
  else {
    bVar1 = false;
  }
  *puVar2 = *puVar2;
  return bVar1;
}

/* ===== nexus_script_port_ui_graphics_cycle @ 001500c0 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 nexus_script_port_ui_graphics_cycle(uint param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;
  undefined4 uVar3;
  long lVar4;
  int iVar5;
  undefined4 *puVar6;
  undefined4 uVar7;
  long local_d0;
  undefined8 local_c8;
  code *local_c0;
  code *pcStack_b8;
  code *local_b0;
  undefined4 local_a8;
  int iStack_a4;
  int local_a0;
  undefined8 local_98;
  int local_90;
  long local_88;
  int local_80;
  int local_7c;
  char local_78;
  long local_70;
  int local_68;
  int local_64;
  char local_60;
  long local_58;
  
  lVar4 = tpidr_el0;
  local_58 = *(long *)(lVar4 + 0x28);
  puVar6 = (undefined4 *)__errno();
  uVar7 = 0;
  uVar3 = *puVar6;
  if ((((((1 < (param_2 | param_1)) || (DAT_0022dfb0 == 0)) || (DAT_0022dfb4 != 0)) ||
       (((uVar7 = 0, (int)DAT_0022d880 != 0 || ((DAT_0022d88c & 1) != 0)) ||
        (((DAT_0022d890 & 1) != 0 || (((DAT_0022dfb8 & 1) != 0 || ((DAT_0022d880._4_4_ & 1) != 0))))
        )))) || ((DAT_0022d888 & 1) != 0)) ||
     (((DAT_0022dfbc != 0 || (DAT_0022dff4 != 0)) || (DAT_0022dff8 != 0)))) goto LAB_00150334;
  iVar5 = FUN_00150378();
  if ((iVar5 != 0) && (iVar5 = FUN_001505d8(), iVar5 != 0)) {
    local_c0 = FUN_0014e7c4;
    pcStack_b8 = FUN_001556ec;
    local_d0 = DAT_0022d8e0;
    local_c8 = 0;
    local_b0 = FUN_00155848;
    iVar5 = FUN_00150710(&local_d0,&local_70);
    uVar7 = 0;
    if ((iVar5 == 0) || (local_60 != '\0')) goto LAB_00150334;
    iVar5 = (*local_b0)(local_c8,local_70 + 0x88,8);
    if ((iVar5 != 0) && (iVar5 = (*local_b0)(local_c8,local_d0 + 0x12f03e4,1), iVar5 != 0)) {
      if (param_2 == 0) {
        uVar7 = 1;
        goto LAB_00150334;
      }
      iVar5 = FUN_00150710(&local_d0,&local_88);
      if ((((iVar5 != 0) && (local_70 == local_88)) && (local_68 == local_80)) &&
         ((local_64 == local_7c && (local_60 == local_78)))) {
        iStack_a4 = local_68;
        local_a0 = local_64;
        uVar7 = 1;
        local_a8 = 1;
        uVar1 = *(int *)(((ulong)&local_a8 | 4) + (ulong)param_1 * 4) + 1;
        uVar2 = uVar1 & 3;
        local_98 = _DAT_0022e000;
        if ((int)uVar1 < 1) {
          uVar2 = -(-uVar1 & 3);
        }
        local_90 = DAT_0022e004._4_4_;
        *(uint *)(((ulong)&local_a8 | 4) + (ulong)param_1 * 4) = uVar2;
        iVar5 = FUN_00150920(&local_a8);
        if (iVar5 == 0) {
          iVar5 = FUN_00150920(&local_98);
          if (iVar5 != 0) goto LAB_00150330;
        }
        else {
          _DAT_0022e000 = CONCAT44(iStack_a4,local_a8);
          DAT_0022dfb4 = 1;
          DAT_0022e004._4_4_ = local_a0;
          iVar5 = nexus_script_port_ui_reload_request();
          if (iVar5 != 0) goto LAB_00150334;
          DAT_0022dfb4 = 0;
          _DAT_0022e000 = local_98;
          DAT_0022e004._4_4_ = local_90;
          iVar5 = FUN_00150920(&local_98);
          uVar7 = 0;
          if (iVar5 != 0) goto LAB_00150334;
        }
        uVar7 = 0;
        DAT_0022dfb0 = 0;
        goto LAB_00150334;
      }
    }
  }
LAB_00150330:
  uVar7 = 0;
LAB_00150334:
  *puVar6 = uVar3;
  if (*(long *)(lVar4 + 0x28) != local_58) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return uVar7;
}

/* ===== FUN_001608e0 @ 001608e0 ===== */

int FUN_001608e0(long param_1,long param_2,undefined4 param_3)

{
  long lVar1;
  int iVar2;
  undefined8 uVar3;
  long lVar4;
  char *pcVar5;
  undefined8 local_a0;
  undefined8 local_98;
  long lStack_90;
  char *local_88;
  char *pcStack_80;
  code *local_78;
  code *pcStack_70;
  code *local_68;
  code *pcStack_60;
  long local_58;
  
  lVar1 = tpidr_el0;
  local_58 = *(long *)(lVar1 + 0x28);
  if ((DAT_0022f180 != 0) && ((DAT_0022f178 != param_1 || (DAT_0022db60 != param_2)))) {
    pcVar5 = "launcher_stage_changed_during_wait";
    goto LAB_00160b2c;
  }
  DAT_0022db60 = param_2;
  DAT_0022f038 = param_3;
  DAT_0022f178 = param_1;
  uVar3 = FUN_00183c78(param_3);
  if ((int)uVar3 != 0) {
    if (DAT_0025ca38 == (int *)0x0) {
      DAT_0025ca38 = (int *)nexus_rich_renderer_ops_v1();
      if (((DAT_0025ca38 == (int *)0x0) || (*DAT_0025ca38 != 1)) || (DAT_0025ca38[1] != 0x28)) {
        pcVar5 = "launcher_renderer_abi";
      }
      else {
        local_88 = "a10aeb6b4085fb2a15d969a130cd9608231998b5269a40aed41129d99624ede3";
        pcStack_80 = "2e7e6e69e839a8b790f2d96c9d540fc313d8481d232828283034c71d705ec8d4";
        local_98 = 0;
        lStack_90 = DAT_0022d8e0;
        local_78 = FUN_0014e7c4;
        pcStack_70 = FUN_0014e720;
        local_68 = FUN_00163eb8;
        pcStack_60 = FUN_00156760;
        local_a0 = DAT_0010f730;
        if (((DAT_0022f4c0 != 0) && (iVar2 = (**(code **)(DAT_0025ca38 + 2))(&local_a0), iVar2 == 1)
            ) && (uVar3 = nexus_rich_validate_native(), (int)uVar3 == 1)) goto LAB_00160968;
        pcVar5 = "launcher_native_guards";
      }
      goto LAB_00160b2c;
    }
LAB_00160968:
    iVar2 = FUN_0014e7c4(uVar3,param_1 + 0x3c,&local_a0,0x10);
    if (iVar2 != 0) {
      iVar2 = FUN_00160c3c(0,&local_a0);
      if (iVar2 != 1) goto LAB_00160b34;
      if (DAT_0022f180 == 0) {
        DAT_0022f180 = (*(code *)(DAT_0022d8e0 + 0x11a2840))(0x260);
        if (DAT_0022f180 == 0) {
          pcVar5 = "launcher_allocation";
        }
        else {
          (*(code *)(DAT_0022d8e0 + 0x887c14))();
          iVar2 = FUN_0014e5ac(DAT_0022f180);
          if ((iVar2 != 0) && (iVar2 = FUN_00164090(DAT_0022f180), iVar2 != 0)) {
            if ((DAT_0025ca48 & 1) == 0) {
              (*(code *)(DAT_0022d8e0 + 0x66ae58))(&DAT_0025ca50,"Nexus");
              DAT_0025ca48 = 1;
            }
            goto LAB_0016099c;
          }
          pcVar5 = "launcher_constructor";
        }
      }
      else {
LAB_0016099c:
        iVar2 = FUN_00160c3c(0,&local_a0);
        if (iVar2 != 1) goto LAB_00160b34;
        iVar2 = FUN_00164090(DAT_0022f180);
        if (iVar2 == 0) {
          pcVar5 = "launcher_fresh_contract";
        }
        else {
          lVar4 = FUN_00154cf0(DAT_0022d8e0 + 0x12eb9f0);
          if ((lVar4 == param_1) && (lVar4 = FUN_00154cf0(param_1 + 0x90), lVar4 == param_2)) {
            (*(code *)(DAT_0022d8e0 + 0x5988ec))(param_1,DAT_0022f180);
            iVar2 = FUN_00154d5c(DAT_0022f180,param_2);
            if (iVar2 != 0) {
              iVar2 = 1;
              DAT_0022f03c = 1;
              uVar3 = FUN_00183cdc(2,param_3,"building_rich_menu");
              FUN_00156760(uVar3,"nexus_shell_launcher_attached","current_root_verified",0);
              goto LAB_00160b34;
            }
            pcVar5 = "launcher_attachment";
          }
          else {
            pcVar5 = "launcher_stage_changed";
          }
        }
      }
LAB_00160b2c:
      iVar2 = -1;
      PTR_s_launcher_initializing_001a7c58 = pcVar5;
      goto LAB_00160b34;
    }
  }
  iVar2 = 0;
LAB_00160b34:
  if (*(long *)(lVar1 + 0x28) == local_58) {
    return iVar2;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0016205c @ 0016205c ===== */

void FUN_0016205c(undefined8 param_1,undefined4 param_2)

{
  long lVar1;
  int iVar2;
  undefined4 uVar3;
  undefined1 auStack_40 [5];
  char local_3b;
  byte local_3a;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  iVar2 = nexus_menu_ui_state(auStack_40);
  if (iVar2 == 1) {
    if (((local_3a - 0x43 < 0x32) &&
        ((1L << ((ulong)(local_3a - 0x43) & 0x3f) & 0x22006000cd001U) != 0)) && (local_3b == '\0'))
    {
      uVar3 = nexus_rich_main_row();
      nexus_menu_main_view(param_1,uVar3,param_2);
    }
    else {
      nexus_menu_view(param_1,param_2);
    }
  }
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00164450 @ 00164450 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00164450(uint param_1)

{
  uint uVar1;
  long lVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  bool bVar6;
  bool bVar7;
  bool bVar8;
  bool bVar9;
  bool bVar10;
  bool bVar11;
  int iVar12;
  ulong uVar13;
  undefined8 local_78;
  undefined8 uStack_70;
  undefined8 uStack_68;
  undefined8 uStack_60;
  undefined8 local_58;
  undefined8 uStack_50;
  long local_48;
  
  uVar3 = DAT_0010f748;
  lVar2 = tpidr_el0;
  local_48 = *(long *)(lVar2 + 0x28);
  uStack_50 = 0;
  uStack_68 = 0;
  uStack_70 = 0;
  local_58 = 0;
  uStack_60 = 0;
  local_78 = DAT_0010f748;
  iVar12 = FUN_00191df8(&local_78);
  if ((((iVar12 != 0) && ((int)local_78 == 1)) && (local_78._4_4_ == 0x30)) &&
     (((int)uStack_70 != 0 && ((uint)uStack_68 < 6)))) {
    _DAT_0025caf8 = uStack_70;
    uVar4 = _DAT_0025caf8;
    _DAT_0025caf0 = local_78;
    _DAT_0025cb08 = uStack_60;
    _DAT_0025cb00 = uStack_68;
    uVar5 = _DAT_0025cb00;
    DAT_0025cafc = (int)((ulong)uStack_70 >> 0x20);
    uRam000000000025cb18 = uStack_50;
    _DAT_0025cb10 = local_58;
    bVar6 = DAT_0025cafc != 0;
    _DAT_0025caf8 = uVar4;
    if ((bVar6) &&
       (DAT_0025cb08 = (uint)uStack_60, uVar1 = DAT_0025cb08 >> (ulong)(param_1 & 0x1f),
       (uVar1 & 1) != 0)) {
      DAT_0025cb04 = (int)((ulong)uStack_68 >> 0x20);
      bVar6 = (uint)uStack_68 == param_1;
      bVar7 = DAT_0025cb04 == 0;
      _DAT_0025cb00 = uVar5;
      uVar13 = FUN_00191e38(param_1);
      if ((int)uVar13 != 0) {
        uStack_50 = 0;
        local_78 = uVar3;
        uStack_68 = 0;
        uStack_70 = 0;
        local_58 = 0;
        uStack_60 = 0;
        iVar12 = FUN_00191df8(&local_78);
        bVar8 = iVar12 == 0;
        bVar9 = (int)local_78 != 1;
        bVar10 = local_78._4_4_ != 0x30;
        bVar11 = (int)uStack_70 == 0;
        if ((((!bVar8 && !bVar9) && !bVar10) && !bVar11) && (uint)uStack_68 < 5 ||
            (((!bVar8 && !bVar9) && !bVar10) && !bVar11) && (uint)uStack_68 == 5) {
          _DAT_0025caf8 = uStack_70;
          _DAT_0025caf0 = local_78;
          _DAT_0025cb08 = uStack_60;
          _DAT_0025cb00 = uStack_68;
          uRam000000000025cb18 = uStack_50;
          _DAT_0025cb10 = local_58;
        }
        if ((DAT_0025cb04 == 0 ||
            ((((bVar8 || bVar9) || bVar10) || bVar11) || (uint)uStack_68 >= 5) &&
            ((((bVar8 || bVar9) || bVar10) || bVar11) || (uint)uStack_68 != 5)) && (bVar6 && bVar7))
        {
          uVar13 = 1;
        }
        else {
          iVar12 = nexus_script_port_ui_reload_request();
          uVar13 = (ulong)(iVar12 != 0);
        }
      }
      goto LAB_001645ac;
    }
  }
  uVar13 = 0;
LAB_001645ac:
  if (*(long *)(lVar2 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar13);
}

/* ===== FUN_0016793c @ 0016793c ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0016793c(undefined8 param_1,long param_2)

{
  bool bVar1;
  uint uVar2;
  long lVar3;
  int iVar4;
  __pid_t _Var5;
  undefined4 uVar6;
  undefined8 uVar7;
  undefined1 *puVar8;
  long lVar9;
  char *pcVar10;
  long local_d8;
  undefined8 local_d0;
  code *local_c8;
  code *local_c0;
  code *local_b8;
  undefined1 auStack_b0 [8];
  long local_a8;
  int local_a0;
  int local_9c;
  char local_98;
  long local_90;
  undefined8 uStack_88;
  char local_80;
  timespec local_70;
  char local_60;
  long local_58;
  
  lVar3 = tpidr_el0;
  local_58 = *(long *)(lVar3 + 0x28);
  uVar7 = FUN_00167524(&DAT_00281a28,auStack_b0);
  if ((int)uVar7 == 0) goto LAB_00167da4;
  iVar4 = nexus_ui_graphics_resources(DAT_0022f178,DAT_0022db60,DAT_0022f180,0);
  uVar6 = (undefined4)DAT_0022d880;
  uVar7 = 0;
  if (iVar4 != 1) goto LAB_00167da4;
  local_c8 = FUN_0014e7c4;
  local_c0 = FUN_001556ec;
  local_d8 = DAT_0022d8e0;
  local_d0 = 0;
  local_b8 = FUN_00155848;
  if (DAT_0022dfb4 == 0) {
    iVar4 = FUN_00168028(&local_d8,param_2,(undefined4)DAT_0022d880);
LAB_00167b1c:
    if (iVar4 != 1) {
      DAT_0022dfb8 = 1;
      if (iVar4 < 0) {
        pcVar10 = "policy_rollback_unverified";
        puVar8 = &DAT_0022d888;
        goto LAB_00167c4c;
      }
      pcVar10 = "policy_unavailable";
      goto LAB_00167c54;
    }
    DAT_0022d890 = 1;
    iVar4 = clock_gettime(1,&local_70);
    if (iVar4 == 0) {
      DAT_00281a20 = local_70.tv_sec * 1000 +
                     CONCAT44(local_70.tv_nsec._4_4_,(int)local_70.tv_nsec) / 1000000;
    }
    else {
      DAT_00281a20 = 0;
    }
    DAT_0022db70 = 0;
    uVar7 = nexus_ui_graphics_resources(DAT_0022f178,DAT_0022db60,DAT_0022f180,1);
    if ((int)uVar7 == 1) {
      DAT_0022f180 = 0;
      DAT_0022f03c = 0;
      DAT_0025ca28 = 0;
      DAT_0025ca20 = 0;
      DAT_001a7c64 = 0xffffffff;
      DAT_001a7c60 = 0xffffffff;
      DAT_0025ca60 = 0;
      DAT_0025ca6c = 0;
      DAT_0025ca78 = 0;
      DAT_0025ca68 = 0;
      DAT_0022d898 = 0;
      _DAT_0022d8a0 = 0;
      DAT_00281a2c = uVar6;
      goto LAB_00167da4;
    }
    DAT_0022dfb8 = 1;
    if ((int)uVar7 < 0) {
      pcVar10 = "ui_release_unverified_restart_required";
      DAT_0022d880._4_4_ = CONCAT31(DAT_0022d880._5_3_,1);
      DAT_0022f03c = 0xffffffff;
    }
    else {
      pcVar10 = "ui_release_unavailable";
      DAT_0022d890 = 0;
    }
    uVar2 = DAT_0022f4bc + 1;
    bVar1 = DAT_0022f4bc < 0x80;
    DAT_0022f4bc = uVar2;
    if (bVar1) {
      _Var5 = getpid();
      uVar6 = gettid(_Var5);
      iVar4 = clock_gettime(0,&local_70);
      if (iVar4 == 0) {
        lVar9 = local_70.tv_sec * 1000 +
                CONCAT44(local_70.tv_nsec._4_4_,(int)local_70.tv_nsec) / 1000000;
      }
      else {
        lVar9 = 0;
      }
      goto LAB_00167d98;
    }
  }
  else {
    puVar8 = &DAT_0022dfb8;
    pcVar10 = "policy_unavailable";
    if ((((DAT_0022dfb0 != 0) && (DAT_0022e000 != 0)) &&
        (puVar8 = &DAT_0022dfb8, pcVar10 = "policy_unavailable", DAT_0022dff4 == 0)) &&
       (DAT_0022dff8 == 0)) {
      iVar4 = FUN_00150710(&local_d8,&local_70);
      if (((iVar4 != 0) && (iVar4 = 0, local_70.tv_sec == param_2)) && (local_60 == '\0')) {
        iVar4 = (*local_b8)(local_d0,param_2 + 0x88,8);
        if ((iVar4 != 0) && (iVar4 = (*local_b8)(local_d0,local_d8 + 0x12f03e4,1), iVar4 != 0)) {
          local_80 = local_60;
          local_90 = local_70.tv_sec;
          uStack_88 = DAT_0022e004;
          if ((((int)local_70.tv_nsec == (int)DAT_0022e004) &&
              (local_70.tv_nsec._4_4_ == (int)((ulong)DAT_0022e004 >> 0x20))) ||
             (((((iVar4 = (*local_c0)(local_d0,param_2 + 0x88,(ulong)&local_90 | 8,8), iVar4 != 0 &&
                 (iVar4 = FUN_00150710(&local_d8,&local_a8), iVar4 != 0)) && (local_a8 == local_90))
               && ((local_a0 == (int)uStack_88 && (local_9c == uStack_88._4_4_)))) &&
              (local_98 == local_80)))) {
            iVar4 = FUN_00168028(&local_d8,param_2,uVar6);
            if (iVar4 != 1) {
              if (-1 < iVar4) goto LAB_00167e48;
              goto LAB_00167e60;
            }
            DAT_0022dfb4 = 0;
            iVar4 = 1;
          }
          else {
LAB_00167e48:
            iVar4 = FUN_00168410(&local_d8,&local_70);
            if (iVar4 == 0) {
LAB_00167e60:
              iVar4 = -1;
              DAT_0022dff8 = 1;
            }
            else {
              iVar4 = 0;
            }
          }
        }
      }
      goto LAB_00167b1c;
    }
LAB_00167c4c:
    *puVar8 = 1;
LAB_00167c54:
    uVar2 = DAT_0022f4bc + 1;
    bVar1 = DAT_0022f4bc < 0x80;
    DAT_0022f4bc = uVar2;
    if (bVar1) {
      _Var5 = getpid();
      uVar6 = gettid(_Var5);
      iVar4 = clock_gettime(0,&local_70);
      if (iVar4 == 0) {
        lVar9 = local_70.tv_sec * 1000 +
                CONCAT44(local_70.tv_nsec._4_4_,(int)local_70.tv_nsec) / 1000000;
      }
      else {
        lVar9 = 0;
      }
LAB_00167d98:
      __android_log_print(4,"NexusLab69252",
                          "{\"stage\":\"%s\",\"reason\":\"%s\",\"action\":%u,\"pid\":%d,\"tid\":%d,\"time_ms\":%llu}"
                          ,"performance_graphics",pcVar10,0x20002,_Var5,uVar6,lVar9);
    }
  }
  uVar7 = 0;
LAB_00167da4:
  if (*(long *)(lVar3 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar7);
}

/* ===== nexus_rich_asset @ 0016a1dc ===== */

undefined * nexus_rich_asset(uint param_1)

{
  if (param_1 < 7) {
    return &DAT_0013cd7c + *(int *)(&DAT_0013cd7c + (ulong)param_1 * 4);
  }
  return (undefined *)0x0;
}

/* ===== nexus_rich_color @ 0016a200 ===== */

uint nexus_rich_color(int param_1,int param_2,int param_3,int param_4,ulong param_5)

{
  undefined *puVar1;
  uint uVar2;
  undefined4 uVar3;
  short sVar4;
  short sVar5;
  short sVar6;
  int iVar7;
  int iVar8;
  short sVar9;
  uint uVar10;
  uint uVar11;
  bool bVar12;
  int iVar13;
  int iVar14;
  uint uVar15;
  int iVar16;
  int iVar17;
  int iVar18;
  
  bVar12 = param_1 != 0;
  uVar15 = 0x60;
  if (bVar12) {
    uVar15 = 0xa0;
  }
  iVar16 = -7;
  if (bVar12) {
    iVar16 = -6;
  }
  iVar14 = 5;
  if (bVar12) {
    iVar14 = 0xd;
  }
  iVar18 = 0;
  if ((ulong)uVar15 != 0) {
    iVar18 = (int)((param_5 / 0x28) / (ulong)uVar15);
  }
  puVar1 = &UNK_0013cd98;
  if (bVar12) {
    puVar1 = &DAT_0013ce18;
  }
  iVar18 = (int)(param_5 / 0x28) - iVar18 * uVar15;
  iVar16 = iVar16 * param_2 + iVar14 * param_3 + iVar18;
  iVar14 = 0;
  if (uVar15 != 0) {
    iVar14 = iVar16 / (int)uVar15;
  }
  iVar16 = iVar16 - iVar14 * uVar15;
  sVar9 = ((ushort)uVar15 & (ushort)(iVar16 >> 0x1f)) + (short)iVar16;
  iVar16 = 0x10;
  if (bVar12) {
    iVar16 = 0x14;
  }
  iVar14 = 0xe;
  if (bVar12) {
    iVar14 = 0x10;
  }
  sVar4 = 0;
  if (iVar16 != 0) {
    sVar4 = (short)((int)sVar9 / iVar16);
  }
  iVar17 = 6;
  if (bVar12) {
    iVar17 = 8;
  }
  iVar13 = 7;
  iVar7 = iVar13;
  if (bVar12) {
    iVar13 = 8;
    iVar7 = 9;
  }
  iVar18 = iVar18 + iVar7 * param_3;
  iVar8 = (int)(char)((char)sVar4 + '\x01');
  sVar9 = sVar9 - sVar4 * (short)iVar16;
  iVar7 = 0;
  if (iVar17 != 0) {
    iVar7 = iVar8 / iVar17;
  }
  if (param_4 == 0) {
    param_4 = 1;
  }
  uVar15 = *(uint *)(puVar1 + (long)sVar4 * 4);
  iVar13 = (param_4 + iVar13) * 4;
  uVar3 = *(undefined4 *)(puVar1 + (long)(iVar8 - iVar7 * iVar17) * 4);
  iVar17 = 0;
  if (iVar13 != 0) {
    iVar17 = iVar18 / iVar13;
  }
  uVar10 = uVar15 >> 0x10 & 0xff;
  uVar11 = uVar15 >> 8 & 0xff;
  sVar4 = 0;
  if (iVar16 != 0) {
    sVar4 = (short)((int)(short)(sVar9 * (((ushort)((uint)uVar3 >> 0x10) & 0xff) - (short)uVar10)) /
                   iVar16);
  }
  sVar5 = 0;
  if (iVar16 != 0) {
    sVar5 = (short)((int)(short)(sVar9 * (((ushort)((uint)uVar3 >> 8) & 0xff) - (short)uVar11)) /
                   iVar16);
  }
  sVar6 = 0;
  if (iVar16 != 0) {
    sVar6 = (short)((int)(short)(sVar9 * (((ushort)uVar3 & 0xff) - (short)(uVar15 & 0xff))) / iVar16
                   );
  }
  uVar2 = iVar14 + param_2 * 4 + (iVar17 * iVar13 - iVar18);
  iVar16 = uVar10 + (int)sVar4;
  iVar14 = uVar11 + (int)sVar5;
  uVar10 = -uVar2;
  if (-1 < (int)uVar2) {
    uVar10 = uVar2;
  }
  uVar15 = (uVar15 & 0xff) + (int)sVar6;
  if (uVar10 < 9) {
    iVar18 = 0xfa;
    if (param_1 != 0) {
      iVar18 = 0xff;
    }
    iVar13 = (9 - uVar10) * 0x12;
    iVar17 = 0xcd;
    if (param_1 != 0) {
      iVar17 = 0xff;
    }
    iVar16 = (iVar13 * (0xff - iVar16)) / 0xff + iVar16;
    iVar14 = (iVar13 * (iVar18 - iVar14)) / 0xff + iVar14;
    uVar15 = (int)(iVar13 * (iVar17 - uVar15)) / 0xff + uVar15;
  }
  return iVar16 << 0x10 | iVar14 << 8 | uVar15 | 0xff000000;
}

/* ===== FUN_0016b76c @ 0016b76c ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0016b76c(float param_1,uint param_2,float param_3,uint *param_4,char *param_5,int param_6,
                 undefined4 param_7,uint param_8,undefined8 param_9)

{
  uint uVar1;
  char cVar2;
  long lVar3;
  bool bVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  uint uVar7;
  undefined8 uVar8;
  size_t sVar9;
  ulong uVar10;
  long lVar11;
  uint uVar12;
  int iVar13;
  uint *puVar14;
  uint uVar15;
  undefined4 uVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  undefined4 uVar20;
  char local_ac [4];
  long local_a8;
  
  uVar5 = DAT_0010f7e0;
  lVar3 = tpidr_el0;
  local_a8 = *(long *)(lVar3 + 0x28);
  fVar18 = 0.0;
  fVar19 = param_1 + -36.0;
  if (param_6 != 0) {
    fVar19 = param_1;
  }
  uVar15 = 0;
  bVar4 = false;
  uVar12 = 0;
  uVar20 = 0;
  while( true ) {
    while( true ) {
      uVar6 = _UNK_0010f9b8;
      uVar8 = _DAT_0010f9b0;
      cVar2 = param_5[uVar12];
      if (cVar2 != ' ') break;
      uVar12 = uVar12 + 1;
      fVar18 = (float)NEON_fmadd(param_3,0x40a00000,fVar18);
    }
    if (cVar2 == '\0') {
      if (param_8 <= uVar15) goto LAB_0016b9e4;
      uVar12 = *param_4;
      uVar10 = (ulong)uVar12;
      iVar13 = param_8 - uVar15;
      if (uVar12 < 0x281) {
        uVar12 = 0x280;
      }
      lVar11 = uVar10 * 0xb0;
      goto LAB_0016b9b0;
    }
    iVar13 = (int)cVar2;
    uVar8 = 0;
    if ((iVar13 < 0) || (iVar13 == 10 || uVar15 == param_8)) goto LAB_0016b9f0;
    uVar16 = FUN_0016cca0(param_3,iVar13);
    if (bVar4) {
      fVar17 = (float)NEON_fmadd(uVar20,0x3f000000,param_3 * 3.0);
      fVar18 = (float)NEON_fmadd(uVar16,0x3f000000,fVar17 + fVar18);
      fVar18 = fVar19 + fVar18;
    }
    else {
      fVar18 = (float)NEON_fmadd(uVar16,0x3f000000,param_1 + -36.0);
      if (param_6 != 0) {
        fVar18 = fVar19;
      }
    }
    fVar19 = fVar18;
    local_ac[0] = param_5[uVar12];
    local_ac[1] = 0;
    sVar9 = strlen(param_5);
    uVar7 = nexus_rich_color(param_6,uVar12,param_7,sVar9,param_9);
    uVar10 = __strlen_chk(local_ac,2);
    if ((0x7f < uVar10) || (uVar1 = *param_4, 0x27f < uVar1)) break;
    puVar14 = param_4 + (ulong)uVar1 * 0x2c + 4;
    *param_4 = uVar1 + 1;
    *(undefined8 *)puVar14 = uVar5;
    puVar14[5] = (uint)fVar19;
    puVar14[6] = param_2;
    *(ulong *)(puVar14 + 7) = CONCAT44(param_3,param_3);
    *(undefined1 *)(puVar14 + 0xb) = 1;
    puVar14[3] = 0xffffffff;
    puVar14[4] = 0;
    lVar11 = __strlen_chk(local_ac,2);
    memcpy((void *)((long)puVar14 + 0x2e),local_ac,lVar11 + 1);
    uVar1 = *param_4;
    fVar18 = 0.0;
    puVar14[3] = uVar7;
    bVar4 = true;
    uVar15 = uVar15 + 1;
    *(char *)((long)param_4 + (ulong)(uVar1 - 1) * 0xb0 + 0x3d) = (char)param_6 + '\x01';
    uVar12 = uVar12 + 1;
    uVar20 = uVar16;
  }
LAB_0016b9ec:
  uVar8 = 0;
  goto LAB_0016b9f0;
  while( true ) {
    uVar10 = uVar10 + 1;
    iVar13 = iVar13 + -1;
    *param_4 = (uint)uVar10;
    *(undefined8 *)((long)param_4 + lVar11 + 0x10) = uVar5;
    *(undefined8 *)((long)param_4 + lVar11 + 0x2c) = uVar6;
    *(undefined8 *)((long)param_4 + lVar11 + 0x24) = uVar8;
    *(undefined1 *)((long)param_4 + lVar11 + 0x3e) = 0;
    *(undefined8 *)((long)param_4 + lVar11 + 0x1c) = 0xffffffff;
    *(undefined1 *)((long)param_4 + lVar11 + 0x3c) = 0;
    lVar11 = lVar11 + 0xb0;
    if (iVar13 == 0) break;
LAB_0016b9b0:
    if (uVar12 == uVar10) goto LAB_0016b9ec;
  }
LAB_0016b9e4:
  uVar8 = 1;
LAB_0016b9f0:
  if (*(long *)(lVar3 + 0x28) == local_a8) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar8);
}

/* ===== FUN_0016c388 @ 0016c388 ===== */

/* WARNING: Type propagation algorithm not settling */

undefined1
FUN_0016c388(undefined4 param_1,undefined4 param_2,float param_3,undefined4 param_4,float param_5,
            uint *param_6,uint *param_7,byte *param_8,uint param_9,uint param_10,int param_11,
            undefined8 param_12)

{
  bool bVar1;
  bool bVar2;
  byte bVar3;
  bool bVar4;
  uint uVar5;
  uint uVar6;
  bool bVar7;
  uint uVar8;
  uint uVar9;
  byte *pbVar10;
  ulong uVar11;
  undefined1 uVar12;
  long lVar13;
  size_t __n;
  undefined8 *__dest;
  float fVar14;
  undefined4 uVar15;
  float fVar16;
  float fVar17;
  uint uVar18;
  undefined4 uVar19;
  
  bVar3 = *param_8;
  if (bVar3 == 0) {
    fVar14 = 0.0;
    uVar6 = 0;
  }
  else {
    uVar19 = 0x40866666;
    fVar14 = 0.0;
    uVar6 = 0;
    if (param_11 != 0) {
      uVar19 = 0x40400000;
    }
    pbVar10 = param_8;
    do {
      uVar5 = (uint)bVar3;
      if ((char)bVar3 < '\0') {
        uVar18 = uVar5 + 0x3e;
        bVar4 = 0x1d < (uVar18 & 0xff);
        bVar1 = !bVar4;
        if (bVar4) {
          if ((uVar5 & 0xf0) != 0xe0) {
            if ((uVar5 + 0xb & 0xff) < 0xfb) {
              return 0;
            }
            uVar8 = 7;
            bVar4 = true;
            lVar13 = 4;
            bVar3 = pbVar10[1];
            goto joined_r0x0016c4e0;
          }
          bVar4 = false;
          uVar8 = 0xf;
          bVar7 = true;
          lVar13 = 3;
          bVar3 = pbVar10[1];
        }
        else {
          bVar4 = false;
          uVar8 = 0x1f;
          lVar13 = 2;
          bVar3 = pbVar10[1];
joined_r0x0016c4e0:
          bVar7 = false;
        }
        if (bVar3 == 0) {
          return 0;
        }
        if ((bVar3 & 0xc0) != 0x80) {
          return 0;
        }
        uVar5 = bVar3 & 0x3f | (uVar5 & uVar8) << 6;
        if (0x1d < (uVar18 & 0xff)) {
          bVar3 = pbVar10[2];
          if (bVar3 == 0) {
            return 0;
          }
          if ((bVar3 & 0xc0) != 0x80) {
            return 0;
          }
          uVar5 = bVar3 & 0x3f | uVar5 << 6;
          if (!bVar7) {
            bVar3 = pbVar10[3];
            if (bVar3 == 0) {
              return 0;
            }
            if ((bVar3 & 0xc0) != 0x80) {
              return 0;
            }
            uVar5 = bVar3 & 0x3f | uVar5 << 6;
          }
        }
      }
      else {
        bVar4 = false;
        bVar7 = false;
        bVar1 = false;
        lVar13 = 1;
      }
      bVar2 = false;
      if (uVar5 < 0x80) {
        bVar2 = bVar1;
      }
      bVar1 = false;
      if (uVar5 < 0x800) {
        bVar1 = bVar7;
      }
      if (bVar2) {
        return 0;
      }
      if (bVar1) {
        return 0;
      }
      bVar1 = false;
      if (uVar5 < 0x10000) {
        bVar1 = bVar4;
      }
      if (bVar1) {
        return 0;
      }
      if (0x10ffff < uVar5) {
        return 0;
      }
      if ((uVar5 & 0x7ff800) == 0xd800) {
        return 0;
      }
      if (uVar5 == 0x20) {
        fVar14 = (float)NEON_fmadd(param_3,0x40a00000,fVar14);
      }
      else {
        if (uVar6 == param_10) {
          if (param_5 != 0.0 && param_5 < 0.0 == NAN(param_5)) goto LAB_0016c640;
          goto LAB_0016c664;
        }
        fVar17 = (float)NEON_fmadd(uVar19,param_3,fVar14);
        if (uVar6 != 0) {
          fVar14 = fVar17;
        }
        if (uVar5 < 0x80) {
          fVar17 = (float)FUN_0016cca0(param_3);
        }
        else {
          fVar17 = param_3 * 8.2;
          if (param_11 == 0) {
            fVar17 = 8.8;
            uVar5 = uVar5 - 0x414;
            if (uVar5 < 0x3b) {
              if ((1L << ((ulong)uVar5 & 0x3f) & 0x491010504910105U) == 0) {
                if ((1L << ((ulong)uVar5 & 0x3f) & 0x20000000200000U) != 0) {
                  fVar17 = 11.8;
                }
              }
              else {
                fVar17 = 10.8;
              }
            }
            fVar17 = fVar17 * param_3;
          }
        }
        fVar14 = fVar14 + fVar17;
        uVar6 = uVar6 + 1;
      }
      pbVar10 = pbVar10 + lVar13;
      bVar3 = *pbVar10;
    } while (bVar3 != 0);
  }
  param_10 = uVar6;
  if (param_5 != 0.0 && param_5 < 0.0 == NAN(param_5)) {
LAB_0016c640:
    if (fVar14 != param_5 && fVar14 < param_5 == (NAN(fVar14) || NAN(param_5))) {
      param_3 = (param_5 / fVar14) * param_3;
      fVar14 = param_5;
    }
  }
LAB_0016c664:
  uVar6 = (uint)*param_8;
  if ((*param_8 == 0) || (param_10 == 0)) {
    return 1;
  }
  uVar19 = 0x40866666;
  uVar15 = NEON_fmadd(param_3,0xc1a00000,param_1);
  uVar18 = NEON_fmsub(param_4,param_3,param_2);
  uVar5 = 0;
  fVar14 = (float)NEON_fmadd(fVar14,0xbf000000,uVar15);
  if (param_11 != 0) {
    uVar19 = 0x40400000;
  }
  while( true ) {
    if (uVar6 >> 7 == 0) {
      bVar4 = false;
      bVar7 = false;
      bVar1 = false;
      __n = 1;
    }
    else {
      uVar8 = uVar6 + 0x3e;
      bVar4 = 0x1d < (uVar8 & 0xff);
      bVar1 = !bVar4;
      if (bVar4) {
        if ((uVar6 & 0xf0) == 0xe0) {
          bVar4 = false;
          uVar9 = 0xf;
          bVar7 = true;
          __n = 3;
        }
        else {
          if ((uVar6 + 0xb & 0xff) < 0xfb) {
            return 0;
          }
          bVar7 = false;
          uVar9 = 7;
          bVar4 = true;
          __n = 4;
        }
      }
      else {
        bVar7 = false;
        bVar4 = false;
        uVar9 = 0x1f;
        __n = 2;
      }
      bVar3 = param_8[1];
      if (bVar3 == 0) {
        return 0;
      }
      if ((bVar3 & 0xc0) != 0x80) {
        return 0;
      }
      uVar6 = bVar3 & 0x3f | (uVar6 & uVar9) << 6;
      if (0x1d < (uVar8 & 0xff)) {
        bVar3 = param_8[2];
        if (bVar3 == 0) {
          return 0;
        }
        if ((bVar3 & 0xc0) != 0x80) {
          return 0;
        }
        uVar6 = bVar3 & 0x3f | uVar6 << 6;
        if (!bVar7) {
          bVar3 = param_8[3];
          if (bVar3 == 0) {
            return 0;
          }
          if ((bVar3 & 0xc0) != 0x80) {
            return 0;
          }
          uVar6 = bVar3 & 0x3f | uVar6 << 6;
        }
      }
    }
    bVar2 = false;
    if (uVar6 < 0x80) {
      bVar2 = bVar1;
    }
    bVar1 = false;
    if (uVar6 < 0x800) {
      bVar1 = bVar7;
    }
    if (bVar2) {
      return 0;
    }
    if (bVar1) {
      return 0;
    }
    bVar1 = false;
    if (uVar6 < 0x10000) {
      bVar1 = bVar4;
    }
    if (bVar1) {
      return 0;
    }
    if (0x10ffff < uVar6) {
      return 0;
    }
    if ((uVar6 & 0x7ff800) == 0xd800) {
      return 0;
    }
    if (uVar6 == 0x20) {
      fVar14 = (float)NEON_fmadd(param_3,0x40a00000,fVar14);
    }
    else {
      uVar8 = *param_7;
      uVar11 = (ulong)uVar8;
      if (*param_6 <= uVar8) {
        return 0;
      }
      lVar13 = (ulong)uVar8 * 0xb0 + 0x10;
      while (*(int *)((long)param_6 + lVar13) != 3) {
        uVar11 = uVar11 + 1;
        lVar13 = lVar13 + 0xb0;
        *param_7 = (uint)uVar11;
        if (*param_6 <= uVar11) {
          return 0;
        }
      }
      *param_7 = (int)uVar11 + 1;
      if (uVar6 < 0x80) {
        fVar17 = (float)FUN_0016cca0(param_3);
      }
      else {
        fVar17 = param_3 * 8.2;
        if (param_11 == 0) {
          fVar17 = 8.8;
          uVar6 = uVar6 - 0x414;
          if (uVar6 < 0x3b) {
            if ((1L << ((ulong)uVar6 & 0x3f) & 0x491010504910105U) == 0) {
              if ((1L << ((ulong)uVar6 & 0x3f) & 0x20000000200000U) != 0) {
                fVar17 = 11.8;
              }
            }
            else {
              fVar17 = 10.8;
            }
          }
          fVar17 = param_3 * fVar17;
        }
      }
      uVar11 = uVar11 & 0xffffffff;
      fVar16 = (float)NEON_fmadd(uVar19,param_3,fVar14);
      if (uVar5 != 0) {
        fVar14 = fVar16;
      }
      __dest = (undefined8 *)((long)param_6 + uVar11 * 0xb0 + 0x3e);
      *(undefined8 *)((long)param_6 + uVar11 * 0xb0 + 0x46) = 0;
      *__dest = 0;
      *(undefined8 *)((long)param_6 + uVar11 * 0xb0 + 0x66) = 0;
      *(undefined8 *)((long)param_6 + uVar11 * 0xb0 + 0x5e) = 0;
      *(undefined8 *)((long)param_6 + uVar11 * 0xb0 + 0x76) = 0;
      *(undefined8 *)((long)param_6 + uVar11 * 0xb0 + 0x6e) = 0;
      *(undefined8 *)((long)param_6 + uVar11 * 0xb0 + 0x86) = 0;
      *(undefined8 *)((long)param_6 + uVar11 * 0xb0 + 0x7e) = 0;
      *(undefined8 *)((long)param_6 + uVar11 * 0xb0 + 0x96) = 0;
      *(undefined8 *)((long)param_6 + uVar11 * 0xb0 + 0x8e) = 0;
      *(undefined8 *)((long)param_6 + uVar11 * 0xb0 + 0xa6) = 0;
      *(undefined8 *)((long)param_6 + uVar11 * 0xb0 + 0x9e) = 0;
      *(undefined8 *)((long)param_6 + uVar11 * 0xb0 + 0xb6) = 0;
      *(undefined8 *)((long)param_6 + uVar11 * 0xb0 + 0xae) = 0;
      *(undefined8 *)((long)param_6 + uVar11 * 0xb0 + 0x56) = 0;
      *(undefined8 *)((long)param_6 + uVar11 * 0xb0 + 0x4e) = 0;
      memcpy(__dest,param_8,__n);
      uVar6 = NEON_fmadd(fVar17,0x3f000000,fVar14);
      param_6[uVar11 * 0x2c + 10] = uVar18;
      *(ulong *)(param_6 + uVar11 * 0x2c + 0xb) = CONCAT44(param_3,param_3);
      param_6[uVar11 * 0x2c + 9] = uVar6;
      if (param_11 == 0) {
        uVar12 = 0;
        uVar6 = param_9;
      }
      else {
        uVar12 = 2;
        uVar6 = nexus_rich_color(1,uVar5,2,param_10,param_12);
      }
      fVar14 = fVar14 + fVar17;
      uVar5 = uVar5 + 1;
      param_6[uVar11 * 0x2c + 7] = uVar6;
      *(undefined1 *)((long)param_6 + uVar11 * 0xb0 + 0x3d) = uVar12;
      *(undefined1 *)(param_6 + uVar11 * 0x2c + 0xf) = 1;
    }
    param_8 = param_8 + __n;
    uVar6 = (uint)*param_8;
    if (uVar6 == 0) break;
    if (param_10 <= uVar5) {
      return 1;
    }
  }
  return 1;
}

/* ===== nexus_rich_launcher_geometry @ 0016c9d4 ===== */

undefined4
nexus_rich_launcher_geometry
          (float param_1,float param_2,float param_3,float param_4,float param_5,float *param_6)

{
  undefined4 uVar1;
  bool bVar2;
  bool bVar3;
  bool bVar4;
  undefined4 uVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  
  uVar5 = 4;
  if (((((ABS(param_1) != INFINITY) && (!NAN(ABS(param_1)))) && (param_6 != (float *)0x0)) &&
      ((param_2 == 8192.0 || param_2 < 8192.0 != NAN(param_2) &&
       (param_1 == 8192.0 || param_1 < 8192.0 != NAN(param_1))))) &&
     (((540.0 <= param_2 && ((960.0 <= param_1 && (ABS(param_2) != INFINITY)))) &&
      (!NAN(ABS(param_2)))))) {
    fVar7 = ABS(param_5);
    uVar5 = 4;
    if (((((fVar7 == 16384.0 || fVar7 < 16384.0 != NAN(fVar7)) &&
          (fVar6 = ABS(param_4), fVar6 == 16384.0 || fVar6 < 16384.0 != NAN(fVar6))) &&
         (fVar7 != INFINITY)) &&
        (((!NAN(fVar7) && (fVar6 != INFINITY)) &&
         ((!NAN(fVar6) && ((param_3 == 64.0 || param_3 < 64.0 != NAN(param_3) && (0.0 < param_3)))))
         ))) && ((ABS(param_3) != INFINITY && (!NAN(ABS(param_3)))))) {
      fVar7 = (float)NEON_fnmsub(param_1,0x3f000000,param_4);
      fVar6 = (float)NEON_fminnm(param_1 * 0.0009765625,param_2 / 576.0);
      uVar5 = NEON_fmadd(fVar6,0xc4100000,param_2);
      fVar8 = fVar6 / param_3;
      fVar6 = (float)NEON_fmadd(uVar5,0x3f000000,fVar6 * 30.0);
      *param_6 = fVar7 / param_3;
      param_6[2] = fVar8;
      uVar1 = 4;
      if (fVar8 <= 128.0) {
        uVar1 = 1;
      }
      bVar3 = true;
      if ((ABS(fVar8) != INFINITY) && (bVar3 = true, !NAN(ABS(fVar8)))) {
        bVar3 = false;
      }
      bVar2 = true;
      bVar4 = false;
      if (!bVar3) {
        bVar2 = false;
        bVar4 = true;
        if (!NAN(fVar8)) {
          bVar2 = fVar8 < 0.0001;
          bVar4 = false;
        }
      }
      param_6[1] = (fVar6 - param_5) / param_3;
      uVar5 = 4;
      if (bVar2 == bVar4) {
        uVar5 = uVar1;
      }
    }
  }
  return uVar5;
}

/* ===== nexus_rich_plan_stage @ 0016cb58 ===== */

ulong nexus_rich_plan_stage
                (undefined1 param_1 [16],undefined1 param_2 [16],float param_3,float param_4,
                float param_5,undefined8 param_6,undefined8 param_7,long param_8)

{
  bool bVar1;
  bool bVar2;
  bool bVar3;
  ulong uVar4;
  uint uVar5;
  float fVar6;
  float fVar7;
  
  fVar7 = ABS(param_5);
  uVar4 = 4;
  fVar6 = ABS(param_4);
  bVar1 = false;
  bVar2 = false;
  bVar3 = false;
  if (fVar7 == 16384.0 || fVar7 < 16384.0 != NAN(fVar7)) {
    bVar1 = false;
    bVar2 = false;
    bVar3 = true;
    if (!NAN(fVar6)) {
      bVar1 = fVar6 < 16384.0;
      bVar2 = fVar6 == 16384.0;
      bVar3 = false;
    }
  }
  if ((((((bVar2 || bVar1 != bVar3) && (fVar7 != INFINITY)) && (!NAN(fVar7))) &&
       ((fVar6 != INFINITY && (!NAN(fVar6))))) &&
      ((param_3 == 64.0 || param_3 < 64.0 != NAN(param_3) &&
       ((0.0 < param_3 && (ABS(param_3) != INFINITY)))))) &&
     ((!NAN(ABS(param_3)) && (uVar4 = nexus_rich_plan(param_6), (int)uVar4 == 1)))) {
    uVar4 = 4;
    fVar7 = *(float *)(param_8 + 4) / param_3;
    fVar6 = (*(float *)(param_8 + 8) - param_4) / param_3;
    param_3 = (*(float *)(param_8 + 0xc) - param_5) / param_3;
    *(float *)(param_8 + 4) = fVar7;
    *(float *)(param_8 + 8) = fVar6;
    *(float *)(param_8 + 0xc) = param_3;
    if ((((ABS(fVar7) != INFINITY) && (!NAN(ABS(fVar7)))) && (0.0001 <= fVar7)) &&
       (fVar7 == 128.0 || fVar7 < 128.0 != NAN(fVar7))) {
      param_3 = ABS(param_3);
      fVar6 = ABS(fVar6);
      bVar1 = true;
      if ((param_3 != INFINITY) && (bVar1 = true, !NAN(param_3))) {
        bVar1 = false;
      }
      bVar2 = true;
      if ((!bVar1) && (bVar2 = false, !NAN(fVar6))) {
        bVar2 = fVar6 == INFINITY;
      }
      bVar1 = true;
      if ((!bVar2) && (bVar1 = true, !NAN(fVar6))) {
        bVar1 = false;
      }
      uVar5 = 4;
      if (!bVar1) {
        uVar5 = 1;
      }
      uVar4 = (ulong)uVar5;
    }
  }
  return uVar4;
}

/* ===== FUN_0016d020 @ 0016d020 ===== */

void FUN_0016d020(char *param_1,int param_2)

{
  ulong uVar1;
  long lVar2;
  bool bVar3;
  undefined8 uVar4;
  code *pcVar5;
  ulong uVar6;
  bool bVar7;
  int iVar8;
  undefined8 uVar9;
  size_t sVar10;
  ulong uVar11;
  ulong uVar12;
  uint uVar13;
  float fVar14;
  short local_8c [2];
  undefined8 local_88;
  long local_78;
  
  lVar2 = tpidr_el0;
  local_78 = *(long *)(lVar2 + 0x28);
  if (DAT_001a7d0c == 0) {
LAB_0016d184:
    uVar9 = 0;
    goto LAB_0016d1d8;
  }
  iVar8 = (*DAT_001a7cf0)(DAT_001a7cc8);
  uVar9 = 0;
  if (((iVar8 == 0) || (DAT_001a7d28 == 0)) || (DAT_001a7d30 == 0)) goto LAB_0016d1d8;
  if (DAT_00281a50 != 0) {
    if ((DAT_00281a58 == DAT_001a7d30) &&
       (iVar8 = FUN_0016d5cc(), uVar11 = DAT_00281a50, iVar8 != 0)) {
      uVar12 = DAT_00281a60;
      if (DAT_00281a60 != 0) {
        uVar13 = 0;
        do {
          if ((uVar12 == uVar11) || (0xf < uVar13)) goto LAB_0016d154;
          local_88 = 0;
          if (uVar12 + 0x40 >> 3 < 0x201) {
            bVar7 = false;
          }
          else {
            iVar8 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar12 + 0x38,&local_88,8);
            bVar7 = iVar8 == 1;
          }
          uVar6 = local_88;
          bVar3 = false;
          if (0xfff < local_88) {
            bVar3 = bVar7;
          }
          bVar3 = (bool)(bVar3 & (local_88 & 7) == 0);
          uVar1 = local_88;
          if (!bVar3) {
            uVar1 = 0;
          }
          iVar8 = FUN_0016d5cc(uVar12,uVar1);
          if (iVar8 == 0) goto LAB_0016d18c;
          uVar13 = uVar13 + 1;
          uVar12 = uVar6;
        } while (bVar3);
        uVar12 = 0;
      }
LAB_0016d154:
      if (uVar12 == uVar11) goto LAB_0016d15c;
    }
LAB_0016d18c:
    uVar9 = 0;
    DAT_00281a68 = 1;
    goto LAB_0016d1d8;
  }
LAB_0016d15c:
  if (param_2 == 0) {
    if ((DAT_00281a50 != 0) && (DAT_00281a6c != 0)) {
      FUN_0016cf5c(0xc61c3c00,0xc61c3c00);
    }
    uVar9 = 1;
    DAT_00281a6c = 0;
    DAT_00281a70 = '\0';
    goto LAB_0016d1d8;
  }
  uVar9 = 0;
  if ((param_1 == (char *)0x0) || ((DAT_00281a68 & 1) != 0)) goto LAB_0016d1d8;
  sVar10 = strlen(param_1);
  if (0x7f < sVar10) goto LAB_0016d184;
  uVar9 = FUN_0016d910();
  if (((int)uVar9 == 0) ||
     (uVar9 = FUN_0016db8c(), pcVar5 = DAT_001a7cf8, uVar4 = DAT_001a7cc8, (int)uVar9 == 0))
  goto LAB_0016d1d8;
  if (DAT_00281a50 == 0) {
    uVar9 = nexus_rich_asset(5);
    uVar9 = (*pcVar5)(uVar4,uVar9);
    if ((int)uVar9 == 0) goto LAB_0016d1d8;
    nexus_rich_asset(5);
    uVar11 = FUN_0016ce48();
    iVar8 = FUN_0016dc20(uVar11,5,0);
    if ((iVar8 != 0) && (iVar8 = FUN_0016ddc8(uVar11), iVar8 != 0)) {
      FUN_0016ce84(uVar11,0);
      uVar12 = FUN_0016ce98(uVar11,&DAT_00134a97);
      iVar8 = FUN_0016debc(uVar12,0x11abad8);
      if ((iVar8 != 0) &&
         ((iVar8 = FUN_0016df78(uVar12), iVar8 != 0 &&
          (iVar8 = FUN_0016e020(uVar11,uVar12), iVar8 != 0)))) {
        FUN_0016ce20(uVar11,0);
        FUN_0016cf5c(0xc61c3c00,0xc61c3c00,uVar11);
        FUN_0016cfb8(DAT_001a7d28,uVar11);
        iVar8 = FUN_0016d5cc(uVar11,DAT_001a7d30);
        if ((iVar8 != 0) && (iVar8 = FUN_0016e464(uVar11,0), iVar8 != 0)) {
          DAT_00281a58 = DAT_001a7d30;
          DAT_00281a50 = uVar11;
          DAT_00281a60 = uVar12;
          goto LAB_0016d224;
        }
      }
    }
    uVar9 = 0;
    DAT_00281a68 = 1;
  }
  else {
LAB_0016d224:
    iVar8 = strcmp(param_1,&DAT_00281a74);
    if (iVar8 != 0) {
      DAT_00281a70 = '\0';
      FUN_0016ced8(&local_88,param_1);
      FUN_0016ceac(DAT_00281a60,&local_88);
      FUN_0014ec30(&local_88);
      sVar10 = strlen(param_1);
      __memcpy_chk(&DAT_00281a74,param_1,sVar10 + 1,0x80);
      FUN_0016ceec(DAT_00281a60,0xffffdf75);
    }
    bVar7 = true;
    local_8c[0] = 0;
    if ((0xfff < DAT_00281a60 + 0xb0) && ((DAT_00281a60 & 0xfffffffffffffffe) != 0xffffffffffffff4e)
       ) {
      iVar8 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_00281a60 + 0xb0,local_8c,2);
      bVar7 = iVar8 != 1;
    }
    uVar9 = 0;
    if ((((!bVar7) && (iVar8 = (int)local_8c[0], 0 < iVar8)) && (iVar8 < 0x101)) &&
       (0.0 < DAT_001e53fc)) {
      uVar9 = 0;
      fVar14 = 21.0 / (DAT_001e53fc * (float)iVar8);
      if (((ABS(fVar14) != INFINITY) && (!NAN(ABS(fVar14)))) &&
         ((0.01 <= fVar14 && (fVar14 == 8.0 || fVar14 < 8.0 != NAN(fVar14))))) {
        if (((DAT_00281a70 == '\x01') && (DAT_00281a6c != 0)) &&
           ((DAT_00281af4 == fVar14 &&
            (((DAT_00281af8 == DAT_001e53fc && (DAT_00281afc == DAT_001e5400)) &&
             (DAT_00281b00 == DAT_001e5404)))))) {
          uVar9 = 1;
        }
        else {
          DAT_00281a70 = '\0';
          FUN_0016cf00(fVar14,fVar14,DAT_00281a60);
          FUN_0016cf84(DAT_00281a60,&local_88);
          uVar9 = 0;
          if ((ABS((float)local_88) != INFINITY) && (!NAN(ABS((float)local_88)))) {
            uVar9 = 0;
            if ((ABS(local_88._4_4_) != INFINITY) && (!NAN(ABS(local_88._4_4_)))) {
              FUN_0016cf5c((20.0 - DAT_001e5400) / DAT_001e53fc - (float)local_88,
                           (55.0 - DAT_001e5404) / DAT_001e53fc - local_88._4_4_,DAT_00281a60);
              if ((DAT_00281a6c & 1) == 0) {
                FUN_0016cf5c(0,0,DAT_00281a50);
              }
              DAT_00281af8 = DAT_001e53fc;
              DAT_00281afc = DAT_001e5400;
              uVar9 = 1;
              DAT_00281b00 = DAT_001e5404;
              DAT_00281a6c = 1;
              DAT_00281a70 = '\x01';
              DAT_00281af4 = fVar14;
            }
          }
        }
      }
    }
  }
LAB_0016d1d8:
  if (*(long *)(lVar2 + 0x28) == local_78) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar9);
}

/* ===== nexus_script_port_fps_open @ 0016e618 ===== */

void nexus_script_port_fps_open(void)

{
  bool bVar1;
  uint uVar2;
  long lVar3;
  int iVar4;
  undefined8 uVar5;
  undefined8 local_98;
  ulong local_90;
  undefined8 uStack_88;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined8 uStack_70;
  undefined8 uStack_68;
  undefined8 local_60;
  undefined8 uStack_58;
  undefined8 uStack_50;
  undefined8 uStack_48;
  undefined8 local_40;
  long local_38;
  
  lVar3 = tpidr_el0;
  local_38 = *(long *)(lVar3 + 0x28);
  local_40 = 0;
  uStack_78 = 0;
  local_80 = 0;
  uStack_68 = 0;
  uStack_70 = 0;
  uStack_58 = 0;
  local_60 = 0;
  uStack_48 = 0;
  uStack_50 = 0;
  uStack_88 = 0;
  local_90 = 0;
  local_98 = DAT_0010f850;
  if (DAT_001a7cf0 != (code *)0x0) {
    uVar5 = (*DAT_001a7cf0)(DAT_001a7cc8);
    if ((((int)uVar5 == 0) || (uVar5 = FUN_0016d910(), (int)uVar5 == 0)) ||
       (uVar5 = FUN_00191c90(&local_98), (int)uVar5 == 0)) goto LAB_0016e694;
    if ((local_90 & 1) != 0) {
      if ((DAT_00282150 == 0) || (DAT_00282150 == DAT_001a7d30)) {
        DAT_00282afc = 0x91;
        if (0xffffff6f < (int)uStack_88 - 0x91U) {
          DAT_00282afc = (int)uStack_88;
        }
        DAT_00282af8 = 1;
        DAT_00282b04 = 0;
        DAT_00282b00 = DAT_00282afc;
        if ((DAT_00282ae0 != 0) && (iVar4 = FUN_0016e7c0(), iVar4 != 0)) {
          *(int *)(DAT_00282168 + 0xe8) = DAT_00282afc;
          *(undefined1 *)(DAT_00282168 + 0x140) = 0;
        }
      }
      else {
        memset(&DAT_00282150,0,0x9a8);
        DAT_00282b04 = 0;
        DAT_00282af8 = 1;
        DAT_00282afc = 0x91;
        DAT_00282b00 = DAT_00282afc;
        if (0xffffff6f < (int)uStack_88 - 0x91U) {
          DAT_00282afc = (int)uStack_88;
          DAT_00282b00 = (int)uStack_88;
        }
      }
      uVar5 = 1;
      uVar2 = DAT_00282b0c + 1;
      bVar1 = DAT_00282b0c < 0x18;
      DAT_00282b0c = uVar2;
      if ((bVar1) && (DAT_001a7d00 != (code *)0x0)) {
        (*DAT_001a7d00)(DAT_001a7cc8,"script_port_fps","popup_open",DAT_00282afc);
        uVar5 = 1;
      }
      goto LAB_0016e694;
    }
  }
  uVar5 = 0;
LAB_0016e694:
  if (*(long *)(lVar3 + 0x28) != local_38) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar5);
  }
  return;
}

/* ===== nexus_rich_initialize @ 0016e968 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 nexus_rich_initialize(int *param_1)

{
  int iVar1;
  ulong uVar2;
  undefined8 uVar3;
  
  if ((((((param_1 == (int *)0x0) || (*param_1 != 1)) || (param_1[1] != 0x48)) ||
       ((*(long *)(param_1 + 10) == 0 || (*(long *)(param_1 + 0xc) == 0)))) ||
      ((*(long *)(param_1 + 0xe) == 0 ||
       ((*(ulong *)(param_1 + 4) < 0x1000 || (*(char **)(param_1 + 6) == (char *)0x0)))))) ||
     ((iVar1 = strcmp(*(char **)(param_1 + 6),
                      "a10aeb6b4085fb2a15d969a130cd9608231998b5269a40aed41129d99624ede3"),
      iVar1 != 0 ||
      ((*(char **)(param_1 + 8) == (char *)0x0 ||
       (iVar1 = strcmp(*(char **)(param_1 + 8),
                       "2e7e6e69e839a8b790f2d96c9d540fc313d8481d232828283034c71d705ec8d4"),
       iVar1 != 0)))))) {
    uVar3 = 4;
  }
  else {
    uVar2 = FUN_00193f80(1,&DAT_001a7d08);
    if ((uVar2 & 1) == 0) {
      if (DAT_001a7d0c == 0) {
        DAT_001a7ce8 = *(undefined8 *)(param_1 + 10);
        _DAT_001a7ce0 = *(undefined8 *)(param_1 + 8);
        DAT_001a7cf8 = *(undefined8 *)(param_1 + 0xe);
        DAT_001a7cf0 = *(undefined8 *)(param_1 + 0xc);
        uVar3 = 1;
        DAT_001a7d00 = *(undefined8 *)(param_1 + 0x10);
        uRam00000000001a7cd8 = *(undefined8 *)(param_1 + 6);
        DAT_001a7cd0 = *(undefined8 *)(param_1 + 4);
        DAT_001a7cc8 = *(undefined8 *)(param_1 + 2);
        _DAT_001a7cc0 = *(undefined8 *)param_1;
        DAT_001a7d0c = 1;
        _DAT_001e5188 = 0;
        _DAT_001e5180 = 0;
        _DAT_001e5198 = 0;
        _DAT_001e5190 = 0;
        _DAT_001e51a8 = 0;
        _DAT_001e51a0 = 0;
        _DAT_001e51b8 = 0;
        _DAT_001e51b0 = 0;
        _DAT_001e51c8 = 0;
        _DAT_001e51c0 = 0;
        _DAT_001e51d8 = 0;
        _DAT_001e51d0 = 0;
        DAT_001e51e0 = 0;
        DAT_001e5168 = DAT_0010f8a0;
        _DAT_001e5178 = _UNK_0010fa88;
        _DAT_001e5170 = _DAT_0010fa80;
        PTR_s_not_initialized_001a7d50 = s_waiting_parent_mainloop_00138fd5;
        FUN_0016cde4(*(undefined8 *)(param_1 + 4));
      }
      else {
        uVar3 = 4;
      }
      DAT_001a7d08 = 0;
    }
    else {
      uVar3 = 3;
    }
  }
  return uVar3;
}

/* ===== nexus_rich_render @ 0016eac4 ===== */

void nexus_rich_render(undefined8 param_1,undefined8 param_2,int *param_3,ulong param_4,
                      ulong param_5,ulong param_6)

{
  long lVar1;
  int iVar2;
  long lVar3;
  undefined4 uVar4;
  bool bVar5;
  undefined4 uVar6;
  code *pcVar7;
  bool bVar8;
  int iVar9;
  undefined8 uVar10;
  ulong uVar11;
  undefined8 uVar12;
  char *pcVar13;
  uint uVar14;
  ushort uVar15;
  undefined1 auVar16 [16];
  ulong local_88;
  undefined1 local_80 [16];
  long local_68;
  
  lVar3 = tpidr_el0;
  local_68 = *(long *)(lVar3 + 0x28);
  uVar10 = 0;
  if ((((param_3 == (int *)0x0) || (DAT_001a7d0c == 0)) || (DAT_001a7d10 < 0)) ||
     (uVar10 = (*DAT_001a7cf0)(DAT_001a7cc8), (int)uVar10 == 0)) goto LAB_0016f4e0;
  uVar11 = FUN_00193f80(1,&DAT_001a7d08);
  if ((uVar11 & 1) != 0) {
    uVar10 = 3;
    goto LAB_0016f4e0;
  }
  if ((DAT_001a7d10 != 0) && ((DAT_001a7d28 != param_4 || (DAT_001a7d30 != param_5)))) {
    uVar10 = 0xffffffff;
    DAT_001a7d10 = -1;
    PTR_s_not_initialized_001a7d50 = s_stage_generation_0013200d;
    uVar14 = DAT_001a7d20 + 1;
    bVar8 = 0x3f < DAT_001a7d20;
    DAT_001a7d20 = uVar14;
    if ((bVar8) || (DAT_001a7d00 == (code *)0x0)) goto LAB_0016f4dc;
    pcVar13 = "stage_generation";
    goto LAB_0016ed38;
  }
  DAT_001a7d28 = param_4;
  DAT_001a7d30 = param_5;
  if ((param_4 == 0) || (param_5 == 0)) {
LAB_0016ece4:
    if (DAT_001a7d10 == 0) {
LAB_0016f4d8:
      uVar10 = 0;
    }
    else {
      uVar10 = 0xffffffff;
      DAT_001a7d10 = -1;
      PTR_s_not_initialized_001a7d50 = s_stage_root_identity_001399f8;
      uVar14 = DAT_001a7d20 + 1;
      bVar8 = DAT_001a7d20 < 0x40;
      DAT_001a7d20 = uVar14;
      if ((bVar8) && (DAT_001a7d00 != (code *)0x0)) {
        pcVar13 = "stage_root_identity";
LAB_0016ed38:
        DAT_001a7d10 = -1;
        (*DAT_001a7d00)(DAT_001a7cc8,"nexus_rich_stopped",pcVar13,0);
        uVar10 = 0xffffffff;
      }
    }
  }
  else {
    local_88 = 0;
    if (DAT_001a7cd0 + 0x12eb9f8U >> 3 < 0x201) {
      bVar8 = false;
    }
    else {
      iVar9 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_001a7cd0 + 0x12eb9f0,&local_88,8);
      bVar8 = iVar9 == 1;
    }
    bVar5 = false;
    if (0xfff < local_88) {
      bVar5 = bVar8;
    }
    uVar11 = local_88;
    if (!(bool)(bVar5 & (local_88 & 7) == 0)) {
      uVar11 = 0;
    }
    if (uVar11 != param_4) goto LAB_0016ece4;
    local_88 = 0;
    if (param_4 + 0x98 >> 3 < 0x201) {
      bVar8 = false;
    }
    else {
      iVar9 = (*DAT_001a7ce8)(DAT_001a7cc8,param_4 + 0x90,&local_88,8);
      bVar8 = iVar9 == 1;
    }
    bVar5 = false;
    if (0xfff < local_88) {
      bVar5 = bVar8;
    }
    uVar11 = local_88;
    if (!(bool)(bVar5 & (local_88 & 7) == 0)) {
      uVar11 = 0;
    }
    if (uVar11 != param_5) goto LAB_0016ece4;
    uVar10 = FUN_0016db8c();
    if ((int)uVar10 == 0) {
      PTR_s_not_initialized_001a7d50 = s_waiting_stage_bounds_idle_0013c1ca;
      goto LAB_0016f4dc;
    }
    iVar9 = FUN_0016d910();
    if (iVar9 == 0) {
      PTR_s_not_initialized_001a7d50 = s_waiting_identity_root_transform_00138af4;
      if (DAT_001a7d10 != 0) {
        uVar10 = 0xffffffff;
        DAT_001a7d10 = -1;
        PTR_s_not_initialized_001a7d50 = s_root_transform_changed_00133cbf;
        uVar14 = DAT_001a7d20 + 1;
        bVar8 = DAT_001a7d20 < 0x40;
        DAT_001a7d20 = uVar14;
        if ((bVar8) && (DAT_001a7d00 != (code *)0x0)) {
          pcVar13 = "root_transform_changed";
          goto LAB_0016ed38;
        }
        goto LAB_0016f4dc;
      }
      goto LAB_0016f4d8;
    }
    if (param_4 + 0x4c >> 4 < 0x101) {
      bVar8 = false;
    }
    else {
      iVar9 = (*DAT_001a7ce8)(DAT_001a7cc8,param_4 + 0x3c,local_80,0x10);
      bVar8 = iVar9 == 1;
    }
    uVar10 = 4;
    auVar16._4_4_ = (int)param_2;
    auVar16._0_4_ = (int)param_1;
    auVar16._8_8_ = 0;
    auVar16 = NEON_fcmeq(local_80,auVar16 << 0x40,4);
    uVar15 = NEON_umaxv(CONCAT26(CONCAT11(~auVar16[0xd],~auVar16[0xc]),
                                 CONCAT24(CONCAT11(~auVar16[9],~auVar16[8]),
                                          CONCAT22(CONCAT11(~auVar16[5],~auVar16[4]),
                                                   CONCAT11(~auVar16[1],~auVar16[0])))),2);
    if (((uVar15 & 1) == 0) && (bVar8)) {
      uVar10 = FUN_0016f840(4);
      if ((int)uVar10 == 0) {
        PTR_s_not_initialized_001a7d50 = s_waiting_stage_ui_projection_001357ce;
        goto LAB_0016f4dc;
      }
      FUN_0016fb10();
      FUN_0016fc60(param_1,param_2,param_3,param_6);
      FUN_00170640();
      FUN_00170db4(param_3);
      FUN_00171808(param_1,param_2,param_6);
      FUN_00171d68(param_1,param_2,param_6);
      FUN_00172aac(param_3);
      if (DAT_001a7d10 != 0) {
        iVar9 = FUN_0016d5cc(DAT_001a7d38,param_5);
        if (iVar9 != 0) {
          if ((DAT_001a7d10 != 0) && (iVar9 = FUN_0016e464(DAT_001a7d38,1), iVar9 == 0)) {
            uVar10 = 0xffffffff;
            DAT_001a7d10 = -1;
            PTR_s_not_initialized_001a7d50 = s_container_input_disabled_001367ea;
            uVar14 = DAT_001a7d20 + 1;
            bVar8 = DAT_001a7d20 < 0x40;
            DAT_001a7d20 = uVar14;
            if ((bVar8) && (DAT_001a7d00 != (code *)0x0)) {
              pcVar13 = "container_input_disabled";
              goto LAB_0016ed38;
            }
            goto LAB_0016f4dc;
          }
          goto LAB_0016ee9c;
        }
LAB_0016f1b0:
        uVar10 = 0xffffffff;
        DAT_001a7d10 = -1;
        PTR_s_not_initialized_001a7d50 = s_container_root_membership_0013a4ff;
        uVar14 = DAT_001a7d20 + 1;
        bVar8 = DAT_001a7d20 < 0x40;
        DAT_001a7d20 = uVar14;
        if ((bVar8) && (DAT_001a7d00 != (code *)0x0)) {
          pcVar13 = "container_root_membership";
          goto LAB_0016ed38;
        }
        goto LAB_0016f4dc;
      }
LAB_0016ee9c:
      if (*(char *)((long)param_3 + 9) == '\0') {
        if (DAT_001a7d10 == 2) {
          if (DAT_001a7d14 == 0) {
            FUN_0016cf5c(0xc61c3c00,0xc61c3c00,DAT_001a7d38);
          }
          uVar10 = 1;
          DAT_001a7d14 = 1;
          PTR_s_not_initialized_001a7d50 = s_rich_scene_hidden_00133188;
        }
        else {
          uVar10 = 0;
          PTR_s_not_initialized_001a7d50 = "building_paused_while_closed";
          if (DAT_001a7d10 != 1) {
            PTR_s_not_initialized_001a7d50 = "waiting_first_open";
          }
        }
        goto LAB_0016f4dc;
      }
      iVar9 = nexus_ui_performance_mode();
      if (((param_6 < DAT_001a7d40) && (DAT_001a7d10 == 2)) && (DAT_001a7d14 == 0)) {
        iVar2 = *param_3;
        if (((int)DAT_001a7d48 == iVar2) && (DAT_001a7d48._4_4_ == iVar9)) {
          uVar10 = 1;
          goto LAB_0016f4dc;
        }
      }
      else {
        iVar2 = *param_3;
      }
      DAT_001a7d48._0_4_ = iVar2;
      lVar1 = 0x28;
      if (iVar9 != 0) {
        lVar1 = 100;
      }
      DAT_001a7d40 = lVar1 + param_6;
      if (iVar9 != 0) {
        param_6 = 0;
      }
      DAT_001a7d48._4_4_ = iVar9;
      iVar9 = nexus_rich_plan_stage
                        (param_1,param_2,DAT_001e53fc,DAT_001e5400,DAT_001e5404,param_3,param_6,
                         &DAT_001a7d58);
      if ((iVar9 != 1) ||
         (((((char)param_3[2] == 'Y' && (*(char *)((long)param_3 + 10) == '\0')) &&
           (iVar9 = nexus_rich_plus_layout(&DAT_001a7d58,&DAT_001e5168,param_6), iVar9 != 1)) ||
          (iVar9 = nexus_rich_header_entitlement(&DAT_001a7d58,DAT_001e5168._4_4_),
          pcVar7 = DAT_001a7cf8, uVar10 = DAT_001a7cc8, iVar9 != 1)))) {
        uVar10 = 4;
        goto LAB_0016f4dc;
      }
      if (DAT_001a7d10 == 0) {
        if (*(char *)((long)param_3 + 9) == '\0') goto LAB_0016f4d8;
        uVar12 = nexus_rich_asset(0);
        uVar10 = (*pcVar7)(uVar10,uVar12);
        pcVar7 = DAT_001a7cf8;
        uVar12 = DAT_001a7cc8;
        if ((int)uVar10 != 0) {
          uVar10 = nexus_rich_asset(1);
          uVar10 = (*pcVar7)(uVar12,uVar10);
          pcVar7 = DAT_001a7cf8;
          uVar12 = DAT_001a7cc8;
          if ((int)uVar10 != 0) {
            uVar10 = nexus_rich_asset(2);
            uVar10 = (*pcVar7)(uVar12,uVar10);
            pcVar7 = DAT_001a7cf8;
            uVar12 = DAT_001a7cc8;
            if ((int)uVar10 != 0) {
              uVar10 = nexus_rich_asset(3);
              uVar10 = (*pcVar7)(uVar12,uVar10);
              pcVar7 = DAT_001a7cf8;
              uVar12 = DAT_001a7cc8;
              if ((int)uVar10 != 0) {
                uVar10 = nexus_rich_asset(4);
                uVar10 = (*pcVar7)(uVar12,uVar10);
                pcVar7 = DAT_001a7cf8;
                uVar12 = DAT_001a7cc8;
                if ((int)uVar10 != 0) {
                  uVar10 = nexus_rich_asset(5);
                  uVar10 = (*pcVar7)(uVar12,uVar10);
                  pcVar7 = DAT_001a7cf8;
                  uVar12 = DAT_001a7cc8;
                  if ((int)uVar10 != 0) {
                    uVar10 = nexus_rich_asset(6);
                    uVar10 = (*pcVar7)(uVar12,uVar10);
                    if ((int)uVar10 != 0) {
                      iVar9 = FUN_00172da8();
                      if (iVar9 == 0) {
                        uVar10 = 0xffffffff;
                        DAT_001a7d10 = -1;
                        PTR_s_not_initialized_001a7d50 = s_a10_code_or_vtable_guard_001350f4;
                        uVar14 = DAT_001a7d20 + 1;
                        bVar8 = DAT_001a7d20 < 0x40;
                        DAT_001a7d20 = uVar14;
                        if ((bVar8) && (DAT_001a7d00 != (code *)0x0)) {
                          pcVar13 = "a10_code_or_vtable_guard";
                          goto LAB_0016ed38;
                        }
                      }
                      else {
                        DAT_001a7d38 = FUN_0016cdf0(0x80);
                        if (DAT_001a7d38 == 0) {
                          uVar10 = 0xffffffff;
                          DAT_001a7d10 = -1;
                          PTR_s_not_initialized_001a7d50 = s_container_allocation_00136803;
                          uVar14 = DAT_001a7d20 + 1;
                          bVar8 = DAT_001a7d20 < 0x40;
                          DAT_001a7d20 = uVar14;
                          if ((bVar8) && (DAT_001a7d00 != (code *)0x0)) {
                            pcVar13 = "container_allocation";
                            goto LAB_0016ed38;
                          }
                        }
                        else {
                          FUN_0016ce08();
                          iVar9 = FUN_0016debc(DAT_001a7d38,0x11abcf0);
                          if ((iVar9 == 0) || (iVar9 = FUN_0016ddc8(DAT_001a7d38), iVar9 == 0)) {
                            uVar10 = 0xffffffff;
                            DAT_001a7d10 = -1;
                            PTR_s_not_initialized_001a7d50 = s_sprite_constructor_001357fd;
                            uVar14 = DAT_001a7d20 + 1;
                            bVar8 = DAT_001a7d20 < 0x40;
                            DAT_001a7d20 = uVar14;
                            if ((bVar8) && (DAT_001a7d00 != (code *)0x0)) {
                              pcVar13 = "sprite_constructor";
                              goto LAB_0016ed38;
                            }
                          }
                          else {
                            local_88 = CONCAT62(local_88._2_6_,1);
                            if ((((DAT_001a7d38 + 0x4e < 0x1000) ||
                                 ((DAT_001a7d38 & 0xfffffffffffffffe) == 0xffffffffffffffb0)) ||
                                (iVar9 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_001a7d38 + 0x4e,&local_88,
                                                         2), iVar9 != 1)) || ((short)local_88 != 0))
                            {
                              DAT_001a7d10 = -1;
                              PTR_s_not_initialized_001a7d50 =
                                   s_container_not_empty_for_input_0013266a;
                              uVar14 = DAT_001a7d20 + 1;
                              bVar8 = DAT_001a7d20 < 0x40;
                              DAT_001a7d20 = uVar14;
                              if ((bVar8) && (DAT_001a7d00 != (code *)0x0)) {
                                pcVar13 = "container_not_empty_for_input";
LAB_0016f6ec:
                                DAT_001a7d10 = -1;
                                (*DAT_001a7d00)(DAT_001a7cc8,"nexus_rich_stopped",pcVar13,0);
                              }
                            }
                            else {
                              FUN_0016ce20(DAT_001a7d38,1);
                              iVar9 = FUN_0016e464(DAT_001a7d38,1);
                              if (iVar9 == 0) {
                                DAT_001a7d10 = -1;
                                PTR_s_not_initialized_001a7d50 =
                                     s_container_input_enable_failed_00134421;
                                uVar14 = DAT_001a7d20 + 1;
                                bVar8 = DAT_001a7d20 < 0x40;
                                DAT_001a7d20 = uVar14;
                                if ((bVar8) && (DAT_001a7d00 != (code *)0x0)) {
                                  pcVar13 = "container_input_enable_failed";
                                  goto LAB_0016f6ec;
                                }
                              }
                              else {
                                uVar14 = DAT_001a7d20 + 1;
                                bVar8 = DAT_001a7d20 < 0x40;
                                DAT_001a7d20 = uVar14;
                                if ((bVar8) && (DAT_001a7d00 != (code *)0x0)) {
                                  (*DAT_001a7d00)(DAT_001a7cc8,"nexus_rich_input_enabled",
                                                  "empty_container_byte48_verified",0);
                                }
                                FUN_0016cf5c(0xc61c3c00,0xc61c3c00,DAT_001a7d38);
                                FUN_0016cfb8(param_4,DAT_001a7d38);
                                iVar9 = FUN_0016d5cc(DAT_001a7d38,param_5);
                                if (iVar9 != 0) {
                                  uVar14 = DAT_001a7d20 + 1;
                                  DAT_001a7d10 = 1;
                                  PTR_s_not_initialized_001a7d50 =
                                       s_building_hidden_rich_scene_00134a9b;
                                  bVar8 = DAT_001a7d20 < 0x40;
                                  DAT_001a7d20 = uVar14;
                                  if ((bVar8) && (DAT_001a7d00 != (code *)0x0)) {
                                    (*DAT_001a7d00)(DAT_001a7cc8,"nexus_rich_building",
                                                    "building_hidden_rich_scene",DAT_001a7d58);
                                  }
                                  goto LAB_0016f030;
                                }
                                DAT_001a7d10 = -1;
                                uVar14 = DAT_001a7d20 + 1;
                                PTR_s_not_initialized_001a7d50 =
                                     s_container_stage_attachment_0013319a;
                                bVar8 = DAT_001a7d20 < 0x40;
                                DAT_001a7d20 = uVar14;
                                if ((bVar8) && (DAT_001a7d00 != (code *)0x0)) {
                                  pcVar13 = "container_stage_attachment";
                                  goto LAB_0016f6ec;
                                }
                              }
                            }
                            uVar10 = 0xffffffff;
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
        goto LAB_0016f4dc;
      }
LAB_0016f030:
      iVar9 = FUN_0016d5cc(DAT_001a7d38,param_5);
      if (iVar9 == 0) goto LAB_0016f1b0;
      if (DAT_001a7d10 == 1) {
        uVar14 = DAT_001a7d18 + 8;
        if (DAT_001a7d58 <= DAT_001a7d18 + 8) {
          uVar14 = DAT_001a7d58;
        }
        for (; DAT_001a7d18 < uVar14; DAT_001a7d18 = DAT_001a7d18 + 1) {
          uVar10 = FUN_00172fd4(DAT_001a7d18);
          if ((int)uVar10 != 1) goto LAB_0016f4dc;
        }
        uVar10 = 2;
        if (DAT_001a7d18 < DAT_001a7d58) goto LAB_0016f4dc;
        DAT_001a7d10 = 2;
        PTR_s_not_initialized_001a7d50 = s_rich_scene_attached_00138b14;
        uVar14 = DAT_001a7d20 + 1;
        bVar8 = DAT_001a7d20 < 0x40;
        DAT_001a7d20 = uVar14;
        if ((bVar8) && (DAT_001a7d00 != (code *)0x0)) {
          (*DAT_001a7d00)(DAT_001a7cc8,"nexus_rich_constructed","rich_scene_attached");
        }
      }
      if (DAT_001a7d58 != 0) {
        uVar14 = 0;
        do {
          uVar10 = FUN_0017474c(uVar14);
          if ((int)uVar10 != 1) goto LAB_0016f4dc;
          uVar14 = uVar14 + 1;
        } while (uVar14 < DAT_001a7d58);
      }
      iVar9 = FUN_0016d910();
      if (iVar9 != 0) {
        iVar9 = FUN_001753b0(param_3);
        if (iVar9 == 0) {
          uVar10 = 0xffffffff;
          DAT_001a7d10 = -1;
          PTR_s_not_initialized_001a7d50 = s_main_page_content_paint_00137d90;
          uVar14 = DAT_001a7d20 + 1;
          bVar8 = DAT_001a7d20 < 0x40;
          DAT_001a7d20 = uVar14;
          if ((bVar8) && (DAT_001a7d00 != (code *)0x0)) {
            pcVar13 = "main_page_content_paint";
            goto LAB_0016ed38;
          }
        }
        else {
          FUN_0016cf00(DAT_001a7d5c,DAT_001a7d5c,DAT_001a7d38);
          uVar4 = 0xc61c3c00;
          uVar6 = 0xc61c3c00;
          if (*(char *)((long)param_3 + 9) != '\0') {
            uVar4 = DAT_001a7d60._4_4_;
            uVar6 = (undefined4)DAT_001a7d60;
          }
          FUN_0016cf5c(uVar6,uVar4,DAT_001a7d38);
          DAT_001a7d14 = 0;
          uVar10 = 1;
          PTR_s_not_initialized_001a7d50 = "rich_scene_hidden";
          if (*(char *)((long)param_3 + 9) != '\0') {
            PTR_s_not_initialized_001a7d50 = "rich_scene_attached";
          }
        }
        goto LAB_0016f4dc;
      }
      uVar10 = 0xffffffff;
      DAT_001a7d10 = -1;
      PTR_s_not_initialized_001a7d50 = s_stage_changed_during_paint_00139a0c;
      uVar14 = DAT_001a7d20 + 1;
      bVar8 = 0x3f < DAT_001a7d20;
      DAT_001a7d20 = uVar14;
      if ((bVar8) || (DAT_001a7d00 == (code *)0x0)) goto LAB_0016f4dc;
      pcVar13 = "stage_changed_during_paint";
      goto LAB_0016ed38;
    }
  }
LAB_0016f4dc:
  DAT_001a7d08 = 0;
LAB_0016f4e0:
  if (*(long *)(lVar3 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar10);
}

/* ===== FUN_0016f840 @ 0016f840 ===== */

void FUN_0016f840(float param_1,float param_2)

{
  bool bVar1;
  uint uVar2;
  long lVar3;
  int iVar4;
  undefined8 uVar5;
  float fVar6;
  float fVar7;
  float local_5c;
  undefined8 local_58;
  undefined8 local_50;
  long local_48;
  
  lVar3 = tpidr_el0;
  local_48 = *(long *)(lVar3 + 0x28);
  uVar5 = 0;
  local_58 = 0;
  local_50 = 0;
  local_5c = 0.0;
  if ((DAT_001a7d28 + 0x178 < 0x1000) || ((DAT_001a7d28 & 0xfffffffffffffffc) == 0xfffffffffffffe84)
     ) goto LAB_0016fa2c;
  iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_001a7d28 + 0x178,(long)&local_58 + 4,4);
  if (iVar4 == 1) {
    uVar5 = 0;
    if ((DAT_001a7d28 + 0x4c < 0x1000) ||
       ((DAT_001a7d28 & 0xfffffffffffffffc) == 0xffffffffffffffb0)) goto LAB_0016fa2c;
    iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_001a7d28 + 0x4c,&local_58,4);
    if (iVar4 == 1) {
      uVar5 = 0;
      if ((DAT_001a7d28 + 0x54 < 0x1000) ||
         ((DAT_001a7d28 & 0xfffffffffffffffc) == 0xffffffffffffffa8)) goto LAB_0016fa2c;
      iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_001a7d28 + 0x54,&local_5c,4);
      if ((iVar4 == 1) &&
         ((0x200 < DAT_001a7d28 + 0x28c >> 3 &&
          (iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_001a7d28 + 0x284,&local_50,8), iVar4 == 1)))) {
        uVar5 = 0;
        if ((ABS(local_58._4_4_) == INFINITY) ||
           (((NAN(ABS(local_58._4_4_)) || (local_58._4_4_ <= 0.0)) ||
            (local_58._4_4_ != 64.0 && local_58._4_4_ < 64.0 == NAN(local_58._4_4_)))))
        goto LAB_0016fa2c;
        uVar5 = 0;
        fVar6 = ABS((float)local_58);
        if ((fVar6 == INFINITY) || (NAN(fVar6))) goto LAB_0016fa2c;
        uVar5 = 0;
        fVar7 = ABS(local_5c);
        if (((fVar7 == INFINITY) ||
            ((NAN(fVar7) || (fVar6 != 16384.0 && fVar6 < 16384.0 == NAN(fVar6))))) ||
           (fVar7 != 16384.0 && fVar7 < 16384.0 == NAN(fVar7))) goto LAB_0016fa2c;
        if (((float)(int)(param_1 / local_58._4_4_) == (float)(int)local_50) &&
           ((float)(int)(param_2 / local_58._4_4_) == (float)local_50._4_4_)) {
          if ((local_58._4_4_ != DAT_001e53fc) ||
             (((float)local_58 != DAT_001e5400 || (local_5c != DAT_001e5404)))) {
            FUN_00176a24((double)param_1,(double)param_2,(double)local_58._4_4_,
                         (double)(float)local_58,(double)local_5c,&DAT_001e5408,0xc0,0xc0,
                         "pixels=%.9gx%.9g stage_scale=%.9g offset=%.9g,%.9g ui=%dx%d");
            uVar2 = DAT_001a7d20 + 1;
            bVar1 = DAT_001a7d20 < 0x40;
            DAT_001a7d20 = uVar2;
            if ((bVar1) && (DAT_001a7d00 != (code *)0x0)) {
              (*DAT_001a7d00)(DAT_001a7cc8,"nexus_rich_viewport",&DAT_001e5408,0);
            }
          }
          DAT_001e5400 = (float)local_58;
          DAT_001e53fc = local_58._4_4_;
          DAT_001e5404 = local_5c;
          uVar5 = 1;
          goto LAB_0016fa2c;
        }
      }
    }
  }
  uVar5 = 0;
LAB_0016fa2c:
  if (*(long *)(lVar3 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar5);
}

/* ===== FUN_00170640 @ 00170640 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00170640(void)

{
  undefined1 *puVar1;
  char *pcVar2;
  long lVar3;
  bool bVar4;
  code *pcVar5;
  bool bVar6;
  int iVar7;
  undefined8 uVar8;
  ulong uVar9;
  ulong uVar10;
  undefined8 uVar11;
  bool bVar12;
  ulong *puVar13;
  long lVar14;
  float fVar15;
  undefined4 uVar16;
  float fVar17;
  float local_8b0;
  float local_8ac;
  float local_8a8;
  float local_8a4;
  undefined1 auStack_8a0 [2048];
  undefined8 local_a0;
  float local_98;
  float local_94;
  long local_90;
  
  lVar3 = tpidr_el0;
  local_90 = *(long *)(lVar3 + 0x28);
  iVar7 = FUN_001921d4(auStack_8a0,0x800);
  DAT_002839fc = 0;
  fVar15 = (float)NEON_fmadd(DAT_0022d818,0x41000000,DAT_00283330);
  if (DAT_00283330 == 0.0 || DAT_00283330 < 0.0 != NAN(DAT_00283330)) {
    fVar15 = 0.0;
  }
  puVar1 = (undefined1 *)0x0;
  if (iVar7 != 0) {
    puVar1 = auStack_8a0;
  }
  fVar17 = DAT_0022d818 * 260.0;
  if (DAT_0022d818 * 260.0 <= fVar15) {
    fVar17 = fVar15;
  }
  FUN_00179f68(DAT_0022d818 * 20.0,fVar17,DAT_0022d818 * 14.0,&DAT_00283a00,puVar1);
  pcVar5 = DAT_001a7cf8;
  uVar11 = DAT_001a7cc8;
  if (iVar7 == 0) {
    if ((DAT_00284220 != 0) && (iVar7 = FUN_0016d5cc(DAT_00284220,DAT_001a7d30), iVar7 != 0)) {
      FUN_0016cf5c(0xc61c3c00,0xc61c3c00,DAT_00284220);
    }
    if ((DAT_002842f8 != 0) && (iVar7 = FUN_0016d5cc(DAT_002842f8,DAT_001a7d30), iVar7 != 0)) {
      FUN_0016cf5c(0xc61c3c00,0xc61c3c00,DAT_002842f8);
    }
    goto LAB_00170d78;
  }
  if ((DAT_002843d0 == 0) || (DAT_002843d0 == DAT_001a7d30)) {
    if (DAT_002843d8 != '\x01') goto LAB_00170800;
    iVar7 = FUN_0016d5cc(DAT_00284220);
    if ((iVar7 == 0) || (iVar7 = FUN_0016d5cc(DAT_002842f8,DAT_001a7d30), iVar7 == 0))
    goto LAB_00170d78;
  }
  else {
    DAT_002843d0 = 0;
    uRam0000000000284238 = 0;
    _DAT_00284230 = 0;
    uRam0000000000284248 = 0;
    _DAT_00284240 = 0;
    uRam0000000000284258 = 0;
    _DAT_00284250 = 0;
    uRam0000000000284268 = 0;
    _DAT_00284260 = 0;
    uRam0000000000284278 = 0;
    _DAT_00284270 = 0;
    uRam0000000000284288 = 0;
    _DAT_00284280 = 0;
    uRam0000000000284298 = 0;
    _DAT_00284290 = 0;
    uRam00000000002842a8 = 0;
    _DAT_002842a0 = 0;
    uRam00000000002842b8 = 0;
    _DAT_002842b0 = 0;
    uRam00000000002842c8 = 0;
    _DAT_002842c0 = 0;
    uRam00000000002842d8 = 0;
    _DAT_002842d0 = 0;
    uRam00000000002842e8 = 0;
    _DAT_002842e0 = 0;
    DAT_002842f8 = 0;
    _DAT_002842f0 = 0;
    uRam0000000000284308 = 0;
    _DAT_00284300 = 0;
    uRam0000000000284318 = 0;
    _DAT_00284310 = 0;
    uRam0000000000284328 = 0;
    _DAT_00284320 = 0;
    uRam0000000000284338 = 0;
    _DAT_00284330 = 0;
    uRam0000000000284348 = 0;
    _DAT_00284340 = 0;
    uRam0000000000284358 = 0;
    _DAT_00284350 = 0;
    uRam0000000000284368 = 0;
    _DAT_00284360 = 0;
    uRam0000000000284378 = 0;
    _DAT_00284370 = 0;
    uRam0000000000284388 = 0;
    _DAT_00284380 = 0;
    uRam0000000000284398 = 0;
    _DAT_00284390 = 0;
    uRam00000000002843a8 = 0;
    _DAT_002843a0 = 0;
    uRam00000000002843b8 = 0;
    _DAT_002843b0 = 0;
    uRam00000000002843c8 = 0;
    _DAT_002843c0 = 0;
    uRam0000000000284228 = 0;
    DAT_00284220 = 0;
    DAT_002843d8 = '\0';
LAB_00170800:
    uVar8 = nexus_rich_asset(5);
    iVar7 = (*pcVar5)(uVar11,uVar8);
    if ((iVar7 == 0) || (iVar7 = FUN_00172da8(), iVar7 == 0)) goto LAB_00170d78;
    lVar14 = 0;
    bVar6 = true;
    do {
      bVar12 = bVar6;
      puVar13 = (ulong *)(&DAT_00284220 + lVar14 * 0x1b);
      if (*puVar13 == 0) {
        nexus_rich_asset(5);
        uVar9 = FUN_0016ce48();
        iVar7 = FUN_0016dc20(uVar9,5,0);
        if (iVar7 == 0) goto LAB_00170d78;
        local_8b0 = 0.0;
        local_a0 = 1;
        if ((((uVar9 + 0x38 < 0x1000) || ((uVar9 & 0xfffffffffffffff8) == 0xffffffffffffffc0)) ||
            (iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar9 + 0x38,&local_a0,8), iVar7 != 1)) ||
           (((local_a0 != 0 || (uVar9 + 0x40 < 0x1000)) ||
            (((uVar9 & 0xfffffffffffffffc) == 0xffffffffffffffbc ||
             ((iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar9 + 0x40,&local_8b0,4), iVar7 != 1 ||
              (local_8b0 != -NAN)))))))) goto LAB_00170d78;
        uVar10 = FUN_0016cdf0(0x260);
        *puVar13 = uVar10;
        if (uVar10 == 0) goto LAB_00170d78;
        FUN_0016ce34();
        local_a0 = 0;
        if (*puVar13 + 8 >> 3 < 0x201) {
          bVar6 = false;
        }
        else {
          iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,*puVar13,&local_a0,8);
          bVar6 = iVar7 == 1;
        }
        bVar4 = false;
        if (0xfff < local_a0) {
          bVar4 = bVar6;
        }
        uVar10 = local_a0;
        if (!(bool)(bVar4 & (local_a0 & 7) == 0)) {
          uVar10 = 0;
        }
        if (uVar10 != DAT_001a7cd0 + 0x11c0a48U) goto LAB_00170d78;
        uVar10 = *puVar13;
        local_8b0 = 0.0;
        local_a0 = 1;
        if (((((uVar10 + 0x38 < 0x1000) || ((uVar10 & 0xfffffffffffffff8) == 0xffffffffffffffc0)) ||
             (iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar10 + 0x38,&local_a0,8), iVar7 != 1)) ||
            (((local_a0 != 0 || (uVar10 + 0x40 < 0x1000)) ||
             (((uVar10 & 0xfffffffffffffffc) == 0xffffffffffffffbc ||
              ((iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar10 + 0x40,&local_8b0,4), iVar7 != 1 ||
               (local_8b0 != -NAN)))))))) ||
           ((iVar7 = FUN_0014e5ac(*puVar13), iVar7 == 0 ||
            (iVar7 = FUN_0017c42c(puVar13,uVar9,(uint)lVar14 | 900), iVar7 == 0))))
        goto LAB_00170d78;
        FUN_0016ce84(uVar9,0);
        uVar9 = *puVar13;
        pcVar2 = "CHAT";
        if (!bVar12) {
          pcVar2 = "CLEAR";
        }
        uVar11 = FUN_0017b858(pcVar2);
        FUN_0016cec0(uVar9,uVar11);
        FUN_0016cf5c(0xc61c3c00,0xc61c3c00,*puVar13);
        FUN_0016cfb8(DAT_001a7d28,*puVar13);
        iVar7 = FUN_0016d5cc(*puVar13,DAT_001a7d30);
        if (iVar7 == 0) goto LAB_00170d78;
        local_a0 = CONCAT71(local_a0._1_7_,0xff);
        if (((*puVar13 + 0x49 < 0x1001) ||
            (iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,*puVar13 + 0x48,&local_a0,1), iVar7 != 1)) ||
           ((char)local_a0 != '\x01')) goto LAB_00170d78;
      }
      lVar14 = 1;
      bVar6 = false;
    } while (bVar12);
    DAT_002843d0 = DAT_001a7d30;
    DAT_002843d8 = '\x01';
  }
  lVar14 = 0;
  bVar6 = true;
  do {
    bVar12 = bVar6;
    puVar13 = (ulong *)(&DAT_00284220 + lVar14 * 0x1b);
    FUN_0016cf5c(0,0,*puVar13);
    FUN_0016cf00(0x3f800000,0x3f800000,*puVar13);
    FUN_0016cf84(*puVar13,&local_8b0);
    if ((((ABS(local_8a8 - local_8b0) == INFINITY) || (NAN(ABS(local_8a8 - local_8b0)))) ||
        ((ABS(local_8a4 - local_8ac) == INFINITY ||
         ((NAN(ABS(local_8a4 - local_8ac)) || (fVar15 = local_8a8 - local_8b0, fVar15 <= 0.0))))))
       || (local_8a4 - local_8ac <= 0.0)) goto LAB_00170d78;
    uVar9 = *puVar13 + 0x10;
    if (((uVar9 < 0x1000) || ((*puVar13 & 0xfffffffffffffff0) == 0xffffffffffffffe0)) ||
       (iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar9,&local_a0,0x10), iVar7 != 1)) goto LAB_00170d78;
    if ((ABS((float)local_a0) == INFINITY) || (NAN(ABS((float)local_a0)))) goto LAB_00170d78;
    if ((((ABS(local_a0._4_4_) == INFINITY) ||
         ((NAN(ABS(local_a0._4_4_)) || (ABS(local_98) == INFINITY)))) || (NAN(ABS(local_98)))) ||
       (((((ABS(local_94) == INFINITY || (NAN(ABS(local_94)))) || (local_a0._4_4_ != 0.0)) ||
         ((local_98 != 0.0 || ((float)local_a0 <= 0.0)))) ||
        ((local_94 <= 0.0 ||
         (((float)local_a0 != 128.0 && (float)local_a0 < 128.0 == NAN((float)local_a0) ||
          (local_94 != 128.0 && local_94 < 128.0 == NAN(local_94))))))))) goto LAB_00170d78;
    fVar15 = ((float)local_a0 * 76.0 * DAT_0022d818) / (fVar15 * DAT_001e53fc);
    if ((ABS(fVar15) == INFINITY) ||
       (((NAN(ABS(fVar15)) || (fVar15 <= 0.0)) || (fVar15 != 8.0 && fVar15 < 8.0 == NAN(fVar15)))))
    goto LAB_00170d78;
    FUN_0016cf00(fVar15,fVar15,*puVar13);
    FUN_0016cf84(*puVar13,&local_8b0);
    uVar16 = NEON_fmadd((float)lVar14,0x42a80000,0x41a00000);
    fVar15 = (float)NEON_fnmsub(uVar16,DAT_0022d818,DAT_001e5400);
    fVar17 = (float)NEON_fnmsub(DAT_0022d818,0x431e0000,DAT_001e5404);
    FUN_0016cf5c(fVar15 / DAT_001e53fc - local_8b0,fVar17 / DAT_001e53fc - local_8ac,*puVar13);
    lVar14 = 1;
    bVar6 = false;
  } while (bVar12);
  DAT_002839fc = 1;
LAB_00170d78:
  if (*(long *)(lVar3 + 0x28) == local_90) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0017609c @ 0017609c ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint FUN_0017609c(ulong *param_1,int *param_2)

{
  uint uVar1;
  ulong uVar2;
  undefined8 *puVar3;
  uint uVar4;
  long lVar5;
  bool bVar6;
  bool bVar7;
  int iVar8;
  int iVar9;
  uint uVar10;
  ulong local_a8;
  undefined8 uStack_a0;
  undefined8 local_98;
  undefined8 uStack_90;
  undefined8 local_88;
  undefined8 uStack_80;
  undefined8 local_78;
  undefined4 uStack_70;
  undefined4 uStack_6c;
  short local_68;
  undefined2 uStack_66;
  undefined4 uStack_64;
  undefined4 uStack_60;
  undefined4 uStack_5c;
  long local_58;
  
  lVar5 = tpidr_el0;
  local_58 = *(long *)(lVar5 + 0x28);
  if ((int)param_1[0x1a] != 0) {
    uVar4 = (int)((ulong)(param_1 + -0x386ad) >> 3) * 0x684bda13;
    puVar3 = &DAT_001a7d38;
    if (0x2a < uVar4) {
      puVar3 = &DAT_0022d4f0;
    }
    iVar8 = FUN_0016d5cc(*param_1,*puVar3);
    if (iVar8 != 0) {
      local_a8 = 0;
      uVar2 = *param_1 + 0x30;
      if ((((0xfff < uVar2) && ((*param_1 & 0xfffffffffffffff8) != 0xffffffffffffffc8)) &&
          (iVar8 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar2,&local_a8,8), iVar8 == 1)) &&
         ((local_a8 == 0 || (local_a8 == DAT_001a7d28)))) {
        iVar8 = *param_2;
        local_a8 = CONCAT71(local_a8._1_7_,0xff);
        if (0x1000 < *param_1 + 0x49) {
          iVar9 = (*DAT_001a7ce8)(DAT_001a7cc8,*param_1 + 0x48,&local_a8,1);
          uVar10 = 0;
          if ((iVar9 != 1) || (uVar10 = 0, (bool)(char)local_a8 != (iVar8 == 2))) goto LAB_00176198;
          if (*param_2 == 2) {
            uVar10 = FUN_0017c704(param_1,1,&local_a8);
            if (uVar10 == 0) {
              uRam00000000001e5220 = CONCAT44(uStack_6c,uStack_70);
              uRam00000000001e5230 = CONCAT44(uStack_5c,uStack_60);
              _DAT_001e5228 = CONCAT44(uStack_64,CONCAT22(uStack_66,local_68));
              uRam00000000001e5210 = uStack_80;
              _DAT_001e5208 = local_88;
              _DAT_001e5218 = local_78;
              uRam00000000001e51f0 = uStack_a0;
              _DAT_001e51e8 = local_a8;
              uRam00000000001e5200 = uStack_90;
              _DAT_001e51f8 = local_98;
              FUN_00176a24(&DAT_001e5238,0x1c0,0x1c0,
                           "ok=%d mode=%u reads=%x button=%llx source=%llx clip80=%llx display88=%llx sourceb0=%llx mcparent=%llx mcindex=%d displayparent=%llx displayindex=%d named=%d matches=%u active=%u"
                           ,0,uStack_5c,uStack_64,local_a8,uStack_a0,local_98,uStack_90,local_88,
                           uStack_80,uStack_70,local_78,uStack_6c,(int)local_68,uStack_60,uStack_66)
              ;
              uVar1 = DAT_001a7d20 + 1;
              bVar7 = DAT_001a7d20 < 0x40;
              DAT_001a7d20 = uVar1;
              if ((bVar7) && (DAT_001a7d00 != (code *)0x0)) {
                (*DAT_001a7d00)(DAT_001a7cc8,"nexus_rich_binding",&DAT_001e5238,uVar4);
              }
            }
            goto LAB_00176198;
          }
          local_a8 = 0;
          if (*param_1 + 8 >> 3 < 0x201) {
            bVar7 = false;
          }
          else {
            iVar8 = (*DAT_001a7ce8)(DAT_001a7cc8,*param_1,&local_a8,8);
            bVar7 = iVar8 == 1;
          }
          bVar6 = false;
          if (0xfff < local_a8) {
            bVar6 = bVar7;
          }
          uVar2 = local_a8;
          if (!(bool)(bVar6 & (local_a8 & 7) == 0)) {
            uVar2 = 0;
          }
          if (uVar2 == DAT_001a7cd0 + 0x11ad208U) {
            if (*param_2 != 3) {
              uVar10 = 1;
              goto LAB_00176198;
            }
            local_a8 = 0;
            if (param_1[2] + 8 >> 3 < 0x201) {
              bVar7 = false;
            }
            else {
              iVar8 = (*DAT_001a7ce8)(DAT_001a7cc8,param_1[2],&local_a8,8);
              bVar7 = iVar8 == 1;
            }
            bVar6 = false;
            if (0xfff < local_a8) {
              bVar6 = bVar7;
            }
            uVar2 = local_a8;
            if (!(bool)(bVar6 & (local_a8 & 7) == 0)) {
              uVar2 = 0;
            }
            if ((uVar2 == DAT_001a7cd0 + 0x11abad8U) &&
               (iVar8 = FUN_0016df78(param_1[2]), iVar8 != 0)) {
              iVar8 = FUN_0016d800(param_1[2],*param_1);
              uVar10 = (uint)(iVar8 != 0);
              goto LAB_00176198;
            }
          }
        }
      }
    }
  }
  uVar10 = 0;
LAB_00176198:
  if (*(long *)(lVar5 + 0x28) == local_58) {
    return uVar10;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== nexus_rich_validate_native @ 00176904 ===== */

ulong nexus_rich_validate_native(void)

{
  uint uVar1;
  int iVar2;
  ulong uVar3;
  
  if (DAT_001a7d0c == 0) {
    uVar3 = 0;
  }
  else {
    uVar3 = (*DAT_001a7cf0)(DAT_001a7cc8);
    if ((int)uVar3 != 0) {
      uVar3 = FUN_00193f80(1,&DAT_001a7d08);
      if ((uVar3 & 1) == 0) {
        iVar2 = FUN_00172da8();
        uVar1 = 1;
        if (iVar2 == 0) {
          uVar1 = 0xffffffff;
        }
        uVar3 = (ulong)uVar1;
        DAT_001a7d08 = 0;
      }
      else {
        uVar3 = 3;
      }
    }
  }
  return uVar3;
}

/* ===== nexus_rich_diagnostics @ 0017697c ===== */

int nexus_rich_diagnostics(long param_1,long param_2)

{
  int iVar1;
  ulong uVar2;
  
  iVar1 = 0;
  if ((param_1 != 0) && (param_2 != 0)) {
    uVar2 = FUN_00193f80(1,&DAT_001a7d08);
    if ((uVar2 & 1) == 0) {
      iVar1 = FUN_00176a24(param_1,0xffffffffffffffff,param_2,
                           "{\"schema\":1,\"phase\":%d,\"built\":%u,\"planned\":%u,\"strings\":%u,\"reason\":\"%s\",\"projection\":\"%s\",\"binding\":\"%s\"}"
                           ,DAT_001a7d10,DAT_001a7d18,DAT_001a7d58,DAT_001a7d1c,
                           PTR_s_not_initialized_001a7d50,&DAT_001e5408,&DAT_001e5238);
      DAT_001a7d08 = 0;
      if (iVar1 < 1) {
        iVar1 = 0;
      }
    }
    else {
      iVar1 = 0;
    }
  }
  return iVar1;
}

/* ===== nexus_rich_renderer_ops_v1 @ 00176ac8 ===== */

undefined * nexus_rich_renderer_ops_v1(void)

{
  return &DAT_0019c048;
}

/* ===== nexus_ui_graphics_resources @ 00177afc ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void nexus_ui_graphics_resources(ulong param_1,ulong param_2,ulong param_3,uint param_4)

{
  bool bVar1;
  uint *puVar2;
  ulong *puVar3;
  uint uVar4;
  long lVar5;
  undefined1 auVar6 [16];
  int iVar7;
  undefined8 uVar8;
  ulong uVar9;
  uint uVar10;
  ulong uVar11;
  ulong uVar12;
  ulong *puVar13;
  ulong uVar14;
  int *piVar15;
  undefined8 *puVar16;
  uint *puVar17;
  undefined1 auVar18 [16];
  undefined1 auVar19 [16];
  undefined1 auVar20 [16];
  int local_c90;
  int local_c8c;
  ulong local_c88;
  short local_c80 [4];
  ulong local_c78;
  uint local_c70 [2];
  ulong local_c68;
  uint local_80;
  long local_70;
  
  lVar5 = tpidr_el0;
  local_70 = *(long *)(lVar5 + 0x28);
  uVar8 = 0;
  if (((1 < param_4) || (DAT_001a7d0c == 0)) ||
     (uVar8 = (*DAT_001a7cf0)(DAT_001a7cc8), (int)uVar8 == 0)) goto LAB_00177bf8;
  uVar9 = FUN_00193f80(1,&DAT_001a7d08);
  if ((uVar9 & 1) != 0) {
    uVar8 = 0;
    goto LAB_00177bf8;
  }
  local_c90 = 0;
  if (DAT_001a7d0c == 0) goto LAB_00177be8;
  iVar7 = (*DAT_001a7cf0)(DAT_001a7cc8);
  uVar8 = 0;
  if ((((param_2 == 0) || (param_1 == 0)) ||
      ((iVar7 == 0 || ((DAT_001a7d28 != param_1 || (DAT_001a7d30 != param_2)))))) ||
     (uVar8 = FUN_0016d910(0), (int)uVar8 == 0)) goto LAB_00177bec;
  local_c80[0] = CONCAT11(local_c80[0]._1_1_,1);
  if (DAT_001a7d28 + 0x19d < 0x1001) goto LAB_00177be8;
  iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_001a7d28 + 0x19c,local_c80,1);
  uVar8 = 0;
  if (((((iVar7 != 1) || ((char)local_c80[0] != '\0')) ||
       (uVar8 = 0, (_DAT_001a7d10 & 0xfffffffd) != 0)) ||
      ((0x280 < DAT_001a7d18 || ((DAT_00281a68 & 1) != 0)))) ||
     ((((uVar8 = 0, param_3 == param_2 || ((param_3 == 0 || (param_3 == param_1)))) ||
       (DAT_001a7d38 == param_3)) || (DAT_00281a50 == param_3)))) goto LAB_00177bec;
  iVar7 = FUN_00177620(param_3,param_2,local_c80);
  if (iVar7 == 0) {
LAB_00177be8:
    uVar8 = 0;
  }
  else {
    if (DAT_001a7d10 != 0) {
      local_c88 = local_c88 & 0xffffffffffff0000;
      if ((((((DAT_001a7d38 != param_2) && (DAT_001a7d38 != 0)) && (DAT_001a7d38 != param_1)) &&
           ((DAT_001a7d38 != DAT_00281a50 &&
            (iVar7 = FUN_0016debc(DAT_001a7d38,0x11abcf0), iVar7 != 0)))) &&
          (((iVar7 = FUN_0016d5cc(DAT_001a7d38,param_2), iVar7 != 0 &&
            ((iVar7 = FUN_0016df78(DAT_001a7d38), iVar7 != 0 &&
             (iVar7 = FUN_0016e464(DAT_001a7d38,1), iVar7 != 0)))) &&
           ((0xfff < DAT_001a7d38 + 0x4e &&
            ((((DAT_001a7d38 & 0xfffffffffffffffe) != 0xffffffffffffffb0 &&
              (iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_001a7d38 + 0x4e,&local_c88,2), iVar7 == 1))
             && ((short)local_c88 == 0x2c)))))))) &&
         ((DAT_001a7d58 == DAT_001a7d18 && (iVar7 = FUN_00179448(), iVar7 != 0))))
      goto LAB_00177dec;
      goto LAB_00177be8;
    }
    if ((DAT_001a7d38 != 0) || ((DAT_001a7d18 != 0 || (iVar7 = FUN_00179448(), iVar7 == 0))))
    goto LAB_00177be8;
LAB_00177dec:
    uVar9 = 0;
    do {
      puVar3 = &DAT_001c3568 + uVar9 * 0x1b;
      uVar11 = *puVar3;
      if (uVar9 < DAT_001a7d18) {
        if ((uVar11 == 0) || (DAT_001a7d28 == uVar11)) goto LAB_00177be8;
        auVar18._8_8_ = DAT_00281a50;
        auVar18._0_8_ = DAT_001a7d38;
        auVar19._8_8_ = uVar11;
        auVar19._0_8_ = uVar11;
        auVar20 = NEON_cmeq(_DAT_0022d4e0,auVar19,8);
        auVar6._8_8_ = uVar11;
        auVar6._0_8_ = uVar11;
        auVar19 = NEON_cmeq(auVar18,auVar6,8);
        if ((((((auVar19 & (undefined1  [16])0x1) != (undefined1  [16])0x0 ||
               (auVar20 & (undefined1  [16])0x1) != (undefined1  [16])0x0) ||
              (auVar20 & (undefined1  [16])0x1) != (undefined1  [16])0x0) || DAT_001a7d30 == uVar11)
            || uVar11 == param_3) || DAT_0022d4f0 == uVar11) goto LAB_00177be8;
        if (uVar9 != 0) {
          if (DAT_001c3568 == uVar11) goto LAB_00177be8;
          uVar12 = 0;
          puVar13 = &DAT_001c3640;
          do {
            if (uVar9 - 1 == uVar12) goto LAB_00177ef0;
            uVar14 = *puVar13;
            uVar12 = uVar12 + 1;
            puVar13 = puVar13 + 0x1b;
          } while (uVar14 != uVar11);
          if (uVar12 < uVar9) goto LAB_00177be8;
        }
LAB_00177ef0:
        iVar7 = FUN_0017609c(puVar3);
        if (((iVar7 == 0) ||
            (uVar11 = (&DAT_001c3570)[uVar9 * 0x1b], uVar11 == local_c78 || uVar11 == local_c68)) ||
           ((uVar12 = (&DAT_001c3580)[uVar9 * 0x1b], uVar12 != 0 &&
            ((uVar12 == local_c78 || (uVar12 == local_c68)))))) goto LAB_00177be8;
        if (uVar9 != 0) {
          puVar13 = &DAT_001c3580;
          uVar14 = uVar9;
          do {
            if (((uVar11 == puVar13[-2]) || (uVar11 == *puVar13)) ||
               ((uVar12 != 0 && ((uVar12 == puVar13[-2] || (uVar12 == *puVar13))))))
            goto LAB_00177be8;
            uVar14 = uVar14 - 1;
            puVar13 = puVar13 + 0x1b;
          } while (uVar14 != 0);
        }
        if ((&DAT_001a7d68)[uVar9 * 0x2c] == 2) {
          iVar7 = FUN_0017c704(puVar3,1,local_c80);
          if (iVar7 == 0) goto LAB_00177be8;
        }
        else if ((uVar12 != 0) || (uVar11 != *puVar3)) goto LAB_00177be8;
      }
      else if ((((uVar11 != 0) || ((&DAT_001c3570)[uVar9 * 0x1b] != 0)) ||
               ((&DAT_001c3580)[uVar9 * 0x1b] != 0)) ||
              (((&DAT_001c3578)[uVar9 * 0x1b] != 0 || ((&DAT_001c3638)[uVar9 * 0x36] != 0))))
      goto LAB_00177be8;
      uVar9 = uVar9 + 1;
    } while (uVar9 != 0x280);
    if (DAT_00281a50 != 0) {
      if (((((DAT_00281a50 != param_1) && (DAT_00281a50 != param_2)) && (DAT_00281a60 != 0)) &&
          (((DAT_00281a58 == param_2 && (iVar7 = FUN_0016dc20(DAT_00281a50,5,0), iVar7 != 0)) &&
           ((iVar7 = FUN_0016d5cc(DAT_00281a50,param_2), iVar7 != 0 &&
            ((iVar7 = FUN_0016e464(DAT_00281a50,0), iVar7 != 0 &&
             (iVar7 = FUN_0016debc(DAT_00281a60,0x11abad8), iVar7 != 0)))))))) &&
         ((iVar7 = FUN_0016df78(DAT_00281a60), iVar7 != 0 &&
          (iVar7 = FUN_0016d800(DAT_00281a60,DAT_00281a50), iVar7 != 0)))) goto LAB_0017812c;
      goto LAB_00177be8;
    }
    if (DAT_00281a60 != 0 || DAT_00281a58 != 0) goto LAB_00177be8;
LAB_0017812c:
    uVar8 = FUN_00176d98(local_c80,param_3);
    if (((int)uVar8 != 0) && (uVar8 = FUN_0014f830(0), (int)uVar8 != 0)) {
      if (param_4 == 0) {
        uVar8 = 1;
      }
      else {
        iVar7 = FUN_00176d98(local_c80,param_3);
        if (iVar7 == 0) {
LAB_00178698:
          if (local_c90 == 0) goto LAB_00177be8;
        }
        else {
          if (DAT_00284650 != 0) {
            local_c90 = 1;
            iVar7 = FUN_00182b58(DAT_00284650,DAT_00284658);
            if (iVar7 == 0) goto LAB_00178698;
            DAT_00284670 = 0;
            DAT_00284658 = 0;
            DAT_00284650 = 0;
            _DAT_00284668 = 0;
            DAT_00284660 = 0;
          }
          if (local_80 != 0) {
            uVar9 = 0;
            puVar17 = local_c70;
            do {
              uVar10 = *puVar17;
              uVar11 = **(ulong **)(puVar17 + -4);
              if (uVar10 < 2) {
                iVar7 = FUN_00177834(uVar11,uVar10,&local_c90);
                if (iVar7 == 0) goto LAB_00178698;
              }
              else {
                if (uVar10 == 3) {
                  local_c88 = CONCAT62(local_c88._2_6_,1);
                  if ((((uVar11 + 0x4e < 0x1000) ||
                       ((uVar11 & 0xfffffffffffffffe) == 0xffffffffffffffb0)) ||
                      (iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar11 + 0x4e,&local_c88,2), iVar7 != 1)
                      ) || ((short)local_c88 != 0)) goto LAB_00178698;
                  uVar10 = *puVar17;
                }
                local_c90 = 1;
                uVar4 = 0x5d48e8;
                if (uVar10 != 4) {
                  (*(code *)(DAT_001a7cd0 + 0x59567c))(uVar11);
                  local_c88 = 1;
                  local_c8c = 0;
                  if (((((uVar11 + 0x38 < 0x1000) ||
                        ((uVar11 & 0xfffffffffffffff8) == 0xffffffffffffffc0)) ||
                       ((iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar11 + 0x38,&local_c88,8),
                        iVar7 != 1 || ((local_c88 != 0 || (uVar11 + 0x40 < 0x1000)))))) ||
                      ((uVar11 & 0xfffffffffffffffc) == 0xffffffffffffffbc)) ||
                     ((iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar11 + 0x40,&local_c8c,4), iVar7 != 1
                      || (local_c8c != -1)))) goto LAB_00178698;
                  uVar10 = 0x5940d0;
                  if (*puVar17 != 3) {
                    uVar10 = 0x5d48e8;
                  }
                  uVar4 = 0x88991c;
                  if (*puVar17 != 2) {
                    uVar4 = uVar10;
                  }
                }
                (*(code *)(DAT_001a7cd0 + (ulong)uVar4))(uVar11);
              }
              puVar2 = puVar17 + -4;
              uVar9 = uVar9 + 1;
              puVar17 = puVar17 + 8;
              **(undefined8 **)puVar2 = 0;
            } while (uVar9 < local_80);
          }
          memset(&DAT_00283338,0,0x6b8);
          memset(&DAT_00282b10,0,0x820);
          memset(&DAT_00283a00,0,0x820);
          uRam0000000000284228 = 0;
          DAT_00284220 = 0;
          uRam0000000000284238 = 0;
          _DAT_00284230 = 0;
          uRam0000000000284248 = 0;
          _DAT_00284240 = 0;
          uRam0000000000284258 = 0;
          _DAT_00284250 = 0;
          uRam0000000000284268 = 0;
          _DAT_00284260 = 0;
          uRam0000000000284278 = 0;
          _DAT_00284270 = 0;
          uRam0000000000284288 = 0;
          _DAT_00284280 = 0;
          uRam0000000000284298 = 0;
          _DAT_00284290 = 0;
          uRam00000000002842a8 = 0;
          _DAT_002842a0 = 0;
          uRam00000000002842b8 = 0;
          _DAT_002842b0 = 0;
          uRam00000000002842c8 = 0;
          _DAT_002842c0 = 0;
          uRam00000000002842d8 = 0;
          _DAT_002842d0 = 0;
          uRam00000000002842e8 = 0;
          _DAT_002842e0 = 0;
          DAT_002842f8 = 0;
          _DAT_002842f0 = 0;
          uRam0000000000284308 = 0;
          _DAT_00284300 = 0;
          uRam0000000000284318 = 0;
          _DAT_00284310 = 0;
          uRam0000000000284328 = 0;
          _DAT_00284320 = 0;
          uRam0000000000284338 = 0;
          _DAT_00284330 = 0;
          uRam0000000000284348 = 0;
          _DAT_00284340 = 0;
          uRam0000000000284358 = 0;
          _DAT_00284350 = 0;
          uRam0000000000284368 = 0;
          _DAT_00284360 = 0;
          uRam0000000000284378 = 0;
          _DAT_00284370 = 0;
          uRam0000000000284388 = 0;
          _DAT_00284380 = 0;
          uRam0000000000284398 = 0;
          _DAT_00284390 = 0;
          uRam00000000002843a8 = 0;
          _DAT_002843a0 = 0;
          uRam00000000002843b8 = 0;
          _DAT_002843b0 = 0;
          uRam00000000002843c8 = 0;
          _DAT_002843c0 = 0;
          DAT_002839fc = 0;
          DAT_002843d8 = 0;
          DAT_002843d0 = 0;
          DAT_00283330 = 0;
          uRam00000000002843e8 = 0;
          DAT_002843e0 = 0;
          uRam00000000002843f8 = 0;
          _DAT_002843f0 = 0;
          uRam0000000000284408 = 0;
          _DAT_00284400 = 0;
          uRam0000000000284418 = 0;
          _DAT_00284410 = 0;
          uRam0000000000284428 = 0;
          _DAT_00284420 = 0;
          uRam0000000000284438 = 0;
          _DAT_00284430 = 0;
          uRam0000000000284448 = 0;
          _DAT_00284440 = 0;
          uRam0000000000284458 = 0;
          _DAT_00284450 = 0;
          uRam0000000000284468 = 0;
          _DAT_00284460 = 0;
          uRam0000000000284478 = 0;
          _DAT_00284470 = 0;
          uRam0000000000284488 = 0;
          _DAT_00284480 = 0;
          uRam0000000000284498 = 0;
          _DAT_00284490 = 0;
          uRam00000000002844a8 = 0;
          _DAT_002844a0 = 0;
          DAT_002844b8 = 0;
          DAT_002844b0 = 0;
          _DAT_002844c8 = 0;
          DAT_002844c0 = 0;
          _DAT_002844d0 = 0;
          memset(&DAT_00282150,0,0x9b8);
          DAT_002844f0 = 0;
          DAT_002844e8 = 0;
          DAT_00284500 = 0;
          DAT_002844f8 = 0;
          uRam0000000000284510 = 0;
          _DAT_00284508 = 0;
          uRam0000000000284520 = 0;
          _DAT_00284518 = 0;
          uRam0000000000284530 = 0;
          _DAT_00284528 = 0;
          uRam0000000000284540 = 0;
          _DAT_00284538 = 0;
          uRam0000000000284550 = 0;
          _DAT_00284548 = 0;
          uRam0000000000284560 = 0;
          _DAT_00284558 = 0;
          uRam0000000000284570 = 0;
          _DAT_00284568 = 0;
          uRam0000000000284580 = 0;
          _DAT_00284578 = 0;
          uRam0000000000284590 = 0;
          _DAT_00284588 = 0;
          uRam00000000002845a0 = 0;
          _DAT_00284598 = 0;
          uRam00000000002845b0 = 0;
          _DAT_002845a8 = 0;
          uRam00000000002845c0 = 0;
          _DAT_002845b8 = 0;
          uRam00000000002845d0 = 0;
          _DAT_002845c8 = 0;
          uRam00000000002845e0 = 0;
          _DAT_002845d8 = 0;
          uRam00000000002845f0 = 0;
          _DAT_002845e8 = 0;
          uRam0000000000284600 = 0;
          _DAT_002845f8 = 0;
          uRam0000000000284610 = 0;
          _DAT_00284608 = 0;
          uRam0000000000284620 = 0;
          _DAT_00284618 = 0;
          uRam0000000000284630 = 0;
          _DAT_00284628 = 0;
          uRam0000000000284640 = 0;
          _DAT_00284638 = 0;
          DAT_00284648 = 0;
          memset(&DAT_00281b08,0,0x648);
          iVar7 = FUN_0014f830(1);
          if (iVar7 == 1) {
            local_c90 = 1;
            if (DAT_001a7d18 != 0) {
              piVar15 = &DAT_001a7d68;
              uVar9 = 0;
              puVar16 = &DAT_001c3568;
              do {
                iVar7 = FUN_00177834(*puVar16,*piVar15 == 2,&local_c90);
                if (iVar7 == 0) goto LAB_00178698;
                puVar16[0x1a] = 0;
                uVar9 = uVar9 + 1;
                piVar15 = piVar15 + 0x2c;
                puVar16[3] = 0;
                puVar16[2] = 0;
                puVar16[5] = 0;
                puVar16[4] = 0;
                puVar16[7] = 0;
                puVar16[6] = 0;
                puVar16[9] = 0;
                puVar16[8] = 0;
                puVar16[0xb] = 0;
                puVar16[10] = 0;
                puVar16[0xd] = 0;
                puVar16[0xc] = 0;
                puVar16[0xf] = 0;
                puVar16[0xe] = 0;
                puVar16[0x11] = 0;
                puVar16[0x10] = 0;
                puVar16[0x13] = 0;
                puVar16[0x12] = 0;
                puVar16[0x15] = 0;
                puVar16[0x14] = 0;
                puVar16[0x17] = 0;
                puVar16[0x16] = 0;
                puVar16[0x19] = 0;
                puVar16[0x18] = 0;
                puVar16[1] = 0;
                *puVar16 = 0;
                puVar16 = puVar16 + 0x1b;
              } while (uVar9 < DAT_001a7d18);
            }
            iVar7 = FUN_00178714();
            if (iVar7 != 0) {
              if (DAT_001a7d38 == 0) {
LAB_00178584:
                if (DAT_00281a50 != 0) {
                  iVar7 = FUN_00177834(DAT_00281a50,0,&local_c90);
                  if (iVar7 == 0) goto LAB_00178698;
                  DAT_00281a58 = 0;
                  DAT_00281a60 = 0;
                  DAT_00281a50 = 0;
                }
                iVar7 = FUN_00177834(param_3,1,&local_c90);
                if (iVar7 != 0) {
                  _DAT_001a7d10 = 0;
                  DAT_001a7d18 = 0;
                  DAT_001a7d48 = 0;
                  DAT_001a7d30 = 0;
                  DAT_001a7d28 = 0;
                  DAT_001a7d40 = 0;
                  DAT_001a7d38 = 0;
                  FUN_00178ab0();
                  memset(&DAT_001a7d58,0,0x3d410);
                  uVar8 = 1;
                  DAT_00281a60 = 0;
                  DAT_001e5238 = 0;
                  uRam00000000001e51f0 = 0;
                  _DAT_001e51e8 = 0;
                  uRam00000000001e5200 = 0;
                  _DAT_001e51f8 = 0;
                  uRam00000000001e5210 = 0;
                  _DAT_001e5208 = 0;
                  uRam00000000001e5220 = 0;
                  _DAT_001e5218 = 0;
                  uRam00000000001e5230 = 0;
                  _DAT_001e5228 = 0;
                  DAT_00281a50 = 0;
                  _DAT_001e53fc = 0;
                  PTR_s_not_initialized_001a7d50 = s_waiting_resources_after_reload_00136325;
                  DAT_001e5400._1_3_ = 0;
                  _DAT_001e5404 = 0;
                  DAT_00281a6c = 0;
                  DAT_00281a58 = 0;
                  DAT_00281a74 = 0;
                  DAT_00281a68 = 0;
                  DAT_00281a70 = 0;
                  DAT_00281b00 = 0;
                  DAT_00281af8 = 0;
                  DAT_00281afc = 0;
                  DAT_00281af4 = 0;
                  goto LAB_00177bec;
                }
              }
              else {
                local_c80[0] = 1;
                if ((((0xfff < DAT_001a7d38 + 0x4e) &&
                     ((DAT_001a7d38 & 0xfffffffffffffffe) != 0xffffffffffffffb0)) &&
                    (iVar7 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_001a7d38 + 0x4e,local_c80,2),
                    iVar7 == 1)) && (local_c80[0] == 0)) {
                  local_c90 = 1;
                  iVar7 = FUN_001789a8(DAT_001a7d38);
                  if (iVar7 != 0) {
                    (*(code *)(DAT_001a7cd0 + 0x5940d0))(DAT_001a7d38);
                    DAT_001a7d38 = 0;
                    goto LAB_00178584;
                  }
                }
              }
            }
            goto LAB_00178698;
          }
          if (-1 < iVar7) goto LAB_00178698;
          local_c90 = 1;
        }
        uVar8 = 0xffffffff;
        DAT_001a7d28 = 0;
        DAT_001a7d30 = 0;
        _DAT_001a7d10 = CONCAT44(DAT_001a7d14,0xffffffff);
        PTR_s_not_initialized_001a7d50 = s_graphics_owned_release_unverifie_0013bb0e;
        uVar10 = DAT_001a7d20 + 1;
        bVar1 = DAT_001a7d20 < 0x40;
        DAT_001a7d20 = uVar10;
        if ((bVar1) && (DAT_001a7d00 != (code *)0x0)) {
          (*DAT_001a7d00)(DAT_001a7cc8,"nexus_rich_stopped","graphics_owned_release_unverified",0);
          uVar8 = 0xffffffff;
        }
      }
    }
  }
LAB_00177bec:
  DAT_001a7d08 = 0;
LAB_00177bf8:
  if (*(long *)(lVar5 + 0x28) == local_70) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar8);
}

/* ===== nexus_rich_main_frame @ 00178c78 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void nexus_rich_main_frame(undefined8 param_1,uint param_2,int param_3)

{
  bool bVar1;
  uint uVar2;
  long lVar3;
  int iVar4;
  undefined8 uVar5;
  ulong uVar6;
  undefined4 local_1f0;
  char local_1ec [4];
  undefined1 auStack_1e8 [400];
  long local_58;
  
  lVar3 = tpidr_el0;
  local_58 = *(long *)(lVar3 + 0x28);
  if (DAT_001a7d0c == 0) {
LAB_00178ddc:
    uVar5 = 0;
    goto LAB_00178fc0;
  }
  iVar4 = (*DAT_001a7cf0)(DAT_001a7cc8);
  uVar5 = 0;
  if ((1 < param_2) || (iVar4 == 0)) goto LAB_00178fc0;
  uVar6 = FUN_00193f80(1,&DAT_001a7d08);
  if ((uVar6 & 1) != 0) goto LAB_00178ddc;
  iVar4 = nexus_menu_main_count();
  uVar6 = (ulong)(iVar4 + 4U) / 5;
  if ((((param_3 == 0) || (DAT_0022d798 == 0)) || (DAT_001a7d10 != 2)) || (DAT_001a7d14 != 0)) {
    if (DAT_0022d560 == 0) {
      DAT_0022d5f8 = 0;
      DAT_0022d560 = 1;
      _DAT_0022d5f0 = 0;
      _DAT_0022d5e8 = 0;
      _DAT_0022d5e0 = 0;
      _DAT_0022d5d8 = 0;
      DAT_0022d5d0 = 0;
      DAT_0022d5c8 = 0;
      _DAT_0022d5c0 = 0;
      _DAT_0022d5b8 = 0;
      _DAT_0022d5b0 = 0;
      DAT_0022d5a8 = 0;
      DAT_0022d5a0 = 0;
      DAT_0022d598 = 0;
      _DAT_0022d568 = 0;
      _DAT_0022d580 = 0xffffffff00000000;
      DAT_0022d588 = DAT_0022d7a8;
      DAT_0022d590 = param_1;
    }
    else if (DAT_0022d588 != DAT_0022d7a8) {
      _DAT_0022d580 = 0xffffffff00000000;
      DAT_0022d588 = DAT_0022d7a8;
    }
    _DAT_0022d580 = CONCAT44(DAT_0022d584,(iVar4 + 4U) / 5);
    _DAT_0022d578 = _UNK_0010fa98;
    _DAT_0022d570 = _DAT_0010fa90;
    DAT_0022d564 = 0;
    FUN_00183230(0,0,&DAT_0022d518,param_1,0,4,0xffffffff,uVar6);
    DAT_0022d590 = param_1;
LAB_00178fb8:
    uVar5 = 0;
    DAT_0022d794 = 0;
  }
  else {
    iVar4 = FUN_0016d910();
    if ((((iVar4 == 0) || (local_1ec[0] = '\x01', DAT_001a7d28 + 0x19dU < 0x1001)) ||
        ((iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_001a7d28 + 0x19c,local_1ec,1), iVar4 != 1 ||
         ((local_1ec[0] != '\0' || (iVar4 = FUN_00179448(), iVar4 == 0)))))) ||
       (iVar4 = FUN_001795ac(), iVar4 == 0)) {
LAB_00178f78:
      DAT_0022d564 = 0;
      _DAT_0022d578 = _UNK_0010fa98;
      _DAT_0022d570 = _DAT_0010fa90;
      FUN_00183230(0,0,&DAT_0022d518,param_1,0,4,0xffffffff,uVar6);
      goto LAB_00178fb8;
    }
    if (param_2 != 0) {
      DAT_0022d7b8 = param_1;
      iVar4 = FUN_0017975c(1,&DAT_0022d600,&DAT_0022d790);
      uVar5 = 0;
      DAT_0022d794 = (uint)(iVar4 == 1);
      if ((iVar4 < 0) || (iVar4 == 1)) goto LAB_00178fbc;
      goto LAB_00178f78;
    }
    local_1f0 = 0;
    iVar4 = FUN_0017975c(0,auStack_1e8,&local_1f0);
    if (iVar4 < 0) goto LAB_00178f70;
    if (iVar4 == 0) goto LAB_00178f78;
    FUN_00179000(param_1,1,DAT_0022d7a8,auStack_1e8,local_1f0,uVar6);
    DAT_0022d7c0 = DAT_0022d7c0 + 1;
    iVar4 = nexus_rich_main_row();
    if (iVar4 == DAT_0022d79c) {
      uVar5 = FUN_00179c50(DAT_0022d518);
      if ((int)uVar5 == 0) {
        DAT_001a7d10 = -1;
        PTR_s_not_initialized_001a7d50 = s_main_page_motion_0013854d;
        uVar2 = DAT_001a7d20 + 1;
        bVar1 = 0x3f < DAT_001a7d20;
        DAT_001a7d20 = uVar2;
        if ((bVar1) || (DAT_001a7d00 == (code *)0x0)) goto LAB_00178fbc;
        (*DAT_001a7d00)(DAT_001a7cc8,"nexus_rich_stopped","main_page_motion",0);
      }
LAB_00178f70:
      uVar5 = 0;
    }
    else {
      uVar5 = 1;
      DAT_001a7d40 = 0;
    }
  }
LAB_00178fbc:
  DAT_001a7d08 = 0;
LAB_00178fc0:
  if (*(long *)(lVar3 + 0x28) != local_58) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar5);
  }
  return;
}

/* ===== FUN_00179f68 @ 00179f68 ===== */

void FUN_00179f68(float param_1,float param_2,float param_3,ulong *param_4,char *param_5)

{
  ulong uVar1;
  long lVar2;
  bool bVar3;
  undefined8 uVar4;
  code *pcVar5;
  float fVar6;
  ulong uVar7;
  bool bVar8;
  int iVar9;
  size_t sVar10;
  undefined8 uVar11;
  ulong uVar12;
  ulong uVar13;
  uint uVar14;
  float fVar15;
  short local_9c [2];
  undefined8 local_98;
  float local_8c;
  long local_88;
  
  lVar2 = tpidr_el0;
  local_88 = *(long *)(lVar2 + 0x28);
  if ((param_4[2] == 0) || (param_4[2] == DAT_001a7d30)) {
    if (*param_4 != 0) {
      iVar9 = FUN_0016d5cc();
      if (iVar9 != 0) {
        uVar12 = *param_4;
        uVar13 = param_4[1];
        if (uVar13 != 0) {
          uVar14 = 0;
          do {
            if ((uVar13 == uVar12) || (0xf < uVar14)) goto LAB_0017a088;
            local_98 = 0;
            if (uVar13 + 0x40 >> 3 < 0x201) {
              bVar8 = false;
            }
            else {
              iVar9 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar13 + 0x38,&local_98,8);
              bVar8 = iVar9 == 1;
            }
            uVar7 = local_98;
            bVar3 = false;
            if (0xfff < local_98) {
              bVar3 = bVar8;
            }
            bVar3 = (bool)(bVar3 & (local_98 & 7) == 0);
            uVar1 = local_98;
            if (!bVar3) {
              uVar1 = 0;
            }
            iVar9 = FUN_0016d5cc(uVar13,uVar1);
            if (iVar9 == 0) goto LAB_00179fc8;
            uVar14 = uVar14 + 1;
            uVar13 = uVar7;
          } while (bVar3);
          uVar13 = 0;
        }
LAB_0017a088:
        if (uVar13 == uVar12) goto LAB_0017a090;
      }
      goto LAB_00179fc8;
    }
  }
  else {
LAB_00179fc8:
    memset(param_4,0,0x820);
  }
LAB_0017a090:
  if ((param_5 == (char *)0x0) || (*param_5 == '\0')) {
    if (*param_4 != 0) {
      FUN_0016cf5c(0xc61c3c00,0xc61c3c00);
    }
    *(undefined4 *)((long)param_4 + 0x81c) = 0;
    goto LAB_0017a328;
  }
  if (((int)param_4[0x103] != 0) || (iVar9 = FUN_0016d910(), iVar9 == 0)) goto LAB_0017a328;
  local_98 = CONCAT71(local_98._1_7_,1);
  if ((DAT_001a7d28 + 0x19dU < 0x1001) ||
     ((iVar9 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_001a7d28 + 0x19c,&local_98,1), pcVar5 = DAT_001a7cf8
      , uVar4 = DAT_001a7cc8, iVar9 != 1 || ((char)local_98 != '\0')))) goto LAB_0017a328;
  if (*param_4 == 0) {
    uVar11 = nexus_rich_asset(5);
    iVar9 = (*pcVar5)(uVar4,uVar11);
    if ((iVar9 == 0) || (iVar9 = FUN_00172da8(), iVar9 == 0)) goto LAB_0017a328;
    nexus_rich_asset(5);
    uVar12 = FUN_0016ce48();
    iVar9 = FUN_0016dc20(uVar12,5,0);
    if ((iVar9 != 0) && (iVar9 = FUN_0016ddc8(uVar12), iVar9 != 0)) {
      FUN_0016ce84(uVar12,0);
      uVar13 = FUN_0016ce98(uVar12,&DAT_00134a97);
      iVar9 = FUN_0016debc(uVar13,0x11abad8);
      if (((iVar9 != 0) && (iVar9 = FUN_0016df78(uVar13), iVar9 != 0)) &&
         (iVar9 = FUN_0016e020(uVar12,uVar13), iVar9 != 0)) {
        FUN_0016ce20(uVar12,0);
        FUN_0016cf5c(0xc61c3c00,0xc61c3c00,uVar12);
        FUN_0016cfb8(DAT_001a7d28,uVar12);
        iVar9 = FUN_0016d5cc(uVar12,DAT_001a7d30);
        if ((iVar9 != 0) && (iVar9 = FUN_0016e464(uVar12,0), iVar9 != 0)) {
          *param_4 = uVar12;
          param_4[1] = uVar13;
          param_4[2] = DAT_001a7d30;
          goto LAB_0017a100;
        }
      }
    }
    *(undefined4 *)(param_4 + 0x103) = 1;
  }
  else {
LAB_0017a100:
    iVar9 = strcmp(param_5,(char *)(param_4 + 3));
    if (iVar9 != 0) {
      sVar10 = strlen(param_5);
      if (0x7ff < sVar10) goto LAB_0017a328;
      FUN_0016ced8(&local_98,param_5);
      FUN_0016ceac(param_4[1],&local_98);
      (*(code *)(DAT_001a7cd0 + 0x66ad48))(&local_98);
      memcpy(param_4 + 3,param_5,sVar10 + 1);
      FUN_0016ceec(param_4[1],0xffffffff);
    }
    bVar8 = true;
    local_9c[0] = 0;
    uVar12 = param_4[1] + 0xb0;
    if ((0xfff < uVar12) && ((param_4[1] & 0xfffffffffffffffe) != 0xffffffffffffff4e)) {
      iVar9 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar12,local_9c,2);
      bVar8 = iVar9 != 1;
    }
    if ((((!bVar8) && (iVar9 = (int)local_9c[0], 0 < iVar9)) && (iVar9 < 0x101)) &&
       (0.0 < DAT_001e53fc)) {
      param_3 = param_3 / (DAT_001e53fc * (float)iVar9);
      if (((ABS(param_3) != INFINITY) && (!NAN(ABS(param_3)))) &&
         ((0.01 <= param_3 && (param_3 == 8.0 || param_3 < 8.0 != NAN(param_3))))) {
        FUN_0016cf5c(0,0,*param_4);
        FUN_0016cf5c(0,0,param_4[1]);
        FUN_0016cf00(param_3,param_3,param_4[1]);
        FUN_0016cf84(param_4[1],&local_98);
        fVar6 = DAT_001e53fc;
        if ((ABS((float)local_98) != INFINITY) && (!NAN(ABS((float)local_98)))) {
          if ((ABS(local_98._4_4_) != INFINITY) &&
             (((!NAN(ABS(local_98._4_4_)) && (ABS(local_8c) != INFINITY)) && (!NAN(ABS(local_8c)))))
             ) {
            fVar15 = (local_8c - local_98._4_4_) * DAT_001e53fc;
            if (fVar15 <= 0.0) {
              fVar15 = 0.0;
            }
            *(float *)((long)param_4 + 0x81c) = fVar15;
            FUN_0016cf5c((param_1 - DAT_001e5400) / fVar6 - (float)local_98,
                         (param_2 - DAT_001e5404) / fVar6 - local_98._4_4_,param_4[1]);
            FUN_0016cf5c(0,0,*param_4);
          }
        }
      }
    }
  }
LAB_0017a328:
  if (*(long *)(lVar2 + 0x28) == local_88) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0017bf48 @ 0017bf48 ===== */

void FUN_0017bf48(ulong *param_1,undefined4 param_2,undefined8 param_3)

{
  long lVar1;
  bool bVar2;
  bool bVar3;
  int iVar4;
  ulong uVar5;
  ulong uVar6;
  undefined8 uVar7;
  int local_134;
  ulong local_130 [28];
  
  lVar1 = tpidr_el0;
  local_130[0x1b] = *(long *)(lVar1 + 0x28);
  nexus_rich_asset(param_2);
  uVar5 = FUN_0016ce48();
  uVar6 = FUN_0016dc20(uVar5,param_2,0);
  if ((int)uVar6 == 0) goto LAB_0017c030;
  local_134 = 0;
  local_130[0] = 1;
  if (((((0xfff < uVar5 + 0x38) && ((uVar5 & 0xfffffffffffffff8) != 0xffffffffffffffc0)) &&
       (iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar5 + 0x38,local_130,8), iVar4 == 1)) &&
      ((local_130[0] == 0 && (0xfff < uVar5 + 0x40)))) &&
     (((uVar5 & 0xfffffffffffffffc) != 0xffffffffffffffbc &&
      ((iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar5 + 0x40,&local_134,4), iVar4 == 1 &&
       (local_134 == -1)))))) {
    FUN_0016ce84(uVar5,0);
    uVar6 = FUN_0016cdf0(0x260);
    *param_1 = uVar6;
    if (uVar6 == 0) goto LAB_0017c030;
    FUN_0016ce34();
    local_130[0] = 0;
    if (*param_1 + 8 >> 3 < 0x201) {
      bVar3 = false;
    }
    else {
      iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,*param_1,local_130,8);
      bVar3 = iVar4 == 1;
    }
    bVar2 = false;
    if (0xfff < local_130[0]) {
      bVar2 = bVar3;
    }
    uVar6 = local_130[0];
    if (!(bool)(bVar2 & (local_130[0] & 7) == 0)) {
      uVar6 = 0;
    }
    if (uVar6 == DAT_001a7cd0 + 0x11c0a48U) {
      uVar6 = *param_1;
      local_134 = 0;
      local_130[0] = 1;
      if ((((0xfff < uVar6 + 0x38) && ((uVar6 & 0xfffffffffffffff8) != 0xffffffffffffffc0)) &&
          (iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar6 + 0x38,local_130,8), iVar4 == 1)) &&
         (((local_130[0] == 0 && (0xfff < uVar6 + 0x40)) &&
          (((uVar6 & 0xfffffffffffffffc) != 0xffffffffffffffbc &&
           ((iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar6 + 0x40,&local_134,4), iVar4 == 1 &&
            (local_134 == -1)))))))) {
        uVar6 = FUN_0014e5ac(*param_1);
        if ((int)uVar6 == 0) goto LAB_0017c030;
        local_130[0x1a] = 0;
        local_130[0x19] = 0;
        local_130[0x18] = 0;
        local_130[0x17] = 0;
        local_130[0x16] = 0;
        local_130[0x15] = 0;
        local_130[0x14] = 0;
        local_130[0x13] = 0;
        local_130[0x12] = 0;
        local_130[0x11] = 0;
        local_130[0] = *param_1;
        local_130[0x10] = 0;
        local_130[0xf] = 0;
        local_130[0xe] = 0;
        local_130[0xd] = 0;
        local_130[0xc] = 0;
        local_130[0xb] = 0;
        local_130[10] = 0;
        local_130[9] = 0;
        local_130[8] = 0;
        local_130[7] = 0;
        local_130[6] = 0;
        local_130[5] = 0;
        local_130[4] = 0;
        local_130[3] = 0;
        local_130[2] = 0;
        local_130[1] = 0;
        uVar6 = FUN_0017c42c(local_130,uVar5,0);
        if ((int)uVar6 == 0) goto LAB_0017c030;
        uVar6 = *param_1;
        param_1[1] = uVar5;
        uVar7 = FUN_0017b858(param_3);
        FUN_0016cec0(uVar6,uVar7);
        FUN_0016cf5c(0xc61c3c00,0xc61c3c00,*param_1);
        FUN_0016cfa4(DAT_00283340,*param_1);
        uVar6 = FUN_0017c598(*param_1);
        if ((int)uVar6 == 0) goto LAB_0017c030;
        local_134 = CONCAT31(local_134._1_3_,0xff);
        if (0x1000 < *param_1 + 0x49) {
          iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,*param_1 + 0x48,&local_134,1);
          uVar6 = (ulong)(iVar4 == 1 && (char)local_134 == '\x01');
          goto LAB_0017c030;
        }
      }
    }
  }
  uVar6 = 0;
LAB_0017c030:
  if (*(long *)(lVar1 + 0x28) == local_130[0x1b]) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar6);
}

/* ===== FUN_0017c42c @ 0017c42c ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_0017c42c(undefined8 *param_1,undefined8 param_2,undefined4 param_3)

{
  bool bVar1;
  uint uVar2;
  long lVar3;
  int iVar4;
  undefined8 local_88;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined8 local_70;
  undefined8 local_68;
  undefined8 uStack_60;
  undefined8 local_58;
  undefined4 uStack_50;
  undefined4 uStack_4c;
  short local_48;
  undefined2 uStack_46;
  undefined4 uStack_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  long local_38;
  
  lVar3 = tpidr_el0;
  local_38 = *(long *)(lVar3 + 0x28);
  FUN_0016ce6c(*param_1);
  param_1[1] = param_2;
  iVar4 = FUN_0017c704(param_1,0,&local_88);
  uRam00000000001e5220 = CONCAT44(uStack_4c,uStack_50);
  uRam00000000001e5230 = CONCAT44(uStack_3c,uStack_40);
  _DAT_001e5228 = CONCAT44(uStack_44,CONCAT22(uStack_46,local_48));
  uRam00000000001e5210 = uStack_60;
  _DAT_001e5208 = local_68;
  _DAT_001e5218 = local_58;
  uRam00000000001e51f0 = local_80;
  _DAT_001e51e8 = local_88;
  uRam00000000001e5200 = local_70;
  _DAT_001e51f8 = uStack_78;
  FUN_00176a24(&DAT_001e5238,0x1c0,0x1c0,
               "ok=%d mode=%u reads=%x button=%llx source=%llx clip80=%llx display88=%llx sourceb0=%llx mcparent=%llx mcindex=%d displayparent=%llx displayindex=%d named=%d matches=%u active=%u"
               ,iVar4,uStack_3c,uStack_44,local_88,local_80,uStack_78,local_70,local_68,uStack_60,
               uStack_50,local_58,uStack_4c,(int)local_48,uStack_40,uStack_46);
  if ((iVar4 == 0) ||
     (uVar2 = DAT_001e53f8 + 1, bVar1 = DAT_001e53f8 < 4, DAT_001e53f8 = uVar2, bVar1)) {
    uVar2 = DAT_001a7d20 + 1;
    bVar1 = DAT_001a7d20 < 0x40;
    DAT_001a7d20 = uVar2;
    if ((bVar1) && (DAT_001a7d00 != (code *)0x0)) {
      (*DAT_001a7d00)(DAT_001a7cc8,"nexus_rich_binding",&DAT_001e5238,param_3);
    }
    if (iVar4 == 0) goto LAB_0017c56c;
  }
  param_1[3] = local_70;
LAB_0017c56c:
  if (*(long *)(lVar3 + 0x28) == local_38) {
    return iVar4;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0017d8c4 @ 0017d8c4 ===== */

void FUN_0017d8c4(ulong *param_1,undefined4 param_2,undefined8 param_3)

{
  long lVar1;
  bool bVar2;
  bool bVar3;
  int iVar4;
  ulong uVar5;
  ulong uVar6;
  undefined8 uVar7;
  int local_134;
  ulong local_130 [28];
  
  lVar1 = tpidr_el0;
  local_130[0x1b] = *(long *)(lVar1 + 0x28);
  nexus_rich_asset(param_2);
  uVar5 = FUN_0016ce48();
  uVar6 = FUN_0016dc20(uVar5,param_2,0);
  if ((int)uVar6 == 0) goto LAB_0017d9ac;
  local_134 = 0;
  local_130[0] = 1;
  if (((((0xfff < uVar5 + 0x38) && ((uVar5 & 0xfffffffffffffff8) != 0xffffffffffffffc0)) &&
       (iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar5 + 0x38,local_130,8), iVar4 == 1)) &&
      ((local_130[0] == 0 && (0xfff < uVar5 + 0x40)))) &&
     (((uVar5 & 0xfffffffffffffffc) != 0xffffffffffffffbc &&
      ((iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar5 + 0x40,&local_134,4), iVar4 == 1 &&
       (local_134 == -1)))))) {
    FUN_0016ce84(uVar5,0);
    uVar6 = FUN_0016cdf0(0x260);
    *param_1 = uVar6;
    if (uVar6 == 0) goto LAB_0017d9ac;
    FUN_0016ce34();
    local_130[0] = 0;
    if (*param_1 + 8 >> 3 < 0x201) {
      bVar3 = false;
    }
    else {
      iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,*param_1,local_130,8);
      bVar3 = iVar4 == 1;
    }
    bVar2 = false;
    if (0xfff < local_130[0]) {
      bVar2 = bVar3;
    }
    uVar6 = local_130[0];
    if (!(bool)(bVar2 & (local_130[0] & 7) == 0)) {
      uVar6 = 0;
    }
    if (uVar6 == DAT_001a7cd0 + 0x11c0a48U) {
      uVar6 = *param_1;
      local_134 = 0;
      local_130[0] = 1;
      if ((((0xfff < uVar6 + 0x38) && ((uVar6 & 0xfffffffffffffff8) != 0xffffffffffffffc0)) &&
          (iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar6 + 0x38,local_130,8), iVar4 == 1)) &&
         (((local_130[0] == 0 && (0xfff < uVar6 + 0x40)) &&
          (((uVar6 & 0xfffffffffffffffc) != 0xffffffffffffffbc &&
           ((iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar6 + 0x40,&local_134,4), iVar4 == 1 &&
            (local_134 == -1)))))))) {
        uVar6 = FUN_0014e5ac(*param_1);
        if ((int)uVar6 == 0) goto LAB_0017d9ac;
        local_130[0x1a] = 0;
        local_130[0x19] = 0;
        local_130[0x18] = 0;
        local_130[0x17] = 0;
        local_130[0x16] = 0;
        local_130[0x15] = 0;
        local_130[0x14] = 0;
        local_130[0x13] = 0;
        local_130[0x12] = 0;
        local_130[0x11] = 0;
        local_130[0] = *param_1;
        local_130[0x10] = 0;
        local_130[0xf] = 0;
        local_130[0xe] = 0;
        local_130[0xd] = 0;
        local_130[0xc] = 0;
        local_130[0xb] = 0;
        local_130[10] = 0;
        local_130[9] = 0;
        local_130[8] = 0;
        local_130[7] = 0;
        local_130[6] = 0;
        local_130[5] = 0;
        local_130[4] = 0;
        local_130[3] = 0;
        local_130[2] = 0;
        local_130[1] = 0;
        uVar6 = FUN_0017c42c(local_130,uVar5,0);
        if ((int)uVar6 == 0) goto LAB_0017d9ac;
        uVar6 = *param_1;
        param_1[1] = uVar5;
        uVar7 = FUN_0017b858(param_3);
        FUN_0016cec0(uVar6,uVar7);
        FUN_0016cf5c(0xc61c3c00,0xc61c3c00,*param_1);
        FUN_0016cfa4(DAT_00281b10,*param_1);
        uVar6 = FUN_0017df44(*param_1);
        if ((int)uVar6 == 0) goto LAB_0017d9ac;
        local_134 = CONCAT31(local_134._1_3_,0xff);
        if (0x1000 < *param_1 + 0x49) {
          iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,*param_1 + 0x48,&local_134,1);
          uVar6 = (ulong)(iVar4 == 1 && (char)local_134 == '\x01');
          goto LAB_0017d9ac;
        }
      }
    }
  }
  uVar6 = 0;
LAB_0017d9ac:
  if (*(long *)(lVar1 + 0x28) == local_130[0x1b]) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar6);
}

/* ===== FUN_0017f2f8 @ 0017f2f8 ===== */

void FUN_0017f2f8(ulong *param_1,undefined4 param_2,undefined8 param_3)

{
  long lVar1;
  bool bVar2;
  bool bVar3;
  int iVar4;
  ulong uVar5;
  ulong uVar6;
  undefined8 uVar7;
  int local_134;
  ulong local_130 [28];
  
  lVar1 = tpidr_el0;
  local_130[0x1b] = *(long *)(lVar1 + 0x28);
  nexus_rich_asset(param_2);
  uVar5 = FUN_0016ce48();
  uVar6 = FUN_0016dc20(uVar5,param_2,0);
  if ((int)uVar6 == 0) goto LAB_0017f3e0;
  local_134 = 0;
  local_130[0] = 1;
  if (((((0xfff < uVar5 + 0x38) && ((uVar5 & 0xfffffffffffffff8) != 0xffffffffffffffc0)) &&
       (iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar5 + 0x38,local_130,8), iVar4 == 1)) &&
      ((local_130[0] == 0 && (0xfff < uVar5 + 0x40)))) &&
     (((uVar5 & 0xfffffffffffffffc) != 0xffffffffffffffbc &&
      ((iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar5 + 0x40,&local_134,4), iVar4 == 1 &&
       (local_134 == -1)))))) {
    FUN_0016ce84(uVar5,0);
    uVar6 = FUN_0016cdf0(0x260);
    *param_1 = uVar6;
    if (uVar6 == 0) goto LAB_0017f3e0;
    FUN_0016ce34();
    local_130[0] = 0;
    if (*param_1 + 8 >> 3 < 0x201) {
      bVar3 = false;
    }
    else {
      iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,*param_1,local_130,8);
      bVar3 = iVar4 == 1;
    }
    bVar2 = false;
    if (0xfff < local_130[0]) {
      bVar2 = bVar3;
    }
    uVar6 = local_130[0];
    if (!(bool)(bVar2 & (local_130[0] & 7) == 0)) {
      uVar6 = 0;
    }
    if (uVar6 == DAT_001a7cd0 + 0x11c0a48U) {
      uVar6 = *param_1;
      local_134 = 0;
      local_130[0] = 1;
      if ((((0xfff < uVar6 + 0x38) && ((uVar6 & 0xfffffffffffffff8) != 0xffffffffffffffc0)) &&
          (iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar6 + 0x38,local_130,8), iVar4 == 1)) &&
         (((local_130[0] == 0 && (0xfff < uVar6 + 0x40)) &&
          (((uVar6 & 0xfffffffffffffffc) != 0xffffffffffffffbc &&
           ((iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,uVar6 + 0x40,&local_134,4), iVar4 == 1 &&
            (local_134 == -1)))))))) {
        uVar6 = FUN_0014e5ac(*param_1);
        if ((int)uVar6 == 0) goto LAB_0017f3e0;
        local_130[0x1a] = 0;
        local_130[0x19] = 0;
        local_130[0x18] = 0;
        local_130[0x17] = 0;
        local_130[0x16] = 0;
        local_130[0x15] = 0;
        local_130[0x14] = 0;
        local_130[0x13] = 0;
        local_130[0x12] = 0;
        local_130[0x11] = 0;
        local_130[0] = *param_1;
        local_130[0x10] = 0;
        local_130[0xf] = 0;
        local_130[0xe] = 0;
        local_130[0xd] = 0;
        local_130[0xc] = 0;
        local_130[0xb] = 0;
        local_130[10] = 0;
        local_130[9] = 0;
        local_130[8] = 0;
        local_130[7] = 0;
        local_130[6] = 0;
        local_130[5] = 0;
        local_130[4] = 0;
        local_130[3] = 0;
        local_130[2] = 0;
        local_130[1] = 0;
        uVar6 = FUN_0017c42c(local_130,uVar5,0);
        if ((int)uVar6 == 0) goto LAB_0017f3e0;
        uVar6 = *param_1;
        param_1[1] = uVar5;
        uVar7 = FUN_0017b858(param_3);
        FUN_0016cec0(uVar6,uVar7);
        FUN_0016cf5c(0xc61c3c00,0xc61c3c00,*param_1);
        FUN_0016cfa4(DAT_00282158,*param_1);
        uVar6 = FUN_00179dfc(*param_1);
        if ((int)uVar6 == 0) goto LAB_0017f3e0;
        local_134 = CONCAT31(local_134._1_3_,0xff);
        if (0x1000 < *param_1 + 0x49) {
          iVar4 = (*DAT_001a7ce8)(DAT_001a7cc8,*param_1 + 0x48,&local_134,1);
          uVar6 = (ulong)(iVar4 == 1 && (char)local_134 == '\x01');
          goto LAB_0017f3e0;
        }
      }
    }
  }
  uVar6 = 0;
LAB_0017f3e0:
  if (*(long *)(lVar1 + 0x28) == local_130[0x1b]) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar6);
}

/* ===== nexus_menu_set_value @ 001849cc ===== */

void nexus_menu_set_value(uint param_1,uint param_2,int param_3)

{
  byte bVar1;
  long lVar2;
  undefined *puVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  ulong uVar7;
  long lVar8;
  undefined *puVar9;
  undefined8 *puVar10;
  ulong uVar11;
  undefined1 auStack_260 [16];
  undefined4 local_250;
  char local_24c;
  undefined4 local_248;
  uint uStack_244;
  uint local_240;
  uint uStack_23c;
  undefined8 local_238;
  undefined8 local_230;
  undefined8 uStack_228;
  undefined8 local_220;
  undefined8 uStack_218;
  undefined4 local_210;
  long local_70;
  
  lVar2 = tpidr_el0;
  local_70 = *(long *)(lVar2 + 0x28);
  if (0xa7 < param_1) {
    uVar7 = 4;
    goto LAB_00184bf4;
  }
  uVar7 = FUN_00193f80(1,&DAT_00284688);
  if ((uVar7 & 1) != 0) {
    uVar7 = 3;
    goto LAB_00184bf4;
  }
  uVar7 = 4;
  if ((0 < param_3) && (DAT_00284694 == param_3)) {
    DAT_002846a4 = param_1;
    iVar4 = FUN_0018d0fc(param_1);
    if ((iVar4 == 0) &&
       ((iVar4 = pthread_once((pthread_once_t *)&DAT_0028dcfc,FUN_0018d500),
        DAT_0028dd00 == (code *)0x0 || (iVar4 = (*DAT_0028dd00)(iVar4), iVar4 != 1)))) {
LAB_00184bcc:
      uVar7 = 0;
    }
    else {
      FUN_00185d10();
      puVar3 = PTR_DAT_001a36f0;
      puVar9 = PTR_DAT_001a36f0 + (ulong)param_1 * 0x48;
      if (puVar9[0x47] != '\0') goto LAB_00184bcc;
      uVar5 = FUN_0018d15c(puVar9);
      if ((((int)uVar5 < 0) ||
          (puVar10 = (undefined8 *)(PTR_PTR_s_nexus_sx_spin_001a36f8 + (ulong)uVar5 * 0x18),
          (int)param_2 < *(int *)(puVar10 + 1))) ||
         (*(int *)(PTR_PTR_s_nexus_sx_spin_001a36f8 + (ulong)uVar5 * 0x18 + 0xc) < (int)param_2)) {
LAB_00184bd4:
        uVar7 = 4;
      }
      else {
        if (param_1 == 0x70) {
          if (param_2 < 7) {
            memcpy(&local_248,&DAT_002846b0,0x1d8);
            iVar4 = FUN_0018d2a0(&local_248,"nexus_sx_outline_color_preset",param_2);
            if ((iVar4 != 0) &&
               (iVar4 = FUN_0018d2a0(&local_248,"nexus_sx_outline_r",
                                     *(undefined4 *)(&DAT_0014073c + (ulong)param_2 * 0xc)),
               iVar4 != 0)) {
              iVar4 = FUN_0018d2a0(&local_248,"nexus_sx_outline_g",
                                   *(undefined4 *)(&DAT_00140740 + (ulong)param_2 * 0xc));
              if ((iVar4 != 0) &&
                 (iVar4 = FUN_0018d2a0(&local_248,"nexus_sx_outline_b",
                                       *(undefined4 *)(&DAT_00140744 + (ulong)param_2 * 0xc)),
                 iVar4 != 0)) {
                uVar7 = FUN_001846b8(&local_248);
                if (0xfffffffd < (uint)uVar7 - 3) {
                  uVar6 = FUN_0018d334();
                  uVar5 = (uint)uVar7;
                  if (uVar6 != 1) {
                    uVar5 = uVar6;
                  }
                  uVar7 = (ulong)uVar5;
                }
                goto LAB_00184bd8;
              }
            }
          }
          goto LAB_00184bd4;
        }
        uVar11 = (ulong)param_1;
        bVar1 = puVar3[uVar11 * 0x48 + 0x44];
        uVar6 = (uint)bVar1;
        if ((bVar1 < 8) && ((1 << (ulong)(uVar6 & 0x1f) & 0xd8U) != 0)) {
          if ((uVar6 < 5) &&
             ((0x3d < param_1 - 0x2f ||
              ((1L << ((ulong)(param_1 - 0x2f) & 0x3f) & 0x2000000003c00001U) == 0)))) {
LAB_00184cbc:
            lVar8 = FUN_0018d3b4(param_1);
            FUN_00184e28(auStack_260,puVar9);
            DAT_002846ac = local_250;
            if ((lVar8 == 0) || (local_24c == '\0')) goto LAB_00184bcc;
            local_238 = *puVar10;
            local_220 = 0;
            uStack_218 = 0;
            local_248 = 0x40;
            uStack_228 = *(undefined8 *)(puVar3 + uVar11 * 0x48 + 0x30);
            local_230 = *(undefined8 *)(puVar3 + uVar11 * 0x48 + 0x28);
            uStack_244 = param_1;
            local_240 = uVar6;
            uStack_23c = param_2;
            local_210 = FUN_0018d48c();
            uVar6 = (**(code **)(lVar8 + 0x30))(*(undefined8 *)(lVar8 + 8),&local_248);
            uVar11 = (ulong)uVar6;
            if (uVar6 - 1 < 2) goto LAB_00184c84;
          }
          else {
            uVar11 = 1;
LAB_00184c84:
            *(uint *)((long)&DAT_002846b0 + (ulong)uVar5 * 4) = param_2;
            uVar7 = FUN_0018d334();
            if ((int)uVar7 != 1) goto LAB_00184bd8;
          }
          uVar7 = uVar11;
        }
        else {
          uVar7 = 4;
          if ((param_1 == 0x79) && ((param_2 < 3 && (bVar1 == 2)))) goto LAB_00184cbc;
        }
      }
    }
LAB_00184bd8:
    DAT_002846a8 = (undefined4)uVar7;
    DAT_002846a0 = DAT_002846a0 + 1;
  }
  DAT_00284688 = 0;
LAB_00184bf4:
  if (*(long *)(lVar2 + 0x28) != local_70) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar7);
  }
  return;
}

/* ===== FUN_00186c90 @ 00186c90 ===== */

void FUN_00186c90(int param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 *puVar1;
  
  if (((DAT_00287ba4 == 0) && (DAT_00287b98 != 0)) && (DAT_00285410 != 0)) {
    DAT_00287ba4 = 2;
    puVar1 = &DAT_00287c40;
    if (param_1 != 1) {
      puVar1 = &DAT_00285414;
    }
    DAT_00287c44 = *puVar1;
    DAT_00287c50 = DAT_00287c48;
    DAT_00287ba8 = param_1;
    DAT_00287bb8 = param_2;
    DAT_00287bbc = param_3;
    FUN_00183ac8(&DAT_00287c6c,0x60,0x60,&DAT_001343dc,"APPLYING ON NEXT FRAME");
    DAT_00287bb4 = 0;
    DAT_0028469f = 0;
    DAT_00287bac = DAT_00287bac + 1;
    DAT_002846a0 = DAT_002846a0 + 1;
  }
  return;
}

/* ===== FUN_00190ad0 @ 00190ad0 ===== */

void FUN_00190ad0(long *param_1,undefined8 *param_2,long param_3,undefined8 param_4,
                 undefined8 param_5,undefined8 param_6)

{
  ulong uVar1;
  long lVar2;
  bool bVar3;
  bool bVar4;
  int iVar5;
  ulong uVar6;
  undefined8 uVar7;
  char *pcVar8;
  short local_64 [2];
  ulong local_60;
  long local_58;
  
  lVar2 = tpidr_el0;
  local_58 = *(long *)(lVar2 + 0x28);
  uVar6 = (*(code *)param_2[5])(*param_2,param_4);
  local_64[0] = 0;
  param_1[0x10] = uVar6;
  param_1[0x11] = 0;
  *(undefined4 *)(param_1 + 0x12) = 0xffffffff;
  if ((uVar6 < 0x1000) || ((uVar6 & 7) != 0)) {
    pcVar8 = "movie_result_null_or_unaligned";
  }
  else {
    local_60 = 0;
    if (uVar6 + 8 >> 3 < 0x201) {
      bVar4 = false;
    }
    else {
      iVar5 = (*(code *)param_2[1])(*param_2,uVar6,&local_60,8);
      bVar4 = iVar5 != 0;
    }
    bVar3 = false;
    if (0xfff < local_60) {
      bVar3 = bVar4;
    }
    uVar1 = local_60;
    if (!(bool)(bVar3 & (local_60 & 7) == 0)) {
      uVar1 = 0;
    }
    param_1[0x11] = uVar1;
    if (uVar1 == *param_1 + 0x11ad208U) {
      if (((uVar6 + 0xbe < 0x1000) || ((uVar6 & 0xfffffffffffffffe) == 0xffffffffffffff40)) ||
         (iVar5 = (*(code *)param_2[1])(*param_2,uVar6 + 0xbe,local_64,2), iVar5 == 0)) {
        pcVar8 = "movie_be_read_failed";
      }
      else {
        *(int *)(param_1 + 0x12) = (int)local_64[0];
        (*(code *)param_2[0xc])(*param_2,"movie_observed",param_4,param_1);
        if (local_64[0] < 1) {
          pcVar8 = "movie_frame_guard";
        }
        else {
          (*(code *)param_2[6])(*param_2,param_3,uVar6);
          local_60 = 0;
          if (((0x200 < param_3 + 0x88U >> 3) &&
              (iVar5 = (*(code *)param_2[1])(*param_2,param_3 + 0x80,&local_60,8), iVar5 != 0)) &&
             ((0xfff < local_60 && ((local_60 & 7) == 0)))) {
            (*(code *)param_2[7])(*param_2,uVar6,0);
            (*(code *)param_2[8])(*param_2,param_5,param_6);
            (*(code *)param_2[9])(*param_2,param_3,param_5);
            uVar7 = 1;
            goto LAB_00190d08;
          }
          pcVar8 = "bound_clip_missing";
        }
      }
    }
    else {
      pcVar8 = "movie_vptr_mismatch";
    }
  }
  *(undefined4 *)(param_1 + 8) = 3;
  (*(code *)param_2[0xc])(*param_2,"stopped",pcVar8,param_1);
  uVar7 = 0;
LAB_00190d08:
  if (*(long *)(lVar2 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar7);
}

/* ===== nexus_script_port_ui_reload_request @ 00194150 ===== */

void nexus_script_port_ui_reload_request(void)

{
  (*(code *)PTR_nexus_script_port_ui_reload_request_001a37b0)();
  return;
}

/* ===== nexus_rich_main_frame @ 00194640 ===== */

void nexus_rich_main_frame(void)

{
  (*(code *)PTR_nexus_rich_main_frame_001a3a28)();
  return;
}

/* ===== nexus_script_port_fps_open @ 00194690 ===== */

void nexus_script_port_fps_open(void)

{
  (*(code *)PTR_nexus_script_port_fps_open_001a3a50)();
  return;
}

/* ===== nexus_ui_graphics_resources @ 00194780 ===== */

void nexus_ui_graphics_resources(void)

{
  (*(code *)PTR_nexus_ui_graphics_resources_001a3ac8)();
  return;
}

/* ===== nexus_rich_renderer_ops_v1 @ 00194790 ===== */

void nexus_rich_renderer_ops_v1(void)

{
  (*(code *)PTR_nexus_rich_renderer_ops_v1_001a3ad0)();
  return;
}

/* ===== nexus_rich_validate_native @ 001947a0 ===== */

void nexus_rich_validate_native(void)

{
  (*(code *)PTR_nexus_rich_validate_native_001a3ad8)();
  return;
}

/* ===== nexus_rich_launcher_geometry @ 001947b0 ===== */

void nexus_rich_launcher_geometry(void)

{
  (*(code *)PTR_nexus_rich_launcher_geometry_001a3ae0)();
  return;
}

/* ===== nexus_rich_main_row @ 001947f0 ===== */

void nexus_rich_main_row(void)

{
  (*(code *)PTR_nexus_rich_main_row_001a3b00)();
  return;
}

/* ===== nexus_rich_asset @ 00194870 ===== */

void nexus_rich_asset(void)

{
  (*(code *)PTR_nexus_rich_asset_001a3b40)();
  return;
}

/* ===== nexus_rich_color @ 00194880 ===== */

void nexus_rich_color(void)

{
  (*(code *)PTR_nexus_rich_color_001a3b48)();
  return;
}

/* ===== nexus_rich_plan_stage @ 001948c0 ===== */

void nexus_rich_plan_stage(void)

{
  (*(code *)PTR_nexus_rich_plan_stage_001a3b68)();
  return;
}

