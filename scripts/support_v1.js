/* Own source ports from pinned static contracts; no legacy factory execution.
 * Historical ModProperties values describe the recovered script schema, not
 * the identity/version or entitlement of the running independent application.
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
