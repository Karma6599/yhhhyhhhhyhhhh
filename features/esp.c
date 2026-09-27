/*
 * ESP — Feature
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
 * Notes: Visual overlays driven from the GL hook layer; see also core/gl_hooks.c and outline shader patcher in features/outline.c.
 */

/* ===== FUN_00112b5c @ 00112b5c [libNexusEvasion69252.so] ===== */

char * FUN_00112b5c(char *param_1)

{
  int iVar1;
  
  if (param_1 != (char *)0x0) {
    iVar1 = strcmp(param_1,"espEnabled");
    if ((((iVar1 == 0) || (iVar1 = strcmp(param_1,"espShowTracer"), iVar1 == 0)) ||
        (iVar1 = strcmp(param_1,"espTeamFilterEnemies"), iVar1 == 0)) ||
       (iVar1 = strcmp(param_1,"espTeamFilterSelf"), iVar1 == 0)) {
      param_1 = (char *)0x1;
    }
    else {
      iVar1 = strcmp(param_1,"espTeamFilterTeammates");
      param_1 = (char *)(ulong)(iVar1 == 0);
    }
  }
  return param_1;
}

