/* Own source reimplementations of the recovered arithmetic contracts.
 * Evidence: frozen IR functions 946..948 and 2653..2658. No native APIs,
 * legacy evaluator, startup patches or copied factory bytecode are executed.
 *
 * These two modules stay deliberately pure (no Nexus.binding / Nexus.emit
 * calls, no module side effects) so they can be evaluated by any conforming
 * QuickJS build without the Nexus host: the recovered IR only encodes plain
 * arithmetic and string formatting. Boundary behaviours below are part of
 * the contract and were kept even where they look odd:
 *  - GlobalID.getClassID uses Math.round, so ids produced by a foreign
 *    encoder with a negative instance component round towards the nearest
 *    class instead of truncating.
 *  - LogicColor.generateColorArray starts its interpolation parameter at
 *    t = 1/count on the first element and never reaches exact 1; the mix
 *    direction is b -> a as t grows (first argument is the END color).
 *  - LogicColor.argbToIntString takes percent components (0..100), clamps
 *    nothing itself, and emits AARRGGBB upper-case hex.
 * Module ids keep the original suitcase tree layout because the bridge
 * normalizer (FUN_001358cc) keys its 64-slot registry by these strings.
 */
Nexus.define('./src/laser/client/game/utils/GlobalID.ts', [], (module, exports) => {
    'use strict';
    class GlobalID {
        static createGlobalID(classID, instanceID) { return instanceID % 1000000 + 1000000 * classID; }
        // The actual recovered method uses round, including its boundary behavior.
        static getClassID(id) { return Math.round(id / 1000000); }
        static getInstanceID(id) { return id % 1000000; }
    }
    Object.defineProperty(exports, '__esModule', {value: true});
    exports.GlobalID = GlobalID;
});
Nexus.define('./src/suitcase/logic/LogicColor.ts', [], (module, exports) => {
    'use strict';
    class LogicColor {
        static argbToIntString(rgba) {
            const a = Math.round(rgba[3] * 255 / 100).toString(16).padStart(2, '0');
            const r = Math.round(rgba[0] * 255 / 100).toString(16).padStart(2, '0');
            const g = Math.round(rgba[1] * 255 / 100).toString(16).padStart(2, '0');
            const b = Math.round(rgba[2] * 255 / 100).toString(16).padStart(2, '0');
            return (a + r + g + b).toUpperCase();
        }
        static rgbToInt(rgb) { return (rgb[0] << 16) + (rgb[1] << 8) + rgb[2]; }
        static intToRGB(value) { return [(value & 0xff0000) >> 16, (value & 0xff00) >> 8, value & 255]; }
        static generateColorArray(first, second, count) {
            const a = this.intToRGB(first), b = this.intToRGB(second), output = [];
            let t = 0;
            for (let i = 0; i < count; i++) {
                t += 1 / count;
                output.push(this.rgbToInt([a[0] * t + (1 - t) * b[0],
                    a[1] * t + (1 - t) * b[1], a[2] * t + (1 - t) * b[2]]));
            }
            return output;
        }
        static lerp(a, b, t) { return a + (b - a) * t; }
        static lerpColor(first, second, t) {
            const [r1, g1, b1] = this.intToRGB(first), [r2, g2, b2] = this.intToRGB(second);
            return this.rgbToInt([Math.round(this.lerp(r1, r2, t)),
                Math.round(this.lerp(g1, g2, t)), Math.round(this.lerp(b1, b2, t))]);
        }
    }
    Object.defineProperty(exports, '__esModule', {value: true});
    exports.LogicColor = LogicColor;
});
