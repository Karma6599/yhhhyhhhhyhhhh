/*
 * Visual tweaks (HUD, names, bars...) — Feature
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
 * Notes: Many small HUD/visual toggles (FPS counter, ammo, black bars, coordinates, names, shake, skins...).
 */

/* ===== FUN_00137de4 @ 00137de4 [libNexusEvasionRuntime69252.so] ===== */

char * FUN_00137de4(char *param_1)

{
  int iVar1;
  long lVar2;
  undefined **ppuVar3;
  
  if (param_1 != (char *)0x0) {
    lVar2 = 0;
    ppuVar3 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar1 = strcmp(param_1,*ppuVar3);
      if (iVar1 == 0) {
        if (-1 < (int)lVar2) {
          return *(char **)((long)&DAT_001dfea0 + lVar2 * 4);
        }
        break;
      }
      lVar2 = lVar2 + 1;
      ppuVar3 = ppuVar3 + 3;
    } while (lVar2 != 0x37);
    param_1 = (char *)0x0;
  }
  return param_1;
}

/* ===== nexus_script_port_query @ 00137e58 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

char * nexus_script_port_query(char *param_1,undefined4 *param_2,undefined4 *param_3)

{
  char cVar1;
  bool bVar2;
  uint uVar3;
  int iVar4;
  undefined4 uVar6;
  undefined **ppuVar7;
  long lVar8;
  undefined **ppuVar9;
  char *pcVar5;
  
  if (param_1 != (char *)0x0) {
    lVar8 = 0;
    ppuVar7 = &PTR_s_DisablePinAnimation_001c2838;
    while( true ) {
      uVar3 = strcmp(param_1,*ppuVar7);
      pcVar5 = (char *)(ulong)uVar3;
      if (uVar3 == 0) break;
      lVar8 = lVar8 + 1;
      ppuVar7 = ppuVar7 + 3;
      if (lVar8 == 0x37) {
        return (char *)0x0;
      }
    }
    if (param_3 == (undefined4 *)0x0) {
      return pcVar5;
    }
    if (param_2 == (undefined4 *)0x0) {
      return pcVar5;
    }
    if ((int)lVar8 < 0) {
      return pcVar5;
    }
    do {
      uVar3 = _DAT_001dff7c;
      cVar1 = '\x01';
      bVar2 = (bool)ExclusiveMonitorPass(0x1dff7c,0x10);
      if (bVar2) {
        _DAT_001dff7c = CONCAT31(DAT_001dff7c_1,1);
        cVar1 = ExclusiveMonitorsStatus();
      }
    } while (cVar1 != '\0');
    if ((uVar3 & 1) == 0) {
      FUN_00137f5c();
      _DAT_001dff7c = 0;
    }
    lVar8 = 0;
    ppuVar9 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar4 = strcmp(param_1,*ppuVar9);
      if (iVar4 == 0) {
        if (-1 < (int)lVar8) {
          uVar6 = (undefined4)*(undefined8 *)((long)&DAT_001dfea0 + lVar8 * 4);
          goto LAB_00137f2c;
        }
        break;
      }
      lVar8 = lVar8 + 1;
      ppuVar9 = ppuVar9 + 3;
    } while (lVar8 != 0x37);
    uVar6 = 0;
LAB_00137f2c:
    *param_2 = uVar6;
    *param_3 = *(undefined4 *)((long)ppuVar7 + 0x14);
    iVar4 = FUN_001381e0(param_1);
    param_1 = (char *)(ulong)(iVar4 != 0);
  }
  return param_1;
}

/* ===== FUN_00137f5c @ 00137f5c [libNexusEvasionRuntime69252.so] ===== */

void FUN_00137f5c(char *param_1)

{
  undefined4 *puVar1;
  long lVar2;
  long lVar3;
  __mode_t _Var4;
  uint uVar5;
  int iVar6;
  __uid_t _Var7;
  char *pcVar8;
  char *pcVar9;
  char *pcVar10;
  size_t sVar11;
  char *__s;
  long lVar12;
  undefined **ppuVar13;
  char *local_21d8;
  stat local_21d0;
  undefined8 local_2140;
  undefined8 uStack_2138;
  undefined8 uStack_2130;
  undefined8 uStack_2128;
  undefined8 local_2120;
  undefined8 uStack_2118;
  undefined8 uStack_2110;
  undefined8 uStack_2108;
  undefined8 local_2100;
  undefined8 uStack_20f8;
  undefined8 uStack_20f0;
  undefined8 uStack_20e8;
  undefined8 local_20e0;
  undefined8 uStack_20d8;
  undefined8 uStack_20d0;
  undefined8 uStack_20c8;
  undefined8 local_20c0;
  undefined8 uStack_20b8;
  undefined8 uStack_20b0;
  undefined8 uStack_20a8;
  undefined8 local_20a0;
  undefined8 uStack_2098;
  undefined8 uStack_2090;
  undefined4 uStack_2088;
  undefined4 local_2084;
  undefined4 uStack_2080;
  undefined8 uStack_207c;
  long local_2068;
  char local_2060 [8184];
  long local_68;
  
  lVar2 = tpidr_el0;
  local_68 = *(long *)(lVar2 + 0x28);
  if (((DAT_0020af40 & 1) == 0) && (param_1 = (char *)(ulong)DAT_001cfb50, -1 < (int)DAT_001cfb50))
  {
    DAT_0020af40 = 1;
    uVar5 = openat(DAT_001cfb50,"script-port-settings.v1",0x88000);
    param_1 = (char *)(ulong)uVar5;
    if (-1 < (int)uVar5) {
      local_21d0.st_ino = 0;
      local_21d0.st_dev = 0;
      local_21d0.st_mode = 0;
      local_21d0.st_uid = 0;
      local_21d0.st_nlink = 0;
      local_21d0.st_rdev = 0;
      local_21d0.st_gid = 0;
      local_21d0.__pad0 = 0;
      local_21d0.st_blksize = 0;
      local_21d0.st_size = 0;
      local_21d0.st_atim.tv_sec = 0;
      local_21d0.st_blocks = 0;
      local_21d0.st_mtim.tv_sec = 0;
      local_21d0.st_atim.tv_nsec = 0;
      local_21d0.st_ctim.tv_sec = 0;
      local_21d0.st_mtim.tv_nsec = 0;
      local_21d0.__unused[0] = 0;
      local_21d0.st_ctim.tv_nsec = 0;
      iVar6 = fstat(uVar5,&local_21d0);
      if ((iVar6 == 0) && (((uint)local_21d0.st_nlink & 0xf000) == 0x8000)) {
        _Var4 = local_21d0.st_mode;
        _Var7 = getuid();
        sVar11 = 0xffffffffffffffff;
        if (((_Var4 == _Var7) && (0 < local_21d0.st_size)) && (local_21d0.st_size < 0x2000)) {
          sVar11 = read(uVar5,&local_2068,local_21d0.st_size);
        }
      }
      else {
        sVar11 = 0xffffffffffffffff;
      }
      uVar5 = close(uVar5);
      param_1 = (char *)(ulong)uVar5;
      if (((7 < (long)sVar11) && (sVar11 == local_21d0.st_size)) &&
         (local_2068 == 0xa3120393650534e)) {
        local_2060[sVar11 - 8] = '\0';
        uStack_207c = 0;
        uStack_2080 = 0;
        uStack_2138 = 0;
        local_2140 = 0;
        uStack_2128 = 0;
        uStack_2130 = 0;
        uStack_2118 = 0;
        local_2120 = 0;
        uStack_2108 = 0;
        uStack_2110 = 0;
        uStack_20f8 = 0;
        local_2100 = 0;
        uStack_20e8 = 0;
        uStack_20f0 = 0;
        uStack_20d8 = 0;
        local_20e0 = 0;
        uStack_20c8 = 0;
        uStack_20d0 = 0;
        uStack_20b8 = 0;
        local_20c0 = 0;
        uStack_20a8 = 0;
        uStack_20b0 = 0;
        uStack_2098 = 0;
        local_20a0 = 0;
        uStack_2088 = 0;
        local_2084 = 0;
        uStack_2090 = 0;
        local_21d0.__unused[2] = 0;
        local_21d0.__unused[1] = 0;
        if (local_2060[0] != '\0') {
          __s = local_2060;
          do {
            pcVar8 = strchr(__s,10);
            pcVar9 = strchr(__s,0x3d);
            param_1 = pcVar9;
            if (((pcVar8 == (char *)0x0) || (pcVar9 == (char *)0x0)) || (pcVar8 < pcVar9))
            goto LAB_001381a8;
            *pcVar9 = '\0';
            *pcVar8 = '\0';
            pcVar10 = (char *)strtol(pcVar9 + 1,&local_21d8,10);
            param_1 = pcVar10;
            if ((((local_21d8 == pcVar9 + 1) || (*local_21d8 != '\0')) || ((long)pcVar10 < 0)) ||
               (1000000 < (long)pcVar10)) goto LAB_001381a8;
            lVar12 = 0;
            ppuVar13 = &PTR_s_DisablePinAnimation_001c2838;
            do {
              uVar5 = strcmp(__s,*ppuVar13);
              param_1 = (char *)(ulong)uVar5;
              if (uVar5 == 0) {
                if (-1 < (int)lVar12) {
                  if ((long)(ulong)*(uint *)((long)ppuVar13 + 0x14) < (long)pcVar10)
                  goto LAB_001381a8;
                  *(int *)((long)local_21d0.__unused + lVar12 * 4 + 8) = (int)pcVar10;
                }
                break;
              }
              lVar12 = lVar12 + 1;
              ppuVar13 = ppuVar13 + 3;
            } while (lVar12 != 0x37);
            __s = pcVar8 + 1;
          } while (*__s != '\0');
        }
        lVar12 = 0;
        do {
          lVar3 = lVar12 + 8;
          puVar1 = (undefined4 *)((long)&DAT_001dfea0 + lVar12);
          lVar12 = lVar12 + 4;
          *puVar1 = *(undefined4 *)((long)local_21d0.__unused + lVar3);
        } while (lVar12 != 0xdc);
      }
    }
  }
LAB_001381a8:
  if (*(long *)(lVar2 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(param_1);
  }
  return;
}

/* ===== FUN_001381e0 @ 001381e0 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulong FUN_001381e0(char *param_1)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  undefined8 *puVar4;
  ulong uVar5;
  
  if (param_1 != (char *)0x0) {
    iVar1 = strcmp(param_1,"Font");
    if ((iVar1 == 0) && ((int)DAT_001e12d0 != 0)) {
      return 1;
    }
    iVar1 = strcmp(param_1,"ShowSkinNamesInProfile");
    uVar5 = DAT_00209a78;
    if (((iVar1 == 0) && (((_DAT_0020af9c ^ 0xffffffff) & 7) == 0)) &&
       (((uint)DAT_0020afa0 >> 5 & 1) != 0)) {
      return 1;
    }
    iVar1 = strcmp(param_1,"FPSLimit");
    if (iVar1 == 0) {
LAB_00138254:
      if ((uVar5 & 1) != 0) {
        return 1;
      }
    }
    else {
      iVar1 = strcmp(param_1,"HideHomeScreenText");
      if (iVar1 == 0) {
        uVar5 = uVar5 >> 1 & 0x7fffffff;
        goto LAB_00138254;
      }
    }
    iVar1 = strcmp(param_1,"ChromaticName");
    if (iVar1 == 0) {
      uVar3 = 3;
    }
    else {
      iVar1 = strcmp(param_1,"HideBattlingStatusFromOthers");
      if (iVar1 == 0) {
        uVar3 = 4;
      }
      else {
        iVar1 = strcmp(param_1,"InstantStarrDropOpening");
        if (iVar1 == 0) {
          uVar3 = 8;
        }
        else {
          iVar1 = strcmp(param_1,"FriendListOptimization");
          if (iVar1 == 0) {
            uVar3 = 0x10;
          }
          else {
            iVar1 = strcmp(param_1,"LegacyNames");
            if (iVar1 != 0) goto LAB_00138330;
            uVar3 = 0x20;
          }
        }
      }
    }
    if ((uVar3 & ((uint)DAT_0020afa0 ^ 0xffffffff)) == 0) {
      return 1;
    }
  }
LAB_00138330:
  iVar1 = strcmp(param_1,"BattleServerRegion");
  if (iVar1 != 0) {
    iVar1 = strcmp(param_1,"BattleServersPopup");
    if (iVar1 == 0) {
      puVar4 = &DAT_001e12c0;
      goto LAB_001383e8;
    }
    iVar1 = strcmp(param_1,"ShowFastPlayAgainButton");
    if ((iVar1 != 0) && (iVar1 = strcmp(param_1,"BattleEndInstantExit"), iVar1 != 0)) {
      iVar1 = strcmp(param_1,"ShowBattleCameraButton");
      if (iVar1 != 0) {
        iVar1 = FUN_0017f95c(param_1);
        if (iVar1 != 0) {
          return 1;
        }
        if (((param_1 != (char *)0x0) && (DAT_002290a8 != '\0')) &&
           (iVar1 = strcmp(param_1,"ExtendedTrajectory"), iVar1 == 0)) {
          return 1;
        }
        iVar1 = FUN_0017fa00(param_1);
        if (iVar1 != 0) {
          return 1;
        }
        iVar1 = strcmp(param_1,"CameraZoom");
        if (((iVar1 != 0) && (iVar1 = strcmp(param_1,"CameraMode"), iVar1 != 0)) &&
           (iVar1 = strncmp(param_1,"Camera",6), iVar1 != 0)) {
          iVar1 = strcmp(param_1,"HideSuperAim");
          if (iVar1 == 0) {
            puVar4 = &DAT_00214938;
LAB_0013859c:
            return (ulong)((int)*puVar4 == 1);
          }
          iVar1 = strcmp(param_1,"HighlightLeonClone");
          if (iVar1 == 0) {
            if ((int)DAT_00214938 == 1) {
              puVar4 = (undefined8 *)&DAT_0022cf44;
              goto LAB_001383e8;
            }
          }
          else {
            iVar1 = strcmp(param_1,"DisablePinAnimation");
            if (iVar1 == 0) {
              if ((int)DAT_00214938 == 1) {
                puVar4 = (undefined8 *)&DAT_0022cf40;
LAB_001385e8:
                return (ulong)((uint)*puVar4 & 1);
              }
            }
            else {
              iVar1 = strcmp(param_1,"ShowFriendlyRoomOpponents");
              if (iVar1 == 0) {
                if ((int)DAT_00214938 == 1) {
                  return (ulong)((uint)_DAT_0022cf40 >> 1 & 1);
                }
              }
              else {
                iVar1 = strcmp(param_1,"ShowEnemyAmmoStatus");
                if (iVar1 == 0) {
                  if ((int)DAT_00214938 == 1) {
                    return (ulong)((uint)_DAT_0022cf40 >> 2 & 1);
                  }
                }
                else {
                  iVar1 = strcmp(param_1,"DisableSkins");
                  if (iVar1 == 0) {
                    if ((int)DAT_00214938 == 1) {
                      return (ulong)((int)DAT_0022d064 == 7);
                    }
                  }
                  else {
                    iVar1 = strcmp(param_1,"DefaultEnvironments");
                    if (iVar1 == 0) {
                      if ((int)DAT_00214938 == 1) {
                        puVar4 = (undefined8 *)((long)&DAT_0022d064 + 4);
LAB_001383e8:
                        return (ulong)((int)*puVar4 != 0);
                      }
                    }
                    else {
                      iVar1 = strcmp(param_1,"DisableShake");
                      if (iVar1 == 0) {
                        if ((int)DAT_00214938 == 1) {
                          puVar4 = &DAT_0022d06c;
                          goto LAB_001383e8;
                        }
                      }
                      else {
                        iVar1 = strcmp(param_1,"UseBattleProxy");
                        if (iVar1 == 0) {
                          iVar1 = pthread_once((pthread_once_t *)&DAT_0021ca04,FUN_00164598);
                          if (((DAT_0021ca08 != (code *)0x0) &&
                              (iVar1 = (*DAT_0021ca08)(iVar1), iVar1 == 1)) &&
                             ((int)DAT_00214938 == 1)) {
                            return (ulong)((int)_DAT_0022d088 == 3);
                          }
                        }
                        else {
                          iVar1 = strcmp(param_1,"SlowMode");
                          if (iVar1 == 0) {
                            return (ulong)DAT_0022d134;
                          }
                          iVar1 = strcmp(param_1,"DoNotShowBattleHighlight");
                          if (iVar1 == 0) {
                            puVar4 = &DAT_0022d14c;
                            goto LAB_001385e8;
                          }
                          iVar1 = strcmp(param_1,"EnforceBattleChatButton");
                          if (iVar1 == 0) {
                            return (ulong)((uint)(DAT_0022d148 == 7) & (uint)DAT_0022d14c >> 1);
                          }
                          iVar2 = strcmp(param_1,"ShowDPS");
                          iVar1 = DAT_002fd5d0;
                          if ((iVar2 != 0) &&
                             (iVar2 = strcmp(param_1,"ShowBattleConnectionIndicator"),
                             iVar1 = DAT_002fd5ec, iVar2 != 0)) {
                            iVar1 = strcmp(param_1,"BackgroundMatchmaking");
                            if (iVar1 == 0) {
                              return (ulong)(DAT_002fd600 != 0 & DAT_0020af50);
                            }
                            iVar1 = strcmp(param_1,"HideBattleBlackBars");
                            if (iVar1 != 0) {
                              uVar5 = FUN_0017fa50(param_1);
                              return uVar5;
                            }
                            return (ulong)DAT_002fd604;
                          }
                          if (iVar1 != 0) {
                            puVar4 = (undefined8 *)&DAT_00220784;
                            goto LAB_0013859c;
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
          return 0;
        }
      }
      return (ulong)DAT_00220800;
    }
  }
  return 1;
}

/* ===== nexus_script_port_set @ 0013879c [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 nexus_script_port_set(char *param_1,uint param_2)

{
  char cVar1;
  bool bVar2;
  uint uVar3;
  int iVar4;
  undefined8 uVar5;
  ulong uVar6;
  undefined **ppuVar7;
  
  if (param_1 != (char *)0x0) {
    uVar6 = 0;
    ppuVar7 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar4 = strcmp(param_1,*ppuVar7);
      if (iVar4 == 0) goto LAB_001387ec;
      uVar6 = uVar6 + 1;
      ppuVar7 = ppuVar7 + 3;
    } while (uVar6 != 0x37);
  }
  uVar6 = 0xffffffff;
LAB_001387ec:
  if ((-1 < (int)((uint)uVar6 | param_2)) &&
     (param_2 <= (uint)(&DAT_001c284c)[(uVar6 & 0xffffffff) * 6])) {
    uVar5 = FUN_001381e0(param_1);
    if ((int)uVar5 == 0) {
      return uVar5;
    }
    do {
      uVar3 = _DAT_001dff7c;
      cVar1 = '\x01';
      bVar2 = (bool)ExclusiveMonitorPass(0x1dff7c,0x10);
      if (bVar2) {
        _DAT_001dff7c = CONCAT31(DAT_001dff7c_1,1);
        cVar1 = ExclusiveMonitorsStatus();
      }
    } while (cVar1 != '\0');
    if ((uVar3 & 1) == 0) {
      FUN_00137f5c();
      uVar5 = FUN_001388b4(uVar6 & 0xffffffff,param_2);
      if ((int)uVar5 != 0) {
        *(uint *)((long)&DAT_001dfea0 + (uVar6 & 0xffffffff) * 4) = param_2;
        do {
          cVar1 = '\x01';
          bVar2 = (bool)ExclusiveMonitorPass(0x1cfad0,0x10);
          if (bVar2) {
            cVar1 = ExclusiveMonitorsStatus();
            DAT_001cfad0 = DAT_001cfad0 + 1;
          }
        } while (cVar1 != '\0');
        if (DAT_001dff80 != (code *)0x0) {
          (*DAT_001dff80)(param_1,param_2);
        }
        uVar5 = 1;
      }
      _DAT_001dff7c = 0;
      return uVar5;
    }
  }
  return 0;
}

/* ===== FUN_001388b4 @ 001388b4 [libNexusEvasionRuntime69252.so] ===== */

void FUN_001388b4(uint param_1,uint param_2)

{
  long lVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  size_t sVar5;
  undefined8 uVar6;
  ulong uVar7;
  size_t __n;
  char *__s1;
  undefined **ppuVar8;
  long lVar9;
  char acStack_2070 [8192];
  long local_70;
  
  lVar1 = tpidr_el0;
  local_70 = *(long *)(lVar1 + 0x28);
  if (DAT_001cfb50 < 0) {
    uVar6 = 0;
  }
  else {
    lVar9 = 0;
    ppuVar8 = &PTR_s_DisablePinAnimation_001c2838;
    __n = 8;
    builtin_strncpy(acStack_2070,"NSP69 1\n",8);
    do {
      __s1 = *ppuVar8;
      if ((param_1 == 0xffffffff) ||
         ((param_1 == 0xfffffffe && (iVar2 = strncmp(__s1,"Camera",6), iVar2 == 0)))) {
        uVar7 = 0;
      }
      else {
        uVar7 = (ulong)param_2;
        if ((ulong)param_1 << 2 != lVar9) {
          uVar7 = *(ulong *)((long)&DAT_001dfea0 + lVar9);
        }
      }
      uVar3 = snprintf(acStack_2070 + __n,0x2000 - __n,"%s=%d\n",__s1,uVar7);
      if (((int)uVar3 < 0) || (0x2000 - __n <= (ulong)uVar3)) goto LAB_00138a50;
      __n = __n + uVar3;
      ppuVar8 = ppuVar8 + 3;
      lVar9 = lVar9 + 4;
    } while (lVar9 != 0xdc);
    iVar2 = openat(DAT_001cfb50,"script-port-settings.v1.tmp",0x88241,0x180);
    if (-1 < iVar2) {
      sVar5 = write(iVar2,acStack_2070,__n);
      if (sVar5 == __n) {
        iVar4 = fsync(iVar2);
        close(iVar2);
        if ((iVar4 == 0) &&
           (iVar2 = renameat(DAT_001cfb50,"script-port-settings.v1.tmp",DAT_001cfb50,
                             "script-port-settings.v1"), iVar2 == 0)) {
          fsync(DAT_001cfb50);
          uVar6 = 1;
          goto LAB_00138a58;
        }
      }
      else {
        close(iVar2);
      }
    }
LAB_00138a50:
    uVar6 = 0;
  }
LAB_00138a58:
  if (*(long *)(lVar1 + 0x28) != local_70) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar6);
  }
  return;
}

/* ===== nexus_script_port_reset @ 00138a90 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 nexus_script_port_reset(uint param_1)

{
  char cVar1;
  bool bVar2;
  uint uVar3;
  int iVar4;
  undefined8 uVar5;
  undefined4 uVar6;
  long lVar7;
  undefined **ppuVar8;
  
  if (param_1 < 2) {
    do {
      uVar3 = _DAT_001dff7c;
      cVar1 = '\x01';
      bVar2 = (bool)ExclusiveMonitorPass(0x1dff7c,0x10);
      if (bVar2) {
        _DAT_001dff7c = CONCAT31(DAT_001dff7c_1,1);
        cVar1 = ExclusiveMonitorsStatus();
      }
    } while (cVar1 != '\0');
    if ((uVar3 & 1) == 0) {
      FUN_00137f5c();
      uVar6 = 0xfffffffe;
      if (param_1 == 0) {
        uVar6 = 0xffffffff;
      }
      uVar5 = FUN_001388b4(uVar6,0);
      if ((int)uVar5 != 0) {
        lVar7 = 0;
        ppuVar8 = &PTR_s_DisablePinAnimation_001c2838;
        do {
          if (((param_1 == 0) || (iVar4 = strncmp(*ppuVar8,"Camera",6), iVar4 == 0)) &&
             (*(undefined4 *)((long)&DAT_001dfea0 + lVar7) = 0, DAT_001dff80 != (code *)0x0)) {
            (*DAT_001dff80)(*ppuVar8,0);
          }
          ppuVar8 = ppuVar8 + 3;
          lVar7 = lVar7 + 4;
        } while (lVar7 != 0xdc);
        do {
          cVar1 = '\x01';
          bVar2 = (bool)ExclusiveMonitorPass(0x1cfad0,0x10);
          if (bVar2) {
            cVar1 = ExclusiveMonitorsStatus();
            DAT_001cfad0 = DAT_001cfad0 + 1;
          }
        } while (cVar1 != '\0');
        uVar5 = 1;
      }
      _DAT_001dff7c = 0;
      return uVar5;
    }
  }
  return 0;
}

/* ===== nexus_script_port_camera_snapshot @ 00138b98 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void nexus_script_port_camera_snapshot(int *param_1)

{
  char cVar1;
  long lVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  long lVar6;
  bool bVar7;
  bool bVar8;
  bool bVar9;
  int iVar10;
  int iVar11;
  int *piVar12;
  uint uVar13;
  ulong uVar14;
  long lVar15;
  undefined **ppuVar16;
  timespec local_68;
  long local_58;
  
  lVar2 = tpidr_el0;
  local_58 = *(long *)(lVar2 + 0x28);
  piVar12 = param_1;
  if (param_1 == (int *)0x0) goto LAB_00138c2c;
  if (*param_1 == 1) {
    piVar12 = (int *)0x0;
    if ((param_1[1] != 0x38) || (DAT_00220800 == '\0')) goto LAB_00138c2c;
    do {
      uVar13 = _DAT_001dff88;
      cVar1 = '\x01';
      bVar7 = (bool)ExclusiveMonitorPass(0x1dff88,0x10);
      if (bVar7) {
        _DAT_001dff88 = CONCAT31(DAT_001dff88_1,1);
        cVar1 = ExclusiveMonitorsStatus();
      }
    } while (cVar1 != '\0');
    if ((uVar13 & 1) == 0) {
      FUN_00138d54(0);
      iVar10 = clock_gettime(1,&local_68);
      if (iVar10 == 0) {
        uVar14 = local_68.tv_sec * 1000 + (ulong)local_68.tv_nsec / 1000000;
      }
      else {
        uVar14 = 0;
      }
      lVar15 = 0;
      ppuVar16 = &PTR_s_DisablePinAnimation_001c2838;
      do {
        iVar11 = strcmp("ShowBattleCameraButton",*ppuVar16);
        iVar10 = DAT_001dffa0;
        lVar6 = DAT_001dff90;
        uVar3 = DAT_001cfad8;
        if (iVar11 == 0) {
          if (-1 < (int)lVar15) {
            uVar13 = (uint)((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar15 * 4) == 1);
            goto LAB_00138cd8;
          }
          break;
        }
        lVar15 = lVar15 + 1;
        ppuVar16 = ppuVar16 + 3;
      } while (lVar15 != 0x37);
      uVar13 = 0;
LAB_00138cd8:
      piVar12 = (int *)0x1;
      bVar8 = DAT_001dff90 != 0;
      bVar9 = DAT_001dff98 != 0;
      bVar7 = DAT_001dff98 <= uVar14;
      uVar14 = uVar14 - DAT_001dff98;
      *(undefined8 *)param_1 = DAT_0010e7a8;
      param_1[4] = iVar10;
      param_1[2] = uVar13;
      param_1[3] = (uint)(((bVar8 && bVar9) && bVar7) && uVar14 < 0x5dc);
      uVar5 = _DAT_001cfae8;
      uVar4 = _DAT_001cfae0;
      *(long *)(param_1 + 10) = lVar6;
      *(undefined8 *)(param_1 + 0xc) = uVar3;
      iVar10 = DAT_001cfaf0;
      *(undefined8 *)(param_1 + 7) = uVar5;
      *(undefined8 *)(param_1 + 5) = uVar4;
      param_1[9] = iVar10;
      _DAT_001dff88 = 0;
      goto LAB_00138c2c;
    }
  }
  piVar12 = (int *)0x0;
LAB_00138c2c:
  if (*(long *)(lVar2 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(piVar12);
}

/* ===== FUN_00138d54 @ 00138d54 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00138d54(ulong param_1)

{
  long lVar1;
  __mode_t _Var2;
  int iVar3;
  uint uVar4;
  __uid_t _Var5;
  undefined4 uVar6;
  ulong uVar7;
  size_t sVar8;
  long lVar9;
  undefined **ppuVar10;
  float fVar11;
  int local_1c4;
  stat local_1c0;
  int iStack_130;
  int iStack_12c;
  int iStack_128;
  int local_124;
  char local_120 [200];
  long local_58;
  
  lVar1 = tpidr_el0;
  local_58 = *(long *)(lVar1 + 0x28);
  if (((DAT_0020af44 & 1) == 0) && (-1 < DAT_001cfb50)) {
    lVar9 = 0;
    ppuVar10 = &PTR_s_DisablePinAnimation_001c2838;
    DAT_0020af44 = 1;
    do {
      iVar3 = strcmp("CameraZoom",*ppuVar10);
      if (iVar3 == 0) {
        if (-1 < (int)lVar9) {
          uVar7 = *(ulong *)((long)&DAT_001dfea0 + lVar9 * 4);
          if ((uint)uVar7 < 9) goto code_r0x00138e04;
          uVar6 = 100;
          goto LAB_00138e0c;
        }
        break;
      }
      lVar9 = lVar9 + 1;
      ppuVar10 = ppuVar10 + 3;
    } while (lVar9 != 0x37);
    uVar7 = 0;
code_r0x00138e04:
    uVar6 = (&DAT_001213a0)[uVar7 & 0xffffffff];
LAB_00138e0c:
    lVar9 = 0;
    ppuVar10 = &PTR_s_DisablePinAnimation_001c2838;
    _DAT_001cfae0 = CONCAT44(DAT_001cfae4,uVar6);
    do {
      iVar3 = strcmp("CameraZ",*ppuVar10);
      if (iVar3 == 0) {
        if (-1 < (int)lVar9) {
          fVar11 = 0.0;
          uVar7 = *(ulong *)((long)&DAT_001dfea0 + lVar9 * 4);
          uVar4 = (uint)uVar7;
          if (0x50 < uVar4) goto LAB_00138e94;
          if ((uVar7 & 1) == 0) goto LAB_00138e80;
          uVar4 = uVar4 + 1 >> 1;
          goto LAB_00138e84;
        }
        break;
      }
      lVar9 = lVar9 + 1;
      ppuVar10 = ppuVar10 + 3;
    } while (lVar9 != 0x37);
    uVar4 = 0;
LAB_00138e80:
    uVar4 = -(uVar4 >> 1);
LAB_00138e84:
    fVar11 = (float)(int)uVar4 * 250.0;
LAB_00138e94:
    lVar9 = 0;
    ppuVar10 = &PTR_s_DisablePinAnimation_001c2838;
    _DAT_001cfae0 = CONCAT44((int)fVar11,DAT_001cfae0);
    do {
      iVar3 = strcmp("CameraX",*ppuVar10);
      if (iVar3 == 0) {
        if (-1 < (int)lVar9) {
          fVar11 = 0.0;
          uVar7 = *(ulong *)((long)&DAT_001dfea0 + lVar9 * 4);
          uVar4 = (uint)uVar7;
          if (0x50 < uVar4) goto LAB_00138f20;
          if ((uVar7 & 1) == 0) goto LAB_00138f0c;
          uVar4 = uVar4 + 1 >> 1;
          goto LAB_00138f10;
        }
        break;
      }
      lVar9 = lVar9 + 1;
      ppuVar10 = ppuVar10 + 3;
    } while (lVar9 != 0x37);
    uVar4 = 0;
LAB_00138f0c:
    uVar4 = -(uVar4 >> 1);
LAB_00138f10:
    fVar11 = (float)(int)uVar4 * 250.0;
LAB_00138f20:
    lVar9 = 0;
    ppuVar10 = &PTR_s_DisablePinAnimation_001c2838;
    _DAT_001cfae8 = CONCAT44(DAT_001cfaec,(int)fVar11);
    do {
      iVar3 = strcmp("CameraY",*ppuVar10);
      if (iVar3 == 0) {
        if (-1 < (int)lVar9) {
          fVar11 = 0.0;
          uVar7 = *(ulong *)((long)&DAT_001dfea0 + lVar9 * 4);
          uVar4 = (uint)uVar7;
          if (0x50 < uVar4) goto LAB_00138fac;
          if ((uVar7 & 1) == 0) goto LAB_00138f98;
          uVar4 = uVar4 + 1 >> 1;
          goto LAB_00138f9c;
        }
        break;
      }
      lVar9 = lVar9 + 1;
      ppuVar10 = ppuVar10 + 3;
    } while (lVar9 != 0x37);
    uVar4 = 0;
LAB_00138f98:
    uVar4 = -(uVar4 >> 1);
LAB_00138f9c:
    fVar11 = (float)(int)uVar4 * 250.0;
LAB_00138fac:
    lVar9 = 0;
    ppuVar10 = &PTR_s_DisablePinAnimation_001c2838;
    _DAT_001cfae8 = CONCAT44((int)fVar11,DAT_001cfae8);
    do {
      iVar3 = strcmp("CameraTilt",*ppuVar10);
      if (iVar3 == 0) {
        if (-1 < (int)lVar9) {
          fVar11 = 0.0;
          uVar7 = *(ulong *)((long)&DAT_001dfea0 + lVar9 * 4);
          uVar4 = (uint)uVar7;
          if (0x50 < uVar4) goto LAB_00139038;
          if ((uVar7 & 1) == 0) goto LAB_00139024;
          uVar4 = uVar4 + 1 >> 1;
          goto LAB_00139028;
        }
        break;
      }
      lVar9 = lVar9 + 1;
      ppuVar10 = ppuVar10 + 3;
    } while (lVar9 != 0x37);
    uVar4 = 0;
LAB_00139024:
    uVar4 = -(uVar4 >> 1);
LAB_00139028:
    fVar11 = (float)(int)uVar4 * 250.0;
LAB_00139038:
    DAT_001cfaf0 = (int)fVar11;
    uVar4 = openat(DAT_001cfb50,"script-port-camera.v1",0x88000);
    param_1 = (ulong)uVar4;
    if (-1 < (int)uVar4) {
      local_120[8] = '\0';
      local_120[9] = '\0';
      local_120[10] = '\0';
      local_120[0xb] = '\0';
      local_120[0xc] = '\0';
      local_120[0xd] = '\0';
      local_120[0xe] = '\0';
      local_120[0xf] = '\0';
      local_120[0] = '\0';
      local_120[1] = '\0';
      local_120[2] = '\0';
      local_120[3] = '\0';
      local_120[4] = '\0';
      local_120[5] = '\0';
      local_120[6] = '\0';
      local_120[7] = '\0';
      local_120[0x18] = '\0';
      local_120[0x19] = '\0';
      local_120[0x1a] = '\0';
      local_120[0x1b] = '\0';
      local_120[0x1c] = '\0';
      local_120[0x1d] = '\0';
      local_120[0x1e] = '\0';
      local_120[0x1f] = '\0';
      local_120[0x10] = '\0';
      local_120[0x11] = '\0';
      local_120[0x12] = '\0';
      local_120[0x13] = '\0';
      local_120[0x14] = '\0';
      local_120[0x15] = '\0';
      local_120[0x16] = '\0';
      local_120[0x17] = '\0';
      local_120[0x28] = '\0';
      local_120[0x29] = '\0';
      local_120[0x2a] = '\0';
      local_120[0x2b] = '\0';
      local_120[0x2c] = '\0';
      local_120[0x2d] = '\0';
      local_120[0x2e] = '\0';
      local_120[0x2f] = '\0';
      local_120[0x20] = '\0';
      local_120[0x21] = '\0';
      local_120[0x22] = '\0';
      local_120[0x23] = '\0';
      local_120[0x24] = '\0';
      local_120[0x25] = '\0';
      local_120[0x26] = '\0';
      local_120[0x27] = '\0';
      local_120[0x38] = '\0';
      local_120[0x39] = '\0';
      local_120[0x3a] = '\0';
      local_120[0x3b] = '\0';
      local_120[0x3c] = '\0';
      local_120[0x3d] = '\0';
      local_120[0x3e] = '\0';
      local_120[0x3f] = '\0';
      local_120[0x30] = '\0';
      local_120[0x31] = '\0';
      local_120[0x32] = '\0';
      local_120[0x33] = '\0';
      local_120[0x34] = '\0';
      local_120[0x35] = '\0';
      local_120[0x36] = '\0';
      local_120[0x37] = '\0';
      local_120[0x48] = '\0';
      local_120[0x49] = '\0';
      local_120[0x4a] = '\0';
      local_120[0x4b] = '\0';
      local_120[0x4c] = '\0';
      local_120[0x4d] = '\0';
      local_120[0x4e] = '\0';
      local_120[0x4f] = '\0';
      local_120[0x40] = '\0';
      local_120[0x41] = '\0';
      local_120[0x42] = '\0';
      local_120[0x43] = '\0';
      local_120[0x44] = '\0';
      local_120[0x45] = '\0';
      local_120[0x46] = '\0';
      local_120[0x47] = '\0';
      local_120[0x58] = '\0';
      local_120[0x59] = '\0';
      local_120[0x5a] = '\0';
      local_120[0x5b] = '\0';
      local_120[0x5c] = '\0';
      local_120[0x5d] = '\0';
      local_120[0x5e] = '\0';
      local_120[0x5f] = '\0';
      local_120[0x50] = '\0';
      local_120[0x51] = '\0';
      local_120[0x52] = '\0';
      local_120[0x53] = '\0';
      local_120[0x54] = '\0';
      local_120[0x55] = '\0';
      local_120[0x56] = '\0';
      local_120[0x57] = '\0';
      local_120[0x68] = '\0';
      local_120[0x69] = '\0';
      local_120[0x6a] = '\0';
      local_120[0x6b] = '\0';
      local_120[0x6c] = '\0';
      local_120[0x6d] = '\0';
      local_120[0x6e] = '\0';
      local_120[0x6f] = '\0';
      local_120[0x60] = '\0';
      local_120[0x61] = '\0';
      local_120[0x62] = '\0';
      local_120[99] = '\0';
      local_120[100] = '\0';
      local_120[0x65] = '\0';
      local_120[0x66] = '\0';
      local_120[0x67] = '\0';
      local_120[0x78] = '\0';
      local_120[0x79] = '\0';
      local_120[0x7a] = '\0';
      local_120[0x7b] = '\0';
      local_120[0x7c] = '\0';
      local_120[0x7d] = '\0';
      local_120[0x7e] = '\0';
      local_120[0x7f] = '\0';
      local_120[0x70] = '\0';
      local_120[0x71] = '\0';
      local_120[0x72] = '\0';
      local_120[0x73] = '\0';
      local_120[0x74] = '\0';
      local_120[0x75] = '\0';
      local_120[0x76] = '\0';
      local_120[0x77] = '\0';
      local_120[0x88] = '\0';
      local_120[0x89] = '\0';
      local_120[0x8a] = '\0';
      local_120[0x8b] = '\0';
      local_120[0x8c] = '\0';
      local_120[0x8d] = '\0';
      local_120[0x8e] = '\0';
      local_120[0x8f] = '\0';
      local_120[0x80] = '\0';
      local_120[0x81] = '\0';
      local_120[0x82] = '\0';
      local_120[0x83] = '\0';
      local_120[0x84] = '\0';
      local_120[0x85] = '\0';
      local_120[0x86] = '\0';
      local_120[0x87] = '\0';
      local_120[0x98] = '\0';
      local_120[0x99] = '\0';
      local_120[0x9a] = '\0';
      local_120[0x9b] = '\0';
      local_120[0x9c] = '\0';
      local_120[0x9d] = '\0';
      local_120[0x9e] = '\0';
      local_120[0x9f] = '\0';
      local_120[0x90] = '\0';
      local_120[0x91] = '\0';
      local_120[0x92] = '\0';
      local_120[0x93] = '\0';
      local_120[0x94] = '\0';
      local_120[0x95] = '\0';
      local_120[0x96] = '\0';
      local_120[0x97] = '\0';
      local_120[0xa8] = '\0';
      local_120[0xa9] = '\0';
      local_120[0xaa] = '\0';
      local_120[0xab] = '\0';
      local_120[0xac] = '\0';
      local_120[0xad] = '\0';
      local_120[0xae] = '\0';
      local_120[0xaf] = '\0';
      local_120[0xa0] = '\0';
      local_120[0xa1] = '\0';
      local_120[0xa2] = '\0';
      local_120[0xa3] = '\0';
      local_120[0xa4] = '\0';
      local_120[0xa5] = '\0';
      local_120[0xa6] = '\0';
      local_120[0xa7] = '\0';
      local_120[0xb8] = '\0';
      local_120[0xb9] = '\0';
      local_120[0xba] = '\0';
      local_120[0xbb] = '\0';
      local_120[0xbc] = '\0';
      local_120[0xbd] = '\0';
      local_120[0xbe] = '\0';
      local_120[0xbf] = '\0';
      local_120[0xb0] = '\0';
      local_120[0xb1] = '\0';
      local_120[0xb2] = '\0';
      local_120[0xb3] = '\0';
      local_120[0xb4] = '\0';
      local_120[0xb5] = '\0';
      local_120[0xb6] = '\0';
      local_120[0xb7] = '\0';
      local_1c0.st_ino = 0;
      local_1c0.st_dev = 0;
      local_1c0.st_mode = 0;
      local_1c0.st_uid = 0;
      local_1c0.st_nlink = 0;
      local_1c0.st_rdev = 0;
      local_1c0.st_gid = 0;
      local_1c0.__pad0 = 0;
      local_1c0.st_blksize = 0;
      local_1c0.st_size = 0;
      local_1c0.st_atim.tv_sec = 0;
      local_1c0.st_blocks = 0;
      local_1c0.st_mtim.tv_sec = 0;
      local_1c0.st_atim.tv_nsec = 0;
      local_1c0.st_ctim.tv_sec = 0;
      local_1c0.st_mtim.tv_nsec = 0;
      local_1c0.__unused[0] = 0;
      local_1c0.st_ctim.tv_nsec = 0;
      iVar3 = fstat(uVar4,&local_1c0);
      if ((iVar3 == 0) && (((uint)local_1c0.st_nlink & 0xf000) == 0x8000)) {
        _Var2 = local_1c0.st_mode;
        _Var5 = getuid();
        sVar8 = 0xffffffffffffffff;
        if (((_Var2 == _Var5) && (0 < local_1c0.st_size)) && (local_1c0.st_size < 0xc0)) {
          sVar8 = read(uVar4,local_120,local_1c0.st_size);
        }
      }
      else {
        sVar8 = 0xffffffffffffffff;
      }
      uVar4 = close(uVar4);
      param_1 = (ulong)uVar4;
      if ((0 < (long)sVar8) && (sVar8 == local_1c0.st_size)) {
        local_1c4 = 0;
        uVar4 = sscanf(local_120,"NSPC69 1\n%d %d %d %d %d\n%n",
                       (undefined1 *)((long)local_1c0.__unused + 0x14),&iStack_130,&iStack_12c,
                       &iStack_128,&local_124,&local_1c4);
        param_1 = (ulong)uVar4;
        if ((((uVar4 == 5) && ((sVar8 == (long)local_1c4 && (local_1c0.__unused[2]._4_4_ < 0xc9))))
            && (0xffffb1de < iStack_130 - 0x2711U)) &&
           (((0xffffb1de < iStack_12c - 0x2711U && (0xffffb1de < iStack_128 - 0x2711U)) &&
            (0xffffb1de < local_124 - 0x2711U)))) {
          _DAT_001cfae8 = CONCAT44(iStack_128,iStack_12c);
          _DAT_001cfae0 = CONCAT44(iStack_130,local_1c0.__unused[2]._4_4_);
          DAT_001cfaf0 = local_124;
        }
      }
    }
  }
  if (*(long *)(lVar1 + 0x28) != local_58) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(param_1);
  }
  return;
}

/* ===== nexus_script_port_hud_snapshot @ 00139990 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void nexus_script_port_hud_snapshot(char *param_1,ulong param_2)

{
  char cVar1;
  bool bVar2;
  long lVar3;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  uint uVar7;
  undefined4 *puVar8;
  pthread_t __target_thread;
  undefined4 uVar9;
  ulong uVar10;
  int *piVar11;
  long lVar12;
  undefined **ppuVar13;
  long lVar14;
  ulong uVar15;
  ulong uVar16;
  timespec local_f0;
  undefined8 uStack_e0;
  undefined8 uStack_d8;
  undefined8 local_d0;
  undefined8 uStack_c8;
  undefined8 uStack_c0;
  undefined8 uStack_b8;
  undefined8 local_b0;
  undefined8 uStack_a8;
  undefined8 uStack_a0;
  undefined8 uStack_98;
  undefined8 local_90;
  undefined8 uStack_88;
  undefined8 uStack_80;
  undefined8 uStack_78;
  long local_68;
  
  lVar3 = tpidr_el0;
  local_68 = *(long *)(lVar3 + 0x28);
  uVar6 = 0;
  if ((param_1 == (char *)0x0) || (param_2 == 0)) goto LAB_00139a68;
  *param_1 = '\0';
  iVar5 = DAT_00209cd8;
  if (DAT_00209cd8 == 0) {
    local_f0.tv_sec = 0;
    local_f0.tv_nsec = 0;
    __target_thread = pthread_self();
    iVar5 = pthread_getname_np(__target_thread,(char *)&local_f0,0x10);
    if ((iVar5 == 0) && (local_f0.tv_sec == 0x706f6f6c6e69614d && (char)local_f0.tv_nsec == '\0'))
    goto LAB_001399ec;
  }
  else {
    iVar4 = gettid(0);
    if (iVar5 == iVar4) {
LAB_001399ec:
      if ((int)DAT_001dfff0 != 0) {
        puVar8 = (undefined4 *)__errno();
        uVar6 = *puVar8;
        FUN_0013f7e0();
        *puVar8 = uVar6;
        iVar5 = clock_gettime(1,&local_f0);
        if (iVar5 == 0) {
          uVar15 = local_f0.tv_sec * 1000 + (ulong)local_f0.tv_nsec / 1000000;
        }
        else {
          uVar15 = 0;
        }
        lVar12 = 0;
        ppuVar13 = &PTR_s_DisablePinAnimation_001c2838;
        do {
          iVar5 = strcmp("ShowFPSCounter",*ppuVar13);
          if (iVar5 == 0) {
            if ((-1 < (int)lVar12) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar12 * 4) != 0))
            goto LAB_00139b5c;
            break;
          }
          lVar12 = lVar12 + 1;
          ppuVar13 = ppuVar13 + 3;
        } while (lVar12 != 0x37);
        lVar12 = 0;
        ppuVar13 = &PTR_s_DisablePinAnimation_001c2838;
        do {
          iVar5 = strcmp("SmoothHud",*ppuVar13);
          if (iVar5 == 0) {
            if ((-1 < (int)lVar12) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar12 * 4) != 0))
            goto LAB_00139b5c;
            break;
          }
          lVar12 = lVar12 + 1;
          ppuVar13 = ppuVar13 + 3;
        } while (lVar12 != 0x37);
        uVar16 = 0;
        DAT_001e0000 = 0;
        DAT_001dfff8 = 0;
        DAT_001e0008 = 0;
        goto LAB_00139c10;
      }
    }
  }
LAB_00139a64:
  uVar6 = 0;
  goto LAB_00139a68;
LAB_00139b5c:
  local_f0.tv_nsec = 0;
  local_f0.tv_sec = 0;
  uStack_d8 = 0;
  uStack_e0 = 0;
  uStack_c8 = 0;
  local_d0 = 0;
  uStack_b8 = 0;
  uStack_c0 = 0;
  uStack_a8 = 0;
  local_b0 = 0;
  uStack_98 = 0;
  uStack_a0 = 0;
  uStack_88 = 0;
  local_90 = 0;
  uStack_78 = 0;
  uStack_80 = 0;
  iVar5 = nexus_visual_gl_status_v1(&local_f0);
  uVar16 = 0;
  if ((iVar5 != 0) && (local_f0.tv_sec != 0)) {
    if (((DAT_001dfff8 - 1 < uVar15) && (uVar16 = uVar15 - DAT_001dfff8, uVar16 < 0x9c5)) &&
       (DAT_001e0000 <= (ulong)local_f0.tv_sec)) {
      if (uVar16 < 500) {
        uVar10 = (ulong)DAT_001e0008;
      }
      else {
        uVar10 = 0;
        if (uVar16 != 0) {
          uVar10 = ((local_f0.tv_sec - DAT_001e0000) * 1000) / uVar16;
        }
        DAT_001e0008 = (uint)uVar10;
        DAT_001dfff8 = uVar15;
        DAT_001e0000 = local_f0.tv_sec;
      }
    }
    else {
      uVar10 = 0;
      DAT_001e0000 = local_f0.tv_sec;
      DAT_001e0008 = 0;
      DAT_001dfff8 = uVar15;
    }
    uVar7 = snprintf(param_1,param_2,"FPS: %d\n",uVar10);
    if (((int)uVar7 < 0) || (uVar16 = (ulong)uVar7, param_2 <= uVar16)) goto LAB_00139a64;
  }
LAB_00139c10:
  lVar12 = FUN_00139fcc();
  if (lVar12 != 0) {
    lVar14 = 0;
    ppuVar13 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar5 = strcmp("ShowOwnPlayerCoordinates",*ppuVar13);
      if (iVar5 == 0) {
        if ((-1 < (int)lVar14) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar14 * 4) != 0)) {
          uVar7 = snprintf(param_1 + uVar16,param_2 - uVar16,"X: %d  Y: %d\n",(ulong)DAT_001e0084,
                           (ulong)DAT_001e0088);
          uVar6 = 0;
          if (((int)uVar7 < 0) || (param_2 - uVar16 <= (ulong)uVar7)) goto LAB_00139a68;
          uVar16 = uVar16 + uVar7;
        }
        break;
      }
      lVar14 = lVar14 + 1;
      ppuVar13 = ppuVar13 + 3;
    } while (lVar14 != 0x37);
    lVar14 = 0;
    ppuVar13 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar5 = strcmp("ShowDPS",*ppuVar13);
      if (iVar5 == 0) {
        if ((-1 < (int)lVar14) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar14 * 4) != 0)) {
          lVar14 = 0;
          ppuVar13 = &PTR_s_DisablePinAnimation_001c2838;
          goto LAB_00139d24;
        }
        break;
      }
      lVar14 = lVar14 + 1;
      ppuVar13 = ppuVar13 + 3;
    } while (lVar14 != 0x37);
    goto LAB_00139e90;
  }
  uVar9 = 1;
  goto LAB_00139fb0;
  while( true ) {
    lVar14 = lVar14 + 1;
    ppuVar13 = ppuVar13 + 3;
    if (lVar14 == 0x37) break;
LAB_00139d24:
    iVar5 = strcmp("ShowDPS",*ppuVar13);
    if (iVar5 == 0) {
      if (-1 < (int)lVar14) {
        iVar5 = (int)*(undefined8 *)((long)&DAT_001dfea0 + lVar14 * 4);
        goto LAB_00139d70;
      }
      break;
    }
  }
  iVar5 = 0;
LAB_00139d70:
  do {
    uVar7 = _DAT_0020b098;
    cVar1 = '\x01';
    bVar2 = (bool)ExclusiveMonitorPass(0x20b098,0x10);
    if (bVar2) {
      _DAT_0020b098 = CONCAT31(DAT_0020b098_1,1);
      cVar1 = ExclusiveMonitorsStatus();
    }
  } while (cVar1 != '\0');
  if ((uVar7 & 1) == 0) {
    if (iVar5 == 0) {
      memset(&DAT_0020b0a8,0,0x1008);
      lVar14 = 0;
      DAT_0020b0a0 = DAT_0020c0b0;
    }
    else if ((DAT_0020b0a0 == lVar12) && (uVar10 = (ulong)DAT_0020b0ac, DAT_0020b0ac != 0)) {
      lVar14 = 0;
      piVar11 = &DAT_0020b0b8;
      do {
        if ((*(ulong *)(piVar11 + -2) <= uVar15) && (uVar15 - *(ulong *)(piVar11 + -2) < 0x3e9)) {
          lVar14 = lVar14 + *piVar11;
        }
        piVar11 = piVar11 + 4;
        uVar10 = uVar10 - 1;
      } while (uVar10 != 0);
    }
    else {
      lVar14 = 0;
    }
    _DAT_0020b098 = 0;
    if (lVar14 < -9999999) {
      lVar14 = -10000000;
    }
    if (9999999 < lVar14) {
      lVar14 = 10000000;
    }
  }
  else {
    lVar14 = 0;
  }
  uVar7 = snprintf(param_1 + uVar16,param_2 - uVar16,"DPS: %d\n",lVar14);
  uVar6 = 0;
  if (((int)uVar7 < 0) || (param_2 - uVar16 <= (ulong)uVar7)) goto LAB_00139a68;
  uVar16 = uVar16 + uVar7;
LAB_00139e90:
  lVar14 = 0;
  ppuVar13 = &PTR_s_DisablePinAnimation_001c2838;
  do {
    iVar5 = strcmp("ShowBattleConnectionIndicator",*ppuVar13);
    if (iVar5 == 0) {
      if ((-1 < (int)lVar14) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar14 * 4) != 0)) {
        lVar14 = 0;
        ppuVar13 = &PTR_s_DisablePinAnimation_001c2838;
        goto LAB_00139ef4;
      }
      break;
    }
    lVar14 = lVar14 + 1;
    ppuVar13 = ppuVar13 + 3;
  } while (lVar14 != 0x37);
  goto LAB_00139fac;
  while( true ) {
    lVar14 = lVar14 + 1;
    ppuVar13 = ppuVar13 + 3;
    if (lVar14 == 0x37) break;
LAB_00139ef4:
    iVar5 = strcmp("ShowBattleConnectionIndicator",*ppuVar13);
    if (iVar5 == 0) {
      if ((((-1 < (int)lVar14) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar14 * 4) != 0)) &&
          (DAT_0020c0c8 == lVar12)) && (DAT_0020c0d0 <= uVar15)) {
        if (DAT_0020c0d8 < 1) goto LAB_00139f80;
        uVar7 = snprintf(param_1 + uVar16,param_2 - uVar16,"PING: %d MS\n");
        goto LAB_00139f94;
      }
      break;
    }
  }
  DAT_0020c0c8 = 0;
  DAT_0020c0d0 = 0;
  _DAT_0020c0d8 = 0;
LAB_00139f80:
  uVar7 = snprintf(param_1 + uVar16,param_2 - uVar16,"PING: --\n");
LAB_00139f94:
  if (((int)uVar7 < 0) || (param_2 - uVar16 <= (ulong)uVar7)) goto LAB_00139a64;
  uVar16 = uVar16 + uVar7;
LAB_00139fac:
  uVar9 = 2;
LAB_00139fb0:
  uVar6 = 0;
  if (uVar16 != 0) {
    uVar6 = uVar9;
  }
LAB_00139a68:
  if (*(long *)(lVar3 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar6);
  }
  return;
}

/* ===== nexus_script_port_chat_snapshot @ 0013a39c [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void nexus_script_port_chat_snapshot(undefined1 *param_1,ulong param_2)

{
  char cVar1;
  bool bVar2;
  long lVar3;
  uint uVar4;
  int iVar5;
  undefined8 uVar6;
  size_t __n;
  ulong uVar7;
  char *__s;
  long lVar8;
  undefined **ppuVar9;
  ulong uVar10;
  long lVar11;
  timespec local_78;
  long local_68;
  
  lVar3 = tpidr_el0;
  local_68 = *(long *)(lVar3 + 0x28);
  uVar6 = 0;
  if ((param_1 == (undefined1 *)0x0) || (param_2 == 0)) goto LAB_0013a54c;
  *param_1 = 0;
  uVar10 = DAT_001e01d0;
  lVar11 = DAT_001e01c8;
  iVar5 = clock_gettime(1,&local_78);
  if (iVar5 == 0) {
    uVar7 = local_78.tv_sec * 1000 + (ulong)local_78.tv_nsec / 1000000;
  }
  else {
    uVar7 = 0;
  }
  uVar6 = 0;
  if ((((lVar11 == 0) || (uVar6 = 0, uVar10 == 0)) || (uVar7 < uVar10)) || (0x5dc < uVar7 - uVar10))
  goto LAB_0013a54c;
  lVar8 = 0;
  ppuVar9 = &PTR_s_DisablePinAnimation_001c2838;
  do {
    iVar5 = strcmp("BattleTextChat",*ppuVar9);
    if (iVar5 == 0) {
      if ((-1 < (int)lVar8) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar8 * 4) == 1))
      goto LAB_0013a4bc;
      break;
    }
    lVar8 = lVar8 + 1;
    ppuVar9 = ppuVar9 + 3;
  } while (lVar8 != 0x37);
  goto LAB_0013a548;
LAB_0013a4bc:
  do {
    uVar4 = _DAT_001e01d8;
    cVar1 = '\x01';
    bVar2 = (bool)ExclusiveMonitorPass(0x1e01d8,0x10);
    if (bVar2) {
      _DAT_001e01d8 = CONCAT31(DAT_001e01d8_1,1);
      cVar1 = ExclusiveMonitorsStatus();
    }
  } while (cVar1 != '\0');
  if ((uVar4 & 1) == 0) {
    if (DAT_001e01e0 == lVar11) {
      *param_1 = 0;
      if (DAT_001e0968 == 0) {
        lVar11 = 0;
      }
      else {
        uVar10 = 0;
        lVar11 = 0;
        __s = &DAT_001e01e8;
        do {
          __n = strlen(__s);
          lVar8 = __n + lVar11;
          if (param_2 < lVar8 + 2U) goto LAB_0013a584;
          memcpy(param_1 + lVar11,__s,__n);
          param_1[lVar8] = 10;
          lVar11 = lVar8 + 1;
          uVar10 = uVar10 + 1;
          __s = __s + 0xc0;
        } while (uVar10 < DAT_001e0968);
      }
      param_1[lVar11] = 0;
    }
LAB_0013a584:
    uVar6 = 1;
    _DAT_001e01d8 = 0;
    goto LAB_0013a54c;
  }
LAB_0013a548:
  uVar6 = 0;
LAB_0013a54c:
  if (*(long *)(lVar3 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar6);
}

/* ===== nexus_script_port_chat_action @ 0013a594 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void nexus_script_port_chat_action(int param_1)

{
  char cVar1;
  bool bVar2;
  long lVar3;
  uint uVar4;
  int iVar5;
  long lVar6;
  undefined **ppuVar7;
  undefined1 auStack_68 [8];
  undefined8 local_60;
  ulong local_58;
  ulong local_50;
  long local_48;
  
  lVar3 = tpidr_el0;
  local_48 = *(long *)(lVar3 + 0x28);
  if (DAT_001e096c == '\x01') {
    lVar6 = 0;
    ppuVar7 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar5 = strcmp("BattleTextChat",*ppuVar7);
      if (iVar5 == 0) {
        if ((-1 < (int)lVar6) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar6 * 4) != 0)) {
          lVar6 = FUN_00139fcc();
          if (lVar6 == 0) goto LAB_0013a760;
          if (param_1 == 2) goto LAB_0013a638;
          lVar6 = 0;
          if (((((param_1 != 1) || (DAT_001e0970 == 0)) ||
               (lVar6 = FUN_001428fc(0,DAT_001e0978 + 0x1304c00,&local_50,8), (int)lVar6 == 0)) ||
              ((lVar6 = 0, local_50 + 0x2000 < 0x12000 || ((local_50 & 7) != 0)))) ||
             ((lVar6 = FUN_001428fc(0,local_50,&local_58,8), (int)lVar6 == 0 ||
              ((lVar6 = 0, local_58 + 0x2000 < 0x12000 || ((local_58 & 7) != 0))))))
          goto LAB_0013a760;
          if (local_58 == DAT_001e0978 + 0x11c4e30U) {
            lVar6 = FUN_0013a78c(local_50 + 0x28,&local_60);
            if (((int)lVar6 != 0) &&
               (lVar6 = FUN_0013a78c(local_50 + 0x58,auStack_68), (int)lVar6 != 0)) {
              (*(code *)(DAT_001e0978 + 0x8d8ab8))(local_50,local_60);
              lVar6 = 1;
            }
            goto LAB_0013a760;
          }
        }
        break;
      }
      lVar6 = lVar6 + 1;
      ppuVar7 = ppuVar7 + 3;
    } while (lVar6 != 0x37);
  }
LAB_0013a75c:
  lVar6 = 0;
LAB_0013a760:
  if (*(long *)(lVar3 + 0x28) != local_48) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(lVar6);
  }
  return;
LAB_0013a638:
  do {
    uVar4 = _DAT_001e01d8;
    cVar1 = '\x01';
    bVar2 = (bool)ExclusiveMonitorPass(0x1e01d8,0x10);
    if (bVar2) {
      _DAT_001e01d8 = CONCAT31(DAT_001e01d8_1,1);
      cVar1 = ExclusiveMonitorsStatus();
    }
  } while (cVar1 != '\0');
  if ((uVar4 & 1) == 0) {
    memset(&DAT_001e01e8,0,0x784);
    lVar6 = 1;
    DAT_001e01e0 = 0;
    _DAT_001e01d8 = 0;
    goto LAB_0013a760;
  }
  goto LAB_0013a75c;
}

/* ===== nexus_script_port_server_snapshot @ 0013a7d8 [libNexusEvasionRuntime69252.so] ===== */

void nexus_script_port_server_snapshot(undefined4 *param_1,long param_2)

{
  bool bVar1;
  uint uVar2;
  char *pcVar3;
  uint uVar4;
  undefined4 uVar5;
  long lVar6;
  int iVar7;
  undefined8 uVar8;
  undefined4 *puVar9;
  void *pvVar10;
  long lVar11;
  ulong uVar12;
  undefined8 *puVar13;
  char *__s1;
  undefined **ppuVar14;
  uint *puVar15;
  uint *puVar16;
  long lVar17;
  uint local_d18;
  undefined4 uStack_d14;
  ulong local_d10;
  ulong local_d08;
  ulong local_d00;
  undefined8 local_cf8;
  ulong local_cf0;
  ulong local_ce8;
  ulong local_ce0;
  ulong local_cd8;
  ulong local_cd0 [8];
  uint local_c90 [32];
  undefined8 local_c10;
  undefined8 uStack_c08;
  undefined8 uStack_c00;
  undefined8 uStack_bf8;
  undefined8 local_bf0;
  undefined8 uStack_be8;
  undefined8 uStack_be0;
  undefined8 uStack_bd8;
  undefined8 local_bd0;
  undefined8 uStack_bc8;
  undefined8 uStack_bc0;
  undefined8 uStack_bb8;
  undefined8 local_bb0;
  undefined8 uStack_ba8;
  undefined8 uStack_ba0;
  undefined8 uStack_b98;
  ulong local_b90 [64];
  timespec local_990 [2];
  char acStack_970 [2296];
  long local_78;
  
  lVar6 = tpidr_el0;
  local_78 = *(long *)(lVar6 + 0x28);
  uVar8 = 0;
  if ((param_1 != (undefined4 *)0x0) && (param_2 == 0x918)) {
    puVar9 = (undefined4 *)__errno(0);
    uVar5 = *puVar9;
    if ((int)DAT_001e12c0 != 0) {
      iVar7 = clock_gettime(1,local_990);
      if (iVar7 == 0) {
        uVar12 = CONCAT44(local_990[0].tv_sec._4_4_,(undefined4)local_990[0].tv_sec) * 1000 +
                 (ulong)local_990[0].tv_nsec / 1000000;
      }
      else {
        uVar12 = 0;
      }
      if (((uVar12 <= DAT_0020af58 - 1) || (499 < uVar12 - DAT_0020af58)) &&
         (uVar8 = FUN_0013c82c(), (int)uVar8 != 0)) {
        local_ce0 = 0;
        local_cd8 = 0;
        local_cf0 = 0;
        local_ce8 = 0;
        local_cf8 = 0;
        DAT_0020af58 = uVar12;
        uVar8 = FUN_001428fc(uVar8,DAT_001e0978 + 0x13053c0,&local_cd8,8);
        if (((((int)uVar8 != 0) && (0x11fff < local_cd8 + 0x2000)) &&
            (((local_cd8 & 7) == 0 &&
             ((uVar8 = FUN_001428fc(uVar8,local_cd8,&local_ce0,8), (int)uVar8 != 0 &&
              (0x11fff < local_ce0 + 0x2000)))))) &&
           (((local_ce0 & 7) == 0 &&
            (((((local_ce0 == DAT_001e0978 + 0x11ebcd0U &&
                (uVar8 = FUN_001428fc(uVar8,local_cd8 + 0x214,(long)&local_cf8 + 4,4),
                (int)uVar8 != 0)) && (0 < (int)local_cf8._4_4_)) &&
              (((int)local_cf8._4_4_ < 0x41 &&
               (iVar7 = FUN_001428fc(uVar8,local_cd8 + 0x208,&local_ce8,8), iVar7 != 0)))) &&
             ((0x11fff < local_ce8 + 0x2000 && ((local_ce8 & 7) == 0)))))))) {
          pvVar10 = memset(local_990,0,0x918);
          uVar2 = local_cf8._4_4_;
          if (0x1f < local_cf8._4_4_) {
            uVar2 = 0x20;
          }
          local_c90[2] = 0;
          local_c90[3] = 0;
          local_c90[0] = 0;
          local_c90[1] = 0;
          local_c90[6] = 0;
          local_c90[7] = 0;
          local_c90[4] = 0;
          local_c90[5] = 0;
          local_c90[10] = 0;
          local_c90[0xb] = 0;
          local_c90[8] = 0;
          local_c90[9] = 0;
          local_c90[0xe] = 0;
          local_c90[0xf] = 0;
          local_c90[0xc] = 0;
          local_c90[0xd] = 0;
          local_c90[0x12] = 0;
          local_c90[0x13] = 0;
          local_c90[0x10] = 0;
          local_c90[0x11] = 0;
          local_c90[0x16] = 0;
          local_c90[0x17] = 0;
          local_c90[0x14] = 0;
          local_c90[0x15] = 0;
          local_c90[0x1a] = 0;
          local_c90[0x1b] = 0;
          local_c90[0x18] = 0;
          local_c90[0x19] = 0;
          local_c90[0x1e] = 0;
          local_c90[0x1f] = 0;
          local_c90[0x1c] = 0;
          local_c90[0x1d] = 0;
          uStack_c08 = 0;
          local_c10 = 0;
          uStack_bf8 = 0;
          uStack_c00 = 0;
          uStack_be8 = 0;
          local_bf0 = 0;
          uStack_bd8 = 0;
          uStack_be0 = 0;
          uStack_bc8 = 0;
          local_bd0 = 0;
          uStack_bb8 = 0;
          uStack_bc0 = 0;
          uStack_ba8 = 0;
          local_bb0 = 0;
          uStack_b98 = 0;
          uStack_ba0 = 0;
          local_b90[1] = 0;
          local_b90[0] = 0;
          local_b90[3] = 0;
          local_b90[2] = 0;
          local_b90[5] = 0;
          local_b90[4] = 0;
          local_b90[7] = 0;
          local_b90[6] = 0;
          local_b90[9] = 0;
          local_b90[8] = 0;
          local_b90[0xb] = 0;
          local_b90[10] = 0;
          local_b90[0xd] = 0;
          local_b90[0xc] = 0;
          local_b90[0xf] = 0;
          local_b90[0xe] = 0;
          local_b90[0x11] = 0;
          local_b90[0x10] = 0;
          local_b90[0x13] = 0;
          local_b90[0x12] = 0;
          local_b90[0x15] = 0;
          local_b90[0x14] = 0;
          local_b90[0x17] = 0;
          local_b90[0x16] = 0;
          local_b90[0x19] = 0;
          local_b90[0x18] = 0;
          local_b90[0x1b] = 0;
          local_b90[0x1a] = 0;
          local_b90[0x1d] = 0;
          local_b90[0x1c] = 0;
          local_b90[0x1f] = 0;
          local_b90[0x1e] = 0;
          local_b90[0x21] = 0;
          local_b90[0x20] = 0;
          local_b90[0x23] = 0;
          local_b90[0x22] = 0;
          local_b90[0x25] = 0;
          local_b90[0x24] = 0;
          local_b90[0x27] = 0;
          local_b90[0x26] = 0;
          local_b90[0x29] = 0;
          local_b90[0x28] = 0;
          local_b90[0x2b] = 0;
          local_b90[0x2a] = 0;
          local_b90[0x2d] = 0;
          local_b90[0x2c] = 0;
          local_b90[0x2f] = 0;
          local_b90[0x2e] = 0;
          local_b90[0x31] = 0;
          local_b90[0x30] = 0;
          local_b90[0x33] = 0;
          local_b90[0x32] = 0;
          local_b90[0x35] = 0;
          local_b90[0x34] = 0;
          local_b90[0x37] = 0;
          local_b90[0x36] = 0;
          local_b90[0x39] = 0;
          local_b90[0x38] = 0;
          local_b90[0x3b] = 0;
          local_b90[0x3a] = 0;
          local_b90[0x3d] = 0;
          local_b90[0x3c] = 0;
          local_b90[0x3f] = 0;
          local_b90[0x3e] = 0;
          if (0 < (int)local_cf8._4_4_) {
            lVar17 = 0;
            puVar15 = local_c90;
            puVar13 = &local_c10;
            lVar11 = 1;
            do {
              local_d08 = 0;
              local_d00 = 0;
              local_d10 = 0;
              local_d18 = 0xffffffff;
              uStack_d14 = 0xffffffff;
              local_cd0[1] = 0;
              local_cd0[0] = 0;
              local_cd0[3] = 0;
              local_cd0[2] = 0;
              local_cd0[5] = 0;
              local_cd0[4] = 0;
              local_cd0[7] = 0;
              local_cd0[6] = 0;
              uVar8 = FUN_001428fc(pvVar10,lVar17 + local_ce8,&local_d00,8);
              if ((((int)uVar8 == 0) || (local_d00 + 0x2000 < 0x12000)) ||
                 (((local_d00 & 7) != 0 ||
                  (((uVar8 = FUN_001428fc(uVar8,local_d00,&uStack_d14,4), (int)uVar8 == 0 ||
                    (uVar8 = FUN_001428fc(uVar8,local_d00 + 4,&local_d18,4), (int)uVar8 == 0)) ||
                   (uVar8 = FUN_001428fc(uVar8,local_d00 + 0x28,&local_d08,8), (int)uVar8 == 0))))))
              goto LAB_0013ae10;
              iVar7 = FUN_001428fc(uVar8,local_d00 + 0x28,&local_d10,8);
              if (((iVar7 == 0) || (0xfffffffffffedfff < local_d10 - 0x10000)) ||
                 (((local_d10 & 7) != 0 ||
                  ((local_d10 != local_d08 ||
                   (iVar7 = FUN_0014a6ac(local_d10,local_cd0,0x40), iVar7 == 0)))))) {
                local_cd0[0] = local_cd0[0] & 0xffffffffffffff00;
              }
              *(undefined4 *)puVar13 = uStack_d14;
              uVar4 = local_d18 - 1000;
              if (local_d18 - 1000 == 0 || (int)local_d18 < 1000) {
                uVar4 = local_d18;
              }
              if (0x497c8 < local_d18) {
                uVar4 = 0xffffffff;
              }
              *puVar15 = local_d18;
              if (0x493de < uVar4 - 2) {
                uVar4 = 0xffffffff;
              }
              *(ulong *)((long)local_b90 + lVar17 + 0x100) = local_d00;
              *(ulong *)((long)local_b90 + lVar17) = local_d08;
              pvVar10 = (void *)FUN_0014a7b0(local_990,uStack_d14,uVar4,local_cd0);
              if ((int)pvVar10 == 0) goto LAB_0013ae10;
              if (0x1e < lVar11 - 1U) break;
              lVar17 = lVar17 + 8;
              puVar15 = puVar15 + 1;
              puVar13 = (undefined8 *)((long)puVar13 + 4);
              bVar1 = lVar11 < (int)local_cf8._4_4_;
              lVar11 = lVar11 + 1;
            } while (bVar1);
          }
          uVar8 = FUN_001428fc(pvVar10,DAT_001e0978 + 0x13053c0,&local_cf0,8);
          if (((((((int)uVar8 != 0) && (local_cf0 - 0x10000 < 0xfffffffffffee000)) &&
                ((local_cf0 & 7) == 0)) &&
               ((local_cf0 == local_cd8 &&
                (uVar8 = FUN_001428fc(uVar8,local_cf0 + 0x208,&local_cf0,8), (int)uVar8 != 0)))) &&
              (local_cf0 - 0x10000 < 0xfffffffffffee000)) &&
             ((((local_cf0 & 7) == 0 && (local_cf0 == local_ce8)) &&
              (uVar8 = FUN_001428fc(uVar8,local_cd8 + 0x214,&local_cf8,4), (int)uVar8 != 0)))) {
            if ((int)local_cf8 == local_cf8._4_4_) {
              if (uVar2 != 0) {
                lVar17 = 0;
                puVar15 = local_c90;
                puVar16 = (uint *)&local_c10;
                do {
                  local_cd0[0] = 0;
                  local_d00 = 0;
                  local_d08 = CONCAT44(local_d08._4_4_,0xffffffff);
                  local_d10 = CONCAT44(local_d10._4_4_,0xffffffff);
                  uVar8 = FUN_001428fc(uVar8,lVar17 + local_ce8,local_cd0,8);
                  if ((((((int)uVar8 == 0) || (local_cd0[0] + 0x2000 < 0x12000)) ||
                       ((local_cd0[0] & 7) != 0)) ||
                      ((local_cd0[0] != *(ulong *)((long)local_b90 + lVar17 + 0x100) ||
                       (uVar8 = FUN_001428fc(uVar8,local_cd0[0],&local_d08,4), (int)uVar8 == 0))))
                     || ((((uVar8 = FUN_001428fc(uVar8,local_cd0[0] + 4,&local_d10,4),
                           (int)uVar8 == 0 || ((999999 < *puVar16 || (0x497c9 < *puVar15 + 1)))) ||
                          (*puVar16 != (uint)local_d08)) ||
                         (((*puVar15 != (uint)local_d10 ||
                           (uVar8 = FUN_001428fc(uVar8,local_cd0[0] + 0x28,&local_d00,8),
                           (int)uVar8 == 0)) || (local_d00 != *(ulong *)((long)local_b90 + lVar17)))
                         )))) goto LAB_0013ae10;
                  lVar17 = lVar17 + 8;
                  puVar15 = puVar15 + 1;
                  puVar16 = puVar16 + 1;
                } while ((ulong)uVar2 << 3 != lVar17);
              }
              pthread_mutex_lock((pthread_mutex_t *)&DAT_001e0980);
              if (local_990[0].tv_sec._4_4_ != 0) {
                uVar12 = 0;
                __s1 = acStack_970;
                do {
                  iVar7 = strncmp(__s1,"UNKNOWN REGION ",0xf);
                  pcVar3 = (char *)0x0;
                  if (iVar7 != 0) {
                    pcVar3 = __s1;
                  }
                  FUN_0014a7b0(&DAT_001e09a8,*(undefined4 *)(__s1 + -8),*(undefined4 *)(__s1 + -4),
                               pcVar3);
                  uVar12 = uVar12 + 1;
                  __s1 = __s1 + 0x48;
                } while (uVar12 < local_990[0].tv_sec._4_4_);
              }
              pthread_mutex_unlock((pthread_mutex_t *)&DAT_001e0980);
            }
          }
        }
      }
    }
LAB_0013ae10:
    pthread_mutex_lock((pthread_mutex_t *)&DAT_001e0980);
    memcpy(param_1,&DAT_001e09a8,0x918);
    pthread_mutex_unlock((pthread_mutex_t *)&DAT_001e0980);
    *param_1 = 0x918;
    lVar17 = 0;
    ppuVar14 = &PTR_s_DisablePinAnimation_001c2838;
    param_1[3] = (int)DAT_001e12c0;
    do {
      iVar7 = strcmp("BattleServerRegion",*ppuVar14);
      if (iVar7 == 0) {
        if (-1 < (int)lVar17) {
          iVar7 = (int)*(undefined8 *)((long)&DAT_001dfea0 + lVar17 * 4) + -1;
          goto LAB_0013aea4;
        }
        break;
      }
      lVar17 = lVar17 + 1;
      ppuVar14 = ppuVar14 + 3;
    } while (lVar17 != 0x37);
    iVar7 = -1;
LAB_0013aea4:
    uVar8 = 1;
    param_1[4] = iVar7;
    *puVar9 = uVar5;
  }
  if (*(long *)(lVar6 + 0x28) != local_78) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar8);
  }
  return;
}

/* ===== nexus_script_port_server_select @ 0013aee4 [libNexusEvasionRuntime69252.so] ===== */

int nexus_script_port_server_select(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  undefined4 *puVar4;
  ulong uVar5;
  int *piVar6;
  long lVar7;
  undefined **ppuVar8;
  
  puVar4 = (undefined4 *)__errno();
  uVar1 = *puVar4;
  if (param_1 == -2) {
    iVar2 = FUN_0013b030();
  }
  else {
    if (0xfff0bdbe < param_1 - 1000000U) {
      if (param_1 < 0) {
LAB_0013af90:
        lVar7 = 0;
        ppuVar8 = &PTR_s_DisablePinAnimation_001c2838;
        do {
          iVar3 = strcmp("BattleServerRegion",*ppuVar8);
          if (iVar3 == 0) {
            if (-1 < (int)lVar7) {
              iVar3 = (int)*(undefined8 *)((long)&DAT_001dfea0 + lVar7 * 4);
              goto LAB_0013afe4;
            }
            break;
          }
          lVar7 = lVar7 + 1;
          ppuVar8 = ppuVar8 + 3;
        } while (lVar7 != 0x37);
        iVar3 = 0;
LAB_0013afe4:
        iVar2 = nexus_script_port_set("BattleServerRegion",param_1 + 1);
        if (iVar2 != 0) {
          if (iVar3 != param_1 + 1) {
            DAT_001e12c8 = 0;
          }
          FUN_0013b030();
        }
        goto LAB_0013b014;
      }
      pthread_mutex_lock((pthread_mutex_t *)&DAT_001e0980);
      uVar5 = (ulong)DAT_001e09ac;
      if (DAT_001e09ac != 0) {
        piVar6 = &DAT_001e09c0;
        do {
          if (*piVar6 == param_1) {
            pthread_mutex_unlock((pthread_mutex_t *)&DAT_001e0980);
            goto LAB_0013af90;
          }
          piVar6 = piVar6 + 0x12;
          uVar5 = uVar5 - 1;
        } while (uVar5 != 0);
      }
      pthread_mutex_unlock((pthread_mutex_t *)&DAT_001e0980);
    }
    iVar2 = 0;
  }
LAB_0013b014:
  *puVar4 = uVar1;
  return iVar2;
}

/* ===== nexus_script_port_font_query @ 0013b3ac [libNexusEvasionRuntime69252.so] ===== */

void nexus_script_port_font_query(int *param_1)

{
  undefined4 uVar1;
  char cVar2;
  bool bVar3;
  long lVar4;
  undefined8 uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  uint uVar9;
  uint uVar10;
  uint uVar11;
  uint uVar12;
  uint uVar13;
  int iVar14;
  undefined4 *puVar15;
  int *piVar16;
  long lVar17;
  undefined **ppuVar18;
  undefined8 local_e0;
  char acStack_c8 [96];
  long local_68;
  
  lVar4 = tpidr_el0;
  local_68 = *(long *)(lVar4 + 0x28);
  piVar16 = param_1;
  if (param_1 != (int *)0x0) {
    if ((*param_1 == 1) && (param_1[1] == 0x30)) {
      puVar15 = (undefined4 *)__errno();
      uVar1 = *puVar15;
      if (((int)DAT_001e12d0 == 0) || ((int)DAT_001dfff0 == 0)) {
        local_e0 = 0;
      }
      else {
        iVar8 = FUN_0013b764();
        local_e0 = DAT_0010e740;
        if (iVar8 != 0) {
          do {
            iVar8 = DAT_0020af98;
            cVar2 = '\x01';
            bVar3 = (bool)ExclusiveMonitorPass(0x20af98,0x10);
            if (bVar3) {
              DAT_0020af98 = 1;
              cVar2 = ExclusiveMonitorsStatus();
            }
          } while (cVar2 != '\0');
          local_e0 = 0x100000001;
          if (iVar8 == 0) {
            (*(code *)(DAT_001e0978 + 0x66ae58))(acStack_c8,"font/Pusia-Bold.otf");
            uVar9 = (*(code *)(DAT_001e0978 + 0x75da50))(acStack_c8,0);
            (*(code *)(DAT_001e0978 + 0x66ad48))(acStack_c8);
            (*(code *)(DAT_001e0978 + 0x66ae58))(acStack_c8,"font/Cocon-Regular.otf");
            uVar10 = (*(code *)(DAT_001e0978 + 0x75da50))(acStack_c8,0);
            (*(code *)(DAT_001e0978 + 0x66ad48))(acStack_c8);
            (*(code *)(DAT_001e0978 + 0x66ae58))(acStack_c8,"font/Downcome.otf");
            uVar11 = (*(code *)(DAT_001e0978 + 0x75da50))(acStack_c8,0);
            (*(code *)(DAT_001e0978 + 0x66ad48))(acStack_c8);
            (*(code *)(DAT_001e0978 + 0x66ae58))(acStack_c8,"font/Impact.ttf");
            uVar12 = (*(code *)(DAT_001e0978 + 0x75da50))(acStack_c8,0);
            (*(code *)(DAT_001e0978 + 0x66ad48))(acStack_c8);
            (*(code *)(DAT_001e0978 + 0x66ae58))(acStack_c8,"font/HYWenHei-85W.ttf");
            uVar13 = (*(code *)(DAT_001e0978 + 0x75da50))(acStack_c8,0);
            (*(code *)(DAT_001e0978 + 0x66ad48))(acStack_c8);
            DAT_001e12d8._0_4_ = FUN_0014ce5c();
            lVar17 = CONCAT44(uRam00000000001cfafc,DAT_001cfaf4._4_4_);
            do {
              cVar2 = '\x01';
              bVar3 = (bool)ExclusiveMonitorPass(0x1cfaf8,0x10);
              if (bVar3) {
                cVar2 = ExclusiveMonitorsStatus();
                lVar17 = lVar17 + 1;
              }
              uRam00000000001cfafc = (undefined4)((ulong)lVar17 >> 0x20);
              DAT_001cfaf4._4_4_ = (undefined4)lVar17;
            } while (cVar2 != '\0');
            snprintf(acStack_c8,0x60,"asset_preflight new_mask=%u loaded_mask=%u",
                     (ulong)((uVar10 & 1) << 2 | (uVar9 & 1) << 1 | (uVar11 & 1) << 3 |
                             (uVar12 & 1) << 4 | (uVar13 & 1) << 5 | 1),(ulong)(uint)DAT_001e12d8);
            FUN_001417c8("script_port_fonts",acStack_c8,0);
          }
        }
      }
      lVar17 = 0;
      ppuVar18 = &PTR_s_DisablePinAnimation_001c2838;
      do {
        iVar14 = strcmp("Font",*ppuVar18);
        iVar7 = DAT_001e12d8._4_4_;
        uVar9 = (uint)DAT_001e12d8;
        iVar6 = DAT_001e12d0._4_4_;
        iVar8 = (int)DAT_001cfaf4;
        if (iVar14 == 0) {
          if (-1 < (int)lVar17) {
            iVar14 = (int)*(undefined8 *)((long)&DAT_001dfea0 + lVar17 * 4);
            goto LAB_0013b6d0;
          }
          break;
        }
        lVar17 = lVar17 + 1;
        ppuVar18 = ppuVar18 + 3;
      } while (lVar17 != 0x37);
      iVar14 = 0;
LAB_0013b6d0:
      piVar16 = (int *)0x1;
      uVar5 = CONCAT44(uRam00000000001cfafc,DAT_001cfaf4._4_4_);
      *(undefined8 *)param_1 = DAT_0010e580;
      *(undefined8 *)(param_1 + 2) = local_e0;
      param_1[4] = iVar14;
      param_1[5] = iVar6;
      param_1[6] = uVar9;
      param_1[7] = iVar7;
      param_1[8] = iVar8;
      param_1[9] = 0;
      *(undefined8 *)(param_1 + 10) = uVar5;
      *puVar15 = uVar1;
    }
    else {
      piVar16 = (int *)0x0;
    }
  }
  if (*(long *)(lVar4 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(piVar16);
  }
  return;
}

/* ===== nexus_script_port_client_performance_query @ 0013c53c [libNexusEvasionRuntime69252.so] ===== */

void nexus_script_port_client_performance_query(int *param_1)

{
  undefined4 uVar1;
  long lVar2;
  int iVar3;
  undefined4 *puVar4;
  int *piVar5;
  undefined4 uVar6;
  uint uVar7;
  ulong uVar8;
  long lVar9;
  undefined1 *puVar10;
  undefined **ppuVar11;
  ulong uVar12;
  undefined1 auStack_9e0 [4];
  uint local_9dc;
  int local_9d4;
  undefined1 auStack_9c0 [2296];
  undefined8 local_c8;
  undefined8 uStack_c0;
  undefined8 uStack_b8;
  undefined8 local_b0;
  undefined8 uStack_a8;
  undefined8 local_a0;
  undefined8 uStack_98;
  undefined8 local_90;
  undefined8 uStack_88;
  undefined8 local_80;
  undefined8 uStack_78;
  undefined8 local_70;
  long local_68;
  
  lVar2 = tpidr_el0;
  local_68 = *(long *)(lVar2 + 0x28);
  piVar5 = param_1;
  if (param_1 != (int *)0x0) {
    if ((*param_1 == 1) && (param_1[1] == 0x60)) {
      puVar4 = (undefined4 *)__errno();
      local_70 = 0;
      uVar1 = *puVar4;
      uStack_78 = 0;
      local_80 = 0;
      uStack_88 = 0;
      local_90 = 0;
      uStack_98 = 0;
      local_a0 = 0;
      uStack_a8 = 0;
      local_b0 = 0;
      uStack_b8._0_4_ = 0;
      uStack_b8._4_4_ = 0xffffffff;
      local_c8 = DAT_0010e720;
      uStack_c0._0_4_ = (uint)DAT_00209a78;
      if ((int)DAT_001dfff0 == 0) {
        uStack_c0._0_4_ = 0;
      }
      uStack_c0 = (ulong)(uint)uStack_c0;
      lVar9 = 0;
      ppuVar11 = &PTR_s_DisablePinAnimation_001c2838;
      do {
        iVar3 = strcmp("FPSLimit",*ppuVar11);
        if (iVar3 == 0) {
          if (-1 < (int)lVar9) {
            uVar6 = (undefined4)*(undefined8 *)((long)&DAT_001dfea0 + lVar9 * 4);
            goto LAB_0013c648;
          }
          break;
        }
        lVar9 = lVar9 + 1;
        ppuVar11 = ppuVar11 + 3;
      } while (lVar9 != 0x37);
      uVar6 = 0;
LAB_0013c648:
      lVar9 = 0;
      ppuVar11 = &PTR_s_DisablePinAnimation_001c2838;
      uStack_b8 = CONCAT44(uStack_b8._4_4_,uVar6);
      do {
        iVar3 = strcmp("HideHomeScreenText",*ppuVar11);
        if (iVar3 == 0) {
          if (-1 < (int)lVar9) {
            uVar7 = (uint)((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar9 * 4) != 0);
            goto LAB_0013c6a8;
          }
          break;
        }
        lVar9 = lVar9 + 1;
        ppuVar11 = ppuVar11 + 3;
      } while (lVar9 != 0x37);
      uVar7 = 0;
LAB_0013c6a8:
      uStack_c0 = CONCAT44(uVar7,(uint)uStack_c0);
      local_b0 = DAT_001cfad0;
      memset(auStack_9e0,0,0x918);
      iVar3 = nexus_script_port_server_snapshot(auStack_9e0,0x918);
      if ((((iVar3 != 0) && (local_9d4 != 0)) && (uVar8 = (ulong)local_9dc, uVar8 < 0x21)) &&
         (uVar8 != 0)) {
        uVar12 = 0;
        puVar10 = auStack_9c0;
        do {
          iVar3 = *(int *)(puVar10 + -4);
          if ((-1 < iVar3) && ((uStack_b8 < 0 || (iVar3 < uStack_b8._4_4_)))) {
            uStack_b8 = CONCAT44(iVar3,(undefined4)uStack_b8);
            snprintf((char *)&uStack_a8,0x40,"%s",puVar10);
            uVar8 = (ulong)local_9dc;
          }
          uVar12 = uVar12 + 1;
          puVar10 = puVar10 + 0x48;
        } while (uVar12 < uVar8);
      }
      piVar5 = (int *)0x1;
      *(undefined8 *)(param_1 + 10) = local_a0;
      *(undefined8 *)(param_1 + 8) = uStack_a8;
      *(undefined8 *)(param_1 + 0xe) = local_90;
      *(undefined8 *)(param_1 + 0xc) = uStack_98;
      *(undefined8 *)(param_1 + 0x12) = local_80;
      *(undefined8 *)(param_1 + 0x10) = uStack_88;
      *(undefined8 *)(param_1 + 0x16) = local_70;
      *(undefined8 *)(param_1 + 0x14) = uStack_78;
      *(ulong *)(param_1 + 2) = uStack_c0;
      *(undefined8 *)param_1 = local_c8;
      *(undefined8 *)(param_1 + 6) = local_b0;
      *(long *)(param_1 + 4) = uStack_b8;
      *puVar4 = uVar1;
    }
    else {
      piVar5 = (int *)0x0;
    }
  }
  if (*(long *)(lVar2 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(piVar5);
  }
  return;
}

/* ===== FUN_0014a254 @ 0014a254 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0014a254(long param_1)

{
  char cVar1;
  bool bVar2;
  uint uVar3;
  int iVar4;
  long lVar5;
  undefined **ppuVar6;
  
  if (param_1 != 0) {
    lVar5 = 0;
    ppuVar6 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar4 = strcmp("BattleTextChat",*ppuVar6);
      if (iVar4 == 0) {
        if ((-1 < (int)lVar5) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar5 * 4) == 1)) {
          DAT_001e01c8 = FUN_00139fcc();
          if (DAT_001e01c8 != 0) {
            DAT_001e01d0 = *(undefined8 *)(param_1 + 0x28);
            return;
          }
          goto LAB_0014a2f4;
        }
        break;
      }
      lVar5 = lVar5 + 1;
      ppuVar6 = ppuVar6 + 3;
    } while (lVar5 != 0x37);
  }
  DAT_001e01c8 = 0;
LAB_0014a2f4:
  DAT_001e01d0 = 0;
  do {
    uVar3 = _DAT_001e01d8;
    cVar1 = '\x01';
    bVar2 = (bool)ExclusiveMonitorPass(0x1e01d8,0x10);
    if (bVar2) {
      _DAT_001e01d8 = CONCAT31(DAT_001e01d8_1,1);
      cVar1 = ExclusiveMonitorsStatus();
    }
  } while (cVar1 != '\0');
  if ((uVar3 & 1) == 0) {
    memset(&DAT_001e01e8,0,0x784);
    DAT_001e01e0 = 0;
    _DAT_001e01d8 = 0;
  }
  return;
}

/* ===== FUN_0014b2ec @ 0014b2ec [libNexusEvasionRuntime69252.so] ===== */

void FUN_0014b2ec(void)

{
  char *pcVar1;
  undefined4 uVar2;
  char cVar3;
  long lVar4;
  undefined4 uVar5;
  bool bVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  undefined4 *puVar10;
  int *piVar11;
  ulong uVar12;
  long lVar13;
  undefined **ppuVar14;
  int local_c8 [24];
  long local_68;
  
  lVar4 = tpidr_el0;
  local_68 = *(long *)(lVar4 + 0x28);
  puVar10 = (undefined4 *)__errno();
  uVar2 = *puVar10;
  local_c8[0] = 0;
  iVar7 = FUN_001428fc(puVar10,DAT_001e0978 + 0xd06d98,local_c8,4);
  if (((iVar7 == 0) || (local_c8[0] != DAT_0020af60)) || (iVar7 = FUN_0014a9a0(), iVar7 == 0)) {
    DAT_001e12d0._0_4_ = 0;
    DAT_001e12d8._4_4_ = 3;
    DAT_001e12d0._4_4_ = 0;
    iVar7 = FUN_0014bdb4(DAT_0020af60);
    pcVar1 = "init_guard_owner_changed";
    if (iVar7 != 0) {
      pcVar1 = "init_guard_rolled_back";
    }
    FUN_001417c8("script_port_fonts",pcVar1,0);
    *puVar10 = uVar2;
    if (iVar7 != 0) {
      (*(code *)(DAT_001e0978 + 0xd06d98))();
    }
    goto LAB_0014b60c;
  }
  piVar11 = (int *)FUN_001bd828(&DAT_001cfcb8);
  iVar7 = *piVar11;
  lVar13 = 0;
  ppuVar14 = &PTR_s_DisablePinAnimation_001c2838;
  *piVar11 = iVar7 + 1;
  do {
    iVar8 = strcmp("Font",*ppuVar14);
    if (iVar8 == 0) {
      if (-1 < (int)lVar13) {
        uVar12 = *(ulong *)((long)&DAT_001dfea0 + lVar13 * 4);
        if (iVar7 == 0) goto LAB_0014b454;
        goto LAB_0014b4e8;
      }
      break;
    }
    lVar13 = lVar13 + 1;
    ppuVar14 = ppuVar14 + 3;
  } while (lVar13 != 0x37);
  uVar12 = 0;
  if (iVar7 == 0) {
LAB_0014b454:
    if (((int)DAT_001e12d0 == 0) || ((int)DAT_001dfff0 == 0)) goto LAB_0014b4e8;
    bVar6 = (uint)uVar12 < 6;
    if ((uint)uVar12 - 1 < 5) {
      (*(code *)(DAT_001e0978 + 0x66ae58))(local_c8,(&PTR_DAT_001c2d88)[(uVar12 & 0xffffffff) * 3]);
      (*(code *)(DAT_001e0978 + 0x75da50))(local_c8,0);
      (*(code *)(DAT_001e0978 + 0x66ad48))(local_c8);
    }
  }
  else {
LAB_0014b4e8:
    bVar6 = false;
  }
  *puVar10 = uVar2;
  (*DAT_0020af70)();
  lVar13 = CONCAT44(uRam00000000001cfafc,DAT_001cfaf4._4_4_);
  uVar2 = *puVar10;
  if (bVar6) {
    iVar9 = FUN_0014b8e8(uVar12 & 0xffffffff);
    iVar8 = (uint)(iVar9 == 0) << 1;
    uVar5 = (int)uVar12;
    if (iVar9 == 0) {
      FUN_0014b8e8();
      DAT_001cfaf4._0_4_ = 0xffffffff;
      uVar5 = (undefined4)DAT_001cfaf4;
    }
    DAT_001cfaf4._0_4_ = uVar5;
    lVar13 = CONCAT44(uRam00000000001cfafc,DAT_001cfaf4._4_4_);
    DAT_001e12d0._4_4_ = 0;
    do {
      cVar3 = '\x01';
      bVar6 = (bool)ExclusiveMonitorPass(0x1cfaf8,0x10);
      if (bVar6) {
        cVar3 = ExclusiveMonitorsStatus();
        lVar13 = lVar13 + 1;
      }
      uRam00000000001cfafc = (undefined4)((ulong)lVar13 >> 0x20);
      DAT_001cfaf4._4_4_ = (undefined4)lVar13;
    } while (cVar3 != '\0');
    DAT_001e12d8._4_4_ = iVar8;
    snprintf((char *)local_c8,0x60,"choice=%d applied=%d error=%u loaded_mask=%u",
             uVar12 & 0xffffffff,CONCAT44(DAT_001cfaf4._4_4_,(undefined4)DAT_001cfaf4),
             (ulong)(iVar9 == 0) << 1,CONCAT44(iVar8,(undefined4)DAT_001e12d8));
    FUN_001417c8("script_port_fonts",local_c8,0);
    lVar13 = CONCAT44(uRam00000000001cfafc,DAT_001cfaf4._4_4_);
    if (iVar7 == 0) {
      DAT_001e12d8._0_4_ = FUN_0014ce5c();
      lVar13 = CONCAT44(uRam00000000001cfafc,DAT_001cfaf4._4_4_);
      DAT_0020af98 = 0;
    }
  }
  else if (iVar7 == 0) {
    DAT_001e12d8._0_4_ = FUN_0014ce5c();
    lVar13 = CONCAT44(uRam00000000001cfafc,DAT_001cfaf4._4_4_);
    DAT_0020af98 = 0;
    do {
      cVar3 = '\x01';
      bVar6 = (bool)ExclusiveMonitorPass(0x1cfaf8,0x10);
      if (bVar6) {
        cVar3 = ExclusiveMonitorsStatus();
        lVar13 = lVar13 + 1;
      }
    } while (cVar3 != '\0');
  }
  uRam00000000001cfafc = (undefined4)((ulong)lVar13 >> 0x20);
  DAT_001cfaf4._4_4_ = (undefined4)lVar13;
  *piVar11 = *piVar11 + -1;
  *puVar10 = uVar2;
LAB_0014b60c:
  if (*(long *)(lVar4 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

/* ===== FUN_00164dfc @ 00164dfc [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int FUN_00164dfc(undefined8 param_1,long param_2,undefined8 *param_3)

{
  undefined1 (*pauVar1) [16];
  char cVar2;
  bool bVar3;
  long lVar4;
  undefined1 auVar5 [16];
  undefined1 auVar6 [16];
  undefined8 uVar7;
  int iVar8;
  int iVar9;
  uint uVar10;
  int iVar11;
  void *pvVar12;
  undefined8 uVar13;
  undefined4 uVar14;
  ulong uVar15;
  int *piVar16;
  undefined8 uVar17;
  undefined1 (*pauVar18) [16];
  ulong uVar19;
  long lVar20;
  ulong uVar21;
  undefined **ppuVar22;
  long lVar23;
  uint uVar24;
  undefined8 *puVar25;
  ushort uVar26;
  undefined1 auVar27 [16];
  undefined1 auVar28 [16];
  undefined1 auVar29 [16];
  undefined1 auVar30 [16];
  undefined1 auVar31 [16];
  int local_28c;
  uint local_288;
  int local_284;
  ulong local_280;
  long local_278;
  uint local_270;
  undefined4 uStack_26c;
  int local_268 [2];
  undefined8 uStack_260;
  undefined1 local_258 [16];
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
  undefined8 uStack_1f8;
  undefined8 uStack_1f0;
  undefined8 local_1e8;
  undefined8 uStack_1e0;
  long local_68;
  
  iVar9 = DAT_00209cd8;
  lVar4 = tpidr_el0;
  local_68 = *(long *)(lVar4 + 0x28);
  if (DAT_00209cd8 == 0) {
    iVar11 = 0;
  }
  else {
    iVar8 = gettid();
    iVar11 = 0;
    if ((((iVar9 == iVar8) && (iVar11 = 0, DAT_0020f6cc != 0)) && ((int)DAT_0020f638 != 0)) &&
       (DAT_0020f6c8 < 0x21)) {
      if (((DAT_0020f6f8 == *(long *)(*(long *)(param_2 + 0x10) + 0x20)) &&
          (DAT_0020f6f0 == *(long *)(*(long *)(param_2 + 0x10) + 0x18))) &&
         ((DAT_0020f668 == *(long *)(param_2 + 0x28) &&
          (iVar9 = FUN_00150bf0(DAT_0020f670,DAT_0020f680), lVar20 = DAT_0020f6f0,
          uVar10 = DAT_0020f6c8, uVar14 = DAT_0020f638._4_4_, uVar7 = _UNK_00112a48,
          uVar13 = _DAT_00112a40, iVar9 != 0)))) {
        lVar23 = DAT_0020f6f8;
        if ((((DAT_0021e8d8 & 1) == 0) || (DAT_0021e8e8 != DAT_0020f6f8)) ||
           ((lVar23 = DAT_0021e8e8, DAT_0021e8f0 != DAT_0020f6f0 ||
            ((DAT_0021e8f8 != *(long *)(param_2 + 0x28) ||
             (lVar23 = DAT_0021e8e8, DAT_002204b8 != DAT_0020f6c8)))))) {
          uVar17 = *(undefined8 *)(param_2 + 0x28);
          *param_3 = 0;
          param_3[1] = lVar23;
          param_3[2] = lVar20;
          param_3[3] = uVar17;
          *(undefined4 *)(param_3 + 4) = uVar14;
          *(undefined8 *)((long)param_3 + 0x2c) = uVar7;
          *(undefined8 *)((long)param_3 + 0x24) = uVar13;
          memset((void *)((long)param_3 + 0x34),0,0x1ba4);
          *(uint *)(param_3 + 0x37b) = uVar10;
          *(undefined4 *)((long)param_3 + 0x1cdc) = 0;
          *(undefined8 *)((long)param_3 + 0x1be4) = 0;
          *(undefined8 *)((long)param_3 + 0x1bdc) = 0;
          *(undefined8 *)((long)param_3 + 0x1bf4) = 0;
          *(undefined8 *)((long)param_3 + 0x1bec) = 0;
          *(undefined8 *)((long)param_3 + 0x1c04) = 0;
          *(undefined8 *)((long)param_3 + 0x1bfc) = 0;
          *(undefined8 *)((long)param_3 + 0x1c14) = 0;
          *(undefined8 *)((long)param_3 + 0x1c0c) = 0;
          *(undefined8 *)((long)param_3 + 0x1c24) = 0;
          *(undefined8 *)((long)param_3 + 0x1c1c) = 0;
          *(undefined8 *)((long)param_3 + 0x1c34) = 0;
          *(undefined8 *)((long)param_3 + 0x1c2c) = 0;
          *(undefined8 *)((long)param_3 + 0x1c44) = 0;
          *(undefined8 *)((long)param_3 + 0x1c3c) = 0;
          *(undefined8 *)((long)param_3 + 0x1c54) = 0;
          *(undefined8 *)((long)param_3 + 0x1c4c) = 0;
          *(undefined8 *)((long)param_3 + 0x1c64) = 0;
          *(undefined8 *)((long)param_3 + 0x1c5c) = 0;
          *(undefined8 *)((long)param_3 + 0x1c74) = 0;
          *(undefined8 *)((long)param_3 + 0x1c6c) = 0;
          *(undefined8 *)((long)param_3 + 0x1c84) = 0;
          *(undefined8 *)((long)param_3 + 0x1c7c) = 0;
          *(undefined8 *)((long)param_3 + 0x1c94) = 0;
          *(undefined8 *)((long)param_3 + 0x1c8c) = 0;
          *(undefined8 *)((long)param_3 + 0x1ca4) = 0;
          *(undefined8 *)((long)param_3 + 0x1c9c) = 0;
          *(undefined8 *)((long)param_3 + 0x1cb4) = 0;
          *(undefined8 *)((long)param_3 + 0x1cac) = 0;
          *(undefined8 *)((long)param_3 + 0x1cc4) = 0;
          *(undefined8 *)((long)param_3 + 0x1cbc) = 0;
          *(undefined8 *)((long)param_3 + 0x1cd4) = 0;
          *(undefined8 *)((long)param_3 + 0x1ccc) = 0;
          FUN_001659c8(local_268,&DAT_0020f680,1,uVar14,*(undefined8 *)(param_2 + 0x28));
          param_3[0x14] = uStack_210;
          param_3[0x13] = local_218;
          param_3[0x16] = uStack_200;
          param_3[0x15] = local_208;
          param_3[0x18] = uStack_1f0;
          param_3[0x17] = uStack_1f8;
          param_3[0x1a] = uStack_1e0;
          param_3[0x19] = local_1e8;
          param_3[0x12] = uStack_220;
          param_3[0x11] = local_228;
          param_3[0xc] = local_258._8_8_;
          param_3[0xb] = local_258._0_8_;
          param_3[0xe] = uStack_240;
          param_3[0xd] = local_248;
          param_3[0x10] = uStack_230;
          param_3[0xf] = local_238;
          param_3[10] = uStack_260;
          param_3[9] = CONCAT44(local_268[1],local_268[0]);
          if (*(int *)(param_3 + 0x37b) != 0) {
            lVar20 = 0;
            uVar21 = 0;
            puVar25 = param_3 + 0x1b;
            do {
              FUN_001659c8(local_268,DAT_0020f6c0 + lVar20,0,*(undefined4 *)(param_3 + 4),
                           *(undefined8 *)(param_2 + 0x28));
              uVar21 = uVar21 + 1;
              lVar20 = lVar20 + 0x40;
              puVar25[0xd] = uStack_200;
              puVar25[0xc] = local_208;
              puVar25[0xf] = uStack_1f0;
              puVar25[0xe] = uStack_1f8;
              puVar25[0x11] = uStack_1e0;
              puVar25[0x10] = local_1e8;
              puVar25[5] = uStack_240;
              puVar25[4] = local_248;
              puVar25[7] = uStack_230;
              puVar25[6] = local_238;
              puVar25[9] = uStack_220;
              puVar25[8] = local_228;
              puVar25[0xb] = uStack_210;
              puVar25[10] = local_218;
              puVar25[1] = uStack_260;
              *puVar25 = CONCAT44(local_268[1],local_268[0]);
              puVar25[3] = local_258._8_8_;
              puVar25[2] = local_258._0_8_;
              puVar25 = puVar25 + 0x12;
            } while (uVar21 < *(uint *)(param_3 + 0x37b));
          }
          pvVar12 = (void *)FUN_001658b0(param_3);
          if ((DAT_00220780 != 0) && ((int)_DAT_00220784 == 1)) {
            lVar20 = 0;
            ppuVar22 = &PTR_s_DisablePinAnimation_001c2838;
LAB_00165134:
            uVar10 = strcmp("AllyRespawnTimer",*ppuVar22);
            pvVar12 = (void *)(ulong)uVar10;
            if (uVar10 != 0) goto code_r0x00165144;
            if (((-1 < (int)lVar20) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar20 * 4) != 0)
                ) && (pvVar12 = (void *)FUN_00165f8c(DAT_0020f678), (int)pvVar12 != 0)) {
              local_28c = 0;
              pvVar12 = (void *)FUN_001428fc(pvVar12,DAT_0020f678 + 0x124,&local_28c,4);
              lVar20 = DAT_0020f678;
              if ((((int)pvVar12 != 0) && (local_28c != 9)) &&
                 ((local_28c != 0x4e && (local_28c != 0x45)))) {
                local_280 = 0;
                local_278 = 0;
                local_288 = 0;
                pvVar12 = (void *)FUN_001428fc(pvVar12,DAT_0020f678 + 0x128,&local_270,8);
                if ((((int)pvVar12 != 0) && (0x11fff < CONCAT44(uStack_26c,local_270) + 0x2000U)) &&
                   (((local_270 & 7) == 0 &&
                    ((((pvVar12 = (void *)FUN_001428fc(pvVar12,CONCAT44(uStack_26c,local_270) +
                                                               0x30c,&local_284,4),
                       (int)pvVar12 != 0 && (0 < local_284)) && (local_284 < 0xb5)) &&
                     (pvVar12 = (void *)FUN_001428fc(pvVar12,lVar20 + 0x80,&local_278,8),
                     (int)pvVar12 != 0)))))) {
                  iVar9 = local_284;
                  if (local_278 == 0) goto LAB_001652c8;
                  pvVar12 = (void *)FUN_001428fc(pvVar12,local_278 + 0xc,&local_288,4);
                  if ((((int)pvVar12 != 0) && (-1 < (int)local_288)) && ((int)local_288 < 0x81)) {
                    iVar9 = local_284;
                    if (local_288 == 0) goto LAB_001652c8;
                    pvVar12 = (void *)FUN_001428fc(pvVar12,local_278,&local_280,8);
                    if ((((int)pvVar12 != 0) && (0x11fff < local_280 + 0x2000)) &&
                       (((local_280 & 7) == 0 &&
                        (pvVar12 = (void *)FUN_001428fc(pvVar12,local_280,local_268,
                                                        (long)(int)local_288 << 2),
                        (int)pvVar12 != 0)))) {
                      uVar21 = (ulong)local_288;
                      iVar9 = local_284;
                      if ((int)local_288 < 1) goto LAB_001652c8;
                      if (7 < local_288) {
                        uVar15 = uVar21 & 0xfffffff8;
                        auVar27 = ZEXT816(0);
                        pauVar18 = &local_258;
                        auVar29 = ZEXT816(0);
                        uVar19 = uVar15;
                        do {
                          pauVar1 = pauVar18 + -1;
                          auVar5 = *pauVar18;
                          pauVar18 = pauVar18 + 2;
                          uVar19 = uVar19 - 8;
                          auVar30._8_4_ = 0x22;
                          auVar30._0_8_ = 0x2200000022;
                          auVar30._12_4_ = 0x22;
                          auVar30 = NEON_cmeq(*pauVar1,auVar30,4);
                          auVar31._8_4_ = 0x22;
                          auVar31._0_8_ = 0x2200000022;
                          auVar31._12_4_ = 0x22;
                          auVar31 = NEON_cmeq(auVar5,auVar31,4);
                          auVar5._8_4_ = 1;
                          auVar5._0_8_ = 0x100000001;
                          auVar5._12_4_ = 1;
                          auVar27 = NEON_bit(auVar27,auVar5,auVar30,1);
                          auVar6._8_4_ = 1;
                          auVar6._0_8_ = 0x100000001;
                          auVar6._12_4_ = 1;
                          auVar29 = NEON_bit(auVar29,auVar6,auVar31,1);
                        } while (uVar19 != 0);
                        auVar28[0] = auVar27[0] | auVar29[0];
                        auVar28[1] = auVar27[1] | auVar29[1];
                        auVar28[2] = auVar27[2] | auVar29[2];
                        auVar28[3] = auVar27[3] | auVar29[3];
                        auVar28[4] = auVar27[4] | auVar29[4];
                        auVar28[5] = auVar27[5] | auVar29[5];
                        auVar28[6] = auVar27[6] | auVar29[6];
                        auVar28[7] = auVar27[7] | auVar29[7];
                        auVar28[8] = auVar27[8] | auVar29[8];
                        auVar28[9] = auVar27[9] | auVar29[9];
                        auVar28[10] = auVar27[10] | auVar29[10];
                        auVar28[0xb] = auVar27[0xb] | auVar29[0xb];
                        auVar28[0xc] = auVar27[0xc] | auVar29[0xc];
                        auVar28[0xd] = auVar27[0xd] | auVar29[0xd];
                        auVar28[0xe] = auVar27[0xe] | auVar29[0xe];
                        auVar28[0xf] = auVar27[0xf] | auVar29[0xf];
                        auVar27 = NEON_cmtst(auVar28,auVar28,4);
                        uVar26 = NEON_umaxv(CONCAT26(auVar27._12_2_,
                                                     CONCAT24(auVar27._8_2_,
                                                              CONCAT22(auVar27._4_2_,auVar27._0_2_))
                                                    ),2);
                        uVar26 = uVar26 & 1;
                        if (uVar15 != uVar21) {
LAB_001657d0:
                          lVar20 = uVar21 - uVar15;
                          piVar16 = local_268 + uVar15;
                          do {
                            if (*piVar16 == 0x22) {
                              uVar26 = 1;
                            }
                            lVar20 = lVar20 + -1;
                            piVar16 = piVar16 + 1;
                          } while (lVar20 != 0);
                        }
                        if (uVar26 != 0 && local_284 < 9) {
                          iVar9 = 9;
LAB_001652cc:
                          uVar21 = (ulong)DAT_0020f6c8;
                          if (DAT_0020f6c8 == 0) {
                            iVar11 = 0;
                          }
                          else {
                            iVar11 = 0;
                            piVar16 = (int *)(DAT_0020f6c0 + 0x2c);
                            do {
                              if ((((*(long *)(piVar16 + -0xb) != DAT_0020f680) &&
                                   (piVar16[-1] != 0)) && (*piVar16 != 0)) &&
                                 (((uint)piVar16[-5] < 10 && (piVar16[-4] == DAT_0020f69c)))) {
                                iVar11 = iVar11 + 1;
                              }
                              piVar16 = piVar16 + 0x10;
                              uVar21 = uVar21 - 1;
                            } while (uVar21 != 0);
                          }
                          if ((DAT_00220728 == param_3[1]) && (DAT_00220730 == local_28c)) {
                            iVar8 = DAT_00220734;
                            if (iVar11 < DAT_00220734) {
                              uVar10 = DAT_00220738;
                              uVar21 = _DAT_00220738 & 0xffffffff;
                              if (DAT_00220738 < 8) {
                                uVar13 = param_3[3];
                                lVar20 = uVar21 + 1;
                                (&DAT_00220740)[uVar21] = uVar13;
                                lVar23 = lVar20;
                                if ((iVar11 + 1 < iVar8) && (uVar10 < 7)) {
                                  lVar23 = uVar21 + 2;
                                  (&DAT_00220740)[lVar20] = uVar13;
                                  if ((iVar11 + 2 < iVar8) && (uVar10 < 6)) {
                                    lVar20 = uVar21 + 3;
                                    (&DAT_00220740)[lVar23] = uVar13;
                                    lVar23 = lVar20;
                                    if ((iVar11 + 3 < iVar8) && (uVar10 < 5)) {
                                      lVar23 = uVar21 + 4;
                                      (&DAT_00220740)[lVar20] = uVar13;
                                      if ((iVar11 + 4 < iVar8) && (uVar10 < 4)) {
                                        lVar20 = uVar21 + 5;
                                        (&DAT_00220740)[lVar23] = uVar13;
                                        lVar23 = lVar20;
                                        if ((iVar11 + 5 < iVar8) && (uVar10 < 3)) {
                                          lVar23 = uVar21 + 6;
                                          (&DAT_00220740)[lVar20] = uVar13;
                                          if ((iVar11 + 6 < iVar8) && (uVar10 < 2)) {
                                            lVar20 = uVar21 + 7;
                                            (&DAT_00220740)[lVar23] = uVar13;
                                            lVar23 = lVar20;
                                            if ((iVar11 + 7 < iVar8) && (uVar10 == 0)) {
                                              lVar23 = uVar21 + 8;
                                              (&DAT_00220740)[lVar20] = uVar13;
                                            }
                                          }
                                        }
                                      }
                                    }
                                  }
                                }
                                _DAT_00220738 = CONCAT44(DAT_00220738_4,(int)lVar23);
                              }
                            }
                            uVar10 = iVar11 - iVar8;
                            uVar24 = DAT_00220738;
                            if (uVar10 != 0 && iVar8 <= iVar11) {
                              if (DAT_00220738 <= uVar10) {
                                uVar10 = DAT_00220738;
                              }
                              uVar24 = DAT_00220738 - uVar10;
                              pvVar12 = memmove(&DAT_00220740,&DAT_00220740 + uVar10,
                                                (ulong)uVar24 << 3);
                              _DAT_00220738 = CONCAT44(DAT_00220738_4,uVar24);
                            }
                            _DAT_00220730 = CONCAT44(iVar11,DAT_00220730);
                            if (((uVar24 != 0) && ((long)DAT_00220740 <= (long)param_3[3])) &&
                               (iVar9 = (int)((double)iVar9 +
                                              ((double)(long)param_3[3] - (double)DAT_00220740) /
                                              -1000.0 + -1.0), 0 < iVar9)) {
                              *(int *)((long)param_3 + 0x3c) = iVar9;
                              *(uint *)(param_3 + 8) = uVar24;
                            }
                          }
                          else {
                            DAT_00220778 = 0;
                            DAT_00220740 = 0;
                            _DAT_00220738 = 0;
                            uRam0000000000220750 = 0;
                            _DAT_00220748 = 0;
                            uRam0000000000220760 = 0;
                            _DAT_00220758 = 0;
                            uRam0000000000220770 = 0;
                            _DAT_00220768 = 0;
                            _DAT_00220730 = CONCAT44(iVar11,local_28c);
                            DAT_00220728 = param_3[1];
                          }
                          goto LAB_00165314;
                        }
LAB_001652c8:
                        if (iVar9 != 0) goto LAB_001652cc;
                        goto LAB_001652f8;
                      }
                      uVar15 = 0;
                      uVar26 = 0;
                      goto LAB_001657d0;
                    }
                  }
                }
              }
            }
          }
LAB_001652f8:
          DAT_00220778 = 0;
          DAT_00220740 = 0;
          _DAT_00220738 = 0;
          uRam0000000000220750 = 0;
          _DAT_00220748 = 0;
          uRam0000000000220760 = 0;
          _DAT_00220758 = 0;
          uRam0000000000220770 = 0;
          _DAT_00220768 = 0;
          _DAT_00220730 = 0;
          DAT_00220728 = 0;
LAB_00165314:
          local_268[0] = 0;
          local_270 = 0;
          if (((((DAT_0020f728 != 0) &&
                (uVar13 = FUN_001428fc(pvVar12,DAT_0020f728 + 0xc4,local_268,4), (int)uVar13 != 0))
               && ((iVar9 = FUN_001428fc(uVar13,DAT_0020f728 + 200,&local_270,4), iVar9 != 0 &&
                   ((0 < local_268[0] && (0 < (int)local_270)))))) && (local_268[0] < 0x65)) &&
             ((int)local_270 < 0x65)) {
            *(int *)((long)param_3 + 0x34) = local_268[0];
            *(uint *)(param_3 + 7) = local_270;
          }
          lVar20 = 0;
          uVar21 = *(ulong *)(param_2 + 0x28);
          lVar23 = param_3[1];
          ppuVar22 = &PTR_s_DisablePinAnimation_001c2838;
          do {
            iVar9 = strcmp("ShowBattleConnectionIndicator",*ppuVar22);
            if (iVar9 == 0) {
              if ((((-1 < (int)lVar20) &&
                   ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar20 * 4) != 0)) &&
                  (DAT_0020c0c8 == lVar23)) && (DAT_0020c0d0 <= uVar21)) {
                goto LAB_00165418;
              }
              break;
            }
            lVar20 = lVar20 + 1;
            ppuVar22 = ppuVar22 + 3;
          } while (lVar20 != 0x37);
          DAT_0020c0d8 = 0;
          DAT_0020c0c8 = 0;
          DAT_0020c0d0 = 0;
          _DAT_0020c0d8 = 0;
LAB_00165418:
          *(undefined4 *)param_3 = DAT_0020c0d8;
          lVar20 = 0;
          uVar21 = *(ulong *)(param_2 + 0x28);
          ppuVar22 = &PTR_s_DisablePinAnimation_001c2838;
          lVar23 = param_3[1];
          do {
            iVar9 = strcmp("ShowDPS",*ppuVar22);
            if (iVar9 == 0) {
              if (-1 < (int)lVar20) {
                iVar9 = (int)*(undefined8 *)((long)&DAT_001dfea0 + lVar20 * 4);
                goto LAB_00165484;
              }
              break;
            }
            lVar20 = lVar20 + 1;
            ppuVar22 = ppuVar22 + 3;
          } while (lVar20 != 0x37);
          iVar9 = 0;
LAB_00165484:
          do {
            uVar10 = _DAT_0020b098;
            cVar2 = '\x01';
            bVar3 = (bool)ExclusiveMonitorPass(0x20b098,0x10);
            if (bVar3) {
              _DAT_0020b098 = CONCAT31(DAT_0020b098_1,1);
              cVar2 = ExclusiveMonitorsStatus();
            }
          } while (cVar2 != '\0');
          if ((uVar10 & 1) == 0) {
            if (iVar9 == 0) {
              memset(&DAT_0020b0a8,0,0x1008);
              lVar20 = 0;
              DAT_0020b0a0 = DAT_0020c0b0;
            }
            else if ((DAT_0020b0a0 == lVar23) && (uVar15 = (ulong)DAT_0020b0ac, DAT_0020b0ac != 0))
            {
              lVar20 = 0;
              piVar16 = &DAT_0020b0b8;
              do {
                if ((*(ulong *)(piVar16 + -2) <= uVar21) &&
                   (uVar21 - *(ulong *)(piVar16 + -2) < 0x3e9)) {
                  lVar20 = lVar20 + *piVar16;
                }
                piVar16 = piVar16 + 4;
                uVar15 = uVar15 - 1;
              } while (uVar15 != 0);
            }
            else {
              lVar20 = 0;
            }
            _DAT_0020b098 = 0;
            if (lVar20 < -9999999) {
              lVar20 = -10000000;
            }
            if (9999999 < lVar20) {
              lVar20 = 10000000;
            }
            uVar14 = (undefined4)lVar20;
          }
          else {
            uVar14 = 0;
          }
          *(undefined4 *)((long)param_3 + 4) = uVar14;
          iVar11 = FUN_0018f704(0,FUN_001428fc,DAT_001e0978,DAT_0020f670,param_3[1],param_3 + 0x37c)
          ;
          if (iVar11 != 0) {
            memcpy(&DAT_0021e8e0,param_3,0x1ce0);
          }
          DAT_0021e8d8 = iVar11 != 0;
        }
        else {
          memcpy(param_3,&DAT_0021e8e0,0x1ce0);
          FUN_001658b0(param_3);
          iVar11 = 1;
        }
      }
      else {
        iVar11 = 0;
      }
    }
  }
  if (*(long *)(lVar4 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return iVar11;
code_r0x00165144:
  lVar20 = lVar20 + 1;
  ppuVar22 = ppuVar22 + 3;
  if (lVar20 == 0x37) goto LAB_001652f8;
  goto LAB_00165134;
}

/* ===== FUN_00166588 @ 00166588 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_00166588(undefined8 param_1,undefined8 *param_2)

{
  int iVar1;
  undefined8 uVar2;
  uint uVar3;
  long lVar4;
  undefined **ppuVar5;
  
  uVar2 = FUN_00166934(param_2);
  if ((int)uVar2 != 0) {
    lVar4 = 0;
    ppuVar5 = &PTR_s_DisablePinAnimation_001c2838;
    *(undefined4 *)((long)param_2 + 0x5c) = 0;
    *(undefined4 *)(param_2 + 0xc) = 0;
    do {
      iVar1 = strcmp("TileGrid",*ppuVar5);
      if (iVar1 == 0) {
        if (-1 < (int)lVar4) {
          uVar3 = (uint)((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar4 * 4) == 1);
          goto LAB_00166608;
        }
        break;
      }
      lVar4 = lVar4 + 1;
      ppuVar5 = ppuVar5 + 3;
    } while (lVar4 != 0x37);
    uVar3 = 0;
LAB_00166608:
    lVar4 = 0;
    ppuVar5 = &PTR_s_DisablePinAnimation_001c2838;
    *(uint *)((long)param_2 + 100) = uVar3;
    *(undefined4 *)(param_2 + 0xd) = 0;
    do {
      iVar1 = strcmp("SmoothHudGraph",*ppuVar5);
      if (iVar1 == 0) {
        if (-1 < (int)lVar4) {
          uVar3 = (uint)((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar4 * 4) == 1);
          goto LAB_00166668;
        }
        break;
      }
      lVar4 = lVar4 + 1;
      ppuVar5 = ppuVar5 + 3;
    } while (lVar4 != 0x37);
    uVar3 = 0;
LAB_00166668:
    lVar4 = 0;
    ppuVar5 = &PTR_s_DisablePinAnimation_001c2838;
    *(uint *)((long)param_2 + 0x6c) = uVar3;
    *(undefined4 *)(param_2 + 0xe) = 0;
    do {
      iVar1 = strcmp("AllyRespawnTimer",*ppuVar5);
      if (iVar1 == 0) {
        if (-1 < (int)lVar4) {
          uVar3 = (uint)((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar4 * 4) == 1);
          goto LAB_001666c8;
        }
        break;
      }
      lVar4 = lVar4 + 1;
      ppuVar5 = ppuVar5 + 3;
    } while (lVar4 != 0x37);
    uVar3 = 0;
LAB_001666c8:
    uRam00000000002205e8 = param_2[5];
    _DAT_002205e0 = param_2[4];
    uRam00000000002205f8 = param_2[7];
    _DAT_002205f0 = param_2[6];
    *param_2 = 0;
    uVar2 = 1;
    *(uint *)((long)param_2 + 0x74) = uVar3;
    DAT_002205c8 = param_2[1];
    _DAT_002205c0 = *param_2;
    uRam00000000002205d8 = param_2[3];
    _DAT_002205d0 = param_2[2];
    _DAT_00220618 = param_2[0xb];
    _DAT_00220610 = param_2[10];
    uRam0000000000220628 = param_2[0xd];
    _DAT_00220620 = param_2[0xc];
    DAT_00220630 = param_2[0xe];
    uRam0000000000220608 = param_2[9];
    _DAT_00220600 = param_2[8];
  }
  return uVar2;
}

/* ===== FUN_00166718 @ 00166718 [libNexusEvasionRuntime69252.so] ===== */

void FUN_00166718(undefined8 param_1,long param_2,long param_3,long param_4)

{
  long lVar1;
  int iVar2;
  int iVar3;
  ulong uVar4;
  long lVar5;
  undefined **ppuVar6;
  ushort uVar7;
  undefined1 auVar8 [16];
  undefined1 auStack_c0 [8];
  long local_b8;
  undefined4 local_b0;
  undefined4 local_a0;
  undefined8 local_70;
  int local_68;
  int local_64;
  long local_48;
  
  iVar3 = DAT_00209cd8;
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  if (DAT_00209cd8 == 0) {
    uVar4 = 0;
  }
  else {
    iVar2 = gettid();
    uVar4 = 0;
    if (((iVar3 == iVar2) && (DAT_0020f6f8 == param_2)) && (DAT_0020f6f0 == param_3)) {
      iVar3 = FUN_00166934(auStack_c0);
      uVar4 = 0;
      if ((iVar3 != 0) && (local_b8 == param_4)) {
        auVar8._4_4_ = local_a0;
        auVar8._0_4_ = local_b0;
        auVar8._8_8_ = local_70;
        auVar8 = NEON_cmtst(auVar8,auVar8,4);
        uVar7 = NEON_umaxv(CONCAT26(auVar8._12_2_,
                                    CONCAT24(auVar8._8_2_,CONCAT22(auVar8._4_2_,auVar8._0_2_))),2);
        if (((uVar7 & 1) == 0) && ((local_68 == 0 && (local_64 == 0)))) {
          lVar5 = 0;
          ppuVar6 = &PTR_s_DisablePinAnimation_001c2838;
          do {
            iVar3 = strcmp("ShowBattleConnectionIndicator",*ppuVar6);
            if (iVar3 == 0) {
              if ((-1 < (int)lVar5) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar5 * 4) != 0))
              goto LAB_001668ec;
              break;
            }
            lVar5 = lVar5 + 1;
            ppuVar6 = ppuVar6 + 3;
          } while (lVar5 != 0x37);
          lVar5 = 0;
          ppuVar6 = &PTR_s_DisablePinAnimation_001c2838;
          do {
            iVar3 = strcmp("ShowDPS",*ppuVar6);
            if (iVar3 == 0) {
              if ((-1 < (int)lVar5) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar5 * 4) != 0))
              goto LAB_001668ec;
              break;
            }
            lVar5 = lVar5 + 1;
            ppuVar6 = ppuVar6 + 3;
          } while (lVar5 != 0x37);
          iVar3 = FUN_00137de4("TileGrid");
          if (((((iVar3 == 0) && (iVar3 = FUN_00137de4("SmoothHud"), iVar3 == 0)) &&
               (iVar3 = FUN_00137de4("SmoothHudGraph"), iVar3 == 0)) &&
              ((iVar3 = FUN_00137de4("ShowFPSCounter"), iVar3 == 0 &&
               (iVar3 = FUN_00137de4("ShowOwnPlayerCoordinates"), iVar3 == 0)))) &&
             (uVar4 = FUN_00137de4("AllyRespawnTimer"), (int)uVar4 == 0)) goto LAB_00166908;
        }
LAB_001668ec:
        iVar3 = FUN_00150bf0(DAT_0020f670,DAT_0020f680);
        uVar4 = (ulong)(iVar3 != 0);
      }
    }
  }
LAB_00166908:
  if (*(long *)(lVar1 + 0x28) != local_48) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar4);
  }
  return;
}

/* ===== FUN_0016b478 @ 0016b478 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0016b478(undefined8 param_1,int param_2,long param_3,ulong param_4)

{
  ulong uVar1;
  long lVar2;
  uint uVar3;
  int iVar4;
  ulong uVar5;
  ushort uVar6;
  undefined1 auVar7 [16];
  undefined1 auVar8 [16];
  timespec local_c0;
  int local_b0;
  undefined4 local_a0;
  undefined4 local_70;
  undefined4 uStack_6c;
  int iStack_68;
  int iStack_64;
  long local_48;
  
  lVar2 = tpidr_el0;
  uVar5 = 0;
  local_48 = *(long *)(lVar2 + 0x28);
  if (((param_2 != 2) || (param_3 == 0)) || (param_4 == 0)) goto LAB_0016b728;
  if ((((int)DAT_001dfff0 != 0) && ((int)_DAT_00220784 == 1)) &&
     ((DAT_002208f8 == param_3 && (DAT_00220908 == param_4)))) {
    uVar3 = clock_gettime(1,&local_c0);
    uVar5 = (ulong)uVar3;
    if (uVar3 != 0) {
      uVar5 = 0;
      goto LAB_0016b728;
    }
    uVar1 = local_c0.tv_sec * 1000 + (ulong)local_c0.tv_nsec / 1000000;
    if ((uVar1 <= DAT_00220900 - 1U) || (500 < uVar1 - DAT_00220900)) goto LAB_0016b728;
    iVar4 = FUN_00166934(&local_c0);
    uVar5 = 0;
    if ((iVar4 == 0) || (local_c0.tv_nsec != param_4)) goto LAB_0016b728;
    auVar7._12_4_ = uStack_6c;
    auVar7._8_4_ = local_70;
    auVar7._4_4_ = local_a0;
    auVar7._0_4_ = local_b0;
    auVar7 = NEON_cmtst(auVar7,auVar7,4);
    uVar6 = NEON_umaxv(CONCAT26(auVar7._12_2_,
                                CONCAT24(auVar7._8_2_,CONCAT22(auVar7._4_2_,auVar7._0_2_))),2);
    if ((((((uVar6 & 1) == 0) && (iStack_68 == 0)) && (iStack_64 == 0)) &&
        ((iVar4 = FUN_00137de4("ShowBattleConnectionIndicator"), iVar4 == 0 &&
         (iVar4 = FUN_00137de4("ShowDPS"), iVar4 == 0)))) &&
       ((iVar4 = FUN_00137de4("TileGrid"), iVar4 == 0 &&
        (((iVar4 = FUN_00137de4("SmoothHud"), iVar4 == 0 &&
          (iVar4 = FUN_00137de4("SmoothHudGraph"), iVar4 == 0)) &&
         ((iVar4 = FUN_00137de4("ShowFPSCounter"), iVar4 == 0 &&
          ((iVar4 = FUN_00137de4("ShowOwnPlayerCoordinates"), iVar4 == 0 &&
           (uVar5 = FUN_00137de4("AllyRespawnTimer"), (int)uVar5 == 0)))))))))) goto LAB_0016b728;
    if ((DAT_002208f8 == param_3) && (DAT_00220908 == param_4)) {
      auVar8._12_4_ = iStack_68;
      auVar8._8_4_ = uStack_6c;
      auVar8._4_4_ = local_a0;
      auVar8._0_4_ = local_70;
      uVar3 = 1;
      auVar7 = NEON_cmtst(auVar8,auVar8,4);
      uVar6 = NEON_umaxv(CONCAT26(auVar7._12_2_,
                                  CONCAT24(auVar7._8_2_,CONCAT22(auVar7._4_2_,auVar7._0_2_))),2);
      if (((uVar6 & 1) == 0) && (iStack_64 == 0)) {
        iVar4 = FUN_00137de4("ShowBattleConnectionIndicator");
        if (((iVar4 == 0) &&
            (((iVar4 = FUN_00137de4("ShowDPS"), iVar4 == 0 &&
              (iVar4 = FUN_00137de4("TileGrid"), iVar4 == 0)) &&
             (iVar4 = FUN_00137de4("SmoothHud"), iVar4 == 0)))) &&
           (((iVar4 = FUN_00137de4("SmoothHudGraph"), iVar4 == 0 &&
             (iVar4 = FUN_00137de4("ShowFPSCounter"), iVar4 == 0)) &&
            (iVar4 = FUN_00137de4("ShowOwnPlayerCoordinates"), iVar4 == 0)))) {
          iVar4 = FUN_00137de4("AllyRespawnTimer");
          uVar3 = (uint)(iVar4 != 0);
        }
        else {
          uVar3 = 1;
        }
      }
      uVar5 = (ulong)(uVar3 | (uint)(local_b0 != 0) << 1);
      goto LAB_0016b728;
    }
  }
  uVar5 = 0;
LAB_0016b728:
  if (*(long *)(lVar2 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar5);
}

/* ===== FUN_0016bd40 @ 0016bd40 [libNexusEvasionRuntime69252.so] ===== */

void FUN_0016bd40(void)

{
  long lVar1;
  long lVar2;
  bool bVar3;
  long lVar4;
  int iVar5;
  int iVar6;
  undefined8 *puVar7;
  long lVar8;
  undefined8 uVar9;
  long lVar10;
  uint uVar11;
  uint uVar12;
  uint local_b4;
  uint uStack_b0;
  uint local_ac [19];
  uint local_60;
  uint local_5c;
  long local_58;
  
  lVar2 = tpidr_el0;
  local_58 = *(long *)(lVar2 + 0x28);
  iVar5 = FUN_001419f8();
  if ((((iVar5 != 0) &&
       (iVar5 = FUN_0014b1d0(0xe45000,0xab8,
                             "f490a4b6045692dcb24a39b754bfeb2d0c668b17555b84f7fdb68d9f493d4d57"),
       iVar5 != 0)) &&
      (iVar5 = FUN_0014b1d0(0xe45f28,0x20,
                            "afe806a653e90c4c55961d4f5f9ebe3251e529d563cb3d4d72e6eda4666540eb"),
      iVar5 != 0)) &&
     ((iVar5 = FUN_0014b1d0(0x533eb0,0x10,
                            "431504d966d6b81b5ba5442c844791b524748bfcc4b28de510001b2096172b73"),
      iVar5 != 0 &&
      (iVar5 = FUN_0014b1d0(0x533ec0,0x10,
                            "4ead9da4fcd8c8d8f3e63f9b03c292b64c990bbe11ff82af6bf945c01178a658"),
      lVar4 = DAT_001e0978, iVar5 != 0)))) {
    lVar10 = DAT_001e0978 + 0xe45f28;
    puVar7 = (undefined8 *)FUN_00152af0(lVar10);
    if (((puVar7 != (undefined8 *)0x0) &&
        ((iVar5 = FUN_001428fc(puVar7,lVar10,&uStack_b0,4), iVar5 != 0 &&
         ((((uint)puVar7 | (uint)lVar4) & 3) == 0)))) &&
       (0xfffffffff0000002 < ((long)puVar7 - lVar10) - 0x7fffffdU)) {
      uVar11 = (uint)((long)puVar7 - lVar10);
      uVar12 = uVar11 + 3;
      if (-1 < (int)uVar11) {
        uVar12 = uVar11;
      }
      local_60 = uVar12 >> 2 & 0x3ffffff | 0x14000000;
      *puVar7 = 0xd61f020058000050;
      puVar7[1] = FUN_00176ad4;
      FUN_001bdaf0(puVar7,puVar7 + 2);
      iVar5 = mprotect(puVar7,DAT_00209d88,5);
      if (iVar5 == 0) {
        lVar1 = lVar4 + 0xe45f38;
        puVar7 = (undefined8 *)FUN_00152af0(lVar1);
        if ((((puVar7 != (undefined8 *)0x0) &&
             (iVar5 = FUN_001428fc(puVar7,lVar1,local_ac,4), iVar5 != 0)) &&
            ((((uint)puVar7 | (uint)lVar4) & 3) == 0)) &&
           (0xfffffffff0000002 < ((long)puVar7 - lVar1) - 0x7fffffdU)) {
          uVar11 = (uint)((long)puVar7 - lVar1);
          uVar12 = uVar11 + 3;
          if (-1 < (int)uVar11) {
            uVar12 = uVar11;
          }
          local_5c = uVar12 >> 2 & 0x3ffffff | 0x14000000;
          *puVar7 = 0xd61f020058000050;
          puVar7[1] = FUN_00176b48;
          FUN_001bdaf0(puVar7,puVar7 + 2);
          iVar5 = mprotect(puVar7,DAT_00209d88,5);
          if (iVar5 == 0) {
            iVar5 = FUN_001419f8();
            if ((iVar5 == 0) || (lVar8 = FUN_0014bef8(DAT_001cfb4c,&local_60,4,lVar10), lVar8 != 4))
            {
LAB_0016c134:
              bVar3 = false;
            }
            else {
              uVar9 = FUN_001bdaf0(lVar10,lVar4 + 0xe45f2c);
              iVar5 = FUN_001428fc(uVar9,lVar10,&local_b4,4);
              if ((iVar5 == 0) || (local_b4 != local_60)) goto LAB_0016c134;
              iVar5 = FUN_001419f8();
              if ((iVar5 == 0) || (lVar8 = FUN_0014bef8(DAT_001cfb4c,&local_5c,4,lVar1), lVar8 != 4)
                 ) {
LAB_0016c1d8:
                bVar3 = true;
              }
              else {
                uVar9 = FUN_001bdaf0(lVar1,lVar4 + 0xe45f3c);
                iVar5 = FUN_001428fc(uVar9,lVar1,&local_b4,4);
                if (iVar5 == 0) goto LAB_0016c1d8;
                bVar3 = true;
                if (local_b4 == local_5c) {
                  uVar12 = 1;
                  DAT_0022cf44 = 1;
                  FUN_001417c8("script_port_ready","HighlightLeonClone_v69_native_getters",0);
                  goto LAB_0016be98;
                }
              }
            }
            lVar8 = FUN_0014bef8(DAT_001cfb4c,&uStack_b0,4,lVar10);
            if (lVar8 != 4) {
LAB_0016c1e4:
                    /* WARNING: Subroutine does not return */
              abort();
            }
            uVar9 = FUN_001bdaf0(lVar10,lVar4 + 0xe45f2c);
            iVar5 = FUN_001428fc(uVar9,lVar10,&local_b4,4);
            if ((iVar5 == 0) || (local_b4 != uStack_b0)) {
LAB_0016c1d4:
                    /* WARNING: Subroutine does not return */
              abort();
            }
            if (bVar3) {
              lVar10 = FUN_0014bef8(DAT_001cfb4c,local_ac,4,lVar1);
              if (lVar10 != 4) goto LAB_0016c1e4;
              uVar9 = FUN_001bdaf0(lVar1,lVar4 + 0xe45f3c);
              iVar5 = FUN_001428fc(uVar9,lVar1,&local_b4,4);
              if ((iVar5 == 0) || (local_b4 != local_ac[0])) goto LAB_0016c1d4;
            }
          }
        }
      }
    }
  }
  uVar12 = 0;
LAB_0016be98:
  iVar5 = FUN_00173c58(0xbb5200,0x274,
                       "26b7d49685350c821950a694f2735baac26701992baf92a9160b72ed48d3c158",0xbb5200,
                       0xfc1c0fe8,FUN_001750a8);
  iVar6 = FUN_00173c58(0xa64818,0xd60,
                       "bed76fecd36c87ca211df8766a16a8ab4de9a1e379e4a7114809645086baa92f",0xa64818,
                       0xd104c3ff,FUN_00175164);
  uVar11 = (uint)(iVar5 != 0);
  if (iVar6 != 0) {
    uVar11 = iVar5 != 0 | 2;
  }
  iVar5 = FUN_00173c58(0x81b298,0x1484,
                       "1dd1fef35059b52de083648e1a7e1ffdd95d66a9acdd3dfb68058a4dca03b82f",0x81c6f4,
                       0xa94b4ff4,FUN_00175220);
  if (iVar5 != 0) {
    uVar11 = uVar11 | 4;
  }
  DAT_0022cf40 = uVar11;
  snprintf((char *)&uStack_b0,0x50,",\"clone\":%d,\"observer_mask\":%u",(ulong)uVar12,(ulong)uVar11)
  ;
  FUN_001417c8("script_port_hooks","v69_native_gameplay",&uStack_b0);
  if (*(long *)(lVar2 + 0x28) == local_58) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar12 != 0 && uVar11 == 7);
}

/* ===== FUN_0016c748 @ 0016c748 [libNexusEvasionRuntime69252.so] ===== */

void FUN_0016c748(void)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  long lVar4;
  undefined **ppuVar5;
  
  iVar2 = FUN_0014b1d0(0xd0ba10,0x25c,
                       "f2bb7422387baf6f531909504141f1cd3034f4e57bf30cb54253e180f454371a");
  if (iVar2 != 0) {
    DAT_0022d134 = FUN_00173c58(0x75e6a0,0xc,
                                "7ce713049f8d31080501c810638e08a9a353cef97c534803d3139266c5818af5",
                                0x75e6a4,0x393fa100,FUN_00179ad4);
  }
  DAT_001dff80 = FUN_00179b78;
  if (DAT_0022d134 != 0) {
    lVar4 = 0;
    ppuVar5 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar2 = strcmp("SlowMode",*ppuVar5);
      if (iVar2 == 0) {
        if (((-1 < (int)lVar4) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar4 * 4) != 0)) &&
           (DAT_0022d134 != 0)) {
          puVar3 = (undefined4 *)__errno();
          uVar1 = *puVar3;
          (*(code *)(DAT_001e0978 + 0x75e6a0))(1);
          *puVar3 = uVar1;
        }
        break;
      }
      lVar4 = lVar4 + 1;
      ppuVar5 = ppuVar5 + 3;
    } while (lVar4 != 0x37);
  }
  FUN_00179be0(0x922ffc,0x524,"ceba13cf13c0ed00e0da2e2452ec5f02dc998da79622bee05604aab64f2ed204",
               FUN_00179e14,&DAT_0022d138,1);
  FUN_00179be0(0xb3fcec,0xa8,"fed298499238ac622efdd657a7a50599617132f0907306189b30825f8b5dcd9f",
               FUN_00179ee8,&DAT_0022d140,2);
  iVar2 = FUN_0014b1d0(0x8683c0,0x332c,
                       "0bfac2bff9221607fe0eaba0d6406e19a4078c307d545165e170f0e4a67bdda7");
  if (iVar2 != 0) {
    iVar2 = FUN_00173c58(0x86a214,4,
                         "b95c3d0b050224c2030dfff0e05768712c3d549ebf5ba905623e9e037a76544b",0x86a214
                         ,0x39002117,FUN_00179f98);
    if (iVar2 != 0) {
      DAT_0022d148 = DAT_0022d148 | 1;
    }
    iVar2 = FUN_00173c58(0x86a270,4,
                         "b95c3d0b050224c2030dfff0e05768712c3d549ebf5ba905623e9e037a76544b",0x86a270
                         ,0x39002117,FUN_00179f98);
    if (iVar2 != 0) {
      DAT_0022d148 = DAT_0022d148 | 2;
    }
    iVar2 = FUN_00173c58(0x86a284,4,
                         "b95c3d0b050224c2030dfff0e05768712c3d549ebf5ba905623e9e037a76544b",0x86a284
                         ,0x39002117,FUN_00179f98);
    if (iVar2 != 0) {
      DAT_0022d148 = DAT_0022d148 | 4;
    }
  }
  return;
}

/* ===== FUN_0016e6d0 @ 0016e6d0 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0016e6d0(ulong *param_1)

{
  undefined4 uVar1;
  char cVar2;
  bool bVar3;
  long lVar4;
  char cVar5;
  uint uVar6;
  int iVar7;
  undefined4 uVar8;
  int iVar9;
  undefined4 *puVar10;
  undefined8 uVar11;
  undefined4 uVar12;
  ulong uVar13;
  long lVar14;
  undefined **ppuVar15;
  long local_3e0;
  ulong local_3d8;
  int local_3d0;
  undefined4 local_3cc;
  undefined8 local_3c8;
  long local_3c0;
  ulong uStack_3b8;
  ulong local_3b0;
  long lStack_3a8;
  undefined *local_3a0;
  undefined *puStack_398;
  undefined *local_390;
  undefined1 *puStack_388;
  undefined8 *local_380;
  undefined *puStack_378;
  undefined8 local_370;
  ulong uStack_368;
  undefined8 uStack_360;
  undefined8 uStack_358;
  undefined8 local_350;
  undefined8 uStack_348;
  ulong local_340;
  ulong local_338;
  undefined8 local_330;
  undefined8 uStack_328;
  undefined8 uStack_320;
  undefined8 uStack_318;
  undefined8 local_310;
  undefined1 auStack_300 [16];
  int local_2f0;
  int local_288;
  undefined4 local_284;
  timespec local_280 [32];
  long local_78;
  
  lVar4 = tpidr_el0;
  local_78 = *(long *)(lVar4 + 0x28);
  puVar10 = (undefined4 *)__errno();
  iVar9 = DAT_00209cd8;
  uVar1 = *puVar10;
  if ((param_1 == (ulong *)0x0) || ((int)DAT_00214938 != 1)) {
LAB_0016e7a4:
    uVar8 = 0;
    goto LAB_0016e7a8;
  }
  if (DAT_00209cd8 == 0) {
    uVar8 = 0;
    goto LAB_0016e7a8;
  }
  iVar7 = gettid();
  uVar8 = 0;
  if (((iVar9 != iVar7) || (uVar8 = 0, DAT_0020f6cc == 0)) || (DAT_00220798 != 0))
  goto LAB_0016e7a8;
  do {
    uVar6 = _DAT_001cfe94;
    cVar5 = DAT_001cfe94;
    cVar2 = '\x01';
    bVar3 = (bool)ExclusiveMonitorPass(0x1cfe94,0x10);
    if (bVar3) {
      _DAT_001cfe94 = CONCAT31(DAT_001cfe94_1,1);
      cVar2 = ExclusiveMonitorsStatus();
    }
  } while (cVar2 != '\0');
  if ((((uVar6 & 0xff) != 0) && (DAT_0020d158 != 3)) && (DAT_0020d158 != 1)) goto LAB_0016e7a4;
  iVar7 = clock_gettime(1,local_280);
  iVar9 = DAT_00209cd8;
  if (iVar7 == 0) {
    local_3e0 = CONCAT44(local_280[0].tv_sec._4_4_,(int)local_280[0].tv_sec) * 1000 +
                CONCAT44(local_280[0].tv_nsec._4_4_,(int)local_280[0].tv_nsec) / 1000000;
  }
  else {
    local_3e0 = 0;
  }
  if ((((DAT_0020d158 == 0) && (DAT_0020f6cc != 0)) &&
      ((DAT_00209cd8 != 0 &&
       (((((iVar7 = gettid(), iVar9 == iVar7 &&
           (uVar11 = FUN_00150bf0(DAT_0020f670,DAT_0020f680), (int)uVar11 != 0)) &&
          (uVar11 = FUN_001428fc(uVar11,DAT_0020f708 + 0x58,&local_3d8,8), (int)uVar11 != 0)) &&
         ((0x11fff < local_3d8 + 0x2000 && ((local_3d8 & 7) == 0)))) && (*param_1 == local_3d8))))))
     && (((uVar13 = param_1[1], 0x11fff < uVar13 + 0x2000 && ((uVar13 & 7) == 0)) &&
         ((iVar9 = FUN_001428fc(uVar11,uVar13 + 8,local_280,0xc), iVar9 != 0 &&
          (((((int)local_280[0].tv_sec == 2 && (-60000 < local_280[0].tv_sec._4_4_)) &&
            (local_280[0].tv_sec._4_4_ < 60000)) &&
           ((-60000 < (int)local_280[0].tv_nsec && ((int)local_280[0].tv_nsec < 60000)))))))))) {
    DAT_002285e0 = DAT_0020f6f8;
    DAT_002285e8 = DAT_0020f690;
    DAT_002285fc = (int)local_280[0].tv_nsec;
    DAT_002285f8 = local_280[0].tv_sec._4_4_;
    DAT_00228880 = 1;
    DAT_002285f0 = local_3e0;
  }
  if ((uVar6 & 0xff) == 0) {
    lVar14 = 0;
    ppuVar15 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar9 = strcmp("HideSuperAim",*ppuVar15);
      if (iVar9 == 0) {
        if ((((((-1 < (int)lVar14) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar14 * 4) != 0))
              && ((uVar11 = FUN_001550fc(), (int)uVar11 != 0 &&
                  (((DAT_0020d158 == 0 && (DAT_00228908 != 0)) &&
                   (uVar11 = FUN_001428fc(uVar11,DAT_00228908,&local_284,4), (int)uVar11 != 0))))))
             && ((iVar9 = FUN_001428fc(uVar11,DAT_00228910,local_280,0x208), iVar9 != 0 &&
                 (uVar11 = FUN_00199b4c(&DAT_00228908,local_284,local_280,0x208), (int)uVar11 != 0))
                )) && (uVar11 = FUN_001428fc(uVar11,DAT_0020f708 + 0x58,&local_3d8,8),
                      (int)uVar11 != 0)) &&
           (((0x11fff < local_3d8 + 0x2000 && ((local_3d8 & 7) == 0)) &&
            ((local_3d8 == *param_1 &&
             ((iVar9 = FUN_001428fc(uVar11,param_1[1] + 8,&local_288,4), iVar9 != 0 &&
              (local_288 == 5)))))))) {
          (*(code *)(DAT_001e0978 + 0x11a2830))(param_1[1]);
          uVar12 = 1;
          goto LAB_0016ec90;
        }
        break;
      }
      lVar14 = lVar14 + 1;
      ppuVar15 = ppuVar15 + 3;
    } while (lVar14 != 0x37);
    iVar9 = FUN_0017f590(param_1);
    if (iVar9 == 0) goto LAB_0016e968;
    uVar12 = 1;
  }
  else {
LAB_0016e968:
    local_310 = 0;
    uStack_348 = 0;
    local_350 = 0;
    local_338 = 0;
    local_340 = 0;
    uStack_328 = 0;
    local_330 = 0;
    uStack_318 = 0;
    uStack_320 = 0;
    uStack_368 = 0;
    local_370 = 0;
    uStack_358 = 0;
    uStack_360 = 0;
    iVar9 = FUN_00164c78(auStack_300);
    if (((iVar9 != 0) && (local_2f0 != 0)) && (_DAT_00220788 != -1)) {
      _DAT_00220788 = _DAT_00220788 + 1;
      DAT_00220790 = *param_1;
      DAT_00220798 = param_1[1];
      iVar9 = clock_gettime(1,local_280);
      if (iVar9 == 0) {
        lVar14 = CONCAT44(local_280[0].tv_sec._4_4_,(int)local_280[0].tv_sec) * 1000 +
                 CONCAT44(local_280[0].tv_nsec._4_4_,(int)local_280[0].tv_nsec) / 1000000;
      }
      else {
        lVar14 = 0;
      }
      local_3d8 = DAT_0010e7a0;
      local_3d0 = DAT_00209cd8;
      DAT_002207a0 = lVar14;
      local_3cc = gettid();
      local_3c0 = _DAT_00220788;
      uStack_3b8 = DAT_00220790;
      local_3b0 = DAT_00220798;
      local_3a0 = &DAT_0021ca20;
      puStack_398 = &DAT_0021e828;
      puStack_388 = auStack_300;
      local_390 = &DAT_0021ca50;
      local_3c8 = DAT_0010e608;
      local_380 = &DAT_00214e50;
      puStack_378 = &DAT_001cfbc8;
      lStack_3a8 = lVar14;
      iVar9 = FUN_001a05ac(DAT_0021e8d0,&local_3d8,&local_370);
      if (iVar9 != 0) {
        DAT_002fd638 = DAT_002fd638 + 1;
        uRam000000000021e870 = uStack_368;
        _DAT_0021e868 = local_370;
        uRam000000000021e880 = uStack_358;
        DAT_0021e878 = uStack_360;
        uRam000000000021e8b0 = uStack_328;
        _DAT_0021e8a8 = local_330;
        uRam000000000021e8c0 = uStack_318;
        DAT_0021e8b8 = uStack_320;
        DAT_0021e8c8 = local_310;
        uRam000000000021e890 = uStack_348;
        _DAT_0021e888 = local_350;
        uRam000000000021e8a0 = local_338;
        _DAT_0021e898 = local_340;
      }
      if ((DAT_002fd640 == 0) || (999 < (ulong)(DAT_002207a0 - DAT_002fd640))) {
        snprintf((char *)local_280,0x100,
                 ",\"reason\":%u,\"gid\":%u,\"writes\":%llu,\"readback\":%u,\"extra_inputs\":0",
                 uStack_368 & 0xffffffff,local_340 & 0xffffffff,DAT_002fd638,local_338 & 0xffffffff)
        ;
        FUN_001417c8("xray_input","existing_input_target",local_280);
        DAT_002fd640 = DAT_002207a0;
      }
      DAT_00220798 = 0;
      DAT_00220790 = 0;
    }
    uVar12 = 0;
    uVar8 = 0;
    if (cVar5 != '\0') goto LAB_0016e7a8;
  }
LAB_0016ec90:
  _DAT_001cfe94 = 0;
  uVar8 = uVar12;
LAB_0016e7a8:
  *puVar10 = uVar1;
  if (*(long *)(lVar4 + 0x28) != local_78) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar8);
  }
  return;
}

/* ===== FUN_0016fa60 @ 0016fa60 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0016fa60(long param_1,undefined8 param_2)

{
  undefined4 uVar1;
  byte bVar2;
  char cVar3;
  bool bVar4;
  long lVar5;
  long lVar6;
  int iVar7;
  undefined4 *puVar8;
  undefined8 uVar9;
  uint uVar10;
  ulong uVar11;
  ulong uVar12;
  ulong uVar13;
  size_t sVar14;
  long lVar15;
  undefined **ppuVar16;
  ulong uVar17;
  ulong uVar18;
  ulong local_c00;
  ulong local_bf8;
  char local_bf0 [1024];
  undefined1 auStack_7f0 [384];
  timespec local_670 [96];
  long local_70;
  
  lVar5 = tpidr_el0;
  local_70 = *(long *)(lVar5 + 0x28);
  puVar8 = (undefined4 *)__errno();
  (*DAT_00229020)(param_1,param_2);
  uVar12 = DAT_001e01d0;
  lVar6 = DAT_001e01c8;
  uVar1 = *puVar8;
  iVar7 = clock_gettime(1,local_670);
  if (iVar7 == 0) {
    uVar11 = local_670[0].tv_sec * 1000 + (ulong)local_670[0].tv_nsec / 1000000;
  }
  else {
    uVar11 = 0;
  }
  if ((((DAT_001e096c == '\x01') && (lVar6 != 0)) && (uVar12 != 0)) &&
     ((uVar12 <= uVar11 && (uVar11 - uVar12 < 0x5dd)))) {
    lVar15 = 0;
    ppuVar16 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar7 = strcmp("BattleTextChat",*ppuVar16);
      if (iVar7 == 0) {
        if (((((-1 < (int)lVar15) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar15 * 4) == 1))
             && ((uVar9 = FUN_001428fc(0,param_1 + 0x18,&local_bf8,8), (int)uVar9 != 0 &&
                 (((0x11fff < local_bf8 + 0x2000 && ((local_bf8 & 7) == 0)) &&
                  (iVar7 = FUN_001428fc(uVar9,param_1 + 0x30,&local_c00,8), iVar7 != 0)))))) &&
            ((0x11fff < local_c00 + 0x2000 && ((local_c00 & 7) == 0)))) &&
           ((iVar7 = FUN_0014a6ac(local_bf8,auStack_7f0,0x180), iVar7 != 0 &&
            ((iVar7 = FUN_0014a6ac(local_c00,local_bf0,0x400), iVar7 != 0 && (local_bf0[0] != '\0'))
            )))) goto LAB_0016fc28;
        break;
      }
      lVar15 = lVar15 + 1;
      ppuVar16 = ppuVar16 + 3;
    } while (lVar15 != 0x37);
  }
  goto LAB_0016ffa0;
LAB_0016fc28:
  do {
    uVar10 = _DAT_001e01d8;
    cVar3 = '\x01';
    bVar4 = (bool)ExclusiveMonitorPass(0x1e01d8,0x10);
    if (bVar4) {
      _DAT_001e01d8 = CONCAT31(DAT_001e01d8_1,1);
      cVar3 = ExclusiveMonitorsStatus();
    }
  } while (cVar3 != '\0');
  if ((uVar10 & 1) == 0) {
    if (DAT_001e01e0 != lVar6) {
      memset(&DAT_001e01e8,0,0x784);
      DAT_001e01e0 = lVar6;
    }
    iVar7 = snprintf((char *)local_670,0x600,"[%s]: %s",auStack_7f0,local_bf0);
    if (0xfffffa00 < iVar7 - 0x600U) {
      uVar17 = 0;
      uVar10 = 0;
      uVar18 = (ulong)iVar7;
      uVar12 = 0;
      uVar11 = 0;
      do {
        while( true ) {
          bVar2 = *(byte *)((long)&local_670[0].tv_sec + uVar11);
          if ((char)bVar2 < '\0') {
            if ((bVar2 & 0xe0) == 0xc0) {
              uVar13 = 2;
            }
            else if ((bVar2 & 0xf0) == 0xe0) {
              uVar13 = 3;
            }
            else {
              uVar13 = 4;
              if ((bVar2 & 0xf8) != 0xf0) {
                uVar13 = 1;
              }
            }
          }
          else {
            uVar13 = 1;
          }
          if (uVar18 < uVar13 + uVar11) {
            uVar13 = 1;
          }
          if ((uVar13 < 2) || ((*(byte *)((long)&local_670[0].tv_sec + uVar11 + 1) & 0xc0) != 0x80))
          break;
          if (uVar13 == 2) goto joined_r0x0016fe4c;
          if ((*(byte *)((long)&local_670[0].tv_sec + uVar11 + 2) & 0xc0) != 0x80) break;
          if (uVar13 == 3) goto joined_r0x0016fe4c;
          if ((*(byte *)((long)&local_670[0].tv_sec + uVar11 + 3) & 0xc0) != 0x80) break;
          if (uVar13 == 4) goto joined_r0x0016fe4c;
          if ((*(byte *)((long)&local_670[0].tv_sec + uVar11 + 4) & 0xc0) != 0x80) break;
          if ((*(byte *)((long)&local_670[0].tv_sec + uVar11 + 5) & 0xc0) != 0x80) {
            uVar13 = 1;
            goto joined_r0x0016fe4c;
          }
          if ((*(byte *)((long)&local_670[0].tv_sec + uVar11 + 6) & 0xc0) != 0x80) {
            uVar13 = 1;
          }
          if (bVar2 != 0xd) goto LAB_0016fe50;
LAB_0016fdfc:
          sVar14 = uVar11 - uVar17;
          if (0xffffffffffffff40 < sVar14 - 0xc0) {
            uVar12 = (ulong)DAT_001e0968;
            if (DAT_001e0968 == 10) {
              memmove(&DAT_001e01e8,&DAT_001e02a8,0x6c0);
              uVar12 = 9;
              DAT_001e0968 = 9;
            }
            memcpy(&DAT_001e01e8 + uVar12 * 0xc0,(void *)((long)&local_670[0].tv_sec + uVar17),
                   sVar14);
            (&DAT_001e01e8)[sVar14 + (ulong)DAT_001e0968 * 0xc0] = 0;
            DAT_001e0968 = DAT_001e0968 + 1;
          }
LAB_0016fcdc:
          uVar17 = uVar11 + 1;
LAB_0016fce4:
          uVar10 = 0;
          uVar12 = uVar17;
          uVar11 = uVar17;
          uVar13 = uVar17;
          if (uVar18 <= uVar17) goto LAB_0016ff24;
        }
        uVar13 = 1;
joined_r0x0016fe4c:
        if (bVar2 == 0xd) goto LAB_0016fdfc;
LAB_0016fe50:
        if (((bVar2 == 10) || (0x21 < uVar10)) || (uVar13 = uVar13 + uVar11, 0xbf < uVar13 - uVar17)
           ) {
          if (uVar12 <= uVar17) {
            uVar12 = uVar11;
          }
          if (bVar2 != 10) {
            uVar11 = uVar12;
          }
          sVar14 = uVar11 - uVar17;
          if (0xffffffffffffff40 < sVar14 - 0xc0) {
            if (DAT_001e0968 == 10) {
              memmove(&DAT_001e01e8,&DAT_001e02a8,0x6c0);
              DAT_001e0968 = 9;
            }
            memcpy(&DAT_001e01e8 + (ulong)DAT_001e0968 * 0xc0,
                   (void *)((long)&local_670[0].tv_sec + uVar17),sVar14);
            (&DAT_001e01e8)[sVar14 + (ulong)DAT_001e0968 * 0xc0] = 0;
            DAT_001e0968 = DAT_001e0968 + 1;
          }
          cVar3 = *(char *)((long)&local_670[0].tv_sec + uVar11);
          if ((cVar3 == ' ') || (uVar17 = uVar11, cVar3 == '\n')) goto LAB_0016fcdc;
          goto LAB_0016fce4;
        }
        uVar10 = uVar10 + 1;
        if (bVar2 != 0x20) {
          uVar11 = uVar12;
        }
        uVar12 = uVar11;
        uVar11 = uVar13;
      } while (uVar13 < uVar18);
LAB_0016ff24:
      sVar14 = uVar13 - uVar17;
      if ((uVar17 <= uVar13 && sVar14 != 0) && (0xffffffffffffff40 < sVar14 - 0xc0)) {
        if (DAT_001e0968 == 10) {
          memmove(&DAT_001e01e8,&DAT_001e02a8,0x6c0);
          DAT_001e0968 = 9;
        }
        memcpy(&DAT_001e01e8 + (ulong)DAT_001e0968 * 0xc0,
               (void *)((long)&local_670[0].tv_sec + uVar17),sVar14);
        (&DAT_001e01e8)[sVar14 + (ulong)DAT_001e0968 * 0xc0] = 0;
        DAT_001e0968 = DAT_001e0968 + 1;
      }
    }
    _DAT_001e01d8 = 0;
  }
LAB_0016ffa0:
  *puVar8 = uVar1;
  if (*(long *)(lVar5 + 0x28) != local_70) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

/* ===== FUN_0017019c @ 0017019c [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Type propagation algorithm not settling */

void FUN_0017019c(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                 undefined4 param_5,undefined4 param_6,long param_7,long param_8,undefined8 param_9,
                 undefined4 param_10,undefined4 param_11,undefined4 param_12)

{
  undefined4 uVar1;
  undefined4 uVar2;
  long lVar3;
  long lVar4;
  int iVar5;
  int iVar6;
  undefined4 *puVar7;
  long *plVar8;
  undefined8 uVar9;
  undefined8 *puVar10;
  ulong uVar11;
  long lVar12;
  ulong uVar13;
  uint uVar14;
  undefined **ppuVar15;
  float fVar16;
  float fVar17;
  float fVar18;
  char local_57c [4];
  long local_578 [4];
  int local_554;
  undefined8 local_550;
  undefined4 uStack_548;
  undefined3 uStack_544;
  undefined8 local_540;
  float local_538;
  undefined8 local_534;
  float local_52c;
  undefined8 local_510;
  float local_508;
  undefined8 local_504;
  float local_4fc [17];
  undefined8 local_4b8;
  undefined4 local_4b0;
  int iStack_4ac;
  undefined1 auStack_4a8 [1032];
  long local_a0;
  
  lVar3 = tpidr_el0;
  local_a0 = *(long *)(lVar3 + 0x28);
  puVar7 = (undefined4 *)__errno();
  uVar1 = *puVar7;
  if (DAT_002290a8 == '\x01') {
    lVar12 = 0;
    ppuVar15 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar5 = strcmp("ExtendedTrajectory",*ppuVar15);
      if (iVar5 == 0) {
        if ((-1 < (int)lVar12) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar12 * 4) == 1)) {
          iVar5 = FUN_001550fc();
          uVar14 = 0;
          if ((iVar5 != 0) && (DAT_0020f670 == param_7)) {
            uVar14 = (uint)(DAT_0020f680 == param_8);
          }
          goto LAB_001702ac;
        }
        break;
      }
      lVar12 = lVar12 + 1;
      ppuVar15 = ppuVar15 + 3;
    } while (lVar12 != 0x37);
  }
  uVar14 = 0;
LAB_001702ac:
  plVar8 = (long *)FUN_001bd828(&DAT_001cfd58);
  *(uint *)(plVar8 + 3) = uVar14;
  *(undefined4 *)((long)plVar8 + 0x1c) = 0;
  *plVar8 = param_7;
  plVar8[2] = DAT_0020f6f8;
  *puVar7 = uVar1;
  (*DAT_002290a0)(param_1,param_2,param_3,param_4,param_5,param_6,param_7,param_8,param_9,param_10,
                  param_11,param_12);
  uVar1 = *puVar7;
  *(undefined4 *)(plVar8 + 3) = 0;
  if ((((uVar14 != 0) && (3 < *(uint *)((long)plVar8 + 0x1c))) &&
      (iVar5 = FUN_001550fc(), iVar5 != 0)) && (plVar8[2] == DAT_0020f6f8)) {
    lVar12 = 0;
    ppuVar15 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar5 = strcmp("ExtendedTrajectory",*ppuVar15);
      lVar4 = DAT_0020f728;
      if (iVar5 == 0) {
        if ((-1 < (int)lVar12) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar12 * 4) != 0)) {
          local_550 = 0;
          local_554 = 0;
          local_578[3] = 0;
          uVar9 = FUN_001428fc(0,plVar8[1] + 0x20,(long)&local_550 + 4,4);
          if (((((int)uVar9 != 0) &&
               ((uVar9 = FUN_001428fc(uVar9,lVar4 + 0xc4,&local_550,4), (int)uVar9 != 0 &&
                (iVar5 = FUN_001428fc(uVar9,lVar4 + 200,&local_554,4), iVar5 != 0)))) &&
              (0 < (int)local_550)) &&
             ((((0 < local_554 && ((int)local_550 < 0x65)) && (local_554 < 0x65)) &&
              (iVar5 = FUN_0013a78c(lVar4 + 0x20,local_578 + 3), iVar5 != 0)))) {
            fVar16 = (float)FUN_00170760(local_550._4_4_);
            fVar17 = 9600.0;
            if (fVar16 != 4800.0) {
              fVar17 = fVar16;
            }
            if (0.0 < fVar17) {
              memset(auStack_4a8,0,0x408);
              local_4b8 = local_578[3];
              local_4b0 = (int)local_550;
              iStack_4ac = local_554;
              iVar5 = FUN_00170788(fVar17,plVar8 + 4,*(undefined4 *)((long)plVar8 + 0x1c),&local_4b8
                                   ,&local_510);
              if (iVar5 != 0) {
                local_578[1] = 0;
                local_578[2] = 0;
                iVar6 = FUN_0013a78c(param_7 + 0xb18,local_578 + 2);
                if (iVar6 != 0) {
                  iVar6 = FUN_0013a78c(param_7 + 0xb20,local_578 + 1);
                  if (iVar6 != 0) {
                    uVar13 = (ulong)(iVar5 - 1U);
                    local_540 = local_510;
                    local_538 = local_508;
                    if (iVar5 - 1U == 0) {
                      uVar13 = 0;
                      plVar8 = local_578 + 2;
                      local_534 = local_504;
                      local_52c = local_4fc[0];
                    }
                    else {
                      puVar10 = &local_504;
                      lVar12 = 0xc;
                      uVar11 = uVar13;
                      do {
                        uVar9 = *puVar10;
                        uVar2 = *(undefined4 *)(puVar10 + 1);
                        puVar10 = (undefined8 *)((long)puVar10 + 0x1c);
                        uVar11 = uVar11 - 1;
                        *(undefined8 *)((long)&local_540 + lVar12) = uVar9;
                        *(undefined4 *)((long)&local_538 + lVar12) = uVar2;
                        lVar12 = lVar12 + 0xc;
                      } while (uVar11 != 0);
                      FUN_00170ef8(local_578[2],&local_540,iVar5);
                      local_540 = *(undefined8 *)((long)&local_510 + uVar13 * 0x1c);
                      local_538 = (&local_508)[uVar13 * 7];
                      local_534 = *(undefined8 *)(local_4fc + uVar13 * 7 + -2);
                      local_52c = local_4fc[uVar13 * 7];
                      plVar8 = local_578 + 1;
                    }
                    FUN_00170ef8(*plVar8,&local_540,2);
                    fVar16 = local_4fc[uVar13 * 7 + -1];
                    fVar17 = local_4fc[uVar13 * 7 + -2];
                    if (((fVar16 <= 950.0) ||
                        (fVar18 = (float)NEON_fmadd((float)local_554,0x43960000,0xc46d8000),
                        fVar16 < fVar18 == (NAN(fVar16) || NAN(fVar18)))) &&
                       (fVar17 < 2200.0 == NAN(fVar17))) {
                      fVar16 = (float)NEON_fmadd((float)(int)local_550,0x43960000,0xc5098000);
                      if (fVar17 <= fVar16) {
                        local_578[0] = 0;
                        uStack_544 = 0xff00;
                        uStack_548 = 0xff00ff00;
                        local_57c[0] = '\0';
                        uVar9 = FUN_0013a78c(param_7 + 0xb10,local_578);
                        if ((((int)uVar9 != 0) &&
                            (iVar5 = FUN_001428fc(uVar9,local_578[0] + 8,local_57c,1), iVar5 != 0))
                           && (local_57c[0] != '\0')) {
                          FUN_0014bef8(DAT_001cfb4c,&uStack_548,7,local_578[0] + 9);
                        }
                        local_578[0] = 0;
                        local_57c[0] = '\0';
                        uVar9 = FUN_0013a78c(param_7 + 0xb18,local_578);
                        if ((((int)uVar9 != 0) &&
                            (iVar5 = FUN_001428fc(uVar9,local_578[0] + 8,local_57c,1), iVar5 != 0))
                           && (local_57c[0] != '\0')) {
                          FUN_0014bef8(DAT_001cfb4c,&uStack_548,7,local_578[0] + 9);
                        }
                        local_578[0] = 0;
                        local_57c[0] = '\0';
                        uVar9 = FUN_0013a78c(param_7 + 0xb20,local_578);
                        if ((((int)uVar9 != 0) &&
                            (iVar5 = FUN_001428fc(uVar9,local_578[0] + 8,local_57c,1), iVar5 != 0))
                           && (local_57c[0] != '\0')) {
                          FUN_0014bef8(DAT_001cfb4c,&uStack_548,7,local_578[0] + 9);
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
        break;
      }
      lVar12 = lVar12 + 1;
      ppuVar15 = ppuVar15 + 3;
    } while (lVar12 != 0x37);
  }
  *puVar7 = uVar1;
  if (*(long *)(lVar3 + 0x28) == local_a0) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_00170f9c @ 00170f9c [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00170f9c(long param_1,undefined8 param_2)

{
  char *pcVar1;
  char *pcVar2;
  char *pcVar3;
  char *pcVar4;
  undefined4 uVar5;
  char cVar6;
  bool bVar7;
  bool bVar8;
  long lVar9;
  char *pcVar10;
  bool bVar11;
  int iVar12;
  int iVar13;
  int iVar14;
  undefined4 *puVar15;
  void *pvVar16;
  undefined8 uVar17;
  size_t sVar18;
  uint uVar19;
  uint uVar20;
  uint uVar21;
  ulong uVar22;
  ulong *puVar23;
  long lVar24;
  long lVar25;
  undefined **ppuVar26;
  char *local_520;
  int local_514;
  undefined8 local_510;
  ulong local_508;
  ulong local_500;
  ulong local_4f8;
  ulong local_4f0;
  ulong local_4e8;
  int local_4dc;
  ulong local_4d8;
  int local_4cc;
  ulong local_4c8;
  undefined8 local_4c0;
  undefined8 uStack_4b8;
  undefined8 uStack_4b0;
  undefined8 uStack_4a8;
  undefined8 local_4a0;
  byte local_490;
  undefined7 uStack_48f;
  char local_20c [404];
  long local_78;
  
  lVar9 = tpidr_el0;
  local_78 = *(long *)(lVar9 + 0x28);
  puVar15 = (undefined4 *)__errno();
  (*DAT_002290e8)(param_1,param_2);
  uVar5 = *puVar15;
  local_4c8 = 0;
  local_4c0 = 0;
  local_4d8 = 0;
  local_490 = 0;
  if ((DAT_0020af9c >> 3 & 1) == 0) {
LAB_00171018:
    uVar19 = DAT_0020afa0._4_4_ & 1;
  }
  else {
    uVar19 = DAT_0020afa0._4_4_ & 0xff;
    if ((((((uint)DAT_0020afa0 ^ 0xffffffff) & 3) == 0) && (uVar19 != 0)) && (DAT_0020af50 != '\0'))
    {
      if ((int)DAT_001dfff0 != 0) {
        do {
          uVar19 = _DAT_001e12e8;
          cVar6 = '\x01';
          bVar7 = (bool)ExclusiveMonitorPass(0x1e12e8,0x10);
          if (bVar7) {
            _DAT_001e12e8 = CONCAT31(DAT_001e12e8_1,1);
            cVar6 = ExclusiveMonitorsStatus();
          }
        } while (cVar6 != '\0');
        if ((uVar19 & 1) == 0) {
          pvVar16 = memcpy(&local_490,&DAT_001e1311,0x191);
          _DAT_001e12e8 = 0;
          if ((local_490 != 0) &&
             (uVar17 = FUN_001428fc(pvVar16,param_1 + 0x40,&local_4c0,8), (int)uVar17 != 0)) {
            do {
              uVar19 = _DAT_001e12e8;
              cVar6 = '\x01';
              bVar7 = (bool)ExclusiveMonitorPass(0x1e12e8,0x10);
              if (bVar7) {
                _DAT_001e12e8 = CONCAT31(DAT_001e12e8_1,1);
                cVar6 = ExclusiveMonitorsStatus();
              }
            } while (cVar6 != '\0');
            if (((((uVar19 & 1) == 0) && (_DAT_001e12e8 = 0, DAT_002290f0 != 0)) &&
                ((local_4c0 == DAT_002290f0 &&
                 (((((uVar17 = FUN_001428fc(uVar17,param_1 + 0x270,&local_4c8,8), (int)uVar17 != 0
                     && (0x11fff < local_4c8 + 0x2000)) && ((local_4c8 & 7) == 0)) &&
                   ((iVar12 = FUN_001428fc(uVar17,local_4c8,&local_4d8,8), iVar12 != 0 &&
                    (0x11fff < local_4d8 + 0x2000)))) && ((local_4d8 & 7) == 0)))))) &&
               (iVar12 = FUN_0014a6ac(local_4d8,local_20c,0x191), uVar22 = local_4d8, iVar12 != 0))
            {
              (*(code *)(DAT_001e0978 + 0x66ad48))(local_4d8);
              (*(code *)(DAT_001e0978 + 0x66ae58))(uVar22,&local_490);
            }
          }
        }
      }
      goto LAB_00171018;
    }
  }
  if ((uVar19 == 0) || ((int)DAT_001dfff0 == 0)) goto LAB_001712f0;
  lVar24 = 0;
  ppuVar26 = &PTR_s_DisablePinAnimation_001c2838;
  do {
    iVar12 = strcmp("ShowCharactersInNames",*ppuVar26);
    if (iVar12 == 0) {
      if (-1 < (int)lVar24) {
        iVar12 = (int)*(undefined8 *)((long)&DAT_001dfea0 + lVar24 * 4);
        goto LAB_001711e4;
      }
      break;
    }
    lVar24 = lVar24 + 1;
    ppuVar26 = ppuVar26 + 3;
  } while (lVar24 != 0x37);
  iVar12 = 0;
LAB_001711e4:
  lVar24 = 0;
  ppuVar26 = &PTR_s_DisablePinAnimation_001c2838;
  do {
    iVar13 = strcmp("ShamePlayersWithThumbsdownPin",*ppuVar26);
    if (iVar13 == 0) {
      if (-1 < (int)lVar24) {
        uVar19 = (uint)*(undefined8 *)((long)&DAT_001dfea0 + lVar24 * 4);
        goto LAB_00171238;
      }
      break;
    }
    lVar24 = lVar24 + 1;
    ppuVar26 = ppuVar26 + 3;
  } while (lVar24 != 0x37);
  uVar19 = 0;
LAB_00171238:
  lVar24 = 0;
  ppuVar26 = &PTR_s_DisablePinAnimation_001c2838;
  do {
    iVar13 = strcmp("ShowFriendsInBattle",*ppuVar26);
    if (iVar13 == 0) {
      if (-1 < (int)lVar24) {
        iVar13 = (int)*(undefined8 *)((long)&DAT_001dfea0 + lVar24 * 4);
        goto LAB_0017128c;
      }
      break;
    }
    lVar24 = lVar24 + 1;
    ppuVar26 = ppuVar26 + 3;
  } while (lVar24 != 0x37);
  iVar13 = 0;
LAB_0017128c:
  lVar24 = 0;
  ppuVar26 = &PTR_s_DisablePinAnimation_001c2838;
  do {
    iVar14 = strcmp("ShowAllianceMembersInBattle",*ppuVar26);
    if (iVar14 == 0) {
      if (-1 < (int)lVar24) {
        uVar21 = (uint)*(undefined8 *)((long)&DAT_001dfea0 + lVar24 * 4);
        goto LAB_001712e0;
      }
      break;
    }
    lVar24 = lVar24 + 1;
    ppuVar26 = ppuVar26 + 3;
  } while (lVar24 != 0x37);
  uVar21 = 0;
LAB_001712e0:
  if ((((iVar12 == 0) && (uVar19 == 0)) && (iVar13 == 0)) && (uVar21 == 0)) goto LAB_001712f0;
  local_510 = 0;
  local_514 = -1;
  uVar17 = FUN_001428fc(iVar14,param_1 + 0x48,&local_514,4);
  if (((((((int)uVar17 == 0) || (local_514 < 0)) ||
        ((0x3f < local_514 ||
         ((uVar17 = FUN_001428fc(uVar17,param_1 + 0x270,&local_4e8,8), (int)uVar17 == 0 ||
          (local_4e8 + 0x2000 < 0x12000)))))) || ((local_4e8 & 7) != 0)) ||
      ((((uVar17 = FUN_001428fc(uVar17,local_4e8,&local_4f0,8), (int)uVar17 == 0 ||
         (local_4f0 + 0x2000 < 0x12000)) || ((local_4f0 & 7) != 0)) ||
       (((uVar17 = FUN_001428fc(uVar17,param_1 + 0x7c,(long)&local_510 + 4,4), (int)uVar17 == 0 ||
         (local_510._4_4_ < 1)) ||
        ((0x10 < local_510._4_4_ ||
         ((uVar17 = FUN_001428fc(uVar17,param_1 + 0x70,&local_4f8,8), (int)uVar17 == 0 ||
          (local_4f8 + 0x2000 < 0x12000)))))))))) ||
     ((((local_4f8 & 7) != 0 ||
       ((((uVar17 = FUN_001428fc(uVar17,local_4f8,&local_500,8), (int)uVar17 == 0 ||
          (local_500 + 0x2000 < 0x12000)) || ((local_500 & 7) != 0)) ||
        ((uVar17 = FUN_001428fc(uVar17,local_500,&local_508,8), (int)uVar17 == 0 ||
         (local_508 + 0x2000 < 0x12000)))))) ||
      (((((local_508 & 7) != 0 ||
         ((iVar14 = FUN_001428fc(uVar17,local_508 + 0x20,&local_510,4), iVar14 == 0 ||
          ((int)local_510 < 16000000)))) || (17999999 < (int)local_510)) ||
       ((uVar17 = FUN_0014a6ac(local_4f0,local_20c,0x191), (int)uVar17 == 0 ||
        (local_20c[0] == '\0')))))))) goto LAB_001712f0;
  if (iVar12 == 0) {
switchD_0017150c_caseD_f42480:
    bVar7 = false;
    local_520 = (char *)0x0;
  }
  else {
    iVar12 = (int)local_510 + -1000000;
    if (999999 < (int)local_510 + 0xfefc99c0U) {
      iVar12 = (int)local_510;
    }
    bVar7 = true;
    local_520 = "SHELLY";
                    /* WARNING: Could not find normalized switch variable to match jumptable */
    switch(iVar12) {
    case 16000000:
      break;
    case 0xf42440:
      bVar7 = true;
      local_520 = "GRAY";
      break;
    default:
      goto switchD_0017150c_caseD_f42480;
    }
  }
  if (uVar19 != 0) {
    local_4cc = 0;
    uVar17 = FUN_001428fc(uVar17,local_500 + 0x10,&local_490,8);
    uVar19 = 0;
    if ((((int)uVar17 != 0) && (0x11fff < CONCAT71(uStack_48f,local_490) + 0x2000U)) &&
       ((local_490 & 7) == 0)) {
      uVar17 = FUN_001428fc(uVar17,CONCAT71(uStack_48f,local_490),&local_4c0,8);
      uVar19 = 0;
      if ((((int)uVar17 != 0) && (0x11fff < local_4c0 + 0x2000)) && ((local_4c0 & 7) == 0)) {
        uVar17 = FUN_001428fc(uVar17,local_4c0 + 0xc,&local_4cc,4);
        uVar19 = 0;
        if ((((int)uVar17 != 0) && (0 < local_4cc)) && (local_4cc < 0x41)) {
          uVar17 = FUN_001428fc(uVar17,local_4c0,&local_4c8,8);
          uVar19 = 0;
          if (((((int)uVar17 != 0) && (local_4c8 - 0x10000 < 0xfffffffffffee000)) &&
              ((local_4c8 & 7) == 0)) && (0 < local_4cc)) {
            lVar25 = 0;
            lVar24 = 0;
            do {
              local_4dc = 0;
              uVar17 = FUN_001428fc(uVar17,lVar25 + local_4c8,&local_4d8,8);
              if (((((int)uVar17 != 0) && (0x11fff < local_4d8 + 0x2000)) &&
                  (((local_4d8 & 7) == 0 &&
                   ((uVar17 = FUN_001428fc(uVar17,local_4d8 + 0x20,&local_4dc,4), (int)uVar17 != 0
                    && (51999999 < local_4dc)))))) && (local_4dc < 53000000)) {
                if (local_4dc < 0x3197a1a) {
                  if (local_4dc != 0x31975af) {
                    iVar12 = 0x3197723;
LAB_00172020:
                    if (local_4dc != iVar12) goto LAB_00172028;
                  }
                }
                else if (local_4dc != 0x3197b58) {
                  iVar12 = 0x3197a1a;
                  goto LAB_00172020;
                }
                uVar19 = 1;
                break;
              }
LAB_00172028:
              uVar19 = 0;
              lVar24 = lVar24 + 1;
              lVar25 = lVar25 + 8;
            } while (lVar24 < local_4cc);
          }
        }
      }
    }
  }
  local_4c8 = 0;
  iVar12 = FUN_001428fc(uVar17,param_1 + 0x40,&local_4c8,8);
  if (iVar12 != 0) {
    if (iVar13 == 0) {
      bVar11 = false;
    }
    else {
      uVar20 = 0;
      if ((local_4c8 != 0) && (DAT_0020af50 != '\0')) {
        do {
          uVar20 = _DAT_002290f8;
          cVar6 = '\x01';
          bVar11 = (bool)ExclusiveMonitorPass(0x2290f8,0x10);
          if (bVar11) {
            _DAT_002290f8 = CONCAT31(DAT_002290f8_1,1);
            cVar6 = ExclusiveMonitorsStatus();
          }
        } while (cVar6 != '\0');
        if ((uVar20 & 1) == 0) {
          uVar22 = (ulong)DAT_0022b504;
          uVar20 = DAT_0022b504;
          if (DAT_0022b504 != 0) {
            puVar23 = &DAT_00229500;
            do {
              if (*puVar23 == local_4c8) {
                uVar20 = 1;
                goto LAB_0017218c;
              }
              uVar22 = uVar22 - 1;
              puVar23 = puVar23 + 1;
            } while (uVar22 != 0);
            uVar20 = 0;
          }
LAB_0017218c:
          _DAT_002290f8 = 0;
        }
        else {
          uVar20 = 0;
        }
      }
      bVar11 = uVar20 != 0;
    }
    if (uVar21 != 0) {
      uVar21 = 0;
      if ((local_4c8 != 0) && (DAT_0020af50 != '\0')) {
        do {
          uVar21 = _DAT_002290f8;
          cVar6 = '\x01';
          bVar8 = (bool)ExclusiveMonitorPass(0x2290f8,0x10);
          if (bVar8) {
            _DAT_002290f8 = CONCAT31(DAT_002290f8_1,1);
            cVar6 = ExclusiveMonitorsStatus();
          }
        } while (cVar6 != '\0');
        if ((uVar21 & 1) == 0) {
          uVar22 = (ulong)DAT_0022b500;
          uVar21 = DAT_0022b500;
          if (DAT_0022b500 != 0) {
            puVar23 = &DAT_00229100;
            do {
              if (*puVar23 == local_4c8) {
                uVar21 = 1;
                goto LAB_0017220c;
              }
              uVar22 = uVar22 - 1;
              puVar23 = puVar23 + 1;
            } while (uVar22 != 0);
            uVar21 = 0;
          }
LAB_0017220c:
          _DAT_002290f8 = 0;
        }
        else {
          uVar21 = 0;
        }
      }
      uVar21 = (uint)(uVar21 != 0);
    }
    if (((bVar7 || (uVar19 & 1) != 0) || bVar11) || (uVar21 != 0)) {
      local_4a0 = 0;
      uStack_4b8 = 0;
      local_4c0 = 0;
      uStack_4a8 = 0;
      uStack_4b0 = 0;
      if (uVar19 != 0) {
        sVar18 = strlen((char *)&local_4c0);
        *(undefined1 *)((long)&local_4c0 + sVar18 + 4) = 0;
        *(undefined4 *)((long)&local_4c0 + sVar18) = 0xa1a49ff0;
      }
      if (bVar11) {
        sVar18 = strlen((char *)&local_4c0);
        *(undefined1 *)((long)&local_4c0 + sVar18 + 4) = 0;
        *(undefined4 *)((long)&local_4c0 + sVar18) = 0xa5919ff0;
      }
      if (uVar21 != 0) {
        sVar18 = strlen((char *)&local_4c0);
        *(undefined8 *)((long)&local_4c0 + sVar18) = 0x8fb8efa19b9ff0;
      }
      pcVar1 = "";
      pcVar2 = pcVar1;
      pcVar10 = pcVar1;
      if ((char)local_4c0 != '\0') {
        pcVar2 = "] ";
        pcVar10 = "[";
      }
      bVar7 = !bVar7;
      pcVar3 = " (";
      if (bVar7) {
        pcVar3 = pcVar1;
      }
      pcVar4 = ")";
      if (bVar7) {
        pcVar4 = pcVar1;
      }
      if (bVar7) {
        local_520 = pcVar1;
      }
      iVar12 = snprintf((char *)&local_490,0x280,"%s%s%s%s%s%s%s",pcVar10,&local_4c0,pcVar2,
                        local_20c,pcVar3,local_520,pcVar4);
      if (0xfffffd80 < iVar12 - 0x280U) {
        (*(code *)(DAT_001e0978 + 0x66ad48))(local_4f0);
        (*(code *)(DAT_001e0978 + 0x66ae58))(local_4f0,&local_490);
      }
    }
  }
LAB_001712f0:
  *puVar15 = uVar5;
  if (*(long *)(lVar9 + 0x28) == local_78) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_001730bc @ 001730bc [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8
FUN_001730bc(undefined4 param_1,undefined8 param_2,int param_3,long param_4,long param_5,
            uint param_6)

{
  undefined4 uVar1;
  long lVar2;
  bool bVar3;
  int iVar4;
  int iVar5;
  undefined4 *puVar6;
  long lVar7;
  long lVar8;
  undefined8 uVar9;
  undefined8 uVar10;
  long lVar11;
  uint uVar12;
  ulong uVar13;
  undefined8 *puVar14;
  undefined **ppuVar15;
  long *plVar16;
  long lVar17;
  float fVar18;
  float fVar19;
  undefined1 auStack_1d0 [16];
  int local_1c0;
  uint local_1b0;
  uint local_1ac;
  int local_1a4;
  undefined4 local_190;
  undefined3 uStack_18c;
  undefined4 local_188;
  undefined3 uStack_184;
  undefined8 local_180;
  long local_80;
  
  lVar2 = tpidr_el0;
  local_80 = *(long *)(lVar2 + 0x28);
  puVar6 = (undefined4 *)__errno();
  uVar1 = *puVar6;
  if (((DAT_0022b5b0 == '\x01') && (iVar4 = FUN_001550fc(), iVar5 = DAT_00209cd8, iVar4 != 0)) &&
     (DAT_00209cd8 != 0)) {
    iVar4 = gettid();
    bVar3 = false;
    if (((iVar5 == iVar4) && (DAT_0020f680 == param_4)) && (DAT_0020f69c == param_3)) {
      iVar5 = FUN_00150bf0(DAT_0020f670,param_4);
      if (iVar5 == 0) goto LAB_00173294;
      if (DAT_0022b5b8 != DAT_0020f6f8) {
        DAT_0022b5b8 = DAT_0020f6f8;
        DAT_0022b5c8 = 0;
        DAT_0022b5c0 = 0;
        ram0x0022b5d8 = 0;
        _DAT_0022b5d0 = 0;
        uRam000000000022b5e8 = 0;
        DAT_0022b5e0 = 0;
        uRam000000000022b5f8 = 0;
        _DAT_0022b5f0 = 0;
        uRam000000000022b608 = 0;
        DAT_0022b600 = 0;
        uRam000000000022b618 = 0;
        _DAT_0022b610 = 0;
        uRam000000000022b628 = 0;
        DAT_0022b620 = 0;
        uRam000000000022b638 = 0;
        _DAT_0022b630 = 0;
        uRam000000000022b648 = 0;
        DAT_0022b640 = 0;
        uRam000000000022b658 = 0;
        _DAT_0022b650 = 0;
        uRam000000000022b668 = 0;
        DAT_0022b660 = 0;
        uRam000000000022b678 = 0;
        _DAT_0022b670 = 0;
        uRam000000000022b688 = 0;
        DAT_0022b680 = 0;
        uRam000000000022b698 = 0;
        _DAT_0022b690 = 0;
        DAT_0022b6a8 = 0;
        DAT_0022b6a0 = 0;
        ram0x0022b6b8 = 0;
        _DAT_0022b6b0 = 0;
      }
      lVar7 = (*(code *)(DAT_001e0978 + 0xd49da8))(param_5,"arrow",0,0,0,0,0);
      lVar17 = 0;
      lVar8 = lVar7;
      do {
        if (*(long *)((long)&DAT_0022b5c0 + lVar17) == param_5) {
          if (((lVar7 == *(long *)((long)&DAT_0022b5c8 + lVar17)) &&
              (lVar8 = FUN_001428fc(lVar8,lVar7 + 9,&local_180,7), (int)lVar8 != 0)) &&
             ((int)local_180 == *(int *)((long)&DAT_0022b5d7 + lVar17) &&
              CONCAT31(local_180._4_3_,local_180._3_1_) ==
              *(int *)((long)&DAT_0022b5d7 + lVar17 + 3))) {
            lVar8 = FUN_0014bef8(DAT_001cfb4c,&DAT_0022b5d0 + lVar17,7,lVar7 + 9);
          }
          *(undefined8 *)((long)&DAT_0022b5c8 + lVar17) = 0;
          *(long *)((long)&DAT_0022b5c0 + lVar17) = 0;
          *(undefined8 *)((long)&DAT_0022b5d7 + lVar17 + 1) = 0;
          *(undefined8 *)(&DAT_0022b5d0 + lVar17) = 0;
        }
        lVar17 = lVar17 + 0x20;
      } while (lVar17 != 0x100);
      bVar3 = true;
    }
  }
  else {
LAB_00173294:
    bVar3 = false;
  }
  *puVar6 = uVar1;
  uVar9 = (*DAT_0022b5a8)(param_1,param_2,param_3,param_4,param_5,param_6);
  uVar1 = *puVar6;
  if (bVar3) {
    lVar17 = 0;
    ppuVar15 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar5 = strcmp("TeammateHPIndicator",*ppuVar15);
      if (iVar5 == 0) {
        if ((((-1 < (int)lVar17) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar17 * 4) == 1))
            && (iVar5 = FUN_00165f8c(DAT_0020f678), -1 < (int)param_6)) &&
           ((iVar5 != 0 && (DAT_0020f6c8 != 0)))) {
          uVar12 = 0;
          uVar13 = 1;
          plVar16 = DAT_0020f6c0;
          goto LAB_0017335c;
        }
        break;
      }
      lVar17 = lVar17 + 1;
      ppuVar15 = ppuVar15 + 3;
    } while (lVar17 != 0x37);
  }
LAB_001735e0:
  *puVar6 = uVar1;
  if (*(long *)(lVar2 + 0x28) == local_80) {
    return uVar9;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
  while( true ) {
    plVar16 = plVar16 + 8;
    uVar13 = uVar13 + 1;
    if (0x1f < uVar12) break;
LAB_0017335c:
    if (((*plVar16 != param_4) && ((int)plVar16[5] != 0)) &&
       (((*(int *)((long)plVar16 + 0x2c) != 0 &&
         (((*(uint *)(plVar16 + 3) < 10 && (*(int *)((long)plVar16 + 0x1c) == param_3)) &&
          (*(uint *)(plVar16 + 4) <= *(uint *)((long)plVar16 + 0x24))))) &&
        (*(uint *)((long)plVar16 + 0x24) != 0)))) {
      (&local_180)[uVar12] = plVar16;
      uVar12 = uVar12 + 1;
    }
    if (DAT_0020f6c8 <= uVar13) break;
  }
  if (uVar12 != 0) {
    if (uVar12 <= param_6) {
      param_6 = uVar12 - 1;
    }
    puVar14 = (undefined8 *)(&local_180)[param_6];
    iVar5 = FUN_00189c88(DAT_001e0978,*puVar14,FUN_001428fc,0,auStack_1d0);
    if ((((iVar5 != 0) && (local_1c0 == *(int *)(puVar14 + 2))) &&
        ((local_1a4 != 0 && ((local_1ac != 0 && (local_1b0 <= local_1ac)))))) &&
       (lVar17 = (*(code *)(DAT_001e0978 + 0xd49da8))(param_5,"arrow",0,0,0,0,0), lVar17 != 0)) {
      if (DAT_0022b5c0 == 0) {
        lVar8 = 0;
        plVar16 = &DAT_0022b5c0;
      }
      else {
        plVar16 = &DAT_0022b5e0;
        if (DAT_0022b5e0 == 0) {
          lVar8 = 1;
        }
        else {
          plVar16 = &DAT_0022b600;
          if (DAT_0022b600 == 0) {
            lVar8 = 2;
          }
          else {
            plVar16 = &DAT_0022b620;
            if (DAT_0022b620 == 0) {
              lVar8 = 3;
            }
            else {
              plVar16 = &DAT_0022b640;
              if (DAT_0022b640 == 0) {
                lVar8 = 4;
              }
              else {
                plVar16 = &DAT_0022b660;
                if (DAT_0022b660 == 0) {
                  lVar8 = 5;
                }
                else {
                  plVar16 = &DAT_0022b680;
                  if (DAT_0022b680 == 0) {
                    lVar8 = 6;
                  }
                  else {
                    plVar16 = &DAT_0022b6a0;
                    if (DAT_0022b6a0 != 0) goto LAB_001735e0;
                    lVar8 = 7;
                  }
                }
              }
            }
          }
        }
      }
      lVar7 = lVar17 + 9;
      iVar5 = FUN_001428fc(lVar17,lVar7,&local_188,7);
      if (iVar5 != 0) {
        fVar18 = (float)NEON_ucvtf(local_1b0);
        fVar19 = (float)NEON_ucvtf(local_1ac);
        uVar10 = FUN_001391f0(1);
        FUN_00173620(fVar18 / fVar19,uVar10,&local_190);
        lVar11 = FUN_0014bef8(DAT_001cfb4c,&local_190,7,lVar7);
        if (lVar11 == 7) {
          lVar7 = lVar8 * 0x20;
          *plVar16 = param_5;
          (&DAT_0022b5c8)[lVar8 * 4] = lVar17;
          *(undefined4 *)(&DAT_0022b5d0 + lVar7) = local_188;
          *(uint *)(lVar7 + 0x22b5d3) = CONCAT31(uStack_184,local_188._3_1_);
          (&DAT_0022b5d7)[lVar8 * 8] = local_190;
          *(uint *)((long)&DAT_0022b5d7 + lVar7 + 3) = CONCAT31(uStack_18c,local_190._3_1_);
        }
        else {
          FUN_0014bef8(DAT_001cfb4c,&local_188,7,lVar7);
        }
      }
    }
  }
  goto LAB_001735e0;
}

/* ===== FUN_001750a8 @ 001750a8 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_001750a8(long param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  long lVar4;
  undefined **ppuVar5;
  
  puVar3 = (undefined4 *)__errno();
  uVar1 = *puVar3;
  if (((param_1 != 0) && ((_DAT_0022cf40 & 1) != 0)) && ((int)DAT_00214938 == 1)) {
    lVar4 = 0;
    ppuVar5 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar2 = strcmp("DisablePinAnimation",*ppuVar5);
      if (iVar2 == 0) {
        if ((-1 < (int)lVar4) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar4 * 4) != 0)) {
          *(undefined4 *)(param_1 + 0x110) = 0;
          *(undefined8 *)(param_1 + 8) = 0;
          *(undefined8 *)(param_1 + 0x10) = 0;
        }
        break;
      }
      lVar4 = lVar4 + 1;
      ppuVar5 = ppuVar5 + 3;
    } while (lVar4 != 0x37);
  }
  *puVar3 = uVar1;
  return;
}

/* ===== FUN_00175164 @ 00175164 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00175164(long param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  long lVar4;
  undefined **ppuVar5;
  
  puVar3 = (undefined4 *)__errno();
  uVar1 = *puVar3;
  if (((param_1 != 0) && (((uint)_DAT_0022cf40 >> 1 & 1) != 0)) && ((int)DAT_00214938 == 1)) {
    lVar4 = 0;
    ppuVar5 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar2 = strcmp("ShowFriendlyRoomOpponents",*ppuVar5);
      if (iVar2 == 0) {
        if ((-1 < (int)lVar4) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar4 * 4) != 0)) {
          *(undefined8 *)(param_1 + 0x30) = 0xff;
        }
        break;
      }
      lVar4 = lVar4 + 1;
      ppuVar5 = ppuVar5 + 3;
    } while (lVar4 != 0x37);
  }
  *puVar3 = uVar1;
  return;
}

/* ===== FUN_00176bbc @ 00176bbc [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00176bbc(long param_1)

{
  long lVar1;
  int iVar2;
  ulong uVar3;
  long lVar4;
  undefined **ppuVar5;
  int local_a8;
  int local_a4;
  ulong local_a0;
  ulong local_98;
  ulong local_90;
  ulong local_88;
  ulong local_80;
  ulong local_78;
  undefined1 auStack_6c [4];
  int local_68;
  undefined8 local_64;
  long local_5c;
  short local_54;
  int local_50;
  int local_4c;
  long local_48;
  
  lVar1 = tpidr_el0;
  local_48 = *(long *)(lVar1 + 0x28);
  if (((int)_DAT_0022cf44 != 0) && ((int)DAT_00214938 == 1)) {
    lVar4 = 0;
    ppuVar5 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar2 = strcmp("HighlightLeonClone",*ppuVar5);
      if (iVar2 == 0) {
        if ((-1 < (int)lVar4) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar4 * 4) != 0)) {
          uVar3 = FUN_001428fc(0,param_1,&local_78,8);
          if (((int)uVar3 == 0) ||
             ((uVar3 = 0, local_78 + 0x2000 < 0x12000 || ((local_78 & 7) != 0)))) goto LAB_00176ea8;
          if (local_78 == DAT_001e0978 + 0x121c438U) {
            uVar3 = FUN_001428fc(0,param_1 + 0x20,&local_a4,4);
            if ((int)uVar3 == 0) goto LAB_00176ea8;
            if ((local_a4 == 0x10366f9) || (local_a4 == 0xf424b9)) {
              iVar2 = FUN_001428fc(uVar3,DAT_001e0978 + 0x1244690,&local_50,8);
              uVar3 = 0;
              if (((iVar2 != 0) &&
                  (((local_50 == 0x91 && (local_4c == 0x92)) &&
                   (uVar3 = FUN_001428fc(0,param_1 + 8,&local_80,8), (int)uVar3 != 0)))) &&
                 ((uVar3 = 0, 0x11fff < local_80 + 0x2000 && ((local_80 & 7) == 0)))) {
                iVar2 = FUN_001428fc(0,local_80 + 8,&local_a8,4);
                uVar3 = 0;
                if ((((iVar2 != 0) &&
                     (((local_a8 == 0xb9 &&
                       (uVar3 = FUN_001428fc(0,local_80,&local_88,8), (int)uVar3 != 0)) &&
                      ((uVar3 = 0, 0x11fff < local_88 + 0x2000 &&
                       ((((local_88 & 7) == 0 &&
                         (uVar3 = FUN_001428fc(0,local_88 + 0x38,&local_90,8), (int)uVar3 != 0)) &&
                        (uVar3 = 0, 0x11fff < local_90 + 0x2000)))))))) &&
                    ((((local_90 & 7) == 0 &&
                      (uVar3 = FUN_001428fc(0,local_90,&local_98,8), (int)uVar3 != 0)) &&
                     (uVar3 = 0, 0x11fff < local_98 + 0x2000)))) &&
                   ((((local_98 & 7) == 0 &&
                     (uVar3 = FUN_001428fc(0,local_98 + 8,&local_a0,8), (int)uVar3 != 0)) &&
                    ((uVar3 = 0, 0x11fff < local_a0 + 0x2000 && ((local_a0 & 7) == 0)))))) {
                  iVar2 = FUN_001428fc(0,local_a0 + (long)local_a8 * 0x10,auStack_6c,0x10);
                  uVar3 = 0;
                  if (((iVar2 != 0) && (local_68 == 9)) &&
                     (uVar3 = FUN_001428fc(0,local_64,&local_5c,10), (int)uVar3 != 0)) {
                    uVar3 = (ulong)(local_5c == 0x6b6146616a6e694e && local_54 == 0x65);
                  }
                }
              }
              goto LAB_00176ea8;
            }
          }
        }
        break;
      }
      lVar4 = lVar4 + 1;
      ppuVar5 = ppuVar5 + 3;
    } while (lVar4 != 0x37);
  }
  uVar3 = 0;
LAB_00176ea8:
  if (*(long *)(lVar1 + 0x28) == local_48) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar3);
}

/* ===== FUN_00176ed4 @ 00176ed4 [libNexusEvasionRuntime69252.so] ===== */

void FUN_00176ed4(long param_1)

{
  undefined4 uVar1;
  long lVar2;
  int iVar3;
  undefined4 *puVar4;
  undefined8 uVar5;
  long lVar6;
  undefined **ppuVar7;
  long local_70;
  ulong local_68;
  ulong local_60;
  long local_58;
  
  lVar2 = tpidr_el0;
  local_58 = *(long *)(lVar2 + 0x28);
  puVar4 = (undefined4 *)__errno();
  uVar1 = *puVar4;
  if (((param_1 != 0) && (((uint)DAT_0022d064 >> 1 & 1) != 0)) && ((int)DAT_00214938 == 1)) {
    lVar6 = 0;
    ppuVar7 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar3 = strcmp("DisableSkins",*ppuVar7);
      if (iVar3 == 0) {
        if (((((-1 < (int)lVar6) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar6 * 4) != 0)) &&
             ((uVar5 = FUN_001428fc(0,*(long *)(param_1 + 8) + 0x10,&local_60,8), (int)uVar5 != 0 &&
              ((0x11fff < local_60 + 0x2000 && ((local_60 & 7) == 0)))))) &&
            (iVar3 = FUN_001428fc(uVar5,local_60 + 0x10,&local_68,8), iVar3 != 0)) &&
           (((0x11fff < local_68 + 0x2000 && ((local_68 & 7) == 0)) &&
            (local_70 = FUN_001775a4(), local_70 != 0)))) {
          FUN_0014bef8(DAT_001cfb4c,&local_70,8,*(long *)(param_1 + 8) + 0xb18);
        }
        break;
      }
      lVar6 = lVar6 + 1;
      ppuVar7 = ppuVar7 + 3;
    } while (lVar6 != 0x37);
  }
  *puVar4 = uVar1;
  if (*(long *)(lVar2 + 0x28) != local_58) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

/* ===== FUN_00177044 @ 00177044 [libNexusEvasionRuntime69252.so] ===== */

void FUN_00177044(long param_1)

{
  undefined4 uVar1;
  long lVar2;
  int iVar3;
  undefined4 *puVar4;
  undefined8 uVar5;
  long lVar6;
  undefined **ppuVar7;
  int local_80;
  uint local_7c;
  long local_78;
  ulong local_70;
  ulong local_68;
  ulong local_60;
  long local_58;
  
  lVar2 = tpidr_el0;
  local_58 = *(long *)(lVar2 + 0x28);
  puVar4 = (undefined4 *)__errno();
  uVar1 = *puVar4;
  if (((param_1 != 0) && (((uint)DAT_0022d064 >> 2 & 1) != 0)) && ((int)DAT_00214938 == 1)) {
    lVar6 = 0;
    ppuVar7 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar3 = strcmp("DisableSkins",*ppuVar7);
      if (iVar3 == 0) {
        if ((-1 < (int)lVar6) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar6 * 4) != 0)) {
          lVar6 = *(long *)(param_1 + 8);
          uVar5 = FUN_001428fc(0,lVar6 + 0x70,&local_60,8);
          if (((((int)uVar5 != 0) && ((0x11fff < local_60 + 0x2000 && ((local_60 & 7) == 0)))) &&
              (uVar5 = FUN_001428fc(uVar5,lVar6 + 0x7c,&local_80,4), (int)uVar5 != 0)) &&
             (((((0 < local_80 && (local_80 < 5)) &&
                (uVar5 = FUN_001428fc(uVar5,lVar6 + 0x80,&local_7c,4), (int)uVar5 != 0)) &&
               (((-1 < (int)local_7c && ((int)local_7c < local_80)) &&
                ((uVar5 = FUN_001428fc(uVar5,local_60 + (ulong)local_7c * 8,&local_68,8),
                 (int)uVar5 != 0 && ((0x11fff < local_68 + 0x2000 && ((local_68 & 7) == 0)))))))) &&
              ((iVar3 = FUN_001428fc(uVar5,local_68,&local_70,8), iVar3 != 0 &&
               (((0x11fff < local_70 + 0x2000 && ((local_70 & 7) == 0)) &&
                (local_78 = FUN_001775a4(), local_78 != 0)))))))) {
            FUN_0014bef8(DAT_001cfb4c,&local_78,8,lVar6 + 0x218);
            FUN_0014bef8(DAT_001cfb4c,&local_78,8,local_68 + 0x20);
          }
        }
        break;
      }
      lVar6 = lVar6 + 1;
      ppuVar7 = ppuVar7 + 3;
    } while (lVar6 != 0x37);
  }
  *puVar4 = uVar1;
  if (*(long *)(lVar2 + 0x28) != local_58) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

/* ===== FUN_00177244 @ 00177244 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00177244(long *param_1)

{
  undefined4 uVar1;
  long lVar2;
  int iVar3;
  undefined4 *puVar4;
  undefined8 uVar5;
  long lVar6;
  long lVar7;
  int *piVar8;
  undefined **ppuVar9;
  int local_7c;
  int local_78;
  int iStack_74;
  undefined8 local_70;
  undefined8 uStack_68;
  long local_58;
  
  lVar2 = tpidr_el0;
  local_58 = *(long *)(lVar2 + 0x28);
  puVar4 = (undefined4 *)__errno();
  uVar1 = *puVar4;
  if (((param_1 != (long *)0x0) && ((int)ram0x0022d068 != 0)) && ((int)DAT_00214938 == 1)) {
    lVar6 = 0;
    ppuVar9 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar3 = strcmp("DefaultEnvironments",*ppuVar9);
      if (iVar3 == 0) {
        if ((-1 < (int)lVar6) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar6 * 4) != 0)) {
          lVar6 = param_1[0x13];
          lVar7 = *param_1;
          uVar5 = FUN_001428fc(0,lVar6 + 0x20,&iStack_74,4);
          if (((int)uVar5 != 0) &&
             (iVar3 = FUN_001428fc(uVar5,lVar7 + 0x20,&local_78,4), iVar3 != 0)) {
            lVar7 = 0x2f4;
            piVar8 = &DAT_001c5b38;
            goto LAB_00177344;
          }
        }
        break;
      }
      lVar6 = lVar6 + 1;
      ppuVar9 = ppuVar9 + 3;
    } while (lVar6 != 0x37);
  }
  goto LAB_001773dc;
  while( true ) {
    piVar8 = piVar8 + 6;
    lVar7 = lVar7 + -1;
    if (lVar7 == 0) break;
LAB_00177344:
    if ((piVar8[-2] == iStack_74) && (piVar8[-1] == local_78)) {
      local_70 = 0;
      uStack_68 = 0;
      (*(code *)(DAT_001e0978 + 0x66ae58))(&local_70,*(undefined8 *)(piVar8 + 2));
      lVar6 = (*(code *)(DAT_001e0978 + 0xe205d0))(&local_70,lVar6);
      uVar5 = (*(code *)(DAT_001e0978 + 0x66ad48))(&local_70);
      if ((lVar6 != 0) &&
         ((iVar3 = FUN_001428fc(uVar5,lVar6 + 0x20,&local_7c,4), iVar3 != 0 && (local_7c == *piVar8)
          ))) {
        *param_1 = lVar6;
      }
      break;
    }
  }
LAB_001773dc:
  *puVar4 = uVar1;
  if (*(long *)(lVar2 + 0x28) != local_58) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

/* ===== FUN_00177410 @ 00177410 [libNexusEvasionRuntime69252.so] ===== */

void FUN_00177410(long param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  long lVar4;
  undefined **ppuVar5;
  
  puVar3 = (undefined4 *)__errno();
  uVar1 = *puVar3;
  if (((param_1 != 0) && ((int)DAT_0022d06c != 0)) && ((int)DAT_00214938 == 1)) {
    lVar4 = 0;
    ppuVar5 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar2 = strcmp("DisableShake",*ppuVar5);
      if (iVar2 == 0) {
        if ((-1 < (int)lVar4) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar4 * 4) != 0)) {
          *(undefined4 *)(param_1 + 0x110) = 0;
        }
        break;
      }
      lVar4 = lVar4 + 1;
      ppuVar5 = ppuVar5 + 3;
    } while (lVar4 != 0x37);
  }
  *puVar3 = uVar1;
  return;
}

/* ===== FUN_001774c8 @ 001774c8 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

long FUN_001774c8(undefined8 param_1,undefined8 param_2,undefined8 param_3)

{
  undefined4 uVar1;
  int iVar2;
  long lVar3;
  undefined4 *puVar4;
  long lVar5;
  undefined **ppuVar6;
  
  lVar3 = (*ram0x0022d070)();
  puVar4 = (undefined4 *)__errno();
  uVar1 = *puVar4;
  if (((DAT_0022d064 & 1) != 0) && ((int)DAT_00214938 == 1)) {
    lVar5 = 0;
    ppuVar6 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar2 = strcmp("DisableSkins",*ppuVar6);
      if (iVar2 == 0) {
        if (((-1 < (int)lVar5) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar5 * 4) != 0)) &&
           (lVar5 = FUN_001775a4(param_3), lVar5 != 0)) {
          lVar3 = lVar5;
        }
        break;
      }
      lVar5 = lVar5 + 1;
      ppuVar6 = ppuVar6 + 3;
    } while (lVar5 != 0x37);
  }
  *puVar4 = uVar1;
  return lVar3;
}

/* ===== FUN_00177bd8 @ 00177bd8 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00177bd8(long param_1)

{
  undefined4 uVar1;
  long lVar2;
  ulong uVar3;
  void *pvVar4;
  bool bVar5;
  int iVar6;
  undefined4 *puVar7;
  void *pvVar8;
  void *pvVar9;
  undefined8 uVar10;
  char *pcVar11;
  char *pcVar12;
  void *pvVar13;
  long lVar14;
  undefined **ppuVar15;
  ulong uVar16;
  undefined4 local_4bc0;
  int local_4bbc;
  ulong local_4bb8;
  timespec local_4bb0;
  undefined1 auStack_4ba0 [16000];
  undefined1 auStack_d20 [3072];
  undefined1 auStack_120 [16];
  undefined1 auStack_110 [48];
  undefined1 auStack_e0 [48];
  undefined8 local_b0;
  undefined8 uStack_a8;
  undefined8 uStack_a0;
  undefined8 local_98;
  ulong local_90;
  undefined8 uStack_88;
  long local_78;
  
  lVar2 = tpidr_el0;
  local_78 = *(long *)(lVar2 + 0x28);
  (*DAT_0022d080)();
  puVar7 = (undefined4 *)__errno();
  uVar1 = *puVar7;
  if (((((int)_DAT_0022d088 == 3) &&
       (iVar6 = pthread_once((pthread_once_t *)&DAT_0021ca04,FUN_00164598),
       DAT_0021ca08 != (code *)0x0)) && (iVar6 = (*DAT_0021ca08)(iVar6), iVar6 == 1)) &&
     ((int)DAT_00214938 == 1)) {
    lVar14 = 0;
    ppuVar15 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar6 = strcmp("UseBattleProxy",*ppuVar15);
      if (iVar6 == 0) {
        if ((-1 < (int)lVar14) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar14 * 4) != 0)) {
          pthread_mutex_lock((pthread_mutex_t *)&DAT_0022d08c);
          uStack_a8 = uRam000000000022d0c0;
          local_b0 = _DAT_0022d0b8;
          local_98 = uRam000000000022d0d0;
          uStack_a0 = _DAT_0022d0c8;
          uStack_88 = DAT_0022d0e0;
          local_90 = _DAT_0022d0d8;
          pthread_mutex_unlock((pthread_mutex_t *)&DAT_0022d08c);
          iVar6 = clock_gettime(0,&local_4bb0);
          if (iVar6 == 0) {
            uVar16 = (local_4bb0.tv_sec * 1000 + (ulong)local_4bb0.tv_nsec / 1000000) / 1000;
          }
          else {
            uVar16 = 0;
          }
          pvVar8 = (void *)strlen((char *)&local_b0);
          pvVar9 = pvVar8;
          if (0xffffffffffffffe8 < (long)pvVar8 - 0x18U) {
            pvVar9 = memchr("0289PYLQGRJCUV",(uint)(byte)local_b0,0xf);
            if (pvVar9 != (void *)0x0) {
              pvVar4 = (void *)0x1;
              goto LAB_00177d94;
            }
          }
          bVar5 = true;
          goto LAB_00177dc8;
        }
        break;
      }
      lVar14 = lVar14 + 1;
      ppuVar15 = ppuVar15 + 3;
    } while (lVar14 != 0x37);
  }
  goto LAB_00177f1c;
  while( true ) {
    pvVar9 = memchr("0289PYLQGRJCUV",(uint)*(byte *)((long)&local_b0 + (long)pvVar13),0xf);
    pvVar4 = (void *)((long)pvVar13 + 1);
    if (pvVar9 == (void *)0x0) break;
LAB_00177d94:
    pvVar13 = pvVar4;
    if (pvVar8 == pvVar13) break;
  }
  bVar5 = pvVar13 < pvVar8;
LAB_00177dc8:
  uVar3 = local_90;
  if (((((bVar5) || (local_90 == 0)) ||
       ((uVar16 < local_90 ||
        (((uVar16 - local_90 >> 0x1f != 0 ||
          (uVar10 = FUN_001428fc(pvVar9,param_1 + 0x90,&local_4bbc,4), (int)uVar10 == 0)) ||
         (local_4bbc < 1)))))) ||
      ((0xffff < local_4bbc ||
       (uVar10 = FUN_001428fc(uVar10,param_1 + 0x98,&local_4bb8,8), (int)uVar10 == 0)))) ||
     ((local_4bb8 + 0x2000 < 0x12000 ||
      ((((local_4bb8 & 7) != 0 ||
        (iVar6 = FUN_001428fc(uVar10,local_4bb8,auStack_120,0x10), iVar6 == 0)) ||
       (iVar6 = FUN_00177f6c(local_4bb8,auStack_e0), iVar6 == 0)))))) {
    pcVar11 = "battle_proxy_refused";
    pcVar12 = "session_or_udp_endpoint_unavailable";
  }
  else {
    local_4bc0 = 0;
    iVar6 = FUN_00178090(&local_b0,auStack_e0,local_4bbc,(uVar3 - uVar16) + (long)(int)local_98,
                         auStack_d20);
    if (iVar6 == 0) goto LAB_00177f1c;
    iVar6 = FUN_00178390(auStack_d20,auStack_4ba0);
    if ((iVar6 == 0) || (iVar6 = FUN_00178a30(auStack_4ba0,auStack_110,&local_4bc0), iVar6 == 0)) {
      pcVar11 = "battle_proxy_unavailable";
      pcVar12 = "original_BSD_service_or_entitlement_required";
    }
    else {
      iVar6 = FUN_0017919c(&local_b0);
      if (iVar6 == 0) goto LAB_00177f1c;
      iVar6 = FUN_001792b0(param_1,local_4bb8,auStack_120,local_4bbc,auStack_110,local_4bc0);
      pcVar11 = "battle_proxy_refused";
      pcVar12 = "decoded_endpoint_changed";
      if (iVar6 != 0) {
        pcVar11 = "battle_proxy_applied";
        pcVar12 = "provider_endpoint_committed";
      }
    }
  }
  FUN_001417c8(pcVar11,pcVar12,0);
LAB_00177f1c:
  *puVar7 = uVar1;
  if (*(long *)(lVar2 + 0x28) == local_78) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0017919c @ 0017919c [libNexusEvasionRuntime69252.so] ===== */

bool FUN_0017919c(char *param_1)

{
  int iVar1;
  long lVar2;
  undefined **ppuVar3;
  
  pthread_mutex_lock((pthread_mutex_t *)&DAT_0022d08c);
  if (DAT_0022d0e0 == *(long *)(param_1 + 0x28)) {
    iVar1 = strcmp(param_1,&DAT_0022d0b8);
    pthread_mutex_unlock((pthread_mutex_t *)&DAT_0022d08c);
    if ((((iVar1 == 0) &&
         (iVar1 = pthread_once((pthread_once_t *)&DAT_0021ca04,FUN_00164598),
         DAT_0021ca08 != (code *)0x0)) && (iVar1 = (*DAT_0021ca08)(iVar1), iVar1 == 1)) &&
       ((int)DAT_00214938 == 1)) {
      lVar2 = 0;
      ppuVar3 = &PTR_s_DisablePinAnimation_001c2838;
      while (iVar1 = strcmp("UseBattleProxy",*ppuVar3), iVar1 != 0) {
        lVar2 = lVar2 + 1;
        ppuVar3 = ppuVar3 + 3;
        if (lVar2 == 0x37) {
          return false;
        }
      }
      if (-1 < (int)lVar2) {
        return (int)*(undefined8 *)((long)&DAT_001dfea0 + lVar2 * 4) != 0;
      }
    }
  }
  else {
    pthread_mutex_unlock((pthread_mutex_t *)&DAT_0022d08c);
  }
  return false;
}

/* ===== FUN_00179ad4 @ 00179ad4 [libNexusEvasionRuntime69252.so] ===== */

void FUN_00179ad4(undefined8 *param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  long lVar4;
  undefined **ppuVar5;
  
  puVar3 = (undefined4 *)__errno();
  uVar1 = *puVar3;
  if ((param_1 != (undefined8 *)0x0) && (DAT_0022d134 != 0)) {
    lVar4 = 0;
    ppuVar5 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar2 = strcmp("SlowMode",*ppuVar5);
      if (iVar2 == 0) {
        if ((-1 < (int)lVar4) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar4 * 4) != 0)) {
          *param_1 = 1;
        }
        break;
      }
      lVar4 = lVar4 + 1;
      ppuVar5 = ppuVar5 + 3;
    } while (lVar4 != 0x37);
  }
  *puVar3 = uVar1;
  return;
}

/* ===== FUN_00179e14 @ 00179e14 [libNexusEvasionRuntime69252.so] ===== */

void FUN_00179e14(undefined8 param_1,undefined8 param_2)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined8 uVar4;
  long lVar5;
  undefined **ppuVar6;
  
  puVar3 = (undefined4 *)__errno();
  uVar1 = *puVar3;
  if ((DAT_0022d14c & 1) != 0) {
    lVar5 = 0;
    ppuVar6 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar2 = strcmp("DoNotShowBattleHighlight",*ppuVar6);
      if (iVar2 == 0) {
        if (-1 < (int)lVar5) {
          uVar4 = *(undefined8 *)((long)&DAT_001dfea0 + lVar5 * 4);
          *puVar3 = uVar1;
          if ((int)uVar4 != 0) {
            return;
          }
          goto LAB_00179ec0;
        }
        break;
      }
      lVar5 = lVar5 + 1;
      ppuVar6 = ppuVar6 + 3;
    } while (lVar5 != 0x37);
  }
  *puVar3 = uVar1;
LAB_00179ec0:
                    /* WARNING: Could not recover jumptable at 0x00179ee4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*DAT_0022d138)(param_1,param_2);
  return;
}

/* ===== FUN_00179ee8 @ 00179ee8 [libNexusEvasionRuntime69252.so] ===== */

undefined4 FUN_00179ee8(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 *puVar4;
  long lVar5;
  undefined **ppuVar6;
  
  uVar2 = (*DAT_0022d140)();
  puVar4 = (undefined4 *)__errno();
  uVar1 = *puVar4;
  if (((uint)DAT_0022d14c >> 1 & 1) != 0) {
    lVar5 = 0;
    ppuVar6 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar3 = strcmp("EnforceBattleChatButton",*ppuVar6);
      if (iVar3 == 0) {
        if ((-1 < (int)lVar5) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar5 * 4) != 0)) {
          uVar2 = 1;
        }
        break;
      }
      lVar5 = lVar5 + 1;
      ppuVar6 = ppuVar6 + 3;
    } while (lVar5 != 0x37);
  }
  *puVar4 = uVar1;
  return uVar2;
}

/* ===== FUN_00179f98 @ 00179f98 [libNexusEvasionRuntime69252.so] ===== */

void FUN_00179f98(long param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  long lVar4;
  undefined **ppuVar5;
  
  puVar3 = (undefined4 *)__errno();
  uVar1 = *puVar3;
  if ((param_1 != 0) && (DAT_0022d148 == 7)) {
    lVar4 = 0;
    ppuVar5 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar2 = strcmp("EnforceBattleChatButton",*ppuVar5);
      if (iVar2 == 0) {
        if (((-1 < (int)lVar4) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar4 * 4) != 0)) &&
           (lVar4 = FUN_00139fcc(), lVar4 != 0)) {
          *(undefined8 *)(param_1 + 0xb8) = 1;
        }
        break;
      }
      lVar4 = lVar4 + 1;
      ppuVar5 = ppuVar5 + 3;
    } while (lVar4 != 0x37);
  }
  *puVar3 = uVar1;
  return;
}

/* ===== FUN_0017a42c @ 0017a42c [libNexusEvasionRuntime69252.so] ===== */

undefined1  [16] FUN_0017a42c(void)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  long lVar4;
  undefined **ppuVar5;
  long unaff_x30;
  double dVar6;
  undefined1 auVar7 [16];
  
  dVar6 = (double)(*DAT_0022d168)();
  puVar3 = (undefined4 *)__errno();
  uVar1 = *puVar3;
  if ((((DAT_00209a78 & 1) != 0) && ((int)DAT_001dfff0 != 0)) &&
     (DAT_001e0978 + 0x7316cc == unaff_x30)) {
    lVar4 = 0;
    ppuVar5 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar2 = strcmp("FPSLimit",*ppuVar5);
      if (iVar2 == 0) {
        if (-1 < (int)lVar4) {
          iVar2 = (int)*(undefined8 *)((long)&DAT_001dfea0 + lVar4 * 4);
          goto LAB_0017a4f4;
        }
        break;
      }
      lVar4 = lVar4 + 1;
      ppuVar5 = ppuVar5 + 3;
    } while (lVar4 != 0x37);
    iVar2 = 0;
LAB_0017a4f4:
    dVar6 = (double)iVar2;
    if (0x8f < iVar2 - 1U) {
      dVar6 = 600.0;
    }
  }
  *puVar3 = uVar1;
  auVar7._8_8_ = 0;
  auVar7._0_8_ = dVar6;
  return auVar7;
}

/* ===== FUN_0017a530 @ 0017a530 [libNexusEvasionRuntime69252.so] ===== */

undefined1  [16] FUN_0017a530(void)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  long lVar4;
  undefined **ppuVar5;
  long unaff_x30;
  double dVar6;
  undefined1 auVar7 [16];
  
  dVar6 = (double)(*DAT_0022d178)();
  puVar3 = (undefined4 *)__errno();
  uVar1 = *puVar3;
  if ((((DAT_00209a78 & 1) != 0) && ((int)DAT_001dfff0 != 0)) &&
     (DAT_001e0978 + 0x7316d4 == unaff_x30)) {
    lVar4 = 0;
    ppuVar5 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar2 = strcmp("FPSLimit",*ppuVar5);
      if (iVar2 == 0) {
        if (-1 < (int)lVar4) {
          iVar2 = (int)*(undefined8 *)((long)&DAT_001dfea0 + lVar4 * 4);
          goto LAB_0017a5f8;
        }
        break;
      }
      lVar4 = lVar4 + 1;
      ppuVar5 = ppuVar5 + 3;
    } while (lVar4 != 0x37);
    iVar2 = 0;
LAB_0017a5f8:
    dVar6 = (double)iVar2;
    if (0x8f < iVar2 - 1U) {
      dVar6 = 600.0;
    }
  }
  *puVar3 = uVar1;
  auVar7._8_8_ = 0;
  auVar7._0_8_ = dVar6;
  return auVar7;
}

/* ===== FUN_0017a808 @ 0017a808 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0017a808(undefined4 param_1)

{
  uint uVar1;
  undefined4 uVar2;
  char cVar3;
  bool bVar4;
  float fVar5;
  code *UNRECOVERED_JUMPTABLE;
  int iVar6;
  undefined4 *puVar7;
  uint uVar8;
  ulong uVar9;
  ulong uVar10;
  float *pfVar11;
  long lVar12;
  undefined **ppuVar13;
  float fVar14;
  float fVar15;
  
  puVar7 = (undefined4 *)__errno();
  uVar2 = *puVar7;
  do {
    uVar8 = _DAT_0022d190;
    cVar3 = '\x01';
    bVar4 = (bool)ExclusiveMonitorPass(0x22d190,0x10);
    if (bVar4) {
      _DAT_0022d190 = CONCAT31(DAT_0022d190_1,1);
      cVar3 = ExclusiveMonitorsStatus();
    }
  } while (cVar3 != '\0');
  if ((uVar8 & 1) == 0) {
    if (((DAT_00209a78 & 1) != 0) && ((int)DAT_001dfff0 != 0)) {
      lVar12 = 0;
      ppuVar13 = &PTR_s_DisablePinAnimation_001c2838;
      do {
        iVar6 = strcmp("FPSLimit",*ppuVar13);
        if (iVar6 == 0) {
          if (-1 < (int)lVar12) {
            iVar6 = (int)*(undefined8 *)((long)&DAT_001dfea0 + lVar12 * 4);
            goto LAB_0017a8c4;
          }
          break;
        }
        lVar12 = lVar12 + 1;
        ppuVar13 = ppuVar13 + 3;
      } while (lVar12 != 0x37);
      iVar6 = 0;
LAB_0017a8c4:
      if (0xffffffdf < DAT_0022d194 - 0x21) {
        fVar5 = (float)iVar6;
        uVar9 = 0;
        uVar8 = 0;
        uVar10 = 0xffffffff;
        pfVar11 = (float *)&DAT_0022d19c;
        if (0x8f < iVar6 - 1U) {
          fVar5 = 600.0;
        }
        do {
          fVar14 = *pfVar11;
          fVar15 = (float)(&DAT_0022d19c)[(ulong)uVar8 * 2];
          uVar1 = (uint)uVar9;
          if (fVar14 == fVar15 || fVar14 < fVar15 != (NAN(fVar14) || NAN(fVar15))) {
            uVar1 = uVar8;
          }
          if ((fVar5 <= fVar14) &&
             (((int)uVar10 < 0 || (fVar14 < (float)(&DAT_0022d19c)[uVar10 * 2])))) {
            uVar10 = uVar9 & 0xffffffff;
          }
          uVar9 = uVar9 + 1;
          pfVar11 = pfVar11 + 2;
          uVar8 = uVar1;
        } while (DAT_0022d194 != uVar9);
        if (-1 < (int)(uint)uVar10) {
          uVar1 = (uint)uVar10;
        }
        param_1 = (&DAT_0022d198)[(ulong)uVar1 * 2];
      }
    }
    DAT_0022d194 = 0;
    _DAT_0022d190 = 0;
  }
  UNRECOVERED_JUMPTABLE = DAT_0022d188;
  *puVar7 = uVar2;
                    /* WARNING: Could not recover jumptable at 0x0017a998. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*UNRECOVERED_JUMPTABLE)(param_1);
  return;
}

/* ===== FUN_0017a99c @ 0017a99c [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0017a99c(long param_1,undefined8 *param_2,undefined4 param_3,undefined8 param_4,
                 undefined8 param_5,undefined4 param_6,undefined4 param_7,undefined4 param_8)

{
  undefined4 uVar1;
  char cVar2;
  bool bVar3;
  bool bVar4;
  long lVar5;
  long lVar6;
  uint uVar7;
  bool bVar8;
  int iVar9;
  undefined4 *puVar10;
  int *piVar11;
  undefined8 uVar12;
  size_t sVar13;
  undefined8 *puVar14;
  long lVar15;
  undefined **ppuVar16;
  undefined8 *local_250;
  undefined8 local_220;
  undefined8 local_218;
  undefined8 uStack_210;
  char local_204 [404];
  long local_70;
  
  lVar5 = tpidr_el0;
  local_70 = *(long *)(lVar5 + 0x28);
  puVar10 = (undefined4 *)__errno();
  lVar6 = DAT_001cfbe8;
  uVar1 = *puVar10;
  piVar11 = (int *)FUN_001bd828(&DAT_001cfd78);
  if (((((*piVar11 != 0) && (local_204[0] = '\0', (DAT_0020af9c >> 3 & 1) != 0)) &&
       ((((uint)DAT_0020afa0 ^ 0xffffffff) & 3) == 0)) &&
      ((DAT_0020afa0._4_1_ != '\0' && (DAT_0020af50 != '\0')))) && ((int)DAT_001dfff0 != 0)) {
    do {
      uVar7 = _DAT_001e12e8;
      cVar2 = '\x01';
      bVar3 = (bool)ExclusiveMonitorPass(0x1e12e8,0x10);
      if (bVar3) {
        _DAT_001e12e8 = CONCAT31(DAT_001e12e8_1,1);
        cVar2 = ExclusiveMonitorsStatus();
      }
    } while (cVar2 != '\0');
    if ((uVar7 & 1) == 0) {
      memcpy(local_204,&DAT_001e1311,0x191);
      _DAT_001e12e8 = 0;
      if (local_204[0] != '\0') {
        local_218 = 0;
        uStack_210 = 0;
        (*(code *)(DAT_001e0978 + 0x66ae58))(&local_218,local_204);
        bVar3 = false;
        puVar14 = &local_218;
        goto LAB_0017aa5c;
      }
    }
  }
  bVar3 = true;
  puVar14 = param_2;
LAB_0017aa5c:
  local_250 = &local_218;
  if (((((uint)DAT_0020afa0 ^ 0xffffffff) & 3) == 0) && ((int)DAT_001dfff0 != 0)) {
    lVar15 = 0;
    ppuVar16 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar9 = strcmp("ChromaticName",*ppuVar16);
      if (iVar9 == 0) {
        if ((-1 < (int)lVar15) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar15 * 4) != 0)) {
          param_6 = 0xfffffffe;
        }
        break;
      }
      lVar15 = lVar15 + 1;
      ppuVar16 = ppuVar16 + 3;
    } while (lVar15 != 0x37);
  }
  *puVar10 = uVar1;
  (*DAT_0022d2a0)(param_1,puVar14,param_3,param_4,param_5,param_6,param_7,param_8);
  uVar1 = *puVar10;
  local_220 = 0;
  uVar12 = FUN_0014a6ac(param_2,local_204,0xc0);
  bVar8 = false;
  if (((int)uVar12 != 0) && (local_204[0] != '\0')) {
    uVar12 = FUN_001428fc(uVar12,param_1 + 0x14,(long)&local_220 + 4,4);
    if ((int)uVar12 == 0) {
      bVar8 = false;
    }
    else {
      iVar9 = FUN_001428fc(uVar12,param_1 + 0x18,&local_220,4);
      bVar8 = iVar9 != 0;
    }
  }
  do {
    uVar7 = _DAT_0022d430;
    cVar2 = '\x01';
    bVar4 = (bool)ExclusiveMonitorPass(0x22d430,0x10);
    if (bVar4) {
      _DAT_0022d430 = CONCAT31(DAT_0022d430_1,1);
      cVar2 = ExclusiveMonitorsStatus();
    }
  } while (cVar2 != '\0');
  if ((uVar7 & 1) == 0) {
    DAT_0022d508 = 0;
    if ((bVar8) && (lVar6 == DAT_001cfbe8)) {
      sVar13 = strlen(local_204);
      memcpy(&DAT_0022d438,local_204,sVar13 + 1);
      DAT_0022d500 = lVar6;
      DAT_0022d4f8 = local_220._4_4_;
      DAT_0022d4fc = (undefined4)local_220;
      DAT_0022d508 = 1;
    }
    _DAT_0022d430 = 0;
  }
  if (!bVar3) {
    (*(code *)(DAT_001e0978 + 0x66ad48))(local_250);
  }
  *puVar10 = uVar1;
  if (*(long *)(lVar5 + 0x28) != local_70) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

/* ===== FUN_0017ace4 @ 0017ace4 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0017ace4(long param_1)

{
  undefined4 uVar1;
  char cVar2;
  bool bVar3;
  long lVar4;
  uint uVar5;
  int iVar6;
  undefined4 *puVar7;
  undefined8 uVar8;
  long lVar9;
  long lVar10;
  undefined **ppuVar11;
  undefined8 local_390;
  int local_388;
  int local_384;
  int local_380;
  char acStack_37c [192];
  char acStack_2bc [192];
  char local_1fc [404];
  long local_68;
  
  lVar4 = tpidr_el0;
  local_68 = *(long *)(lVar4 + 0x28);
  puVar7 = (undefined4 *)__errno();
  uVar1 = *puVar7;
  if (param_1 != 0) {
    lVar9 = *(long *)(param_1 + 0x98);
    local_388 = 0;
    local_390 = 0;
    if (lVar9 != 0) {
      if (((((uint)DAT_0020afa0 ^ 0xffffffff) & 3) == 0) && ((int)DAT_001dfff0 != 0)) {
        lVar10 = 0;
        ppuVar11 = &PTR_s_DisablePinAnimation_001c2838;
        do {
          iVar6 = strcmp("ChromaticName",*ppuVar11);
          if (iVar6 == 0) {
            if ((((-1 < (int)lVar10) &&
                 ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar10 * 4) != 0)) &&
                (uVar8 = FUN_0014a6ac(lVar9,acStack_37c,0xc0), (int)uVar8 != 0)) &&
               ((uVar8 = FUN_001428fc(uVar8,lVar9 + 0x14,&local_388,4), (int)uVar8 != 0 &&
                (uVar8 = FUN_001428fc(uVar8,lVar9 + 0x18,(long)&local_390 + 4,4), (int)uVar8 != 0)))
               ) {
              iVar6 = FUN_001428fc(uVar8,(undefined4 *)(lVar9 + 0x1c),&local_390,4);
              if (iVar6 != 0) {
                do {
                  uVar5 = _DAT_0022d430;
                  cVar2 = '\x01';
                  bVar3 = (bool)ExclusiveMonitorPass(0x22d430,0x10);
                  if (bVar3) {
                    _DAT_0022d430 = CONCAT31(DAT_0022d430_1,1);
                    cVar2 = ExclusiveMonitorsStatus();
                  }
                } while (cVar2 != '\0');
                if ((uVar5 & 1) == 0) {
                  if (((DAT_0022d508 == 0) || (DAT_0022d500 != DAT_001cfbe8)) ||
                     ((local_388 != DAT_0022d4f8 || (local_390._4_4_ != DAT_0022d4fc)))) {
                    _DAT_0022d430 = 0;
                  }
                  else {
                    iVar6 = strcmp(acStack_37c,&DAT_0022d438);
                    _DAT_0022d430 = 0;
                    if (iVar6 == 0) {
                      *(undefined4 *)(lVar9 + 0x1c) = 0xfffffffe;
                    }
                  }
                }
              }
            }
            break;
          }
          lVar10 = lVar10 + 1;
          ppuVar11 = ppuVar11 + 3;
        } while (lVar10 != 0x37);
      }
      local_384 = 0;
      local_380 = 0;
      local_1fc[0] = '\0';
      if (((((DAT_0020af9c >> 3 & 1) != 0) && ((((uint)DAT_0020afa0 ^ 0xffffffff) & 3) == 0)) &&
          (DAT_0020afa0._4_1_ != '\0')) && ((DAT_0020af50 != '\0' && ((int)DAT_001dfff0 != 0)))) {
        do {
          uVar5 = _DAT_001e12e8;
          cVar2 = '\x01';
          bVar3 = (bool)ExclusiveMonitorPass(0x1e12e8,0x10);
          if (bVar3) {
            _DAT_001e12e8 = CONCAT31(DAT_001e12e8_1,1);
            cVar2 = ExclusiveMonitorsStatus();
          }
        } while (cVar2 != '\0');
        if ((uVar5 & 1) == 0) {
          memcpy(local_1fc,&DAT_001e1311,0x191);
          _DAT_001e12e8 = 0;
          if (((local_1fc[0] != '\0') &&
              (uVar8 = FUN_0014a6ac(lVar9,acStack_2bc,0xc0), (int)uVar8 != 0)) &&
             ((uVar8 = FUN_001428fc(uVar8,lVar9 + 0x14,&local_380,4), (int)uVar8 != 0 &&
              (iVar6 = FUN_001428fc(uVar8,lVar9 + 0x18,&local_384,4), iVar6 != 0)))) {
            do {
              uVar5 = _DAT_0022d430;
              cVar2 = '\x01';
              bVar3 = (bool)ExclusiveMonitorPass(0x22d430,0x10);
              if (bVar3) {
                _DAT_0022d430 = CONCAT31(DAT_0022d430_1,1);
                cVar2 = ExclusiveMonitorsStatus();
              }
            } while (cVar2 != '\0');
            if ((uVar5 & 1) == 0) {
              if ((((DAT_0022d508 == 0) || (DAT_0022d500 != DAT_001cfbe8)) ||
                  (local_380 != DAT_0022d4f8)) || (local_384 != DAT_0022d4fc)) {
                _DAT_0022d430 = 0;
              }
              else {
                iVar6 = strcmp(acStack_2bc,&DAT_0022d438);
                _DAT_0022d430 = 0;
                if (iVar6 == 0) {
                  (*(code *)(DAT_001e0978 + 0x66ad48))(lVar9);
                  (*(code *)(DAT_001e0978 + 0x66ae58))(lVar9,local_1fc);
                }
              }
            }
          }
        }
      }
    }
  }
  *puVar7 = uVar1;
  if (*(long *)(lVar4 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

/* ===== FUN_0017b058 @ 0017b058 [libNexusEvasionRuntime69252.so] ===== */

void FUN_0017b058(long param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  long lVar4;
  undefined **ppuVar5;
  
  puVar3 = (undefined4 *)__errno();
  uVar1 = *puVar3;
  if (((param_1 != 0) && (((uint)DAT_0020afa0 >> 2 & 1) != 0)) && ((int)DAT_001dfff0 != 0)) {
    lVar4 = 0;
    ppuVar5 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar2 = strcmp("HideBattlingStatusFromOthers",*ppuVar5);
      if (iVar2 == 0) {
        if (((-1 < (int)lVar4) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar4 * 4) != 0)) &&
           (*(int *)(param_1 + 8) == 1)) {
          *(undefined8 *)(param_1 + 8) = 3;
        }
        break;
      }
      lVar4 = lVar4 + 1;
      ppuVar5 = ppuVar5 + 3;
    } while (lVar4 != 0x37);
  }
  *puVar3 = uVar1;
  return;
}

/* ===== FUN_0017b11c @ 0017b11c [libNexusEvasionRuntime69252.so] ===== */

void FUN_0017b11c(long param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  long lVar4;
  undefined **ppuVar5;
  
  puVar3 = (undefined4 *)__errno();
  uVar1 = *puVar3;
  if ((((param_1 != 0) && (*(long *)(param_1 + 0xb8) != 0)) && (*(long *)(param_1 + 0xd0) != 0)) &&
     ((((uint)DAT_0020afa0 >> 3 & 1) != 0 && ((int)DAT_001dfff0 != 0)))) {
    lVar4 = 0;
    ppuVar5 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar2 = strcmp("InstantStarrDropOpening",*ppuVar5);
      if (iVar2 == 0) {
        if (((-1 < (int)lVar4) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar4 * 4) != 0)) &&
           (*(int *)(param_1 + 0x40) == 4)) {
          *(undefined8 *)(param_1 + 0x40) = 0;
        }
        break;
      }
      lVar4 = lVar4 + 1;
      ppuVar5 = ppuVar5 + 3;
    } while (lVar4 != 0x37);
  }
  *puVar3 = uVar1;
  return;
}

/* ===== FUN_0017b1ec @ 0017b1ec [libNexusEvasionRuntime69252.so] ===== */

void FUN_0017b1ec(long param_1)

{
  undefined4 uVar1;
  long lVar2;
  int iVar3;
  undefined4 *puVar4;
  undefined8 uVar5;
  long lVar6;
  long lVar7;
  long lVar8;
  long lVar9;
  undefined **ppuVar10;
  undefined1 local_74 [4];
  ulong local_70;
  ulong local_68;
  ulong local_60;
  long local_58;
  
  lVar2 = tpidr_el0;
  local_58 = *(long *)(lVar2 + 0x28);
  puVar4 = (undefined4 *)__errno();
  uVar1 = *puVar4;
  if (param_1 != 0) {
    lVar8 = *(long *)(param_1 + 0x98);
    local_60 = 0;
    local_68 = 0;
    if (((lVar8 != 0) && (((uint)DAT_0020afa0 >> 4 & 1) != 0)) && ((int)DAT_001dfff0 != 0)) {
      lVar9 = 0;
      ppuVar10 = &PTR_s_DisablePinAnimation_001c2838;
      do {
        iVar3 = strcmp("FriendListOptimization",*ppuVar10);
        if (iVar3 == 0) {
          if (((((-1 < (int)lVar9) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar9 * 4) != 0))
               && ((uVar5 = FUN_001428fc(0,lVar8,&local_68,8), (int)uVar5 != 0 &&
                   ((0x11fff < local_68 + 0x2000 && ((local_68 & 7) == 0)))))) &&
              (local_68 == DAT_001e0978 + 0x11e28d8U)) &&
             (((iVar3 = FUN_001428fc(uVar5,lVar8 + 0x80,&local_60,8), iVar3 != 0 &&
               (0x11fff < local_60 + 0x2000)) && ((local_60 & 7) == 0)))) {
            lVar9 = (*(code *)(DAT_001e0978 + 0x5d8874))(local_60,"player_icon");
            lVar6 = (*(code *)(DAT_001e0978 + 0x5d8874))(local_60,"rank_icon");
            lVar7 = (*(code *)(DAT_001e0978 + 0x5d8ad4))(local_60,"friend_picture");
            local_70 = 0;
            local_74[0] = 0;
            lVar8 = lVar7;
            if (((lVar9 != 0) &&
                (lVar8 = FUN_001428fc(lVar7,lVar9 + 0x38,&local_70,8), (int)lVar8 != 0)) &&
               ((local_70 - 0x10000 < 0xfffffffffffee000 &&
                (((local_70 & 7) == 0 && (local_70 == local_60)))))) {
              lVar8 = FUN_001428fc(lVar8,(undefined1 *)(lVar9 + 8),local_74,1);
              if ((int)lVar8 != 0) {
                local_74[0] = 0;
                *(undefined1 *)(lVar9 + 8) = 0;
              }
            }
            if ((((lVar6 != 0) &&
                 (lVar8 = FUN_001428fc(lVar8,lVar6 + 0x38,&local_70,8), (int)lVar8 != 0)) &&
                (local_70 - 0x10000 < 0xfffffffffffee000)) &&
               (((local_70 & 7) == 0 && (local_70 == local_60)))) {
              lVar8 = (*(code *)(DAT_001e0978 + 0x59567c))(lVar6);
            }
            if (((lVar7 != 0) && (iVar3 = FUN_001428fc(lVar8,lVar7 + 0x38,&local_70,8), iVar3 != 0))
               && ((local_70 - 0x10000 < 0xfffffffffffee000 &&
                   (((local_70 & 7) == 0 && (local_70 == local_60)))))) {
              (*(code *)(DAT_001e0978 + 0x59567c))(lVar7);
            }
          }
          break;
        }
        lVar9 = lVar9 + 1;
        ppuVar10 = ppuVar10 + 3;
      } while (lVar9 != 0x37);
    }
  }
  *puVar4 = uVar1;
  if (*(long *)(lVar2 + 0x28) != local_58) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

/* ===== FUN_0017b4c0 @ 0017b4c0 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined * FUN_0017b4c0(long param_1)

{
  undefined4 uVar1;
  byte bVar2;
  char cVar3;
  bool bVar4;
  long lVar5;
  uint uVar6;
  uint uVar7;
  int iVar8;
  undefined4 *puVar9;
  undefined *puVar10;
  size_t sVar11;
  undefined8 uVar12;
  ulong uVar13;
  byte *pbVar14;
  char *pcVar15;
  long lVar16;
  undefined *puVar17;
  long lVar18;
  undefined **ppuVar19;
  long *plVar20;
  undefined4 local_140;
  char local_130;
  undefined2 uStack_12f;
  undefined5 uStack_12d;
  short local_128;
  undefined1 uStack_126;
  undefined5 local_125;
  undefined3 uStack_120;
  long local_70;
  
  lVar5 = tpidr_el0;
  local_70 = *(long *)(lVar5 + 0x28);
  puVar9 = (undefined4 *)__errno();
  uVar1 = *puVar9;
  lVar16 = param_1;
  if ((((int)DAT_001dfff0 != 0) && (((_DAT_0020af9c ^ 0xffffffff) & 7) == 0)) &&
     (((uint)DAT_0020afa0 >> 5 & 1) != 0)) {
    lVar18 = 0;
    ppuVar19 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar8 = strcmp("ShowSkinNamesInProfile",*ppuVar19);
      if (iVar8 == 0) {
        if (((-1 < (int)lVar18) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar18 * 4) != 0)) &&
           (iVar8 = FUN_0014a6ac(param_1,&local_130,0x60), iVar8 != 0)) goto LAB_0017b5a0;
        break;
      }
      lVar18 = lVar18 + 1;
      ppuVar19 = ppuVar19 + 3;
    } while (lVar18 != 0x37);
  }
  goto LAB_0017b600;
LAB_0017b5a0:
  do {
    uVar6 = _DAT_001e12e8;
    cVar3 = '\x01';
    bVar4 = (bool)ExclusiveMonitorPass(0x1e12e8,0x10);
    if (bVar4) {
      _DAT_001e12e8 = CONCAT31(DAT_001e12e8_1,1);
      cVar3 = ExclusiveMonitorsStatus();
    }
  } while (cVar3 != '\0');
  if ((uVar6 & 1) == 0) {
    if (((DAT_0022b560 != 0) && (DAT_001e14bc != 0)) &&
       ((DAT_001e14e0 == DAT_0022b558 && (uVar13 = (ulong)DAT_001e14d0, DAT_001e14d0 != 0)))) {
      pcVar15 = &DAT_001fca78;
      plVar20 = &DAT_00208a78;
      do {
        lVar16 = *plVar20;
        if ((lVar16 != 0) && (iVar8 = strcmp(&local_130,pcVar15), iVar8 == 0)) break;
        uVar13 = uVar13 - 1;
        pcVar15 = pcVar15 + 0x60;
        plVar20 = plVar20 + 1;
        lVar16 = param_1;
      } while (uVar13 != 0);
    }
    _DAT_001e12e8 = 0;
  }
LAB_0017b600:
  *puVar9 = uVar1;
  puVar10 = (undefined *)(*DAT_0022d428)(lVar16);
  uVar1 = *puVar9;
  if (((((int)_DAT_0020b040 != 0) && ((int)DAT_001dfff0 != 0)) &&
      (iVar8 = FUN_0014a6ac(param_1,&local_130,0xc0), uVar6 = DAT_0022d514, iVar8 != 0)) &&
     (local_130 != '\0')) {
    do {
      uVar7 = _DAT_0022d510;
      cVar3 = '\x01';
      bVar4 = (bool)ExclusiveMonitorPass(0x22d510,0x10);
      if (bVar4) {
        _DAT_0022d510 = CONCAT31(DAT_0022d510_1,1);
        cVar3 = ExclusiveMonitorsStatus();
      }
    } while (cVar3 != '\0');
    if ((uVar7 & 1) == 0) {
      uVar13 = (ulong)DAT_0022d514;
      if (DAT_0022d514 == 0) {
LAB_0017b6c4:
        iVar8 = FUN_0013c82c();
        if (iVar8 != 0) {
          uVar13 = (ulong)DAT_0022d514;
          sVar11 = strlen(&local_130);
          memcpy(&DAT_0022d518 + uVar13 * 0xd0,&local_130,sVar11 + 1);
          puVar17 = (undefined *)(uVar13 * 0xd0 + 0x22d5d8);
          (*(code *)(DAT_001e0978 + 0x66ae58))(puVar17,&local_130);
          DAT_0022d514 = DAT_0022d514 + 1;
LAB_0017b724:
          puVar10 = puVar17;
          _DAT_0022d510 = 0;
          goto LAB_0017b9ec;
        }
      }
      else {
        pcVar15 = &DAT_0022d518;
        puVar17 = (undefined *)0x22d5d8;
        do {
          iVar8 = strcmp(pcVar15,&local_130);
          if (iVar8 == 0) goto LAB_0017b724;
          puVar17 = puVar17 + 0xd0;
          uVar13 = uVar13 - 1;
          pcVar15 = pcVar15 + 0xd0;
        } while (uVar13 != 0);
        if (uVar6 < 0x1000) goto LAB_0017b6c4;
      }
      _DAT_0022d510 = 0;
    }
  }
  if ((((uint)DAT_0020afa0 >> 5 & 1) != 0) && ((int)DAT_001dfff0 != 0)) {
    lVar18 = 0;
    ppuVar19 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar8 = strcmp("LegacyNames",*ppuVar19);
      if (iVar8 == 0) {
        if ((((int)lVar18 < 0) || ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar18 * 4) == 0)) ||
           (iVar8 = FUN_0014a6ac(lVar16,&local_130,0x40), iVar8 == 0)) break;
        if (CONCAT53(uStack_12d,CONCAT21(uStack_12f,local_130)) == 0x444e454d5f444954 &&
            CONCAT17(uStack_126,CONCAT25(local_128,uStack_12d)) == 0x5245444e454d5f) {
          lVar16 = 0;
        }
        else if ((CONCAT53(uStack_12d,CONCAT21(uStack_12f,local_130)) == 0x434952545f444954 &&
                 CONCAT53(local_125,CONCAT12(uStack_126,local_128)) == 0x55445f544f48534b) &&
                 CONCAT35(uStack_120,local_125) == 0x454455445f544f) {
          lVar16 = 1;
        }
        else {
          if (CONCAT53(uStack_12d,CONCAT21(uStack_12f,local_130)) != 0x464655525f444954 ||
              local_128 != 0x53) break;
          lVar16 = 2;
        }
        uVar12 = (*(code *)(DAT_001e0978 + 0xd173ac))();
        iVar8 = FUN_0014a6ac(uVar12,&local_140,0x10);
        if (iVar8 == 0) {
          lVar18 = 0;
        }
        else {
          if ((byte)local_140 != 0) {
            pbVar14 = (byte *)((ulong)&local_140 | 1);
            bVar2 = (byte)local_140;
            do {
              if (bVar2 - 0x41 < 0x1a) {
                pbVar14[-1] = bVar2 + 0x20;
              }
              bVar2 = *pbVar14;
              pbVar14 = pbVar14 + 1;
            } while (bVar2 != 0);
          }
          if ((short)local_140 == 0x7572 && local_140._2_1_ == '\0') {
            lVar18 = 1;
          }
          else if ((short)local_140 == 0x6e63 && local_140._2_1_ == '\0') {
            lVar18 = 2;
          }
          else if (local_140 == 0x746e63) {
            lVar18 = 3;
          }
          else if ((short)local_140 == 0x7274 && local_140._2_1_ == '\0') {
            lVar18 = 4;
          }
          else if ((short)local_140 == 0x6c70 && local_140._2_1_ == '\0') {
            lVar18 = 5;
          }
          else if ((short)local_140 == 0x7469 && local_140._2_1_ == '\0') {
            lVar18 = 6;
          }
          else {
            lVar18 = 0;
            if ((short)local_140 == 0x6564 && local_140._2_1_ == '\0') {
              lVar18 = 7;
            }
          }
        }
        puVar10 = &DAT_0022d2a8 + lVar16 * 0x10 + lVar18 * 0x30;
        break;
      }
      lVar18 = lVar18 + 1;
      ppuVar19 = ppuVar19 + 3;
    } while (lVar18 != 0x37);
  }
LAB_0017b9ec:
  *puVar9 = uVar1;
  if (*(long *)(lVar5 + 0x28) == local_70) {
    return puVar10;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0017ba28 @ 0017ba28 [libNexusEvasionRuntime69252.so] ===== */

void FUN_0017ba28(long param_1)

{
  uint uVar1;
  undefined4 uVar2;
  char cVar3;
  long lVar4;
  int iVar5;
  undefined4 *puVar6;
  long lVar7;
  uint uVar8;
  ulong uVar9;
  uint *puVar10;
  bool bVar11;
  long lVar12;
  undefined **ppuVar13;
  long local_a4;
  long local_9c;
  int local_94;
  undefined8 local_90;
  long lStack_88;
  int local_80;
  uint local_7c;
  uint uStack_78;
  long lStack_74;
  int local_6c;
  long local_68;
  
  lVar4 = tpidr_el0;
  local_68 = *(long *)(lVar4 + 0x28);
  puVar6 = (undefined4 *)__errno();
  uVar2 = *puVar6;
  if ((int)DAT_001e12c0 == 0) goto LAB_0017bd00;
  lVar7 = param_1 + 0x90;
  iVar5 = FUN_001428fc(puVar6,lVar7,&local_7c,0x14);
  if (iVar5 == 0) goto LAB_0017bd00;
  uVar1 = uStack_78 - 1000;
  if (uStack_78 - 1000 == 0 || (int)uStack_78 < 1000) {
    uVar1 = uStack_78;
  }
  if (0x497c8 < uStack_78) {
    uVar1 = 0xffffffff;
  }
  if (0x493de < uVar1 - 2) {
    uVar1 = 0xffffffff;
  }
  pthread_mutex_lock((pthread_mutex_t *)&DAT_001e0980);
  if ((local_7c < 1000000) && (DAT_001e09ac < 0x21)) {
    if (DAT_001e09ac == 0) {
      uVar8 = 0;
    }
    else {
      uVar9 = 0;
      puVar10 = &DAT_001e09c0;
      do {
        if (*puVar10 == local_7c) goto LAB_0017bb18;
        uVar9 = uVar9 + 1;
        puVar10 = puVar10 + 0x12;
      } while (DAT_001e09ac != uVar9);
      uVar9 = (ulong)DAT_001e09ac;
LAB_0017bb18:
      uVar8 = (uint)uVar9;
      if (0x1f < uVar8) goto LAB_0017bb94;
    }
    if (uVar8 == DAT_001e09ac) {
      uVar9 = (ulong)DAT_001e09ac;
      lVar12 = uVar9 * 0x48;
      DAT_001e09ac = DAT_001e09ac + 1;
      *(undefined8 *)(&DAT_001e0a00 + lVar12) = 0;
      *(undefined8 *)(lVar12 + 0x1e09f8) = 0;
      *(undefined8 *)(&DAT_001e09f0 + lVar12) = 0;
      *(undefined8 *)(lVar12 + 0x1e09e8) = 0;
      *(undefined8 *)(&DAT_001e09e0 + lVar12) = 0;
      *(undefined8 *)(lVar12 + 0x1e09d8) = 0;
      *(undefined8 *)(&DAT_001e09d0 + lVar12) = 0;
      *(undefined8 *)(&DAT_001e09c8 + lVar12) = 0;
      *(undefined8 *)(&DAT_001e09c0 + uVar9 * 0x12) = 0;
    }
    lVar12 = (ulong)uVar8 * 0x48;
    cVar3 = (&DAT_001e09c8)[lVar12];
    (&DAT_001e09c0)[(ulong)uVar8 * 0x12] = local_7c;
    *(uint *)(&DAT_001e09c4 + lVar12) = uVar1;
    if (cVar3 == '\0') {
      snprintf(&DAT_001e09c8 + lVar12,0x40,"UNKNOWN REGION %d");
    }
    DAT_001e09b0 = DAT_001e09b0 + 1;
  }
LAB_0017bb94:
  pthread_mutex_unlock((pthread_mutex_t *)&DAT_001e0980);
  lVar12 = 0;
  ppuVar13 = &PTR_s_DisablePinAnimation_001c2838;
  do {
    iVar5 = strcmp("BattleServerRegion",*ppuVar13);
    if (iVar5 == 0) {
      if ((-1 < (int)lVar12) &&
         (uVar1 = (int)*(undefined8 *)((long)&DAT_001dfea0 + lVar12 * 4) - 1, uVar1 < 1000000)) {
        bVar11 = true;
        if ((999999 < local_7c) || (0x7ffffc17 < uStack_78)) goto LAB_0017bd04;
        uVar8 = uStack_78;
        if (local_7c != uVar1) {
          uVar8 = uStack_78 + 1000;
        }
        local_90 = CONCAT44(uVar8,local_7c);
        lStack_88 = 0x500000005;
        local_80 = 10;
        lVar12 = FUN_0014bef8(DAT_001cfb4c,&local_90,0x14,lVar7);
        if (((lVar12 == 0x14) && (iVar5 = FUN_001428fc(0x14,lVar7,&local_a4,0x14), iVar5 != 0)) &&
           ((local_a4 == local_90 && local_9c == lStack_88) && local_94 == local_80)) {
          bVar11 = false;
          goto LAB_0017bd04;
        }
        lVar12 = FUN_0014bef8(DAT_001cfb4c,&local_7c,0x14,lVar7);
        if (((lVar12 != 0x14) || (iVar5 = FUN_001428fc(0x14,lVar7,&local_a4,0x14), iVar5 == 0)) ||
           ((local_a4 != CONCAT44(uStack_78,local_7c) || local_9c != lStack_74) ||
            local_94 != local_6c)) {
                    /* WARNING: Subroutine does not return */
          abort();
        }
      }
      break;
    }
    lVar12 = lVar12 + 1;
    ppuVar13 = ppuVar13 + 3;
  } while (lVar12 != 0x37);
LAB_0017bd00:
  bVar11 = true;
LAB_0017bd04:
  *puVar6 = uVar2;
  (*DAT_002fd518)(param_1);
  uVar2 = *puVar6;
  if (!bVar11) {
    lVar7 = FUN_0014bef8(DAT_001cfb4c,&local_7c,0x14,param_1 + 0x90);
    if (((lVar7 != 0x14) || (iVar5 = FUN_001428fc(0x14,param_1 + 0x90,&local_a4,0x14), iVar5 == 0))
       || ((local_a4 != CONCAT44(uStack_78,local_7c) || local_9c != lStack_74) ||
           local_94 != local_6c)) {
                    /* WARNING: Subroutine does not return */
      abort();
    }
  }
  *puVar6 = uVar2;
  if (*(long *)(lVar4 + 0x28) == local_68) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

/* ===== FUN_0017e8b0 @ 0017e8b0 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0017e8b0(long *param_1)

{
  ulong uVar1;
  undefined4 uVar2;
  char cVar3;
  bool bVar4;
  long lVar5;
  uint uVar6;
  uint uVar7;
  undefined4 uVar8;
  int iVar9;
  int iVar10;
  undefined4 *puVar11;
  long lVar12;
  ulong uVar13;
  undefined **ppuVar14;
  int local_7c;
  timespec local_78;
  long local_68;
  
  lVar5 = tpidr_el0;
  local_68 = *(long *)(lVar5 + 0x28);
  puVar11 = (undefined4 *)__errno();
  uVar2 = *puVar11;
  do {
    cVar3 = '\x01';
    bVar4 = (bool)ExclusiveMonitorPass(0x2fd5d4,0x10);
    if (bVar4) {
      cVar3 = ExclusiveMonitorsStatus();
      DAT_002fd5d4 = DAT_002fd5d4 + 1;
    }
  } while (cVar3 != '\0');
  uVar8 = gettid();
  DAT_002fd5d8 = uVar8;
  if ((param_1 != (long *)0x0) && (DAT_002fd5d0 != 0)) {
    lVar12 = 0;
    ppuVar14 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar9 = strcmp("ShowDPS",*ppuVar14);
      if (iVar9 == 0) {
        if ((-1 < (int)lVar12) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar12 * 4) != 0)) {
          iVar10 = clock_gettime(1,&local_78);
          iVar9 = DAT_0020c0c0;
          lVar12 = DAT_0020c0b0;
          if (iVar10 == 0) {
            uVar13 = local_78.tv_sec * 1000 + (ulong)local_78.tv_nsec / 1000000;
          }
          else {
            uVar13 = 0;
          }
          do {
            uVar6 = _DAT_0020b098;
            cVar3 = '\x01';
            bVar4 = (bool)ExclusiveMonitorPass(0x20b098,0x10);
            if (bVar4) {
              _DAT_0020b098 = CONCAT31(DAT_0020b098_1,1);
              cVar3 = ExclusiveMonitorsStatus();
            }
          } while (cVar3 != '\0');
          if ((((((uVar6 & 1) == 0) && (_DAT_0020b098 = 0, DAT_0020c0b0 != 0)) &&
               (DAT_0020c0b8 != 0)) && ((DAT_0020c0b8 <= uVar13 && (uVar13 - DAT_0020c0b8 < 0x1f5)))
              ) && ((((-1 < DAT_0020c0c0 &&
                      ((iVar10 = FUN_0017eb60(*param_1 + 0x10,&local_78,8), iVar10 != 0 &&
                       (0x11fff < local_78.tv_sec + 0x2000U)))) && ((local_78.tv_sec & 7U) == 0)) &&
                    ((((iVar10 = FUN_0017eb60(local_78.tv_sec + 0x3c,&local_7c,4),
                       uVar6 = DAT_0020b0ac, iVar10 != 0 && (-1 < local_7c)) && (local_7c < 10)) &&
                     ((local_7c != iVar9 && (0xfeced2fe < (int)param_1[1] - 0x989681U)))))))) {
            do {
              uVar7 = _DAT_0020b098;
              cVar3 = '\x01';
              bVar4 = (bool)ExclusiveMonitorPass(0x20b098,0x10);
              if (bVar4) {
                _DAT_0020b098 = CONCAT31(DAT_0020b098_1,1);
                cVar3 = ExclusiveMonitorsStatus();
              }
            } while (cVar3 != '\0');
            if ((uVar7 & 1) == 0) {
              if (((DAT_0020c0b0 == lVar12) && (DAT_0020c0c0 == iVar9)) && (DAT_0020b0a0 == lVar12))
              {
                DAT_002fd5e8 = -(int)param_1[1];
                uVar1 = (ulong)DAT_0020b0a8;
                DAT_0020b0a8 = DAT_0020b0a8 + 1 & 0xff;
                (&DAT_0020b0b0)[uVar1 * 2] = uVar13;
                (&DAT_0020b0b8)[uVar1 * 4] = DAT_002fd5e8;
                if (uVar6 < 0x100) {
                  DAT_0020b0ac = uVar6 + 1;
                }
                DAT_002fd5e4 = local_7c;
                DAT_002fd5dc = DAT_002fd5dc + 1;
                DAT_002fd5e0 = uVar8;
              }
              _DAT_0020b098 = 0;
            }
          }
        }
        break;
      }
      lVar12 = lVar12 + 1;
      ppuVar14 = ppuVar14 + 3;
    } while (lVar12 != 0x37);
  }
  *puVar11 = uVar2;
  if (*(long *)(lVar5 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

/* ===== FUN_0017ef80 @ 0017ef80 [libNexusEvasionRuntime69252.so] ===== */

void FUN_0017ef80(long param_1)

{
  undefined4 uVar1;
  long lVar2;
  int iVar3;
  int iVar4;
  undefined4 *puVar5;
  long lVar6;
  long lVar7;
  undefined **ppuVar8;
  timespec local_68;
  long local_58;
  
  lVar2 = tpidr_el0;
  local_58 = *(long *)(lVar2 + 0x28);
  puVar5 = (undefined4 *)__errno();
  uVar1 = *puVar5;
  lVar6 = FUN_00139fcc();
  if ((param_1 != 0) && (DAT_002fd5ec != 0)) {
    lVar7 = 0;
    ppuVar8 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar3 = strcmp("ShowBattleConnectionIndicator",*ppuVar8);
      if (iVar3 == 0) {
        if ((((-1 < (int)lVar7) && ((int)*(undefined8 *)((long)&DAT_001dfea0 + lVar7 * 4) != 0)) &&
            (lVar6 != 0)) && (iVar3 = *(int *)(param_1 + 0xb8), iVar3 - 1U >> 4 < 0x753)) {
          DAT_0020c0c8 = lVar6;
          iVar4 = clock_gettime(1,&local_68);
          DAT_0020c0d8 = iVar3;
          if (iVar4 == 0) {
            DAT_0020c0d0 = local_68.tv_sec * 1000 + (ulong)local_68.tv_nsec / 1000000;
          }
          else {
            DAT_0020c0d0 = 0;
          }
        }
        break;
      }
      lVar7 = lVar7 + 1;
      ppuVar8 = ppuVar8 + 3;
    } while (lVar7 != 0x37);
  }
  *puVar5 = uVar1;
  if (*(long *)(lVar2 + 0x28) != local_58) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

/* ===== FUN_0017f0c0 @ 0017f0c0 [libNexusEvasionRuntime69252.so] ===== */

void FUN_0017f0c0(undefined8 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined8 uVar4;
  long lVar5;
  undefined **ppuVar6;
  
  puVar3 = (undefined4 *)__errno();
  if (DAT_002fd600 != 0 && DAT_0020af50 != '\0') {
    lVar5 = 0;
    uVar1 = *puVar3;
    ppuVar6 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar2 = strcmp("BackgroundMatchmaking",*ppuVar6);
      if (iVar2 == 0) {
        if ((-1 < (int)lVar5) &&
           (uVar4 = *(undefined8 *)((long)&DAT_001dfea0 + lVar5 * 4), *puVar3 = uVar1,
           (int)uVar4 != 0)) {
          return;
        }
        break;
      }
      lVar5 = lVar5 + 1;
      ppuVar6 = ppuVar6 + 3;
    } while (lVar5 != 0x37);
  }
                    /* WARNING: Could not recover jumptable at 0x0017f1a0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*DAT_002fd5f0)(param_1,param_2,param_3);
  return;
}

/* ===== FUN_0017f1a4 @ 0017f1a4 [libNexusEvasionRuntime69252.so] ===== */

void FUN_0017f1a4(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined4 param_4,
                 undefined4 param_5,undefined4 param_6)

{
  code *UNRECOVERED_JUMPTABLE_00;
  undefined4 uVar1;
  uint uVar2;
  int iVar3;
  undefined4 *puVar4;
  undefined **ppuVar5;
  long lVar6;
  
  puVar4 = (undefined4 *)__errno();
  uVar1 = *puVar4;
  if (DAT_002fd600 == 0 || DAT_0020af50 == '\0') {
    uVar2 = 0;
  }
  else {
    lVar6 = 0;
    ppuVar5 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      iVar3 = strcmp("BackgroundMatchmaking",*ppuVar5);
      if (iVar3 == 0) {
        if (-1 < (int)lVar6) {
          iVar3 = (int)*(undefined8 *)((long)&DAT_001dfea0 + lVar6 * 4);
          uVar2 = (uint)(iVar3 != 0);
          if ((iVar3 != 0) && (uVar2 = (uint)(iVar3 != 0), DAT_002172e0 != 0)) {
            DAT_002172e0 = 0;
            UNRECOVERED_JUMPTABLE_00 = (code *)(DAT_001e0978 + 0xf481f4);
            *puVar4 = uVar1;
                    /* WARNING: Could not recover jumptable at 0x0017f2b4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
            (*UNRECOVERED_JUMPTABLE_00)(param_1);
            return;
          }
          goto LAB_0017f2c4;
        }
        break;
      }
      lVar6 = lVar6 + 1;
      ppuVar5 = ppuVar5 + 3;
    } while (lVar6 != 0x37);
    uVar2 = 0;
  }
LAB_0017f2c4:
  DAT_002172e0 = uVar2;
  UNRECOVERED_JUMPTABLE_00 = DAT_002fd5f8;
  *puVar4 = uVar1;
                    /* WARNING: Could not recover jumptable at 0x0017f308. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*UNRECOVERED_JUMPTABLE_00)(param_1,param_2,param_3,param_4,param_5,param_6);
  return;
}

/* ===== FUN_0017f330 @ 0017f330 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0017f330(long *param_1)

{
  undefined4 uVar1;
  long lVar2;
  int iVar3;
  bool bVar4;
  int iVar5;
  uint uVar6;
  undefined4 *puVar7;
  ulong uVar8;
  long lVar9;
  undefined **ppuVar10;
  long lVar11;
  long lVar12;
  undefined1 local_78 [4];
  char local_74 [4];
  ulong local_70;
  long local_68;
  
  lVar2 = tpidr_el0;
  local_68 = *(long *)(lVar2 + 0x28);
  puVar7 = (undefined4 *)__errno();
  iVar3 = DAT_00209cd8;
  uVar1 = *puVar7;
  if ((((param_1 != (long *)0x0) && (DAT_002fd604 != 0)) && (DAT_00209cd8 != 0)) &&
     (iVar5 = gettid(), iVar3 == iVar5)) {
    lVar12 = *param_1;
    if (DAT_002fd608 != lVar12) {
      _DAT_002fd630 = 0;
      uRam00000000002fd628 = 0;
      _DAT_002fd620 = 0;
      uRam00000000002fd618 = 0;
      DAT_002fd610 = 0;
      DAT_002fd608 = lVar12;
    }
    lVar9 = 0;
    ppuVar10 = &PTR_s_DisablePinAnimation_001c2838;
    do {
      uVar6 = strcmp("HideBattleBlackBars",*ppuVar10);
      uVar8 = (ulong)uVar6;
      if (uVar6 == 0) {
        if (-1 < (int)lVar9) {
          bVar4 = (int)*(undefined8 *)((long)&DAT_001dfea0 + lVar9 * 4) == 0;
          goto LAB_0017f420;
        }
        break;
      }
      lVar9 = lVar9 + 1;
      ppuVar10 = ppuVar10 + 3;
    } while (lVar9 != 0x37);
    bVar4 = true;
LAB_0017f420:
    lVar11 = 0;
    lVar9 = 0x28;
    do {
      local_70 = 0;
      uVar8 = FUN_001428fc(uVar8,lVar12 + 0x120 + lVar11,&local_70,8);
      if ((((int)uVar8 != 0) && (0x11fff < local_70 + 0x2000)) &&
         (((local_70 & 7) == 0 &&
          (uVar8 = FUN_001428fc(uVar8,local_70 + 8,local_74,1), (int)uVar8 != 0)))) {
        if (*(ulong *)((long)&DAT_002fd610 + lVar11) != local_70) {
          uVar6 = DAT_002fd634;
          *(ulong *)((long)&DAT_002fd610 + lVar11) = local_70;
          _DAT_002fd630 =
               CONCAT44(uVar6 & (1 << (ulong)((int)lVar9 - 0x28U & 0x1f) ^ 0xffffffffU),
                        _DAT_002fd630);
        }
        uVar6 = 1 << (ulong)((int)lVar9 - 0x28U & 0x1f);
        if (bVar4) {
          if ((DAT_002fd634 & uVar6) != 0) {
            if (local_74[0] == '\0') {
              uVar8 = FUN_0014bef8(DAT_001cfb4c,(long)&DAT_002fd608 + lVar9,1,local_70 + 8);
            }
            _DAT_002fd630 = CONCAT44(DAT_002fd634 & (uVar6 ^ 0xffffffff),_DAT_002fd630);
          }
        }
        else {
          if ((DAT_002fd634 & uVar6) == 0) {
            *(char *)((long)&DAT_002fd608 + lVar9) = local_74[0];
            _DAT_002fd630 = CONCAT44(DAT_002fd634 | uVar6,_DAT_002fd630);
          }
          local_78[0] = 0;
          uVar8 = FUN_0014bef8(DAT_001cfb4c,local_78,1,local_70 + 8);
        }
      }
      lVar9 = lVar9 + 1;
      lVar11 = lVar11 + 8;
    } while (lVar11 != 0x20);
  }
  *puVar7 = uVar1;
  if (*(long *)(lVar2 + 0x28) != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

/* ===== FUN_0017f95c @ 0017f95c [libNexusEvasionRuntime69252.so] ===== */

bool FUN_0017f95c(char *param_1)

{
  int iVar1;
  
  if (param_1 == (char *)0x0) {
    return false;
  }
  if (DAT_0020afa0._4_1_ == '\0') {
    return false;
  }
  iVar1 = strcmp(param_1,"ShowCharactersInNames");
  if ((iVar1 != 0) && (iVar1 = strcmp(param_1,"ShamePlayersWithThumbsdownPin"), iVar1 != 0)) {
    if (DAT_0020af50 != '\x01') {
      return false;
    }
    iVar1 = strcmp(param_1,"ShowFriendsInBattle");
    if (iVar1 != 0) {
      iVar1 = strcmp(param_1,"ShowAllianceMembersInBattle");
      return iVar1 == 0;
    }
  }
  return true;
}

/* ===== FUN_0017fa50 @ 0017fa50 [libNexusEvasionRuntime69252.so] ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulong FUN_0017fa50(char *param_1)

{
  bool bVar1;
  int iVar2;
  
  if (param_1 == (char *)0x0) {
    return 0;
  }
  iVar2 = strcmp(param_1,"TeammateHPIndicator");
  if (iVar2 == 0) {
    return (ulong)DAT_0022b5b0;
  }
  iVar2 = strcmp(param_1,"AllyRespawnTimer");
  if (iVar2 == 0) {
    if (DAT_00220780 == 0) {
      return 0;
    }
    bVar1 = (int)_DAT_00220784 == 1;
  }
  else {
    if ((int)_DAT_00220784 != 1) {
      return 0;
    }
    iVar2 = strcmp(param_1,"TileGrid");
    if ((((iVar2 == 0) || (iVar2 = strcmp(param_1,"ShowOwnPlayerCoordinates"), iVar2 == 0)) ||
        (iVar2 = strcmp(param_1,"ShowFPSCounter"), iVar2 == 0)) ||
       (iVar2 = strcmp(param_1,"SmoothHud"), iVar2 == 0)) {
      return 1;
    }
    iVar2 = strcmp(param_1,"SmoothHudGraph");
    bVar1 = iVar2 == 0;
  }
  return (ulong)bVar1;
}

