/*
 * Prediction — Feature
 * Decompiled with Ghidra 11.3.2 (arm64 pseudocode) from: libNexusEvasion69252.so
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
 * Notes: Predictor versions selectable from the menu (aopPredictVersion).
 */

/* ===== FUN_0011359c @ 0011359c [libNexusEvasion69252.so] ===== */

void FUN_0011359c(undefined4 param_1)

{
  undefined **ppuVar1;
  int iVar2;
  undefined **ppuVar3;
  
  ppuVar1 = &PTR_s_cameraEnabled_001216c0;
  do {
    ppuVar3 = ppuVar1;
    iVar2 = strcmp("aopPredictEnabled",*ppuVar3);
    ppuVar1 = ppuVar3 + 5;
  } while (iVar2 != 0);
  FUN_0011b604(param_1,(&DAT_00129130)[*(int *)(ppuVar3 + 1)]);
  return;
}

/* ===== FUN_0011b604 @ 0011b604 [libNexusEvasion69252.so] ===== */

undefined8 FUN_0011b604(uint param_1,int param_2)

{
  undefined **ppuVar1;
  int iVar2;
  undefined **ppuVar3;
  
  ppuVar1 = &PTR_s_cameraEnabled_001216c0;
  do {
    ppuVar3 = ppuVar1;
    iVar2 = strcmp("aopTargetMode",*ppuVar3);
    ppuVar1 = ppuVar3 + 5;
  } while (iVar2 != 0);
  if (-1 < (int)(&DAT_00129130)[*(int *)(ppuVar3 + 1)]) {
    ppuVar1 = &PTR_s_cameraEnabled_001216c0;
    do {
      ppuVar3 = ppuVar1;
      iVar2 = strcmp("aopTargetMode",*ppuVar3);
      ppuVar1 = ppuVar3 + 5;
    } while (iVar2 != 0);
    if ((int)(&DAT_00129130)[*(int *)(ppuVar3 + 1)] < 3) {
      if (param_2 == 0) {
        return 1;
      }
      if ((param_1 >> 2 & 1) != 0) {
        ppuVar1 = &PTR_s_cameraEnabled_001216c0;
        do {
          ppuVar3 = ppuVar1;
          iVar2 = strcmp("aopPredictVersion",*ppuVar3);
          ppuVar1 = ppuVar3 + 5;
        } while (iVar2 != 0);
        if (0 < (int)(&DAT_00129130)[*(int *)(ppuVar3 + 1)]) {
          ppuVar1 = &PTR_s_cameraEnabled_001216c0;
          do {
            ppuVar3 = ppuVar1;
            iVar2 = strcmp("aopPredictVersion",*ppuVar3);
            ppuVar1 = ppuVar3 + 5;
          } while (iVar2 != 0);
          if ((int)(&DAT_00129130)[*(int *)(ppuVar3 + 1)] < 3) {
            return 1;
          }
        }
      }
    }
  }
  return 0;
}

