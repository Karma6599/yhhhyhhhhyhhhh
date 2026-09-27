/*
 * themes — UI subsystem
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
 * Notes: Theme selection/preview + music options.
 */

/* ===== FUN_00172aac @ 00172aac ===== */

/* WARNING: Type propagation algorithm not settling */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00172aac(long param_1)

{
  uint uVar1;
  long lVar2;
  long lVar3;
  int iVar4;
  undefined8 uVar5;
  undefined8 local_178;
  undefined8 local_170;
  long lStack_168;
  undefined8 uStack_160;
  undefined8 uStack_158;
  undefined8 local_150;
  undefined8 uStack_148;
  undefined8 uStack_140;
  undefined8 uStack_138;
  undefined8 local_130;
  undefined8 uStack_128;
  undefined8 uStack_120;
  undefined8 uStack_118;
  undefined8 local_110;
  undefined8 uStack_108;
  undefined8 uStack_100;
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
  undefined8 uStack_78;
  undefined8 local_70;
  undefined8 uStack_68;
  undefined8 local_60;
  undefined8 uStack_58;
  undefined8 uStack_50;
  undefined8 uStack_48;
  undefined8 local_40;
  long local_38;
  
  lVar2 = tpidr_el0;
  local_38 = *(long *)(lVar2 + 0x28);
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
  uStack_d8 = 0;
  local_e0 = 0;
  uStack_e8 = 0;
  local_f0 = 0;
  uStack_58 = 0;
  local_60 = 0;
  uStack_48 = 0;
  uStack_50 = 0;
  uStack_68 = 0;
  local_70 = 0;
  lStack_168 = 0;
  local_170 = 0;
  uStack_158 = 0;
  uStack_160 = 0;
  uStack_148 = 0;
  local_150 = 0;
  uStack_138 = 0;
  uStack_140 = 0;
  uStack_128 = 0;
  local_130 = 0;
  uStack_118 = 0;
  uStack_120 = 0;
  uStack_108 = 0;
  local_110 = 0;
  uStack_f8 = 0;
  uStack_100 = 0;
  local_40 = 0;
  local_178 = DAT_0010f7f0;
  if (((*(char *)(param_1 + 10) == '\0') && (*(char *)(param_1 + 8) == 't')) &&
     (iVar4 = FUN_0016d910(), iVar4 != 0)) {
    iVar4 = nexus_menu_theme_preview(&local_178,0x140);
    lVar3 = DAT_001a7d30;
    if (iVar4 != 0) {
      if ((DAT_001a7d30 == 0) || ((int)local_170 == 0)) {
        if ((DAT_00284650 != 0) && (iVar4 = FUN_00182b58(DAT_00284650,DAT_00284658), iVar4 == 0))
        goto LAB_00172d1c;
        uVar5 = 0;
        DAT_00284670 = 0;
        DAT_00284658 = 0;
        DAT_00284650 = 0;
        _DAT_00284668 = 0;
        DAT_00284660 = 0;
        goto joined_r0x00172d9c;
      }
      if ((DAT_00284660 == lStack_168) && (DAT_00284658 == DAT_001a7d30)) {
        if (DAT_0028466c != 0) goto LAB_00172d1c;
LAB_00172c48:
        if (DAT_00284660 == 0) {
LAB_00172d10:
          _DAT_00284668 = CONCAT44(1,DAT_00284668);
          goto LAB_00172d1c;
        }
        iVar4 = FUN_00181d04(&uStack_160);
        if (iVar4 < 0) goto LAB_00172d10;
        if (iVar4 == 0) {
          uVar1 = (int)DAT_00284670 + 1;
          DAT_00284670 = CONCAT44(DAT_00284670._4_4_,uVar1);
          if (0xb4 < uVar1) goto LAB_00172d10;
          uVar5 = 0;
        }
        else {
          DAT_00284670 = (ulong)DAT_00284670._4_4_ << 0x20;
          if (DAT_00284650 == 0) {
            DAT_00284650 = FUN_00181ea0(&uStack_160,local_170._4_4_);
            if (DAT_00284650 == 0) {
              uVar5 = 0xffffffff;
              _DAT_00284668 = CONCAT44(1,DAT_00284668);
              goto joined_r0x00172d9c;
            }
            iVar4 = FUN_001826ec(lVar3,DAT_00284650);
            if (iVar4 == 0) goto LAB_00172d10;
          }
          iVar4 = FUN_00181b78(DAT_00284650,lVar3);
          if ((iVar4 == 0) ||
             ((DAT_00284668 == 0 && (iVar4 = FUN_00182848(DAT_00284650), iVar4 == 0)))) {
            _DAT_00284668 = 0;
            goto LAB_00172d10;
          }
          uVar5 = 1;
          _DAT_00284668 = CONCAT44(DAT_0028466c,1);
        }
      }
      else {
        if ((DAT_00284650 == 0) || (iVar4 = FUN_00182b58(), iVar4 != 0)) {
          DAT_00284670 = 0;
          DAT_00284650 = 0;
          _DAT_00284668 = 0;
          DAT_00284658 = lVar3;
          DAT_00284660 = lStack_168;
          goto LAB_00172c48;
        }
LAB_00172d1c:
        if ((DAT_00284650 != 0) && (iVar4 = FUN_00181b78(DAT_00284650,DAT_00284658), iVar4 != 0)) {
          FUN_0016cf5c(0xc61c3c00,0xc61c3c00,DAT_00284650);
        }
        uVar5 = 0xffffffff;
      }
joined_r0x00172d9c:
      if ((int)local_170 != 0) {
        nexus_menu_theme_preview_report(lStack_168,uVar5);
      }
    }
  }
  else if ((DAT_00284650 == 0) || (iVar4 = FUN_00182b58(DAT_00284650,DAT_00284658), iVar4 != 0)) {
    if (*(long *)(lVar2 + 0x28) == local_38) {
      DAT_00284650 = 0;
      DAT_00284658 = 0;
      DAT_00284660 = 0;
      _DAT_00284668 = 0;
      DAT_00284670 = 0;
      return;
    }
    goto LAB_00172da4;
  }
  if (*(long *)(lVar2 + 0x28) == local_38) {
    return;
  }
LAB_00172da4:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== nexus_menu_theme_preview @ 00186108 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 nexus_menu_theme_preview(int *param_1,long param_2)

{
  ushort uVar1;
  int iVar2;
  undefined8 uVar3;
  ulong uVar4;
  ushort uVar5;
  undefined1 auVar6 [16];
  
  uVar3 = 0;
  if ((param_1 != (int *)0x0) && (param_2 == 0x140)) {
    if ((*param_1 == 1) &&
       ((param_1[1] == 0x140 && (uVar4 = FUN_00193f80(1,&DAT_00284688), (uVar4 & 1) == 0)))) {
      param_1[0x4e] = 0;
      uVar3 = DAT_0010f7f0;
      param_1[0x4f] = 0;
      param_1[8] = 0;
      param_1[9] = 0;
      param_1[6] = 0;
      param_1[7] = 0;
      param_1[0xc] = 0;
      param_1[0xd] = 0;
      param_1[10] = 0;
      param_1[0xb] = 0;
      param_1[0x10] = 0;
      param_1[0x11] = 0;
      param_1[0xe] = 0;
      param_1[0xf] = 0;
      param_1[0x14] = 0;
      param_1[0x15] = 0;
      param_1[0x12] = 0;
      param_1[0x13] = 0;
      param_1[0x18] = 0;
      param_1[0x19] = 0;
      param_1[0x16] = 0;
      param_1[0x17] = 0;
      param_1[0x1c] = 0;
      param_1[0x1d] = 0;
      param_1[0x1a] = 0;
      param_1[0x1b] = 0;
      *(undefined8 *)param_1 = uVar3;
      param_1[0x20] = 0;
      param_1[0x21] = 0;
      param_1[0x1e] = 0;
      param_1[0x1f] = 0;
      param_1[0x24] = 0;
      param_1[0x25] = 0;
      param_1[0x22] = 0;
      param_1[0x23] = 0;
      param_1[0x28] = 0;
      param_1[0x29] = 0;
      param_1[0x26] = 0;
      param_1[0x27] = 0;
      param_1[0x2c] = 0;
      param_1[0x2d] = 0;
      param_1[0x2a] = 0;
      param_1[0x2b] = 0;
      param_1[0x30] = 0;
      param_1[0x31] = 0;
      param_1[0x2e] = 0;
      param_1[0x2f] = 0;
      param_1[0x34] = 0;
      param_1[0x35] = 0;
      param_1[0x32] = 0;
      param_1[0x33] = 0;
      param_1[0x38] = 0;
      param_1[0x39] = 0;
      param_1[0x36] = 0;
      param_1[0x37] = 0;
      param_1[0x3c] = 0;
      param_1[0x3d] = 0;
      param_1[0x3a] = 0;
      param_1[0x3b] = 0;
      param_1[0x40] = 0;
      param_1[0x41] = 0;
      param_1[0x3e] = 0;
      param_1[0x3f] = 0;
      param_1[4] = 0;
      param_1[5] = 0;
      param_1[2] = 0;
      param_1[3] = 0;
      param_1[0x44] = 0;
      param_1[0x45] = 0;
      param_1[0x42] = 0;
      param_1[0x43] = 0;
      param_1[0x48] = 0;
      param_1[0x49] = 0;
      param_1[0x46] = 0;
      param_1[0x47] = 0;
      param_1[0x4c] = 0;
      param_1[0x4d] = 0;
      param_1[0x4a] = 0;
      uVar3 = DAT_00287c60;
      param_1[0x4b] = 0;
      auVar6._4_4_ = DAT_00285410;
      auVar6._0_4_ = DAT_00287b98;
      auVar6._8_8_ = _DAT_00287b9c;
      auVar6 = NEON_cmeq(auVar6,0,2);
      uVar5 = NEON_umaxv(CONCAT26(auVar6._12_2_,
                                  CONCAT24(auVar6._8_2_,CONCAT22(auVar6._4_2_,auVar6._0_2_))),2);
      uVar1 = 0;
      if (DAT_0028469c != '\0') {
        uVar1 = (ushort)(DAT_00287b90 == 2) & (uVar5 ^ 0xffff);
      }
      uVar5 = 0;
      if (DAT_0028469e == '\0') {
        uVar5 = uVar1;
      }
      if (((uVar5 == 1) && (DAT_0028469d == 't')) && (DAT_00287c40 == DAT_00285414)) {
        param_1[2] = 1;
        iVar2 = DAT_00285424;
        *(undefined8 *)(param_1 + 4) = uVar3;
        param_1[3] = DAT_00285438 & (iVar2 << 0x1c) >> 0x1f;
        memcpy(param_1 + 6,&DAT_0028793c,0x128);
      }
      uVar3 = 1;
      DAT_00284688 = 0;
    }
    else {
      uVar3 = 0;
    }
  }
  return uVar3;
}

/* ===== nexus_menu_theme_scroll_revision @ 001862a4 ===== */

undefined8 nexus_menu_theme_scroll_revision(void)

{
  undefined8 uVar1;
  
  uVar1 = 0;
  if (((DAT_0028469c != '\0') && (DAT_0028469e == '\0')) && (DAT_0028469d == 't')) {
    uVar1 = DAT_0022d824;
  }
  return uVar1;
}

/* ===== nexus_menu_theme_preview_report @ 001862dc ===== */

bool nexus_menu_theme_preview_report(long param_1,int param_2)

{
  char *pcVar1;
  char *pcVar2;
  bool bVar3;
  ulong uVar4;
  
  bVar3 = false;
  if (param_1 != 0) {
    if (0xfffffffc < param_2 - 2U) {
      uVar4 = FUN_00193f80(1,&DAT_00284688);
      if ((uVar4 & 1) == 0) {
        bVar3 = false;
        if ((((DAT_0028469c != '\0') && (DAT_0028469e == '\0')) && (DAT_0028469d == 't')) &&
           (((DAT_00287b90 == 2 && (bVar3 = DAT_00287c60 == param_1, bVar3)) &&
            (DAT_00287c68 != param_2)))) {
          pcVar1 = "LOADING PREVIEW";
          if (param_2 != 0) {
            pcVar1 = "PREVIEW COULD NOT LOAD";
          }
          pcVar2 = "PREVIEW READY - CONFIRM TO APPLY";
          if (param_2 < 1) {
            pcVar2 = pcVar1;
          }
          DAT_00287c68 = param_2;
          FUN_00183ac8(&DAT_00287c6c,0x60,0x60,&DAT_001343dc,pcVar2);
          bVar3 = true;
          DAT_00287bb4 = 0;
          DAT_0028469f = 0;
          DAT_00287bac = DAT_00287bac + 1;
          DAT_002846a0 = DAT_002846a0 + 1;
        }
        DAT_00284688 = 0;
      }
      else {
        bVar3 = false;
      }
    }
  }
  return bVar3;
}

/* ===== nexus_menu_theme_open @ 0018640c ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 nexus_menu_theme_open(int param_1)

{
  long lVar1;
  uint uVar2;
  ulong uVar3;
  undefined8 uVar4;
  
  uVar3 = FUN_00193f80(1,&DAT_00284688);
  if ((uVar3 & 1) == 0) {
    uVar4 = 4;
    if ((((0 < param_1) && (DAT_00284694 == param_1)) && (DAT_0028469e == '\0')) &&
       (DAT_0028469c != '\0')) {
      uVar2 = DAT_00287b8c;
      if (DAT_0028469d != 0x74) {
        uVar2 = (uint)DAT_0028469d;
      }
      lVar1 = DAT_00287c48 + 1;
      memset(&DAT_00285408,0,0x4fc8);
      DAT_0028469d = 0x74;
      DAT_00287b8c = uVar2;
      DAT_00287c48 = lVar1;
      FUN_00183ac8(&DAT_00287c6c,0x60,0x60,&DAT_001343dc,"CHOOSE A BACKGROUND");
      uVar4 = 1;
      DAT_00287ba4 = 1;
      DAT_00287c50 = DAT_00287c48;
      _DAT_00287b94 = 0;
      FUN_00194010(1,&DAT_0022d824);
      DAT_00287bb4 = 0;
      DAT_0028469f = 0;
      DAT_00287bac = DAT_00287bac + 1;
      DAT_002846a0 = DAT_002846a0 + 1;
    }
    DAT_00284688 = 0;
  }
  else {
    uVar4 = 3;
  }
  return uVar4;
}

/* ===== nexus_menu_theme_pump @ 00186d58 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 nexus_menu_theme_pump(int param_1,ulong param_2)

{
  long lVar1;
  bool bVar2;
  undefined8 uVar3;
  uint uVar4;
  undefined4 uVar5;
  uint uVar6;
  bool bVar7;
  bool bVar8;
  bool bVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  ulong uVar13;
  ulong uVar14;
  undefined4 uVar15;
  uint uVar16;
  char *pcVar17;
  uint local_4dac;
  long local_4d58;
  undefined8 local_4d40;
  int local_4d38;
  int local_4d34;
  int local_2808 [2];
  char acStack_2800 [96];
  char acStack_27a0 [96];
  char acStack_2740 [96];
  int local_26e0 [2];
  char acStack_26d8 [96];
  char acStack_2678 [96];
  char acStack_2618 [96];
  undefined8 local_25b8;
  int local_25b0;
  int local_25ac;
  uint local_25a8;
  uint local_25a0;
  char acStack_251c [9372];
  long local_80;
  
  lVar1 = tpidr_el0;
  local_80 = *(long *)(lVar1 + 0x28);
  uVar13 = FUN_00193f80(1,&DAT_00284688);
  iVar12 = DAT_00287c44;
  uVar5 = DAT_00287bbc;
  uVar15 = DAT_00287bb8;
  uVar4 = DAT_00287ba8;
  uVar14 = _DAT_00287b94;
  if ((uVar13 & 1) != 0) {
LAB_00187240:
    uVar15 = 3;
    goto LAB_00187244;
  }
  if ((param_1 < 1) || (DAT_00284694 != param_1)) {
    uVar15 = 4;
  }
  else if (((DAT_0028469c == '\0') || (DAT_0028469e != '\0')) || (DAT_0028469d != 't')) {
    uVar15 = 1;
    DAT_00287ba4 = 0;
    _DAT_00287b94 = _DAT_00287b94 & 0xffffffff;
    DAT_00287c48 = DAT_00287c48 + 1;
  }
  else if (DAT_00287ba4 == 0) {
    if ((param_2 <= DAT_00287c58 - 1) || (999 < param_2 - DAT_00287c58)) {
      bVar7 = false;
      local_4d58 = DAT_00287c48;
      goto LAB_00186e8c;
    }
    uVar15 = 1;
  }
  else {
    if (DAT_00287ba4 != 3) {
      bVar7 = DAT_00287ba4 == 2;
      local_4d58 = DAT_00287c50;
LAB_00186e8c:
      DAT_00287c50 = local_4d58;
      memcpy(local_26e0,&DAT_0028793c,0x128);
      memcpy(local_2808,&DAT_00287a64,0x128);
      uVar3 = DAT_0010f900;
      DAT_00287ba4 = 3;
      DAT_00284688 = 0;
      if (bVar7) {
        local_4d40 = DAT_0010f900;
        memset(&local_4d38,0,0x252c);
        iVar10 = FUN_00191ff0(&local_4d40,0x2534,uVar14 & 0xffffffff);
        if (iVar10 == 0) {
LAB_00187164:
          bVar8 = false;
        }
        else {
          iVar10 = FUN_0018cb98(&local_4d40,uVar14 & 0xffffffff);
          bVar8 = false;
          if ((iVar10 != 0) && (local_4d38 != 0)) {
            bVar8 = local_4d34 == iVar12;
            if ((bVar8) && (uVar4 == 1)) {
              bVar9 = local_2808[0] == -2;
              bVar2 = false;
              uVar16 = 0;
              local_4dac = 0;
              do {
                local_25b8 = uVar3;
                memset(&local_25b0,0,0x252c);
                iVar10 = FUN_00191ff0(&local_25b8,0x2534,uVar16);
                if (iVar10 == 0) {
LAB_0018713c:
                  bVar8 = false;
                  goto LAB_00187140;
                }
                iVar10 = FUN_0018cb98(&local_25b8,uVar16);
                uVar6 = local_25a8;
                bVar8 = false;
                if (((iVar10 == 0) || (local_25b0 == 0)) || (local_25ac != iVar12))
                goto LAB_00187140;
                if ((uVar16 != 0) && (local_25a8 != local_4dac)) goto LAB_0018713c;
                uVar13 = (ulong)local_25a0;
                pcVar17 = acStack_251c;
                if (local_25a0 != 0) {
                  do {
                    iVar10 = *(int *)(pcVar17 + -0x68);
                    if (iVar10 == local_26e0[0]) {
                      iVar11 = strcmp(pcVar17 + -0x60,acStack_26d8);
                      if (((iVar11 != 0) || (iVar11 = strcmp(pcVar17,acStack_2678), iVar11 != 0)) ||
                         ((iVar11 = strcmp(pcVar17 + 0x60,acStack_2618), iVar11 != 0 ||
                          (((byte)pcVar17[-100] >> 1 & 1) == 0)))) goto LAB_0018713c;
                      bVar2 = true;
                    }
                    if (iVar10 == local_2808[0]) {
                      iVar10 = strcmp(pcVar17 + -0x60,acStack_2800);
                      if (((iVar10 != 0) || (iVar10 = strcmp(pcVar17,acStack_27a0), iVar10 != 0)) ||
                         (iVar10 = strcmp(pcVar17 + 0x60,acStack_2740), iVar10 != 0))
                      goto LAB_0018713c;
                      bVar9 = true;
                    }
                    uVar13 = uVar13 - 1;
                    pcVar17 = pcVar17 + 0x128;
                  } while (uVar13 != 0);
                }
                if ((bVar2) && (bVar9)) goto LAB_00187144;
                bVar8 = false;
                if (0xff7 < uVar16) break;
                local_4dac = uVar6;
                uVar16 = uVar16 + 8;
              } while (uVar16 < uVar6);
            }
            else {
LAB_00187140:
              if (!bVar8) goto LAB_00187164;
LAB_00187144:
              iVar12 = FUN_0019204c(uVar4,uVar15,uVar5);
              bVar8 = iVar12 != 0;
            }
          }
        }
      }
      else {
        bVar8 = true;
      }
      local_4d40 = DAT_0010f900;
      memset(&local_4d38,0,0x252c);
      iVar12 = FUN_00191ff0(&local_4d40,0x2534,uVar14 & 0xffffffff);
      if (iVar12 == 0) {
        uVar16 = 0;
      }
      else {
        iVar12 = FUN_0018cb98(&local_4d40,uVar14 & 0xffffffff);
        uVar16 = (uint)(iVar12 != 0);
      }
      uVar14 = FUN_00193f80(1,&DAT_00284688);
      if ((uVar14 & 1) != 0) goto LAB_00187240;
      if (((DAT_00284694 == param_1) && (DAT_0028469c != '\0')) &&
         ((DAT_0028469e == '\0' && ((DAT_0028469d == 't' && (local_4d58 == DAT_00287c48)))))) {
        DAT_00287ba4 = 0;
        _DAT_00287b94 = CONCAT44(uVar16,DAT_00287b94);
        DAT_00287c58 = param_2;
        if (uVar16 == 0) {
          memset(&DAT_00285408,0,0x2534);
          if (bVar7) goto LAB_001872a0;
          FUN_00183ac8(&DAT_00287c6c,0x60,0x60,&DAT_001343dc,"THEME LIST IS NOT AVAILABLE");
        }
        else {
          memcpy(&DAT_00285408,&local_4d40,0x2534);
          if (bVar7) {
LAB_001872a0:
            pcVar17 = "NOT APPLIED - REFRESH OR SELECT AGAIN";
            if (bVar8) {
              pcVar17 = "APPLIED";
            }
            FUN_00183ac8(&DAT_00287c6c,0x60,0x60,&DAT_001343dc,pcVar17);
            if (((bVar8) && (uVar4 < 7)) && ((1 << (ulong)(uVar4 & 0x1f) & 0x46U) != 0)) {
              uVar15 = 1;
              DAT_00287b90 = 0;
              DAT_00287b9c = 0;
              DAT_00287ba0 = 0;
              FUN_00194010(1,&DAT_0022d824);
              if (DAT_00287b94 != 0) {
                DAT_00287ba4 = 1;
                _DAT_00287b94 = 0;
                DAT_00287c50 = DAT_00287c48;
                FUN_00194010(1,&DAT_0022d824);
                DAT_00287bb4 = 0;
                DAT_0028469f = 0;
                DAT_00287bac = DAT_00287bac + 1;
                DAT_002846a0 = DAT_002846a0 + 1;
                goto LAB_001873cc;
              }
            }
          }
        }
        DAT_0028469f = 0;
        DAT_00287bb4 = 0;
        DAT_002846a0 = DAT_002846a0 + 1;
        uVar15 = 4;
        if (bVar8) {
          uVar15 = 1;
        }
        DAT_00287bac = DAT_00287bac + 1;
      }
      else {
        uVar15 = 4;
        DAT_00287ba4 = 0;
      }
LAB_001873cc:
      DAT_00284688 = 0;
      goto LAB_00187244;
    }
    uVar15 = 3;
  }
  DAT_00284688 = 0;
LAB_00187244:
  if (*(long *)(lVar1 + 0x28) == local_80) {
    return uVar15;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== nexus_menu_main_view @ 00189884 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void nexus_menu_main_view(int *param_1,uint param_2,int param_3)

{
  uint uVar1;
  undefined *puVar2;
  undefined *puVar3;
  ushort uVar4;
  long lVar5;
  ushort uVar6;
  undefined1 uVar7;
  bool bVar8;
  int iVar9;
  ulong uVar10;
  undefined8 uVar11;
  char *pcVar12;
  char cVar13;
  uint uVar14;
  uint uVar15;
  byte bVar16;
  uint uVar17;
  int *piVar18;
  undefined1 *puVar19;
  int *piVar20;
  uint uVar21;
  ulong uVar22;
  ulong uVar23;
  uint uVar24;
  ushort *puVar25;
  ulong uVar26;
  long lVar27;
  undefined8 *puVar28;
  undefined **ppuVar29;
  char *pcVar30;
  long lVar31;
  undefined *puVar32;
  undefined4 uVar33;
  int iVar34;
  int iVar35;
  undefined4 uVar36;
  uint local_f8;
  undefined4 uStack_ec;
  undefined8 local_d0;
  undefined8 uStack_c8;
  undefined8 uStack_c0;
  undefined1 local_b8;
  undefined1 local_b7;
  undefined6 uStack_b6;
  uint local_b0;
  undefined4 uStack_ac;
  undefined8 uStack_a8;
  undefined *local_a0;
  undefined4 local_98;
  undefined4 local_94;
  undefined8 local_90;
  undefined8 uStack_88;
  undefined8 uStack_80;
  undefined2 local_78;
  undefined6 uStack_76;
  long local_70;
  
  lVar5 = tpidr_el0;
  local_70 = *(long *)(lVar5 + 0x28);
  if (param_1 == (int *)0x0) {
    uVar11 = 4;
    goto LAB_0018c418;
  }
  uVar10 = FUN_00193f80(1,&DAT_00284688);
  iVar34 = DAT_00285414;
  iVar9 = DAT_002846a0;
  uVar7 = DAT_0028469c;
  if ((uVar10 & 1) != 0) {
    uVar11 = 3;
    goto LAB_0018c418;
  }
  uVar11 = 4;
  if (((0 < param_3) && (DAT_00284694 == param_3)) && (DAT_0028469e == '\0')) {
    uVar17 = DAT_0028469d - 0x43;
    if ((0x31 < uVar17) || ((1L << ((ulong)uVar17 & 0x3f) & 0x22006000cd001U) == 0))
    goto LAB_0018c414;
    if (DAT_0028dce8._4_4_ != DAT_0028469d) {
      param_2 = 0;
      DAT_0028dce8._4_4_ = (uint)DAT_0028469d;
    }
    switch((uint)DAT_0028469d) {
    case 0x51:
      FUN_00191920(&DAT_00284af0,0x918);
      uVar7 = DAT_0028469c;
      if (0x20 < DAT_00284af4) {
        DAT_00284af4 = 0;
      }
      uVar17 = DAT_00284af4;
      iVar9 = DAT_00284af8 + DAT_002846a0;
      if ((DAT_00284af4 + 7 & 0xff) / 5 <= param_2) {
        param_2 = 0;
      }
      *(undefined1 *)(param_1 + 2) = 0x51;
      *(undefined1 *)((long)param_1 + 9) = uVar7;
      *param_1 = iVar9;
      param_1[1] = 0x14;
      *(undefined1 *)((long)param_1 + 10) = 0;
      *(char *)((long)param_1 + 0xb) = (char)param_2;
      memset(param_1 + 3,0,0x684);
      param_2 = param_2 * 5;
      uVar10 = (ulong)param_2;
      param_1[0x142] = 0;
      param_1[0x143] = 0;
      param_1[0x44] = 0;
      param_1[0x45] = 0;
      param_1[0x42] = 0;
      param_1[0x43] = 0;
      param_1[0x5c] = 0;
      param_1[0x5d] = 0;
      param_1[0x5a] = 0;
      param_1[0x5b] = 0;
      param_1[0x60] = 0;
      param_1[0x61] = 0;
      param_1[0x5e] = 0;
      param_1[0x5f] = 0;
      param_1[0x4c] = 0;
      param_1[0x4d] = 0;
      param_1[0x4a] = 0;
      param_1[0x4b] = 0;
      param_1[0x50] = 0;
      param_1[0x51] = 0;
      param_1[0x4e] = 0;
      param_1[0x4f] = 0;
      param_1[0x54] = 0;
      param_1[0x55] = 0;
      param_1[0x52] = 0;
      param_1[0x53] = 0;
      param_1[100] = 0;
      param_1[0x65] = 0;
      param_1[0x62] = 0;
      param_1[99] = 0;
      *(undefined **)(param_1 + 6) = &DAT_00134f22;
      *(undefined **)(param_1 + 8) = &DAT_00134f22;
      param_1[0x6c] = 0;
      param_1[0x6d] = 0;
      param_1[0x6a] = 0;
      param_1[0x6b] = 0;
      param_1[0x70] = 0;
      param_1[0x71] = 0;
      param_1[0x6e] = 0;
      param_1[0x6f] = 0;
      param_1[0x74] = 0;
      param_1[0x75] = 0;
      param_1[0x72] = 0;
      param_1[0x73] = 0;
      param_1[0x7c] = 0;
      param_1[0x7d] = 0;
      param_1[0x7a] = 0;
      param_1[0x7b] = 0;
      param_1[0x80] = 0;
      param_1[0x81] = 0;
      param_1[0x7e] = 0;
      param_1[0x7f] = 0;
      param_1[0x8c] = 0;
      param_1[0x8d] = 0;
      param_1[0x8a] = 0;
      param_1[0x8b] = 0;
      param_1[0x90] = 0;
      param_1[0x91] = 0;
      param_1[0x8e] = 0;
      param_1[0x8f] = 0;
      param_1[0x94] = 0;
      param_1[0x95] = 0;
      param_1[0x92] = 0;
      param_1[0x93] = 0;
      param_1[0x84] = 0;
      param_1[0x85] = 0;
      param_1[0x82] = 0;
      param_1[0x83] = 0;
      param_1[0xc] = 0;
      param_1[0xd] = 0;
      param_1[10] = 0;
      param_1[0xb] = 0;
      param_1[0xac] = 0;
      param_1[0xad] = 0;
      param_1[0xaa] = 0;
      param_1[0xab] = 0;
      param_1[0xb0] = 0;
      param_1[0xb1] = 0;
      param_1[0xae] = 0;
      param_1[0xaf] = 0;
      param_1[0xb4] = 0;
      param_1[0xb5] = 0;
      param_1[0xb2] = 0;
      param_1[0xb3] = 0;
      param_1[0x9c] = 0;
      param_1[0x9d] = 0;
      param_1[0x9a] = 0;
      param_1[0x9b] = 0;
      param_1[0xa0] = 0;
      param_1[0xa1] = 0;
      param_1[0x9e] = 0;
      param_1[0x9f] = 0;
      param_1[0xa4] = 0;
      param_1[0xa5] = 0;
      param_1[0xa2] = 0;
      param_1[0xa3] = 0;
      param_1[0xd4] = 0;
      param_1[0xd5] = 0;
      param_1[0xd2] = 0;
      param_1[0xd3] = 0;
      param_1[0xd0] = 0;
      param_1[0xd1] = 0;
      param_1[0xce] = 0;
      param_1[0xcf] = 0;
      param_1[0xcc] = 0;
      param_1[0xcd] = 0;
      param_1[0xca] = 0;
      param_1[0xcb] = 0;
      param_1[0xc4] = 0;
      param_1[0xc5] = 0;
      param_1[0xc2] = 0;
      param_1[0xc3] = 0;
      param_1[0xc0] = 0;
      param_1[0xc1] = 0;
      param_1[0xbe] = 0;
      param_1[0xbf] = 0;
      param_1[0xbc] = 0;
      param_1[0xbd] = 0;
      param_1[0xba] = 0;
      param_1[0xbb] = 0;
      param_1[0xf4] = 0;
      param_1[0xf5] = 0;
      param_1[0xf2] = 0;
      param_1[0xf3] = 0;
      param_1[0xf0] = 0;
      param_1[0xf1] = 0;
      param_1[0xee] = 0;
      param_1[0xef] = 0;
      param_1[0xec] = 0;
      param_1[0xed] = 0;
      param_1[0xea] = 0;
      param_1[0xeb] = 0;
      param_1[0xe4] = 0;
      param_1[0xe5] = 0;
      param_1[0xe2] = 0;
      param_1[0xe3] = 0;
      param_1[0xe0] = 0;
      param_1[0xe1] = 0;
      param_1[0xde] = 0;
      param_1[0xdf] = 0;
      param_1[0xdc] = 0;
      param_1[0xdd] = 0;
      param_1[0xda] = 0;
      param_1[0xdb] = 0;
      param_1[0x114] = 0;
      param_1[0x115] = 0;
      param_1[0x112] = 0;
      param_1[0x113] = 0;
      param_1[0x110] = 0;
      param_1[0x111] = 0;
      param_1[0x10e] = 0;
      param_1[0x10f] = 0;
      param_1[0x10c] = 0;
      param_1[0x10d] = 0;
      param_1[0x10a] = 0;
      param_1[0x10b] = 0;
      param_1[0x104] = 0;
      param_1[0x105] = 0;
      param_1[0x102] = 0;
      param_1[0x103] = 0;
      param_1[0x100] = 0;
      param_1[0x101] = 0;
      param_1[0xfe] = 0;
      param_1[0xff] = 0;
      param_1[0xfc] = 0;
      param_1[0xfd] = 0;
      param_1[0xfa] = 0;
      param_1[0xfb] = 0;
      param_1[0x134] = 0;
      param_1[0x135] = 0;
      param_1[0x132] = 0;
      param_1[0x133] = 0;
      param_1[0x130] = 0;
      param_1[0x131] = 0;
      param_1[0x12e] = 0;
      param_1[0x12f] = 0;
      param_1[300] = 0;
      param_1[0x12d] = 0;
      param_1[0x12a] = 0;
      param_1[299] = 0;
      param_1[0x10] = 0;
      param_1[0x11] = 0;
      param_1[0xe] = 0;
      param_1[0xf] = 0;
      param_1[0x14] = 0;
      param_1[0x15] = 0;
      param_1[0x12] = 0;
      param_1[0x13] = 0;
      *(undefined **)(param_1 + 0x16) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x18) = &DAT_00134f22;
      param_1[0x1c] = 0;
      param_1[0x1d] = 0;
      param_1[0x1a] = 0;
      param_1[0x1b] = 0;
      param_1[0x20] = 0;
      param_1[0x21] = 0;
      param_1[0x1e] = 0;
      param_1[0x1f] = 0;
      param_1[0x24] = 0;
      param_1[0x25] = 0;
      param_1[0x22] = 0;
      param_1[0x23] = 0;
      *(undefined **)(param_1 + 0x26) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x28) = &DAT_00134f22;
      param_1[0x2c] = 0;
      param_1[0x2d] = 0;
      param_1[0x2a] = 0;
      param_1[0x2b] = 0;
      param_1[0x30] = 0;
      param_1[0x31] = 0;
      param_1[0x2e] = 0;
      param_1[0x2f] = 0;
      param_1[0x34] = 0;
      param_1[0x35] = 0;
      param_1[0x32] = 0;
      param_1[0x33] = 0;
      *(undefined **)(param_1 + 0x36) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x38) = &DAT_00134f22;
      param_1[0x40] = 0;
      param_1[0x41] = 0;
      param_1[0x3e] = 0;
      param_1[0x3f] = 0;
      param_1[0x3c] = 0;
      param_1[0x3d] = 0;
      param_1[0x3a] = 0;
      param_1[0x3b] = 0;
      *(undefined **)(param_1 + 0x46) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x48) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x56) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x58) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x66) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x68) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x76) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x78) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x86) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x88) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x96) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x98) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xa6) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xa8) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xb6) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xb8) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xc6) = &DAT_00134f22;
      *(undefined **)(param_1 + 200) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xd6) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xd8) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xe6) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xe8) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xf6) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xf8) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x106) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x108) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x116) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x118) = &DAT_00134f22;
      param_1[0x124] = 0;
      param_1[0x125] = 0;
      param_1[0x122] = 0;
      param_1[0x123] = 0;
      param_1[0x120] = 0;
      param_1[0x121] = 0;
      param_1[0x11e] = 0;
      param_1[0x11f] = 0;
      param_1[0x11c] = 0;
      param_1[0x11d] = 0;
      param_1[0x11a] = 0;
      param_1[0x11b] = 0;
      *(undefined **)(param_1 + 0x126) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x128) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x136) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x138) = &DAT_00134f22;
      param_1[0x140] = 0;
      param_1[0x141] = 0;
      param_1[0x13e] = 0;
      param_1[0x13f] = 0;
      param_1[0x13c] = 0;
      param_1[0x13d] = 0;
      param_1[0x13a] = 0;
      param_1[0x13b] = 0;
      if ((param_2 < uVar17 + 3) && (param_2 < 0xffffffec)) {
        do {
          uVar23 = uVar10 & 0xffffffff;
          iVar9 = (int)uVar10;
          uVar26 = (ulong)(uint)(iVar9 + (int)(uVar23 / 0x14) * -0x14);
          piVar18 = param_1 + uVar26 * 0x10;
          iVar34 = NEON_fmadd((float)(uVar23 / 5),0x42dc0000,0x43320000);
          piVar20 = piVar18 + 4;
          piVar20[0] = 0;
          piVar20[1] = 0;
          piVar18[6] = 0;
          piVar18[7] = 0;
          piVar18[8] = 0;
          piVar18[9] = 0;
          piVar18[0xb] = iVar34;
          iVar34 = NEON_fmadd((float)(uint)(iVar9 + (int)(uVar23 / 5) * -5),0x432f0000,0xc3dc0000);
          piVar18[0xc] = 0x18;
          piVar18[0xf] = 0;
          piVar18[0x10] = 0;
          piVar18[0xd] = 0;
          piVar18[0xe] = 0;
          *(undefined1 *)(piVar18 + 0x11) = 1;
          *(undefined8 *)((long)piVar18 + 0x45) = 0;
          piVar18[10] = iVar34;
          piVar18[0x13] = 0;
          uVar14 = DAT_00284b00;
          if (iVar9 == 2) {
            *piVar20 = 0x24003;
            *(char **)(piVar18 + 6) = "REFRESH PING";
            pcVar30 = "LOAD AVAILABLE REGIONS";
            if (DAT_00284af4 != 0) {
              pcVar30 = "MEASURE AVAILABLE REGIONS";
            }
            *(char **)(piVar18 + 8) = pcVar30;
LAB_00189e34:
            cVar13 = DAT_00284afc;
            *(char *)(piVar18 + 0x11) = DAT_00284afc;
          }
          else if (iVar9 == 1) {
            *piVar20 = 0x24002;
            *(char **)(param_1 + uVar26 * 0x10 + 6) = "AUTOMATIC";
            uVar14 = DAT_00284b00;
            pcVar30 = "SELECTED - BEST CONNECTION";
            if (-1 < (int)DAT_00284b00) {
              pcVar30 = "RESTORE GAME SERVER CHOICE";
            }
            cVar13 = '\x01';
            param_1[uVar26 * 0x10 + 0xe] = DAT_00284b00 >> 0x1f;
            *(byte *)((long)param_1 + uVar26 * 0x40 + 0x49) = (byte)(uVar14 >> 0x1f);
            *(char **)(param_1 + uVar26 * 0x10 + 8) = pcVar30;
          }
          else {
            if (iVar9 != 0) {
              uVar15 = iVar9 - 3;
              lVar31 = (ulong)uVar15 * 0x48;
              uVar24 = (&DAT_00284b08)[(ulong)uVar15 * 0x12];
              *(undefined **)(param_1 + uVar26 * 0x10 + 6) = &DAT_00284b10 + lVar31;
              *piVar20 = iVar9 + 0x2400d;
              lVar27 = (ulong)uVar15 * 0x50 + 0x28dd10;
              pcVar30 = "SELECTED";
              if (uVar24 != uVar14) {
                pcVar30 = "TAP TO SELECT";
              }
              if (*(int *)(lVar31 + 0x284b0c) < 0) {
                FUN_00183ac8(lVar27,0xffffffffffffffff,0x50,"NOT MEASURED - %s",pcVar30);
              }
              else {
                FUN_00183ac8(lVar27,0xffffffffffffffff,0x50,"%d MS - %s");
              }
              piVar18 = param_1 + uVar26 * 0x10;
              *(long *)(piVar18 + 8) = lVar27;
              *(bool *)((long)piVar18 + 0x49) = uVar24 == uVar14;
              piVar18[0xe] = (uint)(uVar24 == uVar14);
              goto LAB_00189e34;
            }
            *piVar20 = 0x24001;
            cVar13 = '\x01';
            *(undefined1 *)(param_1 + uVar26 * 0x10 + 0x12) = 1;
            *(undefined **)(param_1 + uVar26 * 0x10 + 6) = &DAT_00137c9c;
            *(char **)(param_1 + uVar26 * 0x10 + 8) = "BATTLE SERVER SETTINGS";
          }
          uVar10 = uVar10 + 1;
          param_1[uVar26 * 0x10 + 0x10] = (uint)(cVar13 == '\0');
        } while ((uVar10 < uVar17 + 3) && (uVar10 < param_2 + 0x14));
      }
      puVar32 = PTR_DAT_001a36f0;
      uVar10 = 0;
      puVar28 = (undefined8 *)(PTR_DAT_001a3720 + 8);
      do {
        uVar17 = param_1[1];
        local_94 = 0x44010000;
        param_1[1] = uVar17 + 1;
        local_98 = NEON_fmadd((float)(uVar10 & 0xffffffff),0x43120000,0xc3e40000);
        uStack_a8 = *puVar28;
        local_b0 = (uint)*(ushort *)(puVar28 + -1);
        local_a0 = &DAT_00134f22;
        FUN_00184e28(&local_90,puVar32 + (ulong)*(ushort *)(puVar28 + -1) * 0x48);
        local_78 = 1;
        uVar10 = uVar10 + 1;
        *(undefined8 *)(param_1 + (ulong)uVar17 * 0x10 + 6) = uStack_a8;
        *(ulong *)(param_1 + (ulong)uVar17 * 0x10 + 4) = CONCAT44(uStack_ac,local_b0);
        *(ulong *)(param_1 + (ulong)uVar17 * 0x10 + 10) = CONCAT44(local_94,local_98);
        *(undefined **)(param_1 + (ulong)uVar17 * 0x10 + 8) = local_a0;
        *(undefined8 *)(param_1 + (ulong)uVar17 * 0x10 + 0xe) = uStack_88;
        *(undefined8 *)(param_1 + (ulong)uVar17 * 0x10 + 0xc) = local_90;
        *(ulong *)(param_1 + (ulong)uVar17 * 0x10 + 0x12) = CONCAT62(uStack_76,1);
        *(undefined8 *)(param_1 + (ulong)uVar17 * 0x10 + 0x10) = uStack_80;
        puVar28 = puVar28 + 2;
      } while (uVar10 != 6);
      break;
    default:
      iVar9 = nexus_menu_main_count(4);
      if ((iVar9 + 4U) / 5 <= param_2) {
        param_2 = 0;
      }
      FUN_00185d10();
      iVar9 = DAT_002846a0;
      bVar16 = DAT_0028469d;
      uVar7 = DAT_0028469c;
      *(undefined1 *)((long)param_1 + 10) = 0;
      *param_1 = iVar9;
      *(byte *)(param_1 + 2) = bVar16;
      *(undefined1 *)((long)param_1 + 9) = uVar7;
      *(char *)((long)param_1 + 0xb) = (char)param_2;
      memset(param_1 + 3,0,0x684);
      uVar10 = 0;
      param_1[1] = 0x14;
      uVar17 = 0;
      param_1[0x74] = 0;
      param_1[0x75] = 0;
      param_1[0x72] = 0;
      param_1[0x73] = 0;
      param_1[0x4c] = 0;
      param_1[0x4d] = 0;
      param_1[0x4a] = 0;
      param_1[0x4b] = 0;
      param_1[0x50] = 0;
      param_1[0x51] = 0;
      param_1[0x4e] = 0;
      param_1[0x4f] = 0;
      ppuVar29 = &PTR_s_AUTO_TARGETS_ENEMIES_0019dde0;
      param_1[0x84] = 0;
      param_1[0x85] = 0;
      param_1[0x82] = 0;
      param_1[0x83] = 0;
      param_1[0x60] = 0;
      param_1[0x61] = 0;
      param_1[0x5e] = 0;
      param_1[0x5f] = 0;
      param_1[0x5c] = 0;
      param_1[0x5d] = 0;
      param_1[0x5a] = 0;
      param_1[0x5b] = 0;
      param_1[0x90] = 0;
      param_1[0x91] = 0;
      param_1[0x8e] = 0;
      param_1[0x8f] = 0;
      param_1[0x8c] = 0;
      param_1[0x8d] = 0;
      param_1[0x8a] = 0;
      param_1[0x8b] = 0;
      param_1[0x6c] = 0;
      param_1[0x6d] = 0;
      param_1[0x6a] = 0;
      param_1[0x6b] = 0;
      param_1[0x70] = 0;
      param_1[0x71] = 0;
      param_1[0x6e] = 0;
      param_1[0x6f] = 0;
      param_1[4] = 0;
      param_1[5] = 0;
      *(undefined **)(param_1 + 6) = &DAT_00134f22;
      *(undefined **)(param_1 + 8) = &DAT_00134f22;
      param_1[0x7c] = 0;
      param_1[0x7d] = 0;
      param_1[0x7a] = 0;
      param_1[0x7b] = 0;
      param_1[0x80] = 0;
      param_1[0x81] = 0;
      param_1[0x7e] = 0;
      param_1[0x7f] = 0;
      param_1[0xc] = 0;
      param_1[0xd] = 0;
      param_1[10] = 0;
      param_1[0xb] = 0;
      param_1[0x10] = 0;
      param_1[0x11] = 0;
      param_1[0xe] = 0;
      param_1[0xf] = 0;
      param_1[0x94] = 0;
      param_1[0x95] = 0;
      param_1[0x92] = 0;
      param_1[0x93] = 0;
      param_1[0x12] = 0;
      param_1[0x13] = 0;
      param_1[0x14] = 0;
      param_1[0x15] = 0;
      *(undefined **)(param_1 + 0x16) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x18) = &DAT_00134f22;
      param_1[0xa0] = 0;
      param_1[0xa1] = 0;
      param_1[0x9e] = 0;
      param_1[0x9f] = 0;
      param_1[0x9c] = 0;
      param_1[0x9d] = 0;
      param_1[0x9a] = 0;
      param_1[0x9b] = 0;
      param_1[0x1c] = 0;
      param_1[0x1d] = 0;
      param_1[0x1a] = 0;
      param_1[0x1b] = 0;
      param_1[0x20] = 0;
      param_1[0x21] = 0;
      param_1[0x1e] = 0;
      param_1[0x1f] = 0;
      param_1[0xb0] = 0;
      param_1[0xb1] = 0;
      param_1[0xae] = 0;
      param_1[0xaf] = 0;
      param_1[0xac] = 0;
      param_1[0xad] = 0;
      param_1[0xaa] = 0;
      param_1[0xab] = 0;
      param_1[0x22] = 0;
      param_1[0x23] = 0;
      param_1[0x24] = 0;
      param_1[0x25] = 0;
      *(undefined **)(param_1 + 0x26) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x28) = &DAT_00134f22;
      param_1[0xc0] = 0;
      param_1[0xc1] = 0;
      param_1[0xbe] = 0;
      param_1[0xbf] = 0;
      param_1[0xbc] = 0;
      param_1[0xbd] = 0;
      param_1[0xba] = 0;
      param_1[0xbb] = 0;
      param_1[0x2c] = 0;
      param_1[0x2d] = 0;
      param_1[0x2a] = 0;
      param_1[0x2b] = 0;
      param_1[0x30] = 0;
      param_1[0x31] = 0;
      param_1[0x2e] = 0;
      param_1[0x2f] = 0;
      param_1[0xd0] = 0;
      param_1[0xd1] = 0;
      param_1[0xce] = 0;
      param_1[0xcf] = 0;
      param_1[0xcc] = 0;
      param_1[0xcd] = 0;
      param_1[0xca] = 0;
      param_1[0xcb] = 0;
      param_1[0x32] = 0;
      param_1[0x33] = 0;
      param_1[0x34] = 0;
      param_1[0x35] = 0;
      *(undefined **)(param_1 + 0x36) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x38) = &DAT_00134f22;
      param_1[0xe0] = 0;
      param_1[0xe1] = 0;
      param_1[0xde] = 0;
      param_1[0xdf] = 0;
      param_1[0xdc] = 0;
      param_1[0xdd] = 0;
      param_1[0xda] = 0;
      param_1[0xdb] = 0;
      param_1[0x3c] = 0;
      param_1[0x3d] = 0;
      param_1[0x3a] = 0;
      param_1[0x3b] = 0;
      param_1[0x40] = 0;
      param_1[0x41] = 0;
      param_1[0x3e] = 0;
      param_1[0x3f] = 0;
      param_1[0xf0] = 0;
      param_1[0xf1] = 0;
      param_1[0xee] = 0;
      param_1[0xef] = 0;
      param_1[0xec] = 0;
      param_1[0xed] = 0;
      param_1[0xea] = 0;
      param_1[0xeb] = 0;
      param_1[0x42] = 0;
      param_1[0x43] = 0;
      param_1[0x44] = 0;
      param_1[0x45] = 0;
      *(undefined **)(param_1 + 0x46) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x48) = &DAT_00134f22;
      param_1[0x100] = 0;
      param_1[0x101] = 0;
      param_1[0xfe] = 0;
      param_1[0xff] = 0;
      param_1[0xfc] = 0;
      param_1[0xfd] = 0;
      param_1[0xfa] = 0;
      param_1[0xfb] = 0;
      param_1[0x52] = 0;
      param_1[0x53] = 0;
      param_1[0x54] = 0;
      param_1[0x55] = 0;
      *(undefined **)(param_1 + 0x56) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x58) = &DAT_00134f22;
      param_1[0x110] = 0;
      param_1[0x111] = 0;
      param_1[0x10e] = 0;
      param_1[0x10f] = 0;
      param_1[0x10c] = 0;
      param_1[0x10d] = 0;
      param_1[0x10a] = 0;
      param_1[0x10b] = 0;
      param_1[0x62] = 0;
      param_1[99] = 0;
      param_1[100] = 0;
      param_1[0x65] = 0;
      *(undefined **)(param_1 + 0x66) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x68) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x76) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x78) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x86) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x88) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x96) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x98) = &DAT_00134f22;
      param_1[0xa2] = 0;
      param_1[0xa3] = 0;
      param_1[0xa4] = 0;
      param_1[0xa5] = 0;
      *(undefined **)(param_1 + 0xa6) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xa8) = &DAT_00134f22;
      param_1[0xb2] = 0;
      param_1[0xb3] = 0;
      param_1[0xb4] = 0;
      param_1[0xb5] = 0;
      *(undefined **)(param_1 + 0xb6) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xb8) = &DAT_00134f22;
      param_1[0xc2] = 0;
      param_1[0xc3] = 0;
      param_1[0xc4] = 0;
      param_1[0xc5] = 0;
      *(undefined **)(param_1 + 0xc6) = &DAT_00134f22;
      *(undefined **)(param_1 + 200) = &DAT_00134f22;
      param_1[0xd2] = 0;
      param_1[0xd3] = 0;
      param_1[0xd4] = 0;
      param_1[0xd5] = 0;
      *(undefined **)(param_1 + 0xd6) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xd8) = &DAT_00134f22;
      param_1[0xe2] = 0;
      param_1[0xe3] = 0;
      param_1[0xe4] = 0;
      param_1[0xe5] = 0;
      *(undefined **)(param_1 + 0xe6) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xe8) = &DAT_00134f22;
      param_1[0xf2] = 0;
      param_1[0xf3] = 0;
      param_1[0xf4] = 0;
      param_1[0xf5] = 0;
      *(undefined **)(param_1 + 0xf6) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xf8) = &DAT_00134f22;
      param_1[0x102] = 0;
      param_1[0x103] = 0;
      param_1[0x104] = 0;
      param_1[0x105] = 0;
      *(undefined **)(param_1 + 0x106) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x108) = &DAT_00134f22;
      param_1[0x112] = 0;
      param_1[0x113] = 0;
      param_1[0x114] = 0;
      param_1[0x115] = 0;
      *(undefined **)(param_1 + 0x116) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x118) = &DAT_00134f22;
      param_1[0x122] = 0;
      param_1[0x123] = 0;
      param_1[0x120] = 0;
      param_1[0x121] = 0;
      param_1[0x11e] = 0;
      param_1[0x11f] = 0;
      param_1[0x11c] = 0;
      param_1[0x11d] = 0;
      param_1[0x11a] = 0;
      param_1[0x11b] = 0;
      param_1[0x124] = 0;
      param_1[0x125] = 0;
      *(undefined **)(param_1 + 0x126) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x128) = &DAT_00134f22;
      param_1[0x132] = 0;
      param_1[0x133] = 0;
      param_1[0x130] = 0;
      param_1[0x131] = 0;
      param_1[0x12e] = 0;
      param_1[0x12f] = 0;
      param_1[300] = 0;
      param_1[0x12d] = 0;
      param_1[0x12a] = 0;
      param_1[299] = 0;
      param_1[0x134] = 0;
      param_1[0x135] = 0;
      *(undefined **)(param_1 + 0x136) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x138) = &DAT_00134f22;
      param_1[0x140] = 0;
      param_1[0x141] = 0;
      param_1[0x13e] = 0;
      param_1[0x13f] = 0;
      param_1[0x13c] = 0;
      param_1[0x13d] = 0;
      param_1[0x13a] = 0;
      param_1[0x13b] = 0;
      param_1[0x142] = 0;
      param_1[0x143] = 0;
      do {
        if ((((DAT_0028469d == 0x52) ||
             (uVar14 = uVar17, *(uint *)(ppuVar29 + 1) == (uint)DAT_0028469d)) &&
            (uVar14 = uVar17 + 1, param_2 * 5 <= uVar17)) && (uVar17 < param_2 * 5 + 0x14)) {
          uVar15 = *(uint *)(ppuVar29 + -2);
          uVar23 = (ulong)(uVar17 % 0x14);
          puVar32 = ppuVar29[-1];
          puVar3 = *ppuVar29;
          param_1[uVar23 * 0x10 + 4] = uVar15;
          iVar34 = NEON_fmadd((float)((ulong)uVar17 / 5),0x42dc0000,0x43320000);
          *(undefined **)(param_1 + uVar23 * 0x10 + 6) = puVar32;
          *(undefined **)(param_1 + uVar23 * 0x10 + 8) = puVar3;
          iVar9 = NEON_fmadd((float)(uVar17 % 5),0x432f0000,0xc3dc0000);
          (param_1 + uVar23 * 0x10 + 0xe)[0] = 0;
          (param_1 + uVar23 * 0x10 + 0xe)[1] = 0;
          (param_1 + uVar23 * 0x10 + 0x10)[0] = 0;
          (param_1 + uVar23 * 0x10 + 0x10)[1] = 0;
          (param_1 + uVar23 * 0x10 + 0xc)[0] = 0;
          (param_1 + uVar23 * 0x10 + 0xc)[1] = 0;
          *(undefined2 *)(param_1 + uVar23 * 0x10 + 0x12) = 0;
          param_1[uVar23 * 0x10 + 0xb] = iVar34;
          param_1[uVar23 * 0x10 + 10] = iVar9;
          puVar32 = PTR_DAT_001a36f0;
          if (uVar10 < 0x1d) {
            if (uVar15 == 0x20002) {
              *(undefined1 *)(param_1 + uVar23 * 0x10 + 0x12) = 5;
              param_1[uVar23 * 0x10 + 0xc] = 0x18;
            }
            else {
              *(undefined *)(param_1 + uVar23 * 0x10 + 0x12) =
                   PTR_DAT_001a36f0[(ulong)uVar15 * 0x48 + 0x44];
              if (uVar15 == 10) {
                uVar17 = 0x1a;
              }
              else if (uVar15 == 0xd) {
                uVar17 = 0x1d;
              }
              else {
                uVar17 = 0x20;
                if (uVar15 != 0x6d) {
                  uVar17 = uVar15;
                }
              }
              FUN_00184e28(&local_b0,puVar32 + (ulong)uVar17 * 0x48);
              *(undefined8 *)(param_1 + uVar23 * 0x10 + 0xe) = uStack_a8;
              *(ulong *)(param_1 + uVar23 * 0x10 + 0xc) = CONCAT44(uStack_ac,local_b0);
              *(undefined **)(param_1 + uVar23 * 0x10 + 0x10) = local_a0;
              if (uVar17 != uVar15) {
                *(undefined1 *)(param_1 + uVar23 * 0x10 + 0x11) = 1;
              }
            }
          }
          else {
            uVar26 = (ulong)(uVar15 - 0x21000);
            bVar16 = ((byte)((int)(0x29811000000000 >> ((ulong)(uVar15 + 0x29000) & 0x3f)) << 1) ^
                     0xff) & 2;
            if ((0x42020ff0000000U >> ((ulong)(uVar15 + 0x29000) & 0x3f) & 1) != 0) {
              bVar16 = 4;
            }
            *(byte *)(param_1 + uVar23 * 0x10 + 0x12) = bVar16;
            FUN_00192340(ppuVar29[2],param_1 + uVar23 * 0x10 + 0xc);
            if ((char)param_1[uVar23 * 0x10 + 0x11] == '\0') {
              puVar32 = (undefined *)(uVar26 * 0x50 + 0x28e710);
              FUN_00183ac8(puVar32,0xffffffffffffffff,0x50,"UNAVAILABLE - %s",puVar3);
            }
            else {
              pcVar30 = (&PTR_s_DisablePinAnimation_0019d588)[uVar26 * 3];
              iVar9 = strcmp(pcVar30,"FPSLimit");
              if (iVar9 == 0) {
                puVar32 = (undefined *)(uVar26 * 0x50 + 0x28e710);
                if (param_1[uVar23 * 0x10 + 0xf] == 0) {
                  FUN_00183ac8(puVar32,0xffffffffffffffff,0x50,"UNLIMITED - TAP TO CONFIGURE");
                }
                else {
                  FUN_00183ac8(puVar32,0xffffffffffffffff,0x50,"%d FPS - TAP TO CONFIGURE");
                }
              }
              else {
                iVar9 = strcmp(pcVar30,"Font");
                uVar17 = param_1[uVar23 * 0x10 + 0xf];
                uVar22 = (ulong)uVar17;
                if (iVar9 == 0) {
                  pcVar30 = "UNKNOWN";
                  if (uVar17 < 6) {
                    pcVar30 = &DAT_001407e4 + *(int *)(&DAT_001407e4 + uVar22 * 4);
                  }
                  puVar32 = (undefined *)(uVar26 * 0x50 + 0x28e710);
                  FUN_00183ac8(puVar32,0xffffffffffffffff,0x50,"%s - TAP TO CONFIGURE",pcVar30);
                }
                else {
                  iVar9 = strcmp(pcVar30,"CameraZoom");
                  if (iVar9 == 0) {
                    if (uVar17 < 9) {
                      FUN_00183ac8((double)*(float *)(&DAT_001407fc + uVar22 * 4),&DAT_0028f840,
                                   0x280,0x50,"%.2fx - TAP TO CHANGE");
                      lVar31 = 0;
                      goto LAB_0018a684;
                    }
                  }
                  else {
                    iVar9 = strcmp(pcVar30,"CameraMode");
                    if (iVar9 == 0) {
                      if (uVar17 < 4) {
                        FUN_00183ac8(&DAT_0028f890,0x230,0x50,"%s - TAP TO CHANGE",
                                     &DAT_00140820 + *(int *)(&DAT_00140820 + uVar22 * 4));
                        lVar31 = 1;
                        goto LAB_0018a684;
                      }
                    }
                    else {
                      iVar9 = strcmp(pcVar30,"CameraX");
                      if (iVar9 == 0) {
                        lVar31 = 2;
                      }
                      else {
                        iVar9 = strcmp(pcVar30,"CameraY");
                        if (iVar9 == 0) {
                          lVar31 = 3;
                        }
                        else {
                          iVar9 = strcmp(pcVar30,"CameraZ");
                          if (iVar9 == 0) {
                            lVar31 = 4;
                          }
                          else {
                            iVar9 = strcmp(pcVar30,"CameraTargetX");
                            if (iVar9 == 0) {
                              lVar31 = 5;
                            }
                            else {
                              iVar9 = strcmp(pcVar30,"CameraTargetY");
                              if (iVar9 == 0) {
                                lVar31 = 6;
                              }
                              else {
                                iVar9 = strcmp(pcVar30,"CameraTilt");
                                if (iVar9 != 0) goto LAB_0018a5f0;
                                lVar31 = 7;
                              }
                            }
                          }
                        }
                      }
                      if (uVar17 < 0x51) {
                        iVar9 = 0xfa;
                        if ((uVar17 & 1) == 0) {
                          iVar9 = -0xfa;
                        }
                        FUN_00183ac8(&DAT_0028f840 + lVar31 * 0x50,0xffffffffffffffff,0x50,
                                     "%+d UNITS - STEP 250",((uVar17 & 1) + uVar17 >> 1) * iVar9);
LAB_0018a684:
                        puVar32 = &DAT_0028f840 + lVar31 * 0x50;
                        goto LAB_0018a688;
                      }
                    }
                  }
LAB_0018a5f0:
                  puVar32 = puVar3;
                  if ((0x147ce00fffffffU >> (uVar26 & 0x3f) & 1) != 0) {
                    puVar32 = (undefined *)(uVar26 * 0x50 + 0x28e710);
                    puVar2 = &UNK_0013280a;
                    if (uVar17 != 0) {
                      puVar2 = &DAT_001346a5;
                    }
                    FUN_00183ac8(puVar32,0xffffffffffffffff,0x50,"%s - %s",puVar2,puVar3);
                  }
                }
              }
            }
LAB_0018a688:
            *(undefined **)(param_1 + uVar23 * 0x10 + 8) = puVar32;
          }
        }
        uVar17 = uVar14;
        puVar32 = PTR_DAT_001a36f0;
        uVar10 = uVar10 + 1;
        ppuVar29 = ppuVar29 + 5;
      } while (uVar10 != 0x4a);
      uVar10 = 0;
      puVar25 = (ushort *)PTR_DAT_001a3720;
      do {
        uVar17 = param_1[1];
        param_1[1] = uVar17 + 1;
        uVar33 = NEON_fmadd((float)(uVar10 & 0xffffffff),0x43120000,0xc3e40000);
        uVar4 = *puVar25;
        uVar11 = *(undefined8 *)(puVar25 + 4);
        FUN_00184e28(&local_d0,puVar32 + (ulong)uVar4 * 0x48);
        bVar16 = DAT_0028469d;
        uVar6 = puVar25[1];
        uVar10 = uVar10 + 1;
        puVar25 = puVar25 + 8;
        local_b8 = 1;
        *(undefined8 *)(param_1 + (ulong)uVar17 * 0x10 + 6) = uVar11;
        *(ulong *)(param_1 + (ulong)uVar17 * 0x10 + 4) = CONCAT44(uStack_ec,(uint)uVar4);
        *(ulong *)(param_1 + (ulong)uVar17 * 0x10 + 10) = CONCAT44(0x44010000,uVar33);
        *(undefined **)(param_1 + (ulong)uVar17 * 0x10 + 8) = &DAT_00134f22;
        local_b7 = bVar16 == (byte)uVar6;
        *(undefined8 *)(param_1 + (ulong)uVar17 * 0x10 + 0xe) = uStack_c8;
        *(undefined8 *)(param_1 + (ulong)uVar17 * 0x10 + 0xc) = local_d0;
        *(ulong *)(param_1 + (ulong)uVar17 * 0x10 + 0x12) = CONCAT62(uStack_b6,CONCAT11(local_b7,1))
        ;
        *(undefined8 *)(param_1 + (ulong)uVar17 * 0x10 + 0x10) = uStack_c0;
      } while (uVar10 != 6);
      goto LAB_0018c408;
    case 100:
      uVar17 = 0;
      if (param_2 < 9) {
        uVar17 = param_2;
      }
      *param_1 = DAT_002846a0;
      param_1[1] = 0x14;
      *(undefined1 *)(param_1 + 2) = 100;
      *(undefined1 *)((long)param_1 + 9) = uVar7;
      *(undefined1 *)((long)param_1 + 10) = 0;
      *(char *)((long)param_1 + 0xb) = (char)uVar17;
      memset(param_1 + 3,0,0x684);
      uVar10 = (ulong)(uVar17 * 5);
      uRam000000000028a40c = 0;
      _DAT_0028a404 = 0;
      uRam000000000028a41c = 0;
      _DAT_0028a414 = 0;
      param_1[4] = 0;
      param_1[5] = 0;
      *(undefined **)(param_1 + 6) = &DAT_00134f22;
      param_1[0x4c] = 0;
      param_1[0x4d] = 0;
      param_1[0x4a] = 0;
      param_1[0x4b] = 0;
      param_1[0x50] = 0;
      param_1[0x51] = 0;
      param_1[0x4e] = 0;
      param_1[0x4f] = 0;
      param_1[0x54] = 0;
      param_1[0x55] = 0;
      param_1[0x52] = 0;
      param_1[0x53] = 0;
      param_1[0x5c] = 0;
      param_1[0x5d] = 0;
      param_1[0x5a] = 0;
      param_1[0x5b] = 0;
      param_1[0x60] = 0;
      param_1[0x61] = 0;
      param_1[0x5e] = 0;
      param_1[0x5f] = 0;
      param_1[0x6c] = 0;
      param_1[0x6d] = 0;
      param_1[0x6a] = 0;
      param_1[0x6b] = 0;
      param_1[0x70] = 0;
      param_1[0x71] = 0;
      param_1[0x6e] = 0;
      param_1[0x6f] = 0;
      param_1[0x74] = 0;
      param_1[0x75] = 0;
      param_1[0x72] = 0;
      param_1[0x73] = 0;
      param_1[100] = 0;
      param_1[0x65] = 0;
      param_1[0x62] = 0;
      param_1[99] = 0;
      *(undefined **)(param_1 + 8) = &DAT_00134f22;
      param_1[0x8c] = 0;
      param_1[0x8d] = 0;
      param_1[0x8a] = 0;
      param_1[0x8b] = 0;
      param_1[0x90] = 0;
      param_1[0x91] = 0;
      param_1[0x8e] = 0;
      param_1[0x8f] = 0;
      param_1[0x94] = 0;
      param_1[0x95] = 0;
      param_1[0x92] = 0;
      param_1[0x93] = 0;
      param_1[0x7c] = 0;
      param_1[0x7d] = 0;
      param_1[0x7a] = 0;
      param_1[0x7b] = 0;
      param_1[0x80] = 0;
      param_1[0x81] = 0;
      param_1[0x7e] = 0;
      param_1[0x7f] = 0;
      param_1[0xb4] = 0;
      param_1[0xb5] = 0;
      param_1[0xb2] = 0;
      param_1[0xb3] = 0;
      param_1[0xb0] = 0;
      param_1[0xb1] = 0;
      param_1[0xae] = 0;
      param_1[0xaf] = 0;
      param_1[0xac] = 0;
      param_1[0xad] = 0;
      param_1[0xaa] = 0;
      param_1[0xab] = 0;
      param_1[0x84] = 0;
      param_1[0x85] = 0;
      param_1[0x82] = 0;
      param_1[0x83] = 0;
      param_1[0xc] = 0;
      param_1[0xd] = 0;
      param_1[10] = 0;
      param_1[0xb] = 0;
      param_1[0xd4] = 0;
      param_1[0xd5] = 0;
      param_1[0xd2] = 0;
      param_1[0xd3] = 0;
      param_1[0xd0] = 0;
      param_1[0xd1] = 0;
      param_1[0xce] = 0;
      param_1[0xcf] = 0;
      param_1[0xcc] = 0;
      param_1[0xcd] = 0;
      param_1[0xca] = 0;
      param_1[0xcb] = 0;
      param_1[0x9c] = 0;
      param_1[0x9d] = 0;
      param_1[0x9a] = 0;
      param_1[0x9b] = 0;
      param_1[0xa0] = 0;
      param_1[0xa1] = 0;
      param_1[0x9e] = 0;
      param_1[0x9f] = 0;
      param_1[0xe4] = 0;
      param_1[0xe5] = 0;
      param_1[0xe2] = 0;
      param_1[0xe3] = 0;
      param_1[0xe0] = 0;
      param_1[0xe1] = 0;
      param_1[0xde] = 0;
      param_1[0xdf] = 0;
      param_1[0xdc] = 0;
      param_1[0xdd] = 0;
      param_1[0xda] = 0;
      param_1[0xdb] = 0;
      param_1[0xa4] = 0;
      param_1[0xa5] = 0;
      param_1[0xa2] = 0;
      param_1[0xa3] = 0;
      param_1[0x10] = 0;
      param_1[0x11] = 0;
      param_1[0xe] = 0;
      param_1[0xf] = 0;
      param_1[0xf4] = 0;
      param_1[0xf5] = 0;
      param_1[0xf2] = 0;
      param_1[0xf3] = 0;
      param_1[0xf0] = 0;
      param_1[0xf1] = 0;
      param_1[0xee] = 0;
      param_1[0xef] = 0;
      param_1[0xec] = 0;
      param_1[0xed] = 0;
      param_1[0xea] = 0;
      param_1[0xeb] = 0;
      param_1[0xc4] = 0;
      param_1[0xc5] = 0;
      param_1[0xc2] = 0;
      param_1[0xc3] = 0;
      param_1[0xc0] = 0;
      param_1[0xc1] = 0;
      param_1[0xbe] = 0;
      param_1[0xbf] = 0;
      param_1[0xbc] = 0;
      param_1[0xbd] = 0;
      param_1[0xba] = 0;
      param_1[0xbb] = 0;
      param_1[0x104] = 0;
      param_1[0x105] = 0;
      param_1[0x102] = 0;
      param_1[0x103] = 0;
      param_1[0x100] = 0;
      param_1[0x101] = 0;
      param_1[0xfe] = 0;
      param_1[0xff] = 0;
      param_1[0xfc] = 0;
      param_1[0xfd] = 0;
      param_1[0xfa] = 0;
      param_1[0xfb] = 0;
      param_1[0x124] = 0;
      param_1[0x125] = 0;
      param_1[0x122] = 0;
      param_1[0x123] = 0;
      param_1[0x120] = 0;
      param_1[0x121] = 0;
      param_1[0x11e] = 0;
      param_1[0x11f] = 0;
      param_1[0x114] = 0;
      param_1[0x115] = 0;
      param_1[0x112] = 0;
      param_1[0x113] = 0;
      param_1[0x110] = 0;
      param_1[0x111] = 0;
      param_1[0x10e] = 0;
      param_1[0x10f] = 0;
      param_1[0x10c] = 0;
      param_1[0x10d] = 0;
      param_1[0x10a] = 0;
      param_1[0x10b] = 0;
      param_1[0x11c] = 0;
      param_1[0x11d] = 0;
      param_1[0x11a] = 0;
      param_1[0x11b] = 0;
      param_1[0x14] = 0;
      param_1[0x15] = 0;
      param_1[0x12] = 0;
      param_1[0x13] = 0;
      *(undefined **)(param_1 + 0x16) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x18) = &DAT_00134f22;
      param_1[0x1c] = 0;
      param_1[0x1d] = 0;
      param_1[0x1a] = 0;
      param_1[0x1b] = 0;
      param_1[0x20] = 0;
      param_1[0x21] = 0;
      param_1[0x1e] = 0;
      param_1[0x1f] = 0;
      param_1[0x24] = 0;
      param_1[0x25] = 0;
      param_1[0x22] = 0;
      param_1[0x23] = 0;
      *(undefined **)(param_1 + 0x26) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x28) = &DAT_00134f22;
      param_1[0x2c] = 0;
      param_1[0x2d] = 0;
      param_1[0x2a] = 0;
      param_1[0x2b] = 0;
      param_1[0x30] = 0;
      param_1[0x31] = 0;
      param_1[0x2e] = 0;
      param_1[0x2f] = 0;
      param_1[0x34] = 0;
      param_1[0x35] = 0;
      param_1[0x32] = 0;
      param_1[0x33] = 0;
      *(undefined **)(param_1 + 0x36) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x38) = &DAT_00134f22;
      param_1[0x3c] = 0;
      param_1[0x3d] = 0;
      param_1[0x3a] = 0;
      param_1[0x3b] = 0;
      param_1[0x40] = 0;
      param_1[0x41] = 0;
      param_1[0x3e] = 0;
      param_1[0x3f] = 0;
      param_1[0x44] = 0;
      param_1[0x45] = 0;
      param_1[0x42] = 0;
      param_1[0x43] = 0;
      *(undefined **)(param_1 + 0x46) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x48) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x56) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x58) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x66) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x68) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x76) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x78) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x86) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x88) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x96) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x98) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xa6) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xa8) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xb6) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xb8) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xc6) = &DAT_00134f22;
      *(undefined **)(param_1 + 200) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xd6) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xd8) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xe6) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xe8) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xf6) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xf8) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x106) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x108) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x116) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x118) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x126) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x128) = &DAT_00134f22;
      param_1[0x134] = 0;
      param_1[0x135] = 0;
      param_1[0x132] = 0;
      param_1[0x133] = 0;
      param_1[0x130] = 0;
      param_1[0x131] = 0;
      param_1[0x12e] = 0;
      param_1[0x12f] = 0;
      param_1[300] = 0;
      param_1[0x12d] = 0;
      param_1[0x12a] = 0;
      param_1[299] = 0;
      *(undefined **)(param_1 + 0x136) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x138) = &DAT_00134f22;
      param_1[0x142] = 0;
      param_1[0x143] = 0;
      param_1[0x140] = 0;
      param_1[0x141] = 0;
      param_1[0x13e] = 0;
      param_1[0x13f] = 0;
      param_1[0x13c] = 0;
      param_1[0x13d] = 0;
      param_1[0x13a] = 0;
      param_1[0x13b] = 0;
      do {
        uVar15 = (uint)uVar10;
        uVar14 = (uVar15 & 0xff) / 5;
        uVar23 = (ulong)(uVar15 + ((uVar15 & 0xff) / 0x14) * -0x14) & 0xff;
        piVar18 = param_1 + uVar23 * 0x10 + 4;
        piVar18[0] = 0;
        piVar18[1] = 0;
        (param_1 + uVar23 * 0x10 + 6)[0] = 0;
        (param_1 + uVar23 * 0x10 + 6)[1] = 0;
        iVar9 = NEON_fmadd((float)(uVar15 + uVar14 * -5 & 0xff),0x432f0000,0xc3dc0000);
        iVar34 = NEON_fmadd((float)uVar14,0x42dc0000,0x43320000);
        (param_1 + uVar23 * 0x10 + 8)[0] = 0;
        (param_1 + uVar23 * 0x10 + 8)[1] = 0;
        param_1[uVar23 * 0x10 + 0xc] = 0x18;
        (param_1 + uVar23 * 0x10 + 0xf)[0] = 0;
        (param_1 + uVar23 * 0x10 + 0xf)[1] = 0;
        (param_1 + uVar23 * 0x10 + 0xd)[0] = 0;
        (param_1 + uVar23 * 0x10 + 0xd)[1] = 0;
        *(undefined1 *)(param_1 + uVar23 * 0x10 + 0x11) = 1;
        *(undefined8 *)((long)param_1 + uVar23 * 0x40 + 0x45) = 0;
        param_1[uVar23 * 0x10 + 10] = iVar9;
        param_1[uVar23 * 0x10 + 0xb] = iVar34;
        param_1[uVar23 * 0x10 + 0x13] = 0;
        if (uVar10 == 0) {
          *piVar18 = 0x27000;
          *(undefined **)(param_1 + uVar23 * 0x10 + 6) = &DAT_00137c9c;
          pcVar30 = "BSD DEBUG";
          if (DAT_0028a454 != '\0') {
            pcVar30 = &DAT_0028a454;
          }
          bVar8 = true;
          *(char **)(param_1 + uVar23 * 0x10 + 8) = pcVar30;
          *(undefined1 *)(param_1 + uVar23 * 0x10 + 0x12) = 1;
        }
        else {
          uVar22 = uVar10 - 1;
          uVar26 = uVar22 & 0xffffffff;
          *piVar18 = uVar15 + 0x2701f;
          *(undefined **)(param_1 + uVar23 * 0x10 + 6) = (&PTR_s_ABOUT_SCREEN_0019cf58)[uVar26 * 5];
          iVar9 = DAT_0028a42c;
          if ((((DAT_0028a428 == 0) || (DAT_0028a42c != 0)) ||
              (uVar14 = (&DAT_0019cf48)[uVar26 * 10], 0xff < uVar14)) || (DAT_0028a3e0 <= uVar14)) {
            *(undefined1 *)(param_1 + uVar23 * 0x10 + 0x11) = 0;
LAB_0018b198:
            pcVar30 = "AVAILABLE FROM THE HOME SCREEN";
            if (iVar9 != 0) {
              pcVar30 = "WAITING FOR INPUT OR SCREEN";
            }
            *(char **)(param_1 + uVar23 * 0x10 + 8) = pcVar30;
            if (DAT_0028a438 == 0) {
              bVar8 = false;
            }
            else {
              pcVar30 = (&PTR_s_ABOUT_SCREEN_0019cf50)[uVar26 * 5];
              iVar9 = strcmp(pcVar30,"GFX_QUALITY_CYCLE");
              if ((iVar9 == 0) || (iVar9 = strcmp(pcVar30,"MEM_QUALITY_CYCLE"), iVar9 == 0)) {
                *(char **)(param_1 + uVar23 * 0x10 + 8) = "DISABLE MAX OPTIMIZATION FIRST";
              }
              bVar8 = false;
            }
          }
          else {
            uVar14 = *(uint *)(&DAT_0028a3e4 + ((ulong)(uVar14 >> 3) & 0x1ffffffc)) &
                     1 << (ulong)(uVar14 & 0x1f);
            *(bool *)(param_1 + uVar23 * 0x10 + 0x11) = uVar14 != 0;
            if (uVar14 == 0) goto LAB_0018b198;
            bVar8 = true;
            *(undefined **)(param_1 + uVar23 * 0x10 + 8) =
                 (&PTR_s_GAME_INFORMATION_0019cf60)[uVar26 * 5];
          }
          uVar26 = uVar22 >> 5 & 0x7ffffff;
          (&DAT_0028a404)[uVar26] = (&DAT_0028a404)[uVar26] | 1 << (ulong)((uint)uVar22 & 0x1f);
        }
        param_1[uVar23 * 0x10 + 0x10] = (uint)!bVar8;
        puVar32 = PTR_DAT_001a36f0;
      } while ((uVar10 < 0x28) && (uVar10 = uVar10 + 1, uVar10 < uVar17 * 5 + 0x14));
      uVar10 = 0;
      puVar28 = (undefined8 *)(PTR_DAT_001a3720 + 8);
      do {
        uVar17 = param_1[1];
        local_94 = 0x44010000;
        param_1[1] = uVar17 + 1;
        local_98 = NEON_fmadd((float)(uVar10 & 0xffffffff),0x43120000,0xc3e40000);
        uStack_a8 = *puVar28;
        local_b0 = (uint)*(ushort *)(puVar28 + -1);
        local_a0 = &DAT_00134f22;
        FUN_00184e28(&local_90,puVar32 + (ulong)*(ushort *)(puVar28 + -1) * 0x48);
        local_78 = 1;
        uVar10 = uVar10 + 1;
        *(undefined8 *)(param_1 + (ulong)uVar17 * 0x10 + 6) = uStack_a8;
        *(ulong *)(param_1 + (ulong)uVar17 * 0x10 + 4) = CONCAT44(uStack_ac,local_b0);
        *(ulong *)(param_1 + (ulong)uVar17 * 0x10 + 10) = CONCAT44(local_94,local_98);
        *(undefined **)(param_1 + (ulong)uVar17 * 0x10 + 8) = local_a0;
        *(undefined8 *)(param_1 + (ulong)uVar17 * 0x10 + 0xe) = uStack_88;
        *(undefined8 *)(param_1 + (ulong)uVar17 * 0x10 + 0xc) = local_90;
        *(ulong *)(param_1 + (ulong)uVar17 * 0x10 + 0x12) = CONCAT62(uStack_76,1);
        *(undefined8 *)(param_1 + (ulong)uVar17 * 0x10 + 0x10) = uStack_80;
        puVar28 = puVar28 + 2;
      } while (uVar10 != 6);
      break;
    case 0x65:
      uVar17 = 2;
      if (DAT_0028cc30 != 4) {
        uVar17 = 0x15;
      }
      if (uVar17 * 0x34 + 0xd0 >> 8 <= param_2) {
        param_2 = 0;
      }
      *(undefined1 *)(param_1 + 2) = 0x65;
      *param_1 = iVar9;
      param_1[1] = 0x14;
      *(undefined1 *)((long)param_1 + 9) = uVar7;
      *(undefined1 *)((long)param_1 + 10) = 0;
      *(char *)((long)param_1 + 0xb) = (char)param_2;
      memset(param_1 + 3,0,0x684);
      DAT_0028cc3c = 0;
      param_2 = param_2 * 5;
      uVar10 = (ulong)param_2;
      param_1[4] = 0;
      param_1[5] = 0;
      *(undefined **)(param_1 + 6) = &DAT_00134f22;
      *(undefined **)(param_1 + 8) = &DAT_00134f22;
      param_1[0x14] = 0;
      param_1[0x15] = 0;
      param_1[0x12] = 0;
      param_1[0x13] = 0;
      param_1[0x4c] = 0;
      param_1[0x4d] = 0;
      param_1[0x4a] = 0;
      param_1[0x4b] = 0;
      param_1[0x50] = 0;
      param_1[0x51] = 0;
      param_1[0x4e] = 0;
      param_1[0x4f] = 0;
      param_1[0x54] = 0;
      param_1[0x55] = 0;
      param_1[0x52] = 0;
      param_1[0x53] = 0;
      param_1[0x5c] = 0;
      param_1[0x5d] = 0;
      param_1[0x5a] = 0;
      param_1[0x5b] = 0;
      param_1[0x60] = 0;
      param_1[0x61] = 0;
      param_1[0x5e] = 0;
      param_1[0x5f] = 0;
      param_1[0x74] = 0;
      param_1[0x75] = 0;
      param_1[0x72] = 0;
      param_1[0x73] = 0;
      param_1[0x70] = 0;
      param_1[0x71] = 0;
      param_1[0x6e] = 0;
      param_1[0x6f] = 0;
      param_1[0x6c] = 0;
      param_1[0x6d] = 0;
      param_1[0x6a] = 0;
      param_1[0x6b] = 0;
      param_1[100] = 0;
      param_1[0x65] = 0;
      param_1[0x62] = 0;
      param_1[99] = 0;
      param_1[0x10] = 0;
      param_1[0x11] = 0;
      param_1[0xe] = 0;
      param_1[0xf] = 0;
      param_1[0x8c] = 0;
      param_1[0x8d] = 0;
      param_1[0x8a] = 0;
      param_1[0x8b] = 0;
      param_1[0x90] = 0;
      param_1[0x91] = 0;
      param_1[0x8e] = 0;
      param_1[0x8f] = 0;
      param_1[0x94] = 0;
      param_1[0x95] = 0;
      param_1[0x92] = 0;
      param_1[0x93] = 0;
      param_1[0x7c] = 0;
      param_1[0x7d] = 0;
      param_1[0x7a] = 0;
      param_1[0x7b] = 0;
      param_1[0x80] = 0;
      param_1[0x81] = 0;
      param_1[0x7e] = 0;
      param_1[0x7f] = 0;
      param_1[0x84] = 0;
      param_1[0x85] = 0;
      param_1[0x82] = 0;
      param_1[0x83] = 0;
      param_1[0xb4] = 0;
      param_1[0xb5] = 0;
      param_1[0xb2] = 0;
      param_1[0xb3] = 0;
      param_1[0xb0] = 0;
      param_1[0xb1] = 0;
      param_1[0xae] = 0;
      param_1[0xaf] = 0;
      param_1[0xac] = 0;
      param_1[0xad] = 0;
      param_1[0xaa] = 0;
      param_1[0xab] = 0;
      param_1[0x9c] = 0;
      param_1[0x9d] = 0;
      param_1[0x9a] = 0;
      param_1[0x9b] = 0;
      param_1[0xa0] = 0;
      param_1[0xa1] = 0;
      param_1[0x9e] = 0;
      param_1[0x9f] = 0;
      param_1[0xa4] = 0;
      param_1[0xa5] = 0;
      param_1[0xa2] = 0;
      param_1[0xa3] = 0;
      param_1[0xd4] = 0;
      param_1[0xd5] = 0;
      param_1[0xd2] = 0;
      param_1[0xd3] = 0;
      param_1[0xd0] = 0;
      param_1[0xd1] = 0;
      param_1[0xce] = 0;
      param_1[0xcf] = 0;
      param_1[0xcc] = 0;
      param_1[0xcd] = 0;
      param_1[0xca] = 0;
      param_1[0xcb] = 0;
      param_1[0xc4] = 0;
      param_1[0xc5] = 0;
      param_1[0xc2] = 0;
      param_1[0xc3] = 0;
      param_1[0xc0] = 0;
      param_1[0xc1] = 0;
      param_1[0xbe] = 0;
      param_1[0xbf] = 0;
      param_1[0xbc] = 0;
      param_1[0xbd] = 0;
      param_1[0xba] = 0;
      param_1[0xbb] = 0;
      param_1[0xf4] = 0;
      param_1[0xf5] = 0;
      param_1[0xf2] = 0;
      param_1[0xf3] = 0;
      param_1[0xf0] = 0;
      param_1[0xf1] = 0;
      param_1[0xee] = 0;
      param_1[0xef] = 0;
      param_1[0xec] = 0;
      param_1[0xed] = 0;
      param_1[0xea] = 0;
      param_1[0xeb] = 0;
      param_1[0xe4] = 0;
      param_1[0xe5] = 0;
      param_1[0xe2] = 0;
      param_1[0xe3] = 0;
      param_1[0xe0] = 0;
      param_1[0xe1] = 0;
      param_1[0xde] = 0;
      param_1[0xdf] = 0;
      param_1[0xdc] = 0;
      param_1[0xdd] = 0;
      param_1[0xda] = 0;
      param_1[0xdb] = 0;
      param_1[0x114] = 0;
      param_1[0x115] = 0;
      param_1[0x112] = 0;
      param_1[0x113] = 0;
      param_1[0x110] = 0;
      param_1[0x111] = 0;
      param_1[0x10e] = 0;
      param_1[0x10f] = 0;
      param_1[0x10c] = 0;
      param_1[0x10d] = 0;
      param_1[0x10a] = 0;
      param_1[0x10b] = 0;
      param_1[0x104] = 0;
      param_1[0x105] = 0;
      param_1[0x102] = 0;
      param_1[0x103] = 0;
      param_1[0x100] = 0;
      param_1[0x101] = 0;
      param_1[0xfe] = 0;
      param_1[0xff] = 0;
      param_1[0xfc] = 0;
      param_1[0xfd] = 0;
      param_1[0xfa] = 0;
      param_1[0xfb] = 0;
      param_1[0x134] = 0;
      param_1[0x135] = 0;
      param_1[0x132] = 0;
      param_1[0x133] = 0;
      param_1[0x130] = 0;
      param_1[0x131] = 0;
      param_1[0x12e] = 0;
      param_1[0x12f] = 0;
      param_1[300] = 0;
      param_1[0x12d] = 0;
      param_1[0x12a] = 0;
      param_1[299] = 0;
      param_1[0xc] = 0;
      param_1[0xd] = 0;
      param_1[10] = 0;
      param_1[0xb] = 0;
      *(undefined **)(param_1 + 0x16) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x18) = &DAT_00134f22;
      param_1[0x1c] = 0;
      param_1[0x1d] = 0;
      param_1[0x1a] = 0;
      param_1[0x1b] = 0;
      param_1[0x20] = 0;
      param_1[0x21] = 0;
      param_1[0x1e] = 0;
      param_1[0x1f] = 0;
      param_1[0x24] = 0;
      param_1[0x25] = 0;
      param_1[0x22] = 0;
      param_1[0x23] = 0;
      *(undefined **)(param_1 + 0x26) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x28) = &DAT_00134f22;
      param_1[0x2c] = 0;
      param_1[0x2d] = 0;
      param_1[0x2a] = 0;
      param_1[0x2b] = 0;
      param_1[0x30] = 0;
      param_1[0x31] = 0;
      param_1[0x2e] = 0;
      param_1[0x2f] = 0;
      param_1[0x34] = 0;
      param_1[0x35] = 0;
      param_1[0x32] = 0;
      param_1[0x33] = 0;
      *(undefined **)(param_1 + 0x36) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x38) = &DAT_00134f22;
      param_1[0x44] = 0;
      param_1[0x45] = 0;
      param_1[0x42] = 0;
      param_1[0x43] = 0;
      param_1[0x40] = 0;
      param_1[0x41] = 0;
      param_1[0x3e] = 0;
      param_1[0x3f] = 0;
      param_1[0x3c] = 0;
      param_1[0x3d] = 0;
      param_1[0x3a] = 0;
      param_1[0x3b] = 0;
      *(undefined **)(param_1 + 0x46) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x48) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x56) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x58) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x66) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x68) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x76) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x78) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x86) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x88) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x96) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x98) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xa6) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xa8) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xb6) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xb8) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xc6) = &DAT_00134f22;
      *(undefined **)(param_1 + 200) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xd6) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xd8) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xe6) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xe8) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xf6) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xf8) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x106) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x108) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x116) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x118) = &DAT_00134f22;
      param_1[0x124] = 0;
      param_1[0x125] = 0;
      param_1[0x122] = 0;
      param_1[0x123] = 0;
      param_1[0x120] = 0;
      param_1[0x121] = 0;
      param_1[0x11e] = 0;
      param_1[0x11f] = 0;
      param_1[0x11c] = 0;
      param_1[0x11d] = 0;
      param_1[0x11a] = 0;
      param_1[0x11b] = 0;
      *(undefined **)(param_1 + 0x126) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x128) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x136) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x138) = &DAT_00134f22;
      param_1[0x142] = 0;
      param_1[0x143] = 0;
      param_1[0x140] = 0;
      param_1[0x141] = 0;
      param_1[0x13e] = 0;
      param_1[0x13f] = 0;
      param_1[0x13c] = 0;
      param_1[0x13d] = 0;
      param_1[0x13a] = 0;
      param_1[0x13b] = 0;
      if ((param_2 < uVar17) && (param_2 < 0xffffffec)) {
        do {
          iVar9 = (int)uVar10;
          uVar23 = (ulong)(uint)(iVar9 + (int)((uVar10 & 0xffffffff) / 0x14) * -0x14);
          piVar18 = param_1 + uVar23 * 0x10 + 4;
          piVar18[0] = 0;
          piVar18[1] = 0;
          (param_1 + uVar23 * 0x10 + 6)[0] = 0;
          (param_1 + uVar23 * 0x10 + 6)[1] = 0;
          (param_1 + uVar23 * 0x10 + 8)[0] = 0;
          (param_1 + uVar23 * 0x10 + 8)[1] = 0;
          iVar34 = NEON_fmadd((float)(uint)(iVar9 + (int)((uVar10 & 0xffffffff) / 5) * -5),
                              0x432f0000,0xc3dc0000);
          iVar35 = NEON_fmadd((float)((uVar10 & 0xffffffff) / 5),0x42dc0000,0x43320000);
          param_1[uVar23 * 0x10 + 0xc] = 0x18;
          (param_1 + uVar23 * 0x10 + 0xf)[0] = 0;
          (param_1 + uVar23 * 0x10 + 0xf)[1] = 0;
          (param_1 + uVar23 * 0x10 + 0xd)[0] = 0;
          (param_1 + uVar23 * 0x10 + 0xd)[1] = 0;
          *(undefined1 *)(param_1 + uVar23 * 0x10 + 0x11) = 1;
          *(undefined8 *)((long)param_1 + uVar23 * 0x40 + 0x45) = 0;
          param_1[uVar23 * 0x10 + 10] = iVar34;
          param_1[uVar23 * 0x10 + 0xb] = iVar35;
          param_1[uVar23 * 0x10 + 0x13] = 0;
          if (uVar10 == 0) {
            *piVar18 = 0x29000;
            *(undefined **)(param_1 + uVar23 * 0x10 + 6) = &DAT_00137c9c;
            pcVar30 = "BSD MAP EDITOR";
            if (DAT_0028cc64 != '\0') {
              pcVar30 = &DAT_0028cc64;
            }
            uVar14 = 1;
            *(char **)(param_1 + uVar23 * 0x10 + 8) = pcVar30;
            *(undefined1 *)(param_1 + uVar23 * 0x10 + 0x12) = 1;
          }
          else if (DAT_0028cc30 == 4) {
            *piVar18 = 0x29001;
            *(char **)(param_1 + uVar23 * 0x10 + 6) = "CONFIRM";
            *(undefined **)(param_1 + uVar23 * 0x10 + 8) = &DAT_0028dbe4;
            uVar14 = DAT_0028cc2c;
            if (DAT_0028cc2c != 0) {
              uVar14 = (uint)((int)DAT_0028dce8 == 0);
            }
            *(char *)(param_1 + uVar23 * 0x10 + 0x11) = (char)uVar14;
            DAT_0028cc3c = DAT_0028cc3c | 1;
          }
          else {
            uVar26 = uVar10 - 1 & 0xffffffff;
            uVar15 = *(uint *)(&DAT_0019dab0 + uVar26 * 0x28);
            *piVar18 = iVar9 + 0x2901f;
            *(undefined **)(param_1 + uVar23 * 0x10 + 6) =
                 (&PTR_s_OPEN_MAP_EDITOR_0019dac0)[uVar26 * 5];
            bVar16 = 0;
            if ((DAT_0028cc2c != 0) && (DAT_0028cc30 == 0)) {
              bVar16 = ((0x13 >= uVar15 && uVar15 <= DAT_0028cbf8) &&
                       (0x13 < uVar15 || DAT_0028cbf8 != uVar15)) &
                       (byte)(DAT_0028cbfc >> (ulong)(uVar15 & 0x1f));
            }
            *(byte *)(param_1 + uVar23 * 0x10 + 0x11) = bVar16;
            lVar31 = uVar26 * 0xc0 + 0x28cce4;
            if (uVar15 - 0xe < 3) {
              *(undefined1 *)(param_1 + uVar23 * 0x10 + 0x12) = 2;
              uVar14 = DAT_0028cc00 >> (ulong)(uVar15 & 0x1f);
              *(ulong *)(param_1 + uVar23 * 0x10 + 0xe) = CONCAT44(uVar14,uVar14) & 0x100000001;
LAB_0018aae0:
              if (uVar15 == 0x10) {
                pcVar30 = "APPLIES WHEN THE EDITOR OPENS";
                if (DAT_0028cc24 != 0) {
                  pcVar30 = "REOPEN EDITOR TO APPLY";
                }
LAB_0018abb4:
                FUN_00183ac8(lVar31,0xffffffffffffffff,0xc0,&DAT_001343dc,pcVar30);
              }
              else {
                if (uVar15 != 0x12) goto LAB_0018ab20;
LAB_0018ab3c:
                pcVar30 = "LAST FILL";
                if (DAT_0028cc14 != 0) {
                  pcVar30 = "FILLING";
                }
                FUN_00183ac8(lVar31,0xffffffffffffffff,0xc0,"%s %u/%u TILES; NO AUTO-SAVE",pcVar30,
                             DAT_0028cc18,DAT_0028cc1c);
              }
            }
            else {
              if (uVar15 == 1) {
                *(undefined1 *)(param_1 + uVar23 * 0x10 + 0x12) = 2;
                uVar14 = DAT_0028cc04;
                param_1[uVar23 * 0x10 + 0xe] = DAT_0028cc04 & 0xff;
                param_1[uVar23 * 0x10 + 0xf] = uVar14;
LAB_0018ab20:
                if (((uVar15 & 0xfffffffe) != 8 && uVar15 != 10) || (DAT_0028cc14 == 0)) {
                  pcVar30 = (&PTR_s_Open_the_native_map_selection_sc_0019dac8)[uVar26 * 5];
                  goto LAB_0018abb4;
                }
                goto LAB_0018ab3c;
              }
              if (2 < uVar15 - 2) goto LAB_0018aae0;
              uVar14 = DAT_0028cc08;
              if (4 < DAT_0028cc08) {
                uVar14 = 0;
              }
              FUN_00183ac8(lVar31,0xffffffffffffffff,0xc0,"CURRENT: %s (%d)",
                           &DAT_001407d0 + *(int *)(&DAT_001407d0 + (ulong)uVar14 * 4));
            }
            uVar14 = (uint)*(byte *)(param_1 + uVar23 * 0x10 + 0x11);
            *(long *)(param_1 + uVar23 * 0x10 + 8) = lVar31;
            if ((uVar14 == 0) && (pcVar30 = "WAITING FOR INPUT OR CONFIRMATION", DAT_0028cc30 != 0))
            {
LAB_0018ac4c:
              *(char **)(param_1 + uVar23 * 0x10 + 8) = pcVar30;
            }
            else if ((uVar14 == 0) && (DAT_0028cc14 == 0)) {
              pcVar30 = "AVAILABLE FROM HOME";
              if (uVar15 != 0) {
                pcVar30 = "AVAILABLE IN THE MAP EDITOR";
              }
              goto LAB_0018ac4c;
            }
            DAT_0028cc3c = DAT_0028cc3c | 1 << (ulong)((uint)(uVar10 - 1) & 0x1f);
          }
          uVar10 = uVar10 + 1;
          param_1[uVar23 * 0x10 + 0x10] = (uint)(uVar14 == 0);
        } while ((uVar10 < uVar17) && (uVar10 < param_2 + 0x14));
      }
      puVar32 = PTR_DAT_001a36f0;
      uVar10 = 0;
      puVar28 = (undefined8 *)(PTR_DAT_001a3720 + 8);
      do {
        uVar17 = param_1[1];
        local_94 = 0x44010000;
        param_1[1] = uVar17 + 1;
        uStack_a8 = *puVar28;
        local_98 = NEON_fmadd((float)(uVar10 & 0xffffffff),0x43120000,0xc3e40000);
        local_b0 = (uint)*(ushort *)(puVar28 + -1);
        local_a0 = &DAT_00134f22;
        FUN_00184e28(&local_90,puVar32 + (ulong)*(ushort *)(puVar28 + -1) * 0x48);
        local_78 = 1;
        uVar10 = uVar10 + 1;
        *(undefined8 *)(param_1 + (ulong)uVar17 * 0x10 + 6) = uStack_a8;
        *(ulong *)(param_1 + (ulong)uVar17 * 0x10 + 4) = CONCAT44(uStack_ac,local_b0);
        *(ulong *)(param_1 + (ulong)uVar17 * 0x10 + 10) = CONCAT44(local_94,local_98);
        *(undefined **)(param_1 + (ulong)uVar17 * 0x10 + 8) = local_a0;
        *(undefined8 *)(param_1 + (ulong)uVar17 * 0x10 + 0xe) = uStack_88;
        *(undefined8 *)(param_1 + (ulong)uVar17 * 0x10 + 0xc) = local_90;
        *(ulong *)(param_1 + (ulong)uVar17 * 0x10 + 0x12) = CONCAT62(uStack_76,1);
        *(undefined8 *)(param_1 + (ulong)uVar17 * 0x10 + 0x10) = uStack_80;
        puVar28 = puVar28 + 2;
      } while (uVar10 != 6);
      break;
    case 0x70:
      uVar17 = 0;
      if (param_2 < 2) {
        uVar17 = param_2;
      }
      *param_1 = DAT_002846a0;
      param_1[1] = 0x14;
      *(undefined1 *)(param_1 + 2) = 0x70;
      *(undefined1 *)((long)param_1 + 9) = uVar7;
      *(undefined1 *)((long)param_1 + 10) = 0;
      *(char *)((long)param_1 + 0xb) = (char)uVar17;
      memset(param_1 + 3,0,0x684);
      param_1[0x44] = 0;
      param_1[0x45] = 0;
      param_1[0x42] = 0;
      param_1[0x43] = 0;
      param_1[0x5c] = 0;
      param_1[0x5d] = 0;
      param_1[0x5a] = 0;
      param_1[0x5b] = 0;
      param_1[0x60] = 0;
      param_1[0x61] = 0;
      param_1[0x5e] = 0;
      param_1[0x5f] = 0;
      param_1[100] = 0;
      param_1[0x65] = 0;
      param_1[0x62] = 0;
      param_1[99] = 0;
      param_1[0x4c] = 0;
      param_1[0x4d] = 0;
      param_1[0x4a] = 0;
      param_1[0x4b] = 0;
      param_1[0x50] = 0;
      param_1[0x51] = 0;
      param_1[0x4e] = 0;
      param_1[0x4f] = 0;
      param_1[0x54] = 0;
      param_1[0x55] = 0;
      param_1[0x52] = 0;
      param_1[0x53] = 0;
      param_1[0x7c] = 0;
      param_1[0x7d] = 0;
      param_1[0x7a] = 0;
      param_1[0x7b] = 0;
      param_1[0x80] = 0;
      param_1[0x81] = 0;
      param_1[0x7e] = 0;
      param_1[0x7f] = 0;
      param_1[0x6c] = 0;
      param_1[0x6d] = 0;
      param_1[0x6a] = 0;
      param_1[0x6b] = 0;
      param_1[0x70] = 0;
      param_1[0x71] = 0;
      param_1[0x6e] = 0;
      param_1[0x6f] = 0;
      param_1[0x74] = 0;
      param_1[0x75] = 0;
      param_1[0x72] = 0;
      param_1[0x73] = 0;
      param_1[0x84] = 0;
      param_1[0x85] = 0;
      param_1[0x82] = 0;
      param_1[0x83] = 0;
      *(undefined **)(param_1 + 6) = &DAT_00134f22;
      *(undefined **)(param_1 + 8) = &DAT_00134f22;
      param_1[0x8c] = 0;
      param_1[0x8d] = 0;
      param_1[0x8a] = 0;
      iVar9 = DAT_0028a4d0;
      param_1[0x8b] = 0;
      param_1[0x90] = 0;
      param_1[0x91] = 0;
      param_1[0x8e] = 0;
      param_1[0x8f] = 0;
      param_1[0x94] = 0;
      param_1[0x95] = 0;
      param_1[0x92] = 0;
      param_1[0x93] = 0;
      param_1[0x9c] = 0;
      param_1[0x9d] = 0;
      param_1[0x9a] = 0;
      param_1[0x9b] = 0;
      param_1[0xa0] = 0;
      param_1[0xa1] = 0;
      param_1[0x9e] = 0;
      param_1[0x9f] = 0;
      param_1[0xa4] = 0;
      param_1[0xa5] = 0;
      param_1[0xa2] = 0;
      param_1[0xa3] = 0;
      param_1[0xac] = 0;
      param_1[0xad] = 0;
      param_1[0xaa] = 0;
      param_1[0xab] = 0;
      param_1[0xb0] = 0;
      param_1[0xb1] = 0;
      param_1[0xae] = 0;
      param_1[0xaf] = 0;
      param_1[0xb4] = 0;
      param_1[0xb5] = 0;
      param_1[0xb2] = 0;
      param_1[0xb3] = 0;
      param_1[0xc4] = 0;
      param_1[0xc5] = 0;
      param_1[0xc2] = 0;
      param_1[0xc3] = 0;
      param_1[0xc0] = 0;
      param_1[0xc1] = 0;
      param_1[0xbe] = 0;
      param_1[0xbf] = 0;
      param_1[0xbc] = 0;
      param_1[0xbd] = 0;
      param_1[0xba] = 0;
      param_1[0xbb] = 0;
      param_1[0xd4] = 0;
      param_1[0xd5] = 0;
      param_1[0xd2] = 0;
      param_1[0xd3] = 0;
      param_1[0xd0] = 0;
      param_1[0xd1] = 0;
      param_1[0xce] = 0;
      param_1[0xcf] = 0;
      param_1[0xcc] = 0;
      param_1[0xcd] = 0;
      param_1[0xca] = 0;
      param_1[0xcb] = 0;
      param_1[0xe4] = 0;
      param_1[0xe5] = 0;
      param_1[0xe2] = 0;
      param_1[0xe3] = 0;
      param_1[0xe0] = 0;
      param_1[0xe1] = 0;
      param_1[0xde] = 0;
      param_1[0xdf] = 0;
      param_1[0xdc] = 0;
      param_1[0xdd] = 0;
      param_1[0xda] = 0;
      param_1[0xdb] = 0;
      param_1[0xf4] = 0;
      param_1[0xf5] = 0;
      param_1[0xf2] = 0;
      param_1[0xf3] = 0;
      param_1[0xf0] = 0;
      param_1[0xf1] = 0;
      param_1[0xee] = 0;
      param_1[0xef] = 0;
      param_1[0xec] = 0;
      param_1[0xed] = 0;
      param_1[0xea] = 0;
      param_1[0xeb] = 0;
      param_1[0x104] = 0;
      param_1[0x105] = 0;
      param_1[0x102] = 0;
      param_1[0x103] = 0;
      param_1[0x100] = 0;
      param_1[0x101] = 0;
      param_1[0xfe] = 0;
      param_1[0xff] = 0;
      param_1[0xfc] = 0;
      param_1[0xfd] = 0;
      param_1[0xfa] = 0;
      param_1[0xfb] = 0;
      param_1[0x114] = 0;
      param_1[0x115] = 0;
      param_1[0x112] = 0;
      param_1[0x113] = 0;
      param_1[0x110] = 0;
      param_1[0x111] = 0;
      param_1[0x10e] = 0;
      param_1[0x10f] = 0;
      param_1[0x10c] = 0;
      param_1[0x10d] = 0;
      param_1[0x10a] = 0;
      param_1[0x10b] = 0;
      param_1[0x124] = 0;
      param_1[0x125] = 0;
      param_1[0x122] = 0;
      param_1[0x123] = 0;
      param_1[0x120] = 0;
      param_1[0x121] = 0;
      param_1[0x11e] = 0;
      param_1[0x11f] = 0;
      param_1[0x11c] = 0;
      param_1[0x11d] = 0;
      param_1[0x11a] = 0;
      param_1[0x11b] = 0;
      param_1[0x134] = 0;
      param_1[0x135] = 0;
      param_1[0x132] = 0;
      param_1[0x133] = 0;
      pcVar30 = "SEARCH BY PLAYER TAG";
      if (iVar9 != 0) {
        pcVar30 = "PROFILE REQUEST PENDING";
      }
      param_1[0x130] = 0;
      param_1[0x131] = 0;
      param_1[0x12e] = 0;
      param_1[0x12f] = 0;
      param_1[300] = 0;
      param_1[0x12d] = 0;
      param_1[0x12a] = 0;
      param_1[299] = 0;
      param_1[0xc] = 0;
      param_1[0xd] = 0;
      param_1[10] = 0;
      param_1[0xb] = 0;
      param_1[0x10] = 0;
      param_1[0x11] = 0;
      param_1[0xe] = 0;
      param_1[0xf] = 0;
      param_1[0x14] = 0;
      param_1[0x15] = 0;
      param_1[0x12] = 0;
      param_1[0x13] = 0;
      *(undefined **)(param_1 + 0x16) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x18) = &DAT_00134f22;
      param_1[0x1c] = 0;
      param_1[0x1d] = 0;
      param_1[0x1a] = 0;
      param_1[0x1b] = 0;
      param_1[0x20] = 0;
      param_1[0x21] = 0;
      param_1[0x1e] = 0;
      param_1[0x1f] = 0;
      param_1[0x24] = 0;
      param_1[0x25] = 0;
      param_1[0x22] = 0;
      param_1[0x23] = 0;
      *(undefined **)(param_1 + 0x26) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x28) = &DAT_00134f22;
      param_1[0x2c] = 0;
      param_1[0x2d] = 0;
      param_1[0x2a] = 0;
      param_1[0x2b] = 0;
      param_1[0x30] = 0;
      param_1[0x31] = 0;
      param_1[0x2e] = 0;
      param_1[0x2f] = 0;
      param_1[0x34] = 0;
      param_1[0x35] = 0;
      param_1[0x32] = 0;
      param_1[0x33] = 0;
      *(undefined **)(param_1 + 0x36) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x38) = &DAT_00134f22;
      param_1[0x40] = 0;
      param_1[0x41] = 0;
      param_1[0x3e] = 0;
      param_1[0x3f] = 0;
      param_1[0x3c] = 0;
      param_1[0x3d] = 0;
      param_1[0x3a] = 0;
      param_1[0x3b] = 0;
      *(undefined **)(param_1 + 0x46) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x48) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x56) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x58) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x66) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x68) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x76) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x78) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x86) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x88) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x96) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x98) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xa6) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xa8) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xb6) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xb8) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xc6) = &DAT_00134f22;
      *(undefined **)(param_1 + 200) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xd6) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xd8) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xe6) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xe8) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xf6) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xf8) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x106) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x108) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x116) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x118) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x126) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x128) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x136) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x138) = &DAT_00134f22;
      param_1[0x142] = 0;
      param_1[0x143] = 0;
      param_1[0x140] = 0;
      param_1[0x141] = 0;
      param_1[0x13e] = 0;
      param_1[0x13f] = 0;
      param_1[0x13c] = 0;
      param_1[0x13d] = 0;
      param_1[0x13a] = 0;
      param_1[0x13b] = 0;
      FUN_00183ac8(&DAT_0028c328,0x1c0,0x1c0,&DAT_001343dc,pcVar30);
      pcVar30 = &DAT_0028a771;
      if (DAT_0028a771 == '\0' || DAT_0028c28c == 0) {
        pcVar30 = "LOCAL NAME APPEARANCE";
      }
      FUN_00183ac8(&DAT_0028c4e8,0x1c0,0x1c0,&DAT_001343dc,pcVar30);
      FUN_00183ac8(&DAT_0028c6a8,0x1c0,0x1c0,"RESTORE YOUR ACCOUNT NAME APPEARANCE");
      puVar32 = &UNK_0013280a;
      if (DAT_0028c2b8._4_4_ != 0) {
        puVar32 = &DAT_001346a5;
      }
      FUN_00183ac8(&DAT_0028c868,0x1c0,0x1c0,"%s - SHOW SKIN NAMES IN PROFILES",puVar32);
      pcVar30 = "NO VISUAL NAME CHANGE PENDING";
      if (DAT_0028a4d4 != 0) {
        pcVar30 = "APPLY THE SAVED VISUAL NAME";
      }
      FUN_00183ac8(&DAT_0028ca28,0x1c0,0x1c0,&DAT_001343dc,pcVar30);
      uVar10 = (ulong)(uVar17 * 5);
      puVar19 = (undefined1 *)((long)param_1 + uVar10 * 0x40 + 0x49);
      pcVar30 = (char *)(uVar10 * 0x1c0 + 0x28c168);
      do {
        uVar17 = 8;
        if (uVar10 != 4) {
          uVar17 = 4;
        }
        if (uVar10 == 1) {
          uVar17 = 1;
        }
        pcVar12 = pcVar30;
        if ((uVar10 == 0) && (pcVar12 = "BSD PLAYER PROFILES", DAT_0028c2c8 != '\0')) {
          pcVar12 = &DAT_0028c2c8;
        }
        uVar15 = (uint)uVar10;
        uVar14 = uVar15 - 5;
        uVar33 = 0x3f800000;
        if (uVar10 < 5) {
          uVar33 = 0;
          uVar14 = uVar15;
        }
        if (uVar10 == 0) {
          uVar24 = 1;
        }
        else {
          uVar24 = 0;
          if (((DAT_0028c28c != 0) && (DAT_0028a4c0 != 0)) && (DAT_0028c290 == 0)) {
            uVar24 = (uint)((DAT_0028a4c4 & uVar17) != 0);
          }
        }
        iVar9 = (&DAT_001407b8)[uVar10];
        uVar33 = NEON_fmadd(uVar33,0x42dc0000,0x43320000);
        *(char **)(puVar19 + -0x29) = pcVar12;
        *(undefined4 *)(puVar19 + -0x19) = 0x18;
        uVar36 = NEON_fmadd((float)uVar14,0x432f0000,0xc3dc0000);
        *(uint *)(puVar19 + -0x39) = uVar15 + 0x28000;
        *(long *)(puVar19 + -0x31) = (long)&DAT_001407b8 + (long)iVar9;
        *(undefined4 *)(puVar19 + -0x1d) = uVar33;
        *(undefined8 *)(puVar19 + -0xd) = 0;
        *(undefined4 *)(puVar19 + -0x21) = uVar36;
        *(undefined8 *)(puVar19 + -0x15) = 0;
        puVar19[-5] = (char)uVar24;
        *(undefined2 *)(puVar19 + -4) = 0;
        puVar19[-2] = 0;
        puVar19[-1] = uVar10 == 0;
        *puVar19 = 0;
        if ((uVar10 == 5) && (DAT_0028a4d4 == 0)) {
          puVar19[-5] = 0;
          *(undefined4 *)(puVar19 + -9) = 1;
          break;
        }
        if (uVar10 == 4) {
          puVar19[-1] = 2;
          *(ulong *)(puVar19 + -0x11) = CONCAT44(DAT_0028c2b8._4_4_,(undefined4)DAT_0028c2b8);
        }
        uVar10 = uVar10 + 1;
        pcVar30 = pcVar30 + 0x1c0;
        *(uint *)(puVar19 + -9) = uVar24 ^ 1;
        puVar19 = puVar19 + 0x40;
      } while (uVar10 != 6);
      puVar32 = PTR_DAT_001a36f0;
      uVar10 = 0;
      puVar28 = (undefined8 *)(PTR_DAT_001a3720 + 8);
      do {
        uVar17 = param_1[1];
        local_94 = 0x44010000;
        param_1[1] = uVar17 + 1;
        uStack_a8 = *puVar28;
        local_98 = NEON_fmadd((float)(uVar10 & 0xffffffff),0x43120000,0xc3e40000);
        local_b0 = (uint)*(ushort *)(puVar28 + -1);
        local_a0 = &DAT_00134f22;
        FUN_00184e28(&local_90,puVar32 + (ulong)*(ushort *)(puVar28 + -1) * 0x48);
        local_78 = 1;
        uVar10 = uVar10 + 1;
        *(undefined8 *)(param_1 + (ulong)uVar17 * 0x10 + 6) = uStack_a8;
        *(ulong *)(param_1 + (ulong)uVar17 * 0x10 + 4) = CONCAT44(uStack_ac,local_b0);
        *(ulong *)(param_1 + (ulong)uVar17 * 0x10 + 10) = CONCAT44(local_94,local_98);
        *(undefined **)(param_1 + (ulong)uVar17 * 0x10 + 8) = local_a0;
        *(undefined8 *)(param_1 + (ulong)uVar17 * 0x10 + 0xe) = uStack_88;
        *(undefined8 *)(param_1 + (ulong)uVar17 * 0x10 + 0xc) = local_90;
        *(ulong *)(param_1 + (ulong)uVar17 * 0x10 + 0x12) = CONCAT62(uStack_76,1);
        *(undefined8 *)(param_1 + (ulong)uVar17 * 0x10 + 0x10) = uStack_80;
        puVar28 = puVar28 + 2;
      } while (uVar10 != 6);
      break;
    case 0x74:
      if (DAT_00287b90 == 2) {
        uVar17 = 4;
      }
      else if (DAT_00287b90 == 3) {
        uVar17 = 10;
      }
      else {
        uVar17 = 6;
        if (DAT_00287b98 != 0) {
          uVar17 = DAT_00285420 + 6;
        }
      }
      *(undefined1 *)((long)param_1 + 10) = 0;
      *(undefined1 *)((long)param_1 + 9) = uVar7;
      if ((uVar17 + 4) / 5 <= param_2) {
        param_2 = 0;
      }
      *param_1 = iVar34 + iVar9;
      param_1[1] = 0x14;
      *(undefined1 *)(param_1 + 2) = 0x74;
      *(char *)((long)param_1 + 0xb) = (char)param_2;
      memset(param_1 + 3,0,0x684);
      param_1[0x142] = 0;
      param_1[0x143] = 0;
      local_f8 = 0;
      param_1[0x44] = 0;
      param_1[0x45] = 0;
      param_1[0x42] = 0;
      param_1[0x43] = 0;
      param_1[0x5c] = 0;
      param_1[0x5d] = 0;
      param_1[0x5a] = 0;
      param_1[0x5b] = 0;
      param_1[0x60] = 0;
      param_1[0x61] = 0;
      param_1[0x5e] = 0;
      param_1[0x5f] = 0;
      param_1[0x4c] = 0;
      param_1[0x4d] = 0;
      param_1[0x4a] = 0;
      param_1[0x4b] = 0;
      param_1[0x50] = 0;
      param_1[0x51] = 0;
      param_1[0x4e] = 0;
      param_1[0x4f] = 0;
      param_1[0x54] = 0;
      param_1[0x55] = 0;
      param_1[0x52] = 0;
      param_1[0x53] = 0;
      param_1[100] = 0;
      param_1[0x65] = 0;
      param_1[0x62] = 0;
      param_1[99] = 0;
      *(undefined **)(param_1 + 6) = &DAT_00134f22;
      *(undefined **)(param_1 + 8) = &DAT_00134f22;
      param_1[0x6c] = 0;
      param_1[0x6d] = 0;
      param_1[0x6a] = 0;
      param_1[0x6b] = 0;
      param_1[0x70] = 0;
      param_1[0x71] = 0;
      param_1[0x6e] = 0;
      param_1[0x6f] = 0;
      param_1[0x74] = 0;
      param_1[0x75] = 0;
      param_1[0x72] = 0;
      param_1[0x73] = 0;
      param_1[0x7c] = 0;
      param_1[0x7d] = 0;
      param_1[0x7a] = 0;
      param_1[0x7b] = 0;
      param_1[0x80] = 0;
      param_1[0x81] = 0;
      param_1[0x7e] = 0;
      param_1[0x7f] = 0;
      param_1[0x8c] = 0;
      param_1[0x8d] = 0;
      param_1[0x8a] = 0;
      param_1[0x8b] = 0;
      param_1[0x90] = 0;
      param_1[0x91] = 0;
      param_1[0x8e] = 0;
      param_1[0x8f] = 0;
      param_1[0x94] = 0;
      param_1[0x95] = 0;
      param_1[0x92] = 0;
      param_1[0x93] = 0;
      param_1[0x84] = 0;
      param_1[0x85] = 0;
      param_1[0x82] = 0;
      param_1[0x83] = 0;
      param_1[0xc] = 0;
      param_1[0xd] = 0;
      param_1[10] = 0;
      param_1[0xb] = 0;
      param_1[0xac] = 0;
      param_1[0xad] = 0;
      param_1[0xaa] = 0;
      param_1[0xab] = 0;
      param_1[0xb0] = 0;
      param_1[0xb1] = 0;
      param_1[0xae] = 0;
      param_1[0xaf] = 0;
      param_1[0xb4] = 0;
      param_1[0xb5] = 0;
      param_1[0xb2] = 0;
      param_1[0xb3] = 0;
      param_1[0x9c] = 0;
      param_1[0x9d] = 0;
      param_1[0x9a] = 0;
      param_1[0x9b] = 0;
      param_1[0xa0] = 0;
      param_1[0xa1] = 0;
      param_1[0x9e] = 0;
      param_1[0x9f] = 0;
      param_1[0xa4] = 0;
      param_1[0xa5] = 0;
      param_1[0xa2] = 0;
      param_1[0xa3] = 0;
      param_1[0xd4] = 0;
      param_1[0xd5] = 0;
      param_1[0xd2] = 0;
      param_1[0xd3] = 0;
      param_1[0xd0] = 0;
      param_1[0xd1] = 0;
      param_1[0xce] = 0;
      param_1[0xcf] = 0;
      param_1[0xcc] = 0;
      param_1[0xcd] = 0;
      param_1[0xca] = 0;
      param_1[0xcb] = 0;
      param_1[0xc4] = 0;
      param_1[0xc5] = 0;
      param_1[0xc2] = 0;
      param_1[0xc3] = 0;
      param_1[0xc0] = 0;
      param_1[0xc1] = 0;
      param_1[0xbe] = 0;
      param_1[0xbf] = 0;
      param_1[0xbc] = 0;
      param_1[0xbd] = 0;
      param_1[0xba] = 0;
      param_1[0xbb] = 0;
      param_1[0xf4] = 0;
      param_1[0xf5] = 0;
      param_1[0xf2] = 0;
      param_1[0xf3] = 0;
      param_1[0xf0] = 0;
      param_1[0xf1] = 0;
      param_1[0xee] = 0;
      param_1[0xef] = 0;
      param_1[0xec] = 0;
      param_1[0xed] = 0;
      param_1[0xea] = 0;
      param_1[0xeb] = 0;
      param_1[0xe4] = 0;
      param_1[0xe5] = 0;
      param_1[0xe2] = 0;
      param_1[0xe3] = 0;
      param_1[0xe0] = 0;
      param_1[0xe1] = 0;
      param_1[0xde] = 0;
      param_1[0xdf] = 0;
      param_1[0xdc] = 0;
      param_1[0xdd] = 0;
      param_1[0xda] = 0;
      param_1[0xdb] = 0;
      param_1[0x114] = 0;
      param_1[0x115] = 0;
      param_1[0x112] = 0;
      param_1[0x113] = 0;
      param_1[0x110] = 0;
      param_1[0x111] = 0;
      param_1[0x10e] = 0;
      param_1[0x10f] = 0;
      param_1[0x10c] = 0;
      param_1[0x10d] = 0;
      param_1[0x10a] = 0;
      param_1[0x10b] = 0;
      param_1[0x104] = 0;
      param_1[0x105] = 0;
      param_1[0x102] = 0;
      param_1[0x103] = 0;
      param_1[0x100] = 0;
      param_1[0x101] = 0;
      param_1[0xfe] = 0;
      param_1[0xff] = 0;
      param_1[0xfc] = 0;
      param_1[0xfd] = 0;
      param_1[0xfa] = 0;
      param_1[0xfb] = 0;
      param_1[0x134] = 0;
      param_1[0x135] = 0;
      param_1[0x132] = 0;
      param_1[0x133] = 0;
      param_1[0x130] = 0;
      param_1[0x131] = 0;
      param_1[0x12e] = 0;
      param_1[0x12f] = 0;
      param_1[300] = 0;
      param_1[0x12d] = 0;
      param_1[0x12a] = 0;
      param_1[299] = 0;
      param_1[0x10] = 0;
      param_1[0x11] = 0;
      param_1[0xe] = 0;
      param_1[0xf] = 0;
      param_1[0x14] = 0;
      param_1[0x15] = 0;
      param_1[0x12] = 0;
      param_1[0x13] = 0;
      *(undefined **)(param_1 + 0x16) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x18) = &DAT_00134f22;
      param_1[0x1c] = 0;
      param_1[0x1d] = 0;
      param_1[0x1a] = 0;
      param_1[0x1b] = 0;
      param_1[0x20] = 0;
      param_1[0x21] = 0;
      param_1[0x1e] = 0;
      param_1[0x1f] = 0;
      param_1[0x24] = 0;
      param_1[0x25] = 0;
      param_1[0x22] = 0;
      param_1[0x23] = 0;
      *(undefined **)(param_1 + 0x26) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x28) = &DAT_00134f22;
      param_1[0x2c] = 0;
      param_1[0x2d] = 0;
      param_1[0x2a] = 0;
      param_1[0x2b] = 0;
      param_1[0x30] = 0;
      param_1[0x31] = 0;
      param_1[0x2e] = 0;
      param_1[0x2f] = 0;
      param_1[0x34] = 0;
      param_1[0x35] = 0;
      param_1[0x32] = 0;
      param_1[0x33] = 0;
      *(undefined **)(param_1 + 0x36) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x38) = &DAT_00134f22;
      param_1[0x40] = 0;
      param_1[0x41] = 0;
      param_1[0x3e] = 0;
      param_1[0x3f] = 0;
      param_1[0x3c] = 0;
      param_1[0x3d] = 0;
      param_1[0x3a] = 0;
      param_1[0x3b] = 0;
      *(undefined **)(param_1 + 0x46) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x48) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x56) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x58) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x66) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x68) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x76) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x78) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x86) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x88) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x96) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x98) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xa6) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xa8) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xb6) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xb8) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xc6) = &DAT_00134f22;
      *(undefined **)(param_1 + 200) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xd6) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xd8) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xe6) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xe8) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xf6) = &DAT_00134f22;
      *(undefined **)(param_1 + 0xf8) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x106) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x108) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x116) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x118) = &DAT_00134f22;
      param_1[0x124] = 0;
      param_1[0x125] = 0;
      param_1[0x122] = 0;
      param_1[0x123] = 0;
      param_1[0x120] = 0;
      param_1[0x121] = 0;
      param_1[0x11e] = 0;
      param_1[0x11f] = 0;
      param_1[0x11c] = 0;
      param_1[0x11d] = 0;
      param_1[0x11a] = 0;
      param_1[0x11b] = 0;
      *(undefined **)(param_1 + 0x126) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x128) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x136) = &DAT_00134f22;
      *(undefined **)(param_1 + 0x138) = &DAT_00134f22;
      param_1[0x140] = 0;
      param_1[0x141] = 0;
      param_1[0x13e] = 0;
      param_1[0x13f] = 0;
      param_1[0x13c] = 0;
      param_1[0x13d] = 0;
      param_1[0x13a] = 0;
      uVar14 = DAT_00285424;
      param_1[0x13b] = 0;
      if ((DAT_00287b98 != 0) && (DAT_00285410 != 0)) {
        local_f8 = (uint)(DAT_00287ba4 == 0);
      }
      param_2 = param_2 * 5;
      uVar10 = (ulong)param_2;
      DAT_00287bb4 = 0;
      DAT_00287bb0 = DAT_00287bac;
      if ((param_2 < uVar17) && (param_2 < 0xffffffec)) {
        do {
          uVar24 = (uint)uVar10;
          uVar23 = (ulong)(uVar24 + (int)((uVar10 & 0xffffffff) / 0x14) * -0x14);
          iVar9 = NEON_fmadd((float)((uVar10 & 0xffffffff) / 5),0x42dc0000,0x43320000);
          piVar18 = param_1 + uVar23 * 0x10 + 4;
          piVar18[0] = 0;
          piVar18[1] = 0;
          (param_1 + uVar23 * 0x10 + 6)[0] = 0;
          (param_1 + uVar23 * 0x10 + 6)[1] = 0;
          param_1[uVar23 * 0x10 + 0xb] = iVar9;
          iVar9 = NEON_fmadd((float)(uVar24 + (int)((uVar10 & 0xffffffff) / 5) * -5),0x432f0000,
                             0xc3dc0000);
          (param_1 + uVar23 * 0x10 + 8)[0] = 0;
          (param_1 + uVar23 * 0x10 + 8)[1] = 0;
          param_1[uVar23 * 0x10 + 0xc] = 0x18;
          (param_1 + uVar23 * 0x10 + 0xd)[0] = 0;
          (param_1 + uVar23 * 0x10 + 0xd)[1] = 0;
          (param_1 + uVar23 * 0x10 + 0x11)[0] = 0;
          (param_1 + uVar23 * 0x10 + 0x11)[1] = 0;
          (param_1 + uVar23 * 0x10 + 0xf)[0] = 0;
          (param_1 + uVar23 * 0x10 + 0xf)[1] = 0;
          param_1[uVar23 * 0x10 + 10] = iVar9;
          param_1[uVar23 * 0x10 + 0x13] = 0;
          iVar35 = DAT_00287c68;
          iVar34 = DAT_00287ba4;
          iVar9 = DAT_00287b94;
          uVar15 = DAT_00285418;
          if (uVar10 == 0) {
            bVar8 = DAT_00287b90 != 0;
            *piVar18 = 0x26001;
            pcVar30 = "LEAVE THEME SETTINGS";
            if (bVar8) {
              pcVar30 = "CANCEL / PREVIOUS STEP";
            }
            param_1[uVar23 * 0x10 + 0xe] = 0;
            *(undefined1 *)(param_1 + uVar23 * 0x10 + 0x11) = 1;
            param_1[uVar23 * 0x10 + 0x10] = 0;
            *(undefined **)(param_1 + uVar23 * 0x10 + 6) = &DAT_00137c9c;
            *(char **)(param_1 + uVar23 * 0x10 + 8) = pcVar30;
            *(undefined2 *)(param_1 + uVar23 * 0x10 + 0x12) = 1;
            goto LAB_0018c350;
          }
          if (DAT_00287b90 == 3) {
            switch(uVar24) {
            case 1:
              uVar15 = uVar24;
              break;
            case 2:
              uVar15 = 4;
              break;
            case 3:
              uVar15 = 2;
              break;
            default:
              uVar21 = 8;
              if (uVar10 != 7) {
                uVar21 = 1;
              }
              uVar15 = 0;
              if (DAT_00285438 != 0) {
                uVar15 = (uint)(uVar10 == 7);
              }
              if ((uVar24 & 0xfffffffe) == 4) {
                bVar8 = false;
                pcVar30 = "APPLY NEIGHBORING THEME";
                goto LAB_0018c2d8;
              }
              if (uVar24 == 8) {
                bVar8 = false;
                pcVar30 = "RESTORE GAME BACKGROUND AND MUSIC";
                goto LAB_0018c2d8;
              }
              if (uVar24 != 9) goto LAB_0018bec4;
              bVar8 = false;
              uVar21 = 0;
              iVar9 = (&DAT_00140790)[uVar10];
              puVar32 = (&PTR_DAT_0019e990)[uVar10];
              pcVar30 = &DAT_00287c6c;
              goto LAB_0018c318;
            case 6:
              uVar21 = 4;
              uVar15 = (uint)(DAT_00285434 != 0);
LAB_0018bec4:
              bVar8 = false;
              pcVar12 = "ON";
              if (uVar15 == 0) {
                pcVar12 = "OFF";
              }
              pcVar30 = "REQUIRES BACKGROUND LAYER SUPPORT";
              if ((uVar21 & uVar14) != 0) {
                pcVar30 = pcVar12;
              }
              goto LAB_0018c2d8;
            }
            bVar8 = (DAT_00285430 & uVar15) != 0;
            uVar15 = (uint)bVar8;
            pcVar30 = "ON";
            if (!bVar8) {
              pcVar30 = "OFF";
            }
            bVar8 = true;
            uVar21 = 2;
LAB_0018c2d8:
            iVar9 = (&DAT_00140790)[uVar10];
            puVar32 = (&PTR_DAT_0019e990)[uVar10];
            uVar1 = 0;
            if (uVar10 != 9) {
              uVar1 = local_f8;
            }
            if (uVar1 == 1) {
              uVar21 = (uint)((uVar21 & uVar14) != 0);
            }
            else {
              uVar21 = 0;
            }
LAB_0018c318:
            *piVar18 = iVar9;
            *(undefined **)(param_1 + uVar23 * 0x10 + 6) = puVar32;
            *(char **)(param_1 + uVar23 * 0x10 + 8) = pcVar30;
            param_1[uVar23 * 0x10 + 0xe] = uVar15;
            *(char *)((long)param_1 + uVar23 * 0x40 + 0x49) = (char)uVar15;
            *(char *)(param_1 + uVar23 * 0x10 + 0x11) = (char)uVar21;
            param_1[uVar23 * 0x10 + 0x10] = uVar21 ^ 1;
            if ((bVar8) || ((uVar24 & 0xfffffffe) == 6)) {
              *(undefined1 *)(param_1 + uVar23 * 0x10 + 0x12) = 2;
            }
          }
          else if (DAT_00287b90 == 2) {
            if (uVar24 == 3) {
              piVar20 = param_1 + uVar23 * 0x10;
              *piVar18 = 0x26012;
              *(char **)(piVar20 + 6) = "MUSIC";
              puVar32 = &DAT_00287a6c;
            }
            else {
              if (uVar24 != 2) {
                if (uVar24 == 1) {
                  *piVar18 = 0x26007;
                  param_1[uVar23 * 0x10 + 0xe] = 0;
                  *(undefined1 *)((long)param_1 + uVar23 * 0x40 + 0x49) = 0;
                  *(char **)(param_1 + uVar23 * 0x10 + 6) = "CONFIRM";
                  *(undefined1 **)(param_1 + uVar23 * 0x10 + 8) = &DAT_00287c6c;
                  uVar15 = 0;
                  if (iVar35 == 1) {
                    uVar15 = local_f8;
                  }
                  *(char *)(param_1 + uVar23 * 0x10 + 0x11) = (char)(uVar15 & uVar14);
                  param_1[uVar23 * 0x10 + 0x10] = uVar15 & uVar14 ^ 1;
                }
                goto LAB_0018c350;
              }
              piVar20 = param_1 + uVar23 * 0x10;
              *piVar18 = 0x26011;
              *(char **)(piVar20 + 6) = "BACKGROUND";
              puVar32 = &DAT_00287944;
            }
            *(undefined **)(piVar20 + 8) = puVar32;
            piVar20[0xe] = 0;
            *(undefined1 *)((long)piVar20 + 0x49) = 0;
            *(undefined1 *)(piVar20 + 0x11) = 0;
            piVar20[0x10] = 1;
          }
          else {
            switch(uVar24) {
            case 1:
              iVar9 = 0;
              if (DAT_00285420 != 0) {
                iVar9 = DAT_00287b94 + 1;
              }
              FUN_00183ac8(&DAT_00287ccc,0x2704,0xd0,"%u-%u / %u",iVar9,DAT_00287b94 + DAT_00285420,
                           DAT_00285418);
              bVar8 = DAT_00287b90 != 0;
              *piVar18 = 0x26010;
              pcVar30 = "CHOOSE BACKGROUND";
              if (bVar8) {
                pcVar30 = "CHOOSE MUSIC";
              }
              param_1[uVar23 * 0x10 + 0xe] = 0;
              *(undefined1 *)((long)param_1 + uVar23 * 0x40 + 0x49) = 0;
              *(undefined1 *)(param_1 + uVar23 * 0x10 + 0x11) = 0;
              *(char **)(param_1 + uVar23 * 0x10 + 6) = pcVar30;
              *(undefined **)(param_1 + uVar23 * 0x10 + 8) = &DAT_00287ccc;
              param_1[uVar23 * 0x10 + 0x10] = 1;
              break;
            case 2:
              *piVar18 = 0x26002;
              param_1[uVar23 * 0x10 + 0xe] = 0;
              *(undefined1 *)((long)param_1 + uVar23 * 0x40 + 0x49) = 0;
              uVar15 = 0;
              if (iVar9 != 0) {
                uVar15 = local_f8;
              }
              *(char **)(param_1 + uVar23 * 0x10 + 6) = "PREVIOUS PAGE";
              *(char **)(param_1 + uVar23 * 0x10 + 8) = "PREVIOUS 8 THEMES";
              *(char *)(param_1 + uVar23 * 0x10 + 0x11) = (char)uVar15;
              param_1[uVar23 * 0x10 + 0x10] = uVar15 ^ 1;
              break;
            case 3:
              *piVar18 = 0x26003;
              param_1[uVar23 * 0x10 + 0xe] = 0;
              *(undefined1 *)((long)param_1 + uVar23 * 0x40 + 0x49) = 0;
              uVar24 = 0;
              if (iVar9 + 8U < uVar15) {
                uVar24 = local_f8;
              }
              *(char **)(param_1 + uVar23 * 0x10 + 6) = "NEXT PAGE";
              *(char **)(param_1 + uVar23 * 0x10 + 8) = "NEXT 8 THEMES";
              *(char *)(param_1 + uVar23 * 0x10 + 0x11) = (char)uVar24;
              param_1[uVar23 * 0x10 + 0x10] = uVar24 ^ 1;
              break;
            case 4:
              if (DAT_00287b90 == 0) {
                iVar9 = 0x26004;
                *(char **)(param_1 + uVar23 * 0x10 + 6) = "THEME OPTIONS";
                pcVar30 = "RANDOM / RESET / BACKGROUND";
              }
              else {
                iVar9 = 0x26006;
                *(char **)(param_1 + uVar23 * 0x10 + 6) = "NO MUSIC";
                pcVar30 = "CONTINUE WITHOUT MUSIC";
              }
              param_1[uVar23 * 0x10 + 0xe] = 0;
              *(char **)(param_1 + uVar23 * 0x10 + 8) = pcVar30;
              *piVar18 = iVar9;
              *(char *)(param_1 + uVar23 * 0x10 + 0x11) = (char)local_f8;
              *(undefined1 *)((long)param_1 + uVar23 * 0x40 + 0x49) = 0;
              param_1[uVar23 * 0x10 + 0x10] = local_f8 ^ 1;
              break;
            case 5:
              *piVar18 = 0x26014;
              param_1[uVar23 * 0x10 + 0xe] = 0;
              *(undefined1 *)((long)param_1 + uVar23 * 0x40 + 0x49) = 0;
              pcVar30 = "THEME STATUS";
              if (iVar34 != 0) {
                pcVar30 = "LOADING";
              }
              *(undefined1 *)(param_1 + uVar23 * 0x10 + 0x11) = 0;
              param_1[uVar23 * 0x10 + 0x10] = 1;
              *(char **)(param_1 + uVar23 * 0x10 + 6) = pcVar30;
              *(undefined1 **)(param_1 + uVar23 * 0x10 + 8) = &DAT_00287c6c;
              break;
            default:
              uVar26 = uVar10 - 6 & 0xffffffff;
              piVar20 = (int *)0x285428;
              if (DAT_00287b90 != 0) {
                piVar20 = &DAT_0028542c;
              }
              iVar34 = (&DAT_0028543c)[uVar26 * 0x4a];
              iVar9 = *piVar20;
              if (local_f8 == 0) {
                pcVar30 = "SELECTED - ";
                if (iVar34 != iVar9) {
                  pcVar30 = "";
                }
                if (DAT_00287b90 == 1) {
                  uVar21 = 0;
                  goto LAB_0018c11c;
                }
                uVar21 = 0;
                uVar15 = (&DAT_00285440)[uVar26 * 0x4a];
LAB_0018c1b0:
                pcVar12 = "BACKGROUND FILE UNAVAILABLE";
                if ((uVar15 & 2) != 0) {
                  pcVar12 = "CHOOSE MUSIC NEXT";
                }
              }
              else {
                if (DAT_00287b90 != 1) {
                  pcVar30 = "SELECTED - ";
                  if (iVar34 != iVar9) {
                    pcVar30 = "";
                  }
                  uVar15 = (&DAT_00285440)[uVar26 * 0x4a];
                  uVar21 = uVar15 >> 1 & 1;
                  goto LAB_0018c1b0;
                }
                uVar21 = 1;
                pcVar30 = "SELECTED - ";
                if (iVar34 != iVar9) {
                  pcVar30 = "";
                }
LAB_0018c11c:
                pcVar12 = "USE THIS THEME\'S MUSIC";
              }
              FUN_00183ac8(&DAT_00287ccc + (ulong)(uVar24 - 5) * 0xd0,0xffffffffffffffff,0xd0,
                           &DAT_00135833,pcVar30,pcVar12);
              *piVar18 = uVar24 + 0x2601a;
              *(char *)(param_1 + uVar23 * 0x10 + 0x11) = (char)uVar21;
              *(undefined **)(param_1 + uVar23 * 0x10 + 6) = &DAT_00285444 + uVar26 * 0x128;
              *(undefined **)(param_1 + uVar23 * 0x10 + 8) =
                   &DAT_00287ccc + (ulong)(uVar24 - 5) * 0xd0;
              param_1[uVar23 * 0x10 + 0xe] = (uint)(iVar34 == iVar9);
              *(bool *)((long)param_1 + uVar23 * 0x40 + 0x49) = iVar34 == iVar9;
              param_1[uVar23 * 0x10 + 0x10] = (uint)(uVar21 == 0);
              DAT_00287bb4 = DAT_00287bb4 | 1 << (ulong)((uint)(uVar10 - 6) & 0x1f);
              (&DAT_00287bc0)[uVar26] = (&DAT_0028543c)[uVar26 * 0x4a];
            }
          }
LAB_0018c350:
          uVar10 = uVar10 + 1;
        } while ((uVar10 < uVar17) && (uVar10 < param_2 + 0x14));
      }
      puVar32 = PTR_DAT_001a36f0;
      uVar10 = 0;
      puVar28 = (undefined8 *)(PTR_DAT_001a3720 + 8);
      do {
        uVar17 = param_1[1];
        local_94 = 0x44010000;
        param_1[1] = uVar17 + 1;
        uStack_a8 = *puVar28;
        local_98 = NEON_fmadd((float)(uVar10 & 0xffffffff),0x43120000,0xc3e40000);
        local_b0 = (uint)*(ushort *)(puVar28 + -1);
        local_a0 = &DAT_00134f22;
        FUN_00184e28(&local_90,puVar32 + (ulong)*(ushort *)(puVar28 + -1) * 0x48);
        local_78 = 1;
        uVar10 = uVar10 + 1;
        *(undefined8 *)(param_1 + (ulong)uVar17 * 0x10 + 6) = uStack_a8;
        *(ulong *)(param_1 + (ulong)uVar17 * 0x10 + 4) = CONCAT44(uStack_ac,local_b0);
        *(ulong *)(param_1 + (ulong)uVar17 * 0x10 + 10) = CONCAT44(local_94,local_98);
        *(undefined **)(param_1 + (ulong)uVar17 * 0x10 + 8) = local_a0;
        *(undefined8 *)(param_1 + (ulong)uVar17 * 0x10 + 0xe) = uStack_88;
        *(undefined8 *)(param_1 + (ulong)uVar17 * 0x10 + 0xc) = local_90;
        *(ulong *)(param_1 + (ulong)uVar17 * 0x10 + 0x12) = CONCAT62(uStack_76,1);
        *(undefined8 *)(param_1 + (ulong)uVar17 * 0x10 + 0x10) = uStack_80;
        puVar28 = puVar28 + 2;
      } while (uVar10 != 6);
    }
    local_78 = 1;
LAB_0018c408:
    uVar11 = 1;
  }
LAB_0018c414:
  DAT_00284688 = 0;
LAB_0018c418:
  if (*(long *)(lVar5 + 0x28) != local_70) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar11);
  }
  return;
}

/* ===== nexus_menu_theme_open @ 001946a0 ===== */

void nexus_menu_theme_open(void)

{
  (*(code *)PTR_nexus_menu_theme_open_001a3a58)();
  return;
}

/* ===== nexus_menu_main_view @ 00194800 ===== */

void nexus_menu_main_view(void)

{
  (*(code *)PTR_nexus_menu_main_view_001a3b08)();
  return;
}

/* ===== nexus_menu_theme_preview @ 001948d0 ===== */

void nexus_menu_theme_preview(void)

{
  (*(code *)PTR_nexus_menu_theme_preview_001a3b70)();
  return;
}

/* ===== nexus_menu_theme_preview_report @ 001948e0 ===== */

void nexus_menu_theme_preview_report(void)

{
  (*(code *)PTR_nexus_menu_theme_preview_report_001a3b78)();
  return;
}

/* ===== nexus_menu_theme_scroll_revision @ 00194910 ===== */

void nexus_menu_theme_scroll_revision(void)

{
  (*(code *)PTR_nexus_menu_theme_scroll_revision_001a3b90)();
  return;
}

