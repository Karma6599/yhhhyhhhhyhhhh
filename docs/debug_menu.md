# Hidden / Debug Menu Layer — Nexus Brawl 69.252

> Auto-generated from the embedded `nexus-overlay-wire/v1` JSON (233 entries) and the
> decompiled native modules. This documents every non-ordinary-menu feature surface:
> the game's own hidden dev menus (re-exposed by the mod), the map-editor unlock,
> and the full 151-entry client-tweak flag layer.

## Layer Map

| Layer | Count | Where it lives | Dispatcher |
|---|---|---|---|
| Mod config entries (`menu.*`, popups) | 82 | UI wire | `nexus_menu_actions` |
| Client tweak flags (game-property patches) | 41 | EvasionRuntime | `FUN_001381e0` (strcmp resolver) |
| Cheat-engine flags (lowerCamelCase) | 110 | EvasionRuntime / Evasion | per-feature `nexus_*_set_feature` |
| Game dev menu — **BSD debug** | 40 | game (hidden) | `nexus_menu_debug_action` |
| Game dev menu — **Map Editor** | 20 | game (hidden) | `nexus_menu_editor_action` |
| Themes popup | 11 | UI | `nexus_menu_theme_action` |
| Profile popup | 4 | UI | `nexus_menu_profile_action` |
| Battle servers popup | 2 | UI | `nexus_menu_server_action` |

---

## 1. BSD Debug Popup — the game's own dev menu (40 commands)

Key `BSDDebugPopup`, label **"BSD debug"**. All commands are `control=3` (buttons),
**all free**. actionId base `0x27020` (159776). These drive the stock game's internal
debug command index — screens and behaviours normally unreachable without a dev build.

| actionId | Key | Label | Action |
|---|---|---|---|
| 159776 | `debug.ABOUT_SCREEN` | About Screen | open About screen |
| 159777 | `debug.GENERIC_INFO` | Generic Info | open generic info popup |
| 159778 | `debug.ESPORTS` | ESPorts | open esports screen |
| 159779 | `debug.NOTIFICATION_SETTINGS` | Notifications | open notification settings |
| 159780 | `debug.FAME_POPUP` | Fame Preview | preview Fame popup |
| 159781 | `debug.GOTO_HOME` | Go To Home | navigate Home |
| 159782 | `debug.GOTO_QUESTS` | Go To Quests | navigate Quests |
| 159783 | `debug.GOTO_BRAWL_PASS` | Go To Brawl Pass | navigate Brawl Pass |
| 159784 | `debug.GOTO_CLUBS` | Go To Clubs | navigate Clubs |
| 159785 | `debug.GOTO_PRO_PASS` | Go To Pro Pass | navigate Pro Pass |
| 159786 | `debug.GOTO_CLAN` | Go To Clan | navigate Clan |
| 159787 | `debug.GOTO_SCID_REWARDS` | SCID Rewards | open SCID rewards |
| 159788 | `debug.CHAT_OPTIONS` | Chat Options | open chat options |
| 159789 | `debug.BRAWLER_UNLOCK_ANIM` | Brawler Reward Preview | preview brawler reward |
| 159790 | `debug.NOT_ENOUGH_GEMS` | Gems Popup | preview Gems popup |
| 159791 | `debug.FAME_LEVEL_UP_PREVIEW` | Fame Level Up | preview Fame level-up |
| 159792 | `debug.RANKED_SEASON_END_POPUP` | Ranked Season End | preview Ranked season end |
| 159793 | `debug.MOVIE_PLAYER_POUP` | Movie Player | open movie player |
| 159794 | `debug.BRAWL_TV` | Brawl TV Intro | play Brawl TV intro |
| 159795 | `debug.SHOW_PRESTIGE_INTRO` | Prestige Intro | play Prestige intro |
| 159796 | `debug.INVITE_FRIEND_CODE` | Invite code | show invite code |
| 159797 | `debug.UNLOCK_ACCOUNT_SCREEN` | Account unlock | open account unlock |
| 159798 | `debug.SET_COUNTRY` | Country | set country |
| 159799 | `debug.STOP_ALL_SFX` | Stop sound effects | stop all SFX |
| 159800 | `debug.STOP_MUSIC` | Stop music | stop music |
| 159801 | `debug.PAUSE_MUSIC_TOGGLE` | Pause / resume music | pause/resume music |
| 159802 | `debug.MUSIC_VOLUME_CYCLE_0_50_100` | Next music volume | cycle music volume 0/50/100 |
| 159803 | `debug.BOSS_MUSIC_TOGGLE` | Boss music | toggle boss music |
| 159804 | `debug.GUI_CLOSE_ALL_POPUPS` | Close popups | close all popups |
| 159805 | `debug.GUI_TEST_FLOATER` | Test message | show test message floater |
| 159806 | `debug.OPEN_CLAN_POPUP` | Club window | open Club window |
| 159807 | `debug.OPEN_TEAMUP_POPUP` | Team window | open Team window |
| 159808 | `debug.LATENCY_TEST_START` | Latency diagnostics | start latency diagnostics |
| 159809 | `debug.CYCLE_LANGUAGE` | Next language | cycle game language |
| 159810 | `debug.SHOW_TID_KEYS` | Show / hide text keys | toggle raw text-key display |
| 159811 | `debug.SKIP_GACHA_ANIM` | Skip gacha animation | skip gacha animation |
| 159812 | `debug.GFX_QUALITY_CYCLE` | Next graphics quality | cycle graphics quality |
| 159813 | `debug.MEM_QUALITY_CYCLE` | Next memory quality | cycle memory quality |
| 159814 | `debug.SOFT_RELOAD_GAME` | Reload game | soft-reload the game |
| 159815 | `debug.AA_DIALOG` | Play time dialog | show play-time (anti-addiction) dialog |

---

## 2. Map Editor Popup — hidden map editor unlock (20 commands)

Key `MapEditorPopup`. actionId base `0x28FC0` (167968). Unlocks the game's internal
map editor plus validation/placement bypasses and the full tile palette.

| actionId | Key | Label |
|---|---|---|
| 167968 | `editor.OPEN_MAP_EDITOR_POPUP` | Open Map Editor — opens the editor popup itself |
| 167969 | `editor.TOGGLE_GRID` | Grid — editor grid overlay |
| 167970 | `editor.PLACEMENT_NEXT` | Mirror Next — cycle placement mirror (next) |
| 167971 | `editor.PLACEMENT_PREVIOUS` | Mirror Previous — cycle placement mirror (prev) |
| 167972 | `editor.PLACEMENT_SET` | Placement Mode — placement mode |
| 167973 | `editor.UNDO` | Undo — undo |
| 167974 | `editor.REDO` | Redo — redo |
| 167975 | `editor.SELECT_ERASER` | Eraser — eraser tool |
| 167976 | `editor.FILL_ALL` | Fill All — fill all tiles |
| 167977 | `editor.FILL_REGION` | Fill Region — fill region |
| 167978 | `editor.ERASE_ALL` | Erase All — erase all tiles |
| 167979 | `editor.SAVE` | Save Map — save map |
| 167980 | `editor.CLEAR_ALL` | Clear Map — clear map |
| 167981 | `editor.REFRESH_TILE_COUNTS` | Refresh Counts — refresh tile counters |
| 167982 | `editor.BYPASS_SAVE_VALIDATION` | Save Validation Bypass — skip save validation rules |
| 167983 | `editor.BYPASS_PLACEMENT_ZONES` | Placement Zones Bypass — ignore placement-zone limits |
| 167984 | `editor.UNLOCK_FULL_PALETTE` | Full Palette — unlock all tiles/objects |
| 167985 | `editor.MAP_MODIFIERS` | Map Modifiers — map modifiers panel |
| 167986 | `editor.CANCEL_FILL` | Cancel Fill — cancel fill operation |
| 167987 | `editor.GO_HOME` | Go Home — leave editor |

---

## 3. Client-Tweak Flag Layer (151 entries)

These are the "~150 features". They patch game properties at runtime. UpperCamelCase
keys are classic BSD-Mods-style client patches resolved by a single strcmp dispatcher
(`FUN_001381e0` in libNexusEvasionRuntime69252.so → state bitfields / typed globals).

### 3.1 Game-property patches (41, UpperCamelCase)

| Key | Label | Category | Paid |
|---|---|---|---|
| `ExtendedTrajectory` | Ball trajectory | combat | free |
| `HideSuperAim` | Hide super aim | combat | free |
| `FPSLimit` | FPS limit | settings | free |
| `FriendListOptimization` | Light friend list | settings | free |
| `UseBattleProxy` | Battle proxy | settings | **Nexus+** |
| `UseLowResGraphics` | Max optimization | settings | free |
| `BackgroundMatchmaking` | Background matchmaking | utility | free |
| `BattleEndInstantExit` | Instant battle exit | utility | free |
| `BattleTextChat` | Battle chat | utility | free |
| `DoNotShowBattleHighlight` | Skip highlights | utility | free |
| `EnforceBattleChatButton` | Show chat button | utility | free |
| `HideBattlingStatusFromOthers` | Hide battle status | utility | free |
| `InstantStarrDropOpening` | Instant Starr Drop | utility | free |
| `ShowFastPlayAgainButton` | Fast play again | utility | free |
| `ShowFriendlyRoomOpponents` | Friendly opponents | utility | free |
| `SlowMode` | Slow mode | utility | free |
| `AllyRespawnTimer` | Ally respawn | visual | free |
| `ChromaticName` | Chromatic name | visual | free |
| `DefaultEnvironments` | Default environments | visual | free |
| `DisablePinAnimation` | Static pins | visual | free |
| `DisableShake` | Disable shake | visual | free |
| `DisableSkins` | Disable skins | visual | free |
| `Font` | Fonts | visual | free |
| `HideBattleBlackBars` | Hide black bars | visual | free |
| `HideHomeScreenText` | Hide lobby info | visual | free |
| `HighlightLeonClone` | Highlight Leon clone | visual | free |
| `LegacyNames` | Legacy brawler names | visual | free |
| `ShamePlayersWithThumbsdownPin` | Mark thumbs-down | visual | free |
| `ShowAllianceMembersInBattle` | Mark club members | visual | free |
| `ShowBattleCameraButton` | Camera | visual | free |
| `ShowBattleConnectionIndicator` | Ping | visual | free |
| `ShowCharactersInNames` | Brawler names | visual | free |
| `ShowDPS` | DPS counter | visual | free |
| `ShowEnemyAmmoStatus` | Enemy ammo | visual | free |
| `ShowFPSCounter` | FPS counter | visual | free |
| `ShowFriendsInBattle` | Mark friends | visual | free |
| `ShowOwnPlayerCoordinates` | Player coordinates | visual | free |
| `ShowSkinNamesInProfile` | Profile skin names | visual | free |
| `SmoothHudGraph` | Frame-time graph | visual | free |
| `TeammateHPIndicator` | Teammate HP | visual | free |
| `TileGrid` | Tile grid | visual | free |

### 3.2 Cheat-engine flags (110, lowerCamelCase)

Runtime state consumed by the native feature modules (killaura, ESP, prediction...).

#### Combat (47)

| Key | Label | Paid |
|---|---|---|
| `aopAimEnabled` | Smart aim | free |
| `aopAimGadget` | Aim gadget | **Nexus+** |
| `aopAimTargetMode` | Aim target mode | free |
| `aopAimUltimate` | Aim ultimate | **Nexus+** |
| `aopLeadScale` | Lead strength | free |
| `aopPredictEnabled` | Prediction | free |
| `aopPredictVersion` | Predictor | free |
| `aopReactionMs` | Reaction delay | free |
| `aopTargetMode` | Target mode | free |
| `autododgeEnabled` | Auto dodge | free |
| `ballAssistEnabled` | Ball assist | **Nexus+** |
| `boltAutoAttackEnabled` | Bolt auto attack | **Nexus+** |
| `boltExitHoldMs` | Exit hold | **Nexus+** |
| `boltModEnabled` | Bolt autopilot | **Nexus+** |
| `boltPredictionEnabled` | Bolt prediction | **Nexus+** |
| `boltSafeExitEnabled` | Safe exit | **Nexus+** |
| `boltSmoothPercent` | Steering smoothness | **Nexus+** |
| `boltTargetRange` | Target range | **Nexus+** |
| `boltWallAvoidEnabled` | Avoid walls | **Nexus+** |
| `boltWallLookahead` | Wall look-ahead | **Nexus+** |
| `coltModEnabled` | Colt mod | free |
| `combatAuraRange` | Attack range | free |
| `combatFireInterval` | Fire interval | free |
| `dodgeBurstHoldMs` | Burst hold | free |
| `dodgeCommitPercent` | Dodge commitment | free |
| `dodgeDistancePercent` | Dodge distance | free |
| `dodgePowerPercent` | Dodge strength | free |
| `dodgeReactionPercent` | Dodge reaction | free |
| `dodgeSafetyPercent` | Safety margin | free |
| `dodgeVersion` | Dodge logic | free |
| `dynaJumpEnabled` | Dyna jump | **Nexus+** |
| `followEnabled` | Follow | **Nexus+** |
| `holdToShootEnabled` | Hold fire | free |
| `isSpinEnabled` | Spin | **Nexus+** |
| `isXrayEnabled` | X-Ray | **Nexus+** |
| `killauraEnabled` | Kill aura | free |
| `killauraMainAttack` | Main attack | free |
| `killauraNoBall` | Ignore ball | free |
| `killauraNoWall` | Wall check | free |
| `kitNaniModEnabled` | Nani ulti mod | **Nexus+** |
| `naniAutoDodgeEnabled` | Nani ulti mod | **Nexus+** |
| `naniControlMode` | Nani mode | **Nexus+** |
| `spinMovementMode` | Online movement | **Nexus+** |
| `spinOnlineOnlyIdle` | Only while idle | **Nexus+** |
| `spinSpeed` | Spin speed | **Nexus+** |
| `xrayShowTargetName` | Target name | **Nexus+** |
| `xrayTargetMode` | Target mode | **Nexus+** |

#### Visual (8)

| Key | Label | Paid |
|---|---|---|
| `attackRangeIndicator` | Attack range | **Nexus+** |
| `characterOutlineEnabled` | Character outline | free |
| `enemyTracer` | Enemy tracer | **Nexus+** |
| `espEnabled` | ESP | **Nexus+** |
| `hitboxRenderer` | Hitboxes | **Nexus+** |
| `outlineOpacity` | Outline opacity | free |
| `speedLocalMoveEnabled` | Spectator mode | **Nexus+** |
| `trophiesAboveHead` | Trophies | **Nexus+** |

#### Utility (32)

| Key | Label | Paid |
|---|---|---|
| `antiAfkEnabled` | Anti-AFK | free |
| `autofarmAttackEnemies` | Attack enemies | **Nexus+** |
| `autofarmEnabled` | Auto farm | **Nexus+** |
| `editor.BYPASS_PLACEMENT_ZONES` | Placement Zones Bypass | free |
| `editor.BYPASS_SAVE_VALIDATION` | Save Validation Bypass | free |
| `editor.CANCEL_FILL` | Cancel Fill | free |
| `editor.CLEAR_ALL` | Clear Map | free |
| `editor.ERASE_ALL` | Erase All | free |
| `editor.FILL_ALL` | Fill All | free |
| `editor.FILL_REGION` | Fill Region | free |
| `editor.GO_HOME` | Go Home | free |
| `editor.MAP_MODIFIERS` | Map Modifiers | free |
| `editor.OPEN_MAP_EDITOR_POPUP` | Open Map Editor | free |
| `editor.PLACEMENT_NEXT` | Mirror Next | free |
| `editor.PLACEMENT_PREVIOUS` | Mirror Previous | free |
| `editor.PLACEMENT_SET` | Placement Mode | free |
| `editor.REDO` | Redo | free |
| `editor.REFRESH_TILE_COUNTS` | Refresh Counts | free |
| `editor.SAVE` | Save Map | free |
| `editor.SELECT_ERASER` | Eraser | free |
| `editor.TOGGLE_GRID` | Grid | free |
| `editor.UNDO` | Undo | free |
| `editor.UNLOCK_FULL_PALETTE` | Full Palette | free |
| `followClosestAllyEnabled` | Closest ally | **Nexus+** |
| `followDistance` | Follow distance | **Nexus+** |
| `holdToShootAim` | Hold aim | free |
| `pinEnabled` | Auto pin | **Nexus+** |
| `profile.name` | Visual name | free |
| `profile.open` | Open profile | free |
| `profile.reload` | Reload game | free |
| `profile.reset_name` | Reset visual name | free |
| `sprayEnabled` | Auto spray | **Nexus+** |

#### Settings (23)

| Key | Label | Paid |
|---|---|---|
| `combatDodgeBlacklistMask.bit0` | Shelly Attack | free |
| `combatDodgeBlacklistMask.bit1` | Shelly Super | free |
| `combatDodgeBlacklistMask.bit2` | Primo Attack | free |
| `combatDodgeBlacklistMask.bit3` | Frank / Rosa | free |
| `combatDodgeBlacklistMask.bit4` | Frank Super | free |
| `combatDodgeBlacklistMask.bit5` | Emz Attack | free |
| `combatDodgeBlacklistMask.bit6` | Buzz Attack | free |
| `combatDodgeBlacklistMask.bit7` | Edgar Attack | free |
| `combatDodgeBlacklistMask.bit8` | Grom Attack | free |
| `combatDodgeBlacklistMask.bit9` | Bull Attack | free |
| `server.automatic` | Automatic | free |
| `server.refresh` | Refresh ping | free |
| `theme.confirm` | Apply theme | free |
| `theme.legacy` | Legacy background | free |
| `theme.next` | Next theme | free |
| `theme.no_music` | No music | free |
| `theme.options` | Theme options | free |
| `theme.previous` | Previous theme | free |
| `theme.random_battle` | Random after battle | free |
| `theme.random_launch` | Random at launch | free |
| `theme.random_music` | Independent music | free |
| `theme.reset` | Reset theme | free |
| `theme.shared` | Shared background | free |

---

## 4. Auxiliary Popups

### Themes (11) — `ThemesPopup`

Dispatched via `nexus_menu_theme_action`.

| actionId | Key | Label |
|---|---|---|
| 155652 | `theme.options` | Theme options |
| 155653 | `theme.reset` | Reset theme |
| 155654 | `theme.no_music` | No music |
| 155655 | `theme.confirm` | Apply theme |
| 155656 | `theme.random_launch` | Random at launch |
| 155657 | `theme.random_battle` | Random after battle |
| 155658 | `theme.random_music` | Independent music |
| 155659 | `theme.previous` | Previous theme |
| 155660 | `theme.next` | Next theme |
| 155661 | `theme.shared` | Shared background |
| 155662 | `theme.legacy` | Legacy background |

### Profile (4) — `ProfilePopup`

Dispatched via `nexus_menu_profile_action`.

| actionId | Key | Label |
|---|---|---|
| 163841 | `profile.open` | Open profile |
| 163842 | `profile.name` | Visual name |
| 163843 | `profile.reset_name` | Reset visual name |
| 163845 | `profile.reload` | Reload game |

### Battle Servers (2) — `BattleServersPopup`

Dispatched via `nexus_menu_server_action`.

| actionId | Key | Label |
|---|---|---|
| 147458 | `server.automatic` | Automatic |
| 147459 | `server.refresh` | Refresh ping |

---

## 5. Dispatch Internals (reconstruction anchors)

All addresses are file/RVA offsets in libNexusUI69252.so / libNexusEvasionRuntime69252.so.

| What | Symbol / address | Behaviour |
|---|---|---|
| Debug command dispatcher | `nexus_menu_debug_action` @ libNexusUI `0x3a154` region (line 39188 of export) | validates actionId in `[0x27020, 0x27048)`, checks 40-bit availability bitmap `DAT_0028a404`, maps slot→internal command id via table `DAT_0019cf48` (stride 10), enforces single-in-flight + game-thread ready |
| Editor command dispatcher | `nexus_menu_editor_action` @ libNexusUI (line 40172 of export) | same pattern for `[0x28FC0, 0x290D4)` |
| Tweak-flag resolver | `FUN_001381e0` @ libNexusEvasionRuntime `0x1381e0` | strcmp ladder over the 41 game-property keys → bitfields (`DAT_0020afa0` bits 3/4/8/0x10/0x20, `DAT_00209a78`, `DAT_00214938` typed globals); sub-resolvers `FUN_0017f95c`, `FUN_0017fa00` |
| Menu action pipeline | `nexus_menu_actions` / `nexus_menu_action_status` | generic wire action executor + status query |
| Debug open/pump | `nexus_menu_debug_open`, `nexus_menu_debug_pump` | open hidden screens, pump pending open requests ("OPENING...", "SCREEN COULD NOT BE OPENED") |
| Diagnostics | `nexus_menu_diagnostics`, `nexus_rich_diagnostics` | state JSON: phase/built/planned/strings/projection/binding |
| Script ports | `nexus_script_port_client_debug_apply`, `nexus_script_port_client_debug_snapshot` | QuickJS-side apply/snapshot of client debug state |

### Slot mapping rule

```
debug.* :  actionId = 159776 + slot          (slot 0..39)
editor.*:  actionId = 167968 + slot          (slot 0..19)
```

---

## 6. What `assets/nexus-lab/` is (and is not)

`nexus-lab` is **not** a menu. It is a standalone diagnostic laboratory package
(`nexus.independent.lab69252`) shipping a 5th module, `libNexusBase69252.so` (97,952 B),
used to research Supercell Shield anti-cheat behaviour away from the production mod:

- 23 experimental "clone" bootstrap profiles (JNI table clones, file-view aliases, signal adapters...) — all `enabled_by_default: false`, none production-ready
- native probes for `libg.so` and `libsupercell_brawlstars.so` (segment dumps, Build ID pinning)
- pins the stock `stock-base-69252.apk` for package-manager lookups
- `unsupported_offsets`: game offsets from source version 68.263 are **unported**; no game hooks run in the lab

It has no user-facing features and is inert in the shipped mod path.

