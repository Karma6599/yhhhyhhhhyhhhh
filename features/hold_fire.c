/*
 * Hold Fire — Feature
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
 * Notes: nexus_evasion_hold_* lease system for hold-to-shoot / hold-aim.
 */

/* ===== FUN_0010f934 @ 0010f934 [libNexusEvasion69252.so] ===== */

ulong FUN_0010f934(char *param_1)

{
  bool bVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  ulong uVar5;
  long lVar6;
  
  iVar2 = FUN_00112940();
  if (iVar2 == 0) {
    if (param_1 == (char *)0x0) {
      iVar2 = FUN_00112b5c(0);
      if (iVar2 == 0) {
LAB_0010fb74:
        iVar2 = strcmp(param_1,"isSpinEnabled");
        if (iVar2 == 0) {
          uVar5 = FUN_001158a0();
          return uVar5;
        }
        iVar2 = FUN_00112d44(param_1);
        if (-1 < iVar2) {
          uVar5 = FUN_0011b5d0(param_1,0);
          return uVar5;
        }
        uVar3 = FUN_001130c0(param_1);
        uVar4 = FUN_00113200();
        if (-1 < (int)uVar3) {
          if ((uVar4 >> (ulong)(uVar3 & 0x1f) & 1) == 0) {
            return 0;
          }
          iVar2 = FUN_0011359c();
LAB_0010fbdc:
          return (ulong)(iVar2 != 0);
        }
        lVar6 = FUN_0010f504(param_1);
        if (lVar6 == 0) {
          return 0;
        }
        if (*(int *)(lVar6 + 0xc) != 0) {
          return 0;
        }
        bVar1 = *(int *)(lVar6 + 0x20) == 0;
        goto LAB_0010fb6c;
      }
    }
    else {
      iVar2 = strcmp(param_1,"pinEnabled");
      if ((iVar2 == 0) || (iVar2 = strcmp(param_1,"sprayEnabled"), iVar2 == 0)) {
        if (DAT_00129450 == 0) {
          return 0;
        }
        if (DAT_00129468 == (code *)0x0) {
          return 0;
        }
        iVar2 = (*DAT_00129468)(0);
        if (iVar2 != 1) {
          return 0;
        }
        if (DAT_001293d0 != DAT_00129450) {
          return 0;
        }
        if (DAT_001293a8 != DAT_00129458) {
          return 0;
        }
        goto LAB_0010fb44;
      }
      iVar2 = FUN_00112b5c(param_1);
      if (iVar2 == 0) {
        iVar2 = strcmp(param_1,"speedExploitEnabled");
        if (iVar2 == 0) {
          return 1;
        }
        iVar2 = strcmp(param_1,"speedLocalMoveEnabled");
        if (iVar2 == 0) {
          return 1;
        }
        iVar2 = strcmp(param_1,"holdToShootEnabled");
        if (iVar2 != 0) {
          iVar2 = strcmp(param_1,"holdToShootAim");
          if ((iVar2 != 0) && (iVar2 = strcmp(param_1,"holdToShootRangeCheck"), iVar2 != 0))
          goto LAB_0010fb74;
          iVar2 = strcmp(param_1,"holdToShootAim");
          if ((iVar2 != 0) && (iVar2 = strcmp(param_1,"holdToShootRangeCheck"), iVar2 != 0)) {
            return 0;
          }
        }
        iVar2 = FUN_00117314(0);
        goto LAB_0010fbdc;
      }
    }
    if (DAT_00129428 == 0) {
      return 0;
    }
    if (DAT_00129440 == (code *)0x0) {
      return 0;
    }
    iVar2 = (*DAT_00129440)(0);
    if (iVar2 != 1) {
      return 0;
    }
    if (DAT_001293d0 != DAT_00129428) {
      return 0;
    }
    if (DAT_001293a8 != DAT_00129430) {
      return 0;
    }
  }
  else {
    if (DAT_00129478 == 0) {
      return 0;
    }
    if (DAT_00129490 == (code *)0x0) {
      return 0;
    }
    iVar2 = (*DAT_00129490)(0);
    if (iVar2 != 1) {
      return 0;
    }
    if (DAT_001293d0 != DAT_00129478) {
      return 0;
    }
    if (DAT_001293a8 != DAT_00129480) {
      return 0;
    }
    iVar2 = FUN_0011b320(param_1);
    if (iVar2 != 0) {
      return 1;
    }
  }
LAB_0010fb44:
  iVar2 = pthread_once((pthread_once_t *)&DAT_00129cd8,FUN_0011b480);
  if (DAT_00129ce0 == (code *)0x0) {
    return 0;
  }
  iVar2 = (*DAT_00129ce0)(iVar2);
  bVar1 = iVar2 == 1;
LAB_0010fb6c:
  return (ulong)bVar1;
}

/* ===== nexus_evasion_hold_register_v1 @ 00116144 [libNexusEvasion69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void nexus_evasion_hold_register_v1(undefined8 *param_1)

{
  char cVar1;
  bool bVar2;
  long lVar3;
  char cVar4;
  int iVar5;
  ulong uVar6;
  ushort uVar7;
  undefined1 auVar8 [16];
  long lVar9;
  undefined8 uVar10;
  code *pcVar11;
  long lVar12;
  undefined8 local_3f0;
  undefined8 uStack_3e8;
  undefined8 uStack_3e0;
  undefined8 uStack_3d8;
  undefined8 local_3d0;
  undefined8 uStack_3c8;
  long local_3c0;
  long lStack_3b8;
  long local_3b0;
  long lStack_3a8;
  long local_3a0;
  long lStack_398;
  long local_390;
  undefined8 local_380;
  long lStack_378;
  undefined8 uStack_370;
  long local_368;
  undefined8 local_360;
  undefined8 uStack_358;
  undefined8 uStack_350;
  undefined8 uStack_348;
  undefined8 local_340;
  undefined8 uStack_338;
  undefined8 uStack_330;
  undefined8 uStack_328;
  undefined8 local_320;
  undefined8 uStack_318;
  undefined8 uStack_310;
  undefined8 uStack_308;
  undefined8 local_300;
  undefined8 uStack_2f8;
  undefined8 uStack_2f0;
  undefined8 uStack_2e8;
  undefined8 local_2e0;
  undefined8 local_2d0;
  long lStack_2c8;
  ulong local_2c0;
  long local_2b8;
  undefined8 local_2b0;
  long lStack_2a8;
  long local_2a0;
  long lStack_298;
  code *local_290;
  long lStack_288;
  undefined8 local_280;
  undefined8 local_270;
  long lStack_268;
  undefined8 uStack_260;
  undefined8 local_258;
  undefined8 uStack_250;
  undefined8 local_248;
  undefined8 uStack_240;
  undefined8 local_238;
  undefined8 uStack_230;
  undefined8 local_228;
  undefined8 uStack_220;
  undefined8 local_218;
  undefined8 uStack_210;
  undefined8 local_208;
  undefined8 uStack_200;
  undefined8 local_1f8;
  undefined8 uStack_1f0;
  undefined8 local_1e8;
  long local_48;
  
  lVar3 = tpidr_el0;
  local_48 = *(long *)(lVar3 + 0x28);
  if (param_1 == (undefined8 *)0x0) {
    do {
      cVar4 = DAT_00129ce8;
      cVar1 = '\x01';
      bVar2 = (bool)ExclusiveMonitorPass(0x129ce8,0x10);
      if (bVar2) {
        _DAT_00129ce8 = 1;
        cVar1 = ExclusiveMonitorsStatus();
      }
    } while (cVar1 != '\0' || cVar4 != '\0');
    if (DAT_00129b38 < DAT_00129b50) {
      DAT_00129b38 = DAT_00129b50;
    }
    uVar6 = 1;
    DAT_00129c28 = DAT_00129c28 + 1;
    _DAT_00129b90 = 0;
    lRam0000000000129b58 = 0;
    DAT_00129b50 = 0;
    _DAT_00129b68 = 0;
    DAT_00129b60 = 0;
    DAT_00129b78 = 0;
    _DAT_00129b70 = 0;
    DAT_00129b88 = 0;
    DAT_00129b80 = (code *)0x0;
    DAT_00129b48 = 0;
    _DAT_00129b40 = 0;
    lRam0000000000129ba0 = 0;
    _DAT_00129b98 = 0;
    uRam0000000000129bb0 = 0;
    _DAT_00129ba8 = 0;
    uRam0000000000129bc0 = 0;
    DAT_00129bb8 = 0;
    uRam0000000000129bd0 = 0;
    _DAT_00129bc8 = 0;
    uRam0000000000129be0 = 0;
    _DAT_00129bd8 = 0;
    uRam0000000000129bf0 = 0;
    _DAT_00129be8 = 0;
    uRam0000000000129c00 = 0;
    _DAT_00129bf8 = 0;
    uRam0000000000129c10 = 0;
    _DAT_00129c08 = 0;
    uRam0000000000129c20 = 0;
    _DAT_00129c18 = 0;
    DAT_00129cf4 = 0;
    _DAT_00129ce8 = 0;
    goto LAB_001162cc;
  }
  uVar6 = 0;
  local_2d0 = *param_1;
  lStack_2c8 = param_1[1];
  local_2b8 = param_1[3];
  local_2c0 = param_1[2];
  local_280 = param_1[10];
  lVar9 = param_1[5];
  local_2b0 = param_1[4];
  auVar8._0_8_ = NEON_rev64(local_2d0,4);
  uVar10 = NEON_rev64(local_280,4);
  auVar8._8_8_ = uVar10;
  auVar8 = NEON_cmeq(auVar8,_DAT_00104810,4);
  uVar7 = NEON_umaxv(CONCAT26(CONCAT11(~auVar8[0xd],~auVar8[0xc]),
                              CONCAT24(CONCAT11(~auVar8[9],~auVar8[8]),
                                       CONCAT22(CONCAT11(~auVar8[5],~auVar8[4]),
                                                CONCAT11(~auVar8[1],~auVar8[0])))),2);
  lVar12 = param_1[9];
  pcVar11 = (code *)param_1[8];
  lStack_298 = SUB168(*(undefined1 (*) [16])(param_1 + 6),8);
  local_2a0 = SUB168(*(undefined1 (*) [16])(param_1 + 6),0);
  lStack_2a8 = lVar9;
  local_290 = pcVar11;
  lStack_288 = lVar12;
  if (((((((uVar7 & 1) != 0) || (pcVar11 == (code *)0x0)) || (lVar12 == 0)) ||
       ((lVar9 == 0 || (lStack_2c8 == 0)))) || (local_2c0 == 0)) || (local_2b8 == 0))
  goto LAB_001162cc;
  iVar5 = dladdr(local_2b8,&local_270);
  if ((((((iVar5 == 0) || (lStack_268 == 0)) ||
        ((iVar5 = dladdr(pcVar11,&local_380), iVar5 == 0 ||
         (((lStack_378 != lStack_268 || (iVar5 = dladdr(lVar12,&local_380), iVar5 == 0)) ||
          (lStack_378 != lStack_268)))))) ||
       ((iVar5 = dladdr(lVar9,&local_380), iVar5 == 0 || (lStack_378 != lStack_268)))) ||
      ((local_2a0 != 0 &&
       ((iVar5 = dladdr(local_2a0,&local_380), iVar5 == 0 || (lStack_378 != lStack_268)))))) ||
     ((lStack_298 != 0 &&
      ((iVar5 = dladdr(lStack_298,&local_380), iVar5 == 0 || (lStack_378 != lStack_268)))))) {
    uVar6 = 0;
    goto LAB_001162cc;
  }
  do {
    lVar12 = DAT_001293d0;
    lVar9 = DAT_001293b8;
    cVar4 = DAT_00129ce8;
    cVar1 = '\x01';
    bVar2 = (bool)ExclusiveMonitorPass(0x129ce8,0x10);
    if (bVar2) {
      _DAT_00129ce8 = 1;
      cVar1 = ExclusiveMonitorsStatus();
    }
  } while (cVar1 != '\0' || cVar4 != '\0');
  local_3c0 = DAT_00129380;
  lStack_3b8 = DAT_00129388;
  lStack_3a8 = DAT_00129398;
  local_3b0 = DAT_00129390;
  uVar6 = 0;
  lStack_398 = DAT_001293a8;
  local_3a0 = DAT_001293a0;
  uStack_2f8 = uRam00000000001299a0;
  local_300 = _DAT_00129998;
  uStack_2f0 = _DAT_001299a8;
  uStack_2e8 = uRam00000000001299b0;
  uStack_338 = uRam0000000000129960;
  local_340 = _DAT_00129958;
  uStack_328 = uRam0000000000129970;
  uStack_330 = _DAT_00129968;
  uStack_318 = uRam0000000000129980;
  local_320 = _DAT_00129978;
  uStack_310 = _DAT_00129988;
  uStack_308 = uRam0000000000129990;
  lStack_378 = DAT_00129920;
  local_380 = _DAT_00129918;
  local_368 = lRam0000000000129930;
  uStack_370 = _DAT_00129928;
  uStack_358 = uRam0000000000129940;
  local_360 = _DAT_00129938;
  uStack_350 = _DAT_00129948;
  uStack_348 = uRam0000000000129950;
  uStack_3e8 = DAT_00129358;
  local_3f0 = _DAT_00129350;
  uStack_3d8 = DAT_00129368;
  uStack_3e0 = DAT_00129360;
  local_390 = DAT_001293b0;
  local_2e0 = DAT_001299b8;
  local_3d0 = DAT_00129370;
  uStack_3c8 = DAT_00129378;
  _DAT_00129ce8 = 0;
  if ((((lStack_2c8 != DAT_001293d0) || (uVar6 = 0, local_2b8 != DAT_001293a8)) ||
      (DAT_00129920 != DAT_001293d0)) || (lRam0000000000129930 != local_2b8)) goto LAB_001162cc;
  do {
    cVar4 = DAT_00129ce8;
    cVar1 = '\x01';
    bVar2 = (bool)ExclusiveMonitorPass(0x129ce8,0x10);
    if (bVar2) {
      _DAT_00129ce8 = 1;
      cVar1 = ExclusiveMonitorsStatus();
    }
  } while (cVar1 != '\0' || cVar4 != '\0');
  memcpy(&local_270,&DAT_001296c0,0x228);
  _DAT_00129ce8 = 0;
  uVar6 = FUN_00111448(&local_3f0,&local_3c0,&local_270);
  if (((int)uVar6 == 0) || (uVar6 = FUN_00113ff8(&local_3f0,&local_380), (int)uVar6 == 0))
  goto LAB_001162cc;
  do {
    cVar4 = DAT_00129ce8;
    cVar1 = '\x01';
    bVar2 = (bool)ExclusiveMonitorPass(0x129ce8,0x10);
    if (bVar2) {
      _DAT_00129ce8 = CONCAT31(DAT_00129ce8_1,1);
      cVar1 = ExclusiveMonitorsStatus();
    }
  } while (cVar1 != '\0' || cVar4 != '\0');
  if (((lVar9 == DAT_001293b8) && (lVar12 == DAT_001293d0)) &&
     ((DAT_00129b38 < local_2c0 &&
      ((((((local_3c0 == DAT_00129380 && lStack_3b8 == DAT_00129388) && local_3b0 == DAT_00129390)
         && lStack_3a8 == DAT_00129398) && local_3a0 == DAT_001293a0) && lStack_398 == DAT_001293a8)
       && local_390 == DAT_001293b0)))) {
    iVar5 = memcmp(&local_380,&DAT_00129918,0xa8);
    local_1e8 = 0;
    uStack_1f0 = 0;
    local_1f8 = 0;
    uStack_200 = 0;
    local_208 = 0;
    uStack_210 = 0;
    local_218 = 0;
    uStack_220 = 0;
    local_228 = 0;
    uStack_230 = 0;
    local_238 = 0;
    uStack_240 = 0;
    local_248 = 0;
    uStack_250 = 0;
    local_258 = 0;
    uStack_260 = 0;
    lStack_268 = 0;
    local_270 = DAT_001047b8;
    if ((iVar5 != 0) || (iVar5 = (*local_290)(local_2b0,&local_270), iVar5 != 1)) goto LAB_0011663c;
    uVar6 = FUN_001166d8(&local_2d0,&local_270);
    if ((int)uVar6 != 0) {
      if ((local_248._4_4_ == 0) && (DAT_00129b48 != 0)) {
        iVar5 = memcmp(&DAT_00129b40,&local_2d0,0x58);
        if (iVar5 == 0) {
          iVar5 = FUN_00116854();
          uVar6 = (ulong)(iVar5 != 0);
        }
        else {
LAB_0011663c:
          uVar6 = 0;
        }
      }
      else {
        if (local_248._4_4_ != 0) goto LAB_0011663c;
        if (DAT_00129b38 < DAT_00129b50) {
          DAT_00129b38 = DAT_00129b50;
        }
        DAT_00129b60 = local_2b0;
        _DAT_00129b68 = lStack_2a8;
        DAT_00129b78 = lStack_298;
        _DAT_00129b70 = local_2a0;
        uVar6 = 1;
        DAT_00129cf4 = 0;
        DAT_00129b88 = lStack_288;
        DAT_00129b80 = local_290;
        _DAT_00129b40 = local_2d0;
        DAT_00129b48 = lStack_2c8;
        lRam0000000000129b58 = local_2b8;
        DAT_00129b50 = local_2c0;
        DAT_00129c28 = DAT_00129c28 + 1;
        uRam0000000000129bc0 = local_248;
        DAT_00129bb8 = uStack_250;
        uRam0000000000129bd0 = local_238;
        _DAT_00129bc8 = uStack_240;
        _DAT_00129b98 = local_270;
        lRam0000000000129ba0 = lStack_268;
        uRam0000000000129bb0 = local_258;
        _DAT_00129ba8 = uStack_260;
        uRam0000000000129c10 = local_1f8;
        _DAT_00129c08 = uStack_200;
        uRam0000000000129c20 = local_1e8;
        _DAT_00129c18 = uStack_1f0;
        uRam0000000000129bf0 = local_218;
        _DAT_00129be8 = uStack_220;
        _DAT_00129bf8 = uStack_210;
        uRam0000000000129c00 = local_208;
        _DAT_00129b90 = local_280;
        uRam0000000000129be0 = local_228;
        _DAT_00129bd8 = uStack_230;
      }
    }
  }
  else {
    uVar6 = 0;
    local_1e8 = 0;
    uStack_1f0 = 0;
    local_1f8 = 0;
    uStack_200 = 0;
    local_208 = 0;
    uStack_210 = 0;
    local_218 = 0;
    uStack_220 = 0;
    local_228 = 0;
    uStack_230 = 0;
    local_238 = 0;
    uStack_240 = 0;
    local_248 = 0;
    uStack_250 = 0;
    local_258 = 0;
    uStack_260 = 0;
    lStack_268 = 0;
    local_270 = DAT_001047b8;
  }
  _DAT_00129ce8 = 0;
LAB_001162cc:
  if (*(long *)(lVar3 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar6);
}

/* ===== FUN_00117314 @ 00117314 [libNexusEvasion69252.so] ===== */

undefined8 FUN_00117314(char *param_1)

{
  undefined **ppuVar1;
  int iVar2;
  undefined4 uVar3;
  undefined8 uVar4;
  uint uVar5;
  undefined **ppuVar6;
  uint uVar7;
  
  ppuVar1 = &PTR_s_cameraEnabled_001216c0;
  do {
    ppuVar6 = ppuVar1;
    iVar2 = strcmp("holdToShootAim",*ppuVar6);
    ppuVar1 = ppuVar6 + 5;
  } while (iVar2 != 0);
  uVar7 = (&DAT_00129130)[*(int *)(ppuVar6 + 1)];
  ppuVar1 = &PTR_s_cameraEnabled_001216c0;
  do {
    ppuVar6 = ppuVar1;
    iVar2 = strcmp("holdToShootRangeCheck",*ppuVar6);
    ppuVar1 = ppuVar6 + 5;
  } while (iVar2 != 0);
  uVar5 = (&DAT_00129130)[*(int *)(ppuVar6 + 1)];
  if (param_1 != (char *)0x0) {
    iVar2 = strcmp(param_1,"holdToShootAim");
    if (iVar2 == 0) {
      uVar7 = 1;
    }
    iVar2 = strcmp(param_1,"holdToShootRangeCheck");
    if (iVar2 == 0) {
      uVar5 = 1;
    }
  }
  iVar2 = FUN_00116854();
  if (iVar2 == 0) {
    return 0;
  }
  if (1 < (uVar5 | uVar7)) {
    return 0;
  }
  if ((DAT_00129b90 >> (ulong)((uVar7 | uVar5 << 1) & 0x1f) & 1) != 0) {
    if (uVar7 != 0) {
      if (DAT_00129b78 == 0) {
        return 0;
      }
      ppuVar1 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar6 = ppuVar1;
        iVar2 = strcmp("aopPredictVersion",*ppuVar6);
        ppuVar1 = ppuVar6 + 5;
      } while (iVar2 != 0);
      if ((int)(&DAT_00129130)[*(int *)(ppuVar6 + 1)] < 1) {
        return 0;
      }
      ppuVar1 = &PTR_s_cameraEnabled_001216c0;
      do {
        ppuVar6 = ppuVar1;
        iVar2 = strcmp("aopPredictVersion",*ppuVar6);
        ppuVar1 = ppuVar6 + 5;
      } while (iVar2 != 0);
      if (2 < (int)(&DAT_00129130)[*(int *)(ppuVar6 + 1)]) {
        return 0;
      }
    }
    ppuVar1 = &PTR_s_cameraEnabled_001216c0;
    do {
      ppuVar6 = ppuVar1;
      iVar2 = strcmp("aopAimEnabled",*ppuVar6);
      ppuVar1 = ppuVar6 + 5;
    } while (iVar2 != 0);
    if ((&DAT_00129130)[*(int *)(ppuVar6 + 1)] != 0) {
      if (DAT_00129b78 == 0) {
        return 0;
      }
      uVar3 = FUN_001187d8();
      uVar4 = FUN_001189fc(0,uVar3,0);
      if ((int)uVar4 == 0) {
        return uVar4;
      }
    }
    return 1;
  }
  return 0;
}

/* ===== nexus_evasion_hold_lease_v1 @ 001174f0 [libNexusEvasion69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 nexus_evasion_hold_lease_v1(undefined8 param_1,undefined8 param_2,int *param_3)

{
  char cVar1;
  bool bVar2;
  char cVar3;
  undefined8 uVar4;
  
  if (((param_3 == (int *)0x0) || (*param_3 != 1)) || (param_3[1] != 0x60)) {
    uVar4 = 0;
  }
  else {
    do {
      cVar3 = DAT_00129ce8;
      cVar1 = '\x01';
      bVar2 = (bool)ExclusiveMonitorPass(0x129ce8,0x10);
      if (bVar2) {
        _DAT_00129ce8 = CONCAT31(DAT_00129ce8_1,1);
        cVar1 = ExclusiveMonitorsStatus();
      }
    } while (cVar1 != '\0' || cVar3 != '\0');
    uVar4 = FUN_00117554();
    _DAT_00129ce8 = 0;
  }
  return uVar4;
}

/* ===== FUN_00117554 @ 00117554 [libNexusEvasion69252.so] ===== */

void FUN_00117554(int param_1,int param_2,undefined8 *param_3)

{
  long lVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  undefined **ppuVar8;
  int iVar9;
  undefined8 uVar10;
  undefined **ppuVar11;
  undefined8 local_70;
  undefined8 uStack_68;
  long lStack_60;
  long local_58;
  undefined8 uStack_50;
  long local_48;
  
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  FUN_0010f580();
  uVar10 = 0;
  if (((param_1 == 0x1d) && (0xfffffffc < param_2 - 4U)) && (DAT_001293c4 == 0)) {
    ppuVar8 = &PTR_s_cameraEnabled_001216c0;
    do {
      ppuVar11 = ppuVar8;
      iVar9 = strcmp("holdToShootEnabled",*ppuVar11);
      ppuVar8 = ppuVar11 + 5;
    } while (iVar9 != 0);
    if ((&DAT_00129130)[*(int *)(ppuVar11 + 1)] == 0) {
      uVar10 = 0;
    }
    else {
      uVar10 = FUN_00117314(0);
      if ((((int)uVar10 != 0) &&
          (uVar10 = 0, *(long *)(&DAT_00129b68 + (ulong)(param_2 - 1) * 8) != 0)) &&
         (DAT_00129b88 != (code *)0x0)) {
        uStack_50 = 0;
        local_58 = 0;
        lStack_60 = 0;
        uStack_68 = 0;
        local_70 = DAT_001047a8;
        iVar9 = (*DAT_00129b88)(DAT_00129b60,0x1d,param_2,&local_70);
        uVar6 = DAT_00129bb8;
        uVar5 = DAT_00129b50;
        uVar4 = DAT_00129b48;
        uVar3 = DAT_00129348;
        uVar2 = DAT_001047c0;
        uVar10 = 0;
        if ((((iVar9 == 1) && ((int)local_70 == 1)) &&
            (((local_70._4_4_ == 0x28 &&
              ((((uVar10 = 0, (((uint)uStack_68 ^ 0xffffffff) & 0x3f) == 0 && (uStack_50._4_4_ == 0)
                 ) && (lStack_60 != 0)) && ((local_58 != 0 && (uStack_68._4_4_ != 0)))))) &&
             (999999 < (uint)uStack_50)))) && ((uint)uStack_50 < 2000000)) {
          uVar10 = 1;
          *(undefined4 *)(param_3 + 1) = 0x1d;
          *(int *)((long)param_3 + 0xc) = param_2;
          uVar7 = DAT_00129c28;
          *param_3 = uVar2;
          param_3[4] = uVar5;
          param_3[3] = uVar4;
          param_3[2] = uVar3;
          param_3[5] = uVar6;
          param_3[6] = uVar7;
          param_3[8] = uStack_68;
          param_3[7] = local_70;
          param_3[10] = local_58;
          param_3[9] = lStack_60;
          param_3[0xb] = uStack_50;
        }
      }
    }
  }
  if (*(long *)(lVar1 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar10);
}

/* ===== nexus_evasion_hold_recheck_v1 @ 00117758 [libNexusEvasion69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void nexus_evasion_hold_recheck_v1(int *param_1)

{
  char cVar1;
  bool bVar2;
  long lVar3;
  char cVar4;
  int iVar5;
  int *piVar6;
  undefined8 local_a0;
  undefined8 uStack_98;
  undefined8 uStack_90;
  undefined8 uStack_88;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined8 uStack_70;
  undefined8 uStack_68;
  undefined8 local_60;
  undefined8 uStack_58;
  undefined8 uStack_50;
  undefined8 uStack_48;
  long local_38;
  
  lVar3 = tpidr_el0;
  local_38 = *(long *)(lVar3 + 0x28);
  piVar6 = param_1;
  if (param_1 != (int *)0x0) {
    if ((*param_1 == 1) && (param_1[1] == 0x60)) {
      do {
        cVar4 = DAT_00129ce8;
        cVar1 = '\x01';
        bVar2 = (bool)ExclusiveMonitorPass(0x129ce8,0x10);
        if (bVar2) {
          _DAT_00129ce8 = CONCAT31(DAT_00129ce8_1,1);
          cVar1 = ExclusiveMonitorsStatus();
        }
      } while (cVar1 != '\0' || cVar4 != '\0');
      uStack_98 = 0;
      local_a0 = 0;
      uStack_88 = 0;
      uStack_90 = 0;
      uStack_78 = 0;
      local_80 = 0;
      uStack_68 = 0;
      uStack_70 = 0;
      uStack_58 = 0;
      local_60 = 0;
      uStack_48 = 0;
      uStack_50 = 0;
      piVar6 = (int *)FUN_00117554(param_1[2],param_1[3],&local_a0);
      if ((int)piVar6 != 0) {
        iVar5 = memcmp(param_1,&local_a0,0x60);
        piVar6 = (int *)(ulong)(iVar5 == 0);
      }
      _DAT_00129ce8 = 0;
    }
    else {
      piVar6 = (int *)0x0;
    }
  }
  if (*(long *)(lVar3 + 0x28) == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(piVar6);
}

/* ===== FUN_001476dc @ 001476dc [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_001476dc(int param_1)

{
  long lVar1;
  int iVar2;
  char *pcVar3;
  undefined8 uVar4;
  char *pcVar5;
  char *pcVar6;
  undefined8 local_f0;
  undefined8 uStack_e8;
  char *local_e0;
  long local_d8;
  undefined8 local_c0;
  undefined8 local_b8;
  long lStack_b0;
  code *local_a8;
  undefined8 uStack_a0;
  undefined *local_98;
  undefined *puStack_90;
  undefined *local_88;
  code *pcStack_80;
  code *local_78;
  undefined8 local_70;
  long local_68;
  
  lVar1 = tpidr_el0;
  local_68 = *(long *)(lVar1 + 0x28);
  DAT_00228e28 = (code *)dlsym(DAT_0020d0f0,"nexus_evasion_hold_register_v1");
  DAT_00214a40 = dlsym(DAT_0020d0f0,"nexus_evasion_hold_snapshot_v1");
  DAT_00214d98 = dlsym(DAT_0020d0f0,"nexus_evasion_hold_lease_v1");
  DAT_00214da0 = dlsym(DAT_0020d0f0,"nexus_evasion_hold_recheck_v1");
  DAT_00214ce8 = (code *)dlsym(DAT_0020d0f0,"nexus_evasion_event_routes_v2");
  if ((((((((((DAT_00228e28 == (code *)0x0) || (iVar2 = dladdr(DAT_00228e28,&local_e0), iVar2 == 0))
            || (local_d8 != DAT_0020c0e8)) ||
           ((local_e0 == (char *)0x0 || (iVar2 = strcmp(local_e0,&DAT_0020c0f0), iVar2 != 0)))) ||
          (DAT_00214a40 == 0)) ||
         (((iVar2 = dladdr(DAT_00214a40,&local_e0), iVar2 == 0 || (local_d8 != DAT_0020c0e8)) ||
          ((local_e0 == (char *)0x0 ||
           (((iVar2 = strcmp(local_e0,&DAT_0020c0f0), iVar2 != 0 || (DAT_00214d98 == 0)) ||
            (iVar2 = dladdr(DAT_00214d98,&local_e0), iVar2 == 0)))))))) ||
        ((local_d8 != DAT_0020c0e8 || (local_e0 == (char *)0x0)))) ||
       ((iVar2 = strcmp(local_e0,&DAT_0020c0f0), iVar2 != 0 ||
        (((DAT_00214da0 == 0 || (iVar2 = dladdr(DAT_00214da0,&local_e0), iVar2 == 0)) ||
         ((local_d8 != DAT_0020c0e8 ||
          (((local_e0 == (char *)0x0 || (iVar2 = strcmp(local_e0,&DAT_0020c0f0), iVar2 != 0)) ||
           (DAT_00214ce8 == (code *)0x0)))))))))) ||
      (((iVar2 = dladdr(DAT_00214ce8,&local_e0), iVar2 == 0 || (local_d8 != DAT_0020c0e8)) ||
       (local_e0 == (char *)0x0)))) || (iVar2 = strcmp(local_e0,&DAT_0020c0f0), iVar2 != 0)) {
    pcVar5 = "exports";
  }
  else {
    DAT_00228e30 = 1;
    pcVar5 = "actual_signed_loader_identity";
    if (((param_1 != 0) && (DAT_00216f08 != '\0')) &&
       (pcVar5 = "actual_signed_loader_identity", DAT_00214c18 != 0)) {
      iVar2 = FUN_0018d938(&DAT_00214c28,DAT_001e0978,&PTR_DAT_001c3220,FUN_001428fc,0);
      if (iVar2 == 0) {
        pcVar5 = "guest_game_identity";
      }
      else {
        uRam0000000000214ce0 = 0;
        _DAT_00214cd8 = 0;
        uRam0000000000214cd0 = 0;
        _DAT_00214cc8 = 0;
        uRam0000000000214cc0 = 0;
        _DAT_00214cb8 = 0;
        uRam0000000000214cb0 = 0;
        _DAT_00214ca8 = 0;
        uRam0000000000214ca0 = 0;
        _DAT_00214c98 = 0;
        uRam0000000000214c90 = 0;
        _DAT_00214c88 = 0;
        uRam0000000000214c80 = 0;
        _DAT_00214c78 = 0;
        uRam0000000000214c70 = 0;
        _DAT_00214c68 = 0;
        uRam0000000000214c60 = 0;
        _DAT_00214c58 = 0;
        uRam0000000000214c50 = 0;
        _DAT_00214c48 = 0;
        DAT_00214c40 = DAT_0010e780;
        uVar4 = (*DAT_00214ce8)();
        if (((int)uVar4 == 1) && (iVar2 = FUN_001563f4(uVar4,&DAT_00214c40), iVar2 != 0)) {
          local_f0 = 0;
          uStack_e8 = 0;
          DAT_00214c20 = 1;
          iVar2 = FUN_00156308(1,&uStack_e8);
          if ((iVar2 == 0) || (iVar2 = FUN_00156308(2,&local_f0), iVar2 == 0)) {
            pcVar5 = "provider_all1420_native_guards";
          }
          else {
            DAT_00228e38 = 0x100000001;
            local_a8 = FUN_001455a4;
            uStack_a0 = 0;
            local_c0 = DAT_0010e700;
            local_b8 = DAT_00209d00;
            lStack_b0 = DAT_00214c18;
            puStack_90 = PTR_FUN_001c5888;
            local_98 = PTR_FUN_001c5880;
            local_78 = FUN_0016ac24;
            local_88 = PTR_FUN_001c5890;
            pcStack_80 = FUN_0016ab90;
            local_70 = DAT_0010e548;
            iVar2 = (*DAT_00228e28)(&local_c0);
            if (iVar2 == 1) {
              pcVar3 = "hold_registered";
              pcVar5 = "actual_held_default_free_handler";
              DAT_00214940 = 1;
              pcVar6 = 
              ",\"action\":29,\"scope\":\"SIGNED_LOCAL_FREE\",\"supported_domains\":15,\"main\":true,\"super_typed\":true,\"normal_aim_fold\":true,\"hold_aim\":true,\"new_hooks\":0,\"paid_state_changed\":false"
              ;
              goto LAB_00147804;
            }
            pcVar5 = "registered_default_handler";
          }
        }
        else {
          pcVar5 = "owned_event_proof";
        }
      }
    }
  }
  pcVar3 = "hold_unavailable";
  pcVar6 = ",\"action\":29,\"other_features_unchanged\":true";
  DAT_00228e38 = 0x200000000;
LAB_00147804:
  FUN_001417c8(pcVar3,pcVar5,pcVar6);
  if (*(long *)(lVar1 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0015525c @ 0015525c [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0015525c(int param_1)

{
  char *pcVar1;
  long lVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  long lVar6;
  undefined8 local_750;
  undefined8 uStack_748;
  undefined8 local_740;
  undefined8 uStack_738;
  int local_730;
  int iStack_72c;
  long local_728;
  uint local_720;
  uint uStack_71c;
  uint local_718;
  undefined8 local_714;
  undefined4 local_70c;
  undefined4 local_708;
  undefined4 local_704;
  undefined4 local_700;
  uint uStack_6fc;
  undefined8 local_6f8;
  undefined8 local_6f0;
  undefined8 uStack_6e8;
  undefined8 local_6e0;
  undefined8 uStack_6d8;
  undefined8 local_6d0;
  undefined8 uStack_6c8;
  undefined8 local_6c0;
  undefined4 local_6b8;
  timespec local_6b0 [100];
  long local_70;
  
  iVar5 = DAT_00209cd8;
  lVar2 = tpidr_el0;
  local_70 = *(long *)(lVar2 + 0x28);
  if ((((DAT_00214940 == '\x01') && ((DAT_00214978 & 1) == 0)) && (DAT_00209cd8 != 0)) &&
     (iVar3 = gettid(), iVar5 == iVar3)) {
    DAT_00214a38 = 0;
    uRam0000000000214a30 = 0;
    _DAT_00214a28 = 0;
    uRam0000000000214a20 = 0;
    _DAT_00214a18 = 0;
    uRam0000000000214a10 = 0;
    _DAT_00214a08 = 0;
    uRam0000000000214a00 = 0;
    _DAT_002149f8 = 0;
    uRam00000000002149f0 = 0;
    _DAT_002149e8 = 0;
    _DAT_002149e0 = 0;
    _DAT_002149d8 = 0;
    uRam00000000002149d0 = 0;
    DAT_002149c8 = 0;
    DAT_002149cc = 0;
    DAT_002149c0 = 0;
    DAT_002149c4 = 0;
    _DAT_002149b8 = 0;
    uRam00000000002149b0 = 0;
    _DAT_002149a8 = 0;
    uRam00000000002149a0 = 0;
    _DAT_00214998 = 0;
    uRam0000000000214990 = 0;
    DAT_00214988 = 0;
    DAT_00214980 = DAT_0010e7d8;
    iVar3 = (*DAT_00214a40)();
    if (iVar3 == 1) {
      uStack_6c8 = 0;
      local_6d0 = 0;
      uStack_6d8 = 0;
      local_6e0 = 0;
      local_6f8 = DAT_0010e7a8;
      uStack_6e8 = 0;
      local_6f0 = 0;
      iVar3 = FUN_001556a8(&local_6f8);
      iVar4 = clock_gettime(1,local_6b0);
      if (iVar4 == 0) {
        local_728 = local_6b0[0].tv_sec * 1000 + (ulong)local_6b0[0].tv_nsec / 1000000;
      }
      else {
        local_728 = 0;
      }
      uStack_738 = DAT_00214960;
      local_740 = DAT_00214958;
      local_714 = CONCAT44(DAT_002149c8,DAT_002149c4);
      local_730 = iVar5;
      iStack_72c = DAT_00209cd8;
      local_750 = DAT_00214a68;
      uStack_748 = DAT_00214b38;
      local_718 = (uint)DAT_00214b50;
      local_70c = 0;
      local_708 = DAT_002149cc;
      local_720 = (uint)((iVar3 != 0 && local_6f0._4_4_ != 0) && (int)uStack_6c8 == 0);
      uStack_71c = (uint)((DAT_002149c0 != 0 && DAT_002149b4 != 0) && DAT_002149bc != 0);
      local_704 = FUN_00155ad0();
      local_700 = FUN_00155ad0();
      uStack_6fc = (uint)(param_1 == 2);
      DAT_00214b58 = DAT_00214b58 + 1;
      DAT_00214978 = 1;
      if (param_1 == 2) {
        FUN_001bcfe8();
      }
      else {
        FUN_001bc970(&DAT_00214b60,&local_750,&DAT_001cfb60,&DAT_00214bb0);
      }
      DAT_00214978 = 0;
      iVar5 = clock_gettime(1,local_6b0);
      if (iVar5 == 0) {
        lVar6 = local_6b0[0].tv_sec * 1000 + (ulong)local_6b0[0].tv_nsec / 1000000;
      }
      else {
        lVar6 = 0;
      }
      if ((((DAT_00214b58 < 0x11) || (DAT_00214bb4 != 0)) ||
          ((iVar3 != 0 && (int)uStack_6c8 != DAT_001cfbb8 ||
           ((DAT_00214bb0 != DAT_001cfbbc || (DAT_00214e48 == 0)))))) ||
         (999 < (ulong)(lVar6 - DAT_00214e48))) {
        local_6b8 = 0;
        local_6c0 = 0;
        if (DAT_00214b50 == 1) {
          FUN_001bc828(local_740,FUN_001428fc,0,&local_6c0);
        }
        snprintf((char *)local_6b0,0x640,
                 ",\"action\":29,\"scope\":\"SIGNED_LOCAL_FREE\",\"publication\":%llu,\"epoch\":%llu,\"own_gid\":%d,\"generation\":%llu,\"requested\":%d,\"configured\":%u,\"active_known\":%u,\"active\":%u,\"ended\":%u,\"held_known\":%d,\"main_held\":%u,\"super_held\":%u,\"delay_ms\":%d,\"status\":%d,\"route\":%d,\"called\":%d,\"return_known\":%d,\"native_rc\":%d,\"raw_x\":%d,\"raw_y\":%d,\"main_calls\":%llu,\"super_calls\":%llu,\"manual_fallback_calls\":%llu,\"calls_while_ended\":%llu,\"paid_state_changed\":false"
                 ,uStack_748,local_750,(ulong)DAT_00214ab8,DAT_00214988,(ulong)DAT_002149c0,
                 DAT_002149bc);
        pcVar1 = "bounded_held_policy";
        if (DAT_00214bb4 != 0) {
          pcVar1 = "original_once_called";
        }
        FUN_001417c8("hold_fire",pcVar1,local_6b0);
        DAT_001cfbbc = DAT_00214bb0;
        DAT_00214e48 = lVar6;
        if (iVar3 != 0) {
          DAT_001cfbb8 = (int)uStack_6c8;
        }
      }
    }
  }
  if (*(long *)(lVar2 + 0x28) == local_70) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

