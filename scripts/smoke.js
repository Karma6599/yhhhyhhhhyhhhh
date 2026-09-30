/* Own lab fixture; this acknowledges VM/source execution only.
 *
 * Exercises the exact surface the recovered host layer provides, in the order
 * the host provides it (core/script_bindings.c):
 *   1. the frozen Nexus API shape installed by nexus-own-bridge.js
 *   2. module define/require with the bridge rejection paths
 *   3. engine sanity (typed arrays, BigInt) on the pinned QuickJS build
 *   4. the bridge-side binding-name gate (native i32 rules live behind it)
 *   5. one fixture emit, delivered to the loader as "fixture_event"
 * No bindings that touch game state are invoked and no gameplay is proven:
 * the fixture must stay green on a bare host with zero bindings registered.
 */
Nexus.assert(Nexus.apiVersion === 1, 'runtime_api');
Nexus.assert(Nexus.require === undefined || typeof Nexus.require === 'function', 'runtime_require');
Nexus.assert(Nexus.emit === undefined || typeof Nexus.emit === 'function', 'runtime_emit');

Nexus.define('nexus/runtime-fixture', [], (module) => { module.exports = {answer: 6 * 7}; });
Nexus.assert(Nexus.require('nexus/runtime-fixture').answer === 42, 'module_execution');

// Rejection paths from the bridge: duplicate ids, invalid ids, undeclared
// dependencies and non-function factories must all refuse loudly.
const refused = probe => { try { probe(); return false; } catch (error) { return true; } };
Nexus.assert(refused(() => Nexus.define('nexus/runtime-fixture', [], () => {})), 'duplicate_refused');
Nexus.assert(refused(() => Nexus.define('../escape', [], () => {})), 'id_refused');
Nexus.assert(refused(() => {
    Nexus.define('nexus/undeclared-dep', ['./missing'], (module, exports, require) => require('./missing'));
    Nexus.require('nexus/undeclared-dep');
}), 'undeclared_refused');
Nexus.assert(refused(() => Nexus.define('nexus/not-a-factory', [], 'nope')), 'factory_refused');

// Bridge binding gate: names must match ^[A-Za-z][A-Za-z0-9_.-]{0,95}$.
// Obtaining a binding only wraps the native call, so this stays safe on a
// bare host; invoking it without a registered backend is not probed here.
Nexus.assert(refused(() => Nexus.binding('9bad/name')), 'binding_name_refused');
Nexus.assert(typeof Nexus.binding('nexus_script_port_camera_control') === 'function', 'binding_ok');

Nexus.assert(new Uint8Array([258])[0] === 2, 'typed_array');
Nexus.assert(2 ** 53 - 2 ** 53 % 1 === 2 ** 53, 'float_int');
Promise.resolve().then(() => {
    Nexus.assert(2n ** 65n === 36893488147419103232n, 'bigint');
    // The single fixture emit: within the recovered limits (event <= 0x40,
    // value <= 0x180 after ToString, 64 emits per consumer).
    Nexus.emit('runtime_fixture', {api: 1, sourceEvaluated: true, legacyBundleEvaluated: false, gameplayProven: false});
});
