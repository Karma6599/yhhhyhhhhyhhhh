/*
 * Ball Assist — Feature
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
 * Notes: Ball assist / ignore-ball toggles + extended trajectory rendering.
 */

/* ===== FUN_00156570 @ 00156570 [libNexusEvasionRuntime69252.so] ===== */

void FUN_00156570(char *param_1,undefined4 *param_2,undefined4 *param_3)

{
  long lVar1;
  int iVar2;
  ulong uVar3;
  undefined8 uVar4;
  char **local_228;
  code *pcStack_220;
  code *local_218;
  code *pcStack_210;
  char *local_208;
  undefined8 local_200;
  undefined8 local_1f8;
  undefined8 uStack_1f0;
  undefined8 local_1e8;
  undefined8 uStack_1e0;
  undefined8 local_1d8;
  undefined8 uStack_1d0;
  undefined8 local_1c8;
  undefined8 uStack_1c0;
  undefined8 local_1b8;
  undefined8 uStack_1b0;
  undefined8 local_1a8;
  undefined8 local_1a0;
  undefined8 uStack_198;
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
  undefined8 local_120;
  undefined8 local_118;
  undefined8 uStack_110;
  undefined8 local_108;
  undefined8 uStack_100;
  undefined8 local_f8;
  undefined8 uStack_f0;
  undefined8 local_e8;
  undefined8 local_e0;
  undefined4 uStack_d8;
  float local_d4;
  float fStack_d0;
  undefined4 uStack_cc;
  long local_c8;
  undefined4 local_c0;
  undefined8 local_bc;
  undefined4 local_b4;
  undefined4 uStack_b0;
  undefined8 local_a8;
  undefined8 uStack_a0;
  undefined8 local_98;
  undefined8 local_90;
  undefined8 uStack_88;
  undefined4 local_80;
  undefined4 uStack_7c;
  undefined8 local_78;
  undefined8 local_70;
  undefined8 local_68;
  undefined4 local_60;
  undefined4 uStack_5c;
  long local_58;
  
  lVar1 = tpidr_el0;
  local_58 = *(long *)(lVar1 + 0x28);
  uVar3 = 0;
  if ((((param_1 != (char *)0x0) && (param_2 != (undefined4 *)0x0)) &&
      (param_3 != (undefined4 *)0x0)) &&
     ((uVar3 = FUN_00155c04(0,param_1), (int)uVar3 != 0 &&
      (uVar3 = FUN_00150bf0(*(undefined8 *)(param_1 + 0x10),*(undefined8 *)(param_1 + 0x18)),
      (int)uVar3 != 0)))) {
    local_120 = CONCAT44(local_120._4_4_,0xffffffff);
    local_208 = "ballAssistEnabled";
    if ((((int)DAT_00214938 == 1) &&
        ((DAT_00214930 != (code *)0x0 &&
         (iVar2 = (*DAT_00214930)(&local_208,&local_1a0,1,&local_c8,&local_120), iVar2 == 1)))) &&
       ((local_c8 != 0 &&
        ((((int)local_120 == 0 && ((int)uStack_198 == 2)) && (local_1a0._4_4_ == 1)))))) {
      uVar3 = FUN_00156950(*(undefined8 *)(param_1 + 0x10),*(undefined8 *)(param_1 + 0x18),param_2,
                           param_3);
    }
    else if (DAT_002149c4 == 0 && DAT_002149d4 == 0) {
      uVar3 = 1;
    }
    else {
      uVar3 = FUN_00189c88(DAT_001e0978,*(undefined8 *)(param_1 + 0x18),FUN_001428fc,0,&DAT_0020f680
                          );
      if (((int)uVar3 != 0) &&
         (uVar3 = FUN_001543f8(DAT_0020f678,*(undefined8 *)(param_1 + 0x18)), (int)uVar3 != 0)) {
        uStack_88 = *(undefined8 *)(param_1 + 8);
        local_90 = *(undefined8 *)param_1;
        uVar4 = NEON_scvtf(CONCAT44(*param_3,*param_2),4);
        local_70 = CONCAT44((float)((ulong)uVar4 >> 0x20) / 300.0,(float)uVar4 / 300.0);
        local_b4 = DAT_002149d8;
        uStack_b0 = 0;
        local_c8 = DAT_0010e608;
        local_c0 = DAT_002149e0;
        local_a8 = *(undefined8 *)(param_1 + 0x10);
        local_98 = *(undefined8 *)(param_1 + 0x18);
        local_78 = DAT_00214988;
        local_bc = DAT_0010e740;
        local_e0 = 0;
        uStack_d8 = 0;
        local_d4 = 0.0;
        uStack_a0 = DAT_0020f678;
        fStack_d0 = 0.0;
        uStack_cc = 0;
        local_e8 = 0;
        uStack_f0 = 0;
        local_f8 = 0;
        uStack_100 = 0;
        local_108 = 0;
        uStack_110 = 0;
        local_118 = 0;
        local_80 = DAT_0020f690;
        uStack_7c = DAT_00214b24;
        local_120 = DAT_0010e7f8;
        uVar4 = FUN_00185c68(&DAT_0020d168,&DAT_0020f628,&local_c8,FUN_00150f7c,0,&local_e0,
                             &local_120);
        if ((int)uVar4 == 1) {
          local_190 = *(undefined8 *)(param_1 + 0x10);
          local_180 = *(undefined8 *)(param_1 + 0x18);
          local_130 = 0;
          uStack_188 = 0;
          uStack_178 = 0;
          uStack_168 = 0;
          local_170 = 0;
          uStack_158 = 0;
          uStack_160 = 0;
          uStack_148 = 0;
          local_150 = 0;
          uStack_138 = 0;
          uStack_140 = 0;
          uStack_198 = 0;
          local_1a0 = 0;
          uVar3 = FUN_001546d4(uVar4,&local_1a0,&local_e0);
          if ((int)uVar3 != 0) {
            uVar3 = 0;
            if ((ABS(local_d4) != INFINITY) && (!NAN(ABS(local_d4)))) {
              uVar3 = 0;
              if ((ABS(fStack_d0) != INFINITY) &&
                 ((((!NAN(ABS(fStack_d0)) && (0.0 <= local_d4)) && (0.0 <= fStack_d0)) &&
                  ((local_d4 < 200.0 != NAN(local_d4) && (fStack_d0 < 200.0 != NAN(fStack_d0)))))))
              {
                local_1a8 = 0;
                uStack_1b0 = 0;
                local_1b8 = 0;
                uStack_1c0 = 0;
                local_1c8 = 0;
                uStack_1d0 = 0;
                local_1d8 = 0;
                uStack_1e0 = 0;
                local_1e8 = 0;
                uStack_1f0 = 0;
                local_1f8 = 0;
                local_200 = DAT_0010e720;
                local_208 = param_1;
                uVar3 = FUN_00156a78(param_1,3,&local_200);
                if ((int)uVar3 != 0) {
                  uStack_5c = *param_3;
                  local_60 = *param_2;
                  pcStack_220 = FUN_001428fc;
                  local_218 = FUN_00156b60;
                  pcStack_210 = FUN_00156ba0;
                  local_68 = CONCAT44((int)(fStack_d0 * 300.0),(int)(local_d4 * 300.0));
                  local_228 = &local_208;
                  uVar3 = FUN_0018a084(&local_228,*(undefined8 *)(param_1 + 0x10),&local_60,
                                       &local_68);
                  if ((int)uVar3 == 1) {
                    *param_2 = (undefined4)local_68;
                    *param_3 = local_68._4_4_;
                  }
                  else {
                    if ((int)uVar3 == -1) {
                      FUN_001417c8("fatal","held_xy_restore_unverified",0);
                    /* WARNING: Subroutine does not return */
                      abort();
                    }
                    uVar3 = 0;
                  }
                }
              }
            }
          }
        }
        else {
          uVar3 = (ulong)((int)uVar4 == 0 && DAT_002149c8 == 0);
        }
      }
    }
  }
  if (*(long *)(lVar1 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar3);
}

/* ===== FUN_00156950 @ 00156950 [libNexusEvasionRuntime69252.so] ===== */

void FUN_00156950(long param_1,undefined8 param_2,undefined4 *param_3,undefined4 *param_4)

{
  long lVar1;
  int iVar2;
  undefined8 uVar3;
  long *local_98;
  code *pcStack_90;
  code *local_88;
  code *pcStack_80;
  long local_78;
  undefined8 uStack_70;
  undefined8 local_68;
  undefined8 local_60;
  undefined4 local_58;
  undefined4 local_54;
  undefined1 auStack_50 [8];
  long local_48;
  
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  uVar3 = FUN_00156d04(param_1,param_2,&local_58,&local_60);
  if (((int)uVar3 != 0) &&
     (uVar3 = FUN_001428fc(uVar3,param_1 + 0xfac,auStack_50,8), (int)uVar3 != 0)) {
    local_98 = &local_78;
    local_68 = local_60;
    pcStack_90 = FUN_001428fc;
    local_88 = FUN_00154ec4;
    pcStack_80 = FUN_00156f50;
    local_78 = param_1;
    uStack_70 = param_2;
    iVar2 = FUN_0018a084(&local_98,param_1,auStack_50,&local_58);
    if (iVar2 == 1) {
      if (param_3 != (undefined4 *)0x0) {
        *param_3 = local_58;
      }
      if (param_4 != (undefined4 *)0x0) {
        *param_4 = local_54;
      }
      uVar3 = 1;
      DAT_00214d90 = DAT_00214d90 + 1;
    }
    else {
      if (iVar2 == -1) {
        FUN_001417c8("fatal","ball_xy_restore_unverified",0);
                    /* WARNING: Subroutine does not return */
        abort();
      }
      uVar3 = 0;
    }
  }
  if (*(long *)(lVar1 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar3);
}

/* ===== FUN_00156d04 @ 00156d04 [libNexusEvasionRuntime69252.so] ===== */

void FUN_00156d04(undefined8 param_1,undefined8 param_2,int *param_3,long *param_4)

{
  long lVar1;
  bool bVar2;
  int iVar3;
  ulong uVar4;
  float fVar5;
  float fVar6;
  undefined4 uVar7;
  float fVar8;
  undefined1 auStack_98 [4];
  int local_94;
  int local_90;
  int local_6c;
  undefined4 local_64;
  float local_60;
  int local_54;
  char *local_50;
  long local_48;
  
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  local_50 = "ballAssistEnabled";
  local_54 = -1;
  if (((((int)DAT_00214938 == 1) && (DAT_00214930 != (code *)0x0)) &&
      (iVar3 = (*DAT_00214930)(&local_50,auStack_98,1,param_4,&local_54), iVar3 == 1)) &&
     (((*param_4 != 0 && (local_54 == 0)) && ((local_90 == 2 && (local_94 == 1)))))) {
    uVar4 = FUN_001550fc();
    if (((int)uVar4 != 0) && (uVar4 = FUN_00150bf0(param_1,param_2), (int)uVar4 != 0)) {
      iVar3 = FUN_00189c88(DAT_001e0978,param_2,FUN_001428fc,0,auStack_98);
      uVar4 = 0;
      if ((iVar3 != 0) &&
         ((local_6c != 0 &&
          (uVar4 = FUN_001428fc(0,DAT_0020f728 + 0xc4,&local_50,4), (int)uVar4 != 0)))) {
        iVar3 = FUN_001428fc(uVar4,DAT_0020f728 + 200,&local_54,4);
        uVar4 = 0;
        if ((iVar3 != 0) &&
           ((((4 < (int)local_50 && ((int)local_50 < 0x79)) && (4 < local_54)) && (local_54 < 0x79))
           )) {
          uVar4 = 0;
          fVar5 = ((float)local_54 + -2.25) - local_60;
          uVar7 = NEON_fnmsub((float)(int)local_50 + -1.0,0x3f000000,local_64);
          fVar8 = (float)NEON_fmadd(uVar7,uVar7,fVar5 * fVar5);
          if (((ABS(fVar8) != INFINITY) && (!NAN(ABS(fVar8)))) && (1e-06 <= fVar8)) {
            fVar6 = (float)NEON_fmadd(uVar7,12.0 / SQRT(fVar8),local_64);
            fVar5 = (float)NEON_fmadd(fVar5,12.0 / SQRT(fVar8),local_60);
            iVar3 = (int)(fVar5 * 300.0);
            bVar2 = 120000 < (int)(fVar6 * 300.0) + 60000U;
            *param_3 = (int)(fVar6 * 300.0);
            param_3[1] = iVar3;
            uVar4 = (ulong)(((!bVar2 && iVar3 != -0xea61) && (bVar2 || -0xea62 < iVar3)) &&
                           iVar3 < 0xea61);
          }
        }
      }
    }
  }
  else {
    uVar4 = 0;
  }
  if (*(long *)(lVar1 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar4);
}

/* ===== FUN_00156f50 @ 00156f50 [libNexusEvasionRuntime69252.so] ===== */

void FUN_00156f50(undefined8 *param_1)

{
  long lVar1;
  int iVar2;
  ulong uVar3;
  long local_50;
  int local_44;
  undefined1 auStack_40 [4];
  int local_3c;
  int local_38;
  char *local_30;
  long local_28;
  
  lVar1 = tpidr_el0;
  local_28 = *(long *)(lVar1 + 0x28);
  uVar3 = FUN_001550fc();
  if ((int)uVar3 != 0) {
    local_44 = -1;
    local_30 = "ballAssistEnabled";
    if ((((((int)DAT_00214938 == 1) && (DAT_00214930 != (code *)0x0)) &&
         (iVar2 = (*DAT_00214930)(&local_30,auStack_40,1,&local_50,&local_44), iVar2 == 1)) &&
        ((local_50 != 0 && (local_44 == 0)))) &&
       ((local_38 == 2 && ((local_3c == 1 && (local_50 == param_1[2])))))) {
      iVar2 = FUN_00150bf0(*param_1,param_1[1]);
      uVar3 = (ulong)(iVar2 != 0);
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

/* ===== FUN_00157358 @ 00157358 [libNexusEvasionRuntime69252.so] ===== */

void FUN_00157358(long param_1,long param_2)

{
  long lVar1;
  int iVar2;
  undefined8 uVar3;
  long local_60;
  int local_54;
  undefined4 local_50;
  int local_4c;
  int local_48;
  char *local_40;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  local_40 = "ballAssistEnabled";
  local_54 = -1;
  if ((((((int)DAT_00214938 == 1) && (DAT_00214930 != (code *)0x0)) &&
       (iVar2 = (*DAT_00214930)(&local_40,&local_50,1,&local_60,&local_54), iVar2 == 1)) &&
      ((local_60 != 0 && (local_54 == 0)))) && ((local_48 == 2 && (local_4c == 1)))) {
    uVar3 = FUN_00156950(*(undefined8 *)(param_1 + 0x10),*(undefined8 *)(param_1 + 0x18),0,0);
  }
  else if ((DAT_002149d4 == 0) && ((DAT_002149c4 == 0 || (*(int *)(param_2 + 4) != 2)))) {
    uVar3 = 1;
  }
  else {
    local_50 = *(undefined4 *)(param_2 + 0x30);
    local_40 = (char *)CONCAT44(local_40._4_4_,*(undefined4 *)(param_2 + 0x34));
    uVar3 = FUN_00156570(param_1,&local_50,&local_40);
  }
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar3);
}

/* ===== FUN_001574e0 @ 001574e0 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_001574e0(undefined8 *param_1,undefined8 *param_2)

{
  char cVar1;
  bool bVar2;
  long lVar3;
  char cVar4;
  int iVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  long local_60;
  int local_54;
  char *local_50;
  undefined4 local_48;
  int iStack_44;
  int local_40;
  long local_38;
  
  lVar3 = tpidr_el0;
  local_38 = *(long *)(lVar3 + 0x28);
  local_50 = "ballAssistEnabled";
  local_54 = -1;
  if (((((int)DAT_00214938 == 1) && (DAT_00214930 != (code *)0x0)) &&
      (iVar5 = (*DAT_00214930)(&local_50,&local_48,1,&local_60,&local_54), iVar5 == 1)) &&
     (((local_60 != 0 && (local_54 == 0)) && ((local_40 == 2 && (iStack_44 == 1)))))) {
    do {
      cVar4 = DAT_001cfe94;
      cVar1 = '\x01';
      bVar2 = (bool)ExclusiveMonitorPass(0x1cfe94,0x10);
      if (bVar2) {
        _DAT_001cfe94 = CONCAT31(DAT_001cfe94_1,1);
        cVar1 = ExclusiveMonitorsStatus();
      }
    } while (cVar1 != '\0');
    if (cVar4 == '\0') {
      uVar6 = FUN_00156d04(param_1[6],param_1[7],&local_48,&local_50);
      if ((int)uVar6 == 0) {
        _DAT_001cfe94 = 0;
      }
      else {
        uVar7 = param_1[2];
        uVar9 = param_1[0xc];
        uVar8 = param_1[0xb];
        DAT_002159e8 = local_50;
        uVar6 = 1;
        param_2[3] = param_1[3];
        param_2[2] = uVar7;
        param_2[5] = uVar9;
        param_2[4] = uVar8;
        uVar8 = _UNK_00112948;
        uVar7 = _DAT_00112940;
        param_2[6] = local_50;
        param_2[7] = CONCAT44(iStack_44,local_48);
        param_2[1] = uVar8;
        *param_2 = uVar7;
        DAT_002158f8 = 1;
        _DAT_00215978 = param_1[0xf];
        _DAT_00215970 = param_1[0xe];
        _DAT_00215988 = param_1[0x11];
        _DAT_00215980 = param_1[0x10];
        DAT_00215990 = param_1[0x12];
        _DAT_00215968 = param_1[0xd];
        _DAT_00215960 = param_1[0xc];
        _DAT_00215948 = param_1[9];
        _DAT_00215940 = param_1[8];
        _DAT_00215958 = param_1[0xb];
        _DAT_00215950 = param_1[10];
        _DAT_00215928 = param_1[5];
        _DAT_00215920 = param_1[4];
        _DAT_00215938 = param_1[7];
        _DAT_00215930 = param_1[6];
        _DAT_00215908 = param_1[1];
        _DAT_00215900 = *param_1;
        _DAT_00215918 = param_1[3];
        _DAT_00215910 = param_1[2];
        DAT_002159c0 = param_2[5];
        DAT_002159b8 = param_2[4];
        DAT_002159d0 = param_2[7];
        DAT_002159c8 = param_2[6];
        DAT_002159a0 = param_2[1];
        DAT_00215998 = *param_2;
        DAT_002159b0 = param_2[3];
        DAT_002159a8 = param_2[2];
        uRam00000000002159e0 = param_1[7];
        _DAT_002159d8 = param_1[6];
      }
      goto LAB_001575a4;
    }
  }
  uVar6 = 0;
LAB_001575a4:
  if (*(long *)(lVar3 + 0x28) != local_38) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar6);
  }
  return;
}

