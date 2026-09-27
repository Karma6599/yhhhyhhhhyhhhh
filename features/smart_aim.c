/*
 * Smart Aim (AOP) — Feature
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
 * Notes: Input injection path (smartaim_input), target mode, ultimate/gadget aim sub-modes; separate "natural_smartaim_only" profile when paid scope off.
 */

/* ===== FUN_001512f8 @ 001512f8 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_001512f8(undefined8 param_1,int *param_2)

{
  char *pcVar1;
  undefined4 uVar2;
  int iVar3;
  char cVar4;
  bool bVar5;
  long lVar6;
  undefined4 uVar7;
  char cVar8;
  int iVar9;
  int iVar10;
  undefined8 uVar11;
  long lVar12;
  char *pcVar13;
  uint uVar14;
  ushort uVar15;
  undefined1 auVar16 [16];
  undefined1 auVar17 [16];
  undefined8 **local_a88;
  code *pcStack_a80;
  code *local_a78;
  code *local_a70;
  undefined8 *puStack_a68;
  undefined8 local_a60;
  undefined8 uStack_a58;
  undefined **local_a50;
  code *pcStack_a48;
  code *local_a40;
  undefined8 uStack_a38;
  code *local_a30;
  undefined *local_a28;
  undefined8 *puStack_a20;
  code *local_a18;
  undefined8 uStack_a10;
  undefined8 *local_a08;
  long lStack_a00;
  undefined4 local_9f8;
  undefined4 local_9f4;
  undefined8 local_9f0;
  undefined4 local_9e8;
  undefined8 local_9e4;
  undefined8 local_9dc;
  undefined4 local_9d4;
  undefined8 local_9d0;
  int local_9c8;
  undefined4 local_9c4;
  undefined8 local_9c0;
  undefined8 uStack_9b8;
  undefined8 local_9b0;
  undefined8 uStack_9a8;
  undefined8 local_9a0;
  undefined4 local_998;
  undefined8 local_994;
  undefined8 local_988;
  undefined8 local_980;
  undefined4 local_978;
  undefined4 local_974;
  undefined8 local_970;
  undefined4 local_968;
  undefined8 local_964;
  undefined8 local_958;
  undefined8 local_950;
  long lStack_948;
  undefined8 local_940;
  undefined8 uStack_938;
  undefined8 uStack_930;
  undefined8 uStack_928;
  undefined8 local_920;
  undefined8 uStack_918;
  undefined8 uStack_910;
  undefined8 uStack_908;
  undefined8 local_900;
  undefined8 local_8f8;
  undefined8 uStack_8f0;
  undefined8 local_8e8;
  undefined8 uStack_8e0;
  undefined8 local_8d8;
  undefined8 uStack_8d0;
  undefined8 local_8c8;
  undefined8 local_8c0;
  undefined8 uStack_8b8;
  undefined8 local_8b0;
  long lStack_8a8;
  undefined8 uStack_8a0;
  long local_898;
  undefined8 local_890;
  undefined8 uStack_888;
  undefined8 uStack_880;
  undefined8 uStack_878;
  undefined4 local_870;
  undefined4 uStack_86c;
  undefined4 uStack_868;
  undefined4 uStack_864;
  undefined8 uStack_860;
  long local_858;
  undefined8 local_850;
  undefined8 uStack_848;
  undefined8 uStack_840;
  undefined8 uStack_838;
  undefined8 local_830;
  undefined8 uStack_828;
  undefined8 uStack_820;
  undefined8 uStack_818;
  undefined8 local_810;
  undefined8 uStack_808;
  undefined8 uStack_800;
  undefined8 uStack_7f8;
  int local_7ec [425];
  undefined8 local_148;
  undefined8 local_140;
  long local_138;
  undefined8 uStack_130;
  undefined8 local_128;
  undefined8 uStack_120;
  ulong local_118;
  undefined8 uStack_110;
  undefined8 local_108;
  undefined8 uStack_100;
  undefined8 local_f8;
  undefined8 uStack_f0;
  undefined8 local_e8;
  undefined8 uStack_e0;
  undefined8 local_d8;
  undefined8 uStack_d0;
  undefined8 local_c8;
  undefined8 uStack_c0;
  undefined8 local_b8;
  undefined8 uStack_b0;
  undefined8 local_a8;
  undefined8 uStack_a0;
  undefined8 local_98;
  undefined8 uStack_90;
  undefined8 local_88;
  undefined8 local_80;
  long local_78;
  
  iVar10 = DAT_00209cd8;
  lVar6 = tpidr_el0;
  local_78 = *(long *)(lVar6 + 0x28);
  if (((((param_2 != (int *)0x0) && (DAT_002147dc != '\0')) && (DAT_00209cd8 != 0)) &&
      ((iVar9 = gettid(), iVar10 == iVar9 && (DAT_0020d158 < 4)))) && (DAT_0020d158 != 2)) {
    if ((((*param_2 == 2) && (param_2[1] == 0x98)) &&
        ((param_2[2] == 2 &&
         (((*(long *)(param_2 + 4) == DAT_00209d00 && (*(long *)(param_2 + 8) == DAT_001e0978)) &&
          (*(long *)(param_2 + 10) == *(long *)(param_2 + 8) + 0xb2e994)))))) &&
       ((*(long *)(param_2 + 0x14) == *(long *)(param_2 + 0xc) &&
        (*(long *)(param_2 + 0x16) == *(long *)(param_2 + 0xe))))) {
      iVar10 = FUN_0015494c();
    }
    else {
      iVar10 = 0;
    }
    if (DAT_0020d158 == 0) {
      if (iVar10 != 0) {
        if (*(long *)(lVar6 + 0x28) == local_78) {
          FUN_00153edc(param_2);
          return;
        }
        goto LAB_00151b7c;
      }
      do {
        cVar8 = DAT_001cfe94;
        cVar4 = '\x01';
        bVar5 = (bool)ExclusiveMonitorPass(0x1cfe94,0x10);
        if (bVar5) {
          _DAT_001cfe94 = CONCAT31(DAT_001cfe94_1,1);
          cVar4 = ExclusiveMonitorsStatus();
        }
      } while (cVar4 != '\0');
      if (cVar8 != '\0') goto LAB_001513fc;
      DAT_002147e0 = DAT_002147e0 + 1;
      FUN_00153ff8(param_2);
      local_8c8 = 0;
      local_88 = 0;
      local_80 = 0;
      lStack_8a8 = 0;
      local_8b0 = 0;
      local_898 = 0;
      uStack_8a0 = 0;
      local_900 = DAT_0010e7f8;
      uStack_888 = 0;
      local_890 = 0;
      uStack_878 = 0;
      uStack_880 = 0;
      uStack_868 = 0;
      uStack_864 = 0;
      local_870 = 0;
      uStack_86c = 0;
      local_858 = 0;
      uStack_860 = 0;
      uStack_848 = 0;
      local_850 = 0;
      uStack_838 = 0;
      uStack_840 = 0;
      uStack_828 = 0;
      local_830 = 0;
      uStack_818 = 0;
      uStack_820 = 0;
      uStack_808 = 0;
      local_810 = 0;
      uStack_7f8 = 0;
      uStack_800 = 0;
      uStack_8b8 = 0;
      local_8c0 = 0;
      uStack_8d0 = 0;
      local_8d8 = 0;
      uStack_8e0 = 0;
      local_8e8 = 0;
      uStack_8f0 = 0;
      local_8f8 = 0;
      uStack_90 = 0;
      local_98 = 0;
      uStack_a0 = 0;
      local_a8 = 0;
      uStack_b0 = 0;
      local_b8 = 0;
      uStack_c0 = 0;
      local_c8 = 0;
      uStack_d0 = 0;
      local_d8 = 0;
      uStack_e0 = 0;
      local_e8 = 0;
      uStack_f0 = 0;
      local_f8 = 0;
      uStack_100 = 0;
      local_108 = 0;
      uStack_110 = 0;
      local_118 = 0;
      uStack_120 = 0;
      local_128 = 0;
      uStack_130 = 0;
      local_138 = 0;
      local_140 = DAT_0010e7d0;
      if (((DAT_002147d8 == '\x01') && (*(long *)(param_2 + 4) == DAT_00209d00)) &&
         ((*(long *)(param_2 + 8) == DAT_001e0978 &&
          (uVar11 = FUN_00150bf0(*(undefined8 *)(param_2 + 0xc),*(undefined8 *)(param_2 + 0xe)),
          (int)uVar11 != 0)))) {
        iVar10 = FUN_001428fc(uVar11,*(long *)(param_2 + 0xc) + 0xfac,&local_88,8);
        if (iVar10 != 0) {
          local_80 = local_88;
        }
        uVar14 = (uint)(iVar10 != 0);
        iVar9 = FUN_00154184(param_2);
        if (iVar9 == 0) {
          iVar9 = (*DAT_0020d160)(&local_140);
          pcVar13 = "configuration_or_generation";
          if ((iVar9 == 1) && ((int)uStack_110 != 0)) {
            if ((local_118 & 1) == 0) {
              iVar10 = 0;
              iVar9 = 0;
              pcVar13 = "configuration_or_generation";
            }
            else if (local_138 == DAT_0020f5e8) {
              if ((uStack_f0._4_4_ == 0) || ((int)local_e8 == 0)) {
                local_7ec[0] = 0;
                iVar9 = FUN_001541f8(*(undefined8 *)(param_2 + 0xc),*(undefined8 *)(param_2 + 0xe),0
                                     ,local_7ec);
                if (iVar9 == 0) {
                  pcVar13 = "ability_input_unknown";
                }
                else {
                  pcVar1 = "gadget_smartaim_disabled";
                  if (local_7ec[0] != 3) {
                    pcVar1 = "ability_allowed";
                  }
                  pcVar13 = "ultimate_smartaim_disabled";
                  if (local_7ec[0] != 2) {
                    pcVar13 = pcVar1;
                  }
                  if (local_7ec[0] == 3) {
                    iVar9 = (int)local_e8;
                  }
                  else {
                    if (local_7ec[0] != 2) goto LAB_00151814;
                    iVar9 = uStack_f0._4_4_;
                  }
                  if (iVar9 != 0) goto LAB_00151814;
                }
                iVar10 = 0;
                iVar9 = 0;
              }
              else {
LAB_00151814:
                iVar9 = FUN_00189c88(DAT_001e0978,*(undefined8 *)(param_2 + 0xe),FUN_001428fc,0,
                                     &DAT_0020f680);
                if ((iVar9 == 0) ||
                   (iVar9 = FUN_001543f8(DAT_0020f678,*(undefined8 *)(param_2 + 0xe)), iVar9 == 0))
                {
                  iVar10 = 0;
                  iVar9 = 0;
                  pcVar13 = "current_actor_refresh";
                }
                else {
                  uStack_938 = 0;
                  local_940 = 0;
                  uStack_928 = 0;
                  uStack_930 = 0;
                  uStack_918 = 0;
                  local_920 = 0;
                  uStack_908 = 0;
                  uStack_910 = 0;
                  lStack_948 = 0;
                  local_950 = 0;
                  local_958 = DAT_0010e700;
                  iVar9 = (*DAT_00213040)(0x17,1,&local_958);
                  if (iVar9 == 0) {
                    iVar10 = 0;
                    iVar9 = 0;
                    pcVar13 = "input_lease_unavailable";
                  }
                  else if (iVar10 == 0) {
                    iVar10 = 0;
                    iVar9 = 0;
                    pcVar13 = "untouched_xy_unknown";
                  }
                  else {
                    local_9c0 = *(undefined8 *)(param_2 + 0xc);
                    local_9b0 = *(undefined8 *)(param_2 + 0xe);
                    local_9c4 = 1;
                    local_9d0 = DAT_0010e6e0;
                    local_994 = CONCAT44(DAT_0020f638._4_4_,(undefined4)DAT_0020f638);
                    uStack_9b8 = DAT_0020f678;
                    local_9a0 = *(undefined8 *)(param_2 + 6);
                    local_9c8 = param_2[2];
                    uStack_9a8 = DAT_0020f6f8;
                    local_998 = DAT_0020f690;
                    local_988 = FUN_001391f0(1);
                    local_964 = local_88;
                    local_974 = DAT_0020f694;
                    local_9e8 = local_108._4_4_;
                    local_9f4 = uStack_110._4_4_;
                    local_978 = 1;
                    local_a28 = &DAT_0020d168;
                    puStack_a20 = &DAT_0020f628;
                    local_a08 = &local_900;
                    local_a18 = FUN_00150f7c;
                    uStack_a10 = 0;
                    local_a50 = &local_a28;
                    local_970 = 0;
                    lStack_a00 = local_138;
                    local_968 = 1;
                    local_980 = DAT_0010e588;
                    local_9f8 = 1;
                    pcStack_a48 = thunk_FUN_00186cc8;
                    local_9f0 = 0;
                    local_9dc = 0;
                    local_9e4 = 0;
                    local_a40 = FUN_001546d4;
                    uStack_a38 = 0;
                    local_9d4 = 0;
                    local_a30 = FUN_00154808;
                    iVar10 = FUN_00184ce0(&DAT_00213098,&lStack_a00,&local_9d0,&local_a50,&local_8c0
                                         );
                    pcVar1 = "proposal_valid";
                    pcVar13 = "proposal_refused";
                    if ((((iVar10 == 1) && (pcVar13 = pcVar1, uStack_8b8._4_4_ == 1)) &&
                        (lStack_8a8 == *(long *)(param_2 + 0xc))) &&
                       (local_898 == *(long *)(param_2 + 0xe))) {
                      auVar16._8_8_ = CONCAT44(uStack_868,uStack_86c);
                      auVar16._0_8_ = CONCAT44(uStack_868,uStack_86c);
                      auVar17 = NEON_cmgt(auVar16,_DAT_00112be0,4);
                      auVar16 = NEON_cmgt(_DAT_00112be0,auVar16,4);
                      uVar15 = NEON_umaxv(CONCAT26(auVar16._12_2_,
                                                   CONCAT24(auVar16._8_2_,
                                                            CONCAT22(auVar17._4_2_,auVar17._0_2_))),
                                          2);
                      if ((((uVar15 & 1) == 0) && (local_858 == lStack_948)) &&
                         (iVar10 = FUN_00150bf0(), iVar10 != 0)) {
                        iVar10 = (*DAT_00213048)(&local_958);
                        if (iVar10 != 0) {
                          uStack_a58 = *(undefined8 *)(param_2 + 0xe);
                          local_a60 = *(undefined8 *)(param_2 + 0xc);
                          local_148 = CONCAT44(uStack_868,uStack_86c);
                          local_a88 = &puStack_a68;
                          pcStack_a80 = FUN_001428fc;
                          local_a78 = FUN_001548cc;
                          local_a70 = FUN_00154908;
                          puStack_a68 = &local_958;
                          iVar9 = FUN_0018a084(&local_a88,local_a60,&local_80,&local_148);
                          if (iVar9 == 1) {
                            iVar10 = 1;
                            DAT_002147e8 = DAT_002147e8 + 1;
                            pcVar13 = "natural_event_xy_committed";
                          }
                          else {
                            if (iVar9 == -1) {
                              FUN_001417c8("fatal","input_xy_restore_unverified",0);
                              __android_log_write(6,"NexusLab69252","input_xy_restore_unverified");
                    /* WARNING: Subroutine does not return */
                              abort();
                            }
                            pcVar1 = "untouched_xy_changed";
                            if (iVar9 != 3) {
                              pcVar1 = "xy_write_refused";
                            }
                            iVar10 = 0;
                            pcVar13 = "xy_restored_after_failed_write";
                            if (iVar9 != 2) {
                              pcVar13 = pcVar1;
                            }
                          }
                          goto LAB_0015159c;
                        }
                      }
                    }
                    iVar10 = 0;
                    iVar9 = 0;
                  }
                }
              }
            }
            else {
              iVar10 = 0;
              iVar9 = 0;
            }
          }
          else {
            iVar10 = 0;
            iVar9 = 0;
          }
          goto LAB_0015159c;
        }
      }
      else {
        iVar10 = 0;
        uVar14 = 0;
        iVar9 = 0;
        pcVar13 = "identity_or_active";
LAB_0015159c:
        lVar12 = FUN_001391f0(1);
        if (0x20 < DAT_002147e0) {
          iVar3 = 0;
          if (DAT_002147e8 < 9) {
            iVar3 = iVar10;
          }
          if (((iVar3 == 0) && (DAT_002147f0 != 0)) && ((ulong)(lVar12 - DAT_002147f0) < 1000))
          goto LAB_00151700;
        }
        auVar17._0_8_ = (double)DAT_0020f6b4;
        auVar17._8_8_ = 0;
        uVar2 = uStack_868;
        uVar7 = uStack_86c;
        if (iVar10 == 0) {
          uVar2 = local_80._4_4_;
          uVar7 = (undefined4)local_80;
        }
        snprintf((char *)local_7ec,0x6a4,
                 ",\"action\":23,\"kind\":%u,\"delivery\":%llu,\"event_count\":%llu,\"publication\":%llu,\"source_tick\":%u,\"clock_known\":%u,\"map_ready\":%u,\"config_supported\":%u,\"active\":%u,\"evidence\":%u,\"generation\":%llu,\"plan_reason\":%u,\"selection_reason\":%u,\"target_gid\":%u,\"untouched_xy_known\":%d,\"untouched_raw_x\":%d,\"untouched_raw_y\":%d,\"proposed_raw_x\":%d,\"proposed_raw_y\":%d,\"committed_xy_known\":%d,\"committed_raw_x\":%d,\"committed_raw_y\":%d,\"commit_status\":%d,\"xy_written\":%d,\"xy_writes\":%llu,\"own_gid\":%u,\"own_x\":%.6f,\"own_y\":%.6f,\"ended\":%u,\"extra_fire_calls\":0"
                 ,auVar17,SUB82((double)DAT_0020f6b8,0),(ulong)(uint)param_2[2],
                 *(undefined8 *)(param_2 + 6),DAT_002147e0,DAT_0020f650,(ulong)DAT_0020f638._4_4_,
                 (undefined4)DAT_0020f638,(uint)DAT_0020d15c,(uint)local_118 & 1,DAT_0020f7c4,
                 DAT_00214800,local_138,(undefined4)uStack_8b8,(undefined4)local_8f8,local_870,
                 uVar14,(undefined4)local_80,local_80._4_4_,uStack_86c,uStack_868,
                 (uint)(iVar9 - 1U < 2),uVar7,uVar2,iVar9,iVar10,DAT_002147e8,DAT_0020f690,
                 DAT_0020f7e8);
        FUN_001417c8("smartaim_input",pcVar13,local_7ec);
        DAT_002147f0 = lVar12;
      }
LAB_00151700:
      _DAT_001cfe94 = 0;
      FUN_00153edc(param_2);
    }
  }
LAB_001513fc:
  if (*(long *)(lVar6 + 0x28) == local_78) {
    return;
  }
LAB_00151b7c:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00151ec4 @ 00151ec4 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00151ec4(undefined8 param_1,int *param_2,undefined8 *param_3)

{
  char *pcVar1;
  char cVar2;
  long lVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  char cVar6;
  bool bVar7;
  bool bVar8;
  int iVar9;
  undefined8 uVar10;
  char *pcVar11;
  long lVar12;
  ushort uVar13;
  undefined1 auVar14 [16];
  undefined8 uVar15;
  undefined1 auVar16 [16];
  undefined **local_3d0;
  code *pcStack_3c8;
  code *local_3c0;
  undefined8 uStack_3b8;
  code *local_3b0;
  undefined *local_3a8;
  undefined8 *puStack_3a0;
  code *local_398;
  undefined8 uStack_390;
  undefined8 *local_388;
  long lStack_380;
  undefined4 local_378;
  undefined4 uStack_374;
  undefined8 local_370;
  undefined4 local_368;
  undefined8 local_364;
  undefined8 local_35c;
  undefined4 local_354;
  undefined8 local_350;
  undefined8 uStack_348;
  undefined8 local_340;
  undefined8 uStack_338;
  undefined8 local_330;
  undefined8 uStack_328;
  undefined8 local_320;
  undefined4 local_318;
  undefined8 local_314;
  undefined8 local_308;
  undefined8 local_300;
  undefined4 local_2f8;
  undefined4 uStack_2f4;
  undefined8 local_2f0;
  undefined4 local_2e8;
  undefined8 local_2e4;
  int local_2cc;
  undefined8 local_2c8;
  undefined8 uStack_2c0;
  long lStack_2b8;
  undefined8 uStack_2b0;
  undefined8 uStack_2a8;
  undefined8 uStack_2a0;
  undefined8 uStack_298;
  undefined8 local_290;
  undefined8 uStack_288;
  undefined8 uStack_280;
  undefined8 local_278;
  undefined8 local_270;
  undefined8 uStack_268;
  undefined8 local_260;
  undefined8 uStack_258;
  undefined8 uStack_250;
  undefined8 uStack_248;
  undefined8 local_240;
  undefined8 uStack_238;
  undefined8 uStack_230;
  undefined8 local_228;
  undefined8 uStack_220;
  undefined8 local_218;
  undefined8 uStack_210;
  undefined8 local_208;
  undefined8 local_200;
  undefined8 uStack_1f8;
  undefined8 local_1f0;
  long lStack_1e8;
  undefined8 local_1e0;
  long lStack_1d8;
  undefined8 local_1d0;
  undefined8 uStack_1c8;
  undefined8 local_1c0;
  undefined8 uStack_1b8;
  undefined4 local_1b0;
  undefined4 uStack_1ac;
  undefined4 uStack_1a8;
  undefined4 uStack_1a4;
  undefined8 local_1a0;
  long lStack_198;
  undefined8 local_190;
  undefined8 uStack_188;
  undefined8 local_180;
  undefined8 uStack_178;
  undefined8 local_170;
  undefined8 uStack_168;
  undefined8 uStack_160;
  undefined8 uStack_158;
  undefined8 local_150;
  undefined8 uStack_148;
  undefined8 uStack_140;
  undefined8 uStack_138;
  undefined8 local_130;
  long local_128;
  undefined8 uStack_120;
  undefined8 local_118;
  undefined8 uStack_110;
  ulong local_108;
  undefined8 uStack_100;
  undefined8 local_f8;
  undefined8 uStack_f0;
  undefined8 local_e8;
  undefined8 uStack_e0;
  undefined8 local_d8;
  undefined8 uStack_d0;
  undefined8 local_c8;
  undefined8 uStack_c0;
  undefined8 local_b8;
  undefined8 uStack_b0;
  undefined8 local_a8;
  undefined8 uStack_a0;
  undefined8 local_98;
  undefined8 uStack_90;
  undefined8 local_88;
  undefined8 uStack_80;
  long local_78;
  
  lVar3 = tpidr_el0;
  local_78 = *(long *)(lVar3 + 0x28);
  if (param_3 != (undefined8 *)0x0) {
    param_3[1] = 0;
    *param_3 = 0;
    param_3[3] = 0;
    param_3[2] = 0;
    param_3[5] = 0;
    param_3[4] = 0;
    param_3[7] = 0;
    param_3[6] = 0;
    if (((param_2 != (int *)0x0) && (*param_2 == 2)) && (param_2[1] == 0x98)) {
      uVar10 = 0;
      if ((param_2[2] != 4) || (DAT_002147d8 == '\0')) goto LAB_0015203c;
      if ((*(long *)(param_2 + 4) == DAT_00209d00) &&
         ((*(long *)(param_2 + 8) == DAT_001e0978 && (iVar9 = FUN_00151bb4(0,param_2), iVar9 == 1)))
         ) {
        lVar12 = *(long *)(param_2 + 0x12);
        if ((lVar12 == DAT_001e0978 + 0xb2d5c8) && (*(long *)(param_2 + 0x1e) == 0)) {
          bVar8 = *(long *)(param_2 + 0x20) == 0;
        }
        else {
          bVar8 = false;
        }
        bVar7 = false;
        if ((lVar12 != DAT_001e0978 + 0xb2d3d4) && (lVar12 != DAT_001e0978 + 0xb380e0)) {
          if (*(long *)(param_2 + 0x1e) == 1) {
            bVar7 = param_2[0x21] == 0;
          }
          else {
            bVar7 = false;
          }
        }
        if ((bool)(bVar8 | bVar7)) {
          iVar9 = FUN_001574e0(param_2,param_3);
          if (iVar9 != 0) {
            uVar10 = 1;
            goto LAB_0015203c;
          }
          do {
            cVar6 = DAT_001cfe94;
            cVar2 = '\x01';
            bVar8 = (bool)ExclusiveMonitorPass(0x1cfe94,0x10);
            if (bVar8) {
              _DAT_001cfe94 = CONCAT31(DAT_001cfe94_1,1);
              cVar2 = ExclusiveMonitorsStatus();
            }
          } while (cVar2 != '\0');
          if (cVar6 != '\0') goto LAB_00152038;
          FUN_00153ff8(param_2);
          DAT_00215670 = DAT_00215670 + 1;
          memset(&DAT_00215678,0,0x280);
          local_208 = 0;
          local_240 = DAT_0010e7f8;
          uStack_258 = 0;
          local_260 = 0;
          uStack_248 = 0;
          uStack_250 = 0;
          lStack_1e8 = 0;
          local_1f0 = 0;
          lStack_1d8 = 0;
          local_1e0 = 0;
          local_130 = DAT_0010e7d0;
          uStack_1c8 = 0;
          local_1d0 = 0;
          uStack_1b8 = 0;
          local_1c0 = 0;
          uStack_1a8 = 0;
          uStack_1a4 = 0;
          local_1b0 = 0;
          uStack_1ac = 0;
          lStack_198 = 0;
          local_1a0 = 0;
          uStack_188 = 0;
          local_190 = 0;
          uStack_178 = 0;
          local_180 = 0;
          uStack_168 = 0;
          local_170 = 0;
          uStack_158 = 0;
          uStack_160 = 0;
          uStack_148 = 0;
          local_150 = 0;
          uStack_138 = 0;
          uStack_140 = 0;
          uStack_1f8 = 0;
          local_200 = 0;
          uStack_210 = 0;
          local_218 = 0;
          uStack_220 = 0;
          local_228 = 0;
          uStack_230 = 0;
          uStack_238 = 0;
          uStack_80 = 0;
          local_88 = 0;
          uStack_90 = 0;
          local_98 = 0;
          uStack_a0 = 0;
          local_a8 = 0;
          uStack_b0 = 0;
          local_b8 = 0;
          uStack_c0 = 0;
          local_c8 = 0;
          uStack_d0 = 0;
          local_d8 = 0;
          uStack_e0 = 0;
          local_e8 = 0;
          uStack_f0 = 0;
          local_f8 = 0;
          uStack_100 = 0;
          local_108 = 0;
          uStack_110 = 0;
          local_118 = 0;
          uStack_120 = 0;
          local_128 = 0;
          uStack_268 = 0;
          local_270 = 0;
          uStack_2a8 = 0;
          uStack_2b0 = 0;
          uStack_298 = 0;
          uStack_2a0 = 0;
          uStack_288 = 0;
          local_290 = 0;
          local_278 = 0;
          uStack_280 = 0;
          lStack_2b8 = 0;
          uStack_2c0 = 0;
          local_2c8 = DAT_0010e700;
          iVar9 = FUN_00150bf0(*(undefined8 *)(param_2 + 0xc),*(undefined8 *)(param_2 + 0xe));
          if (iVar9 == 0) {
            pcVar11 = "skill_identity_or_active";
          }
          else {
            iVar9 = (*DAT_0020d160)(&local_130);
            pcVar11 = "skill_configuration_or_generation";
            if ((((iVar9 == 1) && ((int)uStack_100 != 0)) &&
                (pcVar11 = "skill_configuration_or_generation", (local_108 & 1) != 0)) &&
               (local_128 == DAT_0020f5e8)) {
              iVar9 = FUN_00189c88(DAT_001e0978,*(undefined8 *)(param_2 + 0xe),FUN_001428fc,0,
                                   &DAT_0020f680);
              if ((iVar9 == 0) ||
                 (iVar9 = FUN_001543f8(DAT_0020f678,*(undefined8 *)(param_2 + 0xe)), iVar9 == 0)) {
                pcVar11 = "skill_current_actor_refresh";
              }
              else {
                iVar9 = FUN_0018a244(DAT_001e0978,*(undefined8 *)(param_2 + 0xe),
                                     *(undefined8 *)(param_2 + 0x24),FUN_001428fc,0,&local_270);
                if (iVar9 == 0) {
                  pcVar11 = "skill_context_not_current_member";
                }
                else {
                  local_2cc = 0;
                  iVar9 = FUN_001541f8(*(undefined8 *)(param_2 + 0xc),*(undefined8 *)(param_2 + 0xe)
                                       ,&local_270,&local_2cc);
                  if (iVar9 == 0) {
                    pcVar11 = "skill_ability_input_unknown";
                  }
                  else {
                    bVar8 = false;
                    if (local_2cc != 2) {
                      bVar8 = bVar7;
                    }
                    if (bVar8) {
                      pcVar11 = "skill_ultimate_route_not_live";
                    }
                    else {
                      pcVar1 = "skill_gadget_smartaim_disabled";
                      if (local_2cc != 3) {
                        pcVar1 = "skill_ability_allowed";
                      }
                      pcVar11 = "skill_ultimate_smartaim_disabled";
                      if (local_2cc != 2) {
                        pcVar11 = pcVar1;
                      }
                      if (local_2cc == 3) {
                        iVar9 = (int)local_d8;
LAB_001522a4:
                        if (iVar9 == 0) goto LAB_00152218;
                      }
                      else if (local_2cc == 2) {
                        iVar9 = uStack_e0._4_4_;
                        goto LAB_001522a4;
                      }
                      iVar9 = (*DAT_00213040)(0x17,4,&local_2c8);
                      if (iVar9 == 0) {
                        pcVar11 = "register_input_lease_unavailable";
                      }
                      else {
                        local_340 = *(undefined8 *)(param_2 + 0xc);
                        local_330 = *(undefined8 *)(param_2 + 0xe);
                        uStack_338 = DAT_0020f678;
                        local_320 = *(undefined8 *)(param_2 + 6);
                        uStack_328 = DAT_0020f6f8;
                        uStack_348 = _UNK_00112918;
                        local_350 = _DAT_00112910;
                        local_318 = DAT_0020f690;
                        local_314 = DAT_0020f638;
                        local_308 = FUN_001391f0(1);
                        local_368 = local_f8._4_4_;
                        local_300 = DAT_0010e588;
                        local_2f8 = 1;
                        uStack_2f4 = DAT_0020f694;
                        local_398 = FUN_00150f7c;
                        uStack_390 = 0;
                        local_3d0 = &local_3a8;
                        local_378 = 1;
                        uStack_374 = uStack_100._4_4_;
                        local_2e4 = CONCAT44(param_2[0x18],param_2[0x16]);
                        local_2e8 = 1;
                        local_3a8 = &DAT_0020d168;
                        puStack_3a0 = &DAT_0020f628;
                        local_388 = &local_240;
                        local_2f0 = 0;
                        lStack_380 = local_128;
                        local_370 = 0;
                        local_35c = 0;
                        local_364 = 0;
                        pcStack_3c8 = thunk_FUN_00186cc8;
                        local_354 = 0;
                        local_3c0 = FUN_001546d4;
                        uStack_3b8 = 0;
                        local_3b0 = FUN_00154808;
                        iVar9 = FUN_00184ce0(&DAT_00213098,&lStack_380,&local_350,&local_3d0,
                                             &local_200);
                        pcVar11 = "register_proposal_refused";
                        if (((iVar9 == 1) && (uStack_1f8._4_4_ == 2)) &&
                           ((lStack_1e8 == *(long *)(param_2 + 0xc) &&
                            (lStack_1d8 == *(long *)(param_2 + 0xe))))) {
                          auVar14._8_8_ = CONCAT44(uStack_1a8,uStack_1ac);
                          auVar14._0_8_ = CONCAT44(uStack_1a8,uStack_1ac);
                          auVar16 = NEON_cmgt(auVar14,_DAT_00112be0,4);
                          auVar14 = NEON_cmgt(_DAT_00112be0,auVar14,4);
                          uVar13 = NEON_umaxv(CONCAT26(auVar14._12_2_,
                                                       CONCAT24(auVar14._8_2_,
                                                                CONCAT22(auVar16._4_2_,auVar16._0_2_
                                                                        ))),2);
                          if (((uVar13 & 1) == 0) && (lStack_198 == lStack_2b8)) {
                            iVar9 = FUN_00150bf0();
                            if ((iVar9 != 0) && (iVar9 = (*DAT_00213048)(&local_2c8), iVar9 != 0)) {
                              uVar5 = *(undefined8 *)(param_2 + 4);
                              uVar15 = *(undefined8 *)(param_2 + 0x18);
                              uVar4 = *(undefined8 *)(param_2 + 0x16);
                              uVar10 = 1;
                              param_3[3] = *(undefined8 *)(param_2 + 6);
                              param_3[2] = uVar5;
                              param_3[5] = uVar15;
                              param_3[4] = uVar4;
                              uVar5 = _UNK_00112948;
                              uVar4 = _DAT_00112940;
                              param_3[6] = lStack_2b8;
                              DAT_002157a8 = local_278;
                              param_3[7] = CONCAT44(uStack_1a8,uStack_1ac);
                              param_3[1] = uVar5;
                              *param_3 = uVar4;
                              DAT_00215678 = 1;
                              uRam00000000002156f8 = *(undefined8 *)(param_2 + 0x1e);
                              _DAT_002156f0 = *(undefined8 *)(param_2 + 0x1c);
                              _DAT_00215700 = *(undefined8 *)(param_2 + 0x20);
                              uRam0000000000215708 = *(undefined8 *)(param_2 + 0x22);
                              DAT_00215710 = *(undefined8 *)(param_2 + 0x24);
                              _DAT_002156e0 = *(undefined8 *)(param_2 + 0x18);
                              uRam00000000002156e8 = *(undefined8 *)(param_2 + 0x1a);
                              uRam00000000002156c8 = *(undefined8 *)(param_2 + 0x12);
                              _DAT_002156c0 = *(undefined8 *)(param_2 + 0x10);
                              _DAT_002156d0 = *(undefined8 *)(param_2 + 0x14);
                              uRam00000000002156d8 = *(undefined8 *)(param_2 + 0x16);
                              uRam00000000002156a8 = *(undefined8 *)(param_2 + 10);
                              _DAT_002156a0 = *(undefined8 *)(param_2 + 8);
                              _DAT_002156b0 = *(undefined8 *)(param_2 + 0xc);
                              uRam00000000002156b8 = *(undefined8 *)(param_2 + 0xe);
                              uRam0000000000215688 = *(undefined8 *)(param_2 + 2);
                              _DAT_00215680 = *(undefined8 *)param_2;
                              _DAT_00215690 = *(undefined8 *)(param_2 + 4);
                              uRam0000000000215698 = *(undefined8 *)(param_2 + 6);
                              DAT_00215740 = param_3[5];
                              DAT_00215738 = param_3[4];
                              DAT_00215748 = param_3[6];
                              DAT_00215750 = param_3[7];
                              DAT_00215718 = *param_3;
                              DAT_00215720 = param_3[1];
                              DAT_00215730 = param_3[3];
                              DAT_00215728 = param_3[2];
                              _DAT_00215778 = uStack_2a8;
                              uRam0000000000215780 = uStack_2a0;
                              uRam0000000000215790 = local_290;
                              _DAT_00215788 = uStack_298;
                              _DAT_00215798 = uStack_288;
                              uRam00000000002157a0 = uStack_280;
                              _DAT_00215758 = local_2c8;
                              uRam0000000000215760 = uStack_2c0;
                              uRam0000000000215770 = uStack_2b0;
                              _DAT_00215768 = lStack_2b8;
                              DAT_002157b8 = uStack_268;
                              DAT_002157b0 = local_270;
                              DAT_002157c0 = local_260;
                              DAT_002157c8 = uStack_258;
                              DAT_002157d0 = uStack_250;
                              DAT_002157d8 = uStack_248;
                              lRam0000000000215808 = lStack_1d8;
                              _DAT_00215800 = local_1e0;
                              DAT_00215810 = local_1d0;
                              uRam0000000000215818 = uStack_1c8;
                              _DAT_002157e0 = local_200;
                              uRam00000000002157e8 = uStack_1f8;
                              lRam00000000002157f8 = lStack_1e8;
                              _DAT_002157f0 = local_1f0;
                              lRam0000000000215848 = lStack_198;
                              _DAT_00215840 = local_1a0;
                              _DAT_00215850 = local_190;
                              uRam0000000000215858 = uStack_188;
                              _DAT_00215820 = local_1c0;
                              uRam0000000000215828 = uStack_1b8;
                              uRam0000000000215838 = CONCAT44(uStack_1a4,uStack_1a8);
                              _DAT_00215830 = CONCAT44(uStack_1ac,local_1b0);
                              uRam0000000000215898 = uStack_148;
                              _DAT_00215890 = local_150;
                              _DAT_002158a0 = uStack_140;
                              uRam00000000002158a8 = uStack_138;
                              uRam0000000000215868 = uStack_178;
                              _DAT_00215860 = local_180;
                              _DAT_00215870 = local_170;
                              uRam0000000000215878 = uStack_168;
                              _DAT_00215880 = uStack_160;
                              uRam0000000000215888 = uStack_158;
                              DAT_002158f0 = local_2cc;
                              uRam00000000002158c8 = local_228;
                              _DAT_002158c0 = uStack_230;
                              uRam00000000002158d8 = local_218;
                              _DAT_002158d0 = uStack_220;
                              _DAT_002158e0 = uStack_210;
                              uRam00000000002158e8 = local_208;
                              uRam00000000002158b8 = uStack_238;
                              _DAT_002158b0 = local_240;
                              goto LAB_0015203c;
                            }
                            pcVar11 = "register_proposal_refused";
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
LAB_00152218:
          FUN_00157680(param_2,&local_200,&local_240,&local_130,pcVar11,0);
          uVar10 = 0;
          _DAT_001cfe94 = 0;
          goto LAB_0015203c;
        }
      }
    }
  }
LAB_00152038:
  uVar10 = 0;
LAB_0015203c:
  if (*(long *)(lVar3 + 0x28) != local_78) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar10);
  }
  return;
}

/* ===== FUN_00157680 @ 00157680 [libNexusEvasionRuntime69252.so] ===== */

void FUN_00157680(long param_1,long param_2,long param_3,long param_4,undefined8 param_5,int param_6
                 )

{
  undefined4 uVar1;
  undefined4 uVar2;
  long lVar3;
  int iVar4;
  long lVar5;
  timespec local_778 [112];
  long local_70;
  
  lVar3 = tpidr_el0;
  local_70 = *(long *)(lVar3 + 0x28);
  iVar4 = clock_gettime(1,local_778);
  if (iVar4 == 0) {
    lVar5 = local_778[0].tv_sec * 1000 + (ulong)local_778[0].tv_nsec / 1000000;
  }
  else {
    lVar5 = 0;
  }
  if ((((DAT_00215670 < 0x21) || (DAT_002159f0 < 9 && param_6 != 0)) || (DAT_002159f8 == 0)) ||
     (999 < (ulong)(lVar5 - DAT_002159f8))) {
    uVar1 = *(undefined4 *)(param_1 + 0x60);
    if (param_6 != 0) {
      uVar1 = *(undefined4 *)(param_2 + 0x58);
    }
    uVar2 = *(undefined4 *)(param_1 + 0x58);
    if (param_6 != 0) {
      uVar2 = *(undefined4 *)(param_2 + 0x54);
    }
    snprintf((char *)local_778,0x708,
             ",\"action\":23,\"kind\":4,\"destination\":2,\"operation\":4,\"delivery\":%llu,\"event_count\":%llu,\"caller_lr_rva\":\"0x%llx\",\"publication\":%llu,\"source_tick\":%u,\"clock_known\":%u,\"map_ready\":%u,\"config_supported\":%u,\"active\":%u,\"generation\":%llu,\"target_gid\":%u,\"plan_reason\":%d,\"selection_reason\":%u,\"untouched_xy_known\":1,\"untouched_raw_x\":%d,\"untouched_raw_y\":%d,\"proposed_raw_x\":%d,\"proposed_raw_y\":%d,\"committed_xy_known\":1,\"committed_raw_x\":%d,\"committed_raw_y\":%d,\"register_xy_committed\":%d,\"register_xy_commits\":%llu,\"own_gid\":%u,\"own_x\":%.6f,\"own_y\":%.6f,\"ended\":%u,\"extra_fire_calls\":0,\"screen_xy_writes\":0"
             ,(double)DAT_0020f6b4,(double)DAT_0020f6b8,*(undefined8 *)(param_1 + 0x18),DAT_00215670
             ,*(long *)(param_1 + 0x48) - *(long *)(param_1 + 0x20),DAT_0020f650,
             (ulong)DAT_0020f638._4_4_,(undefined4)DAT_0020f638,(uint)DAT_0020d15c,
             *(uint *)(param_4 + 0x28) & 1,DAT_0020f7c4,*(undefined8 *)(param_4 + 8),
             *(undefined4 *)(param_2 + 0x50),*(undefined4 *)(param_2 + 8),
             *(undefined4 *)(param_3 + 8),*(undefined4 *)(param_1 + 0x58),
             *(undefined4 *)(param_1 + 0x60),*(undefined4 *)(param_2 + 0x54),
             *(undefined4 *)(param_2 + 0x58),uVar2,uVar1,param_6,DAT_002159f0,DAT_0020f690,
             DAT_0020f7e8);
    FUN_001417c8("smartaim_input",param_5,local_778);
    DAT_002159f8 = lVar5;
  }
  if (*(long *)(lVar3 + 0x28) == local_70) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0015b854 @ 0015b854 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0015b854(long param_1)

{
  uint uVar1;
  long lVar2;
  bool bVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  undefined4 uVar7;
  undefined8 uVar8;
  ulong uVar9;
  uint uVar10;
  long lVar11;
  long lVar12;
  ulong uVar13;
  undefined8 *puVar14;
  undefined8 local_850;
  undefined8 uStack_848;
  undefined8 local_840;
  undefined8 uStack_838;
  undefined8 local_830;
  undefined8 uStack_828;
  undefined8 local_820;
  int local_814;
  undefined8 local_810;
  undefined8 uStack_808;
  ulong local_800;
  undefined8 uStack_7f8;
  undefined8 local_7f0;
  undefined8 uStack_7e8;
  undefined8 uStack_7e0;
  undefined8 local_7d8;
  ulong local_7d0;
  long local_7c8;
  ulong local_7c0;
  ulong local_7b8;
  undefined8 local_7b0;
  undefined8 local_7a8;
  long lStack_7a0;
  undefined4 local_798;
  undefined8 local_794;
  undefined8 uStack_78c;
  undefined8 local_784;
  undefined8 uStack_77c;
  undefined8 local_108;
  undefined8 local_100;
  undefined8 uStack_f8;
  undefined8 local_f0;
  undefined8 uStack_e8;
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
  long local_78;
  
  lVar2 = tpidr_el0;
  local_78 = *(long *)(lVar2 + 0x28);
  if (DAT_002147dc != '\x01') goto LAB_0015bd1c;
  DAT_00214818 = 0;
  _DAT_00214800 = 0;
  DAT_002147f8 = 0;
  DAT_00214810 = 0;
  DAT_00214808 = 0;
  _DAT_0020f7c0 = 0;
  _DAT_0020f7b8 = 0;
  uRam000000000020f7d0 = 0;
  _DAT_0020f7c8 = 0;
  uRam000000000020f7e0 = 0;
  _DAT_0020f7d8 = 0;
  _DAT_0020f7e8 = 0;
  _DAT_0020f630 = 0;
  DAT_0020f628 = 0;
  _DAT_0020f640 = 0;
  DAT_0020f638 = 0;
  DAT_0020f650 = 0;
  _DAT_0020f648 = 0;
  DAT_0020f660 = 0;
  _DAT_0020f658 = 0;
  DAT_0020f670 = 0;
  DAT_0020f668 = 0;
  DAT_0020f680 = 0;
  DAT_0020f678 = 0;
  _DAT_0020f690 = 0;
  _DAT_0020f688 = 0;
  _DAT_0020f6a0 = 0;
  _DAT_0020f698 = 0;
  _DAT_0020f6b0 = 0;
  _DAT_0020f6a8 = 0;
  DAT_0020f6c0 = 0;
  _DAT_0020f6b8 = 0;
  _DAT_0020f6d0 = 0;
  _DAT_0020f6c8 = 0;
  local_7c8 = 0;
  uStack_808 = 0;
  local_810 = 0;
  uStack_7f8 = 0;
  local_800 = 0;
  uStack_7e8 = 0;
  local_7f0 = 0;
  local_7d8 = 0;
  uStack_7e0 = 0;
  if ((param_1 == 0) || (lVar11 = *(long *)(param_1 + 8), lVar11 == 0)) {
    lVar12 = 0;
    uVar9 = 0;
LAB_0015ba70:
    uVar8 = 0;
    bVar3 = true;
    local_7c8 = 0;
    DAT_00217328 = 0;
  }
  else {
    uVar13 = *(ulong *)(lVar11 + 0x10);
    lVar12 = 0;
    if (uVar13 + 0x2000 < 0x12000) {
      uVar9 = 0;
      goto LAB_0015ba70;
    }
    uVar9 = 0;
    if ((uVar13 & 7) != 0) goto LAB_0015ba70;
    lVar12 = *(long *)(lVar11 + 0x18);
    uVar8 = FUN_00189c88(DAT_001e0978,lVar12,FUN_001428fc,0,&local_810);
    uVar9 = uVar13;
    if (((((((int)uVar8 == 0) || (uStack_7e8._4_4_ == 0)) ||
          ((ABS(uStack_7e0._4_4_) < 0.5 && (ABS((float)local_7d8) < 0.5)))) ||
         (((((uVar8 = FUN_001428fc(uVar8,DAT_001e0978 + 0x1307e20,&local_7b8,8), (int)uVar8 == 0 ||
             (local_7b8 + 0x2000 < 0x12000)) || ((local_7b8 & 7) != 0)) ||
           ((uVar8 = FUN_001428fc(uVar8,local_7b8 + 0x50,&local_814,4), (int)uVar8 == 0 ||
            (local_814 != 5)))) ||
          (uVar8 = FUN_001428fc(uVar8,local_7b8 + 0x48,&local_7c0,8), (int)uVar8 == 0)))) ||
        ((local_7c0 + 0x2000 < 0x12000 || ((local_7c0 & 7) != 0)))) ||
       (((iVar4 = FUN_001428fc(uVar8,uVar13 + 0x918,&local_7d0,8), iVar4 == 0 ||
         (((0xfffffffffffedfff < local_7d0 - 0x10000 || ((local_7d0 & 7) != 0)) ||
          (local_7d0 != local_7c0)))) ||
        (iVar4 = FUN_0013a78c(local_7d0 + 0x28,&local_7c8), iVar4 == 0)))) goto LAB_0015ba70;
    bVar3 = false;
    uVar8 = 1;
  }
  local_820 = 0;
  uStack_838 = 0;
  local_840 = 0;
  uStack_828 = 0;
  local_830 = 0;
  uStack_848 = 0;
  local_850 = 0;
  if (DAT_00217330 == -1) goto LAB_0015bd1c;
  DAT_00217330 = DAT_00217330 + 1;
  FUN_00189b6c(&DAT_00213060,DAT_00217330,uVar9,local_7c8,uVar8,&local_850);
  if (param_1 == 0) {
    DAT_0020f668 = 0;
  }
  else {
    DAT_0020f668 = *(undefined8 *)(param_1 + 0x28);
  }
  _DAT_0020f6d0 = _DAT_0020f6d0 & 0xffffffff00000000;
  DAT_0020f6c0 = 0;
  _DAT_0020f6c8 = 0;
  _DAT_0020f688 = uStack_808;
  DAT_0020f680 = local_810;
  _DAT_0020f698 = uStack_7f8;
  _DAT_0020f690 = local_800;
  _DAT_0020f6a8 = uStack_7e8;
  _DAT_0020f6a0 = local_7f0;
  _DAT_0020f6b8 = local_7d8;
  _DAT_0020f6b0 = uStack_7e0;
  DAT_0020f628 = DAT_0010e550;
  DAT_0020f638 = uStack_848;
  _DAT_0020f630 = local_850;
  _DAT_0020f648 = uStack_838;
  _DAT_0020f640 = local_840;
  _DAT_0020f658 = uStack_828;
  DAT_0020f650 = local_830;
  DAT_0020f660 = local_820;
  DAT_0020f678 = local_7c8;
  DAT_0020f670 = uVar9;
  if ((((bVar3) || (*(int *)(param_1 + 0x24) == 0)) ||
      ((puVar14 = *(undefined8 **)(param_1 + 0x10), puVar14 == (undefined8 *)0x0 ||
       ((*(int *)(puVar14 + 0xd) == 0 || ((int)uStack_848 == 0)))))) ||
     ((puVar14[5] != uVar9 ||
      (((puVar14[7] != local_7c8 || (puVar14[8] != lVar12)) ||
       (*(int *)(puVar14 + 0xe) != (int)local_800)))))) {
    DAT_0020f624 = 0;
    goto LAB_0015bd1c;
  }
  if (((DAT_0020f6f8 != puVar14[4]) || (DAT_0020f700 != uVar9)) || (DAT_0020f710 != local_7c8)) {
    DAT_00217328 = 0;
    DAT_0020d15c = 0;
    DAT_00217338 = 0;
    DAT_0021733c = 0;
    DAT_00217340 = 0;
    DAT_00217344 = '\0';
    FUN_0018e7f0();
  }
  uRam000000000020f6e0 = puVar14[1];
  _DAT_0020f6d8 = *puVar14;
  DAT_0020f710 = puVar14[7];
  DAT_0020f708 = puVar14[6];
  uRam000000000020f720 = puVar14[9];
  DAT_0020f718 = puVar14[8];
  DAT_0020f6f0 = puVar14[3];
  _DAT_0020f6e8 = puVar14[2];
  DAT_0020f700 = puVar14[5];
  DAT_0020f6f8 = puVar14[4];
  _DAT_0020f750 = puVar14[0xf];
  _DAT_0020f748 = puVar14[0xe];
  _DAT_0020f760 = puVar14[0x11];
  _DAT_0020f758 = puVar14[0x10];
  DAT_0020f730 = puVar14[0xb];
  DAT_0020f728 = puVar14[10];
  uRam000000000020f740 = puVar14[0xd];
  _DAT_0020f738 = puVar14[0xc];
  uRam000000000020f790 = puVar14[0x17];
  _DAT_0020f788 = puVar14[0x16];
  uRam000000000020f7a0 = puVar14[0x19];
  _DAT_0020f798 = puVar14[0x18];
  uRam000000000020f770 = puVar14[0x13];
  _DAT_0020f768 = puVar14[0x12];
  uRam000000000020f780 = puVar14[0x15];
  _DAT_0020f778 = puVar14[0x14];
  DAT_002148a0 = 0;
  DAT_002148a4 = 0;
  _DAT_00214898 = 0;
  uRam00000000002148b0 = 0;
  _DAT_002148b4 = 0;
  _DAT_002148a8 = 0;
  uRam00000000002148ac = 0;
  uRam00000000002148c0 = 0;
  uRam00000000002148c4 = 0;
  DAT_002148b8 = 0;
  uRam00000000002148bc = 0;
  uRam00000000002148d0 = 0;
  _DAT_002148c8 = 0;
  _DAT_002148e0 = 0;
  _DAT_002148d8 = 0;
  uRam00000000002148f0 = 0;
  _DAT_002148e8 = 0;
  uRam0000000000214900 = 0;
  _DAT_002148f8 = 0;
  uRam0000000000214910 = 0;
  _DAT_00214908 = 0;
  uRam0000000000214920 = 0;
  _DAT_00214918 = 0;
  uRam0000000000214890 = 0;
  _DAT_00214888 = 0;
  uRam0000000000214880 = 0;
  DAT_00214878 = 0;
  DAT_00214870 = DAT_0010e7d0;
  if (((DAT_0020d160 == (code *)0x0) || (iVar4 = (*DAT_0020d160)(&DAT_00214870), iVar4 != 1)) ||
     (iVar4 = FUN_001543f8(local_7c8,lVar12), iVar4 == 0)) goto LAB_0015bd1c;
  _DAT_0020f6c8 = CONCAT44(1,DAT_0020f6c8);
  iVar4 = FUN_00150bf0(uVar9,lVar12);
  if (iVar4 == 0) {
    if (DAT_00217344 == '\x01') {
      DAT_00217328 = 0;
      goto LAB_0015bcc4;
    }
  }
  else {
LAB_0015bcc4:
    DAT_00217344 = iVar4 != 0;
  }
  _DAT_0020f6d0 = (CONCAT44(DAT_0020f6d0_4,(uint)DAT_00217328) ^ 0xffffffff) & 0xffffffff00000001;
  if (DAT_002148a0 == 0) {
    if (DAT_002148cc == 0) goto LAB_0015bd58;
LAB_0015bd8c:
    FUN_00161098(DAT_0020f678,DAT_0020f728,DAT_0020f638._4_4_,DAT_0020f6cc);
    uStack_78c = CONCAT44(uRam00000000002148b0,uRam00000000002148ac);
    local_794 = CONCAT44(_DAT_002148a8,DAT_002148a4);
    uStack_77c = CONCAT44(uRam00000000002148c0,uRam00000000002148bc);
    local_784 = CONCAT44(DAT_002148b8,_DAT_002148b4);
    local_7a8 = DAT_00214878;
    lStack_7a0 = DAT_0020f6f8;
    local_798 = 1;
    local_7b0 = DAT_0010e7f8;
    iVar5 = FUN_001858c0(&DAT_0020d168,&DAT_0020f628,&local_7b0,0);
    if (iVar5 != 0) {
      DAT_00217328 = 1;
    }
  }
  else {
    if ((_DAT_00214898 & 1) != 0 || DAT_002148cc != 0) goto LAB_0015bd8c;
LAB_0015bd58:
    iVar5 = FUN_00160fc0();
    if ((((iVar5 != 0) || (DAT_002148e0 != 0)) || (DAT_002148d8 != 0)) || (DAT_002148e8 != 0))
    goto LAB_0015bd8c;
    DAT_0020f624 = 0;
  }
  iVar5 = DAT_00209cd8;
  if ((DAT_00209cd8 == 0) || (iVar6 = gettid(), iVar5 != iVar6)) {
    uVar10 = 0x1a;
  }
  else {
    uVar10 = 0x1b;
  }
  uVar1 = uVar10 | 4;
  if (iVar4 == 0) {
    uVar1 = uVar10;
  }
  DAT_00214808 = local_830;
  DAT_002147f8 = DAT_0010e668;
  DAT_00214810 = puVar14[4];
  uVar10 = uVar1 | 0x20;
  if ((DAT_0020f624 != 0 & DAT_0020d15c & DAT_002147d8) == 0) {
    uVar10 = uVar1;
  }
  _DAT_00214800 = CONCAT44(uStack_848._4_4_,uVar10);
  DAT_00214818 = local_800 & 0xffffffff;
  if ((DAT_002148a0 != 0) &&
     ((DAT_00217348 == 0 || (999 < (ulong)(*(long *)(param_1 + 0x28) - DAT_00217348))))) {
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
    uStack_d8 = 0;
    local_e0 = 0;
    uStack_e8 = 0;
    local_f0 = 0;
    uStack_f8 = 0;
    local_100 = 0;
    local_108 = DAT_0010e760;
    uVar7 = FUN_00184398(&DAT_00213130,&local_108);
    snprintf((char *)&local_7b0,0x6a4,
             ",\"action\":23,\"publication\":%llu,\"source_tick\":%u,\"source_known\":%u,\"actors\":%u,\"map_known\":%u,\"evidence\":%u,\"generation\":%llu,\"xy_writes\":%llu,\"manual_events\":%llu,\"wrapper_events\":%llu,\"wrong_thread_events\":%llu,\"bad_receiver_events\":%llu,\"route_status_known\":%d,\"route_phase\":%u,\"route_disabled\":%llu,\"route_nested\":%llu,\"skill_events\":%llu,\"skill_proposed\":%llu,\"skill_committed\":%llu,\"skill_refused\":%llu,\"wrapper_continuation\":%llu,\"unsupported_caller\":%llu,\"unsupported_flags\":%llu,\"folded\":%llu,\"unknown_origin\":%llu,\"map_refresh_us\":%llu,\"map_failure_stage\":%u,\"map_failure_index\":%u"
             ,local_830,uStack_848 >> 0x20,uStack_848 & 0xffffffff,_DAT_0020f6c8 & 0xffffffff,
             (ulong)DAT_0020d15c,uVar10,DAT_00214878,DAT_002147e8,local_f0,uStack_e8,uStack_d8,
             local_c0,uVar7,(undefined4)local_100,uStack_c8,local_d0,local_e0,uStack_b8,local_b0,
             uStack_a8,local_90,uStack_88,local_80,local_a0,uStack_98,DAT_00217350,DAT_00217358,
             DAT_0021735c);
    FUN_001417c8("function_frame","standalone_aim_feed",&local_7b0);
    DAT_00217348 = *(long *)(param_1 + 0x28);
  }
LAB_0015bd1c:
  if (*(long *)(lVar2 + 0x28) == local_78) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

