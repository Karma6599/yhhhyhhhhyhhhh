/*
 * Bolt Mod (autopilot) — Feature
 * Decompiled with Ghidra 11.3.2 (arm64 pseudocode) from: libNexusEvasion69252.so, libNexusEvasionRuntime69252.so
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
 * Notes: Bolt autopilot: wall avoidance, prediction, safe exit, steering smoothness.
 */

/* ===== FUN_00112940 @ 00112940 [libNexusEvasion69252.so] ===== */

char * FUN_00112940(char *param_1)

{
  int iVar1;
  
  if (param_1 != (char *)0x0) {
    iVar1 = strcmp(param_1,"isXrayEnabled");
    if ((((((iVar1 == 0) || (iVar1 = strcmp(param_1,"xRayEnabled"), iVar1 == 0)) ||
          (iVar1 = strcmp(param_1,"xrayEnabled"), iVar1 == 0)) ||
         (((iVar1 = strcmp(param_1,"xrayShowTargetName"), iVar1 == 0 ||
           (iVar1 = strcmp(param_1,"antiAfkEnabled"), iVar1 == 0)) ||
          ((iVar1 = strcmp(param_1,"hitboxRenderer"), iVar1 == 0 ||
           ((iVar1 = strcmp(param_1,"enemyTracer"), iVar1 == 0 ||
            (iVar1 = strcmp(param_1,"dynaJumpEnabled"), iVar1 == 0)))))))) ||
        (((iVar1 = strcmp(param_1,"attackRangeIndicator"), iVar1 == 0 ||
          ((((((iVar1 = strcmp(param_1,"ballAssistEnabled"), iVar1 == 0 ||
               (iVar1 = strcmp(param_1,"coltModEnabled"), iVar1 == 0)) ||
              (iVar1 = strcmp(param_1,"koltModEnabled"), iVar1 == 0)) ||
             ((iVar1 = strcmp(param_1,"autofarmEnabled"), iVar1 == 0 ||
              (iVar1 = strcmp(param_1,"autofarmAttackEnemies"), iVar1 == 0)))) ||
            ((iVar1 = strcmp(param_1,"characterOutlineEnabled"), iVar1 == 0 ||
             ((iVar1 = strcmp(param_1,"boltModEnabled"), iVar1 == 0 ||
              (iVar1 = strcmp(param_1,"boltWallAvoidEnabled"), iVar1 == 0)))))) ||
           (iVar1 = strcmp(param_1,"boltPredictionEnabled"), iVar1 == 0)))) ||
         (((iVar1 = strcmp(param_1,"boltAutoAttackEnabled"), iVar1 == 0 ||
           (iVar1 = strcmp(param_1,"boltSafeExitEnabled"), iVar1 == 0)) ||
          (iVar1 = strcmp(param_1,"trophiesAboveHead"), iVar1 == 0)))))) ||
       (((iVar1 = strcmp(param_1,"kitNaniModEnabled"), iVar1 == 0 ||
         (iVar1 = strcmp(param_1,"killauraSuper"), iVar1 == 0)) ||
        (iVar1 = strcmp(param_1,"killauraGadget"), iVar1 == 0)))) {
      param_1 = (char *)0x1;
    }
    else {
      iVar1 = strcmp(param_1,"speedLocalMoveEnabled");
      param_1 = (char *)(ulong)(iVar1 == 0);
    }
  }
  return param_1;
}

/* ===== FUN_0015fe20 @ 0015fe20 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0015fe20(long param_1)

{
  uint uVar1;
  long lVar2;
  undefined4 uVar3;
  float fVar4;
  float fVar5;
  int iVar6;
  undefined8 uVar7;
  ulong uVar8;
  uint *puVar9;
  ulong uVar10;
  ushort uVar11;
  float fVar12;
  undefined1 auVar13 [16];
  undefined1 auVar14 [16];
  float fVar15;
  float fVar16;
  float fVar17;
  float fVar18;
  float __y;
  long *local_1e8;
  code *pcStack_1e0;
  code *local_1d8;
  code *pcStack_1d0;
  long local_1c8;
  undefined8 uStack_1c0;
  long local_1b8;
  char *local_1b0 [2];
  uint local_1a0;
  undefined4 local_198;
  undefined4 uStack_194;
  int local_184;
  float local_17c;
  float fStack_178;
  undefined4 local_174;
  char local_170;
  char local_16f;
  long local_160;
  timespec local_158 [11];
  undefined8 local_a0;
  undefined1 auStack_98 [8];
  long local_90;
  
  lVar2 = tpidr_el0;
  local_90 = *(long *)(lVar2 + 0x28);
  iVar6 = clock_gettime(1,local_158);
  if (iVar6 == 0) {
    uVar10 = CONCAT44(local_158[0].tv_sec._4_4_,(undefined4)local_158[0].tv_sec) * 1000 +
             CONCAT44(local_158[0].tv_nsec._4_4_,(int)local_158[0].tv_nsec) / 1000000;
  }
  else {
    uVar10 = 0;
  }
  if (param_1 != 0) {
    iVar6 = DAT_0020f694 + -1000000;
    if (999999 < DAT_0020f694 + 0xfefc99c0U) {
      iVar6 = DAT_0020f694;
    }
    if (iVar6 == 0xf4246a) {
      local_1e8 = (long *)CONCAT44(local_1e8._4_4_,0xffffffff);
      local_1b0[0] = "boltModEnabled";
      if (((((int)DAT_00214938 == 1) && (DAT_00214930 != (code *)0x0)) &&
          (iVar6 = (*DAT_00214930)(local_1b0,local_158,1,&local_160,&local_1e8), iVar6 == 1)) &&
         (((local_160 != 0 && ((int)local_1e8 == 0)) &&
          (((int)local_158[0].tv_nsec == 2 && (local_158[0].tv_sec._4_4_ == 1)))))) {
        local_1e8 = (long *)CONCAT44(local_1e8._4_4_,0xffffffff);
        local_1b0[0] = "boltAutoAttackEnabled";
        if ((((int)DAT_00214938 == 1) && (DAT_00214930 != (code *)0x0)) &&
           ((iVar6 = (*DAT_00214930)(local_1b0,local_158,1,&local_160,&local_1e8), iVar6 == 1 &&
            ((((((local_160 != 0 && ((int)local_1e8 == 0)) && ((int)local_158[0].tv_nsec == 2)) &&
               ((local_158[0].tv_sec._4_4_ == 1 && (iVar6 = FUN_001550fc(), iVar6 != 0)))) &&
              (local_160 == DAT_00228648)) &&
             (((DAT_00228640 == DAT_0020f650 && (DAT_0022862c != 0)) && (DAT_00228394 == 0x84)))))))
           ) {
          auVar13._12_4_ = DAT_00214c14;
          auVar13._8_4_ = DAT_00214c10;
          auVar13._4_4_ = DAT_00214cf8;
          auVar13._0_4_ = DAT_00228480;
          auVar14._8_8_ = _UNK_00112868;
          auVar14._0_8_ = _DAT_00112860;
          auVar14 = NEON_cmeq(auVar13,auVar14,4);
          uVar11 = NEON_umaxv(CONCAT26(CONCAT11(~auVar14[0xd],~auVar14[0xc]),
                                       CONCAT24(CONCAT11(~auVar14[9],~auVar14[8]),
                                                CONCAT22(CONCAT11(~auVar14[5],~auVar14[4]),
                                                         CONCAT11(~auVar14[1],~auVar14[0])))),2);
          if ((((((((uVar11 & 1) == 0) && (DAT_00228488 == 3)) && (DAT_00228398 == DAT_00228640)) &&
                ((DAT_00214cf0 != DAT_00228640 &&
                 ((uVar10 <= DAT_00228c50 - 1 || (0x351 < uVar10 - DAT_00228c50)))))) &&
               (iVar6 = FUN_00168fe8(), iVar6 != 0)) &&
              ((iVar6 = FUN_001bc828(DAT_0020f670,FUN_001428fc,0,&local_170), iVar6 != 0 &&
               (local_170 == '\0')))) &&
             ((local_16f == '\0' && (uVar8 = (ulong)DAT_0020f6c8, DAT_0020f6c8 != 0)))) {
            puVar9 = (uint *)(DAT_0020f6c0 + 0x10);
            do {
              if (*puVar9 == DAT_0022862c) {
                if (*(long *)(puVar9 + -4) != 0) {
                  uVar1 = *puVar9;
                  iVar6 = FUN_00189c88(DAT_001e0978,*(long *)(puVar9 + -4),FUN_001428fc,0,local_1b0)
                  ;
                  if ((((iVar6 != 0) && (local_1a0 == uVar1)) && (local_184 != 0)) &&
                     (iVar6 = FUN_0017fba8(DAT_0020f698,DAT_0020f69c,local_198,uStack_194),
                     fVar5 = fStack_178, fVar4 = local_17c, iVar6 == 0)) {
                    fVar18 = local_17c - DAT_0020f6b4;
                    __y = fStack_178 - DAT_0020f6b8;
                    fVar12 = hypotf(fVar18,__y);
                    fVar15 = (float)NEON_ucvtf(DAT_0020f6bc);
                    fVar17 = (float)NEON_ucvtf(local_174);
                    fVar15 = fVar15 / 300.0;
                    fVar17 = fVar17 / 300.0;
                    if (fVar15 <= 0.25) {
                      fVar15 = 0.25;
                    }
                    if (fVar17 <= 0.25) {
                      fVar17 = 0.25;
                    }
                    fVar15 = (float)NEON_fminnm(fVar15,0x3f99999a);
                    fVar17 = (float)NEON_fminnm(fVar17,0x3f99999a);
                    fVar16 = -1.0;
                    if (fVar12 != 0.0001 && fVar12 < 0.0001 == NAN(fVar12)) {
                      fVar16 = (float)NEON_fmadd(DAT_002284b0,fVar18,__y * DAT_002284b4);
                      fVar16 = fVar16 / fVar12;
                    }
                    fVar18 = fVar15 + fVar17 + 3.25;
                    if (((fVar12 == fVar18 || fVar12 < fVar18 != (NAN(fVar12) || NAN(fVar18))) &&
                        ((0.15 <= fVar16 ||
                         (fVar15 = fVar15 + fVar17 + 0.85,
                         fVar12 == fVar15 || fVar12 < fVar15 != (NAN(fVar12) || NAN(fVar15)))))) &&
                       (uVar7 = FUN_00169714(fVar4,fVar5,0), (int)uVar7 != 0)) {
                      local_a0 = CONCAT44((int)(fStack_178 * 300.0),(int)(local_17c * 300.0));
                      iVar6 = FUN_001428fc(uVar7,DAT_0020f670 + 0xfac,auStack_98,8);
                      if (iVar6 != 0) {
                        local_1e8 = &local_1c8;
                        local_1b8 = local_160;
                        pcStack_1e0 = FUN_001428fc;
                        local_1c8 = DAT_0020f670;
                        uStack_1c0 = DAT_0020f680;
                        local_1d8 = FUN_00154ec4;
                        pcStack_1d0 = FUN_00169b3c;
                        iVar6 = FUN_0018a084(&local_1e8,DAT_0020f670,auStack_98,&local_a0);
                        if (iVar6 == 1) {
                          iVar6 = FUN_00169b3c(&local_1c8);
                          uVar3 = DAT_0020d158;
                          if (iVar6 != 0) {
                            DAT_00214cf8 = 5;
                            DAT_0020d158 = 1;
                            DAT_00214c10 = 1;
                            DAT_00228c50 = uVar10;
                            uVar10 = (*(code *)(DAT_001e0978 + 0xb2e994))(local_1c8,uStack_1c0);
                            DAT_00214cf8 = 0;
                            DAT_00214c10 = 0;
                            DAT_00214cf0 = DAT_0020f650;
                            DAT_0020d158 = uVar3;
                            snprintf((char *)local_158,0xb4,",\"target_gid\":%u,\"result\":%d",
                                     (ulong)local_1a0,uVar10 & 0xffffffff);
                            FUN_001417c8("bolt_fire","original_wrapper",local_158);
                          }
                        }
                        else if (iVar6 == -1) {
                    /* WARNING: Subroutine does not return */
                          abort();
                        }
                      }
                    }
                  }
                }
                break;
              }
              puVar9 = puVar9 + 0x10;
              uVar8 = uVar8 - 1;
            } while (uVar8 != 0);
          }
        }
      }
    }
  }
  if (*(long *)(lVar2 + 0x28) == local_90) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00167544 @ 00167544 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00167544(undefined8 *param_1)

{
  int *piVar1;
  long *plVar2;
  uint uVar3;
  ulong uVar4;
  long lVar5;
  undefined8 uVar6;
  int iVar7;
  undefined *puVar8;
  int iVar9;
  bool bVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  uint uVar14;
  long lVar15;
  long lVar16;
  long lVar17;
  uint uVar18;
  long lVar19;
  long lVar20;
  ulong uVar21;
  long lVar22;
  uint uVar23;
  ulong uVar24;
  float fVar25;
  float fVar26;
  undefined4 uVar27;
  undefined4 uVar28;
  undefined8 uVar29;
  undefined1 auVar30 [16];
  float fVar31;
  undefined8 uVar32;
  float fVar33;
  float fVar34;
  float fVar35;
  float fVar36;
  float fVar37;
  undefined8 uVar38;
  float fVar39;
  float fVar40;
  undefined4 local_870;
  undefined4 local_86c;
  undefined4 uStack_868;
  undefined1 auStack_864 [4];
  float local_860;
  float fStack_85c;
  undefined4 local_858;
  undefined4 local_854;
  undefined8 local_850;
  undefined8 uStack_848;
  undefined4 local_840;
  undefined4 uStack_83c;
  undefined4 uStack_838;
  undefined8 uStack_834;
  long local_820;
  undefined8 local_818;
  undefined8 local_810;
  undefined4 local_808;
  undefined4 uStack_804;
  char *local_800;
  undefined *puStack_7f8;
  undefined *local_7f0;
  undefined8 local_7e8;
  uint local_7e0;
  float fStack_7dc;
  float fStack_7d8;
  float fStack_7d4;
  undefined8 local_7d0;
  undefined *local_7c8;
  uint *local_7c0;
  uint local_7b8;
  undefined4 local_7b4;
  uint local_7b0 [4];
  long local_7a0;
  undefined4 uStack_794;
  undefined8 uStack_790;
  float local_788 [11];
  int local_75c;
  long local_b0;
  
  lVar5 = tpidr_el0;
  local_b0 = *(long *)(lVar5 + 0x28);
  if (param_1 == (undefined8 *)0x0) goto LAB_00168358;
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
  param_1[1] = 0;
  *param_1 = 0;
  FUN_00168604(&DAT_00215aa0);
  FUN_0016881c(param_1,"kitNaniModEnabled",0x79,&DAT_00228600,&DAT_00228608,&DAT_00228610);
  if (*(int *)((long)param_1 + 0xc) != 0) goto LAB_00168358;
  local_850 = 0;
  puStack_7f8 = PTR_s_boltWallAvoidEnabled_001c57a0;
  local_800 = PTR_s_boltModEnabled_001c5798;
  local_7e8 = PTR_s_boltSafeExitEnabled_001c57b0;
  local_7f0 = PTR_s_boltPredictionEnabled_001c57a8;
  iVar11 = DAT_0020f694 + -1000000;
  if (999999 < DAT_0020f694 + 0xfefc99c0U) {
    iVar11 = DAT_0020f694;
  }
  fStack_7d8 = SUB164(_PTR_s_boltSmoothPercent_001c57b8,8);
  fStack_7d4 = SUB164(_PTR_s_boltSmoothPercent_001c57b8,0xc);
  local_7e0 = SUB164(_PTR_s_boltSmoothPercent_001c57b8,0);
  fStack_7dc = SUB164(_PTR_s_boltSmoothPercent_001c57b8,4);
  local_7c8 = PTR_s_boltTargetRange_001c57d0;
  local_7d0 = PTR_s_boltExitHoldMs_001c57c8;
  if ((((((iVar11 == 0xf4246a) && (DAT_00214930 != (code *)0x0)) &&
        (iVar11 = (*DAT_00214930)(&local_800,local_7b0,8,&local_850,&local_820), iVar11 == 1)) &&
       (((local_850 != 0 && ((int)local_820 == 0)) &&
        ((local_7b0[1] == 1 &&
         ((iVar12 = FUN_001550fc(), uVar27 = (undefined4)local_7a0, lVar17 = local_850,
          iVar9 = DAT_00215b00, puVar8 = DAT_00215ad0, lVar15 = DAT_00215ac0, uVar23 = DAT_00215aac,
          iVar7 = DAT_0020f7f4, iVar11 = DAT_0020f7f0, iVar12 != 0 && (local_75c != 0)))))))) &&
      (DAT_00215ab8 == DAT_0020f650)) &&
     ((((DAT_00215ac0 == DAT_0020f6f8 && (local_850 == DAT_00215ac8)) &&
       (DAT_00215b00 == DAT_0020f690)) && ((DAT_0020d15c & 1) != 0)))) {
    uVar32 = 0;
    uVar38 = 0;
    uVar29 = NEON_scvtf(CONCAT44(DAT_00215b04._4_4_,(int)DAT_00215b04),4);
    fVar34 = (float)uVar29 / 300.0;
    fVar36 = (float)((ulong)uVar29 >> 0x20) / 300.0;
    if ((DAT_0022872c == DAT_00215b00) && (uVar38 = 0, DAT_00228728 != 0)) {
      uVar38 = 0;
      uVar18 = DAT_00215aac - DAT_00228728;
      if ((DAT_00228728 <= DAT_00215aac && uVar18 != 0) && (uVar38 = 0, uVar18 < 9)) {
        fVar37 = (fVar34 - DAT_00228730) * (30.0 / (float)uVar18);
        fVar39 = (fVar36 - DAT_00228734) * (30.0 / (float)uVar18);
        uVar38 = CONCAT44(fVar39,fVar37);
        fVar37 = fVar39 * fVar39 + fVar37 * fVar37;
        if (fVar37 != 400.0 && fVar37 < 400.0 == NAN(fVar37)) {
          uVar38 = 0;
        }
      }
    }
    fVar37 = (float)NEON_ucvtf(DAT_0020f6bc);
    DAT_0022872c = DAT_00215b00;
    DAT_00228728 = DAT_00215aac;
    DAT_00228730 = fVar34;
    DAT_00228734 = fVar36;
    if (DAT_0020f6c8 == 0) {
      fVar26 = 0.0;
      fVar39 = 0.0;
      fVar35 = 0.0;
      iVar12 = 0;
    }
    else {
      uVar32 = 0;
      fVar35 = 0.0;
      fVar39 = 0.0;
      fVar40 = 1e+09;
      fVar26 = 0.0;
      uVar24 = 0;
      iVar12 = 0;
      fVar25 = ((float)local_75c / 100.0) * ((float)local_75c / 100.0);
      lVar22 = DAT_0020f6c0;
      do {
        plVar2 = (long *)(lVar22 + uVar24 * 0x40);
        lVar16 = lVar22;
        if (((*(int *)((long)plVar2 + 0x2c) != 0) && ((int)plVar2[5] != 0)) &&
           ((*(int *)(lVar22 + uVar24 * 0x40 + 0x30) != 0 && (*plVar2 != DAT_0020f680)))) {
          lVar19 = lVar22 + uVar24 * 0x40;
          iVar13 = FUN_0017fba8(DAT_0020f698,DAT_0020f69c,*(undefined4 *)(lVar19 + 0x18),
                                *(undefined4 *)(lVar19 + 0x1c));
          lVar16 = DAT_0020f6c0;
          if (iVar13 == 0) {
            fVar31 = *(float *)(lVar19 + 0x38) - fVar36;
            fVar33 = *(float *)(lVar19 + 0x34) - fVar34;
            fVar31 = (float)NEON_fmadd(fVar33,fVar33,fVar31 * fVar31);
            if ((0.04 <= fVar31) &&
               (fVar31 == fVar25 || fVar31 < fVar25 != (NAN(fVar31) || NAN(fVar25)))) {
              piVar1 = (int *)(lVar22 + uVar24 * 0x40 + 0x10);
              lVar20 = -0x1f00;
              do {
                if (((*plVar2 == *(long *)(lVar20 + 0x20f0c0)) &&
                    (iVar13 = *(int *)(lVar20 + 0x20f0c8), iVar13 == *piVar1)) &&
                   (*(int *)(lVar20 + 0x20f0e0) - 1U < uVar23)) {
                  uVar18 = uVar23 - *(int *)(lVar20 + 0x20f0e0);
                  if (0x12 < uVar18) goto LAB_001678d0;
                  uVar29 = *(undefined8 *)(lVar20 + 0x20f0d4);
                  goto LAB_00167a18;
                }
                lVar20 = lVar20 + 0xf8;
              } while (lVar20 != 0);
              iVar13 = *piVar1;
              uVar18 = 0;
              uVar29 = 0;
LAB_00167a18:
              fVar33 = (float)NEON_fmadd((float)uVar18,0x3f0ccccd,fVar31);
              fVar31 = fVar33 * 0.72;
              if (iVar13 != DAT_0022862c) {
                fVar31 = fVar33;
              }
              if (fVar31 < fVar40) {
                fVar35 = (float)NEON_ucvtf(*(undefined4 *)(lVar22 + uVar24 * 0x40 + 0x3c));
                fVar35 = fVar35 / 300.0;
                uVar32 = uVar29;
                fVar40 = fVar31;
                iVar12 = iVar13;
                fVar39 = *(float *)(lVar19 + 0x38);
                fVar26 = *(float *)(lVar19 + 0x34);
              }
            }
          }
        }
LAB_001678d0:
        uVar24 = uVar24 + 1;
        lVar22 = lVar16;
      } while (uVar24 < DAT_0020f6c8);
    }
    lVar22 = DAT_00215ab8;
    uVar6 = _UNK_00112958;
    uVar29 = _DAT_00112950;
    lVar16 = DAT_00215ac0;
    if ((((DAT_00228650 == 0) || (DAT_00228638 != DAT_00215ac0)) ||
        ((DAT_00228628 != iVar9 ||
         ((DAT_0022862c != iVar12 ||
          (lVar16 = DAT_00228638, lVar19 = DAT_00228738, DAT_00228738 != local_850)))))) &&
       (bVar10 = DAT_00228740 == -1, DAT_00228740 = DAT_00228740 + 1, lVar19 = local_850, bVar10)) {
      DAT_00228740 = 1;
    }
    lVar20 = DAT_00228740;
    DAT_00228738 = lVar19;
    param_1[6] = lVar19;
    param_1[7] = lVar20;
    *(uint *)(param_1 + 3) = (uint)(iVar12 == 0) << 1;
    *(undefined4 *)((long)param_1 + 0x1c) = 0;
    uVar18 = DAT_00215aac;
    param_1[1] = uVar6;
    *param_1 = uVar29;
    uVar6 = _UNK_00112848;
    uVar29 = _DAT_00112840;
    *(uint *)(param_1 + 0x17) = uVar23;
    *(int *)((long)param_1 + 0xbc) = iVar9;
    *(undefined4 *)(param_1 + 0xb) = 0;
    *(uint *)((long)param_1 + 0x5c) = uVar18;
    param_1[9] = uVar6;
    param_1[8] = uVar29;
    uVar29 = DAT_0010e680;
    param_1[0x13] = 0;
    param_1[0x14] = lVar15;
    *(undefined4 *)(param_1 + 0x19) = uStack_794;
    *(float *)((long)param_1 + 0xcc) = local_788[0];
    param_1[10] = uVar29;
    uVar6 = _UNK_00112c48;
    uVar29 = _DAT_00112c40;
    *(int *)(param_1 + 2) = iVar9;
    *(int *)((long)param_1 + 0x14) = iVar12;
    param_1[4] = lVar16;
    param_1[5] = lVar22;
    param_1[0xc] = lVar16;
    param_1[0xd] = lVar22;
    param_1[0xe] = lVar19;
    param_1[0x10] = uVar6;
    param_1[0xf] = uVar29;
    param_1[0x11] = 0;
    param_1[0x12] = 0;
    param_1[0x15] = lVar17;
    param_1[0x16] = puVar8;
    *(int *)(param_1 + 0x18) = iVar12;
    *(undefined4 *)((long)param_1 + 0xc4) = uVar27;
    *(float *)(param_1 + 0x1a) = local_788[2];
    *(float *)((long)param_1 + 0xd4) = local_788[5];
    *(float *)(param_1 + 0x1b) = local_788[8];
    *(ulong *)((long)param_1 + 0xdc) = CONCAT44(fVar36,fVar34);
    *(undefined8 *)((long)param_1 + 0xe4) = uVar38;
    *(float *)((long)param_1 + 0xec) = fVar37 / 300.0;
    *(float *)(param_1 + 0x1e) = (float)iVar11;
    *(float *)((long)param_1 + 0xf4) = (float)iVar7;
    *(float *)(param_1 + 0x1f) = fVar26;
    *(float *)((long)param_1 + 0xfc) = fVar39;
    param_1[0x20] = uVar32;
    *(float *)(param_1 + 0x21) = fVar35;
    *(undefined4 *)((long)param_1 + 0x10c) = 0;
    memcpy(&DAT_00228618,param_1,0x110);
    iVar11 = *(int *)((long)param_1 + 0xc);
  }
  else {
    uRam0000000000228630 = 0;
    _DAT_00228628 = 0;
    DAT_00228640 = 0;
    DAT_00228638 = 0;
    DAT_00228650 = 0;
    DAT_00228648 = 0;
    uRam0000000000228660 = 0;
    _DAT_00228658 = 0;
    uRam0000000000228670 = 0;
    _DAT_00228668 = 0;
    uRam0000000000228680 = 0;
    _DAT_00228678 = 0;
    uRam0000000000228690 = 0;
    _DAT_00228688 = 0;
    uRam00000000002286a0 = 0;
    _DAT_00228698 = 0;
    uRam00000000002286b0 = 0;
    _DAT_002286a8 = 0;
    uRam00000000002286c0 = 0;
    _DAT_002286b8 = 0;
    uRam00000000002286d0 = 0;
    _DAT_002286c8 = 0;
    uRam00000000002286e0 = 0;
    _DAT_002286d8 = 0;
    uRam00000000002286f0 = 0;
    _DAT_002286e8 = 0;
    uRam0000000000228700 = 0;
    _DAT_002286f8 = 0;
    uRam0000000000228710 = 0;
    _DAT_00228708 = 0;
    uRam0000000000228720 = 0;
    _DAT_00228718 = 0;
    uRam0000000000228620 = 0;
    _DAT_00228618 = 0;
    DAT_00228728 = 0;
    iVar11 = *(int *)((long)param_1 + 0xc);
  }
  if (iVar11 != 0) goto LAB_00168358;
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
  param_1[1] = 0;
  *param_1 = 0;
  iVar7 = DAT_0020f694;
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
  iVar11 = iVar7 + -1000000;
  if (999999 < iVar7 + 0xfefc99c0U) {
    iVar11 = iVar7;
  }
  if (iVar11 == 0xf42401) {
    local_850._4_4_ = (undefined4)((ulong)local_850 >> 0x20);
    local_850 = CONCAT44(local_850._4_4_,0xffffffff);
    local_800 = "coltModEnabled";
    if (((((((int)DAT_00214938 != 1) || (DAT_00214930 == (code *)0x0)) ||
          (iVar11 = (*DAT_00214930)(&local_800,local_7b0,1,&local_820,&local_850), iVar11 != 1)) ||
         ((local_820 == 0 || ((int)local_850 != 0)))) ||
        (((local_7b0[2] != 2 || ((local_7b0[1] != 1 || (iVar11 = FUN_001550fc(), iVar11 == 0)))) ||
         (DAT_00215ac8 != local_820)))) ||
       ((((DAT_00215ab8 != DAT_0020f650 || (DAT_00215b00 != DAT_0020f690)) ||
         (DAT_00215ac0 != DAT_0020f6f8)) || (lVar15 = FUN_00154d1c(local_7b0), lVar15 == 0))))
    goto LAB_00167f74;
    local_808 = *(undefined4 *)(lVar15 + 0x14);
    local_854 = *(undefined4 *)(lVar15 + 0x18);
    FUN_001887c4(&local_800,lVar15,&local_808,&local_854,&local_858);
    fVar36 = (float)(int)DAT_00215b04 / 300.0;
    fVar37 = (float)DAT_00215b04._4_4_ / 300.0;
    fVar34 = local_788[3] - fVar36;
    fVar39 = local_788[4] - fVar37;
    local_860 = fVar39;
    fStack_85c = fVar34;
    uVar27 = FUN_00187924(&DAT_0020f5c0,DAT_0020f694,DAT_002148b8,0);
    uVar27 = FUN_00189070(fVar34,fVar39,local_808,local_854,uVar27,0x3d0f5c29,0x3f800000,auStack_864
                         );
    uStack_868 = NEON_fmadd(local_808,uVar27,local_788[3]);
    local_86c = NEON_fmadd(local_854,uVar27,local_788[4]);
    fVar34 = hypotf(fVar34,fVar39);
    FUN_0018940c(local_858,fVar34,uVar27,&local_800,&uStack_868,&local_86c);
    FUN_001894e4(fVar36,fVar37,local_788[3],local_788[4],local_808,local_854,uVar27,&uStack_868,
                 &local_86c);
    local_870 = 0;
    iVar11 = FUN_0019a4f4(fVar36,fVar37,uStack_868,local_86c,&DAT_00214820,
                          (ulong)local_7d0 & 0xffffffff,fStack_7d4,&fStack_85c,&local_860,&local_870
                         );
    if (iVar11 < 0) goto LAB_00167f84;
    if (iVar11 == 0) {
      uVar27 = 1;
    }
    else {
      uStack_848 = _UNK_001c57e0;
      local_850 = _DAT_001c57d8;
      local_840 = SUB84(PTR_FUN_001c57e8,0);
      uStack_83c = (undefined4)((ulong)PTR_FUN_001c57e8 >> 0x20);
      FUN_001ba6c0(fVar36,fVar37,&DAT_00214860,&local_850,DAT_00215aac,&fStack_85c,&local_860);
      uVar27 = 2;
    }
    iVar7 = DAT_00215b00;
    lVar17 = DAT_00215ac0;
    *(undefined4 *)(param_1 + 9) = uVar27;
    lVar15 = DAT_00215ab8;
    uVar29 = _UNK_00112a28;
    uVar32 = _DAT_00112a20;
    *(int *)(param_1 + 2) = iVar7;
    *(undefined4 *)((long)param_1 + 0x14) = (undefined4)local_7a0;
    uVar23 = DAT_00215aac;
    uVar38 = DAT_00214828;
    param_1[1] = uVar29;
    *param_1 = uVar32;
    uVar32 = DAT_0010e6a8;
    param_1[3] = 0;
    param_1[4] = lVar17;
    uVar29 = DAT_0010e720;
    param_1[7] = uVar38;
    *(undefined8 *)((long)param_1 + 0x4c) = uVar32;
    param_1[8] = uVar29;
    param_1[5] = lVar15;
    param_1[6] = local_820;
    *(undefined4 *)((long)param_1 + 0x54) = 2;
    *(uint *)(param_1 + 0xb) = (uint)(iVar11 == 0);
    *(uint *)((long)param_1 + 0x5c) = uVar23;
    *(float *)(param_1 + 0xf) = fStack_85c;
    *(float *)((long)param_1 + 0x7c) = local_860;
    param_1[0xc] = lVar17;
    param_1[0xd] = lVar15;
    param_1[0xe] = local_820;
    *(undefined4 *)(param_1 + 0x10) = 0x43250000;
    *(undefined4 *)((long)param_1 + 0x84) = local_870;
    param_1[0x21] = 0;
    param_1[0x20] = 0;
    param_1[0x1f] = 0;
    param_1[0x1e] = 0;
    param_1[0x1d] = 0;
    param_1[0x1c] = 0;
    param_1[0x1b] = 0;
    param_1[0x1a] = 0;
    param_1[0x19] = 0;
    param_1[0x18] = 0;
    param_1[0x17] = 0;
    param_1[0x16] = 0;
    param_1[0x15] = 0;
    param_1[0x14] = 0;
    param_1[0x13] = 0;
    param_1[0x12] = 0;
    param_1[0x11] = 0;
    iVar11 = *(int *)((long)param_1 + 0xc);
  }
  else {
LAB_00167f74:
    DAT_00214828 = 0;
    DAT_00214820 = 0;
    _DAT_00214838 = 0;
    DAT_00214830 = 0;
    DAT_00214848 = 0;
    _DAT_00214840 = 0;
    uRam0000000000214858 = 0;
    _DAT_00214850 = 0;
LAB_00167f84:
    iVar11 = *(int *)((long)param_1 + 0xc);
  }
  if (iVar11 == 0) {
    local_850._4_4_ = (undefined4)((ulong)local_850 >> 0x20);
    local_850 = CONCAT44(local_850._4_4_,0xffffffff);
    local_800 = "autofarmEnabled";
    if ((((((int)DAT_00214938 == 1) && (DAT_00214930 != (code *)0x0)) &&
         ((iVar11 = (*DAT_00214930)(&local_800,local_7b0,1,&local_808,&local_850), iVar11 == 1 &&
          ((CONCAT44(uStack_804,local_808) != 0 && ((int)local_850 == 0)))))) &&
        ((local_7b0[2] == 2 &&
         (((local_7b0[1] == 1 && (iVar11 = FUN_001550fc(), iVar11 != 0)) &&
          (lVar15 = CONCAT44(uStack_804,local_808), lVar15 == DAT_00215ac8)))))) &&
       (((DAT_00215ab8 == DAT_0020f650 && (DAT_00215ac0 == DAT_0020f6f8)) &&
        ((DAT_00215b00 == DAT_0020f690 && (DAT_0020d15c != 0)))))) {
      puStack_7f8 = SUB168(_PTR_s_autofarmFollowTarget_001c57f0,8);
      local_800 = SUB168(_PTR_s_autofarmFollowTarget_001c57f0,0);
      if ((((DAT_00214930 != (code *)0x0) &&
           (iVar11 = (*DAT_00214930)(&local_800,local_7b0,2,&local_850,&local_820), iVar11 == 1)) &&
          (local_850 == lVar15)) &&
         ((((int)local_820 == 0 && (-1 < (int)local_7b0[0])) && ((int)local_7b0[0] < 2)))) {
        lVar15 = CONCAT44(uStack_804,local_808);
        if (DAT_002287a8 != lVar15) {
          uRam0000000000228750 = 0;
          _DAT_00228748 = 0;
          DAT_00228760 = 0;
          _DAT_00228758 = 0;
          uRam0000000000228770 = 0;
          _DAT_00228768 = 0;
          uRam0000000000228780 = 0;
          _DAT_00228778 = 0;
          uRam0000000000228790 = 0;
          _DAT_00228788 = 0;
          uRam00000000002287a0 = 0;
          _DAT_00228798 = 0;
          DAT_002287a8 = lVar15;
        }
        local_820 = 0;
        local_818 = 0;
        local_810 = 0;
        if (local_7b0[0] == 0) {
          FUN_00168bac(lVar15,&local_820);
        }
        uVar24 = (ulong)DAT_0020f6c8;
        if (DAT_0020f6c8 == 0) {
          uVar23 = 0;
        }
        else {
          uVar21 = 0;
          uVar23 = 0;
          lVar15 = DAT_0020f6c0;
          do {
            plVar2 = (long *)(lVar15 + uVar21 * 0x40);
            if ((((int)plVar2[5] != 0) && (*(int *)((long)plVar2 + 0x2c) != 0)) &&
               ((*(int *)(lVar15 + uVar21 * 0x40 + 0x30) != 0 && (*plVar2 != DAT_0020f680)))) {
              lVar15 = lVar15 + uVar21 * 0x40;
              uVar3 = *(uint *)(lVar15 + 0x10);
              uVar14 = FUN_0017fba8(DAT_0020f698,DAT_0020f69c,*(undefined4 *)(lVar15 + 0x18),
                                    *(undefined4 *)(lVar15 + 0x1c));
              iVar11 = FUN_00186cf0(DAT_0020f6b4,DAT_0020f6b8,*(undefined4 *)(lVar15 + 0x34),
                                    *(undefined4 *)(lVar15 + 0x38),DAT_0020f7f0,DAT_0020f7f4,
                                    FUN_00150fa0,0,0x40);
              uVar18 = *(uint *)(lVar15 + 0x24);
              fVar34 = 1.0;
              if ((uVar18 != 0) && (fVar34 = 1.0, *(uint *)(lVar15 + 0x20) <= uVar18)) {
                fVar34 = (float)*(uint *)(lVar15 + 0x20) / (float)uVar18;
              }
              lVar22 = *plVar2;
              uVar32 = *(undefined8 *)(lVar15 + 0x34);
              fVar36 = (float)FUN_00165c60(plVar2);
              lVar17 = -0x1f00;
              do {
                if ((((*plVar2 == *(long *)(lVar17 + 0x20f0c0)) &&
                     (*(uint *)(lVar17 + 0x20f0c8) == *(uint *)(lVar15 + 0x10))) &&
                    (*(int *)(lVar17 + 0x20f0e0) - 1U < DAT_00215aac)) &&
                   (uVar18 = DAT_00215aac - *(int *)(lVar17 + 0x20f0e0), uVar18 < 0x13)) {
                  fVar37 = (float)NEON_fmadd((float)uVar18,0xbda3d70a,0x3f800000);
                  if (fVar37 <= 0.15) {
                    fVar37 = 0.15;
                  }
                  uVar29 = *(undefined8 *)(lVar17 + 0x20f0d4);
                  fVar39 = 1.0;
                  if (4 < uVar18) {
                    fVar39 = fVar37;
                  }
                  goto LAB_001682bc;
                }
                lVar17 = lVar17 + 0xf8;
              } while (lVar17 != 0);
              uVar29 = 0;
              uVar18 = 0;
              fVar39 = 1.0;
LAB_001682bc:
              uVar4 = (ulong)uVar23;
              uVar23 = uVar23 + 1;
              uVar24 = (ulong)DAT_0020f6c8;
              local_7b0[uVar4 * 0xe] = uVar3;
              local_7b0[uVar4 * 0xe + 1] = uVar14;
              local_7b0[uVar4 * 0xe + 2] = uVar18;
              local_7b0[uVar4 * 0xe + 3] = (uint)(iVar11 == 1);
              (&local_7a0)[uVar4 * 7] = lVar22;
              *(undefined8 *)(&stack0xfffffffffffff868 + uVar4 * 0x38) = uVar32;
              (&uStack_790)[uVar4 * 7] = uVar29;
              local_788[uVar4 * 0xe] = fVar34;
              local_788[uVar4 * 0xe + 1] = fVar36;
              local_788[uVar4 * 0xe + 2] = fVar39;
              lVar15 = DAT_0020f6c0;
            }
            uVar21 = uVar21 + 1;
          } while ((uVar21 < uVar24) && (uVar23 < 0x20));
        }
        uVar27 = 1;
        uVar32 = NEON_scvtf(CONCAT44(DAT_00215b04._4_4_,(int)DAT_00215b04),4);
        local_7f0 = DAT_00215ad0;
        local_7e8 = (undefined *)CONCAT44(1,DAT_00215b00);
        fStack_7dc = (float)uVar32 / 300.0;
        fStack_7d8 = (float)((ulong)uVar32 >> 0x20) / 300.0;
        auVar30._8_8_ = DAT_00215ac0;
        auVar30._0_8_ = DAT_00215ab8;
        local_7e0 = local_7b0[0];
        auVar30 = NEON_ext(auVar30,auVar30,8,1);
        puStack_7f8 = auVar30._8_8_;
        local_800 = auVar30._0_8_;
        fStack_7d4 = 1.0;
        if ((DAT_0020f6a4 != 0) && (DAT_0020f6a0 <= DAT_0020f6a4)) {
          fStack_7d4 = (float)DAT_0020f6a0 / (float)DAT_0020f6a4;
        }
        uVar28 = FUN_00165c60(&DAT_0020f680);
        local_7c0 = local_7b0;
        local_7d0 = (undefined *)CONCAT44((float)DAT_0020f7f0,uVar28);
        local_7c8 = (undefined *)CONCAT44(local_7c8._4_4_,(float)DAT_0020f7f4);
        local_7b4 = (undefined4)local_818;
        local_7b8 = uVar23;
        iVar11 = FUN_0019a950(&DAT_00228748,&local_800,&local_850);
        uVar29 = _UNK_00112858;
        uVar32 = _DAT_00112850;
        param_1[0x21] = 0;
        uVar38 = DAT_00228760;
        lVar17 = DAT_00215ac0;
        lVar15 = DAT_00215ab8;
        *(int *)(param_1 + 2) = DAT_00215b00;
        uVar23 = DAT_00215aac;
        param_1[1] = uVar29;
        *param_1 = uVar32;
        *(undefined4 *)((long)param_1 + 0x1c) = local_850._4_4_;
        if (iVar11 != 0) {
          uVar27 = 2;
        }
        param_1[4] = lVar17;
        param_1[5] = lVar15;
        *(undefined8 *)((long)param_1 + 0x14) = uStack_848;
        uVar32 = DAT_0010e720;
        param_1[6] = CONCAT44(uStack_804,local_808);
        param_1[7] = uVar38;
        *(undefined4 *)(param_1 + 9) = uVar27;
        param_1[8] = uVar32;
        uVar32 = DAT_0010e6a8;
        *(uint *)((long)param_1 + 0x5c) = uVar23;
        param_1[0xc] = lVar17;
        param_1[0xd] = lVar15;
        param_1[0xe] = CONCAT44(uStack_804,local_808);
        *(undefined8 *)((long)param_1 + 0x4c) = uVar32;
        *(undefined4 *)((long)param_1 + 0x54) = local_840;
        *(uint *)(param_1 + 0xb) = (uint)(iVar11 == 0);
        param_1[0x10] = uStack_834;
        param_1[0xf] = CONCAT44(uStack_838,uStack_83c);
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
        param_1[0x1c] = 0;
        param_1[0x1b] = 0;
        param_1[0x1e] = 0;
        param_1[0x1d] = 0;
        param_1[0x20] = 0;
        param_1[0x1f] = 0;
        goto LAB_00168358;
      }
    }
    uRam0000000000228750 = 0;
    _DAT_00228748 = 0;
    DAT_00228760 = 0;
    _DAT_00228758 = 0;
    uRam0000000000228770 = 0;
    _DAT_00228768 = 0;
    uRam0000000000228780 = 0;
    _DAT_00228778 = 0;
    uRam0000000000228790 = 0;
    _DAT_00228788 = 0;
    uRam00000000002287a0 = 0;
    _DAT_00228798 = 0;
    DAT_002287a8 = 0;
    if (*(int *)((long)param_1 + 0xc) == 0) {
      FUN_0016881c(param_1,"speedExploitEnabled",0x2e,&DAT_002287b0,&DAT_002287b8,&DAT_002287c0);
    }
  }
LAB_00168358:
  if (*(long *)(lVar5 + 0x28) == local_b0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0016944c @ 0016944c [libNexusEvasionRuntime69252.so] ===== */

void FUN_0016944c(void)

{
  long lVar1;
  bool bVar2;
  int iVar3;
  long local_60;
  int local_54;
  undefined1 auStack_50 [4];
  int local_4c;
  int local_48;
  char *local_40;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  iVar3 = DAT_0020f694 + -1000000;
  if (999999 < DAT_0020f694 + 0xfefc99c0U) {
    iVar3 = DAT_0020f694;
  }
  if (iVar3 == 0xf4246a) {
    local_54 = -1;
    local_40 = "boltModEnabled";
    if (((((int)DAT_00214938 == 1) && (DAT_00214930 != (code *)0x0)) &&
        (iVar3 = (*DAT_00214930)(&local_40,auStack_50,1,&local_60,&local_54), iVar3 == 1)) &&
       (((local_60 != 0 && (local_54 == 0)) && ((local_48 == 2 && (local_4c == 1)))))) {
      local_54 = -1;
      local_40 = "boltAutoAttackEnabled";
      if (((((int)DAT_00214938 == 1) && (DAT_00214930 != (code *)0x0)) &&
          ((iVar3 = (*DAT_00214930)(&local_40,auStack_50,1,&local_60,&local_54), iVar3 == 1 &&
           (((local_60 != 0 && (local_54 == 0)) && (local_48 == 2)))))) && (local_4c == 1)) {
        iVar3 = FUN_001550fc();
        bVar2 = iVar3 != 0;
        goto LAB_001695a4;
      }
    }
  }
  bVar2 = false;
LAB_001695a4:
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(bVar2);
}

/* ===== FUN_00169b3c @ 00169b3c [libNexusEvasionRuntime69252.so] ===== */

void FUN_00169b3c(undefined8 *param_1)

{
  long lVar1;
  int iVar2;
  ulong uVar3;
  long local_60;
  int local_54;
  undefined1 auStack_50 [4];
  int local_4c;
  int local_48;
  char *local_40;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  uVar3 = FUN_001550fc();
  if ((int)uVar3 != 0) {
    iVar2 = DAT_0020f694 + -1000000;
    if (999999 < DAT_0020f694 + 0xfefc99c0U) {
      iVar2 = DAT_0020f694;
    }
    if (iVar2 == 0xf4246a) {
      local_54 = -1;
      local_40 = "boltModEnabled";
      if (((((int)DAT_00214938 == 1) && (DAT_00214930 != (code *)0x0)) &&
          (iVar2 = (*DAT_00214930)(&local_40,auStack_50,1,&local_60,&local_54), iVar2 == 1)) &&
         (((local_60 != 0 && (local_54 == 0)) &&
          ((local_48 == 2 && ((local_4c == 1 && (local_60 == param_1[2])))))))) {
        local_54 = -1;
        local_40 = "boltAutoAttackEnabled";
        if (((int)DAT_00214938 == 1) &&
           (((((DAT_00214930 != (code *)0x0 &&
               (iVar2 = (*DAT_00214930)(&local_40,auStack_50,1,&local_60,&local_54), iVar2 == 1)) &&
              (local_60 != 0)) && ((local_54 == 0 && (local_48 == 2)))) &&
            ((local_4c == 1 && (local_60 == param_1[2])))))) {
          iVar2 = FUN_00150bf0(*param_1,param_1[1]);
          uVar3 = (ulong)(iVar2 != 0);
          goto LAB_00169cbc;
        }
      }
    }
    uVar3 = 0;
  }
LAB_00169cbc:
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar3);
}

