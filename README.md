# Nexus Brawl — decompiled feature source (rev 137, build 69.252)

Decompiled & decrypted sources of the "Nexus" Brawl Stars mod (package `bsd.suitcase.nexusv2`,
build `69.252-nexus-protected-r3`, server release revision **137**). All content below was
recovered from the live delivery server payloads + Ghidra 11.3.2 headless decompilation (arm64).

## How the mod works (boot chain)

```
APK (patched Brawl Stars 69.252)
 └─ classes3.dex (mod loader, injected; source not in this repo)
     ├─ XOR-config -> delivery servers (mandarinvpn.space:2097 / 45.9.116.97:2097)
     ├─ protocol v5: /challenge -> /session -> /payload  (RSA-OAEP + AES-256-GCM)
     └─ downloads 4 native modules + 3 JS scripts (release ticket, sha256-pinned)
          ├─ libNexusScriptRuntime69252.so  QuickJS host + JS<->native bridge
          ├─ libNexusEvasionRuntime69252.so game hooks: EGL/GL, aim/aura/dodge engine
          ├─ libNexusEvasion69252.so        movement cheats (spin/dodge/hold)
          ├─ libNexusUI69252.so             mod menu + renderer (233 entries)
          └─ scripts/*.js                   plaintext helper scripts on Nexus.* API
```

There is **no single obfuscated main script** — every feature is compiled C++ inside the
native modules. The JS layer is only a thin bridge (`scripts/nexus-own-bridge.js`, frozen
`Nexus.{define,require,binding,emit}` API over `__nexusCall`/`__nexusEmit` natives).

## Layout

| folder | content |
|---|---|
| `features/` | one file per cheat feature — implementations from all modules merged |
| `ui/` | mod-menu subsystem split by concern |
| `core/` | shared plumbing: GL hooks, observers, script host, runtime |
| `scripts/` | the actual JS layer (bridge bootstrap + the 3 server scripts) |
| `docs/` | `feature_list.json` + `menu_wire.json` (full embedded menu config) + [`debug_menu.md`](docs/debug_menu.md) + extracted tables |

## features/ (233 menu entries total: 182 free / 51 Nexus+ paid)

| file | funcs | notes |
|---|---|---|
| `anti_afk.c` | 3 | |
| `aura.c` | 8 | — exports: `nexus_evasion_prediction_snapshot_v1()`, `nexus_evasion_hold_snapshot_v1()`, `nexus_evasion_aura_snapshot_v1()`, `nexus_evasion_runtime_aura_route_v1()`, `JNI_OnLoad()` |
| `autododge.c` | 23 | — exports: `nexus_evasion_functions_snapshot_v1()`, `nexus_evasion_dodge_profile()`, `nexus_autododge_set_enabled()`, `nexus_autododge_set_replay_block()`, `nexus_evasion_resolve_slot()` |
| `autofarm.c` | 8 | — exports: `nexus_evasion_handler_status_v1()`, `nexus_evasion_snapshot_keys_v1()` |
| `ball.c` | 6 | |
| `bolt_mod.c` | 5 | |
| `brawler_mods.c` | 8 | |
| `debug_menu.c` | 8 | — exports: `nexus_menu_debug_open()`, `nexus_menu_debug_action()`, `nexus_menu_debug_pump()`, `nexus_menu_diagnostics()` — bridge to the game's own 40-command dev menu (see [docs/debug_menu.md](docs/debug_menu.md)) |
| `esp.c` | 1 | |
| `follow.c` | 2 | |
| `hold_fire.c` | 8 | — exports: `nexus_evasion_hold_register_v1()`, `nexus_evasion_hold_lease_v1()`, `nexus_evasion_hold_recheck_v1()` |
| `killaura.c` | 4 | |
| `map_editor.c` | 8 | — exports: `nexus_menu_editor_open()`, `nexus_menu_editor_action()`, `nexus_menu_editor_pump()`, `nexus_menu_editor_scroll_revision()` — unlocks the hidden map editor (20 cmds incl. save/placement bypasses) |
| `outline.c` | 5 | |
| `prediction.c` | 2 | |
| `smart_aim.c` | 4 | |
| `spectate.c` | 5 | |
| `speed_fly.c` | 3 | — exports: `nexus_script_port_client_performance_apply()` |
| `spin.c` | 18 | — exports: `nexus_evasion_get_port_state()`, `nexus_evasion_spin_snapshot_v1()`, `nexus_evasion_get_requested()`, `nexus_evasion_get_effective()`, `nexus_evasion_get_port_state()` |
| `trophies.c` | 7 | |
| `visual_tweaks.c` | 60 | — exports: `nexus_script_port_query()`, `nexus_script_port_set()`, `nexus_script_port_reset()`, `nexus_script_port_camera_snapshot()`, `nexus_script_port_hud_snapshot()`, `nexus_script_port_chat_snapshot()` +5 more |
| `xray.c` | 3 | — exports: `nexus_shadowx_set_feature()`, `nexus_shadowx_set_param()` |

## ui/

| file | funcs | notes |
|---|---|---|
| `activation_plus.c` | 7 | — exports: `nexus_rich_plus_layout()`, `nexus_rich_header_entitlement()`, `nexus_rich_set_plus_state()`, `nexus_rich_set_plus_state()`, `nexus_rich_plus_layout()`, `nexus_rich_header_entitlement()` |
| `fonts.c` | 3 | — exports: `nexus_script_port_fonts_open()`, `nexus_script_port_fonts_open()` |
| `menu_engine.c` | 98 | — exports: `nexus_rich_main_row()`, `nexus_menu_init()`, `nexus_menu_start()`, `nexus_menu_status()`, `nexus_menu_register_backend()` +54 more (debug/editor bridges moved to `features/`) |
| `misc.c` | 411 | — exports: `nexus_ui_performance_mode()`, `nexus_ui_performance_mode()` |
| `renderer.c` | 44 | — exports: `nexus_script_port_ui_reload_request()`, `nexus_release_ui_frame_v1()`, `nexus_script_port_ui_graphics_cycle()`, `nexus_rich_asset()`, `nexus_rich_color()`, `nexus_rich_launcher_geometry()` +21 more |
| `themes.c` | 12 | — exports: `nexus_menu_theme_preview()`, `nexus_menu_theme_scroll_revision()`, `nexus_menu_theme_preview_report()`, `nexus_menu_theme_open()`, `nexus_menu_theme_pump()`, `nexus_menu_main_view()` +5 more |
| `widgets.c` | 34 | — exports: `nexus_native_trophy_text_guard()`, `JNI_OnLoad()`, `nexus_rich_plan()`, `nexus_rich_action()`, `nexus_menu_dispatch()`, `nexus_rich_plan()` |

## core/

| file | funcs | notes |
|---|---|---|
| `evasion_core.c` | 24 | **RECONSTRUCTED** (replaces raw pseudocode) — libNexusEvasion69252.so control plane with the binary's real tables recovered via relocation parsing (`docs/evasion_property_table.json`, `docs/evasion_query_table.json`): 144-slot property sheet, 168-slot action table, 118-slot query table; set/get dispatcher, menu-backend protocol, intercept math, full JNI surface; gcc -Wall -Wextra clean — exports: `nexus_evasion_set_feature()`, `nexus_evasion_set_parameter()`, `nexus_evasion_intercept()`, `nexus_evasion_segment_distance()`, `nexus_evasion_menu_backend_v1()`, `JNI_OnLoad()` |
| `game_state_readers.c` | 14 | |
| `gl_hooks.c` | 175 | — exports: `nexus_evasion_get_requested()`, `nexus_evasion_get_effective()`, `nexus_evasion_initialize()`, `nexus_evasion_restore_v1()`, `nexus_evasion_unbind()`, `nexus_evasion_reset()` +48 more |
| `memory_observers.c` | 7 | — exports: `nexus_evasion_unbind()` |
| `runtime_core.c` | 773 | — exports: `nexus_script_port_fast_replay_snapshot()`, `nexus_script_port_fast_replay_claim()`, `nexus_script_port_font_body_current()`, `nexus_script_port_font_apply()`, `nexus_visual_gl_publish_v1()`, `nexus_visual_gl_calibrate_v1()` +3 more |
| `script_bindings.c` | 134 | — exports: `nexus_script_install_bindings_v1()`, `nexus_script_verify_game_v1()`, `nexus_script_game_base_v1()`, `JNI_OnLoad()` |

## Key exported entry points

- `nexus_evasion_runtime_aura_route_v1` — aim/aura dispatcher (feature target routing)
- `nexus_visual_gl_install_v1` — EGL/GL hook installation (`eglSwapBuffers`, `glClear`, ...)
- `nexus_evasion_runtime_frame_consumer_v1` / `ng_bind_v1` / `ng_feed_v1` — game-state observer pipeline
- `nexus_script_install_bindings_v1` + `JNI_OnLoad` (ScriptRuntime) — QuickJS host + bridge install
- `nexus_menu_*` (UI) — menu engine surface consumed by the Java overlay host
- `nexus_shadowx_set_feature` — X-Ray/ShadowX toggle

## Reconstruction notes

- Visuals (ESP / outline / xray / tracers) render from the GL hook layer: the outline is
  done by **patching the game's GLSL at runtime** — `u_nexusOutline` uniform injected at
  `vec4(v_outlineColor, 1.0)` emission sites (see `features/outline.c`).
- Aim family: `aura` = weapon-aware attack engine (candidates, weapon radius, wall/ball
  checks, fire interval); `smart_aim` = input injection; `prediction` = selectable
  predictor versions; policy JSON marks aim/prediction/dodge as free entitlement scope.
- Each `.c` header lists the related menu entries with free/paid flags; function headers
  carry the source module tag `[libNexus*.so]`.
- `FUN_xxxxxxx` names are Ghidra auto-names; the hex after `@` is the offset in the
  original module. Cross-module calls need rebasing per module.
- Stock QuickJS internals ({~1200 functions}) are **not** included — ScriptRuntime embeds
  the public QuickJS `2024-01-13-frida` build; only the Nexus host layer is in
  `core/script_bindings.c`.
- `ui/misc.c` = string-less C++ internals (STL/templates) that could not be attributed.

## Hidden / debug menus

The 233 wire entries include surfaces beyond the ordinary cheat toggles — full breakdown
in [`docs/debug_menu.md`](docs/debug_menu.md):

- **BSD debug popup** (40 cmds, `0x27020+`): the game's own dev menu re-exposed — hidden
  screens (Fame, ESPorts, Prestige, Brawl TV...), audio control, quality cycles, latency
  diagnostics, TID-key display, soft reload. Dispatched by `nexus_menu_debug_action`.
- **Map editor popup** (20 cmds, `0x28FC0+`): unlocks the game's internal map editor with
  save-validation / placement-zone bypasses and the full tile palette. Dispatched by
  `nexus_menu_editor_action`.
- **151 tweak-flag layer**: 41 UpperCamelCase game-property patches (`ShowFPSCounter`,
  `HideSuperAim`, ...) resolved by the strcmp dispatcher `FUN_001381e0`, plus 110
  lowerCamelCase cheat-engine flags consumed by the native feature modules.
- `assets/nexus-lab/` is **not** a menu — it's a Shield anti-cheat research lab package
  (5th module `libNexusBase69252.so`, 23 experimental clone profiles, no game hooks).

## Menu entries by category (from the embedded wire config)


### Combat (57)

| key | label | paid |
|---|---|---|
| `menu.killaura` | Kill aura |  |
| `menu.autododge` | Auto dodge |  |
| `menu.aim` | Smart aim |  |
| `menu.xray` | X-Ray | **Nexus+** |
| `menu.spin` | Spin | **Nexus+** |
| `killauraEnabled` | Kill aura |  |
| `aopPredictEnabled` | Prediction |  |
| `killauraMainAttack` | Main attack |  |
| `killauraNoWall` | Wall check |  |
| `killauraNoBall` | Ignore ball |  |
| `autododgeEnabled` | Auto dodge |  |
| `aopAimEnabled` | Smart aim |  |
| `isSpinEnabled` | Spin | **Nexus+** |
| `followEnabled` | Follow | **Nexus+** |
| `ballAssistEnabled` | Ball assist | **Nexus+** |
| `holdToShootEnabled` | Hold fire |  |
| `isXrayEnabled` | X-Ray | **Nexus+** |
| `aopTargetMode` | Target mode |  |
| `combatAuraRange` | Attack range |  |
| `combatFireInterval` | Fire interval |  |
| `aopReactionMs` | Reaction delay |  |
| `aopLeadScale` | Lead strength |  |
| `dodgeReactionPercent` | Dodge reaction |  |
| `dodgeSafetyPercent` | Safety margin |  |
| `dodgePowerPercent` | Dodge strength |  |
| `dodgeDistancePercent` | Dodge distance |  |
| `dodgeCommitPercent` | Dodge commitment |  |
| `dodgeBurstHoldMs` | Burst hold |  |
| `dodgeVersion` | Dodge logic |  |
| `menu.dodge_reference` | Ignored attacks |  |
| `xrayShowTargetName` | Target name | **Nexus+** |
| `xrayTargetMode` | Target mode | **Nexus+** |
| `kitNaniModEnabled` | Nani ulti mod | **Nexus+** |
| `coltModEnabled` | Colt mod |  |
| `menu.bolt_mod_config` | Bolt mod | **Nexus+** |
| `boltModEnabled` | Bolt autopilot | **Nexus+** |
| `boltAutoAttackEnabled` | Bolt auto attack | **Nexus+** |
| `boltWallAvoidEnabled` | Avoid walls | **Nexus+** |
| `boltPredictionEnabled` | Bolt prediction | **Nexus+** |
| `boltSafeExitEnabled` | Safe exit | **Nexus+** |
| `boltSmoothPercent` | Steering smoothness | **Nexus+** |
| `boltWallLookahead` | Wall look-ahead | **Nexus+** |
| `boltTargetRange` | Target range | **Nexus+** |
| `boltExitHoldMs` | Exit hold | **Nexus+** |
| `menu.bolt_defaults` | Bolt defaults | **Nexus+** |
| `aopAimTargetMode` | Aim target mode |  |
| `spinSpeed` | Spin speed | **Nexus+** |
| `dynaJumpEnabled` | Dyna jump | **Nexus+** |
| `aopPredictVersion` | Predictor |  |
| `spinMovementMode` | Online movement | **Nexus+** |
| `spinOnlineOnlyIdle` | Only while idle | **Nexus+** |
| `aopAimUltimate` | Aim ultimate | **Nexus+** |
| `aopAimGadget` | Aim gadget | **Nexus+** |
| `HideSuperAim` | Hide super aim |  |
| `ExtendedTrajectory` | Ball trajectory |  |
| `naniAutoDodgeEnabled` | Nani ulti mod | **Nexus+** |
| `naniControlMode` | Nani mode | **Nexus+** |

### Visual (35)

| key | label | paid |
|---|---|---|
| `espEnabled` | ESP | **Nexus+** |
| `characterOutlineEnabled` | Character outline |  |
| `attackRangeIndicator` | Attack range | **Nexus+** |
| `hitboxRenderer` | Hitboxes | **Nexus+** |
| `enemyTracer` | Enemy tracer | **Nexus+** |
| `trophiesAboveHead` | Trophies | **Nexus+** |
| `menu.outline_config` | Character outline |  |
| `outlineOpacity` | Outline opacity |  |
| `nexus_sx_outline_color_preset` | Outline color |  |
| `speedLocalMoveEnabled` | Spectator mode | **Nexus+** |
| `DisablePinAnimation` | Static pins |  |
| `ShowFPSCounter` | FPS counter |  |
| `ShowEnemyAmmoStatus` | Enemy ammo |  |
| `HideBattleBlackBars` | Hide black bars |  |
| `ShowBattleConnectionIndicator` | Ping |  |
| `ShowOwnPlayerCoordinates` | Player coordinates |  |
| `ShowCharactersInNames` | Brawler names |  |
| `ShamePlayersWithThumbsdownPin` | Mark thumbs-down |  |
| `ShowFriendsInBattle` | Mark friends |  |
| `ShowAllianceMembersInBattle` | Mark club members |  |
| `DisableShake` | Disable shake |  |
| `HighlightLeonClone` | Highlight Leon clone |  |
| `ShowDPS` | DPS counter |  |
| `AllyRespawnTimer` | Ally respawn |  |
| `TeammateHPIndicator` | Teammate HP |  |
| `DisableSkins` | Disable skins |  |
| `DefaultEnvironments` | Default environments |  |
| `TileGrid` | Tile grid |  |
| `SmoothHudGraph` | Frame-time graph |  |
| `ShowBattleCameraButton` | Camera |  |
| `ChromaticName` | Chromatic name |  |
| `LegacyNames` | Legacy brawler names |  |
| `HideHomeScreenText` | Hide lobby info |  |
| `ShowSkinNamesInProfile` | Profile skin names |  |
| `Font` | Fonts |  |

### Utility (56)

| key | label | paid |
|---|---|---|
| `menu.follow` | Follow | **Nexus+** |
| `menu.hold` | Hold fire |  |
| `followClosestAllyEnabled` | Closest ally | **Nexus+** |
| `pinEnabled` | Auto pin | **Nexus+** |
| `sprayEnabled` | Auto spray | **Nexus+** |
| `antiAfkEnabled` | Anti-AFK |  |
| `nexus_auto_play_again_enabled` | Auto play again |  |
| `menu.autofarm` | Auto farm | **Nexus+** |
| `autofarmEnabled` | Auto farm | **Nexus+** |
| `nexus_autofarm_post_delay_ms` | Result delay | **Nexus+** |
| `nexus_autofarm_click_gap_ms` | Click interval | **Nexus+** |
| `autofarmAttackEnemies` | Attack enemies | **Nexus+** |
| `nexus_autofarm_follow_target` | Follow ally | **Nexus+** |
| `followDistance` | Follow distance | **Nexus+** |
| `nexus_autofarm_show_stats` | Session stats | **Nexus+** |
| `menu.spectate_with_tag` | Spectate by tag | **Nexus+** |
| `nexus_social_spectate_as_brawltv` | Spectate as Brawl TV | **Nexus+** |
| `menu.visual_brawltv` | Visual Brawl TV | **Nexus+** |
| `menu.invite_by_tag` | Invite by tag | **Nexus+** |
| `holdToShootAim` | Hold aim |  |
| `BattleTextChat` | Battle chat |  |
| `ShowFastPlayAgainButton` | Fast play again |  |
| `BattleEndInstantExit` | Instant battle exit |  |
| `BackgroundMatchmaking` | Background matchmaking |  |
| `DoNotShowBattleHighlight` | Skip highlights |  |
| `EnforceBattleChatButton` | Show chat button |  |
| `ShowFriendlyRoomOpponents` | Friendly opponents |  |
| `SlowMode` | Slow mode |  |
| `HideBattlingStatusFromOthers` | Hide battle status |  |
| `InstantStarrDropOpening` | Instant Starr Drop |  |
| `ProfilePopup` | Player profiles |  |
| `MapEditorPopup` | Map editor |  |
| `editor.OPEN_MAP_EDITOR_POPUP` | Open Map Editor |  |
| `editor.TOGGLE_GRID` | Grid |  |
| `editor.PLACEMENT_NEXT` | Mirror Next |  |
| `editor.PLACEMENT_PREVIOUS` | Mirror Previous |  |
| `editor.PLACEMENT_SET` | Placement Mode |  |
| `editor.UNDO` | Undo |  |
| `editor.REDO` | Redo |  |
| `editor.SELECT_ERASER` | Eraser |  |
| `editor.FILL_ALL` | Fill All |  |
| `editor.FILL_REGION` | Fill Region |  |
| `editor.ERASE_ALL` | Erase All |  |
| `editor.SAVE` | Save Map |  |
| `editor.CLEAR_ALL` | Clear Map |  |
| `editor.REFRESH_TILE_COUNTS` | Refresh Counts |  |
| `editor.BYPASS_SAVE_VALIDATION` | Save Validation Bypass |  |
| `editor.BYPASS_PLACEMENT_ZONES` | Placement Zones Bypass |  |
| `editor.UNLOCK_FULL_PALETTE` | Full Palette |  |
| `editor.MAP_MODIFIERS` | Map Modifiers |  |
| `editor.CANCEL_FILL` | Cancel Fill |  |
| `editor.GO_HOME` | Go Home |  |
| `profile.open` | Open profile |  |
| `profile.name` | Visual name |  |
| `profile.reset_name` | Reset visual name |  |
| `profile.reload` | Reload game |  |

### Settings (81)

| key | label | paid |
|---|---|---|
| `menu.dodge_defaults` | Dodge defaults |  |
| `combatDodgeBlacklistMask.bit0` | Shelly Attack |  |
| `combatDodgeBlacklistMask.bit1` | Shelly Super |  |
| `combatDodgeBlacklistMask.bit2` | Primo Attack |  |
| `combatDodgeBlacklistMask.bit3` | Frank / Rosa |  |
| `combatDodgeBlacklistMask.bit4` | Frank Super |  |
| `combatDodgeBlacklistMask.bit5` | Emz Attack |  |
| `combatDodgeBlacklistMask.bit6` | Buzz Attack |  |
| `combatDodgeBlacklistMask.bit7` | Edgar Attack |  |
| `combatDodgeBlacklistMask.bit8` | Grom Attack |  |
| `combatDodgeBlacklistMask.bit9` | Bull Attack |  |
| `menu.quick_menu_config` | Quick menu |  |
| `nexus_quick_menu_disabled` | Hide quick menu |  |
| `nexus_quick_menu_style` | Button style |  |
| `nexus_quick_menu_preset` | Quick preset |  |
| `nexus_quick_menu_offset_x` | Quick menu X |  |
| `nexus_quick_menu_offset_y` | Quick menu Y |  |
| `menu.activation` | Activation |  |
| `menu.nexus_plus` | Nexus+ |  |
| `UseLowResGraphics` | Max optimization |  |
| `UseBattleProxy` | Battle proxy | **Nexus+** |
| `BattleServersPopup` | Battle server |  |
| `FriendListOptimization` | Light friend list |  |
| `ThemesPopup` | Themes |  |
| `BSDDebugPopup` | BSD debug |  |
| `FPSLimit` | FPS limit |  |
| `debug.ABOUT_SCREEN` | About Screen |  |
| `debug.GENERIC_INFO` | Generic Info |  |
| `debug.ESPORTS` | ESPorts |  |
| `debug.NOTIFICATION_SETTINGS` | Notifications |  |
| `debug.FAME_POPUP` | Fame Preview |  |
| `debug.GOTO_HOME` | Go To Home |  |
| `debug.GOTO_QUESTS` | Go To Quests |  |
| `debug.GOTO_BRAWL_PASS` | Go To Brawl Pass |  |
| `debug.GOTO_CLUBS` | Go To Clubs |  |
| `debug.GOTO_PRO_PASS` | Go To Pro Pass |  |
| `debug.GOTO_CLAN` | Go To Clan |  |
| `debug.GOTO_SCID_REWARDS` | SCID Rewards |  |
| `debug.CHAT_OPTIONS` | Chat Options |  |
| `debug.BRAWLER_UNLOCK_ANIM` | Brawler Reward Preview |  |
| `debug.NOT_ENOUGH_GEMS` | Gems Popup |  |
| `debug.FAME_LEVEL_UP_PREVIEW` | Fame Level Up |  |
| `debug.RANKED_SEASON_END_POPUP` | Ranked Season End |  |
| `debug.MOVIE_PLAYER_POUP` | Movie Player |  |
| `debug.BRAWL_TV` | Brawl TV Intro |  |
| `debug.SHOW_PRESTIGE_INTRO` | Prestige Intro |  |
| `debug.INVITE_FRIEND_CODE` | Invite code |  |
| `debug.UNLOCK_ACCOUNT_SCREEN` | Account unlock |  |
| `debug.SET_COUNTRY` | Country |  |
| `debug.STOP_ALL_SFX` | Stop sound effects |  |
| `debug.STOP_MUSIC` | Stop music |  |
| `debug.PAUSE_MUSIC_TOGGLE` | Pause / resume music |  |
| `debug.MUSIC_VOLUME_CYCLE_0_50_100` | Next music volume |  |
| `debug.BOSS_MUSIC_TOGGLE` | Boss music |  |
| `debug.GUI_CLOSE_ALL_POPUPS` | Close popups |  |
| `debug.GUI_TEST_FLOATER` | Test message |  |
| `debug.OPEN_CLAN_POPUP` | Club window |  |
| `debug.OPEN_TEAMUP_POPUP` | Team window |  |
| `debug.LATENCY_TEST_START` | Latency diagnostics |  |
| `debug.CYCLE_LANGUAGE` | Next language |  |
| `debug.SHOW_TID_KEYS` | Show / hide text keys |  |
| `debug.SKIP_GACHA_ANIM` | Skip gacha animation |  |
| `debug.GFX_QUALITY_CYCLE` | Next graphics quality |  |
| `debug.MEM_QUALITY_CYCLE` | Next memory quality |  |
| `debug.SOFT_RELOAD_GAME` | Reload game |  |
| `debug.AA_DIALOG` | Play time dialog |  |
| `server.automatic` | Automatic |  |
| `server.refresh` | Refresh ping |  |
| `theme.options` | Theme options |  |
| `theme.reset` | Reset theme |  |
| `theme.no_music` | No music |  |
| `theme.confirm` | Apply theme |  |
| `theme.random_launch` | Random at launch |  |
| `theme.random_battle` | Random after battle |  |
| `theme.random_music` | Independent music |  |
| `theme.previous` | Previous theme |  |
| `theme.next` | Next theme |  |
| `theme.shared` | Shared background |  |
| `theme.legacy` | Legacy background |  |
| `menu.language` | Language |  |
| `menu.battle_menu` | Battle Menu |  |

### Nexus+ (4)

| key | label | paid |
|---|---|---|
| `menu.activation_paste` | Paste key |  |
| `menu.activation_submit` | Activate |  |
| `menu.activation_bot` | Activation bot |  |
| `menu.activation_input` | Enter key |  |

## Provenance

- APK: `Nexus-Brawl-69.252_new.apk` (1,850,448,176 bytes,
  SHA-256 `830104fa089268e30c7fb181e867d892805a95234e8bda6d72c44f37d72cee30`)
- Payloads: decrypted from the live delivery server (release ticket revision 137),
  all SHA-256 verified against the signed ticket.
- Pseudocode: Ghidra 11.3.2 headless, DecompInterface export. Not compilable as-is —
  treat as reading source for reconstruction.
