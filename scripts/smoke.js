/* Own lab fixture; this acknowledges VM/source execution only. */
Nexus.assert(Nexus.apiVersion === 1, 'runtime_api');
Nexus.define('nexus/runtime-fixture', [], (module) => { module.exports = {answer: 6 * 7}; });
Nexus.assert(Nexus.require('nexus/runtime-fixture').answer === 42, 'module_execution');
Nexus.assert(new Uint8Array([258])[0] === 2, 'typed_array');
Promise.resolve().then(() => {
    Nexus.assert(2n ** 65n === 36893488147419103232n, 'bigint');
    Nexus.emit('runtime_fixture', {api: 1, sourceEvaluated: true, legacyBundleEvaluated: false, gameplayProven: false});
});
