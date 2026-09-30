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
| `anti_afk.c` | 3 | **RECONSTRUCTED** — `anti_afk_plan_tick()` (context snapshot + movement detection dist-sq>63 + plan queue submit with callback record + JSON log), `anti_afk_plan_ready()` (plan validation: epoch/context match, 500ms window, scaled-position tolerance 1e-5, 350ms cooldown), `anti_afk_flag_clear()` (clears bit 0 of the game AFK flag at +0x40 after probe-word verification) |
| `aura.c` | 8 | **RECONSTRUCTED** — 3 snapshot providers under handler lock (`prediction` 0x88: 17 property reads + port bitmap + triple-gate flags + port-gate epoch/serial; `hold` 0xc0: 28 property reads + holdfire gates a/b + epoch/aux/aux2/aux3; `aura` 0x48: 11 property reads + adapter mask), route installer `nexus_evasion_runtime_aura_route_v1()` (one-shot exchange lock, consumer fn install/uninstall, frame-callback registration), `aura_frame_callback()` (56-byte frame-record parse + mirror, 2s rate-limited `aura_phase` journal, -1/0/1/consumer dispatch), `aura_state_logger()` (wrapper-call counters, 1s rate-limited `aura_state` journal with weapon record), `aura_adapter_bootstrap()` (7 dlsym exports each dladdr-verified, image bind + frame record install + 14-qword frame family with 8 callbacks + family template), `JNI_OnLoad()` (proc-stat starttime epoch, NexusLoader context/package/files-dir verification, exclusive journal file, phdr game/paired/BuildID/SHA checks, ng bind, adapter bootstrap, function routes, ng entry write with fatal-abort path, NativeLoaded ack callback, 7 handler installs, full runtime teardown with periodic stop + telemetry zeroing + GL disable + event route restore) |
| `autododge.c` | 23 | **RECONSTRUCTED** — the full auto-dodge stack across all 3 modules. Evasion control plane: `evasion_set_value()` (the set dispatcher: 142-entry table walk, entitlement/pre-exempt gates, visual-only skip, forces-pass adapter registry, pin/spray + movement registry with entitlement, speed direct pass, holdfire/spin keys, brawler bitmap + port-slot deep check vs predict entry with f24 gate, index-based coercion cascade {dodgeVersion 1-5 else 3 @135, target-mode 2-else-1 @137-139, speed level 4 @136}, follow-family alias sync @41-45, speedExploit/speedLocalMove mutual exclusion, revision bump), `nexus_evasion_functions_snapshot_v1()` (0xb8 triple: registry words, 10 adapter-ready bits, status, 10 port bits, 26 property values + 6 mangled dodge-profile slots dodge%s / v%dDodge%s), `evasion_action_lease_submit()` (actions 22/23/26/46/68/121/122/132 → port families, aim modes 1/4 vs movement 2/3, family key check incl. autododge→kitNani fallback, 0x28 lease record with token 1e6-2e6 + mask 0xff/0x3f, 11-qword out record), `nexus_evasion_dodge_profile()` (6-slot clamped fill under state lock), `nexus_autododge_set_enabled()`, `nexus_autododge_set_replay_block()`, `nexus_evasion_resolve_slot()` (slots 5-8), `evasion_menu_backend_dodge()` (0x10000+ query-table triples and raw 0-0xa7 property slots with dodge key mangling + dodgeBlacklistMask bit extraction for keys 0x5f-0x68), `dodge_key_resolve()`, `dodge_version_set_param()`. EvasionRuntime engine: `runtime_journal_write()` (NexusLab69252 log + bounded capture journal with aura/dodge 0x140 vs default 0x120 line limits), `evasion_backend_bind()` (dlopen/dlsym/dladdr owner-verified, dodgeVersion 1-5 + profile probe), `evasion_game_frame()` (THE frame pump: capture-request file protocol with fstat stability + journal reset, Mainloop thread gate, ng_feed/ng_status/ng_projectiles, 11 timed subsystem pumps with max trackers, consumer dispatch, frame journal with projectile list, 120-frame NexusPerf aggregation), `functions_route_register()` (two route records, tag 0x1000003f7 vs 0x100000003 by dodge_bound, smartaim_dodge_v1_v5_follow journal), `dodge_bind()` (guest ELF + phdr owner + 6-region actor contract checksums, dual mmap state with SHA-pinned init, action-22 dodge_bound/dodge_disabled journals), `dodge_tick()` (single-consumer tick: own-entity/team/projectile-array validation with pointer-range gates, per-gid classify into 8 projectile records + 48 poison slots, wall/own types 0x121fff8/0x12205d0/0x12206a8, ignore-mask compaction, state publish, rate-limited dodge_frame/dodge_phase journals with raw projectile dump), `projectile_classify()` (autododge-ignore.bin NRI1 refresh every 250ms with rol5 checksum chain, object-graph name resolve with charset gate, 319-entry AlternatorHealProjectile binary search + 128-bit mask bit test), `ng_feed_v1()` (passive game-state feed: full graph walk frame→session→world→projectiles with 9 reject reasons, own-entity stamp/vtable/ping validation, per-projectile threat collection into 64 slots, stable-frame counter → passive_ready, atomic state publish), `ng_diagnostics()`. UI: `ui_ignore_mask_write()`/`ui_ignore_mask_read()` (32-byte mask file with tmp+rename+fsync, dual checksum chains), `ui_dodge_key_normalize()` (nexus_dodge_ → nexus_v%d_dodge_ remap over the 118-key UI table), `ui_dodge_version_current()` |
| `autofarm.c` | 8 | **RECONSTRUCTED** — `nexus_evasion_snapshot_keys_v1()` (THE snapshot engine behind every gate: 142-slot property walk, per-key triple fill {requested, effective, flags}, port bitmap + predict gate, Nexus+/pin/brawler family gates, force-key pass), `nexus_evasion_handler_status_v1()` (0x48 status dispatcher: pin/spray, Nexus+, holdfire, spin, port and adapter-family branches with the shared 18-int status layout), `autofarm_armed()`, `autofarm_write_verify()`, `autofarm_attack_tick()` (fire-interval clamp 120-420ms, frame-change reset, plan seed, target resolve + team check, hp/threat/dist decide, engine+0xecfb74 aim check, wall probes, guarded write, engine+0xb2e994 attack with JSON log), `pin_status_query()` (4-key pin/spray + delay ranges), `autofarm_play_again()` (dlsym-free engine vtable chain: static+0x13053c0 -> vtable+0x11ebcd0 -> fn+0xac319c, alloc 0x98/init/send), `ui_persist_key()` |
| `ball.c` | 6 | **RECONSTRUCTED** — ball-assist family: `ball_intercept_compute()` (12-unit approach vector toward the ball with ±60000 bounds), `ball_assist_position()` (guarded remote write of the intercept XY with restore-verify-abort), `ball_assist_held_adjust()` (held-ball trajectory pipeline + dispatch tag {1,0x60}), `ball_write_verify()`, `ball_assist_frame_tick()`, `ball_assist_state_snapshot()` (one-shot latch + 29-qword state mirror) |
| `bolt_mod.c` | 5 | **RECONSTRUCTED** — bolt autopilot: `evasion_key_adapter_family()` (25-key adapter family), `bolt_auto_attack_tick()` (armed-marker gate 17000010, plan-tag {2,0,0,0} check, 849ms cooldown, enemy-list walk with hypot range + aim-dot checks, guarded remote write + game+0xb2e994 attack wrapper with JSON log), `bolt_mod_armed()`, `bolt_write_verify()`, and `bolt_movement_tick()` (the 0x700-line wall-avoidance pathing engine: kitNani section gate, 8-key bolt snapshot, wall-grid validity, velocity smoother over 1-8 ticks with 20-unit/s cap, enemy scan with threat-table age scoring 0.55/tick + 0.72 same-target hysteresis, publish/h reacquire counter, colt section with aim extrapolation + wall_avoid_step/adjust, autofarm section with the 56-byte farm records + threat decay 1-0.08*age clamped 0.15 + LOS checks + target pick, speedExploit handoff) |
| `brawler_mods.c` | 8 | **RECONSTRUCTED** — `brawler_mod_key_family()` (visual-vs-combat classifier), `colt_movement_apply()` (grid/epoch gates, remote object fetch + resolve, colt_avoid_step with hold counter, aim-velocity extrapolation clamped 5-16 units, guarded write with colt_xy_restore_unverified abort), `colt_write_verify()`, `dyna_jump_armed()` (marker 16000009 + state byte), `movement_restore_verify()` (speedExploit 0x2e / kitNani 0x79 sections, signature blob check, 0.05f/saved-value apply + readback + cascading restore with movement_restore_unverified abort), `dyna_engine_tick()` (query struct + target resolve, 501ms window, engine+0xb3d64c submit with reentrancy guards), `brawler_overlay_render()` (ESP wireframe: 3-row affine projection with z-offset far face, 12-line box, tracer, 90-entry brawler marker->name table), `overlay_section_lookup()` |
| `debug_menu.c` | 8 | **RECONSTRUCTED** — BSD debug-menu bridge with the full recovered 40-command table (ABOUT SCREEN...PLAY TIME DIALOG, slot 4 = value entry): `nexus_menu_debug_open/action/pump/diagnostics` — action ids 0x27000-0x27120, 40-bit slot bitmap + 256-bit command availability bitmap, 6-phase pump (idle/value/poll/queued/apply/done) with digit parsing, dialog lifecycle, result reporting; JSON diagnostics |
| `esp.c` | 1 | **RECONSTRUCTED** — `evasion_key_is_visual_only()`: the ESP feature-family classifier consumed by the core activation gates (espEnabled / espShowTracer / espTeamFilter*) |
| `follow.c` | 2+118 | **RECONSTRUCTED** — UI provider pump + the full 118-slot UI query table recovered from .data.rel.ro (nexus_sx_* / nexus_vis_* / nexus_dodge_* keys with min/max/defaults): `ui_provider_poll()` walks 8 backend slots x 118 keys with range validation into the shadow values + revision counter, then refreshes all 168 action records; `ui_values_set_key()` bulk setter |
| `hold_fire.c` | 8 | **RECONSTRUCTED** — `evasion_key_active()` (fast key-active check with all family gates), `nexus_evasion_hold_register_v1()` (signed-module registration: 32-bit-half-swap magic vs 0x104810, dladdr anti-tamper on 5 fn ptrs, registry snapshot + consistency, verify/template checks, idempotent re-register via 0x58 memcmp, full state-blob publish), `holdfire_gate_b()` (aim/range gate with record[10] bit mask + aopPredictVersion 1..2 + aopAimEnabled port gate), `nexus_evasion_hold_lease_v1()` + `hold_lease_impl()` (action 0x1d, slots 1-3, describe-fn lease with mask/val checks), `nexus_evasion_hold_recheck_v1()` (0x60 memcmp), `holdfire_bootstrap()` (dlsym 5 exports + fname/base verification, guest identity, event routes, provider guards, registration record), `holdfire_fire_tick()` (main-thread gate, snapshot, policy resolve, super/main dispatch, bounded-held-policy logging with dedup) |
| `killaura.c` | 4 | **RECONSTRUCTED** — `evasion_key_forces_pass()` (7 always-pass keys), `evasion_brawler_bitmap_index()` (3-bit aim bitmap: killauraEnabled/killauraMainAttack/aopPredictEnabled over first 69 feature keys), `evasion_aura_route_compose()` (route tag {1,0x28} + epoch + 3 registry identities, gated on adapter-ready bits + prediction version + status), `killaura_channels_snapshot()` (6-key triple snapshot: autofarm/killaura/super/gadget channels with HP-threshold safety and 120ms fire-interval clamp) |
| `map_editor.c` | 8 | **RECONSTRUCTED** — hidden map-editor unlock with the recovered 20-command table (OPEN MAP EDITOR...GO HOME, slot 4 = placement mode 0-4, slot 9 = rectangle x1,y1,x2,y2): `nexus_menu_editor_open/action/pump/scroll_revision` — action ids 0x29000-0x29200, 5-phase pump with review banners, editor session guard, fill-cmd family (8/9/10/12/19) review flow, screen-closing cmds {0,15,19}, result messages |
| `outline.c` | 5 | **RECONSTRUCTED** — character outline: `outline_register_observers()` (2 SHA-256-guarded code observers at game+0x7ff030/+0x7c7c44), `outline_environment_observer()` + `outline_scene_observer()` (RGBA channel validation vs 1/255 scale + byte quantized rewrite, log-once latches + stage-indexed refusal slots), `outline_context_fetch()` (6-stage context: battle frame kind 4/5 + 5-key color snapshot), `outline_shader_patch()` — the GLSL injector: `uniform highp vec4 u_nexusOutline;` before `void main` + each `vec4(v_outlineColor, 1.0)` replaced by `(u_nexusOutline.a < 0.0 ? vec4(v_outlineColor, 1.0) : u_nexusOutline)` |
| `prediction.c` | 3 | **RECONSTRUCTED** — predictor-version gating: `evasion_prediction_alias_ok()` (generic property gate for the gates fallback), `evasion_prediction_version_ok()` (aopTargetMode in [0,3), predict-off fast pass, aopPredictVersion in 1..2 under dep-bit 2), `evasion_prediction_dependency_ok()` wrapper |
| `smart_aim.c` | 4 | **RECONSTRUCTED** — `smartaim_input_hook()` (action-23 input hook: event identity vs engine+0xb2e994, colt interplay, config/ability gating for ultimate(2)/gadget(3), input lease 0x17, aim proposal engine, xy sentinel match, guarded xy commit with input_xy_restore_unverified abort + android log, rate-limited JSON), `smartaim_skill_hook()` (kind-4 skill route classification by caller engine+0xb2d5c8/0xb2d3d4/0xb380e0, skill context resolve, register lease, 0x2c0 state publish, out record), `smartaim_skill_log()`, `smartaim_frame_update()` (map/world refresh: blob chain validation engine+0x1307e20, region/ctrl checks, full 0x1d0 context-block plumbing, world-change detection + map clear, route install, evidence flags 0x1a/0x1b/0x4/0x20, standalone_aim_feed counters log) |
| `spectate.c` | 5 | **RECONSTRUCTED** — spectate/BrawlTV family: `spectate_state_reset()`, `spectate_name_hook()` (12-byte name tag patch via remote write, restore-verified), `spectate_camera_mode_hook()` (return-address check game+0xb3062c, +0x92c mode word patch 0/4), `spectate_camera_transform_hook()` (3 camera modes incl. sin36 BrawlTV orbit, config-tag dirty check, calls game+0x671630 with replaced vectors), `spectate_brawltv_enter()` (session resolve + game+0x11a2830 call) |
| `speed_fly.c` | 3 | **RECONSTRUCTED** — `nexus_script_port_client_performance_apply()` (FPSLimit < 145 via script port, errno-preserving); `speed_hook_body()` = the BL-hook body injected at game+0xf86758: forwards the original call to game+0xe7af70, verifies hook word + trampoline identity, gates on speedExploitEnabled triple + plan coherence, then replays the game movement-advance (value/50 units + (value%50)*20 sub-units) within the pacing budget; `projectile_controller_switch()` (ulti/speedy projectile controller handoff with name validation) |
| `spin.c` | 18 | **RECONSTRUCTED** — `prop_entry_lookup()` (142-entry table walk with 0x60 length gate), `nexus_evasion_get_port_state()` (force-key fast path 1, port-gate slot f32: deep check under lock -> 2/0 for feature keys <69, 1 for param keys, 3 closed), `spin_status_fill()` (isSpinEnabled 0x48 status: spin_active + mode gate code cascade 1-5), `port_status_fill()` (port-id gated fill with 10-bit gate flags + version bitmap), `nexus_evasion_spin_snapshot_v1()` (0x58: 4 spin props + family epoch/aux + 0x168 rotation), `spin_action_lease_submit()` (action-25 lease: mode eligibility {1 vs 2/3}, 0x28 lease record with flag-mask + token 1e6-2e6 validation, 12-qword out record), `port_gate_check()` (ports 0-9: aim ports 0/1 with holdfire interplay + aim-pair bitmap + predict version 3-4, brawler ports 5-8 with family gate + key active, spin default with dodge version 1-5 + idle chain, ports 4/9 via port-once), `evasion_query_restore()` (118-slot query table walk with min/max bounds -> restore dispatch), 3 export thunks, `spin_exports_bind()` (8 dlsym exports dladdr-verified, guest identity, provider install 0x1000, online movement, dual code islands: frame RVA 0xb30690 + movement RVA 0xe7b768, BL encoding with ±128MB range check, memcpy + icache flush + mprotect RX + game_read readback + memcmp verify, publish with fatal-abort restore path), `spin_handler_install()` (post proof validation, action-25 register record with 9 fields, LAB_TRIAL scope), `spin_post_callback()` (errno-safe action-25 executor: telemetry mirror, identity pair check, exec + field apply + provider publish, rate-limited spin_post journal), `loader_identity_verify()` (movement+frame post restore with fatal aborts), `spin_move_callback()` (online circle movement: entity resolve, frame record parse, angle advance dt*rate*1e-7 with 2pi wrap, sincosf target, 23-tick apply window with error states 5/7, spin_move journal), `handlers_install_main()` (JNI lab-loader handshake: nexus/loader/d+e field chain, linking/EvasionGame phase strings, profileSha 114ba105... verification, revision/localAllowed/transport/gameSha checks, module list walk for jni-onload-v1 EvasionGame bytes, self-path SHA via dladdr+game_image_sha_verify, plan-revision equality, loader generation latch) |
| `trophies.c` | 7 | **RECONSTRUCTED** — the trophies-above-head subsystem. `trophies_install()` (the master bootstrap: 5+1 queue-guard SHA regions with optional native-fields flag, 10 touch guards + 0xcf5044 flag + 3 more, 4 spectator islands with B/BL hook words + ±128MB reach checks + align-rounded imm26 + icache flush + mprotect RX + publish-with-restore-abort, battle_text_chat hook at 0xf39550 (STP 0xa9be7bfd prologue gate), extended_ball_trajectory pair at 0xb444a0/0xb4071c (0x6db63bef LDP gate) + social_name_metadata, native_teammate_arrow at 0x86bdfc (0xd10343ff SUB gate), ally_respawn flag, speed BL hook at 0xf86758 (expects 0x94000000|imm to 0xe7af70), AFK-visual 0x158-byte ng observer template at 0x8640fc with 3 patched branch slots + template B entry, outline guards, hud guards, 13 visual installs + 2 guarded hooks 0x821428/0x85d6d8, xray mmap state + queue plan at 0xaa2830 with 0x208 template + owner registration via nexus_evasion_register_restored_v1 + fatal restore-abort chain), `trophies_battle_tick()` (roster read at battle+0x38 arrays with 0x40/0x200 bounds, own-index gate, 19-word roster entries, per-gid entity classify into the 64-slot player table with dedup mask, epoch stability revalidation, capture-reason journal 0-4), `trophies_capture_journal()` (2s rate-limited v69_battle_intro_total with roster/known/actors/epoch/legacy sums), `trophies_hud_guards_install()` (23-region body guards with 0x5921a0 dl_iterate_phdr owner discovery, 6 vtable guards 0x11be3f0/8/0x400→0x814068/0x814130/0x8142e8 + 0x11ad300→0x5d6594 + 0x11ad210→0x5d48e8 + 0x11abae0→0x58d1c4, 2 owner hooks at 0x8142e8/0x814068 → state mask 3), `trophies_owner_release()` (64-slot HUD field release: refcount +0x38/id -1 checks at +0x40, game release calls, dedup clear), `trophies_hud_update_callback()` (THE HUD update hook: view/own/projectile graph validation with gate counters 1-0x1b, trophiesAboveHead snapshot gates, roster/name cross-checks with own-hit == 1, donor node adoption from popover_text_left + icon_trophy styles with gold 0xffd700 color, font 0x10, scale 1, visibility writes, position -6/-20 or -21/-13 with team -45 offset, text update via owner-verified set-text, ShowEnemyAmmoStatus +0xb48 extra), `plusapi_users_fetch()` (the plusapi client: tag-mask JSON body fully \xNN-hex-escaped, POST /bsd/api/v1/get_bsd_users_by_mask to 87.228.126.116:80 Host plusapi.bsd.meowfox.net, response 2-stage unescape, per-tag trophies/character extraction, mutex-guarded cache compare with all-field match, complete flag + inflight clear) |
| `visual_tweaks.c` | 60 | — exports: `nexus_script_port_query()`, `nexus_script_port_set()`, `nexus_script_port_reset()`, `nexus_script_port_camera_snapshot()`, `nexus_script_port_hud_snapshot()`, `nexus_script_port_chat_snapshot()` +5 more |
| `xray.c` | 3 | **RECONSTRUCTED** — ShadowX bridge: `nexus_shadowx_set_feature/set_param()` over the core dispatcher; `shadowx_snapshot_fetch()` pulls the 4-key triple snapshot (isXrayEnabled, xrayShowTargetName, xrayTargetMode, aopAimEnabled) via `nexus_evasion_snapshot_keys_v1` with flags==ACTIVE checks, Nexus+ entitlement gate through the delivery-module validator, 0x78 descriptor with {100,200} marker constants |

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
| `game_state_readers.c` | 12 | **RECONSTRUCTED** (replaces raw pseudocode) — libNexusEvasionRuntime69252.so game-memory access layer with the binary's real lookup tables recovered via relocation parsing (`docs/reader_tables.json`): 246-entry weapon table (damage + elite), 246-entry alternator damage table, 26-phase near-alloc state names, ulti controller kinds — near-mirror allocator (ADRP-range mmap ring scan + /proc/self/maps gap placement), rw-mapping locator, player-name branding restore (SHA-256 ABI gate over 13 game offsets), progressive tile-map wall scanner (120x120 bitmap, 512 tiles/tick), weapon controller reader with target dedup and damage/elite cross-validation, ult/speed projectile controller registry; gcc -Wall -Wextra clean |
| `evasion_api.c` | 46 | **RECONSTRUCTED** (split out of gl_hooks.c) — libNexusEvasion69252.so published v1 API: state layer over the 142-key property sheet (69 feature keys gated by activation dependencies, 73 params read raw), batch restore with range/alias-conflict validation, generation-counted registry protocol (publish → validate via dladdr same-module + epoch match → double-checked commit under the state spinlock) for observer / handlers / visual / periodic / restored / event-routes-v2 / both spin posts / functions table with required-adapter mask, package identity recheck ("bsd.suitcase.nexusv2"), bind_image with 7 guarded game-code regions + 10-entry anchor patch table + branch-island trampoline verifier recovered from the binary (`docs/evasion_bind_guards.json`), frame observation pump walking game+0x1307e20 frame objects, JSON diagnostics; gcc -Wall -Wextra clean |
| `script_ports.c` | 25 | **RECONSTRUCTED** (split out of gl_hooks.c) — EvasionRuntime script-port bridges: camera control (5-axis clamped state machine), battle snapshot (frame-object stability dedup with epoch tagging, owner-thread gating), profile open/set_name/snapshot (base-13 tag codec "0289PYLQGRJCUV", NPCN69 atomic-rename persistence, 0x1dd0 cache with 32×0xcc brawler records), client debug/editor apply bridges (40+20 command surfaces), theme snapshot (0x2534, 8×0x128 theme slots), runtime template/frame-consumer/diagnostics/capture/frame registrars; gcc -Wall -Wextra clean |
| `gl_hooks.c` | 124 | **MOSTLY RECONSTRUCTED** — the EvasionRuntime visual pipeline. ng_* observer family + GL hook installer (see prior note) plus: remote_write (process_vm_writev "vm" / pwrite64 /proc/pid/mem "proc" with mremap page-replacement fallback for 4-byte aligned writes, stage labels range/maps/alloc/copy/seal/remap); ChaCha20 stream crypt pair (20 rounds, hardcoded 48-byte seed, counter at word 4) used for {magic_number, signature, data} JSON envelopes; three SHA-256 clones + HMAC-SHA256 message-id derivation (IPC magic 0x1c2d3e4f, 15-bit ids) and custom-pad stable-id HMAC; SDF angular-coverage kernel (N-direction sincos sampling of multi-contour rounded-polygon SDF, per-contour offsets at +0x87c/0x3c stride, up-to-8 greedy max-spread direction selection = attack-range/aim-cone sectors); overlay render frame pump (eglSwapBuffers hook: EGL/GL revalidation, scissored-clear rect drawing with redundant-state suppression, state restore + 16-frame verify, NexusPerfGL 120-frame rolling logger); HUD builders (ESP boxes/snaplines, waypoint markers with direction arrows, nameplates with red→green progress bars, FPS + 90-sample frametime histogram, coordinates, RESPAWN banner, DPS/ping readouts); NDC1/game/entity snapshot ingest, remote entity fetch, autododge threat tracker (24 slots), SuperAim (80-target priority pool, dual lead prediction), trajectory exposure accumulator, motion tracker EMA; functions-route boot (dlsym+dladdr anti-spoof, 15-region SHA-256 guard), ability command dispatcher (game fn @0xb3d64c with magic 0x5f3f356b), ESP subsystem boot, periodic pin/spray tick, env override (game+0x1308444→3), theme select/apply + camera target, aura dispatch family, profile base-13/14 tag codec, plusapi HTTP chunked client, GLLCAL descriptor parser, game-call blockers and accessors. Still raw: guarded-hook install plumbing family (FUN_0014a9a0…FUN_001a8430, 23 blocks) |
| `memory_observers.c` | 8 | **RECONSTRUCTED** (replaces raw pseudocode) — ng observer trampoline installer from libNexusEvasionRuntime69252.so: the 0x158-byte full-context ARM64 trampoline template recovered from .rodata (STP X0-X30 + Q0-Q31 save/restore, body call slot +0x150, replaced-anchor word slot +0x144, B-return slot +0x148) with slot offsets from relocation addends; ng_prepare_observer_v1 plan contract (0x30-byte plan: entry/home/resume/original word/enter branch — B-instruction encoding with imm26 sign fixup and ±128MB reach checks, stages plan_contract..resume); ng_install_observer home allocator (1MB-step mmap ring ±111MB, then /proc/self/maps gap scan offering page-aligned candidates outside the 20MB game image, MAP_FIXED placement, mprotect RX seal, phase-cell TLS diagnostics with all 26 stage labels); ng_observe_registers_v1 feed record (magic/counter/X0-X2) into ng_feed_v1; ng_remote_store_pair guarded 8-byte write to game addr+0xfac via the process_vm_writev/pwrite64 remote-write channel; JNI_OnUnload closing the two binder fds and unbinding; gcc -Wall -Wextra clean |
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
