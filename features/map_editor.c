/*
 * Map Editor unlock bridge — Feature
 * Decompiled with Ghidra 11.3.2 (arm64 pseudocode) from: libNexusUI69252.so
 * Related menu entries (from embedded nexus-overlay-wire/v1):
 *   - editor.OPEN_MAP_EDITOR_POPUP "Open Map Editor" [free]
 *   - editor.TOGGLE_GRID "Grid" [free]
 *   - editor.PLACEMENT_NEXT "Mirror Next" [free]
 *   - editor.PLACEMENT_PREVIOUS "Mirror Previous" [free]
 *   - editor.PLACEMENT_SET "Placement Mode" [free]
 *   - editor.UNDO "Undo" [free]
 *   - ... +14 more (full table below / docs/debug_menu.md)
 * Bridge that unlocks the stock game's internal map editor ("Map editor" popup, 20 commands).
 * Command table (actionId = slot + base), from the embedded wire:
 *   167968  editor.OPEN_MAP_EDITOR_POPUP               "Open Map Editor" [free]
 *   167969  editor.TOGGLE_GRID                         "Grid" [free]
 *   167970  editor.PLACEMENT_NEXT                      "Mirror Next" [free]
 *   167971  editor.PLACEMENT_PREVIOUS                  "Mirror Previous" [free]
 *   167972  editor.PLACEMENT_SET                       "Placement Mode" [free]
 *   167973  editor.UNDO                                "Undo" [free]
 *   167974  editor.REDO                                "Redo" [free]
 *   167975  editor.SELECT_ERASER                       "Eraser" [free]
 *   167976  editor.FILL_ALL                            "Fill All" [free]
 *   167977  editor.FILL_REGION                         "Fill Region" [free]
 *   167978  editor.ERASE_ALL                           "Erase All" [free]
 *   167979  editor.SAVE                                "Save Map" [free]
 *   167980  editor.CLEAR_ALL                           "Clear Map" [free]
 *   167981  editor.REFRESH_TILE_COUNTS                 "Refresh Counts" [free]
 *   167982  editor.BYPASS_SAVE_VALIDATION              "Save Validation Bypass" [free]
 *   167983  editor.BYPASS_PLACEMENT_ZONES              "Placement Zones Bypass" [free]
 *   167984  editor.UNLOCK_FULL_PALETTE                 "Full Palette" [free]
 *   167985  editor.MAP_MODIFIERS                       "Map Modifiers" [free]
 *   167986  editor.CANCEL_FILL                         "Cancel Fill" [free]
 *   167987  editor.GO_HOME                             "Go Home" [free]
 * Notes:
 *   - nexus_menu_editor_action: dispatches actionId in [0x28FC0,0x290D4) (167968+i) — incl.
 *   - BYPASS_SAVE_VALIDATION, BYPASS_PLACEMENT_ZONES, UNLOCK_FULL_PALETTE, MAP_MODIFIERS.
 *   - nexus_menu_editor_open/pump/scroll_revision: editor lifecycle, pending-request pump,
 *   - scroll revision counter for the tile palette.
 *   - Cross-file references remain in ui/menu_engine.c.
 */

/* ===== nexus_menu_editor_scroll_revision @ 001888d8 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 nexus_menu_editor_scroll_revision(void)

{
  undefined8 uVar1;
  
  uVar1 = 0;
  if ((DAT_0028469c != '\0') && (DAT_0028469d == 'e')) {
    uVar1 = ram0x0022d828;
  }
  return uVar1;
}


/* ===== nexus_menu_editor_open @ 00188908 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 nexus_menu_editor_open(int param_1)

{
  ulong uVar1;
  undefined8 uVar2;
  
  uVar1 = FUN_00193f80(1,&DAT_00284688);
  if ((uVar1 & 1) == 0) {
    uVar2 = 4;
    if (((0 < param_1) && (DAT_00284694 == param_1)) && (DAT_0028469c != '\0')) {
      if (DAT_0028469d != 0x65) {
        DAT_0028cc28 = (uint)DAT_0028469d;
      }
      _DAT_0028cc2c = 0;
      DAT_0028cc3c = 0;
      DAT_0028cc48 = 0;
      DAT_0028cc40 = DAT_0028cc40 + 1;
      DAT_0028cc64 = 0;
      DAT_0028469f = 0;
      uVar2 = 1;
      DAT_002846a0 = DAT_002846a0 + 1;
      DAT_0028469d = 0x65;
      FUN_00194010(1,0x22d828);
    }
    DAT_00284688 = 0;
  }
  else {
    uVar2 = 3;
  }
  return uVar2;
}


/* ===== nexus_menu_editor_action @ 001889ec ===== */

undefined8 nexus_menu_editor_action(int param_1,int param_2)

{
  uint uVar1;
  uint uVar2;
  ulong uVar3;
  char *pcVar4;
  
  if (param_1 - 0x29200U < 0xfffffe00) {
    return 4;
  }
  uVar3 = FUN_00193f80(1,&DAT_00284688);
  if ((uVar3 & 1) != 0) {
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
  if (DAT_0028469d != 'e') {
    DAT_00284688 = 0;
    return 4;
  }
  if (param_1 == 0x29000) {
    DAT_0028cc40 = DAT_0028cc40 + 1;
    if (DAT_0028cc30 - 3U < 2) {
      DAT_0028cc3c = 0;
      DAT_0028cc64 = 0;
    }
    else {
      DAT_0028469d = (char)DAT_0028cc28;
    }
    DAT_0028469f = 0;
    DAT_0028cc30 = 0;
    DAT_002846a0 = DAT_002846a0 + 1;
    FUN_00194010(1,0x22d828);
    DAT_00284688 = 0;
    return 1;
  }
  if ((int)DAT_0028dce8 == 0) {
    if (param_1 == 0x29001) {
      if (DAT_0028cc30 != 4) {
        DAT_00284688 = 0;
        return 4;
      }
      if ((DAT_0028cc3c & 1) == 0) {
        DAT_00284688 = 0;
        return 4;
      }
      DAT_0028cc30 = 5;
      FUN_00183ac8(&DAT_0028cc64,0x80,0x80,"APPLYING...");
    }
    else {
      if (DAT_0028cc30 != 0) {
        DAT_00284688 = 0;
        return 2;
      }
      uVar1 = param_1 - 0x29020;
      if (0x13 < uVar1) {
        DAT_00284688 = 0;
        return 4;
      }
      if (DAT_0028cc2c == 0) {
        DAT_00284688 = 0;
        return 4;
      }
      if ((DAT_0028cc3c >> (ulong)(uVar1 & 0x1f) & 1) == 0) {
        DAT_00284688 = 0;
        return 4;
      }
      uVar2 = *(uint *)(&DAT_0019dab0 + (ulong)uVar1 * 0x28);
      if (0x13 < uVar2) {
        DAT_00284688 = 0;
        return 0;
      }
      if (DAT_0028cbf8 <= uVar2) {
        DAT_00284688 = 0;
        return 0;
      }
      if ((DAT_0028cbfc >> (ulong)(uVar2 & 0x1f) & 1) == 0) {
        DAT_00284688 = 0;
        return 0;
      }
      DAT_0028cc30 = 1;
      DAT_0028cc5c = 0;
      DAT_0028cc54 = 0;
      DAT_0028cc38 = DAT_0028cbf0;
      if ((param_1 != 0x29024) && (param_1 != 0x29029)) {
        if ((uVar2 - 8 < 5) && ((0x17U >> (ulong)(uVar2 - 8 & 0x1f) & 1) != 0)) {
          DAT_0028cc30 = 3;
        }
        else {
          DAT_0028cc30 = 3;
          if (uVar2 != 0x13) {
            DAT_0028cc30 = 5;
          }
        }
      }
      pcVar4 = "ENTER A VALUE";
      if ((param_1 != 0x29024) && (param_1 != 0x29029)) {
        if ((uVar2 - 8 < 5) && ((0x17U >> (ulong)(uVar2 - 8 & 0x1f) & 1) != 0)) {
          pcVar4 = "REVIEW ACTION";
        }
        else {
          pcVar4 = "REVIEW ACTION";
          if (uVar2 != 0x13) {
            pcVar4 = "APPLYING...";
          }
        }
      }
      DAT_0028cc34 = uVar1;
      FUN_00183ac8(&DAT_0028cc64,0x80,0x80,&DAT_001343dc,pcVar4);
    }
    DAT_002846a0 = DAT_002846a0 + 1;
  }
  DAT_00284688 = 0;
  return 2;
}


/* ===== nexus_menu_editor_pump @ 00188d04 ===== */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void nexus_menu_editor_pump(int param_1,ulong param_2)

{
  char *pcVar1;
  uint uVar2;
  long lVar3;
  undefined1 auVar4 [16];
  undefined1 auVar5 [16];
  bool bVar6;
  uint uVar7;
  long lVar8;
  int iVar9;
  undefined1 uVar10;
  bool bVar11;
  int iVar12;
  int iVar13;
  uint uVar14;
  int iVar15;
  ulong uVar16;
  ulong uVar17;
  void *pvVar18;
  char *pcVar19;
  undefined4 uVar20;
  uint uVar21;
  ushort uVar22;
  undefined1 auVar23 [16];
  uint uStack_4fc;
  int local_4e4;
  int local_4e0;
  int iStack_4dc;
  int iStack_4d8;
  uint uStack_4d4;
  undefined8 local_4d0;
  uint local_4c8;
  undefined4 uStack_4c4;
  undefined4 local_4c0;
  undefined4 uStack_4bc;
  undefined4 uStack_4b8;
  uint uStack_4b4;
  undefined8 local_4b0;
  undefined8 local_4a8;
  undefined1 auStack_494 [1028];
  undefined4 local_90;
  undefined4 uStack_8c;
  undefined4 uStack_88;
  undefined4 uStack_84;
  long local_78;
  
  lVar3 = tpidr_el0;
  local_78 = *(long *)(lVar3 + 0x28);
  uVar16 = FUN_00193f80(1,&DAT_00284688);
  if ((uVar16 & 1) != 0) {
    uVar10 = 3;
    goto LAB_00188e5c;
  }
  if ((param_1 < 1) || (DAT_00284694 != param_1)) {
    uVar10 = 4;
    DAT_00284688 = 0;
    goto LAB_00188e5c;
  }
  iVar12 = FUN_00193fe0(1,&DAT_0028dce8);
  iVar9 = DAT_0028cc50;
  lVar8 = DAT_0028cc40;
  iVar15 = DAT_0028cc38;
  uVar2 = DAT_0028cc34;
  if (iVar12 != 0) {
    uVar10 = 3;
    DAT_00284688 = 0;
    goto LAB_00188e5c;
  }
  uVar7 = DAT_0028cc30;
  uVar16 = (ulong)DAT_0028cc34;
  uStack_88 = (undefined4)DAT_0028cc5c;
  uStack_84 = (undefined4)((ulong)DAT_0028cc5c >> 0x20);
  local_90 = (undefined4)DAT_0028cc54;
  uStack_8c = (undefined4)((ulong)DAT_0028cc54 >> 0x20);
  if ((DAT_0028469c == '\0') || (DAT_0028469d != 'e')) {
    DAT_0028cc40 = DAT_0028cc40 + 1;
    DAT_0028cc50 = 0;
    _DAT_0028cc2c = 0;
    DAT_0028cc3c = 0;
    DAT_00284688 = 0;
    FUN_00191dd0();
    if (iVar9 != 0) goto LAB_00188e4c;
LAB_00188e54:
    uVar10 = 1;
  }
  else {
    bVar11 = true;
    if (((DAT_0028cc30 == 0) && (DAT_0028cc48 != 0)) && (DAT_0028cc48 <= param_2)) {
      bVar11 = 0xf9 < param_2 - DAT_0028cc48;
    }
    if ((DAT_0028cc50 != 0) && (DAT_0028cc30 == 0)) {
      DAT_0028cc50 = 0;
      _DAT_0028cc2c = _DAT_0028cc2c & 0xffffffff;
      DAT_00284688 = 0;
      FUN_00191dd0();
LAB_00188e4c:
      FUN_0014cc0c(iVar9);
      goto LAB_00188e54;
    }
    DAT_00284688 = 0;
    FUN_00191dd0();
    if (!bVar11) goto LAB_00188e54;
    iStack_4d8 = 0;
    uStack_4d4 = 0;
    local_4e0 = 0;
    iStack_4dc = 0;
    local_4c8 = 0;
    uStack_4c4 = 0;
    local_4d0 = 0;
    uStack_4b8 = 0;
    uStack_4b4 = 0;
    local_4c0 = 0;
    uStack_4bc = 0;
    local_4a8 = 0;
    local_4b0 = 0;
    iVar13 = FUN_00191d10(&local_4e0,0x40);
    iVar12 = iStack_4d8;
    uVar21 = 0;
    if (((iVar13 != 0) && (local_4e0 == 1)) && ((iStack_4dc == 0x40 && (iStack_4d8 != 0)))) {
      uVar21 = 0;
      if ((0xffffffeb < (uint)local_4d0 - 0x15) && (uStack_4d4 < 4)) {
        if (local_4d0._4_4_ >> (ulong)((uint)local_4d0 & 0x1f) == 0) {
          auVar23._4_4_ = local_4c0;
          auVar23._0_4_ = uStack_4c4;
          auVar23._8_4_ = uStack_4bc;
          auVar23._12_4_ = uStack_4b8;
          uVar14 = 0;
          auVar23 = NEON_cmhi(auVar23,_DAT_0010fae0,4);
          uVar22 = NEON_umaxv(CONCAT26(auVar23._12_2_,
                                       CONCAT24(auVar23._8_2_,CONCAT22(auVar23._4_2_,auVar23._0_2_))
                                      ),2);
          uVar21 = 0;
          if (((((uVar22 & 1) == 0) && (uVar21 = uVar14, uStack_4b4 < 2)) &&
              ((local_4c8 & 0xfffe3fff) == 0)) && (local_4a8._4_4_ < 2)) {
            uVar21 = 0;
            if (((uint)local_4b0 <= local_4b0._4_4_) && (local_4b0._4_4_ < 0x4001)) {
              uVar21 = (uint)((uint)local_4a8 < 5);
            }
          }
        }
        else {
          uVar21 = 0;
        }
      }
    }
    memset(auStack_494,0,0x401);
    if (uVar7 == 0) {
switchD_00189070_caseD_4:
      bVar11 = false;
      iVar13 = 0;
      uVar14 = 1;
      bVar6 = false;
      uStack_4fc = 0;
    }
    else if ((uVar2 < 0x14) && (bVar11 = false, uVar21 != 0)) {
      uVar2 = *(uint *)(&DAT_0019dab0 + uVar16 * 0x28);
      if (uVar2 < 0x14) {
        iVar13 = 0;
        uVar14 = 0;
        bVar6 = false;
        uStack_4fc = 1;
        if (uVar2 < (uint)local_4d0) {
          if (((local_4d0._4_4_ >> (ulong)(uVar2 & 0x1f) & 1) == 0) ||
             ((uVar2 != 0x12 && (iVar12 != iVar15)))) goto LAB_00188fac;
          bVar11 = true;
          uVar14 = 1;
          bVar6 = false;
          uStack_4fc = 1;
          iVar13 = 0;
          switch(uVar7) {
          case 1:
            iVar13 = FUN_0014c510();
            if (iVar13 == 0) {
              uVar14 = 0;
            }
            else {
              bVar11 = uVar2 == 9;
              pcVar19 = "PLACEMENT MODE 0-4";
              if (bVar11) {
                pcVar19 = "RECTANGLE x1,y1,x2,y2";
              }
              uVar20 = 0x60;
              if (!bVar11) {
                uVar20 = 1;
              }
              iVar15 = FUN_0014c550(iVar13,pcVar19,&DAT_00134f22,!bVar11,uVar20);
              uVar14 = (uint)(iVar15 != 0);
            }
            bVar11 = false;
            bVar6 = false;
            uStack_4fc = uVar14 ^ 1;
            break;
          case 2:
            local_4e4 = 3;
            uVar14 = FUN_0014c8ac(iVar9,&local_4e4,auStack_494,0x401);
            bVar11 = false;
            if ((uVar14 == 0) || (local_4e4 == 3)) {
              bVar6 = false;
              uStack_4fc = 1;
              iVar13 = 0;
            }
            else if (local_4e4 == 2) {
              bVar11 = false;
              bVar6 = false;
              uStack_4fc = 1;
              iVar13 = 0;
              uVar14 = 2;
            }
            else if (local_4e4 == 1) {
              pvVar18 = memchr(auStack_494,0,0x401);
              if ((pvVar18 == (void *)0x0) ||
                 (iVar15 = FUN_001895e0(uVar2,auStack_494,&local_90), iVar15 == 0)) {
                bVar11 = false;
                uVar14 = 0;
                iVar13 = 0;
                bVar6 = false;
                uStack_4fc = 1;
              }
              else {
                if ((uVar2 - 8 < 5) && ((0x17U >> (ulong)(uVar2 - 8 & 0x1f) & 1) != 0)) {
                  bVar11 = true;
                }
                else {
                  bVar11 = uVar2 == 0x13;
                }
                bVar6 = (bool)(bVar11 ^ 1);
                uVar14 = 1;
                uStack_4fc = 1;
                iVar13 = 0;
              }
            }
            else {
              bVar11 = false;
              bVar6 = false;
              uStack_4fc = 0;
              iVar13 = 0;
            }
            break;
          case 3:
            break;
          default:
            goto switchD_00189070_caseD_4;
          case 5:
            bVar11 = false;
            iVar13 = 0;
            bVar6 = true;
            uStack_4fc = 1;
          }
        }
      }
      else {
        iVar13 = 0;
        bVar6 = false;
        uStack_4fc = 1;
        uVar14 = 0;
      }
    }
    else {
LAB_00188fac:
      uVar14 = 0;
      bVar11 = false;
      iVar13 = 0;
      bVar6 = false;
      uStack_4fc = 1;
    }
    uVar17 = FUN_00193f80(1,&DAT_00284688);
    if ((uVar17 & 1) == 0) {
      if (((DAT_0028469c == '\0') || (DAT_0028469d != 'e')) || (DAT_0028cc40 != lVar8)) {
        DAT_00284688 = 0;
        if (iVar13 != 0) {
          FUN_0014cc0c(iVar13);
        }
        uVar10 = 4;
      }
      else {
        _DAT_0028cc2c = CONCAT44(DAT_0028cc30,uVar21);
        if (uVar21 != 0) {
          _DAT_0028cbe8 = CONCAT44(iStack_4dc,local_4e0);
          auVar4._8_4_ = iStack_4d8;
          auVar4._0_8_ = _DAT_0028cbe8;
          auVar4._12_4_ = uStack_4d4;
          _DAT_0028cc00 = CONCAT44(uStack_4c4,local_4c8);
          uRam000000000028cc10 = CONCAT44(uStack_4b4,uStack_4b8);
          _DAT_0028cc08 = CONCAT44(uStack_4bc,local_4c0);
          _DAT_0028cbf0 = auVar4._8_8_;
          _DAT_0028cbf8 = local_4d0;
          uRam000000000028cc20 = local_4a8;
          _DAT_0028cc18 = local_4b0;
        }
        DAT_0028cc48 = param_2;
        if (uVar7 == 0) {
          _DAT_0028cc2c = (ulong)uVar21;
        }
        else if (bVar11) {
          DAT_0028cc54 = CONCAT44(uStack_8c,local_90);
          auVar5._8_4_ = uStack_88;
          auVar5._0_8_ = DAT_0028cc54;
          auVar5._12_4_ = uStack_84;
          DAT_0028cc50 = 0;
          DAT_0028cc3c = 0;
          _DAT_0028cc2c = CONCAT44(4,uVar21);
          DAT_0028cc5c = auVar5._8_8_;
          DAT_0028469f = 0;
          FUN_00194010(1,0x22d828);
          if (*(int *)(&DAT_0019dab0 + uVar16 * 0x28) == 9) {
            FUN_00183ac8(&DAT_0028dbe4,0x100,0x100,
                         "RECTANGLE %d,%d TO %d,%d | MAP %dx%d | CONFIRM TO EDIT",local_90,uStack_8c
                         ,uStack_88,uStack_84,uStack_4bc,uStack_4b8);
          }
          else {
            pcVar19 = "UNSAVED EDITS ARE NOT SAVED";
            if (*(int *)(&DAT_0019dab0 + uVar16 * 0x28) != 0x13) {
              pcVar19 = "CHANGES THE CURRENT MAP; NO AUTO-SAVE";
            }
            FUN_00183ac8(&DAT_0028dbe4,0x100,0x100,"%s | MAP %dx%d | %s",
                         (&PTR_s_OPEN_MAP_EDITOR_0019dac0)[uVar16 * 5],uStack_4bc,uStack_4b8,pcVar19
                        );
          }
        }
        else if (uStack_4fc == 0) {
          if ((uVar7 < 3) && (_DAT_0028cc2c = CONCAT44(2,uVar21), iVar13 != 0)) {
            DAT_0028cc50 = iVar13;
          }
        }
        else {
          pcVar19 = "EDITOR OR INPUT CHANGED - SELECT ACTION AGAIN";
          if (uVar14 != 0) {
            pcVar19 = "APPLYING...";
          }
          pcVar1 = "CANCELLED";
          if (uVar14 != 2) {
            pcVar1 = pcVar19;
          }
          _DAT_0028cc2c = (ulong)uVar21;
          DAT_0028cc50 = 0;
          FUN_00183ac8(&DAT_0028cc64,0x80,0x80,&DAT_001343dc,pcVar1);
        }
        if (((bVar6) && (*(uint *)(&DAT_0019dab0 + uVar16 * 0x28) < 0x14)) &&
           ((1 << (ulong)(*(uint *)(&DAT_0019dab0 + uVar16 * 0x28) & 0x1f) & 0xa0801U) != 0)) {
          DAT_0028469c = '\0';
          _DAT_0028cc2c = _DAT_0028cc2c & 0xffffffff00000000;
        }
        DAT_002846a0 = DAT_002846a0 + 1;
        DAT_00284688 = 0;
        if ((uStack_4fc != 0) && (iVar9 != 0)) {
          FUN_0014cc0c(iVar9);
        }
        if ((uStack_4fc != 0) && (iVar13 != 0)) {
          FUN_0014cc0c(iVar13);
        }
        if (bVar6) {
          uVar2 = *(uint *)(&DAT_0019dab0 + uVar16 * 0x28);
          uVar14 = FUN_00191d58(uVar2,local_90,uStack_8c,uStack_88,uStack_84);
          uVar16 = FUN_00193f80(1,&DAT_00284688);
          if ((uVar16 & 1) == 0) {
            if ((DAT_0028cc40 == lVar8) && (DAT_0028469d == 'e')) {
              DAT_0028cc48 = 0;
              if (uVar14 == 0) {
                DAT_0028469c = '\x01';
                FUN_00183ac8(&DAT_0028cc64,0x80,0x80,"ACTION COULD NOT BE APPLIED");
              }
              else {
                if (uVar2 == 0x10) {
                  pcVar19 = "REOPEN THE EDITOR TO APPLY PALETTE";
                }
                else if (uVar2 == 0x12) {
                  pcVar19 = "FILL CANCELLED; COMPLETED TILES KEPT";
                }
                else {
                  pcVar19 = "FILL QUEUED; NO AUTO-SAVE";
                  if ((uVar2 & 0xfffffffe) != 8 && uVar2 != 10) {
                    pcVar19 = "APPLIED";
                  }
                }
                FUN_00183ac8(&DAT_0028cc64,0x80,0x80,&DAT_001343dc,pcVar19);
              }
              DAT_002846a0 = DAT_002846a0 + 1;
            }
            DAT_00284688 = 0;
          }
        }
        uVar10 = uVar14 != 0;
      }
    }
    else {
      if (iVar13 != 0) {
        FUN_0014cc0c(iVar13);
      }
      uVar10 = 3;
    }
  }
  DAT_0028dce8._0_4_ = 0;
LAB_00188e5c:
  if (*(long *)(lVar3 + 0x28) == local_78) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar10);
}


/* ===== nexus_menu_editor_open @ 00194710 ===== */

void nexus_menu_editor_open(void)

{
  (*(code *)PTR_nexus_menu_editor_open_001a3a90)();
  return;
}


/* ===== nexus_menu_editor_action @ 00194720 ===== */

void nexus_menu_editor_action(void)

{
  (*(code *)PTR_nexus_menu_editor_action_001a3a98)();
  return;
}


/* ===== nexus_menu_editor_pump @ 00194770 ===== */

void nexus_menu_editor_pump(void)

{
  (*(code *)PTR_nexus_menu_editor_pump_001a3ac0)();
  return;
}


/* ===== nexus_menu_editor_scroll_revision @ 00194900 ===== */

void nexus_menu_editor_scroll_revision(void)

{
  (*(code *)PTR_nexus_menu_editor_scroll_revision_001a3b88)();
  return;
}
