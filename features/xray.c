/*
 * X-Ray / ShadowX — Feature
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
 * Notes: ShadowX subsystem: nexus_shadowx_set_feature, target cycling, name display.
 */

/* ===== nexus_shadowx_set_feature @ 0011a72c [libNexusEvasion69252.so] ===== */

bool nexus_shadowx_set_feature(undefined8 param_1,undefined8 param_2)

{
  int iVar1;
  
  iVar1 = FUN_0010ec90(param_1,param_2,0);
  return iVar1 == 1;
}

/* ===== nexus_shadowx_set_param @ 0011a74c [libNexusEvasion69252.so] ===== */

bool nexus_shadowx_set_param(undefined8 param_1,undefined8 param_2)

{
  int iVar1;
  
  iVar1 = FUN_0010ec90(param_1,param_2,1);
  return iVar1 == 1;
}

/* ===== FUN_00164c78 @ 00164c78 [libNexusEvasionRuntime69252.so] ===== */

void FUN_00164c78(undefined8 *param_1)

{
  long lVar1;
  undefined8 uVar2;
  int iVar3;
  undefined8 uVar4;
  int local_74;
  long local_70;
  undefined1 auStack_68 [4];
  int local_64;
  int local_60;
  int local_58;
  int local_54;
  int local_4c;
  int local_40;
  long local_38;
  
  lVar1 = tpidr_el0;
  local_38 = *(long *)(lVar1 + 0x28);
  uVar4 = 0;
  local_70 = 0;
  local_74 = -1;
  if (((int)DAT_00214938 == 1) && (DAT_00214930 != (code *)0x0)) {
    iVar3 = pthread_once((pthread_once_t *)&DAT_0021ca04,FUN_00164598);
    if ((DAT_0021ca08 == (code *)0x0) || (iVar3 = (*DAT_0021ca08)(iVar3), iVar3 != 1)) {
      uVar4 = 0;
    }
    else {
      iVar3 = (*DAT_00214930)(&PTR_s_isXrayEnabled_001c3368,auStack_68,4,&local_70,&local_74);
      uVar4 = 0;
      if (((((iVar3 == 1) && (local_70 != 0)) && (local_74 == 0)) &&
          (((uVar4 = 0, local_60 == 2 && (local_54 == 2)) &&
           ((-1 < local_64 && ((local_64 < 2 && (-1 < local_58)))))))) &&
         ((local_58 < 2 &&
          ((((0 < local_4c && (local_4c < 3)) && (-1 < local_40)) && (local_40 < 2)))))) {
        uVar4 = 1;
        *param_1 = 0;
        param_1[1] = local_70;
        *(int *)(param_1 + 2) = local_64;
        *(int *)((long)param_1 + 0x14) = local_40;
        uVar2 = DAT_0010e5c0;
        *(int *)(param_1 + 3) = local_4c;
        *(int *)((long)param_1 + 0x1c) = local_58;
        param_1[4] = 0;
        param_1[5] = 0;
        param_1[6] = uVar2;
        param_1[8] = 0;
        param_1[7] = 0;
        param_1[10] = 0;
        param_1[9] = 0;
        param_1[0xc] = 0;
        param_1[0xb] = 0;
        param_1[0xe] = 0;
        param_1[0xd] = 0;
      }
    }
  }
  if (*(long *)(lVar1 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar4);
}

