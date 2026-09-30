/* Own source ports from pinned static contracts; no legacy factory execution.
 * Historical ModProperties values describe the recovered script schema, not
 * the identity/version or entitlement of the running independent application.
 *
 * Evidence chain for the contract modules appended below (all re-derived from
 * the decompiled host layer in this repo, no legacy payload executed):
 *  - libNexusScriptRuntime69252.so (core/script_bindings.c): FUN_00135624
 *    bridge bootstrap (0x95a embedded source, 64-entry module registry),
 *    FUN_00135d88 __nexusCall (name + i32-only args, registry/identity gate),
 *    FUN_00135ac0 __nexusEmit (event <= 0x40, value <= 0x180 after ToString,
 *    64 emits per consumer), FUN_00136350 nexus-js-source-v1 ingestion
 *    (decode_ack/parse_ack/eval_ack chain).
 *  - libNexusEvasionRuntime69252.so (core/runtime_core.c) emit sites:
 *    script_port_ready/unavailable, script_port_fonts_ready,
 *    script_port_client_performance, branding (20-event dedup mask).
 *  - core/script_ports.c: camera control ops 1-5 with axis ranges, client
 *    debug apply 0-39 / editor apply 0-19 (only command 9 carries extra
 *    args, command 4 arg <= 4), theme command 0-4, profile tag alphabet
 *    "0289PYLQGRJCUV" base 13 (<= 11 digits, 500 ms reopen cooldown),
 *    snapshot record sizes 0x30/0x1dd0/0x1dd0/0x2534.
 *  - features/visual_tweaks.c: 55-slot tweak key table (49 keys evidenced by
 *    name), NSP69 settings persistence (value 0..1000000, 0x2000 file cap,
 *    tmp+rename+fsync), reset scopes 0/1 (all / Camera*).
 */
Nexus.define('./src/suitcase/config/ModProperties.ts', [], (module, exports) => {
    class ModProperties {
        static isRelease() { return ModProperties.environment === 'release'; }
        static isPlus() { return ModProperties.environment === 'plus'; }
        static isDev() { return ModProperties.environment === 'dev'; }
    }
    ModProperties.environment = 'release';
    ModProperties.version = '68.263-50';
    ModProperties.showPatchNotesFor = '68.263-49';
    ModProperties.isIntegration = false;
    class CustomModNames {}
    CustomModNames.oldRankMod = 'OldRankMod';
    Object.defineProperty(exports, '__esModule', {value: true});
    Object.assign(exports, {ModProperties, CustomModNames});
});

Nexus.define('./src/suitcase/crypto/BASE64.ts', [], (module, exports) => {
    class BASE64 {
        static decode(input) {
            const alphabet = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=';
            const reverse = new Array(256);
            for (let i = 0; i < 64; i++) reverse[alphabet.charCodeAt(i)] = i;
            const padding = input.charAt(input.length - 2) === '=' ? 2 : input.charAt(input.length - 1) === '=' ? 1 : 0;
            const size = Math.ceil(input.length / 4) * 3 - padding;
            const output = new Uint8Array(size);
            for (let i = 0, at = 0; i < input.length;) {
                const a = reverse[input.charCodeAt(i++)], b = reverse[input.charCodeAt(i++)];
                const c = reverse[input.charCodeAt(i++)], d = reverse[input.charCodeAt(i++)];
                output[at++] = (a << 2) | (b >> 4);
                if (at < size) output[at++] = ((b & 15) << 4) | (c >> 2);
                if (at < size) output[at++] = ((c & 3) << 6) | d;
            }
            return output;
        }
        static encode(input) {
            const alphabet = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=';
            let output = '';
            for (let i = 0; i < input.length; i += 3) {
                const a = input[i], b = input[i + 1], c = input[i + 2];
                output += alphabet.charAt(a >> 2) + alphabet.charAt(((a & 3) << 4) | (b >> 4))
                    + (i + 1 < input.length ? alphabet.charAt(((b & 15) << 2) | (c >> 6)) : '=')
                    + (i + 2 < input.length ? alphabet.charAt(c & 63) : '=');
            }
            return output;
        }
    }
    Object.defineProperty(exports, '__esModule', {value: true});exports.BASE64 = BASE64;
});

Nexus.define('./src/suitcase/crypto/CustomTextEncoder.ts', [], (module, exports) => {
    class CustomTextEncoder {
        constructor(text) { this.instance = CustomTextEncoder.encode(text); }
        static encode(text) {
            const bytes = [];
            for (let i = 0; i < text.length; i++) {
                let c = text.charCodeAt(i);
                if (c >= 0xd800 && c <= 0xdbff && i + 1 < text.length) {
                    const next = text.charCodeAt(i + 1);
                    if (next >= 0xdc00 && next <= 0xdfff) { c = 0x10000 + ((c - 0xd800) << 10) + next - 0xdc00; i++; }
                }
                if (c < 0x80) bytes.push(c);
                else if (c < 0x800) bytes.push(0xc0 | (c >> 6), 0x80 | (c & 63));
                else if (c < 0x10000) bytes.push(0xe0 | (c >> 12), 0x80 | ((c >> 6) & 63), 0x80 | (c & 63));
                else bytes.push(0xf0 | (c >> 18), 0x80 | ((c >> 12) & 63), 0x80 | ((c >> 6) & 63), 0x80 | (c & 63));
            }
            return new Uint8Array(bytes);
        }
        static decode(input) {
            const bytes = new Uint8Array(input);let result = '', i = 0;
            while (i < bytes.length) {
                const a = bytes[i++];
                if (a < 0x80) { result += String.fromCharCode(a); continue; }
                if (a < 0xe0) {
                    const b = bytes[i++];if (b === undefined) break;
                    result += String.fromCharCode(((a & 31) << 6) | (b & 63));continue;
                }
                if (a < 0xf0) {
                    const b = bytes[i++], c = bytes[i++];if (b === undefined || c === undefined) break;
                    result += String.fromCharCode(((a & 15) << 12) | ((b & 63) << 6) | (c & 63));continue;
                }
                const b = bytes[i++], c = bytes[i++], d = bytes[i++];
                if (b === undefined || c === undefined || d === undefined) break;
                const point = ((a & 7) << 18) | ((b & 63) << 12) | ((c & 63) << 6) | (d & 63);
                if (point <= 0xffff) result += String.fromCharCode(point);
                else { const rest = point - 0x10000;result += String.fromCharCode(0xd800 + (rest >> 10), 0xdc00 + (rest & 1023)); }
            }
            return result || '';
        }
        static toArrayBuffer(input) { const result = new ArrayBuffer(input.length);new Uint8Array(result).set(input);return result; }
    }
    Object.defineProperty(exports, '__esModule', {value: true});exports.CustomTextEncoder = CustomTextEncoder;
});

// Explicit independent data adapter. It does not load the old EDebugger,
// FileManager, native logging hooks or their entry/service dependencies.
Nexus.define('nexus/config/log-store', [], (module) => { module.exports = {logsObject: {}}; });
Nexus.define('./src/suitcase/filesystem/Json.ts', ['./src/suitcase/config/ModProperties.ts', 'nexus/config/log-store'], (module, exports, require) => {
    const {ModProperties} = require('./src/suitcase/config/ModProperties.ts');
    const store = require('nexus/config/log-store');
    class Json {
        static formatJSONString(value, spaces = ModProperties.environment === 'dev' ? 4 : 0) { return JSON.stringify(value, null, spaces); }
        static formatLogJSON(type, message) {
            store.logsObject[Date.now()] = {type: ['ERROR', 'WARNING'][type], message};
            return Json.formatJSONString(store.logsObject, 4);
        }
    }
    Object.defineProperty(exports, '__esModule', {value: true});exports.Json = Json;
});

Nexus.define('./src/suitcase/logic/LogicTime.ts', [], (module, exports) => {
    class LogicTime {
        static timestampToDate(timestamp) {
            const pad = (value, width = 2) => ''.concat(new Array(width).fill(0), value).slice(-width);
            const date = new Date(timestamp);
            return `${pad(date.getDate())}.${pad(date.getMonth() + 1)}.${pad(date.getFullYear(), 4)} (dd.mm.yyyy)\n  ${pad(date.getHours())}:${pad(date.getMinutes())}:${pad(date.getSeconds())} (hh:mm:ss)`;
        }
        static humanizeTime(value) {
            const seconds = value % 60, minutes = Math.floor(value / 60) % 60;
            const hours = Math.floor(value / 3600) % 24, days = Math.floor(value / 86400), parts = [];
            if (days > 0) parts.push(`${days}\u0434`);
            if (hours > 0) parts.push(`${hours}\u0447`);
            if (minutes > 0 || parts.length > 0) parts.push(`${minutes}\u043c`);
            parts.push(`${seconds}c`);return parts.join(' ');
        }
    }
    Object.defineProperty(exports, '__esModule', {value: true});exports.LogicTime = LogicTime;
});

Nexus.define('./src/suitcase/modmenu/debugmenu/DebugRecents.ts', [], (module, exports) => {
    class DebugRecents {
        static push(label) {
            const index = DebugRecents.labels.indexOf(label);
            if (index >= 0) DebugRecents.labels.splice(index, 1);
            DebugRecents.labels.unshift(label);
            if (DebugRecents.labels.length > 8) DebugRecents.labels.length = 8;
            DebugRecents.listeners.forEach(listener => listener());
        }
        static getAll() { return DebugRecents.labels.slice(); }
        static clear() { DebugRecents.labels.length = 0;DebugRecents.listeners.forEach(listener => listener()); }
        static onChange(listener) { DebugRecents.listeners.push(listener); }
    }
    DebugRecents.labels = [];DebugRecents.listeners = [];
    Object.defineProperty(exports, '__esModule', {value: true});exports.DebugRecents = DebugRecents;
});

Nexus.define('./src/suitcase/modmenu/debugmenu/DebugSearch.ts', [], (module, exports) => {
    class DebugSearch {
        static getQuery() { return DebugSearch.query; }
        static setQuery(query) {
            const normalized = query.trim().toLowerCase();if (normalized === DebugSearch.query) return;
            DebugSearch.query = normalized;DebugSearch.listeners.forEach(listener => listener());
        }
        static clear() { DebugSearch.setQuery(''); }
        static matches(label) { return DebugSearch.query.length === 0 || label.toLowerCase().includes(DebugSearch.query); }
        static onChange(listener) { DebugSearch.listeners.push(listener); }
    }
    DebugSearch.query = '';DebugSearch.listeners = [];
    Object.defineProperty(exports, '__esModule', {value: true});exports.DebugSearch = DebugSearch;
});

/* Native event vocabulary and host protocol constants, ported from the emit
 * sites in the decompiled modules. The native side delivers every event to
 * the Java loader through the host record() callback as "fixture_event" with
 * an "event:value" payload; the JS side only ever sees Nexus.emit (JS to
 * native). Names, payload shapes and limits below mirror the recovered
 * snprintf formats byte-for-byte so client code can parse them back.
 */
Nexus.define('nexus/runtime-events', [], (module, exports) => {
    const EVENTS = Object.freeze({
        SCRIPT_PORT_READY: 'script_port_ready',
        SCRIPT_PORT_UNAVAILABLE: 'script_port_unavailable',
        SCRIPT_PORT_FONTS_READY: 'script_port_fonts_ready',
        SCRIPT_PORT_CLIENT_PERFORMANCE: 'script_port_client_performance',
        SCRIPT_PORT_OPTIONS: 'script_port_options',
        SCRIPT_PORT_EXTENDED_READY: 'script_port_extended_ready',
        BATTLE_PROXY_BINDING: 'battle_proxy_binding',
        BRANDING: 'branding',
        RUNTIME_FIXTURE: 'runtime_fixture'
    });
    // Value payloads are the exact recovered formats:
    //   client_performance: "ready=%u hooks=%u expected=3/15"
    //   options:            ",\"skins_mask\":%u,\"environment\":%u,\"shake\":%u"
    //   extended_ready:     "installed=%u expected=63"
    //   battle_proxy:       ",\"hook_mask\":%u,\"external_provider_required\":true"
    //   branding:           label text / "label_created" / "label_refused"
    const SOURCES = Object.freeze({
        SCRIPT_PORT_FONTS_READY: 'native_init_and_assets',
        BATTLE_CONNECTION: 'battle_connection_indicator',
        BATTLE_PROXY: 'v69_original_BSD_protocol',
        SCRIPT_PORT_OPTIONS: 'v69_skin_and_location_pipeline'
    });
    // Host ack chain for a nexus-js-source-v1 script (FUN_00136350).
    const ACKS = Object.freeze({
        DECODE: 'decode_ack', DECODE_DETAIL: 'strict_utf8_source',
        PARSE: 'parse_ack', PARSE_DETAIL: 'quickjs_compile_only',
        EVAL: 'eval_ack', EVAL_DETAIL: 'synchronous_source_and_pending_jobs_completed',
        FIXTURE: 'fixture_event'
    });
    const LIMITS = Object.freeze({
        API_VERSION: 1,
        EVENT_NAME_MAX: 0x40,          // charset [A-Za-z0-9._-], ".." refused
        EVENT_VALUE_MAX: 0x180,        // after ToString coercion
        EMITS_PER_CONSUMER: 0x40,      // hard cap, then "emit_limit"
        BRANDING_EVENTS: 0x14,         // dedup via a seen-bitmask first
        MODULE_REGISTRY: 0x40,         // bridge module normalizer slots
        DEFINITIONS: 512,              // bridge define() cap
        BINDING_NAME_MAX: 96           // ^[A-Za-z][A-Za-z0-9_.-]{0,95}$
    });
    module.exports = {EVENTS, SOURCES, ACKS, LIMITS};
});

/* Client-side port of the native script-port surface (core/script_ports.c,
 * features/visual_tweaks.c). The __nexusCall dispatcher only accepts the
 * binding name plus pure int32 arguments, so this module owns the client-side
 * halves of the contracts: value ranges, command legality, tag parsing and
 * settings-file shape. Everything that crosses the bridge goes through
 * call() with pre-validated int32 arguments.
 */
Nexus.define('nexus/script-port', ['nexus/runtime-events'], (module, exports, require) => {
    const {LIMITS} = require('nexus/runtime-events');

    const I32_MIN = -2147483648, I32_MAX = 2147483647;
    // Mirrors FUN_00135d88: every argument must be a finite integral number
    // inside the int32 range (5.0 is accepted, 5.5 / NaN / Infinity are not).
    const toI32 = value => {
        if (typeof value !== 'number' || !Number.isFinite(value) || Math.trunc(value) !== value
            || value < I32_MIN || value > I32_MAX)
            throw new TypeError('binding_i32_required');
        return value;
    };
    const call = (name, ...args) => Nexus.binding(name)(...args.map(toI32));

    // 55-slot tweak key table; 49 keys are evidenced by name in the
    // availability cascade / snapshot readers, the remaining slots are
    // Camera-family keys living in the raw gl_hooks.c data tail.
    const KEYS = Object.freeze([
        'AllyRespawnTimer', 'BackgroundMatchmaking', 'BattleEndInstantExit',
        'BattleServerRegion', 'BattleServersPopup', 'BattleTextChat',
        'CameraMode', 'CameraTilt', 'CameraX', 'CameraY', 'CameraZoom', 'CameraZ',
        'ChromaticName', 'DefaultEnvironments', 'DisablePinAnimation',
        'DisableShake', 'DisableSkins', 'DoNotShowBattleHighlight',
        'EnforceBattleChatButton', 'ExtendedTrajectory', 'FPSLimit', 'Font',
        'FriendListOptimization', 'HideBattleBlackBars',
        'HideBattlingStatusFromOthers', 'HideHomeScreenText', 'HideSuperAim',
        'HighlightLeonClone', 'InstantStarrDropOpening', 'LegacyNames',
        'ShamePlayersWithThumbsdownPin', 'ShowAllianceMembersInBattle',
        'ShowBattleCameraButton', 'ShowBattleConnectionIndicator',
        'ShowCharactersInNames', 'ShowDPS', 'ShowEnemyAmmoStatus',
        'ShowFPSCounter', 'ShowFastPlayAgainButton', 'ShowFriendlyRoomOpponents',
        'ShowFriendsInBattle', 'ShowOwnPlayerCoordinates', 'ShowSkinNamesInProfile',
        'SlowMode', 'SmoothHud', 'SmoothHudGraph', 'TeammateHPIndicator',
        'TileGrid', 'UseBattleProxy'
    ]);
    const SETTINGS = Object.freeze({
        FILE: 'script-port-settings.v1',
        HEADER: 'NSP69 1\n',
        VALUE_MAX: 1000000,          // load-side clamp; set-side uses per-key max
        FILE_MAX: 0x2000,
        LINE: (key, value) => `${key}=${value}\n`
    });
    const keyIndex = key => KEYS.indexOf(key) + 1;   // 0 = unknown, like tweak_index()
    const isCameraKey = key => key.startsWith('Camera');

    // nexus_script_port_camera_control(op, axis, value): ops 1-5.
    //   1 set axis (axis 0 = zoom 0..200, axes 1-4 = X/Y/Z/tilt +-10000)
    //   2 set mode (0..3)  3 toggle zoom 60<->100  4 reset  5 commit
    const CAMERA = Object.freeze({
        OP_SET: 1, OP_MODE: 2, OP_TOGGLE_ZOOM: 3, OP_RESET: 4, OP_COMMIT: 5,
        AXES: Object.freeze(['zoom', 'x', 'y', 'z', 'tilt']),
        RANGE: Object.freeze([
            {min: 0, max: 200}, {min: -10000, max: 10000}, {min: -10000, max: 10000},
            {min: -10000, max: 10000}, {min: -10000, max: 10000}
        ]),
        DEFAULT_ZOOM: 100, TOGGLE_ZOOM: 60,
        cameraControl(op, axis, value) {
            if (op < CAMERA.OP_SET || op > CAMERA.OP_COMMIT) return 0;
            if (op === CAMERA.OP_SET) {
                if (axis < 0 || axis > 4) return 0;
                const range = CAMERA.RANGE[axis];
                if (value < range.min || value > range.max) return 0;
            } else if (op === CAMERA.OP_MODE && value > 3) return 0;
            return call('nexus_script_port_camera_control', op, axis, value);
        }
    });

    // Client debug popup: 40 slots, actionId = 0x27020 (159776) + slot.
    // Only command 9 carries the extra (arg, a3, a4, a5) tuple; command 4
    // takes an enum arg in 0..4. Snapshot records are 0x1dd0 bytes.
    const DEBUG = Object.freeze({
        ACTION_BASE: 0x27020, COMMANDS: 40, SNAPSHOT_SIZE: 0x1dd0,
        ARG_COMMAND: 9, ENUM_COMMAND: 4, ENUM_MAX: 4,
        actionId(slot) { return DEBUG.ACTION_BASE + slot; },
        apply(command, arg = 0, a3 = 0, a4 = 0, a5 = 0) {
            if (command > DEBUG.COMMANDS - 1) return 0;
            if (command !== DEBUG.ARG_COMMAND && (a4 !== 0 || a3 !== 0 || a5 !== 0)) return 0;
            if (command === DEBUG.ENUM_COMMAND && arg > DEBUG.ENUM_MAX) return 0;
            return call('nexus_script_port_client_debug_apply', command, arg, a3, a4, a5);
        }
    });

    // Map editor popup: 20 slots, actionId = 0x28FC0 (167968) + slot.
    // Same argument rules as debug; commands 0x0e..0x10 are toggles,
    // command 0 = confirm, 0x12 = flush.
    const EDITOR = Object.freeze({
        ACTION_BASE: 0x28FC0, COMMANDS: 20, SNAPSHOT_SIZE: 0x1dd0,
        ARG_COMMAND: 9, ENUM_COMMAND: 4, ENUM_MAX: 4,
        TOGGLE_FIRST: 0x0e, TOGGLE_LAST: 0x10, COMMAND_CONFIRM: 0, COMMAND_FLUSH: 0x12,
        actionId(slot) { return EDITOR.ACTION_BASE + slot; },
        apply(command, arg = 0, a3 = 0, a4 = 0, a5 = 0) {
            if (command > EDITOR.COMMANDS - 1) return 0;
            if (command !== EDITOR.ARG_COMMAND && (a4 !== 0 || a3 !== 0 || a5 !== 0 || arg !== 0)) return 0;
            if (command === EDITOR.ENUM_COMMAND && arg > EDITOR.ENUM_MAX) return 0;
            return call('nexus_script_port_client_editor_apply', command, arg, a3, a4, a5);
        }
    });

    // Theme store: commands 0-4, 'THEM' snapshots of 0x2534 bytes holding at
    // most 8 named entries of 0x128 bytes each, paged by max_themes.
    const THEME = Object.freeze({
        COMMANDS: 5, SNAPSHOT_SIZE: 0x2534, MAGIC: 0x4d454854, // 'THEM'
        NAMED_MAX: 8, ENTRY_SIZE: 0x128,
        command(command, arg = 0) {
            if (command > THEME.COMMANDS - 1) return 0;
            return call('nexus_script_port_theme_command', command, arg);
        }
    });

    // Player-profile tag: raw length 0x21..0x40, trimmed of whitespace with an
    // optional '#' prefix, then 1..11 digits over the 13-char alphabet.
    // Re-opens are refused within 500 ms. Snapshots are 'CPRO' records of
    // 0x1dd0 bytes paging 0x20 entries of 0xcc bytes from from_index.
    const PROFILE = Object.freeze({
        ALPHABET: '0289PYLQGRJCUV', BASE: 13, DIGITS_MAX: 11,
        RAW_MIN: 0x21, RAW_MAX: 0x40, REOPEN_COOLDOWN_MS: 499,
        SNAPSHOT_SIZE: 0x1dd0, MAGIC: 0x4f525043, // 'CPRO'
        PAGE_ENTRIES: 0x20, ENTRY_SIZE: 0xcc,
        parseTag(raw) {
            if (typeof raw !== 'string' || raw.length < PROFILE.RAW_MIN || raw.length > PROFILE.RAW_MAX)
                return null;
            let start = 0, end = raw.length;
            const space = c => c === '\n' || c === '\r' || c === ' ' || c === '\t';
            while (start < end && space(raw[start])) start++;
            while (end > start && space(raw[end - 1])) end--;
            if (raw[start] === '#') start++;
            const digits = raw.slice(start, end);
            if (digits.length === 0 || digits.length > PROFILE.DIGITS_MAX) return null;
            let value = 0;
            for (const ch of digits) {
                const digit = PROFILE.ALPHABET.indexOf(ch);
                if (digit < 0) return null;
                value = value * PROFILE.BASE + digit;
            }
            return {tag: digits, value};
        }
    });

    // Battle snapshot: 0x30 record, phase 5 = not ready / wrong thread,
    // phase 4 = unstable frame, phase 2 = kind 5 without sub-frame.
    const BATTLE = Object.freeze({SNAPSHOT_SIZE: 0x30, PHASE_UNSTABLE: 4, PHASE_CLOSED: 5, PHASE_NO_SUB: 2});

    module.exports = Object.freeze({
        LIMITS, toI32, call,
        KEYS, SETTINGS, keyIndex, isCameraKey,
        CAMERA, DEBUG, EDITOR, THEME, PROFILE, BATTLE
    });
});
