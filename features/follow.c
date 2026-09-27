/*
 * Follow — Feature
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
 * Notes: Ally following incl. closest-ally selection and team follow.
 */

/* ===== FUN_00185d10 @ 00185d10 [libNexusUI69252.so] ===== */

void FUN_00185d10(void)

{
  long lVar1;
  undefined *puVar2;
  int iVar3;
  undefined8 uVar4;
  code *pcVar5;
  long lVar6;
  undefined *puVar7;
  undefined8 *puVar8;
  long lVar9;
  int *piVar10;
  int local_80 [3];
  int local_74;
  char local_6b;
  long local_68;
  
  puVar2 = PTR_PTR_s_nexus_sx_spin_001a36f8;
  lVar1 = tpidr_el0;
  lVar6 = 0;
  local_68 = *(long *)(lVar1 + 0x28);
  puVar8 = (undefined8 *)((ulong)local_80 | 4);
  do {
    if ((&DAT_002848b0)[lVar6 * 8] != 0) {
      lVar9 = 0;
      piVar10 = (int *)(puVar2 + 0xc);
      do {
        iVar3 = FUN_0018ded4(*(undefined8 *)(piVar10 + -3));
        if (iVar3 == 0) {
          pcVar5 = (code *)(&DAT_002848b0)[lVar6 * 8];
          uVar4 = (&DAT_00284890)[lVar6 * 8];
          *puVar8 = 0;
          puVar8[1] = 0;
          *(undefined4 *)(puVar8 + 2) = 0;
          local_80[0] = 0x18;
          iVar3 = (*pcVar5)(uVar4,(int)lVar9 + 0x10000,local_80);
          if ((((iVar3 == 1) && (local_80[0] == 0x18)) && (local_6b != '\0')) &&
             (((piVar10[-1] <= local_74 && (local_74 <= *piVar10)) &&
              (*(int *)((long)&DAT_002846b0 + lVar9 * 4) != local_74)))) {
            *(int *)((long)&DAT_002846b0 + lVar9 * 4) = local_74;
            DAT_002846a0 = DAT_002846a0 + 1;
          }
        }
        lVar9 = lVar9 + 1;
        piVar10 = piVar10 + 6;
      } while (lVar9 != 0x76);
    }
    puVar7 = PTR_DAT_001a36f0;
    lVar6 = lVar6 + 1;
  } while (lVar6 != 8);
  FUN_0018df74(PTR_DAT_001a36f0 + 0x1998);
  lVar6 = 0x5b;
  do {
    if (lVar6 != 0) {
      FUN_0018df74(puVar7);
    }
    lVar6 = lVar6 + -1;
    puVar7 = puVar7 + 0x48;
  } while (lVar6 != -0x4d);
  if (*(long *)(lVar1 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0018d2a0 @ 0018d2a0 [libNexusUI69252.so] ===== */

undefined8 FUN_0018d2a0(long param_1,char *param_2,int param_3)

{
  int iVar1;
  long lVar2;
  int *piVar3;
  
  lVar2 = 0;
  piVar3 = (int *)(PTR_PTR_s_nexus_sx_spin_001a36f8 + 0xc);
  while (iVar1 = strcmp(*(char **)(piVar3 + -3),param_2), iVar1 != 0) {
    lVar2 = lVar2 + 1;
    piVar3 = piVar3 + 6;
    if (lVar2 == 0x76) {
      return 0;
    }
  }
  if ((int)lVar2 < 0) {
    return 0;
  }
  if (param_3 < piVar3[-1]) {
    return 0;
  }
  if (*piVar3 < param_3) {
    return 0;
  }
  *(int *)(param_1 + lVar2 * 4) = param_3;
  return 1;
}

