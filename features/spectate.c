/*
 * Spectate / BrawlTV — Feature
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
 * Notes: Spectate-as-BrawlTV + tag spectate + spectator movement mode (speedLocalMove).
 */

/* ===== FUN_0015d2d4 @ 0015d2d4 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0015d2d4(void)

{
  long lVar1;
  int iVar2;
  long local_50;
  int local_44;
  undefined1 auStack_40 [4];
  int local_3c;
  int local_38;
  char *local_30;
  long local_28;
  
  lVar1 = tpidr_el0;
  local_28 = *(long *)(lVar1 + 0x28);
  if (DAT_00220800 == '\x01') {
    local_44 = -1;
    local_30 = "speedLocalMoveEnabled";
    if ((((((int)DAT_00214938 == 1) && (DAT_00214930 != (code *)0x0)) &&
         (iVar2 = (*DAT_00214930)(&local_30,auStack_40,1,&local_50,&local_44), iVar2 == 1)) &&
        ((local_50 != 0 && (local_44 == 0)))) &&
       ((local_38 == 2 && ((local_3c == 1 && (iVar2 = FUN_001550fc(), iVar2 != 0))))))
    goto LAB_0015d3a0;
  }
  DAT_002207f8 = 0;
  uRam00000000002207c0 = 0;
  DAT_002207b8 = 0;
  uRam00000000002207d0 = 0;
  _DAT_002207c8 = 0;
  uRam00000000002207e0 = 0;
  _DAT_002207d8 = 0;
  uRam00000000002207f0 = 0;
  _DAT_002207e8 = 0;
  uRam00000000002207b0 = 0;
  DAT_002207a8 = 0;
LAB_0015d3a0:
  if (*(long *)(lVar1 + 0x28) == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0016eca0 @ 0016eca0 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_0016eca0(long param_1)

{
  undefined4 uVar1;
  long lVar2;
  bool bVar3;
  int iVar4;
  undefined4 *puVar5;
  int *piVar6;
  undefined8 uVar7;
  long lVar8;
  ulong local_d0;
  int local_c4;
  undefined4 uStack_c0;
  uint local_bc;
  timespec local_b8;
  ulong uStack_a8;
  undefined4 local_a0;
  char *local_9c;
  uint local_94;
  undefined8 local_90;
  undefined8 uStack_88;
  undefined8 local_80;
  long local_78 [2];
  char *local_68;
  uint local_60;
  long local_58;
  
  lVar2 = tpidr_el0;
  local_58 = *(long *)(lVar2 + 0x28);
  puVar5 = (undefined4 *)__errno();
  uVar1 = *puVar5;
  local_d0 = 0;
  piVar6 = (int *)FUN_001bd828(&DAT_001cfd18);
  if ((*piVar6 == 0) && (DAT_00220800 == '\x01')) {
    local_c4 = -1;
    local_68 = "speedLocalMoveEnabled";
    if ((((int)DAT_00214938 == 1) &&
        ((((DAT_00214930 != (code *)0x0 &&
           (iVar4 = (*DAT_00214930)(&local_68,&local_b8,1,local_78,&local_c4), iVar4 == 1)) &&
          (local_78[0] != 0)) && ((local_c4 == 0 && ((int)local_b8.tv_nsec == 2)))))) &&
       ((local_b8.tv_sec._4_4_ == 1 && (uVar7 = FUN_001550fc(), (int)uVar7 != 0)))) {
      uVar7 = FUN_001428fc(uVar7,param_1 + 0x10,&local_d0,8);
      bVar3 = false;
      if ((((int)uVar7 == 0) || (0xfffffffffffedfff < local_d0 - 0x10000)) ||
         (((local_d0 & 7) != 0 || (local_d0 != DAT_0020f718)))) goto LAB_0016ef50;
      iVar4 = FUN_001428fc(uVar7,local_d0 + 0x30,&local_68,0xc);
      if (iVar4 != 0) {
        iVar4 = clock_gettime(1,&local_b8);
        if (iVar4 == 0) {
          local_b8.tv_nsec = local_b8.tv_sec * 1000 + (ulong)local_b8.tv_nsec / 1000000;
        }
        else {
          local_b8.tv_nsec = 0;
        }
        uStack_a8 = local_d0;
        local_b8.tv_sec = (__time_t)DAT_0020f6f8;
        local_a0 = DAT_0020f690;
        local_80 = DAT_002285b0;
        uStack_88 = _DAT_002285a8;
        local_90 = DAT_002285a0;
        local_9c = local_68;
        local_94 = local_60;
        if (((DAT_00228598 != DAT_0020f650) || ((ulong)local_b8.tv_nsec < DAT_0020f668)) ||
           (0xfa < local_b8.tv_nsec - DAT_0020f668)) {
          local_90 = 0;
          uStack_88 = 0;
          local_80 = 0;
        }
        iVar4 = FUN_0019f514(&DAT_002207a8,&local_b8,local_78);
        if (iVar4 != 0) {
          bVar3 = true;
          *piVar6 = 1;
          lVar8 = FUN_0014bef8(DAT_001cfb4c,local_78,0xc,local_d0 + 0x30);
          if (lVar8 != 0xc) {
            lVar8 = FUN_0014bef8(DAT_001cfb4c,&local_68,0xc,local_d0 + 0x30);
            if (((lVar8 != 0xc) ||
                (iVar4 = FUN_001428fc(0xc,local_d0 + 0x30,&local_c4,0xc), iVar4 == 0)) ||
               ((char *)CONCAT44(uStack_c0,local_c4) != local_68 || local_bc != local_60)) {
                    /* WARNING: Subroutine does not return */
              abort();
            }
            bVar3 = false;
            *piVar6 = 0;
          }
          goto LAB_0016ef50;
        }
      }
    }
  }
  bVar3 = false;
LAB_0016ef50:
  *puVar5 = uVar1;
  uVar7 = (*DAT_00228fd8)(param_1);
  uVar1 = *puVar5;
  if (bVar3) {
    lVar8 = FUN_0014bef8(DAT_001cfb4c,&local_68,0xc,local_d0 + 0x30);
    if (((lVar8 != 0xc) || (iVar4 = FUN_001428fc(0xc,local_d0 + 0x30,&local_b8,0xc), iVar4 == 0)) ||
       ((char *)local_b8.tv_sec != local_68 || (local_b8.tv_nsec & 0xffffffffU) != (ulong)local_60))
    {
      FUN_001417c8("fatal","spectator_restore_unverified",0);
                    /* WARNING: Subroutine does not return */
      abort();
    }
    *piVar6 = 0;
  }
  *puVar5 = uVar1;
  if (*(long *)(lVar2 + 0x28) == local_58) {
    return uVar7;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0016f024 @ 0016f024 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0016f024(long param_1)

{
  long lVar1;
  undefined4 uVar2;
  char cVar3;
  bool bVar4;
  long lVar5;
  undefined1 auVar6 [16];
  uint uVar7;
  int iVar8;
  undefined4 *puVar9;
  long *plVar10;
  long lVar11;
  long lVar12;
  long unaff_x30;
  ushort uVar13;
  undefined1 auVar14 [16];
  undefined1 auVar15 [16];
  undefined1 auVar16 [16];
  long local_90;
  int local_84;
  int local_80;
  int local_7c;
  int local_78;
  char *local_70;
  long local_68;
  
  lVar5 = tpidr_el0;
  local_68 = *(long *)(lVar5 + 0x28);
  puVar9 = (undefined4 *)__errno();
  uVar2 = *puVar9;
  plVar10 = (long *)FUN_001bd828(&DAT_001cfd38);
  lVar12 = *plVar10;
  if (DAT_001e0978 + 0xb3062c == unaff_x30) {
    if ((DAT_0020f700 == param_1) && (DAT_00220800 == '\x01')) {
      local_84 = -1;
      local_70 = "speedLocalMoveEnabled";
      if (((((int)DAT_00214938 != 1) ||
           (((DAT_00214930 == (code *)0x0 ||
             (iVar8 = (*DAT_00214930)(&local_70,&local_80,1,&local_90,&local_84), iVar8 != 1)) ||
            (local_90 == 0)))) || (((local_84 != 0 || (local_78 != 2)) || (local_7c != 1)))) ||
         (iVar8 = FUN_001550fc(), iVar8 == 0)) goto LAB_0016f148;
LAB_0016f284:
      *plVar10 = param_1;
      local_80 = 0;
      if ((param_1 != 0) && (lVar11 = FUN_00139fcc(), lVar11 != 0)) {
        do {
          uVar7 = _DAT_001dff88;
          cVar3 = '\x01';
          bVar4 = (bool)ExclusiveMonitorPass(0x1dff88,0x10);
          if (bVar4) {
            _DAT_001dff88 = CONCAT31(DAT_001dff88_1,1);
            cVar3 = ExclusiveMonitorsStatus();
          }
        } while (cVar3 != '\0');
        if ((uVar7 & 1) == 0) {
          auVar16._8_4_ = DAT_001cfae8;
          auVar16._0_8_ = _DAT_001cfae0;
          auVar6._8_8_ = _UNK_00112b38;
          auVar6._0_8_ = _DAT_00112b30;
          auVar16._12_4_ = DAT_001dffa0;
          _DAT_001dff88 = 0;
          auVar15 = NEON_cmeq(auVar16,auVar6,4);
          uVar13 = NEON_umaxv(CONCAT26(CONCAT11(~auVar15[0xd],~auVar15[0xc]),
                                       CONCAT24(CONCAT11(~auVar15[9],~auVar15[8]),
                                                CONCAT22(CONCAT11(~auVar15[5],~auVar15[4]),
                                                         CONCAT11(~auVar15[1],~auVar15[0])))),2);
          if ((((uVar13 & 1) == 0) && (DAT_001cfaec == 0)) && (bVar4 = false, DAT_001cfaf0 == 0))
          goto LAB_0016f1d8;
          lVar1 = param_1 + 0x92c;
          iVar8 = FUN_001428fc(lVar11,lVar1,&local_80,4);
          if (iVar8 != 0) {
            do {
              uVar7 = _DAT_001dff88;
              cVar3 = '\x01';
              bVar4 = (bool)ExclusiveMonitorPass(0x1dff88,0x10);
              if (bVar4) {
                _DAT_001dff88 = CONCAT31(DAT_001dff88_1,1);
                cVar3 = ExclusiveMonitorsStatus();
              }
            } while (cVar3 != '\0');
            if ((uVar7 & 1) == 0) {
              _DAT_001dff88 = 0;
              iVar8 = (uint)(DAT_001dffa0 == 3) << 2;
            }
            else {
              iVar8 = 0;
            }
            local_70 = (char *)CONCAT44(local_70._4_4_,iVar8);
            lVar11 = FUN_0014bef8(DAT_001cfb4c,&local_70,4,lVar1);
            if (lVar11 == 4) {
              bVar4 = true;
              goto LAB_0016f1d8;
            }
            lVar11 = FUN_0014bef8(DAT_001cfb4c,&local_80,4,lVar1);
            if (lVar11 != 4) {
                    /* WARNING: Subroutine does not return */
              abort();
            }
          }
        }
      }
      bVar4 = false;
      goto LAB_0016f1d8;
    }
LAB_0016f148:
    lVar11 = FUN_00139fcc();
    if ((lVar11 != 0) && (DAT_001e0038 == param_1)) {
      do {
        uVar7 = _DAT_001dff88;
        cVar3 = '\x01';
        bVar4 = (bool)ExclusiveMonitorPass(0x1dff88,0x10);
        if (bVar4) {
          _DAT_001dff88 = CONCAT31(DAT_001dff88_1,1);
          cVar3 = ExclusiveMonitorsStatus();
        }
      } while (cVar3 != '\0');
      if ((uVar7 & 1) == 0) {
        auVar14._8_4_ = DAT_001cfae8;
        auVar14._0_8_ = _DAT_001cfae0;
        auVar15._8_8_ = _UNK_00112b38;
        auVar15._0_8_ = _DAT_00112b30;
        auVar14._12_4_ = DAT_001dffa0;
        _DAT_001dff88 = 0;
        auVar15 = NEON_cmeq(auVar14,auVar15,4);
        uVar13 = NEON_umaxv(CONCAT26(CONCAT11(~auVar15[0xd],~auVar15[0xc]),
                                     CONCAT24(CONCAT11(~auVar15[9],~auVar15[8]),
                                              CONCAT22(CONCAT11(~auVar15[5],~auVar15[4]),
                                                       CONCAT11(~auVar15[1],~auVar15[0])))),2);
        if ((((uVar13 & 1) != 0) || (DAT_001cfaec != 0)) || (DAT_001cfaf0 != 0)) goto LAB_0016f284;
      }
    }
  }
  bVar4 = false;
  *plVar10 = 0;
  local_80 = 0;
LAB_0016f1d8:
  *puVar9 = uVar2;
  (*DAT_00228fe0)(param_1);
  uVar2 = *puVar9;
  if (bVar4) {
    local_70 = (char *)CONCAT44(local_70._4_4_,0xffffffff);
    lVar11 = FUN_0014bef8(DAT_001cfb4c,&local_80,4,param_1 + 0x92c);
    if (((lVar11 != 4) || (iVar8 = FUN_001428fc(4,param_1 + 0x92c,&local_70,4), iVar8 == 0)) ||
       ((int)local_70 != local_80)) {
                    /* WARNING: Subroutine does not return */
      abort();
    }
  }
  *puVar9 = uVar2;
  *plVar10 = lVar12;
  if (*(long *)(lVar5 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

/* ===== FUN_0016f3b0 @ 0016f3b0 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0016f3b0(undefined8 param_1,float *param_2,float *param_3,undefined8 param_4)

{
  undefined4 uVar1;
  char cVar2;
  bool bVar3;
  long lVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  undefined4 *puVar8;
  long *plVar9;
  long lVar10;
  uint uVar11;
  ushort uVar12;
  undefined1 auVar13 [16];
  float fVar14;
  float fVar15;
  float fVar16;
  int local_bc;
  int local_b8;
  undefined4 uStack_b4;
  int local_ac;
  undefined8 local_a8;
  float local_a0;
  float local_9c;
  float fStack_98;
  float local_94;
  float local_90;
  int iStack_8c;
  int local_88;
  float local_84 [3];
  long local_78;
  
  lVar4 = tpidr_el0;
  local_78 = *(long *)(lVar4 + 0x28);
  puVar8 = (undefined4 *)__errno();
  uVar1 = *puVar8;
  plVar9 = (long *)FUN_001bd828(&DAT_001cfd38);
  if ((*plVar9 != 0) && (DAT_00220800 == '\x01')) {
    local_ac = -1;
    local_a8 = "speedLocalMoveEnabled";
    if ((((int)DAT_00214938 == 1) &&
        (((((DAT_00214930 != (code *)0x0 &&
            (iVar6 = (*DAT_00214930)(&local_a8,&local_90,1,&local_b8,&local_ac), iVar6 == 1)) &&
           (CONCAT44(uStack_b4,local_b8) != 0)) && ((local_ac == 0 && (local_88 == 2)))) &&
         (iStack_8c == 1)))) &&
       ((((iVar6 = FUN_001550fc(), iVar6 != 0 && (DAT_002207a8 == DAT_0020f6f8)) &&
         (DAT_002207b8 == DAT_0020f718)) &&
        (((float *)(*plVar9 + 0x8e8) == param_2 && ((float *)(*plVar9 + 0x8f4) == param_3)))))) {
      iVar6 = FUN_0019f810(&DAT_002207a8,param_2,param_3,&local_90);
      if (iVar6 != 0) {
        param_3 = local_84;
        param_2 = &local_90;
      }
    }
  }
  if ((*plVar9 != 0) && (lVar10 = FUN_00139fcc(), lVar10 != 0)) {
    local_b8 = 0;
    local_ac = 0;
    if ((DAT_001e0060 == 0) ||
       (lVar10 = FUN_001428fc(lVar10,DAT_001e0060 + 0xcc,&local_b8,4), (int)lVar10 == 0)) {
      local_bc = 0;
    }
    else {
      lVar10 = FUN_001428fc(lVar10,DAT_001e0060 + 0xd0,&local_ac,4);
      local_bc = local_ac;
    }
    iVar6 = local_b8;
    local_a8 = (char *)((ulong)local_a8 & 0xffffffffffffff00);
    fVar15 = (float)DAT_001e0084 / 300.0;
    fVar16 = (float)DAT_001e0088 / 300.0;
    iVar7 = FUN_001428fc(lVar10,DAT_001e0978 + 0x1303f20,&local_a8,1);
    uVar11 = (uint)(byte)local_a8;
    if (1 < uVar11 || iVar7 == 0) {
      uVar11 = 0xffffffff;
    }
    do {
      uVar5 = _DAT_001dff88;
      cVar2 = '\x01';
      bVar3 = (bool)ExclusiveMonitorPass(0x1dff88,0x10);
      if (bVar3) {
        _DAT_001dff88 = CONCAT31(DAT_001dff88_1,1);
        cVar2 = ExclusiveMonitorsStatus();
      }
    } while (cVar2 != '\0');
    if ((uVar5 & 1) == 0) {
      auVar13._8_4_ = DAT_001cfae8;
      auVar13._0_8_ = _DAT_001cfae0;
      _DAT_001dff88 = 0;
      auVar13._12_4_ = DAT_001dffa0;
      auVar13 = NEON_cmeq(auVar13,_DAT_00112b30,4);
      uVar12 = NEON_umaxv(CONCAT26(CONCAT11(~auVar13[0xd],~auVar13[0xc]),
                                   CONCAT24(CONCAT11(~auVar13[9],~auVar13[8]),
                                            CONCAT22(CONCAT11(~auVar13[5],~auVar13[4]),
                                                     CONCAT11(~auVar13[1],~auVar13[0])))),2);
      if ((((uVar12 & 1) != 0) || (DAT_001cfaec != 0)) || (DAT_001cfaf0 != 0)) {
        fVar14 = ABS(*param_2);
        if ((fVar14 != INFINITY) && (!NAN(fVar14))) {
          fVar14 = ABS(*param_3);
          if ((fVar14 != INFINITY) && (!NAN(fVar14))) {
            fVar14 = ABS(param_2[1]);
            if ((fVar14 != INFINITY) && (!NAN(fVar14))) {
              fVar14 = ABS(param_3[1]);
              if ((fVar14 != INFINITY) && (!NAN(fVar14))) {
                fVar14 = ABS(param_2[2]);
                if ((((fVar14 != INFINITY) && (!NAN(fVar14))) && (ABS(param_3[2]) != INFINITY)) &&
                   (!NAN(ABS(param_3[2])))) {
                  local_a8 = *(char **)param_2;
                  local_9c = (float)*(undefined8 *)param_3;
                  fStack_98 = (float)((ulong)*(undefined8 *)param_3 >> 0x20);
                  local_a0 = param_2[2];
                  fVar14 = (float)(int)_DAT_001cfae0 / 100.0;
                  local_94 = param_3[2];
                  if (DAT_001dffa0 == 1) {
                    if (((ABS(fVar15) == INFINITY) || (NAN(ABS(fVar15)))) ||
                       ((ABS(fVar16) == INFINITY || (NAN(ABS(fVar16)))))) goto LAB_0016f648;
                    fStack_98 = fVar16 * -300.0;
                    local_a8._4_4_ = (float)NEON_fmadd(fVar16,0xc3960000,param_2[1] - param_3[1]);
                    local_a8._0_4_ = (float)NEON_fmadd(fVar15,0x43960000,*param_2 - *param_3);
                    fVar14 = fVar14 * 4000.0;
                    local_9c = fVar15 * 300.0;
                    local_94 = 300.0;
                  }
                  else if (DAT_001dffa0 == 2) {
                    if (((iVar6 - 0x7531U < 0xffff8ad0) || (local_bc - 0x7531U < 0xffff8ad0)) ||
                       (1 < uVar11)) goto LAB_0016f648;
                    fVar16 = (float)local_bc;
                    fVar15 = -0.58779;
                    if (uVar11 != 0) {
                      fVar15 = 0.58779;
                    }
                    fStack_98 = fVar16 * -0.5;
                    local_9c = (float)iVar6 * 0.5;
                    fVar14 = fVar16 * 4.0451 * fVar14;
                    local_a8._4_4_ = (float)NEON_fmadd(fVar16,0xbf000000,fVar16 * 5.0 * fVar15);
                    local_94 = 0.0;
                    local_a8._0_4_ = local_9c;
                  }
                  else {
                    local_a8._4_4_ = (float)((ulong)local_a8 >> 0x20);
                    fVar14 = fVar14 * param_2[2];
                  }
                  fVar16 = (float)DAT_001cfae8;
                  param_2 = (float *)&local_a8;
                  fVar15 = (float)DAT_001cfaec;
                  if (DAT_001dffa0 != 1) {
                    fVar15 = 0.0;
                  }
                  param_3 = &local_9c;
                  local_a8._0_4_ = (float)local_a8 + fVar16;
                  if (DAT_001dffa0 != 1) {
                    fVar16 = 0.0;
                  }
                  local_a0 = fVar14 + (float)(int)((ulong)_DAT_001cfae0 >> 0x20);
                  local_9c = fVar16 + local_9c;
                  local_a8 = (char *)CONCAT44(local_a8._4_4_ + (float)DAT_001cfaec,(float)local_a8);
                  fStack_98 = fVar15 + fStack_98;
                  local_94 = (float)DAT_001cfaf0 + local_94;
                }
              }
            }
          }
        }
      }
    }
  }
LAB_0016f648:
  *puVar8 = uVar1;
  (*(code *)(DAT_001e0978 + 0x671630))(param_1,param_2,param_3,param_4);
  if (*(long *)(lVar4 + 0x28) != local_78) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

/* ===== FUN_0017f590 @ 0017f590 [libNexusEvasionRuntime69252.so] ===== */

void FUN_0017f590(long *param_1)

{
  long lVar1;
  int iVar2;
  undefined8 uVar3;
  long local_258;
  int local_24c;
  char *local_248;
  uint local_240;
  int iStack_23c;
  int local_238;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  if ((DAT_0020d158 == 0) && (DAT_00220800 == '\x01')) {
    local_24c = -1;
    local_248 = "speedLocalMoveEnabled";
    if ((((int)DAT_00214938 == 1) &&
        (((DAT_00214930 != (code *)0x0 &&
          (iVar2 = (*DAT_00214930)(&local_248,&local_240,1,&local_258,&local_24c), iVar2 == 1)) &&
         (local_258 != 0)))) && (((local_24c == 0 && (local_238 == 2)) && (iStack_23c == 1)))) {
      uVar3 = FUN_001550fc();
      if ((int)uVar3 == 0) goto LAB_0017f778;
      if (((DAT_00228908 != 0) &&
          (uVar3 = FUN_001428fc(uVar3,DAT_00228908,&local_248,4), (int)uVar3 != 0)) &&
         (iVar2 = FUN_001428fc(uVar3,DAT_00228910,&local_240,0x208), iVar2 != 0)) {
        uVar3 = FUN_00199b4c(&DAT_00228908,(ulong)local_248 & 0xffffffff,&local_240,0x208);
        if (((int)uVar3 == 0) ||
           (uVar3 = FUN_001428fc(uVar3,DAT_0020f708 + 0x58,&local_240,8), (int)uVar3 == 0))
        goto LAB_0017f778;
        uVar3 = 0;
        if ((CONCAT44(iStack_23c,local_240) + 0x2000U < 0x12000) || ((local_240 & 7) != 0))
        goto LAB_0017f778;
        if (CONCAT44(iStack_23c,local_240) == *param_1) {
          iVar2 = FUN_001428fc(0,param_1[1] + 8,&local_248,4);
          uVar3 = 0;
          if ((iVar2 == 0) || ((int)local_248 != 2)) goto LAB_0017f778;
          if (DAT_00228488 == 4) {
            if (DAT_002148d8 != 0) goto LAB_0017f774;
          }
          else {
            uVar3 = 0;
            if ((((DAT_00228480 == 2) || (DAT_00228534 != 0)) || (DAT_0022856c != 0)) ||
               (DAT_002148d8 != 0)) goto LAB_0017f778;
          }
          if (DAT_00214914 == 0) {
            (*(code *)(DAT_001e0978 + 0x11a2830))(param_1[1]);
            uVar3 = 1;
            goto LAB_0017f778;
          }
        }
      }
    }
  }
LAB_0017f774:
  uVar3 = 0;
LAB_0017f778:
  if (*(long *)(lVar1 + 0x28) != local_38) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar3);
  }
  return;
}

