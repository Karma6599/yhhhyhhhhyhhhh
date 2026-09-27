/*
 * BSD Debug Menu bridge — Feature
 * Decompiled with Ghidra 11.3.2 (arm64 pseudocode) from: libNexusUI69252.so
 * Related menu entries (from embedded nexus-overlay-wire/v1):
 *   - debug.ABOUT_SCREEN "About Screen" [free]
 *   - debug.GENERIC_INFO "Generic Info" [free]
 *   - debug.ESPORTS "ESPorts" [free]
 *   - debug.NOTIFICATION_SETTINGS "Notifications" [free]
 *   - debug.FAME_POPUP "Fame Preview" [free]
 *   - debug.GOTO_HOME "Go To Home" [free]
 *   - ... +34 more (full table below / docs/debug_menu.md)
 * Bridge to the stock game's own developer command index ("BSD debug" popup, 40 commands).
 * Command table (actionId = slot + base), from the embedded wire:
 *   159776  debug.ABOUT_SCREEN                         "About Screen" [free]
 *   159777  debug.GENERIC_INFO                         "Generic Info" [free]
 *   159778  debug.ESPORTS                              "ESPorts" [free]
 *   159779  debug.NOTIFICATION_SETTINGS                "Notifications" [free]
 *   159780  debug.FAME_POPUP                           "Fame Preview" [free]
 *   159781  debug.GOTO_HOME                            "Go To Home" [free]
 *   159782  debug.GOTO_QUESTS                          "Go To Quests" [free]
 *   159783  debug.GOTO_BRAWL_PASS                      "Go To Brawl Pass" [free]
 *   159784  debug.GOTO_CLUBS                           "Go To Clubs" [free]
 *   159785  debug.GOTO_PRO_PASS                        "Go To Pro Pass" [free]
 *   159786  debug.GOTO_CLAN                            "Go To Clan" [free]
 *   159787  debug.GOTO_SCID_REWARDS                    "SCID Rewards" [free]
 *   159788  debug.CHAT_OPTIONS                         "Chat Options" [free]
 *   159789  debug.BRAWLER_UNLOCK_ANIM                  "Brawler Reward Preview" [free]
 *   159790  debug.NOT_ENOUGH_GEMS                      "Gems Popup" [free]
 *   159791  debug.FAME_LEVEL_UP_PREVIEW                "Fame Level Up" [free]
 *   159792  debug.RANKED_SEASON_END_POPUP              "Ranked Season End" [free]
 *   159793  debug.MOVIE_PLAYER_POUP                    "Movie Player" [free]
 *   159794  debug.BRAWL_TV                             "Brawl TV Intro" [free]
 *   159795  debug.SHOW_PRESTIGE_INTRO                  "Prestige Intro" [free]
 *   159796  debug.INVITE_FRIEND_CODE                   "Invite code" [free]
 *   159797  debug.UNLOCK_ACCOUNT_SCREEN                "Account unlock" [free]
 *   159798  debug.SET_COUNTRY                          "Country" [free]
 *   159799  debug.STOP_ALL_SFX                         "Stop sound effects" [free]
 *   159800  debug.STOP_MUSIC                           "Stop music" [free]
 *   159801  debug.PAUSE_MUSIC_TOGGLE                   "Pause / resume music" [free]
 *   159802  debug.MUSIC_VOLUME_CYCLE_0_50_100          "Next music volume" [free]
 *   159803  debug.BOSS_MUSIC_TOGGLE                    "Boss music" [free]
 *   159804  debug.GUI_CLOSE_ALL_POPUPS                 "Close popups" [free]
 *   159805  debug.GUI_TEST_FLOATER                     "Test message" [free]
 *   159806  debug.OPEN_CLAN_POPUP                      "Club window" [free]
 *   159807  debug.OPEN_TEAMUP_POPUP                    "Team window" [free]
 *   159808  debug.LATENCY_TEST_START                   "Latency diagnostics" [free]
 *   159809  debug.CYCLE_LANGUAGE                       "Next language" [free]
 *   159810  debug.SHOW_TID_KEYS                        "Show / hide text keys" [free]
 *   159811  debug.SKIP_GACHA_ANIM                      "Skip gacha animation" [free]
 *   159812  debug.GFX_QUALITY_CYCLE                    "Next graphics quality" [free]
 *   159813  debug.MEM_QUALITY_CYCLE                    "Next memory quality" [free]
 *   159814  debug.SOFT_RELOAD_GAME                     "Reload game" [free]
 *   159815  debug.AA_DIALOG                            "Play time dialog" [free]
 * Notes:
 *   - nexus_menu_debug_action: validates actionId in [0x27020,0x27048) (159776+i), checks the 40-bit
 *   - availability bitmap DAT_0028a404, maps slot -> internal game command id via table DAT_0019cf48
 *   - (stride 10), enforces single-in-flight + game-thread ready; returns 0=queued,2=busy,3=busy,4=rejected.
 *   - nexus_menu_diagnostics: emits {"schema":1,"state":%d,"screen":..,"open":..,"battle":..,"revision":..,
 *   - "owner_tid":..,"last_action":..,"last_result":..,"blocked_reason":..,"reason":"%s"} state JSON.
 *   - nexus_menu_debug_open/pump: request opening hidden screens and pump the pending request
 *   - ("OPENING..." / "SCREEN COULD NOT BE OPENED" / "CANCELLED").
 *   - Cross-file references (FUN_00193f80 readiness gate etc.) remain in ui/menu_engine.c.
 */

/* ===== nexus_menu_diagnostics @ 00183d98 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int nexus_menu_diagnostics(long param_1,long param_2)

{
  int iVar1;
  ulong uVar2;
  undefined8 uVar3;
  
  iVar1 = 0;
  if ((param_1 != 0) && (param_2 != 0)) {
    uVar2 = FUN_00193f80(1,&DAT_00284688);
    if ((uVar2 & 1) == 0) {
      if (DAT_00284ae8 == 0) {
        uVar3 = 0;
      }
      else {
        uVar3 = CONCAT44(DAT_00284af0,_DAT_00284aec);
      }
      iVar1 = FUN_00183ac8(param_1,0xffffffffffffffff,param_2,
                           "{\"schema\":1,\"state\":%d,\"screen\":\"%c\",\"open\":%u,\"battle\":%u,\"revision\":%u,\"owner_tid\":%d,\"last_action\":%u,\"last_result\":%u,\"blocked_reason\":%u,\"reason\":\"%s\"}"
                           ,uVar3,DAT_0028469d,DAT_0028469c,DAT_0028469e,DAT_002846a0,DAT_00284694,
                           DAT_002846a4,DAT_002846a8,DAT_002846ac,&DAT_00284aa8);
      DAT_00284688 = 0;
    }
    else {
      iVar1 = FUN_00183ac8(param_1,0xffffffffffffffff,param_2,"{\"state\":2,\"reason\":\"busy\"}");
    }
    if (iVar1 < 1) {
      iVar1 = 0;
    }
  }
  return iVar1;
}


/* ===== nexus_menu_debug_open @ 001873d8 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 nexus_menu_debug_open(int param_1)

{
  ulong uVar1;
  undefined8 uVar2;
  
  uVar1 = FUN_00193f80(1,&DAT_00284688);
  if ((uVar1 & 1) == 0) {
    uVar2 = 4;
    if ((((0 < param_1) && (DAT_00284694 == param_1)) && (DAT_0028469e == '\0')) &&
       (DAT_0028469c != '\0')) {
      if (DAT_0028469d != 100) {
        DAT_0028a424 = (uint)DAT_0028469d;
      }
      uVar2 = 1;
      DAT_0028a428 = 0;
      DAT_0028a42c = 0;
      DAT_0028a448 = 0;
      DAT_0028a454 = 0;
      DAT_0028a440 = DAT_0028a440 + 1;
      uRam000000000028a40c = 0;
      _DAT_0028a404 = 0;
      uRam000000000028a41c = 0;
      _DAT_0028a414 = 0;
      DAT_002846a0 = DAT_002846a0 + 1;
      DAT_0028469f = 0;
      DAT_0028469d = 100;
    }
    DAT_00284688 = 0;
  }
  else {
    uVar2 = 3;
  }
  return uVar2;
}


/* ===== nexus_menu_debug_action @ 001874ac ===== */

undefined8 nexus_menu_debug_action(uint param_1,int param_2)

{
  uint uVar1;
  char *pcVar2;
  uint uVar3;
  ulong uVar4;
  undefined8 uVar5;
  
  if (param_1 - 0x27120 < 0xfffffee0) {
    return 4;
  }
  uVar4 = FUN_00193f80(1,&DAT_00284688);
  if ((uVar4 & 1) != 0) {
    return 3;
  }
  if (param_2 < 1) {
    DAT_00284688 = 0;
    return 4;
  }
  if (DAT_00284694 != param_2) {
    DAT_00284688 = 0;
    return 4;
  }
  if (DAT_0028469c == '\0') {
    DAT_00284688 = 0;
    return 4;
  }
  if (DAT_0028469e != '\0') {
    DAT_00284688 = 0;
    return 4;
  }
  if (DAT_0028469d != 'd') {
    DAT_00284688 = 0;
    return 4;
  }
  if (param_1 == 0x27000) {
    uVar5 = 1;
    DAT_0028a42c = 0;
    DAT_0028469f = 0;
    DAT_0028a440 = DAT_0028a440 + 1;
    DAT_0028469d = (char)DAT_0028a424;
  }
  else {
    if (DAT_0028a42c != 0) {
      DAT_00284688 = 0;
      return 2;
    }
    uVar1 = param_1 - 0x27020;
    if (0x27 < uVar1) {
      DAT_00284688 = 0;
      return 4;
    }
    if (DAT_0028a428 == 0) {
      DAT_00284688 = 0;
      return 4;
    }
    if (((uint)(&DAT_0028a404)[uVar1 >> 5] >> (ulong)(param_1 & 0x1f) & 1) == 0) {
      DAT_00284688 = 0;
      return 4;
    }
    uVar3 = (&DAT_0019cf48)[(ulong)uVar1 * 10];
    if (0xff < uVar3) {
      DAT_00284688 = 0;
      return 0;
    }
    if (DAT_0028a3e0 <= uVar3) {
      DAT_00284688 = 0;
      return 0;
    }
    if ((*(uint *)(&DAT_0028a3e4 + ((ulong)(uVar3 >> 3) & 0x1ffffffc)) >> (ulong)(uVar3 & 0x1f) & 1)
        == 0) {
      DAT_00284688 = 0;
      return 0;
    }
    pcVar2 = "OPENING...";
    DAT_0028a42c = 3;
    if (uVar1 == 4) {
      DAT_0028a42c = 1;
      pcVar2 = "ENTER A VALUE";
    }
    DAT_0028a434 = DAT_0028a3d8;
    DAT_0028a430 = uVar1;
    FUN_00183ac8(&DAT_0028a454,0x60,0x60,&DAT_001343dc,pcVar2);
    uVar5 = 2;
  }
  DAT_002846a0 = DAT_002846a0 + 1;
  DAT_00284688 = 0;
  return uVar5;
}


/* ===== nexus_menu_debug_pump @ 0018768c ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void nexus_menu_debug_pump(int param_1,ulong param_2)

{
  uint uVar1;
  long lVar2;
  int iVar3;
  uint uVar4;
  long lVar5;
  uint uVar6;
  bool bVar7;
  bool bVar8;
  undefined1 uVar9;
  undefined4 uVar10;
  int iVar11;
  uint uVar12;
  int iVar13;
  uint uVar14;
  ulong uVar15;
  ulong uVar16;
  char *pcVar17;
  uint uVar18;
  byte *pbVar19;
  uint uVar20;
  uint uVar21;
  int local_4b8;
  byte local_4b4 [1028];
  undefined8 local_b0;
  undefined8 uStack_a8;
  undefined8 local_a0;
  undefined8 uStack_98;
  undefined8 local_90;
  undefined8 uStack_88;
  undefined4 local_80;
  long local_78;
  
  lVar2 = tpidr_el0;
  local_78 = *(long *)(lVar2 + 0x28);
  uVar15 = FUN_00193f80(1,&DAT_00284688);
  uVar6 = DAT_0028a450;
  lVar5 = DAT_0028a440;
  iVar13 = DAT_0028a434;
  uVar4 = DAT_0028a430;
  if ((uVar15 & 1) != 0) {
LAB_00187c2c:
    uVar9 = 3;
    goto LAB_00187c30;
  }
  if ((param_1 < 1) || (DAT_00284694 != param_1)) {
    uVar9 = 4;
    DAT_00284688 = 0;
    goto LAB_00187c30;
  }
  if (((DAT_0028469c == '\0') || (DAT_0028469e != '\0')) || (DAT_0028469d != 'd')) {
    DAT_0028a450 = 0;
    _DAT_0028a428 = 0;
    DAT_0028a440 = DAT_0028a440 + 1;
    DAT_00284688 = 0;
    if (uVar6 == 0) {
      uVar9 = 1;
      goto LAB_00187c30;
    }
  }
  else {
    iVar3 = DAT_0028a42c;
    if ((DAT_0028a450 == 0) || (DAT_0028a42c != 0)) {
      if (DAT_0028a42c == 4) {
        uVar9 = 3;
        DAT_00284688 = 0;
        goto LAB_00187c30;
      }
      uVar15 = (ulong)DAT_0028a430;
      if (((DAT_0028a42c == 0) && (DAT_0028a448 != 0)) &&
         ((DAT_0028a448 <= param_2 && (param_2 - DAT_0028a448 < 500)))) {
        uVar9 = 1;
        DAT_00284688 = 0;
        goto LAB_00187c30;
      }
      _DAT_0028a428 = CONCAT44(4,DAT_0028a428);
      DAT_00284688 = 0;
      uVar10 = nexus_ui_performance_mode();
      local_80 = 0;
      uStack_98 = 0;
      local_a0 = 0;
      uStack_88 = 0;
      local_90 = 0;
      uStack_a8 = 0;
      local_b0 = 0;
      iVar11 = FUN_001920a8(&local_b0,0x34);
      uVar21 = 0;
      if (((iVar11 != 0) && ((int)local_b0 == 1)) && (local_b0._4_4_ == 0x34)) {
        uVar21 = 0;
        if (((uint)local_a0 < 0x101) && (uStack_a8._4_4_ < 4)) {
          if ((uint)local_a0 < 0x100) {
            uVar21 = (uint)local_a0;
            if ((*(uint *)((long)&local_a0 + ((ulong)((uint)local_a0 >> 3) & 0x1ffffffc) + 4) >>
                 (ulong)((uint)local_a0 & 0x1f) & 1) == 0) {
              do {
                uVar18 = uVar21;
                if (uVar18 == 0xff) break;
                uVar21 = uVar18 + 1;
              } while ((*(uint *)((long)&local_a0 + (ulong)(uVar21 >> 5) * 4 + 4) >>
                        (ulong)(uVar21 & 0x1f) & 1) == 0);
              uVar21 = (uint)(0xfe < uVar18);
            }
            else {
              uVar21 = 0;
            }
          }
          else {
            uVar21 = 1;
          }
        }
      }
      memset(local_4b4,0,0x401);
      if (iVar3 == 0) {
LAB_001879bc:
        uVar20 = 0;
        uVar18 = 0;
        bVar8 = false;
        uVar12 = 0;
        uVar14 = 1;
      }
      else {
        uVar20 = 0;
        uVar18 = 1;
        if ((uVar21 == 0) || (0x27 < uVar4)) {
LAB_001879d4:
          uVar20 = 0;
          uVar14 = 0;
          bVar8 = false;
          uVar12 = 0;
        }
        else {
          uVar14 = 0;
          bVar8 = false;
          uVar12 = 0;
          if ((int)uStack_a8 == iVar13) {
            uVar20 = 0;
            uVar1 = (&DAT_0019cf48)[uVar15 * 10];
            if (0xff < uVar1) goto LAB_001879d4;
            uVar14 = 0;
            bVar8 = false;
            uVar12 = 0;
            if (uVar1 < (uint)local_a0) {
              if ((*(uint *)((long)&local_a0 + ((ulong)(uVar1 >> 3) & 0x1ffffffc) + 4) >>
                   (ulong)(uVar1 & 0x1f) & 1) == 0) {
                uVar20 = 0;
                uVar14 = 0;
                bVar8 = false;
                uVar12 = 0;
                uVar18 = 1;
              }
              else if (iVar3 == 3) {
                uVar20 = 0;
                uVar12 = 0;
                uVar14 = 1;
                uVar18 = 1;
                bVar8 = true;
              }
              else if (iVar3 == 2) {
                local_4b8 = 3;
                uVar14 = FUN_0014c8ac(uVar6,&local_4b8,local_4b4,0x401);
                uVar20 = 0;
                uVar18 = 1;
                if ((uVar14 == 0) || (local_4b8 == 3)) {
                  bVar8 = false;
                }
                else if (local_4b8 == 2) {
                  uVar20 = 0;
                  bVar8 = false;
                  uVar14 = 2;
                }
                else {
                  if (local_4b8 == 1) {
                    uVar14 = (uint)local_4b4[0];
                    if (local_4b4[0] != 0) {
                      uVar20 = 0;
                      pbVar19 = (byte *)((ulong)local_4b4 | 1);
                      do {
                        if ((uVar14 - 0x3a < 0xfffffff6) || ((0x8000002f - uVar14) / 10 < uVar20)) {
                          uVar20 = 0;
                          uVar14 = 0;
                          bVar8 = false;
                          uVar18 = 1;
                          goto LAB_00187b04;
                        }
                        iVar13 = uVar14 + uVar20 * 10;
                        uVar14 = (uint)*pbVar19;
                        uVar20 = iVar13 - 0x30;
                        pbVar19 = pbVar19 + 1;
                      } while (uVar14 != 0);
                      uVar14 = 1;
                      uVar18 = 1;
                      bVar8 = true;
                      goto LAB_00187b04;
                    }
                    uVar14 = 0;
                  }
                  else {
                    uVar18 = 0;
                  }
                  uVar20 = 0;
                  bVar8 = false;
                }
LAB_00187b04:
                uVar12 = 0;
              }
              else {
                if (iVar3 != 1) goto LAB_001879bc;
                uVar12 = FUN_0014c510();
                uVar14 = uVar12;
                if (uVar12 != 0) {
                  iVar13 = FUN_0014c550(uVar12,(&PTR_s_ABOUT_SCREEN_0019cf58)[uVar15 * 5],
                                        &DAT_00134f22,1,10);
                  uVar14 = (uint)(iVar13 != 0);
                }
                uVar20 = 0;
                bVar8 = false;
                uVar18 = uVar14 ^ 1;
              }
            }
          }
        }
      }
      uVar16 = FUN_00193f80(1,&DAT_00284688);
      if ((uVar16 & 1) != 0) {
        if (uVar12 != 0) {
          FUN_0014cc0c(uVar12);
          uVar9 = 3;
          goto LAB_00187c30;
        }
        goto LAB_00187c2c;
      }
      if ((((DAT_0028469c == '\0') || (DAT_0028469e != '\0')) || (DAT_0028469d != 'd')) ||
         ((DAT_0028a440 != lVar5 || (DAT_0028a42c != 4)))) {
        DAT_00284688 = 0;
        if (uVar12 == 0) {
          uVar9 = 4;
        }
        else {
          FUN_0014cc0c(uVar12);
          uVar9 = 4;
        }
        goto LAB_00187c30;
      }
      if (uVar21 != 0) {
        _DAT_0028a3d8 = uStack_a8;
        _DAT_0028a3d0 = local_b0;
        uRam000000000028a3e8 = uStack_98;
        _DAT_0028a3e0 = local_a0;
        uRam000000000028a3f8 = uStack_88;
        _DAT_0028a3f0 = local_90;
        DAT_0028a400 = local_80;
      }
      DAT_0028a438 = uVar10;
      DAT_0028a448 = param_2;
      if (iVar3 == 0) {
        _DAT_0028a428 = (ulong)uVar21;
      }
      else if (uVar18 == 0) {
        _DAT_0028a428 = CONCAT44(2,uVar21);
        if (uVar12 != 0) {
          DAT_0028a450 = uVar12;
        }
      }
      else {
        _DAT_0028a428 = (ulong)uVar21;
        DAT_0028a450 = 0;
        if ((uVar14 | 2) == 2) {
          DAT_0028dcf4 = uVar4 + 0x27020;
          bVar7 = DAT_0028dcf0 == -1;
          DAT_0028dcf0 = DAT_0028dcf0 + 1;
          if (bVar7) {
            DAT_0028dcf0 = 1;
          }
          DAT_0028dcf8 = 2;
          if (uVar14 != 2) goto LAB_00187c8c;
          DAT_0028dcf8 = 3;
          pcVar17 = "CANCELLED";
        }
        else {
LAB_00187c8c:
          pcVar17 = "UNAVAILABLE IN THE CURRENT SCREEN";
          if (uVar14 != 0) {
            pcVar17 = "OPENING...";
          }
        }
        FUN_00183ac8(&DAT_0028a454,0x60,0x60,&DAT_001343dc,pcVar17);
      }
      if (bVar8) {
        DAT_0028469c = '\0';
        _DAT_0028a428 = _DAT_0028a428 & 0xffffffff00000000;
      }
      DAT_002846a0 = DAT_002846a0 + 1;
      DAT_00284688 = 0;
      if ((uVar18 != 0) && (uVar6 != 0)) {
        FUN_0014cc0c(uVar6);
      }
      if ((uVar18 != 0) && (uVar12 != 0)) {
        FUN_0014cc0c(uVar12);
      }
      if (bVar8) {
        uVar14 = FUN_001920f0((&DAT_0019cf48)[uVar15 * 10],uVar20);
        uVar15 = FUN_00193f80(1,&DAT_00284688);
        if ((uVar15 & 1) == 0) {
          DAT_0028dcf8 = 1;
          if (uVar14 == 0) {
            DAT_0028dcf8 = 2;
          }
          DAT_0028dcf4 = uVar4 + 0x27020;
          bVar8 = DAT_0028dcf0 == -1;
          DAT_0028dcf0 = DAT_0028dcf0 + 1;
          if (bVar8) {
            DAT_0028dcf0 = 1;
          }
          DAT_00284688 = 0;
        }
        if (uVar14 == 0) {
          uVar15 = FUN_00193f80(1,&DAT_00284688);
          if ((uVar15 & 1) == 0) {
            if (((DAT_0028a440 == lVar5) && (DAT_0028469e == '\0')) && (DAT_0028469d == 'd')) {
              DAT_0028469c = '\x01';
              FUN_00183ac8(&DAT_0028a454,0x60,0x60,"SCREEN COULD NOT BE OPENED");
              DAT_002846a0 = DAT_002846a0 + 1;
            }
            uVar14 = 0;
            DAT_00284688 = 0;
          }
          else {
            uVar14 = 0;
          }
        }
      }
      uVar9 = uVar14 != 0;
      goto LAB_00187c30;
    }
    _DAT_0028a428 = _DAT_0028a428 & 0xffffffff;
  }
  DAT_0028a450 = 0;
  DAT_00284688 = 0;
  FUN_0014cc0c(uVar6);
  uVar9 = 1;
LAB_00187c30:
  if (*(long *)(lVar2 + 0x28) == local_78) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar9);
}


/* ===== nexus_menu_diagnostics @ 001944b0 ===== */

void nexus_menu_diagnostics(void)

{
  (*(code *)PTR_nexus_menu_diagnostics_001a3960)();
  return;
}


/* ===== nexus_menu_debug_open @ 001946d0 ===== */

void nexus_menu_debug_open(void)

{
  (*(code *)PTR_nexus_menu_debug_open_001a3a70)();
  return;
}


/* ===== nexus_menu_debug_action @ 001946e0 ===== */

void nexus_menu_debug_action(void)

{
  (*(code *)PTR_nexus_menu_debug_action_001a3a78)();
  return;
}


/* ===== nexus_menu_debug_pump @ 00194750 ===== */

void nexus_menu_debug_pump(void)

{
  (*(code *)PTR_nexus_menu_debug_pump_001a3ab0)();
  return;
}
