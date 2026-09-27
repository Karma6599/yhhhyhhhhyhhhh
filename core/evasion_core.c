/*
 * evasion_core — Core plumbing
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
 * Notes: Evasion module init/bind/register plumbing.
 */

/* ===== nexus_evasion_set_feature @ 0010ec88 [libNexusEvasion69252.so] ===== */

void nexus_evasion_set_feature(undefined8 param_1,undefined8 param_2)

{
  FUN_0010ec90(param_1,param_2,0);
  return;
}

/* ===== nexus_evasion_set_parameter @ 0010f444 [libNexusEvasion69252.so] ===== */

void nexus_evasion_set_parameter(undefined8 param_1,undefined8 param_2)

{
  FUN_0010ec90(param_1,param_2,1);
  return;
}

/* ===== nexus_evasion_intercept @ 0011a2e0 [libNexusEvasion69252.so] ===== */

undefined4
nexus_evasion_intercept
          (float param_1,float param_2,float param_3,float param_4,float param_5,float *param_6)

{
  bool bVar1;
  bool bVar2;
  bool bVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  undefined4 uVar8;
  
  uVar8 = 0;
  if ((((((ABS(param_4) != INFINITY) && (!NAN(ABS(param_4)))) && (ABS(param_3) != INFINITY)) &&
       ((!NAN(ABS(param_3)) && (ABS(param_2) != INFINITY)))) &&
      ((!NAN(ABS(param_2)) && ((ABS(param_1) != INFINITY && (!NAN(ABS(param_1)))))))) &&
     ((param_6 != (float *)0x0 &&
      (((uVar8 = 0, 0.0 < param_5 && (ABS(param_5) != INFINITY)) && (!NAN(ABS(param_5)))))))) {
    fVar4 = 0.0;
    fVar7 = (float)NEON_fmadd(param_1,param_1,param_2 * param_2);
    if (0.0001 <= fVar7) {
      uVar8 = NEON_fmadd(param_3,param_3,param_4 * param_4);
      fVar4 = (float)NEON_fmadd(param_1,param_3,param_2 * param_4);
      fVar5 = (float)NEON_fmsub(param_5,param_5,uVar8);
      fVar4 = fVar4 + fVar4;
      if ((fVar5 == -0.0005 || fVar5 < -0.0005 != NAN(fVar5)) || (0.0005 <= fVar5)) {
        fVar7 = (float)NEON_fmadd(fVar4,fVar4,fVar7 * fVar5 * -4.0);
        if (fVar7 < 0.0) {
          return 0;
        }
        fVar6 = (-fVar4 - SQRT(fVar7)) / (fVar5 + fVar5);
        fVar7 = (SQRT(fVar7) - fVar4) / (fVar5 + fVar5);
        fVar4 = fVar6;
        if (fVar7 <= fVar6) {
          fVar4 = fVar7;
        }
        if (fVar7 == 0.0 || fVar7 < 0.0 != NAN(fVar7)) {
          fVar4 = fVar6;
        }
        if (fVar6 == 0.0 || fVar6 < 0.0 != NAN(fVar6)) {
          fVar4 = fVar7;
        }
      }
      else {
        if (fVar4 < -0.0005 == NAN(fVar4)) {
          return 0;
        }
        fVar4 = -fVar7 / fVar4;
      }
      bVar1 = false;
      bVar2 = false;
      bVar3 = false;
      if (0.0 <= fVar4) {
        bVar1 = false;
        bVar2 = false;
        bVar3 = true;
        if (!NAN(fVar4)) {
          bVar1 = fVar4 < 1.55;
          bVar2 = fVar4 == 1.55;
          bVar3 = false;
        }
      }
      if (!bVar2 && bVar1 == bVar3) {
        return 0;
      }
    }
    uVar8 = 0;
    if ((ABS(fVar4) != INFINITY) && (!NAN(ABS(fVar4)))) {
      uVar8 = 1;
      *param_6 = fVar4;
    }
  }
  return uVar8;
}

/* ===== nexus_evasion_segment_distance @ 0011a478 [libNexusEvasion69252.so] ===== */

undefined4
nexus_evasion_segment_distance
          (float param_1,float param_2,float param_3,float param_4,float param_5,float param_6,
          float *param_7,float *param_8,float *param_9,float *param_10)

{
  undefined4 uVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  
  uVar1 = 0;
  if ((((((ABS(param_6) != INFINITY) && (!NAN(ABS(param_6)))) && (ABS(param_5) != INFINITY)) &&
       (((!NAN(ABS(param_5)) && (ABS(param_4) != INFINITY)) &&
        ((!NAN(ABS(param_4)) && ((ABS(param_3) != INFINITY && (!NAN(ABS(param_3)))))))))) &&
      (ABS(param_2) != INFINITY)) &&
     ((((!NAN(ABS(param_2)) && (ABS(param_1) != INFINITY)) && (!NAN(ABS(param_1)))) &&
      (param_7 != (float *)0x0)))) {
    param_6 = param_6 - param_4;
    param_5 = param_5 - param_3;
    fVar6 = (float)NEON_fmadd(param_5,param_5,param_6 * param_6);
    fVar5 = 0.0;
    if (fVar6 != 1e-06 && fVar6 < 1e-06 == NAN(fVar6)) {
      fVar5 = (float)NEON_fmadd(param_1 - param_3,param_5,(param_2 - param_4) * param_6);
      fVar5 = fVar5 / fVar6;
      if (fVar5 <= 0.0) {
        fVar5 = 0.0;
      }
      if (fVar5 != 1.0 && fVar5 < 1.0 == NAN(fVar5)) {
        fVar5 = 1.0;
      }
    }
    fVar4 = (float)NEON_fmadd(param_6,fVar5,param_4);
    fVar3 = (float)NEON_fmadd(param_5,fVar5,param_3);
    uVar1 = 0;
    fVar6 = (float)NEON_fmadd(param_1 - fVar3,param_1 - fVar3,(param_2 - fVar4) * (param_2 - fVar4))
    ;
    fVar2 = ABS(SQRT(fVar6));
    if ((fVar2 != INFINITY) && (!NAN(fVar2))) {
      *param_7 = SQRT(fVar6);
      if (param_8 != (float *)0x0) {
        *param_8 = fVar3;
      }
      if (param_9 != (float *)0x0) {
        *param_9 = fVar4;
      }
      uVar1 = 1;
      if (param_10 != (float *)0x0) {
        *param_10 = fVar5;
      }
    }
  }
  return uVar1;
}

/* ===== nexus_evasion_menu_backend_v1 @ 0011c4a8 [libNexusEvasion69252.so] ===== */

undefined8 nexus_evasion_menu_backend_v1(int *param_1)

{
  undefined8 uVar1;
  ulong uVar2;
  long *plVar3;
  ulong uVar4;
  
  if (((param_1 != (int *)0x0) && (*param_1 == 1)) && (param_1[1] == 0x40)) {
    uVar2 = 0;
    plVar3 = &DAT_00122d38;
    param_1[0xe] = 0;
    uVar1 = DAT_001047e0;
    param_1[0xf] = 0;
    param_1[0xc] = 0;
    param_1[0xd] = 0;
    param_1[10] = 0;
    param_1[0xb] = 0;
    param_1[8] = 0;
    param_1[9] = 0;
    param_1[6] = 0;
    param_1[7] = 0;
    param_1[4] = 0;
    param_1[5] = 0;
    param_1[2] = 0;
    param_1[3] = 0;
    *(undefined8 *)param_1 = uVar1;
    do {
      if (*plVar3 != 0) {
        uVar4 = uVar2 >> 6 & 0x3ffffff;
        *(ulong *)(param_1 + uVar4 * 2 + 4) =
             *(ulong *)(param_1 + uVar4 * 2 + 4) | 1L << (uVar2 & 0x3f);
      }
      uVar2 = uVar2 + 1;
      plVar3 = plVar3 + 4;
    } while (uVar2 != 0xa8);
    *(code **)(param_1 + 10) = FUN_0011c560;
    *(code **)(param_1 + 0xc) = FUN_0011c72c;
    *(code **)(param_1 + 0xe) = FUN_0011c828;
    return 1;
  }
  return 4;
}

/* ===== FUN_0011c72c @ 0011c72c [libNexusEvasion69252.so] ===== */

void FUN_0011c72c(undefined8 param_1,int *param_2)

{
  long lVar1;
  int iVar2;
  int iVar3;
  undefined8 uVar4;
  ulong uVar5;
  undefined1 auStack_a8 [96];
  long local_48;
  
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  if ((((param_2 == (int *)0x0) || (*param_2 != 0x40)) ||
      (uVar5 = (ulong)(uint)param_2[1], 0xa7 < (uint)param_2[1])) || (*(long *)(param_2 + 0xc) != 0)
     ) {
LAB_0011c778:
    iVar2 = 4;
  }
  else {
    if ((&DAT_00122d38)[uVar5 * 4] != 0) {
      if (param_2[2] != *(int *)(&DAT_00122d4c + uVar5 * 0x20)) goto LAB_0011c778;
      uVar4 = FUN_0011c914(&DAT_00122d38 + uVar5 * 4,param_2[0xe],auStack_a8);
      iVar2 = nexus_evasion_get_port_state();
      if (iVar2 != 3) {
        if (*(int *)(&DAT_00122d48 + uVar5 * 0x20) == 0) {
          iVar3 = nexus_evasion_set_feature(uVar4,param_2[3]);
        }
        else {
          iVar3 = nexus_evasion_set_parameter();
        }
        iVar2 = iVar3;
        if (iVar3 != 2) {
          iVar2 = 4;
        }
        if (iVar3 == 1) {
          iVar2 = 1;
        }
        goto LAB_0011c77c;
      }
    }
    iVar2 = 0;
  }
LAB_0011c77c:
  if (*(long *)(lVar1 + 0x28) != local_48) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(iVar2);
  }
  return;
}

/* ===== JNI_OnLoad @ 0011c9d4 [libNexusEvasion69252.so] ===== */

undefined4 JNI_OnLoad(long *param_1)

{
  long lVar1;
  int iVar2;
  long lVar3;
  long lVar4;
  long lVar5;
  char *pcVar6;
  undefined4 uVar7;
  long *local_50;
  long local_48;
  
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  uVar7 = 0x10006;
  local_50 = (long *)0x0;
  iVar2 = (**(code **)(*param_1 + 0x30))(param_1,&local_50,0x10006);
  if ((iVar2 == 0) && (local_50 != (long *)0x0)) {
    lVar3 = (**(code **)(*local_50 + 0x30))
                      (local_50,"nexus/brawl/evasion/AdvancedEvasionController");
    if (lVar3 == 0) {
      pcVar6 = "facade_class";
      goto LAB_0011cc14;
    }
    lVar4 = (**(code **)(*local_50 + 0x30))(local_50,"nexus/evasion/EvasionBackend");
    if (lVar4 == 0) {
      FUN_0011cd28(param_1,"api_class");
    }
    else {
      lVar5 = (**(code **)(*local_50 + 0x388))
                        (local_50,lVar3,PTR_s_initNativeHooks_00129038,PTR_DAT_00129040);
      if ((((lVar5 == 0) ||
           (lVar5 = (**(code **)(*local_50 + 0x388))
                              (local_50,lVar3,PTR_s_setEvasionVersion_00129050,PTR_DAT_00129058),
           lVar5 == 0)) ||
          (lVar5 = (**(code **)(*local_50 + 0x388))
                             (local_50,lVar3,PTR_s_setNativeCrashDownloadFd_00129068,
                              PTR_DAT_00129070), lVar5 == 0)) ||
         (lVar5 = (**(code **)(*local_50 + 0x388))
                            (local_50,lVar3,PTR_s_setNativeCrashPrivateFd_00129080,PTR_DAT_00129088)
         , lVar5 == 0)) {
        pcVar6 = "facade_signatures";
      }
      else {
        lVar5 = (**(code **)(*local_50 + 0x388))
                          (local_50,lVar4,PTR_s_setFeature_00129098,
                           PTR_s__Ljava_lang_String_I_I_001290a0);
        if ((((lVar5 == 0) ||
             (lVar5 = (**(code **)(*local_50 + 0x388))
                                (local_50,lVar4,PTR_s_setParameter_001290b0,
                                 PTR_s__Ljava_lang_String_I_I_001290b8), lVar5 == 0)) ||
            ((lVar5 = (**(code **)(*local_50 + 0x388))
                                (local_50,lVar4,PTR_s_getRequested_001290c8,
                                 PTR_s__Ljava_lang_String__I_001290d0), lVar5 == 0 ||
             ((lVar5 = (**(code **)(*local_50 + 0x388))
                                 (local_50,lVar4,PTR_s_getEffective_001290e0,
                                  PTR_s__Ljava_lang_String__I_001290e8), lVar5 == 0 ||
              (lVar5 = (**(code **)(*local_50 + 0x388))
                                 (local_50,lVar4,PTR_s_getPortState_001290f8,
                                  PTR_s__Ljava_lang_String__I_00129100), lVar5 == 0)))))) ||
           (lVar5 = (**(code **)(*local_50 + 0x388))
                              (local_50,lVar4,PTR_s_getDiagnostics_00129110,
                               PTR_s___Ljava_lang_String__00129118), lVar5 == 0)) {
          pcVar6 = "api_signatures";
        }
        else {
          iVar2 = (**(code **)(*local_50 + 0x6b8))(local_50,lVar4,&PTR_s_setFeature_00129098,6);
          if (iVar2 == 0) {
            iVar2 = (**(code **)(*local_50 + 0x6b8))
                              (local_50,lVar3,&PTR_s_initNativeHooks_00129038,4);
            if (iVar2 == 0) {
              (**(code **)(*local_50 + 0xb8))(local_50,lVar3);
              (**(code **)(*local_50 + 0xb8))(local_50,lVar4);
              nexus_evasion_initialize();
              goto LAB_0011cc78;
            }
            (**(code **)(*local_50 + 0x6c0))(local_50,lVar4);
            pcVar6 = "facade_registration";
          }
          else {
            pcVar6 = "api_registration";
          }
        }
      }
      FUN_0011cd28(param_1,pcVar6);
      (**(code **)(*local_50 + 0xb8))(local_50,lVar3);
      lVar3 = lVar4;
    }
    (**(code **)(*local_50 + 0xb8))(local_50,lVar3);
  }
  else {
    pcVar6 = "jni_environment";
LAB_0011cc14:
    FUN_0011cd28(param_1,pcVar6);
  }
  uVar7 = 0xffffffff;
LAB_0011cc78:
  if (*(long *)(lVar1 + 0x28) == local_48) {
    return uVar7;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0011d138 @ 0011d138 [libNexusEvasion69252.so] ===== */

undefined8 FUN_0011d138(void)

{
  nexus_evasion_initialize();
  return 2;
}

/* ===== FUN_0011d1b0 @ 0011d1b0 [libNexusEvasion69252.so] ===== */

undefined4 FUN_0011d1b0(long *param_1,undefined8 param_2,long param_3,undefined4 param_4)

{
  undefined4 uVar1;
  long lVar2;
  
  if (param_3 == 0) {
    uVar1 = 0xffffffff;
  }
  else {
    lVar2 = (**(code **)(*param_1 + 0x548))(param_1,param_3,0);
    if (lVar2 == 0) {
      uVar1 = 0xfffffffe;
    }
    else {
      uVar1 = nexus_evasion_set_feature(lVar2,param_4);
      (**(code **)(*param_1 + 0x550))(param_1,param_3,lVar2);
    }
  }
  return uVar1;
}

/* ===== FUN_0011d234 @ 0011d234 [libNexusEvasion69252.so] ===== */

undefined4 FUN_0011d234(long *param_1,undefined8 param_2,long param_3,undefined4 param_4)

{
  undefined4 uVar1;
  long lVar2;
  
  if (param_3 == 0) {
    uVar1 = 0xffffffff;
  }
  else {
    lVar2 = (**(code **)(*param_1 + 0x548))(param_1,param_3,0);
    if (lVar2 == 0) {
      uVar1 = 0xfffffffe;
    }
    else {
      uVar1 = nexus_evasion_set_parameter(lVar2,param_4);
      (**(code **)(*param_1 + 0x550))(param_1,param_3,lVar2);
    }
  }
  return uVar1;
}

/* ===== FUN_0011d2b8 @ 0011d2b8 [libNexusEvasion69252.so] ===== */

undefined4 FUN_0011d2b8(long *param_1,undefined8 param_2,long param_3)

{
  undefined4 uVar1;
  long lVar2;
  
  if ((param_3 == 0) || (lVar2 = (**(code **)(*param_1 + 0x548))(param_1,param_3,0), lVar2 == 0)) {
    uVar1 = 0x80000000;
  }
  else {
    uVar1 = nexus_evasion_get_requested();
    (**(code **)(*param_1 + 0x550))(param_1,param_3,lVar2);
  }
  return uVar1;
}

/* ===== FUN_0011d32c @ 0011d32c [libNexusEvasion69252.so] ===== */

undefined4 FUN_0011d32c(long *param_1,undefined8 param_2,long param_3)

{
  undefined4 uVar1;
  long lVar2;
  
  if ((param_3 == 0) || (lVar2 = (**(code **)(*param_1 + 0x548))(param_1,param_3,0), lVar2 == 0)) {
    uVar1 = 0x80000000;
  }
  else {
    uVar1 = nexus_evasion_get_effective();
    (**(code **)(*param_1 + 0x550))(param_1,param_3,lVar2);
  }
  return uVar1;
}

/* ===== FUN_0011d3a0 @ 0011d3a0 [libNexusEvasion69252.so] ===== */

undefined4 FUN_0011d3a0(long *param_1,undefined8 param_2,long param_3)

{
  undefined4 uVar1;
  long lVar2;
  
  if ((param_3 == 0) || (lVar2 = (**(code **)(*param_1 + 0x548))(param_1,param_3,0), lVar2 == 0)) {
    uVar1 = 0xffffffff;
  }
  else {
    uVar1 = nexus_evasion_get_port_state();
    (**(code **)(*param_1 + 0x550))(param_1,param_3,lVar2);
  }
  return uVar1;
}

/* ===== FUN_0011d414 @ 0011d414 [libNexusEvasion69252.so] ===== */

void FUN_0011d414(long *param_1)

{
  long lVar1;
  undefined1 auStack_438 [1024];
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  nexus_evasion_diagnostics(auStack_438,0x400);
  (**(code **)(*param_1 + 0x538))(param_1,auStack_438);
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== nexus_evasion_set_feature @ 0011d4d0 [libNexusEvasion69252.so] ===== */

void nexus_evasion_set_feature(void)

{
  (*(code *)PTR_nexus_evasion_set_feature_00124f28)();
  return;
}

/* ===== nexus_evasion_set_parameter @ 0011d510 [libNexusEvasion69252.so] ===== */

void nexus_evasion_set_parameter(void)

{
  (*(code *)PTR_nexus_evasion_set_parameter_00124f48)();
  return;
}

/* ===== nexus_evasion_initialize @ 0011d550 [libNexusEvasion69252.so] ===== */

void nexus_evasion_initialize(void)

{
  (*(code *)PTR_nexus_evasion_initialize_00124f68)();
  return;
}

/* ===== nexus_evasion_restore_v1 @ 0011d560 [libNexusEvasion69252.so] ===== */

void nexus_evasion_restore_v1(void)

{
  (*(code *)PTR_nexus_evasion_restore_v1_00124f70)();
  return;
}

/* ===== nexus_evasion_diagnostics @ 0011d5e0 [libNexusEvasion69252.so] ===== */

void nexus_evasion_diagnostics(void)

{
  (*(code *)PTR_nexus_evasion_diagnostics_00124fb0)();
  return;
}

/* ===== FUN_00147aec @ 00147aec [libNexusEvasionRuntime69252.so] ===== */

void FUN_00147aec(int param_1)

{
  long lVar1;
  int iVar2;
  char *pcVar3;
  char *pcVar4;
  char *local_70;
  long local_68;
  code *pcStack_60;
  code *local_58;
  code *pcStack_50;
  long local_48;
  
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  if ((param_1 == 0) || (iVar2 = FUN_001419f8(), iVar2 == 0)) goto LAB_00147b9c;
  DAT_00228e40 = (code *)dlsym(DAT_0020d0f0,"nexus_evasion_register_periodic_v1");
  DAT_002208a8 = dlsym(DAT_0020d0f0,"nexus_evasion_snapshot_keys_v1");
  if ((DAT_00228e40 != (code *)0x0) &&
     (((iVar2 = dladdr(DAT_00228e40,&local_70), iVar2 != 0 && (local_68 == DAT_0020c0e8)) &&
      (local_70 != (char *)0x0)))) {
    iVar2 = strcmp(local_70,&DAT_0020c0f0);
    if ((((iVar2 == 0) && (DAT_002208a8 != 0)) &&
        (iVar2 = dladdr(DAT_002208a8,&local_70), iVar2 != 0)) &&
       ((local_68 == DAT_0020c0e8 && (local_70 != (char *)0x0)))) {
      iVar2 = strcmp(local_70,&DAT_0020c0f0);
      pcVar3 = DAT_0010e668;
      if (iVar2 == 0) {
        DAT_00228e48 = DAT_001e0978;
        DAT_00228e58 = 0;
        DAT_00228e60 = FUN_0016acb8;
        DAT_00228e50 = DAT_0010e668;
        DAT_00228e68 = FUN_001428fc;
        DAT_00228e70 = FUN_00158260;
        iVar2 = FUN_001a16a0(&DAT_00228e48,&DAT_002208b0);
        if (iVar2 == 0) {
LAB_00147cb4:
          DAT_002208a0 = 0;
          if (DAT_00228e40 != (code *)0x0) {
            (*DAT_00228e40)(0);
          }
          pcVar3 = "periodic_refused";
          pcVar4 = "optional_periodic_adapter_refused";
        }
        else {
          DAT_002208a0 = 1;
          local_70 = pcVar3;
          local_68 = DAT_00209d00;
          pcStack_60 = FUN_001455a4;
          local_58 = FUN_0015d3c4;
          pcStack_50 = FUN_00166188;
          iVar2 = (*DAT_00228e40)(&local_70);
          if (iVar2 != 1) goto LAB_00147cb4;
          pcVar3 = "periodic_ready";
          pcVar4 = "pin_and_spray_native_commands_bound";
        }
        FUN_001417c8(pcVar3,pcVar4,
                     ",\"types\":[9,15],\"new_hooks\":0,\"native_calls_during_bind\":0");
        goto LAB_00147b9c;
      }
    }
  }
  DAT_00228e40 = (code *)0x0;
LAB_00147b9c:
  if (*(long *)(lVar1 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

