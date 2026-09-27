/* Own module layer. No legacy bootstrap, raw pointers or old game offsets. */
(() => {
    'use strict';
    const nativeCall = __nexusCall, nativeEmit = __nexusEmit;
    delete globalThis.__nexusCall; delete globalThis.__nexusEmit;
    const definitions = new Map(), cache = new Map();
    const valid = id => typeof id === 'string' && /^[A-Za-z0-9_./-]{1,160}$/.test(id)
        && !id.split('/').includes('..');
    const define = (id, dependencies, factory) => {
        if (!valid(id) || definitions.has(id) || !Array.isArray(dependencies)
            || dependencies.some(x => !valid(x)) || new Set(dependencies).size !== dependencies.length
            || typeof factory !== 'function' || definitions.size >= 512)
            throw new TypeError('module_definition_refused');
        definitions.set(id, { dependencies: new Set(dependencies), factory });
    };
    const require = id => {
        if (!valid(id)) throw new TypeError('module_id_refused');
        if (cache.has(id)) return cache.get(id).exports;
        const definition = definitions.get(id);
        if (!definition) throw new Error('module_unavailable:' + id);
        const module = { exports: {} };
        cache.set(id, module); // CommonJS cycles see the partial exports object.
        try {
            const ownRequire = dependency => {
                if (!definition.dependencies.has(dependency)) throw new Error('undeclared_dependency:' + dependency);
                return require(dependency);
            };
            const result = definition.factory.call(module.exports, module, module.exports, ownRequire);
            if (result && typeof result.then === 'function') throw new Error('async_module_factory_unsupported');
            return module.exports;
        } catch (error) { cache.delete(id); throw error; }
    };
    const binding = name => {
        if (typeof name !== 'string' || !/^[A-Za-z][A-Za-z0-9_.-]{0,95}$/.test(name))
            throw new TypeError('binding_name_refused');
        return (...args) => nativeCall(name, ...args);
    };
    Object.defineProperty(globalThis, 'Nexus', { value: Object.freeze({
        apiVersion: 1, define, require, binding,
        emit: (event, value) => nativeEmit(event, value),
        assert: (condition, message = 'assertion_failed') => { if (!condition) throw new Error(message); }
    }), writable: false, configurable: false });
})();
