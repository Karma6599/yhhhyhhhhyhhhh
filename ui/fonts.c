/*
 * fonts — UI subsystem
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
 * Notes: Font body/port selection.
 */

/* ===== nexus_script_port_fonts_open @ 0016e4f8 ===== */

void nexus_script_port_fonts_open(void)

{
  bool bVar1;
  uint uVar2;
  long lVar3;
  int iVar4;
  undefined8 uVar5;
  undefined8 local_58;
  undefined8 local_50;
  ulong uStack_48;
  undefined8 uStack_40;
  undefined8 uStack_38;
  undefined8 local_30;
  long local_28;
  
  lVar3 = tpidr_el0;
  local_28 = *(long *)(lVar3 + 0x28);
  local_30 = 0;
  uStack_48 = 0;
  local_50 = 0;
  uStack_38 = 0;
  uStack_40 = 0;
  local_58 = DAT_0010f748;
  if (DAT_001a7cf0 == (code *)0x0) {
    uVar5 = 0;
  }
  else {
    uVar5 = (*DAT_001a7cf0)(DAT_001a7cc8);
    if (((int)uVar5 != 0) && (uVar5 = FUN_0016d910(), (int)uVar5 != 0)) {
      iVar4 = FUN_00191df8(&local_58);
      uVar5 = 0;
      if ((iVar4 != 0) && ((int)local_50 != 0)) {
        if ((DAT_00281b08 != 0) && (DAT_00281b08 != DAT_001a7d30)) {
          memset(&DAT_00281b08,0,0x648);
        }
        uVar5 = 1;
        DAT_00282118 = 1;
        uVar2 = DAT_00282b08 + 1;
        bVar1 = DAT_00282b08 < 0x20;
        DAT_00282b08 = uVar2;
        if ((bVar1) && (DAT_001a7d00 != (code *)0x0)) {
          (*DAT_001a7d00)(DAT_001a7cc8,"script_port_font_ui","chooser_open",uStack_48 & 0xffffffff);
          uVar5 = 1;
        }
      }
    }
  }
  if (*(long *)(lVar3 + 0x28) == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar5);
}

/* ===== FUN_00171808 @ 00171808 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00171808(float param_1,float param_2,ulong param_3)

{
  bool bVar1;
  uint uVar2;
  undefined *puVar3;
  char *pcVar4;
  long lVar5;
  int iVar6;
  undefined8 uVar7;
  char *pcVar8;
  long lVar9;
  undefined8 uVar10;
  ulong uVar11;
  undefined **ppuVar12;
  undefined8 *puVar13;
  undefined4 uVar14;
  float fVar15;
  float fVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  undefined8 local_130;
  undefined8 uStack_128;
  undefined8 uStack_120;
  undefined8 local_118;
  undefined8 uStack_110;
  long local_108;
  char local_100 [96];
  long local_a0;
  
  lVar5 = tpidr_el0;
  local_a0 = *(long *)(lVar5 + 0x28);
  iVar6 = FUN_0016d910();
  if ((((iVar6 == 0) || (local_100[0] = '\x01', DAT_001a7d28 + 0x19dU < 0x1001)) ||
      (iVar6 = (*DAT_001a7ce8)(DAT_001a7cc8,DAT_001a7d28 + 0x19c,local_100,1), iVar6 != 1)) ||
     (local_100[0] != '\0')) goto LAB_00171870;
  local_108 = 0;
  uStack_110 = 0;
  local_118 = 0;
  uStack_120 = 0;
  uStack_128 = 0;
  local_130 = DAT_0010f748;
  iVar6 = FUN_00191df8(&local_130);
  lVar9 = DAT_001a7d30;
  if (iVar6 == 0) {
    local_118 = 0;
    uStack_120 = 0;
    local_108 = 0;
    uStack_110 = 0;
    uStack_128 = 0;
    local_130 = 0;
  }
  if ((DAT_00281b08 != 0) && (DAT_00281b08 != DAT_001a7d30)) {
    memset(&DAT_00281b08,0,0x648);
  }
  if (((int)uStack_128 == 0) || (5 < (uint)uStack_120)) {
    _DAT_00282118 = 0;
LAB_00171bfc:
    lRam0000000000282148 = local_108;
    _DAT_00282140 = uStack_110;
    uRam0000000000282138 = local_118;
    _DAT_00282130 = uStack_120;
    uRam0000000000282128 = uStack_128;
    _DAT_00282120 = local_130;
    if ((DAT_00282110 != 0) && (iVar6 = FUN_0016d5cc(DAT_00281b10,lVar9), iVar6 != 0)) {
      FUN_0016cf5c(0xc61c3c00,0xc61c3c00,DAT_00281b10);
    }
    if ((((((int)uStack_128 == 0) || (uStack_128._4_4_ == 0)) || (uStack_120._4_4_ == 0)) ||
        (local_108 == DAT_002844d8)) ||
       (((DAT_002844e0 - 1 < param_3 && (param_3 - DAT_002844e0 < 500)) ||
        (DAT_002844e0 = param_3, iVar6 = nexus_script_port_ui_reload_request(), iVar6 == 0))))
    goto LAB_00171870;
    DAT_002844d8 = local_108;
    uVar2 = DAT_00282b08 + 1;
    bVar1 = 0x1f < DAT_00282b08;
    DAT_00282b08 = uVar2;
    if ((bVar1) || (DAT_001a7d00 == (code *)0x0)) goto LAB_00171870;
    pcVar8 = "reload_queued";
    uVar14 = (uint)uStack_120;
  }
  else {
    uRam0000000000282128 = uStack_128;
    _DAT_00282120 = local_130;
    uRam0000000000282138 = local_118;
    _DAT_00282130 = uStack_120;
    lRam0000000000282148 = local_108;
    _DAT_00282140 = uStack_110;
    _DAT_00282118 = CONCAT44(DAT_00282118,DAT_00282118);
    if (DAT_00282118 == 0) goto LAB_00171bfc;
    if (DAT_00282114 != 0) goto LAB_00171870;
    if (DAT_00282110 != 0) {
LAB_00171994:
      iVar6 = FUN_0016d5cc(DAT_00281b10,lVar9);
      if (iVar6 != 0) {
        fVar18 = param_1 / DAT_001e53fc;
        fVar19 = param_2 / DAT_001e53fc;
        uVar14 = NEON_fminnm(fVar18 / 640.0,fVar19 / 450.0);
        fVar17 = (float)NEON_fminnm(uVar14,0x3f800000);
        if (((ABS(fVar17) != INFINITY) && (!NAN(ABS(fVar17)))) && (0.0 < fVar17)) {
          FUN_0016cf00(fVar17,fVar17,DAT_00281b10);
          fVar15 = (float)NEON_fnmsub(param_1,0x3f000000,DAT_001e5400);
          fVar16 = (float)NEON_fnmsub(param_2,0x3f000000,DAT_001e5404);
          FUN_0016cf5c(fVar15 / DAT_001e53fc,fVar16 / DAT_001e53fc,DAT_00281b10);
          FUN_0017b5e8(0,0,fVar18 / fVar17,fVar19 / fVar17,DAT_00281b20);
          FUN_0016cf70(0x3f266666,DAT_00281b20);
          FUN_0017b5e8(0,0,0x441b0000,0x43cd0000,DAT_00281b18);
          FUN_0017b5e8(0x43898000,0xc32f0000,0x42400000,0x42280000,DAT_00281bb8);
          FUN_0017d6b0(0xc2140000,0xc3320000,&DAT_00281fe0,&DAT_00135038);
          uVar11 = 0;
          ppuVar12 = &PTR_s_DEFAULT_0019c080;
          puVar13 = &DAT_00281c50;
          do {
            puVar3 = &UNK_001326cc;
            if (uVar11 != (uStack_120 & 0xffffffff)) {
              puVar3 = &DAT_00134f22;
            }
            FUN_00176a24(local_100,0x60,0x60,&DAT_00135833,puVar3,*ppuVar12);
            uVar10 = *puVar13;
            uVar7 = FUN_0017b858(local_100);
            FUN_0016cec0(uVar10,uVar7);
            iVar6 = 0x96;
            if ((uVar11 & 1) == 0) {
              iVar6 = -0x96;
            }
            uVar14 = NEON_fmadd((float)(uVar11 >> 1 & 0x7fffffff),0x42ac0000,0xc2be0000);
            FUN_0017b5e8((float)iVar6,uVar14,0x43820000,0x42780000,*puVar13);
            uVar11 = uVar11 + 1;
            ppuVar12 = ppuVar12 + 3;
            puVar13 = puVar13 + 0x13;
          } while (uVar11 != 6);
          pcVar8 = "Selecting a font reloads the game.";
          if (local_118._4_4_ != 0) {
            pcVar8 = "Font could not load. Choose another or DEFAULT.";
          }
          pcVar4 = "Select a font in the lobby.";
          if (uStack_128._4_4_ != 0) {
            pcVar4 = pcVar8;
          }
          FUN_0017d6b0(0xc3820000,0x43180000,&DAT_00282078,pcVar4);
        }
      }
      goto LAB_00171870;
    }
    iVar6 = (*DAT_001a7cf8)(DAT_001a7cc8,"popup_generic");
    if ((iVar6 == 0) || (iVar6 = (*DAT_001a7cf8)(DAT_001a7cc8,"popover_text_left"), iVar6 == 0))
    goto LAB_00171870;
    iVar6 = FUN_0017d01c();
    lVar9 = DAT_001a7d30;
    if (iVar6 != 0) goto LAB_00171994;
    DAT_00282114 = 1;
    uVar2 = DAT_00282b08 + 1;
    bVar1 = 0x1f < DAT_00282b08;
    DAT_00282b08 = uVar2;
    if ((bVar1) || (DAT_001a7d00 == (code *)0x0)) goto LAB_00171870;
    pcVar8 = "chooser_contract_failed";
    uVar14 = 0;
  }
  (*DAT_001a7d00)(DAT_001a7cc8,"script_port_font_ui",pcVar8,uVar14);
LAB_00171870:
  if (*(long *)(lVar5 + 0x28) == local_a0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== nexus_script_port_fonts_open @ 001946b0 ===== */

void nexus_script_port_fonts_open(void)

{
  (*(code *)PTR_nexus_script_port_fonts_open_001a3a60)();
  return;
}

